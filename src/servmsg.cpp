#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#define RCSID(x,y) const char *rcsid_servmsg_cpp_ ## x () { return y; }
RCSID(servmsg_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.93  2001/11/04 19:23:09  ungod
** fixed up compilation for borland c++ builder
**
** Revision 1.92  2001/11/03 21:02:54  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.91  2001/07/02 03:39:30  prez
** Fixed bug with users sending printf strings (mainly in memos).
**
** Revision 1.90  2001/06/17 09:39:07  prez
** Hopefully some more changes that ensure uptime (mainly to do with locking
** entries in an iterated search, and using copies of data instead of references
** where we can get away with it -- reducing the need to lock the data).
**
** Revision 1.89  2001/06/15 07:20:41  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.88  2001/05/17 19:18:55  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.87  2001/05/06 03:03:08  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.86  2001/05/05 17:33:59  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.85  2001/05/01 14:00:24  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.84  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.83  2001/03/27 07:04:32  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.82  2001/03/20 14:22:15  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.81  2001/03/02 05:24:42  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.80  2001/02/11 07:41:28  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.79  2001/02/03 02:21:35  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.78  2001/01/15 23:31:39  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.77  2001/01/01 05:32:45  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.76  2000/12/29 15:31:55  prez
** Added locking/checking for dcc/events threads.  Also for ACE_Log_Msg
**
** Revision 1.75  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.74  2000/12/21 14:18:18  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.73  2000/12/19 14:26:55  prez
** Bahamut has changed SVSNICK -> MODNICK, so i_SVS has been changed into
** several SVS command text strings, if blank, support isnt there.
**
** Revision 1.72  2000/12/19 07:24:54  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.71  2000/12/10 13:06:12  prez
** Ditched alot of the *toa's since mstring can do it internally now.
**
** Revision 1.70  2000/10/10 11:47:53  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.69  2000/09/30 10:48:09  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.68  2000/09/27 11:21:39  prez
** Added a BURST mode ...
**
** Revision 1.67  2000/09/18 08:17:58  prez
** Intergrated mpatrol into the xml/des sublibs, and did
** some minor fixes as a result of mpatrol.
**
** Revision 1.66  2000/09/09 02:17:49  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.65  2000/09/07 08:13:17  prez
** Fixed some of the erronous messages (SVSHOST, SQLINE, etc).
** Also added CPU statistics and fixed problem with socket deletions.
**
** Revision 1.64  2000/08/28 10:51:40  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.63  2000/08/06 05:27:48  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.62  2000/08/03 13:06:32  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.61  2000/06/27 18:56:59  prez
** Added choosing of keys to configure, also created the keygen,
** and scrambler (so keys are not stored in clear text, even in
** the .h and binary files).  We should be set to do the decryption
** process now, as encryption (except for encryption of db's) is
** all done :)
**
** Revision 1.60  2000/06/25 07:58:50  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.59  2000/06/18 12:49:28  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.58  2000/06/12 06:07:51  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.57  2000/06/11 08:20:12  prez
** More minor bug fixes, godda love testers.
**
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

#include "magick.h"
#include "dccengine.h"

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
	    "STAT* H*LP", Parent->commserv.OPER_Name() + " " +
	    Parent->commserv.SOP_Name(), do_2param);
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
	    "FILE* H*LP", Parent->commserv.ALL_Name(), do_2param);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ASK*", Parent->commserv.ALL_Name(), ServMsg::do_Ask);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "QUEST*", Parent->commserv.ALL_Name(), ServMsg::do_Ask);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "H*LP*OP*", Parent->commserv.ALL_Name(), ServMsg::do_Ask);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "H*LP", Parent->commserv.ALL_Name(), ServMsg::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CRED*", Parent->commserv.ALL_Name(), ServMsg::do_Credits);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CONTRIB*", Parent->commserv.ALL_Name(), ServMsg::do_Contrib);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LANG*", Parent->commserv.REGD_Name(), ServMsg::do_Languages);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "BREAKD*", Parent->commserv.ALL_Name(), ServMsg::do_BreakDown);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*MAP", Parent->commserv.ALL_Name(), ServMsg::do_BreakDown);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GLOB*", Parent->commserv.ADMIN_Name(), ServMsg::do_Global);

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
	    "STAT* H*LP", Parent->commserv.OPER_Name() + " " +
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
	    "FILE* H*LP", Parent->commserv.ALL_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ASK*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "QUEST*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "H*LP*OP*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "H*LP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CRED*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CONTRIB*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LANG*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "BREAKD*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "*MAP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GLOB*", Parent->commserv.ADMIN_Name());

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

void ServMsg::execute(mstring& source, const mstring& msgtype, const mstring& params)
{
    mThread::ReAttach(tt_OtherServ);
    FT("ServMsg::execute", (source, msgtype, params));
    //okay this is the main servmsg command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message
    mstring mynick(Parent->getLname(params.ExtractWord(1, ": ")));
    mstring message(params.After(":"));
    mstring command(message.Before(" "));

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

void ServMsg::do_Help(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (message));
	return;
    }}

    mstring HelpTopic = Parent->servmsg.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector<mstring> help = Parent->getHelp(source, HelpTopic.UpperCase());
					
    unsigned int i;
    for (i=0; i<help.size(); i++)
	::send(mynick, source, help[i]);
}


void ServMsg::do_Credits(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_Credits", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (message));
	return;
    }}

    Parent->servmsg.stats.i_Credits++;
    for (int i=0; credits[i] != "---EOM---"; i++)
	if (credits[i].length())
	    ::sendV(mynick, source, credits[i], mynick.c_str());
	else
	    ::send(mynick, source, " ");
}


void ServMsg::do_Contrib(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_Contrib", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    Parent->servmsg.stats.i_Credits++;
    for (int i=0; contrib[i] != "---EOM---"; i++)
	if (contrib[i].length())
	    ::sendV(mynick, source, contrib[i], mynick.c_str());
	else
	    ::send(mynick, source, " ");
}


void ServMsg::do_Languages(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_Languages", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    set<mstring> langs = mFile::DirList(Parent->files.Langdir(), "*.lng");
    mstring output, val;
    if (langs.size())
    {
	NSEND(mynick, source, "MISC/LANG_LIST");
	set<mstring>::iterator i;
	for (i=langs.begin(); i != langs.end(); i++)
	{
	    if (output.length() > Parent->server.proto.MaxLine())
	    {
		::send(mynick, source, "    " + output);
		output.erase();
	    }
	    val = *i;
	    val.Truncate(val.Find(".", true));
	    if (output.length())
		output += ", ";
	    if (val.UpperCase() == Parent->nickserv.DEF_Language())
		output += IRC_Bold;
	    output += val.UpperCase();
	    if (val.UpperCase() == Parent->nickserv.DEF_Language())
		output += IRC_Off;
	}
	if (output.length())
	    ::send(mynick, source, "    " + output);
    }
    else
    {
	NSEND(mynick, source, "MISC/LANG_NOLIST");
    }
}


void ServMsg::do_BreakDown(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_BreakDown", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    NSEND(mynick, source, "MISC/BREAKDOWN_HEAD");
    mstring out;

    map<mstring,pair<unsigned int,unsigned int> > ServCounts;
    NickServ::live_t::iterator k;
    { RLOCK(("NickServ", "live"));
    for (k=Parent->nickserv.LiveBegin(); k!=Parent->nickserv.LiveEnd(); k++)
    {
	RLOCK2(("NickServ", "live", k->first));
	if (ServCounts.find(k->second.Server().LowerCase()) == ServCounts.end())
	{
	    ServCounts[k->second.Server()] =
	    				pair<unsigned int,unsigned int>(0,0);
	}
	if (!k->second.Name().empty())
	{
	    ServCounts[k->second.Server()].first++;
	    if (k->second.HasMode("o"))
		ServCounts[k->second.Server()].second++;
	}
    }}
    ::sendV(mynick, source, "%-35s  % 3.3fs  %5d (%3d)  %3.2f%%",
	    Parent->startup.Server_Name().LowerCase().c_str(), 0.0,
	    ServCounts[""].first, ServCounts[""].second,
	    100.0 * static_cast<float>(ServCounts[""].first) /
	    static_cast<float>(Parent->nickserv.LiveSize()));
    do_BreakDown2(ServCounts, mynick, source, "", "");
}

void ServMsg::do_BreakDown2(map<mstring,pair<unsigned int,unsigned int> > ServCounts,
	const mstring &mynick, const mstring &source,
	const mstring &previndent, const mstring &server)
{
    FT("ServMsg::do_BreakDown2", (mynick, source, previndent, server));
    vector<mstring> downlinks;
    mstring out, servername;
    unsigned int users, opers;
    float lag;

    if (server.empty())
    {
	Server::list_t::iterator iter;
	RLOCK(("Server", "list"));
	for (iter = Parent->server.ListBegin();
		iter != Parent->server.ListEnd(); iter++)
	{
	    RLOCK2(("Server", "list", iter->first));
	    if (!iter->second.Name().empty() &&
		iter->second.Uplink() == Parent->startup.Server_Name().LowerCase())
		downlinks.push_back(iter->first);
	}
    }
    else
    {
	if (Parent->server.IsList(server))
	    downlinks = Parent->server.GetList(server).Downlinks();
    }

    for (unsigned int i=0; i<downlinks.size(); i++)
    {
	if (Parent->server.IsList(downlinks[i]))
	{
	    users = ServCounts[downlinks[i]].first;
	    opers = ServCounts[downlinks[i]].second;
	    lag = Parent->server.GetList(downlinks[i]).Lag();
	    servername = Parent->server.GetList(downlinks[i]).AltName();
	    if (i<downlinks.size()-1)
	    {
		::sendV(mynick, source, "%-35s  % 3.3fs  %5d (%3d)  %3.2f%%",
			(previndent + "|-" + servername).c_str(), lag, users, opers,
			100.0 * static_cast<float>(users) /
			static_cast<float>(Parent->nickserv.LiveSize()));
		do_BreakDown2(ServCounts, mynick, source, previndent + "| ", downlinks[i]);
	    }
	    else
	    {
		::sendV(mynick, source, "%-35s  % 3.3fs  %5d (%3d)  %3.2f%%",
			(previndent + "`-" + servername).c_str(), lag, users, opers,
			100.0 * static_cast<float>(users) /
			static_cast<float>(Parent->nickserv.LiveSize()));
		do_BreakDown2(ServCounts, mynick, source, previndent + "  ", downlinks[i]);
	    }
	}
    }
}


void ServMsg::do_stats_Nick(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_stats_Nick", (mynick, source, params));

    mstring message = params.Before(" ", 2);
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") > 2 &&
	params.ExtractWord(3, " ").IsSameAs("CLEAR", true) &&
	Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	Parent->commserv.GetList(Parent->commserv.SADMIN_Name()).IsOn(source))
    {
	Parent->nickserv.stats.clear();
	return;
    }

    unsigned long linked = 0, suspended = 0, forbidden = 0;
    NickServ::stored_t::iterator i;
    { RLOCK(("NickServ", "stored"));
    for (i=Parent->nickserv.StoredBegin();
		i!=Parent->nickserv.StoredEnd(); i++)
    {
	RLOCK2(("NickServ", "stored", i->first));
	if (i->second.Forbidden())
	    forbidden++;
	else
	{
	    if (!i->second.Host().empty())
		linked++;
	    if (i->second.Suspended())
		suspended++;
	}
    }}

    SEND(mynick, source, "STATS/NICK_REGD", (
		Parent->nickserv.StoredSize(), linked));
    SEND(mynick, source, "STATS/NICK_DENIED", (
		suspended, forbidden));
    SEND(mynick, source, "STATS/NICK_CMD", (
		ToHumanTime(Parent->nickserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/NICK_CMD1", (
		fmstring("%10d", Parent->nickserv.stats.Register()),
		fmstring("%10d", Parent->nickserv.stats.Drop())));
    SEND(mynick, source, "STATS/NICK_CMD2", (
		fmstring("%10d", Parent->nickserv.stats.Link()),
		fmstring("%10d", Parent->nickserv.stats.Unlink())));
    SEND(mynick, source, "STATS/NICK_CMD3", (
		fmstring("%10d", Parent->nickserv.stats.Host()),
		fmstring("%10d", Parent->nickserv.stats.Identify())));
    SEND(mynick, source, "STATS/NICK_CMD4", (
		fmstring("%10d", Parent->nickserv.stats.Ghost()),
		fmstring("%10d", Parent->nickserv.stats.Recover())));
    SEND(mynick, source, "STATS/NICK_CMD5", (
		fmstring("%10d", Parent->nickserv.stats.Suspend()),
		fmstring("%10d", Parent->nickserv.stats.Unsuspend())));
#ifdef GETPASS
    SEND(mynick, source, "STATS/NICK_CMD6A", (
		fmstring("%10d", Parent->nickserv.stats.Forbid()),
		fmstring("%10d", Parent->nickserv.stats.Getpass())));
#else
    SEND(mynick, source, "STATS/NICK_CMD6B", (
		fmstring("%10d", Parent->nickserv.stats.Forbid()),
		fmstring("%10d", Parent->nickserv.stats.Getpass())));
#endif
    SEND(mynick, source, "STATS/NICK_CMD7", (
		fmstring("%10d", Parent->nickserv.stats.Access()),
		fmstring("%10d", Parent->nickserv.stats.Ignore())));
    SEND(mynick, source, "STATS/NICK_CMD8", (
		fmstring("%10d", Parent->nickserv.stats.Set()),
		fmstring("%10d", Parent->nickserv.stats.NoExpire())));
    SEND(mynick, source, "STATS/NICK_CMD9", (
		fmstring("%10d", Parent->nickserv.stats.Lock()),
		fmstring("%10d", Parent->nickserv.stats.Unlock())));
    SEND(mynick, source, "STATS/NICK_CMD10", (
		fmstring("%10d", Parent->nickserv.stats.SetPicture()),
		fmstring("%10d", Parent->nickserv.stats.Send())));
    Parent->servmsg.stats.i_Stats++;
}


void ServMsg::do_stats_Channel(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_stats_Channel", (mynick, source, params));

    mstring message = params.Before(" ", 2);
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") > 2 &&
	params.ExtractWord(3, " ").IsSameAs("CLEAR", true) &&
	Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	Parent->commserv.GetList(Parent->commserv.SADMIN_Name()).IsOn(source))
    {
	Parent->chanserv.stats.clear();
	return;
    }

    unsigned long suspended = 0, forbidden = 0;
    ChanServ::stored_t::iterator i;
    { RLOCK(("ChanServ", "stored"));
    for (i=Parent->chanserv.StoredBegin();
		i!=Parent->chanserv.StoredEnd(); i++)
    {
	RLOCK2(("ChanServ", "stored", i->first));
	if (i->second.Forbidden())
	    forbidden++;
	else
	{
	    if (i->second.Suspended())
		suspended++;
	}
    }}

    SEND(mynick, source, "STATS/CHAN_REGD", (
		Parent->chanserv.StoredSize()));
    SEND(mynick, source, "STATS/CHAN_DENIED", (
		suspended, forbidden));
    SEND(mynick, source, "STATS/CHAN_CMD", (
		ToHumanTime(Parent->chanserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/CHAN_CMD1", (
		fmstring("%10d", Parent->chanserv.stats.Register()),
		fmstring("%10d", Parent->chanserv.stats.Drop())));
    SEND(mynick, source, "STATS/CHAN_CMD2", (
		fmstring("%10d", Parent->chanserv.stats.Identify())));
    SEND(mynick, source, "STATS/CHAN_CMD3", (
		fmstring("%10d", Parent->chanserv.stats.Suspend()),
		fmstring("%10d", Parent->chanserv.stats.Unsuspend())));
#ifdef GETPASS
    SEND(mynick, source, "STATS/CHAN_CMD4A", (
		fmstring("%10d", Parent->chanserv.stats.Forbid()),
		fmstring("%10d", Parent->chanserv.stats.Getpass())));
#else
    SEND(mynick, source, "STATS/CHAN_CMD4B", (
		fmstring("%10d", Parent->chanserv.stats.Forbid()),
		fmstring("%10d", Parent->chanserv.stats.Getpass())));
#endif
    SEND(mynick, source, "STATS/CHAN_CMD5", (
		fmstring("%10d", Parent->chanserv.stats.Mode()),
		fmstring("%10d", Parent->chanserv.stats.Topic())));
    SEND(mynick, source, "STATS/CHAN_CMD6", (
		fmstring("%10d", Parent->chanserv.stats.Op()),
		fmstring("%10d", Parent->chanserv.stats.Deop())));
    SEND(mynick, source, "STATS/CHAN_CMD7", (
		fmstring("%10d", Parent->chanserv.stats.Halfop()),
		fmstring("%10d", Parent->chanserv.stats.Dehalfop())));
    SEND(mynick, source, "STATS/CHAN_CMD8", (
		fmstring("%10d", Parent->chanserv.stats.Voice()),
		fmstring("%10d", Parent->chanserv.stats.Devoice())));
    SEND(mynick, source, "STATS/CHAN_CMD9", (
		fmstring("%10d", Parent->chanserv.stats.Kick()),
		fmstring("%10d", Parent->chanserv.stats.Anonkick())));
    SEND(mynick, source, "STATS/CHAN_CMD10", (
		fmstring("%10d", Parent->chanserv.stats.Invite()),
		fmstring("%10d", Parent->chanserv.stats.Unban())));
    SEND(mynick, source, "STATS/CHAN_CMD11", (
		fmstring("%10d", Parent->chanserv.stats.Clear()),
		fmstring("%10d", Parent->chanserv.stats.Akick())));
    SEND(mynick, source, "STATS/CHAN_CMD12", (
		fmstring("%10d", Parent->chanserv.stats.Level()),
		fmstring("%10d", Parent->chanserv.stats.Access())));
    SEND(mynick, source, "STATS/CHAN_CMD13", (
		fmstring("%10d", Parent->chanserv.stats.Greet()),
		fmstring("%10d", Parent->chanserv.stats.Message())));
    SEND(mynick, source, "STATS/CHAN_CMD14", (
		fmstring("%10d", Parent->chanserv.stats.Set()),
		fmstring("%10d", Parent->chanserv.stats.NoExpire())));
    SEND(mynick, source, "STATS/CHAN_CMD15", (
		fmstring("%10d", Parent->chanserv.stats.Lock()),
		fmstring("%10d", Parent->chanserv.stats.Unlock())));
    Parent->servmsg.stats.i_Stats++;
}


void ServMsg::do_stats_Other(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_stats_Other", (mynick, source, params));

    mstring message = params.Before(" ", 2);
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") > 2 &&
	params.ExtractWord(3, " ").IsSameAs("CLEAR", true) &&
	Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	Parent->commserv.GetList(Parent->commserv.SADMIN_Name()).IsOn(source))
    {
	Parent->memoserv.stats.clear();
	Parent->commserv.stats.clear();
	Parent->servmsg.stats.clear();
	return;
    }



    SEND(mynick, source, "STATS/OTH_MEMO", (
		Parent->memoserv.NickSize()));
    SEND(mynick, source, "STATS/OTH_NEWS", (
		Parent->memoserv.ChannelSize()));
    SEND(mynick, source, "STATS/OTH_COMM", (
		Parent->commserv.ListSize()));

    SEND(mynick, source, "STATS/OTH_CMD", (
		Parent->memoserv.GetInternalName(),
		ToHumanTime(Parent->operserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/OTH_CMD1", (
		fmstring("%10d", Parent->memoserv.stats.Read()),
		fmstring("%10d", Parent->memoserv.stats.Unread())));
    SEND(mynick, source, "STATS/OTH_CMD2", (
		fmstring("%10d", Parent->memoserv.stats.Send()),
		fmstring("%10d", Parent->memoserv.stats.Flush())));
    SEND(mynick, source, "STATS/OTH_CMD3", (
		fmstring("%10d", Parent->memoserv.stats.Reply()),
		fmstring("%10d", Parent->memoserv.stats.Forward())));
    SEND(mynick, source, "STATS/OTH_CMD4", (
		fmstring("%10d", Parent->memoserv.stats.Cancel()),
		fmstring("%10d", Parent->memoserv.stats.Del())));
    SEND(mynick, source, "STATS/OTH_CMD5", (
		fmstring("%10d", Parent->memoserv.stats.Continue()),
		fmstring("%10d", Parent->memoserv.stats.Set())));
    SEND(mynick, source, "STATS/OTH_CMD6", (
		fmstring("%10d", Parent->memoserv.stats.File()),
		fmstring("%10d", Parent->memoserv.stats.Get())));

    SEND(mynick, source, "STATS/OTH_CMD", (
		Parent->commserv.GetInternalName(),
		ToHumanTime(Parent->operserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/OTH_CMD11", (
		fmstring("%10d", Parent->commserv.stats.Add()),
		fmstring("%10d", Parent->commserv.stats.Del())));
    SEND(mynick, source, "STATS/OTH_CMD12", (
		fmstring("%10d", Parent->commserv.stats.Member()),
		fmstring("%10d", Parent->commserv.stats.Logon())));
    SEND(mynick, source, "STATS/OTH_CMD13", (
		fmstring("%10d", Parent->commserv.stats.Memo()),
		fmstring("%10d", Parent->commserv.stats.Set())));
    SEND(mynick, source, "STATS/OTH_CMD14", (
		fmstring("%10d", Parent->commserv.stats.Lock()),
		fmstring("%10d", Parent->commserv.stats.Unlock())));

    SEND(mynick, source, "STATS/OTH_CMD", (
		Parent->servmsg.GetInternalName(),
		ToHumanTime(Parent->operserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/OTH_CMD21", (
		fmstring("%10d", Parent->servmsg.stats.Global()),
		fmstring("%10d", Parent->servmsg.stats.Credits())));
    SEND(mynick, source, "STATS/OTH_CMD22", (
		fmstring("%10d", Parent->servmsg.stats.Ask()),
		fmstring("%10d", Parent->servmsg.stats.Stats())));
    SEND(mynick, source, "STATS/OTH_CMD23", (
		fmstring("%10d", Parent->servmsg.stats.File_Add()),
		fmstring("%10d", Parent->servmsg.stats.File_Del())));
    SEND(mynick, source, "STATS/OTH_CMD24", (
		fmstring("%10d", Parent->servmsg.stats.File_Priv()),
		fmstring("%10d", Parent->servmsg.stats.File_Rename())));
    SEND(mynick, source, "STATS/OTH_CMD25", (
		fmstring("%10d", Parent->servmsg.stats.File_Send()),
		fmstring("%10d", Parent->servmsg.stats.File_Cancel())));
    Parent->servmsg.stats.i_Stats++;
}


void ServMsg::do_stats_Oper(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_stats_Oper", (mynick, source, params));

    mstring message = params.Before(" ", 2);
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") > 2 &&
	params.ExtractWord(3, " ").IsSameAs("CLEAR", true) &&
	Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	Parent->commserv.GetList(Parent->commserv.SADMIN_Name()).IsOn(source))
    {
	Parent->operserv.stats.clear();
	return;
    }


    SEND(mynick, source, "STATS/OPER_CLONE", (
		Parent->operserv.Clone_size()));
    SEND(mynick, source, "STATS/OPER_AKILL", (
		Parent->operserv.Akill_size()));
    SEND(mynick, source, "STATS/OPER_OPERDENY", (
		Parent->operserv.OperDeny_size()));
    SEND(mynick, source, "STATS/OPER_IGNORE", (
		Parent->operserv.Ignore_size()));

    SEND(mynick, source, "STATS/OPER_CMD", (
		ToHumanTime(Parent->operserv.stats.ClearTime().SecondsSince())));
    SEND(mynick, source, "STATS/OPER_CMD1", (
		fmstring("%10d", Parent->operserv.stats.Trace()),
		fmstring("%10d", Parent->operserv.stats.Mode())));
    if (!(Parent->server.proto.SQLINE().empty() ||
	Parent->server.proto.UNSQLINE().empty()))
    {
	SEND(mynick, source, "STATS/OPER_CMD2", (
		fmstring("%10d", Parent->operserv.stats.Qline()),
		fmstring("%10d", Parent->operserv.stats.Unqline())));
    }
    if (!(Parent->server.proto.SVSNOOP().empty() ||
	Parent->server.proto.SVSKILL().empty()))
    {
	SEND(mynick, source, "STATS/OPER_CMD3", (
		fmstring("%10d", Parent->operserv.stats.Noop()),
		fmstring("%10d", Parent->operserv.stats.Kill())));
    }
    SEND(mynick, source, "STATS/OPER_CMD4", (
		fmstring("%10d", Parent->operserv.stats.Ping()),
		fmstring("%10d", Parent->operserv.stats.Update())));
    SEND(mynick, source, "STATS/OPER_CMD5", (
		fmstring("%10d", Parent->operserv.stats.Reload()),
		fmstring("%10d", Parent->operserv.stats.Unload())));
    SEND(mynick, source, "STATS/OPER_CMD6", (
		fmstring("%10d", Parent->operserv.stats.Jupe()),
		fmstring("%10d", Parent->operserv.stats.OnOff())));
    SEND(mynick, source, "STATS/OPER_CMD7", (
		fmstring("%10d", Parent->operserv.stats.Clone()),
		fmstring("%10d", Parent->operserv.stats.Akill())));
    SEND(mynick, source, "STATS/OPER_CMD8", (
		fmstring("%10d", Parent->operserv.stats.OperDeny()),
		fmstring("%10d", Parent->operserv.stats.Ignore())));
    if (!Parent->server.proto.SVSHOST().empty())
	SEND(mynick, source, "STATS/OPER_CMD9", (
		fmstring("%10d", Parent->operserv.stats.Hide())));
    Parent->servmsg.stats.i_Stats++;
}


void ServMsg::do_stats_Usage(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_stats_Usage", (mynick, source, params));
    int count;
    size_t size;

    mstring message = params.Before(" ", 2);
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    {
	rusage tmp;
	ACE_OS::getrusage(RUSAGE_SELF, &tmp);
	ACE_Time_Value user, sys;
	user = tmp.ru_utime;
	sys  = tmp.ru_stime;
	SEND(mynick, source, "STATS/USE_CPU", (
		((sys.sec() == 0) ?
			Parent->getMessage(source, "VALS/TIME_NONE") :
			ToHumanTime(sys.sec(), source)),
		((user.sec() == 0) ?
			Parent->getMessage(source, "VALS/TIME_NONE") :
			ToHumanTime(user.sec(), source))));
	}

	{ RLOCK(("IrcSvcHandler"));
	if (Parent->ircsvchandler != NULL)
	{
	SEND(mynick, source, "STATS/USE_TRAFFIC", (
		ToHumanSpace(Parent->ircsvchandler->In_Traffic()),
		ToHumanSpace(Parent->ircsvchandler->In_Traffic() /
		Parent->ircsvchandler->Connect_Time().SecondsSince()),
		ToHumanSpace(Parent->ircsvchandler->Out_Traffic()),
		ToHumanSpace(Parent->ircsvchandler->Out_Traffic() /
		Parent->ircsvchandler->Connect_Time().SecondsSince()),
		ToHumanTime(Parent->ircsvchandler->Connect_Time().SecondsSince(), source)));
	}}

	size = 0;
	NickServ::live_t::iterator i;
	{ RLOCK(("NickServ", "live"));
	for (i=Parent->nickserv.LiveBegin(); i!=Parent->nickserv.LiveEnd(); i++)
	{
	RLOCK2(("NickServ", "live", i->first));
	size += i->first.capacity();
	size += i->second.Usage();
    }}
    SEND(mynick, source, "STATS/USE_NS_LIVE", (
	fmstring("%5d", Parent->nickserv.LiveSize()),ToHumanSpace(size)));
    size = 0;
    ChanServ::live_t::iterator j;
    { RLOCK(("ChanServ", "live"));
    for (j=Parent->chanserv.LiveBegin(); j!=Parent->chanserv.LiveEnd(); j++)
    {
	RLOCK(("ChanServ", "live", j->first));
	size += j->first.capacity();
	size += j->second.Usage();
    }}
    SEND(mynick, source, "STATS/USE_CS_LIVE", (
	fmstring("%5d", Parent->chanserv.LiveSize()), ToHumanSpace(size)));
    size = 0;
    NickServ::stored_t::iterator k;
    { RLOCK(("NickServ", "stored"));
    for (k=Parent->nickserv.StoredBegin(); k!=Parent->nickserv.StoredEnd(); k++)
    {
	RLOCK2(("NickServ", "stored", i->first));
	size += k->first.capacity();
	size += k->second.Usage();
    }}
    SEND(mynick, source, "STATS/USE_NS_STORED", (
	fmstring("%5d", Parent->nickserv.StoredSize()), ToHumanSpace(size)));
    size = 0;
    ChanServ::stored_t::iterator l;
    { RLOCK(("ChanServ", "stored"));
    for (l=Parent->chanserv.StoredBegin(); l!=Parent->chanserv.StoredEnd(); l++)
    {
	RLOCK2(("chanServ", "stored", i->first));
	size += l->first.capacity();
	size += l->second.Usage();
    }}
    SEND(mynick, source, "STATS/USE_CS_STORED", (
	fmstring("%5d", Parent->chanserv.StoredSize()), ToHumanSpace(size)));

    size = 0;
    MemoServ::nick_t::iterator m1;
    MemoServ::nick_memo_t::iterator m2;
    { RLOCK(("MemoServ", "nick"));
    for (count = 0, m1=Parent->memoserv.NickBegin();
			m1!=Parent->memoserv.NickEnd(); m1++)
    {
	{ RLOCK2(("MemoServ", "nick", m1->first));
	size += m1->first.capacity();
	count += m1->second.size();
	for (m2 = m1->second.begin(); m2 != m1->second.end(); m2++)
	{
	    size += m2->Usage();
	}}
    }}
    SEND(mynick, source, "STATS/USE_MEMO", (
	fmstring("%5d", count), ToHumanSpace(size)));

    size = 0;
    MemoServ::channel_t::iterator n1;
    MemoServ::channel_news_t::iterator n2;
    { RLOCK(("MemoServ", "channel"));
    for (count = 0, n1=Parent->memoserv.ChannelBegin();
			n1!=Parent->memoserv.ChannelEnd(); n1++)
    {
	size += n1->first.capacity();
	{ RLOCK2(("MemoServ", "channel", n1->first));
	count += n1->second.size();
	for (n2 = n1->second.begin(); n2 != n1->second.end(); n2++)
	{
	    size += n2->Usage();
	}}
    }}
    SEND(mynick, source, "STATS/USE_NEWS", (
	fmstring("%5d", count), ToHumanSpace(size)));

    size = 0;
    CommServ::list_t::iterator o;
    { RLOCK(("CommServ", "list"));
    for (o=Parent->commserv.ListBegin(); o!=Parent->commserv.ListEnd(); o++)
    {
	RLOCK2(("CommServ", "list", o->first));
	size += o->first.capacity();
	size += o->second.Usage();
    }}
    SEND(mynick, source, "STATS/USE_COMMITTEE", (
	fmstring("%5d", Parent->commserv.ListSize()), ToHumanSpace(size)));


    SEND(mynick, source, "STATS/USE_OPERSERV", (
		fmstring("%5d", (Parent->operserv.Clone_size() +
		Parent->operserv.Akill_size() +
		Parent->operserv.OperDeny_size() +
		Parent->operserv.Ignore_size())),
		ToHumanSpace(Parent->operserv.Clone_Usage() +
		Parent->operserv.Akill_Usage() +
		Parent->operserv.OperDeny_Usage() +
		Parent->operserv.Ignore_Usage())));

    size = 0;
    Server::list_t::iterator p;
    { RLOCK(("Server", "list"));
    for (p=Parent->server.ListBegin(); p!=Parent->server.ListEnd(); p++)
    {
	RLOCK2(("Server", "list", p->first));
	size += p->first.capacity();
	size += p->second.Usage();
    }}
    SEND(mynick, source, "STATS/USE_OTHER", (
	fmstring("%5d", Parent->server.ListSize()), ToHumanSpace(size)));

    NSEND(mynick, source, "STATS/USE_LANGHEAD");

    set<mstring> tmp, lang;
    set<mstring>::iterator iter;
    tmp.clear(); tmp = Parent->LNG_Loaded();
    for (iter=tmp.begin(); iter!=tmp.end(); iter++)
	lang.insert(*iter);
    tmp.clear(); tmp = Parent->HLP_Loaded();
    for (iter=tmp.begin(); iter!=tmp.end(); iter++)
	lang.insert(*iter);

    set<mstring>::iterator q;
    for (q=lang.begin(); q!=lang.end(); q++)
    {
	::sendV(mynick, source, "%-20s %7s  %7s",
		q->c_str(), ToHumanSpace(Parent->LNG_Usage(*q)).c_str(),
		ToHumanSpace(Parent->HLP_Usage(*q)).c_str());
	
    }
    Parent->servmsg.stats.i_Stats++;
}

void ServMsg::do_stats_All(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_stats_All", (mynick, source, params));

    mstring message = params.Before(" ", 2);
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    do_Stats(mynick, source, params.Before(" "));
    do_stats_Nick(mynick, source, params);
    do_stats_Channel(mynick, source, params);
    do_stats_Oper(mynick, source, params);
    do_stats_Other(mynick, source, params);
    Parent->servmsg.stats.i_Stats -= 5;
    do_stats_Usage(mynick, source, params);
}

void ServMsg::do_Stats(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_Stats", (mynick, source, params));

    if (params.WordCount(" ") > 1 &&
	((Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source)) ||
	 (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.GetList(Parent->commserv.SOP_Name()).IsOn(source))))
    {
	do_1_2param(mynick, source, params);
	return;
    }

    mstring message = params.Before(" ");
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    SEND(mynick, source, "STATS/GEN_UPTIME", (
		StartTime.Ago()));
    if (StartTime != Parent->ResetTime())
	SEND(mynick, source, "STATS/GEN_RESET", (
		Parent->ResetTime().Ago()));
    SEND(mynick, source, "STATS/GEN_MAXUSERS", (
		Parent->server.UserMax()));
    size_t opers = 0;
    NickServ::live_t::iterator k;
    { RLOCK(("NickServ", "live"));
    for (k=Parent->nickserv.LiveBegin(); k!=Parent->nickserv.LiveEnd(); k++)
    {
	RLOCK2(("NickServ", "live", k->first));
	if (k->second.HasMode("o"))
		opers++;
    }}
    SEND(mynick, source, "STATS/GEN_USERS", (
		Parent->nickserv.LiveSize(), opers));

    if ((Parent->operserv.CloneList_size() - Parent->operserv.CloneList_size(1)))
	SEND(mynick, source, "STATS/GEN_CLONES", (
		Parent->operserv.CloneList_sum() - Parent->operserv.CloneList_size(),
		Parent->operserv.CloneList_size() - Parent->operserv.CloneList_size(1)));
    Parent->servmsg.stats.i_Stats++;
}


void ServMsg::do_file_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_file_List", (mynick, source, params));

    unsigned int listsize, i, j, count;
    bool display;
    mstring mask, priv;

    mstring message  = params.Before(" ", 2).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

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
	listsize = atoi(params.ExtractWord(4, " ").c_str());
	if (listsize > Parent->config.Maxlist())
	{
	    mstring output;
	    SEND(mynick, source, "LIST/MAXLIST", (
					Parent->config.Maxlist()));
	    return;
	}
    }

    vector<unsigned long> filelist = Parent->filesys.GetList(FileMap::Public, source);

    if (!filelist.size())
    {
 	SEND(mynick, source, "LIST/EMPTY", (
 			Parent->getMessage(source, "LIST/FILES")));
 	return;
    }

    SEND(mynick, source, "LIST/DISPLAY_MATCH", (
    		mask, Parent->getMessage(source, "LIST/FILES")));

    bool issop = (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
		Parent->commserv.GetList(Parent->commserv.SOP_Name()).IsOn(source));

    for (j=0, i=0, count = 0; j < filelist.size(); j++)
    {
	if (Parent->filesys.GetName(FileMap::Public, filelist[j]).Matches(mask, true))
	{
	    if (i < listsize)
	    {
		if (issop)
		    ::sendV(mynick, source, "%s (%s) [%s]",
			Parent->filesys.GetName(FileMap::Public, filelist[j]).c_str(),
			ToHumanSpace(Parent->filesys.GetSize(FileMap::Public, filelist[j])).c_str(),
			Parent->filesys.GetPriv(FileMap::Public, filelist[j]).c_str());
		else
		{
		    display = false;
		    priv = Parent->filesys.GetPriv(FileMap::Public, filelist[j]);
		    if (priv.empty())
			display = true;
		    else
		    {
			for (unsigned int k=1; k<=priv.WordCount(" "); k++)
			    if (Parent->commserv.IsList(priv.ExtractWord(k, " ")) &&
				Parent->commserv.GetList(priv.ExtractWord(k, " ")).IsOn(source))
			    {
				display = true;
				break;
			    }
		    }
		    if (display)
			::sendV(mynick, source, "%s (%s)",
				Parent->filesys.GetName(FileMap::Public, filelist[j]).c_str(),
				ToHumanSpace(Parent->filesys.GetSize(FileMap::Public, filelist[j])).c_str());
		}
		
		i++;
	    }
	    count++;
	}
    }
    SEND(mynick, source, "LIST/DISPLAYED", (
							i, count));
}


void ServMsg::do_file_Add(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_file_Add", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring priv;
    if (params.WordCount(" ") > 2)
	priv = params.After(" ", 2).UpperCase();

    Parent->servmsg.stats.i_file_Add++;
    Parent->nickserv.GetLive(source).InFlight.Public(mynick, priv);
}


void ServMsg::do_file_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_file_Del", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring file = params.ExtractWord(3, " ");
    unsigned long num = Parent->filesys.GetNum(FileMap::Public, file);

    if (!num)
    {
 	SEND(mynick, source, "LIST/NOTEXISTS", (
 		file, Parent->getMessage(source, "LIST/FILES")));
 	return;
    }

    Parent->servmsg.stats.i_file_Del++;
    SEND(mynick, source, "LIST/DEL", (
    		Parent->filesys.GetName(FileMap::Public, num),
    		Parent->getMessage(source,"LIST/FILES")));
    Parent->filesys.EraseFile(FileMap::Public, num);
}


void ServMsg::do_file_Rename(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_file_Rename", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring file    = params.ExtractWord(3, " ");
    mstring newfile = params.ExtractWord(4, " ");
    unsigned long num = Parent->filesys.GetNum(FileMap::Public, file);

    if (!num)
    {
 	SEND(mynick, source, "LIST/NOTEXISTS", (
 		file, Parent->getMessage(source, "LIST/FILES")));
 	return;
    }

    Parent->servmsg.stats.i_file_Rename++;
    SEND(mynick, source, "LIST/CHANGE_TIME", (
    		Parent->filesys.GetName(FileMap::Public, num),
    		Parent->getMessage(source, "LIST/FILES"),
    		newfile));
    LOG(LM_INFO, "SERVMSG/FILE_RENAME", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	file, newfile));
    Parent->filesys.Rename(FileMap::Public, num, newfile);
}


void ServMsg::do_file_Priv(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_file_Priv", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring file = params.ExtractWord(3, " ");
    mstring priv;
    if (params.WordCount(" ") > 3)
	priv = params.After(" ", 3).UpperCase();
    unsigned long num = Parent->filesys.GetNum(FileMap::Public, file);

    if (!num)
    {
 	SEND(mynick, source, "LIST/NOTEXISTS", (
 		file, Parent->getMessage(source, "LIST/FILES")));
 	return;
    }

    Parent->servmsg.stats.i_file_Priv++;
    SEND(mynick, source, "LIST/CHANGE2_TIME", (
    		Parent->filesys.GetName(FileMap::Public, num),
    		Parent->getMessage(source, "LIST/FILES"),
    		Parent->getMessage(source, "LIST/ACCESS"),
    		priv));
    LOG(LM_INFO, "SERVMSG/FILE_PRIV", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	file, ((priv.empty()) ? "ALL" : priv.c_str())));
    Parent->filesys.SetPriv(FileMap::Public, num, priv);
}


void ServMsg::do_file_Send(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_file_Send", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring filename = params.ExtractWord(3, " ");
    unsigned long filenum = Parent->filesys.GetNum(FileMap::Public, filename);

    bool display = false;
    mstring priv = Parent->filesys.GetPriv(FileMap::Public, filenum);
    if (priv.empty())
	display = true;
    else
    {
	for (unsigned int k=1; k<=priv.WordCount(" "); k++)
	    if (Parent->commserv.IsList(priv.ExtractWord(k, " ")) &&
		Parent->commserv.GetList(priv.ExtractWord(k, " ")).IsOn(source))
	    {
		display = true;
		break;
	    }
    }

    if (!(filenum && display))
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (
		filename, Parent->getMessage(source, "LIST/FILES")));
	return;
    }

    filename = Parent->filesys.GetName(FileMap::Public, filenum);
    size_t filesize = Parent->filesys.GetSize(FileMap::Public, filenum);
    if (filename.empty() || filesize <= 0)
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (
		filename, Parent->getMessage(source, "LIST/FILES")));
	return;
    }

    if (!(Parent->files.TempDirSize() == 0 ||
	mFile::DirUsage(Parent->files.TempDir()) <=
	Parent->files.TempDirSize()))
    {
	NSEND(mynick, source, "DCC/NOSPACE2");
	return;
    }

    { RLOCK(("DCC"));
    if (Parent->dcc != NULL)
    {
	Parent->servmsg.stats.i_file_Send++;
	LOG(LM_INFO, "SERVMSG/FILE_SEND", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		filename));
	unsigned short port = mSocket::FindAvailPort();
	::privmsg(mynick, source, DccEngine::encode("DCC SEND", filename +
		" " + mstring(Parent->LocalHost()) + " " +
		mstring(port) + " " + mstring(filesize)));
	Parent->dcc->Accept(port, mynick, source, FileMap::Public, filenum);
    }}
}


void ServMsg::do_file_Dcc(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_file_Dcc", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();

    if (DccMap::XfersSize())
    {
	NSEND(mynick, source, "DCC/LIST_HEAD");
	DccMap::xfers_t::iterator iter;
	RLOCK(("DccMap", "xfers"));
	for (iter = DccMap::XfersBegin(); iter != DccMap::XfersEnd();
 						iter++)
 	{
	    RLOCK2(("DccMap", "xfers", iter->first));
	    if (iter->second == NULL)
		continue;

	    float speed = static_cast<float>(iter->second->Average());
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
		((iter->second->Type() == DccXfer::Get) ? 'R' : 'S'),			
		iter->second->Filesize(),
		100.0 * static_cast<float>(iter->second->Total()) /
			static_cast<float>(iter->second->Filesize()),
		speed, scale, iter->second->Source().c_str(),
		iter->second->Filename().c_str());
 	}
    }
    else
    {
	NSEND(mynick, source, "DCC/NOACTIVE");
    }    
}


void ServMsg::do_file_Cancel(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_file_Cancel", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring hexstr = params.ExtractWord(3, " ").LowerCase();

    if (hexstr.length() != 8)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", ( 8));
	return;
    }
    else
    {
	for (unsigned int i=0; i<hexstr.length(); i++)
	    if (!mstring("0123456789abcdef").Contains(hexstr[i]))
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", ( 8));
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
	SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", ( 8));
	return;
    }

    RLOCK(("DccMap", "xfers"));
    if (!DccMap::IsXfers(number))
    {
	SEND(mynick, source, "DCC/NODCCID", (
		fmstring("%08x", number)));
    }
    else
    {
	{ RLOCK2(("DCC"));
	if (Parent->dcc != NULL)
	{
	    Parent->dcc->Cancel(number);
	    Parent->servmsg.stats.i_file_Cancel++;
	    SEND(mynick, source, "DCC/CANCEL", (
		fmstring("%08x", number)));
	}}
    }
}


void ServMsg::do_file_Lookup(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_file_Lookup", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring type   = params.ExtractWord(3, " ").UpperCase();
    mstring hexstr = params.ExtractWord(4, " ").LowerCase();

    if (hexstr.length() != 8)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", ( 8));
	return;
    }
    else
    {
	for (unsigned int i=0; i<hexstr.length(); i++)
	    if (!mstring("0123456789abcdef").Contains(hexstr[i]))
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", ( 8));
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
	SEND(mynick, source, "ERR_SYNTAX/MUSTBEHEX", ( 8));
	return;
    }

    if (type.Matches("M*A*", true))
    {
	type = "MemoAttach";
    	if (Parent->filesys.Exists(FileMap::MemoAttach, number))
    	{
    	    MemoServ::nick_t::iterator i;
    	    MemoServ::nick_memo_t::iterator j;
	    RLOCK(("MemoServ", "nick"));
	    for (i=Parent->memoserv.NickBegin(); i!=Parent->memoserv.NickEnd(); i++)
	    {
		RLOCK2(("MemoServ", "nick", i->first));
	    	for(k=1, j=i->second.begin(); j!=i->second.end(); j++, k++)
	    	{
	    	    if (j->File() == number)
	    	    {
			SEND(mynick, source, "DCC/LOOKUP_MEMOATTACH", (
				fmstring("%08x", number),
				Parent->filesys.GetName(FileMap::MemoAttach, number),
				j->Nick(), k, j->Sender(), j->Time().Ago()));
			LOG(LM_DEBUG, "SERVMSG/FILE_LOOKUP", (
				Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
				fmstring("%08x", number), type));
	  		return;
	    	    }
	    	}
	    }
	}
	SEND(mynick, source, "DCC/NLOOKUP_MEMOATTACH", (
	    		fmstring("%08x", number)));
    }
    else if (type.Matches("PIC*", true))
    {
	type = "Picture";
    	if (Parent->filesys.Exists(FileMap::Picture, number))
    	{
    	    map<mstring, Nick_Stored_t >::iterator i;
	    RLOCK(("NickServ", "stored"));
	    for (i=Parent->nickserv.StoredBegin(); i!=Parent->nickserv.StoredEnd(); i++)
	    {
		RLOCK2(("NickServ", "stored", i->first));
	    	if (i->second.PicNum() == number)
	    	{
		    SEND(mynick, source, "DCC/LOOKUP_PICTURE", (
	  		fmstring("%08x", number), i->second.Name()));
		    LOG(LM_DEBUG, "SERVMSG/FILE_LOOKUP", (
			Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
			fmstring("%08x", number), type));
	  	    return;
	    	}
	    }
	}
	SEND(mynick, source, "DCC/NLOOKUP_PICTURE", (
	    		fmstring("%08x", number)));
    }
    else if (type.Matches("PUB*", true))
    {
	type = "Public";
    	if (Parent->filesys.Exists(FileMap::Public, number))
    	{
	    SEND(mynick, source, "DCC/LOOKUP_PUBLIC", (
	  		fmstring("%08x", number),
			Parent->filesys.GetName(FileMap::Public, number),
	  		Parent->filesys.GetPriv(FileMap::Public, number)));
	    LOG(LM_DEBUG, "SERVMSG/FILE_LOOKUP", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		fmstring("%08x", number), type));
	    return;
    	}
	SEND(mynick, source, "DCC/NLOOKUP_PUBLIC", (
	    		fmstring("%08x", number)));
    }
    else
    {
	SEND(mynick, source, "DCC/NLOOKUP_OTHER", (
			type));
    }
}


void ServMsg::do_Global(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_Global", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }
    mstring text = params.After(" ");

    Server::list_t::iterator iter;
    RLOCK(("Server", "list"));
    for (iter=Parent->server.ListBegin();
			iter != Parent->server.ListEnd(); iter++)
    {
	RLOCK2(("Server", "list", iter->first));
	Parent->server.NOTICE(Parent->servmsg.FirstName(), "$" +
						    iter->first, text);
    }
    Parent->servmsg.stats.i_Global++;
    ANNOUNCE(mynick, "MISC/GLOBAL_MSG", (
				source));
    LOG(LM_NOTICE, "SERVMSG/GLOBAL", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H), text));
}

void ServMsg::do_Ask(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ServMsg::do_Ask", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (!Parent->server.proto.Helpops())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }
    mstring text = params.After(" ");

    Parent->servmsg.stats.i_Ask++;
    Parent->server.HELPOPS(mynick, source + " (ASK) - " + text);
}

