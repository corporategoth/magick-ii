#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_convert_ircservices_cpp_ ## x () { return y; }
RCSID(convert_ircservices_cpp, "@(#)$Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#ifdef CONVERT

/* Based upon the DB loading routines from ircservices 5.0.9
 * (c) 1996-2002 Andrew Church <achurch@dragonfire.net>
 */

#include "magick.h"
#include "convert/interface.h"
#include "convert/ircservices.h"

/* Define this if the DB's use MD5 encryption ... */
#undef ircservices_DB_USES_ENCRYPTION

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

/* Maximum length of a channel name, including the trailing null.  Any
 * channels with a length longer than ircservices_CHANMAX-1 (including the leading #)
 * will not be usable with ChanServ. */
#define ircservices_CHANMAX		64

/* Maximum length of a nickname, including the trailing null.  This MUST be
 * at least one greater than the maximum allowable nickname length on your
 * network, or people will run into problems using Services!  The default
 * (32) works with all servers I know of. */
#define ircservices_NICKMAX		32

/* Maximum length of a password */
#define ircservices_PASSMAX		32

#define ircservices_CHANMAX_DEFAULT	-1
#define ircservices_TIMEZONE_DEFAULT	0x7FFF

#define ircservices_SAFE(x) do { if ((x) < 0) goto fail; } while (0)
#define ircservices_ARRAY_FOREACH(x,y) for (x=0; x < y##_count; x++)

struct ircservices_SuspendInfo {
    char who[ircservices_NICKMAX];	/* who added this suspension */
    char *reason;
    time_t suspended;
    time_t expires;	/* 0 for no expiry */
};

/* Memo info structures.  Since both nicknames and channels can have memos,
 * we encapsulate memo data in a MemoList to make it easier to handle. */

struct ircservices_Memo {
    uint32 number;	/* Index number -- not necessarily array position! */
    int16 flags;
    time_t time;	/* When it was sent */
    char sender[ircservices_NICKMAX];
    char *text;
};

#define ircservices_MF_UNREAD	0x0001	/* ircservices_Memo has not yet been read */
#define ircservices_MF_EXPIREOK	0x0002	/* ircservices_Memo may be expired */

struct ircservices_MemoInfo {
    ircservices_Memo *memos;
    int16 memos_count;
    int16 memomax;
};

/* Data structure for a single nickname. */

int ircservices_next_id;
map<int, mstring> ircservices_groupmap;

struct ircservices_NickInfo {
    char nick[ircservices_NICKMAX];

    int16 status;	/* See NS_* below */
    char *last_usermask; /* Last user@host mask (uses fakehost if avail.) */
    char *last_realmask; /* Last user@realhost (not fakehost) mask seen */
    char *last_realname;
    char *last_quit;
    time_t time_registered;
    time_t last_seen;

    uint32 nickgroup;	/* ID of nick group for this nick */
};

/*************************************************************************/

/* Data for a group of nicks. */

struct ircservices_NickGroupInfo {
    uint32 id;
    uint16 nicks_count;	/* Number of nicks in this nick group */
    uint16 mainnick;	/* Index of "main" nick (for chan access lists etc) */

    char pass[ircservices_PASSMAX];
    char *url;
    char *email;

    ircservices_SuspendInfo *suspendinfo;	/* Suspension info (non-NULL => suspended) */

    int32 flags;	/* See NF_* below */

    int16 language;	/* Language selected by nickname owner (LANG_*) */
    int16 timezone;	/* Offset from UTC, in minutes */
    int16 channelmax;	/* Maximum number of registered channels allowed */

    char **access;	/* Array of access masks */
    int16 access_count;	/* # of entries */

    ircservices_MemoInfo memos;
};

struct ircservices_NickExt
{
    bool group;

    // standard ...
    char *last_realmask; /* Last user@realhost (not fakehost) mask seen */
    uint32 id_stamp;	/* Services stamp of user who last ID'd for nick */

    int32 flags;	/* See NF_* below */
    int32 authcode;	/* Authorization code (used by mail-auth module) */
    time_t authset;	/* Time authcode set (used by mail-auth module) */
    int16 channelmax;	/* Maximum number of registered channels allowed */
    char **ajoin;	/* Array of autojoin channel */
    int16 ajoin_count;	/* # of entries */
    int16 memomax;
    char **ignore;	/* Array of memo ignore entries */
    int16 ignore_count;	/* # of entries */
    int16 language;	/* Language selected by nickname owner (LANG_*) */
    int16 timezone;	/* Offset from UTC, in minutes */
    char *info;
    int16 os_priv;	/* OperServ privilege level; see NP_* below */
};

#define ircservices_LANG_EN_US      0       /* United States English */
#define ircservices_LANG_UNUSED1    1       /* Unused; was Japanese (JIS encoding) */
#define ircservices_LANG_JA_EUC     2       /* Japanese (EUC encoding) */
#define ircservices_LANG_JA_SJIS    3       /* Japanese (SJIS encoding) */
#define ircservices_LANG_ES         4       /* Spanish */
#define ircservices_LANG_PT         5       /* Portugese */
#define ircservices_LANG_FR         6       /* French */
#define ircservices_LANG_TR         7       /* Turkish */
#define ircservices_LANG_IT         8       /* Italian */
#define ircservices_LANG_DE         9       /* German */
#define ircservices_LANG_NL         10      /* Dutch */
#define ircservices_LANG_HU         11      /* Hungarian */

#define ircservices_NUM_LANGS       12      /* Number of languages */
#define ircservices_DEF_LANGUAGE    0      /* "Use the default" setting */

/*************************************************************************/

/* Nickname status flags: */
#define ircservices_NS_VERBOTEN	0x0002      /* Nick may not be registered or used */
#define ircservices_NS_NOEXPIRE	0x0004      /* Nick never expires */
/* The following flags are temporary: */
#define ircservices_NS_KILL_HELD	0x8000      /* Nick is being held after a kill */
#define ircservices_NS_GUESTED	0x4000	    /* SVSNICK has been sent but nick has not
				     * yet changed.  An enforcer will be
				     * introduced when it does change. */
#define ircservices_NS_TEMPORARY	0xC000      /* All temporary flags */

/* Nickgroup setting flags: */
#define ircservices_NF_KILLPROTECT	0x00000001  /* Kill others who take this nick */
#define ircservices_NF_SECURE	0x00000002  /* Don't recognize unless IDENTIFY'd */
#define ircservices_NF_MEMO_HARDMAX	0x00000008  /* Don't allow user to change memo limit */
#define ircservices_NF_MEMO_SIGNON	0x00000010  /* Notify of memos at signon and un-away */
#define ircservices_NF_MEMO_RECEIVE	0x00000020  /* Notify of new memos when sent */
#define ircservices_NF_PRIVATE	0x00000040  /* Don't show in LIST to non-servadmins */
#define ircservices_NF_HIDE_EMAIL	0x00000080  /* Don't show E-mail in INFO */
#define ircservices_NF_HIDE_MASK	0x00000100  /* Don't show last seen address in INFO */
#define ircservices_NF_HIDE_QUIT	0x00000200  /* Don't show last quit message in INFO */
#define ircservices_NF_KILL_QUICK	0x00000400  /* Kill in 20 seconds instead of 60 */
#define ircservices_NF_KILL_IMMED	0x00000800  /* Kill immediately instead of in 60 sec */
#define ircservices_NF_MEMO_FWD	0x00001000  /* Forward memos to E-mail address */
#define ircservices_NF_MEMO_FWDCOPY	0x00002000  /* Save copy of forwarded memos */

/* Nickgroup OperServ privilege levels: */
#define ircservices_NP_SERVOPER	0x1000
#define ircservices_NP_SERVADMIN	0x2000

/* Access levels for users. */
struct ircservices_ChanAccess {
    char *nickgroup;	/* Zero if entry is not in use */
    int16 level;
};

/* Note that these two levels also serve as exclusive boundaries for valid
 * access levels.  ACCLEV_FOUNDER may be assumed to be strictly greater
 * than any valid access level, and ACCLEV_INVALID may be assumed to be
 * strictly less than any valid access level.
 */
#define ircservices_ACCLEV_FOUNDER	1000	/* Numeric level indicating founder access */
#define ircservices_ACCLEV_INVALID	-1000	/* Used in levels[] for disabled settings */

/* Access levels used to represent AOP's, SOP's and VOP's in channel access
 * lists. */

#define ircservices_ACCLEV_SOP	100
#define ircservices_ACCLEV_AOP	50
#define ircservices_ACCLEV_HOP	40
#define ircservices_ACCLEV_VOP	30

/*************************************************************************/

/* ircservices_AutoKick data. */
struct ircservices_AutoKick {
    char *mask;		/* NULL if not in use */
    char *reason;
    char who[ircservices_NICKMAX];
    time_t set;
    time_t lastused;
};

/*************************************************************************/

/* Data for a registered channel. */

struct ircservices_ChanInfo {
    char name[ircservices_CHANMAX];
    char *founder;
    char *successor;		/* Who gets the channel if founder nick
				 * group is dropped or expires */
    char founderpass[ircservices_PASSMAX];
    char *desc;
    char *url;
    char *email;

    time_t time_registered;
    time_t last_used;
    char *last_topic;		/* Last topic on the channel */
    char last_topic_setter[ircservices_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;	/* When the last topic was set */

    int32 flags;		/* See below */
    ircservices_SuspendInfo *suspendinfo;	/* Non-NULL iff suspended */

    int16 *levels;		/* Access levels for commands */

    ircservices_ChanAccess *access;		/* List of authorized users */
    int16 access_count;
    ircservices_AutoKick *akick;		/* List of users to kickban */
    int16 akick_count;

#ifdef CONVERT_DB
    char *mlock_on, *mlock_off;	/* Strings of mode characters */
#else
    int32 mlock_on, mlock_off;	/* See channel modes below */
#endif
    int32 mlock_limit;		/* 0 if no limit */
    char *mlock_key;		/* NULL if no key */

    char *entry_message;	/* Notice sent on entering channel */

    ircservices_MemoInfo memos;
};

struct ircservices_ChanExt
{
    int16 memomax;
    char *mlock_link;		/* +L (Unreal) */
    char *mlock_flood;		/* +f (Unreal) */
    int16 *levels;		/* Access levels for commands */
};

/* Retain topic even after last person leaves channel */
#define ircservices_CI_KEEPTOPIC	0x00000001
/* Don't allow non-authorized users to be opped */
#define ircservices_CI_SECUREOPS	0x00000002
/* Hide channel from ChanServ LIST command */
#define ircservices_CI_PRIVATE	0x00000004
/* Topic can only be changed by SET TOPIC */
#define ircservices_CI_TOPICLOCK	0x00000008
/* Those not allowed ops are kickbanned */
#define ircservices_CI_RESTRICTED	0x00000010
/* Don't auto-deop anyone */
#define ircservices_CI_LEAVEOPS	0x00000020
/* Don't allow any privileges unless a user is IDENTIFY'd with NickServ */
#define ircservices_CI_SECURE	0x00000040
/* Don't allow the channel to be registered or used */
#define ircservices_CI_VERBOTEN	0x00000080
/* Channel does not expire */
#define ircservices_CI_NOEXPIRE	0x00000200
/* Channel memo limit may not be changed */
#define ircservices_CI_MEMO_HARDMAX	0x00000400
/* Send notice to channel on use of OP/DEOP */
#define ircservices_CI_OPNOTICE	0x00000800
/* Enforce +o, +v modes (don't allow deopping) */
#define ircservices_CI_ENFORCE	0x00001000
/* Hide E-mail address from INFO */
#define ircservices_CI_HIDE_EMAIL	0x00002000
/* Hide last topic from INFO */
#define ircservices_CI_HIDE_TOPIC	0x00004000
/* Hide mode lock from INFO */
#define ircservices_CI_HIDE_MLOCK	0x00008000

/*************************************************************************/

/* Indices for ci->levels[]: (DO NOT REORDER THESE unless you hack
 * database modules to deal with them) */
#define ircservices_CA_INVITE	0
#define ircservices_CA_AKICK	1
#define ircservices_CA_SET		2	/* but not FOUNDER or PASSWORD */
#define ircservices_CA_UNBAN	3
#define ircservices_CA_AUTOOP	4
#define ircservices_CA_AUTODEOP	5	/* Maximum, not minimum */
#define ircservices_CA_AUTOVOICE	6
#define ircservices_CA_OPDEOP	7	/* ChanServ commands OP and DEOP */
#define ircservices_CA_ACCESS_LIST	8
#define ircservices_CA_CLEAR	9
#define ircservices_CA_NOJOIN	10	/* Maximum */
#define ircservices_CA_ACCESS_CHANGE 11
#define ircservices_CA_MEMO		12
#define ircservices_CA_VOICE	13	/* VOICE/DEVOICE commands */
#define ircservices_CA_AUTOHALFOP	14
#define ircservices_CA_HALFOP	15	/* HALFOP/DEHALFOP commands */
#define ircservices_CA_AUTOPROTECT	16
#define ircservices_CA_PROTECT	17
#define ircservices_CA_AUTOOWNER	18	/* dummy entry for auto +q */
#define ircservices_CA_KICK		19
#define ircservices_CA_STATUS	20
#define ircservices_CA_TOPIC	21

#define ircservices_CA_SIZE		22

#define ircservices_CMODE_i		0x00000001
#define ircservices_CMODE_m		0x00000002
#define ircservices_CMODE_n		0x00000004
#define ircservices_CMODE_p		0x00000008
#define ircservices_CMODE_s		0x00000010
#define ircservices_CMODE_t		0x00000020
#define ircservices_CMODE_k		0x00000040
#define ircservices_CMODE_l		0x00000080


int16 ircservices_def_levels[ircservices_CA_SIZE] = {
	50,		// ircservices_CA_INVITE
	100,	// ircservices_CA_AKICK
	ircservices_ACCLEV_FOUNDER,		// ircservices_CA_SET
	50,		// ircservices_CA_UNBAN
	50,		// ircservices_CA_AUTOOP
	-1,		// ircservices_CA_AUTODEOP
	30,		// ircservices_CA_AUTOVOICE
	50,		// ircservices_CA_OPDEOP
	0,		// ircservices_CA_ACCESS_LIST
	100,	// ircservices_CA_CLEAR
	-100,	// ircservices_CA_NOJOIN
	40,		// ircservices_CA_ACCESS_CHANGE
	100,	// ircservices_CA_MEMO
	30,		// ircservices_CA_VOICE
	40,		// ircservices_CA_AUTOHALFOP
	40,		// ircservices_CA_HALFOP
	100,	// ircservices_CA_AUTOPROTECT
	100,	// ircservices_CA_PROTECT
	ircservices_ACCLEV_FOUNDER,		// ircservices_CA_AUTOOWNER
	50,		// ircservices_CA_KICK
	100,	// ircservices_CA_STATUS
	50		// ircservices_CA_TOPIC
};

struct ircservices_MaskData {
    char *mask;
    int16 limit;	/* For exceptions only */
    char *reason;
    char who[ircservices_NICKMAX];
    time_t time;
    time_t expires;	/* Or 0 for no expiry */
    time_t lastused;	/* Last time used, 0 if never used */
};

/* Types of mask data: */
#define ircservices_MD_AKILL	0
#define ircservices_MD_EXCLUSION	1
#define ircservices_MD_EXCEPTION	2

struct ircservices_NewsItem {
    int16 type;
    int32 num;		/* Numbering is separate for login and oper news */
    char *text;
    char who[ircservices_NICKMAX];
    time_t time;
};

/* Constants for news types. */

#define ircservices_NEWS_INVALID	-1	/* Used as placeholder */
#define ircservices_NEWS_LOGON	0
#define ircservices_NEWS_OPER	1

/* Callers may access fields in this structure directly, but MUST NOT
 * modify them. */
struct ircservices_dbFILE {
    int mode;			/* 'r' for reading, 'w' for writing */
    FILE *fp;			/* The file pointer itself */
    char filename[PATH_MAX+1];	/* Name of the database file */
    char tempname[PATH_MAX+1];	/* Name of the temporary file (for writing) */
};

#define ircservices_read_db(f,buf,len)	(fread((buf),1,(len),(f)->fp))
#define ircservices_getc_db(f)		(fgetc((f)->fp))

#define ircservices_read_buffer(buf,f)	(ircservices_read_db((f),(buf),sizeof(buf)) == sizeof(buf) ? 0 : -1)
#define ircservices_read_buflen(buf,len,f)	(ircservices_read_db((f),(buf),(len)) == (len) ? 0 : -1)
#define ircservices_read_variable(var,f)	(ircservices_read_db((f),&(var),sizeof(var)) == sizeof(var) ? 0 : -1)

int32 ircservices_get_file_version(ircservices_dbFILE *f)
{
    FILE *fp = f->fp;
    int version = fgetc(fp)<<24 | fgetc(fp)<<16 | fgetc(fp)<<8 | fgetc(fp);
    if (ferror(fp)) {
#ifndef CONVERT_DB
	SLOG(LM_ERROR, "Error reading version number on %s",( f->filename));
#endif
	return -1;
    } else if (feof(fp)) {
#ifndef CONVERT_DB
	SLOG(LM_ERROR, "Error reading version number on %s: End of file detected",(
		   f->filename));
#endif
	return -1;
    } else if (version < 1) {
#ifndef CONVERT_DB
	SLOG(LM_ERROR, "Invalid version number (%d) on %s",( version, f->filename));
#endif
	return -1;
    }
    return version;
}

static ircservices_dbFILE *ircservices_open_db_read(const char *filename)
{
    ircservices_dbFILE *f;
    FILE *fp;

    f = (ircservices_dbFILE *) malloc(sizeof(*f));
    *f->tempname = 0;
    strncpy(f->filename, filename, sizeof(f->filename));
    f->mode = 'r';
    fp = fopen(f->filename, "rb");
    if (!fp) {
	int errno_save = errno;
#ifndef CONVERT_DB
	if (errno != ENOENT)
	    SLOG(LM_ERROR, "Can't read database file %s",( f->filename));
#endif
	free(f);
	errno = errno_save;
	return NULL;
    }
    f->fp = fp;
    return f;
}

ircservices_dbFILE *ircservices_open_db(const char *filename, const char *mode, int32 version)
{
    static_cast<void>(version);
    if (*mode == 'r') {
	return ircservices_open_db_read(filename);
    } else {
	errno = EINVAL;
	return NULL;
    }
}

int ircservices_close_db(ircservices_dbFILE *f)
{
    fclose(f->fp);
    free(f);
    return 0;
}

int ircservices_read_int8(uint8 *ret, ircservices_dbFILE *f)
{
    int c = fgetc(f->fp);
    if (c == EOF)
	return -1;
    *ret = c;
    return 0;
}

inline int ircservices_read_int16(uint16 *ret, ircservices_dbFILE *f)
{
    int c1, c2;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    if (c2 == EOF)
	return -1;
    *ret = c1<<8 | c2;
    return 0;
}

inline int ircservices_read_int16(int16 *ret, ircservices_dbFILE *f)
{
	return ircservices_read_int16((uint16 *) ret, f);
}

int ircservices_read_int32(uint32 *ret, ircservices_dbFILE *f)
{
    int c1, c2, c3, c4;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    c3 = fgetc(f->fp);
    c4 = fgetc(f->fp);
    if (c4 == EOF)
	return -1;
    *ret = c1<<24 | c2<<16 | c3<<8 | c4;
    return 0;
}

inline int ircservices_read_int32(int32 *ret, ircservices_dbFILE *f)
{
	return ircservices_read_int32((uint32 *) ret, f);
}

int ircservices_read_time(time_t *ret, ircservices_dbFILE *f)
{
    int32 high, low;
    if (ircservices_read_int32(&high, f) < 0 || ircservices_read_int32(&low, f) < 0)
	return -1;
#if SIZEOF_TIME_T > 4
    *ret = (time_t)high << 32 | (time_t)low;
#else
    *ret = low;
#endif
    return 0;
}

int ircservices_read_ptr(void **ret, ircservices_dbFILE *f)
{
    int c;

    c = fgetc(f->fp);
    if (c == EOF)
	return -1;
    *ret = (c ? (void *)1 : (void *)0);
    return 0;
}

int ircservices_read_string(char **ret, ircservices_dbFILE *f)
{
    char *s;
    uint16 len;

    if (ircservices_read_int16(&len, f) < 0)
	return -1;
    if (len == 0) {
	*ret = NULL;
	return 0;
    }
    s = (char *) malloc(len);
    if (len != fread(s, 1, len, f->fp)) {
	free(s);
	return -1;
    }
    *ret = s;
    return 0;
}

#define ircservices_FILE_VERSION 11
#define ircservices_LOCAL_VERSION 23	/* For extensions to database files */

/* Default channel entries in version 4.5: */
#define ircservices_CA_SIZE_4_5		18
#define ircservices_ACCLEV_INVALID_4_5	-10000
static int ircservices_def_levels_4_5[ircservices_CA_SIZE_4_5] = {
    /* ircservices_CA_INVITE */         5,
    /* ircservices_CA_AKICK */         10,
    /* ircservices_CA_SET */ ircservices_ACCLEV_INVALID_4_5,
    /* ircservices_CA_UNBAN */          5,
    /* ircservices_CA_AUTOOP */         5,
    /* ircservices_CA_AUTODEOP */      -1,
    /* ircservices_CA_AUTOVOICE */      3,
    /* ircservices_CA_OPDEOP */         5,
    /* ircservices_CA_ACCESS_LIST */    0,
    /* ircservices_CA_CLEAR */ ircservices_ACCLEV_INVALID_4_5,
    /* ircservices_CA_NOJOIN */        -2,
    /* ircservices_CA_ACCESS_CHANGE */ 10,
    /* ircservices_CA_MEMO */          10,
    /* ircservices_CA_VOICE */          3,
    /* ircservices_CA_AUTOHALFOP */     4,
    /* ircservices_CA_HALFOP */         4,
    /* ircservices_CA_AUTOPROTECT */   10,
    /* ircservices_CA_PROTECT */       10,
};

/* Common routine to open a file for reading and check version number. */

#define ircservices_OPENDB_ERROR	(ircservices_dbFILE *)-1

static ircservices_dbFILE *ircservices_my_open_db_r(const char *dbname, int32 *ver_ret)
{
    ircservices_dbFILE *f;
    int32 ver;

    f = ircservices_open_db(dbname, "r", 0);
    if (!f)
	return NULL;
    ver = ircservices_get_file_version(f);
    if (ver < 5 || ver > 11) {
	if (ver == -1) {
	    SLOG(LM_ERROR, "Unable to read version number from %s",(
		dbname));
	} else {
	    SLOG(LM_ERROR, "Invalid version number %d on %s",( ver,
		dbname));
	}
	ircservices_close_db(f);
	return ircservices_OPENDB_ERROR;
    }
    *ver_ret = ver;
    return f;
}

void ircservices_free_nickinfo(ircservices_NickInfo *ni);
void ircservices_free_nickgroupinfo(ircservices_NickGroupInfo *ngi);
void ircservices_free_chaninfo(ircservices_ChanInfo *ci);
void ircservices_reset_levels(ircservices_ChanInfo *ci, int set);
void ircservices_reset_levels(ircservices_ChanExt *ce, int set);

static int load_one_nick(ircservices_dbFILE *f, int32 ver)
{
    ircservices_NickInfo *ni;
    ircservices_NickGroupInfo *ngi;
    int16 tmp16;
    int32 tmp32;
    int i;
    char passbuf[ircservices_PASSMAX];
    char *url, *email;

    ni = (ircservices_NickInfo *) calloc(sizeof(ircservices_NickInfo), 1);
    ircservices_SAFE(ircservices_read_buffer(ni->nick, f));
    ircservices_SAFE(ircservices_read_buffer(passbuf, f));
    ircservices_SAFE(ircservices_read_string(&url, f));
    ircservices_SAFE(ircservices_read_string(&email, f));
    ircservices_SAFE(ircservices_read_string(&ni->last_usermask, f));
    if (!ni->last_usermask)
	ni->last_usermask = strdup("@");
    ircservices_SAFE(ircservices_read_string(&ni->last_realname, f));
    if (!ni->last_realname)
	ni->last_realname = strdup("");
    ircservices_SAFE(ircservices_read_string(&ni->last_quit, f));
    ircservices_SAFE(ircservices_read_int32(&tmp32, f));
    ni->time_registered = tmp32;
    ircservices_SAFE(ircservices_read_int32(&tmp32, f));
    ni->last_seen = tmp32;
    ircservices_SAFE(ircservices_read_int16(&ni->status, f));
    ni->status &= ~ircservices_NS_TEMPORARY | 0x0001;  /* 0x0001 was ENCRYPTEDPW */
    /* Old-style links were hierarchical; if this nick was linked to
     * another, the name of the parent link, else NULL, was stored here.
     * Store that value in ni->last_realmask, which coincidentally was
     * not saved in old versions, and resolve links later. */
    ircservices_SAFE(ircservices_read_string(&ni->last_realmask, f));
    ircservices_SAFE(ircservices_read_int16(&tmp16, f));  /* linkcount */
    if (ni->last_realmask) {
	ircservices_SAFE(ircservices_read_int16(&tmp16, f));  /* channelcount */
	free(url);
	free(email);

	Nick_Stored_t *nick = Convert::ircservices_CreateNickEntry(ni, NULL);
	if (nick != NULL)
	    Magick::instance().nickserv.AddStored(nick);
	ircservices_free_nickinfo(ni);

    } else {
	ngi = (ircservices_NickGroupInfo *) calloc(sizeof(ircservices_NickGroupInfo), 1);
	ngi->id = ircservices_next_id++;
	ircservices_groupmap[ngi->id] = ni->nick;
	memcpy(ngi->pass, passbuf, ircservices_PASSMAX);
	ngi->url = url;
	ngi->email = email;
	ircservices_SAFE(ircservices_read_int32(&ngi->flags, f));
	if (ngi->flags & ircservices_NF_KILL_IMMED)
	    ngi->flags |= ircservices_NF_KILL_QUICK;
	if (ver >= 9) {
	    ircservices_read_ptr((void **)&ngi->suspendinfo, f);
	} else if (ver == 8 && (ngi->flags & 0x10000000)) {
	    /* In version 8, 0x10000000 was NI_SUSPENDED */
	    ngi->suspendinfo = (ircservices_SuspendInfo *)1;
	}
	if (ngi->suspendinfo) {
	    ircservices_SuspendInfo *si = (ircservices_SuspendInfo *) malloc(sizeof(*si));
	    ircservices_SAFE(ircservices_read_buffer(si->who, f));
	    ircservices_SAFE(ircservices_read_string(&si->reason, f));
	    ircservices_SAFE(ircservices_read_int32(&tmp32, f));
	    si->suspended = tmp32;
	    ircservices_SAFE(ircservices_read_int32(&tmp32, f));
	    si->expires = tmp32;
	    ngi->suspendinfo = si;
	}
	ircservices_SAFE(ircservices_read_int16(&ngi->access_count, f));
	if (ngi->access_count) {
	    char **access;
	    access = (char **) malloc(sizeof(char *) * ngi->access_count);
	    ngi->access = access;
	    ircservices_ARRAY_FOREACH (i, ngi->access)
		ircservices_SAFE(ircservices_read_string(&ngi->access[i], f));
	}
	ircservices_SAFE(ircservices_read_int16(&ngi->memos.memos_count, f));
	ircservices_SAFE(ircservices_read_int16(&ngi->memos.memomax, f));
	/*
	 * Note that at this stage we have no way of comparing this to the
	 * default memo max (MSMaxircservices_Memos) because the ircservices_MemoServ module is not
	 * loaded.  If this is a 5.x database, this is not a problem,
	 * because the correct memo max value will be stored in the
	 * extension data, but if not, we need to check and change the
	 * value to MEMOMAX_DEFAULT as needed.  This is handled by a
	 * callback (nick_memomax_callback() below) which triggers when the
	 * ircservices_MemoServ module is loaded.  The callback is added by
	 * open_nick_db() if needed.
	 */
	if (ngi->memos.memos_count) {
	    ircservices_Memo *memos;
	    memos = (ircservices_Memo *) malloc(sizeof(ircservices_Memo) * ngi->memos.memos_count);
	    ngi->memos.memos = memos;
	    ircservices_ARRAY_FOREACH (i, ngi->memos.memos) {
		ircservices_SAFE(ircservices_read_int32(&ngi->memos.memos[i].number, f));
		ircservices_SAFE(ircservices_read_int16(&ngi->memos.memos[i].flags, f));
		ircservices_SAFE(ircservices_read_int32(&tmp32, f));
		ngi->memos.memos[i].time = tmp32;
		ircservices_SAFE(ircservices_read_buffer(ngi->memos.memos[i].sender, f));
		ircservices_SAFE(ircservices_read_string(&ngi->memos.memos[i].text, f));
	    }
	}
	/* Channel counts are recalculated by open_channel_db() */
	ircservices_SAFE(ircservices_read_int16(&tmp16, f));  /* channelcount */
	/* If this is a 5.x database, we now get the real nickgroup value
	 * from bits 30-15 of the flags and the 16 bits we just read; the
	 * real flags value is stored in the extension data. */
	if (ngi->flags & 0x80000000)
	    ngi->id = (ngi->flags & 0x7FFF8000) << 1 | ((int)tmp16 & 0xFFFF);
	/* There was no way to set channel limits, so must be the default.
	 * Note that if this is a 5.x database, the correct value for this
	 * field (as well as memomax and language) will be read from the
	 * extension data. */
	ircservices_SAFE(ircservices_read_int16(&tmp16, f));  /* channelmax */
	ngi->channelmax = ircservices_CHANMAX_DEFAULT;
	ircservices_SAFE(ircservices_read_int16(&ngi->language, f));
	if (ngi->language < 0 || ngi->language >= ircservices_NUM_LANGS)
	    ngi->language = ircservices_DEF_LANGUAGE;
	ngi->timezone = ircservices_TIMEZONE_DEFAULT;

	Nick_Stored_t *nick = Convert::ircservices_CreateNickEntry(ni, ngi);
	if (nick != NULL)
	{
	    Magick::instance().nickserv.AddStored(nick);
	    if (ngi->memos.memos_count)
	    {
		MemoServ::nick_memo_t memo = Convert::ircservices_CreateMemoEntry(&ngi->memos, ni->nick);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);
	    }
	}
	ircservices_free_nickinfo(ni);
	ircservices_free_nickgroupinfo(ngi);
    }
    return 0;

  fail:
    SLOG(LM_ERROR, "Read error on %s",( f->filename));
    return 1;
}

/*************************************************************************/

/* Load extension data for a nick.  Returns zero on success, nonzero on
 * failure.
 */

static int load_one_nick_ext(ircservices_dbFILE *f, int32 ver)
{
    char *nick;
    ircservices_NickExt ne;
    memset(&ne, 0, sizeof(ne));
    ne.group = false;

    ircservices_SAFE(ircservices_read_string(&nick, f));
    if (!nick)
	goto fail;

    ircservices_SAFE(ircservices_read_string(&ne.last_realmask, f));
    if (ver >= 19)
	ircservices_SAFE(ircservices_read_int32(&ne.id_stamp, f));

    Convert::ircservices_UpdateNickEntry(&ne, nick);
    free(ne.last_realmask);
    return 0;

  fail:
    SLOG(LM_ERROR, "Read error on %s",( f->filename));
    return 1;
}

/*************************************************************************/

/* Load extension data for a nick group. */

static int load_one_nickgroup_ext(ircservices_dbFILE *f, int32 ver)
{
    uint32 group;
    ircservices_NickExt ne;
    memset(&ne, 0, sizeof(ne));
    ne.group = true;
    int i;

    ircservices_SAFE(ircservices_read_int32(&group, f));
    ircservices_SAFE(ircservices_read_int32(&ne.flags, f));
    ircservices_SAFE(ircservices_read_int32(&ne.authcode, f));
    ircservices_SAFE(ircservices_read_time(&ne.authset, f));
    ircservices_SAFE(ircservices_read_int16(&ne.channelmax, f));
    if (ver >= 18) {
	ircservices_SAFE(ircservices_read_int16(&ne.ajoin_count, f));
	if (ne.ajoin_count) {
	    ne.ajoin = (char **) malloc(sizeof(char *) * ne.ajoin_count);
	    ircservices_ARRAY_FOREACH (i, ne.ajoin)
		ircservices_SAFE(ircservices_read_string(&ne.ajoin[i], f));
	}
    }
    ircservices_SAFE(ircservices_read_int16(&ne.memomax, f));
    if (ver >= 17) {
	ircservices_SAFE(ircservices_read_int16(&ne.ignore_count, f));
	if (ne.ignore_count) {
	    ne.ignore = (char **) malloc(sizeof(char *) * ne.ignore_count);
	    ircservices_ARRAY_FOREACH (i, ne.ignore)
		ircservices_SAFE(ircservices_read_string(&ne.ignore[i], f));
	}
    }
    ircservices_SAFE(ircservices_read_int16(&ne.language, f));
    if (ver >= 15)
	ircservices_SAFE(ircservices_read_int16(&ne.timezone, f));
    ircservices_SAFE(ircservices_read_string(&ne.info, f));
    if (ver >= 13)
	ircservices_SAFE(ircservices_read_int16(&ne.os_priv, f));

    if (ircservices_groupmap.find(group) != ircservices_groupmap.end())
	Convert::ircservices_UpdateNickEntry(&ne, ircservices_groupmap[group].c_str());

    ircservices_ARRAY_FOREACH (i, ne.ajoin)
	free(ne.ajoin[i]);
    free(ne.ajoin);
    ircservices_ARRAY_FOREACH (i, ne.ignore)
	free(ne.ignore[i]);
    free(ne.ignore);
    return 0;

  fail:
    SLOG(LM_ERROR, "Read error on %s",( f->filename));
    return 1;
}

int ircservices_open_nick(const char *dbname)
{
    ircservices_dbFILE *f;
    int32 ver;
    int i, c;
    int failed = 0;

    /* Open database. */
    if (!(f = ircservices_my_open_db_r(dbname, &ver)))
	return 1;
    else if (f == ircservices_OPENDB_ERROR)
	return 0;

    /* Load original data. */
    ircservices_next_id = 1;
    for (i = 0; i < 256 && !failed; i++) {
	while ((c = ircservices_getc_db(f)) != 0) {
	    if (c != 1)
		SLOG(LM_EMERGENCY, "database/version4: Invalid format in %s",( dbname));
	    if ((failed = load_one_nick(f, ver)) != 0)
		break;
	}
    }

    /* Load extension data if present. */
    ver = 0;
    if (!failed && ircservices_read_int32(&ver, f) == 0) {
	if (ver <= ircservices_FILE_VERSION || ver > ircservices_LOCAL_VERSION)
	    SLOG(LM_EMERGENCY, "database/version4: Invalid extension data version in %s",(
		  dbname));
	while ((c = ircservices_getc_db(f)) != 0) {
	    if (c != 1)
		SLOG(LM_EMERGENCY, "database/version4: Invalid format in %s extension"
		      " data",( dbname));
	    if ((failed = load_one_nick_ext(f, ver)) != 0)
		break;
	}
	while ((c = ircservices_getc_db(f)) != 0) {
	    if (c != 1)
		SLOG(LM_EMERGENCY, "database/version4: Invalid format in %s extension"
		      " data",( dbname));
	    if ((failed = load_one_nickgroup_ext(f, ver)) != 0)
		break;
	}
    }

    /* Close database. */
    ircservices_close_db(f);

    /* All done! */
    return !failed;
}

void ircservices_free_nickinfo(ircservices_NickInfo *ni)
{
    free(ni->last_usermask);
    free(ni->last_realmask);
    free(ni->last_realname);
    free(ni->last_quit);
    free(ni);
}

void ircservices_free_nickgroupinfo(ircservices_NickGroupInfo *ngi)
{
    int i;

    free(ngi->url);
    free(ngi->email);
    if (ngi->suspendinfo) {
	free(ngi->suspendinfo->reason);
	free(ngi->suspendinfo);
    }
    ircservices_ARRAY_FOREACH (i, ngi->access)
	free(ngi->access[i]);
    free(ngi->access);
    ircservices_ARRAY_FOREACH (i, ngi->memos.memos)
	free(ngi->memos.memos[i].text);
    free(ngi->memos.memos);
    free(ngi);
}

/* Helper functions to convert between old and new channel levels. */

static inline int16 ircservices_convert_old_level(int16 old)
{
    if (old < 0)
	return -ircservices_convert_old_level(-old);/* avoid negative division */
    else if (old <= 25)
	return old*10;			/*    0..  25 ->   0..250 (10x) */
    else if (old <= 50)
	return 200 + old*2;		/*   25..  50 -> 250..300 ( 2x) */
    else if (old <= 100)
	return 280 + old*2/5;		/*   50.. 100 -> 300..320 ( 0.4x) */
    else if (old <= 1000)
	return 300 + old/5;		/*  100..1000 -> 320..500 ( 0.2x) */
    else if (old <= 2000)
	return 400 + old/10;		/* 1000..2000 -> 500..600 ( 0.1x) */
    else
	return 500 + old/20;		/* 2000..9999 -> 600..999 ( 0.05x) */
}

/*************************************************************************/

static int load_one_channel(ircservices_dbFILE *f, int32 ver)
{
    ircservices_ChanInfo *ci;
    int16 tmp16, lev;
    int32 tmp32;
    int n_levels;
    int i;
    int is_default;  /* Are all access levels at defaults? */

    ci = (ircservices_ChanInfo *) calloc(sizeof(ircservices_ChanInfo), 1);
    ircservices_SAFE(ircservices_read_buffer(ci->name, f));
    ircservices_SAFE(ircservices_read_string(&ci->founder, f));
    if (ver >= 7) {
	ircservices_SAFE(ircservices_read_string(&ci->successor, f));
	if ((mstring(ci->founder)).IsSameAs(ci->successor, true))
	{
	    free(ci->successor);
	    ci->successor = 0;
	}
    }
    ircservices_SAFE(ircservices_read_buffer(ci->founderpass, f));
    ircservices_SAFE(ircservices_read_string(&ci->desc, f));
    if (!ci->desc)
	ci->desc = strdup("");
    ircservices_SAFE(ircservices_read_string(&ci->url, f));
    ircservices_SAFE(ircservices_read_string(&ci->email, f));
    ircservices_SAFE(ircservices_read_int32(&tmp32, f));
    ci->time_registered = tmp32;
    ircservices_SAFE(ircservices_read_int32(&tmp32, f));
    ci->last_used = tmp32;
    ircservices_SAFE(ircservices_read_string(&ci->last_topic, f));
    ircservices_SAFE(ircservices_read_buffer(ci->last_topic_setter, f));
    ircservices_SAFE(ircservices_read_int32(&tmp32, f));
    ci->last_topic_time = tmp32;
    ircservices_SAFE(ircservices_read_int32(&ci->flags, f));
    if (ver >= 9)
	ircservices_SAFE(ircservices_read_ptr((void **)&ci->suspendinfo, f));
    if (ci->suspendinfo) {
	ircservices_SuspendInfo *si = (ircservices_SuspendInfo *) malloc(sizeof(*si));
	ircservices_SAFE(ircservices_read_buffer(si->who, f));
	ircservices_SAFE(ircservices_read_string(&si->reason, f));
	ircservices_SAFE(ircservices_read_int32(&tmp32, f));
	si->suspended = tmp32;
	ircservices_SAFE(ircservices_read_int32(&tmp32, f));
	si->expires = tmp32;
	ci->suspendinfo = si;
    }
    ci->flags &= ~0x0100;  /* 0x0100 = ENCRYPTEDPW */
    ircservices_SAFE(ircservices_read_int16(&tmp16, f));
    n_levels = tmp16;
    ircservices_reset_levels(ci, 1);
    is_default = 1;
    for (i = 0; i < n_levels; i++) {
	ircservices_SAFE(ircservices_read_int16(&lev, f));
	if (i >= ircservices_CA_SIZE_4_5 || lev != ircservices_def_levels_4_5[i])
	    is_default = 0;
	if (i < ircservices_CA_SIZE)
	    ci->levels[i] = ircservices_convert_old_level(lev);
    }
    /* Check whether all levels are at the default, and if so, remove the
     * levels array */
    if (is_default)
	ircservices_reset_levels(ci, 0);

    ircservices_SAFE(ircservices_read_int16(&ci->access_count, f));
    if (ci->access_count) {
	ci->access = (ircservices_ChanAccess *) calloc(ci->access_count, sizeof(ircservices_ChanAccess));
	ircservices_ARRAY_FOREACH (i, ci->access) {
	    ircservices_SAFE(ircservices_read_int16(&tmp16, f));  /* in_use */
	    if (tmp16) {
		ircservices_SAFE(ircservices_read_int16(&lev, f));
		ci->access[i].level = ircservices_convert_old_level(lev);
		ircservices_SAFE(ircservices_read_string(&ci->access[i].nickgroup, f));
	    }
	}
    } else {
	ci->access = NULL;
    }

    ircservices_SAFE(ircservices_read_int16(&ci->akick_count, f));
    if (ci->akick_count) {
	ci->akick = (ircservices_AutoKick *) calloc(ci->akick_count, sizeof(ircservices_AutoKick));
	ircservices_ARRAY_FOREACH (i, ci->akick) {
	    ircservices_SAFE(ircservices_read_int16(&tmp16, f));  /* in_use */
	    if (tmp16) {
		ircservices_SAFE(ircservices_read_int16(&tmp16, f));  /* is_nick */
		ircservices_SAFE(ircservices_read_string(&ci->akick[i].mask, f));
		ircservices_SAFE(ircservices_read_string(&ci->akick[i].reason, f));
		if (ver >= 8)
		    ircservices_SAFE(ircservices_read_buffer(ci->akick[i].who, f));
		else
		    *ci->akick[i].who = 0;
		time(&ci->akick[i].set);
		ci->akick[i].lastused = 0;
	    } /* if (in_use) */
	} /* for (i = 0..ci->akick_count-1) */
    } else {
	ci->akick = NULL;
    }

    if (ver < 10) {
	ircservices_SAFE(ircservices_read_int16(&tmp16, f));
	ci->mlock_on = tmp16;
	ircservices_SAFE(ircservices_read_int16(&tmp16, f));
	ci->mlock_off = tmp16;
    } else {
	ircservices_SAFE(ircservices_read_int32(&ci->mlock_on, f));
	ircservices_SAFE(ircservices_read_int32(&ci->mlock_off, f));
    }
    ircservices_SAFE(ircservices_read_int32(&ci->mlock_limit, f));
    ircservices_SAFE(ircservices_read_string(&ci->mlock_key, f));
//    ci->mlock_on &= ~chanmode_reg;  /* check_modes() takes care of this */

    ircservices_SAFE(ircservices_read_int16(&ci->memos.memos_count, f));
    ircservices_SAFE(ircservices_read_int16(&ci->memos.memomax, f));
    /* We may need to check memomax later (see load_one_nick()) */
    if (ci->memos.memos_count) {
	ircservices_Memo *memos;
	memos = (ircservices_Memo *) malloc(sizeof(ircservices_Memo) * ci->memos.memos_count);
	ci->memos.memos = memos;
	ircservices_ARRAY_FOREACH (i, ci->memos.memos) {
	    ircservices_SAFE(ircservices_read_int32(&ci->memos.memos[i].number, f));
	    ircservices_SAFE(ircservices_read_int16(&ci->memos.memos[i].flags, f));
	    ircservices_SAFE(ircservices_read_int32(&tmp32, f));
	    ci->memos.memos[i].time = tmp32;
	    ircservices_SAFE(ircservices_read_buffer(ci->memos.memos[i].sender, f));
	    ircservices_SAFE(ircservices_read_string(&ci->memos.memos[i].text, f));
	}
    }

    ircservices_SAFE(ircservices_read_string(&ci->entry_message, f));

    {
	Chan_Stored_t *chan = Convert::ircservices_CreateChanEntry(ci);
	if (chan != NULL)
	{
	    Magick::instance().chanserv.AddStored(chan);
	    if (ci->memos.memos_count)
	    {
		MemoServ::channel_news_t news = Convert::ircservices_CreateNewsEntry(&ci->memos, ci->name);
		if (news.size())
		    Magick::instance().memoserv.AddChannel(news);
	    }
	}
    }
    ircservices_free_chaninfo(ci);

    return 0;

  fail:
    SLOG(LM_ERROR, "Read error on %s",( f->filename));
    return 1;
}

/*************************************************************************/

/* Load extension data for a channel. */

static int load_one_channel_ext(ircservices_dbFILE *f, int32 ver)
{
    char *name;
    ircservices_ChanExt ce;
    memset(&ce, 0, sizeof(ircservices_ChanExt));
    int i;
    int16 count;

    ircservices_SAFE(ircservices_read_string(&name, f));
    if (!name)
	goto fail;

    ircservices_SAFE(ircservices_read_int16(&ce.memomax, f));
    if (ver >= 14) {
	if (ver >= 23) {
	    ircservices_SAFE(ircservices_read_int16(&count, f));
	}
	for (i = 0; i < count; i++) {
	    time_t t;
	    ircservices_SAFE(ircservices_read_time(&t, f));
	    ircservices_SAFE(ircservices_read_time(&t, f));
	}
    }
    if (ver >= 16) {
	ircservices_SAFE(ircservices_read_string(&ce.mlock_link, f));
	ircservices_SAFE(ircservices_read_string(&ce.mlock_flood, f));
    }
    if (ver >= 20) {
	int16 lev;
	ircservices_SAFE(ircservices_read_int16(&count, f));
	if (count) {
	    ircservices_reset_levels(&ce, 1);
	    for (i = 0; i < count; i++) {
		ircservices_SAFE(ircservices_read_int16(&lev, f));
		if (i < ircservices_CA_SIZE)
		    ce.levels[i] = lev;
	    }
	    if (ver == 20) {
		if (ce.levels[ircservices_CA_AUTODEOP] == -10)
		    ce.levels[ircservices_CA_AUTODEOP] = -1;
		if (ce.levels[ircservices_CA_NOJOIN] == -20)
		    ce.levels[ircservices_CA_NOJOIN] = -100;
	    }
	} else if (ce.levels) {
	    free(ce.levels);
	    ce.levels = NULL;
	}
	ircservices_SAFE(ircservices_read_int16(&count, f));
	for (i = 0; i < count; i++) {
	    ircservices_SAFE(ircservices_read_int16(&lev, f));
	}
    }

    Convert::ircservices_UpdateChanEntry(&ce, name);

    if (ce.mlock_link)
	free(ce.mlock_link);
    if (ce.mlock_flood)
	free(ce.mlock_flood);
    if (ce.levels)
	free(ce.levels);
    return 0;

  fail:
    SLOG(LM_ERROR, "Read error on %s",( f->filename));
    return 1;
}

int ircservices_open_chan(const char *dbname)
{
    ircservices_dbFILE *f;
    int32 ver;
    int i, c;
    int failed = 0;

    /* Open database. */
    if (!(f = ircservices_my_open_db_r(dbname, &ver)))
	return 1;
    else if (f == ircservices_OPENDB_ERROR)
	return 0;

    /* Load original data. */
    for (i = 0; i < 256 && !failed; i++) {
	while ((c = ircservices_getc_db(f)) != 0) {
	    if (c != 1)
		SLOG(LM_EMERGENCY, "database/version4: Invalid format in %s",( dbname));
	    if ((failed = load_one_channel(f, ver)) != 0) {
		break;
	    }
	}
    }

    /* Load extension data if present. */
    if (!failed && ircservices_read_int32(&ver, f) == 0) {
	if (ver <= ircservices_FILE_VERSION || ver > ircservices_LOCAL_VERSION)
	    SLOG(LM_EMERGENCY, "database/version4: Invalid extension data version in %s",(
		  dbname));
	while ((c = ircservices_getc_db(f)) != 0) {
	    if (c != 1)
		SLOG(LM_EMERGENCY, "database/version4: Invalid format in %s extension"
		      " data",( dbname));
	    if ((failed = load_one_channel_ext(f, ver)) != 0)
		break;
	}
    }

    /* Close database and return. */
    ircservices_close_db(f);
    return !failed;
}

void ircservices_free_chaninfo(ircservices_ChanInfo *ci)
{
    int i;

    if (ci->founder)
	free(ci->founder);
    if (ci->successor)
	free(ci->successor);
    free(ci->desc);
    free(ci->url);
    free(ci->email);
    free(ci->last_topic);
    if (ci->suspendinfo) {
	free(ci->suspendinfo->reason);
	free(ci->suspendinfo);
    }
    free(ci->levels);
    ircservices_ARRAY_FOREACH (i, ci->access) {
	free(ci->access[i].nickgroup);
    }
    free(ci->access);
    ircservices_ARRAY_FOREACH (i, ci->akick) {
	free(ci->akick[i].mask);
	free(ci->akick[i].reason);
    }
    free(ci->akick);
    free(ci->mlock_key);
    free(ci->entry_message);
    if (ci->memos.memos) {
	ircservices_ARRAY_FOREACH (i, ci->memos.memos)
	    free(ci->memos.memos[i].text);
	free(ci->memos.memos);
    }
    free(ci);
}

void ircservices_reset_levels(ircservices_ChanInfo *ci, int set)
{
    int i;

    free(ci->levels);
    if (set) {
	ci->levels = (short *) calloc(ircservices_CA_SIZE, sizeof(*ci->levels));
	for (i = 0; i < ircservices_CA_SIZE; i++)
	    ci->levels[i] = ircservices_def_levels[i];
    } else {
	ci->levels = NULL;
    }
}

void ircservices_reset_levels(ircservices_ChanExt *ce, int set)
{
    int i;

    free(ce->levels);
    if (set) {
	ce->levels = (short *) calloc(ircservices_CA_SIZE, sizeof(*ce->levels));
	for (i = 0; i < ircservices_CA_SIZE; i++)
	    ce->levels[i] = ircservices_def_levels[i];
    } else {
	ce->levels = NULL;
    }
}

int ircservices_open_oper(const char *dbname)
{
    ircservices_dbFILE *f;
    int32 ver;
    int16 i, n;
    char *s;

    if (!(f = ircservices_my_open_db_r(dbname, &ver)))
	return 1;
    else if (f == ircservices_OPENDB_ERROR)
	return 0;

    ircservices_SAFE(ircservices_read_int16(&n, f));
    for (i = 0; i < n; i++) {
	ircservices_SAFE(ircservices_read_string(&s, f));

	if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());

	free(s);
    }
    ircservices_SAFE(ircservices_read_int16(&n, f));
    for (i = 0; i < n; i++) {
	ircservices_SAFE(ircservices_read_string(&s, f));

	if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)) &&
		!(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->find(s)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());

	free(s);
    }
    if (ver >= 7) {
	int32 tmp32;
	ircservices_SAFE(ircservices_read_int32(&tmp32, f));
	ircservices_SAFE(ircservices_read_int32(&tmp32, f));
    }
    if (ver >= 9) {
	char no_supass;
	char supass[ircservices_PASSMAX];
	ircservices_SAFE(ircservices_read_int8((uint8 *) &no_supass, f));
	if (!no_supass)
	    ircservices_SAFE(ircservices_read_buffer(supass, f));
    }

    ircservices_close_db(f);
    return 1;

  fail:
    SLOG(LM_ERROR, "Read error on %s",( dbname));
    return 0;
}

int ircservices_open_news(const char *dbname)
{
    ircservices_dbFILE *f;
    int32 ver;
    int i, newslist_count;
    int16 tmp16;
    ircservices_NewsItem news;

    if (!(f = ircservices_my_open_db_r(dbname, &ver)))
	return 1;
    else if (f == ircservices_OPENDB_ERROR)
	return 0;

    ircservices_read_int16(&tmp16, f);
    newslist_count = tmp16;

    for (i = 0; i < tmp16; i++) {
	int32 tmp32;
	ircservices_SAFE(ircservices_read_int16(&news.type, f));
	ircservices_SAFE(ircservices_read_int32(&news.num, f));
	ircservices_SAFE(ircservices_read_string(&news.text, f));
	ircservices_SAFE(ircservices_read_buffer(news.who, f));
	ircservices_SAFE(ircservices_read_int32(&tmp32, f));
	news.time = tmp32;

	if (news.type == ircservices_NEWS_LOGON && Magick::instance().commserv.IsList(Magick::instance().commserv.ALL_Name()))
	{
	    Magick::instance().commserv.GetList(Magick::instance().commserv.ALL_Name())->
		    MSG_insert(mstring(news.text), mstring(news.who), mDateTime(news.time));
	}
	else if (news.type == ircservices_NEWS_OPER && Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
	{
	    Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->
		    MSG_insert(mstring(news.text), mstring(news.who), mDateTime(news.time));
	}

	free(news.text);
    }

    ircservices_close_db(f);
    return 1;

  fail:
    SLOG(LM_ERROR, "Read error on %s",( dbname));
    return 0;
}

static int ircservices_read_maskdata(uint8 type, const char *dbname, ircservices_dbFILE *f)
{
    int32 ver;
    int i;
    int16 tmp16;
    ircservices_MaskData *list;
    int count;

    ircservices_read_int16(&tmp16, f);
    count = tmp16;
    list = (ircservices_MaskData *) calloc(sizeof(*list), count);
    for (i = 0; i < tmp16; i++) {
	int32 tmp32;
	ircservices_SAFE(ircservices_read_string(&list[i].mask, f));
	if (type == ircservices_MD_EXCEPTION) {
	    ircservices_SAFE(ircservices_read_int16(&list[i].limit, f));
	    ircservices_SAFE(ircservices_read_buffer(list[i].who, f));
	    ircservices_SAFE(ircservices_read_string(&list[i].reason, f));
	} else {
	    ircservices_SAFE(ircservices_read_string(&list[i].reason, f));
	    ircservices_SAFE(ircservices_read_buffer(list[i].who, f));
	}
	ircservices_SAFE(ircservices_read_int32(&tmp32, f));
	list[i].time = tmp32;
	ircservices_SAFE(ircservices_read_int32(&tmp32, f));
	list[i].expires = tmp32;
    }
    if (ircservices_read_int32(&ver, f) == 0) {
	if (ver <= ircservices_FILE_VERSION || ver > ircservices_LOCAL_VERSION)
	    SLOG(LM_EMERGENCY, "database/version4: Invalid extension data version in %s",(
		  dbname));
	for (i = 0; i < count; i++) {
	    ircservices_SAFE(ircservices_read_time(&list[i].time, f));
	    ircservices_SAFE(ircservices_read_time(&list[i].expires, f));
	    ircservices_SAFE(ircservices_read_time(&list[i].lastused, f));
	}
    }

    for (i = 0; i < count; i++) {
	if (type == ircservices_MD_AKILL)
	{
	    if (list[i].expires == 0)
	    {
		Magick::instance().operserv.Akill_insert(mstring(list[i].mask), Magick::instance().operserv.Expire_Sop(),
			mstring(list[i].reason), mstring(list[i].who), mDateTime(list[i].time));
	    }
	    else
	    {
		Magick::instance().operserv.Akill_insert(mstring(list[i].mask), list[i].expires-list[i].time,
			mstring(list[i].reason), mstring(list[i].who), mDateTime(list[i].time));
	    }
	}
	else if (type == ircservices_MD_EXCEPTION)
	{
	    Magick::instance().operserv.Clone_insert(mstring(list[i].mask).After("@"), list[i].limit,
			mstring(list[i].reason), mstring(list[i].who), mDateTime(list[i].time));
	}
	free(list[i].mask);
	free(list[i].reason);
    }
    free(list);

    return 1;

  fail:
    SLOG(LM_ERROR, "Read error on %s",( dbname));
    return 0;
}

int ircservices_open_akill(const char *dbname)
{
    ircservices_dbFILE *f;
    int32 ver;

    if (!(f = ircservices_my_open_db_r(dbname, &ver)))
	return 1;
    else if (f == ircservices_OPENDB_ERROR)
	return 0;
    if (!ircservices_read_maskdata(ircservices_MD_AKILL, dbname, f))
	return 0;
    if (ircservices_getc_db(f) == 1) {
	if (!ircservices_read_maskdata(ircservices_MD_EXCLUSION, dbname, f))
	    return 0;
    }
    ircservices_close_db(f);
    return 1;
}

int ircservices_open_exception(const char *dbname)
{
    ircservices_dbFILE *f;
    int32 ver;

    if (!(f = ircservices_my_open_db_r(dbname, &ver)))
	return 1;
    else if (f == ircservices_OPENDB_ERROR)
	return 0;
    if (!ircservices_read_maskdata(ircservices_MD_EXCEPTION, dbname, f))
	return 0;
    ircservices_close_db(f);
    return 1;
}

void ircservices_load_nick()
{
    ircservices_open_nick("nick.db");
}

void ircservices_load_chan()
{
    ircservices_open_nick("chan.db");
}

void ircservices_load_news()
{
    ircservices_open_nick("news.db");
}

void ircservices_load_akill()
{
    ircservices_open_nick("akill.db");
}

void ircservices_load_oper()
{
    ircservices_open_nick("oper.db");
}

void ircservices_load_exception()
{
    ircservices_open_nick("exception.db");
}

Nick_Stored_t *Convert::ircservices_CreateNickEntry(ircservices_NickInfo * ni, ircservices_NickGroupInfo * ngi)
{
    BTCB();
    int i;
    char **string;

    if (ni == NULL || ni->nick == NULL || !strlen(ni->nick))
	return NULL;

    if (ni->status & ircservices_NS_VERBOTEN)
    {
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick);

	return out;
    }
    else if (ngi == NULL && ni->last_realmask && strlen(ni->last_realmask))
    {
	Nick_Stored_t tmp(mstring(ni->last_realmask));
	Nick_Stored_t *out = new Nick_Stored_t(mstring(ni->nick), mDateTime(ni->time_registered), tmp);

	if (out == NULL)
	    return NULL;

	if (ni->last_realname != NULL && strlen(ni->last_realname))
	    out->i_LastRealName = ni->last_realname;
	if (ni->last_usermask != NULL && strlen(ni->last_usermask))
	    out->i_LastMask = ni->last_usermask;
	if (ni->last_quit != NULL && strlen(ni->last_quit))
	    out->i_LastQuit = mstring(ni->last_quit);
	out->i_LastSeenTime = mDateTime(ni->last_seen);
	return out;
    }
    else if (ngi != NULL)
    {
	Nick_Stored_t *out = new Nick_Stored_t(mstring(ni->nick), mstring(ngi->pass));

	if (out == NULL)
	    return NULL;

#ifdef ircservices_DB_USES_ENCRYPTION
	    char pwbuf[33] = {0};
	    for (int i=0; i<16; i++)
		sprintf(&pwbuf[i*2], "%02x", ni->pass[i]);
	    out->i_Password = pwbuf;
#endif

	if (ni->last_realname != NULL && strlen(ni->last_realname))
	    out->i_LastRealName = mstring(ni->last_realname);
	if (ni->last_usermask != NULL && strlen(ni->last_usermask))
	    out->i_LastMask = mstring(ni->last_usermask);
	if (ni->last_quit != NULL && strlen(ni->last_quit))
	    out->i_LastQuit = mstring(ni->last_quit);
	out->i_RegTime = mDateTime(ni->time_registered);
	out->i_LastSeenTime = mDateTime(ni->last_seen);

	if (ngi->email != NULL && strlen(ngi->email))
	    out->i_Email = mstring(ngi->email);
	if (ngi->url != NULL && strlen(ngi->url))
	    out->i_URL = mstring(ngi->url);
	if (out->i_URL.Contains("http://"))
	    out->i_URL.Remove("http://", false);
	if (out->i_URL.Contains("HTTP://"))
	    out->i_URL.Remove("HTTP://", false);

	for (i = 0, string = ngi->access; i < ngi->access_count; ++i, ++string)
	{
	    out->i_access.insert(mstring(*string));
	}

	switch (ngi->language)
	{
	case ircservices_LANG_EN_US:
	    out->setting.Language = "ENGLISH";
	    break;
	case ircservices_LANG_JA_EUC:
	case ircservices_LANG_JA_SJIS:
	    out->setting.Language = "JAPANESE";
	    break;
	case ircservices_LANG_ES:
	    out->setting.Language = "SPANISH";
	    break;
	case ircservices_LANG_PT:
	    out->setting.Language = "PORTUGUESE";
	    break;
	case ircservices_LANG_FR:
	    out->setting.Language = "FRENCH";
	    break;
	case ircservices_LANG_TR:
	    out->setting.Language = "TURKISH";
	    break;
	case ircservices_LANG_IT:
	    out->setting.Language = "ITALIAN";
	    break;
	case ircservices_LANG_DE:
	    out->setting.Language = "GERMAN";
	    break;
	case ircservices_LANG_NL:
	    out->setting.Language = "DUTCH";
	    break;
	case ircservices_LANG_HU:
	    out->setting.Language = "HUNGARIAN";
	    break;
	}

	if (ni->status & ircservices_NS_NOEXPIRE && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if (ngi->flags & ircservices_NF_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (ngi->flags & ircservices_NF_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ngi->flags & ircservices_NF_PRIVATE && !out->L_Private())
	    out->setting.Private = true;

	if (ngi->suspendinfo)
	{
	    if (ngi->suspendinfo->reason)
		out->i_Comment = ngi->suspendinfo->reason;
	    out->i_Suspend_By = ngi->suspendinfo->who;
	    out->i_Suspend_Time = mDateTime(ngi->suspendinfo->suspended);
	}

	return out;
    }
    else
    {
	return NULL;
    }

    ETCB();
}

void Convert::ircservices_UpdateNickEntry(ircservices_NickExt * ne, const char *name)
{
    BTCB();
    int i;

    if (!Magick::instance().nickserv.IsStored(name))
	return;

    map_entry<Nick_Stored_t> out = Magick::instance().nickserv.GetStored(name);

    if (ne->group)
    {
	for (i=0; i<ne->ignore_count; i++)
	{
	    if (!ne->ignore[i] || !strlen(ne->ignore[i]))
		continue;

	    out->IgnoreAdd(ne->ignore[i]);
	}

	if (ne->info && strlen(ne->info))
	    out->Description(ne->info);

	switch (ne->language)
	{
	case ircservices_LANG_EN_US:
	    out->setting.Language = "ENGLISH";
	    break;
	case ircservices_LANG_JA_EUC:
	case ircservices_LANG_JA_SJIS:
	    out->setting.Language = "JAPANESE";
	    break;
	case ircservices_LANG_ES:
	    out->setting.Language = "SPANISH";
	    break;
	case ircservices_LANG_PT:
	    out->setting.Language = "PORTUGUESE";
	    break;
	case ircservices_LANG_FR:
	    out->setting.Language = "FRENCH";
	    break;
	case ircservices_LANG_TR:
	    out->setting.Language = "TURKISH";
	    break;
	case ircservices_LANG_IT:
	    out->setting.Language = "ITALIAN";
	    break;
	case ircservices_LANG_DE:
	    out->setting.Language = "GERMAN";
	    break;
	case ircservices_LANG_NL:
	    out->setting.Language = "DUTCH";
	    break;
	case ircservices_LANG_HU:
	    out->setting.Language = "HUNGARIAN";
	    break;
	}

	if (ne->flags & ircservices_NF_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (ne->flags & ircservices_NF_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ne->flags & ircservices_NF_PRIVATE && !out->L_Private())
	    out->setting.Private = true;

	if ((ne->os_priv & ircservices_NP_SERVADMIN) &&
	    Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	    !Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->find(name))
	{
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(name)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(name),
									Magick::instance().commserv.FirstName());
	}
	else if ((ne->os_priv & ircservices_NP_SERVOPER) &&
		 Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		 !Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->find(name))
	{
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(name)) &&
		!(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->find(name)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->insert(mstring(name),
									Magick::instance().commserv.FirstName());
	}
    }

    ETCB();
}

static mstring ircservices_getmodes(int32 modes)
{
    BTCB();
    mstring retval;

    if (modes & ircservices_CMODE_i)
	retval += "i";
    if (modes & ircservices_CMODE_m)
	retval += "m";
    if (modes & ircservices_CMODE_n)
	retval += "n";
    if (modes & ircservices_CMODE_p)
	retval += "p";
    if (modes & ircservices_CMODE_s)
	retval += "s";
    if (modes & ircservices_CMODE_t)
	retval += "t";
    if (modes & ircservices_CMODE_k)
	retval += "k";
    if (modes & ircservices_CMODE_l)
	retval += "l";
    if (modes & 0x00000100)
	retval += "R";
    if (modes & 0x00000200)
	retval += "r";
    if (modes & 0x00000400)
	retval += "c";
    if (modes & 0x00000800)
	retval += "O";
    if (modes & 0x00001000)
	retval += "A";
    if (modes & 0x00002000)
	retval += "z";
    if (modes & 0x00004000)
	retval += "Q";
    if (modes & 0x00008000)
	retval += "K";
    if (modes & 0x00010000)
	retval += "V";
    if (modes & 0x00020000)
	retval += "H";
    if (modes & 0x00040000)
	retval += "C";
    if (modes & 0x00080000)
	retval += "N";
    if (modes & 0x00100000)
	retval += "S";
    if (modes & 0x00200000)
	retval += "G";
    if (modes & 0x00400000)
	retval += "u";
    if (modes & 0x00800000)
	retval += "f";
    if (modes & 0x01000000)
	retval += "L";
    if (modes & 0x02000000)
	retval += "M";
    if (modes & 0x04000000)
	retval += "T";
    if (modes & 0x08000000)
	retval += "x";
    if (modes & 0x10000000)
	retval += "N";

    return retval;
    ETCB();
}


Chan_Stored_t *Convert::ircservices_CreateChanEntry(ircservices_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name == NULL || !strlen(ci->name))
	return NULL;

    if (ci->flags & ircservices_CI_VERBOTEN)
    {
	Chan_Stored_t *out = new Chan_Stored_t(mstring(ci->name));

	return out;
    }
    else
    {
	ircservices_ChanAccess *i_access;
	ircservices_AutoKick *akick;
	int i;

	if (ci->founder == NULL || !strlen(ci->founder) || ci->desc == NULL || !strlen(ci->desc) || ci->founderpass == NULL ||
	    !strlen(ci->founderpass))
	{
	    return NULL;
	}

	Chan_Stored_t *out =
	    new Chan_Stored_t(mstring(ci->name), mstring(ci->founder), mstring(ci->founderpass), mstring(ci->desc));

	if (out == NULL)
	    return NULL;

#ifdef ircservices_DB_USES_ENCRYPTION
	    char pwbuf[33] = {0};
	    for (i=0; i<16; i++)
		sprintf(&pwbuf[i*2], "%02x", ci->founderpass[i]);
	    out->i_Password = pwbuf;
#endif

	if (ci->successor != NULL && strlen(ci->successor))
	    out->i_CoFounder = mstring(ci->successor);
	if (ci->email != NULL && strlen(ci->email))
	    out->i_Email = mstring(ci->email);
	if (ci->url != NULL && strlen(ci->url))
	    out->i_URL = mstring(ci->url);
	if (out->i_URL.Contains("http://"))
	    out->i_URL.Remove("http://", false);
	if (out->i_URL.Contains("HTTP://"))
	    out->i_URL.Remove("HTTP://", false);
	out->i_RegTime = mDateTime(ci->time_registered);
	out->i_LastUsed = mDateTime(ci->last_used);

	mstring modelock;
	if (ci->mlock_on || ci->mlock_key != NULL || ci->mlock_limit)
	{
	    mstring modes = ircservices_getmodes(ci->mlock_on);

	    modes.Remove("k");
	    modes.Remove("l");
	    modelock << "+" << modes << (ci->mlock_key != NULL ? "k" : "") << (ci->mlock_limit ? "l" : "");
	}
	if (ci->mlock_off)
	{
	    modelock << "-" << ircservices_getmodes(ci->mlock_off);
	}
	if (ci->mlock_key != NULL)
	{
	    modelock << " " << ci->mlock_key;
	}
	if (ci->mlock_limit)
	{
	    modelock << " " << ci->mlock_limit;
	}
	if (modelock.length())
	    out->Mlock(Magick::instance().chanserv.FirstName(), modelock);

	if (ci->last_topic != NULL && strlen(ci->last_topic))
	    out->i_Topic = mstring(ci->last_topic);
	if (ci->last_topic_setter != NULL && strlen(ci->last_topic_setter))
	    out->i_Topic_Setter = mstring(ci->last_topic_setter);
	 out->i_Topic_Set_Time = mDateTime(ci->last_topic_time);

	if (ci->entry_message != NULL && strlen(ci->entry_message))
	    out->Message_insert(ci->entry_message, Magick::instance().chanserv.FirstName());

	long newlevel;
	float mod = (float) Magick::instance().chanserv.Level_Max() / (float) ircservices_ACCLEV_FOUNDER;

	for (i = 0, i_access = ci->access; i < ci->access_count; ++i, ++i_access)
	{
	    if (i_access->nickgroup == NULL)
		continue;

	    if (i_access->level < 0)
		newlevel = -1;
	    else
		newlevel = (long) ((float) i_access->level * mod);
	    if (newlevel == 0)
		newlevel = 1;

	    out->Access_insert(i_access->nickgroup, newlevel, Magick::instance().chanserv.FirstName());
	}

	for (i = 0, akick = ci->akick; i < ci->akick_count; ++i, ++akick)
	{
	    if (akick->mask == NULL)
		continue;
	    if (akick->reason != NULL)
		out->Akick_insert(akick->mask, akick->reason, mstring(akick->who));
	    else
		out->Akick_insert(akick->mask, mstring(akick->who));
	}

	if (ci->flags & ircservices_CI_KEEPTOPIC && !out->L_Keeptopic())
	    out->setting.Keeptopic = true;
	if (ci->flags & ircservices_CI_SECUREOPS && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & ircservices_CI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & ircservices_CI_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & ircservices_CI_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & ircservices_CI_LEAVEOPS && !out->L_Anarchy())
	    out->setting.Anarchy = true;
	if (ci->flags & ircservices_CI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & ircservices_CI_ENFORCE && !out->L_Revenge())
	    out->setting.Revenge = "REVERSE";
	if (ci->flags & ircservices_CI_NOEXPIRE && !Magick::instance().chanserv.LCK_NoExpire())
	    out->setting.NoExpire = true;

	if (ci->suspendinfo)
	{
	    if (ci->suspendinfo->reason)
		out->i_Comment = ci->suspendinfo->reason;
	    out->i_Suspend_By = ci->suspendinfo->who;
	    out->i_Suspend_Time = mDateTime(ci->suspendinfo->suspended);
	}

	if (ci->levels != NULL)
	{
	    for (i = 0; i < ircservices_CA_SIZE; ++i)
	    {
		if (ci->levels[i] == ircservices_ACCLEV_INVALID)
		    newlevel = Magick::instance().chanserv.Level_Max() + 2;
		else if (ci->levels[i] == ircservices_ACCLEV_FOUNDER)
		    newlevel = Magick::instance().chanserv.Level_Max() + 1;
		else if (ci->levels[i] < 0)
		    newlevel = -1;
		else
		    newlevel = (long) ((float) ci->levels[i] * mod);

		switch (i)
		{
		case ircservices_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_AUTOHALFOP:
		    out->Level_change("AUTOHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_MEMO:
		    out->Level_change("WRITEMEMO", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_UNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_ACCESS_CHANGE:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_ACCESS_LIST:
		    out->Level_change("VIEW", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_INVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_OPDEOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_HALFOP:
		    out->Level_change("CMDHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ircservices_CA_CLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}
	return out;
    }
    ETCB();
}

void Convert::ircservices_UpdateChanEntry(ircservices_ChanExt * ce, const char *name)
{
    BTCB();
    static_cast<void>(ce);
    static_cast<void>(name);
    ETCB();
}

MemoServ::nick_memo_t Convert::ircservices_CreateMemoEntry(ircservices_MemoInfo *mi, const char *name)
{
    BTCB();
    int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;

    for (i = 0; i < mi->memos_count; i++)
    {
	if (mi->memos[i].sender || !strlen(mi->memos[i].sender) ||
	    mi->memos[i].text || !strlen(mi->memos[i].text))
	    continue;

	tmp = new Memo_t(name, mi->memos[i].sender, mi->memos[i].text);
	tmp->i_Time = mDateTime(mi->memos[i].time);
	if (!(mi->memos[i].flags & ircservices_MF_UNREAD))
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}

MemoServ::channel_news_t Convert::ircservices_CreateNewsEntry(ircservices_MemoInfo *mi, const char *name)
{
    BTCB();
    int i;

    MemoServ::channel_news_t out;
    News_t *tmp;

    for (i = 0; i < mi->memos_count; i++)
    {
	if (mi->memos[i].sender || !strlen(mi->memos[i].sender) ||
	    mi->memos[i].text || !strlen(mi->memos[i].text))
	    continue;

	tmp = new News_t(name, mi->memos[i].sender, mi->memos[i].text);
	tmp->i_Time = mDateTime(mi->memos[i].time);
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}



#endif /* CONVERT */
