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
#define RCSID(x,y) const char *rcsid_convert_auspice_cpp_ ## x () { return y; }
RCSID(convert_auspice_cpp, "@(#)$Id$");

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

/* Based upon the DB loading routines from Auspice 2.8
 * (c) 2000-2001 In Mean <auspice@auspice.org>
 */

#include "magick.h"
#include "convert/interface.h"
#include "convert/auspice.h"

#define auspice_CHANMAX  64
#define auspice_NICKMAX  32
#define auspice_PASSMAX  32
#define auspice_MODEMAX  64

#define NICKSERV_DB	"nick.db"
#define CHANSERV_DB	"chan.db"
#define MEMOSERV_DB	"memo.db"
#define AKILL_DB	"akill.db"
#define IGNORE_DB       "ignore.db"
#define TRIGGER_DB	"trigger.db"
#define MSG_DB		"msg.db"
#define ADMINSERV_DB	"admin.db"

/*************************************************************************/

/* Nickname info structure.  Each nick structure is stored in one of 256
 * lists; the list is determined by the first character of the nick.  Nicks
 * are stored in alphabetical order within lists. */

struct auspice_NickInfo_V5 {
    auspice_NickInfo_V5 *next, *prev;
    char nick[auspice_NICKMAX];
    char pass[auspice_PASSMAX];
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;	/* # of entries */
    char **access;	/* Array of strings */
    long flags;		/* See below */
    time_t id_timestamp;/* TS8 timestamp of user who last ID'd for nick */
    unsigned short memomax;
    unsigned short channelcount;/* Number of channels currently registered */
    char *url;
    char *email;
    char *forward;
    char *hold;       /*  }                                       */
    char *mark;       /*  }   --   Identities (what svsadmin did it?)  */
    char *forbid;
    int news;
    char *uin;
    char *age;
    /* FIELDS information about the owner */
    char *info;
    char *sex;
    char *mlock;
};

struct auspice_NickInfo {
    auspice_NickInfo *next, *prev;
    char nick[auspice_NICKMAX];
    char pass[auspice_PASSMAX];
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;	/* # of entries */
    char **access;	/* Array of strings */
    long flags;		/* See below */
    time_t id_timestamp;/* TS8 timestamp of user who last ID'd for nick */
    unsigned short memomax;
    unsigned short channelcount;/* Number of channels currently registered */
    char *url;
    char *email;
    char *forward;
    char *hold;       /*  }                                       */
    char *mark;       /*  }   --   Identities (what svsadmin did it?)  */
    char *forbid;
    int news;
    char *uin;
    char *age;
    /* FIELDS information about the owner */
    char *info;
    char *sex;
    char *mlock;
    char *last_quit;  /* Last Quit Message */

    /* Extra Flags */
    long eflags;

    /* Auto Join Field */
    long ajoincount;
    char **autojoin;
    /* User Comment */
    long comline;
    char **comment;
    /* User Note */
    long noteline;
    char **note;
};


#define auspice_NI_KILLPROTECT	0x00000001  /* Kill others who take this nick */
#define auspice_NI_SECURE	0x00000002  /* Don't recognize unless IDENTIFY'd */
#define auspice_NI_VERBOTEN	0x00000004  /* Nick may not be registered or used */
#define auspice_NI_ENCRYPTEDPW	0x00000008  /* Nickname password is encrypted */
#define auspice_NI_MEMO_SIGNON	0x00000010  /* Notify of memos at signon and un-away */
#define auspice_NI_MEMO_RECEIVE	0x00000020  /* Notify of new memos when sent */
#define auspice_NI_PRIVATE	0x00000040  /* Don't show in LIST to non-servadmins */
#define auspice_NI_HIDE_EMAIL   0x00000080
#define auspice_NI_MARK		0x00000100
#define auspice_NI_HOLD		0x00000200
#define auspice_NI_EMAILMEMOS   0x00000400
#define auspice_NI_NOOP         0x00000800
#define auspice_NI_NOMEMO       0x00001000
#define auspice_NI_NOHOST	0x00002000
#define auspice_NI_SENDLOGS     0x00004000
#define auspice_NI_NEVEROP      0x00008000
#define auspice_NI_AUTH         0x00010000  /* Reserve for AUTH services */
#define auspice_NI_PRIVMSG      0x00020000  /* Recieve notice in private message */
#define auspice_NI_MALAY        0x00040000  /* Malaysia Language for my friend ghost */
#define auspice_NI_HOP		0x00080000  /* Reconize as Services Help op */
#define auspice_NI_SOP		0x00100000  /* Reconize as Services Operator */
#define auspice_NI_EXMAIL	0x00200000  /* have been sent e-mail confirm expired */
#define auspice_NI_BOTMSG	0x00400000  /* Recieved the message from your bot */
#define auspice_NI_SLAVE	0x00800000  /* A slave nick */
#define auspice_NI_FREEZE	0x01000000  /* Stop this nick being use as a register */
#define auspice_NI_WAUTH	0x02000000  /* This nick can be use until AUTH */

#define auspice_NI_IDENTIFIED	0x80000000  /* User has IDENTIFY'd */
#define auspice_NI_RECOGNIZED	0x40000000  /* User comes from a known addy */
#define auspice_NI_KILL_HELD	0x20000000  /* Nick is being held after a kill */

/* Channel info structures.  Stored similarly to the nicks, except that
 * the second character of the channel name, not the first, is used to
 * determine the list.  (Hashing based on the first character of the name
 * wouldn't get very far. ;) ) */

struct auspice_ChanAccess_V7 {
    short level;
    short is_nick;	/* 1 if this is a nick, 0 if a user@host mask.  If
			 * -1, then this entry is currently unused (a hack
			 * to get numbered lists to have consistent
			 * numbering). */
# define in_use is_nick
    char *name;
    char *adder;
};

#define auspice_ACCESS_FOUNDER	10000	/* Numeric level indicating founder access */
#define auspice_ACCESS_INVALID	-10000	/* Used in levels[] for disabled settings */

/* auspice_AutoKick data. */
struct auspice_AutoKick {
    short is_nick;
    short pad;
    char *name;
    char *reason;
};

struct auspice_ChanInfo_V7 {
    auspice_ChanInfo_V7 *next, *prev;
    char name[auspice_CHANMAX];
    char founder[auspice_NICKMAX];		/* Always a reg'd nick */
    char founderpass[auspice_PASSMAX];          
    char *desc;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    auspice_ChanAccess_V7 *access;			/* List of authorized users */
    long akickcount;
    auspice_AutoKick *akick;
    char mlock_on[auspice_MODEMAX], mlock_off[auspice_MODEMAX]; 
    long mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[auspice_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;		/* When the last topic was set */
    long flags;				/* See below */
    short *levels;			/* Access levels for commands */
    char *url;
    char *email;
    char *welcome;
    char *hold;       /*  }                                         */
    char *mark;       /*  }   --   Identities (what admin did it?)  */
    char *freeze;     /*  }   --                                    */
    char *forbid;     /*  }                                         */
    int topic_allow;                  /* Who's allowed to change topic */
    char *successor;            /* Person who get channel when founder
				         * nick droped */
    char *mlock_link;         /* +L #mlock_link */
    char *mlock_flood;        /* +f mlock_flood */
    char *bot;                /* Bot name */
    long botflag;              /* flag CBI_ */
    long newsline;
    char **news;
    long badwline;
    char **badwords;

    long reserved[3]; /*For future expansion -- set to 0 */
};
/*****************************************************/

struct auspice_ChanAccess {
    short level;
    short is_nick;	/* 1 if this is a nick, 0 if a user@host mask.  If
			 * -1, then this entry is currently unused (a hack
			 * to get numbered lists to have consistent
			 * numbering). */
# define in_use is_nick
    char *name;
    char *adder;
    int accflag;	 /* For Time Op ... */
};

#define auspice_ACI_ZONE1  0x0002		/* 0->5 */
#define auspice_ACI_ZONE2  0x0004		/* 6->11 */
#define auspice_ACI_ZONE3  0x0008		/* 12->17 */
#define auspice_ACI_ZONE4  0x0010		/* 18->23 */

struct auspice_ChanInfo {
    auspice_ChanInfo *next, *prev;
    char name[auspice_CHANMAX];
    char founder[auspice_NICKMAX];		/* Always a reg'd nick */
    char founderpass[auspice_PASSMAX];          
    char *desc;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    auspice_ChanAccess *access;			/* List of authorized users */
    long akickcount;
    auspice_AutoKick *akick;
    char mlock_on[auspice_MODEMAX], mlock_off[auspice_MODEMAX]; 
    long mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[auspice_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;		/* When the last topic was set */
    long flags;				/* See below */
    short *levels;			/* Access levels for commands */
    char *url;
    char *email;
    char *welcome;
    char *hold;       /*  }                                         */
    char *mark;       /*  }   --   Identities (what admin did it?)  */
    char *freeze;     /*  }   --                                    */
    char *forbid;     /*  }                                         */
    int topic_allow;                  /* Who's allowed to change topic */
    char *successor;            /* Person who get channel when founder
				         * nick droped */
    char *mlock_link;         /* +L #mlock_link */
    char *mlock_flood;        /* +f mlock_flood */

    long newsline;
    char **news;

    long badwline;
    char **badwords;

    char *markreason;
    char *freezereason;
    char *holdreason;
    char *lastgetpass;

    char *bot;			      /* Bot name */
    long botflag;		      /* flag CBI_ */
    int ttb;			      /* Time for to ban */
    int capsmin, capspercent;         /* For CAPS kicker */
    int floodlines, floodsecs;        /* For FLOOD kicker */
    int repeattimes;                  /* For REPEAT kicker */
};

/* Retain topic even after last person leaves channel */
#define auspice_CI_KEEPTOPIC	0x00000001
/* Don't allow non-authorized users to be opped */
#define auspice_CI_OPGUARD	0x00000002
/* Hide channel from ChanServ LIST command */
#define auspice_CI_PRIVATE	0x00000004
/* Topic can only be changed by SET TOPIC */
#define auspice_CI_TOPICLOCK	0x00000008
/* Those not allowed ops are kickbanned */
#define auspice_CI_RESTRICTED	0x00000010
/* Don't auto-deop anyone */
#define auspice_CI_LEAVEOPS	0x00000020
/* Don't allow any privileges unless a user is IDENTIFY'd with NickServ */
#define auspice_CI_IDENT	0x00000040
/* Don't allow the channel to be registered or used */
#define auspice_CI_VERBOTEN	0x00000080
/* Channel password is encrypted */
#define auspice_CI_ENCRYPTEDPW	0x00000100
// Held channel
#define auspice_CI_HELDCHAN     0x00000200
// marked channel
#define auspice_CI_MARKCHAN     0x00000400
// Only Opers can join
#define auspice_CI_OPERONLY     0x00000800
// Only SOP+ can join
#define auspice_CI_SOPONLY      0x00001000
// Only SA+ can join
#define auspice_CI_SAONLY       0x00002000
// Only SRA+ can join
#define auspice_CI_SRAONLY      0x00004000
// Only Coders can join
#define auspice_CI_CODERONLY    0x00008000
// Only Abuse Team can join
#define auspice_CI_ABUSEONLY	0x00010000
// Frozen Channel
#define auspice_CI_FREEZECHAN   0x00020000
// Memo Levels
#define auspice_CI_MEMO_AV      0x00040000
#define auspice_CI_MEMO_AOP     0x00080000
#define auspice_CI_MEMO_SOP     0x00100000
#define auspice_CI_MEMO_CF      0x00200000
#define auspice_CI_MEMO_FR      0x00400000
#define auspice_CI_MEMO_NONE    0x00800000
#define auspice_CI_UNSECURE     0x01000000
#define auspice_CI_MEMO_HOP     0x02000000
#define auspice_CI_CHANJOIN     0x04000000  /* ChanServ stay in Channel */
#define auspice_CI_CLOSED       0x08000000  /* Reserved for Require AUTH */
#define auspice_CI_AUTOVOP      0x10000000  /* Auto +v to all guest */
#define auspice_CI_AUTOOP       0x20000000  /* Auto +o to all guest */
#define auspice_CI_AUTOHOP      0x40000000  /* Auto +h to all guest */

/* Channel's access list has masks in it, not just registered nicks */
#define auspice_CI_HAS_MASKS	0x80000000

/* Indices for cmd_access[]: */
#define auspice_CA_INVITE	0
#define auspice_CA_AKICK	1
#define auspice_CA_SET		2	/* but not FOUNDER or PASSWORD */
#define auspice_CA_UNBAN	3
#define auspice_CA_AUTOOP	4
#define auspice_CA_AUTODEOP	5	/* Maximum, not minimum */
#define auspice_CA_AUTOVOICE	6
#define auspice_CA_OPDEOP	7	/* ChanServ commands OP and DEOP */
#define auspice_CA_ACCESS_LIST	8
#define auspice_CA_CLEAR	9
#define auspice_CA_NOJOIN	10	/* Maximum */
#define auspice_CA_ACCESS_CHANGE 11
#define auspice_CA_AUTOHOP      12
#define auspice_CA_USEROP       13
#define auspice_CA_SIZE		14

#define auspice_CBI_BOTCOM		0x00000001 /* Make bot act like eggdrop bot */
#define auspice_CBI_BOTSTAY		0x00000002 /* Make bot stay in channel forever */
#define auspice_CBI_KICKCAP		0x00000004 /* Make bot kick the excess CAP */
#define auspice_CBI_KICKBOLDS		0x00000008
#define auspice_CBI_KICKCOLORS		0x00000010
#define auspice_CBI_KICKREVERSES	0x00000020
#define auspice_CBI_KICKUNDERLINES	0x00000040
#define auspice_CBI_KICKBADWORDS	0x00000080
#define auspice_CBI_KICKFLOOD		0x00000100
#define auspice_CBI_KICKREPEAT		0x00000200
#define auspice_CBI_PROTOP		0x00000400
#define auspice_CBI_GUESSNUM		0x00000800
#define auspice_CBI_GUESSWRD		0x00001000
#define auspice_CBI_NORMALMSG		0x00002000
#define auspice_CBI_GREET		0x00004000
#define auspice_CBI_VERBOSE		0x00008000
#define auspice_CBI_BADCHAN		0x00010000
#define auspice_CBI_BADNICK		0x00020000
#define auspice_CBI_PROTECT		0x00040000 /* Protect op from being deop */
#define auspice_CBI_NOGB		0x00080000 /* No Global Badword */
#define auspice_CBI_HOPGUARD		0x00100000 /* Halfop Guard */

static int auspice_def_levels[][2] = {
    { auspice_CA_AUTOOP,             5 },
    { auspice_CA_AUTOHOP,            4 },
    { auspice_CA_AUTOVOICE,          3 },
    { auspice_CA_USEROP,	     2 },
    { auspice_CA_AUTODEOP,          -1 },
    { auspice_CA_NOJOIN,            -1 },
    { auspice_CA_INVITE,             5 },
    { auspice_CA_AKICK,             10 },
    { auspice_CA_SET,               13 },
    { auspice_CA_CLEAR,             13 },
    { auspice_CA_UNBAN,              5 },
    { auspice_CA_OPDEOP,             5 },
    { auspice_CA_ACCESS_LIST,        3 },
    { auspice_CA_ACCESS_CHANGE,      1 },
    { -1 }
};

/* MemoServ data.  Only nicks that actually have memos get records in
 * MemoServ's lists, which are stored the same way NickServ's are. */

struct auspice_Memo {
    char sender[auspice_NICKMAX];
    long number;	/* Index number -- not necessarily array position! */
    time_t time;	/* When it was sent */
    char *text;
    char *chan;
    short flags;
    short reserved_s;
    long reserved[3];   /* For future expansion -- set to 0 */
};

#define auspice_MF_UNREAD	0x0001	/* Memo has not yet been read */
#define auspice_MF_DEL          0x0002  /* Memo marked as deleted */
#define auspice_MF_ACK          0x0004  /* Maked as sending back the ACK on read */
#define auspice_MF_MARK         0x0008  /* Mark as not expire */

struct auspice_MemoList {
    auspice_MemoList *next, *prev;
    char nick[auspice_NICKMAX];	/* Owner of the memos */
    long n_memos;	/* Number of memos */
    auspice_Memo *memos;	/* The memos themselves */
    long reserved[4];   /* For future expansion -- set to 0 */
};

struct auspice_Message {
    char *text;
    short type;
    char *who;
    long reserved[4];
};

#define auspice_M_LOGON 0
#define auspice_M_OPER  1

struct auspice_Trigger {
    char *mask;
    long tvalue;
    char who[auspice_NICKMAX];
    long reserved[4];
};

struct auspice_Akill {
    char *mask;
    char *reason;
    char who[auspice_NICKMAX];
    time_t time;
    time_t expires;	/* or 0 for no expiry */
    long reserved[4];
};

struct auspice_Ignore {
    char *mask;
    char who[auspice_NICKMAX];
    time_t time;
    time_t expires;	/* or 0 for no expiry */
    long reserved[4];
};

struct auspice_AdminInfo {
        auspice_AdminInfo *next, *prev;
        char *nick;             /* Admin Nickname*/
        char *host;             /* Admin host */
        char *who;		/* Who add this admin */
	char *server;
	char **mark;		/* Make by Services Root flag mark */
	long markline;
        long adflags;           /* Admin Flags */
        long flags;		/* What this Admin Can Do ? */
        time_t added;
};

#define auspice_ADF_HELPOP		0x00000001
#define auspice_ADF_SERVICEOP		0x00000002
#define auspice_ADF_SERVICEADMIN	0x00000004
#define auspice_ADF_SERVICEROOT		0x00000008

#define auspice_AI_ADDSRA		0x00000001 /* Can add SRA */
#define auspice_AI_ADDSA		0x00000002 /* Can add SA */
#define auspice_AI_HOLD			0x00000004 /* can do hold */
#define auspice_AI_GLOBAL		0x00000008 /* can send global message */
#define auspice_AI_SINGLE		0x00000010 /* Only have access in his own server */
#define auspice_AI_SUPEROP		0x00000020 /* Always AOP */
#define auspice_AI_AKILL		0x00000040 /* can do akill */
#define auspice_AI_FORCEJOIN		0x00000080 /* can do force join */
#define auspice_AI_ABUSETM		0x00000100 /* Abuse Team */
#define auspice_AI_QUIT			0x00000200 /* Suspend From Rank */
#define auspice_AI_FORBID		0x00000400 /* Can do forbid */
#define auspice_AI_IGNORE		0x00000800 /* ignore tigger */
#define auspice_AI_NICKOP		0x00001000 /* Nick Operator, setnick flag */
#define auspice_AI_CHANOP		0x00002000 /* chan Operator */
#define auspice_AI_NPASS		0x00004000 /* nick password */
#define auspice_AI_CPASS		0x00008000 /* chan password */
#define auspice_AI_CLOSE		0x00010000 /* chan closure */	
#define auspice_AI_BOTSERV		0x00020000 /* BotServ */	
#define auspice_AI_GPASS		0x00040000 /* Get password */	
#define auspice_AI_FREEZE		0x00080000 /* Freeze */	

#define auspice_ADS_VERSION	1
#define auspice_NS_VERSION	6
#define auspice_CS_VERSION	8
#define auspice_MS_VERSION	6
#define auspice_OS_TR_VERSION	6
#define auspice_OS_AKI_VERSION	6 /* Akill */
#define auspice_OS_IG_VERSION	6
#define auspice_AS_MSG_VERSION	6

/* Highest Version */

#define auspice_FILE_VERSION    8

/* Return the version number on the file.  Panic if there is no version
 * number or the number doesn't make sense (i.e. less than 1 or greater
 * than FILE_VERSION).
 */

int auspice_get_file_version(FILE *f, const char *filename)
{
    int version = fgetc(f)<<24 | fgetc(f)<<16 | fgetc(f)<<8 | fgetc(f);
    if (ferror(f))
    {
	SLOG(LM_EMERGENCY, "Error reading version number on $1", (filename));
    }
    else if (ferror(f))
    {
	SLOG(LM_EMERGENCY, "Error reading version number on $1: End of file detected", (
		filename));
    }
    else if (version > auspice_FILE_VERSION || version < 1)
    {
	SLOG(LM_EMERGENCY, "Invalid version number ($1) on $2", ( version, filename));
    }
    return version;
}

FILE *auspice_open_db_read(const char *service, const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f) {
	if (errno != ENOENT)
	{
	    SLOG(LM_ERROR, "Can't read $1 database $2", (service, filename));
	}
	return NULL;
    }
    return f;
}

FILE *auspice_open_db(const char *service, const char *filename, const char *mode, short ver)
{
    static_cast<void>(ver);
    if (*mode == 'r') {
	return auspice_open_db_read(service, filename);
    } else {
	errno = EINVAL;
	return NULL;
    }
}

/* Close a database file.  If the file was opened for write, remove the
 * backup we (may have) created earlier.
 */

void auspice_close_db(FILE *dbfile, const char *filename)
{
    int flags;

    flags = fcntl(fileno(dbfile), F_GETFL);
    if ((flags != -1) &&
		(((flags & O_ACCMODE) == O_WRONLY) ||
		 ((flags & O_ACCMODE) == O_RDWR)))
    {
	char namebuf[NAME_MAX+1];
#ifndef CYGWIN
	snprintf(namebuf, sizeof(namebuf), "%s.save", filename);
#else
	snprintf(namebuf, sizeof(namebuf), "save-%s", filename);
#endif
	if (*namebuf && strcmp(namebuf, filename) != 0)
	    remove(namebuf);
    }
    fclose(dbfile);
}

/* read_string, write_string:
 *	Read a string from a file, or write a string to a file, with the
 *	string length prefixed as a two-byte big-endian integer.  The
 *	filename is passed in so that it can be reported in the log file
 *	(and possibly with wallops) if an error occurs.
 */

char *auspice_read_string(FILE *f, const char *filename)
{
    char *s;
    size_t len;

    len = fgetc(f) * 256 + fgetc(f);
    s = (char *) malloc(len);
    if (len != fread(s, 1, len, f))
    {
        SLOG(LM_EMERGENCY, "Read error on file: $1", (filename));
    }
    return s;
}

int auspice_delnick(auspice_NickInfo *ni);
int auspice_delchan(auspice_ChanInfo *ci);
void auspice_del_memolist(auspice_MemoList * ml);
int auspice_deladmin(auspice_AdminInfo *ais);
void auspice_reset_levels(auspice_ChanInfo *ci);

void auspice_load_nick()
{
    FILE *f;
    int i,j;
    auspice_NickInfo *ni;

    if (!(f = auspice_open_db("NickServ", NICKSERV_DB, "r", 1)))
		return;

    switch (i = auspice_get_file_version(f, NICKSERV_DB)) {
      case 6:
        for (i = 33; i < 256; ++i) {
            while (fgetc(f) == 1) {
                   ni = (auspice_NickInfo *) malloc(sizeof(auspice_NickInfo));
                   if (1 != fread(ni, sizeof(auspice_NickInfo), 1, f))
		   {
                       SLOG(LM_EMERGENCY, "Read error on $1", (NICKSERV_DB));
		   }

                ni->flags &= ~(auspice_NI_IDENTIFIED | auspice_NI_RECOGNIZED);

                if (ni->url)
                    ni->url = auspice_read_string(f, NICKSERV_DB);

                if (ni->email)
                    ni->email = auspice_read_string(f, NICKSERV_DB);

                if (ni->uin)
                    ni->uin = auspice_read_string(f, NICKSERV_DB);

                if (ni->age)
                    ni->age = auspice_read_string(f, NICKSERV_DB);

			    if (ni->forward)
			 		ni->forward = auspice_read_string(f, NICKSERV_DB);

                if (ni->hold)
                    ni->hold = auspice_read_string(f, NICKSERV_DB);

                if (ni->mark)
                    ni->mark = auspice_read_string(f, NICKSERV_DB);

                if (ni->forbid)
                    ni->forbid = auspice_read_string(f, NICKSERV_DB);

                if (ni->info)
                    ni->info = auspice_read_string(f, NICKSERV_DB);

                if (ni->sex)
                    ni->sex = auspice_read_string(f, NICKSERV_DB);

                if (ni->mlock)
                    ni->mlock = auspice_read_string(f, NICKSERV_DB);
				else
		    		ni->mlock = NULL;

                if (ni->last_quit)
                    ni->last_quit = auspice_read_string(f, NICKSERV_DB);

		if (ni->last_usermask)
			ni->last_usermask = auspice_read_string(f, NICKSERV_DB);

		if (ni->last_realname)
			ni->last_realname = auspice_read_string(f, NICKSERV_DB);

                if (ni->accesscount) {
                    char **access1;
                    access1 = (char **) malloc(sizeof(char *) * ni->accesscount);
                    ni->access = access1;
                    for (j = 0; j < ni->accesscount; ++j, ++access1)
                        *access1 = auspice_read_string(f, NICKSERV_DB);
		} else {
			ni->access = NULL;
		}

                if (ni->ajoincount) {
                    char **autojoin;
                    autojoin = (char **) malloc(sizeof(char *) * ni->ajoincount);
                    ni->autojoin = autojoin;
                    for (j = 0; j < ni->ajoincount; ++j, ++autojoin)
                        *autojoin = auspice_read_string(f, NICKSERV_DB);
                }

                if (ni->comline) {
                    char **nickcom;
                    nickcom = (char **) malloc(sizeof(char *) * ni->comline);
                    ni->comment = nickcom;
                    for (j = 0; j < ni->comline; ++j, ++nickcom)
                        *nickcom = auspice_read_string(f, NICKSERV_DB);
                }

                if (ni->noteline) {
                    char **nicknote;
                    nicknote = (char **) malloc(sizeof(char *) * ni->noteline);
                    ni->note = nicknote;
                    for (j = 0; j < ni->noteline; ++j, ++nicknote)
                        *nicknote = auspice_read_string(f, NICKSERV_DB);
                }
                ni->id_timestamp = 0;

		Nick_Stored_t *nick = Convert::auspice_CreateNickEntry(ni);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);
		auspice_delnick(ni);
            } /* while (fgetc(f) == 1) */
        } /* for (i) */
        break;
      case 5:
	{
	auspice_NickInfo_V5 *old_ni;
        for (i = 33; i < 2000; ++i) {
            while (fgetc(f) == 1) {
                   ni = (auspice_NickInfo *) malloc(sizeof(auspice_NickInfo));
                   old_ni = (auspice_NickInfo_V5 *) malloc(sizeof(auspice_NickInfo_V5));

                   if (1 != fread(old_ni, sizeof(auspice_NickInfo_V5), 1, f))
		   {
                       SLOG(LM_EMERGENCY, "Read error on $1", (NICKSERV_DB));
		   }
                strncpy(ni->nick, old_ni->nick, sizeof(ni->nick));
                strncpy(ni->pass, old_ni->pass, sizeof(ni->pass));

                ni->time_registered = old_ni->time_registered;
                ni->last_seen = old_ni->last_seen;
                ni->accesscount = old_ni->accesscount;
                ni->flags = old_ni->flags;
                ni->memomax = old_ni->memomax;
                ni->channelcount = old_ni->channelcount;
                ni->news = old_ni->news;
                ni->flags &= ~(auspice_NI_IDENTIFIED | auspice_NI_RECOGNIZED);
		ni->eflags = 0;
		ni->ajoincount = 0;
		ni->comline = 0;
		ni->noteline = 0;

                if (old_ni->url) {
                    old_ni->url = auspice_read_string(f, NICKSERV_DB);
		    if (strlen(old_ni->url)>0)
			ni->url = old_ni->url;
		    else
			ni->url = NULL;
		} else
		    ni->url = NULL;

                if (old_ni->email) {
                    old_ni->email = auspice_read_string(f, NICKSERV_DB);
		    if (strlen(old_ni->email)>0)
			ni->email = old_ni->email;
		    else
			ni->email = NULL;
		} else
		    ni->email = NULL;

                if (old_ni->uin) {
                    old_ni->uin = auspice_read_string(f, NICKSERV_DB);
		    if(strlen(old_ni->uin)>0)
			ni->uin = old_ni->uin;
		    else
			ni->uin = NULL;
		} else
		    ni->uin = NULL;

                if (old_ni->age) {
                    old_ni->age = auspice_read_string(f, NICKSERV_DB);
		    if (strlen(old_ni->age)>0)
			ni->age = old_ni->age;
		    else
			ni->age = NULL;
		} else
		    ni->age = NULL;

		if (old_ni->forward) {
		    old_ni->forward = auspice_read_string(f, NICKSERV_DB);
		    if (strlen(old_ni->forward)>0)
			ni->forward = old_ni->forward;
		    else
			ni->forward = NULL;
		} else
		    ni->forward = NULL;

                if (old_ni->hold) {
                    old_ni->hold = auspice_read_string(f, NICKSERV_DB);
		    if (strlen(old_ni->hold)>0)
			ni->hold = old_ni->hold;
		    else
			ni->hold = NULL;
		} else
		    ni->hold = NULL;

                if (old_ni->mark) {
                    old_ni->mark = auspice_read_string(f, NICKSERV_DB);
		    if (strlen(old_ni->mark)>0)
			ni->mark = old_ni->mark;
		    else
			ni->mark = NULL;
		} else
		    ni->mark = NULL;

                if (old_ni->forbid) {
                    old_ni->forbid = auspice_read_string(f, NICKSERV_DB);
		    if (strlen(old_ni->forbid)>0)
			ni->forbid = old_ni->forbid;
		    else
			ni->forbid = NULL;
		} else
		    ni->forbid = NULL;

                if (old_ni->info) {
                    old_ni->info = auspice_read_string(f, NICKSERV_DB);
		    if (strlen(old_ni->info)>0)
			ni->info = old_ni->info;
		    else
			ni->info = NULL;	
		} else
			ni->info = NULL;
#ifdef OLDB3
  	        ni->mlock = NULL;
#else
                if (old_ni->mlock) {
                    old_ni->mlock = auspice_read_string(f, NICKSERV_DB);
		    if (strlen(old_ni->mlock)>0)
			ni->mlock = old_ni->mlock;
		    else
			ni->mlock = NULL;
		} else
		    ni->mlock = NULL;
#endif

                if (old_ni->sex) {
                    old_ni->sex = auspice_read_string(f, NICKSERV_DB);
		    if (strlen(old_ni->sex)>0)
			ni->sex = old_ni->sex;
		    else
			ni->sex = NULL;
		} else
		    ni->sex = NULL;

		ni->last_quit = NULL;

                old_ni->last_usermask = auspice_read_string(f, NICKSERV_DB);
		ni->last_usermask = old_ni->last_usermask;
                old_ni->last_realname = auspice_read_string(f, NICKSERV_DB);
		ni->last_realname = old_ni->last_realname;

		free(old_ni);

                if (ni->accesscount) {
                    char **access1;
                    access1 = (char **) malloc(sizeof(char *) * ni->accesscount);
                    ni->access = access1;
                    for (j = 0; j < ni->accesscount; j++, access1++)
                        *access1 = auspice_read_string(f, NICKSERV_DB);
                }

                ni->autojoin = NULL;
                ni->note = NULL;
                ni->comment = NULL;

                ni->id_timestamp = 0;

		Nick_Stored_t *nick = Convert::auspice_CreateNickEntry(ni);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);
		auspice_delnick(ni);

            } /* while (fgetc(f) == 1) */
        } /* for (i) */
        break;
	}
      case 4:
      case 3:
      case 2:
      case 1:
      default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", ( i, NICKSERV_DB));
    } /* switch (version) */

    auspice_close_db(f, NICKSERV_DB);
}

/* Remove a nick from the NickServ database.  Return 1 on success, 0
 * otherwise.  Also deletes the nick from any channel access lists it is
 * on. 
 */

int auspice_delnick(auspice_NickInfo *ni)
{
    int i;

    if (ni->last_usermask)
	free(ni->last_usermask);
    if (ni->last_realname)
	free(ni->last_realname);
    if (ni->access) {
	for (i = 0; i < ni->accesscount; ++i)
	    free(ni->access[i]);
	free(ni->access);
    }
    if (ni->autojoin) {
	for (i = 0; i < ni->ajoincount; i++) {
	    if (ni->autojoin[i])
	    free(ni->autojoin[i]);
	}
	free(ni->autojoin);
    }
    if (ni->comline) {
	for (i = 0; i < ni->comline; i++) {
	    if (ni->comment[i])
	    free(ni->comment[i]);
	}
	free(ni->comment);
    }
    if (ni->noteline) {
	for (i = 0; i < ni->noteline; i++) {
	    if (ni->note[i])
	    free(ni->note[i]);
	}
	free(ni->note);
    }

    if (ni->url)
      free(ni->url);
    if (ni->email)
      free(ni->email);
    if (ni->forward)
      free(ni->forward);
    if (ni->hold)
      free(ni->hold);
    if (ni->mark);
      free(ni->mark);
    if (ni->forbid)
      free(ni->forbid);
    if (ni->uin)
      free(ni->uin);
    if (ni->age)
      free(ni->age);
    if (ni->info)
      free(ni->info);
    if (ni->sex)
      free(ni->sex);
    if (ni->mlock)
      free(ni->mlock);
    if (ni->last_quit)
      free(ni->last_quit);

    free(ni);
    return 1;
}

void auspice_load_chan()
{
    FILE *f = fopen(CHANSERV_DB, "r");
    int i, j;
    auspice_ChanInfo *ci;

    if (!(f = auspice_open_db("ChanServ", CHANSERV_DB, "r",1)))
	return;

    switch (i = auspice_get_file_version(f, CHANSERV_DB)) {

      case 8:
		  {
        for (i = 33; i < 256; ++i) {
            while (fgetc(f) == 1) {
                ci = (auspice_ChanInfo *) malloc(sizeof(auspice_ChanInfo));
       	        if (1 != fread(ci, sizeof(auspice_ChanInfo), 1, f))
		{
                    SLOG(LM_EMERGENCY, "Read error on $1", (CHANSERV_DB));
		}

                ci->desc = auspice_read_string(f, CHANSERV_DB);

                if (ci->url) {
                    ci->url = auspice_read_string(f, CHANSERV_DB);
		} else
                    ci->url = NULL;
		
                if (ci->email) {
                    ci->email = auspice_read_string(f, CHANSERV_DB);
		} else
                        ci->email = NULL;
		
                if (ci->mlock_key) {
                    ci->mlock_key = auspice_read_string(f, CHANSERV_DB);
                } else
                    ci->mlock_key = NULL;

                if (ci->last_topic) {
                    ci->last_topic = auspice_read_string(f, CHANSERV_DB);
                } else
                    ci->last_topic = NULL;

		if (ci->welcome)
		    ci->welcome = auspice_read_string(f, CHANSERV_DB);
                if (ci->hold)
                    ci->hold = auspice_read_string(f, CHANSERV_DB);
                if (ci->mark)
                    ci->mark = auspice_read_string(f, CHANSERV_DB);
                if (ci->freeze)
                    ci->freeze = auspice_read_string(f, CHANSERV_DB);
                if (ci->forbid)
                    ci->forbid = auspice_read_string(f, CHANSERV_DB);

                if (ci->successor) {
                    ci->successor = auspice_read_string(f, CHANSERV_DB);
		} else
		    ci->successor = NULL;

		if (ci->mlock_link)
                    ci->mlock_link = auspice_read_string(f, CHANSERV_DB);
		else
		    ci->mlock_link = NULL;

       		if (ci->mlock_flood)
                    ci->mlock_flood = auspice_read_string(f, CHANSERV_DB);
		else
		    ci->mlock_flood = NULL;

       		if (ci->bot)
                    ci->bot = auspice_read_string(f, CHANSERV_DB);
		else
		    ci->bot = NULL;

       		if (ci->markreason)
                    ci->markreason = auspice_read_string(f, CHANSERV_DB);
		else
		    ci->markreason = NULL;

       		if (ci->freezereason)
                    ci->freezereason = auspice_read_string(f, CHANSERV_DB);
		else
		    ci->freezereason = NULL;

       		if (ci->holdreason)
                    ci->holdreason = auspice_read_string(f, CHANSERV_DB);
		else
		    ci->holdreason = NULL;

       		if (ci->lastgetpass)
                    ci->lastgetpass = auspice_read_string(f, CHANSERV_DB);
		else
		    ci->lastgetpass = NULL;


                if (ci->accesscount) {
                    auspice_ChanAccess *access1;
                    access1 = (auspice_ChanAccess *) malloc(sizeof(auspice_ChanAccess) * ci->accesscount);
                    ci->access = access1;
                    if ((size_t) ci->accesscount != fread(access1, sizeof(auspice_ChanAccess),
                                                        ci->accesscount, f))
		    {
                        SLOG(LM_EMERGENCY, "Read error on ", (CHANSERV_DB));
		    }
                    for (j = 0; j < ci->accesscount; ++j, ++access1) {
                        access1->name = auspice_read_string(f, CHANSERV_DB);
                        access1->adder = auspice_read_string(f, CHANSERV_DB);
		    }
                    access1 = ci->access;
                    /* Clear out unused entries */
                    while (j < ci->accesscount) {
                        if (access1->is_nick < 0
			|| (access1->is_nick == 0 && !Magick::instance().nickserv.IsStored(access1->name)))
			{
                            --ci->accesscount;
                            free(access1->name);
                            free(access1->adder);
                            if (j < ci->accesscount)
				memmove(access1, access1+1, sizeof(*access1) * (ci->accesscount - j));
                        } else {
                            access1->is_nick = 1;
                            ++j; ++access1;
                        }
                    }
                    if (ci->accesscount) {
                        ci->access = (auspice_ChanAccess *) realloc(ci->access, sizeof(auspice_ChanAccess) * ci->accesscount);
                    } else {
                        ci->access = NULL;
                        free(ci->access);
                    }
                } else {
                        ci->access = NULL;
                        free(ci->access);
		} /* if (ci->accesscount) */

                if (ci->akickcount) {
                    auspice_AutoKick *akick;
                    akick = (auspice_AutoKick *) malloc(sizeof(auspice_AutoKick) * ci->akickcount);
                    ci->akick = akick;
                    if ((size_t) ci->akickcount !=
                            fread(akick, sizeof(auspice_AutoKick), ci->akickcount, f))
		    {
                        SLOG(LM_EMERGENCY, "Read error on $1", (CHANSERV_DB));
		    }
                    for (j = 0; j < ci->akickcount; ++j, ++akick) {
                        akick->name = auspice_read_string(f, CHANSERV_DB);
                        if (akick->reason)
                            akick->reason = auspice_read_string(f, CHANSERV_DB);
                    }
                    j = 0; akick = ci->akick;
                    while (j < ci->akickcount) {
                        if (akick->is_nick < 0) {
                            --ci->akickcount;
                            free(akick->name);
                            if (akick->reason)
                                free(akick->reason);
                            if (j < ci->akickcount)
				memmove (akick, akick+1, sizeof(*akick) * (ci->akickcount - j));
                        } else {
                            ++j; ++akick;
                        }
                    }
                    if (ci->akickcount) {
                        ci->akick = (auspice_AutoKick *) realloc(ci->akick,
                                        sizeof(auspice_AutoKick) * ci->akickcount);
                    } else {
                        free(ci->akick);
                        ci->akick = NULL;
                    }
                } /* if (ci->akickcount) */
                if (ci->levels) {
                    int n_entries;
                    ci->levels = NULL;
                    auspice_reset_levels(ci);
                    n_entries = fgetc(f)<<8 | fgetc(f);
                    if (n_entries < 0)
		    {
                        SLOG(LM_EMERGENCY, "Read error on $1", (CHANSERV_DB));
		    }
                    /* Ignore earlier, incompatible levels list */
                    if (n_entries == 6) {
                        fseek(f, sizeof(short) * n_entries, SEEK_CUR);
                    } else
                    if (n_entries <= auspice_CA_SIZE) {
                        fread(ci->levels, sizeof(short), n_entries, f);
                    } else {
                        fread(ci->levels, sizeof(short), auspice_CA_SIZE, f);
                        fseek(f, sizeof(short) * (n_entries - auspice_CA_SIZE),
                                                                SEEK_CUR);
                    }
                } else {
                    auspice_reset_levels(ci);
                }

                if (ci->newsline) {
                    char **channews;
                    channews = (char **) malloc(sizeof(char *) * ci->newsline);
                    ci->news = channews;
                    for (j = 0; j < ci->newsline; ++j, ++channews) {
                        *channews = auspice_read_string(f, CHANSERV_DB);
                    }
                } else {
			 ci->news = NULL;
		    }

                if (ci->badwline) {
                    char **badwords;
                    badwords = (char **) malloc(sizeof(char *) * ci->badwline);
                    ci->badwords = badwords;
                    for (j = 0; j < ci->badwline; ++j, ++badwords) {
                        *badwords = auspice_read_string(f, CHANSERV_DB);
                    }
                } else {
			ci->badwords = NULL;
                }

		Chan_Stored_t *chan = Convert::auspice_CreateChanEntry(ci);
		if (chan != NULL)
		{
		    Magick::instance().chanserv.AddStored(chan);
		    if (ci->newsline)
		    {
			MemoServ::channel_news_t news = Convert::auspice_CreateNewsEntry(ci->newsline, ci->news, ci->name);
			if (news.size())
			    Magick::instance().memoserv.AddChannel(news);
		    }
		}
		auspice_delchan(ci);

            } /* while (fgetc(f) == 1) */
}
        } /* for (i) */
	break;
      case 7:
	{
	auspice_ChanInfo_V7 *old_ci;
        for (i = 33; i < 256; ++i) {
            while (fgetc(f) == 1) {

                ci = (auspice_ChanInfo *) malloc(sizeof(auspice_ChanInfo));
                old_ci = (auspice_ChanInfo_V7 *) malloc(sizeof(auspice_ChanInfo_V7));
       	        if (1 != fread(old_ci, sizeof(auspice_ChanInfo_V7), 1, f))
		{
                    SLOG(LM_EMERGENCY, "Read error on $1", (CHANSERV_DB));
		}

                if (old_ci->flags & auspice_CI_ENCRYPTEDPW) {
                    SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted"
                          "but encryption disabled, aborting",
                          ("ChanServ", old_ci->name));
                }

		strncpy(ci->mlock_on, old_ci->mlock_on, sizeof(ci->mlock_on));
		strncpy(ci->mlock_off, old_ci->mlock_off, sizeof(ci->mlock_off));
                strncpy(ci->name, old_ci->name, sizeof(ci->name));
                strncpy(ci->founder, old_ci->founder, sizeof(ci->founder));
                strncpy(ci->founderpass, old_ci->founderpass,
                                                sizeof(ci->founderpass));
                strncpy(ci->last_topic_setter, old_ci->last_topic_setter,
                                        sizeof(ci->last_topic_setter));
                ci->time_registered = old_ci->time_registered;
                ci->last_used = old_ci->last_used;
                ci->accesscount = old_ci->accesscount;
                ci->akickcount = old_ci->akickcount;
                ci->mlock_limit = old_ci->mlock_limit;
                ci->last_topic_time = old_ci->last_topic_time;
                ci->flags = old_ci->flags;
                ci->levels = old_ci->levels;
                ci->topic_allow = old_ci->topic_allow;
		ci->botflag = old_ci->botflag;
		ci->newsline = old_ci->newsline;
		ci->badwline = old_ci->badwline;

		if (old_ci->desc) {
        	        old_ci->desc = auspice_read_string(f, CHANSERV_DB);
			if (strlen(old_ci->desc)>0) {
	        	        ci->desc = strdup(old_ci->desc);
			} else
				ci->desc = NULL;
	                free (old_ci->desc);
		} else
			ci->desc = NULL;

                if (old_ci->url) {
                    old_ci->url = auspice_read_string(f, CHANSERV_DB);
                    if (strlen(old_ci->url) > 0)
                        ci->url = strdup(old_ci->url);
                    else
                        ci->url = NULL;
                    free (old_ci->url);
		} else
                    ci->url = NULL;
		
                if (old_ci->email) {
                    old_ci->email = auspice_read_string(f, CHANSERV_DB);
                    if (strlen(old_ci->email) > 0)
                        ci->email = strdup(old_ci->email);
                    else
                        ci->email = NULL;
                    free (old_ci->email);
		} else
                        ci->email = NULL;

                if (old_ci->mlock_key)
                {
                    old_ci->mlock_key = auspice_read_string(f, CHANSERV_DB);
                    if (strlen(old_ci->mlock_key) > 0)
	                    ci->mlock_key = strdup(old_ci->mlock_key);
		    else
			    ci->mlock_key = NULL;
                    free(old_ci->mlock_key);
                }
                else
                    ci->mlock_key = NULL;

                if (old_ci->last_topic) {
                    old_ci->last_topic = auspice_read_string(f, CHANSERV_DB);
		    if (strlen(old_ci->last_topic)>0)
	                    ci->last_topic = strdup(old_ci->last_topic);
		    else
			    ci->last_topic = NULL;
                    free (old_ci->last_topic);
                } else
                    ci->last_topic = NULL;

		if (old_ci->welcome) {
		    old_ci->welcome = auspice_read_string(f, CHANSERV_DB);
		    if (strlen(old_ci->welcome)>0)
	                    ci->welcome = strdup(old_ci->welcome);
		    else
			    ci->welcome = NULL;
                    free (old_ci->welcome);
		} else
                    ci->welcome = NULL;


                if (old_ci->hold) {
                    old_ci->hold = auspice_read_string(f, CHANSERV_DB);
		    if (strlen(old_ci->hold)>0)
			    ci->hold = strdup(old_ci->hold);
		    else
			    ci->hold = NULL;
		    free(old_ci->hold);
		} else
                    ci->hold = NULL;

                if (old_ci->mark) {
                    old_ci->mark = auspice_read_string(f, CHANSERV_DB);
		    if (strlen(old_ci->mark)>0)
			    ci->mark = strdup(old_ci->mark);
		    else
			    ci->mark = NULL;
		    free(old_ci->mark);
		} else
		    ci->mark = NULL;

                if (old_ci->freeze) {
                    old_ci->freeze = auspice_read_string(f, CHANSERV_DB);
		    if (strlen(old_ci->freeze)>0)
			    ci->freeze = strdup(old_ci->freeze);
		    else
			    ci->freeze = NULL;
		    free(old_ci->freeze);
		} else
		    ci->freeze = NULL;

                if (old_ci->forbid) {
                    old_ci->forbid = auspice_read_string(f, CHANSERV_DB);
		    if (strlen(old_ci->forbid)>0)
			    ci->forbid = strdup(old_ci->forbid);
		    else
			    ci->forbid = NULL;
		    free(old_ci->forbid);
		} else
		    ci->forbid = NULL;

                if (old_ci->successor) {
                    old_ci->successor = auspice_read_string(f, CHANSERV_DB);
		    if (strlen(old_ci->successor)>0)
			    ci->successor = strdup(old_ci->successor);
		    else
			    ci->successor = NULL;
		    free(old_ci->successor);
		} else
		    ci->successor = NULL;

		if (old_ci->mlock_link) {
                    old_ci->mlock_link = auspice_read_string(f, CHANSERV_DB);
		    if (strlen(old_ci->mlock_link)>0)
			    ci->mlock_link = strdup(old_ci->mlock_link);
		    else
			    ci->mlock_link = NULL;
		    free(old_ci->mlock_link);
		} else
		    ci->mlock_link = NULL;

       		if (old_ci->mlock_flood) {
                    old_ci->mlock_flood = auspice_read_string(f, CHANSERV_DB);
		    if (strlen(old_ci->mlock_flood)>0)
			    ci->mlock_flood = strdup(old_ci->mlock_flood);
		    else
			   ci->mlock_flood = NULL;
		    free(old_ci->mlock_flood);
		} else
		    ci->mlock_flood = NULL;

       		if (old_ci->bot) {
                    old_ci->bot = auspice_read_string(f, CHANSERV_DB);
		    if (strlen(old_ci->bot)>0)
			    ci->bot = strdup(old_ci->bot);
		    else
			ci->bot=NULL;
		    free(old_ci->bot);
		} else
		    ci->bot = NULL;

                if (ci->accesscount) {
                    auspice_ChanAccess *access1;
                    auspice_ChanAccess_V7 *old_access;
                    old_access = (auspice_ChanAccess_V7 *) malloc(sizeof(auspice_ChanAccess_V7) * old_ci->accesscount);
                    access1 = (auspice_ChanAccess *) malloc(sizeof(auspice_ChanAccess) * ci->accesscount);

                    old_ci->access = old_access;
                    ci->access = access1;

                    if ((size_t) ci->accesscount != fread(old_access,sizeof(auspice_ChanAccess_V7),
                                                        ci->accesscount, f))
		    {
                        SLOG(LM_EMERGENCY, "Read error on ", (CHANSERV_DB));
		    }

                    for (j = 0; j < ci->accesscount; ++j, ++access1, ++old_access) {
			if (old_access->name)
	                        access1->name = auspice_read_string(f, CHANSERV_DB);
			else
			   access1->name = NULL;
			if (old_access->adder)
	                        access1->adder = auspice_read_string(f, CHANSERV_DB);
			else
				access1->adder = NULL;
			access1->is_nick = old_access->is_nick;
			access1->level = old_access->level;
			access1->in_use = old_access->in_use;
			access1->accflag = 0;
		    }

		    access1 = ci->access;
		    j=0;

                    /* Clear out unused entries */
                    while (j < ci->accesscount) {
                        if (!Magick::instance().nickserv.IsStored(access1->name))
			{
                            --ci->accesscount;
			if (access1->name) {
                            free(access1->name);
			}
			if (access1->adder) {
                            free(access1->adder);
			}
                        if (j < ci->accesscount)
			    memmove(access1, access1+1,sizeof(*access1) * (ci->accesscount - j));

                        } else {
                            access1->is_nick = 1;
                            ++j; ++access1;
                        }
                    }

                    if (old_ci->accesscount) {
                        ci->access = (auspice_ChanAccess *) realloc(ci->access,sizeof(auspice_ChanAccess) * ci->accesscount);
                    } else {
                        ci->access = NULL;
                        free(ci->access);
                    }
                } else {
                        ci->access = NULL;
                        free(ci->access);
		} 


                if (ci->akickcount) {
                    auspice_AutoKick *akick;
                    akick = (auspice_AutoKick *) malloc(sizeof(auspice_AutoKick) * ci->akickcount);
                    ci->akick = akick;
                    if ((size_t) ci->akickcount !=
                            fread(akick, sizeof(auspice_AutoKick), ci->akickcount, f))
		    {
                        SLOG(LM_EMERGENCY, "Read error on $1", (CHANSERV_DB));
		    }
                    for (j = 0; j < ci->akickcount; ++j, ++akick) {
                        akick->name = auspice_read_string(f, CHANSERV_DB);
                        if (akick->reason)
                            akick->reason = auspice_read_string(f, CHANSERV_DB);
                    }
                    j = 0; akick = ci->akick;
                    while (j < ci->akickcount) {
                        if (akick->is_nick < 0) {
                            --ci->akickcount;
                            free(akick->name);
                            if (akick->reason)
                                free(akick->reason);
                            if (j < ci->akickcount)
				    memmove(akick, akick+1, sizeof(*akick) * (ci->akickcount - j));
                        } else {
                            ++j; ++akick;
                        }
                    }
                    if (ci->akickcount) {
                        ci->akick = (auspice_AutoKick *) realloc(ci->akick,
                                        sizeof(auspice_AutoKick) * ci->akickcount);
                    } else {
                        free(ci->akick);
                        ci->akick = NULL;
                    }
                }

                if (ci->levels) {
                    int n_entries;
                    ci->levels = NULL;
                    auspice_reset_levels(ci);
                    n_entries = fgetc(f)<<8 | fgetc(f);
                    if (n_entries < 0)
		    {
                        SLOG(LM_EMERGENCY, "Read error on $1", (CHANSERV_DB));
		    }
                    /* Ignore earlier, incompatible levels list */
                    if (n_entries == 6) {
                        fseek(f, sizeof(short) * n_entries, SEEK_CUR);
                    } else
                    if (n_entries <= auspice_CA_SIZE) {
                        fread(ci->levels, sizeof(short), n_entries, f);
                    } else {
                        fread(ci->levels, sizeof(short), auspice_CA_SIZE, f);
                        fseek(f, sizeof(short) * (n_entries - auspice_CA_SIZE),
                                                                SEEK_CUR);
                    }
                } else {
                    auspice_reset_levels(ci);
                }

                if (ci->newsline) {
                    char **channews;
                    channews = (char **) malloc(sizeof(char *) * ci->newsline);
                    ci->news = channews;
                    for (j = 0; j < ci->newsline; ++j, ++channews) {
                        *channews = auspice_read_string(f, CHANSERV_DB);
                    }
                }

                if (ci->badwline) {
                    char **badwords;
                    badwords = (char **) malloc(sizeof(char *) * ci->badwline);
                    ci->badwords = badwords;
                    for (j = 0; j < ci->badwline; ++j, ++badwords) {
                        *badwords = auspice_read_string(f, CHANSERV_DB);
                    }
                }

		ci->markreason = NULL;
		ci->freezereason = NULL;
		ci->holdreason = NULL;
		ci->lastgetpass = NULL;

		ci->ttb = 5; /* Default ban 5 minutes */
		ci->capsmin = 10;
		ci->capspercent = 25; /* 25% */
		ci->floodlines = 5;
		ci->floodsecs = 2;
		ci->repeattimes = 4;

		Chan_Stored_t *chan = Convert::auspice_CreateChanEntry(ci);
		if (chan != NULL)
		{
		    Magick::instance().chanserv.AddStored(chan);
		    if (ci->newsline)
		    {
			MemoServ::channel_news_t news = Convert::auspice_CreateNewsEntry(ci->newsline, ci->news, ci->name);
			if (news.size())
			    Magick::instance().memoserv.AddChannel(news);
		    }
		}
		auspice_delchan(ci);

            } 

        } 

        break; 
	}
      default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", ( i, CHANSERV_DB));

    } /* switch (version) */

    auspice_close_db(f, CHANSERV_DB);
}


void auspice_reset_levels(auspice_ChanInfo *ci)
{
    unsigned int i;

    if (ci->levels)
	free(ci->levels);
    ci->levels = (short *) malloc(auspice_CA_SIZE * sizeof(*ci->levels));
    for (i = 0; auspice_def_levels[i][0] >= 0; i++)
	ci->levels[auspice_def_levels[i][0]] = auspice_def_levels[i][1];
}


/* Remove a channel from the ChanServ database.  Return 1 on success, 0
 * otherwise. */

int auspice_delchan(auspice_ChanInfo *ci)
{
    int i;

    if (ci->desc)
	free(ci->desc);

    if (ci->newsline) {
        for (i = 0; i < ci->newsline; i++) {
            if (ci->news[i])
            free(ci->news[i]);
        }
    }

    if (ci->news)
	free(ci->news);

    if (ci->badwline) {
        for (i = 0; i < ci->badwline; i++) {
            if (ci->badwords[i])
            free(ci->badwords[i]);
        }
    }

    if (ci->badwords)
	free(ci->badwords);

    if (ci->mlock_key)
	free(ci->mlock_key);

    if (ci->mlock_flood)
	free(ci->mlock_flood);

    if (ci->mlock_link)
	free(ci->mlock_link);

    if (ci->last_topic)
	free(ci->last_topic);

    for (i = 0; i < ci->accesscount; ++i) {
	if (ci->access[i].name) 
		free(ci->access[i].name);
	if (ci->access[i].adder)
		free(ci->access[i].adder);
    }


    if (ci->access)
	free(ci->access);

    if (ci->akickcount) {
    for (i = 0; i < ci->akickcount; ++i) {
	free(ci->akick[i].name);
	if (ci->akick[i].reason)
	    free(ci->akick[i].reason);
    }

    if (ci->akick) {
	free(ci->akick);
    }
    }

    if (ci->levels)
        free(ci->levels);

    if (ci->welcome)
        free(ci->welcome);

    if (ci->hold)
        free(ci->hold);

    if (ci->mark)
        free(ci->mark);

    if (ci->freeze)
        free(ci->freeze);

    if (ci->forbid)
        free(ci->forbid);

    if (ci->successor)
        free(ci->successor);

    if (ci->bot)
        free(ci->bot);

    if (ci->markreason)
        free(ci->markreason);

    if (ci->freezereason)
        free(ci->freezereason);

    if (ci->holdreason)
        free(ci->holdreason);

    if (ci->lastgetpass)
        free(ci->lastgetpass);

    free(ci);
    return 1;
}

void auspice_load_memo()
{
    FILE *f;
    int i, j;
    auspice_MemoList *ml;
    auspice_Memo *memos;

    if (!(f = auspice_open_db("MemoServ", MEMOSERV_DB, "r", 1)))
	return;
    switch ((size_t) i = auspice_get_file_version(f, MEMOSERV_DB)) {
      case 6:
      case 5:
      case 4:
      case 3:
      case 2:
      case 1:
	for (i = 33; i < 256; ++i) {
	    while (fgetc(f) == 1) {
		ml = (auspice_MemoList *) malloc(sizeof(auspice_MemoList));
		if (1 != fread(ml, sizeof(auspice_MemoList), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (MEMOSERV_DB));
		}

		ml->memos = memos = (auspice_Memo *) malloc(sizeof(auspice_Memo) * ml->n_memos);
		fread(memos, sizeof(auspice_Memo), ml->n_memos, f);
                for (j = 0; j < ml->n_memos; ++j, ++memos) {
		    memos->text = auspice_read_string(f, MEMOSERV_DB);
                  if (memos->chan)
                    memos->chan = auspice_read_string(f, MEMOSERV_DB);
                }

		MemoServ::nick_memo_t memo = Convert::auspice_CreateMemoEntry(ml);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);
		auspice_del_memolist(ml);
	    }
	}
	break;
      default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", ( i, MEMOSERV_DB));
    } /* switch (version) */
    auspice_close_db(f, MEMOSERV_DB);
}

/* del_memolist:  Remove a nick's memo list from the database.  Assumes
 *                that the memo count for the nick is non-zero.
 */

void auspice_del_memolist(auspice_MemoList * ml)
{
    BTCB();
    int i;
    auspice_Memo *memos;

    if (!ml)
	return;
    memos = ml->memos;
    for (i = 0; i < ml->n_memos; ++i, ++memos)
    {
	free(memos->text);
	if (memos->chan)
	  free(memos->chan);
    }
    if (ml->memos)
	free(ml->memos);
    free(ml);
    ETCB();
}

void auspice_load_msg()
{
    FILE *f;
    int i;
    auspice_Message *messages;
    int nmessage, message_size;

    if (!(f = auspice_open_db("Agent", MSG_DB, "r", 1)))
        return;

    switch (i = auspice_get_file_version(f, MSG_DB)) {
      case 6:
      case 5:
        nmessage = fgetc(f) * 256 + fgetc(f);
        if (nmessage < 8)
            message_size = 16;
        else
            message_size = 2*nmessage;
        messages = (auspice_Message *) malloc(sizeof(*messages) * message_size);
        if (!nmessage) {
            fclose(f);
            return;
        }

        if ((size_t) nmessage != fread(messages, sizeof(*messages), nmessage, f))
	{
            SLOG(LM_EMERGENCY, "Read error on $1", (MSG_DB));
	}
        for (i = 0; i < nmessage; i++) {
            messages[i].text = auspice_read_string(f, MSG_DB);
            messages[i].who = auspice_read_string(f, MSG_DB);

	    if (messages[i].type == auspice_M_LOGON && Magick::instance().commserv.IsList(Magick::instance().commserv.ALL_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.ALL_Name())->
		    MSG_insert(mstring(messages[i].text), mstring(messages[i].who));
	    }
	    else if (messages[i].type == auspice_M_OPER && Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->
		    MSG_insert(mstring(messages[i].text), mstring(messages[i].who));
	    }

	    free(messages[i].who);
	    free(messages[i].text);
        }
	free(messages);

        break;
      default:
        SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", ( i, MSG_DB));
    } /* switch (version) */

    auspice_close_db(f, MSG_DB);
}

void auspice_load_trigger()
{
    FILE *f;
    int i;
    auspice_Trigger *trigger;
    int ntrigger, trigger_size;

    if (!(f = auspice_open_db("TRIGGER", TRIGGER_DB, "r", 1)))
        return;

    switch (i = auspice_get_file_version(f, TRIGGER_DB)) {
      case 6:
      case 5:
        ntrigger = fgetc(f) * 256 + fgetc(f);
        if (ntrigger < 8)
            trigger_size = 16;
        else
            trigger_size = 2*ntrigger;
        trigger = (auspice_Trigger *) malloc(sizeof(*trigger) * trigger_size);
        if (!ntrigger) {
            fclose(f);
            return;
        }
        if ((size_t) ntrigger != fread(trigger, sizeof(*trigger), ntrigger, f))
	{
            SLOG(LM_EMERGENCY, "Read error on $1", (TRIGGER_DB));
	}
        for (i = 0; i < ntrigger; i++) {
            trigger[i].mask = auspice_read_string(f, TRIGGER_DB);
	    Magick::instance().operserv.Clone_insert(mstring(trigger[i].mask).After("@"), trigger[i].tvalue,
			"Converted from Auspice trigger.", mstring(trigger[i].who));
	    free(trigger[i].mask);
        }
	free(trigger);

        break;
      default:
        SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", ( i, TRIGGER_DB));
    } /* switch (version) */

    auspice_close_db(f, TRIGGER_DB);
}

void auspice_load_akill()
{
    FILE *f;
    int i;
    auspice_Akill *akills;
    int akill_size, nakill;

    if (!(f = auspice_open_db("AKILL", AKILL_DB, "r", 1)))
	return;

    switch (i = auspice_get_file_version(f, AKILL_DB)) {
      case 6:
      case 5:
      case 4:
      case 3:
	nakill = fgetc(f) * 256 + fgetc(f);
	if (nakill < 8)
	    akill_size = 16;
	else
	    akill_size = 2*nakill;
	akills = (auspice_Akill *) malloc(sizeof(*akills) * akill_size);
	if (!nakill) {
	    fclose(f);
	    return;
	}
	if ((size_t) nakill != fread(akills, sizeof(*akills), nakill, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", (AKILL_DB));
	}
	for (i = 0; i < nakill; i++) {
	    akills[i].mask = auspice_read_string(f, AKILL_DB);
	    akills[i].reason = auspice_read_string(f, AKILL_DB);

	    if (akills[i].expires == 0)
	    {
		Magick::instance().operserv.Akill_insert(mstring(akills[i].mask), Magick::instance().operserv.Expire_SAdmin(),
				mstring(akills[i].reason), mstring(akills[i].who), mDateTime(akills[i].time));
	    }
	    else
	    {
		Magick::instance().operserv.Akill_insert(mstring(akills[i].mask), akills[i].expires - akills[i].time,
				mstring(akills[i].reason), mstring(akills[i].who), mDateTime(akills[i].time));
	    }

	    free(akills[i].reason);
	    free(akills[i].mask);
	}
	free(akills);
	break;

      case 2: {
	struct {
	    char *mask;
	    char *reason;
	    char who[auspice_NICKMAX];
	    time_t time;
	} old_akill;
	nakill = fgetc(f) * 256 + fgetc(f);
	if (nakill < 8)
	    akill_size = 16;
	else
	    akill_size = 2*nakill;
	akills = (auspice_Akill *) malloc(sizeof(*akills) * akill_size);
	if (!nakill) {
	    fclose(f);
	    return;
	}
	for (i = 0; i < nakill; i++) {
	    if (1 != fread(&old_akill, sizeof(old_akill), 1, f))
	    {
		SLOG(LM_EMERGENCY, "Read error on $1", (AKILL_DB));
	    }
	    akills[i].time = old_akill.time;
	    strncpy(akills[i].who, old_akill.who, sizeof(akills[i].who));
	    akills[i].expires = 0;
	}
	for (i = 0; i < nakill; i++) {
	    akills[i].mask = auspice_read_string(f, AKILL_DB);
	    akills[i].reason = auspice_read_string(f, AKILL_DB);

	    Magick::instance().operserv.Akill_insert(mstring(akills[i].mask), Magick::instance().operserv.Def_Expire(),
				mstring(akills[i].reason), mstring(akills[i].who), mDateTime(akills[i].time));

	    free(akills[i].reason);
	    free(akills[i].mask);
	}
	free(akills);
	break;
      } /* case 2 */

      case 1: {
	struct {
	    char *mask;
	    char *reason;
	    time_t time;
	} old_akill;
	nakill = fgetc(f) * 256 + fgetc(f);
	if (nakill < 8)
	    akill_size = 16;
	else
	    akill_size = 2*nakill;
	akills = (auspice_Akill *) malloc(sizeof(*akills) * akill_size);
	if (!nakill) {
	    fclose(f);
	    return;
	}
	for (i = 0; i < nakill; i++) {
	    if (1 != fread(&old_akill, sizeof(old_akill), 1, f))
	    {
		SLOG(LM_EMERGENCY, "Read error on $1", (AKILL_DB));
	    }
	    akills[i].time = old_akill.time;
	    akills[i].who[0] = 0;
	    akills[i].expires = 0;
	}
	for (i = 0; i < nakill; i++) {
	    akills[i].mask = auspice_read_string(f, AKILL_DB);
	    akills[i].reason = auspice_read_string(f, AKILL_DB);

	    Magick::instance().operserv.Akill_insert(mstring(akills[i].mask), Magick::instance().operserv.Def_Expire(),
				mstring(akills[i].reason), Magick::instance().operserv.FirstName(), mDateTime(akills[i].time));

	    free(akills[i].reason);
	    free(akills[i].mask);
	}
	free(akills);
	break;
      } /* case 1 */

      default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", ( i, AKILL_DB));
    } /* switch (version) */

    auspice_close_db(f, AKILL_DB);
}

void auspice_load_ignore()
{
    FILE *f;
    int i;
    auspice_Ignore *ignores;
    int nignore, ignore_size;

    if (!(f = auspice_open_db("IGNORE", IGNORE_DB, "r", 1)))
	return;

    switch (i = auspice_get_file_version(f, IGNORE_DB)) {
      case 6:
      case 5:
      case 4:
      case 3:
        nignore = fgetc(f) * 256 + fgetc(f);
        if (nignore < 8)
            ignore_size = 16;
	else
            ignore_size = 2*nignore;
        ignores = (auspice_Ignore *) malloc(sizeof(*ignores) * ignore_size);
        if (!nignore) {
	    fclose(f);
	    return;
	}
        if ((size_t) nignore != fread(ignores, sizeof(*ignores), nignore, f))
	{
            SLOG(LM_EMERGENCY, "Read error on $1", (IGNORE_DB));
	}
        for (i = 0; i < nignore; i++)
	{
            ignores[i].mask = auspice_read_string(f, IGNORE_DB);

	    if (!ignores[i].expires)
		Magick::instance().operserv.Ignore_insert(mstring(ignores[i].mask), true,
			mstring(ignores[i].who), mDateTime(ignores[i].time));

	    free(ignores[i].mask);
	}
	free(ignores);
	break;

      case 2: {
	struct {
	    char *mask;
	    char who[auspice_NICKMAX];
	    time_t time;
        } old_ignore;
        nignore = fgetc(f) * 256 + fgetc(f);
        if (nignore < 8)
            ignore_size = 16;
	else
            ignore_size = 2*nignore;
        ignores = (auspice_Ignore *) malloc(sizeof(*ignores) * ignore_size);
        if (!nignore) {
	    fclose(f);
	    return;
	}
        for (i = 0; i < nignore; i++) {
            if (1 != fread(&old_ignore, sizeof(old_ignore), 1, f))
	    {
                SLOG(LM_EMERGENCY, "Read error on $1", (IGNORE_DB));
	    }
            ignores[i].time = old_ignore.time;
            strncpy(ignores[i].who, old_ignore.who, sizeof(ignores[i].who));
            ignores[i].expires = 0;
	}
        for (i = 0; i < nignore; i++)
	{
            ignores[i].mask = auspice_read_string(f, IGNORE_DB);

	    Magick::instance().operserv.Ignore_insert(mstring(ignores[i].mask), true,
			mstring(ignores[i].who), mDateTime(ignores[i].time));

	    free(ignores[i].mask);
	}
	free(ignores);
	break;
      } /* case 2 */

      case 1: {
	struct {
	    char *mask;
	    time_t time;
        } old_ignore;
        nignore = fgetc(f) * 256 + fgetc(f);
        if (nignore < 8)
            ignore_size = 16;
	else
            ignore_size = 2*nignore;
        ignores = (auspice_Ignore *) malloc(sizeof(*ignores) * ignore_size);
        if (!nignore) {
	    fclose(f);
	    return;
	}
        for (i = 0; i < nignore; i++) {
            if (1 != fread(&old_ignore, sizeof(old_ignore), 1, f))
	    {
                SLOG(LM_EMERGENCY, "Read error on $1", (IGNORE_DB));
	    }
            ignores[i].time = old_ignore.time;
            ignores[i].who[0] = 0;
            ignores[i].expires = 0;
	}
        for (i = 0; i < nignore; i++)
	{
            ignores[i].mask = auspice_read_string(f, IGNORE_DB);

	    Magick::instance().operserv.Ignore_insert(mstring(ignores[i].mask), true,
			Magick::instance().operserv.FirstName(), mDateTime(ignores[i].time));

	    free(ignores[i].mask);
	}
	free(ignores);
	break;
      } /* case 1 */

      default:
        SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", ( i, IGNORE_DB));
    } /* switch (version) */

    auspice_close_db(f, IGNORE_DB);
}

void auspice_load_admin()
{
    FILE *f = fopen(ADMINSERV_DB, "r");
    int i, j;
    auspice_AdminInfo *ais;

    if (!(f = auspice_open_db("AdminServ", ADMINSERV_DB, "r", 1)))
        return;

    switch (i = auspice_get_file_version(f, ADMINSERV_DB)) {
	case 1:
        for (i = 33; i < 256; ++i) {
            while (fgetc(f) == 1) {
                   ais = (auspice_AdminInfo *) malloc(sizeof(auspice_AdminInfo));
                   if (1 != fread(ais, sizeof(auspice_AdminInfo), 1, f))
		   {
                       SLOG(LM_EMERGENCY, "Read error on $1", (ADMINSERV_DB));
		   }

                if (ais->nick)
                    ais->nick = auspice_read_string(f, ADMINSERV_DB);

                if (ais->host)
                    ais->host = auspice_read_string(f, ADMINSERV_DB);
                if (ais->who)
                    ais->who = auspice_read_string(f, ADMINSERV_DB);
                if (ais->server)
                    ais->server = auspice_read_string(f, ADMINSERV_DB);
                if (ais->markline) {
                    char **adminmark;
                    adminmark = (char **) malloc(sizeof(char *) * ais->markline);
                    ais->mark = adminmark;
                    for (j = 0; j < ais->markline; ++j, ++adminmark)
                        *adminmark = auspice_read_string(f, ADMINSERV_DB);
                }

		// If they're a services op or admin in auspice, make them a sop in magick.
		if ((ais->adflags & auspice_ADF_SERVICEADMIN) &&
		    !(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		      Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(ais->nick)))
		    Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(
					mstring(ais->nick), mstring(ais->who), mDateTime(ais->added));
		else if ((ais->adflags & auspice_ADF_SERVICEOP) &&
		    !(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		     Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(ais->nick)) &&
		    !(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
		      Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->find(ais->nick)))
		    Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->insert(
					mstring(ais->nick), mstring(ais->who), mDateTime(ais->added));

		auspice_deladmin(ais);

            } /* while (fgetc(f) == 1) */
        } /* for (i) */
	break;
      default:
        SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", ( i, ADMINSERV_DB));
    } /* switch (version) */

    auspice_close_db(f, ADMINSERV_DB);
}

int auspice_deladmin(auspice_AdminInfo *ais)
{
    int i;
    if (ais->nick)
      free(ais->nick);
    if (ais->host)
      free(ais->host);
    if (ais->who)
      free(ais->who);
    if (ais->server)
      free(ais->server);
    if (ais->markline) {
        for (i = 0; i < ais->markline; i++) {
            if (ais->mark[i])
            free(ais->mark[i]);
        }
        free(ais->mark);
    }

    free(ais);
    return 1;
}


Nick_Stored_t *Convert::auspice_CreateNickEntry(auspice_NickInfo * ni)
{
    BTCB();
    int i;
    char **string;

    if (ni == NULL || ni->nick == NULL || !strlen(ni->nick))
	return NULL;

    if (ni->flags & auspice_NI_VERBOTEN)
    {
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick);

	return out;
    }
    else if (ni->flags & auspice_NI_SLAVE && ni->last_usermask != NULL)
    {
	Nick_Stored_t tmp(mstring(ni->last_usermask));
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
	if (ni->uin != NULL && strlen(ni->uin))
	    out->i_ICQ = mstring(ni->uin);
	if (ni->info != NULL && strlen(ni->info))
	    out->i_Description = mstring(ni->info);
	if (ni->last_quit != NULL && strlen(ni->last_quit))
	    out->i_LastQuit = mstring(ni->last_quit);
	for (i=0; i<ni->comline; i++)
	{
	    if (ni->comment[i] != NULL && strlen(ni->comment[i]))
	    if (out->i_Comment.length())
		out->i_Comment += "; ";
	    out->i_Comment += mstring(ni->comment[i]);
	}

	if (ni->last_realname != NULL && strlen(ni->last_realname))
	    out->i_LastRealName = mstring(ni->last_realname);
	if (ni->last_usermask != NULL && strlen(ni->last_usermask))
	    out->i_LastMask = mstring(ni->last_usermask);
	out->i_RegTime = mDateTime(ni->time_registered);
	out->i_LastSeenTime = mDateTime(ni->last_seen);

	for (i = 0, string = ni->access; i < ni->accesscount; ++i, ++string)
	{
	    out->i_access.insert(mstring(*string));
	}

	if (ni->flags & auspice_NI_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (ni->flags & auspice_NI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ni->flags & auspice_NI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ni->flags & auspice_NI_PRIVMSG && !out->L_PRIVMSG())
	    out->setting.PRIVMSG = true;
	if (ni->flags & auspice_NI_HOLD && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if (ni->flags & auspice_NI_NOMEMO && !out->L_NoMemo())
	    out->setting.NoMemo = true;

	if (ni->flags & auspice_NI_FREEZE)
	{
	    out->i_Suspend_By = Magick::instance().nickserv.FirstName();
	    out->i_Suspend_Time = mDateTime::CurrentDateTime();
	}

	return out;
    }
    ETCB();
}

Chan_Stored_t *Convert::Convert::auspice_CreateChanEntry(auspice_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name == NULL || !strlen(ci->name))
	return NULL;

    if (ci->flags & auspice_CI_VERBOTEN)
    {
	Chan_Stored_t *out = new Chan_Stored_t(mstring(ci->name));

	return out;
    }
    else
    {
	auspice_ChanAccess *i_access;
	auspice_AutoKick *akick;
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

	if (ACE_OS::strlen(ci->mlock_on) || ci->mlock_key != NULL || ci->mlock_limit)
	{
	    modelock << "+" << ci->mlock_on << (ci->mlock_key != NULL ? "k" : "") << (ci->mlock_limit ? "l" : "");
	}
	if (ACE_OS::strlen(ci->mlock_off))
	{
	    modelock << "-" << ci->mlock_off;
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

	long newlevel;
	float mod = (float) Magick::instance().chanserv.Level_Max() / (float) auspice_ACCESS_FOUNDER;

	for (i = 0, i_access = ci->access; i < ci->accesscount; ++i, ++i_access)
	{
	    if ((i_access->is_nick && i_access->name == NULL) ||
		(!i_access->is_nick && i_access->adder == NULL))
		continue;

	    if (i_access->level < 0)
		newlevel = -1;
	    else
		newlevel = (long) ((float) i_access->level * mod);
	    if (newlevel == 0)
		newlevel = 1;

	    out->Access_insert(i_access->is_nick ? i_access->name : i_access->adder,
			newlevel, Magick::instance().chanserv.FirstName());
	}

	for (i = 0, akick = ci->akick; i < ci->akickcount; ++i, ++akick)
	{
	    if (akick->name == NULL)
		continue;
	    if (akick->reason != NULL)
		out->Akick_insert(akick->name, akick->reason, Magick::instance().chanserv.FirstName());
	    else
		out->Akick_insert(akick->name, Magick::instance().chanserv.FirstName());
	}

	if (ci->welcome != NULL && strlen(ci->welcome))
	    out->Message_insert(mstring(ci->welcome), Magick::instance().chanserv.FirstName());

	if (ci->flags & auspice_CI_KEEPTOPIC && !out->L_Keeptopic())
	    out->setting.Keeptopic = true;
	if (ci->flags & auspice_CI_OPGUARD && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & auspice_CI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & auspice_CI_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & auspice_CI_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & auspice_CI_LEAVEOPS && !out->L_Anarchy())
	    out->setting.Anarchy = true;
	if (ci->flags & auspice_CI_IDENT && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & auspice_CI_HELDCHAN && !Magick::instance().chanserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if ((ci->flags & auspice_CI_CHANJOIN ||
	     ci->botflag & auspice_CBI_BOTSTAY) && !out->L_Join())
	    out->setting.Join = true;
	if (ci->ttb && !out->L_Bantime())
	    out->setting.Bantime = ci->ttb;
	if ((ci->botflag & auspice_CBI_PROTOP ||
	     ci->botflag & auspice_CBI_PROTECT) && !out->L_Revenge())
	    out->setting.Revenge = "REVERSE";

	if (ci->last_topic != NULL && strlen(ci->last_topic))
	    out->i_Topic = mstring(ci->last_topic);
	if (ci->last_topic_setter != NULL && strlen(ci->last_topic_setter))
	    out->i_Topic_Setter = mstring(ci->last_topic_setter);
	out->i_Topic_Set_Time = mDateTime(ci->last_topic_time);

	if (ci->flags & auspice_CI_FREEZECHAN)
	{
	    if (ci->freezereason != NULL && strlen(ci->freezereason))
		out->i_Comment = mstring(ci->freezereason);
	    if (ci->freeze != NULL && strlen(ci->freeze))
		out->i_Suspend_By = mstring(ci->freeze);
	    out->i_Suspend_Time = mDateTime::CurrentDateTime();
	}

	if (ci->levels != NULL)
	{
	    for (i = 0; i < auspice_CA_SIZE; ++i)
	    {
		if (ci->levels[i] == auspice_ACCESS_INVALID)
		    newlevel = Magick::instance().chanserv.Level_Max() + 2;
		else if (ci->levels[i] == auspice_ACCESS_FOUNDER)
		    newlevel = Magick::instance().chanserv.Level_Max() + 1;
		else if (ci->levels[i] < 0)
		    newlevel = -1;
		else
		    newlevel = (long) ((float) ci->levels[i] * mod);

		switch (i)
		{
		case auspice_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_AUTOHOP:
		    out->Level_change("AUTOHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_UNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_ACCESS_CHANGE:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_ACCESS_LIST:
		    out->Level_change("VIEW", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_INVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_OPDEOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case auspice_CA_CLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}
	if (ci->flags & auspice_CI_AUTOVOP)
	    out->Level_change("AUTOVOICE", 0, Magick::instance().chanserv.FirstName());
	if (ci->flags & auspice_CI_AUTOHOP)
	    out->Level_change("AUTOHALFOP", 0, Magick::instance().chanserv.FirstName());
	if (ci->flags & auspice_CI_AUTOOP)
	    out->Level_change("AUTOOP", 0, Magick::instance().chanserv.FirstName());

	if (ci->flags & auspice_CI_MEMO_AV)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("AUTOVOICE"),
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & auspice_CI_MEMO_HOP)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("AUTOHALFOP"),
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & auspice_CI_MEMO_AOP)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("AUTOOP"),
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & auspice_CI_MEMO_SOP)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("SUPER"),
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & auspice_CI_MEMO_CF)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.Level_Max(),
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & auspice_CI_MEMO_FR)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.Level_Max() + 1,
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & auspice_CI_MEMO_NONE)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.Level_Max() + 2,
		Magick::instance().chanserv.FirstName());

	return out;
    }
    ETCB();
}

MemoServ::nick_memo_t Convert::auspice_CreateMemoEntry(auspice_MemoList * ml)
{
    BTCB();
    int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;
    auspice_Memo *memos;

    memos = ml->memos;
    for (i = 0; i < ml->n_memos; ++i, ++memos)
    {
	if (memos->text == NULL)
	    continue;

	tmp = new Memo_t(mstring(ml->nick), mstring(memos->sender), mstring(memos->text));
	tmp->i_Time = mDateTime(memos->time);
	if (!(memos->flags & auspice_MF_UNREAD))
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}

MemoServ::channel_news_t Convert::auspice_CreateNewsEntry(size_t count, char **list, const char *chan)
{
    BTCB();
    size_t i;

    MemoServ::channel_news_t out;
    News_t *tmp;

    for (i = 0; i < count; ++i)
    {
	if (list[i] == NULL)
	    continue;

	tmp = new News_t(mstring(chan), Magick::instance().chanserv.FirstName(), mstring(list[i]));
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}


#endif /* CONVERT */
