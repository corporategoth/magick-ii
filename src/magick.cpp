#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#pragma implementation "language.h"
#pragma implementation "logfile.h"
#pragma implementation "crypt.h"
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
#include "lockable.h"
#include "utils.h"
#include "convert.h"

//#define ACE_DEBUGGING

mDateTime StartTime;
Magick *Parent;

mstring Magick::files_t::MakePath(mstring in)
{
#ifdef WIN32
	if (in[1u] == ':' && mstring(in[2u]) == DirSlash)
	    return in;
	else
	    return Parent->Services_Dir() + DirSlash + in;
#else
	if (mstring(in[0u]) == DirSlash)
	    return in;
	else
	    return Parent->Services_Dir() + DirSlash + in;
#endif
}

size_t Log(ACE_Log_Priority priority, const char *message, ...)
{
    FT("Log", ((unsigned long) priority, message));

    va_list argptr;
    va_start(argptr, message);
    size_t retval = LogV(priority, message, argptr);
    va_end(argptr);
    RET(retval);
}

size_t LogV(ACE_Log_Priority priority, const char *message, va_list argptr)
{
    FT("LogV", ((unsigned long) priority, "(va_list) argptr"));

    mstring text_priority, text;

    switch (priority)
    {

    // Stuff normal users dont wanna see
    case LM_TRACE:
	text_priority = "TRACE   ";
#ifndef DEBUG
	RET(0);
#endif
	break;

    // Stuff you have to turn VERBOSE on to see
    case LM_DEBUG:
	text_priority = "DEBUG   ";
	if (!Parent->verbose())
	    RET(0);
	break;

    // Normal information that most users want
    case LM_INFO:
	text_priority = "INFO    ";
	break;

    // Still normal, but notable (eg. SOP commands)
    case LM_NOTICE:
	text_priority = "NOTICE  ";
	break;

    // An attempt at data corruption (denied) or recoverable error
    case LM_WARNING:
	text_priority = "WARNING ";
	break;

    // Startup messages only (ie. boot)
    case LM_STARTUP:
	text_priority = "STARTUP ";
	break;

    // A data corruption that we repaird
    case LM_ERROR:
	text_priority = "ERROR   ";
	break;

    // A data corruption we could not repair (left in system or
    // more drastic action taken, eg. the record being removed)
    case LM_CRITICAL:
	text_priority = "CRITICAL";
	break;

    // A situation that caused us to hard abort, but did not kill
    // us but may have caused a thread to die, etc.
    case LM_ALERT:
	text_priority = "ALERT   ";
	break;

    // A situation we could not reover from, we're outtahere.
    case LM_EMERGENCY:
	text_priority = "FATAL   ";
	break;

    // Should NEVER get this, but its there for completeness.
    default:
	text_priority = "UNKNOWN ";
	break;
    }

    // Blatently copied from ACE_DEBUG code...
    int __ace_error = ACE_OS::last_error ();
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance ();
    ace___->set (ASYS_TEXT (__FILE__), __LINE__, 0, __ace_error,
	ace___->restart (), ace___->msg_ostream (), ace___->msg_callback ());

    text.FormatV(message, argptr);
    size_t retval = ace___->log(priority, "%s | %s | %s\n",
	Now().FormatString("dd mmm yyyy hh:nn:ss").c_str(),
	text_priority.c_str(), text.c_str());

    if (priority == LM_EMERGENCY)
	exit(-1);
    RET(retval);
}

Magick::Magick(int inargc, char **inargv)
{
    Parent=this;
    FT("Magick::Magick", (inargc, "(char **) inargv"));
    i_shutdown = false;
    char buf[1024];
    ACE_OS::getcwd(buf, 1024);
    i_services_dir=buf;
    i_config_file="magick.ini";
    for(int i=0;i<inargc;i++)
	argv.push_back(inargv[i]);
    i_programname=argv[0].RevAfter("/");

    ircsvchandler = NULL;
    i_level=0;
    i_reconnect = true;
    i_gotconnect = false;
    i_connected = false;
    i_saving = false;
    i_auto = false;
    i_verbose = false;

    ACE_LOG_MSG->open(i_programname.c_str());
}

Magick::~Magick()
{
}

int Magick::Start()
{
    unsigned int i;
    int Result;
    // this is our main routine, when it leaves here, this sucker's done.
#ifdef ACE_DEBUGGING
    ACE_Log_Msg::instance()->open(PACKAGE);
    ACE_Log_Msg::enable_debug_messages();
#endif

    // We log to STDERR until we find our logfile...
    LoadLogMessages("DEFAULT");

    // more stuff to do
    unsigned int argc=argv.size();
    mstring errstring;
    for(i=1;i<argc;i++)
    {
	if(argv[i][0U]=='-')
	{
	    argv[i].MakeLower();
	    if(argv[i]=="--dir")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),"--dir");
		}
		i_services_dir=files.MakePath(argv[i]);
	    }
	    else if(argv[i]=="--config")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),"--config");
		}
		i_config_file=argv[i];
	    }
#ifdef MAGICK_TRACE_WORKS
	    else if(argv[i]=="--trace")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),"--trace");
		}
		if(!argv[i].Contains(":"))
		{
		    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/TRACE_SYNTAX"));
		}
		unsigned short level = makehex(argv[i].After(":"));
		if (level==0)
		{
		    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ZERO_LEVEL"));
		}
		else
		{
		    mstring type=argv[i].Before(":").UpperCase();
		    if (type=="MAIN" || type=="ALL")
			Trace::TurnSet(tt_MAIN, level);
		    for (int i=tt_MAIN+1; i<tt_MAX; i++)
			if (type==threadname[i] || type=="ALL")
			    Trace::TurnSet((threadtype_enum) i, level);
		}
	    }
#endif
	    else if(argv[i]=="--help" ||
		(argv[i][1U]!='-' && argv[i].Contains("?")))
    	    {
		dump_help();
		return MAGICK_RET_NORMAL;
	    }
	}
    }
    NFT("Magick::Start");
    if (ACE_OS::chdir (Services_Dir()) < 0)
    {
        perror (Services_Dir());
#ifdef WIN32
        WSACleanup ();
#endif
        RET(MAGICK_RET_INVALID_SERVICES_DIR);
    }

    if (!get_config_values())
    {
	Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NO_CFG_FILE"), i_config_file.c_str());
    }

    if(i_shutdown==true)
	Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/STOP"));

    Result=doparamparse();
    if(Result!=MAGICK_RET_NORMAL)
	RET(Result);

    mFile pidfile;
    if (mFile::Exists(files.Pidfile()))
    {
	pidfile.Open(files.Pidfile(),"r");
	if (pidfile.IsOpened())
	{
	    mstring dummystring = pidfile.ReadLine();
	    pid_t pid = atoi(dummystring.c_str());
	    if (ACE::process_active(pid) > 0)
	    {
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ACTIVE"),
			i_programname.c_str());
	    }
	    pidfile.Close();
	    mFile::Erase(files.Pidfile());
	}
    }

    // Re-direct log output to this file (log output is to STDERR)
    freopen(files.Logfile().c_str(), "a", stderr);

    FLUSH();
    // Need to shut down, it wont be carried over fork.
    // We will re-start it ASAP after fork.
    Log(LM_STARTUP, getLogMessage("COMMANDLINE/START_FORK"));
    Result = ACE::fork(i_programname);
    if (Result < 0)
    {
	Log(LM_EMERGENCY, getLogMessage("SYS_ERRORS/FAILED_FORK"), Result);
	RET(1);
    }
    else if (Result != 0)
    {
	RET(0);
    }
/*    Result = ACE_OS::setpgid (0, 0);
    if (Result < 0)
    {
	Log(LM_EMERGENCY, getLogMessage("SYS_ERRORS/FAILED_SETPGID"), Result);
	RET(1);
    } */

    pidfile.Open(files.Pidfile(),"w");
    if(pidfile.IsOpened())
    {
	mstring dummystring;
	dummystring<<getpid();
	pidfile.Write(dummystring);
	pidfile.Close();
    }

    // load the local messages database and internal "default messages"
    // the external messages are part of a separate ini called english.lng (both local and global can be done here too)
    Log(LM_STARTUP, getLogMessage("COMMANDLINE/START_LANG"));
    LoadInternalMessages();

    load_databases();

    // Can only open these after fork if we want then to live
    Log(LM_STARTUP, getLogMessage("COMMANDLINE/START_EVENTS"));
    { WLOCK(("Events"));
    events = new EventTask;
    events->open();
    }
    { WLOCK(("DccMap"));
    dcc = new DccMap;
    dcc->open();
    }

    // okay here we start setting up the ACE_Reactor and ACE_Event_Handler's
    signalhandler=new SignalHandler;
    ACE_Reactor::instance()->register_handler(SIGINT,signalhandler);
#if defined(SIGTERM) && (SIGTERM != 0)
    ACE_Reactor::instance()->register_handler(SIGTERM,signalhandler);
#endif
#if defined(SIGPIPE) && (SIGPIPE != 0)
    ACE_Reactor::instance()->register_handler(SIGPIPE,signalhandler);
#endif
#if defined(SIGQUIT) && (SIGQUIT != 0)
    ACE_Reactor::instance()->register_handler(SIGQUIT,signalhandler);
#endif
    ACE_Reactor::instance()->register_handler(SIGSEGV,signalhandler);
#ifdef SIGBUS
    ACE_Reactor::instance()->register_handler(SIGBUS,signalhandler);
#endif
#ifdef SIGHUP
    ACE_Reactor::instance()->register_handler(SIGHUP,signalhandler);
#endif
    ACE_Reactor::instance()->register_handler(SIGILL,signalhandler);
#ifdef SIGTRAP
    ACE_Reactor::instance()->register_handler(SIGTRAP,signalhandler);
#endif
#ifdef SIGIOT
    ACE_Reactor::instance()->register_handler(SIGIOT,signalhandler);
#endif
#ifdef SIGFPE
    ACE_Reactor::instance()->register_handler(SIGFPE,signalhandler);
#endif
#if 0 // Posix threads use these grrr
#if defined(SIGUSR1) && (SIGUSR1 != 0)
    ACE_Reactor::instance()->register_handler(SIGUSR1,signalhandler);
#endif
#if defined(SIGUSR2) && (SIGUSR2 != 0)
    ACE_Reactor::instance()->register_handler(SIGUSR2,signalhandler);
#endif
#endif
#if defined(SIGALRM) && (SIGALRM != 0)
    ACE_Reactor::instance()->register_handler(SIGALRM,signalhandler);
#endif
#ifdef SIGCHLD
    ACE_Reactor::instance()->register_handler(SIGCHLD,signalhandler);
#endif
#ifdef SIGWINCH
    ACE_Reactor::instance()->register_handler(SIGWINCH,signalhandler);
#endif
#ifdef SIGTTIN
    ACE_Reactor::instance()->register_handler(SIGTTIN,signalhandler);
#endif
#ifdef SIGTTOU
    ACE_Reactor::instance()->register_handler(SIGTTOU,signalhandler);
#endif
#ifdef SIGTSTP
    ACE_Reactor::instance()->register_handler(SIGTSTP,signalhandler);
#endif

    mBase::init();

/* Please leave this somewhere in this file, as it
 * is quite handy when you are getting a segfault
 * after load but before you can turn on trace.
 *
#ifdef MAGICK_TRACE_WORKS
    Trace::TurnSet(tt_MAIN, 0xffff);
    for (int i=tt_MAIN+1; i<tt_MAX; i++)
	Trace::TurnSet((threadtype_enum) i, 0xffff);
#endif
*/
    // etc.

    //Log(LM_STARTUP, getLogMessage("COMMANDLINE/START_CALIBRATE"));
    // calibrate the threshholds.
    //
    // register 250 nicks and 1000 channels (to random nicknames in the nick pool).
    // and add access rand() % 20 random nicknames to the access list of each channel.
    // then go through and delete each nick (it will remove access list and channel entries)
    // This should go on for 60s. First and last 5 seconds are disgarded.
    // 
    // number of iterations/500 is low_water_mark, number of itereations/200 = high_water_mark
    // TODO: how to work out max_thread_pool for all of magick?

    { WLOCK(("i_ResetTime"));
    i_ResetTime=Now();
    }

    // Use the reconnect handler to get a connection

    ACE_Reactor::instance()->schedule_timer(&(Parent->rh),0,ACE_Time_Value::zero);
    AUTO(true); // Activate events from here.

    // next thing to be done here is set up the acceptor mechanism to listen
    // for incoming "magickgui" connections and handle them.

    // This is the main loop.  When we get a Shutdown(),
    // we wait for everything to shutdown cleanly.
    // All that will be left is US and the LOGGER.
    Log(LM_STARTUP, getLogMessage("COMMANDLINE/START_COMPLETE"),
		PACKAGE, VERSION);
    while(!Shutdown())
    {
	ACE_Reactor::instance()->run_event_loop();
	FLUSH();
    }

    Log(LM_STARTUP, getLogMessage("COMMANDLINE/STOP_EVENTS"));
    // We're going down .. execute relivant shutdowns.
    { WLOCK(("IrcSvcHandler"));
    if (ircsvchandler != NULL)
    {
  	ircsvchandler->shutdown();
	delete ircsvchandler;
	ircsvchandler = NULL;
    }}
    mBase::shutdown();
    // Main Thread, DCC and Events engine
    while (mThread::size() > 3)
    {
	ACE_OS::sleep(1);
    }

    //todo work out some way to "ignore" signals
    ACE_Reactor::instance()->remove_handler(SIGINT);
#if defined(SIGTERM) && (SIGTERM != 0)
    ACE_Reactor::instance()->remove_handler(SIGTERM);
#endif
#if defined(SIGPIPE) && (SIGPIPE != 0)
    ACE_Reactor::instance()->remove_handler(SIGPIPE);
#endif
#if defined(SIGQUIT) && (SIGQUIT != 0)
    ACE_Reactor::instance()->remove_handler(SIGQUIT);
#endif
    ACE_Reactor::instance()->remove_handler(SIGSEGV);
#ifdef SIGBUS
    ACE_Reactor::instance()->remove_handler(SIGBUS);
#endif
#ifdef SIGHUP
    ACE_Reactor::instance()->remove_handler(SIGHUP);
#endif
    ACE_Reactor::instance()->remove_handler(SIGILL);
#ifdef SIGTRAP
    ACE_Reactor::instance()->remove_handler(SIGTRAP);
#endif
#ifdef SIGIOT
    ACE_Reactor::instance()->remove_handler(SIGIOT);
#endif
#ifdef SIGFPE
    ACE_Reactor::instance()->remove_handler(SIGFPE);
#endif
#if 0 // Posix threads use these, grr
#if defined(SIGUSR1) && (SIGUSR1 != 0)
    ACE_Reactor::instance()->remove_handler(SIGUSR1);
#endif
#if defined(SIGUSR2) && (SIGUSR2 != 0)
    ACE_Reactor::instance()->remove_handler(SIGUSR2);
#endif
#endif
#if defined(SIGALRM) && (SIGALRM != 0)
    ACE_Reactor::instance()->remove_handler(SIGALRM);
#endif
#ifdef SIGCHLD
    ACE_Reactor::instance()->remove_handler(SIGCHLD);
#endif
#ifdef SIGWINCH
    ACE_Reactor::instance()->remove_handler(SIGWINCH);
#endif
#ifdef SIGTTIN
    ACE_Reactor::instance()->remove_handler(SIGTTIN);
#endif
#ifdef SIGTTOU
    ACE_Reactor::instance()->remove_handler(SIGTTOU);
#endif
#ifdef SIGTSTP
    ACE_Reactor::instance()->remove_handler(SIGTSTP);
#endif

    { WLOCK(("Events"));
    if (events != NULL)
    {
	events->close(0);
	delete events;
    }}

    { WLOCK(("DccMap"));
    if (dcc != NULL)
    {
	dcc->close(0);
	delete dcc;
    }}

    delete signalhandler;

    mFile::Erase(files.Pidfile());

    Log(LM_STARTUP, getLogMessage("COMMANDLINE/STOP_COMPLETE"),
		PACKAGE, VERSION);
    RET(MAGICK_RET_TERMINATE);
}

mstring Magick::getMessage(const mstring & nick, const mstring & name)
{
    FT("Magick::getMessage", (nick, name));

    if (nickserv.IsStored(nick) &&
	nickserv.stored[nick.LowerCase()].IsOnline())
    {
	CP(("Using USER-DEIFNED language."));
	mstring retval = getMessageL(nickserv.stored[nick.LowerCase()].Language(), name);
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
    if (lang != "" &&
	Messages.find(lang.UpperCase()) == Messages.end())
    {
	LoadExternalMessages(lang);
    }
    { RLOCK(("Messages", lang.UpperCase(), name.UpperCase()));
    if (lang != "" &&
	Messages.find(lang.UpperCase()) != Messages.end() &&
	Messages[lang.UpperCase()].find(name.UpperCase()) !=
		Messages[lang.UpperCase()].end())
    {
	retval = Messages[lang.UpperCase()][name.UpperCase()];
	RET(retval);
    }}

    // Load nickserv default language if its NOT loaded.
    // and then look for the message of THAT type.
    CP(("Trying DEFAULT language ..."));
    if (lang.UpperCase() != nickserv.DEF_Language().UpperCase() &&
	nickserv.DEF_Language() != "" &&
	Messages.find(nickserv.DEF_Language().UpperCase()) ==
	Messages.end())
    {
	LoadExternalMessages(nickserv.DEF_Language());
    }
    { RLOCK(("Messages", nickserv.DEF_Language().UpperCase(), name.UpperCase()));
    if (lang.UpperCase() != nickserv.DEF_Language().UpperCase() &&
	nickserv.DEF_Language() != "" &&
	Messages.find(nickserv.DEF_Language().UpperCase()) !=
	Messages.end() &&
	Messages[nickserv.DEF_Language().UpperCase()].find(name.UpperCase()) !=
	Messages[nickserv.DEF_Language().UpperCase()].end())
    {
	retval = Messages[nickserv.DEF_Language().UpperCase()][name.UpperCase()];
	RET(retval);
    }}

    // Otherwise just try and find it in the DEFAULTs.
    CP(("Trying HARD-CODED language ..."));
    { RLOCK(("Messages", "DEFAULT", name.UpperCase()));
    if (Messages["DEFAULT"].find(name.UpperCase()) !=
		Messages["DEFAULT"].end())
    {
	retval = Messages["DEFAULT"][name.UpperCase()];
    }}
    RET(retval);
}

mstring Magick::getLogMessage(const mstring & name)
{
    FT("Magick::getLogMessage", (name));

    // Load nickserv default language if its NOT loaded.
    // and then look for the message of THAT type.
    // Otherwise just try and find it in the DEFAULTs.
    // NEVER implement locking here, we log the locks.

    mstring retval = "Could not find log message token \"" + name.UpperCase() +
			    "\", please check your language file.";
    CP(("Trying to get log entry ..."));
    { RLOCK(("LogMessages", name.UpperCase()));
    if (LogMessages.find(name.UpperCase()) !=
		LogMessages.end())
    {
	retval = LogMessages[name.UpperCase()];
    }}
    RET(retval);
}

vector<mstring> Magick::getHelp(const mstring & nick, const mstring & name)
{
    FT("Magick::getHelp", (nick, name));

    vector<mstring> helptext;
    mstring language = nickserv.DEF_Language().UpperCase();

    // Load requested language if its NOT loaded.
    // and then look for the Help of THAT type.
    if (nick != "" && nickserv.IsStored(nick) &&
	nickserv.stored[nick.LowerCase()].IsOnline())
    {
	language = nickserv.stored[nick.LowerCase()].Language().UpperCase();
    }
    WLOCK(("Magick","LoadHelp"));

StartGetLang:
    if (Help.find(language) == Help.end() &&
	mFile::Exists(files.Langdir()+DirSlash+language.LowerCase()+".hlp"))
    {
	WLOCK(("Help", language.UpperCase()));

	mConfigEngine fconf(files.Langdir()+DirSlash+language.LowerCase()+".hlp");
	map<mstring,mstring> tmp = fconf.GetMap();
	map<mstring,mstring>::iterator i;

	mstring section;
	triplet<mstring,mstring,mstring> entry;
	for (i=tmp.begin(); i!=tmp.end(); i++)
	{
	    section = i->first.RevBefore("/");
	    entry = triplet<mstring, mstring, mstring>(
			i->second.ExtractWord(1, ":", false),
			i->second.ExtractWord(2, ":", false),
			i->second.After(":", 2));
	    
	    if (entry.third == "")
		entry.third = " ";

	    Help[language][section].push_back(entry);
	}
	if (tmp.size())
	{
	    Log(LM_INFO, getLogMessage("OTHER/LOAD_HELP"),
		language.UpperCase().c_str());
	    CP(("Help %s was loaded into memory.", language.c_str()));
	}
    }

    unsigned int i, j;
    { RLOCK(("Help", language.UpperCase(), name.UpperCase()));
    if (Help.find(language) != Help.end() &&
	Help[language].find(name.UpperCase()) != Help[language].end())
    {
	bool sendline;

	for (j=0; j<Help[language][name.UpperCase()].size(); j++)
	{
	    sendline = false;
	    if (Help[language][name.UpperCase()][j].first != "")
	    {
		for (i=1; !sendline && i<=Help[language][name.UpperCase()][j].first.WordCount(" "); i++)
		{
		    if (commserv.IsList(Help[language][name.UpperCase()][j].first.ExtractWord(i, " ")) &&
			commserv.list[Help[language][name.UpperCase()][j].first.ExtractWord(i, " ").UpperCase()].IsOn(nick))
			sendline = true;
		}
	    }
	    else
		sendline = true;
	    if (Help[language][name.UpperCase()][j].second != "")
		for (i=1; sendline && i<=Help[language][name.UpperCase()][j].second.WordCount(" "); i++)
		{
		    if (commserv.IsList(Help[language][name.UpperCase()][j].second.ExtractWord(i, " ")) &&
			commserv.list[Help[language][name.UpperCase()][j].second.ExtractWord(i, " ").UpperCase()].IsOn(nick))
			sendline = false;
		}
	    if (sendline)
		helptext.push_back(Help[language][name.UpperCase()][j].third);
	}
    }}

    if (!helptext.size())
    {
	if (language != nickserv.DEF_Language().UpperCase())
	{
	    language = nickserv.DEF_Language().UpperCase();
	    goto StartGetLang;
	}
	mstring tmpstr, tmpstr2;
	tmpstr2 = name;
	tmpstr2.Replace(mstring("/"), mstring(" "));
	tmpstr.Format(getMessage(nick, "ERR_SITUATION/NOHELP"),
	    tmpstr2.After(" ").c_str());
	helptext.push_back(tmpstr);
    }
    NRET(vector<mstring>, helptext);
}

void Magick::dump_help()
{
    // This needs to be re-written.
    cout << "\n"
	 << FULLNAME + " - " + HOMEPAGE + "\n"
	 << "    (c) 1997-2000 Preston A. Elder <prez@magick.tm>\n"
	 << "    (c) 1998-2000 William King <ungod@magick.tm>\n"
	 << "\n"
	 << "Syntax: " << i_programname << " [options]\n"
	 << "\n"
	 << "--help             -?     	Help output (summary of the below).\n"
	 << "--dir X                    Set the initial services directory.\n"
	 << "--config X                 Set the name of the config file.\n"
	 << "--convert X                Convert another version of services databases\n"
	 << "                           to Magick II format, where X is the type of\n"
	 << "                           database to convert.  Currently recognized:\n"
	 << "                               magick\n"
	 << "--trace X:Y                Set the trace level on startup, equivilant of\n"
	 << "                           using the OperServ TRACE SET command while\n"
	 << "                           running, where X is the trace type (or ALL),\n"
	 << "                           and Y is the trace level in hex.\n"
	 << "--name X           -n      Override [STARTUP/SERVER_NAME] to X.\n"
	 << "--desc X           -d      Override [STARTUP/SERVER_DESC] to X.\n"
	 << "--user X           -u      Override [STARTUP/SERVICES_USER] to X.\n"
	 << "--host X           -h      Override [STARTUP/SERVICES_HOST] to X.\n"
	 << "--ownuser          -o      Override [STARTUP/OWNUSER] to true.\n"
	 << "--protocol X       -P      Override [STARTUP/PROTOCOL] to X.\n"
	 << "--level X          -l      Override [STARTUP/LEVEL] to X.\n"
	 << "--lagtime X        -g      Override [STARTUP/LAGTIME] to X.\n"
	 << "--verbose          -v      Override [FILES/VERBOSE] to X.\n"
	 << "--log X            -L      Override [FILES/LOGFILE] to X.\n"
	 << "--dbase X          -D      Override [FILES/DATABASE] to X.\n"
	 << "--langdir X        -S      Override [FILES/LANGDIR] to X.\n"
	 << "--encrypt          -E      Override [FILES/ENCRYPTION] to true.\n"
	 << "--decrypt          -e      Override [FILES/ENCRYPTION] to false.\n"
	 << "--keyfile X        -K      Override [FILES/KEYFILE] to X.\n"
	 << "--compress X       -c      Override [FILES/COMPRESSION] to X.\n"
	 << "--relink X         -r      Override [CONFIG/SERVER_RELINK] to X.\n"
	 << "--cycle X          -t      Override [CONFIG/CYCLETIME] to X.\n"
	 << "--save X           -w      Override [CONFIG/SAVETIME] to X.\n"
	 << "--check X          -T      Override [CONFIG/CHECKTIME] to X.\n"
	 << "--ping X           -p      Override [CONFIG/PING_FREQUENCY] to X.\n"
	 << "--lwm X            -m      Override [CONFIG/LOW_WATER_MARK] to X.\n"
	 << "--hwm X            -M      Override [CONFIG/HIGH_WATER_MARK] to X.\n"
	 << "--append           -a      Override [NICKSERV/APPEND_RENAME] to true.\n"
	 << "--rename           -A      Override [NICKSERV/APPEND_RENAME] to false.\n"
	 << "--ident X          -R      Override [NICKSERV/IDENT] to X.\n"
	 << "--language X       -s      Override [NICKSERV/DEF_LANGUAGE] to X.\n"
	 << "--nodcc            -x      Override [NICKSERV/PICEXT] to \"\" and\n"
	 << "                           Override [MEMOSERV/FILES] to 0.\n"
	 << "--inflight X       -f      Override [MEMOSERV/INFLIGHT] to X.\n"
	 << "--logignore        -i      Override [OPERSERV/LOG_IGNORE] to true.\n"
	 << "--ignore X         -I      Override [OPERSERV/IGNORE_METHOD] to X.\n"
	 << "\n"
	 << "For more help on the usage of " + mstring(PACKAGE) + ", please browse the docs directory.\n"
	 << "This released under the GNU General Public License.  Please see the\n"
	 << "\"COPYING\" file for more details.\n\n";

}

void Magick::LoadInternalMessages()
{
    NFT("Magick::LoadInternalMessages");

#include "language.h"
    WLOCK(("Messages", "DEFAULT"));

    vector<mstring> lang;
    unsigned int i;
    for (i=0; i<def_langent; i++)
	lang.push_back(def_lang[i]);

    mConfigEngine fconf;
    fconf.LoadFromArray(lang);
    map<mstring,mstring> tmp = fconf.GetMap(); 
    if (tmp.size())
	Messages["DEFAULT"].insert(tmp.begin(), tmp.end());
}

bool Magick::LoadExternalMessages(mstring language)
{
    FT("Magick::LoadExternalMessages", (language));
    // use the previously created name array to get the names to load

    if (language.UpperCase() == "DEFAULT")
    {
	LoadInternalMessages();
	RET(true);
    }
    else if (mFile::Exists(files.Langdir()+DirSlash+language.LowerCase()+".lng"))
    {
	WLOCK(("Messages", language.UpperCase()));
	mConfigEngine fconf(files.Langdir()+DirSlash+language.LowerCase()+".lng");

	map<mstring,mstring> tmp = fconf.GetMap();
	if (tmp.size())
	{
	    Messages[language.UpperCase()].insert(tmp.begin(), tmp.end());
	    RET(true);
	}
    }
    RET(false);
}

bool Magick::LoadLogMessages(mstring language)
{
    FT("Magick::LoadLogMessages", (language));
    // use the previously created name array to get the names to load

    WLOCK(("LogMessages"));
    LogMessages.clear();
    {
#include "logfile.h"
	vector<mstring> log;
	unsigned int i;
	for (i=0; i<def_logent; i++)
	log.push_back(def_log[i]);

	mConfigEngine fconf;
	fconf.LoadFromArray(log);

	map<mstring,mstring> tmp = fconf.GetMap();
	LogMessages.insert(tmp.begin(), tmp.end());
    }
    if (language != "DEFAULT" &&
	mFile::Exists(files.Langdir()+DirSlash+language.LowerCase()+".lfo"))
    {
	mConfigEngine fconf(files.Langdir()+DirSlash+language.LowerCase()+".lfo");
	map<mstring,mstring> tmp = fconf.GetMap();
	LogMessages.insert(tmp.begin(), tmp.end());
    }

    if (LogMessages.size())
    {
	RET(true);
    }
    RET(false);
}

bool Magick::UnloadExternalMessages(mstring language)
{
    FT("Magick::UnloadExternalMessages", (language));

    if (language != "" &&
	Messages.find(language.UpperCase()) != Messages.end())
    {
	WLOCK(("Messages"));
	Messages.erase(language.UpperCase());
	RET(true);
    }
    RET(false);
}

bool Magick::UnloadHelp(mstring language)
{
    FT("Magick::UnloadHelp", (language));

    if (language != "" &&
	Help.find(language.UpperCase()) != Help.end())
    {
	WLOCK(("Help"));
	Help.erase(language.UpperCase());
	RET(true);
    }
    RET(false);
}

int Magick::doparamparse()
{
    NFT("Magick::doparamparse");
    mstring temp;
    int argc=argv.size(),i;
    for(i=1;i<argc;i++)
    {
	if(argv[i][0U]=='-')
	{
	    bool ArgUsed=false;
	    if (argv[i][1U]=='-')
		ArgUsed=paramlong(argv[i], (i+1 < argc) ? argv[i+1].c_str() : "");
	    else
		ArgUsed=paramshort(argv[i], (i+1 < argc) ? argv[i+1].c_str() : "");
	    
	    if (ArgUsed)
		i++;
	}
	else
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NONOPTION"));
	}
    }
    RET(MAGICK_RET_NORMAL);
}


bool Magick::paramlong(mstring first, mstring second)
{
    FT("Magick::paramlong", (first, second));
    if(first=="--dir" || first=="--config" || first=="--trace")
    {
	// already handled, but we needed to i++
	RET(true);
    }
    else if(first=="--name")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	startup.server_name=second;
	RET(true);
    }
    else if(first=="--desc")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	startup.server_name=second;
	RET(true);
    }
    else if(first=="--user")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	startup.services_user=second;
	RET(true);
    }
    else if(first=="--ownuser")
    {
	startup.ownuser=true;
    }
    else if(first=="--host")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	startup.services_host=second;
    }
    else if(first=="--protocol")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(ACE_OS::atoi(second.c_str())<=0)
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/MUSTBENUMBER"),first.c_str());
	}
	if (ACE_OS::atoi(second.c_str()) != server.proto.Number())
	{
	    server.proto.Set(ACE_OS::atoi(second.c_str()));
	    if (ACE_OS::atoi(second.c_str()) != server.proto.Number())
		Log(LM_WARNING, getLogMessage("COMMANDLINE/UNKNOWN_PROTO"),
			    ACE_OS::atoi(second.c_str()), server.proto.Number());
	}
	RET(true);
    }
    else if(first=="--level")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(ACE_OS::atoi(second.c_str())<=0)
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/MUSTBENUMBER"),first.c_str());
	}
	startup.level=ACE_OS::atoi(second.c_str());
	RET(true);
    }
    else if(first=="--lagtime")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(!FromHumanTime(second))
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/TIMEORZERO"),first.c_str());
	}
	startup.lagtime=FromHumanTime(second);
    }
    else if(first=="--verbose" || first=="--debug")
    {
	i_verbose = true;
    }
    else if(first=="--log")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	files.logfile=second;
	RET(true);
    }
    else if(first=="--dbase" || first=="--database")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	files.database=second;
	RET(true);
    }
    else if(first=="--langdir")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	files.langdir=second;
	RET(true);
    }
    else if(first=="--encrypt")
    {
	files.encryption=true;
    }
    else if(first=="--decrypt")
    {
	files.encryption=false;
    }
    else if(first=="--keyfile")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(!mFile::Exists(second))
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NO_KEYFILE"), second.c_str());
	}
	files.keyfile=second;
	RET(true);
    }
    else if(first=="--compress" || first=="--compression")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(ACE_OS::atoi(second.c_str())<0)
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/MUSTBENUMBER"),first.c_str());
	}
	if (ACE_OS::atoi(second.c_str())>9)
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/VALUETOOHIGH"),first.c_str(), 9);
	}
	files.compression=ACE_OS::atoi(second.c_str());
	RET(true);
    }
    else if(first=="--relink")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(ACE_OS::atoi(second.c_str())<0)
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/MUSTBENUMBER"),first.c_str());
	}
	config.server_relink=FromHumanTime(second);
	RET(true);
    }
    else if(first=="--norelink")
    {
	config.server_relink=0;
    }
    else if(first=="--cycle" || first=="--save" || first=="--update")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(!FromHumanTime(second))
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/TIMEORZERO"),first.c_str());
	}
	config.cycletime=FromHumanTime(second);
	RET(true);
    }
    else if(first=="--check" || first=="--hyperactive")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(!FromHumanTime(second))
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/TIMEORZERO"),first.c_str());
	}
	config.checktime=FromHumanTime(second);
	RET(true);
    }
    else if(first=="--ping")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(!FromHumanTime(second))
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/TIMEORZERO"),first.c_str());
	}
	config.ping_frequency=FromHumanTime(second);
	RET(true);
    }
    else if(first=="--lwm" || first=="--low_water_mark")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(ACE_OS::atoi(second.c_str())<0)
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/MUSTBENUMBER"),first.c_str());
	}
	config.low_water_mark=ACE_OS::atoi(second.c_str());
	if (config.high_water_mark < config.low_water_mark)
	    config.high_water_mark = config.low_water_mark;
	RET(true);
    }
    else if(first=="--hwm" || first=="--high_water_mark")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(ACE_OS::atoi(second.c_str())<0)
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/MUSTBENUMBER"),first.c_str());
	}
	config.high_water_mark=ACE_OS::atoi(second.c_str());
	if (config.high_water_mark < config.low_water_mark)
	    config.high_water_mark = config.low_water_mark;
	RET(true);
    }
    else if(first=="--append")
    {
	nickserv.append_rename=true;
    }
    else if(first=="--rename")
    {
	nickserv.append_rename=false;
    }
    else if(first=="--ident")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(!FromHumanTime(second))
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/TIMEORZERO"),first.c_str());
	}
	nickserv.ident=FromHumanTime(second);
	RET(true);
    }
    else if(first=="--language")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	nickserv.def_language=second;
	RET(true);
    }
    else if(first=="--nodcc")
    {
	nickserv.picext="";
	memoserv.files=0;
    }
    else if(first=="--inflight")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(!FromHumanTime(second))
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/TIMEORZERO"),first.c_str());
	}
	nickserv.ident=FromHumanTime(second);
	RET(true);
    }
    else if(first=="--logignore")
    {
	operserv.log_ignore=true;
    }
    else if(first=="--ignore")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if(ACE_OS::atoi(second.c_str())<0)
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/MUSTBENUMBER"),first.c_str());
	}
	if (ACE_OS::atoi(second.c_str())>9)
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/VALUETOOHIGH"),first.c_str(), 9);
	}
	operserv.ignore_method=ACE_OS::atoi(second.c_str());
	RET(true);
    }
    else if(first=="--convert")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/NEEDPARAM"),first.c_str());
	}
	if (second.CmpNoCase("magick")==0)
	{
	    load_ns_dbase();
	    load_cs_dbase();
	    load_ms_dbase();
	    load_news_dbase();
	    load_akill();
	    load_clone();
	    load_sop();
	    load_message();
	}
	else
	{
	    Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/CANNOT_CONVERT"), second.c_str());
	}
	RET(true);
    }
    else
    {
	Log(LM_ERROR, getLogMessage("COMMANDLINE/UNKNOWN_OPTION"),first.c_str());
    }
    RET(false);
}

bool Magick::paramshort(mstring first, mstring second)
{
    FT("Magick::paramshort", (first, second));
    bool ArgUsed = false;
    for (unsigned int i=1; i<first.Len(); i++) {
	if (first[i]=='?')
	{
	    // Already handled
	}
	else if(first[i]=='n')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--name", second);
	}
	else if(first[i]=='d')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--desc", second);
	}
	else if(first[i]=='u')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--user", second);
	}
	else if(first[i]=='h')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--host", second);
	}
	else if(first[i]=='o')
	{
	    ArgUsed = paramlong ("--ownuser", second);
	}
	else if(first[i]=='P')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--protocol", second);
	}
	else if(first[i]=='l')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--level", second);
	}
	else if(first[i]=='g')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--lagtime", second);
	}
	else if(first[i]=='v')
	{
	    ArgUsed = paramlong ("--verbose", second);
	}
	else if(first[i]=='L')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--log", second);
	}
	else if(first[i]=='D')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--dbase", second);
	}
	else if(first[i]=='S')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--langdir", second);
	}
	else if(first[i]=='E')
	{
	    ArgUsed = paramlong ("--encrypt", second);
	}
	else if(first[i]=='e')
	{
	    ArgUsed = paramlong ("--decrypt", second);
	}
	else if(first[i]=='K')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--keyfile", second);
	}
	else if(first[i]=='c')
	{
	    ArgUsed = paramlong ("--compress", second);
	}
	else if(first[i]=='r')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--relink", second);
	}
	else if(first[i]=='t')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--cycle", second);
	}
	else if(first[i]=='T')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--check", second);
	}
	else if(first[i]=='p')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--ping", second);
	}
	else if(first[i]=='m')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--lwm", second);
	}
	else if(first[i]=='M')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--hwm", second);
	}
	else if(first[i]=='a')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--append", second);
	}
	else if(first[i]=='A')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--rename", second);
	}
	else if(first[i]=='R')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--ident", second);
	}
	else if(first[i]=='s')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--language", second);
	}
	else if(first[i]=='x')
	{
	    ArgUsed = paramlong ("--nodcc", second);
	}
	else if(first[i]=='f')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--inflight", second);
	}
	else if(first[i]=='i')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--logignore", second);
	}
	else if(first[i]=='I')
	{
	    if (ArgUsed)
		Log(LM_EMERGENCY, getLogMessage("COMMANDLINE/ONEOPTION"));
	    else
		ArgUsed = paramlong ("--ignore", second);
	}
	else
	{
	    Log(LM_ERROR, getLogMessage("COMMANDLINE/UNKNOWN_OPTION"),("-"+mstring(first[i])).c_str());
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
    int value_int;
    unsigned int value_uint;
    long value_long;
    unsigned long value_ulong;
    short value_float;

    // Groups ...
    mstring ts_Startup=mstring("Startup/");
    mstring ts_Services=mstring("Services/");
    mstring ts_Files=mstring("Files/");
    mstring ts_Config=mstring("Config/");
    mstring ts_ChanServ=mstring("ChanServ/");
    mstring ts_NickServ=mstring("NickServ/");
    mstring ts_MemoServ=mstring("MemoServ/");
    mstring ts_OperServ=mstring("OperServ/");
    mstring ts_CommServ=mstring("CommServ/");
    mstring ts_ServMsg=mstring("ServMsg/");

    in.Read(ts_Startup+"SERVER_NAME",value_mstring,"services.magick.tm");
    if (value_mstring != startup.server_name)
	reconnect = true;
    startup.server_name = value_mstring;

    in.Read(ts_Startup+"SERVER_DESC",value_mstring,FULLNAME);
    if (value_mstring != startup.server_desc)
	reconnect = true;
    startup.server_desc = value_mstring;

    in.Read(ts_Startup+"SERVICES_USER",value_mstring,"services");
    in.Read(ts_Startup+"OWNUSER",value_bool,false);
    if (value_bool != startup.ownuser)
	reconnect_clients = true;
    else if (value_mstring != startup.services_user &&
	     !startup.ownuser)
	reconnect_clients = true;
    startup.services_user = value_mstring;
    startup.ownuser = value_bool;
    in.Read(ts_Startup+"SETMODE",value_mstring, "");
    if (value_mstring != startup.setmode)
	reconnect_clients = true;
    startup.setmode = value_mstring;

    in.Read(ts_Startup+"SERVICES_HOST",value_mstring,"magick.tm");
    if (value_mstring != startup.services_host)
	reconnect_clients = true;
    startup.services_host = value_mstring;

    // REMOTE entries
    mstring ent="";
    i=1;
    { WLOCK(("Startup", "Servers"));
    startup.servers.clear();
    do {
	mstring rem = "REMOTE_";
	rem << i;
	in.Read(ts_Startup+rem,ent,"");
	if (ent!="") {
		mstring tmp[4];
		tmp[0]=ent.ExtractWord(1, ":");
		tmp[1]=ent.ExtractWord(2, ":");
		tmp[2]=ent.ExtractWord(3, ":");
		tmp[3]=ent.ExtractWord(4, ":");
		if (ent.WordCount(":") == 4 && tmp[1].IsNumber() && tmp[3].IsNumber())
		    startup.servers[tmp[0].LowerCase()] = triplet<unsigned int,mstring,unsigned int>(ACE_OS::atoi(tmp[1]),tmp[2],ACE_OS::atoi(tmp[3]));
		else
		    Log(LM_WARNING, getLogMessage("COMMANDLINE/CFG_SYNTAX"), (ts_Startup+rem).c_str());
		ent = "";
	}
	i++;
    } while (ent!="");
    }
    if (Server() == "" || !startup.IsServer(Server()))
	reconnect = true;

    in.Read(ts_Startup+"PROTOCOL",value_uint,0);
    if (value_uint != server.proto.Number())
    {
	server.proto.Set(value_uint);
	if (value_uint == server.proto.Number())
	    reconnect = true;
	else
	    Log(LM_WARNING, getLogMessage("COMMANDLINE/UNKNOWN_PROTO"),
			    value_uint, server.proto.Number());
    }

    in.Read(ts_Startup+"LEVEL",value_uint,1);
    if (value_uint > i_level)
	i_level = value_uint;
    startup.level = value_uint;

    in.Read(ts_Startup+"LAGTIME",value_mstring,"10s");
    if (FromHumanTime(value_mstring))
	startup.lagtime = FromHumanTime(value_mstring);
    else
	startup.lagtime = FromHumanTime("10s");

    in.Read(ts_Startup+"STOP",value_bool,true);
    i_shutdown = value_bool;

    in.Read(ts_Services+"NickServ",value_mstring,"NickServ");
    for (i=0; i<nickserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + nickserv.names.ExtractWord(i+1, " ") + " ")))
	{
	    nickserv.signoff(nickserv.names.ExtractWord(i+1, " "));
	}
    }
    nickserv.names = value_mstring;

    in.Read(ts_Services+"NickServ_Name",value_mstring,"Nickname Service");
    if (value_mstring != nickserv.realname)
    {
	nickserv.realname = value_mstring;
	for (i=0; i<nickserv.names.WordCount(" "); i++)
	{
	    nickserv.signoff(nickserv.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i=0; i<nickserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(nickserv.names.ExtractWord(i+1, " ")))
	    {
		if (isonstr.Len() > server.proto.MaxLine())
		{
		    server.sraw(((server.proto.Tokens() && server.proto.GetNonToken("ISON") != "") ?
			server.proto.GetNonToken("ISON") : mstring("ISON")) + " " + isonstr);
		    isonstr = "";
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(nickserv.names.ExtractWord(i+1, " "));
		isonstr += nickserv.names.ExtractWord(i+1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services+"Enforcer_Name",nickserv.enforcer_name,"Nickname Enforcer");

    in.Read(ts_Services+"ChanServ",value_mstring,"ChanServ");
    for (i=0; i<chanserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + chanserv.names.ExtractWord(i+1, " ") + " ")))
	{
	    chanserv.signoff(chanserv.names.ExtractWord(i+1, " "));
	}
    }
    chanserv.names = value_mstring;

    in.Read(ts_Services+"ChanServ_Name",value_mstring,"Channel Service");
    if (value_mstring != chanserv.realname)
    {
	chanserv.realname = value_mstring;
	for (i=0; i<chanserv.names.WordCount(" "); i++)
	{
	    chanserv.signoff(chanserv.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i=0; i<chanserv.names.WordCount(" "); i++)
	{
	    if (!chanserv.IsLive(chanserv.names.ExtractWord(i+1, " ")))
	    {
		if (isonstr.Len() > server.proto.MaxLine())
		{
		    server.sraw(((server.proto.Tokens() && server.proto.GetNonToken("ISON") != "") ?
			server.proto.GetNonToken("ISON") : mstring("ISON")) + " " + isonstr);
		    isonstr = "";
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(chanserv.names.ExtractWord(i+1, " "));
		isonstr += chanserv.names.ExtractWord(i+1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services+"MemoServ",value_mstring,"MemoServ");
    for (i=0; i<memoserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + memoserv.names.ExtractWord(i+1, " ") + " ")))
	{
	     memoserv.signoff(memoserv.names.ExtractWord(i+1, " "));
	}
    }
    memoserv.names = value_mstring;

    in.Read(ts_Services+"MemoServ_Name",value_mstring,"Memo/News Service");
    if (value_mstring != memoserv.realname)
    {
	memoserv.realname = value_mstring;
	for (i=0; i<memoserv.names.WordCount(" "); i++)
	{
	    memoserv.signoff(memoserv.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i=0; i<memoserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(memoserv.names.ExtractWord(i+1, " ")))
	    {
		if (isonstr.Len() > server.proto.MaxLine())
		{
		    server.sraw(((server.proto.Tokens() && server.proto.GetNonToken("ISON") != "") ?
			server.proto.GetNonToken("ISON") : mstring("ISON")) + " " + isonstr);
		    isonstr = "";
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(memoserv.names.ExtractWord(i+1, " "));
		isonstr += memoserv.names.ExtractWord(i+1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services+"OperServ",value_mstring,"OperServ");
    for (i=0; i<operserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + operserv.names.ExtractWord(i+1, " ") + " ")))
	{
	    operserv.signoff(operserv.names.ExtractWord(i+1, " "));
	}
    }
    operserv.names = value_mstring;

    in.Read(ts_Services+"OperServ_Name",value_mstring,"Operator Service");
    if (value_mstring != operserv.realname)
    {
	operserv.realname = value_mstring;
	for (i=0; i<operserv.names.WordCount(" "); i++)
	{
	    operserv.signoff(operserv.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i=0; i<operserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(operserv.names.ExtractWord(i+1, " ")))
	    {
		if (isonstr.Len() > server.proto.MaxLine())
		{
		    server.sraw(((server.proto.Tokens() && server.proto.GetNonToken("ISON") != "") ?
			server.proto.GetNonToken("ISON") : mstring("ISON")) + " " + isonstr);
		    isonstr = "";
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(operserv.names.ExtractWord(i+1, " "));
		isonstr += operserv.names.ExtractWord(i+1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services+"CommServ",value_mstring,"CommServ");
    for (i=0; i<commserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + commserv.names.ExtractWord(i+1, " ") + " ")))
	{
	    commserv.signoff(commserv.names.ExtractWord(i+1, " "));
	}
    }
    commserv.names = value_mstring;

    in.Read(ts_Services+"CommServ_Name",value_mstring,"Committee Service");
    if (value_mstring != commserv.realname)
    {
	commserv.realname = value_mstring;
	for (i=0; i<commserv.names.WordCount(" "); i++)
	{
	    commserv.signoff(commserv.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i=0; i<commserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(commserv.names.ExtractWord(i+1, " ")))
	    {
		if (isonstr.Len() > server.proto.MaxLine())
		{
		    server.sraw(((server.proto.Tokens() && server.proto.GetNonToken("ISON") != "") ?
			server.proto.GetNonToken("ISON") : mstring("ISON")) + " " + isonstr);
		    isonstr = "";
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(commserv.names.ExtractWord(i+1, " "));
		isonstr += commserv.names.ExtractWord(i+1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services+"ServMsg",value_mstring,"GlobalMsg DevNull");
    for (i=0; i<servmsg.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + servmsg.names.ExtractWord(i+1, " ") + " ")))
	{
	    servmsg.signoff(servmsg.names.ExtractWord(i+1, " "));
	}
    }
    servmsg.names = value_mstring;

    in.Read(ts_Services+"ServMsg_Name",value_mstring,mstring(PACKAGE) + " <--> User");
    if (value_mstring != servmsg.realname)
    {
	servmsg.realname = value_mstring;
	for (i=0; i<servmsg.names.WordCount(" "); i++)
	{
	    servmsg.signoff(servmsg.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && Connected())
    {
	for (i=0; i<servmsg.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(servmsg.names.ExtractWord(i+1, " ")))
	    {
		if (isonstr.Len() > server.proto.MaxLine())
		{
		    server.sraw(((server.proto.Tokens() && server.proto.GetNonToken("ISON") != "") ?
			server.proto.GetNonToken("ISON") : mstring("ISON")) + " " + isonstr);
		    isonstr = "";
		}
		WLOCK(("Server", "WaitIsOn"));
		server.WaitIsOn.insert(servmsg.names.ExtractWord(i+1, " "));
		isonstr += servmsg.names.ExtractWord(i+1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services+"SHOWSYNC",servmsg.showsync,true);

    if (isonstr != "")
	server.sraw(((server.proto.Tokens() && server.proto.GetNonToken("ISON") != "") ?
		server.proto.GetNonToken("ISON") : mstring("ISON")) + " " + isonstr);

    in.Read(ts_Files+"PIDFILE",files.pidfile,"magick.pid");
    in.Read(ts_Files+"LOGFILE",files.logfile,"magick.log");
    in.Read(ts_Files+"VERBOSE", i_verbose, false);
    in.Read(ts_Files+"MOTDFILE",files.motdfile,"magick.motd");
    in.Read(ts_Files+"LANGDIR",files.langdir,"lang");
    in.Read(ts_Files+"DATABASE",files.database,"magick.mnd");
    in.Read(ts_Files+"COMPRESSION",files.compression,6);
    if (files.compression > 9)
	files.compression = 9;
    in.Read(ts_Files+"KEYFILE",files.keyfile,"magick.key");
    in.Read(ts_Files+"ENCRYPTION",files.encryption,false);
    in.Read(ts_Files+"MEMOATTACH",files.memoattach,"files/memo");
    ACE_OS::mkdir(files.memoattach.c_str());
    in.Read(ts_Files+"MEMOATTACHSIZE",value_mstring,"0");
    if (FromHumanSpace(value_mstring))
	files.memoattachsize = FromHumanSpace(value_mstring);
    else
	files.memoattachsize = FromHumanSpace("0");
    in.Read(ts_Files+"PICTURE",files.picture,"files/pic");
    ACE_OS::mkdir(files.picture.c_str());
    in.Read(ts_Files+"PICTURESIZE",value_mstring,"0");
    if (FromHumanSpace(value_mstring))
	files.picturesize = FromHumanSpace(value_mstring);
    else
	files.picturesize = FromHumanSpace("0");
    in.Read(ts_Files+"PUBLIC",files.i_public,"files/public");
    ACE_OS::mkdir(files.i_public.c_str());
    in.Read(ts_Files+"PUBLICSIZE",value_mstring,"0");
    if (FromHumanSpace(value_mstring))
	files.publicsize = FromHumanSpace(value_mstring);
    else
	files.publicsize = FromHumanSpace("0");
    in.Read(ts_Files+"TEMPDIR",files.tempdir,"files/temp");
    ACE_OS::mkdir(files.tempdir.c_str());
    in.Read(ts_Files+"TEMPDIRSIZE",value_mstring,"0");
    if (FromHumanSpace(value_mstring))
	files.tempdirsize = FromHumanSpace(value_mstring);
    else
	files.tempdirsize = FromHumanSpace("0");
    in.Read(ts_Files+"BLOCKSIZE",value_mstring,"1k");
    if (FromHumanSpace(value_mstring))
	files.blocksize = FromHumanSpace(value_mstring);
    else
	files.blocksize = FromHumanSpace("1k");
    in.Read(ts_Files+"TIMEOUT",value_mstring,"2m");
    if (FromHumanTime(value_mstring))
	files.timeout = FromHumanTime(value_mstring);
    else
	files.timeout = FromHumanTime("2m");
    in.Read(ts_Files+"MIN_SPEED",value_mstring,"0");
    if (FromHumanSpace(value_mstring))
	files.min_speed = FromHumanSpace(value_mstring);
    else
	files.min_speed = FromHumanSpace("0");
    in.Read(ts_Files+"MAX_SPEED",value_mstring,"0");
    if (FromHumanSpace(value_mstring))
	files.max_speed = FromHumanSpace(value_mstring);
    else
	files.max_speed = FromHumanSpace("0");
    in.Read(ts_Files+"SAMPLETIME",value_mstring,"30s");
    if (FromHumanTime(value_mstring))
	files.sampletime = FromHumanTime(value_mstring);
    else
	files.sampletime = FromHumanTime("30s");

    in.Read(ts_Config+"SERVER_RELINK",value_mstring,"5s");
    if (FromHumanTime(value_mstring))
	config.server_relink = FromHumanTime(value_mstring);
    else
	config.server_relink = FromHumanTime("5s");

    in.Read(ts_Config+"SQUIT_PROTECT",value_mstring,"2m");
    if (FromHumanTime(value_mstring))
	config.squit_protect = FromHumanTime(value_mstring);
    else
	config.squit_protect = FromHumanTime("2m");

    in.Read(ts_Config+"SQUIT_CANCEL",value_mstring,"10s");
    if (FromHumanTime(value_mstring))
	config.squit_cancel = FromHumanTime(value_mstring);
    else
	config.squit_cancel = FromHumanTime("10s");

    in.Read(ts_Config+"CYCLETIME",value_mstring,"5m");
    if (FromHumanTime(value_mstring))
	config.cycletime = FromHumanTime(value_mstring);
    else
	config.cycletime = FromHumanTime("5m");

    in.Read(ts_Config+"SAVETIME",value_mstring,"5m");
    if (FromHumanTime(value_mstring))
	config.savetime = FromHumanTime(value_mstring);
    else
	config.savetime = FromHumanTime("5m");

    in.Read(ts_Config+"CHECKTIME",value_mstring,"5s");
    if (FromHumanTime(value_mstring))
	config.checktime = FromHumanTime(value_mstring);
    else
	config.checktime = FromHumanTime("5s");

    in.Read(ts_Config+"PING_FREQUENCY",value_mstring,"30s");
    if (FromHumanTime(value_mstring))
	config.ping_frequency = FromHumanTime(value_mstring);
    else
	config.ping_frequency = FromHumanTime("30s");

    in.Read(ts_Config+"STARTHRESH",config.starthresh, 4);
    in.Read(ts_Config+"LISTSIZE",config.listsize, 50);
    in.Read(ts_Config+"MAXLIST",value_uint, 250);
    if (value_uint < config.listsize)
	value_uint = config.listsize;
    config.maxlist = value_uint;

    in.Read(ts_Config+"MIN_THREADS",config.min_threads, 2);
    if (config.min_threads < 1)
	config.min_threads = 1;
    in.Read(ts_Config+"LOW_WATER_MARK",config.low_water_mark, 10);
    in.Read(ts_Config+"HIGH_WATER_MARK",config.high_water_mark, 15);
    if (config.high_water_mark < config.low_water_mark)
	config.high_water_mark = config.low_water_mark;
    in.Read(ts_Config+"MSG_SEEN_TIME",value_mstring,"30s");
    if (FromHumanTime(value_mstring))
	config.msg_seen_time = FromHumanTime(value_mstring);
    else
	config.msg_seen_time = FromHumanTime("30s");
    in.Read(ts_Config+"MSG_SEEN_ACT",config.msg_seen_act, 10);

    in.Read(ts_NickServ+"APPEND_RENAME",nickserv.append_rename, true);
    in.Read(ts_NickServ+"SUFFIXES",nickserv.suffixes,"_-^`");
    in.Read(ts_NickServ+"EXPIRE",value_mstring,"4w");
    if (FromHumanTime(value_mstring))
	nickserv.expire = FromHumanTime(value_mstring);
    else
	nickserv.expire = FromHumanTime("4w");

    in.Read(ts_NickServ+"IDENT",value_mstring,"1m");
    if (FromHumanTime(value_mstring))
	nickserv.ident = FromHumanTime(value_mstring);
    else
	nickserv.ident = FromHumanTime("1m");

    in.Read(ts_NickServ+"RELEASE",value_mstring,"1m");
    if (FromHumanTime(value_mstring))
	nickserv.release = FromHumanTime(value_mstring);
    else
	nickserv.release = FromHumanTime("1m");

    in.Read(ts_NickServ+"PASSFAIL",nickserv.passfail,5);
    in.Read(ts_NickServ+"DEF_PROTECT",nickserv.def_protect,true);
    in.Read(ts_NickServ+"LCK_PROTECT",nickserv.lck_protect,false);
    in.Read(ts_NickServ+"DEF_SECURE",nickserv.def_secure,false);
    in.Read(ts_NickServ+"LCK_SECURE",nickserv.lck_secure,false);
    in.Read(ts_NickServ+"DEF_NOEXPIRE",nickserv.def_noexpire,false);
    in.Read(ts_NickServ+"LCK_NOEXPIRE",nickserv.lck_noexpire,false);
    in.Read(ts_NickServ+"DEF_NOMEMO",nickserv.def_nomemo,false);
    in.Read(ts_NickServ+"LCK_NOMEMO",nickserv.lck_nomemo,false);
    in.Read(ts_NickServ+"DEF_PRIVATE",nickserv.def_private,false);
    in.Read(ts_NickServ+"LCK_PRIVATE",nickserv.lck_private,false);
    in.Read(ts_NickServ+"DEF_PRIVMSG",nickserv.def_privmsg,false);
    in.Read(ts_NickServ+"LCK_PRIVMSG",nickserv.lck_privmsg,false);
    in.Read(ts_NickServ+"DEF_LANGUAGE",value_mstring,"ENGLISH");
    if (value_mstring != nickserv.def_language)
    {
	nickserv.def_language = value_mstring.UpperCase();
	WLOCK(("LogMessages"));
	LogMessages.clear();
	LoadLogMessages(nickserv.def_language);
    }
    in.Read(ts_NickServ+"LCK_LANGUAGE",nickserv.lck_language,false);
    in.Read(ts_NickServ+"PICSIZE",value_mstring,"0");
    if (FromHumanSpace(value_mstring))
	nickserv.picsize = FromHumanSpace(value_mstring);
    else
	nickserv.picsize = FromHumanSpace("0");
    in.Read(ts_NickServ+"PICEXT",nickserv.picext,"jpg gif bmp tif");

    in.Read(ts_ChanServ+"HIDE",value_bool,false);
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


    in.Read(ts_ChanServ+"EXPIRE",value_mstring,"2w");
    if (FromHumanTime(value_mstring))
	chanserv.expire = FromHumanTime(value_mstring);
    else
	chanserv.expire = FromHumanTime("2w");

    in.Read(ts_ChanServ+"DEF_AKICK",chanserv.def_akick_reason,"You have been banned from channel");
    in.Read(ts_ChanServ+"PASSFAIL",chanserv.passfail,5);
    in.Read(ts_ChanServ+"CHANKEEP",value_mstring,"15s");
    if (FromHumanTime(value_mstring))
	chanserv.chankeep = FromHumanTime(value_mstring);
    else
	chanserv.chankeep = FromHumanTime("15s");

    in.Read(ts_ChanServ+"DEF_MLOCK",chanserv.def_mlock,"+nt");
    in.Read(ts_ChanServ+"LCK_MLOCK",chanserv.lck_mlock,"+");
    in.Read(ts_ChanServ+"DEF_BANTIME",value_mstring, "0");
    if (FromHumanTime(value_mstring))
	chanserv.def_bantime = FromHumanTime(value_mstring);
    else
	chanserv.def_bantime = FromHumanTime("0");

    in.Read(ts_ChanServ+"LCK_BANTIME",chanserv.lck_bantime,false);
    in.Read(ts_ChanServ+"DEF_PARTTIME",value_mstring, "0");
    if (FromHumanTime(value_mstring))
	chanserv.def_parttime = FromHumanTime(value_mstring);
    else
	chanserv.def_parttime = FromHumanTime("0");

    in.Read(ts_ChanServ+"LCK_PARTTIME",chanserv.lck_parttime,false);
    in.Read(ts_ChanServ+"DEF_KEEPTOPIC",chanserv.def_keeptopic,true);
    in.Read(ts_ChanServ+"LCK_KEEPTOPIC",chanserv.lck_keeptopic,false);
    in.Read(ts_ChanServ+"DEF_TOPICLOCK",chanserv.def_topiclock,false);
    in.Read(ts_ChanServ+"LCK_TOPICLOCK",chanserv.lck_topiclock,false);
    in.Read(ts_ChanServ+"DEF_PRIVATE",chanserv.def_private,false);
    in.Read(ts_ChanServ+"LCK_PRIVATE",chanserv.lck_private,false);
    in.Read(ts_ChanServ+"DEF_SECUREOPS",chanserv.def_secureops,false);
    in.Read(ts_ChanServ+"LCK_SECUREOPS",chanserv.lck_secureops,false);
    in.Read(ts_ChanServ+"DEF_SECURE",chanserv.def_secure,false);
    in.Read(ts_ChanServ+"LCK_SECURE",chanserv.lck_secure,false);
    in.Read(ts_ChanServ+"DEF_NOEXPIRE",chanserv.def_noexpire,false);
    in.Read(ts_ChanServ+"LCK_NOEXPIRE",chanserv.lck_noexpire,false);
    in.Read(ts_ChanServ+"DEF_ANARCHY",chanserv.def_anarchy,false);
    in.Read(ts_ChanServ+"LCK_ANARCHY",chanserv.lck_anarchy,false);
    in.Read(ts_ChanServ+"DEF_KICKONBAN",chanserv.def_kickonban,false);
    in.Read(ts_ChanServ+"LCK_KICKONBAN",chanserv.lck_kickonban,false);
    in.Read(ts_ChanServ+"DEF_RESTRICTED",chanserv.def_restricted,false);
    in.Read(ts_ChanServ+"LCK_RESTRICTED",chanserv.lck_restricted,false);
    in.Read(ts_ChanServ+"DEF_JOIN",chanserv.def_join,false);
    in.Read(ts_ChanServ+"LCK_JOIN",chanserv.lck_join,false);
    in.Read(ts_ChanServ+"DEF_REVENGE",value_mstring,"NONE");
    if (chanserv.IsRevengeLevel(value_mstring))
	chanserv.def_revenge = value_mstring.UpperCase();
    else
	chanserv.def_revenge = "NONE";

    in.Read(ts_ChanServ+"LCK_REVENGE",chanserv.lck_revenge,false);
    in.Read(ts_ChanServ+"LEVEL_MIN",chanserv.level_min,-1);
    in.Read(ts_ChanServ+"LEVEL_MAX",chanserv.level_max,30);
    in.Read(ts_ChanServ+"LVL_AUTODEOP",chanserv.lvl["AUTODEOP"],-1);
    in.Read(ts_ChanServ+"LVL_AUTOVOICE",chanserv.lvl["AUTOVOICE"],5);
    in.Read(ts_ChanServ+"LVL_AUTOOP",chanserv.lvl["AUTOOP"],10);
    in.Read(ts_ChanServ+"LVL_READMEMO",chanserv.lvl["READMEMO"],0);
    in.Read(ts_ChanServ+"LVL_WRITEMEMO",chanserv.lvl["WRITEMEMO"],15);
    in.Read(ts_ChanServ+"LVL_DELMEMO",chanserv.lvl["DELMEMO"],25);
    in.Read(ts_ChanServ+"LVL_GREET",chanserv.lvl["GREET"],1);
    in.Read(ts_ChanServ+"LVL_OVERGREET",chanserv.lvl["OVERGREET"],25);
    in.Read(ts_ChanServ+"LVL_MESSAGE",chanserv.lvl["MESSAGE"],20);
    in.Read(ts_ChanServ+"LVL_AKICK",chanserv.lvl["AKICK"],20);
    in.Read(ts_ChanServ+"LVL_SUPER",chanserv.lvl["SUPER"],25);
    in.Read(ts_ChanServ+"LVL_UNBAN",chanserv.lvl["UNBAN"],10);
    in.Read(ts_ChanServ+"LVL_ACCESS",chanserv.lvl["ACCESS"],5);
    in.Read(ts_ChanServ+"LVL_SET",chanserv.lvl["SET"],25);
    in.Read(ts_ChanServ+"LVL_VIEW",chanserv.lvl["VIEW"],1);
    in.Read(ts_ChanServ+"LVL_CMDINVITE",chanserv.lvl["CMDINVITE"],5);
    in.Read(ts_ChanServ+"LVL_CMDUNBAN",chanserv.lvl["CMDUNBAN"],10);
    in.Read(ts_ChanServ+"LVL_CMDVOICE",chanserv.lvl["CMDVOICE"],5);
    in.Read(ts_ChanServ+"LVL_CMDOP",chanserv.lvl["CMDOP"],10);
    in.Read(ts_ChanServ+"LVL_CMDKICK",chanserv.lvl["CMDKICK"],15);
    in.Read(ts_ChanServ+"LVL_CMDMODE",chanserv.lvl["CMDMODE"],15);
    in.Read(ts_ChanServ+"LVL_CMDCLEAR",chanserv.lvl["CMDCLEAR"],20);

    in.Read(ts_MemoServ+"NEWS_EXPIRE",value_mstring,"3w");
    if (FromHumanTime(value_mstring))
	memoserv.news_expire = FromHumanTime(value_mstring);
    else
	memoserv.news_expire = FromHumanTime("3w");

    in.Read(ts_MemoServ+"INFLIGHT",value_mstring,"3m");
    if (FromHumanTime(value_mstring))
	memoserv.inflight = FromHumanTime(value_mstring);
    else
	memoserv.inflight = FromHumanTime("3m");

    in.Read(ts_MemoServ+"FILES",memoserv.files,0);
    in.Read(ts_MemoServ+"FILESIZE",value_mstring,"0");
    if (FromHumanSpace(value_mstring))
	memoserv.filesize = FromHumanSpace(value_mstring);
    else
	memoserv.filesize = FromHumanSpace("0");

    in.Read(ts_OperServ+"SERVICES_ADMIN",operserv.services_admin,"");
    in.Read(ts_OperServ+"SECURE",operserv.secure,false);
    in.Read(ts_OperServ+"SECUREOPER",operserv.secureoper,false);
    in.Read(ts_OperServ+"DEF_EXPIRE",value_mstring,"3h");
    if (FromHumanTime(value_mstring))
	operserv.def_expire = FromHumanTime(value_mstring);
    else
	operserv.def_expire = FromHumanTime("3h");

    in.Read(ts_OperServ+"EXPIRE_OPER",value_mstring,"1d");
    if (FromHumanTime(value_mstring))
	operserv.expire_oper = FromHumanTime(value_mstring);
    else
	operserv.expire_oper = FromHumanTime("1d");

    in.Read(ts_OperServ+"EXPIRE_ADMIN",value_mstring,"1w");
    if (FromHumanTime(value_mstring))
	operserv.expire_admin = FromHumanTime(value_mstring);
    else
	operserv.expire_admin = FromHumanTime("1w");

    in.Read(ts_OperServ+"EXPIRE_SOP",value_mstring,"8w");
    if (FromHumanTime(value_mstring))
	operserv.expire_sop = FromHumanTime(value_mstring);
    else
	operserv.expire_sop = FromHumanTime("8w");

    in.Read(ts_OperServ+"EXPIRE_SADMIN",value_mstring,"1y");
    if (FromHumanTime(value_mstring))
	operserv.expire_sadmin = FromHumanTime(value_mstring);
    else
	operserv.expire_sadmin = FromHumanTime("1y");

    in.Read(ts_OperServ+"MAX_CLONE",operserv.max_clone,50);
    in.Read(ts_OperServ+"CLONE_LIMIT",operserv.clone_limit,2);
    in.Read(ts_OperServ+"DEF_CLONE",operserv.def_clone,"Maximum connections from one host exceeded");
    in.Read(ts_OperServ+"CLONE_TRIGGER",operserv.clone_trigger,10);
    in.Read(ts_OperServ+"CLONE_TIME",value_mstring,"3h");
    if (FromHumanTime(value_mstring))
	operserv.clone_time = FromHumanTime(value_mstring);
    else
	operserv.clone_time = FromHumanTime("3h");
    in.Read(ts_OperServ+"CLONE_AKILL",operserv.clone_akill,"Clone trigger exceeded, Automatic AKILL");
    in.Read(ts_OperServ+"CLONE_AKILLTIME",value_mstring,"30m");
    if (FromHumanTime(value_mstring))
	operserv.clone_akilltime = FromHumanTime(value_mstring);
    else
	operserv.clone_akilltime = FromHumanTime("30m");

    in.Read(ts_OperServ+"FLOOD_TIME",value_mstring,"10s");
    if (FromHumanTime(value_mstring))
	operserv.flood_time = FromHumanTime(value_mstring);
    else
	operserv.flood_time = FromHumanTime("10s");

    in.Read(ts_OperServ+"FLOOD_MSGS",operserv.flood_msgs,5);

    in.Read(ts_OperServ+"IGNORE_TIME",value_mstring,"20s");
    if (FromHumanTime(value_mstring))
	operserv.ignore_time = FromHumanTime(value_mstring);
    else
	operserv.ignore_time = FromHumanTime("20s");

    in.Read(ts_OperServ+"IGNORE_LIMIT",operserv.ignore_limit,5);
    in.Read(ts_OperServ+"IGNORE_REMOVE",value_mstring,"5m");
    if (FromHumanTime(value_mstring))
	operserv.ignore_remove = FromHumanTime(value_mstring);
    else
	operserv.ignore_remove = FromHumanTime("5m");

    in.Read(ts_OperServ+"IGNORE_METHOD",operserv.ignore_method,8);
    in.Read(ts_OperServ+"LOG_IGNORE",operserv.log_ignore,false);
    in.Read(ts_OperServ+"INIT_HTM_GAP",value_mstring,"5s");
    if (FromHumanTime(value_mstring))
	operserv.init_htm_gap = FromHumanTime(value_mstring);
    else
	operserv.init_htm_gap = FromHumanTime("5s");
    in.Read(ts_OperServ+"INIT_HTM_THRESH",value_mstring,"16k");
    if (FromHumanSpace(value_mstring))
	operserv.init_htm_thresh = FromHumanSpace(value_mstring);
    else
	operserv.init_htm_thresh = FromHumanSpace("16k");
    in.Read(ts_OperServ+"MAX_HTM_GAP",value_mstring,"1m");
    if (FromHumanTime(value_mstring))
	operserv.max_htm_gap = FromHumanTime(value_mstring);
    else
	operserv.max_htm_gap = FromHumanTime("1m");
    in.Read(ts_OperServ+"HTM_ON_GAP",value_mstring,"30s");
    if (FromHumanTime(value_mstring))
	operserv.htm_on_gap = FromHumanTime(value_mstring);
    else
	operserv.htm_on_gap = FromHumanTime("30s");

    RemCommands();
    in.Read(ts_CommServ+"DEF_OPENMEMOS",commserv.def_openmemos,true);
    in.Read(ts_CommServ+"LCK_OPENMEMOS",commserv.lck_openmemos,false);
    in.Read(ts_CommServ+"DEF_SECURE",commserv.def_secure,false);
    in.Read(ts_CommServ+"LCK_SECURE",commserv.lck_secure,false);
    in.Read(ts_CommServ+"DEF_PRIVATE",commserv.def_private,false);
    in.Read(ts_CommServ+"LCK_PRIVATE",commserv.lck_private,false);
    in.Read(ts_CommServ+"ALL_NAME",commserv.all_name,"ALL");
    in.Read(ts_CommServ+"REGD_NAME",commserv.regd_name,"REGD");
    in.Read(ts_CommServ+"SADMIN_NAME",commserv.sadmin_name,"SADMIN");
    in.Read(ts_CommServ+"SADMIN_SECURE",commserv.sadmin_secure,true);
    in.Read(ts_CommServ+"SADMIN_PRIVATE",commserv.sadmin_private,false);
    in.Read(ts_CommServ+"SADMIN_OPENMEMOS",commserv.sadmin_openmemos,true);
    in.Read(ts_CommServ+"SOP_NAME",commserv.sop_name,"SOP");
    in.Read(ts_CommServ+"SOP_SECURE",commserv.sop_secure,true);
    in.Read(ts_CommServ+"SOP_PRIVATE",commserv.sop_private,false);
    in.Read(ts_CommServ+"SOP_OPENMEMOS",commserv.sop_openmemos,true);
    in.Read(ts_CommServ+"ADMIN_NAME",commserv.admin_name,"ADMIN");
    in.Read(ts_CommServ+"ADMIN_SECURE",commserv.admin_secure,true);
    in.Read(ts_CommServ+"ADMIN_PRIVATE",commserv.admin_private,false);
    in.Read(ts_CommServ+"ADMIN_OPENMEMOS",commserv.admin_openmemos,true);
    in.Read(ts_CommServ+"OPER_NAME",commserv.oper_name,"OPER");
    in.Read(ts_CommServ+"OPER_SECURE",commserv.oper_secure,true);
    in.Read(ts_CommServ+"OPER_PRIVATE",commserv.oper_private,false);
    in.Read(ts_CommServ+"OPER_OPENMEMOS",commserv.oper_openmemos,true);
    commserv.all_name.MakeUpper();
    commserv.regd_name.MakeUpper();
    commserv.sadmin_name.MakeUpper();
    commserv.sop_name.MakeUpper();
    commserv.admin_name.MakeUpper();
    commserv.oper_name.MakeUpper();
    AddCommands();

    if (commserv.IsList(commserv.all_name))
    {
	MLOCK(("CommServ", "list", commserv.all_name, "member"));
	while (commserv.list[commserv.all_name].size())
	{
	    commserv.list[commserv.all_name].member =
			commserv.list[commserv.all_name].begin();
	    commserv.list[commserv.all_name].erase();
	}
    }
    else
    {
	WLOCK(("CommServ", "list"));
	commserv.list[commserv.all_name] = Committee(commserv.all_name, 
					    "All Users");
    }
    commserv.list[commserv.all_name].Secure(false);
    commserv.list[commserv.all_name].Private(true);
    commserv.list[commserv.all_name].OpenMemos(false);

    if (commserv.IsList(commserv.regd_name))
    {
	MLOCK(("CommServ", "list", commserv.regd_name, "member"));
	while (commserv.list[commserv.regd_name].size())
	{
	    commserv.list[commserv.regd_name].member =
			commserv.list[commserv.regd_name].begin();
	    commserv.list[commserv.regd_name].erase();
	}
    }
    else
    {
	WLOCK(("CommServ", "list"));
	commserv.list[commserv.regd_name] = Committee(commserv.regd_name, 
					    "Registered Users");
    }
    commserv.list[commserv.regd_name].Secure(false);
    commserv.list[commserv.regd_name].Private(true);
    commserv.list[commserv.regd_name].OpenMemos(false);

    if (commserv.IsList(commserv.sadmin_name))
    {
	MLOCK(("CommServ", "list", commserv.sadmin_name, "member"));
	while (commserv.list[commserv.sadmin_name].size())
	{
	    commserv.list[commserv.sadmin_name].member =
			commserv.list[commserv.sadmin_name].begin();
	    commserv.list[commserv.sadmin_name].erase();
	}
    }
    else
    {
	WLOCK(("CommServ", "list"));
	commserv.list[commserv.sadmin_name] = Committee(commserv.sadmin_name, 
					    "Services Administrators");
    }
    commserv.list[commserv.sadmin_name].Secure(commserv.sadmin_secure);
    commserv.list[commserv.sadmin_name].Private(commserv.sadmin_private);
    commserv.list[commserv.sadmin_name].OpenMemos(commserv.sadmin_openmemos);
    { MLOCK(("CommServ", "list", commserv.sadmin_name, "member"));
    for (i=1; i<=operserv.services_admin.WordCount(", "); i++)
	commserv.list[commserv.sadmin_name].insert(
	    operserv.services_admin.ExtractWord(i, ", "),
	    operserv.FirstName());
    }

    if (!commserv.IsList(commserv.sop_name))
    {
	WLOCK(("CommServ", "list"));
	commserv.list[commserv.sop_name] = Committee(commserv.sop_name,
				    &commserv.list[commserv.sadmin_name],
				    "Services Operators");
    }
    commserv.list[commserv.sop_name].Secure(commserv.sop_secure);
    commserv.list[commserv.sop_name].Private(commserv.sop_private);
    commserv.list[commserv.sop_name].OpenMemos(commserv.sop_openmemos);

    if (!commserv.IsList(commserv.admin_name))
    {
	WLOCK(("CommServ", "list"));
	commserv.list[commserv.admin_name] = Committee(commserv.admin_name, 
				    &commserv.list[commserv.sadmin_name],
				    "Server Administrators");
    }
    commserv.list[commserv.admin_name].Secure(commserv.admin_secure);
    commserv.list[commserv.admin_name].Private(commserv.admin_private);
    commserv.list[commserv.admin_name].OpenMemos(commserv.admin_openmemos);

    if (!commserv.IsList(commserv.oper_name))
    {
	WLOCK(("CommServ", "list"));
	commserv.list[commserv.oper_name] = Committee(commserv.oper_name, 
				    &commserv.list[commserv.admin_name],
				    "Server Operators");
    }
    commserv.list[commserv.oper_name].Secure(commserv.oper_secure);
    commserv.list[commserv.oper_name].Private(commserv.oper_private);
    commserv.list[commserv.oper_name].OpenMemos(commserv.oper_openmemos);

    if (reconnect && Connected())
    {
	server.raw(((server.proto.Tokens() && server.proto.GetNonToken("ERROR") != "") ?
		server.proto.GetNonToken("ERROR") : mstring("ERROR")) + " " +
		" :Closing Link: Configuration reload required restart!");
	WLOCK(("IrcSvcHandler"));
	if (ircsvchandler != NULL)
	{
	    ircsvchandler->shutdown();
	    delete ircsvchandler;
	    ircsvchandler = NULL;
	}
	ACE_Reactor::instance()->schedule_timer(&rh,0,ACE_Time_Value::zero);
    }

    RET(true);
    CP(("%s read and loaded to live configuration.", i_config_file.c_str()));
}

int SignalHandler::handle_signal(int signum, siginfo_t *siginfo, ucontext_t *ucontext)
{
    // No trace, screws with LastFunc...
    //FT("SignalHandler::handle_signal", (signum, "(siginfo_t *) siginfo", "(ucontext_t *) ucontext"));
    static mDateTime LastSEGV;
    static mstring LastFunc;
    ThreadID *tid;

    // todo: fill this sucker in
    switch(signum)
    {
    case SIGINT:	// CTRL-C, Background.
#if defined(SIGQUIT) && (SIGQUIT != 0)
    case SIGQUIT:	// Terminal dead, Background.
#endif
	Log(LM_WARNING, Parent->getLogMessage("SYS_ERRORS/SIGNAL_IGNORE"), signum);
	//fork();
	break;


#if defined(SIGTERM) && (SIGTERM != 0)
    case SIGTERM:	// Save DB's (often prequil to -KILL!)
	Log(LM_NOTICE, Parent->getLogMessage("SYS_ERRORS/SIGNAL_SAVE"), signum);
	announce(Parent->operserv.FirstName(), Parent->getMessage("MISC/SIGNAL_SAVE"), signum);
	Parent->events->ForceSave();
	break;
#endif


#if defined(SIGHUP) && (SIGHUP != 0)
    case SIGHUP:	// Reload CFG/DB's
	Log(LM_NOTICE, Parent->getLogMessage("SYS_ERRORS/SIGNAL_LOAD"), signum);
	if (!Parent->get_config_values())
	{
	    Log(LM_EMERGENCY, Parent->getLogMessage("COMMANDLINE/NO_CFG_FILE"),
	    					Parent->Config_File().c_str());
	}
	else
	{
	    announce(Parent->operserv.FirstName(), Parent->getMessage("MISC/SIGNAL_LOAD"), signum);
	}
	break;
#endif


    case SIGILL:	// illegal opcode, this suckers gone walkabouts..
#ifdef SIGIOT
    case SIGIOT:	// abort(), exit immediately!
#endif
#ifdef SIGBUS
    case SIGBUS:	// BUS error (fatal)
#endif
	tid = mThread::find();
	if (tid != NULL)
	    LastFunc = tid->LastFunc();
	FLUSH();
	Log(LM_ALERT, Parent->getLogMessage("SYS_ERRORS/SIGNAL_KILL"), signum, LastFunc.c_str());
	announce(Parent->operserv.FirstName(), Parent->getMessage("MISC/SIGNAL_KILL"), signum, LastFunc.c_str());
	LastFunc = "";
	Parent->Shutdown(true);
	Parent->Die();
	return -1;
	break;


    case SIGSEGV:	// Segfault, validate all storage.
	// IF LastSEGV is defined, and time between now and
	// LastSEGV is < 5 seconds ...
	if(LastSEGV.SecondsSince() < 5)
	{
	    FLUSH();
	    Log(LM_ALERT, Parent->getLogMessage("SYS_ERRORS/SIGNAL_KILL"), signum, LastFunc.c_str());
	    CP(("Got second sigsegv call, giving magick the boot"));
	    announce(Parent->operserv.FirstName(), Parent->getMessage("MISC/SIGNAL_KILL"), signum, LastFunc.c_str());
	    LastFunc = "";
	    Parent->Shutdown(true);
	    Parent->Die();
	    return -1;
	}
	else
	{
	    tid = mThread::find();
	    if (tid != NULL)
		LastFunc = tid->LastFunc();
	    FLUSH();
	    Log(LM_ERROR, Parent->getLogMessage("SYS_ERRORS/SIGNAL_RETRY"), signum);
	    LastSEGV = Now();
	    CP(("Got first sigsegv call, giving it another chance"));
	}
	break;


    default:		// Everything else.
	Log(LM_WARNING, Parent->getLogMessage("SYS_ERRORS/SIGNAL_IGNORE"), signum);
	break;//ignore (todo log that we got it and we're ignoring it)
    }
    return 0;
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

bool Magick::startup_t::IsServer(mstring server)
{
    FT("Magick::startup_t::IsServer", (server));

    RLOCK(("Startup", "Servers"));
    if (servers.find(server.LowerCase()) != servers.end()) {
	RET(true);
    }
    RET(false);
}

triplet<unsigned int,mstring,unsigned int> Magick::startup_t::Server(mstring server)
{
    FT("Magick::startup_t::Server", (server));
    triplet<unsigned int,mstring,unsigned int> value(0, "", 0);

    RLOCK(("Startup", "Servers"));
    if (IsServer(server)) {
	value = servers.find(server.LowerCase())->second;
    }
    NRET(triplet<unsigned int.mstring.unsigned int>, value);
}

vector<mstring> Magick::startup_t::PriorityList(unsigned int pri)
{
    FT("Magick::startup_t::PriorityList", (pri));
    vector<mstring> list;

    map<mstring,triplet<unsigned int,mstring,unsigned int> >::iterator iter;

    RLOCK(("Startup", "Servers"));
    for (iter=servers.begin(); iter!=servers.end(); iter++) {
	if (iter->second.third == pri)
	    list.push_back(iter->first);
    }
    NRET(vector<mstring>, list);
}

mstring Magick::GetKey()
{
    NFT("Magick::GetKey");
    mstring retval = "";
#ifdef HASCRYPT
    if (files.KeyFile() != "" &&
	mFile::Exists(files.KeyFile()))
    {
	mFile keyfile(files.KeyFile());
	unsigned char tmp[KEYLEN], key[KEYLEN];
	des_key_schedule key1, key2;
	des_cblock ckey1, ckey2;

#include "crypt.h"
	ACE_OS::memset(tmp, 0, KEYLEN);
	keyfile.Read(tmp, KEYLEN);
	tmp[KEYLEN-1]=0;

	/* Unscramble keyfile keys */
	des_string_to_key(crypto_key1,&ckey1);
	des_set_key(&ckey1,key1);
	des_string_to_key(crypto_key2,&ckey2);
	des_set_key(&ckey2,key2);

	/* Use keyfile keys to get REAL key */
	mDES(tmp, key, KEYLEN, key1, key2, 0);
	retval = (char *) key;
    }
#endif
    NRET(mstring, retval);
}


void Magick::Disconnect()
{
    NFT("Magick::Disconnect");
    i_reconnect = false;
    i_connected = false;
    WLOCK(("IrcSvcHandler"));
    if (ircsvchandler != NULL)
    {
	ircsvchandler->shutdown();
	delete ircsvchandler;
	ircsvchandler = NULL;
    }
}

void Magick::send(mstring in)
{
    RLOCK(("IrcSvcHandler"));
    if (ircsvchandler != NULL)
	ircsvchandler->send(in);
}

SXP::Tag Magick::tag_Magick("Magick");

void Magick::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Magick::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    if( pElement->IsA( operserv.GetClassTag() ) )
    {
        pIn->ReadTo(&operserv);
    }
    else if( pElement->IsA( nickserv.GetClassTag() ) )
    {
        pIn->ReadTo(&nickserv);
    }
    else if( pElement->IsA( chanserv.GetClassTag() ) )
    {
        pIn->ReadTo(&chanserv);
    }
    else if( pElement->IsA( memoserv.GetClassTag() ) )
    {
        pIn->ReadTo(&memoserv);
    }
    else if( pElement->IsA( commserv.GetClassTag() ) )
    {
        pIn->ReadTo(&commserv);
    }
    else if( pElement->IsA( filesys.GetClassTag() ) )
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
    FT("Magick::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
    if( pElement->IsA( tag_Magick ) )
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

void Magick::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("Magick::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    attribs["version"]= VERSION;
    pOut->BeginObject(tag_Magick, attribs);

    SXP::dict attr;
    pOut->WriteSubElement(&operserv, attr);
    pOut->WriteSubElement(&nickserv, attr);
    pOut->WriteSubElement(&chanserv, attr);
    pOut->WriteSubElement(&memoserv, attr);
    pOut->WriteSubElement(&commserv, attr);
    pOut->WriteSubElement(&filesys, attr);

    pOut->EndObject(tag_Magick);
}

void Magick::save_databases()
{
    NFT("Magick::save_databases");
    i_saving = true;
    if (mFile::Exists(files.Database()+".new"))
	mFile::Erase(files.Database()+".new");
    {
	//SXP::CFileOutStream o(files.Database()+".new");
	SXP::MFileOutStream o(files.Database()+".new", files.Compression(),
				(files.Encryption() ? GetKey() : mstring("")));
	o.BeginXML();
	SXP::dict attribs;
	WriteElement(&o, attribs);
    }
    if (mFile::Exists(files.Database()))
	mFile::Copy(files.Database(), files.Database()+".old");
    if (mFile::Exists(files.Database()+".new"))
    {
	mFile::Copy(files.Database()+".new", files.Database());
	mFile::Erase(files.Database()+".new");
	if (mFile::Exists(files.Database()+".old"))
	    mFile::Erase(files.Database()+".old");
    }
    i_saving = false;
}

void Magick::load_databases()
{
    NFT("Magick::load_databases");
    if (mFile::Exists(files.Database()))
    {
	Log(LM_STARTUP, getLogMessage("EVENT/LOAD"));
   	SXP::CParser p( this ); // let the parser know which is the object
	if (p.FeedFile(files.Database(), GetKey()) < 1)
	{
	    Log(LM_EMERGENCY, getLogMessage("ERROR/CORRUPT_DB"));
	}
    }
}


set<mstring> Magick::LNG_Loaded()
{
    set<mstring> retval;
    map<mstring, map<mstring, mstring> >::iterator i;
    RLOCK(("Messages"));
    for (i=Messages.begin(); i!=Messages.end(); i++)
    {
	retval.insert(i->first);
    }
    return retval;
}

size_t Magick::LNG_Usage(mstring lang)
{
    size_t retval = 0;

    map<mstring, mstring>::iterator i;
    RLOCK(("Messages"));
    if (Messages.find(lang.UpperCase()) != Messages.end())
    {
	retval += Messages.find(lang.UpperCase())->first.capacity();
	RLOCK(("Messages", lang.UpperCase()));
	for (i = Messages[lang.UpperCase()].begin();
				i != Messages[lang.UpperCase()].end(); i++)
	{
	    retval += i->first.capacity();
	    retval += i->second.capacity();
	}
    }

    return retval;
}

set<mstring> Magick::HLP_Loaded()
{
    set<mstring> retval;
    map<mstring, map<mstring, vector<triplet<mstring, mstring, mstring> > > >::iterator i;
    RLOCK(("Help"));
    for (i=Help.begin(); i!=Help.end(); i++)
    {
	retval.insert(i->first);
    }
    return retval;
}

size_t Magick::HLP_Usage(mstring lang)
{
    size_t retval = 0;

    map<mstring, vector<triplet<mstring, mstring, mstring> > >::iterator i;
    vector<triplet<mstring, mstring, mstring> >::iterator j;

    RLOCK(("Help"));
    if (Help.find(lang.UpperCase()) != Help.end())
    {
	retval += Help.find(lang.UpperCase())->first.capacity();
	RLOCK(("Help", lang.UpperCase()));
	for (i=Help[lang.UpperCase()].begin();
				i != Help[lang.UpperCase()].end(); i++)
	{
	    retval += i->first.capacity();
	    for (j = i->second.begin(); j != i->second.end(); j++)
	    {
		retval += j->first.capacity();
		retval += j->second.capacity();
		retval += j->third.capacity();
	    }
	}
    }

    return retval;
}

size_t Magick::LFO_Usage()
{
    size_t retval = 0;

    map<mstring,mstring>::iterator i;
    RLOCK(("LogMessages"));
    for (i=LogMessages.begin(); i!=LogMessages.end(); i++)
    {
	retval += i->first.capacity();
	retval += i->second.capacity();
    }

    return retval;
}

