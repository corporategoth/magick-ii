// RCS-ID:      $Id$
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
	MagickIni=new wxFileConfig("magick","",config_file,"");
	if(MagickIni==NULL)
	{
		cerr << "Major fubar, couldn't allocate memory to read config file\nAborting"<<endl;
		return MAGICK_RET_ERROR;
	}
	// load the local messages database and internal "default messages"
	// the external messages are part of a separate ini called language.ini (both local and global can be done here too)
	LoadInternalMessages();
	LoadExternalMessages();

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



	/* note left side of message can have spaces before '=' that will be trimmed
		right side will *not* be trimmed*/
	mstring tempstor[]=
	{
	/* Log stuff (NO FORMATTING HERE!) */
	"ERR_READ_DB=Error reading %s database.",
	"ERR_WRITE_DB=Error writing %s database.",
	"ERR_UNKNOWN_SERVMSG=Unknown message from server (%s).",

	/* Incorrect Syntax */
	"ERR_UNKNOWN_COMMAND=Unrecognized command \2%s\2.  Type \2/msg %s HELP\2 for help.",
	"ERR_UNKNOWN_OPTION=Unrecognized option \2%s\2.  Type \2/msg %s HELP %s\2 for help.",
	"ERR_MORE_INFO=Type \2/msg %s HELP %s\2 for more information.",
	"ERR_NOHELP=No help available for command \2%s\2.",
	"ERR_STARTHRESH=Entry must have at least %d non-*, ? or . characters.",
	"ERR_REQ_PARAM=%s requires a parameter.\n",
	"ERR_NEED_MLOCK_PARAM=Paramater required for MLOCK +%c.",
	"ERR_MLOCK_POSITIVE=Paramater for MLOCK +%c must be positive.",
	"ERR_MLOCK_UNKNOWN=Unknown MLOCK character \2%c\2 ignored.",
	"ERR_SECURE_NICKS=SECURE access lists may only contain registered nicknames.",
	"ERR_FORMATTING=You must specify \2%s\2 only.",
	"ERR_MAY_NOT_HAVE=You may not use a %c character.",
	"ERR_MUST_HAVE=You must include a %c character.",

	/* Access Denied */
	"ERR_WRONG_PASSWORD=Password incorrect.",
	"ERR_ACCESS_DENIED=Access denied.",
	"ERR_SUSPENDED_NICK=Access denied for SUSPENDED nicknames.",
	"ERR_SUSPENDED_CHAN=Access denied for SUSPENDED channels.",
	"ERR_TEMP_DISABLED=Sorry, \2%s\2 is temporarily disabled.",
	"ERR_FAILED=Sorry, \2%s\2 failed.",
	"ERR_YOU_DONT_EXIST=Sorry, could not obtain your user information.",
	"ERR_CHAN_NOTVALID=\2%s\2 is not a valid channel name.",
	"ERR_READ_ONLY=\2WARNING\2: Services are in read-only mode, changes will not be saved.",

	/* Need verification */
	"ERR_NEED_PASSWORD=Password identification required for \2%s\2.",
	"ERR_NEED_OPS=You must be a channel operator for \2%s\2.",
	"ERR_IDENT_NICK_FIRST=Please retry after typing \2/MSG %s IDENTIFY \37password\37.",
	"ERR_IDENT_CHAN_FIRST=Please retry after typing \2/MSG %s IDENTIFY \37%s password\37.",
	"ERR_NICK_FORBIDDEN=This nickname may not be used.  Please choose another.",
	"ERR_NICK_OWNED=This nickname is owned by someone else.  Please choose another.",
	"ERR_NICK_SECURE=This nickname is registered and protected.",
	"ERR_NICK_IDENTIFY=If this is your nick, type \2/MSG %s IDENTIFY \37password\37",
	"ERR_WILL_KILL_YOU=If you do not change within one minute, you will be disconnected.",

	/* Done as or to wrong person */
	"ERR_MUST_BE_HOST=\2%s\2 must be done as the HOST nickname.",
	"ERR_CANT_BE_HOST=\2%s\2 may not be done as the HOST nickname.",
	"ERR_MUST_BE_LINK=\2%s\2 must be done as a LINKED nickname.",
	"ERR_CANT_BE_LINK=\2%s\2 may not be done on a LINKED nickname.",
	"ERR_MUST_BETO_HOST=\2%s\2 must be performed on a HOST nickname.",
	"ERR_CANT_BETO_HOST=\2%s\2 may not be performed on a HOST nickname.",
	"ERR_MUST_BETO_LINK=\2%s\2 must be performed on a LINKED nickname.",
	"ERR_CANT_BETO_LINK=\2%s\2 may not be performed on a LINKED nickname.",
	"ERR_NOT_ON_YOURSELF=\2%s\2 may not be performed on yourself.",
	"ERR_NOT_ON_IRCOP=\2%s\2 may not be performed on an IRC Operator.",
	"ERR_ONLY_ON_IRCOP=\2%s\2 may only be performed on an IRC Operator.",
	"ERR_MUST_BE_IRCOP=You must have the IRC Operator flag set to \2%s\2.",

	/****************************************************************************
	 *********************************** MISC ***********************************
	 ****************************************************************************/

	"INFO_SYNC_TIME=Databases will sync in %s.",
	"INFO_LIST_MATCH=List of entries matching \2%s\2:",
	"INFO_END_OF_LIST=End of list - %d/%d matches shown.",
	"INFO_EMAIL=E-Mail",
	"INFO_URL=WWW Page (URL)",
	"INFO_FOUNDER=Founder",
	"INFO_DESC=Description",
	"INFO_MLOCK=Mode Lock",
	"INFO_JOIN=ChanServ Join",

	/* Different lists maintinance */
	"LIST_THERE=\2%s\2 is already present (or inclusive) on %s %s list.",
	"LIST_NOT_THERE=\2%s\2 not found on %s %s list.",
	"LIST_NOT_FOUND=No such entry \2#%d\2 on %s %s list.",
	"LIST_ADDED=\2%s\2 has been added to %s %s list.",
	"LIST_ADDED_AT=\2%s\2 has been added to %s %s at \2%s\2.",
	"LIST_REMOVED=\2%s\2 has been removed from %s %s list.",
	"LIST_REMOVED_NUM=Entry \2#%d\2 has been removed from %s %s list.",
	"LIST_REMOVED_MASK=%d entr%s matching \2%s\2 removed from %s %s list.",
	"LIST_REMOVED_ALL=All entries on %s %s list removed.",
	"LIST_UNCHANGED=\2%s\2 unchanged on %s %s list at \2%s\2.",
	"LIST_CHANGED=\2%s\2 changed on %s %s list to \2%s\2.",
	"LIST_LIMIT=Sorry, you may only have %d entries on %s %s list.",

	/* Output in multi files */
	"MULTI_GETPASS=Password for %s (%s) is \2%s\2.",
	"MULTI_GETPASS_WALLOP=\2%s\2 used GETPASS on \2%s\2 (%s).",
	"MULTI_FORBID=\2%s\2 has been FORBIDDEN.",
	"MULTI_SUSPEND=\2%s\2 (%s) has been SUSPENDED.",
	"MULTI_UNSUSPEND=\2%s\2 (%s) has been UNSUSPENDED.",

	/* process stuff */
	"FLOODING=%s is FLOODING services (placed on %s ignore).",
	"TEMP_FLOOD=Services FLOOD triggered (>%d messages in %d seconds).  You are being ignored for %d seconds.",
	"PERM_FLOOD=Services FLOOD triggered >%d times in one connection.  You are now on perminant ignore.",
	"IS_IGNORED=You have triggered PERM ignore on a previous connection.  You will not be answered by services.",
	"SERVICES_OFF_REASON=Services are currently \2OFF\2 (%s).",
	"SERVICES_OFF=Services are currently \2OFF\2.",
	"SERVICES_ON=Services have now been switched back \2ON\2 - Please use them at will.",
	"ONOFF_NOTIFY=Services switched \2%s\2 by request of \2%s\2.",

	/****************************************************************************
	 ********************************* NickServ *********************************
	 ****************************************************************************/

	/* INFO displays */
	"NS_INFO_INTRO		=%s is %s",
	"NS_INFO_HOST		=        Host Nick: \2%s\2",
	"NS_INFO_EMAIL		=   E-Mail Address: %s",
	"NS_INFO_URL		=   WWW Page (URL): %s",
	"NS_INFO_SUSPENDED	=    Suspended For: %s",
	"NS_INFO_USERMASK	=Last Seen Address: %s",
	"NS_INFO_REGISTERED	=       Registered: %s ago",
	"NS_INFO_ONLINE_AS	=        Online As: %s",
	"NS_INFO_AONLINE_AS	=   Also Online As: %s",
	"NS_INFO_LAST_SEEN	=        Last Seen: %s ago",
	"NS_INFO_LAST_ONLINE=      Last Online: %s ago",
	"NS_INFO_OPTIONS	=          Options: %s",
	"NS_INFO_COUNT=%d nicknames currently registered.",
	"NS_INFO_ONLINE=This user is currently online, type \2/whois %s\2 for more information."

	/* FLAG names */
	"NS_FLAG_SUSPENDED=\2SUSPENDED USER\2",
	"NS_FLAG_KILLPROTECT=Kill Protection",
	"NS_FLAG_SECURE=Security",
	"NS_FLAG_PRIVATE=Private",
	"NS_FLAG_IRCOP=IRC Operator",
	"NS_FLAG_SOP=Services Operator",
	"NS_FLAG_NONE=None",

	/* General returns */
	"NS_REGISTERED=Your nickname is now registered under host \2%s\2.",
	"NS_LINKED=Your nickname has now been linked to \2%s\2.",
	"NS_DROPPED=Nickname %s%s%s%s has been dropped.",
	"NS_CHANGE_PASSWORD=Your password is now \2%s\2 - Please remember this for later use.",
	"NS_KILLED_IMPOSTER=User claiming your nickname has been killed.",
	"NS_RELEASE=Type \2/MSG %s RELEASE %s \37password\37 to use it before the one-minute timeout.",
	"NS_FORCED_CHANGE=Your nickname has been forcibly changed.",
	"NS_FORCED_KILL=Nickname protection enforced",
	"NS_FAILMAX_KILL=Repeated password failure",
	"NS_GHOST_KILL=Removing GHOST user",
	"NS_IDENTIFIED=Password accepted - you are now recognized.",
	"NS_FORBID_WALLOP=\2%s\2 used FORBID on nickname \2%s\2 (%d slave nicks dropped).",

	/* Ownership and status */
	"NS_NOT_YOURS=Nickname %s does not belong to you.",
	"NS_IN_USE=Nickname %s is currently in use.",
	"NS_NOT_IN_USE=Nickname %s is not currently in use.",
	"NS_YOU_NO_SLAVES=You have no slave nicks.",
	"NS_NO_SLAVES=Nickname %s has no slave nicks.",
	"NS_YOU_NOT_REGISTERED=Your nickname is not registered.",
	"NS_NOT_REGISTERED=Nickname %s is not registered.",
	"NS_TAKEN=Nickname %s is already registered.",
	"NS_CANNOT_REGISTER=Nickname %s may not be registered.",
	"NS_CANNOT_LINK=Nickname %s may not be linked.",
	"NS_AM_IGNORED=%s is ignoring your memos.",
	"NS_IS_SUSPENDED_MEMO=%s is SUSPENDED, therefore may not retrieve memos.",
	"NS_IS_SUSPENDED=Nickname %s is SUSPENDED.",
	"NS_IS_NOT_SUSPENDED=Nickname %s is not SUSPENDED.",


	/****************************************************************************
	 ********************************* ChanServ *********************************
	 ****************************************************************************/

	/* INFO Displays */
	"CS_INFO_INTRO		=Information on channel %s",
	"CS_INFO_FOUNDER	=       Founder: %s%s%s%s",
	"CS_INFO_DESC		=   Description: %s",
	"CS_INFO_URL		=WWW Page (URL): %s",
	"CS_INFO_REG_TIME	=    Registered: %s ago",
	"CS_INFO_LAST_USED	=     Last Used: %s ago",
	"CS_INFO_SUSPENDED	= Suspended For: %s",
	"CS_INFO_SUSPENDER	=  Suspended By: %s",
	"CS_INFO_TOPIC		=    Last Topic: %s",
	"CS_INFO_TOPIC_SET	=  Topic Set By: %s",
	"CS_INFO_REVENGE	= Revenge Level: %s",
	"CS_INFO_CHAN_STAT	= Channel Stats: %d user%s, %d voice%s, %d op%s.",
	"CS_INFO_OPTIONS	=       Options: %s",
	"CS_INFO_MLOCK		=     Mode Lock: %s",
	"CS_INFO_COUNT		=%d channels currently registered.",

	/* FLAG Names */
	"CS_FLAG_SUSPENDED=\2SUSPENDED CHANNEL\2",
	"CS_FLAG_PRIVATE=Private",
	"CS_FLAG_KEEPTOPIC=Topic Retention",
	"CS_FLAG_TOPICLOCK=Topic Lock",
	"CS_FLAG_SECUREOPS=Secure Ops",
	"CS_FLAG_SECURE=Secure",
	"CS_FLAG_RESTRICTED=Restricted",
	"CS_FLAG_NONE=None",

	/* Bitchy Stuff */
	"CS_REV_LEVEL=Revenge level for %s currently at \2%s\2 (%s).",
	"CS_REV_SET=Revenge level for %s set to \2%s\2 (%s).",
	"CS_REV_DEOP=\2REVENGE\2 - Do not deop %s.",
	"CS_REV_KICK=\2REVENGE\2 - Do not kick %s.",
	"CS_REV_BAN=\2REVENGE\2 - Do not ban %s.",
	"CS_SUSPENDED_TOPIC=[\2SUSPENDED\2] %s [\2SUSPENDED\2]",
	"CS_FORBID_WALLOP=\2%s\2 used FORBID on channel \2%s\2.",

	/* Levels and Access */
	"CS_LEVEL_YOU=You can %s.",
	"CS_LEVEL_LIST=Level required for commands on %s:",
	"CS_LEVEL_LOW=Cannot change levels below %d.",
	"CS_LEVEL_HIGH=Cannot change levels above %d.",
	"CS_LEVEL_CHANGE=Level for %s on %s changed to \2%d\2.",
	"CS_LEVEL_NO_CHANGE=Level for %s on %s unchanged at \2%d\2.",
	"CS_LEVEL_RESET=All levels for %s reset.",
	"CS_LEVEL_NONE=Unknown level type \2%s\2.",
	"CS_ACCESS_ZERO=Access levels must be non-zero.",
	"CS_ACCESS_LOW=Cannot add or change to access levels below %d.",
	"CS_ACCESS_HIGH=Cannot add or change to access levels above %d.",
	"CS_ACCESS_HIGHER=Sorry, %s is higher or equal on access list.",
	"CS_ACCESS_HIGHER_MATCH=Sorry, %s matches %s (higher or equal on access list).",

	/* General Outputs */
	"CS_REGISTERED=Channel %s has been registered under your nickname \2%s\2.",
	"CS_DROPPED=Channel %s has been dropped.",
	"CS_CHANGE_PASSWORD=Channel %s password is now \2%s\2 - Please remember this for later use.",
	"CS_IDENTIFIED=Password accepted - you now have founder access to %s.",
	"CS_YOU_UNBANNED=You have been unbanned from %s.",
	"CS_UNBANNED=%s has been unbanned from %s.",
	"CS_CLEARED=\2%s\2 have been cleared from channel %s.",
	"CS_CLEAR_KICK=CLEAR USERS command from %s",

	/* Ownerships */
	"CS_ERR_SUSPENDED=Channel %s is currently SUSPENDED, no ops/voices will be allowed.",
	"CS_ERR_REGISTERED=Channel %s is a registered channel, access privilages apply.",
	"CS_IN_USE=Channel %s is currently in use.",
	"CS_NOT_IN_USE=Channel %s is not currently in use.",
	"CS_NOT_REGISTERED=Channel %s is not registered.",
	"CS_TAKEN=Channel %s is already registered.",
	"CS_CANNOT_REGISTER=Channel %s may not be registered.",
	"CS_FORBIDDEN=Channel %s may not be used.",
	"CS_GET_OUT=You are not permitted in this channel",
	"CS_IS_SUSPENDED=Channel %s is SUSPENDED.",
	"CS_IS_NOT_SUSPENDED=Channel %s is not SUSPENDED.",
	"CS_YOU_NOT_IN_CHAN=You are not in channel %s.",
	"CS_YOU_IN_CHAN=You are already in channel %s.",
	"CS_NOT_IN_CHAN=%s is not in channel %s.",
	"CS_IN_CHAN=%s is already in channel %s.",
	"CS_YOU_NOT_GOT=You are not %s in channel %s.",
	"CS_NOT_GOT=%s is not %s in channel %s.",
	"CS_YOU_ALREADY_GOT=You are already %s in channel %s.",
	"CS_ALREADY_GOT=%s is already %s in channel %s.",

	/****************************************************************************
	 ********************************* MemoServ *********************************
	 ****************************************************************************/

	"MS_IS_BACKUP=Sorry, backup services currently in use, MEMOS and NEWS are disabled.",

	"MS_YOU_DONT_HAVE=You have no memos.",
	"NS_YOU_DONT_HAVE=There are no news articles for %s.",
	"MS_YOU_HAVE=You have %d memo%s.",
	"NS_YOU_HAVE=There %s %d news article%s for %s.",
	"MS_DOESNT_EXIST=Memo %d does not exist!",
	"NS_DOESNT_EXIST=News article %d does not exist for %s!",
	"NS_MAY_NOT=You may not %s a news article.",

	"MS_LIST=To read, type \2/msg %s READ \37num\37.",
	"NS_LIST=To read, type \2/msg %s READ %s \37num\37.",
	"MS_NEW=You have a new %smemo (#%d) from %s.",
	"MS_READ_NEW=Type \2/msg %s READ %d\2 to read it.",
	"NS_NEW=There is a new news article (#%d) for %s from %s.",
	"NS_READ_NEW=Type \2/msg %s READ %s %d\2 to read it.",
	"MS_MEMO=Memo %d from %s (sent %s ago).",
	"NS_MEMO=News article %d for %s from %s (sent %s ago)",
	"MS_TODEL=To delete, type \2/msg %s DEL %d\2.",
	"MS_TODEL_ALL=To delete, type \2/msg %s DEL ALL\2.",

	"MS_SEND=Memo sent to %s (%s).",
	"NS_SEND=News article posted for %s.",
	"MS_MASS_SEND=Memo sent to all %s.",
	"MS_DELETE=Memo %d has been deleted.",
	"NS_DELETE=News article %d for %s has been deleted.",
	"MS_DELETE_ALL=All of your memos have been deleted.",
	"NS_DELETE_ALL=All news articles for %s have been deleted.",

	/****************************************************************************
 	 ********************************* OperServ *********************************
	 ****************************************************************************/


	"OS_GLOBAL_WALLOP=\2%s\2 just sent a message to ALL users.",

	"OS_SET_EXP=Expiries (days):",
	"OS_SET_EXP_CHAN =    Channels : \2%d\2",
	"OS_SET_EXP_NICK =    Nicknames: \2%d\2",
	"OS_SET_EXP_NEWS =    News     : \2%d\2",
	"OS_SET_EXP_AKILL=    AutoKills: \2%d\2",
	"OS_SET_SOPS=Maximum of \2%d\2 Services Operators (%d currently).",
	"OS_SET_CLONES=Each user may have \2%d\2 connections.",
	"OS_SET_RELEASE=Nicknames held for \2%d\2 seconds.",
	"OS_SET_AKICKS=Maximum of \2%d\2 AKICK's per channel.",
	"OS_SET_FLOOD=Flood triggered on \2%d\2 messages in \2%d\2 seconds.",
	"OS_SET_IGNORE=Ignore lasts for \2%d\2 seconds, \2%d\2 ignores makes it perminant.",
	"OS_SET_RELINK=Services relink in \2%d\2 seconds upon SQUIT.",
	"OS_SET_OVERRIDE=ChanServ Override is \2%s\2",
	"OS_SET_UPDATE=Databases saved every \2%d\2 seconds.",
	"OS_SET_ADMINS=Services Admins: \2%s\2",

	"OS_QLINE=\2%s\2 activated QUARENTINE on nick \2%s\2.",
	"OS_UNQLINE=\2%s\2 removed QUARENTINE on nick \2%s\2.",
	"OS_SVSNOOP_ON=\2%s\2 activated OPER QUARENTINE on \2%s\2.",
	"OS_SVSNOOP_OFF=\2%s\2 removed OPER QUARENTINE on \2%s\2.",
	"OS_JUPE=JUPING \2%s\2 by request of \2%s\2.",
	"OS_UPDATE=Updating databases.",
	"OS_NEW_MESSAGE=\2%s\2 created a new %s message.",
	"OS_AKILL_ADDED=\2%s\2 has been added to services %sAKILL list (%d users killed).",
	"OS_AKILL_NOT_THERE=\2%s\2 not found on services AKILL list or is PERMANENT.",
	"OS_AKILL_EXPIRE=Expring AKILL on %s (%s) by %s.",
	"OS_AKILL_BANNED=You are banned (%s)",
	"OS_CLONE_HIGH=CLONE AMOUNT must be less than %d.",
	"OS_CLONE_LOW=CLONE AMOUNT must be greater than %d.",

	""};

	int i=0;

	while(tempstor[i]!="")
	{
		mstring name,value;
		name=tempstor[i].Before('=').Trim().Trim(false);
		value=tempstor[i].After('=');
		Messages[name]=value;
		i++;
	}
}

mstring Magick::parseEscapes(const mstring & in)
{
	ACE_Thread_Mutex_Guard guard(mutex);
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
	}
	return lexer.retstring;
}

void Magick::LoadExternalMessages()
{
	mstring tempstor[] =
	{
	/* Log stuff (NO FORMATTING HERE!) */
	"ERR_READ_DB",
	"ERR_WRITE_DB",
	"ERR_UNKNOWN_SERVMSG",

	/* Incorrect Syntax */
	"ERR_UNKNOWN_COMMAND",
	"ERR_UNKNOWN_OPTION",
	"ERR_MORE_INFO",
	"ERR_NOHELP",
	"ERR_STARTHRESH",
	"ERR_REQ_PARAM",
	"ERR_NEED_MLOCK_PARAM",
	"ERR_MLOCK_POSITIVE",
	"ERR_MLOCK_UNKNOWN",
	"ERR_SECURE_NICKS",
	"ERR_FORMATTING",
	"ERR_MAY_NOT_HAVE",
	"ERR_MUST_HAVE",

	/* Access Denied */
	"ERR_WRONG_PASSWORD",
	"ERR_ACCESS_DENIED",
	"ERR_SUSPENDED_NICK",
	"ERR_SUSPENDED_CHAN",
	"ERR_TEMP_DISABLED",
	"ERR_FAILED",
	"ERR_YOU_DONT_EXIST",
	"ERR_CHAN_NOTVALID",
	"ERR_READ_ONLY",

	/* Need verification */
	"ERR_NEED_PASSWORD",
	"ERR_NEED_OPS",
	"ERR_IDENT_NICK_FIRST",
	"ERR_IDENT_CHAN_FIRST",
	"ERR_NICK_FORBIDDEN",
	"ERR_NICK_OWNED",
	"ERR_NICK_SECURE",
	"ERR_NICK_IDENTIFY",
	"ERR_WILL_KILL_YOU",

	/* Done as or to wrong person */
	"ERR_MUST_BE_HOST",
	"ERR_CANT_BE_HOST",
	"ERR_MUST_BE_LINK",
	"ERR_CANT_BE_LINK",
	"ERR_MUST_BETO_HOST",
	"ERR_CANT_BETO_HOST",
	"ERR_MUST_BETO_LINK",
	"ERR_CANT_BETO_LINK",
	"ERR_NOT_ON_YOURSELF",
	"ERR_NOT_ON_IRCOP",
	"ERR_ONLY_ON_IRCOP",
	"ERR_MUST_BE_IRCOP",

	/****************************************************************************
	 *********************************** MISC ***********************************
	 ****************************************************************************/

	"INFO_SYNC_TIME",
	"INFO_LIST_MATCH",
	"INFO_END_OF_LIST",
	"INFO_EMAIL",
	"INFO_URL",
	"INFO_FOUNDER",
	"INFO_DESC",
	"INFO_MLOCK",
	"INFO_JOIN",

	/* Different lists maintinance */
	"LIST_THERE",
	"LIST_NOT_THERE",
	"LIST_NOT_FOUND",
	"LIST_ADDED",
	"LIST_ADDED_AT",
	"LIST_REMOVED",
	"LIST_REMOVED_NUM",
	"LIST_REMOVED_MASK",
	"LIST_REMOVED_ALL",
	"LIST_UNCHANGED",
	"LIST_CHANGED",
	"LIST_LIMIT",

	/* Output in multi files */
	"MULTI_GETPASS",
	"MULTI_GETPASS_WALLOP",
	"MULTI_FORBID",
	"MULTI_SUSPEND",
	"MULTI_UNSUSPEND",

	/* process stuff */
	"FLOODING",
	"TEMP_FLOOD",
	"PERM_FLOOD",
	"IS_IGNORED",
	"SERVICES_OFF_REASON",
	"SERVICES_OFF",
	"SERVICES_ON",
	"ONOFF_NOTIFY",

	/****************************************************************************
	 ********************************* NickServ *********************************
	 ****************************************************************************/

	/* INFO displays */
	"NS_INFO_INTRO",
	"NS_INFO_HOST",
	"NS_INFO_EMAIL",
	"NS_INFO_URL",
	"NS_INFO_SUSPENDED",
	"NS_INFO_USERMASK",
	"NS_INFO_REGISTERED",
	"NS_INFO_ONLINE_AS",
	"NS_INFO_AONLINE_AS",
	"NS_INFO_LAST_SEEN",
	"NS_INFO_LAST_ONLINE",
	"NS_INFO_OPTIONS",
	"NS_INFO_COUNT",
	"NS_INFO_ONLINE"

	/* FLAG names */
	"NS_FLAG_SUSPENDED",
	"NS_FLAG_KILLPROTECT",
	"NS_FLAG_SECURE",
	"NS_FLAG_PRIVATE",
	"NS_FLAG_IRCOP",
	"NS_FLAG_SOP",
	"NS_FLAG_NONE",

	/* General returns */
	"NS_REGISTERED",
	"NS_LINKED",
	"NS_DROPPED",
	"NS_CHANGE_PASSWORD",
	"NS_KILLED_IMPOSTER",
	"NS_RELEASE",
	"NS_FORCED_CHANGE",
	"NS_FORCED_KILL",
	"NS_FAILMAX_KILL",
	"NS_GHOST_KILL",
	"NS_IDENTIFIED",
	"NS_FORBID_WALLOP",

	/* Ownership and status */
	"NS_NOT_YOURS",
	"NS_IN_USE",
	"NS_NOT_IN_USE",
	"NS_YOU_NO_SLAVES",
	"NS_NO_SLAVES",
	"NS_YOU_NOT_REGISTERED",
	"NS_NOT_REGISTERED",
	"NS_TAKEN",
	"NS_CANNOT_REGISTER",
	"NS_CANNOT_LINK",
	"NS_AM_IGNORED",
	"NS_IS_SUSPENDED_MEMO",
	"NS_IS_SUSPENDED",
	"NS_IS_NOT_SUSPENDED",


	/****************************************************************************
	 ********************************* ChanServ *********************************
	 ****************************************************************************/

	/* INFO Displays */
	"CS_INFO_INTRO",
	"CS_INFO_FOUNDER",
	"CS_INFO_DESC",
	"CS_INFO_URL",
	"CS_INFO_REG_TIME",
	"CS_INFO_LAST_USED",
	"CS_INFO_SUSPENDED",
	"CS_INFO_SUSPENDER",
	"CS_INFO_TOPIC",
	"CS_INFO_TOPIC_SET",
	"CS_INFO_REVENGE",
	"CS_INFO_CHAN_STAT",
	"CS_INFO_OPTIONS",
	"CS_INFO_MLOCK",
	"CS_INFO_COUNT",

	/* FLAG Names */
	"CS_FLAG_SUSPENDED",
	"CS_FLAG_PRIVATE",
	"CS_FLAG_KEEPTOPIC",
	"CS_FLAG_TOPICLOCK",
	"CS_FLAG_SECUREOPS",
	"CS_FLAG_SECURE",
	"CS_FLAG_RESTRICTED",
	"CS_FLAG_NONE",

	/* Bitchy Stuff */
	"CS_REV_LEVEL",
	"CS_REV_SET",
	"CS_REV_DEOP",
	"CS_REV_KICK",
	"CS_REV_BAN",
	"CS_SUSPENDED_TOPIC",
	"CS_FORBID_WALLOP",

	/* Levels and Access */
	"CS_LEVEL_YOU",
	"CS_LEVEL_LIST",
	"CS_LEVEL_LOW",
	"CS_LEVEL_HIGH",
	"CS_LEVEL_CHANGE",
	"CS_LEVEL_NO_CHANGE",
	"CS_LEVEL_RESET",
	"CS_LEVEL_NONE",
	"CS_ACCESS_ZERO",
	"CS_ACCESS_LOW",
	"CS_ACCESS_HIGH",
	"CS_ACCESS_HIGHER",
	"CS_ACCESS_HIGHER_MATCH",

	/* General Outputs */
	"CS_REGISTERED",
	"CS_DROPPED",
	"CS_CHANGE_PASSWORD",
	"CS_IDENTIFIED",
	"CS_YOU_UNBANNED",
	"CS_UNBANNED",
	"CS_CLEARED",
	"CS_CLEAR_KICK",

	/* Ownerships */
	"CS_ERR_SUSPENDED",
	"CS_ERR_REGISTERED",
	"CS_IN_USE",
	"CS_NOT_IN_USE",
	"CS_NOT_REGISTERED",
	"CS_TAKEN",
	"CS_CANNOT_REGISTER",
	"CS_FORBIDDEN",
	"CS_GET_OUT",
	"CS_IS_SUSPENDED",
	"CS_IS_NOT_SUSPENDED",
	"CS_YOU_NOT_IN_CHAN",
	"CS_YOU_IN_CHAN",
	"CS_NOT_IN_CHAN",
	"CS_IN_CHAN",
	"CS_YOU_NOT_GOT",
	"CS_NOT_GOT",
	"CS_YOU_ALREADY_GOT",
	"CS_ALREADY_GOT",

	/****************************************************************************
	 ********************************* MemoServ *********************************
	 ****************************************************************************/

	"MS_IS_BACKUP",

	"MS_YOU_DONT_HAVE",
	"NS_YOU_DONT_HAVE",
	"MS_YOU_HAVE",
	"NS_YOU_HAVE",
	"MS_DOESNT_EXIST",
	"NS_DOESNT_EXIST",
	"NS_MAY_NOT",

	"MS_LIST",
	"NS_LIST",
	"MS_NEW",
	"MS_READ_NEW",
	"NS_NEW",
	"NS_READ_NEW",
	"MS_MEMO",
	"NS_MEMO",
	"MS_TODEL",
	"MS_TODEL_ALL",

	"MS_SEND",
	"NS_SEND",
	"MS_MASS_SEND",
	"MS_DELETE",
	"NS_DELETE",
	"MS_DELETE_ALL",
	"NS_DELETE_ALL",

	/****************************************************************************
 	 ********************************* OperServ *********************************
	 ****************************************************************************/


	"OS_GLOBAL_WALLOP",

	"OS_SET_EXP",
	"OS_SET_EXP_CHAN",
	"OS_SET_EXP_NICK",
	"OS_SET_EXP_NEWS",
	"OS_SET_EXP_AKILL",
	"OS_SET_SOPS",
	"OS_SET_CLONES",
	"OS_SET_RELEASE",
	"OS_SET_AKICKS",
	"OS_SET_FLOOD",
	"OS_SET_IGNORE",
	"OS_SET_RELINK",
	"OS_SET_OVERRIDE",
	"OS_SET_UPDATE",
	"OS_SET_ADMINS",

	"OS_QLINE",
	"OS_UNQLINE",
	"OS_SVSNOOP_ON",
	"OS_SVSNOOP_OFF",
	"OS_JUPE",
	"OS_UPDATE",
	"OS_NEW_MESSAGE",
	"OS_AKILL_ADDED",
	"OS_AKILL_NOT_THERE",
	"OS_AKILL_EXPIRE",
	"OS_AKILL_BANNED",
	"OS_CLONE_HIGH",
	"OS_CLONE_LOW",
	""
	}

	int i=0;
	// todo use magick.ini LANGUAGE value
	wxFileConfig *languageini=new wxFileConfig("magick","","english.lng","");
	// load and escparse the messages

	while(tempstor[i]!="")
	{
		mstring name,value;
		name=tempstor[i].After('/').Trim().Trim(false);
		//languageini.Read(
		Messages[name]=value;
		i++;
	}

	delete languageini;

}
