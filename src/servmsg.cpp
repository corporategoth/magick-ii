#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_servmsg_cpp_ ## x () { return y; }
RCSID(servmsg_cpp, "@(#)$Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "magick.h"
#include "dccengine.h"

ServMsg::ServMsg()
{
    BTCB();
    NFT("ServMsg::ServMsg");
    messages = true;
    ETCB();
}

void ServMsg::AddCommands()
{
    BTCB();
    NFT("ServMsg::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "STAT* NICK*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), ServMsg::do_stats_Nick);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "STAT* CHAN*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), ServMsg::do_stats_Channel);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "STAT* OPER*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), ServMsg::do_stats_Oper);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "STAT* OTH*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), ServMsg::do_stats_Other);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "STAT* USAGE",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), ServMsg::do_stats_Usage);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "STAT* ALL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), ServMsg::do_stats_All);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "STAT* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* LIST*", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_file_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* VIEW*", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_file_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* ADD*", Magick::instance().commserv.SOP_Name(),
						 ServMsg::do_file_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* DEL*", Magick::instance().commserv.SOP_Name(),
						 ServMsg::do_file_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* REM*", Magick::instance().commserv.SOP_Name(),
						 ServMsg::do_file_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* REN*", Magick::instance().commserv.SOP_Name(),
						 ServMsg::do_file_Rename);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* PRIV*", Magick::instance().commserv.SOP_Name(),
						 ServMsg::do_file_Priv);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* SEC*", Magick::instance().commserv.SOP_Name(),
						 ServMsg::do_file_Priv);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* SEND*", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_file_Send);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* DCC*", Magick::instance().commserv.SOP_Name(),
						 ServMsg::do_file_Dcc);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* KILL*", Magick::instance().commserv.SOP_Name(),
						 ServMsg::do_file_Cancel);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* CAN*", Magick::instance().commserv.SOP_Name(),
						 ServMsg::do_file_Cancel);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* LOOK*", Magick::instance().commserv.SADMIN_Name(),
						 ServMsg::do_file_Lookup);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* RES*", Magick::instance().commserv.SADMIN_Name(),
						 ServMsg::do_file_Lookup);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* H*LP", Magick::instance().commserv.ALL_Name(),
						 do_2param);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ASK*", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_Ask);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "QUEST*", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_Ask);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*LP*OP*", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_Ask);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_Help);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CRED*", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_Credits);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CONTRIB*", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_Contrib);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LANG*", Magick::instance().commserv.REGD_Name(),
						 ServMsg::do_Languages);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "BREAKD*", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_BreakDown);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "*MAP", Magick::instance().commserv.ALL_Name(),
						 ServMsg::do_BreakDown);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GLOB*", Magick::instance().commserv.ADMIN_Name(),
						 ServMsg::do_Global);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "STAT* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "STAT*", Magick::instance().commserv.REGD_Name(),
						 do_Stats);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE* *", Magick::instance().commserv.ALL_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE*", Magick::instance().commserv.ALL_Name(),
						 do_1_2param);
    ETCB();
}

void ServMsg::RemCommands()
{
    BTCB();
    NFT("ServMsg::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "STAT* NICK*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "STAT* CHAN*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "STAT* OPER*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "STAT* OTH*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "STAT* USAGE",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "STAT* ALL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "STAT* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* LIST*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* VIEW*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* ADD*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* DEL*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* REM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* REN*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* SEND*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* LOOK*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* RES*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* H*LP", Magick::instance().commserv.ALL_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ASK*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "QUEST*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*LP*OP*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CRED*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CONTRIB*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LANG*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "BREAKD*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "*MAP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GLOB*", Magick::instance().commserv.ADMIN_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "STAT* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "STAT*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE* *", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE*", Magick::instance().commserv.ALL_Name());
    ETCB();
}

void ServMsg::execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    mThread::ReAttach(tt_OtherServ);
    FT("ServMsg::execute", (source, msgtype, params));
    //okay this is the main servmsg command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message
    mstring mynick(Magick::instance().getLname(params.ExtractWord(1, ": ")));
    mstring message(params.After(":"));
    mstring command(message.Before(" "));

    if (message[0U] == CTCP_DELIM_CHAR)
    {
	if (msgtype == "PRIVMSG")
	    DccEngine::decodeRequest(mynick, source, message);
	else
	    DccEngine::decodeReply(mynick, source, message);
    }
    else if (msgtype == "PRIVMSG" && !Magick::instance().commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);

    ETCB();
}

void ServMsg::do_Help(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_Help", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    mstring HelpTopic = Magick::instance().servmsg.GetInternalName();

    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector < mstring > help = Magick::instance().getHelp(source, HelpTopic.UpperCase());

    unsigned int i;

    for (i = 0; i < help.size(); i++)
	::send(mynick, source, help[i]);
    ETCB();
}

void ServMsg::do_Credits(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_Credits", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    Magick::instance().servmsg.stats.i_Credits++;
    for (int i = 0; credits[i] != "---EOM---"; i++)
	if (credits[i].length())
	    ::sendV(mynick, source, credits[i], mynick.c_str());
	else
	    ::send(mynick, source, " ");
    ETCB();
}

void ServMsg::do_Contrib(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_Contrib", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    Magick::instance().servmsg.stats.i_Credits++;
    for (int i = 0; contrib[i] != "---EOM---"; i++)
	if (contrib[i].length())
	    ::sendV(mynick, source, contrib[i], mynick.c_str());
	else
	    ::send(mynick, source, " ");
    ETCB();
}

void ServMsg::do_Languages(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_Languages", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    set < mstring > langs = mFile::DirList(Magick::instance().files.Langdir(), "*.lng");
    mstring output, val;

    if (langs.size())
    {
	NSEND(mynick, source, "MISC/LANG_LIST");
	set < mstring >::iterator i;
	for (i = langs.begin(); i != langs.end(); i++)
	{
	    if (output.length() > Magick::instance().server.proto.MaxLine())
	    {
		::send(mynick, source, "    " + output);
		output.erase();
	    }
	    val = * i;
	    val.Truncate(val.Find(".", true));
	    if (output.length())
		output += ", ";
	    if (val.UpperCase() == Magick::instance().nickserv.DEF_Language())
		output += IRC_Bold;
	    output += val.UpperCase();
	    if (val.UpperCase() == Magick::instance().nickserv.DEF_Language())
		output += IRC_Off;
	}
	if (output.length())
	    ::send(mynick, source, "    " + output);
    }
    else
    {
	NSEND(mynick, source, "MISC/LANG_NOLIST");
    }
    ETCB();
}

void ServMsg::do_BreakDown(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_BreakDown", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    NSEND(mynick, source, "MISC/BREAKDOWN_HEAD");
    mstring out;

    map < mstring, pair < unsigned int, unsigned int > > ServCounts;

    NickServ::live_t::iterator k;
    {
	RLOCK((lck_NickServ, lck_live));
	for (k = Magick::instance().nickserv.LiveBegin(); k != Magick::instance().nickserv.LiveEnd(); k++)
	{
	    map_entry < Nick_Live_t > nlive(k->second);
	    if (ServCounts.find(nlive->Server().LowerCase()) == ServCounts.end())
	    {
		ServCounts[nlive->Server()] = pair < unsigned int, unsigned int > (0, 0);
	    }
	    if (!nlive->Name().empty())
	    {
		ServCounts[nlive->Server()].first++;
		if (nlive->HasMode("o"))
		    ServCounts[nlive->Server()].second++;
	    }
	}
    }
    ::sendV(mynick, source, "%-35s  % 3.3fs  %5d (%3d)  %3.2f%%", Magick::instance().startup.Server_Name().LowerCase().c_str(),
	    0.0, ServCounts[""].first, ServCounts[""].second,
	    100.0 * static_cast < float > (ServCounts[""].first) / static_cast < float >
	    (Magick::instance().nickserv.LiveSize()));
    do_BreakDown2(ServCounts, mynick, source, "", "");
    ETCB();
}

void ServMsg::do_BreakDown2(map < mstring, pair < unsigned int, unsigned int > > ServCounts, const mstring & mynick,
			    const mstring & source, const mstring & previndent, const mstring & server)
{
    BTCB();
    FT("ServMsg::do_BreakDown2", (mynick, source, previndent, server));
    vector < mstring > downlinks;
    mstring out, servername;
    unsigned int users, opers;
    float lag;

    if (server.empty())
    {
	Server::list_t::iterator iter;
	RLOCK((lck_Server, lck_list));
	for (iter = Magick::instance().server.ListBegin(); iter != Magick::instance().server.ListEnd(); iter++)
	{
	    map_entry < Server_t > svr(iter->second);
	    if (!svr->Name().empty() && svr->Uplink().IsSameAs(Magick::instance().startup.Server_Name(), true))
		downlinks.push_back(iter->first);
	}
    }
    else
    {
	if (Magick::instance().server.IsList(server))
	    downlinks = Magick::instance().server.GetList(server)->Downlinks();
    }

    for (unsigned int i = 0; i < downlinks.size(); i++)
    {
	if (Magick::instance().server.IsList(downlinks[i]))
	{
	    users = ServCounts[downlinks[i]].first;
	    opers = ServCounts[downlinks[i]].second;
	    {
		map_entry < Server_t > svr = Magick::instance().server.GetList(downlinks[i]);
		lag = svr->Lag();
		servername = svr->AltName();
	    }
	    if (i < downlinks.size() - 1)
	    {
		::sendV(mynick, source, "%-35s  % 3.3fs  %5d (%3d)  %3.2f%%", (previndent + "|-" + servername).c_str(), lag,
			users, opers,
			100.0 * static_cast < float > (users) / static_cast < float >
			(Magick::instance().nickserv.LiveSize()));
		do_BreakDown2(ServCounts, mynick, source, previndent + "| ", downlinks[i]);
	    }
	    else
	    {
		::sendV(mynick, source, "%-35s  % 3.3fs  %5d (%3d)  %3.2f%%", (previndent + "`-" + servername).c_str(), lag,
			users, opers,
			100.0 * static_cast < float > (users) / static_cast < float >
			(Magick::instance().nickserv.LiveSize()));
		do_BreakDown2(ServCounts, mynick, source, previndent + "  ", downlinks[i]);
	    }
	}
    }
    ETCB();
}

void ServMsg::do_stats_Nick(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_stats_Nick", (mynick, source, params));

    mstring message = params.Before(" ", 2);

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") > 2 && params.ExtractWord(3, " ").IsSameAs("CLEAR", true) &&
	Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source))
    {
	Magick::instance().nickserv.stats.clear();
	return;
    }

    unsigned long linked = 0, suspended = 0, forbidden = 0;

    NickServ::stored_t::iterator i;
    {
	RLOCK((lck_NickServ, lck_stored));
	for (i = Magick::instance().nickserv.StoredBegin(); i != Magick::instance().nickserv.StoredEnd(); i++)
	{
	    map_entry < Nick_Stored_t > nstored(i->second);
	    if (nstored->Forbidden())
		forbidden++;
	    else
	    {
		if (!nstored->Host().empty())
		    linked++;
		if (nstored->Suspended())
		    suspended++;
	    }
	}
    }

    SEND(mynick, source, "STATS/NICK_REGD", (Magick::instance().nickserv.StoredSize(), linked));
    SEND(mynick, source, "STATS/NICK_DENIED", (suspended, forbidden));
    SEND(mynick, source, "STATS/NICK_CMD", (ToHumanTime(Magick::instance().nickserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/NICK_CMD1",
	 (fmstring("%10d", Magick::instance().nickserv.stats.Register()),
	  fmstring("%10d", Magick::instance().nickserv.stats.Drop())));
    SEND(mynick, source, "STATS/NICK_CMD2",
	 (fmstring("%10d", Magick::instance().nickserv.stats.Link()),
	  fmstring("%10d", Magick::instance().nickserv.stats.Unlink())));
    SEND(mynick, source, "STATS/NICK_CMD3",
	 (fmstring("%10d", Magick::instance().nickserv.stats.Host()),
	  fmstring("%10d", Magick::instance().nickserv.stats.Identify())));
    SEND(mynick, source, "STATS/NICK_CMD4",
	 (fmstring("%10d", Magick::instance().nickserv.stats.Ghost()),
	  fmstring("%10d", Magick::instance().nickserv.stats.Recover())));
    SEND(mynick, source, "STATS/NICK_CMD5",
	 (fmstring("%10d", Magick::instance().nickserv.stats.Suspend()),
	  fmstring("%10d", Magick::instance().nickserv.stats.Unsuspend())));
#ifdef GETPASS
    SEND(mynick, source, "STATS/NICK_CMD6A",
	 (fmstring("%10d", Magick::instance().nickserv.stats.Forbid()),
	  fmstring("%10d", Magick::instance().nickserv.stats.Getpass())));
#else
    SEND(mynick, source, "STATS/NICK_CMD6B",
	 (fmstring("%10d", Magick::instance().nickserv.stats.Forbid()),
	  fmstring("%10d", Magick::instance().nickserv.stats.Getpass())));
#endif
    SEND(mynick, source, "STATS/NICK_CMD7",
	 (fmstring("%10d", Magick::instance().nickserv.stats.Access()),
	  fmstring("%10d", Magick::instance().nickserv.stats.Ignore())));
    SEND(mynick, source, "STATS/NICK_CMD8",
	 (fmstring("%10d", Magick::instance().nickserv.stats.Set()),
	  fmstring("%10d", Magick::instance().nickserv.stats.NoExpire())));
    SEND(mynick, source, "STATS/NICK_CMD9",
	 (fmstring("%10d", Magick::instance().nickserv.stats.Lock()),
	  fmstring("%10d", Magick::instance().nickserv.stats.Unlock())));
    SEND(mynick, source, "STATS/NICK_CMD10",
	 (fmstring("%10d", Magick::instance().nickserv.stats.SetPicture()),
	  fmstring("%10d", Magick::instance().nickserv.stats.Send())));
    Magick::instance().servmsg.stats.i_Stats++;
    ETCB();
}

void ServMsg::do_stats_Channel(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_stats_Channel", (mynick, source, params));

    mstring message = params.Before(" ", 2);

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") > 2 && params.ExtractWord(3, " ").IsSameAs("CLEAR", true) &&
	Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source))
    {
	Magick::instance().chanserv.stats.clear();
	return;
    }

    unsigned long suspended = 0, forbidden = 0;

    ChanServ::stored_t::iterator i;
    {
	RLOCK((lck_ChanServ, lck_stored));
	for (i = Magick::instance().chanserv.StoredBegin(); i != Magick::instance().chanserv.StoredEnd(); i++)
	{
	    map_entry < Chan_Stored_t > cstored(i->second);
	    if (cstored->Forbidden())
		forbidden++;
	    else
	    {
		if (cstored->Suspended())
		    suspended++;
	    }
	}
    }

    SEND(mynick, source, "STATS/CHAN_REGD", (Magick::instance().chanserv.StoredSize()));
    SEND(mynick, source, "STATS/CHAN_DENIED", (suspended, forbidden));
    SEND(mynick, source, "STATS/CHAN_CMD", (ToHumanTime(Magick::instance().chanserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/CHAN_CMD1",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Register()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Drop())));
    SEND(mynick, source, "STATS/CHAN_CMD2", (fmstring("%10d", Magick::instance().chanserv.stats.Identify())));
    SEND(mynick, source, "STATS/CHAN_CMD3",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Suspend()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Unsuspend())));
#ifdef GETPASS
    SEND(mynick, source, "STATS/CHAN_CMD4A",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Forbid()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Getpass())));
#else
    SEND(mynick, source, "STATS/CHAN_CMD4B",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Forbid()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Getpass())));
#endif
    SEND(mynick, source, "STATS/CHAN_CMD5",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Mode()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Topic())));
    SEND(mynick, source, "STATS/CHAN_CMD6",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Op()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Deop())));
    SEND(mynick, source, "STATS/CHAN_CMD7",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Halfop()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Dehalfop())));
    SEND(mynick, source, "STATS/CHAN_CMD8",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Voice()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Devoice())));
    SEND(mynick, source, "STATS/CHAN_CMD9",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Kick()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Anonkick())));
    SEND(mynick, source, "STATS/CHAN_CMD10",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Invite()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Unban())));
    SEND(mynick, source, "STATS/CHAN_CMD11",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Clear()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Akick())));
    SEND(mynick, source, "STATS/CHAN_CMD12",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Level()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Access())));
    SEND(mynick, source, "STATS/CHAN_CMD13",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Greet()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Message())));
    SEND(mynick, source, "STATS/CHAN_CMD14",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Set()),
	  fmstring("%10d", Magick::instance().chanserv.stats.NoExpire())));
    SEND(mynick, source, "STATS/CHAN_CMD15",
	 (fmstring("%10d", Magick::instance().chanserv.stats.Lock()),
	  fmstring("%10d", Magick::instance().chanserv.stats.Unlock())));
    Magick::instance().servmsg.stats.i_Stats++;
    ETCB();
}

void ServMsg::do_stats_Other(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_stats_Other", (mynick, source, params));

    mstring message = params.Before(" ", 2);

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") > 2 && params.ExtractWord(3, " ").IsSameAs("CLEAR", true) &&
	Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source))
    {
	Magick::instance().memoserv.stats.clear();
	Magick::instance().commserv.stats.clear();
	Magick::instance().servmsg.stats.clear();
	return;
    }

    SEND(mynick, source, "STATS/OTH_MEMO", (Magick::instance().memoserv.NickSize()));
    SEND(mynick, source, "STATS/OTH_NEWS", (Magick::instance().memoserv.ChannelSize()));
    SEND(mynick, source, "STATS/OTH_COMM", (Magick::instance().commserv.ListSize()));

    SEND(mynick, source, "STATS/OTH_CMD",
	 (Magick::instance().memoserv.GetInternalName(),
	  ToHumanTime(Magick::instance().operserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/OTH_CMD1",
	 (fmstring("%10d", Magick::instance().memoserv.stats.Read()),
	  fmstring("%10d", Magick::instance().memoserv.stats.Unread())));
    SEND(mynick, source, "STATS/OTH_CMD2",
	 (fmstring("%10d", Magick::instance().memoserv.stats.Send()),
	  fmstring("%10d", Magick::instance().memoserv.stats.Flush())));
    SEND(mynick, source, "STATS/OTH_CMD3",
	 (fmstring("%10d", Magick::instance().memoserv.stats.Reply()),
	  fmstring("%10d", Magick::instance().memoserv.stats.Forward())));
    SEND(mynick, source, "STATS/OTH_CMD4",
	 (fmstring("%10d", Magick::instance().memoserv.stats.Cancel()),
	  fmstring("%10d", Magick::instance().memoserv.stats.Del())));
    SEND(mynick, source, "STATS/OTH_CMD5",
	 (fmstring("%10d", Magick::instance().memoserv.stats.Continue()),
	  fmstring("%10d", Magick::instance().memoserv.stats.Set())));
    SEND(mynick, source, "STATS/OTH_CMD6",
	 (fmstring("%10d", Magick::instance().memoserv.stats.File()),
	  fmstring("%10d", Magick::instance().memoserv.stats.Get())));

    SEND(mynick, source, "STATS/OTH_CMD",
	 (Magick::instance().commserv.GetInternalName(),
	  ToHumanTime(Magick::instance().operserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/OTH_CMD11",
	 (fmstring("%10d", Magick::instance().commserv.stats.Add()),
	  fmstring("%10d", Magick::instance().commserv.stats.Del())));
    SEND(mynick, source, "STATS/OTH_CMD12",
	 (fmstring("%10d", Magick::instance().commserv.stats.Member()),
	  fmstring("%10d", Magick::instance().commserv.stats.Logon())));
    SEND(mynick, source, "STATS/OTH_CMD13",
	 (fmstring("%10d", Magick::instance().commserv.stats.Memo()),
	  fmstring("%10d", Magick::instance().commserv.stats.Set())));
    SEND(mynick, source, "STATS/OTH_CMD14",
	 (fmstring("%10d", Magick::instance().commserv.stats.Lock()),
	  fmstring("%10d", Magick::instance().commserv.stats.Unlock())));

    SEND(mynick, source, "STATS/OTH_CMD",
	 (Magick::instance().servmsg.GetInternalName(),
	  ToHumanTime(Magick::instance().operserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/OTH_CMD21",
	 (fmstring("%10d", Magick::instance().servmsg.stats.Global()),
	  fmstring("%10d", Magick::instance().servmsg.stats.Credits())));
    SEND(mynick, source, "STATS/OTH_CMD22",
	 (fmstring("%10d", Magick::instance().servmsg.stats.Ask()),
	  fmstring("%10d", Magick::instance().servmsg.stats.Stats())));
    SEND(mynick, source, "STATS/OTH_CMD23",
	 (fmstring("%10d", Magick::instance().servmsg.stats.File_Add()),
	  fmstring("%10d", Magick::instance().servmsg.stats.File_Del())));
    SEND(mynick, source, "STATS/OTH_CMD24",
	 (fmstring("%10d", Magick::instance().servmsg.stats.File_Priv()),
	  fmstring("%10d", Magick::instance().servmsg.stats.File_Rename())));
    SEND(mynick, source, "STATS/OTH_CMD25",
	 (fmstring("%10d", Magick::instance().servmsg.stats.File_Send()),
	  fmstring("%10d", Magick::instance().servmsg.stats.File_Cancel())));
    Magick::instance().servmsg.stats.i_Stats++;
    ETCB();
}

void ServMsg::do_stats_Oper(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_stats_Oper", (mynick, source, params));

    mstring message = params.Before(" ", 2);

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") > 2 && params.ExtractWord(3, " ").IsSameAs("CLEAR", true) &&
	Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source))
    {
	Magick::instance().operserv.stats.clear();
	return;
    }

    SEND(mynick, source, "STATS/OPER_CLONE", (Magick::instance().operserv.Clone_size()));
    SEND(mynick, source, "STATS/OPER_AKILL", (Magick::instance().operserv.Akill_size()));
    SEND(mynick, source, "STATS/OPER_OPERDENY", (Magick::instance().operserv.OperDeny_size()));
    SEND(mynick, source, "STATS/OPER_IGNORE", (Magick::instance().operserv.Ignore_size()));

    SEND(mynick, source, "STATS/OPER_CMD", (ToHumanTime(Magick::instance().operserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/OPER_CMD1",
	 (fmstring("%10d", Magick::instance().operserv.stats.Trace()),
	  fmstring("%10d", Magick::instance().operserv.stats.Mode())));
    if (!(Magick::instance().server.proto.SQLINE().empty() || Magick::instance().server.proto.UNSQLINE().empty()))
    {
	SEND(mynick, source, "STATS/OPER_CMD2",
	     (fmstring("%10d", Magick::instance().operserv.stats.Qline()),
	      fmstring("%10d", Magick::instance().operserv.stats.Unqline())));
    }
    if (!(Magick::instance().server.proto.SVSNOOP().empty() || Magick::instance().server.proto.SVSKILL().empty()))
    {
	SEND(mynick, source, "STATS/OPER_CMD3",
	     (fmstring("%10d", Magick::instance().operserv.stats.Noop()),
	      fmstring("%10d", Magick::instance().operserv.stats.Kill())));
    }
    SEND(mynick, source, "STATS/OPER_CMD4",
	 (fmstring("%10d", Magick::instance().operserv.stats.Ping()),
	  fmstring("%10d", Magick::instance().operserv.stats.Update())));
    SEND(mynick, source, "STATS/OPER_CMD5",
	 (fmstring("%10d", Magick::instance().operserv.stats.Reload()),
	  fmstring("%10d", Magick::instance().operserv.stats.Unload())));
    SEND(mynick, source, "STATS/OPER_CMD6",
	 (fmstring("%10d", Magick::instance().operserv.stats.Jupe()),
	  fmstring("%10d", Magick::instance().operserv.stats.OnOff())));
    SEND(mynick, source, "STATS/OPER_CMD7",
	 (fmstring("%10d", Magick::instance().operserv.stats.Clone()),
	  fmstring("%10d", Magick::instance().operserv.stats.Akill())));
    SEND(mynick, source, "STATS/OPER_CMD8",
	 (fmstring("%10d", Magick::instance().operserv.stats.OperDeny()),
	  fmstring("%10d", Magick::instance().operserv.stats.Ignore())));
    if (!Magick::instance().server.proto.SVSHOST().empty())
	SEND(mynick, source, "STATS/OPER_CMD9", (fmstring("%10d", Magick::instance().operserv.stats.Hide())));
    Magick::instance().servmsg.stats.i_Stats++;
    ETCB();
}

void ServMsg::do_stats_Usage(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_stats_Usage", (mynick, source, params));
    int count;
    size_t size;

    mstring message = params.Before(" ", 2);

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    rusage tmp;

    ACE_OS::getrusage(RUSAGE_SELF, &tmp);
    ACE_Time_Value user, sys;

    user = tmp.ru_utime;
    sys = tmp.ru_stime;
    SEND(mynick, source, "STATS/USE_CPU",
	 (((sys.sec() == 0) ? Magick::instance().getMessage(source, "VALS/TIME_NONE") : ToHumanTime(sys.sec(), source)),
	  ((user.sec() == 0) ? Magick::instance().getMessage(source, "VALS/TIME_NONE") : ToHumanTime(user.sec(), source))));

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL)
	{
	    SEND(mynick, source, "STATS/USE_TRAFFIC",
		 (ToHumanSpace(Magick::instance().ircsvchandler->In_Traffic()),
		  ToHumanSpace(Magick::instance().ircsvchandler->In_Traffic() /
			       Magick::instance().ircsvchandler->Connect_Time().SecondsSince()),
		  ToHumanSpace(Magick::instance().ircsvchandler->Out_Traffic()),
		  ToHumanSpace(Magick::instance().ircsvchandler->Out_Traffic() /
			       Magick::instance().ircsvchandler->Connect_Time().SecondsSince()),
		  ToHumanTime(Magick::instance().ircsvchandler->Connect_Time().SecondsSince(), source)));
	}
    }

    size = 0;
    NickServ::live_t::iterator i;
    {
	RLOCK((lck_NickServ, lck_live));
	for (i = Magick::instance().nickserv.LiveBegin(); i != Magick::instance().nickserv.LiveEnd(); i++)
	{
	    map_entry < Nick_Live_t > nlive(i->second);
	    size += i->first.capacity();
	    size += sizeof(i->second);
	    size += nlive->Usage();
	}
    }
    SEND(mynick, source, "STATS/USE_NS_LIVE", (fmstring("%5d", Magick::instance().nickserv.LiveSize()), ToHumanSpace(size)));

    size = 0;
    ChanServ::live_t::iterator j;
    {
	RLOCK((lck_ChanServ, lck_live));
	for (j = Magick::instance().chanserv.LiveBegin(); j != Magick::instance().chanserv.LiveEnd(); j++)
	{
	    map_entry < Chan_Live_t > clive(j->second);
	    size += j->first.capacity();
	    size += sizeof(j->second);
	    size += clive->Usage();
	}
    }
    SEND(mynick, source, "STATS/USE_CS_LIVE", (fmstring("%5d", Magick::instance().chanserv.LiveSize()), ToHumanSpace(size)));

    size = 0;
    NickServ::stored_t::iterator k;
    {
	RLOCK((lck_NickServ, lck_stored));
	for (k = Magick::instance().nickserv.StoredBegin(); k != Magick::instance().nickserv.StoredEnd(); k++)
	{
	    map_entry < Nick_Stored_t > nstored(k->second);
	    size += k->first.capacity();
	    size += sizeof(k->second);
	    size += nstored->Usage();
	}
    }
    SEND(mynick, source, "STATS/USE_NS_STORED",
	 (fmstring("%5d", Magick::instance().nickserv.StoredSize()), ToHumanSpace(size)));

    size = 0;
    ChanServ::stored_t::iterator l;
    {
	RLOCK((lck_ChanServ, lck_stored));
	for (l = Magick::instance().chanserv.StoredBegin(); l != Magick::instance().chanserv.StoredEnd(); l++)
	{
	    map_entry < Chan_Stored_t > cstored(l->second);
	    size += l->first.capacity();
	    size += sizeof(l->second);
	    size += cstored->Usage();
	}
    }
    SEND(mynick, source, "STATS/USE_CS_STORED",
	 (fmstring("%5d", Magick::instance().chanserv.StoredSize()), ToHumanSpace(size)));

    size = 0;
    MemoServ::nick_t::iterator m1;
    MemoServ::nick_memo_t::iterator m2;
    {
	RLOCK((lck_MemoServ, lck_nick));
	for (count = 0, m1 = Magick::instance().memoserv.NickBegin(); m1 != Magick::instance().memoserv.NickEnd(); m1++)
	{
	    {
		RLOCK2((lck_MemoServ, lck_nick, m1->first));
		size += m1->first.capacity();
		count += m1->second.size();
		for (m2 = m1->second.begin(); m2 != m1->second.end(); m2++)
		{
		    size += m2->Usage();
		}
	    }
	}
    }
    SEND(mynick, source, "STATS/USE_MEMO", (fmstring("%5d", count), ToHumanSpace(size)));

    size = 0;
    MemoServ::channel_t::iterator n1;
    MemoServ::channel_news_t::iterator n2;
    {
	RLOCK((lck_MemoServ, lck_channel));
	for (count = 0, n1 = Magick::instance().memoserv.ChannelBegin(); n1 != Magick::instance().memoserv.ChannelEnd(); n1++)
	{
	    size += n1->first.capacity();
	    {
		RLOCK2((lck_MemoServ, lck_channel, n1->first));
		count += n1->second.size();
		for (n2 = n1->second.begin(); n2 != n1->second.end(); n2++)
		{
		    size += n2->Usage();
		}
	    }
	}
    }
    SEND(mynick, source, "STATS/USE_NEWS", (fmstring("%5d", count), ToHumanSpace(size)));

    size = 0;
    CommServ::list_t::iterator o;
    {
	RLOCK((lck_CommServ, lck_list));
	for (o = Magick::instance().commserv.ListBegin(); o != Magick::instance().commserv.ListEnd(); o++)
	{
	    map_entry < Committee_t > comm(o->second);
	    size += o->first.capacity();
	    size += sizeof(o->second);
	    size += comm->Usage();
	}
    }
    SEND(mynick, source, "STATS/USE_COMMITTEE", (fmstring("%5d", Magick::instance().commserv.ListSize()), ToHumanSpace(size)));

    SEND(mynick, source, "STATS/USE_OPERSERV",
	 (fmstring
	  ("%5d",
	   (Magick::instance().operserv.Clone_size() + Magick::instance().operserv.Akill_size() +
	    Magick::instance().operserv.OperDeny_size() + Magick::instance().operserv.Ignore_size())),
	  ToHumanSpace(Magick::instance().operserv.Clone_Usage() + Magick::instance().operserv.Akill_Usage() +
		       Magick::instance().operserv.OperDeny_Usage() + Magick::instance().operserv.Ignore_Usage())));

    size = 0;
    Server::list_t::iterator p;
    {
	RLOCK((lck_Server, lck_list));
	for (p = Magick::instance().server.ListBegin(); p != Magick::instance().server.ListEnd(); p++)
	{
	    map_entry < Server_t > server(p->second);
	    size += p->first.capacity();
	    size += sizeof(p->second);
	    size += server->Usage();
	}
    }
    SEND(mynick, source, "STATS/USE_OTHER", (fmstring("%5d", Magick::instance().server.ListSize()), ToHumanSpace(size)));

    NSEND(mynick, source, "STATS/USE_LANGHEAD");

    set < mstring > tmpset, lang;
    set < mstring >::iterator iter;
    tmpset.clear();
    tmpset = Magick::instance().LNG_Loaded();
    for (iter = tmpset.begin(); iter != tmpset.end(); iter++)
	lang.insert(*iter);
    tmpset.clear();
    tmpset = Magick::instance().HLP_Loaded();
    for (iter = tmpset.begin(); iter != tmpset.end(); iter++)
	lang.insert(*iter);

    set < mstring >::iterator q;
    for (q = lang.begin(); q != lang.end(); q++)
    {
	::sendV(mynick, source, "%-20s %7s  %7s", q->c_str(), ToHumanSpace(Magick::instance().LNG_Usage(*q)).c_str(),
		ToHumanSpace(Magick::instance().HLP_Usage(*q)).c_str());

    }
    Magick::instance().servmsg.stats.i_Stats++;
    ETCB();
}

void ServMsg::do_stats_All(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_stats_All", (mynick, source, params));

    mstring message = params.Before(" ", 2);

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    do_Stats(mynick, source, params.Before(" "));
    do_stats_Nick(mynick, source, params);
    do_stats_Channel(mynick, source, params);
    do_stats_Oper(mynick, source, params);
    do_stats_Other(mynick, source, params);
    Magick::instance().servmsg.stats.i_Stats -= 5;
    do_stats_Usage(mynick, source, params);
    ETCB();
}

void ServMsg::do_Stats(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_Stats", (mynick, source, params));

    if (params.WordCount(" ") > 1 &&
	((Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsOn(source)) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source))))
    {
	do_1_2param(mynick, source, params);
	return;
    }

    mstring message = params.Before(" ");

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    SEND(mynick, source, "STATS/GEN_UPTIME", (Magick::StartTime().Ago()));
    if (Magick::StartTime() != Magick::instance().ResetTime())
	SEND(mynick, source, "STATS/GEN_RESET", (Magick::instance().ResetTime().Ago()));
    SEND(mynick, source, "STATS/GEN_MAXUSERS", (Magick::instance().server.UserMax()));
    size_t opers = 0;

    NickServ::live_t::iterator k;
    {
	RLOCK((lck_NickServ, lck_live));
	for (k = Magick::instance().nickserv.LiveBegin(); k != Magick::instance().nickserv.LiveEnd(); k++)
	{
	    map_entry < Nick_Live_t > nlive(k->second);
	    if (nlive->HasMode("o"))
		opers++;
	}
    }
    SEND(mynick, source, "STATS/GEN_USERS", (Magick::instance().nickserv.LiveSize(), opers));

    if ((Magick::instance().operserv.CloneList_size() - Magick::instance().operserv.CloneList_size(1)))
	SEND(mynick, source, "STATS/GEN_CLONES",
	     (Magick::instance().operserv.CloneList_sum() - Magick::instance().operserv.CloneList_size(),
	      Magick::instance().operserv.CloneList_size() - Magick::instance().operserv.CloneList_size(1)));
    Magick::instance().servmsg.stats.i_Stats++;
    ETCB();
}

void ServMsg::do_file_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_file_List", (mynick, source, params));

    unsigned int listsize, i, j, count;
    bool display;
    mstring mask, priv;

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	mask = "*";
	listsize = Magick::instance().config.Listsize();
    }
    else if (params.WordCount(" ") < 4)
    {
	mask = params.ExtractWord(3, " ").LowerCase();
	listsize = Magick::instance().config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(3, " ").LowerCase();
	listsize = atoi(params.ExtractWord(4, " ").c_str());
	if (listsize > Magick::instance().config.Maxlist())
	{
	    mstring output;

	    SEND(mynick, source, "LIST/MAXLIST", (Magick::instance().config.Maxlist()));
	    return;
	}
    }

    vector < unsigned long > filelist = Magick::instance().filesys.GetList(FileMap::Public, source);

    if (!filelist.size())
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/FILES")));
	return;
    }

    SEND(mynick, source, "LIST/DISPLAY_MATCH", (mask, Magick::instance().getMessage(source, "LIST/FILES")));

    bool issop = (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source));

    for (j = 0, i = 0, count = 0; j < filelist.size(); j++)
    {
	if (Magick::instance().filesys.GetName(FileMap::Public, filelist[j]).Matches(mask, true))
	{
	    if (i < listsize)
	    {
		if (issop)
		    ::sendV(mynick, source, "%s (%s) [%s]",
			    Magick::instance().filesys.GetName(FileMap::Public, filelist[j]).c_str(),
			    ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::Public, filelist[j])).c_str(),
			    Magick::instance().filesys.GetPriv(FileMap::Public, filelist[j]).c_str());
		else
		{
		    display = false;
		    priv = Magick::instance().filesys.GetPriv(FileMap::Public, filelist[j]);
		    if (priv.empty())
			display = true;
		    else
		    {
			for (unsigned int k = 1; k <= priv.WordCount(" "); k++)
			    if (Magick::instance().commserv.IsList(priv.ExtractWord(k, " ")) &&
				Magick::instance().commserv.GetList(priv.ExtractWord(k, " "))->IsOn(source))
			    {
				display = true;
				break;
			    }
		    }
		    if (display)
			::sendV(mynick, source, "%s (%s)",
				Magick::instance().filesys.GetName(FileMap::Public, filelist[j]).c_str(),
				ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::Public, filelist[j])).c_str());
		}

		i++;
	    }
	    count++;
	}
    }
    SEND(mynick, source, "LIST/DISPLAYED", (i, count));
    ETCB();
}

void ServMsg::do_file_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_file_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring priv;

    if (params.WordCount(" ") > 2)
	priv = params.After(" ", 2).UpperCase();

    Magick::instance().servmsg.stats.i_file_Add++;
    Magick::instance().nickserv.GetLive(source)->InFlight.Public(mynick, priv);
    ETCB();
}

void ServMsg::do_file_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_file_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring file = params.ExtractWord(3, " ");
    unsigned long num = Magick::instance().filesys.GetNum(FileMap::Public, file);

    if (!num)
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (file, Magick::instance().getMessage(source, "LIST/FILES")));
	return;
    }

    Magick::instance().servmsg.stats.i_file_Del++;
    SEND(mynick, source, "LIST/DEL",
	 (Magick::instance().filesys.GetName(FileMap::Public, num), Magick::instance().getMessage(source, "LIST/FILES")));
    Magick::instance().filesys.EraseFile(FileMap::Public, num);
    ETCB();
}

void ServMsg::do_file_Rename(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_file_Rename", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring file = params.ExtractWord(3, " ");
    mstring newfile = params.ExtractWord(4, " ");
    unsigned long num = Magick::instance().filesys.GetNum(FileMap::Public, file);

    if (!num)
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (file, Magick::instance().getMessage(source, "LIST/FILES")));
	return;
    }

    Magick::instance().servmsg.stats.i_file_Rename++;
    SEND(mynick, source, "LIST/CHANGE_TIME",
	 (Magick::instance().filesys.GetName(FileMap::Public, num), Magick::instance().getMessage(source, "LIST/FILES"),
	  newfile));
    LOG(LM_INFO, "SERVMSG/FILE_RENAME",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), file, newfile));
    Magick::instance().filesys.Rename(FileMap::Public, num, newfile);
    ETCB();
}

void ServMsg::do_file_Priv(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_file_Priv", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring file = params.ExtractWord(3, " ");
    mstring priv;

    if (params.WordCount(" ") > 3)
	priv = params.After(" ", 3).UpperCase();
    unsigned long num = Magick::instance().filesys.GetNum(FileMap::Public, file);

    if (!num)
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (file, Magick::instance().getMessage(source, "LIST/FILES")));
	return;
    }

    Magick::instance().servmsg.stats.i_file_Priv++;
    SEND(mynick, source, "LIST/CHANGE2_TIME",
	 (Magick::instance().filesys.GetName(FileMap::Public, num), Magick::instance().getMessage(source, "LIST/FILES"),
	  Magick::instance().getMessage(source, "LIST/ACCESS"), priv));
    LOG(LM_INFO, "SERVMSG/FILE_PRIV",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), file,
	 ((priv.empty()) ? "ALL" : priv.c_str())));
    Magick::instance().filesys.SetPriv(FileMap::Public, num, priv);
    ETCB();
}

void ServMsg::do_file_Send(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_file_Send", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring filename = params.ExtractWord(3, " ");
    unsigned long filenum = Magick::instance().filesys.GetNum(FileMap::Public, filename);

    bool display = false;
    mstring priv = Magick::instance().filesys.GetPriv(FileMap::Public, filenum);

    if (priv.empty())
	display = true;
    else
    {
	for (unsigned int k = 1; k <= priv.WordCount(" "); k++)
	    if (Magick::instance().commserv.IsList(priv.ExtractWord(k, " ")) &&
		Magick::instance().commserv.GetList(priv.ExtractWord(k, " "))->IsOn(source))
	    {
		display = true;
		break;
	    }
    }

    if (!(filenum && display))
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (filename, Magick::instance().getMessage(source, "LIST/FILES")));
	return;
    }

    filename = Magick::instance().filesys.GetName(FileMap::Public, filenum);
    size_t filesize = Magick::instance().filesys.GetSize(FileMap::Public, filenum);

    if (filename.empty() || filesize <= 0)
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (filename, Magick::instance().getMessage(source, "LIST/FILES")));
	return;
    }

    if (!
	(Magick::instance().files.TempDirSize() == 0 ||
	 mFile::DirUsage(Magick::instance().files.TempDir()) <= Magick::instance().files.TempDirSize()))
    {
	NSEND(mynick, source, "DCC/NOSPACE2");
	return;
    }

    {
	RLOCK(("DCC"));
	if (Magick::instance().dcc != NULL)
	{
	    Magick::instance().servmsg.stats.i_file_Send++;
	    LOG(LM_INFO, "SERVMSG/FILE_SEND",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), filename));
	    unsigned short port = mSocket::FindAvailPort();

	    ::privmsg(mynick, source,
		      DccEngine::encode("DCC SEND",
					filename + " " + mstring(Magick::instance().LocalHost()) + " " + mstring(port) + " " +
					mstring(filesize)));
	    Magick::instance().dcc->Accept(port, mynick, source, FileMap::Public, filenum);
	}
    }
    ETCB();
}

void ServMsg::do_file_Dcc(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_file_Dcc", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (DccMap::XfersSize())
    {
	NSEND(mynick, source, "DCC/LIST_HEAD");
	DccMap::xfers_t::iterator iter;
	RLOCK((lck_DccMap, lck_xfers));
	for (iter = DccMap::XfersBegin(); iter != DccMap::XfersEnd(); iter++)
	{
	    RLOCK2((lck_DccMap, lck_xfers, iter->first));
	    if (iter->second == NULL)
		continue;

	    float speed = static_cast < float > (iter->second->Average());
	    char scale = 'b';

	    while (speed >= 1024.0)
	    {
		speed /= 1024.0;
		switch (scale)
		{
		case 'b':
		    scale = 'k';
		    break;
		case 'k':
		    scale = 'm';
		    break;
		case 'm':
		    scale = 'g';
		    break;
		case 'g':
		    scale = 't';
		    break;
		}
	    }

	    // ID       D      Size   Prog Speed/s User (File Name)
	    // 00000001 S 000000000   0.0% xxxx.xX PreZ (blah.tgz)
	    // 000000b2 S 000000000  48.2%         PreZ
	    // 0000ac36 R 000000000 100.0%         PreZ
	    ::sendV(mynick, source, "%08x %c %9d %5.1f%% %6.1f%c %s (%s)", iter->first,
		    ((iter->second->Type() == DccXfer::Get) ? 'R' : 'S'), iter->second->Filesize(),
		    100.0 * static_cast < float > (iter->second->Total()) / static_cast < float > (iter->second->Filesize()),
		    speed, scale, iter->second->Source().c_str(), iter->second->Filename().c_str());
	}
    }
    else
    {
	NSEND(mynick, source, "DCC/NOACTIVE");
    }
    ETCB();
}

void ServMsg::do_file_Cancel(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_file_Cancel", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring hexstr = params.ExtractWord(3, " ").LowerCase();

    if (hexstr.length() != 8)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", (8));
	return;
    }
    else
    {
	for (unsigned int i = 0; i < hexstr.length(); i++)
	    if (!mstring("0123456789abcdef").Contains(hexstr[i]))
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", (8));
		return;
	    }
    }

    unsigned long number;
    unsigned short high, low;

    high = makehex("0x" + hexstr.SubString(0, 3));
    low = makehex("0x" + hexstr.SubString(4, 7));
    number = (high * 0x00010000) + low;

    if (number == 0)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", (8));
	return;
    }

    RLOCK((lck_DccMap, lck_xfers));
    if (!DccMap::IsXfers(number))
    {
	SEND(mynick, source, "DCC/NODCCID", (fmstring("%08x", number)));
    }
    else
    {
	{
	    RLOCK2(("DCC"));
	    if (Magick::instance().dcc != NULL)
	    {
		Magick::instance().dcc->Cancel(number);
		Magick::instance().servmsg.stats.i_file_Cancel++;
		SEND(mynick, source, "DCC/CANCEL", (fmstring("%08x", number)));
	    }
	}
    }
    ETCB();
}

void ServMsg::do_file_Lookup(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_file_Lookup", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring type = params.ExtractWord(3, " ").UpperCase();
    mstring hexstr = params.ExtractWord(4, " ").LowerCase();

    if (hexstr.length() != 8)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", (8));
	return;
    }
    else
    {
	for (unsigned int i = 0; i < hexstr.length(); i++)
	    if (!mstring("0123456789abcdef").Contains(hexstr[i]))
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", (8));
		return;
	    }
    }

    unsigned long number;
    unsigned short high, low, k;

    high = makehex("0x" + hexstr.SubString(0, 3));
    low = makehex("0x" + hexstr.SubString(4, 7));
    number = (high * 0x00010000) + low;

    if (number == 0)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", (8));
	return;
    }

    if (type.Matches("M*A*", true))
    {
	type = "MemoAttach";
	if (Magick::instance().filesys.Exists(FileMap::MemoAttach, number))
	{
	    MemoServ::nick_t::iterator i;
	    MemoServ::nick_memo_t::iterator j;
	    RLOCK((lck_MemoServ, lck_nick));
	    for (i = Magick::instance().memoserv.NickBegin(); i != Magick::instance().memoserv.NickEnd(); i++)
	    {
		RLOCK2((lck_MemoServ, lck_nick, i->first));
		for (k = 1, j = i->second.begin(); j != i->second.end(); j++, k++)
		{
		    if (j->File() == number)
		    {
			SEND(mynick, source, "DCC/LOOKUP_MEMOATTACH",
			     (fmstring("%08x", number), Magick::instance().filesys.GetName(FileMap::MemoAttach, number),
			      j->Nick(), k, j->Sender(), j->Time().Ago()));
			LOG(LM_DEBUG, "SERVMSG/FILE_LOOKUP",
			    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), fmstring("%08x", number),
			     type));
			return;
		    }
		}
	    }
	}
	SEND(mynick, source, "DCC/NLOOKUP_MEMOATTACH", (fmstring("%08x", number)));
    }
    else if (type.Matches("PIC*", true))
    {
	type = "Picture";
	if (Magick::instance().filesys.Exists(FileMap::Picture, number))
	{
	    NickServ::stored_t::iterator i;
	    RLOCK((lck_NickServ, lck_stored));
	    for (i = Magick::instance().nickserv.StoredBegin(); i != Magick::instance().nickserv.StoredEnd(); i++)
	    {
		map_entry < Nick_Stored_t > nstored(i->second);
		if (nstored->PicNum() == number)
		{
		    SEND(mynick, source, "DCC/LOOKUP_PICTURE", (fmstring("%08x", number), nstored->Name()));
		    LOG(LM_DEBUG, "SERVMSG/FILE_LOOKUP",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), fmstring("%08x", number),
			 type));
		    return;
		}
	    }
	}
	SEND(mynick, source, "DCC/NLOOKUP_PICTURE", (fmstring("%08x", number)));
    }
    else if (type.Matches("PUB*", true))
    {
	type = "Public";
	if (Magick::instance().filesys.Exists(FileMap::Public, number))
	{
	    SEND(mynick, source, "DCC/LOOKUP_PUBLIC",
		 (fmstring("%08x", number), Magick::instance().filesys.GetName(FileMap::Public, number),
		  Magick::instance().filesys.GetPriv(FileMap::Public, number)));
	    LOG(LM_DEBUG, "SERVMSG/FILE_LOOKUP",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), fmstring("%08x", number), type));
	    return;
	}
	SEND(mynick, source, "DCC/NLOOKUP_PUBLIC", (fmstring("%08x", number)));
    }
    else
    {
	SEND(mynick, source, "DCC/NLOOKUP_OTHER", (type));
    }
    ETCB();
}

void ServMsg::do_Global(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_Global", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }
    mstring text = params.After(" ");

    Server::list_t::iterator iter;
    RLOCK((lck_Server, lck_list));
    for (iter = Magick::instance().server.ListBegin(); iter != Magick::instance().server.ListEnd(); iter++)
    {
	Magick::instance().server.NOTICE(Magick::instance().servmsg.FirstName(), "$" + iter->first, text);
    }
    Magick::instance().servmsg.stats.i_Global++;
    ANNOUNCE(mynick, "MISC/GLOBAL_MSG", (source));
    LOG(LM_NOTICE, "SERVMSG/GLOBAL", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), text));
    ETCB();
}

void ServMsg::do_Ask(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ServMsg::do_Ask", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (!Magick::instance().server.proto.Helpops())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }
    mstring text = params.After(" ");

    Magick::instance().servmsg.stats.i_Ask++;
    Magick::instance().server.HELPOPS(mynick, source + " (ASK) - " + text);
    ETCB();
}
