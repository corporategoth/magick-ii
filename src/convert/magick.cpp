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
#define RCSID(x,y) const char *rcsid_convert_magick_cpp_ ## x () { return y; }
RCSID(convert_magick_cpp, "@(#)$Id$");

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

/* Based upon the DB loading routines from Magick 1.4
 * (c) 1996-1998 Preston A. Elder <prez@magick.tm>
 */

#include "magick.h"
#include "convert/interface.h"
#include "convert/magick.h"

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
#define	magick_CHANMAX		64

/* Maximum length of a nickname */
#define	magick_NICKMAX		32

/* Maximum length of a password */
#define	magick_PASSMAX		32

/* Maximum number of valid +/- Modes */
#define magick_MODEMAX		64

/* Nickname info structure.  Each nick structure is stored in one of 256
 * lists; the list is determined by the first character of the nick.  Nicks
 * are stored in alphabetical order within lists. */

#define	magick_NI_KILLPROTECT	0x00000001	/* Kill others who take this nick */
#define	magick_NI_SECURE	0x00000002	/* Don't recognize unless IDENTIFY'd */
#define	magick_NI_VERBOTEN	0x00000004	/* Nick may not be registered or used */
#define	magick_NI_IRCOP		0x00000008	/* IrcOP - Nick will not expire */
#define	magick_NI_PRIVATE	0x00000010	/* Private - Dont show up in list */
#define	magick_NI_SUSPENDED	0x00000020	/* Suspended - May not IDENTIFY */
#define	magick_NI_PRIVMSG	0x00000040	/* use PRIVMSG instead of NOTICE */
#define magick_NI_SLAVE		0x00000080	/* Nick is just a 'linked' nick */

#define	magick_NI_IDENTIFIED	0x80000000	/* User has IDENTIFY'd */
#define	magick_NI_RECOGNIZED	0x40000000	/* User comes from a known addy */
#define	magick_NI_KILL_HELD	0x20000000	/* Nick is being held after a kill */

/*************************************************************************/

/* Channel info structures.  Stored similarly to the nicks, except that
 * the second character of the channel name, not the first, is used to
 * determine the list.  (Hashing based on the first character of the name
 * wouldn't get very far. ;) ) */

/* Retain topic even after last person leaves channel */
#define	magick_CI_KEEPTOPIC	0x00000001

/* Don't allow non-authorized users to be opped */
#define	magick_CI_SECUREOPS	0x00000002

/* Hide channel from ChanServ LIST command */
#define	magick_CI_PRIVATE	0x00000004

/* Topic can only be changed by SET TOPIC */
#define	magick_CI_TOPICLOCK	0x00000008

/* Those not allowed ops are kickbanned */
#define	magick_CI_RESTRICTED	0x00000010

/* Don't auto-deop anyone */
#define	magick_CI_LEAVEOPS	0x00000020

/* Don't allow any privileges unless a user is IDENTIFY'd with NickServ */
#define	magick_CI_SECURE	0x00000040

/* Don't allow the channel to be registered or used */
#define	magick_CI_VERBOTEN	0x00000080

/* Dont honour channel access list or founder */
#define	magick_CI_SUSPENDED	0x00000100

/* ChanServ joins channel when its established */
#define	magick_CI_JOIN		0x00000200

/* Revenge flags */
#define	magick_CI_REV1		0x80000000
#define	magick_CI_REV2		0x40000000
#define	magick_CI_REV3		0x20000000

/* Revenge levels */
#define	magick_CR_NONE		0
#define	magick_CR_REVERSE	1
#define	magick_CR_DEOP		2
#define	magick_CR_KICK		3
#define	magick_CR_NICKBAN	4
#define	magick_CR_USERBAN	5
#define	magick_CR_HOSTBAN	6
#define	magick_CR_MIRROR	7

/* Indices for cmd_access[]: */
#define	magick_CA_AUTODEOP	0
#define	magick_CA_AUTOVOICE	1
#define	magick_CA_AUTOOP	2
#define	magick_CA_READMEMO	3
#define	magick_CA_WRITEMEMO	4
#define	magick_CA_DELMEMO	5
#define	magick_CA_AKICK	6
#define	magick_CA_STARAKICK	7
#define	magick_CA_UNBAN	8
#define	magick_CA_ACCESS	9
#define	magick_CA_SET		10	/* NOT FOUNDER and PASSWORD */

#define	magick_CA_CMDINVITE	11
#define	magick_CA_CMDUNBAN	12
#define	magick_CA_CMDVOICE	13
#define	magick_CA_CMDOP	14
#define	magick_CA_CMDCLEAR	15

#define	magick_CA_FLOOR	16
#define	magick_CA_CAP		17
#define	magick_CA_FOUNDER	18

#define	magick_CA_SIZE		19	/* <--- DO NOT DELETE */

/*************************************************************************/

/* MemoServ data.  Only nicks that actually have memos get records in
 * MemoServ's lists, which are stored the same way NickServ's are. */

#define	magick_MS_READ		0x00000001	/* Yes, I had planned to do this */
#define magick_MS_DELETED	0x00000002

#define	magick_M_LOGON	0
#define	magick_M_OPER	1

struct magick_NickInfo_V1
{
    magick_NickInfo_V1 *next, *prev;
    char nick[magick_NICKMAX];
    char pass[magick_PASSMAX];
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;		/* # of entries */
    char **access;		/* Array of strings */
    long flags;			/* See below */
    long reserved[4];		/* For future expansion -- set to 0 */
};

struct magick_NickInfo_V3
{
    magick_NickInfo_V3 *next, *prev;
    char nick[magick_NICKMAX];
    char pass[magick_PASSMAX];
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

struct magick_NickInfo
{
    magick_NickInfo *next, *prev;
    char nick[magick_NICKMAX];
    char pass[magick_PASSMAX];
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

/****************************************************************************
 * chan.db
 ****************************************************************************/

struct magick_ChanAccess
{
    short level;
    short is_nick;		/* 1 if this is a nick, 0 if a user@host mask.  If
				 * -1, then this entry is currently unused (a hack
				 * to get numbered lists to have consistent
				 * numbering). */
    char *name;
};

struct magick_AutoKick
{
    short is_nick;
    short pad;
    char *name;
    char *reason;
};

struct magick_ChanInfo_V1
{
    magick_ChanInfo_V1 *next, *prev;
    char name[magick_CHANMAX];
    char founder[magick_NICKMAX];	/* Always a reg'd nick */
    char founderpass[magick_PASSMAX];
    char *desc;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    magick_ChanAccess *access;		/* List of authorized users */
    long akickcount;
    magick_AutoKick *akick;
    short mlock_on, mlock_off;	/* See channel modes below */
    long mlock_limit;		/* 0 if no limit */
    char *mlock_key;		/* NULL if no key */
    char *last_topic;		/* Last topic on the channel */
    char last_topic_setter[magick_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;	/* When the last topic was set */
    long flags;			/* See below */
    short *cmd_access;		/* Access levels for commands */
    long reserved[3];		/* For future expansion -- set to 0 */
};

struct magick_ChanInfo_V3
{
    magick_ChanInfo_V3 *next, *prev;
    char name[magick_CHANMAX];
    char founder[magick_NICKMAX];	/* Always a reg'd nick */
    char founderpass[magick_PASSMAX];
    char *desc;
    char *url;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    magick_ChanAccess *access;		/* List of authorized users */
    long akickcount;
    magick_AutoKick *akick;
    short mlock_on, mlock_off;	/* See channel modes below */
    long mlock_limit;		/* 0 if no limit */
    char *mlock_key;		/* NULL if no key */
    char *last_topic;		/* Last topic on the channel */
    char last_topic_setter[magick_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;	/* When the last topic was set */
    long flags;			/* See below */
    short *cmd_access;		/* Access levels for commands */
    long reserved[3];		/* For future expansion -- set to 0 */
};

struct magick_ChanInfo
{
    magick_ChanInfo *next, *prev;
    char name[magick_CHANMAX];
    char founder[magick_NICKMAX];	/* Always a reg'd nick */
    char founderpass[magick_PASSMAX];
    char *desc;
    char *url;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    magick_ChanAccess *access;		/* List of authorized users */
    long akickcount;
    magick_AutoKick *akick;
    char mlock_on[64], mlock_off[64];	/* See channel modes below */
    long mlock_limit;		/* 0 if no limit */
    char *mlock_key;		/* NULL if no key */
    char *last_topic;		/* Last topic on the channel */
    char last_topic_setter[magick_NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;	/* When the last topic was set */
    long flags;			/* See below */
    short *cmd_access;		/* Access levels for commands */
    long reserved[3];		/* For future expansion -- set to 0 */
};

/****************************************************************************
 * memo.db and news.db
 ****************************************************************************/

struct magick_Memo
{
    char sender[magick_NICKMAX];
    long number;		/* Index number -- not necessarily array position! */
    time_t time;		/* When it was sent */
    char *text;
    long reserved[4];		/* For future expansion -- set to 0 */
};

struct magick_MemoList
{
    magick_MemoList *next, *prev;
    char nick[magick_NICKMAX];		/* Owner of the memos */
    long n_memos;		/* Number of memos */
    magick_Memo *memos;		/* The memos themselves */
    long reserved[4];		/* For future expansion -- set to 0 */
};

struct magick_NewsList
{
    magick_NewsList *next, *prev;
    char chan[magick_CHANMAX];		/* Owner of the memos */
    long n_newss;		/* Number of memos */
    magick_Memo *newss;		/* The memos themselves */
    long reserved[4];		/* For future expansion -- set to 0 */
};

/****************************************************************************
 * others
 ****************************************************************************/

struct magick_Message
{
    char *text;
    short type;
    char who[magick_NICKMAX];
    time_t time;
};

struct magick_Akill_V1
{
    char *mask;
    char *reason;
    time_t time;
};

struct magick_Akill
{
    char *mask;
    char *reason;
    char who[magick_NICKMAX];
    time_t time;
};

struct magick_Allow
{
    char *host;
    int amount;
    char *reason;
    char who[magick_NICKMAX];
    time_t time;
};

struct magick_Sop
{
    char nick[magick_NICKMAX];
};

const int magick_file_version = 5;
const char *magick_nickserv_db = "nick.db";
const char *magick_chanserv_db = "chan.db";
const char *magick_memoserv_db = "memo.db";
const char *magick_newsserv_db = "news.db";
const char *magick_sop_db = "sop.db";
const char *magick_message_db = "message.db";
const char *magick_akill_db = "akill.db";
const char *magick_clone_db = "clone.db";

int magick_get_file_version(FILE * f, const char *filename)
{
    BTCB();
    int version = fgetc(f) << 24 | fgetc(f) << 16 | fgetc(f) << 8 | fgetc(f);

    if (ferror(f) || feof(f))
    {
	SLOG(LM_EMERGENCY, "Error reading version number on $1", (filename));
    }
    else if (version > magick_file_version || version < 1)
    {
	SLOG(LM_EMERGENCY, "Invalid version number ($1) on $2", (version, filename));
    }
    return version;
    ETCB();
}

char *magick_read_string(FILE * f, const char *filename)
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

void magick_delnick(magick_NickInfo * ni);
void magick_delchan(magick_ChanInfo * ci);
char *magick_oldmodeconv(short inmode);
void magick_del_memolist(magick_MemoList * ml);
void magick_del_newslist(magick_NewsList * nl);

void magick_load_nick()
{
    BTCB();
    FILE *f = ACE_OS::fopen(magick_nickserv_db, "r");
    int i, j;
    magick_NickInfo *ni;
    Nick_Stored_t *nick;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read NickServ database $1", (magick_nickserv_db));
	return;
    }
    switch (i = magick_get_file_version(f, magick_nickserv_db))
    {
    case 5:
    case 4:
	for (i = 33; i < 256; ++i)
	    while (fgetc(f) == 1)
	    {
		ni = (magick_NickInfo *) malloc(sizeof(magick_NickInfo));
		memset(ni, 0, sizeof(magick_NickInfo));
		if (1 != ACE_OS::fread(ni, sizeof(magick_NickInfo), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (magick_nickserv_db));
		}
		ni->flags &= ~(magick_NI_IDENTIFIED | magick_NI_RECOGNIZED);
		if (ni->email)
		{
		    ni->email = magick_read_string(f, magick_nickserv_db);
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
		    ni->url = magick_read_string(f, magick_nickserv_db);
		    if (!ACE_OS::strlen(ni->url))
		    {
			free(ni->url);
			ni->url = NULL;
		    }
		}
		else
		    ni->url = NULL;
		ni->last_usermask = magick_read_string(f, magick_nickserv_db);
		ni->last_realname = magick_read_string(f, magick_nickserv_db);
		if (ni->accesscount)
		{
		    char **i_access;
		    i_access = (char **) malloc(sizeof(char *) * ni->accesscount);
		    ni->access = i_access;
		    for (j = 0; j < ni->accesscount; ++j, ++i_access)
			*i_access = magick_read_string(f, magick_nickserv_db);
		}
		if (ni->ignorecount)
		{
		    char **ignore;
		    ignore = (char **) malloc(sizeof(char *) * ni->ignorecount);
		    ni->ignore = ignore;
		    for (j = 0; j < ni->ignorecount; ++j, ++ignore)
			*ignore = magick_read_string(f, magick_nickserv_db);
		}

		nick = Convert::magick_CreateNickEntry(ni);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);
		magick_delnick(ni);
	    }
	break;
    case 3:
	{
	    magick_NickInfo_V3 *old_ni;

	    for (i = 33; i < 256; ++i)
		while (fgetc(f) == 1)
		{
		    ni = (magick_NickInfo *) malloc(sizeof(magick_NickInfo));
		    old_ni = (magick_NickInfo_V3 *) malloc(sizeof(magick_NickInfo_V3));
		    memset(ni, 0, sizeof(magick_NickInfo));
		    memset(old_ni, 0, sizeof(magick_NickInfo_V3));
		    if (1 != ACE_OS::fread(old_ni, sizeof(magick_NickInfo_V3), 1, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (magick_nickserv_db));
		    }
		    ACE_OS::strcpy(ni->nick, old_ni->nick);
		    ACE_OS::strcpy(ni->pass, old_ni->pass);
		    ni->time_registered = old_ni->time_registered;
		    ni->last_seen = old_ni->last_seen;
		    ni->accesscount = old_ni->accesscount;
		    ni->ignorecount = old_ni->ignorecount;
		    ni->flags = old_ni->flags;
		    ni->flags &= ~(magick_NI_IDENTIFIED | magick_NI_RECOGNIZED);

		    if (old_ni->email)
		    {
			ni->email = magick_read_string(f, magick_nickserv_db);
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
			ni->url = magick_read_string(f, magick_nickserv_db);
			if (!ACE_OS::strlen(ni->url))
			{
			    free(ni->url);
			    ni->url = NULL;
			}
		    }
		    else
			ni->url = NULL;
		    old_ni->last_usermask = magick_read_string(f, magick_nickserv_db);
		    ni->last_usermask = old_ni->last_usermask;
		    old_ni->last_realname = magick_read_string(f, magick_nickserv_db);
		    ni->last_realname = old_ni->last_realname;
		    free(old_ni);

		    if (ni->accesscount)
		    {
			char **i_access;
			i_access = (char **) malloc(sizeof(char *) * ni->accesscount);
			ni->access = i_access;
			for (j = 0; j < ni->accesscount; ++j, ++i_access)
			    *i_access = magick_read_string(f, magick_nickserv_db);
		    }

		    nick = Convert::magick_CreateNickEntry(ni);
		    if (nick != NULL)
			Magick::instance().nickserv.AddStored(nick);
		    magick_delnick(ni);
		}
	    break;
	}
    case 2:
    case 1:
	{
	    magick_NickInfo_V1 *old_ni;

	    for (i = 33; i < 256; ++i)
		while (fgetc(f) == 1)
		{
		    ni = (magick_NickInfo *) malloc(sizeof(magick_NickInfo));
		    old_ni = (magick_NickInfo_V1 *) malloc(sizeof(magick_NickInfo_V1));
		    memset(ni, 0, sizeof(magick_NickInfo));
		    memset(old_ni, 0, sizeof(magick_NickInfo_V1));
		    if (1 != ACE_OS::fread(ni, sizeof(magick_NickInfo_V1), 1, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (magick_nickserv_db));
		    }
		    ACE_OS::strcpy(ni->nick, old_ni->nick);
		    ACE_OS::strcpy(ni->pass, old_ni->pass);
		    ni->time_registered = old_ni->time_registered;
		    ni->last_seen = old_ni->last_seen;
		    ni->accesscount = old_ni->accesscount;
		    ni->flags = old_ni->flags;
		    ni->flags &= ~(magick_NI_IDENTIFIED | magick_NI_RECOGNIZED);

		    old_ni->last_usermask = magick_read_string(f, magick_nickserv_db);
		    ni->last_usermask = old_ni->last_usermask;
		    old_ni->last_realname = magick_read_string(f, magick_nickserv_db);
		    ni->last_realname = old_ni->last_realname;
		    free(old_ni);

		    if (ni->accesscount)
		    {
			char **i_access;
			i_access = (char **) malloc(sizeof(char *) * ni->accesscount);
			ni->access = i_access;
			for (j = 0; j < ni->accesscount; ++j, ++i_access)
			    *i_access = magick_read_string(f, magick_nickserv_db);
		    }

		    nick = Convert::magick_CreateNickEntry(ni);
		    if (nick != NULL)
			Magick::instance().nickserv.AddStored(nick);
		    magick_delnick(ni);
		}
	    break;
	}
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", (i, magick_nickserv_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

/* Remove a nick from the NickServ database.  Return 1 on success, 0
 * otherwise. */

void magick_delnick(magick_NickInfo * ni)
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

Nick_Stored_t *Convert::magick_CreateNickEntry(magick_NickInfo * ni)
{
    BTCB();
    int i;
    char **string;

    if (ni == NULL || ni->nick == NULL || !strlen(ni->nick))
	return NULL;

    if (ni->flags & magick_NI_VERBOTEN)
    {
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick);

	return out;
    }
    else if (ni->flags & magick_NI_SLAVE && ni->last_usermask != NULL)
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

	if (ni->flags & magick_NI_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (ni->flags & magick_NI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ni->flags & magick_NI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ni->flags & magick_NI_PRIVMSG && !out->L_PRIVMSG())
	    out->setting.PRIVMSG = true;

	if (ni->flags & magick_NI_SUSPENDED)
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

	if (ni->flags & magick_NI_IRCOP)
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

void magick_load_chan()
{
    BTCB();
    FILE *f = ACE_OS::fopen(magick_chanserv_db, "r");
    int i, j;
    magick_ChanInfo *ci;
    Chan_Stored_t *chan;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read ChanServ database $1", (magick_chanserv_db));
	return;
    }

    switch (i = magick_get_file_version(f, magick_chanserv_db))
    {
    case 5:
	for (i = 33; i < 256; ++i)
	    while (fgetc(f) == 1)
	    {
		ci = (magick_ChanInfo *) malloc(sizeof(magick_ChanInfo));
		memset(ci, 0, sizeof(magick_ChanInfo));
		if (1 != ACE_OS::fread(ci, sizeof(magick_ChanInfo), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
		}
		ci->desc = magick_read_string(f, magick_chanserv_db);
		if (ci->url)
		    ci->url = magick_read_string(f, magick_chanserv_db);
		if (ci->mlock_key)
		    ci->mlock_key = magick_read_string(f, magick_chanserv_db);
		if (ci->last_topic)
		    ci->last_topic = magick_read_string(f, magick_chanserv_db);

		if (ci->accesscount)
		{
		    magick_ChanAccess *i_access;

		    i_access = (magick_ChanAccess *) malloc(sizeof(magick_ChanAccess) * ci->accesscount);
		    ci->access = i_access;
		    if (ci->accesscount != (long) ACE_OS::fread(i_access, sizeof(magick_ChanAccess), ci->accesscount, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
		    }
		    for (j = 0; j < ci->accesscount; ++j, ++i_access)
			i_access->name = magick_read_string(f, magick_chanserv_db);
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
			ci->access = (magick_ChanAccess *) realloc(ci->access, sizeof(magick_ChanAccess) * ci->accesscount);
		    else
		    {
			free(ci->access);
			ci->access = NULL;
		    }
		}		/* if (ci->accesscount) */

		if (ci->akickcount)
		{
		    magick_AutoKick *akick;

		    akick = (magick_AutoKick *) malloc(sizeof(magick_AutoKick) * ci->akickcount);
		    ci->akick = akick;
		    if (ci->akickcount != (long) ACE_OS::fread(akick, sizeof(magick_AutoKick), ci->akickcount, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
		    }
		    for (j = 0; j < ci->akickcount; ++j, ++akick)
		    {
			akick->name = magick_read_string(f, magick_chanserv_db);
			if (akick->reason)
			    akick->reason = magick_read_string(f, magick_chanserv_db);
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
			ci->akick = (magick_AutoKick *) realloc(ci->akick, sizeof(magick_AutoKick) * ci->akickcount);
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
		    ci->cmd_access = (short *) malloc(sizeof(short) * magick_CA_SIZE);
		    n_entries = fgetc(f) << 8 | fgetc(f);
		    if (n_entries < 0)
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
		    }
		    if (n_entries <= magick_CA_SIZE)
		    {
			ACE_OS::fread(ci->cmd_access, sizeof(short), n_entries, f);
		    }
		    else
		    {
			ACE_OS::fread(ci->cmd_access, sizeof(short), magick_CA_SIZE, f);
			ACE_OS::fseek(f, sizeof(short) * (n_entries - magick_CA_SIZE), SEEK_CUR);
		    }
		}

		chan = Convert::magick_CreateChanEntry(ci);
		if (chan != NULL)
		    Magick::instance().chanserv.AddStored(chan);
		magick_delchan(ci);
	    }			/* while (fgetc(f) == 1) */
	break;			/* case 5, etc. */

    case 4:
    case 3:
	{
	    magick_ChanInfo_V3 *old_ci;

	    for (i = 33; i < 256; ++i)
		while (fgetc(f) == 1)
		{
		    ci = (magick_ChanInfo *) malloc(sizeof(magick_ChanInfo));
		    old_ci = (magick_ChanInfo_V3 *) malloc(sizeof(magick_ChanInfo_V3));
		    memset(ci, 0, sizeof(magick_ChanInfo));
		    memset(old_ci, 0, sizeof(magick_ChanInfo_V3));
		    if (1 != ACE_OS::fread(old_ci, sizeof(magick_ChanInfo_V3), 1, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
		    }
		    /* Convert old dbase! */
		    ACE_OS::strcpy(ci->mlock_on, magick_oldmodeconv(old_ci->mlock_on));
		    ACE_OS::strcpy(ci->mlock_off, magick_oldmodeconv(old_ci->mlock_off));
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
		    ci->desc = magick_read_string(f, magick_chanserv_db);
		    if (old_ci->url)
		    {
			ci->url = magick_read_string(f, magick_chanserv_db);
			if (!ACE_OS::strlen(old_ci->url))
			{
			    ci->url = NULL;
			    free(ci->url);
			}
		    }
		    else
			ci->url = NULL;
		    if (old_ci->mlock_key)
		    {
			ci->mlock_key = magick_read_string(f, magick_chanserv_db);
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
			ci->last_topic = magick_read_string(f, magick_chanserv_db);
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
			magick_ChanAccess *i_access;

			i_access = (magick_ChanAccess *) malloc(sizeof(magick_ChanAccess) * ci->accesscount);
			ci->access = i_access;
			if (ci->accesscount != (long) ACE_OS::fread(i_access, sizeof(magick_ChanAccess), ci->accesscount, f))
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
			}
			for (j = 0; j < ci->accesscount; ++j, ++i_access)
			    i_access->name = magick_read_string(f, magick_chanserv_db);
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
			    ci->access = (magick_ChanAccess *) realloc(ci->access, sizeof(magick_ChanAccess) * ci->accesscount);
			else
			{
			    free(ci->access);
			    ci->access = NULL;
			}
		    }		/* if (ci->accesscount) */

		    if (ci->akickcount)
		    {
			magick_AutoKick *akick;

			akick = (magick_AutoKick *) malloc(sizeof(magick_AutoKick) * ci->akickcount);
			ci->akick = akick;
			if (ci->akickcount != (long) ACE_OS::fread(akick, sizeof(magick_AutoKick), ci->akickcount, f))
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
			}
			for (j = 0; j < ci->akickcount; ++j, ++akick)
			{
			    akick->name = magick_read_string(f, magick_chanserv_db);
			    if (akick->reason)
				akick->reason = magick_read_string(f, magick_chanserv_db);
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
			    ci->akick = (magick_AutoKick *) realloc(ci->akick, sizeof(magick_AutoKick) * ci->akickcount);
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
			ci->cmd_access = (short *) malloc(sizeof(short) * magick_CA_SIZE);
			n_entries = fgetc(f) << 8 | fgetc(f);
			if (n_entries < 0)
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
			}
			if (n_entries <= magick_CA_SIZE)
			{
			    ACE_OS::fread(ci->cmd_access, sizeof(short), n_entries, f);
			}
			else
			{
			    ACE_OS::fread(ci->cmd_access, sizeof(short), magick_CA_SIZE, f);
			    ACE_OS::fseek(f, sizeof(short) * (n_entries - magick_CA_SIZE), SEEK_CUR);
			}
		    }
		    free(old_ci);

		    chan = Convert::magick_CreateChanEntry(ci);
		    if (chan != NULL)
			Magick::instance().chanserv.AddStored(chan);
		    magick_delchan(ci);
		}		/* while (fgetc(f) == 1) */
	    break;		/* case 3, etc. */
	}
    case 2:
    case 1:
	{
	    magick_ChanInfo_V1 *old_ci;

	    for (i = 33; i < 256; ++i)
		while (fgetc(f) == 1)
		{
		    ci = (magick_ChanInfo *) malloc(sizeof(magick_ChanInfo));
		    old_ci = (magick_ChanInfo_V1 *) malloc(sizeof(magick_ChanInfo_V1));
		    memset(ci, 0, sizeof(magick_ChanInfo));
		    memset(old_ci, 0, sizeof(magick_ChanInfo_V1));
		    if (1 != ACE_OS::fread(old_ci, sizeof(magick_ChanInfo_V1), 1, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
		    }
		    /* Convert old dbase! */
		    ACE_OS::strcpy(ci->mlock_on, magick_oldmodeconv(old_ci->mlock_on));
		    ACE_OS::strcpy(ci->mlock_off, magick_oldmodeconv(old_ci->mlock_off));
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
		    ci->desc = magick_read_string(f, magick_chanserv_db);
		    if (old_ci->mlock_key)
		    {
			ci->mlock_key = magick_read_string(f, magick_chanserv_db);
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
			ci->last_topic = magick_read_string(f, magick_chanserv_db);
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
			magick_ChanAccess *i_access;

			i_access = (magick_ChanAccess *) malloc(sizeof(magick_ChanAccess) * ci->accesscount);
			ci->access = i_access;
			if (ci->accesscount != (long) ACE_OS::fread(i_access, sizeof(magick_ChanAccess), ci->accesscount, f))
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
			}
			for (j = 0; j < ci->accesscount; ++j, ++i_access)
			    i_access->name = magick_read_string(f, magick_chanserv_db);
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
			    ci->access = (magick_ChanAccess *) realloc(ci->access, sizeof(magick_ChanAccess) * ci->accesscount);
			else
			{
			    free(ci->access);
			    ci->access = NULL;
			}
		    }		/* if (ci->accesscount) */

		    if (ci->akickcount)
		    {
			magick_AutoKick *akick;

			akick = (magick_AutoKick *) malloc(sizeof(magick_AutoKick) * ci->akickcount);
			ci->akick = akick;
			if (ci->akickcount != (long) ACE_OS::fread(akick, sizeof(magick_AutoKick), ci->akickcount, f))
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
			}
			for (j = 0; j < ci->akickcount; ++j, ++akick)
			{
			    akick->name = magick_read_string(f, magick_chanserv_db);
			    if (akick->reason)
				akick->reason = magick_read_string(f, magick_chanserv_db);
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
			    ci->akick = (magick_AutoKick *) realloc(ci->akick, sizeof(magick_AutoKick) * ci->akickcount);
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
			ci->cmd_access = (short *) malloc(sizeof(short) * magick_CA_SIZE);
			n_entries = fgetc(f) << 8 | fgetc(f);
			if (n_entries < 0)
			{
			    SLOG(LM_EMERGENCY, "Read error on $1", (magick_chanserv_db));
			}
			if (n_entries <= magick_CA_SIZE)
			{
			    ACE_OS::fread(ci->cmd_access, sizeof(short), n_entries, f);
			}
			else
			{
			    ACE_OS::fread(ci->cmd_access, sizeof(short), magick_CA_SIZE, f);
			    ACE_OS::fseek(f, sizeof(short) * (n_entries - magick_CA_SIZE), SEEK_CUR);
			}
		    }
		    free(old_ci);

		    chan = Convert::magick_CreateChanEntry(ci);
		    if (chan != NULL)
			Magick::instance().chanserv.AddStored(chan);
		    magick_delchan(ci);
		}
	    break;		/* case 1, etc. */
	}
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", (i, magick_chanserv_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

char *magick_oldmodeconv(short inmode)
{
    BTCB();
    static char outmode[magick_MODEMAX];

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

void magick_delchan(magick_ChanInfo * ci)
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

Chan_Stored_t *Convert::magick_CreateChanEntry(magick_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name == NULL || !strlen(ci->name))
	return NULL;

    if (ci->flags & magick_CI_VERBOTEN)
    {
	Chan_Stored_t *out = new Chan_Stored_t(mstring(ci->name));

	return out;
    }
    else
    {
	magick_ChanAccess *i_access;
	magick_AutoKick *akick;
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

	if (ci->flags & magick_CI_KEEPTOPIC && !out->L_Keeptopic())
	    out->setting.Keeptopic = true;
	if (ci->flags & magick_CI_SECUREOPS && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & magick_CI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & magick_CI_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & magick_CI_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & magick_CI_LEAVEOPS && !out->L_Anarchy())
	    out->setting.Anarchy = true;
	if (ci->flags & magick_CI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & magick_CI_JOIN && !out->L_Join())
	    out->setting.Join = true;
	if (!out->L_Revenge())
	{
	    char revlevel = ((ci->flags & magick_CI_REV3) << 2) | ((ci->flags & magick_CI_REV2) << 1) | (ci->flags & magick_CI_REV1);

	    switch (revlevel)
	    {
	    case magick_CR_REVERSE:
		out->setting.Revenge = "REVERSE";
		break;
	    case magick_CR_DEOP:
		out->setting.Revenge = "DEOP";
		break;
	    case magick_CR_KICK:
		out->setting.Revenge = "KICK";
		break;
	    case magick_CR_NICKBAN:
		out->setting.Revenge = "BAN1";
		break;
	    case magick_CR_USERBAN:
		out->setting.Revenge = "BAN2";
		break;
	    case magick_CR_HOSTBAN:
		out->setting.Revenge = "BAN3";
		break;
	    case magick_CR_MIRROR:
		out->setting.Revenge = "MIRROR";
		break;
	    default:
		out->setting.Revenge = "NONE";
	    }
	}

	if (ci->flags & magick_CI_SUSPENDED)
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
	    for (i = 0; i < magick_CA_SIZE; ++i)
	    {
		switch (i)
		{
		case magick_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_AUTOOP:
		    out->Level_change("AUTOOP", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_READMEMO:
		    out->Level_change("READMEMO", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_WRITEMEMO:
		    out->Level_change("WRITEMEMO", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_DELMEMO:
		    out->Level_change("DELMEMO", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_AKICK:
		    out->Level_change("AKICK", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_STARAKICK:
		    out->Level_change("SUPER", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    out->Level_change("OVERGREET", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_UNBAN:
		    out->Level_change("UNBAN", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_ACCESS:
		    out->Level_change("ACCESS", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_SET:
		    out->Level_change("SET", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_CMDINVITE:
		    out->Level_change("CMDINVITE", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_CMDUNBAN:
		    out->Level_change("CMDUNBAN", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_CMDVOICE:
		    out->Level_change("CMDVOICE", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_CMDOP:
		    out->Level_change("CMDOP", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		case magick_CA_CMDCLEAR:
		    out->Level_change("CMDCLEAR", ci->cmd_access[i], Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}
	return out;
    }
    ETCB();
}

void magick_load_memo()
{
    BTCB();
    FILE *f = ACE_OS::fopen(magick_memoserv_db, "r");
    int i, j;
    magick_MemoList *ml;
    magick_Memo *memos;

    MemoServ::nick_memo_t memo;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read MemoServ database $1", (magick_memoserv_db));
	return;
    }
    switch (i = magick_get_file_version(f, magick_memoserv_db))
    {
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	for (i = 33; i < 256; ++i)
	    while (fgetc(f) == 1)
	    {
		ml = (magick_MemoList *) malloc(sizeof(magick_MemoList));
		memset(ml, 0, sizeof(magick_MemoList));
		if (1 != ACE_OS::fread(ml, sizeof(magick_MemoList), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (magick_memoserv_db));
		}
		ml->memos = memos = (magick_Memo *) malloc(sizeof(magick_Memo) * ml->n_memos);
		memset(ml->memos, 0, sizeof(magick_Memo) * ml->n_memos);
		ACE_OS::fread(memos, sizeof(magick_Memo), ml->n_memos, f);
		for (j = 0; j < ml->n_memos; ++j, ++memos)
		    memos->text = magick_read_string(f, magick_memoserv_db);

		memo = Convert::magick_CreateMemoEntry(ml);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);
		magick_del_memolist(ml);
	    }
	break;
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", (i, magick_memoserv_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

void magick_load_news()
{
    BTCB();
    FILE *f = ACE_OS::fopen(magick_newsserv_db, "r");
    int i, j;
    magick_NewsList *nl;
    magick_Memo *newss;

    MemoServ::channel_news_t news;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read NewsServ database $1", (magick_newsserv_db));
	return;
    }
    switch (i = magick_get_file_version(f, magick_newsserv_db))
    {
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	for (i = 33; i < 256; ++i)
	{
	    while (fgetc(f) == 1)
	    {
		nl = (magick_NewsList *) malloc(sizeof(magick_NewsList));
		memset(nl, 0, sizeof(magick_NewsList));
		if (1 != ACE_OS::fread(nl, sizeof(magick_NewsList), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (magick_newsserv_db));
		}
		nl->newss = newss = (magick_Memo *) malloc(sizeof(magick_Memo) * nl->n_newss);
		memset(nl->newss, 0, sizeof(magick_Memo) * nl->n_newss);
		ACE_OS::fread(newss, sizeof(magick_Memo), nl->n_newss, f);
		for (j = 0; j < nl->n_newss; ++j, ++newss)
		    newss->text = magick_read_string(f, magick_newsserv_db);

		news = Convert::magick_CreateNewsEntry(nl);
		if (news.size())
		    Magick::instance().memoserv.AddChannel(news);
		magick_del_newslist(nl);
	    }
	}
	break;
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", (i, magick_newsserv_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

/* del_memolist:  Remove a nick's memo list from the database.  Assumes
 *                that the memo count for the nick is non-zero.
 */

void magick_del_memolist(magick_MemoList * ml)
{
    BTCB();
    int i;
    magick_Memo *memos;

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

/* del_newslist:  Remove a nick's news list from the database.  Assumes
 *                that the news count for the nick is non-zero.
 */

void magick_del_newslist(magick_NewsList * nl)
{
    BTCB();
    int i;
    magick_Memo *newss;

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

MemoServ::nick_memo_t Convert::magick_CreateMemoEntry(magick_MemoList * ml)
{
    BTCB();
    int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;
    magick_Memo *memos;

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

MemoServ::channel_news_t Convert::magick_CreateNewsEntry(magick_NewsList * nl)
{
    BTCB();
    int i;

    MemoServ::channel_news_t out;
    News_t *tmp;
    magick_Memo *newss;

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

void magick_load_sop()
{
    BTCB();
    FILE *f = ACE_OS::fopen(magick_sop_db, "r");
    int i, j, nsop = 0, sop_size = 0;
    magick_Sop *sops = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read SOP database $1", (magick_sop_db));
	return;
    }
    switch (i = magick_get_file_version(f, magick_sop_db))
    {
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
	sops = (magick_Sop *) malloc(sizeof(magick_Sop) * sop_size);
	memset(sops, 0, sizeof(magick_Sop) * sop_size);
	if (!nsop)
	{
	    fclose(f);
	    return;
	}
	if (nsop != (int) ACE_OS::fread(sops, sizeof(magick_Sop), nsop, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", (magick_sop_db));
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
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, magick_sop_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

void magick_load_message()
{
    BTCB();
    FILE *f = ACE_OS::fopen(magick_message_db, "r");
    int i, j, nmessage = 0, message_size = 0;
    magick_Message *messages = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read MESSAGE database $1", (magick_message_db));
	return;
    }
    switch (i = magick_get_file_version(f, magick_message_db))
    {
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
	messages = (magick_Message *) malloc(sizeof(magick_Message) * message_size);
	memset(messages, 0, sizeof(magick_Message) * message_size);
	if (!nmessage)
	{
	    fclose(f);
	    return;
	}
	if (nmessage != (int) ACE_OS::fread(messages, sizeof(*messages), nmessage, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", (magick_message_db));
	}
	for (j = 0; j < nmessage; ++j)
	    messages[j].text = magick_read_string(f, magick_message_db);

	for (j = 0; j < nmessage; ++j)
	{
	    if (messages[j].text == NULL)
		continue;

	    if (messages[j].type == magick_M_LOGON && Magick::instance().commserv.IsList(Magick::instance().commserv.ALL_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.ALL_Name())->
		    MSG_insert(mstring(messages[j].text), mstring(messages[j].who));
	    }
	    else if (messages[j].type == magick_M_OPER && Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
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
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, magick_message_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

void magick_load_akill()
{
    BTCB();
    FILE *f = ACE_OS::fopen(magick_akill_db, "r");
    int i, j, nakill = 0, akill_size = 0;
    magick_Akill *akills = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read AKILL database $1", (magick_akill_db));
	return;
    }
    switch (i = magick_get_file_version(f, magick_akill_db))
    {
    case 5:
    case 4:
    case 3:
    case 2:
	nakill = fgetc(f) * 256 + fgetc(f);
	if (nakill < 8)
	    akill_size = 16;
	else
	    akill_size = 2 * nakill;
	akills = (magick_Akill *) malloc(sizeof(magick_Akill) * akill_size);
	memset(akills, 0, sizeof(magick_Akill) * akill_size);
	if (!nakill)
	{
	    fclose(f);
	    return;
	}
	if (nakill != (int) ACE_OS::fread(akills, sizeof(*akills), nakill, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", (magick_akill_db));
	}
	for (j = 0; j < nakill; ++j)
	{
	    akills[j].mask = magick_read_string(f, magick_akill_db);
	    akills[j].reason = magick_read_string(f, magick_akill_db);
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
	    magick_Akill_V1 old_akill;

	    nakill = fgetc(f) * 256 + fgetc(f);
	    if (nakill < 8)
		akill_size = 16;
	    else
		akill_size = 2 * nakill;
	    akills = (magick_Akill *) malloc(sizeof(magick_Akill) * akill_size);
	    memset(akills, 0, sizeof(magick_Akill) * akill_size);
	    if (!nakill)
	    {
		fclose(f);
		return;
	    }
	    for (j = 0; j < nakill; ++j)
	    {
		if (1 != ACE_OS::fread(&old_akill, sizeof(old_akill), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", (magick_akill_db));
		}
		akills[j].time = old_akill.time;
		akills[j].who[0] = 0;
	    }
	    for (j = 0; j < nakill; ++j)
	    {
		akills[j].mask = magick_read_string(f, magick_akill_db);
		akills[j].reason = magick_read_string(f, magick_akill_db);
	    }
	}			/* case 1 */

	for (j = 0; j < nakill; ++j)
	{
	    if (akills[j].mask != NULL && akills[j].reason != NULL)
	    {
		if (akills[j].time == 0)
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[j].mask), Magick::instance().operserv.Expire_Sop(),
							     mstring(akills[j].reason), Magick::instance().operserv.FirstName());
		}
		else
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[j].mask), Magick::instance().operserv.Def_Expire(),
							     mstring(akills[j].reason), Magick::instance().operserv.FirstName(),
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
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, magick_akill_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

void magick_load_clone()
{
    BTCB();
    FILE *f = ACE_OS::fopen(magick_clone_db, "r");
    int i, j, nclone = 0, clone_size = 0;
    magick_Allow *clones = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read CLONE database $1", (magick_clone_db));
	return;
    }
    switch (i = magick_get_file_version(f, magick_clone_db))
    {
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
	clones = (magick_Allow *) malloc(sizeof(magick_Allow) * clone_size);
	memset(clones, 0, sizeof(magick_Allow) * clone_size);
	if (!nclone)
	{
	    fclose(f);
	    return;
	}
	if (nclone != (int) ACE_OS::fread(clones, sizeof(*clones), nclone, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", (magick_clone_db));
	}
	for (j = 0; j < nclone; ++j)
	{
	    clones[j].host = magick_read_string(f, magick_clone_db);
	    clones[j].reason = magick_read_string(f, magick_clone_db);
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
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, magick_clone_db));
    }				/* switch (version) */
    fclose(f);
    ETCB();
}

#endif /* CONVERT */
