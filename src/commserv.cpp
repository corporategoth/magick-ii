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
#define RCSID(x,y) const char *rcsid_commserv_cpp_ ## x () { return y; }
RCSID(commserv_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.110  2001/12/25 08:43:12  prez
** Fixed XML support properly ... it now works again with new version of
** expat (1.95.2) and sxp (1.1).  Also removed some of my const hacks.
**
** Revision 1.109  2001/12/23 20:46:03  prez
** Added cleanup code for committee entries in channel access list.
**
** Revision 1.108  2001/12/20 08:02:32  prez
** Massive change -- 'Parent' has been changed to Magick::instance(), will
** soon also move the ACE_Reactor over, and will be able to have multipal
** instances of Magick in the same process if necessary.
**
** Revision 1.107  2001/11/12 01:05:02  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.106  2001/11/07 04:11:58  prez
** Added in better message trying to delete a member who is a head.
**
** Revision 1.105  2001/11/04 19:23:09  ungod
** fixed up compilation for borland c++ builder
**
** Revision 1.104  2001/11/03 21:02:52  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.103  2001/06/17 09:39:07  prez
** Hopefully some more changes that ensure uptime (mainly to do with locking
** entries in an iterated search, and using copies of data instead of references
** where we can get away with it -- reducing the need to lock the data).
**
** Revision 1.102  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.101  2001/05/22 22:57:10  prez
** Fixed nick linking/idetify, and recognition of committee heads.
**
** Revision 1.100  2001/05/17 19:18:54  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.99  2001/05/13 00:55:18  prez
** More patches to try and fix deadlocking ...
**
** Revision 1.98  2001/05/08 15:51:41  prez
** Added some security stuff with committees, so certain things are guarenteed
** on database load (ie. the magick.ini assertions).
**
** Revision 1.97  2001/05/06 03:03:07  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.96  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.95  2001/05/01 14:00:23  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.94  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.93  2001/03/27 07:04:31  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.92  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.91  2001/03/04 02:04:14  prez
** Made mstring a little more succinct ... and added vector/list operations
**
** Revision 1.90  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.89  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.88  2001/02/03 02:21:33  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.87  2001/01/15 23:31:38  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.86  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.85  2000/12/31 17:54:29  prez
** Added checking to see if 'http://' was entered in the SET URL commands.
**
** Revision 1.84  2000/12/29 15:31:55  prez
** Added locking/checking for dcc/events threads.  Also for ACE_Log_Msg
**
** Revision 1.83  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.82  2000/12/22 19:50:19  prez
** Made all config options const.  Beginnings of securing all non-modifying
** commands to const.  also added serviceschk.
**
** Revision 1.81  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.80  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.79  2000/12/10 02:56:06  prez
** Added ability to change DESCRIPTION field in committees.
**
** Revision 1.78  2000/12/09 20:16:41  prez
** Fixed SubString and Left to have correct count/end possitions.  Also
** adjusted rest of source to follow suit.
**
** Revision 1.77  2000/10/10 11:47:51  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.76  2000/09/30 10:48:07  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.75  2000/09/22 12:26:11  prez
** Fixed that pesky bug with chanserv not seeing modes *sigh*
**
** Revision 1.74  2000/09/12 21:17:02  prez
** Added IsLiveAll (IsLive now checks to see if user is SQUIT).
**
** Revision 1.73  2000/09/09 02:17:48  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.72  2000/09/05 10:53:06  prez
** Only have operserv.cpp and server.cpp to go with T_Changing / T_Modify
** tracing -- also modified keygen to allow for cmdline generation (ie.
** specify 1 option and enter keys, or 2 options and the key is read from
** a file).  This allows for paragraphs with \n's in them, and helps so you
** do not have to type out 1024 bytes :)
**
** Revision 1.71  2000/09/02 07:20:45  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.70  2000/08/28 10:51:37  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.69  2000/08/08 09:58:56  prez
** Added ModeO to 4 pre-defined committees.
** Also added back some deletes in xml in the hope that it
** will free up some memory ...
**
** Revision 1.68  2000/08/07 12:20:27  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.67  2000/08/06 08:06:41  prez
** Fixed loading of logon messages in committee ..
**
** Revision 1.66  2000/08/02 20:08:57  prez
** Minor code cleanups, added ACE installation instructions, updated the
** suggestions file and stopped people doing a whole bunch of stuff to
** forbidden nicknames.
**
** Revision 1.65  2000/07/21 00:18:49  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.64  2000/06/19 13:15:34  prez
** Documentation, what a novel concept.
**
** Revision 1.63  2000/06/18 13:31:48  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.62  2000/06/18 12:49:27  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.61  2000/06/12 06:07:50  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.60  2000/06/11 08:20:12  prez
** More minor bug fixes, godda love testers.
**
** Revision 1.59  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.58  2000/06/06 08:57:56  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.57  2000/05/27 07:06:01  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.56  2000/05/25 08:16:39  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.55  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.54  2000/05/14 06:30:14  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.53  2000/05/14 04:02:53  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.52  2000/05/10 11:46:59  prez
** added back memo timers
**
** Revision 1.51  2000/05/09 09:11:59  prez
** Added XMLisation to non-mapped structures ... still need to
** do the saving stuff ...
**
** Revision 1.50  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.49  2000/04/03 09:45:22  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.48  2000/03/28 16:20:58  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.47  2000/03/28 09:42:11  prez
** Changed CommServ, ADD/DEL/LIST -> MEMBER ADD/DEL/LIST
** and NEW/KILL -> ADD/DEL and created a new LIST
**
** Revision 1.46  2000/03/27 21:26:12  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.45  2000/03/26 14:59:36  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.44  2000/03/15 08:23:51  prez
** Added locking stuff for commserv options, and other stuff
**
** Revision 1.43  2000/03/14 13:36:46  prez
** Finished P12 compliance (SJOIN, etc).
**
** Revision 1.42  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.41  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.40  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.39  2000/02/17 12:55:05  ungod
** still working on borlandization
**
** Revision 1.38  2000/02/16 12:59:38  ungod
** fixing for borland compilability
**
** Revision 1.37  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.36  2000/02/15 10:37:48  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"
#include "dccengine.h"

#ifndef MAGICK_HAS_EXCEPTIONS
static Committee_t GLOB_Committee_t;
#endif

void Committee_t::defaults()
{
    NFT("Committee_t::defaults");

    setting.Private = Magick::instance().commserv.DEF_Private();
    lock.Private = false;
    setting.OpenMemos = Magick::instance().commserv.DEF_OpenMemos();
    lock.OpenMemos = false;
    setting.Secure = Magick::instance().commserv.DEF_Secure();
    lock.Secure = false;
}

Committee_t::Committee_t(const mstring& name, const mstring& head, const mstring& description)
	: i_Name(name.UpperCase()), i_RegTime(mDateTime::CurrentDateTime()),
	  i_Head(head.LowerCase()), i_Description(description)
{
    FT("Committee_t::Committee_t", (name, head, description));
    defaults();
    DumpE();
}


Committee_t::Committee_t(const mstring& name, const Committee_t& head, const mstring& description)
	: i_Name(name.UpperCase()), i_RegTime(mDateTime::CurrentDateTime()),
	  i_HeadCom(head.Name()), i_Description(description)
{
    FT("Committee_t::Committee_t", (name, "(Committee_t *) head", description));
    defaults();
    DumpE();
}


Committee_t::Committee_t(const mstring& name, const mstring& description)
	: i_Name(name.UpperCase()), i_RegTime(mDateTime::CurrentDateTime()),
	  i_Description(description)
{
    FT("Committee_t::Committee_t", (name, description));
    defaults();
    DumpE();
}


void Committee_t::operator=(const Committee_t &in)
{
    FT("Committee_t::operator=", ("(const Committee_t &) in"));

    i_Name = in.i_Name;
    i_RegTime = in.i_RegTime;
    i_Head = in.i_Head;
    i_HeadCom = in.i_HeadCom;
    i_Description = in.i_Description;
    i_Email = in.i_Email;
    i_URL = in.i_URL;
    setting.OpenMemos = in.setting.OpenMemos;
    lock.OpenMemos = in.lock.OpenMemos;
    setting.Secure = in.setting.Secure;
    lock.Secure = in.lock.Secure;
    setting.Private = in.setting.Private;
    lock.Private = in.lock.Private;
    i_Members = in.i_Members;
    i_Messages = in.i_Messages;

    map<mstring, mstring>::const_iterator j;
    i_UserDef.clear();
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
}

mDateTime Committee_t::RegTime() const
{
    NFT("Committee_t::RegTime");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_RegTime"));
    RET(i_RegTime);
}

void Committee_t::Drop()
{
    // Now we go for our channels ...
    ChanServ::stored_t::iterator iter;
    mstring entry = "@" + i_Name;
    { RLOCK(("ChanServ", "stored"));
    for (iter = Magick::instance().chanserv.StoredBegin();
	    iter != Magick::instance().chanserv.StoredEnd(); iter++)
    {
	MLOCK(("ChanServ", "stored", iter->first, "Access"));
	if (iter->second.Access_find(entry))
	    iter->second.Access_erase();
    }}
}

mstring Committee_t::HeadCom() const
{
    NFT("Committee_t::HeadCom");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_HeadCom"));
    RET(i_HeadCom);
}

mstring Committee_t::Head() const
{
    NFT("Committee_t::Head");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Head"));
    RET(i_Head);
}

void Committee_t::Head(const mstring& newhead)
{
    FT("Committee_t::Head", (newhead));

    WLOCK2(("CommServ", "list", i_Name.UpperCase(), "i_Head"));
    MCB(i_Head);
    { WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_HeadCom"));
    if (!i_HeadCom.empty())
    {
	CB(1, i_HeadCom);
	i_HeadCom.erase();
	CE(1, i_HeadCom);
    }}

    i_Head = newhead.LowerCase();
    MCE(i_Head);
}

bool Committee_t::insert(const mstring& entry, const mstring& nick,
	const mDateTime& modtime)
{
    FT("Committee_t::insert", (entry, nick, modtime));

    entlist_ui iter;
    MLOCK(("CommServ", "list", i_Name.UpperCase(), "member"));
    if (!i_Members.empty())
	for (iter=i_Members.begin(); iter!=i_Members.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;
    if (i_Members.empty() || iter == i_Members.end())
    {
	pair<entlist_ui, bool> tmp;
	MCB(i_Members.size());
	tmp = i_Members.insert(entlist_t(entry, nick, modtime));
	MCE(i_Members.size());
	if (tmp.second)
	    member = tmp.first;
	else
	    member = i_Members.end();
	RET(true);
    }
    else
    {
	RET(false);
    }
}


bool Committee_t::erase()
{
    NFT("Committee_t::erase");

    MLOCK(("CommServ", "list", i_Name.UpperCase(), "member"));
    if (member != i_Members.end())
    {
	MCB(i_Members.size());
	i_Members.erase(member);
	MCE(i_Members.size());
	member = i_Members.end();
	RET(true);
    }
    else
    {
	RET(false);
    }
}

bool Committee_t::find(const mstring& entry)
{
    FT("Committee_t::find", (entry));

    MLOCK(("CommServ", "list", i_Name.UpperCase(), "member"));
    entlist_ui iter = i_Members.end();
    if (!i_Members.empty())
    {
	// FIND exact nickname
	for (iter=i_Members.begin(); iter!=i_Members.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;
    }

    if (iter != i_Members.end())
    {
	member = iter;
	RET(true);
    }
    else
    {
	member = i_Members.end();
	RET(false);
    }
}


bool Committee_t::IsIn(const mstring& nick) const
{
    FT("Committee_t::IsIn", (nick));

    if (i_Name == Magick::instance().commserv.ALL_Name() &&
	Magick::instance().nickserv.IsLive(nick))
    {
	RET(true);
    }

    if (!Magick::instance().nickserv.IsStored(nick))
    {
	RET(false);
    }

    if (i_Name == Magick::instance().commserv.REGD_Name())
    {
	RET(true);
    }

    mstring target = Magick::instance().nickserv.GetStored(nick).Host();
    if (target.empty())
	target = nick.LowerCase();

    // We're a HEAD, in by DEFAULT
    { RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_HeadCom"));
    if (!i_HeadCom.empty() && Magick::instance().commserv.IsList(i_HeadCom) &&
	Magick::instance().commserv.GetList(i_HeadCom).IsIn(target))
    {
	RET(true);
    }}

    { RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Head"));
    if (!i_Head.empty() && target.IsSameAs(i_Head, true))
    {
	RET(true);
    }}

    vector<mstring> members;
    unsigned int i;
    { MLOCK(("CommServ", "list", i_Name.UpperCase(), "member"));
    entlist_cui iter;
    for (iter=i_Members.begin(); iter!=i_Members.end(); iter++)
    {
	members.push_back(iter->Entry().LowerCase());
    }}
    for (i=0; i<members.size(); i++)
    {
	if (target.IsSameAs(members[i], true))
	{
	    RET(true);
	}
    }

    // If this committee is SADMIN, we could be a member
    // as a SLAVE since theres no way of online changing host.
    if (i_Name == Magick::instance().commserv.SADMIN_Name())
    {
	unsigned int j;
	Nick_Stored_t snick = Magick::instance().nickserv.GetStored(target);
	for (i=0; i<snick.Siblings(); i++)
	{
	    for (j=0; j<members.size(); j++)
	    {
		if (snick.Sibling(i).IsSameAs(members[j], true))
		{
		    RET(true);
		}
	    }
	}
    }

    RET(false);
}


bool Committee_t::IsOn(const mstring& nick) const
{
    FT("Committee_t::IsOn", (nick));

    // If we aint online, we aint in nothing.
    if (!Magick::instance().nickserv.IsLive(nick))
    {
	RET(false);
    }

    if (i_Name == Magick::instance().commserv.ALL_Name())
    {
	RET(true);
    }

    // The committee we're looking at has ModeO set,
    // but user doesnt have umode +o.
    if (((i_Name == Magick::instance().commserv.SADMIN_Name() &&
	Magick::instance().commserv.SADMIN_ModeO()) ||
	(i_Name == Magick::instance().commserv.SOP_Name() &&
	Magick::instance().commserv.SOP_ModeO()) ||
	(i_Name == Magick::instance().commserv.ADMIN_Name() &&
	Magick::instance().commserv.ADMIN_ModeO()) ||
	(i_Name == Magick::instance().commserv.OPER_Name() &&
	Magick::instance().commserv.OPER_ModeO())) &&
	!Magick::instance().nickserv.GetLive(nick).HasMode("o"))
    {
	RET(false);
    }

    // This returns wether we're CURRENTLY recognized
    // as 'in' the committee (secure, and nick secure
    // taken into account).
    if (IsIn(nick) && Magick::instance().nickserv.IsStored(nick) &&
	Magick::instance().nickserv.GetStored(nick).IsOnline())
    {
	RLOCK_IF(("CommServ", "list", i_Name.UpperCase(), "setting.Secure"),
		!setting.Secure || Magick::instance().nickserv.GetLive(nick).IsIdentified())
	{
	    RET(true);
	}
    }
    RET(false);
}


bool Committee_t::IsHead(const mstring& nick) const
{
    FT("Committee_t::IsHead", (nick));

    RLOCK_IF(("CommServ", "list", i_Name.UpperCase(), "i_Head"),
	!i_Head.empty() && i_Head.IsSameAs(nick, true))
    {
	RET(true);
    }
    else
    {
	RLOCK2_IF(("CommServ", "list", i_Name.UpperCase(), "i_HeadCom"),
	    !i_HeadCom.empty() && Magick::instance().commserv.IsList(i_HeadCom))
	{
	    if (Magick::instance().commserv.GetList(i_HeadCom).IsIn(nick))
	    {
		RET(true);
	    }
	}
	else if (i_Head.empty() && i_HeadCom.empty())
	{
	    bool retval = IsIn(nick);
	    RET(retval);
	}
    }
    RET(false);
}

void Committee_t::Description(const mstring& in)
{
    FT("Committee_t::Description", (in));
    WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Description"));
    MCB(i_Description);
    i_Description = in;
    MCE(i_Description);
}

mstring Committee_t::Description() const
{
    NFT("Committee_t::Description");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Description"));
    RET(i_Description);
}

void Committee_t::Email(const mstring& in)
{
    FT("Committee_t::Email", (in));
    WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Email"));
    MCB(i_Email);
    i_Email = in;
    MCE(i_Email);
}

mstring Committee_t::Email() const
{
    NFT("Committee_t::Email");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Email"));
    RET(i_Email);
}

void Committee_t::URL(const mstring& in)
{
    FT("Committee_t::URL", (in));
    WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_URL"));
    MCB(i_URL);
    i_URL = in;
    MCE(i_URL);
}

mstring Committee_t::URL() const
{
    NFT("Committee_t::URL");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_URL"));
    RET(i_URL);
}

void Committee_t::Private(const bool in)
{
    FT("Committee_t::Private", (in));
    if (!L_Private())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "setting.Private"));
	MCB(setting.Private);
	setting.Private = in;
	MCE(setting.Private);
    }
}

bool Committee_t::Private() const
{
    NFT("Committee_t::Private");
    if (!Magick::instance().commserv.LCK_Private())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "setting.Private"));
	RET(setting.Private);
    }
    RET(Magick::instance().commserv.DEF_Private());
}


void Committee_t::L_Private(const bool in)
{
    FT("Committee_t::L_Private", (in));
    if (!Magick::instance().commserv.LCK_Private())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "lock.Private"));
	MCB(lock.Private);
	lock.Private = in;
	MCE(lock.Private);
    }
}


bool Committee_t::L_Private() const
{
    NFT("Committee_t::L_Private");
    if (!Magick::instance().commserv.LCK_Private())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "lock.Private"));
	RET(lock.Private);
    }
    RET(true);
}


void Committee_t::Secure(const bool in)
{
    FT("Committee_t::Secure", (in));
    if (!L_Secure())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "setting.Secure"));
	MCB(setting.Secure);
	setting.Secure = in;
	MCE(setting.Secure);
    }
}


bool Committee_t::Secure() const
{
    NFT("Committee_t::Secure");
    if (!Magick::instance().commserv.LCK_Secure())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "setting.Secure"));
	RET(setting.Secure);
    }
    RET(Magick::instance().commserv.DEF_Secure());
}


void Committee_t::L_Secure(const bool in)
{
    FT("Committee_t::L_Secure", (in));
    if (!Magick::instance().commserv.LCK_Secure())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "lock.Secure"));
	MCB(lock.Secure);
	lock.Secure = in;
	MCE(lock.Secure);
    }
}


bool Committee_t::L_Secure() const
{
    NFT("Committee_t::L_Secure");
    if (!Magick::instance().commserv.LCK_Secure())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "lock.Secure"));
	RET(lock.Secure);
    }
    RET(true);
}


void Committee_t::OpenMemos(const bool in)
{
    FT("Committee_t::OpenMemos", (in));
    if (!L_OpenMemos())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "setting.OpenMemos"));
	MCB(setting.OpenMemos);
	setting.OpenMemos = in;
	MCE(setting.OpenMemos);
    }
}


bool Committee_t::OpenMemos() const
{
    NFT("Committee_t::OpenMemos");
    if (!Magick::instance().commserv.LCK_OpenMemos())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "setting.OpenMemos"));
	RET(setting.OpenMemos);
    }
    RET(Magick::instance().commserv.DEF_OpenMemos());
}


void Committee_t::L_OpenMemos(const bool in)
{
    FT("Committee_t::L_OpenMemos", (in));
    if (!Magick::instance().commserv.LCK_OpenMemos())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "lock.OpenMemos"));
	MCB(lock.OpenMemos);
	lock.OpenMemos = in;
	MCE(lock.OpenMemos);
    }
}


bool Committee_t::L_OpenMemos() const
{
    NFT("Committee_t::L_OpenMemos");
    if (!Magick::instance().commserv.LCK_OpenMemos())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "lock.OpenMemos"));
	RET(lock.OpenMemos);
    }
    RET(true);
}


bool Committee_t::MSG_insert(const mstring& entry, const mstring& nick,
	const mDateTime& addtime)
{
    FT("Committee_t::MSG_insert", (entry, nick, addtime));

    MLOCK(("CommServ", "list", i_Name.UpperCase(), "message"));
    if (IsHead(nick))
    {
	MCB(i_Messages.size());
	i_Messages.push_back(entlist_t(entry, nick, addtime));
	MCE(i_Messages.size());
	message = i_Messages.end(); message--;
	RET(true);
    }
    else
    {
	message = i_Messages.end();
	RET(false);
    }
}


bool Committee_t::MSG_erase()
{
    NFT("Committee_t::MSG_erase");

    MLOCK(("CommServ", "list", i_Name.UpperCase(), "message"));
    if (message != i_Messages.end())
    {
	MCB(i_Messages.size());
	i_Messages.erase(message);
	MCE(i_Messages.size());
	message = i_Messages.end();
	RET(true);
    }
    else
    {
	RET(false);
    }
}


bool Committee_t::MSG_find(const int number)
{
    FT("Committee_t::MSG_find", (number));

    MLOCK(("CommServ", "list", i_Name.UpperCase(), "message"));
    entlist_i iter = i_Messages.end();
    int i;
    if (!i_Messages.empty())
    {
	// FIND exact nickname
	for (iter=i_Messages.begin(), i=1; iter!=i_Messages.end() &&
						i != number; iter++, i++);
    }

    if (iter != i_Messages.end())
    {
	message = iter;
	RET(true);
    }
    else
    {
	message = i_Messages.end();
	RET(false);
    }
}

size_t Committee_t::Usage() const
{
    size_t retval = 0;

    WLOCK(("CommServ", "list", i_Name.UpperCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_RegTime.Internal());
    retval += i_HeadCom.capacity();
    retval += i_Head.capacity();
    retval += i_Description.capacity();
    retval += i_Email.capacity();
    retval += i_URL.capacity();
    
    entlist_cui i;
    for (i=i_Members.begin(); i!=i_Members.end(); i++)
    {
	retval += i->Usage();
    }

    retval += sizeof(setting);
    retval += sizeof(lock);

    entlist_ci j;
    for (j=i_Messages.begin(); j!=i_Messages.end(); j++)
    {
	retval += j->Usage();
    }

    map<mstring,mstring>::const_iterator l;
    for (l=i_UserDef.begin(); l!=i_UserDef.end(); l++)
    {
	retval += l->first.capacity();
	retval += l->second.capacity();
    }

    return retval;    
}

void Committee_t::DumpB() const
{
    MB(0, (i_Name, i_RegTime, i_HeadCom, i_Head, i_Description, i_Email,
	i_URL, i_Members.size(), setting.Private, lock.Private, setting.OpenMemos,
	lock.OpenMemos, setting.Secure, lock.Secure, i_Messages.size(), i_UserDef.size()));
}

void Committee_t::DumpE() const
{
    ME(0, (i_Name, i_RegTime, i_HeadCom, i_Head, i_Description, i_Email,
	i_URL, i_Members.size(), setting.Private, lock.Private, setting.OpenMemos,
	lock.OpenMemos, setting.Secure, lock.Secure, i_Messages.size(), i_UserDef.size()));
}

CommServ::CommServ()
{
    NFT("CommServ::CommServ");
    messages = true;
}

#ifdef MAGICK_HAS_EXCEPTIONS
void CommServ::AddList(Committee_t *in) throw(E_CommServ_List)
#else
void CommServ::AddList(Committee_t *in)
#endif
{
    FT("CommServ::AddList", ("(Committee_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_CommServ_List(E_CommServ_List::W_Add, E_CommServ_List::T_Invalid));
#else
	NLOG(LM_CRITICAL, "Exception - Comm:List:Add:Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_CommServ_List(E_CommServ_List::W_Add, E_CommServ_List::T_Blank));
#else
	LOG((LM_CRITICAL, "Exception - Comm:List:Add:Blank"));
	return;
#endif
    }

    WLOCK(("CommServ", "list"));
    /* i_list[in->Name().UpperCase()] = in; */
    i_list[in->Name().UpperCase()] = *in;
}


#ifdef MAGICK_HAS_EXCEPTIONS
Committee_t &CommServ::GetList(const mstring &in) const throw(E_CommServ_List)
#else
Committee_t &CommServ::GetList(const mstring &in) const
#endif
{
    FT("CommServ::GetList", (in));

    RLOCK(("CommServ", "list", in.UpperCase()));
    CommServ::list_t::const_iterator iter = i_list.find(in.UpperCase());
    if (iter == i_list.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_CommServ_List(E_CommServ_List::W_Get, E_CommServ_List::T_NotFound, in.c_str()));
#else
	LOG((LM_EMERGENCY, "Exception - Comm:List:Get:NotFound - %s", in.c_str()));
	NRET(Committee_t &, GLOB_Committee_t);
#endif
    }
    /* if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_CommServ_List(E_CommServ_List::W_Get, E_CommServ_List::T_Invalid, in.c_str()));
#else
	LOG((LM_EMERGENCY, "Exception - Comm:List:Get:Invalid - %s", in.c_str()));
	NRET(Committee_t &, GLOB_Committee_t);
#endif
    }
    if (iter->second->Name().empty()) */
    if (iter->second.Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_CommServ_List(E_CommServ_List::W_Get, E_CommServ_List::T_Blank, in.c_str()));
#else
	LOG((LM_EMERGENCY, "Exception - Comm:List:Get:Blank - %s", in.c_str()));
	NRET(Committee_t &, GLOB_Committee_t);
#endif
    }

    /* NRET(Committee_t &, const_cast<Committee_t &>(*iter->second)); */
    NRET(Committee_t &, const_cast<Committee_t &>(iter->second));
}


#ifdef MAGICK_HAS_EXCEPTIONS
void CommServ::RemList(const mstring &in) throw(E_CommServ_List)
#else
void CommServ::RemList(const mstring &in)
#endif
{
    FT("CommServ::RemList", (in));

    WLOCK(("CommServ", "list"));
    CommServ::list_t::iterator iter = i_list.find(in.UpperCase());
    if (iter == i_list.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_CommServ_List(E_CommServ_List::W_Rem, E_CommServ_List::T_NotFound, in.c_str()));
#else
	LOG((LM_CRITICAL, "Exception - Comm:List:Rem:NotFound - %s", in.c_str()));
	return;
#endif
    }
    WLOCK2(("CommServ", "list", iter->first));
    /* if (iter->second != NULL)
    {
	delete iter->second;
    } */
    i_list.erase(iter);
}


bool CommServ::IsList(const mstring& in)const
{
    FT("CommServ::IsList", (in));
    RLOCK(("CommServ", "list"));
    bool retval = (i_list.find(in.UpperCase())!=i_list.end());
    RET(retval);
}

void CommServ::AddCommands()
{
    NFT("CommServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"MEMB* ADD", Magick::instance().commserv.REGD_Name(), CommServ::do_member_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"MEMB* DEL*", Magick::instance().commserv.REGD_Name(), CommServ::do_member_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"MEMB* ERA*", Magick::instance().commserv.REGD_Name(), CommServ::do_member_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"MEMB* LIST", Magick::instance().commserv.REGD_Name(), CommServ::do_member_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"MEMB* VIEW", Magick::instance().commserv.REGD_Name(), CommServ::do_member_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"MEMB* H*LP", Magick::instance().commserv.REGD_Name(), do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOG* ADD", Magick::instance().commserv.REGD_Name(), CommServ::do_logon_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOG* DEL*", Magick::instance().commserv.REGD_Name(), CommServ::do_logon_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOG* ERA*", Magick::instance().commserv.REGD_Name(), CommServ::do_logon_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOG* LIST", Magick::instance().commserv.REGD_Name(), CommServ::do_logon_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOG* VIEW", Magick::instance().commserv.REGD_Name(), CommServ::do_logon_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOG* H*LP", Magick::instance().commserv.REGD_Name(), do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* HEAD*", Magick::instance().commserv.REGD_Name(), CommServ::do_set_Head);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* DESC*", Magick::instance().commserv.REGD_Name(), CommServ::do_set_Description);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* E*MAIL*", Magick::instance().commserv.REGD_Name(), CommServ::do_set_Email);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* URL", Magick::instance().commserv.REGD_Name(), CommServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* WWW*", Magick::instance().commserv.REGD_Name(), CommServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* WEB*", Magick::instance().commserv.REGD_Name(), CommServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* SEC*", Magick::instance().commserv.REGD_Name(), CommServ::do_set_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* PRIV*", Magick::instance().commserv.REGD_Name(), CommServ::do_set_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* *MEMO*", Magick::instance().commserv.REGD_Name(), CommServ::do_set_OpenMemos);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* H*LP", Magick::instance().commserv.REGD_Name(), do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK SEC*", Magick::instance().commserv.SOP_Name(), CommServ::do_lock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK PRIV*", Magick::instance().commserv.SOP_Name(), CommServ::do_lock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK *MEMO*", Magick::instance().commserv.SOP_Name(), CommServ::do_lock_OpenMemos);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK H*LP", Magick::instance().commserv.SOP_Name(), do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Magick::instance().commserv.SOP_Name(), CommServ::do_unlock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Magick::instance().commserv.SOP_Name(), CommServ::do_unlock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK *MEMO*", Magick::instance().commserv.SOP_Name(), CommServ::do_unlock_OpenMemos);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK H*LP", Magick::instance().commserv.SOP_Name(), do_3param);

    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"H*LP", Magick::instance().commserv.ALL_Name(), CommServ::do_Help);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ADD*", Magick::instance().commserv.SOP_Name(), CommServ::do_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"DEL*", Magick::instance().commserv.SOP_Name(), CommServ::do_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ERA*", Magick::instance().commserv.SOP_Name(), CommServ::do_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LIST", Magick::instance().commserv.ALL_Name(), CommServ::do_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"*MEMO*", Magick::instance().commserv.REGD_Name(), CommServ::do_Memo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"INFO", Magick::instance().commserv.ALL_Name(), CommServ::do_Info);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"MEMB* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"MEMB*", Magick::instance().commserv.REGD_Name(), do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOG* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOG*", Magick::instance().commserv.REGD_Name(), do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET*", Magick::instance().commserv.REGD_Name(), do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK *", Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK", Magick::instance().commserv.SOP_Name(), do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK *", Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK", Magick::instance().commserv.SOP_Name(), do_1_3param);
}

void CommServ::RemCommands()
{
    NFT("CommServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"MEMB* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"MEMB* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"MEMB* ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"MEMB* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"MEMB* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"MEMB* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOG* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOG* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOG* ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOG* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOG* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOG* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* HEAD*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* DESC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* E*MAIL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* URL", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* WWW*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* WEB*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* SEC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* PRIV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* *MEMO*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK *MEMO*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK H*LP", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK *MEMO*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK H*LP", Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"H*LP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ADD*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"DEL*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ERA*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LIST", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"*MEMO*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"INFO", Magick::instance().commserv.ALL_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"MEMB* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"MEMB*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOG* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOG*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK *", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK *", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK", Magick::instance().commserv.SOP_Name());
}


void CommServ::execute(mstring& source, const mstring& msgtype, const mstring& params)
{
    mThread::ReAttach(tt_OtherServ);
    FT("CommServ::execute", (source, msgtype, params));
    //okay this is the main commserv command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message
    mstring mynick(Magick::instance().getLname(params.ExtractWord(1, ": ")));
    mstring message(params.After(":"));
    mstring command(message.Before(" "));

    if (message[0U] == CTCP_DELIM_CHAR)
    {
	if (msgtype == "PRIVMSG")
	    DccEngine::decodeRequest(mynick, source, message);
	else
	    DccEngine::decodeReply(mynick, source, message);
    }
    else if (msgtype == "PRIVMSG" &&
	!Magick::instance().commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);

}

void CommServ::do_Help(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    mstring HelpTopic = Magick::instance().commserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector<mstring> help = Magick::instance().getHelp(source, HelpTopic.UpperCase());
					
    unsigned int i;
    for (i=0; i<help.size(); i++)
	::send(mynick, source, help[i]);
}

void CommServ::do_Add(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_Add", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring head      = params.ExtractWord(3, " ");
    mstring desc      = params.After(" ", 3);

    if (Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISSTORED", (
				committee));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(head))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				head));
	return;
    }

    head = Magick::instance().getSname(head);
    if (Magick::instance().nickserv.GetStored(head).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
				head));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    Committee_t tmp(committee, head, desc);
    Magick::instance().commserv.AddList(&tmp);
    Magick::instance().commserv.stats.i_Add++;
    SEND(mynick, source, "COMMSERV/ADD", (
				committee, head));
    LOG(LM_NOTICE, "COMMSERV/ADD",
	(Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	committee, head));
}


void CommServ::do_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_Del", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    Magick::instance().commserv.GetList(committee).Drop();
    Magick::instance().commserv.RemList(committee);
    Magick::instance().commserv.stats.i_Del++;
    SEND(mynick, source, "COMMSERV/DEL", ( committee));
    LOG(LM_NOTICE, "COMMSERV/DEL", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	committee));
}


void CommServ::do_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_List", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	mask = "*";
	listsize = Magick::instance().config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Magick::instance().config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Magick::instance().config.Maxlist())
	{
	    mstring output;
	    SEND(mynick, source, "LIST/MAXLIST", (
					Magick::instance().config.Maxlist()));
	    return;
	}
    }

    SEND(mynick, source, "LIST/COMM_LIST", (
					mask));
    CommServ::list_t::iterator iter;

    { RLOCK(("CommServ", "list"));
    for (iter = Magick::instance().commserv.ListBegin(), i=0, count = 0;
			iter != Magick::instance().commserv.ListEnd(); iter++)
    {
	RLOCK2(("CommServ", "list", iter->first));
	if (iter->second.Name().Matches(mask, true))
	{
	    if (i < listsize && (!iter->second.Private() ||
		(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
		Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View()).IsOn(source))))
	    {
		::send(mynick, source, iter->second.Name() + " (" +
				iter->second.size() + "): " +
				iter->second.Description());
		i++;
	    }
	    count++;
	}
    }}
    SEND(mynick, source, "LIST/DISPLAYED", (
							i, count));
}

void CommServ::do_Memo(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_Memo", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring text      = params.After(" ", 2);

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONDYNAMIC", (
				message));
	return;
    }

    if (!Magick::instance().commserv.GetList(committee).IsOn(source))
    {
	SEND(mynick, source, "COMMSERV/NOTMEMBER", (
				committee));
	return;
    }

    if (!Magick::instance().commserv.GetList(committee).OpenMemos() &&
	!Magick::instance().commserv.GetList(committee).IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    text.prepend("[" + IRC_Bold + committee + IRC_Off + "] ");
    CommServ::do_Memo2(source, committee, text);
    Magick::instance().commserv.stats.i_Memo++;
    SEND(mynick, source, "COMMSERV/MEMO", (
				committee));
    LOG(LM_INFO, "COMMSERV/MEMO", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	committee));
}


void CommServ::do_Memo2(const mstring &source, const mstring &committee,
	const mstring &text)
{
    FT("CommServ::do_Memo2", (source, committee, text));

    if (!Magick::instance().commserv.IsList(committee))
	return;
    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee.UpperCase());

    mstring realme;
    if (Magick::instance().nickserv.IsStored(source))
	realme = Magick::instance().nickserv.GetStored(source).Host();
    else
	return;
    if (realme.empty())
	realme = source;

    if (!comm.HeadCom().empty())
    {
	if (Magick::instance().commserv.IsList(comm.HeadCom()))
	{
	    CommServ::do_Memo2(source, comm.HeadCom(), text);
	}
    }
    else if (!comm.Head().empty())
    {
	if (Magick::instance().nickserv.IsStored(comm.Head()))
	{
	    mstring realrecipiant = Magick::instance().nickserv.GetStored(comm.Head()).Host();
	    if (realrecipiant.empty())
		realrecipiant = comm.Head();
	    if (!realme.IsSameAs(realrecipiant, true))
	    {
		Memo_t tmp(realrecipiant, source, text);
		Magick::instance().memoserv.AddNickMemo(&tmp);

		RLOCK2(("MemoServ", "nick", realrecipiant.LowerCase()));
		MemoServ::nick_memo_t &memolist = Magick::instance().memoserv.GetNick(realrecipiant);
		RLOCK3(("NickServ", "stored", realrecipiant.LowerCase()));
		Nick_Stored_t nick = Magick::instance().nickserv.GetStored(realrecipiant);
		if (nick.IsOnline())
		    SEND(Magick::instance().memoserv.FirstName(), nick.Name(), "MS_COMMAND/NS_NEW", (
			memolist.size(), Magick::instance().memoserv.FirstName(),
			memolist.size()));
		unsigned int i;
		for (i=0; i < nick.Siblings(); i++)
		{
		    if (Magick::instance().nickserv.IsStored(nick.Sibling(i)) &&
				Magick::instance().nickserv.GetStored(nick.Sibling(i)).IsOnline())
		    {
			SEND(Magick::instance().memoserv.FirstName(), nick.Sibling(i), "MS_COMMAND/NS_NEW", (
				memolist.size(), Magick::instance().memoserv.FirstName(),
				memolist.size()));
		    }
		}
	    }
	}
    }

    MLOCK(("CommServ", "list", comm.Name().UpperCase(), "member"));
    for (comm.member = comm.begin(); comm.member != comm.end(); comm.member++)
    {
	if (Magick::instance().nickserv.IsStored(comm.member->Entry()))
	{
	    mstring realrecipiant = Magick::instance().nickserv.GetStored(comm.member->Entry()).Host();
	    if (realrecipiant.empty())
		realrecipiant = comm.member->Entry();
	    if (!realme.IsSameAs(realrecipiant, true))
	    {
		Memo_t tmp(realrecipiant, source, text);
		Magick::instance().memoserv.AddNickMemo(&tmp);

		RLOCK2(("MemoServ", "nick", realrecipiant.LowerCase()));
		MemoServ::nick_memo_t &memolist = Magick::instance().memoserv.GetNick(realrecipiant);
		RLOCK3(("NickServ", "stored", realrecipiant.LowerCase()));
		Nick_Stored_t nick = Magick::instance().nickserv.GetStored(realrecipiant);
		if (nick.IsOnline())
		    SEND(Magick::instance().memoserv.FirstName(), nick.Name(), "MS_COMMAND/NS_NEW", (
			memolist.size(), Magick::instance().memoserv.FirstName(),
			memolist.size()));
		unsigned int i;
		for (i=0; i < nick.Siblings(); i++)
		{
		    if (Magick::instance().nickserv.IsStored(nick.Sibling(i)) &&
				Magick::instance().nickserv.GetStored(nick.Sibling(i)).IsOnline())
		    {
			SEND(Magick::instance().memoserv.FirstName(), nick.Sibling(i), "MS_COMMAND/NS_NEW", (
				memolist.size(), Magick::instance().memoserv.FirstName(),
				memolist.size()));
		    }
		}
	    }
	}
    }
}

void CommServ::do_Info(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_Info", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONDYNAMIC", (
				message));
	return;
    }

    mstring output;
    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);
    SEND(mynick, source, "COMMSERV_INFO/DESCRIPTION", (
		committee, comm.Description()));
    SEND(mynick, source, "COMMSERV_INFO/REGISTERED", (
    			comm.RegTime().Ago()));
    if (!comm.HeadCom().empty())
    {
	SEND(mynick, source, "COMMSERV_INFO/HEADCOM", (
			comm.HeadCom().UpperCase()));
    }
    else if (!comm.Head().empty())
    {
	SEND(mynick, source, "COMMSERV_INFO/HEAD", (
			Magick::instance().getSname(comm.Head())));
    }

    if (!comm.Email().empty())
    {
	SEND(mynick, source, "COMMSERV_INFO/EMAIL", (
			comm.Email()));
    }

    if (!comm.URL().empty())
    {
	SEND(mynick, source, "COMMSERV_INFO/URL", (
			comm.URL()));
    }

    output.erase();
    if (comm.size())
    {
	if (output.size())
	    output << ", ";
	output << comm.size() << " " << Magick::instance().getMessage(source, "COMMSERV_INFO/MEMBERS");
    }
    if (comm.MSG_size())
    {
	if (output.size())
	    output << ", ";
	output << comm.MSG_size() << " " << Magick::instance().getMessage(source, "COMMSERV_INFO/MESSAGES");
    }
    if (output.size())
	SEND(mynick, source, "COMMSERV_INFO/STATS", (
			output));

    output.erase();
    if (comm.Secure())
    {
	if (output.size())
	    output << ", ";
	if (comm.L_Secure())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE");
	if (comm.L_Secure())
	    output << IRC_Off;
    }
    if (comm.Private())
    {
	if (output.size())
	    output << ", ";
	if (comm.L_Private())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE");
	if (comm.L_Private())
	    output << IRC_Off;
    }
    if (comm.OpenMemos())
    {
	if (output.size())
	    output << ", ";
	if (comm.L_OpenMemos())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS");
	if (comm.L_OpenMemos())
	    output << IRC_Off;
    }
    if (output.size())
	SEND(mynick, source, "COMMSERV_INFO/OPTIONS", (
			output));
    }
    { RLOCK2(("Events"));
    if (Magick::instance().servmsg.ShowSync() && Magick::instance().events != NULL)
	SEND(mynick, source, "MISC/SYNC", (
			Magick::instance().events->SyncTime(source)));
    }
}

void CommServ::do_member_Add(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_member_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring member    = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(member))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				member));
	return;
    }

    member = Magick::instance().getSname(member);
    if (Magick::instance().nickserv.GetStored(member).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
				member));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (comm.IsIn(member))
    {
	SEND(mynick, source, "LIST/EXISTS2", (
				member, committee,
				Magick::instance().getMessage(source, "LIST/MEMBER")));
	return;
    }

    comm.insert(member, source);
    }
    Magick::instance().commserv.stats.i_Member++;
    SEND(mynick, source, "LIST/ADD2", (
				member, committee,
				Magick::instance().getMessage(source, "LIST/MEMBER")));
    LOG(LM_DEBUG, "COMMSERV/MEMBER_ADD", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	member, committee));
}


void CommServ::do_member_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_member_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring member    = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
					committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
					committee));
	return;
    }

    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
					committee));
	return;
    }

    if (comm.IsHead(member))
    {
	if (comm.HeadCom().empty())
	    SEND(mynick, source, "COMMSERV/OTH_HEAD", (
				member,  committee));
	else
	    SEND(mynick, source, "COMMSERV/OTH_MEMBER", (
				member,  comm.HeadCom()));
	return;
    }

    MLOCK(("CommServ", "list", committee.UpperCase(), "member"));
    if (comm.find(member))
    {
	Magick::instance().commserv.stats.i_Member++;
	SEND(mynick, source, "LIST/DEL2", (
			comm.member->Entry(), committee,
			Magick::instance().getMessage(source, "LIST/MEMBER")));
	LOG(LM_DEBUG, "COMMSERV/MEMBER_DEL", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		comm.member->Entry(), committee));
	comm.erase();
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS2", (
				member, committee,
				Magick::instance().getMessage(source, "LIST/MEMBER")));
    }
}

void CommServ::do_member_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_member_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONDYNAMIC", (
				message));
	return;
    }

    if (Magick::instance().commserv.GetList(committee).Private() &&
	!(Magick::instance().commserv.GetList(committee).IsOn(source) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View()).IsOn(source))))
    {
	SEND(mynick, source, "COMMSERV/NOTMEMBER", (
				committee));
	return;
    }

    SEND(mynick, source, "LIST/DISPLAY2", (
		committee, Magick::instance().getMessage(source, "LIST/MEMBER")));
    CommServ::do_member_List2(mynick, source, committee, true, 1);
}


int CommServ::do_member_List2(const mstring &mynick, const mstring &source,
	const mstring &committee, const bool first, const int number)
{
    FT("CommServ::do_member_List2", (mynick, source, committee, first, number));

    int nextnum = number;
    mstring output;

    if (!Magick::instance().commserv.IsList(committee))
    {
	RET(0);
    }
    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.HeadCom().empty())
    {
	if (Magick::instance().commserv.IsList(comm.HeadCom()))
	{
	    nextnum += CommServ::do_member_List2(mynick, source, comm.HeadCom(), false, nextnum);
	}
    }
    else if (!comm.Head().empty())
    {
	output.erase();
	output << nextnum++ << ". " << IRC_Bold;
	if (Magick::instance().nickserv.IsStored(comm.Head()))
	    output << Magick::instance().getSname(comm.Head());
	else
	    output << Magick::instance().getMessage(source, "COMMSERV/DEFUNCT") <<
	    	" " << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_HEAD");
	output << IRC_Off;
	if (!Magick::instance().nickserv.IsStored(comm.Head()))
		output << " [" << Magick::instance().getMessage(source, "COMMSERV/DEFUNCT") << "]";
	::send(mynick, source, output);
    }

    MLOCK(("CommServ", "list", comm.Name().UpperCase(), "member"));
    for (comm.member = comm.begin(); comm.member != comm.end(); comm.member++)
    {
	output.erase();
	output << nextnum++ << ". " << (first ? Blank : IRC_Bold) <<
		comm.member->Entry() << (first ? Blank : IRC_Off);
	if (!Magick::instance().nickserv.IsStored(comm.member->Entry()))
	    output << " [" << Magick::instance().getMessage(source, "COMMSERV/DEFUNCT") << "]";
	::send(mynick, source, output);
	::send(mynick, source, "    " +
		parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
		mVarArray(ToHumanTime(comm.member->Last_Modify_Time().SecondsSince(),
		source), comm.member->Last_Modifier())));
    }
    RET(nextnum-number);
}

void CommServ::do_logon_Add(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_logon_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring msgnum    = params.After(" ", 3);

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (comm.MSG_size() >= Magick::instance().commserv.Max_Logon())
    {
	SEND(mynick, source, "COMMSERV/MAX_MESSAGES", (
		committee));
	return;
    }

    comm.MSG_insert(msgnum, source);
    Magick::instance().commserv.stats.i_Logon++;
    SEND(mynick, source, "LIST/ADD2_NUMBER", (
		comm.MSG_size(), committee,
		Magick::instance().getMessage(source, "LIST/MESSAGES")));
    }
    LOG(LM_INFO, "COMMSERV/LOGON_ADD", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	committee));
}


void CommServ::do_logon_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_logon_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring msgnum    = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (!msgnum.IsNumber() || msgnum.Contains(".") || msgnum.Contains("-"))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }
    
    unsigned int num = atoi(msgnum.c_str());
    if (num <= 0 || num > Magick::instance().commserv.GetList(committee).MSG_size())
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
			1, Magick::instance().commserv.GetList(committee).MSG_size()));
    }

    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    MLOCK(("CommServ", "list", committee.UpperCase(), "message"));
    if (comm.MSG_find(num))
    {
	Magick::instance().commserv.stats.i_Logon++;
	SEND(mynick, source, "LIST/DEL2_NUMBER", (
		num, committee,
		Magick::instance().getMessage(source, "LIST/MESSAGES")));
	LOG(LM_INFO, "COMMSERV/LOGON_DEL", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		committee));
	comm.MSG_erase();
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS2_NUMBER", (
			num, committee,
			Magick::instance().getMessage(source, "LIST/MESSAGES")));
    }
}


void CommServ::do_logon_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_logon_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source) &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View()).IsOn(source)))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (!comm.MSG_size())
    {
	SEND(mynick, source, "LIST/EMPTY2", (
		committee, Magick::instance().getMessage(source, "LIST/MESSAGES")));
	return;
    }

    int i;
    mstring output;
    SEND(mynick, source, "LIST/DISPLAY2", (
		committee, Magick::instance().getMessage(source, "LIST/MESSAGES")));
    MLOCK(("CommServ", "list", committee.UpperCase(), "message"));
    for (i=1, comm.message = comm.MSG_begin();
	comm.message != comm.MSG_end(); comm.message++, i++)
    {
	output.erase();
	output << i << ". " << comm.message->Entry();
	::send(mynick, source, output);
	::send(mynick, source, "    " +
		parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
		mVarArray(ToHumanTime(comm.message->Last_Modify_Time().SecondsSince(),
		source), comm.message->Last_Modifier())));
    }
}


void CommServ::do_set_Head(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_set_Head", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring newhead   = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(newhead))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				newhead));
	return;
    }

    newhead = Magick::instance().getSname(newhead);
    if (Magick::instance().nickserv.GetStored(newhead).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
				newhead));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!(comm.IsHead(source) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_Owner()) &&
	 Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_Owner()).IsOn(source))))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (comm.Head().empty())
    {
	SEND(mynick, source, "COMMSERV/MULTI_HEAD", (
				committee));
	return;
    }

    if (newhead.IsSameAs(comm.Head(), true))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (
				message));
	return;
    }

    comm.Head(newhead);
    }
    Magick::instance().commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO", (
	Magick::instance().getMessage(source, "COMMSERV_INFO/SET_HEAD"),
	committee, newhead));
    LOG(LM_INFO, "COMMSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_HEAD"),
	committee, newhead));
}


void CommServ::do_set_Description(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_set_Description", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring desc      = params.After(" ", 3);

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    comm.Description(desc);
    }
    Magick::instance().commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO", (
	Magick::instance().getMessage(source, "COMMSERV_INFO/SET_DESCRIPTION"),
	committee, desc));
    LOG(LM_INFO, "COMMSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_DESCRIPTION"),
	committee, desc));
}


void CommServ::do_set_Email(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_set_Email", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring email     = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (email.IsSameAs("none", true))
	email.erase();
    else if (!email.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_EMAIL"), '@'));
	return;
    }
    else if (email.WordCount("@") != 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_EMAIL"), '@'));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    comm.Email(email);
    }
    Magick::instance().commserv.stats.i_Set++;
    if (email.empty())
    {
	SEND(mynick, source, "COMMSERV/UNSET", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_EMAIL"),
		committee));
	LOG(LM_INFO, "COMMSERV/UNSET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_EMAIL"),
		committee));
    }
    else
    {
	SEND(mynick, source, "COMMSERV/SET_TO", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_EMAIL"),
		committee, email));
	LOG(LM_INFO, "COMMSERV/SET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("COMMSERV_INFO/SET_EMAIL"),
		committee, email));
    }
}


void CommServ::do_set_URL(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_set_URL", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring url       = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (url.IsSameAs("none", true))
	url.erase();

    if (url.SubString(0, 6).IsSameAs("http://", true))
    {
	url.erase(0, 6);
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    comm.URL(url);
    }
    Magick::instance().commserv.stats.i_Set++;
    if (url.empty())
    {
	SEND(mynick, source, "COMMSERV/UNSET", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_URL"),
		committee));
	LOG(LM_INFO, "COMMSERV/UNSET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_URL"),
		committee));
    }
    else
    {
	SEND(mynick, source, "COMMSERV/SET_TO", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_URL"),
		committee, "http://" + url));
	LOG(LM_INFO, "COMMSERV/SET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("COMMSERV_INFO/SET_URL"),
		committee, "http://" + url));
    }
}


void CommServ::do_set_Secure(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_set_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (comm.L_Secure())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE"),
		committee));
	return;
    }

    comm.Secure(onoff.GetBool());
    }
    Magick::instance().commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE"),
		committee, (onoff.GetBool() ?
			Magick::instance().getMessage(source, "VALS/ON") :
			Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_SECURE"),
	committee, (onoff.GetBool() ?
		Magick::instance().getMessage(source, "VALS/ON") :
		Magick::instance().getMessage(source, "VALS/OFF"))));
}


void CommServ::do_set_Private(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_set_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (comm.L_Private())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE"),
		committee));
	return;
    }

    comm.Private(onoff.GetBool());
    }
    Magick::instance().commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE"),
		committee, (onoff.GetBool() ?
			Magick::instance().getMessage(source, "VALS/ON") :
			Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_PRIVATE"),
	committee, (onoff.GetBool() ?
		Magick::instance().getMessage(source, "VALS/ON") :
		Magick::instance().getMessage(source, "VALS/OFF"))));
}


void CommServ::do_set_OpenMemos(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_set_OpenMemos", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_OpenMemos())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (comm.L_OpenMemos())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"),
		committee));
	return;
    }

    comm.OpenMemos(onoff.GetBool());
    }
    Magick::instance().commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"),
		committee, (onoff.GetBool() ?
			Magick::instance().getMessage(source, "VALS/ON") :
			Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_OPENMEMOS"),
	committee, (onoff.GetBool() ?
		Magick::instance().getMessage(source, "VALS/ON") :
		Magick::instance().getMessage(source, "VALS/OFF"))));
}


void CommServ::do_lock_Secure(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_lock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Magick::instance().commserv.LCK_Secure())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE"),
		committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    comm.L_Secure(false);
    comm.Secure(onoff.GetBool());
    comm.L_Secure(true);
    }
    Magick::instance().commserv.stats.i_Lock++;
    SEND(mynick, source, "COMMSERV/LOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE"),
		committee, (onoff.GetBool() ?
			Magick::instance().getMessage(source, "VALS/ON") :
			Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/LOCKED", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_SECURE"),
	committee, (onoff.GetBool() ?
		Magick::instance().getMessage(source, "VALS/ON") :
		Magick::instance().getMessage(source, "VALS/OFF"))));
}


void CommServ::do_lock_Private(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_lock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Magick::instance().commserv.LCK_Private())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE"),
		committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    comm.L_Private(false);
    comm.Private(onoff.GetBool());
    comm.L_Private(true);
    }
    Magick::instance().commserv.stats.i_Lock++;
    SEND(mynick, source, "COMMSERV/LOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE"),
		committee, (onoff.GetBool() ?
			Magick::instance().getMessage(source, "VALS/ON") :
			Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/LOCKED", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_PRIVATE"),
	committee, (onoff.GetBool() ?
		Magick::instance().getMessage(source, "VALS/ON") :
		Magick::instance().getMessage(source, "VALS/OFF"))));
}


void CommServ::do_lock_OpenMemos(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_lock_OpenMemos", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_OpenMemos())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Magick::instance().commserv.LCK_OpenMemos())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"),
		committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Magick::instance().commserv.GetList(committee);

    comm.L_OpenMemos(false);
    comm.OpenMemos(onoff.GetBool());
    comm.L_OpenMemos(true);
    }
    Magick::instance().commserv.stats.i_Lock++;
    SEND(mynick, source, "COMMSERV/LOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"),
		committee, (onoff.GetBool() ?
			Magick::instance().getMessage(source, "VALS/ON") :
			Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/LOCKED", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_OPENMEMOS"),
	committee, (onoff.GetBool() ?
		Magick::instance().getMessage(source, "VALS/ON") :
		Magick::instance().getMessage(source, "VALS/OFF"))));
}


void CommServ::do_unlock_Secure(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_unlock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Magick::instance().commserv.LCK_Secure())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE"),
		committee));
	return;
    }

    Magick::instance().commserv.GetList(committee).L_Secure(false);
    Magick::instance().commserv.stats.i_Unlock++;
    SEND(mynick, source, "COMMSERV/UNLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE")));
    LOG(LM_INFO, "COMMSERV/UNLOCKED", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_SECURE"),
	committee));
}


void CommServ::do_unlock_Private(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_unlock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Magick::instance().commserv.LCK_Private())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE"),
		committee));
	return;
    }

    Magick::instance().commserv.GetList(committee).L_Private(false);
    Magick::instance().commserv.stats.i_Unlock++;
    SEND(mynick, source, "COMMSERV/UNLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE")));
    LOG(LM_INFO, "COMMSERV/UNLOCKED", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_PRIVATE"),
	committee));
}


void CommServ::do_unlock_OpenMemos(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_unlock_OpenMemos", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() ||
	committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() ||
	committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Magick::instance().commserv.LCK_OpenMemos())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"),
		committee));
	return;
    }

    Magick::instance().commserv.GetList(committee).L_OpenMemos(false);
    Magick::instance().commserv.stats.i_Unlock++;
    SEND(mynick, source, "COMMSERV/UNLOCKED", (
		Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS")));
    LOG(LM_INFO, "COMMSERV/UNLOCKED", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("COMMSERV_INFO/SET_OPENMEMOS"),
	committee));
}


SXP::Tag Committee_t::tag_Committee_t("Committee_t");
SXP::Tag Committee_t::tag_Name("Name");
SXP::Tag Committee_t::tag_RegTime("RegTime");
SXP::Tag Committee_t::tag_HeadCom("HeadCom");
SXP::Tag Committee_t::tag_Head("Head");
SXP::Tag Committee_t::tag_Description("Description");
SXP::Tag Committee_t::tag_Email("EMail");
SXP::Tag Committee_t::tag_URL("URL");
SXP::Tag Committee_t::tag_set_OpenMemos("SET_OpenMemos");
SXP::Tag Committee_t::tag_set_Private("SET_Private");
SXP::Tag Committee_t::tag_set_Secure("SET_Secure");
SXP::Tag Committee_t::tag_lock_OpenMemos("LOCK_OpenMemos");
SXP::Tag Committee_t::tag_lock_Private("LOCK_Private");
SXP::Tag Committee_t::tag_lock_Secure("LOCK_Secure");
SXP::Tag Committee_t::tag_Members("Members");
SXP::Tag Committee_t::tag_Messages("Messages");
SXP::Tag Committee_t::tag_UserDef("UserDef");

void Committee_t::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Committee_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    if (!(i_Name == Magick::instance().commserv.SADMIN_Name() ||
		i_Name == Magick::instance().commserv.ALL_Name() ||
		i_Name == Magick::instance().commserv.REGD_Name()))
    {
	if( pElement->IsA(tag_Members) )
	{
	    entlist_t *tmp = new entlist_t;
	    members_array.push_back(tmp);
	    pIn->ReadTo(tmp);
	}
    }

    if( pElement->IsA(tag_Messages) )
    {
	entlist_t *tmp = new entlist_t;
	messages_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_UserDef) )
    {
	mstring *tmp = new mstring;
	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void Committee_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    static_cast<void>(pIn);

    FT("Committee_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
	if( pElement->IsA(tag_Name) )		pElement->Retrieve(i_Name);
	if( pElement->IsA(tag_RegTime) )	pElement->Retrieve(i_RegTime);
	if( pElement->IsA(tag_HeadCom) )	pElement->Retrieve(i_HeadCom);
	if( pElement->IsA(tag_Head) )		pElement->Retrieve(i_Head);
	if( pElement->IsA(tag_Description) )	pElement->Retrieve(i_Description);
	if( pElement->IsA(tag_Email) )		pElement->Retrieve(i_Email);
	if( pElement->IsA(tag_URL) )		pElement->Retrieve(i_URL);
	if( pElement->IsA(tag_set_OpenMemos) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    setting.OpenMemos = tmp;
	}
	if( pElement->IsA(tag_set_Private) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    setting.Private = tmp;
	}
	if( pElement->IsA(tag_set_Secure) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    setting.Secure = tmp;
	}
	if( pElement->IsA(tag_lock_OpenMemos) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    lock.OpenMemos = tmp;
	}
 	if( pElement->IsA(tag_lock_Private) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    lock.Private = tmp;
	}
	if( pElement->IsA(tag_lock_Secure) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    lock.Secure = tmp;
	}
}

void Committee_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    static_cast<void>(attribs);

    FT("Committee_t::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    //TODO: Add your source code here
	pOut->BeginObject(tag_Committee_t);

	WLOCK(("CommServ", "list", i_Name.UpperCase()));
	pOut->WriteElement(tag_Name, i_Name);
	pOut->WriteElement(tag_RegTime, i_RegTime);
	pOut->WriteElement(tag_HeadCom, i_HeadCom);
	pOut->WriteElement(tag_Head, i_Head);
	pOut->WriteElement(tag_Description, i_Description);
	pOut->WriteElement(tag_Email, i_Email);
	pOut->WriteElement(tag_URL, i_URL);
	pOut->WriteElement(tag_set_OpenMemos, setting.OpenMemos);
	pOut->WriteElement(tag_set_Private, setting.Private);
	pOut->WriteElement(tag_set_Secure, setting.Secure);
	pOut->WriteElement(tag_lock_OpenMemos, lock.OpenMemos);
	pOut->WriteElement(tag_lock_Private, lock.Private);
	pOut->WriteElement(tag_lock_Secure, lock.Secure);

	if (!(i_Name == Magick::instance().commserv.ALL_Name() ||
	      i_Name == Magick::instance().commserv.REGD_Name() ||
	      i_Name == Magick::instance().commserv.SADMIN_Name()))
	{
	    entlist_ui l;
	    for(l=i_Members.begin(); l!=i_Members.end(); l++)
	    {
		pOut->BeginObject(tag_Members);
		pOut->WriteSubElement(const_cast<entlist_t *>(&(*l)));
		pOut->EndObject(tag_Members);
	    }
	}

	entlist_i k;
	for(k=i_Messages.begin(); k!=i_Messages.end(); k++)
	{
	    pOut->BeginObject(tag_Messages);
	    pOut->WriteSubElement(&(*k));
	    pOut->EndObject(tag_Messages);
	}

        map<mstring,mstring>::const_iterator iter;
        for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
        {
            pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
        }

	pOut->EndObject(tag_Committee_t);
}

SXP::Tag CommServ::tag_CommServ("CommServ");

void CommServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("CommServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Committee_t *c = new Committee_t;

    if( pElement->IsA( c->GetClassTag() ) )
    {
	c_array.push_back(c);
	pIn->ReadTo(c);
    }
    else
    {
	delete c;
    }
}

void CommServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    static_cast<void>(pIn);
    static_cast<void>(pElement);

    FT("CommServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void CommServ::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    static_cast<void>(attribs);

    FT("CommServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_CommServ);

    CommServ::list_t::iterator iter;
    { RLOCK(("CommServ", "list"));
    for (iter = i_list.begin(); iter != i_list.end(); iter++)
	pOut->WriteSubElement(&iter->second);
    }

    pOut->EndObject(tag_CommServ);
}


void CommServ::PostLoad()
{
    NFT("CommServ::PostLoad");
    // Linkage, etc
    unsigned int i, j;
    for (i=0; i<c_array.size(); i++)
    {
	if (c_array[i] != NULL)
	{
	    for (j=0; j<c_array[i]->members_array.size(); j++)
	    {
		c_array[i]->i_Members.insert(*c_array[i]->members_array[j]);
		delete c_array[i]->members_array[j];
	    }
	    c_array[i]->members_array.clear();
	    for (j=0; j<c_array[i]->messages_array.size(); j++)
	    {
		c_array[i]->i_Messages.push_back(*c_array[i]->messages_array[j]);
		delete c_array[i]->messages_array[j];
	    }
	    c_array[i]->messages_array.clear();
	    for (j=0; j<c_array[i]->ud_array.size(); j++)
	    {
		if (c_array[i]->ud_array[j] != NULL)
		{
		    if (c_array[i]->ud_array[j]->Contains("\n"))
			c_array[i]->i_UserDef[c_array[i]->ud_array[j]->Before("\n")] =
				c_array[i]->ud_array[j]->After("\n");
		    delete c_array[i]->ud_array[j];
		}
	    }
	    c_array[i]->ud_array.clear();
	    if (!c_array[i]->Name().empty())
		AddList(c_array[i]);
	    delete c_array[i];
	}
    }
    c_array.clear();

    CommServ::list_t::iterator iter;
    for (iter=i_list.begin(); iter!=i_list.end(); iter++)
    {
	for (iter->second.member = iter->second.begin();
		iter->second.member != iter->second.end();
		iter->second.member++)
	{
	    iter->second.member->PostLoad();
	}
	for (iter->second.message = iter->second.MSG_begin();
		iter->second.message != iter->second.MSG_end();
		iter->second.message++)
	    iter->second.message->PostLoad();

	// We must ensure certain settings in pre-defined committees ...
	// So check what committee this is, and if its one of them, then
	// ensure the values we need in place are what we have.  This
	// saves us from data tampering, and also allows us to change
	// committee names on the fly and have the correct knock-on effect.
	// Also ensures magick.ini settings are correctly set.
	if (iter->first == Magick::instance().commserv.SADMIN_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom.erase();
	    iter->second.i_Members.clear();
	    for (j=1; j<=Magick::instance().operserv.Services_Admin().WordCount(", "); j++)
		iter->second.i_Members.insert(entlist_t(
			Magick::instance().operserv.Services_Admin().ExtractWord(j, ", "),
			Magick::instance().operserv.FirstName()));
	    iter->second.Secure(SADMIN_Secure());
	    iter->second.Private(SADMIN_Private());
	    iter->second.OpenMemos(SADMIN_OpenMemos());
	}
	else if (iter->first == Magick::instance().commserv.SOP_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom = Magick::instance().commserv.SADMIN_Name();
	    iter->second.Secure(SOP_Secure());
	    iter->second.Private(SOP_Private());
	    iter->second.OpenMemos(SOP_OpenMemos());
	}
	else if (iter->first == Magick::instance().commserv.ADMIN_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom = Magick::instance().commserv.SADMIN_Name();
	    iter->second.Secure(ADMIN_Secure());
	    iter->second.Private(ADMIN_Private());
	    iter->second.OpenMemos(ADMIN_OpenMemos());
	}
	else if (iter->first == Magick::instance().commserv.OPER_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom = Magick::instance().commserv.ADMIN_Name();
	    iter->second.Secure(OPER_Secure());
	    iter->second.Private(OPER_Private());
	    iter->second.OpenMemos(OPER_OpenMemos());
	}
	else if (iter->first == Magick::instance().commserv.ALL_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom = Magick::instance().commserv.ADMIN_Name();
	    iter->second.i_Members.clear();
	    iter->second.Secure(false);
	    iter->second.Private(true);
	    iter->second.OpenMemos(false);
	}
	else if (iter->first == Magick::instance().commserv.REGD_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom = Magick::instance().commserv.SOP_Name();
	    iter->second.i_Members.clear();
	    iter->second.Secure(false);
	    iter->second.Private(true);
	    iter->second.OpenMemos(false);
	}
    }
}
