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

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"
#include "fileconf.h"
#include "datetime.h"	// Added by ClassView
#include "trace.h"
#include "nickserv.h"
#include "chanserv.h"
#include "ircsocket.h"
#include "variant.h"

const int MAGICK_RET_NORMAL		    = 0;
const int MAGICK_RET_RESTART		    = 1;
const int MAGICK_RET_TERMINATE		    = 2;
const int MAGICK_RET_ERROR		    = -1;
const int MAGICK_RET_INVALID_SERVICES_DIR   = -20;

#if 0
const int RUN_STARTED	    =0x00000001;
const int RUN_MODE	    =0x00000002;
const int RUN_LOG_IS_OPEN   =0x00000004;
const int RUN_DEBUG	    =0x00000008;
const int RUN_SIGTERM	    =0x00000010;
const int RUN_SAVE_DATA	    =0x00000020;
const int RUN_SEND_PINGS    =0x00000040;
const int RUN_NOSEND	    =0x00000080;
const int RUN_QUITTING	    =0x00000100;
const int RUN_TERMINATING   =0x00000200;
const int RUN_NOSLEEP	    =0x00000400;
const int RUN_LIVE	    =0x00000800;
#endif

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

	ChanServ chanserv;
	NickServ nickserv;
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

//	mstring Services_NickServ;
//	mstring Services_NickServ_Name;
//	mstring Services_ChanServ;
//	mstring Services_ChanServ_Name;
	mstring Services_MemoServ;
	mstring Services_MemoServ_Name;
	bool Services_MEMO;
	bool Services_NEWS;
	mstring Services_OperServ;
	mstring Services_OperServ_Name;
	bool Services_FLOOD;
	bool Services_AKILL;
	bool Services_OPERDENY;
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

	mstring OperServ_SERVICES_ADMIN;
	mstring OperServ_EXPIRE_OPER;
	mstring OperServ_EXPIRE_ADMIN;
	mstring OperServ_EXPIRE_SOP;
	mstring OperServ_EXPIRE_SADMIN;
	int OperServ_CLONE_LIMIT;
	mstring OperServ_DEF_CLONE;
	int OperServ_FLOOD_TIME;
	int OperServ_FLOOD_MSGS;
	int OperServ_IGNORE_TIME;
	int OperServ_IGNORE_LIMIT;
	int OperServ_IGNORE_REMOVE;
	int OperServ_IGNORE_METHOD;

	bool CommServ_SECURE_OPER;
	mDateTime ResetTime;
};

extern Magick *Parent;
extern mDateTime StartTime;

inline void SendSVR(mstring message)
{ Parent->ircsvchandler->send(":" + Parent->Startup_SERVER_NAME + " " + message); }

inline void Send(mstring message)
{ Parent->ircsvchandler->send(message); }


#endif
