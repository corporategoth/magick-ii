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

    CloneList[host.LowerCase()]++;

    {
    MLOCK("OperServ","Clone");
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


bool OperServ::Clone_insert(mstring entry, int value, mstring reason, mstring nick)
{
    FT("OperServ::Clone_insert", (entry, value, reason, nick));

    // Wildcards but no @
    if (entry.Contains("@") || entry.Contains("!"))
    {
	    RET(false);
    }

    if (!Clone_find(entry))
    {
	entlist_val_t<pair<int, mstring> > tmp(entry, pair<int, mstring>(value, reason), nick);
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
    set<entlist_val_t<pair<int, mstring> > >::iterator iter = i_Clone.end();
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


pair<int,mstring> OperServ::Clone_value(mstring entry)
{
    FT("OperServ::Clone_value", (entry));

    pair<int,mstring> retval = pair<int,mstring>(0,"");
    {
    MLOCK("OperServ", "Clone");
//  entlist_val_ui<pair<int, mstring> > iter = Clone;
    set<entlist_val_t<pair<int, mstring> > >::iterator iter = Clone;

    if (Clone_find(entry))
	retval=Clone->Value();
    Clone = iter;
    }
    return retval;
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

void OperServ::load_database(wxInputStream& in)
{
}

void OperServ::save_database(wxOutputStream& out)
{
}
