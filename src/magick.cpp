#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#pragma implementation "language.h"
#pragma implementation "logfile.h"
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
#define RCSID(x,y) const char *rcsid_magick_cpp_ ## x () { return y; }
RCSID(magick_cpp, "@(#)$Id$");

/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.344  2002/01/13 05:18:41  prez
** More formatting, changed style slightly
**
** Revision 1.343  2002/01/12 14:42:09  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.342  2002/01/10 19:30:38  prez
** FINALLY finished a MAJOR overhaul ... now have a 'safe pointer', that
** ensures that data being used cannot be deleted while still being used.
**
** Revision 1.341  2002/01/02 08:30:09  prez
** Fixed the shutdown code.  Also added a thread manager as a magick member.
**
** Revision 1.340  2002/01/02 04:44:57  prez
** Made the main loop wait for events to close instead of trying to close
** them manually ...
**
** Revision 1.339  2001/12/27 04:54:46  prez
** Converted SXP to not use STL strings, use mstring instead.
**
** Revision 1.338  2001/12/25 08:43:12  prez
** Fixed XML support properly ... it now works again with new version of
** expat (1.95.2) and sxp (1.1).  Also removed some of my const hacks.
**
** Revision 1.337  2001/12/24 22:54:42  prez
** Removed error on systems where pthread_t is not an int
**
** Revision 1.336  2001/12/21 05:02:29  prez
** Changed over from using a global ACE_Reactor to using an instance inside
** of the Magick instance.
**
** Revision 1.335  2001/12/20 08:02:32  prez
** Massive change -- 'Parent' has been changed to Magick::instance(), will
** soon also move the ACE_Reactor over, and will be able to have multipal
** instances of Magick in the same process if necessary.
**
** Revision 1.334  2001/12/12 07:19:20  prez
** Added check for snprintf, and changed *toa functions to use snprintf.  Also
** moved magick::snprintf and magick::vsnprintf to just snprintf and vsnprintf
** for systems without the system calls.  Made them inline.  Finally, made
** mstring's copy() commands for non-string types use *toa functions.
**
** Revision 1.333  2001/12/12 03:31:15  prez
** Re-wrote the occurances/find/replace functions in mstring to actually work
** with contents that includes a binary 0.  Also fixed PreParse in mconfig.
**
** Revision 1.332  2001/11/30 09:01:56  prez
** Changed Magick to have Init(), Start(), Run(), Stop(), Finish() and
** Pause(bool) functions. This should help if/when we decide to implement
** Magick running as an NT service.
**
** Revision 1.331  2001/11/30 07:30:07  prez
** Added some windows stuff ...
**
** Revision 1.330  2001/11/28 13:40:47  prez
** Added UMASK option to config.  Also made the 'dead thread' protection
** send a SIGIOT signal to try and get the thread to die gracefully, else
** it will do the cancel it used to do.
**
** Revision 1.329  2001/11/18 03:26:53  prez
** More changes re: trace names, and made the command system know the
** difference between 'insufficiant access' and 'unknown command'.
**
** Revision 1.328  2001/11/18 01:54:04  prez
** Fixed up trace levels (again)
**
** Revision 1.327  2001/11/17 03:16:02  prez
** Extra logging, actually made DCC identify as a DCC thread, and fixed some
** mkdir failures ...
**
** Revision 1.326  2001/11/16 20:27:33  prez
** Added a MAX_THREADS option, and made the thread heartbeat a timer based
** operation, instead of part of the threads.
**
** Revision 1.325  2001/11/12 01:05:02  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.324  2001/11/06 09:31:15  prez
** Changed to display a message on windows mode, informing of console state.
**
** Revision 1.323  2001/11/05 05:09:39  prez
** Windows updates, namely disabling fork() on windows.
**
** Revision 1.322  2001/11/03 21:02:53  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.321  2001/07/29 21:22:26  prez
** Delayed clone akills on sync until AFTER we're synced
**
** Revision 1.320  2001/07/29 03:12:23  prez
** Fixed up stuff for the NEW rn4.0
**
** Revision 1.319  2001/07/15 07:35:38  prez
** Fixed problem of it removing access list entries on slave nickname drop.
** Also fixed it so it wouldnt ignore ini entries that were deliberately blank.
**
** Revision 1.318  2001/07/08 01:37:55  prez
** Verified encryption works ...
**
** Revision 1.317  2001/07/01 05:02:45  prez
** Added changes to dependancy system so it wouldnt just remove a dependancy
** after the first one was satisfied.
**
** Revision 1.316  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.315  2001/06/03 02:12:44  prez
** Fixed problem with compress stage not recognizing its end ...
**
** Revision 1.314  2001/06/02 16:27:04  prez
** Intergrated the staging system for dbase loading/saving.
**
** Revision 1.313  2001/05/28 11:17:34  prez
** Added some more anti-deadlock stuff, and fixed nick ident warnings
**
** Revision 1.312  2001/05/22 22:57:10  prez
** Fixed nick linking/idetify, and recognition of committee heads.
**
** Revision 1.311  2001/05/17 19:18:55  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.310  2001/05/14 14:48:00  prez
** Ugh, another error in encryption fixed ...
**
** Revision 1.309  2001/05/14 04:46:32  prez
** Changed to use 3BF (3 * blowfish) encryption.  DES removed totally.
**
** Revision 1.308  2001/05/13 18:45:15  prez
** Fixed up the keyfile validation bug, and added more error reporting to
** the db load (also made sure it did not hang on certain circumstances).
**
** Revision 1.307  2001/05/13 00:55:18  prez
** More patches to try and fix deadlocking ...
**
** Revision 1.306  2001/05/08 15:51:41  prez
** Added some security stuff with committees, so certain things are guarenteed
** on database load (ie. the magick.ini assertions).
**
** Revision 1.305  2001/05/08 06:29:55  prez
** Made ALL have a head of ADMIN and REGD a head of SOP.
**
** Revision 1.304  2001/05/07 03:14:20  prez
** Changes to fix bsd
**
** Revision 1.303  2001/05/06 03:03:07  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.302  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.301  2001/05/03 04:40:18  prez
** Fixed locking mechanism (now use recursive mutexes) ...
** Also now have a deadlock/nonprocessing detection mechanism.
**
** Revision 1.300  2001/05/02 02:35:27  prez
** Fixed dependancy system, and removed printf's - we no longer coredump on
** a 1000 user network.  As a bonus, we actually synd perfectly ;P
**
** Revision 1.299  2001/05/01 14:00:23  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.298  2001/04/22 21:45:20  prez
** Fixed make / make install and RPM spec
**
** Revision 1.297  2001/04/13 07:12:48  prez
** Changed genrankeys style random key generation to binary stamping
** (allowing people to stamp the binary AFTER it has been created, and
** thus, allowing pre-compiled binaries to be stamped for security).
**
** Revision 1.296  2001/04/08 18:53:09  prez
** It now all compiles and RUNS with -fno-default-inline OFF.
**
** Revision 1.295  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.294  2001/03/27 16:09:42  prez
** Fixed chanserv internal maps problem (inserted with incorrect case)
**
** Revision 1.293  2001/03/27 07:04:31  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.292  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.291  2001/03/08 08:07:41  ungod
** fixes for bcc 5.5
**
** Revision 1.290  2001/03/04 02:04:14  prez
** Made mstring a little more succinct ... and added vector/list operations
**
** Revision 1.289  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.288  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.287  2001/02/03 02:21:33  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.286  2001/01/15 23:31:39  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.285  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.284  2000/12/31 17:55:13  prez
** Moved SIGALRM to the ignored signals.
**
** Revision 1.283  2000/12/29 15:46:13  prez
** fixed up log validation
**
** Revision 1.282  2000/12/29 15:31:55  prez
** Added locking/checking for dcc/events threads.  Also for ACE_Log_Msg
**
** Revision 1.281  2000/12/29 13:55:09  prez
** Compiled with 5.1.11, some changes to accomodate (will work with older
** versions of ace still).
**
** Revision 1.280  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.279  2000/12/22 19:50:19  prez
** Made all config options const.  Beginnings of securing all non-modifying
** commands to const.  also added serviceschk.
**
** Revision 1.278  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.277  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.276  2000/11/09 10:58:19  prez
** THINK I have it working again ... with the free list.
** Will check, still thinking of sorting free list by size.
**
** Revision 1.275  2000/10/26 07:59:52  prez
** The goddamn memory system and mstring WORK!  Well, so far ;)
**
** Revision 1.274  2000/10/10 11:47:51  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.273  2000/09/30 10:48:08  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.272  2000/09/22 12:26:11  prez
** Fixed that pesky bug with chanserv not seeing modes *sigh*
**
** Revision 1.271  2000/09/13 12:45:34  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.270  2000/09/09 02:17:48  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.269  2000/09/06 11:27:33  prez
** Finished the T_Modify / T_Changing traces, fixed a bug in clone
** adding (was adding clone liimt as the mask length), updated docos
** a little more, and added a response to SIGINT to signon clients.
**
** Revision 1.268  2000/09/02 07:20:45  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.267  2000/08/31 06:25:09  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.266  2000/08/28 10:51:37  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.265  2000/08/22 08:43:41  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.264  2000/08/19 15:17:39  ungod
** no message
**
** Revision 1.263  2000/08/19 10:59:47  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.262  2000/08/10 22:44:23  prez
** Added 'binding to IP' options for shell servers, etc.  Also added akick
** triggers for when a user changes their nick and suddenly matches akick.
**
** Revision 1.261  2000/08/09 12:14:43  prez
** Ensured chanserv infinate loops wont occur, added 2 new cmdline
** paramaters, and added a manpage (you need to perl2pod it tho).
**
** Revision 1.260  2000/08/08 09:58:56  prez
** Added ModeO to 4 pre-defined committees.
** Also added back some deletes in xml in the hope that it
** will free up some memory ...
**
** Revision 1.259  2000/08/06 05:27:47  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.258  2000/08/03 13:06:31  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.257  2000/07/30 09:04:05  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.256  2000/07/29 21:58:53  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.255  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.254  2000/07/23 18:47:39  prez
** Removed antlr, I dont think we need it, we dont use it!
**
** Revision 1.253  2000/07/21 00:18:49  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.252  2000/07/11 13:22:18  prez
** Fixed loading/saving -- they now work with encryption and compression.
** Tested, it works too!  Now all we need to do is fix the loading, and
** we're set ... :))
**
** Revision 1.251  2000/06/29 06:30:57  prez
** Added the support for the 'extra' chars (ie. at the end of a string)
** so we support odd-length strings.  Also updated documentation.
**
** Revision 1.250  2000/06/28 12:20:48  prez
** Lots of encryption stuff, but essentially, we now have random
** key generation for the keyfile keys, and we can actually encrypt
** something, and get it back as we sent it in (specifically, the
** keyfile itself).
**
** Revision 1.249  2000/06/26 11:23:17  prez
** Added auto-akill on clone triggers
**
** Revision 1.248  2000/06/18 12:49:27  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.247  2000/06/15 13:41:11  prez
** Added my tasks to develop *grin*
** Also did all the chanserv live locking (stored to be done).
** Also made magick check if its running, and kill on startup if so.
**
** Revision 1.246  2000/06/12 08:15:36  prez
** Added 'minimum threads' option to config (set to 2)
**
** Revision 1.245  2000/06/12 06:07:50  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.244  2000/06/11 09:30:20  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.243  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.242  2000/06/08 13:07:34  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.241  2000/06/06 08:57:56  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.240  2000/05/28 02:37:16  prez
** Minor bug fixes (help system and changing nicks)
**
** Revision 1.239  2000/05/27 15:10:12  prez
** Misc changes, mainly re-did the makefile system, makes more sense.
** Also added a config.h file.
**
** Revision 1.238  2000/05/26 11:21:28  prez
** Implemented HTM (High Traffic Mode) -- Can be used at a later date.
**
** Revision 1.237  2000/05/22 13:25:12  ungod
** making it compilable again after removal of log.cpp/.h
**
** Revision 1.236  2000/05/22 13:00:09  prez
** Updated version.h and some other stuff
**
** Revision 1.235  2000/05/21 04:49:40  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.234  2000/05/20 17:00:18  prez
** Added in the VERBOSE mode, now we mimic old logging
**
** Revision 1.233  2000/05/20 16:05:07  prez
** Finished off the log conversion (still via. wrappers)
**
** Revision 1.232  2000/05/20 15:17:00  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.231  2000/05/20 03:28:11  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.230  2000/05/18 11:49:13  ungod
** no message
**
** Revision 1.229  2000/05/18 10:13:15  prez
** Finished off the mFile structure, and the DCC system, it all works.
**
** Revision 1.228  2000/05/17 14:08:11  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.227  2000/05/17 09:10:35  ungod
** changed most wxOutputStream to ofstream and wxInputStream
** to ifstream
**
** Revision 1.226  2000/05/17 07:47:59  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.225  2000/05/14 06:30:14  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.224  2000/05/14 04:02:53  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.223  2000/05/13 15:50:07  ungod
** Getting Magick::BeginElement right this time.
**
** Revision 1.222  2000/05/13 15:46:33  ungod
** more xmlising magick
**
** Revision 1.221  2000/05/13 15:06:42  ungod
** no message
**
** Revision 1.220  2000/05/03 14:12:22  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.219  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.218  2000/04/18 14:34:23  prez
** Fixed the HELP system, it now loads into memory, and can be unloaded
** with the OS unload command.  The stats however are inaccurate.
**
** Revision 1.217  2000/04/18 10:20:26  prez
** Made helpfiles load on usage, like language files.
**
** Revision 1.216  2000/04/16 14:29:44  prez
** Added stats for usage, and standardized grabbing paths, etc.
**
** Revision 1.215  2000/04/16 06:12:13  prez
** Started adding body to the documentation...
**
** Revision 1.214  2000/04/06 12:52:50  prez
** Various code changes, but mainly added AUTOMAKE/AUTOCONF files :)
**
** Revision 1.213  2000/04/04 03:21:34  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.212  2000/04/03 09:45:23  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.211  2000/03/29 14:00:18  prez
** Fixed the thread pool system, and the watermarks.
**
** Revision 1.210  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.209  2000/03/27 21:26:12  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.208  2000/03/26 14:59:37  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.207  2000/03/24 15:35:18  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.206  2000/03/23 10:22:25  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.205  2000/03/19 08:50:55  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.204  2000/03/15 14:42:58  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.203  2000/03/14 10:05:16  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.202  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.201  2000/03/07 09:53:21  prez
** More helpfile updates (and associated updates).
**
** Revision 1.200  2000/03/02 07:25:11  prez
** Added stuff to do the chanserv greet timings (ie. only greet if a user has
** been OUT of channel over 'x' seconds).  New stored chanserv cfg item.
**
** Revision 1.199  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.198  2000/02/27 02:43:50  prez
** More FileSystem additions, plus created 'what' tool
**
** Revision 1.197  2000/02/23 14:29:05  prez
** Added beginnings of a File Map for stored files.
** Also updated Help files (finished nickserv).
**
** Revision 1.196  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.195  2000/02/21 03:27:39  prez
** Updated language files ...
**
** Revision 1.194  2000/02/17 12:55:05  ungod
** still working on borlandization
**
** Revision 1.193  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.192  2000/02/16 10:24:39  prez
** Updated HELP outputs
**
** Revision 1.191  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.190  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"
#include "mconfig.h"
#include "language.h"
#include "logfile.h"
#ifdef CONVERT
#include "convert_magick.h"
#include "convert_esper.h"
#include "convert_epona.h"
#endif

#ifndef WIN32
static bool nofork = false;
#endif

mDateTime Magick::i_StartTime;

map < ACE_thread_t, Magick * >Magick::InstanceMap;
#ifndef MAGICK_HAS_EXCEPTIONS
static Magick GLOB_Magick;
#endif

#ifdef MAGICK_HAS_EXCEPTIONS
void Magick::register_instance(Magick * ins, ACE_thread_t id) throw(E_Magick)
#else
void Magick::register_instance(Magick * ins, ACE_thread_t id)
#endif
{
    if (ins == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_Magick(E_Magick::T_Invalid));
#endif
	return;
    }

    InstanceMap[id] = ins;
}

void Magick::deregister_instance(ACE_thread_t id)
{
    InstanceMap.erase(id);
}

bool Magick::instance_exists(ACE_thread_t id)
{
    map < ACE_thread_t, Magick * >::iterator iter = InstanceMap.find(id);
    return (iter != InstanceMap.end());
}

#ifdef MAGICK_HAS_EXCEPTIONS
Magick & Magick::instance(ACE_thread_t id) throw(E_Magick)
#else
Magick & Magick::instance(ACE_thread_t id)
#endif
{
    map < ACE_thread_t, Magick * >::iterator iter = InstanceMap.find(id);
    if (iter != InstanceMap.end() && iter->second != NULL)
	return *(iter->second);

#ifdef MAGICK_HAS_EXCEPTIONS
    throw(E_Magick(E_Magick::T_NotFound));
#else
    LOG(LM_CRITICAL, "EXCEPTIONS/MAGICK", (id));
    return GLOB_Magick;
#endif
}

Magick::Magick(int inargc, char **inargv):i_verbose(false), i_level(0), i_pause(false), i_auto(false), i_shutdown(false), i_reconnect(true), i_localhost(0),
i_gotconnect(false), i_connected(false), i_saving(false), dh_timer(0)
{
    char buf[1024], *c;

    i_config_file = "magick.ini";
    for (int i = 0; i < inargc; i++)
	argv.push_back(inargv[i]);
    i_programname = argv[0].RevAfter("/");
    i_ResetTime = mDateTime::CurrentDateTime();

    ircsvchandler = NULL;
    logger = NULL;
    signalhandler = NULL;
    events = NULL;
    dcc = NULL;

    LoadLogMessages("DEFAULT");
    c = ACE_OS::getcwd(buf, 1024);
    if (c == NULL && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/DIROPERROR", ("getcwd", ".", errno, strerror(errno)));
	return;
    }
    else
	i_services_dir = buf;
    CurrentState = Constructed;
}

Magick::~Magick()
{
    // Remove all entries from the instance map pointing to us ...
    vector < ACE_thread_t > chunked;
    map < ACE_thread_t, Magick * >::iterator iter;
    for (iter = InstanceMap.begin(); iter != InstanceMap.end(); iter++)
    {
	if (iter->second == this)
	    chunked.push_back(iter->first);
    }
    for (unsigned int i = 0; i < chunked.size(); i++)
	InstanceMap.erase(chunked[i]);
}

static bool firstrun;

int Magick::Init()
{
    if (CurrentState != Constructed)
    {
	return MAGICK_RET_STATE;
    }

    unsigned int i = 0;
    int j = 0;
    int Result;

    // this is our main routine, when it leaves here, this sucker's done.

#if defined(BUILD_NODE) && defined(BUILD_TYPE)
    if (!sysinfo_node().IsSameAs(BUILD_NODE) || !sysinfo_type().IsSameAs(BUILD_TYPE))
    {
	NLOG(LM_EMERGENCY, "SYS_ERRORS/LOCKED_BIN");
	return MAGICK_RET_LOCKED;
    }
#endif

    // more stuff to do
    unsigned int argc = argv.size();
    mstring errstring;

    for (i = 1; i < argc; i++)
    {
	if (argv[i][0U] == '-')
	{
	    argv[i].MakeLower();
	    if (argv[i] == "--dir")
	    {
		i++;
		if (i == argc || argv[i][0U] == '-')
		{
		    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", ("--dir"));
		    return MAGICK_RET_ERROR;
		}
		i_services_dir = files.MakePath(argv[i]);
	    }
	    else if (argv[i] == "--config")
	    {
		i++;
		if (i == argc || argv[i][0U] == '-')
		{
		    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", ("--config"));
		    return MAGICK_RET_ERROR;
		}
		i_config_file = argv[i];
	    }
#ifdef MAGICK_TRACE_WORKS
	    else if (argv[i] == "--trace")
	    {
		i++;
		if (i == argc || argv[i][0U] == '-')
		{
		    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", ("--trace"));
		    return MAGICK_RET_ERROR;
		}
		if (!argv[i].Contains(":"))
		{
		    NLOG(LM_EMERGENCY, "COMMANDLINE/TRACE_SYNTAX");
		    return MAGICK_RET_ERROR;
		}
		unsigned short level = makehex(argv[i].After(":"));

		if (level == 0)
		{
		    NLOG(LM_EMERGENCY, "COMMANDLINE/ZERO_LEVEL");
		    return MAGICK_RET_ERROR;
		}
		else
		{
		    mstring type = argv[i].Before(":").UpperCase();

		    for (j = 0; j < tt_MAX; j++)
			if (type == threadname[j] || type == "ALL")
			    Trace::TurnSet(static_cast < threadtype_enum > (j), level);
		}
	    }
#endif
	    else if (argv[i] == "--help" || (argv[i][1U] != '-' && argv[i].Contains("?")))
	    {
		dump_help();
		return MAGICK_RET_NORMAL;
	    }
	}
    }
    NFT("Magick::Init");
    j = ACE_OS::chdir(Services_Dir());
    if (j < 0 && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/DIROPERROR", ("chdir", Services_Dir(), errno, strerror(errno)));
	RET(MAGICK_RET_ERROR);
    }

    if (!get_config_values())
    {
	LOG(LM_EMERGENCY, "COMMANDLINE/NO_CFG_FILE", (i_config_file));
	RET(MAGICK_RET_ERROR);
    }

    if (i_shutdown == true)
    {
	NLOG(LM_EMERGENCY, "COMMANDLINE/STOP");
	RET(MAGICK_RET_NORMAL);
    }

    Result = doparamparse();
    if (Result != MAGICK_RET_NORMAL)
	RET(Result);

    firstrun = true;
    CurrentState = Initialized;
    RET(MAGICK_RET_NORMAL);
}

int Magick::Start()
{
    NFT("Magick::Start");
    if (CurrentState != Initialized && CurrentState != Stopped)
    {
	RET(MAGICK_RET_STATE);
    }

    int Result;

    ACE_OS::umask(files.umask);
    reactor().reset_reactor_event_loop();

#ifndef WIN32
    mFile pidfile;

    if (firstrun && mFile::Exists(files.Pidfile()))
    {
	pidfile.Open(files.Pidfile(), "r");
	if (pidfile.IsOpened())
	{
	    mstring dummystring = pidfile.ReadLine();
	    pid_t pid = atoi(dummystring.c_str());

	    if (ACE::process_active(pid) > 0)
	    {
		LOG(LM_EMERGENCY, "COMMANDLINE/ACTIVE", (i_programname));
		RET(MAGICK_RET_ERROR);
	    }
	    pidfile.Close();
	    mFile::Erase(files.Pidfile());
	}
    }

#else /* !WIN32 */
    // Must do this BEFORE the logger is started ...
    NLOG(LM_INFO, "COMMANDLINE/START_NOFORK");
#endif

    if (!ActivateLogger())
    {
	RET(MAGICK_RET_ERROR);
    }
    FLUSH();

    Result = ACE_OS::getuid();
    if (Result == 0)
    {
	NLOG(LM_ALERT, "SYS_ERRORS/RUN_AS_ROOT");
    }

    // Need to shut down, it wont be carried over fork.
    // We will re-start it ASAP after fork.
#ifndef WIN32
    if (!nofork && firstrun)
    {
	NLOG(LM_STARTUP, "COMMANDLINE/START_FORK");
	errno = 0;
	Result = ACE::daemonize(Services_Dir(), 0, i_programname);
	if (Result < 0 && errno)
	{
	    LOG(LM_EMERGENCY, "SYS_ERRORS/OPERROR", ("fork", errno, strerror(errno)));
	}
	else if (Result != 0)
	{
	    RET(MAGICK_RET_NORMAL);
	}
	Magick::register_instance(this);
	firstrun = false;
	ACE_LOG_MSG->sync(ProgramName().c_str());
	errno = 0;
	Result = ACE_OS::setpgid(0, 0);
	if (Result < 0 && errno)
	{
	    LOG(LM_WARNING, "SYS_ERRORS/OPERROR", ("setpgid", errno, strerror(errno)));
	}
	ACE_OS::umask(files.umask);

	pidfile.Open(files.Pidfile(), "w");
	if (pidfile.IsOpened())
	{
	    mstring dummystring;

	    dummystring << getpid();
	    pidfile.Write(dummystring);
	    pidfile.Close();
	}
    }
#endif

    // okay here we start setting up the ACE_Reactor and ACE_Event_Handler's
    signalhandler = new SignalHandler;
    reactor().register_handler(SIGINT, signalhandler);
#if defined(SIGTERM) && (SIGTERM != 0)
    reactor().register_handler(SIGTERM, signalhandler);
#endif
#if defined(SIGPIPE) && (SIGPIPE != 0)
    reactor().register_handler(SIGPIPE, signalhandler);
#endif
#if defined(SIGQUIT) && (SIGQUIT != 0)
    reactor().register_handler(SIGQUIT, signalhandler);
#endif
    reactor().register_handler(SIGSEGV, signalhandler);
#ifdef SIGBUS
    reactor().register_handler(SIGBUS, signalhandler);
#endif
#ifdef SIGHUP
    reactor().register_handler(SIGHUP, signalhandler);
#endif
    reactor().register_handler(SIGILL, signalhandler);
#ifdef SIGTRAP
    reactor().register_handler(SIGTRAP, signalhandler);
#endif
#ifdef SIGIOT
    reactor().register_handler(SIGIOT, signalhandler);
#endif
#ifdef SIGFPE
    reactor().register_handler(SIGFPE, signalhandler);
#endif
#ifdef SIGWINCH
    reactor().register_handler(SIGWINCH, signalhandler);
#endif
#ifdef SIGTTIN
    reactor().register_handler(SIGTTIN, signalhandler);
#endif
#ifdef SIGTTOU
    reactor().register_handler(SIGTTOU, signalhandler);
#endif
#ifdef SIGTSTP
    reactor().register_handler(SIGTSTP, signalhandler);
#endif

#if 0

// Linux POSIX threads (pre-lxpthreads) use these ...
#if defined(SIGUSR1) && (SIGUSR1 != 0)
    reactor().register_handler(SIGUSR1, signalhandler);
#endif
#if defined(SIGUSR2) && (SIGUSR2 != 0)
    reactor().register_handler(SIGUSR2, signalhandler);
#endif

// AIX uses this for zombie protection ...
#if defined(SIGALRM) && (SIGALRM != 0)
    reactor().register_handler(SIGALRM, signalhandler);
#endif
#ifdef SIGCHLD
    reactor().register_handler(SIGCHLD, signalhandler);
#endif

#endif /* 0 */

    // load the local messages database and internal "default messages"
    // the external messages are part of a separate ini called english.lng (both local and global can be done here too)
    NLOG(LM_STARTUP, "COMMANDLINE/START_LANG");
    LoadInternalMessages();
    FLUSH();

    load_databases();
    FLUSH();

    //NLOG(LM_STARTUP, "COMMANDLINE/START_CALIBRATE");
    // calibrate the threshholds.
    //
    // register 250 nicks and 1000 channels (to random nicknames in the nick pool).
    // and add access rand() % 20 random nicknames to the access list of each channel.
    // then go through and delete each nick (it will remove access list and channel entries)
    // This should go on for 60s. First and last 5 seconds are disgarded.
    // 
    // number of iterations/500 is low_water_mark, number of itereations/200 = high_water_mark
    // TODO: how to work out max_thread_pool for all of magick?

    LOG(LM_STARTUP, "COMMANDLINE/START_COMPLETE", (PACKAGE, VERSION));
    // Can only open these after fork if we want then to live
    {
	WLOCK(("Events"));
	events = new EventTask(&thr_mgr());
	if (events == NULL)
	{
	    RET(MAGICK_RET_ERROR);
	}
    }
    {
	WLOCK(("DCC"));
	dcc = new DccMap(&thr_mgr());
	if (dcc == NULL)
	{
	    RET(MAGICK_RET_ERROR);
	}
    }

    CurrentState = Started;
    RET(MAGICK_RET_NORMAL);
}

int Magick::Run()
{
    NFT("Magick::Run");
    if (CurrentState != Started)
    {
	RET(MAGICK_RET_STATE);
    }

    // Use the reconnect handler to get a connection

    NLOG(LM_STARTUP, "COMMANDLINE/START_EVENTS");
    {
	WLOCK(("Events"));
	if (events != NULL)
	    events->open((void *) this);
    }
    {
	WLOCK(("DCC"));
	if (dcc != NULL)
	    dcc->open((void *) this);
    }

    reactor().schedule_timer(&Magick::instance().hh, 0, ACE_Time_Value(Magick::instance().config.Heartbeat_Time()));
    reactor().schedule_timer(&(Magick::instance().rh), 0, ACE_Time_Value::zero);
    AUTO(true);			// Activate events from here.

    // next thing to be done here is set up the acceptor mechanism to listen
    // for incoming "magickgui" connections and handle them.

    // This is the main loop.  When we get a Shutdown(),
    // we wait for everything to shutdown cleanly.
    // All that will be left is US and the LOGGER.

    DumpB();
    CurrentState = Running;
    reactor().run_reactor_event_loop();
    DumpE();
    FLUSH();

    CurrentState = RunCompleted;
    RET(MAGICK_RET_NORMAL);
}

int Magick::Stop()
{
    NFT("Magick::Stop");
    if (CurrentState != Started && CurrentState != Running && CurrentState != RunCompleted)
    {
	RET(MAGICK_RET_STATE);
    }

    if (CurrentState == Running)
    {
	reactor().end_reactor_event_loop();
	RET(MAGICK_RET_NORMAL);
    }

    int Result;

    if (Shutdown())
	Result = MAGICK_RET_NORMAL;
    else
	Result = MAGICK_RET_RESTART;

    Shutdown(true);
    Disconnect(false);

    NLOG(LM_STARTUP, "COMMANDLINE/STOP_EVENTS");

    //todo work out some way to "ignore" signals
    reactor().remove_handler(SIGINT);
#if defined(SIGTERM) && (SIGTERM != 0)
    reactor().remove_handler(SIGTERM);
#endif
#if defined(SIGPIPE) && (SIGPIPE != 0)
    reactor().remove_handler(SIGPIPE);
#endif
#if defined(SIGQUIT) && (SIGQUIT != 0)
    reactor().remove_handler(SIGQUIT);
#endif
    reactor().remove_handler(SIGSEGV);
#ifdef SIGBUS
    reactor().remove_handler(SIGBUS);
#endif
#ifdef SIGHUP
    reactor().remove_handler(SIGHUP);
#endif
    reactor().remove_handler(SIGILL);
#ifdef SIGTRAP
    reactor().remove_handler(SIGTRAP);
#endif
#ifdef SIGIOT
    reactor().remove_handler(SIGIOT);
#endif
#ifdef SIGFPE
    reactor().remove_handler(SIGFPE);
#endif
#ifdef SIGWINCH
    reactor().remove_handler(SIGWINCH);
#endif
#ifdef SIGTTIN
    reactor().remove_handler(SIGTTIN);
#endif
#ifdef SIGTTOU
    reactor().remove_handler(SIGTTOU);
#endif
#ifdef SIGTSTP
    reactor().remove_handler(SIGTSTP);
#endif

#if 0

// Linux POSIX threads (pre-lxpthreads) use these ...
#if defined(SIGUSR1) && (SIGUSR1 != 0)
    reactor().remove_handler(SIGUSR1);
#endif
#if defined(SIGUSR2) && (SIGUSR2 != 0)
    reactor().remove_handler(SIGUSR2);
#endif

// AIX uses this for zombie protection ...
#if defined(SIGALRM) && (SIGALRM != 0)
    reactor().remove_handler(SIGALRM);
#endif
#ifdef SIGCHLD
    reactor().remove_handler(SIGCHLD);
#endif

#endif /* 0 */

    if (signalhandler != NULL)
    {
	delete signalhandler;

	signalhandler = NULL;
    }

    if (events != NULL)
    {
	events->wait();
	WLOCK(("Events"));
	delete events;

	events = NULL;
    }

    if (dcc != NULL)
    {
	dcc->wait();
	WLOCK(("DCC"));
	delete dcc;

	dcc = NULL;
    }

#ifndef WIN32
    if (Result != MAGICK_RET_RESTART)
	mFile::Erase(files.Pidfile());
#endif

    LOG(LM_STARTUP, "COMMANDLINE/STOP_COMPLETE", (PACKAGE, VERSION));

    DeactivateLogger();

    CurrentState = Stopped;
    RET(Result);
}

int Magick::Finish()
{
    NFT("Magick::Finish");
    if (CurrentState != Initialized && CurrentState != Stopped)
    {
	RET(MAGICK_RET_STATE);
    }
    RET(MAGICK_RET_NORMAL);
}

mstring Magick::getMessage(const mstring & nick, const mstring & name)
{
    FT("Magick::getMessage", (nick, name));

    if (!nick.empty() && nickserv.IsStored(nick) && nickserv.GetStored(nick)->IsOnline())
    {
	CP(("Using USER-DEIFNED language."));
	mstring retval = getMessageL(nickserv.GetStored(nick)->Language(), name);

	RET(retval);
    }
    else
    {
	CP(("Using DEFAULT language."));
	mstring retval = getMessageL(nickserv.DEF_Language(), name);

	RET(retval);
    }
}

mstring Magick::getMessageL(const mstring & lang, const mstring & name)
{
    FT("Magick::getMessageL", (lang, name));

    mstring retval = "Could not find message token \"" + name.UpperCase() + "\", please report this to your Services Admins.";

    // Load requested language if its NOT loaded.
    // and then look for the message of THAT type.
    CP(("Trying SPECIFIED language ..."));
    {
	WLOCK(("Messages", lang.UpperCase()));
	if (!lang.empty() && Messages.find(lang.UpperCase()) == Messages.end())
	{
	    LoadExternalMessages(lang);
	}
    }
    {
	RLOCK(("Messages", lang.UpperCase(), name.UpperCase()));
	if (!lang.empty() && Messages.find(lang.UpperCase()) != Messages.end() &&
	    Messages[lang.UpperCase()].find(name.UpperCase()) != Messages[lang.UpperCase()].end())
	{
	    retval = Messages[lang.UpperCase()][name.UpperCase()];
	    RET(retval);
	}
    }

    // Load nickserv default language if its NOT loaded.
    // and then look for the message of THAT type.
    CP(("Trying DEFAULT language ..."));
    {
	WLOCK(("Messages", nickserv.DEF_Language().UpperCase()));
	if (lang.UpperCase() != nickserv.DEF_Language().UpperCase() && !nickserv.DEF_Language().empty() &&
	    Messages.find(nickserv.DEF_Language().UpperCase()) == Messages.end())
	{
	    LoadExternalMessages(nickserv.DEF_Language());
	}
    }
    {
	RLOCK(("Messages", nickserv.DEF_Language().UpperCase(), name.UpperCase()));
	if (lang.UpperCase() != nickserv.DEF_Language().UpperCase() && !nickserv.DEF_Language().empty() &&
	    Messages.find(nickserv.DEF_Language().UpperCase()) != Messages.end() &&
	    Messages[nickserv.DEF_Language().UpperCase()].find(name.UpperCase()) !=
	    Messages[nickserv.DEF_Language().UpperCase()].end())
	{
	    retval = Messages[nickserv.DEF_Language().UpperCase()][name.UpperCase()];
	    RET(retval);
	}
    }

    // Otherwise just try and find it in the DEFAULTs.
    CP(("Trying HARD-CODED language ..."));
    {
	RLOCK(("Messages", "DEFAULT", name.UpperCase()));
	if (Messages["DEFAULT"].find(name.UpperCase()) != Messages["DEFAULT"].end())
	{
	    retval = Messages["DEFAULT"][name.UpperCase()];
	    RET(retval);
	}
    }
    LOG(LM_ERROR, "ERROR/NOLANGTOKEN", (name.UpperCase(), lang.UpperCase()));
    RET(retval);
}

mstring Magick::getLogMessage(const mstring & name)
{
    FT("Magick::getLogMessage", (name));

    // Load nickserv default language if its NOT loaded.
    // and then look for the message of THAT type.
    // Otherwise just try and find it in the DEFAULTs.
    // NEVER implement locking here, we log the locks.

    mstring retval = "Could not find log message token \"" + name.UpperCase() + "\", please check your language file.";

    CP(("Trying to get log entry ..."));
    {
	RLOCK(("LogMessages", name.UpperCase()));
	if (LogMessages.find(name.UpperCase()) != LogMessages.end())
	{
	    retval = LogMessages[name.UpperCase()];
	}
    }
    RET(retval);
}

vector < mstring > Magick::getHelp(const mstring & nick, const mstring & name)
{
    FT("Magick::getHelp", (nick, name));

    vector < mstring > helptext;
    mstring language = nickserv.DEF_Language().UpperCase();

    // Load requested language if its NOT loaded.
    // and then look for the Help of THAT type.
    if (!nick.empty() && nickserv.IsStored(nick) && nickserv.GetStored(nick)->IsOnline())
    {
	language = nickserv.GetStored(nick)->Language().UpperCase();
    }

  StartGetLang:
    if (Help.find(language) == Help.end() && mFile::Exists(files.Langdir() + DirSlash + language.LowerCase() + ".hlp"))
    {
	WLOCK(("Help", language));

	mConfigEngine fconf(files.Langdir() + DirSlash + language.LowerCase() + ".hlp");

	map < mstring, mstring > tmp = fconf.GetMap();
	map < mstring, mstring >::iterator i;

	MCB(Help.size());
	mstring section;

	triplet < mstring, mstring, mstring > entry;
	for (i = tmp.begin(); i != tmp.end(); i++)
	{
	    section = i->first.RevBefore("/").UpperCase();
	    entry =
		triplet < mstring, mstring, mstring > (i->second.ExtractWord(1, ":", false),
						       i->second.ExtractWord(2, ":", false), i->second.After(":", 2));

	    if (entry.third.empty())
		entry.third = " ";

	    Help[language][section].push_back(entry);
	}
	MCE(Help.size());
	if (tmp.size())
	{
	    LOG(LM_INFO, "OTHER/LOAD_HELP", (language.UpperCase().c_str()));
	    CP(("Help %s was loaded into memory.", language.c_str()));
	}
    }

    mstring Uname(name.UpperCase());
    unsigned int i, j;

    {
	RLOCK(("Help", language, Uname));
	if (Help.find(language) != Help.end() && Help[language].find(Uname) != Help[language].end())
	{
	    bool sendline;

	    for (j = 0; j < Help[language][Uname].size(); j++)
	    {
		sendline = false;
		if (!Help[language][Uname][j].first.empty())
		{
		    for (i = 1; !sendline && i <= Help[language][Uname][j].first.WordCount(" "); i++)
		    {
			if (commserv.IsList(Help[language][Uname][j].first.ExtractWord(i, " ")) &&
			    commserv.GetList(Help[language][Uname][j].first.ExtractWord(i, " "))->IsOn(nick))
			    sendline = true;
		    }
		}
		else
		    sendline = true;
		if (!Help[language][Uname][j].second.empty())
		    for (i = 1; sendline && i <= Help[language][Uname][j].second.WordCount(" "); i++)
		    {
			if (commserv.IsList(Help[language][Uname][j].second.ExtractWord(i, " ")) &&
			    commserv.GetList(Help[language][Uname][j].second.ExtractWord(i, " "))->IsOn(nick))
			    sendline = false;
		    }
		if (sendline)
		    helptext.push_back(Help[language][Uname][j].third);
	    }
	}
    }

    if (!helptext.size())
    {
	if (language != nickserv.DEF_Language().UpperCase())
	{
	    language = nickserv.DEF_Language().UpperCase();
	    goto StartGetLang;
	}
	mstring tmpstr, tmpstr2;

	tmpstr2 = name;
	tmpstr2.replace("/", " ");
	tmpstr = parseMessage(getMessage(nick, "ERR_SITUATION/NOHELP"), mVarArray(tmpstr2.After(" ")));
	helptext.push_back(tmpstr);
    }
    NRET(vector < mstring >, helptext);
}

void Magick::dump_help() const
{
    // This needs to be re-written.
    cout << "\n" << FULLNAME + " - " + HOMEPAGE +
	"\n" << "    (c) 1997-2001 Preston A. Elder <prez@magick.tm>\n" << "    (c) 1998-2001 William King <ungod@magick.tm>\n"
	<< "\n" << "Syntax: " << i_programname << " [options]\n" << "\n" <<
	"--help             -?      Help output (summary of the below).\n" <<
	"--dir X                    Set the initial services directory.\n" <<
	"--config X                 Set the name of the config file.\n"
#ifndef WIN32
	 << "--nofork                   Do not become a daemon process.\n"
#endif
#ifdef CONVERT
	 << "--convert X                Convert another version of services databases\n" <<
	"                           to Magick II format, where X is the type of\n" <<
	"                           database to convert.  Currently recognized:\n" <<
	"                               magick (1.4), esper (4.4.8), epona (1.4.7)\n" <<
	"                               sirv (N/A)\n"
#endif
#ifdef MAGICK_TRACE_WORKS
	 << "--trace X:Y                Set the trace level on startup, equivilant of\n" <<
	"                           using the OperServ TRACE SET command while\n" <<
	"                           running, where X is the trace type (or ALL),\n" <<
	"                           and Y is the trace level in hex.\n"
#endif
	 << "--name X           -n      Override [STARTUP/SERVER_NAME] to X.\n" <<
	"--desc X           -d      Override [STARTUP/SERVER_DESC] to X.\n" <<
	"--user X           -u      Override [STARTUP/SERVICES_USER] to X.\n" <<
	"--host X           -h      Override [STARTUP/SERVICES_HOST] to X.\n" <<
	"--ownuser          -o      Override [STARTUP/OWNUSER] to true.\n" <<
	"--protocol X       -P      Override [STARTUP/PROTOCOL] to X.\n" <<
	"--level X          -l      Override [STARTUP/LEVEL] to X.\n" <<
	"--lagtime X        -g      Override [STARTUP/LAGTIME] to X.\n" <<
	"--umask            -U      Override [FILES/UMASK] to X.\n" <<
	"--verbose          -v      Override [FILES/VERBOSE] to true.\n" <<
	"--log X            -L      Override [FILES/LOGFILE] to X.\n" <<
	"--logchan X        -C      Override [FILES/LOGCHAN] to X.\n" <<
	"--dbase X          -D      Override [FILES/DATABASE] to X.\n" <<
	"--langdir X        -S      Override [FILES/LANGDIR] to X.\n" <<
	"--encrypt          -E      Override [FILES/ENCRYPTION] to true.\n" <<
	"--decrypt          -e      Override [FILES/ENCRYPTION] to false.\n" <<
	"--keyfile X        -K      Override [FILES/KEYFILE] to X.\n" <<
	"--compress X       -c      Override [FILES/COMPRESSION] to X.\n" <<
	"--relink X         -r      Override [CONFIG/SERVER_RELINK] to X.\n" <<
	"--norelink                 Override [CONFIG/SERVER_RELINK] to 0.\n" <<
	"--cycle X          -t      Override [CONFIG/CYCLETIME] to X.\n" <<
	"--save X           -w      Override [CONFIG/SAVETIME] to X.\n" <<
	"--check X          -T      Override [CONFIG/CHECKTIME] to X.\n" <<
	"--ping X           -p      Override [CONFIG/PING_FREQUENCY] to X.\n" <<
	"--minthreads X     -q      Override [CONFIG/MIN_THREADS] to X.\n" <<
	"--maxthreads X     -Q      Override [CONFIG/MAX_THREADS] to X.\n" <<
	"--lwm X            -m      Override [CONFIG/LOW_WATER_MARK] to X.\n" <<
	"--hwm X            -M      Override [CONFIG/HIGH_WATER_MARK] to X.\n" <<
	"--append           -a      Override [NICKSERV/APPEND_RENAME] to true.\n" <<
	"--rename           -A      Override [NICKSERV/APPEND_RENAME] to false.\n" <<
	"--ident X          -R      Override [NICKSERV/IDENT] to X.\n" <<
	"--language X       -s      Override [NICKSERV/DEF_LANGUAGE] to X.\n" <<
	"--nodcc            -x      Override [NICKSERV/PICEXT] to \"\" and\n" <<
	"                           Override [MEMOSERV/FILES] to 0.\n" <<
	"--inflight X       -f      Override [MEMOSERV/INFLIGHT] to X.\n" <<
	"--logignore        -i      Override [OPERSERV/LOG_IGNORE] to true.\n" <<
	"--ignore X         -I      Override [OPERSERV/IGNORE_METHOD] to X.\n" << "\n" << "For more help on the usage of " +
	mstring(PACKAGE) +
	", please browse the docs directory.\n" << "This released under the GNU General Public License.  Please see the\n" <<
	"\"COPYING\" file for more details.\n\n";

}

void Magick::LoadInternalMessages()
{
    NFT("Magick::LoadInternalMessages");

    WLOCK(("Messages", "DEFAULT"));
    vector < mstring > lang;
    unsigned int i;

    for (i = 0; i < def_langent; i++)
	lang.push_back(def_lang[i]);

    mConfigEngine fconf;

    fconf.LoadFromArray(lang);
    map < mstring, mstring > tmp = fconf.GetMap();
    if (tmp.size())
    {
	MCB(Messages.size());
	Messages.erase("DEFAULT");
	Messages["DEFAULT"] = tmp;
	MCE(Messages.size());
    }
}

bool Magick::LoadExternalMessages(const mstring & language)
{
    FT("Magick::LoadExternalMessages", (language));
    // use the previously created name array to get the names to load

    if (language.UpperCase() == "DEFAULT")
    {
	LoadInternalMessages();
	RET(true);
    }
    else if (mFile::Exists(files.Langdir() + DirSlash + language.LowerCase() + ".lng"))
    {
	WLOCK(("Messages", language.UpperCase()));
	mConfigEngine fconf(files.Langdir() + DirSlash + language.LowerCase() + ".lng");

	map < mstring, mstring > tmp = fconf.GetMap();
	if (tmp.size())
	{
	    MCB(Messages.size());
	    Messages.erase(language.UpperCase());
	    Messages[language.UpperCase()] = tmp;
	    MCE(Messages.size());
	    RET(true);
	}
    }
    RET(false);
}

bool Magick::LoadLogMessages(const mstring & language)
{
    FT("Magick::LoadLogMessages", (language));
    // use the previously created name array to get the names to load

    WLOCK(("LogMessages"));
    LogMessages.clear();
    MCB(LogMessages.size());
    map < mstring, mstring >::iterator iter;
    {
	vector < mstring > lfo;
	unsigned int i;

	for (i = 0; i < def_logent; i++)
	    lfo.push_back(def_log[i]);

	mConfigEngine fconf;

	fconf.LoadFromArray(lfo);

	map < mstring, mstring > tmp = fconf.GetMap();
	for (iter = tmp.begin(); iter != tmp.end(); iter++)
	    LogMessages[iter->first] = iter->second;
	CP(("After DEFAULT - LogMessages = %d", LogMessages.size()));
    }
    if (language != "DEFAULT" && mFile::Exists(files.Langdir() + DirSlash + language.LowerCase() + ".lfo"))
    {
	mConfigEngine fconf(files.Langdir() + DirSlash + language.LowerCase() + ".lfo");

	map < mstring, mstring > tmp = fconf.GetMap();
	for (iter = tmp.begin(); iter != tmp.end(); iter++)
	    LogMessages[iter->first] = iter->second;
    }
    MCE(LogMessages.size());

    if (LogMessages.size())
    {
	RET(true);
    }
    RET(false);
}

bool Magick::UnloadExternalMessages(const mstring & language)
{
    FT("Magick::UnloadExternalMessages", (language));

    if (!language.empty() && Messages.find(language.UpperCase()) != Messages.end())
    {
	WLOCK(("Messages"));
	MCB(Messages.size());
	Messages.erase(language.UpperCase());
	MCE(Messages.size());
	RET(true);
    }
    RET(false);
}

bool Magick::UnloadHelp(const mstring & language)
{
    FT("Magick::UnloadHelp", (language));

    if (!language.empty() && Help.find(language.UpperCase()) != Help.end())
    {
	WLOCK(("Help"));
	MCB(Help.size());
	Help.erase(language.UpperCase());
	MCE(Help.size());
	RET(true);
    }
    RET(false);
}

int Magick::doparamparse()
{
    NFT("Magick::doparamparse");
    mstring temp;
    int argc = argv.size(), i;

    for (i = 1; i < argc; i++)
    {
	if (argv[i][0U] == '-')
	{
	    bool ArgUsed = false;

	    if (argv[i][1U] == '-')
		ArgUsed = paramlong(argv[i], (i + 1 < argc) ? argv[i + 1].c_str() : "");
	    else
		ArgUsed = paramshort(argv[i], (i + 1 < argc) ? argv[i + 1].c_str() : "");

	    if (ArgUsed)
		i++;
	}
	else
	{
	    NLOG(LM_EMERGENCY, "COMMANDLINE/NONOPTION");
	}
    }
    RET(MAGICK_RET_NORMAL);
}

bool Magick::paramlong(const mstring & first, const mstring & second)
{
    FT("Magick::paramlong", (first, second));
    if (first == "--dir" || first == "--config" || first == "--trace")
    {
	// already handled, but we needed to i++
	RET(true);
    }
    else if (first == "--name")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	startup.server_name = second;
	RET(true);
    }
    else if (first == "--desc")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	startup.server_name = second;
	RET(true);
    }
    else if (first == "--user")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	startup.services_user = second;
	RET(true);
    }
    else if (first == "--ownuser")
    {
	startup.ownuser = true;
    }
    else if (first == "--host")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	startup.services_host = second;
    }
    else if (first == "--protocol")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (atoi(second.c_str()) <= 0)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/MUSTBENUMBER", (first));
	}
	if (atoi(second.c_str()) != static_cast < int >(server.proto.Number()))
	{
	    server.proto.Set(atoi(second.c_str()));
	    if (atoi(second.c_str()) != static_cast < int >(server.proto.Number()))
	    {
		LOG(LM_WARNING, "COMMANDLINE/UNKNOWN_PROTO", (second, server.proto.Number()));
	    }
	}
	RET(true);
    }
    else if (first == "--level")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (atoi(second.c_str()) <= 0)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/MUSTBENUMBER", (first));
	}
	startup.level = atoi(second.c_str());
	RET(true);
    }
    else if (first == "--lagtime")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (!FromHumanTime(second))
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/TIMEORZERO", (first));
	}
	startup.lagtime = FromHumanTime(second);
    }
    else if (first == "--verbose" || first == "--debug")
    {
	i_verbose = true;
    }
    else if (first == "--umask")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	unsigned int i;
	mode_t umask = 0;

	for (i = 0; i < second.length(); i++)
	{
	    if (!isdigit(second[i]))
	    {
		LOG(LM_EMERGENCY, "COMMANDLINE/MUSTBENUMBER", (first));
	    }
	    else
	    {
		umask <<= 3;
		umask += second[i] - '0';
	    }
	}
	if (i != 3)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/MUSTHAVEDIGITS", (first, 3, 0, 7));
	}

	files.umask = umask;
	RET(true);
    }
    else if (first == "--log")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	files.logfile = second;
	RET(true);
    }
    else if (first == "--logchan")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	files.logchan = second;
	RET(true);
    }
    else if (first == "--dbase" || first == "--database")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	files.database = second;
	RET(true);
    }
    else if (first == "--langdir")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	files.langdir = second;
	RET(true);
    }
    else if (first == "--encrypt")
    {
	files.encryption = true;
    }
    else if (first == "--decrypt")
    {
	files.encryption = false;
    }
    else if (first == "--keyfile")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (!mFile::Exists(second))
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NO_KEYFILE", (second));
	}
	files.keyfile = second;
	RET(true);
    }
    else if (first == "--compress" || first == "--compression")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (atoi(second.c_str()) < 0)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/MUSTBENUMBER", (first));
	}
	if (atoi(second.c_str()) > 9)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/VALUETOOHIGH", (first, 9));
	}
	files.compression = atoi(second.c_str());
	RET(true);
    }
    else if (first == "--relink")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (atoi(second.c_str()) < 0)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/MUSTBENUMBER", (first));
	}
	config.server_relink = FromHumanTime(second);
	RET(true);
    }
    else if (first == "--norelink")
    {
	config.server_relink = 0;
    }
#ifndef WIN32
    else if (first == "--nofork")
    {
	nofork = true;
    }
#endif
    else if (first == "--cycle" || first == "--expire")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (!FromHumanTime(second))
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/TIMEORZERO", (first));
	}
	config.cycletime = FromHumanTime(second);
	RET(true);
    }
    else if (first == "--save" || first == "--update")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (!FromHumanTime(second))
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/TIMEORZERO", (first));
	}
	config.savetime = FromHumanTime(second);
	RET(true);
    }
    else if (first == "--check" || first == "--hyperactive")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (!FromHumanTime(second))
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/TIMEORZERO", (first));
	}
	config.checktime = FromHumanTime(second);
	RET(true);
    }
    else if (first == "--ping")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (!FromHumanTime(second))
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/TIMEORZERO", (first));
	}
	config.ping_frequency = FromHumanTime(second);
	RET(true);
    }
    else if (first == "--minthreads")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (atoi(second.c_str()) < 0)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/MUSTBENUMBER", (first));
	}
	config.min_threads = atoi(second.c_str());
	if (config.min_threads < 1)
	    config.min_threads = 1;
	if (config.max_threads < config.min_threads)
	    config.max_threads = config.min_threads;
	RET(true);
    }
    else if (first == "--maxthreads")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (atoi(second.c_str()) < 0)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/MUSTBENUMBER", (first));
	}
	config.max_threads = atoi(second.c_str());
	if (config.max_threads < config.min_threads)
	    config.max_threads = config.min_threads;
	RET(true);
    }
    else if (first == "--lwm" || first == "--low_water_mark")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (atoi(second.c_str()) < 0)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/MUSTBENUMBER", (first));
	}
	config.low_water_mark = atoi(second.c_str());
	if (config.high_water_mark < config.low_water_mark)
	    config.high_water_mark = config.low_water_mark;
	RET(true);
    }
    else if (first == "--hwm" || first == "--high_water_mark")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (atoi(second.c_str()) < 0)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/MUSTBENUMBER", (first));
	}
	config.high_water_mark = atoi(second.c_str());
	if (config.high_water_mark < config.low_water_mark)
	    config.high_water_mark = config.low_water_mark;
	RET(true);
    }
    else if (first == "--append")
    {
	nickserv.append_rename = true;
    }
    else if (first == "--rename")
    {
	nickserv.append_rename = false;
    }
    else if (first == "--ident")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (!FromHumanTime(second))
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/TIMEORZERO", (first));
	}
	nickserv.ident = FromHumanTime(second);
	RET(true);
    }
    else if (first == "--language")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	nickserv.def.Language = second;
	RET(true);
    }
    else if (first == "--nodcc")
    {
	nickserv.picext = "";
	memoserv.files = 0;
    }
    else if (first == "--inflight")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (!FromHumanTime(second))
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/TIMEORZERO", (first));
	}
	nickserv.ident = FromHumanTime(second);
	RET(true);
    }
    else if (first == "--logignore")
    {
	operserv.log_ignore = true;
    }
    else if (first == "--ignore")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (atoi(second.c_str()) < 0)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/MUSTBENUMBER", (first));
	}
	if (atoi(second.c_str()) > 9)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/VALUETOOHIGH", (first, 9));
	}
	operserv.ignore_method = atoi(second.c_str());
	RET(true);
    }
#ifdef CONVERT
    else if (first == "--convert")
    {
	if (second.empty() || second[0U] == '-')
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", (first));
	}
	if (second.IsSameAs("magick", true))
	{
	    NLOG(LM_STARTUP, "COMMANDLINE/START_CONVERT");
	    load_ns_dbase();
	    load_cs_dbase();
	    load_ms_dbase();
	    load_news_dbase();
	    load_akill();
	    load_clone();
	    load_sop();
	    load_message();
	}
	else if (second.IsSameAs("esper", true))
	{
	    NLOG(LM_STARTUP, "COMMANDLINE/START_CONVERT");
	    ESP_load_ns_dbase();
	    ESP_load_cs_dbase();
	    ESP_load_os_dbase();
	    ESP_load_akill();
	    ESP_load_news();
	    ESP_load_exceptions();
	}
	else if (second.IsSameAs("epona", true))
	{
	    NLOG(LM_STARTUP, "COMMANDLINE/START_CONVERT");
	    EPO_load_ns_dbase();
	    EPO_load_cs_dbase();
	    EPO_load_os_dbase();
	    EPO_load_news();
	    EPO_load_exceptions();
	}
	else if (second.IsSameAs("sirv", true))
	{
	    LOG(LM_STARTUP, "SYS_ERRORS/NOT_IMPLEMENTED", ("convert_sirv"));
	}
	else
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/CANNOT_CONVERT", (second));
	}
	RET(true);
    }
#endif
    else
    {
	LOG(LM_ERROR, "COMMANDLINE/UNKNOWN_OPTION", (first));
    }
    RET(false);
}

bool Magick::paramshort(const mstring & first, const mstring & second)
{
    FT("Magick::paramshort", (first, second));
    bool ArgUsed = false;

    for (unsigned int i = 1; i < first.length(); i++)
    {
	if (first[i] == '?')
	{
	    // Already handled
	}
	else if (first[i] == 'n')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--name", second);
	}
	else if (first[i] == 'd')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--desc", second);
	}
	else if (first[i] == 'u')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--user", second);
	}
	else if (first[i] == 'h')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--host", second);
	}
	else if (first[i] == 'o')
	{
	    ArgUsed = paramlong("--ownuser", second);
	}
	else if (first[i] == 'P')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--protocol", second);
	}
	else if (first[i] == 'l')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--level", second);
	}
	else if (first[i] == 'g')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--lagtime", second);
	}
	else if (first[i] == 'v')
	{
	    ArgUsed = paramlong("--verbose", second);
	}
	else if (first[i] == 'U')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--umask", second);
	}
	else if (first[i] == 'L')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--log", second);
	}
	else if (first[i] == 'C')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--logchan", second);
	}
	else if (first[i] == 'D')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--dbase", second);
	}
	else if (first[i] == 'S')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--langdir", second);
	}
	else if (first[i] == 'E')
	{
	    ArgUsed = paramlong("--encrypt", second);
	}
	else if (first[i] == 'e')
	{
	    ArgUsed = paramlong("--decrypt", second);
	}
	else if (first[i] == 'K')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--keyfile", second);
	}
	else if (first[i] == 'c')
	{
	    ArgUsed = paramlong("--compress", second);
	}
	else if (first[i] == 'r')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--relink", second);
	}
	else if (first[i] == 't')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--cycle", second);
	}
	else if (first[i] == 'w')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--save", second);
	}
	else if (first[i] == 'T')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--check", second);
	}
	else if (first[i] == 'p')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--ping", second);
	}
	else if (first[i] == 'q')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--minthreads", second);
	}
	else if (first[i] == 'Q')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--maxthreads", second);
	}
	else if (first[i] == 'm')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--lwm", second);
	}
	else if (first[i] == 'M')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--hwm", second);
	}
	else if (first[i] == 'a')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--append", second);
	}
	else if (first[i] == 'A')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--rename", second);
	}
	else if (first[i] == 'R')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--ident", second);
	}
	else if (first[i] == 's')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--language", second);
	}
	else if (first[i] == 'x')
	{
	    ArgUsed = paramlong("--nodcc", second);
	}
	else if (first[i] == 'f')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--inflight", second);
	}
	else if (first[i] == 'i')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--logignore", second);
	}
	else if (first[i] == 'I')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--ignore", second);
	}
	else
	{
	    LOG(LM_ERROR, "COMMANDLINE/UNKNOWN_OPTION", ("-" + mstring(first[i])));
	}
    }
    RET(ArgUsed);
}

bool Magick::get_config_values()
{
    NFT("Magick::get_config_values");
    bool reconnect = false;
    bool reconnect_clients = false;
    unsigned int i;
    mstring isonstr;

    if (!mFile::Exists(Config_File()))
    {
	RET(false);
    }
    mConfigEngine in(Config_File());

    mstring value_mstring;
    bool value_bool;

//    int value_int;
    unsigned int value_uint;

//    long value_long;
//    unsigned long value_ulong;
//    short value_float;

    // Groups ...
    mstring ts_Startup = mstring("Startup/");
    mstring ts_Services = mstring("Services/");
    mstring ts_Files = mstring("Files/");
    mstring ts_Config = mstring("Config/");
    mstring ts_ChanServ = mstring("ChanServ/");
    mstring ts_NickServ = mstring("NickServ/");
    mstring ts_MemoServ = mstring("MemoServ/");
    mstring ts_OperServ = mstring("OperServ/");
    mstring ts_CommServ = mstring("CommServ/");
    mstring ts_ServMsg = mstring("ServMsg/");

    DumpB();

    in.Read(ts_Startup + "SERVER_NAME", value_mstring, "");
    if (value_mstring != startup.server_name)
	reconnect = true;
    startup.server_name = value_mstring;

    in.Read(ts_Startup + "SERVER_DESC", value_mstring, FULLNAME);
    if (value_mstring != startup.server_desc)
	reconnect = true;
    startup.server_desc = value_mstring;

    in.Read(ts_Startup + "SERVICES_USER", value_mstring, "services");
    in.Read(ts_Startup + "OWNUSER", value_bool, false);
    if (value_bool != startup.ownuser)
	reconnect_clients = true;
    else if (value_mstring != startup.services_user && !startup.ownuser)
	reconnect_clients = true;
    startup.services_user = value_mstring;
    startup.ownuser = value_bool;

    in.Read(ts_Startup + "BIND", value_mstring, "*");
    if (value_mstring != startup.bind)
	reconnect = true;
    startup.bind = value_mstring;

    in.Read(ts_Startup + "SETMODE", value_mstring, "");
    value_mstring.Remove("+");
    value_mstring.Remove(" ");
    if (value_mstring != startup.setmode)
	reconnect_clients = true;
    startup.setmode = value_mstring;

    in.Read(ts_Startup + "SERVICES_HOST", value_mstring, "magick.tm");
    if (value_mstring != startup.services_host)
	reconnect_clients = true;
    startup.services_host = value_mstring;

    // REMOTE entries
    value_mstring.erase();
    i = 1;
    {
	WLOCK(("Startup", "Servers"));
	int min_level = 0;

	startup.servers.clear();
	do
	{
	    mstring lookfor = "REMOTE_";

	    lookfor << i;
	    in.Read(ts_Startup + lookfor, value_mstring, "");
	    if (!value_mstring.empty())
	    {
		mstring tmp[5];

		tmp[0] = value_mstring.ExtractWord(1, ":");
		tmp[1] = value_mstring.ExtractWord(2, ":");
		tmp[2] = value_mstring.ExtractWord(3, ":");
		tmp[3] = value_mstring.ExtractWord(4, ":");
		tmp[4] = value_mstring.ExtractWord(5, ":");
		if (value_mstring.WordCount(":") == 5 && tmp[1].IsNumber() && tmp[3].IsNumber() && tmp[4].IsNumber() &&
		    atoi(tmp[1]) > 0 && atoi(tmp[3]) > 0 && atol(tmp[4]) > 0)
		{
		    // startup.servers[servername] = pair<priority, triplet<port, pass, numeric> >
		    startup.servers[tmp[0].LowerCase()] =
			pair < unsigned int, triplet < unsigned int, mstring, unsigned long > >(atol(tmp[3]),
												triplet < unsigned int,
												mstring,
												unsigned long >(atoi(tmp[1]),
														tmp[2],
														atol(tmp[4])));
		    if (min_level < 1 || atoi(tmp[3]) < min_level)
			min_level = atoi(tmp[3]);
		}
		else
		{
		    LOG(LM_WARNING, "COMMANDLINE/CFG_SYNTAX", (ts_Startup + lookfor));
		    RET(false);
		}
	    }
	    i++;
	} while (!value_mstring.empty());
	if (min_level > 1)
	{
	    // We NEED the lowest priority to be 1
	    map < mstring, pair < unsigned int, triplet < unsigned int, mstring, unsigned long > > >::iterator iter;

	    for (iter = startup.servers.begin(); iter != startup.servers.end(); iter++)
	    {
		iter->second.first -= (min_level - 1);
	    }
	}
    }
    if (CurrentServer().empty() || !startup.IsServer(CurrentServer()))
	reconnect = true;

    // REMOTE entries
    value_mstring.erase();
    i = 1;
    {
	WLOCK(("Startup", "Allows"));
	startup.allows.clear();
	do
	{
	    mstring lookfor = "ALLOW_";

	    lookfor << i;
	    in.Read(ts_Startup + lookfor, value_mstring, "");
	    if (!value_mstring.empty())
	    {
		if (value_mstring.Contains(":"))
		{
		    mstring tmp[2];

		    tmp[0] = value_mstring.ExtractWord(1, ":");
		    tmp[1] = value_mstring.ExtractWord(2, ":");
		    if (tmp[1].length() && tmp[1] != "*")
		    {
			for (unsigned int k = 1; k <= tmp[1].WordCount(", "); k++)
			{
			    startup.allows[tmp[0]].push_back(tmp[1].ExtractWord(k, ", "));
			}
		    }
		    else
		    {
			startup.allows[tmp[0]];
		    }
		}
		else
		{
		    LOG(LM_WARNING, "COMMANDLINE/CFG_SYNTAX", (ts_Startup + lookfor));
		    RET(false);
		}
	    }
	    i++;
	} while (!value_mstring.empty());
    }
    if (CurrentServer().empty() || !startup.IsAllowed(CurrentServer(), startup.Server_Name()))
	reconnect = true;

    in.Read(ts_Startup + "PROTOCOL", value_uint, 0U);
    if (value_uint != server.proto.Number())
    {
	server.proto.Set(value_uint);
	if (value_uint == server.proto.Number())
	    reconnect = true;
	else
	{
	    LOG(LM_WARNING, "COMMANDLINE/UNKNOWN_PROTO", (value_uint, server.proto.Number()));
	    RET(false);
	}
    }

    in.Read(ts_Startup + "LEVEL", value_uint, 1U);
    if (value_uint > i_level)
	i_level = value_uint;
    startup.level = value_uint;

    in.Read(ts_Startup + "LAGTIME", value_mstring, "10s");
    if (FromHumanTime(value_mstring))
	startup.lagtime = FromHumanTime(value_mstring);
    else
	startup.lagtime = FromHumanTime("10s");

    in.Read(ts_Startup + "STOP", value_bool, true);
    i_shutdown = value_bool;

    in.Read(ts_Services + "NickServ", value_mstring, "NickServ");
    for (i = 0; i < nickserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains((" " + nickserv.names.ExtractWord(i + 1, " ") + " ")))
	{
	    nickserv.signoff(nickserv.names.ExtractWord(i + 1, " "));
	}
    }
    nickserv.names = value_mstring;

    in.Read(ts_Services + "NickServ_Name", value_mstring, "Nickname Service");
    if (value_mstring != nickserv.realname)
    {
	nickserv.realname = value_mstring;
	for (i = 0; i < nickserv.names.WordCount(" "); i++)
	{
	    nickserv.signoff(nickserv.names.ExtractWord(i + 1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i = 0; i < nickserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(nickserv.names.ExtractWord(i + 1, " ")))
	    {
		if (isonstr.length() > server.proto.MaxLine())
		{
		    server.
			sraw(((server.proto.Tokens() &&
			       !server.proto.GetNonToken("ISON").empty())? server.proto.
			      GetNonToken("ISON") : mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(nickserv.names.ExtractWord(i + 1, " "));
		isonstr += nickserv.names.ExtractWord(i + 1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services + "Enforcer_Name", nickserv.enforcer_name, "Nickname Enforcer");

    in.Read(ts_Services + "ChanServ", value_mstring, "ChanServ");
    for (i = 0; i < chanserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains((" " + chanserv.names.ExtractWord(i + 1, " ") + " ")))
	{
	    chanserv.signoff(chanserv.names.ExtractWord(i + 1, " "));
	}
    }
    chanserv.names = value_mstring;

    in.Read(ts_Services + "ChanServ_Name", value_mstring, "Channel Service");
    if (value_mstring != chanserv.realname)
    {
	chanserv.realname = value_mstring;
	for (i = 0; i < chanserv.names.WordCount(" "); i++)
	{
	    chanserv.signoff(chanserv.names.ExtractWord(i + 1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i = 0; i < chanserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(chanserv.names.ExtractWord(i + 1, " ")))
	    {
		if (isonstr.length() > server.proto.MaxLine())
		{
		    server.
			sraw(((server.proto.Tokens() &&
			       !server.proto.GetNonToken("ISON").empty())? server.proto.
			      GetNonToken("ISON") : mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(chanserv.names.ExtractWord(i + 1, " "));
		isonstr += chanserv.names.ExtractWord(i + 1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services + "MemoServ", value_mstring, "MemoServ");
    for (i = 0; i < memoserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains((" " + memoserv.names.ExtractWord(i + 1, " ") + " ")))
	{
	    memoserv.signoff(memoserv.names.ExtractWord(i + 1, " "));
	}
    }
    memoserv.names = value_mstring;

    in.Read(ts_Services + "MemoServ_Name", value_mstring, "Memo/News Service");
    if (value_mstring != memoserv.realname)
    {
	memoserv.realname = value_mstring;
	for (i = 0; i < memoserv.names.WordCount(" "); i++)
	{
	    memoserv.signoff(memoserv.names.ExtractWord(i + 1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i = 0; i < memoserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(memoserv.names.ExtractWord(i + 1, " ")))
	    {
		if (isonstr.length() > server.proto.MaxLine())
		{
		    server.
			sraw(((server.proto.Tokens() &&
			       !server.proto.GetNonToken("ISON").empty())? server.proto.
			      GetNonToken("ISON") : mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(memoserv.names.ExtractWord(i + 1, " "));
		isonstr += memoserv.names.ExtractWord(i + 1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services + "OperServ", value_mstring, "OperServ");
    for (i = 0; i < operserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains((" " + operserv.names.ExtractWord(i + 1, " ") + " ")))
	{
	    operserv.signoff(operserv.names.ExtractWord(i + 1, " "));
	}
    }
    operserv.names = value_mstring;

    in.Read(ts_Services + "OperServ_Name", value_mstring, "Operator Service");
    if (value_mstring != operserv.realname)
    {
	operserv.realname = value_mstring;
	for (i = 0; i < operserv.names.WordCount(" "); i++)
	{
	    operserv.signoff(operserv.names.ExtractWord(i + 1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i = 0; i < operserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(operserv.names.ExtractWord(i + 1, " ")))
	    {
		if (isonstr.length() > server.proto.MaxLine())
		{
		    server.
			sraw(((server.proto.Tokens() &&
			       !server.proto.GetNonToken("ISON").empty())? server.proto.
			      GetNonToken("ISON") : mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(operserv.names.ExtractWord(i + 1, " "));
		isonstr += operserv.names.ExtractWord(i + 1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services + "CommServ", value_mstring, "CommServ");
    for (i = 0; i < commserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains((" " + commserv.names.ExtractWord(i + 1, " ") + " ")))
	{
	    commserv.signoff(commserv.names.ExtractWord(i + 1, " "));
	}
    }
    commserv.names = value_mstring;

    in.Read(ts_Services + "CommServ_Name", value_mstring, "Committee Service");
    if (value_mstring != commserv.realname)
    {
	commserv.realname = value_mstring;
	for (i = 0; i < commserv.names.WordCount(" "); i++)
	{
	    commserv.signoff(commserv.names.ExtractWord(i + 1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i = 0; i < commserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(commserv.names.ExtractWord(i + 1, " ")))
	    {
		if (isonstr.length() > server.proto.MaxLine())
		{
		    server.
			sraw(((server.proto.Tokens() &&
			       !server.proto.GetNonToken("ISON").empty())? server.proto.
			      GetNonToken("ISON") : mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(commserv.names.ExtractWord(i + 1, " "));
		isonstr += commserv.names.ExtractWord(i + 1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services + "ServMsg", value_mstring, "HelpServ DevNull");
    for (i = 0; i < servmsg.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains((" " + servmsg.names.ExtractWord(i + 1, " ") + " ")))
	{
	    servmsg.signoff(servmsg.names.ExtractWord(i + 1, " "));
	}
    }
    servmsg.names = value_mstring;

    in.Read(ts_Services + "ServMsg_Name", value_mstring, mstring(PACKAGE) + " <--> User");
    if (value_mstring != servmsg.realname)
    {
	servmsg.realname = value_mstring;
	for (i = 0; i < servmsg.names.WordCount(" "); i++)
	{
	    servmsg.signoff(servmsg.names.ExtractWord(i + 1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i = 0; i < servmsg.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(servmsg.names.ExtractWord(i + 1, " ")))
	    {
		if (isonstr.length() > server.proto.MaxLine())
		{
		    server.
			sraw(((server.proto.Tokens() &&
			       !server.proto.GetNonToken("ISON").empty())? server.proto.
			      GetNonToken("ISON") : mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(servmsg.names.ExtractWord(i + 1, " "));
		isonstr += servmsg.names.ExtractWord(i + 1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services + "SHOWSYNC", servmsg.showsync, true);
    in.Read(ts_Services + "QUIT_MESSAGE", startup.services_quitmsg, "");

    if (!isonstr.empty())
	server.
	    sraw(((server.proto.Tokens() &&
		   !server.proto.GetNonToken("ISON").empty())? server.proto.GetNonToken("ISON") : mstring("ISON")) + " :" +
		 isonstr);

    in.Read(ts_Files + "UMASK", value_mstring, "027");
    files.umask = 0;
    for (i = 0; i < value_mstring.length(); i++)
    {
	if (!isdigit(value_mstring[i]))
	    break;
	else
	{
	    files.umask <<= 3;
	    files.umask += value_mstring[i] - '0';
	}
    }
    if (i != 3)
	files.umask = 027;

    in.Read(ts_Files + "PIDFILE", files.pidfile, "magick.pid");
    in.Read(ts_Files + "LOGFILE", files.logfile, "magick.log");
    in.Read(ts_Files + "LOGCHAN", files.logchan, "");
    in.Read(ts_Files + "VERBOSE", i_verbose, false);
    in.Read(ts_Files + "MOTDFILE", files.motdfile, "magick.motd");
    in.Read(ts_Files + "LANGDIR", files.langdir, "lang");
    in.Read(ts_Files + "DATABASE", files.database, "magick.mnd");
    in.Read(ts_Files + "COMPRESSION", files.compression, 6U);
    if (files.compression > 9)
	files.compression = 9;
    in.Read(ts_Files + "KEYFILE", files.keyfile, "magick.key");
    in.Read(ts_Files + "ENCRYPTION", files.encryption, false);
    in.Read(ts_Files + "MEMOATTACH", files.memoattach, "files/memo");
    MakeDirectory(files.memoattach.c_str());

    in.Read(ts_Files + "MEMOATTACHSIZE", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	files.memoattachsize = FromHumanSpace(value_mstring);
    else
	files.memoattachsize = FromHumanSpace("0");
    in.Read(ts_Files + "PICTURE", files.picture, "files/pic");
    MakeDirectory(files.picture.c_str());

    in.Read(ts_Files + "PICTURESIZE", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	files.picturesize = FromHumanSpace(value_mstring);
    else
	files.picturesize = FromHumanSpace("0");
    in.Read(ts_Files + "PUBLIC", files.i_public, "files/public");
    MakeDirectory(files.i_public.c_str());

    in.Read(ts_Files + "PUBLICSIZE", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	files.publicsize = FromHumanSpace(value_mstring);
    else
	files.publicsize = FromHumanSpace("0");
    in.Read(ts_Files + "TEMPDIR", files.tempdir, "files/temp");
    MakeDirectory(files.tempdir.c_str());

    in.Read(ts_Files + "TEMPDIRSIZE", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	files.tempdirsize = FromHumanSpace(value_mstring);
    else
	files.tempdirsize = FromHumanSpace("0");
    in.Read(ts_Files + "BLOCKSIZE", value_mstring, "1k");
    if (FromHumanSpace(value_mstring))
	files.blocksize = FromHumanSpace(value_mstring);
    else
	files.blocksize = FromHumanSpace("1k");
    in.Read(ts_Files + "TIMEOUT", value_mstring, "2m");
    if (FromHumanTime(value_mstring))
	files.timeout = FromHumanTime(value_mstring);
    else
	files.timeout = FromHumanTime("2m");
    in.Read(ts_Files + "MIN_SPEED", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	files.min_speed = FromHumanSpace(value_mstring);
    else
	files.min_speed = FromHumanSpace("0");
    in.Read(ts_Files + "MAX_SPEED", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	files.max_speed = FromHumanSpace(value_mstring);
    else
	files.max_speed = FromHumanSpace("0");
    in.Read(ts_Files + "SAMPLETIME", value_mstring, "30s");
    if (FromHumanTime(value_mstring))
	files.sampletime = FromHumanTime(value_mstring);
    else
	files.sampletime = FromHumanTime("30s");

    in.Read(ts_Config + "SERVER_RELINK", value_mstring, "5s");
    if (FromHumanTime(value_mstring))
	config.server_relink = FromHumanTime(value_mstring);
    else
	config.server_relink = FromHumanTime("5s");

    in.Read(ts_Config + "SQUIT_PROTECT", value_mstring, "2m");
    if (FromHumanTime(value_mstring))
	config.squit_protect = FromHumanTime(value_mstring);
    else
	config.squit_protect = FromHumanTime("2m");

    in.Read(ts_Config + "SQUIT_CANCEL", value_mstring, "10s");
    if (FromHumanTime(value_mstring))
	config.squit_cancel = FromHumanTime(value_mstring);
    else
	config.squit_cancel = FromHumanTime("10s");

    in.Read(ts_Config + "CYCLETIME", value_mstring, "5m");
    if (FromHumanTime(value_mstring))
	config.cycletime = FromHumanTime(value_mstring);
    else
	config.cycletime = FromHumanTime("5m");

    in.Read(ts_Config + "SAVETIME", value_mstring, "5m");
    if (FromHumanTime(value_mstring))
	config.savetime = FromHumanTime(value_mstring);
    else
	config.savetime = FromHumanTime("5m");

    in.Read(ts_Config + "CHECKTIME", value_mstring, "5s");
    if (FromHumanTime(value_mstring))
	config.checktime = FromHumanTime(value_mstring);
    else
	config.checktime = FromHumanTime("5s");

    in.Read(ts_Config + "PING_FREQUENCY", value_mstring, "30s");
    if (FromHumanTime(value_mstring))
	config.ping_frequency = FromHumanTime(value_mstring);
    else
	config.ping_frequency = FromHumanTime("30s");

    in.Read(ts_Config + "STARTHRESH", config.starthresh, 4U);
    in.Read(ts_Config + "LISTSIZE", config.listsize, 50U);
    in.Read(ts_Config + "MAXLIST", value_uint, 250U);
    if (value_uint < config.listsize)
	value_uint = config.listsize;
    config.maxlist = value_uint;

    in.Read(ts_Config + "MIN_THREADS", config.min_threads, 3U);
    if (config.min_threads < 1)
	config.min_threads = 1;
    in.Read(ts_Config + "MAX_THREADS", config.max_threads, 25U);
    if (config.max_threads < config.min_threads)
	config.max_threads = config.min_threads;
    in.Read(ts_Config + "LOW_WATER_MARK", config.low_water_mark, 10U);
    in.Read(ts_Config + "HIGH_WATER_MARK", config.high_water_mark, 15U);
    if (config.high_water_mark < config.low_water_mark)
	config.high_water_mark = config.low_water_mark;
    in.Read(ts_Config + "HEARTBEAT_TIME", value_mstring, "15m");
    if (FromHumanTime(value_mstring))
	config.heartbeat_time = FromHumanTime(value_mstring);
    else
	config.heartbeat_time = FromHumanTime("15m");
    in.Read(ts_Config + "MSG_SEEN_TIME", value_mstring, "30s");
    if (FromHumanTime(value_mstring))
	config.msg_seen_time = FromHumanTime(value_mstring);
    else
	config.msg_seen_time = FromHumanTime("30s");
    in.Read(ts_Config + "MSG_CHECK_TIME", value_mstring, "1m");
    if (FromHumanTime(value_mstring))
	config.msg_check_time = FromHumanTime(value_mstring);
    else
	config.msg_check_time = FromHumanTime("1m");

    in.Read(ts_NickServ + "APPEND_RENAME", nickserv.append_rename, true);
    in.Read(ts_NickServ + "SUFFIXES", nickserv.suffixes, "_-^`");
    in.Read(ts_NickServ + "EXPIRE", value_mstring, "4w");
    if (FromHumanTime(value_mstring))
	nickserv.expire = FromHumanTime(value_mstring);
    else
	nickserv.expire = FromHumanTime("4w");

    in.Read(ts_NickServ + "DELAY", value_mstring, "30s");
    if (FromHumanTime(value_mstring))
	nickserv.delay = FromHumanTime(value_mstring);
    else
	nickserv.delay = FromHumanTime("30s");

    in.Read(ts_NickServ + "IDENT", value_mstring, "1m");
    if (FromHumanTime(value_mstring))
	nickserv.ident = FromHumanTime(value_mstring);
    else
	nickserv.ident = FromHumanTime("1m");

    in.Read(ts_NickServ + "RELEASE", value_mstring, "1m");
    if (FromHumanTime(value_mstring))
	nickserv.release = FromHumanTime(value_mstring);
    else
	nickserv.release = FromHumanTime("1m");

    in.Read(ts_NickServ + "PASSFAIL", nickserv.passfail, 5U);
    in.Read(ts_NickServ + "DEF_PROTECT", value_bool, true);
    nickserv.def.Protect = value_bool;
    in.Read(ts_NickServ + "LCK_PROTECT", value_bool, false);
    nickserv.lock.Protect = value_bool;
    in.Read(ts_NickServ + "DEF_SECURE", value_bool, false);
    nickserv.def.Secure = value_bool;
    in.Read(ts_NickServ + "LCK_SECURE", value_bool, false);
    nickserv.lock.Secure = value_bool;
    in.Read(ts_NickServ + "DEF_NOEXPIRE", value_bool, false);
    nickserv.def.NoExpire = value_bool;
    in.Read(ts_NickServ + "LCK_NOEXPIRE", value_bool, false);
    nickserv.lock.NoExpire = value_bool;
    in.Read(ts_NickServ + "DEF_NOMEMO", value_bool, false);
    nickserv.def.NoMemo = value_bool;
    in.Read(ts_NickServ + "LCK_NOMEMO", value_bool, false);
    nickserv.lock.NoMemo = value_bool;
    in.Read(ts_NickServ + "DEF_PRIVATE", value_bool, false);
    nickserv.def.Private = value_bool;
    in.Read(ts_NickServ + "LCK_PRIVATE", value_bool, false);
    nickserv.lock.Private = value_bool;
    in.Read(ts_NickServ + "DEF_PRIVMSG", value_bool, false);
    nickserv.def.PRIVMSG = value_bool;
    in.Read(ts_NickServ + "LCK_PRIVMSG", value_bool, false);
    nickserv.lock.PRIVMSG = value_bool;
    in.Read(ts_NickServ + "DEF_LANGUAGE", value_mstring, "ENGLISH");
    if (value_mstring != nickserv.def.Language)
    {
	nickserv.def.Language = value_mstring.UpperCase();
	WLOCK(("LogMessages"));
	LogMessages.clear();
	LoadLogMessages(nickserv.def.Language);
    }
    in.Read(ts_NickServ + "LCK_LANGUAGE", value_bool, false);
    nickserv.lock.Language = value_bool;
    in.Read(ts_NickServ + "PICSIZE", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	nickserv.picsize = FromHumanSpace(value_mstring);
    else
	nickserv.picsize = FromHumanSpace("0");
    in.Read(ts_NickServ + "PICEXT", nickserv.picext, "");

    in.Read(ts_ChanServ + "HIDE", value_bool, false);
    if (!reconnect && Connected() && value_bool != chanserv.hide)
    {
	chanserv.hide = value_bool;
	if (nickserv.IsLive(chanserv.FirstName()))
	{
	    if (chanserv.hide)
		server.MODE(chanserv.FirstName(), "+i");
	    else
		server.MODE(chanserv.FirstName(), "-i");
	}
    }

    in.Read(ts_ChanServ + "EXPIRE", value_mstring, "2w");
    if (FromHumanTime(value_mstring))
	chanserv.expire = FromHumanTime(value_mstring);
    else
	chanserv.expire = FromHumanTime("2w");

    in.Read(ts_ChanServ + "DELAY", value_mstring, "30s");
    if (FromHumanTime(value_mstring))
	chanserv.delay = FromHumanTime(value_mstring);
    else
	chanserv.delay = FromHumanTime("30s");

    in.Read(ts_ChanServ + "MAX_PER_NICK", chanserv.max_per_nick, 15U);
    in.Read(ts_ChanServ + "MAX_MESSAGES", chanserv.max_messages, 15U);
    in.Read(ts_ChanServ + "DEF_AKICK", chanserv.def_akick_reason, "You have been banned from channel");
    in.Read(ts_ChanServ + "PASSFAIL", chanserv.passfail, 5U);
    in.Read(ts_ChanServ + "CHANKEEP", value_mstring, "15s");
    if (FromHumanTime(value_mstring))
	chanserv.chankeep = FromHumanTime(value_mstring);
    else
	chanserv.chankeep = FromHumanTime("15s");

    in.Read(ts_ChanServ + "DEF_MLOCK", chanserv.def.Mlock, "");
    in.Read(ts_ChanServ + "LCK_MLOCK", chanserv.lock.Mlock, "");
    in.Read(ts_ChanServ + "DEF_BANTIME", value_mstring, "0");
    if (FromHumanTime(value_mstring))
	chanserv.def.Bantime = FromHumanTime(value_mstring);
    else
	chanserv.def.Bantime = FromHumanTime("0");

    in.Read(ts_ChanServ + "LCK_BANTIME", value_bool, false);
    chanserv.lock.Bantime = value_bool;
    in.Read(ts_ChanServ + "DEF_PARTTIME", value_mstring, "0");
    if (FromHumanTime(value_mstring))
	chanserv.def.Parttime = FromHumanTime(value_mstring);
    else
	chanserv.def.Parttime = FromHumanTime("0");

    in.Read(ts_ChanServ + "LCK_PARTTIME", value_bool, false);
    chanserv.lock.Parttime = value_bool;
    in.Read(ts_ChanServ + "DEF_KEEPTOPIC", value_bool, true);
    chanserv.def.Keeptopic = value_bool;
    in.Read(ts_ChanServ + "LCK_KEEPTOPIC", value_bool, false);
    chanserv.lock.Keeptopic = value_bool;
    in.Read(ts_ChanServ + "DEF_TOPICLOCK", value_bool, false);
    chanserv.def.Topiclock = value_bool;
    in.Read(ts_ChanServ + "LCK_TOPICLOCK", value_bool, false);
    chanserv.lock.Topiclock = value_bool;
    in.Read(ts_ChanServ + "DEF_PRIVATE", value_bool, false);
    chanserv.def.Private = value_bool;
    in.Read(ts_ChanServ + "LCK_PRIVATE", value_bool, false);
    chanserv.lock.Private = value_bool;
    in.Read(ts_ChanServ + "DEF_SECUREOPS", value_bool, false);
    chanserv.def.Secureops = value_bool;
    in.Read(ts_ChanServ + "LCK_SECUREOPS", value_bool, false);
    chanserv.lock.Secureops = value_bool;
    in.Read(ts_ChanServ + "DEF_SECURE", value_bool, false);
    chanserv.def.Secure = value_bool;
    in.Read(ts_ChanServ + "LCK_SECURE", value_bool, false);
    chanserv.lock.Secure = value_bool;
    in.Read(ts_ChanServ + "DEF_NOEXPIRE", value_bool, false);
    chanserv.def.NoExpire = value_bool;
    in.Read(ts_ChanServ + "LCK_NOEXPIRE", value_bool, false);
    chanserv.lock.NoExpire = value_bool;
    in.Read(ts_ChanServ + "DEF_ANARCHY", value_bool, false);
    chanserv.def.Anarchy = value_bool;
    in.Read(ts_ChanServ + "LCK_ANARCHY", value_bool, false);
    chanserv.lock.Anarchy = value_bool;
    in.Read(ts_ChanServ + "DEF_KICKONBAN", value_bool, false);
    chanserv.def.KickOnBan = value_bool;
    in.Read(ts_ChanServ + "LCK_KICKONBAN", value_bool, false);
    chanserv.lock.KickOnBan = value_bool;
    in.Read(ts_ChanServ + "DEF_RESTRICTED", value_bool, false);
    chanserv.def.Restricted = value_bool;
    in.Read(ts_ChanServ + "LCK_RESTRICTED", value_bool, false);
    chanserv.lock.Restricted = value_bool;
    in.Read(ts_ChanServ + "DEF_JOIN", value_bool, false);
    chanserv.def.Join = value_bool;
    in.Read(ts_ChanServ + "LCK_JOIN", value_bool, false);
    chanserv.lock.Join = value_bool;
    in.Read(ts_ChanServ + "DEF_REVENGE", value_mstring, "NONE");
    if (chanserv.IsRevengeLevel(value_mstring))
	chanserv.def.Revenge = value_mstring.UpperCase();
    else
	chanserv.def.Revenge = "NONE";

    in.Read(ts_ChanServ + "LCK_REVENGE", value_bool, false);
    chanserv.lock.Revenge = value_bool;
    in.Read(ts_ChanServ + "LEVEL_MIN", chanserv.level_min, -1L);
    in.Read(ts_ChanServ + "LEVEL_MAX", chanserv.level_max, 30L);
    in.Read(ts_ChanServ + "LVL_AUTODEOP", chanserv.lvl["AUTODEOP"], -1L);
    in.Read(ts_ChanServ + "LVL_AUTOVOICE", chanserv.lvl["AUTOVOICE"], 5L);
    in.Read(ts_ChanServ + "LVL_AUTOHALFOP", chanserv.lvl["AUTOHALFOP"], 10L);
    in.Read(ts_ChanServ + "LVL_AUTOOP", chanserv.lvl["AUTOOP"], 10L);
    in.Read(ts_ChanServ + "LVL_READMEMO", chanserv.lvl["READMEMO"], 0L);
    in.Read(ts_ChanServ + "LVL_WRITEMEMO", chanserv.lvl["WRITEMEMO"], 15L);
    in.Read(ts_ChanServ + "LVL_DELMEMO", chanserv.lvl["DELMEMO"], 25L);
    in.Read(ts_ChanServ + "LVL_GREET", chanserv.lvl["GREET"], 1L);
    in.Read(ts_ChanServ + "LVL_OVERGREET", chanserv.lvl["OVERGREET"], 25L);
    in.Read(ts_ChanServ + "LVL_MESSAGE", chanserv.lvl["MESSAGE"], 20L);
    in.Read(ts_ChanServ + "LVL_AKICK", chanserv.lvl["AKICK"], 20L);
    in.Read(ts_ChanServ + "LVL_SUPER", chanserv.lvl["SUPER"], 25L);
    in.Read(ts_ChanServ + "LVL_UNBAN", chanserv.lvl["UNBAN"], 10L);
    in.Read(ts_ChanServ + "LVL_ACCESS", chanserv.lvl["ACCESS"], 5L);
    in.Read(ts_ChanServ + "LVL_SET", chanserv.lvl["SET"], 25L);
    in.Read(ts_ChanServ + "LVL_VIEW", chanserv.lvl["VIEW"], 1L);
    in.Read(ts_ChanServ + "LVL_CMDINVITE", chanserv.lvl["CMDINVITE"], 5L);
    in.Read(ts_ChanServ + "LVL_CMDUNBAN", chanserv.lvl["CMDUNBAN"], 10L);
    in.Read(ts_ChanServ + "LVL_CMDVOICE", chanserv.lvl["CMDVOICE"], 5L);
    in.Read(ts_ChanServ + "LVL_CMDHALFOP", chanserv.lvl["CMDHALFOP"], 10L);
    in.Read(ts_ChanServ + "LVL_CMDOP", chanserv.lvl["CMDOP"], 10L);
    in.Read(ts_ChanServ + "LVL_CMDKICK", chanserv.lvl["CMDKICK"], 15L);
    in.Read(ts_ChanServ + "LVL_CMDMODE", chanserv.lvl["CMDMODE"], 15L);
    in.Read(ts_ChanServ + "LVL_CMDCLEAR", chanserv.lvl["CMDCLEAR"], 20L);

    in.Read(ts_MemoServ + "NEWS_EXPIRE", value_mstring, "3w");
    if (FromHumanTime(value_mstring))
	memoserv.news_expire = FromHumanTime(value_mstring);
    else
	memoserv.news_expire = FromHumanTime("3w");

    in.Read(ts_MemoServ + "INFLIGHT", value_mstring, "3m");
    if (FromHumanTime(value_mstring))
	memoserv.inflight = FromHumanTime(value_mstring);
    else
	memoserv.inflight = FromHumanTime("3m");

    in.Read(ts_MemoServ + "DELAY", value_mstring, "10s");
    if (FromHumanTime(value_mstring))
	memoserv.delay = FromHumanTime(value_mstring);
    else
	memoserv.delay = FromHumanTime("10s");

    in.Read(ts_MemoServ + "FILES", memoserv.files, 0U);
    in.Read(ts_MemoServ + "FILESIZE", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	memoserv.filesize = FromHumanSpace(value_mstring);
    else
	memoserv.filesize = FromHumanSpace("0");

    in.Read(ts_OperServ + "SERVICES_ADMIN", operserv.services_admin, "");
    in.Read(ts_OperServ + "SECURE", operserv.secure, false);
    in.Read(ts_OperServ + "SECUREOPER", operserv.secureoper, false);
    in.Read(ts_OperServ + "DEF_EXPIRE", value_mstring, "3h");
    if (FromHumanTime(value_mstring))
	operserv.def_expire = FromHumanTime(value_mstring);
    else
	operserv.def_expire = FromHumanTime("3h");

    in.Read(ts_OperServ + "EXPIRE_OPER", value_mstring, "1d");
    if (FromHumanTime(value_mstring))
	operserv.expire_oper = FromHumanTime(value_mstring);
    else
	operserv.expire_oper = FromHumanTime("1d");

    in.Read(ts_OperServ + "EXPIRE_ADMIN", value_mstring, "1w");
    if (FromHumanTime(value_mstring))
	operserv.expire_admin = FromHumanTime(value_mstring);
    else
	operserv.expire_admin = FromHumanTime("1w");

    in.Read(ts_OperServ + "EXPIRE_SOP", value_mstring, "8w");
    if (FromHumanTime(value_mstring))
	operserv.expire_sop = FromHumanTime(value_mstring);
    else
	operserv.expire_sop = FromHumanTime("8w");

    in.Read(ts_OperServ + "EXPIRE_SADMIN", value_mstring, "1y");
    if (FromHumanTime(value_mstring))
	operserv.expire_sadmin = FromHumanTime(value_mstring);
    else
	operserv.expire_sadmin = FromHumanTime("1y");

    in.Read(ts_OperServ + "AKILL_REJECT", operserv.akill_reject, 10.0f);
    if (operserv.akill_reject < 0.0)
	operserv.akill_reject = 0.0;
    if (operserv.akill_reject > 100.0)
	operserv.akill_reject = 100.0;

    in.Read(ts_OperServ + "MAX_CLONE", operserv.max_clone, 50U);
    in.Read(ts_OperServ + "CLONE_LIMIT", operserv.clone_limit, 2U);
    in.Read(ts_OperServ + "DEF_CLONE", operserv.def_clone, "Maximum connections from one host exceeded");
    in.Read(ts_OperServ + "CLONE_TRIGGER", operserv.clone_trigger, 10U);
    in.Read(ts_OperServ + "CLONE_TIME", value_mstring, "3h");
    if (FromHumanTime(value_mstring))
	operserv.clone_time = FromHumanTime(value_mstring);
    else
	operserv.clone_time = FromHumanTime("3h");
    in.Read(ts_OperServ + "CLONE_AKILL", operserv.clone_akill, "Clone trigger exceeded, Automatic AKILL");
    in.Read(ts_OperServ + "CLONE_AKILLTIME", value_mstring, "30m");
    if (FromHumanTime(value_mstring))
	operserv.clone_akilltime = FromHumanTime(value_mstring);
    else
	operserv.clone_akilltime = FromHumanTime("30m");

    in.Read(ts_OperServ + "FLOOD_TIME", value_mstring, "10s");
    if (FromHumanTime(value_mstring))
	operserv.flood_time = FromHumanTime(value_mstring);
    else
	operserv.flood_time = FromHumanTime("10s");

    in.Read(ts_OperServ + "FLOOD_MSGS", operserv.flood_msgs, 5U);

    in.Read(ts_OperServ + "IGNORE_TIME", value_mstring, "20s");
    if (FromHumanTime(value_mstring))
	operserv.ignore_time = FromHumanTime(value_mstring);
    else
	operserv.ignore_time = FromHumanTime("20s");

    in.Read(ts_OperServ + "IGNORE_LIMIT", operserv.ignore_limit, 5U);
    in.Read(ts_OperServ + "IGNORE_REMOVE", value_mstring, "5m");
    if (FromHumanTime(value_mstring))
	operserv.ignore_remove = FromHumanTime(value_mstring);
    else
	operserv.ignore_remove = FromHumanTime("5m");

    in.Read(ts_OperServ + "IGNORE_METHOD", operserv.ignore_method, 8U);
    in.Read(ts_OperServ + "LOG_IGNORE", operserv.log_ignore, false);
    in.Read(ts_OperServ + "INIT_HTM_GAP", value_mstring, "5s");
    if (FromHumanTime(value_mstring))
	operserv.init_htm_gap = FromHumanTime(value_mstring);
    else
	operserv.init_htm_gap = FromHumanTime("5s");
    in.Read(ts_OperServ + "INIT_HTM_THRESH", value_mstring, "16k");
    if (FromHumanSpace(value_mstring))
	operserv.init_htm_thresh = FromHumanSpace(value_mstring);
    else
	operserv.init_htm_thresh = FromHumanSpace("16k");
    in.Read(ts_OperServ + "MAX_HTM_GAP", value_mstring, "1m");
    if (FromHumanTime(value_mstring))
	operserv.max_htm_gap = FromHumanTime(value_mstring);
    else
	operserv.max_htm_gap = FromHumanTime("1m");
    in.Read(ts_OperServ + "HTM_ON_GAP", value_mstring, "30s");
    if (FromHumanTime(value_mstring))
	operserv.htm_on_gap = FromHumanTime(value_mstring);
    else
	operserv.htm_on_gap = FromHumanTime("30s");

    in.Read(ts_CommServ + "MAX_LOGON", commserv.max_logon, 5U);
    RemCommands();
    in.Read(ts_CommServ + "DEF_OPENMEMOS", value_bool, true);
    commserv.def.OpenMemos = value_bool;
    in.Read(ts_CommServ + "LCK_OPENMEMOS", value_bool, false);
    commserv.lock.OpenMemos = value_bool;
    in.Read(ts_CommServ + "DEF_SECURE", value_bool, false);
    commserv.def.Secure = value_bool;
    in.Read(ts_CommServ + "LCK_SECURE", value_bool, false);
    commserv.lock.Secure = value_bool;
    in.Read(ts_CommServ + "DEF_PRIVATE", value_bool, false);
    commserv.def.Private = value_bool;
    in.Read(ts_CommServ + "LCK_PRIVATE", value_bool, false);
    commserv.lock.Private = value_bool;
    in.Read(ts_CommServ + "ALL_NAME", commserv.all.Name, "ALL");
    in.Read(ts_CommServ + "ALL_SETMODE", commserv.all.SetMode, "");
    in.Read(ts_CommServ + "REGD_NAME", commserv.regd.Name, "REGD");
    in.Read(ts_CommServ + "REGD_SETMODE", commserv.regd.SetMode, "");
    in.Read(ts_CommServ + "SADMIN_NAME", commserv.sadmin.Name, "SADMIN");
    in.Read(ts_CommServ + "SADMIN_SECURE", value_bool, true);
    commserv.sadmin.Secure = value_bool;
    in.Read(ts_CommServ + "SADMIN_PRIVATE", value_bool, false);
    commserv.sadmin.Private = value_bool;
    in.Read(ts_CommServ + "SADMIN_OPENMEMOS", value_bool, true);
    commserv.sadmin.OpenMemos = value_bool;
    in.Read(ts_CommServ + "SADMIN_MODEO", value_bool, true);
    commserv.sadmin.ModeO = value_bool;
    in.Read(ts_CommServ + "SADMIN_SETMODE", commserv.sadmin.SetMode, "");
    in.Read(ts_CommServ + "SOP_NAME", commserv.sop.Name, "SOP");
    in.Read(ts_CommServ + "SOP_SECURE", value_bool, true);
    commserv.sop.Secure = value_bool;
    in.Read(ts_CommServ + "SOP_PRIVATE", value_bool, false);
    commserv.sop.Private = value_bool;
    in.Read(ts_CommServ + "SOP_OPENMEMOS", value_bool, true);
    commserv.sop.OpenMemos = value_bool;
    in.Read(ts_CommServ + "SOP_MODEO", value_bool, true);
    commserv.sop.ModeO = value_bool;
    in.Read(ts_CommServ + "SOP_SETMODE", commserv.sop.SetMode, "");
    in.Read(ts_CommServ + "ADMIN_NAME", commserv.admin.Name, "ADMIN");
    in.Read(ts_CommServ + "ADMIN_SECURE", value_bool, true);
    commserv.admin.Secure = value_bool;
    in.Read(ts_CommServ + "ADMIN_PRIVATE", value_bool, false);
    commserv.admin.Private = value_bool;
    in.Read(ts_CommServ + "ADMIN_OPENMEMOS", value_bool, true);
    commserv.admin.OpenMemos = value_bool;
    in.Read(ts_CommServ + "ADMIN_MODEO", value_bool, true);
    commserv.admin.ModeO = value_bool;
    in.Read(ts_CommServ + "ADMIN_SETMODE", commserv.admin.SetMode, "");
    in.Read(ts_CommServ + "OPER_NAME", commserv.oper.Name, "OPER");
    in.Read(ts_CommServ + "OPER_SECURE", value_bool, true);
    commserv.oper.Secure = value_bool;
    in.Read(ts_CommServ + "OPER_PRIVATE", value_bool, false);
    commserv.oper.Private = value_bool;
    in.Read(ts_CommServ + "OPER_OPENMEMOS", value_bool, true);
    commserv.oper.OpenMemos = value_bool;
    in.Read(ts_CommServ + "OPER_MODEO", value_bool, true);
    commserv.oper.ModeO = value_bool;
    in.Read(ts_CommServ + "OPER_SETMODE", commserv.oper.SetMode, "");
    in.Read(ts_CommServ + "OVR_VIEW", commserv.ovr_view, "OPER");
    in.Read(ts_CommServ + "OVR_OWNER", commserv.ovr_owner, "SADMIN");
    in.Read(ts_CommServ + "OVR_CS_MODE", commserv.ovr_cs_mode, "SOP");
    in.Read(ts_CommServ + "OVR_CS_OP", commserv.ovr_cs_op, "SOP");
    in.Read(ts_CommServ + "OVR_CS_VOICE", commserv.ovr_cs_voice, "SOP");
    in.Read(ts_CommServ + "OVR_CS_INVITE", commserv.ovr_cs_invite, "SOP");
    in.Read(ts_CommServ + "OVR_CS_UNBAN", commserv.ovr_cs_unban, "SOP");
    in.Read(ts_CommServ + "OVR_CS_CLEAR", commserv.ovr_cs_clear, "SADMIN");
    commserv.all.Name.MakeUpper();
    commserv.regd.Name.MakeUpper();
    commserv.sadmin.Name.MakeUpper();
    commserv.sop.Name.MakeUpper();
    commserv.admin.Name.MakeUpper();
    commserv.oper.Name.MakeUpper();
    commserv.ovr_view.MakeUpper();
    commserv.ovr_owner.MakeUpper();
    commserv.ovr_cs_mode.MakeUpper();
    commserv.ovr_cs_op.MakeUpper();
    commserv.ovr_cs_voice.MakeUpper();
    commserv.ovr_cs_invite.MakeUpper();
    commserv.ovr_cs_unban.MakeUpper();
    commserv.ovr_cs_clear.MakeUpper();
    AddCommands();

    map_entry < Committee_t > comm;
    if (commserv.IsList(commserv.sadmin.Name))
    {
	comm = commserv.GetList(commserv.sadmin.Name);
	MLOCK(("CommServ", "list", commserv.sadmin.Name, "member"));
	while (comm->size())
	{
	    comm->member = comm->begin();
	    comm->erase();
	}
    }
    else
    {
	comm = map_entry < Committee_t > (new Committee_t(commserv.sadmin.Name, "Services Administrators"));
	commserv.AddList(comm);
    }
    comm->Secure(commserv.sadmin.Secure);
    comm->Private(commserv.sadmin.Private);
    comm->OpenMemos(commserv.sadmin.OpenMemos);
    {
	MLOCK(("CommServ", "list", commserv.sadmin.Name, "member"));
	for (i = 1; i <= operserv.services_admin.WordCount(", "); i++)
	    comm->insert(operserv.services_admin.ExtractWord(i, ", "), operserv.FirstName());
    }

    if (commserv.IsList(commserv.sop.Name))
	comm = commserv.GetList(commserv.sop.Name);
    else
    {
	comm =
	    map_entry < Committee_t >
	    (new Committee_t(commserv.sop.Name, *(commserv.GetList(commserv.sadmin.Name).entry()), "Services Operators"));
	commserv.AddList(comm);
    }
    comm->Secure(commserv.sop.Secure);
    comm->Private(commserv.sop.Private);
    comm->OpenMemos(commserv.sop.OpenMemos);

    if (commserv.IsList(commserv.admin.Name))
	comm = commserv.GetList(commserv.admin.Name);
    else
    {
	comm =
	    map_entry < Committee_t >
	    (new Committee_t(commserv.admin.Name, *(commserv.GetList(commserv.sadmin.Name).entry()), "Server Administrators"));
	commserv.AddList(comm);
    }
    comm->Secure(commserv.admin.Secure);
    comm->Private(commserv.admin.Private);
    comm->OpenMemos(commserv.admin.OpenMemos);

    if (commserv.IsList(commserv.oper.Name))
	comm = commserv.GetList(commserv.oper.Name);
    else
    {
	comm =
	    map_entry < Committee_t >
	    (new Committee_t(commserv.oper.Name, *(commserv.GetList(commserv.admin.Name).entry()), "Server Operators"));
	commserv.AddList(comm);
    }
    comm->Secure(commserv.oper.Secure);
    comm->Private(commserv.oper.Private);
    comm->OpenMemos(commserv.oper.OpenMemos);

    if (commserv.IsList(commserv.all.Name))
    {
	comm = commserv.GetList(commserv.all.Name);
	MLOCK(("CommServ", "list", commserv.all.Name, "member"));
	while (comm->size())
	{
	    comm->member = comm->begin();
	    comm->erase();
	}
    }
    else
    {
	comm =
	    map_entry < Committee_t >
	    (new Committee_t(commserv.all.Name, *(commserv.GetList(commserv.admin.Name).entry()), "All Users"));
	commserv.AddList(comm);
    }
    comm->Secure(false);
    comm->Private(true);
    comm->OpenMemos(false);

    if (commserv.IsList(commserv.regd.Name))
    {
	comm = commserv.GetList(commserv.regd.Name);
	MLOCK(("CommServ", "list", commserv.regd.Name, "member"));
	while (comm->size())
	{
	    comm->member = comm->begin();
	    comm->erase();
	}
    }
    else
    {
	comm =
	    map_entry < Committee_t >
	    (new Committee_t(commserv.regd.Name, *(commserv.GetList(commserv.sop.Name).entry()), "Registered Users"));
	commserv.AddList(comm);
    }
    comm->Secure(false);
    comm->Private(true);
    comm->OpenMemos(false);

    if (reconnect && Connected())
    {
	server.
	    raw(((server.proto.Tokens() &&
		  !server.proto.GetNonToken("ERROR").empty())? server.proto.GetNonToken("ERROR") : mstring("ERROR")) + " " +
		" :Closing Link: Configuration reload required restart!");
	Disconnect();
    }

    DumpE();
    CP(("%s read and loaded to live configuration.", i_config_file.c_str()));
    RET(true);
}

int SignalHandler::handle_signal(int signum, siginfo_t * si, ucontext_t * uctx)
{
    static_cast < void >(si);
    static_cast < void >(uctx);

    // No trace, screws with LastFunc...
    //FT("SignalHandler::handle_signal", (signum, "(siginfo_t *) si", "(ucontext_t *) uctx"));
    ThreadID *tid;

    // todo: fill this sucker in
    switch (signum)
    {
#if defined(SIGPIPE) && (SIGPIPE!=0)
    case SIGPIPE:
	{
	    vector < ThreadID * >ids = mThread::findall();
	    for (unsigned int i = 0; i < ids.size(); i++)
		if (ids[i] != NULL)
		    ids[i]->Flush();
	}
	break;
#endif

    case SIGINT:		// Re-signon all clients
	LOG(LM_NOTICE, "SYS_ERRORS/SIGNAL_SIGNON", (signum));
	Magick::instance().server.SignOnAll();
	break;

#if defined(SIGIOT) && (SIGIOT != 0)
    case SIGIOT:		// Thread abort ...
	{
	    ACE_Thread_Manager *thr_mgr = NULL;

	    switch (Magick::instance().hh.ThreadType())
	    {
	    case Heartbeat_Handler::H_Worker:
		{
		    RLOCK(("IrcSvcHandler"));
		    if (Magick::instance().ircsvchandler != NULL)
			thr_mgr = &Magick::instance().ircsvchandler->tm;
		    else
			thr_mgr = &Magick::instance().thr_mgr();
		}
		break;
	    case Heartbeat_Handler::H_IrcServer:
		{
		    RLOCK(("IrcSvcHandler"));
		    if (Magick::instance().ircsvchandler != NULL)
			thr_mgr = Magick::instance().ircsvchandler->thr_mgr();
		}
		if (thr_mgr == NULL)
		    thr_mgr = &Magick::instance().thr_mgr();
		Magick::instance().Disconnect();
		if (Magick::instance().dh_timer > 0)
		    Magick::instance().reactor().cancel_timer(Magick::instance().dh_timer);
		Magick::instance().dh_timer = 0;
		break;
	    case Heartbeat_Handler::H_Events:
		{
		    WLOCK(("Events"));
		    if (Magick::instance().events != NULL)
		    {
			thr_mgr = Magick::instance().events->thr_mgr();
			// Cant afford to do the wait ...
//              if (!Magick::instance().events->fini())
//                  Magick::instance().events->wait();
			delete Magick::instance().events;

			Magick::instance().events = NULL;
		    }
		    if (thr_mgr == NULL)
			thr_mgr = &Magick::instance().thr_mgr();
		    Magick::instance().events = new EventTask(&Magick::instance().thr_mgr());
		    Magick::instance().events->open((void *) &Magick::instance());
		}
		break;
	    case Heartbeat_Handler::H_DCC:
		{
		    WLOCK(("DCC"));
		    if (Magick::instance().dcc != NULL)
		    {
			thr_mgr = Magick::instance().dcc->thr_mgr();
			// Cant afford to do the wait ...
//              if (!Magick::instance().dcc->fini())
//                  Magick::instance().dcc->close();
			delete Magick::instance().dcc;

			Magick::instance().dcc = NULL;
		    }
		    if (thr_mgr == NULL)
			thr_mgr = &Magick::instance().thr_mgr();
		    Magick::instance().dcc = new DccMap(&Magick::instance().thr_mgr());
		    Magick::instance().dcc->open((void *) &Magick::instance());
		}
		break;
	    case Heartbeat_Handler::H_Main:	// Its a REAL SIGABRT ...
		{
		    vector < ThreadID * >ids = mThread::findall();
		    for (unsigned int i = 0; i < ids.size(); i++)
			if (ids[i] != NULL)
			    ids[i]->Flush();
		}
		tid = mThread::find();
		LOG(LM_ALERT, "SYS_ERRORS/SIGNAL_KILL", (signum, tid->LastFunc()));
		ANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/SIGNAL_KILL", (signum, tid->LastFunc()));
		ACE_OS::sleep(1);
		Magick::instance().Shutdown(true);
		Magick::instance().Die();
		return -1;
	    default:
		// Invalid ... WTF? ignore.
		break;
	    }
	    Magick::instance().hh.RemoveThread();
	    if (thr_mgr != NULL)
		thr_mgr->exit();
	    break;
	}
#endif

#if defined(SIGTERM) && (SIGTERM != 0)
    case SIGTERM:		// Save DB's (often prequil to -KILL!)
	{
	    RLOCK(("Events"));
	    if (Magick::instance().events != NULL)
	    {
		LOG(LM_NOTICE, "SYS_ERRORS/SIGNAL_SAVE", (signum));
		ANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/SIGNAL_SAVE", (signum));
		Magick::instance().events->ForceSave();
	    }
	}
	break;
#endif

#if defined(SIGHUP) && (SIGHUP != 0)
    case SIGHUP:		// Reload CFG/DB's
	LOG(LM_NOTICE, "SYS_ERRORS/SIGNAL_LOAD", (signum));
	if (!Magick::instance().get_config_values())
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NO_CFG_FILE", (Magick::instance().Config_File()));
	}
	else
	{
	    ANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/SIGNAL_LOAD", (signum));
	}
	break;
#endif

    case SIGILL:		// illegal opcode, this suckers gone walkabouts..
#if defined(SIGBUS) && (SIGBUS != 0)
    case SIGBUS:		// BUS error (fatal)
#endif
    case SIGSEGV:		// Segmentation Fault
	{
	    vector < ThreadID * >ids = mThread::findall();
	    for (unsigned int i = 0; i < ids.size(); i++)
		if (ids[i] != NULL)
		    ids[i]->Flush();
	}
	tid = mThread::find();
	LOG(LM_ALERT, "SYS_ERRORS/SIGNAL_KILL", (signum, tid->LastFunc()));
	ANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/SIGNAL_KILL", (signum, tid->LastFunc()));
	ACE_OS::sleep(1);
	Magick::instance().Shutdown(true);
	Magick::instance().Die();
	return -1;
	break;

    default:			// Everything else.
	LOG(LM_WARNING, "SYS_ERRORS/SIGNAL_IGNORE", (signum));
	break;			//ignore (todo log that we got it and we're ignoring it)
    }
    return 0;
}

bool Magick::ValidateLogger(ACE_Log_Msg * instance) const
{
    FT("Magick::ValidateLogger", ("(ACE_Log_Msg *) instance"));

    if (instance == NULL)
	RET(false);

    if (instance->msg_callback() != logger)
    {
	instance->open(i_programname.c_str());
	instance->msg_callback(logger);
    }

    if (logger != NULL)
    {
	if (!(instance->flags() & ACE_Log_Msg::MSG_CALLBACK))
	{
	    instance->set_flags(ACE_Log_Msg::MSG_CALLBACK);
	    instance->clr_flags(ACE_Log_Msg::STDERR);
	}
    }
    else
    {
	if (!(instance->flags() & ACE_Log_Msg::STDERR))
	{
	    instance->set_flags(ACE_Log_Msg::STDERR);
	    instance->clr_flags(ACE_Log_Msg::MSG_CALLBACK);
	}
    }
    RET(true);
}

bool Magick::ActivateLogger()
{
    NFT("Magick::ActivateLogger");
    if (logger != NULL)
	delete logger;

    logger = new Logger;
    if (logger != NULL && !logger->opened())
    {
	delete logger;

	logger = NULL;
    }
    RET(logger != NULL);
}

void Magick::DeactivateLogger()
{
    NFT("Magick::DeactivateLogger");
    if (logger != NULL)
	delete logger;

    logger = NULL;
}

void Magick::EndLogMessage(ACE_Log_Msg * instance) const
{
    FT("Magick::EndLogMessage", ("(ACE_Log_Msg *) instance"));

    if (instance == NULL)
	return;

    if (instance->flags() & ACE_Log_Msg::STDERR)
	fprintf(stderr, "\n");
}

Logger::Logger()
{
    NFT("Logger::Logger");
    ACE_Log_Msg::enable_debug_messages();

    fout.Open(Magick::instance().files.Logfile(), "a");
}

Logger::~Logger()
{
    NFT("Logger::~Logger");

    if (fout.IsOpened())
	fout.Close();
}

void Logger::log(ACE_Log_Record & log_record)
{
    FT("Logger::log", ("(ACE_Log_Record &) log_record"));

    if (!fout.IsOpened())
	return;

    mstring text_priority;

    switch (log_record.type())
    {

	// Stuff normal users dont wanna see
    case LM_TRACE:
#ifndef DEBUG
	return;
#endif
	text_priority = "TRACE";
	break;

	// Stuff you have to turn VERBOSE on to see
    case LM_DEBUG:
	if (!Magick::instance().Verbose())
	    return;
	text_priority = "DEBUG";
	break;

	// Normal information that most users want
    case LM_INFO:
	text_priority = "INFO";
	break;

	// Still normal, but notable (eg. SOP commands)
    case LM_NOTICE:
	text_priority = "NOTICE";
	break;

	// An attempt at data corruption (denied) or recoverable error
    case LM_WARNING:
	text_priority = "WARNING";
	break;

	// Startup messages only (ie. boot)
    case LM_STARTUP:
	text_priority = "STARTUP";
	break;

	// A data corruption that we repaird
    case LM_ERROR:
	text_priority = "ERROR";
	break;

	// A data corruption we could not repair (left in system or
	// more drastic action taken, eg. the record being removed)
    case LM_CRITICAL:
	text_priority = "CRITICAL";
	break;

	// A situation that caused us to hard abort, but did not kill
	// us but may have caused a thread to die, etc.
    case LM_ALERT:
	text_priority = "ALERT";
	break;

	// A situation we could not reover from, we're outtahere.
    case LM_EMERGENCY:
	text_priority = "FATAL";
	break;

	// Should NEVER get this, but its there for completeness.
    default:
	text_priority = "UNKNOWN";
	break;
    }

    /* Pulled directly from ACE ... */
    time_t sec = log_record.time_stamp().sec();
    struct tm *tmval = localtime(&sec);
    char ctp[21];		// 21 is a magic number...

    if (ACE_OS::strftime(ctp, sizeof(ctp), "%d %b %Y %H:%M:%S", tmval) == 0)
	return;

    mstring out;

    out.Format("%s.%03ld | %-8s | ", ctp, log_record.time_stamp().usec() / 1000, text_priority.c_str());
    mstring tmp(log_record.msg_data());

    unsigned int i;

    for (i = 1; i <= tmp.WordCount("\n\r"); i++)
	fout.Write(out + tmp.ExtractWord(i, "\n\r"));

    fout.Flush();

    if (!Magick::instance().files.Logchan().empty() && Magick::instance().Connected())
    {
	{
	    RLOCK(("IrcSvcHandler"));
	    if (Magick::instance().ircsvchandler != NULL && !Magick::instance().ircsvchandler->Burst())
	    {
		if (Magick::instance().chanserv.IsLive(Magick::instance().files.Logchan()))
		{
		    for (i = 1; i <= tmp.WordCount("\n\r"); i++)
			Magick::instance().server.PRIVMSG(Magick::instance().operserv.FirstName(),
							  Magick::instance().files.Logchan(), out + tmp.ExtractWord(i,
														    "\n\r"));
		}
	    }
	}
    }

    if (log_record.type() == LM_EMERGENCY)
	exit(MAGICK_RET_ERROR);
}

void Logger::close()
{
    NFT("Logger::close");

    if (fout.IsOpened())
    {
	fout.Close();
    }
}

void Logger::open()
{
    NFT("Logger::open");

    if (!fout.IsOpened())
    {
	fout.Open(Magick::instance().files.Logfile(), "a");
    }
}

bool Logger::opened() const
{
    NFT("Logger::opened");
    bool retval = fout.IsOpened();

    RET(retval);
}

/*
void Magick::handle(const mstring & server, const mstring & command, const mstring & functionname)
{
   pair<mstring,mstring> data=pair<mstring,mstring>(server, command);
   handlermap[data].insert(handlermap[data].begin(),functionname);
}

void Magick::stophandling(const mstring & server, const mstring & command, const mstring & functionname)
{
   pair<mstring,mstring> data=pair<mstring,mstring>(server, command);
   if(checkifhandled(server,command)&&find(handlermap[data].begin(),handlermap[data].end(),functionname)!=handlermap[data].end())
       handlermap[data].erase(find(handlermap[data].begin(),handlermap[data].end(),functionname));
}

bool Magick::checkifhandled(const mstring & server, const mstring & command)
{
   pair<mstring,mstring> data=pair<mstring,mstring>(server, command);
   if(handlermap.find(data)!=handlermap.end())
   {
       return !(handlermap[data].empty());
   }
   else
       return false;
}

void Magick::doscripthandle(const mstring& server, const mstring& command, const mstring& data)
{
    if(checkifhandled(server,command)==true)
    {
	//todo
    }
}
*/

bool Magick::startup_t::IsServer(const mstring & svr) const
{
    FT("Magick::startup_t::IsServer", (svr));

    RLOCK(("Startup", "Servers"));
    if (servers.find(svr.LowerCase()) != servers.end())
    {
	RET(true);
    }
    RET(false);
}

pair < unsigned int, triplet < unsigned int, mstring, unsigned long > >Magick::startup_t::Server(const mstring & svr) const
{
    FT("Magick::startup_t::Server", (svr));
    pair < unsigned int, triplet < unsigned int, mstring, unsigned long > >value(0, triplet < unsigned int, mstring,
										 unsigned long >(0, "", 0));

    RLOCK(("Startup", "Servers"));
    if (IsServer(svr))
    {
	value = servers.find(svr.LowerCase())->second;
    }
    NRET(pair < unsigned int.triplet < unsigned int.mstring.unsigned long > >, value);
}

vector < mstring > Magick::startup_t::PriorityList(const unsigned int pri) const
{
    FT("Magick::startup_t::PriorityList", (pri));
    vector < mstring > list;

    map < mstring, pair < unsigned int, triplet < unsigned int, mstring, unsigned long > > >::const_iterator iter;

    RLOCK(("Startup", "Servers"));
    for (iter = servers.begin(); iter != servers.end(); iter++)
    {
	if (iter->second.first == pri)
	    list.push_back(iter->first);
    }
    NRET(vector < mstring >, list);
}

bool Magick::startup_t::IsAllowed(const mstring & svr, const mstring & uplink) const
{
    FT("Magick::startup_t::IsAllowed", (svr, uplink));

    map < mstring, vector < mstring > >::const_iterator i;
    RLOCK(("Startup", "Allows"));
    if (!allows.size())
    {
	RET(true);
    }

    for (i = allows.begin(); i != allows.end(); i++)
    {
	if (svr == i->first)
	{
	    if (i->second.size())
	    {
		vector < mstring >::const_iterator j;
		RLOCK2(("Startup", "Allows", i->first));
		for (j = i->second.begin(); j != i->second.end(); j++)
		{
		    if (uplink.Matches(*j))
		    {
			RET(true);
		    }
		}
	    }
	    else
	    {
		RET(true);
	    }
	    break;
	}
    }

    RET(false);
}

vector < mstring > Magick::startup_t::Allow(const mstring & svr) const
{
    FT("Magick::startup_t::Allow", (svr));

    map < mstring, vector < mstring > >::const_iterator i;
    RLOCK(("Startup", "Allows"));
    for (i = allows.begin(); i != allows.end(); i++)
    {
	if (svr == i->first)
	{
	    NRET(vector < mstring >, i->second);
	}
    }

    vector < mstring > blank;
    NRET(vector < mstring >, blank);
}

vector < mstring > Magick::startup_t::AllowList() const
{
    NFT("Magick::startup_t::AllowList");
    vector < mstring > list;

    map < mstring, vector < mstring > >::const_iterator i;
    RLOCK(("Startup", "Allows"));
    for (i = allows.begin(); i != allows.end(); i++)
    {
	list.push_back(i->first);
    }

    NRET(vector < mstring >, list);
}

pair < mstring, mstring > Magick::GetKeys() const
{
    NFT("Magick::GetKeys");
    pair < mstring, mstring > retval;
#ifdef HASCRYPT
    if (!files.KeyFile().empty() && mFile::Exists(files.KeyFile()))
    {
	mFile keyfile(files.KeyFile());
	char key1[MAX_KEYLEN], key2[MAX_KEYLEN], tmp[MAX_KEYLEN];
	char instr[MD5_DIGEST_LENGTH], outstr[MD5_DIGEST_LENGTH], verify[VERIFY_SIZE];

	// First verify syntax is correct!
	memset(instr, 0, MD5_DIGEST_LENGTH);
	keyfile.Read(instr, MD5_DIGEST_LENGTH);
	mCRYPT(instr, outstr, MD5_DIGEST_LENGTH, CRYPTO_KEY1, CRYPTO_KEY2, 0);
	memset(verify, 0, VERIFY_SIZE);
#if defined(BUILD_NODE) && defined(BUILD_TYPE) && defined(BUILD_REL)
	snprintf(verify, VERIFY_SIZE, "%s %s Keyfile: %s %s %s", PACKAGE, VERSION, BUILD_NODE, BUILD_TYPE, BUILD_REL);
#else
	snprintf(verify, VERIFY_SIZE, "%s %s Keyfile: No host information available", PACKAGE, VERSION);
#endif
	memset(instr, 0, MD5_DIGEST_LENGTH);
	mHASH16(verify, VERIFY_SIZE, instr);

	if (memcmp(instr, outstr, MD5_DIGEST_LENGTH) == 0)
	{
	    /* Use keyfile keys to get REAL key */
	    memset(tmp, 0, MAX_KEYLEN);
	    keyfile.Read(tmp, MAX_KEYLEN);
	    mCRYPT(tmp, key1, MAX_KEYLEN, CRYPTO_KEY1, CRYPTO_KEY2, 0);
	    memset(tmp, 0, MAX_KEYLEN);
	    keyfile.Read(tmp, MAX_KEYLEN);
	    mCRYPT(tmp, key2, MAX_KEYLEN, CRYPTO_KEY1, CRYPTO_KEY2, 0);
	    retval = pair < mstring, mstring > (key1, key2);
	    fflush(stdout);
	}
	else
	    LOG(LM_CRITICAL, "ERROR/KEY_CORRUPT", (files.KeyFile()));
    }
#endif
    NRET(pair < mstring_mstring >, retval);
}

void Magick::Disconnect(const bool reconnect)
{
    FT("Magick::Disconnect", (reconnect));
    Magick::instance().server.SignOffAll(startup.Services_Quitmsg());
    MCB(i_reconnect);
    i_reconnect = reconnect;
    MCE(i_reconnect);
    server.sraw("QUIT :" + startup.Services_Quitmsg());
    {
	RLOCK(("IrcSvcHandler"));
	if (ircsvchandler != NULL)
	{
	    if (Magick::instance().hh.ThreadType() != Heartbeat_Handler::H_IrcServer)
	    {
		if (dh_timer == 0)
		{
		    ACE_Thread_Manager *thr_mgr = Magick::instance().ircsvchandler->thr_mgr();

		    if (thr_mgr == NULL)
			thr_mgr = &Magick::instance().thr_mgr();
#if defined(SIGIOT) && (SIGIOT != 0)
		    thr_mgr->kill_task(Magick::instance().ircsvchandler, SIGIOT);
#endif
		    while (Magick::instance().Pause())
			ACE_OS::sleep(1);
		    dh_timer = reactor().schedule_timer(&dh, NULL, ACE_Time_Value(10));
		}
		else if (dh_timer < 0)
		{
		    dh.handle_timeout(ACE_Time_Value::zero, NULL);
		}
	    }
	    else
	    {
		dh.handle_timeout(ACE_Time_Value::zero, NULL);
	    }
	}
    }
}

void Magick::send(const mstring & in) const
{
    RLOCK(("IrcSvcHandler"));
    if (ircsvchandler != NULL)
	ircsvchandler->send(in);
}

SXP::Tag Magick::tag_Magick("Magick");

void Magick::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Magick::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    if (pElement->IsA(operserv.GetClassTag()))
    {
	pIn->ReadTo(&operserv);
    }
    else if (pElement->IsA(nickserv.GetClassTag()))
    {
	pIn->ReadTo(&nickserv);
    }
    else if (pElement->IsA(chanserv.GetClassTag()))
    {
	pIn->ReadTo(&chanserv);
    }
    else if (pElement->IsA(memoserv.GetClassTag()))
    {
	pIn->ReadTo(&memoserv);
    }
    else if (pElement->IsA(commserv.GetClassTag()))
    {
	pIn->ReadTo(&commserv);
    }
    else if (pElement->IsA(filesys.GetClassTag()))
    {
	pIn->ReadTo(&filesys);
    }
    else
    {
	// Scripted ...
    }
}

void Magick::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    static_cast < void >(pIn);

    FT("Magick::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
    if (pElement->IsA(tag_Magick))
    {
	operserv.PostLoad();
	nickserv.PostLoad();
	chanserv.PostLoad();
	memoserv.PostLoad();
	commserv.PostLoad();
	filesys.PostLoad();
	// Scripted ...
    }
}

void Magick::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    static_cast < void >(attribs);

    FT("Magick::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));

    // not sure if this is the right place to do this
    {
	SXP::dict attr;
	attr["version"] = VERSION;
	pOut->BeginObject(tag_Magick, attr);
    }

    pOut->WriteSubElement(&operserv);
    pOut->WriteSubElement(&nickserv);
    pOut->WriteSubElement(&chanserv);
    pOut->WriteSubElement(&memoserv);
    pOut->WriteSubElement(&commserv);
    pOut->WriteSubElement(&filesys);

    pOut->EndObject(tag_Magick);
}

void Magick::save_databases()
{
    NFT("Magick::save_databases");
    if (i_saving)
	return;
    i_saving = true;
    if (mFile::Exists(files.Database() + ".old"))
	mFile::Erase(files.Database() + ".old");
    if (mFile::Exists(files.Database() + ".new"))
	mFile::Erase(files.Database() + ".new");
    if (mFile::Exists(files.Database()))
	mFile::Copy(files.Database(), files.Database() + ".old");
    {
	long retval = 0;
	Stage *ls = NULL;
	XMLStage *xs = NULL;
	CompressStage *zs = NULL;
	CryptStage *cs = NULL;
	FileStage *fs = NULL;

	xs = new XMLStage(this);
	if (xs != NULL && xs->Validate())
	    ls = xs;
	else
	{
	    NLOG(LM_CRITICAL, "SYS_ERRORS/STAGE_FAIL_ABORT");
	    goto CleanUp;
	}

	if (files.Compression())
	{
	    zs = new CompressStage(*ls, files.Compression());
	    if (zs != NULL && zs->Validate())
		ls = zs;
	    else
		NLOG(LM_ALERT, "SYS_ERRORS/STAGE_FAIL");
	}

	if (files.Encryption())
	{
	    pair < mstring, mstring > keys = GetKeys();
	    if (keys.first.length() && keys.second.length())
	    {
		cs = new CryptStage(*ls, keys.first, keys.second);
		if (cs != NULL && cs->Validate())
		    ls = cs;
		else
		    NLOG(LM_ALERT, "SYS_ERRORS/STAGE_FAIL");
	    }
	}

	fs = new FileStage(*ls, files.Database() + ".new");
	if (fs != NULL && fs->Validate())
	    ls = fs;
	else
	{
	    NLOG(LM_CRITICAL, "SYS_ERRORS/STAGE_FAIL_ABORT");
	    goto CleanUp;
	}

	if (ls != NULL)
	    retval = ls->Consume();

      CleanUp:
	if (fs != NULL)
	    delete fs;

	if (cs != NULL)
	    delete cs;

	if (zs != NULL)
	    delete zs;

	if (xs != NULL)
	    delete xs;
    }
    if (mFile::Exists(files.Database() + ".new"))
    {
	mFile::Copy(files.Database() + ".new", files.Database());
	mFile::Erase(files.Database() + ".new");
    }
    if (mFile::Exists(files.Database() + ".old"))
	mFile::Erase(files.Database() + ".old");
    i_saving = false;
}

void Magick::load_databases()
{
    NFT("Magick::load_databases");
    if (mFile::Exists(files.Database()))
    {
	NLOG(LM_STARTUP, "EVENT/LOAD");

	long retval = 0;
	Stage *ls = NULL;
	FileStage *fs = NULL;
	CryptStage *cs = NULL;
	CompressStage *zs = NULL;
	VerifyStage *vs = NULL;
	XMLStage *xs = NULL;

	fs = new FileStage(files.Database());
	if (fs != NULL && fs->Validate())
	    ls = fs;
	else
	{
	    NLOG(LM_CRITICAL, "SYS_ERRORS/STAGE_FAIL_ABORT");
	    goto CleanUp;
	}

	if (ls->GetTag() & STAGE_TAG_CRYPT)
	{
	    pair < mstring, mstring > keys = GetKeys();
	    if (keys.first.length() && keys.second.length())
	    {
		cs = new CryptStage(*ls, keys.first, keys.second);
		if (cs != NULL && cs->Validate())
		    ls = cs;
		else
		    NLOG(LM_ALERT, "SYS_ERRORS/STAGE_FAIL");
	    }
	}

	if (ls->GetTag() & STAGE_TAG_COMPRESS)
	{
	    zs = new CompressStage(*ls, 0);
	    if (zs != NULL && zs->Validate())
		ls = zs;
	    else
		NLOG(LM_ALERT, "SYS_ERRORS/STAGE_FAIL");
	}

	vs = new VerifyStage(*ls, 0, XML_STRING, strlen(XML_STRING));
	if (vs != NULL && vs->Validate())
	    ls = vs;
	else
	{
	    NLOG(LM_CRITICAL, "SYS_ERRORS/STAGE_FAIL_ABORT");
	    goto CleanUp;
	}

	xs = new XMLStage(*ls, this);
	if (xs != NULL && xs->Validate())
	    ls = xs;
	else
	{
	    NLOG(LM_CRITICAL, "SYS_ERRORS/STAGE_FAIL_ABORT");
	    goto CleanUp;
	}

	if (ls != NULL)
	    retval = ls->Consume();
	if (retval <= 0)
	{
	    switch (static_cast < stage_errors > (retval * -1))
	    {
	    case SE_CRY_NoKeys:	// Encrypted but no/invalid key specified
		NLOG(LM_EMERGENCY, "ERROR/DB_NOKEY");
		break;
	    case SE_CRY_CryptError:	// Decryption failed
		NLOG(LM_EMERGENCY, "ERROR/DB_NODECRYPT");
		break;
	    case SE_CPS_StreamError:	// Decompression failed
		NLOG(LM_EMERGENCY, "ERROR/DB_NODECOMPRESS");
		break;
	    case SE_VFY_Failed:	// Sanity check failed
		NLOG(LM_EMERGENCY, "ERROR/DB_NOSANITY");
		break;
	    case SE_XML_ParseError:	// XML parse failed
		NLOG(LM_EMERGENCY, "ERROR/DB_NOPARSE");
		break;
	    default:
		NLOG(LM_EMERGENCY, "ERROR/DB_NOPROCESS");
		break;
	    }
	}

      CleanUp:
	if (xs != NULL)
	    delete xs;

	if (vs != NULL)
	    delete vs;

	if (zs != NULL)
	    delete zs;

	if (cs != NULL)
	    delete cs;

	if (fs != NULL)
	    delete fs;
    }
}

set < mstring > Magick::LNG_Loaded() const
{
    set < mstring > retval;
    map < mstring, map < mstring, mstring > >::const_iterator i;
    RLOCK(("Messages"));
    for (i = Messages.begin(); i != Messages.end(); i++)
    {
	retval.insert(i->first);
    }
    return retval;
}

size_t Magick::LNG_Usage(const mstring & lang) const
{
    size_t retval = 0;

    map < mstring, map < mstring, mstring > >::const_iterator i;
    map < mstring, mstring >::const_iterator j;
    RLOCK(("Messages"));
    i = Messages.find(lang.UpperCase());
    if (i != Messages.end())
    {
	retval += i->first.capacity();
	RLOCK2(("Messages", lang.UpperCase()));
	for (j = i->second.begin(); j != i->second.end(); j++)
	{
	    retval += j->first.capacity();
	    retval += j->second.capacity();
	}
    }

    return retval;
}

set < mstring > Magick::HLP_Loaded() const
{
    set < mstring > retval;
    map < mstring, map < mstring, vector < triplet < mstring, mstring, mstring > > > >::const_iterator i;
    RLOCK(("Help"));
    for (i = Help.begin(); i != Help.end(); i++)
    {
	retval.insert(i->first);
    }
    return retval;
}

size_t Magick::HLP_Usage(const mstring & lang) const
{
    size_t retval = 0;

    map < mstring, map < mstring, vector < triplet < mstring, mstring, mstring > > > >::const_iterator i;
    map < mstring, vector < triplet < mstring, mstring, mstring > > >::const_iterator j;
    vector < triplet < mstring, mstring, mstring > >::const_iterator k;

    RLOCK(("Help"));
    i = Help.find(lang.UpperCase());
    if (i != Help.end())
    {
	retval += i->first.capacity();
	RLOCK2(("Help", lang.UpperCase()));
	for (j = i->second.begin(); j != i->second.end(); j++)
	{
	    retval += j->first.capacity();
	    for (k = j->second.begin(); k != j->second.end(); k++)
	    {
		retval += k->first.capacity();
		retval += k->second.capacity();
		retval += k->third.capacity();
	    }
	}
    }

    return retval;
}

size_t Magick::LFO_Usage() const
{
    size_t retval = 0;

    map < mstring, mstring >::const_iterator i;
    RLOCK(("LogMessages"));
    for (i = LogMessages.begin(); i != LogMessages.end(); i++)
    {
	retval += i->first.capacity();
	retval += i->second.capacity();
    }

    return retval;
}

void Magick::DumpB() const
{
    MB(0,
       (argv.size(), Messages.size(), Help.size(), LogMessages.size(), handlermap.size(), i_verbose, i_services_dir,
	i_config_file, i_programname, i_ResetTime, i_level, i_pause, i_auto, i_shutdown, i_reconnect, i_localhost));
    MB(16, (i_gotconnect, i_currentserver, i_connected, i_saving));
}

void Magick::DumpE() const
{
    ME(0,
       (argv.size(), Messages.size(), Help.size(), LogMessages.size(), handlermap.size(), i_verbose, i_services_dir,
	i_config_file, i_programname, i_ResetTime, i_level, i_pause, i_auto, i_shutdown, i_reconnect, i_localhost));
    ME(16, (i_gotconnect, i_currentserver, i_connected, i_saving));
}
