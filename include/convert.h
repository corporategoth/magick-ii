#ifndef WIN32
#pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _CONVERT_H
#define _CONVERT_H
#include "pch.h"
static const char *ident_convert_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.5  2000/09/30 10:48:06  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.4  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.3  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
**
** ========================================================== */


#include "nickserv.h"
#include "chanserv.h"
#include "memoserv.h"

/* struct		ver
 * ------------------------
 * nickinfo_v1		1-2
 * nickinfo_v3		3
 * nickinfo_		4-
 * chanaccess_		1-
 * autokick_		1-
 * chaninfo_v1		1-2
 * chaninfo_v3		3-4
 * chaninfo_		5-
 * memo_		1-
 * memolist_		1-
 * newslist_		1-
 * message_		1-
 * akill_v1		1
 * akill_		2-
 * allow_		1-
 */
 
/****************************************************************************
 * nick.db
 ****************************************************************************/

extern const int file_version;
extern const char *nickserv_db;
extern const char *chanserv_db;
extern const char *memoserv_db;
extern const char *newsserv_db;
extern const char *sop_db;
extern const char *message_db;
extern const char *akill_db;
extern const char *clone_db;

/* Maximum length of a channel name */
#define	CHANMAX		64

/* Maximum length of a nickname */
#define	NICKMAX		32

/* Maximum length of a password */
#define	PASSMAX		32

/* Maximum number of valid +/- Modes */
#define MODEMAX		64

/* Maximum LastMsg value */
#define LASTMSGMAX	256

/* Nickname info structure.  Each nick structure is stored in one of 256
 * lists; the list is determined by the first character of the nick.  Nicks
 * are stored in alphabetical order within lists. */

#define	NI_KILLPROTECT	0x00000001  /* Kill others who take this nick */
#define	NI_SECURE	0x00000002  /* Don't recognize unless IDENTIFY'd */
#define	NI_VERBOTEN	0x00000004  /* Nick may not be registered or used */
#define	NI_IRCOP	0x00000008  /* IrcOP - Nick will not expire */
#define	NI_PRIVATE	0x00000010  /* Private - Dont show up in list */
#define	NI_SUSPENDED	0x00000020  /* Suspended - May not IDENTIFY */
#define	NI_PRIVMSG	0x00000040  /* use PRIVMSG instead of NOTICE */
#define NI_SLAVE	0x00000080  /* Nick is just a 'linked' nick */

#define	NI_IDENTIFIED	0x80000000  /* User has IDENTIFY'd */
#define	NI_RECOGNIZED	0x40000000  /* User comes from a known addy */
#define	NI_KILL_HELD	0x20000000  /* Nick is being held after a kill */

/*************************************************************************/

/* Channel info structures.  Stored similarly to the nicks, except that
 * the second character of the channel name, not the first, is used to
 * determine the list.  (Hashing based on the first character of the name
 * wouldn't get very far. ;) ) */

/* Retain topic even after last person leaves channel */
#define	CI_KEEPTOPIC	0x00000001
/* Don't allow non-authorized users to be opped */
#define	CI_SECUREOPS	0x00000002
/* Hide channel from ChanServ LIST command */
#define	CI_PRIVATE	0x00000004
/* Topic can only be changed by SET TOPIC */
#define	CI_TOPICLOCK	0x00000008
/* Those not allowed ops are kickbanned */
#define	CI_RESTRICTED	0x00000010
/* Don't auto-deop anyone */
#define	CI_LEAVEOPS	0x00000020
/* Don't allow any privileges unless a user is IDENTIFY'd with NickServ */
#define	CI_SECURE	0x00000040
/* Don't allow the channel to be registered or used */
#define	CI_VERBOTEN	0x00000080
/* Dont honour channel access list or founder */
#define	CI_SUSPENDED	0x00000100
/* ChanServ joins channel when its established */
#define	CI_JOIN		0x00000200
/* Revenge flags */
#define	CI_REV1		0x80000000
#define	CI_REV2		0x40000000
#define	CI_REV3		0x20000000

/* Why ChanServ PARTED or JOINED a channel */
#define CJ_SET		0
#define CJ_NOUSERS	1
#define CJ_KICKED	2
#define CJ_KILLED	3

/* Revenge levels */
#define	CR_NONE		0
#define	CR_REVERSE	1
#define	CR_DEOP		2
#define	CR_KICK		3
#define	CR_NICKBAN	4
#define	CR_USERBAN	5
#define	CR_HOSTBAN	6
#define	CR_MIRROR	7

/* Indices for cmd_access[]: */
#define	CA_AUTODEOP	0
#define	CA_AUTOVOICE	1
#define	CA_AUTOOP	2
#define	CA_READMEMO	3
#define	CA_WRITEMEMO	4
#define	CA_DELMEMO	5
#define	CA_AKICK	6
#define	CA_STARAKICK	7
#define	CA_UNBAN	8
#define	CA_ACCESS	9
#define	CA_SET		10	/* NOT FOUNDER and PASSWORD */

#define	CA_CMDINVITE	11
#define	CA_CMDUNBAN	12
#define	CA_CMDVOICE	13
#define	CA_CMDOP	14
#define	CA_CMDCLEAR	15

#define	CA_FLOOR	16
#define	CA_CAP		17
#define	CA_FOUNDER	18

#define	CA_SIZE		19	/* <--- DO NOT DELETE */

/* OVERRIDE_LEVEL levels (Based on level 2) */
#define	CO_OPER		0
#define CO_SOP		1
#define CO_ADMIN	2

/*************************************************************************/

/* MemoServ data.  Only nicks that actually have memos get records in
 * MemoServ's lists, which are stored the same way NickServ's are. */

#define	MS_READ		0x00000001	/* Yes, I had planned to do this */
#define MS_DELETED	0x00000002

#define	M_LOGON	0
#define	M_OPER	1

struct NickInfo_V1 {
    NickInfo_V1 *next, *prev;
    char nick[NICKMAX];
    char pass[PASSMAX];
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;	/* # of entries */
    char **access;	/* Array of strings */
    long flags;		/* See below */
    long reserved[4];	/* For future expansion -- set to 0 */
};

struct NickInfo_V3 {
    NickInfo_V3 *next, *prev;
    char nick[NICKMAX];
    char pass[PASSMAX];
    char *email;
    char *url;
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;	/* # of entries */
    char **access;	/* Array of strings */
    long ignorecount;   /* # of entries */
    char **ignore;	/* Array of strings */
    long flags;		/* See below */
    long reserved[4];	/* For future expansion -- set to 0 */
};

struct NickInfo {
    NickInfo *next, *prev;
    char nick[NICKMAX];
    char pass[PASSMAX];
    char *email;
    char *url;
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;	/* # of entries */
    char **access;	/* Array of strings */
    long ignorecount;   /* # of entries */
    char **ignore;	/* Array of strings */
    long flags;		/* See below */
    long reserved[4];	/* For future expansion -- set to 0 */
};

/****************************************************************************
 * chan.db
 ****************************************************************************/

struct ChanAccess {
    short level;
    short is_nick;	/* 1 if this is a nick, 0 if a user@host mask.  If
			 * -1, then this entry is currently unused (a hack
			 * to get numbered lists to have consistent
			 * numbering). */
    char *name;
};

struct AutoKick {
    short is_nick;
    short pad;
    char *name;
    char *reason;
};

struct ChanInfo_V1 {
    ChanInfo_V1 *next, *prev;
    char name[CHANMAX];
    char founder[NICKMAX];		/* Always a reg'd nick */
    char founderpass[PASSMAX];
    char *desc;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    ChanAccess *access;			/* List of authorized users */
    long akickcount;
    AutoKick *akick;
    short mlock_on, mlock_off;		/* See channel modes below */
    long mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;		/* When the last topic was set */
    long flags;				/* See below */
    short *cmd_access;			/* Access levels for commands */
    long reserved[3];			/* For future expansion -- set to 0 */
};

struct ChanInfo_V3 {
    ChanInfo_V3 *next, *prev;
    char name[CHANMAX];
    char founder[NICKMAX];		/* Always a reg'd nick */
    char founderpass[PASSMAX];
    char *desc;
    char *url;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    ChanAccess *access;			/* List of authorized users */
    long akickcount;
    AutoKick *akick;
    short mlock_on, mlock_off;		/* See channel modes below */
    long mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;		/* When the last topic was set */
    long flags;				/* See below */
    short *cmd_access;			/* Access levels for commands */
    long reserved[3];			/* For future expansion -- set to 0 */
};

struct ChanInfo {
    ChanInfo *next, *prev;
    char name[CHANMAX];
    char founder[NICKMAX];		/* Always a reg'd nick */
    char founderpass[PASSMAX];
    char *desc;
    char *url;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    ChanAccess *access;			/* List of authorized users */
    long akickcount;
    AutoKick *akick;
    char mlock_on[64], mlock_off[64];	/* See channel modes below */
    long mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;		/* When the last topic was set */
    long flags;				/* See below */
    short *cmd_access;			/* Access levels for commands */
    long reserved[3];			/* For future expansion -- set to 0 */
};

/****************************************************************************
 * memo.db and news.db
 ****************************************************************************/

struct Memo {
    char sender[NICKMAX];
    long number;	/* Index number -- not necessarily array position! */
    time_t time;	/* When it was sent */
    char *text;
    long reserved[4];	/* For future expansion -- set to 0 */
};

struct MemoList {
    MemoList *next, *prev;
    char nick[NICKMAX];	/* Owner of the memos */
    long n_memos;	/* Number of memos */
    Memo *memos;	/* The memos themselves */
    long reserved[4];	/* For future expansion -- set to 0 */
};

struct NewsList {
    NewsList *next, *prev;
    char chan[CHANMAX];	/* Owner of the memos */
    long n_newss;	/* Number of memos */
    Memo *newss;	/* The memos themselves */
    long reserved[4];	/* For future expansion -- set to 0 */
};

/****************************************************************************
 * others
 ****************************************************************************/

struct Message {
    char *text;
    short type;
    char who[NICKMAX];
    time_t time;
};

struct Akill_V1 {
    char *mask;
    char *reason;
    time_t time;
};

struct Akill {
    char *mask;
    char *reason;
    char who[NICKMAX];
    time_t time;
};

struct Allow {
    char *host;
    int amount;
    char *reason;
    char who[NICKMAX];
    time_t time;
};

struct Sop {
    char nick[NICKMAX];
};

class Nick_Stored_t;
class Chan_Stored_t;
class Memo_t;
class News_t;

int get_file_version (FILE * f, const char *filename);
void load_ns_dbase (void);
void delnick (NickInfo * ni);
Nick_Stored_t CreateNickEntry(NickInfo *ni);
void load_cs_dbase (void);
char *oldmodeconv (short inmode);
void delchan (ChanInfo *ci);
Chan_Stored_t CreateChanEntry(ChanInfo *ci);
void load_ms_dbase (void);
void load_news_dbase (void);
void del_memolist (MemoList * ml);
void del_newslist (NewsList * nl);
list<Memo_t> CreateMemoEntry(MemoList *ml);
list<News_t> CreateNewsEntry(NewsList *nl);
void load_sop ();
void load_message ();
void load_akill ();
void load_clone ();


#endif
