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
#ifndef _IRCSOCKET_H
#define _IRCSOCKET_H
#include "pch.h"
RCSID(ircsocket_h, "@(#) $Id$");

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

#include "variant.h"

class Heartbeat_Handler : public ACE_Event_Handler
{
public:
    enum heartbeat_enum
    { H_Invalid = 0, H_Worker, H_Main,
	H_IrcServer, H_Events, H_DCC, H_MAX
    };
    static const char *names[H_MAX];

private:
    typedef map < ACE_thread_t, triplet < heartbeat_enum, mDateTime, bool > > threads_t;
    threads_t threads;

public:
    int handle_timeout(const ACE_Time_Value & tv, const void *arg);

    void AddThread(heartbeat_enum type, ACE_thread_t id = ACE_Thread::self());
    void RemoveThread(ACE_thread_t id = ACE_Thread::self());
    void Heartbeat(ACE_thread_t id = ACE_Thread::self());
    heartbeat_enum ThreadType(ACE_thread_t id = ACE_Thread::self());

    size_t size();
    size_t count(heartbeat_enum type);
};

class Reconnect_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout(const ACE_Time_Value & tv, const void *arg);
    mstring Reconnect_Handler::FindNext(const mstring & i_server);
};

class Disconnect_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout(const ACE_Time_Value & tv, const void *arg);
};

class ToBeSquit_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout(const ACE_Time_Value & tv, const void *arg);
};

class Squit_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout(const ACE_Time_Value & tv, const void *arg);
};

class InFlight_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout(const ACE_Time_Value & tv, const void *arg);
};

class Part_Handler : public ACE_Event_Handler
{
public:
    int handle_timeout(const ACE_Time_Value & tv, const void *arg);
};

class EventTask : public ACE_Task < ACE_MT_SYNCH >
{
    typedef ACE_Task < ACE_MT_SYNCH > internal;

    set < mstring > cmodes_pending;
    Magick *magick_instance;
    mDateTime last_expire;
    mDateTime last_save;
    mDateTime last_check;
    mDateTime last_ping;
    mDateTime last_msgcheck;
    static void *save_databases(void *in = NULL);
    void do_expire(mDateTime & synctime);
    void do_check(mDateTime & synctime);
    void do_ping(mDateTime & synctime);
    void do_msgcheck(mDateTime & synctime);
    void do_modes(mDateTime & synctime);

public:
    EventTask(ACE_Thread_Manager * tm = 0) : internal(tm)
    {
    }
    void AddChannelModePending(const mstring & in);

    void ForceSave();
    void ForcePing();
    mstring SyncTime(const mstring & source = "") const;
    int open(void *in = 0);
    int close(unsigned long in = 0);
    int svc(void);
    int fini()
    {
	return 0;
    }
    void DumpB() const;
    void DumpE() const;
};

class mMessage;
class IrcSvcHandler : public ACE_Svc_Handler < ACE_SOCK_STREAM, ACE_MT_SYNCH >
{
    friend int Heartbeat_Handler::handle_timeout(const ACE_Time_Value & tv, const void *arg);
    friend class SignalHandler;

    typedef ACE_Svc_Handler < ACE_SOCK_STREAM, ACE_MT_SYNCH > inherited;

    // This takes any characters read from the socket that dont
    // end in \r or \n, and adds them to next read's run.
    mstring flack;

    map < time_t, size_t > traffic;
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
    int send(const mstring & data);
    int open(void * = 0);
    int handle_input(ACE_HANDLE handle);
    int handle_input(const char *data);
    int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
    int fini();

    unsigned long Local_IP() const
    {
	return sock.Local_IP();
    }
    size_t In_Traffic() const
    {
	return in_traffic;
    }
    size_t Out_Traffic() const
    {
	return out_traffic;
    }
    mDateTime Connect_Time() const
    {
	return connect_time;
    }
    time_t HTM_Gap() const;
    unsigned short HTM_Level() const;
    size_t HTM_Threshold() const;
    void HTM_Threshold(const size_t in);
    void HTM(const bool in);
    size_t Average(time_t secs = 0) const;
    bool Burst() const;
    float BurstTime() const;
    mDateTime SyncTime() const;
    void EndBurst();
    int Threads()
    {
	return tm.count_threads();
    }

    void enqueue(mMessage * mm);
    void enqueue(const mstring & message, const u_long priority = static_cast < u_long > (P_Normal));
    void enqueue_shutdown();
    void enqueue_sleep(const mstring & time = "1s");
    void enqueue_test();

    void DumpB() const;
    void DumpE() const;
};

typedef ACE_Connector < IrcSvcHandler, ACE_SOCK_CONNECTOR > IrcConnector;

#endif
