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
** Revision 1.56  2000/06/10 07:01:04  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.55  2000/06/09 13:57:00  prez
** Added tracing to mconfig
**
** Revision 1.54  2000/06/08 13:07:35  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.53  2000/06/06 08:57:58  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.52  2000/05/27 15:10:12  prez
** Misc changes, mainly re-did the makefile system, makes more sense.
** Also added a config.h file.
**
** Revision 1.51  2000/05/27 07:06:03  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.50  2000/05/25 08:16:39  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.49  2000/05/19 10:48:15  prez
** Finalized the DCC Sending (now uses the Actions map properly)
**
** Revision 1.48  2000/05/18 11:41:46  prez
** Fixed minor front-end issues with the filesystem...
**
** Revision 1.47  2000/05/17 14:08:12  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.46  2000/05/17 12:39:55  prez
** Fixed DCC Sending and file lookups (bypassed the DccMap for now).
** Still to fix DCC Receiving.  Looks like a wxFile::Length() issue.
**
** Revision 1.45  2000/05/17 07:47:59  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.44  2000/05/13 07:05:47  prez
** Added displaying of sizes to all file fields..
**
** Revision 1.43  2000/05/10 11:47:00  prez
** added back memo timers
**
** Revision 1.42  2000/05/09 09:11:59  prez
** Added XMLisation to non-mapped structures ... still need to
** do the saving stuff ...
**
** Revision 1.41  2000/05/03 14:12:23  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.40  2000/04/18 14:34:23  prez
** Fixed the HELP system, it now loads into memory, and can be unloaded
** with the OS unload command.  The stats however are inaccurate.
**
** Revision 1.39  2000/04/18 10:20:27  prez
** Made helpfiles load on usage, like language files.
**
** Revision 1.38  2000/04/16 14:29:44  prez
** Added stats for usage, and standardized grabbing paths, etc.
**
** Revision 1.37  2000/04/04 03:21:36  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.36  2000/04/04 03:13:51  prez
** Added support for masking hostnames.
**
** Revision 1.35  2000/03/15 08:23:52  prez
** Added locking stuff for commserv options, and other stuff
**
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
	    "STAT* OPER*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), ServMsg::do_stats_Oper);
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
	    "FILE* LIST*", Parent->commserv.ALL_Name(), ServMsg::do_file_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* VIEW*", Parent->commserv.ALL_Name(), ServMsg::do_file_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* ADD*", Parent->commserv.SOP_Name(), ServMsg::do_file_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* DEL*", Parent->commserv.SOP_Name(), ServMsg::do_file_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* REM*", Parent->commserv.SOP_Name(), ServMsg::do_file_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* REN*", Parent->commserv.SOP_Name(), ServMsg::do_file_Rename);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* PRIV*", Parent->commserv.SOP_Name(), ServMsg::do_file_Priv);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* SEC*", Parent->commserv.SOP_Name(), ServMsg::do_file_Priv);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* SEND*", Parent->commserv.ALL_Name(), ServMsg::do_file_Send);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* DCC*", Parent->commserv.SOP_Name(), ServMsg::do_file_Dcc);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* KILL*", Parent->commserv.SOP_Name(), ServMsg::do_file_Cancel);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* CAN*", Parent->commserv.SOP_Name(), ServMsg::do_file_Cancel);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* LOOK*", Parent->commserv.SADMIN_Name(), ServMsg::do_file_Lookup);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* RES*", Parent->commserv.SADMIN_Name(), ServMsg::do_file_Lookup);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "STAT* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "STAT*", Parent->commserv.REGD_Name(), do_Stats);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE* *", Parent->commserv.ALL_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE*", Parent->commserv.ALL_Name(), do_1_2param);
}

void ServMsg::RemCommands()
{
    NFT("ServMsg::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "HELP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CRED*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CONTRIB*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "BREAKD*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "*MAP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GLOB*", Parent->commserv.ADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "STAT* NICK*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "STAT* CHAN*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "STAT* OPER*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "STAT* OTH*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "STAT* USAGE", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "STAT* ALL*", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* LIST*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* VIEW*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* ADD*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* DEL*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* REM*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* REN*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* SEND*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* LOOK*", Parent->commserv.SADMIN_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* RES*", Parent->commserv.SADMIN_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "STAT* *", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "STAT*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE* *", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE*", Parent->commserv.ALL_Name());
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
    else if (msgtype == "PRIVMSG" &&
	!Parent->commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);

}

void ServMsg::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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

    mstring message  = params.Before(" ").UpperCase();
    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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

    mstring message  = params.Before(" ").UpperCase();
    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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
    mstring out, servername;
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
	    servername = Parent->server.ServerList[downlinks[i]].AltName();
	    if (i<downlinks.size()-1)
	    {
		::send(mynick, source, Parent->getMessage(source, "MISC/BREAKDOWN"),
			(previndent + "|-" + servername).c_str(), lag, users, opers,
			((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		do_BreakDown2(mynick, source, previndent + "| ", downlinks[i]);
	    }
	    else
	    {
		::send(mynick, source, Parent->getMessage(source, "MISC/BREAKDOWN"),
			(previndent + "`-" + servername).c_str(), lag, users, opers,
			((float) users / (float) Parent->nickserv.live.size()) * 100.0);
		do_BreakDown2(mynick, source, previndent + "  ", downlinks[i]);
	    }
	}
    }
}


void ServMsg::do_stats_Nick(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_stats_Nick", (mynick, source, params));

    mstring message = params.Before(" ", 2);
    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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

    mstring message = params.Before(" ", 2);
    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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

    mstring message = params.Before(" ", 2);
    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_MEMO"),
		Parent->memoserv.nick.size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_NEWS"),
		Parent->memoserv.channel.size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_COMM"),
		Parent->commserv.list.size());

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
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD9"),
		Parent->commserv.stats.Lock(),
		Parent->commserv.stats.Unlock());

    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD"),
		Parent->servmsg.GetInternalName().c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD10"),
		Parent->servmsg.stats.Global(),
		Parent->servmsg.stats.Credits());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD11"),
		Parent->servmsg.stats.File_AddDel(),
		Parent->servmsg.stats.File_Send());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OTH_CMD12"),
		Parent->servmsg.stats.File_Change(),
		Parent->servmsg.stats.File_Cancel());
}


void ServMsg::do_stats_Oper(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_stats_Oper", (mynick, source, params));

    mstring message = params.Before(" ", 2);
    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CLONE"),
		Parent->operserv.Clone_size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_AKILL"),
		Parent->operserv.Akill_size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_OPERDENY"),
		Parent->operserv.OperDeny_size());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_IGNORE"),
		Parent->operserv.Ignore_size());

    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CMD"));
    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CMD1"),
		Parent->operserv.stats.Trace(),
		Parent->operserv.stats.Mode());
    if (Parent->server.proto.SVS())
    {
	::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CMD2"),
		Parent->operserv.stats.Qline(),
		Parent->operserv.stats.Unqline());
	::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CMD3"),
		Parent->operserv.stats.Noop(),
		Parent->operserv.stats.Kill());
    }
    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CMD4"),
		Parent->operserv.stats.Ping(),
		Parent->operserv.stats.Update());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CMD5"),
		Parent->operserv.stats.Reload(),
		Parent->operserv.stats.Unload());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CMD6"),
		Parent->operserv.stats.Jupe(),
		Parent->operserv.stats.OnOff());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CMD7"),
		Parent->operserv.stats.Clone(),
		Parent->operserv.stats.Akill());
    ::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CMD8"),
		Parent->operserv.stats.OperDeny(),
		Parent->operserv.stats.Ignore());
    if (Parent->server.proto.SVSHOST())
	::send(mynick, source, Parent->getMessage(source, "STATS/OPER_CMD9"),
		Parent->operserv.stats.Hide());
}


void ServMsg::do_stats_Usage(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_stats_Usage", (mynick, source, params));
    int count;

    mstring message = params.Before(" ", 2);
    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_NS_LIVE"),
		Parent->nickserv.live.size(),
		ToHumanSpace(Parent->nickserv.live.size() * sizeof(mstring) +
		Parent->nickserv.live.size() * sizeof(Nick_Live_t)).c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_CS_LIVE"),
		Parent->chanserv.live.size(),
		ToHumanSpace(Parent->chanserv.live.size() * sizeof(mstring) +
		Parent->chanserv.live.size() * sizeof(Chan_Live_t)).c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_NS_STORED"),
		Parent->nickserv.stored.size(),
		ToHumanSpace(Parent->nickserv.stored.size() * sizeof(mstring) +
		Parent->nickserv.stored.size() * sizeof(Nick_Stored_t)).c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_CS_STORED"),
		Parent->chanserv.stored.size(),
		ToHumanSpace(Parent->chanserv.stored.size() * sizeof(mstring) +
		Parent->chanserv.stored.size() * sizeof(Chan_Stored_t)).c_str());
    map<mstring,list<Memo_t> >::iterator mi;
    for (count = 0, mi=Parent->memoserv.nick.begin();
			mi!=Parent->memoserv.nick.end(); mi++)
	count += mi->second.size();
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_MEMO"),
		Parent->memoserv.nick.size(),
		ToHumanSpace(Parent->memoserv.nick.size() * sizeof(mstring) +
		count * sizeof(Memo_t)).c_str());
    map<mstring,list<News_t> >::iterator ni;
    for (count = 0, ni=Parent->memoserv.channel.begin();
			ni!=Parent->memoserv.channel.end(); ni++)
	count += ni->second.size();
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_NEWS"),
		Parent->memoserv.channel.size(),
		ToHumanSpace(Parent->memoserv.channel.size() * sizeof(mstring) +
		count * sizeof(News_t)).c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_COMMITTEE"),
		Parent->commserv.list.size(),
		ToHumanSpace(Parent->commserv.list.size() * sizeof(mstring) +
		Parent->commserv.list.size() * sizeof(Committee)).c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_OPERSERV"),
		(Parent->operserv.Clone_size() +
		Parent->operserv.Akill_size() +
		Parent->operserv.OperDeny_size() +
		Parent->operserv.Ignore_size()),
		ToHumanSpace(Parent->operserv.Clone_size() * sizeof(*Parent->operserv.Clone) +
		Parent->operserv.Akill_size() * sizeof(*Parent->operserv.Akill) +
		Parent->operserv.OperDeny_size() * sizeof(*Parent->operserv.OperDeny) +
		Parent->operserv.Ignore_size() * sizeof(*Parent->operserv.Ignore)).c_str());
    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_OTHER"),
		Parent->server.ServerList.size(),
		ToHumanSpace(Parent->server.ServerList.size() * sizeof(mstring) +
		Parent->server.ServerList.size() * sizeof(Server)).c_str());

    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_LANGHEAD"));
    set<mstring> seen;
    map<mstring, map<mstring, mstring> >::iterator i;
    map<mstring, map<mstring, vector<triplet<mstring, mstring, mstring> > > >::iterator k;
    map<mstring, vector<triplet<mstring, mstring, mstring> > >::iterator j;
    for (i=Parent->Messages.begin(); i!=Parent->Messages.end(); i++)
    {
	if (Parent->Help.find(i->first) != Parent->Help.end())
	{
	    size_t helpsz = 0;
	    for (j=Parent->Help[i->first].begin(); j!=Parent->Help[i->first].end(); j++)
	    {
		helpsz += j->second.size() * sizeof(j->second);
		helpsz += sizeof(j->first);
	    }
	    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_LANG"),
			i->first.c_str(),
			ToHumanSpace(Parent->Messages[i->first].size() * sizeof(i->second)).c_str(),
			ToHumanSpace(sizeof(i->first) + helpsz).c_str());
	    seen.insert(i->first);
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_LANG"),
			i->first.c_str(),
			ToHumanSpace(Parent->Messages[i->first].size() * sizeof(i->second)).c_str(),
			ToHumanSpace(0).c_str());
	}
     }
     for (k=Parent->Help.begin(); k!=Parent->Help.end(); k++)
     {
	if (seen.find(k->first) == seen.end())
	{
	    size_t helpsz = 0;
	    for (j=Parent->Help[k->first].begin(); j!=Parent->Help[k->first].end(); j++)
	    {
		helpsz += j->second.size() * sizeof(j->second);
		helpsz += sizeof(j->first);
	    }
	    ::send(mynick, source, Parent->getMessage(source, "STATS/USE_LANG"),
			k->first.c_str(), ToHumanSpace(0).c_str(),
			ToHumanSpace(sizeof(k->first) + helpsz).c_str());
	}
    }
}

void ServMsg::do_stats_All(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_stats_All", (mynick, source, params));

    mstring message = params.Before(" ", 2);
    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

    do_Stats(mynick, source, params.ExtractWord(1, " "));
    do_stats_Nick(mynick, source, params);
    do_stats_Channel(mynick, source, params);
    do_stats_Oper(mynick, source, params);
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

    mstring message = params.Before(" ");
    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
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


void ServMsg::do_file_List(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_file_List", (mynick, source, params));

    unsigned int listsize, i, j, k, count;
    bool display;
    mstring mask, priv;

    mstring message  = params.Before(" ", 2).UpperCase();

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	mask = "*";
	listsize = Parent->config.Listsize();
    }
    else if (params.WordCount(" ") < 4)
    {
	mask = params.ExtractWord(3, " ").LowerCase();
	listsize = Parent->config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(3, " ").LowerCase();
	listsize = ACE_OS::atoi(params.ExtractWord(4, " ").c_str());
	if (listsize > Parent->config.Maxlist())
	{
	    mstring output;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/MAXLIST"),
					Parent->config.Maxlist());
	    return;
	}
    }

    vector<unsigned long> filelist = Parent->filesys.GetList(FileMap::Public, source);

    if (!filelist.size())
    {
 	::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY"),
 			Parent->getMessage(source, "LIST/FILES").c_str());
 	return;
    }

    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY_MATCH"),
    		mask.c_str(), Parent->getMessage(source, "LIST/FILES").c_str());

    for (j=0, i=0, count = 0; j < filelist.size(); j++)
    {
	if (Parent->filesys.GetName(FileMap::Public, filelist[j]).LowerCase().Matches(mask))
	{
	    if (i < listsize)
	    {
		if (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
			Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))
		    ::send(mynick, source, "%s (%s) [%s]",
			Parent->filesys.GetName(FileMap::Public, filelist[j]).c_str(),
			ToHumanSpace(Parent->filesys.GetSize(FileMap::Public, filelist[j])).c_str(),
			Parent->filesys.GetPriv(FileMap::Public, filelist[j]).c_str());
		else
		{
		    display = false;
		    priv = Parent->filesys.GetPriv(FileMap::Public, filelist[j]);
		    if (priv.IsEmpty())
			display = true;
		    else
		    {
			for (int k=1; k<=priv.WordCount(" "); k++)
			    if (Parent->commserv.IsList(priv.ExtractWord(k, " ")) &&
				Parent->commserv.list[priv.ExtractWord(k, " ").UpperCase()].IsOn(source))
			    {
				display = true;
				break;
			    }
		    }
		    if (display)
			::send(mynick, source, "%s (%s)",
				Parent->filesys.GetName(FileMap::Public, filelist[j]).c_str(),
				ToHumanSpace(Parent->filesys.GetSize(FileMap::Public, filelist[j])).c_str());
		}
		
		i++;
	    }
	    count++;
	}
    }
    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAYED"),
							i, count);
}


void ServMsg::do_file_Add(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_file_Add", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring priv;
    if (params.WordCount(" ") > 2)
	priv = params.After(" ", 2).UpperCase();

    Parent->servmsg.stats.i_file_AddDel++;
    Parent->nickserv.live[source.LowerCase()].InFlight.Public(mynick, priv);
}


void ServMsg::do_file_Del(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_file_Del", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring file = params.ExtractWord(3, " ");
    unsigned long num = Parent->filesys.GetNum(FileMap::Public, file);

    if (!num)
    {
 	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS"),
 		file.c_str(), Parent->getMessage(source, "LIST/FILES").c_str());
 	return;
    }

    Parent->servmsg.stats.i_file_AddDel++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL"),
    		Parent->filesys.GetName(FileMap::Public, num).c_str(),
    		Parent->getMessage(source,"LIST/FILES").c_str());
    Parent->filesys.EraseFile(FileMap::Public, num);
}


void ServMsg::do_file_Rename(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_file_Rename", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring file    = params.ExtractWord(3, " ");
    mstring newfile = params.ExtractWord(4, " ");
    unsigned long num = Parent->filesys.GetNum(FileMap::Public, file);

    if (!num)
    {
 	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS"),
 		file.c_str(), Parent->getMessage(source, "LIST/FILES").c_str());
 	return;
    }

    Parent->servmsg.stats.i_file_Change++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/CHANGE_TIME"),
    		Parent->filesys.GetName(FileMap::Public, num).c_str(),
    		Parent->getMessage(source, "LIST/FILES").c_str(),
    		newfile.c_str());
    Log(LM_INFO, Parent->getLogMessage("SERVMSG/FILE_RENAME"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	file.c_str(), newfile.c_str());
    Parent->filesys.Rename(FileMap::Public, num, newfile);
}


void ServMsg::do_file_Priv(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_file_Priv", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring file = params.ExtractWord(3, " ");
    mstring priv;
    if (params.WordCount(" ") > 3)
	priv = params.After(" ", 3).UpperCase();
    unsigned long num = Parent->filesys.GetNum(FileMap::Public, file);

    if (!num)
    {
 	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS"),
 		file.c_str(), Parent->getMessage(source, "LIST/FILES").c_str());
 	return;
    }

    Parent->servmsg.stats.i_file_Change++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/CHANGE2_TIME"),
    		Parent->filesys.GetName(FileMap::Public, num).c_str(),
    		Parent->getMessage(source, "LIST/FILES").c_str(),
    		Parent->getMessage(source, "LIST/ACCESS").c_str(),
    		priv.c_str());
    Log(LM_INFO, Parent->getLogMessage("SERVMSG/FILE_PRIV"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	file.c_str(), priv.c_str());
    Parent->filesys.SetPriv(FileMap::Public, num, priv);
}


void ServMsg::do_file_Send(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_file_Send", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring filename = params.ExtractWord(3, " ");
    unsigned long filenum = Parent->filesys.GetNum(FileMap::Public, filename);

    bool display = false;
    mstring priv = Parent->filesys.GetPriv(FileMap::Public, filenum);
    if (priv.IsEmpty())
	display = true;
    else
    {
	for (int k=1; k<=priv.WordCount(" "); k++)
	    if (Parent->commserv.IsList(priv.ExtractWord(k, " ")) &&
		Parent->commserv.list[priv.ExtractWord(k, " ").UpperCase()].IsOn(source))
	    {
		display = true;
		break;
	    }
    }

    if (!(filenum && display))
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS"),
		filename.c_str(), Parent->getMessage(source, "LIST/FILES").c_str());
	return;
    }


    if (!(Parent->files.TempDirSize() == 0 ||
	mFile::DirUsage(Parent->files.TempDir()) <=
	Parent->files.TempDirSize()))
    {
	::send(mynick, source, Parent->getMessage(source, "DCC/NOSPACE2"));
	return;
    }


    filename = Parent->filesys.GetName(FileMap::Public, filenum);
    size_t filesize = Parent->filesys.GetSize(FileMap::Public, filenum);

    Parent->servmsg.stats.i_file_Send++;
    unsigned short port = FindAvailPort();
    ::privmsg(mynick, source, DccEngine::encode("DCC SEND", filename +
		" " + mstring(ultoa(Parent->LocalHost())) + " " +
		mstring(ultoa(port)) + " " + mstring(ultoa(filesize))));
    Parent->dcc->Accept(port, mynick, source, FileMap::Public, filenum);
}


void ServMsg::do_file_Dcc(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_file_Dcc", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();


    if (DccMap::xfers.size())
    {
	::send(mynick, source, Parent->getMessage(source, "DCC/LIST_HEAD"));
	map<unsigned long, DccXfer *>::iterator iter;
	for (iter = DccMap::xfers.begin(); iter != DccMap::xfers.end();
 						iter++)
 	{
	    float speed = (float) iter->second->Average();
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
	    ::send(mynick, source, Parent->getMessage(source, "DCC/LIST"), iter->first,
		((iter->second->Type() == DccXfer::Get) ? 'R' : 'S'),			
		iter->second->Filesize(),
		((float) iter->second->Total() /
			(float) iter->second->Filesize() * 100.0),
		speed, scale, iter->second->Source().c_str(),
		iter->second->Filename().c_str());
 	}
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "DCC/NOACTIVE"));
    }    
}


void ServMsg::do_file_Cancel(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_file_Cancel", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring hexstr = params.ExtractWord(3, " ").LowerCase();

    if (hexstr.Len() != 8)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEHEX"), 8);
	return;
    }
    else
    {
	for (unsigned int i=0; i<hexstr.Len(); i++)
	    if (!mstring("0123456789abcdef").Contains(hexstr[i]))
	    {
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEHEX"), 8);
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEHEX"), 8);
	return;
    }

    if (DccMap::xfers.find(number) == DccMap::xfers.end())
    {
	::send(mynick, source, Parent->getMessage(source, "DCC/NODCCID"),
		number);
    }
    else
    {
	Parent->dcc->Cancel(number);
	Parent->servmsg.stats.i_file_Cancel++;
	::send(mynick, source, Parent->getMessage(source, "DCC/CANCEL"),
		number);
    }
}


void ServMsg::do_file_Lookup(mstring mynick, mstring source, mstring params)
{
    FT("ServMsg::do_file_Lookup", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring type   = params.ExtractWord(3, " ").UpperCase();
    mstring hexstr = params.ExtractWord(4, " ").LowerCase();

    if (hexstr.Len() != 8)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEHEX"), 8);
	return;
    }
    else
    {
	for (unsigned int i=0; i<hexstr.Len(); i++)
	    if (!mstring("0123456789abcdef").Contains(hexstr[i]))
	    {
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEHEX"), 8);
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEHEX"), 8);
	return;
    }

    if (type.Matches("M*A*"))
    {
    	if (Parent->filesys.Exists(FileMap::MemoAttach, number))
    	{
    	    map<mstring, list<Memo_t> >::iterator i;
    	    list<Memo_t>::iterator j;
	    for (i=Parent->memoserv.nick.begin(); i!=Parent->memoserv.nick.end(); i++)
	    {
	    	for(k=1, j=i->second.begin(); j!=i->second.end(); j++, k++)
	    	{
	    	    if (j->File() == number)
	    	    {
			::send(mynick, source, Parent->getMessage(source, "DCC/LOOKUP_MEMOATTACH"),
				number, Parent->filesys.GetName(FileMap::MemoAttach, number).c_str(),
				j->Nick().c_str(), k, j->Sender().c_str(), j->Time().Ago().c_str());
			Log(LM_DEBUG, Parent->getLogMessage("SERVMSG/FILE_LOOKUP"),
				Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
				number, type.c_str());
	  		return;
	    	    }
	    	}
	    }
	}
	::send(mynick, source, Parent->getMessage(source, "DCC/NLOOKUP_MEMOATTACH"),
	    		number);
    }
    else if (type.Matches("PIC*"))
    {
    	if (Parent->filesys.Exists(FileMap::Picture, number))
    	{
    	    map<mstring, Nick_Stored_t >::iterator i;
	    for (i=Parent->nickserv.stored.begin(); i!=Parent->nickserv.stored.end(); i++)
	    {
	    	if (i->second.PicNum() == number)
	    	{
		    ::send(mynick, source, Parent->getMessage(source, "DCC/LOOKUP_PICTURE"),
	  			number, i->second.Name().c_str());
		    Log(LM_DEBUG, Parent->getLogMessage("SERVMSG/FILE_LOOKUP"),
			Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
			number, type.c_str());
	  	    return;
	    	}
	    }
	}
	::send(mynick, source, Parent->getMessage(source, "DCC/NLOOKUP_PICTURE"),
	    		number);
    }
    else if (type.Matches("PUB*"))
    {
    	if (Parent->filesys.Exists(FileMap::Public, number))
    	{
	    ::send(mynick, source, Parent->getMessage(source, "DCC/LOOKUP_PUBLIC"),
	  		number, Parent->filesys.GetName(FileMap::Public, number).c_str(),
	  		Parent->filesys.GetPriv(FileMap::Public, number).c_str());
	    Log(LM_DEBUG, Parent->getLogMessage("SERVMSG/FILE_LOOKUP"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		number, type.c_str());
	    return;
    	}
	::send(mynick, source, Parent->getMessage(source, "DCC/NLOOKUP_PUBLIC"),
	    		number);
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "DCC/NLOOKUP_OTHER"),
			type.c_str());
    }
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
    Log(LM_NOTICE, Parent->getLogMessage("SERVMSG/GLOBAL"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	text.c_str());
}

