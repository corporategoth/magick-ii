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
#ifndef _IRCSOCKET_H
#define _IRCSOCKET_H
#include "pch.h"
RCSID(ircsocket_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.58  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.57  2001/11/04 23:43:14  prez
** Updates for MS Visual C++ compilation (it works now!).
**
** Revision 1.56  2001/11/03 21:02:50  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.55  2001/06/15 07:20:39  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.54  2001/05/25 01:59:31  prez
** Changed messaging system ...
**
** Revision 1.53  2001/05/04 01:11:13  prez
** Made chanserv mode stuff more efficiant
**
** Revision 1.52  2001/05/03 04:40:17  prez
** Fixed locking mechanism (now use recursive mutexes) ...
** Also now have a deadlock/nonprocessing detection mechanism.
**
** Revision 1.51  2001/05/01 14:00:21  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.50  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.49  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.48  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.47  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.44  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.43  2000/09/27 11:21:37  prez
** Added a BURST mode ...
**
** Revision 1.42  2000/09/09 02:17:47  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.41  2000/09/06 11:27:33  prez
** Finished the T_Modify / T_Changing traces, fixed a bug in clone
** adding (was adding clone liimt as the mask length), updated docos
** a little more, and added a response to SIGINT to signon clients.
**
** Revision 1.40  2000/08/31 06:25:08  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.39  2000/08/28 10:51:35  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.38  2000/08/06 05:27:46  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.37  2000/08/03 13:06:29  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.36  2000/07/29 21:58:52  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.35  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.34  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.33  2000/05/26 11:21:28  prez
** Implemented HTM (High Traffic Mode) -- Can be used at a later date.
**
** Revision 1.32  2000/05/22 13:00:08  prez
** Updated version.h and some other stuff
**
** Revision 1.31  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.30  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "variant.h"

class Reconnect_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout (const ACE_Time_Value &tv, const void *arg);
    mstring Reconnect_Handler::FindNext(const mstring& i_server);
};

class ToBeSquit_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout (const ACE_Time_Value &tv, const void *arg);
};

class Squit_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout (const ACE_Time_Value &tv, const void *arg);
};

class InFlight_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout (const ACE_Time_Value &tv, const void *arg);
};

class Part_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout (const ACE_Time_Value &tv, const void *arg);
};

class EventTask : public ACE_Task<ACE_MT_SYNCH>
{
    map<ACE_thread_t,mDateTime> thread_heartbeat;
    set<mstring> cmodes_pending;
    mDateTime last_expire;
    mDateTime last_save;
    mDateTime last_check;
    mDateTime last_ping;
    mDateTime last_msgcheck;
    mDateTime last_heartbeat;
    static void *save_databases(void *in = NULL);
    void do_expire(mDateTime &synctime);
    void do_check(mDateTime &synctime);
    void do_ping(mDateTime &synctime);
    void do_msgcheck(mDateTime &synctime);
    void do_heartbeat(mDateTime &synctime);
    void do_modes(mDateTime &synctime);

public:
    void RemoveThread(ACE_thread_t thr = ACE_Thread::self());
    void Heartbeat(ACE_thread_t thr = ACE_Thread::self());
    void AddChannelModePending(const mstring &in);

    void ForceSave();
    void ForcePing();
    mstring SyncTime(const mstring& source = "") const;
    int open(void *in=0);
    int close(u_long in = 0);
    int svc(void);
    void DumpB() const;
    void DumpE() const;
};

class mMessage;
class IrcSvcHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>
{
    friend void EventTask::do_heartbeat(mDateTime &synctime);
    typedef ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH> inherited;
    // This takes any characters read from the socket that dont
    // end in \r or \n, and adds them to next read's run.
    mstring flack;

    map<time_t, size_t> traffic;
    size_t in_traffic, out_traffic;
    mDateTime connect_time;
    mDateTime last_htm_check;
    unsigned short htm_level;
    time_t htm_gap;
    size_t htm_threshold;
    mSocket sock;
    bool i_burst;
    mDateTime i_synctime;

    ACE_Thread_Manager tm;
    ACE_Activation_Queue message_queue;

    static void *worker(void *);
public:
    int send(const mstring& data);
    int open(void * = 0);
    int handle_input(ACE_HANDLE handle);
    int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
		ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
    int fini();

    unsigned long Local_IP() const { return sock.Local_IP(); }
    size_t In_Traffic() const { return in_traffic; }
    size_t Out_Traffic() const { return out_traffic; }
    mDateTime Connect_Time() const { return connect_time; }
    time_t HTM_Gap() const;
    unsigned short HTM_Level() const;
    size_t HTM_Threshold() const;
    void HTM_Threshold(const size_t in);
    void HTM(const bool in);
    size_t Average(time_t secs = 0) const;
    bool Burst() const;
    mDateTime SyncTime() const;
    void EndBurst();
    int Threads() { return tm.count_threads(); }

    void enqueue(mMessage *mm);
    void enqueue(const mstring &message, const u_long priority = static_cast<u_long>(P_Normal));
    void enqueue_shutdown();
    void enqueue_sleep(const mstring& time = "1s");
    void enqueue_test();

    void DumpB() const;
    void DumpE() const;
};

typedef ACE_Connector<IrcSvcHandler,ACE_SOCK_CONNECTOR> IrcConnector;

#endif
