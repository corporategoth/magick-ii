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
}

void ServMsg::do_BreakDown(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_BreakDown", (mynick, source, params));

    ::send(mynick, source,
	"SERVER                                         LAG  USERS (OPS).");
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
    out.Format("%-40s    0.000s  %5d (%3d)  %3.2f%%",
	    Parent->startup.Server_Name().LowerCase().c_str(), users, opers,
	    ((float) users / (float) Parent->nickserv.live.size()) * 100.0);
    ::send(mynick, source, out);

    do_BreakDown2(mynick, source, "", Parent->startup.Server_Name());
}

void ServMsg::do_BreakDown2(mstring mynick, mstring source, mstring previndent, mstring server)
{
    FT("ServMsg::do_BreakDown2", (mynick, source, previndent, server));
    vector<mstring> downlinks;
    mstring out;
    unsigned int users, opers;
    float lag;

    if (server.LowerCase() == Parent->startup.Server_Name().LowerCase())
    {
	if (Parent->server.OurUplink())
	    downlinks.push_back(Parent->server.OurUplink());
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
		if (lag < 10.0)
		    out.Format("%-40s    %1.3fs  %5d (%3d)  %3.2f%%",
			(previndent + "|-" + downlinks[i]).c_str(), lag, users, opers,
			((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		else if (lag < 100.0)
		    out.Format("%-40s   %2.3fs  %5d (%3d)  %3.2f%%",
			(previndent + "|-" + downlinks[i]).c_str(), lag, users, opers,
			((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		else
		    out.Format("%-40s  %3.3fs  %5d (%3d)  %3.2f%%",
			(previndent + "|-" + downlinks[i]).c_str(), lag, users, opers,
			((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		::send(mynick, source, out);
		do_BreakDown2(mynick, source, previndent + "| ", downlinks[i]);
	    }
	    else
	    {
		if (lag < 10.0)
		    out.Format("%-40s    %1.3fs  %5d (%3d)  %3.2f%%",
			(previndent + "`-" + downlinks[i]).c_str(), lag, users, opers,
			((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		else if (lag < 100.0)
		    out.Format("%-40s   %2.3fs  %5d (%3d)  %3.2f%%",
			(previndent + "`-" + downlinks[i]).c_str(), lag, users, opers,
			((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		else
		    out.Format("%-40s  %3.3fs  %5d (%3d)  %3.2f%%",
			(previndent + "`-" + downlinks[i]).c_str(), lag, users, opers,
			((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		::send(mynick, source, out);
		do_BreakDown2(mynick, source, previndent + "  ", downlinks[i]);
	    }
	}
    }
}


void ServMsg::AddCommands()
{
    NFT("ServMsg::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
		    "BREAKD*", "ALL", ServMsg::do_BreakDown);
    Parent->commands.AddSystemCommand(GetInternalName(),
		    "*MAP", "ALL", ServMsg::do_BreakDown);
}

void ServMsg::RemCommands()
{
    NFT("ServMsg::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.RemSystemCommand(GetInternalName(),
		    "BREAKD*", "ALL");
    Parent->commands.RemSystemCommand(GetInternalName(),
		    "*MAP", "ALL");
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

void ServMsg::load_database(wxInputStream& in)
{
}

void ServMsg::save_database(wxOutputStream& out)
{
}
