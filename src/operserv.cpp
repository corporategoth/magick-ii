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


bool OperServ::Clone_insert(mstring entry, unsigned int value, mstring reason, mstring nick)
{
    FT("OperServ::Clone_insert", (entry, value, reason, nick));

    // Wildcards but no @
    if (entry.Contains("@") || entry.Contains("!"))
    {
	    RET(false);
    }

    if (!Clone_find(entry))
    {
	pair<set<entlist_val_t<pair<unsigned int, mstring> > >::iterator,bool> tmp;
	tmp = i_Clone.insert(entlist_val_t<pair<unsigned int, mstring> >(
			entry, pair<unsigned int, mstring>(value, reason), nick));
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


bool OperServ::Akill_insert(mstring entry, unsigned long value, mstring reason, mstring nick)
{
    FT("OperServ::Akill_insert", (entry, value, reason, nick));

    // no @
    if (!entry.Contains("@") || entry.Contains("!"))
    {
	    RET(false);
    }

    if (!Akill_find(entry))
    {
	pair<set<entlist_val_t<pair<unsigned long, mstring> > >::iterator, bool> tmp;
	tmp = i_Akill.insert(entlist_val_t<pair<unsigned long, mstring> >(
			entry, pair<unsigned long, mstring>(value, reason), nick));
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
	    "KICK*", Parent->commserv.OPER_Name(), OperServ::do_Kick);
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
    mynick  = data.ExtractWord(3, ": ");
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
	    ::send(mynick, source, mstring("Invalid thread type \"") + ttype + mstring("\" is not valid."));
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
		    ::send(mynick, source, mstring("Trace level \"") + levels[i] +
		        mstring("\" is not valid, ignored."));
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
		::send(mynick, source, mstring("Trace level \"") + levels[i] +
		    mstring("\" is not valid, ignored."));
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
		::send(mynick, source, mstring("Trace level \"") + levels[i] +
		    mstring("\" is not valid, ignored."));
	}
    }
    else if (action == "VIEW" || action == "LIST")
    {
	// Its taken care of below :)
    }
    else
    {
	::send(mynick, source, "Incorrect TRACE option.");
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
	    announce(mynick, "MODE performed on channel " +
					    target + " (" + mode +
					    ") requested by " + source);
	}
	else
	{
	    ::send(mynick, source, "Channel " + target + " is not in use.");
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
		announce(mynick, "MODE performed on nickname " +
					    target + " (" + mode +
					    ") requested by " + source);
		::send(mynick, source, "MODE sent for nickname " + target);
	    }
	    else
	    {
		::send(mynick, source, "Nickname " + target + " is not online.");
	    }
	}
	else
	{
	    ::send(mynick, source, "Access denied.");
	}
    }
}


void OperServ::do_Kick(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Kick", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target  = params.ExtractWord(2, " ");
    mstring channel = params.ExtractWord(3, " ");
    mstring reason  = "Requested by " + source;
    if (params.WordCount(" ") > 3)
    {
	reason = params.After(" ", 3) + " (" + source + ")";
    }

    if (Parent->chanserv.IsLive(channel))
    {
	if (Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    Parent->server.KICK(mynick, target, channel, reason);
	    announce(mynick, "KICK performed on channel " +
					    channel + " for " + target +
					    " requested by " + source);
	}
	else
	{
	    ::send(mynick, source, "Nickname " + target +
				" is not in channel " + channel + ".");
	}
    }
    else
    {
	::send(mynick, source, "Channel " + channel + " is not in use.");
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
	::send(mynick, source, "Server specified is not linked.");
	return;
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "You may only turn NOOP ON or OFF");
	return;
    }

    Parent->server.NOOP(mynick, target, onoff.GetBool());
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
	announce(mynick, "SILENT KILL performed on nickname " +
					    target + " (" + reason +
					    ") requested by " + source);
    }
    else
    {
	::send(mynick, source, "Nickname " + target + " is not online.");
    }
}


void OperServ::do_Ping(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Ping", (mynick, source, params));
    Parent->events.ForcePing();
    ::send(mynick, source, "Server PING's have been sent out.");
}


void OperServ::do_Update(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Update", (mynick, source, params));
    Parent->events.ForceSave();
    ::send(mynick, source, "Updating Databases.");
}


void OperServ::do_Shutdown(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Shutdown", (mynick, source, params));
    ::send(mynick, source, "Shutting down ...");
    announce(mynick, "SHUTDOWN command received by " + source + ".");
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
	::send(mynick, source, "Configuration reloaded.");
    }
    else
    {
	wxLogError("Could not read magick config file %s.", Parent->Config_File().c_str());
	::send(mynick, source, "WARNING: Could not read config file.");
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
	::send(mynick, source, "Language " + language.UpperCase() + " unloaded.");
    }
    else
    {
	::send(mynick, source, "Language " + language.UpperCase() + " was not loaded.");
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
    ::send(mynick, source, "Server " + target + " JUPED.");
    announce(mynick, "Server " + target + " JUPED by " + source + ".");
}


void OperServ::do_On(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_On", (mynick, source, params));

    // Later, make the ability to turn on/off specific services
    // also the ability to turn of either MSG, or AUTO or BOTH
    Parent->MSG(true);
    ::send(mynick, source, "Services will respond to messages.");
    announce(mynick, "Services turned ON by " + source + ".");
}


void OperServ::do_Off(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_Off", (mynick, source, params));

    // Later, make the ability to turn on/off specific services
    // also the ability to turn of either MSG, or AUTO or BOTH
    Parent->MSG(false);
    ::send(mynick, source, "Services will not respond to messages.");
    announce(mynick, "Services turned OFF by " + source + ".");
}


void OperServ::do_settings_Config(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Config", (mynick, source, params));

/*
-   Databases are (not) encrypted, and compressed at level ?.
-   Minimum threads active is ?, Current threads active is ?.
-   New thread will spawn each ? messages, and die when below ?.
*/
    ::send(mynick, source, "Base level is " + mstring(itoa(Parent->startup.Level())) +
		    ", Current level is " + mstring(itoa(Parent->Level())) + ".");
    ::send(mynick, source, "Services have " + mstring(itoa(Parent->startup.Server_size())) +
		    " possible servers to connect to.");
    ::send(mynick, source, "Level is increased if lag is more than " +
		    ToHumanTime(Parent->startup.Lagtime()) + ".");
    ::send(mynick, source, "Services will relink in " +
		    ToHumanTime(Parent->config.Server_Relink()) +
		    " upon server SQUIT.");
    ::send(mynick, source, "SQUIT protection lasts " +
		    ToHumanTime(Parent->config.Squit_Protect()) + ".");
    ::send(mynick, source, "Users have " +
		    ToHumanTime(Parent->config.Squit_Cancel()) +
		    " to reconnect before SQUIT protection activates.");
    ::send(mynick, source, "Databases are saved every " +
		    ToHumanTime(Parent->config.Cycletime()) +
		    " and sync in " + Parent->events.SyncTime());
    ::send(mynick, source, "HyperActive cycle is " +
		    ToHumanTime(Parent->config.Checktime()) +
		    " and lag check is " +
		    ToHumanTime(Parent->config.Ping_Frequency()) + ".");
}
    
void OperServ::do_settings_Nick(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Nick", (mynick, source, params));

    ::send(mynick, source, "Nicknames will expire after " +
			ToHumanTime(Parent->nickserv.Expire()) + ".");
    ::send(mynick, source, "Users have " +
			ToHumanTime(Parent->nickserv.Ident()) +
			" to identify.");
    ::send(mynick, source, "Nicknames are held for " +
			ToHumanTime(Parent->nickserv.Release()) +
			" on failure to ident.");
    ::send(mynick, source, "Users are killed if they fail to ident " +
			mstring(itoa(Parent->nickserv.Passfail())) +
			" times.");
    mstring output = "";

    if (Parent->nickserv.DEF_Protect())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_Protect())
	    output << IRC_Bold;
	output << "Kill Protect";
	if (Parent->nickserv.LCK_Protect())
	    output << IRC_Off;
    }

    if (Parent->nickserv.DEF_Secure())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_Secure())
	    output << IRC_Bold;
	output << "Secure";
	if (Parent->nickserv.LCK_Secure())
	    output << IRC_Off;
    }

    if (Parent->nickserv.DEF_NoExpire())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_NoExpire())
	    output << IRC_Bold;
	output << "No Expire";
	if (Parent->nickserv.LCK_NoExpire())
	    output << IRC_Off;
    }

    if (Parent->nickserv.DEF_NoMemo())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_NoMemo())
	    output << IRC_Bold;
	output << "Ignoring Memos";
	if (Parent->nickserv.LCK_NoMemo())
	    output << IRC_Off;
    }

    if (Parent->nickserv.DEF_Private())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_Private())
	    output << IRC_Bold;
	output << "Private";
	if (Parent->nickserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Parent->nickserv.DEF_PRIVMSG())
    {
	if (output != "")
	    output << ", ";
	if (Parent->nickserv.LCK_PRIVMSG())
	    output << IRC_Bold;
	output << "PRIVMSG";
	if (Parent->nickserv.LCK_PRIVMSG())
	    output << IRC_Off;
    }

    ::send(mynick, source, "Default options are: " + output);

    output = "";
    if (Parent->nickserv.LCK_Language())
	output << IRC_Bold;
    output << Parent->nickserv.DEF_Language();
    if (Parent->nickserv.LCK_Language())
	output << IRC_Off;    

    ::send(mynick, source, "Default language is: " + output);

    ::send(mynick, source, "Maximum picture size is " +
		    mstring(ltoa(Parent->nickserv.PicSize())) + " bytes.");
    ::send(mynick, source, "Allowable picture extensions are: " +
		    Parent->nickserv.PicExt());
    ::send(mynick, source, "Users may have a maximum of " +
		    mstring(itoa(Parent->memoserv.Files())) +
		    " files of up to " +
		    mstring(ltoa(Parent->memoserv.FileSize())) +
		    " bytes each.");
}


void OperServ::do_settings_Channel(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Channel", (mynick, source, params));

    ::send(mynick, source, "Channels will expire after " +
			ToHumanTime(Parent->chanserv.Expire()) + ".");
    ::send(mynick, source, "Users are killed if they fail to ident " +
			mstring(itoa(Parent->chanserv.Passfail())) +
			" times.");
    ::send(mynick, source, "Channels are kept for " +
			ToHumanTime(Parent->chanserv.ChanKeep()) +
			" after akick of last user.");

    mstring output = "";
    if (Parent->chanserv.LCK_Bantime())
	output << IRC_Bold;
    output << ToHumanTime(Parent->chanserv.DEF_Bantime());
    if (Parent->chanserv.LCK_Bantime())
	output << IRC_Off;    
    ::send(mynick, source, "Default ban time is: " + output);

    ::send(mynick, source, "Default MLOCK is \"" +
			Parent->chanserv.DEF_MLock() + "\" and locked is \"" +
			Parent->chanserv.LCK_MLock() + "\".");

    output = "";

    if (Parent->chanserv.DEF_Keeptopic())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Keeptopic())
	    output << IRC_Bold;
	output << "Keep Topic";
	if (Parent->chanserv.LCK_Keeptopic())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Topiclock())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Topiclock())
	    output << IRC_Bold;
	output << "Topic Lock";
	if (Parent->chanserv.LCK_Topiclock())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Private())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Private())
	    output << IRC_Bold;
	output << "Private";
	if (Parent->chanserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Secureops())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Secureops())
	    output << IRC_Bold;
	output << "Secure Ops";
	if (Parent->chanserv.LCK_Secureops())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Secure())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Secure())
	    output << IRC_Bold;
	output << "Secure";
	if (Parent->chanserv.LCK_Secure())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_NoExpire())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_NoExpire())
	    output << IRC_Bold;
	output << "NoExpire";
	if (Parent->chanserv.LCK_NoExpire())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Anarchy())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Anarchy())
	    output << IRC_Bold;
	output << "Anarchy";
	if (Parent->chanserv.LCK_Anarchy())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Restricted())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Restricted())
	    output << IRC_Bold;
	output << "Restricted";
	if (Parent->chanserv.LCK_Restricted())
	    output << IRC_Off;
    }

    if (Parent->chanserv.DEF_Join())
    {
	if (output != "")
	    output << ", ";
	if (Parent->chanserv.LCK_Join())
	    output << IRC_Bold;
	output << "Join";
	if (Parent->chanserv.LCK_Join())
	    output << IRC_Off;
    }

    ::send(mynick, source, "Default options are: " + output);

    output = "";
    if (Parent->chanserv.LCK_Revenge())
	output << IRC_Bold;
    output << Parent->chanserv.DEF_Revenge();
    if (Parent->chanserv.LCK_Revenge())
	output << IRC_Off;    
    ::send(mynick, source, "Default ban time is: " + output);

    ::send(mynick, source, "Minimum access level is " +
		    mstring(ltoa(Parent->chanserv.Level_Min())) +
		    " and maximum is " +
		    mstring(ltoa(Parent->chanserv.Level_Max())) + ".");
    ::send(mynick, source, "Channel news articles expire after " +
		    ToHumanTime(Parent->memoserv.News_Expire()) + ".");
}


void OperServ::do_settings_Other(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Other", (mynick, source, params));

    ::send(mynick, source, "Memos are InFlight for " +
			ToHumanTime(Parent->memoserv.InFlight()) + ".");
    ::send(mynick, source, "Default AKILL exipry time is " +
			ToHumanTime(Parent->operserv.Def_Expire()) + ".");
    ::send(mynick, source, "Maximum AKILL expire times (by committee):");
    ::send(mynick, source, "    " + Parent->commserv.SADMIN_Name() + ": " +
			ToHumanTime(Parent->operserv.Expire_SAdmin()));
    ::send(mynick, source, "    " + Parent->commserv.SOP_Name() + ": " +
			ToHumanTime(Parent->operserv.Expire_Sop()));
    ::send(mynick, source, "    " + Parent->commserv.ADMIN_Name() + ": " +
			ToHumanTime(Parent->operserv.Expire_Admin()));
    ::send(mynick, source, "    " + Parent->commserv.OPER_Name() + ": " +
			ToHumanTime(Parent->operserv.Expire_Oper()));
    ::send(mynick, source, "A user may have up to " +
			mstring(itoa(Parent->operserv.Clone_Limit())) +
			" clones per host, which can be overridden up to " +
			mstring(itoa(Parent->operserv.Max_Clone())) + ".");

    ::send(mynick, source, "Flood is triggered with " +
			mstring(itoa(Parent->operserv.Flood_Msgs())) + " messages in " +
			ToHumanTime(Parent->operserv.Flood_Time()) + ".");
    ::send(mynick, source, "Services remember old flood triggers for up to " +
			ToHumanTime(Parent->operserv.Ignore_Remove()) + " later.");
    ::send(mynick, source, "Ignore lasts for " +
			ToHumanTime(Parent->operserv.Ignore_Time()) +
			" and is permanent if triggered " +
			mstring(itoa(Parent->operserv.Ignore_Limit())) +
			" times.");
    mstring output = "";

    if (Parent->commserv.DEF_OpenMemos())
    {
	if (output != "")
	    output << ", ";
	if (Parent->commserv.LCK_OpenMemos())
	    output << IRC_Bold;
	output << "Open Memos";
	if (Parent->commserv.LCK_OpenMemos())
	    output << IRC_Off;
    }

    if (Parent->commserv.DEF_Private())
    {
	if (output != "")
	    output << ", ";
	if (Parent->commserv.LCK_Private())
	    output << IRC_Bold;
	output << "Private";
	if (Parent->commserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Parent->commserv.DEF_Secure())
    {
	if (output != "")
	    output << ", ";
	if (Parent->commserv.LCK_Secure())
	    output << IRC_Bold;
	output << "Secure";
	if (Parent->commserv.LCK_Secure())
	    output << IRC_Off;
    }
    ::send(mynick, source, "Default committee options are: " + output);
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

    if (host.Contains("!") || host.Contains("@"))
    {
	::send(mynick, source, "Host may not contain a \'!\' or \'@\' character.");
	return;
    }

    if (!amount.IsNumber() || amount.Contains(".") || atoi(amount.c_str()) <= 0)
    {
	::send(mynick, source, "Amount must be a posetive whole number.");
	return;
    }

    MLOCK(("OperServ", "Clone"));
    if (Parent->operserv.Clone_find(host))
    {
	mstring entry = Parent->operserv.Clone->Entry();
	Parent->operserv.Clone_erase();
	Parent->operserv.Clone_insert(entry, atoi(amount.c_str()), reason, source);
	::send(mynick, source, "Entry " + entry + " changed to " + amount + ".");
    }
    else
    {
	Parent->operserv.Clone_insert(host, atoi(amount.c_str()), reason, source);
	::send(mynick, source, "Entry " + host + " added with " + amount + ".");
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

    if (host.Contains("!") || host.Contains("@"))
    {
	::send(mynick, source, "Host may not contain a \'!\' or \'@\' character.");
	return;
    }

    MLOCK(("OperServ", "Clone"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int i, num = atoi(host.c_str());
	if (num <= 0 || num > Parent->operserv.Clone_size())
	{
	    ::send(mynick, source, "Entry number must be between 0 and " +
				    mstring(itoa(Parent->operserv.Clone_size())));
	    return;
	}

	for (i=1, Parent->operserv.Clone = Parent->operserv.Clone_begin();
		i < num && Parent->operserv.Clone != Parent->operserv.Clone_end();
		i++, Parent->operserv.Clone++) ;
	if (Parent->operserv.Clone != Parent->operserv.Clone_end())
	{
	    Parent->operserv.Clone_erase();
	    ::send(mynick, source, "Clone entry #" + host + " removed.");
	}
	else
	{
	    ::send(mynick, source, "Clone entry #" + host + " not found");
	}
    }
    else
    {
	if (Parent->operserv.Clone_find(host))
	{
	    Parent->operserv.Clone_erase();
	    ::send(mynick, source, "Clone entry matching " + host + " removed.");
	}
	else
	{
	    ::send(mynick, source, "No such clone entry matching " + host + ".");
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

	if (host.Contains("!") || host.Contains("@"))
	{
	    ::send(mynick, source, "Host may not contain a \'!\' or \'@\' character.");
	    return;
	}
    }

    if (Parent->operserv.Clone_size())
    {
	::send(mynick, source, "Clone Override list:");
    }
    else
    {
	::send(mynick, source, "Clone Override list is empty.");
	return;
    }
    unsigned int i=1;
    mstring output;
    MLOCK(("OperServ", "Clone"));
    for (Parent->operserv.Clone = Parent->operserv.Clone_begin();
		Parent->operserv.Clone != Parent->operserv.Clone_end();
		Parent->operserv.Clone++)
    {
	if (Parent->operserv.Clone->Entry().LowerCase().Matches(host))
	{
	    output.Format("%3d. %s (modified %s ago by %s)", i,
			    Parent->operserv.Clone->Entry().c_str(),
			    Parent->operserv.Clone->Last_Modify_Time().Ago().c_str(),
			    Parent->operserv.Clone->Last_Modifier().c_str());
	    ::send(mynick, source, output);
	    output = "";
	    output.Format("     [%4d] %s", Parent->operserv.Clone->Value().first,
			    Parent->operserv.Clone->Value().second.c_str());
	    ::send(mynick, source, output);
	    output = "";
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

    if ((Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	Parent->commserv.list[Parent->commserv.SADMIN_Name().UpperCase()].IsOn(source) &&
	time > Parent->operserv.Expire_SAdmin()) ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name().UpperCase()].IsOn(source) &&
	time > Parent->operserv.Expire_Sop()) ||
	(Parent->commserv.IsList(Parent->commserv.ADMIN_Name()) &&
	Parent->commserv.list[Parent->commserv.ADMIN_Name().UpperCase()].IsOn(source) &&
	time > Parent->operserv.Expire_Admin()) ||
	(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name().UpperCase()].IsOn(source) &&
	time > Parent->operserv.Expire_Oper()))
    {
	::send(mynick, source, "Akill time limit specified is too great for your privilage level.");
	return;
    }
	
    if (host.Contains("!"))
    {
	::send(mynick, source, "Host may not contain a \'!\' character.");
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
	::send(mynick, source, "Entry " + entry + " extended for another " +
							ltoa(time) + " seconds.");
    }
    else
    {
	Parent->operserv.Akill_insert(host, time, reason, source);
	::send(mynick, source, "Entry " + host + " added for " + ltoa(time) +
								    " seconds.");
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
	::send(mynick, source, "Host may not contain a \'!\' character.");
	return;
    }

    MLOCK(("OperServ", "Akill"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int i, num = atoi(host.c_str());
	if (num <= 0 || num > Parent->operserv.Akill_size())
	{
	    ::send(mynick, source, "Entry number must be between 0 and " +
				    mstring(itoa(Parent->operserv.Akill_size())));
	    return;
	}

	for (i=1, Parent->operserv.Akill = Parent->operserv.Akill_begin();
		i < num && Parent->operserv.Akill != Parent->operserv.Akill_end();
		i++, Parent->operserv.Akill++) ;
	if (Parent->operserv.Akill != Parent->operserv.Akill_end())
	{
	    Parent->operserv.Akill_erase();
	    ::send(mynick, source, "Akill entry #" + host + " removed.");
	}
	else
	{
	    ::send(mynick, source, "Entry #" + host + " is not found on AKILL list.");
	}
    }
    else
    {
	if (!host.Contains("@"))
	{
	    host.Prepend("*@");
	}

	if (Parent->operserv.Akill_find(host))
	{
	    Parent->operserv.Akill_erase();
	    ::send(mynick, source, "Akill entry matching " + host + " removed.");
	}
	else
	{
	    ::send(mynick, source, "No such akill entry matching " + host + ".");
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
	    ::send(mynick, source, "Host may not contain a \'!\' character.");
	    return;
	}
    }

    if (Parent->operserv.Akill_size())
    {
	::send(mynick, source, "AKILL list:");
    }
    else
    {
	::send(mynick, source, "AKILL list is empty.");
	return;
    }
    unsigned int i=1;
    mstring output;
    MLOCK(("OperServ", "Akill"));
    for (Parent->operserv.Akill = Parent->operserv.Akill_begin();
		Parent->operserv.Akill != Parent->operserv.Akill_end();
		Parent->operserv.Akill++)
    {
	if (Parent->operserv.Akill->Entry().LowerCase().Matches(host))
	{
	    output.Format("%3d. %s (modified %s ago by %s)", i,
			    Parent->operserv.Akill->Entry().c_str(),
			    Parent->operserv.Akill->Last_Modify_Time().Ago().c_str(),
			    Parent->operserv.Akill->Last_Modifier().c_str());
	    ::send(mynick, source, output);
	    output = "";
	    output.Format("     [%8l] %s", Parent->operserv.Akill->Value().first,
			    Parent->operserv.Akill->Value().second.c_str());
	    ::send(mynick, source, output);
	    output = "";
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
	    ::send(mynick, source, "Nickname " + host + " is not online.");
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
    ::send(mynick, source, "Entry " + host + " added to OperDeny list.");
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
	    ::send(mynick, source, "Entry number must be between 0 and " +
				    mstring(itoa(Parent->operserv.OperDeny_size())));
	    return;
	}

	for (i=1, Parent->operserv.OperDeny = Parent->operserv.OperDeny_begin();
		i < num && Parent->operserv.OperDeny != Parent->operserv.OperDeny_end();
		i++, Parent->operserv.OperDeny++) ;
	if (Parent->operserv.OperDeny != Parent->operserv.OperDeny_end())
	{
	    Parent->operserv.OperDeny_erase();
	    ::send(mynick, source, "OperDeny entry #" + host + " removed.");
	}
	else
	{
	    ::send(mynick, source, "Entry #" + host + " not found on OperDeny");
	}
    }
    else
    {
	if (!host.Contains("@"))
	{
	    if (!Parent->nickserv.IsLive(host))
	    {
		::send(mynick, source, "Nickname " + host + " is not online.");
		return;
	    }
	    host = Parent->nickserv.live[host.LowerCase()].Mask(Parent->operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.Prepend("*!");
	}

	if (Parent->operserv.OperDeny_find(host))
	{
	    Parent->operserv.OperDeny_erase();
	    ::send(mynick, source, "OperDeny entry matching " + host + " removed.");
	}
	else
	{
	    ::send(mynick, source, "No such operdeny entry matching " + host + ".");
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
		::send(mynick, source, "Nickname " + host + " is not online.");
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
	::send(mynick, source, "OperDeny list:");
    }
    else
    {
	::send(mynick, source, "OperDeny list is empty.");
	return;
    }
    unsigned int i=1;
    mstring output;
    MLOCK(("OperServ", "OperDeny"));
    for (Parent->operserv.OperDeny = Parent->operserv.OperDeny_begin();
		Parent->operserv.OperDeny != Parent->operserv.OperDeny_end();
		Parent->operserv.OperDeny++)
    {
	if (Parent->operserv.OperDeny->Entry().LowerCase().Matches(host))
	{
	    output.Format("%3d. %s (modified %s ago by %s)", i,
			    Parent->operserv.OperDeny->Entry().c_str(),
			    Parent->operserv.OperDeny->Last_Modify_Time().Ago().c_str(),
			    Parent->operserv.OperDeny->Last_Modifier().c_str());
	    ::send(mynick, source, output);
	    output = "";
	    output.Format("     %s", Parent->operserv.OperDeny->Value().c_str());
	    ::send(mynick, source, output);
	    output = "";
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
	    ::send(mynick, source, "Nickname " + host + " is not online.");
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
    ::send(mynick, source, "Entry " + host + " added to Ignore list.");
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
	    ::send(mynick, source, "Entry number must be between 0 and " +
				    mstring(itoa(Parent->operserv.Ignore_size())));
	    return;
	}

	for (i=1, Parent->operserv.Ignore = Parent->operserv.Ignore_begin();
		i < num && Parent->operserv.Ignore != Parent->operserv.Ignore_end();
		i++, Parent->operserv.Ignore++) ;
	if (Parent->operserv.Ignore != Parent->operserv.Ignore_end())
	{
	    Parent->operserv.Ignore_erase();
	    ::send(mynick, source, "Ignore entry #" + host + " removed.");
	}
	else
	{
	    ::send(mynick, source, "Could not find entry #" + host);
	}
    }
    else
    {
	if (!host.Contains("@"))
	{
	    if (!Parent->nickserv.IsLive(host))
	    {
		::send(mynick, source, "Nickname " + host + " is not online.");
		return;
	    }
	    host = Parent->nickserv.live[host.LowerCase()].Mask(Parent->operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.Prepend("*!");
	}

	if (Parent->operserv.Ignore_find(host))
	{
	    Parent->operserv.Ignore_erase();
	    ::send(mynick, source, "Ignore entry matching " + host + " removed.");
	}
	else
	{
	    ::send(mynick, source, "No such ignore entry matching " + host + ".");
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
		::send(mynick, source, "Nickname " + host + " is not online.");
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
    mstring output;
    MLOCK(("OperServ", "Ignore"));
    for (Parent->operserv.Ignore = Parent->operserv.Ignore_begin();
		Parent->operserv.Ignore != Parent->operserv.Ignore_end();
		Parent->operserv.Ignore++)
    {
	if (Parent->operserv.Ignore->Entry().LowerCase().Matches(host) &&
	    Parent->operserv.Ignore->Value())
	{
	    if (head == false)
		::send(mynick, source, "Services permanent ignore list:");
	    output.Format("%3d. %s (modified %s ago by %s)", i,
			    Parent->operserv.Ignore->Entry().c_str(),
			    Parent->operserv.Ignore->Last_Modify_Time().Ago().c_str(),
			    Parent->operserv.Ignore->Last_Modifier().c_str());
	    ::send(mynick, source, output);
	    output = "";
	    i++;
	}
    }
    if (head == false)
	::send(mynick, source, "Services permanent ignore list is empty.");
}

void OperServ::load_database(wxInputStream& in)
{
}

void OperServ::save_database(wxOutputStream& out)
{
}
