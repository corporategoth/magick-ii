#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
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
#define RCSID(x,y) const char *rcsid_convert_trircd_cpp_ ## x () { return y; }
RCSID(convert_trircd_cpp, "@(#)$Id$");

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

/* Based upon the DB loading routines from trircd-services 4.26
 * (c) 1999-2000 Andrew Church <achurch@achurch.org>
 */

#include "magick.h"
#include "convert/interface.h"
#include "convert/trircd.h"

/* Should we try and deal with old (v2.x) databases?  Define this if you're
 * upgrading from v2.2.26 or earlier. */
/* #define trircd_COMPATIBILITY_V2 */

/* Default language for newly registered nicks (and nicks imported from
 * old databases); see services.h for available languages (search for
 * "LANG_").  Unless you're running a regional network, you should probably
 * leave this at LANG_EN_US. */
#define trircd_DEF_LANGUAGE	trircd_LANG_EN_US

/* Maximum length of a channel name, including the trailing null.  Any
 * channels with a length longer than trircd_CHANMAX-1 (including the leading #)
 * will not be usable with ChanServ. */
#define trircd_CHANMAX		64

/* Maximum length of a nickname, including the trailing null.  This MUST be
 * at least one greater than the maximum allowable nickname length on your
 * network, or people will run into problems using Services!  The default
 * (32) works with all servers I know of. */
#define trircd_NICKMAX		32

/* Maximum length of a password */
#define trircd_PASSMAX		32

#define trircd_MSMaxMemos	20
#define trircd_CSMaxReg		20
#define trircd_NSAllowKillImmed	0
#define trircd_NSAccessMax	32

#define trircd_NickDBName	"nick.db"
#define trircd_AjoinDBName	"ajoin.db"
#define trircd_ChanDBName	"chan.db"
#define trircd_CForbidDBName	"cforbid.db"
#define trircd_OperDBName	"oper.db"
#define trircd_AutokillDBName	"akill.db"
#define trircd_NewsDBName	"news.db"
#define trircd_ExceptionDBName	"exception.db"

typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

/* Version number for data files; if structures below change, increment
 * this.  (Otherwise -very- bad things will happen!) */

#define FILE_VERSION	25

/* trircd_Memo info structures.  Since both nicknames and channels can have memos,
 * we encapsulate memo data in a trircd_MemoList to make it easier to handle. */

struct trircd_Memo {
    uint32 number;	/* Index number -- not necessarily array position! */
    int16 flags;
    time_t time;	/* When it was sent */
    time_t expiration;	/* when the memo will expire */
    char sender[trircd_NICKMAX];
    char *text;
};

#define trircd_MF_UNREAD	0x0001	/* trircd_Memo has not yet been read */
#define trircd_MF_MARKED	0x0002  /* trircd_Memo has been marked */

struct trircd_MemoInfo {
    int16 memocount, memomax;
    trircd_Memo *memos;
};


struct trircd_SuspendInfo {
    char who[trircd_NICKMAX];	/* who added this suspension */
    char *reason;
    time_t suspended;
    time_t expires;	/* 0 for no expiry */
};

struct trircd_ForbidInfo {
    char forbidder[trircd_NICKMAX];   
    char *forbid_message;
};

struct trircd_IgnoreInfo {
    int16 in_use;
    char *ign;
};


/* Nickname info structure.  Each nick structure is stored in one of 256
 * lists; the list is determined by the first character of the nick.  Nicks
 * are stored in alphabetical order within lists. */

struct trircd_NickInfo {
    char nick[trircd_NICKMAX];
    char pass[trircd_PASSMAX];
    char *url;
    char *email;

    char *last_usermask;
    char *last_realname;
    char *last_quit;
    time_t time_registered;
    time_t last_seen;
    int16 status;		/* See NS_* below */

    char *link;		/* If non-NULL, nick to which this one is linked */
    int16 linkcount;		/* Number of links to this nick */

    /* All information from this point down is governed by links.  Note,
     * however, that channelcount is always saved, even for linked nicks
     * (thus removing the need to recount channels when unlinking a nick). */

    int32 flags;		/* See NI_* below */
    trircd_SuspendInfo *suspendinfo;	/* Suspension info (non-NULL => suspended) */
    uint16 channelcount;	/* Number of channels currently registered */
    uint16 channelmax;		/* Maximum number of channels allowed */
    uint16 language;		/* Language selected by nickname owner (trircd_LANG_*) */

    int16 accesscount;		/* # of entries */
    char **access;		/* Array of strings */

    trircd_MemoInfo memos;

    /* Online-only information: */

    time_t id_stamp;		/* Services stamp of user who last ID'd for nick */

    uint16 foundercount;	/* Number of channels nick is a founder of */
    char **founderchans;	/* Array of ... */

    int bad_passwords;  	/* # of bad passwords for nick since last good one */ 
};

struct trircd_NickExt {
    int16 ajoincount;   	/* Amount of entries in one's autojoin list */
    char **ajoin;       	/* Array of strigns to be the list of autojoins */

    int32 authcode;     	/* Authentication code used by email authentication */
    int16 authmode;     	/* Authentication mode flags NH_* */

    trircd_ForbidInfo *forbidinfo;

    time_t lastmail;    	/* Last time this nick record got a mail */

    int16 ignorecount;		/* Number of nicks where the trircd_Memos will be ignored. */
    trircd_IgnoreInfo *ignored; 	/* Array of ignored nicknames */

    char *infomsg;		/* Info msg for NickServ INFO */

    time_t expiredelay;		/* Amount of time this nick preserves a new memo */
};


/* Nickname status flags: */
#define trircd_NS_ENCRYPTEDPW	0x0001      /* Nickname password is encrypted */
#define trircd_NS_VERBOTEN	0x0002      /* Nick may not be registered or used */
#define trircd_NS_NOEXPIRE	0x0004      /* Nick never expires */

#define trircd_NS_IDENTIFIED	0x8000      /* User has IDENTIFY'd */
#define trircd_NS_RECOGNIZED	0x4000      /* ON_ACCESS true && SECURE flag not set */
#define trircd_NS_ON_ACCESS	0x2000      /* User comes from a known address */
#define trircd_NS_KILL_HELD	0x1000      /* Nick is being held after a kill */
#define trircd_NS_SERVROOT	0x0800      /* User has Services root privileges */
#define trircd_NS_GUESTED	0x0100	    /* SVSNICK has been sent but nick has not
				     * yet changed. An enforcer will be
				     * introduced when it does change. */
#define trircd_NS_TEMPORARY	0xFF00      /* All temporary status flags */


/* Nickname setting flags: */
#define trircd_NI_KILLPROTECT	0x00000001  /* Kill others who take this nick */
#define trircd_NI_SECURE	0x00000002  /* Don't recognize unless IDENTIFY'd */
#define trircd_NI_MEMO_HARDMAX	0x00000008  /* Don't allow user to change memo limit */
#define trircd_NI_MEMO_SIGNON	0x00000010  /* Notify of memos at signon and un-away */
#define trircd_NI_MEMO_RECEIVE	0x00000020  /* Notify of new memos when sent */
#define trircd_NI_PRIVATE	0x00000040  /* Don't show in LIST to non-servadmins */
#define trircd_NI_HIDE_EMAIL	0x00000080  /* Don't show E-mail in INFO */
#define trircd_NI_HIDE_MASK	0x00000100  /* Don't show last seen address in INFO */
#define trircd_NI_HIDE_QUIT	0x00000200  /* Don't show last quit message in INFO */
#define trircd_NI_KILL_QUICK	0x00000400  /* Kill in 20 seconds instead of 60 */
#define trircd_NI_KILL_IMMED	0x00000800  /* Kill immediately instead of in 60 sec */

#define trircd_NI_MAILMEMO	0x80000000  /* Shall we email memos !also! ? */

/* Nickname authentication flags which are needed for a problemless conversion 
 * of older nicks to the new authentication system
 */

#define trircd_NH_AUTHENTIC    0x0001
#define trircd_NH_OLDAUTH      0x0002
#define trircd_NH_NONAUTH      0x0004

#define trircd_LANG_EN_US      0       /* United States English */
#define trircd_LANG_TR         1       /* Japanese (JIS encoding) */
#define trircd_LANG_TR2        2       /* Japanese (EUC encoding) */
#define trircd_LANG_JA_EUC     3       /* Japanese (SJIS encoding) */
#define trircd_LANG_JA_SJIS    4
#define trircd_LANG_ES         5       /* Spanish */
#define trircd_LANG_PT         6       /* Portugese */
#define trircd_LANG_FR         7       /* French */
#define trircd_LANG_IT         8       /* Italian */
#define trircd_LANG_DE		9	/* German */

#define trircd_NUM_LANGS	10	/* Number of languages */

/* Access levels for users. */
struct trircd_ChanAccess {
    int16 in_use;	/* 1 if this entry is in use, else 0 */
    int16 level;
    char *ni;	/* Guaranteed to be non-NULL if in use, NULL if not */
};

struct trircd_ChanAccessExt {
    time_t last_used;	/* time when this akick was last used */
};

/* Note that these two levels also serve as exclusive boundaries for valid
 * access levels.  ACCLEV_FOUNDER may be assumed to be strictly greater
 * than any valid access level, and ACCLEV_INVALID may be assumed to be
 * strictly less than any valid access level.
 */
#define trircd_ACCLEV_FOUNDER	10000	/* Numeric level indicating founder access */
#define trircd_ACCLEV_INVALID	-10000	/* Used in levels[] for disabled settings */

/* Access levels used to represent AOP's, SOP's and VOP's in channel access
 * lists. */

#define trircd_ACCLEV_SOP	10
#define trircd_ACCLEV_AOP	5
#define trircd_ACCLEV_VOP	3
#define trircd_ACCLEV_HOP	4


/* trircd_AutoKick data. */
struct trircd_AutoKick {
    int16 in_use;
    int16 is_nick;	/* 1 if a regged nickname, 0 if a nick!user@host mask */
			/* Always 0 if not in use */
    union {
	char *mask;	/* Guaranteed to be non-NULL if in use, NULL if not */
	char *ni;	/* Same */
    } u;
    char *reason;
    char who[trircd_NICKMAX];
};

struct trircd_AutoKickExt
{
    time_t time_added;	/* Time when this akick has been added */
    time_t last_used;	/* Time when this akick has been last used */
    time_t expires;	/* Time when this akick will expire */
};

struct trircd_ChanInfo {
    char name[trircd_CHANMAX];
    char *founder;
    char *successor;		/* Who gets the channel if the founder
					 * nick is dropped or expires */
    char founderpass[trircd_PASSMAX];
    char *desc;
    char *url;
    char *email;

    time_t time_registered;
    time_t last_used;
    char *last_topic;		/* Last topic on the channel */
    char last_topic_setter[trircd_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;	/* When the last topic was set */

    int32 flags;		/* See below */
    trircd_SuspendInfo *suspendinfo;	/* Non-NULL iff suspended */

    int16 *levels;		/* Access levels for commands */

    int16 accesscount;
    trircd_ChanAccess *access;		/* List of authorized users */
    int16 akickcount;
    trircd_AutoKick *akick;		/* List of users to kickban */

    int32 mlock_on, mlock_off;	/* See channel modes below */
    int32 mlock_limit;		/* 0 if no limit */
    char *mlock_key;		/* NULL if no key */

    char *entry_message;	/* Notice sent on entering channel */

    trircd_MemoInfo memos;

    void *c;		/* Pointer to channel record (if   *
				 *    channel is currently in use) */

    int bad_passwords;		/* # of bad passwords since last good one */
};

struct trircd_ChanExt {
    trircd_ForbidInfo *forbidinfo;	/* Information stored in forbid data */

    int16 accesscount;
    trircd_ChanAccessExt *access;		/* List of authorized users */
    int16 akickcount;
    trircd_AutoKickExt *akick;		/* List of users to kickban */

};

#define trircd_CI_KEEPTOPIC    0x00000001      /* Retain topic even after last person leaves channel */
#define trircd_CI_SECUREOPS    0x00000002      /* Don't allow non-authorized users to be opped */
#define trircd_CI_PRIVATE      0x00000004      /* Hide channel from ChanServ LIST command */
#define trircd_CI_TOPICLOCK    0x00000008      /* Topic can only be changed by SET TOPIC */
#define trircd_CI_RESTRICTED   0x00000010      /* Those not allowed ops are kickbanned */
#define trircd_CI_LEAVEOPS     0x00000020      /* Don't auto-deop anyone */
#define trircd_CI_SECURE       0x00000040      /* Don't allow any privileges unless a user is IDENTIFY'd */
#define trircd_CI_VERBOTEN     0x00000080      /* Don't allow the channel to be registered or used */
#define trircd_CI_ENCRYPTEDPW  0x00000100      /* Channel password is encrypted */
#define trircd_CI_NOEXPIRE     0x00000200      /* Channel does not expire */
#define trircd_CI_MEMO_HARDMAX 0x00000400      /* Channel memo limit may not be changed */
#define trircd_CI_OPNOTICE     0x00000800      /* Send notice to channel on use of OP/DEOP */
#define trircd_CI_ENFORCE	0x00001000	/* Enforce +o, +v modes (don't allow deopping) */

#define trircd_CI_EXCEPTION    0x10000000      /* Lock Ban Exception for every unban instead mode -b */
#define trircd_CI_INVITES      0x20000000      /* Use channel invites +I instead of INVITE */
#define trircd_CI_WARNACCESS	0x40000000	/* Send wallops if access command is used */
#define trircd_CI_HIDE_EMAIL   0x80000000	/* Hide email info from ChanServ INFO */
#define trircd_CI_HIDE_TOPIC   0x01000000	/* Hide topic ONLY from ChanServ INFO */
#define trircd_CI_HIDE_OPTS	0x02000000	/* Hide options section from ChanServ INFO */
#define trircd_CI_HIDE_MLOCK	0x04000000	/* Hide mlock section from ChanServ INFO */
#define trircd_CI_HIDE_DESC	0x08000000	/* Hide description section from ChanServ INFO */

/* Indices for cmd_access[]: (DO NOT REORDER THESE unless you hack
 * load_cs_dbase to deal with them) */

#define trircd_CA_INVITE		0
#define trircd_CA_AKICK		1
#define trircd_CA_SET			2	/* but not FOUNDER or PASSWORD */
#define trircd_CA_UNBAN		3
#define trircd_CA_AUTOOP		4
#define trircd_CA_AUTODEOP		5	/* Maximum, not minimum */
#define trircd_CA_AUTOVOICE		6
#define trircd_CA_OPDEOP		7	/* ChanServ commands OP and DEOP */
#define trircd_CA_ACCESS_LIST		8
#define trircd_CA_CLEAR		9
#define trircd_CA_NOJOIN		10	/* Maximum */
#define trircd_CA_ACCESS_CHANGE 	11
#define trircd_CA_MEMO			12
#define trircd_CA_VOICE		13	/* VOICE/DEVOICE commands */
#define trircd_CA_AUTOHALFOP		14
#define trircd_CA_HALFOP		15	/* HALFOP/DEHALFOP commands */
#define trircd_CA_AUTOPROTECT		16
#define trircd_CA_PROTECT		17

#define trircd_CA_SIZE			18

int trircd_def_levels[][2] = {
    { trircd_CA_AUTOOP,             5 },
    { trircd_CA_AUTOVOICE,          3 },
    { trircd_CA_AUTODEOP,          -1 },
    { trircd_CA_NOJOIN,            -2 },
    { trircd_CA_INVITE,             5 },
    { trircd_CA_AKICK,             10 },
    { trircd_CA_SET,   trircd_ACCLEV_INVALID },
    { trircd_CA_CLEAR, trircd_ACCLEV_INVALID },
    { trircd_CA_UNBAN,              5 },
    { trircd_CA_OPDEOP,             5 },
    { trircd_CA_ACCESS_LIST,        0 },
    { trircd_CA_ACCESS_CHANGE,     10 },
    { trircd_CA_MEMO,              10 },
    { trircd_CA_VOICE,              3 },
    { trircd_CA_AUTOHALFOP,         4 },
    { trircd_CA_HALFOP,             4 },
    { trircd_CA_AUTOPROTECT,       10 },
    { trircd_CA_PROTECT,           10 },
    { -1 }
};

#define trircd_CMODE_i		0x00000001
#define trircd_CMODE_m		0x00000002
#define trircd_CMODE_n		0x00000004
#define trircd_CMODE_p		0x00000008
#define trircd_CMODE_s		0x00000010
#define trircd_CMODE_t		0x00000020
#define trircd_CMODE_k		0x00000040	/* These two used only by ChanServ */
#define trircd_CMODE_l		0x00000080
#define trircd_CMODE_R		0x00000100	/* Only identified users can join */
#define trircd_CMODE_r 		0x00000200	/* Set for all registered channels */
#define trircd_CMODE_c		0x00000400	/* No ANSI colors in channel */
#define trircd_CMODE_O		0x00000800	/* Only opers can join channel */
#define trircd_CMODE_A		0x00001000	/* Only admins can join channel */
#define trircd_CMODE_z		0x00002000	/* Only secure (+z) users allowed */
#define trircd_CMODE_Q		0x00004000	/* No kicks */
#define trircd_CMODE_K		0x00008000	/* No knocks */
#define trircd_CMODE_V		0x00010000	/* No invites */
#define trircd_CMODE_H		0x00020000	/* No hiding (+I) users */
#define trircd_CMODE_C		0x00040000	/* No CTCPs */
#define trircd_CMODE_N		0x00080000	/* No nick changing */
#define trircd_CMODE_S		0x00100000	/* Strip colors */
#define trircd_CMODE_G		0x00200000	/* Strip "bad words" */
#define trircd_CMODE_u		0x00400000	/* Auditorium mode */
#define trircd_CMODE_f		0x00800000	/* Flood limit */
#define trircd_CMODE_M		0x01000000

struct trircd_NewsItem {
    int16 type;
    int32 num;		/* Numbering is separate for login and oper news */
    char *text;
    char who[trircd_NICKMAX];
    time_t time;
};

/*************************************************************************/

/* Constants for news types. */

#define trircd_NEWS_LOGON	0
#define trircd_NEWS_OPER	1

struct trircd_Akill {
    char *mask;
    char *reason;
    char who[trircd_NICKMAX];
    time_t time;
    time_t expires;	/* or 0 for no expiry */
};

struct trircd_Exception {
    char *mask;			/* Hosts to which this exception applies */
    int16 limit;		/* Session limit for exception */
    char who[trircd_NICKMAX];		/* Nick of person who added the exception */
    char *reason;		/* Reason for exception's addition */
    time_t time;		/* When this exception was added */
    time_t expires;		/* Time when it expires. 0 == no expiry */
    int num;			/* Position in exception list */
};

struct trircd_dbFILE {
    int mode;			/* 'r' for reading, 'w' for writing */
    FILE *fp;			/* The normal file descriptor */
    FILE *backupfp;		/* Open file pointer to a backup copy of
				 *    the database file (if non-NULL) */
    char filename[PATH_MAX];	/* Name of the database file */
    char backupname[PATH_MAX];	/* Name of the backup file */
};

#define trircd_read_db(f,buf,len)	(fread((buf),1,(len),(f)->fp))
#define trircd_getc_db(f)		(fgetc((f)->fp))
#define trircd_read_buffer(buf,f)	(trircd_read_db((f),(buf),sizeof(buf)) == sizeof(buf))
#define trircd_read_buflen(buf,len,f)	(trircd_read_db((f),(buf),(len)) == (len))
#define trircd_read_variable(var,f)	(trircd_read_db((f),&(var),sizeof(var)) == sizeof(var))

int trircd_get_file_version(trircd_dbFILE *f)
{
    FILE *fp = f->fp;
    int version = fgetc(fp)<<24 | fgetc(fp)<<16 | fgetc(fp)<<8 | fgetc(fp);
    if (ferror(fp)) {
	SLOG(LM_ERROR, "Error reading version number on %s",( f->filename));
	return -1;
    } else if (feof(fp)) {
	SLOG(LM_ERROR, "Error reading version number on %s: End of file detected",(
		f->filename));
	return -1;
    } else if (version > FILE_VERSION || version < 1) {
	SLOG(LM_ERROR, "Invalid version number (%d) on %s",( version, f->filename));
	return -1;
    }
    return version;
}

trircd_dbFILE *trircd_open_db_read(const char *service, const char *filename)
{
    trircd_dbFILE *f;
    FILE *fp;

    f = (trircd_dbFILE *) malloc(sizeof(*f));
    if (!f) {
	SLOG(LM_ERROR, "Can't read %s database %s",( service, filename));
	return NULL;
    }
    strncpy(f->filename, filename, sizeof(f->filename));
    f->mode = 'r';
    fp = fopen(f->filename, "rb");
    if (!fp) {
	int errno_save = errno;
	if (errno != ENOENT)
	    SLOG(LM_ERROR, "Can't read %s database %s",( service, f->filename));
	free(f);
	errno = errno_save;
	return NULL;
    }
    f->fp = fp;
    f->backupfp = NULL;
    return f;
}

trircd_dbFILE *trircd_open_db(const char *service, const char *filename, const char *mode)
{
    if (*mode == 'r') {
	return trircd_open_db_read(service, filename);
    } else {
	errno = EINVAL;
	return NULL;
    }
}

void trircd_close_db(trircd_dbFILE *f)
{
    if (f->mode == 'w' && *f->backupname
			&& strcmp(f->backupname, f->filename) != 0) {
	if (f->backupfp)
	    fclose(f->backupfp);
	unlink(f->backupname);
    }
    fclose(f->fp);
    free(f);
}

int trircd_read_int8(unsigned char *ret, trircd_dbFILE *f)
{
    int c = fgetc(f->fp);
    if (c == EOF)
	return -1;
    *ret = c;
    return 0;
}

inline int trircd_read_int8(char *ret, trircd_dbFILE *f)
{
    return trircd_read_int8((unsigned char *) ret, f);
}

inline int trircd_read_int16(uint16 *ret, trircd_dbFILE *f)
{
    int c1, c2;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    if (c2 == EOF)
	return -1;
    *ret = c1<<8 | c2;
    return 0;
}

inline int trircd_read_int16(int16 *ret, trircd_dbFILE *f)
{
    return trircd_read_int16((uint16 *) ret, f);
}

int trircd_read_int32(uint32 *ret, trircd_dbFILE *f)
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

inline int trircd_read_int32(int32 *ret, trircd_dbFILE *f)
{
    return trircd_read_int32((uint32 *) ret, f);
}

int trircd_read_ptr(void **ret, trircd_dbFILE *f)
{
    int c;

    c = fgetc(f->fp);
    if (c == EOF)
	return -1;
    *ret = (c ? (void *)1 : (void *)0);
    return 0;
}

int trircd_read_string(char **ret, trircd_dbFILE *f)
{
    char *s;
    uint16 len;

    if (trircd_read_int16(&len, f) < 0)
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

int trircd_delnick(trircd_NickInfo *ni);
int trircd_delchan(trircd_ChanInfo *ci);
void trircd_reset_levels(trircd_ChanInfo *ci);

#define trircd_SAFE(x) do {					\
    if ((x) < 0) {					\
	    NSLOG(LM_EMERGENCY, "Read error on memo.db");		\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void trircd_load_old_memo()
{
    trircd_dbFILE *f;
    int ver, i, j, c;
    trircd_Memo *memos;
    struct memolist_ {
	struct memolist_ *next, *prev;
	char nick[trircd_NICKMAX];
	long n_memos;
	trircd_Memo *memos;
	long reserved[4];
    } old_memolist;
    struct {
	char sender[trircd_NICKMAX];
	long number;
	time_t time;
	char *text;
	short flags;
	short reserved_s;
	long reserved[3];
    } oldmemo;
    int failed = 0;

    if (!(f = trircd_open_db("MemoServ", "memo.db", "r")))
	return;
    switch (ver = trircd_get_file_version(f)) {
      case 4:
      case 3:
      case 2:
      case 1:
	for (i = 33; i < 256 && !failed; ++i) {
	    while ((c = trircd_getc_db(f)) != 0) {
		if (c != 1)
		    NSLOG(LM_EMERGENCY, "Invalid format in memo.db");
		trircd_SAFE(trircd_read_variable(old_memolist, f));
		old_memolist.memos = memos = (trircd_Memo *) 
				malloc(sizeof(trircd_Memo) * old_memolist.n_memos);
		for (j = 0; j < old_memolist.n_memos; j++, memos++) {
		    trircd_SAFE(trircd_read_variable(oldmemo, f));
		    strncpy(memos->sender, oldmemo.sender, trircd_NICKMAX);
		    memos->number = oldmemo.number;
		    memos->time = oldmemo.time;
		    memos->flags = oldmemo.flags;
		}
		memos = old_memolist.memos;
		for (j = 0; j < old_memolist.n_memos; j++) {
		    if (trircd_read_string(&memos[j].text, f) < 0)
			NSLOG(LM_EMERGENCY, "Read error on memo.db");
		}

		if (old_memolist.n_memos)
		{
		    trircd_MemoInfo mi;
		    mi.memocount = mi.memomax = old_memolist.n_memos;
		    mi.memos = old_memolist.memos;

		    MemoServ::nick_memo_t memo = Convert::trircd_CreateMemoEntry(&mi, old_memolist.nick);
		    if (memo.size())
			Magick::instance().memoserv.AddNick(memo);
		}

		for (j = 0; j < old_memolist.n_memos; j++)
		    free(memos[j].text);
		free(memos);
	    }
	}
	break;
      case -1:
	NSLOG(LM_EMERGENCY, "Unable to read version number from memo.db");
      default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on memo.db", (ver));
    } /* switch (version) */
    trircd_close_db(f);
}

#undef trircd_SAFE

#define trircd_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( trircd_NickDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void trircd_load_old_nick(trircd_dbFILE *f, int ver)
{
    struct nickinfo_ {
	trircd_NickInfo *next, *prev;
	char nick[trircd_NICKMAX];
	char pass[trircd_PASSMAX];
	char *last_usermask;
	char *last_realname;
	time_t time_registered;
	time_t last_seen;
	long accesscount;
	char **access;
	long flags;
	time_t id_stamp;
	unsigned short memomax;
	unsigned short channelcount;
	char *url;
	char *email;
    } old_nickinfo;

    int i, j, c;
    trircd_NickInfo *ni;
    int failed = 0;

    for (i = 33; i < 256 && !failed; i++) {
	while ((c = trircd_getc_db(f)) != 0) {
	    if (c != 1)
		SLOG(LM_EMERGENCY, "Invalid format in %s",( trircd_NickDBName));
	    trircd_SAFE(trircd_read_variable(old_nickinfo, f));
	    ni = (trircd_NickInfo *) calloc(1, sizeof(trircd_NickInfo));
	    strncpy(ni->nick, old_nickinfo.nick, trircd_NICKMAX);
	    strncpy(ni->pass, old_nickinfo.pass, trircd_PASSMAX);
	    ni->time_registered = old_nickinfo.time_registered;
	    ni->last_seen = old_nickinfo.last_seen;
	    ni->accesscount = old_nickinfo.accesscount;
	    ni->flags = old_nickinfo.flags;
	    if (ver < 3)	/* trircd_Memo max field created in ver 3 */
		ni->memos.memomax = trircd_MSMaxMemos;
	    else if (old_nickinfo.memomax)
		ni->memos.memomax = old_nickinfo.memomax;
	    else
		ni->memos.memomax = -1;  /* Unlimited is now -1 */
	    /* Reset channel count because counting was broken in old
	     * versions; trircd_load_old_chan() will calculate the count */
	    ni->channelcount = 0;
	    ni->channelmax = trircd_CSMaxReg;
	    ni->language = trircd_DEF_LANGUAGE;
	    /* ENCRYPTEDPW and VERBOTEN moved from ni->flags to ni->status */
	    if (ni->flags & 4)
		ni->status |= trircd_NS_VERBOTEN;
	    if (ni->flags & 8)
		ni->status |= trircd_NS_ENCRYPTEDPW;
	    ni->flags &= ~0xE000000C;
	    if (ni->status & trircd_NS_ENCRYPTEDPW) {
		/* Bail: it makes no sense to continue with encrypted
		 * passwords, since we won't be able to verify them */
		SLOG(LM_EMERGENCY, "%s: load database: password for %s encrypted "
		          "but encryption disabled, aborting",(
		          "NickServ", ni->nick));
	    }
	    if (old_nickinfo.url)
		trircd_SAFE(trircd_read_string(&ni->url, f));
	    if (old_nickinfo.email)
		trircd_SAFE(trircd_read_string(&ni->email, f));
	    trircd_SAFE(trircd_read_string(&ni->last_usermask, f));
	    if (!ni->last_usermask)
		ni->last_usermask = strdup("@");
	    trircd_SAFE(trircd_read_string(&ni->last_realname, f));
	    if (!ni->last_realname)
		ni->last_realname = strdup("");
	    if (ni->accesscount) {
		char **access, *s;
		if (ni->accesscount > trircd_NSAccessMax)
		    ni->accesscount = trircd_NSAccessMax;
		access = (char **) malloc(sizeof(char *) * ni->accesscount);
		ni->access = access;
		for (j = 0; j < ni->accesscount; j++, access++)
		    trircd_SAFE(trircd_read_string(access, f));
		while (j < old_nickinfo.accesscount) {
		    trircd_SAFE(trircd_read_string(&s, f));
		    if (s)
			free(s);
		    j++;
		}
	    }
	    ni->id_stamp = 0;
	    if (ver < 3) {
		ni->flags |= trircd_NI_MEMO_SIGNON | trircd_NI_MEMO_RECEIVE;
	    } else if (ver == 3) {
		if (!(ni->flags & (trircd_NI_MEMO_SIGNON | trircd_NI_MEMO_RECEIVE)))
		    ni->flags |= trircd_NI_MEMO_SIGNON | trircd_NI_MEMO_RECEIVE;
	    }

	    Nick_Stored_t *nick = Convert::trircd_CreateNickEntry(ni);
	    if (nick != NULL)
		Magick::instance().nickserv.AddStored(nick);
	    trircd_delnick(ni);
	} /* while (trircd_getc_db(f) != 0) */
    } /* for (i) */
    trircd_load_old_memo();
}

#undef trircd_SAFE

/*************************************************************************/

#define trircd_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",(( trircd_NickDBName)));	\
	return NULL;					\
    }							\
} while (0)

trircd_NickInfo *trircd_load_nick_record(trircd_dbFILE *f, int ver)
{
    trircd_NickInfo *ni;
    int32 tmp32;
    int i;

    ni = (trircd_NickInfo *) calloc(sizeof(trircd_NickInfo), 1);
    trircd_SAFE(trircd_read_buffer(ni->nick, f));
    trircd_SAFE(trircd_read_buffer(ni->pass, f));
    trircd_SAFE(trircd_read_string(&ni->url, f));
    trircd_SAFE(trircd_read_string(&ni->email, f));
    trircd_SAFE(trircd_read_string(&ni->last_usermask, f));
    if (!ni->last_usermask)
	ni->last_usermask = strdup("@");
    trircd_SAFE(trircd_read_string(&ni->last_realname, f));
    if (!ni->last_realname)
	ni->last_realname = strdup("");
    trircd_SAFE(trircd_read_string(&ni->last_quit, f));
    trircd_SAFE(trircd_read_int32(&tmp32, f));
    ni->time_registered = tmp32;
    trircd_SAFE(trircd_read_int32(&tmp32, f));
    ni->last_seen = tmp32;
    trircd_SAFE(trircd_read_int16(&ni->status, f));
    ni->status &= ~trircd_NS_TEMPORARY;
    if (ni->status & trircd_NS_ENCRYPTEDPW) {
	/* Bail: it makes no sense to continue with encrypted
	 * passwords, since we won't be able to verify them */
	SLOG(LM_EMERGENCY, "%s: load database: password for %s encrypted "
	      "but encryption disabled, aborting",(
	      "NickServ", ni->nick));
    }
    /* Store the _name_ of the link target in ni->link for now;
     * we'll resolve it after we've loaded all the nicks */
    trircd_SAFE(trircd_read_string((char **)&ni->link, f));
    /* We actually recalculate link and channel counts later, but leave
     * them in for now to avoid changing the data file format */
    trircd_SAFE(trircd_read_int16(&ni->linkcount, f));
    if (ni->link) {
	trircd_SAFE(trircd_read_int16(&ni->channelcount, f));
	/* No other information saved for linked nicks, since
	 * they get it all from their link target */
	ni->memos.memomax = trircd_MSMaxMemos;
	ni->channelmax = trircd_CSMaxReg;
	ni->language = trircd_DEF_LANGUAGE;
    } else {
	trircd_SAFE(trircd_read_int32(&ni->flags, f));
	ni->flags |= trircd_NI_HIDE_MASK;
	if (!trircd_NSAllowKillImmed)
	    ni->flags &= ~trircd_NI_KILL_IMMED;
	if (ver >= 21 || ver == 9 || ver == 10 || ver == 11) {
	    trircd_read_ptr((void **)&ni->suspendinfo, f);
	} else if ((ver == 8) && (ni->flags & 0x10000000)) {
	    /* In version 8, 0x10000000 was trircd_NI_SUSPENDED */
	    ni->suspendinfo = (trircd_SuspendInfo *)1;
	}
	if (ni->suspendinfo) {
	    trircd_SuspendInfo *si = (trircd_SuspendInfo *) malloc(sizeof(*si));
	    trircd_SAFE(trircd_read_buffer(si->who, f));
	    trircd_SAFE(trircd_read_string(&si->reason, f));
	    trircd_SAFE(trircd_read_int32(&tmp32, f));
	    si->suspended = tmp32;
	    trircd_SAFE(trircd_read_int32(&tmp32, f));
	    si->expires = tmp32;
	    ni->suspendinfo = si;
	}
	trircd_SAFE(trircd_read_int16(&ni->accesscount, f));
	if (ni->accesscount) {
	    char **access;
	    access = (char **) malloc(sizeof(char *) * ni->accesscount);
	    ni->access = access;
	    for (i = 0; i < ni->accesscount; i++, access++)
		trircd_SAFE(trircd_read_string(access, f));
	}
	trircd_SAFE(trircd_read_int16(&ni->memos.memocount, f));
	trircd_SAFE(trircd_read_int16(&ni->memos.memomax, f));
	if (ni->memos.memocount) {
	    trircd_Memo *memos;
	    memos = (trircd_Memo *) malloc(sizeof(trircd_Memo) * ni->memos.memocount);
	    ni->memos.memos = memos;
	    for (i = 0; i < ni->memos.memocount; i++, memos++) {
		trircd_SAFE(trircd_read_int32(&memos->number, f));
		trircd_SAFE(trircd_read_int16(&memos->flags, f));
		trircd_SAFE(trircd_read_int32(&tmp32, f));
		memos->time = tmp32;
		trircd_SAFE(trircd_read_buffer(memos->sender, f));
		trircd_SAFE(trircd_read_string(&memos->text, f));
	    }
	}
	trircd_SAFE(trircd_read_int16(&ni->channelcount, f));
	trircd_SAFE(trircd_read_int16(&ni->channelmax, f));
	if (ver <= 8) {
	    /* Fields not initialized or updated properly */
	    /* These will be updated by trircd_load_chan() */
	    ni->channelcount = 0;
	    if (ver == 5)
		ni->channelmax = trircd_CSMaxReg;
	}
	trircd_SAFE(trircd_read_int16(&ni->language, f));
    }
    /* Link and channel counts are recalculated later */
    ni->linkcount = 0;
    ni->channelcount = 0;
    return ni;
}

#undef trircd_SAFE

/*************************************************************************/

void trircd_load_nick()
{
    trircd_dbFILE *f;
    int ver, i, c;
    trircd_NickInfo *ni;
    int failed = 0;

    if (!(f = trircd_open_db("NickServ", trircd_NickDBName, "r")))
	return;

    switch (ver = trircd_get_file_version(f)) {
      case 25:
      case 24:
      case 23:
      case 22:
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
	for (i = 0; i < 256 && !failed; i++) {
	    while ((c = trircd_getc_db(f)) != 0) {
		if (c != 1)
		    SLOG(LM_EMERGENCY, "Invalid format in %s",( trircd_NickDBName));
		ni = trircd_load_nick_record(f, ver);
		if (ni) {
		    Nick_Stored_t *nick = Convert::trircd_CreateNickEntry(ni);
		    if (nick != NULL)
		    {
			Magick::instance().nickserv.AddStored(nick);
			if (ni->memos.memocount)
			{
			    MemoServ::nick_memo_t memo = Convert::trircd_CreateMemoEntry(&ni->memos, ni->nick);
			    if (memo.size())
				Magick::instance().memoserv.AddNick(memo);
			}
		    }
		    trircd_delnick(ni);
		} else {
		    failed = 1;
		    break;
		}
	    }
	}
	break;

      case 4:
      case 3:
      case 2:
      case 1:
	trircd_load_old_nick(f, ver);
	break;

      case -1:
	SLOG(LM_EMERGENCY, "Unable to read version number from %s",( trircd_NickDBName));

      default:
	SLOG(LM_EMERGENCY, "Unsupported version number (%d) on %s",( ver, trircd_NickDBName));

    } /* switch (version) */

    trircd_close_db(f);
}

#undef trircd_SAFE

#define trircd_SAFE(x) do {                                    \
    if ((x) < 0) {                                      \
            SLOG(LM_EMERGENCY, "Read error on %s", (trircd_AjoinDBName));     \
        failed = 1;                                     \
        break;                                          \
    }                                                   \
} while (0)
            
    
void trircd_load_nickext()
{       
    trircd_dbFILE *f;
    int ver, i, j, c;
    char mynick[trircd_NICKMAX];
    int failed = 0;
    int32 tmp32;
    int16 tmp16;
    
    if (!(f = trircd_open_db("NickServ", trircd_AjoinDBName, "r")))
        return;
    switch (ver = trircd_get_file_version(f)) {
      case 25:
        for (i = 0; i < 256 && !failed; i++) {
            while ((c = trircd_getc_db(f)) == 1) {
                if (c != 1)
                    SLOG(LM_EMERGENCY, "Invalid format in %s", (trircd_AjoinDBName));

		trircd_NickExt ne;
		memset(&ne, 0, sizeof(trircd_NickExt));
                trircd_SAFE(trircd_read_buffer(mynick, f));
                trircd_SAFE(trircd_read_int16(&ne.ajoincount, f));
                if (ne.ajoincount) {
                    char **ajoin;
                    ajoin = (char **) malloc(sizeof(char *) * ne.ajoincount);
                    ne.ajoin = ajoin;
                    for (j = 0; j < ne.ajoincount; j++, ajoin++)
                        trircd_SAFE(trircd_read_string(ajoin, f));
                }
                trircd_SAFE(trircd_read_ptr((void **)&ne.forbidinfo, f));
                if(ne.forbidinfo) {
                    trircd_ForbidInfo *fi = (trircd_ForbidInfo *) malloc(sizeof(*fi));
                    trircd_SAFE(trircd_read_buffer(fi->forbidder, f));
                    trircd_SAFE(trircd_read_string(&fi->forbid_message, f));
                    ne.forbidinfo = fi;
                }
                trircd_SAFE(trircd_read_int32(&ne.authcode, f));   
                trircd_SAFE(trircd_read_int16(&ne.authmode, f));   
                trircd_SAFE(trircd_read_int16(&ne.ignorecount, f));
                if (ne.ignorecount) {
                    ne.ignored = (trircd_IgnoreInfo *) calloc(ne.ignorecount, sizeof(trircd_IgnoreInfo));
                    for (j = 0; j < ne.ignorecount; j++) {   
                        trircd_SAFE(trircd_read_int16(&ne.ignored[j].in_use, f));
                        if (ne.ignored[j].in_use) {
                            trircd_SAFE(trircd_read_string(&ne.ignored[j].ign, f));
                        }
                    }
                } else {
                    ne.ignored = NULL;
                }
                trircd_SAFE(trircd_read_string(&ne.infomsg, f));
                if (!ne.infomsg)
                    ne.infomsg = strdup("");
		trircd_SAFE(trircd_read_int32(&tmp32, f));
		    ne.expiredelay = tmp32;
        	trircd_SAFE(trircd_read_int16(&tmp16, f));
        	if (tmp16) {
            	    for (i = 0; i < tmp16; i++) {
                	trircd_SAFE(trircd_read_int32(&tmp32, f));
            	    }
        	}

		Convert::trircd_UpdateNickEntry(&ne, mynick);

                if (ne.ajoincount) {
                    char **ajoin;
                    ajoin = ne.ajoin;
                    for (j = 0; j < ne.ajoincount; j++, ajoin++)
                        trircd_SAFE(trircd_read_string(ajoin, f));
		    free(ne.ajoin);
		}
		if (ne.forbidinfo)
		{
		    free(ne.forbidinfo->forbid_message);
		    free(ne.forbidinfo);
		}
                if (ne.ignorecount) {
                    for (j = 0; j < ne.ignorecount; j++)
			free(ne.ignored[j].ign);
		    free(ne.ignored);
		}
		if (ne.infomsg)
		    free(ne.infomsg);
            }
        }
        break;
      case 24:
        for (i = 0; i < 256 && !failed; i++) {
            while ((c = trircd_getc_db(f)) == 1) {
                if (c != 1)
                    SLOG(LM_EMERGENCY, "Invalid format in %s", (trircd_AjoinDBName));
                trircd_SAFE(trircd_read_buffer(mynick, f));

		trircd_NickExt ne;
		memset(&ne, 0, sizeof(trircd_NickExt));
                trircd_SAFE(trircd_read_int16(&ne.ajoincount, f));
                if (ne.ajoincount) {
                    char **ajoin;
                    ajoin = (char **) malloc(sizeof(char *) * ne.ajoincount);
                    ne.ajoin = ajoin;
                    for (j = 0; j < ne.ajoincount; j++, ajoin++)
                        trircd_SAFE(trircd_read_string(ajoin, f));
                }
                trircd_SAFE(trircd_read_ptr((void **)&ne.forbidinfo, f));
                if(ne.forbidinfo) {
                    trircd_ForbidInfo *fi = (trircd_ForbidInfo *) malloc(sizeof(*fi));
                    trircd_SAFE(trircd_read_buffer(fi->forbidder, f));
                    trircd_SAFE(trircd_read_string(&fi->forbid_message, f));
                    ne.forbidinfo = fi;
                }
                trircd_SAFE(trircd_read_int32(&ne.authcode, f));
                trircd_SAFE(trircd_read_int16(&ne.authmode, f));
                trircd_SAFE(trircd_read_int16(&ne.ignorecount, f));
                if (ne.ignorecount) {
                    ne.ignored = (trircd_IgnoreInfo *) calloc(ne.ignorecount, sizeof(trircd_IgnoreInfo));
                    for (j = 0; j < ne.ignorecount; j++) {
                        trircd_SAFE(trircd_read_int16(&ne.ignored[j].in_use, f));
                        if (ne.ignored[j].in_use) {
                            trircd_SAFE(trircd_read_string(&ne.ignored[j].ign, f));
                        }
                    }
                } else {
                    ne.ignored = NULL;
                }
		trircd_SAFE(trircd_read_string(&ne.infomsg, f));
		if (!ne.infomsg)
        	    ne.infomsg = strdup("");

		Convert::trircd_UpdateNickEntry(&ne, mynick);

                if (ne.ajoincount) {
                    char **ajoin;
                    ajoin = ne.ajoin;
                    for (j = 0; j < ne.ajoincount; j++, ajoin++)
                        trircd_SAFE(trircd_read_string(ajoin, f));
		    free(ne.ajoin);
		}
		if (ne.forbidinfo)
		{
		    free(ne.forbidinfo->forbid_message);
		    free(ne.forbidinfo);
		}
                if (ne.ignorecount) {
                    for (j = 0; j < ne.ignorecount; j++)
			free(ne.ignored[j].ign);
		    free(ne.ignored);
		}
		if (ne.infomsg)
		    free(ne.infomsg);
            }
        }
	break;
      case 23:
        for (i = 0; i < 256 && !failed; i++) {
            while ((c = trircd_getc_db(f)) == 1) {
                if (c != 1)
                    SLOG(LM_EMERGENCY, "Invalid format in %s", (trircd_AjoinDBName));
                trircd_SAFE(trircd_read_buffer(mynick, f));

		trircd_NickExt ne;
		memset(&ne, 0, sizeof(trircd_NickExt));
                trircd_SAFE(trircd_read_int16(&ne.ajoincount, f));
                if (ne.ajoincount) {
                    char **ajoin;
                    ajoin = (char **) malloc(sizeof(char *) * ne.ajoincount);
                    ne.ajoin = ajoin;
                    for (j = 0; j < ne.ajoincount; j++, ajoin++)
                        trircd_SAFE(trircd_read_string(ajoin, f));
                }
                trircd_SAFE(trircd_read_ptr((void **)&ne.forbidinfo, f));
                if(ne.forbidinfo) {
                    trircd_ForbidInfo *fi = (trircd_ForbidInfo *) malloc(sizeof(*fi));
                    trircd_SAFE(trircd_read_buffer(fi->forbidder, f));
                    trircd_SAFE(trircd_read_string(&fi->forbid_message, f));
                    ne.forbidinfo = fi;
                }
                trircd_SAFE(trircd_read_int32(&ne.authcode, f));
                trircd_SAFE(trircd_read_int16(&ne.authmode, f));
                trircd_SAFE(trircd_read_int16(&ne.ignorecount, f));
                if (ne.ignorecount) {
                    ne.ignored = (trircd_IgnoreInfo *) calloc(ne.ignorecount, sizeof(trircd_IgnoreInfo));
                    for (j = 0; j < ne.ignorecount; j++) {
			trircd_SAFE(trircd_read_int16(&ne.ignored[j].in_use, f));
			if (ne.ignored[j].in_use) {
                            trircd_SAFE(trircd_read_string(&ne.ignored[j].ign, f));
			}
		    }
                } else {
		    ne.ignored = NULL;
		}

		Convert::trircd_UpdateNickEntry(&ne, mynick);

                if (ne.ajoincount) {
                    char **ajoin;
                    ajoin = ne.ajoin;
                    for (j = 0; j < ne.ajoincount; j++, ajoin++)
                        trircd_SAFE(trircd_read_string(ajoin, f));
		    free(ne.ajoin);
		}
		if (ne.forbidinfo)
		{
		    free(ne.forbidinfo->forbid_message);
		    free(ne.forbidinfo);
		}
                if (ne.ignorecount) {
                    for (j = 0; j < ne.ignorecount; j++)
			free(ne.ignored[j].ign);
		    free(ne.ignored);
		}
            }
        }
        break;
      case 22:
        for (i = 0; i < 256 && !failed; i++) {
            while ((c = trircd_getc_db(f)) == 1) {
                if (c != 1)
                    SLOG(LM_EMERGENCY, "Invalid format in %s", (trircd_AjoinDBName));
                trircd_SAFE(trircd_read_buffer(mynick, f));

		trircd_NickExt ne;
		memset(&ne, 0, sizeof(trircd_NickExt));
                trircd_SAFE(trircd_read_int16(&ne.ajoincount, f));
                if (ne.ajoincount) {
                    char **ajoin;
                    ajoin = (char **) malloc(sizeof(char *) * ne.ajoincount);
                    ne.ajoin = ajoin;
                    for (j = 0; j < ne.ajoincount; j++, ajoin++)
                        trircd_SAFE(trircd_read_string(ajoin, f)); 
                }
                trircd_SAFE(trircd_read_ptr((void **)&ne.forbidinfo, f));
                if(ne.forbidinfo) {
                    trircd_ForbidInfo *fi = (trircd_ForbidInfo *) malloc(sizeof(*fi));
                    trircd_SAFE(trircd_read_buffer(fi->forbidder, f));
                    trircd_SAFE(trircd_read_string(&fi->forbid_message, f));
                    ne.forbidinfo = fi;
                }
                trircd_SAFE(trircd_read_int32(&ne.authcode, f));
                trircd_SAFE(trircd_read_int16(&ne.authmode, f));

		Convert::trircd_UpdateNickEntry(&ne, mynick);

                if (ne.ajoincount) {
                    char **ajoin;
                    ajoin = ne.ajoin;
                    for (j = 0; j < ne.ajoincount; j++, ajoin++)
                        trircd_SAFE(trircd_read_string(ajoin, f));
		    free(ne.ajoin);
		}
		if (ne.forbidinfo)
		{
		    free(ne.forbidinfo->forbid_message);
		    free(ne.forbidinfo);
		}
            } 
        }
    
    
        break;   
    
      default:
        SLOG(LM_EMERGENCY, "Unsupported version number (%d) on %s", (ver, trircd_AjoinDBName));
      
    }
      
    trircd_close_db(f);
}
                
#undef trircd_SAFE

int trircd_delnick(trircd_NickInfo *ni)
{
    int i;

    if (ni->suspendinfo) {
	if (ni->suspendinfo->reason)
	    free(ni->suspendinfo->reason);
	free(ni->suspendinfo);
    }
    if (ni->last_usermask)
	free(ni->last_usermask);
    if (ni->last_realname)
	free(ni->last_realname);
    if (ni->last_quit)
        free(ni->last_quit);
    if (ni->access) {
	for (i = 0; i < ni->accesscount; i++) {
	    if (ni->access[i])
		free(ni->access[i]);
	}
	free(ni->access);
    }
    if (ni->memos.memos) {
	for (i = 0; i < ni->memos.memocount; i++) {
	    if (ni->memos.memos[i].text)
		free(ni->memos.memos[i].text);
	}
	free(ni->memos.memos);
    }
    free(ni);
    return 1;
}


#define trircd_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( trircd_ChanDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void trircd_load_old_chan(trircd_dbFILE *f, int ver)
{
    static_cast<void>(ver);
    int i, j, c;
    trircd_ChanInfo *ci;
    int failed = 0;

    struct {
	short level;
#ifdef trircd_COMPATIBILITY_V2
	short is_nick;
#else
	short in_use;
#endif
	char *name;
    } old_chanaccess;

    struct {
	short is_nick;
	short pad;
	char *name;
	char *reason;
    } old_autokick;

    struct {
	trircd_ChanInfo *next, *prev;
	char name[trircd_CHANMAX];
	char founder[trircd_NICKMAX];
	char founderpass[trircd_PASSMAX];
	char *desc;
	time_t time_registered;
	time_t last_used;
	long accesscount;
	trircd_ChanAccess *access;
	long akickcount;
	trircd_AutoKick *akick;
	short mlock_on, mlock_off;
	long mlock_limit;
	char *mlock_key;
	char *last_topic;
	char last_topic_setter[trircd_NICKMAX];
	time_t last_topic_time;
	long flags;
	short *levels;
	char *url;
	char *email;
	struct channel_ *c;
    } old_channelinfo;


    for (i = 33; i < 256 && !failed; i++) {
	while ((c = trircd_getc_db(f)) != 0) {
	    if (c != 1)
		SLOG(LM_EMERGENCY, "Invalid format in %s",( trircd_ChanDBName));
	    trircd_SAFE(trircd_read_variable(old_channelinfo, f));
	    ci = (trircd_ChanInfo *) calloc(1, sizeof(trircd_ChanInfo));
	    strncpy(ci->name, old_channelinfo.name, trircd_CHANMAX);
	    ci->founder = strdup(old_channelinfo.founder);
	    strncpy(ci->founderpass, old_channelinfo.founderpass, trircd_PASSMAX);
	    ci->time_registered = old_channelinfo.time_registered;
	    ci->last_used = old_channelinfo.last_used;
	    ci->accesscount = old_channelinfo.accesscount;
	    ci->akickcount = old_channelinfo.akickcount;
	    ci->mlock_on = old_channelinfo.mlock_on;
	    ci->mlock_off = old_channelinfo.mlock_off;
	    ci->mlock_limit = old_channelinfo.mlock_limit;
	    strncpy(ci->last_topic_setter,
			old_channelinfo.last_topic_setter, trircd_NICKMAX);
	    ci->last_topic_time = old_channelinfo.last_topic_time;
	    if (ci->flags & trircd_CI_ENCRYPTEDPW) {
		/* Bail: it makes no sense to continue with encrypted
		 * passwords, since we won't be able to verify them */
		SLOG(LM_EMERGENCY, "%s: load database: password for %s encrypted "
		          "but encryption disabled, aborting",(
		          "ChanServ", ci->name));
	    }
	    trircd_SAFE(trircd_read_string(&ci->desc, f));
	    if (!ci->desc)
		ci->desc = strdup("");
	    if (old_channelinfo.url)
		trircd_SAFE(trircd_read_string(&ci->url, f));
	    if (old_channelinfo.email)
		trircd_SAFE(trircd_read_string(&ci->email, f));
	    if (old_channelinfo.mlock_key)
		trircd_SAFE(trircd_read_string(&ci->mlock_key, f));
	    if (old_channelinfo.last_topic)
		trircd_SAFE(trircd_read_string(&ci->last_topic, f));

	    if (ci->accesscount) {
		trircd_ChanAccess *access;

		access = (trircd_ChanAccess *) malloc(sizeof(trircd_ChanAccess) * ci->accesscount);
		ci->access = access;
		for (j = 0; j < ci->accesscount; j++, access++) {
		    trircd_SAFE(trircd_read_variable(old_chanaccess, f));
#ifdef trircd_COMPATIBILITY_V2
		    if (old_chanaccess.is_nick < 0)
			access->in_use = 0;
		    else
			access->in_use = old_chanaccess.is_nick;
#else
		    access->in_use = old_chanaccess.in_use;
#endif
		    access->level = old_chanaccess.level;
		}
		access = ci->access;
		for (j = 0; j < ci->accesscount; j++, access++) {
		    trircd_SAFE(trircd_read_string(&access->ni, f));
		    if (access->ni == NULL)
			access->in_use = 0;
		}
	    } else {
		ci->access = NULL;
	    } /* if (ci->accesscount) */

	    if (ci->akickcount) {
		trircd_AutoKick *akick;
		char *s;

		akick = (trircd_AutoKick *) malloc(sizeof(trircd_AutoKick) * ci->akickcount);
		ci->akick = akick;
		for (j = 0; j < ci->akickcount; j++, akick++) {
		    trircd_SAFE(trircd_read_variable(old_autokick, f));
		    if (old_autokick.is_nick < 0) {
			akick->in_use = 0;
			akick->is_nick = 0;
		    } else {
			akick->in_use = 1;
			akick->is_nick = old_autokick.is_nick;
		    }
		    akick->reason = old_autokick.reason;
		}
		akick = ci->akick;
		for (j = 0; j < ci->akickcount; j++, akick++) {
		    trircd_SAFE(trircd_read_string(&s, f));
		    if (akick->is_nick) {
			akick->u.ni = s;
		    } else {
			akick->u.mask = s;
		    }
		    if (akick->reason)
			trircd_SAFE(trircd_read_string(&akick->reason, f));
		    if (!akick->in_use) {
			if (akick->is_nick) {
			    akick->u.ni = NULL;
			} else {
			    free(akick->u.mask);
			    akick->u.mask = NULL;
			}
			if (akick->reason) {
			    free(akick->reason);
			    akick->reason = NULL;
			}
		    }
		}
	    } else {
		ci->akick = NULL;
	    } /* if (ci->akickcount) */

	    if (old_channelinfo.levels) {
		int16 n_entries;
		ci->levels = NULL;
		trircd_reset_levels(ci);
		trircd_SAFE(trircd_read_int16(&n_entries, f));
#ifdef trircd_COMPATIBILITY_V2
		/* Ignore earlier, incompatible levels list */
		if (n_entries == 6) {
		    fseek(f, sizeof(short) * n_entries, SEEK_CUR);
		} else
#endif
		for (j = 0; j < n_entries; j++) {
		    short lev;
		    trircd_SAFE(trircd_read_variable(lev, f));
		    if (j < trircd_CA_SIZE)
			ci->levels[j] = lev;
		}
	    } else {
		trircd_reset_levels(ci);
	    }

	    ci->memos.memomax = trircd_MSMaxMemos;

	    Chan_Stored_t *chan = Convert::trircd_CreateChanEntry(ci);
	    if (chan != NULL)
		Magick::instance().chanserv.AddStored(chan);
	    trircd_delchan(ci);

	} /* while (trircd_getc_db(f) != 0) */
    } /* for (i) */
}

#undef trircd_SAFE

/*************************************************************************/

#define trircd_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( trircd_ChanDBName));	\
	return NULL;					\
    }							\
} while (0)

trircd_ChanInfo *trircd_load_chan_record(trircd_dbFILE *f, int ver)
{
    trircd_ChanInfo *ci;
    int16 tmp16;
    int32 tmp32;
    int n_levels;
    char *s;
    int i;

    ci = (trircd_ChanInfo *) calloc(sizeof(trircd_ChanInfo), 1);
    trircd_SAFE(trircd_read_buffer(ci->name, f));
    trircd_SAFE(trircd_read_string(&ci->founder, f));
    if (ver >= 7) {
	trircd_SAFE(trircd_read_string(&ci->successor, f));
	/* Founder could be successor, which is bad, in vers 7,8 */
	if (strcmp(ci->founder, ci->successor)==0)
	{
	    free(ci->successor);
	    ci->successor = NULL;
	}
    } else {
	ci->successor = NULL;
    }
    trircd_SAFE(trircd_read_buffer(ci->founderpass, f));
    trircd_SAFE(trircd_read_string(&ci->desc, f));
    if (!ci->desc)
	ci->desc = strdup("");
    trircd_SAFE(trircd_read_string(&ci->url, f));
    trircd_SAFE(trircd_read_string(&ci->email, f));
    trircd_SAFE(trircd_read_int32(&tmp32, f));
    ci->time_registered = tmp32;
    trircd_SAFE(trircd_read_int32(&tmp32, f));
    ci->last_used = tmp32;
    trircd_SAFE(trircd_read_string(&ci->last_topic, f));
    trircd_SAFE(trircd_read_buffer(ci->last_topic_setter, f));
    trircd_SAFE(trircd_read_int32(&tmp32, f));
    ci->last_topic_time = tmp32;
    trircd_SAFE(trircd_read_int32(&ci->flags, f));
    if (ver >= 9)
	trircd_SAFE(trircd_read_ptr((void **)&ci->suspendinfo, f));
    if (ci->suspendinfo) {
	trircd_SuspendInfo *si = (trircd_SuspendInfo *) malloc(sizeof(*si));
	trircd_SAFE(trircd_read_buffer(si->who, f));
	trircd_SAFE(trircd_read_string(&si->reason, f));
	trircd_SAFE(trircd_read_int32(&tmp32, f));
	si->suspended = tmp32;
	trircd_SAFE(trircd_read_int32(&tmp32, f));
	si->expires = tmp32;
	ci->suspendinfo = si;
    }
    if (ci->flags & trircd_CI_ENCRYPTEDPW) {
	/* Bail: it makes no sense to continue with encrypted
	 * passwords, since we won't be able to verify them */
	SLOG(LM_EMERGENCY, "%s: load database: password for %s encrypted "
	      "but encryption disabled, aborting",(
	      "ChanServ", ci->name));
    }
    trircd_SAFE(trircd_read_int16(&tmp16, f));
    n_levels = tmp16;
    trircd_reset_levels(ci);
    for (i = 0; i < n_levels; i++) {
	if (i < trircd_CA_SIZE)
	    trircd_SAFE(trircd_read_int16(&ci->levels[i], f));
	else
	    trircd_SAFE(trircd_read_int16(&tmp16, f));
    }

    trircd_SAFE(trircd_read_int16(&ci->accesscount, f));
    if (ci->accesscount) {
	ci->access = (trircd_ChanAccess *) calloc(ci->accesscount, sizeof(trircd_ChanAccess));
	for (i = 0; i < ci->accesscount; i++) {
	    trircd_SAFE(trircd_read_int16(&ci->access[i].in_use, f));
	    if (ci->access[i].in_use) {
		trircd_SAFE(trircd_read_int16(&ci->access[i].level, f));
		trircd_SAFE(trircd_read_string(&ci->access[i].ni, f));
	    }
	}
    } else {
	ci->access = NULL;
    }

    trircd_SAFE(trircd_read_int16(&ci->akickcount, f));
    if (ci->akickcount) {
	ci->akick = (trircd_AutoKick *) calloc(ci->akickcount, sizeof(trircd_AutoKick));
	for (i = 0; i < ci->akickcount; i++) {
	    trircd_SAFE(trircd_read_int16(&ci->akick[i].in_use, f));
	    if (ci->akick[i].in_use) {
		trircd_SAFE(trircd_read_int16(&ci->akick[i].is_nick, f));
		trircd_SAFE(trircd_read_string(&s, f));
		if (ci->akick[i].is_nick) {
		    ci->akick[i].u.ni = s;
		} else {
		    ci->akick[i].u.mask = s;
		}
		trircd_SAFE(trircd_read_string(&s, f));
		if (ci->akick[i].in_use)
		    ci->akick[i].reason = s;
		else if (s)
		    free(s);
		if (ver >= 8)
		    trircd_SAFE(trircd_read_buffer(ci->akick[i].who, f));
		else
		    ci->akick[i].who[0] = '\0';
	    }
	}
    } else {
	ci->akick = NULL;
    }

    if (ver < 10) {
	trircd_SAFE(trircd_read_int16(&tmp16, f));
	ci->mlock_on = tmp16;
	trircd_SAFE(trircd_read_int16(&tmp16, f));
	ci->mlock_off = tmp16;
    } else {
	trircd_SAFE(trircd_read_int32(&ci->mlock_on, f));
	trircd_SAFE(trircd_read_int32(&ci->mlock_off, f));
    }
    trircd_SAFE(trircd_read_int32(&ci->mlock_limit, f));
    trircd_SAFE(trircd_read_string(&ci->mlock_key, f));
    ci->mlock_on &= ~trircd_CMODE_r;  /* check_modes() takes care of this */

    trircd_SAFE(trircd_read_int16(&ci->memos.memocount, f));
    trircd_SAFE(trircd_read_int16(&ci->memos.memomax, f));
    if (ci->memos.memocount) {
	trircd_Memo *memos;
	memos = (trircd_Memo *) malloc(sizeof(trircd_Memo) * ci->memos.memocount);
	ci->memos.memos = memos;
	for (i = 0; i < ci->memos.memocount; i++, memos++) {
	    trircd_SAFE(trircd_read_int32(&memos->number, f));
	    trircd_SAFE(trircd_read_int16(&memos->flags, f));
	    trircd_SAFE(trircd_read_int32(&tmp32, f));
	    memos->time = tmp32;
	    trircd_SAFE(trircd_read_buffer(memos->sender, f));
	    trircd_SAFE(trircd_read_string(&memos->text, f));
	}
    }

    trircd_SAFE(trircd_read_string(&ci->entry_message, f));

    ci->c = NULL;

    return ci;
}

#undef trircd_SAFE

/*************************************************************************/

void trircd_load_chan()
{
    trircd_dbFILE *f;
    int ver, i, c;
    trircd_ChanInfo *ci;
    int failed = 0;

    if (!(f = trircd_open_db("ChanServ", trircd_ChanDBName, "r")))
	return;

    switch (ver = trircd_get_file_version(f)) {
      case 25:
      case 24:
      case 23:
      case 22:
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
	for (i = 0; i < 256 && !failed; i++) {
	    while ((c = trircd_getc_db(f)) != 0) {
		if (c != 1)
		    SLOG(LM_EMERGENCY, "Invalid format in %s",( trircd_ChanDBName));
		ci = trircd_load_chan_record(f, ver);
		if (ci) {
		    Chan_Stored_t *chan = Convert::trircd_CreateChanEntry(ci);
		    if (chan != NULL)
		    {
			Magick::instance().chanserv.AddStored(chan);
			if (ci->memos.memocount)
			{
			    MemoServ::channel_news_t news = Convert::trircd_CreateNewsEntry(&ci->memos, ci->name);
			    if (news.size())
				Magick::instance().memoserv.AddChannel(news);
			}
		    }
		    trircd_delchan(ci);
		} else {
		    failed = 1;
		    break;
		}
	    }
	}
	break;

      case 4:
      case 3:
      case 2:
      case 1:
	trircd_load_old_chan(f, ver);
	break;

      case -1:
	SLOG(LM_EMERGENCY, "Unable to read version number from %s",( trircd_ChanDBName));

      default:
	SLOG(LM_EMERGENCY, "Unsupported version number (%d) on %s",( ver, trircd_ChanDBName));

    } /* switch (version) */

    trircd_close_db(f);

}

#undef trircd_SAFE

#define trircd_SAFE(x) do {                                    \
    if ((x) < 0) {                                      \
            SLOG(LM_EMERGENCY, "Read error on %s", (trircd_CForbidDBName));     \
        failed = 1;                                     \
        break;                                          \
    }                                                   \
} while (0)   
    
            
void trircd_load_chanext()
{
    trircd_dbFILE *f;
    int ver, i, c;
    char mychan[trircd_CHANMAX];
    int failed = 0;
    int16 tmp16 = 0;
    int16 tmp16_ = 0;
    int32 tmp32 = 0;
        
    if (!(f = trircd_open_db("ChanServ", trircd_CForbidDBName, "r")))
        return;
    switch (ver = trircd_get_file_version(f)) {
      case 25:
        for (i = 0; i < 256 && !failed; i++) {  
            while ((c = trircd_getc_db(f)) == 1) {
                if (c != 1)
                    SLOG(LM_EMERGENCY, "Invalid format in %s", (trircd_CForbidDBName));
                trircd_SAFE(trircd_read_buffer(mychan, f));

		trircd_ChanExt ce;
		memset(&ce, 0, sizeof(trircd_ChanExt));
                trircd_SAFE(trircd_read_ptr((void **)&ce.forbidinfo, f));
                if (ce.forbidinfo) {
                    trircd_ForbidInfo *fi = (trircd_ForbidInfo *) malloc(sizeof(*fi));
                    trircd_SAFE(trircd_read_buffer(fi->forbidder, f));
                    trircd_SAFE(trircd_read_string(&fi->forbid_message, f));
                    ce.forbidinfo = fi;
                }
   	 	trircd_SAFE(trircd_read_int16(&tmp16, f));
    		if (tmp16) {
		    ce.akick = (trircd_AutoKickExt *) malloc(sizeof(*ce.akick) * tmp16); 
        	    for (i = 0; i < tmp16; i++) {
            		trircd_SAFE(trircd_read_int16(&tmp16_, f));
            		if (tmp16_) {
			    trircd_SAFE(trircd_read_int32(&tmp32, f));
			    ce.akick[i].time_added = tmp32;
			    trircd_SAFE(trircd_read_int32(&tmp32, f));
			    ce.akick[i].last_used = tmp32;
			    trircd_SAFE(trircd_read_int32(&tmp32, f));
			    ce.akick[i].expires = tmp32;			    
            		}
        	    }
    		}
    		trircd_SAFE(trircd_read_int16(&tmp16, f));
    		if (tmp16) {
		    ce.access = (trircd_ChanAccessExt *) malloc(sizeof(*ce.access) * tmp16); 
        	    for (i = 0; i < ce.accesscount; i++) {
            		trircd_SAFE(trircd_read_int16(&tmp16_, f));
            		if (tmp16_) {
                	    trircd_SAFE(trircd_read_int32(&tmp32, f));
			    ce.access[i].last_used = tmp32;
                	}
            	    }
        	}

		Convert::trircd_UpdateChanEntry(&ce, mychan);

		if (ce.forbidinfo)
		{
		    free(ce.forbidinfo->forbid_message);
		    free(ce.forbidinfo);
		}
            }
        }
        break;

      case 24:
      case 23:
      case 22:
        for (i = 0; i < 256 && !failed; i++) {
            while ((c = trircd_getc_db(f)) == 1) {
                if (c != 1)
                    SLOG(LM_EMERGENCY, "Invalid format in %s", (trircd_CForbidDBName));
                trircd_SAFE(trircd_read_buffer(mychan, f));

		trircd_ChanExt ce;
		memset(&ce, 0, sizeof(trircd_ChanExt));
                trircd_SAFE(trircd_read_ptr((void **)&ce.forbidinfo, f));
                if (ce.forbidinfo) {
                    trircd_ForbidInfo *fi = (trircd_ForbidInfo *) malloc(sizeof(*fi));
                    trircd_SAFE(trircd_read_buffer(fi->forbidder, f));
                    trircd_SAFE(trircd_read_string(&fi->forbid_message, f));
                    ce.forbidinfo = fi;
                }

		Convert::trircd_UpdateChanEntry(&ce, mychan);

		if (ce.forbidinfo)
		{
		    free(ce.forbidinfo->forbid_message);
		    free(ce.forbidinfo);
		}
            } 
        }

        break;

      default:
        SLOG(LM_EMERGENCY, "Unsupported version number (%d) on %s", (ver, trircd_CForbidDBName));
                
    }
                
    trircd_close_db(f);
}
                
#undef trircd_SAFE

int trircd_delchan(trircd_ChanInfo *ci)
{
    int i;
   
    if (ci->desc)
	free(ci->desc);
    if (ci->mlock_key)
	free(ci->mlock_key);
    if (ci->last_topic)
	free(ci->last_topic);
    if (ci->suspendinfo) {
	if (ci->suspendinfo->reason)
	    free(ci->suspendinfo->reason);
	free(ci->suspendinfo);
    }
    if (ci->access)
	free(ci->access);
    for (i = 0; i < ci->akickcount; i++) {
	if (!ci->akick[i].is_nick && ci->akick[i].u.mask)
	    free(ci->akick[i].u.mask);
	if (ci->akick[i].reason)
	    free(ci->akick[i].reason);
    }
    if (ci->akick)
	free(ci->akick);
    if (ci->levels)
	free(ci->levels);
    if (ci->memos.memos) {
	for (i = 0; i < ci->memos.memocount; i++) {
	    if (ci->memos.memos[i].text)
		free(ci->memos.memos[i].text);
	}
	free(ci->memos.memos);
    }
    free(ci);

    return 1;
}

void trircd_reset_levels(trircd_ChanInfo *ci)
{
    int i;

    if (ci->levels)
	free(ci->levels);
    ci->levels = (int16 *) calloc(trircd_CA_SIZE, sizeof(*ci->levels));
    for (i = 0; trircd_def_levels[i][0] >= 0; i++)
	ci->levels[trircd_def_levels[i][0]] = trircd_def_levels[i][1];
}

mstring trircd_getmodes(short modes)
{
    BTCB();
    mstring retval;

    if (modes & trircd_CMODE_i)
	retval += "i";
    if (modes & trircd_CMODE_m)
	retval += "m";
    if (modes & trircd_CMODE_n)
	retval += "n";
    if (modes & trircd_CMODE_p)
	retval += "p";
    if (modes & trircd_CMODE_s)
	retval += "s";
    if (modes & trircd_CMODE_t)
	retval += "t";
    if (modes & trircd_CMODE_k)
	retval += "k";
    if (modes & trircd_CMODE_l)
	retval += "l";
    if (modes & trircd_CMODE_R)
	retval += "R";
    if (modes & trircd_CMODE_r)
	retval += "r";
    if (modes & trircd_CMODE_c)
	retval += "c";
    if (modes & trircd_CMODE_O)
	retval += "O";
    if (modes & trircd_CMODE_A)
	retval += "A";
    if (modes & trircd_CMODE_z)
	retval += "z";
    if (modes & trircd_CMODE_Q)
	retval += "Q";
    if (modes & trircd_CMODE_K)
	retval += "K";
    if (modes & trircd_CMODE_V)
	retval += "V";
    if (modes & trircd_CMODE_H)
	retval += "H";
    if (modes & trircd_CMODE_C)
	retval += "C";
    if (modes & trircd_CMODE_N)
	retval += "N";
    if (modes & trircd_CMODE_S)
	retval += "S";
    if (modes & trircd_CMODE_G)
	retval += "G";
    if (modes & trircd_CMODE_u)
	retval += "u";
    if (modes & trircd_CMODE_f)
	retval += "f";
    if (modes & trircd_CMODE_M)
	retval += "M";

    return retval;
    ETCB();
}


#define trircd_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( trircd_NewsDBName));	\
	nnews = i;					\
	break;						\
    }							\
} while (0)

void trircd_load_news()
{
    trircd_dbFILE *f;
    int i, nnews;
    int16 n;
    int32 tmp32;
    trircd_NewsItem news;

    if (!(f = trircd_open_db("OperServ", trircd_NewsDBName, "r")))
	return;
    switch (i = trircd_get_file_version(f)) {
      case 25:
      case 24:
      case 23:
      case 22:
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
	trircd_SAFE(trircd_read_int16(&n, f));
	nnews = n;
	if (!nnews) {
	    trircd_close_db(f);
	    return;
	}
	for (i = 0; i < nnews; i++) {
	    trircd_SAFE(trircd_read_int16(&news.type, f));
	    trircd_SAFE(trircd_read_int32(&news.num, f));
	    trircd_SAFE(trircd_read_string(&news.text, f));
	    trircd_SAFE(trircd_read_buffer(news.who, f));
	    trircd_SAFE(trircd_read_int32(&tmp32, f));
	    news.time = tmp32;

	    if (news.type == trircd_NEWS_LOGON && Magick::instance().commserv.IsList(Magick::instance().commserv.ALL_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.ALL_Name())->
		    MSG_insert(mstring(news.text), mstring(news.who), mDateTime(news.time));
	    }
	    else if (news.type == trircd_NEWS_OPER && Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->
		    MSG_insert(mstring(news.text), mstring(news.who), mDateTime(news.time));
	    }

	    free(news.text);
	}
	break;

      case -1:
	SLOG(LM_EMERGENCY, "Unable to read version number from %s",( trircd_NewsDBName));

      default:
	SLOG(LM_EMERGENCY, "Unsupported version (%d) on %s",( i, trircd_NewsDBName));
    } /* switch (ver) */

    trircd_close_db(f);
}

#undef trircd_SAFE

#define trircd_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( trircd_OperDBName));	\
	break;						\
    }							\
} while (0)

void trircd_load_oper()
{
    trircd_dbFILE *f;
    int16 i, n;
    int32 ver;
    char *s;

    if (!(f = trircd_open_db("OperServ", trircd_OperDBName, "r")))
	return;
    switch (ver = trircd_get_file_version(f)) {
      case 25:
      case 24:
      case 23:
      case 22:
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
	trircd_SAFE(trircd_read_int16(&n, f));
	for (i = 0; i < n; i++) {
	    trircd_SAFE(trircd_read_string(&s, f));
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());
	    free(s);
	}
	trircd_SAFE(trircd_read_int16(&n, f));
	for (i = 0; i < n; i++) {
	    trircd_SAFE(trircd_read_string(&s, f));
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());
	    free(s);
	}
	if (ver >= 7) {
	    int32 tmp32;
	    trircd_SAFE(trircd_read_int32(&tmp32, f));
	    trircd_SAFE(trircd_read_int32(&tmp32, f));
	}
	if (ver >= 21 || ver == 9 || ver == 10 || ver == 11) {
	    char no_supass;
	    char supass[trircd_PASSMAX];
	    trircd_SAFE(trircd_read_int8(&no_supass, f));
	    if (!no_supass)
		trircd_SAFE(trircd_read_buffer(supass, f));
	}
	break;

      case 4:
      case 3:
	trircd_SAFE(trircd_read_int16(&n, f));
	for (i = 0; i < n; i++) {
	    trircd_SAFE(trircd_read_string(&s, f));
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());
	    free(s);
	}
	break;

      case -1:
	SLOG(LM_EMERGENCY, "Unable to read version number from %s",( trircd_OperDBName));

      default:
	SLOG(LM_EMERGENCY, "Unsupported version (%d) on %s",( ver, trircd_OperDBName));
    } /* switch (version) */
    trircd_close_db(f);
}

#undef trircd_SAFE

#define trircd_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( trircd_ExceptionDBName));	\
	nexceptions = i;				\
	break;						\
    }							\
} while (0)

void trircd_load_exception()
{
    trircd_dbFILE *f;
    int i, nexceptions;
    int16 n;
    int16 tmp16;
    int32 tmp32;
    trircd_Exception exception;

    if (!(f = trircd_open_db("OperServ", trircd_ExceptionDBName, "r")))
	return;
    switch (i = trircd_get_file_version(f)) {
      case 25:
      case 24:
      case 23:
      case 22:
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
	trircd_SAFE(trircd_read_int16(&n, f));
	nexceptions = n;
	if (!nexceptions) {
	    trircd_close_db(f);
	    return;
	}
	for (i = 0; i < nexceptions; i++) {
	    trircd_SAFE(trircd_read_string(&exception.mask, f));
	    trircd_SAFE(trircd_read_int16(&tmp16, f));
	    exception.limit = tmp16;
	    trircd_SAFE(trircd_read_buffer(exception.who, f));
	    trircd_SAFE(trircd_read_string(&exception.reason, f));
	    trircd_SAFE(trircd_read_int32(&tmp32, f));
	    exception.time = tmp32;
	    trircd_SAFE(trircd_read_int32(&tmp32, f));
	    exception.expires = tmp32;
	    exception.num = i; /* Symbolic position, never saved. */

	    Magick::instance().operserv.Clone_insert(mstring(exception.mask).After("@"), exception.limit,
			mstring(exception.reason), mstring(exception.who), mDateTime(exception.time));

	    free(exception.mask);
	    free(exception.reason);
	}
	break;

      case -1:
	SLOG(LM_EMERGENCY, "Unable to read version number from %s",( trircd_ExceptionDBName));

      default:
	SLOG(LM_EMERGENCY, "Unsupported version (%d) on %s",( i, trircd_ExceptionDBName));
    } /* switch (ver) */

    trircd_close_db(f);
}

#undef trircd_SAFE

#define trircd_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( trircd_AutokillDBName));	\
	nakill = i;					\
	break;						\
    }							\
} while (0)

void trircd_load_akill()
{
    trircd_dbFILE *f;
    int i, ver, nakill;
    int16 tmp16;
    int32 tmp32;
    trircd_Akill akill;

    if (!(f = trircd_open_db("AKILL", trircd_AutokillDBName, "r")))
	return;

    ver = trircd_get_file_version(f);

    trircd_read_int16(&tmp16, f);
    nakill = tmp16;

    switch (ver) {
      case 25:
      case 24:
      case 23:
      case 22:
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
	for (i = 0; i < nakill; i++) {
	    trircd_SAFE(trircd_read_string(&akill.mask, f));
	    trircd_SAFE(trircd_read_string(&akill.reason, f));
	    trircd_SAFE(trircd_read_buffer(akill.who, f));
	    trircd_SAFE(trircd_read_int32(&tmp32, f));
	    akill.time = tmp32;
	    trircd_SAFE(trircd_read_int32(&tmp32, f));
	    akill.expires = tmp32;

	    if (akill.expires == 0)
	    {
		Magick::instance().operserv.Akill_insert(mstring(akill.mask), Magick::instance().operserv.Expire_Sop(),
			mstring(akill.reason), mstring(akill.who), mDateTime(akill.time));
	    }
	    else
	    {
		Magick::instance().operserv.Akill_insert(mstring(akill.mask), akill.expires-akill.time,
			mstring(akill.reason), mstring(akill.who), mDateTime(akill.time));
	    }

	    free(akill.mask);
	    free(akill.reason);
	}
	break;

      case 4:
      case 3: {
	struct {
	    char *mask;
	    char *reason;
	    char who[trircd_NICKMAX];
	    time_t time;
	    time_t expires;
	    long reserved[4];
	} old_akill;

	for (i = 0; i < nakill; i++) {
	    trircd_SAFE(trircd_read_variable(old_akill, f));
	    strncpy(akill.who, old_akill.who, trircd_NICKMAX);
	    akill.time = old_akill.time;
	    akill.expires = old_akill.expires;
	}
	for (i = 0; i < nakill; i++) {
	    trircd_SAFE(trircd_read_string(&akill.mask, f));
	    trircd_SAFE(trircd_read_string(&akill.reason, f));

	    if (akill.expires == 0)
	    {
		Magick::instance().operserv.Akill_insert(mstring(akill.mask), Magick::instance().operserv.Expire_Sop(),
			mstring(akill.reason), mstring(akill.who), mDateTime(akill.time));
	    }
	    else
	    {
		Magick::instance().operserv.Akill_insert(mstring(akill.mask), akill.expires-akill.time,
			mstring(akill.reason), mstring(akill.who), mDateTime(akill.time));
	    }

	    free(akill.mask);
	    free(akill.reason);
	}
	break;
      } /* case 3/4 */

      case 2: {
	struct {
	    char *mask;
	    char *reason;
	    char who[trircd_NICKMAX];
	    time_t time;
	} old_akill;

	for (i = 0; i < nakill; i++) {
	    trircd_SAFE(trircd_read_variable(old_akill, f));
	    akill.time = old_akill.time;
	    strncpy(akill.who, old_akill.who, sizeof(akill.who));
	    akill.expires = 0;
	}
	for (i = 0; i < nakill; i++) {
	    trircd_SAFE(trircd_read_string(&akill.mask, f));
	    trircd_SAFE(trircd_read_string(&akill.reason, f));

	    Magick::instance().operserv.Akill_insert(mstring(akill.mask), Magick::instance().operserv.Expire_Sop(),
			mstring(akill.reason), mstring(akill.who), mDateTime(akill.time));

	    free(akill.mask);
	    free(akill.reason);
	}
	break;
      } /* case 2 */

      case 1: {
	struct {
	    char *mask;
	    char *reason;
	    time_t time;
	} old_akill;

	for (i = 0; i < nakill; i++) {
	    trircd_SAFE(trircd_read_variable(old_akill, f));
	    akill.time = old_akill.time;
	    akill.who[0] = 0;
	    akill.expires = 0;
	}
	for (i = 0; i < nakill; i++) {
	    trircd_SAFE(trircd_read_string(&akill.mask, f));
	    trircd_SAFE(trircd_read_string(&akill.reason, f));

	    Magick::instance().operserv.Akill_insert(mstring(akill.mask), Magick::instance().operserv.Expire_Sop(),
			mstring(akill.reason), Magick::instance().operserv.FirstName(), mDateTime(akill.time));

	    free(akill.mask);
	    free(akill.reason);
	}
	break;
      } /* case 1 */

      case -1:
	SLOG(LM_EMERGENCY, "Unable to read version number from %s",( trircd_AutokillDBName));

      default:
	SLOG(LM_EMERGENCY, "Unsupported version (%d) on %s",( ver, trircd_AutokillDBName));
    } /* switch (version) */

    trircd_close_db(f);
}

#undef trircd_SAFE

Nick_Stored_t *Convert::trircd_CreateNickEntry(trircd_NickInfo * ni)
{
    BTCB();
    int i;
    char **string;

    if (ni == NULL || ni->nick == NULL || !strlen(ni->nick))
	return NULL;

    if (ni->status & trircd_NS_VERBOTEN)
    {
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick);

	return out;
    }
    else if (ni->link && strlen(ni->link))
    {
	Nick_Stored_t tmp(mstring(ni->link));
	Nick_Stored_t *out = new Nick_Stored_t(mstring(ni->nick), mDateTime(ni->time_registered), tmp);

	if (out == NULL)
	    return NULL;
	out->i_LastSeenTime = mDateTime(ni->last_seen);
	return out;
    }
    else
    {
	Nick_Stored_t *out = new Nick_Stored_t(mstring(ni->nick), mstring(ni->pass));

	if (out == NULL)
	    return NULL;
	if (ni->email != NULL && strlen(ni->email))
	    out->i_Email = mstring(ni->email);
	if (ni->url != NULL && strlen(ni->url))
	    out->i_URL = mstring(ni->url);
	if (out->i_URL.Contains("http://"))
	    out->i_URL.Remove("http://", false);
	if (out->i_URL.Contains("HTTP://"))
	    out->i_URL.Remove("HTTP://", false);
	if (ni->last_realname != NULL && strlen(ni->last_realname))
	    out->i_LastRealName = mstring(ni->last_realname);
	if (ni->last_usermask != NULL && strlen(ni->last_usermask))
	    out->i_LastMask = mstring(ni->last_usermask);
	if (ni->last_quit != NULL && strlen(ni->last_quit))
	    out->i_LastQuit = mstring(ni->last_quit);
	out->i_RegTime = mDateTime(ni->time_registered);
	out->i_LastSeenTime = mDateTime(ni->last_seen);

	for (i = 0, string = ni->access; i < ni->accesscount; ++i, ++string)
	{
	    out->i_access.insert(mstring(*string));
	}

	switch (ni->language)
	{
	case trircd_LANG_EN_US:
	    out->setting.Language = "ENGLISH";
	    break;
	case trircd_LANG_TR:
	    out->setting.Language = "TURKISH";
	    break;
	case trircd_LANG_TR2:
	case trircd_LANG_JA_EUC:
	case trircd_LANG_JA_SJIS:
	    out->setting.Language = "JAPANESE";
	    break;
	case trircd_LANG_ES:
	    out->setting.Language = "SPANISH";
	    break;
	case trircd_LANG_PT:
	    out->setting.Language = "PORTUGUESE";
	    break;
	case trircd_LANG_FR:
	    out->setting.Language = "FRENCH";
	    break;
	case trircd_LANG_IT:
	    out->setting.Language = "ITALIAN";
	    break;
	case trircd_LANG_DE:
	    out->setting.Language = "GERMAN";
	    break;
	}

	if (ni->status & trircd_NS_NOEXPIRE && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if (ni->flags & trircd_NI_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (ni->flags & trircd_NI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ni->flags & trircd_NI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;

	if (ni->suspendinfo)
	{
	    if (ni->suspendinfo->reason)
		out->i_Comment = ni->suspendinfo->reason;
	    out->i_Suspend_By = ni->suspendinfo->who;
	    out->i_Suspend_Time = mDateTime(ni->suspendinfo->suspended);
	}

	return out;
    }

    ETCB();
}

void Convert::trircd_UpdateNickEntry(trircd_NickExt * ne, char *name)
{
    BTCB();
    int i;
    trircd_IgnoreInfo *ignore;

    if (!Magick::instance().nickserv.IsStored(name))
	return;

    map_entry<Nick_Stored_t> out = Magick::instance().nickserv.GetStored(name);

    for (i=0, ignore = ne->ignored; i<ne->ignorecount; i++, ignore++)
    {
	if (!ignore->in_use || !ignore->ign || !strlen(ignore->ign))
	    continue;

	out->IgnoreAdd(ignore->ign);
    }

    if (ne->infomsg && strlen(ne->infomsg))
	out->Description(ne->infomsg);

    ETCB();
}

Chan_Stored_t *Convert::Convert::trircd_CreateChanEntry(trircd_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name == NULL || !strlen(ci->name))
	return NULL;

    if (ci->flags & trircd_CI_VERBOTEN)
    {
	Chan_Stored_t *out = new Chan_Stored_t(mstring(ci->name));

	return out;
    }
    else
    {
	trircd_ChanAccess *i_access;
	trircd_AutoKick *akick;
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
	    mstring modes = trircd_getmodes(ci->mlock_on);

	    modes.Remove("k");
	    modes.Remove("l");
	    modelock << "+" << modes << (ci->mlock_key != NULL ? "k" : "") << (ci->mlock_limit ? "l" : "");
	}
	if (ci->mlock_off)
	{
	    modelock << "-" << trircd_getmodes(ci->mlock_off);
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
	float mod = (float) Magick::instance().chanserv.Level_Max() / (float) trircd_ACCLEV_FOUNDER;

	for (i = 0, i_access = ci->access; i < ci->accesscount; ++i, ++i_access)
	{
	    if (i_access->ni == NULL)
		continue;

	    if (i_access->level < 0)
		newlevel = -1;
	    else
		newlevel = (long) ((float) i_access->level * mod);
	    if (newlevel == 0)
		newlevel = 1;

	    out->Access_insert(i_access->ni, newlevel, Magick::instance().chanserv.FirstName());
	}

	for (i = 0, akick = ci->akick; i < ci->akickcount; ++i, ++akick)
	{
	    if (akick->is_nick ? akick->u.ni == NULL : akick->u.mask == NULL)
		continue;
	    if (akick->reason != NULL)
		out->Akick_insert(akick->is_nick ? akick->u.ni : akick->u.mask, akick->reason, mstring(akick->who));
	    else
		out->Akick_insert(akick->is_nick ? akick->u.ni : akick->u.mask, mstring(akick->who));
	}

	if (ci->flags & trircd_CI_KEEPTOPIC && !out->L_Keeptopic())
	    out->setting.Keeptopic = true;
	if (ci->flags & trircd_CI_SECUREOPS && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & trircd_CI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & trircd_CI_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & trircd_CI_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & trircd_CI_LEAVEOPS && !out->L_Anarchy())
	    out->setting.Anarchy = true;
	if (ci->flags & trircd_CI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & trircd_CI_NOEXPIRE && !Magick::instance().chanserv.LCK_NoExpire())
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
	    for (i = 0; i < trircd_CA_SIZE; ++i)
	    {
		if (ci->levels[i] == trircd_ACCLEV_INVALID)
		    newlevel = Magick::instance().chanserv.Level_Max() + 2;
		else if (ci->levels[i] == trircd_ACCLEV_FOUNDER)
		    newlevel = Magick::instance().chanserv.Level_Max() + 1;
		else if (ci->levels[i] < 0)
		    newlevel = -1;
		else
		    newlevel = (long) ((float) ci->levels[i] * mod);

		switch (i)
		{
		case trircd_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_AUTOHALFOP:
		    out->Level_change("AUTOHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_MEMO:
		    out->Level_change("WRITEMEMO", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_UNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_ACCESS_CHANGE:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_ACCESS_LIST:
		    out->Level_change("VIEW", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_INVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_OPDEOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_HALFOP:
		    out->Level_change("CMDHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case trircd_CA_CLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}
	return out;
    }
    ETCB();
}

void Convert::trircd_UpdateChanEntry(trircd_ChanExt * ce, char *name)
{
    BTCB();
    static_cast<void>(ce);
    static_cast<void>(name);
    ETCB();
}

MemoServ::nick_memo_t Convert::trircd_CreateMemoEntry(trircd_MemoInfo *mi, const char *name)
{
    BTCB();
    int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;

    for (i = 0; i < mi->memocount; i++)
    {
	if (mi->memos[i].sender || !strlen(mi->memos[i].sender) ||
	    mi->memos[i].text || !strlen(mi->memos[i].text))
	    continue;

	tmp = new Memo_t(name, mi->memos[i].sender, mi->memos[i].text);
	tmp->i_Time = mDateTime(mi->memos[i].time);
	if (!(mi->memos[i].flags & trircd_MF_UNREAD))
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}

MemoServ::channel_news_t Convert::trircd_CreateNewsEntry(trircd_MemoInfo *mi, const char *name)
{
    BTCB();
    int i;

    MemoServ::channel_news_t out;
    News_t *tmp;

    for (i = 0; i < mi->memocount; i++)
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
