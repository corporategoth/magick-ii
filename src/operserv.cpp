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

OperServ::OperServ()
{
}

void OperServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_OperServ);
    FT("OperServ::execute", (data));
    //okay this is the main nickserv command switcher


    // Nick/Server PRIVMSG/NOTICE mynick :message

    mstring source, msgtype, mynick, message;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ").UpperCase();
    mynick  = data.ExtractWord(3, ": ");
    message = data.After(":", 2);

    if (message.UpperCase()=="BREAKDOWN")
    {

	Parent->server.NOTICE(mynick, source,
		"SERVER                                       LAG  USERS (OPS)");
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

	out.Format("%-40s  %3.3fs  %5d (%3d)  %3.2f%%",
		Parent->startup.Server_Name().LowerCase().c_str(),
		0.0, users, opers, ((float) users / (float) Parent->nickserv.live.size()) * 100.0);
	Parent->server.NOTICE(mynick, source, out);
	if (Parent->server.IsServer(Parent->server.OurUplink()))
	{
	    users = Parent->server.ServerList[Parent->server.OurUplink()].Users();
	    opers = Parent->server.ServerList[Parent->server.OurUplink()].Opers();
	    out.Format("%-40s  %3.3fs  %5d (%3d)  %3.2f%%",
		("`-" + Parent->server.OurUplink()).c_str(),
		Parent->server.ServerList[Parent->server.OurUplink()].Lag(),
		users, opers, ((float) users / (float) Parent->nickserv.live.size()) * 100.0);
	    Parent->server.NOTICE(mynick, source, out);
	    DoBreakdown(mynick, source, "  ", Parent->server.OurUplink());
	}
    }

    mThread::ReAttach(tt_mBase);

}

void OperServ::DoBreakdown(mstring mynick, mstring source, mstring previndent, mstring server)
{
    FT("OperServ::DoDownlinks", (mynick, source, previndent, server));
    vector<mstring> downlinks = Parent->server.ServerList[server].Downlinks();
    mstring out;
    unsigned int users, opers;
    for (int i=0; i<downlinks.size(); i++)
    {
	if (Parent->server.IsServer(downlinks[i]))
	{
	    users = Parent->server.ServerList[downlinks[i]].Users();
	    opers = Parent->server.ServerList[downlinks[i]].Opers();
	    if (i<downlinks.size()-1)
	    {
		out.Format("%-40s  %3.3fs  %5d (%3d)  %3.2f%%",
			(previndent + "|-" + downlinks[i]).c_str(),
			Parent->server.ServerList[downlinks[i]].Lag(),
			users, opers, ((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		Parent->server.NOTICE(mynick, source, out);
		DoBreakdown(mynick, source, previndent + "| ", downlinks[i]);
	    }
	    else
	    {
		out.Format("%-40s  %3.3fs  %5d (%3d)  %3.2f%%",
			(previndent + "`-" + downlinks[i]).c_str(),
			Parent->server.ServerList[downlinks[i]].Lag(),
			users, opers, ((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		Parent->server.NOTICE(mynick, source, out);
		DoBreakdown(mynick, source, previndent + "  ", downlinks[i]);
	    }
	}
    }
}


void OperServ::load_database(wxInputStream& in)
{
}

void OperServ::save_database(wxOutputStream& out)
{
}
