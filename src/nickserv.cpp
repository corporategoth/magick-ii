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
#define RCSID(x,y) const char *rcsid_nickserv_cpp_ ## x () { return y; }
RCSID(nickserv_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.170  2001/05/06 03:03:07  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.169  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.168  2001/05/05 06:04:01  prez
** Fixed nick join stuff ...
**
** Revision 1.167  2001/05/04 03:43:33  prez
** Fixed UMODE problems (re-oper) and problems in mstring erase
**
** Revision 1.166  2001/05/03 22:34:35  prez
** Fixed SQUIT protection ...
**
** Revision 1.165  2001/05/01 14:00:24  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.164  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.163  2001/03/27 19:16:03  prez
** Fixed Chan_Stored_t::ChgNick (had problems coz nick isnt fully changed yet)
**
** Revision 1.162  2001/03/27 16:09:43  prez
** Fixed chanserv internal maps problem (inserted with incorrect case)
**
** Revision 1.161  2001/03/27 07:04:32  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.160  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.159  2001/03/08 08:07:41  ungod
** fixes for bcc 5.5
**
** Revision 1.158  2001/03/04 02:04:14  prez
** Made mstring a little more succinct ... and added vector/list operations
**
** Revision 1.157  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.156  2001/02/11 07:41:28  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.155  2001/02/03 02:21:34  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.154  2001/01/16 15:47:40  prez
** Fixed filesys not generating first entry in maps, fixed chanserv level
** changes (could confuse set) and fixed idle times on whois user user
**
** Revision 1.153  2001/01/15 23:31:39  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.152  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.151  2001/01/01 00:43:25  prez
** Make services realise when they're +o
**
** Revision 1.150  2000/12/31 17:54:29  prez
** Added checking to see if 'http://' was entered in the SET URL commands.
**
** Revision 1.149  2000/12/29 15:31:55  prez
** Added locking/checking for dcc/events threads.  Also for ACE_Log_Msg
**
** Revision 1.148  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.147  2000/12/22 19:50:19  prez
** Made all config options const.  Beginnings of securing all non-modifying
** commands to const.  also added serviceschk.
**
** Revision 1.146  2000/12/22 08:55:41  prez
** Made forbidden entries (chanserv or nickserv) show up as forbidden in
** a list (rather than (nick!) or whatever)
**
** Revision 1.145  2000/12/22 08:15:29  prez
** Fixed bug that created a blank entry in the stored nick list
**
** Revision 1.144  2000/12/22 03:30:26  prez
** Fixed bug in nickserv ident.
**
** Revision 1.143  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.142  2000/12/19 14:26:55  prez
** Bahamut has changed SVSNICK -> MODNICK, so i_SVS has been changed into
** several SVS command text strings, if blank, support isnt there.
**
** Revision 1.141  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.140  2000/12/10 13:06:12  prez
** Ditched alot of the *toa's since mstring can do it internally now.
**
** Revision 1.139  2000/12/09 16:45:37  prez
** Fixed rfind in mstring.
**
** Revision 1.138  2000/12/09 15:40:13  prez
** Fixed some stuff with mstring (ie. Contains called find_first_of
** not find, and the makeupper/makelower calls had != NULL not == NULL).
**
** Revision 1.137  2000/10/10 11:47:52  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.136  2000/09/30 10:48:08  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.135  2000/09/22 12:26:11  prez
** Fixed that pesky bug with chanserv not seeing modes *sigh*
**
** Revision 1.134  2000/09/18 08:17:57  prez
** Intergrated mpatrol into the xml/des sublibs, and did
** some minor fixes as a result of mpatrol.
**
** Revision 1.133  2000/09/12 21:17:02  prez
** Added IsLiveAll (IsLive now checks to see if user is SQUIT).
**
** Revision 1.132  2000/09/10 09:53:43  prez
** Added functionality to ensure the order of messages is kept.
**
** Revision 1.131  2000/09/09 02:17:48  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.130  2000/09/05 10:53:07  prez
** Only have operserv.cpp and server.cpp to go with T_Changing / T_Modify
** tracing -- also modified keygen to allow for cmdline generation (ie.
** specify 1 option and enter keys, or 2 options and the key is read from
** a file).  This allows for paragraphs with \n's in them, and helps so you
** do not have to type out 1024 bytes :)
**
** Revision 1.129  2000/09/01 10:54:38  prez
** Added Changing and implemented Modify tracing, now just need to create
** DumpB() and DumpE() functions in all classes, and put MCB() / MCE() calls
** (or MB() / ME() or CB() / CE() where MCB() / MCE() not appropriate) in.
**
** Revision 1.128  2000/08/28 10:51:38  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.127  2000/08/19 14:45:03  prez
** Fixed mode settings upon commitee recognitition syntax checking
**
** Revision 1.126  2000/08/19 10:59:47  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.125  2000/08/10 23:59:03  prez
** Fixed REC_FORNOTINCHAN
**
** Revision 1.124  2000/08/10 22:44:24  prez
** Added 'binding to IP' options for shell servers, etc.  Also added akick
** triggers for when a user changes their nick and suddenly matches akick.
**
** Revision 1.123  2000/08/09 12:14:43  prez
** Ensured chanserv infinate loops wont occur, added 2 new cmdline
** paramaters, and added a manpage (you need to perl2pod it tho).
**
** Revision 1.122  2000/08/08 09:58:56  prez
** Added ModeO to 4 pre-defined committees.
** Also added back some deletes in xml in the hope that it
** will free up some memory ...
**
** Revision 1.121  2000/08/07 12:20:28  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.120  2000/08/06 21:56:14  prez
** Fixed some small problems in akill/clone protection
**
** Revision 1.119  2000/08/06 08:06:41  prez
** Fixed loading of logon messages in committee ..
**
** Revision 1.118  2000/08/06 05:27:47  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.117  2000/08/03 13:06:31  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.116  2000/08/02 20:08:57  prez
** Minor code cleanups, added ACE installation instructions, updated the
** suggestions file and stopped people doing a whole bunch of stuff to
** forbidden nicknames.
**
** Revision 1.115  2000/07/30 09:04:06  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.114  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.113  2000/07/21 00:18:49  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.112  2000/06/28 12:20:48  prez
** Lots of encryption stuff, but essentially, we now have random
** key generation for the keyfile keys, and we can actually encrypt
** something, and get it back as we sent it in (specifically, the
** keyfile itself).
**
** Revision 1.111  2000/06/25 11:58:03  prez
** Fixed problem where messages from nickserv about killing user would not
** be sent out (people would not know a nick was forbidden).
**
** Revision 1.110  2000/06/25 10:32:41  prez
** Fixed channel forbid.
**
** Revision 1.109  2000/06/18 13:31:48  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.108  2000/06/18 12:49:27  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.107  2000/06/15 13:41:11  prez
** Added my tasks to develop *grin*
** Also did all the chanserv live locking (stored to be done).
** Also made magick check if its running, and kill on startup if so.
**
** Revision 1.106  2000/06/13 14:11:54  prez
** Locking system has been re-written, it doent core anymore.
** So I have set 'MAGICK_LOCKS_WORK' define active :)
**
** Revision 1.105  2000/06/12 06:07:50  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.104  2000/06/11 09:30:21  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.103  2000/06/11 08:20:12  prez
** More minor bug fixes, godda love testers.
**
** Revision 1.102  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.101  2000/06/08 13:07:34  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.100  2000/06/06 08:57:57  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.99  2000/05/28 02:37:16  prez
** Minor bug fixes (help system and changing nicks)
**
** Revision 1.98  2000/05/27 07:06:02  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.97  2000/05/25 08:16:39  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.96  2000/05/22 13:00:09  prez
** Updated version.h and some other stuff
**
** Revision 1.95  2000/05/21 04:49:40  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.94  2000/05/19 10:48:14  prez
** Finalized the DCC Sending (now uses the Action map properly)
**
** Revision 1.93  2000/05/18 11:41:46  prez
** Fixed minor front-end issues with the filesystem...
**
** Revision 1.92  2000/05/17 14:08:12  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.91  2000/05/17 12:39:55  prez
** Fixed DCC Sending and file lookups (bypassed the DccMap for now).
** Still to fix DCC Receiving.  Looks like a wxFile::Length() issue.
**
** Revision 1.90  2000/05/17 07:47:59  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.89  2000/05/14 06:30:14  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.88  2000/05/14 04:02:54  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.87  2000/05/13 08:26:44  ungod
** no message
**
** Revision 1.86  2000/05/13 07:05:46  prez
** Added displaying of sizes to all file fields..
**
** Revision 1.85  2000/05/10 11:46:59  prez
** added back memo timers
**
** Revision 1.84  2000/05/08 14:42:02  prez
** More on xmlisation of nickserv and chanserv
**
** Revision 1.83  2000/05/03 14:12:23  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.82  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.81  2000/04/18 14:34:23  prez
** Fixed the HELP system, it now loads into memory, and can be unloaded
** with the OS unload command.  The stats however are inaccurate.
**
** Revision 1.80  2000/04/16 14:29:44  prez
** Added stats for usage, and standardized grabbing paths, etc.
**
** Revision 1.79  2000/04/16 06:12:13  prez
** Started adding body to the documentation...
**
** Revision 1.78  2000/04/04 03:21:35  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.77  2000/04/04 03:13:51  prez
** Added support for masking hostnames.
**
** Revision 1.76  2000/04/03 09:45:24  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.75  2000/04/02 13:06:04  prez
** Fixed the channel TOPIC and MODE LOCK stuff ...
**
** Also fixed the setting of both on join...
**
** Revision 1.74  2000/04/02 07:25:05  prez
** Fixed low watermarks with threads, it all works now!
**
** Revision 1.73  2000/03/29 09:41:19  prez
** Attempting to fix thread problem with mBase, and added notification
** of new memos on join of channel or signon to network.
**
** Revision 1.72  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.71  2000/03/27 21:26:12  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.70  2000/03/26 14:59:37  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.69  2000/03/24 15:35:18  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.68  2000/03/24 12:53:05  prez
** FileSystem Logging
**
** Revision 1.67  2000/03/23 10:22:25  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.66  2000/03/19 08:50:55  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.65  2000/03/15 14:42:59  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.64  2000/03/14 10:05:17  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.63  2000/03/08 23:38:37  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.62  2000/03/02 07:25:11  prez
** Added stuff to do the chanserv greet timings (ie. only greet if a user has
** been OUT of channel over 'x' seconds).  New stored chanserv cfg item.
**
** Revision 1.61  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.60  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.59  2000/02/21 03:27:39  prez
** Updated language files ...
**
** Revision 1.58  2000/02/17 12:55:06  ungod
** still working on borlandization
**
** Revision 1.57  2000/02/16 12:59:40  ungod
** fixing for borland compilability
**
** Revision 1.56  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.55  2000/02/15 10:37:50  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"
#include "dccengine.h"

#ifndef MAGICK_HAS_EXCEPTIONS
static Nick_Stored_t GLOB_Nick_Stored_t;
static Nick_Live_t GLOB_Nick_Live_t;
static mDateTime GLOB_mDateTime;
#endif

void Nick_Live_t::InFlight_t::ChgNick(const mstring& newnick)
{
    FT("Nick_Live_t::InFlight_t::ChgNick", (newnick));
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    WLOCK2(("NickServ", "live", newnick.LowerCase(), "InFlight"));
    MCB(nick);
    nick = newnick;
    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    if (timer)
    {
	CB(1, timer);
	mstring *arg = NULL;
	if (ACE_Reactor::instance()->cancel_timer(timer,
		reinterpret_cast<const void **>(arg)) &&
	    arg != NULL)
	{
	    delete arg;
	}
	timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(nick.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
	CE(1, timer);
    }
    MCE(nick);
}

void Nick_Live_t::InFlight_t::operator=(const InFlight_t &in)
{
    NFT("Nick_Live_t::InFlight_t::operator=");
    nick        = in.nick;
    type	= in.type;
    fileattach	= in.fileattach;
    fileinprog	= in.fileinprog;
    service	= in.service;
    sender	= in.sender;
    recipiant	= in.recipiant;
    text	= in.text;
}

    
// NICK has been destructed, or is going to be.
// Send any pending memos with End(0), but because
// it may have an 'in progress' memo at the time of
// destruction, we make sure the timer is cancelled.
Nick_Live_t::InFlight_t::~InFlight_t()
{
    NFT("Nick_Live_t::InFlight_t::~InFlight_t");
    if (Exists())
	End(0u);
    mstring *arg = NULL;
    if (timer)
    {
	if (ACE_Reactor::instance()->cancel_timer(timer,
		reinterpret_cast<const void **>(arg)) &&
	    arg != NULL)
	{
	    delete arg;
	}
	timer = 0;
    }
}


// Initialise all veriables (done in Cancel() or End())
void Nick_Live_t::InFlight_t::init()
{
    NFT("Nick_Live_t::InFlight_t::init");
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    type = FileMap::MemoAttach;
    timer = 0u;
    fileattach = false;
    fileinprog = false;
    service.erase();
    sender.erase();
    recipiant.erase();
    text.erase();
}


// We have completed a file transfer, or errored out.
// 0 if we errored, else its a file number.
void Nick_Live_t::InFlight_t::File(const unsigned long filenum)
{
    FT("Nick_Live_t::InFlight_t::File", (filenum));
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileinprog"));
    MCB(fileinprog);
    fileinprog = false;
    MCE(fileinprog);
    if (filenum)
	End(filenum);
    else
	Cancel();
}


// Ok, we've started a file transfer, cancel timer,
// now we wait for the File() command.
void Nick_Live_t::InFlight_t::SetInProg()
{
    NFT("Nick_Live_t::InFlight_t::SetInProg");
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileinprog"));
    MCB(fileinprog);
    fileinprog = true;
    mstring *arg = NULL;
    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    if (timer)
    {
	CB(1, timer);
	if (ACE_Reactor::instance()->cancel_timer(timer,
		reinterpret_cast<const void **>(arg)) &&
	    arg != NULL)
	{
	    delete arg;
	}
	timer = 0;
	CE(1, timer);
    }
    MCE(fileinprog);
}


// New memo, send an old one if it isnt in-progress, and
// cancel it if it was never started.
void Nick_Live_t::InFlight_t::Memo (const bool file, const mstring& mynick,
	const mstring& who, const mstring& message, const bool silent)
{
    FT("Nick_Live_t::InFlight_t::Memo", (file, mynick, who, message, silent));
    if (!Parent->nickserv.IsStored(nick))
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/ISNOTSTORED");
	return;
    }

    if (IsChan(who))
    {
	if (!Parent->chanserv.IsStored(who))
	{
	    SEND(mynick, nick, "CS_STATUS/ISNOTSTORED", (
		    who));
	    return;
	}

	if (file)
	{
	    NSEND(mynick, nick, "ERR_SYNTAX/CHANFILEATTACH");
	    return;
	}
    }
    else
    {
	if (!Parent->nickserv.IsStored(who))
	{
	    SEND(mynick, nick, "NS_OTH_STATUS/ISNOTSTORED", (
			who));
	    return;
	}
	if (Parent->nickserv.GetStored(who).Forbidden())
	{
	    SEND(mynick, nick, "NS_OTH_STATUS/ISFORBIDDEN", (
			Parent->getSname(who)));
	    return;
	}

	if (file && !Parent->memoserv.Files())
	{
	    NSEND(mynick, nick, "MS_SYNTAX/FILEDISABLED");
	    return;
	}
    }

    if (file && !(Parent->files.TempDirSize() == 0 ||
	mFile::DirUsage(Parent->files.TempDir()) <=
		Parent->files.TempDirSize()))
    {
	NSEND(mynick, nick, "DCC/NOSPACE2");
	return;
    }

    if (File())
    {
	if (InProg())
	{
	    NSEND(service, nick, "ERR_SITUATION/FILEINPROG");
	    return;
	}
	else
	{
	    Cancel();
	}
    }
    else if (Exists())
    {
	End(0u);
    }

    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    DumpB();
    type = FileMap::MemoAttach;
    fileattach = file;
    service = mynick;
    sender = nick;
    recipiant = who;
    text = message;

    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
    DumpE();

    if (!silent)
    {
	if (fileattach)
	    SEND(service, nick, "MS_COMMAND/PENDING_FILE", (
		ToHumanTime(Parent->memoserv.InFlight(), nick)));
	else
	    SEND(service, nick, "MS_COMMAND/PENDING", (
		ToHumanTime(Parent->memoserv.InFlight(), nick)));
    }
}


// Add text to a memo, and re-start the timer.
void Nick_Live_t::InFlight_t::Continue(const mstring& message)
{
    FT("Nick_Live_t::InFlight_t::Continue", (message));
    if (!Memo())
    {
	NSEND(service, nick, "MS_STATUS/NOPENDING");
	return;
    }
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "text"));
    MCB(text);
    text += message;
    mstring *arg = NULL;
    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    if (timer)
    {
	CB(1, timer);
	if (ACE_Reactor::instance()->cancel_timer(timer,
		reinterpret_cast<const void **>(arg)) &&
	    arg != NULL)
	{
	    delete arg;
	}
	timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(nick.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
	CE(1, timer);
    }
    MCE(text);
    SEND(service, nick, "MS_COMMAND/CONTINUE", (
	    ToHumanTime(Parent->memoserv.InFlight(), nick)));
}


// Cancel a memo or picture send.
void Nick_Live_t::InFlight_t::Cancel()
{
    NFT("Nick_Live_t::InFlight_t::Cancel");

    mstring *arg = NULL;
    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    if (timer)
    {
	MCB(timer);
	if (ACE_Reactor::instance()->cancel_timer(timer,
		reinterpret_cast<const void **>(arg)) &&
	    arg != NULL)
	{
	    delete arg;
	}
	timer = 0;
	MCE(timer);
    }
    if (Memo() && !File())
    {
	NSEND(service, nick, "MS_COMMAND/CANCEL");
    }
    init();
}


// This is the final destination of all memos.
// It will do nothing if a file is in-progress, and
// will call Cancel() if no file was started, but requested.
// It accepts an argument of 'file number'.  Ignored if
// no file attachment was requested, but set if 
void Nick_Live_t::InFlight_t::End(const unsigned long filenum)
{
    NFT("Nick_Live_t::InFlight_t::End");
    if (File() && InProg())
    {
	// We do nothing ...
    }
    else if (File() && filenum == 0u)
    {
	Cancel();
    }
    else
    {
	mstring *arg = NULL;
	MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
	if (timer)
	{
	    MCB(timer);
	    if (ACE_Reactor::instance()->cancel_timer(timer,
		reinterpret_cast<const void **>(arg)) &&
		arg != NULL)
	    {
		delete arg;
	    }
	    timer = 0;
	    MCE(timer);
	}
	RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
	if (Parent->nickserv.IsStored(sender))
	{
	    if (!Parent->nickserv.GetStored(sender).Host().empty())
	    {
		sender = Parent->nickserv.GetStored(sender).Host();
	    }
	    if (Memo())
	    {
		if (IsChan(recipiant))
		{
		    if (Parent->chanserv.IsStored(recipiant))
		    {
			News_t tmp(recipiant, sender, text);
			Parent->memoserv.AddChannelNews(&tmp);

			RLOCK2(("ChanServ", "stored", recipiant.LowerCase()));
			Chan_Stored_t &cstored = Parent->chanserv.GetStored(recipiant);
			SEND(service, nick, "MS_COMMAND/SENT", (
			    recipiant, cstored.Founder()));
			LOG(LM_DEBUG, "MEMOSERV/SEND", (
				Parent->nickserv.GetLive(sender).Mask(Nick_Live_t::N_U_P_H),
				recipiant));

			RLOCK3(("MemoServ", "channel", recipiant.LowerCase()));
			MemoServ::channel_news_t &newslist = Parent->memoserv.GetChannel(recipiant);
			if (Parent->chanserv.IsLive(recipiant))
			{
			    RLOCK(("ChanServ", "live", recipiant.LowerCase()));
			    Chan_Live_t &clive = Parent->chanserv.GetLive(recipiant);
			    unsigned int i;
			    for(i=0; i<clive.Users(); i++)
			    {
				if (Parent->nickserv.IsStored(clive.User(i)) &&
				    cstored.GetAccess(clive.User(i), "READMEMO"))
				{
				    SEND(service, clive.User(i), "MS_COMMAND/CS_NEW", (
					    newslist.size(), recipiant, nick,
					    service, recipiant, newslist.size()));
				}
			    }
			}
		    }
		}
		else
		{
		    if (Parent->nickserv.IsStored(recipiant))
		    {
			mstring realrecipiant = Parent->nickserv.GetStored(recipiant).Host();
			if (realrecipiant.empty())
			    realrecipiant = recipiant;

			if (!(!filenum || Parent->memoserv.FileSize() == 0 ||
			    Parent->filesys.GetSize(FileMap::MemoAttach, filenum) <=
			    Parent->memoserv.FileSize()))
			{
			    Parent->filesys.EraseFile(FileMap::MemoAttach, filenum);
			    NSEND(service, nick, "MS_COMMAND/TOOBIG");
			}
			else if (!(!filenum || Parent->files.MemoAttachSize() == 0 ||
			    Parent->filesys.FileSysSize(FileMap::MemoAttach) <=
			    Parent->files.MemoAttachSize()))
			{
			    Parent->filesys.EraseFile(FileMap::MemoAttach, filenum);
			    NSEND(service, nick, "MS_COMMAND/NOSPACE");
			}
			else if (recipiant.IsSameAs(realrecipiant, true) ||
			    Parent->nickserv.IsStored(realrecipiant))
			{
			    Memo_t tmp(realrecipiant, sender, text, filenum);
			    Parent->memoserv.AddNickMemo(&tmp);

			    if (filenum)
			    {
				LOG(LM_DEBUG, "MEMOSERV/FILE", (
					Parent->nickserv.GetLive(sender).Mask(Nick_Live_t::N_U_P_H),
					Parent->filesys.GetName(FileMap::MemoAttach, filenum),
					fmstring("%08x", filenum),
					ToHumanSpace(Parent->filesys.GetSize(FileMap::MemoAttach, filenum)),
					realrecipiant));
			    }
			    SEND(service, nick, "MS_COMMAND/SENT", (
				recipiant, realrecipiant));

			    RLOCK2(("MemoServ", "nick", realrecipiant.LowerCase()));
			    MemoServ::nick_memo_t &memolist = Parent->memoserv.GetNick(realrecipiant);
			    RLOCK3(("NickServ", "stored", realrecipiant.LowerCase()));
			    Nick_Stored_t &nstored = Parent->nickserv.GetStored(realrecipiant);
			    if (nstored.IsOnline())
				SEND(service, realrecipiant, "MS_COMMAND/NS_NEW", (
				    memolist.size(), service, memolist.size()));
			    unsigned int i;
			    for (i=0; i < nstored.Siblings(); i++)
			    {
				if (Parent->nickserv.GetStored(nstored.Sibling(i)).IsOnline())
				{
				    SEND(service, nstored.Sibling(i), "MS_COMMAND/NS_NEW", (
					memolist.size(), service, memolist.size()));
				}
			    }
			}
			else if (File())
			{
			    Parent->filesys.EraseFile(FileMap::MemoAttach, filenum);
			    NSEND(service, nick, "MS_COMMAND/CANCEL");
			}
		    }
		    else if (File())
		    {
			Parent->filesys.EraseFile(FileMap::MemoAttach, filenum);
			NSEND(service, nick, "MS_COMMAND/CANCEL");
		    }
		}
	    }
	    else if (Picture())
	    {
		if (filenum && !(Parent->memoserv.FileSize() == 0 ||
		    Parent->filesys.GetSize(FileMap::Picture, filenum) <=
		    Parent->nickserv.PicSize()))
		{
		    Parent->filesys.EraseFile(FileMap::Picture, filenum);
		    NSEND(service, nick, "NS_YOU_COMMAND/TOOBIG");
		}
		else if (filenum && !(Parent->files.PictureSize() == 0 ||
		    Parent->filesys.FileSysSize(FileMap::Picture) <=
		    Parent->files.PictureSize()))
		{
		    Parent->filesys.EraseFile(FileMap::Picture, filenum);
		    NSEND(service, nick, "NS_YOU_COMMAND/NOSPACE");
		}
		else if (filenum)
		{
		    Parent->nickserv.GetStored(sender).GotPic(filenum);
		    NSEND(service, nick, "NS_YOU_COMMAND/SAVED");
		    LOG(LM_DEBUG, "NICKSERV/PICTURE_ADD", (
			Parent->nickserv.GetLive(sender).Mask(Nick_Live_t::N_U_P_H),
			sender, fmstring("%08x", filenum),
			ToHumanSpace(Parent->filesys.GetSize(FileMap::Picture, filenum))));
		}
		else
		{
		    SEND(service, nick, "DCC/FAILED", ( "GET"));
		}
	    }
	    else if (Public())
	    {
		if (filenum && !(Parent->files.PublicSize() == 0 ||
		    Parent->filesys.FileSysSize(FileMap::Public) <=
		    Parent->files.PublicSize()))
		{
		    Parent->filesys.EraseFile(FileMap::Public, filenum);
		    NSEND(service, nick, "DCC/NOSPACE");
		}
		else if (filenum)
		{
		    SEND(service, nick, "LIST/ADD", (
    			Parent->filesys.GetName(FileMap::Public, filenum),
    			Parent->getMessage(nick,"LIST/FILES")));
		    Parent->filesys.SetPriv(FileMap::Public, filenum, text);
		    LOG(LM_DEBUG, "SERVMSG/FILE_ADD", (
			Parent->nickserv.GetLive(sender).Mask(Nick_Live_t::N_U_P_H),
			Parent->filesys.GetName(FileMap::Public, filenum),
			fmstring("%08x", filenum),
			ToHumanSpace(Parent->filesys.GetSize(FileMap::Public, filenum)),
			((Parent->filesys.GetPriv(FileMap::Public, filenum).empty()) ?
				"ALL" : Parent->filesys.GetPriv(FileMap::Public, filenum).c_str())));
		}
		else
		{
		    SEND(service, nick, "DCC/FAILED", ( "GET"));
		}
	    }
	}
	init();
    }
}


void Nick_Live_t::InFlight_t::Picture(const mstring& mynick)
{
    FT("Nick_Live_t::InFlight_t::Picture", (mynick));
    if (!Parent->nickserv.IsStored(nick))
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/ISNOTSTORED");
	return;
    }
    else if (Parent->nickserv.PicExt().empty())
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/PICDISABLED");
	return;
    }

    if (!(Parent->files.TempDirSize() == 0 ||
	mFile::DirUsage(Parent->files.TempDir()) <=
		Parent->files.TempDirSize()))
    {
	NSEND(mynick, nick, "DCC/NOSPACE2");
	return;
    }

    if (File())
    {
	if (InProg())
	{
	    NSEND(mynick, nick, "ERR_SITUATION/FILEINPROG");
	    return;
	}
	else
	{
	    Cancel();
	}
    }
    else if (Exists())
    {
	End(0u);
    }

    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    DumpB();
    type = FileMap::Picture;
    fileattach = true;
    sender = nick;
    service = mynick;

    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
    DumpE();

    NSEND(service, nick, "NS_YOU_COMMAND/PENDING");
}


void Nick_Live_t::InFlight_t::Public(const mstring& mynick, const mstring& committees)
{
    FT("Nick_Live_t::InFlight_t::Public", (mynick, committees));
    if (!Parent->nickserv.IsStored(nick))
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/ISNOTSTORED");
	return;
    }

    if (!(Parent->files.TempDirSize() == 0 ||
	mFile::DirUsage(Parent->files.TempDir()) <=
		Parent->files.TempDirSize()))
    {
	NSEND(mynick, nick, "DCC/NOSPACE2");
	return;
    }

    if (File())
    {
	if (InProg())
	{
	    NSEND(mynick, nick, "ERR_SITUATION/FILEINPROG");
	    return;
	}
	else
	{
	    Cancel();
	}
    }
    else if (Exists())
    {
	End(0u);
    }

    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    DumpB();
    type = FileMap::Public;
    fileattach = true;
    sender = nick;
    service = mynick;
    text = committees;

    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
    DumpE();

    NSEND(service, nick, "NS_YOU_COMMAND/PUB_PENDING");
}

mstring Nick_Live_t::InFlight_t::Text()
{
    NFT("Nick_Live_t::InFlight_t::Memo");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "text"));
    RET(text);
}

mstring Nick_Live_t::InFlight_t::Recipiant()
{
    NFT("Nick_Live_t::InFlight_t::Memo");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "recipiant"));
    RET(recipiant);
}

bool Nick_Live_t::InFlight_t::Memo() const
{
    NFT("Nick_Live_t::InFlight_t::Memo");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "recipiant"));
    RET(!recipiant.empty());
}

bool Nick_Live_t::InFlight_t::Picture() const
{
    NFT("Nick_Live_t::InFlight_t::Picture");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "type"));
    RET(type == FileMap::Picture);
}

bool Nick_Live_t::InFlight_t::Public() const
{
    NFT("Nick_Live_t::InFlight_t::Public");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "type"));
    RET(type == FileMap::Public);
}

bool Nick_Live_t::InFlight_t::Exists() const
{
    NFT("Nick_Live_t::InFlight_t::Exists");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "recipiant"));
    RLOCK2(("NickServ", "live", nick.LowerCase(), "InFlight", "type"));
    RET(!recipiant.empty() || type != FileMap::MemoAttach);
}

bool Nick_Live_t::InFlight_t::File() const
{
    NFT("Nick_Live_t::InFlight_t::File");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileattach"));
    RET(fileattach);
}

bool Nick_Live_t::InFlight_t::InProg() const
{
    NFT("Nick_Live_t::InFlight_t::InProg");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileinprog"));
    RET(fileinprog);
}

size_t Nick_Live_t::InFlight_t::Usage() const
{
    size_t retval = 0;

    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    retval += nick.capacity();
    retval += sizeof(type);
    retval += sizeof(timer);
    retval += sizeof(fileattach);
    retval += sizeof(fileinprog);
    retval += sender.capacity();
    retval += recipiant.capacity();
    retval += text.capacity();

    return retval;
}

void Nick_Live_t::InFlight_t::DumpB() const
{
    // 8 Elements
    MB(0, (nick, type, timer, fileattach, fileinprog, sender, recipiant, text));
}

void Nick_Live_t::InFlight_t::DumpE() const
{
    // 8 Elements
    ME(0, (nick, type, timer, fileattach, fileinprog, sender, recipiant, text));
}


Nick_Live_t::Nick_Live_t()
	: last_msg_entries(0), flood_triggered_times(0), failed_passwds(0),
	  identified(false), services(true)
{
    NFT("Nick_Live_t::Nick_Live_t");
    // Dont call anything that locks, no names!
}


Nick_Live_t::Nick_Live_t(const mstring& name, const mDateTime& signon,
	const mstring& server, const mstring& username,
	const mstring& hostname, const mstring& realname)
	: i_Name(name), i_Signon_Time(signon),
	  i_My_Signon_Time(mDateTime::CurrentDateTime()),
	  i_Last_Action(mDateTime::CurrentDateTime()),
	  i_realname(realname), i_user(username), i_host(hostname),
	  i_alt_host(hostname), i_server(server.LowerCase()), last_msg_entries(0),
	  flood_triggered_times(0), failed_passwds(0), identified(false),
	  services(false), InFlight(name)
{
    FT("Nick_Live_t::Nick_Live_t",(name, signon, server, username, hostname, realname));

    InFlight.init();

    // User is on AKILL, add the mask, and No server will kill
    { MLOCK(("OperServ", "Akill"));
    if (Parent->operserv.Akill_find(i_user + "@" + i_host))
    {
	mstring reason(Parent->operserv.Akill->Value().second);
	// Do this cos it will be removed when we KILL,
	// and we dont wanna get out of touch.
	Parent->operserv.AddHost(i_host);
	LOG(LM_INFO, "OTHER/KILL_AKILL", (
		Mask(N_U_P_H), Parent->operserv.Akill->Entry(),
		reason));
	Parent->server.AKILL(Parent->operserv.Akill->Entry(), reason,
			Parent->operserv.Akill->Value().first -
				Parent->operserv.Akill->Last_Modify_Time().SecondsSince(),
			Parent->operserv.Akill->Last_Modifier());
	i_server.erase();
	i_realname = reason;
	return;
    }}

    // User triggered CLONE protection, No server will kill
    if (Parent->operserv.AddHost(i_host))
    {
	LOG(LM_INFO, "OTHER/KILL_CLONE", (
		Mask(N_U_P_H)));
	i_server.erase();
	i_realname = Parent->operserv.Def_Clone();
	return;
    }

    if (Parent->nickserv.IsStored(i_Name))
    {
	if (IsRecognized() && !Parent->nickserv.GetStored(i_Name).Secure())
	    Parent->nickserv.GetStored(i_Name).Signon(i_realname, Mask(U_P_H).After("!"));
    }
    DumpE();
}


Nick_Live_t::Nick_Live_t(const mstring& name, const mstring& username,
	const mstring& hostname, const mstring& realname)
	: i_Name(name), i_Signon_Time(mDateTime::CurrentDateTime()),
	  i_My_Signon_Time(mDateTime::CurrentDateTime()),
	  i_Last_Action(mDateTime::CurrentDateTime()),
	  i_realname(realname), i_user(username), i_host(hostname),
	  i_alt_host(hostname), last_msg_entries(0), flood_triggered_times(0),
	  failed_passwds(0), identified(true), services(true), InFlight(name)
{
    FT("Nick_Live_t::Nick_Live_t",(name, username, hostname, realname));
    InFlight.init();
    DumpE();
}


void Nick_Live_t::operator=(const Nick_Live_t &in)
{
    NFT("Nick_Live_t::operator=");
    i_Name=in.i_Name;
    i_Signon_Time=in.i_Signon_Time;
    i_My_Signon_Time=in.i_My_Signon_Time;
    i_Last_Action=in.i_Last_Action;
    i_realname=in.i_realname;
    i_user=in.i_user;
    i_host=in.i_host;
    i_alt_host=in.i_alt_host;
    i_server=in.i_server;
    modes=in.modes;
    set<mstring>::const_iterator i;
    joined_channels.clear();
    for(i=in.joined_channels.begin();i!=in.joined_channels.end();i++)
	joined_channels.insert(*i);
    last_msg_times.empty();
    last_msg_times.reserve(in.last_msg_times.size());
    unsigned int k;
    for(k=0;k<in.last_msg_times.size();k++)
	last_msg_times.push_back(in.last_msg_times[k]);
    last_msg_entries=in.last_msg_entries;
    flood_triggered_times=in.flood_triggered_times;
    failed_passwds=in.failed_passwds;
    chans_founder_identd.clear();
    for(i=in.chans_founder_identd.begin();i!=in.chans_founder_identd.end();i++)
	chans_founder_identd.insert(*i);
    identified=in.identified;
    services=in.services;
    last_nick_reg=in.last_nick_reg;
    last_chan_reg=in.last_chan_reg;
    last_memo=in.last_memo;
    map<mstring, mstring>::const_iterator j;
    i_UserDef.clear();
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
    InFlight=in.InFlight;
    if (!InFlight.nick.IsSameAs(i_Name))
	InFlight.nick = i_Name;
}

void Nick_Live_t::Join(const mstring& chan)
{
    FT("Nick_Live_t::Join", (chan));
    bool joined = true;
    if (Parent->chanserv.IsLive(chan))
    {
	joined = Parent->chanserv.GetLive(chan).Join(i_Name);
    }
    else
    {
	Chan_Live_t tmp(chan, i_Name);
	Parent->chanserv.AddLive(&tmp);
    }
    // We do this seperately coz we require initialisation of
    // the channel to be completed.
    if (joined)
    {
	if (Parent->chanserv.IsStored(chan))
	    Parent->chanserv.GetStored(chan).Join(i_Name);
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
	MCB(joined_channels.size());
	joined_channels.insert(chan.LowerCase());
	MCE(joined_channels.size());
    }
}


void Nick_Live_t::Part(const mstring& chan)
{
    FT("Nick_Live_t::Part", (chan));
    if (Parent->chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
	if (Parent->chanserv.GetLive(chan).Part(i_Name) == 0)
	{
	    Parent->chanserv.RemLive(chan);
	}
    }
    else
    {
	LOG(LM_TRACE, "ERROR/REC_FORNONCHAN", (
		"PART", i_Name, chan));
    }

    WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    MCB(joined_channels.size());
    joined_channels.erase(chan.LowerCase());
    MCE(joined_channels.size());
    // Just incase we were cycling and need to JOIN
    //Parent->server.FlushUser(i_Name, chan);
}

void Nick_Live_t::Kick(const mstring& kicker, const mstring& chan)
{
    FT("Nick_Live_t::Kick", (kicker, chan));
    if (Parent->chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
	if (Parent->chanserv.GetLive(chan).Kick(i_Name, kicker) == 0)
	{
	    Parent->chanserv.RemLive(chan);
	}
    }
    else
    {
	LOG(LM_ERROR, "ERROR/REC_FORNONCHAN", (
		"KICK", kicker, chan));
    }

    WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    MCB(joined_channels.size());
    joined_channels.erase(chan.LowerCase());
    MCE(joined_channels.size());
}


void Nick_Live_t::Quit(const mstring& reason)
{
    FT("Nick_Live_t::Quit", (reason));

    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    if (!(IsServices() || HasMode("o")))
	Parent->operserv.RemHost(i_host);
    }

    // Check if we're currently being TEMP ignored ...
    { MLOCK(("OperServ","Ignore"));
    if (Parent->operserv.Ignore_find(Mask(N_U_P_H)))
    {
	if (Parent->operserv.Ignore->Value() != true)
	{
	    Parent->operserv.Ignore_erase();
	}
    }}

    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    while (joined_channels.size())
	Part(*joined_channels.begin());
    }

    unsigned long i;
    { RLOCK(("DCC"));
    if (Parent->dcc != NULL)
    {
	vector<unsigned long> dccs = Parent->dcc->GetList(i_Name);
        for (i=0; i<dccs.size(); i++)
	    Parent->dcc->Cancel(dccs[i], true);
    }}
    if (InFlight.Exists())
	InFlight.End(0u);

    // We successfully ident to all channels we tried to
    // ident for before, so that they 0 our count -- we dont
    // want it carrying over to next time we log on.
    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "try_chan_ident"));
    for (i=0; i<try_chan_ident.size(); i++)
	if (Parent->chanserv.IsStored(try_chan_ident[i]))
	    Parent->chanserv.GetStored(try_chan_ident[i]).CheckPass(i_Name,
		Parent->chanserv.GetStored(try_chan_ident[i]).Password());
    }

    if (Parent->nickserv.IsStored(i_Name) &&
	Parent->nickserv.GetStored(i_Name).IsOnline())
	Parent->nickserv.GetStored(i_Name).Quit(reason);
}


bool Nick_Live_t::IsInChan(const mstring& chan)
{
    FT("Nick_Live_t::IsInChan", (chan));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    bool retval = (joined_channels.find(chan.LowerCase()) != joined_channels.end());
    RET(retval);
}

set<mstring> Nick_Live_t::Channels() const
{
    NFT("Nick_Live_t::Channels");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    NRET(set<mstring>, joined_channels);
}

bool Nick_Live_t::FloodTrigger()
{
    NFT("Nick_Live_t::FloodTrigger");
    bool retval = false;

    // We DONT ignore OPER's
    if (HasMode("o") || IsServices())
    {
	RET(false);
    }

    // Check if we're currently being ignored ...
    { MLOCK(("OperServ", "Ignore"));
    if (Parent->operserv.Ignore_size())
    {
	if (Parent->operserv.Ignore_find(Mask(N_U_P_H)))
	{
	    // IF we havnt ignored for long enough yet, or its perminant ...
	    if (Parent->operserv.Ignore->Last_Modify_Time().SecondsSince() <= Parent->operserv.Ignore_Time()
		|| Parent->operserv.Ignore->Value() == true)
	    {
		RET(true);
	    }
	    else
	    {
		Parent->operserv.Ignore_erase();
	    }
	}
    }}

    // Clean up previous entries and push current entry
    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "last_msg_times"));
    MCB(last_msg_times.size());
    while (last_msg_times.size() && last_msg_times[0u].SecondsSince() > Parent->operserv.Flood_Time())
	last_msg_times.erase(last_msg_times.begin());
    last_msg_times.push_back(mDateTime::CurrentDateTime());

    // Check if we just triggered ignore.
    if (last_msg_times.size() > Parent->operserv.Flood_Msgs())
    {
	WLOCK2(("NickServ", "live", i_Name.LowerCase(), "flood_triggered_times"));
	CB(1, flood_triggered_times);
	flood_triggered_times++;
	// Add To ignore, they're naughty.
	if (flood_triggered_times >= Parent->operserv.Ignore_Limit())
	{
	    Parent->operserv.Ignore_insert(Mask(Parent->operserv.Ignore_Method()), true, i_Name);
	    SEND(Parent->servmsg.FirstName(), i_Name, "ERR_SITUATION/IGNORE_TRIGGER", (
			Parent->operserv.Flood_Msgs(), ToHumanTime(Parent->operserv.Flood_Time(), i_Name)));
	    SEND(Parent->servmsg.FirstName(), i_Name, "ERR_SITUATION/PERM_IGNORE", (
			Parent->operserv.Ignore_Limit()));
	    LOG(LM_NOTICE, "OTHER/PERM_IGNORE", (
			Mask(N_U_P_H)));
	    ANNOUNCE(Parent->servmsg.FirstName(), "MISC/FLOOD_PERM", (
			i_Name));
	}
	else
	{
	    Parent->operserv.Ignore_insert(Mask(Parent->operserv.Ignore_Method()), false, i_Name);
	    SEND(Parent->servmsg.FirstName(), i_Name, "ERR_SITUATION/IGNORE_TRIGGER", (
			Parent->operserv.Flood_Msgs(), ToHumanTime(Parent->operserv.Flood_Time(), i_Name)));
	    SEND(Parent->servmsg.FirstName(), i_Name, "ERR_SITUATION/TEMP_IGNORE", (
			ToHumanNumber(flood_triggered_times), Parent->operserv.Ignore_Limit(),
			ToHumanTime(Parent->operserv.Ignore_Time(), i_Name)));
	    LOG(LM_NOTICE, "OTHER/TEMP_IGNORE", (
			Mask(N_U_P_H)));
	    ANNOUNCE(Parent->servmsg.FirstName(), "MISC/FLOOD_TEMP", (
			i_Name));
	}
 
 	CE(1, flood_triggered_times);
	retval = true;
    }

    MCE(last_msg_times.size()); }
    RET(retval);
}


void Nick_Live_t::Name(const mstring& in)
{
    FT("Nick_Live_t::Name", (in));

    InFlight.ChgNick(in);

    WLOCK(("NickServ", "live", i_Name.LowerCase()));
    WLOCK2(("NickServ", "live", in.LowerCase()));
    if (i_Name.IsSameAs(in, true))
    {
	i_Name = in;
	return;
    }

    set<mstring>::iterator iter;
    vector<mstring> chunked;
    unsigned long i;

    // Store what committee's we WERE on ...
    // This is needed to send logon notices ONLY for committees
    // we have joined by a nick change.
    set<mstring> wason;
    CommServ::list_t::iterator iter2;
    for (iter2 = Parent->commserv.ListBegin(); iter2 != Parent->commserv.ListEnd();
								iter2++)
    {
	if (iter2->second.IsOn(i_Name))
	    wason.insert(iter2->first);
    }

    { RLOCK(("DCC"));
    if (Parent->dcc != NULL)
    {
	vector<unsigned long> dccs = Parent->dcc->GetList(i_Name);
	for (i=0; i<dccs.size(); i++)
	    Parent->dcc->GetXfers(dccs[i]).ChgNick(in);
    }}

    // Carry over failed attempts (so /nick doesnt cure all!)
    // We dont care if it doesnt exist, they can drop channels *shrug*
    for (i=0; i<try_chan_ident.size(); i++)
    {
	if (Parent->chanserv.IsStored(try_chan_ident[i]))
	    Parent->chanserv.GetStored(try_chan_ident[i]).ChgAttempt(i_Name, in);
    }

    MCB(i_Name);
    CB(1, i_My_Signon_Time);
    if (Parent->nickserv.IsStored(i_Name))
    {
	// We are not related (by brotherhood, or parentage)
	if (!(Parent->nickserv.GetStored(i_Name).IsSibling(in) ||
	    Parent->nickserv.GetStored(i_Name).Host().LowerCase() == i_Name.LowerCase()))
	{
	    CB(2, identified);
	    CB(3, chans_founder_identd.size());
	    CB(4, failed_passwds);
	    identified = false;
	    chans_founder_identd.clear();
	    failed_passwds = 0;
	    CE(2, identified);
	    CE(3, chans_founder_identd.size());
	    CE(4, failed_passwds);
	}
	// Last Seen and Last Quit
	Parent->nickserv.GetStored(i_Name).ChgNick(in);
    }

    // WooHoo, we have a new nick!
    mstring oldnick(i_Name);
    i_Name = in;
    i_My_Signon_Time = mDateTime::CurrentDateTime();

    // Rename ourselves in all channels ...
    for (iter=joined_channels.begin(); iter!=joined_channels.end(); iter++)
    {
	if (Parent->chanserv.IsLive(*iter))
	{
	    Parent->chanserv.GetLive(*iter).ChgNick(oldnick, i_Name);
	}
	else
	{
	    chunked.push_back(*iter);
	    LOG(LM_ERROR, "ERROR/REC_FORNOTINCHAN", (
		"NICK", oldnick, *iter));
	}
    }

    // Clean up non-existant channels ...
    CB(5, joined_channels.size());
    for (i=0; i<chunked.size(); i++)
	joined_channels.erase(chunked[i]);
    CE(5, joined_channels.size());

    CE(1, i_My_Signon_Time);
    MCE(i_Name);
    if (Parent->nickserv.IsStored(i_Name))
    {
	if (Parent->nickserv.GetStored(i_Name).Forbidden())
	{
	    Parent->nickserv.send(Parent->nickserv.FirstName(),
		i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/FORBIDDEN"),
		ToHumanTime(Parent->nickserv.Ident(), i_Name).c_str());
	    return;
	}
	else if (Parent->nickserv.GetStored(i_Name).IsOnline())
	    Parent->nickserv.GetStored(i_Name).Signon(i_realname, Mask(U_P_H).After("!"));
	else if (Parent->nickserv.GetStored(i_Name).Protect())
	    Parent->nickserv.send(Parent->nickserv.FirstName(),
		i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/PROTECTED"),
		ToHumanTime(Parent->nickserv.Ident(), i_Name).c_str());
    }

    // Send notices for committees we were NOT on
    mstring setmode;
    for (iter2 = Parent->commserv.ListBegin(); iter2 != Parent->commserv.ListEnd();
								iter2++)
    {
	if (iter2->second.IsOn(i_Name) && wason.find(iter2->first) == wason.end())
	{
	    if (iter2->first == Parent->commserv.ALL_Name())
		setmode += Parent->commserv.ALL_SetMode();
	    else if (iter2->first == Parent->commserv.REGD_Name())
		setmode += Parent->commserv.REGD_SetMode();
	    else if (iter2->first == Parent->commserv.OPER_Name())
		setmode += Parent->commserv.OPER_SetMode();
	    else if (iter2->first == Parent->commserv.ADMIN_Name())
		setmode += Parent->commserv.ADMIN_SetMode();
	    else if (iter2->first == Parent->commserv.SOP_Name())
		setmode += Parent->commserv.SOP_SetMode();
	    else if (iter2->first == Parent->commserv.SADMIN_Name())
		setmode += Parent->commserv.SADMIN_SetMode();

	    for (iter2->second.message = iter2->second.MSG_begin();
		iter2->second.message != iter2->second.MSG_end(); iter2->second.message++)
	    {
		Parent->servmsg.send(i_Name, "[" + IRC_Bold + iter2->first + IRC_Off +
					    "] " + iter2->second.message->Entry());
	    }
	}
    }
    if (!setmode.empty())
    {
	mstring setmode2;
	for (i=0; i<setmode.size(); i++)
	{
	    if (setmode[i] != '+' && setmode[i] != '-' &&
		setmode[i] != ' ' && !HasMode(setmode[i]))
	        setmode2 += setmode[i];
	}
	Parent->server.SVSMODE(Parent->nickserv.FirstName(), i_Name, "+" + setmode2);
    }
}


void Nick_Live_t::SendMode(const mstring& in)
{
    FT("Nick_Live_t::SendMode", (in));

    if (IsServices())
    {
	Parent->server.MODE(i_Name, in);
    }
    else if (!Parent->server.proto.SVSMODE().empty())
    {
	Parent->server.SVSMODE(Parent->nickserv.FirstName(), i_Name, in);
    }
}


void Nick_Live_t::Mode(const mstring& in)
{
    FT("Nick_Live_t::Mode", (in));

    bool add = true;
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "modes"));
    MCB(modes);
    for (unsigned int i=0; i<in.size(); i++)
    {
	COM(("Processing mode %c%c", add ? '+' : '-', in[i]));
	switch(in[i])
	{
	case ':':
	    break;

	case '+':
	    add = true;
	    break;

	case '-':
	    add = false;
	    break;

	case 'o':
	    // We check the existing modes incase we get
	    // duplicate +o/-o (dont want to overhost it!)
	    if (!IsServices())
	    {
		if (add && !modes.Contains(in[i]))
		{
		    // Store what committee's we WERE on ...
		    // This is needed to send logon notices ONLY for committees
		    // we have joined by a nick change.
		    set<mstring> wason;
		    CommServ::list_t::iterator iter2;
		    for (iter2 = Parent->commserv.ListBegin(); iter2 != Parent->commserv.ListEnd();
								iter2++)
		    {
			if (iter2->second.IsOn(i_Name))
			    wason.insert(iter2->first);
		    }

		    modes += in[i];
		    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
		    Parent->operserv.RemHost(i_host);
		    }
		    MLOCK(("OperServ", "OperDeny"));
		    // IF we are SecureOper and NOT (on oper list && recoznized)
		    // OR user is on OperDeny and NOT (on sadmin list && recognized)
		    // Yeah, one UUUUUUGLY if.
		    if ((Parent->operserv.SecureOper() &&
			!(Parent->nickserv.IsStored(i_Name) &&
			Parent->nickserv.GetStored(i_Name).IsOnline() &&
			Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
			Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsIn(i_Name))) ||
			(Parent->operserv.OperDeny_find(Mask(N_U_P_H)) &&
			!(Parent->nickserv.IsStored(i_Name) &&
			Parent->nickserv.GetStored(i_Name).IsOnline() &&
			Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
			Parent->commserv.GetList(Parent->commserv.SADMIN_Name()).IsIn(i_Name))))
		    {
			if (!Parent->server.proto.SVSMODE().empty())
			{
			    SendMode("-o");
			    NSEND(Parent->operserv.FirstName(), i_Name, "OS_STATUS/ISOPERDENY");
			}
			else
			{
			    Parent->server.KILL(Parent->operserv.FirstName(),
		    	    	i_Name, Parent->getMessage(i_Name, "MISC/KILL_OPERDENY"));
			    return;
			}
		    }
		    else
		    {
			mstring setmode;
			for (iter2 = Parent->commserv.ListBegin(); iter2 != Parent->commserv.ListEnd();
								iter2++)
			{
			    if (iter2->second.IsOn(i_Name) && wason.find(iter2->first) == wason.end())
			    {
				if (!Parent->server.proto.SVSMODE().empty())
				{
				    if (iter2->first == Parent->commserv.ALL_Name())
					setmode += Parent->commserv.ALL_SetMode();
				    else if (iter2->first == Parent->commserv.REGD_Name())
					setmode += Parent->commserv.REGD_SetMode();
				    else if (iter2->first == Parent->commserv.OPER_Name())
					setmode += Parent->commserv.OPER_SetMode();
				    else if (iter2->first == Parent->commserv.ADMIN_Name())
					setmode += Parent->commserv.ADMIN_SetMode();
				    else if (iter2->first == Parent->commserv.SOP_Name())
					setmode += Parent->commserv.SOP_SetMode();
				    else if (iter2->first == Parent->commserv.SADMIN_Name())
					setmode += Parent->commserv.SADMIN_SetMode();
				}

				for (iter2->second.message = iter2->second.MSG_begin();
					iter2->second.message != iter2->second.MSG_end(); iter2->second.message++)
				{
				    Parent->servmsg.send(i_Name, "[" + IRC_Bold + iter2->first + IRC_Off +
					    "] " + iter2->second.message->Entry());
				}
			    }
			}
			if (!setmode.empty())
			{
			    mstring setmode2;
			    for (unsigned int j=0; j<setmode.size(); j++)
			    {
				if (setmode[j] != '+' && setmode[j] != '-' &&
					setmode[j] != ' ' && !HasMode(setmode[j]))
				    setmode2 += setmode[j];
			    }
			    SendMode("+" + setmode2);
			}
		    }
		}
		else if (!add && modes.Contains(in[i]))
		{
		    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
		    Parent->operserv.AddHost(i_host);
		    modes.Remove(in[i]);
		}
		else
		{
		    LOG(LM_TRACE, "ERROR/MODE_INEFFECT", (
			add ? '+' : '-', in[i], i_Name, i_Name));
		}
	        break;
	    }
	    // WE dont break here coz services will fall through.

	default:
	    if (add && !modes.Contains(in[i]))
	    {
		modes += in[i];
	    }
	    else if (!add && modes.Contains(in[i]))
	    {
		modes.Remove(in[i]);
	    }
	    else
	    {
		LOG(LM_TRACE, "ERROR/MODE_INEFFECT", (
			add ? '+' : '-', in[i], i_Name, i_Name));
	    }
	    break;
	}
    }
    MCE(modes);
}


mstring Nick_Live_t::Mode() const
{
    NFT("Nick_Live_t::Mode");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "modes"));
    RET(modes);
}

bool Nick_Live_t::HasMode(const mstring& in) const
{
    FT("Nick_Live_t::HasMode", (in));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "modes"));
    RET(modes.Contains(in));
}

void Nick_Live_t::Away(const mstring& in)
{
    FT("Nick_Live_t::Away", (in));
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_away"));
    MCB(i_away);
    i_away = in;
    MCE(i_away);
}

mstring Nick_Live_t::Away() const
{
    NFT("Nick_Live_t::Away");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_away"));
    RET(i_away);
}

mDateTime Nick_Live_t::LastAction() const
{
    NFT("Nick_Live_t::LastAction");
    if (IsServices())
    {
	RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_Last_Action"));
	RET(i_Last_Action);
    }
    RET(mDateTime::CurrentDateTime());
}


void Nick_Live_t::Action()
{
    NFT("Nick_Live_t::Action");
    if (IsServices())
    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_Last_Action"));
	MCB(i_Last_Action);
	i_Last_Action = mDateTime::CurrentDateTime();
	MCE(i_Last_Action);
    }
}


mDateTime Nick_Live_t::SignonTime() const
{
    NFT("Nick_Live_t::SignonTime");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_Signon_Time"));
    RET(i_Signon_Time);
}

mDateTime Nick_Live_t::MySignonTime() const
{
    NFT("Nick_Live_t::MySignonTime");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_My_Signon_Time"));
    RET(i_My_Signon_Time);
}

mstring Nick_Live_t::RealName() const
{
    NFT("Nick_Live_t::RealName");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_realname"));
    RET(i_realname);
}

mstring Nick_Live_t::User() const
{
    NFT("Nick_Live_t::User");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_user"));
    RET(i_user);
}

mstring Nick_Live_t::Host() const
{
    NFT("Nick_Live_t::Host");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    RET(i_host);
}

mstring Nick_Live_t::AltHost() const
{
    NFT("Nick_Live_t::AltHost");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_alt_host"));
    RET(i_alt_host);
}

void Nick_Live_t::AltHost(const mstring& in)
{
    FT("Nick_Live_t::AltHost", (in));
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_alt_host"));
    MCB(i_alt_host);
    i_alt_host = in;
    MCE(i_alt_host);
}

mstring Nick_Live_t::Server() const
{
    NFT("Nick_Live_t::Server");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_server"));
    RET(i_server);
} 

mstring Nick_Live_t::Squit() const
{
    NFT("Nick_Live_t::Squit");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_squit"));
    RET(i_squit);
}


void Nick_Live_t::SetSquit()
{
    NFT("Nick_Live_t::SetSquit");
    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_squit"));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_server"));
    MCB(i_squit);
    i_squit = i_server;
    MCE(i_squit);
    }

    { RLOCK2(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    if (!IsServices())
	Parent->operserv.RemHost(i_host);
    }

    set<mstring>::iterator i;
    RLOCK3(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    for (i=joined_channels.begin(); i!=joined_channels.end(); i++)
	if (Parent->chanserv.IsLive(*i))
	    Parent->chanserv.GetLive(*i).SquitUser(i_Name);
	else
	{
	    LOG(LM_ERROR, "ERROR/REC_FORNONCHAN", (
		"SQUIT", i_Name, *i));
	}
}


void Nick_Live_t::ClearSquit(const mstring& inmodes)
{
    NFT("Nick_Live_t::ClearSquit");

    // This stops 3 locks being re-set 3 times...
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    MCB(i_squit);

    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_squit"));
    i_squit.erase();
    }

    // These will all be set again
    { WLOCK2(("NickServ", "live", i_Name.LowerCase(), "modes"));
    CB(1, modes);
    modes = inmodes;
    CE(1, modes);
    }

    { WLOCK3(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    CB(2, joined_channels.size());
    set<mstring>::iterator i;
    for (i=joined_channels.begin(); i!=joined_channels.end(); i++)
	if (Parent->chanserv.IsLive(*i))
	    Parent->chanserv.GetLive(*i).UnSquitUser(i_Name);
	else
	{
	    LOG(LM_ERROR, "ERROR/REC_FORNONCHAN", (
		"UNSQUIT", i_Name, *i));
	}

    joined_channels.clear();
    CE(2, joined_channels.size());
    }
    MCE(i_squit);

    if (!IsServices() && Parent->operserv.AddHost(i_host))
    {
	LOG(LM_INFO, "OTHER/KILL_CLONE", (
		Mask(N_U_P_H)));
	Parent->server.KILL(Parent->nickserv.FirstName(), i_Name,
			Parent->operserv.Def_Clone());
	return;
    }
}


mstring Nick_Live_t::Mask(const Nick_Live_t::styles type) const
{
    FT("Nick_Live_t::Mask", (static_cast<int>(type)));

    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_user"));
    RLOCK2(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    mstring retval;
    mstring user = i_user;
    switch (type)
    {
    case N:		// nick!*@*
	retval = i_Name + "!*@*";
	break;

    case N_U_P_H:	// nick!user@port.host
	retval = i_Name + "!" + user + "@" + i_host;
	break;

    case N_U_H:		// nick!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_host.IsNumber())
	    retval = i_Name + "!*" + user + "@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
	    retval = i_Name + "!*" + user + "@*." + i_host.After(".");
	else
	    retval = i_Name + "!*" + user + "@" + i_host;
	break;

    case N_P_H:		// nick!*@port.host
	retval = i_Name + "!*@" + i_host;
	break;

    case N_H:		// nick!*@*.host
	if (i_host.IsNumber())
	    retval = i_Name + "!*@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
	    retval = i_Name + "!*@*." + i_host.After(".");
	else
	    retval = i_Name + "!*@" + i_host;
	break;

    case U_P_H:		// *!user@port.host
	retval = "*!" + user + "@" + i_host;
	break;

    case U_H:		// *!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_host.IsNumber())
	    retval = "*!*" + user + "@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
	    retval = "*!*" + user + "@*." + i_host.After(".");
	else
	    retval = "*!*" + user + "@" + i_host;
	break;

    case P_H:		// *!*@port.host
	retval = "*!*@" + i_host;
	break;

    case H:		// *!*@*.host
	if (i_host.IsNumber())
	    retval = "*!*@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
	    retval = "*!*@*." + i_host.After(".");
	else
	    retval = "*!*@" + i_host;
	break;

    default:
	retval = "*!*@*";
	break;
    }
    
    RET(retval);
}


mstring Nick_Live_t::AltMask(const Nick_Live_t::styles type) const
{
    FT("Nick_Live_t::AltMask", (static_cast<int>(type)));

    RLOCK2(("NickServ", "live", i_Name.LowerCase(), "i_user"));
    RLOCK3(("NickServ", "live", i_Name.LowerCase(), "i_alt_host"));
    mstring retval;
    mstring user = i_user;
    switch (type)
    {
    case N:		// nick!*@*
	retval = i_Name + "!*@*";
	break;

    case N_U_P_H:	// nick!user@port.host
	retval = i_Name + "!" + user + "@" + i_alt_host;
	break;

    case N_U_H:		// nick!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_alt_host.IsNumber())
	    retval = i_Name + "!*" + user + "@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = i_Name + "!*" + user + "@*." + i_alt_host.After(".");
	else
	    retval = i_Name + "!*" + user + "@" + i_alt_host;
	break;

    case N_P_H:		// nick!*@port.host
	retval = i_Name + "!*@" + i_alt_host;
	break;

    case N_H:		// nick!*@*.host
	if (i_alt_host.IsNumber())
	    retval = i_Name + "!*@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = i_Name + "!*@*." + i_alt_host.After(".");
	else
	    retval = i_Name + "!*@" + i_alt_host;
	break;

    case U_P_H:		// *!user@port.host
	retval = "*!" + user + "@" + i_alt_host;
	break;

    case U_H:		// *!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_alt_host.IsNumber())
	    retval = "*!*" + user + "@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = "*!*" + user + "@*." + i_alt_host.After(".");
	else
	    retval = "*!*" + user + "@" + i_alt_host;
	break;

    case P_H:		// *!*@port.host
	retval = "*!*@" + i_alt_host;
	break;

    case H:		// *!*@*.host
	if (i_alt_host.IsNumber())
	    retval = "*!*@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = "*!*@*." + i_alt_host.After(".");
	else
	    retval = "*!*@" + i_alt_host;
	break;

    default:
	retval = "*!*@*";
	break;
    }
    
    RET(retval);
}


mstring Nick_Live_t::ChanIdentify(const mstring& channel, const mstring& password)
{
    FT("Nick_Live_t::ChanIdentify", (channel, password));
    mstring retval;
    if (Parent->chanserv.IsStored(channel))
    {
	unsigned int failtimes = Parent->chanserv.GetStored(channel).CheckPass(i_Name, password);
	if (!failtimes)
	{
	    WLOCK(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
	    MCB(chans_founder_identd.size());
	    chans_founder_identd.insert(channel.LowerCase());
	    MCE(chans_founder_identd.size());
	    retval = parseMessage(Parent->getMessage(i_Name, "CS_COMMAND/IDENTIFIED"),
						mVarArray(channel));
	}
	else
	{
	    // Keep record of all channel attempts, so that when
	    // we change nick or quit, we can let chanserv know.
	    vector<mstring>::iterator iter;
	    WLOCK(("NickServ", "live", i_Name.LowerCase(), "try_chan_ident"));
	    for (iter=try_chan_ident.begin(); iter!=try_chan_ident.end(); iter++) ;
	    if (iter == try_chan_ident.end())
	    {
		MCB(try_chan_ident.size());
		try_chan_ident.push_back(channel.LowerCase());
		MCE(try_chan_ident.size());
	    }

	    if (failtimes >= Parent->chanserv.Passfail())
	    {
		Parent->server.KILL(Parent->nickserv.FirstName(), i_Name,
			Parent->getMessage(i_Name, "MISC/KILL_PASS_FAIL"));
		LOG(LM_NOTICE, "OTHER/KLLL_CHAN_PASS", (
			Mask(N_U_P_H), channel));
		RET("");
	    }
	    else
	    {
		LOG(LM_INFO, "CHANSERV/IDENTIFY_FAILED", (
			Mask(N_U_P_H), channel));
		retval = parseMessage(Parent->getMessage(i_Name, "ERR_SITUATION/CHAN_WRONG_PASS"),
								mVarArray(channel));
	    }
	}
    }
    else
    {
	retval = parseMessage(Parent->getMessage(i_Name, "CS_STATUS/ISNOTSTORED"),
							mVarArray(channel));
    }
    RET(retval);
}


void Nick_Live_t::UnChanIdentify(const mstring& channel)
{
    FT("Nick_Live_t::UnChanIdentify", (channel));

    if (IsChanIdentified(channel))
    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
	MCB(chans_founder_identd.size());
	chans_founder_identd.erase(channel.LowerCase());
	MCE(chans_founder_identd.size());
    }
}

bool Nick_Live_t::IsChanIdentified(const mstring& channel)
{
    FT("Nick_Live_t::IsChanIdentified", (channel));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
    bool retval = (chans_founder_identd.find(channel.LowerCase())!=chans_founder_identd.end());
    RET(retval);
}


mstring Nick_Live_t::Identify(const mstring& password)
{
    FT("Nick_Live_t::Identify", (password));
    mstring retval;
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
    if (identified == true)
    {
	retval = Parent->getMessage(i_Name, "NS_YOU_STATUS/IDENTIFIED");
    }
    else if (Parent->nickserv.IsStored(i_Name))
    {
	WLOCK2(("NickServ", "live", i_Name.LowerCase(), "failed_passwds"));
	if (Parent->nickserv.GetStored(i_Name).Password() == password)
	{
	    set<mstring> wason;
	    CommServ::list_t::iterator iter;
	    for (iter = Parent->commserv.ListBegin(); iter != Parent->commserv.ListEnd();
									    iter++)
		if (iter->second.IsOn(i_Name))
		    wason.insert(iter->first);

	    MCB(identified);
	    CB(1, failed_passwds);
	    identified = true;
	    failed_passwds = 0;
	    CE(1, failed_passwds);
	    MCE(identified);
	    if (Parent->nickserv.GetStored(i_Name).Secure())
		Parent->nickserv.GetStored(i_Name).Signon(i_realname, Mask(U_P_H).After("!"));

	    // Send notices for committees we were NOT on
	    for (iter = Parent->commserv.ListBegin(); iter != Parent->commserv.ListEnd();
									    iter++)
	    {
		if (iter->second.IsOn(i_Name) && wason.find(iter->first) == wason.end())
		{
		    for (iter->second.message = iter->second.MSG_begin();
			iter->second.message != iter->second.MSG_end(); iter->second.message++)
		    {
			Parent->servmsg.send(i_Name, "[" + IRC_Bold + iter->first +
					IRC_Off + "] " + iter->second.message->Entry());
		    }
		}
	    }
	    retval = Parent->getMessage(i_Name, "NS_YOU_COMMAND/IDENTIFIED");
	}
	else
	{
	    MCB(failed_passwds);
	    failed_passwds++;
	    MCE(failed_passwds);
	    if (failed_passwds >= Parent->nickserv.Passfail())
	    {
		Parent->server.KILL(Parent->nickserv.FirstName(), i_Name,
			Parent->getMessage(i_Name, "MISC/KILL_PASS_FAIL"));
		LOG(LM_NOTICE, "OTHER/KLLL_NICK_PASS", (
			Mask(N_U_P_H), i_Name));
		RET("");
	    }
	    else
	    {
		LOG(LM_INFO, "NICKSERV/IDENTIFY_FAILED", (
			Mask(N_U_P_H), i_Name));
		retval = Parent->getMessage(i_Name, "ERR_SITUATION/NICK_WRONG_PASS");
	    }
	}
    }
    else
    {
	retval = Parent->getMessage(i_Name, "NS_YOU_STATUS/ISNOTSTORED");
    }
    RET(retval);
}

void Nick_Live_t::UnIdentify()
{
    NFT("Nick_Live_t::UnIdentify");
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
    MCB(identified);
    identified = false;
    MCE(identified);
}

bool Nick_Live_t::IsIdentified() const
{
    NFT("Nick_Live_t::IsIdentified");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
    RET(identified);
}

bool Nick_Live_t::IsRecognized() const
{
    NFT("Nick_Live_t::IsRecognised");
    bool retval = false;
    if (Parent->nickserv.IsStored(i_Name))
    {
	retval = Parent->nickserv.GetStored(i_Name).IsAccess(Mask(U_P_H).After("!"));
    }
    RET(retval);
}

bool Nick_Live_t::IsServices() const
{
    NFT("Nick_Live_t::IsServices");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "services"));
    RET(services);
}

void Nick_Live_t::SetLastNickReg()
{
    NFT("Nick_Live_t::SetLastNickReg");
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "last_nick_reg"));
    MCB(last_nick_reg);
    last_nick_reg = mDateTime::CurrentDateTime();
    MCE(last_nick_reg);
}

mDateTime Nick_Live_t::LastNickReg() const
{
    NFT("Nick_Live_t::LastNickReg");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "last_nick_reg"));
    RET(last_nick_reg);
}

void Nick_Live_t::SetLastChanReg()
{
    NFT("Nick_Live_t::SetLastChanReg");
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "last_chan_reg"));
    MCB(last_chan_reg);
    last_chan_reg = mDateTime::CurrentDateTime();
    MCE(last_chan_reg);
}

mDateTime Nick_Live_t::LastChanReg() const
{
    NFT("Nick_Live_t::LastChanReg");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "last_chan_reg"));
    RET(last_chan_reg);
}

void Nick_Live_t::SetLastMemo()
{
    NFT("Nick_Live_t::SetLastMemo");
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "last_memo"));
    MCB(last_memo);
    last_memo = mDateTime::CurrentDateTime();
    MCE(last_memo);
}

mDateTime Nick_Live_t::LastMemo() const
{
    NFT("Nick_Live_t::LastMemo");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "last_memo"));
    RET(last_memo);
}

size_t Nick_Live_t::Usage() const
{
    size_t retval = 0;

    // We write lock here coz its the only way to
    // ensure the values are NOT going to change.
    WLOCK(("NickServ", "live", i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += InFlight.Usage();
    retval += sizeof(i_Signon_Time.Internal());
    retval += sizeof(i_My_Signon_Time.Internal());
    retval += sizeof(i_Last_Action.Internal());
    retval += i_realname.capacity();
    retval += i_user.capacity();
    retval += i_host.capacity();
    retval += i_alt_host.capacity();
    retval += i_server.capacity();
    retval += i_squit.capacity();
    retval += i_away.capacity();
    retval += modes.capacity();
    set<mstring>::const_iterator i;
    for (i=joined_channels.begin(); i!=joined_channels.end(); i++)
    {
	retval += i->capacity();
    }
    vector<mDateTime>::const_iterator j;
    for (j=last_msg_times.begin(); j!=last_msg_times.end(); j++)
    {
	retval += sizeof(j->Internal());
    }
    retval += sizeof(last_msg_entries);
    retval += sizeof(flood_triggered_times);
    retval += sizeof(failed_passwds);
    for (i=chans_founder_identd.begin(); i!=chans_founder_identd.end(); i++)
    {
	retval += i->capacity();
    }
    vector<mstring>::const_iterator k;
    for (k=try_chan_ident.begin(); k!=try_chan_ident.end(); k++)
    {
	retval += k->capacity();
    }
    retval += sizeof(identified);
    retval += sizeof(services);
    retval += sizeof(last_nick_reg.Internal());
    retval += sizeof(last_chan_reg.Internal());
    retval += sizeof(last_memo.Internal());

    return retval;
}

void Nick_Live_t::DumpB() const
{
    // 16 Elements
    MB(0, (i_Name, i_Signon_Time, i_My_Signon_Time, i_Last_Action, i_realname,
	i_user, i_host, i_alt_host, i_server, i_squit, i_away, modes,
	joined_channels.size(), last_msg_times.size(), last_msg_entries,
	flood_triggered_times));
    // 8 Elements
    MB(16, (failed_passwds, chans_founder_identd.size(), try_chan_ident.size(),
	identified, services, last_nick_reg, last_chan_reg, last_memo));
}

void Nick_Live_t::DumpE() const
{
    // 16 Elements
    ME(0, (i_Name, i_Signon_Time, i_My_Signon_Time, i_Last_Action, i_realname,
	i_user, i_host, i_alt_host, i_server, i_squit, i_away, modes,
	joined_channels.size(), last_msg_times.size(), last_msg_entries,
	flood_triggered_times));
    // 8 Elements
    ME(16, (failed_passwds, chans_founder_identd.size(), try_chan_ident.size(),
	identified, services, last_nick_reg, last_chan_reg, last_memo));
}

// =======================================================================


void Nick_Stored_t::Signon(const mstring& realname, const mstring& mask)
{
    FT("Nick_Stored_t::Signon", (realname, mask));
    { WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastRealName"));
    WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_LastMask"));
    MCB(i_LastRealName);
    CB(1, i_LastMask);
    i_LastRealName = realname;
    i_LastMask = mask;
    CE(1, i_LastMask);
    MCE(i_LastRealName);
    }

    mstring who = Host();
    if (who.empty())
	who = i_Name;
    if (Parent->memoserv.IsNick(who))
    {
	unsigned int count = Parent->memoserv.NickMemoCount(who);
	if (count)
	    SEND(Parent->memoserv.FirstName(), i_Name, "MS_STATUS/NS_UNREAD", (
		count, Parent->memoserv.FirstName()));
    }
}


void Nick_Stored_t::ChgNick(const mstring& nick)
{
    FT("Nick_Stored_t::ChgNick", (nick));
    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastQuit"));
    WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
    MCB(i_LastQuit);
    CB(1, i_LastSeenTime);
    i_LastQuit = "NICK CHANGE -> " + nick;
    i_LastSeenTime = mDateTime::CurrentDateTime();
    CE(1, i_LastSeenTime);
    MCE(i_LastQuit);
}


Nick_Stored_t::Nick_Stored_t()
{
    NFT("Nick_Stored_t::Nick_Stored_t");
}


Nick_Stored_t::Nick_Stored_t(const mstring& nick, const mstring& password)
	: i_Name(nick), i_RegTime(mDateTime::CurrentDateTime()), i_Password(password),
	  i_Protect(Parent->nickserv.DEF_Protect()), l_Protect(false),
	  i_Secure(Parent->nickserv.DEF_Secure()), l_Secure(false),
	  i_NoExpire(Parent->nickserv.DEF_NoExpire()), l_NoExpire(false),
	  i_NoMemo(Parent->nickserv.DEF_NoMemo()), l_NoMemo(false),
	  i_Private(Parent->nickserv.DEF_Private()), l_Private(false),
	  i_PRIVMSG(Parent->nickserv.DEF_PRIVMSG()), l_PRIVMSG(false),
	  i_Language(Parent->nickserv.DEF_Language().UpperCase()), l_Language(false),
	  i_Forbidden(false), i_Picture(0)
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick, password));

    if (Parent->nickserv.IsLive(i_Name))
    {
	i_LastRealName = Parent->nickserv.GetLive(i_Name).RealName();
	i_LastMask = Parent->nickserv.GetLive(i_Name).Mask(Nick_Live_t::U_P_H).After("!");
        Parent->nickserv.GetLive(i_Name).Identify(Password());
    }
    DumpE();
}


Nick_Stored_t::Nick_Stored_t(const mstring& nick)
	: i_Name(nick), i_RegTime(mDateTime::CurrentDateTime()),
	  i_Forbidden(true), i_Picture(0)
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick.LowerCase()));
    DumpE();
} 


Nick_Stored_t::Nick_Stored_t(const mstring& nick, const mDateTime& regtime,
	const Nick_Stored_t &in)
	: i_Name(nick), i_RegTime(regtime), i_Host(in.i_Name.LowerCase()),
	  i_Forbidden(false), i_Picture(false)
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick, "(const Nick_Stored_t &) in"));

    if (Parent->nickserv.IsLive(i_Name))
    {
	i_LastRealName = Parent->nickserv.GetLive(i_Name).RealName();
	i_LastMask = Parent->nickserv.GetLive(i_Name).Mask(Nick_Live_t::U_P_H).After("!");
        Parent->nickserv.GetLive(i_Name).Identify(Password());
    }
    DumpE();
}


mDateTime Nick_Stored_t::RegTime() const
{
    NFT("Nick_Stored_t::RegTime");
    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_RegTime"));
    RET(i_RegTime);
}

unsigned long Nick_Stored_t::Drop()
{
    NFT("Nick_Stored_t::Drop");

    // When we go, we take all our slaves with us!
    unsigned long i, dropped = 1;
    if (Host().empty())
    {
	while(Siblings())
	{
	    mstring nick = Sibling(0);
	    if (!nick.empty())
	    {
		
		dropped += Parent->nickserv.GetStored(nick).Drop();
		Parent->nickserv.RemStored(nick);
	    }
	}
    }
    else
    {
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	Parent->nickserv.GetStored(i_Host).i_slaves.erase(i_Name.LowerCase());
    }

    // Now we go for our channels ...
    vector<mstring> killchans;
    ChanServ::stored_t::iterator iter;
    for (iter = Parent->chanserv.StoredBegin();
	    iter != Parent->chanserv.StoredEnd(); iter++)
    {
	if (iter->second.Founder().IsSameAs(i_Name, true))
	{
	    if (!iter->second.CoFounder().empty() &&
		Parent->nickserv.IsStored(iter->second.CoFounder()))
	    {
		iter->second.Founder(iter->second.CoFounder());
	    }
	    else
		killchans.push_back(iter->first);
	}
    }

    for (i=0; i<killchans.size(); i++)
    {
	Parent->chanserv.RemStored(killchans[i]);
    }

    RET(dropped);
}


void Nick_Stored_t::operator=(const Nick_Stored_t &in)
{
    NFT("Nick_Stored_t::operator=");
    i_Name=in.i_Name;
    i_RegTime=in.i_RegTime;
    i_Password=in.i_Password;
    i_Email=in.i_Email;
    i_URL=in.i_URL;
    i_ICQ=in.i_ICQ;
    i_Description=in.i_Description;
    i_Comment=in.i_Comment;
    i_Host=in.i_Host;
    set<mstring>::const_iterator i;
    i_slaves.clear();
    for(i=in.i_slaves.begin();i!=in.i_slaves.end();i++)
	i_slaves.insert(*i);
    i_access.clear();
    for(i=in.i_access.begin();i!=in.i_access.end();i++)
	i_access.insert(*i);
    i_ignore.clear();
    for(i=in.i_ignore.begin();i!=in.i_ignore.end();i++)
	i_ignore.insert(*i);
    i_Protect=in.i_Protect;
    l_Protect=in.l_Protect;
    i_Secure=in.i_Secure;
    l_Secure=in.l_Secure;
    i_NoExpire=in.i_NoExpire;
    l_NoExpire=in.l_NoExpire;
    i_NoMemo=in.i_NoMemo;
    l_NoMemo=in.l_NoMemo;
    i_Private=in.i_Private;
    l_Private=in.l_Private;
    i_PRIVMSG=in.i_PRIVMSG;
    l_PRIVMSG=in.l_PRIVMSG;
    i_Language=in.i_Language;
    l_Language=in.l_Language;
    i_Forbidden=in.i_Forbidden;
    i_Picture=in.i_Picture;
    i_Suspend_By=in.i_Suspend_By;
    i_Suspend_Time=in.i_Suspend_Time;

    // non-slave & offline
    i_LastSeenTime=in.i_LastSeenTime;
    i_LastRealName=in.i_LastRealName;
    i_LastMask=in.i_LastMask;
    i_LastQuit=in.i_LastQuit;
    map<mstring, mstring>::const_iterator j;
    i_UserDef.clear();
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
}


mstring Nick_Stored_t::Email()
{
    NFT("Nick_Stored_t::Email");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Email"));
	RET(i_Email);
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).Email();
	RET(retval);
    }
}


void Nick_Stored_t::Email(const mstring& in)
{
    FT("Nick_Stored_t::Email", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Email"));
	MCB(i_Email);
	i_Email = in;
	MCE(i_Email);
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).Email(in);
    }
}


mstring Nick_Stored_t::URL()
{
    NFT("Nick_Stored_t::URL");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_URL"));
	RET(i_URL);
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).URL();
	RET(retval);
    }
}


void Nick_Stored_t::URL(const mstring& in)
{
    FT("Nick_Stored_t::URL", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_URL"));
	MCB(i_URL);
	i_URL = in;
	MCE(i_URL);
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).URL(in);
    }
}


mstring Nick_Stored_t::ICQ()
{
    NFT("Nick_Stored_t::ICQ");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ICQ"));
	RET(i_ICQ);
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).ICQ();
	RET(retval);
    }
}


void Nick_Stored_t::ICQ(const mstring& in)
{
    FT("Nick_Stored_t::ICQ", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ICQ"));
	MCB(i_ICQ);
	i_ICQ = in;
	MCE(i_ICQ);
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).ICQ(in);
    }
}


mstring Nick_Stored_t::Description()
{
    NFT("Nick_Stored_t::Description");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Description"));
	RET(i_Description);
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).Description();
	RET(retval);
    }
}


void Nick_Stored_t::Description(const mstring& in)
{
    FT("Nick_Stored_t::Description", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Description"));
	MCB(i_Description);
	i_Description = in;
	MCE(i_Description);
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).Description();
    }
}


mstring Nick_Stored_t::Comment()
{
    NFT("Nick_Stored_t::Comment");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Comment"));
	RET(i_Comment);
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).Comment();
	RET(retval);
    }
}


void Nick_Stored_t::Comment(const mstring& in)
{
    FT("Nick_Stored_t::Comment", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Comment"));
	MCB(i_Comment);
	i_Comment = in;
	MCE(i_Comment);
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).Comment(in);
    }
}


void Nick_Stored_t::Suspend(const mstring& name)
{
    FT("Nick_Stored_t::Suspend", (name));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_Time"));
	MCB(i_Suspend_By);
	CB(1, i_Suspend_Time);
	i_Suspend_By = name;
	i_Suspend_Time = mDateTime::CurrentDateTime();
	CE(1, i_Suspend_Time);
	MCE(i_Suspend_By);
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).Suspend(name);
    }
}


void Nick_Stored_t::UnSuspend()
{
    NFT("Nick_Stored_t::UnSuspend");
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
	MCB(i_Suspend_By);
	i_Suspend_By.erase();
	MCE(i_Suspend_By);
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).UnSuspend();
    }
}

mstring Nick_Stored_t::Host()
{
    NFT("Nick_Stored_t::Host");
    mstring retval;
    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Host"));
    if (!i_Host.empty())
	if (!Parent->nickserv.IsStored(i_Host))
	{
	    LOG(LM_ERROR, "ERROR/HOST_NOTREGD", (
		i_Host, i_Name));
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Host"));
	    MCB(i_Host);
	    i_Host.erase();
	    MCE(i_Host);
	}
	else
	    retval = Parent->nickserv.GetStored(i_Host).Name();
    RET(retval);
}

mstring Nick_Stored_t::Password()
{
    NFT("Nick_Stored_t::Password");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Password"));
	RET(i_Password);
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).Password();
	RET(retval);
    }
}


void Nick_Stored_t::Password(const mstring& in)
{
    FT("Nick_Stored_t::Password", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Password"));
	MCB(i_Password);
	i_Password = in;
	MCE(i_Password);
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).Password(in);
    }
}


bool Nick_Stored_t::Slave(const mstring& nick, const mstring& password,
	const mDateTime& regtime)
{
    FT("Nick_Stored_t::Slave", (nick, password, regtime));

    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Forbidden"));
	RLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_Password"));
	CP(("Checking \"%s\" == \"%s\" ...", password.c_str(), i_Password.c_str()));
	if (i_Forbidden  || password != i_Password)
	{
	    RET(false);
	}

	if (Parent->nickserv.IsStored(nick))
	{
	    ChangeOver(nick);
	    Parent->nickserv.RemStored(nick);
	}

	Nick_Stored_t tmp(nick, regtime, *this);
	Parent->nickserv.AddStored(&tmp);
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	MCB(i_slaves.size());
	i_slaves.insert(nick.LowerCase());
	MCE(i_slaves.size());
	RET(true);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).Slave(nick, password, regtime);
	RET(retval);
    }
}


unsigned int Nick_Stored_t::Siblings()
{
    NFT("Nick_Stored_t::Siblings");
    unsigned int retval = 0;
    if (Host().empty())
    {
	retval = i_slaves.size();
    }
    else
    {
	retval = Parent->nickserv.GetStored(i_Host).Siblings();
    }
    RET(retval);
}


mstring Nick_Stored_t::Sibling(const unsigned int count)
{
    FT("Nick_Stored_t::Siblings", (count));
    mstring retval;
    if (Host().empty())
    {
	vector<mstring> chunked;
	set<mstring>::iterator iter;
	unsigned int i;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	for (i=0, iter=i_slaves.begin(); iter!=i_slaves.end(); iter++, i++)
	{
	    if (Parent->nickserv.IsStored(*iter))
	    {
		if (i==count)
		{
		    retval = iter->LowerCase();
		    break;
		}
	    }
	    else
	    {
		LOG(LM_ERROR, "ERROR/SLAVE_NOTREGD", (
			*iter, i_Name));
		chunked.push_back(iter->c_str());
		i--;
	    }
	}
	if (chunked.size())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	    MCB(i_slaves.size());
	    for (i=0; i<chunked.size(); i++)
		i_slaves.erase(chunked[i]);
	    MCE(i_slaves.size());
	}
    }
    else
    {
	retval = Parent->nickserv.GetStored(i_Host).Sibling(count);
    }
    RET(retval);
}


bool Nick_Stored_t::IsSibling(const mstring& nick)
{
    FT("Nick_Stored_t::IsSibling", (nick));
    if (i_Name.LowerCase() == nick.LowerCase())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	bool retval = (i_slaves.find(nick.LowerCase()) != i_slaves.end());
	if (retval && !Parent->nickserv.IsStored(nick))
	{
	    LOG(LM_ERROR, "ERROR/SLAVE_NOTREGD", (
			nick, i_Name));
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	    MCB(i_slaves.size());
	    i_slaves.erase(nick);
	    MCE(i_slaves.size());
	    retval = false;
	}
	RET(retval);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).IsSibling(nick);
	RET(retval);
    }
}

void Nick_Stored_t::ChangeOver(const mstring& oldnick)
{
    FT("Nick_Stored_t::ChangeOver", (oldnick)); 

    bool found;
    mstring modifier, valueS;
    long valueL;
    mDateTime modtime;

    CommServ::list_t::iterator citer;
    for (citer = Parent->commserv.ListBegin();
			    citer != Parent->commserv.ListEnd(); citer++)
    {
	MLOCK(("CommServ", "list", citer->first, "member"));
	found = false;
	if (citer->second.Name() != Parent->commserv.ALL_Name() &&
	    citer->second.Name() != Parent->commserv.REGD_Name() &&
	    citer->second.Name() != Parent->commserv.SADMIN_Name())
	{
	    if (citer->second.find(i_Name))
	    {
		modifier = citer->second.member->Last_Modifier();
		modtime = citer->second.member->Last_Modify_Time();
		citer->second.erase();
		found = true;
	    }
	    if (citer->second.find(oldnick))
	    {
		modifier = citer->second.member->Last_Modifier();
		modtime = citer->second.member->Last_Modify_Time();
		citer->second.erase();
		found = true;
	    }
	    if ((citer->second.HeadCom().empty()) &&
		(citer->second.IsHead(i_Name) || citer->second.IsHead(oldnick)))
	    {
		citer->second.Head(i_Name);
		found = false;
	    }
	    if (found)
	    {
		citer->second.insert(i_Name, modifier, modtime);
		citer->second.member = citer->second.end();
	    }
	}
    }

    ChanServ::stored_t::iterator csiter;
    for (csiter = Parent->chanserv.StoredBegin();
			csiter != Parent->chanserv.StoredEnd(); csiter++)
    {
	if (csiter->second.CoFounder().LowerCase() == oldnick.LowerCase())
	{
	    csiter->second.CoFounder(i_Name);
	}
	if (csiter->second.Founder().LowerCase() == oldnick.LowerCase())
	{
	    csiter->second.Founder(i_Name);
	}
	{ MLOCK(("ChanServ", "stored", csiter->first, "Access"));
	found = false;
	valueL = 0;
	if (csiter->second.Access_find(i_Name))
	{
	    valueL = csiter->second.Access->Value();
	    modifier = csiter->second.Access->Last_Modifier();
	    modtime = csiter->second.Access->Last_Modify_Time();
	    csiter->second.Access_erase();
	    found = true;
	}
	if (csiter->second.Access_find(oldnick))
	{
	    if (csiter->second.Access->Value() > valueL)
		valueL = csiter->second.Access->Value();
	    modifier = csiter->second.Access->Last_Modifier();
	    modtime = csiter->second.Access->Last_Modify_Time();
	    csiter->second.Access_erase();
	    found = true;
	}
	if (found)
	{
	    csiter->second.Access_insert(i_Name, valueL, modifier, modtime);
	    csiter->second.Access = csiter->second.Access_end();
	} }
	{ MLOCK(("ChanServ", "stored", csiter->first, "Akick"));
	found = false;
	if (csiter->second.Akick_find(i_Name))
	{
	    valueS = csiter->second.Akick->Value();
	    modifier = csiter->second.Akick->Last_Modifier();
	    modtime = csiter->second.Akick->Last_Modify_Time();
	    csiter->second.Akick_erase();
	    found = true;
	}
	if (csiter->second.Akick_find(oldnick))
	{
	    valueS = csiter->second.Akick->Value();
	    modifier = csiter->second.Akick->Last_Modifier();
	    modtime = csiter->second.Akick->Last_Modify_Time();
	    csiter->second.Akick_erase();
	    found = true;
	}
	if (found)
	{
	    csiter->second.Akick_insert(i_Name, valueS, modifier, modtime);
	    csiter->second.Akick = csiter->second.Akick_end();
	} }
	{ MLOCK(("ChanServ", "stored", csiter->first, "Greet"));
	found = false;
	if (csiter->second.Greet_find(i_Name))
	{
	    valueS = csiter->second.Greet->Entry();
	    modtime = csiter->second.Greet->Last_Modify_Time();
	    csiter->second.Greet_erase();
	    found = true;
	}
	if (csiter->second.Greet_find(oldnick))
	{
	    valueS = csiter->second.Greet->Entry();
	    modtime = csiter->second.Greet->Last_Modify_Time();
	    csiter->second.Greet_erase();
	    found = true;
	}
	if (found)
	{
	    csiter->second.Greet_insert(valueS, i_Name, modtime);
	    csiter->second.Greet = csiter->second.Greet_end();
	} }
    }
    
    NickServ::stored_t::iterator niter;
    for (niter = Parent->nickserv.StoredBegin();
			niter != Parent->nickserv.StoredEnd(); niter++)
    {
	if (niter->first != i_Name.LowerCase() &&
	    niter->second.Host().LowerCase() != i_Name.LowerCase() &&
	    !IsSibling(niter->first) &&
	    niter->second.IsIgnore(oldnick))
	{
	    niter->second.IgnoreDel(oldnick);
	    niter->second.IgnoreAdd(i_Name);
	}
    }

    if (Parent->memoserv.IsNick(oldnick))
	Parent->memoserv.NickMemoConvert(oldnick, i_Name);

    MemoServ::channel_t::iterator cniter;
    MemoServ::channel_news_t::iterator cnliter;
    RLOCK(("MemoServ", "channel"));
    for (cniter = Parent->memoserv.ChannelBegin();
			cniter != Parent->memoserv.ChannelEnd(); cniter++)
    {
	RLOCK(("MemoServ", "channel", cniter->first));
	for (cnliter = cniter->second.begin();
			    cnliter != cniter->second.end(); cnliter++)
	{
	    if (cnliter->IsRead(oldnick))
	    {
		cnliter->Unread(oldnick);
		cnliter->Read(i_Name);
	    }
	}
    }
}

bool Nick_Stored_t::MakeHost()
{
    NFT("Nick_Stored_t::MakeHost"); 
    if (Host().empty())
    {
	RET(false);
    }
    else
    {
	// Re-point all slaves to me and copy the slaves list.
	// Then clear the host's slave list, point host to me,
	// and finally set my host pointer to "".
	WLOCK(("NickServ", "stored", i_Name.LowerCase()));
	WLOCK2(("NickServ", "stored", i_Host.LowerCase()));
	DumpB();
	Nick_Stored_t &host = Parent->nickserv.GetStored(i_Host);
	for (unsigned int i=0; i<host.Siblings(); i++)
	{
	    if (host.Sibling(i).LowerCase() != i_Name.LowerCase())
	    {
		i_slaves.insert(host.Sibling(i));
		Parent->nickserv.GetStored(host.Sibling(i)).i_Host = i_Name.LowerCase();
	    }
	}
	i_slaves.insert(i_Host.LowerCase());
	i_Password = host.i_Password;
	i_Email = host.i_Email;
	i_URL = host.i_URL;
	i_ICQ = host.i_ICQ;
	i_Description = host.i_Description;
	i_Comment = host.i_Comment;
	i_access = host.i_access;
	i_ignore = host.i_ignore;
	i_Protect = host.i_Protect;
	l_Protect = host.l_Protect;
	i_Secure = host.i_Secure;
	l_Secure = host.l_Secure;
	i_NoExpire = host.i_NoExpire;
	l_NoExpire = host.l_NoExpire;
	i_NoMemo = host.i_NoMemo;
	l_NoMemo = host.l_NoMemo;
	i_Private = host.i_Private;
	l_Private = host.l_Private;
	i_PRIVMSG = host.i_PRIVMSG;
	l_PRIVMSG = host.l_PRIVMSG;
	i_Language = host.i_Language;
	l_Language = host.l_Language;
	i_Picture = host.i_Picture;
	i_Suspend_By = host.i_Suspend_By;
	i_Suspend_Time = host.i_Suspend_Time;
	host.i_slaves.clear();
	host.i_Host = i_Name.LowerCase();
	mstring tmp = i_Host;
	i_Host.erase();
	DumpE();
	ChangeOver(tmp);
	RET(true);
    }
}


bool Nick_Stored_t::Unlink()
{
    NFT("Nick_Stored_t::Unlink");
    if (Host().empty())
    {
	RET(false);
    }
    else
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase()));
	WLOCK2(("NickServ", "stored", i_Host.LowerCase()));
	DumpB();
	Nick_Stored_t &host = Parent->nickserv.GetStored(i_Host);
	i_slaves.clear();
	i_Password = host.i_Password;
	i_Email = host.i_Email;
	i_URL = host.i_URL;
	i_ICQ = host.i_ICQ;
	i_Description = host.i_Description;
	i_Comment = host.i_Comment;
	i_access = host.i_access;
	i_ignore = host.i_ignore;
	i_Protect = host.i_Protect;
	l_Protect = host.l_Protect;
	i_Secure = host.i_Secure;
	l_Secure = host.l_Secure;
	i_NoExpire = host.i_NoExpire;
	l_NoExpire = host.l_NoExpire;
	i_NoMemo = host.i_NoMemo;
	l_NoMemo = host.l_NoMemo;
	i_Private = host.i_Private;
	l_Private = host.l_Private;
	i_PRIVMSG = host.i_PRIVMSG;
	l_PRIVMSG = host.l_PRIVMSG;
	i_Language = host.i_Language;
	l_Language = host.l_Language;
	i_Picture = 0;
	i_Suspend_By = host.i_Suspend_By;
	i_Suspend_Time = host.i_Suspend_Time;
	host.i_slaves.erase(i_Name.LowerCase());
	i_Host.erase();
	DumpE();
	RET(true);
    }
}


unsigned int Nick_Stored_t::Access()
{
    NFT("Nick_Stored_t::Access");
    unsigned int retval = 0;
    if (Host().empty())
    {
	retval = i_access.size();
    }
    else
    {
	retval = Parent->nickserv.GetStored(i_Host).Access();
    }
    RET(retval);
}


mstring Nick_Stored_t::Access(const unsigned int count)
{
    FT("Nick_Stored_t::Access", (count));
    if (Host().empty())
    {
	set<mstring>::iterator iter;
	unsigned int i;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
	for (i=0, iter=i_access.begin(); iter!=i_access.end(); i++, iter++)
	    if (i==count)
	    {
		RET(*iter);
	    }
	RET("");
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).Access(count);
	RET(retval);
    }
}


bool Nick_Stored_t::AccessAdd(const mstring& in)
{
    FT("Nick_Stored_t::AccessAdd", (in));
    if (Host().empty())
    {
	// Incorrect syntax
	if (!(in.Contains("@") && in.WordCount("@") == 2))
	{
	    RET(false);
	}
	
	// Already exists (or inclusive)
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
	set<mstring>::iterator iter;
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (in.Matches(*iter, true))
	    {
		RET(false);
	    }

	// NEW one includes existing ones ...
	// Remove all obsolite entries.
	vector<mstring> chunked;
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (iter->Matches(in, true))
	    {
		chunked.push_back(*iter);
	    }
	MCB(i_access.size());
	for (unsigned int i=0; i<chunked.size(); i++)
	    i_access.erase(chunked[i]);

	i_access.insert(in.LowerCase());
	MCE(i_access.size());
	RET(true);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).AccessAdd(in);
	RET(retval);
    }
}


unsigned int Nick_Stored_t::AccessDel(const mstring& in)
{
    FT("Nick_Stored_t::AccessDel", (in));
    unsigned int retval = 0;
    if (Host().empty())
    {
	vector<mstring> chunked;
	set<mstring>::iterator iter;
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (in.Matches(*iter, true))
	    {
		chunked.push_back(*iter);
	    }

	MCB(i_access.size());
	for (unsigned int i=0; i<chunked.size(); i++)
	    i_access.erase(chunked[i].LowerCase());
	MCE(i_access.size());
	retval = chunked.size();
    }
    else
    {
	retval = Parent->nickserv.GetStored(i_Host).AccessDel(in);
    }
    RET(retval);
}


bool Nick_Stored_t::IsAccess(const mstring& in)
{
    FT("Nick_Stored_t::IsAccess", (in));
    if (Host().empty())
    {
	set<mstring>::iterator iter;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (in.Matches(*iter, true))
	    {
		RET(true);
	    }
	RET(false);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).IsAccess(in);
	RET(retval);
    }
}


unsigned int Nick_Stored_t::Ignore()
{
    NFT("Nick_Stored_t::Ignore");
    unsigned int retval = 0;
    if (Host().empty())
    {
	retval = i_ignore.size();
    }
    else
    {
	retval = Parent->nickserv.GetStored(i_Host).Ignore();
    }
    RET(retval);
}


mstring Nick_Stored_t::Ignore(const unsigned int count)
{
    FT("Nick_Stored_t::Ignore", (count));
    if (Host().empty())
    {
	set<mstring>::iterator iter;
	unsigned int i;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
	for (i=0, iter=i_ignore.begin(); iter!=i_ignore.end(); i++, iter++)
	    if (i==count)
	    {
		RET(*iter);
	    }
	RET("");
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).Ignore(count);
	RET(retval);
    }
}


bool Nick_Stored_t::IgnoreAdd(const mstring& in)
{
    FT("Nick_Stored_t::IgnoreAdd", (in));
    if (Host().empty())
    {
	// Not stored nick
	if (!Parent->nickserv.IsStored(in))
	{
	    RET(false);
	}
	
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
	if (i_ignore.find(in.LowerCase())!=i_ignore.end())
	{
	    RET(false);
	}

	MCB(i_ignore.size());
	i_ignore.insert(in.LowerCase());
	MCE(i_ignore.size());
	RET(true);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).IgnoreAdd(in);
	RET(retval);
    }
}


unsigned int Nick_Stored_t::IgnoreDel(const mstring& in)
{
    FT("Nick_Stored_t::IgnoreDel", (in));
    unsigned int retval = 0;
    if (Host().empty())
    {
	vector<mstring> chunked;
	set<mstring>::iterator iter;
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
	for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
	    if (in.Matches(*iter, true))
	    {
		chunked.push_back(*iter);
	    }

	MCB(i_ignore.size());
	for (unsigned int i=0; i<chunked.size(); i++)
	    i_ignore.erase(chunked[i].LowerCase());
	MCE(i_ignore.size());
	retval = chunked.size();
    }
    else
    {
	retval = Parent->nickserv.GetStored(i_Host).IgnoreDel(in);
    }
    RET(retval);
}


bool Nick_Stored_t::IsIgnore(const mstring& in)
{
    FT("Nick_Stored_t::IsIgnore", (in));
    if (Host().empty())
    {
	set<mstring>::iterator iter;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
	for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
	    if (in.Matches(*iter, true))
	    {
		RET(true);
	    }
	RET(false);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).IsIgnore(in);
	RET(retval);
    }
}


bool Nick_Stored_t::Protect()
{
    NFT("Nick_Stored_t::Protect");
    if (Parent->nickserv.LCK_Protect())
    {
	RET(Parent->nickserv.DEF_Protect());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Protect"));
	RET(i_Protect);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).Protect();
	RET(retval);
    }
}


void Nick_Stored_t::Protect(const bool in)
{
    FT("Nick_Stored_t::Protect", (in));
    if (Host().empty())
    {
	if (!L_Protect())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Protect"));
	    MCB(i_Protect);
	    i_Protect = in;
	    MCE(i_Protect);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).Protect(in);
    }
}


bool Nick_Stored_t::L_Protect()
{
    NFT("Nick_Stored_t::L_Protect");
    if (Parent->nickserv.LCK_Protect())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Protect"));
	RET(l_Protect);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).L_Protect();
	RET(retval);
    }
}


void Nick_Stored_t::L_Protect(const bool in)
{
    FT("Nick_Stored_t::L_Protect", (in));
    if (Host().empty())
    {
	if (!Parent->nickserv.LCK_Protect())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Protect"));
	    MCB(l_Protect);
	    l_Protect = in;
	    MCE(l_Protect);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).L_Protect(in);
    }
}


bool Nick_Stored_t::Secure()
{
    NFT("Nick_Stored_t::Secure");
    if (Parent->nickserv.LCK_Secure())
    {
	RET(Parent->nickserv.DEF_Secure());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Secure"));
	RET(i_Secure);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).Secure();
	RET(retval);
    }
}


void Nick_Stored_t::Secure(const bool in)
{
    FT("Nick_Stored_t::Secure", (in));
    if (Host().empty())
    {
	if (!L_Secure())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Secure"));
	    MCB(i_Secure);
	    i_Secure = in;
	    MCE(i_Secure);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).Secure(in);
    }
}


bool Nick_Stored_t::L_Secure()
{
    NFT("Nick_Stored_t::L_Secure");
    if (Parent->nickserv.LCK_Secure())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Secure"));
	RET(l_Secure);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).L_Secure();
	RET(retval);
    }
}


void Nick_Stored_t::L_Secure(const bool in)
{
    FT("Nick_Stored_t::L_Secure", (in));
    if (Host().empty())
    {
	if (!Parent->nickserv.LCK_Secure())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Secure"));
	    MCB(l_Secure);
	    l_Secure = in;
	    MCE(l_Secure);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).L_Secure(in);
    }
}


bool Nick_Stored_t::NoExpire()
{
    NFT("Nick_Stored_t::NoExpire");
    if (Parent->nickserv.LCK_NoExpire())
    {
	RET(Parent->nickserv.DEF_NoExpire());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_NoExpire"));
	RET(i_NoExpire);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).NoExpire();
	RET(retval);
    }
}


void Nick_Stored_t::NoExpire(const bool in)
{
    FT("Nick_Stored_t::NoExpire", (in));
    if (Host().empty())
    {
	if (!L_NoExpire())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_NoExpire"));
	    MCB(i_NoExpire);
	    i_NoExpire = in;
	    MCE(i_NoExpire);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).NoExpire(in);
    }
}


bool Nick_Stored_t::L_NoExpire()
{
    NFT("Nick_Stored_t::L_NoExpire");
    if (Parent->nickserv.LCK_NoExpire())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_NoExpire"));
	RET(l_NoExpire);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).L_NoExpire();
	RET(retval);
    }
}


void Nick_Stored_t::L_NoExpire(const bool in)
{
    FT("Nick_Stored_t::L_NoExpire", (in));
    if (Host().empty())
    {
	if (!Parent->nickserv.LCK_NoExpire())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_NoExpire"));
	    MCB(l_NoExpire);
	    l_NoExpire = in;
	    MCE(l_NoExpire);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).L_NoExpire(in);
    }
}


bool Nick_Stored_t::NoMemo()
{
    NFT("Nick_Stored_t::NoMemo");
    if (Parent->nickserv.LCK_NoMemo())
    {
	RET(Parent->nickserv.DEF_NoMemo());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_NoMemo"));
	RET(i_NoMemo);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).NoMemo();
	RET(retval);
    }
}


void Nick_Stored_t::NoMemo(const bool in)
{
    FT("Nick_Stored_t::NoMemo", (in));
    if (Host().empty())
    {
	if (!L_NoMemo())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_NoMemo"));
	    MCB(i_NoMemo);
	    i_NoMemo = in;
	    MCE(i_NoMemo);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).NoMemo(in);
    }
}


bool Nick_Stored_t::L_NoMemo()
{
    NFT("Nick_Stored_t::L_NoMemo");
    if (Parent->nickserv.LCK_NoMemo())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_NoMemo"));
	RET(l_NoMemo);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).L_NoMemo();
	RET(retval);
    }
}


void Nick_Stored_t::L_NoMemo(const bool in)
{
    FT("Nick_Stored_t::L_NoMemo", (in));
    if (Host().empty())
    {
	if (!Parent->nickserv.LCK_NoMemo())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_NoMemo"));
	    MCB(l_NoMemo);
	    l_NoMemo = in;
	    MCE(l_NoMemo);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).L_NoMemo(in);
    }
}


bool Nick_Stored_t::Private()
{
    NFT("Nick_Stored_t::Private");
    if (Parent->nickserv.LCK_Private())
    {
	RET(Parent->nickserv.DEF_Private());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Private"));
	RET(i_Private);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).Private();
	RET(retval);
    }
}


void Nick_Stored_t::Private(const bool in)
{
    FT("Nick_Stored_t::Private", (in));
    if (Host().empty())
    {
	if (!L_Private())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Private"));
	    MCB(i_Private);
	    i_Private = in;
	    MCE(i_Private);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).Private(in);
    }
}


bool Nick_Stored_t::L_Private()
{
    NFT("Nick_Stored_t::L_Private");
    if (Parent->nickserv.LCK_Private())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Private"));
	RET(l_Private);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).L_Private();
	RET(retval);
    }
}


void Nick_Stored_t::L_Private(const bool in)
{
    FT("Nick_Stored_t::L_Private", (in));
    if (Host().empty())
    {
	if (!Parent->nickserv.LCK_Private())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Private"));
	    MCB(l_Private);
	    l_Private = in;
	    MCE(l_Private);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).L_Private(in);
    }
}


bool Nick_Stored_t::PRIVMSG()
{
    NFT("Nick_Stored_t::PRIVMSG");
    if (Parent->nickserv.LCK_PRIVMSG())
    {
	RET(Parent->nickserv.DEF_PRIVMSG());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_PRIVMSG"));
	RET(i_PRIVMSG);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).PRIVMSG();
	RET(retval);
    }
}


void Nick_Stored_t::PRIVMSG(const bool in)
{
    FT("Nick_Stored_t::PRIVMSG", (in));
    if (Host().empty())
    {
	if (!L_PRIVMSG())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_PRIVMSG"));
	    MCB(i_PRIVMSG);
	    i_PRIVMSG = in;
	    MCE(i_PRIVMSG);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).PRIVMSG(in);
    }
}


bool Nick_Stored_t::L_PRIVMSG()
{
    NFT("Nick_Stored_t::L_PRIVMSG");
    if (Parent->nickserv.LCK_PRIVMSG())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_PRIVMSG"));
	RET(l_PRIVMSG);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).L_PRIVMSG();
	RET(retval);
    }
}


void Nick_Stored_t::L_PRIVMSG(const bool in)
{
    FT("Nick_Stored_t::L_PRIVMSG", (in));
    if (Host().empty())
    {
	if (!Parent->nickserv.LCK_PRIVMSG())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_PRIVMSG"));
	    MCB(l_PRIVMSG);
	    l_PRIVMSG = in;
	    MCE(l_PRIVMSG);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).L_PRIVMSG(in);
    }
}


mstring Nick_Stored_t::Language()
{
    NFT("Nick_Stored_t::Language");
    if (Parent->nickserv.LCK_Language())
    {
	RET(Parent->nickserv.DEF_Language().UpperCase());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Language"));
	RET(i_Language);
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).Language();
	RET(retval);
    }
}


void Nick_Stored_t::Language(const mstring& in)
{
    FT("Nick_Stored_t::Language", (in));
    if (Host().empty())
    {
	if (!L_Language())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Language"));
	    MCB(i_Language);
	    i_Language = in.UpperCase();
	    MCE(i_Language);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).Language(in);
    }
}


bool Nick_Stored_t::L_Language()
{
    NFT("Nick_Stored_t::L_Language");
    if (Parent->nickserv.LCK_Language())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Language"));
	RET(l_Language);
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).L_Language();
	RET(retval);
    }
}


void Nick_Stored_t::L_Language(const bool in)
{
    FT("Nick_Stored_t::L_Language", (in));
    if (Host().empty())
    {
	if (!Parent->nickserv.LCK_Language())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Language"));
	    MCB(l_Language);
	    l_Language = in;
	    MCE(l_Language);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).L_Language(in);
    }
}


bool Nick_Stored_t::Suspended()
{
    NFT("Nick_Stored_t::Suspended");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
	RET(!Suspend_By().empty());
    }
    else
    {
	bool retval = Parent->nickserv.GetStored(i_Host).Suspended();
	RET(retval);
    }
}


mstring Nick_Stored_t::Suspend_By()
{
    NFT("Nick_Stored_t::Suspend_By");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
	RET(i_Suspend_By);
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).Suspend_By();
	RET(retval);
    }
}


mDateTime Nick_Stored_t::Suspend_Time()
{
    NFT("Nick_Stored_t::Suspend_Time");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_Time"));
	RET(i_Suspend_Time);
    }
    else
    {
	mDateTime retval = Parent->nickserv.GetStored(i_Host).Suspend_Time();
	RET(retval);
    }
}


bool Nick_Stored_t::Forbidden() const
{
    NFT("Nick_Stored_t::Forbidden");
    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Forbidden"));
    RET(i_Forbidden);
}

void Nick_Stored_t::SendPic(const mstring& nick)
{
    FT("Nick_Stored_t::SendPic", (nick));

    // DccEngine.PicSend(nick, PicNum());
}


unsigned long Nick_Stored_t::PicNum()
{
    NFT("PicNum");
    if (Host().empty())
    {
	if (!Parent->nickserv.PicExt().empty())
	{
	    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Picture"));
	    RET(i_Picture);
	}
	RET(0);
    }
    else
    {
	unsigned long retval = Parent->nickserv.GetStored(i_Host).PicNum();
	RET(retval);
    }
}


void Nick_Stored_t::GotPic(const unsigned long picnum)
{
    FT("Nick_Stored_t::GotPic", (picnum));
    if (Host().empty())
    {
	if (!Parent->nickserv.PicExt().empty())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Picture"));
	    MCB(i_Picture);
	    i_Picture = picnum;
	    MCE(i_Picture);
	}
    }
    else
    {
	Parent->nickserv.GetStored(i_Host).GotPic(picnum);
    }
}


bool Nick_Stored_t::IsOnline()
{
    NFT("Nick_Stored_t::IsOnline");

    if (Parent->nickserv.IsLive(i_Name))
    {
	// Not secure and recognized
	// or not suspended and identified
	if ((!Suspended() && Parent->nickserv.GetLive(i_Name).IsIdentified()) ||
	    (!Secure() && Parent->nickserv.GetLive(i_Name).IsRecognized()))
	{
	    RET(true);
	}
    }
    RET(false);
}


mDateTime Nick_Stored_t::LastAllSeenTime()
{
    NFT("Nick_Stored_t::LastAllSeenTime");
    if (IsOnline())
    {
	RET(mDateTime::CurrentDateTime());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	mDateTime lastseen = i_LastSeenTime;
	unsigned int i;
	for (i=0; i<Siblings(); i++)
	{
	    if (Parent->nickserv.GetStored(Sibling(i)).LastSeenTime() > lastseen)
		lastseen = Parent->nickserv.GetStored(Sibling(i)).LastSeenTime();
	}
	RET(lastseen);
    }
    else
    {
	mDateTime retval = Parent->nickserv.GetStored(i_Host).LastAllSeenTime();
	RET(retval);
    }
}


mDateTime Nick_Stored_t::LastSeenTime()
{
    NFT("Nick_Stored_t::LastSeenTime");
    if (IsOnline())
    {
	RET(mDateTime::CurrentDateTime());
    }
    else
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	RET(i_LastSeenTime);
    }
}


mstring Nick_Stored_t::LastRealName()
{
    NFT("Nick_Stored_t::LastRealName");
    if (IsOnline())
    {
	mstring retval = Parent->nickserv.GetLive(i_Name).RealName();
	RET(retval);
    }
    else
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastRealName"));
	RET(i_LastRealName);
    }
}


mstring Nick_Stored_t::LastAllMask()
{
    NFT("Nick_Stored_t::LastAllMask");
    if (IsOnline())
    {
	RET(Parent->getMessage("VALS/ONLINE"));
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	mDateTime lastseen = i_LastSeenTime;
	mstring lastmask = Name() + "!" + LastMask();
	unsigned int i;
	for (i=0; i<Siblings(); i++)
	{
	    if (Parent->nickserv.GetStored(Sibling(i)).IsOnline())
	    {
		RET(Parent->getMessage("VALS/ONLINE"));
	    }
	    if (Parent->nickserv.GetStored(Sibling(i)).LastSeenTime() > lastseen)
	    {
		lastseen = Parent->nickserv.GetStored(Sibling(i)).LastSeenTime();
		lastmask = Parent->nickserv.GetStored(Sibling(i)).Name() + "!" +
				Parent->nickserv.GetStored(Sibling(i)).LastMask();
	    }
	}
	RET(lastmask);
    }
    else
    {
	mstring retval = Parent->nickserv.GetStored(i_Host).LastAllMask();
	RET(retval);
    }
}


mstring Nick_Stored_t::LastMask()
{
    NFT("Nick_Stored_t::LastMask");
    if (IsOnline())
    {
	RET(Parent->getMessage("VALS/ONLINE"));
    }
    else
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastMask"));
	RET(i_LastMask);
    }
}


mstring Nick_Stored_t::LastQuit()
{
    NFT("Nick_Stored_t::LastQuit");
    if (IsOnline())
    {
	RET("");
    }
    else
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastQuit"));
	RET(i_LastQuit);
    }
}


void Nick_Stored_t::Quit(const mstring& message)
{
    FT("Nick_Stored_t::Quit", (message));
    
    // Dont whinge here, because they may not be
    // ident'd, in which case IsOnline() == false.
    if (IsOnline())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_LastQuit"));
	MCB(i_LastSeenTime);
	CB(1, i_LastQuit);
	i_LastSeenTime = mDateTime::CurrentDateTime();
	i_LastQuit = message;
	CE(1, i_LastQuit);
	MCE(i_LastSeenTime);
    }
}

size_t Nick_Stored_t::MyChannels() const
{
    NFT("Nick_Stored_t::MyChannels");

    size_t count = 0;
    ChanServ::stored_t::const_iterator i;
    RLOCK(("ChanServ", "stored"));
    for (i=Parent->chanserv.StoredBegin(); i!=Parent->chanserv.StoredEnd(); i++)
    {
	if (i->second.Founder().IsSameAs(i_Name, true) ||
	    i->second.CoFounder().IsSameAs(i_Name, true))
	    count++;
    }
    RET(count);
}

SXP::Tag Nick_Stored_t::tag_Nick_Stored_t("Nick_Stored_t");
SXP::Tag Nick_Stored_t::tag_Name("Name");
SXP::Tag Nick_Stored_t::tag_RegTime("RegTime");
SXP::Tag Nick_Stored_t::tag_Password("Password");
SXP::Tag Nick_Stored_t::tag_Email("EMail");
SXP::Tag Nick_Stored_t::tag_URL("URL");
SXP::Tag Nick_Stored_t::tag_ICQ("ICQ");
SXP::Tag Nick_Stored_t::tag_Description("Description");
SXP::Tag Nick_Stored_t::tag_Comment("Comment");
SXP::Tag Nick_Stored_t::tag_Host("Host");
SXP::Tag Nick_Stored_t::tag_set_Protect("SET_Protect");
SXP::Tag Nick_Stored_t::tag_set_Secure("SET_Secure");
SXP::Tag Nick_Stored_t::tag_set_NoExpire("SET_NoExpire");
SXP::Tag Nick_Stored_t::tag_set_NoMemo("SET_NoMemo");
SXP::Tag Nick_Stored_t::tag_set_Private("SET_Private");
SXP::Tag Nick_Stored_t::tag_set_PRIVMSG("SET_PRIVMSG");
SXP::Tag Nick_Stored_t::tag_set_Language("SET_Language");
SXP::Tag Nick_Stored_t::tag_Forbidden("Forbidden");
SXP::Tag Nick_Stored_t::tag_lock_Protect("LOCK_Protect");
SXP::Tag Nick_Stored_t::tag_lock_Secure("LOCK_Secure");
SXP::Tag Nick_Stored_t::tag_lock_NoExpire("LOCK_NoExpire");
SXP::Tag Nick_Stored_t::tag_lock_NoMemo("LOCK_NoMemo");
SXP::Tag Nick_Stored_t::tag_lock_Private("LOCK_Private");
SXP::Tag Nick_Stored_t::tag_lock_PRIVMSG("LOCK_PRIVMSG");
SXP::Tag Nick_Stored_t::tag_lock_Language("LOCK_Language");
SXP::Tag Nick_Stored_t::tag_Picture("Picture");
SXP::Tag Nick_Stored_t::tag_Suspend_By("Suspend_By");
SXP::Tag Nick_Stored_t::tag_Suspend_Time("Suspend_Time");
SXP::Tag Nick_Stored_t::tag_LastSeenTime("LastSeenTime");
SXP::Tag Nick_Stored_t::tag_LastRealName("LastRealName");
SXP::Tag Nick_Stored_t::tag_LastMask("LastMask");
SXP::Tag Nick_Stored_t::tag_LastQuit("LastQuit");
SXP::Tag Nick_Stored_t::tag_Access("Access");
SXP::Tag Nick_Stored_t::tag_Ignore("Ignore");
SXP::Tag Nick_Stored_t::tag_UserDef("UserDef");

void Nick_Stored_t::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Nick_Stored_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if( pElement->IsA(tag_UserDef) )
    {
	mstring *tmp = new mstring;
	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void Nick_Stored_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Nick_Stored_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
	if( pElement->IsA(tag_Name) )		pElement->Retrieve(i_Name);
	if( pElement->IsA(tag_RegTime) )	pElement->Retrieve(i_RegTime);
	if( pElement->IsA(tag_Password) )	pElement->Retrieve(i_Password);
	if( pElement->IsA(tag_Email) )		pElement->Retrieve(i_Email);
	if( pElement->IsA(tag_URL) )		pElement->Retrieve(i_URL);
	if( pElement->IsA(tag_ICQ) )		pElement->Retrieve(i_ICQ);
	if( pElement->IsA(tag_Description) )	pElement->Retrieve(i_Description);
	if( pElement->IsA(tag_Comment) )	pElement->Retrieve(i_Comment);
	if( pElement->IsA(tag_Host) )		pElement->Retrieve(i_Host);
	if( pElement->IsA(tag_set_Protect) )	pElement->Retrieve(i_Protect);
	if( pElement->IsA(tag_set_Secure) )	pElement->Retrieve(i_Secure);
	if( pElement->IsA(tag_set_NoExpire) )	pElement->Retrieve(i_NoExpire);
	if( pElement->IsA(tag_set_NoMemo) )	pElement->Retrieve(i_NoMemo);
	if( pElement->IsA(tag_set_Private) )	pElement->Retrieve(i_Private);
	if( pElement->IsA(tag_set_PRIVMSG) )	pElement->Retrieve(i_PRIVMSG);
	if( pElement->IsA(tag_set_Language) )	pElement->Retrieve(i_Language);
	if( pElement->IsA(tag_Forbidden) )	pElement->Retrieve(i_Forbidden);
	if( pElement->IsA(tag_lock_Protect) )	pElement->Retrieve(l_Protect);
	if( pElement->IsA(tag_lock_Secure) )	pElement->Retrieve(l_Secure);
	if( pElement->IsA(tag_lock_NoExpire) )	pElement->Retrieve(l_NoExpire);
	if( pElement->IsA(tag_lock_NoMemo) )	pElement->Retrieve(l_NoMemo);
	if( pElement->IsA(tag_lock_Private) )	pElement->Retrieve(l_Private);
	if( pElement->IsA(tag_lock_PRIVMSG) )	pElement->Retrieve(l_PRIVMSG);
	if( pElement->IsA(tag_lock_Language) )	pElement->Retrieve(l_Language);
	if( pElement->IsA(tag_Picture) )	pElement->Retrieve(i_Picture);
	if( pElement->IsA(tag_Suspend_By) )	pElement->Retrieve(i_Suspend_By);
	if( pElement->IsA(tag_Suspend_Time) )	pElement->Retrieve(i_Suspend_Time);
	if( pElement->IsA(tag_LastSeenTime) )	pElement->Retrieve(i_LastSeenTime);
	if( pElement->IsA(tag_LastRealName) )	pElement->Retrieve(i_LastRealName);
	if( pElement->IsA(tag_LastMask) )	pElement->Retrieve(i_LastMask);
	if( pElement->IsA(tag_LastQuit) )	pElement->Retrieve(i_LastQuit);

    if( pElement->IsA(tag_Access) )
    {
        mstring tmp;
        pElement->Retrieve(tmp);
        i_access.insert(tmp);
    }
    if( pElement->IsA(tag_Ignore) )
    {
        mstring tmp;
        pElement->Retrieve(tmp);
        i_ignore.insert(tmp);
    }
}

void Nick_Stored_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    //TODO: Add your source code here
	pOut->BeginObject(tag_Nick_Stored_t, attribs);

	// Only way to ENSURE the data wont change.
	WLOCK(("NickServ", "stored", i_Name.LowerCase()));
	pOut->WriteElement(tag_Name, i_Name);
	pOut->WriteElement(tag_RegTime, i_RegTime);
	pOut->WriteElement(tag_Password, i_Password);
	pOut->WriteElement(tag_Email, i_Email);
	pOut->WriteElement(tag_URL, i_URL);
	pOut->WriteElement(tag_ICQ, i_ICQ);
	pOut->WriteElement(tag_Description, i_Description);
	pOut->WriteElement(tag_Comment, i_Comment);
	pOut->WriteElement(tag_Host, i_Host);
	pOut->WriteElement(tag_set_Protect, i_Protect);
	pOut->WriteElement(tag_set_Secure, i_Secure);
	pOut->WriteElement(tag_set_NoExpire, i_NoExpire);
	pOut->WriteElement(tag_set_NoMemo, i_NoMemo);
	pOut->WriteElement(tag_set_Private, i_Private);
	pOut->WriteElement(tag_set_PRIVMSG, i_PRIVMSG);
	pOut->WriteElement(tag_set_Language, i_Language);
	pOut->WriteElement(tag_Forbidden, i_Forbidden);
	pOut->WriteElement(tag_lock_Protect, l_Protect);
	pOut->WriteElement(tag_lock_Secure, l_Secure);
	pOut->WriteElement(tag_lock_NoExpire, l_NoExpire);
	pOut->WriteElement(tag_lock_NoMemo, l_NoMemo);
	pOut->WriteElement(tag_lock_Private, l_Private);
	pOut->WriteElement(tag_lock_PRIVMSG, l_PRIVMSG);
	pOut->WriteElement(tag_lock_Language, l_Language);
	pOut->WriteElement(tag_Picture, i_Picture);
	pOut->WriteElement(tag_Suspend_By, i_Suspend_By);
	pOut->WriteElement(tag_Suspend_Time, i_Suspend_Time);
	pOut->WriteElement(tag_LastSeenTime, i_LastSeenTime);
	pOut->WriteElement(tag_LastRealName, i_LastRealName);
	pOut->WriteElement(tag_LastMask, i_LastMask);
	pOut->WriteElement(tag_LastQuit, i_LastQuit);

	set<mstring>::const_iterator iter2;
	for(iter2=i_access.begin(); iter2!=i_access.end(); iter2++)
	{
	    pOut->WriteElement(tag_Access, *iter2);
	}	
	for(iter2=i_ignore.begin(); iter2!=i_ignore.end(); iter2++)
	{
	    pOut->WriteElement(tag_Ignore, *iter2);
	}	

        map<mstring,mstring>::const_iterator iter;
        for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
        {
            pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
        }

	pOut->EndObject(tag_Nick_Stored_t);
}

size_t Nick_Stored_t::Usage()
{
    size_t retval = 0;

    // Only way to ENSURE the data wont change.
    WLOCK(("NickServ", "stored", i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_RegTime.Internal());
    retval += i_Password.capacity();
    retval += i_Email.capacity();
    retval += i_URL.capacity();
    retval += i_ICQ.capacity();
    retval += i_Description.capacity();
    retval += i_Comment.capacity();
    retval += i_Host.capacity();
    
    set<mstring>::iterator i;
    for (i=i_slaves.begin(); i!=i_slaves.end(); i++)
    {
	retval += i->capacity();
    }
    for (i=i_access.begin(); i!=i_access.end(); i++)
    {
	retval += i->capacity();
    }
    for (i=i_ignore.begin(); i!=i_ignore.end(); i++)
    {
	retval += i->capacity();
    }

    retval += sizeof(i_Protect);
    retval += sizeof(l_Protect);
    retval += sizeof(i_Secure);
    retval += sizeof(l_Secure);
    retval += sizeof(i_NoExpire);
    retval += sizeof(l_NoExpire);
    retval += sizeof(i_NoMemo);
    retval += sizeof(l_NoMemo);
    retval += sizeof(i_Private);
    retval += sizeof(l_Private);
    retval += sizeof(i_PRIVMSG);
    retval += sizeof(l_PRIVMSG);
    retval += i_Language.capacity();
    retval += sizeof(l_Language);
    retval += sizeof(i_Forbidden);
    retval += sizeof(i_Picture);

    retval += i_Suspend_By.capacity();
    retval += sizeof(i_Suspend_Time.Internal());

    retval += sizeof(i_LastSeenTime.Internal());
    retval += i_LastRealName.capacity();
    retval += i_LastMask.capacity();
    retval += i_LastQuit.capacity();

    map<mstring,mstring>::iterator j;
    for (j=i_UserDef.begin(); j!=i_UserDef.end(); j++)
    {
	retval += j->first.capacity();
	retval += j->second.capacity();
    }

    return retval;
}

void Nick_Stored_t::DumpB()
{
    MB(0, (i_Name, i_RegTime, i_Password, i_Email, i_URL, i_ICQ,
	i_Description, i_Comment, i_Host, i_slaves.size(), i_access.size(),
	i_ignore.size(), i_Protect, l_Protect, i_Secure, l_Secure));
    MB(16, (i_NoExpire, l_NoExpire, i_NoMemo, l_NoMemo, i_Private, l_Private,
	i_PRIVMSG, l_PRIVMSG, i_Language, l_Language, i_Forbidden, i_Picture,
	i_Suspend_By, i_Suspend_Time, i_LastSeenTime, i_LastRealName));
    MB(32, (i_LastMask, i_LastQuit, i_UserDef.size()));
}

void Nick_Stored_t::DumpE()
{
    ME(0, (i_Name, i_RegTime, i_Password, i_Email, i_URL, i_ICQ,
	i_Description, i_Comment, i_Host, i_slaves.size(), i_access.size(),
	i_ignore.size(), i_Protect, l_Protect, i_Secure, l_Secure));
    ME(16, (i_NoExpire, l_NoExpire, i_NoMemo, l_NoMemo, i_Private, l_Private,
	i_PRIVMSG, l_PRIVMSG, i_Language, l_Language, i_Forbidden, i_Picture,
	i_Suspend_By, i_Suspend_Time, i_LastSeenTime, i_LastRealName));
    ME(32, (i_LastMask, i_LastQuit, i_UserDef.size()));
}

// =======================================================================

NickServ::NickServ()
{
    NFT("NickServ::NickServ");
    messages=true;
}

mstring NickServ::findnextnick(const mstring& in)
{
    FT("NickServ::findnextnick", (in));
    mstring retval(in);
    // Amountof nicknames it will try, only
    // for the guest????? method.
    unsigned int i, attempts = 64;

    if (Parent->nickserv.Append_Rename())
    {
	for (i=0; i<Parent->nickserv.Suffixes().length(); i++)
	{
	    while (retval.length() < Parent->server.proto.NickLen())
	    {
		retval << Parent->nickserv.Suffixes()[i];
		if (!Parent->nickserv.IsLiveAll(retval) &&
		    !Parent->nickserv.IsStored(retval))
		{
		    RET(retval);
		}
	    }
	    retval = in;
	}
    }
    else
    {
	srand(time(NULL));
	for (i=0; i<attempts; i++)
	{
	    retval.erase();
	    retval.Format("%s%05d",
		    Parent->nickserv.Suffixes().c_str(),
		    rand() % 99999);
	    if (!Parent->nickserv.IsLiveAll(retval) &&
		!Parent->nickserv.IsStored(retval))
	    {
		RET(retval);
	    }
	}
    }
    RET("");
}

void NickServ::AddCommands()
{
    NFT("NickServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* CUR*", Parent->commserv.REGD_Name(), NickServ::do_access_Current);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* ADD", Parent->commserv.REGD_Name(), NickServ::do_access_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* DEL*", Parent->commserv.REGD_Name(), NickServ::do_access_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* ERA*", Parent->commserv.REGD_Name(), NickServ::do_access_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* LIST", Parent->commserv.REGD_Name(), NickServ::do_access_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* VIEW", Parent->commserv.REGD_Name(), NickServ::do_access_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* H*LP", Parent->commserv.REGD_Name(), do_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* ADD", Parent->commserv.REGD_Name(), NickServ::do_ignore_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* DEL*", Parent->commserv.REGD_Name(), NickServ::do_ignore_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* ERA*", Parent->commserv.REGD_Name(), NickServ::do_ignore_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* LIST", Parent->commserv.REGD_Name(), NickServ::do_ignore_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* VIEW", Parent->commserv.REGD_Name(), NickServ::do_ignore_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* H*LP", Parent->commserv.REGD_Name(), do_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PASS*", Parent->commserv.REGD_Name(), NickServ::do_set_Password);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* EMAIL", Parent->commserv.REGD_Name(), NickServ::do_set_Email);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* URL", Parent->commserv.REGD_Name(), NickServ::do_set_URL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* ICQ", Parent->commserv.REGD_Name(), NickServ::do_set_ICQ);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* DESC*", Parent->commserv.REGD_Name(), NickServ::do_set_Description);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* COMM*", Parent->commserv.SOP_Name(), NickServ::do_set_Comment);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PIC*", Parent->commserv.REGD_Name(), NickServ::do_set_Picture);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PROT*", Parent->commserv.REGD_Name(), NickServ::do_set_Protect);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* SEC*", Parent->commserv.REGD_Name(), NickServ::do_set_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* NOEX*", Parent->commserv.SOP_Name(), NickServ::do_set_NoExpire);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* NOMEMO", Parent->commserv.REGD_Name(), NickServ::do_set_NoMemo);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PRIVM*", Parent->commserv.REGD_Name(), NickServ::do_set_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* *MSG", Parent->commserv.REGD_Name(), NickServ::do_set_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PRIV*", Parent->commserv.REGD_Name(), NickServ::do_set_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* LANG*", Parent->commserv.REGD_Name(), NickServ::do_set_Language);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* H*LP", Parent->commserv.REGD_Name(), do_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK PROT*", Parent->commserv.SOP_Name(), NickServ::do_lock_Protect);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK SEC*", Parent->commserv.SOP_Name(), NickServ::do_lock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK NOMEMO", Parent->commserv.SOP_Name(), NickServ::do_lock_NoMemo);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK PRIVM*", Parent->commserv.SOP_Name(), NickServ::do_lock_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK *MSG", Parent->commserv.SOP_Name(), NickServ::do_lock_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK PRIV*", Parent->commserv.SOP_Name(), NickServ::do_lock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK LANG*", Parent->commserv.SOP_Name(), NickServ::do_lock_Language);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK H*LP", Parent->commserv.SOP_Name(), do_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PROT*", Parent->commserv.SOP_Name(), NickServ::do_unlock_Protect);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Parent->commserv.SOP_Name(), NickServ::do_unlock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK NOMEMO", Parent->commserv.SOP_Name(), NickServ::do_unlock_NoMemo);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PRIVM*", Parent->commserv.SOP_Name(), NickServ::do_unlock_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK *MSG", Parent->commserv.SOP_Name(), NickServ::do_unlock_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Parent->commserv.SOP_Name(), NickServ::do_unlock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK LANG*", Parent->commserv.SOP_Name(), NickServ::do_unlock_Language);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK H*LP", Parent->commserv.SOP_Name(), do_2param);

    Parent->commands.AddSystemCommand(GetInternalName(),
		"H*LP", Parent->commserv.ALL_Name(), NickServ::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"REG*", Parent->commserv.ALL_Name(), NickServ::do_Register);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"DROP*", Parent->commserv.REGD_Name(), NickServ::do_Drop);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LINK*", Parent->commserv.ALL_Name(), NickServ::do_Link);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"U*LIN*", Parent->commserv.REGD_Name(), NickServ::do_UnLink);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"HOST", Parent->commserv.REGD_Name(), NickServ::do_Host);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"*SLAV*", Parent->commserv.REGD_Name(), NickServ::do_Slaves);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ID*", Parent->commserv.ALL_Name(), NickServ::do_Identify);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SID*", Parent->commserv.ALL_Name(), NickServ::do_Identify);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"INF*", Parent->commserv.ALL_Name(), NickServ::do_Info);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"GHOST*", Parent->commserv.ALL_Name(), NickServ::do_Ghost);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"REC*", Parent->commserv.ALL_Name(), NickServ::do_Recover);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LIST*", Parent->commserv.ALL_Name(), NickServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SEND*", Parent->commserv.ALL_Name(), NickServ::do_Send);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SUSP*", Parent->commserv.SOP_Name(), NickServ::do_Suspend);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNSUS*", Parent->commserv.SOP_Name(), NickServ::do_UnSuspend);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"FORB*", Parent->commserv.SOP_Name(), NickServ::do_Forbid);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"GET*PASS*", Parent->commserv.SOP_Name(), NickServ::do_Getpass);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LIVE*", Parent->commserv.SOP_Name(), NickServ::do_Live);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET*", Parent->commserv.REGD_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK", Parent->commserv.REGD_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK", Parent->commserv.REGD_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC*", Parent->commserv.REGD_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN*", Parent->commserv.REGD_Name(), do_1_2param);
}

void NickServ::RemCommands()
{
    NFT("NickServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* CUR*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* ERA*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* ERA*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PASS*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* EMAIL", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* URL", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* ICQ", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* DESC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* COMM*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PIC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PROT*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* SEC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* NOEX*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* NOMEMO", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PRIVM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* *MSG", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PRIV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* LANG*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK PROT*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK NOMEMO", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK PRIVM*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK *MSG", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK LANG*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK H*LP", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PROT*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK NOMEMO", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PRIVM*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK *MSG", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK LANG*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK H*LP", Parent->commserv.SOP_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
		"H*LP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"REG*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"DROP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LINK*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"U*LIN*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"HOST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"*SLAV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ID*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"INF*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"GHOST*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"REC*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LIST*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SEND*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SUSP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNSUS*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"FORB*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"GET*PASS*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LIVE*", Parent->commserv.SOP_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN*", Parent->commserv.REGD_Name());
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::AddStored(Nick_Stored_t *in) throw(E_NickServ_Stored)
#else
void NickServ::AddStored(Nick_Stored_t *in)
#endif
{
    FT("NickServ::AddStored", ("(Nick_Stored_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Add, E_NickServ_Stored::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Stored", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Add, E_NickServ_Stored::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Stored", "Add", "Blank"));
	return;
#endif
    }

    WLOCK(("NickServ", "stored"));
    /* stored[in->Name().LowerCase()] = in; */
    stored[in->Name().LowerCase()] = *in;
}


#ifdef MAGICK_HAS_EXCEPTIONS
Nick_Stored_t &NickServ::GetStored(const mstring &in) const throw(E_NickServ_Stored)
#else
Nick_Stored_t &NickServ::GetStored(const mstring &in) const
#endif
{
    FT("NickServ::GetStored", (in));

    RLOCK(("NickServ", "stored", in.LowerCase()));
    NickServ::stored_t::const_iterator iter = stored.find(in.LowerCase());
    if (iter == stored.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Get, E_NickServ_Stored::T_NotFound, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Get", "NotFound", in));
	NRET(Nick_Stored_t &, GLOB_Nick_Stored_t);
#endif
    }
    /* if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Get, E_NickServ_Stored::T_Invalid, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Get", "Invalid", in));
	NRET(Nick_Stored_t &, GLOB_Nick_Stored_t);
#endif
    }
    if (iter->second->Name().empty()) */
    if (iter->second.Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Get, E_NickServ_Stored::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Get", "Blank", in));
	NRET(Nick_Stored_t &, GLOB_Nick_Stored_t);
#endif
    }

    /* NRET(Nick_Stored_t &, const_cast<Nick_Stored_t &>(*iter->second)); */
    NRET(Nick_Stored_t &, const_cast<Nick_Stored_t &>(iter->second));
}


#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::RemStored(const mstring &in) throw(E_NickServ_Stored)
#else
void NickServ::RemStored(const mstring &in)
#endif
{
    FT("NickServ::RemStored", (in));

    WLOCK(("NickServ", "stored"));
    NickServ::stored_t::iterator iter = stored.find(in.LowerCase());
    if (iter == stored.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Rem, E_NickServ_Stored::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Rem", "NotFound", in));
	return;
#endif
    }
    WLOCK2(("NickServ", "stored", iter->first));
    /* if (itr->second != NULL)
    {
	delete iter->second;
    } */
    stored.erase(iter);
}

bool NickServ::IsStored(const mstring& in)const
{
    FT("NickServ::IsStored", (in));
    RLOCK(("NickServ", "stored"));
    bool retval = (stored.find(in.LowerCase())!=stored.end());
    RET(retval);
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::AddLive(Nick_Live_t *in) throw(E_NickServ_Live)
#else
void NickServ::AddLive(Nick_Live_t *in)
#endif
{
    FT("NickServ::AddLive", ("(Nick_Live_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Add, E_NickServ_Live::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Live", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Add, E_NickServ_Live::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Live", "Add", "Blank"));
	return
#endif
    }

    WLOCK(("NickServ", "live"));
    /* live[in->Name().LowerCase()] = in; */
    live[in->Name().LowerCase()] = *in;
}


#ifdef MAGICK_HAS_EXCEPTIONS
Nick_Live_t &NickServ::GetLive(const mstring &in) const throw(E_NickServ_Live)
#else
Nick_Live_t &NickServ::GetLive(const mstring &in) const
#endif
{
    FT("NickServ::GetLive", (in));

    RLOCK(("NickServ", "live", in.LowerCase()));
    NickServ::live_t::const_iterator iter = live.find(in.LowerCase());
    if (iter == live.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Get, E_NickServ_Live::T_NotFound, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Get", "NotFound", in));
	RET(Nick_Live_t &, GLOB_Nick_Live_t);
#endif
    }
    /* if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Get, E_NickServ_Live::T_Invalid, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Get", "Invalid", in));
	RET(Nick_Live_t &, GLOB_Nick_Live_t);
#endif
    }
    if (iter->second->Name().empty()) */
    if (iter->second.Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Get, E_NickServ_Live::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Get", "Blank", in));
	RET(Nick_Live_t &, GLOB_Nick_Live_t);
#endif
    }

    /* NRET(Nick_Live_t &, const_cast<Nick_Live_t &>(*iter->second)); */
    NRET(Nick_Live_t &, const_cast<Nick_Live_t &>(iter->second));
}


#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::RemLive(const mstring &in) throw(E_NickServ_Live)
#else
void NickServ::RemLive(const mstring &in)
#endif
{
    FT("NickServ::RemLive", (in));

    WLOCK(("NickServ", "live"));
    NickServ::live_t::iterator iter = live.find(in.LowerCase());
    if (iter == live.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Rem, E_NickServ_Live::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Rem", "Invalid", in));
	return;
#endif
    }
    WLOCK2(("NickServ", "live", iter->first));
    /* if (iter->second != NULL)
    {
	delete iter->second;
    } */
    live.erase(iter);
}

bool NickServ::IsLive(const mstring& in)const
{
    FT("NickServ::IsLive", (in));
    bool retval = false;
    RLOCK(("NickServ", "live", in.LowerCase()));
    NickServ::live_t::const_iterator i = live.find(in.LowerCase());
    if (i != live.end())
    {
	if (i->second.Squit().empty())
	    retval = true;
    }
    RET(retval);
}

bool NickServ::IsLiveAll(const mstring& in)const
{
    FT("NickServ::IsLiveAll", (in));
    RLOCK(("NickServ", "live"));
    bool retval = (live.find(in.LowerCase())!=live.end());
    RET(retval);
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::AddRecovered(const mstring &name, const mDateTime &in) throw(E_NickServ_Recovered)
#else
void NickServ::AddRecovered(const mstring &name, const mDateTime &in)
#endif
{
    FT("NickServ::AddRecovered", (name, in));

    if (name.empty() || in == mDateTime(0.0))
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Recovered(E_NickServ_Recovered::W_Add, E_NickServ_Recovered::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Recovered", "Add", "Blank"));
	return;
#endif
    }

    WLOCK(("NickServ", "recovered"));
    recovered[name.LowerCase()] = in;
}


#ifdef MAGICK_HAS_EXCEPTIONS
const mDateTime &NickServ::GetRecovered(const mstring &in) const throw(E_NickServ_Recovered)
#else
const mDateTime &NickServ::GetRecovered(const mstring &in) const
#endif
{
    FT("NickServ::GetRecovered", (in));

    RLOCK(("NickServ", "recovered", in.LowerCase()));
    map<mstring, mDateTime>::const_iterator iter = recovered.find(in.LowerCase());
    if (iter == recovered.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Recovered(E_NickServ_Recovered::W_Get, E_NickServ_Recovered::T_NotFound, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Recovered", "Get", "NotFound", in));
	RET(mDateTime &, GLOB_mDateTime);
#endif
    }
    if (iter->second == mDateTime(0.0))
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Recovered(E_NickServ_Recovered::W_Get, E_NickServ_Recovered::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Recovered", "Get", "Blank", in));
	RET(mDateTime &, GLOB_mDateTime);
#endif
    }

    NRET(mDateTime &, iter->second);
}


#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::RemRecovered(const mstring &in) throw(E_NickServ_Recovered)
#else
void NickServ::RemRecovered(const mstring &in)
#endif
{
    FT("NickServ::RemRecovered", (in));

    WLOCK(("NickServ", "recovered"));
    map<mstring, mDateTime>::iterator iter = recovered.find(in.LowerCase());
    if (iter == recovered.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Recovered(E_NickServ_Recovered::W_Rem, E_NickServ_Recovered::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Recovered", "Rem", "NotFound", in));
	return;
#endif
    }
    WLOCK2(("NickServ", "recovered", iter->first));
    recovered.erase(iter);
}

bool NickServ::IsRecovered(const mstring& in)const
{
    FT("NickServ::IsRecovered", (in));
    RLOCK(("NickServ", "recovered"));
    bool retval = (recovered.find(in.LowerCase())!=recovered.end());
    RET(retval);
}



void NickServ::execute(mstring& source, const mstring& msgtype, const mstring& params)
{
    mThread::ReAttach(tt_NickServ);
    FT("NickServ::execute", (source, msgtype, params));
    //okay this is the main nickserv command switcher

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

void NickServ::do_Help(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    mstring HelpTopic = Parent->nickserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector<mstring> help = Parent->getHelp(source, HelpTopic.UpperCase());
					
    unsigned int i;
    for (i=0; i<help.size(); i++)
	::send(mynick, source, help[i]);
}

void NickServ::do_Register(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Register", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring password = params.ExtractWord(2, " ");
    mstring email;
    if (params.WordCount(" ") > 2)
	email        = params.ExtractWord(3, " ");

    if (Parent->nickserv.IsStored(source))
    {
	NSEND(mynick, source, "NS_YOU_STATUS/ISSTORED");
    }
    else if (!Parent->nickserv.GetLive(source).HasMode("o") &&
		Parent->nickserv.GetLive(source).LastNickReg().SecondsSince() <
    		Parent->nickserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Parent->nickserv.Delay() -
		Parent->nickserv.GetLive(source).LastNickReg().SecondsSince(),
		source)));
    }
    else if (password.length() < 5 || password.IsSameAs(source, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
    }
    else
    {
	if (email.empty())
	{
	}
	else if (!email.Contains("@"))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
		Parent->getMessage(source, "NS_SET/EMAIL"), '@'));
	    return;
	}
	else if (email.WordCount("@") != 2)
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE", (
		Parent->getMessage(source, "NS_SET/EMAIL"), '@'));
	    return;
	}

	RLOCK(("NickServ", "live", source.LowerCase()));
	Nick_Live_t &live = Parent->nickserv.GetLive(source);
	live.SetLastNickReg();
	Nick_Stored_t tmp(source, password);
	tmp.AccessAdd(live.Mask(Nick_Live_t::U_H).After("!"));
	if (!email.empty())
	    tmp.Email(email);
	Parent->nickserv.AddStored(&tmp);
	live.Identify(password);
	Parent->nickserv.stats.i_Register++;
	SEND(mynick, source, "NS_YOU_COMMAND/REGISTERED", (
		live.Mask(Nick_Live_t::U_H).After("!")));
	LOG(LM_INFO, "NICKSERV/REGISTER", (
		live.Mask(Nick_Live_t::N_U_P_H),
		source));
    }
}

void NickServ::do_Drop(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Drop", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    unsigned long dropped = 0;
    if (params.WordCount(" ") < 2)
    {
	if (!Parent->nickserv.GetLive(source).IsIdentified())
	{
	    SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
				message, mynick));
	    return;
	}
	else
	{
	    Parent->nickserv.stats.i_Drop++;
	    dropped = Parent->nickserv.GetStored(source).Drop();
	    Parent->nickserv.RemStored(source);
	    Parent->nickserv.GetLive(source).UnIdentify();
	    NSEND(mynick, source, "NS_YOU_COMMAND/DROPPED");
	    LOG(LM_INFO, "NICKSERV/DROP", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		source, dropped-1));
	}
    }
    else if (Parent->nickserv.IsStored(params.ExtractWord(2, " ")))
    {
	mstring target = Parent->getSname(params.ExtractWord(2, " "));
	if ((Parent->nickserv.GetStored(target).IsSibling(source) &&
	    Parent->nickserv.GetLive(source).IsIdentified()) ||
	    (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	     Parent->commserv.GetList(Parent->commserv.SOP_Name()).IsOn(source) &&
	     !(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	     Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsIn(target))))
	{
	    Parent->nickserv.stats.i_Drop++;
	    dropped = Parent->nickserv.GetStored(target).Drop();
	    Parent->nickserv.RemStored(target);
	    if (!Parent->nickserv.IsStored(source))
		Parent->nickserv.GetLive(source).UnIdentify();
	    SEND(mynick, source, "NS_OTH_COMMAND/DROPPED", (
				target));
	    LOG(LM_INFO, "NICKSERV/DROP", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		target, dropped-1));
	}
	else
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTYOURS", (
				target));
	}
    }
    else
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
					params.ExtractWord(2, " ")));
    }
}

void NickServ::do_Link(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Link", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring hostnick = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(hostnick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
							hostnick));
	return;
    }
    hostnick = Parent->getSname(hostnick);

    if (Parent->nickserv.GetStored(hostnick).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
					hostnick));
	return;
    }
    else if (!Parent->nickserv.GetLive(source).HasMode("o") &&
		Parent->nickserv.GetLive(source).LastNickReg().SecondsSince() <
    		Parent->nickserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Parent->nickserv.Delay() -
		Parent->nickserv.GetLive(source).LastNickReg().SecondsSince(),
		source)));
	return;
    }

    mDateTime regtime = mDateTime::CurrentDateTime();
    if (Parent->nickserv.IsStored(source))
    {
	if (!Parent->nickserv.GetLive(source).IsIdentified())
	{
	    SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
					message, mynick));
	    return;
	}
	regtime = Parent->nickserv.GetStored(source).RegTime();
    }

    if (Parent->nickserv.GetStored(hostnick).Slave(source, password, regtime))
    {
	Parent->nickserv.GetLive(source).SetLastNickReg();
	Parent->nickserv.stats.i_Link++;
	Parent->nickserv.GetLive(source).Identify(password);
	SEND(mynick, source, "NS_YOU_COMMAND/LINKED", (
					hostnick));
	LOG(LM_INFO, "NICKSERV/LINK", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		source, hostnick));
    }
    else
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
    }
}

void NickServ::do_UnLink(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_UnLink", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (!Parent->nickserv.GetLive(source).IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
						message, mynick));
	return;
    }

    if (params.WordCount(" ") > 1)
    {
	mstring target = params.ExtractWord(2, " ");

	
	if (!Parent->nickserv.IsStored(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
	    return;
	}

	target = Parent->getSname(target);
	if (Parent->nickserv.GetStored(target).Host().empty())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISHOST", (
							target));
	    return;
	}

	if (!(Parent->nickserv.GetStored(target).IsSibling(source) ||
		Parent->nickserv.GetStored(target).Host().IsSameAs(source, true)))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTYOURS", (
							target));
	    return;
	}
	
	if (Parent->nickserv.GetStored(target).Unlink())
	{
	    Parent->nickserv.stats.i_Unlink++;
	    SEND(mynick, source, "NS_OTH_COMMAND/UNLINKED", (
							target));
	    LOG(LM_INFO, "NICKSERV/UNLINK", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		source, target));
	}
	else
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTLINKED", (
							target));
    }
    else
    {
	if (Parent->nickserv.GetStored(source).Host().empty())
	{
	    NSEND(mynick, source, "NS_YOU_STATUS/ISHOST");
	    return;
	}

	if (Parent->nickserv.GetStored(source).Unlink())
	{
	    Parent->nickserv.stats.i_Unlink++;
	    mstring target = Parent->nickserv.GetStored(source).Host();
	    NSEND(mynick, source, "NS_YOU_COMMAND/UNLINKED");
	    LOG(LM_INFO, "NICKSERV/UNLINK", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		source, target));
	}
	else
	    NSEND(mynick, source, "NS_YOU_STATUS/ISNOTLINKED");
    }
}

void NickServ::do_Host(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Host", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    mstring newhost;
    if (params.WordCount(" ") > 1)
	newhost = params.ExtractWord(2, " ");

    if (!Parent->nickserv.GetLive(source).IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
						message, mynick));
	return;
    }

    if (!newhost.empty())
    {
	if (!Parent->nickserv.IsStored(newhost))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						newhost));
	    return;
	}

	newhost = Parent->getSname(newhost);
	if (Parent->nickserv.GetStored(newhost).Host().empty())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISHOST", (
						newhost));
	    return;
	}

	if (!(source.IsSameAs(newhost, true) ||
		Parent->nickserv.GetStored(newhost).IsSibling(source) ||
		Parent->nickserv.GetStored(newhost).Host().IsSameAs(source, true)))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTLINKED", (
						newhost));
	    return;
	}

	mstring oldhost = Parent->nickserv.GetStored(newhost).Host();
	Parent->nickserv.stats.i_Host++;
	Parent->nickserv.GetStored(newhost).MakeHost();
	SEND(mynick, source, "NS_OTH_COMMAND/NEWHOST", (
						newhost));
	LOG(LM_INFO, "NICKSERV/HOST", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		oldhost, newhost));
    }
    else
    {
	mstring oldhost = Parent->nickserv.GetStored(source).Host();
	Parent->nickserv.stats.i_Host++;
	Parent->nickserv.GetStored(source).MakeHost();
	NSEND(mynick, source, "NS_YOU_COMMAND/NEWHOST");
	LOG(LM_INFO, "NICKSERV/HOST", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		oldhost, source));
    }
}

void NickServ::do_Slaves(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Slaves", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    mstring target;
    if (params.WordCount(" ") < 2)
    {
	target = Parent->getSname(source);
    }
    else
    {
	if (Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
		Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source))
	{
	    target = params.ExtractWord(2, " ");
	    if (!Parent->nickserv.IsStored(target))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
		return;
	    }

	    target = Parent->getSname(target);
	    if (Parent->nickserv.GetStored(target).Forbidden())
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
					target));
		return;
	    }
	}
	else
	    target = Parent->getSname(source);
    }

    mstring output;
    if (!Parent->nickserv.GetStored(target).Host().empty())
	target = Parent->nickserv.GetStored(target).Host();

    output << IRC_Bold << target << IRC_Off << " (" <<
	Parent->nickserv.GetStored(target).Siblings() << "):";

    for (unsigned int i=0; i<Parent->nickserv.GetStored(target).Siblings(); i++)
    {
	if (Parent->nickserv.GetStored(target).Sibling(i).length() +
		output.length() > 510)
	{
	    ::send(mynick, source, output);
	    output.erase();
	    output << IRC_Bold << target << IRC_Off << " (" <<
		Parent->nickserv.GetStored(target).Siblings() << "):";
	}
	output << " " << Parent->nickserv.GetStored(target).Sibling(i);
    }
    ::send(mynick, source, output);
}


void NickServ::do_Identify(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Identify", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    if (!Parent->nickserv.IsStored(source))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						source));
	return;
    }

    if (Parent->nickserv.GetStored(source).Suspended())
    {
	NSEND(mynick, source, "NS_YOU_STATUS/ISSUSPENDED");
	return;
    }

    bool wasident = Parent->nickserv.GetLive(source).IsIdentified();
    mstring password = params.ExtractWord(2, " ");
    mstring output = Parent->nickserv.GetLive(source).Identify(password);
    if (!wasident &&
	Parent->nickserv.GetLive(source).IsIdentified())
    {
	Parent->nickserv.stats.i_Identify++;
	LOG(LM_INFO, "NICKSERV/IDENTIFY", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		source));
    }
    if (!output.empty())
	::send(mynick, source, output);
}

void NickServ::do_Info(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Info", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

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

    unsigned int i;
    mstring target   = params.ExtractWord(2, " ");
    mstring output;
    if (!Parent->nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
	return;
    }

    RLOCK(("NickServ", "stored", target.LowerCase()));
    Nick_Stored_t &nick = Parent->nickserv.GetStored(target);

    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nick.Name()));
	return;
    }

	SEND(mynick, source, "NS_INFO/REALNAME", (
			nick.Name(), nick.LastRealName()));
    if (!nick.Host().empty())
	SEND(mynick, source, "NS_INFO/HOST", (
		Parent->nickserv.GetStored(nick.Host()).Name()));
    output.erase();
    if (nick.NoExpire() && Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source))
	output << " (" << Parent->getMessage(source, "NS_INFO/NOEXPIRE") << ")";
	SEND(mynick, source, "NS_INFO/REGISTERED", (
		nick.RegTime().Ago(), output));

    if (!nick.IsOnline())
    {
	output.erase();
	bool isonline = false;
	if (!nick.Host().empty() &&
	    Parent->nickserv.GetStored(nick.Host()).IsOnline())
	    output = Parent->nickserv.GetLive(nick.Host()).Name() + " ";
	for (i=0; i<nick.Siblings(); i++)
	{
	    if (Parent->nickserv.IsStored(nick.Sibling(i)) &&
		Parent->nickserv.GetStored(nick.Sibling(i)).IsOnline())
	    {
		if (output.length() + nick.Sibling(i).length() > Parent->server.proto.MaxLine())
		{
		    SEND(mynick, source, "NS_INFO/ONLINEAS", (
						output));
		    output.erase();
		    isonline = true;
		}
		output += Parent->nickserv.GetLive(nick.Sibling(i)).Name() + " ";
	    }
	}
	if (!output.empty())
	{
	    SEND(mynick, source, "NS_INFO/ONLINEAS", (
						output));
	    isonline = true;
	}
	if (!isonline)
	{
	    if (!nick.Private() || (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source)))
		SEND(mynick, source, "NS_INFO/LASTALLMASK", (
						nick.LastAllMask()));
	    SEND(mynick, source, "NS_INFO/LASTALLSEEN", (
					nick.LastAllSeenTime().Ago()));
	    if (nick.LastAllMask().UpperCase() !=
		mstring(nick.Name() + "!" + nick.LastMask()).UpperCase() &&
		(!nick.Private() || (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source))))
		SEND(mynick, source, "NS_INFO/LASTMASK", (
					 nick.LastMask()));
	    if (nick.LastAllSeenTime() != nick.LastSeenTime())
		SEND(mynick, source, "NS_INFO/LASTSEEN", (
					nick.LastSeenTime().Ago()));
	    SEND(mynick, source, "NS_INFO/QUITMSG", (
					nick.LastQuit()));
	}
    }
    if (nick.Suspended())
    {
	SEND(mynick, source, "NS_INFO/SUSPEND", (
					nick.Suspend_Time().Ago(),
					nick.Suspend_By()));
	SEND(mynick, source, "NS_INFO/SUSPENDFOR", (
					nick.Comment()));
    }
    else
    {
	if (!nick.Email().empty())
	    SEND(mynick, source, "NS_INFO/EMAIL", (
					nick.Email()));
	if (!nick.URL().empty())
	    SEND(mynick, source, "NS_INFO/URL", (
					nick.URL()));
	if (!nick.ICQ().empty())
	    SEND(mynick, source, "NS_INFO/ICQ", (
					nick.ICQ()));
	if (!nick.Description().empty())
	    SEND(mynick, source, "NS_INFO/DESCRIPTION", (
					nick.Description()));
	if (!nick.Comment().empty() && Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	    Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source))
	    SEND(mynick, source, "NS_INFO/COMMENT", (
					nick.Comment()));
    }

    output.erase();
    CommServ::list_t::iterator iter;
    for (iter=Parent->commserv.ListBegin();
		iter!=Parent->commserv.ListEnd(); iter++)
    {
	// IF committee is not ALL or REGD
	// AND if it has a headcom, we're not in it
	// AND the committee isnt private or the requesting user is in OPER
	if (iter->first != Parent->commserv.ALL_Name() &&
	    iter->first != Parent->commserv.REGD_Name() &&
	    (iter->second.HeadCom().empty() ||
	    !(Parent->commserv.IsList(iter->second.HeadCom()) &&
	      Parent->commserv.GetList(iter->second.HeadCom().UpperCase()).IsIn(nick.Name()))) &&
	    iter->second.IsIn(target) && (!iter->second.Private() ||
	    (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	     Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source))))
	{
	    if (output.length() + iter->second.Name().length() > Parent->server.proto.MaxLine())
	    {
		SEND(mynick, source, "NS_INFO/COMMITTEES", (
						output));
		output.erase();
	    }
	    if (!output.empty())
		output << ", ";
	    if (iter->second.IsHead(target) && !iter->second.Head().empty())
		output << IRC_Bold;
	    output << iter->second.Name();
	    if (iter->second.IsHead(target) && !iter->second.Head().empty())
		output << IRC_Off;
	}
    }
    if (!output.empty())
	SEND(mynick, source, "NS_INFO/COMMITTEES", (
					output));

    output.erase();
    bool firstoption = true;
    if (nick.Protect())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick.L_Protect())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "NS_SET/PROTECT");
	if (nick.L_Protect())
	    output << IRC_Off;
    }

    if (nick.Secure())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick.L_Secure())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "NS_SET/SECURE");
	if (nick.L_Secure())
	    output << IRC_Off;
    }

    if (nick.NoMemo())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick.L_NoMemo())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "NS_SET/NOMEMO");
	if (nick.L_NoMemo())
	    output << IRC_Off;
    }

    if (nick.Private())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick.L_Private())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "NS_SET/PRIVATE");
	if (nick.L_Private())
	    output << IRC_Off;
    }

    if (!output.empty())
	SEND(mynick, source, "NS_INFO/OPTIONS", (
						output));
    if (nick.PicNum() &&
	Parent->filesys.Exists(FileMap::Picture, nick.PicNum()))
	SEND(mynick, source, "NS_INFO/HASPIC", (
				ToHumanSpace(Parent->filesys.GetSize(FileMap::Picture, nick.PicNum())),
				mynick, nick.Name()));
    if (nick.IsOnline())
	SEND(mynick, source,  "NS_INFO/ISONLINE", (
		Parent->getLname(nick.Name())));
    { RLOCK(("Events"));
    if (Parent->servmsg.ShowSync() && Parent->events != NULL)
	SEND(mynick, source, "MISC/SYNC", (
			Parent->events->SyncTime(source)));
    }
}

void NickServ::do_Ghost(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Ghost", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nick = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (nick.IsSameAs(source, true))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (
						message));
	return;
    }

    if (!Parent->nickserv.IsStored(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						nick));
	return;
    }

    if (!Parent->nickserv.IsLive(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (
						nick));
	return;
    }

    nick = Parent->getLname(nick);
    if (Parent->nickserv.GetStored(nick).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nick));
	return;
    }

    if (pass != Parent->nickserv.GetStored(nick).Password())
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
	return;
    }

    Parent->server.ANONKILL(mynick, nick, source + " (" +
				Parent->getMessage(nick, "MISC/KILL_GHOST") + ")");
    if (Parent->nickserv.IsRecovered(nick))
    {
	Parent->nickserv.RemRecovered(nick);
    }
    Parent->nickserv.stats.i_Ghost++;
    SEND(mynick, source, "NS_OTH_COMMAND/RELEASED", (
				nick));
    LOG(LM_DEBUG, "NICKSERV/GHOST", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	nick));
}

void NickServ::do_Recover(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Recover", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nick = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (nick.IsSameAs(source, true))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (
						message));
	return;
    }

    if (!Parent->nickserv.IsStored(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nick));
	return;
    }

    nick = Parent->getSname(nick);
    if (Parent->nickserv.GetStored(nick).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nick));
	return;
    }

    if (pass != Parent->nickserv.GetStored(nick).Password())
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
	return;
    }

    if (Parent->nickserv.IsLive(nick))
    {
	Parent->server.KILL(mynick, Parent->getLname(nick), source + " (" +
			Parent->getMessage(nick, "MISC/KILL_RECOVER") + ")");
    }

    Parent->server.NICK(nick, (Parent->startup.Ownuser() ? nick.LowerCase() :
				Parent->startup.Services_User()),
				Parent->startup.Services_Host(),
				Parent->startup.Server_Name(),
				Parent->nickserv.Enforcer_Name());

    Parent->nickserv.AddRecovered(nick, mDateTime::CurrentDateTime());
    Parent->nickserv.stats.i_Recover++;
    SEND(mynick, source, "NS_OTH_COMMAND/HELD", (
				nick));
    LOG(LM_DEBUG, "NICKSERV/RECOVER", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	nick));
}

void NickServ::do_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_List", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();

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
	mask = "*";
	listsize = Parent->config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Parent->config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Parent->config.Maxlist())
	{
	    mstring output;
	    SEND(mynick, source, "LIST/MAXLIST", (
					Parent->config.Maxlist()));
	    return;
	}
    }

    SEND(mynick, source, "LIST/NICK_LIST", (
					mask));
    NickServ::stored_t::iterator iter;


    bool isoper = (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source));
    bool issop = (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
		Parent->commserv.GetList(Parent->commserv.SOP_Name()).IsOn(source));

    RLOCK(("NickServ", "stored"));
    for (iter = Parent->nickserv.StoredBegin(), i=0, count = 0;
			iter != Parent->nickserv.StoredEnd(); iter++)
    {
	if (iter->second.Name().Matches(mask, true))
	{
	    if (!iter->second.Host().empty())
		continue;

	    if (i < listsize && (!iter->second.Private() || isoper))
	    {
		if (issop)
		{
		    if (message.Contains("NOEXP") && !iter->second.NoExpire())
			continue;
		    if (message.Contains("FORBID") && !iter->second.Forbidden())
			continue;
		    if (message.Contains("SUSP") && !iter->second.Suspended())
			continue;
		}

		if (iter->second.Forbidden())
		{
		    ::send(mynick, source, iter->second.Name() + "  (" +
			Parent->getMessage(source, "VALS/FORBIDDEN") + ")");
		}
		else
		{
		    ::send(mynick, source, iter->second.Name() + "  (" +
					iter->second.LastAllMask() + ")");
		}
		i++;
	    }
	    count++;
	}
    }
    SEND(mynick, source, "LIST/DISPLAYED", (
							i, count));
}

void NickServ::do_Send(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Send", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

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

    mstring target   = params.ExtractWord(2, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
							target));
	return;
    }

    target = Parent->getSname(target);
    if (Parent->nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    unsigned long picnum = Parent->nickserv.GetStored(target).PicNum();
    if (!picnum)
    {
	SEND(mynick, source, "NS_OTH_STATUS/NOPIC", (
							target));
	return;
    }

    if (!Parent->filesys.Exists(FileMap::Picture, picnum))
    {
	Parent->nickserv.GetStored(target).GotPic(0);
	SEND(mynick, source, "DCC/NOFILE", (
							"SEND"));
	return;
    }

    mstring filename = 	Parent->filesys.GetName(FileMap::Picture, picnum);
    size_t filesize = Parent->filesys.GetSize(FileMap::Picture, picnum);
    if (filename.empty() || filesize <= 0)
    {
	Parent->nickserv.GetStored(target).GotPic(0);
	SEND(mynick, source, "DCC/NOFILE", (
							"SEND"));
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
	Parent->nickserv.stats.i_Send++;
	unsigned short port = mSocket::FindAvailPort();
	::privmsg(mynick, source, DccEngine::encode("DCC SEND", filename +
		" " + mstring(Parent->LocalHost()) + " " +
		mstring(port) + " " + mstring(filesize)));
	Parent->dcc->Accept(port, mynick, source, FileMap::Picture, picnum);
    }}
}

void NickServ::do_Suspend(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Suspend", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring target   = params.ExtractWord(2, " ");
    mstring reason   = params.After(" ", 2);

    if (!Parent->nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
							target));
	return;
    }

    target = Parent->getSname(target);
    if (Parent->nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    if (Parent->nickserv.GetStored(target).Suspended())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISSUSPENDED", (
							target));
	return;
    }

    if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsIn(target))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONCOMMITTEE", (
				message, Parent->commserv.OPER_Name()));
	return;
    }

    Parent->nickserv.GetStored(target).Suspend(source, reason);
    Parent->nickserv.stats.i_Suspend++;
    SEND(mynick, source, "NS_OTH_COMMAND/SUSPENDED", (
						target));
    LOG(LM_NOTICE, "NICKSERV/SUSPEND", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, reason));
}

void NickServ::do_UnSuspend(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_UnSuspend", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
							target));
	return;
    }

    target = Parent->getSname(target);
    if (Parent->nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    if (!Parent->nickserv.GetStored(target).Suspended())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSUSPENDED", (
							target));
	return;
    }

    Parent->nickserv.GetStored(target).UnSuspend();
    Parent->nickserv.stats.i_Unsuspend++;
    SEND(mynick, source, "NS_OTH_COMMAND/UNSUSPENDED", (
						target));
    LOG(LM_NOTICE, "NICKSERV/UNSUSPEND", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target));
}

void NickServ::do_Forbid(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Forbid", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring target   = params.ExtractWord(2, " ");
    if (Parent->nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISSTORED", (
					Parent->getSname(target)));
	return;
    }

    Nick_Stored_t tmp(target);
    Parent->nickserv.AddStored(&tmp);
    Parent->nickserv.stats.i_Forbid++;
    SEND(mynick, source, "NS_OTH_COMMAND/FORBIDDEN", (
					target));
    LOG(LM_NOTICE, "NICKSERV/FORBID", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target));
}


void NickServ::do_Getpass(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Getpass", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring target   = params.ExtractWord(2, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
	return;
    }

    target = Parent->getSname(target);
    if (Parent->nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    // If we are NOT a SADMIN, and target is a PRIV GROUP.
    if (!(Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	Parent->commserv.GetList(Parent->commserv.SADMIN_Name()).IsIn(source)) &&
	(Parent->commserv.GetList(Parent->commserv.SADMIN_Name()).IsIn(target) ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.GetList(Parent->commserv.SOP_Name()).IsIn(target)) ||
	(Parent->commserv.IsList(Parent->commserv.ADMIN_Name()) &&
	Parent->commserv.GetList(Parent->commserv.ADMIN_Name()).IsIn(target)) ||
	(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsIn(target))))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (
						message));
	return;
    }

    RLOCK(("NickServ", "stored", target.LowerCase()));
    Nick_Stored_t &nick = Parent->nickserv.GetStored(target);
    mstring host = nick.Host();
    if (host.empty())
	host = nick.Name();

    Parent->nickserv.stats.i_Getpass++;
    SEND(mynick, source, "NS_OTH_COMMAND/GETPASS", (
			nick.Name(), host, nick.Password()));
    ANNOUNCE(mynick, "MISC/NICK_GETPASS", (
			source, nick.Name(), host));
    LOG(LM_NOTICE, "NICKSERV/GETPASS", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	nick.Name(), host));
}


void NickServ::do_Live(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Live", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();

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
	mask = "*!*@*";
	listsize = Parent->config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Parent->config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Parent->config.Maxlist())
	{
	    mstring output;
	    SEND(mynick, source, "LIST/MAXLIST", (
					Parent->config.Maxlist()));
	    return;
	}
    }

    if (!mask.Contains("@") && !mask.Contains("!"))
	mask += "!*@*";
    else if (!mask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
				Parent->getMessage(source, "LIST/NICKNAME"), '@'));
	return;
    }
    else if (!mask.Contains("!"))
	mask.prepend("*!");

    SEND(mynick, source, "LIST/NICK_LIST", (
					mask));
    NickServ::live_t::iterator iter;

    RLOCK(("NickServ", "live"));
    for (iter = Parent->nickserv.LiveBegin(), i=0, count = 0;
			iter != Parent->nickserv.LiveEnd(); iter++)
    {
	if (iter->second.Mask(Nick_Live_t::N_U_P_H).Matches(mask, true))
	{
	    if (i < listsize)
	    {
		if (message.Contains("OPER") && !iter->second.HasMode("o"))
		    continue;

		::send(mynick, source, iter->second.Mask(Nick_Live_t::N_U_P_H) + " (" +
					((!iter->second.Server().empty()) ?
						iter->second.Server() :
						Parent->startup.Server_Name()) +
					((!iter->second.Squit().empty()) ? " (SQUIT)" : "") +
					"): +" + iter->second.Mode());
		i++;
	    }
	    count++;
	}
    }
    SEND(mynick, source, "LIST/DISPLAYED", (
							i, count));
}


void NickServ::do_access_Current(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_access_Current", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    if (Parent->nickserv.GetStored(source).AccessAdd(
	    Parent->nickserv.GetLive(source).Mask(Nick_Live_t::U_H).After("!")))
    {
	Parent->nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/ADD", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::U_H).After("!"),
		Parent->getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_ADD", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::U_H).After("!"),
		source));

    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS", (
		mstring(Parent->nickserv.GetLive(source).Mask(Nick_Live_t::U_H).After("!")),
		Parent->getMessage(source, "LIST/ACCESS")));
    }
}

void NickServ::do_access_Add(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_access_Add", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring hostmask = params.ExtractWord(3, " ");

    if (hostmask.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (
				Parent->getMessage(source, "LIST/ACCESS"), '!'));
	return;
    }

    if (!hostmask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
				Parent->getMessage(source, "LIST/ACCESS"), '@'));
	return;
    }

    if (Parent->nickserv.GetStored(source).AccessAdd(hostmask))
    {
	Parent->nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/ADD", (
			hostmask, Parent->getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_ADD", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		hostmask, source));
    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS", (
			hostmask, Parent->getMessage(source, "LIST/ACCESS")));
    }
}

void NickServ::do_access_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_access_Del", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring hostmask = params.ExtractWord(3, " ");

    if (hostmask.IsNumber())
    {
	if (hostmask.Contains(".") || hostmask.Contains("-"))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(hostmask.c_str());
	if (num <= 0 || num > Parent->nickserv.GetStored(source).Access())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
			1, Parent->nickserv.GetStored(source).Access()));
	    return;
	}

	hostmask = Parent->nickserv.GetStored(source).Access(num-1);
    }

    if (hostmask.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (
				Parent->getMessage(source, "LIST/ACCESS"), '!'));
	return;
    }

    if (!hostmask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
				Parent->getMessage(source, "LIST/ACCESS"), '@'));
	return;
    }

    unsigned int count = Parent->nickserv.GetStored(source).AccessDel(hostmask);
    if (count)
    {
	Parent->nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/DEL_MATCH", (
			count, hostmask,
			Parent->getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		hostmask, source));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (
			hostmask, Parent->getMessage(source, "LIST/ACCESS")));
    }
}

void NickServ::do_access_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_access_List", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    mstring target = Parent->getSname(source);

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") >= 3 &&
	Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsStored(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
					target));
	    return;
	}

	target = Parent->getSname(target);
	if (Parent->nickserv.GetStored(target).Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	    return;
	}
    }

    if (Parent->nickserv.GetStored(target).Access())
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/DISPLAY2", (
			target, Parent->getMessage(source, "LIST/ACCESS")));
	else
	    SEND(mynick, source, "LIST/DISPLAY", (
			Parent->getMessage(source, "LIST/ACCESS")));
    }
    else
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/EMPTY2", (
			target, Parent->getMessage(source, "LIST/ACCESS")));
	else
	    SEND(mynick, source, "LIST/EMPTY", (
			Parent->getMessage(source, "LIST/ACCESS")));
	return;
    }

    unsigned int i;
    mstring retval;
    for (i=0; i<Parent->nickserv.GetStored(target).Access(); i++)
    {
	retval.erase();
	retval << i+1 << ". " << Parent->nickserv.GetStored(target).Access(i);
	::send(mynick, source, retval);
    }
}

void NickServ::do_ignore_Add(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_ignore_Add", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
	return;
    }

    target = Parent->getSname(target);
    if (Parent->nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    if (Parent->nickserv.GetStored(source).IgnoreAdd(target))
    {
	Parent->nickserv.stats.i_Ignore++;
	SEND(mynick, source, "LIST/ADD", (
			target, Parent->getMessage(source, "LIST/IGNORE")));
	LOG(LM_DEBUG, "NICKSERV/IGNORE_ADD", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		target, source));
    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS", (
			target, Parent->getMessage(source, "LIST/IGNORE")));
    }
}

void NickServ::do_ignore_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_ignore_Del", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(3, " ");

    if (target.IsNumber())
    {
	if (target.Contains(".") || target.Contains("-"))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(target.c_str());
	if (num <= 0 || num > Parent->nickserv.GetStored(source).Ignore())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
			1, Parent->nickserv.GetStored(source).Ignore()));
	    return;
	}

	target = Parent->nickserv.GetStored(source).Ignore(num-1);
    }

    unsigned int count = Parent->nickserv.GetStored(source).IgnoreDel(target);
    if (count)
    {
	Parent->nickserv.stats.i_Ignore++;
	SEND(mynick, source, "LIST/DEL_MATCH", (
			count, target,
			Parent->getMessage(source, "LIST/IGNORE")));
	LOG(LM_DEBUG, "NICKSERV/IGNORE_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		target, source));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (
			target, Parent->getMessage(source, "LIST/IGNORE")));
    }

}

void NickServ::do_ignore_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_ignore_List", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    mstring target = Parent->getSname(source);

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") >= 3 &&
	Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsStored(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				target));
	    return;
	}

	target = Parent->getSname(target);
	if (Parent->nickserv.GetStored(target).Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	    return;
	}
    }

    if (Parent->nickserv.GetStored(target).Ignore())
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/DISPLAY2", (
			target, Parent->getMessage(source, "LIST/IGNORE")));
	else
	    SEND(mynick, source, "LIST/DISPLAY", (
			Parent->getMessage(source, "LIST/IGNORE")));
    }
    else
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/EMPTY2", (
			target, Parent->getMessage(source, "LIST/IGNORE")));
	else
	    SEND(mynick, source, "LIST/EMPTY", (
			Parent->getMessage(source, "LIST/IGNORE")));
	return;
    }

    unsigned int i;
    mstring retval;
    for (i=0; i<Parent->nickserv.GetStored(target).Ignore(); i++)
    {
	retval.erase();
	retval << i+1 << ". " << Parent->nickserv.GetStored(target).Ignore(i);
	::send(mynick, source, retval);
    }
}

void NickServ::do_set_Password(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_Password", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring newpass = params.ExtractWord(3, " ");

    if (!Parent->nickserv.GetLive(source).IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
			message, mynick));
	return;
    }

    mstring oldpass = Parent->nickserv.GetStored(source).Password();
    if (newpass.IsSameAs(oldpass, true) || newpass.IsSameAs(source, true) ||
	newpass.length() < 5)
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
	return;
    }

    Parent->nickserv.GetStored(source).Password(newpass);
    Parent->nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Parent->getMessage(source, "NS_SET/PASSWORD"), newpass));
    LOG(LM_INFO, "NICKSERV/SET_PASSWORD", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		source));
}

void NickServ::do_set_Email(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_Email", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();
    else if (!newvalue.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
		Parent->getMessage(source, "NS_SET/EMAIL"), '@'));
	return;
    }
    else if (newvalue.WordCount("@") != 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE", (
		Parent->getMessage(source, "NS_SET/EMAIL"), '@'));
	return;
    }

    Parent->nickserv.GetStored(source).Email(newvalue);
    Parent->nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Parent->getMessage(source, "NS_SET/EMAIL"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("NS_SET/EMAIL"),
		source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (
		Parent->getMessage(source, "NS_SET/EMAIL")));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("NS_SET/EMAIL"),
		source));
    }
}

void NickServ::do_set_URL(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_URL", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();

    if (newvalue.SubString(0, 6).IsSameAs("http://", true))
    {
	newvalue.erase(0, 6);
    }

    Parent->nickserv.GetStored(source).URL(newvalue);
    Parent->nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Parent->getMessage(source, "NS_SET/URL"),
		"http://" + newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("NS_SET/URL"),
		source, "http://" + newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (
		Parent->getMessage(source, "NS_SET/URL")));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("NS_SET/URL"),
		source));
    }
}

void NickServ::do_set_ICQ(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_ICQ", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();
    else if (!newvalue.IsNumber() || newvalue.Contains("."))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }
    else if (newvalue.length() < 6 || newvalue.length() > 9)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
				100000, 999999999));
	return;
    }


    Parent->nickserv.GetStored(source).ICQ(newvalue);
    Parent->nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Parent->getMessage(source, "NS_SET/ICQ"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("NS_SET/ICQ"),
		source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (
		Parent->getMessage(source, "NS_SET/ICQ")));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("NS_SET/ICQ"),
		source));
    }
}

void NickServ::do_set_Description(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_Description", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring newvalue = params.After(" ", 2);

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();

    Parent->nickserv.GetStored(source).Description(newvalue);
    Parent->nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Parent->getMessage(source, "NS_SET/DESCRIPTION"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("NS_SET/DESCRIPTION"),
		source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (
		Parent->getMessage(source, "NS_SET/DESCRIPTION")));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("NS_SET/DESCRIPTION"),
		source));
    }
}

void NickServ::do_set_Comment(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_Comment", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(3, " ");
    mstring comment = params.After(" ", 3);

    if (!Parent->nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
		target));
	return;
    }

    target = Parent->getSname(target);
    if (Parent->nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    if (comment.IsSameAs("none", true))
	comment.erase();

    Parent->nickserv.GetStored(target).Comment(comment);
    Parent->nickserv.stats.i_Set++;
    if (!comment.empty())
    {
	SEND(mynick, source, "NS_OTH_COMMAND/SET_TO", (
		Parent->getMessage(source, "NS_SET/COMMENT"),
		target, comment));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("NS_SET/COMMENT"),
		target, comment));
    }
    else
    {
	SEND(mynick, source, "NS_OTH_COMMAND/UNSET", (
		Parent->getMessage(source, "NS_SET/COMMENT"),
		target));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("NS_SET/COMMENT"),
		target));
    }
}

void NickServ::do_set_Picture(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_Picture", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    if (Parent->nickserv.PicExt().empty())
    {
	NSEND(mynick, source, "NS_YOU_STATUS/PICDISABLED");
	return;
    }

    Parent->nickserv.stats.i_SetPicture++;
    if (params.WordCount(" ") > 2 &&
	params.ExtractWord(3, " ").IsSameAs("none", true))
    {
	if (Parent->nickserv.GetStored(source).PicNum())
	    Parent->filesys.EraseFile(FileMap::Picture,
		Parent->nickserv.GetStored(source).PicNum());
	Parent->nickserv.GetStored(source).GotPic(0u);
	NSEND(mynick, source, "NS_YOU_COMMAND/REMOVED");
	LOG(LM_DEBUG, "NICKSERV/PICTURE_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		source));
    }
    else
    {
	if (Parent->nickserv.GetStored(source).PicNum())
	    Parent->filesys.EraseFile(FileMap::Picture,
		Parent->nickserv.GetStored(source).PicNum());
	Parent->nickserv.GetStored(source).GotPic(0u);
	Parent->nickserv.GetLive(source).InFlight.Picture(mynick);
    }
}

void NickServ::do_set_Protect(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_Protect", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.GetStored(source).L_Protect())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/PROTECT")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(source).Protect(onoff.GetBool());
    Parent->nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO", (
			Parent->getMessage(source, "NS_SET/PROTECT"),
			onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/PROTECT"),
	source, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}

void NickServ::do_set_Secure(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_Secure", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.GetStored(source).L_Secure())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/SECURE")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(source).Secure(onoff.GetBool());
    Parent->nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO", (
			Parent->getMessage(source, "NS_SET/SECURE"),
			onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/SECURE"),
	source, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}

void NickServ::do_set_NoExpire(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_NoExpire", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
		nickname));
	return;
    }

    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_NoExpire())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/NOEXPIRE"),
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_NoExpire())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(nickname).NoExpire(onoff.GetBool());
    Parent->nickserv.stats.i_NoExpire++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_SET_TO", (
			Parent->getMessage(source, "NS_SET/NOEXPIRE"),
			nickname, onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_NOTICE, "NICKSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/NOEXPIRE"),
	nickname, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}


void NickServ::do_set_NoMemo(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_NoMemo", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.GetStored(source).L_NoMemo())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/NOMEMO")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(source).NoMemo(onoff.GetBool());
    Parent->nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO", (
			Parent->getMessage(source, "NS_SET/NOMEMO"),
			onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/NOMEMO"),
	source, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}

void NickServ::do_set_Private(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_Private", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.GetStored(source).L_Private())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/PRIVATE")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(source).Private(onoff.GetBool());
    Parent->nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO", (
			Parent->getMessage(source, "NS_SET/PRIVATE"),
			onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/PRIVATE"),
	source, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}

void NickServ::do_set_PRIVMSG(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_PRIVMSG", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.GetStored(source).L_PRIVMSG())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/PRIVMSG")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(source).PRIVMSG(onoff.GetBool());
    Parent->nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO", (
			Parent->getMessage(source, "NS_SET/PRIVMSG"),
			onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/PRIVMSG"),
	source, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}

void NickServ::do_set_Language(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_Language", (mynick, source, params));

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

    mstring lang = params.ExtractWord(3, " ").UpperCase();

    if (Parent->nickserv.GetStored(source).L_Language())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/LANGUAGE")));
	return;
    }

    if (lang.IsSameAs("default", true) || lang.IsSameAs("reset", true))
    {
	lang = Parent->nickserv.DEF_Language().UpperCase();
    }
    else
    {
	// check for valid language ...
	if (!mFile::Exists(Parent->files.Langdir()+DirSlash+lang.LowerCase()+".lng"))
	{
	    SEND(mynick, source, "OS_STATUS/NOLANG", (
			lang));
	    return;
	}
    }

    Parent->nickserv.GetStored(source).Language(lang);
    Parent->nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
			Parent->getMessage(source, "NS_SET/LANGUAGE"),
			mstring(lang + " (" +
			Parent->getMessage(source, "ERR_SYNTAX/TRANSLATED") + ")")));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/LANGUAGE"),
	source, lang));
}

void NickServ::do_lock_Protect(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_lock_Protect", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_Protect())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/PROTECT"),
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(nickname).L_Protect(false);
    Parent->nickserv.GetStored(nickname).Protect(onoff.GetBool());
    Parent->nickserv.GetStored(nickname).L_Protect(true);
    Parent->nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED", (
			Parent->getMessage(source, "NS_SET/PROTECT"),
			nickname, onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/PROTECT"),
	nickname, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}

void NickServ::do_lock_Secure(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_lock_Secure", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_Secure())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/SECURE"),
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(nickname).L_Secure(false);
    Parent->nickserv.GetStored(nickname).Secure(onoff.GetBool());
    Parent->nickserv.GetStored(nickname).L_Secure(true);
    Parent->nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED", (
			Parent->getMessage(source, "NS_SET/SECURE"),
			nickname, onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/SECURE"),
	nickname, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}

void NickServ::do_lock_NoMemo(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_lock_NoMemo", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_NoMemo())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/NOMEMO"),
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(nickname).L_NoMemo(false);
    Parent->nickserv.GetStored(nickname).NoMemo(onoff.GetBool());
    Parent->nickserv.GetStored(nickname).L_NoMemo(true);
    Parent->nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED", (
			Parent->getMessage(source, "NS_SET/NOMEMO"),
			nickname, onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/NOMEMO"),
	nickname, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}

void NickServ::do_lock_Private(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_lock_Private", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_Private())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/PRIVATE"),
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(nickname).L_Private(false);
    Parent->nickserv.GetStored(nickname).Private(onoff.GetBool());
    Parent->nickserv.GetStored(nickname).L_Private(true);
    Parent->nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED", (
			Parent->getMessage(source, "NS_SET/PRIVATE"),
			nickname, onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/PRIVATE"),
	nickname, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}

void NickServ::do_lock_PRIVMSG(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_lock_PRIVMSG", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_PRIVMSG())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/PRIVMSG"),
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Parent->nickserv.GetStored(nickname).L_PRIVMSG(false);
    Parent->nickserv.GetStored(nickname).PRIVMSG(onoff.GetBool());
    Parent->nickserv.GetStored(nickname).L_PRIVMSG(true);
    Parent->nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED", (
			Parent->getMessage(source, "NS_SET/PRIVMSG"),
			nickname, onoff.GetBool() ?
				Parent->getMessage(source, "VALS/ON") :
				Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/PRIVMSG"),
	nickname, onoff.GetBool() ?
		Parent->getMessage("VALS/ON") :
		Parent->getMessage("VALS/OFF")));
}

void NickServ::do_lock_Language(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_lock_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring lang     = params.ExtractWord(4, " ").UpperCase();

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_Language())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/LANGUAGE"),
				nickname));
	return;
    }

    if (lang.IsSameAs("default", true) || lang.IsSameAs("reset", true))
    {
	lang = Parent->nickserv.DEF_Language().UpperCase();
    }
    else if (!mFile::Exists(Parent->files.Langdir()+DirSlash+lang.LowerCase()+".lng"))
    {
	SEND(mynick, source, "OS_STATUS/NOLANG", (
			lang));
	return;
    }

    Parent->nickserv.GetStored(nickname).L_Language(false);
    Parent->nickserv.GetStored(nickname).Language(lang);
    Parent->nickserv.GetStored(nickname).L_Language(true);
    Parent->nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/LOCKED", (
			Parent->getMessage(source, "NS_SET/LANGUAGE"),
			nickname, mstring(lang + " (" +
			Parent->getMessageL(lang, "ERR_SYNTAX/TRANSLATED") + ")")));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/LANGUAGE"),
	nickname, lang));
}

void NickServ::do_unlock_Protect(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_unlock_Protect", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_Protect())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/PROTECT"),
				nickname));
	return;
    }

    Parent->nickserv.GetStored(nickname).L_Protect(false);
    Parent->nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (
			Parent->getMessage(source, "NS_SET/PROTECT"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/PROTECT"),
	nickname));
}

void NickServ::do_unlock_Secure(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_unlock_Secure", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_Secure())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/SECURE"),
				nickname));
	return;
    }

    Parent->nickserv.GetStored(nickname).L_Secure(false);
    Parent->nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (
			Parent->getMessage(source, "NS_SET/SECURE"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/SECURE"),
	nickname));
}

void NickServ::do_unlock_NoMemo(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_unlock_NoMemo", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_NoMemo())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/NOMEMO"),
				nickname));
	return;
    }

    Parent->nickserv.GetStored(nickname).L_NoMemo(false);
    Parent->nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (
			Parent->getMessage(source, "NS_SET/NOMEMO"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/NOMEMO"),
	nickname));
}

void NickServ::do_unlock_Private(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_unlock_Private", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_Private())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/PRIVATE"),
				nickname));
	return;
    }

    Parent->nickserv.GetStored(nickname).L_Private(false);
    Parent->nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (
			Parent->getMessage(source, "NS_SET/PRIVATE"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/PRIVATE"),
	nickname));
}

void NickServ::do_unlock_PRIVMSG(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_unlock_PRIVMSG", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_PRIVMSG())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/PRIVMSG"),
				nickname));
	return;
    }

    Parent->nickserv.GetStored(nickname).L_PRIVMSG(false);
    Parent->nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (
			Parent->getMessage(source, "NS_SET/PRIVMSG"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/PRIVMSG"),
	nickname));
}

void NickServ::do_unlock_Language(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_unlock_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Parent->nickserv.LCK_Language())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Parent->getMessage(source, "NS_SET/LANGUAGE"),
				nickname));
	return;
    }

    Parent->nickserv.GetStored(nickname).L_Language(false);
    Parent->nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/UNLOCKED", (
			Parent->getMessage(source, "NS_SET/LANGUAGE"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("NS_SET/LANGUAGE"),
	nickname));
}

SXP::Tag NickServ::tag_NickServ("NickServ");

void NickServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("NickServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Nick_Stored_t *ns = new Nick_Stored_t;

    if( pElement->IsA( ns->GetClassTag() ) )
    {
	ns_array.push_back(ns);
	pIn->ReadTo(ns);
    }
    else
    {
	delete ns;
    }
}

void NickServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("NickServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void NickServ::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("NickServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_NickServ, attribs);

    NickServ::stored_t::iterator iter;
    { RLOCK(("NickServ", "stored"));
    for (iter = StoredBegin(); iter != StoredEnd(); iter++)
	pOut->WriteSubElement(&iter->second, attribs);
    }

    pOut->EndObject(tag_NickServ);
}

void NickServ::PostLoad()
{
    NFT("NickServ::PostLoad");
    unsigned int i, j;
    for (i=0; i<ns_array.size(); i++)
    {
	if (ns_array[i] != NULL)
	{
	    for (j=0; j<ns_array[i]->ud_array.size(); j++)
	    {
		if (ns_array[i]->ud_array[j] != NULL)
		{
		    if (ns_array[i]->ud_array[j]->Contains("\n"))
			ns_array[i]->i_UserDef[ns_array[i]->ud_array[j]->Before("\n")] =
				ns_array[i]->ud_array[j]->After("\n");
		    delete ns_array[i]->ud_array[j];
		}
	    }
	    ns_array[i]->ud_array.clear();
	    if (!ns_array[i]->Name().empty())
		AddStored(ns_array[i]);
	    delete ns_array[i];
	}
    }
    ns_array.clear();

    NickServ::stored_t::iterator iter;
    CP(("Linking nickname entries ..."));
    WLOCK(("NickServ", "stored"));
    for (iter=StoredBegin(); iter!=StoredEnd(); iter++)
    {
	if (!iter->second.i_Host.empty())
	{
	    if (IsStored(iter->second.i_Host))
	    {
		COM(("Nickname %s has been linked to %s ...",
		    iter->first.c_str(), iter->second.i_Host.c_str()));
		GetStored(iter->second.i_Host).i_slaves.insert(iter->first);
	    }
	    else
	    {
		LOG(LM_WARNING, "ERROR/HOST_NOTREGD", (
		    iter->second.i_Host, iter->first));
		iter->second.i_Host.erase();
	    }
	}
    }
}
