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


bool OperServ::Akill_insert(mstring entry, long value, mstring reason, mstring nick)
{
    FT("OperServ::Akill_insert", (entry, value, reason, nick));

    // no @
    if (!entry.Contains("@") || entry.Contains("!"))
    {
	    RET(false);
    }

    if (!Akill_find(entry))
    {
	entlist_val_t<pair<long, mstring> > tmp(entry, pair<long, mstring>(value, reason), nick);
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
    set<entlist_val_t<pair<long, mstring> > >::iterator iter = i_Akill.end();
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


pair<long,mstring> OperServ::Akill_value(mstring entry)
{
    FT("OperServ::Akill_value", (entry));

    pair<long,mstring> retval = pair<long,mstring>(0,"");
    {
    MLOCK("OperServ", "Akill");
//  entlist_val_ui<pair<long, mstring> > iter = Akill;
    set<entlist_val_t<pair<long, mstring> > >::iterator iter = Akill;

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
    MLOCK("OperServ", "OperDeny");
//  entlist_val_ui<mstring> iter = OperDeny;
    set<entlist_val_t<mstring> >::iterator iter = OperDeny;

    if (OperDeny_find(entry))
	retval=OperDeny->Value();
    OperDeny = iter;
    }
    return retval;
}



bool OperServ::Ignore_insert(mstring entry, mDateTime value, bool perm, mstring nick)
{
    FT("OperServ::Ignore_insert", (entry, value, perm, nick));

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
	entlist_val_t<pair<mDateTime, bool> > tmp(entry, pair<mDateTime, bool>(value, perm), nick);
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
    set<entlist_val_t<pair<mDateTime, bool> > >::iterator iter = i_Ignore.end();

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


pair<mDateTime,bool> OperServ::Ignore_value(mstring entry)
{
    FT("OperServ::Ignore_value", (entry));

    pair<mDateTime,bool> retval = pair<mDateTime,bool>((long) 0,"");
    {
    MLOCK("OperServ", "Ignore");
//  entlist_val_ui<pair<mDateTime, bool> > iter = Ignore;
    set<entlist_val_t<pair<mDateTime, bool> > >::iterator iter = Ignore;

    if (Ignore_find(entry))
	retval=Ignore->Value();
    Ignore = iter;
    }
    return retval;
}





void OperServ::DoBreakdown(mstring mynick, mstring source, mstring previndent, mstring server)
{
    FT("OperServ::DoDownlinks", (mynick, source, previndent, server));
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
		Parent->server.NOTICE(mynick, source, out);
		DoBreakdown(mynick, source, previndent + "| ", downlinks[i]);
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
		"SERVER                                         LAG  USERS (OPS)");
	mstring out;
 	unsigned int users = 0, opers = 0;
	float lag;

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
	Parent->server.NOTICE(mynick, source, out);

	DoBreakdown(mynick, source, "", Parent->startup.Server_Name());
    }

    mThread::ReAttach(tt_mBase);
}

void OperServ::load_database(wxInputStream& in)
{
}

void OperServ::save_database(wxOutputStream& out)
{
}
