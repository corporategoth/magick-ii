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

#include "lockable.h"
#include "commserv.h"
#include "magick.h"

Committee::Committee(mstring name, mstring head, mstring description)
{
    FT("Committee::Committee", (name, head, description));
    i_Name = name.UpperCase();
    i_RegTime = Now();
    i_Head = head.LowerCase();
    i_HeadCom = "";
    i_Description = description;
    i_OpenMemos = Parent->commserv.DEF_OpenMemos();
    l_OpenMemos = false;
    i_Secure = Parent->commserv.DEF_Secure();
    l_Secure = false;
    i_Private = Parent->commserv.DEF_Private();
    l_Private = false;
}


Committee::Committee(mstring name, Committee *head, mstring description)
{
    FT("Committee::Committee", (name, "(Committee *) head", description));
    i_Name = name.UpperCase();
    i_RegTime = Now();
    i_Head = "";
    i_HeadCom = head->Name();
    i_Description = description;
    i_OpenMemos = Parent->commserv.DEF_OpenMemos();
    l_OpenMemos = false;
    i_Secure = Parent->commserv.DEF_Secure();
    l_Secure = false;
    i_Private = Parent->commserv.DEF_Private();
    l_Private = false;
}


Committee::Committee(mstring name, mstring description)
{
    FT("Committee::Committee", (name, description));
    i_Name = name.UpperCase();
    i_RegTime = Now();
    i_Head = "";
    i_HeadCom = "";
    i_Description = description;
    i_OpenMemos = Parent->commserv.DEF_OpenMemos();
    l_OpenMemos = false;
    i_Secure = Parent->commserv.DEF_Secure();
    l_Secure = false;
    i_Private = Parent->commserv.DEF_Private();
    l_Private = false;
}


void Committee::operator=(const Committee &in)
{
    FT("Committee::operator=", ("(const Committee &) in"));

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

void Committee::Head(mstring newhead)
{
    FT("Committee::Head", (newhead));

    if (i_HeadCom != "")
	i_HeadCom = "";

    i_Head = newhead.LowerCase();
}

bool Committee::insert(mstring entry, mstring nick, mDateTime modtime)
{
    FT("Committee::insert", (entry, nick, modtime));

    entlist_ui iter;
    if (!i_Members.empty())
	for (iter=i_Members.begin(); iter!=i_Members.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;
    if (i_Members.empty() || iter == i_Members.end())
    {
	pair<entlist_ui, bool> tmp;
	tmp = i_Members.insert(entlist_t(entry, nick, modtime));
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

    if (member != i_Members.end())
    {
	i_Members.erase(member);
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


bool Committee::IsIn(mstring nick)
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
    if (Parent->nickserv.stored[target].Host() != "" &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[target].Host()))
	target = Parent->nickserv.stored[target].Host().LowerCase();

    // We're a HEAD, in by DEFAULT
    if (i_HeadCom != "" && Parent->commserv.IsList(i_HeadCom) &&
	Parent->commserv.list[i_HeadCom].IsIn(target))
    {
	RET(true);
    }

    if (i_Head != "" && target == i_Head)
    {
	RET(true);
    }

    entlist_ui iter;
    for (iter=i_Members.begin(); iter!=i_Members.end(); iter++)
    {
	if (target == iter->Entry().LowerCase())
	{
	    RET(true);
	}
    }
    RET(false);
}


bool Committee::IsOn(mstring nick)
{
    FT("Committee::IsOn", (nick));

    if (i_Name == Parent->commserv.ALL_Name() &&
	Parent->nickserv.IsLive(nick))
    {
	RET(true);
    }

    // This returns wether we're CURRENTLY recognized
    // as 'in' the committee (secure, and nick secure
    // taken into account).
    if (IsIn(nick) && Parent->nickserv.IsStored(nick) &&
	Parent->nickserv.stored[nick.LowerCase()].IsOnline())
	if (!i_Secure || (i_Secure &&
	    Parent->nickserv.live[nick.LowerCase()].IsIdentified()))
	{
	    RET(true);
	}
    RET(false);
}


bool Committee::IsHead(mstring nick)
{
    FT("Committee::IsHead", (nick));

    if (i_Head != "" && i_Head == nick.LowerCase())
    {
	RET(true);
    }
    else if (i_HeadCom != "" && Parent->commserv.IsList(i_HeadCom))
    {
	if (Parent->commserv.list[i_HeadCom].IsIn(nick))
	{
	    RET(true);
	}
    }
    else if (i_Head == "" && i_HeadCom == "")
    {
	bool retval = IsIn(nick);
	RET(retval);
    }
    RET(false);
}

void Committee::Secure(bool in)
{
    FT("Committee::Secure", (in));
    if (!(Parent->commserv.LCK_Secure() || l_Secure))
	i_Secure = in;
}


bool Committee::Secure()
{
    NFT("Committee::Secure");
    if (!Parent->commserv.LCK_Secure())
    {
	RET(i_Secure);
    }
    RET(Parent->commserv.DEF_Secure());
}


void Committee::L_Secure(bool in)
{
    FT("Committee::L_Secure", (in));
    if (!Parent->commserv.LCK_Secure())
	l_Secure = in;
}


bool Committee::L_Secure()
{
    NFT("Committee::L_Secure");
    if (!Parent->commserv.LCK_Secure())
    {
	RET(l_Secure);
    }
    RET(true);
}


void Committee::Private(bool in)
{
    FT("Committee::Private", (in));
    if (!(Parent->commserv.LCK_Private() || l_Private))
	i_Private = in;
}


bool Committee::Private()
{
    NFT("Committee::Private");
    if (!Parent->commserv.LCK_Private())
    {
	RET(i_Private);
    }
    RET(Parent->commserv.DEF_Private());
}


void Committee::L_Private(bool in)
{
    FT("Committee::L_Private", (in));
    if (!Parent->commserv.LCK_Private())
	l_Private = in;
}


bool Committee::L_Private()
{
    NFT("Committee::L_Private");
    if (!Parent->commserv.LCK_Private())
    {
	RET(l_Private);
    }
    RET(true);
}


void Committee::OpenMemos(bool in)
{
    FT("Committee::OpenMemos", (in));
    if (!(Parent->commserv.LCK_OpenMemos() || l_OpenMemos))
	i_OpenMemos = in;
}


bool Committee::OpenMemos()
{
    NFT("Committee::OpenMemos");
    if (!Parent->commserv.LCK_OpenMemos())
    {
	RET(i_OpenMemos);
    }
    RET(Parent->commserv.DEF_OpenMemos());
}


void Committee::L_OpenMemos(bool in)
{
    FT("Committee::L_OpenMemos", (in));
    if (!Parent->commserv.LCK_OpenMemos())
	l_OpenMemos = in;
}


bool Committee::L_OpenMemos()
{
    NFT("Committee::L_OpenMemos");
    if (!Parent->commserv.LCK_OpenMemos())
    {
	RET(l_OpenMemos);
    }
    RET(true);
}


bool Committee::MSG_insert(mstring entry, mstring nick)
{
    FT("Committee::MSG_insert", (entry, nick));

    if (IsHead(nick))
    {
	i_Messages.push_back(entlist_t(entry, nick));
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

    if (message != i_Messages.end())
    {
	i_Messages.erase(message);
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

CommServ::CommServ()
{
    NFT("CommServ::CommServ");
    messages = true;
}

bool CommServ::IsList(mstring in)
{
    FT("CommServ::IsList", (in));
    bool retval = (list.find(in.UpperCase())!=list.end());
    RET(retval);
}

void CommServ::AddCommands()
{
    NFT("CommServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

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
		"SET* HEAD*", Parent->commserv.REGD_Name(), CommServ::do_set_Head);
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
		"LOCK SEC*", Parent->commserv.SOP_Name(), CommServ::do_lock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK PRIV*", Parent->commserv.SOP_Name(), CommServ::do_lock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK *MEMO*", Parent->commserv.SOP_Name(), CommServ::do_lock_OpenMemos);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Parent->commserv.SOP_Name(), CommServ::do_unlock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Parent->commserv.SOP_Name(), CommServ::do_unlock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK *MEMO*", Parent->commserv.SOP_Name(), CommServ::do_unlock_OpenMemos);

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
		"SET* HEAD*", Parent->commserv.REGD_Name());
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
		"LOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK *MEMO*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK *MEMO*", Parent->commserv.SOP_Name());

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
    else if (!Parent->commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);

}

void CommServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    mstring HelpTopic = Parent->commserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.Replace(" ", "/");
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

    Parent->commserv.list.erase(committee);
    Parent->commserv.stats.i_Kill++;
    ::send(mynick, source, Parent->getMessage(source, "COMMSERV/KILL"), committee.c_str());
}


void CommServ::do_List(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_List", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();
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
	listsize = ACE_OS::atoi(params.ExtractWord(3, " ").c_str());
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

    for (iter = Parent->commserv.list.begin(), i=0, count = 0;
			iter != Parent->commserv.list.end(); iter++)
    {
	if (iter->second.Name().LowerCase().Matches(mask))
	{
	    if (i < listsize && (!iter->second.Private() ||
		(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))))
	    {
		::send(mynick, source, iter->second.Name() + " (" +
				mstring(itoa(iter->second.size())) + "): " +
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
    if (Parent->nickserv.stored[source.LowerCase()].Host() != "")
	realme = Parent->nickserv.stored[source.LowerCase()].Host();

    if (comm->HeadCom() != "")
    {
	if (Parent->commserv.IsList(comm->HeadCom()))
	{
	    CommServ::do_Memo2(source, comm->HeadCom(), text);
	}
    }
    else if (comm->Head() != "")
    {
	if (Parent->nickserv.IsStored(comm->Head()))
	{
	    mstring realrecipiant = Parent->nickserv.stored[comm->Head().LowerCase()].Host();
	    if (realrecipiant == "")
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

    MLOCK(("CommServ", "list", comm->Name().LowerCase(), "member"));
    for (comm->member = comm->begin(); comm->member != comm->end(); comm->member++)
    {
	if (Parent->nickserv.IsStored(comm->member->Entry()))
	{
	    mstring realrecipiant = Parent->nickserv.stored[comm->member->Entry().LowerCase()].Host();
	    if (realrecipiant == "")
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
    if (comm->HeadCom() != "")
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/HEADCOM"),
			comm->HeadCom().c_str());
    }
    else if (comm->Head() != "")
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/HEAD"),
			comm->Head().c_str());
    }

    if (comm->Email() != "")
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/EMAIL"),
			comm->Email().c_str());
    }

    if (comm->URL() != "")
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV_INFO/URL"),
			comm->URL().c_str());
    }

    output = "";
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

    output = "";
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
    if (Parent->servmsg.ShowSync())
	::send(mynick, source, Parent->getMessage("MISC/SYNC"),
			Parent->events->SyncTime().c_str());
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
    if (Parent->commserv.list[committee].IsIn(member))
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EXISTS2"),
				member.c_str(), committee.c_str(),
				Parent->getMessage(source, "LIST/MEMBER").c_str());
	return;
    }

    Committee *comm = &Parent->commserv.list[committee];
    MLOCK(("CommServ", "list", comm->Name().LowerCase(), "member"));
    comm->insert(member, source);
    Parent->commserv.stats.i_AddDel++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/ADD2"),
				member.c_str(), committee.c_str(),
				Parent->getMessage(source, "LIST/MEMBER").c_str());
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

    if (!Parent->commserv.list[committee].IsIn(member))
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2"),
				member.c_str(), committee.c_str(),
				Parent->getMessage(source, "LIST/MEMBER").c_str());
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
    MLOCK(("CommServ", "list", comm->Name().LowerCase(), "member"));
    if (comm->find(member))
    {
	Parent->commserv.stats.i_AddDel++;
	::send(mynick, source, Parent->getMessage(source, "LIST/DEL2"),
			comm->member->Entry().c_str(), committee.c_str(),
			Parent->getMessage(source, "LIST/MEMBER").c_str());
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

    if (committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOTONDYNAMIC"),
				message.c_str());
	return;
    }

    if (Parent->commserv.list[committee].Private() &&
	!(Parent->commserv.list[committee].IsOn(source) ||
	(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))))
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

    if (comm->HeadCom() != "")
    {
	if (Parent->commserv.IsList(comm->HeadCom()))
	{
	    nextnum += CommServ::do_member_List2(mynick, source, comm->HeadCom(), false, nextnum);
	}
    }
    else if (comm->Head() != "")
    {
	output = "";
	output << nextnum++ << ". " << IRC_Bold << comm->Head() << IRC_Off;
	if (!Parent->nickserv.IsStored(comm->Head()))
		output << " [" << Parent->getMessage(source, "COMMSERV/DEFUNCT") << "]";
	::send(mynick, source, output);
    }

    MLOCK(("CommServ", "list", comm->Name().LowerCase(), "member"));
    for (comm->member = comm->begin(); comm->member != comm->end(); comm->member++)
    {
	output = "";
	output << nextnum++ << ". " << (first ? Blank : IRC_Bold) <<
		comm->member->Entry() << (first ? Blank : IRC_Off);
	if (!Parent->nickserv.IsStored(comm->member->Entry()))
	    output << " [" << Parent->getMessage(source, "COMMSERV/DEFUNCT") << "]";
	::send(mynick, source, output);
	::send(mynick, source, "    " + Parent->getMessage(source, "LIST/LASTMOD"),
		ToHumanTime(comm->member->Last_Modify_Time().SecondsSince()).c_str(),
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

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    Committee *comm = &Parent->commserv.list[committee];
    MLOCK(("CommServ", "list", comm->Name().LowerCase(), "message"));
    Parent->commserv.stats.i_Logon++;
    comm->MSG_insert(msgnum, source);
    ::send(mynick, source, Parent->getMessage(source, "LIST/ADD2_NUMBER"),
		comm->MSG_size(), committee.c_str(),
		Parent->getMessage(source, "LIST/MESSAGES").c_str());
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER"));
	return;
    }
    
    unsigned int num = ACE_OS::atoi(msgnum.c_str());
    if (num <= 0 || num > Parent->commserv.list[committee].MSG_size())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
			1, Parent->commserv.list[committee].MSG_size());
    }

    Committee *comm = &Parent->commserv.list[committee];
    MLOCK(("CommServ", "list", comm->Name().LowerCase(), "message"));
    if (comm->MSG_find(num))
    {
	Parent->commserv.stats.i_Logon++;
	::send(mynick, source, Parent->getMessage(source, "LIST/DEL2_NUMBER"),
		num, committee.c_str(),
		Parent->getMessage(source, "LIST/MESSAGES").c_str());
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
	!(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source)))
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
    MLOCK(("CommServ", "list", comm->Name().LowerCase(), "message"));
    for (i=1, comm->message = comm->MSG_begin();
	comm->message != comm->MSG_end(); comm->message++, i++)
    {
	output = "";
	output << i << ". " << comm->message->Entry();
	::send(mynick, source, output);
	::send(mynick, source, "    " + Parent->getMessage(source, "LIST/LASTMOD"),
		ToHumanTime(comm->message->Last_Modify_Time().SecondsSince()).c_str(),
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
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))))
    {
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/NOTHEAD"),
				committee.c_str());
	return;
    }

    if (Parent->commserv.list[committee].Head() == "")
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

    if (email.CmpNoCase("none") == 0)
	email = "";
    Parent->commserv.list[committee].Email(email);
    Parent->commserv.stats.i_Set++;
    if (email == "")
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/UNSET"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL").c_str(),
		committee.c_str());
    else
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/SET_TO"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_EMAIL").c_str(),
		committee.c_str(), email.c_str());
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

    if (url.CmpNoCase("none") == 0)
	url = "";
    Parent->commserv.list[committee].URL(url);
    Parent->commserv.stats.i_Set++;
    if (url == "")
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/UNSET"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_URL").c_str(),
		committee.c_str());
    else
	::send(mynick, source, Parent->getMessage(source, "COMMSERV/SET_TO"),
		Parent->getMessage(source, "COMMSERV_INFO/SET_URL").c_str(),
		committee.c_str(), url.c_str());
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

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
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
			Parent->getMessage(source, "MISC/ON").c_str() :
			Parent->getMessage(source, "MISC/OFF").c_str());
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

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
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
			Parent->getMessage(source, "MISC/ON").c_str() :
			Parent->getMessage(source, "MISC/OFF").c_str());
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

    if (!(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source)))
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

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
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
			Parent->getMessage(source, "MISC/ON").c_str() :
			Parent->getMessage(source, "MISC/OFF").c_str());
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

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
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
			Parent->getMessage(source, "MISC/ON").c_str() :
			Parent->getMessage(source, "MISC/OFF").c_str());
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

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
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
			Parent->getMessage(source, "MISC/ON").c_str() :
			Parent->getMessage(source, "MISC/OFF").c_str());
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

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
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
			Parent->getMessage(source, "MISC/ON").c_str() :
			Parent->getMessage(source, "MISC/OFF").c_str());
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

    if (i_Name == Parent->commserv.SADMIN_Name())
    {
	for (int j=1; j<=Parent->operserv.Services_Admin().WordCount(", "); j++)
	    i_Members.insert(entlist_t(
		Parent->operserv.Services_Admin().ExtractWord(j, ", "),
		Parent->operserv.FirstName()));
    }
    else if (!(i_Name == Parent->commserv.ALL_Name() ||
		i_Name == Parent->commserv.REGD_Name()))
    {
	if( pElement->IsA(tag_Members) )
	{
	    entlist_t tmp;
	    pIn->ReadTo(&tmp);
	    i_Members.insert(tmp);
	}
    }

    if( pElement->IsA(tag_Messages) )
    {
	entlist_t tmp;
	pIn->ReadTo(&tmp);
	i_Messages.push_back(tmp);
    }


    if( pElement->IsA(tag_UserDef) )
    {
        mstring tmp;
        pElement->Retrieve(tmp);
        i_UserDef[tmp.Before("\n")]=tmp.After("\n");
    }
}

void Committee::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("Committee::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    //TODO: Add your source code here
	pOut->BeginObject(tag_Committee, attribs);

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
    Committee d1;
    if( pElement->IsA( d1.GetClassTag() ) )
    {
	pIn->ReadTo(&d1);
	if (d1.Name() != "")
	    list[d1.Name().UpperCase()] = d1;
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
    for (iter = list.begin(); iter != list.end(); iter++)
	pOut->WriteSubElement(&iter->second, attribs);

    pOut->EndObject(tag_CommServ);
}


void CommServ::PostLoad()
{
    NFT("CommServ::PostLoad");
    // Linkage, etc
}
