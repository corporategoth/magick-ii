#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.34  2000/03/08 23:38:37  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.33  2000/03/07 09:53:21  prez
** More helpfile updates (and associated updates).
**
** Revision 1.32  2000/03/02 11:59:45  prez
** More helpfile updates (slowly but surely writing it)
**
** Revision 1.31  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.30  2000/02/23 12:21:04  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.29  2000/02/17 12:55:07  ungod
** still working on borlandization
**
** Revision 1.28  2000/02/16 12:59:41  ungod
** fixing for borland compilability
**
** Revision 1.27  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.26  2000/02/15 10:37:51  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


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
	    "HELP", Parent->commserv.ALL_Name(), ServMsg::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CRED*", Parent->commserv.ALL_Name(), ServMsg::do_Credits);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CONTRIB*", Parent->commserv.ALL_Name(), ServMsg::do_Contrib);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "BREAKD*", Parent->commserv.ALL_Name(), ServMsg::do_BreakDown);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*MAP", Parent->commserv.ALL_Name(), ServMsg::do_BreakDown);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GLOB*", Parent->commserv.ADMIN_Name(), ServMsg::do_Global);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "STAT* NICK*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), ServMsg::do_stats_Nick);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "STAT* CHAN*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), ServMsg::do_stats_Channel);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "STAT* OTH*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), ServMsg::do_stats_Other);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "STAT* USAGE", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), ServMsg::do_stats_Usage);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "STAT* ALL*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), ServMsg::do_stats_All);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "STAT* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "STAT*", Parent->commserv.REGD_Name(), do_Stats);
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

    mstring message  = params.Before(" ").UpperCase();

    mstring HelpTopic = Parent->servmsg.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.Replace(" ", "/");
    vector<mstring> help = Parent->getHelp(source, HelpTopic.UpperCase());
					
    unsigned int i;
    for (i=0; i<help.size(); i++)
	::send(mynick, source, help[i]);
}


void ServMsg::do_Credits(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_Credits", (mynick, source, params));

    Parent->servmsg.stats.i_Credits++;
    for (int i=0; credits[i] != "---EOM---"; i++)
	if (credits[i].Len())
	    ::send(mynick, source, credits[i], mynick.c_str());
	else
	    ::send(mynick, source, " ");
}


void ServMsg::do_Contrib(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_Contrib", (mynick, source, params));

    Parent->servmsg.stats.i_Credits++;
    for (int i=0; contrib[i] != "---EOM---"; i++)
	if (contrib[i].Len())
	    ::send(mynick, source, contrib[i], mynick.c_str());
	else
	    ::send(mynick, source, " ");
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

    for (unsigned int i=0; i<downlinks.size(); i++)
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


void ServMsg::do_stats_Nick(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_stats_Nick", (mynick, source, params));

    unsigned long linked = 0, suspended = 0, forbidden = 0;
    map<mstring,Nick_Stored_t>::iterator i;
    for (i=Parent->nickserv.stored.begin();
		i!=Parent->nickserv.stored.end(); i++)
    {
	if (i->second.Forbidden())
	    forbidden++;
	else
	{
	    if (i->second.Host() != "")
		linked++;
	    if (i->second.Suspended())
		suspended++;
	}
    }

    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_REGD"),
		Parent->nickserv.stored.size(), linked);
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_DENIED"),
		suspended, forbidden);
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_CMD"));
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_CMD1"),
		Parent->nickserv.stats.Register(),
		Parent->nickserv.stats.Drop());
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_CMD2"),
		Parent->nickserv.stats.Link(),
		Parent->nickserv.stats.Unlink());
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_CMD3"),
		Parent->nickserv.stats.Host(),
		Parent->nickserv.stats.Identify());
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_CMD4"),
		Parent->nickserv.stats.Ghost(),
		Parent->nickserv.stats.Recover());
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_CMD5"),
		Parent->nickserv.stats.Suspend(),
		Parent->nickserv.stats.Unsuspend());
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_CMD6"),
		Parent->nickserv.stats.Forbid(),
		Parent->nickserv.stats.Getpass());
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_CMD7"),
		Parent->nickserv.stats.Access(),
		Parent->nickserv.stats.Ignore());
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_CMD8"),
		Parent->nickserv.stats.Set(),
		Parent->nickserv.stats.NoExpire());
    ::send(mynick, source, Parent->getMessage(source, "STATS/NICK_CMD9"),
		Parent->nickserv.stats.Lock(),
		Parent->nickserv.stats.Unlock());
}


void ServMsg::do_stats_Channel(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_stats_Channel", (mynick, source, params));

    unsigned long suspended = 0, forbidden = 0;
    map<mstring,Chan_Stored_t>::iterator i;
    for (i=Parent->chanserv.stored.begin();
		i!=Parent->chanserv.stored.end(); i++)
    {
	if (i->second.Forbidden())
	    forbidden++;
	else
	{
	    if (i->second.Suspended())
		suspended++;
	}
    }

    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_REGD"),
		Parent->chanserv.stored.size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_DENIED"),
		suspended, forbidden);
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD"));
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD1"),
		Parent->chanserv.stats.Register(),
		Parent->chanserv.stats.Drop());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD2"),
		Parent->chanserv.stats.Identify());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD3"),
		Parent->chanserv.stats.Suspend(),
		Parent->chanserv.stats.Unsuspend());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD4"),
		Parent->chanserv.stats.Forbid(),
		Parent->chanserv.stats.Getpass());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD5"),
		Parent->chanserv.stats.Mode(),
		Parent->chanserv.stats.Topic());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD6"),
		Parent->chanserv.stats.Op(),
		Parent->chanserv.stats.Deop());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD7"),
		Parent->chanserv.stats.Voice(),
		Parent->chanserv.stats.Devoice());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD8"),
		Parent->chanserv.stats.Kick(),
		Parent->chanserv.stats.Anonkick());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD9"),
		Parent->chanserv.stats.Invite(),
		Parent->chanserv.stats.Unban());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD10"),
		Parent->chanserv.stats.Clear(),
		Parent->chanserv.stats.Akick());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD11"),
		Parent->chanserv.stats.Level(),
		Parent->chanserv.stats.Access());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD12"),
		Parent->chanserv.stats.Greet(),
		Parent->chanserv.stats.Message());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD13"),
		Parent->chanserv.stats.Set(),
		Parent->chanserv.stats.NoExpire());
    ::send(mynick, source, Parent->getMessage(source, "STATS/CHAN_CMD14"),
		Parent->chanserv.stats.Lock(),
		Parent->chanserv.stats.Unlock());
}


void ServMsg::do_stats_Other(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_stats_Other", (mynick, source, params));

    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_MEMO"),
		Parent->memoserv.nick.size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_NEWS"),
		Parent->memoserv.channel.size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_COMM"),
		Parent->commserv.list.size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CLONE"),
		Parent->operserv.Clone_size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_AKILL"),
		Parent->operserv.Akill_size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_OPERDENY"),
		Parent->operserv.OperDeny_size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_IGNORE"),
		Parent->operserv.Ignore_size());
    
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD"),
		Parent->memoserv.GetInternalName().c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD1"),
		Parent->memoserv.stats.Read(),
		Parent->memoserv.stats.Unread());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD2"),
		Parent->memoserv.stats.Send(),
		Parent->memoserv.stats.Flush());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD3"),
		Parent->memoserv.stats.Reply(),
		Parent->memoserv.stats.Forward());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD4"),
		Parent->memoserv.stats.Cancel(),
		Parent->memoserv.stats.Del());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD5"),
		Parent->memoserv.stats.Continue(),
		Parent->memoserv.stats.File());

    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD"),
		Parent->commserv.GetInternalName().c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD6"),
		Parent->commserv.stats.New(),
		Parent->commserv.stats.Kill());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD7"),
		Parent->commserv.stats.AddDel(),
		Parent->commserv.stats.Memo());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD8"),
		Parent->commserv.stats.Logon(),
		Parent->commserv.stats.Set());

    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD"),
		Parent->servmsg.GetInternalName().c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD9"),
		Parent->servmsg.stats.Global(),
		Parent->servmsg.stats.Credits());

    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD"),
		Parent->operserv.GetInternalName().c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD10"),
		Parent->operserv.stats.Trace(),
		Parent->operserv.stats.Mode());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD11"),
		Parent->operserv.stats.Qline(),
		Parent->operserv.stats.Unqline());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD12"),
		Parent->operserv.stats.Noop(),
		Parent->operserv.stats.Kill());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD13"),
		Parent->operserv.stats.Ping(),
		Parent->operserv.stats.Update());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD14"),
		Parent->operserv.stats.Reload(),
		Parent->operserv.stats.Unload());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD15"),
		Parent->operserv.stats.Jupe(),
		Parent->operserv.stats.OnOff());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD16"),
		Parent->operserv.stats.Clone(),
		Parent->operserv.stats.Akill());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD17"),
		Parent->operserv.stats.OperDeny(),
		Parent->operserv.stats.Ignore());
}


void ServMsg::do_stats_Usage(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_stats_Usage", (mynick, source, params));
    int count;

    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_NS_LIVE"),
		Parent->nickserv.live.size(),
		(Parent->nickserv.live.size() * sizeof(mstring) +
		Parent->nickserv.live.size() * sizeof(Nick_Live_t)) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_CS_LIVE"),
		Parent->chanserv.live.size(),
		(Parent->chanserv.live.size() * sizeof(mstring) +
		Parent->chanserv.live.size() * sizeof(Chan_Live_t)) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_NS_STORED"),
		Parent->nickserv.stored.size(),
		(Parent->nickserv.stored.size() * sizeof(mstring) +
		Parent->nickserv.stored.size() * sizeof(Nick_Stored_t)) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_CS_STORED"),
		Parent->chanserv.stored.size(),
		(Parent->chanserv.stored.size() * sizeof(mstring) +
		Parent->chanserv.stored.size() * sizeof(Chan_Stored_t)) / 1024);
    map<mstring,list<Memo_t> >::iterator mi;
    for (count = 0, mi=Parent->memoserv.nick.begin();
			mi!=Parent->memoserv.nick.end(); mi++)
	count += mi->second.size();
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_MEMO"),
		Parent->memoserv.nick.size(),
		(Parent->memoserv.nick.size() * sizeof(mstring) +
		count * sizeof(Memo_t)) / 1024);
    map<mstring,list<News_t> >::iterator ni;
    for (count = 0, ni=Parent->memoserv.channel.begin();
			ni!=Parent->memoserv.channel.end(); ni++)
	count += ni->second.size();
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_NEWS"),
		Parent->memoserv.channel.size(),
		(Parent->memoserv.channel.size() * sizeof(mstring) +
		count * sizeof(News_t)) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_COMMITTEE"),
		Parent->commserv.list.size(),
		(Parent->commserv.list.size() * sizeof(mstring) +
		Parent->commserv.list.size() * sizeof(Committee)) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_OPERSERV"),
		(Parent->operserv.Clone_size() +
		Parent->operserv.Akill_size() +
		Parent->operserv.OperDeny_size() +
		Parent->operserv.Ignore_size()),
		(Parent->operserv.Clone_size() * sizeof(*Parent->operserv.Clone) +
		Parent->operserv.Akill_size() * sizeof(*Parent->operserv.Akill) +
		Parent->operserv.OperDeny_size() * sizeof(*Parent->operserv.OperDeny) +
		Parent->operserv.Ignore_size() * sizeof(*Parent->operserv.Ignore)) / 1024);
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_OTHER"),
		Parent->server.ServerList.size(),
		(Parent->server.ServerList.size() * sizeof(mstring) +
		Parent->server.ServerList.size() * sizeof(Server)) / 1024);
}

void ServMsg::do_stats_All(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_stats_All", (mynick, source, params));

    do_Stats(mynick, source, params.ExtractWord(1, " "));
    do_stats_Nick(mynick, source, params);
    do_stats_Channel(mynick, source, params);
    do_stats_Other(mynick, source, params);
    do_stats_Usage(mynick, source, params);
}

void ServMsg::do_Stats(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_Stats", (mynick, source, params));

    if (params.WordCount(" ") > 1 &&
	((Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source)) ||
	 (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))))
    {
	do_1_2param(mynick, source, params);
	return;
    }

    mDateTime tmp = StartTime;
    ::send(mynick, source, Parent->getMessage(source, "STATS/GEN_UPTIME"),
		StartTime.Ago().c_str());
    if ((tmp - Parent->ResetTime()).Minute() >= 1)
	::send(mynick, source, Parent->getMessage(source, "STATS/GEN_RESET"),
		Parent->ResetTime().Ago().c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/GEN_MAXUSERS"),
		Parent->server.UserMax());

    size_t opers = 0;
    map<mstring,Nick_Live_t>::iterator k;
    for (k=Parent->nickserv.live.begin(); k!=Parent->nickserv.live.end(); k++)
    {
	if (k->second.HasMode("o"))
		opers++;
    }
    ::send(mynick, source, Parent->getMessage(source, "STATS/GEN_USERS"),
		Parent->nickserv.live.size(), opers);

    if (Parent->operserv.CloneList_size() - Parent->operserv.CloneList_size(1u))
	::send(mynick, source, Parent->getMessage(source, "STATS/GEN_CLONES"),
		Parent->operserv.CloneList_sum() - Parent->operserv.CloneList_size(),
		Parent->operserv.CloneList_size() - Parent->operserv.CloneList_size(1u));
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
    Parent->servmsg.stats.i_Global++;
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
