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
#include "nickserv.h"
#include "chanserv.h"
#include "operserv.h"
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
inline bool IsChan(mstring input)
{ return (ChanSpec.Contains(input[0U])); }

class SignalHandler : public ACE_Event_Handler
{
public:
    int handle_signal(int signum, siginfo_t *siginfo, ucontext_t *ucontext);
};

class Magick
{
    friend class Reconnect_Handler;
private:
	vector<mstring> argv;
	// Language, longname, string
	map<mstring, map<mstring, mstring> > Messages;
	map<mstring, mstring> LogMessages;
	int doparamparse();
	SignalHandler *signalhandler;
	map<pair<mstring,mstring>,vector<mstring> > handlermap;

	mstring i_services_dir;
	mstring i_config_file;
	mstring i_programname;

	mDateTime i_ResetTime;
	unsigned int i_level;
	bool i_auto;
	bool i_shutdown;

	bool i_reconnect;
  	IrcSvcHandler *ircsvchandler;
	bool i_gotconnect;
	mstring i_server;
	bool i_connected;

public:
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

	mstring Services_Dir()	    { return i_services_dir; }
	mstring Config_File()	    { return i_config_file; }
	mstring ProgramName()	    { return i_programname; }

	// Current STATES, and switching between them.
	Magick(int inargc, char **inargv);
	int Start();
	mDateTime ResetTime()	{ return i_ResetTime; }
	unsigned int Level()	{ return i_level; }
	void LevelUp()
	{
	    i_level++;
	}
	void LevelDown()
	{
	    if (i_level > startup.Level())
		i_level--;
	}
	void AUTO(bool on)	{ i_auto = on; }
	bool AUTO()		{ return i_auto; }
	void MSG(bool on)
	{
	//  operserv.MSG(on);
	    nickserv.MSG(on);
	    chanserv.MSG(on);
	    memoserv.MSG(on);
	    servmsg.MSG(on);
	    commserv.MSG(on);
	}
	void Die()		{ ACE_Reactor::instance()->end_event_loop(); }
	void Shutdown(bool in)	{ i_shutdown = in; }
	bool Shutdown()		{ return i_shutdown; }
	 ~Magick();

	// Streams, etc
	bool Reconnect()	    { return i_reconnect; }
	bool GotConnect()	    { return i_gotconnect; }
	void GotConnect(bool in)    { i_gotconnect = in; }
	mstring Server()	    { return i_server; }
	bool Connected()	    { return i_connected; }
	void Connected(bool in)	    { i_connected = in; }
	void Disconnect();
	void send(mstring text);
	void save_databases();
	void load_databases();
        Reconnect_Handler rh;
	operator mVariant() const { mVariant locvar("Magick"); locvar.truevaluetype="Magick"; return locvar; };

	// Commandline, config, language PARSING.
	void dump_help(mstring& progname);
	bool paramlong(mstring first, mstring second);
	bool paramshort(mstring first, mstring second);
	bool get_config_values();
	void LoadInternalMessages();
	bool LoadExternalMessages(mstring language);
	bool LoadLogMessages(mstring language);
	bool UnloadExternalMessages(mstring language);
	mstring getMessage(const mstring& nick, const mstring& name);
	mstring getMessage(const mstring& name)
	    { return getMessageL(nickserv.DEF_Language(), name); }
	mstring getMessageL(const mstring& language, const mstring& name);
	mstring getLogMessage(const mstring& name);
	mstring parseEscapes(const mstring& in);
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

	mstring getLname(mstring in)
	{
	    if (IsChan(in))
	    {
		if (chanserv.IsLive(in))
		    return chanserv.live[in.LowerCase()].Name();
	    }
	    else
	    {
		if (nickserv.IsLive(in))
		    return nickserv.live[in.LowerCase()].Name();
	    }
	    return "";
	}

	mstring getSname(mstring in)
	{
	    if (IsChan(in))
	    {
		if (chanserv.IsStored(in))
		    return chanserv.stored[in.LowerCase()].Name();
	    }
	    else
	    {
		if (nickserv.IsStored(in))
		    return nickserv.stored[in.LowerCase()].Name();
	    }
	    return "";
	}
};

extern Magick *Parent;
extern mDateTime StartTime;

#endif
