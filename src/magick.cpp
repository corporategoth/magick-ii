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
    services_dir=wxGetCwd();
    config_file="magick.ini";
    for(int i=0;i<inargc;i++)
	argv.push_back(inargv[i]);

    low_water_mark=20;   // To be deciphered at runtime later
    high_water_mark=25;
    reconnect=true;
    events.open();
    loggertask.open();
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

    FILE *logfile = fopen((services_dir+DirSlash+files.Logfile()).c_str(), "a");
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
    GotConnect = false;
    Parent->Server = tmp;
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
    while(this->i_shutdown==false)
    {
	ACE_Reactor::instance()->run_event_loop();
    }

    mBase::shutdown();
    //ircsvchandler->shutdown();
    //trace::shutdown();

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
         << "    -g, --gmt offset               Override GMT in config file.\n"
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
    wxFileConfig fconf("magick","",services_dir+DirSlash+"lang"+DirSlash+nickserv.DEF_Language()+".lng");
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
		--gmt X		-g
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
	startup.gmt=atoi(second.c_str());
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

bool Magick::check_config()
{
    NFT("Magick::check_config");
    // change these later when the appropriate classes are set up
    if(operserv.names.IsEmpty())
    {
	operserv.flood=false;
	operserv.akill=false;
	operserv.operdeny=false;
    }
    if (startup.Level() < 1)
    {
	// change this to the logging mechanism
	wxLogFatal("CONFIG: Cannot set [Startup] LEVEL < 1");
    }
    if (config.Cycletime() < 30)
    {
	// change this to the logging mechanism
        wxLogFatal("CONFIG: Cannot set [Config] CYCLETIME < 30.");
    }
    if (startup.Lagtime() < 1)
    {
	// change this to the logging mechanism
        wxLogFatal("CONFIG: Cannot set [Startup] LAGTIME < 1.");
    }
    if (nickserv.Passfail() < 1)
    {
	// change this to the logging mechanism
        wxLogFatal("CONFIG: Cannot set [NickServ] PASSFAIL < 1.");
    }
    if (chanserv.Passfail() < 1)
    {
	// change this to the logging mechanism
        wxLogFatal("CONFIG: Cannot set [ChanServ] PASSFAIL < 1.");
    }
    // todo check if the keyfile exists and load in the encryption/decryption password
    RET(true);

}

void Magick::get_config_values()
{
    NFT("Magick::get_config_values");
    bool verbose;
    unsigned int i;
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

    in.Read(ts_Startup+"SERVER_NAME",&startup.server_name,"services.magick.tm");
    in.Read(ts_Startup+"SERVER_DESC",&startup.server_desc,FULL_NAME);
    in.Read(ts_Startup+"SERVICES_USER",&startup.services_user,"services");
    in.Read(ts_Startup+"SERVICES_HOST",&startup.services_host,"magick.tm");
    in.Read(ts_Startup+"OWNUSER",&startup.ownuser,false);

    // REMOTE entries
    mstring ent="";
    i=1;
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
		    startup.servers[tmp[0].LowerCase()] = triplet<int,mstring,int>(atoi(tmp[1]),tmp[2],atoi(tmp[3]));
		else
		    wxLogWarning("REMOTE entry for %s contained incorrect syntax, ignored!", tmp[0].c_str());
	}
	i++;
    } while (ent!="");

    in.Read(ts_Startup+"LEVEL",&startup.level,1);
    level = startup.level;
    in.Read(ts_Startup+"LAGTIME",&startup.lagtime,10);
    in.Read(ts_Startup+"STOP",&i_shutdown,true);

    in.Read(ts_Services+"NickServ",&nickserv.names,"NickServ");
    in.Read(ts_Services+"NickServ_Name",&nickserv.realname,"Nickname Service");
    in.Read(ts_Services+"ChanServ",&chanserv.names,"ChanServ");
    in.Read(ts_Services+"ChanServ_Name",&chanserv.realname,"Channel Service");
    in.Read(ts_Services+"MemoServ",&memoserv.names,"MemoServ");
    in.Read(ts_Services+"MemoServ_Name",&memoserv.realname,"Memo/News Service");
    in.Read(ts_Services+"MEMO",&memoserv.memo,true);
    in.Read(ts_Services+"NEWS",&memoserv.news,true);
    in.Read(ts_Services+"OperServ",&operserv.names,"OperServ");
    in.Read(ts_Services+"OperServ_Name",&operserv.realname,"Operator Service");
    in.Read(ts_Services+"FLOOD",&operserv.flood,true);
    in.Read(ts_Services+"AKILL",&operserv.akill,true);
    in.Read(ts_Services+"OPERDENY",&operserv.operdeny,true);
    in.Read(ts_Services+"CommServ",&commserv.names,"CommServ");
    in.Read(ts_Services+"CommServ_Name",&commserv.realname,"Committee Service");
    in.Read(ts_Services+"ServMsg",&servmsg.names,"GlobalMsg DevNull");
    in.Read(ts_Services+"ServMsg_Name",&servmsg.realname,PRODUCT + " <--> User");
    in.Read(ts_Services+"SHOWSYNC",&servmsg.showsync,true);

    in.Read(ts_Files+"PIDFILE",&files.pidfile,"magick.pid");
    in.Read(ts_Files+"LOGFILE",&files.logfile,"magick.log");
    in.Read(ts_Files+"VERBOSE", &verbose, false);
    logger->SetVerbose(verbose);
    in.Read(ts_Files+"MOTDFILE",&files.motdfile,"magick.motd");
    in.Read(ts_Files+"DATABASE",&files.database,"magick.mnd");
    in.Read(ts_Files+"COMPRESSION",&files.compression,6);
    in.Read(ts_Files+"KEYFILE",&files.keyfile,"");
    in.Read(ts_Files+"ENCRYPTION",&files.encryption,false);

    in.Read(ts_Config+"SERVER_RELINK",&config.server_relink,5);
    in.Read(ts_Config+"SQUIT_PROTECT",&config.squit_protect,120);
    in.Read(ts_Config+"SQUIT_CANCEL",&config.squit_cancel,10);
    in.Read(ts_Config+"CYCLETIME",&config.cycletime,300);
    in.Read(ts_Config+"CHECKTIME",&config.checktime,5);
    in.Read(ts_Config+"PING_FREQUENCY",&config.ping_frequency,30);
    in.Read(ts_Config+"STARTHRESH",&config.starthresh, 4);
    in.Read(ts_Config+"STARTUP_THREADS",&config.startup_threads, 2);
    in.Read(ts_Config+"LOW_WATER_MARK",&config.low_water_mark, 20);
    in.Read(ts_Config+"HIGH_WATER_MARK",&config.high_water_mark, 25);

    in.Read(ts_NickServ+"EXPIRE",&nickserv.expire,28);
    in.Read(ts_NickServ+"IDENT",&nickserv.ident,60);
    in.Read(ts_NickServ+"RELEASE",&nickserv.release,60);
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

    in.Read(ts_ChanServ+"EXPIRE",&chanserv.expire,14);
    in.Read(ts_ChanServ+"DEF_AKICK",&chanserv.def_akick_reason,"You have been banned from channel");
    in.Read(ts_ChanServ+"PASSFAIL",&chanserv.passfail,5);
    in.Read(ts_ChanServ+"CHANKEEP",&chanserv.chankeep,15);
    in.Read(ts_ChanServ+"DEF_MLOCK",&chanserv.def_mlock,"+nt");
    in.Read(ts_ChanServ+"LCK_MLOCK",&chanserv.lck_mlock,"+");
    in.Read(ts_ChanServ+"DEF_BANTIME",&chanserv.def_bantime, 0u);
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

    in.Read(ts_MemoServ+"NEWS_EXPIRE",&memoserv.news_expire,21);
    in.Read(ts_MemoServ+"INFLIGHT",&memoserv.inflight,180);
    in.Read(ts_MemoServ+"FILES",&memoserv.files,0);
    in.Read(ts_MemoServ+"FILESIZE",&memoserv.filesize,0);

    in.Read(ts_OperServ+"SERVICES_ADMIN",&operserv.services_admin,"");
    in.Read(ts_OperServ+"SECURE",&operserv.secure,false);
    in.Read(ts_OperServ+"DEF_EXPIRE",&operserv.expire_oper,"3h");
    in.Read(ts_OperServ+"EXPIRE_OPER",&operserv.expire_oper,"1d");
    in.Read(ts_OperServ+"EXPIRE_ADMIN",&operserv.expire_admin,"7d");
    in.Read(ts_OperServ+"EXPIRE_SOP",&operserv.expire_sop,"1m");
    in.Read(ts_OperServ+"EXPIRE_SADMIN",&operserv.expire_sadmin,"1y");
    in.Read(ts_OperServ+"CLONE_MAX",&operserv.clone_limit,50);
    in.Read(ts_OperServ+"CLONE_LIMIT",&operserv.clone_limit,2);
    in.Read(ts_OperServ+"DEF_CLONE",&operserv.def_clone,"Maximum connections from one host exceeded");
    in.Read(ts_OperServ+"FLOOD_TIME",&operserv.flood_time,10);
    in.Read(ts_OperServ+"FLOOD_MSGS",&operserv.flood_msgs,5);

    in.Read(ts_OperServ+"IGNORE_TIME",&operserv.ignore_time,20);
    in.Read(ts_OperServ+"IGNORE_LIMIT",&operserv.ignore_limit,5);
    in.Read(ts_OperServ+"IGNORE_REMOVE",&operserv.ignore_remove,300);
    in.Read(ts_OperServ+"IGNORE_METHOD",&operserv.ignore_method,8);
    
    RemCommands();
    in.Read(ts_CommServ+"DEF_OPENMEMOS",&commserv.def_openmemos,true);
    in.Read(ts_CommServ+"DEF_SECURE",&commserv.def_secure,false);
    in.Read(ts_CommServ+"DEF_PRIVATE",&commserv.def_private,false);
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

    AddCommands();

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
	Parent->shutdown(true);	// Temp, we just kill on CTRL-C
	RET(-1);
	break;
#if defined(SIGTERM) && (SIGTERM != 0)
    case SIGTERM:	// Save DB's (often prequil to -KILL!)
	Parent->shutdown(true);	// Temp, we just kill on CTRL-C
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
	Parent->shutdown(true);	// We've gotta kill her captain, she's breaking up.
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

triplet<int,mstring,int> Magick::startup_t::Server(mstring server)
{
    FT("Magick::startup_t::Server", (server));
    triplet<int,mstring,int> value(0, "", 0);

    if (IsServer(server)) {
	value = servers.find(server.LowerCase())->second;
    }
    NRET(triplet<int.mstring.int>, value);
}

vector<mstring> Magick::startup_t::PriorityList(int pri)
{
    FT("Magick::startup_t::PriorityList", (pri));
    vector<mstring> list;

    map<mstring,triplet<int,mstring,int> >::iterator iter;

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
