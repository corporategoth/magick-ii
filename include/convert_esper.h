#ifndef WIN32
#pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _CONVERT_ESPER_H
#define _CONVERT_ESPER_H
#include "pch.h"
RCSID(convert_esper_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.4  2001/11/04 23:43:14  prez
** Updates for MS Visual C++ compilation (it works now!).
**
** Revision 1.3  2001/11/03 21:02:50  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.2  2001/06/15 07:20:39  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.1  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
**
** ========================================================== */

/* Database file descriptor structure and file handling routine prototypes.
 *
 * Services is copyright (c) 1996-1999 Andrew Church.
 *     E-mail: <achurch@dragonfire.net>
 * Services is copyright (c) 1999-2001 Andrew Kempe.
 *     E-mail: <theshadow@shadowfire.org>
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#ifdef CONVERT

/******* OperServ configuration *******/

typedef   signed short  int16;
typedef unsigned short uint16;
typedef   signed int  int32;
typedef unsigned int uint32;

/* Extra warning:  If you change these, your data files will be unusable! */

/* Maximum length of a channel name, including the trailing null.  Any
 * channels with a length longer than (CHANMAX-1) including the leading #
 * will not be usable with ChanServ. */
#define ESP_CHANMAX		64

/* Maximum length of a nickname, including the trailing null.  This MUST be
 * at least one greater than the maximum allowable nickname length on your
 * network, or people will run into problems using Services!  The default
 * (32) works with all servers I know of. */
#define ESP_NICKMAX		32

/* Maximum length of a password */
#define ESP_PASSMAX		32

#ifndef PATH_MAX
#define PATH_MAX 512
#endif

/*************************************************************************/

struct ESP_dbFILE {
    int mode;			/* 'r' for reading, 'w' for writing */
    FILE *fp;			/* The normal file descriptor */
    FILE *backupfp;		/* Open file pointer to a backup copy of
				 *    the database file (if non-NULL) */
    char filename[PATH_MAX];	/* Name of the database file */
    char backupname[PATH_MAX];	/* Name of the backup file */
};

/*************************************************************************/

/* Prototypes and macros: */

int ESP_get_file_version(ESP_dbFILE *f);
ESP_dbFILE *ESP_open_db(const char *service, const char *filename, const char *mode);
void ESP_close_db(ESP_dbFILE *f);
#define ESP_read_db(f,buf,len)	(fread((buf),1,(len),(f)->fp))
#define ESP_getc_db(f)		(fgetc((f)->fp))

int ESP_read_int16(int16 *ret, ESP_dbFILE *f);
int ESP_read_int32(int32 *ret, ESP_dbFILE *f);
int ESP_read_string(char **ret, ESP_dbFILE *f);

#define ESP_read_int8(ret,f)	((*(ret)=fgetc((f)->fp))==EOF ? -1 : 0)
#define ESP_read_buffer(buf,f)	((ESP_read_db((f),(buf),sizeof(buf)) != sizeof(buf)) ? -1 : 0)
#define ESP_read_variable(var,f)	((ESP_read_db((f),&(var),sizeof(var)) != sizeof(var)) ? -1 : 0)

/*************************************************************************/

/* Version number for data files; if structures below change, increment
 * this.  (Otherwise -very- bad things will happen!) */

#define ESP_FILE_VERSION	8

/*************************************************************************/

/* Memo info structures.  Since both nicknames and channels can have memos,
 * we encapsulate memo data in a MemoList to make it easier to handle. */

struct ESP_Memo {
    int32 number;	/* Index number -- not necessarily array position! */
    int16 flags;
    time_t time;	/* When it was sent */
    char sender[ESP_NICKMAX];
    char *text;
};

#define ESP_MF_UNREAD	0x0001	/* Memo has not yet been read */

struct ESP_MemoInfo {
    int16 memocount, memomax;
    ESP_Memo *memos;
};

/*************************************************************************/

/* Suspension info structures. */

struct ESP_SuspendInfo {
    char who[ESP_NICKMAX];	/* who added this suspension */
    char *reason;	
    time_t suspended;
    time_t expires;	/* 0 for no expiry */
};

/*************************************************************************/

/* Nickname info structure.  Each nick structure is stored in one of 256
 * lists; the list is determined by the first character of the nick.  Nicks
 * are stored in alphabetical order within lists. */


struct ESP_NickInfo {
    char nick[ESP_NICKMAX];
    char pass[ESP_PASSMAX];
    char *url;
    char *email;

    char *last_usermask;
    char *last_realname;
    char *last_quit;
    time_t time_registered;
    time_t last_seen;
    int16 status;	/* See NS_* below */

    char *link;		/* If non-NULL, nick to which this one is linked */
    int16 linkcount;	/* Number of links to this nick */

    /* All information from this point down is governed by links.  Note,
     * however, that channelcount is always saved, even for linked nicks
     * (thus removing the need to recount channels when unlinking a nick). */

    int32 flags;	/* See NI_* below */

    int16 accesscount;	/* # of entries */
    char **access;	/* Array of strings */

    ESP_MemoInfo memos;
    ESP_SuspendInfo susp;

    int16 channelcount;/* Number of channels currently registered */
    int16 channelmax;	/* Maximum number of channels allowed */

    int16 language;	/* Language selected by nickname owner (LANG_*) */

    time_t id_timestamp;/* TS8 timestamp of user who last ID'd for nick */
};


/* Nickname status flags: */
#define ESP_NS_ENCRYPTEDPW	0x0001      /* Nickname password is encrypted */
#define ESP_NS_VERBOTEN		0x0002      /* Nick may not be registered or used */
#define ESP_NS_NO_EXPIRE	0x0004      /* Nick never expires */

#define ESP_NS_IDENTIFIED	0x8000      /* User has IDENTIFY'd */
#define ESP_NS_RECOGNIZED	0x4000      /* ON_ACCESS true && SECURE flag not set */
#define ESP_NS_ON_ACCESS	0x2000      /* User comes from a known address */
#define ESP_NS_KILL_HELD	0x1000      /* Nick is being held after a kill */
#define ESP_NS_GUESTED		0x0100	    /* SVSNICK has been sent but nick has not
					     * yet changed. An enforcer will be
					     * introduced when it does change. */
#define ESP_NS_TEMPORARY	0xFF00      /* All temporary status flags */


/* Nickname setting flags: */
#define ESP_NI_KILLPROTECT	0x00000001  /* Kill others who take this nick */
#define ESP_NI_SECURE		0x00000002  /* Don't recognize unless IDENTIFY'd */
#define ESP_NI_MEMO_HARDMAX	0x00000008  /* Don't allow user to change memo limit */
#define ESP_NI_MEMO_SIGNON	0x00000010  /* Notify of memos at signon and un-away */
#define ESP_NI_MEMO_RECEIVE	0x00000020  /* Notify of new memos when sent */
#define ESP_NI_PRIVATE		0x00000040  /* Don't show in LIST to non-servadmins */
#define ESP_NI_HIDE_EMAIL	0x00000080  /* Don't show E-mail in INFO */
#define ESP_NI_HIDE_MASK	0x00000100  /* Don't show last seen address in INFO */
#define ESP_NI_HIDE_QUIT	0x00000200  /* Don't show last quit message in INFO */
#define ESP_NI_KILL_QUICK	0x00000400  /* Kill in 20 seconds instead of 60 */
#define ESP_NI_KILL_IMMED	0x00000800  /* Kill immediately instead of in 60 sec */

#define ESP_NI_SUSPENDED	0x10000000  /* Nick, and links, can not be used */

/* Languages.  Never insert anything in the middle of this list, or
 * everybody will start getting the wrong language!  If you want to change
 * the order the languages are displayed in for NickServ HELP SET LANGUAGE,
 * do it in language.c.
 */
#define ESP_LANG_EN_US		0	/* United States English */
#define ESP_LANG_JA_JIS		1	/* Japanese (JIS encoding) */
#define ESP_LANG_JA_EUC		2	/* Japanese (EUC encoding) */
#define ESP_LANG_JA_SJIS	3	/* Japanese (SJIS encoding) */
#define ESP_LANG_ES		4	/* Spanish */
#define ESP_LANG_PT		5	/* Portugese */
#define ESP_LANG_FR		6	/* French */
#define ESP_LANG_TR		7	/* Turkish */
#define ESP_LANG_IT		8	/* Italian */

#define ESP_NUM_LANGS		9	/* Number of languages */

/*************************************************************************/

/* Channel info structures.  Stored similarly to the nicks, except that
 * the second character of the channel name, not the first, is used to
 * determine the list.  (Hashing based on the first character of the name
 * wouldn't get very far. ;) ) */

/* Access levels for users. */
struct ESP_ChanAccess {
    int16 in_use;	/* 1 if this entry is in use, else 0 */
    int16 level;
    char *nick;	/* Guaranteed to be non-NULL if in use, NULL if not */
};

/* Note that these two levels also serve as exclusive boundaries for valid
 * access levels.  ACCESS_FOUNDER may be assumed to be strictly greater
 * than any valid access level, and ACCESS_INVALID may be assumed to be
 * strictly less than any valid access level.
 */
#define ESP_ACCESS_FOUNDER	10000	/* Numeric level indicating founder access */
#define ESP_ACCESS_INVALID	-10000	/* Used in levels[] for disabled settings */

/* AutoKick data. */
struct ESP_AutoKick {
    int16 in_use;
    int16 is_nick;	/* 1 if a regged nickname, 0 if a nick!user@host mask */
			/* Always 0 if not in use */
    union {
	char *mask;	/* Guaranteed to be non-NULL if in use, NULL if not */
	char *nick;	/* Same */
    } u;
    char *reason;
    char who[ESP_NICKMAX];
};

struct ESP_ChannelInfo {
    char name[ESP_CHANMAX];
    char *founder;
    char *successor;		/* Who gets the channel if the founder
					 * nick is dropped or expires */
    char founderpass[ESP_PASSMAX];
    char *desc;
    char *url;
    char *email;

    time_t time_registered;
    time_t last_used;
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[ESP_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;		/* When the last topic was set */

    int32 flags;			/* See below */

    int16 *levels;			/* Access levels for commands */

    int16 accesscount;
    ESP_ChanAccess *access;			/* List of authorized users */
    int16 akickcount;
    ESP_AutoKick *akick;			/* List of users to kickban */

    int16 mlock_on, mlock_off;		/* See channel modes below */
    int32 mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */

    char *entry_message;		/* Notice sent on entering channel */

    ESP_MemoInfo memos;
};

/* Retain topic even after last person leaves channel */
#define ESP_CI_KEEPTOPIC	0x00000001
/* Don't allow non-authorized users to be opped */
#define ESP_CI_SECUREOPS	0x00000002
/* Hide channel from ChanServ LIST command */
#define ESP_CI_PRIVATE		0x00000004
/* Topic can only be changed by SET TOPIC */
#define ESP_CI_TOPICLOCK	0x00000008
/* Those not allowed ops are kickbanned */
#define ESP_CI_RESTRICTED	0x00000010
/* Don't auto-deop anyone */
#define ESP_CI_LEAVEOPS		0x00000020
/* Don't allow any privileges unless a user is IDENTIFY'd with NickServ */
#define ESP_CI_SECURE		0x00000040
/* Don't allow the channel to be registered or used */
#define ESP_CI_VERBOTEN		0x00000080
/* Channel password is encrypted */
#define ESP_CI_ENCRYPTEDPW	0x00000100
/* Channel does not expire */
#define ESP_CI_NO_EXPIRE	0x00000200
/* Channel memo limit may not be changed */
#define ESP_CI_MEMO_HARDMAX	0x00000400
/* Send notice to channel on use of OP/DEOP */
#define ESP_CI_OPNOTICE		0x00000800

/* Indices for cmd_access[]: */
#define ESP_CA_INVITE		0
#define ESP_CA_AKICK		1
#define ESP_CA_SET		2	/* but not FOUNDER or PASSWORD */
#define ESP_CA_UNBAN		3
#define ESP_CA_AUTOOP		4
#define ESP_CA_AUTODEOP		5	/* Maximum, not minimum */
#define ESP_CA_AUTOVOICE	6
#define ESP_CA_OPDEOP		7	/* ChanServ commands OP and DEOP */
#define ESP_CA_ACCESS_LIST	8
#define ESP_CA_CLEAR		9
#define ESP_CA_NOJOIN		10	/* Maximum */
#define ESP_CA_ACCESS_CHANGE	11
#define ESP_CA_MEMO		12

#define ESP_CA_SIZE		13

#define ESP_CMODE_I 0x00000001
#define ESP_CMODE_M 0x00000002
#define ESP_CMODE_N 0x00000004
#define ESP_CMODE_P 0x00000008
#define ESP_CMODE_S 0x00000010
#define ESP_CMODE_T 0x00000020
#define ESP_CMODE_K 0x00000040		/* These two used only by ChanServ */
#define ESP_CMODE_L 0x00000080

/* IRC_DAL4_4_15 and IRC_BAHAMUT servers only */
#define ESP_CMODE_R 0x00000100		/* Only identified users can join */
#define ESP_CMODE_r 0x00000200		/* Set for all registered channels */

/* IRC_BAHAMUT servers only */
#define ESP_CMODE_C 0x00000400		/* No mIRC/ANSI colours in channel */


/*************************************************************************/

/* Constants for news types. */

struct ESP_NewsItem {
    int16 type;
    int32 num;		/* Numbering is separate for login and oper news */
    char *text;
    char who[ESP_NICKMAX];
    time_t time;
};

#define ESP_NEWS_LOGON	0
#define ESP_NEWS_OPER	1

/*************************************************************************/

struct ESP_Akill {
    char *mask;
    char *reason;
    char who[ESP_NICKMAX];
    time_t time;
    time_t expires;	/* or 0 for no expiry */
};

/*************************************************************************/

/* Ignorance list data. */

struct ESP_Exception {
    char *mask;			/* Hosts to which this exception applies */
    int limit;			/* Session limit for exception */
    char who[ESP_NICKMAX];		/* Nick of person who added the exception */
    char *reason;               /* Reason for exception's addition */
    time_t time;		/* When this exception was added */
    time_t expires;		/* Time when it expires. 0 == no expiry */
    int num;			/* Position in exception list; used to track
    				 * positions when deleting entries. It is 
				 * symbolic and used internally. It is 
				 * calculated at load time and never saved. */
};

/*************************************************************************/

void ESP_load_ns_dbase(void);
int ESP_delnick(ESP_NickInfo *ni);
void ESP_load_cs_dbase(void);
void reset_levels(ESP_ChannelInfo *ci);
int ESP_delchan(ESP_ChannelInfo *ci);
void ESP_load_old_ms_dbase(void);
void ESP_load_news(void);
void ESP_load_os_dbase(void);
void ESP_load_akill(void);
void ESP_load_exceptions(void);

/*************************************************************************/

class Nick_Stored_t;
class Chan_Stored_t;
class Memo_t;
class News_t;

/* OK -- now for the magick conversions ... */
Nick_Stored_t ESP_CreateNickEntry(ESP_NickInfo *ni);
Chan_Stored_t ESP_CreateChanEntry(ESP_ChannelInfo *ci);
list<Memo_t> ESP_CreateMemoEntry(ESP_MemoInfo *ml, char *nick);
list<News_t> ESP_CreateNewsEntry(ESP_MemoInfo *nl, char *chan);

#endif  /* CONVERT */
#endif	/* _CONVERT_ESPER_H */
