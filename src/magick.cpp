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
	{
		argv.push_back(inargv[i]);
	}
}

int Magick::Start()
{
	int i;
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
	for(i=0;i<argc;i++)
	{
		if(argv[i][0]=='/')
			argv[i][0]='-';
		if(argv[i][0]=='-')
		{
			argv[i].LowerCase();
			if(argv[i]=="-dir")
			{
				i++;
				if(i==argc||argv[i][0]=='-')
				{
					// use static errors here because conf directory is not known yet
					cerr<<"-dir "<<"requires a paramter."<<endl;
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
					cerr<<"-config "<<"requires a paramter."<<endl;
					return MAGICK_RET_ERROR;
				}
				config_file=argv[i];
			}
			else if(argv[i]=="-help"||argv[i]=="--help"||

					argv[i]=="-?"||argv[i]=="-h")
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
	// load the local messages database and internal "default messages"
	// the external messages are part of a separate ini called language.ini (both local and global can be done here too)
	LoadInternalMessages();
	LoadExternalMessages();

	StartTime=Now();
	ThreadtoTypeMap[ACE_Thread::self()]=tt_MAIN;

	if(logfile!=NULL)
		fclose(logfile);
	return MAGICK_RET_TERMINATE;
}

mstring Magick::getMessage(const mstring & name)
{
	return "";
}

void Magick::dump_help(mstring & progname)
{
	cout<<"Magick IRC Services are copyright (c) 1996-1998 Preston A. Elder, W. King.\n"
		<<"    E-mail: <prez@magick.tm>   IRC: PreZ@RelicNet\n"
		<<"    E-mail: <ungod@magick.tm>   IRC: Notagod@Effnet\n"
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
	ACE_Local_Mutex lock("Magick::LoadMessages");
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
	{
		*fostream<<def_lang[i]<<"\n";
	}
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
	ACE_Local_Mutex guard("Magick::parseEscapes");
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
	ACE_Local_Mutex lock("Magick::LoadMessages");
	wxFileConfig fconf("magick","","tmplang.lng");
	int i;
	for(i=0;i<MessageNamesLong.size();i++)
		Messages[MessageNamesShort[i]]=fconf.Read(MessageNamesLong[i],Messages[MessageNamesShort[i]]);
}
