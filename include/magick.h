#ifndef WIN32
  #pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _MAGICK_H
#define _MAGICK_H
#include "pch.h"
RCSID(magick_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.167  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.166  2001/11/03 21:02:50  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.165  2001/06/20 06:07:01  prez
** ome GCC 3.0 and solaris fixes
**
** Revision 1.164  2001/06/15 07:20:39  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.163  2001/06/02 16:27:04  prez
** Intergrated the staging system for dbase loading/saving.
**
** Revision 1.162  2001/05/25 01:59:31  prez
** Changed messaging system ...
**
** Revision 1.161  2001/05/17 19:18:53  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.160  2001/05/14 04:46:31  prez
** Changed to use 3BF (3 * blowfish) encryption.  DES removed totally.
**
** Revision 1.159  2001/05/06 03:03:07  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.158  2001/05/05 17:33:57  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.157  2001/05/03 04:40:17  prez
** Fixed locking mechanism (now use recursive mutexes) ...
** Also now have a deadlock/nonprocessing detection mechanism.
**
** Revision 1.156  2001/05/01 14:00:22  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.155  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.154  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.153  2001/03/27 07:04:30  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.152  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.151  2001/03/08 08:07:40  ungod
** fixes for bcc 5.5
**
** Revision 1.150  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.149  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.148  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.143  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.142  2000/12/22 19:50:19  prez
** Made all config options const.  Beginnings of securing all non-modifying
** commands to const.  also added serviceschk.
**
** Revision 1.141  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.140  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.139  2000/09/02 07:20:44  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.138  2000/08/10 22:44:22  prez
** Added 'binding to IP' options for shell servers, etc.  Also added akick
** triggers for when a user changes their nick and suddenly matches akick.
**
** Revision 1.137  2000/07/29 21:58:52  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.136  2000/07/28 14:49:34  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.135  2000/06/28 12:20:47  prez
** Lots of encryption stuff, but essentially, we now have random
** key generation for the keyfile keys, and we can actually encrypt
** something, and get it back as we sent it in (specifically, the
** keyfile itself).
**
** Revision 1.134  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.133  2000/06/12 08:15:36  prez
** Added 'minimum threads' option to config (set to 2)
**
** Revision 1.132  2000/06/12 06:07:49  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.131  2000/06/08 13:07:33  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.130  2000/06/06 08:57:54  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.129  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.128  2000/05/26 11:21:28  prez
** Implemented HTM (High Traffic Mode) -- Can be used at a later date.
**
** Revision 1.127  2000/05/21 04:49:38  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.126  2000/05/20 17:00:18  prez
** Added in the VERBOSE mode, now we mimic old logging
**
** Revision 1.125  2000/05/20 15:16:59  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.124  2000/05/20 03:28:10  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.123  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.122  2000/05/14 04:02:52  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.121  2000/05/13 15:06:42  ungod
** no message
**
** Revision 1.120  2000/05/03 14:12:22  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.119  2000/04/18 10:20:26  prez
** Made helpfiles load on usage, like language files.
**
** Revision 1.118  2000/04/16 14:29:43  prez
** Added stats for usage, and standardized grabbing paths, etc.
**
** Revision 1.117  2000/04/16 06:12:13  prez
** Started adding body to the documentation...
**
** Revision 1.116  2000/04/06 12:44:09  prez
** Removed SXP and EXPAT directories
**
** Revision 1.115  2000/04/03 09:45:21  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.114  2000/03/24 15:35:17  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.113  2000/03/23 10:22:24  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.112  2000/03/19 08:50:53  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.111  2000/03/15 14:42:58  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.110  2000/02/27 02:43:50  prez
** More FileSystem additions, plus created 'what' tool
**
** Revision 1.109  2000/02/23 14:29:05  prez
** Added beginnings of a File Map for stored files.
** Also updated Help files (finished nickserv).
**
** Revision 1.108  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.107  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

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

const int MAGICK_RET_NORMAL		    = 0;
const int MAGICK_RET_RESTART		    = 1;
const int MAGICK_RET_ERROR		    = -1;
const int MAGICK_RET_LOCKED		    = -2;

class Magick; // fwd reference, leave it here
const mstring ChanSpec = "#&+!";

extern Magick *Parent;
extern mDateTime StartTime;

inline bool IsChan(mstring input)
{ return (ChanSpec.Contains(input[0U])); }

class SignalHandler : public ACE_Event_Handler
{
public:
    int handle_signal(int signum, siginfo_t *siginfo, ucontext_t *ucontext);
};


#define LOG2(X)	\
	if (Parent != NULL && Parent->ValidateLogger(ACE_LOG_MSG)) { \
		ACE_DEBUG(X); Parent->EndLogMessage(ACE_LOG_MSG); }
#define LOG(X, Y, Z) \
	{ LOG2((X, parseMessage(Parent->getLogMessage(Y), mVarArray Z))); }
#define NLOG(X, Y) \
	{ LOG2((X, parseMessage(Parent->getLogMessage(Y)))); }
#define SLOG(X, Y, Z) \
	{ LOG2((X, parseMessage(Y, mVarArray Z))); }
#define NSLOG(X, Y) \
	{ LOG2((X, parseMessage(Y))); }

class Logger : public ACE_Log_Msg_Callback
{
    mFile fout;

public:
    Logger();
    ~Logger();

    void log(ACE_Log_Record &log_record);
    void close();
    void open();
    bool opened() const;
};


class Magick : public SXP::IPersistObj
{
    friend class Reconnect_Handler;
private:
    vector<mstring> argv;
    // Language, token, string
    map<mstring, map<mstring, mstring> > Messages;
    // Language, token, vector<yescom, nocom, string>
    map<mstring, map<mstring, vector<triplet<mstring, mstring, mstring> > > > Help;
    // Token, string
    map<mstring, mstring> LogMessages;
    int doparamparse();
    SignalHandler *signalhandler;
    map<pair<mstring,mstring>,vector<mstring> > handlermap;

    Logger *logger;
    set<ACE_Log_Msg *> LogInstances;
    bool i_verbose;

    mstring i_services_dir;
    mstring i_config_file;
    mstring i_programname;

    mDateTime i_ResetTime;
    unsigned int i_level;
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
    EventTask *events;
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

    // Config Values
    class startup_t {
	friend class Magick;

	// map<server name, pair<priority, triplet<port, password, numeric> > >
	map<mstring,pair<unsigned int, triplet<unsigned int,mstring,unsigned long> > > servers;
	// map<server name, vector<allowed uplinks> >
	map<mstring,vector<mstring> > allows;
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
	bool IsServer(const mstring& server)const;
	pair<unsigned int, triplet<unsigned int,mstring,unsigned long> > Server(const mstring& server)const;
	vector<mstring> PriorityList(const unsigned int pri)const;
	size_t Server_size()const { return servers.size(); }

	bool IsAllowed(const mstring& server, const mstring& uplink)const;
	vector<mstring> Allow(const mstring& server)const;
	vector<mstring> AllowList()const;
	size_t Allow_size()const { return allows.size(); }

	mstring Server_Name()const    { return server_name; }
	mstring Server_Desc()const    { return server_desc; }
	mstring Services_User()const    { return services_user; }
	mstring Services_Host()const    { return services_host; }
	mstring Services_Quitmsg()const    { return services_quitmsg; }
	bool Ownuser()const	{ return ownuser; }
	mstring Setmode()const	{ return setmode; }
	mstring Bind()const	{ return bind; }
	unsigned int Level()const    { return level; }
	unsigned long Lagtime()const    { return lagtime; }
    } startup;

    class files_t {
	friend class Magick;

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
	mstring MakePath(const mstring& in)const
	    {
#ifdef WIN32
		if (in[1u] == ':' && mstring(in[2u]) == DirSlash)
		    return in;
		else
		    return Parent->Services_Dir() + DirSlash + in;
#else
		if (mstring(in[0u]) == DirSlash)
		    return in;
		else
		    return Parent->Services_Dir() + DirSlash + in;
#endif
	    }
	mstring Pidfile()const	    { return MakePath(pidfile); }
	mstring Logfile()const	    { return MakePath(logfile); }
	mstring Logchan()const	    { return logchan; }
	mstring Motdfile()const	    { return MakePath(motdfile); }
	mstring Langdir()const	    { return MakePath(langdir); }
	mstring Database()const	    { return MakePath(database); }
	unsigned int Compression()const	{ return compression; }
	mstring KeyFile()const	    { return MakePath(keyfile); }
	bool Encryption()const	    { return encryption; }
	mstring MemoAttach()const	{ return MakePath(memoattach); }
	unsigned long MemoAttachSize()const { return memoattachsize; }
	mstring Picture()const	    { return MakePath(picture); }
	unsigned long PictureSize()const    { return picturesize; }
	mstring Public()const	    { return MakePath(i_public); }
	unsigned long PublicSize()const	{ return publicsize; }
	mstring TempDir()const	    { return MakePath(tempdir); }
	unsigned long TempDirSize()const    { return tempdirsize; }
	unsigned long Blocksize()const	{ return blocksize; }
	unsigned long Timeout()const	{ return timeout; }
	unsigned long Min_Speed()const	{ return min_speed; }
	unsigned long Max_Speed()const	{ return max_speed; }
	unsigned long Sampletime()const	{ return sampletime; }
    } files;

    class config_t {
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
	unsigned int low_water_mark;
	unsigned int high_water_mark;
	unsigned long heartbeat_time;
	unsigned long msg_seen_time;
	unsigned long msg_check_time;
    public:
	unsigned long Server_Relink()const    { return server_relink; }
	unsigned long Squit_Protect()const    { return squit_protect; }
	unsigned long Squit_Cancel()const    { return squit_cancel; }
	unsigned long Cycletime()const	{ return cycletime; }
	unsigned long Savetime()const	{ return savetime; }
	unsigned long Checktime()const	{ return checktime; }
	unsigned long Ping_Frequency()const    { return ping_frequency; }
	unsigned int Starthresh()const	{ return starthresh; }
	unsigned int Listsize()const	{ return listsize; }
	unsigned int Maxlist()const	{ return maxlist; }
	unsigned int Min_Threads()const	{ return min_threads; }
	unsigned int Low_Water_Mark()const    { return low_water_mark; }
	unsigned int High_Water_Mark()const    { return high_water_mark; }
	unsigned long Heartbeat_Time()const	{ return heartbeat_time; }
	unsigned long MSG_Seen_Time()const    { return msg_seen_time; }
	unsigned long MSG_Check_Time()const    { return msg_check_time; }
    } config;

    bool ActivateLogger();
    void DeactivateLogger();
    bool ValidateLogger(ACE_Log_Msg *instance) const;
    void EndLogMessage(ACE_Log_Msg *instance) const;
    bool Verbose()const	{ return i_verbose; }
    mstring Services_Dir()const    { return i_services_dir; }
    mstring Config_File()const    { return files.MakePath(i_config_file); }
    mstring ProgramName()const    { return i_programname; }

    // Current STATES, and switching between them.
    Magick(int inargc, char **inargv);
    ~Magick() {}

    int Start(bool firstrun = true);
    mDateTime ResetTime()const    { return i_ResetTime; }
    unsigned int Level()const    { return i_level; }
    void LevelUp()
    {
	i_level++;
    }
    void LevelDown()
    {
	if (i_level > startup.Level())
	i_level--;
    }
    void AUTO(const bool on)    { i_auto = on; }
    bool AUTO()const	{ return i_auto; }
    void MSG(const bool on)
    {
        operserv.MSG(on);
	nickserv.MSG(on);
	chanserv.MSG(on);
	memoserv.MSG(on);
	servmsg.MSG(on);
	commserv.MSG(on);
    }
    void Die()	    { ACE_Reactor::instance()->end_event_loop(); }
    void Shutdown(const bool in)    { i_shutdown = in; }
    bool Shutdown()const	{ return i_shutdown; }

    // Streams, etc
    bool Reconnect()const	{ return i_reconnect; }
    bool GotConnect()const	{ return i_gotconnect; }
    void GotConnect(bool in)	{ i_gotconnect = in; }
    unsigned long LocalHost()const    { return i_localhost; }
    mstring CurrentServer()const    { return i_currentserver; }
    bool Connected()const	{ return i_connected; }
    void Connected(bool in)	{ i_connected = in; }
    bool Saving()const	{ return i_saving; }
    void Disconnect(const bool reconnect=true);
    void send(const mstring& text)const;
    pair<mstring,mstring> GetKeys()const;
    void save_databases();
    void load_databases();
    Reconnect_Handler rh;

    operator mVariant() const
	{
	    mVariant locvar("Magick");
	    locvar.truevaluetype="Magick";
	    return locvar;
	}

    // Commandline, config, language PARSING.
    void dump_help() const;
    bool paramlong(const mstring& first, const mstring& second);
    bool paramshort(const mstring& first, const mstring& second);
    bool get_config_values();
    void LoadInternalMessages();
    bool LoadExternalMessages(const mstring& language);
    bool LoadLogMessages(const mstring& language);
    bool UnloadExternalMessages(const mstring& language);
    bool UnloadHelp(const mstring& language);
    mstring getMessage(const mstring& nick, const mstring& name);
    mstring getMessage(const mstring& name)
	{ return getMessageL(nickserv.DEF_Language(), name); }
    mstring getMessageL(const mstring& language, const mstring& name);
    mstring getLogMessage(const mstring& name);
    vector<mstring> getHelp(const mstring& nick, const mstring& name);
    vector<mstring> getHelp(const mstring& name)
	{ return getHelp("", name); }

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

    mstring getLname(const mstring& in)
    {
	if (IsChan(in))
	{
	    if (chanserv.IsLive(in))
		return chanserv.GetLive(in).Name();
	}
	else
	{
	    if (nickserv.IsLiveAll(in))
		return nickserv.GetLive(in).Name();
	}
	return "";
    }

    mstring getSname(const mstring& in)
    {
	if (IsChan(in))
	{
	    if (chanserv.IsStored(in))
		return chanserv.GetStored(in).Name();
	}
	else
	{
	    if (nickserv.IsStored(in))
		return nickserv.GetStored(in).Name();
	}
	return "";
    }

    SXP::Tag& GetClassTag() const { return tag_Magick; }
    void BeginElement(const SXP::IParser * pIn, const SXP::IElement * pElement);
    void EndElement(const SXP::IParser * pIn, const SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs = SXP::blank_dict);

    set<mstring> LNG_Loaded() const;
    size_t LNG_Usage(const mstring& lang) const;
    set<mstring> HLP_Loaded() const;
    size_t HLP_Usage(const mstring& lang) const;
    size_t LFO_Usage() const;
    void DumpB() const;
    void DumpE() const;
};

#endif
