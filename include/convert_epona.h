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
#ifndef _CONVERT_EPONA_H
#define _CONVERT_EPONA_H
#include "pch.h"
RCSID(convert_epona_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.3  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.2  2001/11/04 23:43:14  prez
** Updates for MS Visual C++ compilation (it works now!).
**
** Revision 1.1  2001/11/03 21:02:50  prez
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
#define EPO_CHANMAX		64

/* Maximum length of a nickname, including the trailing null.  This MUST be
 * at least one greater than the maximum allowable nickname length on your
 * network, or people will run into problems using Services!  The default
 * (32) works with all servers I know of. */
#define EPO_NICKMAX		32

/* Maximum length of a password */
#define EPO_PASSMAX		32

#ifndef PATH_MAX
#define PATH_MAX 512
#endif

/*************************************************************************/

struct EPO_dbFILE {
    int mode;			/* 'r' for reading, 'w' for writing */
    FILE *fp;			/* The normal file descriptor */
    FILE *backupfp;		/* Open file pointer to a backup copy of
				 *    the database file (if non-NULL) */
    char filename[PATH_MAX];	/* Name of the database file */
    char backupname[PATH_MAX];	/* Name of the backup file */
};

/*************************************************************************/

/* Prototypes and macros: */

int EPO_get_file_version(EPO_dbFILE *f);
EPO_dbFILE *EPO_open_db(const char *service, const char *filename, const char *mode, uint32 version);
void EPO_close_db(EPO_dbFILE *f);
#define EPO_read_db(f,buf,len)	(fread((buf),1,(len),(f)->fp))
#define EPO_getc_db(f)		(fgetc((f)->fp))

int EPO_read_int16(int16 *ret, EPO_dbFILE *f);
int EPO_read_int32(int32 *ret, EPO_dbFILE *f);
int EPO_read_string(char **ret, EPO_dbFILE *f);

#define EPO_read_int8(ret,f)	((*(ret)=fgetc((f)->fp))==EOF ? -1 : 0)
#define EPO_read_buffer(buf,f)	((EPO_read_db((f),(buf),sizeof(buf)) != sizeof(buf)) ? -1 : 0)
#define EPO_read_variable(var,f)	((EPO_read_db((f),&(var),sizeof(var)) != sizeof(var)) ? -1 : 0)

/*************************************************************************/

/* Version number for data files; if structures below change, increment
 * this.  (Otherwise -very- bad things will happen!) */

/* Version of the DB's we want to load ... */

#define EPO_CHAN_VERSION		16
#define EPO_EXCEPTION_VERSION		9
#define EPO_NEWS_VERSION		9
#define EPO_NICK_VERSION		13
#define EPO_OPER_VERSION		13

/* Set one of these where appropriate ... */
#undef EPO_IRC_BAHAMUT
#undef EPO_IRC_ULTIMATE
#undef EPO_IRC_UNREAL

/*************************************************************************/

/* Memo info structures.  Since both nicknames and channels can have memos,
 * we encapsulate memo data in a MemoList to make it easier to handle. */

struct EPO_Memo {
    int32 number;	/* Index number -- not necessarily array position! */
    int16 flags;
    time_t time;	/* When it was sent */
    char sender[EPO_NICKMAX];
    char *text;
};

#define EPO_MF_UNREAD	0x0001	/* Memo has not yet been read */

struct EPO_MemoInfo {
    int16 memocount, memomax;
    EPO_Memo *memos;
};

/*************************************************************************/

/* Nickname info structure.  Each nick structure is stored in one of 256
 * lists; the list is determined by the first character of the nick.  Nicks
 * are stored in alphabetical order within lists. */

struct EPO_NickAlias {
    char *nick;					/* Nickname */

    char *last_quit;				/* Last quit message */
    char *last_realname;			/* Last realname */
    char *last_usermask;			/* Last usermask */
    
    time_t time_registered;			/* When the nick was registered */
    time_t last_seen;				/* When it was seen online for the last time */
    
    int16 status;				/* See NS_* below */
    
    char *host;					/* I'm an alias of this */
};

struct EPO_NickCore {
    EPO_NickCore *prev, *next;

    char *display;				/* How the nick is displayed */
    char *pass;					/* Password of the nicks */

    char *email;				/* E-mail associated to the nick */
    char *greet;				/* Greet associated to the nick */
    int32 icq;					/* ICQ # associated to the nick */
    char *url;					/* URL associated to the nick */

    int32 flags;				/* See NI_* below */
    int16 language;				/* Language selected by nickname owner (LANG_*) */

    int16 accesscount;				/* # of entries */
    char **access;				/* Array of strings */

    EPO_MemoInfo memos;

    int16 channelcount;				/* Number of channels currently registered */
    int16 channelmax;				/* Maximum number of channels allowed */
};


/* Nickname status flags: */
#define EPO_NS_VERBOTEN		0x0002  	/* Nick may not be registered or used */
#define EPO_NS_NO_EXPIRE	0x0004		/* Nick never expires */
#define EPO_NS_IDENTIFIED	0x8000		/* User has IDENTIFY'd */
#define EPO_NS_RECOGNIZED	0x4000		/* ON_ACCESS true && SECURE flag not set */
#define EPO_NS_ON_ACCESS	0x2000		/* User comes from a known address */
#define EPO_NS_KILL_HELD	0x1000		/* Nick is being held after a kill */
#define EPO_NS_GUESTED		0x0100		/* SVSNICK has been sent but nick has not
				     		 * yet changed. An enforcer will be
				     		 * introduced when it does change. */
#define EPO_NS_MASTER		0x0200		/* Was a master nick; used to import old databases */
#define EPO_NS_TRANSGROUP	0xC000		/* Status flags that can be passed to a nick of the
									   same group during nick change */
#define EPO_NS_TEMPORARY	0xFF00		/* All temporary status flags */
/* These two are not used anymore */
#define EPO_NS_OLD_ENCRYPTEDPW	0x0001		/* Nickname password is encrypted */

/* Nickname setting flags: */
#define EPO_NI_KILLPROTECT	0x00000001	/* Kill others who take this nick */
#define EPO_NI_SECURE		0x00000002	/* Don't recognize unless IDENTIFY'd */
#define EPO_NI_MSG		0x00000004	/* Use PRIVMSGs instead of NOTICEs */
#define EPO_NI_MEMO_HARDMAX	0x00000008	/* Don't allow user to change memo limit */
#define EPO_NI_MEMO_SIGNON	0x00000010	/* Notify of memos at signon and un-away */
#define EPO_NI_MEMO_RECEIVE	0x00000020	/* Notify of new memos when sent */
#define EPO_NI_PRIVATE		0x00000040	/* Don't show in LIST to non-servadmins */
#define EPO_NI_HIDE_EMAIL	0x00000080	/* Don't show E-mail in INFO */
#define EPO_NI_HIDE_MASK	0x00000100	/* Don't show last seen address in INFO */
#define EPO_NI_HIDE_QUIT	0x00000200	/* Don't show last quit message in INFO */
#define EPO_NI_KILL_QUICK	0x00000400	/* Kill in 20 seconds instead of 60 */
#define EPO_NI_KILL_IMMED	0x00000800	/* Kill immediately instead of in 60 sec */
#define EPO_NI_SERVICES_OPER 	0x00001000	/* User is a Services operator */
#define EPO_NI_SERVICES_ADMIN	0x00002000	/* User is a Services admin */
#define EPO_NI_ENCRYPTEDPW	0x00004000	/* Nickname password is encrypted */
#define EPO_NI_SERVICES_ROOT    0x00008000	/* User is a Services root */

/* Languages.  Never insert anything in the middle of this list, or
 * everybody will start getting the wrong language!  If you want to change
 * the order the languages are displayed in for NickServ HELP SET LANGUAGE,
 * do it in language.c.
 */
#define EPO_LANG_EN_US		0		/* United States English */
#define EPO_LANG_JA_JIS		1		/* Japanese (JIS encoding) */
#define EPO_LANG_JA_EUC		2		/* Japanese (EUC encoding) */
#define EPO_LANG_JA_SJIS	3		/* Japanese (SJIS encoding) */
#define EPO_LANG_ES		4		/* Spanish */
#define EPO_LANG_PT		5		/* Portugese */
#define EPO_LANG_FR		6		/* French */
#define EPO_LANG_TR		7		/* Turkish */
#define EPO_LANG_IT		8		/* Italian */
#define EPO_LANG_DE		9		/* German */

#define EPO_NUM_LANGS		10		/* Number of languages */
#define EPO_USED_LANGS		6		/* Number of languages provided */

#define EPO_DEF_LANGUAGE	LANG_EN_US

/*************************************************************************/

/* Channel info structures.  Stored similarly to the nicks, except that
 * the second character of the channel name, not the first, is used to
 * determine the list.  (Hashing based on the first character of the name
 * wouldn't get very far. ;) ) */

/* Access levels for users. */
struct EPO_ChanAccess {
    int16 in_use;	/* 1 if this entry is in use, else 0 */
    int16 level;
    char nick[EPO_NICKMAX];	/* Guaranteed to be non-NULL if in use, NULL if not */
    time_t last_seen;
};

/* Note that these two levels also serve as exclusive boundaries for valid
 * access levels.  ACCESS_FOUNDER may be assumed to be strictly greater
 * than any valid access level, and ACCESS_INVALID may be assumed to be
 * strictly less than any valid access level.
 */
#define EPO_ACCESS_FOUNDER	10000	/* Numeric level indicating founder access */
#define EPO_ACCESS_INVALID	-10000	/* Used in levels[] for disabled settings */

#define EPO_ACCESS_VOP		3
#define EPO_ACCESS_HOP		4
#define EPO_ACCESS_AOP		5
#define EPO_ACCESS_SOP		10


/* AutoKick data. */
struct EPO_AutoKick {
    int16 in_use;
    int16 is_nick;	/* 1 if a regged nickname, 0 if a nick!user@host mask */
			/* Always 0 if not in use */
    int16 flags;
    char *mask;	/* Guaranteed to be non-NULL if in use, NULL if not */
    char *reason;
    char *creator;
    time_t addtime;
};

#define EPO_AK_USED	0x0001
#define EPO_AK_ISNICK	0x0002
#define EPO_AK_STUCK	0x0004

/* Bad words ... ugh */
struct EPO_BadWord {
	int16 in_use;
	
	char *word;
	int16 type;		/* BW_* below */	
};

#define EPO_BW_ANY	0
#define EPO_BW_SINGLE	1
#define EPO_BW_START 	2
#define EPO_BW_END	3




struct EPO_ChannelInfo {
    char name[EPO_CHANMAX];
    char *founder;
    char *successor;			/* Who gets the channel if the founder
					 * nick is dropped or expires */
    char founderpass[EPO_PASSMAX];
    char *desc;
    char *url;
    char *email;

    time_t time_registered;
    time_t last_used;
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[EPO_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;		/* When the last topic was set */

    int32 flags;			/* See below */
    char *forbidby;
    char *forbidreason;

    int16 bantype;

    int16 *levels;			/* Access levels for commands */

    int16 accesscount;
    EPO_ChanAccess *access;		/* List of authorized users */
    int16 akickcount;
    EPO_AutoKick *akick;		/* List of users to kickban */

    int32 mlock_on, mlock_off;		/* See channel modes below */
    int32 mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */
    char *mlock_flood;			/* NULL if no +f */
    char *mlock_redirect;		/* NULL if no +L */

    char *entry_message;		/* Notice sent on entering channel */

    EPO_MemoInfo memos;

    int32 botflags;			/* BS_* below */
    int16 *ttb;				/* Times to ban for each kicker */

    int16 bwcount;
    EPO_BadWord *badwords;		/* For BADWORDS kicker */
    int16 capsmin, capspercent;		/* For CAPS kicker */
    int16 floodlines, floodsecs;	/* For FLOOD kicker */
    int16 repeattimes;			/* For REPEAT kicker */
};

/* Retain topic even after last person leaves channel */
#define EPO_CI_KEEPTOPIC	0x00000001
/* Don't allow non-authorized users to be opped */
#define EPO_CI_SECUREOPS	0x00000002
/* Hide channel from ChanServ LIST command */
#define EPO_CI_PRIVATE		0x00000004
/* Topic can only be changed by SET TOPIC */
#define EPO_CI_TOPICLOCK	0x00000008
/* Those not allowed ops are kickbanned */
#define EPO_CI_RESTRICTED	0x00000010
/* Don't allow ChanServ and BotServ commands to do bad things to bigger levels */
#define EPO_CI_PEACE 		0x00000020
/* Don't allow any privileges unless a user is IDENTIFY'd with NickServ */
#define EPO_CI_SECURE		0x00000040
/* Don't allow the channel to be registered or used */
#define EPO_CI_VERBOTEN		0x00000080
/* Channel password is encrypted */
#define EPO_CI_ENCRYPTEDPW	0x00000100
/* Channel does not expire */
#define EPO_CI_NO_EXPIRE	0x00000200
/* Channel memo limit may not be changed */
#define EPO_CI_MEMO_HARDMAX	0x00000400
/* Send notice to channel on use of OP/DEOP */
#define EPO_CI_OPNOTICE		0x00000800
/* Stricter control of channel founder status */
#define EPO_CI_SECUREFOUNDER	0x00001000
/* Always sign kicks */
#define EPO_CI_SIGNKICK		0x00002000
/* Sign kicks if level is < than the one defined by the SIGNKICK level */
#define EPO_CI_SIGNKICK_LEVEL	0x00004000
/* Use the xOP lists */
#define EPO_CI_XOP		0x00008000

/* Indices for cmd_access[]: */
#define EPO_CA_INVITE			0
#define EPO_CA_AKICK			1
#define EPO_CA_SET			2	/* but not FOUNDER or PASSWORD */
#define EPO_CA_UNBAN			3
#define EPO_CA_AUTOOP			4
#define EPO_CA_AUTODEOP			5	/* Maximum, not minimum */
#define EPO_CA_AUTOVOICE		6
#define EPO_CA_OPDEOP			7	/* ChanServ commands OP and DEOP */
#define EPO_CA_ACCESS_LIST		8
#define EPO_CA_CLEAR			9
#define EPO_CA_NOJOIN			10	/* Maximum */
#define EPO_CA_ACCESS_CHANGE 		11
#define EPO_CA_MEMO			12
#define EPO_CA_ASSIGN       		13  /* BotServ ASSIGN command */
#define EPO_CA_BADWORDS     		14  /* BotServ BADWORDS command */
#define EPO_CA_NOKICK       		15  /* Not kicked by the bot */
#define EPO_CA_FANTASIA   		16
#define EPO_CA_SAY			17
#define EPO_CA_GREET            	18
#define EPO_CA_VOICEME			19
#define EPO_CA_VOICE			20
#define EPO_CA_GETKEY           	21
#define EPO_CA_AUTOHALFOP		22
#define EPO_CA_AUTOPROTECT		23
#define EPO_CA_OPDEOPME         	24
#define EPO_CA_HALFOPME         	25
#define EPO_CA_HALFOP           	26
#define EPO_CA_PROTECTME        	27
#define EPO_CA_PROTECT          	28
#define EPO_CA_KICKME           	29
#define EPO_CA_KICK             	30
#define EPO_CA_SIGNKICK			31
/* #define EPO_CA_AUTOADMIN		32
#define EPO_CA_ADMINME          	33
#define EPO_CA_ADMIN            	34 */
#define EPO_CA_BANME            	32
#define EPO_CA_BAN              	33
#define EPO_CA_TOPIC            	34
#define EPO_CA_INFO             	35

#define EPO_CA_SIZE			36

/* BotServ SET flags */
#define EPO_BS_DONTKICKOPS      0x00000001
#define EPO_BS_DONTKICKVOICES   0x00000002
#define EPO_BS_FANTASY          0x00000004
#define EPO_BS_SYMBIOSIS        0x00000008
#define EPO_BS_GREET            0x00000010
#define EPO_BS_NOBOT            0x00000020

/* BotServ Kickers flags */
#define EPO_BS_KICK_BOLDS 	0x80000000
#define EPO_BS_KICK_COLORS 	0x40000000
#define EPO_BS_KICK_REVERSES	0x20000000
#define EPO_BS_KICK_UNDERLINES	0x10000000
#define EPO_BS_KICK_BADWORDS	0x08000000
#define EPO_BS_KICK_CAPS	0x04000000
#define EPO_BS_KICK_FLOOD	0x02000000
#define EPO_BS_KICK_REPEAT	0x01000000

/* Indices for TTB (Times To Ban) */
#define EPO_TTB_BOLDS 		0
#define EPO_TTB_COLORS 		1
#define EPO_TTB_REVERSES	2
#define EPO_TTB_UNDERLINES	3
#define EPO_TTB_BADWORDS	4
#define EPO_TTB_CAPS		5
#define EPO_TTB_FLOOD		6
#define EPO_TTB_REPEAT		7

#define EPO_TTB_SIZE 		8

#define EPO_CMODE_i 0x00000001
#define EPO_CMODE_m 0x00000002
#define EPO_CMODE_n 0x00000004
#define EPO_CMODE_p 0x00000008
#define EPO_CMODE_s 0x00000010
#define EPO_CMODE_t 0x00000020
#define EPO_CMODE_k 0x00000040		/* These two used only by ChanServ */
#define EPO_CMODE_l 0x00000080

/* The two modes below are for IRC_DREAMFORGE servers only. */
#define EPO_CMODE_R 0x00000100		/* Only identified users can join */
#define EPO_CMODE_r 0x00000200		/* Set for all registered channels */

/* This mode is for IRC_BAHAMUT servers only. */
#ifdef EPO_IRC_BAHAMUT
#define EPO_CMODE_c 0x00000400		/* Colors can't be used */
#define EPO_CMODE_O 0x00008000		/* Only opers can join */
#endif

/* These modes are for IRC_ULTIMATE servers only. */
#ifdef EPO_IRC_ULTIMATE
#define EPO_CMODE_f 0x00000400
#define EPO_CMODE_x 0x00000800
#define EPO_CMODE_A 0x00001000
#define EPO_CMODE_I 0x00002000
#define EPO_CMODE_K 0x00004000
#define EPO_CMODE_L 0x00008000
#define EPO_CMODE_O 0x00010000
#define EPO_CMODE_S 0x00020000
#endif

/* These modes are for IRC_UNREAL servers only. */
#ifdef EPO_IRC_UNREAL
#define EPO_CMODE_c 0x00000400
#define EPO_CMODE_A 0x00000800
#define EPO_CMODE_H 0x00001000
#define EPO_CMODE_K 0x00002000
#define EPO_CMODE_L 0x00004000
#define EPO_CMODE_O 0x00008000		
#define EPO_CMODE_Q 0x00010000
#define EPO_CMODE_S 0x00020000
#define EPO_CMODE_V 0x00040000
#define EPO_CMODE_f 0x00080000
#define EPO_CMODE_G 0x00100000
#define EPO_CMODE_C 0x00200000
#define EPO_CMODE_u 0x00400000
#define EPO_CMODE_z 0x00800000
#define EPO_CMODE_N 0x01000000
#endif

/*************************************************************************/

/* Constants for news types. */

#define EPO_NEWS_LOGON		0
#define EPO_NEWS_OPER		1
#define EPO_NEWS_RANDOM 	2

struct EPO_NewsItem {
    int16 type;
    int32 num;		/* Numbering is separate for login and oper news */
    char *text;
    char who[EPO_NICKMAX];
    time_t time;
};


/*************************************************************************/

/* Ignorance list data. */

 struct EPO_IgnoreData {
    char who[EPO_NICKMAX];
    time_t time;	/* When do we stop ignoring them? */
};

/*************************************************************************/

struct EPO_Akill {
	char *user;			/* User part of the AKILL */
	char *host;			/* Host part of the AKILL */
	
	char *by;			/* Who set the akill */
	char *reason;		/* Why they got akilled */
	
	time_t seton;		/* When it was set */
	time_t expires;		/* When it expires */
};

/*************************************************************************/

struct EPO_SXLine {
	char *mask;
	char *by;
	char *reason;
	time_t seton;
	time_t expires;
};


/*************************************************************************/

struct EPO_HostCache {
	char *host;							/* The hostname */
	int32 ip;							/* The IP address */

	int16 status;						/* HC_* below */
	time_t used;						/* When was this entry last used? */
};

/* We assume that values < 0 are in-progress values, and values > 0 are
 * proxy value. 0 is the normal value.
 */

#define HC_QUEUED		-2				/* Waiting to be scanned */
#define HC_PROGRESS		-1				/* Currently being scanned */
#define HC_NORMAL		 0				/* No proxy found on this host */
#define HC_WINGATE		 1				/* Wingate found */
#define HC_SOCKS4		 2				/* Socks4 found */
#define HC_SOCKS5		 3				/* Socks5 found */
#define HC_HTTP			 4				/* HTTP proxy found */

/*************************************************************************/

struct EPO_Exception {
    char *mask;			/* Hosts to which this exception applies */
    int limit;			/* Session limit for exception */
    char who[EPO_NICKMAX];	/* Nick of person who added the exception */
    char *reason;               /* Reason for exception's addition */
    time_t time;		/* When this exception was added */
    time_t expires;		/* Time when it expires. 0 == no expiry */
    int num;			/* Position in exception list; used to track
    				 * positions when deleting entries. It is 
				 * symbolic and used internally. It is 
				 * calculated at load time and never saved. */
};

/*************************************************************************/


void EPO_load_ns_dbase(void);
EPO_NickCore *EPO_findcore(const char *nick, EPO_NickCore **nclists);
int EPO_delnick(EPO_NickAlias *na);
int EPO_delcore(EPO_NickCore *nc);
void EPO_load_cs_dbase(void);
void reset_levels(EPO_ChannelInfo *ci);
int EPO_delchan(EPO_ChannelInfo *ci);
void EPO_load_old_ms_dbase(void);
void EPO_load_news(void);
void EPO_load_os_dbase(void);
void EPO_load_akill(void);
void EPO_load_exceptions(void);

/*************************************************************************/

class Nick_Stored_t;
class Chan_Stored_t;
class Memo_t;
class News_t;

/* OK -- now for the magick conversions ... */
Nick_Stored_t EPO_CreateNickEntry(EPO_NickAlias *na, EPO_NickCore *nc);
Chan_Stored_t EPO_CreateChanEntry(EPO_ChannelInfo *ci);
list<Memo_t> EPO_CreateMemoEntry(EPO_MemoInfo *ml, char *nick);
list<News_t> EPO_CreateNewsEntry(EPO_MemoInfo *nl, char *chan);

#endif  /* CONVERT */
#endif	/* _CONVERT_EPONA_H */
