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
#include "log.h"
#include "EscLexer.hpp"
#include "EscParser.hpp"
#include "lockable.h"
#include "utils.h"

//#define ACE_DEBUGGING

wxLogStderr *logger;
mDateTime StartTime, ResetTime;

Magick::Magick(int inargc, char **inargv) : chanserv(this), nickserv(this), server(this)
{
    FT("Magick::Magick", (inargc, "(char **) inargv"));
    i_shutdown = false;
    services_dir=wxGetCwd();
    config_file="magick.ini";
    for(int i=0;i<inargc;i++)
	argv.push_back(inargv[i]);

    low_water_mark=100;   // To be deciphered at runtime later
    high_water_mark=200;
}

int Magick::Start()
{
    unsigned int i;
    int Result;
    // this is our main routine, when it leaves here, this sucker's done.
#ifdef ACE_DEBUGGING
    ACE_Log_Msg::instance()->open("Magick");
    ACE_Log_Msg::enable_debug_messages();
#endif

    // We log to STDERR until we find our logfile...
    logger=new wxLogStderr();

    // more stuff to do
    ProgramName=argv[0].RevAfter("/");
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
		services_dir=argv[i];
	    }
	    else if(argv[i]=="--config")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    // use static errors here because conf directory is not known yet
		    wxLogFatal("--config requires a paramter.");
		}
		config_file=argv[i];
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
		short level = makehex(argv[i].After(":"));
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
    if (chdir (services_dir) < 0)
    {
        perror (services_dir);
#ifdef WIN32
        WSACleanup ();
#endif
        RET(MAGICK_RET_INVALID_SERVICES_DIR);
    }

    // Check for \ or / or ?: in first chars.
    if ((errstring = config_file[0u]) == DirSlash || config_file[1u] == ':')
	errstring=config_file;
    else
	errstring=services_dir+DirSlash+config_file;

    {
	FILE *TmpHand;
	if ((TmpHand = fopen(errstring, "r")) == NULL)
	    wxLogFatal("Configuration file (%s) not found.", errstring.c_str());
	else
	    fclose(TmpHand);
    }
    MagickIni=new wxFileConfig("magick","",errstring);
    errstring = "";

    if(MagickIni==NULL)
    {
	wxLogFatal("Major fubar, couldn't allocate memory to read config file.");
    }
    //okay, need a function here to load all the ini file defalts
    get_config_values();
    if(i_shutdown==true)
	wxLogFatal("CONFIG: [Startup] STOP code received.");

    Result=doparamparse();
    if(Result!=MAGICK_RET_NORMAL)
	RET(Result);

    if(!check_config())
	RET(MAGICK_RET_TERMINATE);

    FILE *logfile = fopen((services_dir+DirSlash+Files_LOGFILE).c_str(), "w+");
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
    pidfile.Create(Files_PIDFILE.Strip(mstring::stBoth),true);
    if(pidfile.IsOpened())
    {
	mstring dummystring;
	dummystring<<getpid();
	pidfile.Write(dummystring);
	pidfile.Close();
    }
    /*else
	log_perror ("Warning: cannot write to PID file %s", Files_PIDFILE);*/

    // okay here we start setting up the ACE_Reactor and ACE_Event_Handler's
    signalhandler=new SignalHandler(this);
    ACE_Reactor::instance()->register_handler(SIGINT,signalhandler);
#if defined(SIGTERM) && (SIGTERM != 0)
    ACE_Reactor::instance()->register_handler(SIGTERM,signalhandler);
#endif
#if defined(SIGPIPE) && (SIGPIPE != 0)
    ACE_Sig_Action sigpipe (ACE_SignalHandler (SIG_IGN), SIGPIPE);
    ACE_UNUSED_ARG (sigpipe);
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
    ACE_Sig_Action sigalrm (ACE_SignalHandler (SIG_IGN), SIGALRM);
    ACE_UNUSED_ARG (sigalrm);
#endif
#ifdef SIGCHLD
    ACE_Sig_Action sigchld (ACE_SignalHandler (SIG_IGN), SIGCHLD);
    ACE_UNUSED_ARG (sigchld);
#endif
#ifdef SIGWINCH
    ACE_Sig_Action sigwinch (ACE_SignalHandler (SIG_IGN), SIGWINCH);
    ACE_UNUSED_ARG (sigwinch);
#endif
#ifdef SIGTTIN
    ACE_Sig_Action sigttin (ACE_SignalHandler (SIG_IGN), SIGTTIN);
    ACE_UNUSED_ARG (sigttin);
#endif
#ifdef SIGTTOU
    ACE_Sig_Action sigttou (ACE_SignalHandler (SIG_IGN), SIGTTOU);
    ACE_UNUSED_ARG (sigttou);
#endif
#ifdef SIGTSTP
    ACE_Sig_Action sigttsp (ACE_SignalHandler (SIG_IGN), SIGTSTP);
    ACE_UNUSED_ARG (sigttsp);
#endif

    mBase::init(this);

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

    CP(("Magick II has been started ..."));

    //this little piece of code creates the actual connection from magick
    // to the irc server and sets up the socket handler that receives
    // incoming data and pushes it out to the appropriate service.

    ACE_INET_Addr addr(Startup_REMOTE_PORT,Startup_REMOTE_SERVER);
    IrcServer server(ACE_Reactor::instance(),ACE_NONBLOCK);
    //IrcServer server(ACE_Reactor::instance());
    ircsvchandler=new IrcSvcHandler;
    ircsvchandler->Parent=this;
    if(server.connect(ircsvchandler,addr)==-1)
    {
	//okay we got a connection problem here. log it and shutdown
	RET(MAGICK_RET_TERMINATE);
    }
    // if the blocking version is used, use below
    /*
    ircsvchandler->send(mstring); // ie server line.
    */

    // next thing to be done here is set up the acceptor mechanism to listen
    // for incoming "magickgui" connections and handle them.


    // not so temporary event handling mechanism
    //todo: while( below!=-1(TimeValue) { do cleanup's } that way every
    // say 5 mins or so it breaks from the event loop to cleanup
    ACE_Reactor::instance()->run_event_loop();

    mBase::shutdown();
    ircsvchandler->shutdown();
    //trace::shutdown();

    //todo work out some way to "ignore" signals
    ACE_Reactor::instance()->remove_handler(SIGINT);
#if defined(SIGTERM) && (SIGTERM != 0)
    ACE_Reactor::instance()->remove_handler(SIGTERM);
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
         << "Magick IRC Services\n"
         << "    (c) 1996-2000 Preston A. Elder <prez@magick.tm>\n"
         << "    (c) 1999-2000 William King <ungod@magick.tm>\n"
         << "\n"
         << "Syntax: " << ProgramName << " [ops]\n"
         << "\n"
         << "    -c, --connect server[:port]    Override REMOTE_SERVER and REMOTE_PORT\n"
         << "                                   values in the config file.\n"
         << "    -n, --name servername          Override SERVER_NAME in the config file.\n"
         << "    -d, --desc description         Override SERVER_DESC in the config file.\n"
         << "    -u, --user user                Override SERVICES_USER in the config file.\n"
         << "    -o, --ownuser                  Sets OWNUSER in the config file to TRUE.\n"
         << "    -h, --host host                Override SERVICES_HOST in the config file.\n"
         << "    -s, --save time                Override CYCLETIME in the config file.\n"
         << "    -v, --verbose                  Logs EVERY command to services.\n"
         << "    -l, --level level              Override LEVEL in config file.\n"
         << "    -g, --gmt offset               Override GMT in config file.\n"
         << "    -f, --fg                       Output logs to console, fork() on CTRL-C.\n"
         << "    -r, --relink time              Override SERVER_RELINK in config file.\n"
         << "        --norelink                 Set SERVER_RELINK to 0.\n"
         << "        --dir directory            Change working (database) directory.\n"
         << "	--config config            Specify filename for config file.\n"
         << "	--log logfile              Output logs to different file.\n"
         << "	--trace type:level         Turn on tracing from the onset.\n"
         << "\n"
         << "For more help on the usage of Magick, please browse the docs directory.\n"
         << "This software is free to the public, no registration or licensing fee\n"
         << "may be charged under any circumstances.  Media charges may be imposed.\n"
	 << "\n";
    
}

void Magick::LoadInternalMessages()
{
    NFT("Magick::LoadInternalMessages");
    /* This is to be replaced with language files.
       blah.lng (and magick.ini has LANGUAGE=blah
       Another file should be created for LOGMSG=blahlog
       for log messages (user display and log messages
       should be seperated */
    /*todo: okay, store the string from language.h into a temp file
	parse it in using getfirstgroup/getnextgroup/getfirstvalue/getnextvalue
	to get both the names and the values and store them into two arrays
	one our name array, the other  our value array*/


    /* note left side of message can have spaces before '=' that will be trimmed
	right side will *not* be trimmed*/

    WLOCK lock("Magick","LoadMessages");
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
    MLOCK lock("Magick","parseEscapes");
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
    WLOCK lock("Magick","LoadMessages");
    // need to transfer wxGetWorkingDirectory() and prepend it to english.lng
    wxFileConfig fconf("magick","",services_dir+DirSlash+"lang"+DirSlash+Files_LANGUAGE+".lng");
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
		--connect X	-c	--remote
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
		--gmt X		-g
		--save X	-s	--update
		--help		-?
	    */

	    bool ArgUsed=false;
	    if (argv[i][1U]=='-')
		ArgUsed=paramlong(argv[i], (i-1 < argc) ? argv[i+1] : "");
	    else
		ArgUsed=paramshort(argv[i], (i-1 < argc) ? argv[i+1] : "");
	    
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
    else if(first=="--connect" || first=="--remote")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal("--connect requires a hostname[:port]");
	}
	if(second.After(':').IsNumber())
	{
	    if(atoi(second.After(':').c_str())<=0)
		wxLogError("port must be a positive number (ignoring)");
	    else
		Startup_REMOTE_PORT=atoi(second.After(':').c_str());
	} else
	    wxLogError("port must be a number");
	Startup_REMOTE_SERVER=second.Before(':');
	RET(true);
    }
    else if(first=="--name")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--name");
	}
	Startup_SERVER_NAME=second;
	RET(true);
    }
    else if(first=="--desc")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--desc");
	}
	Startup_SERVER_DESC=second;
	RET(true);
    }
    else if(first=="--user")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--user");
	}
	Startup_SERVICES_USER=second;
	RET(true);
    }
    else if(first=="--ownuser")
    {
	Startup_OWNUSER=true;
    }
    else if(first=="--host")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--host");
	}
	Startup_SERVICES_HOST=second;
    }
    else if(first=="--log")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--log");
	}
	Files_LOGFILE=second;
	RET(true);
    }
    else if(first=="--verbose" || first=="--debug")
	debug=true;
    else if(first=="--fg" || first=="--foreground"
	    || first=="--live" || first=="--nofork")
	live=true;
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
	Config_SERVER_RELINK=atoi(first.c_str());
	RET(true);
    }
    else if(first=="--norelink")
	Config_SERVER_RELINK=0;
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
	Startup_LEVEL=atoi(second.c_str());
	RET(true);
    }
    else if(first=="--gmt")
    {
	if(second.IsEmpty() || second[0U]=='-')
	{
	    wxLogFatal(getMessage("ERR_REQ_PARAM").c_str(),"--gmt");
	}
	if(abs(atoi(second.c_str()))>=12)
	{
	    wxLogFatal("--gmt must be between -12 and 12");
	}
	Startup_GMT=atoi(second.c_str());
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
	Config_CYCLETIME=atoi(second.c_str());
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
	else if (first[i]=='c')
	{
	    if (ArgUsed)
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--connect", second);
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
	else if(first[i]=='g')
	{
	    if (ArgUsed)
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--gmt", second);
	}
	else if(first[i]=='s')
	{
	    if (ArgUsed)
		wxLogFatal("Paramater may only be used once");
	    else
		ArgUsed = paramlong ("--save", second);
	}
	else
	{
	    wxLogError("Unknown option -%c, ignoring.",first[i]);
	}
    }
    RET(ArgUsed);
}

bool Magick::check_config()
{
    NFT("Magick::check_config");
    // change these later when the appropriate classes are set up
    if(!Services_OperServ.IsEmpty())
    {
	Services_FLOOD=false;
	Services_AKILL=false;
	Services_OPERDENY=false;
    }
    if (Startup_LEVEL < 1)
    {
	// change this to the logging mechanism
	wxLogFatal("CONFIG: Cannot set [Startup] LEVEL < 1");
    }
    if (Startup_GMT >= 12 || Startup_GMT <= -12)
    {
	// change this to the logging mechanism
        wxLogFatal("CONFIG: [Startup] GMT must fall between -12 and 12.");
    }
    if (Config_CYCLETIME < 30)
    {
	// change this to the logging mechanism
        wxLogFatal("CONFIG: Cannot set [Config] CYCLETIME < 30.");
    }
    if (Startup_LAGTIME < 1)
    {
	// change this to the logging mechanism
        wxLogFatal("CONFIG: Cannot set [Startup] LAGTIME < 1.");
    }
    if (nickserv.passfail < 1)
    {
	// change this to the logging mechanism
        wxLogFatal("CONFIG: Cannot set [NickServ] PASSFAIL < 1.");
    }
    RET(true);

}

void Magick::get_config_values()
{
    NFT("Magick::get_config_values");
    if(MagickIni==NULL)
    {
	i_shutdown==true;
	return;
    }
    wxFileConfig& in=*MagickIni;

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

    in.Read(ts_Startup+"REMOTE_SERVER",&Startup_REMOTE_SERVER,"127.0.0.1");
    in.Read(ts_Startup+"REMOTE_PORT",&Startup_REMOTE_PORT,9666);
    in.Read(ts_Startup+"PASSWORD",&Startup_PASSWORD,"");
    in.Read(ts_Startup+"SERVER_NAME",&Startup_SERVER_NAME,"services.magick.tm");
    in.Read(ts_Startup+"SERVER_DESC",&Startup_SERVER_DESC,"Magick IRC Services");
    in.Read(ts_Startup+"SERVICES_USER",&Startup_SERVICES_USER,"services");
    in.Read(ts_Startup+"SERVICES_HOST",&Startup_SERVICES_HOST,"magick.tm");
    in.Read(ts_Startup+"OWNUSER",&Startup_OWNUSER,false);
    in.Read(ts_Startup+"LEVEL",&Startup_LEVEL,1);
    in.Read(ts_Startup+"LAGTIME",&Startup_LAGTIME,10);
    in.Read(ts_Startup+"DEADTIME",&Startup_DEADTIME,30);
    in.Read(ts_Startup+"GMT",&Startup_GMT,0.0);
    in.Read(ts_Startup+"STOP",&i_shutdown,true);

    in.Read(ts_Services+"NickServ",&nickserv.names,"NickServ");
    in.Read(ts_Services+"NickServ_Name",&nickserv.realname,"Nickname Service");
    in.Read(ts_Services+"ChanServ",&chanserv.names,"ChanServ");
    in.Read(ts_Services+"ChanServ_Name",&chanserv.realname,"Channel Service");
    in.Read(ts_Services+"MemoServ",&Services_MemoServ,"MemoServ");
    in.Read(ts_Services+"MemoServ_Name",&Services_MemoServ_Name,"Memo/News Service");
    in.Read(ts_Services+"MEMO",&Services_MEMO,true);
    in.Read(ts_Services+"NEWS",&Services_NEWS,true);
    in.Read(ts_Services+"OperServ",&Services_OperServ,"OperServ");
    in.Read(ts_Services+"OperServ_Name",&Services_OperServ_Name,"Operator Service");
    in.Read(ts_Services+"FLOOD",&Services_FLOOD,true);
    in.Read(ts_Services+"AKILL",&Services_AKILL,true);
    in.Read(ts_Services+"OPERDENY",&Services_OPERDENY,true);
    in.Read(ts_Services+"CommServ",&Services_CommServ,"CommServ");
    in.Read(ts_Services+"CommServ_Name",&Services_CommServ_Name,"Committee Service");
    in.Read(ts_Services+"ServMsg",&Services_ServMsg,"GlobalMsg DevNull");
    in.Read(ts_Services+"ServMsg_Name",&Services_ServMsg_Name,"Magick <--> User");
    in.Read(ts_Services+"SHOWSYNC",&Services_SHOWSYNC,true);

    in.Read(ts_Files+"PIDFILE",&Files_PIDFILE,"magick.pid");
    in.Read(ts_Files+"LOGFILE",&Files_LOGFILE,"magick.log");
    in.Read(ts_Files+"MOTDFILE",&Files_MOTDFILE,"magick.motd");
    in.Read(ts_Files+"LANGUAGE",&Files_LANGUAGE,"english");
    in.Read(ts_Files+"COMMANDS",&Files_COMMANDS,"default");
    in.Read(ts_Files+"LINK_DB",&Files_LINK_DB,"link.db");
    in.Read(ts_Files+"NICK_DB",&Files_NICK_DB,"nick.db");
    in.Read(ts_Files+"CHAN_DB",&Files_CHAN_DB,"chan.db");
    in.Read(ts_Files+"MEMO_DB",&Files_MEMO_DB,"memo.db");
    in.Read(ts_Files+"NEWS_DB",&Files_NEWS_DB,"news.db");
    in.Read(ts_Files+"AKILL_DB",&Files_AKILL_DB,"akill.db");
    in.Read(ts_Files+"IGNORE_DB",&Files_IGNORE_DB,"ignore.db");
    in.Read(ts_Files+"CLONE_DB",&Files_CLONE_DB,"clone.db");
    in.Read(ts_Files+"COMM_DB",&Files_COMM_DB,"comm.db");
    in.Read(ts_Files+"MSGS_DB",&Files_MSGS_DB,"msgs.db");

    in.Read(ts_Config+"SERVER_RELINK",&Config_SERVER_RELINK,5);
    in.Read(ts_Config+"CYCLETIME",&Config_CYCLETIME,300);
    in.Read(ts_Config+"PING_FREQUENCY",&Config_PING_FREQUENCY,30);
    in.Read(ts_Config+"STARTHRESH",&Config_STARTHRESH, 4);

    in.Read(ts_NickServ+"EXPIRE",&nickserv.expire,28);
    in.Read(ts_NickServ+"RELEASE",&nickserv.release,60);
    in.Read(ts_NickServ+"PASSFAIL",&nickserv.passfail,5);
    in.Read(ts_NickServ+"DEF_KILL",&nickserv.def_kill,true);
    in.Read(ts_NickServ+"LCK_KILL",&nickserv.lck_kill,false);
    in.Read(ts_NickServ+"DEF_PRIVMSG",&nickserv.def_privmsg,false);
    in.Read(ts_NickServ+"LCK_PRIVMSG",&nickserv.lck_privmsg,false);
    in.Read(ts_NickServ+"DEF_PRIVATE",&nickserv.def_private,false);
    in.Read(ts_NickServ+"LCK_PRIVATE",&nickserv.lck_private,false);
    in.Read(ts_NickServ+"DEF_SECURE",&nickserv.def_secure,false);
    in.Read(ts_NickServ+"LCK_SECURE",&nickserv.lck_secure,false);

    in.Read(ts_ChanServ+"EXPIRE",&chanserv.expire,14);
    in.Read(ts_ChanServ+"DEF_AKICK",&chanserv.def_akick_reason,"You have been banned from channel");
    in.Read(ts_ChanServ+"CHANKEEP",&chanserv.chankeep,15);
    in.Read(ts_ChanServ+"DEF_MLOCK",&chanserv.def_mlock,"+nt");
    in.Read(ts_ChanServ+"LCK_MLOCK",&chanserv.lck_mlock,"+");
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
    in.Read(ts_ChanServ+"DEF_RESTRICTED",&chanserv.def_restricted,false);
    in.Read(ts_ChanServ+"LCK_RESTRICTED",&chanserv.lck_restricted,false);
    in.Read(ts_ChanServ+"DEF_JOIN",&chanserv.def_join,false);
    in.Read(ts_ChanServ+"LCK_JOIN",&chanserv.lck_join,false);
    in.Read(ts_ChanServ+"DEF_REVENGE",&chanserv.def_revenge,"NONE");
    in.Read(ts_ChanServ+"LCK_REVENGE",&chanserv.lck_revenge,false);
    in.Read(ts_ChanServ+"LEVEL_MIN",&chanserv.level_min,-1);
    in.Read(ts_ChanServ+"LEVEL_MAX",&chanserv.level_max,30);
    in.Read(ts_ChanServ+"LVL_AUTODEOP",&chanserv.lvl_autodeop,-1);
    in.Read(ts_ChanServ+"LVL_AUTOVOICE",&chanserv.lvl_autovoice,5);
    in.Read(ts_ChanServ+"LVL_AUTOOP",&chanserv.lvl_autoop,10);
    in.Read(ts_ChanServ+"LVL_READMEMO",&chanserv.lvl_readmemo,0);
    in.Read(ts_ChanServ+"LVL_WRITEMEMO",&chanserv.lvl_writememo,15);
    in.Read(ts_ChanServ+"LVL_DELMEMO",&chanserv.lvl_delmemo,25);
    in.Read(ts_ChanServ+"LVL_AKICK",&chanserv.lvl_akick,20);
    in.Read(ts_ChanServ+"LVL_STARAKICK",&chanserv.lvl_starakick,25);
    in.Read(ts_ChanServ+"LVL_UNBAN",&chanserv.lvl_unban,10);
    in.Read(ts_ChanServ+"LVL_ACCESS",&chanserv.lvl_access,5);
    in.Read(ts_ChanServ+"LVL_SET",&chanserv.lvl_set,25);
    in.Read(ts_ChanServ+"LVL_CMDINVITE",&chanserv.lvl_cmdinvite,5);
    in.Read(ts_ChanServ+"LVL_CMDUNBAN",&chanserv.lvl_cmdunban,10);
    in.Read(ts_ChanServ+"LVL_CMDVOICE",&chanserv.lvl_cmdvoice,5);
    in.Read(ts_ChanServ+"LVL_CMDOP",&chanserv.lvl_cmdop,10);
    in.Read(ts_ChanServ+"LVL_CMDCLEAR",&chanserv.lvl_cmdclear,20);

    in.Read(ts_MemoServ+"NEWS_EXPIRE",&MemoServ_NEWS_EXPIRE,21);

    in.Read(ts_OperServ+"SERVICES_ADMIN",&OperServ_SERVICES_ADMIN,"");
    in.Read(ts_OperServ+"EXPIRE_OPER",&OperServ_EXPIRE_OPER,"1d");
    in.Read(ts_OperServ+"EXPIRE_ADMIN",&OperServ_EXPIRE_ADMIN,"7d");
    in.Read(ts_OperServ+"EXPIRE_SOP",&OperServ_EXPIRE_SOP,"1m");
    in.Read(ts_OperServ+"EXPIRE_SADMIN",&OperServ_EXPIRE_SADMIN,"1y");
    in.Read(ts_OperServ+"CLONE_LIMIT",&OperServ_CLONE_LIMIT,2);
    in.Read(ts_OperServ+"DEF_CLONE",&OperServ_DEF_CLONE,"Maximum connections from one host exceeded");
    in.Read(ts_OperServ+"FLOOD_TIME",&OperServ_FLOOD_TIME,10);
    in.Read(ts_OperServ+"FLOOD_MSGS",&OperServ_FLOOD_MSGS,5);
    in.Read(ts_OperServ+"IGNORE_TIME",&OperServ_IGNORE_TIME,20);
    in.Read(ts_OperServ+"IGNORE_LIMIT",&OperServ_IGNORE_LIMIT,5);
    in.Read(ts_OperServ+"IGNORE_REMOVE",&OperServ_IGNORE_REMOVE,300);
    in.Read(ts_OperServ+"IGNORE_METHOD",&OperServ_IGNORE_METHOD,8);

    in.Read(ts_CommServ+"SECURE_OPER",&CommServ_SECURE_OPER,true);

    CP(("%s read and loaded to live configuration.", config_file.c_str()));
}

int SignalHandler::handle_signal(int signum, siginfo_t *siginfo, ucontext_t *ucontext)
{
    FT("SignalHandler::handle_signal", (signum, "(siginfo_t *) siginfo", "(ucontext_t *) ucontext"));
    // todo: fill this sucker in
    switch(signum)
    {
    case 0:		// this is here to show up clashes for badly defined signal constants
	break;
    case SIGINT:	// CTRL-C, Background.
	Parent->shutdown(true);	// Temp, we just kill on CTRL-C
	break;
#if defined(SIGTERM) && (SIGTERM != 0)
    case SIGTERM:	// Save DB's (often prequil to -KILL!)
	break;
#endif
#if defined(SIGQUIT) && (SIGQUIT != 0)
    case SIGQUIT:	// Terminal dead, Background.
	break;
#endif
    case SIGSEGV:	// Segfault, validate all storage.
	break;
#ifdef SIGBUS
    case SIGBUS:	// Ignore
	break;
#endif
#if defined(SIGHUP) && (SIGHUP != 0)
    case SIGHUP:	// Reload CFG/DB's
	break;
#endif
    case SIGILL:	// Re-try last call.
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
    default:		// Everything else.
	;//ignore (todo log that we got it and we're ignoring it)
    }
    RET(0);
}

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

bool Magick::shutdown()
{
    return i_shutdown;
}

void Magick::shutdown(bool in)
{
    i_shutdown=in;
    if(in==true)
    {
	ACE_Reactor::instance()->end_event_loop();
    }
}
