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


Committee_t::Committee_t(const mstring& name, const mstring& head, const mstring& description)
	: i_Name(name.UpperCase()), i_RegTime(mDateTime::CurrentDateTime()),
	  i_Head(head.LowerCase()), i_Description(description),
	  i_Private(Parent->commserv.DEF_Private()), l_Private(false),
	  i_OpenMemos(Parent->commserv.DEF_OpenMemos()), l_OpenMemos(false),
	  i_Secure(Parent->commserv.DEF_Secure()), l_Secure(false)
{
    FT("Committee_t::Committee_t", (name, head, description));

    DumpE();
}


Committee_t::Committee_t(const mstring& name, const Committee_t& head, const mstring& description)
	: i_Name(name.UpperCase()), i_RegTime(mDateTime::CurrentDateTime()),
	  i_HeadCom(head.Name()), i_Description(description),
	  i_Private(Parent->commserv.DEF_Private()), l_Private(false),
	  i_OpenMemos(Parent->commserv.DEF_OpenMemos()), l_OpenMemos(false),
	  i_Secure(Parent->commserv.DEF_Secure()), l_Secure(false)
{
    FT("Committee_t::Committee_t", (name, "(Committee_t *) head", description));
    DumpE();
}


Committee_t::Committee_t(const mstring& name, const mstring& description)
	: i_Name(name.UpperCase()), i_RegTime(mDateTime::CurrentDateTime()),
	  i_Description(description),
	  i_Private(Parent->commserv.DEF_Private()), l_Private(false),
	  i_OpenMemos(Parent->commserv.DEF_OpenMemos()), l_OpenMemos(false),
	  i_Secure(Parent->commserv.DEF_Secure()), l_Secure(false)
{
    FT("Committee_t::Committee_t", (name, description));
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
    i_OpenMemos = in.i_OpenMemos;
    l_OpenMemos = in.l_OpenMemos;
    i_Secure = in.i_Secure;
    l_Secure = in.l_Secure;
    i_Private = in.i_Private;
    l_Private = in.l_Private;
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

    if (i_Name == Parent->commserv.ALL_Name() &&
	Parent->nickserv.IsLive(nick))
    {
	RET(true);
    }

    if (!Parent->nickserv.IsStored(nick))
    {
	RET(false);
    }

    if (i_Name == Parent->commserv.REGD_Name())
    {
	RET(true);
    }

    mstring target = Parent->nickserv.GetStored(nick).Host();
    if (target.empty())
	target = nick.LowerCase();

    // We're a HEAD, in by DEFAULT
    { RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_HeadCom"));
    if (!i_HeadCom.empty() && Parent->commserv.IsList(i_HeadCom) &&
	Parent->commserv.GetList(i_HeadCom).IsIn(target))
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
    if (i_Name == Parent->commserv.SADMIN_Name())
    {
	unsigned int i;
	for (i=0; i<Parent->nickserv.GetStored(target).Siblings(); i++)
	{
	    for (i=0; i<members.size(); i++)
	    {
		if (Parent->nickserv.GetStored(target).Sibling(i).IsSameAs(members[i], true))
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
    if (!Parent->nickserv.IsLive(nick))
    {
	RET(false);
    }

    if (i_Name == Parent->commserv.ALL_Name())
    {
	RET(true);
    }

    // The committee we're looking at has ModeO set,
    // but user doesnt have umode +o.
    if (((i_Name == Parent->commserv.SADMIN_Name() &&
	Parent->commserv.SADMIN_ModeO()) ||
	(i_Name == Parent->commserv.SOP_Name() &&
	Parent->commserv.SOP_ModeO()) ||
	(i_Name == Parent->commserv.ADMIN_Name() &&
	Parent->commserv.ADMIN_ModeO()) ||
	(i_Name == Parent->commserv.OPER_Name() &&
	Parent->commserv.OPER_ModeO())) &&
	!Parent->nickserv.GetLive(nick).HasMode("o"))
    {
	RET(false);
    }

    // This returns wether we're CURRENTLY recognized
    // as 'in' the committee (secure, and nick secure
    // taken into account).
    if (IsIn(nick) && Parent->nickserv.IsStored(nick) &&
	Parent->nickserv.GetStored(nick).IsOnline())
    {
	RLOCK_IF(("CommServ", "list", i_Name.UpperCase(), "i_Secure"),
		!i_Secure || Parent->nickserv.GetLive(nick).IsIdentified())
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
	!i_Head.empty() && i_Head.IsSameAs(nick, false))
    {
	RET(true);
    }
    else
    {
	RLOCK_IF(("CommServ", "list", i_Name.UpperCase(), "i_HeadCom"),
	    !i_HeadCom.empty() && Parent->commserv.IsList(i_HeadCom))
	{
	    if (Parent->commserv.GetList(i_HeadCom).IsIn(nick))
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
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Private"));
	MCB(i_Private);
	i_Private = in;
	MCE(i_Private);
    }
}

bool Committee_t::Private() const
{
    NFT("Committee_t::Private");
    if (!Parent->commserv.LCK_Private())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Private"));
	RET(i_Private);
    }
    RET(Parent->commserv.DEF_Private());
}


void Committee_t::L_Private(const bool in)
{
    FT("Committee_t::L_Private", (in));
    if (!Parent->commserv.LCK_Private())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "l_Private"));
	MCB(l_Private);
	l_Private = in;
	MCE(l_Private);
    }
}


bool Committee_t::L_Private() const
{
    NFT("Committee_t::L_Private");
    if (!Parent->commserv.LCK_Private())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "l_Private"));
	RET(l_Private);
    }
    RET(true);
}


void Committee_t::Secure(const bool in)
{
    FT("Committee_t::Secure", (in));
    if (!L_Secure())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Secure"));
	MCB(i_Secure);
	i_Secure = in;
	MCE(i_Secure);
    }
}


bool Committee_t::Secure() const
{
    NFT("Committee_t::Secure");
    if (!Parent->commserv.LCK_Secure())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Secure"));
	RET(i_Secure);
    }
    RET(Parent->commserv.DEF_Secure());
}


void Committee_t::L_Secure(const bool in)
{
    FT("Committee_t::L_Secure", (in));
    if (!Parent->commserv.LCK_Secure())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "l_Secure"));
	MCB(l_Secure);
	l_Secure = in;
	MCE(l_Secure);
    }
}


bool Committee_t::L_Secure() const
{
    NFT("Committee_t::L_Secure");
    if (!Parent->commserv.LCK_Secure())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "l_Secure"));
	RET(l_Secure);
    }
    RET(true);
}


void Committee_t::OpenMemos(const bool in)
{
    FT("Committee_t::OpenMemos", (in));
    if (!L_OpenMemos())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_OpenMemos"));
	MCB(i_OpenMemos);
	i_OpenMemos = in;
	MCE(i_OpenMemos);
    }
}


bool Committee_t::OpenMemos() const
{
    NFT("Committee_t::OpenMemos");
    if (!Parent->commserv.LCK_OpenMemos())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_OpenMemos"));
	RET(i_OpenMemos);
    }
    RET(Parent->commserv.DEF_OpenMemos());
}


void Committee_t::L_OpenMemos(const bool in)
{
    FT("Committee_t::L_OpenMemos", (in));
    if (!Parent->commserv.LCK_OpenMemos())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "l_OpenMemos"));
	MCB(l_OpenMemos);
	l_OpenMemos = in;
	MCE(l_OpenMemos);
    }
}


bool Committee_t::L_OpenMemos() const
{
    NFT("Committee_t::L_OpenMemos");
    if (!Parent->commserv.LCK_OpenMemos())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "l_OpenMemos"));
	RET(l_OpenMemos);
    }
    RET(true);
}


bool Committee_t::MSG_insert(const mstring& entry, const mstring& nick,
	const mDateTime& time)
{
    FT("Committee_t::MSG_insert", (entry, nick, time));

    MLOCK(("CommServ", "list", i_Name.UpperCase(), "message"));
    if (IsHead(nick))
    {
	MCB(i_Messages.size());
	i_Messages.push_back(entlist_t(entry, nick, time));
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

    retval += sizeof(i_Private);
    retval += sizeof(l_Private);
    retval += sizeof(i_OpenMemos);
    retval += sizeof(l_OpenMemos);
    retval += sizeof(i_Secure);
    retval += sizeof(l_Secure);

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
	i_URL, i_Members.size(), i_Private, l_Private, i_OpenMemos,
	l_OpenMemos, i_Secure, l_Secure, i_Messages.size(), i_UserDef.size()));
}

void Committee_t::DumpE() const
{
    ME(0, (i_Name, i_RegTime, i_HeadCom, i_Head, i_Description, i_Email,
	i_URL, i_Members.size(), i_Private, l_Private, i_OpenMemos,
	l_OpenMemos, i_Secure, l_Secure, i_Messages.size(), i_UserDef.size()));
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

    Parent->commands.AddSystemCommand(GetInternalName(),
		"MEMB* ADD", Parent->commserv.REGD_Name(), CommServ::do_member_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"MEMB* DEL*", Parent->commserv.REGD_Name(), CommServ::do_member_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"MEMB* ERA*", Parent->commserv.REGD_Name(), CommServ::do_member_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"MEMB* LIST", Parent->commserv.REGD_Name(), CommServ::do_member_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"MEMB* VIEW", Parent->commserv.REGD_Name(), CommServ::do_member_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"MEMB* H*LP", Parent->commserv.REGD_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOG* ADD", Parent->commserv.REGD_Name(), CommServ::do_logon_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOG* DEL*", Parent->commserv.REGD_Name(), CommServ::do_logon_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOG* ERA*", Parent->commserv.REGD_Name(), CommServ::do_logon_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOG* LIST", Parent->commserv.REGD_Name(), CommServ::do_logon_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOG* VIEW", Parent->commserv.REGD_Name(), CommServ::do_logon_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOG* H*LP", Parent->commserv.REGD_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* HEAD*", Parent->commserv.REGD_Name(), CommServ::do_set_Head);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* DESC*", Parent->commserv.REGD_Name(), CommServ::do_set_Description);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* E*MAIL*", Parent->commserv.REGD_Name(), CommServ::do_set_Email);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* U*R*L*", Parent->commserv.REGD_Name(), CommServ::do_set_URL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* SEC*", Parent->commserv.REGD_Name(), CommServ::do_set_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PRIV*", Parent->commserv.REGD_Name(), CommServ::do_set_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* *MEMO*", Parent->commserv.REGD_Name(), CommServ::do_set_OpenMemos);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* H*LP", Parent->commserv.REGD_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK SEC*", Parent->commserv.SOP_Name(), CommServ::do_lock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK PRIV*", Parent->commserv.SOP_Name(), CommServ::do_lock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK *MEMO*", Parent->commserv.SOP_Name(), CommServ::do_lock_OpenMemos);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK H*LP", Parent->commserv.SOP_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Parent->commserv.SOP_Name(), CommServ::do_unlock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Parent->commserv.SOP_Name(), CommServ::do_unlock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK *MEMO*", Parent->commserv.SOP_Name(), CommServ::do_unlock_OpenMemos);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK H*LP", Parent->commserv.SOP_Name(), do_3param);

    Parent->commands.AddSystemCommand(GetInternalName(),
		"H*LP", Parent->commserv.ALL_Name(), CommServ::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ADD*", Parent->commserv.SOP_Name(), CommServ::do_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"DEL*", Parent->commserv.SOP_Name(), CommServ::do_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ERA*", Parent->commserv.SOP_Name(), CommServ::do_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LIST", Parent->commserv.ALL_Name(), CommServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"*MEMO*", Parent->commserv.REGD_Name(), CommServ::do_Memo);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"INFO", Parent->commserv.ALL_Name(), CommServ::do_Info);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Parent->commands.AddSystemCommand(GetInternalName(),
		"MEMB* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"MEMB*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOG* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOG*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK *", Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK", Parent->commserv.SOP_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK *", Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK", Parent->commserv.SOP_Name(), do_1_3param);
}

void CommServ::RemCommands()
{
    NFT("CommServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.RemSystemCommand(GetInternalName(),
		"MEMB* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"MEMB* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"MEMB* ERA*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"MEMB* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"MEMB* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"MEMB* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOG* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOG* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOG* ERA*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOG* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOG* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOG* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* HEAD*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* DESC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* E*MAIL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* U*R*L*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* SEC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PRIV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* *MEMO*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK *MEMO*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK H*LP", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK *MEMO*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK H*LP", Parent->commserv.SOP_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
		"H*LP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ADD*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"DEL*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ERA*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LIST", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"*MEMO*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"INFO", Parent->commserv.ALL_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Parent->commands.RemSystemCommand(GetInternalName(),
		"MEMB* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"MEMB*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOG* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOG*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK *", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK *", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK", Parent->commserv.SOP_Name());
}


void CommServ::execute(mstring& source, const mstring& msgtype, const mstring& params)
{
    mThread::ReAttach(tt_OtherServ);
    FT("CommServ::execute", (source, msgtype, params));
    //okay this is the main commserv command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message
    mstring mynick(Parent->getLname(params.ExtractWord(1, ": ")));
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
	!Parent->commands.DoCommand(mynick, source, command, message))
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
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    mstring HelpTopic = Parent->commserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector<mstring> help = Parent->getHelp(source, HelpTopic.UpperCase());
					
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

    if (Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISSTORED", (
				committee));
	return;
    }

    if (!Parent->nickserv.IsStored(head))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				head));
	return;
    }

    head = Parent->getSname(head);
    if (Parent->nickserv.GetStored(head).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
				head));
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    Committee_t tmp(committee, head, desc);
    Parent->commserv.AddList(&tmp);
    Parent->commserv.stats.i_Add++;
    SEND(mynick, source, "COMMSERV/ADD", (
				committee, head));
    LOG(LM_NOTICE, "COMMSERV/ADD",
	(Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    Parent->commserv.RemList(committee);
    Parent->commserv.stats.i_Del++;
    SEND(mynick, source, "COMMSERV/DEL", ( committee));
    LOG(LM_NOTICE, "COMMSERV/DEL", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	committee));
}


void CommServ::do_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_List", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	mask = "*";
	listsize = Parent->config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Parent->config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Parent->config.Maxlist())
	{
	    mstring output;
	    SEND(mynick, source, "LIST/MAXLIST", (
					Parent->config.Maxlist()));
	    return;
	}
    }

    SEND(mynick, source, "LIST/COMM_LIST", (
					mask));
    CommServ::list_t::iterator iter;

    { RLOCK(("CommServ", "list"));
    for (iter = Parent->commserv.ListBegin(), i=0, count = 0;
			iter != Parent->commserv.ListEnd(); iter++)
    {
	if (iter->second.Name().Matches(mask, true))
	{
	    if (i < listsize && (!iter->second.Private() ||
		(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
		Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source))))
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
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONDYNAMIC", (
				message));
	return;
    }

    if (!Parent->commserv.GetList(committee).IsOn(source))
    {
	SEND(mynick, source, "COMMSERV/NOTMEMBER", (
				committee));
	return;
    }

    if (!Parent->commserv.GetList(committee).OpenMemos() &&
	!Parent->commserv.GetList(committee).IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    text.prepend("[" + IRC_Bold + committee + IRC_Off + "] ");
    CommServ::do_Memo2(source, committee, text);
    Parent->commserv.stats.i_Memo++;
    SEND(mynick, source, "COMMSERV/MEMO", (
				committee));
    LOG(LM_INFO, "COMMSERV/MEMO", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	committee));
}


void CommServ::do_Memo2(const mstring &source, const mstring &committee,
	const mstring &text)
{
    FT("CommServ::do_Memo2", (source, committee, text));

    if (!Parent->commserv.IsList(committee))
	return;
    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee.UpperCase());

    mstring realme;
    if (Parent->nickserv.IsStored(source))
	realme = Parent->nickserv.GetStored(source).Host();
    else
	return;
    if (realme.empty())
	realme = source;

    if (!comm.HeadCom().empty())
    {
	if (Parent->commserv.IsList(comm.HeadCom()))
	{
	    CommServ::do_Memo2(source, comm.HeadCom(), text);
	}
    }
    else if (!comm.Head().empty())
    {
	if (Parent->nickserv.IsStored(comm.Head()))
	{
	    mstring realrecipiant = Parent->nickserv.GetStored(comm.Head()).Host();
	    if (realrecipiant.empty())
		realrecipiant = comm.Head();
	    if (!realme.IsSameAs(realrecipiant, true))
	    {
		Memo_t tmp(realrecipiant, source, text);
		Parent->memoserv.AddNickMemo(&tmp);

		RLOCK2(("MemoServ", "nick", realrecipiant.LowerCase()));
		MemoServ::nick_memo_t &memolist = Parent->memoserv.GetNick(realrecipiant);
		RLOCK3(("NickServ", "stored", realrecipiant.LowerCase()));
		Nick_Stored_t &nick = Parent->nickserv.GetStored(realrecipiant);
		if (nick.IsOnline())
		    SEND(Parent->memoserv.FirstName(), nick.Name(), "MS_COMMAND/NS_NEW", (
			memolist.size(), Parent->memoserv.FirstName(),
			memolist.size()));
		unsigned int i;
		for (i=0; i < nick.Siblings(); i++)
		{
		    if (Parent->nickserv.IsStored(nick.Sibling(i)) &&
				Parent->nickserv.GetStored(nick.Sibling(i)).IsOnline())
		    {
			SEND(Parent->memoserv.FirstName(), nick.Sibling(i), "MS_COMMAND/NS_NEW", (
				memolist.size(), Parent->memoserv.FirstName(),
				memolist.size()));
		    }
		}
	    }
	}
    }

    MLOCK(("CommServ", "list", comm.Name().UpperCase(), "member"));
    for (comm.member = comm.begin(); comm.member != comm.end(); comm.member++)
    {
	if (Parent->nickserv.IsStored(comm.member->Entry()))
	{
	    mstring realrecipiant = Parent->nickserv.GetStored(comm.member->Entry()).Host();
	    if (realrecipiant.empty())
		realrecipiant = comm.member->Entry();
	    if (!realme.IsSameAs(realrecipiant, true))
	    {
		Memo_t tmp(realrecipiant, source, text);
		Parent->memoserv.AddNickMemo(&tmp);

		RLOCK2(("MemoServ", "nick", realrecipiant.LowerCase()));
		MemoServ::nick_memo_t &memolist = Parent->memoserv.GetNick(realrecipiant);
		RLOCK3(("NickServ", "stored", realrecipiant.LowerCase()));
		Nick_Stored_t &nick = Parent->nickserv.GetStored(realrecipiant);
		if (nick.IsOnline())
		    SEND(Parent->memoserv.FirstName(), nick.Name(), "MS_COMMAND/NS_NEW", (
			memolist.size(), Parent->memoserv.FirstName(),
			memolist.size()));
		unsigned int i;
		for (i=0; i < nick.Siblings(); i++)
		{
		    if (Parent->nickserv.IsStored(nick.Sibling(i)) &&
				Parent->nickserv.GetStored(nick.Sibling(i)).IsOnline())
		    {
			SEND(Parent->memoserv.FirstName(), nick.Sibling(i), "MS_COMMAND/NS_NEW", (
				memolist.size(), Parent->memoserv.FirstName(),
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
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONDYNAMIC", (
				message));
	return;
    }

    mstring output;
    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);
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
			Parent->getSname(comm.Head())));
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
	output << comm.size() << " " << Parent->getMessage(source, "COMMSERV_INFO/MEMBERS");
    }
    if (comm.MSG_size())
    {
	if (output.size())
	    output << ", ";
	output << comm.MSG_size() << " " << Parent->getMessage(source, "COMMSERV_INFO/MESSAGES");
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
	output << Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE");
	if (comm.L_Secure())
	    output << IRC_Off;
    }
    if (comm.Private())
    {
	if (output.size())
	    output << ", ";
	if (comm.L_Private())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE");
	if (comm.L_Private())
	    output << IRC_Off;
    }
    if (comm.OpenMemos())
    {
	if (output.size())
	    output << ", ";
	if (comm.L_OpenMemos())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS");
	if (comm.L_OpenMemos())
	    output << IRC_Off;
    }
    if (output.size())
	SEND(mynick, source, "COMMSERV_INFO/OPTIONS", (
			output));
    { RLOCK(("Events"));
    if (Parent->servmsg.ShowSync() && Parent->events != NULL)
	SEND(mynick, source, "MISC/SYNC", (
			Parent->events->SyncTime(source)));
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (!Parent->nickserv.IsStored(member))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				member));
	return;
    }

    member = Parent->getSname(member);
    if (Parent->nickserv.GetStored(member).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
				member));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

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
				Parent->getMessage(source, "LIST/MEMBER")));
	return;
    }

    comm.insert(member, source);
    }
    Parent->commserv.stats.i_Member++;
    SEND(mynick, source, "LIST/ADD2", (
				member, committee,
				Parent->getMessage(source, "LIST/MEMBER")));
    LOG(LM_DEBUG, "COMMSERV/MEMBER_ADD", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
					committee));
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
					committee));
	return;
    }

    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
					committee));
	return;
    }

    if (comm.IsHead(member))
    {
	SEND(mynick, source, "COMMSERV/OTH_HEAD", (
				member, 
				Parent->getMessage(source, "LIST/MEMBER")));
	return;
    }

    MLOCK(("CommServ", "list", committee.UpperCase(), "member"));
    if (comm.find(member))
    {
	Parent->commserv.stats.i_Member++;
	SEND(mynick, source, "LIST/DEL2", (
			comm.member->Entry(), committee,
			Parent->getMessage(source, "LIST/MEMBER")));
	LOG(LM_DEBUG, "COMMSERV/MEMBER_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		comm.member->Entry(), committee));
	comm.erase();
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS2", (
				member, committee,
				Parent->getMessage(source, "LIST/MEMBER")));
    }
}

void CommServ::do_member_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_member_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONDYNAMIC", (
				message));
	return;
    }

    if (Parent->commserv.GetList(committee).Private() &&
	!(Parent->commserv.GetList(committee).IsOn(source) ||
	(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source))))
    {
	SEND(mynick, source, "COMMSERV/NOTMEMBER", (
				committee));
	return;
    }

    SEND(mynick, source, "LIST/DISPLAY2", (
		committee, Parent->getMessage(source, "LIST/MEMBER")));
    CommServ::do_member_List2(mynick, source, committee, true, 1);
}


int CommServ::do_member_List2(const mstring &mynick, const mstring &source,
	const mstring &committee, const bool first, const int number)
{
    FT("CommServ::do_member_List2", (mynick, source, committee, first, number));

    int nextnum = number;
    mstring output;

    if (!Parent->commserv.IsList(committee))
    {
	RET(0);
    }
    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.HeadCom().empty())
    {
	if (Parent->commserv.IsList(comm.HeadCom()))
	{
	    nextnum += CommServ::do_member_List2(mynick, source, comm.HeadCom(), false, nextnum);
	}
    }
    else if (!comm.Head().empty())
    {
	output.erase();
	output << nextnum++ << ". " << IRC_Bold;
	if (Parent->nickserv.IsStored(comm.Head()))
	    output << Parent->getSname(comm.Head());
	else
	    output << Parent->getMessage(source, "COMMSERV/DEFUNCT") <<
	    	" " << Parent->getMessage(source, "COMMSERV_INFO/SET_HEAD");
	output << IRC_Off;
	if (!Parent->nickserv.IsStored(comm.Head()))
		output << " [" << Parent->getMessage(source, "COMMSERV/DEFUNCT") << "]";
	::send(mynick, source, output);
    }

    MLOCK(("CommServ", "list", comm.Name().UpperCase(), "member"));
    for (comm.member = comm.begin(); comm.member != comm.end(); comm.member++)
    {
	output.erase();
	output << nextnum++ << ". " << (first ? Blank : IRC_Bold) <<
		comm.member->Entry() << (first ? Blank : IRC_Off);
	if (!Parent->nickserv.IsStored(comm.member->Entry()))
	    output << " [" << Parent->getMessage(source, "COMMSERV/DEFUNCT") << "]";
	::send(mynick, source, output);
	::send(mynick, source, "    " +
		parseMessage(Parent->getMessage(source, "LIST/LASTMOD"),
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (comm.MSG_size() >= Parent->commserv.Max_Logon())
    {
	SEND(mynick, source, "COMMSERV/MAX_MESSAGES", (
		committee));
	return;
    }

    comm.MSG_insert(msgnum, source);
    Parent->commserv.stats.i_Logon++;
    SEND(mynick, source, "LIST/ADD2_NUMBER", (
		comm.MSG_size(), committee,
		Parent->getMessage(source, "LIST/MESSAGES")));
    }
    LOG(LM_INFO, "COMMSERV/LOGON_ADD", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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

    if (!Parent->commserv.IsList(committee))
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
    if (num <= 0 || num > Parent->commserv.GetList(committee).MSG_size())
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
			1, Parent->commserv.GetList(committee).MSG_size()));
    }

    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    MLOCK(("CommServ", "list", committee.UpperCase(), "message"));
    if (comm.MSG_find(num))
    {
	Parent->commserv.stats.i_Logon++;
	SEND(mynick, source, "LIST/DEL2_NUMBER", (
		num, committee,
		Parent->getMessage(source, "LIST/MESSAGES")));
	LOG(LM_INFO, "COMMSERV/LOGON_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		committee));
	comm.MSG_erase();
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS2_NUMBER", (
			num, committee,
			Parent->getMessage(source, "LIST/MESSAGES")));
    }
}


void CommServ::do_logon_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("CommServ::do_logon_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.IsHead(source) &&
	!(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source)))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (!comm.MSG_size())
    {
	SEND(mynick, source, "LIST/EMPTY2", (
		committee, Parent->getMessage(source, "LIST/MESSAGES")));
	return;
    }

    int i;
    mstring output;
    SEND(mynick, source, "LIST/DISPLAY2", (
		committee, Parent->getMessage(source, "LIST/MESSAGES")));
    MLOCK(("CommServ", "list", committee.UpperCase(), "message"));
    for (i=1, comm.message = comm.MSG_begin();
	comm.message != comm.MSG_end(); comm.message++, i++)
    {
	output.erase();
	output << i << ". " << comm.message->Entry();
	::send(mynick, source, output);
	::send(mynick, source, "    " +
		parseMessage(Parent->getMessage(source, "LIST/LASTMOD"),
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (!Parent->nickserv.IsStored(newhead))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				newhead));
	return;
    }

    newhead = Parent->getSname(newhead);
    if (Parent->nickserv.GetStored(newhead).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
				newhead));
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!(comm.IsHead(source) ||
	(Parent->commserv.IsList(Parent->commserv.OVR_Owner()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_Owner()).IsOn(source))))
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
    Parent->commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO", (
	Parent->getMessage(source, "COMMSERV_INFO/SET_HEAD"),
	committee, newhead));
    LOG(LM_INFO, "COMMSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_HEAD"),
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    comm.Description(desc);
    }
    Parent->commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO", (
	Parent->getMessage(source, "COMMSERV_INFO/SET_DESCRIPTION"),
	committee, desc));
    LOG(LM_INFO, "COMMSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_DESCRIPTION"),
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

    if (!Parent->commserv.IsList(committee))
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
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL"), '@'));
	return;
    }
    else if (email.WordCount("@") != 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL"), '@'));
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    comm.Email(email);
    }
    Parent->commserv.stats.i_Set++;
    if (email.empty())
    {
	SEND(mynick, source, "COMMSERV/UNSET", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL"),
		committee));
	LOG(LM_INFO, "COMMSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL"),
		committee));
    }
    else
    {
	SEND(mynick, source, "COMMSERV/SET_TO", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL"),
		committee, email));
	LOG(LM_INFO, "COMMSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("COMMSERV_INFO/SET_EMAIL"),
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

    if (!Parent->commserv.IsList(committee))
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

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    comm.URL(url);
    }
    Parent->commserv.stats.i_Set++;
    if (url.empty())
    {
	SEND(mynick, source, "COMMSERV/UNSET", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_URL"),
		committee));
	LOG(LM_INFO, "COMMSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage(source, "COMMSERV_INFO/SET_URL"),
		committee));
    }
    else
    {
	SEND(mynick, source, "COMMSERV/SET_TO", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_URL"),
		committee, "http://" + url));
	LOG(LM_INFO, "COMMSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("COMMSERV_INFO/SET_URL"),
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->commserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (comm.L_Secure())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE"),
		committee));
	return;
    }

    comm.Secure(onoff.GetBool());
    }
    Parent->commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE"),
		committee, onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON") :
			Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_INFO, "COMMSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_SECURE"),
	committee, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->commserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (comm.L_Private())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE"),
		committee));
	return;
    }

    comm.Private(onoff.GetBool());
    }
    Parent->commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE"),
		committee, onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON") :
			Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_INFO, "COMMSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_PRIVATE"),
	committee, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->commserv.DEF_OpenMemos())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    if (!comm.IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (
				committee));
	return;
    }

    if (comm.L_OpenMemos())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"),
		committee));
	return;
    }

    comm.OpenMemos(onoff.GetBool());
    }
    Parent->commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"),
		committee, onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON") :
			Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_INFO, "COMMSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_OPENMEMOS"),
	committee, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->commserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Parent->commserv.LCK_Secure())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE"),
		committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    comm.L_Secure(false);
    comm.Secure(onoff.GetBool());
    comm.L_Secure(true);
    }
    Parent->commserv.stats.i_Lock++;
    SEND(mynick, source, "COMMSERV/LOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE"),
		committee, onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON") :
			Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_INFO, "COMMSERV/LOCKED", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_SECURE"),
	committee, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->commserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Parent->commserv.LCK_Private())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE"),
		committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    comm.L_Private(false);
    comm.Private(onoff.GetBool());
    comm.L_Private(true);
    }
    Parent->commserv.stats.i_Lock++;
    SEND(mynick, source, "COMMSERV/LOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE"),
		committee, onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON") :
			Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_INFO, "COMMSERV/LOCKED", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_PRIVATE"),
	committee, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->commserv.DEF_OpenMemos())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Parent->commserv.LCK_OpenMemos())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"),
		committee));
	return;
    }

    { RLOCK(("CommServ", "list", committee.UpperCase()));
    Committee_t &comm = Parent->commserv.GetList(committee);

    comm.L_OpenMemos(false);
    comm.OpenMemos(onoff.GetBool());
    comm.L_OpenMemos(true);
    }
    Parent->commserv.stats.i_Lock++;
    SEND(mynick, source, "COMMSERV/LOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"),
		committee, onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON") :
			Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_INFO, "COMMSERV/LOCKED", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_OPENMEMOS"),
	committee, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Parent->commserv.LCK_Secure())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE"),
		committee));
	return;
    }

    Parent->commserv.GetList(committee).L_Secure(false);
    Parent->commserv.stats.i_Unlock++;
    SEND(mynick, source, "COMMSERV/UNLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE")));
    LOG(LM_INFO, "COMMSERV/UNLOCKED", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_SECURE"),
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Parent->commserv.LCK_Private())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE"),
		committee));
	return;
    }

    Parent->commserv.GetList(committee).L_Private(false);
    Parent->commserv.stats.i_Unlock++;
    SEND(mynick, source, "COMMSERV/UNLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE")));
    LOG(LM_INFO, "COMMSERV/UNLOCKED", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_PRIVATE"),
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

    if (!Parent->commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (
				committee));
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (
				committee));
	return;
    }

    if (Parent->commserv.LCK_OpenMemos())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"),
		committee));
	return;
    }

    Parent->commserv.GetList(committee).L_OpenMemos(false);
    Parent->commserv.stats.i_Unlock++;
    SEND(mynick, source, "COMMSERV/UNLOCKED", (
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS")));
    LOG(LM_INFO, "COMMSERV/UNLOCKED", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("COMMSERV_INFO/SET_OPENMEMOS"),
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
    if (!(i_Name == Parent->commserv.SADMIN_Name() ||
		i_Name == Parent->commserv.ALL_Name() ||
		i_Name == Parent->commserv.REGD_Name()))
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
    FT("Committee_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
	if( pElement->IsA(tag_Name) )		pElement->Retrieve(i_Name);
	if( pElement->IsA(tag_RegTime) )	pElement->Retrieve(i_RegTime);
	if( pElement->IsA(tag_HeadCom) )	pElement->Retrieve(i_HeadCom);
	if( pElement->IsA(tag_Head) )		pElement->Retrieve(i_Head);
	if( pElement->IsA(tag_Description) )	pElement->Retrieve(i_Description);
	if( pElement->IsA(tag_Email) )		pElement->Retrieve(i_Email);
	if( pElement->IsA(tag_URL) )		pElement->Retrieve(i_URL);
	if( pElement->IsA(tag_set_OpenMemos) )	pElement->Retrieve(i_OpenMemos);
	if( pElement->IsA(tag_set_Private) )	pElement->Retrieve(i_Private);
	if( pElement->IsA(tag_set_Secure) )	pElement->Retrieve(i_Secure);
	if( pElement->IsA(tag_lock_OpenMemos) )	pElement->Retrieve(l_OpenMemos);
	if( pElement->IsA(tag_lock_Private) )	pElement->Retrieve(l_Private);
	if( pElement->IsA(tag_lock_Secure) )	pElement->Retrieve(l_Secure);
}

void Committee_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
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
	pOut->WriteElement(tag_set_OpenMemos, i_OpenMemos);
	pOut->WriteElement(tag_set_Private, i_Private);
	pOut->WriteElement(tag_set_Secure, i_Secure);
	pOut->WriteElement(tag_lock_OpenMemos, l_OpenMemos);
	pOut->WriteElement(tag_lock_Private, l_Private);
	pOut->WriteElement(tag_lock_Secure, l_Secure);

	if (!(i_Name == Parent->commserv.ALL_Name() ||
	      i_Name == Parent->commserv.REGD_Name() ||
	      i_Name == Parent->commserv.SADMIN_Name()))
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
    FT("CommServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void CommServ::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
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
	if (iter->first == Parent->commserv.SADMIN_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom.erase();
	    iter->second.i_Members.clear();
	    for (unsigned int j=1; j<=Parent->operserv.Services_Admin().WordCount(", "); j++)
		iter->second.i_Members.insert(entlist_t(
			Parent->operserv.Services_Admin().ExtractWord(j, ", "),
			Parent->operserv.FirstName()));
	    iter->second.Secure(sadmin_secure);
	    iter->second.Private(sadmin_private);
	    iter->second.OpenMemos(sadmin_openmemos);
	}
	else if (iter->first == Parent->commserv.SOP_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom = Parent->commserv.SADMIN_Name();
	    iter->second.Secure(sop_secure);
	    iter->second.Private(sop_private);
	    iter->second.OpenMemos(sop_openmemos);
	}
	else if (iter->first == Parent->commserv.ADMIN_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom = Parent->commserv.SADMIN_Name();
	    iter->second.Secure(admin_secure);
	    iter->second.Private(admin_private);
	    iter->second.OpenMemos(admin_openmemos);
	}
	else if (iter->first == Parent->commserv.OPER_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom = Parent->commserv.ADMIN_Name();
	    iter->second.Secure(oper_secure);
	    iter->second.Private(oper_private);
	    iter->second.OpenMemos(oper_openmemos);
	}
	else if (iter->first == Parent->commserv.ALL_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom = Parent->commserv.ADMIN_Name();
	    iter->second.i_Members.clear();
	    iter->second.Secure(false);
	    iter->second.Private(true);
	    iter->second.OpenMemos(false);
	}
	else if (iter->first == Parent->commserv.REGD_Name())
	{
	    iter->second.i_Head.erase();
	    iter->second.i_HeadCom = Parent->commserv.SOP_Name();
	    iter->second.i_Members.clear();
	    iter->second.Secure(false);
	    iter->second.Private(true);
	    iter->second.OpenMemos(false);
	}
    }
}
