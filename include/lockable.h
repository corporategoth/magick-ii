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
#ifndef _LOCKABLE_H
#define _LOCKABLE_H
#include "pch.h"
RCSID(lockable_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.66  2001/11/28 13:40:47  prez
** Added UMASK option to config.  Also made the 'dead thread' protection
** send a SIGIOT signal to try and get the thread to die gracefully, else
** it will do the cancel it used to do.
**
** Revision 1.65  2001/11/22 17:32:18  prez
** Some fixes to lockable for mpatrol, and mstring overwriting its own memory.
**
** Revision 1.64  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.63  2001/07/01 05:02:46  prez
** Added changes to dependancy system so it wouldnt just remove a dependancy
** after the first one was satisfied.
**
** Revision 1.62  2001/05/13 00:55:17  prez
** More patches to try and fix deadlocking ...
**
** Revision 1.61  2001/05/03 04:40:17  prez
** Fixed locking mechanism (now use recursive mutexes) ...
** Also now have a deadlock/nonprocessing detection mechanism.
**
** Revision 1.60  2001/05/02 02:35:26  prez
** Fixed dependancy system, and removed printf's - we no longer coredump on
** a 1000 user network.  As a bonus, we actually synd perfectly ;P
**
** Revision 1.59  2001/05/01 14:00:22  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.58  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.57  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.56  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.55  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.54  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.53  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.49  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.48  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.47  2000/10/18 18:46:33  prez
** Well, mstring still coredumps, but it gets past the initial loading of
** all the STATIC (or const) strings, etc -- now its coring on loading a
** file (or possibly language.h or something).  Still investigating.
**
** Revision 1.46  2000/10/14 04:25:31  prez
** Added mmemory.h -- MemCluster and the MemoryManager are now in it.
** TODO - make mstring use MemoryManager.
**
** Revision 1.45  2000/10/07 11:01:13  prez
** Took out placement new's from lockable.cpp, now using derived classes.
**
** Revision 1.44  2000/10/04 10:52:07  prez
** Fixed the memory pool and removed printf's.
**
** Revision 1.43  2000/10/04 07:39:45  prez
** Added MemCluster to speed up lockable, but it cores when we start
** getting real messages -- seemingly in an alloc in the events.
** Lots of printf's left in for debugging (so run as ./magick >output)
**
** Revision 1.42  2000/09/07 08:13:17  prez
** Fixed some of the erronous messages (SVSHOST, SQLINE, etc).
** Also added CPU statistics and fixed problem with socket deletions.
**
** Revision 1.41  2000/09/01 10:54:38  prez
** Added Changing and implemented Modify tracing, now just need to create
** DumpB() and DumpE() functions in all classes, and put MCB() / MCE() calls
** (or MB() / ME() or CB() / CE() where MCB() / MCE() not appropriate) in.
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
** Revision 1.38  2000/08/22 08:43:39  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.37  2000/08/19 10:59:46  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.36  2000/08/06 05:27:46  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.35  2000/07/30 09:04:04  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.34  2000/06/13 14:11:53  prez
** Locking system has been re-written, it doent core anymore.
** So I have set 'MAGICK_LOCKS_WORK' define active :)
**
** Revision 1.33  2000/06/06 08:57:54  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.32  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.31  2000/05/20 03:28:10  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.30  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.29  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "trace.h"

#ifdef MAGICK_LOCKS_WORK

#define MAX_LOCKS 16 /* Max variants */
#define LOCK_SEGMENT 8 /* Amount of lock memory to alloc */
class mLOCK
{
    friend class mLock_Read;
    friend class mLock_Write;
    friend class mLock_Mutex;

    static mLock_Mutex *maplock;
    static map<mstring, pair<void *, map<ACE_thread_t, locktype_enum> > > LockMap;
    static ACE_Expandable_Cached_Fixed_Allocator<ACE_Thread_Mutex> memory_area;

    bool islocked;
    vector<mstring> locks;
#ifdef MAGICK_TRACE_WORKS
    T_Locking tlock[MAX_LOCKS];
#endif

    static bool AcquireMapLock();
    static bool ReleaseMapLock();

public:
    mLOCK() {}
    mLOCK(const locktype_enum type, const mVarArray &args);
    ~mLOCK();
    bool Locked() const { return islocked; }
    size_t Locks() const { return locks.size(); }
    static size_t AllLocks() { return LockMap.size(); }
    static list<pair<void *, locktype_enum> > mLOCK::GetLocks(ACE_thread_t thr = ACE_Thread::self());
};

/* We need to ditch these for the below operator new */
#if defined(HAVE_MPATROL_H) && defined(MAGICK_USE_MPATROL)
#undef new
#undef delete
#endif
#undef malloc
#undef free

class mLock_Read : public ACE_RW_Thread_Mutex
{
	typedef ACE_RW_Thread_Mutex base;
public:
	mLock_Read (const char *name = 0)
		: base(name) {}

	int acquire()		{ return acquire_read(); }
	int tryacquire()		{ return tryacquire_read(); }

	void *operator new (size_t size)
		{ static_cast<void>(size);
		return mLOCK::memory_area.malloc(sizeof(mLock_Read)); }
	void operator delete (void *ptr)
		{ mLOCK::memory_area.free(ptr); }
};

class mLock_Write : public ACE_RW_Thread_Mutex
{
	typedef ACE_RW_Thread_Mutex base;
public:
	mLock_Write (const char *name = 0)
		: base(name) {}

	int acquire()		{ return acquire_write(); }
	int tryacquire()		{ return tryacquire_write(); }

	void *operator new (size_t size)
		{ static_cast<void>(size);
		return mLOCK::memory_area.malloc(sizeof(mLock_Write)); }
	void operator delete (void *ptr)
		{ mLOCK::memory_area.free(ptr); }
};

class mLock_Mutex : public ACE_Recursive_Thread_Mutex
{
	typedef ACE_Recursive_Thread_Mutex base;
public:
	mLock_Mutex (const char *name = 0)
		: base(name) {}

	void *operator new (size_t size)
		{ static_cast<void>(size);
		return mLOCK::memory_area.malloc(sizeof(mLock_Mutex)); }
	void operator delete (void *ptr)
		{ mLOCK::memory_area.free(ptr); }
};

/* I hate having to do this ... but *shrug* */
#if defined(HAVE_MPATROL_H) && defined(MAGICK_USE_MPATROL)
#define new ::new(MP_FUNCNAME, __FILE__, __LINE__)
#define delete __mp_pushdelstack(MP_FUNCNAME, __FILE__, __LINE__), ::delete
#define malloc(l) \
    __mp_alloc((l), 0, MP_AT_MALLOC, MP_FUNCNAME, __FILE__, __LINE__, NULL, 0, \
               0)
#define free(p) \
    __mp_free((p), MP_AT_FREE, MP_FUNCNAME, __FILE__, __LINE__, 0)
#else
#    define malloc	ACE_OS::malloc
#    define free	ACE_OS::free
#endif

#define RLOCK(y)   mVarArray __lockR1_VarArray y; mLOCK __lockR1(L_Read,  __lockR1_VarArray)
#define RLOCK2(y)  mVarArray __lockR2_VarArray y; mLOCK __lockR2(L_Read,  __lockR2_VarArray)
#define RLOCK3(y)  mVarArray __lockR3_VarArray y; mLOCK __lockR3(L_Read,  __lockR3_VarArray)
#define RLOCK4(y)  mVarArray __lockR4_VarArray y; mLOCK __lockR4(L_Read,  __lockR4_VarArray)
#define RLOCK5(y)  mVarArray __lockR5_VarArray y; mLOCK __lockR5(L_Read,  __lockR5_VarArray)
#define RLOCK6(y)  mVarArray __lockR6_VarArray y; mLOCK __lockR6(L_Read,  __lockR6_VarArray)
#define RLOCK7(y)  mVarArray __lockR7_VarArray y; mLOCK __lockR7(L_Read,  __lockR7_VarArray)
#define RLOCK8(y)  mVarArray __lockR8_VarArray y; mLOCK __lockR8(L_Read,  __lockR8_VarArray)
#define WLOCK(y)   mVarArray __lockW1_VarArray y; mLOCK __lockW1(L_Write, __lockW1_VarArray)
#define WLOCK2(y)  mVarArray __lockW2_VarArray y; mLOCK __lockW2(L_Write, __lockW2_VarArray)
#define WLOCK3(y)  mVarArray __lockW3_VarArray y; mLOCK __lockW3(L_Write, __lockW3_VarArray)
#define WLOCK4(y)  mVarArray __lockW4_VarArray y; mLOCK __lockW4(L_Write, __lockW4_VarArray)
#define WLOCK5(y)  mVarArray __lockW5_VarArray y; mLOCK __lockW5(L_Write, __lockW5_VarArray)
#define WLOCK6(y)  mVarArray __lockW6_VarArray y; mLOCK __lockW6(L_Write, __lockW6_VarArray)
#define WLOCK7(y)  mVarArray __lockW7_VarArray y; mLOCK __lockW7(L_Write, __lockW7_VarArray)
#define WLOCK8(y)  mVarArray __lockW8_VarArray y; mLOCK __lockW8(L_Write, __lockW8_VarArray)
#define MLOCK(y)   mVarArray __lockM1_VarArray y; mLOCK __lockM1(L_Mutex, __lockM1_VarArray)
#define MLOCK2(y)  mVarArray __lockM2_VarArray y; mLOCK __lockM2(L_Mutex, __lockM2_VarArray)
#define MLOCK3(y)  mVarArray __lockM3_VarArray y; mLOCK __lockM3(L_Mutex, __lockM3_VarArray)
#define MLOCK4(y)  mVarArray __lockM4_VarArray y; mLOCK __lockM4(L_Mutex, __lockM4_VarArray)
#define MLOCK5(y)  mVarArray __lockM5_VarArray y; mLOCK __lockM5(L_Mutex, __lockM5_VarArray)
#define MLOCK6(y)  mVarArray __lockM6_VarArray y; mLOCK __lockM6(L_Mutex, __lockM6_VarArray)
#define MLOCK7(y)  mVarArray __lockM7_VarArray y; mLOCK __lockM7(L_Mutex, __lockM7_VarArray)
#define MLOCK8(y)  mVarArray __lockM8_VarArray y; mLOCK __lockM8(L_Mutex, __lockM8_VarArray)

#define RLOCK_IF(x, y) \
	bool __if_res = false; \
	{ RLOCK(x); __if_res = y; } \
	if (__if_res)
#define RLOCK2_IF(x, y) \
	__if_res = false; \
	{ RLOCK(x); __if_res = y; } \
	if (__if_res)
#define WLOCK_IF(x, y) \
	bool __if_res = false; \
	{ WLOCK(x); __if_res = y; } \
	if (__if_res)
#define WLOCK2_IF(x, y) \
	__if_res = false; \
	{ WLOCK(x); __if_res = y; } \
	if (__if_res)
#define MLOCK_IF(x, y) \
	bool __if_res = false; \
	{ MLOCK(x); __if_res = y; } \
	if (__if_res)
#define MLOCK2_IF(x, y) \
	__if_res = false; \
	{ MLOCK(x); __if_res = y; } \
	if (__if_res)


#else /* MAGICK_LOCKS_WORK */
#define RLOCK(y)
#define RLOCK2(y)
#define RLOCK3(y)
#define RLOCK4(y)
#define RLOCK5(y)
#define RLOCK6(y)
#define RLOCK7(y)
#define RLOCK8(y)
#define WLOCK(y)
#define WLOCK2(y)
#define WLOCK3(y)
#define WLOCK4(y)
#define WLOCK5(y)
#define WLOCK6(y)
#define WLOCK7(y)
#define WLOCK8(y)
#define MLOCK(y)
#define MLOCK2(y)
#define MLOCK3(y)
#define MLOCK4(y)
#define MLOCK5(y)
#define MLOCK6(y)
#define MLOCK7(y)
#define MLOCK8(y)

#define RLOCK_IF(x, y) if (y)
#define RLOCK2_IF(x, y) if (y)
#define WLOCK_IF(x, y) if (y)
#define WLOCK2_IF(x, y) if (y)
#define MLOCK_IF(x, y) if (y)
#define MLOCK2_IF(x, y) if (y)

#endif /* MAGICK_LOCKS_WORK */

class mSocket
{
    unsigned long sockid;
    mutable ACE_SOCK_Stream *sock;
    ACE_INET_Addr local, remote;
    int last_error;
    bool DestroyMe;

#ifdef MAGICK_TRACE_WORKS
    T_Sockets trace;
#endif

    void init();
public:
    static map<unsigned long, mSocket *> SockMap;
    static unsigned short FindAvailPort();

    mSocket()
    { init(); }
    mSocket(const ACE_INET_Addr &addr, const unsigned long timeout)
    {
	init();
	Connect(addr, timeout);
    }
    mSocket(const mstring& host, const unsigned short port, const unsigned long timeout)
    {
	init();
	Connect(host, port, timeout);
    }
    mSocket(const unsigned short port, const unsigned long timeout)
    {
	init();
	Accept(port, timeout);
    }
    mSocket(ACE_SOCK_Stream *in, const dir_enum direction, const bool alloc)
    {
	init();
	Bind(in, direction, alloc);
    }

    mSocket(const mSocket &in) { *this = in; }
    ~mSocket();
    void operator=(const mSocket &in);

    bool Connect(const ACE_INET_Addr &addr, const unsigned long timeout = 0);
    bool Connect(const unsigned long host, const unsigned short port, const unsigned long timeout = 0);
    bool Connect(const mstring &host, const unsigned short port, const unsigned long timeout = 0);
    bool Accept(const unsigned short port, const unsigned long timeout = 0);
    bool Bind(ACE_SOCK_Stream *in, const dir_enum direction = D_Unknown, const bool alloc = true);
    ACE_SOCK_Stream *Unbind();

    mstring Local_Host() const;
    unsigned long Local_IP() const;
    unsigned short Local_Port() const;
    mstring Remote_Host() const;
    unsigned long Remote_IP() const;
    unsigned short Remote_Port() const;

    bool IsConnected() const;
    void Resolve(const socktype_enum type, const mstring &info);
    int Last_Error() const;
    mstring Last_Error_String() const;

    ssize_t send(void *buf, const size_t len, const unsigned long timeout = 0);
    ssize_t recv(void *buf, const size_t len, const unsigned long timeout = 0);
    int close();
};

class mThread
{
private:
/** Custom thread class (inherited ACE_Thread) that is constructed as
	new thread(function, veriable, ThreadID::type_enum);
	It should incorporate ThreadID association in the threads class, so it can
	have 2 static find() functions (ACE_Thread *T, and (type_enum type, int level)). 
	Also should have 2 static members, a ids[tt_MAX], and vector<ACE_Thread,ThreadID *> 
	for 1) storing how many of each thread type there are (And choosing a new 'Thread ID' 
	for each type of thread), and 2) for use with the find() functions.
	This should help with the Tracing needing to know the ThreadID to be able
	to know 1) if tracing is turned on for that type, and 2) do the write out.
*/
    // really should be using auto_ptr's here, *shrug* maybe later
    typedef map<ACE_thread_t,ThreadID*> selftothreadidmap_t;
    static selftothreadidmap_t selftothreadidmap;
    static mLock_Mutex *maplock;

    static bool AcquireMapLock();
    static bool ReleaseMapLock();

public:
    static ThreadID* find(const ACE_thread_t thread=ACE_Thread::self());
    static vector<ThreadID*> findall();
    static size_t size() { return selftothreadidmap.size(); }
    static void Attach(const threadtype_enum ttype);
    static void Detach();
    static void ReAttach(const threadtype_enum ttype);
};

#endif
