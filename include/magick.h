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

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "mstring.h"
#include "fileconf.h"
#include "datetime.h"	// Added by ClassView
#include "trace.h"
#include "server.h"
#include "operserv.h"
#include "nickserv.h"
#include "chanserv.h"
#include "memoserv.h"
#include "helpserv.h"
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
	bool automation;		// Wether to do automatic tasks.
public:
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
	HelpServ helpserv;
	NetworkServ server;
	LoggerTask loggertask;
        IrcServer ACO_server;

//protected:
	bool i_shutdown;

	// Config Values
	mstring Startup_REMOTE_SERVER;
	int Startup_REMOTE_PORT;
	mstring Startup_PASSWORD;
	mstring Startup_SERVER_NAME;
	mstring Startup_SERVER_DESC;
	mstring Startup_SERVICES_USER;
	mstring Startup_SERVICES_HOST;
	bool Startup_OWNUSER;
	int Startup_LEVEL;
	int Startup_LAGTIME;
	int Startup_DEADTIME;
	float Startup_GMT;

	mstring Services_CommServ;
	mstring Services_CommServ_Name;
	mstring Services_ServMsg;
	mstring Services_ServMsg_Name;
	bool Services_SHOWSYNC;

	mstring Files_PIDFILE;
	mstring Files_LOGFILE;
	mstring Files_MOTDFILE;
	mstring Files_LANGUAGE;
	mstring Files_COMMANDS;
	mstring Files_LINK_DB;
	mstring Files_NICK_DB;
	mstring Files_CHAN_DB;
	mstring Files_MEMO_DB;
	mstring Files_NEWS_DB;
	mstring Files_AKILL_DB;
	mstring Files_IGNORE_DB;
	mstring Files_CLONE_DB;
	mstring Files_COMM_DB;
	mstring Files_MSGS_DB;

	int Config_SERVER_RELINK;
	int Config_CYCLETIME;
	int Config_PING_FREQUENCY;
	int Config_STARTHRESH;

	int MemoServ_NEWS_EXPIRE;

	bool CommServ_SECURE_OPER;
	mDateTime ResetTime;
};

extern Magick *Parent;
extern mDateTime StartTime;

inline void SendSVR(mstring message)
{ Parent->ircsvchandler->send(":" + Parent->Startup_SERVER_NAME + " " + message); }

inline void Send(mstring message)
{ Parent->ircsvchandler->send(message); }

inline void KillUnknownUser(mstring user)
{ Parent->ircsvchandler->send(":" + Parent->Startup_SERVER_NAME + " KILL " +
    user + " :" + Parent->Startup_SERVER_NAME + " (" + user + "(?) <- " +
    Parent->Startup_REMOTE_SERVER + ")"); }


#endif
