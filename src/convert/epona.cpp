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
#define RCSID(x,y) const char *rcsid_convert_epona_cpp_ ## x () { return y; }
RCSID(convert_epona_cpp, "@(#)$Id$");

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

/* Based upon the DB loading routines from Epona 1.4.14
 * (c) 2000-2002 PegSoft <epona@pegsoft.net>
 */

#include "magick.h"
#include "convert/interface.h"
#include "convert/epona.h"

#define epona_NickDBName	"nick.db"
#define	epona_ChanDBName	"chan.db"
#define	epona_OperDBName	"oper.db"
#define	epona_NewsDBName	"news.db"
#define	epona_ExceptionDBName	"exception.db"
#define	epona_AutokillDBName	"akill.db"

#define	epona_NICK_VERSION	13
#define	epona_CHAN_VERSION	16
#define	epona_OPER_VERSION	13
#define	epona_NEWS_VERSION	9
#define	epona_EXCEPTION_VERSION	9

#define	epona_NSAllowKillImmed	0
#define	epona_CSMaxReg		20
#define	epona_CSDefBantype		2

#undef	epona_IRC_BAHAMUT
#undef	epona_IRC_UNREAL
#undef	epona_IRC_ULTIMATE

typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

/* Maximum length of a channel name, including the trailing null.  Any
 * channels with a length longer than (CHANMAX-1) including the leading #
 * will not be usable with ChanServ. */
#define epona_CHANMAX		64

/* Maximum length of a nickname, including the trailing null.  This MUST be
 * at least one greater than the maximum allowable nickname length on your
 * network, or people will run into problems using Services!  The default
 * (32) works with all servers I know of. */
#define epona_NICKMAX		32

/* Maximum length of a password */
#define epona_PASSMAX		32

/* Memo info structures.  Since both nicknames and channels can have memos,
 * we encapsulate memo data in a MemoList to make it easier to handle. */

struct epona_Memo {
    uint32 number;	/* Index number -- not necessarily array position! */
    int16 flags;
    time_t time;	/* When it was sent */
    char sender[epona_NICKMAX];
    char *text;
};

#define epona_MF_UNREAD	0x0001	/* epona_Memo has not yet been read */

struct epona_MemoInfo {
    int16 memocount, memomax;
    epona_Memo *memos;
};

/*************************************************************************/

/* NickServ nickname structures. */

struct epona_NickAlias {
	epona_NickAlias *next, *prev;
	
	char *nick;						/* Nickname */
	
	char *last_quit;				/* Last quit message */
	char *last_realname;			/* Last realname */
	char *last_usermask;			/* Last usermask */
    
    time_t time_registered;			/* When the nick was registered */
    time_t last_seen;				/* When it was seen online for the last time */
    
    int16 status;					/* See NS_* below */
    
    char *nc;						/* I'm an alias of this */
    
    /* Not saved */
    void *u;						/* Current online user that has me */
};

struct epona_NickCore {
	epona_NickCore *next, *prev;
	
	char *display;					/* How the nick is displayed */
	char *pass;						/* Password of the nicks */
	
	char *email;					/* E-mail associated to the nick */
	char *greet;					/* Greet associated to the nick */
	uint32 icq;						/* ICQ # associated to the nick */
	char *url;						/* URL associated to the nick */
	
	int32 flags;					/* See NI_* below */
	uint16 language;				/* Language selected by nickname owner (LANG_*) */

    int16 accesscount;				/* # of entries */
    char **access;					/* Array of strings */
    
    epona_MemoInfo memos;

    uint16 channelcount;			/* Number of channels currently registered */
    uint16 channelmax;				/* Maximum number of channels allowed */
    
    /* Unsaved data */
    
    time_t lastmail;				/* Last time this nick record got a mail */
};

/* Nickname status flags: */
#define epona_NS_VERBOTEN		0x0002  	/* Nick may not be registered or used */
#define epona_NS_NO_EXPIRE	0x0004      /* Nick never expires */
#define epona_NS_IDENTIFIED	0x8000      /* User has IDENTIFY'd */
#define epona_NS_RECOGNIZED	0x4000      /* ON_ACCESS true && SECURE flag not set */
#define epona_NS_ON_ACCESS	0x2000      /* User comes from a known address */
#define epona_NS_KILL_HELD	0x1000      /* Nick is being held after a kill */
#define epona_NS_GUESTED	    0x0100	    /* SVSNICK has been sent but nick has not
				     				 * yet changed. An enforcer will be
				     				 * introduced when it does change. */
#define epona_NS_MASTER		0x0200		/* Was a master nick; used to import old databases */
#define epona_NS_TRANSGROUP	0xC000		/* Status flags that can be passed to a nick of the
									   same group during nick change */
#define epona_NS_TEMPORARY	0xFF00      /* All temporary status flags */
/* These two are not used anymore */
#define epona_NS_OLD_ENCRYPTEDPW	0x0001  /* Nickname password is encrypted */

/* Nickname setting flags: */
#define epona_NI_KILLPROTECT		0x00000001  /* Kill others who take this nick */
#define epona_NI_SECURE			0x00000002  /* Don't recognize unless IDENTIFY'd */
#define epona_NI_MSG              0x00000004	/* Use PRIVMSGs instead of NOTICEs */
#define epona_NI_MEMO_HARDMAX		0x00000008  /* Don't allow user to change memo limit */
#define epona_NI_MEMO_SIGNON		0x00000010  /* Notify of memos at signon and un-away */
#define epona_NI_MEMO_RECEIVE		0x00000020  /* Notify of new memos when sent */
#define epona_NI_PRIVATE			0x00000040  /* Don't show in LIST to non-servadmins */
#define epona_NI_HIDE_EMAIL		0x00000080  /* Don't show E-mail in INFO */
#define epona_NI_HIDE_MASK		0x00000100  /* Don't show last seen address in INFO */
#define epona_NI_HIDE_QUIT		0x00000200  /* Don't show last quit message in INFO */
#define epona_NI_KILL_QUICK		0x00000400  /* Kill in 20 seconds instead of 60 */
#define epona_NI_KILL_IMMED		0x00000800  /* Kill immediately instead of in 60 sec */
#define epona_NI_SERVICES_OPER 	0x00001000	/* User is a Services operator */
#define epona_NI_SERVICES_ADMIN	0x00002000	/* User is a Services admin */
#define epona_NI_ENCRYPTEDPW		0x00004000	/* Nickname password is encrypted */
#define epona_NI_SERVICES_ROOT    0x00008000  /* User is a Services root */

/* Languages.  Never insert anything in the middle of this list, or
 * everybody will start getting the wrong language!  If you want to change
 * the order the languages are displayed in for NickServ HELP SET LANGUAGE,
 * do it in language.c.
 */
#define epona_LANG_EN_US              0	/* United States English */
#define epona_LANG_JA_JIS             1	/* Japanese (JIS encoding) */
#define epona_LANG_JA_EUC             2	/* Japanese (EUC encoding) */
#define epona_LANG_JA_SJIS            3	/* Japanese (SJIS encoding) */
#define epona_LANG_ES                 4	/* Spanish */
#define epona_LANG_PT                 5	/* Portugese */
#define epona_LANG_FR                 6	/* French */
#define epona_LANG_TR                 7	/* Turkish */
#define epona_LANG_IT                 8	/* Italian */
#define epona_LANG_DE                 9   /* German */
#define epona_LANG_CAT               10   /* Catalan */
#define epona_LANG_GR                11   /* Greek */

#define epona_NUM_LANGS              12	/* Number of languages */
#define epona_USED_LANGS              8	/* Number of languages provided */

#define DEF_LANGUAGE	epona_LANG_EN_US


/* Channel info structures.  Stored similarly to the nicks, except that
 * the second character of the channel name, not the first, is used to
 * determine the list.  (Hashing based on the first character of the name
 * wouldn't get very far. ;) ) */

/* Access levels for users. */
struct epona_ChanAccess {
    int16 in_use;	/* 1 if this entry is in use, else 0 */
    int16 level;
    char *nc;	/* Guaranteed to be non-NULL if in use, NULL if not */
    time_t last_seen;
};

/* Note that these two levels also serve as exclusive boundaries for valid
 * access levels.  epona_ACCESS_FOUNDER may be assumed to be strictly greater
 * than any valid access level, and epona_ACCESS_INVALID may be assumed to be
 * strictly less than any valid access level.
 */
#define epona_ACCESS_FOUNDER	10000	/* Numeric level indicating founder access */
#define epona_ACCESS_INVALID	-10000	/* Used in levels[] for disabled settings */

/* Levels for xOP */

#define epona_ACCESS_VOP		3
#define epona_ACCESS_HOP		4
#define epona_ACCESS_AOP		5
#define epona_ACCESS_SOP		10

/* AutoKick data. */
struct epona_AutoKick {
    int16 in_use;   /* Always 0 if not in use */
    int16 is_nick;	/* 1 if a regged nickname, 0 if a nick!user@host mask */

	int16 flags;

    union {
		char *mask;		/* Guaranteed to be non-NULL if in use, NULL if not */
		char *nc;	/* Same */
    } u;
    char *reason;
    
    char *creator;
    time_t addtime;
};

#define epona_AK_USED		0x0001
#define epona_AK_ISNICK	0x0002
#define epona_AK_STUCK    0x0004

/* Structure used to contain bad words. */
 
struct epona_BadWord {
	int16 in_use;
	
	char *word;
	int16 type;		/* epona_BW_* below */	
};

#define epona_BW_ANY 		0
#define epona_BW_SINGLE 	1
#define epona_BW_START 	2
#define epona_BW_END 		3

struct epona_ChanInfo {
    epona_ChanInfo *next, *prev;
    char name[epona_CHANMAX];
    char *founder;
    char *successor;		/* Who gets the channel if the founder
					 			 * nick is dropped or expires */
    char founderpass[epona_PASSMAX];
    char *desc;
    char *url;
    char *email;

    time_t time_registered;
    time_t last_used;
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[epona_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;		/* When the last topic was set */

    uint32 flags;				/* See below */
    char *forbidby;
    char *forbidreason;
    
    int16 bantype;

    int16 *levels;				/* Access levels for commands */

    int16 accesscount;
    epona_ChanAccess *access;			/* List of authorized users */
    int16 akickcount;
    epona_AutoKick *akick;			/* List of users to kickban */

    uint32 mlock_on, mlock_off;	/* See channel modes below */
    uint32 mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */
    char *mlock_flood;			/* NULL if no +f */
    char *mlock_redirect;		/* NULL if no +L */

    char *entry_message;		/* Notice sent on entering channel */

    epona_MemoInfo memos;

    void *c;			/* Pointer to channel record (if   *
					 			 *    channel is currently in use) */
					 
    /* For BotServ */
    
    char *bi;						/* Bot used on this channel */
    uint32 botflags;				/* BS_* below */
    int16 *ttb;						/* Times to ban for each kicker */
    
    int16 bwcount;
    epona_BadWord *badwords;				/* For BADWORDS kicker */
    int16 capsmin, capspercent;		/* For CAPS kicker */
    int16 floodlines, floodsecs;	/* For FLOOD kicker */
    int16 repeattimes;				/* For REPEAT kicker */
};

/* Retain topic even after last person leaves channel */
#define epona_CI_KEEPTOPIC	0x00000001
/* Don't allow non-authorized users to be opped */
#define epona_CI_SECUREOPS	0x00000002
/* Hide channel from ChanServ LIST command */
#define epona_CI_PRIVATE	0x00000004
/* Topic can only be changed by SET TOPIC */
#define epona_CI_TOPICLOCK	0x00000008
/* Those not allowed ops are kickbanned */
#define epona_CI_RESTRICTED	0x00000010
/* Don't allow ChanServ and BotServ commands to do bad things to bigger levels */
#define epona_CI_PEACE  0x00000020
/* Don't allow any privileges unless a user is IDENTIFY'd with NickServ */
#define epona_CI_SECURE	0x00000040
/* Don't allow the channel to be registered or used */
#define epona_CI_VERBOTEN	0x00000080
/* Channel password is encrypted */
#define epona_CI_ENCRYPTEDPW	0x00000100
/* Channel does not expire */
#define epona_CI_NO_EXPIRE	0x00000200
/* Channel memo limit may not be changed */
#define epona_CI_MEMO_HARDMAX	0x00000400
/* Send notice to channel on use of OP/DEOP */
#define epona_CI_OPNOTICE	0x00000800
/* Stricter control of channel founder status */
#define epona_CI_SECUREFOUNDER 0x00001000
/* Always sign kicks */
#define epona_CI_SIGNKICK 0x00002000
/* Sign kicks if level is < than the one defined by the SIGNKICK level */
#define epona_CI_SIGNKICK_LEVEL 0x00004000
/* Use the xOP lists */
#define epona_CI_XOP 0x00008000

/* TEMPORARY - ChanServ is on the channel. */
#define epona_CI_INHABIT 0x80000000

/* Indices for cmd_access[]: */
#define epona_CA_INVITE			0
#define epona_CA_AKICK			1
#define epona_CA_SET				2	/* but not FOUNDER or PASSWORD */
#define epona_CA_UNBAN			3
#define epona_CA_AUTOOP			4
#define epona_CA_AUTODEOP			5	/* Maximum, not minimum */
#define epona_CA_AUTOVOICE		6
#define epona_CA_OPDEOP			7	/* ChanServ commands OP and DEOP */
#define epona_CA_ACCESS_LIST		8
#define epona_CA_CLEAR			9
#define epona_CA_NOJOIN			10	/* Maximum */
#define epona_CA_ACCESS_CHANGE 	11
#define epona_CA_MEMO				12
#define epona_CA_ASSIGN       	13  /* BotServ ASSIGN command */
#define epona_CA_BADWORDS     	14  /* BotServ BADWORDS command */
#define epona_CA_NOKICK       	15  /* Not kicked by the bot */
#define epona_CA_FANTASIA   		16
#define epona_CA_SAY				17
#define epona_CA_GREET            18
#define epona_CA_VOICEME			19
#define epona_CA_VOICE			20
#define epona_CA_GETKEY           21
#define epona_CA_AUTOHALFOP		22
#define epona_CA_AUTOPROTECT		23
#define epona_CA_OPDEOPME         24
#define epona_CA_HALFOPME         25
#define epona_CA_HALFOP           26
#define epona_CA_PROTECTME        27
#define epona_CA_PROTECT          28
#define epona_CA_KICKME           29
#define epona_CA_KICK             30
#define epona_CA_SIGNKICK			31
/* #define epona_CA_AUTOADMIN        32
#define epona_CA_ADMINME          33
#define epona_CA_ADMIN            34 */
#define epona_CA_BANME            32
#define epona_CA_BAN              33
#define epona_CA_TOPIC            34
#define epona_CA_INFO             35

#define epona_CA_SIZE		36

/* BotServ SET flags */
#define epona_BS_DONTKICKOPS      0x00000001
#define epona_BS_DONTKICKVOICES   0x00000002
#define epona_BS_FANTASY          0x00000004
#define epona_BS_SYMBIOSIS        0x00000008
#define epona_BS_GREET            0x00000010
#define epona_BS_NOBOT            0x00000020

/* BotServ Kickers flags */
#define epona_BS_KICK_BOLDS 		0x80000000
#define epona_BS_KICK_COLORS 		0x40000000
#define epona_BS_KICK_REVERSES	0x20000000
#define epona_BS_KICK_UNDERLINES	0x10000000
#define epona_BS_KICK_BADWORDS	0x08000000
#define epona_BS_KICK_CAPS		0x04000000
#define epona_BS_KICK_FLOOD		0x02000000
#define epona_BS_KICK_REPEAT		0x01000000

/* Indices for TTB (Times To Ban) */
#define epona_TTB_BOLDS 		0
#define epona_TTB_COLORS 		1
#define epona_TTB_REVERSES	2
#define epona_TTB_UNDERLINES  3
#define epona_TTB_BADWORDS	4
#define epona_TTB_CAPS		5
#define epona_TTB_FLOOD		6
#define epona_TTB_REPEAT		7

#define epona_TTB_SIZE 		8

#define epona_CMODE_i 0x00000001
#define epona_CMODE_m 0x00000002
#define epona_CMODE_n 0x00000004
#define epona_CMODE_p 0x00000008
#define epona_CMODE_s 0x00000010
#define epona_CMODE_t 0x00000020
#define epona_CMODE_k 0x00000040		/* These two used only by ChanServ */
#define epona_CMODE_l 0x00000080

/* The two modes below are for IRC_DREAMFORGE servers only. */
#define epona_CMODE_R 0x00000100		/* Only identified users can join */
#define epona_CMODE_r 0x00000200		/* Set for all registered channels */

/* This mode is for IRC_BAHAMUT servers only. */
#ifdef IRC_BAHAMUT
#define epona_CMODE_c 0x00000400		/* Colors can't be used */
#define epona_CMODE_M 0x00000800      /* Non-regged nicks can't send messages */
#define epona_CMODE_O 0x00008000		/* Only opers can join */
#endif

/* These modes are for IRC_ULTIMATE servers only. */
#ifdef IRC_ULTIMATE
#define epona_CMODE_f 0x00000400
#define epona_CMODE_x 0x00000800
#define epona_CMODE_A 0x00001000
#define epona_CMODE_I 0x00002000
#define epona_CMODE_K 0x00004000
#define epona_CMODE_L 0x00008000
#define epona_CMODE_O 0x00010000
#define epona_CMODE_S 0x00020000
#endif

/* These modes are for IRC_UNREAL servers only. */
#ifdef IRC_UNREAL
#define epona_CMODE_c 0x00000400
#define epona_CMODE_A 0x00000800
#define epona_CMODE_H 0x00001000
#define epona_CMODE_K 0x00002000
#define epona_CMODE_L 0x00004000
#define epona_CMODE_O 0x00008000		
#define epona_CMODE_Q 0x00010000
#define epona_CMODE_S 0x00020000
#define epona_CMODE_V 0x00040000
#define epona_CMODE_f 0x00080000
#define epona_CMODE_G 0x00100000
#define epona_CMODE_C 0x00200000
#define epona_CMODE_u 0x00400000
#define epona_CMODE_z 0x00800000
#define epona_CMODE_N 0x01000000
#endif

static int epona_def_levels[][2] = {
    { epona_CA_AUTOOP,                     5 },
    { epona_CA_AUTOVOICE,                  3 },
    { epona_CA_AUTODEOP,                  -1 },
    { epona_CA_NOJOIN,                    -2 },
    { epona_CA_INVITE,                     5 },
    { epona_CA_AKICK,                     10 },
    { epona_CA_SET,     		epona_ACCESS_INVALID },
    { epona_CA_CLEAR,   		epona_ACCESS_INVALID },
    { epona_CA_UNBAN,                      5 },
    { epona_CA_OPDEOP,                     5 },
    { epona_CA_ACCESS_LIST,                1 },
    { epona_CA_ACCESS_CHANGE,             10 },
    { epona_CA_MEMO,                      10 },
    { epona_CA_ASSIGN,  		epona_ACCESS_INVALID },
    { epona_CA_BADWORDS,                  10 },
    { epona_CA_NOKICK,                     1 },
    { epona_CA_FANTASIA,			         3 },
    { epona_CA_SAY,				         5 },
    { epona_CA_GREET,                      5 },
    { epona_CA_VOICEME,			         3 },
    { epona_CA_VOICE,				         5 },
    { epona_CA_GETKEY,                     5 },
    { epona_CA_AUTOHALFOP,                 4 },
    { epona_CA_AUTOPROTECT,               10 },
    { epona_CA_OPDEOPME,                   5 },
    { epona_CA_HALFOPME,                   4 },
    { epona_CA_HALFOP,                     5 },
    { epona_CA_PROTECTME,                 10 },
    { epona_CA_PROTECT,  		epona_ACCESS_INVALID },
    { epona_CA_KICKME,               		 5 },
    { epona_CA_KICK,                       5 },
    { epona_CA_SIGNKICK, 		epona_ACCESS_INVALID },
    { epona_CA_BANME,                      5 },
    { epona_CA_BAN,                        5 },
    { epona_CA_TOPIC,         epona_ACCESS_INVALID },
    { epona_CA_INFO,          epona_ACCESS_INVALID },
    { -1 }
};

/*************************************************************************/

/* Constants for news types. */

struct epona_NewsItem {
    int16 type;
    int32 num;		/* Numbering is separate for login and oper news */
    char *text;
    char who[epona_NICKMAX];
    time_t time;
};

#define epona_NEWS_LOGON		0
#define epona_NEWS_OPER		1
#define epona_NEWS_RANDOM 	2

struct epona_Akill {
	char *user;			/* User part of the AKILL */
	char *host;			/* Host part of the AKILL */
	
	char *by;			/* Who set the akill */
	char *reason;		/* Why they got akilled */
	
	time_t seton;		/* When it was set */
	time_t expires;		/* When it expires */
};

struct epona_Exception {
    char *mask;			/* Hosts to which this exception applies */
    int limit;			/* Session limit for exception */
    char who[epona_NICKMAX];		/* Nick of person who added the exception */
    char *reason;               /* Reason for exception's addition */
    time_t time;		/* When this exception was added */
    time_t expires;		/* Time when it expires. 0 == no expiry */
    int num;			/* Position in exception list; used to track
    				 * positions when deleting entries. It is 
				 * symbolic and used internally. It is 
				 * calculated at load time and never saved. */
};

struct epona_dbFILE {
    int mode;			/* 'r' for reading, 'w' for writing */
    FILE *fp;			/* The normal file descriptor */
    FILE *backupfp;		/* Open file pointer to a backup copy of
				 *    the database file (if non-NULL) */
    char filename[PATH_MAX];	/* Name of the database file */
    char backupname[PATH_MAX];	/* Name of the backup file */
};

#define epona_read_db(f,buf,len)	(fread((buf),1,(len),(f)->fp))
#define epona_getc_db(f)		(fgetc((f)->fp))

#define epona_read_int8(ret,f)	((*(ret)=fgetc((f)->fp))==EOF ? -1 : 0)
#define epona_read_buffer(buf,f)	(epona_read_db((f),(buf),sizeof(buf)) == sizeof(buf))
#define epona_read_buflen(buf,len,f)	(epona_read_db((f),(buf),(len)) == (len))
#define epona_read_variable(var,f)	(epona_read_db((f),&(var),sizeof(var)) == sizeof(var))

int epona_get_file_version(epona_dbFILE *f)
{
    FILE *fp = f->fp;
    int version = fgetc(fp)<<24 | fgetc(fp)<<16 | fgetc(fp)<<8 | fgetc(fp);
    if (ferror(fp)) {
#ifndef NOT_MAIN
	SLOG(LM_ERROR, "Error reading version number on %s",( f->filename));
#endif
	return 0;
    } else if (feof(fp)) {
#ifndef NOT_MAIN
	SLOG(LM_ERROR, "Error reading version number on %s: End of file detected",(
		f->filename));
#endif
	return 0;
    } else if (version < 1) {
#ifndef NOT_MAIN
	SLOG(LM_ERROR, "Invalid version number (%d) on %s",( version, f->filename));
#endif
	return 0;
    }
    return version;
}

static epona_dbFILE *epona_open_db_read(const char *service, const char *filename)
{
    epona_dbFILE *f;
    FILE *fp;

    f = (epona_dbFILE *) calloc(sizeof(*f), 1);
    if (!f) {
#ifndef NOT_MAIN
	SLOG(LM_ERROR, "Can't read %s database %s",( service, filename));
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
	    SLOG(LM_ERROR, "Can't read %s database %s",( service, f->filename));
#endif
	free(f);
	errno = errno_save;
	return NULL;
    }
    f->fp = fp;
    f->backupfp = NULL;
    return f;
}

epona_dbFILE *epona_open_db(const char *service, const char *filename, const char *mode, uint32 version)
{
    static_cast<void>(version);
    if (*mode == 'r') {
	return epona_open_db_read(service, filename);
    } else {
	errno = EINVAL;
	return NULL;
    }
}

void epona_close_db(epona_dbFILE *f)
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

int epona_read_int16(uint16 *ret, epona_dbFILE *f)
{
    int c1, c2;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    if (c1 == EOF || c2 == EOF)
	return -1;
    *ret = c1<<8 | c2;
    return 0;
}

int epona_read_int16(int16 *ret, epona_dbFILE *f)
{
    return epona_read_int16((uint16 *) ret, f);
}

int epona_read_int32(uint32 *ret, epona_dbFILE *f)
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

int epona_read_int32(int32 *ret, epona_dbFILE *f)
{
    return epona_read_int32((uint32 *) ret, f);
}

int epona_read_ptr(void **ret, epona_dbFILE *f)
{
    int c;

    c = fgetc(f->fp);
    if (c == EOF)
	return -1;
    *ret = (c ? (void *)1 : (void *)0);
    return 0;
}

int epona_read_string(char **ret, epona_dbFILE *f)
{
    char *s;
    uint16 len;

    if (epona_read_int16(&len, f) < 0)
	return -1;
    if (len == 0) {
	*ret = NULL;
	return 0;
    }
    s = (char *) calloc(len, 1);
    if (len != fread(s, 1, len, f->fp)) {
	free(s);
	return -1;
    }
    *ret = s;
    return 0;
}

int epona_delnick(epona_NickAlias *na);
static int epona_delcore(epona_NickCore *nc);
static int epona_delchan(epona_ChanInfo *ci);
static void epona_reset_levels(epona_ChanInfo *ci);

#define epona_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( epona_NickDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

/* Loads NickServ database versions 5 to 11 (<= 4 is not supported) */

void epona_load_old_nick()
{
    epona_dbFILE *f;
    int ver, i, j, c;
    epona_NickAlias *na;
    epona_NickCore *nc;
    int failed = 0;

    int16 tmp16;
    int32 tmp32;
    
    char bufn[epona_NICKMAX], bufp[epona_PASSMAX];
    char *email, *greet, *url, *forbidby, *forbidreason;
    uint32 icq;

    if (!(f = epona_open_db("NickServ", epona_NickDBName, "r", epona_NICK_VERSION)))
	return;

	ver = epona_get_file_version(f);
	if (ver <= 4) {
		SLOG(LM_EMERGENCY, "Unsupported version number (%d) on %s",( ver, epona_NickDBName));
		epona_close_db(f);
		return;
	}

	for (i = 0; i < 256 && !failed; i++) {
	    while ((c = epona_getc_db(f)) == 1) {
			if (c != 1) SLOG(LM_EMERGENCY, "Invalid format in %s",( epona_NickDBName));
		
			na = (epona_NickAlias *) calloc(sizeof(epona_NickAlias), 1);
		
			epona_SAFE(epona_read_buffer(bufn, f));
			na->nick = strdup(bufn);
			epona_SAFE(epona_read_buffer(bufp, f));			/* Will be used later if needed */
			
			epona_SAFE(epona_read_string(&url, f));
			epona_SAFE(epona_read_string(&email, f));
			if (ver>=10) epona_SAFE(epona_read_int32(&icq, f));
			else icq = 0;
			if (ver>=9) epona_SAFE(epona_read_string(&greet, f));
			else greet = NULL;
			
			epona_SAFE(epona_read_string(&na->last_usermask, f));
			epona_SAFE(epona_read_string(&na->last_realname, f));
			epona_SAFE(epona_read_string(&na->last_quit, f));
			
			epona_SAFE(epona_read_int32(&tmp32, f));
			na->time_registered = tmp32;
			epona_SAFE(epona_read_int32(&tmp32, f));
			na->last_seen = tmp32;
			
			epona_SAFE(epona_read_int16(&na->status, f));
			na->status &= ~epona_NS_TEMPORARY;
#ifdef GETPASS
			if (na->status & epona_NS_OLD_ENCRYPTEDPW) {
		    	/* Bail: it makes no sense to continue with encrypted
		    	 * passwords, since we won't be able to verify them */
		    	SLOG(LM_EMERGENCY, "%s: load database: password for %s encrypted but encryption disabled, aborting",( "NickServ", na->nick));
			}
#elsif defined(JP2CRYPT) || defined(DESCRYPT) || defined(MD5CRYPT)
			if (na->status & epona_NS_OLD_ENCRYPTEDPW) {
				SLOG(LM_EMERGENCY, "%s: load database: password for %s encrypted "
		          "but not with the same encryption scheme, aborting",(
		          "NickServ", na->nick));
			}
#endif
			if (ver>=9) {
				epona_SAFE(epona_read_string(&forbidby, f));
				epona_SAFE(epona_read_string(&forbidreason, f));
				/* Cleanup */
				if (forbidby && *forbidby == '@') {
					free(forbidby);
					forbidby = NULL;
				}
				if (forbidreason && *forbidreason == 0) {
					free(forbidreason);
					forbidreason = NULL;
				}
			} else {
				forbidby = NULL;
				forbidreason = NULL;
			}

			if (na->status & epona_NS_VERBOTEN) {
				if (na->last_usermask)
				free(na->last_usermask);
				if (na->last_realname)
				free(na->last_realname);
				
				na->last_usermask = forbidby;
				na->last_realname = forbidreason;
			} else {
				if (!na->last_usermask) 
				na->last_usermask = strdup("");
				if (!na->last_realname) 
				na->last_realname = strdup("");
			}

			/* Store the reference for later resolving */
			epona_SAFE(epona_read_string((char **)&na->nc, f));
			epona_SAFE(epona_read_int16(&tmp16, f)); /* Was linkcount */
		
			if (na->nc) {
		    		epona_SAFE(epona_read_int16(&tmp16, f));	/* Was channelcount */
				Nick_Stored_t *nick = Convert::epona_CreateNickEntry(na, NULL);
				if (nick != NULL)
				    Magick::instance().nickserv.AddStored(nick);
				epona_delnick(na);
			} else {

				/* This nick was a master nick, so it also has all the
				 * core info! =) 
				 */
				nc = (epona_NickCore *) calloc(1, sizeof(epona_NickCore));
				
				/* The initial display is what used to be the master nick */
				nc->display = strdup(na->nick);
				
				/* We grabbed info before; fill the appropriate fields now */
				if (*bufp)
				nc->pass = strdup(bufp);
				else
				nc->pass = NULL;	/* Which may be the case for forbidden nicks .. */
				
				nc->email = email;
				nc->greet = greet;
				nc->icq = icq;
				nc->url = url;
				
				/* We check whether the e-mail is valid because it was not tested
		 	     * in older versions. 
		 	     */
				if (ver<=10 && nc->email) {
					free(nc->email);
					nc->email = NULL;
				}
				
		    	epona_SAFE(epona_read_int32(&nc->flags, f));
		    	if (!epona_NSAllowKillImmed)
				nc->flags &= ~epona_NI_KILL_IMMED;
				
				/* Status flags cleanup */
				if (na->status & epona_NS_OLD_ENCRYPTEDPW) {
					nc->flags |= epona_NI_ENCRYPTEDPW;
					na->status &= ~epona_NS_OLD_ENCRYPTEDPW;
				}
			
		    	epona_SAFE(epona_read_int16(&nc->accesscount, f));
		    	if (nc->accesscount) {
					char **access;
					access = (char **) calloc(sizeof(char *) * nc->accesscount, 1);
					nc->access = access;
					for (j = 0; j < nc->accesscount; j++, access++)
			    		epona_SAFE(epona_read_string(access, f));
		    	}
		    	
		    	epona_SAFE(epona_read_int16(&nc->memos.memocount, f));
		    	epona_SAFE(epona_read_int16(&nc->memos.memomax, f));
		    	if (nc->memos.memocount) {
					epona_Memo *memos;
					memos = (epona_Memo *) calloc(sizeof(epona_Memo) * nc->memos.memocount, 1);
					nc->memos.memos = memos;
					
					for (j = 0; j < nc->memos.memocount; j++, memos++) {
			    		epona_SAFE(epona_read_int32(&memos->number, f));
			    		epona_SAFE(epona_read_int16(&memos->flags, f));
			    		epona_SAFE(epona_read_int32(&tmp32, f));
			    		memos->time = tmp32;
			    		epona_SAFE(epona_read_buffer(memos->sender, f));
			    		epona_SAFE(epona_read_string(&memos->text, f));
					}
		    	}
		    	
		    	/* We read the channel count, but don't take care of it.
		    	   epona_load_cs_dbase will regenerate it correctly. */
		    	epona_SAFE(epona_read_int16(&tmp16, f));
		    	epona_SAFE(epona_read_int16(&nc->channelmax, f));
		    	if (ver == 5)
				nc->channelmax = epona_CSMaxReg;
		    
		    	epona_SAFE(epona_read_int16(&nc->language, f));
		    	
		    	if (ver >= 11 && ver < 13) {
		 			char *s;
		 			
		    		epona_SAFE(epona_read_int16(&tmp16, f));
		    		epona_SAFE(epona_read_int32(&tmp32, f));
		    		epona_SAFE(epona_read_int16(&tmp16, f));
		    		epona_SAFE(epona_read_string(&s, f));	
		    	}
		    	
		    	/* Set us as being a master nick; fill the nc field also.
		    	   The epona_NS_MASTER flag will not be cleared in this function. */
		    	na->status |= epona_NS_MASTER;

			Nick_Stored_t *nick = Convert::epona_CreateNickEntry(na, nc);
			if (nick != NULL)
			{
			    Magick::instance().nickserv.AddStored(nick);
			    if (nc->memos.memocount)
			    {
				MemoServ::nick_memo_t memo = Convert::epona_CreateMemoEntry(&nc->memos, na->nick);
				if (memo.size())
				    Magick::instance().memoserv.AddNick(memo);
			    }
			}
			epona_delnick(na);
			epona_delcore(nc);
		    }
	    } /* while (epona_getc_db(f) != 0) */
	} /* for (i) */

    epona_close_db(f);
}

void epona_load_nick()
{
    epona_dbFILE *f;
    int ver, i, j, c;
    epona_NickAlias *na;
    epona_NickCore *nc;
    int failed = 0;
    int16 tmp16;
    int32 tmp32;
    char *s;

    map<mstring, epona_NickCore *> corelist;
    map<mstring, epona_NickAlias *> aliaslist;

    if (!(f = epona_open_db("NickServ", epona_NickDBName, "r", epona_NICK_VERSION)))
	return;

	ver = epona_get_file_version(f);
	
	if (ver <= 11) {
		epona_close_db(f);
		epona_load_old_nick();
		return;
	}
	
	/* First we load nick cores */
	for (i = 0; i < 1024 && !failed; i++) {
	    while ((c = epona_getc_db(f)) == 1) {
			if (c != 1)
		    SLOG(LM_EMERGENCY, "Invalid format in %s",( epona_NickDBName));
		
			nc = (epona_NickCore *) calloc(1, sizeof(epona_NickCore));

			epona_SAFE(epona_read_string(&nc->display, f));
			epona_SAFE(epona_read_string(&nc->pass, f));
			
			epona_SAFE(epona_read_string(&nc->email, f));
			epona_SAFE(epona_read_string(&nc->greet, f));
			epona_SAFE(epona_read_int32(&nc->icq, f));
			epona_SAFE(epona_read_string(&nc->url, f));

			epona_SAFE(epona_read_int32(&nc->flags, f));
		    if (!epona_NSAllowKillImmed)
			nc->flags &= ~epona_NI_KILL_IMMED;
#ifdef GETPASS
			if (nc->flags & epona_NI_ENCRYPTEDPW) {
		    	/* Bail: it makes no sense to continue with encrypted
		    	 * passwords, since we won't be able to verify them */
		    	SLOG(LM_EMERGENCY, "%s: load database: password for %s encrypted but encryption disabled, aborting",( "NickServ", nc->display));
			}
#elsif defined(JP2CRYPT) || defined(DESCRYPT) || defined(MD5CRYPT)
			if (nc->flags & epona_NI_ENCRYPTEDPW) {
				SLOG(LM_EMERGENCY, "%s: load database: password for %s encrypted "
		          "but not with the same encryption scheme, aborting",(
		          "NickServ", na->nick));
			}
#endif
			epona_SAFE(epona_read_int16(&nc->language, f));
			
		    epona_SAFE(epona_read_int16(&nc->accesscount, f));
		    if (nc->accesscount) {
				char **access;
				access = (char **) calloc(sizeof(char *) * nc->accesscount, 1);
				nc->access = access;
				for (j = 0; j < nc->accesscount; j++, access++)
			    	epona_SAFE(epona_read_string(access, f));
		    }
		    
		    epona_SAFE(epona_read_int16(&nc->memos.memocount, f));
		    epona_SAFE(epona_read_int16(&nc->memos.memomax, f));
		    if (nc->memos.memocount) {
				epona_Memo *memos;
				memos = (epona_Memo *) calloc(sizeof(epona_Memo) * nc->memos.memocount, 1);
				nc->memos.memos = memos;
				for (j = 0; j < nc->memos.memocount; j++, memos++) {
			    	epona_SAFE(epona_read_int32(&memos->number, f));
			    	epona_SAFE(epona_read_int16(&memos->flags, f));
			    	epona_SAFE(epona_read_int32(&tmp32, f));
			    	memos->time = tmp32;
			    	epona_SAFE(epona_read_buffer(memos->sender, f));
			    	epona_SAFE(epona_read_string(&memos->text, f));
				}
		    }
		    
		    epona_SAFE(epona_read_int16(&nc->channelcount, f));
		    epona_SAFE(epona_read_int16(&tmp16, f));
			nc->channelmax = epona_CSMaxReg;
		    
		    if (ver < 13) {
		    	/* Used to be dead authentication system */
		    	epona_SAFE(epona_read_int16(&tmp16, f));
		    	epona_SAFE(epona_read_int32(&tmp32, f));
		    	epona_SAFE(epona_read_int16(&tmp16, f));
		    	epona_SAFE(epona_read_string(&s, f));	
			}
		corelist[mstring(nc->display)] = nc;
			
	    } /* while (epona_getc_db(f) != 0) */
	} /* for (i) */

	for (i = 0; i < 1024 && !failed; i++) {
	    while ((c = epona_getc_db(f)) == 1) {
			if (c != 1)
		    SLOG(LM_EMERGENCY, "Invalid format in %s",( epona_NickDBName));
		
			na = (epona_NickAlias *) calloc(1, sizeof(epona_NickAlias));
			
			epona_SAFE(epona_read_string(&na->nick, f));

			epona_SAFE(epona_read_string(&na->last_usermask, f));
			epona_SAFE(epona_read_string(&na->last_realname, f));
			epona_SAFE(epona_read_string(&na->last_quit, f));
		
			epona_SAFE(epona_read_int32(&tmp32, f));
			na->time_registered = tmp32;
			epona_SAFE(epona_read_int32(&tmp32, f));
			na->last_seen = tmp32;
			epona_SAFE(epona_read_int16(&na->status, f));
			na->status &= ~epona_NS_TEMPORARY;

			epona_SAFE(epona_read_string(&na->nc, f));

		    if (!(na->status & epona_NS_VERBOTEN)) {
			if (!na->last_usermask)
		    	na->last_usermask = strdup("");
				if (!na->last_realname)
		    	na->last_realname = strdup("");
			aliaslist[mstring(na->nick)] = na;
		    }
		    else
		    {
			Nick_Stored_t *nick = Convert::epona_CreateNickEntry(na, NULL);
			if (nick != NULL)
			    Magick::instance().nickserv.AddStored(nick);
			epona_delnick(na);
		    }
		} /* while (epona_getc_db(f) != 0) */
	} /* for (i) */

	map<mstring, epona_NickAlias *>::iterator nai;
	map<mstring, epona_NickCore *>::iterator nci;

	// First, add all core nicknames with the same display name as their alias
	for (nai = aliaslist.begin(); nai != aliaslist.end(); nai++)
	{
	    if (strcasecmp(nai->second->nick, nai->second->nc)==0)
	    {
		nci = corelist.find(nai->second->nc);
		Nick_Stored_t *nick = Convert::epona_CreateNickEntry(nai->second, nci->second);
		if (nick != NULL)
		{
		    Magick::instance().nickserv.AddStored(nick);
		    if (nci->second->memos.memocount)
		    {
			MemoServ::nick_memo_t memo = Convert::epona_CreateMemoEntry(&nci->second->memos, nai->second->nick);
			if (memo.size())
			    Magick::instance().memoserv.AddNick(memo);
		    }
		}
	    }
	}

	// Now we have either pure aliases, or hosts with a different name to the alias.
	// For hosts with a different name, we change the 'name' to the first matching alias.
	for (nai = aliaslist.begin(); nai != aliaslist.end(); nai++)
	{
	    if (strcasecmp(nai->second->nick, nai->second->nc)!=0)
	    {
		nci = corelist.find(nai->second->nc);
		if (Magick::instance().nickserv.IsStored(nci->second->display))
		{
		    free(nai->second->nc);
		    nai->second->nc = strdup(nci->second->display);
		    Nick_Stored_t *nick = Convert::epona_CreateNickEntry(nai->second, NULL);
		    if (nick != NULL)
			Magick::instance().nickserv.AddStored(nick);
		}
		else
		{
		    free(nci->second->display);
		    nci->second->display = strdup(nai->second->nick);
		    Nick_Stored_t *nick = Convert::epona_CreateNickEntry(nai->second, nci->second);
		    if (nick != NULL)
		    {
			Magick::instance().nickserv.AddStored(nick);
			if (nci->second->memos.memocount)
			{
			    MemoServ::nick_memo_t memo = Convert::epona_CreateMemoEntry(&nci->second->memos, nai->second->nick);
			    if (memo.size())
				Magick::instance().memoserv.AddNick(memo);
			}
		    }
		}
	    }
	}

    for (nci=corelist.begin(); nci!=corelist.end(); nci++)
	epona_delcore(nci->second);

    for (nai=aliaslist.begin(); nai!=aliaslist.end(); nai++)
	epona_delnick(nai->second);

    epona_close_db(f);
}

#undef epona_SAFE

int epona_delnick(epona_NickAlias *na)
{
	free(na->nick);
    if (na->last_usermask)
	free(na->last_usermask);
    if (na->last_realname)
	free(na->last_realname);
	if (na->last_quit)
	free(na->last_quit);
    if (na->nc)
	free(na->nc);
    
    free(na);
    
    return 1;
}

static int epona_delcore(epona_NickCore *nc)
{
	int i;
	
	/* Now we can safely free it. */
	free(nc->display);
	if (nc->pass)
	free(nc->pass);
	
	if (nc->email)
	free(nc->email);
	if (nc->greet)
	free(nc->greet);
	if (nc->url)
	free(nc->url);
	
    if (nc->access) {
		for (i = 0; i < nc->accesscount; i++) {
	    	if (nc->access[i])
			free(nc->access[i]);
		}
		free(nc->access);
	}

    if (nc->memos.memos) {
		for (i = 0; i < nc->memos.memocount; i++) {
	    	if (nc->memos.memos[i].text)
			free(nc->memos.memos[i].text);
		}
		free(nc->memos.memos);
	}
		
	free(nc);
	
	return 1;
}

#define epona_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( epona_ChanDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void epona_load_chan()
{
    epona_dbFILE *f;
    int ver, i, j, c;
    epona_ChanInfo *ci;
    int failed = 0;

    if (!(f = epona_open_db("ChanServ", epona_ChanDBName, "r", epona_CHAN_VERSION)))
	return;

    ver = epona_get_file_version(f);

	for (i = 0; i < 256 && !failed; i++) {
	    int16 tmp16;
	    int32 tmp32;
	    int n_levels;
	    char *s;

	    while ((c = epona_getc_db(f)) != 0) {
		if (c != 1)
		    SLOG(LM_EMERGENCY, "Invalid format in %s",( epona_ChanDBName));
		ci = (epona_ChanInfo *) calloc(sizeof(epona_ChanInfo), 1);

		epona_SAFE(epona_read_buffer(ci->name, f));
		epona_SAFE(epona_read_string(&ci->founder, f));
		if (ver >= 7) {
		    epona_SAFE(epona_read_string(&ci->successor, f));
		} else {
		    ci->successor = NULL;
		}
		epona_SAFE(epona_read_buffer(ci->founderpass, f));
		epona_SAFE(epona_read_string(&ci->desc, f));
		if (!ci->desc)
		ci->desc = strdup("");
		epona_SAFE(epona_read_string(&ci->url, f));
		epona_SAFE(epona_read_string(&ci->email, f));
		epona_SAFE(epona_read_int32(&tmp32, f));
		ci->time_registered = tmp32;
		epona_SAFE(epona_read_int32(&tmp32, f));
		ci->last_used = tmp32;
		epona_SAFE(epona_read_string(&ci->last_topic, f));
		epona_SAFE(epona_read_buffer(ci->last_topic_setter, f));
		epona_SAFE(epona_read_int32(&tmp32, f));
		ci->last_topic_time = tmp32;
		epona_SAFE(epona_read_int32(&ci->flags, f));
#ifdef GETPASS
		if (ci->flags & epona_CI_ENCRYPTEDPW) {
		    /* Bail: it makes no sense to continue with encrypted
		     * passwords, since we won't be able to verify them */
		    SLOG(LM_EMERGENCY, "%s: load database: password for %s encrypted "
		          "but encryption disabled, aborting",(
		          "ChanServ", ci->name));
		}
#elsif defined(JP2CRYPT) || defined(DESCRYPT) || defined(MD5CRYPT)
			if (ci->flags & epona_CI_ENCRYPTEDPW) {
				SLOG(LM_EMERGENCY, "%s: load database: password for %s encrypted "
		          "but not with the same encryption scheme, aborting",(
		          "ChanServ", ci->name));
			}
#endif
		/* Leaveops cleanup */
		if (ver <= 13 && (ci->flags & 0x00000020))
			ci->flags &= ~0x00000020;
		/* Temporary flags cleanup */
		ci->flags &= ~epona_CI_INHABIT;
		
		if (ver >= 9) {
			epona_SAFE(epona_read_string(&ci->forbidby, f));
			epona_SAFE(epona_read_string(&ci->forbidreason, f));
		} else {
			ci->forbidreason = NULL;
			ci->forbidby = NULL;
		}
		if (ver >= 9) epona_SAFE(epona_read_int16(&tmp16, f));
		else tmp16 = epona_CSDefBantype;
		ci->bantype = tmp16;
		epona_SAFE(epona_read_int16(&tmp16, f));
		n_levels = tmp16;
		ci->levels = (short *) calloc(2*epona_CA_SIZE, 1);
		epona_reset_levels(ci);
		for (j = 0; j < n_levels; j++) {
		    if (j < epona_CA_SIZE)
			epona_SAFE(epona_read_int16(&ci->levels[j], f));
		    else
			epona_SAFE(epona_read_int16(&tmp16, f));
		}
		/* To avoid levels list silly hacks */
		if (ver < 10) 
			ci->levels[epona_CA_OPDEOPME] = ci->levels[epona_CA_OPDEOP];
		if (ver < 11) { 
			ci->levels[epona_CA_KICKME] = ci->levels[epona_CA_OPDEOP];
			ci->levels[epona_CA_KICK] = ci->levels[epona_CA_OPDEOP];
		}
		if (ver < 15) {
			ci->levels[epona_CA_BANME] = ci->levels[epona_CA_OPDEOP];
			ci->levels[epona_CA_BAN] = ci->levels[epona_CA_OPDEOP];
			ci->levels[epona_CA_TOPIC] = epona_ACCESS_INVALID;
#if 0
			/* Old Ultimate levels import */
			if (n_levels == 35) {
				ci->levels[epona_CA_AUTOPROTECT] = ci->levels[32];
				ci->levels[epona_CA_PROTECTME] = ci->levels[33];
				ci->levels[epona_CA_PROTECT] = ci->levels[34];
			}
#endif
		}

		epona_SAFE(epona_read_int16(&ci->accesscount, f));
		if (ci->accesscount) {
		    ci->access = (epona_ChanAccess *) calloc(ci->accesscount, sizeof(epona_ChanAccess));
		    for (j = 0; j < ci->accesscount; j++) {
			epona_SAFE(epona_read_int16(&ci->access[j].in_use, f));
			if (ci->access[j].in_use) {
			    epona_SAFE(epona_read_int16(&ci->access[j].level, f));
			    epona_SAFE(epona_read_string(&ci->access[j].nc, f));
			    if (ci->access[j].nc == NULL)
				ci->access[j].in_use = 0;
				if (ver >= 11) {
					 epona_SAFE(epona_read_int32(&tmp32, f));
					 ci->access[j].last_seen = tmp32;
				} else {
					ci->access[j].last_seen = 0;	/* Means we have never seen the user */
				}
			}
		    }
		} else {
		    ci->access = NULL;
		}

		epona_SAFE(epona_read_int16(&ci->akickcount, f));
		if (ci->akickcount) {
		    ci->akick = (epona_AutoKick *) calloc(ci->akickcount, sizeof(epona_AutoKick));
		    for (j = 0; j < ci->akickcount; j++) {
		    	if (ver >= 15) {
		    		epona_SAFE(epona_read_int16(&ci->akick[j].flags, f));
		    	} else {
					epona_SAFE(epona_read_int16(&tmp16, f));
					if (tmp16)
						ci->akick[j].flags |= epona_AK_USED;
			}
				if (ci->akick[j].flags & epona_AK_USED) {
					if (ver < 15) {
			    		epona_SAFE(epona_read_int16(&tmp16, f));
			    		if (tmp16)
			    			ci->akick[j].flags |= epona_AK_ISNICK;
			    	}
			    	epona_SAFE(epona_read_string(&s, f));
			    	if (ci->akick[j].flags & epona_AK_ISNICK) {
						ci->akick[j].u.nc = s;
						}
						if (!ci->akick[j].u.nc)
				    		ci->akick[j].flags &= ~epona_AK_USED;
						free(s);
			    	} else {
						ci->akick[j].u.mask = s;
			    	}
			    	epona_SAFE(epona_read_string(&s, f));
			    	if (ci->akick[j].flags & epona_AK_USED)
						ci->akick[j].reason = s;
			    	else if (s) 
			    		free(s);
					if (ver>=9) {
						epona_SAFE(epona_read_string(&s, f));
						if (ci->akick[j].flags & epona_AK_USED) {
							ci->akick[j].creator = s;
			    			} else if (s) { 
			    				free(s);
			    			}
			    			epona_SAFE(epona_read_int32(&tmp32, f));
			    			if (ci->akick[j].flags & epona_AK_USED)
							ci->akick[j].addtime = tmp32;
					} else {
						ci->akick[j].creator = NULL;
						ci->akick[j].addtime = 0;
					}
				
				/* Bugfix */
				if ((ver == 15) && ci->akick[j].flags > 8) {
					ci->akick[j].flags = 0;
					ci->akick[j].u.nc = NULL;
					ci->akick[j].u.nc = NULL;
					ci->akick[j].addtime = 0;
					ci->akick[j].creator = NULL;
					ci->akick[j].reason = NULL;
				}
		    }
		} else {
		    ci->akick = NULL;
		}

		if (ver >= 10) { 
			epona_SAFE(epona_read_int32(&ci->mlock_on, f));
			epona_SAFE(epona_read_int32(&ci->mlock_off, f));
		} else {
			epona_SAFE(epona_read_int16(&tmp16, f));
			ci->mlock_on = tmp16;
			epona_SAFE(epona_read_int16(&tmp16, f));
			ci->mlock_off = tmp16;
		}
		epona_SAFE(epona_read_int32(&ci->mlock_limit, f));
		epona_SAFE(epona_read_string(&ci->mlock_key, f));
		if (ver >= 10) {
			epona_SAFE(epona_read_string(&ci->mlock_flood, f));
			epona_SAFE(epona_read_string(&ci->mlock_redirect, f));
		}

		epona_SAFE(epona_read_int16(&ci->memos.memocount, f));
		epona_SAFE(epona_read_int16(&ci->memos.memomax, f));
		if (ci->memos.memocount) {
		    epona_Memo *memos;
		    memos = (epona_Memo *) calloc(sizeof(epona_Memo) * ci->memos.memocount, 1);
		    ci->memos.memos = memos;
		    for (j = 0; j < ci->memos.memocount; j++, memos++) {
			epona_SAFE(epona_read_int32(&memos->number, f));
			epona_SAFE(epona_read_int16(&memos->flags, f));
			epona_SAFE(epona_read_int32(&tmp32, f));
			memos->time = tmp32;
			epona_SAFE(epona_read_buffer(memos->sender, f));
			epona_SAFE(epona_read_string(&memos->text, f));
		    }
		}

		epona_SAFE(epona_read_string(&ci->entry_message, f));

		ci->c = NULL;
		
		/* Some cleanup */
		if (ver <= 11) {
			/* Cleanup: Founder must be != than successor */
			if (!(ci->flags & epona_CI_VERBOTEN) && ci->successor == ci->founder) {
				SLOG(LM_ERROR, "Warning: founder and successor of %s are equal. Cleaning up.",( ci->name));
				ci->successor = NULL;	
			}
		}
		
		/* BotServ options */
		
		if (ver >= 8)
		{
			int n_ttb;
			
			epona_SAFE(epona_read_string(&s, f));
			if (s)
				free(s);
			
			epona_SAFE(epona_read_int32(&tmp32, f));
			ci->botflags = tmp32;			
			epona_SAFE(epona_read_int16(&tmp16, f));
			n_ttb = tmp16;
			ci->ttb = (short *) calloc(2*epona_TTB_SIZE, 1);
			for (j = 0; j < n_ttb; j++) {
		    		if (j < epona_TTB_SIZE)
					epona_SAFE(epona_read_int16(&ci->ttb[j], f));
		    		else
					epona_SAFE(epona_read_int16(&tmp16, f));
			}
			for (j = n_ttb; j < epona_TTB_SIZE; j++)
				ci->ttb[j] = 0;
			epona_SAFE(epona_read_int16(&tmp16, f));
			ci->capsmin = tmp16;
			epona_SAFE(epona_read_int16(&tmp16, f));
			ci->capspercent = tmp16;
			epona_SAFE(epona_read_int16(&tmp16, f));
			ci->floodlines = tmp16;
			epona_SAFE(epona_read_int16(&tmp16, f));
			ci->floodsecs = tmp16;
			epona_SAFE(epona_read_int16(&tmp16, f));
			ci->repeattimes = tmp16;
			
			epona_SAFE(epona_read_int16(&ci->bwcount, f));
			if (ci->bwcount) {
		    		ci->badwords = (epona_BadWord *) calloc(ci->bwcount, sizeof(epona_BadWord));
		    		for (j = 0; j < ci->bwcount; j++) {
					epona_SAFE(epona_read_int16(&ci->badwords[j].in_use, f));
					if (ci->badwords[j].in_use) {
			    			epona_SAFE(epona_read_string(&ci->badwords[j].word, f));
			    			epona_SAFE(epona_read_int16(&ci->badwords[j].type, f));
					}
		    		}
			} else {
		    		ci->badwords = NULL;
			}
		} else {
			ci->botflags = 0;
			ci->ttb = (short *) calloc(2*epona_TTB_SIZE, 1);
			for (j = 0; j < epona_TTB_SIZE; j++)
				ci->ttb[j] = 0;
			ci->bwcount = 0;
			ci->badwords = NULL;
		}
		
	    Chan_Stored_t *chan = Convert::epona_CreateChanEntry(ci);
	    if (chan != NULL)
	    {
		Magick::instance().chanserv.AddStored(chan);
		if (ci->memos.memocount)
		{
		    MemoServ::channel_news_t news = Convert::epona_CreateNewsEntry(&ci->memos, ci->name);
		    if (news.size())
			Magick::instance().memoserv.AddChannel(news);
		}
	    }
	    epona_delchan(ci);

	    } /* while (epona_getc_db(f) != 0) */
	} /* for (i) */

    epona_close_db(f);

}

#undef epona_SAFE

static int epona_delchan(epona_ChanInfo *ci)
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
	if (ci->mlock_flood)
	free(ci->mlock_flood);
	if (ci->mlock_redirect)
	free(ci->mlock_redirect);
    if (ci->last_topic)
	free(ci->last_topic);
	if (ci->forbidby)
	free(ci->forbidby);
	if (ci->forbidreason)
	free(ci->forbidreason);
    if (ci->access)
	free(ci->access);
    for (i = 0; i < ci->akickcount; i++) {
	if (ci->akick[i].flags & epona_AK_ISNICK)
	    if (ci->akick[i].u.nc)
		free(ci->akick[i].u.nc);
	else
	    if (ci->akick[i].u.mask)
		free(ci->akick[i].u.mask);
	if (ci->akick[i].reason)
	    free(ci->akick[i].reason);
	if (ci->akick[i].creator)
		free(ci->akick[i].creator);
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
    if (ci->ttb)
        free(ci->ttb);
    for (i = 0; i < ci->bwcount; i++) {
    	if (ci->badwords[i].word)
   	    free(ci->badwords[i].word);	
    }
    if (ci->badwords)
        free(ci->badwords);
    free(ci);
    return 1;
}

/*************************************************************************/

/* Reset channel access level values to their default state. */

static void epona_reset_levels(epona_ChanInfo *ci)
{
    int i;

    if (ci->levels)
	free(ci->levels);
    ci->levels = (short *) calloc(epona_CA_SIZE * sizeof(*ci->levels), 1);
    for (i = 0; epona_def_levels[i][0] >= 0; i++)
	ci->levels[epona_def_levels[i][0]] = epona_def_levels[i][1];
}


#define epona_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( epona_AutokillDBName));	\
	break;						\
    }							\
} while (0)

static void epona_load_old_akill()
{
    epona_dbFILE *f;
    int j;
    int16 tmp16;
    int32 tmp32;
    char buf[epona_NICKMAX], *mask;
    epona_Akill *ak;

    if (!(f = epona_open_db("AKILL", epona_AutokillDBName ? epona_AutokillDBName : "akill.db", "r", 9)))
	return;

    epona_get_file_version(f);

    epona_read_int16(&tmp16, f);

	for (j = 0; j < tmp16; j++) {
		ak = (epona_Akill *) calloc(sizeof(epona_Akill), 1);
		
	    epona_SAFE(epona_read_string(&mask, f));
	    epona_SAFE(epona_read_string(&ak->reason, f));
	    epona_SAFE(epona_read_buffer(buf, f));
	    if (!*buf) ak->by = NULL;
	    else ak->by = strdup(buf);
	    epona_SAFE(epona_read_int32(&tmp32, f));
	    ak->seton = tmp32 ? tmp32 : time(NULL);
	    epona_SAFE(epona_read_int32(&tmp32, f));
	    ak->expires = tmp32;
	    
	    /* Sanity checks *sigh* */
	    
	    if (ak->expires == 0)
	    {
		Magick::instance().operserv.Akill_insert(mstring(mask).After("!"), Magick::instance().operserv.Expire_Sop(),
			mstring(ak->reason), ak->by ? mstring(ak->by) : Magick::instance().operserv.FirstName(), mDateTime(ak->seton));
	    }
	    else
	    {
		Magick::instance().operserv.Akill_insert(mstring(mask).After("!"), ak->expires-ak->seton,
			mstring(ak->reason), ak->by ? mstring(ak->by) : Magick::instance().operserv.FirstName(), mDateTime(ak->seton));
	    }

	    free(mask);
	    free(ak->reason);
	    free(ak->by);
	    free(ak);
	}
	
    epona_close_db(f);
}

#undef epona_SAFE

/* Load OperServ data. */

#define epona_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( epona_OperDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void epona_load_oper()
{
    epona_dbFILE *f;
    int16 i, n, ver, c;
    int16 tmp16;
    int32 tmp32;
    char *s;
    int failed = 0;

    if (!(f = epona_open_db("OperServ", epona_OperDBName, "r", epona_OPER_VERSION)))
	return;
    
	ver = epona_get_file_version(f);
      
   	if (ver <= 9) {
		epona_SAFE(epona_read_int16(&n, f));
		for (i = 0; i < n && !failed; i++) {
	    	epona_SAFE(epona_read_string(&s, f));

		    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
			  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
			Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());
		}

		if (!failed)
	    	epona_SAFE(epona_read_int16(&n, f));
		for (i = 0; i < n && !failed; i++) {
	    	epona_SAFE(epona_read_string(&s, f));
		    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
			 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)) &&
			!(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
			  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->find(s)))
			Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());
		}
	}
		
	if (ver >= 7) {
	    int32 tmp32;
	    epona_SAFE(epona_read_int32(&tmp32, f));
	    epona_SAFE(epona_read_int32(&tmp32, f));
	}
	
	if (ver <= 10)
		epona_load_old_akill();
	else {
		epona_Akill *ak;
		
		epona_read_int16(&tmp16, f);
    	
    	for (i = 0; i < tmp16; i++)
    	{
    		ak = (epona_Akill *) calloc(sizeof(epona_Akill), 1);
    		
    		epona_SAFE(epona_read_string(&ak->user, f));
    		epona_SAFE(epona_read_string(&ak->host, f));
    		epona_SAFE(epona_read_string(&ak->by, f));
    		epona_SAFE(epona_read_string(&ak->reason, f));
    		epona_SAFE(epona_read_int32(&tmp32, f));
    		ak->seton = tmp32;
    		epona_SAFE(epona_read_int32(&tmp32, f));
    		ak->expires = tmp32;
    		
	    if (ak->expires == 0)
	    {
		Magick::instance().operserv.Akill_insert(mstring(ak->user) + "@" + mstring(ak->host), Magick::instance().operserv.Expire_Sop(),
			mstring(ak->reason), mstring(ak->by), mDateTime(ak->seton));
	    }
	    else
	    {
		Magick::instance().operserv.Akill_insert(mstring(ak->user) + "@" + mstring(ak->host), ak->expires-ak->seton,
			mstring(ak->reason), mstring(ak->by), mDateTime(ak->seton));
	    }

	    free(ak->user);
	    free(ak->host);
	    free(ak->reason);
	    free(ak->by);

		free(ak);
    	} 
	}
	
	if (ver >= 11) {
		epona_read_int16(&tmp16, f);
    	
    	for (i = 0; i < tmp16; i++)
    	{
    		epona_SAFE(epona_read_string(&s, f));
			free(s);
    		epona_SAFE(epona_read_string(&s, f));
			free(s);
    		epona_SAFE(epona_read_string(&s, f));
			free(s);
    		epona_SAFE(epona_read_int32(&tmp32, f));
    		epona_SAFE(epona_read_int32(&tmp32, f));
    	}
    	
    	if (ver >= 13) {
    		epona_read_int16(&tmp16, f);
    	
    		for (i = 0; i < tmp16; i++) {
    		
    			epona_SAFE(epona_read_string(&s, f));
				free(s);
    			epona_SAFE(epona_read_string(&s, f));
				free(s);
    			epona_SAFE(epona_read_string(&s, f));
				free(s);
    			epona_SAFE(epona_read_int32(&tmp32, f));
    			epona_SAFE(epona_read_int32(&tmp32, f));
    		
    		}
    	}
    	
    	epona_read_int16(&tmp16, f);
    	
    	for (i = 0; i < tmp16; i++)
    	{
    		epona_SAFE(epona_read_string(&s, f));
			free(s);
    		epona_SAFE(epona_read_string(&s, f));
			free(s);
    		epona_SAFE(epona_read_string(&s, f));
			free(s);
    		epona_SAFE(epona_read_int32(&tmp32, f));
    		epona_SAFE(epona_read_int32(&tmp32, f));
    	}
	}
	
	if (ver >= 12) {
		for (i = 0; i < 1024 && !failed; i++) {
	    	while ((c = epona_getc_db(f)) != 0) {
	    		if (c != 1)
		    	SLOG(LM_EMERGENCY, "Invalid format in %s",( epona_NickDBName));
	    		
	    		epona_SAFE(epona_read_string(&s, f));
				free(s);
	    		epona_SAFE(epona_read_int16(&tmp16, f));
	    		epona_SAFE(epona_read_int32(&tmp32, f));
	    	} /* while (epona_getc_db(f) != 0) */
	    }	/* for (i) */
	}
	
    epona_close_db(f);
    
}

#undef epona_SAFE

#define epona_SAFE(x) do {                                    \
    if ((x) < 0) {                                      \
            SLOG(LM_EMERGENCY, "Read error on %s",( epona_ExceptionDBName)); \
        nexceptions = i;                                \
        break;                                          \
    }                                                   \
} while (0)

void epona_load_exception()
{
    epona_dbFILE *f;
    int i, nexceptions;
    int16 n;
    int16 tmp16;
    int32 tmp32;
    epona_Exception exception;

    if (!(f = epona_open_db("OperServ", epona_ExceptionDBName, "r", epona_EXCEPTION_VERSION)))
        return;
    switch (i = epona_get_file_version(f)) {
      case 9:
      case 8:
      case 7:
        epona_SAFE(epona_read_int16(&n, f));
        nexceptions = n;
        if (!nexceptions) {
            epona_close_db(f);
            return;
        }
        for (i = 0; i < nexceptions; i++) {
            epona_SAFE(epona_read_string(&exception.mask, f));
            epona_SAFE(epona_read_int16(&tmp16, f));
            exception.limit = tmp16;
	    epona_SAFE(epona_read_buffer(exception.who, f));
	    epona_SAFE(epona_read_string(&exception.reason, f));
            epona_SAFE(epona_read_int32(&tmp32, f));
            exception.time = tmp32;
            epona_SAFE(epona_read_int32(&tmp32, f));
            exception.expires = tmp32;

	    Magick::instance().operserv.Clone_insert(mstring(exception.mask).After("@"), exception.limit,
			mstring(exception.reason), mstring(exception.who), mDateTime(exception.time));

	    free(exception.mask);
	    free(exception.reason);

        }
        break;

      default:
        SLOG(LM_EMERGENCY, "Unsupported version (%d) on %s",( i, epona_ExceptionDBName));
    } /* switch (ver) */

    epona_close_db(f);
}

#undef epona_SAFE

#define epona_SAFE(x) do {					\
    if ((x) < 0) {					\
	    SLOG(LM_EMERGENCY, "Read error on %s",( epona_NewsDBName));	\
	nnews = i;					\
	break;						\
    }							\
} while (0)

void epona_load_news()
{
    epona_dbFILE *f;
    int i, nnews;
    int16 n;
    int32 tmp32;
    epona_NewsItem news;

    if (!(f = epona_open_db("OperServ", epona_NewsDBName, "r", epona_NEWS_VERSION)))
	return;
    switch (i = epona_get_file_version(f)) {
      case 9:
      case 8:
      case 7:
	epona_SAFE(epona_read_int16(&n, f));
	nnews = n;
	if (!nnews) {
	    epona_close_db(f);
	    return;
	}
	for (i = 0; i < nnews; i++) {
	    epona_SAFE(epona_read_int16(&news.type, f));
	    epona_SAFE(epona_read_int32(&news.num, f));
	    epona_SAFE(epona_read_string(&news.text, f));
	    epona_SAFE(epona_read_buffer(news.who, f));
	    epona_SAFE(epona_read_int32(&tmp32, f));
	    news.time = tmp32;

	    if (news.type == epona_NEWS_LOGON && Magick::instance().commserv.IsList(Magick::instance().commserv.ALL_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.ALL_Name())->
		    MSG_insert(mstring(news.text), mstring(news.who), mDateTime(news.time));
	    }
	    else if (news.type == epona_NEWS_OPER && Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->
		    MSG_insert(mstring(news.text), mstring(news.who), mDateTime(news.time));
	    }

	    free(news.text);
	}
	break;

      default:
	SLOG(LM_EMERGENCY, "Unsupported version (%d) on %s",( i, epona_NewsDBName));
    } /* switch (ver) */

    epona_close_db(f);
}

#undef epona_SAFE

Nick_Stored_t *Convert::epona_CreateNickEntry(epona_NickAlias * na, epona_NickCore * nc)
{
    BTCB();
    if (na == NULL || na->nick == NULL || !strlen(na->nick))
	return NULL;

    if (na->status & epona_NS_VERBOTEN)
    {
	Nick_Stored_t *out = new Nick_Stored_t(na->nick);

	return out;
    }
    else if (nc == NULL && na->nc && strlen(na->nc))
    {
	Nick_Stored_t tmp(na->nc);
	Nick_Stored_t *out = new Nick_Stored_t(na->nick, mDateTime(na->time_registered), tmp);

	if (out == NULL)
	    return NULL;
	if (na->last_realname != NULL && strlen(na->last_realname))
	    out->i_LastRealName = na->last_realname;
	if (na->last_quit != NULL && strlen(na->last_quit))
	    out->i_LastQuit = na->last_quit;
	if (na->last_usermask != NULL && strlen(na->last_usermask))
	    out->i_LastMask = na->last_usermask;
	out->i_LastSeenTime = mDateTime(na->last_seen);
	return out;
    }
    else if (nc != NULL)
    {
	int i;
	char **str;
	Nick_Stored_t *out = new Nick_Stored_t(na->nick, nc->pass);

	if (out == NULL)
	    return NULL;

	if (nc->flags & epona_NI_ENCRYPTEDPW)
	{
	    char pwbuf[33] = {0};
	    for (int i=0; i<16; i++)
		sprintf(&pwbuf[i*2], "%02x", nc->pass[i]);
	    out->i_Password = pwbuf;
	}


	if (na->last_realname != NULL && strlen(na->last_realname))
	    out->i_LastRealName = mstring(na->last_realname);
	if (na->last_usermask != NULL && strlen(na->last_usermask))
	    out->i_LastMask = mstring(na->last_usermask);
	if (na->last_quit != NULL && strlen(na->last_quit))
	    out->i_LastQuit = mstring(na->last_quit);
	out->i_RegTime = mDateTime(na->time_registered);
	out->i_LastSeenTime = mDateTime(na->last_seen);

	if (nc->email != NULL && strlen(nc->email))
	    out->i_Email = mstring(nc->email);
	if (nc->icq != 0)
	    out->i_ICQ = nc->icq;
	if (nc->url != NULL && strlen(nc->url))
	    out->i_URL = mstring(nc->url);
	if (out->i_URL.Contains("http://"))
	    out->i_URL.Remove("http://", false);
	if (out->i_URL.Contains("HTTP://"))
	    out->i_URL.Remove("HTTP://", false);

	for (i = 0, str = nc->access; i < nc->accesscount; ++i, ++str)
	{
	    out->i_access.insert(mstring(*str));
	}

	if (nc->flags & epona_NI_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (nc->flags & epona_NI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (nc->flags & epona_NI_MSG && !out->L_PRIVMSG())
	    out->setting.PRIVMSG = true;
	if (nc->flags & epona_NI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (na->status & epona_NS_NO_EXPIRE && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;

	if ((nc->flags & epona_NI_SERVICES_ADMIN) &&
	    Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	    !Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->find(na->nick))
	{
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(na->nick)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(na->nick),
									Magick::instance().commserv.FirstName());
	}
	else if ((nc->flags & epona_NI_SERVICES_OPER) &&
		 Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		 !Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->find(na->nick))
	{
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(na->nick)) &&
		!(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->find(na->nick)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->insert(mstring(na->nick),
									Magick::instance().commserv.FirstName());
	}

	switch (nc->language)
	{
	case epona_LANG_EN_US:
	    out->setting.Language = "ENGLISH";
	    break;
	case epona_LANG_JA_JIS:
	case epona_LANG_JA_EUC:
	case epona_LANG_JA_SJIS:
	    out->setting.Language = "JAPANESE";
	    break;
	case epona_LANG_ES:
	    out->setting.Language = "SPANISH";
	    break;
	case epona_LANG_PT:
		    out->setting.Language = "PORTUGUESE";
	    break;
	case epona_LANG_FR:
	    out->setting.Language = "FRENCH";
	    break;
	case epona_LANG_TR:
	    out->setting.Language = "TURKISH";
	    break;
	case epona_LANG_IT:
	    out->setting.Language = "ITALIAN";
	    break;
	case epona_LANG_DE:
	    out->setting.Language = "GERMAN";
	    break;
	case epona_LANG_CAT:
	    out->setting.Language = "CATALAN";
	    break;
	case epona_LANG_GR:
	    out->setting.Language = "GREEK";
	    break;
	}

	return out;
    }
    else
    {
	return NULL;
    }
    ETCB();
}

static mstring epona_getmodes(int16 modes)
{
    BTCB();
    mstring retval;

    if (modes & epona_CMODE_i)
	retval += "i";
    if (modes & epona_CMODE_m)
	retval += "m";
    if (modes & epona_CMODE_n)
	retval += "n";
    if (modes & epona_CMODE_p)
	retval += "p";
    if (modes & epona_CMODE_s)
	retval += "s";
    if (modes & epona_CMODE_t)
	retval += "t";
    if (modes & epona_CMODE_k)
	retval += "k";
    if (modes & epona_CMODE_l)
	retval += "l";
    if (modes & epona_CMODE_R)
	retval += "R";
    if (modes & epona_CMODE_r)
	retval += "r";
#ifdef epona_IRC_BAHAMUT
    if (modes & epona_CMODE_c)
	retval += "c";
    if (modes & epona_CMODE_M)
	retval += "M";
    if (modes & epona_CMODE_O)
	retval += "O";
#endif
#ifdef epona_IRC_ULTIMATE
    if (modes & epona_CMODE_f)
	retval += "f";
    if (modes & epona_CMODE_x)
	retval += "x";
    if (modes & epona_CMODE_A)
	retval += "A";
    if (modes & epona_CMODE_I)
	retval += "I";
    if (modes & epona_CMODE_K)
	retval += "K";
    if (modes & epona_CMODE_L)
	retval += "L";
    if (modes & epona_CMODE_O)
	retval += "O";
    if (modes & epona_CMODE_S)
	retval += "S";
#endif
#ifdef epona_IRC_UNREAL
    if (modes & epona_CMODE_c)
	retval += "c";
    if (modes & epona_CMODE_A)
	retval += "A";
    if (modes & epona_CMODE_H)
	retval += "H";
    if (modes & epona_CMODE_K)
	retval += "K";
    if (modes & epona_CMODE_L)
	retval += "L";
    if (modes & epona_CMODE_O)
	retval += "O";
    if (modes & epona_CMODE_Q)
	retval += "Q";
    if (modes & epona_CMODE_S)
	retval += "S";
    if (modes & epona_CMODE_V)
	retval += "V";
    if (modes & epona_CMODE_f)
	retval += "f";
    if (modes & epona_CMODE_G)
	retval += "G";
    if (modes & epona_CMODE_C)
	retval += "C";
    if (modes & epona_CMODE_u)
	retval += "u";
    if (modes & epona_CMODE_z)
	retval += "z";
    if (modes & epona_CMODE_N)
	retval += "N";
#endif

    return retval;
    ETCB();
}

Chan_Stored_t *Convert::epona_CreateChanEntry(epona_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name == NULL || !strlen(ci->name))
	return NULL;

    if (ci->flags & epona_CI_VERBOTEN)
    {
	Chan_Stored_t *out = new Chan_Stored_t(mstring(ci->name));

	return out;
    }
    else
    {
	epona_ChanAccess *i_access;
	epona_AutoKick *akick;
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

	if (ci->flags & epona_CI_ENCRYPTEDPW)
	{
	    char pwbuf[33] = {0};
	    for (i=0; i<16; i++)
		sprintf(&pwbuf[i*2], "%02x", ci->founderpass[i]);
	    out->i_Password = pwbuf;
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

	long newlevel;
	float mod = (float) Magick::instance().chanserv.Level_Max() / (float) epona_ACCESS_FOUNDER;

	for (i = 0, i_access = ci->access; i < ci->accesscount; ++i, ++i_access)
	{
	    if (i_access->nc == NULL)
		continue;
	    if (i_access->level < 0)
		newlevel = -1;
	    else
		newlevel = (long) ((float) i_access->level * mod);
	    if (newlevel == 0)
		newlevel = 1;
	    out->Access_insert(i_access->nc, newlevel, Magick::instance().chanserv.FirstName());
	}
	for (i = 0, akick = ci->akick; i < ci->akickcount; ++i, ++akick)
	{
	    if (!(akick->flags & epona_AK_USED))
		continue;
	    if ((akick->flags & epona_AK_ISNICK) ? (akick->u.nc == NULL || !strlen(akick->u.nc))
						 : (akick->u.mask == NULL || !strlen(akick->u.mask)))
		continue;

	    if (akick->reason != NULL && strlen(akick->reason))
		out->Akick_insert((akick->flags & epona_AK_ISNICK) ? akick->u.nc : akick->u.mask, akick->reason, akick->creator, mDateTime(akick->addtime));
	    else
		out->Akick_insert((akick->flags & epona_AK_ISNICK) ? akick->u.nc : akick->u.mask, akick->creator, mDateTime(akick->addtime));
	}

	if (ci->last_topic != NULL && strlen(ci->last_topic))
	    out->i_Topic = mstring(ci->last_topic);
	if (ci->last_topic_setter != NULL && strlen(ci->last_topic_setter))
	    out->i_Topic_Setter = mstring(ci->last_topic_setter);
	out->i_Topic_Set_Time = mDateTime(ci->last_topic_time);

	if (ci->entry_message != NULL && strlen(ci->entry_message))
	    out->Message_insert(ci->entry_message, Magick::instance().chanserv.FirstName());

	if (ci->flags & epona_CI_KEEPTOPIC && !out->L_Keeptopic())
	    out->setting.Keeptopic = true;
	if (ci->flags & epona_CI_SECUREOPS && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & epona_CI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & epona_CI_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & epona_CI_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & epona_CI_PEACE && !out->L_Revenge())
	    out->setting.Revenge = "REVERSE";
	if (ci->flags & epona_CI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & epona_CI_NO_EXPIRE && !Magick::instance().chanserv.LCK_NoExpire())
	    out->setting.NoExpire = true;

	mstring modelock;

	if (ci->mlock_on || ci->mlock_key != NULL || ci->mlock_limit)
	{
	    mstring modes = epona_getmodes(ci->mlock_on);

	    modes.Remove("k");
	    modes.Remove("l");
	    modelock << "+" << modes << (ci->mlock_key != NULL ? "k" : "") << (ci->mlock_limit ? "l" : "");
	}
	if (ci->mlock_off)
	{
	    modelock << "-" << epona_getmodes(ci->mlock_off);
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

	if (ci->levels != NULL)
	{
	    for (i = 0; i < epona_CA_SIZE; ++i)
	    {
		if (ci->levels[i] == epona_ACCESS_INVALID)
		    newlevel = Magick::instance().chanserv.Level_Max() + 2;
		else if (ci->levels[i] == epona_ACCESS_FOUNDER)
		    newlevel = Magick::instance().chanserv.Level_Max() + 1;
		else if (ci->levels[i] < 0)
		    newlevel = -1;
		else
		    newlevel = (long) ((float) ci->levels[i] * mod);

		switch (i)
		{
		case epona_CA_INVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_UNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_OPDEOP:
		case epona_CA_OPDEOPME:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_ACCESS_LIST:
		    out->Level_change("VIEW", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_CLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_ACCESS_CHANGE:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_MEMO:
		    out->Level_change("WRITEMEMO", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_GREET:
		    out->Level_change("GREET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_VOICEME:
		case epona_CA_VOICE:
		    out->Level_change("CMDVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_GETKEY:
		    out->Level_change("CMDMODE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_AUTOHALFOP:
		    out->Level_change("AUTOHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_AUTOPROTECT:
		    break;
		case epona_CA_HALFOPME:
		case epona_CA_HALFOP:
		    out->Level_change("CMDHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_KICKME:
		case epona_CA_KICK:
		    out->Level_change("CMDKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_BANME:
		case epona_CA_BAN:
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case epona_CA_INFO:
		    out->Level_change("VIEW", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}

	return out;
    }
    ETCB();
}

MemoServ::nick_memo_t Convert::epona_CreateMemoEntry(epona_MemoInfo * ml, const char *nick)
{
    BTCB();
    int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;
    epona_Memo *memos;

    memos = ml->memos;
    for (i = 0; i < ml->memocount; ++i, ++memos)
    {
	if (memos->text == NULL)
	    continue;

	tmp = new Memo_t(mstring(nick), mstring(memos->sender), mstring(memos->text));
	tmp->i_Time = mDateTime(memos->time);
	if (!(memos->flags & epona_MF_UNREAD))
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}

MemoServ::channel_news_t Convert::epona_CreateNewsEntry(epona_MemoInfo * nl, const char *chan)
{
    BTCB();
    int i;

    MemoServ::channel_news_t out;
    News_t *tmp;
    epona_Memo *memos;

    memos = nl->memos;
    for (i = 0; i < nl->memocount; ++i, ++memos)
    {
	if (memos->text == NULL)
	    continue;

	tmp = new News_t(mstring(chan), mstring(memos->sender), mstring(memos->text));
	tmp->i_Time = mDateTime(memos->time);
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}


#endif /* CONVERT */
