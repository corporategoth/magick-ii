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
#define RCSID(x,y) const char *rcsid_convert_wrecked_cpp_ ## x () { return y; }
RCSID(convert_wrecked_cpp, "@(#)$Id$");

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

/* Based upon the DB loading routines from Wrecked 1.2.0
 * (c) 2000 Darcy Grexton <skold@wrecked.net>
 */

#include "magick.h"
#include "convert/interface.h"
#include "convert/wrecked.h"

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

/* Maximum length of a channel name */
#define	wrecked_CHANMAX		64

/* Maximum length of a nickname */
#define	wrecked_NICKMAX		32

/* Maximum length of a password */
#define	wrecked_PASSMAX		32

/* Maximum number of valid +/- Modes */
#define wrecked_MODEMAX		64

/* Nickname info structure.  Each nick structure is stored in one of 256
 * lists; the list is determined by the first character of the nick.  Nicks
 * are stored in alphabetical order within lists. */

#define	wrecked_NI_KILLPROTECT	0x00000001	/* Kill others who take this nick */
#define	wrecked_NI_SECURE	0x00000002	/* Don't recognize unless IDENTIFY'd */
#define	wrecked_NI_VERBOTEN	0x00000004	/* Nick may not be registered or used */
#define	wrecked_NI_IRCOP		0x00000008	/* IrcOP - Nick will not expire */
#define	wrecked_NI_PRIVATE	0x00000010	/* Private - Dont show up in list */
#define	wrecked_NI_SUSPENDED	0x00000020	/* Suspended - May not IDENTIFY */
#define	wrecked_NI_PRIVMSG	0x00000040	/* use PRIVMSG instead of NOTICE */
#define wrecked_NI_SLAVE		0x00000080	/* Nick is just a 'linked' nick */

#define	wrecked_NI_IDENTIFIED	0x80000000	/* User has IDENTIFY'd */
#define	wrecked_NI_RECOGNIZED	0x40000000	/* User comes from a known addy */
#define	wrecked_NI_KILL_HELD	0x20000000	/* Nick is being held after a kill */

/*************************************************************************/

/* Channel info structures.  Stored similarly to the nicks, except that
 * the second character of the channel name, not the first, is used to
 * determine the list.  (Hashing based on the first character of the name
 * wouldn't get very far. ;) ) */

/* Retain topic even after last person leaves channel */
#define	wrecked_CI_KEEPTOPIC	0x00000001

/* Don't allow non-authorized users to be opped */
#define	wrecked_CI_SECUREOPS	0x00000002

/* Hide channel from ChanServ LIST command */
#define	wrecked_CI_PRIVATE	0x00000004

/* Topic can only be changed by SET TOPIC */
#define	wrecked_CI_TOPICLOCK	0x00000008

/* Those not allowed ops are kickbanned */
#define	wrecked_CI_RESTRICTED	0x00000010

/* Don't auto-deop anyone */
#define	wrecked_CI_LEAVEOPS	0x00000020

/* Don't allow any privileges unless a user is IDENTIFY'd with NickServ */
#define	wrecked_CI_SECURE	0x00000040

/* Don't allow the channel to be registered or used */
#define	wrecked_CI_VERBOTEN	0x00000080

/* Dont honour channel access list or founder */
#define	wrecked_CI_SUSPENDED	0x00000100

/* ChanServ joins channel when its established */
#define	wrecked_CI_JOIN		0x00000200

/* Revenge flags */
#define	wrecked_CI_REV1		0x80000000
#define	wrecked_CI_REV2		0x40000000
#define	wrecked_CI_REV3		0x20000000

/* Revenge levels */
#define	wrecked_CR_NONE		0
#define	wrecked_CR_REVERSE	1
#define	wrecked_CR_DEOP		2
#define	wrecked_CR_KICK		3
#define	wrecked_CR_NICKBAN	4
#define	wrecked_CR_USERBAN	5
#define	wrecked_CR_HOSTBAN	6
#define	wrecked_CR_MIRROR	7

/* Indices for cmd_access[]: */
#define	wrecked_CA_AUTODEOP	0
#define	wrecked_CA_AUTOVOICE	1
#define	wrecked_CA_AUTOOP	2
#define	wrecked_CA_READMEMO	3
#define	wrecked_CA_WRITEMEMO	4
#define	wrecked_CA_DELMEMO	5
#define	wrecked_CA_AKICK	6
#define	wrecked_CA_STARAKICK	7
#define	wrecked_CA_UNBAN	8
#define	wrecked_CA_ACCESS	9
#define	wrecked_CA_SET		10	/* NOT FOUNDER and PASSWORD */

#define	wrecked_CA_CMDINVITE	11
#define	wrecked_CA_CMDUNBAN	12
#define	wrecked_CA_CMDVOICE	13
#define	wrecked_CA_CMDOP	14
#define	wrecked_CA_CMDCLEAR	15

#define	wrecked_CA_FLOOR	16
#define	wrecked_CA_CAP		17
#define	wrecked_CA_FOUNDER	18

#define	wrecked_CA_SIZE		19	/* <--- DO NOT DELETE */

/*************************************************************************/

/* MemoServ data.  Only nicks that actually have memos get records in
 * MemoServ's lists, which are stored the same way NickServ's are. */

#define	wrecked_MS_READ		0x00000001	/* Yes, I had planned to do this */
#define wrecked_MS_DELETED	0x00000002

#define	wrecked_M_LOGON	0
#define	wrecked_M_OPER	1

struct wrecked_NickInfo_V1
{
    wrecked_NickInfo_V1 *next, *prev;
    char nick[wrecked_NICKMAX];
    char pass[wrecked_PASSMAX];
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;		/* # of entries */
    char **access;		/* Array of strings */
    long flags;			/* See below */
    long reserved[4];		/* For future expansion -- set to 0 */
};

struct wrecked_NickInfo_V3
{
    wrecked_NickInfo_V3 *next, *prev;
    char nick[wrecked_NICKMAX];
    char pass[wrecked_PASSMAX];
    char *email;
    char *url;
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;		/* # of entries */
    char **access;		/* Array of strings */
    long ignorecount;		/* # of entries */
    char **ignore;		/* Array of strings */
    long flags;			/* See below */
    long reserved[4];		/* For future expansion -- set to 0 */
};

struct wrecked_NickInfo_V5
{
    wrecked_NickInfo_V5 *next, *prev;
    char nick[wrecked_NICKMAX];
    char pass[wrecked_PASSMAX];
    char *email;
    char *url;
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;		/* # of entries */
    char **access;		/* Array of strings */
    long ignorecount;		/* # of entries */
    char **ignore;		/* Array of strings */
    long flags;			/* See below */
    long reserved[4];		/* For future expansion -- set to 0 */
};

struct wrecked_NickInfo
{
    wrecked_NickInfo *next, *prev;
    char nick[wrecked_NICKMAX];
    char pass[wrecked_PASSMAX];
    char *email;
    char *url;
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;		/* # of entries */
    char **access;		/* Array of strings */
    long ignorecount;		/* # of entries */
    char **ignore;		/* Array of strings */
    long flags;			/* See below */
    long reserved[4];		/* For future expansion -- set to 0 */
    time_t last_signon;
    long uin;
};

/****************************************************************************
 * chan.db
 ****************************************************************************/

struct wrecked_ChanAccess
{
    short level;
    short is_nick;		/* 1 if this is a nick, 0 if a user@host mask.  If
				 * -1, then this entry is currently unused (a hack
				 * to get numbered lists to have consistent
				 * numbering). */
    char *name;
};

struct wrecked_AutoKick
{
    short is_nick;
    short pad;
    char *name;
    char *reason;
};

struct wrecked_ChanInfo_V1
{
    wrecked_ChanInfo_V1 *next, *prev;
    char name[wrecked_CHANMAX];
    char founder[wrecked_NICKMAX];	/* Always a reg'd nick */
    char founderpass[wrecked_PASSMAX];
    char *desc;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    wrecked_ChanAccess *access;		/* List of authorized users */
    long akickcount;
    wrecked_AutoKick *akick;
    short mlock_on, mlock_off;	/* See channel modes below */
    long mlock_limit;		/* 0 if no limit */
    char *mlock_key;		/* NULL if no key */
    char *last_topic;		/* Last topic on the channel */
    char last_topic_setter[wrecked_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;	/* When the last topic was set */
    long flags;			/* See below */
    short *cmd_access;		/* Access levels for commands */
    long reserved[3];		/* For future expansion -- set to 0 */
};

struct wrecked_ChanInfo_V3
{
    wrecked_ChanInfo_V3 *next, *prev;
    char name[wrecked_CHANMAX];
    char founder[wrecked_NICKMAX];	/* Always a reg'd nick */
    char founderpass[wrecked_PASSMAX];
    char *desc;
    char *url;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    wrecked_ChanAccess *access;		/* List of authorized users */
    long akickcount;
    wrecked_AutoKick *akick;
    short mlock_on, mlock_off;	/* See channel modes below */
    long mlock_limit;		/* 0 if no limit */
    char *mlock_key;		/* NULL if no key */
    char *last_topic;		/* Last topic on the channel */
    char last_topic_setter[wrecked_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;	/* When the last topic was set */
    long flags;			/* See below */
    short *cmd_access;		/* Access levels for commands */
    long reserved[3];		/* For future expansion -- set to 0 */
};

struct wrecked_ChanInfo
{
    wrecked_ChanInfo *next, *prev;
    char name[wrecked_CHANMAX];
    char founder[wrecked_NICKMAX];	/* Always a reg'd nick */
    char founderpass[wrecked_PASSMAX];
    char *desc;
    char *url;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    wrecked_ChanAccess *access;		/* List of authorized users */
    long akickcount;
    wrecked_AutoKick *akick;
    char mlock_on[64], mlock_off[64];	/* See channel modes below */
    long mlock_limit;		/* 0 if no limit */
    char *mlock_key;		/* NULL if no key */
    char *last_topic;		/* Last topic on the channel */
    char last_topic_setter[wrecked_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;	/* When the last topic was set */
    long flags;			/* See below */
    short *cmd_access;		/* Access levels for commands */
    long reserved[3];		/* For future expansion -- set to 0 */
};

/****************************************************************************
 * memo.db and news.db
 ****************************************************************************/

struct wrecked_Memo
{
    char sender[wrecked_NICKMAX];
    long number;		/* Index number -- not necessarily array position! */
    time_t time;		/* When it was sent */
    char *text;
    long reserved[4];		/* For future expansion -- set to 0 */
};

struct wrecked_MemoList
{
    wrecked_MemoList *next, *prev;
    char nick[wrecked_NICKMAX];		/* Owner of the memos */
    long n_memos;		/* Number of memos */
    wrecked_Memo *memos;		/* The memos themselves */
    long reserved[4];		/* For future expansion -- set to 0 */
};

struct wrecked_NewsList
{
    wrecked_NewsList *next, *prev;
    char chan[wrecked_CHANMAX];		/* Owner of the memos */
    long n_newss;		/* Number of memos */
    wrecked_Memo *newss;		/* The memos themselves */
    long reserved[4];		/* For future expansion -- set to 0 */
};

/****************************************************************************
 * others
 ****************************************************************************/

struct wrecked_Message
{
    char *text;
    short type;
    char who[wrecked_NICKMAX];
    time_t time;
};

struct wrecked_Akill_V1
{
    char *mask;
    char *reason;
    time_t time;
};

struct wrecked_Akill
{
    char *mask;
    char *reason;
    char who[wrecked_NICKMAX];
    time_t time;
};

struct wrecked_Allow
{
    char *host;
    int amount;
    char *reason;
    char who[wrecked_NICKMAX];
    time_t time;
};

struct wrecked_Sop
{
    char nick[wrecked_NICKMAX];
};

const int wrecked_file_version = 6;
const char *wrecked_nickserv_db = "nick.db";
const char *wrecked_chanserv_db = "chan.db";
const char *wrecked_memoserv_db = "memo.db";
const char *wrecked_newsserv_db = "news.db";
const char *wrecked_sop_db = "sop.db";
const char *wrecked_message_db = "message.db";
const char *wrecked_akill_db = "akill.db";
const char *wrecked_clone_db = "clone.db";

int wrecked_get_file_version(FILE * f, const char *filename)
{
    BTCB();
    int version = fgetc(f) << 24 | fgetc(f) << 16 | fgetc(f) << 8 | fgetc(f);

    if (ferror(f) || feof(f))
    {
	SLOG(LM_EMERGENCY, "Error reading version number on $1", (filename));
    }
    else if (version > wrecked_file_version || version < 1)
    {
	SLOG(LM_EMERGENCY, "Invalid version number ($1) on $2", (version, filename));
    }
    return version;
    ETCB();
}

char *wrecked_read_string(FILE * f, const char *filename)
{
    BTCB();
    char *s;
    unsigned int len;

    len = fgetc(f) * 256 + fgetc(f);
    s = (char *) malloc(len);
    if (len != ACE_OS::fread(s, 1, len, f))
    {
	SLOG(LM_EMERGENCY, "Read error on $1", (filename));
    }
    return s;
    ETCB();
}

void wrecked_delnick(wrecked_NickInfo * ni);
void wrecked_delchan(wrecked_ChanInfo * ci);
char *wrecked_oldmodeconv(short inmode);
void wrecked_del_memolist(wrecked_MemoList * ml);
void wrecked_del_newslist(wrecked_NewsList * nl);


void wrecked_load_nick()
{
    BTCB();
    FILE *f = ACE_OS::fopen(wrecked_nickserv_db, "r");
    int i, j;
    wrecked_NickInfo *ni;
    Nick_Stored_t *nick;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read NickServ database $1", (wrecked_nickserv_db));
	return;
    }
    switch (i = wrecked_get_file_version(f, wrecked_nickserv_db))
    {
    case 6:
	for (i = 33; i < 256; ++i)
	    while (fgetc(f) == 1)
	    {
		ni = (wrecked_NickInfo *) malloc(sizeof(wrecked_NickInfo));
		memset(ni, 0, sizeof(wrecked_NickInfo));
		if (1 != ACE_OS::fread(ni, sizeof(wrecked_NickInfo), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_nickserv_db));
		}
		ni->flags &= ~(wrecked_NI_IDENTIFIED | wrecked_NI_RECOGNIZED);
		if (ni->email)
		{
		    ni->email = wrecked_read_string(f, wrecked_nickserv_db);
		    if (!ACE_OS::strlen(ni->email))
		    {
			free(ni->email);
			ni->email = NULL;
		    }
		}
		else
		    ni->email = NULL;
		if (ni->url)
		{
		    ni->url = wrecked_read_string(f, wrecked_nickserv_db);
		    if (!ACE_OS::strlen(ni->url))
		    {
			free(ni->url);
			ni->url = NULL;
		    }
		}
		else
		    ni->url = NULL;
		ni->last_usermask = wrecked_read_string(f, wrecked_nickserv_db);
		ni->last_realname = wrecked_read_string(f, wrecked_nickserv_db);
		if (ni->accesscount)
		{
		    char **i_access;
		    i_access = (char **) malloc(sizeof(char *) * ni->accesscount);
		    ni->access = i_access;
		    for (j = 0; j < ni->accesscount; ++j, ++i_access)
			*i_access = wrecked_read_string(f, wrecked_nickserv_db);
		}
		if (ni->ignorecount)
		{
		    char **ignore;
		    ignore = (char **) malloc(sizeof(char *) * ni->ignorecount);
		    ni->ignore = ignore;
		    for (j = 0; j < ni->ignorecount; ++j, ++ignore)
			*ignore = wrecked_read_string(f, wrecked_nickserv_db);
		}

		nick = Convert::wrecked_CreateNickEntry(ni);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);
		wrecked_delnick(ni);
	    }
    case 5:
    case 4:
	{
	    wrecked_NickInfo_V5 *old_ni;

	    for (i = 33; i < 256; ++i)
		while (fgetc(f) == 1)
		{
		    ni = (wrecked_NickInfo *) malloc(sizeof(wrecked_NickInfo));
		    old_ni = (wrecked_NickInfo_V5 *) malloc(sizeof(wrecked_NickInfo_V5));
		    memset(ni, 0, sizeof(wrecked_NickInfo));
		    memset(ni, 0, sizeof(wrecked_NickInfo_V5));
		    if (1 != ACE_OS::fread(old_ni, sizeof(wrecked_NickInfo_V5), 1, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_nickserv_db));
		    }
		    ACE_OS::strcpy(ni->nick, old_ni->nick);
		    ACE_OS::strcpy(ni->pass, old_ni->pass);
		    ni->time_registered = old_ni->time_registered;
		    ni->last_seen = old_ni->last_seen;
		    ni->accesscount = old_ni->accesscount;
		    ni->ignorecount = old_ni->ignorecount;
		    ni->flags = old_ni->flags;
		    ni->flags &= ~(wrecked_NI_IDENTIFIED | wrecked_NI_RECOGNIZED);
		    if (ni->email)
		    {
			ni->email = wrecked_read_string(f, wrecked_nickserv_db);
			if (!ACE_OS::strlen(ni->email))
			{
			    free(ni->email);
			    ni->email = NULL;
			}
		    }
		    else
			ni->email = NULL;
		    if (ni->url)
		    {
			ni->url = wrecked_read_string(f, wrecked_nickserv_db);
			if (!ACE_OS::strlen(ni->url))
			{
			    free(ni->url);
			    ni->url = NULL;
			}
		    }
		    else
			ni->url = NULL;
		    ni->last_usermask = wrecked_read_string(f, wrecked_nickserv_db);
		    ni->last_realname = wrecked_read_string(f, wrecked_nickserv_db);
		    if (ni->accesscount)
		    {
			char **i_access;
			i_access = (char **) malloc(sizeof(char *) * ni->accesscount);
			ni->access = i_access;
			for (j = 0; j < ni->accesscount; ++j, ++i_access)
			    *i_access = wrecked_read_string(f, wrecked_nickserv_db);
		    }
		    if (ni->ignorecount)
		    {
			char **ignore;
			ignore = (char **) malloc(sizeof(char *) * ni->ignorecount);
			ni->ignore = ignore;
			for (j = 0; j < ni->ignorecount; ++j, ++ignore)
			    *ignore = wrecked_read_string(f, wrecked_nickserv_db);
		    }

		    nick = Convert::wrecked_CreateNickEntry(ni);
		    if (nick != NULL)
			Magick::instance().nickserv.AddStored(nick);
		    wrecked_delnick(ni);
		}
	    break;
	}
    case 3:
	{
	    wrecked_NickInfo_V3 *old_ni;

	    for (i = 33; i < 256; ++i)
		while (fgetc(f) == 1)
		{
		    ni = (wrecked_NickInfo *) malloc(sizeof(wrecked_NickInfo));
		    old_ni = (wrecked_NickInfo_V3 *) malloc(sizeof(wrecked_NickInfo_V3));
		    memset(ni, 0, sizeof(wrecked_NickInfo));
		    memset(old_ni, 0, sizeof(wrecked_NickInfo_V3));
		    if (1 != ACE_OS::fread(old_ni, sizeof(wrecked_NickInfo_V3), 1, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_nickserv_db));
		    }
		    ACE_OS::strcpy(ni->nick, old_ni->nick);
		    ACE_OS::strcpy(ni->pass, old_ni->pass);
		    ni->time_registered = old_ni->time_registered;
		    ni->last_seen = old_ni->last_seen;
		    ni->accesscount = old_ni->accesscount;
		    ni->ignorecount = old_ni->ignorecount;
		    ni->flags = old_ni->flags;
		    ni->flags &= ~(wrecked_NI_IDENTIFIED | wrecked_NI_RECOGNIZED);

		    if (old_ni->email)
		    {
			ni->email = wrecked_read_string(f, wrecked_nickserv_db);
			if (!ACE_OS::strlen(ni->email))
			{
			    free(ni->email);
			    ni->email = NULL;
			}
		    }
		    else
			ni->email = NULL;

		    if (old_ni->url)
		    {
			ni->url = wrecked_read_string(f, wrecked_nickserv_db);
			if (!ACE_OS::strlen(old_ni->url))
			{
			    free(ni->url);
			    ni->url = NULL;
			}
		    }
		    else
			ni->url = NULL;
		    old_ni->last_usermask = wrecked_read_string(f, wrecked_nickserv_db);
		    ni->last_usermask = old_ni->last_usermask;
		    old_ni->last_realname = wrecked_read_string(f, wrecked_nickserv_db);
		    ni->last_realname = old_ni->last_realname;
		    free(old_ni);

		    if (ni->accesscount)
		    {
			char **i_access;
			i_access = (char **) malloc(sizeof(char *) * ni->accesscount);
			ni->access = i_access;
			for (j = 0; j < ni->accesscount; ++j, ++i_access)
			    *i_access = wrecked_read_string(f, wrecked_nickserv_db);
		    }

		    nick = Convert::wrecked_CreateNickEntry(ni);
		    if (nick != NULL)
			Magick::instance().nickserv.AddStored(nick);
		    wrecked_delnick(ni);
		}
	    break;
	}
    case 2:
    case 1:
	{
	    wrecked_NickInfo_V1 *old_ni;

	    for (i = 33; i < 256; ++i)
		while (fgetc(f) == 1)
		{
		    ni = (wrecked_NickInfo *) malloc(sizeof(wrecked_NickInfo));
		    old_ni = (wrecked_NickInfo_V1 *) malloc(sizeof(wrecked_NickInfo_V1));
		    memset(ni, 0, sizeof(wrecked_NickInfo));
		    memset(old_ni, 0, sizeof(wrecked_NickInfo_V1));
		    if (1 != ACE_OS::fread(ni, sizeof(wrecked_NickInfo_V1), 1, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_nickserv_db));
		    }
		    ACE_OS::strcpy(ni->nick, old_ni->nick);
		    ACE_OS::strcpy(ni->pass, old_ni->pass);
		    ni->time_registered = old_ni->time_registered;
		    ni->last_seen = old_ni->last_seen;
		    ni->accesscount = old_ni->accesscount;
		    ni->flags = old_ni->flags;
		    ni->flags &= ~(wrecked_NI_IDENTIFIED | wrecked_NI_RECOGNIZED);

		    old_ni->last_usermask = wrecked_read_string(f, wrecked_nickserv_db);
		    ni->last_usermask = old_ni->last_usermask;
		    old_ni->last_realname = wrecked_read_string(f, wrecked_nickserv_db);
		    ni->last_realname = old_ni->last_realname;
		    free(old_ni);

		    if (ni->accesscount)
		    {
			char **i_access;
			i_access = (char **) malloc(sizeof(char *) * ni->accesscount);
			ni->access = i_access;
			for (j = 0; j < ni->accesscount; ++j, ++i_access)
			    *i_access = wrecked_read_string(f, wrecked_nickserv_db);
		    }

		    nick = Convert::wrecked_CreateNickEntry(ni);
		    if (nick != NULL)
			Magick::instance().nickserv.AddStored(nick);
		    wrecked_delnick(ni);
		}
	    break;
	}
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", (i, wrecked_nickserv_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

/* Remove a nick from the NickServ database.  Return 1 on success, 0
 * otherwise. */

void wrecked_delnick(wrecked_NickInfo * ni)
{
    BTCB();
    int i;

    if (ni->email)
	free(ni->email);
    if (ni->url)
	free(ni->url);
    if (ni->last_usermask)
	free(ni->last_usermask);
    if (ni->last_realname)
	free(ni->last_realname);
    if (ni->access)
    {
	for (i = 0; i < ni->accesscount; ++i)
	    free(ni->access[i]);
	free(ni->access);
    }
    if (ni->ignore)
    {
	for (i = 0; i < ni->ignorecount; ++i)
	    free(ni->ignore[i]);
	free(ni->ignore);
    }
    free(ni);
    ni = NULL;
    ETCB();
}

Nick_Stored_t *Convert::wrecked_CreateNickEntry(wrecked_NickInfo * ni)
{
    BTCB();
    int i;
    char **string;

    if (ni == NULL || ni->nick == NULL || !strlen(ni->nick))
	return NULL;

    if (ni->flags & wrecked_NI_VERBOTEN)
    {
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick);

	return out;
    }
    else if (ni->flags & wrecked_NI_SLAVE && ni->last_usermask != NULL)
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
	if (ni->uin)
	    out->i_ICQ = mstring(ni->uin);
	if (ni->last_realname != NULL && strlen(ni->last_realname))
	    out->i_LastRealName = mstring(ni->last_realname);
	out->i_RegTime = mDateTime(ni->time_registered);
	out->i_LastSeenTime = mDateTime(ni->last_seen);
	for (i = 0, string = ni->access; i < ni->accesscount; ++i, ++string)
	{
	    out->i_access.insert(mstring(*string));
	}
	for (i = 0, string = ni->ignore; i < ni->ignorecount; ++i, ++string)
	{
	    out->i_ignore.insert(mstring(*string));
	}

	if (ni->flags & wrecked_NI_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (ni->flags & wrecked_NI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ni->flags & wrecked_NI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ni->flags & wrecked_NI_PRIVMSG && !out->L_PRIVMSG())
	    out->setting.PRIVMSG = true;

	if (ni->flags & wrecked_NI_SUSPENDED)
	{
	    out->i_Suspend_By = Magick::instance().nickserv.FirstName();
	    out->i_Suspend_Time = mDateTime::CurrentDateTime();
	    if (ni->last_usermask != NULL && strlen(ni->last_usermask))
		out->i_Comment = mstring(ni->last_usermask);
	}
	else
	{
	    if (ni->last_usermask != NULL && strlen(ni->last_usermask))
		out->i_LastMask = mstring(ni->last_usermask);
	}

	if (ni->flags & wrecked_NI_IRCOP)
	{
	    out->setting.NoExpire = true;
	    // NOT a SADMIN, and OPER does exist.
	    if (!
		(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(out->i_Name)) &&
		!(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->find(out->i_Name)) &&
		Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->insert(mstring(out->i_Name),
												     Magick::instance().
												     commserv.FirstName());
	}

	return out;
    }
    ETCB();
}

void wrecked_load_chan()
{
    BTCB();
    FILE *f = ACE_OS::fopen(wrecked_chanserv_db, "r");
    int i, j;
    wrecked_ChanInfo *ci;
    Chan_Stored_t *chan;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read ChanServ database $1", (wrecked_chanserv_db));
	return;
    }

    switch (i = wrecked_get_file_version(f, wrecked_chanserv_db))
    {
    case 6:
    case 5:
	for (i = 33; i < 256; ++i)
	    while (fgetc(f) == 1)
	    {
		ci = (wrecked_ChanInfo *) malloc(sizeof(wrecked_ChanInfo));
		memset(ci, 0, sizeof(wrecked_ChanInfo));
		if (1 != ACE_OS::fread(ci, sizeof(wrecked_ChanInfo), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
		}
		ci->desc = wrecked_read_string(f, wrecked_chanserv_db);
		if (ci->url)
		    ci->url = wrecked_read_string(f, wrecked_chanserv_db);
		if (ci->mlock_key)
		    ci->mlock_key = wrecked_read_string(f, wrecked_chanserv_db);
		if (ci->last_topic)
		    ci->last_topic = wrecked_read_string(f, wrecked_chanserv_db);

		if (ci->accesscount)
		{
		    wrecked_ChanAccess *i_access;

		    i_access = (wrecked_ChanAccess *) malloc(sizeof(wrecked_ChanAccess) * ci->accesscount);
		    ci->access = i_access;
		    if (ci->accesscount != (long) ACE_OS::fread(i_access, sizeof(wrecked_ChanAccess), ci->accesscount, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
		    }
		    for (j = 0; j < ci->accesscount; ++j, ++i_access)
			i_access->name = wrecked_read_string(f, wrecked_chanserv_db);
		    j = 0;
		    i_access = ci->access;
		    /* Clear out unused entries */
		    while (j < ci->accesscount)
		    {
			if (i_access->is_nick < 0)
			{
			    --ci->accesscount;
			    free(i_access->name);
			    if (j < ci->accesscount)
				memcpy(i_access, i_access + 1, sizeof(*i_access) * (ci->accesscount - j));
			}
			else
			{
			    ++j;
			    ++i_access;
			}
		    }
		    if (ci->accesscount)
			ci->access = (wrecked_ChanAccess *) realloc(ci->access, sizeof(wrecked_ChanAccess) * ci->accesscount);
		    else
		    {
			free(ci->access);
			ci->access = NULL;
		    }
		}		/* if (ci->accesscount) */

		if (ci->akickcount)
		{
		    wrecked_AutoKick *akick;

		    akick = (wrecked_AutoKick *) malloc(sizeof(wrecked_AutoKick) * ci->akickcount);
		    ci->akick = akick;
		    if (ci->akickcount != (long) ACE_OS::fread(akick, sizeof(wrecked_AutoKick), ci->akickcount, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
		    }
		    for (j = 0; j < ci->akickcount; ++j, ++akick)
		    {
			akick->name = wrecked_read_string(f, wrecked_chanserv_db);
			if (akick->reason)
			    akick->reason = wrecked_read_string(f, wrecked_chanserv_db);
		    }
		    j = 0;
		    akick = ci->akick;
		    while (j < ci->akickcount)
		    {
			if (akick->is_nick < 0)
			{
			    --ci->akickcount;
			    free(akick->name);
			    if (akick->reason)
				free(akick->reason);
			    if (j < ci->akickcount)
				memcpy(akick, akick + 1, sizeof(*akick) * (ci->akickcount - j));
			}
			else
			{
			    ++j;
			    ++akick;
			}
		    }
		    if (ci->akickcount)
		    {
			ci->akick = (wrecked_AutoKick *) realloc(ci->akick, sizeof(wrecked_AutoKick) * ci->akickcount);
		    }
		    else
		    {
			free(ci->akick);
			ci->akick = NULL;
		    }
		}		/* if (ci->akickcount) */

		if (ci->cmd_access)
		{
		    int n_entries;
		    ci->cmd_access = (short *) malloc(sizeof(short) * wrecked_CA_SIZE);
		    n_entries = fgetc(f) << 8 | fgetc(f);
		    if (n_entries < 0)
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
		    }
		    if (n_entries <= wrecked_CA_SIZE)
		    {
			ACE_OS::fread(ci->cmd_access, sizeof(short), n_entries, f);
		    }
		    else
		    {
			ACE_OS::fread(ci->cmd_access, sizeof(short), wrecked_CA_SIZE, f);
			ACE_OS::fseek(f, sizeof(short) * (n_entries - wrecked_CA_SIZE), SEEK_CUR);
		    }
		}

		chan = Convert::wrecked_CreateChanEntry(ci);
		if (chan != NULL)
		    Magick::instance().chanserv.AddStored(chan);
		wrecked_delchan(ci);
	    }			/* while (fgetc(f) == 1) */
	break;			/* case 5, etc. */

    case 4:
    case 3:
	{
	    wrecked_ChanInfo_V3 *old_ci;

	    for (i = 33; i < 256; ++i)
		while (fgetc(f) == 1)
		{
		    ci = (wrecked_ChanInfo *) malloc(sizeof(wrecked_ChanInfo));
		    old_ci = (wrecked_ChanInfo_V3 *) malloc(sizeof(wrecked_ChanInfo_V3));
		    memset(ci, 0, sizeof(wrecked_ChanInfo));
		    memset(old_ci, 0, sizeof(wrecked_ChanInfo_V3));
		    if (1 != ACE_OS::fread(old_ci, sizeof(wrecked_ChanInfo_V3), 1, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
		    }
		    /* Convert old dbase! */
		    ACE_OS::strcpy(ci->mlock_on, wrecked_oldmodeconv(old_ci->mlock_on));
		    ACE_OS::strcpy(ci->mlock_off, wrecked_oldmodeconv(old_ci->mlock_off));
		    ACE_OS::strcpy(ci->name, old_ci->name);
		    ACE_OS::strcpy(ci->founder, old_ci->founder);
		    ACE_OS::strcpy(ci->founderpass, old_ci->founderpass);
		    ACE_OS::strcpy(ci->last_topic_setter, old_ci->last_topic_setter);
		    ci->time_registered = old_ci->time_registered;
		    ci->last_used = old_ci->last_used;
		    ci->accesscount = old_ci->accesscount;
		    ci->akickcount = old_ci->akickcount;
		    ci->mlock_limit = old_ci->mlock_limit;
		    ci->last_topic_time = old_ci->last_topic_time;
		    ci->flags = old_ci->flags;

		    /* Can't guarantee the file is in a particular order...
		     * (Well, we can, but we don't have to depend on it.) */
		    ci->desc = wrecked_read_string(f, wrecked_chanserv_db);
		    if (old_ci->url)
		    {
			ci->url = wrecked_read_string(f, wrecked_chanserv_db);
			if (!ACE_OS::strlen(old_ci->url))
			{
			    free(ci->url);
			    ci->url = NULL;
			}
		    }
		    else
			ci->url = NULL;
		    if (old_ci->mlock_key)
		    {
			ci->mlock_key = wrecked_read_string(f, wrecked_chanserv_db);
			if (!ACE_OS::strlen(ci->mlock_key))
			{
			    free(ci->mlock_key);
			    ci->mlock_key = NULL;
			}
		    }
		    else
			ci->mlock_key = NULL;
		    if (old_ci->last_topic)
		    {
			ci->last_topic = wrecked_read_string(f, wrecked_chanserv_db);
			if (!ACE_OS::strlen(ci->last_topic))
			{
			    free(ci->last_topic);
			    ci->last_topic = NULL;
			}
		    }
		    else
			ci->last_topic = NULL;

		    if (ci->accesscount)
		    {
			wrecked_ChanAccess *i_access;

			i_access = (wrecked_ChanAccess *) malloc(sizeof(wrecked_ChanAccess) * ci->accesscount);
			ci->access = i_access;
			if (ci->accesscount != (long) ACE_OS::fread(i_access, sizeof(wrecked_ChanAccess), ci->accesscount, f))
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
			}
			for (j = 0; j < ci->accesscount; ++j, ++i_access)
			    i_access->name = wrecked_read_string(f, wrecked_chanserv_db);
			j = 0;
			i_access = ci->access;
			/* Clear out unused entries */
			while (j < ci->accesscount)
			{
			    if (i_access->is_nick < 0)
			    {
				--ci->accesscount;
				free(i_access->name);
				if (j < ci->accesscount)
				    memcpy(i_access, i_access + 1, sizeof(*i_access) * (ci->accesscount - j));
			    }
			    else
			    {
				++j;
				++i_access;
			    }
			}
			if (ci->accesscount)
			    ci->access = (wrecked_ChanAccess *) realloc(ci->access, sizeof(wrecked_ChanAccess) * ci->accesscount);
			else
			{
			    free(ci->access);
			    ci->access = NULL;
			}
		    }		/* if (ci->accesscount) */

		    if (ci->akickcount)
		    {
			wrecked_AutoKick *akick;

			akick = (wrecked_AutoKick *) malloc(sizeof(wrecked_AutoKick) * ci->akickcount);
			ci->akick = akick;
			if (ci->akickcount != (long) ACE_OS::fread(akick, sizeof(wrecked_AutoKick), ci->akickcount, f))
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
			}
			for (j = 0; j < ci->akickcount; ++j, ++akick)
			{
			    akick->name = wrecked_read_string(f, wrecked_chanserv_db);
			    if (akick->reason)
				akick->reason = wrecked_read_string(f, wrecked_chanserv_db);
			}
			j = 0;
			akick = ci->akick;
			while (j < ci->akickcount)
			{
			    if (akick->is_nick < 0)
			    {
				--ci->akickcount;
				free(akick->name);
				if (akick->reason)
				    free(akick->reason);
				if (j < ci->akickcount)
				    memcpy(akick, akick + 1, sizeof(*akick) * (ci->akickcount - j));
			    }
			    else
			    {
				++j;
				++akick;
			    }
			}
			if (ci->akickcount)
			{
			    ci->akick = (wrecked_AutoKick *) realloc(ci->akick, sizeof(wrecked_AutoKick) * ci->akickcount);
			}
			else
			{
			    free(ci->akick);
			    ci->akick = NULL;
			}
		    }		/* if (ci->akickcount) */

		    if (old_ci->cmd_access)
		    {
			int n_entries;
			ci->cmd_access = (short *) malloc(sizeof(short) * wrecked_CA_SIZE);
			n_entries = fgetc(f) << 8 | fgetc(f);
			if (n_entries < 0)
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
			}
			if (n_entries <= wrecked_CA_SIZE)
			{
			    ACE_OS::fread(ci->cmd_access, sizeof(short), n_entries, f);
			}
			else
			{
			    ACE_OS::fread(ci->cmd_access, sizeof(short), wrecked_CA_SIZE, f);
			    ACE_OS::fseek(f, sizeof(short) * (n_entries - wrecked_CA_SIZE), SEEK_CUR);
			}
		    }
		    free(old_ci);

		    chan = Convert::wrecked_CreateChanEntry(ci);
		    if (chan != NULL)
			Magick::instance().chanserv.AddStored(chan);
		    wrecked_delchan(ci);
		}		/* while (fgetc(f) == 1) */
	    break;		/* case 3, etc. */
	}
    case 2:
    case 1:
	{
	    wrecked_ChanInfo_V1 *old_ci;

	    for (i = 33; i < 256; ++i)
		while (fgetc(f) == 1)
		{
		    ci = (wrecked_ChanInfo *) malloc(sizeof(wrecked_ChanInfo));
		    old_ci = (wrecked_ChanInfo_V1 *) malloc(sizeof(wrecked_ChanInfo_V1));
		    memset(ci, 0, sizeof(wrecked_ChanInfo));
		    memset(old_ci, 0, sizeof(wrecked_ChanInfo_V1));
		    if (1 != ACE_OS::fread(old_ci, sizeof(wrecked_ChanInfo_V1), 1, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
		    }
		    /* Convert old dbase! */
		    ACE_OS::strcpy(ci->mlock_on, wrecked_oldmodeconv(old_ci->mlock_on));
		    ACE_OS::strcpy(ci->mlock_off, wrecked_oldmodeconv(old_ci->mlock_off));
		    ACE_OS::strcpy(ci->name, old_ci->name);
		    ACE_OS::strcpy(ci->founder, old_ci->founder);
		    ACE_OS::strcpy(ci->founderpass, old_ci->founderpass);
		    ACE_OS::strcpy(ci->last_topic_setter, old_ci->last_topic_setter);
		    ci->time_registered = old_ci->time_registered;
		    ci->last_used = old_ci->last_used;
		    ci->accesscount = old_ci->accesscount;
		    ci->akickcount = old_ci->akickcount;
		    ci->mlock_limit = old_ci->mlock_limit;
		    ci->last_topic_time = old_ci->last_topic_time;
		    ci->flags = old_ci->flags;
		    ci->url = NULL;
		    ci->desc = wrecked_read_string(f, wrecked_chanserv_db);
		    if (old_ci->mlock_key)
		    {
			ci->mlock_key = wrecked_read_string(f, wrecked_chanserv_db);
			if (!ACE_OS::strlen(ci->mlock_key))
			{
			    free(ci->mlock_key);
			    ci->mlock_key = NULL;
			}
		    }
		    else
			ci->mlock_key = NULL;
		    if (old_ci->last_topic)
		    {
			ci->last_topic = wrecked_read_string(f, wrecked_chanserv_db);
			if (!ACE_OS::strlen(ci->last_topic))
			{
			    free(ci->last_topic);
			    ci->last_topic = NULL;
			}
		    }
		    else
			ci->last_topic = NULL;

		    if (ci->accesscount)
		    {
			wrecked_ChanAccess *i_access;

			i_access = (wrecked_ChanAccess *) malloc(sizeof(wrecked_ChanAccess) * ci->accesscount);
			ci->access = i_access;
			if (ci->accesscount != (long) ACE_OS::fread(i_access, sizeof(wrecked_ChanAccess), ci->accesscount, f))
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
			}
			for (j = 0; j < ci->accesscount; ++j, ++i_access)
			    i_access->name = wrecked_read_string(f, wrecked_chanserv_db);
			j = 0;
			i_access = ci->access;
			/* Clear out unused entries */
			while (j < ci->accesscount)
			{
			    if (i_access->is_nick < 0)
			    {
				--ci->accesscount;
				free(i_access->name);
				if (j < ci->accesscount)
				    memcpy(i_access, i_access + 1, sizeof(*i_access) * (ci->accesscount - j));
			    }
			    else
			    {
				++j;
				++i_access;
			    }
			}
			if (ci->accesscount)
			    ci->access = (wrecked_ChanAccess *) realloc(ci->access, sizeof(wrecked_ChanAccess) * ci->accesscount);
			else
			{
			    free(ci->access);
			    ci->access = NULL;
			}
		    }		/* if (ci->accesscount) */

		    if (ci->akickcount)
		    {
			wrecked_AutoKick *akick;

			akick = (wrecked_AutoKick *) malloc(sizeof(wrecked_AutoKick) * ci->akickcount);
			ci->akick = akick;
			if (ci->akickcount != (long) ACE_OS::fread(akick, sizeof(wrecked_AutoKick), ci->akickcount, f))
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
			}
			for (j = 0; j < ci->akickcount; ++j, ++akick)
			{
			    akick->name = wrecked_read_string(f, wrecked_chanserv_db);
			    if (akick->reason)
				akick->reason = wrecked_read_string(f, wrecked_chanserv_db);
			}
			j = 0;
			akick = ci->akick;
			while (j < ci->akickcount)
			{
			    if (akick->is_nick < 0)
			    {
				--ci->akickcount;
				free(akick->name);
				if (akick->reason)
				    free(akick->reason);
				if (j < ci->akickcount)
				    memcpy(akick, akick + 1, sizeof(*akick) * (ci->akickcount - j));
			    }
			    else
			    {
				++j;
				++akick;
			    }
			}
			if (ci->akickcount)
			{
			    ci->akick = (wrecked_AutoKick *) realloc(ci->akick, sizeof(wrecked_AutoKick) * ci->akickcount);
			}
			else
			{
			    free(ci->akick);
			    ci->akick = NULL;
			}
		    }		/* if (ci->akickcount) */

		    if (old_ci->cmd_access)
		    {
			int n_entries;
			ci->cmd_access = (short *) malloc(sizeof(short) * wrecked_CA_SIZE);
			n_entries = fgetc(f) << 8 | fgetc(f);
			if (n_entries < 0)
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_chanserv_db));
			}
			if (n_entries <= wrecked_CA_SIZE)
			{
			    ACE_OS::fread(ci->cmd_access, sizeof(short), n_entries, f);
			}
			else
			{
			    ACE_OS::fread(ci->cmd_access, sizeof(short), wrecked_CA_SIZE, f);
			    ACE_OS::fseek(f, sizeof(short) * (n_entries - wrecked_CA_SIZE), SEEK_CUR);
			}
		    }
		    free(old_ci);

		    chan = Convert::wrecked_CreateChanEntry(ci);
		    if (chan != NULL)
			Magick::instance().chanserv.AddStored(chan);
		    wrecked_delchan(ci);
		}
	    break;		/* case 1, etc. */
	}
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", (i, wrecked_chanserv_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

char *wrecked_oldmodeconv(short inmode)
{
    BTCB();
    static char outmode[wrecked_MODEMAX];

    ACE_OS::strcpy(outmode, "");
    if (inmode & 0x01)
	ACE_OS::strcat(outmode, "i");
    if (inmode & 0x02)
	ACE_OS::strcat(outmode, "m");
    if (inmode & 0x04)
	ACE_OS::strcat(outmode, "n");
    if (inmode & 0x08)
	ACE_OS::strcat(outmode, "p");
    if (inmode & 0x10)
	ACE_OS::strcat(outmode, "s");
    if (inmode & 0x20)
	ACE_OS::strcat(outmode, "t");
    if (inmode & 0x40)
	ACE_OS::strcat(outmode, "k");
    if (inmode & 0x80)
	ACE_OS::strcat(outmode, "l");
    return outmode;
    ETCB();
}

void wrecked_delchan(wrecked_ChanInfo * ci)
{
    BTCB();
    int i;

    if (ci->desc)
	free(ci->desc);
    if (ci->url)
	free(ci->url);
    if (ci->mlock_key)
	free(ci->mlock_key);
    if (ci->last_topic)
	free(ci->last_topic);
    for (i = 0; i < ci->accesscount; ++i)
	if (ci->access[i].name)
	    free(ci->access[i].name);
    if (ci->access)
	free(ci->access);
    for (i = 0; i < ci->akickcount; ++i)
    {
	if (ci->akick[i].name)
	    free(ci->akick[i].name);
	if (ci->akick[i].reason)
	    free(ci->akick[i].reason);
    }
    if (ci->akick)
	free(ci->akick);
    free(ci);
    ci = NULL;
    ETCB();
}

Chan_Stored_t *Convert::wrecked_CreateChanEntry(wrecked_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name == NULL || !strlen(ci->name))
	return NULL;

    if (ci->flags & wrecked_CI_VERBOTEN)
    {
	Chan_Stored_t *out = new Chan_Stored_t(mstring(ci->name));

	return out;
    }
    else
    {
	wrecked_ChanAccess *i_access;
	wrecked_AutoKick *akick;
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
	for (i = 0, i_access = ci->access; i < ci->accesscount; ++i, ++i_access)
	{
	    if (i_access->name == NULL)
		continue;
	    if (i_access->is_nick > 0)
	    {
		out->Access_insert(i_access->name, i_access->level, Magick::instance().chanserv.FirstName());
	    }
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

	if (ci->flags & wrecked_CI_KEEPTOPIC && !out->L_Keeptopic())
	    out->setting.Keeptopic = true;
	if (ci->flags & wrecked_CI_SECUREOPS && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & wrecked_CI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & wrecked_CI_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & wrecked_CI_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & wrecked_CI_LEAVEOPS && !out->L_Anarchy())
	    out->setting.Anarchy = true;
	if (ci->flags & wrecked_CI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & wrecked_CI_JOIN && !out->L_Join())
	    out->setting.Join = true;
	if (!out->L_Revenge())
	{
	    char revlevel = ((ci->flags & wrecked_CI_REV3) << 2) | ((ci->flags & wrecked_CI_REV2) << 1) | (ci->flags & wrecked_CI_REV1);

	    switch (revlevel)
	    {
	    case wrecked_CR_REVERSE:
		out->setting.Revenge = "REVERSE";
		break;
	    case wrecked_CR_DEOP:
		out->setting.Revenge = "DEOP";
		break;
	    case wrecked_CR_KICK:
		out->setting.Revenge = "KICK";
		break;
	    case wrecked_CR_NICKBAN:
		out->setting.Revenge = "BAN1";
		break;
	    case wrecked_CR_USERBAN:
		out->setting.Revenge = "BAN2";
		break;
	    case wrecked_CR_HOSTBAN:
		out->setting.Revenge = "BAN3";
		break;
	    case wrecked_CR_MIRROR:
		out->setting.Revenge = "MIRROR";
		break;
	    default:
		out->setting.Revenge = "NONE";
	    }
	}

	if (ci->flags & wrecked_CI_SUSPENDED)
	{
	    if (ci->last_topic != NULL && strlen(ci->last_topic))
		out->i_Comment = mstring(ci->last_topic);
	    if (ci->last_topic_setter != NULL && strlen(ci->last_topic_setter))
		out->i_Suspend_By = mstring(ci->last_topic_setter);
	    out->i_Suspend_Time = mDateTime(ci->last_topic_time);
	}
	else
	{
	    if (ci->last_topic != NULL && strlen(ci->last_topic))
		out->i_Topic = mstring(ci->last_topic);
	    if (ci->last_topic_setter != NULL && strlen(ci->last_topic_setter))
		out->i_Topic_Setter = mstring(ci->last_topic_setter);
	    out->i_Topic_Set_Time = mDateTime(ci->last_topic_time);
	}

	if (ci->cmd_access != NULL)
	{
	    for (i = 0; i < wrecked_CA_SIZE; ++i)
	    {
		switch (i)
		{
		case wrecked_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_AUTOOP:
		    out->Level_change("AUTOOP", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_READMEMO:
		    out->Level_change("READMEMO", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_WRITEMEMO:
		    out->Level_change("WRITEMEMO", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_DELMEMO:
		    out->Level_change("DELMEMO", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_AKICK:
		    out->Level_change("AKICK", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_STARAKICK:
		    out->Level_change("SUPER", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    out->Level_change("OVERGREET", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_UNBAN:
		    out->Level_change("UNBAN", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_ACCESS:
		    out->Level_change("ACCESS", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_SET:
		    out->Level_change("SET", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_CMDINVITE:
		    out->Level_change("CMDINVITE", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_CMDUNBAN:
		    out->Level_change("CMDUNBAN", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_CMDVOICE:
		    out->Level_change("CMDVOICE", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_CMDOP:
		    out->Level_change("CMDOP", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case wrecked_CA_CMDCLEAR:
		    out->Level_change("CMDCLEAR", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}
	return out;
    }
    ETCB();
}

void wrecked_load_memo()
{
    BTCB();
    FILE *f = ACE_OS::fopen(wrecked_memoserv_db, "r");
    int i, j;
    wrecked_MemoList *ml;
    wrecked_Memo *memos;

    MemoServ::nick_memo_t memo;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read MemoServ database $1", (wrecked_memoserv_db));
	return;
    }
    switch (i = wrecked_get_file_version(f, wrecked_memoserv_db))
    {
    case 6:
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	for (i = 33; i < 256; ++i)
	    while (fgetc(f) == 1)
	    {
		ml = (wrecked_MemoList *) malloc(sizeof(wrecked_MemoList));
		memset(ml, 0, sizeof(wrecked_MemoList));
		if (1 != ACE_OS::fread(ml, sizeof(wrecked_MemoList), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_memoserv_db));
		}
		ml->memos = memos = (wrecked_Memo *) malloc(sizeof(wrecked_Memo) * ml->n_memos);
		memset(ml->memos, 0, sizeof(wrecked_Memo) * ml->n_memos);
		ACE_OS::fread(memos, sizeof(wrecked_Memo), ml->n_memos, f);
		for (j = 0; j < ml->n_memos; ++j, ++memos)
		    memos->text = wrecked_read_string(f, wrecked_memoserv_db);

		memo = Convert::wrecked_CreateMemoEntry(ml);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);
		wrecked_del_memolist(ml);
	    }
	break;
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", (i, wrecked_memoserv_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

void wrecked_load_news()
{
    BTCB();
    FILE *f = ACE_OS::fopen(wrecked_newsserv_db, "r");
    int i, j;
    wrecked_NewsList *nl;
    wrecked_Memo *newss;

    MemoServ::channel_news_t news;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read NewsServ database $1", (wrecked_newsserv_db));
	return;
    }
    switch (i = wrecked_get_file_version(f, wrecked_newsserv_db))
    {
    case 6:
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	for (i = 33; i < 256; ++i)
	{
	    while (fgetc(f) == 1)
	    {
		nl = (wrecked_NewsList *) malloc(sizeof(wrecked_NewsList));
		memset(nl, 0, sizeof(wrecked_NewsList));
		if (1 != ACE_OS::fread(nl, sizeof(wrecked_NewsList), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_newsserv_db));
		}
		nl->newss = newss = (wrecked_Memo *) malloc(sizeof(wrecked_Memo) * nl->n_newss);
		memset(nl->newss, 0, sizeof(wrecked_Memo) * nl->n_newss);
		ACE_OS::fread(newss, sizeof(wrecked_Memo), nl->n_newss, f);
		for (j = 0; j < nl->n_newss; ++j, ++newss)
		    newss->text = wrecked_read_string(f, wrecked_newsserv_db);

		news = Convert::wrecked_CreateNewsEntry(nl);
		if (news.size())
		    Magick::instance().memoserv.AddChannel(news);
		wrecked_del_newslist(nl);
	    }
	}
	break;
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", (i, wrecked_newsserv_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

/* wrecked_del_memolist:  Remove a nick's memo list from the database.  Assumes
 *                that the memo count for the nick is non-zero.
 */

void wrecked_del_memolist(wrecked_MemoList * ml)
{
    BTCB();
    int i;
    wrecked_Memo *memos;

    if (!ml)
	return;
    memos = ml->memos;
    for (i = 0; i < ml->n_memos; ++i, ++memos)
	free(memos->text);
    if (ml->memos)
	free(ml->memos);
    free(ml);
    ETCB();
}

/* wrecked_del_newslist:  Remove a nick's news list from the database.  Assumes
 *                that the news count for the nick is non-zero.
 */

void wrecked_del_newslist(wrecked_NewsList * nl)
{
    BTCB();
    int i;
    wrecked_Memo *newss;

    if (!nl)
	return;
    newss = nl->newss;
    for (i = 0; i < nl->n_newss; ++i, ++newss)
	free(newss->text);
    if (nl->newss)
	free(nl->newss);
    free(nl);
    ETCB();
}

MemoServ::nick_memo_t Convert::wrecked_CreateMemoEntry(wrecked_MemoList * ml)
{
    BTCB();
    int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;
    wrecked_Memo *memos;

    memos = ml->memos;
    for (i = 0; i < ml->n_memos; ++i, ++memos)
    {
	if (memos->text == NULL)
	    continue;

	tmp = new Memo_t(mstring(ml->nick), mstring(memos->sender), mstring(memos->text));
	tmp->i_Time = mDateTime(memos->time);
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}

MemoServ::channel_news_t Convert::wrecked_CreateNewsEntry(wrecked_NewsList * nl)
{
    BTCB();
    int i;

    MemoServ::channel_news_t out;
    News_t *tmp;
    wrecked_Memo *newss;

    newss = nl->newss;
    for (i = 0; i < nl->n_newss; ++i, ++newss)
    {
	if (newss->text == NULL)
	    continue;
	tmp = new News_t(mstring(nl->chan), mstring(newss->sender), mstring(newss->text));
	tmp->i_Time = mDateTime(newss->time);
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}

void wrecked_load_sop()
{
    BTCB();
    FILE *f = ACE_OS::fopen(wrecked_sop_db, "r");
    int i, j, nsop = 0, sop_size = 0;
    wrecked_Sop *sops = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read SOP database $1", (wrecked_sop_db));
	return;
    }
    switch (i = wrecked_get_file_version(f, wrecked_sop_db))
    {
    case 6:
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	nsop = fgetc(f) * 256 + fgetc(f);
	if (nsop < 8)
	    sop_size = 16;
	else
	    sop_size = 2 * nsop;
	sops = (wrecked_Sop *) malloc(sizeof(wrecked_Sop) * sop_size);
	memset(sops, 0, sizeof(wrecked_Sop) * sop_size);
	if (!nsop)
	{
	    fclose(f);
	    return;
	}
	if (nsop != (int) ACE_OS::fread(sops, sizeof(wrecked_Sop), nsop, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_sop_db));
	}

	if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()))
	{
	    for (j = 0; j < nsop; ++j)
	    {
		if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		      Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(sops[j].nick)))
		    Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(sops[j].nick),
													Magick::instance().
													commserv.FirstName());
	    }
	}
	free(sops);
	break;
    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, wrecked_sop_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

void wrecked_load_message()
{
    BTCB();
    FILE *f = ACE_OS::fopen(wrecked_message_db, "r");
    int i, j, nmessage = 0, message_size = 0;
    wrecked_Message *messages = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read MESSAGE database $1", (wrecked_message_db));
	return;
    }
    switch (i = wrecked_get_file_version(f, wrecked_message_db))
    {
    case 6:
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	nmessage = fgetc(f) * 256 + fgetc(f);
	if (nmessage < 8)
	    message_size = 16;
	else
	    message_size = 2 * nmessage;
	messages = (wrecked_Message *) malloc(sizeof(wrecked_Message) * message_size);
	memset(messages, 0, sizeof(wrecked_Message) * message_size);
	if (!nmessage)
	{
	    fclose(f);
	    return;
	}
	if (nmessage != (int) ACE_OS::fread(messages, sizeof(*messages), nmessage, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_message_db));
	}
	for (j = 0; j < nmessage; ++j)
	    messages[j].text = wrecked_read_string(f, wrecked_message_db);

	for (j = 0; j < nmessage; ++j)
	{
	    if (messages[j].text == NULL)
		continue;

	    if (messages[j].type == wrecked_M_LOGON && Magick::instance().commserv.IsList(Magick::instance().commserv.ALL_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.ALL_Name())->
		    MSG_insert(mstring(messages[j].text), mstring(messages[j].who));
	    }
	    else if (messages[j].type == wrecked_M_OPER && Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->
		    MSG_insert(mstring(messages[j].text), mstring(messages[j].who));
	    }
	}

	for (j = 0; j < nmessage; ++j)
	    free(messages[j].text);
	free(messages);
	break;

    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, wrecked_message_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

void wrecked_load_akill()
{
    BTCB();
    FILE *f = ACE_OS::fopen(wrecked_akill_db, "r");
    int i, j, nakill = 0, akill_size = 0;
    wrecked_Akill *akills = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read AKILL database $1", (wrecked_akill_db));
	return;
    }
    switch (i = wrecked_get_file_version(f, wrecked_akill_db))
    {
    case 6:
    case 5:
    case 4:
    case 3:
    case 2:
	nakill = fgetc(f) * 256 + fgetc(f);
	if (nakill < 8)
	    akill_size = 16;
	else
	    akill_size = 2 * nakill;
	akills = (wrecked_Akill *) malloc(sizeof(wrecked_Akill) * akill_size);
	memset(akills, 0, sizeof(wrecked_Akill) * akill_size);
	if (!nakill)
	{
	    fclose(f);
	    return;
	}
	if (nakill != (int) ACE_OS::fread(akills, sizeof(*akills), nakill, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_akill_db));
	}
	for (j = 0; j < nakill; ++j)
	{
	    akills[j].mask = wrecked_read_string(f, wrecked_akill_db);
	    akills[j].reason = wrecked_read_string(f, wrecked_akill_db);
	}

	for (j = 0; j < nakill; ++j)
	{
	    if (akills[j].mask != NULL && akills[j].reason != NULL)
	    {
		if (akills[j].time == 0)
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[j].mask), Magick::instance().operserv.Expire_Sop(),
							     mstring(akills[j].reason), mstring(akills[j].who));
		}
		else
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[j].mask), Magick::instance().operserv.Def_Expire(),
							     mstring(akills[j].reason), mstring(akills[j].who),
							     mDateTime(akills[j].time));
		}
	    }

	    if (akills[j].mask)
		free(akills[j].mask);
	    if (akills[j].reason)
		free(akills[j].reason);
	}
	free(akills);
	break;

    case 1:
	{
	    wrecked_Akill_V1 old_akill;

	    nakill = fgetc(f) * 256 + fgetc(f);
	    if (nakill < 8)
		akill_size = 16;
	    else
		akill_size = 2 * nakill;
	    akills = (wrecked_Akill *) malloc(sizeof(wrecked_Akill) * akill_size);
	    memset(akills, 0, sizeof(wrecked_Akill) * akill_size);
	    if (!nakill)
	    {
		fclose(f);
		return;
	    }
	    for (j = 0; j < nakill; ++j)
	    {
		if (1 != ACE_OS::fread(&old_akill, sizeof(old_akill), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_akill_db));
		}
		akills[j].time = old_akill.time;
		akills[j].who[0] = 0;
	    }
	    for (j = 0; j < nakill; ++j)
	    {
		akills[j].mask = wrecked_read_string(f, wrecked_akill_db);
		akills[j].reason = wrecked_read_string(f, wrecked_akill_db);
	    }
	}			/* case 1 */

	for (j = 0; j < nakill; ++j)
	{
	    if (akills[j].mask != NULL && akills[j].reason != NULL)
	    {
		if (akills[j].time == 0)
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[j].mask), Magick::instance().operserv.Expire_Sop(),
							     mstring(akills[j].reason), mstring(akills[j].who));
		}
		else
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[j].mask), Magick::instance().operserv.Def_Expire(),
							     mstring(akills[j].reason), mstring(akills[j].who),
							     mDateTime(akills[j].time));
		}
	    }

	    if (akills[j].mask)
		free(akills[j].mask);
	    if (akills[j].reason)
		free(akills[j].reason);
	}
	free(akills);

	break;

    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, wrecked_akill_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

void wrecked_load_clone()
{
    BTCB();
    FILE *f = ACE_OS::fopen(wrecked_clone_db, "r");
    int i, j, nclone = 0, clone_size = 0;
    wrecked_Allow *clones = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read CLONE database $1", (wrecked_clone_db));
	return;
    }
    switch (i = wrecked_get_file_version(f, wrecked_clone_db))
    {
    case 6:
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	nclone = fgetc(f) * 256 + fgetc(f);
	if (nclone < 8)
	    clone_size = 16;
	else
	    clone_size = 2 * nclone;
	clones = (wrecked_Allow *) malloc(sizeof(wrecked_Allow) * clone_size);
	memset(clones, 0, sizeof(wrecked_Allow) * clone_size);
	if (!nclone)
	{
	    fclose(f);
	    return;
	}
	if (nclone != (int) ACE_OS::fread(clones, sizeof(*clones), nclone, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", (wrecked_clone_db));
	}
	for (j = 0; j < nclone; ++j)
	{
	    clones[j].host = wrecked_read_string(f, wrecked_clone_db);
	    clones[j].reason = wrecked_read_string(f, wrecked_clone_db);
	}

	for (j = 0; j < nclone; ++j)
	{
	    if (clones[j].host != NULL && clones[j].reason != NULL)
	    {
		Magick::instance().operserv.Clone_insert(mstring(clones[j].host), clones[j].amount, mstring(clones[j].reason),
							 mstring(clones[j].who), mDateTime(clones[j].time));
	    }
	    if (clones[j].host)
		free(clones[j].host);
	    if (clones[j].reason)
		free(clones[j].reason);
	}
	free(clones);
	break;
    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, wrecked_clone_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

#endif /* CONVERT */
