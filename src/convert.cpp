#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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


#include "magick.h"
#include "convert.h"

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
	wxLogFatal ("Error reading version number on %s", filename);
    else if (version > file_version || version < 1)
	wxLogFatal ("Invalid version number (%d) on %s", version, filename);
    return version;
}

char *
read_string (FILE * f, const char *filename)
{
    char *s;
    int len;

    len = fgetc (f) * 256 + fgetc (f);
    s = (char *) malloc (len);
    if (len != fread (s, 1, len, f))
	wxLogFatal ("Read error on %s", filename);
    return s;
}


void
load_ns_dbase (void)
{
    FILE *f = fopen (nickserv_db, "r");
    int i, j;
    NickInfo *ni;
    Nick_Stored_t nick;

    if (!f)
    {
	wxLogError ("Can't read NickServ database %s", nickserv_db);
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
	    if (1 != fread (ni, sizeof (NickInfo), 1, f))
		wxLogFatal ("Read error on %s", nickserv_db);
	    ni->flags &= ~(NI_IDENTIFIED | NI_RECOGNIZED);
	    if (ni->email) {
		ni->email = read_string (f, nickserv_db);
		if (!strlen(ni->email)) {
		    free(ni->email);
		    ni->email = NULL;
		}
	    } else
		ni->email = NULL;
	    if (ni->url) {
		ni->url = read_string (f, nickserv_db);
		if (!strlen(ni->url)) {
		    free(ni->url);
		    ni->url = NULL;
		}
	    } else
		ni->url = NULL;
	    ni->last_usermask = read_string (f, nickserv_db);
	    ni->last_realname = read_string (f, nickserv_db);
	    if (ni->accesscount)
	    {
		char **access;
		access = (char **) malloc (sizeof (char *) * ni->accesscount);
		ni->access = access;
		for (j = 0; j < ni->accesscount; ++j, ++access)
		    *access = read_string (f, nickserv_db);
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
	    Parent->nickserv.stored[nick.Name().LowerCase()] = nick;
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
		if (1 != fread (old_ni, sizeof (NickInfo_V3), 1, f))
		    wxLogFatal ("Read error on %s", nickserv_db);
		strcpy(ni->nick, old_ni->nick);
		strcpy(ni->pass, old_ni->pass);
		ni->time_registered = old_ni->time_registered;
		ni->last_seen = old_ni->last_seen;
		ni->accesscount = old_ni->accesscount;
		ni->ignorecount = old_ni->ignorecount;
		ni->flags = old_ni->flags;
		ni->flags &= ~(NI_IDENTIFIED | NI_RECOGNIZED);

		if (old_ni->email)
		{
		    old_ni->email = read_string (f, nickserv_db);
		    if (strlen(old_ni->email) > 0)
			ni->email = old_ni->email;
		    else
			ni->email = NULL;
		}
		else
		    ni->email = NULL;
		
		if (old_ni->url)
		{
		    old_ni->url = read_string (f, nickserv_db);
		    if (strlen(old_ni->url) > 0)
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
		    char **access;
		    access = (char **) malloc (sizeof (char *) * ni->accesscount);
		    ni->access = access;
		    for (j = 0; j < ni->accesscount; ++j, ++access)
			*access = read_string (f, nickserv_db);
		}

		nick = CreateNickEntry(ni);
		Parent->nickserv.stored[nick.Name().LowerCase()] = nick;
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
		if (1 != fread (ni, sizeof (NickInfo_V1), 1, f))
		    wxLogFatal ("Read error on %s", nickserv_db);
		strcpy(ni->nick, old_ni->nick);
		strcpy(ni->pass, old_ni->pass);
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
		    char **access;
		    access = (char **) malloc (sizeof (char *) * ni->accesscount);
		    ni->access = access;
		    for (j = 0; j < ni->accesscount; ++j, ++access)
			*access = read_string (f, nickserv_db);
		}

		nick = CreateNickEntry(ni);
		Parent->nickserv.stored[nick.Name().LowerCase()] = nick;
		delnick(ni);
	    }
	break;
	}
    default:
	wxLogFatal ("Unsupported version number (%d) on %s", i, nickserv_db);
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
CreateNickEntry(NickInfo *ni)
{
    /*
    NickInfo *next, *prev;
    char nick[NICKMAX];
    char pass[PASSMAX];
    char *email;
    char *url;
    char *last_usermask;
    char *last_realname;
    time_t time_registered;
    time_t last_seen;
    long accesscount;
    char **access;
    long ignorecount;
    char **ignore;
    long flags;
    long reserved[4]; */

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
	return out;
    }
    else
    {
	Nick_Stored_t out(mstring(ni->nick), mstring(ni->pass));
	if (ni->email != NULL)
	    out.i_Email = mstring(ni->email);
	if (ni->url != NULL)
	    out.i_URL = mstring(ni->url);
	if (ni->last_realname != NULL)
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
	    out.i_Protect = true;
	if (ni->flags & NI_SECURE && !out.L_Secure())
	    out.i_Secure = true;
	if (ni->flags & NI_PRIVATE && !out.L_Private())
	    out.i_Private = true;
	if (ni->flags & NI_PRIVMSG && !out.L_PRIVMSG())
	    out.i_PRIVMSG = true;

	if (ni->flags & NI_SUSPENDED)
	{
	    out.i_Suspend_By = Parent->nickserv.FirstName();
	    out.i_Suspend_Time = Now();
	    if (ni->last_usermask != NULL)
		out.i_Comment = mstring(ni->last_usermask);
	}
	else
	{
	    if (ni->last_usermask != NULL)
		out.i_LastMask = mstring(ni->last_usermask);
	}

	if (ni->flags & NI_IRCOP)
	{
	    out.i_NoExpire = true;
	    if (Parent->commserv.IsList(Parent->commserv.OPER_Name()))
		Parent->commserv.list[Parent->commserv.OPER_Name()].insert(
		    mstring(out.i_Name), Parent->commserv.FirstName());
	}

	return out;
    }
}

void
load_cs_dbase (void)
{
    FILE *f = fopen (chanserv_db, "r");
    int i, j;
    ChanInfo *ci;
    Chan_Stored_t chan;

    if (!f)
    {
	wxLogError ("Can't read ChanServ database %s", chanserv_db);
	return;
    }

    switch (i = get_file_version (f, chanserv_db))
    {
    case 5:
	for (i = 33; i < 256; ++i)
	    while (fgetc (f) == 1)
	    {
		ci = (ChanInfo *) malloc (sizeof (ChanInfo));
		if (1 != fread (ci, sizeof (ChanInfo), 1, f))
		    wxLogFatal ("Read error on %s", chanserv_db);
		ci->desc = read_string (f, chanserv_db);
		if (ci->url)
		    ci->url = read_string (f, chanserv_db);
		if (ci->mlock_key)
		    ci->mlock_key = read_string (f, chanserv_db);
		if (ci->last_topic)
		    ci->last_topic = read_string (f, chanserv_db);

		if (ci->accesscount)
		{
		    ChanAccess *access;
		    access = (ChanAccess *) malloc (sizeof (ChanAccess) * ci->accesscount);
		    ci->access = access;
		    if (ci->accesscount != fread (access, sizeof (ChanAccess),
						  ci->accesscount, f))
			wxLogFatal ("Read error on %s", chanserv_db);
		    for (j = 0; j < ci->accesscount; ++j, ++access)
			access->name = read_string (f, chanserv_db);
		    j = 0;
		    access = ci->access;
		    /* Clear out unused entries */
		    while (j < ci->accesscount)
		    {
			if (access->is_nick < 0)
			{
			    --ci->accesscount;
			    free (access->name);
			    if (j < ci->accesscount)
				memcpy (access, access + 1, sizeof (*access) *
				       (ci->accesscount - j));
			}
			else
			{
			    ++j;
			    ++access;
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
		    if (ci->akickcount !=
			fread (akick, sizeof (AutoKick), ci->akickcount, f))
			wxLogFatal ("Read error on %s", chanserv_db);
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
			wxLogFatal ("Read error on %s", chanserv_db);
		    if (n_entries <= CA_SIZE)
		    {
			fread (ci->cmd_access, sizeof (short), n_entries, f);
		    }
		    else
		    {
			fread (ci->cmd_access, sizeof (short), CA_SIZE, f);
			fseek (f, sizeof (short) * (n_entries - CA_SIZE),
			       SEEK_CUR);
		    }
		}

		chan = CreateChanEntry(ci);
		Parent->chanserv.stored[chan.Name().LowerCase()] = chan;
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
		if (1 != fread (old_ci, sizeof (ChanInfo_V3), 1, f))
		    wxLogFatal ("Read error on %s", chanserv_db);
		/* Convert old dbase! */
		strcpy(ci->mlock_on, oldmodeconv(old_ci->mlock_on));
		strcpy(ci->mlock_off, oldmodeconv(old_ci->mlock_off));
		strcpy(ci->name, old_ci->name);
		strcpy(ci->founder, old_ci->founder);
		strcpy(ci->founderpass, old_ci->founderpass);
		strcpy(ci->last_topic_setter, old_ci->last_topic_setter);
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
		    if (strlen(old_ci->url) > 0)
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
		    ChanAccess *access;
		    access = (ChanAccess *) malloc (sizeof (ChanAccess) * ci->accesscount);
		    ci->access = access;
		    if (ci->accesscount != fread (access, sizeof (ChanAccess),
						  ci->accesscount, f))
			wxLogFatal ("Read error on %s", chanserv_db);
		    for (j = 0; j < ci->accesscount; ++j, ++access)
			access->name = read_string (f, chanserv_db);
		    j = 0;
		    access = ci->access;
		    /* Clear out unused entries */
		    while (j < ci->accesscount)
		    {
			if (access->is_nick < 0)
			{
			    --ci->accesscount;
			    free (access->name);
			    if (j < ci->accesscount)
				memcpy (access, access + 1, sizeof (*access) *
				       (ci->accesscount - j));
			}
			else
			{
			    ++j;
			    ++access;
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
		    if (ci->akickcount !=
			fread (akick, sizeof (AutoKick), ci->akickcount, f))
			wxLogFatal ("Read error on %s", chanserv_db);
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
			wxLogFatal ("Read error on %s", chanserv_db);
		    if (n_entries <= CA_SIZE)
		    {
			fread (ci->cmd_access, sizeof (short), n_entries, f);
		    }
		    else
		    {
			fread (ci->cmd_access, sizeof (short), CA_SIZE, f);
			fseek (f, sizeof (short) * (n_entries - CA_SIZE),
			       SEEK_CUR);
		    }
		}
		free (old_ci);

		chan = CreateChanEntry(ci);
		Parent->chanserv.stored[chan.Name().LowerCase()] = chan;
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
		if (1 != fread (old_ci, sizeof (ChanInfo_V1), 1, f))
		    wxLogFatal ("Read error on %s", chanserv_db);
		/* Convert old dbase! */
		strcpy(ci->mlock_on, oldmodeconv(old_ci->mlock_on));
		strcpy(ci->mlock_off, oldmodeconv(old_ci->mlock_off));
		strcpy(ci->name, old_ci->name);
		strcpy(ci->founder, old_ci->founder);
		strcpy(ci->founderpass, old_ci->founderpass);
		strcpy(ci->last_topic_setter, old_ci->last_topic_setter);
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
		    ChanAccess *access;
		    access = (ChanAccess *) malloc (sizeof (ChanAccess) * ci->accesscount);
		    ci->access = access;
		    if (ci->accesscount != fread (access, sizeof (ChanAccess),
						  ci->accesscount, f))
			wxLogFatal ("Read error on %s", chanserv_db);
		    for (j = 0; j < ci->accesscount; ++j, ++access)
			access->name = read_string (f, chanserv_db);
		    j = 0;
		    access = ci->access;
		    /* Clear out unused entries */
		    while (j < ci->accesscount)
		    {
			if (access->is_nick < 0)
			{
			    --ci->accesscount;
			    free (access->name);
			    if (j < ci->accesscount)
				memcpy (access, access + 1, sizeof (*access) *
				       (ci->accesscount - j));
			}
			else
			{
			    ++j;
			    ++access;
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
		    if (ci->akickcount !=
			fread (akick, sizeof (AutoKick), ci->akickcount, f))
			wxLogFatal ("Read error on %s", chanserv_db);
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
			wxLogFatal ("Read error on %s", chanserv_db);
		    if (n_entries <= CA_SIZE)
		    {
			fread (ci->cmd_access, sizeof (short), n_entries, f);
		    }
		    else
		    {
			fread (ci->cmd_access, sizeof (short), CA_SIZE, f);
			fseek (f, sizeof (short) * (n_entries - CA_SIZE),
			       SEEK_CUR);
		    }
		}
		free (old_ci);

		chan = CreateChanEntry(ci);
		Parent->chanserv.stored[chan.Name().LowerCase()] = chan;
		delchan(ci);
	    }
	break;			/* case 1, etc. */
	}
    default:
	wxLogFatal ("Unsupported version number (%d) on %s", i, chanserv_db);
    }				/* switch (version) */
    fclose (f);
}


static char *
oldmodeconv (short inmode)
{
    static char outmode[MODEMAX];
    strcpy (outmode, "");
    if (inmode & 0x01) strcat(outmode, "i");
    if (inmode & 0x02) strcat(outmode, "m");
    if (inmode & 0x04) strcat(outmode, "n");
    if (inmode & 0x08) strcat(outmode, "p");
    if (inmode & 0x10) strcat(outmode, "s");
    if (inmode & 0x20) strcat(outmode, "t");
    if (inmode & 0x40) strcat(outmode, "k");
    if (inmode & 0x80) strcat(outmode, "l");
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
CreateChanEntry(ChanInfo *ci)
{
    /* ChanInfo *next, *prev;
    char name[CHANMAX];
    char founder[NICKMAX];
    char founderpass[PASSMAX];
    char *desc;
    char *url;
    time_t time_registered;
    time_t last_used;
    long accesscount;
    ChanAccess *access;
    long akickcount;
    AutoKick *akick;
    char mlock_on[64], mlock_off[64];
    long mlock_limit;
    char *mlock_key;
    char *last_topic;
    char last_topic_setter[NICKMAX];
    time_t last_topic_time;
    long flags;
    short *cmd_access;
    long reserved[3]; */

    if (ci->flags & CI_VERBOTEN)
    {
	Chan_Stored_t out(mstring(ci->name));
	return out;
    }
    else
    {
	ChanAccess *access;
	AutoKick *akick;
	int i;

	Chan_Stored_t out(mstring(ci->name), mstring(ci->founder),
		    mstring(ci->founderpass), mstring(ci->desc));

	if (ci->url != NULL)
	    out.i_URL = mstring(ci->url);
	out.i_RegTime = mDateTime(ci->time_registered);
	out.i_LastUsed = mDateTime(ci->last_used);
	mstring modelock;
	if (strlen(ci->mlock_on) || ci->mlock_key != NULL || ci->mlock_limit)
	{
	    modelock << "+" << ci->mlock_on << 
		    (ci->mlock_key != NULL ? "k" : "") <<
		    (ci->mlock_limit ? "l" : "");
	}
	if (strlen(ci->mlock_off))
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
	if (modelock.Len())
	    out.Mlock(Parent->chanserv.FirstName(), modelock);
	for (i=0, access = ci->access; i<ci->accesscount; ++i, ++access)
	{
	    if (access->is_nick > 0)
	    {
		out.Access_insert(access->name, access->level,
			Parent->chanserv.FirstName());
	    }
	}
	for (i=0, akick = ci->akick; i<ci->akickcount; ++i, ++akick)
	{
	    if (akick->reason != NULL)
		out.Akick_insert(akick->name, akick->reason,
			Parent->chanserv.FirstName());
	    else
		out.Akick_insert(akick->name,
			Parent->chanserv.FirstName());
	}

	if (ci->flags & CI_KEEPTOPIC && !out.L_Keeptopic())
	    out.i_Keeptopic = true;
	if (ci->flags & CI_SECUREOPS && !out.L_Secureops())
	    out.i_Secureops = true;
	if (ci->flags & CI_PRIVATE && !out.L_Private())
	    out.i_Private = true;
	if (ci->flags & CI_TOPICLOCK && !out.L_Topiclock())
	    out.i_Topiclock = true;
	if (ci->flags & CI_RESTRICTED && !out.L_Restricted())
	    out.i_Restricted = true;
	if (ci->flags & CI_LEAVEOPS && !out.L_Anarchy())
	    out.i_Anarchy = true;
	if (ci->flags & CI_SECURE && !out.L_Secure())
	    out.i_Secure = true;
	if (ci->flags & CI_JOIN && !out.L_Join())
	    out.i_Join = true;
	if (!out.L_Revenge())
	{
	    char revlevel = ((ci->flags & CI_REV3) << 2) |
			((ci->flags & CI_REV2) << 1) |
			(ci->flags & CI_REV1);
	    switch (revlevel)
	    {
	    case CR_REVERSE:
		out.i_Revenge = "REVERSE";
		break;
	    case CR_DEOP:
		out.i_Revenge = "DEOP";
		break;
	    case CR_KICK:
		out.i_Revenge = "KICK";
		break;
	    case CR_NICKBAN:
		out.i_Revenge = "NICKBAN";
		break;
	    case CR_USERBAN:
		out.i_Revenge = "USERBAN";
		break;
	    case CR_HOSTBAN:
		out.i_Revenge = "HOSTBAN";
		break;
	    case CR_MIRROR:
		out.i_Revenge = "MIRROR";
		break;
	    default:
		out.i_Revenge = "NONE";
	    }
	}

	if (ci->flags & CI_SUSPENDED)
	{
	    if (ci->last_topic != NULL)
		out.i_Comment = mstring(ci->last_topic);
	    if (ci->last_topic_setter != NULL)
		out.i_Suspend_By = mstring(ci->last_topic_setter);
	    out.i_Suspend_Time = mDateTime(ci->last_topic_time);
	}
	else
	{
	    if (ci->last_topic != NULL)
		out.i_Topic = mstring(ci->last_topic);
	    if (ci->last_topic_setter != NULL)
		out.i_Topic_Setter = mstring(ci->last_topic_setter);
	    out.i_Topic_Set_Time = mDateTime(ci->last_topic_time);
	}
/*
	if (ci->cmd_access != NULL)
	{
	    for (i=0; i<CA_SIZE; ++i)
	    {
		switch (i)
		{
		case CA_AUTODEOP:
		    out.Level_change("AUTODEOP", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_AUTOVOICE:
		    out.Level_change("AUTOVOICE", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_AUTOOP:
		    out.Level_change("AUTOOP", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_READMEMO:
		    out.Level_change("READMEMO", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_WRITEMEMO:
		    out.Level_change("WRITEMEMO", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_DELMEMO:
		    out.Level_change("DELMEMO", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_AKICK:
		    out.Level_change("AKICK", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_STARAKICK:
		    out.Level_change("SUPER", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    out.Level_change("OVERGREET", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_UNBAN:
		    out.Level_change("UNBAN", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_ACCESS:
		    out.Level_change("ACCESS", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_SET:
		    out.Level_change("SET", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_CMDINVITE:
		    out.Level_change("CMDINVITE", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_CMDUNBAN:
		    out.Level_change("CMDUNBAN", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_CMDVOICE:
		    out.Level_change("CMDVOICE", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_CMDOP:
		    out.Level_change("CMDOP", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		case CA_CMDCLEAR:
		    out.Level_change("CMDCLEAR", ci->cmd_access[i],
			Parent->chanserv.FirstName());
		    break;
		}
	    }
	}
*/
	return out;
    }
}

void
load_ms_dbase (void)
{
    FILE *f = fopen (memoserv_db, "r");
    int i, j;
    MemoList *ml;
    Memo *memos;
    list<Memo_t> memo;

    if (!f)
    {
	wxLogError ("Can't read MemoServ database %s", memoserv_db);
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
		if (1 != fread (ml, sizeof (MemoList), 1, f))
		    wxLogFatal ("Read error on %s", memoserv_db);
		ml->memos = memos = (Memo *) malloc (sizeof (Memo) * ml->n_memos);
		fread (memos, sizeof (Memo), ml->n_memos, f);
		for (j = 0; j < ml->n_memos; ++j, ++memos)
		    memos->text = read_string (f, memoserv_db);

		memo = CreateMemoEntry(ml);
		if (memo.size())
		    Parent->memoserv.nick[mstring(ml->nick).LowerCase()] = memo;
		del_memolist(ml);
	    }
	break;
    default:
	wxLogFatal ("Unsupported version number (%d) on %s", i, memoserv_db);
    }				/* switch (version) */
    fclose (f);
}



void
load_news_dbase (void)
{
    FILE *f = fopen (newsserv_db, "r");
    int i, j;
    NewsList *nl;
    Memo *newss;
    list<News_t> news;

    if (!f)
    {
	wxLogError ("Can't read NewsServ database %s", newsserv_db);
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
		if (1 != fread (nl, sizeof (NewsList), 1, f))
		    wxLogFatal ("Read error on %s", newsserv_db);
		nl->newss = newss = (Memo *) malloc (sizeof (Memo) * nl->n_newss);
		fread (newss, sizeof (Memo), nl->n_newss, f);
		for (j = 0; j < nl->n_newss; ++j, ++newss)
		    newss->text = read_string (f, newsserv_db);

		news = CreateNewsEntry(nl);
		if (news.size())
		    Parent->memoserv.channel[mstring(nl->chan).LowerCase()] = news;
		del_newslist(nl);
	    }
	}
	break;
    default:
	wxLogFatal ("Unsupported version number (%d) on %s", i, newsserv_db);
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

list<Memo_t>
CreateMemoEntry(MemoList *ml)
{
/*  char sender[NICKMAX];
    long number;
    time_t time;
    char *text;
    long reserved[4];

    MemoList *next, *prev;
    char nick[NICKMAX];
    long n_memos;
    Memo *memos;
    long reserved[4]; */

    int i;
    list<Memo_t> out;
    Memo_t *tmp;
    Memo *memos;

    memos = ml->memos;
    for (i = 0; i < ml->n_memos; ++i, ++memos)
    {
	tmp = new Memo_t(mstring(ml->nick), mstring(memos->sender),
		    mstring(memos->text));
	tmp->i_Time = mDateTime(memos->time);
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
}

list<News_t>
CreateNewsEntry(NewsList *nl)
{
/*  char sender[NICKMAX];
    long number;
    time_t time;
    char *text;
    long reserved[4];

    NewsList *next, *prev;
    char chan[CHANMAX];
    long n_newss;
    Memo *newss;
    long reserved[4]; */

    int i;
    list<News_t> out;
    News_t *tmp;
    Memo *newss;

    newss = nl->newss;
    for (i = 0; i < nl->n_newss; ++i, ++newss)
    {
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
    FILE *f = fopen (sop_db, "r");
    int i, j;
    int nsop = 0;
    int sop_size = 0;
    Sop *sops = NULL;

    if (!f)
    {
	wxLogError ("Can't read SOP database %s", sop_db);
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
	if (!nsop)
	{
	    fclose (f);
	    return;
	}
	if (nsop != fread (sops, sizeof (Sop), nsop, f))
	    wxLogFatal ("Read error on %s", sop_db);

	if (Parent->commserv.IsList(Parent->commserv.SOP_Name()))
	{
	    for (j=0; j<nsop; ++j)
	    {
		Parent->commserv.list[Parent->commserv.SOP_Name()].insert(
		    mstring(sops[i].nick), Parent->commserv.FirstName());
	    }
	}
	free(sops);
	break;
    default:
	wxLogFatal ("Unsupported version (%d) on %s", i, sop_db);
    }				/* switch (version) */
    fclose (f);
}

void
load_message ()
{
    FILE *f = fopen (message_db, "r");
    int i, j;
    int nmessage = 0;
    int message_size = 0;
    Message *messages = NULL;

    if (!f)
    {
	wxLogError ("Can't read MESSAGE database %s", message_db);
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
	messages = (Message *) malloc (sizeof (*messages) * message_size);
	if (!nmessage)
	{
	    fclose (f);
	    return;
	}
	if (nmessage != fread (messages, sizeof (*messages), nmessage, f))
	    wxLogFatal ("Read error on %s", message_db);
	for (j = 0; j < nmessage; ++j)
	    messages[j].text = read_string (f, message_db);

	for (j = 0; j < nmessage; ++j)
	{
	    if (messages[j].type == M_LOGON &&
		Parent->commserv.IsList(Parent->commserv.ALL_Name()))
	    {
		Parent->commserv.list[Parent->commserv.ALL_Name()].MSG_insert(
			mstring(messages[j].text), mstring(messages[j].who));
	    }
	    else if (messages[j].type == M_OPER &&
		Parent->commserv.IsList(Parent->commserv.OPER_Name()))
	    {
		Parent->commserv.list[Parent->commserv.OPER_Name()].MSG_insert(
			mstring(messages[j].text), mstring(messages[j].who));
	    }
	}

	for (j = 0; j < nmessage; ++j)
	    free(messages[j].text);
	free(messages);
	break;

    default:
	wxLogFatal ("Unsupported version (%d) on %s", i, message_db);
    }				/* switch (version) */
    fclose (f);
}

void
load_akill ()
{
    FILE *f = fopen (akill_db, "r");
    int i, j;
    int nakill = 0;
    int akill_size = 0;
    Akill *akills = NULL;

    if (!f)
    {
	wxLogError ("Can't read AKILL database %s", akill_db);
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
	akills = (Akill *) malloc (sizeof (*akills) * akill_size);
	if (!nakill)
	{
	    fclose (f);
	    return;
	}
	if (nakill != fread (akills, sizeof (*akills), nakill, f))
	    wxLogFatal ("Read error on %s", akill_db);
	for (j = 0; j < nakill; ++j)
	{
	    akills[j].mask = read_string (f, akill_db);
	    akills[j].reason = read_string (f, akill_db);
	}

/*  char *mask;
    char *reason;
    char who[NICKMAX];
    time_t time; */

	for (j = 0; j < nakill; ++j)
	{
	    if (akills[j].time == 0)
	    {
		Parent->operserv.Akill_insert(mstring(akills[j].mask),
		    Parent->operserv.Expire_Sop(),
		    mstring(akills[j].reason), mstring(akills[j].who));
	    }
	    else
	    {
		Parent->operserv.Akill_insert(mstring(akills[j].mask),
		    Parent->operserv.Def_Expire(),
		    mstring(akills[j].reason), mstring(akills[j].who),
		    mDateTime(akills[j].time));
	    }

	    free(akills[j].mask);
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
	    akills = (Akill *) malloc (sizeof (*akills) * akill_size);
	    if (!nakill)
	    {
		fclose (f);
		return;
	    }
	    for (j = 0; j < nakill; ++j)
	    {
		if (1 != fread (&old_akill, sizeof (old_akill), 1, f))
		    wxLogFatal ("Read error on %s", akill_db);
		akills[j].time = old_akill.time;
		akills[j].who[0] = 0;
	    }
	    for (j = 0; j < nakill; ++j)
	    {
		akills[j].mask = read_string (f, akill_db);
		akills[j].reason = read_string (f, akill_db);
	    }
	}			/* case 1 */

/*  char *mask;
    char *reason;
    char who[NICKMAX];
    time_t time; */

	for (j = 0; j < nakill; ++j)
	{
	    if (akills[j].time == 0)
	    {
		Parent->operserv.Akill_insert(mstring(akills[i].mask),
		    Parent->operserv.Expire_Sop(),
		    mstring(akills[j].reason), mstring(akills[j].who));
	    }
	    else
	    {
		Parent->operserv.Akill_insert(mstring(akills[i].mask),
		    Parent->operserv.Def_Expire(),
		    mstring(akills[j].reason), mstring(akills[j].who),
		    mDateTime(akills[j].time));
	    }

	    free(akills[j].mask);
	    free(akills[j].reason);
	}
	free(akills);

	break;

    default:
	wxLogFatal ("Unsupported version (%d) on %s", i, akill_db);
    }				/* switch (version) */
    fclose (f);
}

void
load_clone ()
{
    FILE *f = fopen (clone_db, "r");
    int i, j;
    int nclone = 0;
    int clone_size = 0;
    Allow *clones = NULL;

    if (!f)
    {
	wxLogError ("Can't read CLONE database %s", clone_db);
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
	clones = (Allow *) malloc (sizeof (*clones) * clone_size);
	if (!nclone)
	{
	    fclose (f);
	    return;
	}
	if (nclone != fread (clones, sizeof (*clones), nclone, f))
	    wxLogFatal ("Read error on %s", clone_db);
	for (j = 0; j < nclone; ++j)
	{
	    clones[j].host = read_string (f, clone_db);
	    clones[j].reason = read_string (f, clone_db);
	}

/*  char *host;
    int amount;
    char *reason;
    char who[NICKMAX];
    time_t time; */

	for (j = 0; j < nclone; ++j)
	{
	    Parent->operserv.Clone_insert(mstring(clones[j].host),
		    clones[j].amount, mstring(clones[j].reason),
		    mstring(clones[j].who), mDateTime(clones[j].time));
	    free(clones[j].host);
	    free(clones[j].reason);
	}
	free(clones);
	break;
    default:
	wxLogFatal ("Unsupported version (%d) on %s", i, clone_db);
    }				/* switch (version) */
    fclose (f);
}
