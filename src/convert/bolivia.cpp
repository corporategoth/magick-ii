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
#define RCSID(x,y) const char *rcsid_convert_bolivia_cpp_ ## x () { return y; }
RCSID(convert_bolivia_cpp, "@(#)$Id$");

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

/* Based upon the DB loading routines from Bolivia 1.2.0
 * (c) 2001 IRC Services <evilborg@irc.squirrels.net>
 */

#include "magick.h"
#include "convert/interface.h"
#include "convert/bolivia.h"

/* Should we try and deal with old (v2.x) databases?  (Don't undefine this
 * unless you know what you're doing!) */
#define bolivia_COMPATIBILITY_V2

/* Database filenames */
#define bolivia_NICKSERV_DB	"nick.db"
#define bolivia_CHANSERV_DB	"chan.db"
#define bolivia_MEMOSERV_DB	"memo.db"
#define bolivia_OS_SA_DB	"os_sa.db"
#define bolivia_OS_SOP_DB       "os_sop.db"
#define bolivia_AKILL_DB	"akill.db"
#define bolivia_IGNORE_DB       "ignore.db"
#define bolivia_TRIGGER_DB	"trigger.db"

#define bolivia_CHANMAX  64
#define bolivia_NICKMAX  32
#define bolivia_PASSMAX  32
#define bolivia_DEF_MAX_MEMOS 	50

/* Version number for data files; if structures below change, increment
 * this.  (Otherwise -very- bad things will happen!) */

#define bolivia_FILE_VERSION    5
/*************************************************************************/

/* Nickname info structure.  Each nick structure is stored in one of 256
 * lists; the list is determined by the first character of the nick.  Nicks
 * are stored in alphabetical order within lists. */

struct bolivia_NickInfo {
    bolivia_NickInfo *next, *prev;
    char nick[bolivia_NICKMAX];
    char pass[bolivia_PASSMAX];
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
    char *banish;     /*  }
    long reserved[6];                   For future expansion -- set to 0 */
};

#define bolivia_NI_KILLPROTECT	0x00000001  /* Kill others who take this nick */
#define bolivia_NI_SECURE	0x00000002  /* Don't recognize unless IDENTIFY'd */
#define bolivia_NI_VERBOTEN	0x00000004  /* Nick may not be registered or used */
#define bolivia_NI_ENCRYPTEDPW	0x00000008  /* Nickname password is encrypted */
#define bolivia_NI_MEMO_SIGNON	0x00000010  /* Notify of memos at signon and un-away */
#define bolivia_NI_MEMO_RECEIVE	0x00000020  /* Notify of new memos when sent */
#define bolivia_NI_PRIVATE	0x00000040  /* Don't show in LIST to non-servadmins */
#define bolivia_NI_HIDE_EMAIL   0x00000080
#define bolivia_NI_MARK		0x00000100
#define bolivia_NI_HOLD		0x00000200
#define bolivia_NI_EMAILMEMOS   0x00000400
#define bolivia_NI_NOOP         0x00000800
#define bolivia_NI_NOMEMO       0x00001000
#define bolivia_NI_NOHOST	0x00002000
#define bolivia_NI_SENDLOGS     0x00004000
#define bolivia_NI_NEVEROP      0x00008000
#define bolivia_NI_NEWS		0x00010000
#define bolivia_NI_FREEZE       0x00020000

#define bolivia_NI_IDENTIFIED	0x80000000  /* User has IDENTIFY'd */
#define bolivia_NI_RECOGNIZED	0x40000000  /* User comes from a known addy */
#define bolivia_NI_KILL_HELD	0x20000000  /* Nick is being held after a kill */

#ifdef IRC_GIRLS
#define bolivia_NI_GENDERG      0x10000000
#define bolivia_NI_GENDERB      0x08000000
#define bolivia_NI_GENDERH      0x04000000
#endif

/*************************************************************************/
/* Channel info structures.  Stored similarly to the nicks, except that
 * the second character of the channel name, not the first, is used to
 * determine the list.  (Hashing based on the first character of the name
 * wouldn't get very far. ;) ) */

/* Access levels for users. */
struct bolivia_ChanAccess {
    short level;
#ifdef bolivia_COMPATIBILITY_V2
    short is_nick;	/* 1 if this is a nick, 0 if a user@host mask.  If
			 * -1, then this entry is currently unused (a hack
			 * to get numbered lists to have consistent
			 * numbering). */
# define in_use is_nick
#else
    short in_use;      /* pretty well obsolete  - Mar 26, 99 */
#endif
    char *name;
};

#define bolivia_ACCESS_FOUNDER	10000	/* Numeric level indicating founder access */
#define bolivia_ACCESS_INVALID	-10000	/* Used in levels[] for disabled settings */

/* bolivia_AutoKick data. */
struct bolivia_AutoKick {
    short is_nick;
    short pad;
    char *name;
    char *reason;
};

struct bolivia_ChanInfo {
    bolivia_ChanInfo *next, *prev;
    char name[bolivia_CHANMAX];
    char founder[bolivia_NICKMAX];		/* Always a reg'd nick */
    char founderpass[bolivia_PASSMAX];
    char *desc;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    bolivia_ChanAccess *access;			/* List of authorized users */
    long akickcount;
    bolivia_AutoKick *akick;
    short mlock_on, mlock_off;		/* See channel modes below */
    long mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[bolivia_NICKMAX];	/* Who set the last topic */
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
    long reserved[5];                   /* For future expansion -- set to 0 */
};

/* Retain topic even after last person leaves channel */
#define bolivia_CI_KEEPTOPIC	0x00000001
/* Don't allow non-authorized users to be opped */
#define bolivia_CI_OPGUARD	0x00000002
/* Vorbose on Channel: */
#define bolivia_CI_VERBOSE	0x00000004
/* Topic can only be changed by SET TOPIC */
#define bolivia_CI_TOPICLOCK	0x00000008
/* Those not allowed ops are kickbanned */
#define bolivia_CI_RESTRICTED	0x00000010
/* Don't auto-deop anyone */
#define bolivia_CI_LEAVEOPS	0x00000020
/* Don't allow any privileges unless a user is IDENTIFY'd with NickServ */
#define bolivia_CI_IDENT	0x00000040
/* Don't allow the channel to be registered or used */
#define bolivia_CI_VERBOTEN	0x00000080
/* Channel password is encrypted */
#define bolivia_CI_ENCRYPTEDPW	0x00000100
// Held channel
#define bolivia_CI_HELDCHAN     0x00000200
// marked channel
#define bolivia_CI_MARKCHAN     0x00000400
// Only Opers can join
#define bolivia_CI_OPERONLY     0x00000800
// Only SOP+ can join
#define bolivia_CI_SOPONLY      0x00001000
// Only SA+ can join
#define bolivia_CI_SAONLY       0x00002000
// Only SRA+ can join
#define bolivia_CI_SRAONLY      0x00004000
// Only Coders can join
#define bolivia_CI_CODERONLY    0x00008000
// Only Abuse Team can join
#define bolivia_CI_ABUSEONLY	0x00010000
// Frozen Channel
#define bolivia_CI_FREEZECHAN   0x00020000
// Memo Levels
#define bolivia_CI_MEMO_AV      0x00040000
#define bolivia_CI_MEMO_AOP     0x00080000
#define bolivia_CI_MEMO_SOP     0x00100000
#define bolivia_CI_MEMO_CF      0x00200000
#define bolivia_CI_MEMO_FR      0x00400000
#define bolivia_CI_MEMO_NONE    0x00800000
#define bolivia_CI_UNSECURE     0x01000000

#ifdef bolivia_COMPATIBILITY_V2
/* Channel's access list has masks in it, not just registered nicks */
#define bolivia_CI_HAS_MASKS	0x80000000
#endif

/* Indices for cmd_access[]: */
#define bolivia_CA_INVITE	0
#define bolivia_CA_AKICK	1
#define bolivia_CA_SET		2	/* but not FOUNDER or PASSWORD */
#define bolivia_CA_UNBAN	3
#define bolivia_CA_AUTOOP	4
#define bolivia_CA_AUTODEOP	5	/* Maximum, not minimum */
#define bolivia_CA_AUTOVOICE	6
#define bolivia_CA_OPDEOP	7	/* ChanServ commands OP and DEOP */
#define bolivia_CA_ACCESS_LIST	8
#define bolivia_CA_CLEAR	9
#define bolivia_CA_NOJOIN	10	/* Maximum */
#define bolivia_CA_ACCESS_CHANGE 11

#define bolivia_CA_SIZE		12

int bolivia_def_levels[][2] = {
    { bolivia_CA_AUTOOP,             5 },
    { bolivia_CA_AUTOVOICE,          3 },
    { bolivia_CA_AUTODEOP,          -1 },
    { bolivia_CA_NOJOIN,            -1 },
    { bolivia_CA_INVITE,             5 },
    { bolivia_CA_AKICK,             10 },
    { bolivia_CA_SET,               13 },
    { bolivia_CA_CLEAR,              4 },
    { bolivia_CA_UNBAN,              5 },
    { bolivia_CA_OPDEOP,             5 },
    { bolivia_CA_ACCESS_LIST,        3 },
    { bolivia_CA_ACCESS_CHANGE,      1 },
    { -1 }
};

#define bolivia_CMODE_I 0x00000001
#define bolivia_CMODE_M 0x00000002
#define bolivia_CMODE_N 0x00000004
#define bolivia_CMODE_P 0x00000008
#define bolivia_CMODE_S 0x00000010
#define bolivia_CMODE_T 0x00000020
#define bolivia_CMODE_K 0x00000040		/* These two used only by ChanServ */
#define bolivia_CMODE_L 0x00000080
#define bolivia_CMODE_R 0x00000100
#define bolivia_CMODE_r 0x00000200
#define bolivia_CMODE_C 0x00000400
#define bolivia_CMODE_m 0x00000800

/*************************************************************************/
/* MemoServ data.  Only nicks that actually have memos get records in
 * MemoServ's lists, which are stored the same way NickServ's are. */

struct bolivia_Memo {
    char sender[bolivia_NICKMAX];
    long number;	/* Index number -- not necessarily array position! */
    time_t time;	/* When it was sent */
    char *text;
    char *chan;
    short flags;
    short reserved_s;
    long reserved[3];   /* For future expansion -- set to 0 */
};

#define bolivia_MF_UNREAD	0x0001	/* bolivia_Memo has not yet been read */
#define bolivia_MF_DEL          0x0002  /* bolivia_Memo marked as deleted */

struct bolivia_MemoList {
    bolivia_MemoList *next, *prev;
    char nick[bolivia_NICKMAX];	/* Owner of the memos */
    long n_memos;	/* Number of memos */
    bolivia_Memo *memos;	/* The memos themselves */
    long reserved[4];   /* For future expansion -- set to 0 */
};

struct bolivia_Akill {
    char *mask;
    char *reason;
    char who[bolivia_NICKMAX];
    time_t time;
    time_t expires;	/* or 0 for no expiry */
    long reserved[4];
};

struct bolivia_Ignore {
    char *mask;
    char who[bolivia_NICKMAX];
    time_t time;
    time_t expires;	/* or 0 for no expiry */
    long reserved[4];
};

struct bolivia_Trigger {
    char *mask;
    long tvalue;
    char who[bolivia_NICKMAX];
    long reserved[4];
};


int bolivia_get_file_version(FILE *f, const char *filename)
{
    int version = fgetc(f)<<24 | fgetc(f)<<16 | fgetc(f)<<8 | fgetc(f);
    if (ferror(f))
	SLOG(LM_EMERGENCY, "Error reading version number on $1", (filename));
    else if (ferror(f))
	SLOG(LM_EMERGENCY, "Error reading version number on $1: End of file detected",(
		filename));
    else if (version > bolivia_FILE_VERSION || version < 1)
	SLOG(LM_EMERGENCY, "Invalid version number ($1) on $2",( version, filename));
    return version;
}

FILE *bolivia_open_db_read(const char *service, const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f) {
	if (errno != ENOENT)
	    SLOG(LM_ERROR, "Can't read $1 database $2",( service, filename));
	return NULL;
    }
    return f;
}

FILE *bolivia_open_db(const char *service, const char *filename, const char *mode)
{
    if (*mode == 'r') {
	return bolivia_open_db_read(service, filename);
    } else {
	errno = EINVAL;
	return NULL;
    }
}

void bolivia_close_db(FILE *dbfile, const char *filename)
{
    int flags;

    flags = fcntl(fileno(dbfile), F_GETFL);
    if ((flags != -1) &&
		(((flags & O_ACCMODE) == O_WRONLY) ||
		 ((flags & O_ACCMODE) == O_RDWR)))
    {
	char namebuf[NAME_MAX+1];
	snprintf(namebuf, sizeof(namebuf), "%s.save", filename);
	if (*namebuf && strcmp(namebuf, filename) != 0)
	    remove(namebuf);
    }
    fclose(dbfile);
}

char *bolivia_read_string(FILE *f, const char *filename)
{
    char *s;
    int len;

    len = fgetc(f) * 256 + fgetc(f);
    s = (char *) malloc(len);
    if ((size_t) len != fread(s, 1, len, f))
        SLOG(LM_EMERGENCY, "Read error on file: $1", (filename));
    return s;
}

int bolivia_delnick(bolivia_NickInfo *ni);
int bolivia_delchan(bolivia_ChanInfo *ci);
void bolivia_reset_levels(bolivia_ChanInfo *ci);
void bolivia_del_memolist(bolivia_MemoList *ml);

void bolivia_load_nick()
{
    FILE *f;
    int ver, i, j;
    bolivia_NickInfo *ni;

    if (!(f = bolivia_open_db("NickServ", bolivia_NICKSERV_DB, "r")))
	return;

    switch (ver = bolivia_get_file_version(f, bolivia_NICKSERV_DB)) {
      case 5:
        for (i = 33; i < 256; ++i) {
            while (fgetc(f) == 1) {
                ni = (bolivia_NickInfo *) malloc(sizeof(bolivia_NickInfo));
                if (1 != fread(ni, sizeof(bolivia_NickInfo), 1, f))
                    SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_NICKSERV_DB));
                if (ni->flags & bolivia_NI_ENCRYPTEDPW) {
                    /* Bail: it makes no sense to continue with encrypted
                    * passwords, since we won't be able to verify them */
                    SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted "
                          "but encryption disabled, aborting",(
                          "NickServ", ni->nick));
                }
                ni->flags &= ~(bolivia_NI_IDENTIFIED | bolivia_NI_RECOGNIZED);
                if (ni->url)
                    ni->url = bolivia_read_string(f, bolivia_NICKSERV_DB);
                if (ni->email)
                    ni->email = bolivia_read_string(f, bolivia_NICKSERV_DB);
		if (ni->forward)
		    ni->forward = bolivia_read_string(f, bolivia_NICKSERV_DB);
                if (ni->hold)
                    ni->hold = bolivia_read_string(f, bolivia_NICKSERV_DB);
                if (ni->mark)
                    ni->mark = bolivia_read_string(f, bolivia_NICKSERV_DB);
                if (ni->banish)
                    ni->banish = bolivia_read_string(f, bolivia_NICKSERV_DB);
                ni->last_usermask = bolivia_read_string(f, bolivia_NICKSERV_DB);
                ni->last_realname = bolivia_read_string(f, bolivia_NICKSERV_DB);
                if (ni->accesscount) {
                    char **access;
                    access = (char **) malloc(sizeof(char *) * ni->accesscount);
                    ni->access = access;
                    for (j = 0; j < ni->accesscount; j++, access++)
                        *access = bolivia_read_string(f, bolivia_NICKSERV_DB);
                }
                ni->id_timestamp = 0;
                if (ver < 3) {
                    ni->memomax = bolivia_DEF_MAX_MEMOS;
                    ni->flags |= bolivia_NI_MEMO_SIGNON | bolivia_NI_MEMO_RECEIVE;
                } else if (ver == 3) {
                    if (!(ni->flags & (bolivia_NI_MEMO_SIGNON | bolivia_NI_MEMO_RECEIVE |
                        bolivia_NI_NOOP | bolivia_NI_NOMEMO)))
                        ni->flags |= bolivia_NI_MEMO_SIGNON | bolivia_NI_MEMO_RECEIVE |
                        bolivia_NI_NOOP | bolivia_NI_NOMEMO;
                }

		Nick_Stored_t *nick = Convert::bolivia_CreateNickEntry(ni);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);
		bolivia_delnick(ni);

            } /* while (fgetc(f) == 1) */
        } /* for (i) */
        break;

      case 4:
      case 3:
      case 2:
      case 1:

	for (i = 33; i < 256; ++i) {
	    while (fgetc(f) == 1) {
		ni = (bolivia_NickInfo *) malloc(sizeof(bolivia_NickInfo));
		if (1 != fread(ni, sizeof(bolivia_NickInfo), 1, f))
		    SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_NICKSERV_DB));
		if (ni->flags & bolivia_NI_ENCRYPTEDPW) {
		    /* Bail: it makes no sense to continue with encrypted
		    * passwords, since we won't be able to verify them */
		    SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted "
		          "but encryption disabled, aborting",(
		          "NickServ", ni->nick));
		}
		ni->flags &= ~(bolivia_NI_IDENTIFIED | bolivia_NI_RECOGNIZED);
		if (ni->url)
		    ni->url = bolivia_read_string(f, bolivia_NICKSERV_DB);
		if (ni->email)
		    ni->email = bolivia_read_string(f, bolivia_NICKSERV_DB);
		ni->last_usermask = bolivia_read_string(f, bolivia_NICKSERV_DB);
		ni->last_realname = bolivia_read_string(f, bolivia_NICKSERV_DB);
		if (ni->accesscount) {
		    char **access;
		    access = (char **) malloc(sizeof(char *) * ni->accesscount);
		    ni->access = access;
		    for (j = 0; j < ni->accesscount; j++, access++)
			*access = bolivia_read_string(f, bolivia_NICKSERV_DB);
		}
		ni->id_timestamp = 0;
		if (ver < 3) {
		    ni->memomax = bolivia_DEF_MAX_MEMOS;
		    ni->flags |= bolivia_NI_MEMO_SIGNON | bolivia_NI_MEMO_RECEIVE;
		} else if (ver == 3) {
                    if (!(ni->flags & (bolivia_NI_MEMO_SIGNON | bolivia_NI_MEMO_RECEIVE |
                        bolivia_NI_NOOP | bolivia_NI_NOMEMO)))
                        ni->flags |= bolivia_NI_MEMO_SIGNON | bolivia_NI_MEMO_RECEIVE |
                        bolivia_NI_NOOP | bolivia_NI_NOMEMO;

		}

		Nick_Stored_t *nick = Convert::bolivia_CreateNickEntry(ni);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);
		bolivia_delnick(ni);

	    } /* while (fgetc(f) == 1) */
	} /* for (i) */
	break;

      default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2",(ver, bolivia_NICKSERV_DB));
    } /* switch (version) */

    bolivia_close_db(f, bolivia_NICKSERV_DB);
}

int bolivia_delnick(bolivia_NickInfo *ni)
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
    if (ni->url)
      free(ni->url);
    if (ni->email)
      free(ni->email);
    if (ni->forward)
      free(ni->forward);
    if (ni->hold)
      free(ni->hold);
    if (ni->mark)
      free(ni->mark);
    if (ni->banish)
      free(ni->banish);
    free(ni);
    return 1;
}

void bolivia_load_chan()
{
    FILE *f = fopen(bolivia_CHANSERV_DB, "r");
    int ver, i, j;
    bolivia_ChanInfo *ci;

    if (!(f = bolivia_open_db("ChanServ", bolivia_CHANSERV_DB, "r")))
	return;

    switch (ver = bolivia_get_file_version(f, bolivia_CHANSERV_DB)) {
      case 5:
        for (i = 33; i < 256; ++i) {

            while (fgetc(f) == 1) {

                ci = (bolivia_ChanInfo *) malloc(sizeof(bolivia_ChanInfo));
                if (1 != fread(ci, sizeof(bolivia_ChanInfo), 1, f))
                    SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_CHANSERV_DB));
                if (ci->flags & bolivia_CI_ENCRYPTEDPW) {
                    /* Bail: it makes no sense to continue with encrypted
                     * passwords, since we won't be able to verify them */
                    SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted "
                          "but encryption disabled, aborting",(
                          "ChanServ", ci->name));
                }
                /* Can't guarantee the file is in a particular order...
                 * (Well, we can, but we don't have to depend on it.) */
                ci->desc = bolivia_read_string(f, bolivia_CHANSERV_DB);
                if (ci->url)
                    ci->url = bolivia_read_string(f, bolivia_CHANSERV_DB);
                if (ci->email)
                    ci->email = bolivia_read_string(f, bolivia_CHANSERV_DB);
                if (ci->mlock_key)
                    ci->mlock_key = bolivia_read_string(f, bolivia_CHANSERV_DB);
                if (ci->last_topic)
                    ci->last_topic = bolivia_read_string(f, bolivia_CHANSERV_DB);
		if (ci->welcome)
		    ci->welcome = bolivia_read_string(f, bolivia_CHANSERV_DB);
                if (ci->hold)
                    ci->hold = bolivia_read_string(f, bolivia_CHANSERV_DB);
                if (ci->mark)
                    ci->mark = bolivia_read_string(f, bolivia_CHANSERV_DB);
                if (ci->freeze)
                    ci->freeze = bolivia_read_string(f, bolivia_CHANSERV_DB);
                if (ci->forbid)
                    ci->forbid = bolivia_read_string(f, bolivia_CHANSERV_DB);

                if (ci->accesscount) {
                    bolivia_ChanAccess *access;
                    access = (bolivia_ChanAccess *) malloc(sizeof(bolivia_ChanAccess) * ci->accesscount);
                    ci->access = access;
                    if ((size_t) ci->accesscount != fread(access, sizeof(bolivia_ChanAccess),
                                                        ci->accesscount, f))
                        SLOG(LM_EMERGENCY, "Read error on ", (bolivia_CHANSERV_DB));
                    for (j = 0; j < ci->accesscount; ++j, ++access)
                        access->name = bolivia_read_string(f, bolivia_CHANSERV_DB);
                    j = 0; access = ci->access;
                    /* Clear out unused entries */
                    while (j < ci->accesscount) {
#ifdef bolivia_COMPATIBILITY_V2
                        if (access->is_nick < 0 ||
                            (access->is_nick == 0 && !Magick::instance().nickserv.IsStored(access->name))
                        ) {

#else
                        if (!access->in_use) {
#endif
                            --ci->accesscount;
                            free(access->name);
                            if (j < ci->accesscount)
                                bcopy(access+1, access, sizeof(*access) *
                                                        (ci->accesscount - j));
                        } else {
#ifdef bolivia_COMPATIBILITY_V2
                            access->is_nick = 1;
#endif
                            ++j; ++access;
                        }
                    }
                    if (ci->accesscount)
                        ci->access = (bolivia_ChanAccess *) realloc(ci->access,
                                        sizeof(bolivia_ChanAccess) * ci->accesscount);
                    else {
                        free(ci->access);
                        ci->access = NULL;
                    }
                } /* if (ci->accesscount) */

                if (ci->akickcount) {
                    bolivia_AutoKick *akick;
                    akick = (bolivia_AutoKick *) malloc(sizeof(bolivia_AutoKick) * ci->akickcount);
                    ci->akick = akick;
                    if ((size_t) ci->akickcount !=
                            fread(akick, sizeof(bolivia_AutoKick), ci->akickcount, f))
                        SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_CHANSERV_DB));
                    for (j = 0; j < ci->akickcount; ++j, ++akick) {
                        akick->name = bolivia_read_string(f, bolivia_CHANSERV_DB);
                        if (akick->reason)
                            akick->reason = bolivia_read_string(f, bolivia_CHANSERV_DB);
                    }
                    j = 0; akick = ci->akick;
                    while (j < ci->akickcount) {
                        if (akick->is_nick < 0) {
                            --ci->akickcount;
                            free(akick->name);
                            if (akick->reason)
                                free(akick->reason);
                            if (j < ci->akickcount)
                                bcopy(akick+1, akick, sizeof(*akick) *
                                                        (ci->akickcount - j));
                        } else {
                            ++j; ++akick;
                        }
                    }
                    if (ci->akickcount) {
                        ci->akick = (bolivia_AutoKick *) realloc(ci->akick,
                                        sizeof(bolivia_AutoKick) * ci->akickcount);
                    } else {
                        free(ci->akick);
                        ci->akick = NULL;
                    }
                } /* if (ci->akickcount) */
                if (ci->levels) {
                    int n_entries;
                    ci->levels = NULL;
                    bolivia_reset_levels(ci);
                    n_entries = fgetc(f)<<8 | fgetc(f);
                    if (n_entries < 0)
                        SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_CHANSERV_DB));
#ifdef bolivia_COMPATIBILITY_V2
                    /* bolivia_Ignore earlier, incompatible levels list */
                    if (n_entries == 6) {
                        fseek(f, sizeof(short) * n_entries, SEEK_CUR);
                    } else
#endif
                    if (n_entries <= bolivia_CA_SIZE) {
                        fread(ci->levels, sizeof(short), n_entries, f);
                    } else {
                        fread(ci->levels, sizeof(short), bolivia_CA_SIZE, f);
                        fseek(f, sizeof(short) * (n_entries - bolivia_CA_SIZE),
                                                                SEEK_CUR);
                    }
                } else {
                    bolivia_reset_levels(ci);
                }

		Chan_Stored_t *chan = Convert::bolivia_CreateChanEntry(ci);
		if (chan != NULL)
		    Magick::instance().chanserv.AddStored(chan);
		bolivia_delchan(ci);

            } /* while (fgetc(f) == 1) */

        } /* for (i) */

        break; /* case 1, etc. */

      case 4:
      case 3:
      case 2:
      case 1:

	for (i = 33; i < 256; ++i) {

	    while (fgetc(f) == 1) {

		ci = (bolivia_ChanInfo *) malloc(sizeof(bolivia_ChanInfo));
		if (1 != fread(ci, sizeof(bolivia_ChanInfo), 1, f))
		    SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_CHANSERV_DB));
		if (ci->flags & bolivia_CI_ENCRYPTEDPW) {
		    /* Bail: it makes no sense to continue with encrypted
		     * passwords, since we won't be able to verify them */
		    SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted "
		          "but encryption disabled, aborting",(
		          "ChanServ", ci->name));
		}
		/* Can't guarantee the file is in a particular order...
		 * (Well, we can, but we don't have to depend on it.) */
		ci->desc = bolivia_read_string(f, bolivia_CHANSERV_DB);
		if (ci->url)
		    ci->url = bolivia_read_string(f, bolivia_CHANSERV_DB);
		if (ci->email)
		    ci->email = bolivia_read_string(f, bolivia_CHANSERV_DB);
		if (ci->mlock_key)
		    ci->mlock_key = bolivia_read_string(f, bolivia_CHANSERV_DB);
		if (ci->last_topic)
		    ci->last_topic = bolivia_read_string(f, bolivia_CHANSERV_DB);

		if (ci->accesscount) {
		    bolivia_ChanAccess *access;
		    access = (bolivia_ChanAccess *) malloc(sizeof(bolivia_ChanAccess) * ci->accesscount);
		    ci->access = access;
		    if ((size_t) ci->accesscount != fread(access, sizeof(bolivia_ChanAccess),
							ci->accesscount, f))
			SLOG(LM_EMERGENCY, "Read error on ", (bolivia_CHANSERV_DB));
		    for (j = 0; j < ci->accesscount; ++j, ++access)
			access->name = bolivia_read_string(f, bolivia_CHANSERV_DB);
		    j = 0; access = ci->access;
		    /* Clear out unused entries */
		    while (j < ci->accesscount) {
#ifdef bolivia_COMPATIBILITY_V2
			if (access->is_nick < 0 ||
			    (access->is_nick == 0 && !Magick::instance().nickserv.IsStored(access->name))
			) {

#else
			if (!access->in_use) {
#endif
			    --ci->accesscount;
			    free(access->name);
			    if (j < ci->accesscount)
				bcopy(access+1, access,	sizeof(*access) *
							(ci->accesscount - j));
			} else {
#ifdef bolivia_COMPATIBILITY_V2
			    access->is_nick = 1;
#endif
			    ++j; ++access;
			}
		    }
		    if (ci->accesscount)
			ci->access = (bolivia_ChanAccess *) realloc(ci->access,
					sizeof(bolivia_ChanAccess) * ci->accesscount);
		    else {
			free(ci->access);
			ci->access = NULL;
		    }
		} /* if (ci->accesscount) */

		if (ci->akickcount) {
		    bolivia_AutoKick *akick;
		    akick = (bolivia_AutoKick *) malloc(sizeof(bolivia_AutoKick) * ci->akickcount);
		    ci->akick = akick;
		    if ((size_t) ci->akickcount !=
			    fread(akick, sizeof(bolivia_AutoKick), ci->akickcount, f))
			SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_CHANSERV_DB));
		    for (j = 0; j < ci->akickcount; ++j, ++akick) {
			akick->name = bolivia_read_string(f, bolivia_CHANSERV_DB);
			if (akick->reason)
			    akick->reason = bolivia_read_string(f, bolivia_CHANSERV_DB);
		    }
		    j = 0; akick = ci->akick;
		    while (j < ci->akickcount) {
			if (akick->is_nick < 0) {
			    --ci->akickcount;
			    free(akick->name);
			    if (akick->reason)
				free(akick->reason);
			    if (j < ci->akickcount)
				bcopy(akick+1, akick, sizeof(*akick) *
							(ci->akickcount - j));
			} else {
			    ++j; ++akick;
			}
		    }
		    if (ci->akickcount) {
			ci->akick = (bolivia_AutoKick *) realloc(ci->akick,
					sizeof(bolivia_AutoKick) * ci->akickcount);
		    } else {
			free(ci->akick);
			ci->akick = NULL;
		    }
		} /* if (ci->akickcount) */

		if (ci->levels) {
		    int n_entries;
		    ci->levels = NULL;
		    bolivia_reset_levels(ci);
		    n_entries = fgetc(f)<<8 | fgetc(f);
		    if (n_entries < 0)
			SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_CHANSERV_DB));
#ifdef bolivia_COMPATIBILITY_V2
		    /* bolivia_Ignore earlier, incompatible levels list */
		    if (n_entries == 6) {
			fseek(f, sizeof(short) * n_entries, SEEK_CUR);
		    } else
#endif
		    if (n_entries <= bolivia_CA_SIZE) {
			fread(ci->levels, sizeof(short), n_entries, f);
		    } else {
			fread(ci->levels, sizeof(short), bolivia_CA_SIZE, f);
			fseek(f, sizeof(short) * (n_entries - bolivia_CA_SIZE),
								SEEK_CUR);
		    }
		} else {
		    bolivia_reset_levels(ci);
		}

		Chan_Stored_t *chan = Convert::bolivia_CreateChanEntry(ci);
		if (chan != NULL)
		    Magick::instance().chanserv.AddStored(chan);
		bolivia_delchan(ci);

	    } /* while (fgetc(f) == 1) */

	} /* for (i) */

	break; /* case 1, etc. */

      default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2",(ver, bolivia_CHANSERV_DB));

    } /* switch (version) */

    bolivia_close_db(f, bolivia_CHANSERV_DB);
}

int bolivia_delchan(bolivia_ChanInfo *ci)
{
    int i;

    if (ci->desc)
	free(ci->desc);
    if (ci->mlock_key)
	free(ci->mlock_key);
    if (ci->last_topic)
	free(ci->last_topic);
    for (i = 0; i < ci->accesscount; ++i)
	free(ci->access[i].name);
    if (ci->access)
	free(ci->access);
    for (i = 0; i < ci->akickcount; ++i) {
	free(ci->akick[i].name);
	if (ci->akick[i].reason)
	    free(ci->akick[i].reason);
    }
    if (ci->akick)
	free(ci->akick);
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

    free(ci);
    return 1;
}

mstring bolivia_getmodes(short modes)
{
    BTCB();
    mstring retval;

    if (modes & bolivia_CMODE_I)
	retval += "i";
    if (modes & bolivia_CMODE_M)
	retval += "m";
    if (modes & bolivia_CMODE_N)
	retval += "n";
    if (modes & bolivia_CMODE_P)
	retval += "p";
    if (modes & bolivia_CMODE_S)
	retval += "s";
    if (modes & bolivia_CMODE_T)
	retval += "t";
    if (modes & bolivia_CMODE_K)
	retval += "k";
    if (modes & bolivia_CMODE_L)
	retval += "l";
    if (modes & bolivia_CMODE_R)
	retval += "r";
    if (modes & bolivia_CMODE_r)
	retval += "R";
    if (modes & bolivia_CMODE_C)
	retval += "c";
    if (modes & bolivia_CMODE_m)
	retval += "M";

    return retval;
    ETCB();
}


void bolivia_reset_levels(bolivia_ChanInfo *ci)
{
    int i;

    if (ci->levels)
	free(ci->levels);
    ci->levels = (short *) malloc(bolivia_CA_SIZE * sizeof(*ci->levels));
    for (i = 0; bolivia_def_levels[i][0] >= 0; i++)
	ci->levels[bolivia_def_levels[i][0]] = bolivia_def_levels[i][1];
}

void bolivia_load_memo()
{
    FILE *f;
    int ver, i, j;
    bolivia_MemoList *ml;
    bolivia_Memo *memos;

    if (!(f = bolivia_open_db("MemoServ", bolivia_MEMOSERV_DB, "r")))
	return;
    switch (ver = bolivia_get_file_version(f, bolivia_MEMOSERV_DB)) {
      case 5:
      case 4:
      case 3:
      case 2:
      case 1:
	for (i = 33; i < 256; ++i) {
	    while (fgetc(f) == 1) {
		ml = (bolivia_MemoList *) malloc(sizeof(bolivia_MemoList));
		if (1 != fread(ml, sizeof(bolivia_MemoList), 1, f))
		    SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_MEMOSERV_DB));
		ml->memos = memos = (bolivia_Memo *) malloc(sizeof(bolivia_Memo) * ml->n_memos);
		fread(memos, sizeof(bolivia_Memo), ml->n_memos, f);
                for (j = 0; j < ml->n_memos; ++j, ++memos) {
		    memos->text = bolivia_read_string(f, bolivia_MEMOSERV_DB);
                  if (memos->chan)
                    memos->chan = bolivia_read_string(f, bolivia_MEMOSERV_DB);
                }

		MemoServ::nick_memo_t memo = Convert::bolivia_CreateMemoEntry(ml);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);
		bolivia_del_memolist(ml);
	    }
	}
	break;
      default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2",( ver, bolivia_MEMOSERV_DB));
    } /* switch (version) */
    bolivia_close_db(f, bolivia_MEMOSERV_DB);
}

void bolivia_del_memolist(bolivia_MemoList *ml)
{
    int i;
    bolivia_Memo *memos;

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
}

void bolivia_load_sa()
{
    FILE *f;
    int i;

    if (!(f = bolivia_open_db("OperServ", bolivia_OS_SA_DB, "r")))
	return;
    switch (i = bolivia_get_file_version(f, bolivia_OS_SA_DB)) {
      case 5:
      case 4:
      case 3:
	i = fgetc(f)<<8 | fgetc(f);
	while (--i >= 0)
	{
	    char *sa = bolivia_read_string(f, bolivia_OS_SA_DB);
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(sa)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(sa),
									Magick::instance().commserv.FirstName());
	    free(sa);
	}
	break;
      default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2",( i, bolivia_OS_SA_DB));
    } /* switch (version) */
    bolivia_close_db(f, bolivia_OS_SA_DB);
}

void bolivia_load_sop()
{
    FILE *f;
    int i;

    if (!(f = bolivia_open_db("OperServ", bolivia_OS_SOP_DB, "r")))
        return;
    switch (i = bolivia_get_file_version(f, bolivia_OS_SOP_DB)) {
      case 5:
      case 4:
      case 3:
        i = fgetc(f)<<8 | fgetc(f);
        while (--i >= 0)
	{
	    char *s = bolivia_read_string(f, bolivia_OS_SOP_DB);
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)) &&
		!(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->find(s)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->insert(mstring(s),
									Magick::instance().commserv.FirstName());
	    free(s);
	}
        break;
      default:
        SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2",( i, bolivia_OS_SOP_DB));
    } /* switch (version) */
    bolivia_close_db(f, bolivia_OS_SOP_DB);
}

void bolivia_load_trigger()
{
    FILE *f;
    int i;
    bolivia_Trigger *trigger;
    int ntrigger, trigger_size;

    if (!(f = bolivia_open_db("TRIGGER", bolivia_TRIGGER_DB, "r")))
        return;

    switch (i = bolivia_get_file_version(f, bolivia_TRIGGER_DB)) {
      case 5:
        ntrigger = fgetc(f) * 256 + fgetc(f);
        if (ntrigger < 8)
            trigger_size = 16;
        else
            trigger_size = 2*ntrigger;
        trigger = (bolivia_Trigger *) malloc(sizeof(*trigger) * trigger_size);
        if (!ntrigger) {
            fclose(f);
            return;
        }
        if ((size_t) ntrigger != fread(trigger, sizeof(*trigger), ntrigger, f))
            SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_TRIGGER_DB));
        for (i = 0; i < ntrigger; i++) {
            trigger[i].mask = bolivia_read_string(f, bolivia_TRIGGER_DB);
	    Magick::instance().operserv.Clone_insert(mstring(trigger[i].mask).After("@"), trigger[i].tvalue,
			"Converted from Bolivia trigger.", mstring(trigger[i].who));
	    free(trigger[i].mask);
        }
	free(trigger);
        break;
      default:
        SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2",( i, bolivia_TRIGGER_DB));
    } /* switch (version) */

    bolivia_close_db(f, bolivia_TRIGGER_DB);
}

void bolivia_load_akill()
{
    FILE *f;
    int i;
    bolivia_Akill *akills;
    int akill_size, nakill;

    if (!(f = bolivia_open_db("AKILL", bolivia_AKILL_DB, "r")))
	return;

    switch (i = bolivia_get_file_version(f, bolivia_AKILL_DB)) {
      case 5:
      case 4:
      case 3:
	nakill = fgetc(f) * 256 + fgetc(f);
	if (nakill < 8)
	    akill_size = 16;
	else
	    akill_size = 2*nakill;
	akills = (bolivia_Akill *) malloc(sizeof(*akills) * akill_size);
	if (!nakill) {
	    fclose(f);
	    return;
	}
	if ((size_t) nakill != fread(akills, sizeof(*akills), nakill, f))
	    SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_AKILL_DB));
	for (i = 0; i < nakill; i++) {
	    akills[i].mask = bolivia_read_string(f, bolivia_AKILL_DB);
	    akills[i].reason = bolivia_read_string(f, bolivia_AKILL_DB);
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
	    free(akills[i].mask);
	    free(akills[i].reason);
	}
	free(akills);
	break;

      case 2: {
	struct {
	    char *mask;
	    char *reason;
	    char who[bolivia_NICKMAX];
	    time_t time;
	} old_akill;
	nakill = fgetc(f) * 256 + fgetc(f);
	if (nakill < 8)
	    akill_size = 16;
	else
	    akill_size = 2*nakill;
	akills = (bolivia_Akill *) malloc(sizeof(*akills) * akill_size);
	if (!nakill) {
	    fclose(f);
	    return;
	}
	for (i = 0; i < nakill; i++) {
	    if (1 != fread(&old_akill, sizeof(old_akill), 1, f))
		SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_AKILL_DB));
	    akills[i].time = old_akill.time;
	    strncpy(akills[i].who, old_akill.who, sizeof(akills[i].who));
	    akills[i].expires = 0;
	}
	for (i = 0; i < nakill; i++) {
	    akills[i].mask = bolivia_read_string(f, bolivia_AKILL_DB);
	    akills[i].reason = bolivia_read_string(f, bolivia_AKILL_DB);
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
	akills = (bolivia_Akill *) malloc(sizeof(*akills) * akill_size);
	if (!nakill) {
	    fclose(f);
	    return;
	}
	for (i = 0; i < nakill; i++) {
	    if (1 != fread(&old_akill, sizeof(old_akill), 1, f))
		SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_AKILL_DB));
	    akills[i].time = old_akill.time;
	    akills[i].who[0] = 0;
	    akills[i].expires = 0;
	}
	for (i = 0; i < nakill; i++) {
	    akills[i].mask = bolivia_read_string(f, bolivia_AKILL_DB);
	    akills[i].reason = bolivia_read_string(f, bolivia_AKILL_DB);
	    Magick::instance().operserv.Akill_insert(mstring(akills[i].mask), Magick::instance().operserv.Def_Expire(),
				mstring(akills[i].reason), Magick::instance().operserv.FirstName(), mDateTime(akills[i].time));

	    free(akills[i].reason);
	    free(akills[i].mask);
	}
	free(akills);
	break;
      } /* case 1 */

      default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2",( i, bolivia_AKILL_DB));
    } /* switch (version) */

    bolivia_close_db(f, bolivia_AKILL_DB);
}

void bolivia_load_ignore()
{
    FILE *f;
    int i;
    bolivia_Ignore *ignores;
    int nignore, ignore_size;

    if (!(f = bolivia_open_db("IGNORE", bolivia_IGNORE_DB, "r")))
	return;

    switch (i = bolivia_get_file_version(f, bolivia_IGNORE_DB)) {
      case 5:
      case 4:
      case 3:
        nignore = fgetc(f) * 256 + fgetc(f);
        if (nignore < 8)
            ignore_size = 16;
	else
            ignore_size = 2*nignore;
        ignores = (bolivia_Ignore *) malloc(sizeof(*ignores) * ignore_size);
        if (!nignore) {
	    fclose(f);
	    return;
	}
        if ((size_t) nignore != fread(ignores, sizeof(*ignores), nignore, f))
            SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_IGNORE_DB));
        for (i = 0; i < nignore; i++)
	{
            ignores[i].mask = bolivia_read_string(f, bolivia_IGNORE_DB);

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
	    char who[bolivia_NICKMAX];
	    time_t time;
        } old_ignore;
        nignore = fgetc(f) * 256 + fgetc(f);
        if (nignore < 8)
            ignore_size = 16;
	else
            ignore_size = 2*nignore;
        ignores = (bolivia_Ignore *) malloc(sizeof(*ignores) * ignore_size);
        if (!nignore) {
	    fclose(f);
	    return;
	}
        for (i = 0; i < nignore; i++) {
            if (1 != fread(&old_ignore, sizeof(old_ignore), 1, f))
                SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_IGNORE_DB));
            ignores[i].time = old_ignore.time;
            strncpy(ignores[i].who, old_ignore.who, sizeof(ignores[i].who));
            ignores[i].expires = 0;
	}
        for (i = 0; i < nignore; i++)
	{
            ignores[i].mask = bolivia_read_string(f, bolivia_IGNORE_DB);
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
        ignores = (bolivia_Ignore *) malloc(sizeof(*ignores) * ignore_size);
        if (!nignore) {
	    fclose(f);
	    return;
	}
        for (i = 0; i < nignore; i++) {
            if (1 != fread(&old_ignore, sizeof(old_ignore), 1, f))
                SLOG(LM_EMERGENCY, "Read error on $1", (bolivia_IGNORE_DB));
            ignores[i].time = old_ignore.time;
            ignores[i].who[0] = 0;
            ignores[i].expires = 0;
	}
        for (i = 0; i < nignore; i++)
	{
            ignores[i].mask = bolivia_read_string(f, bolivia_IGNORE_DB);
	    Magick::instance().operserv.Ignore_insert(mstring(ignores[i].mask), true,
			Magick::instance().operserv.FirstName(), mDateTime(ignores[i].time));

	    free(ignores[i].mask);
	}
	free(ignores);
	break;
      } /* case 1 */

      default:
        SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2",( i, bolivia_IGNORE_DB));
    } /* switch (version) */

    bolivia_close_db(f, bolivia_IGNORE_DB);
}

Nick_Stored_t *Convert::bolivia_CreateNickEntry(bolivia_NickInfo * ni)
{
    BTCB();
    int i;
    char **string;

    if (ni == NULL || ni->nick == NULL || !strlen(ni->nick))
	return NULL;

    if (ni->flags & bolivia_NI_VERBOTEN)
    {
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick);

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
	out->i_RegTime = mDateTime(ni->time_registered);
	out->i_LastSeenTime = mDateTime(ni->last_seen);

	for (i = 0, string = ni->access; i < ni->accesscount; ++i, ++string)
	{
	    out->i_access.insert(mstring(*string));
	}

	if (ni->flags & bolivia_NI_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (ni->flags & bolivia_NI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ni->flags & bolivia_NI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ni->flags & bolivia_NI_HOLD && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if (ni->flags & bolivia_NI_NOMEMO && !out->L_NoMemo())
	    out->setting.NoMemo = true;

	if (ni->flags & bolivia_NI_FREEZE)
	{
	    out->i_Suspend_By = Magick::instance().nickserv.FirstName();
	    out->i_Suspend_Time = mDateTime::CurrentDateTime();
	}

	return out;
    }
    ETCB();
}

Chan_Stored_t *Convert::Convert::bolivia_CreateChanEntry(bolivia_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name == NULL || !strlen(ci->name))
	return NULL;

    if (ci->flags & bolivia_CI_VERBOTEN)
    {
	Chan_Stored_t *out = new Chan_Stored_t(mstring(ci->name));

	return out;
    }
    else
    {
	bolivia_ChanAccess *i_access;
	bolivia_AutoKick *akick;
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
	    mstring modes = bolivia_getmodes(ci->mlock_on);

	    modes.Remove("k");
	    modes.Remove("l");
	    modelock << "+" << modes << (ci->mlock_key != NULL ? "k" : "") << (ci->mlock_limit ? "l" : "");
	}
	if (ci->mlock_off)
	{
	    modelock << "-" << bolivia_getmodes(ci->mlock_off);
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
	float mod = (float) Magick::instance().chanserv.Level_Max() / (float) bolivia_ACCESS_FOUNDER;

	for (i = 0, i_access = ci->access; i < ci->accesscount; ++i, ++i_access)
	{
	    if (i_access->name == NULL)
		continue;

	    if (i_access->level < 0)
		newlevel = -1;
	    else
		newlevel = (long) ((float) i_access->level * mod);
	    if (newlevel == 0)
		newlevel = 1;

	    out->Access_insert(i_access->name, newlevel,
				Magick::instance().chanserv.FirstName());
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

	if (ci->flags & bolivia_CI_KEEPTOPIC && !out->L_Keeptopic())
	    out->setting.Keeptopic = true;
	if (ci->flags & bolivia_CI_OPGUARD && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & bolivia_CI_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & bolivia_CI_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & bolivia_CI_LEAVEOPS && !out->L_Anarchy())
	    out->setting.Anarchy = true;
	if (ci->flags & bolivia_CI_IDENT && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & bolivia_CI_HELDCHAN && !Magick::instance().chanserv.LCK_NoExpire())
	    out->setting.NoExpire = true;

	if (ci->last_topic != NULL && strlen(ci->last_topic))
	    out->i_Topic = mstring(ci->last_topic);
	if (ci->last_topic_setter != NULL && strlen(ci->last_topic_setter))
	    out->i_Topic_Setter = mstring(ci->last_topic_setter);
	out->i_Topic_Set_Time = mDateTime(ci->last_topic_time);

	if (ci->flags & bolivia_CI_FREEZECHAN)
	{
	    if (ci->freeze != NULL && strlen(ci->freeze))
		out->i_Suspend_By = mstring(ci->freeze);
	    out->i_Suspend_Time = mDateTime::CurrentDateTime();
	}

	if (ci->levels != NULL)
	{
	    for (i = 0; i < bolivia_CA_SIZE; ++i)
	    {
		if (ci->levels[i] == bolivia_ACCESS_INVALID)
		    newlevel = Magick::instance().chanserv.Level_Max() + 2;
		else if (ci->levels[i] == bolivia_ACCESS_FOUNDER)
		    newlevel = Magick::instance().chanserv.Level_Max() + 1;
		else if (ci->levels[i] < 0)
		    newlevel = -1;
		else
		    newlevel = (long) ((float) ci->levels[i] * mod);

		switch (i)
		{
		case bolivia_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case bolivia_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case bolivia_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case bolivia_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case bolivia_CA_UNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case bolivia_CA_ACCESS_CHANGE:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case bolivia_CA_ACCESS_LIST:
		    out->Level_change("VIEW", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case bolivia_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case bolivia_CA_INVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case bolivia_CA_OPDEOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case bolivia_CA_CLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}

	if (ci->flags & bolivia_CI_MEMO_AV)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("AUTOVOICE"),
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & bolivia_CI_MEMO_AOP)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("AUTOOP"),
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & bolivia_CI_MEMO_SOP)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("SUPER"),
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & bolivia_CI_MEMO_CF)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.Level_Max(),
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & bolivia_CI_MEMO_FR)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.Level_Max() + 1,
		Magick::instance().chanserv.FirstName());
	else if (ci->flags & bolivia_CI_MEMO_NONE)
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.Level_Max() + 2,
		Magick::instance().chanserv.FirstName());

	return out;
    }
    ETCB();
}

MemoServ::nick_memo_t Convert::bolivia_CreateMemoEntry(bolivia_MemoList * ml)
{
    BTCB();
    int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;
    bolivia_Memo *memos;

    memos = ml->memos;
    for (i = 0; i < ml->n_memos; ++i, ++memos)
    {
	if (memos->text == NULL)
	    continue;

	tmp = new Memo_t(mstring(ml->nick), mstring(memos->sender), mstring(memos->text));
	tmp->i_Time = mDateTime(memos->time);
	if (!(memos->flags & bolivia_MF_UNREAD))
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}

#endif /* CONVERT */
