#include "pch.h"
#ifdef _MSC_VER
#pragma hdrstop
#endif
// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================

#include "lockable.h"
#include "commserv.h"
#include "magick.h"

Committee::Committee(mstring name, mstring head, mstring description)
{
    FT("Committee::Committee", (name, head, description));
    i_Name = name.UpperCase();
    i_Head = head.LowerCase();
    i_HeadCom = "";
    i_Description = description;
    i_OpenMemos = Parent->commserv.DEF_OpenMemos();
    i_Secure = Parent->commserv.DEF_Secure();
    i_Private = Parent->commserv.DEF_Private();
}


Committee::Committee(mstring name, Committee *head, mstring description)
{
    FT("Committee::Committee", (name, "(Committee *) head", description));
    i_Name = name.UpperCase();
    i_Head = "";
    i_HeadCom = head->Name();
    i_Description = description;
    i_OpenMemos = Parent->commserv.DEF_OpenMemos();
    i_Secure = Parent->commserv.DEF_Secure();
    i_Private = Parent->commserv.DEF_Private();
}


Committee::Committee(mstring name, mstring description)
{
    FT("Committee::Committee", (name, description));
    i_Name = name.UpperCase();
    i_Head = "";
    i_HeadCom = "";
    i_Description = description;
    i_OpenMemos = Parent->commserv.DEF_OpenMemos();
    i_Secure = Parent->commserv.DEF_Secure();
    i_Private = Parent->commserv.DEF_Private();
}


void Committee::operator=(const Committee &in)
{
    FT("Committee::operator=", ("(const Committee &) in"));

    i_Name = in.i_Name;
    i_Head = in.i_Head;
    i_HeadCom = in.i_HeadCom;
    i_Description = in.i_Description;
    i_OpenMemos = in.i_OpenMemos;
    i_Secure = in.i_Secure;
    i_Private = in.i_Private;

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
	entlist_t tmp(entry, nick, modtime);
	member = i_Members.insert(i_Members.end(), tmp);
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
	RET(IsIn(nick));
    }
    RET(false);
}


bool Committee::MSG_insert(mstring entry, mstring nick)
{
    FT("Committee::MSG_insert", (entry, nick));

    if (IsHead(nick))
    {
	entlist_t tmp(entry, nick);
	message = i_Messages.insert(i_Messages.end(), tmp);
	RET(true);
    }
    else
    {
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
	for (iter=i_Messages.begin(), i=0; iter!=i_Messages.end() &&
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


bool CommServ::IsList(mstring in)
{
    FT("CommServ::IsList", (in));
    RET(list.find(in.UpperCase())!=list.end());
}

void CommServ::AddCommands()
{
    NFT("CommServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
		"H*LP", Parent->commserv.ALL_Name(), CommServ::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"NEW", Parent->commserv.SOP_Name(), CommServ::do_AddComm);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"KILL", Parent->commserv.SOP_Name(), CommServ::do_DelComm);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ADD", Parent->commserv.REGD_Name(), CommServ::do_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"DEL*", Parent->commserv.REGD_Name(), CommServ::do_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ERA*", Parent->commserv.REGD_Name(), CommServ::do_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"MEM*", Parent->commserv.REGD_Name(), CommServ::do_Memo);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LIST", Parent->commserv.REGD_Name(), CommServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"VIEW", Parent->commserv.REGD_Name(), CommServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"INFO", Parent->commserv.ALL_Name(), CommServ::do_Info);

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
		"SET* SEC*", Parent->commserv.REGD_Name(), CommServ::do_set_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PRIV*", Parent->commserv.REGD_Name(), CommServ::do_set_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* *MEMO*", Parent->commserv.REGD_Name(), CommServ::do_set_OpenMemos);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOG* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOG*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET*", Parent->commserv.REGD_Name(), do_1_3param);

}

void CommServ::RemCommands()
{
    NFT("CommServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

//  Parent->commands.RemSystemCommand(GetInternalName(),
//		    "TRACE", Parent->commserv.ALL_Name());
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
    mynick  = data.ExtractWord(3, ": ");
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
	send(mynick, source, "Invalid command.");
    }

    mThread::ReAttach(tt_mBase);

}

void CommServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Help", (mynick, source, params));

}

void CommServ::do_AddComm(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_AddComm", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring head      = params.ExtractWord(3, " ");
    mstring desc      = params.After(" ", 3);

    if (Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " already exists.");
	return;
    }

    if (!Parent->nickserv.IsStored(head))
    {
	::send(mynick, source, "Head of committee is not registered.");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "Cannot add DEFAULT committees.");
	return;
    }

    Parent->commserv.list[committee] = Committee(committee, head, desc);
    ::send(mynick, source, "Committee " + committee + " has been created.");
}


void CommServ::do_DelComm(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_DelComm", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "Cannot remove DEFAULT committees.");
	return;
    }

    Parent->commserv.list.erase(committee);
    ::send(mynick, source, "Committee " + committee + " has been dropped.");
}


void CommServ::do_Add(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Add", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring member    = params.ExtractWord(3, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "The " + committee + " list cannot be modified online.");
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, "You are not head of " + committee + ".");
	return;
    }

    if (!Parent->nickserv.IsStored(member))
    {
	::send(mynick, source, "Nickname " + member + " is not registered.");
	return;
    }

    if (Parent->commserv.list[committee].IsIn(member))
    {
	::send(mynick, source, "Nickname " + member + " is already in " +
				committee + ".");
	return;
    }

    Parent->commserv.list[committee].insert(member, source);
    ::send(mynick, source, "Nickname " + member + " has been added to " +
							    committee + ".");
}


void CommServ::do_Del(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Del", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring member    = params.ExtractWord(3, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "The " + committee + " list cannot be modified online.");
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, "You are not head of " + committee + ".");
	return;
    }

    if (!Parent->commserv.list[committee].IsIn(member))
    {
	::send(mynick, source, "Nickname " + member + " is not on " + committee + ".");
	return;
    }

    if (Parent->commserv.list[committee].IsHead(member))
    {
	::send(mynick, source, "Nickname " + member + " the head of " + committee + ".");
	return;
    }

    Committee *comm = &Parent->commserv.list[committee];
    for (comm->member = comm->begin(); comm->member != comm->end(); comm->member++)
    {
	if (comm->member->Entry().LowerCase() == member.LowerCase())
	{
	    Parent->commserv.list[committee].erase();
	    ::send(mynick, source, "Nickname " + member +
			    " has been removed from " + committee + ".");
	}
    }
}

void CommServ::do_Memo(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Memo", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring text      = params.After(" ", 2);

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "Committee " + committee + " is a dynamic committee.");
	return;
    }

    if (!Parent->commserv.list[committee].IsOn(source))
    {
	::send(mynick, source, "You are not on committee " + committee + ".");
	return;
    }

    if (!Parent->commserv.list[committee].OpenMemos() &&
	!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, "Access denied.");
	return;
    }

    CommServ::do_Memo2(source, committee, text);
    ::send(mynick, source, "Memo sent to all members of " + committee + ".");
}


void CommServ::do_Memo2(mstring source, mstring committee, mstring text)
{
    FT("CommServ::do_Memo2", (source, committee, text));

    Committee *comm;
    if (Parent->commserv.IsList(committee))
	comm = &Parent->commserv.list[committee.UpperCase()];
    else
	return;

    Nick_Stored_t *me;
    if (Parent->nickserv.IsStored(source))
	me = &Parent->nickserv.stored[source.LowerCase()];
    else
	return;

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
	    if (((me->Host() != "") ? (realrecipiant != me->Host()) :
		(realrecipiant.LowerCase() != me->Name().LowerCase())) &&
		(comm->Head() == realrecipiant ||
		Parent->nickserv.IsStored(realrecipiant)))
	    {
		Parent->memoserv.nick[realrecipiant.LowerCase()].push_back(
		    Memo_t(realrecipiant, source, "[" + IRC_Bold +
				committee + IRC_Off + "] " + text));
	    }
	}
    }

    for (comm->member = comm->begin(); comm->member != comm->end(); comm->member++)
    {
	if (Parent->nickserv.IsStored(comm->member->Entry()))
	{
	    mstring realrecipiant = Parent->nickserv.stored[comm->member->Entry().LowerCase()].Host();
	    if (realrecipiant == "")
		realrecipiant = comm->member->Entry();
	    if (((me->Host() != "") ? (realrecipiant != me->Host()) :
		(realrecipiant.LowerCase() != me->Name().LowerCase())) &&
	        (comm->member->Entry() == realrecipiant ||
		Parent->nickserv.IsStored(realrecipiant)))
	    {
		Parent->memoserv.nick[realrecipiant.LowerCase()].push_back(
		    Memo_t(realrecipiant, source, "[" + IRC_Bold +
				committee + IRC_Off + "] " + text));
	    }
	}
    }
}


void CommServ::do_List(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_List", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "Committee " + committee + " is a dynamic committee.");
	return;
    }

    if (Parent->commserv.list[committee].Private() &&
	!(Parent->commserv.list[committee].IsOn(source) ||
	(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))))
    {
	::send(mynick, source, "Access denied.");
	return;
    }

    ::send(mynick, source, "Members of " + Parent->commserv.list[committee].Description() + ":");
    CommServ::do_List2(mynick, source, committee, true, 1);
}


int CommServ::do_List2(mstring mynick, mstring source, mstring committee, bool first, int number)
{
    FT("CommServ::do_List2", (mynick, source, committee, first, number));

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
	    nextnum += CommServ::do_List2(mynick, source, comm->HeadCom(), false, nextnum);
	}
    }
    else if (comm->Head() != "")
    {
	output = "";
	output << nextnum++ << ". " <<
		IRC_Bold << comm->Head() << IRC_Off <<
		(Parent->nickserv.IsStored(comm->Head()) ? "" : " [DEFUNCT]");
	::send(mynick, source, output);
    }

    for (comm->member = comm->begin(); comm->member != comm->end(); comm->member++)
    {
	output = "";
	output << nextnum++ << ". " <<
		(first ? Blank : IRC_Bold) <<
		comm->member->Entry() <<
		(first ? Blank : IRC_Off) <<
		(Parent->nickserv.IsStored(comm->member->Entry()) ? "" : " [DEFUNCT]");
	::send(mynick, source, output);
    }
    RET(nextnum-number);
}

void CommServ::do_Info(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_Info", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "Committee " + committee + " is a dynamic committee.");
	return;
    }

    mstring output;
    Committee *comm = &Parent->commserv.list[committee];
    ::send(mynick, source, committee + " is " + comm->Description());
    if (comm->HeadCom() != "")
    {
	::send(mynick, source, "   Head: " + IRC_Bold + comm->HeadCom() +
							IRC_Off);
    }
    else if (comm->Head() != "")
    {
	::send(mynick, source, "   Head: " + comm->Head());
    }

    output = "";
    if (comm->size())
    {
	if (output.size())
	    output << ", ";
	output << comm->size() << " members";
    }
    if (comm->MSG_size())
    {
	if (output.size())
	    output << ", ";
	output << comm->MSG_size() << " messages";
    }
    if (output.size())
	::send(mynick, source, "  Stats: " + output);

    output = "";
    if (comm->Secure())
    {
	if (output.size())
	    output << ", ";
	output << "Secure";
    }
    if (comm->Private())
    {
	if (output.size())
	    output << ", ";
	output << "Private";
    }
    if (comm->OpenMemos())
    {
	if (output.size())
	    output << ", ";
	output << "OpenMemos";
    }
    if (output.size())
	::send(mynick, source, "Options: " + output);
}

void CommServ::do_logon_Add(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_logon_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring msgnum    = params.After(" ", 3);

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, "You are not head of " + committee + ".");
	return;
    }

    Parent->commserv.list[committee].MSG_insert(msgnum, source);
    mstring output = "";
    output << "Entry #" << Parent->commserv.list[committee].MSG_size() <<
	    " has been added to " << committee << " logon messages.";
    ::send(mynick, source, output);
}


void CommServ::do_logon_Del(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_logon_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring msgnum    = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, "You are not head of " + committee + ".");
	return;
    }

    if (!msgnum.IsNumber() || atoi(msgnum.c_str()) < 1)
    {
	::send(mynick, source, "You must specify a POSETIVE NUMBER to delete.");
	return;
    }

    if (atoi(msgnum.c_str()) > Parent->commserv.list[committee].MSG_size())
    {
	::send(mynick, source, "There are not that many logon messages for "
			+ committee + ".");
	return;
    }

    int i;
    Committee *comm = &Parent->commserv.list[committee];
    for (i=1, comm->message = comm->MSG_begin();
	comm->message != comm->MSG_end(); comm->message++, i++)
    {
	if (i==atoi(msgnum.c_str()))
	{
	    Parent->commserv.list[committee].MSG_erase();
	    ::send(mynick, source, "Message #" + msgnum +
		" has been removed from " + committee + " logon messges.");
	}
    }
}


void CommServ::do_logon_List(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_logon_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source) &&
	!(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source)))
    {
	::send(mynick, source, "You are not head of " + committee + ".");
	return;
    }

    if (!Parent->commserv.list[committee].MSG_size())
    {
	::send(mynick, source, "There are no logon messages for " + committee + ".");
	return;
    }

    int i;
    mstring output;
    Committee *comm = &Parent->commserv.list[committee];
    ::send(mynick, source, "Messages for " + comm->Description() + ":");
    for (i=1, comm->message = comm->MSG_begin();
	comm->message != comm->MSG_end(); comm->message++, i++)
    {
	output = "";
	output << i << ". " << comm->message->Entry();
	::send(mynick, source, output);
    }
}


void CommServ::do_set_Head(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_Head", (mynick, source, params));

    mstring message = mstring(params.Before(" ") +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring newhead   = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, "You are not head of " + committee + ".");
	return;
    }

    if (Parent->commserv.list[committee].Head() == "")
    {
	::send(mynick, source, "You cannot re-assign a committee with more than one head.");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "Settings may not be changed on DEFAULT committees.");
	return;
    }

    if (!Parent->nickserv.IsStored(newhead))
    {
	::send(mynick, source, "Nickname " + newhead + " is not registered.");
	return;
    }

    if (newhead.LowerCase() == source.LowerCase())
    {
	::send(mynick, source, "Yeah, right ... pull the other one.");
	return;
    }

    Parent->commserv.list[committee].Head(newhead);
    ::send(mynick, source, "Head of committee  " + committee + " is now " +
	    newhead + ".");
}


void CommServ::do_set_Secure(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, "You are not head of " + committee + ".");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "Settings may not be changed on DEFAULT committees.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->commserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->commserv.list[committee].Secure(onoff.GetBool());
    ::send(mynick, source, "Secure is now set to " +
	    mstring(onoff.GetBool() ? "ON" : "OFF") + " for committee " +
	    committee + ".");
}


void CommServ::do_set_Private(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, "You are not head of " + committee + ".");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "Settings may not be changed on DEFAULT committees.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->commserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->commserv.list[committee].Private(onoff.GetBool());
    ::send(mynick, source, "Private is now set to " +
	    mstring(onoff.GetBool() ? "ON" : "OFF") + " for committee " +
	    committee + ".");
}


void CommServ::do_set_OpenMemos(mstring mynick, mstring source, mstring params)
{
    FT("CommServ::do_set_OpenMemos", (mynick, source, params));

    mstring message = mstring(params.Before(" ") +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->commserv.IsList(committee))
    {
	::send(mynick, source, "Committee " + committee + " does not exist.");
	return;
    }

    if (!Parent->commserv.list[committee].IsHead(source))
    {
	::send(mynick, source, "You are not head of " + committee + ".");
	return;
    }

    if (committee == Parent->commserv.SADMIN_Name() ||
	committee == Parent->commserv.SOP_Name() ||
	committee == Parent->commserv.ADMIN_Name() ||
	committee == Parent->commserv.OPER_Name() ||
	committee == Parent->commserv.ALL_Name() ||
	committee == Parent->commserv.REGD_Name())
    {
	::send(mynick, source, "Settings may not be changed on DEFAULT committees.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->commserv.DEF_OpenMemos())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->commserv.list[committee].OpenMemos(onoff.GetBool());
    ::send(mynick, source, "OpenMemos is now set to " +
	    mstring(onoff.GetBool() ? "ON" : "OFF") + " for committee " +
	    committee + ".");
}


void CommServ::load_database(wxInputStream& in)
{
    int i;
    in>>i;
    Committee tmpcommitee;
    for(int j=0;j<i;j++)
    {
	in>>tmpcommitee;
	list[tmpcommitee.Name()]=tmpcommitee;
    }
}

void CommServ::save_database(wxOutputStream& out)
{
    out<<list.size();
    for(map<mstring,Committee>::iterator i=list.begin();i!=list.end();i++)
	out<<i->second;
}

wxOutputStream &operator<<(wxOutputStream& out,Committee& in)
{
    out<<in.i_Name<<in.i_HeadCom<<in.i_Head<<in.i_Description;

    out<<in.i_Members.size();
    for(in.member=in.i_Members.begin();in.member!=in.i_Members.end();in.member++)
	out<<(*in.member);

    out<<in.i_OpenMemos<<in.i_Private<<in.i_Secure;

    out<<in.i_Messages.size();
    for(in.message=in.i_Messages.begin();in.message!=in.i_Messages.end();in.message++)
	out<<(*in.message);

    return out;
}
wxInputStream &operator>>(wxInputStream& in, Committee& out)
{
    set<entlist_t>::size_type locsize,i;
    entlist_t locent;
    // need to write lock out.

    in>>out.i_Name>>out.i_HeadCom>>out.i_Head>>out.i_Description;

    in>>locsize;
    out.i_Members.clear();
    for(i=0;i<locsize;i++)
    {
	in>>locent;
	out.i_Members.insert(locent);
    }

    in>>out.i_OpenMemos>>out.i_Private>>out.i_Secure;

    in>>locsize;
    out.i_Messages.clear();
    for(i=0;i<locsize;i++)
    {
	in>>locent;
	out.i_Messages.push_back(locent);
    }

    return in;
}
