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

#ifndef _MAGICK_H
#define _MAGICK_H

#include "mstring.h"
#include "fileconf.h"
#include "datetime.h"	// Added by ClassView
#include "trace.h"
#include "server.h"
#include "operserv.h"
#include "nickserv.h"
#include "chanserv.h"
#include "memoserv.h"
#include "servmsg.h"
#include "commserv.h"
#include "ircsocket.h"
#include "variant.h"
#include "version.h"
#include "log.h"

const int MAGICK_RET_NORMAL		    = 0;
const int MAGICK_RET_RESTART		    = 1;
const int MAGICK_RET_TERMINATE		    = 2;
const int MAGICK_RET_ERROR		    = -1;
const int MAGICK_RET_INVALID_SERVICES_DIR   = -20;

class Magick; // fwd reference, leave it here
const mstring ChanSpec = "#&+";

class SignalHandler : public ACE_Event_Handler
{
public:
    int handle_signal(int signum, siginfo_t *siginfo, ucontext_t *ucontext);
};

typedef map<mstring,mstring> mapstringstring;
class Magick
{
private:
	vector<mstring> argv;
	mapstringstring Messages;
	vector<mstring> MessageNamesLong;
	vector<mstring> MessageNamesShort;
	int doparamparse();
	SignalHandler *signalhandler;
	map<pair<mstring,mstring>,vector<mstring> > handlermap;

	bool messages;		// Wether to process /MSG, /NOTICE.
	bool automation;	// Wether to do automatic tasks.


public:
	// Config Values
        class statup_t{
		friend Magick;

		mstring remote_server;
		int remote_port;
		mstring password;
		mstring server_name;
		mstring server_desc;
		mstring services_user;
		mstring services_host;
		bool ownuser;
		int level;
		int lagtime;
		int deadtime;
		float gmt;
	public:
		mstring Remote_Server()	{ return remote_server; }
		int Remote_Port()	{ return remote_port; }
		mstring Password()	{ return password; }
		mstring Server_Name()	{ return server_name; }
		mstring Server_Desc()	{ return server_desc; }
		mstring Services_User()	{ return services_user; }
		mstring Services_Host()	{ return services_host; }
		bool Ownuser()		{ return ownuser; }
		int Level()		{ return level; }
		int Lagtime()		{ return lagtime; }
		int Deadtime()		{ return deadtime; }
		float GMT()		{ return gmt; }
	} startup;

	class files_t{
		friend Magick;

		mstring pidfile;
		mstring logfile;
		mstring motdfile;
		mstring language;
		mstring commands;
		mstring link_db;
		mstring nick_db;
		mstring chan_db;
		mstring memo_db;
		mstring news_db;
		mstring akill_db;
		mstring ignore_db;
		mstring clone_db;
		mstring comm_db;
		mstring msgs_db;
		bool compression;
	public:
		mstring Pidfile()	{ return pidfile; }
		mstring Logfile()	{ return logfile; }
		mstring Motdfile()	{ return motdfile; }
		mstring Language()	{ return language; }
		mstring Commands()	{ return commands; }
		mstring Link_DB()	{ return link_db; }
		mstring Nick_DB()	{ return nick_db; }
		mstring Chan_DB()	{ return chan_db; }
		mstring Memo_DB()	{ return memo_db; }
		mstring News_DB()	{ return news_db; }
		mstring AKill_DB()	{ return akill_db; }
		mstring Ignore_DB()	{ return ignore_db; }
		mstring Clone_DB()	{ return clone_db; }
		mstring Comm_DB()	{ return comm_db; }
		mstring Msgs_DB()	{ return msgs_db; }
		bool Compression()	{ return compression; }
	} files;

	class config_t{
		friend Magick;

		int server_relink;
		int cycletime;
		int ping_frequency;
		int starthresh;
	public:
		int Server_Relink()	{ return server_relink; }
		int Cycletime()		{ return cycletime; }
		int Ping_Frequency()	{ return ping_frequency; }
		int Starthresh()	{ return starthresh; }
	} config;

	bool Files_COMPRESS_STREAMS;
	mstring Password;
	 ~Magick();
	IrcSvcHandler *ircsvchandler;
	void shutdown(bool in);
	bool shutdown();

	// get script to handle it.
	void doscripthandle(const mstring& server, const mstring& command, const mstring& data);
	// is there a script handler there?
	bool checkifhandled(const mstring& server, const mstring& command);
	// remove ascript function to handle commands
	void stophandling(const mstring& server, const mstring& command, const mstring& functionname);
	// add a script function to handle commands
	void handle(const mstring& server, const mstring& command, const mstring& functionname);

	bool MSG() { return messages; }
	void MSG(bool on) { messages = on; }
	bool AUTO() { return automation; }
	void AUTO(bool on) { automation = on; }

	wxFileConfig* MagickIni;
	long runflags;
	mstring services_dir;
	mstring config_file;
	int high_water_mark;
	int low_water_mark;
	mstring ProgramName;
	bool debug;
	bool live;
	bool reconnect;
        Reconnect_Handler rh;
        operator mVariant() const { mVariant locvar("Magick"); locvar.truevaluetype="Magick"; return locvar; };

	void get_config_values();
	bool check_config();
	int ping_frequency;
	int update_timeout;
	int server_relink;
	void LoadExternalMessages();
	mstring parseEscapes(const mstring& in);
	bool paramlong(mstring first, mstring second);
	bool paramshort(mstring first, mstring second);
	void LoadInternalMessages();
	void dump_help(mstring& progname);
	mstring getMessage(const mstring& name);
	Magick(int inargc, char **inargv);
	int Start();

	OperServ operserv;
	ChanServ chanserv;
	NickServ nickserv;
	MemoServ memoserv;
	ServMsg servmsg;
	CommServ commserv;
	NetworkServ server;
	LoggerTask loggertask;
        IrcServer ACO_server;

//protected:
	bool i_shutdown;

	mDateTime ResetTime;
};

extern Magick *Parent;
extern mDateTime StartTime;

inline void KillUnknownUser(mstring user)
{ Parent->ircsvchandler->send(":" + Parent->startup.Server_Name() + " KILL " +
    user + " :" + Parent->startup.Server_Name() + " (" + user + "(?) <- " +
    Parent->startup.Remote_Server() + ")"); }

inline bool IsChan(mstring input)
{ return (ChanSpec.Contains(input[0U])); }

#endif
