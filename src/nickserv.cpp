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
** Revision 1.194  2001/12/23 08:17:36  prez
** Added ability to add both hostmasks and committees to channel access
** lists.  Priority of access search is nickname, committee, then mask.
**
** Revision 1.193  2001/12/21 05:02:29  prez
** Changed over from using a global ACE_Reactor to using an instance inside
** of the Magick instance.
**
** Revision 1.192  2001/12/20 08:02:32  prez
** Massive change -- 'Parent' has been changed to Magick::instance(), will
** soon also move the ACE_Reactor over, and will be able to have multipal
** instances of Magick in the same process if necessary.
**
** Revision 1.191  2001/11/30 09:01:56  prez
** Changed Magick to have Init(), Start(), Run(), Stop(), Finish() and
** Pause(bool) functions. This should help if/when we decide to implement
** Magick running as an NT service.
**
** Revision 1.190  2001/11/12 01:05:03  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.189  2001/11/04 19:23:09  ungod
** fixed up compilation for borland c++ builder
**
** Revision 1.188  2001/11/03 21:02:53  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.187  2001/07/16 03:36:14  prez
** Got rid of mstring's strcmp, now using memcmp.  Also did a little
** tweaking with the protocol support.
**
** Revision 1.186  2001/07/15 07:35:38  prez
** Fixed problem of it removing access list entries on slave nickname drop.
** Also fixed it so it wouldnt ignore ini entries that were deliberately blank.
**
** Revision 1.185  2001/07/08 01:37:55  prez
** Verified encryption works ...
**
** Revision 1.184  2001/07/06 09:15:37  prez
** Fixed nickserv drop
**
** Revision 1.183  2001/07/05 05:59:12  prez
** More enhansements to try and avoid Signal #6's, coredumps, and deadlocks.
**
** Revision 1.182  2001/07/03 06:00:07  prez
** More deadlock fixes ... also cleared up the Signal #6 problem.
**
** Revision 1.181  2001/06/17 09:39:07  prez
** Hopefully some more changes that ensure uptime (mainly to do with locking
** entries in an iterated search, and using copies of data instead of references
** where we can get away with it -- reducing the need to lock the data).
**
** Revision 1.180  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.179  2001/05/28 11:17:34  prez
** Added some more anti-deadlock stuff, and fixed nick ident warnings
**
** Revision 1.178  2001/05/23 02:43:48  prez
** Fixed the NOACCESS bug, the chanserv getpass/setpass bug and nickserv failed
** passwords kill bug.
**
** Revision 1.177  2001/05/22 22:57:10  prez
** Fixed nick linking/idetify, and recognition of committee heads.
**
** Revision 1.176  2001/05/22 05:17:44  prez
** Fixed nickserv ident from slave nick bug.
**
** Revision 1.175  2001/05/17 19:18:55  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.174  2001/05/13 22:01:36  prez
** Should have fixed problems with linked nicknames
**
** Revision 1.173  2001/05/13 00:55:18  prez
** More patches to try and fix deadlocking ...
**
** Revision 1.172  2001/05/09 00:40:55  prez
** Fixed nicks expiring coz of crash before it saw them quit
**
** Revision 1.171  2001/05/08 06:02:28  prez
** Fixed bad KLLL_NICK_PASS and KLLL_CHAN_PASS
**
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
	if (Magick::instance().reactor().cancel_timer(timer,
		reinterpret_cast<const void **>(arg)) &&
	    arg != NULL)
	{
	    delete arg;
	}
	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);
	timer = ACE_Reactor::instance()->schedule_timer(&Magick::instance().nickserv.ifh,
			new mstring(nick.LowerCase()),
			ACE_Time_Value(Magick::instance().memoserv.InFlight()));
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
	if (Magick::instance().reactor().cancel_timer(timer,
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
	if (Magick::instance().reactor().cancel_timer(timer,
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
    if (!Magick::instance().nickserv.IsStored(nick))
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/ISNOTSTORED");
	return;
    }

    if (IsChan(who))
    {
	if (!Magick::instance().chanserv.IsStored(who))
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
	if (!Magick::instance().nickserv.IsStored(who))
	{
	    SEND(mynick, nick, "NS_OTH_STATUS/ISNOTSTORED", (
			who));
	    return;
	}
	if (Magick::instance().nickserv.GetStored(who).Forbidden())
	{
	    SEND(mynick, nick, "NS_OTH_STATUS/ISFORBIDDEN", (
			Magick::instance().getSname(who)));
	    return;
	}

	if (file && !Magick::instance().memoserv.Files())
	{
	    NSEND(mynick, nick, "MS_SYNTAX/FILEDISABLED");
	    return;
	}
    }

    if (file && !(Magick::instance().files.TempDirSize() == 0 ||
	mFile::DirUsage(Magick::instance().files.TempDir()) <=
		Magick::instance().files.TempDirSize()))
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

    DumpB();
    { WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    type = FileMap::MemoAttach;
    fileattach = file;
    service = mynick;
    sender = nick;
    recipiant = who;
    text = message;
    }

    { MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    while (Magick::instance().Pause())
	ACE_OS::sleep(1);
    timer = Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Magick::instance().memoserv.InFlight()));
    }
    DumpE();

    if (!silent)
    {
	RLOCK_IF(("NickServ", "live", nick.LowerCase(), "InFlight", "fileattach"),
		fileattach)
	    SEND(mynick, nick, "MS_COMMAND/PENDING_FILE", (
		ToHumanTime(Magick::instance().memoserv.InFlight(), nick)));
	else
	    SEND(mynick, nick, "MS_COMMAND/PENDING", (
		ToHumanTime(Magick::instance().memoserv.InFlight(), nick)));
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
    MCB(text);
    { WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "text"));
    text += message;
    }
    mstring *arg = NULL;
    { MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    if (timer)
    {
	CB(1, timer);
	if (Magick::instance().reactor().cancel_timer(timer,
		reinterpret_cast<const void **>(arg)) &&
	    arg != NULL)
	{
	    delete arg;
	}
	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);
	timer = Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh,
			new mstring(nick.LowerCase()),
			ACE_Time_Value(Magick::instance().memoserv.InFlight()));
	CE(1, timer);
    }}
    MCE(text);
    SEND(service, nick, "MS_COMMAND/CONTINUE", (
	    ToHumanTime(Magick::instance().memoserv.InFlight(), nick)));
}


// Cancel a memo or picture send.
void Nick_Live_t::InFlight_t::Cancel()
{
    NFT("Nick_Live_t::InFlight_t::Cancel");

    mstring *arg = NULL;
    { MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    if (timer)
    {
	MCB(timer);
	if (Magick::instance().reactor().cancel_timer(timer,
		reinterpret_cast<const void **>(arg)) &&
	    arg != NULL)
	{
	    delete arg;
	}
	timer = 0;
	MCE(timer);
    }}
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
	{ MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
	if (timer)
	{
	    MCB(timer);
	    if (Magick::instance().reactor().cancel_timer(timer,
		reinterpret_cast<const void **>(arg)) &&
		arg != NULL)
	    {
		delete arg;
	    }
	    timer = 0;
	    MCE(timer);
	}}
	RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
	if (Magick::instance().nickserv.IsStored(sender))
	{
	    if (!Magick::instance().nickserv.GetStored(sender).Host().empty())
	    {
		sender = Magick::instance().nickserv.GetStored(sender).Host();
	    }
	    if (Memo())
	    {
		if (IsChan(recipiant))
		{
		    if (Magick::instance().chanserv.IsStored(recipiant))
		    {
			News_t tmp(recipiant, sender, text);
			Magick::instance().memoserv.AddChannelNews(&tmp);

			Chan_Stored_t cstored = Magick::instance().chanserv.GetStored(recipiant);
			SEND(service, nick, "MS_COMMAND/SENT", (
			    recipiant, cstored.Founder()));
			LOG(LM_DEBUG, "MEMOSERV/SEND", (
				Magick::instance().nickserv.GetLive(sender).Mask(Nick_Live_t::N_U_P_H),
				recipiant));

			RLOCK3(("MemoServ", "channel", recipiant.LowerCase()));
			MemoServ::channel_news_t &newslist = Magick::instance().memoserv.GetChannel(recipiant);
			if (Magick::instance().chanserv.IsLive(recipiant))
			{
			    Chan_Live_t clive = Magick::instance().chanserv.GetLive(recipiant);
			    unsigned int i;
			    for(i=0; i<clive.Users(); i++)
			    {
				if (Magick::instance().nickserv.IsStored(clive.User(i)) &&
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
		    if (Magick::instance().nickserv.IsStored(recipiant))
		    {
			mstring realrecipiant = Magick::instance().nickserv.GetStored(recipiant).Host();
			if (realrecipiant.empty())
			    realrecipiant = recipiant;

			if (!(!filenum || Magick::instance().memoserv.FileSize() == 0 ||
			    Magick::instance().filesys.GetSize(FileMap::MemoAttach, filenum) <=
			    Magick::instance().memoserv.FileSize()))
			{
			    Magick::instance().filesys.EraseFile(FileMap::MemoAttach, filenum);
			    NSEND(service, nick, "MS_COMMAND/TOOBIG");
			}
			else if (!(!filenum || Magick::instance().files.MemoAttachSize() == 0 ||
			    Magick::instance().filesys.FileSysSize(FileMap::MemoAttach) <=
			    Magick::instance().files.MemoAttachSize()))
			{
			    Magick::instance().filesys.EraseFile(FileMap::MemoAttach, filenum);
			    NSEND(service, nick, "MS_COMMAND/NOSPACE");
			}
			else if (recipiant.IsSameAs(realrecipiant, true) ||
			    Magick::instance().nickserv.IsStored(realrecipiant))
			{
			    Memo_t tmp(realrecipiant, sender, text, filenum);
			    Magick::instance().memoserv.AddNickMemo(&tmp);

			    if (filenum)
			    {
				LOG(LM_DEBUG, "MEMOSERV/FILE", (
					Magick::instance().nickserv.GetLive(sender).Mask(Nick_Live_t::N_U_P_H),
					Magick::instance().filesys.GetName(FileMap::MemoAttach, filenum),
					fmstring("%08x", filenum),
					ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::MemoAttach, filenum)),
					realrecipiant));
			    }
			    SEND(service, nick, "MS_COMMAND/SENT", (
				recipiant, realrecipiant));

			    RLOCK2(("MemoServ", "nick", realrecipiant.LowerCase()));
			    MemoServ::nick_memo_t &memolist = Magick::instance().memoserv.GetNick(realrecipiant);
			    RLOCK3(("NickServ", "stored", realrecipiant.LowerCase()));
			    Nick_Stored_t nstored = Magick::instance().nickserv.GetStored(realrecipiant);
			    if (nstored.IsOnline())
				SEND(service, realrecipiant, "MS_COMMAND/NS_NEW", (
				    memolist.size(), service, memolist.size()));
			    unsigned int i;
			    for (i=0; i < nstored.Siblings(); i++)
			    {
				if (Magick::instance().nickserv.GetStored(nstored.Sibling(i)).IsOnline())
				{
				    SEND(service, nstored.Sibling(i), "MS_COMMAND/NS_NEW", (
					memolist.size(), service, memolist.size()));
				}
			    }
			}
			else if (File())
			{
			    Magick::instance().filesys.EraseFile(FileMap::MemoAttach, filenum);
			    NSEND(service, nick, "MS_COMMAND/CANCEL");
			}
		    }
		    else if (File())
		    {
			Magick::instance().filesys.EraseFile(FileMap::MemoAttach, filenum);
			NSEND(service, nick, "MS_COMMAND/CANCEL");
		    }
		}
	    }
	    else if (Picture())
	    {
		if (filenum && !(Magick::instance().memoserv.FileSize() == 0 ||
		    Magick::instance().filesys.GetSize(FileMap::Picture, filenum) <=
		    Magick::instance().nickserv.PicSize()))
		{
		    Magick::instance().filesys.EraseFile(FileMap::Picture, filenum);
		    NSEND(service, nick, "NS_YOU_COMMAND/TOOBIG");
		}
		else if (filenum && !(Magick::instance().files.PictureSize() == 0 ||
		    Magick::instance().filesys.FileSysSize(FileMap::Picture) <=
		    Magick::instance().files.PictureSize()))
		{
		    Magick::instance().filesys.EraseFile(FileMap::Picture, filenum);
		    NSEND(service, nick, "NS_YOU_COMMAND/NOSPACE");
		}
		else if (filenum)
		{
		    Magick::instance().nickserv.GetStored(sender).GotPic(filenum);
		    NSEND(service, nick, "NS_YOU_COMMAND/SAVED");
		    LOG(LM_DEBUG, "NICKSERV/PICTURE_ADD", (
			Magick::instance().nickserv.GetLive(sender).Mask(Nick_Live_t::N_U_P_H),
			sender, fmstring("%08x", filenum),
			ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::Picture, filenum))));
		}
		else
		{
		    SEND(service, nick, "DCC/FAILED", ( "GET"));
		}
	    }
	    else if (Public())
	    {
		if (filenum && !(Magick::instance().files.PublicSize() == 0 ||
		    Magick::instance().filesys.FileSysSize(FileMap::Public) <=
		    Magick::instance().files.PublicSize()))
		{
		    Magick::instance().filesys.EraseFile(FileMap::Public, filenum);
		    NSEND(service, nick, "DCC/NOSPACE");
		}
		else if (filenum)
		{
		    SEND(service, nick, "LIST/ADD", (
    			Magick::instance().filesys.GetName(FileMap::Public, filenum),
    			Magick::instance().getMessage(nick,"LIST/FILES")));
		    Magick::instance().filesys.SetPriv(FileMap::Public, filenum, text);
		    LOG(LM_DEBUG, "SERVMSG/FILE_ADD", (
			Magick::instance().nickserv.GetLive(sender).Mask(Nick_Live_t::N_U_P_H),
			Magick::instance().filesys.GetName(FileMap::Public, filenum),
			fmstring("%08x", filenum),
			ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::Public, filenum)),
			((Magick::instance().filesys.GetPriv(FileMap::Public, filenum).empty()) ?
				"ALL" : Magick::instance().filesys.GetPriv(FileMap::Public, filenum).c_str())));
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
    if (!Magick::instance().nickserv.IsStored(nick))
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/ISNOTSTORED");
	return;
    }
    else if (Magick::instance().nickserv.PicExt().empty())
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/PICDISABLED");
	return;
    }

    if (!(Magick::instance().files.TempDirSize() == 0 ||
	mFile::DirUsage(Magick::instance().files.TempDir()) <=
		Magick::instance().files.TempDirSize()))
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

    DumpB();
    { WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    type = FileMap::Picture;
    fileattach = true;
    sender = nick;
    service = mynick;
    }

    { MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    while (Magick::instance().Pause())
	ACE_OS::sleep(1);
    timer = Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Magick::instance().memoserv.InFlight()));
    }
    DumpE();

    NSEND(mynick, nick, "NS_YOU_COMMAND/PENDING");
}


void Nick_Live_t::InFlight_t::Public(const mstring& mynick, const mstring& committees)
{
    FT("Nick_Live_t::InFlight_t::Public", (mynick, committees));
    if (!Magick::instance().nickserv.IsStored(nick))
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/ISNOTSTORED");
	return;
    }

    if (!(Magick::instance().files.TempDirSize() == 0 ||
	mFile::DirUsage(Magick::instance().files.TempDir()) <=
		Magick::instance().files.TempDirSize()))
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

    DumpB();
    { WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    type = FileMap::Public;
    fileattach = true;
    sender = nick;
    service = mynick;
    text = committees;
    }

    { MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    while (Magick::instance().Pause())
	ACE_OS::sleep(1);
    timer = Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Magick::instance().memoserv.InFlight()));
    }
    DumpE();

    NSEND(mynick, nick, "NS_YOU_COMMAND/PUB_PENDING");
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
    if (Magick::instance().operserv.Akill_find(i_user + "@" + i_host))
    {
	mstring reason(Magick::instance().operserv.Akill->Value().second);
	// Do this cos it will be removed when we KILL,
	// and we dont wanna get out of touch.
	Magick::instance().operserv.AddHost(i_host);
	i_server.erase();
	i_realname = reason;
	LOG(LM_INFO, "OTHER/KILL_AKILL", (
		Mask(N_U_P_H), Magick::instance().operserv.Akill->Entry(),
		reason));
	Magick::instance().server.AKILL(Magick::instance().operserv.Akill->Entry(), reason,
			Magick::instance().operserv.Akill->Value().first -
				Magick::instance().operserv.Akill->Last_Modify_Time().SecondsSince(),
			Magick::instance().operserv.Akill->Last_Modifier());
	return;
    }}

    // User triggered CLONE protection, No server will kill
    if (Magick::instance().operserv.AddHost(i_host))
    {
	LOG(LM_INFO, "OTHER/KILL_CLONE", (
		Mask(N_U_P_H)));
	i_server.erase();
	i_realname = Magick::instance().operserv.Def_Clone();
	return;
    }

    if (Magick::instance().nickserv.IsStored(i_Name))
    {
	if (IsRecognized() && !Magick::instance().nickserv.GetStored(i_Name).Secure())
	    Magick::instance().nickserv.GetStored(i_Name).Signon(i_realname, Mask(U_P_H).After("!"));
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
    if (Magick::instance().chanserv.IsLive(chan))
    {
	joined = Magick::instance().chanserv.GetLive(chan).Join(i_Name);
    }
    else
    {
	Chan_Live_t tmp(chan, i_Name);
	Magick::instance().chanserv.AddLive(&tmp);
    }
    // We do this seperately coz we require initialisation of
    // the channel to be completed.
    if (joined)
    {
	{ WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
	MCB(joined_channels.size());
	joined_channels.insert(chan.LowerCase());
	MCE(joined_channels.size());
	}
	if (Magick::instance().chanserv.IsStored(chan))
	    Magick::instance().chanserv.GetStored(chan).Join(i_Name);
    }
    mMessage::CheckDependancies(mMessage::ChanExists, chan);
    mMessage::CheckDependancies(mMessage::UserInChan, chan, i_Name);
}


void Nick_Live_t::Part(const mstring& chan)
{
    FT("Nick_Live_t::Part", (chan));

    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    MCB(joined_channels.size());
    joined_channels.erase(chan.LowerCase());
    MCE(joined_channels.size());
    }

    unsigned int res = 0;
    if (Magick::instance().chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
	res = Magick::instance().chanserv.GetLive(chan).Part(i_Name);
	if (!res)
	    Magick::instance().chanserv.RemLive(chan);
    }
    else
    {
	LOG(LM_TRACE, "ERROR/REC_FORNONCHAN", (
		"PART", i_Name, chan));
    }

    mMessage::CheckDependancies(mMessage::UserNoInChan, chan, i_Name);
    if (!res)
	mMessage::CheckDependancies(mMessage::ChanNoExists, chan);
}

void Nick_Live_t::Kick(const mstring& kicker, const mstring& chan)
{
    FT("Nick_Live_t::Kick", (kicker, chan));

    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    MCB(joined_channels.size());
    joined_channels.erase(chan.LowerCase());
    MCE(joined_channels.size());
    }

    unsigned int res = 0;
    if (Magick::instance().chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
	res = Magick::instance().chanserv.GetLive(chan).Kick(i_Name, kicker);
	if (!res)
	    Magick::instance().chanserv.RemLive(chan);
	
    }
    else
    {
	LOG(LM_ERROR, "ERROR/REC_FORNONCHAN", (
		"KICK", kicker, chan));
    }

    mMessage::CheckDependancies(mMessage::UserNoInChan, chan, i_Name);
    if (!res)
	mMessage::CheckDependancies(mMessage::ChanNoExists, chan);
}


void Nick_Live_t::Quit(const mstring& reason)
{
    FT("Nick_Live_t::Quit", (reason));

    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    if (!(IsServices() || HasMode("o")))
	Magick::instance().operserv.RemHost(i_host);
    }

    // Check if we're currently being TEMP ignored ...
    { MLOCK(("OperServ","Ignore"));
    if (Magick::instance().operserv.Ignore_find(Mask(N_U_P_H)))
    {
	if (Magick::instance().operserv.Ignore->Value() != true)
	{
	    Magick::instance().operserv.Ignore_erase();
	}
    }}

    set<mstring> jc;
    set<mstring>::iterator c;
    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    jc = joined_channels;
    joined_channels.clear();
    }
    for (c=jc.begin(); c!=jc.end(); c++)
	Part(*c);

    unsigned long i;
    { RLOCK(("DCC"));
    if (Magick::instance().dcc != NULL)
    {
	vector<unsigned long> dccs = Magick::instance().dcc->GetList(i_Name);
        for (i=0; i<dccs.size(); i++)
	    Magick::instance().dcc->Cancel(dccs[i], true);
    }}
    if (InFlight.Exists())
	InFlight.End(0u);

    // Ensure our failures dont affect future users ...
    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "try_chan_ident"));
    for (i=0; i<try_chan_ident.size(); i++)
	if (Magick::instance().chanserv.IsStored(try_chan_ident[i]))
	    Magick::instance().chanserv.GetStored(try_chan_ident[i]).Quit(i_Name);
    }

    if (Magick::instance().nickserv.IsStored(i_Name) &&
	Magick::instance().nickserv.GetStored(i_Name).IsOnline())
	Magick::instance().nickserv.GetStored(i_Name).Quit(reason);
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
    if (Magick::instance().operserv.Ignore_size())
    {
	if (Magick::instance().operserv.Ignore_find(Mask(N_U_P_H)))
	{
	    // IF we havnt ignored for long enough yet, or its perminant ...
	    if (Magick::instance().operserv.Ignore->Last_Modify_Time().SecondsSince() <= Magick::instance().operserv.Ignore_Time()
		|| Magick::instance().operserv.Ignore->Value() == true)
	    {
		RET(true);
	    }
	    else
	    {
		Magick::instance().operserv.Ignore_erase();
	    }
	}
    }}

    // Clean up previous entries and push current entry
    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "last_msg_times"));
    MCB(last_msg_times.size());
    while (last_msg_times.size() && last_msg_times[0u].SecondsSince() > Magick::instance().operserv.Flood_Time())
	last_msg_times.erase(last_msg_times.begin());
    last_msg_times.push_back(mDateTime::CurrentDateTime());
    MCE(last_msg_times.size());
    }

    // Check if we just triggered ignore.
    RLOCK_IF(("NickServ", "live", i_Name.LowerCase(), "last_msg_times"),
	last_msg_times.size() > Magick::instance().operserv.Flood_Msgs())
    {
	RLOCK(("NickServ", "live", i_Name.LowerCase(), "flood_triggered_times"));
	CB(1, flood_triggered_times);
	{ WLOCK(("NickServ", "live", i_Name.LowerCase(), "flood_triggered_times"));
	flood_triggered_times++;
	}
	// Add To ignore, they're naughty.
	if (flood_triggered_times >= Magick::instance().operserv.Ignore_Limit())
	{
	    Magick::instance().operserv.Ignore_insert(Mask(Magick::instance().operserv.Ignore_Method()), true, i_Name);
	    SEND(Magick::instance().servmsg.FirstName(), i_Name, "ERR_SITUATION/IGNORE_TRIGGER", (
			Magick::instance().operserv.Flood_Msgs(), ToHumanTime(Magick::instance().operserv.Flood_Time(), i_Name)));
	    SEND(Magick::instance().servmsg.FirstName(), i_Name, "ERR_SITUATION/PERM_IGNORE", (
			Magick::instance().operserv.Ignore_Limit()));
	    LOG(LM_NOTICE, "OTHER/PERM_IGNORE", (
			Mask(N_U_P_H)));
	    ANNOUNCE(Magick::instance().servmsg.FirstName(), "MISC/FLOOD_PERM", (
			i_Name));
	}
	else
	{
	    Magick::instance().operserv.Ignore_insert(Mask(Magick::instance().operserv.Ignore_Method()), false, i_Name);
	    SEND(Magick::instance().servmsg.FirstName(), i_Name, "ERR_SITUATION/IGNORE_TRIGGER", (
			Magick::instance().operserv.Flood_Msgs(), ToHumanTime(Magick::instance().operserv.Flood_Time(), i_Name)));
	    SEND(Magick::instance().servmsg.FirstName(), i_Name, "ERR_SITUATION/TEMP_IGNORE", (
			ToHumanNumber(flood_triggered_times), Magick::instance().operserv.Ignore_Limit(),
			ToHumanTime(Magick::instance().operserv.Ignore_Time(), i_Name)));
	    LOG(LM_NOTICE, "OTHER/TEMP_IGNORE", (
			Mask(N_U_P_H)));
	    ANNOUNCE(Magick::instance().servmsg.FirstName(), "MISC/FLOOD_TEMP", (
			i_Name));
	}
 
 	CE(1, flood_triggered_times);
	retval = true;
    }

    RET(retval);
}


set<mstring> Nick_Live_t::Name(const mstring& in)
{
    FT("Nick_Live_t::Name", (in));

    InFlight.ChgNick(in);
    set<mstring> wason;

    { RLOCK(("NickServ", "live", in.LowerCase()));
    if (i_Name.IsSameAs(in, true))
    {
	i_Name = in;
	NRET(set<mstring>, wason);
    }}

    unsigned long i;

    // Store what committee's we WERE on ...
    // This is needed to send logon notices ONLY for committees
    // we have joined by a nick change.
    CommServ::list_t::iterator iter2;
    { RLOCK2(("CommServ", "list"));
    for (iter2 = Magick::instance().commserv.ListBegin(); iter2 != Magick::instance().commserv.ListEnd();
								iter2++)
    {
	RLOCK3(("CommServ", "list", iter2->first));
	if (iter2->second.IsOn(i_Name))
	    wason.insert(iter2->first);
    }}

    { RLOCK(("DCC"));
    if (Magick::instance().dcc != NULL)
    {
	vector<unsigned long> dccs = Magick::instance().dcc->GetList(i_Name);
	for (i=0; i<dccs.size(); i++)
	    Magick::instance().dcc->GetXfers(dccs[i]).ChgNick(in);
    }}

    // Carry over failed attempts (so /nick doesnt cure all!)
    // We dont care if it doesnt exist, they can drop channels *shrug*
    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "try_chan_ident"));
    for (i=0; i<try_chan_ident.size(); i++)
    {
	if (Magick::instance().chanserv.IsStored(try_chan_ident[i]))
	    Magick::instance().chanserv.GetStored(try_chan_ident[i]).ChgAttempt(i_Name, in);
    }}

    MCB(i_Name);
    if (Magick::instance().nickserv.IsStored(i_Name))
    {
	// We are not related (by brotherhood, or parentage)
	if (!(Magick::instance().nickserv.GetStored(i_Name).IsSibling(in) ||
	    Magick::instance().nickserv.GetStored(i_Name).Host().IsSameAs(in, true)))
	{
	    WLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
	    WLOCK2(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
	    WLOCK3(("NickServ", "live", i_Name.LowerCase(), "failed_paswds"));
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
	Magick::instance().nickserv.GetStored(i_Name).ChgNick(in);
    }

    // WooHoo, we have a new nick!
    mstring oldnick(i_Name);
    i_Name = in;
    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_My_Signon_Time"));
    CB(1, i_My_Signon_Time);
    i_My_Signon_Time = mDateTime::CurrentDateTime();
    CE(1, i_My_Signon_Time);
    }

    set<mstring> jc;
    set<mstring>::iterator iter;
    vector<mstring> chunked;
    // Rename ourselves in all channels ...
    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    jc = joined_channels;
    }
    for (iter=jc.begin(); iter!=jc.end(); iter++)
    {
	if (Magick::instance().chanserv.IsLive(*iter))
	{
	    Magick::instance().chanserv.GetLive(*iter).ChgNick(oldnick, i_Name);
	}
	else
	{
	    chunked.push_back(*iter);
	    LOG(LM_ERROR, "ERROR/REC_FORNOTINCHAN", (
		"NICK", oldnick, *iter));
	}
    }

    // Clean up non-existant channels ...
    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    CB(5, joined_channels.size());
    for (i=0; i<chunked.size(); i++)
	joined_channels.erase(chunked[i]);
    CE(5, joined_channels.size());
    }
    MCE(i_Name);

    if (Magick::instance().nickserv.IsStored(i_Name))
    {
	if (IsRecognized() && !Magick::instance().nickserv.GetStored(i_Name).Secure())
	    Magick::instance().nickserv.GetStored(i_Name).Signon(i_realname, Mask(U_P_H).After("!"));
    }

    NRET(set<mstring>, wason);
}


void Nick_Live_t::SendMode(const mstring& in)
{
    FT("Nick_Live_t::SendMode", (in));

    if (IsServices())
    {
	Magick::instance().server.MODE(i_Name, in);
    }
    else if (!Magick::instance().server.proto.SVSMODE().empty())
    {
	Magick::instance().server.SVSMODE(Magick::instance().nickserv.FirstName(), i_Name, in);
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
		    { RLOCK(("CommServ", "list"));
		    for (iter2 = Magick::instance().commserv.ListBegin(); iter2 != Magick::instance().commserv.ListEnd();
								iter2++)
		    {
			RLOCK2(("CommServ", "list", iter2->first));
			if (iter2->second.IsOn(i_Name))
			    wason.insert(iter2->first);
		    }}

		    modes += in[i];
		    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
		    Magick::instance().operserv.RemHost(i_host);
		    }
		    // IF we are SecureOper and NOT (on oper list && recoznized)
		    // OR user is on OperDeny and NOT (on sadmin list && recognized)
		    // Yeah, one UUUUUUGLY if.
		    MLOCK_IF(("OperServ", "OperDeny"),
			(Magick::instance().operserv.SecureOper() &&
			!(Magick::instance().nickserv.IsStored(i_Name) &&
			Magick::instance().nickserv.GetStored(i_Name).IsOnline() &&
			Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
			Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsIn(i_Name))) ||
			(Magick::instance().operserv.OperDeny_find(Mask(N_U_P_H)) &&
			!(Magick::instance().nickserv.IsStored(i_Name) &&
			Magick::instance().nickserv.GetStored(i_Name).IsOnline() &&
			Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
			Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name()).IsIn(i_Name))))
		    {
			if (!Magick::instance().server.proto.SVSMODE().empty())
			{
			    SendMode("-o");
			    NSEND(Magick::instance().operserv.FirstName(), i_Name, "OS_STATUS/ISOPERDENY");
			}
			else
			{
			    Magick::instance().server.KILL(Magick::instance().operserv.FirstName(),
		    	    	i_Name, Magick::instance().getMessage(i_Name, "MISC/KILL_OPERDENY"));
			    return;
			}
		    }
		    else
		    {
			mstring setmode;
			{ RLOCK(("CommServ", "list"));
			for (iter2 = Magick::instance().commserv.ListBegin(); iter2 != Magick::instance().commserv.ListEnd();
								iter2++)
			{
			    RLOCK2(("CommServ", "list", iter2->first));
			    if (iter2->second.IsOn(i_Name) && wason.find(iter2->first) == wason.end())
			    {
				if (!Magick::instance().server.proto.SVSMODE().empty())
				{
				    if (iter2->first == Magick::instance().commserv.ALL_Name())
					setmode += Magick::instance().commserv.ALL_SetMode();
				    else if (iter2->first == Magick::instance().commserv.REGD_Name())
					setmode += Magick::instance().commserv.REGD_SetMode();
				    else if (iter2->first == Magick::instance().commserv.OPER_Name())
					setmode += Magick::instance().commserv.OPER_SetMode();
				    else if (iter2->first == Magick::instance().commserv.ADMIN_Name())
					setmode += Magick::instance().commserv.ADMIN_SetMode();
				    else if (iter2->first == Magick::instance().commserv.SOP_Name())
					setmode += Magick::instance().commserv.SOP_SetMode();
				    else if (iter2->first == Magick::instance().commserv.SADMIN_Name())
					setmode += Magick::instance().commserv.SADMIN_SetMode();
				}

				for (iter2->second.message = iter2->second.MSG_begin();
					iter2->second.message != iter2->second.MSG_end(); iter2->second.message++)
				{
				    Magick::instance().servmsg.send(i_Name, "[" + IRC_Bold + iter2->first + IRC_Off +
					    "] " + iter2->second.message->Entry());
				}
			    }
			}}
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
		    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
		    Magick::instance().operserv.AddHost(i_host);
		    }
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
	Magick::instance().operserv.RemHost(i_host);
    }

    set<mstring> jc;
    set<mstring>::iterator c;
    vector<mstring> chunked;
    { RLOCK3(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    jc = joined_channels;
    }
    for (c=jc.begin(); c!=jc.end(); c++)
	if (Magick::instance().chanserv.IsLive(*c))
	    Magick::instance().chanserv.GetLive(*c).SquitUser(i_Name);
	else
	{
	    chunked.push_back(*c);
	    LOG(LM_ERROR, "ERROR/REC_FORNONCHAN", (
		"SQUIT", i_Name, *c));
	}

    // Clean up non-existant channels ...
    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    MCB(joined_channels.size());
    for (unsigned int i=0; i<chunked.size(); i++)
	joined_channels.erase(chunked[i]);
    MCE(joined_channels.size());
    }
}


void Nick_Live_t::ClearSquit(const mstring& inmodes)
{
    NFT("Nick_Live_t::ClearSquit");

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

    set<mstring> jc;
    set<mstring>::iterator i;
    { WLOCK3(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    CB(2, joined_channels.size());
    jc = joined_channels;
    joined_channels.clear();
    CE(2, joined_channels.size());
    }
    for (i=jc.begin(); i!=jc.end(); i++)
	if (Magick::instance().chanserv.IsLive(*i))
	    Magick::instance().chanserv.GetLive(*i).UnSquitUser(i_Name);
	else
	{
	    LOG(LM_ERROR, "ERROR/REC_FORNONCHAN", (
		"UNSQUIT", i_Name, *i));
	}

    RLOCK_IF(("NickServ", "live", i_Name.LowerCase(), "i_host"),
	!IsServices() && Magick::instance().operserv.AddHost(i_host))
    {
	LOG(LM_INFO, "OTHER/KILL_CLONE", (
		Mask(N_U_P_H)));
	Magick::instance().server.KILL(Magick::instance().nickserv.FirstName(), i_Name,
			Magick::instance().operserv.Def_Clone());
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
    if (Magick::instance().chanserv.IsStored(channel))
    {
	unsigned int failtimes = Magick::instance().chanserv.GetStored(channel).CheckPass(i_Name, password);
	if (!failtimes)
	{
	    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
	    MCB(chans_founder_identd.size());
	    chans_founder_identd.insert(channel.LowerCase());
	    MCE(chans_founder_identd.size());
	    }
	    retval = parseMessage(Magick::instance().getMessage(i_Name, "CS_COMMAND/IDENTIFIED"),
						mVarArray(channel));
	}
	else
	{
	    // Keep record of all channel attempts, so that when
	    // we change nick or quit, we can let chanserv know.
	    vector<mstring>::iterator iter;
	    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "try_chan_ident"));
	    for (iter = try_chan_ident.begin(); iter != try_chan_ident.end() &&
				!iter->IsSameAs(channel); iter++) ;
	    if (iter == try_chan_ident.end())
	    {
		MCB(try_chan_ident.size());
		try_chan_ident.push_back(channel.LowerCase());
		MCE(try_chan_ident.size());
	    }}

	    if (failtimes >= Magick::instance().chanserv.Passfail())
	    {
		LOG(LM_NOTICE, "OTHER/KILL_CHAN_PASS", (
			Mask(N_U_P_H), channel));
		Magick::instance().server.KILL(Magick::instance().nickserv.FirstName(), i_Name,
			Magick::instance().getMessage(i_Name, "MISC/KILL_PASS_FAIL"));
		RET("");
	    }
	    else
	    {
		LOG(LM_INFO, "CHANSERV/IDENTIFY_FAILED", (
			Mask(N_U_P_H), channel));
		retval = parseMessage(Magick::instance().getMessage(i_Name, "ERR_SITUATION/CHAN_WRONG_PASS"),
								mVarArray(channel));
	    }
	}
    }
    else
    {
	retval = parseMessage(Magick::instance().getMessage(i_Name, "CS_STATUS/ISNOTSTORED"),
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
    RLOCK_IF(("NickServ", "live", i_Name.LowerCase(), "identified"),
	identified)
    {
	retval = Magick::instance().getMessage(i_Name, "NS_YOU_STATUS/IDENTIFIED");
    }
    else if (Magick::instance().nickserv.IsStored(i_Name))
    {
	if (Magick::instance().nickserv.GetStored(i_Name).CheckPass(password))
	{
	    set<mstring> wason;
	    CommServ::list_t::iterator iter;
	    { RLOCK(("CommServ", "list"));
	    for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd();
									    iter++)
	    {
		RLOCK2(("CommServ", "list", iter->first));
		if (iter->second.IsOn(i_Name))
		    wason.insert(iter->first);

	    }}
	    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
	    WLOCK2(("NickServ", "live", i_Name.LowerCase(), "failed_passwds"));
	    MCB(identified);
	    CB(1, failed_passwds);
	    identified = true;
	    failed_passwds = 0;
	    CE(1, failed_passwds);
	    MCE(identified);
	    }
	    if (Magick::instance().nickserv.GetStored(i_Name).Secure())
		Magick::instance().nickserv.GetStored(i_Name).Signon(i_realname, Mask(U_P_H).After("!"));

	    // Send notices for committees we were NOT on
	    { RLOCK(("CommServ", "list"));
	    for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd();
									    iter++)
	    {
		RLOCK2(("CommServ", "list", iter->first));
		if (iter->second.IsOn(i_Name) && wason.find(iter->first) == wason.end())
		{
		    for (iter->second.message = iter->second.MSG_begin();
			iter->second.message != iter->second.MSG_end(); iter->second.message++)
		    {
			Magick::instance().servmsg.send(i_Name, "[" + IRC_Bold + iter->first +
					IRC_Off + "] " + iter->second.message->Entry());
		    }
		}
	    }}
	    retval = Magick::instance().getMessage(i_Name, "NS_YOU_COMMAND/IDENTIFIED");
	}
	else
	{
	    { WLOCK2(("NickServ", "live", i_Name.LowerCase(), "failed_passwds"));
	    MCB(failed_passwds);
	    failed_passwds++;
	    MCE(failed_passwds);
	    }
	    RLOCK2_IF(("NickServ", "live", i_Name.LowerCase(), "failed_passwds"),
		failed_passwds >= Magick::instance().nickserv.Passfail())
	    {
		LOG(LM_NOTICE, "OTHER/KILL_NICK_PASS", (
			Mask(N_U_P_H), i_Name));
		Magick::instance().server.KILL(Magick::instance().nickserv.FirstName(), i_Name,
			Magick::instance().getMessage(i_Name, "MISC/KILL_PASS_FAIL"));
		RET("");
	    }
	    else
	    {
		LOG(LM_INFO, "NICKSERV/IDENTIFY_FAILED", (
			Mask(N_U_P_H), i_Name));
		retval = Magick::instance().getMessage(i_Name, "ERR_SITUATION/NICK_WRONG_PASS");
	    }
	}
    }
    else
    {
	retval = Magick::instance().getMessage(i_Name, "NS_YOU_STATUS/ISNOTSTORED");
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
    if (Magick::instance().nickserv.IsStored(i_Name))
    {
	retval = Magick::instance().nickserv.GetStored(i_Name).IsAccess(Mask(U_P_H).After("!"));
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
    WLOCK3(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
    MCB(i_LastRealName);
    CB(1, i_LastMask);
    CB(2, i_LastSeenTime);
    i_LastRealName = realname;
    i_LastMask = mask;
    i_LastSeenTime = mDateTime::CurrentDateTime();
    CE(1, i_LastMask);
    CE(2, i_LastSeenTime);
    MCE(i_LastRealName);
    }

    mstring who = Host();
    if (who.empty())
	who = i_Name;
    if (Magick::instance().memoserv.IsNick(who))
    {
	unsigned int count = Magick::instance().memoserv.NickMemoCount(who);
	if (count)
	    SEND(Magick::instance().memoserv.FirstName(), i_Name, "MS_STATUS/NS_UNREAD", (
		count, Magick::instance().memoserv.FirstName()));
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


void Nick_Stored_t::defaults()
{
    NFT("Nick_Stored_t::defaults");

    setting.Protect = Magick::instance().nickserv.DEF_Protect();
    setting.Secure = Magick::instance().nickserv.DEF_Secure();
    setting.NoMemo = Magick::instance().nickserv.DEF_NoMemo();
    setting.Private = Magick::instance().nickserv.DEF_Private();
    setting.PRIVMSG = Magick::instance().nickserv.DEF_PRIVMSG();
    setting.Language = Magick::instance().nickserv.DEF_Language().UpperCase();
    setting.NoExpire = Magick::instance().nickserv.DEF_NoExpire();
    setting.Forbidden = false;
    setting.Picture = 0;

    lock.Protect = false;
    lock.Secure = false;
    lock.NoMemo = false;
    lock.Private = false;
    lock.PRIVMSG = false;
    lock.Language = false;

}


Nick_Stored_t::Nick_Stored_t()
{
    NFT("Nick_Stored_t::Nick_Stored_t");
    defaults();
}


Nick_Stored_t::Nick_Stored_t(const mstring& nick, const mstring& password)
	: i_Name(nick), i_RegTime(mDateTime::CurrentDateTime())
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick, password));

    defaults();
    Password(password);
    if (Magick::instance().nickserv.IsLive(i_Name))
    {
	i_LastRealName = Magick::instance().nickserv.GetLive(i_Name).RealName();
	i_LastMask = Magick::instance().nickserv.GetLive(i_Name).Mask(Nick_Live_t::U_P_H).After("!");
	i_LastSeenTime = mDateTime::CurrentDateTime();
    }
    DumpE();
}


Nick_Stored_t::Nick_Stored_t(const mstring& nick)
	: i_Name(nick), i_RegTime(mDateTime::CurrentDateTime())
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick.LowerCase()));
    defaults();
    setting.Forbidden = true;
    DumpE();
} 


Nick_Stored_t::Nick_Stored_t(const mstring& nick, const mDateTime& regtime,
	const Nick_Stored_t &in)
	: i_Name(nick), i_RegTime(regtime), i_Host(in.i_Name.LowerCase())
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick, "(const Nick_Stored_t &) in"));

    defaults();
    if (Magick::instance().nickserv.IsLive(i_Name))
    {
	i_LastRealName = Magick::instance().nickserv.GetLive(i_Name).RealName();
	i_LastMask = Magick::instance().nickserv.GetLive(i_Name).Mask(Nick_Live_t::U_P_H).After("!");
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

    bool host = Host().empty();
    // When we go, we take all our slaves with us!
    unsigned long i, dropped = 1;
    if (host)
    {
	while(Siblings())
	{
	    mstring nick = Sibling(0);
	    if (!nick.empty())
	    {
		dropped += Magick::instance().nickserv.GetStored(nick).Drop();
		Magick::instance().nickserv.RemStored(nick);
	    }
	}
    }
    else
    {
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	Magick::instance().nickserv.GetStored(i_Host).i_slaves.erase(i_Name.LowerCase());
    }

    if (Magick::instance().memoserv.IsNick(i_Name))
	Magick::instance().memoserv.RemNick(i_Name);

    // Now we go for our channels ...
    vector<mstring> killchans;
    ChanServ::stored_t::iterator iter;
    { RLOCK(("ChanServ", "stored"));
    for (iter = Magick::instance().chanserv.StoredBegin();
	    iter != Magick::instance().chanserv.StoredEnd(); iter++)
    {
	RLOCK2(("ChanServ", "stored", iter->first));
	if (iter->second.Founder().IsSameAs(i_Name, true))
	{
	    // If we're not host, and own the channel, give it to our host
	    // Otherwise it falls to the cofounder, or gets removed.
	    if (!host)
	    {
		iter->second.Founder(i_Host);
	    }
	    else if (!iter->second.CoFounder().empty() &&
		Magick::instance().nickserv.IsStored(iter->second.CoFounder()))
	    {
		iter->second.Founder(iter->second.CoFounder());
	    }
	    else
		killchans.push_back(iter->first);
	}
	else if (iter->second.CoFounder().IsSameAs(i_Name, true))
	{
	    if (!host)
		iter->second.CoFounder(i_Host);
	    else
		iter->second.CoFounder("");
	}
	MLOCK(("ChanServ", "stored", iter->first, "Access"));
	if (iter->second.Access_find(i_Name))
	{
	    // It must be specifically ours, not a sibling/hosts
	    if (iter->second.Access->Entry().IsSameAs(i_Name, true))
	    {
		// If we're not a host, give access to the host,
		// else just remove the access (ie. we are host).
		if (!host)
		{
		    long value = iter->second.Access->Value();
		    mstring modifier = iter->second.Access->Last_Modifier();
		    mDateTime modtime = iter->second.Access->Last_Modify_Time();
		    iter->second.Access_erase();
		    iter->second.Access_insert(i_Host, value, modifier, modtime);
		}
		else
		    iter->second.Access_erase();
	    }
	}
    }}

    for (i=0; i<killchans.size(); i++)
    {
	Magick::instance().chanserv.RemStored(killchans[i]);
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
    i_AIM=in.i_AIM;
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
    setting.Protect=in.setting.Protect;
    lock.Protect=in.lock.Protect;
    setting.Secure=in.setting.Secure;
    lock.Secure=in.lock.Secure;
    setting.NoMemo=in.setting.NoMemo;
    lock.NoMemo=in.lock.NoMemo;
    setting.Private=in.setting.Private;
    lock.Private=in.lock.Private;
    setting.PRIVMSG=in.setting.PRIVMSG;
    lock.PRIVMSG=in.lock.PRIVMSG;
    setting.Language=in.setting.Language;
    lock.Language=in.lock.Language;
    setting.NoExpire=in.setting.NoExpire;
    setting.Forbidden=in.setting.Forbidden;
    setting.Picture=in.setting.Picture;
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
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).Email();
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
	Magick::instance().nickserv.GetStored(i_Host).Email(in);
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
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).URL();
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
	Magick::instance().nickserv.GetStored(i_Host).URL(in);
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
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).ICQ();
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
	Magick::instance().nickserv.GetStored(i_Host).ICQ(in);
    }
}


mstring Nick_Stored_t::AIM()
{
    NFT("Nick_Stored_t::AIM");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_AIM"));
	RET(i_AIM);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).AIM();
	RET(retval);
    }
}


void Nick_Stored_t::AIM(const mstring& in)
{
    FT("Nick_Stored_t::AIM", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_AIM"));
	MCB(i_AIM);
	i_AIM = in;
	MCE(i_AIM);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).AIM(in);
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
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).Description();
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
	Magick::instance().nickserv.GetStored(i_Host).Description();
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
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).Comment();
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
	Magick::instance().nickserv.GetStored(i_Host).Comment(in);
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
	Magick::instance().nickserv.GetStored(i_Host).Suspend(name);
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
	Magick::instance().nickserv.GetStored(i_Host).UnSuspend();
    }
}

mstring Nick_Stored_t::Host()
{
    NFT("Nick_Stored_t::Host");
    mstring retval;
    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Host"));
    if (!i_Host.empty())
	if (!Magick::instance().nickserv.IsStored(i_Host))
	{
	    LOG(LM_ERROR, "ERROR/HOST_NOTREGD", (
		i_Host, i_Name));
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Host"));
	    MCB(i_Host);
	    i_Host.erase();
	    MCE(i_Host);
	}
	else
	    retval = Magick::instance().nickserv.GetStored(i_Host).Name();
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
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).Password();
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
#ifdef GETPASS
	i_Password = in;
#else
	char newpass[33];
	mHASH(in.c_str(), in.length(), newpass);
	i_Password = newpass;
#endif
	MCE(i_Password);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).Password(in);
    }
}

bool Nick_Stored_t::CheckPass(const mstring& password)
{
    FT("Nick_Stored_t::CheckPass", (password));
    if (Host().empty())
    {
#ifdef GETPASS
	mstring check(password);
#else
	char chkpass[33];
	mHASH(password.c_str(), password.length(), chkpass);
	mstring check(chkpass);
#endif
	RLOCK_IF(("NickServ", "stored", i_Name.LowerCase(), "i_Password"),
	    i_Password == check)
	{
	    RET(true);
	}
	RET(false);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).CheckPass(password);
	RET(retval);
    }
}


bool Nick_Stored_t::Slave(const mstring& nick, const mstring& password,
	const mDateTime& regtime)
{
    FT("Nick_Stored_t::Slave", (nick, password, regtime));

    if (Host().empty())
    {
	{ RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Forbidden"));
	if (setting.Forbidden || !CheckPass(password))
	{
	    RET(false);
	}}

	if (Magick::instance().nickserv.IsStored(nick))
	{
	    ChangeOver(nick);
	    Magick::instance().nickserv.RemStored(nick);
	}

	Nick_Stored_t tmp(nick, regtime, *this);
	Magick::instance().nickserv.AddStored(&tmp);
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	MCB(i_slaves.size());
	i_slaves.insert(nick.LowerCase());
	MCE(i_slaves.size());
	RET(true);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).Slave(nick, password, regtime);
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
	retval = Magick::instance().nickserv.GetStored(i_Host).Siblings();
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
	{ RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	for (i=0, iter=i_slaves.begin(); iter!=i_slaves.end(); iter++, i++)
	{
	    if (Magick::instance().nickserv.IsStored(*iter))
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
	}}
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
	retval = Magick::instance().nickserv.GetStored(i_Host).Sibling(count);
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
	bool retval;
	{ RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	retval = (i_slaves.find(nick.LowerCase()) != i_slaves.end());
	}
	if (retval && !Magick::instance().nickserv.IsStored(nick))
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
	bool retval = Magick::instance().nickserv.GetStored(i_Host).IsSibling(nick);
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
    { RLOCK(("CommServ", "list"));
    for (citer = Magick::instance().commserv.ListBegin();
			    citer != Magick::instance().commserv.ListEnd(); citer++)
    {
	found = false;
	RLOCK2(("CommServ", "list", citer->first));
	if (citer->second.Name() != Magick::instance().commserv.ALL_Name() &&
	    citer->second.Name() != Magick::instance().commserv.REGD_Name() &&
	    citer->second.Name() != Magick::instance().commserv.SADMIN_Name())
	{
	    MLOCK(("CommServ", "list", citer->first, "member"));
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
    }}

    ChanServ::stored_t::iterator csiter;
    { RLOCK(("ChanServ", "stored"));
    for (csiter = Magick::instance().chanserv.StoredBegin();
			csiter != Magick::instance().chanserv.StoredEnd(); csiter++)
    {
	RLOCK2(("ChanServ", "stored", csiter->first));
	if (csiter->second.Founder().IsSameAs(oldnick, true))
	{
	    csiter->second.Founder(i_Name);
	    if (csiter->second.CoFounder().IsSameAs(i_Name, true) ||
		csiter->second.CoFounder().IsSameAs(oldnick, true))
	    {
		csiter->second.CoFounder("");
	    }
	}
	else if (csiter->second.CoFounder().IsSameAs(oldnick, true))
	{
	    csiter->second.CoFounder(i_Name);
	}
	{ MLOCK(("ChanServ", "stored", csiter->first, "Access"));
	found = false;
	valueL = 0;
	if (csiter->second.Access_find(i_Name, Chan_Stored_t::C_None, false))
	{
	    valueL = csiter->second.Access->Value();
	    modifier = csiter->second.Access->Last_Modifier();
	    modtime = csiter->second.Access->Last_Modify_Time();
	    csiter->second.Access_erase();
	    found = true;
	}
	if (csiter->second.Access_find(oldnick, Chan_Stored_t::C_None, false))
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
	if (csiter->second.Akick_find(i_Name, Chan_Stored_t::C_None, false))
	{
	    valueS = csiter->second.Akick->Value();
	    modifier = csiter->second.Akick->Last_Modifier();
	    modtime = csiter->second.Akick->Last_Modify_Time();
	    csiter->second.Akick_erase();
	    found = true;
	}
	if (csiter->second.Akick_find(oldnick, Chan_Stored_t::C_None, false))
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
    }}
    
    NickServ::stored_t::iterator niter;
    { RLOCK(("NickServ", "stored"));
    for (niter = Magick::instance().nickserv.StoredBegin();
			niter != Magick::instance().nickserv.StoredEnd(); niter++)
    {
	RLOCK2(("NickServ", "stored", niter->first));
	if (niter->first != i_Name.LowerCase() &&
	    niter->second.Host().LowerCase() != i_Name.LowerCase() &&
	    !IsSibling(niter->first) &&
	    niter->second.IsIgnore(oldnick))
	{
	    niter->second.IgnoreDel(oldnick);
	    niter->second.IgnoreAdd(i_Name);
	}
    }}

    if (Magick::instance().memoserv.IsNick(oldnick))
	Magick::instance().memoserv.NickMemoConvert(oldnick, i_Name);

    MemoServ::channel_t::iterator cniter;
    MemoServ::channel_news_t::iterator cnliter;
    { RLOCK(("MemoServ", "channel"));
    for (cniter = Magick::instance().memoserv.ChannelBegin();
			cniter != Magick::instance().memoserv.ChannelEnd(); cniter++)
    {
	RLOCK2(("MemoServ", "channel", cniter->first));
	for (cnliter = cniter->second.begin();
			    cnliter != cniter->second.end(); cnliter++)
	{
	    if (cnliter->IsRead(oldnick))
	    {
		cnliter->Unread(oldnick);
		cnliter->Read(i_Name);
	    }
	}
    }}
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
	RLOCK(("NickServ", "stored", i_Host.LowerCase()));
	DumpB();
	Nick_Stored_t &host = Magick::instance().nickserv.GetStored(i_Host);
	for (unsigned int i=0; i<host.Siblings(); i++)
	{
	    if (host.Sibling(i).LowerCase() != i_Name.LowerCase())
	    {
		i_slaves.insert(host.Sibling(i));
		WLOCK2(("NickServ", "stored", host.Sibling(i).LowerCase(), "i_Host"));
		Magick::instance().nickserv.GetStored(host.Sibling(i)).i_Host = i_Name.LowerCase();
	    }
	}
	i_slaves.insert(i_Host.LowerCase());
	i_Password = host.i_Password;
	i_Email = host.i_Email;
	i_URL = host.i_URL;
	i_ICQ = host.i_ICQ;
	i_AIM = host.i_AIM;
	i_Description = host.i_Description;
	i_Comment = host.i_Comment;
	i_access = host.i_access;
	i_ignore = host.i_ignore;
	setting.Protect = host.setting.Protect;
	lock.Protect = host.lock.Protect;
	setting.Secure = host.setting.Secure;
	lock.Secure = host.lock.Secure;
	setting.NoMemo = host.setting.NoMemo;
	lock.NoMemo = host.lock.NoMemo;
	setting.Private = host.setting.Private;
	lock.Private = host.lock.Private;
	setting.PRIVMSG = host.setting.PRIVMSG;
	lock.PRIVMSG = host.lock.PRIVMSG;
	setting.Language = host.setting.Language;
	lock.Language = host.lock.Language;
	setting.NoExpire = host.setting.NoExpire;
	setting.Forbidden = host.setting.Forbidden;
	setting.Picture = host.setting.Picture;
	i_Suspend_By = host.i_Suspend_By;
	i_Suspend_Time = host.i_Suspend_Time;
	{ WLOCK2(("NickServ", "stored", i_Host.LowerCase()));
	host.i_slaves.clear();
	host.i_Host = i_Name.LowerCase();
	}
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
	RLOCK(("NickServ", "stored", i_Host.LowerCase()));
	DumpB();
	Nick_Stored_t &host = Magick::instance().nickserv.GetStored(i_Host);
	i_slaves.clear();
	i_Password = host.i_Password;
	i_Email = host.i_Email;
	i_URL = host.i_URL;
	i_ICQ = host.i_ICQ;
	i_AIM = host.i_AIM;
	i_Description = host.i_Description;
	i_Comment = host.i_Comment;
	i_access = host.i_access;
	i_ignore = host.i_ignore;
	setting.Protect = host.setting.Protect;
	lock.Protect = host.lock.Protect;
	setting.Secure = host.setting.Secure;
	lock.Secure = host.lock.Secure;
	setting.NoMemo = host.setting.NoMemo;
	lock.NoMemo = host.lock.NoMemo;
	setting.Private = host.setting.Private;
	lock.Private = host.lock.Private;
	setting.PRIVMSG = host.setting.PRIVMSG;
	lock.PRIVMSG = host.lock.PRIVMSG;
	setting.Language = host.setting.Language;
	lock.Language = host.lock.Language;
	setting.NoExpire = host.setting.NoExpire;
	setting.Forbidden = host.setting.Forbidden;
	setting.Picture = 0;
	i_Suspend_By = host.i_Suspend_By;
	i_Suspend_Time = host.i_Suspend_Time;
	{ WLOCK2(("NickServ", "stored", i_Host.LowerCase()));
	host.i_slaves.erase(i_Name.LowerCase());
	}
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
	retval = Magick::instance().nickserv.GetStored(i_Host).Access();
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
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).Access(count);
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
	bool retval = Magick::instance().nickserv.GetStored(i_Host).AccessAdd(in);
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
	retval = Magick::instance().nickserv.GetStored(i_Host).AccessDel(in);
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
	bool retval = Magick::instance().nickserv.GetStored(i_Host).IsAccess(in);
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
	retval = Magick::instance().nickserv.GetStored(i_Host).Ignore();
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
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).Ignore(count);
	RET(retval);
    }
}


bool Nick_Stored_t::IgnoreAdd(const mstring& in)
{
    FT("Nick_Stored_t::IgnoreAdd", (in));
    if (Host().empty())
    {
	mstring target(in.LowerCase());
	// Not stored nick
	if (!Magick::instance().nickserv.IsStored(target))
	{
	    RET(false);
	}

	if (!Magick::instance().nickserv.GetStored(target).Host().empty())
	    target = Magick::instance().nickserv.GetStored(target).Host();
	
	if (IsIgnore(target))
	{
	    RET(false);
	}

	MCB(i_ignore.size());
	i_ignore.insert(target);
	MCE(i_ignore.size());
	RET(true);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).IgnoreAdd(in);
	RET(retval);
    }
}


unsigned int Nick_Stored_t::IgnoreDel(const mstring& in)
{
    FT("Nick_Stored_t::IgnoreDel", (in));
    unsigned int retval = 0;
    if (Host().empty())
    {
	mstring target(in.LowerCase());
	vector<mstring> chunked;
	set<mstring>::iterator iter;
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
	for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
	    if (in.Matches(*iter, true))
	    {
		chunked.push_back(*iter);
	    }

	if (Magick::instance().nickserv.IsStored(in))
	{
	    target = Magick::instance().nickserv.GetStored(in).Host();
	    if (!target.empty())
	    {
		for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
		    if (target.Matches(*iter, true))
		    {
			chunked.push_back(*iter);
		    }
	    }
	}
	MCB(i_ignore.size());
	for (unsigned int i=0; i<chunked.size(); i++)
	    i_ignore.erase(chunked[i].LowerCase());
	MCE(i_ignore.size());
	retval = chunked.size();
    }
    else
    {
	retval = Magick::instance().nickserv.GetStored(i_Host).IgnoreDel(in);
    }
    RET(retval);
}


bool Nick_Stored_t::IsIgnore(const mstring& in)
{
    FT("Nick_Stored_t::IsIgnore", (in));
    if (Host().empty())
    {
	mstring target(in.LowerCase());
	set<mstring>::iterator iter;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
	for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
	    if (target.Matches(*iter, true))
	    {
		RET(true);
	    }
	if (Magick::instance().nickserv.IsStored(target))
	{
	    target = Magick::instance().nickserv.GetStored(target).Host();
	    if (!target.empty())
	    {
		for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
		    if (target.Matches(*iter, true))
		    {
			RET(true);
		    }
	    }
	}
	RET(false);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).IsIgnore(in);
	RET(retval);
    }
}


bool Nick_Stored_t::Protect()
{
    NFT("Nick_Stored_t::Protect");
    if (Magick::instance().nickserv.LCK_Protect())
    {
	RET(Magick::instance().nickserv.DEF_Protect());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Protect"));
	RET(setting.Protect);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).Protect();
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
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Protect"));
	    MCB(setting.Protect);
	    setting.Protect = in;
	    MCE(setting.Protect);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).Protect(in);
    }
}


bool Nick_Stored_t::L_Protect()
{
    NFT("Nick_Stored_t::L_Protect");
    if (Magick::instance().nickserv.LCK_Protect())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Protect"));
	RET(lock.Protect);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).L_Protect();
	RET(retval);
    }
}


void Nick_Stored_t::L_Protect(const bool in)
{
    FT("Nick_Stored_t::L_Protect", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Protect())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Protect"));
	    MCB(lock.Protect);
	    lock.Protect = in;
	    MCE(lock.Protect);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).L_Protect(in);
    }
}


bool Nick_Stored_t::Secure()
{
    NFT("Nick_Stored_t::Secure");
    if (Magick::instance().nickserv.LCK_Secure())
    {
	RET(Magick::instance().nickserv.DEF_Secure());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Secure"));
	RET(setting.Secure);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).Secure();
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
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Secure"));
	    MCB(setting.Secure);
	    setting.Secure = in;
	    MCE(setting.Secure);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).Secure(in);
    }
}


bool Nick_Stored_t::L_Secure()
{
    NFT("Nick_Stored_t::L_Secure");
    if (Magick::instance().nickserv.LCK_Secure())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Secure"));
	RET(lock.Secure);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).L_Secure();
	RET(retval);
    }
}


void Nick_Stored_t::L_Secure(const bool in)
{
    FT("Nick_Stored_t::L_Secure", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Secure())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Secure"));
	    MCB(lock.Secure);
	    lock.Secure = in;
	    MCE(lock.Secure);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).L_Secure(in);
    }
}


bool Nick_Stored_t::NoExpire()
{
    NFT("Nick_Stored_t::NoExpire");
    if (Magick::instance().nickserv.LCK_NoExpire())
    {
	RET(Magick::instance().nickserv.DEF_NoExpire());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.NoExpire"));
	RET(setting.NoExpire);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).NoExpire();
	RET(retval);
    }
}


void Nick_Stored_t::NoExpire(const bool in)
{
    FT("Nick_Stored_t::NoExpire", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_NoExpire())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.NoExpire"));
	    MCB(setting.NoExpire);
	    setting.NoExpire = in;
	    MCE(setting.NoExpire);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).NoExpire(in);
    }
}


bool Nick_Stored_t::NoMemo()
{
    NFT("Nick_Stored_t::NoMemo");
    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	RET(Magick::instance().nickserv.DEF_NoMemo());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.NoMemo"));
	RET(setting.NoMemo);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).NoMemo();
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
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.NoMemo"));
	    MCB(setting.NoMemo);
	    setting.NoMemo = in;
	    MCE(setting.NoMemo);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).NoMemo(in);
    }
}


bool Nick_Stored_t::L_NoMemo()
{
    NFT("Nick_Stored_t::L_NoMemo");
    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.NoMemo"));
	RET(lock.NoMemo);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).L_NoMemo();
	RET(retval);
    }
}


void Nick_Stored_t::L_NoMemo(const bool in)
{
    FT("Nick_Stored_t::L_NoMemo", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_NoMemo())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.NoMemo"));
	    MCB(lock.NoMemo);
	    lock.NoMemo = in;
	    MCE(lock.NoMemo);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).L_NoMemo(in);
    }
}


bool Nick_Stored_t::Private()
{
    NFT("Nick_Stored_t::Private");
    if (Magick::instance().nickserv.LCK_Private())
    {
	RET(Magick::instance().nickserv.DEF_Private());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Private"));
	RET(setting.Private);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).Private();
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
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Private"));
	    MCB(setting.Private);
	    setting.Private = in;
	    MCE(setting.Private);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).Private(in);
    }
}


bool Nick_Stored_t::L_Private()
{
    NFT("Nick_Stored_t::L_Private");
    if (Magick::instance().nickserv.LCK_Private())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Private"));
	RET(lock.Private);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).L_Private();
	RET(retval);
    }
}


void Nick_Stored_t::L_Private(const bool in)
{
    FT("Nick_Stored_t::L_Private", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Private())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Private"));
	    MCB(lock.Private);
	    lock.Private = in;
	    MCE(lock.Private);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).L_Private(in);
    }
}


bool Nick_Stored_t::PRIVMSG()
{
    NFT("Nick_Stored_t::PRIVMSG");
    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	RET(Magick::instance().nickserv.DEF_PRIVMSG());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.PRIVMSG"));
	RET(setting.PRIVMSG);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).PRIVMSG();
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
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.PRIVMSG"));
	    MCB(setting.PRIVMSG);
	    setting.PRIVMSG = in;
	    MCE(setting.PRIVMSG);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).PRIVMSG(in);
    }
}


bool Nick_Stored_t::L_PRIVMSG()
{
    NFT("Nick_Stored_t::L_PRIVMSG");
    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.PRIVMSG"));
	RET(lock.PRIVMSG);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).L_PRIVMSG();
	RET(retval);
    }
}


void Nick_Stored_t::L_PRIVMSG(const bool in)
{
    FT("Nick_Stored_t::L_PRIVMSG", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_PRIVMSG())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.PRIVMSG"));
	    MCB(lock.PRIVMSG);
	    lock.PRIVMSG = in;
	    MCE(lock.PRIVMSG);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).L_PRIVMSG(in);
    }
}


mstring Nick_Stored_t::Language()
{
    NFT("Nick_Stored_t::Language");
    if (Magick::instance().nickserv.LCK_Language())
    {
	RET(Magick::instance().nickserv.DEF_Language().UpperCase());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Language"));
	RET(setting.Language);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).Language();
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
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Language"));
	    MCB(setting.Language);
	    setting.Language = in.UpperCase();
	    MCE(setting.Language);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).Language(in);
    }
}


bool Nick_Stored_t::L_Language()
{
    NFT("Nick_Stored_t::L_Language");
    if (Magick::instance().nickserv.LCK_Language())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Language"));
	RET(lock.Language);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host).L_Language();
	RET(retval);
    }
}


void Nick_Stored_t::L_Language(const bool in)
{
    FT("Nick_Stored_t::L_Language", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Language())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Language"));
	    MCB(lock.Language);
	    lock.Language = in;
	    MCE(lock.Language);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).L_Language(in);
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
	bool retval = Magick::instance().nickserv.GetStored(i_Host).Suspended();
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
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).Suspend_By();
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
	mDateTime retval = Magick::instance().nickserv.GetStored(i_Host).Suspend_Time();
	RET(retval);
    }
}


bool Nick_Stored_t::Forbidden() const
{
    NFT("Nick_Stored_t::Forbidden");
    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Forbidden"));
    RET(setting.Forbidden);
}

unsigned long Nick_Stored_t::PicNum()
{
    NFT("PicNum");
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.PicExt().empty())
	{
	    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Picture"));
	    RET(setting.Picture);
	}
	RET(0);
    }
    else
    {
	unsigned long retval = Magick::instance().nickserv.GetStored(i_Host).PicNum();
	RET(retval);
    }
}


void Nick_Stored_t::GotPic(const unsigned long picnum)
{
    FT("Nick_Stored_t::GotPic", (picnum));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.PicExt().empty())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Picture"));
	    MCB(setting.Picture);
	    setting.Picture = picnum;
	    MCE(setting.Picture);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host).GotPic(picnum);
    }
}


bool Nick_Stored_t::IsOnline()
{
    NFT("Nick_Stored_t::IsOnline");

    if (Magick::instance().nickserv.IsLive(i_Name))
    {
	// Not secure and recognized
	// or not suspended and identified
	if ((!Suspended() && Magick::instance().nickserv.GetLive(i_Name).IsIdentified()) ||
	    (!Secure() && Magick::instance().nickserv.GetLive(i_Name).IsRecognized()))
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
	mDateTime lastseen;
	{ RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	lastseen = i_LastSeenTime;
	} 
	unsigned int i;
	for (i=0; i<Siblings(); i++)
	{
	    if (Magick::instance().nickserv.GetStored(Sibling(i)).LastSeenTime() > lastseen)
		lastseen = Magick::instance().nickserv.GetStored(Sibling(i)).LastSeenTime();
	}
	RET(lastseen);
    }
    else
    {
	mDateTime retval = Magick::instance().nickserv.GetStored(i_Host).LastAllSeenTime();
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
	mstring retval = Magick::instance().nickserv.GetLive(i_Name).RealName();
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
	RET(Magick::instance().getMessage("VALS/ONLINE"));
    }
    else if (Host().empty())
    {
	mDateTime lastseen;
	{ RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	lastseen = i_LastSeenTime;
	}
	mstring lastmask = Name() + "!" + LastMask();
	unsigned int i;
	for (i=0; i<Siblings(); i++)
	{
	    if (Magick::instance().nickserv.GetStored(Sibling(i)).IsOnline())
	    {
		RET(Magick::instance().getMessage("VALS/ONLINE"));
	    }
	    if (Magick::instance().nickserv.GetStored(Sibling(i)).LastSeenTime() > lastseen)
	    {
		lastseen = Magick::instance().nickserv.GetStored(Sibling(i)).LastSeenTime();
		lastmask = Magick::instance().nickserv.GetStored(Sibling(i)).Name() + "!" +
				Magick::instance().nickserv.GetStored(Sibling(i)).LastMask();
	    }
	}
	RET(lastmask);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host).LastAllMask();
	RET(retval);
    }
}


mstring Nick_Stored_t::LastMask()
{
    NFT("Nick_Stored_t::LastMask");
    if (IsOnline())
    {
	RET(Magick::instance().getMessage("VALS/ONLINE"));
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
    for (i=Magick::instance().chanserv.StoredBegin(); i!=Magick::instance().chanserv.StoredEnd(); i++)
    {
	RLOCK2(("ChanServ", "stored", i->first));
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
SXP::Tag Nick_Stored_t::tag_AIM("AIM");
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

void Nick_Stored_t::BeginElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    static_cast<void>(pIn);

    FT("Nick_Stored_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if( pElement->IsA(tag_UserDef) )
    {
	mstring *tmp = new mstring;
	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void Nick_Stored_t::EndElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    static_cast<void>(pIn);

    FT("Nick_Stored_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
	if( pElement->IsA(tag_Name) )		pElement->Retrieve(i_Name);
	if( pElement->IsA(tag_RegTime) )	pElement->Retrieve(i_RegTime);
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
	if( pElement->IsA(tag_Email) )		pElement->Retrieve(i_Email);
	if( pElement->IsA(tag_URL) )		pElement->Retrieve(i_URL);
	if( pElement->IsA(tag_ICQ) )		pElement->Retrieve(i_ICQ);
	if( pElement->IsA(tag_AIM) )		pElement->Retrieve(i_AIM);
	if( pElement->IsA(tag_Description) )	pElement->Retrieve(i_Description);
	if( pElement->IsA(tag_Comment) )	pElement->Retrieve(i_Comment);
	if( pElement->IsA(tag_Host) )		pElement->Retrieve(i_Host);
	if( pElement->IsA(tag_set_Protect) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    setting.Protect = tmp;
	}
	if( pElement->IsA(tag_set_Secure) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    setting.Secure = tmp;
	}
	if( pElement->IsA(tag_set_NoExpire) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    setting.NoExpire = tmp;
	}
	if( pElement->IsA(tag_set_NoMemo) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    setting.NoMemo = tmp;
	}
	if( pElement->IsA(tag_set_Private) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    setting.Private = tmp;
	}
	if( pElement->IsA(tag_set_PRIVMSG) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    setting.PRIVMSG = tmp;
	}
	if( pElement->IsA(tag_Forbidden) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    setting.Forbidden = tmp;
	}
	if( pElement->IsA(tag_lock_Protect) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    lock.Protect = tmp;
	}
	if( pElement->IsA(tag_lock_Secure) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    lock.Secure = tmp;
	}
	if( pElement->IsA(tag_lock_NoMemo) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    lock.NoMemo = tmp;
	}
	if( pElement->IsA(tag_lock_Private) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    lock.Private = tmp;
	}
	if( pElement->IsA(tag_lock_PRIVMSG) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    lock.PRIVMSG = tmp;
	}
	if( pElement->IsA(tag_lock_Language) )
	{
	    bool tmp;
	    pElement->Retrieve(tmp);
	    lock.Language = tmp;
	}
	if( pElement->IsA(tag_set_Language) )	pElement->Retrieve(setting.Language);
	if( pElement->IsA(tag_Picture) )	pElement->Retrieve(setting.Picture);
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
    static_cast<void>(attribs);

    //TODO: Add your source code here
	pOut->BeginObject(tag_Nick_Stored_t);

	// Only way to ENSURE the data wont change.
	WLOCK(("NickServ", "stored", i_Name.LowerCase()));
	pOut->WriteElement(tag_Name, i_Name);
	pOut->WriteElement(tag_RegTime, i_RegTime);
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
	pOut->WriteElement(tag_ICQ, i_ICQ);
	pOut->WriteElement(tag_AIM, i_AIM);
	pOut->WriteElement(tag_Description, i_Description);
	pOut->WriteElement(tag_Comment, i_Comment);
	pOut->WriteElement(tag_Host, i_Host);
	pOut->WriteElement(tag_set_Protect, setting.Protect);
	pOut->WriteElement(tag_set_Secure, setting.Secure);
	pOut->WriteElement(tag_set_NoExpire, setting.NoExpire);
	pOut->WriteElement(tag_set_NoMemo, setting.NoMemo);
	pOut->WriteElement(tag_set_Private, setting.Private);
	pOut->WriteElement(tag_set_PRIVMSG, setting.PRIVMSG);
	pOut->WriteElement(tag_set_Language, setting.Language);
	pOut->WriteElement(tag_Forbidden, setting.Forbidden);
	pOut->WriteElement(tag_lock_Protect, lock.Protect);
	pOut->WriteElement(tag_lock_Secure, lock.Secure);
	pOut->WriteElement(tag_lock_NoMemo, lock.NoMemo);
	pOut->WriteElement(tag_lock_Private, lock.Private);
	pOut->WriteElement(tag_lock_PRIVMSG, lock.PRIVMSG);
	pOut->WriteElement(tag_lock_Language, lock.Language);
	pOut->WriteElement(tag_Picture, setting.Picture);
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
    retval += i_AIM.capacity();
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

    retval += sizeof(setting);
    retval += sizeof(lock);
    retval += setting.Language.capacity();

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
    MB(0, (i_Name, i_RegTime, i_Password, i_Email, i_URL, i_ICQ, i_AIM,
	i_Description, i_Comment, i_Host, i_slaves.size(), i_access.size(),
	i_ignore.size(), setting.Protect, lock.Protect, setting.Secure));
    MB(16, (lock.Secure, setting.NoExpire, setting.NoMemo, lock.NoMemo, setting.Private,
	lock.Private, setting.PRIVMSG, lock.PRIVMSG, setting.Language, lock.Language,
	setting.Forbidden, setting.Picture, i_Suspend_By, i_Suspend_Time,
	i_LastSeenTime, i_LastRealName));
    MB(32, (i_LastMask, i_LastQuit, i_UserDef.size()));
}

void Nick_Stored_t::DumpE()
{
    ME(0, (i_Name, i_RegTime, i_Password, i_Email, i_URL, i_ICQ, i_AIM,
	i_Description, i_Comment, i_Host, i_slaves.size(), i_access.size(),
	i_ignore.size(), setting.Protect, lock.Protect, setting.Secure));
    ME(16, (lock.Secure, setting.NoExpire, setting.NoMemo, lock.NoMemo, setting.Private,
	lock.Private, setting.PRIVMSG, lock.PRIVMSG, setting.Language, lock.Language,
	setting.Forbidden, setting.Picture, i_Suspend_By, i_Suspend_Time,
	i_LastSeenTime, i_LastRealName));
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

    // Amount of nicknames it will try, only
    // for the guest????? method.
    unsigned int i, attempts = 64;

    CP(("Renaming nickname %s", in.c_str()));
    if (Magick::instance().nickserv.Append_Rename())
    {
	for (i=0; i<Magick::instance().nickserv.Suffixes().length(); i++)
	{
	    mstring retval(in);
	    while (retval.length() < Magick::instance().server.proto.NickLen())
	    {
		retval += Magick::instance().nickserv.Suffixes()[i];
		COM(("Attempting to use %s", retval.c_str()));
		if (!Magick::instance().nickserv.IsLiveAll(retval) &&
		    !Magick::instance().nickserv.IsStored(retval))
		{
		    RET(retval);
		}
	    }
	}
    }
    else
    {
	srand(time(NULL));
	for (i=0; i<attempts; i++)
	{
	    mstring retval;
	    retval.Format("%s%05d",
		    Magick::instance().nickserv.Suffixes().c_str(),
		    rand() % 99999);
	    COM(("Attempting to use %s", retval.c_str()));
	    if (!Magick::instance().nickserv.IsLiveAll(retval) &&
		!Magick::instance().nickserv.IsStored(retval))
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

    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ACC* CUR*", Magick::instance().commserv.REGD_Name(), NickServ::do_access_Current);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ACC* ADD", Magick::instance().commserv.REGD_Name(), NickServ::do_access_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ACC* DEL*", Magick::instance().commserv.REGD_Name(), NickServ::do_access_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ACC* ERA*", Magick::instance().commserv.REGD_Name(), NickServ::do_access_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ACC* LIST", Magick::instance().commserv.REGD_Name(), NickServ::do_access_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ACC* VIEW", Magick::instance().commserv.REGD_Name(), NickServ::do_access_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ACC* H*LP", Magick::instance().commserv.REGD_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"IGN* ADD", Magick::instance().commserv.REGD_Name(), NickServ::do_ignore_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"IGN* DEL*", Magick::instance().commserv.REGD_Name(), NickServ::do_ignore_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"IGN* ERA*", Magick::instance().commserv.REGD_Name(), NickServ::do_ignore_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"IGN* LIST", Magick::instance().commserv.REGD_Name(), NickServ::do_ignore_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"IGN* VIEW", Magick::instance().commserv.REGD_Name(), NickServ::do_ignore_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"IGN* H*LP", Magick::instance().commserv.REGD_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* PASS*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_Password);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* E*MAIL*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_Email);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* URL", Magick::instance().commserv.REGD_Name(), NickServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* WWW*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* WEB*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* ICQ*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_ICQ);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* AIM*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_AIM);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* DESC*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_Description);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* COMM*", Magick::instance().commserv.SOP_Name(), NickServ::do_set_Comment);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* PIC*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_Picture);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* PROT*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_Protect);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* SEC*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* NOEX*", Magick::instance().commserv.SOP_Name(), NickServ::do_set_NoExpire);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* NOMEMO", Magick::instance().commserv.REGD_Name(), NickServ::do_set_NoMemo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* PRIVM*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* *MSG", Magick::instance().commserv.REGD_Name(), NickServ::do_set_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* PRIV*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* LANG*", Magick::instance().commserv.REGD_Name(), NickServ::do_set_Language);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* H*LP", Magick::instance().commserv.REGD_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK PROT*", Magick::instance().commserv.SOP_Name(), NickServ::do_lock_Protect);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK SEC*", Magick::instance().commserv.SOP_Name(), NickServ::do_lock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK NOMEMO", Magick::instance().commserv.SOP_Name(), NickServ::do_lock_NoMemo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK PRIVM*", Magick::instance().commserv.SOP_Name(), NickServ::do_lock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK *MSG", Magick::instance().commserv.SOP_Name(), NickServ::do_lock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK PRIV*", Magick::instance().commserv.SOP_Name(), NickServ::do_lock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK LANG*", Magick::instance().commserv.SOP_Name(), NickServ::do_lock_Language);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK H*LP", Magick::instance().commserv.SOP_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PROT*", Magick::instance().commserv.SOP_Name(), NickServ::do_unlock_Protect);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Magick::instance().commserv.SOP_Name(), NickServ::do_unlock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK NOMEMO", Magick::instance().commserv.SOP_Name(), NickServ::do_unlock_NoMemo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PRIVM*", Magick::instance().commserv.SOP_Name(), NickServ::do_unlock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK *MSG", Magick::instance().commserv.SOP_Name(), NickServ::do_unlock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Magick::instance().commserv.SOP_Name(), NickServ::do_unlock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK LANG*", Magick::instance().commserv.SOP_Name(), NickServ::do_unlock_Language);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK H*LP", Magick::instance().commserv.SOP_Name(), do_2param);

    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"H*LP", Magick::instance().commserv.ALL_Name(), NickServ::do_Help);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"REG*", Magick::instance().commserv.ALL_Name(), NickServ::do_Register);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"DROP*", Magick::instance().commserv.REGD_Name(), NickServ::do_Drop);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LINK*", Magick::instance().commserv.ALL_Name(), NickServ::do_Link);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"U*LIN*", Magick::instance().commserv.REGD_Name(), NickServ::do_UnLink);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"HOST", Magick::instance().commserv.REGD_Name(), NickServ::do_Host);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"*SLAV*", Magick::instance().commserv.REGD_Name(), NickServ::do_Slaves);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ID*", Magick::instance().commserv.ALL_Name(), NickServ::do_Identify);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SID*", Magick::instance().commserv.ALL_Name(), NickServ::do_Identify);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"INF*", Magick::instance().commserv.ALL_Name(), NickServ::do_Info);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"GHOST*", Magick::instance().commserv.ALL_Name(), NickServ::do_Ghost);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"REC*", Magick::instance().commserv.ALL_Name(), NickServ::do_Recover);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LIST*", Magick::instance().commserv.ALL_Name(), NickServ::do_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SEND*", Magick::instance().commserv.ALL_Name(), NickServ::do_Send);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SUSP*", Magick::instance().commserv.SOP_Name(), NickServ::do_Suspend);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNSUS*", Magick::instance().commserv.SOP_Name(), NickServ::do_UnSuspend);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"FORB*", Magick::instance().commserv.SOP_Name(), NickServ::do_Forbid);
#ifdef GETPASS
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"GETPASS*", Magick::instance().commserv.SADMIN_Name(), NickServ::do_Getpass);
#endif
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SETPASS*", Magick::instance().commserv.SOP_Name(), NickServ::do_Setpass);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LIVE*", Magick::instance().commserv.SOP_Name(), NickServ::do_Live);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"SET*", Magick::instance().commserv.REGD_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"LOCK", Magick::instance().commserv.REGD_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"UNLOCK", Magick::instance().commserv.REGD_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ACC* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"ACC*", Magick::instance().commserv.REGD_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"IGN* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(),
		"IGN*", Magick::instance().commserv.REGD_Name(), do_1_2param);
}

void NickServ::RemCommands()
{
    NFT("NickServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ACC* CUR*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ACC* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ACC* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ACC* ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ACC* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ACC* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ACC* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"IGN* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"IGN* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"IGN* ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"IGN* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"IGN* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"IGN* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* PASS*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* E*MAIL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* URL", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* WWW*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* WEB*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* ICQ*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* AIM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* DESC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* COMM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* PIC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* PROT*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* SEC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* NOEX*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* NOMEMO", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* PRIVM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* *MSG", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* PRIV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* LANG*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK PROT*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK NOMEMO", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK PRIVM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK *MSG", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK LANG*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK H*LP", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PROT*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK NOMEMO", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PRIVM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK *MSG", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK LANG*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK H*LP", Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"H*LP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"REG*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"DROP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LINK*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"U*LIN*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"HOST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"*SLAV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ID*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"INF*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"GHOST*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"REC*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LIST*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SEND*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SUSP*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNSUS*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"FORB*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"GET*PASS*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LIVE*", Magick::instance().commserv.SOP_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"SET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"LOCK", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"UNLOCK", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ACC* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"ACC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"IGN* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(),
		"IGN*", Magick::instance().commserv.REGD_Name());
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

    RLOCK(("NickServ", "recovered"));
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
    else if (msgtype == "PRIVMSG" &&
	!Magick::instance().commands.DoCommand(mynick, source, command, message))
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
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    mstring HelpTopic = Magick::instance().nickserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector<mstring> help = Magick::instance().getHelp(source, HelpTopic.UpperCase());
					
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

    if (Magick::instance().nickserv.IsStored(source))
    {
	NSEND(mynick, source, "NS_YOU_STATUS/ISSTORED");
    }
    else if (!Magick::instance().nickserv.GetLive(source).HasMode("o") &&
		Magick::instance().nickserv.GetLive(source).LastNickReg().SecondsSince() <
    		Magick::instance().nickserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Magick::instance().nickserv.Delay() -
		Magick::instance().nickserv.GetLive(source).LastNickReg().SecondsSince(),
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
		Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
	    return;
	}
	else if (email.WordCount("@") != 2)
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE", (
		Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
	    return;
	}

	RLOCK(("NickServ", "live", source.LowerCase()));
	Nick_Live_t &nlive = Magick::instance().nickserv.GetLive(source);
	nlive.SetLastNickReg();
	Nick_Stored_t tmp(source, password);
	tmp.AccessAdd(nlive.Mask(Nick_Live_t::U_H).After("!"));
	if (!email.empty())
	    tmp.Email(email);
	Magick::instance().nickserv.AddStored(&tmp);
	nlive.Identify(password);
	Magick::instance().nickserv.stats.i_Register++;
	SEND(mynick, source, "NS_YOU_COMMAND/REGISTERED", (
		nlive.Mask(Nick_Live_t::U_H).After("!")));
	LOG(LM_INFO, "NICKSERV/REGISTER", (
		nlive.Mask(Nick_Live_t::N_U_P_H),
		source));
    }
}

void NickServ::do_Drop(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Drop", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    mstring target;

    if (params.WordCount(" ") < 2)
    {
	target = source;

	if (!Magick::instance().nickserv.GetLive(source).IsIdentified())
	{
	    SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
				message, mynick));
	    return;
	}
    }
    else if (Magick::instance().nickserv.IsStored(params.ExtractWord(2, " ")))
    {
	target = Magick::instance().getSname(params.ExtractWord(2, " "));

	if (Magick::instance().nickserv.GetStored(target).IsSibling(source))
	{
	    // Our sibling, and we're not identified ...
	    if (!Magick::instance().nickserv.GetLive(source).IsIdentified())
	    {
		SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
				message, mynick));
		return;
	    }
	}
	else if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	     Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name()).IsOn(source))
	{
	     // NOT our sibling, and its either a sop or oper ...
	     if ((Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name()).IsIn(target)) ||
	     (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	     Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsIn(target)))
	     {
		SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (message));
		return;
	     }
	}
	else
	{
	    // Not ours at all, and we're not a sop!
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTYOURS", (target));
	    return;
	}
    }
    else
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
					params.ExtractWord(2, " ")));
	return;
    }

    Magick::instance().nickserv.stats.i_Drop++;
    unsigned long dropped = Magick::instance().nickserv.GetStored(target).Drop();
    Magick::instance().nickserv.RemStored(target);
    if (!Magick::instance().nickserv.IsStored(source))
	Magick::instance().nickserv.GetLive(source).UnIdentify();
    if (source == target)
	NSEND(mynick, source, "NS_YOU_COMMAND/DROPPED");
    else
	SEND(mynick, source, "NS_OTH_COMMAND/DROPPED", (target));
    LOG(LM_INFO, "NICKSERV/DROP", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, dropped-1));

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

    if (!Magick::instance().nickserv.IsStored(hostnick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
							hostnick));
	return;
    }
    if (!Magick::instance().nickserv.GetStored(hostnick).Host().empty())
	hostnick = Magick::instance().nickserv.GetStored(hostnick).Host();
    else
	hostnick = Magick::instance().getSname(hostnick);

    if (Magick::instance().nickserv.GetStored(hostnick).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
					hostnick));
	return;
    }

    if (!Magick::instance().nickserv.GetLive(source).HasMode("o") &&
		Magick::instance().nickserv.GetLive(source).LastNickReg().SecondsSince() <
    		Magick::instance().nickserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Magick::instance().nickserv.Delay() -
		Magick::instance().nickserv.GetLive(source).LastNickReg().SecondsSince(),
		source)));
	return;
    }

    mDateTime regtime = mDateTime::CurrentDateTime();
    if (Magick::instance().nickserv.IsStored(source))
    {
	mstring myhost = Magick::instance().nickserv.GetStored(source).Host();
	if (myhost.empty())
	{
	    if (source.IsSameAs(hostnick))
	    {
		SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF",
					(message));
		return;
	    }
	}
	else
	{
	    if (myhost.IsSameAs(hostnick))
	    {
		SEND(mynick, source, "ERR_SITUATION/NOTONSIBLING",
					(message));
		return;
	    }
	}

	if (!Magick::instance().nickserv.GetLive(source).IsIdentified())
	{
	    SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
					message, mynick));
	    return;
	}
	regtime = Magick::instance().nickserv.GetStored(source).RegTime();
    }

    if (Magick::instance().nickserv.GetStored(hostnick).Slave(source, password, regtime))
    {
	Magick::instance().nickserv.GetLive(source).SetLastNickReg();
	Magick::instance().nickserv.stats.i_Link++;
	Magick::instance().nickserv.GetLive(source).Identify(password);
	SEND(mynick, source, "NS_YOU_COMMAND/LINKED", (
					hostnick));
	LOG(LM_INFO, "NICKSERV/LINK", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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

    if (!Magick::instance().nickserv.GetLive(source).IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
						message, mynick));
	return;
    }

    if (params.WordCount(" ") > 1)
    {
	mstring target = params.ExtractWord(2, " ");

	
	if (!Magick::instance().nickserv.IsStored(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
	    return;
	}

	target = Magick::instance().getSname(target);
	if (Magick::instance().nickserv.GetStored(target).Host().empty())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISHOST", (
							target));
	    return;
	}

	if (!(Magick::instance().nickserv.GetStored(target).IsSibling(source) ||
		Magick::instance().nickserv.GetStored(target).Host().IsSameAs(source, true)))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTYOURS", (
							target));
	    return;
	}
	
	if (Magick::instance().nickserv.GetStored(target).Unlink())
	{
	    Magick::instance().nickserv.stats.i_Unlink++;
	    SEND(mynick, source, "NS_OTH_COMMAND/UNLINKED", (
							target));
	    LOG(LM_INFO, "NICKSERV/UNLINK", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		source, target));
	}
	else
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTLINKED", (
							target));
    }
    else
    {
	if (Magick::instance().nickserv.GetStored(source).Host().empty())
	{
	    NSEND(mynick, source, "NS_YOU_STATUS/ISHOST");
	    return;
	}

	if (Magick::instance().nickserv.GetStored(source).Unlink())
	{
	    Magick::instance().nickserv.stats.i_Unlink++;
	    mstring target = Magick::instance().nickserv.GetStored(source).Host();
	    NSEND(mynick, source, "NS_YOU_COMMAND/UNLINKED");
	    LOG(LM_INFO, "NICKSERV/UNLINK", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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

    if (!Magick::instance().nickserv.GetLive(source).IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
						message, mynick));
	return;
    }

    if (!newhost.empty())
    {
	if (!Magick::instance().nickserv.IsStored(newhost))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						newhost));
	    return;
	}

	newhost = Magick::instance().getSname(newhost);
	if (Magick::instance().nickserv.GetStored(newhost).Host().empty())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISHOST", (
						newhost));
	    return;
	}

	if (!(source.IsSameAs(newhost, true) ||
		Magick::instance().nickserv.GetStored(newhost).IsSibling(source) ||
		Magick::instance().nickserv.GetStored(newhost).Host().IsSameAs(source, true)))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTLINKED", (
						newhost));
	    return;
	}

	mstring oldhost = Magick::instance().nickserv.GetStored(newhost).Host();
	Magick::instance().nickserv.stats.i_Host++;
	Magick::instance().nickserv.GetStored(newhost).MakeHost();
	SEND(mynick, source, "NS_OTH_COMMAND/NEWHOST", (
						newhost));
	LOG(LM_INFO, "NICKSERV/HOST", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		oldhost, newhost));
    }
    else
    {
	mstring oldhost = Magick::instance().nickserv.GetStored(source).Host();
	Magick::instance().nickserv.stats.i_Host++;
	Magick::instance().nickserv.GetStored(source).MakeHost();
	NSEND(mynick, source, "NS_YOU_COMMAND/NEWHOST");
	LOG(LM_INFO, "NICKSERV/HOST", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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
	target = Magick::instance().getSname(source);
    }
    else
    {
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
		Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View()).IsOn(source))
	{
	    target = params.ExtractWord(2, " ");
	    if (!Magick::instance().nickserv.IsStored(target))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
		return;
	    }

	    target = Magick::instance().getSname(target);
	    if (Magick::instance().nickserv.GetStored(target).Forbidden())
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
					target));
		return;
	    }
	}
	else
	    target = Magick::instance().getSname(source);
    }

    mstring output;
    if (!Magick::instance().nickserv.GetStored(target).Host().empty())
	target = Magick::instance().nickserv.GetStored(target).Host();

    Nick_Stored_t nick = Magick::instance().nickserv.GetStored(target);
    output << IRC_Bold << target << IRC_Off << " (" <<
	nick.Siblings() << "):";

    for (unsigned int i=0; i<nick.Siblings(); i++)
    {
	if (nick.Sibling(i).length() + output.length() > 510)
	{
	    ::send(mynick, source, output);
	    output.erase();
	    output << IRC_Bold << target << IRC_Off << " (" <<
		nick.Siblings() << "):";
	}
	output << " " << nick.Sibling(i);
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

    if (!Magick::instance().nickserv.IsStored(source))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						source));
	return;
    }

    if (Magick::instance().nickserv.GetStored(source).Suspended())
    {
	NSEND(mynick, source, "NS_YOU_STATUS/ISSUSPENDED");
	return;
    }

    bool wasident = Magick::instance().nickserv.GetLive(source).IsIdentified();
    mstring password = params.ExtractWord(2, " ");
    mstring output = Magick::instance().nickserv.GetLive(source).Identify(password);
    if (!wasident &&
	Magick::instance().nickserv.GetLive(source).IsIdentified())
    {
	Magick::instance().nickserv.stats.i_Identify++;
	LOG(LM_INFO, "NICKSERV/IDENTIFY", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
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
    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
	return;
    }

    Nick_Stored_t nick = Magick::instance().nickserv.GetStored(target);

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
		Magick::instance().nickserv.GetStored(nick.Host()).Name()));
    output.erase();
    if (nick.NoExpire() && Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsOn(source))
	output << " (" << Magick::instance().getMessage(source, "NS_INFO/NOEXPIRE") << ")";
	SEND(mynick, source, "NS_INFO/REGISTERED", (
		nick.RegTime().Ago(), output));

    if (!nick.IsOnline())
    {
	output.erase();
	bool isonline = false;
	if (!nick.Host().empty() &&
	    Magick::instance().nickserv.GetStored(nick.Host()).IsOnline())
	    output = Magick::instance().nickserv.GetLive(nick.Host()).Name() + " ";
	for (i=0; i<nick.Siblings(); i++)
	{
	    if (Magick::instance().nickserv.IsStored(nick.Sibling(i)) &&
		Magick::instance().nickserv.GetStored(nick.Sibling(i)).IsOnline())
	    {
		if (output.length() + nick.Sibling(i).length() > Magick::instance().server.proto.MaxLine())
		{
		    SEND(mynick, source, "NS_INFO/ONLINEAS", (
						output));
		    output.erase();
		    isonline = true;
		}
		output += Magick::instance().nickserv.GetLive(nick.Sibling(i)).Name() + " ";
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
	    if (!nick.Private() || (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsOn(source)))
		SEND(mynick, source, "NS_INFO/LASTALLMASK", (
						nick.LastAllMask()));
	    SEND(mynick, source, "NS_INFO/LASTALLSEEN", (
					nick.LastAllSeenTime().Ago()));
	    if (nick.LastAllMask().UpperCase() !=
		mstring(nick.Name() + "!" + nick.LastMask()).UpperCase() &&
		(!nick.Private() || (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsOn(source))))
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
	if (!nick.AIM().empty())
	    SEND(mynick, source, "NS_INFO/AIM", (
					nick.AIM()));
	if (!nick.Description().empty())
	    SEND(mynick, source, "NS_INFO/DESCRIPTION", (
					nick.Description()));
	if (!nick.Comment().empty() &&
	    ((Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	    Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsOn(source)) ||
	    (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	    Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name()).IsOn(source))))
	    SEND(mynick, source, "NS_INFO/COMMENT", (
					nick.Comment()));
    }

    output.erase();
    CommServ::list_t::iterator iter;
    { RLOCK(("CommServ", "list"));
    for (iter=Magick::instance().commserv.ListBegin();
		iter!=Magick::instance().commserv.ListEnd(); iter++)
    {
	RLOCK2(("CommServ", "list", iter->first));
	// IF committee is not ALL or REGD
	// AND if it has a headcom, we're not in it
	// AND the committee isnt private or the requesting user is in OPER
	if (iter->first != Magick::instance().commserv.ALL_Name() &&
	    iter->first != Magick::instance().commserv.REGD_Name() &&
	    (iter->second.HeadCom().empty() ||
	    !(Magick::instance().commserv.IsList(iter->second.HeadCom()) &&
	      Magick::instance().commserv.GetList(iter->second.HeadCom().UpperCase()).IsIn(nick.Name()))) &&
	    iter->second.IsIn(target) && (!iter->second.Private() ||
	    (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	     Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsOn(source))))
	{
	    if (output.length() + iter->second.Name().length() > Magick::instance().server.proto.MaxLine())
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
	}}
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
	output << Magick::instance().getMessage(source, "NS_SET/PROTECT");
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
	output << Magick::instance().getMessage(source, "NS_SET/SECURE");
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
	output << Magick::instance().getMessage(source, "NS_SET/NOMEMO");
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
	output << Magick::instance().getMessage(source, "NS_SET/PRIVATE");
	if (nick.L_Private())
	    output << IRC_Off;
    }

    if (!output.empty())
	SEND(mynick, source, "NS_INFO/OPTIONS", (
						output));
    if (nick.PicNum() &&
	Magick::instance().filesys.Exists(FileMap::Picture, nick.PicNum()))
	SEND(mynick, source, "NS_INFO/HASPIC", (
				ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::Picture, nick.PicNum())),
				mynick, nick.Name()));
    if (nick.IsOnline())
	SEND(mynick, source,  "NS_INFO/ISONLINE", (
		Magick::instance().getLname(nick.Name())));
    { RLOCK(("Events"));
    if (Magick::instance().servmsg.ShowSync() && Magick::instance().events != NULL)
	SEND(mynick, source, "MISC/SYNC", (
			Magick::instance().events->SyncTime(source)));
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

    if (!Magick::instance().nickserv.IsStored(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						nick));
	return;
    }

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (
						nick));
	return;
    }

    nick = Magick::instance().getLname(nick);
    if (Magick::instance().nickserv.GetStored(nick).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nick));
	return;
    }

    if (!Magick::instance().nickserv.GetStored(nick).CheckPass(pass))
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
	return;
    }

    Magick::instance().server.ANONKILL(mynick, nick, source + " (" +
				Magick::instance().getMessage(nick, "MISC/KILL_GHOST") + ")");
    if (Magick::instance().nickserv.IsRecovered(nick))
    {
	Magick::instance().nickserv.RemRecovered(nick);
    }
    Magick::instance().nickserv.stats.i_Ghost++;
    SEND(mynick, source, "NS_OTH_COMMAND/RELEASED", (
				nick));
    LOG(LM_DEBUG, "NICKSERV/GHOST", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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

    if (!Magick::instance().nickserv.IsStored(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nick));
	return;
    }

    nick = Magick::instance().getSname(nick);
    if (Magick::instance().nickserv.GetStored(nick).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nick));
	return;
    }

    if (!Magick::instance().nickserv.GetStored(nick).CheckPass(pass))
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
	return;
    }

    if (Magick::instance().nickserv.IsLive(nick))
    {
	Magick::instance().server.KILL(mynick, Magick::instance().getLname(nick), source + " (" +
			Magick::instance().getMessage(nick, "MISC/KILL_RECOVER") + ")");
    }

    Magick::instance().server.NICK(nick, (Magick::instance().startup.Ownuser() ? nick.LowerCase() :
				Magick::instance().startup.Services_User()),
				Magick::instance().startup.Services_Host(),
				Magick::instance().startup.Server_Name(),
				Magick::instance().nickserv.Enforcer_Name());

    Magick::instance().nickserv.AddRecovered(nick, mDateTime::CurrentDateTime());
    Magick::instance().nickserv.stats.i_Recover++;
    SEND(mynick, source, "NS_OTH_COMMAND/HELD", (
				nick));
    LOG(LM_DEBUG, "NICKSERV/RECOVER", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	nick));
}

void NickServ::do_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_List", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	mask = "*";
	listsize = Magick::instance().config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Magick::instance().config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Magick::instance().config.Maxlist())
	{
	    mstring output;
	    SEND(mynick, source, "LIST/MAXLIST", (
					Magick::instance().config.Maxlist()));
	    return;
	}
    }

    SEND(mynick, source, "LIST/NICK_LIST", (
					mask));
    NickServ::stored_t::iterator iter;


    bool isoper = (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsOn(source));
    bool issop = (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
		Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name()).IsOn(source));

    { RLOCK(("NickServ", "stored"));
    for (iter = Magick::instance().nickserv.StoredBegin(), i=0, count = 0;
			iter != Magick::instance().nickserv.StoredEnd(); iter++)
    {
	RLOCK2(("NickServ", "stored", iter->first));
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
			Magick::instance().getMessage(source, "VALS/FORBIDDEN") + ")");
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
    }}
    SEND(mynick, source, "LIST/DISPLAYED", (
							i, count));
}

void NickServ::do_Send(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Send", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
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

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
							target));
	return;
    }

    target = Magick::instance().getSname(target);
    if (Magick::instance().nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    unsigned long picnum = Magick::instance().nickserv.GetStored(target).PicNum();
    if (!picnum)
    {
	SEND(mynick, source, "NS_OTH_STATUS/NOPIC", (
							target));
	return;
    }

    if (!Magick::instance().filesys.Exists(FileMap::Picture, picnum))
    {
	Magick::instance().nickserv.GetStored(target).GotPic(0);
	SEND(mynick, source, "DCC/NOFILE", (
							"SEND"));
	return;
    }

    mstring filename = 	Magick::instance().filesys.GetName(FileMap::Picture, picnum);
    size_t filesize = Magick::instance().filesys.GetSize(FileMap::Picture, picnum);
    if (filename.empty() || filesize <= 0)
    {
	Magick::instance().nickserv.GetStored(target).GotPic(0);
	SEND(mynick, source, "DCC/NOFILE", (
							"SEND"));
	return;
    }

    if (!(Magick::instance().files.TempDirSize() == 0 ||
	mFile::DirUsage(Magick::instance().files.TempDir()) <=
		Magick::instance().files.TempDirSize()))
    {
	NSEND(mynick, source, "DCC/NOSPACE2");
	return;
    }

    { RLOCK(("DCC"));
    if (Magick::instance().dcc != NULL)
    {
	Magick::instance().nickserv.stats.i_Send++;
	unsigned short port = mSocket::FindAvailPort();
	::privmsg(mynick, source, DccEngine::encode("DCC SEND", filename +
		" " + mstring(Magick::instance().LocalHost()) + " " +
		mstring(port) + " " + mstring(filesize)));
	Magick::instance().dcc->Accept(port, mynick, source, FileMap::Picture, picnum);
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

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
							target));
	return;
    }

    target = Magick::instance().getSname(target);
    if (Magick::instance().nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    if (Magick::instance().nickserv.GetStored(target).Suspended())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISSUSPENDED", (
							target));
	return;
    }

    if (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsIn(target))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONCOMMITTEE", (
				message, Magick::instance().commserv.OPER_Name()));
	return;
    }

    Magick::instance().nickserv.GetStored(target).Suspend(source, reason);
    Magick::instance().nickserv.stats.i_Suspend++;
    SEND(mynick, source, "NS_OTH_COMMAND/SUSPENDED", (
						target));
    LOG(LM_NOTICE, "NICKSERV/SUSPEND", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
							target));
	return;
    }

    target = Magick::instance().getSname(target);
    if (Magick::instance().nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    if (!Magick::instance().nickserv.GetStored(target).Suspended())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSUSPENDED", (
							target));
	return;
    }

    Magick::instance().nickserv.GetStored(target).UnSuspend();
    Magick::instance().nickserv.stats.i_Unsuspend++;
    SEND(mynick, source, "NS_OTH_COMMAND/UNSUSPENDED", (
						target));
    LOG(LM_NOTICE, "NICKSERV/UNSUSPEND", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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
    if (Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISSTORED", (
					Magick::instance().getSname(target)));
	return;
    }

    if ((Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsIn(target)) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsIn(target)))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (message));
	return;
    }

    Nick_Stored_t tmp(target);
    Magick::instance().nickserv.AddStored(&tmp);
    Magick::instance().nickserv.stats.i_Forbid++;
    SEND(mynick, source, "NS_OTH_COMMAND/FORBIDDEN", (
					target));
    LOG(LM_NOTICE, "NICKSERV/FORBID", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target));
}

#ifdef GETPASS

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

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
	return;
    }

    target = Magick::instance().getSname(target);
    if (Magick::instance().nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    // If we are NOT a SADMIN, and target is a PRIV GROUP.
    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name()).IsIn(source)) &&
	(Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name()).IsIn(target) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name()).IsIn(target)) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name()).IsIn(target)) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsIn(target))))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (
						message));
	return;
    }

    mstring host, password;
    Nick_Stored_t nick = Magick::instance().nickserv.GetStored(target);
    host = nick.Host();
    if (host.empty())
	host = nick.Name();
    password = nick.Password();

    Magick::instance().nickserv.stats.i_Getpass++;
    SEND(mynick, source, "NS_OTH_COMMAND/GETPASS", (
			target, host, password));
    ANNOUNCE(mynick, "MISC/NICK_GETPASS", (
			source, target, host));
    LOG(LM_NOTICE, "NICKSERV/GETPASS", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, host));
}

#endif /* GETPASS */

void NickServ::do_Setpass(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Setpass", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring target   = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
	return;
    }

    target = Magick::instance().getSname(target);
    if (Magick::instance().nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    // If we are NOT a SADMIN, and target is a PRIV GROUP.
    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name()).IsIn(source)) &&
	(Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name()).IsIn(target) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name()).IsIn(target)) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name()).IsIn(target)) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name()).IsIn(target))))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (
						message));
	return;
    }

    mstring host;
    { RLOCK(("NickServ", "stored", target.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(target);
    host = nick.Host();
    if (host.empty())
	host = nick.Name();
    nick.Password(password);
    }

    Magick::instance().nickserv.stats.i_Getpass++;
    SEND(mynick, source, "NS_OTH_COMMAND/SETPASS", (
			target, host, password));
    ANNOUNCE(mynick, "MISC/NICK_SETPASS", (
			source, target, host));
    LOG(LM_NOTICE, "NICKSERV/SETPASS", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	target, host));
}

void NickServ::do_Live(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_Live", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	mask = "*!*@*";
	listsize = Magick::instance().config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Magick::instance().config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Magick::instance().config.Maxlist())
	{
	    mstring output;
	    SEND(mynick, source, "LIST/MAXLIST", (
					Magick::instance().config.Maxlist()));
	    return;
	}
    }

    if (!mask.Contains("@") && !mask.Contains("!"))
	mask += "!*@*";
    else if (!mask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
				Magick::instance().getMessage(source, "LIST/NICKNAME"), '@'));
	return;
    }
    else if (!mask.Contains("!"))
	mask.prepend("*!");

    SEND(mynick, source, "LIST/NICK_LIST", (
					mask));
    NickServ::live_t::iterator iter;

    { RLOCK(("NickServ", "live"));
    for (iter = Magick::instance().nickserv.LiveBegin(), i=0, count = 0;
			iter != Magick::instance().nickserv.LiveEnd(); iter++)
    {
	RLOCK2(("NickServ", "live", iter->first));
	if (iter->second.Mask(Nick_Live_t::N_U_P_H).Matches(mask, true))
	{
	    if (i < listsize)
	    {
		if (message.Contains("OPER") && !iter->second.HasMode("o"))
		    continue;

		::send(mynick, source, iter->second.Mask(Nick_Live_t::N_U_P_H) + " (" +
					((!iter->second.Server().empty()) ?
						iter->second.Server() :
						Magick::instance().startup.Server_Name()) +
					((!iter->second.Squit().empty()) ? " (SQUIT)" : "") +
					"): +" + iter->second.Mode());
		i++;
	    }
	    count++;
	}
    }}
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

    if (Magick::instance().nickserv.GetStored(source).AccessAdd(
	    Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::U_H).After("!")))
    {
	Magick::instance().nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/ADD", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::U_H).After("!"),
		Magick::instance().getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_ADD", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::U_H).After("!"),
		source));

    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS", (
		mstring(Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::U_H).After("!")),
		Magick::instance().getMessage(source, "LIST/ACCESS")));
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
				Magick::instance().getMessage(source, "LIST/ACCESS"), '!'));
	return;
    }

    if (!hostmask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
				Magick::instance().getMessage(source, "LIST/ACCESS"), '@'));
	return;
    }

    if (Magick::instance().nickserv.GetStored(source).AccessAdd(hostmask))
    {
	Magick::instance().nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/ADD", (
			hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_ADD", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		hostmask, source));
    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS", (
			hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
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
	if (num <= 0 || num > Magick::instance().nickserv.GetStored(source).Access())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
			1, Magick::instance().nickserv.GetStored(source).Access()));
	    return;
	}

	hostmask = Magick::instance().nickserv.GetStored(source).Access(num-1);
    }

    if (hostmask.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (
				Magick::instance().getMessage(source, "LIST/ACCESS"), '!'));
	return;
    }

    if (!hostmask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (
				Magick::instance().getMessage(source, "LIST/ACCESS"), '@'));
	return;
    }

    unsigned int count = Magick::instance().nickserv.GetStored(source).AccessDel(hostmask);
    if (count)
    {
	Magick::instance().nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/DEL_MATCH", (
			count, hostmask,
			Magick::instance().getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_DEL", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		hostmask, source));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (
			hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
}

void NickServ::do_access_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_access_List", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    mstring target = Magick::instance().getSname(source);

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") >= 3 &&
	Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View()).IsOn(source))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsStored(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
					target));
	    return;
	}

	target = Magick::instance().getSname(target);
	if (Magick::instance().nickserv.GetStored(target).Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	    return;
	}
    }

    if (Magick::instance().nickserv.GetStored(target).Access())
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/DISPLAY2", (
			target, Magick::instance().getMessage(source, "LIST/ACCESS")));
	else
	    SEND(mynick, source, "LIST/DISPLAY", (
			Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
    else
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/EMPTY2", (
			target, Magick::instance().getMessage(source, "LIST/ACCESS")));
	else
	    SEND(mynick, source, "LIST/EMPTY", (
			Magick::instance().getMessage(source, "LIST/ACCESS")));
	return;
    }

    unsigned int i;
    mstring retval;
    for (i=0; i<Magick::instance().nickserv.GetStored(target).Access(); i++)
    {
	retval.erase();
	retval << i+1 << ". " << Magick::instance().nickserv.GetStored(target).Access(i);
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

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
						target));
	return;
    }

    target = Magick::instance().getSname(target);
    if (Magick::instance().nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    if (Magick::instance().nickserv.GetStored(source).IgnoreAdd(target))
    {
	Magick::instance().nickserv.stats.i_Ignore++;
	SEND(mynick, source, "LIST/ADD", (
			target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	LOG(LM_DEBUG, "NICKSERV/IGNORE_ADD", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		target, source));
    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS", (
			target, Magick::instance().getMessage(source, "LIST/IGNORE")));
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
	if (num <= 0 || num > Magick::instance().nickserv.GetStored(source).Ignore())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
			1, Magick::instance().nickserv.GetStored(source).Ignore()));
	    return;
	}

	target = Magick::instance().nickserv.GetStored(source).Ignore(num-1);
    }

    unsigned int count = Magick::instance().nickserv.GetStored(source).IgnoreDel(target);
    if (count)
    {
	Magick::instance().nickserv.stats.i_Ignore++;
	SEND(mynick, source, "LIST/DEL_MATCH", (
			count, target,
			Magick::instance().getMessage(source, "LIST/IGNORE")));
	LOG(LM_DEBUG, "NICKSERV/IGNORE_DEL", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		target, source));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (
			target, Magick::instance().getMessage(source, "LIST/IGNORE")));
    }

}

void NickServ::do_ignore_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_ignore_List", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    mstring target = Magick::instance().getSname(source);

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") >= 3 &&
	Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View()).IsOn(source))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsStored(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				target));
	    return;
	}

	target = Magick::instance().getSname(target);
	if (Magick::instance().nickserv.GetStored(target).Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	    return;
	}
    }

    if (Magick::instance().nickserv.GetStored(target).Ignore())
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/DISPLAY2", (
			target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	else
	    SEND(mynick, source, "LIST/DISPLAY", (
			Magick::instance().getMessage(source, "LIST/IGNORE")));
    }
    else
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/EMPTY2", (
			target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	else
	    SEND(mynick, source, "LIST/EMPTY", (
			Magick::instance().getMessage(source, "LIST/IGNORE")));
	return;
    }

    unsigned int i;
    mstring retval;
    for (i=0; i<Magick::instance().nickserv.GetStored(target).Ignore(); i++)
    {
	retval.erase();
	retval << i+1 << ". " << Magick::instance().nickserv.GetStored(target).Ignore(i);
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

    if (!Magick::instance().nickserv.GetLive(source).IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (
			message, mynick));
	return;
    }

    mstring oldpass = Magick::instance().nickserv.GetStored(source).Password();
    if (newpass.IsSameAs(oldpass, true) || newpass.IsSameAs(source, true) ||
	newpass.length() < 5)
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
	return;
    }

    Magick::instance().nickserv.GetStored(source).Password(newpass);
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Magick::instance().getMessage(source, "NS_SET/PASSWORD"), newpass));
    LOG(LM_INFO, "NICKSERV/SET_PASSWORD", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
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
		Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
	return;
    }
    else if (newvalue.WordCount("@") != 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE", (
		Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
	return;
    }

    Magick::instance().nickserv.GetStored(source).Email(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Magick::instance().getMessage(source, "NS_SET/EMAIL"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/EMAIL"),
		source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (
		Magick::instance().getMessage(source, "NS_SET/EMAIL")));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/EMAIL"),
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

    Magick::instance().nickserv.GetStored(source).URL(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Magick::instance().getMessage(source, "NS_SET/URL"),
		"http://" + newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/URL"),
		source, "http://" + newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (
		Magick::instance().getMessage(source, "NS_SET/URL")));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/URL"),
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


    Magick::instance().nickserv.GetStored(source).ICQ(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Magick::instance().getMessage(source, "NS_SET/ICQ"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/ICQ"),
		source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (
		Magick::instance().getMessage(source, "NS_SET/ICQ")));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/ICQ"),
		source));
    }
}

void NickServ::do_set_AIM(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_AIM", (mynick, source, params));

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

    Magick::instance().nickserv.GetStored(source).AIM(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Magick::instance().getMessage(source, "NS_SET/AIM"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/AIM"),
		source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (
		Magick::instance().getMessage(source, "NS_SET/AIM")));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/AIM"),
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

    Magick::instance().nickserv.GetStored(source).Description(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
		Magick::instance().getMessage(source, "NS_SET/DESCRIPTION"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/DESCRIPTION"),
		source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (
		Magick::instance().getMessage(source, "NS_SET/DESCRIPTION")));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/DESCRIPTION"),
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

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
		target));
	return;
    }

    target = Magick::instance().getSname(target);
    if (Magick::instance().nickserv.GetStored(target).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						target));
	return;
    }

    if (comment.IsSameAs("none", true))
	comment.erase();

    Magick::instance().nickserv.GetStored(target).Comment(comment);
    Magick::instance().nickserv.stats.i_Set++;
    if (!comment.empty())
    {
	SEND(mynick, source, "NS_OTH_COMMAND/SET_TO", (
		Magick::instance().getMessage(source, "NS_SET/COMMENT"),
		target, comment));
	LOG(LM_DEBUG, "NICKSERV/SET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/COMMENT"),
		target, comment));
    }
    else
    {
	SEND(mynick, source, "NS_OTH_COMMAND/UNSET", (
		Magick::instance().getMessage(source, "NS_SET/COMMENT"),
		target));
	LOG(LM_DEBUG, "NICKSERV/UNSET", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().getMessage("NS_SET/COMMENT"),
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

    if (Magick::instance().nickserv.PicExt().empty())
    {
	NSEND(mynick, source, "NS_YOU_STATUS/PICDISABLED");
	return;
    }

    Magick::instance().nickserv.stats.i_SetPicture++;
    if (params.WordCount(" ") > 2 &&
	params.ExtractWord(3, " ").IsSameAs("none", true))
    {
	if (Magick::instance().nickserv.GetStored(source).PicNum())
	    Magick::instance().filesys.EraseFile(FileMap::Picture,
		Magick::instance().nickserv.GetStored(source).PicNum());
	Magick::instance().nickserv.GetStored(source).GotPic(0u);
	NSEND(mynick, source, "NS_YOU_COMMAND/REMOVED");
	LOG(LM_DEBUG, "NICKSERV/PICTURE_DEL", (
		Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		source));
    }
    else
    {
	if (Magick::instance().nickserv.GetStored(source).PicNum())
	    Magick::instance().filesys.EraseFile(FileMap::Picture,
		Magick::instance().nickserv.GetStored(source).PicNum());
	Magick::instance().nickserv.GetStored(source).GotPic(0u);
	Magick::instance().nickserv.GetLive(source).InFlight.Picture(mynick);
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

    if (Magick::instance().nickserv.GetStored(source).L_Protect())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Magick::instance().getMessage(source, "NS_SET/PROTECT")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Magick::instance().nickserv.GetStored(source).Protect(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO", (
			Magick::instance().getMessage(source, "NS_SET/PROTECT"),
			(onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/PROTECT"),
	source, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
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

    if (Magick::instance().nickserv.GetStored(source).L_Secure())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Magick::instance().getMessage(source, "NS_SET/SECURE")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Magick::instance().nickserv.GetStored(source).Secure(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO", (
			Magick::instance().getMessage(source, "NS_SET/SECURE"),
			(onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/SECURE"),
	source, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
		nickname));
	return;
    }
    nickname = Magick::instance().getSname(nickname);

    if (Magick::instance().nickserv.GetStored(nickname).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_NoExpire())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (
				Magick::instance().getMessage(source, "NS_SET/NOEXPIRE"),
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_NoExpire())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Magick::instance().nickserv.GetStored(nickname).NoExpire(onoff.GetBool());
    Magick::instance().nickserv.stats.i_NoExpire++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_SET_TO", (
			Magick::instance().getMessage(source, "NS_SET/NOEXPIRE"),
			nickname, (onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_NOTICE, "NICKSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/NOEXPIRE"),
	nickname, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
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

    if (Magick::instance().nickserv.GetStored(source).L_NoMemo())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Magick::instance().getMessage(source, "NS_SET/NOMEMO")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Magick::instance().nickserv.GetStored(source).NoMemo(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO", (
			Magick::instance().getMessage(source, "NS_SET/NOMEMO"),
			(onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/NOMEMO"),
	source, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
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

    if (Magick::instance().nickserv.GetStored(source).L_Private())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Magick::instance().getMessage(source, "NS_SET/PRIVATE")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Magick::instance().nickserv.GetStored(source).Private(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO", (
			Magick::instance().getMessage(source, "NS_SET/PRIVATE"),
			(onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/PRIVATE"),
	source, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
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

    if (Magick::instance().nickserv.GetStored(source).L_PRIVMSG())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Magick::instance().getMessage(source, "NS_SET/PRIVMSG")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Magick::instance().nickserv.GetStored(source).PRIVMSG(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO", (
			Magick::instance().getMessage(source, "NS_SET/PRIVMSG"),
			(onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/PRIVMSG"),
	source, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_set_Language(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_set_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
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

    if (Magick::instance().nickserv.GetStored(source).L_Language())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (
				Magick::instance().getMessage(source, "NS_SET/LANGUAGE")));
	return;
    }

    if (lang.IsSameAs("default", true) || lang.IsSameAs("reset", true))
    {
	lang = Magick::instance().nickserv.DEF_Language().UpperCase();
    }
    else
    {
	// check for valid language ...
	if (!mFile::Exists(Magick::instance().files.Langdir()+DirSlash+lang.LowerCase()+".lng"))
	{
	    SEND(mynick, source, "OS_STATUS/NOLANG", (
			lang));
	    return;
	}
    }

    Magick::instance().nickserv.GetStored(source).Language(lang);
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (
			Magick::instance().getMessage(source, "NS_SET/LANGUAGE"),
			mstring(lang + " (" +
			Magick::instance().getMessage(source, "ERR_SYNTAX/TRANSLATED") + ")")));
    LOG(LM_DEBUG, "NICKSERV/SET", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/LANGUAGE"),
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_Protect())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/PROTECT")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_Protect(false);
    nick.Protect(onoff.GetBool());
    nick.L_Protect(true);
    }
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED", (
			Magick::instance().getMessage(source, "NS_SET/PROTECT"),
			nickname, (onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/PROTECT"),
	nickname, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_Secure())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/SECURE")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_Secure(false);
    nick.Secure(onoff.GetBool());
    nick.L_Secure(true);
    }
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED", (
			Magick::instance().getMessage(source, "NS_SET/SECURE"),
			nickname, (onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/SECURE"),
	nickname, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/NOMEMO")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_NoMemo(false);
    nick.NoMemo(onoff.GetBool());
    nick.L_NoMemo(true);
    }
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED", (
			Magick::instance().getMessage(source, "NS_SET/NOMEMO"),
			nickname, (onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/NOMEMO"),
	nickname, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_Private())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/PRIVATE")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_Private(false);
    nick.Private(onoff.GetBool());
    nick.L_Private(true);
    }
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED", (
			Magick::instance().getMessage(source, "NS_SET/PRIVATE"),
			nickname, (onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/PRIVATE"),
	nickname, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/PRIVMSG")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_PRIVMSG(false);
    nick.PRIVMSG(onoff.GetBool());
    nick.L_PRIVMSG(true);
    }
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED", (
			Magick::instance().getMessage(source, "NS_SET/PRIVMSG"),
			nickname, (onoff.GetBool() ?
				Magick::instance().getMessage(source, "VALS/ON") :
				Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/PRIVMSG"),
	nickname, (onoff.GetBool() ?
		Magick::instance().getMessage("VALS/ON") :
		Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_lock_Language(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("NickServ::do_lock_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Magick::instance().ircsvchandler != NULL &&
	Magick::instance().ircsvchandler->HTM_Level() > 3)
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (lang.IsSameAs("default", true) || lang.IsSameAs("reset", true))
    {
	lang = Magick::instance().nickserv.DEF_Language().UpperCase();
    }
    else if (!mFile::Exists(Magick::instance().files.Langdir()+DirSlash+lang.LowerCase()+".lng"))
    {
	SEND(mynick, source, "OS_STATUS/NOLANG", (
			lang));
	return;
    }

    if (Magick::instance().nickserv.LCK_Language())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/LANGUAGE")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_Language(false);
    nick.Language(lang);
    nick.L_Language(true);
    }
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/LOCKED", (
			Magick::instance().getMessage(source, "NS_SET/LANGUAGE"),
			nickname, mstring(lang + " (" +
			Magick::instance().getMessageL(lang, "ERR_SYNTAX/TRANSLATED") + ")")));
    LOG(LM_DEBUG, "NICKSERV/LOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/LANGUAGE"),
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Protect())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/PROTECT")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_Protect(false);
    }
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (
			Magick::instance().getMessage(source, "NS_SET/PROTECT"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/PROTECT"),
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Secure())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/SECURE")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_Secure(false);
    }
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (
			Magick::instance().getMessage(source, "NS_SET/SECURE"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/SECURE"),
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/NOMEMO")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_NoMemo(false);
    }
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (
			Magick::instance().getMessage(source, "NS_SET/NOMEMO"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/NOMEMO"),
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Private())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/PRIVATE")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_Private(false);
    }
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (
			Magick::instance().getMessage(source, "NS_SET/PRIVATE"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/PRIVATE"),
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/PRIVMSG")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_PRIVMSG(false);
    }
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (
			Magick::instance().getMessage(source, "NS_SET/PRIVMSG"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/PRIVMSG"),
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

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
				nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Language())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (
				Magick::instance().getMessage(source, "NS_SET/LANGUAGE")));
	return;
    }

    { RLOCK(("NickServ", "stored", nickname.LowerCase()));
    Nick_Stored_t &nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick.Name();
    if (nick.Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
						nickname));
	return;
    }

    nick.L_Language(false);
    }
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/UNLOCKED", (
			Magick::instance().getMessage(source, "NS_SET/LANGUAGE"),
			nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK", (
	Magick::instance().nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
	Magick::instance().getMessage("NS_SET/LANGUAGE"),
	nickname));
}

SXP::Tag NickServ::tag_NickServ("NickServ");

void NickServ::BeginElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
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

void NickServ::EndElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    static_cast<void>(pIn);
    static_cast<void>(pElement);

    FT("NickServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void NickServ::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    static_cast<void>(attribs);

    FT("NickServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_NickServ);

    NickServ::stored_t::iterator iter;
    { RLOCK(("NickServ", "stored"));
    for (iter = StoredBegin(); iter != StoredEnd(); iter++)
    {
	RLOCK2(("NickServ", "stored", iter->first));
	pOut->WriteSubElement(&iter->second);
    }}

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
    RLOCK(("NickServ", "stored"));
    for (iter=StoredBegin(); iter!=StoredEnd(); iter++)
    {
	RLOCK2(("NickServ", "stored", iter->first));
	if (!iter->second.i_Host.empty())
	{
	    if (IsStored(iter->second.i_Host))
	    {
		COM(("Nickname %s has been linked to %s ...",
		    iter->first.c_str(), iter->second.i_Host.c_str()));
		WLOCK(("NickServ", "stored", iter->second.i_Host));
		GetStored(iter->second.i_Host).i_slaves.insert(iter->first);
	    }
	    else
	    {
		LOG(LM_WARNING, "ERROR/HOST_NOTREGD", (
		    iter->second.i_Host, iter->first));
		WLOCK(("NickServ", "stored", iter->first));
		iter->second.i_Host.erase();
	    }
	}
    }
}
