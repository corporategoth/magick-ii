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
#define RCSID(x,y) const char *rcsid_operserv_cpp_ ## x () { return y; }
RCSID(operserv_cpp, "@(#)$Id$");

/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.142  2002/01/13 05:18:42  prez
** More formatting, changed style slightly
**
** Revision 1.141  2002/01/12 14:42:09  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.140  2002/01/10 19:30:39  prez
** FINALLY finished a MAJOR overhaul ... now have a 'safe pointer', that
** ensures that data being used cannot be deleted while still being used.
**
** Revision 1.139  2001/12/25 08:43:13  prez
** Fixed XML support properly ... it now works again with new version of
** expat (1.95.2) and sxp (1.1).  Also removed some of my const hacks.
**
** Revision 1.138  2001/12/20 08:02:33  prez
** Massive change -- 'Parent' has been changed to Magick::instance(), will
** soon also move the ACE_Reactor over, and will be able to have multipal
** instances of Magick in the same process if necessary.
**
** Revision 1.137  2001/11/18 03:26:53  prez
** More changes re: trace names, and made the command system know the
** difference between 'insufficiant access' and 'unknown command'.
**
** Revision 1.136  2001/11/18 01:54:04  prez
** Fixed up trace levels (again)
**
** Revision 1.135  2001/11/18 01:44:56  prez
** Changed trace levels
**
** Revision 1.134  2001/11/12 01:05:03  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.133  2001/11/04 19:23:09  ungod
** fixed up compilation for borland c++ builder
**
** Revision 1.132  2001/11/03 21:02:54  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.131  2001/07/29 21:22:26  prez
** Delayed clone akills on sync until AFTER we're synced
**
** Revision 1.130  2001/07/21 18:09:44  prez
** Fixed IsBool in mstring and made SVINFO actually give a GMT timestamp.
**
** Revision 1.129  2001/07/02 03:39:29  prez
** Fixed bug with users sending printf strings (mainly in memos).
**
** Revision 1.128  2001/06/17 09:39:07  prez
** Hopefully some more changes that ensure uptime (mainly to do with locking
** entries in an iterated search, and using copies of data instead of references
** where we can get away with it -- reducing the need to lock the data).
**
** Revision 1.127  2001/06/15 07:20:41  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.126  2001/06/07 06:21:06  prez
** Think I fixed staging encryption layer ... ugh.
**
** Revision 1.125  2001/06/03 05:04:53  prez
** small fixes in operdeny and akill adding
**
** Revision 1.124  2001/05/17 19:18:55  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.123  2001/05/13 00:55:18  prez
** More patches to try and fix deadlocking ...
**
** Revision 1.122  2001/05/06 03:03:07  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.121  2001/05/05 17:33:59  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.120  2001/05/01 14:00:24  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.119  2001/03/27 07:04:32  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.118  2001/03/20 14:22:15  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.117  2001/03/04 02:04:15  prez
** Made mstring a little more succinct ... and added vector/list operations
**
** Revision 1.116  2001/03/02 05:24:42  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.115  2001/02/11 07:41:28  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.114  2001/02/03 02:21:34  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.113  2001/01/15 23:31:39  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.112  2001/01/01 05:32:45  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.111  2000/12/29 15:31:55  prez
** Added locking/checking for dcc/events threads.  Also for ACE_Log_Msg
**
** Revision 1.110  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.109  2000/12/22 19:50:19  prez
** Made all config options const.  Beginnings of securing all non-modifying
** commands to const.  also added serviceschk.
**
** Revision 1.108  2000/12/22 03:30:26  prez
** Fixed bug in nickserv ident.
**
** Revision 1.107  2000/12/21 14:18:18  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.106  2000/12/19 14:26:55  prez
** Bahamut has changed SVSNICK -> MODNICK, so i_SVS has been changed into
** several SVS command text strings, if blank, support isnt there.
**
** Revision 1.105  2000/12/19 07:24:54  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.104  2000/10/10 11:47:52  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.103  2000/10/07 11:00:13  ungod
** no message
**
** Revision 1.102  2000/09/30 10:48:08  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.101  2000/09/09 02:17:49  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
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

#include "magick.h"
#include "dccengine.h"

bool OperServ::AddHost(const mstring & host)
{
    FT("OperServ::AddHost", (host));
    bool retval = false;

    MLOCK(("OperServ", "CloneList"));
    MCB(CloneList.size());
    if (CloneList.find(host.LowerCase()) == CloneList.end())
	CloneList[host.LowerCase()].first = 0;
    CloneList[host.LowerCase()].first++;

    CP(("Finding clone list, %s = %d", host.c_str(), CloneList[host.LowerCase()].first));
    {
	MLOCK2(("OperServ", "Clone"));
	if ((Clone_find(host) ? (CloneList[host.LowerCase()].first > Clone->Value().first)
	     : (CloneList[host.LowerCase()].first > Clone_Limit())))
	{
	    // Get rid of entries from the beginning...
	    while (CloneList[host.LowerCase()].second.size() &&
		   CloneList[host.LowerCase()].second.begin()->SecondsSince() > Magick::instance().operserv.Clone_Time())
		CloneList[host.LowerCase()].second.erase(CloneList[host.LowerCase()].second.begin());

	    CP(("Event Size after purge is %d", CloneList[host.LowerCase()].second.size()));

	    CloneList[host.LowerCase()].second.push_back(mDateTime::CurrentDateTime());
	    bool burst = false;

	    {
		RLOCK(("IrcSvcHandler"));
		if (Magick::instance().ircsvchandler != NULL)
		    burst = Magick::instance().ircsvchandler->Burst();
	    }

	    if (!burst && CloneList[host.LowerCase()].second.size() > Magick::instance().operserv.Clone_Trigger())
	    {
		CP(("Reached MAX clone kills, adding AKILL ..."));

		MLOCK3(("OperServ", "Akill"));
		if (!Akill_find("*@" + host))
		{
		    NickServ::live_t::iterator iter;
		    vector < mstring > killusers;
		    {
			RLOCK(("NickServ", "live"));
			for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd();
			     iter++)
			{
			    map_entry < Nick_Live_t > nlive(iter->second);
			    if (nlive->Host().IsSameAs(host, true))
				killusers.push_back(iter->first);
			}
		    }

		    float percent =
			100.0 * static_cast < float >(killusers.size()) / static_cast <
			float >(Magick::instance().nickserv.LiveSize());

		    Magick::instance().server.AKILL("*@" + host, Magick::instance().operserv.Clone_Akill(),
						    Magick::instance().operserv.Clone_AkillTime(),
						    Magick::instance().nickserv.FirstName());

		    Akill_insert("*@" + host, Magick::instance().operserv.Clone_AkillTime(),
				 Magick::instance().operserv.Clone_Akill(), FirstName());
		    ANNOUNCE(FirstName(), "MISC/AKILL_ADD",
			     (FirstName(), host, ToHumanTime(Magick::instance().operserv.Clone_AkillTime()),
			      Magick::instance().operserv.Clone_Akill(), killusers.size(), fmstring("%.2f", percent)));
		    LOG(LM_INFO, "OPERSERV/AKILL_ADD",
			(FirstName(), host, ToHumanTime(Magick::instance().operserv.Clone_AkillTime()),
			 Magick::instance().operserv.Clone_Akill()));
		}
	    }
	    retval = true;
	}
    }

    MCE(CloneList.size());
    RET(retval);
}

void OperServ::RemHost(const mstring & host)
{
    FT("OperServ::RemHost", (host));

    MLOCK(("OperServ", "CloneList"));
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

size_t OperServ::CloneList_sum() const
{
    NFT("OperServ::ClonesList_sum");

    map < mstring, pair < unsigned int, list < mDateTime > > >::const_iterator i;
    size_t value = 0;

    RLOCK(("OperServ", "CloneList"));
    for (i = CloneList.begin(); i != CloneList.end(); i++)
    {
	value += i->second.first;
    }
    RET(value);
}

size_t OperServ::CloneList_size(const unsigned int amt) const
{
    FT("OperServ::CloneList_size", (amt));

    map < mstring, pair < unsigned int, list < mDateTime > > >::const_iterator i;
    size_t value = 0;

    RLOCK(("OperServ", "CloneList"));
    for (i = CloneList.begin(); i != CloneList.end(); i++)
    {
	if (i->second.first == amt)
	    value++;
    }
    RET(value);
}

size_t OperServ::CloneList_Usage() const
{
    size_t retval = 0;
    map < mstring, pair < unsigned int, list < mDateTime > > >::const_iterator i;

    MLOCK(("OperServ", "CloneList"));
    for (i = CloneList.begin(); i != CloneList.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second.first);
	list < mDateTime >::const_iterator j;
	for (j = i->second.second.begin(); j != i->second.second.end(); j++)
	    retval += sizeof(j->Internal());
    }
    return retval;
}

void OperServ::CloneList_check()
{
    NFT("OperServ::CloneList_check");

    map < mstring, pair < unsigned int, list < mDateTime > > >::iterator iter;

    MLOCK(("OperServ", "CloneList"));
    for (iter = CloneList.begin(); iter != CloneList.end(); iter++)
    {
	if (iter->second.second.size() > Magick::instance().operserv.Clone_Trigger())
	{
	    CP(("Reached MAX clone kills, adding AKILL ..."));

	    MLOCK2(("OperServ", "Akill"));
	    if (!Akill_find("*@" + iter->first))
	    {
		NickServ::live_t::iterator iter2;
		vector < mstring > killusers;
		{
		    RLOCK(("NickServ", "live"));
		    for (iter2 = Magick::instance().nickserv.LiveBegin(); iter2 != Magick::instance().nickserv.LiveEnd();
			 iter2++)
		    {
			map_entry < Nick_Live_t > nlive(iter2->second);
			if (nlive->Host().IsSameAs(iter->first, true))
			    killusers.push_back(iter2->first);
		    }
		}

		float percent =
		    100.0 * static_cast < float >(killusers.size()) / static_cast <
		    float >(Magick::instance().nickserv.LiveSize());

		Magick::instance().server.AKILL("*@" + iter->first, Magick::instance().operserv.Clone_Akill(),
						Magick::instance().operserv.Clone_AkillTime(),
						Magick::instance().nickserv.FirstName());

		Akill_insert("*@" + iter->first, Magick::instance().operserv.Clone_AkillTime(),
			     Magick::instance().operserv.Clone_Akill(), FirstName());
		ANNOUNCE(FirstName(), "MISC/AKILL_ADD",
			 (FirstName(), iter->first, ToHumanTime(Magick::instance().operserv.Clone_AkillTime()),
			  Magick::instance().operserv.Clone_Akill(), killusers.size(), fmstring("%.2f", percent)));
		LOG(LM_INFO, "OPERSERV/AKILL_ADD",
		    (FirstName(), iter->first, ToHumanTime(Magick::instance().operserv.Clone_AkillTime()),
		     Magick::instance().operserv.Clone_Akill()));
	    }
	}
    }
}

bool OperServ::Clone_insert(const mstring & entry, const unsigned int value, const mstring & reason, const mstring & nick,
			    const mDateTime & added)
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
	pair < set < Clone_Type >::iterator, bool > tmp;
	MCB(i_Clone.size());
	tmp = i_Clone.insert(Clone_Type(entry, pair < unsigned int, mstring > (value, reason), nick, added));

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

size_t OperServ::Clone_Usage() const
{
    size_t retval = 0;

    set < Clone_Type >::const_iterator i;
    MLOCK(("OperServ", "Clone"));
    for (i = i_Clone.begin(); i != i_Clone.end(); i++)
    {
	retval += i->Usage();
    }
    return retval;
}

bool OperServ::Clone_find(const mstring & entry)
{
    FT("OperServ::Clone_find", (entry));

//  entlist_val_ui<pair<int, mstring> > iter = i_Clone.end();
    MLOCK(("OperServ", "Clone"));
    set < Clone_Type >::iterator iter = i_Clone.end();
    if (!i_Clone.empty())
	for (iter = i_Clone.begin(); iter != i_Clone.end(); iter++)
	    if (entry.Matches(iter->Entry(), true))
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

pair < unsigned int, mstring > OperServ::Clone_value(const mstring & entry)
{
    FT("OperServ::Clone_value", (entry));

    pair < unsigned int, mstring > retval = pair < unsigned int, mstring > (0, "");

    MLOCK(("OperServ", "Clone"));
//  entlist_val_ui<pair<int, mstring> > iter = Clone;
    set < Clone_Type >::iterator iter = Clone;

    if (Clone_find(entry))
	retval = Clone->Value();
    Clone = iter;
    return retval;
}

bool OperServ::Akill_insert(const mstring & entry, const unsigned long value, const mstring & reason, const mstring & nick,
			    const mDateTime & added)
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
	pair < set < Akill_Type >::iterator, bool > tmp;
	MCB(i_Akill.size());
	tmp = i_Akill.insert(Akill_Type(entry, pair < unsigned long, mstring > (value, reason), nick, added));

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

size_t OperServ::Akill_Usage() const
{
    size_t retval = 0;

    set < Akill_Type >::const_iterator i;
    MLOCK(("OperServ", "Akill"));
    for (i = i_Akill.begin(); i != i_Akill.end(); i++)
    {
	retval += i->Usage();
    }
    return retval;
}

bool OperServ::Akill_find(const mstring & entry)
{
    FT("OperServ::Akill_find", (entry));

    // no @
    if (!entry.Contains("@") || entry.Contains("!"))
    {
	RET(false);
    }

//  entlist_val_ui<pair<long, mstring> > iter = i_Akill.end();
    MLOCK(("OperServ", "Akill"));
    set < Akill_Type >::iterator iter = i_Akill.end();
    if (!i_Akill.empty())
	for (iter = i_Akill.begin(); iter != i_Akill.end(); iter++)
	{
	    if (entry.Matches(iter->Entry(), true))
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

pair < unsigned long, mstring > OperServ::Akill_value(const mstring & entry)
{
    FT("OperServ::Akill_value", (entry));

    pair < unsigned long, mstring > retval = pair < unsigned long, mstring > (0, "");

    MLOCK(("OperServ", "Akill"));
//  entlist_val_ui<pair<long, mstring> > iter = Akill;
    set < Akill_Type >::iterator iter = Akill;

    if (Akill_find(entry))
	retval = Akill->Value();
    Akill = iter;
    return retval;
}

bool OperServ::OperDeny_insert(const mstring & i_entry, const mstring & value, const mstring & nick)
{
    FT("OperServ::OperDeny_insert", (i_entry, value, nick));

    // no @
    if (!i_entry.Contains("@"))
    {
	RET(false);
    }
    mstring entry(i_entry);

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.prepend("*!");

    MLOCK(("OperServ", "OperDeny"));
    if (!OperDeny_find(entry))
    {
	pair < set < OperDeny_Type >::iterator, bool > tmp;
	MCB(i_OperDeny.size());
	tmp = i_OperDeny.insert(OperDeny_Type(entry, value, nick));
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

size_t OperServ::OperDeny_Usage() const
{
    size_t retval = 0;

    set < OperDeny_Type >::const_iterator i;
    MLOCK(("OperServ", "OperDeny"));
    for (i = i_OperDeny.begin(); i != i_OperDeny.end(); i++)
    {
	retval += i->Usage();
    }
    return retval;
}

bool OperServ::OperDeny_find(const mstring & i_entry)
{
    FT("OperServ::OperDeny_find", (i_entry));

    // no @
    if (!i_entry.Contains("@"))
    {
	RET(false);
    }
    mstring entry(i_entry);

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.prepend("*!");

    MLOCK(("OperServ", "OperDeny"));
//  entlist_val_ui<mstring> iter = i_OperDeny.end();
    set < OperDeny_Type >::iterator iter = i_OperDeny.end();
    if (!i_OperDeny.empty())
	for (iter = i_OperDeny.begin(); iter != i_OperDeny.end(); iter++)
	    if (entry.Matches(iter->Entry(), true))
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

mstring OperServ::OperDeny_value(const mstring & entry)
{
    FT("OperServ::OperDeny_value", (entry));

    mstring retval;

    MLOCK(("OperServ", "OperDeny"));
//  entlist_val_ui<mstring> iter = OperDeny;
    set < OperDeny_Type >::iterator iter = OperDeny;

    if (OperDeny_find(entry))
	retval = OperDeny->Value();
    OperDeny = iter;
    return retval;
}

bool OperServ::Ignore_insert(const mstring & i_entry, const bool perm, const mstring & nick)
{
    FT("OperServ::Ignore_insert", (i_entry, perm, nick));

    // no @
    if (!i_entry.Contains("@"))
    {
	RET(false);
    }
    mstring entry(i_entry);

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.prepend("*!");

    MLOCK(("OperServ", "Ignore"));
    if (!Ignore_find(entry))
    {
	pair < set < Ignore_Type >::iterator, bool > tmp;
	MCB(i_Ignore.size());
	tmp = i_Ignore.insert(Ignore_Type(entry, perm, nick));
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

size_t OperServ::Ignore_Usage() const
{
    size_t retval = 0;

    set < Ignore_Type >::const_iterator i;
    MLOCK(("OperServ", "Ignore"));
    for (i = i_Ignore.begin(); i != i_Ignore.end(); i++)
    {
	retval += i->Usage();
    }
    return retval;
}

bool OperServ::Ignore_find(const mstring & i_entry)
{
    FT("OperServ::Ignore_find", (i_entry));

    // no @
    if (!i_entry.Contains("@"))
    {
	RET(false);
    }
    mstring entry(i_entry);

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.prepend("*!");

    MLOCK(("OperServ", "Ignore"));
//  entlist_val_ui<pair<mDateTime, bool> > iter = i_Ignore.end();
    set < Ignore_Type >::iterator iter = i_Ignore.end();

    if (!i_Ignore.empty())
	for (iter = i_Ignore.begin(); iter != i_Ignore.end(); iter++)
	{
	    if (entry.Matches(iter->Entry(), true))
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

bool OperServ::Ignore_value(const mstring & entry)
{
    FT("OperServ::Ignore_value", (entry));

    bool retval = false;

    MLOCK(("OperServ", "Ignore"));
//  entlist_val_ui<pair<mDateTime, bool> > iter = Ignore;
    set < Ignore_Type >::iterator iter = Ignore;

    if (Ignore_find(entry))
	retval = Ignore->Value();
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

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* CONF*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_settings_Config);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* NICK*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_settings_Nick);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* CHAN*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_settings_Channel);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* OTH*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_settings_Other);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *ALL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_settings_All);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* ADD*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_clone_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* DEL*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_clone_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* ERA*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_clone_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_clone_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_clone_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL ADD*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_akill_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL DEL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_akill_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL ERA*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_akill_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_akill_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_akill_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* ADD*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_operdeny_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* DEL*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_operdeny_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* ERA*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_operdeny_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* LIST", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_operdeny_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* VIEW", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_operdeny_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* H*LP", Magick::instance().commserv.SOP_Name(),
						 do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* ADD*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_ignore_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* DEL*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_ignore_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* ERA*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_ignore_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_ignore_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_ignore_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_2param);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name(),
						 OperServ::do_Help);
#ifdef MAGICK_TRACE_WORKS
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "TRACE", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Trace);
#endif
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "*MODE*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_Mode);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "Q*LINE*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_Qline);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNQ*LINE*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_UnQline);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "NO*OP*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_NOOP);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "KILL*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_Kill);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "HIDE*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_Hide);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "*PING*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_Ping);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UPD*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Update);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SAVE*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Update);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SHUT*DOWN*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Shutdown);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "DIE*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Shutdown);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "RESTART*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Restart);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "RELOAD*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Reload);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SIGNON*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Signon);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOAD*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Unload);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "JUPE*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_Jupe);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ID*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Identify);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ON", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_On);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "OFF", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Off);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "HTM", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_HTM);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_1_2param);
}

void OperServ::RemCommands()
{
    NFT("OperServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* CONF*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* NICK*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* CHAN*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* OTH*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *ALL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* ADD*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* DEL*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* ERA*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL ADD*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL DEL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL ERA*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* ADD*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* DEL*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* ERA*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* LIST", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* VIEW", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* H*LP", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* ADD*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* DEL*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* ERA*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "TRACE", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "*MODE*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "Q*LINE*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNQ*LINE*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "NO*OP*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "KILL*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "HIDE*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "*PING*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UPD*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SAVE*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SHUT*DOWN*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "DIE*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "RESTART*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "RELOAD*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOAD*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "JUPE*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ID*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ON", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "OFF", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "HTM", Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
}

void OperServ::execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    mThread::ReAttach(tt_OperServ);
    FT("OperServ::execute", (source, msgtype, params));
    //okay this is the main operserv command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message
    mstring mynick(Magick::instance().getLname(params.ExtractWord(1, ": ")));
    mstring message(params.After(":"));
    mstring command(message.Before(" "));

    // We SHOULD still process THESE messages, ONLY.
    if (!MSG())
    {
	if (!(command.IsSameAs("ON", true) || command.IsSameAs("OFF", true) || command.Matches("ID*", true)))
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
    else if (Secure() && !Magick::instance().nickserv.GetLive(source)->HasMode("o"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
    }
    else if (msgtype == "PRIVMSG" && !Magick::instance().commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);
}

void OperServ::do_Help(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Help", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    mstring HelpTopic = Magick::instance().operserv.GetInternalName();

    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector < mstring > help = Magick::instance().getHelp(source, HelpTopic.UpperCase());

    unsigned int i;

    for (i = 0; i < help.size(); i++)
	::send(mynick, source, help[i]);
}

#ifdef MAGICK_TRACE_WORKS
void OperServ::do_Trace(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Trace", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    unsigned int i, j, k;
    mstring output;
    mstring action = params.ExtractWord(2, " ").UpperCase();
    mstring ttype = params.ExtractWord(3, " ").UpperCase();
    threadtype_enum type;

    vector < mstring > levels;
    for (i = 4; i < params.WordCount(" ") + 1; i++)
	levels.push_back(params.ExtractWord(i, " ").UpperCase());

    if (ttype.IsSameAs("ALL", true))
    {
	type = tt_MAX;
	ttype = "ALL";
    }
    else
    {
	for (i = 0 + 1; i < tt_MAX; i++)
	    if (ttype == threadname[i])
		break;
	if (i < tt_MAX)
	{
	    type = static_cast < threadtype_enum > (i);
	}
	else
	{
	    SEND(mynick, source, "OS_STATUS/INVALID_THREAD", (ttype));
	    return;
	}
    }

    if (action.IsSameAs("SET", true))
    {
	if (makehex(levels[0U]) != 0 || levels[0U] == "0" || levels[0U] == "0x0000")
	{
	    if (type == tt_MAX)
	    {
		for (i = 0; i < tt_MAX; i++)
		{
		    Trace::TurnSet(static_cast < threadtype_enum > (i), makehex(levels[0U]));
		    output.Format("%s SET: Trace level set to %#06x.", mDateTime::CurrentDateTime().DateTimeString().c_str(),
				  Trace::TraceLevel(static_cast < threadtype_enum > (i)));
		    {
			MLOCK(("ThreadMessageQueue"));
			ThreadMessageQueue.push_back(pair < threadtype_enum,
						     mstring > (static_cast < threadtype_enum > (i), output));
		    }
		}
	    }
	    else
	    {
		Trace::TurnSet(type, makehex(levels[0U]));
		output.Format("%s SET: Trace level set to %#06x.", mDateTime::CurrentDateTime().DateTimeString().c_str(),
			      Trace::TraceLevel(type));
		{
		    MLOCK(("ThreadMessageQueue"));
		    ThreadMessageQueue.push_back(pair < threadtype_enum, mstring > (type, output));
		}
	    }
	}
	else
	{
	    for (i = 0; i < levels.size(); i++)
	    {
		for (j = 0; j < Trace::levelname.size(); j++)
		{
		    if (levels[i].Matches(Trace::levelname[j].name, true))
		    {
			if (type == tt_MAX)
			{
			    for (k = 0; k < tt_MAX; k++)
			    {
				Trace::TurnSet(static_cast < threadtype_enum > (k), 0U);
				Trace::TurnUp(static_cast < threadtype_enum > (k), Trace::levelname[j].level);
				output.Format("%s SET: Trace level set to %#06x.",
					      mDateTime::CurrentDateTime().DateTimeString().c_str(),
					      Trace::TraceLevel(static_cast < threadtype_enum > (k)));
				{
				    MLOCK(("ThreadMessageQueue"));
				    ThreadMessageQueue.push_back(pair < threadtype_enum,
								 mstring > (static_cast < threadtype_enum > (k), output));
				}
			    }
			}
			else
			{
			    Trace::TurnSet(static_cast < threadtype_enum > (type), 0U);
			    Trace::TurnUp(type, Trace::levelname[j].level);
			    output.Format("%s SET: Trace level set to %#06x.",
					  mDateTime::CurrentDateTime().DateTimeString().c_str(), Trace::TraceLevel(type));
			    {
				MLOCK(("ThreadMessageQueue"));
				ThreadMessageQueue.push_back(pair < threadtype_enum, mstring > (type, output));
			    }
			}
			break;
		    }
		}
		if (j >= Trace::levelname.size())
		    SEND(mynick, source, "OS_STATUS/INVALID_LEVEL", (levels[i]));
	    }
	}
    }
    else if (action.Matches("*UP", true))
    {
	for (i = 0; i < levels.size(); i++)
	{
	    for (j = 0; j < Trace::levelname.size(); j++)
	    {
		if (levels[i].Matches(Trace::levelname[j].name, true))
		{
		    if (type == tt_MAX)
		    {
			for (k = 0; k < tt_MAX; k++)
			{
			    Trace::TurnUp(static_cast < threadtype_enum > (k), Trace::levelname[j].level);
			    output.Format("%s UP: Trace level set to %#06x.",
					  mDateTime::CurrentDateTime().DateTimeString().c_str(),
					  Trace::TraceLevel(static_cast < threadtype_enum > (k)));
			    {
				MLOCK(("ThreadMessageQueue"));
				ThreadMessageQueue.push_back(pair < threadtype_enum,
							     mstring > (static_cast < threadtype_enum > (k), output));
			    }
			}
		    }
		    else
		    {
			Trace::TurnUp(type, Trace::levelname[j].level);
			output.Format("%s UP: Trace level set to %#06x.",
				      mDateTime::CurrentDateTime().DateTimeString().c_str(), Trace::TraceLevel(type));
			{
			    MLOCK(("ThreadMessageQueue"));
			    ThreadMessageQueue.push_back(pair < threadtype_enum, mstring > (type, output));
			}
		    }
		    break;
		}
	    }
	    if (j >= Trace::levelname.size())
		SEND(mynick, source, "OS_STATUS/INVALID_LEVEL", (levels[i]));
	}
    }
    else if (action.Matches("*DOWN", true))
    {
	for (i = 0; i < levels.size(); i++)
	{
	    for (j = 0; j < Trace::levelname.size(); j++)
	    {
		if (levels[i].Matches(Trace::levelname[j].name, true))
		{
		    if (type == tt_MAX)
		    {
			for (k = 0; k < tt_MAX; k++)
			{
			    Trace::TurnDown(static_cast < threadtype_enum > (k), Trace::levelname[j].level);
			    output.Format("%s DOWN: Trace level set to %#06x.",
					  mDateTime::CurrentDateTime().DateTimeString().c_str(),
					  Trace::TraceLevel(static_cast < threadtype_enum > (k)));
			    {
				MLOCK(("ThreadMessageQueue"));
				ThreadMessageQueue.push_back(pair < threadtype_enum,
							     mstring > (static_cast < threadtype_enum > (k), output));
			    }
			}
		    }
		    else
		    {
			Trace::TurnDown(type, Trace::levelname[j].level);
			output.Format("%s DOWN: Trace level set to %#06x.",
				      mDateTime::CurrentDateTime().DateTimeString().c_str(), Trace::TraceLevel(type));
			{
			    MLOCK(("ThreadMessageQueue"));
			    ThreadMessageQueue.push_back(pair < threadtype_enum, mstring > (type, output));
			}
		    }
		    break;
		}
	    }
	    if (j >= Trace::levelname.size())
		SEND(mynick, source, "OS_STATUS/INVALID_LEVEL", (levels[i]));
	}
    }
    else if (action.Matches("VIEW*", true) || action.Matches("LIST*", true))
    {
	// Its taken care of below :)
    }
    else
    {
	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (message + " " + action, mynick, message));
	return;
    }

    mstring line1, line2, tmp;

    for (i = 0; i < tt_MAX; i++)
    {
	tmp.Format("%6s  ", threadname[i].c_str());
	line1 += tmp;
	tmp.Format("%#06x  ", Trace::TraceLevel(static_cast < threadtype_enum > (i)));
	line2 += tmp;
    }
    Magick::instance().operserv.stats.i_Trace++;
    ::send(mynick, source, line1);
    ::send(mynick, source, line2);
    if (!(action.Matches("VIEW*", true) || action.Matches("LIST*", true)))
    {
	NSLOG(LM_NOTICE, line1);
	NSLOG(LM_NOTICE, line2);
    }
}
#endif

void OperServ::do_Mode(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Mode", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring mode = params.After(" ", 2);

    if (IsChan(target))
    {
	if (Magick::instance().chanserv.IsLive(target))
	{
	    Magick::instance().server.MODE(mynick, target, mode);
	    Magick::instance().operserv.stats.i_Mode++;
	    ANNOUNCE(mynick, "MISC/CHAN_MODE", (source, mode, target));
	    SEND(mynick, source, "OS_COMMAND/CHAN_MODE", (mode, target));

	    LOG(LM_INFO, "OPERSERV/MODE",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), mode, target));
	}
	else
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (target));
	}
    }
    else
    {
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	    Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source))
	{
	    if (Magick::instance().nickserv.IsLive(target))
	    {
		if (!Magick::instance().server.proto.SVSMODE().empty())
		{
		    Magick::instance().server.SVSMODE(mynick, target, mode);
		    Magick::instance().operserv.stats.i_Mode++;
		    ANNOUNCE(mynick, "MISC/NICK_MODE", (source, mode, target));
		    SEND(mynick, source, "OS_COMMAND/NICK_MODE", (mode, target));
		    LOG(LM_INFO, "OPERSERV/MODE",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), mode, target));
		}
		else
		{
		    NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
		}
	    }
	    else
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	    }
	}
	else
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	}
    }
}

void OperServ::do_Qline(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Qline", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (Magick::instance().server.proto.SQLINE().empty())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring reason;

    if (params.WordCount(" ") > 2)
	reason = params.After(" ", 2);
    Magick::instance().server.SQLINE(mynick, target, reason);
    Magick::instance().operserv.stats.i_Qline++;
    SEND(mynick, source, "OS_COMMAND/QLINE", (target, Magick::instance().getMessage(source, "VALS/ON")));
    ANNOUNCE(mynick, "MISC/QLINE", (source, Magick::instance().getMessage("VALS/ON"), target));
    LOG(LM_INFO, "OPERSERV/QLINE", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target));
}

void OperServ::do_UnQline(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_UnQline", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (Magick::instance().server.proto.UNSQLINE().empty())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    Magick::instance().server.UNSQLINE(mynick, target);
    Magick::instance().operserv.stats.i_Unqline++;
    SEND(mynick, source, "OS_COMMAND/QLINE", (target, Magick::instance().getMessage(source, "VALS/OFF")));
    ANNOUNCE(mynick, "MISC/QLINE", (source, Magick::instance().getMessage("VALS/OFF"), target));
    LOG(LM_INFO, "OPERSERV/UNQLINE", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target));
}

void OperServ::do_NOOP(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_NOOP", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (Magick::instance().server.proto.SVSNOOP().empty())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(3, " ");

    if (!Magick::instance().server.IsList(target))
    {
	SEND(mynick, source, "OS_STATUS/ISNOTLINKED", (target));
	return;
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Magick::instance().server.SVSNOOP(mynick, target, onoff.GetBool());
    Magick::instance().operserv.stats.i_Noop++;
    SEND(mynick, source, "OS_COMMAND/NOOP",
	 ((onoff.GetBool()? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF")), target));
    ANNOUNCE(mynick, "MISC/NOOP",
	     (source, (onoff.GetBool()? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF")),
	      target));
    LOG(LM_INFO, "OPERSERV/NOOP",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target,
	 (onoff.GetBool()? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void OperServ::do_Kill(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Kill", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (Magick::instance().server.proto.SVSKILL().empty())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring reason = params.After(" ", 2);

    if (Magick::instance().nickserv.IsLive(target))
    {
	Magick::instance().server.SVSKILL(mynick, target, reason);
	Magick::instance().operserv.stats.i_Kill++;
	SEND(mynick, source, "OS_COMMAND/KILL", (target));
	ANNOUNCE(mynick, "MISC/KILL", (source, target));
	LOG(LM_INFO, "OPERSERV/KILL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, reason));
    }
    else
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
    }
}

void OperServ::do_Hide(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Hide", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (Magick::instance().server.proto.SVSHOST().empty())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring newhost = params.ExtractWord(3, " ");

    if (Magick::instance().nickserv.IsLive(target))
    {
	Magick::instance().server.SVSHOST(mynick, target, newhost);
	Magick::instance().operserv.stats.i_Hide++;
	SEND(mynick, source, "OS_COMMAND/HIDE", (target, newhost));
	ANNOUNCE(mynick, "MISC/HIDE", (source, target, newhost));
	LOG(LM_INFO, "OPERSERV/HIDE",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, newhost));
    }
    else
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
    }
}

void OperServ::do_Ping(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Ping", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    {
	RLOCK(("Events"));
	if (Magick::instance().events != NULL)
	{
	    Magick::instance().events->ForcePing();
	    Magick::instance().operserv.stats.i_Ping++;
	    NSEND(mynick, source, "OS_COMMAND/PING");
	    LOG(LM_DEBUG, "OPERSERV/PING", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H)));
	}
    }
}

void OperServ::do_Update(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Update", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("Events"));
	if (Magick::instance().events != NULL)
	{
	    Magick::instance().events->ForceSave();
	    Magick::instance().operserv.stats.i_Update++;
	    NSEND(mynick, source, "OS_COMMAND/UPDATE");
	    LOG(LM_DEBUG, "OPERSERV/UPDATE", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H)));
	}
    }
}

void OperServ::do_Shutdown(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Shutdown", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring reason = params.After(" ");

    NSEND(mynick, source, "OS_COMMAND/SHUTDOWN");
    ANNOUNCE(mynick, "MISC/SHUTDOWN", (source, reason));
    LOG(LM_CRITICAL, "OPERSERV/SHUTDOWN", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), reason));
    Magick::instance().Shutdown(true);
    Magick::instance().Die();
}

void OperServ::do_Restart(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Restart", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring reason = params.After(" ");

    NSEND(mynick, source, "OS_COMMAND/RESTART");
    ANNOUNCE(mynick, "MISC/RESTART", (source, reason));
    LOG(LM_CRITICAL, "OPERSERV/RESTART", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), reason));
    Magick::instance().Die();
}

void OperServ::do_Reload(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Reload", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (Magick::instance().get_config_values())
    {
	Magick::instance().operserv.stats.i_Reload++;
	NSEND(mynick, source, "OS_COMMAND/RELOAD");
	ANNOUNCE(mynick, "MISC/RELOAD", (source));
	LOG(LM_NOTICE, "OPERSERV/RELOAD", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H)));
    }
    else
    {
	LOG(LM_ERROR, "COMMANDLINE/NO_CFG_FILE", (Magick::instance().Config_File()));
	NSEND(mynick, source, "OS_COMMAND/RELOAD_FAIL");
    }
}

void OperServ::do_Signon(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Reload", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    Magick::instance().server.SignOnAll();
    NSEND(mynick, source, "OS_COMMAND/SIGNON");
}

void OperServ::do_Unload(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Unload", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring language = params.ExtractWord(2, " ").UpperCase();

    bool unload1, unload2;

    unload1 = Magick::instance().UnloadExternalMessages(language);
    unload2 = Magick::instance().UnloadHelp(language);
    if (unload1 || unload2)
    {
	Magick::instance().operserv.stats.i_Unload++;
	SEND(mynick, source, "OS_COMMAND/UNLOAD", (language));
	LOG(LM_DEBUG, "OPERSERV/UNLOAD", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), language));
    }
    else
    {
	SEND(mynick, source, "OS_STATUS/ISNOTLANG", (language));
    }
}

void OperServ::do_Jupe(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Jupe", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring reason = params.After(" ", 2);

    if (!target.Contains("."))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", ("JUPE", '.'));
	return;
    }

    Magick::instance().server.Jupe(target, reason);
    Magick::instance().operserv.stats.i_Jupe++;
    SEND(mynick, source, "OS_COMMAND/JUPE", (target));
    ANNOUNCE(mynick, "MISC/JUPE", (source, target));
    LOG(LM_NOTICE, "OPERSERV/JUPE", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, reason));
}

void OperServ::do_On(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_On", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    // Later, make the ability to turn on/off specific services
    // also the ability to turn of either MSG, or AUTO or BOTH

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring type = params.ExtractWord(2, " ");
    mstring service;

    if (type.Matches("AUTO*", true))
    {
	Magick::instance().AUTO(true);
	Magick::instance().operserv.stats.i_OnOff++;
	SEND(mynick, source, "OS_COMMAND/ONOFF",
	     (Magick::instance().getMessage(source, "VALS/SVC_AUTO"), Magick::instance().getMessage(source, "VALS/ON")));
	ANNOUNCE(mynick, "MISC/ONOFF",
		 (Magick::instance().getMessage("VALS/SVC_AUTO"), Magick::instance().getMessage("VALS/ON"), source));
	LOG(LM_NOTICE, "OPERSERV/ONOFF",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("VALS/ON"),
	     Magick::instance().getMessage("VALS/SVC_AUTO")));
    }
    else if (type.Matches("LOG*", true))
    {
	Magick::instance().ActivateLogger();
	Magick::instance().operserv.stats.i_OnOff++;
	SEND(mynick, source, "OS_COMMAND/ONOFF",
	     (Magick::instance().getMessage(source, "VALS/SVC_LOG"), Magick::instance().getMessage(source, "VALS/ON")));
	ANNOUNCE(mynick, "MISC/ONOFF",
		 (Magick::instance().getMessage("VALS/SVC_LOG"), Magick::instance().getMessage("VALS/ON"), source));
	LOG(LM_NOTICE, "OPERSERV/ONOFF",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("VALS/ON"),
	     Magick::instance().getMessage("VALS/SVC_LOG")));
    }
    else if (type.Matches("M*S*G*", true))
    {
	if (params.WordCount(" ") < 3)
	{
	    Magick::instance().MSG(true);
	    Magick::instance().operserv.stats.i_OnOff++;
	    SEND(mynick, source, "OS_COMMAND/ONOFF",
		 (Magick::instance().getMessage(source, "VALS/SVC_MSG"), Magick::instance().getMessage(source, "VALS/ON")));
	    ANNOUNCE(mynick, "MISC/ONOFF",
		     (Magick::instance().getMessage("VALS/SVC_MSG"), Magick::instance().getMessage("VALS/ON"), source));
	    LOG(LM_NOTICE, "OPERSERV/ONOFF",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().getMessage("VALS/ON"), Magick::instance().getMessage("VALS/SVC_MSG")));
	}
	else
	{
	    service = params.ExtractWord(3, " ");
	    if (Magick::instance().operserv.GetInternalName().IsSameAs(service, true))
		Magick::instance().operserv.MSG(true);
	    else if (Magick::instance().nickserv.GetInternalName().IsSameAs(service, true))
		Magick::instance().nickserv.MSG(true);
	    else if (Magick::instance().chanserv.GetInternalName().IsSameAs(service, true))
		Magick::instance().chanserv.MSG(true);
	    else if (Magick::instance().memoserv.GetInternalName().IsSameAs(service, true))
		Magick::instance().memoserv.MSG(true);
	    else if (Magick::instance().commserv.GetInternalName().IsSameAs(service, true))
		Magick::instance().commserv.MSG(true);
	    else if (Magick::instance().servmsg.GetInternalName().IsSameAs(service, true))
		Magick::instance().servmsg.MSG(true);
	    //else
	    //  scripted stuff ...
	    else
	    {
		SEND(mynick, source, "ERR_SITUATION/NOSERVICE", (service));
		service.erase();
	    }
	    if (!service.empty())
	    {
		Magick::instance().operserv.stats.i_OnOff++;
		SEND(mynick, source, "OS_COMMAND/ONOFF_ONE",
		     (Magick::instance().getMessage(source, "VALS/SVC_MSG"), service,
		      Magick::instance().getMessage(source, "VALS/ON")));
		ANNOUNCE(mynick, "MISC/ONOFF_ONE",
			 (Magick::instance().getMessage("VALS/SVC_MSG"), service, Magick::instance().getMessage("VALS/ON"),
			  source));
		LOG(LM_NOTICE, "OPERSERV/ONOFF_ONE",
		    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		     Magick::instance().getMessage("VALS/ON"), Magick::instance().getMessage("VALS/SVC_MSG"), service));
	    }
	}
    }
}

void OperServ::do_Off(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_Off", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    // Later, make the ability to turn on/off specific services
    // also the ability to turn of either MSG, or AUTO or BOTH

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring type = params.ExtractWord(2, " ");
    mstring service;

    if (type.Matches("AUTO*", true))
    {
	Magick::instance().AUTO(false);
	Magick::instance().operserv.stats.i_OnOff++;
	SEND(mynick, source, "OS_COMMAND/ONOFF",
	     (Magick::instance().getMessage(source, "VALS/SVC_AUTO"), Magick::instance().getMessage(source, "VALS/OFF")));
	ANNOUNCE(mynick, "MISC/ONOFF",
		 (Magick::instance().getMessage("VALS/SVC_AUTO"), Magick::instance().getMessage("VALS/OFF"), source));
	LOG(LM_NOTICE, "OPERSERV/ONOFF",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("VALS/OFF"), Magick::instance().getMessage("VALS/SVC_AUTO")));
    }
    else if (type.Matches("LOG*", true))
    {
	Magick::instance().operserv.stats.i_OnOff++;
	SEND(mynick, source, "OS_COMMAND/ONOFF",
	     (Magick::instance().getMessage(source, "VALS/SVC_LOG"), Magick::instance().getMessage(source, "VALS/OFF")));
	ANNOUNCE(mynick, "MISC/ONOFF",
		 (Magick::instance().getMessage("VALS/SVC_LOG"), Magick::instance().getMessage("VALS/OFF"), source));
	LOG(LM_NOTICE, "OPERSERV/ONOFF",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("VALS/OFF"), Magick::instance().getMessage("VALS/SVC_LOG")));
	Magick::instance().DeactivateLogger();
    }
    else if (type.Matches("M*S*G*", true))
    {
	if (params.WordCount(" ") < 3)
	{
	    Magick::instance().MSG(false);
	    Magick::instance().operserv.stats.i_OnOff++;
	    SEND(mynick, source, "OS_COMMAND/ONOFF",
		 (Magick::instance().getMessage(source, "VALS/SVC_MSG"), Magick::instance().getMessage(source, "VALS/OFF")));
	    ANNOUNCE(mynick, "MISC/ONOFF",
		     (Magick::instance().getMessage("VALS/SVC_MSG"), Magick::instance().getMessage("VALS/OFF"), source));
	    LOG(LM_NOTICE, "OPERSERV/ONOFF",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().getMessage("VALS/OFF"), Magick::instance().getMessage("VALS/SVC_MSG")));
	}
	else
	{
	    service = params.ExtractWord(3, " ");
	    if (Magick::instance().operserv.GetInternalName().IsSameAs(service, true))
		Magick::instance().operserv.MSG(false);
	    else if (Magick::instance().nickserv.GetInternalName().IsSameAs(service, true))
		Magick::instance().nickserv.MSG(false);
	    else if (Magick::instance().chanserv.GetInternalName().IsSameAs(service, true))
		Magick::instance().chanserv.MSG(false);
	    else if (Magick::instance().memoserv.GetInternalName().IsSameAs(service, true))
		Magick::instance().memoserv.MSG(false);
	    else if (Magick::instance().commserv.GetInternalName().IsSameAs(service, true))
		Magick::instance().commserv.MSG(false);
	    else if (Magick::instance().servmsg.GetInternalName().IsSameAs(service, true))
		Magick::instance().servmsg.MSG(false);
	    //else
	    //  scripted stuff ...
	    else
	    {
		SEND(mynick, source, "ERR_SITUATION/NOSERVICE", (service));
		service.erase();
	    }
	    if (!service.empty())
	    {
		Magick::instance().operserv.stats.i_OnOff++;
		SEND(mynick, source, "OS_COMMAND/ONOFF_ONE",
		     (Magick::instance().getMessage(source, "VALS/SVC_MSG"), service,
		      Magick::instance().getMessage(source, "VALS/OFF")));
		ANNOUNCE(mynick, "MISC/ONOFF_ONE",
			 (Magick::instance().getMessage("VALS/SVC_MSG"), service, Magick::instance().getMessage("VALS/OFF"),
			  source));
		LOG(LM_NOTICE, "OPERSERV/ONOFF_ONE",
		    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		     Magick::instance().getMessage("VALS/OFF"), Magick::instance().getMessage("VALS/SVC_MSG"), service));
	    }
	}
    }
}

void OperServ::do_HTM(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_HTM", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "OS_STATUS/HTM",
	     (ToHumanSpace(Magick::instance().ircsvchandler->HTM_Threshold()),
	      fmstring("%.1f",
		       static_cast <
		       float >(Magick::instance().ircsvchandler->Average(Magick::instance().ircsvchandler->HTM_Gap())) /
		       static_cast < float >(1024))));
    }
    else
    {
	mstring command = params.ExtractWord(2, " ").UpperCase();

	{
	    RLOCK(("IrcSvcHandler"));
	    if (Magick::instance().ircsvchandler != NULL)
	    {

		if (command.IsSameAs("ON", true))
		{
		    Magick::instance().ircsvchandler->HTM(true);
		    SEND(mynick, source, "OS_COMMAND/HTM", (Magick::instance().getMessage(source, "VALS/ON")));
		    ANNOUNCE(mynick, "MISC/HTM_ON_FORCE", (source));
		    LOG(LM_NOTICE, "OPERSERV/HTM_FORCE",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
			 Magick::instance().getMessage("VALS/ON")));
		}
		else if (command.IsSameAs("OFF", true))
		{
		    Magick::instance().ircsvchandler->HTM(false);
		    SEND(mynick, source, "OS_COMMAND/HTM", (Magick::instance().getMessage(source, "VALS/OFF")));
		    ANNOUNCE(mynick, "MISC/HTM_OFF_FORCE", (source));
		    LOG(LM_NOTICE, "OPERSERV/HTM_FORCE",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
			 Magick::instance().getMessage("VALS/OFF")));
		}
		else if (command.IsSameAs("SET", true))
		{
		    if (params.WordCount(" ") < 3)
		    {
			SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message + " " + command, mynick, message));
			return;
		    }
		    size_t newsize = FromHumanSpace(params.ExtractWord(3, " "));

		    if (!newsize)
		    {
			SEND(mynick, source, "ERR_SYNTAX/MUSTBESIZE", (ToHumanSpace(512)));
			return;
		    }
		    Magick::instance().ircsvchandler->HTM_Threshold(newsize);

		    SEND(mynick, source, "OS_COMMAND/HTM_SET", (ToHumanSpace(newsize)));
		    ANNOUNCE(mynick, "MISC/HTM_SET", (ToHumanSpace(newsize), source));
		    LOG(LM_NOTICE, "OPERSERV/HTM_SET",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), ToHumanSpace(newsize)));
		}
		else
		{
		    SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (message + " " + command, mynick, message));
		    return;
		}

	    }
	}
    }
}

void OperServ::do_settings_Config(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_settings_Config", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

/*
-   Databases are (not) encrypted, and compressed at level ?.
-   Minimum threads active is ?, Current threads active is ?.
-   New thread will spawn each ? messages, and die when below ?.
*/
    SEND(mynick, source, "OS_SETTINGS/CFG_LEVEL", (Magick::instance().startup.Level(), Magick::instance().Level()));
    SEND(mynick, source, "OS_SETTINGS/CFG_LAG", (ToHumanTime(Magick::instance().startup.Lagtime(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_LAGCHECK", (ToHumanTime(Magick::instance().config.Ping_Frequency(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_SERVERS", (Magick::instance().startup.Server_size()));
    SEND(mynick, source, "OS_SETTINGS/CFG_RELINK", (ToHumanTime(Magick::instance().config.Server_Relink(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_SQUIT1", (ToHumanTime(Magick::instance().config.Squit_Protect(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_SQUIT2", (ToHumanTime(Magick::instance().config.Squit_Cancel(), source)));
    {
	RLOCK(("Events"));
	if (Magick::instance().events != NULL)
	    SEND(mynick, source, "OS_SETTINGS/CFG_SYNC",
		 (ToHumanTime(Magick::instance().config.Savetime(), source), Magick::instance().events->SyncTime(source)));
    }
    SEND(mynick, source, "OS_SETTINGS/CFG_CYCLE",
	 (ToHumanTime(Magick::instance().config.Cycletime(), source),
	  ToHumanTime(Magick::instance().config.Checktime(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_DCC1",
	 (ToHumanSpace(Magick::instance().files.Blocksize()), ToHumanTime(Magick::instance().files.Timeout(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_DCC2",
	 (ToHumanSpace(Magick::instance().files.Min_Speed()), ToHumanSpace(Magick::instance().files.Max_Speed()),
	  ToHumanTime(Magick::instance().files.Sampletime(), source)));
}

void OperServ::do_settings_Nick(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_settings_Nick", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    SEND(mynick, source, "OS_SETTINGS/NICK_EXPIRE", (ToHumanTime(Magick::instance().nickserv.Expire(), source)));
    SEND(mynick, source, "OS_SETTINGS/NICK_IDENT", (ToHumanTime(Magick::instance().nickserv.Ident(), source)));
    SEND(mynick, source, "OS_SETTINGS/NICK_HOLD", (ToHumanTime(Magick::instance().nickserv.Release(), source)));
    SEND(mynick, source, "OS_SETTINGS/NICK_PASS", (Magick::instance().nickserv.Passfail()));

    mstring output;

    if (Magick::instance().nickserv.DEF_Protect())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_Protect())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/PROTECT");
	if (Magick::instance().nickserv.LCK_Protect())
	    output << IRC_Off;
    }

    if (Magick::instance().nickserv.DEF_Secure())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_Secure())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/SECURE");
	if (Magick::instance().nickserv.LCK_Secure())
	    output << IRC_Off;
    }

    if (Magick::instance().nickserv.DEF_NoExpire())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_NoExpire())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/NOEXPIRE");
	if (Magick::instance().nickserv.LCK_NoExpire())
	    output << IRC_Off;
    }

    if (Magick::instance().nickserv.DEF_NoMemo())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_NoMemo())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/NOMEMO");
	if (Magick::instance().nickserv.LCK_NoMemo())
	    output << IRC_Off;
    }

    if (Magick::instance().nickserv.DEF_Private())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_Private())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/PRIVATE");
	if (Magick::instance().nickserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Magick::instance().nickserv.DEF_PRIVMSG())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_PRIVMSG())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/PRIVMSG");
	if (Magick::instance().nickserv.LCK_PRIVMSG())
	    output << IRC_Off;
    }

    SEND(mynick, source, "OS_SETTINGS/NICK_OPTIONS", (output));

    output.erase();
    if (Magick::instance().nickserv.LCK_Language())
	output << IRC_Bold;
    output << Magick::instance().nickserv.DEF_Language();
    if (Magick::instance().nickserv.LCK_Language())
	output << IRC_Off;

    SEND(mynick, source, "OS_SETTINGS/NICK_LANG", (output));

    SEND(mynick, source, "OS_SETTINGS/NICK_PICSIZE", (ToHumanSpace(Magick::instance().nickserv.PicSize())));
    SEND(mynick, source, "OS_SETTINGS/NICK_PICEXT", (Magick::instance().nickserv.PicExt()));
    SEND(mynick, source, "OS_SETTINGS/NICK_FILES",
	 (Magick::instance().memoserv.Files(), ToHumanSpace(Magick::instance().memoserv.FileSize())));
}

void OperServ::do_settings_Channel(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_settings_Channel", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    SEND(mynick, source, "OS_SETTINGS/CHAN_EXPIRE", (ToHumanTime(Magick::instance().chanserv.Expire(), source)));
    SEND(mynick, source, "OS_SETTINGS/CHAN_IDENT", (Magick::instance().chanserv.Passfail()));
    SEND(mynick, source, "OS_SETTINGS/CHAN_KEEPTIME", (ToHumanTime(Magick::instance().chanserv.ChanKeep(), source)));

    mstring output;

    if (Magick::instance().chanserv.LCK_Bantime())
	output << IRC_Bold;
    output << ToHumanTime(Magick::instance().chanserv.DEF_Bantime(), source);
    if (Magick::instance().chanserv.LCK_Bantime())
	output << IRC_Off;
    SEND(mynick, source, "OS_SETTINGS/CHAN_BANTIME", (output));
    output.erase();
    if (Magick::instance().chanserv.LCK_Parttime())
	output << IRC_Bold;
    output << ToHumanTime(Magick::instance().chanserv.DEF_Parttime(), source);
    if (Magick::instance().chanserv.LCK_Parttime())
	output << IRC_Off;
    SEND(mynick, source, "OS_SETTINGS/CHAN_PARTTIME", (output));

    SEND(mynick, source, "OS_SETTINGS/CHAN_MLOCK",
	 (Magick::instance().chanserv.DEF_MLock(), Magick::instance().chanserv.LCK_MLock()));

    output.erase();
    if (Magick::instance().chanserv.DEF_Keeptopic())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Keeptopic())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/KEEPTOPIC");
	if (Magick::instance().chanserv.LCK_Keeptopic())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Topiclock())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Topiclock())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/TOPICLOCK");
	if (Magick::instance().chanserv.LCK_Topiclock())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Private())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Private())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/PRIVATE");
	if (Magick::instance().chanserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Secureops())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Secureops())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/SECUREOPS");
	if (Magick::instance().chanserv.LCK_Secureops())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Secure())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Secure())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/SECURE");
	if (Magick::instance().chanserv.LCK_Secure())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_NoExpire())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_NoExpire())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/NOEXPIRE");
	if (Magick::instance().chanserv.LCK_NoExpire())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Anarchy())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Anarchy())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/ANARCHY");
	if (Magick::instance().chanserv.LCK_Anarchy())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_KickOnBan())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_KickOnBan())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/KICKONBAN");
	if (Magick::instance().chanserv.LCK_KickOnBan())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Restricted())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Restricted())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/RESTRICTED");
	if (Magick::instance().chanserv.LCK_Restricted())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Join())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Join())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/JOIN");
	if (Magick::instance().chanserv.LCK_Join())
	    output << IRC_Off;
    }

    SEND(mynick, source, "OS_SETTINGS/CHAN_OPTIONS", (output));

    output.erase();
    if (Magick::instance().chanserv.LCK_Revenge())
	output << IRC_Bold;
    output << Magick::instance().chanserv.DEF_Revenge();
    if (Magick::instance().chanserv.LCK_Revenge())
	output << IRC_Off;
    SEND(mynick, source, "OS_SETTINGS/CHAN_REVENGE", (output));

    SEND(mynick, source, "OS_SETTINGS/CHAN_ACCESS",
	 (Magick::instance().chanserv.Level_Min(), Magick::instance().chanserv.Level_Max()));
    SEND(mynick, source, "OS_SETTINGS/CHAN_NEWS", (ToHumanTime(Magick::instance().memoserv.News_Expire(), source)));
}

void OperServ::do_settings_Other(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_settings_Other", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    SEND(mynick, source, "OS_SETTINGS/MISC_INFLIGHT", (ToHumanTime(Magick::instance().memoserv.InFlight(), source)));
    SEND(mynick, source, "OS_SETTINGS/MISC_AKILL1",
	 (ToHumanTime(Magick::instance().operserv.Def_Expire(), source),
	  fmstring("%.2f", Magick::instance().operserv.Akill_Reject())));
    NSEND(mynick, source, "OS_SETTINGS/MISC_AKILL2");
    ::sendV(mynick, source, "%-20s: %s", Magick::instance().commserv.SADMIN_Name().c_str(),
	    ToHumanTime(Magick::instance().operserv.Expire_SAdmin(), source).c_str());
    ::sendV(mynick, source, "%-20s: %s", Magick::instance().commserv.SOP_Name().c_str(),
	    ToHumanTime(Magick::instance().operserv.Expire_Sop(), source).c_str());
    ::sendV(mynick, source, "%-20s: %s", Magick::instance().commserv.ADMIN_Name().c_str(),
	    ToHumanTime(Magick::instance().operserv.Expire_Admin(), source).c_str());
    ::sendV(mynick, source, "%-20s: %s", Magick::instance().commserv.OPER_Name().c_str(),
	    ToHumanTime(Magick::instance().operserv.Expire_Oper(), source).c_str());
    SEND(mynick, source, "OS_SETTINGS/MISC_CLONES",
	 (Magick::instance().operserv.Clone_Limit(), Magick::instance().operserv.Max_Clone()));
    SEND(mynick, source, "OS_SETTINGS/MISC_FLOOD1",
	 (Magick::instance().operserv.Flood_Msgs(), ToHumanTime(Magick::instance().operserv.Flood_Time(), source)));
    SEND(mynick, source, "OS_SETTINGS/MISC_FLOOD2", (ToHumanTime(Magick::instance().operserv.Ignore_Remove(), source)));
    SEND(mynick, source, "OS_SETTINGS/MISC_IGNORE",
	 (ToHumanTime(Magick::instance().operserv.Ignore_Time(), source), Magick::instance().operserv.Ignore_Limit()));
    mstring output;

    if (Magick::instance().commserv.DEF_OpenMemos())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().commserv.LCK_OpenMemos())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS");
	if (Magick::instance().commserv.LCK_OpenMemos())
	    output << IRC_Off;
    }

    if (Magick::instance().commserv.DEF_Private())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().commserv.LCK_Private())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE");
	if (Magick::instance().commserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Magick::instance().commserv.DEF_Secure())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().commserv.LCK_Secure())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE");
	if (Magick::instance().commserv.LCK_Secure())
	    output << IRC_Off;
    }
    SEND(mynick, source, "OS_SETTINGS/MISC_COMM_OPT", (output));
}

void OperServ::do_settings_All(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_settings_All", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    do_settings_Config(mynick, source, params);
    do_settings_Nick(mynick, source, params);
    do_settings_Channel(mynick, source, params);
    do_settings_Other(mynick, source, params);
}

void OperServ::do_clone_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_clone_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 5)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();
    mstring amount = params.ExtractWord(4, " ");
    mstring reason = params.After(" ", 4);

    if (host.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '!'));
	return;
    }
    else if (host.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '@'));
	return;
    }

    if (!amount.IsNumber() || amount.Contains("."))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }

    unsigned int i, num;
    bool super = (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source));
    // i+1 below because unsigned i will always be >= 0
    for (i = host.size() - 1, num = 0; i + 1 > 0; i--)
    {
	switch (host[i])
	{
	case '@':
	    if (!super)
		i = 0;
	    break;
	case '!':		// ALL these constitute wildcards.
	case '*':
	case '?':
	case '.':
	    break;
	default:
	    num++;
	}
    }
    // IF we have less than 1 char for 
    if (!super && num <= Magick::instance().config.Starthresh())
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH",
	     (Magick::instance().getMessage(source, "LIST/CLONE"), Magick::instance().config.Starthresh()));
	return;
    }
    else if (num <= 1)
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH", (Magick::instance().getMessage(source, "LIST/CLONE"), 1));
	return;
    }

    num = atoi(amount.c_str());
    if (num < 1 || num > Magick::instance().operserv.Max_Clone())
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().operserv.Max_Clone()));
	return;
    }

    MLOCK(("OperServ", "Clone"));
    if (Magick::instance().operserv.Clone_find(host))
    {
	mstring entry = Magick::instance().operserv.Clone->Entry();

	Magick::instance().operserv.Clone_erase();
	Magick::instance().operserv.Clone_insert(entry, num, reason, source);
	Magick::instance().operserv.stats.i_Clone++;
	SEND(mynick, source, "LIST/CHANGE_LEVEL", (entry, Magick::instance().getMessage(source, "LIST/CLONE"), num));
	LOG(LM_DEBUG, "OPERSERV/CLONE_ADD",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), entry, num));
    }
    else
    {
	Magick::instance().operserv.Clone_insert(host, num, reason, source);
	Magick::instance().operserv.stats.i_Clone++;
	SEND(mynick, source, "LIST/ADD_LEVEL", (host, Magick::instance().getMessage(source, "LIST/CLONE"), num));
	LOG(LM_DEBUG, "OPERSERV/CLONE_ADD",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), host, num));
    }
}

void OperServ::do_clone_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_clone_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();

    if (host.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '!'));
	return;
    }
    else if (host.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '@'));
	return;
    }

    MLOCK(("OperServ", "Clone"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int i, num = atoi(host.c_str());

	if (num <= 0 || num > Magick::instance().operserv.Clone_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().operserv.Clone_size()));
	    return;
	}

	for (i = 1, Magick::instance().operserv.Clone = Magick::instance().operserv.Clone_begin();
	     i < num && Magick::instance().operserv.Clone != Magick::instance().operserv.Clone_end();
	     i++, Magick::instance().operserv.Clone++);
	if (Magick::instance().operserv.Clone != Magick::instance().operserv.Clone_end())
	{
	    Magick::instance().operserv.stats.i_Clone++;
	    SEND(mynick, source, "LIST/DEL",
		 (Magick::instance().operserv.Clone->Entry(), Magick::instance().getMessage(source, "LIST/CLONE")));
	    LOG(LM_DEBUG, "OPERSERV/CLONE_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.Clone->Entry()));
	    Magick::instance().operserv.Clone_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS_NUMBER", (num, Magick::instance().getMessage(source, "LIST/CLONE")));
	}
    }
    else
    {
	int count = 0;

	while (Magick::instance().operserv.Clone_find(host))
	{
	    LOG(LM_DEBUG, "OPERSERV/CLONE_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.Clone->Entry()));
	    Magick::instance().operserv.Clone_erase();
	    count++;
	}

	if (count)
	{
	    Magick::instance().operserv.stats.i_Clone++;
	    SEND(mynick, source, "LIST/DEL_MATCH", (count, host, Magick::instance().getMessage(source, "LIST/CLONE")));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS", (host, Magick::instance().getMessage(source, "LIST/CLONE")));
	}
    }
}

void OperServ::do_clone_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_clone_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = "*";

    if (params.WordCount(" ") > 2)
    {
	host = params.ExtractWord(3, " ").LowerCase();

	if (host.Contains("!"))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '!'));
	    return;
	}
	else if (host.Contains("@"))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '@'));
	    return;
	}
    }

    if (Magick::instance().operserv.Clone_size())
    {
	SEND(mynick, source, "LIST/DISPLAY_MATCH", (host, Magick::instance().getMessage(source, "LIST/CLONE")));
    }
    else
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/CLONE")));
	return;
    }
    unsigned int i = 1;

    MLOCK(("OperServ", "Clone"));
    for (Magick::instance().operserv.Clone = Magick::instance().operserv.Clone_begin();
	 Magick::instance().operserv.Clone != Magick::instance().operserv.Clone_end(); Magick::instance().operserv.Clone++)
    {
	if (Magick::instance().operserv.Clone->Entry().Matches(host, true))
	{
	    ::sendV(mynick, source, "%3d. %s (%s)", i, Magick::instance().operserv.Clone->Entry().c_str(),
		    parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
				 mVarArray(Magick::instance().operserv.Clone->Last_Modify_Time().Ago(),
					   Magick::instance().operserv.Clone->Last_Modifier())).c_str());
	    ::sendV(mynick, source, "     [%4d] %s", Magick::instance().operserv.Clone->Value().first,
		    Magick::instance().operserv.Clone->Value().second.c_str());
	    i++;
	}
    }
}

void OperServ::do_akill_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_akill_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();
    mstring reason = params.After(" ", 3);
    unsigned long expire = FromHumanTime(reason.Before(" "));

    if (expire)
    {
	if (params.WordCount(" ") < 5)
	{
	    SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	    return;
	}

	reason = reason.After(" ");
    }
    else
	expire = Magick::instance().operserv.Def_Expire();

    if (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source))
    {
	if (expire > Magick::instance().operserv.Expire_SAdmin())
	{
	    SEND(mynick, source, "ERR_SITUATION/AKILLTOOHIGH",
		 (ToHumanTime(Magick::instance().operserv.Expire_SAdmin(), source)));
	    return;
	}
    }
    else if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	     Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source))
    {
	if (expire > Magick::instance().operserv.Expire_Sop())
	{
	    SEND(mynick, source, "ERR_SITUATION/AKILLTOOHIGH",
		 (ToHumanTime(Magick::instance().operserv.Expire_Sop(), source)));
	    return;
	}
    }
    else if (Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
	     Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->IsOn(source))
    {
	if (expire > Magick::instance().operserv.Expire_Admin())
	{
	    SEND(mynick, source, "ERR_SITUATION/AKILLTOOHIGH",
		 (ToHumanTime(Magick::instance().operserv.Expire_Admin(), source)));
	    return;
	}
    }
    else if (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	     Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsOn(source))
    {
	if (expire > Magick::instance().operserv.Expire_Oper())
	{
	    SEND(mynick, source, "ERR_SITUATION/AKILLTOOHIGH",
		 (ToHumanTime(Magick::instance().operserv.Expire_Oper(), source)));
	    return;
	}
    }

    if (host.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/AKILL"), '!'));
	return;
    }

    if (!host.Contains("@"))
    {
	host.prepend("*@");
    }

    unsigned int num;
    int i;
    bool super = (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source));
    for (i = host.size() - 1, num = 0; i >= 0; i--)
    {
	switch (host[static_cast < size_t > (i)])
	{
	case '@':
	    if (!super)
		i = 0;
	    break;
	case '!':		// ALL these constitute wildcards.
	case '*':
	case '?':
	case '.':
	    break;
	default:
	    num++;
	}
    }
    // IF we have less than 1 char for 
    if (!super && num <= Magick::instance().config.Starthresh())
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH",
	     (Magick::instance().getMessage(source, "LIST/AKILL"), Magick::instance().config.Starthresh()));
	return;
    }
    else if (num <= 1)
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH", (Magick::instance().getMessage(source, "LIST/AKILL"), 1));
	return;
    }

    {
	MLOCK(("OperServ", "Akill"));
	if (Magick::instance().operserv.Akill_find(host))
	{
	    mstring entry = Magick::instance().operserv.Akill->Entry();

	    Magick::instance().operserv.Akill_erase();
	    Magick::instance().operserv.Akill_insert(entry, expire, reason, source);
	    Magick::instance().operserv.stats.i_Akill++;
	    SEND(mynick, source, "LIST/CHANGE_TIME",
		 (entry, Magick::instance().getMessage(source, "LIST/AKILL"), ToHumanTime(expire, source)));
	    ANNOUNCE(mynick, "MISC/AKILL_EXTEND", (source, entry, ToHumanTime(expire, source)));
	    LOG(LM_INFO, "OPERSERV/AKILL_ADD",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), entry, ToHumanTime(expire, source),
		 reason));
	}
	else
	{
	    NickServ::live_t::iterator iter;
	    vector < mstring > killusers;
	    {
		RLOCK(("NickServ", "live"));
		for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
		{
		    map_entry < Nick_Live_t > nlive(iter->second);
		    if (nlive->Mask(Nick_Live_t::N_U_P_H).After("!").Matches(host, true))
			killusers.push_back(iter->first);
		}
	    }

	    float percent =
		100.0 * static_cast < float >(killusers.size()) / static_cast <
		float >(Magick::instance().nickserv.LiveSize());
	    if (percent > Magick::instance().operserv.Akill_Reject() &&
		!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source)))
	    {
		SEND(mynick, source, "ERR_SITUATION/AKILLTOOMANY",
		     (fmstring("%.2f", percent), fmstring("%.2f", Magick::instance().operserv.Akill_Reject())));
	    }
	    else
	    {
		Magick::instance().operserv.Akill_insert(host, expire, reason, source);
		Magick::instance().server.AKILL(host, reason, expire, source);
		Magick::instance().operserv.stats.i_Akill++;
		SEND(mynick, source, "LIST/ADD_TIME",
		     (host, Magick::instance().getMessage(source, "LIST/AKILL"), ToHumanTime(expire, source)));
		ANNOUNCE(mynick, "MISC/AKILL_ADD",
			 (source, host, ToHumanTime(expire, source), reason, killusers.size(), fmstring("%.2f", percent)));
		LOG(LM_INFO, "OPERSERV/AKILL_ADD",
		    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), host,
		     ToHumanTime(expire, source), reason));
	    }
	}
    }
}

void OperServ::do_akill_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_akill_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();

    if (host.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/AKILL"), '!'));
	return;
    }

    MLOCK(("OperServ", "Akill"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int i, num = atoi(host.c_str());

	if (num <= 0 || num > Magick::instance().operserv.Akill_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().operserv.Akill_size()));
	    return;
	}

	for (i = 1, Magick::instance().operserv.Akill = Magick::instance().operserv.Akill_begin();
	     i < num && Magick::instance().operserv.Akill != Magick::instance().operserv.Akill_end();
	     i++, Magick::instance().operserv.Akill++);
	if (Magick::instance().operserv.Akill != Magick::instance().operserv.Akill_end())
	{
	    Magick::instance().operserv.stats.i_Akill++;
	    Magick::instance().server.RAKILL(Magick::instance().operserv.Akill->Entry());
	    SEND(mynick, source, "LIST/DEL",
		 (Magick::instance().operserv.Akill->Entry(), Magick::instance().getMessage(source, "LIST/AKILL")));
	    LOG(LM_INFO, "OPERSERV/AKILL_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.Akill->Entry()));
	    Magick::instance().operserv.Akill_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS_NUMBER", (num, Magick::instance().getMessage(source, "LIST/AKILL")));
	}
    }
    else
    {
	int count = 0;

	while (Magick::instance().operserv.Akill_find(host))
	{
	    Magick::instance().server.RAKILL(Magick::instance().operserv.Akill->Entry());
	    LOG(LM_INFO, "OPERSERV/AKILL_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.Akill->Entry()));
	    Magick::instance().operserv.Akill_erase();
	    count++;
	}

	if (count)
	{
	    Magick::instance().operserv.stats.i_Akill++;
	    SEND(mynick, source, "LIST/DEL_MATCH", (count, host, Magick::instance().getMessage(source, "LIST/AKILL")));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS", (host, Magick::instance().getMessage(source, "LIST/AKILL")));
	}
    }
}

void OperServ::do_akill_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_akill_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = "*";

    if (params.WordCount(" ") > 2)
    {
	host = params.ExtractWord(3, " ").LowerCase();

	if (host.Contains("!"))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/AKILL"), '!'));
	    return;
	}
    }

    if (Magick::instance().operserv.Akill_size())
    {
	SEND(mynick, source, "LIST/DISPLAY_MATCH", (host, Magick::instance().getMessage(source, "LIST/AKILL")));
    }
    else
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/AKILL")));
	return;
    }
    unsigned int i = 1;

    MLOCK(("OperServ", "Akill"));
    for (Magick::instance().operserv.Akill = Magick::instance().operserv.Akill_begin();
	 Magick::instance().operserv.Akill != Magick::instance().operserv.Akill_end(); Magick::instance().operserv.Akill++)
    {
	if (Magick::instance().operserv.Akill->Entry().Matches(host, true))
	{
	    ::sendV(mynick, source, "%3d. %s (%s)", i, Magick::instance().operserv.Akill->Entry().c_str(),
		    parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
				 mVarArray(Magick::instance().operserv.Akill->Last_Modify_Time().Ago(),
					   Magick::instance().operserv.Akill->Last_Modifier())).c_str());
	    ::sendV(mynick, source, "     [%s] %s",
		    ToHumanTime(Magick::instance().operserv.Akill->Value().first, source).c_str(),
		    Magick::instance().operserv.Akill->Value().second.c_str());
	    i++;
	}
    }
}

void OperServ::do_operdeny_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_operdeny_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();
    mstring reason = params.After(" ", 3);

    if (!host.Contains("@"))
    {
	if (!Magick::instance().nickserv.IsLive(host))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
	    return;
	}
	host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
    }
    else if (!host.Contains("!"))
    {
	host.prepend("*!");
    }

    unsigned int i, num;

    // i+1 below because unsigned i will always be >= 0
    for (i = host.size() - 1, num = 0; i + 1 > 0; i--)
    {
	switch (host[i])
	{
	case '@':
	case '!':		// ALL these constitute wildcards.
	case '*':
	case '?':
	case '.':
	    break;
	default:
	    num++;
	}
    }

    // IF we have less than 1 char for 
    if (num <= Magick::instance().config.Starthresh())
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH",
	     (Magick::instance().getMessage(source, "LIST/OPERDENY"), Magick::instance().config.Starthresh()));
	return;
    }

    {
	MLOCK(("OperServ", "OperDeny"));
	if (Magick::instance().operserv.OperDeny_find(host))
	{
	    Magick::instance().operserv.OperDeny_erase();
	}
	Magick::instance().operserv.OperDeny_insert(host, reason, source);
    }
    Magick::instance().operserv.stats.i_OperDeny++;
    SEND(mynick, source, "LIST/ADD", (host, Magick::instance().getMessage(source, "LIST/OPERDENY")));
    ANNOUNCE(mynick, "MISC/OPERDENY_ADD", (source, host));
    LOG(LM_NOTICE, "OPERSERV/OPERDENY_ADD",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), host, reason));

    NickServ::live_t::iterator iter;
    vector < mstring > killusers;
    {
	map_entry < Committee_t > sadmin;
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()))
	    sadmin = Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name());
	RLOCK(("NickServ", "live"));
	for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
	{
	    map_entry < Nick_Live_t > nlive(iter->second);
	    if (nlive->Mask(Nick_Live_t::N_U_P_H).Matches(host, true))
	    {
		// IF user is recognized and on sadmin, ignore.
		if (Magick::instance().nickserv.IsStored(iter->first) &&
		    Magick::instance().nickserv.GetStored(iter->first)->IsOnline() && sadmin.entry() != NULL &&
		    sadmin->IsIn(iter->first))
		    continue;
		if (!Magick::instance().server.proto.SVSMODE().empty())
		{
		    nlive->SendMode("-oAa");
		}
		else
		{
		    killusers.push_back(iter->first);
		}
	    }
	}
    }
    for (i = 0; i < killusers.size(); i++)
	Magick::instance().server.KILL(Magick::instance().operserv.FirstName(), killusers[i],
				       Magick::instance().getMessage("MISC/KILL_OPERDENY"));

}

void OperServ::do_operdeny_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_operdeny_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();

    MLOCK(("OperServ", "OperDeny"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int i, num = atoi(host.c_str());

	if (num <= 0 || num > Magick::instance().operserv.OperDeny_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().operserv.OperDeny_size()));
	    return;
	}

	for (i = 1, Magick::instance().operserv.OperDeny = Magick::instance().operserv.OperDeny_begin();
	     i < num && Magick::instance().operserv.OperDeny != Magick::instance().operserv.OperDeny_end();
	     i++, Magick::instance().operserv.OperDeny++);
	if (Magick::instance().operserv.OperDeny != Magick::instance().operserv.OperDeny_end())
	{
	    Magick::instance().operserv.stats.i_OperDeny++;
	    SEND(mynick, source, "LIST/DEL",
		 (Magick::instance().operserv.OperDeny->Entry(), Magick::instance().getMessage(source, "LIST/OPERDENY")));
	    LOG(LM_NOTICE, "OPERSERV/OPERDENY_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.OperDeny->Entry()));
	    Magick::instance().operserv.OperDeny_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS_NUMBER", (num, Magick::instance().getMessage(source, "LIST/OPERDENY")));
	}
    }
    else
    {
	if (!host.Contains("@"))
	{
	    if (!Magick::instance().nickserv.IsLive(host))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
		return;
	    }
	    host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.prepend("*!");
	}

	int count = 0;

	while (Magick::instance().operserv.OperDeny_find(host))
	{
	    LOG(LM_NOTICE, "OPERSERV/OPERDENY_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.OperDeny->Entry()));
	    Magick::instance().operserv.OperDeny_erase();
	    count++;
	}

	if (count)
	{
	    Magick::instance().operserv.stats.i_OperDeny++;
	    SEND(mynick, source, "LIST/DEL_MATCH", (count, host, Magick::instance().getMessage(source, "LIST/OPERDENY")));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS", (host, Magick::instance().getMessage(source, "LIST/OPERDENY")));
	}
    }
}

void OperServ::do_operdeny_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_operdeny_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = "*";

    if (params.WordCount(" ") > 2)
    {
	host = params.ExtractWord(3, " ").LowerCase();

	if (!host.Contains("@"))
	{
	    if (!Magick::instance().nickserv.IsLive(host))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
		return;
	    }
	    host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.prepend("*!");
	}
    }

    if (Magick::instance().operserv.OperDeny_size())
    {
	SEND(mynick, source, "LIST/DISPLAY_MATCH", (host, Magick::instance().getMessage(source, "LIST/OPERDENY")));
    }
    else
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/OPERDENY")));
	return;
    }
    unsigned int i = 1;

    MLOCK(("OperServ", "OperDeny"));
    for (Magick::instance().operserv.OperDeny = Magick::instance().operserv.OperDeny_begin();
	 Magick::instance().operserv.OperDeny != Magick::instance().operserv.OperDeny_end();
	 Magick::instance().operserv.OperDeny++)
    {
	if (Magick::instance().operserv.OperDeny->Entry().Matches(host, true))
	{
	    ::sendV(mynick, source, "%3d. %s (%s)", i, Magick::instance().operserv.OperDeny->Entry().c_str(),
		    parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
				 mVarArray(Magick::instance().operserv.OperDeny->Last_Modify_Time().Ago(),
					   Magick::instance().operserv.OperDeny->Last_Modifier())).c_str());
	    ::sendV(mynick, source, "     %s", Magick::instance().operserv.OperDeny->Value().c_str());
	    i++;
	}
    }
}

void OperServ::do_ignore_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_ignore_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();

    if (!host.Contains("@"))
    {
	if (!Magick::instance().nickserv.IsLive(host))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
	    return;
	}
	host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
    }
    else if (!host.Contains("!"))
    {
	host.prepend("*!");
    }

    unsigned int i, num;
    bool super = (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source));
    // i+1 below because unsigned i will always be >= 0
    for (i = host.size() - 1, num = 0; i + 1 > 0; i--)
    {
	switch (host[i])
	{
	case '@':
	case '!':		// ALL these constitute wildcards.
	case '*':
	case '?':
	case '.':
	    break;
	default:
	    num++;
	}
    }
    // IF we have less than 1 char for 
    if (!super && num <= Magick::instance().config.Starthresh())
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH",
	     (Magick::instance().getMessage(source, "LIST/SIGNORE"), Magick::instance().config.Starthresh()));
	return;
    }
    else if (num <= 1)
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH", (Magick::instance().getMessage(source, "LIST/SIGNORE"), 1));
	return;
    }

    MLOCK(("OperServ", "Ignore"));
    if (Magick::instance().operserv.Ignore_find(host))
    {
	Magick::instance().operserv.Ignore_erase();
    }
    Magick::instance().operserv.Ignore_insert(host, true, source);
    Magick::instance().operserv.stats.i_Ignore++;
    SEND(mynick, source, "LIST/ADD", (host, Magick::instance().getMessage(source, "LIST/SIGNORE")));
    LOG(LM_DEBUG, "OPERSERV/IGNORE_ADD", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), host));
}

void OperServ::do_ignore_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_ignore_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();

    MLOCK(("OperServ", "Ignore"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int i, num = atoi(host.c_str());

	if (num <= 0 || num > Magick::instance().operserv.Ignore_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().operserv.Ignore_size()));
	    return;
	}

	for (i = 1, Magick::instance().operserv.Ignore = Magick::instance().operserv.Ignore_begin();
	     i < num && Magick::instance().operserv.Ignore != Magick::instance().operserv.Ignore_end();
	     i++, Magick::instance().operserv.Ignore++);
	if (Magick::instance().operserv.Ignore != Magick::instance().operserv.Ignore_end())
	{
	    Magick::instance().operserv.stats.i_Ignore++;
	    SEND(mynick, source, "LIST/DEL",
		 (Magick::instance().operserv.Ignore->Entry(), Magick::instance().getMessage(source, "LIST/SIGNORE")));
	    LOG(LM_DEBUG, "OPERSERV/IGNORE_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.Ignore->Entry()));
	    Magick::instance().operserv.Ignore_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS_NUMBER", (num, Magick::instance().getMessage(source, "LIST/SIGNORE")));
	}
    }
    else
    {
	if (!host.Contains("@"))
	{
	    if (!Magick::instance().nickserv.IsLive(host))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
		return;
	    }
	    host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.prepend("*!");
	}

	int count = 0;

	while (Magick::instance().operserv.Ignore_find(host))
	{
	    LOG(LM_DEBUG, "OPERSERV/IGNORE_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.Ignore->Entry()));
	    Magick::instance().operserv.Ignore_erase();
	    count++;
	}

	if (count)
	{
	    Magick::instance().operserv.stats.i_Ignore++;
	    SEND(mynick, source, "LIST/DEL_MATCH", (count, host, Magick::instance().getMessage(source, "LIST/SIGNORE")));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS", (host, Magick::instance().getMessage(source, "LIST/SIGNORE")));
	}
    }
}

void OperServ::do_ignore_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("OperServ::do_ignore_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = "*";

    if (params.WordCount(" ") > 2)
    {
	host = params.ExtractWord(3, " ").LowerCase();

	if (!host.Contains("@"))
	{
	    if (!Magick::instance().nickserv.IsLive(host))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
		return;
	    }
	    host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.prepend("*!");
	}
    }

    unsigned int i = 1;
    bool head = false;

    MLOCK(("OperServ", "Ignore"));
    for (Magick::instance().operserv.Ignore = Magick::instance().operserv.Ignore_begin();
	 Magick::instance().operserv.Ignore != Magick::instance().operserv.Ignore_end(); Magick::instance().operserv.Ignore++)
    {
	if (Magick::instance().operserv.Ignore->Entry().Matches(host, true) && Magick::instance().operserv.Ignore->Value())
	{
	    if (head == false)
	    {
		SEND(mynick, source, "LIST/DISPLAY_MATCH", (host, Magick::instance().getMessage(source, "LIST/SIGNORE")));
		head = true;
	    }
	    ::sendV(mynick, source, "%3d. %s (%s)", i, Magick::instance().operserv.Ignore->Entry().c_str(),
		    parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
				 mVarArray(Magick::instance().operserv.Ignore->Last_Modify_Time().Ago(),
					   Magick::instance().operserv.Ignore->Last_Modifier())).c_str());
	    i++;
	}
    }
    if (head == false)
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/SIGNORE")));
}

SXP::Tag OperServ::tag_OperServ("OperServ");
SXP::Tag OperServ::tag_Clone("Clone");
SXP::Tag OperServ::tag_Akill("Akill");
SXP::Tag OperServ::tag_OperDeny("OperDeny");
SXP::Tag OperServ::tag_Ignore("Ignore");

void OperServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("OperServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if (pElement->IsA(tag_Clone))
    {
	Clone_Type *tmp = new Clone_Type;

	c_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_Akill))
    {
	Akill_Type *tmp = new Akill_Type;

	a_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_OperDeny))
    {
	OperDeny_Type *tmp = new OperDeny_Type;

	o_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_Ignore))
    {
	Ignore_Type *tmp = new Ignore_Type;

	i_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }
}

void OperServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    static_cast < void >(pIn);
    static_cast < void >(pElement);

    FT("OperServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
}

void OperServ::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    static_cast < void >(attribs);

    FT("OperServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    set < Clone_Type >::iterator i;
    set < Akill_Type >::iterator j;
    set < OperDeny_Type >::iterator k;
    set < Ignore_Type >::iterator l;

    //TODO: Add your source code here
    pOut->BeginObject(tag_OperServ);

    {
	MLOCK(("OperServ", "Clone"));
	for (i = i_Clone.begin(); i != i_Clone.end(); i++)
	{
	    pOut->BeginObject(tag_Clone);
	    pOut->WriteSubElement(const_cast < Clone_Type * >(&(*i)));
	    pOut->EndObject(tag_Clone);
	}
    }

    {
	MLOCK(("OperServ", "Akill"));
	for (j = i_Akill.begin(); j != i_Akill.end(); j++)
	{
	    pOut->BeginObject(tag_Akill);
	    pOut->WriteSubElement(const_cast < Akill_Type * >(&(*j)));
	    pOut->EndObject(tag_Akill);
	}
    }

    {
	MLOCK(("OperServ", "OperDeny"));
	for (k = i_OperDeny.begin(); k != i_OperDeny.end(); k++)
	{
	    pOut->BeginObject(tag_OperDeny);
	    pOut->WriteSubElement(const_cast < OperDeny_Type * >(&(*k)));
	    pOut->EndObject(tag_OperDeny);
	}
    }

    {
	MLOCK(("OperServ", "Ignore"));
	for (l = i_Ignore.begin(); l != i_Ignore.end(); l++)
	{
	    // Only save PERM entries
	    if (l->Value())
	    {
		pOut->BeginObject(tag_Ignore);
		pOut->WriteSubElement(const_cast < Ignore_Type * >(&(*l)));
		pOut->EndObject(tag_Ignore);
	    }
	}
    }

    pOut->EndObject(tag_OperServ);
}

void OperServ::PostLoad()
{
    NFT("OperServ::PostLoad");
    // Linkage, etc

    unsigned int i;

    for (i = 0; i < c_array.size(); i++)
    {
	if (c_array[i] != NULL)
	{
	    i_Clone.insert(*c_array[i]);
	    delete c_array[i];
	}
    }
    c_array.clear();

    for (i = 0; i < a_array.size(); i++)
    {
	if (a_array[i] != NULL)
	{
	    i_Akill.insert(*a_array[i]);
	    delete a_array[i];
	}
    }
    a_array.clear();

    for (i = 0; i < o_array.size(); i++)
    {
	if (o_array[i] != NULL)
	{
	    i_OperDeny.insert(*o_array[i]);
	    delete o_array[i];
	}
    }
    o_array.clear();

    for (i = 0; i < i_array.size(); i++)
    {
	if (i_array[i] != NULL)
	{
	    i_Ignore.insert(*i_array[i]);
	    delete i_array[i];
	}
    }
    i_array.clear();

    for (Clone = Clone_begin(); Clone != Clone_end(); Clone++)
    {
	Clone->PostLoad();
    }
    for (Akill = Akill_begin(); Akill != Akill_end(); Akill++)
    {
	Akill->PostLoad();
    }
    for (OperDeny = OperDeny_begin(); OperDeny != OperDeny_end(); OperDeny++)
    {
	OperDeny->PostLoad();
    }
    for (Ignore = Ignore_begin(); Ignore != Ignore_end(); Ignore++)
    {
	Ignore->PostLoad();
    }
}

void OperServ::DumpB() const
{
    MB(0, (i_Clone.size(), CloneList.size(), i_Akill.size(), i_OperDeny.size(), i_Ignore.size()));
}

void OperServ::DumpE() const
{
    ME(0, (i_Clone.size(), CloneList.size(), i_Akill.size(), i_OperDeny.size(), i_Ignore.size()));
}
