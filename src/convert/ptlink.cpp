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
#define RCSID(x,y) const char *rcsid_convert_ptlink_cpp_ ## x () { return y; }
RCSID(convert_ptlink_cpp, "@(#)$Id$");

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

/* Based upon the DB loading routines from PTlink Services 2.22.3
 * (c) 1999-2002 PTlink Coders Team <coders@ptlink.net>
 */

#include "magick.h"
#include "convert/interface.h"
#include "convert/ptlink.h"

/* What is the maximum number of Services admins we will allow? */
#define ptlink_MAX_SERVADMINS	16

/* What is the maximum number of Services operators we will allow? */
#define ptlink_MAX_SERVOPERS	64

/* Maximum length of a channel name, including the trailing null.  Any
 * channels with a length longer than (ptlink_CHANMAX-1) including the leading #
 * will not be usable with ChanServ. */
#define ptlink_CHANMAX		64

/* Maximum length of a nickname, including the trailing null.  This MUST be
 * at least one greater than the maximum allowable nickname length on your
 * network, or people will run into problems using Services!  The default
 * (32) works with all servers I know of. */
#define ptlink_NICKMAX		32

/* Maximum length of a password */
#define ptlink_PASSMAX		32

/* Maximum length of a auth code */
#define ptlink_AUTHMAX		16

// Some defaulted args ...
#define ptlink_MSMaxMemos	20
#define ptlink_CSMaxReg		20
#define ptlink_DefLanguage	1
#define ptlink_NSMaxNotes	5
#define ptlink_NSAllowKillImmed	0

#define ptlink_NickDBName	"nick.db"
#define ptlink_ChanDBName	"chan.db"
#define ptlink_OperDBName	"oper.db"
#define ptlink_AutokillDBName	"akill.db"
#define ptlink_NewsDBName	"news.db"

#define ptlink_FILE_VERSION	8
#define ptlink_DFV_NICKSERV	7
#define	ptlink_DFV_CHANSERV	7
#define	ptlink_DFV_OPERSERV	2
#define	ptlink_DFV_AKILL	1
#define	ptlink_DFV_NEWS		1

typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

/*************************************************************************/

/* Memo info structures.  Since both nicknames and channels can have memos,
 * we encapsulate memo data in a MemoList to make it easier to handle. */

struct ptlink_Memo {
    uint32 number;	/* Index number -- not necessarily array position! */
    int16 flags;
    time_t time;	/* When it was sent */
    char sender[ptlink_NICKMAX];
    char *text;
};

#define ptlink_MF_UNREAD	0x0001	/* ptlink_Memo has not yet been read */

struct ptlink_MemoInfo {
    int16 memocount, memomax;
    ptlink_Memo *memos;

};

struct ptlink_NoteInfo {
    int16 count, max;
    char **note;
};

/*************************************************************************/

/* Nickname info structure.  Each nick structure is stored in one of 256
 * lists; the list is determined by the first character of the nick.  Nicks
 * are stored in alphabetical order within lists. */


struct ptlink_NickInfo {
    ptlink_NickInfo *next, *prev;
    int id;                     /* for SQL */
    char nick[ptlink_NICKMAX];
    int	crypt_method;
    char pass[ptlink_PASSMAX];
    char auth[ptlink_AUTHMAX];
    char *url;
    char *email;		/* validated email */
    char *email_request;	/* register/change email */
    char *icq_number;
    char *location;
    char *last_usermask;
    char *last_realname;
    char *last_quit;
    time_t time_registered;
    time_t last_identify;
    time_t last_seen;
    time_t last_signon;
    time_t first_TS;	/* for svs* TS */
    time_t birth_date;
    time_t suspension_expire;
    time_t last_email_request;
    int32 online;	/* total online time */
    int16 status;	/* See NS_* below */
    int32 news_mask;	/* news subscription bit mask, see NM_ below (on NewsServ) */
    int16 news_status;	/* news status bit mask, see NW_ below below (on NewsServ)*/
    char *link;	/* If non-NULL, nick to which this one is linked */
    int16 linkcount;	/* Number of links to this nick */

    /* All information from this point down is governed by links.  Note,
     * however, that channelcount is always saved, even for linked nicks
     * (thus removing the need to recount channels when unlinking a nick). */

    int32 flags;	/* See NI_* below */
    
    int16 ajoincount;  /* # of entries */
    char **autojoin;	/* Array of strings */
    ptlink_MemoInfo memos;
    ptlink_NoteInfo notes;
    uint16 channelcount;/* Number of channels currently registered */
    uint16 channelmax;	/* Maximum number of channels allowed */

    uint16 language;	/* Language selected by nickname owner (LANG_*) */

    int forcednicks;    /* forced nicks change since last identify/NickServ kill */
};

/* Nickname status flags: */
#define ptlink_NS_ENCRYPTEDPW	0x0001      /* Nickname password is encrypted */
#define ptlink_NS_VERBOTEN	0x0002      /* Nick may not be registered or used */
#define ptlink_NS_NO_EXPIRE	0x0004      /* Nick never expires */
#define ptlink_NS_AUTH		0x0008	    /* Nick registration with valid AUTH code */

#define ptlink_NS_IDENTIFIED	0x8000      /* User has IDENTIFY'd */
#define ptlink_NS_RECOGNIZED	0x4000      /* ON_ACCESS true && SECURE flag not set */
#define ptlink_NS_ON_ACCESS	0x2000      /* User comes from a known address */
#define ptlink_NS_KILL_HELD	0x1000      /* Nick is being held after a kill */
#define ptlink_NS_TEMPORARY	0xF000      /* All temporary status flags */


/* Nickname setting flags: */
#define ptlink_NI_KILLPROTECT	0x00000001  /* Kill others who take this nick */
#define ptlink_NI_SUSPENDED	0x00000002  /* old ptlink_SAFE bit (STILL SET ON OLD DBs) */
#define ptlink_NI_MEMO_HARDMAX	0x00000008  /* Don't allow user to change memo limit */
#define ptlink_NI_MEMO_SIGNON	0x00000010  /* Notify of memos at signon and un-away */
#define ptlink_NI_MEMO_RECEIVE	0x00000020  /* Notify of new memos when sent */
#define ptlink_NI_PRIVATE	0x00000040  /* Don't show in LIST to non-servadmins */
#define ptlink_NI_HIDE_EMAIL	0x00000080  /* Don't show E-mail in INFO */
#define ptlink_NI_HIDE_MASK	0x00000100  /* Don't show last seen address in INFO */
#define ptlink_NI_HIDE_QUIT	0x00000200  /* Don't show last quit message in INFO */
#define ptlink_NI_KILL_QUICK	0x00000400  /* Kill in 20 seconds instead of 60 */
#define ptlink_NI_KILL_IMMED	0x00000800  /* Kill immediately instead of in 60 sec */
#define ptlink_NI_AUTO_JOIN   	0x00001000  /* Make users autojoin on identify */
#define ptlink_NI_DROPPED	0x00002000  /* This nick is drop pending */
#define ptlink_NI_OSUSPENDED	0x00004000  /* Oper suspension */ 

/* Languages.  Never insert anything in the middle of this list, or
 * everybody will start getting the wrong language!  If you want to change
 * the order the languages are displayed in for NickServ HELP SET LANGUAGE,
 * do it in language.c.
 */
#define ptlink_LANG_EN_US	0	/* United States English */
#define ptlink_LANG_PT		1	/* Portugese */
#define ptlink_LANG_TR		2	/* Turkish */
#define ptlink_LANG_DE		3	/* German */
#define ptlink_LANG_IT		4	/* Italian */

#define ptlink_NUM_LANGS	5	/* Number of languages */

/* NewsMask */
/* the only representative bit in newsmask is 
 bit 0, values: 0 = receive news via privmsg, 1 = receive news via notice */
#define ptlink_NM_NOTICE	0x00000001
#define ptlink_NM_ALL		0xFFFFFFFE

/* Flags */
#define ptlink_NW_WELCOME	0x0001 /* needing welcome service msg */
#define ptlink_NW_NONEWS	0x0002

/*************************************************************************/

/* Channel info structures.  Stored similarly to the nicks, except that
 * the second character of the channel name, not the first, is used to
 * determine the list.  (Hashing based on the first character of the name
 * wouldn't get very far. ;) ) */

/* Access levels for users. */
struct ptlink_ChanAccess {
    int16 in_use;	/* 1 if this entry is in use, else 0 */
    int16 level;
    char *ni;	/* Guaranteed to be non-NULL if in use, NULL if not */
    char *who;	/* who last added/changed this access entry */
};

/* Note that these two levels also serve as exclusive boundaries for valid
 * access levels.  ptlink_ACCESS_FOUNDER may be assumed to be strictly greater
 * than any valid access level, and ptlink_ACCESS_INVALID may be assumed to be
 * strictly less than any valid access level.
 */
#define ptlink_ACCESS_FOUNDER	10000	/* Numeric level indicating founder access */
#define ptlink_ACCESS_INVALID	-10000	/* Used in levels[] for disabled settings */

/* ptlink_AutoKick data. */
struct ptlink_AutoKick {
    int16 in_use;
    int16 is_nick;	/* 1 if a regged nickname, 0 if a nick!user@host mask */
			/* Always 0 if not in use */
    char *mask;	/* Guaranteed to be non-NULL if in use, NULL if not */
    char *reason;
    char *who;	/* who added the akick */
    time_t last_kick;	/* last time this akick was triggered */
};

struct ptlink_ChanInfo {
    ptlink_ChanInfo *next, *prev;
    char name[ptlink_CHANMAX];    
    char *founder;
    char *successor;		/* Who gets the channel if the founder
					 * nick is dropped or expires */
    char founderpass[ptlink_PASSMAX];
    char *desc;
    char *url;
    char *email;

    time_t time_registered;
    time_t last_used;
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[ptlink_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;		/* When the last topic was set */

    int32 flags;			/* See below */
    int16  crypt_method;		/* encryption method */    
    time_t drop_time;			/* Time when drop command was issued
					   (only applicable if CI_DROPPED */
    int16 *levels;			/* Access levels for commands */

    int16 accesscount;
    ptlink_ChanAccess *access;			/* List of authorized users */
    int16 akickcount;
    ptlink_AutoKick *akick;			/* List of users to kickban */

    int16 mlock_on, mlock_off;		/* See channel modes below */
    int32 mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */

    char *entry_message;		/* Notice sent on entering channel */

    ptlink_MemoInfo memos;

    void *c;			/* Pointer to channel record (if   *
					 *    channel is currently in use) */
    time_t maxtime;
    int16 maxusers;					 
};

/* Retain topic even after last person leaves channel */
#define ptlink_CI_KEEPTOPIC	0x00000001
/* Don't allow non-authorized users to be opped */
#define ptlink_CI_SECUREOPS	0x00000002
/* Hide channel from ChanServ LIST command */
#define ptlink_CI_PRIVATE	0x00000004
/* Topic can only be changed by SET TOPIC */
#define ptlink_CI_TOPICLOCK	0x00000008
/* Those not allowed ops are kickbanned */
#define ptlink_CI_RESTRICTED	0x00000010
/* Don't auto-deop anyone */
#define ptlink_CI_LEAVEOPS	0x00000020
/* Don't allow the channel to be registered or used */
#define ptlink_CI_VERBOTEN	0x00000080
/* Channel password is encrypted */
#define ptlink_CI_ENCRYPTEDPW	0x00000100 /* obsolete by crypt_method */
/* Channel does not expire */
#define ptlink_CI_NO_EXPIRE	0x00000200
/* Channel memo limit may not be changed */
#define ptlink_CI_MEMO_HARDMAX	0x00000400
/* Send notice to channel on use of OP/DEOP */
#define ptlink_CI_OPNOTICE	0x00000800
/* Channel was dropped */
#define ptlink_CI_DROPPED	0x00001000
/* Restrict access level recognition to real nick (ignoring links) */
#define ptlink_CI_NOLINKS	0x00002000 

/* Indices for cmd_access[]: */
#define ptlink_CA_INVITE	0
#define ptlink_CA_AKICK	1
#define ptlink_CA_SET		2	/* but not FOUNDER or PASSWORD */
#define ptlink_CA_UNBAN	3
#define ptlink_CA_AUTOOP	4
#define ptlink_CA_AUTODEOP	5	/* Maximum, not minimum */
#define ptlink_CA_AUTOVOICE	6
#define ptlink_CA_OPDEOP	7	/* ChanServ commands OP and DEOP */
#define ptlink_CA_ACCESS_LIST	8
#define ptlink_CA_CLEAR	9
#define ptlink_CA_NOJOIN	10	/* Maximum */
#define ptlink_CA_ACCESS_CHANGE 11
#define ptlink_CA_MEMOREAD	12
#define ptlink_CA_PROTECT	13
#define ptlink_CA_KICK		14
#define ptlink_CA_MEMOSEND	15
#define ptlink_CA_MEMODEL	16
#define ptlink_CA_SIZE		17

#define ptlink_CMODE_I  0x00000001
#define ptlink_CMODE_M  0x00000002
#define ptlink_CMODE_N  0x00000004
#define ptlink_CMODE_P  0x00000008
#define ptlink_CMODE_s  0x00000010
#define ptlink_CMODE_T  0x00000020
#define ptlink_CMODE_k  0x00000040		/* These two used only by ChanServ */
#define ptlink_CMODE_l  0x00000080
#define ptlink_CMODE_q	 0x00000100
#define ptlink_CMODE_R  0x00000200
#define ptlink_CMODE_c  0x00000400
#define ptlink_CMODE_K  0x00000800
#define ptlink_CMODE_O	 0x00001000
#define ptlink_CMODE_A	 0x00002000
#define ptlink_CMODE_S	 0x00004000
#define ptlink_CMODE_d	 0x00008000

int ptlink_def_levels[][2] = {
    { ptlink_CA_AUTOOP,             5 },
    { ptlink_CA_AUTOVOICE,          3 },
    { ptlink_CA_AUTODEOP,          -1 },
    { ptlink_CA_NOJOIN,            -1 },
    { ptlink_CA_INVITE,             5 },
    { ptlink_CA_AKICK,             10 },
    { ptlink_CA_SET,   ptlink_ACCESS_INVALID },
    { ptlink_CA_CLEAR, ptlink_ACCESS_INVALID },
    { ptlink_CA_UNBAN,              5 },
    { ptlink_CA_OPDEOP,             5 },
    { ptlink_CA_ACCESS_LIST,        0 },
    { ptlink_CA_ACCESS_CHANGE,      1 },
    { ptlink_CA_MEMOREAD,           5 },
    { ptlink_CA_PROTECT, ptlink_ACCESS_INVALID },
    { ptlink_CA_KICK,		    10 },            
    { ptlink_CA_MEMOSEND,	    10 },
    { ptlink_CA_MEMODEL,	    10 },        
    { -1 }
};

/* Constants for news types. */

struct ptlink_NewsItem {
    int16 type;
    int32 num;		/* Numbering is separate for login and oper news */
    char *text;
    char who[ptlink_NICKMAX];
    time_t time;
};

#define ptlink_NEWS_LOGON	0
#define ptlink_NEWS_OPER	1

struct ptlink_Akill {
    char *mask;
    char *reason;
    char who[ptlink_NICKMAX];
    time_t time;
    time_t expires;	/* or 0 for no expiry */
};

struct ptlink_dbFILE {
    int mode;			/* 'r' for reading, 'w' for writing */
    FILE *fp;			/* The normal file descriptor */
    FILE *backupfp;		/* Open file pointer to a backup copy of
				 *    the database file (if non-NULL) */
    char filename[PATH_MAX];	/* Name of the database file */
    char backupname[PATH_MAX];	/* Name of the backup file */
};

int ptlink_get_file_version(ptlink_dbFILE *f)
{
    FILE *fp = f->fp;
    int version;
    int fver = fgetc(fp);
    if(fver==255) { version=-1; }
	else
	    version = fver << 24 | fgetc(fp)<<16 | fgetc(fp)<<8 | fgetc(fp);
    if (ferror(fp)) {
#ifndef NOT_MAIN
	SLOG(LM_ERROR, "Error reading version number on %s", (f->filename));
#endif
	return 0;
    } else if (feof(fp)) {
#ifndef NOT_MAIN
	SLOG(LM_ERROR, "Error reading version number on %s: End of file detected",(
		f->filename));
#endif
	return 0;
    } else if (version > ptlink_FILE_VERSION || version < -1) {
#ifndef NOT_MAIN
	SLOG(LM_ERROR, "Invalid version number (%d) on %s",( version, f->filename));
#endif
	return 0;
    }
    return version;
}

#define ptlink_read_db(f,buf,len)	(fread((buf),1,(len),(f)->fp))
#define ptlink_getc_db(f)		(fgetc((f)->fp))
#define ptlink_read_int8(ret,f)	((*(ret)=fgetc((f)->fp))==EOF ? -1 : 0)
#define ptlink_read_buffer(buf,f)	(ptlink_read_db((f),(buf),sizeof(buf)) == sizeof(buf))
#define ptlink_read_buflen(buf,len,f)	(ptlink_read_db((f),(buf),(len)) == (len))
#define ptlink_read_variable(var,f)	(ptlink_read_db((f),&(var),sizeof(var)) == sizeof(var))

ptlink_dbFILE *ptlink_open_db_read(const char *service, const char *filename)
{
    ptlink_dbFILE *f;
    FILE *fp;

    f = (ptlink_dbFILE *) malloc(sizeof(*f));
    if (!f) {
#ifndef NOT_MAIN
	SLOG(LM_ERROR, "Can't read %s database %s", (service, filename));
#endif
	return NULL;
    }
    strncpy(f->filename, filename, sizeof(f->filename));
    f->mode = 'r';
    fp = fopen(f->filename, "rb");
    if (!fp) {
	int errno_save = errno;
#ifndef NOT_MAIN
	if (errno != ENOENT)
	    SLOG(LM_ERROR, "Can't read %s database %s", (service, f->filename));
#endif
	free(f);
	errno = errno_save;
	return NULL;
    }
    f->fp = fp;
    f->backupfp = NULL;
    return f;
}

ptlink_dbFILE *ptlink_open_db(const char *service, const char *filename, const char *mode)
{
    if (*mode == 'r') {
	return ptlink_open_db_read(service, filename);
    } else {
	errno = EINVAL;
	return NULL;
    }
}

void ptlink_close_db(ptlink_dbFILE *f)
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

int ptlink_read_int16(uint16 *ret, ptlink_dbFILE *f)
{
    int c1, c2;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    if (c1 == EOF || c2 == EOF)
	return -1;
    *ret = c1<<8 | c2;
    return 0;
}

int ptlink_read_int16(int16 *ret, ptlink_dbFILE *f)
{
    int c1, c2;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    if (c1 == EOF || c2 == EOF)
	return -1;
    *ret = c1<<8 | c2;
    return 0;
}

int ptlink_read_int32(uint32 *ret, ptlink_dbFILE *f)
{
    int c1, c2, c3, c4;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    c3 = fgetc(f->fp);
    c4 = fgetc(f->fp);
    if (c1 == EOF || c2 == EOF || c3 == EOF || c4 == EOF)
	return -1;
    *ret = c1<<24 | c2<<16 | c3<<8 | c4;
    return 0;
}

int ptlink_read_int32(int32 *ret, ptlink_dbFILE *f)
{
    int c1, c2, c3, c4;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    c3 = fgetc(f->fp);
    c4 = fgetc(f->fp);
    if (c1 == EOF || c2 == EOF || c3 == EOF || c4 == EOF)
	return -1;
    *ret = c1<<24 | c2<<16 | c3<<8 | c4;
    return 0;
}

int ptlink_read_ptr(void **ret, ptlink_dbFILE *f)
{
    int c;

    c = fgetc(f->fp);
    if (c == EOF)
	return -1;
    *ret = (c ? (void *)1 : (void *)0);
    return 0;
}

int ptlink_read_string_buffer(char *ret, ptlink_dbFILE *f)
{
    uint16 len;

    if (ptlink_read_int16(&len, f) < 0)
	return -1;
    if (len == 0) {
	ret[0] = '\0';
	return 0;
    }
    if (len != fread(ret, 1, len, f->fp)) {
	return -1;
    }
    return 0;
}

int ptlink_read_string(char **ret, ptlink_dbFILE *f)
{
    char *s;
    uint16 len;

    if (ptlink_read_int16(&len, f) < 0)
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

#define ptlink_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( ptlink_NickDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

int ptlink_delnick(ptlink_NickInfo *ni);
int ptlink_delchan(ptlink_ChanInfo *ci);
void ptlink_reset_levels(ptlink_ChanInfo *ci);
mstring ptlink_getmodes(short modes);

void ptlink_load_nick()
{
    ptlink_dbFILE *f;
    int ver, i, j, c;
    int16 sver = 0;
    int16 tmp16;
    int32 total;
    ptlink_NickInfo *ni;
    int failed = 0;
    if (!(f = ptlink_open_db("NickServ", ptlink_NickDBName, "r")))
	return;
	
    switch (ver = ptlink_get_file_version(f)) {
      case -1: ptlink_SAFE(ptlink_read_int16(&sver, f));
	    if(sver<1 || sver>ptlink_DFV_NICKSERV) 
		SLOG(LM_EMERGENCY, "Unsupported subversion (%d) on %s",( sver, ptlink_NickDBName));
	    if(sver<ptlink_DFV_NICKSERV) 
		SLOG(LM_ERROR, "NickServ DataBase WARNING: converting from subversion %d to %d",(
		    sver, ptlink_DFV_NICKSERV));	       
      case 7:
      case 6:
      case 5:
	if(sver>3) {
	    ptlink_SAFE(ptlink_read_int32(&total, f));
	}
	for (i = 0; i < 256 && !failed; i++) {
	    int32 tmp32;
	    while ((c = ptlink_getc_db(f)) == 1) {
		ni = (ptlink_NickInfo *) calloc(sizeof(ptlink_NickInfo), 1);
		ni->forcednicks = 0;
		ni->last_signon = 0;
		ptlink_SAFE(ptlink_read_buffer(ni->nick, f));
		ptlink_SAFE(ptlink_read_buffer(ni->pass, f));
		if(sver>6)
		  ptlink_SAFE(ptlink_read_buffer(ni->auth, f));
		else
		 ni->auth[0]='\0';		 
		ptlink_SAFE(ptlink_read_string(&ni->url, f));
		ptlink_SAFE(ptlink_read_string(&ni->email_request, f));
		if(sver>6)
		  ptlink_SAFE(ptlink_read_string(&ni->email, f));
		else
		  ni->email = NULL;
		if(sver>5) {
		    ptlink_SAFE(ptlink_read_string(&ni->icq_number, f));
		    ptlink_SAFE(ptlink_read_string(&ni->location, f));
		}
		else {
		    ni->icq_number=NULL;
		    ni->location=NULL;
		}
		ptlink_SAFE(ptlink_read_string(&ni->last_usermask, f));
		if (!ni->last_usermask)
		    ni->last_usermask = strdup("@");
		ptlink_SAFE(ptlink_read_string(&ni->last_realname, f));
		if (!ni->last_realname)
		    ni->last_realname = strdup("");
		ptlink_SAFE(ptlink_read_string(&ni->last_quit, f));
		ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		ni->time_registered = tmp32;
		ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		ni->last_identify = tmp32;
		if(sver>1) {
		    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		    ni->last_seen = tmp32;
		} else ni->last_seen = ni->last_identify;
		if(sver>6)
		  {
		    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		    ni->last_email_request = tmp32;
		  }
		if(sver>5) {
		    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		    ni->birth_date = tmp32;
		} else ni->birth_date = 0;
		ptlink_SAFE(ptlink_read_int16(&ni->status, f));
		ni->status &= ~ptlink_NS_TEMPORARY;
		if(sver>4) {
		    ptlink_SAFE(ptlink_read_int16(&tmp16, f));
		    ni->crypt_method = tmp16;
		} else
		    ni->crypt_method = (ni->status & ptlink_NS_ENCRYPTEDPW) ? 1 : 0;

#ifdef GETPASS
		if (ni->crypt_method)
		{
		    SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted "
			"but encryption disabled, aborting",
			("NickServ", ni->nick));
		}
#else
#if !defined(JP2CRYPT)
		if (ni->crypt_method == 1)
		{
		    SLOG(LM_EMERGENCY, "$1: load database: password for $2 uses the "
			"JP2 encryption scheme, but magick is not compiled with JP2 "
			"as our password encryption method of choice.  Re-compile with "
			"the --enable-jp2-crypt option to fix.", ("NickServ", ni->nick));
		}
#endif
#if !defined(DESCRYPT)
		if (ni->crypt_method == 2)
		{
		    SLOG(LM_EMERGENCY, "$1: load database: password for $2 uses the "
			"DES crypt() encryption scheme, but magick is not compiled with "
			"DES crypt() as our password encryption method of choice.  Re-compile "
			"with the --enable-des-crypt option to fix.", ("NickServ", ni->nick));
		}
#endif
#endif

		if(sver<3) {
		    ni->news_mask = ptlink_NM_ALL;
		    ni->news_status = ptlink_NW_WELCOME;
		} else {
		    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		    ni->news_mask = tmp32;
		    ptlink_SAFE(ptlink_read_int16(&ni->news_status, f));
		}		
		if (!ni->crypt_method) {
		    SLOG(LM_EMERGENCY, "%s: Can't encrypt `%s' nickname password!",(
				"NickServ", ni->nick));
		}

		/* Store the _name_ of the link target in ni->link for now;
		 * we'll resolve it after we've loaded all the nicks */
		ptlink_SAFE(ptlink_read_string((char **)&ni->link, f));
		ptlink_SAFE(ptlink_read_int16(&ni->linkcount, f));
		if (ni->link) {
		    ptlink_SAFE(ptlink_read_int16(&ni->channelcount, f));
		    /* No other information saved for linked nicks, since
		     * they get it all from their link target */
		    ni->flags = 0;
		    ni->ajoincount = 0;
		    ni->autojoin = NULL;		    
		    ni->memos.memocount = 0;
		    ni->memos.memomax = ptlink_MSMaxMemos;
		    ni->memos.memos = NULL;
		    ni->notes.count = 0;
		    ni->notes.max = 0;
		    ni->notes.note = NULL;
		    ni->channelmax = ptlink_CSMaxReg;
		    ni->language = ptlink_DefLanguage;
		    ni->last_signon = 0;
		} else {
		    ptlink_SAFE(ptlink_read_int32(&ni->flags, f));
		    if(sver<3) {
			ni->flags &= ~ptlink_NI_DROPPED;
		    }		    
		    if(sver>2) {
			ptlink_SAFE(ptlink_read_int32(&tmp32, f));
			if(sver>5)
			    ni->online = tmp32;		    
			else
			    ni->online = 0;
		    } else ni->online = 0;
		    if(sver>5 && (ni->flags & (ptlink_NI_SUSPENDED | ptlink_NI_OSUSPENDED))) {
			ptlink_SAFE(ptlink_read_int32(&tmp32, f));
			ni->suspension_expire = tmp32;
		    }		    
		    if (!ptlink_NSAllowKillImmed)
			ni->flags &= ~ptlink_NI_KILL_IMMED;
		/* We don't use access list anymore, just discard it */
		    if(sver<3) {
			int16 count=0;
			ptlink_SAFE(ptlink_read_int16(&count, f));
			if (count) {
			    char *tmp;
			    for (j = 0; j < count; j++) {
				ptlink_SAFE(ptlink_read_string(&tmp, f));
				free(tmp);
			    }
			}
		    }
		    if(!sver) {
			    ni->flags |= ptlink_NI_AUTO_JOIN;
			    ni->ajoincount = 0;
			    ni->autojoin = NULL;
		    } else {
			ptlink_SAFE(ptlink_read_int16(&ni->ajoincount, f));
			if (ni->ajoincount) {
			    char **autojoin;
			    autojoin = (char **) malloc(sizeof(char *) * ni->ajoincount);
			    ni->autojoin = autojoin;
			    for (j = 0; j < ni->ajoincount; j++, autojoin++) 
				ptlink_SAFE(ptlink_read_string(autojoin, f));
			}
		    }
		    ptlink_SAFE(ptlink_read_int16(&ni->memos.memocount, f));
		    ptlink_SAFE(ptlink_read_int16(&ni->memos.memomax, f));
		    if (ni->memos.memocount) {
			ptlink_Memo *memos;
			memos = (ptlink_Memo *) malloc(sizeof(ptlink_Memo) * ni->memos.memocount);
			ni->memos.memos = memos;
			for (j = 0; j < ni->memos.memocount; j++, memos++) {
			    ptlink_SAFE(ptlink_read_int32(&memos->number, f));
			    ptlink_SAFE(ptlink_read_int16(&memos->flags, f));
			    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
			    memos->time = tmp32;
			    ptlink_SAFE(ptlink_read_buffer(memos->sender, f));
			    ptlink_SAFE(ptlink_read_string(&memos->text, f));
			}
		    }
		    ni->notes.count = 0;
		    ni->notes.max = ptlink_NSMaxNotes;		    
		    if(sver>3) {
			ptlink_SAFE(ptlink_read_int16(&ni->notes.count, f));
			ptlink_SAFE(ptlink_read_int16(&ni->notes.max, f));			
		    }
		    if(ptlink_NSMaxNotes>ni->notes.max)
		        ni->notes.max = ptlink_NSMaxNotes;
		    if(ni->notes.max)
			  ni->notes.note= (char **) malloc(sizeof(char*) * ni->notes.max);
			else 
		    	  ni->notes.note = NULL;
		    for (j = 0; j < ni->notes.count; j++) {
			ptlink_SAFE(ptlink_read_string(&ni->notes.note[j], f));
		    }
		    ptlink_SAFE(ptlink_read_int16(&ni->channelcount, f));
		    ptlink_SAFE(ptlink_read_int16(&ni->channelmax, f));		    
		    if (ver == 5) {
			/* Fields not initialized properly for new nicks */
			/* These will be updated by ptlink_load_cs_dbase() */
			ni->channelcount = 0;
			ni->channelmax = ptlink_CSMaxReg;
		    }
		    ptlink_SAFE(ptlink_read_int16(&ni->language, f));
		    /* this code is only needed for compatibility */
		    if(sver>3) {
			if (ni->language==5) ni->language=ptlink_LANG_PT;
			    else if(ni->language) ni->language = ptlink_DefLanguage;
		    }
		}

		Nick_Stored_t *nick = Convert::ptlink_CreateNickEntry(ni);
		if (nick != NULL)
		{
		    Magick::instance().nickserv.AddStored(nick);
		    if (ni->memos.memocount)
		    {
			MemoServ::nick_memo_t memo = Convert::ptlink_CreateMemoEntry(&ni->memos, ni->nick);
			if (memo.size())
			    Magick::instance().memoserv.AddNick(memo);
		    }
		}
		ptlink_delnick(ni);

	    } /* while (ptlink_getc_db(f) != 0) */
	} /* for (i) */

	break;
      default:
	SLOG(LM_EMERGENCY, "Unsupported version number (%d) on %s",( ver, ptlink_NickDBName));

    } /* switch (version) */
    ptlink_close_db(f);
}

#undef ptlink_SAFE

int ptlink_delnick(ptlink_NickInfo *ni)
{
    int i;

    if (ni->link)
	free(ni->link);
    if (ni->last_usermask)
	free(ni->last_usermask);
    if (ni->last_realname)
	free(ni->last_realname);
    if (ni->autojoin) {
	for (i = 0; i < ni->ajoincount; i++) {
	    if (ni->autojoin[i])
		free(ni->autojoin[i]);
	}
	free(ni->autojoin);
    }    
    if (ni->memos.memos) {
	for (i = 0; i < ni->memos.memocount; i++) {
	    if (ni->memos.memos[i].text)
		free(ni->memos.memos[i].text);
	}
	free(ni->memos.memos);
    }
    if (ni->notes.count) {
	for (i = 0; i < ni->notes.count; i++) 
	    if(ni->notes.note[i])
		free(ni->notes.note[i]);
    }
    free(ni->notes.note);
    free(ni);
    return 1;
}

#define ptlink_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( ptlink_ChanDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void ptlink_load_chan()
{
    ptlink_dbFILE *f;   
    int ver, i, j, c;
    int16 sver=0;
    ptlink_ChanInfo *ci;
    int failed = 0;
    if (!(f = ptlink_open_db("ChanServ", ptlink_ChanDBName, "r")))
	return;
    
    switch (ver = ptlink_get_file_version(f)) {
      case -1:  ptlink_read_int16(&sver, f);		    
      	  if(sver>ptlink_DFV_CHANSERV) SLOG(LM_EMERGENCY, "Unsupported subversion (%d) on %s",( sver, ptlink_ChanDBName));
	  if(sver<ptlink_DFV_CHANSERV) SLOG(LM_ERROR, "ChanServ DataBase WARNING: converting from subversion %d to %d",(
	      sver,ptlink_DFV_CHANSERV));
      case 7:
      case 6:
      case 5:

	for (i = 0; i < 256 && !failed; i++) {
	    int16 tmp16;
	    int32 tmp32;
	    int n_levels;
	    char *s;

	    while ((c = ptlink_getc_db(f)) != 0) {
		if (c != 1)
		    SLOG(LM_EMERGENCY, "Invalid format in %s",( ptlink_ChanDBName));
		ci = (ptlink_ChanInfo *) calloc(sizeof(ptlink_ChanInfo), 1);
		ptlink_SAFE(ptlink_read_buffer(ci->name, f));
		ptlink_SAFE(ptlink_read_string(&ci->founder, f));
		if (ver == 7 || sver) {
		    ptlink_SAFE(ptlink_read_string(&ci->successor, f));
		} else {
		    ci->successor = NULL;
		}
		if(sver>2)
		{
		    ptlink_SAFE(ptlink_read_int16(&tmp16, f));
		    ci->maxusers = tmp16;
		    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		    ci->maxtime = tmp32;
		} else
		{
		    ci->maxusers = 0;
		    ci->maxtime = time(NULL);
		}
		if (!(ci->founder) && ci->successor) {
			ci->founder = ci->successor;
			ci->successor = NULL;
		}
		ptlink_SAFE(ptlink_read_buffer(ci->founderpass, f));
		ptlink_SAFE(ptlink_read_string(&ci->desc, f));
		if (!ci->desc)
		    ci->desc = strdup("");
		ptlink_SAFE(ptlink_read_string(&ci->url, f));
		ptlink_SAFE(ptlink_read_string(&ci->email, f));
		ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		ci->time_registered = tmp32;
		ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		ci->last_used = tmp32;
		ptlink_SAFE(ptlink_read_string(&ci->last_topic, f));
		ptlink_SAFE(ptlink_read_buffer(ci->last_topic_setter, f));
		ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		ci->last_topic_time = tmp32;
		ptlink_SAFE(ptlink_read_int32(&ci->flags, f));		
		if(sver>5) {
		    ptlink_SAFE(ptlink_read_int16(&tmp16, f));
		    ci->crypt_method = tmp16;		
		} else
		    ci->crypt_method = (ci->flags & ptlink_CI_ENCRYPTEDPW) ? 1 : 0;

#ifdef GETPASS
		if (ci->crypt_method)
		{
		    SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted "
			"but encryption disabled, aborting",
			("ChanServ", ci->name));
		}
#else
#if !defined(JP2CRYPT)
		if (ci->crypt_method == 1)
		{
		    SLOG(LM_EMERGENCY, "$1: load database: password for $2 uses the "
			"JP2 encryption scheme, but magick is not compiled with JP2 "
			"as our password encryption method of choice.  Re-compile with "
			"the --enable-jp2-crypt option to fix.", ("ChanServ", ci->name));
		}
#endif
#if !defined(DESCRYPT)
		if (ci->crypt_method == 2)
		{
		    SLOG(LM_EMERGENCY, "$1: load database: password for $2 uses the "
			"DES crypt() encryption scheme, but magick is not compiled with "
			"DES crypt() as our password encryption method of choice.  Re-compile "
			"with the --enable-des-crypt option to fix.", ("ChanServ", ci->name));
		}
#endif
#endif

		if (ci->flags & ptlink_CI_DROPPED)    {
		    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
		    ci->drop_time = tmp32;		
		}
		ptlink_SAFE(ptlink_read_int16(&tmp16, f));
		n_levels = tmp16;
		ci->levels = (short *) malloc(2*ptlink_CA_SIZE);
		ptlink_reset_levels(ci);
		for (j = 0; j < n_levels; j++) {
		    if (j < ptlink_CA_SIZE)
			ptlink_SAFE(ptlink_read_int16(&ci->levels[j], f));
		    else
			ptlink_SAFE(ptlink_read_int16(&tmp16, f));
		}
		
		ptlink_SAFE(ptlink_read_int16(&ci->accesscount, f));
		if (ci->accesscount) {
		    ci->access = (ptlink_ChanAccess *) calloc(ci->accesscount, sizeof(ptlink_ChanAccess));
		    for (j = 0; j < ci->accesscount; j++) {
			ptlink_SAFE(ptlink_read_int16(&ci->access[j].in_use, f));
			if (ci->access[j].in_use) {
			    ptlink_SAFE(ptlink_read_int16(&ci->access[j].level, f));
			    ptlink_SAFE(ptlink_read_string(&ci->access[j].ni, f));
			    if(sver>1) {
				ptlink_SAFE(ptlink_read_string(&ci->access[j].who, f));			    
			    } else ci->access[j].who = NULL;		    
			    if(!ci->access[j].ni) 
			      ci->access[j].in_use = 0; /* access list bug killed :P */
			}			
		    }
		} else {
		    ci->access = NULL;
		}
		ptlink_SAFE(ptlink_read_int16(&ci->akickcount, f));
		if (ci->akickcount) {
		    ci->akick = (ptlink_AutoKick *) calloc(ci->akickcount, sizeof(ptlink_AutoKick));
		    for (j = 0; j < ci->akickcount; j++) {
			ptlink_SAFE(ptlink_read_int16(&ci->akick[j].in_use, f));
			if (ci->akick[j].in_use) {			   	
			    if(sver < 7) {
			    	int16 is_nick;
			        ptlink_SAFE(ptlink_read_int16(&is_nick, f));
			        ptlink_SAFE(ptlink_read_string(&ci->akick[j].mask, f));
			    } else {
			    	ptlink_SAFE(ptlink_read_string(&s, f));
			    	ci->akick[j].mask = s;
			    }
			    ptlink_SAFE(ptlink_read_string(&ci->akick[j].reason, f));
			    if(sver > 4) {
				ptlink_SAFE(ptlink_read_string(&ci->akick[j].who, f));			    
			    } else ci->akick[j].who = NULL;		    
			    if(sver>5) {
				ptlink_SAFE(ptlink_read_int32(&tmp32, f));
				ci->akick[j].last_kick = tmp32;
			    } else ci->akick[j].last_kick = time(NULL);
			}
		    }
		} else {
		    ci->akick = NULL;
		}

		ptlink_SAFE(ptlink_read_int16(&ci->mlock_on, f));
		ptlink_SAFE(ptlink_read_int16(&ci->mlock_off, f));
		ptlink_SAFE(ptlink_read_int32(&ci->mlock_limit, f));
		ptlink_SAFE(ptlink_read_string(&ci->mlock_key, f));

		ptlink_SAFE(ptlink_read_int16(&ci->memos.memocount, f));
		ptlink_SAFE(ptlink_read_int16(&ci->memos.memomax, f));
		if (ci->memos.memocount) {
		    ptlink_Memo *memos;
		    memos = (ptlink_Memo *) malloc(sizeof(ptlink_Memo) * ci->memos.memocount);
		    ci->memos.memos = memos;
		    for (j = 0; j < ci->memos.memocount; j++, memos++) {
			ptlink_SAFE(ptlink_read_int32(&memos->number, f));
			ptlink_SAFE(ptlink_read_int16(&memos->flags, f));
			ptlink_SAFE(ptlink_read_int32(&tmp32, f));
			memos->time = tmp32;
			ptlink_SAFE(ptlink_read_buffer(memos->sender, f));
			ptlink_SAFE(ptlink_read_string(&memos->text, f));
		    }
		}

		ptlink_SAFE(ptlink_read_string(&ci->entry_message, f));

		ci->c = NULL;

		Chan_Stored_t *chan = Convert::ptlink_CreateChanEntry(ci);
		if (chan != NULL)
		{
		    Magick::instance().chanserv.AddStored(chan);
		    if (ci->memos.memocount)
		    {
			MemoServ::channel_news_t news = Convert::ptlink_CreateNewsEntry(&ci->memos, ci->name);
			if (news.size())
			    Magick::instance().memoserv.AddChannel(news);
		    }
		}
		ptlink_delchan(ci);
	    } /* while (ptlink_getc_db(f) != 0) */
	} /* for (i) */

	break; /* case 5 and up */

      default:
	SLOG(LM_EMERGENCY, "Unsupported version number (%d) on %s",( ver, ptlink_ChanDBName));

    } /* switch (version) */

    ptlink_close_db(f);
}

#undef ptlink_SAFE

int ptlink_delchan(ptlink_ChanInfo *ci)
{
    int i;

    if (ci->founder)
	free(ci->founder);
    if (ci->successor)
	free(ci->successor);
    if (ci->desc)
	free(ci->desc);
    if (ci->mlock_key)
	free(ci->mlock_key);
    if (ci->last_topic)
	free(ci->last_topic);
    for (i = 0; i < ci->accesscount; i++) {
	if (ci->access[i].ni)
	    free(ci->access[i].ni);
	if (ci->access[i].who)
	    free(ci->access[i].who);
    }
    if (ci->access)
	free(ci->access);
    for (i = 0; i < ci->akickcount; i++) {
	if (ci->akick[i].who)
	    free(ci->akick[i].who);
	if (ci->akick[i].mask)
	    free(ci->akick[i].mask);
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

/*************************************************************************/

/* Reset channel access level values to their default state. */

void ptlink_reset_levels(ptlink_ChanInfo *ci)
{
    int i;

    if (ci->levels)
	free(ci->levels);
    ci->levels = (short *) malloc(ptlink_CA_SIZE * sizeof(*ci->levels));
    for (i = 0; ptlink_def_levels[i][0] >= 0; i++)
	ci->levels[ptlink_def_levels[i][0]] = ptlink_def_levels[i][1];
}

mstring ptlink_getmodes(short modes)
{
    BTCB();
    mstring retval;

    if (modes & ptlink_CMODE_I)
	retval += "i";
    if (modes & ptlink_CMODE_M)
	retval += "m";
    if (modes & ptlink_CMODE_N)
	retval += "n";
    if (modes & ptlink_CMODE_P)
	retval += "p";
    if (modes & ptlink_CMODE_s)
	retval += "s";
    if (modes & ptlink_CMODE_T)
	retval += "t";
    if (modes & ptlink_CMODE_k)
	retval += "k";
    if (modes & ptlink_CMODE_l)
	retval += "l";
    if (modes & ptlink_CMODE_q)
	retval += "q";
    if (modes & ptlink_CMODE_R)
	retval += "R";
    if (modes & ptlink_CMODE_c)
	retval += "c";
    if (modes & ptlink_CMODE_K)
	retval += "K";
    if (modes & ptlink_CMODE_O)
	retval += "O";
    if (modes & ptlink_CMODE_A)
	retval += "A";
    if (modes & ptlink_CMODE_S)
	retval += "S";
    if (modes & ptlink_CMODE_d)
	retval += "d";

    return retval;
    ETCB();
}

#define ptlink_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( ptlink_NewsDBName));	\
	break;						\
    }							\
} while (0)

void ptlink_load_news()
{
    ptlink_dbFILE *f;
    int i;
    int16 n;
    int16 sver;
    int32 tmp32;
    ptlink_NewsItem news;

    if (!(f = ptlink_open_db("OperServ", ptlink_NewsDBName, "r")))
	return;
    switch (i = ptlink_get_file_version(f)) {
      case -1:
          ptlink_read_int16(&sver, f);
      case 8:
      case 7:
	ptlink_SAFE(ptlink_read_int16(&n, f));
	if (!n) {
	    ptlink_close_db(f);
	    return;
	}
	for (i = 0; i < n; i++) {
	    ptlink_SAFE(ptlink_read_int16(&news.type, f));
	    ptlink_SAFE(ptlink_read_int32(&news.num, f));
	    ptlink_SAFE(ptlink_read_string(&news.text, f));
	    ptlink_SAFE(ptlink_read_buffer(news.who, f));
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
	    news.time = tmp32;

	    if (news.type == ptlink_NEWS_LOGON && Magick::instance().commserv.IsList(Magick::instance().commserv.ALL_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.ALL_Name())->
		    MSG_insert(mstring(news.text), mstring(news.who), mDateTime(news.time));
	    }
	    else if (news.type == ptlink_NEWS_OPER && Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->
		    MSG_insert(mstring(news.text), mstring(news.who), mDateTime(news.time));
	    }

	    free(news.text);
	}
	break;

      default:
	SLOG(LM_EMERGENCY, "Unsupported version (%d) on %s",( i, ptlink_NewsDBName));
    } /* switch (ver) */

    ptlink_close_db(f);
}

#undef ptlink_SAFE

#define ptlink_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( ptlink_AutokillDBName));	\
	nakill = i;					\
	break;						\
    }							\
} while (0)

void ptlink_load_akill()
{
    ptlink_dbFILE *f;
    int i, ver, nakill;
    int16 tmp16;
    int32 tmp32;
    int16 sver;
    ptlink_Akill akill;
    if (!(f = ptlink_open_db("AKILL", ptlink_AutokillDBName, "r")))
	return;

    ver = ptlink_get_file_version(f);
    if(ver==-1) ptlink_read_int16(&sver, f);		    
    ptlink_read_int16(&tmp16, f);
    nakill = tmp16;

    switch (ver) {
      case -1: 
	  if(sver!=1) SLOG(LM_EMERGENCY, "Unsupported subversion (%d) on %s",( sver, ptlink_AutokillDBName));
      case 8:
      case 7:
      case 6:
      case 5:
	for (i = 0; i < nakill; i++) {
	    ptlink_SAFE(ptlink_read_string(&akill.mask, f));
	    ptlink_SAFE(ptlink_read_string(&akill.reason, f));
	    ptlink_SAFE(ptlink_read_buffer(akill.who, f));
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
	    akill.time = tmp32;
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
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
	    char who[ptlink_NICKMAX];
	    time_t time;
	    time_t expires;
	    long reserved[4];
	} old_akill;

	for (i = 0; i < nakill; i++) {
	    ptlink_SAFE(ptlink_read_variable(old_akill, f));
	    strncpy(akill.who, old_akill.who, ptlink_NICKMAX);
	    akill.time = old_akill.time;
	    akill.expires = old_akill.expires;
	}
	for (i = 0; i < nakill; i++) {
	    ptlink_SAFE(ptlink_read_string(&akill.mask, f));
	    ptlink_SAFE(ptlink_read_string(&akill.reason, f));

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
	    char who[ptlink_NICKMAX];
	    time_t time;
	} old_akill;

	for (i = 0; i < nakill; i++) {
	    ptlink_SAFE(ptlink_read_variable(old_akill, f));
	    akill.time = old_akill.time;
	    strncpy(akill.who, old_akill.who, sizeof(akill.who));
	    akill.expires = 0;
	}
	for (i = 0; i < nakill; i++) {
	    ptlink_SAFE(ptlink_read_string(&akill.mask, f));
	    ptlink_SAFE(ptlink_read_string(&akill.reason, f));

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
	    ptlink_SAFE(ptlink_read_variable(old_akill, f));
	    akill.time = old_akill.time;
	    akill.who[0] = 0;
	    akill.expires = 0;
	}
	for (i = 0; i < nakill; i++) {
	    ptlink_SAFE(ptlink_read_string(&akill.mask, f));
	    ptlink_SAFE(ptlink_read_string(&akill.reason, f));

	    Magick::instance().operserv.Akill_insert(mstring(akill.mask), Magick::instance().operserv.Expire_Sop(),
			mstring(akill.reason), Magick::instance().operserv.FirstName(), mDateTime(akill.time));

	    free(akill.mask);
	    free(akill.reason);
	}
	break;
      } /* case 1 */

      default:
	SLOG(LM_EMERGENCY, "Unsupported version (%d) on %s",( ver, ptlink_AutokillDBName));
    } /* switch (version) */

    ptlink_close_db(f);
}

#undef ptlink_SAFE

#define ptlink_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( ptlink_OperDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void ptlink_load_oper()
{
    ptlink_dbFILE *f;
    int16  i, n, ver, sver=0;
    char *s;
    int failed = 0;
    if (!(f = ptlink_open_db("OperServ", ptlink_OperDBName, "r")))
	return;
    switch (ver = ptlink_get_file_version(f)) {
      case -1:  ptlink_read_int16(&sver, f);		    
		if(sver<1 || sver>ptlink_DFV_OPERSERV ) 
		    SLOG(LM_EMERGENCY, "Unsupported subversion (%d) on %s",( sver, ptlink_OperDBName));
		if(sver<ptlink_DFV_OPERSERV) 
		    SLOG(LM_ERROR, "OperServ DataBase WARNING, reading from subversion %d to %d",(
		    sver,ptlink_DFV_OPERSERV));	       
      case 7:
      case 6:
      case 5:
	ptlink_SAFE(ptlink_read_int16(&n, f));
	for (i = 0; i < n && !failed; i++) {
	    ptlink_SAFE(ptlink_read_string(&s, f));
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());
	    free(s);
	}
	if (!failed)
	    ptlink_SAFE(ptlink_read_int16(&n, f));
	for (i = 0; i < n && !failed; i++) {
	    ptlink_SAFE(ptlink_read_string(&s, f));
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());
	    free(s);
	}
	if ((ver==7) || sver) {
	    int32 tmp32;
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
	}
	if (sver>1) {
	    int32 tmp32;
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
	    ptlink_SAFE(ptlink_read_int32(&tmp32, f));
	}
	break;

      case 4:
      case 3:
	ptlink_SAFE(ptlink_read_int16(&n, f));
	for (i = 0; i < n && !failed; i++) {
	    ptlink_SAFE(ptlink_read_string(&s, f));
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());
	    free(s);
	}
	break;

      default:
	SLOG(LM_EMERGENCY, "Unsupported version (%d) on %s",( ver, ptlink_OperDBName));
    } /* switch (version) */
    ptlink_close_db(f);
}

#undef ptlink_SAFE

Nick_Stored_t *Convert::ptlink_CreateNickEntry(ptlink_NickInfo * ni)
{
    BTCB();
    if (ni == NULL || ni->nick == NULL || !strlen(ni->nick))
	return NULL;

    if (ni->status & ptlink_NS_VERBOTEN)
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
	switch (ni->crypt_method)
	{
	case 1:
	case 2:
	    out->i_Password = ni->pass;
	    break;
	case 3:
	    {
		char pwbuf[33] = {0};
		for (int i=0; i<16; i++)
		    sprintf(&pwbuf[i*2], "%02x", ni->pass[i]);
		out->i_Password = pwbuf;
	    }
	    break;
	}
	if (ni->email != NULL && strlen(ni->email))
	    out->i_Email = mstring(ni->email);
	if (ni->url != NULL && strlen(ni->url))
	    out->i_URL = mstring(ni->url);
	if (out->i_URL.Contains("http://"))
	    out->i_URL.Remove("http://", false);
	if (out->i_URL.Contains("HTTP://"))
	    out->i_URL.Remove("HTTP://", false);
	if (ni->icq_number != NULL && strlen(ni->icq_number))
	    out->i_ICQ = mstring(ni->icq_number);
	if (ni->last_realname != NULL && strlen(ni->last_realname))
	    out->i_LastRealName = mstring(ni->last_realname);
	if (ni->last_usermask != NULL && strlen(ni->last_usermask))
	    out->i_LastMask = mstring(ni->last_usermask);
	if (ni->last_quit != NULL && strlen(ni->last_quit))
	    out->i_LastQuit = mstring(ni->last_quit);
	out->i_RegTime = mDateTime(ni->time_registered);
	out->i_LastSeenTime = mDateTime(ni->last_seen);

	switch (ni->language)
	{
	case ptlink_LANG_EN_US:
	    out->setting.Language = "ENGLISH";
	    break;
	case ptlink_LANG_PT:
	    out->setting.Language = "PORTUGUESE";
	    break;
	case ptlink_LANG_TR:
	    out->setting.Language = "TURKISH";
	    break;
	case ptlink_LANG_DE:
	    out->setting.Language = "GERMAN";
	    break;
	case ptlink_LANG_IT:
	    out->setting.Language = "ITALIAN";
	    break;
	}

	if (ni->flags & ptlink_NI_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (ni->flags & ptlink_NI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ni->status & ptlink_NS_NO_EXPIRE && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;

	if (ni->flags & ptlink_NI_SUSPENDED)
	{
	    out->i_Suspend_By = Magick::instance().nickserv.FirstName();
	    out->i_Suspend_Time = mDateTime::CurrentDateTime();
	}

	return out;
    }

    ETCB();
}

Chan_Stored_t *Convert::Convert::ptlink_CreateChanEntry(ptlink_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name == NULL || !strlen(ci->name))
	return NULL;

    if (ci->flags & ptlink_CI_VERBOTEN)
    {
	Chan_Stored_t *out = new Chan_Stored_t(mstring(ci->name));

	return out;
    }
    else
    {
	ptlink_ChanAccess *i_access;
	ptlink_AutoKick *akick;
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

	switch (ci->crypt_method)
	{
	case 1:
	case 2:
	    out->i_Password = ci->founderpass;
	    break;
	case 3:
	    {
		char pwbuf[33] = {0};
		for (i=0; i<16; i++)
		    sprintf(&pwbuf[i*2], "%02x", ci->founderpass[i]);
		out->i_Password = pwbuf;
	    }
	    break;
	}
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
	    mstring modes = ptlink_getmodes(ci->mlock_on);

	    modes.Remove("k");
	    modes.Remove("l");
	    modelock << "+" << modes << (ci->mlock_key != NULL ? "k" : "") << (ci->mlock_limit ? "l" : "");
	}
	if (ci->mlock_off)
	{
	    modelock << "-" << ptlink_getmodes(ci->mlock_off);
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
	float mod = (float) Magick::instance().chanserv.Level_Max() / (float) ptlink_ACCESS_FOUNDER;

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

	    out->Access_insert(i_access->ni, newlevel, mstring(i_access->who));
	}

	for (i = 0, akick = ci->akick; i < ci->akickcount; ++i, ++akick)
	{
	    if (akick->mask == NULL)
		continue;
	    if (akick->reason != NULL)
		out->Akick_insert(akick->mask, akick->reason, mstring(akick->who));
	    else
		out->Akick_insert(akick->mask, mstring(akick->who));
	}

	if (ci->flags & ptlink_CI_KEEPTOPIC && !out->L_Keeptopic())
	    out->setting.Keeptopic = true;
	if (ci->flags & ptlink_CI_SECUREOPS && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & ptlink_CI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & ptlink_CI_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & ptlink_CI_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & ptlink_CI_LEAVEOPS && !out->L_Anarchy())
	    out->setting.Anarchy = true;
	if (ci->flags & ptlink_CI_NO_EXPIRE && !Magick::instance().chanserv.LCK_NoExpire())
	    out->setting.NoExpire = true;

	if (ci->levels != NULL)
	{
	    for (i = 0; i < ptlink_CA_SIZE; ++i)
	    {
		if (ci->levels[i] == ptlink_ACCESS_INVALID)
		    newlevel = Magick::instance().chanserv.Level_Max() + 2;
		else if (ci->levels[i] == ptlink_ACCESS_FOUNDER)
		    newlevel = Magick::instance().chanserv.Level_Max() + 1;
		else if (ci->levels[i] < 0)
		    newlevel = -1;
		else
		    newlevel = (long) ((float) ci->levels[i] * mod);

		switch (i)
		{
		case ptlink_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_MEMOREAD:
		    out->Level_change("READMEMO", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_MEMOSEND:
		    out->Level_change("WRITEMEMO", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_MEMODEL:
		    out->Level_change("DELMEMO", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_UNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_ACCESS_CHANGE:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_ACCESS_LIST:
		    out->Level_change("VIEW", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_INVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_OPDEOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_CLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ptlink_CA_KICK:
		    out->Level_change("CMDKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}
	return out;
    }
    ETCB();
}

MemoServ::nick_memo_t Convert::ptlink_CreateMemoEntry(ptlink_MemoInfo *mi, const char *name)
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
	if (!(mi->memos[i].flags & ptlink_MF_UNREAD))
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}

MemoServ::channel_news_t Convert::ptlink_CreateNewsEntry(ptlink_MemoInfo *mi, const char *name)
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
