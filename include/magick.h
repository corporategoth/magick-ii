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
#include "utils.h"
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
#include "cryptstream.h"

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

	// loading and saving internal stuff
	wxInputStream *create_input_stream(wxMemoryStream &in);
	void destroy_input_stream();
	wxOutputStream *create_output_stream(wxMemoryStream &out);
	void destroy_output_stream();
        wxZlibInputStream *zstrm;
        wxZlibOutputStream *ozstrm;
	mDecryptStream *cstrm;
	mEncryptStream *ocstrm;

	// Running config
	

public:
	void save_databases();
	void load_databases();

	// Config Values
	class startup_t {
		friend Magick;

		// map<server name, triplet<port, password, priority> >
		map<mstring,triplet<unsigned int,mstring,unsigned int> > servers;
		mstring server_name;
		mstring server_desc;
		mstring services_user;
		mstring services_host;
		bool ownuser;
		unsigned int level;
		unsigned long lagtime;
	public:
		bool IsServer(mstring server);
		triplet<unsigned int,mstring,unsigned int> Server(mstring server);
		vector<mstring> PriorityList(unsigned int pri);
		size_t Server_size() { return servers.size(); }

		mstring Server_Name()const	{ return server_name; }
		mstring Server_Desc()const	{ return server_desc; }
		mstring Services_User()const	{ return services_user; }
		mstring Services_Host()const	{ return services_host; }
		bool Ownuser()const		{ return ownuser; }
		unsigned int Level()const	{ return level; }
		unsigned long Lagtime()const	{ return lagtime; }
	} startup;

	class files_t {
		friend Magick;

		mstring pidfile;
		mstring logfile;
		mstring motdfile;
		mstring language;
		mstring database;
		unsigned int compression;
		mstring keyfile;
		bool encryption;
	public:
		mstring Pidfile()const		    { return pidfile; }
		mstring Logfile()const		    { return logfile; }
		mstring Motdfile()const		    { return motdfile; }
		mstring Language()const		    { return language; }
		mstring Database()const		    { return database; }
		unsigned int Compression()const	    { return compression; }
		mstring KeyFile()const		    { return keyfile; }
		bool Encryption()const		    { return encryption; }
	} files;

	class config_t {
		friend Magick;

		unsigned long server_relink;
		unsigned long squit_protect;
		unsigned long squit_cancel;
		unsigned long cycletime;
		unsigned long checktime;
		unsigned long ping_frequency;
		unsigned int starthresh;
		unsigned int listsize;
		unsigned int maxlist;
		unsigned int startup_threads;
		unsigned int low_water_mark;
		unsigned int high_water_mark;
	public:
		unsigned long Server_Relink()	{ return server_relink; }
		unsigned long Squit_Protect()	{ return squit_protect; }
		unsigned long Squit_Cancel()	{ return squit_cancel; }
		unsigned long Cycletime()	{ return cycletime; }
		unsigned long Checktime()	{ return checktime; }
		unsigned long Ping_Frequency()	{ return ping_frequency; }
		unsigned int Starthresh()	{ return starthresh; }
		unsigned int Listsize()		{ return listsize; }
		unsigned int Maxlist()		{ return maxlist; }
		unsigned int Startup_Threads()	{ return startup_threads; }
		unsigned int Low_Water_Mark()	{ return low_water_mark; }
		unsigned int High_Water_Mark()	{ return high_water_mark; }
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

	long runflags;
	mstring services_dir;
	mstring config_file;
	unsigned int level;
	mstring ProgramName;
	bool debug;
	bool live;
	bool reconnect;
        Reconnect_Handler rh;
        operator mVariant() const { mVariant locvar("Magick"); locvar.truevaluetype="Magick"; return locvar; };

	bool GotConnect;
	mstring Server;

	bool get_config_values();
	bool check_config();
	void LoadExternalMessages();
	mstring parseEscapes(const mstring& in);
	bool paramlong(mstring first, mstring second);
	bool paramshort(mstring first, mstring second);
	void LoadInternalMessages();
	void dump_help(mstring& progname);
	mstring getMessage(const mstring& name);
	Magick(int inargc, char **inargv);
	int Start();

	// Stuff that NEEDS to be there first
	LoggerTask loggertask;
	CommandMap commands;

	// Services ...
	OperServ operserv;
	ChanServ chanserv;
	NickServ nickserv;
	MemoServ memoserv;
	ServMsg servmsg;
	CommServ commserv;

	// Other stuff ...
	NetworkServ server;
        IrcServer ACO_server;
	EventTask events;

	void AddCommands(void)
	{
	    operserv.AddCommands();
	    nickserv.AddCommands();
	    chanserv.AddCommands();
	    memoserv.AddCommands();
	    servmsg.AddCommands();
	    commserv.AddCommands();
	}

	void RemCommands(void)
	{
	    operserv.RemCommands();
	    nickserv.RemCommands();
	    chanserv.RemCommands();
	    memoserv.RemCommands();
	    servmsg.RemCommands();
	    commserv.RemCommands();
	}

//protected:
	bool i_shutdown;

	mDateTime ResetTime;
};

extern Magick *Parent;
extern mDateTime StartTime;

inline void KillUnknownUser(mstring user)
{ Parent->ircsvchandler->send(":" + Parent->startup.Server_Name() + " KILL " +
    user + " :" + Parent->startup.Server_Name() + " (" + user + "(?) <- " +
    Parent->Server + ")"); }

inline bool IsChan(mstring input)
{ return (ChanSpec.Contains(input[0U])); }

#endif
