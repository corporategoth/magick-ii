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
#include <iostream>
#include <strstream>
using namespace std;
#include "EscLexer.hpp"
#include "EscParser.hpp"

Magick::Magick(int inargc, char **inargv)
{
    services_dir=".";
    config_file="magick.ini";
    for(int i=0;i<inargc;i++)
	argv.push_back(inargv[i]);
}

int Magick::Start()
{
    int i;
    int Result;
    // this is our main routine, when it leaves here, this sucker's done.
    if(bob.StartBob("")==false)
    {
    	/*log that bob couldn't start*/
	bob.bobavail=false;
    }
    FILE *logfile=fopen("magick.log","w+");
    // the below defaults to stderr if logfile cannot be opened
    wxLogStderr logger(logfile);
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
	if(argv[i][0]=='-')
	{
	    argv[i].LowerCase();
	    if(argv[i]=="-dir")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    // use static errors here because conf directory is not known yet
		    cerr<<"-dir"<<" requires a paramter."<<endl;
		    return MAGICK_RET_ERROR;
		}
		services_dir=argv[i];
	    }
	    else if(argv[i]=="-config")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    // use static errors here because conf directory is not known yet
		    cerr<<"-config"<<" requires a paramter."<<endl;
		    return MAGICK_RET_ERROR;
		}
		config_file=argv[i];
	    }
	    else if(argv[i]=="-help"||argv[i]=="--help"||argv[i]=="-?"||argv[i]=="-h")
    	    {
		dump_help(argv[0]);
		return MAGICK_RET_NORMAL;
	    }
	}
    }
    if (chdir (services_dir) < 0)
    {
        perror (services_dir);
#ifdef WIN32
        WSACleanup ();
#endif
        return MAGICK_RET_INVALID_SERVICES_DIR;
    }

    MagickIni=new wxFileConfig("magick","",config_file);
    if(MagickIni==NULL)
    {
	cerr << "Major fubar, couldn't allocate memory to read config file\nAborting"<<endl;
	return MAGICK_RET_ERROR;
    }
    //okay, need a function here to load all the ini file defalts
    get_config_values();
    if(shutdown==true)
	return MAGICK_RET_ERROR;

    // load the local messages database and internal "default messages"
    // the external messages are part of a separate ini called language.ini (both local and global can be done here too)
    LoadInternalMessages();
    LoadExternalMessages();
    StartTime=Now();
    ThreadtoTypeMap[ACE_Thread::self()]=tt_MAIN;

    if(ProgramName=="listnicks")
	return MAGICK_RET_TERMINATE/*nickserv.listnicks(argv)*/;
    if(ProgramName=="listchans")
	return chanserv.listchans(argv);
    //todo here if !win32, freopen stdout,stdin, and stderr and spawn off.

    Result=doparamparse();
    if(Result!=MAGICK_RET_NORMAL)
	return Result;

    if(!check_config())
	return MAGICK_RET_TERMINATE;

    if(logfile!=NULL)
	fclose(logfile);

    if(outlet_on==true)
	s_Outlet.Format("%s%d",services_prefix.c_str(),services_level);
    //open_log();

#ifndef WIN32
    if(live==false)
    {
	if ((i = fork ()) < 0)
	{
	    //log_perror ("fork()");
            return 1;
        }
        else if (i != 0)
            return 0;
        if (setpgid (0, 0) < 0)
        {
            //log_perror ("setpgid()");
            return 1;
        }
    }
#endif
    wxFile pidfile;
    pidfile.Create(pid_filename,true);
    if(pidfile.IsOpened())
    {
	mstring dummystring;
	dummystring<<getpid();
	pidfile.Write(dummystring);
	pidfile.Close();
    }
    /*else
	log_perror ("Warning: cannot write to PID file %s", pid_filename);*/

    //write_log ("All systems nominal");

    // okay here we start setting up the ACE_Reactor and ACE_Event_Handler's
    signalhandler=new SignalHandler;
    ACE_Reactor::instance()->register_handler(SIGINT,signalhandler);
    ACE_Reactor::instance()->register_handler(SIGTERM,signalhandler);

    socket.open(0);
    
    // temporary placeholder
    while(shutdown!=true)
	ACE_Reactor::instance()->handle_events();

    ACE_Reactor::instance()->remove_handler(SIGINT);
    ACE_Reactor::instance()->remove_handler(SIGTERM);
    delete signalhandler;

    return MAGICK_RET_TERMINATE;
}

mstring Magick::getMessage(const mstring & name)
{
    return "";
}

void Magick::dump_help(mstring & progname)
{
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

    //ACE_Thread_Mutex_Guard guard(mutex);
    ACE_Local_WLock lock("Magick::LoadMessages");
    ACE_Guard<ACE_Local_WLock> lockguard(lock);
    // so that the language file strings are only loaded in memory while this function is in effect.
#include "language.h"
    int i;
    remove("tmplang.lng");

    /* Why not just:
    {
    	ofstream out("tmplang.lng");
    	for (i=0;i<def_langent;i++, def_lang++)
	    out << def_lang[i] << endl;
    }
    No memory allocation, and it closes itself :)
    You need a wxFileOutputStream for a strait string -> file write?
    Also -- why cant we go string away -> parser (no tmp file!) */

    /* back to prez: we need a temp file because the parser uses a file
    to read from, if you wanna rewrite it go ahead. wxFileOutputStream 
    and not ofstream because ofstream will mean linking in more 
    unnecessary code we have our streams code in the wxStream classes, 
    why use unnecessary	extra overhead?*/

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
    // hmm doesn't *really* need a mutex here.
    ACE_Local_Mutex lock("Magick::parseEscapes");
    ACE_Guard<ACE_Local_Mutex> guard(lock);
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
    return lexer.retstring;
}

void Magick::LoadExternalMessages()
{

    // use the previously created name array to get the names to load
    ACE_Local_WLock lock("Magick::LoadMessages");
    ACE_Guard<ACE_Local_WLock> guard(lock);
    wxFileConfig fconf("magick","","tmplang.lng");
    int i;
    for(i=0;i<MessageNamesLong.size();i++)
    	Messages[MessageNamesShort[i]]=fconf.Read(MessageNamesLong[i],Messages[MessageNamesShort[i]]);
}

int Magick::doparamparse()
{
    mstring temp;
    int argc=argv.size(),i;
    for(i=1;i<argc;i++)
    {
	/* i've taken out / command switches below cause i hate them myself,
	courtesy of gnu we now have a common switch format -- for long, and - for short*/
    	/*if(argv[i][0]=='/')
	    argv[i][0]='-';*/
	if(argv[i][0]=='-')
	{
	    argv[i].LowerCase();
	    if(argv[i]=="-remote")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    cerr<<"-remote"<<" requires hostname[:port]"<<endl;
		    return MAGICK_RET_ERROR;
		}
		if(argv[i].Contains(":"))
		{
		    if(argv[i].After(':').IsNumber())
		    {
			if(atoi(argv[i].After(':').c_str())<0)
			    cerr<<"port must be a positive number"<<endl;
			else
			    remote_port=atoi(argv[i].After(':').c_str());
		    }
		    remote_server=argv[i].Before(':');
		}
	    }
	    else if(argv[i]=="-name")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-name");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		server_name=argv[i];
	    }
	    else if(argv[i]=="-desc")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-desc");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		server_desc=argv[i];
	    }
	    else if(argv[i]=="-user")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-user");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		services_user=argv[i];
	    }
	    else if(argv[i]=="-host")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-host");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		services_host=argv[i];
	    }
	    else if(argv[i]=="-prefix")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-prefix");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		services_prefix=argv[i];
	    }
	    else if(argv[i]=="-dir")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-dir");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		// already handled, but we needed to i++
	    }
	    else if(argv[i]=="-config")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-config");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		// already handled, but we needed to i++
	    }
	    else if(argv[i]=="-log")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-log");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		log_filename=argv[i];
	    }
	    else if(argv[i]=="-debug")
		debug=true;
	    else if(argv[i]=="-live"||argv[i]=="nofork")
		live=true;
	    else if(argv[i]=="-relink")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-relink");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		if(atoi(argv[i].c_str())<0)
		{
		    cerr<<"-relink"<<" parameter must be positive"<<endl;
		    return MAGICK_RET_ERROR;
		}
		server_relink=atoi(argv[i].c_str());
	    }
	    else if(argv[i]=="-level")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-level");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		if(atoi(argv[i].c_str())<0)
		{
		    cerr<<"-level"<<" parameter must be positive"<<endl;
		    return MAGICK_RET_ERROR;
		}
		services_level=atoi(argv[i].c_str());
	    }
	    else if(argv[i]=="-offset")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-offset");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		if(abs(atoi(argv[i].c_str()))>24)
		{
		    cerr<<"-offset"<<" must be between -24 and 24"<<endl;
		    return MAGICK_RET_ERROR;
		}
		tz_offset=atoi(argv[i].c_str());
	    }
	    else if(argv[i]=="-norelink")
		server_relink=-1;
	    else if(argv[i]=="-update")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-update");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		if(atoi(argv[i].c_str())<0)
		{
		    cerr<<"-update"<<": number of seconds must be positive"<<endl;
		    return MAGICK_RET_ERROR;
		}
		update_timeout=atoi(argv[i].c_str());
	    }
	    else if(argv[i]=="-ping")
	    {
		i++;
		if(i==argc||argv[i][0]=='-')
		{
		    temp.Format(getMessage("ERR_REQ_PARAM"),"-ping");
		    cerr<<temp<<endl;
		    return MAGICK_RET_ERROR;
		}
		if(atoi(argv[i].c_str())<0)
		{
		    cerr<<"-ping"<<": number of seconds must be positive"<<endl;
		    return MAGICK_RET_ERROR;
		}
		ping_frequency=atoi(argv[i].c_str());
	    }
	    else
	    {
		temp.Format("Unknown option %s.",argv[i]);
		cerr<<temp<<endl;
		return MAGICK_RET_ERROR;
	    }
	}
	else
	{
	    cerr<<"Non-option arguments not allowed"<<endl;
	    return MAGICK_RET_ERROR;
	}
    }
    return MAGICK_RET_NORMAL;
}

bool Magick::check_config()
{
    // change these later when the appropriate classes are set up
    if(operserv_on==false)
    {
	globalnoticer_on=false;
	outlet_on=false;
	akill_on=false;
	clones_on=false;
    }
    if(clones_allowed<1)
	clones_on=false;
    if (services_level < 1)
    {
	// change this to the logging mechanism
        cerr<<"CONFIG: Cannot set SERVICES_LEVEL < 1"<<endl;
        return false;
    }
    if (tz_offset >= 24 || tz_offset <= -24)
    {
	// change this to the logging mechanism
        cerr<<"CONFIG: TZ_OFFSET must fall between -24 and 24."<<endl;
        return false;
    }
    if (update_timeout < 30)
    {
	// change this to the logging mechanism
        cerr<<"CONFIG: Cannot set UPDATE_TIMEOUT < 30."<<endl;
        return false;
    }
    if (read_timeout < 1)
    {
	// change this to the logging mechanism
        cerr<<"CONFIG: Cannot set READ_TIMEOUT < 1."<<endl;
        return false;
    }
    if (passfail_max < 1)
    {
	// change this to the logging mechanism
        cerr<<"CONFIG: Cannot set PASSFAIL_MAX < 1."<<endl;
        return false;
    }
    if (flood_messages > lastmsgmax)
    {
	// change this to the logging mechanism
	mstring temp;
	temp.Format("CONFIG: Cannot set FLOOD_MESSAGES > %d.",lastmsgmax);
        cerr<<temp<<endl;
        return false;
    }
    return true;

}

void Magick::get_config_values()
{
    if(MagickIni==NULL)
    {
	shutdown==true;
	return;
    }
    wxFileConfig& in=*MagickIni;
    mstring ts_Startup=mstring("Startup\\");
    mstring ts_Services=mstring("Services\\");
    mstring ts_Files=mstring("Files\\");
    mstring ts_Config=mstring("Config\\");
    mstring ts_Chanserv=mstring("Chanserv\\");
    mstring ts_Nickserv=mstring("Nickserv\\");
    mstring ts_Memoserv=mstring("Memoserv\\");
    mstring ts_Operserv=mstring("Operserv\\");
    mstring ts_DevNull=mstring("DevNull\\");

    in.Read(ts_Startup+"Remote_Server",&remote_server,"127.0.0.1");
    in.Read(ts_Startup+"Remote_Port",&remote_port,9666);
    in.Read(ts_Startup+"Password",&password,"");
    in.Read(ts_Startup+"Server_Name",&server_name,"hell.darker.net");
    in.Read(ts_Startup+"Server_Desc",&server_desc,"DarkerNet's IRC Services");
    in.Read(ts_Startup+"Services_User",&services_user,"reaper");
    in.Read(ts_Startup+"Services_Host",&services_host,"darker.net");
    in.Read(ts_Startup+"Services_Level",&services_level,1);
    in.Read(ts_Startup+"TZ_Offset",&tz_offset,0);
    in.Read(ts_Startup+"Stop",&shutdown);

    //in.Read(ts_Services+"Nickserv",&nickserv.on,true);
    in.Read(ts_Services+"Chanserv",&chanserv.on,true);
    //in.Read(ts_Services+"Helpserv",&helpserv.on,true);
    //in.Read(ts_Services+"IrcIIHelp",&helpserv.irciihelp_on,true);
    //in.Read(ts_Services+"Memoserv",&memoserv.on,true);
    //in.Read(ts_Services+"Memos",&memoserv.memos_on,true);
    //in.Read(ts_Services+"News",&newsserv.news_on,true);
    //in.Read(ts_Services+"DevNull",&devnull.on,true);
    //in.Read(ts_Services+"Operserv",&operserv.on,true);
    //in.Read(ts_Services+"Outlet",&devnull.outlet_on,true);
    //in.Read(ts_Services+"AKill",&nickserv.akill_on,true);
    //in.Read(ts_Services+"Clones",&nickserv.clones_on,true);
    //in.Read(ts_Services+"GlobalNoticer",&operserv.global_noticer_on,true);
    in.Read(ts_Services+"Show_Sync",&show_sync,false);
    //in.Read(ts_Services+"Nickserv_Name",&nickserv.name,"NickServ");
    in.Read(ts_Services+"Chanserv_Name",&chanserv.name,"ChanServ");
    //in.Read(ts_Services+"Operserv_Name",&operserv.name,"OperServ");
    //in.Read(ts_Services+"Memoserv_Name",&memoserv.name,"MemoServ");
    //in.Read(ts_Services+"Helpserv_Name",&helpserv.name,"HelpServ");
    //in.Read(ts_Services+"GlobalNoticer_Name",&operserv.global_noticer_name,"Death");
    //in.Read(ts_Services+"DevNull_Name",&devnull.name,"DevNull");
    //in.Read(ts_Services+"IrcIIHelp_Name",&helpserv.irciihelp_name,"IrcIIHelp");
    in.Read(ts_Services+"Services_Prefix",&services_prefix,"Magick-"); 

    in.Read(ts_Files+"Log_Filename",&log_filename,"magick.log");
    in.Read(ts_Files+"MOTD_Filename",&motd_filename,"magick.motd");
    //in.Read(ts_Files+"Nickserv_DB",&nickserv.db_filename,"nick.db");
    in.Read(ts_Files+"Chanserv_DB",&chanserv.db_filename,"chan.db");
    //in.Read(ts_Files+"Memoserv_DB",&memoserv.memodb_filename,"memo.db");
    //in.Read(ts_Files+"Newsserv_DB",&memoserv.newsdb_filename,"news.db");
    //in.Read(ts_Files+"AKill_DB",&nickserv.akilldb_filename,"akill.db");
    //in.Read(ts_Files+"Clone_DB",&nickserv.clonedb_filename,"clone.db");
    //in.Read(ts_Files+"SOP_DB",&operserv.db_filename,"sop.db");
    //in.Read(ts_Files+"Message_DB",&memoserv.mesgdb_filename,"message.db");
    in.Read(ts_Files+"Pid_File",&pid_filename,"magick.pid");
    //in.Read(ts_Files+"Helpserv_File"+&helpserv.filename,"magick.help");.

    in.Read(ts_Config+"Override_Level",&override_level,2);
    in.Read(ts_Config+"Server_Relink",&server_relink,5);
    in.Read(ts_Config+"Update_Timeout",&update_timeout,300);
    in.Read(ts_Config+"Ping_Frequency",&ping_frequency,30);
    in.Read(ts_Config+"Read_Timeout",&read_timeout,10);
    in.Read(ts_Config+"StarThresh",&StarThresh,4);

    in.Read(ts_Chanserv+"Channel_Expire",&chanserv.expire,4);
    // do we need the below anymore?
    in.Read(ts_Chanserv+"AKick_Max",&chanserv.akick_max,32);
    in.Read(ts_Chanserv+"Def_AKick_Reason",&chanserv.def_akick_reason,"You have been banned from the channel");
    in.Read(ts_Chanserv+"Msg_Threshhold",&chanserv.msg_thresh,150);

    //in.Read(ts_Nickserv+"Nick_Expire",&nickserv.nick_expire,28);
    //in.Read(ts_Nickserv+"Release_Timeout",&nickserv.release_timeout,60);
    //in.Read(ts_Nickserv+"Wait_Collide",&nickserv.wait_collide,0);
    //in.Read(ts_Nickserv+"Passfail_Max",&nickserv.passfail_max,5);
    //in.Read(ts_Nickserv+"Msg_Threshhold",&nickserv.msg_thresh,150);

    //in.Read(ts_Memoserv+"News_Expire",&memoserv.news_expire,21);
    //in.Read(ts_Memoserv+"Msg_Threshhold",&memoserv.msg_thresh,150);

    //in.Read(ts_Operserv+"Services_Admin",&operserv.services_admin);
    //in.Read(ts_Operserv+"AKill_Expire",&operserv.akill_expire,7);
    //in.Read(ts_Operserv+"Clones_Allowed",&operserv,clones_allowed,2);
    //in.Read(ts_Operserv+"Def_Clone_Reason",&operserv.def_clone_reason,"Exceeded maximum amount of connections from one host.");
    //in.Read(ts_Operserv+"Msg_Threshhold",&operserv.msg_thresh,150);

    //in.Read(ts_DevNull+"Flood_Messages",&devnull.flood_messages,5);
    //in.Read(ts_DevNull+"Flood_Time",&devnull.flood_time,10);
    //in.Read(ts_DevNull+"Ignore_Time",&devnull.ignore_time,20);
    //in.Read(ts_DevNull+"Ignore_Offences",&devnull.ignore_offences,5);
    //in.Read(ts_DevNull+"Msg_Threshhold",&devnull.msg_thresh,250);
}

int SignalHandler::handle_signal(int signum, siginfo_t *siginfo, ucontext_t *ucontext)
{
    // todo: fill this sucker in
    // switch(signum)
    return 0;
}
