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

#include <iostream>
#include <strstream>
#include <ace/INET_Addr.h>
#include "magick.h"
#include "log.h"
#include "EscLexer.hpp"
#include "EscParser.hpp"
#include "lockable.h"

#include <algorithm>
using namespace std;

Magick::Magick(int inargc, char **inargv)
{
    FT("Magick::Magick", (inargc, "(char **) inargv"));
    services_dir=".";
    config_file="magick.ini";
    for(int i=0;i<inargc;i++)
	argv.push_back(inargv[i]);

    low_water_mark=100;   // To be deciphered at runtime later
    high_water_mark=200;
}

int Magick::Start()
{
    NFT("Magick::Start");
    int i;
    int Result;
    // this is our main routine, when it leaves here, this sucker's done.

    CP(("Magick II has been started ..."));
    FILE *logfile=fopen("mdebug.log","w+");
    // the below defaults to stderr if logfile cannot be opened
    wxLogStderr logger(logfile);

    if(bob.StartBob("")==false)
    {
    	/*log that bob couldn't start*/
	bob.bobavail=false;
    }
    // more stuff to do
    ProgramName=argv[0].RevAfter("/");
    int argc=argv.size();
    mstring errstring;
    for(i=1;i<argc;i++)
    {
	/* i've taken out / command switches below cause i hate them myself,
	courtesy of gnu we now have a common switch format -- for long, and - for short*/
    	/*if(argv[i][0]=='/')
	    argv[i][0]='-';*/
	if(argv[i][0U]=='-')
	{
	    argv[i].MakeLower();
	    if(argv[i]=="-dir")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    // use static errors here because conf directory is not known yet
		    cerr<< argv[i-1] <<" requires a paramter."<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		services_dir=argv[i];
	    }
	    else if(argv[i]=="-config")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    // use static errors here because conf directory is not known yet
		    cerr<< argv[i-1] <<" requires a paramter."<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		config_file=argv[i];
	    }
	    else if(argv[i]=="-help"||argv[i]=="--help"||argv[i]=="-?"||argv[i]=="-h")
    	    {
		dump_help(argv[0]);
		RET(MAGICK_RET_NORMAL);
	    }
	}
    }
    if (chdir (services_dir) < 0)
    {
        perror (services_dir);
#ifdef WIN32
        WSACleanup ();
#endif
        RET(MAGICK_RET_INVALID_SERVICES_DIR);
    }

    MagickIni=new wxFileConfig("magick","",config_file);
    if(MagickIni==NULL)
    {
	cerr << "Major fubar, couldn't allocate memory to read config file\nAborting"<<endl;
	RET(MAGICK_RET_ERROR);
    }
    //okay, need a function here to load all the ini file defalts
    get_config_values();
    if(i_shutdown==true)
	RET(MAGICK_RET_ERROR);

    // load the local messages database and internal "default messages"
    // the external messages are part of a separate ini called english.lng (both local and global can be done here too)
    LoadInternalMessages();
    LoadExternalMessages();
    StartTime=Now();


    //todo here if !win32, freopen stdout,stdin, and stderr and spawn off.

    Result=doparamparse();
    if(Result!=MAGICK_RET_NORMAL)
	RET(Result);

    if(!check_config())
	RET(MAGICK_RET_TERMINATE);

    if(logfile!=NULL)
	fclose(logfile);

    //open_log();

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
    pidfile.Create(Files_PIDFILE,true);
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
    signalhandler=new SignalHandler;
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
#if defined(SIGALRM) && (SIGALRM != 0)
    ACE_Sig_Action sigalrm (ACE_SignalHandler (SIG_IGN), SIGALRM);
    ACE_UNUSED_ARG (sigalrm);
#endif
#if defined(SIGUSR2) && (SIGUSR2 != 0)
    ACE_Sig_Action sigusr2 (ACE_SignalHandler (SIG_IGN), SIGUSR2);
    ACE_UNUSED_ARG (sigusr2);
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
#if defined(SIGUSR1) && (SIGUSR1 != 0)
    ACE_Reactor::instance()->register_handler(SIGUSR1,signalhandler);
#endif

    if(strlen(Services_NickServ))
	nickserv.init();
    if(strlen(Services_ChanServ))
	chanserv.init();

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

    //this little piece of code creates the actual connection from magick
    // to the irc server and sets up the socket handler that receives
    // incoming data and pushes it out to the appropriate service.

    ACE_INET_Addr addr(Startup_REMOTE_PORT,Startup_REMOTE_SERVER);
    IrcServer server(ACE_Reactor::instance(),ACE_NONBLOCK);
    ircsvchandler=new IrcSvcHandler;
    if(server.connect(ircsvchandler,addr)==-1)
    {
	//okay we got a connection problem here. log it and shutdown
	RET(MAGICK_RET_TERMINATE);
    }
    
    // next thing to be done here is set up a connector mechanism to listen
    // for incoming "magickgui" connections and handle them.


    // not so temporary event handling mechanism
    //todo: while( below!=-1(TimeValue) { do cleanup's } that way every
    // say 5 mins or so it breaks from the event loop to cleanup
    ACE_Reactor::instance()->run_event_loop();

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
#if defined(SIGUSR1) && (SIGUSR1 != 0)
    ACE_Reactor::instance()->remove_handler(SIGUSR1);
#endif
    delete signalhandler;

    RET(MAGICK_RET_TERMINATE);
}

mstring Magick::getMessage(const mstring & name)
{
    FT("Magick::getMessage", (name));
    RET("");
}

void Magick::dump_help(mstring & progname)
{
    FT("Magick::dump_help", (progname));

    // This needs to be re-written.
    cout<<"Magick IRC Services are copyright (c) 1996-1998 Preston A. Elder, W. King.\n"
	<<"    E-mail: <prez@magick.tm>   IRC: PreZ@RelicNet,Prez@Effnet,Prez@DarkerNet\n"
	<<"    E-mail: <ungod@magick.tm>   IRC: Notagod@Effnet,Ungod@DarkerNet\n"
	<<"This program is free but copyrighted software; see the file COPYING for\n"
	<<"details.  Please do not forget to read ALL the files in the doc directory.\n\n"
	<<"Syntax: "<<progname.c_str()<<" [opts]\n\n"
	<<"-remote server[:port]   Connect to the specified server.\n"
	<<"-name servername        Our server name (e.g. hell.darker.net).\n"
	<<"-desc string            Description of us (e.g. DarkerNet Services).\n"
	<<"-user username          Username for Services' nicks (e.g. services).\n"
	<<"-host hostname          Hostname for Services' nicks (e.g. darker.net).\n"
	<<"-prefix prefix          Prefix for Magick Outlet (see magick.ini)\n"
	<<"                            (e.g. \"Magick-\").\n"
	<<"-dir directory          Directory containing Services' data files\n"
	<<"                            (e.g. /usr/local/lib/services).\n"
	<<"-config filename        Config file to be used (e.g. magick.ini).\n"
	<<"-log filename           Services log filename (e.g. magick.log).\n"
	<<"-update secs            How often to update databases, in seconds.\n"
	<<"-debug                  Enable debugging mode--more info sent to log.\n"
	<<"-relink secs            Specify seconds to re-attempt link if SQUIT.\n"
	<<"-norelink               Die on SQUIT or kill -1.\n"
	<<"-level level            Specify services level (>1 = backup).\n"
	<<"-offset hours           Specify the TimeZone offset for backups.\n"
	<<"-live                   Dont fork (log screen + log file).\n"
	<<endl;
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

    wxFileOutputStream *fostream=new wxFileOutputStream("tmplang.lng");
    for(i=0;i<def_langent;i++)
	*fostream<<def_lang[i]<<"\n";
    fostream->Sync();
    delete fostream;
    wxFileConfig fconf("magick","","tmplang.lng");
    bool bContGroup, bContEntries;
    long dummy1,dummy2;
    mstring groupname,entryname;
    bContGroup=fconf.GetFirstGroup(groupname,dummy1);
    while(bContGroup)
    {
	bContEntries=fconf.GetFirstEntry(entryname,dummy2);
	while(bContEntries)
	{
    	    MessageNamesLong.push_back(groupname+"/"+entryname);
	    MessageNamesShort.push_back(entryname);
	    bContEntries=fconf.GetNextEntry(entryname,dummy2);
	    Messages[entryname]=fconf.Read(groupname+"/"+entryname,"");
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
    wxFileConfig fconf("magick","","tmplang.lng");
    int i;
    for(i=0;i<MessageNamesLong.size();i++)
    	Messages[MessageNamesShort[i]]=fconf.Read(MessageNamesLong[i],Messages[MessageNamesShort[i]]);
}

int Magick::doparamparse()
{
    NFT("Magick::doparamparse");
    mstring temp;
    int argc=argv.size(),i;
    for(i=1;i<argc;i++)
    {
	/* i've taken out / command switches below cause i hate them myself,
	courtesy of gnu we now have a common switch format -- for long, and - for short*/
    	/*if(argv[i][0]=='/')
	    argv[i][0]='-';*/
	if(argv[i][0U]=='-')
	{
	    argv[i].MakeLower();
	    if(argv[i]=="-remote")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    cerr<<"-remote"<<" requires hostname[:port]"<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		if(argv[i].Contains(":"))
		{
		    if(argv[i].After(':').IsNumber())
		    {
			if(atoi(argv[i].After(':').c_str())<0)
			    cerr<<"port must be a positive number"<<endl;
			else
			    Startup_REMOTE_PORT=atoi(argv[i].After(':').c_str());
		    }
		    Startup_REMOTE_SERVER=argv[i].Before(':');
		}
	    }
	    else if(argv[i]=="-name")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-name");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		Startup_SERVER_NAME=argv[i];
	    }
	    else if(argv[i]=="-desc")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-desc");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		Startup_SERVER_DESC=argv[i];
	    }
	    else if(argv[i]=="-user")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-user");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		Startup_SERVICES_USER=argv[i];
	    }
	    else if(argv[i]=="-host")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-host");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		Startup_SERVICES_HOST=argv[i];
	    }
	    else if(argv[i]=="-dir")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-dir");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		// already handled, but we needed to i++
	    }
	    else if(argv[i]=="-config")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-config");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		// already handled, but we needed to i++
	    }
	    else if(argv[i]=="-log")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-log");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		Files_LOGFILE=argv[i];
	    }
	    else if(argv[i]=="-debug")
		debug=true;
	    else if(argv[i]=="-live"||argv[i]=="nofork")
		live=true;
	    else if(argv[i]=="-relink")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-relink");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		if(atoi(argv[i].c_str())<0)
		{
		    cerr<<"-relink"<<" parameter must be positive"<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		Config_SERVER_RELINK=atoi(argv[i].c_str());
	    }
	    else if(argv[i]=="-norelink")
		Config_SERVER_RELINK=-1;
	    else if(argv[i]=="-level")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-level");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		if(atoi(argv[i].c_str())<0)
		{
		    cerr<<"-level"<<" parameter must be positive"<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		Startup_LEVEL=atoi(argv[i].c_str());
	    }
	    else if(argv[i]=="-gmt")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-gmt");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		if(abs(atoi(argv[i].c_str()))>12)
		{
		    cerr<<"-offset"<<" must be between -12 and 12"<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		Startup_GMT=atoi(argv[i].c_str());
	    }
	    else if(argv[i]=="-update")
	    {
		i++;
		if(i==argc||argv[i][0U]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-update");
		    cerr<<temp<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		if(atoi(argv[i].c_str())<0)
		{
		    cerr<<"-update"<<": number of seconds must be positive"<<endl;
		    RET(MAGICK_RET_ERROR);
		}
		Config_CYCLETIME=atoi(argv[i].c_str());
	    }
	    else
	    {
		temp.Format("Unknown option %s.",argv[i].c_str());
		cerr<<temp<<endl;
		RET(MAGICK_RET_ERROR);
	    }
	}
	else
	{
	    cerr<<"Non-option arguments not allowed"<<endl;
	    RET(MAGICK_RET_ERROR);
	}
    }
    RET(MAGICK_RET_NORMAL);
}

bool Magick::check_config()
{
    NFT("Magick::check_config");
    // change these later when the appropriate classes are set up
    if(!strlen(Services_OperServ))
    {
	Services_FLOOD=false;
	Services_AKILL=false;
	Services_OPERDENY=false;
    }
    if (Startup_LEVEL < 1)
    {
	// change this to the logging mechanism
        cerr<<"CONFIG: Cannot set [Startup] LEVEL < 1"<<endl;
        RET(false);
    }
    if (Startup_GMT >= 12 || Startup_GMT <= -12)
    {
	// change this to the logging mechanism
        cerr<<"CONFIG: [Startup] GMT must fall between -12 and 12."<<endl;
        RET(false);
    }
    if (Config_CYCLETIME < 30)
    {
	// change this to the logging mechanism
        cerr<<"CONFIG: Cannot set [Config] CYCLETIME < 30."<<endl;
        RET(false);
    }
    if (Startup_LAGTIME < 1)
    {
	// change this to the logging mechanism
        cerr<<"CONFIG: Cannot set Startup_LAGTIME < 1."<<endl;
        RET(false);
    }
    if (NickServ_PASSFAIL < 1)
    {
	// change this to the logging mechanism
        cerr<<"CONFIG: Cannot set [NickServ] PASSFAIL < 1."<<endl;
        RET(false);
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
    mstring ts_ChanServ=mstring("NickServ/");
    mstring ts_NickServ=mstring("ChanServ/");
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
    in.Read(ts_Startup+"LEVEL",&Startup_LEVEL,1);
    in.Read(ts_Startup+"LAGTIME",&Startup_LAGTIME,10);
    in.Read(ts_Startup+"DEADTIME",&Startup_DEADTIME,30);
    in.Read(ts_Startup+"GMT",&Startup_GMT,+10.0);
    in.Read(ts_Startup+"STOP",&Startup_STOP,true);

    in.Read(ts_Services+"NickServ",&Services_NickServ,"NickServ");
    in.Read(ts_Services+"NickServ_Name",&Services_NickServ_Name,"Nickname Service");
    in.Read(ts_Services+"ChanServ",&Services_ChanServ,"ChanServ");
    in.Read(ts_Services+"ChanServ_Name",&Services_ChanServ_Name,"Channel Service");
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

    in.Read(ts_NickServ+"EXPIRE",&NickServ_EXPIRE,28);
    in.Read(ts_NickServ+"RELEASE",&NickServ_RELEASE,60);
    in.Read(ts_NickServ+"PASSFAIL",&NickServ_PASSFAIL,5);
    in.Read(ts_NickServ+"DEF_KILL",&NickServ_DEF_KILL,true);
    in.Read(ts_NickServ+"LCK_KILL",&NickServ_LCK_KILL,false);
    in.Read(ts_NickServ+"DEF_PRIVMSG",&NickServ_DEF_PRIVMSG,false);
    in.Read(ts_NickServ+"LCK_PRIVMSG",&NickServ_LCK_PRIVMSG,false);
    in.Read(ts_NickServ+"DEF_PRIVATE",&NickServ_DEF_PRIVATE,false);
    in.Read(ts_NickServ+"LCK_PRIVATE",&NickServ_LCK_PRIVATE,false);
    in.Read(ts_NickServ+"DEF_SECURE",&NickServ_DEF_SECURE,false);
    in.Read(ts_NickServ+"LCK_SECURE",&NickServ_LCK_SECURE,false);

    in.Read(ts_ChanServ+"EXPIRE",&ChanServ_EXPIRE,14);
    in.Read(ts_ChanServ+"DEF_AKICK",&ChanServ_DEF_AKICK,"You have been banned from channel");
    in.Read(ts_ChanServ+"CHANKEEP",&ChanServ_CHANKEEP,15);
    in.Read(ts_ChanServ+"DEF_MLOCK",&ChanServ_DEF_MLOCK,"+nt");
    in.Read(ts_ChanServ+"LCK_MLOCK",&ChanServ_LCK_MLOCK,"+");
    in.Read(ts_ChanServ+"DEF_KEEPTOPIC",&ChanServ_DEF_KEEPTOPIC,true);
    in.Read(ts_ChanServ+"LCK_KEEPTOPIC",&ChanServ_LCK_KEEPTOPIC,false);
    in.Read(ts_ChanServ+"DEF_TOPICLOCK",&ChanServ_DEF_TOPICLOCK,false);
    in.Read(ts_ChanServ+"LCK_TOPICLOCK",&ChanServ_LCK_TOPICLOCK,false);
    in.Read(ts_ChanServ+"DEF_PRIVATE",&ChanServ_DEF_PRIVATE,false);
    in.Read(ts_ChanServ+"LCK_PRIVATE",&ChanServ_LCK_PRIVATE,false);
    in.Read(ts_ChanServ+"DEF_SECUREOPS",&ChanServ_DEF_SECUREOPS,false);
    in.Read(ts_ChanServ+"LCK_SECUREOPS",&ChanServ_LCK_SECUREOPS,false);
    in.Read(ts_ChanServ+"DEF_SECURE",&ChanServ_DEF_SECURE,false);
    in.Read(ts_ChanServ+"LCK_SECURE",&ChanServ_LCK_SECURE,false);
    in.Read(ts_ChanServ+"DEF_RESTRICTED",&ChanServ_DEF_RESTRICTED,false);
    in.Read(ts_ChanServ+"LCK_RESTRICTED",&ChanServ_LCK_RESTRICTED,false);
    in.Read(ts_ChanServ+"DEF_JOIN",&ChanServ_DEF_JOIN,false);
    in.Read(ts_ChanServ+"LCK_JOIN",&ChanServ_LCK_JOIN,false);
    in.Read(ts_ChanServ+"DEF_REVENGE",&ChanServ_DEF_REVENGE,"NONE");
    in.Read(ts_ChanServ+"LCK_REVENGE",&ChanServ_LCK_REVENGE,false);
    in.Read(ts_ChanServ+"LEVEL_MIN",&ChanServ_LEVEL_MIN,-1);
    in.Read(ts_ChanServ+"LEVEL_MAX",&ChanServ_LEVEL_MAX,30);
    in.Read(ts_ChanServ+"LVL_AUTODEOP",&ChanServ_LVL_AUTODEOP,-1);
    in.Read(ts_ChanServ+"LVL_AUTOVOICE",&ChanServ_LVL_AUTOVOICE,5);
    in.Read(ts_ChanServ+"LVL_AUTOOP",&ChanServ_LVL_AUTOOP,10);
    in.Read(ts_ChanServ+"LVL_READMEMO",&ChanServ_LVL_READMEMO,0);
    in.Read(ts_ChanServ+"LVL_WRITEMEMO",&ChanServ_LVL_WRITEMEMO,15);
    in.Read(ts_ChanServ+"LVL_DELMEMO",&ChanServ_LVL_DELMEMO,25);
    in.Read(ts_ChanServ+"LVL_AKICK",&ChanServ_LVL_AKICK,20);
    in.Read(ts_ChanServ+"LVL_STARAKICK",&ChanServ_LVL_STARAKICK,25);
    in.Read(ts_ChanServ+"LVL_UNBAN",&ChanServ_LVL_UNBAN,10);
    in.Read(ts_ChanServ+"LVL_ACCESS",&ChanServ_LVL_ACCESS,5);
    in.Read(ts_ChanServ+"LVL_SET",&ChanServ_LVL_SET,25);
    in.Read(ts_ChanServ+"LVL_CMDINVITE",&ChanServ_LVL_CMDINVITE,5);
    in.Read(ts_ChanServ+"LVL_CMDUNBAN",&ChanServ_LVL_CMDUNBAN,10);
    in.Read(ts_ChanServ+"LVL_CMDVOICE",&ChanServ_LVL_CMDVOICE,5);
    in.Read(ts_ChanServ+"LVL_CMDOP",&ChanServ_LVL_CMDOP,10);
    in.Read(ts_ChanServ+"LVL_CMDCLEAR",&ChanServ_LVL_CMDCLEAR,20);

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
    case 0:
	break;  // this is here to show up clashes for badly defined signal constants
    case SIGINT:
	break;
#if defined(SIGTERM) && (SIGTERM != 0)
    case SIGTERM:
	break;
#endif
#if defined(SIGQUIT) && (SIGQUIT != 0)
    case SIGQUIT:
	break;
#endif
    case SIGSEGV:
	break;
#ifdef SIGBUS
    case SIGBUS:
	break;
#endif
#if defined(SIGHUP) && (SIGHUP != 0)
    case SIGHUP:
	break;
#endif
    case SIGILL:
	break;
#ifdef SIGTRAP
    case SIGTRAP:
	break;
#endif
#ifdef SIGIOT
    case SIGIOT:
	break;
#endif
    case SIGFPE:
	break;
#if defined(SIGUSR1) && (SIGUSR1 != 0)
    case SIGUSR1:
	break;
#endif
    default:
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

void Magick::dobobhandle(const mstring& server, const mstring& command, const mstring& data)
{
    if(checkifhandled(server,command)==true)
    {
	bob.Call((handlermap[pair<mstring,mstring>(server,command)])[0],1,data.c_str());
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
