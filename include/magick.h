#ifndef WIN32
#pragma interface
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#ifndef _MAGICK_H
#define _MAGICK_H
#include "pch.h"
RCSID(magick_h, "@(#) $Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "mexceptions.h"
#include "filesys.h"
#include "utils.h"
#include "stages.h"
#include "server.h"
#include "nickserv.h"
#include "chanserv.h"
#include "operserv.h"
#include "memoserv.h"
#include "servmsg.h"
#include "commserv.h"
#include "ircsocket.h"
#include "version.h"

const int MAGICK_RET_NORMAL = 0;
const int MAGICK_RET_RESTART = 1;
const int MAGICK_RET_ERROR = -1;
const int MAGICK_RET_LOCKED = -2;
const int MAGICK_RET_STATE = -3;

const mstring ChanSpec = "#&+!";

inline bool IsChan(mstring input)
{
    return (ChanSpec.Contains(input[0U]));
}

class SignalHandler : public ACE_Event_Handler
{
public:
    int handle_signal(int signum, siginfo_t * siginfo, ucontext_t * ucontext);
};

class Logger : public ACE_Log_Msg_Callback
{
    mFile fout;

public:
    Logger();
    ~Logger();

    void log(ACE_Log_Record & log_record);
    void close();
    void open();
    bool opened() const;
};

#define LOG2(X)	\
	if (Magick::instance_exists() && \
	    Magick::instance().ValidateLogger(ACE_LOG_MSG)) { \
		ACE_DEBUG(X); \
		Magick::instance().EndLogMessage(ACE_LOG_MSG); } \

#define LOG(X, Y, Z) \
	{ LOG2((X, parseMessage(Magick::instance().getLogMessage(Y), mVarArray Z))); }
#define NLOG(X, Y) \
	{ LOG2((X, parseMessage(Magick::instance().getLogMessage(Y)))); }
#define SLOG(X, Y, Z) \
	{ LOG2((X, parseMessage(Y, mVarArray Z))); }
#define NSLOG(X, Y) \
	{ LOG2((X, parseMessage(Y))); }

class Magick : public SXP::IPersistObj
{
    friend class Reconnect_Handler;
    friend class Disconnect_Handler;

private:
    enum
    { Unknown = 0, Constructed, Initialized, Started,
	Running, RunCompleted, Stopped, Finished
    }
    CurrentState;

    static map < ACE_thread_t, Magick * > InstanceMap;
    ACE_Reactor i_reactor;
    ACE_Thread_Manager i_thr_mgr;

    vector < mstring > argv;
    // Language, token, string
    map < mstring, map < mstring, mstring > > Messages;
    // Language, token, vector<yescom, nocom, string>
    map < mstring, map < mstring, vector < triplet < mstring, mstring, mstring > > > > Help;
    // Token, string
    map < mstring, mstring > LogMessages;
    int doparamparse();
    SignalHandler *signalhandler;

    map < pair < mstring, mstring >, vector < mstring > > handlermap;

    Logger *logger;

    set < ACE_Log_Msg * > LogInstances;
    bool i_verbose;

    mstring i_services_dir;
    mstring i_config_file;
    mstring i_programname;

    static mDateTime i_StartTime;
    mDateTime i_ResetTime;
    unsigned int i_level;
    bool i_pause;
    bool i_auto;
    bool i_shutdown;

    bool i_reconnect;
    unsigned long i_localhost;
    bool i_gotconnect;
    mstring i_currentserver;
    bool i_connected;
    bool i_saving;

    static SXP::Tag tag_Magick;

public:
    EventTask * events;
    DccMap *dcc;
    IrcSvcHandler *ircsvchandler;

    // Stuff that NEEDS to be there first
    CommandMap commands;
    FileMap filesys;

    // Services ...
    OperServ operserv;
    ChanServ chanserv;
    NickServ nickserv;
    MemoServ memoserv;
    ServMsg servmsg;
    CommServ commserv;

    // Other stuff ...
    Server server;

#ifdef MAGICK_HAS_EXCEPTIONS
    static void register_instance(Magick * ins, ACE_thread_t id = ACE_Thread::self()) throw(E_Magick);
#else
    static void register_instance(Magick * ins, ACE_thread_t id = ACE_Thread::self());
#endif
    static void deregister_instance(ACE_thread_t id = ACE_Thread::self());
    static bool instance_exists(ACE_thread_t id = ACE_Thread::self());

#ifdef MAGICK_HAS_EXCEPTIONS
    static Magick &instance(ACE_thread_t id = ACE_Thread::self()) throw(E_Magick);
#else
    static Magick &instance(ACE_thread_t id = ACE_Thread::self());
#endif
    ACE_Reactor &reactor()
    {
	return i_reactor;
    }
    ACE_Thread_Manager &thr_mgr()
    {
	return i_thr_mgr;
    }

    // Config Values
    class startup_t
    {
	friend class Magick;

	// map<server name, pair<priority, triplet<port, password, numeric> > >
	map < mstring, pair < unsigned int, triplet < unsigned int, mstring, unsigned long > > > servers;

	// map<server name, vector<allowed uplinks> >
	map < mstring, vector < mstring > > allows;
	mstring server_name;
	mstring server_desc;
	mstring services_user;
	mstring services_host;
	mstring services_quitmsg;
	bool ownuser;
	mstring setmode;
	mstring bind;
	unsigned int level;
	unsigned long lagtime;

    public:
	bool IsServer(const mstring & server) const;
	pair < unsigned int, triplet < unsigned int, mstring, unsigned long > > Server(const mstring & server) const;
	vector < mstring > PriorityList(const unsigned int pri) const;
	size_t Server_size() const
	{
	    return servers.size();
	}

	bool IsAllowed(const mstring & server, const mstring & uplink) const;
	vector < mstring > Allow(const mstring & server) const;
	vector < mstring > AllowList() const;
	size_t Allow_size() const
	{
	    return allows.size();
	}

	mstring Server_Name() const
	{
	    return server_name;
	}
	mstring Server_Desc() const
	{
	    return server_desc;
	}
	mstring Services_User() const
	{
	    return services_user;
	}
	mstring Services_Host() const
	{
	    return services_host;
	}
	mstring Services_Quitmsg() const
	{
	    return services_quitmsg;
	}
	bool Ownuser() const
	{
	    return ownuser;
	}
	mstring Setmode() const
	{
	    return setmode;
	}
	mstring Bind() const
	{
	    return bind;
	}
	unsigned int Level() const
	{
	    return level;
	}
	unsigned long Lagtime() const
	{
	    return lagtime;
	}
    }
    startup;

    class files_t
    {
	friend class Magick;

	mode_t umask;
	mstring pidfile;
	mstring logfile;
	mstring logchan;
	mstring motdfile;
	mstring langdir;
	mstring database;
	unsigned int compression;
	mstring keyfile;
	bool encryption;
	mstring memoattach;
	unsigned long memoattachsize;
	mstring picture;
	unsigned long picturesize;
	mstring i_public;
	unsigned long publicsize;
	mstring tempdir;
	unsigned long tempdirsize;
	unsigned long blocksize;
	unsigned long timeout;
	unsigned long min_speed;
	unsigned long max_speed;
	unsigned long sampletime;

    public:
	mstring MakePath(const mstring & in) const
	{
#ifdef WIN32
	    if (in[1u] == ':' && mstring(in[2u]) == DirSlash)
		return in;
	    else
		return Magick::instance().Services_Dir() + DirSlash + in;
#else
	    if (mstring(in[0u]) == DirSlash)
		return in;
	    else
		return Magick::instance().Services_Dir() + DirSlash + in;
#endif
	}
	mode_t Umask() const
	{
	    return umask;
	}
	mstring Pidfile() const
	{
	    return MakePath(pidfile);
	}
	mstring Logfile() const
	{
	    return MakePath(logfile);
	}
	mstring Logchan() const
	{
	    return logchan;
	}
	mstring Motdfile() const
	{
	    return MakePath(motdfile);
	}
	mstring Langdir() const
	{
	    return MakePath(langdir);
	}
	mstring Database() const
	{
	    return MakePath(database);
	}
	unsigned int Compression() const
	{
	    return compression;
	}
	mstring KeyFile() const
	{
	    return MakePath(keyfile);
	}
	bool Encryption() const
	{
	    return encryption;
	}
	mstring MemoAttach() const
	{
	    return MakePath(memoattach);
	}
	unsigned long MemoAttachSize() const
	{
	    return memoattachsize;
	}
	mstring Picture() const
	{
	    return MakePath(picture);
	}
	unsigned long PictureSize() const
	{
	    return picturesize;
	}
	mstring Public() const
	{
	    return MakePath(i_public);
	}
	unsigned long PublicSize() const
	{
	    return publicsize;
	}
	mstring TempDir() const
	{
	    return MakePath(tempdir);
	}
	unsigned long TempDirSize() const
	{
	    return tempdirsize;
	}
	unsigned long Blocksize() const
	{
	    return blocksize;
	}
	unsigned long Timeout() const
	{
	    return timeout;
	}
	unsigned long Min_Speed() const
	{
	    return min_speed;
	}
	unsigned long Max_Speed() const
	{
	    return max_speed;
	}
	unsigned long Sampletime() const
	{
	    return sampletime;
	}
    }
    files;

    class config_t
    {
	friend class Magick;

	unsigned long server_relink;
	unsigned long squit_protect;
	unsigned long squit_cancel;
	unsigned long cycletime;
	unsigned long savetime;
	unsigned long checktime;
	unsigned long ping_frequency;
	unsigned int starthresh;
	unsigned int listsize;
	unsigned int maxlist;
	unsigned int min_threads;
	unsigned int max_threads;
	unsigned int low_water_mark;
	unsigned int high_water_mark;
	unsigned long heartbeat_time;
	unsigned long msg_seen_time;
	unsigned long msg_check_time;

    public:
	unsigned long Server_Relink() const
	{
	    return server_relink;
	}
	unsigned long Squit_Protect() const
	{
	    return squit_protect;
	}
	unsigned long Squit_Cancel() const
	{
	    return squit_cancel;
	}
	unsigned long Cycletime() const
	{
	    return cycletime;
	}
	unsigned long Savetime() const
	{
	    return savetime;
	}
	unsigned long Checktime() const
	{
	    return checktime;
	}
	unsigned long Ping_Frequency() const
	{
	    return ping_frequency;
	}
	unsigned int Starthresh() const
	{
	    return starthresh;
	}
	unsigned int Listsize() const
	{
	    return listsize;
	}
	unsigned int Maxlist() const
	{
	    return maxlist;
	}
	unsigned int Min_Threads() const
	{
	    return min_threads;
	}
	unsigned int Max_Threads() const
	{
	    return max_threads;
	}
	unsigned int Low_Water_Mark() const
	{
	    return low_water_mark;
	}
	unsigned int High_Water_Mark() const
	{
	    return high_water_mark;
	}
	unsigned long Heartbeat_Time() const
	{
	    return heartbeat_time;
	}
	unsigned long MSG_Seen_Time() const
	{
	    return msg_seen_time;
	}
	unsigned long MSG_Check_Time() const
	{
	    return msg_check_time;
	}
    }
    config;

    bool ActivateLogger();
    void DeactivateLogger();
    bool ValidateLogger(ACE_Log_Msg * instance) const;
    void EndLogMessage(ACE_Log_Msg * instance) const;
    bool Verbose() const
    {
	return i_verbose;
    }
    mstring Services_Dir() const
    {
	return i_services_dir;
    }
    mstring Config_File() const
    {
	return files.MakePath(i_config_file);
    }
    mstring ProgramName() const
    {
	return i_programname;
    }

    // Current STATES, and switching between them.
    Magick(int inargc, char **inargv);

    virtual ~ Magick();

    // Init and Finish are only EVER called once
    // Start should be called after Init or Stop
    // Run should only be called after Start
    // Stop should only be called after Start or Run
    int Init();
    int Start();
    int Run();
    int Stop();
    int Finish();

    void Pause(bool in)
    {
	i_pause = in;
    }
    bool Pause()
    {
	return i_pause;
    }

    static void StartTime(const mDateTime & in)
    {
	i_StartTime = in;
    }
    static mDateTime StartTime()
    {
	return i_StartTime;
    }
    mDateTime ResetTime() const
    {
	return i_ResetTime;
    }
    unsigned int Level() const
    {
	return i_level;
    }
    void LevelUp()
    {
	i_level++;
    }
    void LevelDown()
    {
	if (i_level > startup.Level())
	    i_level--;
    }
    void AUTO(const bool on)
    {
	i_auto = on;
    }
    bool AUTO() const
    {
	return i_auto;
    }
    void MSG(const bool on)
    {
	operserv.MSG(on);
	nickserv.MSG(on);
	chanserv.MSG(on);
	memoserv.MSG(on);
	servmsg.MSG(on);
	commserv.MSG(on);
    }
    void Die()
    {
	reactor().end_reactor_event_loop();
    }
    void Shutdown(const bool in)
    {
	i_shutdown = in;
    }
    bool Shutdown() const
    {
	return i_shutdown;
    }

    // Streams, etc
    bool Reconnect() const
    {
	return i_reconnect;
    }
    bool GotConnect() const
    {
	return i_gotconnect;
    }
    void GotConnect(bool in)
    {
	i_gotconnect = in;
    }
    unsigned long LocalHost() const
    {
	return i_localhost;
    }
    mstring CurrentServer() const
    {
	return i_currentserver;
    }
    bool Connected() const
    {
	return i_connected;
    }
    void Connected(bool in)
    {
	i_connected = in;
    }
    bool Saving() const
    {
	return i_saving;
    }
    void Disconnect(const bool reconnect = true);
    void send(const mstring & text) const;
    pair < mstring, mstring > GetKeys() const;
    void save_databases();
    void load_databases();

    Heartbeat_Handler hh;
    Reconnect_Handler rh;
    Disconnect_Handler dh;
    long dh_timer;

    operator      mVariant() const
    {
	mVariant locvar("Magick");

	locvar.truevaluetype = "Magick";
	return locvar;
    }

    // Commandline, config, language PARSING.
    void dump_help() const;
    bool paramlong(const mstring & first, const mstring & second);
    bool paramshort(const mstring & first, const mstring & second);
    bool get_config_values();
    void LoadInternalMessages();
    bool LoadExternalMessages(const mstring & language);
    bool LoadLogMessages(const mstring & language);
    bool UnloadExternalMessages(const mstring & language);
    bool UnloadHelp(const mstring & language);
    mstring getMessage(const mstring & nick, const mstring & name);
    mstring getMessage(const mstring & name)
    {
	return getMessageL(nickserv.DEF_Language(), name);
    }
    mstring getMessageL(const mstring & language, const mstring & name);
    mstring getLogMessage(const mstring & name);
    vector < mstring > getHelp(const mstring & nick, const mstring & name);
    vector < mstring > getHelp(const mstring & name)
    {
	return getHelp("", name);
    }

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

    mstring getLname(const mstring & in)
    {
	if (IsChan(in))
	{
	    if (chanserv.IsLive(in))
		return chanserv.GetLive(in)->Name();
	}
	else
	{
	    if (nickserv.IsLiveAll(in))
		return nickserv.GetLive(in)->Name();
	}
	return "";
    }

    mstring getSname(const mstring & in)
    {
	if (IsChan(in))
	{
	    if (chanserv.IsStored(in))
		return chanserv.GetStored(in)->Name();
	}
	else
	{
	    if (nickserv.IsStored(in))
		return nickserv.GetStored(in)->Name();
	}
	return "";
    }

    SXP::Tag & GetClassTag() const
    {
	return tag_Magick;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);

    set < mstring > LNG_Loaded() const;
    size_t LNG_Usage(const mstring & lang) const;
    set < mstring > HLP_Loaded() const;
    size_t HLP_Usage(const mstring & lang) const;
    size_t LFO_Usage() const;
    void DumpB() const;
    void DumpE() const;
};

#endif
