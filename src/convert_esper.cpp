#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
#define RCSID(x,y) const char *rcsid_convert_esper_cpp_ ## x () { return y; }
RCSID(convert_esper_cpp, "@(#)$Id$");

/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.14  2002/01/13 05:18:41  prez
** More formatting, changed style slightly
**
** Revision 1.13  2002/01/12 14:42:09  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.12  2002/01/10 19:30:38  prez
** FINALLY finished a MAJOR overhaul ... now have a 'safe pointer', that
** ensures that data being used cannot be deleted while still being used.
**
** Revision 1.11  2001/12/20 08:02:32  prez
** Massive change -- 'Parent' has been changed to Magick::instance(), will
** soon also move the ACE_Reactor over, and will be able to have multipal
** instances of Magick in the same process if necessary.
**
** Revision 1.10  2001/11/17 07:18:12  prez
** Fixed up unbanning, so it gets ALL bans ...
**
** Revision 1.9  2001/11/12 01:05:02  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.8  2001/11/04 23:43:14  prez
** Updates for MS Visual C++ compilation (it works now!).
**
** Revision 1.7  2001/11/03 21:02:53  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.6  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.5  2001/05/28 11:17:34  prez
** Added some more anti-deadlock stuff, and fixed nick ident warnings
**
** Revision 1.4  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.3  2001/03/27 07:04:31  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.2  2001/03/04 02:04:14  prez
** Made mstring a little more succinct ... and added vector/list operations
**
** Revision 1.1  2001/02/03 02:22:34  prez
** added conversion for ESPERNET
**
**
** ========================================================== */

#ifdef CONVERT
#include "convert_esper.h"
#include "magick.h"

/* Database file handling routines.
 *
 * Services is copyright (c) 1996-1999 Andrew Church.
 *     E-mail: <achurch@dragonfire.net>
 * Services is copyright (c) 1999-2000 Andrew Kempe.
 *     E-mail: <theshadow@shadowfire.org>
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

const char *ESP_NickDBName = "nick.db";
const char *ESP_ChanDBName = "chan.db";
const char *ESP_NewsDBName = "news.db";
const char *ESP_OperDBName = "oper.db";
const char *ESP_AutokillDBName = "akill.db";
const char *ESP_ExceptionDBName = "exception.db";

const char *ESP_s_NickServ = "NickServ";
const char *ESP_s_ChanServ = "ChanServ";
const char *ESP_s_MemoServ = "MemoServ";
const char *ESP_s_OperServ = "OperServ";

const int ESP_debug = 0;
const int ESP_MSMaxMemos = 20;
const int ESP_CSMaxReg = 20;
const int ESP_NSAccessMax = 32;
const int ESP_NSAllowKillImmed = 0;

int32 ESP_nnews;
int32 ESP_news_size;
int32 ESP_nakill;
int32 ESP_akill_size;
int32 ESP_nexceptions;

/*************************************************************************/

/*************************************************************************/

/* Return the version number on the file.  Return 0 if there is no version
 * number or the number doesn't make sense (i.e. less than 1 or greater
 * than ESP_FILE_VERSION).
 */

int ESP_get_file_version(ESP_dbFILE * f)
{
    FILE *fp = f->fp;
    int version = fgetc(fp) << 24 | fgetc(fp) << 16 | fgetc(fp) << 8 | fgetc(fp);

    if (ferror(fp))
    {
	return 0;
    }
    else if (feof(fp))
    {
	return 0;
    }
    else if (version > ESP_FILE_VERSION || version < 1)
    {
	return 0;
    }
    return version;
}

/*************************************************************************/

ESP_dbFILE *ESP_open_db_read(const char *service, const char *filename)
{
    ESP_dbFILE *f;
    FILE *fp;

    static_cast < void >(service);

    f = (ESP_dbFILE *) malloc(sizeof(*f));
    if (!f)
    {
	return NULL;
    }
    strncpy(f->filename, filename, sizeof(f->filename));
    f->mode = 'r';
    fp = fopen(f->filename, "rb");
    if (!fp)
    {
	int errno_save = errno;

	free(f);
	errno = errno_save;
	return NULL;
    }
    f->fp = fp;
    f->backupfp = NULL;
    return f;
}

/*************************************************************************/

/* Open a database file for reading (*mode == 'r') or writing (*mode == 'w').
 * Return the stream pointer, or NULL on error.  When opening for write, it
 * is an error for rename() to return an error (when backing up the original
 * file) other than ENOENT, if NO_BACKUP_OKAY is not defined; it is an error
 * if the version number cannot be written to the file; and it is a fatal
 * error if opening the file for write fails and the backup was successfully
 * made but cannot be restored.
 */

ESP_dbFILE *ESP_open_db(const char *service, const char *filename, const char *mode)
{
    if (*mode == 'r')
    {
	return ESP_open_db_read(service, filename);
    }
    else
    {
	errno = EINVAL;
	return NULL;
    }
}

/*************************************************************************/

/* Close a database file.  If the file was opened for write, remove the
 * backup we (may have) created earlier.
 */

void ESP_close_db(ESP_dbFILE * f)
{
    fclose(f->fp);
    free(f);
}

/*************************************************************************/

/*************************************************************************/

/* Read and write 2- and 4-byte quantities, pointers, and strings.  All
 * multibyte values are stored in big-endian order (most significant byte
 * first).  A pointer is stored as a byte, either 0 if NULL or 1 if not,
 * and read pointers are returned as either (void *)0 or (void *)1.  A
 * string is stored with a 2-byte unsigned length (including the trailing
 * \0) first; a length of 0 indicates that the string pointer is NULL.
 * Written strings are truncated silently at 65534 bytes, and are always
 * null-terminated.
 *
 * All routines return -1 on error, 0 otherwise.
 */

int ESP_read_int16(int16 * ret, ESP_dbFILE * f)
{
    int c1, c2;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    if (c1 == EOF || c2 == EOF)
	return -1;
    *ret = c1 << 8 | c2;
    return 0;
}

int ESP_read_int32(int32 * ret, ESP_dbFILE * f)
{
    int c1, c2, c3, c4;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    c3 = fgetc(f->fp);
    c4 = fgetc(f->fp);
    if (c1 == EOF || c2 == EOF || c3 == EOF || c4 == EOF)
	return -1;
    *ret = c1 << 24 | c2 << 16 | c3 << 8 | c4;
    return 0;
}

int ESP_read_string(char **ret, ESP_dbFILE * f)
{
    char *s;
    int16 len;

    if (ESP_read_int16(&len, f) < 0)
	return -1;
    if (len == 0)
    {
	*ret = NULL;
	return 0;
    }
    s = (char *) malloc(len);
    if (len != (int16) fread(s, 1, len, f->fp))
    {
	free(s);
	return -1;
    }
    *ret = s;
    return 0;
}

/*************************************************************************/

/* Load/save data files. */

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	SLOG(LM_EMERGENCY, "Read error on $1", ( ESP_NickDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void ESP_load_old_ns_dbase(ESP_dbFILE * f, int ver)
{
    Nick_Stored_t *nick;
    struct nickinfo_
    {
	char nick[ESP_NICKMAX];
	char pass[ESP_PASSMAX];
	char *last_usermask;
	char *last_realname;
	time_t time_registered;
	time_t last_seen;
	long accesscount;
	char **access;
	long flags;
	time_t id_timestamp;
	unsigned short memomax;
	unsigned short channelcount;
	char *url;
	char *email;
    }
    old_nickinfo;

    int i, j, c;
    ESP_NickInfo *ni;
    int failed = 0;

    for (i = 33; i < 256 && !failed; i++)
    {
	while ((c = ESP_getc_db(f)) != 0)
	{
	    if (c != 1)
		SLOG(LM_EMERGENCY, "Invalid format in $1", (ESP_NickDBName));
	    SAFE(ESP_read_variable(old_nickinfo, f));
	    if (ESP_debug >= 3)
		SLOG(LM_DEBUG, "ESP_load_old_ns_dbase read nick $1", (old_nickinfo.nick));
	    ni = (ESP_NickInfo *) calloc(1, sizeof(ESP_NickInfo));
	    strncpy(ni->nick, old_nickinfo.nick, ESP_NICKMAX);
	    strncpy(ni->pass, old_nickinfo.pass, ESP_PASSMAX);
	    ni->time_registered = old_nickinfo.time_registered;
	    ni->last_seen = old_nickinfo.last_seen;
	    ni->accesscount = old_nickinfo.accesscount;
	    ni->flags = old_nickinfo.flags;
	    if (ver < 3)	/* Memo max field created in ver 3 */
		ni->memos.memomax = ESP_MSMaxMemos;
	    else if (old_nickinfo.memomax)
		ni->memos.memomax = old_nickinfo.memomax;
	    else
		ni->memos.memomax = -1;	/* Unlimited is now -1 */
	    /* Reset channel count because counting was broken in old
	     * versions; ESP_load_old_cs_dbase() will calculate the count */
	    ni->channelcount = 0;
	    ni->channelmax = ESP_CSMaxReg;
	    ni->language = ESP_LANG_EN_US;
	    /* ENCRYPTEDPW and VERBOTEN moved from ni->flags to ni->status */
	    if (ni->flags & 4)
		ni->status |= ESP_NS_VERBOTEN;
	    if (ni->flags & 8)
		ni->status |= ESP_NS_ENCRYPTEDPW;
	    ni->flags &= ~0xE000000C;
	    if (ni->status & ESP_NS_ENCRYPTEDPW)
	    {
		/* Bail: it makes no sense to continue with encrypted
		 * passwords, since we won't be able to verify them */
		SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted " "but encryption disabled, aborting",
		     (ESP_s_NickServ, ni->nick));
	    }
	    if (old_nickinfo.url)
		SAFE(ESP_read_string(&ni->url, f));
	    if (old_nickinfo.email)
		SAFE(ESP_read_string(&ni->email, f));
	    SAFE(ESP_read_string(&ni->last_usermask, f));
	    if (!ni->last_usermask)
		ni->last_usermask = strdup("@");
	    SAFE(ESP_read_string(&ni->last_realname, f));
	    if (!ni->last_realname)
		ni->last_realname = strdup("");
	    if (ni->accesscount)
	    {
		char **i_access, *s;

		if (ni->accesscount > ESP_NSAccessMax)
		    ni->accesscount = ESP_NSAccessMax;
		i_access = (char **) malloc(sizeof(char *) * ni->accesscount);
		ni->access = i_access;
		for (j = 0; j < ni->accesscount; j++, i_access++)
		    SAFE(ESP_read_string(i_access, f));
		while (j < old_nickinfo.accesscount)
		{
		    SAFE(ESP_read_string(&s, f));
		    if (s)
			free(s);
		    j++;
		}
	    }
	    ni->id_timestamp = 0;
	    if (ver < 3)
	    {
		ni->flags |= ESP_NI_MEMO_SIGNON | ESP_NI_MEMO_RECEIVE;
	    }
	    else if (ver == 3)
	    {
		if (!(ni->flags & (ESP_NI_MEMO_SIGNON | ESP_NI_MEMO_RECEIVE)))
		    ni->flags |= ESP_NI_MEMO_SIGNON | ESP_NI_MEMO_RECEIVE;
	    }

	    nick = ESP_CreateNickEntry(ni);
	    if (nick != NULL)
		Magick::instance().nickserv.AddStored(nick);
	    ESP_delnick(ni);
	}			/* while (ESP_getc_db(f) != 0) */
    }				/* for (i) */
    if (ESP_debug >= 2)
	NSLOG(LM_DEBUG, "ESP_load_old_ns_dbase(): loading memos");
    ESP_load_old_ms_dbase();
}

void ESP_load_ns_dbase(void)
{
    Nick_Stored_t *nick;

    MemoServ::nick_memo_t memo;

    ESP_dbFILE *f;
    int ver, i, j, c;
    ESP_NickInfo *ni;
    int failed = 0;

    if (!(f = ESP_open_db(ESP_s_NickServ, ESP_NickDBName, "r")))
	return;

    switch (ver = ESP_get_file_version(f))
    {

    case 8:
    case 7:
    case 6:
    case 5:
	for (i = 0; i < 256 && !failed; i++)
	{
	    int32 tmp32;

	    while ((c = ESP_getc_db(f)) == 1)
	    {
		if (c != 1)
		    SLOG(LM_EMERGENCY, "Invalid format in $1", (ESP_NickDBName));
		ni = (ESP_NickInfo *) calloc(sizeof(ESP_NickInfo), 1);
		SAFE(ESP_read_buffer(ni->nick, f));
		SAFE(ESP_read_buffer(ni->pass, f));
		SAFE(ESP_read_string(&ni->url, f));
		SAFE(ESP_read_string(&ni->email, f));
		SAFE(ESP_read_string(&ni->last_usermask, f));
		if (!ni->last_usermask)
		    ni->last_usermask = strdup("@");
		SAFE(ESP_read_string(&ni->last_realname, f));
		if (!ni->last_realname)
		    ni->last_realname = strdup("");
		SAFE(ESP_read_string(&ni->last_quit, f));
		SAFE(ESP_read_int32(&tmp32, f));
		ni->time_registered = tmp32;
		SAFE(ESP_read_int32(&tmp32, f));
		ni->last_seen = tmp32;
		SAFE(ESP_read_int16(&ni->status, f));
		ni->status &= ~ESP_NS_TEMPORARY;
		if (ni->status & ESP_NS_ENCRYPTEDPW)
		{
		    /* Bail: it makes no sense to continue with encrypted
		     * passwords, since we won't be able to verify them */
		    SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted " "but encryption disabled, aborting",
			 (ESP_s_NickServ, ni->nick));
		}
		/* Store the _name_ of the link target in ni->link for now;
		 * we'll resolve it after we've loaded all the nicks */
		SAFE(ESP_read_string(&ni->link, f));
		SAFE(ESP_read_int16(&ni->linkcount, f));
		if (ni->link)
		{
		    SAFE(ESP_read_int16(&ni->channelcount, f));
		    /* No other information saved for linked nicks, since
		     * they get it all from their link target */
		    ni->flags = 0;
		    ni->accesscount = 0;
		    ni->access = NULL;
		    ni->memos.memocount = 0;
		    ni->memos.memomax = ESP_MSMaxMemos;
		    ni->memos.memos = NULL;
		    ni->channelmax = ESP_CSMaxReg;
		    ni->language = ESP_LANG_EN_US;
		}
		else
		{
		    ni->link = strdup("");
		    SAFE(ESP_read_int32(&ni->flags, f));
		    if (!ESP_NSAllowKillImmed)
			ni->flags &= ~ESP_NI_KILL_IMMED;
		    if (ni->flags & ESP_NI_SUSPENDED)
		    {
			SAFE(ESP_read_buffer(ni->susp.who, f));
			SAFE(ESP_read_string(&ni->susp.reason, f));
			SAFE(ESP_read_int32(&tmp32, f));
			ni->susp.suspended = tmp32;
			SAFE(ESP_read_int32(&tmp32, f));
			ni->susp.expires = tmp32;
		    }
		    SAFE(ESP_read_int16(&ni->accesscount, f));
		    if (ni->accesscount)
		    {
			char **i_access;
			i_access = (char **) malloc(sizeof(char *) * ni->accesscount);
			ni->access = i_access;
			for (j = 0; j < ni->accesscount; j++, i_access++)
			    SAFE(ESP_read_string(i_access, f));
		    }
		    SAFE(ESP_read_int16(&ni->memos.memocount, f));
		    SAFE(ESP_read_int16(&ni->memos.memomax, f));
		    if (ni->memos.memocount)
		    {
			ESP_Memo *memos;

			memos = (ESP_Memo *) malloc(sizeof(ESP_Memo) * ni->memos.memocount);
			ni->memos.memos = memos;
			for (j = 0; j < ni->memos.memocount; j++, memos++)
			{
			    SAFE(ESP_read_int32(&memos->number, f));
			    SAFE(ESP_read_int16(&memos->flags, f));
			    SAFE(ESP_read_int32(&tmp32, f));
			    memos->time = tmp32;
			    SAFE(ESP_read_buffer(memos->sender, f));
			    SAFE(ESP_read_string(&memos->text, f));
			}
		    }
		    else
		    {
			ni->memos.memos = NULL;
		    }
		    SAFE(ESP_read_int16(&ni->channelcount, f));
		    SAFE(ESP_read_int16(&ni->channelmax, f));
		    if (ver == 5)
		    {
			/* Fields not initialized properly for new nicks */
			/* These will be updated by ESP_load_cs_dbase() */
			ni->channelcount = 0;
			ni->channelmax = ESP_CSMaxReg;
		    }
		    SAFE(ESP_read_int16(&ni->language, f));
		}
		ni->id_timestamp = 0;

		nick = ESP_CreateNickEntry(ni);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);
		memo = ESP_CreateMemoEntry(&ni->memos, ni->nick);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);
		ESP_delnick(ni);
	    }			/* while (ESP_getc_db(f) != 0) */
	}			/* for (i) */
	break;

    case 4:
    case 3:
    case 2:
    case 1:
	ESP_load_old_ns_dbase(f, ver);
	break;

    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", (ver, ESP_NickDBName));

    }				/* switch (version) */

    ESP_close_db(f);
}

int ESP_delnick(ESP_NickInfo * ni)
{
    int i;

    /* cs_remove_nick(ni);
     * os_remove_nick(ni); */
    if (ni->link)
	free(ni->link);
    if (ni->last_usermask)
	free(ni->last_usermask);
    if (ni->last_realname)
	free(ni->last_realname);
    if (ni->susp.reason)
	free(ni->susp.reason);
    if (ni->access)
    {
	for (i = 0; i < ni->accesscount; i++)
	{
	    if (ni->access[i])
		free(ni->access[i]);
	}
	free(ni->access);
    }
    if (ni->memos.memos)
    {
	for (i = 0; i < ni->memos.memocount; i++)
	{
	    if (ni->memos.memos[i].text)
		free(ni->memos.memos[i].text);
	}
	free(ni->memos.memos);
    }
    free(ni);
    return 1;
}

#undef SAFE

/*************************************************************************/

/* Load/save data files. */

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	SLOG(LM_EMERGENCY, "Read error on $1", ( ESP_ChanDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

static int def_levels[][2] = {
    {ESP_CA_AUTOOP, 5},
    {ESP_CA_AUTOVOICE, 3},
    {ESP_CA_AUTODEOP, -1},
    {ESP_CA_NOJOIN, -2},
    {ESP_CA_INVITE, 5},
    {ESP_CA_AKICK, 10},
    {ESP_CA_SET, ESP_ACCESS_INVALID},
    {ESP_CA_CLEAR, ESP_ACCESS_INVALID},
    {ESP_CA_UNBAN, 5},
    {ESP_CA_OPDEOP, 5},
    {ESP_CA_ACCESS_LIST, 0},
    {ESP_CA_ACCESS_CHANGE, 10},
    {ESP_CA_MEMO, 10},
    {-1}
};

void ESP_reset_levels(ESP_ChannelInfo * ci)
{
    int i;

    if (ci->levels)
	free(ci->levels);
    ci->levels = (int16 *) malloc(ESP_CA_SIZE * sizeof(int16 *));
    for (i = 0; def_levels[i][0] >= 0; i++)
	ci->levels[def_levels[i][0]] = def_levels[i][1];
}

/* Load v1-v4 files. */
void ESP_load_old_cs_dbase(ESP_dbFILE * f, int ver)
{
    Chan_Stored_t *chan;
    int i, j, c;
    ESP_ChannelInfo *ci;
    int failed = 0;

    static_cast < void >(ver);

    struct
    {
	short level;
#ifdef COMPATIBILITY_V2
	short is_nick;
#else
	short in_use;
#endif
	char *name;
    }
    old_chanaccess;

    struct
    {
	short is_nick;
	short pad;
	char *name;
	char *reason;
    }
    old_autokick;

    struct
    {
	char name[ESP_CHANMAX];
	char founder[ESP_NICKMAX];
	char founderpass[ESP_PASSMAX];
	char *desc;
	time_t time_registered;
	time_t last_used;
	long accesscount;
	ESP_ChanAccess *access;
	long akickcount;
	ESP_AutoKick *akick;
	short mlock_on, mlock_off;
	long mlock_limit;
	char *mlock_key;
	char *last_topic;
	char last_topic_setter[ESP_NICKMAX];
	time_t last_topic_time;
	long flags;
	short *levels;
	char *url;
	char *email;
	struct channel_ *c;
    }
    old_channelinfo;

    for (i = 33; i < 256 && !failed; i++)
    {
	while ((c = ESP_getc_db(f)) != 0)
	{
	    if (c != 1)
		SLOG(LM_EMERGENCY, "Invalid format in $1", (ESP_ChanDBName));
	    SAFE(ESP_read_variable(old_channelinfo, f));
	    if (ESP_debug >= 3)
		SLOG(LM_DEBUG, "ESP_load_old_cs_dbase: read channel $1", (old_channelinfo.name));
	    ci = (ESP_ChannelInfo *) calloc(1, sizeof(ESP_ChannelInfo));
	    strncpy(ci->name, old_channelinfo.name, ESP_CHANMAX);
	    /* ci->founder = findnick(old_channelinfo.founder); */
	    strncpy(ci->founder, old_channelinfo.founder, ESP_NICKMAX);
	    strncpy(ci->founderpass, old_channelinfo.founderpass, ESP_PASSMAX);
	    ci->time_registered = old_channelinfo.time_registered;
	    ci->last_used = old_channelinfo.last_used;
	    ci->accesscount = old_channelinfo.accesscount;
	    ci->akickcount = old_channelinfo.akickcount;
	    ci->mlock_on = old_channelinfo.mlock_on;
	    ci->mlock_off = old_channelinfo.mlock_off;
	    ci->mlock_limit = old_channelinfo.mlock_limit;
	    strncpy(ci->last_topic_setter, old_channelinfo.last_topic_setter, ESP_NICKMAX);
	    ci->last_topic_time = old_channelinfo.last_topic_time;
	    ci->flags = old_channelinfo.flags;
	    if (ci->flags & ESP_CI_ENCRYPTEDPW)
	    {
		/* Bail: it makes no sense to continue with encrypted
		 * passwords, since we won't be able to verify them */
		SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted " "but encryption disabled, aborting",
		     (ESP_s_ChanServ, ci->name));
	    }
	    SAFE(ESP_read_string(&ci->desc, f));
	    if (!ci->desc)
		ci->desc = strdup("");
	    if (old_channelinfo.url)
		SAFE(ESP_read_string(&ci->url, f));
	    if (old_channelinfo.email)
		SAFE(ESP_read_string(&ci->email, f));
	    if (old_channelinfo.mlock_key)
		SAFE(ESP_read_string(&ci->mlock_key, f));
	    if (old_channelinfo.last_topic)
		SAFE(ESP_read_string(&ci->last_topic, f));

	    if (ci->accesscount)
	    {
		ESP_ChanAccess *i_access;

		i_access = (ESP_ChanAccess *) malloc(sizeof(ESP_ChanAccess) * ci->accesscount);
		ci->access = i_access;
		for (j = 0; j < ci->accesscount; j++, i_access++)
		{
		    SAFE(ESP_read_variable(old_chanaccess, f));
#ifdef COMPATIBILITY_V2
		    if (old_chanaccess.is_nick < 0)
			i_access->in_use = 0;
		    else
			i_access->in_use = old_chanaccess.is_nick;
#else
		    i_access->in_use = old_chanaccess.in_use;
#endif
		    i_access->level = old_chanaccess.level;
		}
		i_access = ci->access;
		for (j = 0; j < ci->accesscount; j++, i_access++)
		{
		    SAFE(ESP_read_string(&i_access->nick, f));
		    if (i_access->nick == NULL)
			i_access->in_use = 0;
		}
	    }
	    else
	    {
		ci->access = NULL;
	    }			/* if (ci->accesscount) */

	    if (ci->akickcount)
	    {
		ESP_AutoKick *akick;
		char *s;

		akick = (ESP_AutoKick *) malloc(sizeof(ESP_AutoKick) * ci->akickcount);
		ci->akick = akick;
		for (j = 0; j < ci->akickcount; j++, akick++)
		{
		    SAFE(ESP_read_variable(old_autokick, f));
		    if (old_autokick.is_nick < 0)
		    {
			akick->in_use = 0;
			akick->is_nick = 0;
		    }
		    else
		    {
			akick->in_use = 1;
			akick->is_nick = old_autokick.is_nick;
		    }
		    akick->reason = old_autokick.reason;
		}
		akick = ci->akick;
		for (j = 0; j < ci->akickcount; j++, akick++)
		{
		    SAFE(ESP_read_string(&s, f));
		    if (akick->is_nick)
		    {
			if (!(akick->u.nick = s))
			    akick->in_use = akick->is_nick = 0;
		    }
		    else
		    {
			if (!(akick->u.mask = s))
			    akick->in_use = 0;
		    }
		    if (akick->reason)
			SAFE(ESP_read_string(&akick->reason, f));
		    if (!akick->in_use)
		    {
			if (akick->is_nick)
			{
			    akick->u.nick = NULL;
			}
			else
			{
			    free(akick->u.mask);
			    akick->u.mask = NULL;
			}
			if (akick->reason)
			{
			    free(akick->reason);
			    akick->reason = NULL;
			}
		    }
		}
	    }
	    else
	    {
		ci->akick = NULL;
	    }			/* if (ci->akickcount) */

	    if (old_channelinfo.levels)
	    {
		int16 n_entries;

		ci->levels = NULL;
		ESP_reset_levels(ci);
		SAFE(ESP_read_int16(&n_entries, f));
#ifdef COMPATIBILITY_V2
		/* Ignore earlier, incompatible levels list */
		if (n_entries == 6)
		{
		    fseek(f, sizeof(short) * n_entries, SEEK_CUR);
		}
		else
#endif
		    for (j = 0; j < n_entries; j++)
		    {
			short lev;

			SAFE(ESP_read_variable(lev, f));
			if (j < ESP_CA_SIZE)
			    ci->levels[j] = lev;
		    }
	    }
	    else
	    {
		ESP_reset_levels(ci);
	    }

	    ci->memos.memomax = ESP_MSMaxMemos;

	    chan = ESP_CreateChanEntry(ci);
	    if (chan != NULL)
		Magick::instance().chanserv.AddStored(chan);
	    ESP_delchan(ci);
	}			/* while (ESP_getc_db(f) != 0) */
    }				/* for (i) */
}

void ESP_load_cs_dbase(void)
{
    Chan_Stored_t *chan;

    MemoServ::channel_news_t news;

    ESP_dbFILE *f;
    int ver, i, j, c;
    ESP_ChannelInfo *ci;
    int failed = 0;

    if (!(f = ESP_open_db(ESP_s_ChanServ, ESP_ChanDBName, "r")))
	return;

    switch (ver = ESP_get_file_version(f))
    {

    case 8:
    case 7:
    case 6:
    case 5:

	for (i = 0; i < 256 && !failed; i++)
	{
	    int16 tmp16;
	    int32 tmp32;
	    int n_levels;
	    char *s;

	    while ((c = ESP_getc_db(f)) != 0)
	    {
		if (c != 1)
		    SLOG(LM_EMERGENCY, "Invalid format in $1", (ESP_ChanDBName));
		ci = (ESP_ChannelInfo *) malloc(sizeof(ESP_ChannelInfo));
		SAFE(ESP_read_buffer(ci->name, f));
		SAFE(ESP_read_string(&ci->founder, f));
		if (ver >= 7)
		{
		    SAFE(ESP_read_string(&ci->successor, f));
		    /* Founder could be successor, which is bad, in vers <8 */
		}
		else
		{
		    ci->successor = NULL;
		}
		SAFE(ESP_read_buffer(ci->founderpass, f));
		SAFE(ESP_read_string(&ci->desc, f));
		if (!ci->desc)
		    ci->desc = strdup("");
		SAFE(ESP_read_string(&ci->url, f));
		SAFE(ESP_read_string(&ci->email, f));
		SAFE(ESP_read_int32(&tmp32, f));
		ci->time_registered = tmp32;
		SAFE(ESP_read_int32(&tmp32, f));
		ci->last_used = tmp32;
		SAFE(ESP_read_string(&ci->last_topic, f));
		SAFE(ESP_read_buffer(ci->last_topic_setter, f));
		SAFE(ESP_read_int32(&tmp32, f));
		ci->last_topic_time = tmp32;
		SAFE(ESP_read_int32(&ci->flags, f));
		if (ci->flags & ESP_CI_ENCRYPTEDPW)
		{
		    /* Bail: it makes no sense to continue with encrypted
		     * passwords, since we won't be able to verify them */
		    SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted " "but encryption disabled, aborting",
			 (ESP_s_ChanServ, ci->name));
		}
		SAFE(ESP_read_int16(&tmp16, f));
		n_levels = tmp16;
		ci->levels = (int16 *) malloc(2 * ESP_CA_SIZE);
		ESP_reset_levels(ci);
		for (j = 0; j < n_levels; j++)
		{
		    if (j < ESP_CA_SIZE)
			SAFE(ESP_read_int16(&ci->levels[j], f));
		    else
			SAFE(ESP_read_int16(&tmp16, f));
		}

		SAFE(ESP_read_int16(&ci->accesscount, f));
		if (ci->accesscount)
		{
		    ci->access = (ESP_ChanAccess *) calloc(ci->accesscount, sizeof(ESP_ChanAccess));
		    for (j = 0; j < ci->accesscount; j++)
		    {
			SAFE(ESP_read_int16(&ci->access[j].in_use, f));
			if (ci->access[j].in_use)
			{
			    SAFE(ESP_read_int16(&ci->access[j].level, f));
			    SAFE(ESP_read_string(&ci->access[j].nick, f));
			    if (ci->access[j].nick == NULL)
				ci->access[j].in_use = 0;
			}
		    }
		}
		else
		{
		    ci->access = NULL;
		}

		SAFE(ESP_read_int16(&ci->akickcount, f));
		if (ci->akickcount)
		{
		    ci->akick = (ESP_AutoKick *) calloc(ci->akickcount, sizeof(ESP_AutoKick));
		    for (j = 0; j < ci->akickcount; j++)
		    {
			SAFE(ESP_read_int16(&ci->akick[j].in_use, f));
			if (ci->akick[j].in_use)
			{
			    SAFE(ESP_read_int16(&ci->akick[j].is_nick, f));
			    SAFE(ESP_read_string(&s, f));
			    if (ci->akick[j].is_nick)
			    {
				ci->akick[j].u.nick = s;
				if (!ci->akick[j].u.nick)
				    ci->akick[j].in_use = 0;
			    }
			    else
			    {
				ci->akick[j].u.mask = s;
			    }
			    SAFE(ESP_read_string(&s, f));
			    if (ci->akick[j].in_use)
				ci->akick[j].reason = s;
			    else if (s)
				free(s);
			    if (ver >= 8)
				SAFE(ESP_read_buffer(ci->akick[j].who, f));
			    else
				ci->akick[j].who[0] = '\0';
			}
		    }
		}
		else
		{
		    ci->akick = NULL;
		}

		SAFE(ESP_read_int16(&ci->mlock_on, f));
		SAFE(ESP_read_int16(&ci->mlock_off, f));
		SAFE(ESP_read_int32(&ci->mlock_limit, f));
		SAFE(ESP_read_string(&ci->mlock_key, f));

		SAFE(ESP_read_int16(&ci->memos.memocount, f));
		SAFE(ESP_read_int16(&ci->memos.memomax, f));
		if (ci->memos.memocount)
		{
		    ESP_Memo *memos;

		    memos = (ESP_Memo *) malloc(sizeof(ESP_Memo) * ci->memos.memocount);
		    ci->memos.memos = memos;
		    for (j = 0; j < ci->memos.memocount; j++, memos++)
		    {
			SAFE(ESP_read_int32(&memos->number, f));
			SAFE(ESP_read_int16(&memos->flags, f));
			SAFE(ESP_read_int32(&tmp32, f));
			memos->time = tmp32;
			SAFE(ESP_read_buffer(memos->sender, f));
			SAFE(ESP_read_string(&memos->text, f));
		    }
		}
		else
		{
		    ci->memos.memos = NULL;
		}

		SAFE(ESP_read_string(&ci->entry_message, f));

		chan = ESP_CreateChanEntry(ci);
		if (chan != NULL)
		    Magick::instance().chanserv.AddStored(chan);
		news = ESP_CreateNewsEntry(&ci->memos, ci->name);
		if (news.size())
		    Magick::instance().memoserv.AddChannel(news);
		ESP_delchan(ci);
	    }			/* while (ESP_getc_db(f) != 0) */
	}			/* for (i) */

	break;			/* case 5 and up */

    case 4:
    case 3:
    case 2:
    case 1:
	ESP_load_old_cs_dbase(f, ver);
	break;

    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", (ver, ESP_ChanDBName));

    }				/* switch (version) */

    ESP_close_db(f);

    /* Check for non-forbidden channels with no founder */
    /* for (i = 0; i < 256; i++) {
     * ESP_ChannelInfo *next;
     * for (ci = chanlists[i]; ci; ci = next) {
     * next = ci->next;
     * if (!(ci->flags & ESP_CI_VERBOTEN) && !ci->founder) {
     * SLOG(LM_INFO, "$1: database load: Deleting founderless channel $2", (
     * ESP_s_ChanServ, ci->name));
     * ESP_delchan(ci);
     * }
     * }
     * } */
}

int ESP_delchan(ESP_ChannelInfo * ci)
{
    int i;

    if (ci->founder)
	free(ci->founder);
    if (ci->successor)
	free(ci->successor);
    if (ci->url)
	free(ci->url);
    if (ci->email)
	free(ci->email);
    if (ci->desc)
	free(ci->desc);
    if (ci->mlock_key)
	free(ci->mlock_key);
    if (ci->last_topic)
	free(ci->last_topic);
    for (i = 0; i < ci->accesscount; i++)
    {
	if (ci->access[i].nick)
	    free(ci->access[i].nick);
    }
    if (ci->access)
	free(ci->access);
    for (i = 0; i < ci->akickcount; i++)
    {
	if (ci->akick[i].is_nick)
	{
	    if (ci->akick[i].u.nick)
		free(ci->akick[i].u.nick);
	}
	else
	{
	    if (ci->akick[i].u.mask)
		free(ci->akick[i].u.mask);
	}
	if (ci->akick[i].reason)
	    free(ci->akick[i].reason);
    }
    if (ci->akick)
	free(ci->akick);
    if (ci->levels)
	free(ci->levels);
    if (ci->memos.memos)
    {
	for (i = 0; i < ci->memos.memocount; i++)
	{
	    if (ci->memos.memos[i].text)
		free(ci->memos.memos[i].text);
	}
	free(ci->memos.memos);
    }
    if (ci->entry_message)
	free(ci->entry_message);
    free(ci);
    return 1;
}

#undef SAFE

/*************************************************************************/

/* Compatibility memo load routine. */

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	NSLOG(LM_EMERGENCY, "Read error on memo.db");		\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void ESP_load_old_ms_dbase(void)
{
    MemoServ::nick_memo_t memo;
    ESP_dbFILE *f;
    int ver, i, j, c;
    ESP_Memo *memos;
    ESP_MemoInfo mi;
    struct memolist_
    {
	char nick[ESP_NICKMAX];
	long n_memos;
	ESP_Memo *memos;
	long reserved[4];
    }
    old_memolist;
    struct
    {
	char sender[ESP_NICKMAX];
	long number;
	time_t time;
	char *text;
	short flags;
	short reserved_s;
	long reserved[3];
    }
    oldmemo;
    int failed = 0;

    if (!(f = ESP_open_db(ESP_s_MemoServ, "memo.db", "r")))
	return;
    switch (ver = ESP_get_file_version(f))
    {
    case 4:
    case 3:
    case 2:
    case 1:
	for (i = 33; i < 256 && !failed; ++i)
	{
	    while ((c = ESP_getc_db(f)) != 0)
	    {
		if (c != 1)
		    NSLOG(LM_EMERGENCY, "Invalid format in memo.db");
		SAFE(ESP_read_variable(old_memolist, f));
		if (ESP_debug >= 3)
		    SLOG(LM_DEBUG, "ESP_load_old_ms_dbase: got memolist for $1", (old_memolist.nick));
		old_memolist.memos = mi.memos = memos = (ESP_Memo *) malloc(sizeof(ESP_Memo) * old_memolist.n_memos);
		for (j = 0; j < old_memolist.n_memos; j++, memos++)
		{
		    SAFE(ESP_read_variable(oldmemo, f));
		    strncpy(memos->sender, oldmemo.sender, ESP_NICKMAX);
		    memos->number = oldmemo.number;
		    memos->time = oldmemo.time;
		    memos->flags = oldmemo.flags;
		}
		memos = old_memolist.memos;
		for (j = 0; j < old_memolist.n_memos; j++)
		{
		    if (ESP_read_string(&memos[j].text, f) < 0)
			NSLOG(LM_EMERGENCY, "Read error on memo.db");
		}
		mi.memocount = old_memolist.n_memos;
		mi.memomax = -1;

		memo = ESP_CreateMemoEntry(&mi, old_memolist.nick);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);

		for (j = 0; j < old_memolist.n_memos; j++)
		{
		    if (memos[j].text)
			free(memos[j].text);
		}
		if (memos)
		    free(memos);
	    }
	}
	break;
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on memo.db", (ver));
    }				/* switch (version) */
    ESP_close_db(f);
}

#undef SAFE

/*************************************************************************/

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	SLOG(LM_EMERGENCY, "Read error on $1", ( ESP_NewsDBName));	\
	ESP_nnews = i;					\
	break;						\
    }							\
} while (0)

void ESP_load_news()
{
    ESP_dbFILE *f;
    int i;
    int16 n;
    int32 tmp32;
    ESP_NewsItem *news = NULL;

    if (!(f = ESP_open_db(ESP_s_OperServ, ESP_NewsDBName, "r")))
	return;
    switch (i = ESP_get_file_version(f))
    {
    case 8:
    case 7:
	SAFE(ESP_read_int16(&n, f));
	ESP_nnews = n;
	if (ESP_nnews < 8)
	    ESP_news_size = 16;
	else if (ESP_nnews >= 16384)
	    ESP_news_size = 32767;
	else
	    ESP_news_size = 2 * ESP_nnews;
	news = (ESP_NewsItem *) malloc(sizeof(ESP_NewsItem) * ESP_news_size);
	if (!ESP_nnews)
	{
	    ESP_close_db(f);
	    return;
	}
	for (i = 0; i < ESP_nnews; i++)
	{
	    SAFE(ESP_read_int16(&news[i].type, f));
	    SAFE(ESP_read_int32(&news[i].num, f));
	    SAFE(ESP_read_string(&news[i].text, f));
	    SAFE(ESP_read_buffer(news[i].who, f));
	    SAFE(ESP_read_int32(&tmp32, f));
	    news[i].time = tmp32;
	}

	for (i = 0; i < ESP_nnews; i++)
	{
	    if (news[i].text == NULL)
		continue;

	    if (news[i].type == ESP_NEWS_LOGON && Magick::instance().commserv.IsList(Magick::instance().commserv.ALL_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.ALL_Name())->MSG_insert(mstring(news[i].text),
													mstring(news[i].who),
													mDateTime(news[i].
														  time));
	    }
	    else if (news[i].type == ESP_NEWS_OPER &&
		     Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->MSG_insert(mstring(news[i].text),
													 mstring(news[i].who),
													 mDateTime(news[i].
														   time));
	    }
	}

	for (i = 0; i < ESP_nnews; i++)
	{
	    if (news[i].text)
		free(news[i].text);
	}
	if (news)
	    free(news);

	break;

    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, ESP_NewsDBName));
    }				/* switch (ver) */

    ESP_close_db(f);
}

#undef SAFE

/*************************************************************************/

/* Load OperServ data. */

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	SLOG(LM_EMERGENCY, "Read error on $1", ( ESP_OperDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void ESP_load_os_dbase(void)
{
    ESP_dbFILE *f;
    int16 i, n, ver;
    char *s;
    int failed = 0;

    if (!(f = ESP_open_db(ESP_s_OperServ, ESP_OperDBName, "r")))
	return;
    switch (ver = ESP_get_file_version(f))
    {
    case 8:
    case 7:
    case 6:
    case 5:
	SAFE(ESP_read_int16(&n, f));
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()))
	{
	    for (i = 0; i < n && !failed; i++)
	    {
		SAFE(ESP_read_string(&s, f));
		if (s == NULL)
		    continue;

		if (!
		    (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		     Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
		    Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
													Magick::instance().
													commserv.FirstName());
		free(s);
	    }
	}
	else
	{
	    for (i = 0; i < n && !failed; i++)
	    {
		SAFE(ESP_read_string(&s, f));
		if (s)
		    free(s);
	    }
	}
	if (!failed)
	    SAFE(ESP_read_int16(&n, f));
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
	{
	    for (i = 0; i < n && !failed; i++)
	    {
		SAFE(ESP_read_string(&s, f));
		if (s == NULL)
		    continue;

		if (!
		    (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		     Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)) &&
		    !(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
		      Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->find(s)))
		    Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->insert(mstring(s),
													 Magick::instance().
													 commserv.FirstName());
		free(s);
	    }
	}
	else
	{
	    for (i = 0; i < n && !failed; i++)
	    {
		SAFE(ESP_read_string(&s, f));
		if (s)
		    free(s);
	    }
	}

/*	if (ver >= 7) {
	    int32 tmp32;
	    SAFE(ESP_read_int32(&maxusercnt, f));
	    SAFE(ESP_read_int32(&tmp32, f));
	    maxusertime = tmp32;
	} */
	break;

    case 4:
    case 3:
	SAFE(ESP_read_int16(&n, f));
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()))
	{
	    for (i = 0; i < n && !failed; i++)
	    {
		SAFE(ESP_read_string(&s, f));
		if (s == NULL)
		    continue;

		if (!
		    (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		     Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
		    Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
													Magick::instance().
													commserv.FirstName());
		free(s);
	    }
	}
	else
	{
	    for (i = 0; i < n && !failed; i++)
	    {
		SAFE(ESP_read_string(&s, f));
		if (s)
		    free(s);
	    }
	}
	break;

    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (ver, ESP_OperDBName));
    }				/* switch (version) */
    ESP_close_db(f);
}

#undef SAFE

/*************************************************************************/

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	SLOG(LM_EMERGENCY, "Read error on $1", ( ESP_AutokillDBName));	\
	ESP_nakill = i;					\
	break;						\
    }							\
} while (0)

void ESP_load_akill(void)
{
    ESP_dbFILE *f;
    int i, ver;
    int16 tmp16;
    int32 tmp32;

    ESP_Akill *akills = NULL;

    if (!(f = ESP_open_db("AKILL", ESP_AutokillDBName, "r")))
	return;

    ver = ESP_get_file_version(f);

    ESP_read_int16(&tmp16, f);
    ESP_nakill = tmp16;
    if (ESP_nakill < 8)
	ESP_akill_size = 16;
    else if (ESP_nakill >= 16384)
	ESP_akill_size = 32767;
    else
	ESP_akill_size = 2 * ESP_nakill;
    akills = (ESP_Akill *) calloc(sizeof(ESP_Akill), ESP_akill_size);

    switch (ver)
    {
    case 8:
    case 7:
    case 6:
    case 5:
	for (i = 0; i < ESP_nakill; i++)
	{
	    SAFE(ESP_read_string(&akills[i].mask, f));
	    SAFE(ESP_read_string(&akills[i].reason, f));
	    SAFE(ESP_read_buffer(akills[i].who, f));
	    SAFE(ESP_read_int32(&tmp32, f));
	    akills[i].time = tmp32;
	    SAFE(ESP_read_int32(&tmp32, f));
	    akills[i].expires = tmp32;
	}

	for (i = 0; i < ESP_nakill; ++i)
	{
	    if (akills[i].mask != NULL && akills[i].reason != NULL)
	    {
		Magick::instance().operserv.Akill_insert(mstring(akills[i].mask), akills[i].expires - akills[i].time,
							 mstring(akills[i].reason), mstring(akills[i].who),
							 mDateTime(akills[i].time));
	    }
	    if (akills[i].mask)
		free(akills[i].mask);
	    if (akills[i].reason)
		free(akills[i].reason);
	}
	free(akills);

	break;

    case 4:
    case 3:
	{
	    struct
	    {
		char *mask;
		char *reason;
		char who[ESP_NICKMAX];
		time_t time;
		time_t expires;
		long reserved[4];
	    }
	    old_akill;

	    for (i = 0; i < ESP_nakill; i++)
	    {
		SAFE(ESP_read_variable(old_akill, f));
		strncpy(akills[i].who, old_akill.who, ESP_NICKMAX);
		akills[i].time = old_akill.time;
		akills[i].expires = old_akill.expires;
	    }
	    for (i = 0; i < ESP_nakill; i++)
	    {
		SAFE(ESP_read_string(&akills[i].mask, f));
		SAFE(ESP_read_string(&akills[i].reason, f));
	    }

	    for (i = 0; i < ESP_nakill; ++i)
	    {
		if (akills[i].mask != NULL && akills[i].reason != NULL)
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[i].mask), akills[i].expires - akills[i].time,
							     mstring(akills[i].reason), mstring(akills[i].who),
							     mDateTime(akills[i].time));
		}
		if (akills[i].mask)
		    free(akills[i].mask);
		if (akills[i].reason)
		    free(akills[i].reason);
	    }
	    free(akills);
	    break;
	}			/* case 3/4 */

    case 2:
	{
	    struct
	    {
		char *mask;
		char *reason;
		char who[ESP_NICKMAX];
		time_t time;
	    }
	    old_akill;

	    for (i = 0; i < ESP_nakill; i++)
	    {
		SAFE(ESP_read_variable(old_akill, f));
		akills[i].time = old_akill.time;
		strncpy(akills[i].who, old_akill.who, sizeof(akills[i].who));
		akills[i].expires = 0;
	    }
	    for (i = 0; i < ESP_nakill; i++)
	    {
		SAFE(ESP_read_string(&akills[i].mask, f));
		SAFE(ESP_read_string(&akills[i].reason, f));
	    }

	    for (i = 0; i < ESP_nakill; ++i)
	    {
		if (akills[i].mask != NULL && akills[i].reason != NULL)
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[i].mask), akills[i].expires - akills[i].time,
							     mstring(akills[i].reason), mstring(akills[i].who),
							     mDateTime(akills[i].time));
		}
		if (akills[i].mask)
		    free(akills[i].mask);
		if (akills[i].reason)
		    free(akills[i].reason);
	    }
	    free(akills);
	    break;
	}			/* case 2 */

    case 1:
	{
	    struct
	    {
		char *mask;
		char *reason;
		time_t time;
	    }
	    old_akill;

	    for (i = 0; i < ESP_nakill; i++)
	    {
		SAFE(ESP_read_variable(old_akill, f));
		akills[i].time = old_akill.time;
		akills[i].who[0] = 0;
		akills[i].expires = 0;
	    }
	    for (i = 0; i < ESP_nakill; i++)
	    {
		SAFE(ESP_read_string(&akills[i].mask, f));
		SAFE(ESP_read_string(&akills[i].reason, f));
	    }

	    for (i = 0; i < ESP_nakill; ++i)
	    {
		if (akills[i].mask != NULL && akills[i].reason != NULL)
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[i].mask), akills[i].expires - akills[i].time,
							     mstring(akills[i].reason), mstring(akills[i].who),
							     mDateTime(akills[i].time));
		}
		if (akills[i].mask)
		    free(akills[i].mask);
		if (akills[i].reason)
		    free(akills[i].reason);
	    }
	    free(akills);
	    break;
	}			/* case 1 */

    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (ver, ESP_AutokillDBName));
    }				/* switch (version) */

    ESP_close_db(f);
}

#undef SAFE

/*************************************************************************/

#define SAFE(x) do {                                    \
    if ((x) < 0) {                                      \
        SLOG(LM_EMERGENCY, "Read error on $1", ( ESP_ExceptionDBName)); \
        ESP_nexceptions = i;                                \
        break;                                          \
    }                                                   \
} while (0)

void ESP_load_exceptions()
{
    ESP_dbFILE *f;
    int i;
    int16 n;
    int16 tmp16;
    int32 tmp32;

    ESP_Exception *exceptions = NULL;

    if (!(f = ESP_open_db(ESP_s_OperServ, ESP_ExceptionDBName, "r")))
	return;
    switch (i = ESP_get_file_version(f))
    {
    case 8:
    case 7:
	SAFE(ESP_read_int16(&n, f));
	ESP_nexceptions = n;
	if (!ESP_nexceptions)
	{
	    ESP_close_db(f);
	    return;
	}
	exceptions = (ESP_Exception *) malloc(sizeof(ESP_Exception) * ESP_nexceptions);
	for (i = 0; i < ESP_nexceptions; i++)
	{
	    SAFE(ESP_read_string(&exceptions[i].mask, f));
	    SAFE(ESP_read_int16(&tmp16, f));
	    exceptions[i].limit = tmp16;
	    SAFE(ESP_read_buffer(exceptions[i].who, f));
	    SAFE(ESP_read_string(&exceptions[i].reason, f));
	    SAFE(ESP_read_int32(&tmp32, f));
	    exceptions[i].time = tmp32;
	    SAFE(ESP_read_int32(&tmp32, f));
	    exceptions[i].expires = tmp32;
	    exceptions[i].num = i;	/* Symbolic position, never saved. */
	}

	for (i = 0; i < ESP_nexceptions; i++)
	{
	    if (exceptions[i].mask != NULL && exceptions[i].reason != NULL)
	    {
		Magick::instance().operserv.Clone_insert(mstring(exceptions[i].mask), exceptions[i].limit,
							 mstring(exceptions[i].reason), mstring(exceptions[i].who),
							 mDateTime(exceptions[i].time));
	    }
	    if (exceptions[i].mask)
		free(exceptions[i].mask);
	    if (exceptions[i].reason)
		free(exceptions[i].reason);
	}
	free(exceptions);
	break;

    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, ESP_ExceptionDBName));
    }				/* switch (ver) */

    ESP_close_db(f);
}

#undef SAFE

/*************************************************************************/

Nick_Stored_t *ESP_CreateNickEntry(ESP_NickInfo * ni)
{
    if (ni == NULL || ni->nick == NULL || !strlen(ni->nick))
	return NULL;

    if (ni->status & ESP_NS_VERBOTEN)
    {
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick);

	return out;
    }
    else if (ni->link != NULL && strlen(ni->link))
    {
	Nick_Stored_t tmp(ni->link);
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick,
					       mDateTime(ni->time_registered), tmp);

	if (out == NULL)
	    return NULL;
	if (ni->last_realname != NULL && strlen(ni->last_realname))
	    out->i_LastRealName = mstring(ni->last_realname);
	if (ni->last_usermask != NULL && strlen(ni->last_usermask))
	    out->i_LastMask = mstring(ni->last_usermask);
	if (ni->last_quit != NULL && strlen(ni->last_quit))
	    out->i_LastQuit = mstring(ni->last_quit);
	out->i_LastSeenTime = mDateTime(ni->last_seen);
	return out;
    }
    else
    {
	int i;
	char **str;
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick, ni->pass);

	if (out == NULL)
	    return NULL;
	if (ni->last_realname != NULL && strlen(ni->last_realname))
	    out->i_LastRealName = mstring(ni->last_realname);
	if (ni->last_usermask != NULL && strlen(ni->last_usermask))
	    out->i_LastMask = mstring(ni->last_usermask);
	if (ni->last_quit != NULL && strlen(ni->last_quit))
	    out->i_LastQuit = mstring(ni->last_quit);
	out->i_RegTime = mDateTime(ni->time_registered);
	out->i_LastSeenTime = mDateTime(ni->last_seen);
	if (ni->email != NULL && strlen(ni->email))
	    out->i_Email = mstring(ni->email);
	if (ni->url != NULL && strlen(ni->url))
	    out->i_URL = mstring(ni->url);
	if (out->i_URL.Contains("http://"))
	    out->i_URL.Remove("http://", false);
	if (out->i_URL.Contains("HTTP://"))
	    out->i_URL.Remove("HTTP://", false);
	for (i = 0, str = ni->access; i < ni->accesscount; ++i, ++str)
	{
	    out->i_access.insert(mstring(*str));
	}
	if (ni->flags & ESP_NI_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (ni->flags & ESP_NI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ni->flags & ESP_NI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ni->status & ESP_NS_NO_EXPIRE && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;

	if (ni->flags & ESP_NI_SUSPENDED)
	{
	    out->i_Suspend_By = ni->susp.who;
	    out->i_Suspend_Time = mDateTime(ni->susp.suspended);
	    if (ni->susp.reason != NULL && strlen(ni->susp.reason))
		out->i_Comment = mstring(ni->susp.reason);
	}

	switch (ni->language)
	{
	case ESP_LANG_EN_US:
	    out->setting.Language = "ENGLISH";
	    break;
	case ESP_LANG_JA_JIS:
	case ESP_LANG_JA_EUC:
	case ESP_LANG_JA_SJIS:
	    out->setting.Language = "JAPANESE";
	    break;
	case ESP_LANG_ES:
	    out->setting.Language = "SPANISH";
	    break;
	case ESP_LANG_PT:
	    out->setting.Language = "PORTUGUESE";
	    break;
	case ESP_LANG_FR:
	    out->setting.Language = "FRENCH";
	    break;
	case ESP_LANG_TR:
	    out->setting.Language = "TURKISH";
	    break;
	case ESP_LANG_IT:
	    out->setting.Language = "ITALIAN";
	    break;
	default:
	    out->setting.Language = "ENGLISH";
	}

	return out;
    }
}

mstring ESP_getmodes(int16 modes)
{
    mstring retval;

    if (modes & ESP_CMODE_I)
	retval += "i";
    if (modes & ESP_CMODE_M)
	retval += "m";
    if (modes & ESP_CMODE_N)
	retval += "n";
    if (modes & ESP_CMODE_P)
	retval += "p";
    if (modes & ESP_CMODE_S)
	retval += "s";
    if (modes & ESP_CMODE_T)
	retval += "t";
    if (modes & ESP_CMODE_K)
	retval += "k";
    if (modes & ESP_CMODE_L)
	retval += "l";
    if (modes & ESP_CMODE_R)
	retval += "R";
    if (modes & ESP_CMODE_r)
	retval += "r";
    if (modes & ESP_CMODE_C)
	retval += "c";

    return retval;
}

Chan_Stored_t *ESP_CreateChanEntry(ESP_ChannelInfo * ci)
{
    if (ci == NULL || ci->name == NULL || !strlen(ci->name))
	return NULL;

    if (ci->flags & ESP_CI_VERBOTEN)
    {
	Chan_Stored_t *out = new Chan_Stored_t(ci->name);

	return out;
    }
    else
    {
	ESP_ChanAccess *i_access;
	ESP_AutoKick *akick;
	int i;

	if (ci->founder == NULL || !strlen(ci->founder) || ci->desc == NULL || !strlen(ci->desc) || ci->founderpass == NULL ||
	    !strlen(ci->founderpass))
	{
	    return NULL;
	}

	Chan_Stored_t *out = new Chan_Stored_t(mstring(ci->name),
					       mstring(ci->founder), mstring(ci->founderpass),
					       mstring(ci->desc));

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

	long newlevel;
	float mod = (float) Magick::instance().chanserv.Level_Max() / (float) ESP_ACCESS_FOUNDER;

	for (i = 0, i_access = ci->access; i < ci->accesscount; ++i, ++i_access)
	{
	    if (i_access->nick == NULL)
		continue;
	    if (i_access->level < 0)
		newlevel = -1;
	    else
		newlevel = (long) ((float) i_access->level * mod);
	    if (newlevel == 0)
		newlevel = 1;
	    out->Access_insert(i_access->nick, newlevel, Magick::instance().chanserv.FirstName());
	}
	for (i = 0, akick = ci->akick; i < ci->akickcount; ++i, ++akick)
	{
	    if (akick->is_nick)
	    {
		if (akick->u.nick == NULL)
		    continue;
	    }
	    else
	    {
		if (akick->u.mask == NULL)
		    continue;
	    }

	    if (akick->reason != NULL)
	    {
		if (akick->is_nick)
		    out->Akick_insert(akick->u.nick, akick->reason, akick->who);
		else
		    out->Akick_insert(akick->u.mask, akick->reason, akick->who);
	    }
	    else
	    {
		if (akick->is_nick)
		    out->Akick_insert(akick->u.nick, akick->who);
		else
		    out->Akick_insert(akick->u.mask, akick->who);
	    }
	}

	if (ci->last_topic != NULL && strlen(ci->last_topic))
	    out->i_Topic = mstring(ci->last_topic);
	if (ci->last_topic_setter != NULL && strlen(ci->last_topic_setter))
	    out->i_Topic_Setter = mstring(ci->last_topic_setter);
	out->i_Topic_Set_Time = mDateTime(ci->last_topic_time);

	if (ci->entry_message != NULL && strlen(ci->entry_message))
	    out->Message_insert(ci->entry_message, Magick::instance().chanserv.FirstName());

	if (ci->flags & ESP_CI_KEEPTOPIC && !out->L_Keeptopic())
	    out->setting.Keeptopic = true;
	if (ci->flags & ESP_CI_SECUREOPS && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & ESP_CI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & ESP_CI_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & ESP_CI_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & ESP_CI_LEAVEOPS && !out->L_Anarchy())
	    out->setting.Anarchy = true;
	if (ci->flags & ESP_CI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & ESP_CI_NO_EXPIRE && !Magick::instance().chanserv.LCK_NoExpire())
	    out->setting.NoExpire = true;

	mstring modelock;

	if (ci->mlock_on || ci->mlock_key != NULL || ci->mlock_limit)
	{
	    mstring modes = ESP_getmodes(ci->mlock_on);

	    modes.Remove("k");
	    modes.Remove("l");
	    modelock << "+" << modes << (ci->mlock_key != NULL ? "k" : "") << (ci->mlock_limit ? "l" : "");
	}
	if (ci->mlock_off)
	{
	    modelock << "-" << ESP_getmodes(ci->mlock_off);
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
	    for (i = 0; i < ESP_CA_SIZE; ++i)
	    {
		if (ci->levels[i] == ESP_ACCESS_INVALID)
		    newlevel = Magick::instance().chanserv.Level_Max() + 2;
		else if (ci->levels[i] == ESP_ACCESS_FOUNDER)
		    newlevel = Magick::instance().chanserv.Level_Max() + 1;
		else if (ci->levels[i] < 0)
		    newlevel = -1;
		else
		    newlevel = (long) ((float) ci->levels[i] * mod);

		switch (i)
		{
		case ESP_CA_INVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_UNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_OPDEOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_ACCESS_LIST:
		    out->Level_change("VIEW", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_CLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_NOJOIN:
		    break;
		case ESP_CA_ACCESS_CHANGE:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case ESP_CA_MEMO:
		    out->Level_change("WRITEMEMO", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}

	return out;
    }
}

MemoServ::nick_memo_t ESP_CreateMemoEntry(ESP_MemoInfo * ml, char *nick)
{
    int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;
    ESP_Memo *memos;

    memos = ml->memos;
    for (i = 0; i < ml->memocount; ++i, ++memos)
    {
	if (memos->text == NULL)
	    continue;

	tmp = new Memo_t(mstring(nick), mstring(memos->sender), mstring(memos->text));
	tmp->i_Time = mDateTime(memos->time);
	if (!(memos->flags & ESP_MF_UNREAD))
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
}

MemoServ::channel_news_t ESP_CreateNewsEntry(ESP_MemoInfo * nl, char *chan)
{
    int i;

    MemoServ::channel_news_t out;
    News_t *tmp;
    ESP_Memo *memos;

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
}

#endif /* CONVERT */
