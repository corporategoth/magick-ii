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
#include "operserv.h"
#include "magick.h"

bool OperServ::AddHost(mstring host)
{
    FT("OperServ::AddHost", (host));

    if (CloneList[host.LowerCase()] < 1)
	CloneList[host.LowerCase()] = 0;
    CloneList[host.LowerCase()]++;

    {
    //MLOCK("OperServ","Clone");
    if (Clone_find(host))
    {
	if (CloneList[host.LowerCase()] > Clone->Value().first)
	{
	    RET(true);
	}
    }
    else
    {
	if (CloneList[host.LowerCase()] > Clone_Limit())
	{
	    RET(true);
	}
    }
    }
    RET(false);
}

void OperServ::RemHost(mstring host)
{
    FT("OperServ::RemHost", (host));

    if (CloneList.find(host.LowerCase()) != CloneList.end())
    {
	if (CloneList[host.LowerCase()] > 1)
	    CloneList[host.LowerCase()]--;
	else
	    CloneList.erase(host.LowerCase());
    }
}

unsigned int OperServ::CloneList_sum()
{
    NFT("OperServ::Clones");

    map<mstring, unsigned int>::iterator i;
    unsigned int value = 0;

    for (i=CloneList.begin(); i!=CloneList.end(); i++)
    {
	value += i->second;
    }
    RET(value);
}


bool OperServ::Clone_insert(mstring entry, unsigned int value, mstring reason, mstring nick, mDateTime added)
{
    FT("OperServ::Clone_insert", (entry, value, reason, nick, added));

    // Wildcards but no @
    if (entry.Contains("@") || entry.Contains("!"))
    {
	    RET(false);
    }

    if (!Clone_find(entry))
    {
	pair<set<entlist_val_t<pair<unsigned int, mstring> > >::iterator,bool> tmp;
	tmp = i_Clone.insert(entlist_val_t<pair<unsigned int, mstring> >(
			entry, pair<unsigned int, mstring>(value, reason), nick, added));
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

    if (Clone != i_Clone.end())
    {
	i_Clone.erase(Clone);
	Clone = i_Clone.end();
	RET(true);
    }
    else
    {
	RET(false);
    }
}


bool OperServ::Clone_find(mstring entry)
{
    FT("OperServ::Clone_find", (entry));

//  entlist_val_ui<pair<int, mstring> > iter = i_Clone.end();
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
    {
    MLOCK(("OperServ", "Clone"));
//  entlist_val_ui<pair<int, mstring> > iter = Clone;
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator iter = Clone;

    if (Clone_find(entry))
	retval=Clone->Value();
    Clone = iter;
    }
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

    if (!Akill_find(entry))
    {
	pair<set<entlist_val_t<pair<unsigned long, mstring> > >::iterator, bool> tmp;
	tmp = i_Akill.insert(entlist_val_t<pair<unsigned long, mstring> >(
			entry, pair<unsigned long, mstring>(value, reason), nick, added));
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

    if (Akill != i_Akill.end())
    {
	i_Akill.erase(Akill);
	Akill = i_Akill.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

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
    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator iter = i_Akill.end();
    if (!i_Akill.empty())
	for (iter=i_Akill.begin(); iter!=i_Akill.end(); iter++)
	    if (entry.LowerCase().Matches(iter->Entry().LowerCase()))
		break;

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
    {
    MLOCK(("OperServ", "Akill"));
//  entlist_val_ui<pair<long, mstring> > iter = Akill;
    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator iter = Akill;

    if (Akill_find(entry))
	retval=Akill->Value();
    Akill = iter;
    }
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

    if (!OperDeny_find(entry))
    {
	pair<set<entlist_val_t<mstring > >::iterator, bool> tmp;
	tmp = i_OperDeny.insert(entlist_val_t<mstring>(entry, value, nick));
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

    if (OperDeny != i_OperDeny.end())
    {
	i_OperDeny.erase(OperDeny);
	OperDeny = i_OperDeny.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

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
    {
    MLOCK(("OperServ", "OperDeny"));
//  entlist_val_ui<mstring> iter = OperDeny;
    set<entlist_val_t<mstring> >::iterator iter = OperDeny;

    if (OperDeny_find(entry))
	retval=OperDeny->Value();
    OperDeny = iter;
    }
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

    if (!Ignore_find(entry))
    {
	pair<set<entlist_val_t<bool> >::iterator, bool> tmp;
	tmp = i_Ignore.insert(entlist_val_t<bool>(entry, perm, nick));
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

    if (Ignore != i_Ignore.end())
    {
	i_Ignore.erase(Ignore);
	Ignore = i_Ignore.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

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
    {
    MLOCK(("OperServ", "Ignore"));
//  entlist_val_ui<pair<mDateTime, bool> > iter = Ignore;
    set<entlist_val_t<bool> >::iterator iter = Ignore;

    if (Ignore_find(entry))
	retval=Ignore->Value();
    Ignore = iter;
    }
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
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "TRACE", Parent->commserv.SADMIN_Name(), OperServ::do_Trace);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*MODE*", Parent->commserv.OPER_Name(), OperServ::do_Mode);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "Q*LINE*", Parent->commserv.ADMIN_Name(), OperServ::do_Qline);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNQ*LINE*", Parent->commserv.ADMIN_Name(), OperServ::do_UnQline);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "NO*OP*", Parent->commserv.ADMIN_Name(), OperServ::do_NOOP);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "KILL*", Parent->commserv.SOP_Name(), OperServ::do_Kill);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*PING*", Parent->commserv.OPER_Name(), OperServ::do_Ping);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UPD*", Parent->commserv.SOP_Name(), OperServ::do_Update);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SHUT*DOWN*", Parent->commserv.SADMIN_Name(), OperServ::do_Shutdown);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "RELOAD*", Parent->commserv.SADMIN_Name(), OperServ::do_Reload);
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
	    "SET* CONF*", Parent->commserv.OPER_Name(), OperServ::do_settings_Config);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* NICK*", Parent->commserv.OPER_Name(), OperServ::do_settings_Nick);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* CHAN*", Parent->commserv.OPER_Name(), OperServ::do_settings_Channel);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* OTH*", Parent->commserv.OPER_Name(), OperServ::do_settings_Other);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *ALL*", Parent->commserv.OPER_Name(), OperServ::do_settings_All);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* ADD*", Parent->commserv.SOP_Name(), OperServ::do_clone_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* DEL*", Parent->commserv.SOP_Name(), OperServ::do_clone_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* ERA*", Parent->commserv.SOP_Name(), OperServ::do_clone_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* LIST", Parent->commserv.OPER_Name(), OperServ::do_clone_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* VIEW", Parent->commserv.OPER_Name(), OperServ::do_clone_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL ADD*", Parent->commserv.OPER_Name(), OperServ::do_akill_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL DEL*", Parent->commserv.OPER_Name(), OperServ::do_akill_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL ERA*", Parent->commserv.OPER_Name(), OperServ::do_akill_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL LIST", Parent->commserv.OPER_Name(), OperServ::do_akill_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL VIEW", Parent->commserv.OPER_Name(), OperServ::do_akill_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* ADD*", Parent->commserv.SOP_Name(), OperServ::do_operdeny_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* DEL*", Parent->commserv.SOP_Name(), OperServ::do_operdeny_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* ERA*", Parent->commserv.SOP_Name(), OperServ::do_operdeny_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* LIST", Parent->commserv.OPER_Name(), OperServ::do_operdeny_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* VIEW", Parent->commserv.OPER_Name(), OperServ::do_operdeny_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* ADD*", Parent->commserv.SOP_Name(), OperServ::do_ignore_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* DEL*", Parent->commserv.SOP_Name(), OperServ::do_ignore_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* ERA*", Parent->commserv.SOP_Name(), OperServ::do_ignore_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* LIST", Parent->commserv.OPER_Name(), OperServ::do_ignore_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* VIEW", Parent->commserv.OPER_Name(), OperServ::do_ignore_List);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *", Parent->commserv.OPER_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET*", Parent->commserv.OPER_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE* *", Parent->commserv.OPER_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLONE*", Parent->commserv.OPER_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL* *", Parent->commserv.OPER_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KILL*", Parent->commserv.OPER_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY* *", Parent->commserv.OPER_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "O*DENY*", Parent->commserv.OPER_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN* *", Parent->commserv.OPER_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "IGN*", Parent->commserv.OPER_Name(), do_1_2param);
}

void OperServ::RemCommands()
{
    NFT("OperServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.RemSystemCommand(GetInternalName(),
		    "TRACE", Parent->commserv.SADMIN_Name());
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
	      command.Matches("ID*")))
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
    else if (!Parent->commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);
}

void OperServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Help", (mynick, source, params));
}


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

    unsigned short i, j, k;
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
	if (levels[0U].RxMatches("0X[0-9A-F]\\{4\\}") || levels[0U] == "0")
	{
	    if (type == tt_MAX)
	    {
		for (i=tt_MAIN; i<tt_MAX; i++)
		{
		    Trace::TurnSet((threadtype_enum) i, makehex(levels[0U]));
		    output.Format("%s SET: Trace level set to %#06x.",
			Now().DateTimeString().c_str(), Trace::TraceLevel((threadtype_enum) i));
		    ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>((threadtype_enum) i, output));
		}
	    }
	    else
	    {
		Trace::TurnSet(type, makehex(levels[0U]));
		output.Format("%s SET: Trace level set to %#06x.",
		    Now().DateTimeString().c_str(), Trace::TraceLevel(type));
		ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>(type, output));
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
				ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>((threadtype_enum) k, output));
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
				ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>(type, output));
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
			    ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>((threadtype_enum) k, output));
			}
		    }
		    else
		    {
			Trace::TurnUp(type, Trace::levelname[j].level);
			output.Format("%s UP: Trace level set to %#06x.",
			    Now().DateTimeString().c_str(), Trace::TraceLevel(type));
			ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>(type, output));
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
			    ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>((threadtype_enum) k, output));
			}
		    }
		    else
		    {
			Trace::TurnDown(type, Trace::levelname[j].level);
			output.Format("%s DOWN: Trace level set to %#06x.",
			    Now().DateTimeString().c_str(), Trace::TraceLevel(type));
			ThreadMessageQueue.push_back(pair<threadtype_enum, mstring>(type, output));
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
			(message + action).c_str(), mynick.c_str(), message.c_str());
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
}


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
		Parent->server.SVSMODE(mynick, target, mode);
		Parent->operserv.stats.i_Mode++;
		announce(mynick, Parent->getMessage("MISC/NICK_MODE"),
			source.c_str(), mode.c_str(), target.c_str());
		::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/NICK_MODE"),
			mode.c_str(), target.c_str());

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
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target  = params.ExtractWord(2, " ");
    mstring reason  = params.After(" ", 2);
    Parent->server.QLINE(mynick, target, reason);
    Parent->operserv.stats.i_Qline++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/QLINE"),
		target.c_str(), Parent->getMessage(source, "MISC/ON").c_str());
    announce(mynick, Parent->getMessage("MISC/QLINE"),
		source.c_str(), Parent->getMessage("MISC/ON").c_str(),
		target.c_str());
}


void OperServ::do_UnQline(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_UnQline", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target  = params.ExtractWord(2, " ");
    Parent->server.UNQLINE(mynick, target);
    Parent->operserv.stats.i_Unqline++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/QLINE"),
		target.c_str(), Parent->getMessage(source, "MISC/OFF").c_str());
    announce(mynick, Parent->getMessage("MISC/QLINE"),
		source.c_str(), Parent->getMessage("MISC/OFF").c_str(),
		target.c_str());
}


void OperServ::do_NOOP(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_NOOP", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
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
}


void OperServ::do_Kill(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Kill", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
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
    Parent->events->ForcePing();
    Parent->operserv.stats.i_Ping++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/PING"));
}


void OperServ::do_Update(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Update", (mynick, source, params));
    Parent->events->ForceSave();
    Parent->operserv.stats.i_Update++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/UPDATE"));
}


void OperServ::do_Shutdown(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Shutdown", (mynick, source, params));
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/SHUTDOWN"));
    announce(mynick, Parent->getMessage("MISC/SHUTDOWN"), source.c_str());
#ifdef WIN32
    Sleep(1000);
#else
    sleep(1);
#endif
    Parent->Shutdown(true);
    Parent->Die();
}


void OperServ::do_Reload(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Reload", (mynick, source, params));
    if (Parent->get_config_values())
    {
	Parent->operserv.stats.i_Reload++;
	::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/RELOAD"));
	announce(mynick, Parent->getMessage("MISC/RELOAD"), source.c_str());
    }
    else
    {
	wxLogError("Could not read magick config file %s.", Parent->Config_File().c_str());
	::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/RELOAD_FAIL"));
    }
}


void OperServ::do_Unload(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Unload", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring language = params.ExtractWord(2, " ");

    if (Parent->UnloadExternalMessages(language))
    {
	Parent->operserv.stats.i_Unload++;
	::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/UNLOAD"),
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
}


void OperServ::do_On(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_On", (mynick, source, params));

    // Later, make the ability to turn on/off specific services
    // also the ability to turn of either MSG, or AUTO or BOTH
    Parent->MSG(true);
    Parent->operserv.stats.i_OnOff++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/ONOFF"),
	    Parent->getMessage(source, "MISC/ON").c_str());
    announce(mynick, Parent->getMessage("MISC/ONOFF"),
	    Parent->getMessage("MISC/ON").c_str(), source.c_str());
}


void OperServ::do_Off(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Off", (mynick, source, params));

    // Later, make the ability to turn on/off specific services
    // also the ability to turn of either MSG, or AUTO or BOTH
    Parent->MSG(false);
    Parent->operserv.stats.i_OnOff++;
    ::send(mynick, source, Parent->getMessage(source, "OS_COMMAND/ONOFF"),
	    Parent->getMessage(source, "MISC/OFF").c_str());
    announce(mynick, Parent->getMessage("MISC/ONOFF"),
	    Parent->getMessage("MISC/OFF").c_str(), source.c_str());
}


void OperServ::do_settings_Config(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Config", (mynick, source, params));

/*
-   Databases are (not) encrypted, and compressed at level ?.
-   Minimum threads active is ?, Current threads active is ?.
-   New thread will spawn each ? messages, and die when below ?.
*/
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_LEVEL"),
		    Parent->startup.Level(), Parent->Level());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_LAG"),
		    ToHumanTime(Parent->startup.Lagtime()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_SERVERS"),
		    Parent->startup.Server_size());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_RELINK"),
		    ToHumanTime(Parent->config.Server_Relink()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_SQUIT1"),
		    ToHumanTime(Parent->config.Squit_Protect()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_SQUIT1"),
		    ToHumanTime(Parent->config.Squit_Cancel()).c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_SYNC"),
		    ToHumanTime(Parent->config.Cycletime()).c_str(),
		    Parent->events->SyncTime().c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/CFG_CYCLE"),
		    ToHumanTime(Parent->config.Checktime()).c_str(),
		    ToHumanTime(Parent->config.Ping_Frequency()).c_str());
}
    
void OperServ::do_settings_Nick(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Nick", (mynick, source, params));

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
		    Parent->nickserv.PicSize());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_PICEXT"),
		    Parent->nickserv.PicExt().c_str());
    ::send(mynick, source, Parent->getMessage(source, "OS_SETTINGS/NICK_FILES"),
		    Parent->memoserv.Files(),
		    Parent->memoserv.FileSize());
}


void OperServ::do_settings_Channel(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Channel", (mynick, source, params));

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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER"));
	return;
    }

    unsigned int num = atoi(amount.c_str());
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
    }
    else
    {
	Parent->operserv.Clone_insert(host, num, reason, source);
	Parent->operserv.stats.i_Clone++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD_LEVEL"),
		    host.c_str(),
		    Parent->getMessage(source, "LIST/CLONE").c_str(),
		    num);
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
	unsigned int i, num = atoi(host.c_str());
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
	    Parent->operserv.Clone_erase();

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
	Parent->commserv.list[Parent->commserv.SADMIN_Name().UpperCase()].IsOn(source) &&
	time > Parent->operserv.Expire_SAdmin())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/AKILLTOOHIGH"),
		    ToHumanTime(Parent->operserv.Expire_SAdmin()).c_str());
	return;
    }
    else if (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name().UpperCase()].IsOn(source) &&
	time > Parent->operserv.Expire_Sop())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/AKILLTOOHIGH"),
		    ToHumanTime(Parent->operserv.Expire_Sop()).c_str());
	return;
    }
    else if (Parent->commserv.IsList(Parent->commserv.ADMIN_Name()) &&
	Parent->commserv.list[Parent->commserv.ADMIN_Name().UpperCase()].IsOn(source) &&
	time > Parent->operserv.Expire_Admin())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/AKILLTOOHIGH"),
		    ToHumanTime(Parent->operserv.Expire_Admin()).c_str());
	return;
    }
    else if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name().UpperCase()].IsOn(source) &&
	time > Parent->operserv.Expire_Oper())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/AKILLTOOHIGH"),
		    ToHumanTime(Parent->operserv.Expire_Oper()).c_str());
	return;
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

    MLOCK(("OperServ", "Akill"));
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
    }
    else
    {
	Parent->operserv.Akill_insert(host, time, reason, source);
	Parent->operserv.stats.i_Akill++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD_TIME"),
		    host.c_str(),
		    Parent->getMessage(source, "LIST/AKILL").c_str(),
		    ToHumanTime(time).c_str());
    }
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
	unsigned int i, num = atoi(host.c_str());
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
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL"),
			Parent->operserv.Akill->Entry().c_str(),
			Parent->getMessage(source, "LIST/AKILL").c_str());
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
	    Parent->operserv.Akill_erase();

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

    MLOCK(("OperServ", "OperDeny"));
    if (Parent->operserv.OperDeny_find(host))
    {
	Parent->operserv.OperDeny_erase();
    }
    Parent->operserv.OperDeny_insert(host, reason, source);
    Parent->operserv.stats.i_OperDeny++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/ADD"),
	host.c_str(), Parent->getMessage(source, "LIST/OPERDENY").c_str());
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
	unsigned int i, num = atoi(host.c_str());
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
	    Parent->operserv.OperDeny_erase();

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
			    Parent->operserv.Akill->Value().second.c_str());
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

    MLOCK(("OperServ", "Ignore"));
    if (Parent->operserv.Ignore_find(host))
    {
	Parent->operserv.Ignore_erase();
    }
    Parent->operserv.Ignore_insert(host, true, source);
    Parent->operserv.stats.i_Ignore++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/ADD"),
	    host.c_str(), Parent->getMessage(source, "LIST/SIGNORE").c_str());
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
	unsigned int i, num = atoi(host.c_str());
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
	    Parent->operserv.Ignore_erase();

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
		::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY_MATCH"),
			host.c_str(),
			Parent->getMessage(source, "LIST/SIGNORE").c_str());
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

void OperServ::load_database(wxInputStream& in)
{
    FT("OperServ::load_database", ("(wxInputStream &) in"));
    entlist_val_t<pair<unsigned int, mstring> > clone;
    set<entlist_val_t<pair<unsigned int, mstring> > >::size_type clone_c, i;
    entlist_val_t<pair<unsigned long, mstring> > akill;
    set<entlist_val_t<pair<unsigned long, mstring> > >::size_type akill_c, j;
    entlist_val_t<mstring> operdeny;
    set<entlist_val_t<mstring> >::size_type operdeny_c, k;
    entlist_val_t<bool> ignore;
    set<entlist_val_t<bool> >::size_type ignore_c, l;

    in>>clone_c;
    CP(("Loading CLONE entries (%d) ...", clone_c));
    for (i=0; i<clone_c; i++)
    {
	COM(("Loading CLONE entry %d ...", i));
	in>>clone;
	i_Clone.insert(clone);
	COM(("Entry CLONE %s loaded ...", clone.Entry().c_str()));
    }

    in>>akill_c;
    CP(("Loading AKILL entries (%d) ...", akill_c));
    for (j=0; j<akill_c; j++)
    {
	COM(("Loading AKILL entry %d ...", j));
	in>>akill;
	i_Akill.insert(akill);
	COM(("Entry AKILL %s loaded ...", akill.Entry().c_str()));
    }

    in>>operdeny_c;
    CP(("Loading OPERDENY entries (%d) ...", operdeny_c));
    for (k=0; k<operdeny_c; k++)
    {
	COM(("Loading OPERDENY entry %d ...", k));
	in>>operdeny;
	i_OperDeny.insert(operdeny);
	COM(("Entry OPERDENY %s loaded ...", operdeny.Entry().c_str()));
    }

    in>>ignore_c;
    CP(("Loading IGNORE entries (%d) ...", ignore_c));
    for (l=0; l<ignore_c; l++)
    {
	COM(("Loading IGNORE entry %d ...", l));
	in>>ignore;
	i_Ignore.insert(ignore);
	COM(("Entry IGNORE %s loaded ...", ignore.Entry().c_str()));
    }
}

void OperServ::save_database(wxOutputStream& out)
{
    FT("OperServ::save_database", ("(wxOutputStream &) out"));
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator clone_i;
    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator akill_i;
    set<entlist_val_t<mstring> >::iterator operdeny_i;
    set<entlist_val_t<bool> >::iterator ignore_i;
    set<entlist_val_t<bool> >::size_type ignore_c = 0;

    CP(("Saving CLONE entries (%d) ...", i_Clone.size()));
    out<<i_Clone.size();
    for (clone_i=i_Clone.begin(); clone_i!=i_Clone.end(); clone_i++)
    {
	out<<*clone_i;
	COM(("Entry CLONE %s saved ...", clone_i->Entry().c_str()));
    }

    CP(("Saving AKILL entries (%d) ...", i_Akill.size()));
    out<<i_Akill.size();
    for (akill_i=i_Akill.begin(); akill_i!=i_Akill.end(); akill_i++)
    {
	out<<*akill_i;
	COM(("Entry AKILL %s saved ...", akill_i->Entry().c_str()));
    }

    CP(("Saving OPERDENY entries (%d) ...", i_OperDeny.size()));
    out<<i_OperDeny.size();
    for (operdeny_i=i_OperDeny.begin(); operdeny_i!=i_OperDeny.end(); operdeny_i++)
    {
	out<<*operdeny_i;
	COM(("Entry OPERDENY %s saved ...", operdeny_i->Entry().c_str()));
    }

    for (ignore_i=i_Ignore.begin(); ignore_i!=i_Ignore.end(); ignore_i++)
	if (ignore_i->Value())
	    ignore_c++;
    CP(("Saving IGNORE entries (%d) ...", ignore_c));
    out<<ignore_c;
    for (ignore_i=i_Ignore.begin(); ignore_i!=i_Ignore.end(); ignore_i++)
	if (ignore_i->Value())
	{
	    out<<*ignore_i;
	    COM(("Entry IGNORE %s saved ...", ignore_i->Entry().c_str()));
	}
}
