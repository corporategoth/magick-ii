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
#include "servmsg.h"
#include "magick.h"

ServMsg::ServMsg()
{
    NFT("ServMsg::ServMsg");
    messages = true;
}


void ServMsg::AddCommands()
{
    NFT("ServMsg::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "HELP", Parent->commserv.ADMIN_Name(), ServMsg::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "BREAKD*", Parent->commserv.ALL_Name(), ServMsg::do_BreakDown);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*MAP", Parent->commserv.ALL_Name(), ServMsg::do_BreakDown);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GLOB*", Parent->commserv.ADMIN_Name(), ServMsg::do_Global);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "STAT*", Parent->commserv.OPER_Name(), ServMsg::do_Stats);
}

void ServMsg::RemCommands()
{
    NFT("ServMsg::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.
}

void ServMsg::execute(const mstring & data)
{
    mThread::ReAttach(tt_OtherServ);
    FT("ServMsg::execute", (data));
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

void ServMsg::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_Help", (mynick, source, params));
}


void ServMsg::do_BreakDown(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_BreakDown", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    ::send(mynick, source, Parent->getMessage(source, "MISC/BREAKDOWN_HEAD"));
    mstring out;
    unsigned int users = 0, opers = 0;

    map<mstring,Nick_Live_t>::iterator k;
    for (k=Parent->nickserv.live.begin(); k!=Parent->nickserv.live.end(); k++)
    {
	if (k->second.IsServices() && k->second.Name() != "")
	{
	    users++;
	    if (k->second.HasMode("o"))
		opers++;
	}
    }
    ::send(mynick, source, Parent->getMessage(source, "MISC/BREAKDOWN"),
	    Parent->startup.Server_Name().LowerCase().c_str(), 0.0, users, opers,
	    ((float) users / (float) Parent->nickserv.live.size()) * 100.0);
    do_BreakDown2(mynick, source, "", "");
}

void ServMsg::do_BreakDown2(mstring mynick, mstring source, mstring previndent, mstring server)
{
    FT("ServMsg::do_BreakDown2", (mynick, source, previndent, server));
    vector<mstring> downlinks;
    mstring out;
    unsigned int users, opers;
    float lag;

    if (server == "")
    {
	map<mstring, Server>::iterator iter;
	for (iter = Parent->server.ServerList.begin();
		iter != Parent->server.ServerList.end(); iter++)
	{
	    if (iter->second.Name() != "" &&
		iter->second.Uplink() == Parent->startup.Server_Name().LowerCase())
		downlinks.push_back(iter->first);
	}
    }
    else
    {
	if (Parent->server.IsServer(server))
	    downlinks = Parent->server.ServerList[server].Downlinks();
    }

    for (int i=0; i<downlinks.size(); i++)
    {
	if (Parent->server.IsServer(downlinks[i]))
	{
	    users = Parent->server.ServerList[downlinks[i]].Users();
	    opers = Parent->server.ServerList[downlinks[i]].Opers();
	    lag = Parent->server.ServerList[downlinks[i]].Lag();
	    if (i<downlinks.size()-1)
	    {
		::send(mynick, source, Parent->getMessage(source, "MISC/BREAKDOWN"),
			(previndent + "|-" + downlinks[i]).c_str(), lag, users, opers,
			((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		do_BreakDown2(mynick, source, previndent + "| ", downlinks[i]);
	    }
	    else
	    {
		::send(mynick, source, Parent->getMessage(source, "MISC/BREAKDOWN"),
			(previndent + "`-" + downlinks[i]).c_str(), lag, users, opers,
			((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		do_BreakDown2(mynick, source, previndent + "  ", downlinks[i]);
	    }
	}
    }
}


void ServMsg::do_Stats(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_Stats", (mynick, source, params));

    ::send(mynick, source, Parent->getMessage(source, "STATS/NS_LIVE"),
			Parent->nickserv.live.size(),
			Parent->nickserv.live.size() * sizeof(Nick_Live_t) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/CS_LIVE"),
			Parent->chanserv.live.size(),
			Parent->chanserv.live.size() * sizeof(Chan_Live_t) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/NS_STORED"),
			Parent->nickserv.stored.size(),
			Parent->nickserv.stored.size() * sizeof(Nick_Stored_t) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/CS_STORED"),
			Parent->chanserv.stored.size(),
			Parent->chanserv.stored.size() * sizeof(Chan_Stored_t) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/MEMO"),
			Parent->memoserv.nick.size(),
			Parent->memoserv.nick.size() * sizeof(Memo_t) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/NEWS"),
			Parent->memoserv.channel.size(),
			Parent->memoserv.channel.size() * sizeof(News_t) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/COMMITTEE"),
			Parent->commserv.list.size(),
			Parent->commserv.list.size() * sizeof(Committee) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/OPERSERV"),
			Parent->operserv.Clone_size() +
			Parent->operserv.Akill_size() +
			Parent->operserv.OperDeny_size() +
			Parent->operserv.Ignore_size(),
			Parent->operserv.Clone_size() * sizeof(*Parent->operserv.Clone) / 1024 +
			Parent->operserv.Akill_size() * sizeof(*Parent->operserv.Akill) / 1024 +
			Parent->operserv.OperDeny_size() * sizeof(*Parent->operserv.OperDeny) / 1024 +
			Parent->operserv.Ignore_size() * sizeof(*Parent->operserv.Ignore) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTHER"),
			Parent->server.ServerList.size(),
			Parent->server.ServerList.size() * sizeof(Server) / 1024);
}


void ServMsg::do_Global(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_Global", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }
    mstring text = params.After(" ");

    map<mstring, Server>::iterator iter;
    for (iter=Parent->server.ServerList.begin();
			iter != Parent->server.ServerList.end(); iter++)
    {
	Parent->server.NOTICE(Parent->servmsg.FirstName(), "$" +
						    iter->first, text);
    }
    announce(mynick, Parent->getMessage(source, "MISC/GLOBAL_MSG"),
				source.c_str());
}


void ServMsg::load_database(wxInputStream& in)
{
    FT("ServMsg::load_database", ("(wxInputStream &) in"));
}

void ServMsg::save_database(wxOutputStream& out)
{
    FT("ServMsg::save_database", ("(wxOutputStream &) out"));
}
