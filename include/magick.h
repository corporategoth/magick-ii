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

#include <vector>
#include <map>
using namespace std;
//#include <ace/Synch.h>
#include <ace/Thread.h>
#include <ace/Local_Tokens.h>
#include <ace/Reactor.h>
#include <ace/Event_Handler.h>
#include "bob.hpp"
#include "mstring.h"
#include "fileconf.h"
#include "datetime.h"	// Added by ClassView
#include "trace.h"
#include "chanserv.h"

const int MAGICK_RET_NORMAL = 0;
const int MAGICK_RET_RESTART = 1;
const int MAGICK_RET_TERMINATE = 2;
const int MAGICK_RET_ERROR = -1;
const int MAGICK_RET_INVALID_SERVICES_DIR = -20;

class SignalHandler : public ACE_Event_Handler
{
    int handle_signal(int signum, siginfo_t *siginfo, ucontext_t *ucontext); 
};

typedef map<mstring,mstring> mapstringstring;
class Magick
{
private:
	vector<mstring> argv;
	wxFileConfig* MagickIni;
	mapstringstring Messages;
	vector<mstring> MessageNamesLong;
	vector<mstring> MessageNamesShort;
	int doparamparse();
	SignalHandler *signalhandler;
public:
	int StarThresh;
	int override_level;
	mstring motd_filename;
	bool show_sync;
	bool shutdown;
	mstring password;
	void get_config_values();
	mstring pid_filename;
	mstring s_Outlet;
	int lastmsgmax;
	int flood_messages;
	// this get's moved to nickserv
	int passfail_max;
	int read_timeout;
	// move these to the appropriate classes later
	bool globalnoticer_on;
	bool outlet_on;
	bool akill_on;
	bool clones_on;
	bool operserv_on;
	int clones_allowed;


	bool check_config();
	int ping_frequency;
	int update_timeout;
	int server_relink;
	ChanServ chanserv;
	map<ACE_thread_t,threadtype_enum> ThreadtoTypeMap;
	mDateTime StartTime;
	void LoadExternalMessages();
	mstring parseEscapes(const mstring& in);
	void LoadInternalMessages();
	void dump_help(mstring& progname);
	mstring getMessage(const mstring& name);
	mstring config_file;
	mstring services_dir;
	mstring ProgramName;
	Bob bob;
	Magick(int inargc, char **inargv);
	int Start();
protected:
	int tz_offset;
	int services_level;
	bool live;
	bool debug;
	mstring log_filename;
	mstring services_prefix;
	mstring services_host;
	mstring services_user;
	mstring server_desc;
	mstring server_name;
	int remote_port;
	mstring remote_server;
};

extern Magick *MagickObject;
extern mDateTime Reset_Time;

#endif