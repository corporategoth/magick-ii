
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
#define RCSID(x,y) const char *rcsid_chanserv_cpp_ ## x () { return y; }
RCSID(chanserv_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.256  2001/07/05 05:59:08  prez
** More enhansements to try and avoid Signal #6's, coredumps, and deadlocks.
**
** Revision 1.255  2001/07/03 06:00:07  prez
** More deadlock fixes ... also cleared up the Signal #6 problem.
**
** Revision 1.254  2001/07/02 03:39:29  prez
** Fixed bug with users sending printf strings (mainly in memos).
**
** Revision 1.253  2001/07/01 05:02:45  prez
** Added changes to dependancy system so it wouldnt just remove a dependancy
** after the first one was satisfied.
**
** Revision 1.252  2001/06/17 09:39:06  prez
** Hopefully some more changes that ensure uptime (mainly to do with locking
** entries in an iterated search, and using copies of data instead of references
** where we can get away with it -- reducing the need to lock the data).
**
** Revision 1.251  2001/06/16 09:35:24  prez
** More tiny bugs ...
**
** Revision 1.250  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.249  2001/06/11 03:44:45  prez
** Re-wrote how burst works, and made the burst message a lower priority
** than normal.  Also removed the chance of a stray pointer being picked
** up in the dependancy system.
**
** Revision 1.248  2001/05/28 11:17:33  prez
** Added some more anti-deadlock stuff, and fixed nick ident warnings
**
** Revision 1.247  2001/05/23 02:47:10  prez
** *** empty log message ***
**
** Revision 1.246  2001/05/23 02:43:47  prez
** Fixed the NOACCESS bug, the chanserv getpass/setpass bug and nickserv failed
** passwords kill bug.
**
** Revision 1.245  2001/05/17 19:18:53  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.244  2001/05/16 15:47:13  prez
** Added ability to have a CS level set to MAX_LEVEL+2 (for disabled).  Also
** made it display 'disabled' and 'founder' nicer than just MAX_LEVEL+1/2.
**
** Revision 1.243  2001/05/13 22:01:36  prez
** Should have fixed problems with linked nicknames
**
** Revision 1.242  2001/05/13 00:55:18  prez
** More patches to try and fix deadlocking ...
**
** Revision 1.241  2001/05/08 03:22:27  prez
** Removed one possible deadlock cause, and stopped events engine from doing
** anything until synch is over.
**
** Revision 1.240  2001/05/06 03:03:07  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.239  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.238  2001/05/04 01:11:13  prez
** Made chanserv mode stuff more efficiant
**
** Revision 1.237  2001/05/01 14:00:22  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.236  2001/04/13 00:46:38  prez
** Fixec channel registering
**
** Revision 1.235  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.234  2001/03/27 19:16:03  prez
** Fixed Chan_Stored_t::ChgNick (had problems coz nick isnt fully changed yet)
**
** Revision 1.233  2001/03/27 16:09:42  prez
** Fixed chanserv internal maps problem (inserted with incorrect case)
**
** Revision 1.232  2001/03/27 07:49:10  prez
** Fixed channels being added in case
**
** Revision 1.231  2001/03/27 07:04:31  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.230  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.229  2001/03/08 08:07:40  ungod
** fixes for bcc 5.5
**
** Revision 1.228  2001/03/04 02:04:14  prez
** Made mstring a little more succinct ... and added vector/list operations
**
** Revision 1.227  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.226  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.225  2001/02/03 02:21:32  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.224  2001/01/16 15:47:39  prez
** Fixed filesys not generating first entry in maps, fixed chanserv level
** changes (could confuse set) and fixed idle times on whois user user
**
** Revision 1.223  2001/01/16 12:47:36  prez
** Fixed mlock setting in live channels (also fixed helpop)
**
** Revision 1.222  2001/01/15 23:31:38  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.221  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.220  2000/12/31 17:54:29  prez
** Added checking to see if 'http://' was entered in the SET URL commands.
**
** Revision 1.219  2000/12/29 15:31:55  prez
** Added locking/checking for dcc/events threads.  Also for ACE_Log_Msg
**
** Revision 1.218  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.217  2000/12/22 19:50:19  prez
** Made all config options const.  Beginnings of securing all non-modifying
** commands to const.  also added serviceschk.
**
** Revision 1.216  2000/12/22 08:55:40  prez
** Made forbidden entries (chanserv or nickserv) show up as forbidden in
** a list (rather than (nick!) or whatever)
**
** Revision 1.215  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.214  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.213  2000/11/09 10:58:19  prez
** THINK I have it working again ... with the free list.
** Will check, still thinking of sorting free list by size.
**
** Revision 1.212  2000/10/10 11:47:50  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.211  2000/10/07 11:00:11  ungod
** no message
**
** Revision 1.210  2000/09/30 10:48:06  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.209  2000/09/27 11:21:38  prez
** Added a BURST mode ...
**
** Revision 1.208  2000/09/22 12:26:11  prez
** Fixed that pesky bug with chanserv not seeing modes *sigh*
**
** Revision 1.207  2000/09/13 12:45:33  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.206  2000/09/12 21:17:01  prez
** Added IsLiveAll (IsLive now checks to see if user is SQUIT).
**
** Revision 1.205  2000/09/11 10:58:19  prez
** Now saves in in GMT
**
** Revision 1.204  2000/09/10 09:53:43  prez
** Added functionality to ensure the order of messages is kept.
**
** Revision 1.203  2000/09/09 02:17:47  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.202  2000/09/05 10:53:06  prez
** Only have operserv.cpp and server.cpp to go with T_Changing / T_Modify
** tracing -- also modified keygen to allow for cmdline generation (ie.
** specify 1 option and enter keys, or 2 options and the key is read from
** a file).  This allows for paragraphs with \n's in them, and helps so you
** do not have to type out 1024 bytes :)
**
** Revision 1.201  2000/09/02 07:20:45  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.200  2000/08/28 10:51:35  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.199  2000/08/22 08:43:39  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.198  2000/08/19 14:45:02  prez
** Fixed mode settings upon commitee recognitition syntax checking
**
** Revision 1.197  2000/08/19 10:59:46  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.196  2000/08/10 22:44:22  prez
** Added 'binding to IP' options for shell servers, etc.  Also added akick
** triggers for when a user changes their nick and suddenly matches akick.
**
** Revision 1.195  2000/08/09 12:14:42  prez
** Ensured chanserv infinate loops wont occur, added 2 new cmdline
** paramaters, and added a manpage (you need to perl2pod it tho).
**
** Revision 1.194  2000/08/08 09:58:55  prez
** Added ModeO to 4 pre-defined committees.
** Also added back some deletes in xml in the hope that it
** will free up some memory ...
**
** Revision 1.193  2000/08/07 12:20:27  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.192  2000/08/06 07:25:10  prez
** Fixed some minor channel bugs
**
** Revision 1.191  2000/08/06 05:27:46  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.190  2000/08/03 13:06:30  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.189  2000/08/02 20:08:56  prez
** Minor code cleanups, added ACE installation instructions, updated the
** suggestions file and stopped people doing a whole bunch of stuff to
** forbidden nicknames.
**
** Revision 1.188  2000/07/30 09:04:05  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.187  2000/07/29 21:58:52  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.186  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.185  2000/06/28 12:20:47  prez
** Lots of encryption stuff, but essentially, we now have random
** key generation for the keyfile keys, and we can actually encrypt
** something, and get it back as we sent it in (specifically, the
** keyfile itself).
**
** Revision 1.184  2000/06/25 10:32:41  prez
** Fixed channel forbid.
**
** Revision 1.183  2000/06/25 07:58:49  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.182  2000/06/18 13:31:47  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.181  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.180  2000/06/16 14:47:31  prez
** Finished chanserv locking ...
**
** Revision 1.179  2000/06/15 13:41:11  prez
** Added my tasks to develop *grin*
** Also did all the chanserv live locking (stored to be done).
** Also made magick check if its running, and kill on startup if so.
**
** Revision 1.178  2000/06/13 14:11:53  prez
** Locking system has been re-written, it doent core anymore.
** So I have set 'MAGICK_LOCKS_WORK' define active :)
**
** Revision 1.177  2000/06/12 06:07:50  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.176  2000/06/11 09:30:20  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.175  2000/06/11 08:20:12  prez
** More minor bug fixes, godda love testers.
**
** Revision 1.174  2000/06/10 07:01:02  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.173  2000/06/06 08:57:55  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.172  2000/05/27 15:10:11  prez
** Misc changes, mainly re-did the makefile system, makes more sense.
** Also added a config.h file.
**
** Revision 1.171  2000/05/27 07:06:01  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.170  2000/05/25 08:16:38  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.169  2000/05/21 04:49:39  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.168  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.167  2000/05/14 06:30:13  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.166  2000/05/14 04:02:52  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.165  2000/05/10 11:46:59  prez
** added back memo timers
**
** Revision 1.164  2000/05/08 14:42:01  prez
** More on xmlisation of nickserv and chanserv
**
** Revision 1.163  2000/04/30 03:48:28  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.162  2000/04/16 14:29:43  prez
** Added stats for usage, and standardized grabbing paths, etc.
**
** Revision 1.161  2000/04/04 03:13:50  prez
** Added support for masking hostnames.
**
** Revision 1.160  2000/04/03 09:45:21  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.159  2000/04/02 13:06:03  prez
** Fixed the channel TOPIC and MODE LOCK stuff ...
**
** Also fixed the setting of both on join...
**
** Revision 1.158  2000/04/02 08:08:08  prez
** Misc Changes
**
** Revision 1.157  2000/03/29 09:41:17  prez
** Attempting to fix thread problem with mBase, and added notification
** of new memos on join of channel or signon to network.
**
** Revision 1.156  2000/03/28 16:20:57  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.155  2000/03/28 09:42:10  prez
** Changed CommServ, ADD/DEL/LIST -> MEMBER ADD/DEL/LIST
** and NEW/KILL -> ADD/DEL and created a new LIST
**
** Revision 1.154  2000/03/27 21:26:12  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.153  2000/03/27 10:40:11  prez
** Started implementing revenge
**
** Revision 1.152  2000/03/26 14:59:36  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.151  2000/03/24 15:35:17  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.150  2000/03/19 08:50:53  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.149  2000/03/14 15:10:15  prez
** OK -- more stuff with SJOIN/SNICK -- but it WORKS!!!
**
** Revision 1.148  2000/03/14 10:02:48  prez
** Added SJOIN and SNICK
**
** Revision 1.147  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.146  2000/03/07 09:53:19  prez
** More helpfile updates (and associated updates).
**
** Revision 1.145  2000/03/02 07:25:10  prez
** Added stuff to do the chanserv greet timings (ie. only greet if a user has
** been OUT of channel over 'x' seconds).  New stored chanserv cfg item.
**
** Revision 1.144  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.143  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.142  2000/02/17 12:55:04  ungod
** still working on borlandization
**
** Revision 1.141  2000/02/16 12:59:38  ungod
** fixing for borland compilability
**
** Revision 1.140  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.139  2000/02/15 10:37:48  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"
#include "dccengine.h"

#ifndef MAGICK_HAS_EXCEPTIONS
static Chan_Stored_t GLOB_Chan_Stored_t;
static Chan_Live_t GLOB_Chan_Live_t;
#endif

#ifdef __BORLAND__
#pragma codeseg CODE2
#endif

// Private functions

bool Chan_Live_t::Join(const mstring& nick)
{
    FT("Chan_Live_t::Join", (nick));

    RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "users"),
	users.find(nick.LowerCase())!=users.end())
    {
	LOG(LM_WARNING, "ERROR/DUP_CHAN", (
		"JOIN", nick, i_Name));
	RET(false);
    }
    else
    {
	MCB(users.size());
	{ WLOCK(("ChanServ", "live", i_Name.LowerCase(), "squit"));
	if (squit.find(nick.LowerCase())!=squit.end())
	    squit.erase(nick.LowerCase());
	}
	{ WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
	users[nick.LowerCase()] = pair<bool,bool>(false,false);
	}
	MCE(users.size());
	RET(true);
    }
}

unsigned int Chan_Live_t::Part(const mstring& nick)
{
    FT("Chan_Live_t::Part", (nick));
    RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "users"),
	users.find(nick.LowerCase())!=users.end())
    {
	mstring target = nick.LowerCase();
	if (Parent->nickserv.IsStored(nick) &&
	    Parent->nickserv.GetStored(nick).IsOnline() &&
	    !Parent->nickserv.GetStored(nick).Host().empty())
	    target = Parent->nickserv.GetStored(nick).Host().LowerCase();
	MCB(users.size());
	CB(1, recent_parts.size());
	{ WLOCK(("ChanServ", "live", i_Name.LowerCase(), "recent_parts"));
	recent_parts[target.LowerCase()] = mDateTime::CurrentDateTime();
	}
	{ WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
	users.erase(nick.LowerCase());
	}
	if (!users.size())
	{
	    CB(2, modes);
	    CB(3, i_Limit);
	    CB(4, i_Key);
	    { WLOCK(("ChanServ", "live", i_Name.LowerCase(), "modes"));
	    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "i_Limit"));
	    WLOCK3(("ChanServ", "live", i_Name.LowerCase(), "i_Key"));
	    modes.erase();
	    i_Limit = 0;
	    i_Key.erase();
	    }
	    CE(2, modes);
	    CE(3, i_Limit);
	    CE(4, i_Key);
	}
	CE(1, recent_parts.size());
	MCE(users.size());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.GetStored(i_Name).Part(nick);
    }
    else
    {
	RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "squit"),
	    squit.find(nick.LowerCase())!=squit.end())
	{
	    MCB(squit.size());
	    { WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
	    squit.erase(nick.LowerCase());
	    }
	    MCE(squit.size());
	    if (Parent->chanserv.IsStored(i_Name))
		Parent->chanserv.GetStored(i_Name).Part(nick);
	}
	else
	{
	    LOG(LM_TRACE, "ERROR/REC_FORNOTINCHAN", (
		"PART", nick, i_Name));
	}
    }

    unsigned int retval = users.size() + squit.size();
    RET(retval);
}

void Chan_Live_t::SquitUser(const mstring& nick)
{
    FT("Chan_Live_t::SquitUser", (nick));
    RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "squit"),
	users.find(nick.LowerCase())!=users.end())
    {
	MCB(squit.size());
	CB(1, users.size());
	{ WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "squit"));
	squit[nick.LowerCase()] = users[nick.LowerCase()];
	}
	{ WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
	users.erase(nick.LowerCase());
	}
	CE(1, users.size());
	MCE(squit.size());
    }
}

void Chan_Live_t::UnSquitUser(const mstring& nick)
{
    FT("Chan_Live_t::UnSquitUser", (nick));

    // We'll get ALL modes if all users are squit
    if (!users.size())
    {
	MCB(modes);
	{ WLOCK(("ChanServ", "live", i_Name.LowerCase(), "modes"));
	modes.erase();
	}
	MCE(modes);
    }

    RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "squit"),
	squit.find(nick.LowerCase())==squit.end())
    {
	LOG(LM_WARNING, "ERROR/REC_FORNOTINCHAN", (
		"UNSQUIT", nick, i_Name));
    }
    else
	Part(nick);
}

unsigned int Chan_Live_t::Kick(const mstring& nick, const mstring& kicker)
{
    FT("Chan_Live_t::Kick", (nick, kicker));
    RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "users"),
	users.find(nick.LowerCase())!=users.end())
    {
	MCB(users.size());
	{ WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
	users.erase(nick.LowerCase());
	}
	MCE(users.size());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.GetStored(i_Name).Kick(nick, kicker);
    }
    else
    {
	RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "squit"),
		squit.find(nick.LowerCase())!=squit.end())
	{
	    MCB(squit.size());
	    { WLOCK(("ChanServ", "live", i_Name.LowerCase(), "squit"));
	    squit.erase(nick.LowerCase());
	    }
	    MCE(squit.size());
	    if (Parent->chanserv.IsStored(i_Name))
		Parent->chanserv.GetStored(i_Name).Kick(nick, kicker);
	}
	else
	{
	    LOG(LM_WARNING, "ERROR/REC_NOTINCHAN", (
		"KICK", kicker, nick, i_Name));
	}
    }

    unsigned int retval = users.size() + squit.size();
    RET(retval);
}

void Chan_Live_t::ChgNick(const mstring& nick, const mstring& newnick)
{
    FT("Chan_Live_t::ChgNick", (nick, newnick));
    RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "users"),
	users.find(nick.LowerCase())!=users.end())
    {
	MCB(users.size());
	{ WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
	users[newnick.LowerCase()] = users[nick.LowerCase()];
	users.erase(nick.LowerCase());
	}
	MCE(users.size());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.GetStored(i_Name).ChgNick(nick, newnick);
    }
    else
    {
	RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "squit"),
		squit.find(nick.LowerCase())!=squit.end())
	{
	    MCB(squit.size());
	    { WLOCK(("ChanServ", "live", i_Name.LowerCase(), "squit"));
	    squit[newnick.LowerCase()] = squit[nick.LowerCase()];
	    squit.erase(nick.LowerCase());
	    }
	    MCE(squit.size());
	}
	else
	{
	    LOG(LM_WARNING, "ERROR/REC_FORNOTINCHAN", (
		"NICK", nick, i_Name));
	}
    }
}


Chan_Live_t::Chan_Live_t(const mstring& name, const mstring& first_user)
	: i_Name(name), i_Limit(0), ph_timer(0)
{
    FT("Chan_Live_t::Chan_Live_t", (name, first_user));
    users[first_user.LowerCase()] = pair<bool,bool>(false,false);
    DumpB();
}


void Chan_Live_t::operator=(const Chan_Live_t &in)
{
    NFT("Chan_Live_t::operator=");

    i_Name=in.i_Name;
    i_Creation_Time=in.i_Creation_Time;
    users.clear();
    map<mstring, pair<bool, bool> >::const_iterator k;
    for(k=in.users.begin();k!=in.users.end();k++)
	users.insert(*k);
    for(k=in.squit.begin();k!=in.squit.end();k++)
	squit.insert(*k);
    bans.clear();
    exempt.clear();
    map<mstring, mDateTime>::const_iterator i;
    for(i=in.bans.begin();i!=in.bans.end();i++)
	bans.insert(*i);
    for(i=in.exempt.begin();i!=in.exempt.end();i++)
	exempt.insert(*i);
    i_Topic=in.i_Topic;
    i_Topic_Set_Time=in.i_Topic_Set_Time;
    i_Topic_Setter=in.i_Topic_Setter;
    modes=in.modes;
    i_Key=in.i_Key;
    i_Limit=in.i_Limit;
    p_modes_on=in.p_modes_on;
    p_modes_off=in.p_modes_off;
    p_modes_on_params.clear();
    p_modes_on_params=in.p_modes_on_params;
    p_modes_off_params.clear();
    p_modes_off_params=in.p_modes_off_params;
    ph_timer=in.ph_timer;
    recent_parts.clear();
    for(i=in.recent_parts.begin(); i!=in.recent_parts.end(); i++)
	recent_parts.insert(*i);
    i_UserDef.clear();
    map<mstring,mstring>::const_iterator j;
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
}

mDateTime Chan_Live_t::Creation_Time() const 
{
    NFT("Chan_Live_t::Creation_Time");
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Creation_Time"));
    RET(i_Creation_Time);
}


void Chan_Live_t::Topic(const mstring& source, const mstring& topic, const mstring& setter, const mDateTime& time)
{
    FT("Chan_Live_t::Topic", (source, topic, setter, time));
    { WLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Topic"));
    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "i_Topic_Setter"));
    WLOCK3(("ChanServ", "live", i_Name.LowerCase(), "i_Topic_Set_Time"));
    MCB(i_Topic);
    CB(1, i_Topic_Setter);
    CB(2, i_Topic_Set_Time);
    i_Topic = topic;
    i_Topic_Setter = setter;
    i_Topic_Set_Time = time;
    CE(1, i_Topic_Setter);
    CE(2, i_Topic_Set_Time);
    MCE(i_Topic);
    }
    if (Parent->chanserv.IsStored(i_Name))
	Parent->chanserv.GetStored(i_Name).Topic(source, topic, setter, time);
}


mstring Chan_Live_t::Topic() const
{
    NFT(("Chan_Live_t::Topic"));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Topic"));
    RET(i_Topic);
}

mstring Chan_Live_t::Topic_Setter() const
{
    NFT(("Chan_Live_t::Topic_Setter"));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Topic_Setter"));
    RET(i_Topic_Setter);
}

mDateTime Chan_Live_t::Topic_Set_Time() const
{
    NFT(("Chan_Live_t::Topic_Set_Time"));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Topic_Set_Time"));
    RET(i_Topic_Set_Time);
}


unsigned int Chan_Live_t::Squit() const
{
    NFT("Chan_Live_t::Squit");
    unsigned int retval = squit.size();
    RET(retval);
}


mstring Chan_Live_t::Squit(const unsigned int num) const
{
    FT("Chan_Live_t::Squit", (num));
    unsigned int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "squit"));
    for(i=0, k=squit.begin();k!=squit.end();k++, i++)
	if (i==num)
	{
	    RET(k->first);
	}

    RET("");
}


unsigned int Chan_Live_t::Users() const 
{
    NFT("Chan_Live_t::Users");
    unsigned int retval = users.size();
    RET(retval);
}


mstring Chan_Live_t::User(const unsigned int num) const
{
    FT("Chan_Live_t::User", (num));
    unsigned int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    for(i=0, k=users.begin();k!=users.end();k++, i++)
	if (i==num)
	{
	    RET(k->first);
	}
 
    RET("");
}


unsigned int Chan_Live_t::Ops() const
{
    NFT("Chan_Live_t::Ops");
    unsigned int count = 0;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    map<mstring,pair<bool,bool> >::const_iterator i;
    for (i=users.begin(); i!=users.end(); i++)
	if (i->second.first)
	    count++;
    RET(count);
}


mstring Chan_Live_t::Op(const unsigned int num) const
{
    FT("Chan_Live_t::Op", (num));
    unsigned int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    for(i=0, k=users.begin();k!=users.end();k++)
	if (IsOp(k->first))
	{
	    if (i==num)
	    {
		RET(k->first);
	    }
	    i++;
	}

    RET("");
}


unsigned int Chan_Live_t::Voices() const
{
    NFT("Chan_Live_t::Voices");
    unsigned int count = 0;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    map<mstring,pair<bool,bool> >::const_iterator i;
    for (i=users.begin(); i!=users.end(); i++)
	if (i->second.second)
	    count++;
    RET(count);
}


mstring Chan_Live_t::Voice(const unsigned int num) const
{
    FT("Chan_Live_t::Voice", (num));
    unsigned int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    for(i=0, k=users.begin();k!=users.end();k++)
	if (IsVoice(k->first))
	{
	    if (i==num)
	    {
		RET(k->first);
	    }
	    i++;
	}
    RET("");
}



pair<bool,bool> Chan_Live_t::User(const mstring& name) const
{
    FT("Chan_Live_t::User", (name));
    if (IsIn(name))
    {
	RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
	map<mstring, pair<bool,bool> >::const_iterator i = users.find(name.LowerCase());
	NRET(pair<bool.bool>, i->second);
    }
    else
    {
	pair<bool,bool> tmp(false,false);
	NRET(pair<bool.bool>, tmp);
    }
}


unsigned int Chan_Live_t::Bans() const
{
    NFT("Chan_Live_t::Bans");
    unsigned int retval = bans.size();
    RET(retval);
}


mstring Chan_Live_t::Ban(const unsigned int num) const
{
    FT("Chan_Live_t::Ban", (num));
    unsigned int i;
    map<mstring, mDateTime>::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "bans"));
    for(i=0, k=bans.begin();k!=bans.end();k++, i++)
	if (i==num)
	{
	    RET(k->first);
	}
 
    RET("");
}


mDateTime Chan_Live_t::Ban(const mstring& mask) const
{
    FT("Chan_Live_t::Ban", (mask));
    mDateTime retval(0.0);
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "bans"));
    map<mstring,mDateTime>::const_iterator i = bans.find(mask.LowerCase());
    if (i != bans.end())
    {
	retval = i->second;
    }
    RET(retval);
}


unsigned int Chan_Live_t::Exempts() const
{
    NFT("Chan_Live_t::Exempts");
    unsigned int retval = exempt.size();
    RET(retval);
}


mstring Chan_Live_t::Exempt(const unsigned int num) const 
{
    FT("Chan_Live_t::Exempt", (num));
    unsigned int i;
    map<mstring, mDateTime>::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
    for(i=0, k=exempt.begin();k!=exempt.end();k++, i++)
	if (i==num)
	{
	    RET(k->first);
	}
 
    RET("");
}


mDateTime Chan_Live_t::Exempt(const mstring& mask) const
{
    FT("Chan_Live_t::Exempt", (mask));
    mDateTime retval(0.0);
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
    map<mstring,mDateTime>::const_iterator i = exempt.find(mask.LowerCase());
    if (i != exempt.end())
    {
	retval = i->second;
    }
    RET(retval);
}


bool Chan_Live_t::IsSquit(const mstring& nick)const 
{
    FT("Chan_Live_t::IsSquit", (nick));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "squit"));
    bool retval = (squit.find(nick.LowerCase()) != squit.end());
    RET(retval);
}


bool Chan_Live_t::IsIn(const mstring& nick) const
{
    FT("Chan_Live_t::IsIn", (nick));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    bool retval = (users.find(nick.LowerCase()) != users.end());
    RET(retval);
}


bool Chan_Live_t::IsOp(const mstring& nick) const
{
    FT("Chan_Live_t::IsOp", (nick));

    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    map<mstring, pair<bool,bool> >::const_iterator i = users.find(nick.LowerCase());
    if (i != users.end() && i->second.first)
    {
	RET(true);
    }
    RET(false);
}


bool Chan_Live_t::IsVoice(const mstring& nick) const
{
    FT("Chan_Live_t::IsVoice", (nick));

    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    map<mstring, pair<bool,bool> >::const_iterator i = users.find(nick.LowerCase());
    if (i != users.end() && i->second.second)
    {
	RET(true);
    }
    RET(false);
    RET(false);
}


bool Chan_Live_t::IsBan(const mstring& mask) const
{
    FT("Chan_Live_t::IsBan", (mask));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "bans"));
    bool retval = (bans.find(mask.LowerCase()) != bans.end());
    RET(retval);
}

bool Chan_Live_t::MatchBan(const mstring& mask) const
{
    FT("Chan_Live_t::MatchBan", (mask));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "bans"));
    map<mstring, mDateTime>::const_iterator i;
    for (i=bans.begin(); i!=bans.end(); i++)
    {
	if (mask.Matches(i->first, true))
	{
	    RET(true);
	}
    }
    RET(false);
}

bool Chan_Live_t::IsExempt(const mstring& mask) const
{
    FT("Chan_Live_t::IsExempt", (mask));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
    bool retval = (exempt.find(mask.LowerCase()) != exempt.end());
    RET(retval);
}

bool Chan_Live_t::MatchExempt(const mstring& mask) const 
{
    FT("Chan_Live_t::MatchExempt", (mask));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
    map<mstring, mDateTime>::const_iterator i;
    for (i=exempt.begin(); i!=exempt.end(); i++)
    {
	if (mask.Matches(i->first, true))
	{
	    RET(true);
	}
    }
    RET(false);
}

void Chan_Live_t::LockDown()
{
    NFT("Chan_Live_t::LockDown");

    Parent->server.JOIN(Parent->chanserv.FirstName(), i_Name);
    // Override the MLOCK checking.
    SendMode("+s");
    MLOCK(("ChanServ", "live", i_Name.LowerCase(), "ph_timer"));
    MCB(ph_timer);
    ph_timer = ACE_Reactor::instance()->schedule_timer(&(Parent->chanserv.ph),
			    new mstring(i_Name),
			    ACE_Time_Value(Parent->chanserv.ChanKeep()));
    MCE(ph_timer);
}

void Chan_Live_t::UnLock()
{
    NFT("Chan_Live_t::UnLock");

    if (Parent->chanserv.IsStored(i_Name))
    {
	if (!Parent->chanserv.GetStored(i_Name).Mlock_On().Contains("s") && modes.Contains("s"))
	    SendMode("-s");
    }

    mstring *arg = NULL;
    MLOCK(("ChanServ", "live", i_Name.LowerCase(), "ph_timer"));
    MCB(ph_timer);
    if (ph_timer &&
	ACE_Reactor::instance()->cancel_timer(ph_timer,
		reinterpret_cast<const void **>(arg)) &&
	arg != NULL)
    {
	    delete arg;
    }
    ph_timer = 0;
    MCE(ph_timer);
}

bool Chan_Live_t::ModeExists(const mstring& mode, const vector<mstring>& mode_params,
	const bool change, const char reqmode, const mstring& reqparam)
{
    FT("Chan_Live_t::ModeExists", (mode, "vector<mstring> mode_params", change, reqmode, reqparam));
    unsigned int i, param;

    if (reqparam.empty())
	if (mode.Contains(reqmode))
	{
	    RET(true);
	}
	else
	{
	    RET(false);
	}

    for (param=0, i=0; i<mode.size(); i++)
    {
	if (Parent->server.proto.ChanModeArg().Contains(mode[i]))
	{
	    if (mode[i] == reqmode && param < mode_params.size()
		&& mode_params[param] == reqparam)
	    {
		    RET(true);
	    }
	    if (mode[i] != 'l' || change)
		param++;
	}
	else
	{
	    if (mode[i] == reqmode)
	    {
		RET(true);
	    }
	}
    }
    RET(false);
}


void Chan_Live_t::RemoveMode(mstring& mode, vector<mstring>& mode_params,
	const bool change, const char reqmode, const mstring& reqparam)
{
    FT("Chan_Live_t::RemoveMode", (mode, "vector<mstring> mode_params", change, reqmode, reqparam));
    unsigned int i, param;
    mstring new_mode;
    vector<mstring> new_params;

    for (param=0, i=0; i<mode.size(); i++)
    {
	if (Parent->server.proto.ChanModeArg().Contains(mode[i]))
	{
	    if (reqmode == mode[i] && param < mode_params.size())
	    {
		if ((!reqparam.empty() && mode_params[param] == reqparam) ||
		    reqparam.empty())
		{
		    // Do nothing ... we want to delete it!
		}
		else
		{
		    new_mode += mode[i];
		    if (mode[i] != 'l' || change)
			new_params.push_back(mode_params[param]);
		}
	    }
	    if (mode[i] != 'l' || !change)
		param++;
	}
	else
	{
	    if (mode[i] == reqmode)
	    {
		// Do nothing ... we want to delete it!
	    }
	    else
	    {
		new_mode += mode[i];
	    }
	}
    }
    mode = new_mode;
    mode_params = new_params;
}


void Chan_Live_t::SendMode(const mstring& in)
{
    FT("Chan_Live_t::SendMode", (in));
    unsigned int i, param = 2;
    mstring mode(in.Before(" "));

    bool add = true;
    bool cstored = false;
    mstring s_key, s_mlock_on, s_mlock_off;
    int s_limit = 0;

    if (Parent->chanserv.IsStored(i_Name))
    {
	cstored = true;
	Chan_Stored_t cs = Parent->chanserv.GetStored(i_Name);
	s_key = cs.Mlock_Key();
	s_mlock_on = cs.Mlock_On();
	s_mlock_off = cs.Mlock_Off();
	s_limit = cs.Mlock_Limit();	
    }

    { WLOCK(("ChanServ", "live", i_Name.LowerCase(), "p_modes_on"));
    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "p_modes_on_params"));
    WLOCK3(("ChanServ", "live", i_Name.LowerCase(), "p_modes_off"));
    WLOCK4(("ChanServ", "live", i_Name.LowerCase(), "p_modes_off_params"));
    MCB(p_modes_on);
    CB(1, p_modes_on_params.size());
    CB(2, p_modes_off);
    CB(3, p_modes_off_params.size());
    for (i=0; i<mode.size(); i++)
    {
	if (mode[i] == '+')
	{
	    add = true;
	}
	else if (mode[i] == '-')
	{
	    add = false;
	}
	else if (Parent->server.proto.ChanModeArg().Contains(mode[i]))
	{
	    switch (mode[i])
	    {
	    case 'o':
		if (in.WordCount(" ") >= param)
		{
		    if (add)
		    {
			if (!IsOp(in.ExtractWord(param, " ")))
			{
			    if (ModeExists(p_modes_off, p_modes_off_params, false, 'o', in.ExtractWord(param, " ")))
				RemoveMode(p_modes_off, p_modes_off_params, false, 'o', in.ExtractWord(param, " "));
			    if (!ModeExists(p_modes_on, p_modes_on_params, true, 'o', in.ExtractWord(param, " ")))
			    {
				p_modes_on += "o";
				p_modes_on_params.push_back(in.ExtractWord(param, " "));
			    }
			}
		    }
		    else
		    {
			if (IsOp(in.ExtractWord(param, " ")))
			{
			    if (ModeExists(p_modes_on, p_modes_on_params, true, 'o', in.ExtractWord(param, " ")))
				RemoveMode(p_modes_on, p_modes_on_params, true, 'o', in.ExtractWord(param, " "));
			    if (!ModeExists(p_modes_off, p_modes_off_params, false, 'o', in.ExtractWord(param, " ")))
			    {
				p_modes_off += "o";
				p_modes_off_params.push_back(in.ExtractWord(param, " "));
			    }
			}
		    }
		    param++;
		}
		break;

	    case 'v':
	        if (in.WordCount(" ") >= param)
		{
		    if (add)
		    {
			if (!IsVoice(in.ExtractWord(param, " ")))
			{
			    if (ModeExists(p_modes_off, p_modes_off_params, false, 'v', in.ExtractWord(param, " ")))
				RemoveMode(p_modes_off, p_modes_off_params, false, 'v', in.ExtractWord(param, " "));
			    if (!ModeExists(p_modes_on, p_modes_on_params, true, 'v', in.ExtractWord(param, " ")))
			    {
				p_modes_on += "v";
				p_modes_on_params.push_back(in.ExtractWord(param, " "));
			    }
			}
		    }
		    else
		    {
			if (IsVoice(in.ExtractWord(param, " ")))
			{
			    if (ModeExists(p_modes_on, p_modes_on_params, true, 'v', in.ExtractWord(param, " ")))
				RemoveMode(p_modes_on, p_modes_on_params, true, 'v', in.ExtractWord(param, " "));
			    if (!ModeExists(p_modes_off, p_modes_off_params, false, 'v', in.ExtractWord(param, " ")))
			    {
				p_modes_off += "v";
				p_modes_off_params.push_back(in.ExtractWord(param, " "));
			    }
			}
		    }
		    param++;
		}
		break;

	    case 'b':
		if (in.WordCount(" ") >= param)
		{
		    if (add)
		    {
			if (!IsBan(in.ExtractWord(param, " ")))
			{
			    if (ModeExists(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(param, " ")))
				RemoveMode(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(param, " "));
			    if (!ModeExists(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(param, " ")))
			    {
				p_modes_on += "b";
				p_modes_on_params.push_back(in.ExtractWord(param, " "));
			    }
			}
		    }
		    else
		    {
			if (IsBan(in.ExtractWord(param, " ")))
			{
			    if (ModeExists(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(param, " ")))
				RemoveMode(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(param, " "));
			    if (!ModeExists(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(param, " ")))
			    {
				p_modes_off += "b";
				p_modes_off_params.push_back(in.ExtractWord(param, " "));
			    }
			}
		    }
		    param++;
		}
		break;

	    case 'e':
		if (in.WordCount(" ") >= param)
		{
		    if (add)
		    {
			if (!IsExempt(in.ExtractWord(param, " ")))
			{
			    if (ModeExists(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(param, " ")))
				RemoveMode(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(param, " "));
			    if (!ModeExists(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(param, " ")))
			    {
				p_modes_on += "e";
				p_modes_on_params.push_back(in.ExtractWord(param, " "));
			    }
			}
		    }
		    else
		    {
			if (IsExempt(in.ExtractWord(param, " ")))
			{
			    if (ModeExists(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(param, " ")))
				RemoveMode(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(param, " "));
			    if (!ModeExists(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(param, " ")))
			    {
				p_modes_off += "e";
				p_modes_off_params.push_back(in.ExtractWord(param, " "));
			    }
			}
		    }
		    param++;
		}
		break;

	    case 'l':
		if (add)
		{
		    if (!(cstored && s_mlock_off.Contains("l")))
		    {
			if (in.WordCount(" ") >= param)
			{
			    if (ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
				RemoveMode(p_modes_off, p_modes_off_params, false, 'l');
			    if (ModeExists(p_modes_on, p_modes_on_params, true, 'l'))
				RemoveMode(p_modes_on, p_modes_on_params, true, 'l');
			    p_modes_on += "l";
			    p_modes_on_params.push_back(in.ExtractWord(param, " "));
			    param++;
			}
		    }
		}
		else
		{
		    if (!(cstored && s_limit))
		    {
			if (i_Limit)
			{
			    if (ModeExists(p_modes_on, p_modes_on_params, true, 'l'))
				RemoveMode(p_modes_on, p_modes_on_params, true, 'l');
			    if (!ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
				p_modes_off += "l";
			}
		    }
		}
		break;
	    case 'k':
		if (in.WordCount(" ") >= param)
		{
		    if (add)
		    {
			// ONLY allow +k if we've turned it off before, or one isnt set
			if (i_Key.empty() || ModeExists(p_modes_off, p_modes_off_params, false, 'k'))
			{
			    if (!(cstored && s_mlock_off.Contains("k")))
			    {
				// DONT take off 'off' value, coz we can -k+k key1 key2
				if (!ModeExists(p_modes_on, p_modes_on_params, true, 'k'))
				{
				    p_modes_on += "k";
				    p_modes_on_params.push_back(in.ExtractWord(param, " "));
				}
			    }
			}
		    }
		    else
		    {
			if (i_Key == in.ExtractWord(param, " "))
			{
			    if (!(cstored && !s_key.empty()))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'k'))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'k');
				if (!ModeExists(p_modes_off, p_modes_off_params, false, 'k'))
				{
				    p_modes_off += "k";
				    p_modes_off_params.push_back(in.ExtractWord(param, " "));
				}
			    }
			}
		    }
		    param++;
		}
		break;
	    default:
		if (in.WordCount(" ") >= param)
		    param++;
	    }
	}
	else
	{
	    if (add)
	    {
		RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "modes"),
			!modes.Contains(mode[i]))
		{
		    if (!(cstored && s_mlock_off.Contains(mode[i])))
		    {
			if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
			    RemoveMode(p_modes_off, p_modes_off_params, false, mode[i]);
			if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
			    p_modes_on += mode[i];
		    }
		}
	    }
	    else
	    {
		RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "modes"),
			modes.Contains(mode[i]))
		{
		    if (!(cstored && s_mlock_on.Contains(mode[i])))
		    {
			if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
			    RemoveMode(p_modes_on, p_modes_on_params, true, mode[i]);
			if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
			    p_modes_off += mode[i];
		    }
		}
	    }
	}
    }	
    CE(1, p_modes_on_params.size());
    CE(2, p_modes_off);
    CE(3, p_modes_off_params.size());
    MCE(p_modes_on);
    }
    RLOCK2(("Events"));
    if (Parent->events != NULL)
	Parent->events->AddChannelModePending(i_Name);
}


void Chan_Live_t::Mode(const mstring& source, const mstring& in)
{
    FT("Chan_Live_t::Mode", (source, in));

    mstring change(in.ExtractWord(1, ": "));
    mstring newmode, newmode_param;
    unsigned int fwdargs = 2, i, wc;
    bool add = true;
    wc = in.WordCount(": ");
    CP(("MODE CHANGE (%s): %s", i_Name.c_str(), in.c_str()));

    { WLOCK(("ChanServ", "live", i_Name.LowerCase(), "p_modes_on"));
    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "p_modes_on_params"));
    WLOCK3(("ChanServ", "live", i_Name.LowerCase(), "p_modes_off"));
    WLOCK4(("ChanServ", "live", i_Name.LowerCase(), "p_modes_off_params"));
    MCB(modes);
    CB(1, p_modes_on);
    CB(2, p_modes_on_params.size());
    CB(3, p_modes_off);
    CB(4, p_modes_off_params.size());
    for (fwdargs=2, i=0; i<change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	    newmode += change[i];
	}
	else if (change[i] == '-')
	{
	    add = false;
	    newmode += change[i];
	}
	else if (Parent->server.proto.ChanModeArg().Contains(change[i]))
	{
	    mstring arg(in.ExtractWord(fwdargs, ": "));
	    switch(change[i])
	    {
	    case 'o':
		if (fwdargs <= wc)
		{
		if (IsIn(arg))
		{
		    if (add)
		    {
			{ WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "users"));
			users[arg.LowerCase()].first = true;
			}
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'o', arg))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'o', arg);
		    }
		    else
		    {
			{ WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "users"));
			users[arg.LowerCase()].first = false;
			}
			if (ModeExists(p_modes_off, p_modes_off_params, false, 'o', arg))
			    RemoveMode(p_modes_off, p_modes_off_params, false, 'o', arg);
		    }
		    newmode += change[i];
		    newmode_param += " " + arg;
		}
		else
		{
		    LOG(LM_WARNING, "ERROR/MODE_NOTINCHAN", (
			add ? '+' : '-', change[i], source,
			arg, i_Name));
		}
		fwdargs++;
		}
		break;

	    case 'v':
		if (fwdargs <= wc)
		{
		if (IsIn(arg))
		{
		    if (add)
		    {
			{ WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "users"));
			users[arg.LowerCase()].second = true;
			}
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'v', arg))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'v', arg);
		    }
		    else
		    {
			{ WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "users"));
			users[arg.LowerCase()].second = false;
			}
			if (ModeExists(p_modes_off, p_modes_off_params, false, 'v', arg))
			    RemoveMode(p_modes_off, p_modes_off_params, false, 'v', arg);
		    }
		    newmode += change[i];
		    newmode_param += " " + arg;
		}
		else
		{
		    LOG(LM_WARNING, "ERROR/MODE_NOTINCHAN", (
			add ? '+' : '-', change[i], source,
			arg, i_Name));
		}
		fwdargs++;
		}
	        break;

	    case 'b':
		if (fwdargs <= wc)
		{
		{ RLOCK(("ChanServ", "live", i_Name.LowerCase(), "bans"));
		CB(5, bans.size());
		if (add)
		{
		    { WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "bans"));
		    bans[arg.LowerCase()] = mDateTime::CurrentDateTime();
		    }
		    if (ModeExists(p_modes_on, p_modes_on_params, true, 'b', arg))
			RemoveMode(p_modes_on, p_modes_on_params, true, 'b', arg);
		}
		else
		{
		    { WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "bans"));
		    bans.erase(arg.LowerCase());
		    }
		    if (ModeExists(p_modes_off, p_modes_off_params, false, 'b', arg))
			RemoveMode(p_modes_off, p_modes_off_params, false, 'b', arg);
		}
		CE(5, bans.size());
		}
		newmode += change[i];
		newmode_param += " " + arg;
		fwdargs++;
		}
		break;

	    case 'e':
		if (fwdargs <= wc)
		{
		{ RLOCK(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
		CB(5, exempt.size());
		if (add)
		{
		    { WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
		    exempt[arg.LowerCase()] = mDateTime::CurrentDateTime();
		    }
		    if (ModeExists(p_modes_on, p_modes_on_params, true, 'e', arg))
			RemoveMode(p_modes_on, p_modes_on_params, true, 'e', arg);
		}
		else
		{
		    { WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
		    exempt.erase(arg.LowerCase());
		    }
		    if (ModeExists(p_modes_off, p_modes_off_params, false, 'e', arg))
			RemoveMode(p_modes_off, p_modes_off_params, false, 'e', arg);
		}
		CE(5, exempt.size());
		}
		newmode += change[i];
		newmode_param += " " + arg;
		fwdargs++;
		}
		break;

	    case 'k':
		if (fwdargs <= wc)
		{
		{ RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Key"));
		CB(5, i_Key);
		if (add)
		{
		    { WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "i_Key"));
		    i_Key = arg;
		    }
		    if (ModeExists(p_modes_on, p_modes_on_params, true, 'k', arg))
			RemoveMode(p_modes_on, p_modes_on_params, true, 'k', arg);
		}
		else
		{
		    if (i_Key != arg)
		    {
			LOG(LM_ERROR, "ERROR/KEYMISMATCH", (
				i_Key, arg,
				i_Name, source));
		    }
		    { WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "i_Key"));
		    i_Key.erase();
		    }
		    if (ModeExists(p_modes_off, p_modes_off_params, false, 'k'))
			RemoveMode(p_modes_off, p_modes_off_params, false, 'k');
		}
		CE(5, i_Key);
		}
		newmode += change[i];
		newmode_param += " " + arg;
		fwdargs++;
		}
		break;

	    case 'l':
		if (add)
		{
		    if (fwdargs <= wc)
		    {
		    { RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Limit"));
		    CB(5, i_Limit);
		    if (fwdargs > in.WordCount(": "))
		    {
			LOG(LM_ERROR, "ERROR/NOLIMIT", (i_Name, source));
			WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "i_Limit"));
			i_Limit = 0;
		    }
		    else if (!arg.IsNumber())
		    {
			LOG(LM_ERROR, "ERROR/NOLIMIT", (i_Name, source));
			WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "i_Limit"));
			i_Limit = 0;
		    }
		    else
		    {
			{ WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "i_Limit"));
			i_Limit = atoi(arg.c_str());
			}
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'l', arg))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'l', arg);
			newmode += change[i];
			newmode_param += " " + arg;
		    }
		    CE(5, i_Limit);
		    }
		    fwdargs++;
		    }
		}
		else
		{
		    { WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "i_Limit"));
		    CB(5, i_Limit);
		    i_Limit = 0;
		    CE(5, i_Limit);
		    }
		    if (ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
			RemoveMode(p_modes_off, p_modes_off_params, false, 'l');
		    newmode += change[i];
		}
		break;
	    default:
		if (fwdargs <= wc)
		{
		    newmode += change[i];
		    newmode_param += " " + arg;
		    fwdargs++;
		}
	    }
	}
	else
	{
	    RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "modes"),
		add && !modes.Contains(change[i]))
	    {
		{ WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "modes"));
		modes += change[i];
		}
		if (ModeExists(p_modes_on, p_modes_on_params, true, change[i]))
		    RemoveMode(p_modes_on, p_modes_on_params, true, change[i]);
	    }
	    else
	    {
		RLOCK_IF(("ChanServ", "live", i_Name.LowerCase(), "modes"),
			!add && modes.Contains(change[i]))
		{
		    { WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "modes"));
		    modes.Remove(change[i]);
		    }
		    if (ModeExists(p_modes_off, p_modes_off_params, false, change[i]))
			RemoveMode(p_modes_off, p_modes_off_params, false, change[i]);
		}
		else
		{
		    LOG(LM_TRACE, "ERROR/INEFFECT", (
			add ? '+' : '-', change[i], source, i_Name));
		}
	    }
	    newmode += change[i];
	}
    }
    CE(1, p_modes_on);
    CE(2, p_modes_on_params.size());
    CE(3, p_modes_off);
    CE(4, p_modes_off_params.size());
    MCE(modes);
    }

    if (Parent->chanserv.IsStored(i_Name))
	Parent->chanserv.GetStored(i_Name).Mode(source,
						newmode + newmode_param);
}

bool Chan_Live_t::HasMode(const mstring& in) const
{
    FT("Chan_Live_t::HasMode", (in));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "modes"));
    RET(modes.Contains(in));
}

mstring Chan_Live_t::Mode() const 
{
    NFT("Chan_Live_t::Mode");
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "modes"));
    RET(modes);
}

mstring Chan_Live_t::Key() const
{
    NFT("Chan_Live_t::Key");
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Key"));
    RET(i_Key);
}

unsigned int Chan_Live_t::Limit() const
{
    NFT("Chan_Live_t::Limit");
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Limit"));
    RET(i_Limit);
}

mDateTime Chan_Live_t::PartTime(const mstring& nick) const
{
    FT("Chan_Live_t::PartTime", (nick));
    mDateTime retval(0.0);
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "recent_parts"));
    map<mstring, mDateTime>::const_iterator i = recent_parts.find(nick.LowerCase());
    if (i != recent_parts.end())
    {
	retval = i->second;
    }
    RET(retval);
}


size_t Chan_Live_t::Usage() const
{
    size_t retval = 0;

    WLOCK(("ChanServ", "live", i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_Creation_Time.Internal());
    map<mstring, pair<bool,bool> >::const_iterator i;
    for (i=squit.begin(); i!=squit.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second.first);
	retval += sizeof(i->second.second);
    }
    for (i=users.begin(); i!=users.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second.first);
	retval += sizeof(i->second.second);
    }
    map<mstring, mDateTime>::const_iterator j;
    for (j=bans.begin(); j!=bans.end(); j++)
    {
	retval += j->first.capacity();
	retval += sizeof(j->second.Internal());
    }
    for (j=exempt.begin(); j!=exempt.end(); j++)
    {
	retval += j->first.capacity();
	retval += sizeof(j->second.Internal());
    }
    retval += i_Topic.capacity();
    retval += i_Topic_Setter.capacity();
    retval += sizeof(i_Topic_Set_Time.Internal());
    retval += modes.capacity();
    retval += sizeof(i_Limit);
    retval += i_Key.capacity();
    retval += p_modes_on.capacity();
    retval += p_modes_off.capacity();
    vector<mstring>::const_iterator k;
    for (k=p_modes_on_params.begin(); k!=p_modes_on_params.end(); k++)
    {
	retval += k->capacity();
    }
    for (k=p_modes_off_params.begin(); k!=p_modes_off_params.end(); k++)
    {
	retval += k->capacity();
    }
    retval += sizeof(ph_timer);
    for (j=recent_parts.begin(); j!=recent_parts.end(); j++)
    {
	retval += j->first.capacity();
	retval += sizeof(j->second.Internal());
    }

    return retval;
}


void Chan_Live_t::DumpB() const
{
    MB(0, (i_Name, i_Creation_Time, squit.size(), users.size(), bans.size(),
	exempt.size(), i_Topic, i_Topic_Setter, i_Topic_Set_Time, modes,
	i_Limit, i_Key, p_modes_on, p_modes_off, p_modes_on_params.size(),
	p_modes_off_params.size()));
    MB(16, (ph_timer, recent_parts.size()));
}

void Chan_Live_t::DumpE() const 
{
    ME(0, (i_Name, i_Creation_Time, squit.size(), users.size(), bans.size(),
	exempt.size(), i_Topic, i_Topic_Setter, i_Topic_Set_Time, modes,
	i_Limit, i_Key, p_modes_on, p_modes_off, p_modes_on_params.size(),
	p_modes_off_params.size()));
    ME(16, (ph_timer, recent_parts.size()));
}


// --------- end of Chan_Live_t -----------------------------------


void Chan_Stored_t::ChgAttempt(const mstring& nick, const mstring& newnick)
{
    FT("Chan_Stored_t::ChgAttempt", (nick, newnick));

    map<mstring, unsigned int>::iterator iter;

    // Create a new one if we find the entry
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "failed_passwds"));
    MCB(failed_passwds.size());
    for (iter=failed_passwds.begin(); iter!=failed_passwds.end(); iter++)
	if (iter->first == nick.LowerCase())
	{
	    failed_passwds[newnick.LowerCase()] = iter->second;
	    break;
	}
    failed_passwds.erase(nick.LowerCase());
    MCE(failed_passwds.size());
}


bool Chan_Stored_t::Join(const mstring& nick)
{
    FT("Chan_Stored_t::Join", (nick));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONUSER", (
			"JOIN", i_Name, nick));
	RET(false);
    }
    Nick_Live_t nlive = Parent->nickserv.GetLive(nick);

    if (!Parent->chanserv.IsLive(i_Name))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", (
			"JOIN", nick, i_Name));
	RET(false);
    }
    Chan_Live_t clive = Parent->chanserv.GetLive(i_Name);
    size_t users = clive.Users();

    if (nlive.IsServices() &&
	Parent->chanserv.FirstName().IsSameAs(nick, true))
    {
	if (Parent->chanserv.IsLive(i_Name))
	    Parent->chanserv.GetLive(i_Name).SendMode("+o " + nick);
	RET(true);
    }

    bool burst = false;
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL)
	burst = Parent->ircsvchandler->Burst();
    }    

    if (Forbidden())
    {
	if (!nlive.HasMode("o"))
	{
	    if (Parent->chanserv.IsLive(i_Name))
	    {
		RLOCK(("ChanServ", "live", i_Name));
		Chan_Live_t &cptr = Parent->chanserv.GetLive(i_Name);

		// If this user is the only user in channel
		if (users == 1)
		    cptr.LockDown();

		RLOCK2(("ChanServ", "stored", i_Name, "i_Mlock_On"));
		cptr.SendMode("+" + i_Mlock_On +
			"b " + nlive.AltMask(Parent->operserv.Ignore_Method()));
	    }

	    // Can only insert with reason or default, so its safe.
	    mstring reason = parseMessage(Parent->getMessage(nick, "CS_STATUS/ISFORBIDDEN"),
						mVarArray(i_Name));
	    Parent->server.KICK(Parent->chanserv.FirstName(), nick,
		i_Name, reason);
	    RET(false);
	}
	RET(true);
    }

    { MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    if (Akick_find(nick))
    {
	if (Parent->chanserv.IsLive(i_Name))
	{
	    RLOCK(("ChanServ", "live", i_Name));
	    Chan_Live_t &cptr = Parent->chanserv.GetLive(i_Name);

	    // If this user is the only user in channel
	    if (users == 1)
		cptr.LockDown();

	    if (Parent->nickserv.IsStored(Akick->Entry()))
		cptr.SendMode("+b " +
			nlive.AltMask(Parent->operserv.Ignore_Method()));
	    else
		cptr.SendMode("+b " +
			Akick->Entry());
	}

	LOG(LM_DEBUG, "EVENT/AKICK", (
			nick, i_Name, Akick->Value()));

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), nick,
		i_Name, Akick->Value());

	RET(false);
    }}

    if (Restricted() && !Suspended() && GetAccess(nick) < 1)
    {
	if (Parent->chanserv.IsLive(i_Name))
	{
	    RLOCK(("ChanServ", "live", i_Name));
	    Chan_Live_t &cptr = Parent->chanserv.GetLive(i_Name);

	    // If this user is the only user in channel
	    if (users == 1)
		cptr.LockDown();

	    cptr.SendMode("+b " +
			nlive.AltMask(Parent->operserv.Ignore_Method()));
	}

	LOG(LM_DEBUG, "EVENT/RESTRICTED", (
			nick, i_Name));

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), nick,
		i_Name, Parent->getMessage(nick, "MISC/KICK_RESTRICTED"));

	RET(false);
    }

    if (Parent->chanserv.IsLive(i_Name))
	Parent->chanserv.GetLive(i_Name).UnLock();
    if (!Join() && users == 2 &&
			clive.IsIn(Parent->chanserv.FirstName()))
    {
	Parent->server.PART(Parent->chanserv.FirstName(), i_Name);
	users--;
    }

    if (users == 1)
    {
	mstring modes;
	{ RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
	RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
	RLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
	modes = i_Mlock_On;
	if (!i_Mlock_Key.empty())
	    modes << "k";
	if (i_Mlock_Limit)
	    modes << "l";
	if (!i_Mlock_Key.empty())
	    modes << " " << i_Mlock_Key;
	if (i_Mlock_Limit)
	    modes << " " << i_Mlock_Limit;
	}

	if (!modes.empty() && Parent->chanserv.IsLive(i_Name))
	{
	    Parent->chanserv.GetLive(i_Name).SendMode("+" + modes + " " + i_Mlock_Key + " " +
			(i_Mlock_Limit ? mstring(i_Mlock_Limit) : mstring("")));
	}

	{ RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic"));
	RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Setter"));
	RLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Set_Time"));
	// Carry over topic ..
	if (!burst && Keeptopic() && !i_Topic.empty())
	{
	    Parent->server.TOPIC(Parent->chanserv.FirstName(),
		i_Topic_Setter, i_Name, i_Topic, i_Topic_Set_Time);
	}}
    }

    if (!burst && Join() && users == 1)
    {
	Parent->server.JOIN(Parent->chanserv.FirstName(), i_Name);
	users++;
    }

    if (GetAccess(nick)>0)
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_LastUsed"));
	MCB(i_LastUsed);
	i_LastUsed = mDateTime::CurrentDateTime();
	MCE(i_LastUsed);
    }

    if (Parent->chanserv.IsLive(i_Name))
    {
	if (GetAccess(nick, "AUTOOP"))
	    Parent->chanserv.GetLive(i_Name).SendMode("+o " + nick);
	else if (GetAccess(nick, "AUTOVOICE"))
	    Parent->chanserv.GetLive(i_Name).SendMode("+v " + nick);
    }

    if (Suspended())
    {
	RET(true);
    }

    { MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    for(Message = Message_begin(); Message != Message_end(); Message++)
    {
	if (Parent->nickserv.IsLive(Parent->chanserv.FirstName()))
	    Parent->chanserv.notice(nick, "[" + i_Name + "] " + Message->Entry());
    }}

    mstring target;
    if (Parent->nickserv.IsStored(nick))
    {
	target = Parent->nickserv.GetStored(nick).Host();
	if (target.empty())
	    target = Parent->nickserv.GetStored(nick).Name();

	MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
	if (Greet_find(target) &&
		clive.PartTime(target).SecondsSince() > Parttime())
	{
	    if (Greet->Entry()[0U] == '!')
	    {
		if (Parent->nickserv.IsLive(Parent->chanserv.FirstName()))
		    Parent->chanserv.privmsg(i_Name, "[" + nick + "] " +
					Greet->Entry().After("!"));
	    }
	    else
	    {
		if (Parent->nickserv.IsLive(Parent->chanserv.FirstName()))
		    Parent->chanserv.privmsg(i_Name, "[" + nick + "] " +
						Greet->Entry());
	    }
	}
    }

    if (!target.empty() && GetAccess(nick, "READMEMO") &&
	Parent->memoserv.IsChannel(i_Name))
    {
	size_t count = Parent->memoserv.ChannelNewsCount(i_Name, nick);
	if (count)
	    SEND(Parent->memoserv.FirstName(), nick, "MS_STATUS/CS_UNREAD", (
		i_Name, count,
		Parent->memoserv.FirstName(), i_Name));
    }
    RET(true);
}


void Chan_Stored_t::Part(const mstring& nick)
{
    FT("Chan_Stored_t::Part", (nick));

    if (Parent->nickserv.IsLive(nick) &&
	Parent->nickserv.GetLive(nick).IsServices())
	return;

    if (GetAccess(nick)>0)
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_LastUsed"));
	MCB(i_LastUsed);
	i_LastUsed = mDateTime::CurrentDateTime();
	MCE(i_LastUsed);
    }

    size_t users = 0;
    if (Parent->chanserv.IsLive(i_Name))
	users = Parent->chanserv.GetLive(i_Name).Users();

    if (users == 1 && Join())
    {
	Parent->server.PART(Parent->chanserv.FirstName(), i_Name);
	users--;
    }

    if (users == 0 && (Mlock_On().Contains("i") || !Mlock_Key().empty()))
    {
	Parent->server.JOIN(Parent->chanserv.FirstName(), i_Name);
	if (Parent->chanserv.IsLive(i_Name))
	{
	    mstring mode("+s");
	    if (Mlock_On().Contains("i"))
		mode << "+i";
	    if (!Mlock_Key().empty())
		mode << "+k " + Mlock_Key();
	    Parent->chanserv.GetLive(i_Name).SendMode(mode);
	}
    }
}


void Chan_Stored_t::Kick(const mstring& nick, const mstring& kicker)
{
    FT("Chan_Stored_t::Kick", (nick, kicker));

    // Users shouldnt kick us, but we just rejoin!
    if (Parent->nickserv.IsLive(nick) &&
	Parent->nickserv.GetLive(nick).IsServices())
    {
	if (Join())
	    Parent->server.JOIN(nick, i_Name);
	return;
    }

    if (DoRevenge("KICK", kicker, nick))
	Parent->server.INVITE(Parent->chanserv.FirstName(),
		nick, i_Name);
}

void Chan_Stored_t::ChgNick(const mstring& nick, const mstring& newnick)
{
    FT("Chan_Stored_t::ChgNick", (nick, newnick));

    if (!Parent->chanserv.IsLive(i_Name))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", (
			"NICK", nick, i_Name));
	return;
    }

    if (!Parent->nickserv.IsLiveAll(nick))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONUSER", (
			"NICK", i_Name, nick));
	return;
    }

    size_t users = 0;
    if (Parent->chanserv.IsLive(i_Name))
	users = Parent->chanserv.GetLive(i_Name).Users();

    // Check we dont now trigger AKICK
    // We supply the OLD nick to check the mask, and then the
    // new nick to check nick only (livelook is off).
    { MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    if (Akick_find(nick) || Akick_find(newnick, false))
    {
	if (Parent->chanserv.IsLive(i_Name))
	{
	    RLOCK(("ChanServ", "live", i_Name));
	    Chan_Live_t &cptr = Parent->chanserv.GetLive(i_Name);

	    // If this user is the only user in channel
	    if (users == 1)
		cptr.LockDown();

	    if (Parent->nickserv.IsLive(Akick->Entry()))
		cptr.SendMode("+b " +
			Parent->nickserv.GetLive(nick).AltMask(Parent->operserv.Ignore_Method()));
	    else
		cptr.SendMode("+b " + Akick->Entry());
	}

	LOG(LM_DEBUG, "EVENT/AKICK", (
			newnick, i_Name, Akick->Value()));

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), newnick,
		i_Name, Akick->Value());

	return;
    }}

    // Check we're still alowed in here!
/* Taken out as I need to overcome the problem of us not being
 * on the new nick yet (GetAccess requires a full changeover)

    if (Restricted() && !Suspended() && GetAccess(nick) < 1)
    {
	if (Parent->chanserv.IsLive(i_Name))
	{
	    RLOCK(("ChanServ", "live", i_Name));
	    Chan_Live_t &cptr = Parent->chanserv.GetLive(i_Name);

	    // If this user is the only user in channel
	    if (users == 1)
		cptr.LockDown();

	    mstring mask = nlive.AltMask(Parent->operserv.Ignore_Method());
	    if (mask.SubString(0, 1) != "*!")
	    {
		mask.replace(0, mask.find("!")-1, newnick);
	    }
	    cptr.SendMode("+b " + mask);
	}

	LOG(LM_DEBUG, "EVENT/RESTRICTED", (
			newnick, i_Name));

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), newnick,
		i_Name, Parent->getMessage(newnick, "MISC/KICK_RESTRICTED"));

	return;
    }
*/
}

void Chan_Stored_t::Quit(const mstring& nick)
{
    FT("Chan_Stored_t::Quit", (nick));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "failed_passwds"));
    MCB(failed_passwds.size());
    failed_passwds.erase(nick.LowerCase());
    MCE(failed_passwds.size());
}


void Chan_Stored_t::Topic(const mstring& source, const mstring& topic,
	const mstring& setter, const mDateTime& time)
{
    FT("Chan_Stored_t::Topic", (source, topic, setter, time));

    bool burst = false;
    // Still in burst ...
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL && Parent->ircsvchandler->Burst())
	burst = true;
    }

    // Its us re-setting it!
    if (!source.Contains(".") && Parent->nickserv.IsLive(source) &&
	Parent->nickserv.GetLive(source).IsServices())
	return;

    if (!Parent->chanserv.IsLive(i_Name))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", (
			"TOPIC", source, i_Name));
	return;
    }

    if (Suspended())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Comment"));
	Parent->server.TOPIC(Parent->chanserv.FirstName(),
			Parent->chanserv.FirstName(), i_Name, "[" + IRC_Bold +
			Parent->getMessage("VALS/SUSPENDED") + IRC_Off + "] " +
			i_Comment + " [" + IRC_Bold +
			Parent->getMessage("VALS/SUSPENDED") + IRC_Off + "]",
			time - (1.0 / (60.0 * 60.0 * 24.0)));
	return;
    }

    if (Topiclock())
    {
	// Lets handle this later ...
	if (burst)
	    return;

	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic"));
	RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Setter"));
	RLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Set_Time"));
	Parent->server.TOPIC(Parent->chanserv.FirstName(),
			i_Topic_Setter, i_Name, i_Topic,
			time - (1.0 / (60.0 * 60.0 * 24.0)));
    }
    else
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic"));
	WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Setter"));
	WLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Set_Time"));
	MCB(i_Topic);
	CB(1, i_Topic_Setter);
	CB(2, i_Topic_Set_Time);
	i_Topic = topic;
	i_Topic_Setter = setter;
	i_Topic_Set_Time = time;
	CE(1, i_Topic_Setter);
	CE(2, i_Topic_Set_Time);
	MCE(i_Topic);
    }
}


void Chan_Stored_t::SetTopic(const mstring& source, const mstring& setter,
	const mstring& topic)
{
    FT("Chan_Stored_t::SetTopic", (source, setter, topic));

    // Its us re-setting it!
    if (!setter.Contains(".") && Parent->nickserv.IsLive(setter) &&
	Parent->nickserv.GetLive(setter).IsServices())
	return;

    if (!Parent->chanserv.IsLive(i_Name))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", (
			"TOPIC", source, i_Name));
	return;
    }

    if (Suspended())
	return;

    { WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic"));
    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Setter"));
    WLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Set_Time"));
    MCB(i_Topic);
    CB(1, i_Topic_Setter);
    CB(2, i_Topic_Set_Time);
    i_Topic = topic;
    i_Topic_Setter = setter;
    i_Topic_Set_Time = mDateTime::CurrentDateTime();
    CE(1, i_Topic_Setter);
    CE(2, i_Topic_Set_Time);
    MCE(i_Topic);
    }
    Parent->server.TOPIC(source, setter, i_Name, topic,
	Parent->chanserv.GetLive(i_Name).Topic_Set_Time() -
		(1.0 / (60.0 * 60.0 * 24.0)));
}


void Chan_Stored_t::Mode(const mstring& setter, const mstring& mode)
{
    FT("Chan_Stored_t::Mode", (setter, mode));
    // ENFORCE mlock

    if (!Parent->chanserv.IsLive(i_Name))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", (
			"MODE", setter, i_Name));
	return;
    }

    if (Parent->nickserv.IsLive(setter) &&
	Parent->nickserv.GetLive(setter).IsServices())
	    return;

    Chan_Live_t clive = Parent->chanserv.GetLive(i_Name);
    mstring out_mode, out_param;
    mstring change(mode.Before(" "));
    unsigned int fwdargs = 2, i, wc = mode.WordCount(": ");
    bool add = true;
    for (i=0; i<change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else if (Parent->server.proto.ChanModeArg().Contains(change[i]))
	{
	    mstring arg = mode.ExtractWord(fwdargs, ": ");
	    switch(change[i])
	    {
	    case 'o':
		if (fwdargs <= wc)
		{
		if (add)
		{
		    // IF not services, AND (user is AUTODEOP OR (channel is
		    // secure AND (user is not AUTOOP or CMDOP)))
		    if (!(Parent->nickserv.IsLive(arg) &&
			  Parent->nickserv.GetLive(arg).IsServices()) &&
			(Access_value(arg) <= Level_value("AUTODEOP") ||
			(!(GetAccess(arg, "CMDOP") || GetAccess(arg, "AUTOOP")) &&
			Secureops())))
		    {
			out_mode += "-o";
			out_param += " " + arg;
		    }
		}
		else if (!setter.Contains("."))
		{
		    // If user is services or a beneficiary of revenge
		    if ((Parent->nickserv.IsLive(arg) &&
			Parent->nickserv.GetLive(arg).IsServices()) ||
			DoRevenge("DEOP", setter, arg))
		    {
			out_mode += "-o";
			out_param += " " + arg;
		    }
		}

		fwdargs++;
		}
		break;

	    case 'v':
		if (fwdargs <= wc)
		{
		if (add)
		{
		    // IF not services, AND (user is AUTODEOP OR (channel is
		    // secure AND (user is not AUTOVOICE or CMDVOICE)))
		    if (!(Parent->nickserv.IsLive(arg) &&
			  Parent->nickserv.GetLive(arg).IsServices()) &&
			(Access_value(arg) <= Level_value("AUTODEOP") ||
			(!(GetAccess(arg, "CMDVOICE") ||
			  GetAccess(arg, "AUTOVOICE")) &&
			Secureops())))
		    {
			out_mode += "-v";
			out_param += " " + arg;
		    }
		}

		fwdargs++;
		}
		break;

	    case 'b':
		if (fwdargs <= wc)
		{
		if (add && !setter.Contains("."))
		{
		    vector<mstring> tobekicked;
		    bool DidRevenge = false;
		    mstring bantype = "BAN1";
		    unsigned int j;

		    mstring nick = arg.Before("!");
		    mstring user = arg.After("!").Before("@");
		    mstring host = arg.After("!").After("@");

		    if (host.Contains("*") || host.Contains("?"))
			bantype = "BAN4";
		    else
			bantype = "BAN3";

		    for (j=0; bantype != "BAN2" && j<user.size(); j++)
			switch (user[j])
			{
			case '*':
			case '?':
			    break;
			default:
			    bantype = "BAN2";
			    break;
			}

		    for (j=0; bantype != "BAN1" && j<nick.size(); j++)
			switch (nick[j])
			{
			case '*':
			case '?':
			    break;
			default:
			    bantype = "BAN1";
			    break;
			}

		    for (j=0; !DidRevenge && j<clive.Users(); j++)
		    {
			if (Parent->nickserv.IsLive(clive.User(j)) &&
			    (Parent->nickserv.GetLive(clive.User(j)).Mask(Nick_Live_t::N_U_P_H).Matches(arg, true) ||
			    Parent->nickserv.GetLive(clive.User(j)).AltMask(Nick_Live_t::N_U_P_H).Matches(arg, true)))
			{
			    // Only do revenge or kickonban if user
			    // is not exempt from bans (+e).
			    if (!(clive.MatchExempt(Parent->nickserv.GetLive(clive.User(j)).Mask(Nick_Live_t::N_U_P_H)) ||
				clive.MatchExempt(Parent->nickserv.GetLive(clive.User(j)).AltMask(Nick_Live_t::N_U_P_H))))
			    {
				DidRevenge = DoRevenge(bantype, setter, clive.User(j));
				if (DidRevenge)
				{
				    out_mode += "-b";
				    out_param += " " + arg;
				}
				else
				    tobekicked.push_back(clive.User(j));
			    }
			}
		    }
		    if (!DidRevenge && KickOnBan())
		    {
			for (j=0; j<tobekicked.size(); j++)
			{
			    Parent->server.KICK(Parent->chanserv.FirstName(), tobekicked[j], i_Name, Parent->chanserv.DEF_Akick_Reason());
		        }
		    }
		}
		fwdargs++;
		}
		break;

	    case 'k':
		if (fwdargs <= wc)
		{
		{ RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
		RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
		if (add && i_Mlock_Off.Contains("k"))
		{
		    out_mode += "-k";
		    out_param += " " + arg;
		}
		else if (!add && !i_Mlock_Key.empty())
		{
		    out_mode += "+k";
		    out_param += " " + i_Mlock_Key;
		}}

		fwdargs++;
		}
		break;

	    case 'l':
		{ RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
		RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
		if (add ? i_Mlock_Off.Contains("l") : i_Mlock_Limit)
		{
		    if (add)
		    {
			out_mode += "-l";
		    }
		    else
		    {
			out_mode += "+l";
			out_param += " " + i_Mlock_Limit;
		    }
		}}

		if (add && fwdargs <= wc)
		    fwdargs++;
		break;

	    default:
		if (fwdargs <= wc)
		{
		    fwdargs++;
		}
	    }
	}
	else
	{
	    { RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
	    RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
	    if (add && i_Mlock_Off.Contains(change[i]))
		out_mode += "-" + mstring(change[i]);
	    else if (!add && i_Mlock_On.Contains(change[i]))
		out_mode += "+" + mstring(change[i]);
	    }
	}
    }
    if (out_mode.size() && Parent->chanserv.IsLive(i_Name))
	Parent->chanserv.GetLive(i_Name).SendMode(out_mode + out_param);
}


void Chan_Stored_t::defaults()
{
    NFT("Chan_Stored_t::defaults");

    // Dont lock in here, we locked outside ...
    i_Mlock_On.erase();
    i_Mlock_Off.erase();
    i_Mlock_Key.erase();
    l_Mlock_On.erase();
    l_Mlock_Off.erase();
    i_Mlock_Limit = 0;
    
    i_Bantime = Parent->chanserv.DEF_Bantime();
    l_Bantime = false;
    i_Parttime = Parent->chanserv.DEF_Parttime();
    l_Parttime = false;
    i_Keeptopic = Parent->chanserv.DEF_Keeptopic();
    l_Keeptopic = false;
    i_Topiclock = Parent->chanserv.DEF_Topiclock();
    l_Topiclock = false;
    i_Private = Parent->chanserv.DEF_Private();
    l_Private = false;
    i_Secureops = Parent->chanserv.DEF_Secureops();
    l_Secureops = false;
    i_Secure = Parent->chanserv.DEF_Secure();
    l_Secure = false;
    i_NoExpire = Parent->chanserv.DEF_NoExpire();
    l_NoExpire = false;
    i_Anarchy = Parent->chanserv.DEF_Anarchy();
    l_Anarchy = false;
    i_KickOnBan = Parent->chanserv.DEF_KickOnBan();
    l_KickOnBan = false;
    i_Restricted = Parent->chanserv.DEF_Restricted();
    l_Restricted = false;
    i_Join = Parent->chanserv.DEF_Join();
    l_Join = false;
    i_Revenge = Parent->chanserv.DEF_Revenge();
    l_Revenge = false;
    i_Forbidden = false;

    mstring defaulted = Parent->chanserv.DEF_MLock();
    mstring locked = Parent->chanserv.LCK_MLock();
    bool add = true;
    unsigned int i;

    for (i=0; i<defaulted.size(); i++)
    {
	if (defaulted[i] == '+')
	{
	    add = true;
	}
	else if (defaulted[i] == '-')
	{
	    add = false;
	}
	else if (!Parent->server.proto.ChanModeArg().Contains(defaulted[i]))
	{
	    if (add)
	    {
		if (!i_Mlock_On.Contains(defaulted[i]))
		    i_Mlock_On += defaulted[i];
		if (i_Mlock_Off.Contains(defaulted[i]))
		    i_Mlock_Off.Remove(defaulted[i]);
	    }
	    else
	    {
		if (!i_Mlock_Off.Contains(defaulted[i]))
		    i_Mlock_Off += defaulted[i];
		if (i_Mlock_On.Contains(defaulted[i]))
		    i_Mlock_On.Remove(defaulted[i]);
	    }
	}
    }

    add = true;
    for (i=0; i<locked.size(); i++)
    {
	if (locked[i] == '+')
	{
	    add = true;
	}
	else if (locked[i] == '-')
	{
	    add = false;
	}
	else if (!Parent->server.proto.ChanModeArg().Contains(locked[i]))
	{
	    if (add)
	    {
		if (!i_Mlock_On.Contains(locked[i]))
		    i_Mlock_On += locked[i];
		if (i_Mlock_Off.Contains(locked[i]))
		    i_Mlock_Off.Remove(locked[i]);
	    }
	    else
	    {
		if (!i_Mlock_Off.Contains(locked[i]))
		    i_Mlock_Off += locked[i];
		if (i_Mlock_On.Contains(locked[i]))
		    i_Mlock_On.Remove(locked[i]);
	    }
	}
    }

    vector<mstring> levels = Parent->chanserv.LVL();
    for (i=0; i<levels.size(); i++)
    {
	if (Parent->chanserv.LVL(levels[i]) >= Parent->chanserv.Level_Min())
	    i_Level.insert(entlist_val_t<long>(levels[i],
					Parent->chanserv.LVL(levels[i]),
					Parent->chanserv.FirstName()));
    }    
}

bool Chan_Stored_t::DoRevenge(const mstring& i_type, const mstring& target,
	const mstring& source)
{
    FT("Chan_Stored_t::DoRevenge", (i_type, target, source));

    if (!(Parent->chanserv.IsLive(i_Name) &&
	Parent->nickserv.IsLive(source) &&
	Parent->nickserv.IsLive(target)))
	RET(false);

    mstring type(i_type);
    if (GetAccess(source) > GetAccess(target))
    {
	mstring tmp;
	{ RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Revenge"));
	tmp = i_Revenge;
	}
	if (tmp == "REVERSE")
	{
	    // All we need to do now is return 'screw em' (true)
	}
	else if (tmp == "DEOP")
	{
DoRevenge_DeOp:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    Parent->chanserv.GetLive(i_Name).SendMode("-o " + target);
	    SEND(Parent->chanserv.FirstName(), target, "MISC/REVENGE", (
			type, Parent->getLname(source)));
	}
	else if (tmp == "KICK")
	{
DoRevenge_Kick:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    mstring reason = parseMessage(Parent->getMessage(source, "MISC/REVENGE"),
			mVarArray(type, Parent->getLname(source)));
	    Parent->server.KICK(Parent->chanserv.FirstName(),
			target, i_Name, reason);
	}
	else if (tmp == "BAN1")
	{
DoRevenge_Ban1:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    Parent->chanserv.GetLive(i_Name).SendMode("-o+b " + target + " " +
		Parent->nickserv.GetLive(target).AltMask(Nick_Live_t::N));
	    mstring reason = parseMessage(Parent->getMessage(source, "MISC/REVENGE"),
			mVarArray(type, Parent->getLname(source)));
	    Parent->server.KICK(Parent->chanserv.FirstName(),
			target, i_Name, reason);
	}
	else if (tmp == "BAN2")
	{
DoRevenge_Ban2:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    Parent->chanserv.GetLive(i_Name).SendMode("-o+b " + target + " " +
		Parent->nickserv.GetLive(target).AltMask(Nick_Live_t::U_H));
	    mstring reason = parseMessage(Parent->getMessage(source, "MISC/REVENGE"),
			mVarArray(type, Parent->getLname(source)));
	    Parent->server.KICK(Parent->chanserv.FirstName(),
			target, i_Name, reason);
	}
	else if (tmp == "BAN3")
	{
DoRevenge_Ban3:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    Parent->chanserv.GetLive(i_Name).SendMode("-o+b " + target + " " +
		Parent->nickserv.GetLive(target).AltMask(Nick_Live_t::P_H));
	    mstring reason = parseMessage(Parent->getMessage(source, "MISC/REVENGE"),
			mVarArray(type, Parent->getLname(source)));
	    Parent->server.KICK(Parent->chanserv.FirstName(),
			target, i_Name, reason);
	}
	else if (tmp == "BAN4")
	{
DoRevenge_Ban4:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    Parent->chanserv.GetLive(i_Name).SendMode("-o+b " + target + " " +
		Parent->nickserv.GetLive(target).AltMask(Nick_Live_t::H));
	    mstring reason = parseMessage(Parent->getMessage(source, "MISC/REVENGE"),
			mVarArray(type, Parent->getLname(source)));
	    Parent->server.KICK(Parent->chanserv.FirstName(),
			target, i_Name, reason);
	}
	else if (tmp == "MIRROR")
	{
	    if (type == "DEOP")
		goto DoRevenge_DeOp;
	    else if (type == "KICK")
		goto DoRevenge_Kick;
	    else if (type == "BAN1")
		goto DoRevenge_Ban1;
	    else if (type == "BAN2")
		goto DoRevenge_Ban2;
	    else if (type == "BAN3")
		goto DoRevenge_Ban3;
	    else if (type == "BAN4")
		goto DoRevenge_Ban4;
	}
	else
	{
	    // Revenge is OFF or UNKNOWN...
	    RET(false);
	}
	LOG(LM_INFO, "OTHER/REVENGE", (
			target, i_Name, type, source));
	RET(true);
    }

    RET(false);
}

Chan_Stored_t::Chan_Stored_t(const mstring& name, const mstring& founder,
	const mstring& password, const mstring& desc)
	: i_Name(name), i_RegTime(mDateTime::CurrentDateTime()),
	  i_LastUsed(mDateTime::CurrentDateTime()), i_Founder(founder),
	  i_Description(desc)
{
    FT("Chan_Stored_t::Chan_Stored_t", (name, founder, password, desc));

    defaults();
    Password(password);
    DumpE();
}


Chan_Stored_t::Chan_Stored_t(const mstring& name)
	: i_Name(name), i_RegTime(mDateTime::CurrentDateTime()),
	  i_LastUsed(mDateTime::CurrentDateTime())
{
    FT("Chan_Stored_t::Chan_Stored_t", (name));

    defaults();
    i_Mlock_On = "nits";
    i_Forbidden = true;
    DumpE();
}


void Chan_Stored_t::operator=(const Chan_Stored_t &in)
{
    NFT("Chan_Stored_t::operator=");
    i_Name=in.i_Name;
    i_RegTime=in.i_RegTime;
    i_LastUsed=in.i_LastUsed;
    i_Founder=in.i_Founder;
    i_CoFounder=in.i_CoFounder;
    i_Description=in.i_Description;
    i_Password=in.i_Password;
    i_URL=in.i_URL;
    i_Comment=in.i_Comment;

    i_Mlock_On=in.i_Mlock_On;
    l_Mlock_On=in.l_Mlock_On;
    i_Mlock_Off=in.i_Mlock_Off;
    l_Mlock_Off=in.l_Mlock_Off;
    i_Mlock_Key=in.i_Mlock_Key;
    i_Mlock_Limit=in.i_Mlock_Limit;

    i_Bantime=in.i_Bantime;
    l_Bantime=in.l_Bantime;
    i_Parttime=in.i_Parttime;
    l_Parttime=in.l_Parttime;
    i_Keeptopic=in.i_Keeptopic;
    l_Keeptopic=in.l_Keeptopic;
    i_Topiclock=in.i_Topiclock;
    l_Topiclock=in.l_Topiclock;
    i_Private=in.i_Private;
    l_Private=in.l_Private;
    i_Secureops=in.i_Secureops;
    l_Secureops=in.l_Secureops;
    i_Secure=in.i_Secure;
    l_Secure=in.l_Secure;
    i_NoExpire=in.i_NoExpire;
    l_NoExpire=in.l_NoExpire;
    i_Anarchy=in.i_Anarchy;
    l_Anarchy=in.l_Anarchy;
    i_KickOnBan=in.i_KickOnBan;
    l_KickOnBan=in.l_KickOnBan;
    i_Restricted=in.i_Restricted;
    l_Restricted=in.l_Restricted;
    i_Join=in.i_Join;
    l_Join=in.l_Join;
    i_Revenge=in.i_Revenge;
    l_Revenge=in.l_Revenge;
    i_Suspend_By=in.i_Suspend_By;
    i_Suspend_Time=in.i_Suspend_Time;
    i_Forbidden=in.i_Forbidden;

//  entlist_val_cui<long> j;
    set<entlist_val_t<long> >::const_iterator j;
    i_Level.clear();
    for(j=in.i_Level.begin();j!=in.i_Level.end();j++)
	i_Level.insert(*j);

    i_Access.clear();
    for(j=in.i_Access.begin();j!=in.i_Access.end();j++)
	i_Access.insert(*j);

//  entlist_val_cui<mstring> k;
    set<entlist_val_t<mstring> >::const_iterator k;
    i_Akick.clear();
    for(k=in.i_Akick.begin();k!=in.i_Akick.end();k++)
	i_Akick.insert(*k);

    entlist_ci l;
    i_Greet.clear();
    for(l=in.i_Greet.begin();l!=in.i_Greet.end();l++)
	i_Greet.push_back(*l);

    i_Message.clear();
    for(l=in.i_Message.begin();l!=in.i_Message.end();l++)
	i_Message.push_back(*l);

    i_UserDef.clear();
    map<mstring, mstring>::const_iterator i;
    for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
    i_UserDef.insert(*i);
}

mDateTime Chan_Stored_t::LastUsed()
{
    NFT("Chan_Stored_t::LastUsed");

    if (Parent->chanserv.IsLive(i_Name))
    {
	Chan_Live_t clive = Parent->chanserv.GetLive(i_Name);
	for (unsigned int i=0; i<clive.Users(); i++)
	{
	    if (GetAccess(clive.User(i)) > 0)
	    {
		RET(mDateTime::CurrentDateTime());
	    }
	}
    }
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_LastUsed"));
    RET(i_LastUsed);
}

mDateTime Chan_Stored_t::RegTime() const
{
    NFT("Chan_Stored_t::RegTime");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_RegTime"));
    RET(i_RegTime);
}

void Chan_Stored_t::Founder(const mstring& in)
{
    FT("Chan_Stored_t::Founder", (in));

    if (!Parent->nickserv.IsStored(in))
    {
	LOG(LM_WARNING, "ERROR/BADSET", (
		in, "FOUNDER", i_Name));
	return;
    }

    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Founder"));
    MCB(i_Founder);
    { WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_CoFounder"));
    if (i_CoFounder.IsSameAs(in, true))
    {
	CB(1, i_CoFounder);
	i_CoFounder.erase();
	CE(1, i_CoFounder);
    }}

    i_Founder = in;
    MCE(i_Founder);
}


mstring Chan_Stored_t::Founder() const 
{
    NFT("Chan_Stored_t::Founder");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Founder"));
    RET(i_Founder);
}

void Chan_Stored_t::CoFounder(const mstring& in)
{
    FT("Chan_Stored_t::CoFounder", (in));

    if (!Parent->nickserv.IsStored(in))
    {
	LOG(LM_WARNING, "ERROR/BADSET", (
		in, "COFOUNDER", i_Name));
	return;
    }

    { RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Founder"));
    if (i_Founder.IsSameAs(in, true))
	return;
    }

    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_CoFounder"));
    MCB(i_CoFounder);
    i_CoFounder = in;
    MCE(i_CoFounder);
}

mstring Chan_Stored_t::CoFounder() const
{
    NFT("Chan_Stored_t::CoFounder");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_CoFounder"));
    RET(i_CoFounder);
}

void Chan_Stored_t::Description(const mstring& in)
{
    FT("Chan_Stored_t::Description", (in));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Description"));
    MCB(i_Description);
    i_Description = in;
    MCE(i_Description);
}

mstring Chan_Stored_t::Description() const
{
    NFT("Description(mstring in)	{ i_Description = in;");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Description"));
    RET(i_Description);
}

void Chan_Stored_t::Password(const mstring& in)
{
    FT("Chan_Stored_t::Password", (in));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Password"));
    MCB(i_Password);
#ifdef GETPASS
    i_Password = in;
#else
    char newpass[33];
    mHASH(in.c_str(), in.length(), newpass);
    i_Password = newpass;
#endif
    MCE(i_Password);
}

mstring Chan_Stored_t::Password() const
{
    NFT("Chan_Stored_t::Password");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Password"));
    RET(i_Password);
}

void Chan_Stored_t::Email(const mstring& in)
{
    FT("Chan_Stored_t::Email", (in));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Email"));
    MCB(i_Email);
    i_Email = in;
    MCE(i_Email);
}

mstring Chan_Stored_t::Email() const
{
    NFT("Chan_Stored_t::Email");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Email"));
    RET(i_Email);
}

void Chan_Stored_t::URL(const mstring& in)
{
    FT("Chan_Stored_t::URL", (in));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_URL"));
    MCB(i_URL);
    i_URL = in;
    MCE(i_URL);
}

mstring Chan_Stored_t::URL() const
{
    NFT("Chan_Stored_t::URL");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_URL"));
    RET(i_URL);
}

void Chan_Stored_t::Comment(const mstring& in)
{
    FT("Chan_Stored_t::Comment", (in));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Comment"));
    MCB(i_Comment);
    i_Comment = in;
    MCE(i_Comment);
}
	
mstring Chan_Stored_t::Comment() const
{
    NFT("Chan_Stored_t::Comment");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Comment"));
    RET(i_Comment);
}


unsigned int Chan_Stored_t::CheckPass(const mstring& nick, const mstring& password)
{
    FT("Chan_Stored_t::CheckPass", (nick, password));
    unsigned int retval = 0;
#ifdef GETPASS
    mstring check(password);
#else
    char chkpass[33];
    mHASH(password.c_str(), password.length(), chkpass);
    mstring check(chkpass);
#endif
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "failed_passwds"));
    MCB(failed_passwds.size());
    RLOCK_IF(("ChanServ", "stored", i_Name.LowerCase(), "i_Password"),
	i_Password == check)
	failed_passwds.erase(nick.LowerCase());
    else
    {
	if (failed_passwds.find(nick.LowerCase()) == failed_passwds.end())
	    failed_passwds[nick.LowerCase()]=0;
	retval = ++failed_passwds[nick.LowerCase()];
    }
    MCE(failed_passwds.size());
    RET(retval);
}


void Chan_Stored_t::Suspend(const mstring& name)
{
    FT("Chan_Stored_t::Suspend", (name));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_Time"));
    MCB(i_Suspend_By);
    CB(1, i_Suspend_Time);
    i_Suspend_By = name;
    i_Suspend_Time = mDateTime::CurrentDateTime();
    CE(1, i_Suspend_Time);
    MCE(i_Suspend_By);
}


void Chan_Stored_t::UnSuspend()
{
    NFT("Chan_Stored_t::UnSuspend");
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
    MCB(i_Suspend_By);
    i_Suspend_By.erase();
    MCE(i_Suspend_By);
}


mstring Chan_Stored_t::Mlock_Off() const
{
    NFT("Chan_Stored_t::Mlock_Off");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
    RET(i_Mlock_Off);
}

mstring Chan_Stored_t::Mlock_On() const
{
    NFT("Chan_Stored_t::Mlock_On");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
    RET(i_Mlock_On);
}

mstring Chan_Stored_t::Mlock() const
{
    NFT("Chan_Stored_t::Mlock");
    mstring Result;
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
    RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
    RLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
    RLOCK4(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
    if(!i_Mlock_On.empty() || !i_Mlock_Key.empty() || i_Mlock_Limit)
	Result << "+" << i_Mlock_On;
    if(!i_Mlock_Key.empty())
	Result << "k";
    if(i_Mlock_Limit)
	Result << "l";
    if(!i_Mlock_Off.empty())
	Result << "-" << i_Mlock_Off;
    RET(Result);
}


vector<mstring> Chan_Stored_t::Mlock(const mstring& source, const mstring& mode)
{
    FT("Chan_Stored_t::Mlock", (source, mode));

    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
    WLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
    WLOCK4(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
    MCB(i_Mlock_On);
    CB(1, i_Mlock_Off);
    CB(2, i_Mlock_Key);
    CB(3, i_Mlock_Limit);
    i_Mlock_On.erase();
    i_Mlock_Off.erase();
    i_Mlock_Key.erase();
    i_Mlock_Limit = 0;
    vector<mstring> retval;
    mstring output, change(mode.ExtractWord(1, ": "));
    unsigned int i;
    unsigned int fwdargs = 2;
    bool add = true;
    bool ignorek = false;
    bool ignorel = false;

    if (change.WordCount("k") > 2)
    {
	ignorek = true;
	output.erase();
	output = parseMessage(Parent->getMessage(source, "ERR_SYNTAX/MULTI_MODE"),
		    				mVarArray('k'));
	retval.push_back(output);
    }
    if (change.WordCount("l") > 2)
    {
	ignorel = true;
	output.erase();
	output = parseMessage(Parent->getMessage(source, "ERR_SYNTAX/MULTI_MODE"),
		    				mVarArray('l'));
	retval.push_back(output);
    }

    for (i=0; i<change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else if (Parent->server.proto.ChanModeArg().Contains(change[i]))
	{
	    switch (change[i])
	    {
	    case 'k':
		if (!ignorek && add)
		{
		    if (fwdargs > mode.WordCount(": "))
		    {
			output.erase();
			output = Parent->getMessage(source, "ERR_SYNTAX/NOKEY");
			retval.push_back(output);
			fwdargs--;
		    }
		    else
		    {
			i_Mlock_Key = mode.ExtractWord(fwdargs, ": ");
		    }
		    fwdargs++;
		}
		break;

	    case 'l':
		if (!ignorel && add)
		{
		    if (fwdargs > mode.WordCount(": "))
		    {
			output.erase();
			output = Parent->getMessage(source, "ERR_SYNTAX/NOLIMIT");
			retval.push_back(output);
			fwdargs--;
		    }
		    else if (!mode.ExtractWord(fwdargs, ": ").IsNumber() ||
			mode.ExtractWord(fwdargs, ": ").Contains("."))
		    {
			output.erase();
			output = Parent->getMessage(source, "ERR_SYNTAX/WHOLENUMBER");
			retval.push_back(output);
		    }
		    else if (atol(mode.ExtractWord(fwdargs, ": ")) < 1)
		    {
			output.erase();
			output = parseMessage(Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
			    					mVarArray(1, 32768));
			retval.push_back(output);
		    }
		    else
		    {
			i_Mlock_Limit = atol(mode.ExtractWord(fwdargs, ": ").c_str());
		    }
		    fwdargs++;
		}
		break;
	    default:
		fwdargs++;
	    }
	}
    }

    add = true;
    for (i=0; i<change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Parent->server.proto.ChanModeArg().Contains(change[i]))
		{
		    if (!i_Mlock_On.Contains(change[i]))
			i_Mlock_On += change[i];
		    if (i_Mlock_Off.Contains(change[i]))
			i_Mlock_Off.Remove(change[i]);
		}
	    }
	    else
	    {
		// Nothing with arguments unless its k or l
		if (!Parent->server.proto.ChanModeArg().Contains(change[i]) ||
		    ((change[i]=='k' && !ignorek) || (change[i]=='l' && !ignorel)))
		{
		    if (!i_Mlock_Off.Contains(change[i]))
			i_Mlock_Off += change[i];
		    if (i_Mlock_On.Contains(change[i]))
			i_Mlock_On.Remove(change[i]);
		}
	    }
	}
    }

    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_Off"));
    RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_On"));
    mstring locked(Parent->chanserv.LCK_MLock() +
	"+" + l_Mlock_On + "-" + l_Mlock_Off);
    mstring override_on;
    mstring override_off;
    mstring forced_on;
    mstring forced_off;
    add = true;
    for (i=0; i<locked.size(); i++)
    {
	if (locked[i] == '+')
	{
	    add = true;
	}
	else if (locked[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (!i_Mlock_On.Contains(locked[i]))
		    {
			forced_on += locked[i];
			i_Mlock_On += locked[i];
		    }
		    if (i_Mlock_Off.Contains(locked[i]))
		    {
			override_off += locked[i];
			i_Mlock_Off.Remove(locked[i]);
		    }
		}
	    }
	    else
	    {
		if (locked[i] == 'k')
		    i_Mlock_Key.erase();
		else if (locked[i] == 'l')
		    i_Mlock_Limit = 0;

		if (locked[i] == 'k' || locked[i] == 'l' ||
		    !Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (!i_Mlock_Off.Contains(locked[i]))
		    {
			forced_off += locked[i];
			i_Mlock_Off += locked[i];
		    }
		    if (i_Mlock_On.Contains(locked[i]))
		    {
			override_on += locked[i];
			i_Mlock_On.Remove(locked[i]);
		    }
		}
	    }
	}
    }

    mstring output2;
    if (!override_on.empty() || !override_off.empty())
    {	
	if (!output2.empty())
	    output2 += "  ";
	output.erase();
	output = parseMessage(Parent->getMessage(source, "CS_COMMAND/MLOCK_REVERSED"),
	    mVarArray((!override_on.empty() ? ("+" + override_on ) : mstring("")) +
	     (!override_off.empty() ? ("-" + override_off) : mstring(""))));
	output2 += output;
    }
    if (!forced_on.empty() || !forced_off.empty())
    {
	if (!output2.empty())
	    output2 += "  ";
	output.erase();
	output = parseMessage(Parent->getMessage(source, "CS_COMMAND/MLOCK_FORCED"),
	    mVarArray((!forced_on.empty() ? ("+" + forced_on ) : mstring("")) +
	     (!forced_off.empty() ? ("-" + forced_off) : mstring(""))));
	output2 += output;
    }
    if (!output2.empty())
	retval.push_back(output2);
    if (!i_Mlock_On.empty() || !i_Mlock_Key.empty() || i_Mlock_Limit || !i_Mlock_Off.empty())
    {
	mstring modes;
	if (!i_Mlock_On.empty() || !i_Mlock_Key.empty() || i_Mlock_Limit)
	    modes << "+" << i_Mlock_On;
	if (!i_Mlock_Key.empty())
	    modes << "k";
	if (i_Mlock_Limit)
	    modes << "l";
	if (!i_Mlock_Off.empty())
	    modes << "-" << i_Mlock_Off;

	if (Parent->nickserv.IsLive(source))
	{
	    LOG(LM_DEBUG, "CHANSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/MLOCK"), i_Name, modes));
	}

	if (!i_Mlock_Key.empty())
	    modes << " " << i_Mlock_Key;
	if (i_Mlock_Limit)
	    modes << " " << i_Mlock_Limit;

	output.erase();
	output = parseMessage(Parent->getMessage(source, "CS_COMMAND/MLOCK_SET"),
				mVarArray(i_Name, modes));
	retval.push_back(output);

	if (Parent->chanserv.IsLive(i_Name))
	{
	    Chan_Live_t clive = Parent->chanserv.GetLive(i_Name);
	    mstring modes_param;
	    modes = "+";
	    for (i=0; i<i_Mlock_On.size(); i++)
	    {
		if (!clive.HasMode(i_Mlock_On[i]))
		{
		    modes << i_Mlock_On[i];
		}
	    }
	    modes << "-";
	    for (i=0; i<i_Mlock_Off.size(); i++)
	    {
		if (i_Mlock_Off[i] == 'k' && !clive.Key().empty())
		{
		    modes << "k";
		    modes_param << " " << clive.Key();
		}
		else if (clive.HasMode(i_Mlock_Off[i]))
		{
		    modes << i_Mlock_Off[i];
		}
	    }
	    if (!i_Mlock_Key.empty())
	    {
		modes << "+k";
		modes_param << " " << i_Mlock_Key;
	    }
	    if (i_Mlock_Limit)
	    {
		modes << "+l";
		modes_param << " " << i_Mlock_Limit;
	    }
	    if (modes.length() > 2 && Parent->chanserv.IsLive(i_Name))
		Parent->chanserv.GetLive(i_Name).SendMode(modes + modes_param);
	}
    }
    else
    {	
	output.erase();
	output = parseMessage(Parent->getMessage(source, "CS_COMMAND/MLOCK_UNSET"),
				mVarArray(i_Name));
	retval.push_back(output);

	if (Parent->nickserv.IsLive(source))
	{
	    LOG(LM_DEBUG, "CHANSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/MLOCK"), i_Name));
	}
    }
    CE(1, i_Mlock_Off);
    CE(2, i_Mlock_Key);
    CE(3, i_Mlock_Limit);
    MCE(i_Mlock_On);
    NRET(vector<mstring>, retval);
}


mstring Chan_Stored_t::L_Mlock() const
{
    NFT("Chan_Stored_t::L_Mlock");
    mstring Result;
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_Off"));
    RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_On"));
    mstring mode_on(l_Mlock_On);
    mstring mode_off(l_Mlock_Off);
    mstring locked(Parent->chanserv.LCK_MLock());
    bool add = true;
    unsigned int i;
    for (i=0; i<locked.size(); i++)
    {
	if (locked[i] == '+')
	{
	    add = true;
	}
	else if (locked[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (!mode_on.Contains(locked[i]))
			mode_on += locked[i];
		    if (mode_off.Contains(locked[i]))
			mode_off.Remove(locked[i]);
		}
	    }
	    else
	    {
		if (locked[i] == 'k' || locked[i] == 'l' ||
		    !Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (!mode_off.Contains(locked[i]))
			mode_off += locked[i];
		    if (mode_on.Contains(locked[i]))
			mode_on.Remove(locked[i]);
		}
	    }
	}
    }

    if(!mode_on.empty())
	Result+="+" + mode_on;
    if(!mode_off.empty())
	Result+="-" + mode_off;
    RET(Result);
}


vector<mstring> Chan_Stored_t::L_Mlock(const mstring& source, const mstring& mode)
{
    FT("Chan_Stored_t::L_Mlock", (source, mode));

    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_Off"));
    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_On"));
    MCB(l_Mlock_Off);
    CB(1, l_Mlock_On);
    l_Mlock_On.erase();
    l_Mlock_Off.erase();
    vector<mstring> retval;
    mstring output, change(mode.ExtractWord(1, ": "));
    bool add = true;
    unsigned int i;

    add = true;
    for (i=0; i<change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Parent->server.proto.ChanModeArg().Contains(change[i]))
		{
		    if (!l_Mlock_On.Contains(change[i]))
			l_Mlock_On += change[i];
		    if (l_Mlock_Off.Contains(change[i]))
			l_Mlock_Off.Remove(change[i]);
		}
	    }
	    else
	    {
		if (change[i] == 'l' || change[i] == 'k' ||
		    !Parent->server.proto.ChanModeArg().Contains(change[i]))
		{
		    if (!l_Mlock_Off.Contains(change[i]))
			l_Mlock_Off += change[i];
		    if (l_Mlock_On.Contains(change[i]))
			l_Mlock_On.Remove(change[i]);
		}
	    }
	}
    }

    mstring locked(Parent->chanserv.LCK_MLock());
    mstring override_on;
    mstring override_off;
    add = true;
    for (i=0; i<locked.size(); i++)
    {
	if (locked[i] == '+')
	{
	    add = true;
	}
	else if (locked[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (l_Mlock_Off.Contains(locked[i]))
		    {
			override_off += locked[i];
			l_Mlock_Off.Remove(locked[i]);
		    }
		}
	    }
	    else
	    {
		if (locked[i] == 'l' || locked[i] == 'k' ||
		    !Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (l_Mlock_On.Contains(locked[i]))
		    {
			override_on += locked[i];
			l_Mlock_On.Remove(locked[i]);
		    }
		}
	    }
	}
    }

    { WLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
    WLOCK4(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
    CB(2, i_Mlock_Off);
    CB(3, i_Mlock_On);
    // Have to change the REAL mlock
    for (i=0; i<l_Mlock_On.size(); i++)
    {
	if (!i_Mlock_On.Contains(l_Mlock_On[i]))
	{
	    i_Mlock_On += l_Mlock_On[i];
	}
	if (i_Mlock_Off.Contains(l_Mlock_On[i]))
	{
	    i_Mlock_Off.Remove(l_Mlock_On[i]);
	}
    }

    for (i=0; i<l_Mlock_Off.size(); i++)
    {
	if (l_Mlock_Off[i] == 'k')
	{
	    WLOCK5(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
	    i_Mlock_Key.erase();
	}
	else if (l_Mlock_Off[i] == 'l')
	{
	    WLOCK5(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
	    i_Mlock_Limit = 0;
	}
	if (!i_Mlock_Off.Contains(l_Mlock_Off[i]))
	{
	    i_Mlock_Off += l_Mlock_Off[i];
	}
	if (i_Mlock_On.Contains(l_Mlock_Off[i]))
	{
	    i_Mlock_On.Remove(l_Mlock_Off[i]);
	}
    }
    CE(2, i_Mlock_Off);
    CE(3, i_Mlock_Off);
    }

    if (!override_on.empty() || !override_off.empty())
    {
	output.erase();
	output = parseMessage(Parent->getMessage(source, "CS_COMMAND/MLOCK_REVERSED"),
	    mVarArray((!override_on.empty() ? ("+" + override_on ) : mstring("")) +
	     (!override_off.empty() ? ("-" + override_off) : mstring(""))));
	retval.push_back(output);
    }
    if (!l_Mlock_On.empty() || !l_Mlock_Off.empty())
    {
	output.erase();
	output = parseMessage(Parent->getMessage(source, "CS_COMMAND/MLOCK_LOCK"),
	    mVarArray(i_Name.c_str(),
	    ((!l_Mlock_On.empty() ? ("+" + l_Mlock_On )  : mstring("")) +
	     (!l_Mlock_Off.empty() ? ("-" + l_Mlock_Off)  : mstring("")))));
	retval.push_back(output);
	if (Parent->chanserv.IsLive(i_Name))
	    Parent->chanserv.GetLive(i_Name).SendMode(
		"+" + i_Mlock_On + "-" + i_Mlock_Off);

	LOG(LM_DEBUG, "CHANSERV/LOCK", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/MLOCK"), i_Name,
		((!l_Mlock_On.empty() ? ("+" + l_Mlock_On )  : mstring("")) +
		(!l_Mlock_Off.empty() ? ("-" + l_Mlock_Off)  : mstring("")))));
    }
    else
    {
	output.erase();
	output = parseMessage(Parent->getMessage(source, "CS_COMMAND/MLOCK_UNLOCK"),
						mVarArray(i_Name));
	retval.push_back(output);
	LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/MLOCK"), i_Name));
    }
    CE(1, l_Mlock_On);
    MCE(l_Mlock_Off);
    NRET(vector<mstring>, retval);
}

mstring Chan_Stored_t::Mlock_Key() const
{
    NFT("Chan_Stored_t::Mlock_Key");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
    RET(i_Mlock_Key);
}

unsigned int Chan_Stored_t::Mlock_Limit() const
{
    NFT("Chan_Stored_t::Mlock_Limit");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
    RET(i_Mlock_Limit);
}

mstring Chan_Stored_t::Last_Topic() const
{
    NFT("Chan_Stored_t::i_Topic");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic"));
    RET(i_Topic);
}

mstring Chan_Stored_t::Last_Topic_Setter() const
{
    NFT("Chan_Stored_t::i_Topic_Setter");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Setter"));
    RET(i_Topic_Setter);
}

mDateTime Chan_Stored_t::Last_Topic_Set_Time() const
{
    NFT("Chan_Stored_t::i_Topic_Set_Time");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Set_Time"));
    RET(i_Topic_Set_Time);
}

void Chan_Stored_t::Bantime(const unsigned long in)
{
    FT("Chan_Stored_t::Bantime", (in));
    if (!L_Bantime())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Bantime"));
	MCB(i_Bantime);
	i_Bantime = in;
	MCE(i_Bantime);
    }
}


unsigned long Chan_Stored_t::Bantime() const
{
    NFT("Chan_Stored_t::Bantime");
    if (!Parent->chanserv.LCK_Bantime())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Bantime"));
	RET(i_Bantime);
    }
    RET(Parent->chanserv.DEF_Bantime());
}


void Chan_Stored_t::L_Bantime(const bool in)
{
    FT("Chan_Stored_t::L_Bantime", (in));
    if (!Parent->chanserv.LCK_Bantime())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Bantime"));
	MCB(l_Bantime);
	l_Bantime = in;
	MCE(l_Bantime);
    }
}


bool Chan_Stored_t::L_Bantime() const 
{
    NFT("Chan_Stored_t::L_Bantime");
    if (!Parent->chanserv.LCK_Bantime())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Bantime"));
	RET(l_Bantime);
    }
    RET(true);
}


void Chan_Stored_t::Parttime(const unsigned long in)
{
    FT("Chan_Stored_t::Parttime", (in));
    if (!L_Parttime())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Parttime"));
	MCB(i_Parttime);
	i_Parttime = in;
	MCE(i_Parttime);
    }
}


unsigned long Chan_Stored_t::Parttime() const
{
    NFT("Chan_Stored_t::Parttime");
    if (!Parent->chanserv.LCK_Parttime())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Parttime"));
	RET(i_Parttime);
    }
    RET(Parent->chanserv.DEF_Parttime());
}


void Chan_Stored_t::L_Parttime(const bool in)
{
    FT("Chan_Stored_t::L_Parttime", (in));
    if (!Parent->chanserv.LCK_Parttime())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Parttime"));
	MCB(l_Parttime);
	l_Parttime = in;
	MCE(l_Parttime);
    }
}


bool Chan_Stored_t::L_Parttime() const
{
    NFT("Chan_Stored_t::L_Parttime");
    if (!Parent->chanserv.LCK_Parttime())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Parttime"));
	RET(l_Parttime);
    }
    RET(true);
}


void Chan_Stored_t::Keeptopic(const bool in)
{
    FT("Chan_Stored_t::Keeptopic", (in));
    if (!L_Keeptopic())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Keeptopic"));
	MCB(i_Keeptopic);
	i_Keeptopic = in;
	MCE(i_Keeptopic);
    }
}


bool Chan_Stored_t::Keeptopic() const
{
    NFT("Chan_Stored_t::Keeptopic");
    if (!Parent->chanserv.LCK_Keeptopic())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Keeptopic"));
	RET(i_Keeptopic);
    }
    RET(Parent->chanserv.DEF_Keeptopic());
}


void Chan_Stored_t::L_Keeptopic(const bool in)
{
    FT("Chan_Stored_t::L_Keeptopic", (in));
    if (!Parent->chanserv.LCK_Keeptopic())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Keeptopic"));
	MCB(l_Keeptopic);
	l_Keeptopic = in;
	MCE(l_Keeptopic);
    }
}


bool Chan_Stored_t::L_Keeptopic() const
{
    NFT("Chan_Stored_t::L_Keeptopic");
    if (!Parent->chanserv.LCK_Keeptopic())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Keeptopic"));
	RET(l_Keeptopic);
    }
    RET(true);
}


void Chan_Stored_t::Topiclock(const bool in)
{
    FT("Chan_Stored_t::Topiclock", (in));
    if (!L_Topiclock())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topiclock"));
	MCB(i_Topiclock);
	i_Topiclock = in;
	MCE(i_Topiclock);
    }
}


bool Chan_Stored_t::Topiclock() const
{
    NFT("Chan_Stored_t::Topiclock");
    if (!Parent->chanserv.LCK_Topiclock())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topiclock"));
	RET(i_Topiclock);
    }
    RET(Parent->chanserv.DEF_Topiclock());
}


void Chan_Stored_t::L_Topiclock(const bool in)
{
    FT("Chan_Stored_t::L_Topiclock", (in));
    if (!Parent->chanserv.LCK_Topiclock())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Topiclock"));
	MCB(l_Topiclock);
	l_Topiclock = in;
	MCE(l_Topiclock);
    }
}


bool Chan_Stored_t::L_Topiclock() const
{
    NFT("Chan_Stored_t::L_Topiclock");
    if (!Parent->chanserv.LCK_Topiclock())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Topiclock"));
	RET(l_Topiclock);
    }
    RET(true);
}


void Chan_Stored_t::Private(const bool in)
{
    FT("Chan_Stored_t::Private", (in));
    if (!L_Private())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Private"));
	MCB(i_Private);
	i_Private = in;
	MCE(i_Private);
    }
}


bool Chan_Stored_t::Private() const
{
    NFT("Chan_Stored_t::Private");
    if (!Parent->chanserv.LCK_Private())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Private"));
	RET(i_Private);
    }
    RET(Parent->chanserv.DEF_Private());
}


void Chan_Stored_t::L_Private(const bool in)
{
    FT("Chan_Stored_t::L_Private", (in));
    if (!Parent->chanserv.LCK_Private())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Private"));
	MCB(l_Private);
	l_Private = in;
	MCE(l_Private);
    }
}


bool Chan_Stored_t::L_Private() const
{
    NFT("Chan_Stored_t::L_Private");
    if (!Parent->chanserv.LCK_Private())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Private"));
	RET(l_Private);
    }
    RET(true);
}


void Chan_Stored_t::Secureops(const bool in)
{
    FT("Chan_Stored_t::Secureops", (in));
    if (!L_Secureops())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Secureops"));
	MCB(i_Secureops);
	i_Secureops = in;
	MCE(i_Secureops);
    }
}


bool Chan_Stored_t::Secureops() const
{
    NFT("Chan_Stored_t::Secureops");
    if (!Parent->chanserv.LCK_Secureops())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Secureops"));
	RET(i_Secureops);
    }
    RET(Parent->chanserv.DEF_Secureops());
}


void Chan_Stored_t::L_Secureops(const bool in)
{
    FT("Chan_Stored_t::L_Secureops", (in));
    if (!Parent->chanserv.LCK_Secureops())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Secureops"));
	MCB(l_Secureops);
	l_Secureops = in;
	MCE(l_Secureops);
    }
}


bool Chan_Stored_t::L_Secureops() const
{
    NFT("Chan_Stored_t::L_Secureops");
    if (!Parent->chanserv.LCK_Secureops())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Secureops"));
	RET(l_Secureops);
    }
    RET(true);
}


void Chan_Stored_t::Secure(const bool in)
{
    FT("Chan_Stored_t::Secure", (in));
    if (!L_Secure())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Secure"));
	MCB(i_Secure);
	i_Secure = in;
	MCE(i_Secure);
    }
}


bool Chan_Stored_t::Secure() const
{
    NFT("Chan_Stored_t::Secure");
    if (!Parent->chanserv.LCK_Secure())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Secure"));
	RET(i_Secure);
    }
    RET(Parent->chanserv.DEF_Secure());
}


void Chan_Stored_t::L_Secure(const bool in)
{
    FT("Chan_Stored_t::L_Secure", (in));
    if (!Parent->chanserv.LCK_Secure())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Secure"));
	MCB(l_Secure);
	l_Secure = in;
	MCE(l_Secure);
    }
}


bool Chan_Stored_t::L_Secure() const
{
    NFT("Chan_Stored_t::L_Secure");
    if (!Parent->chanserv.LCK_Secure())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Secure"));
	RET(l_Secure);
    }
    RET(true);
}


void Chan_Stored_t::NoExpire(const bool in)
{
    FT("Chan_Stored_t::NoExpire", (in));
    if (!L_NoExpire())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_NoExpire"));
	MCB(i_NoExpire);
	i_NoExpire = in;
	MCE(i_NoExpire);
    }
}


bool Chan_Stored_t::NoExpire() const
{
    NFT("Chan_Stored_t::NoExpire");
    if (!Parent->chanserv.LCK_NoExpire())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_NoExpire"));
	RET(i_NoExpire);
    }
    RET(Parent->chanserv.DEF_NoExpire());
}


void Chan_Stored_t::L_NoExpire(const bool in)
{
    FT("Chan_Stored_t::L_NoExpire", (in));
    if (!Parent->chanserv.LCK_NoExpire())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_NoExpire"));
	MCB(l_NoExpire);
	l_NoExpire = in;
	MCE(l_NoExpire);
    }
}


bool Chan_Stored_t::L_NoExpire() const
{
    NFT("Chan_Stored_t::L_NoExpire");
    if (!Parent->chanserv.LCK_NoExpire())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_NoExpire"));
	RET(l_NoExpire);
    }
    RET(true);
}


void Chan_Stored_t::Anarchy(const bool in)
{
    FT("Chan_Stored_t::Anarchy", (in));
    if (!L_Anarchy())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Anarchy"));
	MCB(i_Anarchy);
	i_Anarchy = in;
	MCE(i_Anarchy);
    }
}


bool Chan_Stored_t::Anarchy() const
{
    NFT("Chan_Stored_t::Anarchy");
    if (!Parent->chanserv.LCK_Anarchy())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Anarchy"));
	RET(i_Anarchy);
    }
    RET(Parent->chanserv.DEF_Anarchy());
}


void Chan_Stored_t::L_Anarchy(const bool in)
{
    FT("Chan_Stored_t::L_Anarchy", (in));
    if (!Parent->chanserv.LCK_Anarchy())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Anarchy"));
	MCB(l_Anarchy);
	l_Anarchy = in;
	MCE(l_Anarchy);
    }
}


bool Chan_Stored_t::L_Anarchy() const
{
    NFT("Chan_Stored_t::L_Anarchy");
    if (!Parent->chanserv.LCK_Anarchy())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Anarchy"));
	RET(l_Anarchy);
    }
    RET(true);
}


void Chan_Stored_t::KickOnBan(const bool in)
{
    FT("Chan_Stored_t::KickOnBan", (in));
    if (!L_KickOnBan())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_KickOnBan"));
	MCB(i_KickOnBan);
	i_KickOnBan = in;
	MCE(i_KickOnBan);
    }
}


bool Chan_Stored_t::KickOnBan() const
{
    NFT("Chan_Stored_t::KickOnBan");
    if (!Parent->chanserv.LCK_KickOnBan())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_KickOnBan"));
	RET(i_KickOnBan);
    }
    RET(Parent->chanserv.DEF_KickOnBan());
}


void Chan_Stored_t::L_KickOnBan(const bool in)
{
    FT("Chan_Stored_t::L_KickOnBan", (in));
    if (!Parent->chanserv.LCK_KickOnBan())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_KickOnBan"));
	MCB(l_KickOnBan);
	l_KickOnBan = in;
	MCE(l_KickOnBan);
    }
}


bool Chan_Stored_t::L_KickOnBan() const
{
    NFT("Chan_Stored_t::L_KickOnBan");
    if (!Parent->chanserv.LCK_KickOnBan())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_KickOnBan"));
	RET(l_KickOnBan);
    }
    RET(true);
}


void Chan_Stored_t::Restricted(const bool in)
{
    FT("Chan_Stored_t::Restricted", (in));
    if (!L_Restricted())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Restricted"));
	MCB(i_Restricted);
	i_Restricted = in;
	MCE(i_Restricted);
    }
}


bool Chan_Stored_t::Restricted() const
{
    NFT("Chan_Stored_t::Restricted");
    if (!Parent->chanserv.LCK_Restricted())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Restricted"));
	RET(i_Restricted);
    }
    RET(Parent->chanserv.DEF_Restricted());
}


void Chan_Stored_t::L_Restricted(const bool in)
{
    FT("Chan_Stored_t::L_Restricted", (in));
    if (!Parent->chanserv.LCK_Restricted())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Restricted"));
	MCB(l_Restricted);
	l_Restricted = in;
	MCE(l_Restricted);
    }
}


bool Chan_Stored_t::L_Restricted() const
{
    NFT("Chan_Stored_t::L_Restricted");
    if (!Parent->chanserv.LCK_Restricted())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Restricted"));
	RET(l_Restricted);
    }
    RET(true);
}


void Chan_Stored_t::Join(const bool in)
{
    FT("Chan_Stored_t::Join", (in));
    if (!L_Join())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Join"));
	MCB(i_Join);
	i_Join = in;
	MCE(i_Join);
    }
}


bool Chan_Stored_t::Join() const
{
    NFT("Chan_Stored_t::Join");
    if (!Parent->chanserv.LCK_Join())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Join"));
	RET(i_Join);
    }
    RET(Parent->chanserv.DEF_Join());
}


void Chan_Stored_t::L_Join(const bool in)
{
    FT("Chan_Stored_t::L_Join", (in));
    if (!Parent->chanserv.LCK_Join())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Join"));
	MCB(l_Join);
	l_Join = in;
	MCE(l_Join);
    }
}


bool Chan_Stored_t::L_Join() const
{
    NFT("Chan_Stored_t::L_Join");
    if (!Parent->chanserv.LCK_Join())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Join"));
	RET(l_Join);
    }
    RET(true);
}


bool Chan_Stored_t::Revenge(const mstring& in)
{
    FT("Chan_Stored_t::Revenge", (in));
    if (!L_Revenge())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Revenge"));
	MCB(i_Revenge);
	i_Revenge = in;
	MCE(i_Revenge);
	RET(true);
    }
    RET(false);
}


mstring Chan_Stored_t::Revenge() const
{
    NFT("Chan_Stored_t::Revenge");
    if (!Parent->chanserv.LCK_Revenge())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Revenge"));
	RET(i_Revenge);
    }
    RET(Parent->chanserv.DEF_Revenge());
}


void Chan_Stored_t::L_Revenge(const bool in)
{
    FT("Chan_Stored_t::L_Revenge", (in));
    if (!Parent->chanserv.LCK_Revenge())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Revenge"));
	MCB(l_Revenge);
	l_Revenge = in;
	MCE(l_Revenge);
    }
}


bool Chan_Stored_t::L_Revenge() const
{
    NFT("Chan_Stored_t::L_Revenge");
    if (!Parent->chanserv.LCK_Revenge())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Revenge"));
	RET(l_Revenge);
    }
    RET(true);
}

bool Chan_Stored_t::Suspended() const
{
    NFT("Chan_Stored_t::Suspended");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
    RET(!i_Suspend_By.empty());
}

mstring Chan_Stored_t::Suspend_By() const
{
    NFT("Chan_Stored_t::Suspend_By");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
    RET(i_Suspend_By);
}

mDateTime Chan_Stored_t::Suspend_Time() const
{
    NFT("Chan_Stored_t::Suspend_Time");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_Time"));
    RET(i_Suspend_Time);
}

bool Chan_Stored_t::Forbidden() const
{
    NFT("Chan_Stored_t::Forbidden");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Forbidden"));
    RET(i_Forbidden);
}


bool Chan_Stored_t::Level_change(const mstring& entry, const long value,
	const mstring& nick)
{
    FT("Chan_Stored_t::Level_change", (entry, value, nick));

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
    if (Level_find(entry))
    {
	const_cast<entlist_val_t<long> *>(&(*Level))->Value(value, nick);
	RET(true);
    }
    else
    {
	Level = i_Level.end();
	RET(false);
    }
}

bool Chan_Stored_t::Level_find(const mstring& entry)
{
    FT("Chan_Stored_t::Level_find", (entry));

    //  entlist_val_ui<long> iter = i_Level.end();
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
    set<entlist_val_t<long> >::iterator iter = i_Level.end();
    if (!i_Level.empty())
	for (iter=i_Level.begin(); iter!=i_Level.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;

    if (iter != i_Level.end())
    {
	Level = iter;
	RET(true);
    }
    else
    {
	Level = i_Level.end();
	RET(false);
    }
}


long Chan_Stored_t::Level_value(const mstring& entry)
{
    FT("Chan_Stored_t::Level_value", (entry));

    long retval = 0;
//  entlist_val_ui<long> iter = Level;
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
    set<entlist_val_t<long> >::iterator iter = Level;

    if (Level_find(entry))
	retval=Level->Value();
    Level = iter;
    RET(retval);
}


bool Chan_Stored_t::Access_insert(const mstring& i_entry, const long value,
	const mstring& nick, const mDateTime& modtime)
{
    FT("Chan_Stored_t::Access_insert", (i_entry, value, nick, modtime));

    // Wildcards but no @
    if ((i_entry.Contains("*") || i_entry.Contains("?")) &&
	!i_entry.Contains("@"))
    {
	    RET(false);
    }

    // ! without @
    if (i_entry.Contains("!") && !i_entry.Contains("@"))
    {
	    RET(false);
    }
    mstring entry(i_entry.LowerCase());

    // Ensure its a stored nick if no @
    // Add *! if its not *!*@*
    if (!entry.Contains("@"))
    {
	if (!Parent->nickserv.IsStored(entry))
	{
	    RET(false);
	}
	else
	{
	    entry = Parent->nickserv.GetStored(entry).Host();
	    if (entry.empty())
		entry = i_entry.LowerCase();
	}
    }
    else
    {
	if (!entry.Contains("!"))
	    entry.prepend("*!");
    }

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
    if (!Access_find(entry))
    {
	pair<set<entlist_val_t<long> >::iterator, bool> tmp;
	MCB(i_Access.size());
	tmp = i_Access.insert(entlist_val_t<long>(
				entry, value, nick, modtime));
	MCE(i_Access.size());
	if (tmp.second)
	    Access = tmp.first;
	else
	    Access = i_Access.end();
	RET(true);
    }
    else
    {
	Access = i_Access.end();
	RET(false);
    }
}


bool Chan_Stored_t::Access_erase()
{
    NFT("Chan_Stored_t::Access_erase");

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
    if (Access != i_Access.end())
    {
	MCB(i_Access.size());
	i_Access.erase(Access);
	MCE(i_Access.size());
	Access = i_Access.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


bool Chan_Stored_t::Access_find(const mstring& entry, const bool livelook)
{
    FT("Chan_Stored_t::Access_find", (entry, livelook));

//  entlist_val_ui<long> iter = i_Access.end();
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
    set<entlist_val_t<long> >::iterator iter = i_Access.end();
    if (!i_Access.empty())
    {
	// FIND exact nickname
	for (iter=i_Access.begin(); iter!=i_Access.end(); iter++)
	    if (iter->Entry().IsSameAs(entry, true))
		break;

	// FIND host nickname
	if (iter == i_Access.end() && Parent->nickserv.IsStored(entry))
	{
	    mstring tmp = Parent->nickserv.GetStored(entry).Host();
	    if (!tmp.empty())
		for (iter=i_Access.begin(); iter!=i_Access.end(); iter++)
		    if (iter->Entry().IsSameAs(tmp, true))
			break;
	}

	// Not exact or host, try either match or live lookup
	if (iter == i_Access.end())
	{
	    if (entry.Contains("@"))
	    {
		for (iter=i_Access.begin(); iter!=i_Access.end(); iter++)
		    if (entry.Matches(iter->Entry(), true))
			break;
	    }
	    else
	    {
		if (livelook && Parent->nickserv.IsLive(entry))
		{
		    mstring mask = Parent->nickserv.GetLive(entry).Mask(Nick_Live_t::N_U_P_H);

		    for (iter=i_Access.begin(); iter!=i_Access.end(); iter++)
			if (mask.Matches(iter->Entry(), true))
			    break;
		}
	    }
	}
    }

    if (iter != i_Access.end())
    {
	Access = iter;
	RET(true);
    }
    else
    {
	Access = i_Access.end();
	RET(false);
    }
}


long Chan_Stored_t::Access_value(const mstring& entry, const bool looklive)
{
    FT("Chan_Stored_t::Access_value", (entry));

    long retval = 0;
//  entlist_val_ui<long> iter = Access;
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
    set<entlist_val_t<long> >::iterator iter = Access;

    if (Access_find(entry, looklive))
	retval=Access->Value();
    Access = iter;
    RET(retval);
}


long Chan_Stored_t::GetAccess(const mstring& entry)
{
    FT("Chan_Stored_t::GetAccess", (entry));

    long retval = 0;

    if (!Parent->nickserv.IsLive(entry))
    {
	RET(retval);
    }

    if (Parent->nickserv.GetLive(entry).IsChanIdentified(i_Name) && !Suspended())
    {
	retval = Parent->chanserv.Level_Max() + 1;
	RET(retval);
    }

    mstring realentry;
    if (Parent->nickserv.IsStored(entry) &&
	Parent->nickserv.GetStored(entry).IsOnline())
    {
	realentry = Parent->nickserv.GetStored(entry).Host().LowerCase();
	if (realentry.empty())
	    realentry = entry.LowerCase();
    }
    else
    {
	RET(retval);
    }

    if (Suspended())
    {
	if (Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	    Parent->commserv.GetList(Parent->commserv.SADMIN_Name()).IsOn(realentry))
	{
	    retval = Parent->chanserv.Level_Max() + 1;
	}
	else if (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	    Parent->commserv.GetList(Parent->commserv.SOP_Name()).IsOn(realentry))
	{
	    retval = Level_value("SUPER");
	}
	else if (Parent->commserv.IsList(Parent->commserv.ADMIN_Name()) &&
	    Parent->commserv.GetList(Parent->commserv.ADMIN_Name()).IsOn(realentry))
	{
	    retval = Level_value("AUTOOP");
	}
	else if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	    Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(realentry))
	{
	    retval = Level_value("AUTOVOICE");
	}
    }
    else if (Secure() ? Parent->nickserv.GetLive(entry).IsIdentified() : 1)
    {
	if (i_Founder.LowerCase() == realentry.LowerCase())
	{
	    retval = Parent->chanserv.Level_Max() + 1;
	}
	else
	{
	    retval = Access_value(realentry);
	}
    }
    RET(retval);
}

bool Chan_Stored_t::GetAccess(const mstring& entry, const mstring& type)
{
    FT("Chan_Stored_t::GetAccess", (entry, type));

    bool retval = (GetAccess(entry) >= Level_value(type));
    RET(retval);
}


bool Chan_Stored_t::Akick_insert(const mstring& i_entry, const mstring& value,
	const mstring& nick, const mDateTime& modtime)
{
    FT("Chan_Stored_t::Akick_insert", (i_entry, value, nick, modtime));

    // Wildcards but no @
    if ((i_entry.Contains("*") || i_entry.Contains("?")) &&
	!i_entry.Contains("@"))
    {
	    RET(false);
    }

    // ! without @
    if (i_entry.Contains("!") && !i_entry.Contains("@"))
    {
	    RET(false);
    }
    mstring entry(i_entry.LowerCase());

    // Ensure its a stored nick if no @
    // Add *! if its not *!*@*
    if (!entry.Contains("@"))
    {
	if (!Parent->nickserv.IsStored(entry))
	{
	    RET(false);
	}
	else
	{
	    entry = Parent->nickserv.GetStored(entry).Host();
	    if (entry.empty())
		entry = i_entry.LowerCase();
	}
    }
    else
    {
	if (!entry.Contains("!"))
	    entry.prepend("*!");
    }

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    if (!Akick_find(entry))
    {
	pair<set<entlist_val_t<mstring> >::iterator, bool> tmp;
	MCB(i_Akick.size());
	tmp = i_Akick.insert(entlist_val_t<mstring>(
				entry, value, nick, modtime));
	MCE(i_Akick.size());
	if (tmp.second)
	    Akick = tmp.first;
	else
	    Akick = i_Akick.end();
	RET(true);
    }
    else
    {
	Akick = i_Akick.end();
	RET(false);
    }
}

bool Chan_Stored_t::Akick_insert(const mstring& entry, const mstring& nick,
	const mDateTime& modtime)
{
    FT("Chan_Stored_t::Akick_insert", (entry, nick, modtime));
    MCB(i_Akick.size());
    bool retval = Akick_insert(entry, Parent->chanserv.DEF_Akick_Reason(), nick, modtime);
    MCE(i_Akick.size());
    RET(retval);
}

bool Chan_Stored_t::Akick_erase()
{
    NFT("Chan_Stored_t::Akick_erase");

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    if (Akick != i_Akick.end())
    {
	MCB(i_Akick.size());
	i_Akick.erase(Akick);
	MCE(i_Akick.size());
	Akick = i_Akick.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


bool Chan_Stored_t::Akick_find(const mstring& entry, const bool livelook)
{
    FT("Chan_Stored_t::Akick_find", (entry, livelook));

//  entlist_val_ui<mstring> iter = i_Akick.end();
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    set<entlist_val_t<mstring> >::iterator iter = i_Akick.end();
    if (!i_Akick.empty())
    {
	// FIND exact nickname
	for (iter=i_Akick.begin(); iter!=i_Akick.end(); iter++)
	    if (iter->Entry().IsSameAs(entry, true))
		break;

	// FIND host nickname
	if (iter == i_Akick.end() && Parent->nickserv.IsStored(entry))
	{
	    mstring tmp = Parent->nickserv.GetStored(entry).Host();
	    if (!tmp.empty())
		for (iter=i_Akick.begin(); iter!=i_Akick.end(); iter++)
		    if (iter->Entry().IsSameAs(tmp, true))
			break;
	}

	// Not exact or host, try either match or live lookup
	if (iter == i_Akick.end())
	{
	    if (entry.Contains("@"))
	    {
		for (iter=i_Akick.begin(); iter!=i_Akick.end(); iter++)
		    if (entry.Matches(iter->Entry(), true))
			break;
	    }
	    else if (livelook && Parent->nickserv.IsLive(entry))
	    {
		mstring mask(Parent->nickserv.GetLive(entry).Mask(Nick_Live_t::N_U_P_H));

		for (iter=i_Akick.begin(); iter!=i_Akick.end(); iter++)
		    if (mask.Matches(iter->Entry(), true))
			break;
	    }
	}
    }

    if (iter != i_Akick.end())
    {
	Akick = iter;
	RET(true);
    }
    else
    {
	Akick = i_Akick.end();
	RET(false);
    }
}


mstring Chan_Stored_t::Akick_string(const mstring& entry, const bool looklive)
{
    FT("Chan_Stored_t::Akick_string", (entry));

    mstring retval;
//  entlist_val_ui<mstring> iter = Akick;
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    set<entlist_val_t<mstring> >::iterator iter = Akick;

    if (Akick_find(entry, looklive))
	retval=Akick->Value();
    Akick = iter;
    RET(retval);
}


bool Chan_Stored_t::Greet_insert(const mstring& entry, const mstring& nick,
	const mDateTime& modtime)
{
    FT("Chan_Stored_t::Greet_insert", (entry, nick, modtime));

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
    if (!Greet_find(entry))
    {
	MCB(i_Greet.size());
	i_Greet.push_back(entlist_t(entry, nick, modtime));
	MCE(i_Greet.size());
	Greet = i_Greet.end();
	Greet--;
	RET(true);
    }
    else
    {
	Greet = i_Greet.end();
	RET(false);
    }
}


bool Chan_Stored_t::Greet_erase()
{
    NFT("Chan_Stored_t::Greet_erase");

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
    if (Greet != i_Greet.end())
    {
	MCB(i_Greet.size());
	i_Greet.erase(Greet);
	MCE(i_Greet.size());
	Greet = i_Greet.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


bool Chan_Stored_t::Greet_find(const mstring& nick)
{
    FT("Chan_Stored_t::Greet_find", (nick));

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
    entlist_i iter = i_Greet.end();
    if (!i_Greet.empty())
	for (iter=i_Greet.begin(); iter!=i_Greet.end(); iter++)
	    if (nick.LowerCase() == iter->Last_Modifier().LowerCase())
		break;

    if (iter != i_Greet.end())
    {
	Greet = iter;
	RET(true);
    }
    else
    {
	Greet = i_Greet.end();
	RET(false);
    }
}


bool Chan_Stored_t::Message_insert(const mstring& entry, const mstring& nick)
{
    FT("Chan_Stored_t::Message_insert", (entry, nick));

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    MCB(i_Message.size());
    i_Message.push_back(entlist_t(entry, nick));
    MCE(i_Message.size());
    Message = i_Message.end(); Message--;
    RET(true);
}


bool Chan_Stored_t::Message_erase()
{
    NFT("Chan_Stored_t::Message_erase");

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    if (Message != i_Message.end())
    {
	MCB(i_Message.size());
	i_Message.erase(Message);
	MCE(i_Message.size());
	Message = i_Message.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


bool Chan_Stored_t::Message_find(const unsigned int num)
{
    FT("Chan_Stored_t::Message_find", (num));

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    if (num <= 0 || num > i_Message.size())
    {
	RET(false);
    }

    unsigned int i;
    entlist_i iter = i_Message.end();
    for (iter=i_Message.begin(), i=1; iter!=i_Message.end() && i<num;
							    iter++, i++) ;

    if (iter != i_Message.end())
    {
	Message = iter;
	RET(true);
    }
    else
    {
	Message = i_Message.end();
	RET(false);
    }
}


SXP::Tag Chan_Stored_t::tag_Chan_Stored_t("Chan_Stored_t");
SXP::Tag Chan_Stored_t::tag_Name("Name");
SXP::Tag Chan_Stored_t::tag_RegTime("RegTime");
SXP::Tag Chan_Stored_t::tag_LastUsed("LastUsed");
SXP::Tag Chan_Stored_t::tag_Founder("Founder");
SXP::Tag Chan_Stored_t::tag_CoFounder("CoFounder");
SXP::Tag Chan_Stored_t::tag_Description("Description");
SXP::Tag Chan_Stored_t::tag_Password("Password");
SXP::Tag Chan_Stored_t::tag_Email("Email");
SXP::Tag Chan_Stored_t::tag_URL("URL");
SXP::Tag Chan_Stored_t::tag_Comment("Comment");
SXP::Tag Chan_Stored_t::tag_Topic("Topic");
SXP::Tag Chan_Stored_t::tag_Topic_Setter("Topic_Setter");
SXP::Tag Chan_Stored_t::tag_Topic_Set_Time("Topic_Set_Time");
SXP::Tag Chan_Stored_t::tag_set_Mlock_On("SET_Mlock_On");
SXP::Tag Chan_Stored_t::tag_set_Mlock_Off("SET_Mlock_Off");
SXP::Tag Chan_Stored_t::tag_set_Mlock_Key("SET_Mlock_Key");
SXP::Tag Chan_Stored_t::tag_set_Mlock_Limit("SET_Mlock_Limit");
SXP::Tag Chan_Stored_t::tag_set_Bantime("SET_BanTime");
SXP::Tag Chan_Stored_t::tag_set_Parttime("SET_PartTime");
SXP::Tag Chan_Stored_t::tag_set_KeepTopic("SET_KeepTopic");
SXP::Tag Chan_Stored_t::tag_set_TopicLock("SET_TopicLock");
SXP::Tag Chan_Stored_t::tag_set_Private("SET_Private");
SXP::Tag Chan_Stored_t::tag_set_SecureOps("SET_SecureOps");
SXP::Tag Chan_Stored_t::tag_set_Secure("SET_Secure");
SXP::Tag Chan_Stored_t::tag_set_NoExpire("SET_NoExpire");
SXP::Tag Chan_Stored_t::tag_set_Anarchy("SET_Anarchy");
SXP::Tag Chan_Stored_t::tag_set_KickOnBan("SET_KickOnBan");
SXP::Tag Chan_Stored_t::tag_set_Restricted("SET_Restricted");
SXP::Tag Chan_Stored_t::tag_set_Join("SET_Join");
SXP::Tag Chan_Stored_t::tag_set_Revenge("SET_Revenge");
SXP::Tag Chan_Stored_t::tag_Forbidden("Forbidden");
SXP::Tag Chan_Stored_t::tag_lock_Mlock_On("LOCK_Mlock_On");
SXP::Tag Chan_Stored_t::tag_lock_Mlock_Off("LOCK_Mlock_Off");
SXP::Tag Chan_Stored_t::tag_lock_Bantime("LOCK_BanTime");
SXP::Tag Chan_Stored_t::tag_lock_Parttime("LOCK_PartTime");
SXP::Tag Chan_Stored_t::tag_lock_KeepTopic("LOCK_KeepTopic");
SXP::Tag Chan_Stored_t::tag_lock_TopicLock("LOCK_TopicLock");
SXP::Tag Chan_Stored_t::tag_lock_Private("LOCK_Private");
SXP::Tag Chan_Stored_t::tag_lock_SecureOps("LOCK_SecureOps");
SXP::Tag Chan_Stored_t::tag_lock_Secure("LOCK_Secure");
SXP::Tag Chan_Stored_t::tag_lock_NoExpire("LOCK_NoExpire");
SXP::Tag Chan_Stored_t::tag_lock_Anarchy("LOCK_Anarchy");
SXP::Tag Chan_Stored_t::tag_lock_KickOnBan("LOCK_KickOnBan");
SXP::Tag Chan_Stored_t::tag_lock_Restricted("LOCK_Restricted");
SXP::Tag Chan_Stored_t::tag_lock_Join("LOCK_Join");
SXP::Tag Chan_Stored_t::tag_lock_Revenge("LOCK_Revenge");
SXP::Tag Chan_Stored_t::tag_Suspend_By("Suspend_By");
SXP::Tag Chan_Stored_t::tag_Suspend_Time("Suspend_Time");
SXP::Tag Chan_Stored_t::tag_Level("Level");
SXP::Tag Chan_Stored_t::tag_Access("Access");
SXP::Tag Chan_Stored_t::tag_Akick("Akick");
SXP::Tag Chan_Stored_t::tag_Greet("Greet");
SXP::Tag Chan_Stored_t::tag_Message("Message");
SXP::Tag Chan_Stored_t::tag_UserDef("UserDef");

void Chan_Stored_t::BeginElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    FT("Chan_Stored_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    if( pElement->IsA(tag_Level) )
    {
	entlist_val_t<long> *tmp = new entlist_val_t<long>;
	level_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_Access) )
    {
	entlist_val_t<long> *tmp = new entlist_val_t<long>;
	access_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_Akick) )
    {
	entlist_val_t<mstring> *tmp = new entlist_val_t<mstring>;
	akick_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_Greet) )
    {
	entlist_t *tmp = new entlist_t;
	greet_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_Message) )
    {
	entlist_t *tmp = new entlist_t;
	message_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_UserDef) )
    {
	mstring *tmp = new mstring;
	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void Chan_Stored_t::EndElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    FT("Chan_Stored_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

	if( pElement->IsA(tag_Name) )			pElement->Retrieve(i_Name);
	if( pElement->IsA(tag_RegTime) )		pElement->Retrieve(i_RegTime);
	if( pElement->IsA(tag_LastUsed) )		pElement->Retrieve(i_LastUsed);
	if( pElement->IsA(tag_Founder) )		pElement->Retrieve(i_Founder);
	if( pElement->IsA(tag_CoFounder) )		pElement->Retrieve(i_CoFounder);
	if( pElement->IsA(tag_Description) )		pElement->Retrieve(i_Description);
	if( pElement->IsA(tag_Password) )
	{
#ifdef GETPASS
	    if (atoi(pElement->Attrib("hash")))
	    {
		NLOG(LM_EMERGENCY, "ERROR/WRONG_PASS_TYPE");
	    }
#else
	    // If password was stored clear, but we use one-way, change it.
	    if (!atoi(pElement->Attrib("hash")))
	    {
		mstring clearpass;
		pElement->Retrieve(clearpass);
		char newpass[33];
		mHASH(clearpass.c_str(), clearpass.length(), newpass);
		i_Password = newpass;
	    }
#endif
	    else
		pElement->Retrieve(i_Password);
	}
	if( pElement->IsA(tag_Email) )			pElement->Retrieve(i_Email);
	if( pElement->IsA(tag_URL) )			pElement->Retrieve(i_URL);
	if( pElement->IsA(tag_Comment) )		pElement->Retrieve(i_Comment);
	if( pElement->IsA(tag_Topic) )			pElement->Retrieve(i_Topic);
	if( pElement->IsA(tag_Topic_Setter) )		pElement->Retrieve(i_Topic_Setter);
	if( pElement->IsA(tag_Topic_Set_Time) )		pElement->Retrieve(i_Topic_Set_Time);
	if( pElement->IsA(tag_set_Mlock_On) )		pElement->Retrieve(i_Mlock_On);
	if( pElement->IsA(tag_set_Mlock_Off) )		pElement->Retrieve(i_Mlock_Off);
	if( pElement->IsA(tag_set_Mlock_Key) )		pElement->Retrieve(i_Mlock_Key);
	if( pElement->IsA(tag_set_Mlock_Limit) )	pElement->Retrieve(i_Mlock_Limit);
	if( pElement->IsA(tag_set_Bantime) )		pElement->Retrieve(i_Bantime);
	if( pElement->IsA(tag_set_Parttime) )		pElement->Retrieve(i_Parttime);
	if( pElement->IsA(tag_set_KeepTopic) )		pElement->Retrieve(i_Keeptopic);
	if( pElement->IsA(tag_set_TopicLock) )		pElement->Retrieve(i_Topiclock);
	if( pElement->IsA(tag_set_Private) )		pElement->Retrieve(i_Private);
	if( pElement->IsA(tag_set_SecureOps) )		pElement->Retrieve(i_Secureops);
	if( pElement->IsA(tag_set_Secure) )		pElement->Retrieve(i_Secure);
	if( pElement->IsA(tag_set_NoExpire) )		pElement->Retrieve(i_NoExpire);
	if( pElement->IsA(tag_set_Anarchy) )		pElement->Retrieve(i_Anarchy);
	if( pElement->IsA(tag_set_KickOnBan) )		pElement->Retrieve(i_KickOnBan);
	if( pElement->IsA(tag_set_Restricted) )		pElement->Retrieve(i_Restricted);
	if( pElement->IsA(tag_set_Join) )		pElement->Retrieve(i_Join);
	if( pElement->IsA(tag_set_Revenge) )		pElement->Retrieve(i_Revenge);
	if( pElement->IsA(tag_Forbidden) )		pElement->Retrieve(i_Forbidden);
	if( pElement->IsA(tag_lock_Mlock_On) )		pElement->Retrieve(l_Mlock_On);
	if( pElement->IsA(tag_lock_Mlock_Off) )		pElement->Retrieve(l_Mlock_Off);
	if( pElement->IsA(tag_lock_Bantime) )		pElement->Retrieve(l_Bantime);
	if( pElement->IsA(tag_lock_Parttime) )		pElement->Retrieve(l_Parttime);
	if( pElement->IsA(tag_lock_KeepTopic) )		pElement->Retrieve(l_Keeptopic);
	if( pElement->IsA(tag_lock_TopicLock) )		pElement->Retrieve(l_Topiclock);
	if( pElement->IsA(tag_lock_Private) )		pElement->Retrieve(l_Private);
	if( pElement->IsA(tag_lock_SecureOps) )		pElement->Retrieve(l_Secureops);
	if( pElement->IsA(tag_lock_Secure) )		pElement->Retrieve(l_Secure);
	if( pElement->IsA(tag_lock_NoExpire) )		pElement->Retrieve(l_NoExpire);
	if( pElement->IsA(tag_lock_Anarchy) )		pElement->Retrieve(l_Anarchy);
	if( pElement->IsA(tag_lock_KickOnBan) )		pElement->Retrieve(l_KickOnBan);
	if( pElement->IsA(tag_lock_Restricted) )	pElement->Retrieve(l_Restricted);
	if( pElement->IsA(tag_lock_Join) )		pElement->Retrieve(l_Join);
	if( pElement->IsA(tag_lock_Revenge) )		pElement->Retrieve(l_Revenge);
	if( pElement->IsA(tag_Suspend_By) )		pElement->Retrieve(i_Suspend_By);
	if( pElement->IsA(tag_Suspend_Time) )		pElement->Retrieve(i_Suspend_Time);
}

void Chan_Stored_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    set<entlist_val_t<long> >::iterator j;
    set<entlist_val_t<mstring> >::iterator k;
    entlist_i l;

    //TODO: Add your source code here
	pOut->BeginObject(tag_Chan_Stored_t);

	WLOCK(("ChanServ", "stored", i_Name.LowerCase()));
	pOut->WriteElement(tag_Name, i_Name);
	pOut->WriteElement(tag_RegTime, i_RegTime);
	pOut->WriteElement(tag_LastUsed, i_LastUsed);
	pOut->WriteElement(tag_Founder, i_Founder);
	pOut->WriteElement(tag_CoFounder, i_CoFounder);
	pOut->WriteElement(tag_Description, i_Description);
	{
	    SXP::dict attr;
#ifdef GETPASS
	    attr["hash"] = "0";
#else
	    attr["hash"] = "1";
#endif
	    pOut->WriteElement(tag_Password, i_Password, attr);
	}
	pOut->WriteElement(tag_Email, i_Email);
	pOut->WriteElement(tag_URL, i_URL);
	pOut->WriteElement(tag_Comment, i_Comment);
	pOut->WriteElement(tag_Topic, i_Topic);
	pOut->WriteElement(tag_Topic_Setter, i_Topic_Setter);
	pOut->WriteElement(tag_Topic_Set_Time, i_Topic_Set_Time);
	pOut->WriteElement(tag_set_Mlock_On, i_Mlock_On);
	pOut->WriteElement(tag_set_Mlock_Off, i_Mlock_Off);
	pOut->WriteElement(tag_set_Mlock_Key, i_Mlock_Key);
	pOut->WriteElement(tag_set_Mlock_Limit, i_Mlock_Limit);
	pOut->WriteElement(tag_set_Bantime, i_Bantime);
	pOut->WriteElement(tag_set_Parttime, i_Parttime);
	pOut->WriteElement(tag_set_KeepTopic, i_Keeptopic);
	pOut->WriteElement(tag_set_TopicLock, i_Topiclock);
	pOut->WriteElement(tag_set_Private, i_Private);
	pOut->WriteElement(tag_set_SecureOps, i_Secureops);
	pOut->WriteElement(tag_set_Secure, i_Secure);
	pOut->WriteElement(tag_set_NoExpire, i_NoExpire);
	pOut->WriteElement(tag_set_Anarchy, i_Anarchy);
	pOut->WriteElement(tag_set_KickOnBan, i_KickOnBan);
	pOut->WriteElement(tag_set_Restricted, i_Restricted);
	pOut->WriteElement(tag_set_Join, i_Join);
	pOut->WriteElement(tag_set_Revenge, i_Revenge);
	pOut->WriteElement(tag_Forbidden, i_Forbidden);
	pOut->WriteElement(tag_lock_Mlock_On, l_Mlock_On);
	pOut->WriteElement(tag_lock_Mlock_Off, l_Mlock_Off);
	pOut->WriteElement(tag_lock_Bantime, l_Bantime);
	pOut->WriteElement(tag_lock_Parttime, l_Parttime);
	pOut->WriteElement(tag_lock_KeepTopic, l_Keeptopic);
	pOut->WriteElement(tag_lock_TopicLock, l_Topiclock);
	pOut->WriteElement(tag_lock_Private, l_Private);
	pOut->WriteElement(tag_lock_SecureOps, l_Secureops);
	pOut->WriteElement(tag_lock_Secure, l_Secure);
	pOut->WriteElement(tag_lock_NoExpire, l_NoExpire);
	pOut->WriteElement(tag_lock_Anarchy, l_Anarchy);
	pOut->WriteElement(tag_lock_KickOnBan, l_KickOnBan);
	pOut->WriteElement(tag_lock_Restricted, l_Restricted);
	pOut->WriteElement(tag_lock_Join, l_Join);
	pOut->WriteElement(tag_lock_Revenge, l_Revenge);
	pOut->WriteElement(tag_Suspend_By, i_Suspend_By);
	pOut->WriteElement(tag_Suspend_Time, i_Suspend_Time);

	{MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
	for(j=i_Level.begin(); j!=i_Level.end(); j++)
	{
	    pOut->BeginObject(tag_Level);
	    pOut->WriteSubElement(const_cast<entlist_val_t<long> *>(&(*j)));
	    pOut->EndObject(tag_Level);
	}}

	{MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
	for(j=i_Access.begin(); j!=i_Access.end(); j++)
	{
	    pOut->BeginObject(tag_Access);
	    pOut->WriteSubElement(const_cast<entlist_val_t<long> *>(&(*j)));
	    pOut->EndObject(tag_Access);
	}}

	{MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
	for(k=i_Akick.begin(); k!=i_Akick.end(); k++)
	{
	    pOut->BeginObject(tag_Akick);
	    pOut->WriteSubElement(const_cast<entlist_val_t<mstring> *>(&(*k)));
	    pOut->EndObject(tag_Akick);
	}}

	{MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
	for(l=i_Greet.begin(); l!=i_Greet.end(); l++)
	{
	    pOut->BeginObject(tag_Greet);
	    pOut->WriteSubElement(&(*l));
	    pOut->EndObject(tag_Greet);
	}}

	{MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
	for(l=i_Message.begin(); l!=i_Message.end(); l++)
	{
	    pOut->BeginObject(tag_Message);
	    pOut->WriteSubElement(&(*l));
	    pOut->EndObject(tag_Message);
	}}

        map<mstring,mstring>::const_iterator iter;
        for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
        {
            pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
        }

	pOut->EndObject(tag_Chan_Stored_t);
}


size_t Chan_Stored_t::Usage() const
{
    size_t retval = 0;

    WLOCK(("ChanServ", "stored", i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_RegTime.Internal());
    retval += sizeof(i_LastUsed.Internal());
    retval += i_Founder.capacity();
    retval += i_CoFounder.capacity();
    retval += i_Description.capacity();
    retval += i_Password.capacity();
    retval += i_Email.capacity();
    retval += i_URL.capacity();
    retval += i_Comment.capacity();
    map<mstring, unsigned int>::const_iterator i;
    for (i=failed_passwds.begin(); i!=failed_passwds.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second);
    }

    retval += i_Mlock_On.capacity();
    retval += l_Mlock_On.capacity();
    retval += i_Mlock_Off.capacity();
    retval += l_Mlock_Off.capacity();
    retval += i_Mlock_Key.capacity();
    retval += sizeof(i_Mlock_Limit);
    retval += i_Topic.capacity();
    retval += i_Topic_Setter.capacity();
    retval += sizeof(i_Topic_Set_Time.Internal());
    
    retval += sizeof(i_Bantime);
    retval += sizeof(l_Bantime);
    retval += sizeof(i_Parttime);
    retval += sizeof(l_Parttime);
    retval += sizeof(i_Keeptopic);
    retval += sizeof(l_Keeptopic);
    retval += sizeof(i_Topiclock);
    retval += sizeof(l_Topiclock);
    retval += sizeof(i_Private);
    retval += sizeof(l_Private);
    retval += sizeof(i_Secureops);
    retval += sizeof(l_Secureops);
    retval += sizeof(i_Secure);
    retval += sizeof(l_Secure);
    retval += sizeof(i_NoExpire);
    retval += sizeof(l_NoExpire);
    retval += sizeof(i_Anarchy);
    retval += sizeof(l_Anarchy);
    retval += sizeof(i_KickOnBan);
    retval += sizeof(l_KickOnBan);
    retval += sizeof(i_Restricted);
    retval += sizeof(l_Restricted);
    retval += sizeof(i_Join);
    retval += sizeof(l_Join);
    retval += sizeof(i_Forbidden);
    retval += i_Revenge.capacity();
    retval += sizeof(l_Revenge);

    retval += i_Suspend_By.capacity();
    retval += sizeof(i_Suspend_Time.Internal());

    set<entlist_val_t<long> >::const_iterator j;
    {MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
    for (j=i_Level.begin(); j!=i_Level.end(); j++)
    {
	j->Usage();
    }}
    {MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
    for (j=i_Access.begin(); j!=i_Access.end(); j++)
    {
	j->Usage();
    }}

    set<entlist_val_t<mstring> >::const_iterator k;
    {MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    for (k=i_Akick.begin(); k!=i_Akick.end(); k++)
    {
	k->Usage();
    }}

    list<entlist_t>::const_iterator l;
    {MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
    for (l = i_Greet.begin(); l != i_Greet.end(); l++)
    {
	retval += l->Usage();
    }}
    {MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    for (l = i_Message.begin(); l != i_Message.end(); l++)
    {
	retval += l->Usage();
    }}

    map<mstring,mstring>::const_iterator m;
    for (m=i_UserDef.begin(); m!=i_UserDef.end(); m++)
    {
	retval += m->first.capacity();
	retval += m->second.capacity();
    }

    return retval;
}

void Chan_Stored_t::DumpB() const
{
    MB(0, (i_Name, i_RegTime, i_LastUsed, i_Founder, i_CoFounder,
	i_Description, i_Password, i_Email, i_URL, i_Comment,
	failed_passwds.size(), i_Mlock_On, l_Mlock_On, i_Mlock_Off,
	l_Mlock_Off, i_Mlock_Key));
    MB(16, (i_Mlock_Limit, i_Topic, i_Topic_Setter, i_Topic_Set_Time,
	i_Bantime, l_Bantime, i_Parttime, l_Parttime, i_Keeptopic,
	l_Keeptopic, i_Topiclock, l_Topiclock, i_Private, l_Private,
	i_Secureops, l_Secureops));
    MB(32, (i_Secure, l_Secure, i_NoExpire, l_NoExpire, i_Anarchy,
	l_Anarchy, i_KickOnBan, l_KickOnBan, i_Restricted, l_Restricted,
	i_Join, l_Join, i_Forbidden, i_Revenge, l_Revenge, i_Suspend_By));
    MB(48, (i_Suspend_Time, i_Level.size(), i_Access.size(),
	i_Akick.size(), i_Greet.size(), i_Message.size(), i_UserDef.size()));
}


void Chan_Stored_t::DumpE()const 
{
    ME(0, (i_Name, i_RegTime, i_LastUsed, i_Founder, i_CoFounder,
	i_Description, i_Password, i_Email, i_URL, i_Comment,
	failed_passwds.size(), i_Mlock_On, l_Mlock_On, i_Mlock_Off,
	l_Mlock_Off, i_Mlock_Key));
    ME(16, (i_Mlock_Limit, i_Topic, i_Topic_Setter, i_Topic_Set_Time,
	i_Bantime, l_Bantime, i_Parttime, l_Parttime, i_Keeptopic,
	l_Keeptopic, i_Topiclock, l_Topiclock, i_Private, l_Private,
	i_Secureops, l_Secureops));
    ME(32, (i_Secure, l_Secure, i_NoExpire, l_NoExpire, i_Anarchy,
	l_Anarchy, i_KickOnBan, l_KickOnBan, i_Restricted, l_Restricted,
	i_Join, l_Join, i_Forbidden, i_Revenge, l_Revenge, i_Suspend_By));
    ME(48, (i_Suspend_Time, i_Level.size(), i_Access.size(),
	i_Akick.size(), i_Greet.size(), i_Message.size(), i_UserDef.size()));
}


// --------- end of Chan_Stored_t ---------------------------------

ChanServ::ChanServ()
{
    NFT("ChanServ::ChanServ");
    messages=true;
    Revenge_Levels.insert("NONE");
    Revenge_Levels.insert("REVERSE");
    Revenge_Levels.insert("DEOP");
    Revenge_Levels.insert("KICK");
    Revenge_Levels.insert("BAN1");
    Revenge_Levels.insert("BAN2");
    Revenge_Levels.insert("BAN3");
    Revenge_Levels.insert("BAN4");
    Revenge_Levels.insert("MIRROR");
}

void ChanServ::AddCommands()
{
    NFT("ChanServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *USER*", Parent->commserv.REGD_Name(), ChanServ::do_clear_Users);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *OP*", Parent->commserv.REGD_Name(), ChanServ::do_clear_Ops);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *VOICE*", Parent->commserv.REGD_Name(), ChanServ::do_clear_Voices);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *MODE*", Parent->commserv.REGD_Name(), ChanServ::do_clear_Modes);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *BAN*", Parent->commserv.REGD_Name(), ChanServ::do_clear_Bans);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *ALL*", Parent->commserv.REGD_Name(), ChanServ::do_clear_All);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* H*LP", Parent->commserv.REGD_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* SET*", Parent->commserv.REGD_Name(), ChanServ::do_level_Set);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* RESET*", Parent->commserv.REGD_Name(), ChanServ::do_level_Reset);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* LIST", Parent->commserv.REGD_Name(), ChanServ::do_level_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* VIEW", Parent->commserv.REGD_Name(), ChanServ::do_level_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* H*LP", Parent->commserv.REGD_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* ADD", Parent->commserv.REGD_Name(), ChanServ::do_access_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* DEL*", Parent->commserv.REGD_Name(), ChanServ::do_access_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* REM*", Parent->commserv.REGD_Name(), ChanServ::do_access_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* LIST", Parent->commserv.REGD_Name(), ChanServ::do_access_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* VIEW", Parent->commserv.REGD_Name(), ChanServ::do_access_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* H*LP", Parent->commserv.REGD_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK ADD", Parent->commserv.REGD_Name(), ChanServ::do_akick_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK DEL*", Parent->commserv.REGD_Name(), ChanServ::do_akick_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK REM*", Parent->commserv.REGD_Name(), ChanServ::do_akick_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK LIST", Parent->commserv.REGD_Name(), ChanServ::do_akick_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK VIEW", Parent->commserv.REGD_Name(), ChanServ::do_akick_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK H*LP", Parent->commserv.REGD_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* ADD", Parent->commserv.REGD_Name(), ChanServ::do_greet_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* DEL*", Parent->commserv.REGD_Name(), ChanServ::do_greet_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* REM*", Parent->commserv.REGD_Name(), ChanServ::do_greet_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* LIST", Parent->commserv.REGD_Name(), ChanServ::do_greet_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* VIEW", Parent->commserv.REGD_Name(), ChanServ::do_greet_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* H*LP", Parent->commserv.REGD_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* ADD", Parent->commserv.REGD_Name(), ChanServ::do_message_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* DEL*", Parent->commserv.REGD_Name(), ChanServ::do_message_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* REM*", Parent->commserv.REGD_Name(), ChanServ::do_message_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* LIST", Parent->commserv.REGD_Name(), ChanServ::do_message_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* VIEW", Parent->commserv.REGD_Name(), ChanServ::do_message_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* H*LP", Parent->commserv.REGD_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* FOUND*", Parent->commserv.REGD_Name(), ChanServ::do_set_Founder);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* COFOUND*", Parent->commserv.REGD_Name(), ChanServ::do_set_CoFounder);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* DESC*", Parent->commserv.REGD_Name(), ChanServ::do_set_Description);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* PASS*", Parent->commserv.REGD_Name(), ChanServ::do_set_Password);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* EMAIL", Parent->commserv.REGD_Name(), ChanServ::do_set_Email);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* URL", Parent->commserv.REGD_Name(), ChanServ::do_set_URL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* WWW*", Parent->commserv.REGD_Name(), ChanServ::do_set_URL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* WEB*", Parent->commserv.REGD_Name(), ChanServ::do_set_URL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* COMM*", Parent->commserv.SOP_Name(), ChanServ::do_set_Comment);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* M*LOCK", Parent->commserv.REGD_Name(), ChanServ::do_set_Mlock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* BAN*TIME", Parent->commserv.REGD_Name(), ChanServ::do_set_BanTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* PART*TIME", Parent->commserv.REGD_Name(), ChanServ::do_set_PartTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* KEEP*", Parent->commserv.REGD_Name(), ChanServ::do_set_KeepTopic);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* TOPIC*", Parent->commserv.REGD_Name(), ChanServ::do_set_TopicLock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* PRIV*", Parent->commserv.REGD_Name(), ChanServ::do_set_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* SEC*OP*", Parent->commserv.REGD_Name(), ChanServ::do_set_SecureOps);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* SEC*", Parent->commserv.REGD_Name(), ChanServ::do_set_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* NOEX*", Parent->commserv.SOP_Name(), ChanServ::do_set_NoExpire);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* ANAR*", Parent->commserv.REGD_Name(), ChanServ::do_set_Anarchy);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* KICK*", Parent->commserv.REGD_Name(), ChanServ::do_set_KickOnBan);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* RES*", Parent->commserv.REGD_Name(), ChanServ::do_set_Restricted);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *JOIN*", Parent->commserv.REGD_Name(), ChanServ::do_set_Join);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* REV*", Parent->commserv.REGD_Name(), ChanServ::do_set_Revenge);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* H*LP", Parent->commserv.REGD_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK M*LOCK", Parent->commserv.SOP_Name(), ChanServ::do_lock_Mlock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK BAN*TIME", Parent->commserv.SOP_Name(), ChanServ::do_lock_BanTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK PART*TIME", Parent->commserv.SOP_Name(), ChanServ::do_lock_PartTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK KEEP*", Parent->commserv.SOP_Name(), ChanServ::do_lock_KeepTopic);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK TOPIC*", Parent->commserv.SOP_Name(), ChanServ::do_lock_TopicLock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK PRIV*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK SEC*OP*", Parent->commserv.SOP_Name(), ChanServ::do_lock_SecureOps);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK SEC*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK ANAR*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Anarchy);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK KICK*", Parent->commserv.SOP_Name(), ChanServ::do_lock_KickOnBan);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK RES*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Restricted);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK *JOIN*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Join);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK REV*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Revenge);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK H*LP", Parent->commserv.SOP_Name(), do_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK M*LOCK", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Mlock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK BAN*TIME", Parent->commserv.SOP_Name(), ChanServ::do_unlock_BanTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK PART*TIME", Parent->commserv.SOP_Name(), ChanServ::do_unlock_PartTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK KEEP*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_KeepTopic);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK TOPIC*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_TopicLock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK PRIV*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK SEC*OP*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_SecureOps);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK SEC*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK ANAR*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Anarchy);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK KICK*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_KickOnBan);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK RES*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Restricted);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK *JOIN*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Join);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK REV*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Revenge);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK H*LP", Parent->commserv.SOP_Name(), do_3param);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "H*LP", Parent->commserv.ALL_Name(), ChanServ::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "REG*", Parent->commserv.REGD_Name(), ChanServ::do_Register);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "DROP", Parent->commserv.REGD_Name(), ChanServ::do_Drop);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ID*", Parent->commserv.REGD_Name(), ChanServ::do_Identify);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "INFO*", Parent->commserv.ALL_Name(), ChanServ::do_Info);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LIST", Parent->commserv.ALL_Name(), ChanServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SUSP*", Parent->commserv.SOP_Name(), ChanServ::do_Suspend);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNSUSP*", Parent->commserv.SOP_Name(), ChanServ::do_UnSuspend);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FORB*", Parent->commserv.SOP_Name(), ChanServ::do_Forbid);
#ifdef GETPASS
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GETPASS*", Parent->commserv.SADMIN_Name(), ChanServ::do_Getpass);
#endif
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SETPASS*", Parent->commserv.SOP_Name(), ChanServ::do_Setpass);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "MODE*", Parent->commserv.REGD_Name(), ChanServ::do_Mode);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "OP*", Parent->commserv.REGD_Name(), ChanServ::do_Op);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "D*OP*", Parent->commserv.REGD_Name(), ChanServ::do_DeOp);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "VOIC*", Parent->commserv.REGD_Name(), ChanServ::do_Voice);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "D*VOIC*", Parent->commserv.REGD_Name(), ChanServ::do_DeVoice);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "TOPIC*", Parent->commserv.REGD_Name(), ChanServ::do_Topic);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "KICK*", Parent->commserv.REGD_Name(), ChanServ::do_Kick);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "REM*", Parent->commserv.REGD_Name(), ChanServ::do_AnonKick);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "USER*", Parent->commserv.REGD_Name(), ChanServ::do_Users);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "INV*", Parent->commserv.REGD_Name(), ChanServ::do_Invite);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNBAN*", Parent->commserv.REGD_Name(), ChanServ::do_Unban);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LIVE*", Parent->commserv.SOP_Name(), ChanServ::do_Live);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK *", Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK", Parent->commserv.SOP_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK *", Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK", Parent->commserv.SOP_Name(), do_1_3param);
}

void ChanServ::RemCommands()
{
    NFT("ChanServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.


    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *USER*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *OP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *VOICE*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *MODE*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *BAN*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *ALL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* SET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* RESET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* REM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK REM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* REM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* REM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* FOUND*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* COFOUND*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* DESC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* PASS*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* EMAIL", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* URL", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* WWW*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* WEB*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* COMM*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* M*LOCK", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* BAN*TIME", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* PART*TIME", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* KEEP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* TOPIC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* PRIV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* SEC*OP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* SEC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* NOEX*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* ANAR*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* KICK*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* RES*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* *JOIN*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* REV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* H*LP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK M*LOCK", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK BAN*TIME", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK PART*TIME", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK KEEP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK TOPIC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK SEC*OP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK ANAR*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK KICK*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK RES*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK *JOIN*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK REV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK H*LP", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK M*LOCK", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK BAN*TIME", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK PART*TIME", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK KEEP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK TOPIC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK SEC*OP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK ANAR*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK KICK*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK RES*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK *JOIN*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK REV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK H*LP", Parent->commserv.SOP_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "H*LP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "REG*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "DROP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ID*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "INFO*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LIST", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SUSP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNSUSP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FORB*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GET*PASS*", Parent->commserv.SOP_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "MODE", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "OP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "D*OP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "VOIC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "D*VOIC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "TOPIC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "KICK*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "REM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "USER*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "INV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNBAN*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LIVE*", Parent->commserv.SOP_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK *", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK *", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK", Parent->commserv.SOP_Name());
}

#ifdef MAGICK_HAS_EXCEPTIONS
void ChanServ::AddStored(Chan_Stored_t *in) throw(E_ChanServ_Stored)
#else
void ChanServ::AddStored(Chan_Stored_t *in)
#endif
{
    FT("ChanServ::AddStored", ("(Chan_Stored_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Stored(E_ChanServ_Stored::W_Add, E_ChanServ_Stored::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Chan", "Stored", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Stored(E_ChanServ_Stored::W_Add, E_ChanServ_Stored::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Chan", "Stored", "Add", "Blank"));
	return;
#endif
    }

    WLOCK(("ChanServ", "stored"));
    /* stored[in->Name().LowerCase()] = in; */
    stored[in->Name().LowerCase()] = *in;
}


#ifdef MAGICK_HAS_EXCEPTIONS
Chan_Stored_t &ChanServ::GetStored(const mstring &in) const throw(E_ChanServ_Stored)
#else
Chan_Stored_t &ChanServ::GetStored(const mstring &in) const
#endif
{
    FT("ChanServ::GetStored", (in));

    RLOCK(("ChanServ", "stored", in.LowerCase()));
    ChanServ::stored_t::const_iterator iter = stored.find(in.LowerCase());
    if (iter == stored.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Stored(E_ChanServ_Stored::W_Get, E_ChanServ_Stored::T_NotFound, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Get", "NotFound", in));
	NRET(Chan_Stored_t &, GLOB_Chan_Stored_t);
#endif
    }
    /* if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Stored(E_ChanServ_Stored::W_Get, E_ChanServ_Stored::T_Invalid, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Get", "Invalid", in));
	NRET(Chan_Stored_t &, GLOB_Chan_Stored_t);
#endif
    }
    if (iter->second->Name().empty()) */
    if (iter->second.Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Stored(E_ChanServ_Stored::W_Get, E_ChanServ_Stored::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Get", "Blank", in));
	NRET(Chan_Stored_t &, GLOB_Chan_Stored_t);
#endif
    }

    /* NRET(Chan_Stored_t &, const_cast<Chan_Stored_t &>(*iter->second)); */
    NRET(Chan_Stored_t &, const_cast<Chan_Stored_t &>(iter->second));
}


#ifdef MAGICK_HAS_EXCEPTIONS
void ChanServ::RemStored(const mstring &in) throw(E_ChanServ_Stored)
#else
void ChanServ::RemStored(const mstring &in)
#endif
{
    FT("ChanServ::RemStored", (in));

    WLOCK(("ChanServ", "stored"));
    ChanServ::stored_t::iterator iter = stored.find(in.LowerCase());
    if (iter == stored.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Stored(E_ChanServ_Stored::W_Rem, E_ChanServ_Stored::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Rem", "NotFound", in));
	return;
#endif
    }
    WLOCK2(("ChanServ", "stored", iter->first));
    /* if (iter->second != NULL)
    {
	delete iter->second;
    } */
    stored.erase(iter);
}


bool ChanServ::IsStored(const mstring& in)const
{
    FT("ChanServ::IsStored", (in));
    RLOCK(("ChanServ", "stored"));
    bool retval = stored.find(in.LowerCase())!=stored.end();
    RET(retval);
}

#ifdef MAGICK_HAS_EXCEPTIONS
void ChanServ::AddLive(Chan_Live_t *in) throw(E_ChanServ_Live)
#else
void ChanServ::AddLive(Chan_Live_t *in)
#endif
{
    FT("ChanServ::AddLive", ("(Chan_Live_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Live(E_ChanServ_Live::W_Add, E_ChanServ_Live::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Chan", "Live", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Live(E_ChanServ_Live::W_Add, E_ChanServ_Live::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Chan", "Live", "Add", "Blank"));
	return;
#endif
    }

    WLOCK(("ChanServ", "live"));
    /* live[in->Name().LowerCase()] = in; */
    live[in->Name().LowerCase()] = *in;
}


#ifdef MAGICK_HAS_EXCEPTIONS
Chan_Live_t &ChanServ::GetLive(const mstring &in) const throw(E_ChanServ_Live)
#else
Chan_Live_t &ChanServ::GetLive(const mstring &in) const
#endif
{
    FT("ChanServ::GetLive", (in));

    RLOCK(("ChanServ", "live", in.LowerCase()));
    ChanServ::live_t::const_iterator iter = live.find(in.LowerCase());
    if (iter == live.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Live(E_ChanServ_Live::W_Get, E_ChanServ_Live::T_NotFound, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Get", "NotFound", in)));
	NRET(Chan_Live_t &, GLOB_Chan_Live_t);
#endif
    }
    /* if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Live(E_ChanServ_Live::W_Get, E_ChanServ_Live::T_Invalid, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Get", "Invalid", in));
	NRET(Chan_Live_t &, GLOB_Chan_Live_t);
#endif
    }
    if (iter->second->Name().empty()) */
    if (iter->second.Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Live(E_ChanServ_Live::W_Get, E_ChanServ_Live::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Get", "Blank", in));
	NRET(Chan_Live_t &, GLOB_Chan_Live_t);
#endif
    }

    /* NRET(Chan_Live_t &, const_cast<Chan_Live_t &>(*iter->second)); */
    NRET(Chan_Live_t &, const_cast<Chan_Live_t &>(iter->second));
}


#ifdef MAGICK_HAS_EXCEPTIONS
void ChanServ::RemLive(const mstring &in) throw(E_ChanServ_Live)
#else
void ChanServ::RemLive(const mstring &in)
#endif
{
    FT("ChanServ::RemLive", (in));

    WLOCK(("ChanServ", "live"));
    ChanServ::live_t::iterator iter = live.find(in.LowerCase());
    if (iter == live.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_ChanServ_Live(E_ChanServ_Live::W_Rem, E_ChanServ_Live::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Rem", "NotFound", in));
	return;
#endif
    }
    WLOCK2(("ChanServ", "live", iter->first));
    /* if (iter->second != NULL)
    {
	delete iter->second;
    } */
    live.erase(iter);
}

bool ChanServ::IsLive(const mstring& in)const
{
    FT("ChanServ::IsLive", (in));
    RLOCK(("ChanServ", "live"));
    bool retval = live.find(in.LowerCase())!=live.end();
    RET(retval);
}

void ChanServ::execute(mstring& source, const mstring& msgtype, const mstring& params)
{
    mThread::ReAttach(tt_ChanServ);
    FT("ChanServ::execute", (source, msgtype, params));
    //okay this is the main chanserv command switcher

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

void ChanServ::do_Help(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    mstring HelpTopic = Parent->chanserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector<mstring> help = Parent->getHelp(source, HelpTopic.UpperCase());
					
    unsigned int i;
    for (i=0; i<help.size(); i++)
	::send(mynick, source, help[i]);
}

void ChanServ::do_Register(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Register", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring password  = params.ExtractWord(3, " ");
    mstring desc      = params.After(" ", 3);

    if (Parent->chanserv.IsStored(channel))
    {
	if (Parent->chanserv.GetStored(channel).Forbidden())
	    SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	else
	    SEND(mynick, source, "CS_STATUS/ISSTORED", (channel));
 	return;
    }

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		    channel));
	return;
    }

    if (!Parent->chanserv.GetLive(channel).IsOp(source))
    {
	SEND(mynick, source, "CS_STATUS/NOTOPPED", (
		    channel));
	return;
    }

    if (!Parent->nickserv.GetLive(source).HasMode("o") &&
	Parent->nickserv.GetLive(source).LastChanReg().SecondsSince() <
    		Parent->chanserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Parent->chanserv.Delay() -
		Parent->nickserv.GetLive(source).LastChanReg().SecondsSince(),
		source)));
	return;
    }

    if (password.length() < 5 || password.IsSameAs(channel.After(channel[0u]), true) ||
	password.IsSameAs(channel, true) || password.IsSameAs(source, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
	return;
    }

    mstring founder = Parent->nickserv.GetStored(source).Host();
    if (founder.empty())
	founder = Parent->getSname(source);

    if (Parent->chanserv.Max_Per_Nick() &&
	Parent->nickserv.GetStored(founder).MyChannels() >=
	Parent->chanserv.Max_Per_Nick())
    {
	NSEND(mynick, source, "CS_STATUS/TOOMANY");
	return;
    }

    { RLOCK(("ChanServ", "live", channel.LowerCase()));
    Chan_Live_t &clive = Parent->chanserv.GetLive(channel);
    Parent->nickserv.GetLive(source).SetLastChanReg();
    Chan_Stored_t tmp(channel, founder, password, desc);
    tmp.Topic(clive.Topic(), clive.Topic_Setter(), clive.Topic_Set_Time());
    clive.SendMode(tmp.Mlock());
    Parent->chanserv.AddStored(&tmp);
    }
    Parent->nickserv.GetLive(source).ChanIdentify(channel, password);
    Parent->chanserv.stats.i_Register++;
    SEND(mynick, source, "CS_COMMAND/REGISTERED", (
		channel, founder));
    LOG(LM_INFO, "CHANSERV/REGISTER", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	channel));
}

void ChanServ::do_Drop(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Drop", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (!((Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.GetList(Parent->commserv.SOP_Name()).IsOn(source)) ||
	Parent->nickserv.GetLive(source).IsChanIdentified(channel)))
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_CHAN_IDENT", (
		message, mynick, channel));
	return;
    }

    mstring founder = Parent->chanserv.GetStored(channel).Founder();
    if (Parent->chanserv.GetStored(channel).Join() &&
	Parent->chanserv.IsLive(channel) &&
	Parent->chanserv.GetLive(channel).IsIn(Parent->chanserv.FirstName()))
    {
	Parent->server.PART(Parent->chanserv.FirstName(), channel);
    }
    Parent->chanserv.RemStored(channel);
    Parent->nickserv.GetLive(source).UnChanIdentify(channel);
    Parent->chanserv.stats.i_Drop++;
    SEND(mynick, source, "CS_COMMAND/DROPPED", (
	    channel));
    LOG(LM_INFO, "CHANSERV/DROP", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	channel, founder));
}

void ChanServ::do_Identify(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Identify", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring pass      = params.ExtractWord(3, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.GetStored(channel).Suspended())
    {
	SEND(mynick, source, "CS_STATUS/ISSUSPENDED", (channel));
	return;
    }

    mstring output;
    { RLOCK(("NickServ", "live", source.LowerCase()));
    Nick_Live_t &nlive = Parent->nickserv.GetLive(source);
    bool wasident = nlive.IsChanIdentified(channel);
    output = nlive.ChanIdentify(channel, pass);
    if (!wasident &&
	nlive.IsChanIdentified(channel))
    {
	Parent->chanserv.stats.i_Identify++;
	LOG(LM_INFO, "CHANSERV/IDENTIFY", (
		nlive.Mask(Nick_Live_t::N_U_P_H),
		channel));
    }}
    if (!output.empty())
	::send(mynick, source, output);
}

void ChanServ::do_Info(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Info", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    mstring output;

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

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t chan = Parent->chanserv.GetStored(channel);

    if (chan.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    SEND(mynick, source, "CS_INFO/IS", (
		chan.Name()));
    SEND(mynick, source, "CS_INFO/FOUNDER", (
		chan.Founder()));
    if (!chan.CoFounder().empty())
	SEND(mynick, source, "CS_INFO/COFOUNDER", (
		chan.CoFounder()));
    SEND(mynick, source, "CS_INFO/DESCRIPTION", (
		chan.Description()));

    output.erase();
    if (chan.NoExpire() && Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source))
	output << " (" << Parent->getMessage(source, "CS_INFO/NOEXPIRE") << ")";
    SEND(mynick, source, "CS_INFO/REGISTERED", (
		chan.RegTime().Ago(), output));
    if (!(chan.Private()) || (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source)))
    {
	if (Parent->chanserv.IsLive(channel))
	{
	    Chan_Live_t clive = Parent->chanserv.GetLive(channel);
	    output.erase();
	    if (clive.Ops())
	    {
		output << clive.Ops() << " " << Parent->getMessage(source, "CS_INFO/OPS");
	    }
	    if (clive.Voices())
	    {
		if (!output.empty())
		    output << ", ";
		output << clive.Voices() << " " << Parent->getMessage(source, "CS_INFO/VOICES");
	    }
	    if (clive.Users())
	    {
		if (!output.empty())
		    output << ", ";
		output << clive.Users() << " " << Parent->getMessage(source, "CS_INFO/USERS");
	    }
	    if (clive.Squit())
	    {
		if (!output.empty())
		    output << ", ";
		output << clive.Squit() << " " << Parent->getMessage(source, "CS_INFO/SPLIT_USERS");
	    }
	    SEND(mynick, source, "CS_INFO/INUSEBY", (
		output));
	}
	else
	{
	    SEND(mynick, source, "CS_INFO/LASTUSED", (
		chan.LastUsed().Ago()));
	}
    }
    if (!chan.Email().empty())
	SEND(mynick, source, "CS_INFO/EMAIL", (
		chan.Email()));
    if (!chan.URL().empty())
	SEND(mynick, source, "CS_INFO/URL", (
		chan.URL()));
    if (chan.Suspended())
    {
	SEND(mynick, source, "CS_INFO/SUSPEND", (
		chan.Suspend_Time().Ago(),
		chan.Suspend_By()));
	if (!chan.Comment().empty())
	    SEND(mynick, source, "CS_INFO/SUSPENDFOR", (
		chan.Comment()));
    }
    else if (!chan.Comment().empty() && Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	    Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source))
    {
	SEND(mynick, source, "CS_INFO/COMMENT", (
	    chan.Comment()));
    }
    if (!chan.Last_Topic().empty())
    {
	SEND(mynick, source, "CS_INFO/TOPIC", (
		chan.Last_Topic()));
	SEND(mynick, source, "CS_INFO/TOPICBY", (
		chan.Last_Topic_Set_Time().Ago(),
		chan.Last_Topic_Setter()));
    }
    if (!chan.Mlock().empty())
	SEND(mynick, source, "CS_INFO/MLOCK", (
		chan.Mlock()));
    if (!chan.Revenge().empty())
	SEND(mynick, source, "CS_INFO/REVENGE", (
	    Parent->getMessage(source, "CS_SET/REV_" + chan.Revenge())));
    if (chan.Bantime())
	SEND(mynick, source, "CS_INFO/BANTIME", (
		ToHumanTime(chan.Bantime(), source)));
    if (chan.Parttime())
	SEND(mynick, source, "CS_INFO/PARTTIME", (
		ToHumanTime(chan.Parttime(), source)));

    output.erase();
    if (chan.Keeptopic())
    {
	if (!output.empty())
	    output << ", ";
	if (chan.L_Keeptopic())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/KEEPTOPIC");
	if (chan.L_Keeptopic())
	    output << IRC_Off;
    }

    if (chan.Topiclock())
    {
	if (!output.empty())
	    output << ", ";
	if (chan.L_Topiclock())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/TOPICLOCK");
	if (chan.L_Topiclock())
	    output << IRC_Off;
    }

    if (chan.Private())
    {
	if (!output.empty())
	    output << ", ";
	if (chan.L_Private())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/PRIVATE");
	if (chan.L_Private())
	    output << IRC_Off;
    }

    if (chan.Secureops())
    {
	if (!output.empty())
	    output << ", ";
	if (chan.L_Secureops())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/SECUREOPS");
	if (chan.L_Secureops())
	    output << IRC_Off;
    }

    if (chan.Secure())
    {
	if (!output.empty())
	    output << ", ";
	if (chan.L_Secure())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/SECURE");
	if (chan.L_Secure())
	    output << IRC_Off;
    }

    if (chan.Restricted())
    {
	if (!output.empty())
	    output << ", ";
	if (chan.L_Restricted())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/RESTRICTED");
	if (chan.L_Restricted())
	    output << IRC_Off;
    }

    if (chan.Anarchy())
    {
	if (!output.empty())
	    output << ", ";
	if (chan.L_Anarchy())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/ANARCHY");
	if (chan.L_Anarchy())
	    output << IRC_Off;
    }

    if (chan.KickOnBan())
    {
	if (!output.empty())
	    output << ", ";
	if (chan.L_KickOnBan())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/KICKONBAN");
	if (chan.L_KickOnBan())
	    output << IRC_Off;
    }

    if (!output.empty())
	SEND(mynick, source, "CS_INFO/OPTIONS", (
	    output));
    { RLOCK(("Events"));
    if (Parent->servmsg.ShowSync() && Parent->events != NULL)
	SEND(mynick, source, "MISC/SYNC", (
			Parent->events->SyncTime(source)));
    }
}

void ChanServ::do_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_List", (mynick, source, params));

    unsigned int listsize, i;
    int count;
    mstring mask;

    mstring message = params.Before(" ").UpperCase();
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
	    SEND(mynick, source, "LIST/MAXLIST", (
		    Parent->config.Maxlist()));
	    return;
	}
    }

    SEND(mynick, source, "LIST/CHAN_LIST", (
					mask));
    ChanServ::stored_t::iterator iter;

    bool isoper = (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.GetList(Parent->commserv.OPER_Name()).IsOn(source));
    bool issop = (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
		Parent->commserv.GetList(Parent->commserv.SOP_Name()).IsOn(source));
    { RLOCK(("ChanServ", "stored"));
    for (iter = Parent->chanserv.StoredBegin(), i=0, count = 0;
			iter != Parent->chanserv.StoredEnd(); iter++)
    {
	RLOCK2(("ChanServ", "stored", iter->first));
	if (iter->second.Name().Matches(mask, true))
	{
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
		    ::send(mynick, source, iter->second.Name() + "  " +
			Parent->getMessage(source, "VALS/FORBIDDEN"));
		}
		else
		{
		    ::send(mynick, source, iter->second.Name() + "  " +
					    iter->second.Description());
		}
		i++;
	    }
	    count++;
	}
    }}
    SEND(mynick, source, "LIST/DISPLAYED", (
							i, count));
}

void ChanServ::do_Suspend(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Suspend", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring reason    = params.After(" ", 2);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    Parent->chanserv.GetStored(channel).Suspend(source, reason);
    Parent->chanserv.stats.i_Suspend++;
    SEND(mynick, source, "CS_COMMAND/SUSPENDED", (
	    channel));
    LOG(LM_NOTICE, "CHANSERV/SUSPEND", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	channel, reason));
    if (Parent->chanserv.IsLive(channel))
    {
	Parent->server.TOPIC(mynick, mynick, channel, "[" + IRC_Bold +
			Parent->getMessage("VALS/SUSPENDED") +
			IRC_Off + "] " + reason + " [" + IRC_Bold +
			Parent->getMessage("VALS/SUSPENDED") + IRC_Off + "]",
			Parent->chanserv.GetLive(channel).Topic_Set_Time() -
				(1.0 / (60.0 * 60.0 * 24.0)));

	RLOCK(("ChanServ", "live", channel.LowerCase()));
	Chan_Live_t &clive = Parent->chanserv.GetLive(channel);
	Chan_Stored_t cstored = Parent->chanserv.GetStored(channel);
	clive.SendMode("-" + clive.Mode() + " " + clive.Key());
	if (!cstored.Mlock().empty())
	    clive.SendMode(cstored.Mlock() + " " + cstored.Mlock_Key() + " " +
		cstored.Mlock_Limit());

	for (unsigned int i=0; i < clive.Users(); i++)
	{
	    if (clive.IsOp(clive.User(i)) &&
		!(cstored.GetAccess(clive.User(i), "AUTOOP") ||
		cstored.GetAccess(clive.User(i), "CMDOP")))
	    {
		clive.SendMode("-o " + clive.User(i));
	    }
	    if (clive.IsVoice(clive.User(i)) &&
		!(cstored.GetAccess(clive.User(i), "AUTOVOICE") ||
		cstored.GetAccess(clive.User(i), "CMDVOICE")))
	    {
		clive.SendMode("-v " + clive.User(i));
	    }
	}
    }
}

void ChanServ::do_UnSuspend(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_UnSuspend", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.IsLive(channel))
	Parent->server.TOPIC(mynick, mynick, channel, "",
			Parent->chanserv.GetLive(channel).Topic_Set_Time() -
				(1.0 / (60.0 * 60.0 * 24.0)));

    Parent->chanserv.GetStored(channel).UnSuspend();
    Parent->chanserv.stats.i_Unsuspend++;
    SEND(mynick, source, "CS_COMMAND/UNSUSPENDED", (
	    channel));
    LOG(LM_NOTICE, "CHANSERV/UNSUSPEND", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	channel));
}

void ChanServ::do_Forbid(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Forbid", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISSTORED", (
		channel));
	return;
    }

    Chan_Stored_t tmp(channel);
    Parent->chanserv.AddStored(&tmp);
    Parent->chanserv.stats.i_Forbid++;
    SEND(mynick, source, "CS_COMMAND/FORBIDDEN", (
	    channel));
    LOG(LM_NOTICE, "CHANSERV/FORBID", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	channel));

    if (Parent->chanserv.IsLive(channel))
    {
	unsigned int i;
	vector<mstring> kickees;
	Chan_Live_t clive = Parent->chanserv.GetLive(channel);
	for (i=0; i<clive.Users(); i++)
	{
		kickees.push_back(clive.User(i));
	}
	for (i=0; i<kickees.size(); i++)
	{
	    mstring reason = parseMessage(Parent->getMessage(kickees[i], "CS_STATUS/ISFORBIDDEN"),
							mVarArray(channel));
	    Parent->server.KICK(Parent->chanserv.FirstName(), kickees[i],
		channel, reason);
	}
    }
}

#ifdef GETPASS
void ChanServ::do_Getpass(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Getpass", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t chan = Parent->chanserv.GetStored(channel);
    channel = chan.Name();
    if (chan.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!Parent->nickserv.IsStored(chan.Founder()))
    {
	LOG(LM_CRITICAL, "ERROR/FOUNDER_NOTREGD", (
			chan.Name(), chan.Founder()));
	Parent->chanserv.RemStored(channel);
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Parent->chanserv.stats.i_Getpass++;
    SEND(mynick, source, "CS_COMMAND/GETPASS", (
			chan.Name(),
			Parent->getSname(chan.Founder()),
			chan.Password()));
    ANNOUNCE(mynick, "MISC/CHAN_GETPASS", (
			source, chan.Name(),
			Parent->getSname(chan.Founder())));
    LOG(LM_NOTICE, "CHANSERV/GETPASS", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	chan.Name(), Parent->getSname(chan.Founder())));
}

#endif /* GETPASS */

void ChanServ::do_Setpass(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Setpass", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring password  = params.ExtractWord(3, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (password.length() < 5 || password.IsSameAs(channel.After(channel[0u]), true) ||
	password.IsSameAs(channel, true) || password.IsSameAs(source, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &chan = Parent->chanserv.GetStored(channel);
    channel = chan.Name();
    if (chan.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!Parent->nickserv.IsStored(chan.Founder()))
    {
	LOG(LM_CRITICAL, "ERROR/FOUNDER_NOTREGD", (
			chan.Name(), chan.Founder()));
	Parent->chanserv.RemStored(channel);
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Parent->chanserv.stats.i_Getpass++;
    chan.Password(password);
    SEND(mynick, source, "CS_COMMAND/SETPASS", (
			chan.Name(),
			Parent->getSname(chan.Founder()),
			password));
    ANNOUNCE(mynick, "MISC/CHAN_SETPASS", (
			source, chan.Name(),
			Parent->getSname(chan.Founder())));
    LOG(LM_NOTICE, "CHANSERV/SETPASS", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	chan.Name(), Parent->getSname(chan.Founder())));
}

void ChanServ::do_Mode(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Mode", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(Parent->chanserv.GetStored(channel).GetAccess(source, "CMDMODE") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Mode()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Mode()).IsOn(source))))
    {
	mstring modes = params.After(" ", 2);
	Parent->chanserv.GetLive(channel).SendMode(modes);
	Parent->chanserv.stats.i_Mode++;
	LOG(LM_DEBUG, "CHANSERV/MODE", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		modes, channel));
	return;
    }
    else if (Parent->chanserv.GetStored(channel).GetAccess(source, "VIEW") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source)))
    {
	mstring output;
	{ RLOCK(("ChanServ", "live", channel.LowerCase()));
	Chan_Live_t &clive = Parent->chanserv.GetLive(channel);
	output << clive.Name() << ": +" << clive.Mode();
	if (!clive.Key().empty())
	    output << "k";
	if (clive.Limit())
	    output << "l";
	if (!clive.Key().empty())
	    output << " " << clive.Key();
	if (clive.Limit())
	    output << " " << clive.Limit();
	}

	Parent->chanserv.stats.i_Mode++;
	::send(mynick, source, output);
	return;
    }

    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
}

void ChanServ::do_Op(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Op", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t chan = Parent->chanserv.GetStored(channel);
    channel = chan.Name();

    if (chan.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(chan.GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Op()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Op()).IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (
		    target));
	    return;
	}
	else if (!Parent->chanserv.GetLive(channel).IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (
		    target, channel));
	    return;
	}
	else if (Parent->chanserv.GetLive(channel).IsOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_OPPED", (
		    target, channel));
	    return;
	}
	else if (chan.Secureops() &&
		!(chan.GetAccess(target, "CMDOP") ||
		 chan.GetAccess(target, "AUTOOP")) &&
		!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Op()) &&
		 Parent->commserv.GetList(Parent->commserv.OVR_CS_Op()).IsOn(source)))
	{
	    SEND(mynick, source, "CS_STATUS/RESTRICT", (
		channel, Parent->getMessage(source, "CS_SET/SECUREOPS")));
	    return;
	}
    }
    else
    {
	if (!Parent->chanserv.GetLive(channel).IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (
		    target, channel));
	    return;
	}
	else if (Parent->chanserv.GetLive(channel).IsOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/OPPED", (
		    channel));
	    return;
	}
	else if (!chan.GetAccess(target, "CMDOP"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }

    Parent->chanserv.stats.i_Op++;
    Parent->chanserv.GetLive(channel).SendMode("+o " + target);
    LOG(LM_DEBUG, "CHANSERV/OP", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, channel));
}

void ChanServ::do_DeOp(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_DeOp", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(Parent->chanserv.GetStored(channel).GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Op()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Op()).IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (
		    target));
	    return;
	}
	else if (!Parent->chanserv.GetLive(channel).IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (
		    target, channel));
	    return;
	}
	else if (!Parent->chanserv.GetLive(channel).IsOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTOPPED", (
		    target, channel));
	    return;
	}
    }
    else
    {
	if (!Parent->chanserv.GetLive(channel).IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (
		    target, channel));
	    return;
	}
	else if (!Parent->chanserv.GetLive(channel).IsOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/NOTOPPED", (
		    channel));
	    return;
	}
    }

    Parent->chanserv.stats.i_Deop++;
    Parent->chanserv.GetLive(channel).SendMode("-o " + target);
    LOG(LM_DEBUG, "CHANSERV/DEOP", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, channel));
}

void ChanServ::do_Voice(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Voice", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t chan = Parent->chanserv.GetStored(channel);
    channel = chan.Name();

    if (chan.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(chan.GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Voice()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Voice()).IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (
		    target));
	    return;
	}
	else if (!Parent->chanserv.GetLive(channel).IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (
		    target, channel));
	    return;
	}
	else if (Parent->chanserv.GetLive(channel).IsVoice(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_VOICED", (
		    target, channel));
	    return;
	}
	else if (chan.Secureops() &&
		!(chan.GetAccess(target, "CMDVOICE") ||
		 chan.GetAccess(target, "AUTOVOICE")) &&
		!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Op()) &&
		 Parent->commserv.GetList(Parent->commserv.OVR_CS_Op()).IsOn(source)))
	{
	    SEND(mynick, source, "CS_STATUS/RESTRICT", (
		    channel, Parent->getMessage(source, "CS_SET/SECUREOPS")));
	    return;
	}
    }
    else
    {
	if (!Parent->chanserv.GetLive(channel).IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (
		    target, channel));
	    return;
	}
	else if (Parent->chanserv.GetLive(channel).IsVoice(target))
	{
	    SEND(mynick, source, "CS_STATUS/VOICED", (
		    channel));
	    return;
	}
	else if (!chan.GetAccess(target, "CMDVOICE"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }

    Parent->chanserv.stats.i_Voice++;
    Parent->chanserv.GetLive(channel).SendMode("+v " + target);
    LOG(LM_DEBUG, "CHANSERV/VOICE", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, channel));
}

void ChanServ::do_DeVoice(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_DeVoice", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(Parent->chanserv.GetStored(channel).GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Voice()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Voice()).IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (
		    target));
	    return;
	}
	else if (!Parent->chanserv.GetLive(channel).IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (
		    target, channel));
	    return;
	}
	else if (!Parent->chanserv.GetLive(channel).IsVoice(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTVOICED", (
		    target, channel));
	    return;
	}
    }
    else
    {
	if (!Parent->chanserv.GetLive(channel).IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (
		    target, channel));
	    return;
	}
	else if (!Parent->chanserv.GetLive(channel).IsVoice(target))
	{
	    SEND(mynick, source, "CS_STATUS/NOTVOICED", (
		    channel));
	    return;
	}
    }

    Parent->chanserv.stats.i_Devoice++;
    Parent->chanserv.GetLive(channel).SendMode("-v " + target);
    LOG(LM_DEBUG, "CHANSERV/DEVOICE", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, channel));
}

void ChanServ::do_Topic(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Topic", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring topic     = params.After(" ", 2);

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &chan = Parent->chanserv.GetStored(channel);
    channel = chan.Name();

    if (chan.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!chan.GetAccess(source, "CMDMODE"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (topic.IsSameAs("none", true))
    {
	topic.erase();
    }

    Parent->chanserv.stats.i_Topic++;
    chan.SetTopic(mynick, source, topic);
    LOG(LM_DEBUG, "CHANSERV/TOPIC", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	channel));
}

void ChanServ::do_Kick(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Kick", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = params.ExtractWord(3, " ");
    mstring reason;
    if (params.WordCount(" ") >= 4)
	reason = params.After(" ", 3);
    else
	reason = Parent->chanserv.DEF_Akick_Reason();

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t chan = Parent->chanserv.GetStored(channel);
    channel = chan.Name();

    if (chan.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!chan.GetAccess(source, "CMDKICK"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (!Parent->nickserv.IsLive(target))
    {
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (
		    target));
	return;
    }

    if (!Parent->chanserv.GetLive(channel).IsIn(target))
    {
	SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (
		    target, channel));
	return;
    }

    if (chan.GetAccess(target) > chan.GetAccess(source))
    {
	SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (
		    target, channel));
	return;
    }

    mstring output = parseMessage(Parent->getMessage(target, "CS_COMMAND/KICK"),
	    				mVarArray(source, reason));
    Parent->chanserv.stats.i_Kick++;
    Parent->server.KICK(mynick, target, channel, output);
    LOG(LM_DEBUG, "CHANSERV/KICK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, channel));
}

void ChanServ::do_AnonKick(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_AnonKick", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = params.ExtractWord(3, " ");
    mstring reason    = params.After(" ", 3);

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t chan = Parent->chanserv.GetStored(channel);
    channel = chan.Name();

    if (chan.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!chan.GetAccess(source, "SUPER"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (!Parent->nickserv.IsLive(target))
    {
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (
		    target));
	return;
    }

    if (!Parent->chanserv.GetLive(channel).IsIn(target))
    {
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (
		    target, channel));
	return;
    }

    if (chan.GetAccess(target) > chan.GetAccess(source))
    {
	SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (
		    target, channel));
	return;
    }

    Parent->chanserv.stats.i_Anonkick++;
    Parent->server.KICK(mynick, target, channel, reason);
    LOG(LM_DEBUG, "CHANSERV/ANONKICK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, channel));
}

void ChanServ::do_Users(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Users", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!Parent->chanserv.GetStored(channel).GetAccess(source, "VIEW"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    Chan_Live_t chan = Parent->chanserv.GetLive(channel);
    channel = chan.Name();
    unsigned int i;
    mstring user, output = channel + ": ";

    for (i=0; i<chan.Users(); i++)
    {
	user = Parent->getLname(chan.User(i));
	if (output.size() + user.length() > Parent->server.proto.MaxLine())
	{
	    ::send(mynick, source, output);
	    output = channel + ": ";
	}
	if (chan.IsOp(user))
	{
	    output << "@";
	}
	else if (chan.IsVoice(user))
	{
	    output << "+";
	}
	output << user << " ";
    }
    if (output.length() > channel.length() + 2)
	::send(mynick, source, output);

    output = channel + " (SPLIT): ";
    for (i=0; i<chan.Squit(); i++)
    {
	user = Parent->getLname(chan.Squit(i));
	if (output.size() + user.length() > Parent->server.proto.MaxLine())
	{
	    ::send(mynick, source, output);
	    output = channel + " (SQUIT): ";
	}
	output << user << " ";
    }
    if (output.length() > channel.length() + 10)
	::send(mynick, source, output);
}

void ChanServ::do_Invite(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Invite", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(Parent->chanserv.GetStored(channel).GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Invite()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Invite()).IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (
		    target));
	    return;
	}
	else if (Parent->chanserv.GetLive(channel).IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_IN", (
		    target, channel));
	    return;
	}
	SEND(mynick, target, "CS_COMMAND/OTH_INVITE", (
		    channel, source));
    }
    else
    {
	if (Parent->chanserv.GetLive(channel).IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/IN", (
		    channel));
	    return;
	}
	else if (!Parent->chanserv.GetStored(channel).GetAccess(target, "CMDINVITE"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
	SEND(mynick, target, "CS_COMMAND/INVITE", (
		channel));
    }

    Parent->chanserv.stats.i_Invite++;
    Parent->server.INVITE(mynick, target, channel);
    LOG(LM_DEBUG, "CHANSERV/INVITE", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, channel));
}

void ChanServ::do_Unban(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Unban", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(cstored.GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Unban()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Unban()).IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (
		    target));
	    return;
	}
    }
    else
    {
	if (!cstored.GetAccess(target, "CMDUNBAN"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }

    bool found = false;
    { RLOCK2(("ChanServ", "live", channel.LowerCase()));
    Chan_Live_t &clive = Parent->chanserv.GetLive(channel);
    Nick_Live_t nlive = Parent->nickserv.GetLive(target);
    unsigned int i;
    for (i=0; i < clive.Bans(); i++)
    {
	if (nlive.Mask(Nick_Live_t::N_U_P_H).Matches(clive.Ban(i), true) ||
	    nlive.AltMask(Nick_Live_t::N_U_P_H).Matches(clive.Ban(i), true))
	{
	    clive.SendMode("-b " + clive.Ban(i));
	    i--;
	    found = true;
	}
    }}
    if (found)
    {
	Parent->chanserv.stats.i_Unban++;
	if (source.IsSameAs(target, true))
	    SEND(mynick, source, "CS_COMMAND/UNBANNED", (
		    channel));
	else
	    SEND(mynick, source, "CS_COMMAND/OTH_UNBANNED", (
		    target, channel));
	LOG(LM_DEBUG, "CHANSERV/UNBAN", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		target, channel));
    }
    else
    {
	if (source.IsSameAs(target, true))
	    SEND(mynick, source, "CS_STATUS/NOTBANNED", (
		    channel));
	else
	    SEND(mynick, source, "CS_STATUS/OTH_NOTBANNED", (
		    target, channel));
    }
}

void ChanServ::do_Live(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_Live", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (message));
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

    SEND(mynick, source, "LIST/CHAN_LIST", (mask));
    ChanServ::live_t::iterator iter;

    { RLOCK(("ChanServ", "live"));
    for (iter = Parent->chanserv.LiveBegin(), i=0, count = 0;
			iter != Parent->chanserv.LiveEnd(); iter++)
    {
	RLOCK2(("ChanServ", "live", iter->first));
	if (iter->second.Name().Matches(mask, true))
	{
	    if (i < listsize)
	    {
		::sendV(mynick, source, "%s (%du %do %dv %ds %db): +%s %s %d",
					iter->second.Name().c_str(),
					iter->second.Users(),
					iter->second.Ops(),
					iter->second.Voices(),
					iter->second.Squit(),
					iter->second.Bans(),
					iter->second.Mode().c_str(),
					iter->second.Key().c_str(),
					iter->second.Limit());
		i++;
	    }
	    count++;
	}
    }}
    SEND(mynick, source, "LIST/DISPLAYED", (
							i, count));
}


void ChanServ::do_clear_Users(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_clear_Users", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!Parent->chanserv.GetStored(channel).GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Clear()).IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    unsigned int i;
    vector<mstring> kickees;
    Chan_Live_t clive = Parent->chanserv.GetLive(channel);
    for (i=0; i<clive.Users(); i++)
    {
	mstring user = clive.User(i);
	if (!user.IsSameAs(source) && Parent->nickserv.IsLive(user) &&
		!Parent->nickserv.GetLive(user).IsServices())
	    kickees.push_back(user);
    }
    for (i=0; i<kickees.size(); i++)
    {
	mstring output = parseMessage(Parent->getMessage(kickees[i], "CS_COMMAND/CLEAR"),
				mVarArray(message, source, channel));
	Parent->server.KICK(mynick, kickees[i], channel, output);
    }
    Parent->chanserv.stats.i_Clear++;
    LOG(LM_INFO, "CHANSERV/COMMAND", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	message, channel));
}

void ChanServ::do_clear_Ops(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_clear_Ops", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!Parent->chanserv.GetStored(channel).GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Clear()).IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    bool allmode = false;
    if (message.After(" ").Matches("*ALL*", true))
	allmode = true;

    Chan_Live_t clt = Parent->chanserv.GetLive(channel);
    vector<mstring> deop;
    unsigned int i;
    for (i=0; i<clt.Ops(); i++)
    {
	if (!Parent->nickserv.IsLive(clt.Op(i)) ||
		Parent->nickserv.GetLive(clt.Op(i)).IsServices())
	    continue;
	deop.push_back(clt.Op(i));
	if (!allmode)
	    SEND(mynick, clt.Op(i), "CS_COMMAND/CLEAR", (
		    message, source, channel));
    }

    { RLOCK(("ChanServ", "live", channel.LowerCase()));
    Chan_Live_t &clive = Parent->chanserv.GetLive(channel);
    for (i=0; i<deop.size(); i++)
    {
	clive.SendMode("-o " + deop[i]);
    }}
    if (!allmode)
    {
	Parent->chanserv.stats.i_Clear++;
	LOG(LM_INFO, "CHANSERV/COMMAND", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		message, channel));
    }
}

void ChanServ::do_clear_Voices(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_clear_Voices", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!Parent->chanserv.GetStored(channel).GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Clear()).IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    bool allmode = false;
    if (message.After(" ").Matches("*ALL*", true))
	allmode = true;

    Chan_Live_t clt = Parent->chanserv.GetLive(channel);
    vector<mstring> devoice, ops;
    unsigned int i;
    for (i=0; i<clt.Voices(); i++)
    {
	if (!Parent->nickserv.IsLive(clt.Voice(i)) ||
		Parent->nickserv.GetLive(clt.Voice(i)).IsServices())
	    continue;
	devoice.push_back(clt.Voice(i));
	if (!allmode)
	    SEND(mynick, clt.Voice(i), "CS_COMMAND/CLEAR", (
		    message, source, channel));
    }
    for (i=0; i<clt.Ops(); i++)
    {
	if (!allmode)
	    SEND(mynick, clt.Op(i), "CS_COMMAND/CLEAR", (
		    message, source, channel));
    }

    { RLOCK(("ChanServ", "live", channel.LowerCase()));
    Chan_Live_t &clive = Parent->chanserv.GetLive(channel);
    for (i=0; i<devoice.size(); i++)
    {
	clive.SendMode("-v " + devoice[i]);
    }}

    if (!allmode)
    {
	Parent->chanserv.stats.i_Clear++;
	LOG(LM_INFO, "CHANSERV/COMMAND", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		message, channel));
    }
}

void ChanServ::do_clear_Modes(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_clear_Modes", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!Parent->chanserv.GetStored(channel).GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Clear()).IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    bool allmode = false;
    if (message.After(" ").Matches("*ALL*", true))
	allmode = true;

    vector<mstring> ops;
    unsigned int i;
    mstring mode;
    { RLOCK2(("ChanServ", "live", channel.LowerCase()));
    Chan_Live_t &clive = Parent->chanserv.GetLive(channel);

    mode << "-" << clive.Mode();
    if (clive.Limit())
	mode << "l";
    if (!clive.Key().empty())
	mode << "k " << clive.Key();
    clive.SendMode(mode);
    if (!allmode)
    {
	for (i=0; i<clive.Ops(); i++)
	{
	    ops.push_back(clive.Op(i));
	}
    }}
    { Chan_Stored_t cstored = Parent->chanserv.GetStored(channel);
    if (!cstored.Mlock_On().empty())
    {
	mode = "+" + cstored.Mlock_On();
	if (cstored.Mlock_Limit())
	    mode << "l";
	if (!cstored.Mlock_Key().empty())
	    mode << "l";
	if (cstored.Mlock_Limit())
	    mode << " " << cstored.Mlock_Limit();
	if (!cstored.Mlock_Key().empty())
	    mode << " " << cstored.Mlock_Key();
	
	Parent->chanserv.GetLive(channel).SendMode(mode);
    }}
    if (!allmode)
    {
	for (i=0; i<ops.size(); i++)
	{
	    if (!Parent->nickserv.IsLive(ops[i]) ||
		Parent->nickserv.GetLive(ops[i]).IsServices())
		continue;
	    SEND(mynick, ops[i], "CS_COMMAND/CLEAR", (
		    message, source, channel));
	}
	Parent->chanserv.stats.i_Clear++;
	LOG(LM_INFO, "CHANSERV/COMMAND", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		message, channel));
    }
}

void ChanServ::do_clear_Bans(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_clear_Bans", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!Parent->chanserv.GetStored(channel).GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Clear()).IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    bool allmode = false;
    if (message.After(" ").Matches("*ALL*", true))
	allmode = true;

    vector<mstring> ops;
    unsigned int i;
    { RLOCK(("ChanServ", "live", channel.LowerCase()));
    Chan_Live_t &clive = Parent->chanserv.GetLive(channel);

    for (i=0; i<clive.Bans(); i++)
    {
	clive.SendMode("-b " + clive.Ban(i));
    }
    if (!allmode)
    {
	for (i=0; i<clive.Ops(); i++)
	{
	    ops.push_back(clive.Op(i));
	}
    }}

    if (!allmode)
    {
	for (i=0; i<ops.size(); i++)
	{
	    if (!Parent->nickserv.IsLive(ops[i]) ||
		Parent->nickserv.GetLive(ops[i]).IsServices())
		continue;
	    SEND(mynick, ops[i], "CS_COMMAND/CLEAR", (
		    message, source, channel));
	}
	Parent->chanserv.stats.i_Clear++;
	LOG(LM_INFO, "CHANSERV/COMMAND", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		message, channel));
    }
}

void ChanServ::do_clear_All(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_clear_All", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (
		channel));
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!Parent->chanserv.GetStored(channel).GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.GetList(Parent->commserv.OVR_CS_Clear()).IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    ChanServ::do_clear_Modes(mynick, source, params);
    ChanServ::do_clear_Ops(mynick, source, params);
    ChanServ::do_clear_Voices(mynick, source, params);
    ChanServ::do_clear_Bans(mynick, source, params);

    Parent->chanserv.stats.i_Clear++;
    ::notice(mynick, channel, parseMessage(Parent->getMessage("CS_COMMAND/CLEAR"),
		mVarArray(message, source, channel)));
    LOG(LM_INFO, "CHANSERV/COMMAND", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	message, channel));
}

void ChanServ::do_level_Set(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_level_Set", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 5)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring what      = params.ExtractWord(4, " ");
    mstring level     = params.ExtractWord(5, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    long num = 0;
    if (level.Matches("F*ND*R", true))
    {
	num = Parent->chanserv.Level_Max()+1;
    }
    else if (level.Matches("DIS*", true))
    {
	num = Parent->chanserv.Level_Max()+2;
    }
    else if (!level.IsNumber() || level.Contains("."))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }
    else
    {
	num = atol(level.c_str());
    }

    if (num < Parent->chanserv.Level_Min() ||
	num > Parent->chanserv.Level_Max()+2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
		Parent->chanserv.Level_Min(),
		Parent->chanserv.Level_Max()+2));
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored.GetAccess(source) <= Parent->chanserv.Level_Max())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    MLOCK(("ChanServ", "stored", channel.LowerCase(), "Level"));
    if (cstored.Level_find(what))
    {
	const_cast<entlist_val_t<long> *>(&(*cstored.Level))->Value(num, source);
	Parent->chanserv.stats.i_Level++;
	SEND(mynick, source, "LIST/CHANGE2_LEVEL", (
		    cstored.Level->Entry(), channel,
		    Parent->getMessage(source, "LIST/LEVEL"),
		    cstored.Level->Value()));
	LOG(LM_DEBUG, "CHANSERV/LEVEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		cstored.Level->Entry(), cstored.Level->Value(),
		channel));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS2", (
		    what, channel,
		    Parent->getMessage(source, "LIST/LEVEL")));
    }
}

void ChanServ::do_level_Reset(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_level_Reset", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring what      = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored.GetAccess(source) <= Parent->chanserv.Level_Max())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    MLOCK(("ChanServ", "stored", channel.LowerCase(), "Level"));
    if (!what.Matches("ALL", true))
    {
	if (cstored.Level_find(what) &&
		Parent->chanserv.LVL(what) > Parent->chanserv.Level_Min())
	{
	    const_cast<entlist_val_t<long> *>(&(*cstored.Level))->Value(
		Parent->chanserv.LVL(cstored.Level->Entry()), source);
	    Parent->chanserv.stats.i_Level++;
	    SEND(mynick, source, "LIST/CHANGE2_LEVEL", (
		    cstored.Level->Entry(), channel,
		    Parent->getMessage(source, "LIST/LEVEL"),
		    cstored.Level->Value()));
	    LOG(LM_DEBUG, "CHANSERV/LEVEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		cstored.Level->Entry(), cstored.Level->Value(),
		channel));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2", (
		    what, channel,
		    Parent->getMessage(source, "LIST/LEVEL")));
	}
    }
    else
    {
	for (cstored.Level = cstored.Level_begin();
		cstored.Level != cstored.Level_end(); cstored.Level++)
	{
	    const_cast<entlist_val_t<long> *>(&(*cstored.Level))->Value(
		Parent->chanserv.LVL(cstored.Level->Entry()), source);
	}
	Parent->chanserv.stats.i_Level++;
	SEND(mynick, source, "LIST/CHANGE2_ALL", (
		channel, Parent->getMessage(source, "LIST/LEVEL")));
	LOG(LM_DEBUG, "CHANSERV/LEVEL_ALL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		channel));
    }
}

void ChanServ::do_level_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_level_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");
    mstring what;
    if (params.WordCount(" ") > 3)
	what = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    long myaccess = cstored.GetAccess(source);
    bool haveset = cstored.GetAccess(source, "SET");
    if (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.GetList(Parent->commserv.SOP_Name().UpperCase()).IsIn(source))
	haveset = true;

    if (haveset)
    {
	SEND(mynick, source, "CS_COMMAND/LEVEL_HEAD", (
		channel));
    }

    MLOCK(("ChanServ", "stored", channel.LowerCase(), "Level"));
    for (cstored.Level = cstored.Level_begin();
		    cstored.Level != cstored.Level_end();
		    cstored.Level++)
    {
	if (haveset)
	{
	    mstring value;
	    if (cstored.Level->Value() == Parent->chanserv.Level_Max()+2)
		value = Parent->getMessage(source, "VALS/LVL_DISABLED");
	    else if (cstored.Level->Value() == Parent->chanserv.Level_Max()+1)
		value = Parent->getMessage(source, "VALS/LVL_FOUNDER");
	    else
		value = cstored.Level->Value();
	    ::sendV(mynick, source, "%10s  %-15s  %s",
		    value.c_str(), cstored.Level->Entry().c_str(),
		    Parent->getMessage(source, "CS_SET/LVL_" + cstored.Level->Entry()).c_str());
	}
	else if(cstored.Level->Entry() != "AUTODEOP" &&
		cstored.Level->Value() <= myaccess)
	{
	    SEND(mynick, source, "CS_COMMAND/LEVEL_HAVE", (
		    Parent->getMessage(source, "CS_SET/LVL_" + cstored.Level->Entry()),
		    channel));
	}
    }
}

void ChanServ::do_access_Add(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_access_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 5)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring who       = params.ExtractWord(4, " ");
    mstring level     = params.ExtractWord(5, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (!Parent->nickserv.IsStored(who))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
		who));
	return;
    }

    who = Parent->getSname(who);
    if (Parent->nickserv.GetStored(who).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
		who));
	return;
    }

    if (!level.IsNumber() || level.Contains("."))
    {
        NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }
    long num = atol(level.c_str());

    if (num == 0)
    {
	NSEND(mynick, source, "CS_STATUS/NOTZERO");
	return;
    }

    if (num < Parent->chanserv.Level_Min() ||
	num > Parent->chanserv.Level_Max())
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
		Parent->chanserv.Level_Min(),
		Parent->chanserv.Level_Max()));
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "ACCESS"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (num >= cstored.GetAccess(source))
    {
	NSEND(mynick, source, "CS_STATUS/ONLYBELOW");
	return;
    }

    MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Access"));
    if (cstored.Access_find(who))
    {
	mstring entry = cstored.Access->Entry();
	if (cstored.Access->Value() >= cstored.GetAccess(source))
	{
	    SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (
			entry, channel));
	    return;
	}

	cstored.Access_erase();
	cstored.Access_insert(entry, num, source);
	Parent->chanserv.stats.i_Access++;
	SEND(mynick, source, "LIST/CHANGE2_LEVEL", (
		    cstored.Access->Entry(), channel,
		    Parent->getMessage(source, "LIST/ACCESS"),
		    num));
	LOG(LM_DEBUG, "CHANSERV/ACCESS_CHANGE", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		cstored.Access->Entry(), channel, num));
    }
    else
    {
	cstored.Access_insert(who, num, source);
	Parent->chanserv.stats.i_Access++;
	SEND(mynick, source, "LIST/ADD2_LEVEL", (
		    cstored.Access->Entry(), channel,
		    Parent->getMessage(source, "LIST/ACCESS"),
		    num));
	LOG(LM_DEBUG, "CHANSERV/ACCESS_ADD", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		cstored.Access->Entry(), channel, num));
    }
}

void ChanServ::do_access_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_access_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring who       = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "ACCESS"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (who.IsNumber())
    {
	if (who.Contains("."))
	{
	    NSEND(mynick, source, "ERR_STYNTAX/WHOLENUMBER");
	    return;
	}
	unsigned int i, num = atoi(who);
	if (num < 1 || num > cstored.Access_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
		1, cstored.Access_size()));
	    return;
	}

	MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Access"));
	for (i=1, cstored.Access = cstored.Access_begin();
				i<num && cstored.Access != cstored.Access_end();
				i++, cstored.Access++) ;
	if (cstored.Access != cstored.Access_end())
	{
	    if (cstored.Access->Value() >= cstored.GetAccess(source))
	    {
		SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (
			cstored.Access->Entry(), channel));
		return;
	    }

	    Parent->chanserv.stats.i_Access++;
	    SEND(mynick, source, "LIST/DEL2", (
		    cstored.Access->Entry(), channel,
		    Parent->getMessage(source, "LIST/ACCESS")));
	    LOG(LM_DEBUG, "CHANSERV/ACCESS_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		cstored.Access->Entry(), channel));
	    cstored.Access_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2_NUMBER", (
		    num, channel,
		    Parent->getMessage(source, "LIST/ACCESS")));
	}
    }
    else
    {
	MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Access"));
	if (cstored.Access_find(who))
	{
	    if (cstored.Access->Value() >= cstored.GetAccess(source))
	    {
		SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (
			cstored.Access->Entry(), channel));
		return;
	    }

	    Parent->chanserv.stats.i_Access++;
	    SEND(mynick, source, "LIST/DEL2", (
		    cstored.Access->Entry(), channel,
		    Parent->getMessage(source, "LIST/ACCESS")));
	    LOG(LM_DEBUG, "CHANSERV/ACCESS_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		cstored.Access->Entry(), channel));
	    cstored.Access_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2", (
		    who, channel,
		    Parent->getMessage(source, "LIST/ACCESS")));
	}
    }
}

void ChanServ::do_access_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_access_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "VIEW") &&
	!(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.Access_size())
    {
	SEND(mynick, source, "LIST/DISPLAY2", (
		    channel,
		    Parent->getMessage(source, "LIST/ACCESS")));
    }
    else
    {
	SEND(mynick, source, "LIST/EMPTY2", (
		    channel,
		    Parent->getMessage(source, "LIST/ACCESS")));
	return;
    }

    MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Access"));
    unsigned int i;

    for (i=1, cstored.Access = cstored.Access_begin();
	cstored.Access != cstored.Access_end(); cstored.Access++, i++)
    {
	::sendV(mynick, source, "%4d. %3d %s (%s)", i, cstored.Access->Value(),
		    Parent->getSname(cstored.Access->Entry()).c_str(),
		    parseMessage(Parent->getMessage(source, "LIST/LASTMOD"),
		    mVarArray(cstored.Access->Last_Modify_Time().Ago(),
		    cstored.Access->Last_Modifier())).c_str());
    }
}

void ChanServ::do_akick_Add(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_akick_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring who       = params.ExtractWord(4, " ");
    mstring reason    = Parent->chanserv.DEF_Akick_Reason();
    if (params.WordCount(" ") > 4)
	reason        = params.After(" ", 4);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "AKICK"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (who.Contains("!") || who.Contains("@"))
    {
	if (!who.Contains("@"))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
		    Parent->getMessage(source, "LIST/AKICK"), '@'));
	    return;
	}
	else if (!who.Contains("!"))
	{
	    who.prepend("*!");
	}

	unsigned int i, num;
	bool super = cstored.GetAccess(source, "SUPER");
	// i+1 below because unsigned i will always be >= 0
	for (i=who.size()-1, num=0; i+1>0; i--)
	{
	    switch (who[i])
	    {
	    case '@':
		if (!super)
		    i=0;
		break;
	    case '!':	// ALL these constitute wildcards.
	    case '*':
	    case '?':
	    case '.':
		break;
	    default:
		num++;
	    }
	}
	// IF we have less than 1 char for 
	if (!super && num <= Parent->config.Starthresh())
	{
	    SEND(mynick, source, "ERR_SYNTAX/STARTHRESH", (
			Parent->getMessage(source, "LIST/AKICK"),
			Parent->config.Starthresh()));
	    return;
	}
	else if (num <= 1)
	{
	    SEND(mynick, source, "ERR_SYNTAX/STARTHRESH", (
			Parent->getMessage(source, "LIST/AKICK"), 1));
	    return;
	}
    }
    else if (!Parent->nickserv.IsStored(who))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
		who));
	return;
    }
    else if (Parent->nickserv.GetStored(who).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
		Parent->getSname(who)));
	return;
    }
    else
    {
	who = Parent->getSname(who);
	MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Access"));
	if (cstored.Access_find(who))
	{
	    // Reject if they're higher on access list, else erase them
	    // from the access list, AKICK doesnt play nice with ACCESS.
	    if (cstored.Access->Value() >= cstored.GetAccess(source))
	    {
		SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (
			who, channel));
		return;
	    }
	    else
	    {
		cstored.Access_erase();
	    }
	}
    }

    MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Akick"));
    if (cstored.Akick_find(who))
    {
	SEND(mynick, source, "LIST/EXISTS2", (
		who, channel,
		Parent->getMessage(source, "LIST/AKICK")));
    }
    else
    {
	cstored.Akick_insert(who, reason, source);
	Parent->chanserv.stats.i_Akick++;
	SEND(mynick, source, "LIST/ADD2", (
		who, channel,
		Parent->getMessage(source, "LIST/AKICK")));
	LOG(LM_DEBUG, "CHANSERV/AKICK_ADD", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		who, channel));

	if (Parent->chanserv.IsLive(channel.c_str()))
	{
	    unsigned int i;
	    if (who.Contains("@"))
	    {
		// Kick matching users ...
		vector<mstring> kickees;
		Chan_Live_t chan = Parent->chanserv.GetLive(channel);
		for (i=0; i<chan.Users(); i++)
		{
		    // MAN these commands can get REAL long .. ;)
		    if (Parent->nickserv.IsLive(chan.User(i)) &&
			Parent->nickserv.GetLive(chan.User(i)).Mask(Nick_Live_t::N_U_P_H).Matches(who, true))
		    {
			kickees.push_back(chan.User(i));
		    }
		}
		for (i=0; i<kickees.size(); i++)
		{
		    Parent->server.KICK(mynick, kickees[i], channel,
				((!reason.empty()) ? reason : Parent->chanserv.DEF_Akick_Reason()));
		}
	    }
	    else
	    {
		// Kick stored user ...
		mstring realnick = Parent->nickserv.GetStored(who).Host();
		if (realnick.empty())
		    realnick = who;
		if (Parent->chanserv.GetLive(channel).IsIn(realnick))
		{
		    Parent->server.KICK(mynick, realnick, channel,
			((!reason.empty()) ? reason : Parent->chanserv.DEF_Akick_Reason()));
		}
		Nick_Stored_t nick = Parent->nickserv.GetStored(realnick);
		for (i=0; i<nick.Siblings(); i++)
		{
		    if (Parent->chanserv.GetLive(channel).IsIn(nick.Sibling(i)))
		    {
			Parent->server.KICK(mynick, nick.Sibling(i), channel,
				((!reason.empty()) ? reason : Parent->chanserv.DEF_Akick_Reason()));
		    }
		}
	    }
	}
    }
}

void ChanServ::do_akick_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_akick_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring who       = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "AKICK"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (who.IsNumber())
    {
	if (who.Contains("."))
	{
	    NSEND(mynick, source, "ERR_STYNTAX/WHOLENUMBER");
	    return;
	}
	unsigned int i, num = atoi(who);
	if (num < 1 || num > cstored.Akick_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
		1, cstored.Akick_size()));
	    return;
	}

	MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Akick"));
	for (i=1, cstored.Akick = cstored.Akick_begin();
				i<num && cstored.Akick != cstored.Akick_end();
				i++, cstored.Akick++) ;
	if (cstored.Akick != cstored.Akick_end())
	{
	    Parent->chanserv.stats.i_Akick++;
	    SEND(mynick, source, "LIST/DEL2", (
		    cstored.Akick->Entry(), channel,
		    Parent->getMessage(source, "LIST/AKICK")));
	    LOG(LM_DEBUG, "CHANSERV/AKICK_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		cstored.Akick->Entry(), channel));
	    cstored.Akick_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2_NUMBER", (
		    num, channel,
		    Parent->getMessage(source, "LIST/AKICK")));
	}
    }
    else
    {
	if (who.Contains("@") || who.Contains("!"))
	{
	    if (!who.Contains("@"))
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
		    Parent->getMessage(source, "LIST/AKICK"), '@'));
		return;
	    }
	    else if (!who.Contains("!"))
	    {
		who.prepend("*!");
	    }
	}

	MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Akick"));
	if (cstored.Akick_find(who))
	{
	    Parent->chanserv.stats.i_Akick++;
	    SEND(mynick, source, "LIST/DEL2", (
		    cstored.Akick->Entry(), channel,
		    Parent->getMessage(source, "LIST/AKICK")));
	    LOG(LM_DEBUG, "CHANSERV/AKICK_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		cstored.Akick->Entry(), channel));
	    cstored.Akick_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2", (
		    who, channel,
		    Parent->getMessage(source, "LIST/AKICK")));
	}
    }
}

void ChanServ::do_akick_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_akick_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "VIEW") &&
	!(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.Akick_size())
    {
	SEND(mynick, source, "LIST/DISPLAY2", (
		    channel,
		    Parent->getMessage(source, "LIST/AKICK")));
    }
    else
    {
	SEND(mynick, source, "LIST/EMPTY2", (
		    channel,
		    Parent->getMessage(source, "LIST/AKICK")));
	return;
    }

    MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Akick"));
    int i;
    for (i=1, cstored.Akick = cstored.Akick_begin();
	cstored.Akick != cstored.Akick_end(); cstored.Akick++, i++)
    {
	::sendV(mynick, source, "%4d. %s (%s)", i, cstored.Akick->Entry().c_str(),
		    parseMessage(Parent->getMessage(source, "LIST/LASTMOD"),
		    mVarArray(cstored.Akick->Last_Modify_Time().Ago(),
		    cstored.Akick->Last_Modifier())).c_str());
	::send(mynick, source, "      " + cstored.Akick->Value());
    }
}

void ChanServ::do_greet_Add(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_greet_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = Parent->getSname(source);
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if ((option[0U] == '@' || option[0U] == '!') &&
	params.WordCount(" ") > 4 &&
	cstored.GetAccess(source, "OVERGREET"))
    {
	if (option[0U]=='@')
	{
	    target = params.ExtractWord(4, " ").After("@");
	    option = params.After(" ", 4);
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
	while (option[1U] == '!')
	    option = option.After("!");
    }
    else if (!cstored.GetAccess(source, "GREET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }
    else
    {
	while (option[0U] == '!')
	    option = option.After("!");
    }

    if (!Parent->nickserv.GetStored(target).Host().empty())
    {
	target = Parent->nickserv.GetStored(target).Host();
    }

    { MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Greet"));
    if (cstored.Greet_find(target))
    {
	if (cstored.Greet->Entry()[0U] == '!' && source.IsSameAs(target, true) &&
	    !cstored.GetAccess(source, "OVERGREET"))
	{
	    SEND(mynick, source, "CS_STATUS/LOCKGREET", (
		    channel));
	    return;
	}
	cstored.Greet_erase();
    }}
    cstored.Greet_insert(option, target);
    Parent->chanserv.stats.i_Greet++;
    SEND(mynick, source, "LIST/ADD2", (
		target, channel,
		Parent->getMessage(source, "LIST/GREET")));
    LOG(LM_DEBUG, "CHANSERV/GREET_ADD", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, channel));
}

void ChanServ::do_greet_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_greet_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = Parent->getSname(source);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 3 &&
	cstored.GetAccess(source, "OVERGREET"))
    {
	target = params.ExtractWord(4, " ").After("@");
	{ MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Greet"));
	if (!cstored.Greet_find(target))
	{
	    if (Parent->nickserv.IsStored(target))
	    {
		target = Parent->nickserv.GetStored(target).Host();
		if (target.empty())
		    target = Parent->getSname(target);
	    }

	    if (!cstored.Greet_find(target))
	    {
		SEND(mynick, source, "LIST/NOTEXISTS2", (
			target, channel,
			Parent->getMessage(source, "LIST/GREET")));
		return;
	    }
	}}
    }
    else if (!cstored.GetAccess(source, "GREET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (!source.IsSameAs(target, true))
    {
	cstored.Greet_erase();
	Parent->chanserv.stats.i_Greet++;
	SEND(mynick, source, "LIST/DEL2", (
			target, channel,
			Parent->getMessage(source, "LIST/GREET")));
    }
    else
    {
	if (!Parent->nickserv.GetStored(target).Host().empty())
	    target = Parent->nickserv.GetStored(target).Host();

	{ MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Greet"));
	if (cstored.Greet_find(target))
	{
	    if (cstored.Greet->Entry()[0U] == '!' &&
		!cstored.GetAccess(source, "OVERGREET"))
	    {
		SEND(mynick, source, "CS_STATUS/LOCKGREET", (
			    channel));
		return;
	    }
	    cstored.Greet_erase();
	    Parent->chanserv.stats.i_Greet++;
	    SEND(mynick, source, "LIST/DEL2", (
			target, channel,
			Parent->getMessage(source, "LIST/GREET")));
	    LOG(LM_DEBUG, "CHANSERV/GREET_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		target, channel));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2", (
			target, channel,
			Parent->getMessage(source, "LIST/GREET")));
	}}
    }
}

void ChanServ::do_greet_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_greet_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel = params.ExtractWord(2, " ");
    mstring target;
    bool all = false;

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 3 &&
	(cstored.GetAccess(source, "OVERGREET") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source))))
    {
	if (params.ExtractWord(4, " ").IsSameAs("all", true))
	    all = true;
	else
	    target = params.ExtractWord(4, " ").After("@");
    }
    else if (!cstored.GetAccess(source, "GREET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }
    else
    {
	target = Parent->getSname(source);
    }

    if (!target.empty() && Parent->nickserv.IsStored(target))
    {
	if (!Parent->nickserv.GetStored(target).Host().empty())
	    target = Parent->nickserv.GetStored(target).Host();
    }

    bool found = false;
    { MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Greet"));
    for (cstored.Greet = cstored.Greet_begin();
		    cstored.Greet != cstored.Greet_end(); cstored.Greet++)
    {
	if (all || cstored.Greet->Last_Modifier().IsSameAs(target, true))
	{
	    ::sendV(mynick, source, "[%s] %s",
				cstored.Greet->Last_Modifier().c_str(),
				cstored.Greet->Entry().c_str());
	    found = true;
	}
    }}
    if (!found)
    {
	if (all)
	    SEND(mynick, source, "LIST/EMPTY2", (
			channel,
			Parent->getMessage(source, "LIST/GREET")));
	else
	    SEND(mynick, source, "LIST/NOTEXISTS2", (
			target, channel,
			Parent->getMessage(source, "LIST/GREET")));
    }
}

void ChanServ::do_message_Add(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_message_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring text      = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "MESSAGE"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.Message_size() >= Parent->chanserv.Max_Messages())
    {
	SEND(mynick, source, "CS_STATUS/MAX_MESSAGES", (
		channel));
	return;
    }

    cstored.Message_insert(text, source);
    Parent->chanserv.stats.i_Message++;
    SEND(mynick, source, "LIST/ADD2_NUMBER", (
		cstored.Message_size(), channel,
		Parent->getMessage(source, "LIST/JOINMSG")));
    LOG(LM_DEBUG, "CHANSERV/MESSAGE_ADD", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	channel));
}

void ChanServ::do_message_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_message_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target  = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (!target.IsNumber() || target.Contains("."))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }

    RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "MESSAGE"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    int num = atoi(target);
    if (num < 1 || num > static_cast<int>(cstored.Message_size()))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
		1, cstored.Message_size()));
	return;
    }

    if (!cstored.Message_size())
    {
	SEND(mynick, source, "LIST/EMPTY2", (
		channel,
		Parent->getMessage(source, "LIST/JOINMSG")));
	return;
    }

    { MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Message"));
    if (cstored.Message_find(num))
    {
        cstored.Message_erase();
	Parent->chanserv.stats.i_Message++;
	SEND(mynick, source, "LIST/DEL2_NUMBER", (
		num, channel,
		Parent->getMessage(source, "LIST/JOINMSG")));
	LOG(LM_DEBUG, "CHANSERV/MESSAGE_DEL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		channel));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS2_NUMBER", (
		num, channel,
		Parent->getMessage(source, "LIST/JOINMSG")));
    }}
}

void ChanServ::do_message_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_message_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    Chan_Stored_t cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!(cstored.GetAccess(source, "MESSAGE") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.GetList(Parent->commserv.OVR_View()).IsOn(source))))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (!cstored.Message_size())
    {
	SEND(mynick, source, "LIST/EMPTY2", (
		channel,
		Parent->getMessage(source, "LIST/JOINMSG")));
	return;
    }

    SEND(mynick, source, "LIST/DISPLAY2", (
	    channel, Parent->getMessage(source, "LIST/JOINMSG")));

    { MLOCK(("ChanServ", "stored", cstored.Name().LowerCase(), "Message"));
    int i;
    for (i=1, cstored.Message = cstored.Message_begin();
				cstored.Message != cstored.Message_end();
				cstored.Message++, i++)
    {
        ::sendV(mynick, source, "%d. %s", i, cstored.Message->Entry().c_str());
    }}
}

void ChanServ::do_set_Founder(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Founder", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring founder   = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (!Parent->nickserv.IsStored(founder))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
		founder));
	return;
    }
    else if (Parent->nickserv.GetStored(founder).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
		Parent->getSname(founder)));
	return;
    }
    else if (!Parent->nickserv.GetStored(founder).Host().empty())
    {
	founder = Parent->nickserv.GetStored(founder).Host();
    }
    else
    {
	founder = Parent->getSname(founder);
    }

    if (Parent->chanserv.Max_Per_Nick() &&
	Parent->nickserv.GetStored(founder).MyChannels() >=
	Parent->chanserv.Max_Per_Nick())
    {
	SEND(mynick, source, "CS_STATUS/OTH_TOOMANY", (
				founder));
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!(cstored.GetAccess(source) > Parent->chanserv.Level_Max() ||
	(Parent->commserv.IsList(Parent->commserv.OVR_Owner()) &&
	Parent->commserv.GetList(Parent->commserv.OVR_Owner()).IsOn(source))))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored.Founder(founder);
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/FOUNDER"),
	    channel, founder));
    LOG(LM_INFO, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/FOUNDER"),
	channel, founder));
}

void ChanServ::do_set_CoFounder(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_CoFounder", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring founder   = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (!Parent->nickserv.IsStored(founder))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
		founder));
	return;
    }
    else if (Parent->nickserv.GetStored(founder).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
		Parent->getSname(founder)));
	return;
    }
    else if (!Parent->nickserv.GetStored(founder).Host().empty())
    {
	founder = Parent->nickserv.GetStored(founder).Host();
    }
    else
    {
	founder = Parent->getSname(founder);
    }

    if (Parent->chanserv.Max_Per_Nick() &&
	Parent->nickserv.GetStored(founder).MyChannels() >=
	Parent->chanserv.Max_Per_Nick())
    {
	SEND(mynick, source, "CS_STATUS/OTH_TOOMANY", (
				founder));
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (cstored.Founder().IsSameAs(founder, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COFOUNDER");
	return;
    }

    if (!(cstored.GetAccess(source) > Parent->chanserv.Level_Max() ||
	(Parent->commserv.IsList(Parent->commserv.OVR_Owner()) &&
	Parent->commserv.GetList(Parent->commserv.OVR_Owner()).IsOn(source))))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored.CoFounder(founder);
    } 
    Parent->chanserv.stats.i_Set++;
    if (founder.empty())
    {
	SEND(mynick, source, "CS_COMMAND/UNSET", (
	    Parent->getMessage(source, "CS_SET/COFOUNDER"),
	    channel));
	LOG(LM_INFO, "CHANSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/COFOUNDER"),
		channel));
    }
    else
    {
	SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/COFOUNDER"),
	    channel, founder));
	LOG(LM_INFO, "CHANSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/COFOUNDER"),
		channel, founder));
    }
}

void ChanServ::do_set_Description(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Description", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored.Description(option);
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/DESCRIPTION"),
	    channel, option));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/DESCRIPTION"),
	channel, option));
}

void ChanServ::do_set_Password(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Password", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring password  = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (password.length() < 5 || password.IsSameAs(channel.After(channel[0u]), true) ||
	password.IsSameAs(channel, true) || password.IsSameAs(source, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored.GetAccess(source) <= Parent->chanserv.Level_Max())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored.Password(password);
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/PASSWORD"),
	    channel, password));
    LOG(LM_INFO, "CHANSERV/SET_PASSWORD", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	channel));
}

void ChanServ::do_set_Email(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Email", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (option.IsSameAs("none", true))
	option.erase();
    else if (!option.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
		Parent->getMessage(source, "CS_SET/EMAIL"), '@'));
	return;
    }
    else if (option.WordCount("@") != 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE", (
		Parent->getMessage(source, "CS_SET/EMAIL"), '@'));
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored.Email(option);
    }
    Parent->chanserv.stats.i_Set++;
    if (option.empty())
    {
	SEND(mynick, source, "CS_COMMAND/UNSET", (
	    Parent->getMessage(source, "CS_SET/EMAIL"),
	    channel));
	LOG(LM_DEBUG, "CHANSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/EMAIL"),
		channel));
    }
    else
    {
	SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/EMAIL"),
	    channel, option));
	LOG(LM_DEBUG, "CHANSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/EMAIL"),
		channel, option));
    }
}

void ChanServ::do_set_URL(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_URL", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (option.IsSameAs("none", true))
	option.erase();

    if (option.SubString(0, 6).IsSameAs("http://", true))
    {
	option.erase(0, 6);
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored.URL(option);
    }
    Parent->chanserv.stats.i_Set++;
    if (option.empty())
    {
	SEND(mynick, source, "CS_COMMAND/UNSET", (
	    Parent->getMessage(source, "CS_SET/URL"),
	    channel));
	LOG(LM_DEBUG, "CHANSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/URL"),
		channel));
    }
    else
    {
	SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/URL"),
	    channel, "http://" + option));
	LOG(LM_DEBUG, "CHANSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/URL"),
		channel, "http://" + option));
    }
}

void ChanServ::do_set_Comment(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Comment", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (option.IsSameAs("none", true))
	option.erase();

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    cstored.Comment(option);
    }
    Parent->chanserv.stats.i_Set++;
    if (option.empty())
    {
	SEND(mynick, source, "CS_COMMAND/UNSET", (
	    Parent->getMessage(source, "CS_SET/COMMENT"),
	    channel));
	LOG(LM_DEBUG, "CHANSERV/UNSET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/COMMENT"),
		channel));
    }
    else
    {
	SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/COMMENT"),
	    channel, option));
	LOG(LM_DEBUG, "CHANSERV/SET", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage("CS_SET/COMMENT"),
		channel, option));
    }
}

void ChanServ::do_set_Mlock(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Mlock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (option.IsSameAs("default", true) || option.IsSameAs("reset", true))
    {
	option = Parent->chanserv.DEF_MLock();
    }

    vector<mstring> retval;
    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    retval = cstored.Mlock(source, option);
    } 
    Parent->chanserv.stats.i_Set++;
    for (unsigned int i=0; i<retval.size(); i++)
	::send(mynick, source, retval[i]);
}

void ChanServ::do_set_BanTime(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_BanTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring value     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    unsigned long num = FromHumanTime(value);
    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Bantime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/BANTIME"),
		channel));
	return;
    }

    cstored.Bantime(num);
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/BANTIME"),
	    channel, ToHumanTime(num, source)));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/BANTIME"),
	channel, ToHumanTime(num, source)));
}

void ChanServ::do_set_PartTime(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_PartTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring value     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    unsigned long num = FromHumanTime(value);
    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Parttime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/PARTTIME"),
		channel));
	return;
    }

    cstored.Parttime(num);
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/PARTTIME"),
	    channel, ToHumanTime(num, source)));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/PARTTIME"),
	channel, ToHumanTime(num, source)));
}

void ChanServ::do_set_KeepTopic(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_KeepTopic", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Keeptopic())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Keeptopic())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/KEEPTOPIC"),
		channel));
	return;
    }

    cstored.Keeptopic(onoff.GetBool());
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/KEEPTOPIC"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/KEEPTOPIC"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_set_TopicLock(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_TopicLock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Topiclock())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Topiclock())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/TOPICLOCK"),
		channel));
	return;
    }

    cstored.Topiclock(onoff.GetBool());
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/TOPICLOCK"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/KEEPTOPIC"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_set_Private(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Private())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/PRIVATE"),
		channel));
	return;
    }

    cstored.Private(onoff.GetBool());
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/PRIVATE"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/PRIVATE"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_set_SecureOps(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_SecureOps", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Secureops())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Secureops())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/SECUREOPS"),
		channel));
	return;
    }

    cstored.Secureops(onoff.GetBool());
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/SECUREOPS"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/SECUREOPS"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_set_Secure(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Secure())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/SECURE"),
		channel));
	return;
    }

    cstored.Secure(onoff.GetBool());
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/SECURE"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/SECURE"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_set_NoExpire(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_NoExpire", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_NoExpire())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (cstored.L_NoExpire())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/NOEXPIRE"),
		channel));
	return;
    }

    cstored.NoExpire(onoff.GetBool());
    }
    Parent->chanserv.stats.i_NoExpire++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/NOEXPIRE"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/NOEXPIRE"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_set_Anarchy(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Anarchy", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Anarchy())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Anarchy())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/ANARCHY"),
		channel));
	return;
    }

    cstored.Anarchy(onoff.GetBool());
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/ANARCHY"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/ANARCHY"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_set_KickOnBan(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_KickOnBan", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_KickOnBan())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_KickOnBan())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/KICKONBAN"),
		channel));
	return;
    }

    cstored.KickOnBan(onoff.GetBool());
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/KICKONBAN"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/KICKONBAN"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_set_Restricted(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Restricted", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Restricted())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Restricted())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/RESTRICTED"),
		channel));
	return;
    }

    cstored.Restricted(onoff.GetBool());
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/RESTRICTED"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/RESTRICTED"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_set_Join(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Join())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Join())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/JOIN"),
		channel));
	return;
    }

    cstored.Join(onoff.GetBool());
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/JOIN"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/JOIN"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    if (onoff.GetBool() && Parent->chanserv.IsLive(channel) &&
	!Parent->chanserv.GetLive(channel).IsIn(
		Parent->chanserv.FirstName()))
    {
	Parent->server.JOIN(Parent->chanserv.FirstName(), channel);
    }
    else if (!onoff.GetBool() && Parent->chanserv.IsLive(channel) &&
	Parent->chanserv.GetLive(channel).IsIn(
		Parent->chanserv.FirstName()))
    {
	Parent->server.PART(Parent->chanserv.FirstName(), channel);
    }
}

void ChanServ::do_set_Revenge(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_set_Revenge", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (option.IsSameAs("default", true) || option.IsSameAs("reset", true))
    {
	option = Parent->chanserv.DEF_Revenge();
    }
    else if (!Parent->chanserv.IsRevengeLevel(option))
    {
	SEND(mynick, source, "CS_STATUS/NOREVENGE", (
		option.UpperCase()));
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored.GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored.L_Revenge())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/REVENGE"),
		channel));
	return;
    }

    cstored.Revenge(option.UpperCase());
    }
    Parent->chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (
	    Parent->getMessage(source, "CS_SET/REVENGE"),
	    channel,
	    Parent->getMessage(source, "CS_SET/REV_" + option.UpperCase())));
    LOG(LM_DEBUG, "CHANSERV/SET", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/REVENGE"),
	channel,
	Parent->getMessage("CS_SET/REV_" + option.UpperCase())));
}

void ChanServ::do_lock_Mlock(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_Mlock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (option.IsSameAs("default", true) || option.IsSameAs("reset", true))
    {
	option = Parent->chanserv.DEF_MLock();
    }

    vector<mstring> retval;
    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    retval = cstored.L_Mlock(source, option);
    }
    Parent->chanserv.stats.i_Lock++;
    for (unsigned int i=0; i<retval.size(); i++)
	::send(mynick, source, retval[i]);
}

void ChanServ::do_lock_BanTime(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_BanTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring value     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    unsigned long num = FromHumanTime(value);
    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Bantime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/BANTIME"),
		channel));
	return;
    }

    cstored.L_Bantime(false);
    cstored.Bantime(num);
    cstored.L_Bantime(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/BANTIME"),
	    channel, ToHumanTime(num)));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/BANTIME"),
	channel, ToHumanTime(num)));
}

void ChanServ::do_lock_PartTime(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_PartTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring value     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    unsigned long num = FromHumanTime(value);
    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Parttime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/PARTTIME"),
		channel));
	return;
    }

    cstored.L_Parttime(false);
    cstored.Parttime(num);
    cstored.L_Parttime(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/PARTTIME"),
	    channel, ToHumanTime(num, source)));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/PARTTIME"),
	channel, ToHumanTime(num, source)));
}

void ChanServ::do_lock_KeepTopic(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_KeepTopic", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Keeptopic())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Keeptopic())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/KEEPTOPIC"),
		channel));
	return;
    }

    cstored.L_Keeptopic(false);
    cstored.Keeptopic(onoff.GetBool());
    cstored.L_Keeptopic(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/KEEPTOPIC"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/KEEPTOPIC"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_lock_TopicLock(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_TopicLock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Topiclock())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Topiclock())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/TOPICLOCK"),
		channel));
	return;
    }

    cstored.L_Topiclock(false);
    cstored.Topiclock(onoff.GetBool());
    cstored.L_Topiclock(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/TOPICLOCK"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/TOPICLOCK"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_lock_Private(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Private())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/PRIVATE"),
		channel));
	return;
    }

    cstored.L_Private(false);
    cstored.Private(onoff.GetBool());
    cstored.L_Private(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/PRIVATE"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/PRIVATE"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_lock_SecureOps(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_SecureOps", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Secureops())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Secureops())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/SECUREOPS"),
		channel));
	return;
    }

    cstored.L_Secureops(false);
    cstored.Secureops(onoff.GetBool());
    cstored.L_Secureops(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/SECUREOPS"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/SECUREOPS"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_lock_Secure(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Secure())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/SECURE"),
		channel));
	return;
    }

    cstored.L_Secure(false);
    cstored.Secure(onoff.GetBool());
    cstored.L_Secure(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/SECURE"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/SECURE"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_lock_Anarchy(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_Anarchy", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Anarchy())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Anarchy())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/ANARCHY"),
		channel));
	return;
    }

    cstored.L_Anarchy(false);
    cstored.Anarchy(onoff.GetBool());
    cstored.L_Anarchy(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/ANARCHY"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/ANARCHY"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_lock_KickOnBan(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_KickOnBan", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_KickOnBan())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_KickOnBan())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/KICKONBAN"),
		channel));
	return;
    }

    cstored.L_KickOnBan(false);
    cstored.KickOnBan(onoff.GetBool());
    cstored.L_KickOnBan(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/KICKONBAN"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/KICKONBAN"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_lock_Restricted(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_Restricted", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Restricted())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Restricted())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/RESTRICTED"),
		channel));
	return;
    }

    cstored.L_Restricted(false);
    cstored.Restricted(onoff.GetBool());
    cstored.L_Restricted(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/RESTRICTED"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/RESTRICTED"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
}

void ChanServ::do_lock_Join(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

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

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Parent->chanserv.DEF_Join())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Join())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/JOIN"),
		channel));
	return;
    }

    cstored.L_Join(false);
    cstored.Join(onoff.GetBool());
    cstored.L_Join(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/JOIN"),
	    channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/JOIN"),
	channel, onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON") :
		Parent->getMessage(source, "VALS/OFF")));
    if (onoff.GetBool() && Parent->chanserv.IsLive(channel) &&
	!Parent->chanserv.GetLive(channel).IsIn(
		Parent->chanserv.FirstName()))
    {
	Parent->server.JOIN(Parent->chanserv.FirstName(), channel);
    }
    else if (!onoff.GetBool() && Parent->chanserv.IsLive(channel) &&
	Parent->chanserv.GetLive(channel).IsIn(
		Parent->chanserv.FirstName()))
    {
	Parent->server.PART(Parent->chanserv.FirstName(), channel);
    }
}

void ChanServ::do_lock_Revenge(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_lock_Revenge", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    if (option.IsSameAs("default", true) || option.IsSameAs("reset", true))
    {
	option = Parent->chanserv.DEF_Revenge();
    }
    else if (!Parent->chanserv.IsRevengeLevel(option))
    {
	SEND(mynick, source, "CS_STATUS/NOREVENGE", (
		option.UpperCase()));
	return;
    }

    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Revenge())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/REVENGE"),
		channel));
	return;
    }

    cstored.L_Revenge(false);
    cstored.Revenge(option.UpperCase());
    cstored.L_Revenge(true);
    }
    Parent->chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED", (
	    Parent->getMessage(source, "CS_SET/REVENGE"),
	    channel,
	    Parent->getMessage(source, "CS_SET/REV_" + option.UpperCase())));
    LOG(LM_DEBUG, "CHANSERV/LOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/REVENGE"),
	channel,
	Parent->getMessage("CS_SET/REV_" + option.UpperCase())));
}

void ChanServ::do_unlock_Mlock(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_Mlock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }

    vector<mstring> retval;
    { RLOCK(("ChanServ", "stored", channel.LowerCase()));
    Chan_Stored_t &cstored = Parent->chanserv.GetStored(channel);
    channel = cstored.Name();

    if (cstored.Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    retval = cstored.L_Mlock(source, "");
    }
    Parent->chanserv.stats.i_Unlock++;
    for (unsigned int i=0; i<retval.size(); i++)
	::send(mynick, source, retval[i]);
}

void ChanServ::do_unlock_BanTime(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_BanTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Bantime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/BANTIME"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Bantime(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/BANTIME"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/BANTIME"),
	channel));
}

void ChanServ::do_unlock_PartTime(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_PartTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Parttime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/PARTTIME"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Parttime(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/PARTTIME"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/PARTTIME"),
	channel));
}

void ChanServ::do_unlock_KeepTopic(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_KeepTopic", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Keeptopic())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/KEEPTOPIC"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Keeptopic(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/KEEPTOPIC"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/KEEPTOPIC"),
	channel));
}

void ChanServ::do_unlock_TopicLock(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_TopicLock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Topiclock())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/TOPICLOCK"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Topiclock(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/TOPICLOCK"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/TOPICLOCK"),
	channel));
}

void ChanServ::do_unlock_Private(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
 
    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Private())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/PRIVATE"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Private(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/PRIVATE"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/PRIVATE"),
	channel));
}

void ChanServ::do_unlock_SecureOps(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_SecureOps", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Secureops())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/SECUREOPS"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Secureops(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/SECUREOPS"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/SECUREOPS"),
	channel));
}

void ChanServ::do_unlock_Secure(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Secure())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/SECURE"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Secure(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/SECURE"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/SECURE"),
	channel));
}

void ChanServ::do_unlock_Anarchy(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_Anarchy", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Anarchy())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/ANARCHY"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Anarchy(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/ANARCHY"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/ANARCHY"),
	channel));
}

void ChanServ::do_unlock_KickOnBan(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_KickOnBan", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_KickOnBan())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/KICKONBAN"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_KickOnBan(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/KICKONBAN"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/KICKONBAN"),
	channel));
}

void ChanServ::do_unlock_Restricted(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_Restricted", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Restricted())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/RESTRICTED"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Restricted(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/RESTRICTED"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/RESTRICTED"),
	channel));
}

void ChanServ::do_unlock_Join(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Join())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/JOIN"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Join(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/JOIN"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/JOIN"),
	channel));
}

void ChanServ::do_unlock_Revenge(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("ChanServ::do_unlock_Revenge", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		channel));
	return;
    }
    channel = Parent->getSname(channel);

    if (Parent->chanserv.GetStored(channel).Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Parent->chanserv.LCK_Revenge())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (
		Parent->getMessage(source, "CS_SET/REVENGE"),
		channel));
	return;
    }

    Parent->chanserv.GetStored(channel).L_Revenge(false);
    Parent->chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (
	    Parent->getMessage(source, "CS_SET/REVENGE"),
	    channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK", (
	Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Parent->getMessage("CS_SET/REVENGE"),
	channel));
}


long ChanServ::LVL(const mstring& level) const
{
    FT("ChanServ::LVL", (level));
    long retval = 0;
    map<mstring,long>::const_iterator i = lvl.find(level.UpperCase());
    if (i == lvl.end() ||
	i->second > Level_Max() + 1 ||
	i->second < Level_Min())
    {
	retval = Level_Min()-1;
    }
    else
    {
	retval = i->second;
    }
    RET(retval);
}


vector<mstring> ChanServ::LVL()const
{
    NFT("ChanServ::LVL");

    vector<mstring> retval;
    map<mstring, long>::const_iterator iter;
    for (iter = lvl.begin(); iter != lvl.end(); iter++)
    {
	retval.push_back(iter->first.UpperCase());
    }
    NRET(vector<mstring>, retval);
}


bool ChanServ::IsLVL(const mstring& level)const
{
    FT("ChanServ::IsLVL", (level));
    bool retval = lvl.find(level.UpperCase()) != lvl.end();
    RET(retval);
}


SXP::Tag ChanServ::tag_ChanServ("ChanServ");

void ChanServ::BeginElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    FT("ChanServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Chan_Stored_t *cs = new Chan_Stored_t;

    if( pElement->IsA( cs->GetClassTag() ) )
    {
	cs_array.push_back(cs);
	pIn->ReadTo(cs);
    }
    else
    {
	delete cs;
    }
}

void ChanServ::EndElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    FT("ChanServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void ChanServ::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("ChanServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_ChanServ);

    ChanServ::stored_t::iterator iter;
    { RLOCK(("ChanServ", "stored"));
    for (iter = StoredBegin(); iter != StoredEnd(); iter++)
    {
	RLOCK2(("ChanServ", "stored", iter->first));
	pOut->WriteSubElement(&iter->second);
    }}

    pOut->EndObject(tag_ChanServ);
}

void ChanServ::PostLoad()
{
    NFT("ChanServ::PostLoad");
    // Linkage, etc
    unsigned int i, j;
    for (i=0; i<cs_array.size(); i++)
    {
	if (cs_array[i] != NULL)
	{
	    for (j=0; j<cs_array[i]->level_array.size(); j++)
	    {
		if (cs_array[i]->level_array[j] != NULL)
		{
		    cs_array[i]->i_Level.insert(*cs_array[i]->level_array[j]);
		    delete cs_array[i]->level_array[j];
		}
	    }
	    cs_array[i]->level_array.clear();
	    for (j=0; j<cs_array[i]->access_array.size(); j++)
	    {
		if (cs_array[i]->access_array[j] != NULL)
		{
		    cs_array[i]->i_Access.insert(*cs_array[i]->access_array[j]);
		    delete cs_array[i]->access_array[j];
		}
	    }
	    cs_array[i]->access_array.clear();
	    for (j=0; j<cs_array[i]->akick_array.size(); j++)
	    {
		if (cs_array[i]->akick_array[j] != NULL)
		{
		    cs_array[i]->i_Akick.insert(*cs_array[i]->akick_array[j]);
		    delete cs_array[i]->akick_array[j];
		}
	    }
	    cs_array[i]->akick_array.clear();
	    for (j=0; j<cs_array[i]->greet_array.size(); j++)
	    {
		if (cs_array[i]->greet_array[j] != NULL)
		{
		    cs_array[i]->i_Greet.push_back(*cs_array[i]->greet_array[j]);
		    delete cs_array[i]->greet_array[j];
		}
	    }
	    cs_array[i]->greet_array.clear();
	    for (j=0; j<cs_array[i]->message_array.size(); j++)
	    {
		if (cs_array[i]->message_array[j] != NULL)
		{
		    cs_array[i]->i_Message.push_back(*cs_array[i]->message_array[j]);
		    delete cs_array[i]->message_array[j];
		}
	    }
	    cs_array[i]->message_array.clear();
	    for (j=0; j<cs_array[i]->ud_array.size(); j++)
	    {
		if (cs_array[i]->ud_array[j] != NULL)
		{
		    if (cs_array[i]->ud_array[j]->Contains("\n"))
			cs_array[i]->i_UserDef[cs_array[i]->ud_array[j]->Before("\n")] =
				cs_array[i]->ud_array[j]->After("\n");
		    delete cs_array[i]->ud_array[j];
		}
	    }
	    cs_array[i]->ud_array.clear();
	    if (!cs_array[i]->Name().empty())
		AddStored(cs_array[i]);
	    delete cs_array[i];
	}
    }
    cs_array.clear();

    mstring locked = Parent->chanserv.LCK_MLock();

    ChanServ::stored_t::iterator iter;
    RLOCK(("ChanServ", "stored"));
    for (iter=StoredBegin(); iter!=StoredEnd(); iter++)
    {
	WLOCK(("ChanServ", "stored", iter->first));
	for (iter->second.Level = iter->second.Level_begin();
		iter->second.Level != iter->second.Level_end();
		iter->second.Level++)
	{
	    iter->second.Level->PostLoad();
	}
	for (iter->second.Access = iter->second.Access_begin();
		iter->second.Access != iter->second.Access_end();
		iter->second.Access++)
	{
	    iter->second.Access->PostLoad();
	}
	for (iter->second.Akick = iter->second.Akick_begin();
		iter->second.Akick != iter->second.Akick_end();
		iter->second.Akick++)
	{
	    iter->second.Akick->PostLoad();
	}
	for (iter->second.Greet = iter->second.Greet_begin();
		iter->second.Greet != iter->second.Greet_end();
		iter->second.Greet++)
	    iter->second.Greet->PostLoad();
	for (iter->second.Message = iter->second.Message_begin();
		iter->second.Message != iter->second.Message_end();
		iter->second.Message++)
	    iter->second.Message->PostLoad();

	// Now, we're fully loaded, do sanity checks from CFG ...
	bool add = true;
	for (i=0; i<locked.size(); i++)
	{
	    if (locked[i] == '+')
	    {
		add = true;
	    }
	    else if (locked[i] == '-')
	    {
		add = false;
	    }
	    else
	    {
		if (add)
		{
		    if (!Parent->server.proto.ChanModeArg().Contains(locked[i]))
		    {
			if (!iter->second.i_Mlock_On.Contains(locked[i]))
			    iter->second.i_Mlock_On += locked[i];
			if (iter->second.i_Mlock_Off.Contains(locked[i]))
			    iter->second.i_Mlock_Off.Remove(locked[i]);
		    }
		}
		else
		{
		    if (locked[i] == 'k')
			iter->second.i_Mlock_Key.erase();
		    else if (locked[i] == 'l')
			iter->second.i_Mlock_Limit = 0;

		    if (locked[i] == 'k' || locked[i] == 'l' ||
			!Parent->server.proto.ChanModeArg().Contains(locked[i]))
		    {
			if (!iter->second.i_Mlock_Off.Contains(locked[i]))
			    iter->second.i_Mlock_Off += locked[i];
			if (iter->second.i_Mlock_On.Contains(locked[i]))
			    iter->second.i_Mlock_On.Remove(locked[i]);
		    }
		}
	    }
	}
    }
}
