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
** Revision 1.100  2000/09/07 08:13:17  prez
** Fixed some of the erronous messages (SVSHOST, SQLINE, etc).
** Also added CPU statistics and fixed problem with socket deletions.
**
** Revision 1.99  2000/09/06 12:10:14  prez
** OOps, fixed compile error with operserv
**
** Revision 1.98  2000/09/06 11:27:33  prez
** Finished the T_Modify / T_Changing traces, fixed a bug in clone
** adding (was adding clone liimt as the mask length), updated docos
** a little more, and added a response to SIGINT to signon clients.
**
** Revision 1.97  2000/09/02 07:20:46  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.96  2000/08/31 06:25:09  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.95  2000/08/28 10:51:39  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.94  2000/08/09 12:14:44  prez
** Ensured chanserv infinate loops wont occur, added 2 new cmdline
** paramaters, and added a manpage (you need to perl2pod it tho).
**
** Revision 1.93  2000/08/06 05:27:47  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.92  2000/08/02 20:08:58  prez
** Minor code cleanups, added ACE installation instructions, updated the
** suggestions file and stopped people doing a whole bunch of stuff to
** forbidden nicknames.
**
** Revision 1.91  2000/07/29 21:58:54  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.90  2000/07/21 00:18:50  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.89  2000/06/27 18:56:59  prez
** Added choosing of keys to configure, also created the keygen,
** and scrambler (so keys are not stored in clear text, even in
** the .h and binary files).  We should be set to do the decryption
** process now, as encryption (except for encryption of db's) is
** all done :)
**
** Revision 1.87  2000/06/21 09:00:06  prez
** Fixed bug in mFile
**
** Revision 1.86  2000/06/18 12:49:27  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.85  2000/06/12 06:07:51  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.84  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.83  2000/06/06 08:57:57  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.82  2000/05/27 15:10:12  prez
** Misc changes, mainly re-did the makefile system, makes more sense.
** Also added a config.h file.
**
** Revision 1.81  2000/05/27 07:06:03  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.80  2000/05/26 11:21:28  prez
** Implemented HTM (High Traffic Mode) -- Can be used at a later date.
**
** Revision 1.79  2000/05/25 08:16:39  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.78  2000/05/21 04:49:40  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.77  2000/05/17 07:47:59  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.76  2000/05/14 06:30:14  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.75  2000/05/14 04:02:54  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.74  2000/04/30 03:48:30  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.73  2000/04/18 10:20:27  prez
** Made helpfiles load on usage, like language files.
**
** Revision 1.72  2000/04/06 12:52:50  prez
** Various code changes, but mainly added AUTOMAKE/AUTOCONF files :)
**
** Revision 1.71  2000/04/04 03:21:35  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.70  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.69  2000/03/28 09:42:11  prez
** Changed CommServ, ADD/DEL/LIST -> MEMBER ADD/DEL/LIST
** and NEW/KILL -> ADD/DEL and created a new LIST
**
** Revision 1.68  2000/03/27 21:26:13  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.67  2000/03/26 14:59:37  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.66  2000/03/15 15:02:47  prez
** Added checking for current ops matching an OPERDENY mask
**
** Revision 1.65  2000/03/15 14:42:59  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.64  2000/03/14 13:36:46  prez
** Finished P12 compliance (SJOIN, etc).
**
** Revision 1.63  2000/03/14 10:05:17  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.62  2000/03/08 23:38:37  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.61  2000/03/07 09:53:21  prez
** More helpfile updates (and associated updates).
**
** Revision 1.60  2000/03/02 11:59:45  prez
** More helpfile updates (slowly but surely writing it)
**
** Revision 1.59  2000/03/02 07:25:11  prez
** Added stuff to do the chanserv greet timings (ie. only greet if a user has
** been OUT of channel over 'x' seconds).  New stored chanserv cfg item.
**
** Revision 1.58  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.57  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.56  2000/02/17 12:55:07  ungod
** still working on borlandization
**
** Revision 1.55  2000/02/16 12:59:40  ungod
** fixing for borland compilability
**
** Revision 1.54  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.53  2000/02/15 10:37:50  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#include "lockable.h"
#include "operserv.h"
#include "magick.h"

bool OperServ::AddHost(mstring host)
{
    FT("OperServ::AddHost", (host));
    bool retval = false;

    WLOCK(("OperServ", "CloneList"));
    MCB(CloneList.size());
    if (CloneList.find(host.LowerCase()) == CloneList.end())
	CloneList[host.LowerCase()].first = 0;
    CloneList[host.LowerCase()].first++;

    CP(("Finding clone list, %s = %d", host.c_str(),
		CloneList[host.LowerCase()].first));
    { MLOCK2(("OperServ","Clone"));
    if (Clone_find(host))
    {
	if (CloneList[host.LowerCase()].first > Clone->Value().first)
	{
	    // Get rid of entries from the beginning...
	    while (CloneList[host.LowerCase()].second.size() &&
	    		CloneList[host.LowerCase()].second.begin()->SecondsSince() >
			Parent->operserv.Clone_Time())
		CloneList[host.LowerCase()].second.erase(CloneList[host.LowerCase()].second.begin());
	    CP(("Event Size after purge is %d",
				CloneList[host.LowerCase()].second.size()));

	    CloneList[host.LowerCase()].second.push_back(Now());
	    if (CloneList[host.LowerCase()].second.size() >
			Parent->operserv.Clone_Trigger())
	    {
		CP(("Reached MAX clone kills, adding AKILL ..."));
		MLOCK(("OperServ", "Akill"));
		if (Akill_find("*@" + host))
		    Akill_erase();
		Akill_insert("*@" + host, Parent->operserv.Clone_AkillTime(),
			Parent->operserv.Clone_Akill(), FirstName());
		announce(FirstName(), Parent->getMessage("MISC/AKILL_ADD"),
			FirstName().c_str(), host.c_str(),
			ToHumanTime(Parent->operserv.Clone_AkillTime()).c_str(),
			Parent->operserv.Clone_Akill().c_str());
		Log(LM_INFO, Parent->getLogMessage("OPERSERV/AKILL_ADD"),
			FirstName().c_str(), host.c_str(),
			ToHumanTime(Parent->operserv.Clone_AkillTime()).c_str(),
			Parent->operserv.Clone_Akill().c_str());
	    }
	    retval = true;
	}
    }
    else
    {
	if (CloneList[host.LowerCase()].first > Clone_Limit())
	{
	    // Get rid of entries from the beginning...
	    while (CloneList[host.LowerCase()].second.size() &&
	    		CloneList[host.LowerCase()].second.begin()->SecondsSince() >
			Parent->operserv.Clone_Time())
		CloneList[host.LowerCase()].second.erase(CloneList[host.LowerCase()].second.begin());
	    CP(("Event Size after purge is %d",
				CloneList[host.LowerCase()].second.size()));

	    CloneList[host.LowerCase()].second.push_back(Now());
	    if (CloneList[host.LowerCase()].second.size() >
			Parent->operserv.Clone_Trigger())
	    {
		CP(("Reached MAX clone kills, adding AKILL ..."));
		MLOCK(("OperServ", "Akill"));
		if (Akill_find("*@" + host))
		    Akill_erase();
		Akill_insert("*@" + host, Parent->operserv.Clone_AkillTime(),
			Parent->operserv.Clone_Akill(), FirstName());
		announce(FirstName(), Parent->getMessage("MISC/AKILL_ADD"),
			FirstName().c_str(), host.c_str(),
			ToHumanTime(Parent->operserv.Clone_AkillTime()).c_str(),
			Parent->operserv.Clone_Akill().c_str());
		Log(LM_INFO, Parent->getLogMessage("OPERSERV/AKILL_ADD"),
			FirstName().c_str(), host.c_str(),
			ToHumanTime(Parent->operserv.Clone_AkillTime()).c_str(),
			Parent->operserv.Clone_Akill().c_str());
	    }
	    retval = true;
	}
    }}

    MCE(CloneList.size());
    RET(retval);
}

void OperServ::RemHost(mstring host)
{
    FT("OperServ::RemHost", (host));

    WLOCK(("OperServ", "CloneList"));
    MCB(CloneList.size());
    if (CloneList.find(host.LowerCase()) != CloneList.end())
    {
	if (CloneList[host.LowerCase()].first > 1)
	    CloneList[host.LowerCase()].first--;
	else
	    CloneList.erase(host.LowerCase());
    }
    MCE(CloneList.size());
}

size_t OperServ::CloneList_sum()
{
    NFT("OperServ::ClonesList_sum");

    map<mstring, pair<unsigned int, list<mDateTime> > >::iterator i;
    size_t value = 0;

    RLOCK(("OperServ", "CloneList"));
    for (i=CloneList.begin(); i!=CloneList.end(); i++)
    {
	value += i->second.first;
    }
    RET(value);
}

size_t OperServ::CloneList_size(unsigned int amt)
{
    FT("OperServ::CloneList_size", (amt));

    map<mstring, pair<unsigned int, list<mDateTime> > >::iterator i;
    size_t value = 0;

    RLOCK(("OperServ", "CloneList"));
    for (i=CloneList.begin(); i!=CloneList.end(); i++)
    {
	if (i->second.first == amt)
	    value++;
    }
    RET(value);
}

size_t OperServ::CloneList_Usage()
{
    size_t retval = 0;
    map<mstring, pair<unsigned int, list<mDateTime> > >::iterator i;
    WLOCK(("OperServ", "CloneList"));
    for (i=CloneList.begin(); i!=CloneList.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second.first);
	list<mDateTime>::iterator j;
	for (j=i->second.second.begin(); j!=i->second.second.end(); j++)
	    retval += sizeof(j->Internal());
    }
    return retval;
}


bool OperServ::Clone_insert(mstring entry, unsigned int value, mstring reason, mstring nick, mDateTime added)
{
    FT("OperServ::Clone_insert", (entry, value, reason, nick, added));

    // Wildcards but no @
    if (entry.Contains("@") || entry.Contains("!"))
    {
	    RET(false);
    }

    MLOCK(("OperServ", "Clone"));
    if (!Clone_find(entry))
    {
	pair<set<entlist_val_t<pair<unsigned int, mstring> > >::iterator,bool> tmp;
	MCB(i_Clone.size());
	tmp = i_Clone.insert(entlist_val_t<pair<unsigned int, mstring> >(
			entry, pair<unsigned int, mstring>(value, reason), nick, added));
	MCE(i_Clone.size());
	if (tmp.second)
	    Clone = tmp.first;
	else
	    Clone = i_Clone.end();
	RET(true);
    }
    else
    {
	Clone = i_Clone.end();
	RET(false);
    }
}


bool OperServ::Clone_erase()
{
    NFT("OperServ::Clone_erase");

    MLOCK(("OperServ", "Clone"));
    if (Clone != i_Clone.end())
    {
	MCB(i_Clone.size());
	i_Clone.erase(Clone);
	MCE(i_Clone.size());
	Clone = i_Clone.end();
	RET(true);
    }
    else
    {
	RET(false);
    }
}

size_t OperServ::Clone_Usage()
{
    size_t retval = 0;
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator i;
    MLOCK(("OperServ", "Clone"));
    entlist_val_t<pair<unsigned int, mstring> > *tmp;
    for (i=i_Clone.begin(); i!=i_Clone.end(); i++)
    {
	tmp = (entlist_val_t<pair<unsigned int, mstring> > *) &(*i);
	retval += tmp->Usage();
    }
    return retval;
}


bool OperServ::Clone_find(mstring entry)
{
    FT("OperServ::Clone_find", (entry));

//  entlist_val_ui<pair<int, mstring> > iter = i_Clone.end();
    MLOCK(("OperServ", "Clone"));
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator iter = i_Clone.end();
    if (!i_Clone.empty())
	for (iter=i_Clone.begin(); iter!=i_Clone.end(); iter++)
	    if (entry.LowerCase().Matches(iter->Entry().LowerCase()))
		break;

    if (iter != i_Clone.end())
    {
	Clone = iter;
	RET(true);
    }
    else
    {
	Clone = i_Clone.end();
	RET(false);
    }
}


pair<unsigned int,mstring> OperServ::Clone_value(mstring entry)
{
    FT("OperServ::Clone_value", (entry));

    pair<unsigned int,mstring> retval = pair<unsigned int,mstring>(0,"");
    MLOCK(("OperServ", "Clone"));
//  entlist_val_ui<pair<int, mstring> > iter = Clone;
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator iter = Clone;

    if (Clone_find(entry))
	retval=Clone->Value();
    Clone = iter;
    return retval;
}


bool OperServ::Akill_insert(mstring entry, unsigned long value, mstring reason, mstring nick, mDateTime added)
{
    FT("OperServ::Akill_insert", (entry, value, reason, nick, added));

    // no @
    if (!entry.Contains("@") || entry.Contains("!"))
    {
	    RET(false);
    }

    MLOCK(("OperServ", "Akill"));
    if (!Akill_find(entry))
    {
	pair<set<entlist_val_t<pair<unsigned long, mstring> > >::iterator, bool> tmp;
	MCB(i_Akill.size());
	tmp = i_Akill.insert(entlist_val_t<pair<unsigned long, mstring> >(
			entry, pair<unsigned long, mstring>(value, reason), nick, added));
	MCE(i_Akill.size());
	if (tmp.second)
	    Akill = tmp.first;
	else
	    Akill = i_Akill.end();
	RET(true);
    }
    else
    {
	Akill = i_Akill.end();
	RET(false);
    }
}


bool OperServ::Akill_erase()
{
    NFT("OperServ::Akill_erase");

    MLOCK(("OperServ", "Akill"));
    if (Akill != i_Akill.end())
    {
	MCB(i_Akill.size());
	i_Akill.erase(Akill);
	MCE(i_Akill.size());
	Akill = i_Akill.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


size_t OperServ::Akill_Usage()
{
    size_t retval = 0;
    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator i;
    MLOCK(("OperServ", "Akill"));
    entlist_val_t<pair<unsigned long, mstring> > *tmp;
    for (i=i_Akill.begin(); i!=i_Akill.end(); i++)
    {
	tmp = (entlist_val_t<pair<unsigned long, mstring> > *) &(*i);
	retval += tmp->Usage();
    }
    return retval;
}


bool OperServ::Akill_find(mstring entry)
{
    FT("OperServ::Akill_find", (entry));

    // no @
    if (!entry.Contains("@") || entry.Contains("!"))
    {
	    RET(false);
    }

//  entlist_val_ui<pair<long, mstring> > iter = i_Akill.end();
    MLOCK(("OperServ", "Akill"));
    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator iter = i_Akill.end();
    if (!i_Akill.empty())
	for (iter=i_Akill.begin(); iter!=i_Akill.end(); iter++)
	{
	    if (entry.LowerCase().Matches(iter->Entry().LowerCase()))
	    {
		break;
	    }
	}

    if (iter != i_Akill.end())
    {
	Akill = iter;
	RET(true);
    }
    else
    {
	Akill = i_Akill.end();
	RET(false);
    }
}


pair<unsigned long,mstring> OperServ::Akill_value(mstring entry)
{
    FT("OperServ::Akill_value", (entry));

    pair<unsigned long,mstring> retval = pair<unsigned long,mstring>(0,"");
    MLOCK(("OperServ", "Akill"));
//  entlist_val_ui<pair<long, mstring> > iter = Akill;
    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator iter = Akill;

    if (Akill_find(entry))
	retval=Akill->Value();
    Akill = iter;
    return retval;
}


bool OperServ::OperDeny_insert(mstring entry, mstring value, mstring nick)
{
    FT("OperServ::OperDeny_insert", (entry, value, nick));

    // no @
    if (!entry.Contains("@"))
    {
	    RET(false);
    }

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.Prepend("*!");

    MLOCK(("OperServ", "OperDeny"));
    if (!OperDeny_find(entry))
    {
	pair<set<entlist_val_t<mstring > >::iterator, bool> tmp;
	MCB(i_OperDeny.size());
	tmp = i_OperDeny.insert(entlist_val_t<mstring>(entry, value, nick));
	MCE(i_OperDeny.size());
	if (tmp.second)
	    OperDeny = tmp.first;
	else
	    OperDeny = i_OperDeny.end();
	RET(true);
    }
    else
    {
	OperDeny = i_OperDeny.end();
	RET(false);
    }
}


bool OperServ::OperDeny_erase()
{
    NFT("OperServ::OperDeny_erase");

    MLOCK(("OperServ", "OperDeny"));
    if (OperDeny != i_OperDeny.end())
    {
	MCB(i_OperDeny.size());
	i_OperDeny.erase(OperDeny);
	MCE(i_OperDeny.size());
	OperDeny = i_OperDeny.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


size_t OperServ::OperDeny_Usage()
{
    size_t retval = 0;
    set<entlist_val_t<mstring> >::iterator i;
    MLOCK(("OperServ", "OperDeny"));
    entlist_val_t<mstring> *tmp;
    for (i=i_OperDeny.begin(); i!=i_OperDeny.end(); i++)
    {
	tmp = (entlist_val_t<mstring> *) &(*i);
	retval += tmp->Usage();
    }
    return retval;
}


bool OperServ::OperDeny_find(mstring entry)
{
    FT("OperServ::OperDeny_find", (entry));

    // no @
    if (!entry.Contains("@"))
    {
	    RET(false);
    }

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.Prepend("*!");

    MLOCK(("OperServ", "OperDeny"));
//  entlist_val_ui<mstring> iter = i_OperDeny.end();
    set<entlist_val_t<mstring> >::iterator iter = i_OperDeny.end();
    if (!i_OperDeny.empty())
	for (iter=i_OperDeny.begin(); iter!=i_OperDeny.end(); iter++)
	    if (entry.LowerCase().Matches(iter->Entry().LowerCase()))
		break;

    if (iter != i_OperDeny.end())
    {
	OperDeny = iter;
	RET(true);
    }
    else
    {
	OperDeny = i_OperDeny.end();
	RET(false);
    }
}


mstring OperServ::OperDeny_value(mstring entry)
{
    FT("OperServ::OperDeny_value", (entry));

    mstring retval = mstring(0,"");
    MLOCK(("OperServ", "OperDeny"));
//  entlist_val_ui<mstring> iter = OperDeny;
    set<entlist_val_t<mstring> >::iterator iter = OperDeny;

    if (OperDeny_find(entry))
	retval=OperDeny->Value();
    OperDeny = iter;
    return retval;
}



bool OperServ::Ignore_insert(mstring entry, bool perm, mstring nick)
{
    FT("OperServ::Ignore_insert", (entry, perm, nick));

    // no @
    if (!entry.Contains("@"))
    {
	    RET(false);
    }

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.Prepend("*!");

    MLOCK(("OperServ", "Ignore"));
    if (!Ignore_find(entry))
    {
	pair<set<entlist_val_t<bool> >::iterator, bool> tmp;
	MCB(i_Ignore.size());
	tmp = i_Ignore.insert(entlist_val_t<bool>(entry, perm, nick));
	MCE(i_Ignore.size());
	if (tmp.second)
	    Ignore = tmp.first;
	else
	    Ignore = i_Ignore.end();
	RET(true);
    }
    else
    {
	Ignore = i_Ignore.end();
	RET(false);
    }
}


bool OperServ::Ignore_erase()
{
    NFT("OperServ::Ignore_erase");

    MLOCK(("OperServ", "Ignore"));
    if (Ignore != i_Ignore.end())
    {
	MCB(i_Ignore.size());
	i_Ignore.erase(Ignore);
	MCE(i_Ignore.size());
	Ignore = i_Ignore.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


size_t OperServ::Ignore_Usage()
{
    size_t retval = 0;
    set<entlist_val_t<bool> >::iterator i;
    MLOCK(("OperServ", "Ignore"));
    entlist_val_t<bool> *tmp;
    for (i=i_Ignore.begin(); i!=i_Ignore.end(); i++)
    {
	tmp = (entlist_val_t<bool> *) &(*i);
	retval += tmp->Usage();
    }
    return retval;
}


bool OperServ::Ignore_find(mstring entry)
{
    FT("OperServ::Ignore_find", (entry));

    // no @
    if (!entry.Contains("@"))
    {
	    RET(false);
    }

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.Prepend("*!");

    MLOCK(("OperServ", "Ignore"));
//  entlist_val_ui<pair<mDateTime, bool> > iter = i_Ignore.end();
    set<entlist_val_t<bool> >::iterator iter = i_Ignore.end();

    if (!i_Ignore.empty())
	for (iter=i_Ignore.begin(); iter!=i_Ignore.end(); iter++)
	{
	    if (entry.LowerCase().Matches(iter->Entry().LowerCase()))
		break;
	}

    if (iter != i_Ignore.end())
    {
	Ignore = iter;
	RET(true);
    }
    else
    {
	Ignore = i_Ignore.end();
	RET(false);
    }
}


bool OperServ::Ignore_value(mstring entry)
{
    FT("OperServ::Ignore_value", (entry));

    bool retval = false;
    MLOCK(("OperServ", "Ignore"));
//  entlist_val_ui<pair<mDateTime, bool> > iter = Ignore;
    set<entlist_val_t<bool> >::iterator iter = Ignore;

    if (Ignore_find(entry))
	retval=Ignore->Value();
    Ignore = iter;
    return retval;
}

OperServ::OperServ()
{
    NFT("OperServ::OperServ");
    messages = true;
}

void OperServ::AddCommands()
{
    NFT("OperServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "HELP", Parent->commserv.ALL_Name(), OperServ::do_Help);
#ifdef MAGICK_TRACE_WORKS
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "TRACE", Parent->commserv.SADMIN_Name(), OperServ::do_Trace);
#endif
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*MODE*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_Mode);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "Q*LINE*", Parent->commserv.ADMIN_Name(), OperServ::do_Qline);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNQ*LINE*", Parent->commserv.ADMIN_Name(), OperServ::do_UnQline);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "NO*OP*", Parent->commserv.ADMIN_Name(), OperServ::do_NOOP);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "KILL*", Parent->commserv.SOP_Name(), OperServ::do_Kill);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "HIDE*", Parent->commserv.ADMIN_Name(), OperServ::do_Hide);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*PING*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_Ping);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UPD*", Parent->commserv.SADMIN_Name(), OperServ::do_Update);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SHUT*DOWN*", Parent->commserv.SADMIN_Name(), OperServ::do_Shutdown);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "RELOAD*", Parent->commserv.SADMIN_Name(), OperServ::do_Reload);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SIGNON*", Parent->commserv.SADMIN_Name(), OperServ::do_Signon);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOAD*", Parent->commserv.SADMIN_Name(), OperServ::do_Unload);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "JUPE*", Parent->commserv.ADMIN_Name(), OperServ::do_Jupe);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ID*", Parent->commserv.SADMIN_Name(), NickServ::do_Identify);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ON", Parent->commserv.SADMIN_Name(), OperServ::do_On);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "OFF", Parent->commserv.SADMIN_Name(), OperServ::do_Off);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "HTM", Parent->commserv.SOP_Name(), OperServ::do_HTM);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* CONF*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_settings_Config);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* NICK*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_settings_Nick);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* CHAN*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_settings_Channel);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* OTH*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_settings_Other);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *ALL*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_settings_All);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* ADD*", Parent->commserv.SOP_Name(), OperServ::do_clone_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* DEL*", Parent->commserv.SOP_Name(), OperServ::do_clone_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* ERA*", Parent->commserv.SOP_Name(), OperServ::do_clone_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* LIST", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_clone_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* VIEW", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_clone_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL ADD*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_akill_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL DEL*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_akill_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL ERA*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_akill_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL LIST", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_akill_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL VIEW", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_akill_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* ADD*", Parent->commserv.SADMIN_Name(), OperServ::do_operdeny_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* DEL*", Parent->commserv.SADMIN_Name(), OperServ::do_operdeny_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* ERA*", Parent->commserv.SADMIN_Name(), OperServ::do_operdeny_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* LIST", Parent->commserv.SOP_Name(), OperServ::do_operdeny_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* VIEW", Parent->commserv.SOP_Name(), OperServ::do_operdeny_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* ADD*", Parent->commserv.SOP_Name(), OperServ::do_ignore_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* DEL*", Parent->commserv.SOP_Name(), OperServ::do_ignore_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* ERA*", Parent->commserv.SOP_Name(), OperServ::do_ignore_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* LIST", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_ignore_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* VIEW", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), OperServ::do_ignore_List);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), do_1_2param);
}

void OperServ::RemCommands()
{
    NFT("OperServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "HELP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "TRACE", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "*MODE*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "Q*LINE*", Parent->commserv.ADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNQ*LINE*", Parent->commserv.ADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "NO*OP*", Parent->commserv.ADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "KILL*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "HIDE*", Parent->commserv.ADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "*PING*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UPD*", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SHUT*DOWN*", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "RELOAD*", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOAD*", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "JUPE*", Parent->commserv.ADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ID*", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ON", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "OFF", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "HTM", Parent->commserv.SOP_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* CONF*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* NICK*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* CHAN*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* OTH*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* *ALL*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLONE* ADD*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLONE* DEL*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLONE* ERA*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLONE* LIST", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLONE* VIEW", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KILL ADD*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KILL DEL*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KILL ERA*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KILL LIST", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KILL VIEW", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "O*DENY* ADD*", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "O*DENY* DEL*", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "O*DENY* ERA*", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "O*DENY* LIST", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "O*DENY* VIEW", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "IGN* ADD*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "IGN* DEL*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "IGN* ERA*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "IGN* LIST", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "IGN* VIEW", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLONE* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLONE*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KILL* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KILL*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "O*DENY* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "O*DENY*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "IGN* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "IGN*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
}

void OperServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_OperServ);
    FT("OperServ::execute", (data));
    //okay this is the main operserv command switcher


    // Nick/Server PRIVMSG/NOTICE mynick :message

    mstring source, msgtype, mynick, message, command;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ").UpperCase();
    mynick  = Parent->getLname(data.ExtractWord(3, ": "));
    message = data.After(":", 2);
    command = message.ExtractWord(1, " ").UpperCase();

    // We SHOULD still process THESE messages, ONLY.
    if (!MSG())
    {
	if (!(command == "ON" || command == "OFF" ||
	      command.Matches(mstring("ID*"))))
	{
	    return;
	}
    }

    if (message[0U] == CTCP_DELIM_CHAR)
    {
	if (msgtype == "PRIVMSG")
	    DccEngine::decodeRequest(mynick, source, message);
	else
	    DccEngine::decodeReply(mynick, source, message);
    }
    else if (Secure() && !Parent->nickserv.live[source.LowerCase()].HasMode("o"))
    {
	send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
    }
    else if (msgtype == "PRIVMSG" &&
	!Parent->commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);
}

void OperServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    mstring HelpTopic = Parent->operserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.Replace(" ", "/");
    vector<mstring> help = Parent->getHelp(source, HelpTopic.UpperCase());
					
    unsigned int i;
    for (i=0; i<help.size(); i++)
	::send(mynick, source, help[i]);
}


#ifdef MAGICK_TRACE_WORKS
void OperServ::do_Trace(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Trace", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    unsigned int i, j, k;
    mstring output;
    mstring action  = params.ExtractWord(2, " ").UpperCase();
    mstring ttype   = params.ExtractWord(3, " ").UpperCase();
    threadtype_enum type;
    vector<mstring> levels;
    for (i=4; i < params.WordCount(" ")+1; i++)
	levels.push_back(params.ExtractWord(i, " ").UpperCase());

    if (ttype == "ALL")
    {
	type = tt_MAX;
	ttype = "ALL";
    }
    else if (ttype == "MAIN")
    {
	type = tt_MAIN;
	ttype = "MAIN";
    }
    else
    {
        for (i=tt_MAIN+1; i<tt_MAX; i++)
	    if (ttype == threadname[i])
		break;
	if (i<tt_MAX)
	{
	    type = (threadtype_enum) i;
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "OS_STATUS/INVALID_THREAD"),
 							ttype.c_str());
	    return;
	}
    }

    if (action == "SET")
    {
	if (makehex(levels[0U]) != 0 || levels[0U] == "0" || levels[0U] == "0x0000")
	{
	    if (type == tt_MAX)
	    {
		for (i=tt_MAIN; i<tt_MAX; i++)
		{
		    Trace::TurnSet((threadtype_enum) i, makehex(levels[0U]));
		    output.Format("%s SET: Trace level set to %#06x.",
			Now().DateTimeString().c_str(), Trace::TraceLevel((threadtype_enum) i));
		    { MLOCK(("ThreadMessageQueue"));
		    ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>((threadtype_enum) i, output));
		    }
		}
	    }
	    else
	    {
		Trace::TurnSet(type, makehex(levels[0U]));
		output.Format("%s SET: Trace level set to %#06x.",
		    Now().DateTimeString().c_str(), Trace::TraceLevel(type));
		{ MLOCK(("ThreadMessageQueue"));
		ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>(type, output));
		}
	    }
	}
	else
	{
	    bool gotone = false;
	    for (i=0; i<levels.size(); i++)
	    {
		for (j=0; j<Trace::levelname.size(); j++)
		{
		    if (levels[i].Matches(Trace::levelname[j].name.UpperCase()))
		    {
			if (type == tt_MAX)
			{
			    for (k=tt_MAIN; k<tt_MAX; k++)
			    {
				if (k==tt_MAIN)
				{
				    Trace::TurnSet((threadtype_enum) k, 0U);
				    gotone = true;
				}
				Trace::TurnUp((threadtype_enum) k, Trace::levelname[j].level);
				output.Format("%s SET: Trace level set to %#06x.",
				    Now().DateTimeString().c_str(), Trace::TraceLevel((threadtype_enum) k));
				{ MLOCK(("ThreadMessageQueue"));
				ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>((threadtype_enum) k, output));
				}
			    }
			}
			else
			{
				if (!gotone)
				{
				    Trace::TurnSet(type, 0U);
				    gotone = true;
				}
				Trace::TurnUp(type, Trace::levelname[j].level);
				output.Format("%s SET: Trace level set to %#06x.",
				    Now().DateTimeString().c_str(), Trace::TraceLevel(type));
				{ MLOCK(("ThreadMessageQueue"));
				ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>(type, output));
				}
			}
			break;
		    }
		}
		if (j>=Trace::levelname.size())
		    ::send(mynick, source, Parent->getMessage(source, "OS_STATUS/INVALID_LEVEL"),
								levels[i].c_str());
	    }
	}
    }
    else if (action == "UP")
    {
	for (i=0; i<levels.size(); i++)
	{
	    for (j=0; j<Trace::levelname.size(); j++)
	    {
		if (levels[i].Matches(Trace::levelname[j].name.UpperCase()))
		{
		    if (type == tt_MAX)
		    {
			for (k=tt_MAIN; k<tt_MAX; k++)
			{
			    Trace::TurnUp((threadtype_enum) k, Trace::levelname[j].level);
			    output.Format("%s UP: Trace level set to %#06x.",
				Now().DateTimeString().c_str(), Trace::TraceLevel((threadtype_enum) k));
			    { MLOCK(("ThreadMessageQueue"));
			    ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>((threadtype_enum) k, output));
			    }
			}
		    }
		    else
		    {
			Trace::TurnUp(type, Trace::levelname[j].level);
			output.Format("%s UP: Trace level set to %#06x.",
			    Now().DateTimeString().c_str(), Trace::TraceLevel(type));
			{ MLOCK(("ThreadMessageQueue"));
			ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>(type, output));
			}
		    }
		    break;
		}
	    }
	    if (j>=Trace::levelname.size())
		::send(mynick, source, Parent->getMessage(source, "OS_STATUS/INVALID_LEVEL"),
								levels[i].c_str());
	}
    }
    else if (action == "DOWN")
    {
	for (i=0; i<levels.size(); i++)
	{
	    for (j=0; j<Trace::levelname.size(); j++)
	    {
		if (levels[i].Matches(Trace::levelname[j].name.UpperCase()))
		{
		    if (type == tt_MAX)
		    {
			for (k=tt_MAIN; k<tt_MAX; k++)
			{
			    Trace::TurnDown((threadtype_enum) k, Trace::levelname[j].level);
			    output.Format("%s DOWN: Trace level set to %#06x.",
				Now().DateTimeString().c_str(), Trace::TraceLevel((threadtype_enum) k));
			    { MLOCK(("ThreadMessageQueue"));
			    ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>((threadtype_enum) k, output));
			    }
			}
		    }
		    else
		    {
			Trace::TurnDown(type, Trace::levelname[j].level);
			output.Format("%s DOWN: Trace level set to %#06x.",
			    Now().DateTimeString().c_str(), Trace::TraceLevel(type));
			{ MLOCK(("ThreadMessageQueue"));
			ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>(type, output));
			}
		    }
		    break;
		}
	    }
	    if (j>=Trace::levelname.size())
		::send(mynick, source, Parent->getMessage(source, "OS_STATUS/INVALID_LEVEL"),
								levels[i].c_str());
	}
    }
    else if (action == "VIEW" || action == "LIST")
    {
	// Its taken care of below :)
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/UNKNOWN_OPTION"),
			(message + " " + action).c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring line1, line2, tmp;
    for (i=tt_MAIN; i<tt_MAX; i++)
    {
	tmp.Format("%6s  ", (i == tt_MAIN) ? "MAIN" : threadname[i].c_str());
	line1 += tmp;
	tmp.Format("%#06x  ", Trace::TraceLevel((threadtype_enum) i));
	line2 += tmp;
    }
    Parent->operserv.stats.i_Trace++;
    ::send(mynick, source, line1);
    ::send(mynick, source, line2);
    if (!(action == "VIEW" || action == "LIST"))
    {
	Log(LM_NOTICE, line1);
	Log(LM_NOTICE, line2);
    }
}
#endif


void OperServ::do_Mode(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Mode", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring mode = params.After(" ", 2);

    if (IsChan(target))
    {
	if (Parent->chanserv.IsLive(target))
	{
	    Parent->server.MODE(mynick, target, mode);
	    Parent->operserv.stats.i_Mode++;
	    announce(mynick, Parent->getMessage("MISC/CHAN_MODE"),
			source.c_str(), mode.c_str(), target.c_str());
	    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/CHAN_MODE"),
			mode.c_str(), target.c_str());

	    Log(LM_INFO, Parent->getLogMessage("OPERSERV/MODE"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		mode.c_str(), target.c_str());
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
					    target.c_str());
	}
    }
    else
    {
	if (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	    Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))
	{
	    if (Parent->nickserv.IsLive(target))
	    {
		if (Parent->server.proto.SVS())
		{
		    Parent->server.SVSMODE(mynick, target, mode);
		    Parent->operserv.stats.i_Mode++;
		    announce(mynick, Parent->getMessage("MISC/NICK_MODE"),
			source.c_str(), mode.c_str(), target.c_str());
		    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/NICK_MODE"),
			mode.c_str(), target.c_str());
		    Log(LM_INFO, Parent->getLogMessage("OPERSERV/MODE"),
			Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
			mode.c_str(), target.c_str());
		}
		else
		{
		    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOT_SUPPORTED"));
		}
	    }
	    else
	    {
		::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
					    target.c_str());
	    }
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	}
    }
}


void OperServ::do_Qline(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Qline", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (!Parent->server.proto.SVS())
    {
	::send(mynick, source, Parent->getMessage("ERR_SITUATION/NOT_SUPPORTED"));
	return;
    }

    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target  = params.ExtractWord(2, " ");
    mstring reason;
    if (params.WordCount(" ") > 2)
	reason = params.After(" ", 2);
    Parent->server.SQLINE(mynick, target, reason);
    Parent->operserv.stats.i_Qline++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/QLINE"),
		target.c_str(), Parent->getMessage(source, "MISC/ON").c_str());
    announce(mynick, Parent->getMessage("MISC/QLINE"),
		source.c_str(), Parent->getMessage("MISC/ON").c_str(),
		target.c_str());
    Log(LM_INFO, Parent->getLogMessage("OPERSERV/QLINE"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str());
}


void OperServ::do_UnQline(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_UnQline", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (!Parent->server.proto.SVS())
    {
	::send(mynick, source, Parent->getMessage("ERR_SITUATION/NOT_SUPPORTED"));
	return;
    }

    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target  = params.ExtractWord(2, " ");
    Parent->server.UNSQLINE(mynick, target);
    Parent->operserv.stats.i_Unqline++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/QLINE"),
		target.c_str(), Parent->getMessage(source, "MISC/OFF").c_str());
    announce(mynick, Parent->getMessage("MISC/QLINE"),
		source.c_str(), Parent->getMessage("MISC/OFF").c_str(),
		target.c_str());
    Log(LM_INFO, Parent->getLogMessage("OPERSERV/UNQLINE"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str());
}


void OperServ::do_NOOP(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_NOOP", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (!Parent->server.proto.SVS())
    {
	::send(mynick, source, Parent->getMessage("ERR_SITUATION/NOT_SUPPORTED"));
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target  = params.ExtractWord(2, " ");
    mstring onoff   = params.ExtractWord(3, " ");

    if (!Parent->server.IsServer(target))
    {
	::send(mynick, source, Parent->getMessage(source, "OS_STATUS/ISNOTLINKED"),
			target.c_str());
	return;
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->server.NOOP(mynick, target, onoff.GetBool());
    Parent->operserv.stats.i_Noop++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/NOOP"),
	    onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str(),
	    target.c_str());
    announce(mynick, Parent->getMessage("MISC/NOOP"),
	    source.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str(),
	    target.c_str());
    Log(LM_INFO, Parent->getLogMessage("OPERSERV/NOOP"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
}


void OperServ::do_Kill(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Kill", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (!Parent->server.proto.SVS())
    {
	::send(mynick, source, Parent->getMessage("ERR_SITUATION/NOT_SUPPORTED"));
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring reason = params.After(" ", 2);

    if (Parent->nickserv.IsLive(target))
    {
	Parent->server.SVSKILL(mynick, target, reason);
	Parent->operserv.stats.i_Kill++;
	::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/KILL"),
		    target.c_str());
	announce(mynick, Parent->getMessage("MISC/KILL"),
		    source.c_str(), target.c_str());
	Log(LM_INFO, Parent->getLogMessage("OPERSERV/KILL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		target.c_str(), reason.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
		    target.c_str());
    }
}


void OperServ::do_Hide(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Hide", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (!Parent->server.proto.SVSHOST())
    {
	::send(mynick, source, Parent->getMessage("ERR_SITUATION/NOT_SUPPORTED"));
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target  = params.ExtractWord(2, " ");
    mstring newhost = params.ExtractWord(3, " ");

    if (Parent->nickserv.IsLive(target))
    {
	Parent->server.SVSHOST(mynick, target, newhost);
	Parent->operserv.stats.i_Hide++;
	::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/HIDE"),
		    target.c_str(), newhost.c_str());
	announce(mynick, Parent->getMessage("MISC/HIDE"),
		    source.c_str(), target.c_str(), newhost.c_str());
	Log(LM_INFO, Parent->getLogMessage("OPERSERV/HIDE"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		target.c_str(), newhost.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
		    target.c_str());
    }
}


void OperServ::do_Ping(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Ping", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    Parent->events->ForcePing();
    Parent->operserv.stats.i_Ping++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/PING"));
    Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/PING"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str());
}


void OperServ::do_Update(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Update", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();
    Parent->events->ForceSave();
    Parent->operserv.stats.i_Update++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/UPDATE"));
    Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/UPDATE"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str());
}


void OperServ::do_Shutdown(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Shutdown", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/SHUTDOWN"));
    announce(mynick, Parent->getMessage("MISC/SHUTDOWN"), source.c_str());
    Log(LM_CRITICAL, Parent->getLogMessage("OPERSERV/SHUTDOWN"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str());
    ACE_OS::sleep(1);
    Parent->Shutdown(true);
    Parent->Die();
}


void OperServ::do_Reload(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Reload", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    if (Parent->get_config_values())
    {
	Parent->operserv.stats.i_Reload++;
	::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/RELOAD"));
	announce(mynick, Parent->getMessage("MISC/RELOAD"), source.c_str());
	Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/RELOAD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str());
    }
    else
    {
	Log(LM_ERROR, Parent->getLogMessage("COMMANDLINE/NO_CFG_FILE"),
		Parent->Config_File().c_str());
	::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/RELOAD_FAIL"));
    }
}


void OperServ::do_Signon(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Reload", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    Parent->server.SignOnAll();
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/SIGNON"));
}


void OperServ::do_Unload(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Unload", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring language = params.ExtractWord(2, " ").UpperCase();

    bool unload1, unload2;
    unload1 = Parent->UnloadExternalMessages(language);
    unload2 = Parent->UnloadHelp(language);
    if (unload1 || unload2)
    {
	Parent->operserv.stats.i_Unload++;
	::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/UNLOAD"),
			language.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/UNLOAD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		language.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "OS_STATUS/ISNOTLANG"),
			language.c_str());
    }
}


void OperServ::do_Jupe(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Jupe", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring reason = params.After(" ", 2);

    Parent->server.Jupe(target, reason);
    Parent->operserv.stats.i_Jupe++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/JUPE"),
		target.c_str());
    announce(mynick, Parent->getMessage(source, "MISC/JUPE"),
		source.c_str(), target.c_str());
    Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/JUPE"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), reason.c_str());
}


void OperServ::do_On(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_On", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    // Later, make the ability to turn on/off specific services
    // also the ability to turn of either MSG, or AUTO or BOTH
    Parent->MSG(true);
    Parent->operserv.stats.i_OnOff++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/ONOFF"),
	    Parent->getMessage(source, "MISC/ON").c_str());
    announce(mynick, Parent->getMessage("MISC/ONOFF"),
	    Parent->getMessage("MISC/ON").c_str(), source.c_str());
    Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/ONOFF"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("MISC/ON").c_str());
}


void OperServ::do_Off(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Off", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    // Later, make the ability to turn on/off specific services
    // also the ability to turn of either MSG, or AUTO or BOTH
    Parent->MSG(false);
    Parent->operserv.stats.i_OnOff++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/ONOFF"),
	    Parent->getMessage(source, "MISC/OFF").c_str());
    announce(mynick, Parent->getMessage("MISC/ONOFF"),
	    Parent->getMessage("MISC/OFF").c_str(), source.c_str());
    Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/ONOFF"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("MISC/OFF").c_str());
}


void OperServ::do_HTM(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_HTM", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "OS_STATUS/HTM"),
		ToHumanSpace(Parent->ircsvchandler->HTM_Threshold()).c_str(),
		(float) Parent->ircsvchandler->Average(Parent->ircsvchandler->HTM_Gap()) / (float) 1024);
    }
    else
    {
	mstring command = params.ExtractWord(2, " ").UpperCase();
	if (command == "ON")
	{
	    Parent->ircsvchandler->HTM(true);
	    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/HTM"),
		Parent->getMessage(source, "MISC/ON").c_str());
	    announce(mynick, Parent->getMessage("MISC/HTM_ON_FORCE"),
		source.c_str());
	    Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/HTM_FORCE"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("MISC/ON").c_str());
	}
	else if (command == "OFF")
	{
	    Parent->ircsvchandler->HTM(false);
	    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/HTM"),
		Parent->getMessage(source, "MISC/OFF").c_str());
	    announce(mynick, Parent->getMessage("MISC/HTM_OFF_FORCE"),
		source.c_str());
	    Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/HTM_FORCE"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("MISC/OFF").c_str());
	}
	else if (command == "SET")
	{
	    if (params.WordCount(" ") < 3)
	    {
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				(message + " " + command).c_str(), mynick.c_str(), message.c_str());
		return;
	    }
	    size_t newsize = FromHumanSpace(params.ExtractWord(3, " "));
	    if (!newsize)
	    {
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBESIZE"),
				ToHumanSpace(1).c_str());
		return;
	    }
	    Parent->ircsvchandler->HTM_Threshold(newsize);

	    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/HTM_SET"),
		ToHumanSpace(newsize).c_str());
	    announce(mynick, Parent->getMessage("MISC/HTM_SET"),
		ToHumanSpace(newsize).c_str(), source.c_str());
	    Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/HTM_SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		ToHumanSpace(newsize).c_str());
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/UNKNOWN_OPTION"),
			(message + " " + command).c_str(), mynick.c_str(), message.c_str());
	    return;
	}
	
    }
}


void OperServ::do_settings_Config(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Config", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

/*
-   Databases are (not) encrypted, and compressed at level ?.
-   Minimum threads active is ?, Current threads active is ?.
-   New thread will spawn each ? messages, and die when below ?.
*/
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_LEVEL"),
		    Parent->startup.Level(), Parent->Level());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_LAG"),
		    ToHumanTime(Parent->startup.Lagtime()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_LAGCHECK"),
		    ToHumanTime(Parent->config.Ping_Frequency()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_SERVERS"),
		    Parent->startup.Server_size());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_RELINK"),
		    ToHumanTime(Parent->config.Server_Relink()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_SQUIT1"),
		    ToHumanTime(Parent->config.Squit_Protect()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_SQUIT2"),
		    ToHumanTime(Parent->config.Squit_Cancel()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_SYNC"),
		    ToHumanTime(Parent->config.Savetime()).c_str(),
		    Parent->events->SyncTime().c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_CYCLE"),
		    ToHumanTime(Parent->config.Cycletime()).c_str(),
		    ToHumanTime(Parent->config.Checktime()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_DCC1"),
		    ToHumanSpace(Parent->files.Blocksize()).c_str(),
		    ToHumanTime(Parent->files.Timeout()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_DCC2"),
		    ToHumanSpace(Parent->files.Min_Speed()).c_str(),
		    ToHumanSpace(Parent->files.Max_Speed()).c_str(),
		    ToHumanTime(Parent->files.Sampletime()).c_str());
}
    
void OperServ::do_settings_Nick(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Nick", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_EXPIRE"),
			ToHumanTime(Parent->nickserv.Expire()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_IDENT"),
			ToHumanTime(Parent->nickserv.Ident()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_HOLD"),
			ToHumanTime(Parent->nickserv.Release()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_PASS"),
			Parent->nickserv.Passfail());

    mstring output = "";
    if (Parent->nickserv.DEF_Protect())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_Protect())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "NS_SET/PROTECT");
	if (Parent->nickserv.LCK_Protect())
	    output << IRC_Off;
    }

    if (Parent->nickserv.DEF_Secure())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_Secure())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "NS_SET/SECURE");
	if (Parent->nickserv.LCK_Secure())
	    output << IRC_Off;
    }

    if (Parent->nickserv.DEF_NoExpire())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_NoExpire())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "NS_SET/NOEXPIRE");
	if (Parent->nickserv.LCK_NoExpire())
	    output << IRC_Off;
    }

    if (Parent->nickserv.DEF_NoMemo())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_NoMemo())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "NS_SET/NOMEMO");
	if (Parent->nickserv.LCK_NoMemo())
	    output << IRC_Off;
    }

    if (Parent->nickserv.DEF_Private())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_Private())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "NS_SET/PRIVATE");
	if (Parent->nickserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Parent->nickserv.DEF_PRIVMSG())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_PRIVMSG())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "NS_SET/PRIVMSG");
	if (Parent->nickserv.LCK_PRIVMSG())
	    output << IRC_Off;
    }

    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_OPTIONS"),
			output.c_str());

    output = "";
    if (Parent->nickserv.LCK_Language())
	output << IRC_Bold;
    output << Parent->nickserv.DEF_Language();
    if (Parent->nickserv.LCK_Language())
	output << IRC_Off;    

    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_LANG"),
			output.c_str());

    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_PICSIZE"),
		    ToHumanSpace(Parent->nickserv.PicSize()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_PICEXT"),
		    Parent->nickserv.PicExt().c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_FILES"),
		    Parent->memoserv.Files(),
		    ToHumanSpace(Parent->memoserv.FileSize()).c_str());
}


void OperServ::do_settings_Channel(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Channel", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CHAN_EXPIRE"),
		    ToHumanTime(Parent->chanserv.Expire()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CHAN_IDENT"),
		    Parent->chanserv.Passfail());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CHAN_KEEPTIME"),
		    ToHumanTime(Parent->chanserv.ChanKeep()).c_str());

    mstring output = "";
    if (Parent->chanserv.LCK_Bantime())
	output << IRC_Bold;
    output << ToHumanTime(Parent->chanserv.DEF_Bantime());
    if (Parent->chanserv.LCK_Bantime())
	output << IRC_Off;
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CHAN_BANTIME"),
		    output.c_str());
    output = "";
    if (Parent->chanserv.LCK_Parttime())
	output << IRC_Bold;
    output << ToHumanTime(Parent->chanserv.DEF_Parttime());
    if (Parent->chanserv.LCK_Parttime())
	output << IRC_Off;
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CHAN_PARTTIME"),
		    output.c_str());

    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CHAN_MLOCK"),
		    Parent->chanserv.DEF_MLock().c_str(),
		    Parent->chanserv.LCK_MLock().c_str());

    output = "";
    if (Parent->chanserv.DEF_Keeptopic())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Keeptopic())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/KEEPTOPIC");
	if (Parent->chanserv.LCK_Keeptopic())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Topiclock())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Topiclock())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/TOPICLOCK");
	if (Parent->chanserv.LCK_Topiclock())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Private())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Private())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/PRIVATE");
	if (Parent->chanserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Secureops())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Secureops())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/SECUREOPS");
	if (Parent->chanserv.LCK_Secureops())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Secure())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Secure())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/SECURE");
	if (Parent->chanserv.LCK_Secure())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_NoExpire())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_NoExpire())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/NOEXPIRE");
	if (Parent->chanserv.LCK_NoExpire())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Anarchy())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Anarchy())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/ANARCHY");
	if (Parent->chanserv.LCK_Anarchy())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_KickOnBan())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_KickOnBan())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/KICKONBAN");
	if (Parent->chanserv.LCK_KickOnBan())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Restricted())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Restricted())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/RESTRICTED");
	if (Parent->chanserv.LCK_Restricted())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Join())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Join())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/JOIN");
	if (Parent->chanserv.LCK_Join())
	    output << IRC_Off;
    }

    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CHAN_OPTIONS"),
		    output.c_str());

    output = "";
    if (Parent->chanserv.LCK_Revenge())
	output << IRC_Bold;
    output << Parent->chanserv.DEF_Revenge();
    if (Parent->chanserv.LCK_Revenge())
	output << IRC_Off;    
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CHAN_REVENGE"),
		    output.c_str());

    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CHAN_ACCESS"),
		    Parent->chanserv.Level_Min(),
		    Parent->chanserv.Level_Max());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CHAN_NEWS"),
		    ToHumanTime(Parent->memoserv.News_Expire()).c_str());
}


void OperServ::do_settings_Other(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Other", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_INFLIGHT"),
		    ToHumanTime(Parent->memoserv.InFlight()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_AKILL1"),
		    ToHumanTime(Parent->operserv.Def_Expire()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_AKILL2"));
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_AKILL3"),
		    Parent->commserv.SADMIN_Name().c_str(),
		    ToHumanTime(Parent->operserv.Expire_SAdmin()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_AKILL3"),
		    Parent->commserv.SOP_Name().c_str(),
		    ToHumanTime(Parent->operserv.Expire_Sop()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_AKILL3"),
		    Parent->commserv.ADMIN_Name().c_str(),
		    ToHumanTime(Parent->operserv.Expire_Admin()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_AKILL3"),
		    Parent->commserv.OPER_Name().c_str(),
		    ToHumanTime(Parent->operserv.Expire_Oper()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_CLONES"),
		    Parent->operserv.Clone_Limit(),
		    Parent->operserv.Max_Clone());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_FLOOD1"),
		    Parent->operserv.Flood_Msgs(),
		    ToHumanTime(Parent->operserv.Flood_Time()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_FLOOD2"),
		    ToHumanTime(Parent->operserv.Ignore_Remove()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_IGNORE"),
		    ToHumanTime(Parent->operserv.Ignore_Time()).c_str(),
		    Parent->operserv.Ignore_Limit());
    mstring output = "";

    if (Parent->commserv.DEF_OpenMemos())
    {
	if (output != "")
	    output << ", ";
	if (Parent->commserv.LCK_OpenMemos())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS");
	if (Parent->commserv.LCK_OpenMemos())
	    output << IRC_Off;
    }

    if (Parent->commserv.DEF_Private())
    {
	if (output != "")
	    output << ", ";
	if (Parent->commserv.LCK_Private())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE");
	if (Parent->commserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Parent->commserv.DEF_Secure())
    {
	if (output != "")
	    output << ", ";
	if (Parent->commserv.LCK_Secure())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE");
	if (Parent->commserv.LCK_Secure())
	    output << IRC_Off;
    }
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/MISC_COMM_OPT"),
		    output.c_str());
}


void OperServ::do_settings_All(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_All", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    do_settings_Config(mynick, source, params);
    do_settings_Nick(mynick, source, params);
    do_settings_Channel(mynick, source, params);
    do_settings_Other(mynick, source, params);
}


void OperServ::do_clone_Add(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_clone_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 5)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = params.ExtractWord(3, " ").LowerCase();
    mstring amount = params.ExtractWord(4, " ");
    mstring reason = params.After(" ", 4);

    if (host.Contains("!"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
		    Parent->getMessage(source, "LIST/CLONE").c_str(), '!');
	return;
    }
    else if (host.Contains("@"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
		    Parent->getMessage(source, "LIST/CLONE").c_str(), '@');
	return;
    }

    if (!amount.IsNumber() || amount.Contains("."))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/WHOLENUMBER"));
	return;
    }

    unsigned int i, num;
    bool super = (Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	Parent->commserv.list[Parent->commserv.SADMIN_Name().UpperCase()].IsOn(source));
    // i+1 below because unsigned i will always be >= 0
    for (i=host.size()-1, num=0; i+1>0; i--)
    {
	switch (host[i])
	{
	case '@':
	    if (!super)
		i=0;
	    break;
	case '!':	// ALL these constitute wildcards.
	case '*':
	case '?':
	case '.':
	    break;
	default:
	    num++;
	}
    }
    // IF we have less than 1 char for 
    if (!super && num <= Parent->config.Starthresh())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/STARTHRESH"),
			Parent->getMessage(source, "LIST/CLONE").c_str(),
			Parent->config.Starthresh());
	return;
    }
    else if (num <= 1)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/STARTHRESH"),
			Parent->getMessage(source, "LIST/CLONE").c_str(), 1);
	return;
    }

    num = ACE_OS::atoi(amount.c_str());
    if (num < 1 || num > Parent->operserv.Max_Clone())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
				1, Parent->operserv.Max_Clone());
	return;
    }

    MLOCK(("OperServ", "Clone"));
    if (Parent->operserv.Clone_find(host))
    {
	mstring entry = Parent->operserv.Clone->Entry();
	Parent->operserv.Clone_erase();
	Parent->operserv.Clone_insert(entry, num, reason, source);
	Parent->operserv.stats.i_Clone++;
	::send(mynick, source, Parent->getMessage(source, "LIST/CHANGE_LEVEL"),
		    entry.c_str(),
		    Parent->getMessage(source, "LIST/CLONE").c_str(),
		    num);
	Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/CLONE_ADD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		entry.c_str(), num);
    }
    else
    {
	Parent->operserv.Clone_insert(host, num, reason, source);
	Parent->operserv.stats.i_Clone++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD_LEVEL"),
		    host.c_str(),
		    Parent->getMessage(source, "LIST/CLONE").c_str(),
		    num);
	Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/CLONE_ADD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		host.c_str(), num);
    }
}

void OperServ::do_clone_Del(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_clone_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = params.ExtractWord(3, " ").LowerCase();

    if (host.Contains("!"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
		    Parent->getMessage(source, "LIST/CLONE").c_str(), '!');
	return;
    }
    else if (host.Contains("@"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
		    Parent->getMessage(source, "LIST/CLONE").c_str(), '@');
	return;
    }

    MLOCK(("OperServ", "Clone"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int i, num = ACE_OS::atoi(host.c_str());
	if (num <= 0 || num > Parent->operserv.Clone_size())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
				1, Parent->operserv.Clone_size());
	    return;
	}

	for (i=1, Parent->operserv.Clone = Parent->operserv.Clone_begin();
		i < num && Parent->operserv.Clone != Parent->operserv.Clone_end();
		i++, Parent->operserv.Clone++) ;
	if (Parent->operserv.Clone != Parent->operserv.Clone_end())
	{
	    Parent->operserv.stats.i_Clone++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL"),
			Parent->operserv.Clone->Entry().c_str(),
			Parent->getMessage(source, "LIST/CLONE").c_str());
	    Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/CLONE_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->operserv.Clone->Entry().c_str());
	    Parent->operserv.Clone_erase();
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS_NUMBER"),
			num, Parent->getMessage(source, "LIST/CLONE").c_str());
	}
    }
    else
    {
	int count = 0;
	while (Parent->operserv.Clone_find(host))
	{
	    Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/CLONE_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->operserv.Clone->Entry().c_str());
	    Parent->operserv.Clone_erase();
	    count++;
	}

	if (count)
	{
	    Parent->operserv.stats.i_Clone++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL_MATCH"),
			count, host.c_str(),
			Parent->getMessage(source, "LIST/CLONE").c_str());
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS"),
			host.c_str(),
			Parent->getMessage(source, "LIST/CLONE").c_str());
	}
    }
}

void OperServ::do_clone_List(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_clone_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = "*";
    if (params.WordCount(" ") > 2)
    {
	host   = params.ExtractWord(3, " ").LowerCase();

	if (host.Contains("!"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
		    Parent->getMessage(source, "LIST/CLONE").c_str(), '!');
	    return;
	}
	else if (host.Contains("@"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
		    Parent->getMessage(source, "LIST/CLONE").c_str(), '@');
	    return;
	}
    }

    if (Parent->operserv.Clone_size())
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY_MATCH"),
		host.c_str(), Parent->getMessage(source, "LIST/CLONE").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY"),
		Parent->getMessage(source, "LIST/CLONE").c_str());
	return;
    }
    unsigned int i=1;
    MLOCK(("OperServ", "Clone"));
    for (Parent->operserv.Clone = Parent->operserv.Clone_begin();
		Parent->operserv.Clone != Parent->operserv.Clone_end();
		Parent->operserv.Clone++)
    {
	if (Parent->operserv.Clone->Entry().LowerCase().Matches(host))
	{
	    ::send(mynick, source, "%3d. %s (" + Parent->getMessage(source, "LIST/LASTMOD") + ")",
			    i, Parent->operserv.Clone->Entry().c_str(),
			    Parent->operserv.Clone->Last_Modify_Time().Ago().c_str(),
			    Parent->operserv.Clone->Last_Modifier().c_str());
	    ::send(mynick, source, "     [%4d] %s",
			    Parent->operserv.Clone->Value().first,
			    Parent->operserv.Clone->Value().second.c_str());
	    i++;
	}
    }
}

void OperServ::do_akill_Add(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_akill_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = params.ExtractWord(3, " ").LowerCase();
    mstring reason = params.After(" ", 3);
    unsigned long time = Parent->operserv.Def_Expire();

    if (FromHumanTime(reason.Before(" ")))
    {
	if (params.WordCount(" ") < 5)
	{
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	    return;
	}

	time = FromHumanTime(reason.Before(" "));
	reason = reason.After(" ");
    }

    if (Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	Parent->commserv.list[Parent->commserv.SADMIN_Name().UpperCase()].IsOn(source))
    {
	if (time > Parent->operserv.Expire_SAdmin())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/AKILLTOOHIGH"),
		    ToHumanTime(Parent->operserv.Expire_SAdmin()).c_str());
	    return;
	}
    }
    else if (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name().UpperCase()].IsOn(source))
    {
	if (time > Parent->operserv.Expire_Sop())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/AKILLTOOHIGH"),
		    ToHumanTime(Parent->operserv.Expire_Sop()).c_str());
	    return;
	}
    }
    else if (Parent->commserv.IsList(Parent->commserv.ADMIN_Name()) &&
	Parent->commserv.list[Parent->commserv.ADMIN_Name().UpperCase()].IsOn(source))
    {
	if (time > Parent->operserv.Expire_Admin())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/AKILLTOOHIGH"),
		    ToHumanTime(Parent->operserv.Expire_Admin()).c_str());
	    return;
	}
    }
    else if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name().UpperCase()].IsOn(source))
    {
	if (time > Parent->operserv.Expire_Oper())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/AKILLTOOHIGH"),
		    ToHumanTime(Parent->operserv.Expire_Oper()).c_str());
	    return;
	}
    }
	
    if (host.Contains("!"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
		    Parent->getMessage(source, "LIST/AKILL").c_str(), '!');
	return;
    }

    if (!host.Contains("@"))
    {
	host.Prepend("*@");
    }

    int i, num;
    bool super = (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name().UpperCase()].IsOn(source));
    for (i=host.size()-1, num=0; i>=0; i--)
    {
	switch (host[i])
	{
	case '@':
	    if (!super)
		i=0;
	    break;
	case '!':	// ALL these constitute wildcards.
	case '*':
	case '?':
	case '.':
	    break;
	default:
	    num++;
	}
    }
    // IF we have less than 1 char for 
    if (!super && num <= Parent->config.Starthresh())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/STARTHRESH"),
			Parent->getMessage(source, "LIST/AKILL").c_str(),
			Parent->config.Starthresh());
	return;
    }
    else if (num <= 1)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/STARTHRESH"),
			Parent->getMessage(source, "LIST/AKILL").c_str(), 1);
	return;
    }


    { MLOCK(("OperServ", "Akill"));
    if (Parent->operserv.Akill_find(host))
    {
	mstring entry = Parent->operserv.Akill->Entry();
	Parent->operserv.Akill_erase();
	Parent->operserv.Akill_insert(entry, time, reason, source);
	Parent->operserv.stats.i_Akill++;
	::send(mynick, source, Parent->getMessage(source, "LIST/CHANGE_TIME"),
		    entry.c_str(),
		    Parent->getMessage(source, "LIST/AKILL").c_str(),
		    ToHumanTime(time).c_str());
	announce(mynick, Parent->getMessage("MISC/AKILL_EXTEND"),
		    source.c_str(), entry.c_str(),
		    ToHumanTime(time).c_str());
	Log(LM_INFO, Parent->getLogMessage("OPERSERV/AKILL_ADD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		entry.c_str(), ToHumanTime(time).c_str(), reason.c_str());
    }
    else
    {
	Parent->operserv.Akill_insert(host, time, reason, source);
	Parent->operserv.stats.i_Akill++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD_TIME"),
		    host.c_str(),
		    Parent->getMessage(source, "LIST/AKILL").c_str(),
		    ToHumanTime(time).c_str());
	announce(mynick, Parent->getMessage("MISC/AKILL_ADD"),
		    source.c_str(), host.c_str(),
		    ToHumanTime(time).c_str(), reason.c_str());
	Log(LM_INFO, Parent->getLogMessage("OPERSERV/AKILL_ADD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		host.c_str(), ToHumanTime(time).c_str(), reason.c_str());
    }}

    Parent->server.AKILL(host, reason, time, source);
    map<mstring,Nick_Live_t>::iterator nlive;
    vector<mstring> killusers;
    { RLOCK(("NickServ", "live"));
    for (nlive = Parent->nickserv.live.begin(); nlive != Parent->nickserv.live.end(); nlive++)
    {
	if (nlive->second.Mask(Nick_Live_t::N_U_P_H).After("!").Matches(host))
	    killusers.push_back(nlive->first);
    }}
    for (i=0; i<killusers.size(); i++)
	Parent->server.KILL(mynick, killusers[i], reason);
}

void OperServ::do_akill_Del(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_akill_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = params.ExtractWord(3, " ").LowerCase();

    if (host.Contains("!"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
		    Parent->getMessage(source, "LIST/AKILL").c_str(), '!');
	return;
    }

    MLOCK(("OperServ", "Akill"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int i, num = ACE_OS::atoi(host.c_str());
	if (num <= 0 || num > Parent->operserv.Akill_size())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
				1, Parent->operserv.Akill_size());
	    return;
	}

	for (i=1, Parent->operserv.Akill = Parent->operserv.Akill_begin();
		i < num && Parent->operserv.Akill != Parent->operserv.Akill_end();
		i++, Parent->operserv.Akill++) ;
	if (Parent->operserv.Akill != Parent->operserv.Akill_end())
	{
	    Parent->operserv.stats.i_Akill++;
	    Parent->server.RAKILL(Parent->operserv.Akill->Entry());
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL"),
			Parent->operserv.Akill->Entry().c_str(),
			Parent->getMessage(source, "LIST/AKILL").c_str());
	Log(LM_INFO, Parent->getLogMessage("OPERSERV/AKILL_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->operserv.Akill->Entry().c_str());
	    Parent->operserv.Akill_erase();
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS_NUMBER"),
			num, Parent->getMessage(source, "LIST/AKILL").c_str());
	}
    }
    else
    {
	int count = 0;
	while (Parent->operserv.Akill_find(host))
	{
	    Parent->server.RAKILL(Parent->operserv.Akill->Entry());
	    Log(LM_INFO, Parent->getLogMessage("OPERSERV/AKILL_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->operserv.Akill->Entry().c_str());
	    Parent->operserv.Akill_erase();
	    count++;
	}

	if (count)
	{
	    Parent->operserv.stats.i_Akill++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL_MATCH"),
			count, host.c_str(),
			Parent->getMessage(source, "LIST/AKILL").c_str());
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS"),
			host.c_str(),
			Parent->getMessage(source, "LIST/AKILL").c_str());
	}
    }
}

void OperServ::do_akill_List(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_akill_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = "*";
    if (params.WordCount(" ") > 2)
    {
	host   = params.ExtractWord(3, " ").LowerCase();

	if (host.Contains("!"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
		    Parent->getMessage(source, "LIST/AKILL").c_str(), '!');
	    return;
	}
    }

    if (Parent->operserv.Akill_size())
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY_MATCH"),
		host.c_str(), Parent->getMessage(source, "LIST/AKILL").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY"),
		Parent->getMessage(source, "LIST/AKILL").c_str());
	return;
    }
    unsigned int i=1;
    MLOCK(("OperServ", "Akill"));
    for (Parent->operserv.Akill = Parent->operserv.Akill_begin();
		Parent->operserv.Akill != Parent->operserv.Akill_end();
		Parent->operserv.Akill++)
    {
	if (Parent->operserv.Akill->Entry().LowerCase().Matches(host))
	{
	    ::send(mynick, source, "%3d. %s (" + Parent->getMessage(source, "LIST/LASTMOD") + ")",
			    i, Parent->operserv.Akill->Entry().c_str(),
			    Parent->operserv.Akill->Last_Modify_Time().Ago().c_str(),
			    Parent->operserv.Akill->Last_Modifier().c_str());
	    ::send(mynick, source, "     [%s] %s",
			    ToHumanTime(Parent->operserv.Akill->Value().first).c_str(),
			    Parent->operserv.Akill->Value().second.c_str());
	    i++;
	}
    }
}

void OperServ::do_operdeny_Add(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_operdeny_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = params.ExtractWord(3, " ").LowerCase();
    mstring reason = params.After(" ", 3);

    if (!host.Contains("@"))
    {
	if (!Parent->nickserv.IsLive(host))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
			    host.c_str());
	    return;
	}
	host = Parent->nickserv.live[host.LowerCase()].Mask(Parent->operserv.Ignore_Method());
    }
    else if (!host.Contains("!"))
    {
	host.Prepend("*!");
    }

    unsigned int i, num;
    // i+1 below because unsigned i will always be >= 0
    for (i=host.size()-1, num=0; i+1>0; i--)
    {
	switch (host[i])
	{
	case '@':
	case '!':	// ALL these constitute wildcards.
	case '*':
	case '?':
	case '.':
	    break;
	default:
	    num++;
	}
    }

    // IF we have less than 1 char for 
    if (num <= Parent->config.Starthresh())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/STARTHRESH"),
			Parent->getMessage(source, "LIST/OPERDENY").c_str(),
			Parent->config.Starthresh());
	return;
    }

    { MLOCK(("OperServ", "OperDeny"));
    if (Parent->operserv.OperDeny_find(host))
    {
	Parent->operserv.OperDeny_erase();
    }
    Parent->operserv.OperDeny_insert(host, reason, source);
    }
    Parent->operserv.stats.i_OperDeny++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/ADD"),
	host.c_str(), Parent->getMessage(source, "LIST/OPERDENY").c_str());
    announce(mynick, Parent->getMessage("MISC/OPERDENY_ADD"),
		source.c_str(), host.c_str());
    Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/OPERDENY_ADD"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	host.c_str(), reason.c_str());

    map<mstring,Nick_Live_t>::iterator nlive;
    vector<mstring> killusers;
    for (nlive = Parent->nickserv.live.begin(); nlive != Parent->nickserv.live.end(); nlive++)
    {
	if (nlive->second.Mask(Nick_Live_t::N_U_P_H).Matches(host))
	{
	    // IF user is recognized and on sadmin, ignore.
	    if (!(Parent->nickserv.IsStored(nlive->first) &&
		Parent->nickserv.stored[nlive->first].IsOnline() &&
		Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
		Parent->commserv.list[Parent->commserv.SADMIN_Name()].IsIn(nlive->first)))
		continue;
	    if (Parent->server.proto.SVS())
	    {
		nlive->second.SendMode("-oAa");
	    }
	    else
	    {
		killusers.push_back(nlive->first);
	    }
	}
    }
    for (i=0; i<killusers.size(); i++)
	Parent->server.KILL(Parent->operserv.FirstName(), killusers[i],
		    Parent->getMessage("MISC/KILL_OPERDENY"));

}

void OperServ::do_operdeny_Del(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_operdeny_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = params.ExtractWord(3, " ").LowerCase();

    MLOCK(("OperServ", "OperDeny"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int i, num = ACE_OS::atoi(host.c_str());
	if (num <= 0 || num > Parent->operserv.OperDeny_size())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
				1, Parent->operserv.OperDeny_size());
	    return;
	}

	for (i=1, Parent->operserv.OperDeny = Parent->operserv.OperDeny_begin();
		i < num && Parent->operserv.OperDeny != Parent->operserv.OperDeny_end();
		i++, Parent->operserv.OperDeny++) ;
	if (Parent->operserv.OperDeny != Parent->operserv.OperDeny_end())
	{
	    Parent->operserv.stats.i_OperDeny++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL"),
			Parent->operserv.OperDeny->Entry().c_str(),
			Parent->getMessage(source, "LIST/OPERDENY").c_str());
	    Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/OPERDENY_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->operserv.OperDeny->Entry().c_str());
	    Parent->operserv.OperDeny_erase();
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS_NUMBER"),
			num, Parent->getMessage(source, "LIST/OPERDENY").c_str());
	}
    }
    else
    {
	if (!host.Contains("@"))
	{
	    if (!Parent->nickserv.IsLive(host))
	    {
		::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
			    host.c_str());
		return;
	    }
	    host = Parent->nickserv.live[host.LowerCase()].Mask(Parent->operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
        {
	    host.Prepend("*!");
	}

	int count = 0;
	while (Parent->operserv.OperDeny_find(host))
	{
	    Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/OPERDENY_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->operserv.OperDeny->Entry().c_str());
	    Parent->operserv.OperDeny_erase();
	    count++;
	}

	if (count)
	{
	    Parent->operserv.stats.i_OperDeny++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL_MATCH"),
			count, host.c_str(),
			Parent->getMessage(source, "LIST/OPERDENY").c_str());
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS"),
			host.c_str(),
			Parent->getMessage(source, "LIST/OPERDENY").c_str());
	}
    }
}

void OperServ::do_operdeny_List(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_operdeny_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = "*";
    if (params.WordCount(" ") > 2)
    {
	host   = params.ExtractWord(3, " ").LowerCase();

	if (!host.Contains("@"))
	{
	    if (!Parent->nickserv.IsLive(host))
	    {
		::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
			    host.c_str());
		return;
	    }
	    host = Parent->nickserv.live[host.LowerCase()].Mask(Parent->operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.Prepend("*!");
	}
    }

    if (Parent->operserv.OperDeny_size())
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY_MATCH"),
		host.c_str(), Parent->getMessage(source, "LIST/OPERDENY").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY"),
		Parent->getMessage(source, "LIST/OPERDENY").c_str());
	return;
    }
    unsigned int i=1;
    MLOCK(("OperServ", "OperDeny"));
    for (Parent->operserv.OperDeny = Parent->operserv.OperDeny_begin();
		Parent->operserv.OperDeny != Parent->operserv.OperDeny_end();
		Parent->operserv.OperDeny++)
    {
	if (Parent->operserv.OperDeny->Entry().LowerCase().Matches(host))
	{
	    ::send(mynick, source, "%3d. %s (" + Parent->getMessage(source, "LIST/LASTMOD") + ")",
			    i, Parent->operserv.OperDeny->Entry().c_str(),
			    Parent->operserv.OperDeny->Last_Modify_Time().Ago().c_str(),
			    Parent->operserv.OperDeny->Last_Modifier().c_str());
	    ::send(mynick, source, "     %s",
			    Parent->operserv.OperDeny->Value().c_str());
	    i++;
	}
    }
}

void OperServ::do_ignore_Add(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_ignore_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = params.ExtractWord(3, " ").LowerCase();

    if (!host.Contains("@"))
    {
	if (!Parent->nickserv.IsLive(host))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
			    host.c_str());
	    return;
	}
	host = Parent->nickserv.live[host.LowerCase()].Mask(Parent->operserv.Ignore_Method());
    }
    else if (!host.Contains("!"))
    {
	host.Prepend("*!");
    }

    unsigned int i, num;
    bool super = (Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	Parent->commserv.list[Parent->commserv.SADMIN_Name().UpperCase()].IsOn(source));
    // i+1 below because unsigned i will always be >= 0
    for (i=host.size()-1, num=0; i+1>0; i--)
    {
	switch (host[i])
	{
	case '@':
	case '!':	// ALL these constitute wildcards.
	case '*':
	case '?':
	case '.':
	    break;
	default:
	    num++;
	}
    }
    // IF we have less than 1 char for 
    if (!super && num <= Parent->config.Starthresh())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/STARTHRESH"),
			Parent->getMessage(source, "LIST/SIGNORE").c_str(),
			Parent->config.Starthresh());
	return;
    }
    else if (num <= 1)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/STARTHRESH"),
			Parent->getMessage(source, "LIST/SIGNORE").c_str(), 1);
	return;
    }

    MLOCK(("OperServ", "Ignore"));
    if (Parent->operserv.Ignore_find(host))
    {
	Parent->operserv.Ignore_erase();
    }
    Parent->operserv.Ignore_insert(host, true, source);
    Parent->operserv.stats.i_Ignore++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/ADD"),
	    host.c_str(), Parent->getMessage(source, "LIST/SIGNORE").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/IGNORE_ADD"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	host.c_str());
}

void OperServ::do_ignore_Del(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_ignore_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = params.ExtractWord(3, " ").LowerCase();

    MLOCK(("OperServ", "Ignore"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int i, num = ACE_OS::atoi(host.c_str());
	if (num <= 0 || num > Parent->operserv.Ignore_size())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
				1, Parent->operserv.Ignore_size());
	    return;
	}

	for (i=1, Parent->operserv.Ignore = Parent->operserv.Ignore_begin();
		i < num && Parent->operserv.Ignore != Parent->operserv.Ignore_end();
		i++, Parent->operserv.Ignore++) ;
	if (Parent->operserv.Ignore != Parent->operserv.Ignore_end())
	{
	    Parent->operserv.stats.i_Ignore++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL"),
			Parent->operserv.Ignore->Entry().c_str(),
			Parent->getMessage(source, "LIST/SIGNORE").c_str());
	    Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/IGNORE_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->operserv.Ignore->Entry().c_str());
	    Parent->operserv.Ignore_erase();
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS_NUMBER"),
			num, Parent->getMessage(source, "LIST/SIGNORE").c_str());
	}
    }
    else
    {
	if (!host.Contains("@"))
	{
	    if (!Parent->nickserv.IsLive(host))
	    {
		::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
			    host.c_str());
		return;
	    }
	    host = Parent->nickserv.live[host.LowerCase()].Mask(Parent->operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
        {
	    host.Prepend("*!");
	}

	int count = 0;
	while (Parent->operserv.Ignore_find(host))
	{
	    Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/IGNORE_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->operserv.Ignore->Entry().c_str());
	    Parent->operserv.Ignore_erase();
	    count++;
	}

	if (count)
	{
	    Parent->operserv.stats.i_Ignore++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL_MATCH"),
			count, host.c_str(),
			Parent->getMessage(source, "LIST/SIGNORE").c_str());
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS"),
			host.c_str(),
			Parent->getMessage(source, "LIST/SIGNORE").c_str());
	}
    }
}

void OperServ::do_ignore_List(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_ignore_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring host   = "*";
    if (params.WordCount(" ") > 2)
    {
	host   = params.ExtractWord(3, " ").LowerCase();

	if (!host.Contains("@"))
	{
	    if (!Parent->nickserv.IsLive(host))
	    {
		::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
			    host.c_str());
		return;
	    }
	    host = Parent->nickserv.live[host.LowerCase()].Mask(Parent->operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.Prepend("*!");
	}
    }

    unsigned int i=1;
    bool head = false;
    MLOCK(("OperServ", "Ignore"));
    for (Parent->operserv.Ignore = Parent->operserv.Ignore_begin();
		Parent->operserv.Ignore != Parent->operserv.Ignore_end();
		Parent->operserv.Ignore++)
    {
	if (Parent->operserv.Ignore->Entry().LowerCase().Matches(host) &&
	    Parent->operserv.Ignore->Value())
	{
	    if (head == false)
	    {
		::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY_MATCH"),
			host.c_str(),
			Parent->getMessage(source, "LIST/SIGNORE").c_str());
		head = true;
	    }
	    ::send(mynick, source, "%3d. %s (" + Parent->getMessage(source, "LIST/LASTMOD") + ")",
			    i, Parent->operserv.Ignore->Entry().c_str(),
			    Parent->operserv.Ignore->Last_Modify_Time().Ago().c_str(),
			    Parent->operserv.Ignore->Last_Modifier().c_str());
	    i++;
	}
    }
    if (head == false)
	::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY"),
		Parent->getMessage(source, "LIST/SIGNORE").c_str());
}

SXP::Tag OperServ::tag_OperServ("OperServ");
SXP::Tag OperServ::tag_Clone("Clone");
SXP::Tag OperServ::tag_Akill("Akill");
SXP::Tag OperServ::tag_OperDeny("OperDeny");
SXP::Tag OperServ::tag_Ignore("Ignore");

void OperServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("OperServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    set<entlist_t>::size_type ei,ecount;
    set<entlist_val_t<long> >::size_type vli,vlcount;
    set<entlist_val_t<mstring> >::size_type vsi,vscount;
    mstring dummy,dummy2;
    entlist_t edummy;
    entlist_val_t<long> eldummy;
    entlist_val_t<mstring> esdummy;

    //TODO: Add your source code here

    if( pElement->IsA(tag_Clone) )
    {
	entlist_val_t<pair<unsigned int, mstring> > *tmp = new entlist_val_t<pair<unsigned int, mstring> >;
	c_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_Akill) )
    {
	entlist_val_t<pair<unsigned long, mstring> > *tmp = new entlist_val_t<pair<unsigned long, mstring> >;
	a_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_OperDeny) )
    {
	entlist_val_t<mstring> *tmp = new entlist_val_t<mstring>;
	o_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_Ignore) )
    {
	entlist_val_t<bool> *tmp = new entlist_val_t<bool>;
	i_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }
}

void OperServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("OperServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
}

void OperServ::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("OperServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator i;
    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator j;
    set<entlist_val_t<mstring> >::iterator k;
    set<entlist_val_t<bool> >::iterator l;

    //TODO: Add your source code here
	pOut->BeginObject(tag_OperServ, attribs);

	{ MLOCK(("OperServ", "Clone"));
	for(i=i_Clone.begin(); i!=i_Clone.end(); i++)
	{
	    pOut->BeginObject(tag_Clone, attribs);
	    pOut->WriteSubElement((entlist_val_t<pair<unsigned int, mstring> > *) &(*i), attribs);
	    pOut->EndObject(tag_Clone);
	}}

	{ MLOCK(("OperServ", "Akill"));
	for(j=i_Akill.begin(); j!=i_Akill.end(); j++)
	{
	    pOut->BeginObject(tag_Akill, attribs);
	    pOut->WriteSubElement((entlist_val_t<pair<unsigned long, mstring> > *) &(*j), attribs);
	    pOut->EndObject(tag_Akill);
	}}

	{ MLOCK(("OperServ", "OperDeny"));
	for(k=i_OperDeny.begin(); k!=i_OperDeny.end(); k++)
	{
	    pOut->BeginObject(tag_OperDeny, attribs);
	    pOut->WriteSubElement((entlist_val_t<mstring> *) &(*k), attribs);
	    pOut->EndObject(tag_OperDeny);
	}}

	{ MLOCK(("OperServ", "Ignore"));
	for(l=i_Ignore.begin(); l!=i_Ignore.end(); l++)
	{
	    // Only save PERM entries
	    if (l->Value())
	    {
		pOut->BeginObject(tag_Ignore, attribs);
		pOut->WriteSubElement((entlist_val_t<bool> *) &(*l), attribs);
		pOut->EndObject(tag_Ignore);
	    }
	}}

	pOut->EndObject(tag_OperServ);
}

void OperServ::PostLoad()
{
    NFT("OperServ::PostLoad");
    // Linkage, etc

    unsigned int i;
    for (i=0; i<c_array.size(); i++)
    {
	if (c_array[i] != NULL)
	{
	    i_Clone.insert(*c_array[i]);
	    delete c_array[i];
	}
    }
    c_array.clear();

    for (i=0; i<a_array.size(); i++)
    {
	if (a_array[i] != NULL)
	{
	    i_Akill.insert(*a_array[i]);
	    delete a_array[i];
	}
    }
    a_array.clear();

    for (i=0; i<o_array.size(); i++)
    {
	if (o_array[i] != NULL)
	{
	    i_OperDeny.insert(*o_array[i]);
	    delete o_array[i];
	}
    }
    o_array.clear();

    for (i=0; i<i_array.size(); i++)
    {
	if (i_array[i] != NULL)
	{
	    i_Ignore.insert(*i_array[i]);
	    delete i_array[i];
	}
    }
    i_array.clear();

    // Kind of dodgy, yes, I know, however we NEED to do the
    // post load on all the elements and its just too time
    // and energy consuming to pop them all of and re-insert
    entlist_val_t<pair<unsigned int, mstring> > *c_ptr;
    entlist_val_t<pair<unsigned long, mstring> > *a_ptr;
    entlist_val_t<mstring> *o_ptr;
    entlist_val_t<bool> *i_ptr;

    for (Clone = Clone_begin(); Clone != Clone_end(); Clone++)
    {
	c_ptr = (entlist_val_t<pair<unsigned int, mstring> > *) &(*Clone);
	c_ptr->PostLoad();
    }
    for (Akill = Akill_begin(); Akill != Akill_end(); Akill++)
    {
	a_ptr = (entlist_val_t<pair<unsigned long, mstring> > *) &(*Akill);
	a_ptr->PostLoad();
    }
    for (OperDeny = OperDeny_begin(); OperDeny != OperDeny_end(); OperDeny++)
    {
	o_ptr = (entlist_val_t<mstring> *) &(*OperDeny);
	o_ptr->PostLoad();
    }
    for (Ignore = Ignore_begin(); Ignore != Ignore_end(); Ignore++)
    {
	i_ptr = (entlist_val_t<bool> *) &(*Ignore);
	i_ptr->PostLoad();
    }
}

void OperServ::DumpB()
{
    MB(0, (i_Clone.size(), CloneList.size(), i_Akill.size(),
	i_OperDeny.size(), i_Ignore.size()));
}

void OperServ::DumpE()
{
    ME(0, (i_Clone.size(), CloneList.size(), i_Akill.size(),
	i_OperDeny.size(), i_Ignore.size()));
}

