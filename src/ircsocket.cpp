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
#define RCSID(x,y) const char *rcsid_ircsocket_cpp_ ## x () { return y; }
RCSID(ircsocket_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.182  2001/11/17 03:16:02  prez
** Extra logging, actually made DCC identify as a DCC thread, and fixed some
** mkdir failures ...
**
** Revision 1.181  2001/11/16 20:27:33  prez
** Added a MAX_THREADS option, and made the thread heartbeat a timer based
** operation, instead of part of the threads.
**
** Revision 1.180  2001/11/12 01:05:02  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.179  2001/11/07 06:30:45  prez
** Fixing some indenting and an unused veriable.
**
** Revision 1.178  2001/11/04 23:43:14  prez
** Updates for MS Visual C++ compilation (it works now!).
**
** Revision 1.177  2001/11/03 21:02:53  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.176  2001/08/04 18:32:02  prez
** Made some changes for Hybrid 6 -- we now work with it ... mostly.
**
** Revision 1.175  2001/07/21 18:09:44  prez
** Fixed IsBool in mstring and made SVINFO actually give a GMT timestamp.
**
** Revision 1.174  2001/07/06 09:15:37  prez
** Fixed nickserv drop
**
** Revision 1.173  2001/07/05 15:23:54  prez
** Fixed the channel topic carryover
**
** Revision 1.172  2001/07/05 05:59:11  prez
** More enhansements to try and avoid Signal #6's, coredumps, and deadlocks.
**
** Revision 1.171  2001/07/03 06:00:07  prez
** More deadlock fixes ... also cleared up the Signal #6 problem.
**
** Revision 1.170  2001/07/01 05:02:45  prez
** Added changes to dependancy system so it wouldnt just remove a dependancy
** after the first one was satisfied.
**
** Revision 1.169  2001/06/17 09:39:07  prez
** Hopefully some more changes that ensure uptime (mainly to do with locking
** entries in an iterated search, and using copies of data instead of references
** where we can get away with it -- reducing the need to lock the data).
**
** Revision 1.168  2001/06/16 09:35:24  prez
** More tiny bugs ...
**
** Revision 1.167  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.166  2001/06/11 03:44:45  prez
** Re-wrote how burst works, and made the burst message a lower priority
** than normal.  Also removed the chance of a stray pointer being picked
** up in the dependancy system.
**
** Revision 1.165  2001/05/28 11:17:34  prez
** Added some more anti-deadlock stuff, and fixed nick ident warnings
**
** Revision 1.164  2001/05/25 01:59:31  prez
** Changed messaging system ...
**
** Revision 1.163  2001/05/13 00:55:18  prez
** More patches to try and fix deadlocking ...
**
** Revision 1.162  2001/05/08 03:22:27  prez
** Removed one possible deadlock cause, and stopped events engine from doing
** anything until synch is over.
**
** Revision 1.161  2001/05/06 03:03:07  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.160  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.159  2001/05/04 03:43:33  prez
** Fixed UMODE problems (re-oper) and problems in mstring erase
**
** Revision 1.158  2001/05/04 01:11:13  prez
** Made chanserv mode stuff more efficiant
**
** Revision 1.157  2001/05/03 22:34:35  prez
** Fixed SQUIT protection ...
**
** Revision 1.156  2001/05/03 04:40:17  prez
** Fixed locking mechanism (now use recursive mutexes) ...
** Also now have a deadlock/nonprocessing detection mechanism.
**
** Revision 1.155  2001/05/02 02:35:27  prez
** Fixed dependancy system, and removed printf's - we no longer coredump on
** a 1000 user network.  As a bonus, we actually synd perfectly ;P
**
** Revision 1.154  2001/05/01 14:00:23  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.153  2001/04/13 00:46:38  prez
** Fixec channel registering
**
** Revision 1.152  2001/04/09 07:52:22  prez
** Fixed /nickserv.  Fixed cordump in nick expiry.  Fixed slight bugs in mstring.
**
** Revision 1.151  2001/03/27 07:04:31  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.150  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.149  2001/03/04 20:23:18  prez
** Tweaked akill expiry to avoid iterator dependancies ... should work :P
**
** Revision 1.148  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.147  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.146  2001/02/03 02:21:33  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.145  2001/01/15 23:31:38  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.144  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.143  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.142  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.141  2000/12/19 14:26:55  prez
** Bahamut has changed SVSNICK -> MODNICK, so i_SVS has been changed into
** several SVS command text strings, if blank, support isnt there.
**
** Revision 1.140  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.139  2000/12/10 13:06:12  prez
** Ditched alot of the *toa's since mstring can do it internally now.
**
** Revision 1.138  2000/10/10 11:47:51  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.137  2000/10/04 10:52:08  prez
** Fixed the memory pool and removed printf's.
**
** Revision 1.136  2000/10/04 07:39:46  prez
** Added MemCluster to speed up lockable, but it cores when we start
** getting real messages -- seemingly in an alloc in the events.
** Lots of printf's left in for debugging (so run as ./magick >output)
**
** Revision 1.135  2000/09/30 10:48:07  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.134  2000/09/27 11:21:39  prez
** Added a BURST mode ...
**
** Revision 1.133  2000/09/22 12:26:11  prez
** Fixed that pesky bug with chanserv not seeing modes *sigh*
**
** Revision 1.132  2000/09/13 12:45:33  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.131  2000/09/12 21:17:02  prez
** Added IsLiveAll (IsLive now checks to see if user is SQUIT).
**
** Revision 1.130  2000/09/09 02:17:48  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.129  2000/09/07 08:13:17  prez
** Fixed some of the erronous messages (SVSHOST, SQLINE, etc).
** Also added CPU statistics and fixed problem with socket deletions.
**
** Revision 1.128  2000/09/06 11:27:33  prez
** Finished the T_Modify / T_Changing traces, fixed a bug in clone
** adding (was adding clone liimt as the mask length), updated docos
** a little more, and added a response to SIGINT to signon clients.
**
** Revision 1.127  2000/08/31 06:25:09  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.126  2000/08/28 10:51:37  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.125  2000/08/22 08:43:41  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.124  2000/08/19 10:59:47  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.123  2000/08/10 22:44:23  prez
** Added 'binding to IP' options for shell servers, etc.  Also added akick
** triggers for when a user changes their nick and suddenly matches akick.
**
** Revision 1.122  2000/08/07 22:38:50  prez
** Stopped the 'expiring nickname  ()' messages.
**
** Revision 1.121  2000/08/07 12:20:27  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.120  2000/08/06 08:06:41  prez
** Fixed loading of logon messages in committee ..
**
** Revision 1.119  2000/08/06 05:27:47  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.118  2000/08/03 13:06:31  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.117  2000/07/30 09:04:05  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.116  2000/07/29 21:58:53  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.115  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.114  2000/06/25 11:58:03  prez
** Fixed problem where messages from nickserv about killing user would not
** be sent out (people would not know a nick was forbidden).
**
** Revision 1.113  2000/06/25 10:32:41  prez
** Fixed channel forbid.
**
** Revision 1.112  2000/06/25 07:58:50  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.111  2000/06/18 13:31:48  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.110  2000/06/18 12:49:27  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.109  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.108  2000/06/08 13:07:34  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.107  2000/06/06 08:57:56  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.106  2000/05/27 15:10:12  prez
** Misc changes, mainly re-did the makefile system, makes more sense.
** Also added a config.h file.
**
** Revision 1.105  2000/05/27 07:06:01  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.104  2000/05/26 11:21:28  prez
** Implemented HTM (High Traffic Mode) -- Can be used at a later date.
**
** Revision 1.103  2000/05/22 13:00:09  prez
** Updated version.h and some other stuff
**
** Revision 1.102  2000/05/21 04:49:40  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.101  2000/05/20 15:17:00  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.100  2000/05/20 03:28:11  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.99  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.98  2000/05/14 06:30:14  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.97  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.96  2000/04/04 03:13:50  prez
** Added support for masking hostnames.
**
** Revision 1.95  2000/03/28 16:20:58  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.94  2000/03/27 21:26:12  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.93  2000/03/24 15:35:18  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.92  2000/03/19 08:50:55  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.91  2000/03/15 14:42:58  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.90  2000/03/15 08:23:51  prez
** Added locking stuff for commserv options, and other stuff
**
** Revision 1.89  2000/03/14 10:05:16  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.88  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.87  2000/03/02 07:25:11  prez
** Added stuff to do the chanserv greet timings (ie. only greet if a user has
** been OUT of channel over 'x' seconds).  New stored chanserv cfg item.
**
** Revision 1.86  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.85  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.84  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.83  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.82  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"

const char *Heartbeat_Handler::names[] = { "invalid", "worker", "main",
					"IRC server", "events", "DCC" };


void *IrcSvcHandler::worker(void *in)
{
    mThread::Attach(tt_mBase);
    FT("IrcSvcHandler::worker", (in));
    mMessage *msg = NULL;
    bool active = true;

    Parent->hh.AddThread(Heartbeat_Handler::H_Worker);    
    try {
	while(!Parent->Shutdown() && active)
	{
	    Parent->hh.Heartbeat();

	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL)
	    {
		MLOCK(("MessageQueue"));
	    	msg = dynamic_cast<mMessage *>(Parent->ircsvchandler->message_queue.dequeue());
	    }}
	    if (msg != NULL)
	    {
		int rv = msg->call();
		delete msg;
		if (rv < 0)
		    break;
	    }

	    size_t msgcnt = 0, thrcnt = 0;
	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL)
	    {
		msgcnt = Parent->ircsvchandler->message_queue.method_count();
		thrcnt = Parent->ircsvchandler->tm.count_threads();
	    }}

	    CP(("thread count = %d, message queue = %d, lwm = %d, hwm = %d",
		thrcnt, msgcnt,
		Parent->config.Low_Water_Mark() + (Parent->config.High_Water_Mark() * (thrcnt-2)),
		thrcnt * Parent->config.High_Water_Mark()));

	    if(thrcnt > Parent->config.Min_Threads() &&
		msgcnt < Parent->config.Low_Water_Mark() +
		(Parent->config.High_Water_Mark() * (thrcnt-2)))
	    {
		COM(("Low water mark reached, killing thread."));
		NLOG(LM_NOTICE, "EVENT/KILL_THREAD");
		active = false;
	    }

	    FLUSH();
	}
    }
    catch (E_Thread &e)
    {
	e.what();
    }

    Parent->hh.RemoveThread();
    DRET(static_cast<void *>(NULL));
}


int IrcSvcHandler::open(void *in)
{
    //mThread::Attach(tt_MAIN);
    FT("IrcSvcHandler::open", (in));
    this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
    Parent->hh.AddThread(Heartbeat_Handler::H_IrcServer);

    sock.Bind(&Parent->ircsvchandler->peer(), D_From, false);
    sock.Resolve(S_IrcServer, Parent->CurrentServer());

    in_traffic = out_traffic = 0;
    connect_time = mDateTime::CurrentDateTime();
    htm_level = 0;
    htm_gap = Parent->operserv.Init_HTM_Gap();
    htm_threshold = Parent->operserv.Init_HTM_Thresh();
    last_htm_check = mDateTime::CurrentDateTime();
    i_burst = true;
    i_synctime = mDateTime(0.0);

    // Only activate the threads when we're ready.
    tm.spawn_n(Parent->config.Min_Threads(), IrcSvcHandler::worker);

    DumpB();
    CP(("IrcSvcHandler activated"));
    RET(0);
}

int IrcSvcHandler::handle_input(ACE_HANDLE hin)
{
    static_cast<void>(hin);

    mThread::Attach(tt_MAIN);
    FT("IrcSvcHandler::handle_input", ("(ACE_HANDLE) hin"));
    Parent->hh.Heartbeat();

    //todo this is the sucker that get's data from the socket, so this is our main routine.
    // might set this up to be an active object here.
    char data[513];
    int recvResult;
    memset(data,0,513);
    recvResult=sock.recv(data,512);
    if(recvResult<=0 || Parent->Shutdown())
	return -1;

    // possibly mstring(data,0,recvResult); rather than mstring(data)
    // depends on null terminators etc.

    // Traffic Accounting ...
    map<time_t, size_t>::iterator iter;
    time_t now = time(NULL);
    DumpB();
    
    { WLOCK(("IrcSvcHandler", "traffic"));
    for (iter=traffic.begin(); iter != traffic.end() &&
		iter->first < now - static_cast<time_t>(Parent->operserv.Max_HTM_Gap()+2); iter = traffic.begin())
	traffic.erase(iter->first);
    if (traffic.find(now) == traffic.end())
	traffic[now] = 0;
    traffic[now] += ACE_OS::strlen(data);
    in_traffic += ACE_OS::strlen(data);
    }

    // Check to see if we're in HTM.
    { WLOCK(("IrcSvcHandler", "htm_gap"));
    WLOCK2(("IrcSvcHandler", "htm_level"));
    WLOCK3(("IrcSvcHandler", "last_htm_check"));
    if (static_cast<time_t>(last_htm_check.SecondsSince()) > htm_gap)
    {
	last_htm_check = mDateTime::CurrentDateTime();
	size_t total = 0;
	time_t i;
	{ RLOCK(("IrcSvcHandler", "traffic"));
	for (i = now - (htm_gap + 1); i<now; i++)
	    if (traffic.find(i) != traffic.end())
		total += traffic[i];
	}
	RLOCK(("IrcSvcHandler", "htm_threshold"));
	if (total > (htm_gap * htm_threshold))
	{
	    if (htm_gap > static_cast<time_t>(Parent->operserv.Max_HTM_Gap()))
	    {
		NANNOUNCE(Parent->operserv.FirstName(),
			"MISC/HTM_DIE");
		CP(("HTM gap limit reached"));
		return -1;
	    }
	    else
	    {
		if (!htm_level)
		    ANNOUNCE(Parent->operserv.FirstName(),
			"MISC/HTM_ON", (
			fmstring("%.1f", static_cast<float>(total) /
				static_cast<float>(htm_gap) / 1024.0),
			fmstring("%.1f", static_cast<float>(htm_threshold) / 1024.0)));
		else if (htm_level < 3)
		    ANNOUNCE(Parent->operserv.FirstName(),
			"MISC/HTM_STILL", (
			htm_level + 1, htm_gap,
			fmstring("%.1f", static_cast<float>(total) /
				static_cast<float>(htm_gap) / 1024.0)));
		else
		    ANNOUNCE(Parent->operserv.FirstName(),
			"MISC/HTM_TURBO", (
			htm_level + 1, htm_gap,
			fmstring("%.1f", static_cast<float>(total) /
				static_cast<float>(htm_gap) / 1024.0)));
		htm_level++;
		htm_gap += 2;
		LOG(LM_NOTICE, "OPERSERV/HTM_ON", (htm_level, htm_gap,
			fmstring("%.1f", static_cast<float>(total) /
				static_cast<float>(htm_gap) / 1024.0),
			fmstring("%.1f", static_cast<float>(htm_threshold) / 1024.0)));
	    }
	}
	else if (htm_level)
	{
	    NANNOUNCE(Parent->operserv.FirstName(),
		"MISC/HTM_OFF");
	    htm_level = 0;
	    htm_gap = Parent->operserv.Init_HTM_Gap();
	    NLOG(LM_NOTICE, "OPERSERV/HTM_OFF");
	}
    }}

    unsigned int i;
    mstring data2 = flack + data;
    flack.erase();
    // if(recvResult==-1) major problem.
    // if(recvResult==0) socket has close down    

    if(data2.Contains("\n")||data2.Contains("\r"))
    {
	for(i=1;i<data2.WordCount("\n\r");i++)
	{
	    mstring text = data2.ExtractWord(i,"\n\r");
	    if(!text.empty())
	    {
		enqueue(text);
	    }
	}

	if (data2.last() == '\n' || data2.last() == '\r')
	{
	    mstring text = data2.ExtractWord(i,"\n\r");
	    if(!text.empty())
	    {
		enqueue(text);
	    }
	}
	else
	{
	    flack = data2.ExtractWord(i, "\n\r");
	}

	FLUSH();
    }
    else
        flack = data2;
    DumpE();

    DRET(0);
}

int IrcSvcHandler::handle_close (ACE_HANDLE h, ACE_Reactor_Mask mask)
{
    static_cast<void>(h);
    static_cast<void>(mask);

    mThread::Attach(tt_MAIN);
    FT("IrcSvcHandler::handle_close", ("(ACE_HANDLE hin)", "(ACE_Reactor_Mask) mask"));
    CP(("IrcSvcHandler closed"));

    LOG(LM_ERROR, "OTHER/CLOSED", (Parent->CurrentServer()));

    unsigned int i;
    // We DONT want any processing once we're gone ...
    // Dump the queue and kill all our threads nicely.
    for (i=0; i<static_cast<unsigned int>(tm.count_threads()); i++)
	enqueue_sleep();
    { MLOCK(("MessageQueue"));
    mMessage *msg;
    while (!message_queue.is_empty())
    {
	msg = dynamic_cast<mMessage *>(message_queue.dequeue());
	if (msg != NULL)
	    delete msg;
    }}
    for (i=0; i<static_cast<unsigned int>(tm.count_threads()); i++)
	enqueue_shutdown();

    { MLOCK(("AllDependancies"));
    mMessage::AllDependancies.clear();
    }
    { MLOCK(("MsgIdMap"));
    map<unsigned long, mMessage *>::iterator mi;
    for (mi=mMessage::MsgIdMap.begin(); mi!=mMessage::MsgIdMap.end(); mi++)
	delete mi->second;
    mMessage::MsgIdMap.clear();
    }

    // Should I do this with SQUIT protection ...?
    { WLOCK(("NickServ", "recovered"));
    Parent->nickserv.recovered.clear();
    }

    // Essentially here, we enact SQUIT protection ...
    { WLOCK(("Server", "ToBeSquit"));
    WLOCK2(("Server", "ServerSquit"));
    Parent->server.DumpB();
    CB(0, Parent->server.ToBeSquit.size());
    CB(1, Parent->server.ServerSquit.size());
    Server::list_t::iterator si;
    { RLOCK(("Server", "list"));
    for (si=Parent->server.ListBegin(); si!=Parent->server.ListEnd(); si++)
    {
	RLOCK2(("Server", "list", si->first));
	if (Parent->server.ToBeSquit.find(si->first) != Parent->server.ToBeSquit.end())
	    Parent->server.ToBeSquit.erase(si->first);
	if (Parent->server.ServerSquit.find(si->first) != Parent->server.ServerSquit.end())
	{
	    mstring *arg = NULL;
	    if (ACE_Reactor::instance()->cancel_timer(
		Parent->server.ServerSquit[si->first], reinterpret_cast<const void **>(arg))
		&& arg != NULL)
		delete arg;
	}
	if (!Parent->Shutdown())
	{
	    CP(("Scheduling SQUIT protect timer..."));
	    Parent->server.ServerSquit[si->first] =
		ACE_Reactor::instance()->schedule_timer(&Parent->server.squit,
		new mstring(si->first),
		ACE_Time_Value(Parent->config.Squit_Protect()));
	}
    }}
    CE(1, Parent->server.ServerSquit.size());
    CE(0, Parent->server.ToBeSquit.size());
    Parent->server.DumpE();
    }
  
    if (!Parent->Shutdown())
    {
	RLOCK(("NickServ", "live"));
	NickServ::live_t::iterator iter;
	vector<mstring> chunked;
	for (iter=Parent->nickserv.LiveBegin(); iter != Parent->nickserv.LiveEnd(); iter++)
	{
	    RLOCK2(("NickServ", "live", iter->first));
	    if (iter->second.IsServices())
	    {
		chunked.push_back(iter->first);
	    }
	    else if (Parent->server.IsList(iter->second.Server()))
	    {
		iter->second.SetSquit();
	    }
	}
	// Sign off services if we have NO uplink
	for (i=0; i<chunked.size(); i++)
	    Parent->server.QUIT(chunked[i], "SQUIT - " + Parent->startup.Server_Name());
    }

    { WLOCK(("Server", "list"));
    Parent->server.i_list.clear();
    }
    Parent->server.OurUplink("");

    // Let other threads process shutdown instruction
    // Otherwise cancel them.

    if (tm.count_threads())
    {
	ACE_Time_Value tv(time(NULL)+10);
	tm.wait(&tv);
	if (tm.count_threads())
	{
#if defined(SIGIOT) && (SIGIOT != 0)
	    tm.kill_all(SIGIOT);
	    tv.set(time(NULL)+10);
	    tm.wait(&tv);
	    if (tm.count_threads())
#endif
		tm.cancel_all();
	}
    }

    // This sets fini() == 1
    if (sock.IsConnected())
	sock.close();
    Parent->Connected(false);

    if (!Parent->Shutdown() && Parent->Reconnect() &&
	Parent->config.Server_Relink() >= 1)
    {
	CP(("Scheduling reconnect"));
	ACE_Reactor::instance()->schedule_timer(&(Parent->rh), NULL, 
		ACE_Time_Value(Parent->config.Server_Relink()));
    }

    Parent->hh.RemoveThread();
    this->reactor()->remove_handler(this, ACE_Event_Handler::READ_MASK);
//  this->destroy();
    DRET(0);
}

int IrcSvcHandler::fini()
{
    return sock.IsConnected() ? 0 : 1;
}

time_t IrcSvcHandler::HTM_Gap() const
{
    NFT("IrcSvcHandler::HTM_Gap");
    RLOCK(("IrcSvcHandler", "htm_gap"));
    RET(htm_gap);
}

unsigned short IrcSvcHandler::HTM_Level() const
{
    NFT("IrcSvcHandler::HTM_Level");
    RLOCK(("IrcSvcHandler", "htm_level"));
    RET(htm_level);
}

size_t IrcSvcHandler::HTM_Threshold() const
{
    NFT("IrcSvcHandler::HTM_Threshold");
    RLOCK(("IrcSvcHandler", "htm_threshold"));
    RET(htm_threshold);
}

void IrcSvcHandler::HTM_Threshold(const size_t in)
{
    FT("IrcSvcHandler::HTM_Threshold", (in));
    WLOCK(("IrcSvcHandler", "htm_threshold"));
    MCB(htm_threshold);
    htm_threshold = in;
    MCE(htm_threshold);
}


void IrcSvcHandler::HTM(const bool in)
{
    FT("IrcSvcHandler::HTM", (in));
    WLOCK(("IrcSvcHandler", "last_htm_check"));
    WLOCK2(("IrcSvcHandler", "htm_level"));
    WLOCK3(("IrcSvcHandler", "htm_gap"));
    MCB(last_htm_check);
    CB(1, htm_level);
    CB(2, htm_gap);
    last_htm_check = mDateTime::CurrentDateTime();
    if (in)
    {
	htm_level = 4;
	htm_gap = Parent->operserv.HTM_On_Gap();
    }
    else
    {
	htm_level = 0;
	htm_gap = Parent->operserv.Init_HTM_Gap();
    }
    CE(1, htm_level);
    CE(2, htm_gap);
    MCE(last_htm_check);
}

size_t IrcSvcHandler::Average(time_t secs) const
{
    FT("IrcSvcHandler::Average", (secs));
    time_t now = time(NULL);
    size_t total = 0;
    int i = 0;
    map<time_t, size_t>::const_iterator iter;
    if (secs > static_cast<time_t>(Parent->operserv.Max_HTM_Gap()))
	secs = 0;
    RLOCK(("IrcSvcHandler", "traffic"));
    for (iter=traffic.begin(); iter != traffic.end() &&
			iter->first < now; iter++)
    {
	if (secs ? iter->first >= (now-1) - secs : 1)
	{
	    total += iter->second;
	    i++;
	}
    }
    RET(total / (i ? i : 1));
}

bool IrcSvcHandler::Burst() const
{
    NFT("IrcSvcHandler::Burst");
    RLOCK(("IrcSvcHandler", "i_burst"));
    RET(i_burst);
}

mDateTime IrcSvcHandler::SyncTime() const
{
    NFT("IrcSvcHandler::SyncTime");
    RLOCK(("IrcSvcHandler", "i_synctime"));
    RET(i_synctime);
}

void IrcSvcHandler::EndBurst()
{
    NFT("IrcSvcHandler::EndBurst");
    WLOCK(("IrcSvcHandler", "i_burst"));
    WLOCK2(("IrcSvcHandler", "i_synctime"));
    MCB(i_burst);
    CB(1, i_synctime);
    i_burst = false;
    i_synctime = mDateTime::CurrentDateTime();
    CE(1, i_synctime);
    MCE(i_burst);
}

int IrcSvcHandler::send(const mstring & data)
{
    FT("IrcSvcHandler::send",(data));
    int recvResult;
    mstring tmp(data);
    tmp.replace("\n", "");
    tmp.replace("\r", "");
    out_traffic += tmp.length() + 2;
    recvResult=sock.send(const_cast<char *>((tmp + "\n\r").c_str()),
							tmp.length() + 2);
    CH(D_To,tmp);
    RET(recvResult);
}

void IrcSvcHandler::enqueue(mMessage *mm)
{
    FT("IrcSvcHandler::enqueue", (mm));

    if (mm == NULL)
	return;

    // Make sure we have at LEAST our minimum ...
    while (static_cast<unsigned int>(tm.count_threads()) < Parent->config.Min_Threads())
    {
	if (tm.spawn(IrcSvcHandler::worker) != -1)
	{
	    NLOG(LM_NOTICE, "EVENT/NEW_THREAD");
	}
	else
	    CP(("Could not start new thread (below thread threshold)!"));
    }

    // Only spawn if we are less than our maximum ... and need it :)
    if (static_cast<unsigned int>(tm.count_threads()) < Parent->config.Max_Threads() &&
	message_queue.method_count() > static_cast<int>(tm.count_threads() * Parent->config.High_Water_Mark()))
    {
	CP(("Queue is full - Starting new thread and increasing watermarks ..."));
	if(tm.spawn(IrcSvcHandler::worker) != -1)
	{
	    NLOG(LM_NOTICE, "EVENT/NEW_THREAD");
	}
	else
	    CP(("Could not start new thread to handle load!"));
    }

    message_queue.enqueue(mm);
}

void IrcSvcHandler::enqueue(const mstring &message, const u_long pri)
{
    FT("IrcSvcHandler::enqueue",(message, pri));
    CH(D_From,message);

    u_long p(pri);
    mstring source, msgtype, params;
    if (message[0u] == ':' || message[0u] == '@')
    {
	source = message.ExtractWord(1,": ");
        msgtype = message.ExtractWord(2,": ").UpperCase();
	if (message.WordCount(" ") > 2)
	    params = message.After(" ", 2);
    }
    else
    {
        msgtype = message.ExtractWord(1,": ").UpperCase();
	if (message.WordCount(" ") > 1)
	    params = message.After(" ");
    }

    // Exception case ...
    // We dont want to end burst until everything else
    // is done ... sometimes this happens prematurely.
    if (msgtype == "303")
	p = P_Delay;

    try {
	mMessage *msg = new mMessage(source, msgtype, params, p);
	if (!msg->OutstandingDependancies())
	    enqueue(msg);
    }
    catch (E_NickServ_Live &e)
    {
	switch(e.where())
	{
	    case E_NickServ_Live::W_Get:
		switch (e.type())
		{
		case E_NickServ_Live::T_Invalid:
		case E_NickServ_Live::T_Blank:
		    if (strlen(e.what()) && Parent->nickserv.IsLiveAll(e.what()))
		    {
			Parent->nickserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_ChanServ_Live &e)
    {
	switch(e.where())
	{
	    case E_ChanServ_Live::W_Get:
		switch (e.type())
		{
		case E_ChanServ_Live::T_Invalid:
		case E_ChanServ_Live::T_Blank:
		    if (strlen(e.what()) && Parent->chanserv.IsLive(e.what()))
		    {
			Parent->chanserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_Server_List &e)
    {
	switch(e.where())
	{
	    case E_Server_List::W_Get:
		switch (e.type())
		{
		case E_Server_List::T_Invalid:
		case E_Server_List::T_Blank:
		    if (strlen(e.what()) && Parent->server.IsList(e.what()))
		    {
			Parent->server.RemList(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (exception &e)
    {
	LOG(LM_CRITICAL, "EXCEPTIONS/UNHANDLED", ( e.what()));
    }
    catch (...)
    {
	NLOG(LM_CRITICAL, "EXCEPTIONS/UNKNOWN");
    }
}

void IrcSvcHandler::enqueue_shutdown()
{
    NFT("IrcSvcHandler::enqueue_shutdown");
    mMessage *msg = new mMessage(" ", "SHUTDOWN", "", P_System);
    message_queue.enqueue(msg);
}

void IrcSvcHandler::enqueue_sleep(const mstring &in)
{
    NFT("IrcSvcHandler::enqueue_sleep");
    mMessage *msg = new mMessage(" ", "SLEEP", in, P_System);
    message_queue.enqueue(msg);
}

void IrcSvcHandler::enqueue_test()
{
    NFT("IrcSvcHandler::enqueue_test");
    mMessage *msg = new mMessage(" ", "TEST", "", P_System);
    message_queue.enqueue(msg);
}

void IrcSvcHandler::DumpB() const
{
    MB(0, (traffic.size(), in_traffic, out_traffic, connect_time,
	last_htm_check, htm_level, htm_gap, htm_threshold, i_burst,
	i_synctime));
}

void IrcSvcHandler::DumpE() const
{
    ME(0, (traffic.size(), in_traffic, out_traffic, connect_time,
	last_htm_check, htm_level, htm_gap, htm_threshold, i_burst,
	i_synctime));
}

mstring Reconnect_Handler::FindNext(const mstring& i_server) {
    FT("Reconnect_Handler::FindNext", (i_server));
    mstring result, server(i_server.LowerCase());

    // IF current server is found
    //     IF last server of this priority
    //         IF size of serverlist of next priority > 0 size
    //             return first element of serverlist of next priority
    //         ELSE return NULL
    //     ELSE return next server of this priority
    // ELSE return NULL

    if (Parent->startup.IsServer(server)) {
	vector<mstring> serverlist = Parent->startup.PriorityList(Parent->startup.Server(server).first);
	vector<mstring>::iterator iter;
	for (iter=serverlist.begin(); iter!=serverlist.end(); iter++)
	    if (*iter == server)
		break;

	if (iter != serverlist.end()) iter++;

	if (iter == serverlist.end())
	{
	    serverlist = Parent->startup.PriorityList(Parent->startup.Server(server).first+1);

	    if (serverlist.size())
	    {
		RET(*serverlist.begin());
	    }
	}
	else
	{
	    RET(*iter);
	}
    }
    RET("");
}

int Heartbeat_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    static_cast<void>(tv);
    static_cast<void>(arg);

    mThread::Attach(tt_MAIN);
    FT("Heartbeat_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));

    if (Parent->Shutdown())
	DRET(0);

    vector<ACE_thread_t> dead;
    threads_t::iterator iter;
    unsigned int i;

    { RLOCK(("Heartbeat_Handler", "threads"));
    CP(("Starting HEARTBEAT for %d entries (%s) ...", threads.size(),
		mDateTime::CurrentDateTime().DateTimeString().c_str()));
    for (iter=threads.begin(); iter!=threads.end(); iter++)
    {
	COM(("Checking %s thread (last checkin %s)", names[iter->second.first],
				iter->second.second.DateTimeString().c_str()));
	if (iter->second.second.SecondsSince() > Parent->config.Heartbeat_Time())
	{
	    dead.push_back(iter->first);
	}
    }
    if (dead.size() > (threads.size() / 2))
    {
	NANNOUNCE(Parent->operserv.FirstName(), "MISC/THREAD_DEAD_HALF");
	NLOG(LM_EMERGENCY, "SYS_ERRORS/THREAD_DEAD_HALF");
    }
    else if (dead.size())
    {
	WLOCK(("Heartbeat_Handler", "threads"));
	for (i=0; i<dead.size(); i++)
	{
	    iter = threads.find(dead[i]);
	    if (iter != threads.end())
	    {
		if (iter->second.first != H_Main)
		    LOG(LM_CRITICAL, "SYS_ERRORS/THREAD_DEAD",
					(names[iter->second.first]));
		switch (iter->second.first)
		{
		case H_Worker:
		    { RLOCK2(("IrcSvcHandler"));
		    if (Parent->ircsvchandler != NULL)
			Parent->ircsvchandler->tm.cancel(dead[i]);
		    }
		    break;
		case H_Main:
		    NANNOUNCE(Parent->operserv.FirstName(),
					"MISC/THREAD_DEAD_MAIN");
		    NLOG(LM_EMERGENCY, "SYS_ERRORS/THREAD_DEAD_MAIN");
		    break;
		case H_IrcServer:
		    Parent->Disconnect();
		    break;
		case H_Events:
		    { WLOCK(("Events"));
		    if (Parent->events != NULL)
		    {
			ACE_Thread_Manager *thr_mgr = Parent->events->thr_mgr();
			if (thr_mgr != NULL)
			    thr_mgr->cancel_task(Parent->events);
			Parent->events->close(0);
			delete Parent->events;
		    }
		    Parent->events = new EventTask;
		    if (Parent->events != NULL)
			Parent->events->open();
		    }
		    break;
		case H_DCC:
		    { WLOCK(("DCC"));
		    if (Parent->dcc != NULL)
		    {
			ACE_Thread_Manager *thr_mgr = Parent->dcc->thr_mgr();
			if (thr_mgr != NULL)
			    thr_mgr->cancel_task(Parent->dcc);
			Parent->dcc->close(0);
			delete Parent->dcc;
		    }
		    Parent->dcc = new DccMap;
		    if (Parent->dcc != NULL)
			Parent->dcc->open();
		    }
		    break;
		default:
		    break;
		}
		threads.erase(iter);
	    }
	}
    }}

    // Force workers to checkin ...
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL)
	for (i=0; i<threads.size(); i++)
	    Parent->ircsvchandler->enqueue_test();
    }
    ACE_Reactor::instance()->schedule_timer(this, 0,
		ACE_Time_Value(Parent->config.Heartbeat_Time()));
    DRET(0);
}

void Heartbeat_Handler::AddThread(heartbeat_enum type, ACE_thread_t id)
{
    FT("Heartbeat_Handler::AddThread", (static_cast<int>(type), id));
    WLOCK(("Heartbeat_Handler", "threads"));
    threads[id] = pair<heartbeat_enum, mDateTime>(type,
				mDateTime::CurrentDateTime());
}

void Heartbeat_Handler::RemoveThread(ACE_thread_t id = ACE_Thread::self())
{
    FT("Heartbeat_Handler::RemoveThread", (id));
    WLOCK(("Heartbeat_Handler", "threads"));
    threads_t::iterator iter = threads.find(id);
    if (iter != threads.end())
	threads.erase(iter);
}

void Heartbeat_Handler::Heartbeat(ACE_thread_t id = ACE_Thread::self())
{
    FT("Heartbeat_Handler::Heartbeat", (id));
    WLOCK(("Heartbeat_Handler", "threads"));
    threads_t::iterator iter = threads.find(id);
    if (iter != threads.end())
	iter->second.second = mDateTime::CurrentDateTime();
}

size_t Heartbeat_Handler::size()
{
    NFT("Heartbeat_Handler::size");
    size_t retval = 0;
    { RLOCK(("Heartbeat_Handler", "threads"));
    retval = threads.size();
    }
    RET(retval);
}

size_t Heartbeat_Handler::count(heartbeat_enum type)
{
    FT("Heartbeat_Handler::count", (static_cast<int>(type)));
    size_t retval = 0;
    { RLOCK(("Heartbeat_Handler", "threads"));
    threads_t::iterator iter;
    for (iter=threads.begin(); iter!=threads.end(); iter++)
	if (iter->second.first == type)
	    retval++;
    }

    RET(retval);
}

int Reconnect_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    static_cast<void>(tv);
    static_cast<void>(arg);

    mThread::Attach(tt_MAIN);
    FT("Reconnect_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));

    if(Parent->config.Server_Relink()<1 || !Parent->Reconnect() ||
	    Parent->Shutdown())
	DRET(0);

    mstring server;
    if (Parent->startup.Server_size())
    {
	if (Parent->GotConnect())
	{
	    server = Parent->startup.PriorityList(1)[0];
	    while (!server.empty() && !Parent->startup.IsAllowed(server, Parent->startup.Server_Name()))
	    {
		server = FindNext(server);
	    }
	} else {
	    server = Parent->CurrentServer();
	    if (!server.empty())
	    {
		server = FindNext(server);
		while (!server.empty() && !Parent->startup.IsAllowed(server, Parent->startup.Server_Name()))
		{
		    server = FindNext(server);
		}
	    }
	    if (server.empty())
	    {
		server = Parent->startup.PriorityList(1)[0];
		while (!server.empty() && !Parent->startup.IsAllowed(server, Parent->startup.Server_Name()))
		{
		    server = FindNext(server);
		}
	    }
	}
    }
    if (server.empty())
    {
	NLOG(LM_EMERGENCY, "OTHER/NOVALIDSERVERS");
    }

    pair<unsigned int, triplet<unsigned int,mstring,unsigned long> > details = Parent->startup.Server(server);

    ACE_INET_Addr addr(details.second.first, server);

    Parent->DumpB();
    CB(1, Parent->i_currentserver);
    Parent->GotConnect(false);
    Parent->i_currentserver = server;
    Parent->server.proto.Tokens(false);
    if (Parent->Connected())
	Parent->Disconnect();
    LOG(LM_INFO, "OTHER/CONNECTING", (server, details.second.first));

    IrcConnector C_server(ACE_Reactor::instance(),ACE_NONBLOCK);

    unsigned int i;
    for (i=1; i<5; i++)
    {
        mstring octet = Parent->startup.Bind().ExtractWord(i, ".", false);
	if (!octet.IsNumber() || atoi(octet.c_str()) < 0 ||
		atoi(octet.c_str()) > 255)
	    break;
    }

    ACE_INET_Addr laddr;
    unsigned short port = mSocket::FindAvailPort();
    if (i==5)
	laddr.set(port, Parent->startup.Bind().c_str());
    else
	laddr.set(port);

    int res = 0;
    { WLOCK(("IrcSvcHandler"));
    res = C_server.connect(Parent->ircsvchandler, addr,
    		ACE_Synch_Options::defaults, laddr);
    }
    if (res == -1)
    {
	LOG(LM_ERROR, "OTHER/REFUSED", (server, details.second.first));
	//okay we got a connection problem here. log it and try again
	CP(("Refused connection, rescheduling and trying again ..."));
	ACE_Reactor::instance()->schedule_timer(&(Parent->rh),0,ACE_Time_Value(Parent->config.Server_Relink()));
    }
    else
    {
	{ RLOCK(("IrcSvcHandler"));
	if (Parent->ircsvchandler != NULL)
	{
	    CB(2, Parent->i_localhost);
	    Parent->i_localhost = Parent->ircsvchandler->Local_IP();
	    CE(2, Parent->i_localhost);
	}}
	if (!Parent->server.proto.Protoctl().empty())
	    Parent->server.raw(Parent->server.proto.Protoctl());
	if (Parent->server.proto.TSora())
	    Parent->server.raw("PASS " + details.second.second + " :TS");
	else
	    Parent->server.raw("PASS " + details.second.second);
	mstring tmp;
	if (Parent->server.proto.Numeric())
	    tmp.Format(Parent->server.proto.Server().c_str(),
		Parent->startup.Server_Name().c_str(), 1,
		details.second.third, Parent->startup.Server_Desc().c_str());
	else
	    tmp.Format(Parent->server.proto.Server().c_str(),
		Parent->startup.Server_Name().c_str(), 1,
		Parent->startup.Server_Desc().c_str());
	Parent->server.raw(tmp);
	if (Parent->server.proto.TSora())
	    // SVINFO <TS_CURRENT> <TS_MIN> <STANDALONE> :<UTC-TIME>
	    Parent->server.raw("SVINFO 3 1 0 :" + mDateTime::CurrentDateTime().timetstring());
	Parent->Connected(true);
    }
    CE(1, Parent->i_currentserver);
    Parent->DumpE();

    DRET(0);
}

int ToBeSquit_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    mThread::Attach(tt_MAIN);
    // We ONLY get here if we didnt receive a SQUIT message in <10s
    // after any QUIT message with 2 valid servers in it
    FT("ToBeSquit_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    static_cast<void>(tv);
    mstring *tmp = reinterpret_cast<mstring *>(const_cast<void *>(arg));

    { WLOCK(("Server", "ServerSquit"));
    Parent->server.DumpB();
    CB(1, Parent->server.ServerSquit.size());
    Parent->server.ServerSquit.erase(*tmp);
    CE(1, Parent->server.ServerSquit.size());
    }

    if (Parent->server.IsList(*tmp))
    {
	LOG(LM_NOTICE, "OTHER/SQUIT_CANCEL", (tmp, Parent->server.GetList(tmp->LowerCase()).Uplink()));
    }
    else
    {
	LOG(LM_NOTICE, "OTHER/SQUIT_CANCEL", (tmp, "?"));
    }

    // QUIT all user's who faked it ...
    vector<mstring> chunked;
    { WLOCK2(("Server", "ToBeSquit"));
    if (Parent->server.ToBeSquit.find(*tmp) != Parent->server.ToBeSquit.end())
    {
	list<mstring>::iterator iter;
	CB(2, Parent->server.ToBeSquit.size());
	for (iter=Parent->server.ToBeSquit[*tmp].begin(); iter!=Parent->server.ToBeSquit[*tmp].end(); iter++)
	{
	    chunked.push_back(*iter);
	}
	Parent->server.ToBeSquit.erase(*tmp);
	CE(2, Parent->server.ToBeSquit.size());
    }}  
    vector<mstring>::iterator k;
    for (k=chunked.begin(); k!=chunked.end(); k++)
    {
	try
	{
	    if (Parent->nickserv.IsLiveAll(*k))
	    {
		Parent->nickserv.GetLive(*k).Quit("FAKE SQUIT - " + *tmp);
		Parent->nickserv.RemLive(*k);
		mMessage::CheckDependancies(mMessage::NickNoExists, *k);
	    }
	}
	catch (E_NickServ_Live &e)
	{
	    switch(e.where())
	    {
		case E_NickServ_Live::W_Get:
		    switch (e.type())
		    {
		    case E_NickServ_Live::T_Invalid:
		    case E_NickServ_Live::T_Blank:
			if (strlen(e.what()) && Parent->nickserv.IsLiveAll(e.what()))
			{
			    Parent->nickserv.RemLive(e.what());
			}
			break;
		    default:
			break;
		    }
		    break;
		default:
		    break;
	    }
	}
	catch (exception &e)
	{
	    LOG(LM_CRITICAL, "EXCEPTIONS/UNHANDLED", ( e.what()));
	}
	catch (...)
	{
	    NLOG(LM_CRITICAL, "EXCEPTIONS/UNKNOWN");
	}
    }
    Parent->server.DumpE();

    delete tmp;
    DRET(0);
}

int Squit_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    mThread::Attach(tt_MAIN);
    // OK -- we get here after we've passwd Squit_Protect()
    // seconds after a REAL squit
    FT("Squit_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    static_cast<void>(tv);
    mstring *tmp = reinterpret_cast<mstring *>(const_cast<void *>(arg));

    { WLOCK(("Server", "ServerSquit"));
    WLOCK2(("Server", "ToBeSquit"));
    Parent->server.DumpB();
    CB(1, Parent->server.ServerSquit.size());
    CB(2, Parent->server.ToBeSquit.size());
    Parent->server.ServerSquit.erase(*tmp);
    Parent->server.ToBeSquit.erase(*tmp);
    CE(1, Parent->server.ServerSquit.size());
    CE(2, Parent->server.ToBeSquit.size());
    Parent->server.DumpE();
    }

    // QUIT all user's who did not come back from SQUIT
    vector<mstring> SquitMe;
    NickServ::live_t::iterator i;
    { RLOCK(("NickServ", "live"));
    for (i=Parent->nickserv.LiveBegin(); i != Parent->nickserv.LiveEnd(); i++)
    {
	RLOCK2(("NickServ", "live", i->first));
	if (i->second.Squit() == *tmp)
	    SquitMe.push_back(i->first);
    }}
    vector<mstring>::iterator k;
    for (k=SquitMe.begin(); k != SquitMe.end(); k++)
    {
	try
	{
	    if (Parent->nickserv.IsLiveAll(*k))
	    {
		Parent->nickserv.GetLive(*k).Quit("SQUIT - " + *tmp);
		Parent->nickserv.RemLive(*k);
		mMessage::CheckDependancies(mMessage::NickNoExists, *k);
	    }
	}
	catch (E_NickServ_Live &e)
	{
	    switch(e.where())
	    {
		case E_NickServ_Live::W_Get:
		    switch (e.type())
		    {
		    case E_NickServ_Live::T_Invalid:
		    case E_NickServ_Live::T_Blank:
			if (strlen(e.what()) && Parent->nickserv.IsLiveAll(e.what()))
			{
			    Parent->nickserv.RemLive(e.what());
			}
			break;
		    default:
			break;
		    }
		    break;
		default:
		    break;
	    }
	}
	catch (exception &e)
	{
	    LOG(LM_CRITICAL, "EXCEPTIONS/UNHANDLED", ( e.what()));
	}
	catch (...)
	{
	    NLOG(LM_CRITICAL, "EXCEPTIONS/UNKNOWN");
	}
    }

    delete tmp;
    DRET(0);
}


int InFlight_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    mThread::Attach(tt_MAIN);
    // Memo timed out, send it!
    // If its a file, and not inprogress, ignore.
    FT("InFlight_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    static_cast<void>(tv);
    mstring *tmp = reinterpret_cast<mstring *>(const_cast<void *>(arg));

    if (Parent->nickserv.IsLiveAll(*tmp))
    {
	RLOCK(("NickServ", "live", tmp->LowerCase()));
	Nick_Live_t &entry = Parent->nickserv.GetLive(tmp->LowerCase());
	if (entry.InFlight.File())
	{
	    if (!entry.InFlight.InProg())
	    {
		/* Already handled by DccXfer::DccXfer
		SEND(entry.InFlight.service, "DCC/NOCONNECT", ( "GET"));
		*/
		entry.InFlight.Cancel();
	    }
	}
	else
	{
	    entry.InFlight.End(0u);
	}
    }
    delete tmp;
    DRET(0);
}


int Part_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    mThread::Attach(tt_MAIN);
    FT("Part_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    static_cast<void>(tv);
    mstring *tmp = reinterpret_cast<mstring *>(const_cast<void *>(arg));

    // This is to part channels I'm not supposed to be
    // in (ie. dont have JOIN on, and I'm the only user
    // in them).  ie. after AKICK, etc.
    try
    {
	if (Parent->chanserv.IsLive(*tmp) &&
		Parent->chanserv.GetLive(*tmp).IsIn(
			Parent->chanserv.FirstName()))
	{
	    Parent->server.PART(Parent->chanserv.FirstName(), *tmp);
	    if (Parent->chanserv.IsLive(*tmp))
	    {
		RLOCK(("ChanServ", "live", tmp->LowerCase()));
		Chan_Live_t &clive = Parent->chanserv.GetLive(tmp->LowerCase());
		clive.DumpB();
		{ MLOCK(("ChanServ", "live", tmp->LowerCase(), "ph_timer"));
		CB(1, clive.ph_timer);
		clive.ph_timer = 0;
		CE(1, clive.ph_timer);
		}
		clive.DumpE();
	    }
	}
    }
    catch (E_NickServ_Live &e)
    {
	switch(e.where())
	{
	    case E_NickServ_Live::W_Get:
		switch (e.type())
		{
		case E_NickServ_Live::T_Invalid:
		case E_NickServ_Live::T_Blank:
		    if (strlen(e.what()) && Parent->nickserv.IsLiveAll(e.what()))
		    {
			Parent->nickserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_ChanServ_Stored &e)
    {
	switch(e.where())
	{
	    case E_ChanServ_Stored::W_Get:
		switch (e.type())
		{
		case E_ChanServ_Stored::T_Invalid:
		case E_ChanServ_Stored::T_Blank:
		    if (strlen(e.what()) && Parent->chanserv.IsStored(e.what()))
		    {
			Parent->chanserv.RemStored(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_ChanServ_Live &e)
    {
	switch(e.where())
	{
	    case E_ChanServ_Live::W_Get:
		switch (e.type())
		{
		case E_ChanServ_Live::T_Invalid:
		case E_ChanServ_Live::T_Blank:
		    if (strlen(e.what()) && Parent->chanserv.IsLive(e.what()))
		    {
			Parent->chanserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (exception &e)
    {
	LOG(LM_CRITICAL, "EXCEPTIONS/UNHANDLED", ( e.what()));
    }
    catch (...)
    {
	NLOG(LM_CRITICAL, "EXCEPTIONS/UNKNOWN");
    }
    delete tmp;
    DRET(0);
}

void *EventTask::save_databases(void *in)
{
    static_cast<void>(in);
    mThread::Attach(tt_MAIN);
    Parent->save_databases();
    mThread::Detach();
    return NULL;
}

void EventTask::AddChannelModePending(const mstring &in)
{
    FT("EventTask::AddChannelModePending", (in));
    WLOCK(("Events", "cmodes_pending"));
    MCB(cmodes_pending.size());
    cmodes_pending.insert(in);
    MCE(cmodes_pending.size());
}


void EventTask::ForceSave()
{
    NFT("EventTask::ForceSave");
    WLOCK(("Events", "last_save"));
    MCB(last_save);
    last_save = mDateTime(0.0);
    MCE(last_save);
}

void EventTask::ForcePing()
{
    NFT("EventTask::ForcePing");
    WLOCK(("Events", "last_ping"));
    MCB(last_ping);
    last_ping = mDateTime(0.0);
    MCE(last_ping);
}

mstring EventTask::SyncTime(const mstring& source) const
{
    FT("EventTask::SyncTime", (source));
    RLOCK(("Events", "last_save"));
    mstring retval = ToHumanTime(Parent->config.Savetime() -
	last_save.SecondsSince(), source);
    RET(retval);
}

int EventTask::open(void *in)
{
    static_cast<void>(in);
    FT("EventTask::open", ("(void *) in"));
    int retval = activate();
    RET(retval);
}

int EventTask::close(unsigned long in)
{
    FT("EventTask::close", (in));
    // dump all and close open file handles.
    RET(0);
}


int EventTask::svc(void)
{
    mThread::Attach(tt_MAIN);
    // The biggie, so big, it has its own zip code ... uhh .. thread.
    NFT("EventTask::svc");
    Parent->hh.AddThread(Heartbeat_Handler::H_Events);

    bool proc;
    { WLOCK(("Events", "last_expire"));
    WLOCK2(("Events", "last_save"));
    WLOCK3(("Events", "last_check"));
    WLOCK4(("Events", "last_ping"));
    WLOCK5(("Events", "last_msgcheck"));
    last_expire = last_save = last_check = last_ping = last_msgcheck = mDateTime::CurrentDateTime();
    }
    DumpB();

    ACE_Thread_Manager thrmgr;
    mDateTime synctime;
    while(!Parent->Shutdown())
    {
	Parent->hh.Heartbeat();

	CP(("TIMERS:  Current time: %ld,  Earliest Timer: %ld",
		ACE_OS::gettimeofday().sec(),
		ACE_Reactor::instance()->timer_queue()->is_empty() ? 0 :
		ACE_Reactor::instance()->timer_queue()->earliest_time().sec()));

	proc = true;
	// Make sure we're turned on ...
	if (!Parent->AUTO())
	    proc = false;
	else
	{
	    // Make sure we're sync'd to network ...
	    RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler == NULL || Parent->ircsvchandler->Burst())
		proc = false;
	    else
		synctime = Parent->ircsvchandler->SyncTime();
	}

	if (!proc)
	{
	    ACE_OS::sleep(1);
	    continue;
	}


	try
        {
	    // This is mainly used for 'only do this if users have had
	    // enough time to rectify the situation since sync' ...
	    RLOCK_IF(("Events", "last_expire"),
		last_expire.SecondsSince() >= Parent->config.Cycletime())
	    {
		do_expire(synctime);
		WLOCK(("Events", "last_expire"));
		MCB(last_expire);
		last_expire = mDateTime::CurrentDateTime();
		MCE(last_expire);
	    }

	    if (Parent->Saving())
	    {
		WLOCK(("Events", "last_save"));
		MCB(last_save);
		last_save = mDateTime::CurrentDateTime();
		MCE(last_save);
	    }
	    RLOCK2_IF(("Events", "last_save"),
		last_save.SecondsSince() >= Parent->config.Savetime())
	    {
		CP(("Starting DATABASE SAVE ..."));
		thrmgr.spawn(save_databases, NULL);

		WLOCK(("Events", "last_save"));
		MCB(last_save);
		last_save = mDateTime::CurrentDateTime();
		MCE(last_save);
	    }

	    RLOCK2_IF(("Events", "last_check"),
		last_check.SecondsSince() >= Parent->config.Checktime())
	    {
		do_check(synctime);
		WLOCK(("Events", "last_check"));
		MCB(last_check);
		last_check = mDateTime::CurrentDateTime();
		MCE(last_check);
	    }

	    if (Parent->nickserv.IsLive(Parent->chanserv.FirstName()))
		do_modes(synctime);


	    RLOCK2_IF(("Events", "last_msgcheck"),
		last_msgcheck.SecondsSince() > Parent->config.MSG_Check_Time())
	    {
		do_msgcheck(synctime);
		WLOCK(("Events", "last_msgcheck"));
		MCB(last_msgcheck);
		last_msgcheck = mDateTime::CurrentDateTime();
		MCE(last_msgcheck);
	    }

	    RLOCK2_IF(("Events", "last_ping"),
		last_ping.SecondsSince() >= Parent->config.Ping_Frequency())
	    {
		do_ping(synctime);
		WLOCK(("Events", "last_ping"));
		MCB(last_ping);
		last_ping = mDateTime::CurrentDateTime();
		MCE(last_ping);
	    }
	}
	catch (E_NickServ_Stored &e)
	{
	    switch(e.where())
	    {
	    case E_NickServ_Stored::W_Get:
		switch (e.type())
		{
		case E_NickServ_Stored::T_Invalid:
		case E_NickServ_Stored::T_Blank:
		    if (strlen(e.what()) && Parent->nickserv.IsStored(e.what()))
		    {
			Parent->nickserv.RemStored(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_NickServ_Live &e)
	{
	    switch(e.where())
	    {
	    case E_NickServ_Live::W_Get:
		switch (e.type())
		{
		case E_NickServ_Live::T_Invalid:
		case E_NickServ_Live::T_Blank:
		    if (strlen(e.what()) && Parent->nickserv.IsLiveAll(e.what()))
		    {
			Parent->nickserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_NickServ_Recovered &e)
	{
	    switch(e.where())
	    {
	    case E_NickServ_Recovered::W_Get:
		switch (e.type())
		{
		case E_NickServ_Recovered::T_Invalid:
		case E_NickServ_Recovered::T_Blank:
		    if (strlen(e.what()) && Parent->nickserv.IsRecovered(e.what()))
		    {
			Parent->nickserv.RemRecovered(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_ChanServ_Stored &e)
	{
	    switch(e.where())
	    {
	    case E_ChanServ_Stored::W_Get:
		switch (e.type())
		{
		case E_ChanServ_Stored::T_Invalid:
		case E_ChanServ_Stored::T_Blank:
		    if (strlen(e.what()) && Parent->chanserv.IsStored(e.what()))
		    {
			Parent->chanserv.RemStored(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_ChanServ_Live &e)
	{
	    switch(e.where())
	    {
	    case E_ChanServ_Live::W_Get:
		switch (e.type())
		{
		case E_ChanServ_Live::T_Invalid:
		case E_ChanServ_Live::T_Blank:
		    if (strlen(e.what()) && Parent->chanserv.IsLive(e.what()))
		    {
			Parent->chanserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_CommServ_List &e)
	{
	    switch(e.where())
	    {
	    case E_CommServ_List::W_Get:
		switch (e.type())
		{
		case E_CommServ_List::T_Invalid:
		case E_CommServ_List::T_Blank:
		    if (strlen(e.what()) && Parent->commserv.IsList(e.what()))
		    {
			Parent->commserv.RemList(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_Server_List &e)
	{
	    switch(e.where())
	    {
	    case E_Server_List::W_Get:
		switch (e.type())
		{
		case E_Server_List::T_Invalid:
		case E_Server_List::T_Blank:
		    if (strlen(e.what()) && Parent->server.IsList(e.what()))
		    {
			Parent->server.RemList(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_MemoServ_Nick &e)
	{
	    e.what();
	}
	catch (E_MemoServ_Channel &e)
	{
	    e.what();
	}
	catch (E_DccMap_Xfers &e)
	{
	    switch(e.where())
	    {
	    case E_DccMap_Xfers::W_Get:
		switch (e.type())
		{
		case E_DccMap_Xfers::T_Invalid:
		case E_DccMap_Xfers::T_Blank:
		    if (strlen(e.what()) && DccMap::IsXfers(atoi(e.what())))
		    {
			DccMap::RemXfers(atoi(e.what()));
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (exception &e)
	{
	    LOG(LM_CRITICAL, "EXCEPTIONS/UNHANDLED", ( e.what()));
	}
	catch (...)
	{
	    NLOG(LM_CRITICAL, "EXCEPTIONS/UNKNOWN");
	}

	COM(("Completed Events Cycle"));
	FLUSH(); // Force TRACE output dump
	ACE_OS::sleep(1);
    }
    Parent->hh.RemoveThread();
    DRET(0);
}

void EventTask::do_expire(mDateTime &synctime)
{
    CP(("Starting EXPIRATION check ..."));

    static_cast<void>(synctime);
    NickServ::stored_t::iterator nsi;
    ChanServ::stored_t::iterator csi;
    MemoServ::channel_t::iterator ni;
    MemoServ::channel_news_t::iterator lni;
    unsigned int i;

    // akills
    //try
    {
	vector<mstring> expired_akills;
	MLOCK(("OperServ","Akill"));
	for (Parent->operserv.Akill = Parent->operserv.Akill_begin();
	    Parent->operserv.Akill != Parent->operserv.Akill_end();
	    Parent->operserv.Akill++)
	{
	    if (Parent->operserv.Akill->Last_Modify_Time().SecondsSince() >
		    Parent->operserv.Akill->Value().first)
	    {
		expired_akills.push_back(Parent->operserv.Akill->Entry());
	    }
	}
	// OK, ugly, but it avoids SET re-ordering...
	for (i=0; i<expired_akills.size(); i++)
	{
	    for (Parent->operserv.Akill = Parent->operserv.Akill_begin();
		Parent->operserv.Akill != Parent->operserv.Akill_end();
		Parent->operserv.Akill++)
		if (Parent->operserv.Akill->Entry() == expired_akills[i])
	    break;

	    if (Parent->operserv.Akill != Parent->operserv.Akill_end())
	    {
		Parent->server.RAKILL(Parent->operserv.Akill->Entry());
		LOG(LM_INFO, "EVENT/EXPIRE_AKILL",
			(Parent->operserv.Akill->Entry(),
			Parent->operserv.Akill->Value().second,
			Parent->operserv.Akill->Last_Modifier(),
			ToHumanTime(Parent->operserv.Akill->Value().first)));
		ANNOUNCE(Parent->operserv.FirstName(),
			"MISC/EXPIRE_AKILL", (
			Parent->operserv.Akill->Entry(),
			Parent->operserv.Akill->Value().second,
			Parent->operserv.Akill->Last_Modifier(),
			ToHumanTime(Parent->operserv.Akill->Value().first)));
		Parent->operserv.Akill_erase();
	    }
	}
    }
    //catch(...)
    //{
    //}

    // nicknames
    try
    {
	vector<pair<mstring,mstring> > expired_nicks;
	{ RLOCK2(("NickServ", "stored"));
	for (nsi = Parent->nickserv.StoredBegin();
		nsi != Parent->nickserv.StoredEnd(); nsi++)
	{
	    RLOCK3(("NickServ", "stored", nsi->first));
	    if (!(nsi->second.NoExpire() || nsi->second.Forbidden() ||
		nsi->second.Suspended()))
	    {
		if (nsi->second.Host().empty())
		{
		    if (nsi->second.LastAllSeenTime().SecondsSince() >
			Parent->nickserv.Expire())
		    {
			expired_nicks.push_back(pair<mstring,mstring>(nsi->second.Name(), nsi->second.Name()));
		    }
		}
		else
		{
		    if (nsi->second.LastSeenTime().SecondsSince() >
			Parent->nickserv.Expire())
		    {
			expired_nicks.push_back(pair<mstring,mstring>(nsi->second.Name(), nsi->second.Host()));
		    }
		}
	    }
	}}
	for (i=0; i<expired_nicks.size(); i++)
	{
	    if (Parent->nickserv.IsStored(expired_nicks[i].first))
	    {
		Parent->nickserv.GetStored(expired_nicks[i].first).Drop();
		Parent->nickserv.RemStored(expired_nicks[i].first);
		LOG(LM_INFO, "EVENT/EXPIRE_NICK", (expired_nicks[i].first,
			    expired_nicks[i].second));
	    }
	}
    }
    catch (E_NickServ_Stored &e)
    {
	switch(e.where())
	{
	    case E_NickServ_Stored::W_Get:
		switch (e.type())
		{
		case E_NickServ_Stored::T_Invalid:
		case E_NickServ_Stored::T_Blank:
		    if (strlen(e.what()))
		    {
			Parent->nickserv.RemStored(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }

    // channels
    try
    {
	vector<pair<mstring, mstring> > expired_chans;
	{ RLOCK2(("ChanServ", "stored"));
	for (csi = Parent->chanserv.StoredBegin();
		csi != Parent->chanserv.StoredEnd(); csi++)
	{
	    RLOCK3(("ChanServ", "stored", csi->first));
	    if (!(csi->second.NoExpire() || csi->second.Forbidden() ||
		csi->second.Suspended()))
	    {
		if (csi->second.LastUsed().SecondsSince() >
		    Parent->chanserv.Expire())
		    expired_chans.push_back(pair<mstring,mstring>(csi->second.Name(), csi->second.Founder()));
	    }
	}}
	for (i=0; i<expired_chans.size(); i++)
	{
	    if (Parent->chanserv.IsStored(expired_chans[i].first))
	    {
		Parent->chanserv.RemStored(expired_chans[i].first);
		LOG(LM_INFO, "EVENT/EXPIRE_CHAN", (expired_chans[i].first, expired_chans[i].second));
	    }
	}
    }
    catch (E_ChanServ_Stored &e)
    {
	switch(e.where())
	{
	    case E_ChanServ_Stored::W_Get:
		switch (e.type())
		{
		case E_ChanServ_Stored::T_Invalid:
		case E_ChanServ_Stored::T_Blank:
		    if (strlen(e.what()))
		    {
			Parent->chanserv.RemStored(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }

    // news articles
    try
    {
	map<mstring, vector<size_t> > expired_news;
	map<mstring, vector<size_t> >::iterator iter;
	{ RLOCK2(("MemoServ", "channel"));
	for (ni=Parent->memoserv.ChannelBegin();
		ni!=Parent->memoserv.ChannelEnd(); ni++)
	{
	    size_t cnt = 0;
	    RLOCK3(("MemoServ", "channel", ni->first));
	    for (lni=ni->second.begin(), i=0; lni != ni->second.end(); lni++, i++)
	    {
		if (!lni->NoExpire() && lni->Time().SecondsSince() >
		    Parent->memoserv.News_Expire())
		{
		    LOG(LM_DEBUG, "EVENT/EXPIRE_NEWS", (lni->Channel()));
		    expired_news[ni->first].push_back(i);
		    cnt++;
		}
	    }
	    // If we see an entry in the map, but its empty, it will mean
	    // ALL news articles are gone ... an efficiancy thing.
	    if (cnt == ni->second.size())
		expired_news[ni->first].clear();
	}}
	for (iter=expired_news.begin(); iter!=expired_news.end(); iter++)
	{
	    if (iter->second.size())
	    {
		size_t adjust = 0;
		for (i=0; i < iter->second.size(); i++)
		{
		    Parent->memoserv.RemChannelNews(iter->first, iter->second[i] - adjust);
		    adjust++;
		}
	    }
	    else
	    {
		Parent->memoserv.RemChannel(iter->first);
	    }
	}
    }
    catch (E_MemoServ_Channel &e)
    {
	e.what();
    }
}

void EventTask::do_check(mDateTime &synctime)
{
    CP(("Starting CHECK cycle ..."));

    NickServ::live_t::iterator nli;
    NickServ::recovered_t::iterator di;
    ChanServ::live_t::iterator cli;
    unsigned int i;
    vector<mstring> chunked;

    if (Parent->nickserv.IsLive(Parent->chanserv.FirstName()))
    { RLOCK2(("ChanServ", "live"));
    for (cli=Parent->chanserv.LiveBegin();
	    cli!=Parent->chanserv.LiveEnd(); cli++)
    {
	RLOCK3(("ChanServ", "live", cli->first));
	bool found = false;
	unsigned long bantime = 0, parttime = 0;

	if (Parent->chanserv.IsStored(cli->first))
	{
	    bantime = Parent->chanserv.GetStored(cli->first).Bantime();
	    parttime = Parent->chanserv.GetStored(cli->first).Parttime();
	    found = true;
	}
	// Removing bans ...
	if (found && (!Parent->chanserv.LCK_Bantime() ||
	    Parent->chanserv.DEF_Bantime()))
	{
	    if (Parent->chanserv.LCK_Bantime())
		bantime = Parent->chanserv.DEF_Bantime();
	    if (bantime)
	    {
		vector<mstring> rem;
		vector<mstring>::iterator ri;
		{ RLOCK4(("ChanServ", "live", cli->first, "bans"));
		for (di=cli->second.bans.begin();
			di!=cli->second.bans.end(); di++)
		{
		    if (di->second.SecondsSince() > bantime)
		    {
			rem.push_back(di->first);
		    }
		}}
		for (ri=rem.begin(); ri!=rem.end(); ri++)
		{
		    LOG(LM_DEBUG, "EVENT/UNBAN", (*ri,
			cli->second.Name(),
			ToHumanTime(bantime)));
		    cli->second.SendMode("-b " + *ri);
		}
	    }
	}

	chunked.clear();
	if (found)
	{
	    WLOCK(("ChanServ", "live", cli->first, "recent_parts"));
	    for (di=cli->second.recent_parts.begin();
			di!=cli->second.recent_parts.end(); di++)
	    {
		if (di->second.SecondsSince() > parttime)
		    chunked.push_back(di->first);
	    }
	    for (i=0; i<chunked.size(); i++)
		cli->second.recent_parts.erase(chunked[i]);
	}
	else if (cli->second.recent_parts.size())
	{
	    WLOCK(("ChanServ", "live", cli->first, "recent_parts"));
	    cli->second.recent_parts.clear();
	}
    }}

    // Check if we should rename people who are past their
    // grace time on ident (if KillProtect is on, and they
    // are not on access list or secure is on).
    chunked.clear();
    if (synctime.SecondsSince() >= Parent->nickserv.Ident() &&
	Parent->nickserv.IsLive(Parent->nickserv.FirstName()))
    {
	{ RLOCK(("NickServ", "live"));
	for (nli = Parent->nickserv.LiveBegin();
		    nli != Parent->nickserv.LiveEnd(); nli++)
	{
	    RLOCK2(("NickServ", "live", nli->first));
	    if (Parent->nickserv.IsStored(nli->first))
	    {
		if (!Parent->nickserv.GetStored(nli->first).IsOnline() &&
			Parent->nickserv.GetStored(nli->first).Protect() &&
			!nli->second.IsServices() && nli->second.Squit().empty() &&
			nli->second.MySignonTime().SecondsSince() >=
				    Parent->nickserv.Ident())
		{
		    chunked.push_back(nli->second.Name());
		}
	    }
	}}
	for (i=0; i<chunked.size(); i++)
	{
	    if (!Parent->nickserv.IsLive(chunked[i]) ||
		!Parent->nickserv.IsStored(chunked[i]))
		continue;

	    mstring newnick = Parent->nickserv.findnextnick(chunked[i]);
	    LOG(LM_INFO, "EVENT/KILLPROTECT", (Parent->nickserv.GetLive(chunked[i]).Mask(Nick_Live_t::N_U_P_H)));
	    if (!newnick.empty() && !Parent->server.proto.SVSNICK().empty())
	    {
		if (Parent->nickserv.GetStored(chunked[i]).Forbidden())
		    NSEND(Parent->nickserv.FirstName(), chunked[i], "MISC/RENAMED_FORBID");
		else
		    NSEND(Parent->nickserv.FirstName(), chunked[i], "MISC/RENAMED_IDENT");
		Parent->server.SVSNICK(Parent->nickserv.FirstName(),
			chunked[i], newnick);
	    }
	    else
	    {
		if (Parent->nickserv.GetStored(chunked[i]).Forbidden())
		    Parent->server.KILL(Parent->nickserv.FirstName(),
			chunked[i], Parent->getMessage("NS_YOU_STATUS/ISFORBIDDEN"));
		else
		    Parent->server.KILL(Parent->nickserv.FirstName(),
			chunked[i], Parent->getMessage("NS_SET/PROTECT"));
		Parent->server.NICK(chunked[i], (Parent->startup.Ownuser() ?
			    chunked[i].LowerCase() :
			    Parent->startup.Services_User()),
			    Parent->startup.Services_Host(),
			    Parent->startup.Server_Name(),
			    Parent->nickserv.Enforcer_Name());
	    }
	    Parent->nickserv.AddRecovered(chunked[i], mDateTime::CurrentDateTime());
	}
		
	// Sign off clients we've decided to take.
	chunked.clear();
	{ RLOCK(("NickServ", "recovered"));
	for (di = Parent->nickserv.RecoveredBegin();
		di != Parent->nickserv.RecoveredEnd(); di++)
	{
	    if (di->second.SecondsSince() >= Parent->nickserv.Release())
	    {
		if (Parent->nickserv.IsLive(di->first) &&
			Parent->nickserv.GetLive(di->first).IsServices())
		{
		    chunked.push_back(di->first);
		}
	    }
	}}
	for (i=0; i<chunked.size(); i++)
	{
	    Parent->nickserv.RemRecovered(chunked[i]);
	    Parent->server.QUIT(chunked[i], "RECOVER period expired");
	}
    }
}

void EventTask::do_modes(mDateTime &synctime)
{
    CP(("Starting PENDING MODES check ..."));

    static_cast<void>(synctime);
    set<mstring> cmp;
    set<mstring>::iterator iter;
    unsigned int i;
    
    { WLOCK(("Events", "cmodes_pending"));
    cmp = cmodes_pending;
    MCB(cmodes_pending.size());
    cmodes_pending.clear();
    MCE(cmodes_pending.size());
    }
    map<mstring,vector<mstring> > modelines;
    map<mstring,vector<mstring> >::iterator ml;
    for (iter=cmp.begin(); iter!=cmp.end(); iter++)
    {
	if (Parent->chanserv.IsLive(*iter))
	{
	    COM(("Looking at channel %s", iter->c_str()));
	    RLOCK(("ChanServ", "live", *iter));
	    Chan_Live_t &chan = Parent->chanserv.GetLive(*iter);
	    if (!chan.p_modes_on.empty() || !chan.p_modes_off.empty())
	    {
		unsigned int j, k;
		mstring mode;
		mstring modeparam;

		{ RLOCK2(("ChanServ", "live", *iter, "p_modes_off"));
		RLOCK3(("ChanServ", "live", *iter, "p_modes_off_params"));
		CP(("p_modes_off_size %d (%s)", chan.p_modes_off.size(), chan.p_modes_off.c_str()));
		for (i=0, j=0, k=0; i<chan.p_modes_off.size(); i++)
		{
		    COM(("i = %d (%c), j = %d, k = %d", i, chan.p_modes_off[i], j, k));
		    if (j>=Parent->server.proto.Modes())
		    {
			modelines[*iter].push_back(mode + " " + modeparam);
			mode.erase();
			modeparam.erase();
			j=0;
		    }
		    if (mode.empty())
			mode += "-";
		    mode += chan.p_modes_off[i];
		    if (chan.p_modes_off[i] != 'l' &&
			Parent->server.proto.ChanModeArg().Contains(chan.p_modes_off[i]))
		    {
			if (!modeparam.empty())
			    modeparam += " ";
			modeparam +=  chan.p_modes_off_params[k];
			j++; k++;
		    }
		}
		WLOCK2(("ChanServ", "live", *iter, "p_modes_off"));
		WLOCK3(("ChanServ", "live", *iter, "p_modes_off_params"));
		chan.p_modes_off.erase();
		chan.p_modes_off_params.clear();
		}
		{ RLOCK2(("ChanServ", "live", *iter, "p_modes_on"));
		RLOCK3(("ChanServ", "live", *iter, "p_modes_on_params"));
		if (mode.size() && chan.p_modes_on.size())
		    mode += "+";
		CP(("p_modes_on_size %d (%s)", chan.p_modes_on.size(), chan.p_modes_on.c_str()));
		for (i=0, k=0; i<chan.p_modes_on.size(); i++)
		{
		    COM(("i = %d (%c), j = %d, k = %d", i, chan.p_modes_on[i], j, k));
		    if (j>=Parent->server.proto.Modes())
		    {
			modelines[*iter].push_back(mode + " " + modeparam);
			mode.erase();
			modeparam.erase();
			j=0;
		    }
		    if (mode.empty())
			mode += "+";
		    mode += chan.p_modes_on[i];
		    if (Parent->server.proto.ChanModeArg().Contains(chan.p_modes_on[i]))
		    {
			if (!modeparam.empty())
			    modeparam += " ";
			modeparam += chan.p_modes_on_params[k];
			j++; k++;
		    }
		}
		WLOCK2(("ChanServ", "live", *iter, "p_modes_on"));
		WLOCK3(("ChanServ", "live", *iter, "p_modes_on_params"));
		chan.p_modes_on.erase();
		chan.p_modes_on_params.clear();
		}
		if (mode.size())
		    modelines[*iter].push_back(mode + " " + modeparam);
	    }
	}
    }
    for (ml=modelines.begin(); ml!=modelines.end(); ml++)
    {
	for (i=0; i<ml->second.size(); i++)
	    Parent->server.MODE(Parent->chanserv.FirstName(),
		ml->first, ml->second[i]);
    }
}

void EventTask::do_msgcheck(mDateTime &synctime)
{
    CP(("Starting EXPIRED MESSAGE check ..."));

    static_cast<void>(synctime);
    vector<mMessage *> Msgs;
    unsigned int i;
    vector<mstring> chunked;

    { MLOCK(("AllDependancies"));
    map<mMessage::type_t, map<mstring, set<unsigned long> > >::iterator j;
    for (j=mMessage::AllDependancies.begin(); j!=mMessage::AllDependancies.end(); j++)
    {
	map<mstring, set<unsigned long> >::iterator k;
	for (k=j->second.begin(); k!=j->second.end(); k++)
	{
	    set<unsigned long> Ids;
	    set<unsigned long>::iterator l;
	    for (l=k->second.begin(); l!=k->second.end(); l++)
	    {
		{ MLOCK2(("MsgIdMap"));
		map<unsigned long, mMessage *>::iterator m = mMessage::MsgIdMap.find(*l);
		if (m != mMessage::MsgIdMap.end())
		{
		    if (m->second == NULL ||
			m->second->creation().SecondsSince() > Parent->config.MSG_Seen_Time())
		    {
			Ids.insert(m->first);
			if (m->second != NULL)
			    Msgs.push_back(m->second);
			mMessage::MsgIdMap.erase(m);
		    }
		}}
	    }
	    for (l=Ids.begin(); l!=Ids.end(); l++)
		k->second.erase(*l);
	    if (!k->second.size())
		chunked.push_back(k->first);
	}
	for (i=0; i<chunked.size(); i++)
	    j->second.erase(chunked[i]);
	chunked.clear();
    }}
    vector<mMessage *>::iterator m;
    for (m=Msgs.begin(); m!=Msgs.end(); m++)
    {
	{ RLOCK(("IrcSvcHandler"));
	if (Parent->ircsvchandler != NULL)
	{
	    COM(("(%d) Requing without filled dependancies\n", (*m)->msgid()));
	    (*m)->priority(static_cast<u_long>(P_DepFilled));
	    Parent->ircsvchandler->enqueue(*m);
	}
	else
	{
	    COM(("(%d) Deleting obsolete message\n", (*m)->msgid()));
	    delete *m;
	}}
    }
}

void EventTask::do_ping(mDateTime &synctime)
{
    CP(("Starting SERVER PING ..."));

    static_cast<void>(synctime);
    Server::list_t::iterator si;

    double min = -1, max = 0, sum = 0, avg = 0, count = 0;
    { RLOCK2(("Server", "list"));
    for (si=Parent->server.ListBegin();
	    si!=Parent->server.ListEnd(); si++)
    {
	RLOCK3(("Server", "list", si->first));
	if (min == -1 || si->second.Lag() < min)
	    min = si->second.Lag();
	if (si->second.Lag() > max)
	    max = si->second.Lag();
	sum += si->second.Lag();
	count++;
    }}
    if (count >= 3)
	avg = (sum - min - max) / (count - 2);
    else
	avg = sum / count;

    if (avg > static_cast<double>(Parent->startup.Lagtime() * (Parent->Level() - Parent->startup.Level() + 1)))
    {
	Parent->LevelUp();
	LOG(LM_WARNING, "EVENT/LEVEL_UP", (fmstring("%.3f", avg)));
    }
    else if (Parent->Level() > Parent->startup.Level() &&
	avg <= static_cast<double>(Parent->startup.Lagtime() * (Parent->Level() - Parent->startup.Level())))
    {
	Parent->LevelDown();
	LOG(LM_WARNING, "EVENT/LEVEL_DOWN", (fmstring("%.3f", avg)));
    }

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() <= 3)
    {
	RLOCK2(("Server", "list"));
	for (si = Parent->server.ListBegin();
		si != Parent->server.ListEnd(); si++)
	{
	    RLOCK3(("Server", "list", si->first));
	    si->second.Ping();
	}
	NLOG(LM_TRACE, "EVENT/PING");
    }}
}

void EventTask::DumpB() const
{
    MB(0, (last_expire, last_save, last_check, last_ping));
}

void EventTask::DumpE() const
{
    ME(0, (last_expire, last_save, last_check, last_ping));
}

