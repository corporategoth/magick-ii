#include "pch.h"
#ifdef _MSC_VER
#pragma hdrstop
#endif
// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================

#include "magick.h"
#include "EscLexer.hpp"
#include "EscParser.hpp"
#include "lockable.h"
#include "utils.h"
#include "mstream.h"

//#define ACE_DEBUGGING

wxLogStderr *logger;
mDateTime StartTime;
Magick *Parent;

Magick::Magick(int inargc, char **inargv)
{
    Parent=this;
    FT("Magick::Magick", (inargc, "(char **) inargv"));
    i_shutdown = false;
    i_services_dir=wxGetCwd();
    i_config_file="magick.ini";
    for(int i=0;i<inargc;i++)
	argv.push_back(inargv[i]);

    i_level=0;
    i_reconnect=true;
    loggertask.open();
    events.open();
}

int Magick::Start()
{
    unsigned int i;
    int Result;
    // this is our main routine, when it leaves here, this sucker's done.
#ifdef ACE_DEBUGGING
    ACE_Log_Msg::instance()->open(PRODUCT);
    ACE_Log_Msg::enable_debug_messages();
#endif

    // We log to STDERR until we find our logfile...
    logger=new wxLogStderr();

    // more stuff to do
    i_programname=argv[0].RevAfter("/");
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
		    // use static errors here because conf directory is not known yet
		    wxLogFatal("--dir requires a paramter.");
		}
		i_services_dir=argv[i];
	    }
	    else if(argv[i]=="--config")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    // use static errors here because conf directory is not known yet
		    wxLogFatal("--config requires a paramter.");
		}
		i_config_file=argv[i];
	    }
	    else if(argv[i]=="--trace")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    // use static errors here because conf directory is not known yet
		    wxLogFatal("--trace requires a paramter.");
		}
		if(!argv[i].Contains(":"))
		{
		    // use static errors here because conf directory is not known yet
		    wxLogFatal("trace level must be in format of TYPE:LEVEL.");
		}
		unsigned short level = makehex(argv[i].After(":"));
		if (level==0)
		{
		    wxLogError("Zero or error parsing LEVEL, ignoring.");
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
	    else if(argv[i]=="--help" ||
		(argv[i][1U]!='-' && argv[i].Contains("?")))
    	    {
		dump_help(argv[0]);
		return MAGICK_RET_NORMAL;
	    }
	}
    }
    NFT("Magick::Start");
    if (chdir (i_services_dir) < 0)
    {
        perror (i_services_dir);
#ifdef WIN32
        WSACleanup ();
#endif
        RET(MAGICK_RET_INVALID_SERVICES_DIR);
    }

    if (!get_config_values())
    {
	wxLogFatal("Could not read magick config file %s.", i_config_file.c_str());
    }

    if(i_shutdown==true)
	wxLogFatal("CONFIG: [Startup] STOP code received.");

    Result=doparamparse();
    if(Result!=MAGICK_RET_NORMAL)
	RET(Result);

    FILE *logfile = fopen((i_services_dir+DirSlash+files.Logfile()).c_str(), "a");
    logger->ChangeFile(logfile);

    // load the local messages database and internal "default messages"
    // the external messages are part of a separate ini called english.lng (both local and global can be done here too)
    LoadInternalMessages();
    LoadExternalMessages();


#if 0
#ifndef WIN32
    if(live==false)
    {
	if ((i = fork ()) < 0)
	{
	    //log_perror ("fork()");
            RET(1);
        }
        else if (i != 0)
            RET(0);
        if (setpgid (0, 0) < 0)
        {
            //log_perror ("setpgid()");
            RET(1);
        }
    }
#endif
#endif
    wxFile pidfile;
    pidfile.Create(files.Pidfile().Strip(mstring::stBoth),true);
    if(pidfile.IsOpened())
    {
	mstring dummystring;
	dummystring<<getpid();
	pidfile.Write(dummystring);
	pidfile.Close();
    }
    /*else
	log_perror ("Warning: cannot write to PID file %s", files.Pidfile());*/

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

    // etc.

    // calibrate the threshholds.
    //
    // register 250 nicks and 1000 channels (to random nicknames in the nick pool).
    // and add access rand() % 20 random nicknames to the access list of each channel.
    // then go through and delete each nick (it will remove access list and channel entries)
    // This should go on for 60s. First and last 5 seconds are disgarded.
    // 
    // number of iterations/500 is low_water_mark, number of itereations/200 = high_water_mark
    // TODO: how to work out max_thread_pool for all of magick?

    CP((PRODUCT + " II has been started ..."));
    ResetTime=Now();

    //this little piece of code creates the actual connection from magick
    // to the irc server and sets up the socket handler that receives
    // incoming data and pushes it out to the appropriate service.

    // Grabs the FIRST entry of PRIORITY 1.
    mstring tmp = Parent->startup.PriorityList(1)[0];
    if (tmp == "")
	RET(MAGICK_RET_TERMINATE);
    ACE_INET_Addr addr(Parent->startup.Server(tmp).first, tmp);

//  ACE_INET_Addr addr(startup.Remote_Port(),startup.Remote_Server());
    //IrcServer server(ACE_Reactor::instance(),ACE_NONBLOCK);
    i_gotconnect = false;
    i_server = tmp;
    ircsvchandler=new IrcSvcHandler;
    if(ACO_server.connect(ircsvchandler,addr)==-1)
    {
	//okay we got a connection problem here. log it and shutdown
	RET(MAGICK_RET_TERMINATE);
    }
    // if the blocking version is used, use below
    /*
    ircsvchandler->send(mstring); // ie server line.
    */
    ACE_INET_Addr localaddr;
    ircsvchandler->peer().get_local_addr(localaddr);
    CP(("Local connection point=%s port:%u",localaddr.get_host_name(),localaddr.get_port_number()));
    server.raw("PASS " + startup.Server(tmp).second);
    server.raw("SERVER " + startup.Server_Name() + " 1 :" + startup.Server_Desc());

    // next thing to be done here is set up the acceptor mechanism to listen
    // for incoming "magickgui" connections and handle them.


    // not so temporary event handling mechanism
    //todo: while( below!=-1(TimeValue) { do cleanup's } that way every
    // say 5 mins or so it breaks from the event loop to cleanup

    // This is the main loop.  When we get a Shutdown(),
    // we wait for everything to shutdown cleanly.
    // All that will be left is US and the LOGGER.
    while(!Shutdown())
    {
	ACE_Reactor::instance()->run_event_loop();
    }

    // We're going down .. execute relivant shutdowns.
    if (ircsvchandler != NULL)
    {
  	ircsvchandler->shutdown();
    }
    mBase::shutdown();
    while (mThread::size() > 2)
    {
#ifdef WIN32
	Sleep(1000);
#else
	sleep(1);
#endif
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

    delete signalhandler;
    if(logger!=NULL)
	delete logger;
    if(logfile!=NULL)
	fclose(logfile);

    RET(MAGICK_RET_TERMINATE);
}

mstring Magick::getMessage(const mstring & name)
{
    FT("Magick::getMessage", (name));
    // to be filled in this get's the message from our previously loaded internal message database.
    RET("");
}

void Magick::dump_help(mstring & progname)
{
    // This needs to be re-written.
    cout << "\n"
         << FULL_NAME + " - " + FULL_URL + "\n"
         << "    (c) 1996-2000 Preston A. Elder <prez@magick.tm>\n"
         << "    (c) 1999-2000 William King <ungod@magick.tm>\n"
         << "\n"
         << "Syntax: " << ProgramName << " [ops]\n"
         << "\n"
         << "    -n, --name servername          Override SERVER_NAME in the config file.\n"
         << "    -d, --desc description         Override SERVER_DESC in the config file.\n"
         << "    -u, --user user                Override SERVICES_USER in the config file.\n"
         << "    -o, --ownuser                  Sets OWNUSER in the config file to TRUE.\n"
         << "    -h, --host host                Override SERVICES_HOST in the config file.\n"
         << "    -s, --save time                Override CYCLETIME in the config file.\n"
         << "    -v, --verbose                  Logs EVERY command to services.\n"
         << "    -l, --level level              Override LEVEL in config file.\n"
         << "    -f, --fg                       Output logs to console, fork() on CTRL-C.\n"
         << "    -r, --relink time              Override SERVER_RELINK in config file.\n"
         << "        --norelink                 Set SERVER_RELINK to 0.\n"
         << "        --dir directory            Change working (database) directory.\n"
         << "	--config config            Specify filename for config file.\n"
         << "	--log logfile              Output logs to different file.\n"
         << "	--trace type:level         Turn on tracing from the onset.\n"
         << "\n"
         << "For more help on the usage of " + PRODUCT + ", please browse the docs directory.\n"
         << "This software is free to the public, no registration or licensing fee\n"
         << "may be charged under any circumstances.  Media charges may be imposed.\n"
	 << "\n";
    
}

void Magick::LoadInternalMessages()
{
    NFT("Magick::LoadInternalMessages");
    /* This is to be replaced with language files.
       blah.lng (and magick.ini has DEF_LANGUAGE=blah
       Another file should be created for LOGMSG=blahlog
       for log messages (user display and log messages
       should be seperated */
    /*todo: okay, store the string from language.h into a temp file
	parse it in using getfirstgroup/getnextgroup/getfirstvalue/getnextvalue
	to get both the names and the values and store them into two arrays
	one our name array, the other  our value array*/


    /* note left side of message can have spaces before '=' that will be trimmed
	right side will *not* be trimmed*/

    WLOCK(("Magick","LoadMessages"));
    // so that the language file strings are only loaded in memory while this function is in effect.
#include "language.h"
    int i;
    remove("tmplang.lng");

    wxFileOutputStream *fostream=new wxFileOutputStream(wxGetCwd()+DirSlash+"tmplang.lng");
    for(i=0;i<def_langent;i++)
	*fostream<<def_lang[i]<<"\n";
    fostream->Sync();
    delete fostream;
    // need to transfer wxGetWorkingDirectory() and prepend it to tmplang.lng
    wxFileConfig fconf("magick","",wxGetCwd()+DirSlash+"tmplang.lng");
    bool bContGroup, bContEntries;
    long dummy1,dummy2;
    mstring groupname,entryname,combined;
    bContGroup=fconf.GetFirstGroup(groupname,dummy1);
    // this code is fucked up and won't work. debug to find why it's not
    // finding the entries when it is actually loading them.
    // *sigh* spent an hour so far with no luck.
    while(bContGroup)
    {
	bContEntries=fconf.GetFirstEntry(entryname,dummy2);
	while(bContEntries)
	{
    	    MessageNamesLong.push_back(groupname+"/"+entryname);
	    MessageNamesShort.push_back(entryname);
	    bContEntries=fconf.GetNextEntry(entryname,dummy2);
	    combined = groupname+"/"+entryname;
	    Messages[entryname]=fconf.Read(combined,(mstring *) "");
	}
	bContGroup=fconf.GetNextGroup(groupname,dummy1);
    }
    remove("tmplang.lng");
}

mstring Magick::parseEscapes(const mstring & in)
{
    FT("Magick::parseEscparse", (in));
    // hmm doesn't *really* need a mutex here.
    MLOCK(("Magick","parseEscapes"));
    mstring Result;
    strstream inputstream;
    inputstream<<in.c_str();
    EscLexer lexer(inputstream);
    EscParser parser(lexer);
    try
    {
	parser.expr();
    }
    catch(ParserException &E)
    {
	//todo
	mstring blah;
	blah.Format("Escape parser threw and exception at line: %d, column:%d of %s",E.line,E.column,E.getMessage().c_str());
	wxLogWarning(blah);
    }
    RET(lexer.retstring);
}

void Magick::LoadExternalMessages()
{
    NFT("Magick::LoadExternalMessages");
    // use the previously created name array to get the names to load
    WLOCK(("Magick","LoadMessages"));
    // need to transfer wxGetWorkingDirectory() and prepend it to english.lng
    wxFileConfig fconf("magick","",i_services_dir+DirSlash+"lang"+DirSlash+nickserv.DEF_Language()+".lng");
    int i;
    // change this to not just update the internal defaults but also to
    // add new one's like loadinternal does.
    for(i=0;i<MessageNamesLong.size();i++)
    	Messages[MessageNamesShort[i]]=fconf.Read(MessageNamesLong[i],&Messages[MessageNamesShort[i]]);
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
	    /*	COMMAND		SHORT	ALIASES
		--name X	-n
		--desc X	-d
		--user X	-u
		--ownuser	-o
		--host X	-h
		--dir X
		--config X
		--trace X
		--log X
		--verbose	-v	--debug
		--fg	   	-f	--live, --nofork
		--relink X	-r
		--norelink
		--level X	-l
		--save X	-s	--update
		--help		-?
		--keyfile	-k
	    */

	    bool ArgUsed=false;
	    if (argv[i][1U]=='-')
		ArgUsed=paramlong(argv[i], (i-1 < argc) ? argv[i+1].c_str() : "");
	    else
		ArgUsed=paramshort(argv[i], (i-1 < argc) ? argv[i+1].c_str() : "");
	    
	    if (ArgUsed)
		i++;
	}
	else
	{
	    wxLogFatal("Non-option arguments not allowed");
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
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--name");
	}
	startup.server_name=second;
	RET(true);
    }
    else if(first=="--desc")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--desc");
	}
	startup.server_name=second;
	RET(true);
    }
    else if(first=="--user")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--user");
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
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--host");
	}
	startup.services_host=second;
    }
    else if(first=="--log")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--log");
	}
	files.logfile=second;
	RET(true);
    }
    else if(first=="--verbose" || first=="--debug")
	logger->SetVerbose(true);
    else if(first=="--relink")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--relink");
	}
	if(atoi(second.c_str())<0)
	{
	    wxLogFatal("--relink parameter must be positive");
	}
	config.server_relink=atoi(first.c_str());
	RET(true);
    }
    else if(first=="--norelink")
	config.server_relink=0;
    else if(first=="--level")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--level");
	}
	if(atoi(second.c_str())<=0)
	{
	    wxLogFatal("--level paramater must be positive");
	}
	startup.level=atoi(second.c_str());
	RET(true);
    }
    else if(first=="--save" || first=="--update")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--save");
	}
	if(atoi(second.c_str())<=0)
	{
	    wxLogFatal("--save: number of seconds must be positive");
	}
	config.cycletime=atoi(second.c_str());
	RET(true);
    }
    else if(first=="--keyfile")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--keyfile");
	}
	if(!wxFile::Exists(second.c_str()))
	{
	    wxLogFatal("--keyfile: keyfile doesn't exist");
	}
	files.keyfile=second;
	RET(true);
    }
    else
    {
   	wxLogError("Unknown option %s, ignoring.",first.c_str());
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
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--name", second);
	}
	else if(first[i]=='d')
	{
	    if (ArgUsed)
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--desc", second);
	}
	else if(first[i]=='u')
	{
	    if (ArgUsed)
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--user", second);
	}
	else if(first[i]=='o')
	{
	    ArgUsed = paramlong ("--ownuser", second);
	}
	else if(first[i]=='h')
	{
	    if (ArgUsed)
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--host", second);
	}
	else if(first[i]=='v')
	{
	    ArgUsed = paramlong ("--verbose", second);
	}
	else if(first[i]=='f')
	{
	    ArgUsed = paramlong ("--fg", second);
	}
	else if(first[i]=='r')
	{
	    if (ArgUsed)
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--relink", second);
	}
	else if(first[i]=='l')
	{
	    if (ArgUsed)
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--level", second);
	}
	else if(first[i]=='s')
	{
	    if (ArgUsed)
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--save", second);
	}
	else if(first[i]=='k')
	{
	    if (ArgUsed)
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--keyfile", second);
	}
	else
	{
	    wxLogError("Unknown option -%c, ignoring.",first[i]);
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
    mstring errstring;

    // Check for \ or / or ?: in first chars.
    if ((errstring = i_config_file[0u]) == DirSlash || i_config_file[1u] == ':')
	errstring=i_config_file;
    else
	errstring=i_services_dir+DirSlash+i_config_file;

    {
	FILE *TmpHand;
	if ((TmpHand = fopen(errstring, "r")) == NULL)
	{
	    RET(false);
	}
	else
	    fclose(TmpHand);
    }
    wxFileConfig in("magick","",errstring);
    errstring = "";

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

    in.Read(ts_Startup+"SERVER_NAME",&value_mstring,"services.magick.tm");
    if (value_mstring != startup.server_name)
	reconnect = true;
    startup.server_name = value_mstring;

    in.Read(ts_Startup+"SERVER_DESC",&value_mstring,FULL_NAME);
    if (value_mstring != startup.server_desc)
	reconnect = true;
    startup.server_desc = value_mstring;

    in.Read(ts_Startup+"SERVICES_USER",&value_mstring,"services");
    in.Read(ts_Startup+"OWNUSER",&value_bool,false);
    if (value_bool != startup.ownuser)
	reconnect_clients = true;
    else if (value_mstring != startup.services_user &&
	     !startup.ownuser)
	reconnect_clients = true;
    startup.services_user = value_mstring;
    startup.ownuser = value_bool;

    in.Read(ts_Startup+"SERVICES_HOST",&value_mstring,"magick.tm");
    if (value_mstring != startup.services_host)
	reconnect_clients = true;
    startup.services_host = value_mstring;

    // REMOTE entries
    mstring ent="";
    i=1;
    startup.servers.clear();
    do {
	mstring rem = "REMOTE_";
	rem << i;
	in.Read(ts_Startup+rem,&ent,"");
	if (ent!="") {
		mstring tmp[4];
		tmp[0]=ent.ExtractWord(1, ":");
		tmp[1]=ent.ExtractWord(2, ":");
		tmp[2]=ent.ExtractWord(3, ":");
		tmp[3]=ent.ExtractWord(4, ":");
		if (ent.WordCount(":") == 4 && tmp[1].IsNumber() && tmp[3].IsNumber())
		    startup.servers[tmp[0].LowerCase()] = triplet<unsigned int,mstring,unsigned int>(atoi(tmp[1]),tmp[2],atoi(tmp[3]));
		else
		    wxLogWarning("REMOTE entry for %s contained incorrect syntax, ignored!", tmp[0].c_str());
	}
	i++;
    } while (ent!="");
    if (startup.servers.find(server.i_OurUplink) == startup.servers.end())
	reconnect = true;

    in.Read(ts_Startup+"LEVEL",&value_uint,1);
    if (value_uint > i_level)
	i_level = value_uint;
    startup.level = value_uint;

    in.Read(ts_Startup+"LAGTIME",&value_mstring,"10s");
    if (FromHumanTime(value_mstring))
	startup.lagtime = FromHumanTime(value_mstring);
    else
	startup.lagtime = FromHumanTime("10s");

    in.Read(ts_Startup+"STOP",&value_bool,true);
    i_shutdown = value_bool;

    in.Read(ts_Services+"NickServ",&value_mstring,"NickServ");
    for (i=0; i<nickserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + nickserv.names.ExtractWord(i+1, " ") + " ")))
	{
	    nickserv.signoff(nickserv.names.ExtractWord(i+1, " "));
	}
    }
    nickserv.names = value_mstring;

    in.Read(ts_Services+"NickServ_Name",&value_mstring,"Nickname Service");
    if (value_mstring != nickserv.realname)
    {
	nickserv.realname = value_mstring;
	for (i=0; i<nickserv.names.WordCount(" "); i++)
	{
	    nickserv.signoff(nickserv.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && GotConnect)
    {
	for (i=0; i<nickserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(nickserv.names.ExtractWord(i+1, " ")))
	    {
		nickserv.signon(nickserv.names.ExtractWord(i+1, " "));
	    }
	}
    }

    in.Read(ts_Services+"ChanServ",&value_mstring,"ChanServ");
    for (i=0; i<chanserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + chanserv.names.ExtractWord(i+1, " ") + " ")))
	{
	    chanserv.signoff(chanserv.names.ExtractWord(i+1, " "));
	}
    }
    chanserv.names = value_mstring;

    in.Read(ts_Services+"ChanServ_Name",&value_mstring,"Channel Service");
    if (value_mstring != chanserv.realname)
    {
	chanserv.realname = value_mstring;
	for (i=0; i<chanserv.names.WordCount(" "); i++)
	{
	    chanserv.signoff(chanserv.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && GotConnect)
    {
	for (i=0; i<chanserv.names.WordCount(" "); i++)
	{
	    if (!chanserv.IsLive(chanserv.names.ExtractWord(i+1, " ")))
	    {
		chanserv.signon(chanserv.names.ExtractWord(i+1, " "));
	    }
	}
    }

    in.Read(ts_Services+"MemoServ",&value_mstring,"MemoServ");
    for (i=0; i<memoserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + memoserv.names.ExtractWord(i+1, " ") + " ")))
	{
	     memoserv.signoff(memoserv.names.ExtractWord(i+1, " "));
	}
    }
    memoserv.names = value_mstring;

    in.Read(ts_Services+"MemoServ_Name",&value_mstring,"Memo/News Service");
    if (value_mstring != memoserv.realname)
    {
	memoserv.realname = value_mstring;
	for (i=0; i<memoserv.names.WordCount(" "); i++)
	{
	    memoserv.signoff(memoserv.names.ExtractWord(i+1, " "));
	}
    }

    in.Read(ts_Services+"MEMO",&memoserv.memo,true);
    in.Read(ts_Services+"NEWS",&memoserv.news,true);
    if (!reconnect && GotConnect && (memoserv.memo || memoserv.news))
    {
	for (i=0; i<memoserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(memoserv.names.ExtractWord(i+1, " ")))
	    {
		memoserv.signon(memoserv.names.ExtractWord(i+1, " "));
	    }
	}
    }

    in.Read(ts_Services+"OperServ",&value_mstring,"OperServ");
    for (i=0; i<operserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + operserv.names.ExtractWord(i+1, " ") + " ")))
	{
	    operserv.signoff(operserv.names.ExtractWord(i+1, " "));
	}
    }
    operserv.names = value_mstring;

    in.Read(ts_Services+"OperServ_Name",&value_mstring,"Operator Service");
    if (value_mstring != operserv.realname)
    {
	operserv.realname = value_mstring;
	for (i=0; i<operserv.names.WordCount(" "); i++)
	{
	    operserv.signoff(operserv.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && GotConnect)
    {
	for (i=0; i<operserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(operserv.names.ExtractWord(i+1, " ")))
	    {
		operserv.signon(operserv.names.ExtractWord(i+1, " "));
	    }
	}
    }

    in.Read(ts_Services+"FLOOD",&operserv.flood,true);
    in.Read(ts_Services+"AKILL",&operserv.akill,true);
    in.Read(ts_Services+"OPERDENY",&operserv.operdeny,true);

    in.Read(ts_Services+"CommServ",&value_mstring,"CommServ");
    for (i=0; i<commserv.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + commserv.names.ExtractWord(i+1, " ") + " ")))
	{
	    commserv.signoff(commserv.names.ExtractWord(i+1, " "));
	}
    }
    commserv.names = value_mstring;

    in.Read(ts_Services+"CommServ_Name",&value_mstring,"Committee Service");
    if (value_mstring != commserv.realname)
    {
	commserv.realname = value_mstring;
	for (i=0; i<commserv.names.WordCount(" "); i++)
	{
	    commserv.signoff(commserv.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && GotConnect)
    {
	for (i=0; i<commserv.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(commserv.names.ExtractWord(i+1, " ")))
	    {
		commserv.signon(commserv.names.ExtractWord(i+1, " "));
	    }
	}
    }

    in.Read(ts_Services+"ServMsg",&value_mstring,"GlobalMsg DevNull");
    for (i=0; i<servmsg.names.WordCount(" "); i++)
    {
	if (reconnect_clients || !(" " + value_mstring + " ").Contains(
	    (" " + servmsg.names.ExtractWord(i+1, " ") + " ")))
	{
	    servmsg.signoff(servmsg.names.ExtractWord(i+1, " "));
	}
    }
    servmsg.names = value_mstring;

    in.Read(ts_Services+"ServMsg_Name",&value_mstring,PRODUCT + " <--> User");
    if (value_mstring != servmsg.realname)
    {
	servmsg.realname = value_mstring;
	for (i=0; i<servmsg.names.WordCount(" "); i++)
	{
	    servmsg.signoff(servmsg.names.ExtractWord(i+1, " "));
	}
    }

    if (!reconnect && GotConnect)
    {
	for (i=0; i<servmsg.names.WordCount(" "); i++)
	{
	    if (!nickserv.IsLive(servmsg.names.ExtractWord(i+1, " ")))
	    {
		servmsg.signon(servmsg.names.ExtractWord(i+1, " "));
		Parent->server.MODE(servmsg.names.ExtractWord(i+1, " "), "+o");
	    }
	}
    }

    in.Read(ts_Services+"SHOWSYNC",&servmsg.showsync,true);

    in.Read(ts_Files+"PIDFILE",&files.pidfile,"magick.pid");
    in.Read(ts_Files+"LOGFILE",&files.logfile,"magick.log");
    in.Read(ts_Files+"VERBOSE", &value_bool, false);
    logger->SetVerbose(value_bool);
    in.Read(ts_Files+"MOTDFILE",&files.motdfile,"magick.motd");
    in.Read(ts_Files+"DATABASE",&files.database,"magick.mnd");
    in.Read(ts_Files+"COMPRESSION",&files.compression,6);
    if (files.compression < 0)
	files.compression = 0;
    else if (files.compression > 9)
	files.compression = 9;
    in.Read(ts_Files+"KEYFILE",&files.keyfile,"magick.key");
    in.Read(ts_Files+"ENCRYPTION",&files.encryption,false);

    in.Read(ts_Config+"SERVER_RELINK",&value_mstring,"5s");
    if (FromHumanTime(value_mstring))
	config.server_relink = FromHumanTime(value_mstring);
    else
	config.server_relink = FromHumanTime("5s");

    in.Read(ts_Config+"SQUIT_PROTECT",&value_mstring,"2m");
    if (FromHumanTime(value_mstring))
	config.squit_protect = FromHumanTime(value_mstring);
    else
	config.squit_protect = FromHumanTime("2m");

    in.Read(ts_Config+"SQUIT_CANCEL",&value_mstring,"10s");
    if (FromHumanTime(value_mstring))
	config.squit_cancel = FromHumanTime(value_mstring);
    else
	config.squit_cancel = FromHumanTime("10s");

    in.Read(ts_Config+"CYCLETIME",&value_mstring,"5m");
    if (FromHumanTime(value_mstring))
	config.cycletime = FromHumanTime(value_mstring);
    else
	config.cycletime = FromHumanTime("5m");

    in.Read(ts_Config+"CHECKTIME",&value_mstring,"5s");
    if (FromHumanTime(value_mstring))
	config.checktime = FromHumanTime(value_mstring);
    else
	config.checktime = FromHumanTime("5s");

    in.Read(ts_Config+"PING_FREQUENCY",&value_mstring,"30s");
    if (FromHumanTime(value_mstring))
	config.ping_frequency = FromHumanTime(value_mstring);
    else
	config.ping_frequency = FromHumanTime("30s");

    in.Read(ts_Config+"STARTHRESH",&config.starthresh, 4);
    in.Read(ts_Config+"LISTSIZE",&config.listsize, 50);
    in.Read(ts_Config+"MAXLIST",&value_uint, 250);
    if (value_int < config.listsize)
	value_int = config.listsize;
    config.maxlist = value_int;

    in.Read(ts_Config+"STARTUP_THREADS",&config.startup_threads, 2);
    in.Read(ts_Config+"LOW_WATER_MARK",&config.low_water_mark, 20);
    in.Read(ts_Config+"HIGH_WATER_MARK",&config.high_water_mark, 25);
    if (config.high_water_mark < config.low_water_mark)
	config.high_water_mark = config.low_water_mark;

    in.Read(ts_NickServ+"MAXLEN",&nickserv.maxlen,9);
    in.Read(ts_NickServ+"SUFFIXES",&nickserv.suffixes,"_-^`");
    in.Read(ts_NickServ+"EXPIRE",&value_mstring,"4w");
    if (FromHumanTime(value_mstring))
	nickserv.expire = FromHumanTime(value_mstring);
    else
	nickserv.expire = FromHumanTime("4w");

    in.Read(ts_NickServ+"IDENT",&value_mstring,"1m");
    if (FromHumanTime(value_mstring))
	nickserv.ident = FromHumanTime(value_mstring);
    else
	nickserv.ident = FromHumanTime("1m");

    in.Read(ts_NickServ+"RELEASE",&value_mstring,"1m");
    if (FromHumanTime(value_mstring))
	nickserv.release = FromHumanTime(value_mstring);
    else
	nickserv.release = FromHumanTime("1m");

    in.Read(ts_NickServ+"PASSFAIL",&nickserv.passfail,5);
    in.Read(ts_NickServ+"DEF_PROTECT",&nickserv.def_protect,true);
    in.Read(ts_NickServ+"LCK_PROTECT",&nickserv.lck_protect,false);
    in.Read(ts_NickServ+"DEF_SECURE",&nickserv.def_secure,false);
    in.Read(ts_NickServ+"LCK_SECURE",&nickserv.lck_secure,false);
    in.Read(ts_NickServ+"DEF_NOEXPIRE",&nickserv.def_noexpire,false);
    in.Read(ts_NickServ+"LCK_NOEXPIRE",&nickserv.lck_noexpire,false);
    in.Read(ts_NickServ+"DEF_NOMEMO",&nickserv.def_nomemo,false);
    in.Read(ts_NickServ+"LCK_NOMEMO",&nickserv.lck_nomemo,false);
    in.Read(ts_NickServ+"DEF_PRIVATE",&nickserv.def_private,false);
    in.Read(ts_NickServ+"LCK_PRIVATE",&nickserv.lck_private,false);
    in.Read(ts_NickServ+"DEF_PRIVMSG",&nickserv.def_privmsg,false);
    in.Read(ts_NickServ+"LCK_PRIVMSG",&nickserv.lck_privmsg,false);
    in.Read(ts_NickServ+"PICSIZE",&nickserv.picsize,0);
    in.Read(ts_NickServ+"PICEXT",&nickserv.picext,"jpg gif bmp tif");

    in.Read(ts_ChanServ+"EXPIRE",&value_mstring,"2w");
    if (FromHumanTime(value_mstring))
	chanserv.expire = FromHumanTime(value_mstring);
    else
	chanserv.expire = FromHumanTime("2w");

    in.Read(ts_ChanServ+"DEF_AKICK",&chanserv.def_akick_reason,"You have been banned from channel");
    in.Read(ts_ChanServ+"PASSFAIL",&chanserv.passfail,5);
    in.Read(ts_ChanServ+"CHANKEEP",&value_mstring,"15s");
    if (FromHumanTime(value_mstring))
	chanserv.chankeep = FromHumanTime(value_mstring);
    else
	chanserv.chankeep = FromHumanTime("15s");

    in.Read(ts_ChanServ+"DEF_MLOCK",&chanserv.def_mlock,"+nt");
    in.Read(ts_ChanServ+"LCK_MLOCK",&chanserv.lck_mlock,"+");
    in.Read(ts_ChanServ+"DEF_BANTIME",&value_mstring, "0");
    if (FromHumanTime(value_mstring))
	chanserv.def_bantime = FromHumanTime(value_mstring);
    else
	chanserv.def_bantime = FromHumanTime("0");

    in.Read(ts_ChanServ+"LCK_BANTIME",&chanserv.lck_bantime,false);
    in.Read(ts_ChanServ+"DEF_KEEPTOPIC",&chanserv.def_keeptopic,true);
    in.Read(ts_ChanServ+"LCK_KEEPTOPIC",&chanserv.lck_keeptopic,false);
    in.Read(ts_ChanServ+"DEF_TOPICLOCK",&chanserv.def_topiclock,false);
    in.Read(ts_ChanServ+"LCK_TOPICLOCK",&chanserv.lck_topiclock,false);
    in.Read(ts_ChanServ+"DEF_PRIVATE",&chanserv.def_private,false);
    in.Read(ts_ChanServ+"LCK_PRIVATE",&chanserv.lck_private,false);
    in.Read(ts_ChanServ+"DEF_SECUREOPS",&chanserv.def_secureops,false);
    in.Read(ts_ChanServ+"LCK_SECUREOPS",&chanserv.lck_secureops,false);
    in.Read(ts_ChanServ+"DEF_SECURE",&chanserv.def_secure,false);
    in.Read(ts_ChanServ+"LCK_SECURE",&chanserv.lck_secure,false);
    in.Read(ts_ChanServ+"DEF_NOEXPIRE",&chanserv.def_noexpire,false);
    in.Read(ts_ChanServ+"LCK_NOEXPIRE",&chanserv.lck_noexpire,false);
    in.Read(ts_ChanServ+"DEF_ANARCHY",&chanserv.def_anarchy,false);
    in.Read(ts_ChanServ+"LCK_ANARCHY",&chanserv.lck_anarchy,false);
    in.Read(ts_ChanServ+"DEF_RESTRICTED",&chanserv.def_restricted,false);
    in.Read(ts_ChanServ+"LCK_RESTRICTED",&chanserv.lck_restricted,false);
    in.Read(ts_ChanServ+"DEF_JOIN",&chanserv.def_join,false);
    in.Read(ts_ChanServ+"LCK_JOIN",&chanserv.lck_join,false);
    in.Read(ts_ChanServ+"DEF_REVENGE",&chanserv.def_revenge,"NONE");
    in.Read(ts_ChanServ+"LCK_REVENGE",&chanserv.lck_revenge,false);
    in.Read(ts_ChanServ+"LEVEL_MIN",&chanserv.level_min,-1);
    in.Read(ts_ChanServ+"LEVEL_MAX",&chanserv.level_max,30);
    in.Read(ts_ChanServ+"LVL_AUTODEOP",&chanserv.lvl["AUTODEOP"],-1);
    in.Read(ts_ChanServ+"LVL_AUTOVOICE",&chanserv.lvl["AUTOVOICE"],5);
    in.Read(ts_ChanServ+"LVL_AUTOOP",&chanserv.lvl["AUTOOP"],10);
    in.Read(ts_ChanServ+"LVL_READMEMO",&chanserv.lvl["READMEMO"],0);
    in.Read(ts_ChanServ+"LVL_WRITEMEMO",&chanserv.lvl["WRITEMEMO"],15);
    in.Read(ts_ChanServ+"LVL_DELMEMO",&chanserv.lvl["DELMEMO"],25);
    in.Read(ts_ChanServ+"LVL_GREET",&chanserv.lvl["GREET"],1);
    in.Read(ts_ChanServ+"LVL_OVERGREET",&chanserv.lvl["OVERGREET"],25);
    in.Read(ts_ChanServ+"LVL_MESSAGE",&chanserv.lvl["MESSAGE"],20);
    in.Read(ts_ChanServ+"LVL_AKICK",&chanserv.lvl["AKICK"],20);
    in.Read(ts_ChanServ+"LVL_SUPER",&chanserv.lvl["SUPER"],25);
    in.Read(ts_ChanServ+"LVL_UNBAN",&chanserv.lvl["UNBAN"],10);
    in.Read(ts_ChanServ+"LVL_ACCESS",&chanserv.lvl["ACCESS"],5);
    in.Read(ts_ChanServ+"LVL_SET",&chanserv.lvl["SET"],25);
    in.Read(ts_ChanServ+"LVL_VIEW",&chanserv.lvl["VIEW"],1);
    in.Read(ts_ChanServ+"LVL_CMDINVITE",&chanserv.lvl["CMDINVITE"],5);
    in.Read(ts_ChanServ+"LVL_CMDUNBAN",&chanserv.lvl["CMDUNBAN"],10);
    in.Read(ts_ChanServ+"LVL_CMDVOICE",&chanserv.lvl["CMDVOICE"],5);
    in.Read(ts_ChanServ+"LVL_CMDOP",&chanserv.lvl["CMDOP"],10);
    in.Read(ts_ChanServ+"LVL_CMDKICK",&chanserv.lvl["CMDKICK"],15);
    in.Read(ts_ChanServ+"LVL_CMDMODE",&chanserv.lvl["CMDMODE"],15);
    in.Read(ts_ChanServ+"LVL_CMDCLEAR",&chanserv.lvl["CMDCLEAR"],20);

    in.Read(ts_MemoServ+"NEWS_EXPIRE",&value_mstring,"3w");
    if (FromHumanTime(value_mstring))
	memoserv.news_expire = FromHumanTime(value_mstring);
    else
	memoserv.news_expire = FromHumanTime("3w");

    in.Read(ts_MemoServ+"INFLIGHT",&value_mstring,"3m");
    if (FromHumanTime(value_mstring))
	memoserv.inflight = FromHumanTime(value_mstring);
    else
	memoserv.inflight = FromHumanTime("3m");

    in.Read(ts_MemoServ+"FILES",&memoserv.files,0);
    in.Read(ts_MemoServ+"FILESIZE",&memoserv.filesize,0);

    in.Read(ts_OperServ+"SERVICES_ADMIN",&operserv.services_admin,"");
    in.Read(ts_OperServ+"SECURE",&operserv.secure,false);
    in.Read(ts_OperServ+"DEF_EXPIRE",&value_mstring,"3h");
    if (FromHumanTime(value_mstring))
	operserv.def_expire = FromHumanTime(value_mstring);
    else
	operserv.def_expire = FromHumanTime("3h");

    in.Read(ts_OperServ+"EXPIRE_OPER",&value_mstring,"1d");
    if (FromHumanTime(value_mstring))
	operserv.expire_oper = FromHumanTime(value_mstring);
    else
	operserv.expire_oper = FromHumanTime("1d");

    in.Read(ts_OperServ+"EXPIRE_ADMIN",&value_mstring,"1w");
    if (FromHumanTime(value_mstring))
	operserv.expire_admin = FromHumanTime(value_mstring);
    else
	operserv.expire_admin = FromHumanTime("1w");

    in.Read(ts_OperServ+"EXPIRE_SOP",&value_mstring,"8w");
    if (FromHumanTime(value_mstring))
	operserv.expire_sop = FromHumanTime(value_mstring);
    else
	operserv.expire_sop = FromHumanTime("8w");

    in.Read(ts_OperServ+"EXPIRE_SADMIN",&value_mstring,"1y");
    if (FromHumanTime(value_mstring))
	operserv.expire_sadmin = FromHumanTime(value_mstring);
    else
	operserv.expire_sadmin = FromHumanTime("1y");

    in.Read(ts_OperServ+"MAX_CLONE",&operserv.max_clone,50);
    in.Read(ts_OperServ+"CLONE_LIMIT",&operserv.clone_limit,2);
    in.Read(ts_OperServ+"DEF_CLONE",&operserv.def_clone,"Maximum connections from one host exceeded");
    in.Read(ts_OperServ+"FLOOD_TIME",&value_mstring,"10s");
    if (FromHumanTime(value_mstring))
	operserv.flood_time = FromHumanTime(value_mstring);
    else
	operserv.flood_time = FromHumanTime("10s");

    in.Read(ts_OperServ+"FLOOD_MSGS",&operserv.flood_msgs,5);

    in.Read(ts_OperServ+"IGNORE_TIME",&value_mstring,"20s");
    if (FromHumanTime(value_mstring))
	operserv.ignore_time = FromHumanTime(value_mstring);
    else
	operserv.ignore_time = FromHumanTime("20s");

    in.Read(ts_OperServ+"IGNORE_LIMIT",&operserv.ignore_limit,5);
    in.Read(ts_OperServ+"IGNORE_REMOVE",&value_mstring,"5m");
    if (FromHumanTime(value_mstring))
	operserv.ignore_remove = FromHumanTime(value_mstring);
    else
	operserv.ignore_remove = FromHumanTime("5m");

    in.Read(ts_OperServ+"IGNORE_METHOD",&operserv.ignore_method,8);
    
    RemCommands();
    in.Read(ts_CommServ+"DEF_OPENMEMOS",&commserv.def_openmemos,true);
    in.Read(ts_CommServ+"LCK_OPENMEMOS",&commserv.lck_openmemos,false);
    in.Read(ts_CommServ+"DEF_SECURE",&commserv.def_secure,false);
    in.Read(ts_CommServ+"LCK_SECURE",&commserv.lck_secure,false);
    in.Read(ts_CommServ+"DEF_PRIVATE",&commserv.def_private,false);
    in.Read(ts_CommServ+"LCK_PRIVATE",&commserv.lck_private,false);
    in.Read(ts_CommServ+"ALL_NAME",&commserv.all_name,"ALL");
    in.Read(ts_CommServ+"REGD_NAME",&commserv.regd_name,"REGD");
    in.Read(ts_CommServ+"SADMIN_NAME",&commserv.sadmin_name,"SADMIN");
    in.Read(ts_CommServ+"SADMIN_SECURE",&commserv.sadmin_secure,true);
    in.Read(ts_CommServ+"SADMIN_PRIVATE",&commserv.sadmin_private,false);
    in.Read(ts_CommServ+"SADMIN_OPENMEMOS",&commserv.sadmin_openmemos,true);
    in.Read(ts_CommServ+"SOP_NAME",&commserv.sop_name,"SOP");
    in.Read(ts_CommServ+"SOP_SECURE",&commserv.sop_secure,true);
    in.Read(ts_CommServ+"SOP_PRIVATE",&commserv.sop_private,false);
    in.Read(ts_CommServ+"SOP_OPENMEMOS",&commserv.sop_openmemos,true);
    in.Read(ts_CommServ+"ADMIN_NAME",&commserv.admin_name,"ADMIN");
    in.Read(ts_CommServ+"ADMIN_SECURE",&commserv.admin_secure,true);
    in.Read(ts_CommServ+"ADMIN_PRIVATE",&commserv.admin_private,false);
    in.Read(ts_CommServ+"ADMIN_OPENMEMOS",&commserv.admin_openmemos,true);
    in.Read(ts_CommServ+"OPER_NAME",&commserv.oper_name,"OPER");
    in.Read(ts_CommServ+"OPER_SECURE",&commserv.oper_secure,true);
    in.Read(ts_CommServ+"OPER_PRIVATE",&commserv.oper_private,false);
    in.Read(ts_CommServ+"OPER_OPENMEMOS",&commserv.oper_openmemos,true);
    commserv.all_name.MakeUpper();
    commserv.regd_name.MakeUpper();
    commserv.sadmin_name.MakeUpper();
    commserv.sop_name.MakeUpper();
    commserv.admin_name.MakeUpper();
    commserv.oper_name.MakeUpper();
    AddCommands();

    if (Parent->commserv.IsList(commserv.all_name))
	while (i<Parent->commserv.list[commserv.all_name].size())
	{
	    Parent->commserv.list[commserv.all_name].member =
			Parent->commserv.list[commserv.all_name].begin();
	    Parent->commserv.list[commserv.all_name].erase();
	}
    else
    {
	Parent->commserv.list[commserv.all_name] = Committee(commserv.all_name, 
					    "All Users");
	Parent->commserv.list[commserv.all_name].Secure(false);
	Parent->commserv.list[commserv.all_name].Private(true);
	Parent->commserv.list[commserv.all_name].OpenMemos(false);
    }

    if (Parent->commserv.IsList(commserv.regd_name))
	while (i<Parent->commserv.list[commserv.regd_name].size())
	{
	    Parent->commserv.list[commserv.regd_name].member =
			Parent->commserv.list[commserv.regd_name].begin();
	    Parent->commserv.list[commserv.regd_name].erase();
	}
    else
    {
	Parent->commserv.list[commserv.regd_name] = Committee(commserv.regd_name, 
					    "Registered Users");
	Parent->commserv.list[commserv.regd_name].Secure(false);
	Parent->commserv.list[commserv.regd_name].Private(true);
	Parent->commserv.list[commserv.regd_name].OpenMemos(false);
    }

    if (Parent->commserv.IsList(commserv.sadmin_name))
	while (i<Parent->commserv.list[commserv.sadmin_name].size())
	{
	    Parent->commserv.list[commserv.sadmin_name].member =
			Parent->commserv.list[commserv.sadmin_name].begin();
	    Parent->commserv.list[commserv.sadmin_name].erase();
	}
    else
    {
	Parent->commserv.list[commserv.sadmin_name] = Committee(commserv.sadmin_name, 
					    "Services Administrators");
	Parent->commserv.list[commserv.sadmin_name].Secure(commserv.sadmin_secure);
	Parent->commserv.list[commserv.sadmin_name].Private(commserv.sadmin_private);
	Parent->commserv.list[commserv.sadmin_name].OpenMemos(commserv.sadmin_openmemos);
    }
    for (i=1; i<=Parent->operserv.services_admin.WordCount(", "); i++)
	Parent->commserv.list[commserv.sadmin_name].insert(
	    Parent->operserv.services_admin.ExtractWord(i, ", "),
	    Parent->operserv.FirstName());
    if (!Parent->commserv.IsList(commserv.sop_name))
    {
	Parent->commserv.list[commserv.sop_name] = Committee(commserv.sop_name,
				    &Parent->commserv.list[commserv.sadmin_name],
				    "Services Operators");
	Parent->commserv.list[commserv.sop_name].Secure(commserv.sop_secure);
	Parent->commserv.list[commserv.sop_name].Private(commserv.sop_private);
	Parent->commserv.list[commserv.sop_name].OpenMemos(commserv.sop_openmemos);
    }
    if (!Parent->commserv.IsList(commserv.admin_name))
    {
	Parent->commserv.list[commserv.admin_name] = Committee(commserv.admin_name, 
				    &Parent->commserv.list[commserv.sadmin_name],
				    "Server Administrators");
	Parent->commserv.list[commserv.admin_name].Secure(commserv.admin_secure);
	Parent->commserv.list[commserv.admin_name].Private(commserv.admin_private);
	Parent->commserv.list[commserv.admin_name].OpenMemos(commserv.admin_openmemos);
    }
    if (!Parent->commserv.IsList(commserv.oper_name))
    {
	Parent->commserv.list[commserv.oper_name] = Committee(commserv.oper_name, 
				    &Parent->commserv.list[commserv.admin_name],
				    "Server Operators");
	Parent->commserv.list[commserv.oper_name].Secure(commserv.oper_secure);
	Parent->commserv.list[commserv.oper_name].Private(commserv.oper_private);
	Parent->commserv.list[commserv.oper_name].OpenMemos(commserv.oper_openmemos);
    }

/*
    if (reconnect && GotConnect)
    {
	server.raw("ERROR :Closing Link: Configuration reload required restart!");
	ircsvchandler->shutdown();
	ACE_Reactor::instance()->schedule_timer(&rh,0,ACE_Time_Value(1));
    }
*/

    RET(true);
    CP(("%s read and loaded to live configuration.", config_file.c_str()));
}

int SignalHandler::handle_signal(int signum, siginfo_t *siginfo, ucontext_t *ucontext)
{
    FT("SignalHandler::handle_signal", (signum, "(siginfo_t *) siginfo", "(ucontext_t *) ucontext"));
    static mDateTime LastSEGV;

    // todo: fill this sucker in
    switch(signum)
    {
    case 0:		// this is here to show up clashes for badly defined signal constants
	break;
    case SIGINT:	// CTRL-C, Background.
	Parent->Shutdown(true);	// Temp, we just kill on CTRL-C
	RET(-1);
	break;
#if defined(SIGTERM) && (SIGTERM != 0)
    case SIGTERM:	// Save DB's (often prequil to -KILL!)
	Parent->Shutdown(true);	// Temp, we just kill on CTRL-C
	break;
#endif
#if defined(SIGPIPE) && (SIGPIPE != 0)
    case SIGPIPE:	// Ignore
	break;
#endif
#if defined(SIGQUIT) && (SIGQUIT != 0)
    case SIGQUIT:	// Terminal dead, Background.
	break;
#endif
    case SIGSEGV:	// Segfault, validate all storage.
	// IF LastSEGV is defined, and time between now and
	// LastSEGV is < 5 seconds ...
	if(((time_t) LastSEGV != 0) && ((time_t) (Now() - LastSEGV) < 5))
	{
	    CP(("Got second sigsegv call, giving magick the boot"));
	    RET(-1);
	}
	else
	{
	    LastSEGV = Now();
	    CP(("Got first sigsegv call, giving it another chance"));
	}
	break;
#ifdef SIGBUS
    case SIGBUS:	// Ignore
	break;
#endif
#if defined(SIGHUP) && (SIGHUP != 0)
    case SIGHUP:	// Reload CFG/DB's
	break;
#endif
    case SIGILL:	// illegal opcode, this suckers gone awry..
	Parent->Shutdown(true);	// We've gotta kill her captain, she's breaking up.
	return -1;
	break;
#ifdef SIGTRAP
    case SIGTRAP:	// Throw exception
	break;
#endif
#ifdef SIGIOT
    case SIGIOT:	// abort(), exit immediately!
	break;
#endif
    case SIGFPE:	// Retry last call
	break;
#if defined(SIGUSR1) && (SIGUSR1 != 0)
    case SIGUSR1:	// ??
	break;
#endif
#if defined(SIGUSR2) && (SIGUSR2 != 0)
    case SIGUSR2:	// ??
	break;
#endif
#if defined(SIGALRM) && (SIGALRM != 0)
    case SIGALRM:	// Ignore
	break;
#endif
#if defined(SIGCHLD) && (SIGCHLD != 0)
    case SIGCHLD:	// Ignore
	break;
#endif
#ifdef SIGWINCH
    case SIGWINCH:	// Ignore
	break;
#endif
#ifdef SIGTTIN
    case SIGTTIN:	// Ignore
	break;
#endif
#ifdef SIGTTOU
    case SIGTTOU:	// Ignore
	break;
#endif
#ifdef SIGTSTP
    case SIGTSTP:	// Ignore
	break;
#endif
    default:		// Everything else.
	;//ignore (todo log that we got it and we're ignoring it)
    }
    RET(0);
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

Magick::~Magick()
{
    loggertask.i_shutdown();
}

bool Magick::startup_t::IsServer(mstring server)
{
    FT("Magick::startup_t::IsServer", (server));

    if (servers.find(server.LowerCase()) != servers.end()) {
	RET(true);
    }
    RET(false);
}

triplet<unsigned int,mstring,unsigned int> Magick::startup_t::Server(mstring server)
{
    FT("Magick::startup_t::Server", (server));
    triplet<unsigned int,mstring,unsigned int> value(0, "", 0);

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

    for (iter=servers.begin(); iter!=servers.end(); iter++) {
	if (iter->second.third == pri)
	    list.push_back(iter->first);
    }
    NRET(vector<mstring>, list);
}


void Magick::load_databases()
{
    // to buggered to think about it tonight, maybe tommorow night.
    // files.Main_DB() == output filename
    // todo: mlock the load/save structures.

    // the below is megaly fucked up, need to do load from file to memory stream, 
    // then pass that stream on down to the load_database code

/*    wxMemoryStream chanservstrm;
    wxInputStream *strm=create_input_stream(chanservstrm);
    chanserv.load_database(*strm);
    destroy_input_stream();*/
}

void Magick::save_databases()
{
    // to buggered to think about it tonight, maybe tommorow night.
/*    wxOutputStream *strm;

    wxMemoryStream chanservstream;
    strm=create_output_stream(chanservstrm);
    chaserv.save_database(*strm);
    destroy_output_stream();

    wxMemoryStream nickservstream;
    strm=create_output_stream(chanservstrm);
    nickserv.save_database(*strm);
    destroy_output_stream();

    wxFileOutputStream */

}

wxInputStream *Magick::create_input_stream(wxMemoryStream &in)
{
    wxInputStream *Result=&in;
/*    if(files.Password())
    {
	cstrm=new mDecryptStream(*Result,files.Password());
	Result=cstrm;
    }
    else
	cstrm=NULL;
*/
    if(files.Compression())
    {
	zstrm=new wxZlibInputStream(*Result);
	Result=zstrm;
    }
    else
	zstrm=NULL;
    return Result;
}

void Magick::destroy_input_stream()
{
    if(zstrm!=NULL)
    {
	delete zstrm;
	zstrm=NULL;
    }
    if(cstrm!=NULL)
    {
	delete cstrm;
	cstrm=NULL;
    }
}
wxOutputStream *Magick::create_output_stream(wxMemoryStream &out)
{
    wxOutputStream *Result=&out;
/*    if(files.Password()!="")
    {
	cstrm=new mEncryptStream(*Result,files.Password());
	Result=cstrm;
    }
    else
	cstrm=NULL;*/
    if(files.Compression())
    {
	ozstrm=new wxZlibOutputStream(*Result);
	Result=ozstrm;
    }
    else
	ozstrm=NULL;
    return Result;
}
void Magick::destroy_output_stream()
{
    if(zstrm!=NULL)
    {
	delete zstrm;
	zstrm=NULL;
    }
    if(cstrm!=NULL)
    {
	delete cstrm;
	cstrm=NULL;
    }
}

void Magick::send(mstring in)
{
    if (ircsvchandler != NULL)
	ircsvchandler->send(in);
}