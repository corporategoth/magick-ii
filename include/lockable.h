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
#ifndef _LOCKABLE_H
#define _LOCKABLE_H
#include "pch.h"
RCSID(lockable_h, "@(#) $Id$");

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

#include "trace.h"

#ifdef MAGICK_LOCKS_WORK

#define MAX_LOCKS 16		/* Max variants */
#define LOCK_SEGMENT 8		/* Amount of lock memory to alloc */
#ifdef ACE_HAS_TOKENS_LIBRARY
#define
#endif

class mLock_Read;
class mLock_Write;
class mLock_Mutex;

class Lock_Tokenizer
{
    friend class mLock_Read;
    friend class mLock_Write;
    friend class mLock_Mutex;

    static ACE_Thread_Mutex *maplock;
    static map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > > LockMap;
    static ACE_Expandable_Cached_Fixed_Allocator < ACE_Thread_Mutex > memory_area;

#ifdef MAGICK_HAS_EXCEPTIONS
    static bool AcquireMapLock() throw(E_Lock);
    static bool ReleaseMapLock() throw(E_Lock);
#else
    static bool AcquireMapLock();
    static bool ReleaseMapLock();
#endif

public:
    // Will return NULL if map entry exists, unless recursive is set to true
#ifdef MAGICK_HAS_EXCEPTIONS
    static mLock_Read *getReadLock(const mstring & lockname, bool recursive = false) throw(E_Thread);
    static mLock_Write *getWriteLock(const mstring & lockname, bool recursive = false) throw(E_Thread);
    static mLock_Mutex *getMutexLock(const mstring & lockname, bool recursive = false) throw(E_Thread);
#else
    static mLock_Read *getReadLock(const mstring & lockname, bool recursive = false);
    static mLock_Write *getWriteLock(const mstring & lockname, bool recursive = false);
    static mLock_Mutex *getMutexLock(const mstring & lockname, bool recursive = false);
#endif
    static void releaseLock(const mstring & lockname);
    static locktype_enum getType(const mstring & lockname);

    static size_t AllLocks()
    {
	return LockMap.size();
    }
    static list < pair < void *, locktype_enum > > GetLocks(ACE_thread_t thr = ACE_Thread::self());
};

class mLOCK
{
    bool islocked;

    vector < mstring > locks;
#ifdef MAGICK_TRACE_WORKS
    T_Locking tlock[MAX_LOCKS];
#endif

public:
    mLOCK() : islocked(false)
    {
    }

#ifdef MAGICK_HAS_EXCEPTIONS
    mLOCK(const locktype_enum type, const mVarArray & args) throw(E_Lock);

    ~mLOCK() throw(E_Lock);
#else
    mLOCK(const locktype_enum type, const mVarArray & args);

    ~mLOCK();
#endif

    bool Locked() const
    {
	return islocked;
    }
    size_t Locks() const
    {
	return locks.size();
    }
};

/* We need to ditch these for the below operator new */
#if defined(HAVE_MPATROL_H) && defined(MAGICK_USE_MPATROL)
#undef new
#undef delete
#endif
#undef malloc
#undef free

#ifdef MAGICK_TOKENIZED_LOCKS
class mLock_Read : public ACE_Local_RLock
#else
class mLock_Read : public ACE_RW_Thread_Mutex
#endif
{
#ifdef MAGICK_TOKENIZED_LOCKS
    typedef ACE_Local_RLock base;
#else
    typedef ACE_RW_Thread_Mutex base;
#endif

public:
    mLock_Read(const char *name = 0) : base(name)
    {
    }

#ifndef MAGICK_TOKENIZED_LOCKS
    int acquire()
    {
	return acquire_read();
    }
    int tryacquire()
    {
	return tryacquire_read();
    }
#endif

    void *operator        new(size_t size)
    {
	static_cast < void > (size);

	return Lock_Tokenizer::memory_area.malloc(sizeof(mLock_Read));
    }
    void operator        delete(void *ptr)
    {
	Lock_Tokenizer::memory_area.free(ptr);
    }
};

#ifdef MAGICK_TOKENIZED_LOCKS
class mLock_Write : public ACE_Local_WLock
#else
class mLock_Write : public ACE_RW_Thread_Mutex
#endif
{
#ifdef MAGICK_TOKENIZED_LOCKS
    typedef ACE_Local_WLock base;
#else
    typedef ACE_RW_Thread_Mutex base;
#endif

public:
    mLock_Write(const char *name = 0) : base(name)
    {
    }

#ifndef MAGICK_TOKENIZED_LOCKS
    int acquire()
    {
	return acquire_write();
    }
    int tryacquire()
    {
	return tryacquire_write();
    }
#endif

    void *operator        new(size_t size)
    {
	static_cast < void > (size);

	return Lock_Tokenizer::memory_area.malloc(sizeof(mLock_Write));
    }
    void operator        delete(void *ptr)
    {
	Lock_Tokenizer::memory_area.free(ptr);
    }
};

#ifdef MAGICK_TOKENIZED_LOCKS
class mLock_Mutex : public ACE_Local_Mutex
#else
class mLock_Mutex : public ACE_Recursive_Thread_Mutex
#endif
{
#ifdef MAGICK_TOKENIZED_LOCKS
    typedef ACE_Local_Mutex base;
#else
    typedef ACE_Recursive_Thread_Mutex base;
#endif

public:
    mLock_Mutex(const char *name = 0) : base(name)
    {
    }

    void *operator        new(size_t size)
    {
	static_cast < void > (size);

	return Lock_Tokenizer::memory_area.malloc(sizeof(mLock_Mutex));
    }
    void operator        delete(void *ptr)
    {
	Lock_Tokenizer::memory_area.free(ptr);
    }
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

#define if_RLOCK(x, y) \
	bool __if_res = false; \
	{ RLOCK(x); __if_res = y; } \
	if (__if_res)
#define if_RLOCK2(x, y) \
	__if_res = false; \
	{ RLOCK(x); __if_res = y; } \
	if (__if_res)
#define if_WLOCK(x, y) \
	bool __if_res = false; \
	{ WLOCK(x); __if_res = y; } \
	if (__if_res)
#define if_WLOCK2(x, y) \
	__if_res = false; \
	{ WLOCK(x); __if_res = y; } \
	if (__if_res)
#define if_MLOCK(x, y) \
	bool __if_res = false; \
	{ MLOCK(x); __if_res = y; } \
	if (__if_res)
#define if_MLOCK2(x, y) \
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

#define if_RLOCK(x, y) if (y)
#define if_RLOCK2(x, y) if (y)
#define if_WLOCK(x, y) if (y)
#define if_WLOCK2(x, y) if (y)
#define if_MLOCK(x, y) if (y)
#define if_MLOCK2(x, y) if (y)

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
    static map < unsigned long, mSocket * > SockMap;
    static unsigned short FindAvailPort();

    mSocket()
    {
	init();
    }
    mSocket(const ACE_INET_Addr & addr, const unsigned long timeout)
    {
	init();
	Connect(addr, timeout);
    }
    mSocket(const mstring & host, const unsigned short port, const unsigned long timeout)
    {
	init();
	Connect(host, port, timeout);
    }
    mSocket(const unsigned short port, const unsigned long timeout)
    {
	init();
	Accept(port, timeout);
    }
    mSocket(ACE_SOCK_Stream * in, const dir_enum direction, const bool alloc)
    {
	init();
	Bind(in, direction, alloc);
    }

    mSocket(const mSocket & in)
    {
	*this = in;
    }
    ~mSocket();
    mSocket &operator=(const mSocket & in);

    bool Connect(const ACE_INET_Addr & addr, const unsigned long timeout = 0);
    bool Connect(const unsigned long host, const unsigned short port, const unsigned long timeout = 0);
    bool Connect(const mstring & host, const unsigned short port, const unsigned long timeout = 0);
    bool Accept(const unsigned short port, const unsigned long timeout = 0);
    bool Bind(ACE_SOCK_Stream * in, const dir_enum direction = D_Unknown, const bool alloc = true);
    ACE_SOCK_Stream *Unbind();

    mstring Local_Host() const;
    unsigned long Local_IP() const;
    unsigned short Local_Port() const;
    mstring Remote_Host() const;
    unsigned long Remote_IP() const;
    unsigned short Remote_Port() const;

    bool IsConnected() const;
    void Resolve(const socktype_enum type, const mstring & info);
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
    typedef map < ACE_thread_t, ThreadID * > selftothreadidmap_t;
    static selftothreadidmap_t selftothreadidmap;
    static mLock_Mutex *maplock;

#ifdef MAGICK_HAS_EXCEPTIONS
    static bool AcquireMapLock() throw(E_Lock);
    static bool ReleaseMapLock() throw(E_Lock);
#else
    static bool AcquireMapLock();
    static bool ReleaseMapLock();
#endif

public:
    static ThreadID *find(const ACE_thread_t thread = ACE_Thread::self());
    static vector < ThreadID * > findall();
    static size_t size()
    {
	return selftothreadidmap.size();
    }
    static void Attach(const threadtype_enum ttype);
    static void Detach();
    static void ReAttach(const threadtype_enum ttype);
};

#endif
