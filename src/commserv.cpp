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

Committee::Committee(mstring name, mstring head, mstring description)
{
    FT("Committee::Committee", (name, head, description));

    i_Name = name.UpperCase();
    WLOCK(("CommServ", "list", i_Name.UpperCase()));
    i_RegTime = mDateTime::CurrentDateTime();
    i_Head = head.LowerCase();
    i_Description = description;
    i_OpenMemos = Parent->commserv.DEF_OpenMemos();
    l_OpenMemos = false;
    i_Secure = Parent->commserv.DEF_Secure();
    l_Secure = false;
    i_Private = Parent->commserv.DEF_Private();
    l_Private = false;
    DumpE();
}


Committee::Committee(mstring name, Committee *head, mstring description)
{
    FT("Committee::Committee", (name, "(Committee *) head", description));
    i_Name = name.UpperCase();
    WLOCK(("CommServ", "list", i_Name.UpperCase()));
    i_RegTime = mDateTime::CurrentDateTime();
    i_HeadCom = head->Name();
    i_Description = description;
    i_OpenMemos = Parent->commserv.DEF_OpenMemos();
    l_OpenMemos = false;
    i_Secure = Parent->commserv.DEF_Secure();
    l_Secure = false;
    i_Private = Parent->commserv.DEF_Private();
    l_Private = false;
    DumpE();
}


Committee::Committee(mstring name, mstring description)
{
    FT("Committee::Committee", (name, description));
    i_Name = name.UpperCase();
    WLOCK(("CommServ", "list", i_Name.UpperCase()));
    i_RegTime = mDateTime::CurrentDateTime();
    i_Description = description;
    i_OpenMemos = Parent->commserv.DEF_OpenMemos();
    l_OpenMemos = false;
    i_Secure = Parent->commserv.DEF_Secure();
    l_Secure = false;
    i_Private = Parent->commserv.DEF_Private();
    l_Private = false;
    DumpE();
}


void Committee::operator=(const Committee &in)
{
    FT("Committee::operator=", ("(const Committee &) in"));

    WLOCK(("CommServ", "list", in.i_Name.UpperCase()));
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

mDateTime Committee::RegTime() const
{
    NFT("Committee::RegTime");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_RegTime"));
    RET(i_RegTime);
}

mstring Committee::HeadCom() const
{
    NFT("Committee::HeadCom");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_HeadCom"));
    RET(i_HeadCom);
}

mstring Committee::Head() const
{
    NFT("Committee::Head");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Head"));
    RET(i_Head);
}

void Committee::Head(mstring newhead)
{
    FT("Committee::Head", (newhead));

    WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_HeadCom"));
    WLOCK2(("CommServ", "list", i_Name.UpperCase(), "i_Head"));
    MCB(i_Head);
    if (!i_HeadCom.empty())
    {
	CB(1, i_HeadCom);
	i_HeadCom.erase();
	CE(1, i_HeadCom);
    }

    i_Head = newhead.LowerCase();
    MCE(i_Head);
}

bool Committee::insert(mstring entry, mstring nick, mDateTime modtime)
{
    FT("Committee::insert", (entry, nick, modtime));

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


bool Committee::erase()
{
    NFT("Committee::erase");

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

bool Committee::find(mstring entry)
{
    FT("Committee::find", (entry));

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


bool Committee::IsIn(mstring nick) const
{
    FT("Committee::IsIn", (nick));

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

    mstring target = nick.LowerCase();
    if (!Parent->nickserv.stored[target].Host().empty() &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[target].Host()))
	target = Parent->nickserv.stored[target].Host().LowerCase();

    // We're a HEAD, in by DEFAULT
    { RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_HeadCom"));
    if (!i_HeadCom.empty() && Parent->commserv.IsList(i_HeadCom) &&
	Parent->commserv.list[i_HeadCom].IsIn(target))
    {
	RET(true);
    }}

    { RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Head"));
    if (!i_Head.empty() && target == i_Head)
    {
	RET(true);
    }}

    entlist_cui iter;
    MLOCK(("CommServ", "list", i_Name.UpperCase(), "member"));
    for (iter=i_Members.begin(); iter!=i_Members.end(); iter++)
    {
	if (target == iter->Entry().LowerCase())
	{
	    RET(true);
	}
    }

    // If this committee is SADMIN, we could be a member
    // as a SLAVE since theres no way of online changing host.
    if (i_Name == Parent->commserv.SADMIN_Name())
    {
	unsigned int i;
	for (i=0; i<Parent->nickserv.stored[target.LowerCase()].Siblings(); i++)
	{
	    for (iter=i_Members.begin(); iter!=i_Members.end(); iter++)
	    {
		if (Parent->nickserv.stored[target.LowerCase()].Sibling(i).LowerCase() ==
						iter->Entry().LowerCase())
		{
		    RET(true);
		}
	    }
	}
    }

    RET(false);
}


bool Committee::IsOn(mstring nick) const
{
    FT("Committee::IsOn", (nick));

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
	!Parent->nickserv.live[nick.LowerCase()].HasMode("o"))
    {
	RET(false);
    }

    // This returns wether we're CURRENTLY recognized
    // as 'in' the committee (secure, and nick secure
    // taken into account).
    if (IsIn(nick) && Parent->nickserv.IsStored(nick) &&
	Parent->nickserv.stored[nick.LowerCase()].IsOnline())
	if (!i_Secure ||
	    Parent->nickserv.live[nick.LowerCase()].IsIdentified())
	{
	    RET(true);
	}
    RET(false);
}


bool Committee::IsHead(mstring nick) const
{
    FT("Committee::IsHead", (nick));

    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Head"));
    RLOCK2(("CommServ", "list", i_Name.UpperCase(), "i_HeadCom"));
    if (!i_Head.empty() && i_Head == nick.LowerCase())
    {
	RET(true);
    }
    else if (!i_HeadCom.empty() && Parent->commserv.IsList(i_HeadCom) &&
	Parent->commserv.list[i_HeadCom.UpperCase()].IsIn(nick))
    {
	RET(true);
    }
    else if (i_Head.empty() && i_HeadCom.empty())
    {
	bool retval = IsIn(nick);
	RET(retval);
    }
    RET(false);
}

void Committee::Description(mstring in)
{
    FT("Committee::Description", (in));
    WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Description"));
    MCB(i_Description);
    i_Description = in;
    MCE(i_Description);
}

mstring Committee::Description() const
{
    NFT("Committee::Description");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Description"));
    RET(i_Description);
}

void Committee::Email(mstring in)
{
    FT("Committee::Email", (in));
    WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Email"));
    MCB(i_Email);
    i_Email = in;
    MCE(i_Email);
}

mstring Committee::Email() const
{
    NFT("Committee::Email");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Email"));
    RET(i_Email);
}

void Committee::URL(mstring in)
{
    FT("Committee::URL", (in));
    WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_URL"));
    MCB(i_URL);
    i_URL = in;
    MCE(i_URL);
}

mstring Committee::URL() const
{
    NFT("Committee::URL");
    RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_URL"));
    RET(i_URL);
}

void Committee::Private(bool in)
{
    FT("Committee::Private", (in));
    if (!L_Private())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Private"));
	MCB(i_Private);
	i_Private = in;
	MCE(i_Private);
    }
}

bool Committee::Private() const
{
    NFT("Committee::Private");
    if (!Parent->commserv.LCK_Private())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Private"));
	RET(i_Private);
    }
    RET(Parent->commserv.DEF_Private());
}


void Committee::L_Private(bool in)
{
    FT("Committee::L_Private", (in));
    if (!Parent->commserv.LCK_Private())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "l_Private"));
	MCB(l_Private);
	l_Private = in;
	MCE(l_Private);
    }
}


bool Committee::L_Private() const
{
    NFT("Committee::L_Private");
    if (!Parent->commserv.LCK_Private())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "l_Private"));
	RET(l_Private);
    }
    RET(true);
}


void Committee::Secure(bool in)
{
    FT("Committee::Secure", (in));
    if (!L_Secure())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Secure"));
	MCB(i_Secure);
	i_Secure = in;
	MCE(i_Secure);
    }
}


bool Committee::Secure() const
{
    NFT("Committee::Secure");
    if (!Parent->commserv.LCK_Secure())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_Secure"));
	RET(i_Secure);
    }
    RET(Parent->commserv.DEF_Secure());
}


void Committee::L_Secure(bool in)
{
    FT("Committee::L_Secure", (in));
    if (!Parent->commserv.LCK_Secure())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "l_Secure"));
	MCB(l_Secure);
	l_Secure = in;
	MCE(l_Secure);
    }
}


bool Committee::L_Secure() const
{
    NFT("Committee::L_Secure");
    if (!Parent->commserv.LCK_Secure())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "l_Secure"));
	RET(l_Secure);
    }
    RET(true);
}


void Committee::OpenMemos(bool in)
{
    FT("Committee::OpenMemos", (in));
    if (!L_OpenMemos())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "i_OpenMemos"));
	MCB(i_OpenMemos);
	i_OpenMemos = in;
	MCE(i_OpenMemos);
    }
}


bool Committee::OpenMemos() const
{
    NFT("Committee::OpenMemos");
    if (!Parent->commserv.LCK_OpenMemos())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "i_OpenMemos"));
	RET(i_OpenMemos);
    }
    RET(Parent->commserv.DEF_OpenMemos());
}


void Committee::L_OpenMemos(bool in)
{
    FT("Committee::L_OpenMemos", (in));
    if (!Parent->commserv.LCK_OpenMemos())
    {
	WLOCK(("CommServ", "list", i_Name.UpperCase(), "l_OpenMemos"));
	MCB(l_OpenMemos);
	l_OpenMemos = in;
	MCE(l_OpenMemos);
    }
}


bool Committee::L_OpenMemos() const
{
    NFT("Committee::L_OpenMemos");
    if (!Parent->commserv.LCK_OpenMemos())
    {
	RLOCK(("CommServ", "list", i_Name.UpperCase(), "l_OpenMemos"));
	RET(l_OpenMemos);
    }
    RET(true);
}


bool Committee::MSG_insert(mstring entry, mstring nick, mDateTime time)
{
    FT("Committee::MSG_insert", (entry, nick, time));

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


bool Committee::MSG_erase()
{
    NFT("Committee::MSG_erase");

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


bool Committee::MSG_find(int number)
{
    FT("Committee::MSG_find", (number));

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

size_t Committee::Usage() const
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

void Committee::DumpB() const
{
    MB(0, (i_Name, i_RegTime, i_HeadCom, i_Head, i_Description, i_Email,
	i_URL, i_Members.size(), i_Private, l_Private, i_OpenMemos,
	l_OpenMemos, i_Secure, l_Secure, i_Messages.size(), i_UserDef.size()));
}

void Committee::DumpE() const
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

bool CommServ::IsList(mstring in)const
{
    FT("CommServ::IsList", (in));
    RLOCK(("CommServ", "list", in.LowerCase()));
    bool retval = (list.find(in.UpperCase())!=list.end());
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


void CommServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_OtherServ);
    FT("CommServ::execute", (data));
    //okay this is the main nickserv command switcher


    // Nick/Server PRIVMSG/NOTICE mynick :message

    mstring source, msgtype, mynick, message, command;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ").UpperCase();
    mynick  = Parent->getLname(data.ExtractWord(3, ": "));
    message = data.After(":", 2);
    command = message.Before(" ");

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

void CommServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
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

void CommServ::do_Add(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Add", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring head      = params.ExtractWord(3, " ");
    mstring desc      = params.After(" ", 3);

    if (Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISSTORED"),
				committee.c_str());
	return;
    }

    if (!Parent->nickserv.IsStored(head))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				head.c_str());
	return;
    }

    head = Parent->getSname(head);
    if (Parent->nickserv.stored[head.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
				head.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    Parent->commserv.list[committee] = Committee(committee, head, desc);
    Parent->commserv.stats.i_New++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/NEW"),
				committee.c_str(), head.c_str());
    LOG((LM_NOTICE, Parent->getLogMessage("COMMSERV/ADD"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	committee.c_str(), head.c_str()));
}


void CommServ::do_Del(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Del", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    { WLOCK(("CommServ", "list"));
    Parent->commserv.list.erase(committee);
    }
    Parent->commserv.stats.i_Kill++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/KILL"), committee.c_str());
    LOG((LM_NOTICE, Parent->getLogMessage("COMMSERV/DEL"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	committee.c_str()));
}


void CommServ::do_List(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_List", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();

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
	    ::send(mynick, source, Parent->getMessage(source, "LIST/MAXLIST"),
					Parent->config.Maxlist());
	    return;
	}
    }

    ::send(mynick, source, Parent->getMessage(source, "LIST/COMM_LIST"),
					mask.c_str());
    map<mstring, Committee>::iterator iter;

    RLOCK(("CommServ", "list"));
    for (iter = Parent->commserv.list.begin(), i=0, count = 0;
			iter != Parent->commserv.list.end(); iter++)
    {
	if (iter->second.Name().Matches(mask, true))
	{
	    if (i < listsize && (!iter->second.Private() ||
		(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
		Parent->commserv.list[Parent->commserv.OVR_View()].IsOn(source))))
	    {
		::send(mynick, source, iter->second.Name() + " (" +
				iter->second.size() + "): " +
				iter->second.Description());
		i++;
	    }
	    count++;
	}
    }
    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAYED"),
							i, count);
}

void CommServ::do_Memo(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Memo", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring text      = params.After(" ", 2);

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOTONDYNAMIC"),
				message.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsOn(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMEMBER"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].OpenMemos() &&
	!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    text.Prepend("[" + IRC_Bold + committee + IRC_Off + "] ");
    CommServ::do_Memo2(source, committee, text);
    Parent->commserv.stats.i_Memo++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/MEMO"),
				committee.c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/MEMO"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	committee.c_str()));
}


void CommServ::do_Memo2(mstring source, mstring committee, mstring text)
{
    FT("CommServ::do_Memo2", (source, committee, text));

    Committee *comm;
    if (Parent->commserv.IsList(committee))
	comm = &Parent->commserv.list[committee.UpperCase()];
    else
	return;

    Nick_Stored_t *nick;
    mstring realme;
    if (Parent->nickserv.IsStored(source))
	realme = source;
    else
	return;
    if (!Parent->nickserv.stored[source.LowerCase()].Host().empty())
	realme = Parent->nickserv.stored[source.LowerCase()].Host();

    if (!comm->HeadCom().empty())
    {
	if (Parent->commserv.IsList(comm->HeadCom()))
	{
	    CommServ::do_Memo2(source, comm->HeadCom(), text);
	}
    }
    else if (!comm->Head().empty())
    {
	if (Parent->nickserv.IsStored(comm->Head()))
	{
	    mstring realrecipiant = Parent->nickserv.stored[comm->Head().LowerCase()].Host();
	    if (realrecipiant.empty())
		realrecipiant = comm->Head();
	    if (realme.LowerCase() != realrecipiant.LowerCase())
	    {
		Parent->memoserv.nick[realrecipiant.LowerCase()].push_back(
		    Memo_t(realrecipiant, source, text));

		nick = &Parent->nickserv.stored[realrecipiant.LowerCase()];
		if (nick->IsOnline())
		    ::send(Parent->memoserv.FirstName(), realrecipiant,
			Parent->getMessage(realrecipiant, "MS_COMMAND/NS_NEW"),
			Parent->memoserv.nick[realrecipiant.LowerCase()].size(),
			Parent->memoserv.FirstName().c_str(),
			Parent->memoserv.nick[realrecipiant.LowerCase()].size());
		unsigned int i;
		for (i=0; i < nick->Siblings(); i++)
		{
		    if (Parent->nickserv.IsStored(nick->Sibling(i)) &&
				Parent->nickserv.stored[nick->Sibling(i).LowerCase()].IsOnline())
		    {
			::send(Parent->memoserv.FirstName(), nick->Sibling(i),
				Parent->getMessage(nick->Sibling(i), "MS_COMMAND/NS_NEW"),
				Parent->memoserv.nick[realrecipiant.LowerCase()].size(),
				Parent->memoserv.FirstName().c_str(),
				Parent->memoserv.nick[realrecipiant.LowerCase()].size());
		    }
		}
	    }
	}
    }

    MLOCK(("CommServ", "list", comm->Name().UpperCase(), "member"));
    for (comm->member = comm->begin(); comm->member != comm->end(); comm->member++)
    {
	if (Parent->nickserv.IsStored(comm->member->Entry()))
	{
	    mstring realrecipiant = Parent->nickserv.stored[comm->member->Entry().LowerCase()].Host();
	    if (realrecipiant.empty())
		realrecipiant = comm->member->Entry();
	    if (realme.LowerCase() != realrecipiant.LowerCase())
	    {
		Parent->memoserv.nick[realrecipiant.LowerCase()].push_back(
		    Memo_t(realrecipiant, source, text));

		nick = &Parent->nickserv.stored[realrecipiant.LowerCase()];
		if (nick->IsOnline())
		    ::send(Parent->memoserv.FirstName(), realrecipiant,
			Parent->getMessage(realrecipiant, "MS_COMMAND/NS_NEW"),
			Parent->memoserv.nick[realrecipiant.LowerCase()].size(),
			Parent->memoserv.FirstName().c_str(),
			Parent->memoserv.nick[realrecipiant.LowerCase()].size());
		unsigned int i;
		for (i=0; i < nick->Siblings(); i++)
		{
		    if (Parent->nickserv.IsStored(nick->Sibling(i)) &&
				Parent->nickserv.stored[nick->Sibling(i).LowerCase()].IsOnline())
		    {
			::send(Parent->memoserv.FirstName(), nick->Sibling(i),
				Parent->getMessage(nick->Sibling(i), "MS_COMMAND/NS_NEW"),
				Parent->memoserv.nick[realrecipiant.LowerCase()].size(),
				Parent->memoserv.FirstName().c_str(),
				Parent->memoserv.nick[realrecipiant.LowerCase()].size());
		    }
		}
	    }
	}
    }
}

void CommServ::do_Info(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Info", (mynick, source, params));

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

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOTONDYNAMIC"),
				message.c_str());
	return;
    }

    mstring output;
    Committee *comm = &Parent->commserv.list[committee];
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/DESCRIPTION"),
		committee.c_str(), comm->Description().c_str());
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/REGISTERED"),
    			comm->RegTime().Ago().c_str());
    if (!comm->HeadCom().empty())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/HEADCOM"),
			comm->HeadCom().UpperCase().c_str());
    }
    else if (!comm->Head().empty())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/HEAD"),
			Parent->getSname(comm->Head()).c_str());
    }

    if (!comm->Email().empty())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/EMAIL"),
			comm->Email().c_str());
    }

    if (!comm->URL().empty())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/URL"),
			comm->URL().c_str());
    }

    output.erase();
    if (comm->size())
    {
	if (output.size())
	    output << ", ";
	output << comm->size() << " " << Parent->getMessage(source, "COMMSERV_INFO/MEMBERS");
    }
    if (comm->MSG_size())
    {
	if (output.size())
	    output << ", ";
	output << comm->MSG_size() << " " << Parent->getMessage(source, "COMMSERV_INFO/MESSAGES");
    }
    if (output.size())
	::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/STATS"),
			output.c_str());

    output.erase();
    if (comm->Secure())
    {
	if (output.size())
	    output << ", ";
	if (comm->L_Secure())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE");
	if (comm->L_Secure())
	    output << IRC_Off;
    }
    if (comm->Private())
    {
	if (output.size())
	    output << ", ";
	if (comm->L_Private())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE");
	if (comm->L_Private())
	    output << IRC_Off;
    }
    if (comm->OpenMemos())
    {
	if (output.size())
	    output << ", ";
	if (comm->L_OpenMemos())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS");
	if (comm->L_OpenMemos())
	    output << IRC_Off;
    }
    if (output.size())
	::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/OPTIONS"),
			output.c_str());
    { RLOCK(("Events"));
    if (Parent->servmsg.ShowSync() && Parent->events != NULL)
	::send(mynick, source, Parent->getMessage("MISC/SYNC"),
			Parent->events->SyncTime(source).c_str());
    }
}

void CommServ::do_member_Add(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_member_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring member    = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (!Parent->nickserv.IsStored(member))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				member.c_str());
	return;
    }

    member = Parent->getSname(member);
    if (Parent->nickserv.stored[member.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
				member.c_str());
	return;
    }

    if (Parent->commserv.list[committee].IsIn(member))
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EXISTS2"),
				member.c_str(), committee.c_str(),
				Parent->getMessage(source, "LIST/MEMBER").c_str());
	return;
    }

    Committee *comm = &Parent->commserv.list[committee];
    MLOCK(("CommServ", "list", comm->Name().UpperCase(), "member"));
    comm->insert(member, source);
    Parent->commserv.stats.i_AddDel++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/ADD2"),
				member.c_str(), committee.c_str(),
				Parent->getMessage(source, "LIST/MEMBER").c_str());
    LOG((LM_DEBUG, Parent->getLogMessage("COMMSERV/MEMBER_ADD"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	member.c_str(), committee.c_str()));
}


void CommServ::do_member_Del(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_member_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring member    = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
					committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
					committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
					committee.c_str());
	return;
    }

    if (Parent->commserv.list[committee].IsHead(member))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/OTH_HEAD"),
				member.c_str(), 
				Parent->getMessage(source, "LIST/MEMBER").c_str());
	return;
    }

    Committee *comm = &Parent->commserv.list[committee];
    MLOCK(("CommServ", "list", comm->Name().UpperCase(), "member"));
    if (comm->find(member))
    {
	Parent->commserv.stats.i_AddDel++;
	::send(mynick, source, Parent->getMessage(source, "LIST/DEL2"),
			comm->member->Entry().c_str(), committee.c_str(),
			Parent->getMessage(source, "LIST/MEMBER").c_str());
	LOG((LM_DEBUG, Parent->getLogMessage("COMMSERV/MEMBER_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		comm->member->Entry().c_str(), committee.c_str()));
	comm->erase();
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2"),
				member.c_str(), committee.c_str(),
				Parent->getMessage(source, "LIST/MEMBER").c_str());
    }
}

void CommServ::do_member_List(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_member_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOTONDYNAMIC"),
				message.c_str());
	return;
    }

    if (Parent->commserv.list[committee].Private() &&
	!(Parent->commserv.list[committee].IsOn(source) ||
	(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.list[Parent->commserv.OVR_View()].IsOn(source))))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMEMBER"),
				committee.c_str());
	return;
    }

    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY2"),
		committee.c_str(), Parent->getMessage(source, "LIST/MEMBER").c_str());
    CommServ::do_member_List2(mynick, source, committee, true, 1);
}


int CommServ::do_member_List2(mstring mynick, mstring source, mstring committee, bool first, int number)
{
    FT("CommServ::do_member_List2", (mynick, source, committee, first, number));

    int nextnum = number;
    mstring output;
    Committee *comm;

    if (Parent->commserv.IsList(committee))
    {
	comm = &Parent->commserv.list[committee.UpperCase()];
    }
    else
    {
	RET(0);
    }

    if (!comm->HeadCom().empty())
    {
	if (Parent->commserv.IsList(comm->HeadCom()))
	{
	    nextnum += CommServ::do_member_List2(mynick, source, comm->HeadCom(), false, nextnum);
	}
    }
    else if (!comm->Head().empty())
    {
	output.erase();
	output << nextnum++ << ". " << IRC_Bold;
	if (Parent->nickserv.IsStored(comm->Head()))
	    output << Parent->getSname(comm->Head());
	else
	    output << Parent->getMessage(source, "COMMSERV/DEFUNCT") <<
	    	" " << Parent->getMessage(source, "COMMSERV_INFO/SET_HEAD");
	output << IRC_Off;
	if (!Parent->nickserv.IsStored(comm->Head()))
		output << " [" << Parent->getMessage(source, "COMMSERV/DEFUNCT") << "]";
	::send(mynick, source, output);
    }

    MLOCK(("CommServ", "list", comm->Name().UpperCase(), "member"));
    for (comm->member = comm->begin(); comm->member != comm->end(); comm->member++)
    {
	output.erase();
	output << nextnum++ << ". " << (first ? Blank : IRC_Bold) <<
		comm->member->Entry() << (first ? Blank : IRC_Off);
	if (!Parent->nickserv.IsStored(comm->member->Entry()))
	    output << " [" << Parent->getMessage(source, "COMMSERV/DEFUNCT") << "]";
	::send(mynick, source, output);
	::send(mynick, source, "    " + Parent->getMessage(source, "LIST/LASTMOD"),
		ToHumanTime(comm->member->Last_Modify_Time().SecondsSince(), source).c_str(),
		comm->member->Last_Modifier().c_str());
    }
    RET(nextnum-number);
}

void CommServ::do_logon_Add(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_logon_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring msgnum    = params.After(" ", 3);

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }
    Committee *comm = &Parent->commserv.list[committee];

    if (!comm->IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (comm->MSG_size() >= Parent->commserv.Max_Logon())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/MAX_MESSAGES"),
		committee.c_str());
	return;
    }

    MLOCK(("CommServ", "list", comm->Name().UpperCase(), "message"));
    Parent->commserv.stats.i_Logon++;
    comm->MSG_insert(msgnum, source);
    ::send(mynick, source, Parent->getMessage(source, "LIST/ADD2_NUMBER"),
		comm->MSG_size(), committee.c_str(),
		Parent->getMessage(source, "LIST/MESSAGES").c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/LOGON_ADD"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	committee.c_str()));
}


void CommServ::do_logon_Del(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_logon_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring msgnum    = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (!msgnum.IsNumber() || msgnum.Contains(".") || msgnum.Contains("-"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/WHOLENUMBER"));
	return;
    }
    
    unsigned int num = atoi(msgnum.c_str());
    if (num <= 0 || num > Parent->commserv.list[committee].MSG_size())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
			1, Parent->commserv.list[committee].MSG_size());
    }

    Committee *comm = &Parent->commserv.list[committee];
    MLOCK(("CommServ", "list", comm->Name().UpperCase(), "message"));
    if (comm->MSG_find(num))
    {
	Parent->commserv.stats.i_Logon++;
	::send(mynick, source, Parent->getMessage(source, "LIST/DEL2_NUMBER"),
		num, committee.c_str(),
		Parent->getMessage(source, "LIST/MESSAGES").c_str());
	LOG((LM_INFO, Parent->getLogMessage("COMMSERV/LOGON_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		committee.c_str()));
	comm->MSG_erase();
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2_NUMBER"),
			num, committee.c_str(),
			Parent->getMessage(source, "LIST/MESSAGES").c_str());
    }
}


void CommServ::do_logon_List(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_logon_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source) &&
	!(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.list[Parent->commserv.OVR_View()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].MSG_size())
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY2"),
		committee.c_str(), Parent->getMessage(source, "LIST/MESSAGES").c_str());
	return;
    }

    int i;
    mstring output;
    Committee *comm = &Parent->commserv.list[committee];
    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY2"),
		committee.c_str(), Parent->getMessage(source, "LIST/MESSAGES").c_str());
    MLOCK(("CommServ", "list", comm->Name().UpperCase(), "message"));
    for (i=1, comm->message = comm->MSG_begin();
	comm->message != comm->MSG_end(); comm->message++, i++)
    {
	output.erase();
	output << i << ". " << comm->message->Entry();
	::send(mynick, source, output);
	::send(mynick, source, "    " + Parent->getMessage(source, "LIST/LASTMOD"),
		ToHumanTime(comm->message->Last_Modify_Time().SecondsSince(), source).c_str(),
		comm->message->Last_Modifier().c_str());
    }
}


void CommServ::do_set_Head(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_Head", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring newhead   = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (!(Parent->commserv.list[committee].IsHead(source) ||
	(Parent->commserv.IsList(Parent->commserv.OVR_Owner()) &&
	 Parent->commserv.list[Parent->commserv.OVR_Owner()].IsOn(source))))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.list[committee].Head().empty())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/MULTI_HEAD"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (!Parent->nickserv.IsStored(newhead))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				newhead.c_str());
	return;
    }

    newhead = Parent->getSname(newhead);
    if (Parent->nickserv.stored[newhead.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
				newhead.c_str());
	return;
    }

    if (newhead.LowerCase() == Parent->commserv.list[committee].Head().LowerCase())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOTONYOURSELF"),
				message.c_str());
	return;
    }

    Parent->commserv.list[committee].Head(newhead);
    Parent->commserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/SET_TO"),
	Parent->getMessage(source, "COMMSERV_INFO/SET_HEAD").c_str(),
	committee.c_str(), newhead.c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_HEAD").c_str(),
	committee.c_str(), newhead.c_str()));
}


void CommServ::do_set_Description(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_Description", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring desc      = params.After(" ", 3);

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    Parent->commserv.list[committee].Description(desc);
    Parent->commserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/SET_TO"),
	Parent->getMessage(source, "COMMSERV_INFO/SET_DESCRIPTION").c_str(),
	committee.c_str(), desc.c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_DESCRIPTION").c_str(),
	committee.c_str(), desc.c_str()));
}


void CommServ::do_set_Email(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_Email", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring email     = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (email.IsSameAs("none", true))
	email.erase();
    else if (!email.Contains("@"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAIN"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL").c_str(), '@');
	return;
    }
    else if (email.WordCount("@") != 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAINONE"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL").c_str(), '@');
	return;
    }

    Parent->commserv.list[committee].Email(email);
    Parent->commserv.stats.i_Set++;
    if (email.empty())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/UNSET"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL").c_str(),
		committee.c_str());
	LOG((LM_INFO, Parent->getLogMessage("COMMSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL").c_str(),
		committee.c_str()));
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/SET_TO"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL").c_str(),
		committee.c_str(), email.c_str());
	LOG((LM_INFO, Parent->getLogMessage("COMMSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("COMMSERV_INFO/SET_EMAIL").c_str(),
		committee.c_str(), email.c_str()));
    }
}


void CommServ::do_set_URL(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_URL", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring url       = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (url.IsSameAs("none", true))
	url.erase();

    if (url.SubString(0, 6).IsSameAs("http://", true))
    {
	url.erase(0, 6);
    }

    Parent->commserv.list[committee].URL(url);
    Parent->commserv.stats.i_Set++;
    if (url.empty())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/UNSET"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_URL").c_str(),
		committee.c_str());
	LOG((LM_INFO, Parent->getLogMessage("COMMSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage(source, "COMMSERV_INFO/SET_URL").c_str(),
		committee.c_str()));
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/SET_TO"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_URL").c_str(),
		committee.c_str(), ("http://" + url).c_str());
	LOG((LM_INFO, Parent->getLogMessage("COMMSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("COMMSERV_INFO/SET_URL").c_str(),
		committee.c_str(), ("http://" + url).c_str()));
    }
}


void CommServ::do_set_Secure(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.list[committee].L_Secure())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE").c_str(),
		committee.c_str());
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->commserv.list[committee].Secure(onoff.GetBool());
    Parent->commserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/SET_TO"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE").c_str(),
		committee.c_str(), onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON").c_str() :
			Parent->getMessage(source, "VALS/OFF").c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_SECURE").c_str(),
	committee.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str()));
}


void CommServ::do_set_Private(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.list[committee].L_Private())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE").c_str(),
		committee.c_str());
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->commserv.list[committee].Private(onoff.GetBool());
    Parent->commserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/SET_TO"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE").c_str(),
		committee.c_str(), onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON").c_str() :
			Parent->getMessage(source, "VALS/OFF").c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_PRIVATE").c_str(),
	committee.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str()));
}


void CommServ::do_set_OpenMemos(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_OpenMemos", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.list[committee].L_OpenMemos())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS").c_str(),
		committee.c_str());
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->commserv.list[committee].OpenMemos(onoff.GetBool());
    Parent->commserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/SET_TO"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS").c_str(),
		committee.c_str(), onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON").c_str() :
			Parent->getMessage(source, "VALS/OFF").c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_OPENMEMOS").c_str(),
	committee.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str()));
}


void CommServ::do_lock_Secure(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_lock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.LCK_Secure())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE").c_str(),
		committee.c_str());
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->commserv.list[committee].L_Secure(false);
    Parent->commserv.list[committee].Secure(onoff.GetBool());
    Parent->commserv.list[committee].L_Secure(true);
    Parent->commserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/LOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE").c_str(),
		committee.c_str(), onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON").c_str() :
			Parent->getMessage(source, "VALS/OFF").c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/LOCKED"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_SECURE").c_str(),
	committee.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str()));
}


void CommServ::do_lock_Private(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_lock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.LCK_Private())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE").c_str(),
		committee.c_str());
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->commserv.list[committee].L_Private(false);
    Parent->commserv.list[committee].Private(onoff.GetBool());
    Parent->commserv.list[committee].L_Private(true);
    Parent->commserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/LOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE").c_str(),
		committee.c_str(), onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON").c_str() :
			Parent->getMessage(source, "VALS/OFF").c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/LOCKED"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_PRIVATE").c_str(),
	committee.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str()));
}


void CommServ::do_lock_OpenMemos(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_lock_OpenMemos", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.LCK_OpenMemos())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS").c_str(),
		committee.c_str());
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->commserv.list[committee].L_OpenMemos(false);
    Parent->commserv.list[committee].OpenMemos(onoff.GetBool());
    Parent->commserv.list[committee].L_OpenMemos(true);
    Parent->commserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/LOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS").c_str(),
		committee.c_str(), onoff.GetBool() ?
			Parent->getMessage(source, "VALS/ON").c_str() :
			Parent->getMessage(source, "VALS/OFF").c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/LOCKED"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_OPENMEMOS").c_str(),
	committee.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str()));
}


void CommServ::do_unlock_Secure(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_unlock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.LCK_Secure())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE").c_str(),
		committee.c_str());
	return;
    }

    Parent->commserv.list[committee].L_Secure(false);
    Parent->commserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/UNLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_SECURE").c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/UNLOCKED"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_SECURE").c_str(),
	committee.c_str()));
}


void CommServ::do_unlock_Private(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_unlock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.LCK_Private())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE").c_str(),
		committee.c_str());
	return;
    }

    Parent->commserv.list[committee].L_Private(false);
    Parent->commserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/UNLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_PRIVATE").c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/UNLOCKED"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_PRIVATE").c_str(),
	committee.c_str()));
}


void CommServ::do_unlock_OpenMemos(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_unlock_OpenMemos", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISNOTSTORED"),
				committee.c_str());
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTMODIFY"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.LCK_OpenMemos())
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/ISLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS").c_str(),
		committee.c_str());
	return;
    }

    Parent->commserv.list[committee].L_OpenMemos(false);
    Parent->commserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/UNLOCKED"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS").c_str());
    LOG((LM_INFO, Parent->getLogMessage("COMMSERV/UNLOCKED"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("COMMSERV_INFO/SET_OPENMEMOS").c_str(),
	committee.c_str()));
}


SXP::Tag Committee::tag_Committee("Committee");
SXP::Tag Committee::tag_Name("Name");
SXP::Tag Committee::tag_RegTime("RegTime");
SXP::Tag Committee::tag_HeadCom("HeadCom");
SXP::Tag Committee::tag_Head("Head");
SXP::Tag Committee::tag_Description("Description");
SXP::Tag Committee::tag_Email("EMail");
SXP::Tag Committee::tag_URL("URL");
SXP::Tag Committee::tag_set_OpenMemos("SET_OpenMemos");
SXP::Tag Committee::tag_set_Private("SET_Private");
SXP::Tag Committee::tag_set_Secure("SET_Secure");
SXP::Tag Committee::tag_lock_OpenMemos("LOCK_OpenMemos");
SXP::Tag Committee::tag_lock_Private("LOCK_Private");
SXP::Tag Committee::tag_lock_Secure("LOCK_Secure");
SXP::Tag Committee::tag_Members("Members");
SXP::Tag Committee::tag_Messages("Messages");
SXP::Tag Committee::tag_UserDef("UserDef");

void Committee::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Committee::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
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

void Committee::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Committee::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
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

void Committee::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("Committee::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    //TODO: Add your source code here
	pOut->BeginObject(tag_Committee, attribs);

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
		pOut->BeginObject(tag_Members, attribs);
		pOut->WriteSubElement((entlist_t *) &(*l), attribs);
		pOut->EndObject(tag_Members);
	    }
	}

	entlist_i k;
	for(k=i_Messages.begin(); k!=i_Messages.end(); k++)
	{
	    pOut->BeginObject(tag_Messages, attribs);
	    pOut->WriteSubElement(&(*k), attribs);
	    pOut->EndObject(tag_Messages);
	}

        map<mstring,mstring>::const_iterator iter;
        for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
        {
            pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
        }

	pOut->EndObject(tag_Committee);
}

SXP::Tag CommServ::tag_CommServ("CommServ");

void CommServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("CommServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Committee *c = new Committee;

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
    pOut->BeginObject(tag_CommServ, attribs);

    map<mstring, Committee>::iterator iter;
    { RLOCK(("CommServ", "stored"));
    for (iter = list.begin(); iter != list.end(); iter++)
	pOut->WriteSubElement(&iter->second, attribs);
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
		list[c_array[i]->Name().UpperCase()] = *c_array[i];
	    delete c_array[i];
	}
    }
    c_array.clear();

    map<mstring,Committee>::iterator iter;
    for (iter=list.begin(); iter!=list.end(); iter++)
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
    }

    if (IsList(Parent->commserv.SADMIN_Name()))
    {
	list[Parent->commserv.SADMIN_Name()].i_Members.clear();
	for (unsigned int j=1; j<=Parent->operserv.Services_Admin().WordCount(", "); j++)
	    list[Parent->commserv.SADMIN_Name()].i_Members.insert(entlist_t(
		Parent->operserv.Services_Admin().ExtractWord(j, ", "),
		Parent->operserv.FirstName()));
    }
}
