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
#define RCSID(x,y) const char *rcsid_convert_magick_cpp_ ## x () { return y; }
RCSID(convert_magick_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.8  2001/12/20 08:02:32  prez
** Massive change -- 'Parent' has been changed to Magick::instance(), will
** soon also move the ACE_Reactor over, and will be able to have multipal
** instances of Magick in the same process if necessary.
**
** Revision 1.7  2001/11/12 01:05:02  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.6  2001/11/03 21:02:53  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
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
** Revision 1.23  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.22  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.21  2000/10/10 11:47:51  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.20  2000/09/30 10:48:07  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.19  2000/09/13 12:45:33  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.18  2000/09/06 11:27:33  prez
** Finished the T_Modify / T_Changing traces, fixed a bug in clone
** adding (was adding clone liimt as the mask length), updated docos
** a little more, and added a response to SIGINT to signon clients.
**
** Revision 1.17  2000/08/07 12:20:27  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.16  2000/07/30 09:04:05  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.15  2000/07/29 21:58:53  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.14  2000/05/21 04:49:39  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.13  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.12  2000/03/28 16:20:58  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.11  2000/03/19 08:50:54  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.10  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.9  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.8  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.7  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.6  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#ifdef CONVERT
#include "convert_magick.h"
#include "magick.h"

const int file_version = 5;
const char *nickserv_db = "nick.db";
const char *chanserv_db = "chan.db";
const char *memoserv_db = "memo.db";
const char *newsserv_db = "news.db";
const char *sop_db = "sop.db";
const char *message_db = "message.db";
const char *akill_db = "akill.db";
const char *clone_db = "clone.db";

int
get_file_version (FILE * f, const char *filename)
{
    int version = fgetc (f) << 24 | fgetc (f) << 16 | fgetc (f) << 8 | fgetc (f);
    if (ferror (f) || feof (f))
    {
	SLOG(LM_EMERGENCY, "Error reading version number on $1", ( filename));
    }
    else if (version > file_version || version < 1)
    {
	SLOG(LM_EMERGENCY, "Invalid version number ($1) on $2", ( version, filename));
    }
    return version;
}

char *
read_string (FILE * f, const char *filename)
{
    char *s;
    unsigned int len;

    len = fgetc (f) * 256 + fgetc (f);
    s = (char *) malloc (len);
    if (len != ACE_OS::fread (s, 1, len, f))
    {
	SLOG(LM_EMERGENCY, "Read error on $1", ( filename));
    }
    return s;
}


void
load_ns_dbase (void)
{
    FILE *f = ACE_OS::fopen (nickserv_db, "r");
    int i, j;
    NickInfo *ni;
    Nick_Stored_t nick;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read NickServ database $1", ( nickserv_db));
	return;
    }
    switch (i = get_file_version (f, nickserv_db))
    {
    case 5:
    case 4:
	for (i = 33; i < 256; ++i)
	while (fgetc (f) == 1)
	{
	    ni = (NickInfo *) malloc (sizeof (NickInfo));
	    memset(ni, 0, sizeof(NickInfo));
	    if (1 != ACE_OS::fread (ni, sizeof (NickInfo), 1, f))
	    {
		SLOG(LM_EMERGENCY, "Read error on $1", ( nickserv_db));
	    }
	    ni->flags &= ~(NI_IDENTIFIED | NI_RECOGNIZED);
	    if (ni->email) {
		ni->email = read_string (f, nickserv_db);
		if (!ACE_OS::strlen(ni->email)) {
		    free(ni->email);
		    ni->email = NULL;
		}
	    } else
		ni->email = NULL;
	    if (ni->url) {
		ni->url = read_string (f, nickserv_db);
		if (!ACE_OS::strlen(ni->url)) {
		    free(ni->url);
		    ni->url = NULL;
		}
	    } else
		ni->url = NULL;
	    ni->last_usermask = read_string (f, nickserv_db);
	    ni->last_realname = read_string (f, nickserv_db);
	    if (ni->accesscount)
	    {
		char **i_access;
		i_access = (char **) malloc (sizeof (char *) * ni->accesscount);
		ni->access = i_access;
		for (j = 0; j < ni->accesscount; ++j, ++i_access)
		    *i_access = read_string (f, nickserv_db);
	    }
	    if (ni->ignorecount)
	    {
		char **ignore;
		ignore = (char **) malloc (sizeof (char *) * ni->ignorecount);
		ni->ignore = ignore;
		for (j = 0; j < ni->ignorecount; ++j, ++ignore)
		    *ignore = read_string (f, nickserv_db);
	    }

	    nick = CreateNickEntry(ni);
	    if (!nick.Name().empty())
		Magick::instance().nickserv.AddStored(&nick);
	    delnick(ni);
	}
	break;
    case 3:
	{
	NickInfo_V3 *old_ni;
	for (i = 33; i < 256; ++i)
	    while (fgetc (f) == 1)
	    {
		ni = (NickInfo *) malloc (sizeof (NickInfo));
		old_ni = (NickInfo_V3 *) malloc (sizeof (NickInfo_V3));
		memset(ni, 0, sizeof(NickInfo));
		memset(old_ni, 0, sizeof(NickInfo_V3));
		if (1 != ACE_OS::fread (old_ni, sizeof (NickInfo_V3), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", ( nickserv_db));
		}
		ACE_OS::strcpy(ni->nick, old_ni->nick);
		ACE_OS::strcpy(ni->pass, old_ni->pass);
		ni->time_registered = old_ni->time_registered;
		ni->last_seen = old_ni->last_seen;
		ni->accesscount = old_ni->accesscount;
		ni->ignorecount = old_ni->ignorecount;
		ni->flags = old_ni->flags;
		ni->flags &= ~(NI_IDENTIFIED | NI_RECOGNIZED);

		if (old_ni->email)
		{
		    old_ni->email = read_string (f, nickserv_db);
		    if (ACE_OS::strlen(old_ni->email) > 0)
			ni->email = old_ni->email;
		    else
			ni->email = NULL;
		}
		else
		    ni->email = NULL;
		
		if (old_ni->url)
		{
		    old_ni->url = read_string (f, nickserv_db);
		    if (ACE_OS::strlen(old_ni->url) > 0)
			ni->url = old_ni->url;
		    else
			ni->url = NULL;
		}
		else
		    ni->url = NULL;
		old_ni->last_usermask = read_string (f, nickserv_db);
		ni->last_usermask = old_ni->last_usermask;
		old_ni->last_realname = read_string (f, nickserv_db);
		ni->last_realname = old_ni->last_realname;
		free(old_ni);

		if (ni->accesscount)
		{
		    char **i_access;
		    i_access = (char **) malloc (sizeof (char *) * ni->accesscount);
		    ni->access = i_access;
		    for (j = 0; j < ni->accesscount; ++j, ++i_access)
			*i_access = read_string (f, nickserv_db);
		}

		nick = CreateNickEntry(ni);
		if (!nick.Name().empty())
		    Magick::instance().nickserv.AddStored(&nick);
		delnick(ni);
	    }
	break;
	}
    case 2:
    case 1:
	{
	NickInfo_V1 *old_ni;
	for (i = 33; i < 256; ++i)
	    while (fgetc (f) == 1)
	    {
		ni = (NickInfo *) malloc (sizeof (NickInfo));
		old_ni = (NickInfo_V1 *) malloc (sizeof (NickInfo_V1));
		memset(ni, 0, sizeof(NickInfo));
		memset(old_ni, 0, sizeof(NickInfo_V1));
		if (1 != ACE_OS::fread (ni, sizeof (NickInfo_V1), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", ( nickserv_db));
		}
		ACE_OS::strcpy(ni->nick, old_ni->nick);
		ACE_OS::strcpy(ni->pass, old_ni->pass);
		ni->time_registered = old_ni->time_registered;
		ni->last_seen = old_ni->last_seen;
		ni->accesscount = old_ni->accesscount;
		ni->flags = old_ni->flags;
		ni->flags &= ~(NI_IDENTIFIED | NI_RECOGNIZED);

		old_ni->last_usermask = read_string (f, nickserv_db);
		ni->last_usermask = old_ni->last_usermask;
		old_ni->last_realname = read_string (f, nickserv_db);
		ni->last_realname = old_ni->last_realname;
		free(old_ni);

		if (ni->accesscount)
		{
		    char **i_access;
		    i_access = (char **) malloc (sizeof (char *) * ni->accesscount);
		    ni->access = i_access;
		    for (j = 0; j < ni->accesscount; ++j, ++i_access)
			*i_access = read_string (f, nickserv_db);
		}

		nick = CreateNickEntry(ni);
		if (!nick.Name().empty())
		    Magick::instance().nickserv.AddStored(&nick);
		delnick(ni);
	    }
	break;
	}
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", ( i, nickserv_db));
    }				/* switch (version) */
    fclose (f);
}

/* Remove a nick from the NickServ database.  Return 1 on success, 0
 * otherwise. */

void
delnick (NickInfo * ni)
{
    int i;

    if (ni->email)
	free (ni->email);
    if (ni->url)
	free (ni->url);
    if (ni->last_usermask)
	free (ni->last_usermask);
    if (ni->last_realname)
	free (ni->last_realname);
    if (ni->access)
    {
	for (i = 0; i < ni->accesscount; ++i)
	    free (ni->access[i]);
	free (ni->access);
    }
    if (ni->ignore)
    {
	for (i = 0; i < ni->ignorecount; ++i)
	    free (ni->ignore[i]);
        free (ni->ignore);
    }
    free (ni);
    ni = NULL;
}

Nick_Stored_t
CreateNickEntry(NickInfo_CUR *ni)
{
    int i;
    char **string;

    if (ni->flags & NI_VERBOTEN)
    {
	Nick_Stored_t out(ni->nick);
	return out;
    }
    else if (ni->flags & NI_SLAVE && ni->last_usermask != NULL)
    {
	Nick_Stored_t tmp(mstring(ni->last_usermask));
	Nick_Stored_t out(mstring(ni->nick),
			mDateTime(ni->time_registered), tmp);
	out.i_LastSeenTime = mDateTime(ni->last_seen);
	return out;
    }
    else
    {
	Nick_Stored_t out(mstring(ni->nick), mstring(ni->pass));
	if (ni->email != NULL && strlen(ni->email))
	    out.i_Email = mstring(ni->email);
	if (ni->url != NULL && strlen(ni->url))
	    out.i_URL = mstring(ni->url);
	if (out.i_URL.Contains("http://"))
	    out.i_URL.Remove("http://", false);
	if (out.i_URL.Contains("HTTP://"))
	    out.i_URL.Remove("HTTP://", false);
	if (ni->last_realname != NULL && strlen(ni->last_realname))
	    out.i_LastRealName = mstring(ni->last_realname);
	out.i_RegTime = mDateTime(ni->time_registered);
	out.i_LastSeenTime = mDateTime(ni->last_seen);
	for (i=0, string = ni->access; i<ni->accesscount; ++i, ++string)
	{
	    out.i_access.insert(mstring(*string));
	}
	for (i=0, string = ni->ignore; i<ni->ignorecount; ++i, ++string)
	{
	    out.i_ignore.insert(mstring(*string));
	}

	if (ni->flags & NI_KILLPROTECT && !out.L_Protect())
	    out.setting.Protect = true;
	if (ni->flags & NI_SECURE && !out.L_Secure())
	    out.setting.Secure = true;
	if (ni->flags & NI_PRIVATE && !out.L_Private())
	    out.setting.Private = true;
	if (ni->flags & NI_PRIVMSG && !out.L_PRIVMSG())
	    out.setting.PRIVMSG = true;

	if (ni->flags & NI_SUSPENDED)
	{
	    out.i_Suspend_By = Magick::instance().nickserv.FirstName();
	    out.i_Suspend_Time = mDateTime::CurrentDateTime();
	    if (ni->last_usermask != NULL && strlen(ni->last_usermask))
		out.i_Comment = mstring(ni->last_usermask);
	}
	else
	{
	    if (ni->last_usermask != NULL && strlen(ni->last_usermask))
		out.i_LastMask = mstring(ni->last_usermask);
	}

	if (ni->flags & NI_IRCOP)
	{
	    out.setting.NoExpire = true;
	    // NOT a SADMIN, and OPER does exist.
	    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name()).find(out.i_Name)) &&
		Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).insert(
		    mstring(out.i_Name), Magick::instance().commserv.FirstName());
	}

	return out;
    }
}

void
load_cs_dbase (void)
{
    FILE *f = ACE_OS::fopen (chanserv_db, "r");
    int i, j;
    ChanInfo *ci;
    Chan_Stored_t chan;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read ChanServ database $1", ( chanserv_db));
	return;
    }

    switch (i = get_file_version (f, chanserv_db))
    {
    case 5:
	for (i = 33; i < 256; ++i)
	    while (fgetc (f) == 1)
	    {
		ci = (ChanInfo *) malloc (sizeof (ChanInfo));
		memset(ci, 0, sizeof(ChanInfo));
		if (1 != ACE_OS::fread (ci, sizeof (ChanInfo), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		}
		ci->desc = read_string (f, chanserv_db);
		if (ci->url)
		    ci->url = read_string (f, chanserv_db);
		if (ci->mlock_key)
		    ci->mlock_key = read_string (f, chanserv_db);
		if (ci->last_topic)
		    ci->last_topic = read_string (f, chanserv_db);

		if (ci->accesscount)
		{
		    ChanAccess *i_access;
		    i_access = (ChanAccess *) malloc (sizeof (ChanAccess) * ci->accesscount);
		    ci->access = i_access;
		    if (ci->accesscount != (long) ACE_OS::fread (i_access, sizeof (ChanAccess),
						  ci->accesscount, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		    }
		    for (j = 0; j < ci->accesscount; ++j, ++i_access)
			i_access->name = read_string (f, chanserv_db);
		    j = 0;
		    i_access = ci->access;
		    /* Clear out unused entries */
		    while (j < ci->accesscount)
		    {
			if (i_access->is_nick < 0)
			{
			    --ci->accesscount;
			    free (i_access->name);
			    if (j < ci->accesscount)
				memcpy (i_access, i_access + 1, sizeof (*i_access) *
				       (ci->accesscount - j));
			}
			else
			{
			    ++j;
			    ++i_access;
			}
		    }
		    if (ci->accesscount)
			ci->access = (ChanAccess *) realloc (ci->access,
				     sizeof (ChanAccess) * ci->accesscount);
		    else
		    {
			free (ci->access);
			ci->access = NULL;
		    }
		}		/* if (ci->accesscount) */

		if (ci->akickcount)
		{
		    AutoKick *akick;
		    akick = (AutoKick *) malloc (sizeof (AutoKick) * ci->akickcount);
		    ci->akick = akick;
		    if (ci->akickcount != (long)
			ACE_OS::fread (akick, sizeof (AutoKick), ci->akickcount, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		    }
		    for (j = 0; j < ci->akickcount; ++j, ++akick)
		    {
			akick->name = read_string (f, chanserv_db);
			if (akick->reason)
			    akick->reason = read_string (f, chanserv_db);
		    }
		    j = 0;
		    akick = ci->akick;
		    while (j < ci->akickcount)
		    {
			if (akick->is_nick < 0)
			{
			    --ci->akickcount;
			    free (akick->name);
			    if (akick->reason)
				free (akick->reason);
			    if (j < ci->akickcount)
				memcpy (akick, akick + 1, sizeof (*akick) *
				       (ci->akickcount - j));
			}
			else
			{
			    ++j;
			    ++akick;
			}
		    }
		    if (ci->akickcount)
		    {
			ci->akick = (AutoKick *) realloc (ci->akick,
					sizeof (AutoKick) * ci->akickcount);
		    }
		    else
		    {
			free (ci->akick);
			ci->akick = NULL;
		    }
		}		/* if (ci->akickcount) */

		if (ci->cmd_access)
		{
		    int n_entries;
		    ci->cmd_access = (short *) malloc (sizeof (short) * CA_SIZE);
		    n_entries = fgetc (f) << 8 | fgetc (f);
		    if (n_entries < 0)
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		    }
		    if (n_entries <= CA_SIZE)
		    {
			ACE_OS::fread (ci->cmd_access, sizeof (short), n_entries, f);
		    }
		    else
		    {
			ACE_OS::fread (ci->cmd_access, sizeof (short), CA_SIZE, f);
			ACE_OS::fseek (f, sizeof (short) * (n_entries - CA_SIZE),
			       SEEK_CUR);
		    }
		}

		chan = CreateChanEntry(ci);
		if (!chan.Name().empty())
		    Magick::instance().chanserv.AddStored(&chan);
		delchan(ci);
	    }			/* while (fgetc(f) == 1) */
	break;			/* case 5, etc. */

    case 4:
    case 3:
	{
	ChanInfo_V3 *old_ci;
	for (i = 33; i < 256; ++i)
	    while (fgetc (f) == 1)
	    {
		ci = (ChanInfo *) malloc (sizeof (ChanInfo));
		old_ci = (ChanInfo_V3 *) malloc (sizeof (ChanInfo_V3));
		memset(ci, 0, sizeof(ChanInfo));
		memset(old_ci, 0, sizeof(ChanInfo_V3));
		if (1 != ACE_OS::fread (old_ci, sizeof (ChanInfo_V3), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		}
		/* Convert old dbase! */
		ACE_OS::strcpy(ci->mlock_on, oldmodeconv(old_ci->mlock_on));
		ACE_OS::strcpy(ci->mlock_off, oldmodeconv(old_ci->mlock_off));
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
		old_ci->desc = read_string (f, chanserv_db);
		ci->desc = strdup(old_ci->desc);
		free (old_ci->desc);
		if (old_ci->url)
		{
		    old_ci->url = read_string (f, chanserv_db);
		    if (ACE_OS::strlen(old_ci->url) > 0)
			ci->url = strdup(old_ci->url);
		    else
			ci->url = NULL;
		    free (old_ci->url);
		}
		else
		    ci->url = NULL;
		if (old_ci->mlock_key)
		{
		    old_ci->mlock_key = read_string (f, chanserv_db);
		    ci->mlock_key = strdup(old_ci->mlock_key);
		    free (old_ci->mlock_key);
		}
		else
		    ci->mlock_key = NULL;
		if (old_ci->last_topic)
		{
		    old_ci->last_topic = read_string (f, chanserv_db);
		    ci->last_topic = strdup(old_ci->last_topic);
		    free (old_ci->last_topic);
		}
		else
		    ci->last_topic = NULL;

		if (ci->accesscount)
		{
		    ChanAccess *i_access;
		    i_access = (ChanAccess *) malloc (sizeof (ChanAccess) * ci->accesscount);
		    ci->access = i_access;
		    if (ci->accesscount != (long) ACE_OS::fread (i_access, sizeof (ChanAccess),
						  ci->accesscount, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		    }
		    for (j = 0; j < ci->accesscount; ++j, ++i_access)
			i_access->name = read_string (f, chanserv_db);
		    j = 0;
		    i_access = ci->access;
		    /* Clear out unused entries */
		    while (j < ci->accesscount)
		    {
			if (i_access->is_nick < 0)
			{
			    --ci->accesscount;
			    free (i_access->name);
			    if (j < ci->accesscount)
				memcpy (i_access, i_access + 1, sizeof (*i_access) *
				       (ci->accesscount - j));
			}
			else
			{
			    ++j;
			    ++i_access;
			}
		    }
		    if (ci->accesscount)
			ci->access = (ChanAccess *) realloc (ci->access,
				     sizeof (ChanAccess) * ci->accesscount);
		    else
		    {
			free (ci->access);
			ci->access = NULL;
		    }
		}		/* if (ci->accesscount) */

		if (ci->akickcount)
		{
		    AutoKick *akick;
		    akick = (AutoKick *) malloc (sizeof (AutoKick) * ci->akickcount);
		    ci->akick = akick;
		    if (ci->akickcount != (long)
			ACE_OS::fread (akick, sizeof (AutoKick), ci->akickcount, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		    }
		    for (j = 0; j < ci->akickcount; ++j, ++akick)
		    {
			akick->name = read_string (f, chanserv_db);
			if (akick->reason)
			    akick->reason = read_string (f, chanserv_db);
		    }
		    j = 0;
		    akick = ci->akick;
		    while (j < ci->akickcount)
		    {
			if (akick->is_nick < 0)
			{
			    --ci->akickcount;
			    free (akick->name);
			    if (akick->reason)
				free (akick->reason);
			    if (j < ci->akickcount)
				memcpy (akick, akick + 1, sizeof (*akick) *
				       (ci->akickcount - j));
			}
			else
			{
			    ++j;
			    ++akick;
			}
		    }
		    if (ci->akickcount)
		    {
			ci->akick = (AutoKick *) realloc (ci->akick,
					sizeof (AutoKick) * ci->akickcount);
		    }
		    else
		    {
			free (ci->akick);
			ci->akick = NULL;
		    }
		}		/* if (ci->akickcount) */

		if (old_ci->cmd_access)
		{
		    int n_entries;
		    ci->cmd_access = (short *) malloc (sizeof (short) * CA_SIZE);
		    n_entries = fgetc (f) << 8 | fgetc (f);
		    if (n_entries < 0)
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		    }
		    if (n_entries <= CA_SIZE)
		    {
			ACE_OS::fread (ci->cmd_access, sizeof (short), n_entries, f);
		    }
		    else
		    {
			ACE_OS::fread (ci->cmd_access, sizeof (short), CA_SIZE, f);
			ACE_OS::fseek (f, sizeof (short) * (n_entries - CA_SIZE),
			       SEEK_CUR);
		    }
		}
		free (old_ci);

		chan = CreateChanEntry(ci);
		if (!chan.Name().empty())
		    Magick::instance().chanserv.AddStored(&chan);
		delchan(ci);
	    }			/* while (fgetc(f) == 1) */
	break;			/* case 3, etc. */
	}
    case 2:
    case 1:
	{
	ChanInfo_V1 *old_ci;
	for (i = 33; i < 256; ++i)
	    while (fgetc (f) == 1)
	    {
		ci = (ChanInfo *) malloc (sizeof (ChanInfo));
		old_ci = (ChanInfo_V1 *) malloc (sizeof (ChanInfo_V1));
		memset(ci, 0, sizeof(ChanInfo));
		memset(old_ci, 0, sizeof(ChanInfo_V1));
		if (1 != ACE_OS::fread (old_ci, sizeof (ChanInfo_V1), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		}
		/* Convert old dbase! */
		ACE_OS::strcpy(ci->mlock_on, oldmodeconv(old_ci->mlock_on));
		ACE_OS::strcpy(ci->mlock_off, oldmodeconv(old_ci->mlock_off));
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
		old_ci->desc = read_string (f, chanserv_db);
		ci->desc = strdup(old_ci->desc);
		free (old_ci->desc);
		if (old_ci->mlock_key)
		{
		    old_ci->mlock_key = read_string (f, chanserv_db);
		    ci->mlock_key = strdup(old_ci->mlock_key);
		    free (old_ci->mlock_key);
		}
		else
		    ci->mlock_key = NULL;
		if (old_ci->last_topic)
		{
		    old_ci->last_topic = read_string (f, chanserv_db);
		    ci->last_topic = strdup(old_ci->last_topic);
		    free (old_ci->last_topic);
		}
		else
		    ci->last_topic = NULL;

		if (ci->accesscount)
		{
		    ChanAccess *i_access;
		    i_access = (ChanAccess *) malloc (sizeof (ChanAccess) * ci->accesscount);
		    ci->access = i_access;
		    if (ci->accesscount != (long) ACE_OS::fread (i_access, sizeof (ChanAccess),
						  ci->accesscount, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		    }
		    for (j = 0; j < ci->accesscount; ++j, ++i_access)
			i_access->name = read_string (f, chanserv_db);
		    j = 0;
		    i_access = ci->access;
		    /* Clear out unused entries */
		    while (j < ci->accesscount)
		    {
			if (i_access->is_nick < 0)
			{
			    --ci->accesscount;
			    free (i_access->name);
			    if (j < ci->accesscount)
				memcpy (i_access, i_access + 1, sizeof (*i_access) *
				       (ci->accesscount - j));
			}
			else
			{
			    ++j;
			    ++i_access;
			}
		    }
		    if (ci->accesscount)
			ci->access = (ChanAccess *) realloc (ci->access,
				     sizeof (ChanAccess) * ci->accesscount);
		    else
		    {
			free (ci->access);
			ci->access = NULL;
		    }
		}		/* if (ci->accesscount) */

		if (ci->akickcount)
		{
		    AutoKick *akick;
		    akick = (AutoKick *) malloc (sizeof (AutoKick) * ci->akickcount);
		    ci->akick = akick;
		    if (ci->akickcount != (long)
			ACE_OS::fread (akick, sizeof (AutoKick), ci->akickcount, f))
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		    }
		    for (j = 0; j < ci->akickcount; ++j, ++akick)
		    {
			akick->name = read_string (f, chanserv_db);
			if (akick->reason)
			    akick->reason = read_string (f, chanserv_db);
		    }
		    j = 0;
		    akick = ci->akick;
		    while (j < ci->akickcount)
		    {
			if (akick->is_nick < 0)
			{
			    --ci->akickcount;
			    free (akick->name);
			    if (akick->reason)
				free (akick->reason);
			    if (j < ci->akickcount)
				memcpy (akick, akick + 1, sizeof (*akick) *
				       (ci->akickcount - j));
			}
			else
			{
			    ++j;
			    ++akick;
			}
		    }
		    if (ci->akickcount)
		    {
			ci->akick = (AutoKick *) realloc (ci->akick,
					sizeof (AutoKick) * ci->akickcount);
		    }
		    else
		    {
			free (ci->akick);
			ci->akick = NULL;
		    }
		}		/* if (ci->akickcount) */

		if (old_ci->cmd_access)
		{
		    int n_entries;
		    ci->cmd_access = (short *) malloc (sizeof (short) * CA_SIZE);
		    n_entries = fgetc (f) << 8 | fgetc (f);
		    if (n_entries < 0)
		    {
			SLOG(LM_EMERGENCY, "Read error on $1", ( chanserv_db));
		    }
		    if (n_entries <= CA_SIZE)
		    {
			ACE_OS::fread (ci->cmd_access, sizeof (short), n_entries, f);
		    }
		    else
		    {
			ACE_OS::fread (ci->cmd_access, sizeof (short), CA_SIZE, f);
			ACE_OS::fseek (f, sizeof (short) * (n_entries - CA_SIZE),
			       SEEK_CUR);
		    }
		}
		free (old_ci);

		chan = CreateChanEntry(ci);
		if (!chan.Name().empty())
		    Magick::instance().chanserv.AddStored(&chan);
		delchan(ci);
	    }
	break;			/* case 1, etc. */
	}
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", ( i, chanserv_db));
    }				/* switch (version) */
    fclose (f);
}


char *
oldmodeconv (short inmode)
{
    static char outmode[MODEMAX];
    ACE_OS::strcpy (outmode, "");
    if (inmode & 0x01) ACE_OS::strcat(outmode, "i");
    if (inmode & 0x02) ACE_OS::strcat(outmode, "m");
    if (inmode & 0x04) ACE_OS::strcat(outmode, "n");
    if (inmode & 0x08) ACE_OS::strcat(outmode, "p");
    if (inmode & 0x10) ACE_OS::strcat(outmode, "s");
    if (inmode & 0x20) ACE_OS::strcat(outmode, "t");
    if (inmode & 0x40) ACE_OS::strcat(outmode, "k");
    if (inmode & 0x80) ACE_OS::strcat(outmode, "l");
    return outmode;
}


void
delchan (ChanInfo * ci)
{
    int i;

    if (ci->desc)
	free (ci->desc);
    if (ci->url)
	free (ci->url);
    if (ci->mlock_key)
	free (ci->mlock_key);
    if (ci->last_topic)
	free (ci->last_topic);
    for (i = 0; i < ci->accesscount; ++i)
	if (ci->access[i].name)
	    free (ci->access[i].name);
    if (ci->access)
	free (ci->access);
    for (i = 0; i < ci->akickcount; ++i)
    {
	if (ci->akick[i].name)
	    free (ci->akick[i].name);
	if (ci->akick[i].reason)
	    free (ci->akick[i].reason);
    }
    if (ci->akick)
	free (ci->akick);
    free (ci);
    ci = NULL;
}	

Chan_Stored_t
CreateChanEntry(ChanInfo_CUR *ci)
{
    if (ci->flags & CI_VERBOTEN)
    {
	Chan_Stored_t out(mstring(ci->name));
	return out;
    }
    else
    {
	ChanAccess *i_access;
	AutoKick *akick;
	int i;

	if (ci->desc == NULL)
	{
	    Chan_Stored_t out;
	    return out;
	}

	Chan_Stored_t out(mstring(ci->name), mstring(ci->founder),
		    mstring(ci->founderpass), mstring(ci->desc));

	if (ci->url != NULL && strlen(ci->url))
	    out.i_URL = mstring(ci->url);
	if (out.i_URL.Contains("http://"))
	    out.i_URL.Remove("http://", false);
	if (out.i_URL.Contains("HTTP://"))
	    out.i_URL.Remove("HTTP://", false);
	out.i_RegTime = mDateTime(ci->time_registered);
	out.i_LastUsed = mDateTime(ci->last_used);
	mstring modelock;
	if (ACE_OS::strlen(ci->mlock_on) || ci->mlock_key != NULL || ci->mlock_limit)
	{
	    modelock << "+" << ci->mlock_on <<
		    (ci->mlock_key != NULL ? "k" : "") <<
		    (ci->mlock_limit ? "l" : "");
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
	    out.Mlock(Magick::instance().chanserv.FirstName(), modelock);
	for (i=0, i_access = ci->access; i<ci->accesscount; ++i, ++i_access)
	{
	    if (i_access->name == NULL)
		continue;
	    if (i_access->is_nick > 0)
	    {
		out.Access_insert(i_access->name, i_access->level,
			Magick::instance().chanserv.FirstName());
	    }
	}
	for (i=0, akick = ci->akick; i<ci->akickcount; ++i, ++akick)
	{
	    if (akick->name == NULL)
		continue;
	    if (akick->reason != NULL)
		out.Akick_insert(akick->name, akick->reason,
			Magick::instance().chanserv.FirstName());
	    else
		out.Akick_insert(akick->name,
			Magick::instance().chanserv.FirstName());
	}

	if (ci->flags & CI_KEEPTOPIC && !out.L_Keeptopic())
	    out.setting.Keeptopic = true;
	if (ci->flags & CI_SECUREOPS && !out.L_Secureops())
	    out.setting.Secureops = true;
	if (ci->flags & CI_PRIVATE && !out.L_Private())
	    out.setting.Private = true;
	if (ci->flags & CI_TOPICLOCK && !out.L_Topiclock())
	    out.setting.Topiclock = true;
	if (ci->flags & CI_RESTRICTED && !out.L_Restricted())
	    out.setting.Restricted = true;
	if (ci->flags & CI_LEAVEOPS && !out.L_Anarchy())
	    out.setting.Anarchy = true;
	if (ci->flags & CI_SECURE && !out.L_Secure())
	    out.setting.Secure = true;
	if (ci->flags & CI_JOIN && !out.L_Join())
	    out.setting.Join = true;
	if (!out.L_Revenge())
	{
	    char revlevel = ((ci->flags & CI_REV3) << 2) |
			((ci->flags & CI_REV2) << 1) |
			(ci->flags & CI_REV1);
	    switch (revlevel)
	    {
	    case CR_REVERSE:
		out.setting.Revenge = "REVERSE";
		break;
	    case CR_DEOP:
		out.setting.Revenge = "DEOP";
		break;
	    case CR_KICK:
		out.setting.Revenge = "KICK";
		break;
	    case CR_NICKBAN:
		out.setting.Revenge = "BAN1";
		break;
	    case CR_USERBAN:
		out.setting.Revenge = "BAN2";
		break;
	    case CR_HOSTBAN:
		out.setting.Revenge = "BAN3";
		break;
	    case CR_MIRROR:
		out.setting.Revenge = "MIRROR";
		break;
	    default:
		out.setting.Revenge = "NONE";
	    }
	}

	if (ci->flags & CI_SUSPENDED)
	{
	    if (ci->last_topic != NULL && strlen(ci->last_topic))
		out.i_Comment = mstring(ci->last_topic);
	    if (ci->last_topic_setter != NULL && strlen(ci->last_topic_setter))
		out.i_Suspend_By = mstring(ci->last_topic_setter);
	    out.i_Suspend_Time = mDateTime(ci->last_topic_time);
	}
	else
	{
	    if (ci->last_topic != NULL && strlen(ci->last_topic))
		out.i_Topic = mstring(ci->last_topic);
	    if (ci->last_topic_setter != NULL && strlen(ci->last_topic_setter))
		out.i_Topic_Setter = mstring(ci->last_topic_setter);
	    out.i_Topic_Set_Time = mDateTime(ci->last_topic_time);
	}

	if (ci->cmd_access != NULL)
	{
	    for (i=0; i<CA_SIZE; ++i)
	    {
		switch (i)
		{
		case CA_AUTODEOP:
		    out.Level_change("AUTODEOP", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_AUTOVOICE:
		    out.Level_change("AUTOVOICE", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_AUTOOP:
		    out.Level_change("AUTOOP", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_READMEMO:
		    out.Level_change("READMEMO", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_WRITEMEMO:
		    out.Level_change("WRITEMEMO", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_DELMEMO:
		    out.Level_change("DELMEMO", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_AKICK:
		    out.Level_change("AKICK", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_STARAKICK:
		    out.Level_change("SUPER", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    out.Level_change("OVERGREET", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_UNBAN:
		    out.Level_change("UNBAN", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_ACCESS:
		    out.Level_change("ACCESS", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_SET:
		    out.Level_change("SET", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_CMDINVITE:
		    out.Level_change("CMDINVITE", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_CMDUNBAN:
		    out.Level_change("CMDUNBAN", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_CMDVOICE:
		    out.Level_change("CMDVOICE", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_CMDOP:
		    out.Level_change("CMDOP", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		case CA_CMDCLEAR:
		    out.Level_change("CMDCLEAR", ci->cmd_access[i],
			Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}
	return out;
    }
}

void
load_ms_dbase (void)
{
    FILE *f = ACE_OS::fopen (memoserv_db, "r");
    int i, j;
    MemoList *ml;
    Memo *memos;
    MemoServ::nick_memo_t memo;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read MemoServ database $1", ( memoserv_db));
	return;
    }
    switch (i = get_file_version (f, memoserv_db))
    {
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	for (i = 33; i < 256; ++i)
	    while (fgetc (f) == 1)
	    {
		ml = (MemoList *) malloc (sizeof (MemoList));
		memset(ml, 0, sizeof(MemoList));
		if (1 != ACE_OS::fread (ml, sizeof (MemoList), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", ( memoserv_db));
		}
		ml->memos = memos = (Memo *) malloc (sizeof (Memo) * ml->n_memos);
		memset(ml->memos, 0, sizeof(Memo) * ml->n_memos);
		ACE_OS::fread (memos, sizeof (Memo), ml->n_memos, f);
		for (j = 0; j < ml->n_memos; ++j, ++memos)
		    memos->text = read_string (f, memoserv_db);

		memo = CreateMemoEntry(ml);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);
		del_memolist(ml);
	    }
	break;
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", ( i, memoserv_db));
    }				/* switch (version) */
    fclose (f);
}



void
load_news_dbase (void)
{
    FILE *f = ACE_OS::fopen (newsserv_db, "r");
    int i, j;
    NewsList *nl;
    Memo *newss;
    MemoServ::channel_news_t news;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read NewsServ database $1", ( newsserv_db));
	return;
    }
    switch (i = get_file_version (f, newsserv_db))
    {
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	for (i = 33; i < 256; ++i)
	{
	    while (fgetc (f) == 1)
	    {
		nl = (NewsList *) malloc (sizeof (NewsList));
		memset(nl, 0, sizeof(NewsList));
		if (1 != ACE_OS::fread (nl, sizeof (NewsList), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", ( newsserv_db));
		}
		nl->newss = newss = (Memo *) malloc (sizeof (Memo) * nl->n_newss);
		memset(nl->newss, 0, sizeof(Memo) * nl->n_newss);
		ACE_OS::fread (newss, sizeof (Memo), nl->n_newss, f);
		for (j = 0; j < nl->n_newss; ++j, ++newss)
		    newss->text = read_string (f, newsserv_db);

		news = CreateNewsEntry(nl);
		if (news.size())
		    Magick::instance().memoserv.AddChannel(news);
		del_newslist(nl);
	    }
	}
	break;
    default:
	SLOG(LM_EMERGENCY, "Unsupported version number ($1) on $2", ( i, newsserv_db));
    }				/* switch (version) */
    fclose (f);
}

/* del_memolist:  Remove a nick's memo list from the database.  Assumes
 *                that the memo count for the nick is non-zero.
 */

void
del_memolist (MemoList * ml)
{
    int i;
    Memo *memos;

    if (!ml) return;
    memos = ml->memos;
    for (i = 0; i < ml->n_memos; ++i, ++memos)
	free(memos->text);
    if (ml->memos)
	free (ml->memos);
    free (ml);
}

/* del_newslist:  Remove a nick's news list from the database.  Assumes
 *                that the news count for the nick is non-zero.
 */

void
del_newslist (NewsList * nl)
{
    int i;
    Memo *newss;

    if (!nl) return;
    newss = nl->newss;
    for (i = 0; i < nl->n_newss; ++i, ++newss)
	free(newss->text);
    if (nl->newss)
	free (nl->newss);
    free (nl);
}

MemoServ::nick_memo_t
CreateMemoEntry(MemoList_CUR *ml)
{
    int i;
    MemoServ::nick_memo_t out;
    Memo_t *tmp;
    Memo *memos;

    memos = ml->memos;
    for (i = 0; i < ml->n_memos; ++i, ++memos)
    {
	if (memos->text == NULL)
	    continue;

	tmp = new Memo_t(mstring(ml->nick), mstring(memos->sender),
		    mstring(memos->text));
	tmp->i_Time = mDateTime(memos->time);
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
}

MemoServ::channel_news_t
CreateNewsEntry(NewsList_CUR *nl)
{
    int i;
    MemoServ::channel_news_t out;
    News_t *tmp;
    Memo *newss;

    newss = nl->newss;
    for (i = 0; i < nl->n_newss; ++i, ++newss)
    {
	if (newss->text == NULL)
	    continue;
	tmp = new News_t(mstring(nl->chan), mstring(newss->sender),
		    mstring(newss->text));
	tmp->i_Time = mDateTime(newss->time);
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
}

void
load_sop ()
{
    FILE *f = ACE_OS::fopen (sop_db, "r");
    int i, j, nsop = 0, sop_size = 0;
    Sop *sops = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read SOP database $1", ( sop_db));
	return;
    }
    switch (i = get_file_version (f, sop_db))
    {
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	nsop = fgetc (f) * 256 + fgetc (f);
	if (nsop < 8)
	    sop_size = 16;
	else
	    sop_size = 2 * nsop;
	sops = (Sop *) malloc (sizeof (Sop) * sop_size);
	memset(sops, 0, sizeof(Sop) * sop_size);
	if (!nsop)
	{
	    fclose (f);
	    return;
	}
	if (nsop != (int) ACE_OS::fread (sops, sizeof (Sop), nsop, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", ( sop_db));
	}

	if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()))
	{
	    for (j=0; j<nsop; ++j)
	    {
 		if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		     Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name()).find(sops[j].nick)))
		    Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name()).insert(
		    mstring(sops[j].nick), Magick::instance().commserv.FirstName());
	    }
	}
	free(sops);
	break;
    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", ( i, sop_db));
    }				/* switch (version) */
    fclose (f);
}

void
load_message ()
{
    FILE *f = ACE_OS::fopen (message_db, "r");
    int i, j, nmessage = 0, message_size = 0;
    Message *messages = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read MESSAGE database $1", ( message_db));
	return;
    }
    switch (i = get_file_version (f, message_db))
    {
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	nmessage = fgetc (f) * 256 + fgetc (f);
	if (nmessage < 8)
	    message_size = 16;
	else
	    message_size = 2 * nmessage;
	messages = (Message *) malloc (sizeof (Message) * message_size);
	memset(messages, 0, sizeof(Message) * message_size);
	if (!nmessage)
	{
	    fclose (f);
	    return;
	}
	if (nmessage != (int) ACE_OS::fread (messages, sizeof (*messages), nmessage, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", ( message_db));
	}
	for (j = 0; j < nmessage; ++j)
	    messages[j].text = read_string (f, message_db);

	for (j = 0; j < nmessage; ++j)
	{
	    if (messages[j].text == NULL)
		continue;

	    if (messages[j].type == M_LOGON &&
		Magick::instance().commserv.IsList(Magick::instance().commserv.ALL_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.ALL_Name()).MSG_insert(
			mstring(messages[j].text), mstring(messages[j].who));
	    }
	    else if (messages[j].type == M_OPER &&
		Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()))
	    {
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).MSG_insert(
			mstring(messages[j].text), mstring(messages[j].who));
	    }
	}

	for (j = 0; j < nmessage; ++j)
	    free(messages[j].text);
	free(messages);
	break;

    default:
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", ( i, message_db));
    }				/* switch (version) */
    fclose (f);
}

void
load_akill ()
{
    FILE *f = ACE_OS::fopen (akill_db, "r");
    int i, j, nakill = 0, akill_size = 0;
    Akill *akills = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read AKILL database $1", ( akill_db));
	return;
    }
    switch (i = get_file_version (f, akill_db))
    {
    case 5:
    case 4:
    case 3:
    case 2:
	nakill = fgetc (f) * 256 + fgetc (f);
	if (nakill < 8)
	    akill_size = 16;
	else
	    akill_size = 2 * nakill;
	akills = (Akill *) malloc (sizeof (Akill) * akill_size);
	memset(akills, 0, sizeof(Akill) * akill_size);
	if (!nakill)
	{
	    fclose (f);
	    return;
	}
	if (nakill != (int) ACE_OS::fread (akills, sizeof (*akills), nakill, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", ( akill_db));
	}
	for (j = 0; j < nakill; ++j)
	{
	    akills[j].mask = read_string (f, akill_db);
	    akills[j].reason = read_string (f, akill_db);
	}

	for (j = 0; j < nakill; ++j)
	{
	    if (akills[j].mask != NULL && akills[j].reason != NULL)
	    {
		if (akills[j].time == 0)
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[j].mask),
			Magick::instance().operserv.Expire_Sop(),
			mstring(akills[j].reason), mstring(akills[j].who));
		}
		else
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[j].mask),
			Magick::instance().operserv.Def_Expire(),
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
	    Akill_V1 old_akill;
	    nakill = fgetc (f) * 256 + fgetc (f);
	    if (nakill < 8)
		akill_size = 16;
	    else
		akill_size = 2 * nakill;
	    akills = (Akill *) malloc (sizeof (Akill) * akill_size);
	    memset(akills, 0, sizeof(Akill) * akill_size);
	    if (!nakill)
	    {
		fclose (f);
		return;
	    }
	    for (j = 0; j < nakill; ++j)
	    {
		if (1 != ACE_OS::fread (&old_akill, sizeof (old_akill), 1, f))
		{
		    SLOG(LM_EMERGENCY, "Read error on $1", ( akill_db));
		}
		akills[j].time = old_akill.time;
		akills[j].who[0] = 0;
	    }
	    for (j = 0; j < nakill; ++j)
	    {
		akills[j].mask = read_string (f, akill_db);
		akills[j].reason = read_string (f, akill_db);
	    }
	}			/* case 1 */

	for (j = 0; j < nakill; ++j)
	{
	    if (akills[j].mask != NULL && akills[j].reason != NULL)
	    {
		if (akills[j].time == 0)
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[j].mask),
			Magick::instance().operserv.Expire_Sop(),
			mstring(akills[j].reason), mstring(akills[j].who));
		}
		else
		{
		    Magick::instance().operserv.Akill_insert(mstring(akills[j].mask),
			Magick::instance().operserv.Def_Expire(),
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
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", ( i, akill_db));
    }				/* switch (version) */
    fclose (f);
}

void
load_clone ()
{
    FILE *f = ACE_OS::fopen (clone_db, "r");
    int i, j, nclone = 0, clone_size = 0;
    Allow *clones = NULL;

    if (!f)
    {
	SLOG(LM_ERROR, "Can't read CLONE database $1", ( clone_db));
	return;
    }
    switch (i = get_file_version (f, clone_db))
    {
    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
	nclone = fgetc (f) * 256 + fgetc (f);
	if (nclone < 8)
	    clone_size = 16;
	else
	    clone_size = 2 * nclone;
	clones = (Allow *) malloc (sizeof (Allow) * clone_size);
	memset(clones, 0, sizeof(Allow) * clone_size);
	if (!nclone)
	{
	    fclose (f);
	    return;
	}
	if (nclone != (int) ACE_OS::fread (clones, sizeof (*clones), nclone, f))
	{
	    SLOG(LM_EMERGENCY, "Read error on $1", ( clone_db));
	}
	for (j = 0; j < nclone; ++j)
	{
	    clones[j].host = read_string (f, clone_db);
	    clones[j].reason = read_string (f, clone_db);
	}

	for (j = 0; j < nclone; ++j)
	{
	    if (clones[j].host != NULL && clones[j].reason != NULL)
	    {
		Magick::instance().operserv.Clone_insert(mstring(clones[j].host),
		    clones[j].amount, mstring(clones[j].reason),
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
	SLOG(LM_EMERGENCY, "Unsupported version ($1) on $2", ( i, clone_db));
    }				/* switch (version) */
    fclose (f);
}

#endif /* CONVERT */
