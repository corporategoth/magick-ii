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
	entlist_val_t<pair<unsigned int, mstring> > tmp(entry, pair<unsigned int, mstring>(value, reason), nick);
	Clone = i_Clone.insert(i_Clone.end(), tmp);
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
	entlist_val_t<pair<unsigned long, mstring> > tmp(entry, pair<unsigned long, mstring>(value, reason), nick);
	Akill = i_Akill.insert(i_Akill.end(), tmp);
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
	entlist_val_t<mstring> tmp(entry, value, nick);
	OperDeny = i_OperDeny.insert(i_OperDeny.end(), tmp);
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
	entlist_val_t<bool> tmp(entry, perm, nick);
	Ignore = i_Ignore.insert(i_Ignore.end(), tmp);
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
}

void OperServ::AddCommands()
{
    NFT("OperServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "HELP", Parent->commserv.SADMIN_Name(), OperServ::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "TRACE", Parent->commserv.SADMIN_Name(), OperServ::do_Trace);
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
	::send(mynick, source, "Not enough paramaters");
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


void OperServ::do_settings_Config(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Config", (mynick, source, params));
/*
CONFIG:
    Base level is ?, Current level is ?.
    Level is increased if lag is more than ? seconds.
-   Databases are (not) encrypted, and compressed at level ?.
    Services will re-link in ? seconds upon server SQUIT.
    Squit protection lasts ? seconds.  Nicknames have ? seconds
	to rejoin before squit protection activates.
    Services have ? server(s) available to connect to.
    Databases are saved every ? seconds, and sync in ...?
    HyperActive cycle is ? seconds, and lag check is ? seconds.
-   Minimum threads active is ?, Current threads active is ?.
-   New thread will spawn each ? messages, and die when below ?.
*/
}
    
void OperServ::do_settings_Nick(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Nick", (mynick, source, params));

/*
NICK:
    Nicknames will expire after ?.
    Users have ? seconds to identify.
    Nicks are held for ? seconds on ident failure.
    Users are killed if they fail to ident ? times.
    Default options are: ...
    Maximim picture size is ? bytes.
    Allowable picture extensions are: ?
    Users may have a maximum of ? file attachments, of up to ? bytes each.
*/
}


void OperServ::do_settings_Channel(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Channel", (mynick, source, params));

/*
CHAN:
    Channels will expire after ?.
    Users are killed if they fail to chan ident ? times.
    Channels are kept for ? seconds after akick of last user.
    Default ban time is ? seconds, Default MLOCK is: ?.
    Default options are: ...
    Default revenge level is: 
    Minimum ACCESS level is ?, Maximum is ?
    Channel news articles expire after ?.
*/
}


void OperServ::do_settings_Other(mstring mynick, mstring source, mstring params)
{
    FT("OperServ::do_settings_Other", (mynick, source, params));

/*
OTHER:
    Memos are InFlight for ? seconds.
    Default AKILL expire time is ?.
    Maximums are (by committee):
	x: ?
	x: ?
	x: ?
	x: ?
    A user may have up to ? clones per host, or overridden to up to ?.
    Flood is triggered with ? messages in ? seconds.
    Services remember old flood for triggers up to ? seconds later.
    Ignore lasts ? seconds, and is perminant if triggered more than ? times.
    Default committee options are: ...
*/
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
	::send(mynick, source, "Not enough paramaters");
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
	::send(mynick, source, "Not enough paramaters");
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
	    ::send(mynick, source, "Clone entry #" + mstring(itoa(num)) + " removed.");
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
	::send(mynick, source, "Not enough paramaters");
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
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring host   = params.ExtractWord(3, " ").LowerCase();
    mstring reason = params.After(" ", 3);
    unsigned long time = Parent->operserv.Def_Expire();

    if (FromHumanTime(reason.Before(" ")))
    {
	if (params.WordCount(" ") < 5)
	{
	    ::send(mynick, source, "Not enough paramaters");
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
	::send(mynick, source, "Not enough paramaters");
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
	    ::send(mynick, source, "Akill entry #" + mstring(itoa(num)) + " removed.");
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
	::send(mynick, source, "Not enough paramaters");
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
	::send(mynick, source, "Not enough paramaters");
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
	::send(mynick, source, "Not enough paramaters");
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
	    ::send(mynick, source, "OperDeny entry #" + mstring(itoa(num)) + " removed.");
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
	::send(mynick, source, "Not enough paramaters");
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
	::send(mynick, source, "Not enough paramaters");
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
	::send(mynick, source, "Not enough paramaters");
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
	    ::send(mynick, source, "Ignore entry #" + mstring(itoa(num)) + " removed.");
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
	::send(mynick, source, "Not enough paramaters");
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
    mstring output;
    MLOCK(("OperServ", "OperDeny"));
    for (Parent->operserv.OperDeny = Parent->operserv.OperDeny_begin();
		Parent->operserv.OperDeny != Parent->operserv.OperDeny_end();
		Parent->operserv.OperDeny++)
    {
	if (Parent->operserv.OperDeny->Entry().LowerCase().Matches(host) &&
	    Parent->operserv.OperDeny->Value())
	{
	    output.Format("%3d. %s (modified %s ago by %s)", i,
			    Parent->operserv.OperDeny->Entry().c_str(),
			    Parent->operserv.OperDeny->Last_Modify_Time().Ago().c_str(),
			    Parent->operserv.OperDeny->Last_Modifier().c_str());
	    ::send(mynick, source, output);
	    output = "";
	    i++;
	}
    }
}

void OperServ::load_database(wxInputStream& in)
{
}

void OperServ::save_database(wxOutputStream& out)
{
}
