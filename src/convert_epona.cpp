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
#define RCSID(x,y) const char *rcsid_convert_epona_cpp_ ## x () { return y; }
RCSID(convert_epona_cpp, "@(#)$Id$");

/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.11  2002/01/14 07:16:55  prez
** More pretty printing with a newer indent with C++ fixes (not totally done)
**
** Revision 1.10  2002/01/13 05:18:41  prez
** More formatting, changed style slightly
**
** Revision 1.9  2002/01/12 14:42:08  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.8  2002/01/10 19:30:38  prez
** FINALLY finished a MAJOR overhaul ... now have a 'safe pointer', that
** ensures that data being used cannot be deleted while still being used.
**
** Revision 1.7  2001/12/20 08:02:32  prez
** Massive change -- 'Parent' has been changed to Magick::instance(), will
** soon also move the ACE_Reactor over, and will be able to have multipal
** instances of Magick in the same process if necessary.
**
** Revision 1.6  2001/12/12 07:43:52  prez
** Some more platform changes.  Made it look for _snprintf and _vsnprintf
** aswell (is the case on windows).  Also updated windows config.h.win.
**
** Revision 1.5  2001/11/17 07:18:12  prez
** Fixed up unbanning, so it gets ALL bans ...
**
** Revision 1.4  2001/11/12 01:05:02  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.3  2001/11/05 16:23:02  ungod
** more borland goodness
**
** Revision 1.2  2001/11/04 19:23:09  ungod
** fixed up compilation for borland c++ builder
**
** Revision 1.1  2001/11/03 21:02:53  prez
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
** added conversion for EPOERNET
**
**
** ========================================================== */

#ifdef CONVERT
#include "convert_epona.h"
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

const char *EPO_NickDBName = "nick.db";
const char *EPO_ChanDBName = "chan.db";
const char *EPO_NewsDBName = "news.db";
const char *EPO_OperDBName = "oper.db";
const char *EPO_AutokillDBName = "akill.db";
const char *EPO_ExceptionDBName = "exception.db";

const char *EPO_s_NickServ = "NickServ";
const char *EPO_s_ChanServ = "ChanServ";
const char *EPO_s_MemoServ = "MemoServ";
const char *EPO_s_OperServ = "OperServ";

const int EPO_debug = 0;
const int EPO_MSMaxMemos = 20;
const int EPO_CSMaxReg = 20;
const int EPO_CSDefBantype = 2;
const int EPO_NSAccessMax = 32;
const int EPO_NSAllowKillImmed = 0;

/*************************************************************************/

/*************************************************************************/

/* Return the version number on the file.  Return 0 if there is no version
 * number or the number doesn't make sense (i.e. less than 1 or greater
 * than EPO_FILE_VERSION).
 */

int EPO_get_file_version(EPO_dbFILE * f)
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
    else if (version < 1)
    {
	return 0;
    }
    return version;
}

/*************************************************************************/

EPO_dbFILE *EPO_open_db_read(const char *service, const char *filename)
{
    EPO_dbFILE *f;
    FILE *fp;

    static_cast < void > (service);

    f = (EPO_dbFILE *) malloc(sizeof(*f));
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

EPO_dbFILE *EPO_open_db(const char *service, const char *filename, const char *mode, uint32 version)
{
    static_cast < void > (version);

    if (*mode == 'r')
    {
	return EPO_open_db_read(service, filename);
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

void EPO_close_db(EPO_dbFILE * f)
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

int EPO_read_int16(int16 * ret, EPO_dbFILE * f)
{
    int c1, c2;

    c1 = fgetc(f->fp);
    c2 = fgetc(f->fp);
    if (c1 == EOF || c2 == EOF)
	return -1;
    *ret = c1 << 8 | c2;
    return 0;
}

int EPO_read_int32(int32 * ret, EPO_dbFILE * f)
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

int EPO_read_string(char **ret, EPO_dbFILE * f)
{
    char *s;
    int16 len;

    if (EPO_read_int16(&len, f) < 0)
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

#define HASH(nick)	((tolower((nick)[0])&31)<<5 | (tolower((nick)[1])&31))
#define SAFE(x) do {					\
    if ((x) < 0) {					\
	SLOG(LM_EMERGENCY, "Read error on $1", ( EPO_NickDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void EPO_load_old_ns_dbase(EPO_dbFILE * f, int ver)
{
    Nick_Stored_t *nick;

    MemoServ::nick_memo_t memo;

    int i, j, c;
    EPO_NickAlias *na;
    EPO_NickCore *nc;
    int failed = 0;

    int16 tmp16;
    int32 tmp32;

    char bufn[EPO_NICKMAX], bufp[EPO_PASSMAX];
    char *email, *greet, *url, *forbidby, *forbidreason;
    int32 icq;

    f = EPO_open_db(EPO_s_NickServ, EPO_NickDBName, "r", EPO_NICK_VERSION);
    if (!f)
	return;

    ver = EPO_get_file_version(f);
    if (ver <= 4)
    {
	SLOG(LM_EMERGENCY, "Invalid format in $1", (EPO_NickDBName));
	EPO_close_db(f);
	return;
    }

    for (i = 0; i < 256 && !failed; i++)
    {
	while ((c = EPO_getc_db(f)) == 1)
	{
	    if (c != 1)
	    {
		SLOG(LM_EMERGENCY, "Invalid format in %s", (EPO_NickDBName));
	    }

	    na = (EPO_NickAlias *) calloc(sizeof(EPO_NickAlias), 1);

	    SAFE(EPO_read_buffer(bufn, f));
	    na->nick = strdup(bufn);
	    SAFE(EPO_read_buffer(bufp, f));	/* Will be used later if needed */

	    SAFE(EPO_read_string(&url, f));
	    SAFE(EPO_read_string(&email, f));
	    if (ver >= 10)
		SAFE(EPO_read_int32(&icq, f));
	    else
		icq = 0;
	    if (ver >= 9)
		SAFE(EPO_read_string(&greet, f));
	    else
		greet = NULL;

	    SAFE(EPO_read_string(&na->last_usermask, f));
	    SAFE(EPO_read_string(&na->last_realname, f));
	    SAFE(EPO_read_string(&na->last_quit, f));

	    SAFE(EPO_read_int32(&tmp32, f));
	    na->time_registered = tmp32;
	    SAFE(EPO_read_int32(&tmp32, f));
	    na->last_seen = tmp32;

	    SAFE(EPO_read_int16(&na->status, f));
	    na->status &= ~EPO_NS_TEMPORARY;
	    if (na->status & EPO_NS_OLD_ENCRYPTEDPW)
	    {
		/* Bail: it makes no sense to continue with encrypted
		 * passwords, since we won't be able to verify them */
		SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted " "but encryption disabled, aborting",
		     (EPO_s_NickServ, na->nick));
	    }
	    if (ver >= 9)
	    {
		SAFE(EPO_read_string(&forbidby, f));
		SAFE(EPO_read_string(&forbidreason, f));
		/* Cleanup */
		if (forbidby && *forbidby == '@')
		{
		    free(forbidby);
		    forbidby = NULL;
		}
		if (forbidreason && *forbidreason == 0)
		{
		    free(forbidreason);
		    forbidreason = NULL;
		}
	    }
	    else
	    {
		forbidby = NULL;
		forbidreason = NULL;
	    }

	    if (na->status & EPO_NS_VERBOTEN)
	    {
		if (na->last_usermask)
		    free(na->last_usermask);
		if (na->last_realname)
		    free(na->last_realname);

		na->last_usermask = forbidby;
		na->last_realname = forbidreason;
	    }
	    else
	    {
		if (!na->last_usermask)
		    na->last_usermask = strdup("");
		if (!na->last_realname)
		    na->last_realname = strdup("");
	    }

	    /* Store the reference for later resolving */
	    SAFE(EPO_read_string(&na->host, f));
	    SAFE(EPO_read_int16(&tmp16, f));	/* Was linkcount */

	    if (na->host)
	    {
		SAFE(EPO_read_int16(&tmp16, f));	/* Was channelcount */

		nick = EPO_CreateNickEntry(na, NULL);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);
	    }
	    else
	    {
		/* This nick was a master nick, so it also has all the
		 * core info! =) 
		 */
		nc = (EPO_NickCore *) calloc(sizeof(EPO_NickCore), 1);

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

		SAFE(EPO_read_int32(&nc->flags, f));
		// condition is flagged as "always true", check...
		if (!EPO_NSAllowKillImmed)
		    nc->flags &= ~EPO_NI_KILL_IMMED;

		/* Status flags cleanup */
		if (na->status & EPO_NS_OLD_ENCRYPTEDPW)
		{
		    nc->flags |= EPO_NI_ENCRYPTEDPW;
		    na->status &= ~EPO_NS_OLD_ENCRYPTEDPW;
		}

		SAFE(EPO_read_int16(&nc->accesscount, f));
		if (nc->accesscount)
		{
		    char **i_access;
		    i_access = (char **) calloc(sizeof(char *), nc->accesscount);
		    nc->access = i_access;
		    for (j = 0; j < nc->accesscount; j++, i_access++)
			SAFE(EPO_read_string(i_access, f));
		}

		SAFE(EPO_read_int16(&nc->memos.memocount, f));
		SAFE(EPO_read_int16(&nc->memos.memomax, f));
		if (nc->memos.memocount)
		{
		    EPO_Memo *memos;

		    memos = (EPO_Memo *) calloc(sizeof(EPO_Memo), nc->memos.memocount);
		    nc->memos.memos = memos;

		    for (j = 0; j < nc->memos.memocount; j++, memos++)
		    {
			SAFE(EPO_read_int32(&memos->number, f));
			SAFE(EPO_read_int16(&memos->flags, f));
			SAFE(EPO_read_int32(&tmp32, f));
			memos->time = tmp32;
			SAFE(EPO_read_buffer(memos->sender, f));
			SAFE(EPO_read_string(&memos->text, f));
		    }
		}

		/* We read the channel count, but don't take care of it.
		 * load_cs_dbase will regenerate it correctly. */
		SAFE(EPO_read_int16(&tmp16, f));
		SAFE(EPO_read_int16(&nc->channelmax, f));
		if (ver == 5)
		    nc->channelmax = EPO_CSMaxReg;

		SAFE(EPO_read_int16(&nc->language, f));

		if (ver >= 11 && ver < 13)
		{
		    char *s;

		    SAFE(EPO_read_int16(&tmp16, f));
		    SAFE(EPO_read_int32(&tmp32, f));
		    SAFE(EPO_read_int16(&tmp16, f));
		    SAFE(EPO_read_string(&s, f));
		}

		/* Set us as being a master nick; fill the nc field also.
		 * The NS_MASTER flag will not be cleared in this function. */
		na->status |= EPO_NS_MASTER;

		nick = EPO_CreateNickEntry(na, nc);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);
		memo = EPO_CreateMemoEntry(&nc->memos, nc->display);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);
		EPO_delcore(nc);
	    }
	    EPO_delnick(na);
	}			/* while (EPO_getc_db(f) != 0) */
    }				/* for (i) */

    EPO_close_db(f);
}

void EPO_load_ns_dbase(void)
{
    Nick_Stored_t *nick;

    MemoServ::nick_memo_t memo;

    EPO_dbFILE *f;
    int ver, i, j, c;
    EPO_NickAlias *na;
    EPO_NickCore *nc, **nclast, *ncprev, *nclists[1024];
    int failed = 0;
    int16 tmp16;
    int32 tmp32;
    char *s;

    f = EPO_open_db(EPO_s_NickServ, EPO_NickDBName, "r", EPO_NICK_VERSION);
    if (!f)
	return;

    ver = EPO_get_file_version(f);
    if (ver <= 11)
    {
	EPO_close_db(f);
	EPO_load_old_ns_dbase(f, ver);
	return;
    }

    /* First we load nick cores */
    for (i = 0; i < 1024 && !failed; i++)
    {
	nclast = & nclists[i];
	ncprev = NULL;

	while ((c = EPO_getc_db(f)) == 1)
	{
	    if (c != 1)
	    {
		SLOG(LM_EMERGENCY, "Invalid format in $1", (EPO_NickDBName));
	    }

	    nc = (EPO_NickCore *) calloc(sizeof(EPO_NickCore), 1);
	    *nclast = nc;
	    nclast = & nc->next;
	    nc->prev = ncprev;
	    ncprev = nc;

	    SAFE(EPO_read_string(&nc->display, f));
	    SAFE(EPO_read_string(&nc->pass, f));
	    SAFE(EPO_read_string(&nc->email, f));
	    SAFE(EPO_read_string(&nc->greet, f));
	    SAFE(EPO_read_int32(&nc->icq, f));
	    SAFE(EPO_read_string(&nc->url, f));

	    SAFE(EPO_read_int32(&nc->flags, f));
	    // condition is flagged as "always true", check...
	    if (!EPO_NSAllowKillImmed)
		nc->flags &= ~EPO_NI_KILL_IMMED;
	    if (nc->flags & EPO_NI_ENCRYPTEDPW)
	    {
		/* Bail: it makes no sense to continue with encrypted
		 * passwords, since we won't be able to verify them */
		SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted " "but encryption disabled, aborting",
		     (EPO_s_NickServ, nc->display));
	    }
	    SAFE(EPO_read_int16(&nc->language, f));

	    /* Add services opers and admins to the appropriate list, but
	     * only if the database version is more than 10. */

	    SAFE(EPO_read_int16(&nc->accesscount, f));
	    if (nc->accesscount)
	    {
		char **i_access;
		i_access = (char **) calloc(sizeof(char *), nc->accesscount);
		nc->access = i_access;
		for (j = 0; j < nc->accesscount; j++, i_access++)
		    SAFE(EPO_read_string(i_access, f));
	    }

	    SAFE(EPO_read_int16(&nc->memos.memocount, f));
	    SAFE(EPO_read_int16(&nc->memos.memomax, f));
	    if (nc->memos.memocount)
	    {
		EPO_Memo *memos;

		memos = (EPO_Memo *) calloc(sizeof(EPO_Memo), nc->memos.memocount);
		nc->memos.memos = memos;
		for (j = 0; j < nc->memos.memocount; j++, memos++)
		{
		    SAFE(EPO_read_int32(&memos->number, f));
		    SAFE(EPO_read_int16(&memos->flags, f));
		    SAFE(EPO_read_int32(&tmp32, f));
		    memos->time = tmp32;
		    SAFE(EPO_read_buffer(memos->sender, f));
		    SAFE(EPO_read_string(&memos->text, f));
		}
	    }

	    SAFE(EPO_read_int16(&nc->channelcount, f));
	    SAFE(EPO_read_int16(&nc->channelmax, f));

	    if (ver < 13)
	    {
		/* Used to be dead authentication system */
		SAFE(EPO_read_int16(&tmp16, f));
		SAFE(EPO_read_int32(&tmp32, f));
		SAFE(EPO_read_int16(&tmp16, f));
		SAFE(EPO_read_string(&s, f));
	    }

	    memo = EPO_CreateMemoEntry(&nc->memos, nc->display);
	    if (memo.size())
		Magick::instance().memoserv.AddNick(memo);
	}			/* while (EPO_getc_db(f) != 0) */
	*nclast = NULL;
    }				/* for (i) */

    for (i = 0; i < 1024 && !failed; i++)
    {
	while ((c = EPO_getc_db(f)) == 1)
	{
	    if (c != 1)
	    {
		SLOG(LM_EMERGENCY, "Invalid format in $1", (EPO_NickDBName));
	    }

	    na = (EPO_NickAlias *) calloc(sizeof(EPO_NickAlias), 1);

	    SAFE(EPO_read_string(&na->nick, f));
	    SAFE(EPO_read_string(&na->last_usermask, f));
	    SAFE(EPO_read_string(&na->last_realname, f));
	    SAFE(EPO_read_string(&na->last_quit, f));

	    SAFE(EPO_read_int32(&tmp32, f));
	    na->time_registered = tmp32;
	    SAFE(EPO_read_int32(&tmp32, f));
	    na->last_seen = tmp32;
	    SAFE(EPO_read_int16(&na->status, f));
	    na->status &= ~EPO_NS_TEMPORARY;

	    SAFE(EPO_read_string(&na->host, f));
	    nc = EPO_findcore(na->nick, nclists);

	    if (!(na->status & EPO_NS_VERBOTEN))
	    {
		if (!na->last_usermask)
		    na->last_usermask = strdup("");
		if (!na->last_realname)
		    na->last_realname = strdup("");
	    }

	    nick = EPO_CreateNickEntry(na, nc);
	    if (nick != NULL)
		Magick::instance().nickserv.AddStored(nick);
	    EPO_delnick(na);
	}			/* while (EPO_getc_db(f) != 0) */
    }				/* for (i) */
    EPO_close_db(f);

    for (i = 0; i < 1024 && !failed; i++)
    {
	nc = nclists[i];
	while (nc != NULL)
	{
	    ncprev = nc;
	    nc = nc->next;
	    EPO_delcore(ncprev);
	}
    }
}

EPO_NickCore *EPO_findcore(const char *nick, EPO_NickCore ** nclists)
{
    EPO_NickCore *nc;

    for (nc = nclists[HASH(nick)]; nc; nc = nc->next)
    {
	if (ACE_OS_String::strcasecmp(nc->display, nick) == 0)
	    return nc;
    }

    return NULL;
}

int EPO_delnick(EPO_NickAlias * na)
{
    free(na->nick);
    if (na->last_usermask)
	free(na->last_usermask);
    if (na->last_realname)
	free(na->last_realname);
    if (na->last_quit)
	free(na->last_quit);

    free(na);
    return 1;
}

int EPO_delcore(EPO_NickCore * nc)
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

    if (nc->access)
    {
	for (i = 0; i < nc->accesscount; i++)
	{
	    if (nc->access[i])
		free(nc->access[i]);
	}
	free(nc->access);
    }

    if (nc->memos.memos)
    {
	for (i = 0; i < nc->memos.memocount; i++)
	{
	    if (nc->memos.memos[i].text)
		free(nc->memos.memos[i].text);
	}
	free(nc->memos.memos);
    }

    free(nc);

    return 1;
}

#undef HASH
#undef SAFE

/*************************************************************************/

/* Load/save data files. */

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	SLOG(LM_EMERGENCY, "Read error on $1", ( EPO_ChanDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

static int def_levels[] [2] = {
    {EPO_CA_AUTOOP, 5},
    {EPO_CA_AUTOVOICE, 3},
    {EPO_CA_AUTODEOP, -1},
    {EPO_CA_NOJOIN, -2},
    {EPO_CA_INVITE, 5},
    {EPO_CA_AKICK, 10},
    {EPO_CA_SET, EPO_ACCESS_INVALID},
    {EPO_CA_CLEAR, EPO_ACCESS_INVALID},
    {EPO_CA_UNBAN, 5},
    {EPO_CA_OPDEOP, 5},
    {EPO_CA_ACCESS_LIST, 1},
    {EPO_CA_ACCESS_CHANGE, 10},
    {EPO_CA_MEMO, 10},
    {EPO_CA_ASSIGN, EPO_ACCESS_INVALID},
    {EPO_CA_BADWORDS, 10},
    {EPO_CA_NOKICK, 1},
    {EPO_CA_FANTASIA, 3},
    {EPO_CA_SAY, 5},
    {EPO_CA_GREET, 5},
    {EPO_CA_VOICEME, 3},
    {EPO_CA_VOICE, 5},
    {EPO_CA_GETKEY, 5},
    {EPO_CA_AUTOHALFOP, 4},
    {EPO_CA_AUTOPROTECT, 10},
    {EPO_CA_OPDEOPME, 5},
    {EPO_CA_HALFOPME, 4},
    {EPO_CA_HALFOP, 5},
    {EPO_CA_PROTECTME, 10},
    {EPO_CA_PROTECT, EPO_ACCESS_INVALID},
    {EPO_CA_KICKME, 5},
    {EPO_CA_KICK, 5},
    {EPO_CA_SIGNKICK, EPO_ACCESS_INVALID},
    {EPO_CA_BANME, 5},
    {EPO_CA_BAN, 5},
    {EPO_CA_TOPIC, EPO_ACCESS_INVALID},
    {EPO_CA_INFO, EPO_ACCESS_INVALID},
    {-1}
};

void EPO_reset_levels(EPO_ChannelInfo * ci)
{
    int i;

    if (ci->levels)
	free(ci->levels);
    ci->levels = (int16 *) malloc(EPO_CA_SIZE * sizeof(int16 *));
    for (i = 0; def_levels[i] [0] >= 0; i++)
	ci->levels[def_levels[i] [0]] = def_levels[i] [1];
}

void EPO_load_cs_dbase(void)
{
    Chan_Stored_t *chan;

    MemoServ::channel_news_t news;

    EPO_dbFILE *f;
    int ver, i, j, c;
    EPO_ChannelInfo *ci;
    int failed = 0;

    f = EPO_open_db(EPO_s_ChanServ, EPO_ChanDBName, "r", EPO_CHAN_VERSION);
    if (!f)
	return;

    ver = EPO_get_file_version(f);

    for (i = 0; i < 256 && !failed; i++)
    {
	int16 tmp16;
	int32 tmp32;
	int n_levels;
	char *s;

	while ((c = EPO_getc_db(f)) != 0)
	{
	    if (c != 1)
	    {
		SLOG(LM_EMERGENCY, "Invalid format in $1", (EPO_ChanDBName));
	    }

	    ci = (EPO_ChannelInfo *) calloc(sizeof(EPO_ChannelInfo), 1);
	    SAFE(EPO_read_buffer(ci->name, f));
	    SAFE(EPO_read_string(&ci->founder, f));

	    if (ver >= 7)
	    {
		SAFE(EPO_read_string(&ci->successor, f));
	    }
	    else
		ci->successor = NULL;

	    SAFE(EPO_read_buffer(ci->founderpass, f));
	    SAFE(EPO_read_string(&ci->desc, f));
	    if (!ci->desc)
		ci->desc = strdup("");
	    SAFE(EPO_read_string(&ci->url, f));
	    SAFE(EPO_read_string(&ci->email, f));
	    SAFE(EPO_read_int32(&tmp32, f));
	    ci->time_registered = tmp32;
	    SAFE(EPO_read_int32(&tmp32, f));
	    ci->last_used = tmp32;
	    SAFE(EPO_read_string(&ci->last_topic, f));
	    SAFE(EPO_read_buffer(ci->last_topic_setter, f));
	    SAFE(EPO_read_int32(&tmp32, f));
	    ci->last_topic_time = tmp32;
	    SAFE(EPO_read_int32(&ci->flags, f));
	    if (ci->flags & EPO_CI_ENCRYPTEDPW)
	    {
		/* Bail: it makes no sense to continue with encrypted
		 * passwords, since we won't be able to verify them */
		SLOG(LM_EMERGENCY, "$1: load database: password for $2 encrypted " "but encryption disabled, aborting",
		     (EPO_s_ChanServ, ci->name));
	    }
	    /* Leaveops cleanup */
	    if (ver <= 13 && (ci->flags & 0x00000020))
		ci->flags &= ~0x00000020;
	    if (ver >= 9)
	    {
		SAFE(EPO_read_string(&ci->forbidby, f));
		SAFE(EPO_read_string(&ci->forbidreason, f));
	    }
	    else
	    {
		ci->forbidreason = NULL;
		ci->forbidby = NULL;
	    }
	    if (ver >= 9)
		SAFE(EPO_read_int16(&tmp16, f));
	    else
		tmp16 = EPO_CSDefBantype;
	    ci->bantype = tmp16;
	    SAFE(EPO_read_int16(&tmp16, f));
	    n_levels = tmp16;
	    ci->levels = (int16 *) calloc(sizeof(int16), EPO_CA_SIZE);
	    EPO_reset_levels(ci);
	    for (j = 0; j < n_levels; j++)
	    {
		SAFE(EPO_read_int16(&tmp16, f));
		if (j < EPO_CA_SIZE)
		    ci->levels[j] = tmp16;
	    }
	    /* To avoid levels list silly hacks */
	    if (ver < 10)
		ci->levels[EPO_CA_OPDEOPME] = ci->levels[EPO_CA_OPDEOP];
	    if (ver < 11)
	    {
		ci->levels[EPO_CA_KICKME] = ci->levels[EPO_CA_OPDEOP];
		ci->levels[EPO_CA_KICK] = ci->levels[EPO_CA_OPDEOP];
	    }
	    if (ver < 15)
	    {
		ci->levels[EPO_CA_BANME] = ci->levels[EPO_CA_OPDEOP];
		ci->levels[EPO_CA_BAN] = ci->levels[EPO_CA_OPDEOP];
		ci->levels[EPO_CA_TOPIC] = EPO_ACCESS_INVALID;
	    }

	    SAFE(EPO_read_int16(&ci->accesscount, f));
	    if (ci->accesscount)
	    {
		ci->access = (EPO_ChanAccess *) calloc(sizeof(EPO_ChanAccess), ci->accesscount);
		for (j = 0; j < ci->accesscount; j++)
		{
		    SAFE(EPO_read_int16(&ci->access[j].in_use, f));
		    if (ci->access[j].in_use)
		    {
			SAFE(EPO_read_int16(&ci->access[j].level, f));
			SAFE(EPO_read_string(&s, f));
			if (s)
			{
			    strncpy(ci->access[j].nick, s, EPO_NICKMAX);
			    free(s);
			}
			if (ver >= 11)
			{
			    SAFE(EPO_read_int32(&tmp32, f));
			    ci->access[j].last_seen = tmp32;
			}
		    }
		    else
			ci->access[j].last_seen = 0;	/* Means we have never seen the user */
		}
	    }
	    else
		ci->access = NULL;

	    SAFE(EPO_read_int16(&ci->akickcount, f));
	    if (ci->akickcount)
	    {
		ci->akick = (EPO_AutoKick *) calloc(sizeof(EPO_AutoKick), ci->akickcount);
		for (j = 0; j < ci->akickcount; j++)
		{
		    if (ver >= 15)
		    {
			SAFE(EPO_read_int16(&ci->akick[j].flags, f));
		    }
		    else
		    {
			SAFE(EPO_read_int16(&tmp16, f));
			if (tmp16)
			    ci->akick[j].flags |= EPO_AK_USED;
		    }
		    if (ci->akick[j].flags & EPO_AK_USED)
		    {
			if (ver < 15)
			{
			    SAFE(EPO_read_int16(&tmp16, f));
			    if (tmp16)
				ci->akick[j].flags |= EPO_AK_ISNICK;
			}
			SAFE(EPO_read_string(&ci->akick[j].mask, f));
			SAFE(EPO_read_string(&s, f));
			ci->akick[j].reason = NULL;
			if (s)
			{
			    if (ci->akick[j].flags & EPO_AK_USED)
				ci->akick[j].reason = s;
			    else
				free(s);
			}
			if (ver >= 9)
			{
			    SAFE(EPO_read_string(&s, f));
			    ci->akick[j].creator = NULL;
			    if (s)
			    {
				if (ci->akick[j].flags & EPO_AK_USED)
				    ci->akick[j].creator = s;
				else
				    free(s);
			    }
			    SAFE(EPO_read_int32(&tmp32, f));
			    if (ci->akick[j].flags & EPO_AK_USED)
				ci->akick[j].addtime = tmp32;
			}
			else
			{
			    ci->akick[j].creator = NULL;
			    ci->akick[j].addtime = 0;
			}
		    }

		    /* Bugfix */
		    if ((ver == 15) && ci->akick[j].flags > 8)
		    {
			ci->akick[j].flags = 0;
			ci->akick[j].mask = NULL;
			ci->akick[j].addtime = 0;
			ci->akick[j].creator = NULL;
			ci->akick[j].reason = NULL;
		    }
		}
	    }
	    else
	    {
		ci->akick = NULL;
	    }

	    if (ver >= 10)
	    {
		SAFE(EPO_read_int32(&ci->mlock_on, f));
		SAFE(EPO_read_int32(&ci->mlock_off, f));
	    }
	    else
	    {
		SAFE(EPO_read_int16(&tmp16, f));
		ci->mlock_on = tmp16;
		SAFE(EPO_read_int16(&tmp16, f));
		ci->mlock_off = tmp16;
	    }
	    SAFE(EPO_read_int32(&ci->mlock_limit, f));
	    SAFE(EPO_read_string(&ci->mlock_key, f));
	    if (ver >= 10)
	    {
		SAFE(EPO_read_string(&ci->mlock_flood, f));
		SAFE(EPO_read_string(&ci->mlock_redirect, f));
	    }

	    SAFE(EPO_read_int16(&ci->memos.memocount, f));
	    SAFE(EPO_read_int16(&ci->memos.memomax, f));
	    if (ci->memos.memocount)
	    {
		EPO_Memo *memos;

		memos = (EPO_Memo *) calloc(sizeof(EPO_Memo), ci->memos.memocount);
		ci->memos.memos = memos;
		for (j = 0; j < ci->memos.memocount; j++, memos++)
		{
		    SAFE(EPO_read_int32(&memos->number, f));
		    SAFE(EPO_read_int16(&memos->flags, f));
		    SAFE(EPO_read_int32(&tmp32, f));
		    memos->time = tmp32;
		    SAFE(EPO_read_buffer(memos->sender, f));
		    SAFE(EPO_read_string(&memos->text, f));
		}
	    }

	    SAFE(EPO_read_string(&ci->entry_message, f));

	    /* Some cleanup */
	    if (ver <= 11)
	    {
		/* Cleanup: Founder must be != than successor */
		if (!(ci->flags & EPO_CI_VERBOTEN) && ci->successor == ci->founder)
		{
		    ci->successor = NULL;
		}
	    }

	    /* BotServ options */

	    if (ver >= 8)
	    {
		int n_ttb;

		SAFE(EPO_read_string(&s, f));
		if (s)
		    free(s);

		SAFE(EPO_read_int32(&tmp32, f));
		ci->botflags = tmp32;
		SAFE(EPO_read_int16(&tmp16, f));
		n_ttb = tmp16;
		ci->ttb = (int16 *) calloc(sizeof(int16), EPO_TTB_SIZE);
		for (j = 0; j < n_ttb; j++)
		{
		    SAFE(EPO_read_int16(&tmp16, f));
		    if (j < EPO_TTB_SIZE)
			ci->ttb[j] = tmp16;
		}
		for (j = n_ttb; j < EPO_TTB_SIZE; j++)
		{
		    ci->ttb[j] = 0;
		}
		SAFE(EPO_read_int16(&tmp16, f));
		ci->capsmin = tmp16;
		SAFE(EPO_read_int16(&tmp16, f));
		ci->capspercent = tmp16;
		SAFE(EPO_read_int16(&tmp16, f));
		ci->floodlines = tmp16;
		SAFE(EPO_read_int16(&tmp16, f));
		ci->floodsecs = tmp16;
		SAFE(EPO_read_int16(&tmp16, f));
		ci->repeattimes = tmp16;

		SAFE(EPO_read_int16(&ci->bwcount, f));
		if (ci->bwcount)
		{
		    ci->badwords = (EPO_BadWord *) calloc(sizeof(EPO_BadWord), ci->bwcount);
		    for (j = 0; j < ci->bwcount; j++)
		    {
			SAFE(EPO_read_int16(&ci->badwords[j].in_use, f));
			if (ci->badwords[j].in_use)
			{
			    SAFE(EPO_read_string(&ci->badwords[j].word, f));
			    SAFE(EPO_read_int16(&ci->badwords[j].type, f));
			}
		    }
		}
		else
		{
		    ci->badwords = NULL;
		}
	    }
	    else
	    {
		ci->botflags = 0;
		ci->ttb = (int16 *) calloc(sizeof(int16), EPO_TTB_SIZE);
		for (j = 0; j < EPO_TTB_SIZE; j++)
		    ci->ttb[j] = 0;
		ci->bwcount = 0;
		ci->badwords = NULL;
	    }
	    chan = EPO_CreateChanEntry(ci);
	    if (chan != NULL)
		Magick::instance().chanserv.AddStored(chan);

	    news = EPO_CreateNewsEntry(&ci->memos, ci->name);
	    if (news.size())
		Magick::instance().memoserv.AddChannel(news);
	    EPO_delchan(ci);
	}			/* while (EPO_getc_db(f) != 0) */
    }				/* for (i) */

    EPO_close_db(f);
}

int EPO_delchan(EPO_ChannelInfo * ci)
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
    for (i = 0; i < ci->akickcount; i++)
    {
	if (ci->akick[i].mask)
	    free(ci->akick[i].mask);
	if (ci->akick[i].reason)
	    free(ci->akick[i].reason);
	if (ci->akick[i].creator)
	    free(ci->akick[i].creator);
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
    if (ci->ttb)
	free(ci->ttb);
    for (i = 0; i < ci->bwcount; i++)
    {
	if (ci->badwords[i].word)
	    free(ci->badwords[i].word);
    }
    if (ci->badwords)
	free(ci->badwords);
    free(ci);
    return 1;
}

#undef SAFE

/*************************************************************************/

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	SLOG(LM_EMERGENCY, "Read error on $1", ( EPO_NewsDBName));	\
	nnews = i;					\
	break;						\
    }							\
} while (0)

void EPO_load_news()
{
    EPO_dbFILE *f;
    int i;
    int16 n;
    int32 tmp32;
    int32 nnews;
    EPO_NewsItem *news;

    f = EPO_open_db(EPO_s_OperServ, EPO_NewsDBName, "r", EPO_NEWS_VERSION);
    if (!f)
	return;
    switch (i = EPO_get_file_version(f))
    {
    case 9:
    case 8:
    case 7:
	SAFE(EPO_read_int16(&n, f));
	nnews = n;
	if (!nnews)
	{
	    EPO_close_db(f);
	    return;
	}
	for (i = 0; i < nnews; i++)
	{
	    news = (EPO_NewsItem *) calloc(sizeof(EPO_NewsItem), 1);
	    SAFE(EPO_read_int16(&news->type, f));
	    SAFE(EPO_read_int32(&news->num, f));
	    SAFE(EPO_read_string(&news->text, f));
	    SAFE(EPO_read_buffer(&news->who, f));
	    SAFE(EPO_read_int32(&tmp32, f));
	    news->time = tmp32;

	    if (news->type == EPO_NEWS_LOGON && Magick::instance().commserv.IsList(Magick::instance().commserv.ALL_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.ALL_Name())->MSG_insert(mstring(news->text),
													mstring(news->who),
													mDateTime(news->time));
	    }
	    else if (news->type == EPO_NEWS_OPER &&
		     Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->MSG_insert(mstring(news->text),
													 mstring(news->who),
													 mDateTime(news->
														   time));
	    }

	    if (news->text)
		free(news->text);
	    free(news);
	}
	break;

    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, EPO_NewsDBName));
    }				/* switch (ver) */

    EPO_close_db(f);
}

#undef SAFE

/*************************************************************************/

/* Load OperServ data. */

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	SLOG(LM_EMERGENCY, "Read error on $1", ( EPO_AutokillDBName));	\
	break;						\
    }							\
} while (0)

static void EPO_load_old_akill(void)
{
    EPO_dbFILE *f;
    int j;
    int32 tmp32;
    char buf[EPO_NICKMAX], *mask;
    EPO_Akill *ak;

    f = EPO_open_db(EPO_s_OperServ, EPO_AutokillDBName, "r", 9);
    if (!f)
	return;

    EPO_get_file_version(f);

    int16 nakills;

    EPO_read_int16(&nakills, f);

    for (j = 0; j < nakills; j++)
    {
	ak = (EPO_Akill *) calloc(sizeof(EPO_Akill), 1);

	ak->user = NULL;
	ak->host = NULL;
	SAFE(EPO_read_string(&mask, f));
	SAFE(EPO_read_string(&ak->reason, f));
	SAFE(EPO_read_buffer(buf, f));
	if (!*buf)
	    ak->by = strdup("<unknown>");
	else
	    ak->by = strdup(buf);
	SAFE(EPO_read_int32(&tmp32, f));
	ak->seton = tmp32 ? tmp32 : time(NULL);
	SAFE(EPO_read_int32(&tmp32, f));
	ak->expires = tmp32;

	/* Sanity checks *sigh* */
	/* No nicknames allowed! */
	if (!strchr(ak->user, '!'))
	{
	    Magick::instance().operserv.Akill_insert(mstring(mask), ak->expires - ak->seton, mstring(ak->reason),
						     mstring(ak->by), mDateTime(ak->seton));
	}

	if (ak->user)
	    free(ak->user);
	if (ak->host)
	    free(ak->host);
	if (ak->reason)
	    free(ak->reason);
	if (ak->by)
	    free(ak->by);
	free(ak);
    }

    EPO_close_db(f);
}

#undef SAFE

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	SLOG(LM_EMERGENCY, "Read error on $1", ( EPO_OperDBName));	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void EPO_load_os_dbase(void)
{
    EPO_dbFILE *f;
    int16 i, n, ver, c;
    EPO_HostCache *hc;
    int16 tmp16;
    int32 tmp32;
    char *s;
    int failed = 0;

    f = EPO_open_db(EPO_s_OperServ, EPO_OperDBName, "r", EPO_OPER_VERSION);
    if (!f)
	return;

    ver = EPO_get_file_version(f);

    if (ver <= 9)
    {

	SAFE(EPO_read_int16(&n, f));
	for (i = 0; i < n && !failed; i++)
	{
	    SAFE(EPO_read_string(&s, f));
	    if (s)
		free(s);
	}

	if (!failed)
	    SAFE(EPO_read_int16(&n, f));
	for (i = 0; i < n && !failed; i++)
	{
	    SAFE(EPO_read_string(&s, f));
	    if (s)
	    {
		if (!
		    (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		     Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(s)))
		    Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(s),
													Magick::instance().
													commserv.FirstName());
		free(s);
	    }
	}
    }

    if (ver >= 7)
    {
	SAFE(EPO_read_int32(&tmp32, f));
	SAFE(EPO_read_int32(&tmp32, f));
    }

    if (ver <= 10)
	EPO_load_old_akill();
    else
    {
	EPO_Akill *ak;

	int16 nakills;

	EPO_read_int16(&nakills, f);

	for (i = 0; i < nakills; i++)
	{
	    ak = (EPO_Akill *) calloc(sizeof(EPO_Akill), 1);

	    SAFE(EPO_read_string(&ak->user, f));
	    SAFE(EPO_read_string(&ak->host, f));
	    SAFE(EPO_read_string(&ak->by, f));
	    SAFE(EPO_read_string(&ak->reason, f));
	    SAFE(EPO_read_int32(&tmp32, f));
	    ak->seton = tmp32;
	    SAFE(EPO_read_int32(&tmp32, f));
	    ak->expires = tmp32;

	    Magick::instance().operserv.Akill_insert(mstring(ak->user) + "@" + mstring(ak->host), ak->expires - ak->seton,
						     mstring(ak->reason), mstring(ak->by), mDateTime(ak->seton));

	    if (ak->user)
		free(ak->user);
	    if (ak->host)
		free(ak->host);
	    if (ak->reason)
		free(ak->reason);
	    if (ak->by)
		free(ak->by);
	    free(ak);
	}
    }

    if (ver >= 11)
    {
	EPO_SXLine *sx;

	int16 nsglines;

	EPO_read_int16(&nsglines, f);

	for (i = 0; i < nsglines; i++)
	{
	    sx = (EPO_SXLine *) calloc(sizeof(EPO_SXLine), 1);

	    SAFE(EPO_read_string(&sx->mask, f));
	    SAFE(EPO_read_string(&sx->by, f));
	    SAFE(EPO_read_string(&sx->reason, f));
	    SAFE(EPO_read_int32(&tmp32, f));
	    sx->seton = tmp32;
	    SAFE(EPO_read_int32(&tmp32, f));
	    sx->expires = tmp32;

	    if (sx->mask)
		free(sx->mask);
	    if (sx->by)
		free(sx->by);
	    if (sx->reason)
		free(sx->reason);
	    free(sx);
	}

	if (ver >= 13)
	{
	    int16 nsqlines;

	    EPO_read_int16(&nsqlines, f);

	    for (i = 0; i < nsqlines; i++)
	    {
		sx = (EPO_SXLine *) calloc(sizeof(EPO_SXLine), 1);

		SAFE(EPO_read_string(&sx->mask, f));
		SAFE(EPO_read_string(&sx->by, f));
		SAFE(EPO_read_string(&sx->reason, f));
		SAFE(EPO_read_int32(&tmp32, f));
		sx->seton = tmp32;
		SAFE(EPO_read_int32(&tmp32, f));
		sx->expires = tmp32;

		if (sx->mask)
		    free(sx->mask);
		if (sx->by)
		    free(sx->by);
		if (sx->reason)
		    free(sx->reason);
		free(sx);
	    }
	}

	int16 nszlines;

	EPO_read_int16(&nszlines, f);

	for (i = 0; i < nszlines; i++)
	{
	    sx = (EPO_SXLine *) calloc(sizeof(EPO_SXLine), 1);

	    SAFE(EPO_read_string(&sx->mask, f));
	    SAFE(EPO_read_string(&sx->by, f));
	    SAFE(EPO_read_string(&sx->reason, f));
	    SAFE(EPO_read_int32(&tmp32, f));
	    sx->seton = tmp32;
	    SAFE(EPO_read_int32(&tmp32, f));
	    sx->expires = tmp32;

	    if (sx->mask)
		free(sx->mask);
	    if (sx->by)
		free(sx->by);
	    if (sx->reason)
		free(sx->reason);
	    free(sx);
	}
    }

    if (ver >= 12)
    {
	for (i = 0; i < 1024 && !failed; i++)
	{
	    while ((c = EPO_getc_db(f)) != 0)
	    {
		if (c != 1)
		{
		    SLOG(LM_EMERGENCY, "Invalid format in $1", (EPO_OperDBName));
		}

		hc = (EPO_HostCache *) calloc(sizeof(EPO_HostCache), 1);

		SAFE(EPO_read_string(&hc->host, f));
		SAFE(EPO_read_int16(&tmp16, f));
		hc->status = tmp16;
		SAFE(EPO_read_int32(&tmp32, f));
		hc->used = tmp32;

		if (hc->host)
		    free(hc->host);
		free(hc);
	    }			/* while (EPO_getc_db(f) != 0) */
	}			/* for (i) */
    }
    EPO_close_db(f);
}

#undef SAFE

/*************************************************************************/

#define SAFE(x) do {                                    \
    if ((x) < 0) {                                      \
        SLOG(LM_EMERGENCY, "Read error on $1", ( EPO_ExceptionDBName)); \
        nexceptions = i;                                \
        break;                                          \
    }                                                   \
} while (0)

void EPO_load_exceptions()
{
    EPO_dbFILE *f;
    int i;
    int16 n;
    int16 tmp16;
    int32 tmp32;

    EPO_Exception *exception;
    int16 nexceptions = 0;

    f = EPO_open_db(EPO_s_OperServ, EPO_ExceptionDBName, "r", EPO_EXCEPTION_VERSION);
    if (!f)
	return;
    switch (i = EPO_get_file_version(f))
    {
    case 9:
    case 8:
    case 7:
	SAFE(EPO_read_int16(&n, f));
	nexceptions = n;
	if (!nexceptions)
	{
	    EPO_close_db(f);
	    return;
	}
	for (i = 0; i < nexceptions; i++)
	{
	    exception = (EPO_Exception *) calloc(sizeof(EPO_Exception), 1);
	    SAFE(EPO_read_string(&exception->mask, f));
	    SAFE(EPO_read_int16(&tmp16, f));
	    exception->limit = tmp16;
	    SAFE(EPO_read_buffer(exception->who, f));
	    SAFE(EPO_read_string(&exception->reason, f));
	    SAFE(EPO_read_int32(&tmp32, f));
	    exception->time = tmp32;
	    SAFE(EPO_read_int32(&tmp32, f));
	    exception->expires = tmp32;

	    if (exception->mask != NULL && exception->reason != NULL)
	    {
		Magick::instance().operserv.Clone_insert(mstring(exception->mask), exception->limit,
							 mstring(exception->reason), mstring(exception->who),
							 mDateTime(exception->time));
	    }
	    if (exception->mask)
		free(exception->mask);
	    if (exception->reason)
		free(exception->reason);

	    free(exception);
	}
	break;

    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", (i, EPO_ExceptionDBName));
    }				/* switch (ver) */

    EPO_close_db(f);
}

#undef SAFE

/*************************************************************************/

Nick_Stored_t *EPO_CreateNickEntry(EPO_NickAlias * na, EPO_NickCore * nc)
{
    if (na == NULL || na->nick == NULL || !strlen(na->nick))
	return NULL;

    if (na->status & EPO_NS_VERBOTEN)
    {
	Nick_Stored_t *out = new Nick_Stored_t(na->nick);

	return out;
    }
    else if (nc == NULL && na->host && strlen(na->host))
    {
	Nick_Stored_t tmp(na->host);
	Nick_Stored_t *out = new Nick_Stored_t(na->nick,
					       mDateTime(na->time_registered), tmp);

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
	if (nc->flags & EPO_NI_KILLPROTECT && !out->L_Protect())
	    out->setting.Protect = true;
	if (nc->flags & EPO_NI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (nc->flags & EPO_NI_MSG && !out->L_PRIVMSG())
	    out->setting.PRIVMSG = true;
	if (nc->flags & EPO_NI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (na->status & EPO_NS_NO_EXPIRE && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;

	if ((nc->flags & EPO_NI_SERVICES_ADMIN) && Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name())
	    && !Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->find(na->nick))
	{
	    if (!
		(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(na->nick)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->insert(mstring(na->nick),
												    Magick::instance().
												    commserv.FirstName());
	}
	else if ((nc->flags & EPO_NI_SERVICES_OPER) &&
		 Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		 !Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->find(na->nick))
	{
	    if (!
		(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->find(na->nick)) &&
		!(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->find(na->nick)))
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->insert(mstring(na->nick),
												     Magick::instance().
												     commserv.FirstName());
	}

	switch (nc->language)
	{
	case EPO_LANG_EN_US:
	    out->setting.Language = "ENGLISH";
	    break;
	case EPO_LANG_JA_JIS:
	case EPO_LANG_JA_EUC:
	case EPO_LANG_JA_SJIS:
	    out->setting.Language = "JAPANESE";
	    break;
	case EPO_LANG_ES:
	    out->setting.Language = "SPANISH";
	    break;
	case EPO_LANG_PT:
	    out->setting.Language = "PORTUGUESE";
	    break;
	case EPO_LANG_FR:
	    out->setting.Language = "FRENCH";
	    break;
	case EPO_LANG_TR:
	    out->setting.Language = "TURKISH";
	    break;
	case EPO_LANG_IT:
	    out->setting.Language = "ITALIAN";
	    break;
	case EPO_LANG_DE:
	    out->setting.Language = "GERMAN";
	    break;
	default:
	    out->setting.Language = "ENGLISH";
	}

	return out;
    }
    else
    {
	return NULL;
    }
}

mstring EPO_getmodes(int16 modes)
{
    mstring retval;

    if (modes & EPO_CMODE_i)
	retval += "i";
    if (modes & EPO_CMODE_m)
	retval += "m";
    if (modes & EPO_CMODE_n)
	retval += "n";
    if (modes & EPO_CMODE_p)
	retval += "p";
    if (modes & EPO_CMODE_s)
	retval += "s";
    if (modes & EPO_CMODE_t)
	retval += "t";
    if (modes & EPO_CMODE_k)
	retval += "k";
    if (modes & EPO_CMODE_l)
	retval += "l";
    if (modes & EPO_CMODE_R)
	retval += "R";
    if (modes & EPO_CMODE_r)
	retval += "r";
#ifdef EPO_IRC_BAHAMUT
    if (modes & EPO_CMODE_c)
	retval += "c";
    if (modes & EPO_CMODE_O)
	retval += "O";
#endif
#ifdef EPO_IRC_ULTIMATE
    if (modes & EPO_CMODE_f)
	retval += "f";
    if (modes & EPO_CMODE_x)
	retval += "x";
    if (modes & EPO_CMODE_A)
	retval += "A";
    if (modes & EPO_CMODE_I)
	retval += "I";
    if (modes & EPO_CMODE_K)
	retval += "K";
    if (modes & EPO_CMODE_L)
	retval += "L";
    if (modes & EPO_CMODE_O)
	retval += "O";
    if (modes & EPO_CMODE_S)
	retval += "S";
#endif
#ifdef EPO_IRC_UNREAL
    if (modes & EPO_CMODE_c)
	retval += "c";
    if (modes & EPO_CMODE_A)
	retval += "A";
    if (modes & EPO_CMODE_H)
	retval += "H";
    if (modes & EPO_CMODE_K)
	retval += "K";
    if (modes & EPO_CMODE_L)
	retval += "L";
    if (modes & EPO_CMODE_O)
	retval += "O";
    if (modes & EPO_CMODE_S)
	retval += "S";
    if (modes & EPO_CMODE_V)
	retval += "V";
    if (modes & EPO_CMODE_f)
	retval += "f";
    if (modes & EPO_CMODE_G)
	retval += "G";
    if (modes & EPO_CMODE_C)
	retval += "C";
    if (modes & EPO_CMODE_u)
	retval += "u";
    if (modes & EPO_CMODE_z)
	retval += "z";
    if (modes & EPO_CMODE_N)
	retval += "N";
#endif

    return retval;
}

Chan_Stored_t *EPO_CreateChanEntry(EPO_ChannelInfo * ci)
{
    if (ci == NULL || ci->name == NULL || !strlen(ci->name))
	return NULL;

    if (ci->flags & EPO_CI_VERBOTEN)
    {
	Chan_Stored_t *out = new Chan_Stored_t(mstring(ci->name));

	return out;
    }
    else
    {
	EPO_ChanAccess *i_access;
	EPO_AutoKick *akick;
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
	float mod = (float) Magick::instance().chanserv.Level_Max() / (float) EPO_ACCESS_FOUNDER;

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
	    if (!(akick->flags & EPO_AK_USED))
		continue;
	    if (akick->mask == NULL || !strlen(akick->mask))
		continue;

	    if (akick->reason != NULL && strlen(akick->reason))
		out->Akick_insert(akick->mask, akick->reason, akick->creator, mDateTime(akick->addtime));
	    else
		out->Akick_insert(akick->mask, akick->creator, mDateTime(akick->addtime));
	}

	if (ci->last_topic != NULL && strlen(ci->last_topic))
	    out->i_Topic = mstring(ci->last_topic);
	if (ci->last_topic_setter != NULL && strlen(ci->last_topic_setter))
	    out->i_Topic_Setter = mstring(ci->last_topic_setter);
	out->i_Topic_Set_Time = mDateTime(ci->last_topic_time);

	if (ci->entry_message != NULL && strlen(ci->entry_message))
	    out->Message_insert(ci->entry_message, Magick::instance().chanserv.FirstName());

	if (ci->flags & EPO_CI_KEEPTOPIC && !out->L_Keeptopic())
	    out->setting.Keeptopic = true;
	if (ci->flags & EPO_CI_SECUREOPS && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & EPO_CI_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & EPO_CI_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & EPO_CI_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & EPO_CI_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & EPO_CI_NO_EXPIRE && !Magick::instance().chanserv.LCK_NoExpire())
	    out->setting.NoExpire = true;

	mstring modelock;

	if (ci->mlock_on || ci->mlock_key != NULL || ci->mlock_limit)
	{
	    mstring modes = EPO_getmodes(ci->mlock_on);

	    modes.Remove("k");
	    modes.Remove("l");
	    modelock << "+" << modes << (ci->mlock_key != NULL ? "k" : "") << (ci->mlock_limit ? "l" : "");
	}
	if (ci->mlock_off)
	{
	    modelock << "-" << EPO_getmodes(ci->mlock_off);
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
	    for (i = 0; i < EPO_CA_SIZE; ++i)
	    {
		if (ci->levels[i] == EPO_ACCESS_INVALID)
		    newlevel = Magick::instance().chanserv.Level_Max() + 2;
		else if (ci->levels[i] == EPO_ACCESS_FOUNDER)
		    newlevel = Magick::instance().chanserv.Level_Max() + 1;
		else if (ci->levels[i] < 0)
		    newlevel = -1;
		else
		    newlevel = (long) ((float) ci->levels[i] * mod);

		switch (i)
		{
		case EPO_CA_INVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_UNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_OPDEOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_ACCESS_LIST:
		    out->Level_change("VIEW", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_CLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_NOJOIN:
		    break;
		case EPO_CA_ACCESS_CHANGE:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_MEMO:
		    out->Level_change("WRITEMEMO", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_ASSIGN:
		    break;
		case EPO_CA_BADWORDS:
		    break;
		case EPO_CA_NOKICK:
		    break;
		case EPO_CA_FANTASIA:
		    break;
		case EPO_CA_SAY:
		    break;
		case EPO_CA_GREET:
		    out->Level_change("GREET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_VOICEME:
		    out->Level_change("CMDVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_VOICE:
		    break;
		case EPO_CA_GETKEY:
		    out->Level_change("CMDMODE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_AUTOHALFOP:
		    out->Level_change("AUTOHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_AUTOPROTECT:
		    break;
		case EPO_CA_OPDEOPME:
		    break;
		case EPO_CA_HALFOPME:
		    out->Level_change("CMDHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_HALFOP:
		    break;
		case EPO_CA_PROTECTME:
		    break;
		case EPO_CA_PROTECT:
		    break;
		case EPO_CA_KICKME:
		    break;
		case EPO_CA_KICK:
		    out->Level_change("CMDKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case EPO_CA_SIGNKICK:
		    break;
		case EPO_CA_BANME:
		    break;
		case EPO_CA_BAN:
		    break;
		case EPO_CA_TOPIC:
		    break;
		case EPO_CA_INFO:
		    break;
		}
	    }
	}

	return out;
    }
}

MemoServ::nick_memo_t EPO_CreateMemoEntry(EPO_MemoInfo * ml, char *nick)
{
    int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;
    EPO_Memo *memos;

    memos = ml->memos;
    for (i = 0; i < ml->memocount; ++i, ++memos)
    {
	if (memos->text == NULL)
	    continue;

	tmp = new Memo_t(mstring(nick), mstring(memos->sender), mstring(memos->text));
	tmp->i_Time = mDateTime(memos->time);
	if (!(memos->flags & EPO_MF_UNREAD))
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
}

MemoServ::channel_news_t EPO_CreateNewsEntry(EPO_MemoInfo * nl, char *chan)
{
    int i;

    MemoServ::channel_news_t out;
    News_t *tmp;
    EPO_Memo *memos;

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
