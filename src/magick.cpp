#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#pragma implementation "language.h"
#pragma implementation "logfile.h"
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
#define RCSID(x,y) const char *rcsid_magick_cpp_ ## x () { return y; }
RCSID(magick_cpp, "@(#)$Id$");

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
#include "mconfig.h"
#include "language.h"
#include "logfile.h"
#ifdef CONVERT
#include "convert_magick.h"
#include "convert_esper.h"
#include "convert_epona.h"
#endif

mDateTime Magick::i_StartTime;

map < ACE_thread_t, Magick * > Magick::InstanceMap;
#ifndef MAGICK_HAS_EXCEPTIONS
static Magick GLOB_Magick;
#endif

/* Class for testing purposes ... */
#ifdef TEST_MODE
class TestInput : public ACE_Task < ACE_MT_SYNCH >
{
    typedef ACE_Task < ACE_MT_SYNCH > internal;
    Magick *magick_instance;

public:
    TestInput(ACE_Thread_Manager * tm = 0) : internal(tm)
    {
    }

    int open(void *in = 0)
    {
	magick_instance = (Magick *) in;
	int retval = activate();

	return retval;
    }

    int close(u_long flags = 0)
    {
	static_cast < void > (flags);

	return 0;
    }

    int svc(void)
    {
	Magick::register_instance(magick_instance);
	while (!Magick::instance().Shutdown())
	{
	    char buf[513];

	    ACE_OS::memset(buf, 0, 513);
	    ACE_OS::fgets(buf, 512, stdin);
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL)
		Magick::instance().ircsvchandler->handle_input(buf);
	}
	Magick::deregister_instance();
	return 0;
    }

    int fini()
    {
	return 0;
    }
};
#endif /* TEST_MODE */

#ifdef MAGICK_HAS_EXCEPTIONS
void Magick::register_instance(Magick * ins, ACE_thread_t id) throw (E_Magick)
#else
void Magick::register_instance(Magick * ins, ACE_thread_t id)
#endif
{
    BTCB();
    if (ins == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw E_Magick(E_Magick::T_Invalid);
#endif
	return;
    }

    InstanceMap[id] = ins;
    ETCB();
}

void Magick::deregister_instance(ACE_thread_t id)
{
    BTCB();
    FLUSH();
    InstanceMap.erase(id);
    ETCB();
}

bool Magick::instance_exists(ACE_thread_t id)
{
    BTCB();
    map < ACE_thread_t, Magick * >::iterator iter = InstanceMap.find(id);
    return (iter != InstanceMap.end());
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
Magick &Magick::instance(ACE_thread_t id) throw (E_Magick)
#else
Magick &Magick::instance(ACE_thread_t id)
#endif
{
    BTCB();
    map < ACE_thread_t, Magick * >::iterator iter = InstanceMap.find(id);
    if (iter != InstanceMap.end() && iter->second != NULL)
	return *(iter->second);

#ifdef MAGICK_HAS_EXCEPTIONS
    throw E_Magick(E_Magick::T_NotFound);
#else
    LOG(LM_CRITICAL, "EXCEPTIONS/MAGICK", (id));
    return GLOB_Magick;
#endif
    ETCB();
}

Magick::Magick(int inargc, char **inargv)
{
    BTCB();
    for (int i = 0; i < inargc; i++)
	argv.push_back(inargv[i]);
    init();
    ETCB();
}

Magick::Magick(const vector < mstring > & inargv)
{
    BTCB();
    argv = inargv;
    init();
    ETCB();
}

void Magick::init()
{
    BTCB();
    i_verbose = false;
    i_level = 0;
    i_pause = false;
    i_auto = false;
    i_shutdown = false;
    i_reconnect = true;
    i_localhost = 0;
    i_gotconnect = false;
    i_connected = false;
    i_saving = false;
    dh_timer = 0;
#ifdef WIN32
    i_fork = isWinNT();
#else
    i_fork = true;
#endif

    i_programname = argv[0].RevAfter(DirSlash);
    i_config_file = "magick.ini";
    i_ResetTime = mDateTime::CurrentDateTime();

    ircsvchandler = NULL;
    logger = NULL;
    signalhandler = NULL;
    events = NULL;
    dcc = NULL;

    LoadLogMessages("DEFAULT");

    char buf[1024], *c;

    c = ACE_OS::getcwd(buf, 1024);
    if (c == NULL && errno)
    {
	LOG(LM_CRITICAL, "SYS_ERRORS/DIROPERROR", ("getcwd", ".", errno, strerror(errno)));
	return;
    }
    else
    {
	i_services_dir = buf;
    }
    CurrentState = Constructed;
    ETCB();
}

Magick::~Magick()
{
    BTCB();
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
    ETCB();
}

static bool firstrun;

int Magick::Init()
{
    BTCB();
    if (CurrentState != Constructed)
    {
	LOG(LM_ALERT, "ERROR/SEQUENCE", (Initialized, CurrentState));
	return MAGICK_RET_STATE;
    }

    unsigned int i = 0;
    int j = 0;
    int Result = MAGICK_RET_NORMAL;

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
	if (argv[i] [0U] == '-')
	{
	    argv[i].MakeLower();
	    if (argv[i] == "--dir")
	    {
		i++;
		if (i == argc || argv[i] [0U] == '-')
		{
		    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", ("--dir"));
		    return MAGICK_RET_ERROR;
		}
		i_services_dir = MakePath(argv[i]);
	    }
	    else if (argv[i] == "--config")
	    {
		i++;
		if (i == argc || argv[i] [0U] == '-')
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
		if (i == argc || argv[i] [0U] == '-')
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
#ifdef WIN32
	    else if (argv[i] == "--service")
	    {
		if (!isWinNT())
		{
		    LOG(LM_EMERGENCY, "COMMANDLINE/INCORRECT_OS", ("--service", "Windows NT"));
		    return MAGICK_RET_ERROR;
		}
		i++;
		if (i == argc || argv[i] [0U] == '-')
		{
		    LOG(LM_EMERGENCY, "COMMANDLINE/NEEDPARAM", ("--service"));
		    return MAGICK_RET_ERROR;
		}
		// Insertion & startup, run the gauntlet (parse config file)
		if (argv[i].IsSameAs("insert", true))
		    Result = MAGICK_RET_SERVICE_INSERT;
		else if (argv[i].IsSameAs("start", true))
		    Result = MAGICK_RET_SERVICE_START;

		// Stop & removal, we really don't care what in the config file.
		else if (argv[i].IsSameAs("stop", true))
		    return MAGICK_RET_SERVICE_STOP;
		else if (argv[i].IsSameAs("remove", true))
		    return MAGICK_RET_SERVICE_REMOVE;
		else
		{
		    LOG(LM_EMERGENCY, "COMMANDLINE/UNKNOWN_PARAM", (argv[i], "--service"));
		    return MAGICK_RET_ERROR;
		}
	    }
#endif
	    else if (argv[i] == "--help" || (argv[i] [1U] != '-' && (argv[i].Contains("?") || argv[i].Contains("h"))))
	    {
		dump_help();
		return MAGICK_RET_DIE;
	    }
	    else if (argv[i] == "--version" || (argv[i] [1U] != '-' && argv[i].Contains("v")))
	    {
		printf("%s v%s\n", FULLNAME.c_str(), version_string().c_str());
		return MAGICK_RET_DIE;
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
	RET(MAGICK_RET_DIE);
    }

    if (Result != MAGICK_RET_NORMAL)
	RET(Result);

    Result = doparamparse();
    if (Result != MAGICK_RET_NORMAL)
	RET(Result);

    firstrun = true;
    CurrentState = Initialized;
    RET(MAGICK_RET_NORMAL);
    ETCB();
}

int Magick::Start()
{
    BTCB();
    NFT("Magick::Start");
    if (CurrentState != Initialized && CurrentState != Stopped)
    {
	LOG(LM_ALERT, "ERROR/SEQUENCE", (Started, CurrentState));
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
    if (DoItAll())
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
	NLOG(LM_WARNING, "SYS_ERRORS/RUN_AS_ROOT");
    }

    // Need to shut down, it wont be carried over fork.
    // We will re-start it ASAP after fork.
#ifndef TEST_MODE
#ifndef WIN32
    if (i_fork && firstrun)
    {
	NLOG(LM_STARTUP, "COMMANDLINE/START_FORK");
	errno = 0;
	Result = ACE::daemonize(Services_Dir(), 0, ProgramName());
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
#else /* WIN32 */
    if (!DoItAll())
	NLOG(LM_STARTUP, "COMMANDLINE/START_FORK");
#endif
#endif

    // okay here we start setting up the ACE_Reactor and ACE_Event_Handler's
    signalhandler = new SignalHandler;

#if defined(SIGHUP) && (SIGHUP != 0)
    reactor().register_handler(SIGHUP, signalhandler);
#endif
#if defined(SIGINT) && (SIGINT != 0)
    reactor().register_handler(SIGINT, signalhandler);
#endif
#if defined(SIGQUIT) && (SIGQUIT != 0)
    reactor().register_handler(SIGQUIT, signalhandler);
#endif
#if defined(SIGILL) && (SIGILL != 0)
    reactor().register_handler(SIGILL, signalhandler);
#endif
#if defined(SIGTRAP) && (SIGTRAP != 0)
    reactor().register_handler(SIGTRAP, signalhandler);
#endif
#if defined(SIGABRT) && (SIGABRT != 0)
    reactor().register_handler(SIGABRT, signalhandler);
#endif
#if defined(SIGFPE) && (SIGFPE != 0)
    reactor().register_handler(SIGFPE, signalhandler);
#endif
#if defined(SIGSEGV) && (SIGSEGV != 0)
    reactor().register_handler(SIGSEGV, signalhandler);
#endif
#if defined(SIGBUS) && (SIGBUS != 0)
    reactor().register_handler(SIGBUS, signalhandler);
#endif
#if defined(SIGPIPE) && (SIGPIPE != 0)
    reactor().register_handler(SIGPIPE, signalhandler);
#endif
#if defined(SIGTERM) && (SIGTERM != 0)
    reactor().register_handler(SIGTERM, signalhandler);
#endif
#if defined(SIGTTIN) && (SIGTTIN != 0)
    reactor().register_handler(SIGTTIN, signalhandler);
#endif
#if defined(SIGTTOU) && (SIGTTOU != 0)
    reactor().register_handler(SIGTTOU, signalhandler);
#endif

#if 0

// NOFORK & Test Mode needs this to stop the task (CTRL-Z)
#if defined(SIGTSTP) && (SIGTSTP != 0)
    reactor().register_handler(SIGTSTP, signalhandler);
#endif

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
#if defined(SIGCHLD) && (SIGCHLD != 0)
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
	WLOCK((lck_Events));
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
    ETCB();
}

int Magick::Run()
{
    BTCB();
    NFT("Magick::Run");
    if (CurrentState != Started)
    {
	LOG(LM_ALERT, "ERROR/SEQUENCE", (Running, CurrentState));
	RET(MAGICK_RET_STATE);
    }

    // Use the reconnect handler to get a connection

    NLOG(LM_STARTUP, "COMMANDLINE/START_EVENTS");
    {
	WLOCK((lck_Events));
	if (events != NULL)
	    events->open(reinterpret_cast < void * > (this));
    }
    {
	WLOCK(("DCC"));
	if (dcc != NULL)
	    dcc->open(reinterpret_cast < void * > (this));
    }

//    reactor().schedule_timer(&Magick::instance().hh, 0, ACE_Time_Value(Magick::instance().config.Heartbeat_Time()));
    if (startup.Server_size())
	reactor().schedule_timer(&(Magick::instance().rh), 0, ACE_Time_Value::zero);
#ifdef TEST_MODE
    TestInput ti;

    ti.open(this);
#endif
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

#ifdef TEST_MODE
    ti.close();
#endif

    CurrentState = RunCompleted;
    RET(MAGICK_RET_NORMAL);
    ETCB();
}

int Magick::Stop()
{
    BTCB();
    NFT("Magick::Stop");
    if (CurrentState != Started && CurrentState != Running && CurrentState != RunCompleted)
    {
	LOG(LM_ALERT, "ERROR/SEQUENCE", (Stopped, CurrentState));
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

#if defined(SIGHUP) && (SIGHUP != 0)
    reactor().remove_handler(SIGHUP);
#endif
#if defined(SIGINT) && (SIGINT != 0)
    reactor().remove_handler(SIGINT);
#endif
#if defined(SIGQUIT) && (SIGQUIT != 0)
    reactor().remove_handler(SIGQUIT);
#endif
#if defined(SIGILL) && (SIGILL != 0)
    reactor().remove_handler(SIGILL);
#endif
#if defined(SIGTRAP) && (SIGTRAP != 0)
    reactor().remove_handler(SIGTRAP);
#endif
#if defined(SIGABRT) && (SIGABRT != 0)
    reactor().remove_handler(SIGABRT);
#endif
#if defined(SIGFPE) && (SIGFPE != 0)
    reactor().remove_handler(SIGFPE);
#endif
#if defined(SIGSEGV) && (SIGSEGV != 0)
    reactor().remove_handler(SIGSEGV);
#endif
#if defined(SIGBUS) && (SIGBUS != 0)
    reactor().remove_handler(SIGBUS);
#endif
#if defined(SIGPIPE) && (SIGPIPE != 0)
    reactor().remove_handler(SIGPIPE);
#endif
#if defined(SIGTERM) && (SIGTERM != 0)
    reactor().remove_handler(SIGTERM);
#endif
#if defined(SIGTTIN) && (SIGTTIN != 0)
    reactor().remove_handler(SIGTTIN);
#endif
#if defined(SIGTTOU) && (SIGTTOU != 0)
    reactor().remove_handler(SIGTTOU);
#endif

#if 0

// NOFORK & Test Mode needs this to stop the task (CTRL-Z)
#if defined(SIGTSTP) && (SIGTSTP != 0)
    reactor().remove_handler(SIGTSTP);
#endif

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
#if defined(SIGCHLD) && (SIGCHLD != 0)
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
	WLOCK((lck_Events));
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
    ETCB();
}

int Magick::Finish()
{
    BTCB();
    NFT("Magick::Finish");

    // Special case, we never completed Init, so we shouldnt
    // need to destroy anything, so we're already done ...
    if (CurrentState == Constructed)
	RET(MAGICK_RET_NORMAL);

    if (CurrentState != Initialized && CurrentState != Stopped)
    {
	LOG(LM_ALERT, "ERROR/SEQUENCE", (Finished, CurrentState));
	RET(MAGICK_RET_STATE);
    }

    CurrentState = Finished;
    RET(MAGICK_RET_NORMAL);
    ETCB();
}

mstring Magick::getMessage(const mstring & nick, const mstring & name)
{
    BTCB();
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
    ETCB();
}

mstring Magick::getMessageL(const mstring & lang, const mstring & name)
{
    BTCB();
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
	    retval = Messages[lang.UpperCase()] [name.UpperCase()];
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
	    retval = Messages[nickserv.DEF_Language().UpperCase()] [name.UpperCase()];
	    RET(retval);
	}
    }

    // Otherwise just try and find it in the DEFAULTs.
    CP(("Trying HARD-CODED language ..."));
    {
	RLOCK(("Messages", "DEFAULT", name.UpperCase()));
	if (Messages["DEFAULT"].find(name.UpperCase()) != Messages["DEFAULT"].end())
	{
	    retval = Messages["DEFAULT"] [name.UpperCase()];
	    RET(retval);
	}
    }
    LOG(LM_ERROR, "ERROR/NOLANGTOKEN", (name.UpperCase(), lang.UpperCase()));
    RET(retval);
    ETCB();
}

mstring Magick::getLogMessage(const mstring & name)
{
    BTCB();
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
    ETCB();
}

vector<mstring> HelpText::get(const mstring &nick) const
{
    FT("HelpText::get", (nick));
    vector<mstring> retval;

    vector<triplet<mstring,mstring,mstring> >::const_iterator iter;
    unsigned int i;
    for (iter=lines.begin(); iter!=lines.end(); iter++)
    {
	bool sendline;
	if (!iter->first.empty())
	{
	    sendline = false;
	    for (i = 1; !sendline && i <= iter->first.WordCount(" "); i++)
	    {
		mstring comm = iter->first.ExtractWord(i, " ");
		if (Magick::instance().commserv.IsList(comm) && Magick::instance().commserv.GetList(comm)->IsOn(nick))
		    sendline = true;
	    }
	}
	else
	    sendline = true;

	if (!iter->second.empty())
	{
	    for (i = 1; sendline && i <= iter->second.WordCount(" "); i++)
	    {
		mstring comm = iter->second.ExtractWord(i, " ");
		if (Magick::instance().commserv.IsList(comm) && Magick::instance().commserv.GetList(comm)->IsOn(nick))
		    sendline = false;
	    }
	}

	if (sendline)
	    retval.push_back(iter->third);
    }
    NRET(vector<mstring>, retval);
}

vector < mstring > Magick::getHelp(const mstring & nick, const mstring & name)
{
    BTCB();
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
	    Help[language][section].add(i->second.ExtractWord(1, ":", false),
					i->second.ExtractWord(2, ":", false),
					i->second.After(":", 2));
	}
	MCE(Help.size());
	if (tmp.size())
	{
	    LOG(LM_INFO, "OTHER/LOAD_HELP", (language.UpperCase().c_str()));
	    CP(("Help %s was loaded into memory.", language.c_str()));
	}
    }

    {
	mstring Uname(name.UpperCase());
	RLOCK(("Help", language, Uname));
	if (Help.find(language) != Help.end() && Help[language].find(Uname) != Help[language].end())
	    helptext = Help[language][Uname].get(nick);
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
    ETCB();
}

void Magick::dump_help() const
{
    BTCB();
    // This needs to be re-written.
    cout << "\n" << FULLNAME + " - " + HOMEPAGE + "\n" <<
	"    (c) 1997-2002 Preston A. Elder <prez@magick.tm>\n" <<
	"    (c) 1998-2002 William King <ungod@magick.tm>\n" << "\n" <<
	"Syntax: " << i_programname << " [options]\n" << "\n" <<
	"--help             -h      Help output (summary of the below).\n" <<
	"--version          -v      Display the Magick version string.\n" <<
	"--dir X                    Set the initial services directory.\n" <<
	"--config X                 Set the name of the config file.\n" <<
	"--nofork                   Do not become a daemon/service process.\n" <<
#ifdef WIN32
	"--service X                Manipulate Magick's NT Service settings.\n" <<
	"                               Values are: insert, start, stop, remove.\n" <<
#endif
#ifdef CONVERT
	"--convert X                Convert another version of services databases\n" <<
	"                           to Magick II format, where X is the type of\n" <<
	"                           database to convert.  Currently recognized:\n" <<
	"                               magick (1.4), esper (4.4.8), epona (1.4.7)\n" <<
#endif
#ifdef MAGICK_TRACE_WORKS
	"--trace X:Y                Set the trace level on startup, equivilant of\n" <<
	"                           using the OperServ TRACE SET command while\n" <<
	"                           running, where X is the trace type (or ALL),\n" <<
	"                           and Y is the trace level in hex.\n" <<
#endif
	"--name X           -N      Override [STARTUP/SERVER_NAME] to X.\n" <<
	"--desc X           -D      Override [STARTUP/SERVER_DESC] to X.\n" <<
	"--user X           -U      Override [STARTUP/SERVICES_USER] to X.\n" <<
	"--host X           -H      Override [STARTUP/SERVICES_HOST] to X.\n" <<
	"--ownuser          -o      Override [STARTUP/OWNUSER] to true.\n" <<
	"--level X          -l      Override [STARTUP/LEVEL] to X.\n" <<
	"--lagtime X        -g      Override [STARTUP/LAGTIME] to X.\n" <<
	"--umask            -u      Override [FILES/UMASK] to X.\n" <<
	"--protocol X       -P      Override [FILES/PROTOCOL] to X.\n" <<
	"--verbose          -V      Override [FILES/VERBOSE] to true.\n" <<
	"--log X            -L      Override [FILES/LOGFILE] to X.\n" <<
	"--logchan X        -C      Override [FILES/LOGCHAN] to X.\n" <<
	"--dbase X          -d      Override [FILES/DATABASE] to X.\n" <<
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
	"--ignore X         -I      Override [OPERSERV/IGNORE_METHOD] to X.\n" << "\n" <<
	"For more help on the usage of " + mstring(PACKAGE) + ", please browse the docs directory.\n" <<
	"This released under the Artistic License v2.0 or better.  Please see the\n" <<
	"\"COPYING\" file for more details.\n\n";

    ETCB();
}

class AddToUCMap
{
    map<mstring, mstring> &ucmap;
public:
    AddToUCMap(map<mstring, mstring> &m) : ucmap(m) {}
    void operator()(const pair<mstring, mstring> &in)
    {
	ucmap[in.first.UpperCase()] = in.second;
    }
};

void Magick::LoadInternalMessages()
{
    BTCB();
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
	for_each(tmp.begin(), tmp.end(), AddToUCMap(Messages["DEFAULT"]));
	MCE(Messages.size());
    }
    ETCB();
}

bool Magick::LoadExternalMessages(const mstring & language)
{
    BTCB();
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
	    for_each(tmp.begin(), tmp.end(), AddToUCMap(Messages[language.UpperCase()]));
	    MCE(Messages.size());
	    RET(true);
	}
    }
    RET(false);
    ETCB();
}

bool Magick::LoadLogMessages(const mstring & language)
{
    BTCB();
    FT("Magick::LoadLogMessages", (language));
    // use the previously created name array to get the names to load

    WLOCK(("LogMessages"));
    LogMessages.clear();
    MCB(LogMessages.size());
    {
	vector < mstring > lfo;
	unsigned int i;

	for (i = 0; i < def_logent; i++)
	    lfo.push_back(def_log[i]);

	mConfigEngine fconf;

	fconf.LoadFromArray(lfo);

	map < mstring, mstring > tmp = fconf.GetMap();
	for_each(tmp.begin(), tmp.end(), AddToUCMap(LogMessages));
	CP(("After DEFAULT - LogMessages = %d", LogMessages.size()));
    }
    if (language != "DEFAULT" && mFile::Exists(files.Langdir() + DirSlash + language.LowerCase() + ".lfo"))
    {
	mConfigEngine fconf(files.Langdir() + DirSlash + language.LowerCase() + ".lfo");

	map < mstring, mstring > tmp = fconf.GetMap();
	for_each(tmp.begin(), tmp.end(), AddToUCMap(LogMessages));
    }
    MCE(LogMessages.size());

    if (LogMessages.size())
    {
	RET(true);
    }
    RET(false);
    ETCB();
}

bool Magick::UnloadExternalMessages(const mstring & language)
{
    BTCB();
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
    ETCB();
}

bool Magick::UnloadHelp(const mstring & language)
{
    BTCB();
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
    ETCB();
}

int Magick::doparamparse()
{
    BTCB();
    NFT("Magick::doparamparse");
    mstring temp;
    int argc = argv.size(), i;

    for (i = 1; i < argc; i++)
    {
	if (argv[i] [0U] == '-')
	{
	    bool ArgUsed = false;

	    if (argv[i] [1U] == '-')
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
    ETCB();
}

bool Magick::paramlong(const mstring & first, const mstring & second)
{
    BTCB();
    FT("Magick::paramlong", (first, second));
    if (first == "--dir" || first == "--config" || first == "--trace" || first == "--service")
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
	if (second.c_str() != files.protocol)
	{
	    files.protocol = second.c_str();
	    if (!server.proto.Set(files.Protocol()))
	    {
		LOG(LM_ERROR, "COMMANDLINE/UNKNOWN_PROTO", (second));
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
    else if (first == "--nofork")
    {
	i_fork = false;
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
    ETCB();
}

bool Magick::paramshort(const mstring & first, const mstring & second)
{
    BTCB();
    FT("Magick::paramshort", (first, second));
    bool ArgUsed = false;

    for (unsigned int i = 1; i < first.length(); i++)
    {
	if (first[i] == '?' || first[i] == 'h' || first[i] == 'v')
	{
	    // Already handled
	}
	else if (first[i] == 'N')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--name", second);
	}
	else if (first[i] == 'D')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--desc", second);
	}
	else if (first[i] == 'U')
	{
	    if (ArgUsed)
	    {
		NLOG(LM_EMERGENCY, "COMMANDLINE/ONEOPTION");
	    }
	    else
		ArgUsed = paramlong("--user", second);
	}
	else if (first[i] == 'H')
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
	else if (first[i] == 'V')
	{
	    ArgUsed = paramlong("--verbose", second);
	}
	else if (first[i] == 'u')
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
	else if (first[i] == 'd')
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
    ETCB();
}

bool Magick::get_config_values()
{
    BTCB();
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

	startup.servers.clear();
	do
	{
	    mstring lookfor = "REMOTE_";

	    lookfor << i;
	    in.Read(ts_Startup + lookfor, value_mstring, "");
	    if (!value_mstring.empty())
	    {
		mstring tmp[5];

		tmp[0] = value_mstring.ExtractWord(1, "|");
		tmp[1] = value_mstring.ExtractWord(2, "|");
		tmp[2] = value_mstring.ExtractWord(3, "|");
		tmp[3] = value_mstring.ExtractWord(4, "|");
		tmp[4] = value_mstring.ExtractWord(5, "|");
		if (value_mstring.WordCount("|") == 5 && tmp[1].IsNumber() && tmp[3].IsNumber() && tmp[4].IsNumber() &&
		    atoi(tmp[1]) > 0 && atoi(tmp[3]) > 0 && atol(tmp[4]) > 0)
		{
		    Connection_t svr(tmp[0].LowerCase(), static_cast<unsigned short>(atoui(tmp[1])),
				tmp[2], atoui(tmp[3]), atoul(tmp[4]));
		    startup.servers.push_back(svr);
		}
		else
		{
		    LOG(LM_WARNING, "COMMANDLINE/CFG_SYNTAX", (ts_Startup + lookfor));
		    RET(false);
		}
	    }
	    i++;
	} while (!value_mstring.empty());

	// Sort the entries by priority, and then re-write the priorities to be sequencial.
//	stable_sort(startup.servers.begin(), startup.servers.end(), Connection_t::PrioritySort());
	sort(startup.servers.begin(), startup.servers.end(), Connection_t::PrioritySort());
	unsigned int lastPrio = 0;
	vector<Connection_t>::iterator iter;
	for (i=0, iter=startup.servers.begin(); iter!=startup.servers.end(); iter++)
	{
	    if (iter->Priority() != lastPrio)
		i++;
	    lastPrio = iter->Priority();
	    iter->Priority(i);
	}
	
    }
    if (CurrentServer().first.empty() || !startup.IsServer(CurrentServer().first, CurrentServer().second))
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
		if (value_mstring.Contains("|"))
		{
		    mstring tmp[2];

		    tmp[0] = value_mstring.ExtractWord(1, "|");
		    tmp[1] = value_mstring.ExtractWord(2, "|");
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
    if (CurrentServer().first.empty() || !startup.IsAllowed(server.OurUplink(), startup.Server_Name()))
	reconnect = true;

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
		    server.sraw(((server.proto.Tokens() &&
				  !server.proto.GetNonToken("ISON").empty()) ? server.proto.GetNonToken("ISON") :
									       mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK((lck_Server, "WaitIsOn"));
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
		    server.sraw(((server.proto.Tokens() &&
				  !server.proto.GetNonToken("ISON").empty()) ? server.proto.GetNonToken("ISON") :
									       mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK((lck_Server, "WaitIsOn"));
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
		    server.sraw(((server.proto.Tokens() &&
				  !server.proto.GetNonToken("ISON").empty()) ? server.proto.GetNonToken("ISON") :
									       mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK((lck_Server, "WaitIsOn"));
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
		    server.sraw(((server.proto.Tokens() &&
				  !server.proto.GetNonToken("ISON").empty()) ? server.proto.GetNonToken("ISON") :
				  mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK((lck_Server, "WaitIsOn"));
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
		    server.sraw(((server.proto.Tokens() &&
				  !server.proto.GetNonToken("ISON").empty()) ? server.proto.GetNonToken("ISON") :
									       mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK((lck_Server, "WaitIsOn"));
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
		    server.sraw(((server.proto.Tokens() &&
				  !server.proto.GetNonToken("ISON").empty()) ? server.proto.GetNonToken("ISON") :
									       mstring("ISON")) + " :" + isonstr);
		    isonstr.erase();
		}
		WLOCK((lck_Server, "WaitIsOn"));
		server.WaitIsOn.insert(servmsg.names.ExtractWord(i + 1, " "));
		isonstr += servmsg.names.ExtractWord(i + 1, " ") + " ";
	    }
	}
    }

    in.Read(ts_Services + "SHOWSYNC", servmsg.showsync, true);
    in.Read(ts_Services + "QUIT_MESSAGE", startup.services_quitmsg, "");

    if (!isonstr.empty())
	server.sraw(((server.proto.Tokens() &&
		      !server.proto.GetNonToken("ISON").empty()) ? server.proto.GetNonToken("ISON") :
								   mstring("ISON")) + " :" + isonstr);

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

    in.Read(ts_Files + "PROTOCOL", value_mstring, "ircd/ircd.rfc1459.ini");
    if (value_mstring != files.protocol)
    {
	files.protocol = value_mstring;
	bool rv = server.proto.Set(files.Protocol());

	if (!rv)
	{
	    LOG(LM_EMERGENCY, "COMMANDLINE/NO_CFG_FILE", (value_mstring));
	    RET(false);
	}
    }

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

    in.Read(ts_Files + "MEMOATTACH_SIZE", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	files.memoattachsize = FromHumanSpace(value_mstring);
    else
	files.memoattachsize = FromHumanSpace("0");
    in.Read(ts_Files + "PICTURE", files.picture, "files/pic");
    MakeDirectory(files.picture.c_str());

    in.Read(ts_Files + "PICTURE_SIZE", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	files.picturesize = FromHumanSpace(value_mstring);
    else
	files.picturesize = FromHumanSpace("0");
    in.Read(ts_Files + "PUBLIC", files.i_public, "files/public");
    MakeDirectory(files.i_public.c_str());

    in.Read(ts_Files + "PUBLIC_SIZE", value_mstring, "0");
    if (FromHumanSpace(value_mstring))
	files.publicsize = FromHumanSpace(value_mstring);
    else
	files.publicsize = FromHumanSpace("0");
    in.Read(ts_Files + "TEMPDIR", files.tempdir, "files/temp");
    MakeDirectory(files.tempdir.c_str());

    in.Read(ts_Files + "TEMPDIR_SIZE", value_mstring, "0");
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
    in.Read(ts_ChanServ + "OVR_PER_NICK", chanserv.ovr_per_nick, "");
    in.Read(ts_ChanServ + "MAX_MESSAGES", chanserv.max_messages, 15U);
    in.Read(ts_ChanServ + "OVR_MESSAGES", chanserv.ovr_messages, "");
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
    in.Read(ts_ChanServ + "OVR_LOGON", commserv.ovr_logon, "");
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
    in.Read(ts_CommServ + "OVR_CS_OP", commserv.ovr_cs_halfop, "ADMIN");
    in.Read(ts_CommServ + "OVR_CS_VOICE", commserv.ovr_cs_voice, "OPER");
    in.Read(ts_CommServ + "OVR_CS_INVITE", commserv.ovr_cs_invite, "SOP");
    in.Read(ts_CommServ + "OVR_CS_KICK", commserv.ovr_cs_kick, "SOP");
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
    commserv.ovr_cs_kick.MakeUpper();
    commserv.ovr_cs_unban.MakeUpper();
    commserv.ovr_cs_clear.MakeUpper();
    AddCommands();

    map_entry < Committee_t > comm;
    if (commserv.IsList(commserv.sadmin.Name))
    {
	comm = commserv.GetList(commserv.sadmin.Name);
	MLOCK((lck_CommServ, lck_list, commserv.sadmin.Name, "member"));
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
	MLOCK((lck_CommServ, lck_list, commserv.sadmin.Name, "member"));
	for (i = 1; i <= operserv.services_admin.WordCount(", "); i++)
	    comm->insert(operserv.services_admin.ExtractWord(i, ", "), operserv.FirstName());
    }

    if (commserv.IsList(commserv.sop.Name))
	comm = commserv.GetList(commserv.sop.Name);
    else
    {
	comm = map_entry < Committee_t > (new Committee_t(commserv.sop.Name, *(commserv.GetList(commserv.sadmin.Name)),
							  "Services Operators"));
	commserv.AddList(comm);
    }
    comm->Secure(commserv.sop.Secure);
    comm->Private(commserv.sop.Private);
    comm->OpenMemos(commserv.sop.OpenMemos);

    if (commserv.IsList(commserv.admin.Name))
	comm = commserv.GetList(commserv.admin.Name);
    else
    {
	comm = map_entry < Committee_t > (new Committee_t(commserv.admin.Name, *(commserv.GetList(commserv.sadmin.Name)),
							  "Server Administrators"));
	commserv.AddList(comm);
    }
    comm->Secure(commserv.admin.Secure);
    comm->Private(commserv.admin.Private);
    comm->OpenMemos(commserv.admin.OpenMemos);

    if (commserv.IsList(commserv.oper.Name))
	comm = commserv.GetList(commserv.oper.Name);
    else
    {
	comm = map_entry < Committee_t > (new Committee_t(commserv.oper.Name, *(commserv.GetList(commserv.admin.Name)),
							  "Server Operators"));
	commserv.AddList(comm);
    }
    comm->Secure(commserv.oper.Secure);
    comm->Private(commserv.oper.Private);
    comm->OpenMemos(commserv.oper.OpenMemos);

    if (commserv.IsList(commserv.all.Name))
    {
	comm = commserv.GetList(commserv.all.Name);
	MLOCK((lck_CommServ, lck_list, commserv.all.Name, "member"));
	while (comm->size())
	{
	    comm->member = comm->begin();
	    comm->erase();
	}
    }
    else
    {
	comm = map_entry < Committee_t > (new Committee_t(commserv.all.Name, *(commserv.GetList(commserv.admin.Name)),
							  "All Users"));
	commserv.AddList(comm);
    }
    comm->Secure(false);
    comm->Private(true);
    comm->OpenMemos(false);

    if (commserv.IsList(commserv.regd.Name))
    {
	comm = commserv.GetList(commserv.regd.Name);
	MLOCK((lck_CommServ, lck_list, commserv.regd.Name, "member"));
	while (comm->size())
	{
	    comm->member = comm->begin();
	    comm->erase();
	}
    }
    else
    {
	comm = map_entry < Committee_t >(new Committee_t(commserv.regd.Name, *(commserv.GetList(commserv.sop.Name)),
							 "Registered Users"));
	commserv.AddList(comm);
    }
    comm->Secure(false);
    comm->Private(true);
    comm->OpenMemos(false);

    if (reconnect && Connected())
    {
	server.raw(((server.proto.Tokens() &&
		     !server.proto.GetNonToken("ERROR").empty()) ? server.proto.GetNonToken("ERROR") :
		     mstring("ERROR")) + " " + " :Closing Link: Configuration reload required restart!");
	Disconnect();
    }

    DumpE();
    CP(("%s read and loaded to live configuration.", i_config_file.c_str()));
    RET(true);
    ETCB();
}

int SignalHandler::handle_signal(int signum, siginfo_t * si, ucontext_t * uctx)
{
    BTCB();
    static_cast < void > (si);
    static_cast < void > (uctx);

    // No trace, screws with LastFunc...
    //FT("SignalHandler::handle_signal", (signum, "(siginfo_t *) si", "(ucontext_t *) uctx"));
    ThreadID *tid;

    // todo: fill this sucker in
    switch (signum)
    {
#if defined(SIGPIPE) && (SIGPIPE!=0)
    case SIGPIPE:
	{
	    vector < ThreadID * > ids = mThread::findall();
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
		    RLOCK((lck_IrcSvcHandler));
		    if (Magick::instance().ircsvchandler != NULL)
			thr_mgr = & Magick::instance().ircsvchandler->tm;
		    else
			thr_mgr = & Magick::instance().thr_mgr();
		}
		break;
	    case Heartbeat_Handler::H_IrcServer:
		{
		    RLOCK((lck_IrcSvcHandler));
		    if (Magick::instance().ircsvchandler != NULL)
			thr_mgr = Magick::instance().ircsvchandler->thr_mgr();
		}
		if (thr_mgr == NULL)
		    thr_mgr = & Magick::instance().thr_mgr();
		Magick::instance().Disconnect();
		if (Magick::instance().dh_timer > 0)
		    Magick::instance().reactor().cancel_timer(Magick::instance().dh_timer);
		Magick::instance().dh_timer = 0;
		break;
	    case Heartbeat_Handler::H_Events:
		{
		    WLOCK((lck_Events));
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
			thr_mgr = & Magick::instance().thr_mgr();
		    Magick::instance().events = new EventTask(&Magick::instance().thr_mgr());
		    Magick::instance().events->open(reinterpret_cast < void * > (&Magick::instance()));
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
			thr_mgr = & Magick::instance().thr_mgr();
		    Magick::instance().dcc = new DccMap(&Magick::instance().thr_mgr());
		    Magick::instance().dcc->open(reinterpret_cast < void * > (&Magick::instance()));
		}
		break;
	    case Heartbeat_Handler::H_Main:	// Its a REAL SIGABRT ...
		{
		    vector < ThreadID * > ids = mThread::findall();
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
	    RLOCK((lck_Events));
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
	    vector < ThreadID * > ids = mThread::findall();
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
    ETCB();
}

bool Magick::ValidateLogger(ACE_Log_Msg * inst) const
{
    BTCB();
    // Removed so it stops appearing as LastFunction
    // FT("Magick::ValidateLogger", ("(ACE_Log_Msg *) instance"));

    if (inst == NULL)
	return false;

    if (inst->msg_callback() != logger)
    {
	inst->open(ProgramName().c_str());
	inst->msg_callback(logger);
    }

    if (logger != NULL)
    {
	if (!(inst->flags() & ACE_Log_Msg::MSG_CALLBACK))
	{
	    inst->set_flags(ACE_Log_Msg::MSG_CALLBACK);
	    inst->clr_flags(ACE_Log_Msg::STDERR);
	}
    }
    else
    {
	if (!(inst->flags() & ACE_Log_Msg::STDERR))
	{
	    inst->set_flags(ACE_Log_Msg::STDERR);
	    inst->clr_flags(ACE_Log_Msg::MSG_CALLBACK);
	}
    }
    return true;
    ETCB();
}

bool Magick::ActivateLogger()
{
    BTCB();
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
    ETCB();
}

void Magick::DeactivateLogger()
{
    BTCB();
    NFT("Magick::DeactivateLogger");
    if (logger != NULL)
	delete logger;

    logger = NULL;
    ETCB();
}

/*
void Magick::handle(const mstring & server, const mstring & command, const mstring & functionname)
{
    BTCB();
    pair<mstring,mstring> data=pair<mstring,mstring>(server, command);
    handlermap[data].insert(handlermap[data].begin(),functionname);
    ETCB();
}

void Magick::stophandling(const mstring & server, const mstring & command, const mstring & functionname)
{
    BTCB();
    pair<mstring,mstring> data=pair<mstring,mstring>(server, command);
    if(checkifhandled(server,command)&&find(handlermap[data].begin(),handlermap[data].end(),functionname)!=handlermap[data].end())
	handlermap[data].erase(find(handlermap[data].begin(),handlermap[data].end(),functionname));
    ETCB();
}

bool Magick::checkifhandled(const mstring & server, const mstring & command)
{
    BTCB();
   pair<mstring,mstring> data=pair<mstring,mstring>(server, command);
   if(handlermap.find(data)!=handlermap.end())
   {
       return !(handlermap[data].empty());
   }
   else
       return false;
    ETCB();
}

void Magick::doscripthandle(const mstring& server, const mstring& command, const mstring& data)
{
    BTCB();
    if(checkifhandled(server,command)==true)
    {
	//todo
    }
    ETCB();
}
*/

bool Magick::startup_t::IsServer(const mstring & svr, unsigned short port) const
{
    BTCB();
    FT("Magick::startup_t::IsServer", (svr, port));

    vector<Connection_t>::const_iterator iter;
    RLOCK(("Startup", "Servers"));
    for (iter=servers.begin(); iter!=servers.end(); iter++)
    {
	if (iter->Name().IsSameAs(svr, true) && iter->Port() == port)
	    RET(true);
    }

    RET(false);
    ETCB();
}

Connection_t Magick::startup_t::Server(const mstring & svr, unsigned short port) const
{
    BTCB();
    FT("Magick::startup_t::Server", (svr, port));

    vector<Connection_t>::const_iterator iter;
    RLOCK(("Startup", "Servers"));
    for (iter=servers.begin(); iter!=servers.end(); iter++)
    {
	if (iter->Name().IsSameAs(svr, true) && iter->Port() == port)
	    NRET(Connection_t, *iter);
    }

    Connection_t tmp;
    NRET(Connection_t, tmp);
    ETCB();
}

vector<Connection_t> Magick::startup_t::ServerList(const mstring & svr) const
{
    BTCB();
    FT("Magick::startup_t::ServerList", (svr));

    vector<Connection_t> retval;
    vector<Connection_t>::const_iterator iter;

    RLOCK(("Startup", "Servers"));
    for (iter=servers.begin(); iter!=servers.end(); iter++)
    {
	if (iter->Name().IsSameAs(svr, true))
	    retval.push_back(*iter);
    }

    NRET(vector< Connection_t >, retval);
    ETCB();
}

vector <Connection_t> Magick::startup_t::PriorityList(unsigned int pri) const
{
    BTCB();
    FT("Magick::startup_t::PriorityList", (pri));

    vector<Connection_t> retval;
    vector<Connection_t>::const_iterator iter;

    RLOCK(("Startup", "Servers"));
    for (iter=servers.begin(); iter!=servers.end(); iter++)
    {
	if (iter->Priority() == pri)
	    retval.push_back(*iter);
	else if (iter->Priority() > pri)
	    break;
    }

    NRET(vector< Connection_t >, retval);
    ETCB();
}

bool Magick::startup_t::IsAllowed(const mstring & svr, const mstring & uplink) const
{
    BTCB();
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
    ETCB();
}

vector < mstring > Magick::startup_t::Allow(const mstring & svr) const
{
    BTCB();
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
    ETCB();
}

vector < mstring > Magick::startup_t::AllowList() const
{
    BTCB();
    NFT("Magick::startup_t::AllowList");
    vector < mstring > list;

    map < mstring, vector < mstring > >::const_iterator i;
    RLOCK(("Startup", "Allows"));
    for (i = allows.begin(); i != allows.end(); i++)
    {
	list.push_back(i->first);
    }

    NRET(vector < mstring >, list);
    ETCB();
}

pair < mstring, mstring > Magick::GetKeys() const
{
    BTCB();
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
	}
	else
	    LOG(LM_ERROR, "ERROR/KEY_CORRUPT", (files.KeyFile()));
    }
#endif
    NRET(pair < mstring_mstring >, retval);
    ETCB();
}

void Magick::Disconnect(const bool reconnect)
{
    BTCB();
    FT("Magick::Disconnect", (reconnect));
    Magick::instance().server.SignOffAll(startup.Services_Quitmsg());
    MCB(i_reconnect);
    i_reconnect = reconnect;
    MCE(i_reconnect);
    server.sraw("QUIT :" + startup.Services_Quitmsg());
    {
	RLOCK((lck_IrcSvcHandler));
	if (ircsvchandler != NULL)
	{
	    if (Magick::instance().hh.ThreadType() != Heartbeat_Handler::H_IrcServer)
	    {
		if (dh_timer == 0)
		{
		    ACE_Thread_Manager *thrmgr = Magick::instance().ircsvchandler->thr_mgr();

		    if (thrmgr == NULL)
			thrmgr = & Magick::instance().thr_mgr();
#if defined(SIGIOT) && (SIGIOT != 0)
		    thrmgr->kill_task(Magick::instance().ircsvchandler, SIGIOT);
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
    ETCB();
}

void Magick::send(const mstring & in) const
{
    BTCB();
    RLOCK((lck_IrcSvcHandler));
    if (ircsvchandler != NULL)
	ircsvchandler->send(in);
    ETCB();
}

SXP::Tag Magick::tag_Magick("Magick");

void Magick::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
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
    ETCB();
}

void Magick::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

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
    ETCB();
}

void Magick::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

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
    ETCB();
}

void Magick::save_databases()
{
    BTCB();
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
		NLOG(LM_ERROR, "SYS_ERRORS/STAGE_FAIL");
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
		    NLOG(LM_ERROR, "SYS_ERRORS/STAGE_FAIL");
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
    ETCB();
}

void Magick::load_databases()
{
    BTCB();
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
		    NLOG(LM_ERROR, "SYS_ERRORS/STAGE_FAIL");
	    }
	}

	if (ls->GetTag() & STAGE_TAG_COMPRESS)
	{
	    zs = new CompressStage(*ls, 0);
	    if (zs != NULL && zs->Validate())
		ls = zs;
	    else
		NLOG(LM_ERROR, "SYS_ERRORS/STAGE_FAIL");
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
    ETCB();
}

set < mstring > Magick::LNG_Loaded() const
{
    BTCB();
    set < mstring > retval;
    map < mstring, map < mstring, mstring > >::const_iterator i;
    RLOCK(("Messages"));
    for (i = Messages.begin(); i != Messages.end(); i++)
    {
	retval.insert(i->first);
    }
    return retval;
    ETCB();
}

size_t Magick::LNG_Usage(const mstring & lang) const
{
    BTCB();
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
    ETCB();
}

set < mstring > Magick::HLP_Loaded() const
{
    BTCB();
    set < mstring > retval;
    map < mstring, map < mstring, HelpText > >::const_iterator i;
    RLOCK(("Help"));
    for (i = Help.begin(); i != Help.end(); i++)
    {
	retval.insert(i->first);
    }
    return retval;
    ETCB();
}

size_t Magick::HLP_Usage(const mstring & lang) const
{
    BTCB();
    size_t retval = 0;

    map < mstring, map < mstring, HelpText > >::const_iterator i;
    map < mstring, HelpText >::const_iterator j;

    RLOCK(("Help"));
    i = Help.find(lang.UpperCase());
    if (i != Help.end())
    {
	retval += i->first.capacity();
	RLOCK2(("Help", lang.UpperCase()));
	for (j = i->second.begin(); j != i->second.end(); j++)
	{
	    retval += j->first.capacity();
	    retval += j->second.Usage();
	}
    }

    return retval;
    ETCB();
}

size_t Magick::LFO_Usage() const
{
    BTCB();
    size_t retval = 0;

    map < mstring, mstring >::const_iterator i;
    RLOCK(("LogMessages"));
    for (i = LogMessages.begin(); i != LogMessages.end(); i++)
    {
	retval += i->first.capacity();
	retval += i->second.capacity();
    }

    return retval;
    ETCB();
}

void Magick::DumpB() const
{
    BTCB();
    MB(0,
       (argv.size(), Messages.size(), Help.size(), LogMessages.size(), handlermap.size(), i_verbose, i_services_dir,
	i_config_file, i_programname, i_ResetTime, i_level, i_pause, i_auto, i_shutdown, i_reconnect, i_localhost));
    MB(16, (i_gotconnect, i_currentserver.first, i_currentserver.second, i_connected, i_saving));
    ETCB();
}

void Magick::DumpE() const
{
    BTCB();
    ME(0,
       (argv.size(), Messages.size(), Help.size(), LogMessages.size(), handlermap.size(), i_verbose, i_services_dir,
	i_config_file, i_programname, i_ResetTime, i_level, i_pause, i_auto, i_shutdown, i_reconnect, i_localhost));
    ME(16, (i_gotconnect, i_currentserver.first, i_currentserver.second, i_connected, i_saving));
    ETCB();
}
