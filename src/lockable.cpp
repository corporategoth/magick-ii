#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_lockable_cpp_ ## x () { return y; }
RCSID(lockable_cpp, "@(#)$Id$");

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

#include "magick.h"

#ifdef MAGICK_LOCKS_WORK

ACE_Thread_Mutex *Lock_Tokenizer::maplock = NULL;
mLock_Mutex *mThread::maplock = NULL;
map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > > Lock_Tokenizer::LockMap;

ACE_Expandable_Cached_Fixed_Allocator < ACE_Thread_Mutex >
    Lock_Tokenizer::
memory_area((sizeof(mLock_Read) >
	     sizeof(mLock_Write) ? (sizeof(mLock_Read) >
				    sizeof(mLock_Mutex) ? sizeof(mLock_Read) : sizeof(mLock_Mutex)) : (sizeof(mLock_Write) >
												       sizeof(mLock_Mutex) ?
												       sizeof(mLock_Write) :
												       sizeof(mLock_Mutex))),
	    LOCK_SEGMENT);

map < unsigned long, mSocket * > mSocket::SockMap;

#ifdef MAGICK_HAS_EXCEPTIONS
bool Lock_Tokenizer::AcquireMapLock() throw (E_Lock)
#else
bool Lock_Tokenizer::AcquireMapLock()
#endif
{
    BTCB();
    if (maplock == NULL)
	maplock = new ACE_Thread_Mutex("LockMap");
    if (maplock == NULL)
    {
	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", ("MUTEX", "LockMap"));
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Lock(L_Mutex, E_Lock::T_Create, "LockMap"));
#endif
	return false;
    }
    if (maplock->acquire() < 0)
    {
	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("MUTEX", "LockMap"));
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Lock(L_Mutex, E_Lock::T_Acquire, "LockMap"));
#endif
	return false;
    }
    return true;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
bool Lock_Tokenizer::ReleaseMapLock() throw (E_Lock)
#else
bool Lock_Tokenizer::ReleaseMapLock()
#endif
{
    BTCB();
    if (maplock == NULL)
	return true;
    if (maplock->release() < 0)
    {
	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", ("MUTEX", "LockMap"));
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Lock(L_Mutex, E_Lock::T_Release, "LockMap"));
#endif
	return false;
    }
    return true;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
mLock_Read *Lock_Tokenizer::getReadLock(const mstring & lockname, bool recursive) throw (E_Thread)
#else
mLock_Read *Lock_Tokenizer::getReadLock(const mstring & lockname, bool recursive)
#endif
{
    BTCB();
    map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > >::iterator lockiter;
    mLock_Read *read_lock = NULL;
    char hash[33];

    memset(hash, 0, sizeof(hash));

    if (!AcquireMapLock())
	return NULL;
    if ((lockiter = LockMap.find(lockname)) != LockMap.end())
    {
	map < ACE_thread_t, locktype_enum >::iterator typeiter = lockiter->second.second.find(ACE_Thread::self());
	if (typeiter == lockiter->second.second.end())
	{
	    lockiter->second.second[ACE_Thread::self()] = L_Read;
	    read_lock = reinterpret_cast < mLock_Read * > (lockiter->second.first);
	}
	else if (typeiter->second == L_Mutex)
	{
	    ReleaseMapLock();
	    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_DUP", ("READ", lockname));
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_Lock(L_Read, E_Lock::T_Duplicate, lockname.c_str()));
#endif
	    return NULL;
	}
	else if (recursive && (typeiter->second == L_Read || typeiter->second == L_WriteUpgrade))
	    read_lock = reinterpret_cast < mLock_Read * > (lockiter->second.first);
    }
    else
    {
	mHASH(lockname.c_str(), lockname.length(), hash);
	read_lock = new mLock_Read(reinterpret_cast < const char * > (hash));

	if (read_lock == NULL)
	{
	    ReleaseMapLock();
	    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", ("READ", lockname));
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_Lock(L_Read, E_Lock::T_Create, lockname.c_str()));
#endif
	    return NULL;
	}
	map < ACE_thread_t, locktype_enum > tmap;
	LockMap[lockname] = pair < void *, map < ACE_thread_t, locktype_enum > > (read_lock, tmap);

	LockMap[lockname].second[ACE_Thread::self()] = L_Read;
    }
    ReleaseMapLock();

    return read_lock;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
mLock_Write *Lock_Tokenizer::getWriteLock(const mstring & lockname, bool recursive) throw (E_Thread)
#else
mLock_Write *Lock_Tokenizer::getWriteLock(const mstring & lockname, bool recursive)
#endif
{
    BTCB();
    map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > >::iterator lockiter;
    mLock_Write *write_lock = NULL;
    char hash[33];

    memset(hash, 0, sizeof(hash));

    if (!AcquireMapLock())
	return NULL;
    if ((lockiter = LockMap.find(lockname)) != LockMap.end())
    {
	map < ACE_thread_t, locktype_enum >::iterator typeiter = lockiter->second.second.find(ACE_Thread::self());
	if (typeiter == lockiter->second.second.end())
	{
	    lockiter->second.second[ACE_Thread::self()] = L_Write;
	    write_lock = reinterpret_cast < mLock_Write * > (lockiter->second.first);
	}
	else if (typeiter->second == L_Mutex)
	{
	    ReleaseMapLock();
	    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_DUP", ("WRITE", lockname));
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_Lock(L_Write, E_Lock::T_Duplicate, lockname.c_str()));
#endif
	    return NULL;
	}
	else if (typeiter->second == L_Read)
	{
	    typeiter->second = L_WriteUpgrade;
	    write_lock = reinterpret_cast < mLock_Write * > (lockiter->second.first);
	}
	else if (recursive && (typeiter->second == L_Write || typeiter->second == L_WriteUpgrade))
	    write_lock = reinterpret_cast < mLock_Write * > (lockiter->second.first);
    }
    else
    {
	mHASH(lockname.c_str(), lockname.length(), hash);
	write_lock = new mLock_Write(reinterpret_cast < const char * > (hash));

	if (write_lock == NULL)
	{
	    ReleaseMapLock();
	    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", ("WRITE", lockname));
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_Lock(L_Write, E_Lock::T_Create, lockname.c_str()));
#endif
	    return NULL;
	}
	map < ACE_thread_t, locktype_enum > tmap;
	LockMap[lockname] = pair < void *, map < ACE_thread_t, locktype_enum > > (write_lock, tmap);

	LockMap[lockname].second[ACE_Thread::self()] = L_Write;
    }
    ReleaseMapLock();

    return write_lock;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
mLock_Mutex *Lock_Tokenizer::getMutexLock(const mstring & lockname, bool recursive) throw (E_Thread)
#else
mLock_Mutex *Lock_Tokenizer::getMutexLock(const mstring & lockname, bool recursive)
#endif
{
    BTCB();
    map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > >::iterator lockiter;
    mLock_Mutex *mutex_lock = NULL;
    char hash[33];

    memset(hash, 0, sizeof(hash));

    if (!AcquireMapLock())
	return NULL;
    if ((lockiter = LockMap.find(lockname)) != LockMap.end())
    {
	map < ACE_thread_t, locktype_enum >::iterator typeiter = lockiter->second.second.find(ACE_Thread::self());
	if (typeiter == lockiter->second.second.end())
	{
	    lockiter->second.second[ACE_Thread::self()] = L_Mutex;
	    mutex_lock = reinterpret_cast < mLock_Mutex * > (lockiter->second.first);
	}
	else if (typeiter->second != L_Mutex)
	{
	    ReleaseMapLock();
	    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_DUP", ("MUTEX", lockname));
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_Lock(L_Mutex, E_Lock::T_Duplicate, lockname.c_str()));
#endif
	    return NULL;
	}
	else if (recursive && typeiter->second == L_Mutex)
	    mutex_lock = reinterpret_cast < mLock_Mutex * > (lockiter->second.first);
    }
    else
    {
	mHASH(lockname.c_str(), lockname.length(), hash);
	mutex_lock = new mLock_Mutex(reinterpret_cast < const char * > (hash));

	if (mutex_lock == NULL)
	{
	    ReleaseMapLock();
	    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", ("MUTEX", lockname));
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_Lock(L_Mutex, E_Lock::T_Create, lockname.c_str()));
#endif
	    return NULL;
	}
	map < ACE_thread_t, locktype_enum > tmap;
	LockMap[lockname] = pair < void *, map < ACE_thread_t, locktype_enum > > (mutex_lock, tmap);

	LockMap[lockname].second[ACE_Thread::self()] = L_Mutex;
    }
    ReleaseMapLock();

    return mutex_lock;
    ETCB();
}

void Lock_Tokenizer::releaseLock(const mstring & lockname)
{
    BTCB();
    map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > >::iterator lockiter;

    map < ACE_thread_t, locktype_enum >::iterator typeiter;
    mLock_Read *read_lock = NULL;
    mLock_Write *write_lock = NULL;
    mLock_Mutex *mutex_lock = NULL;

    if (!AcquireMapLock())
	return;
    lockiter = LockMap.find(lockname);
    if (lockiter != LockMap.end())
    {
	typeiter = lockiter->second.second.find(ACE_Thread::self());
	if (typeiter != lockiter->second.second.end())
	{
	    if (lockiter->second.second.size() == 1)
	    {
		switch (typeiter->second)
		{
		case L_Read:
		    read_lock = reinterpret_cast < mLock_Read * > (lockiter->second.first);
		    if (read_lock != NULL)
			delete read_lock;
		    LockMap.erase(lockiter);
		    break;
		case L_Write:
		    write_lock = reinterpret_cast < mLock_Write * > (lockiter->second.first);
		    if (write_lock != NULL)
			delete write_lock;
		    LockMap.erase(lockiter);
		    break;
		case L_WriteUpgrade:
		    typeiter->second = L_Read;
		    break;
		case L_Mutex:
		    mutex_lock = reinterpret_cast < mLock_Mutex * > (lockiter->second.first);
		    if (mutex_lock != NULL)
			delete mutex_lock;
		    LockMap.erase(lockiter);
		    break;
		default:
		    // WTF?
		    break;
		}
	    }
	    else
	    {
		if (typeiter->second == L_WriteUpgrade)
		    typeiter->second = L_Read;
		else
		    lockiter->second.second.erase(typeiter);
	    }
	}
    }
    ReleaseMapLock();
    ETCB();
}

locktype_enum Lock_Tokenizer::getType(const mstring & lockname)
{
    BTCB();
    map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > >::iterator lockiter;
    locktype_enum retval = L_Invalid;

    if (!AcquireMapLock())
	return retval;
    if ((lockiter = LockMap.find(lockname)) != LockMap.end())
    {
	map < ACE_thread_t, locktype_enum >::iterator typeiter = lockiter->second.second.find(ACE_Thread::self());
	if (typeiter != lockiter->second.second.end())
	    retval = typeiter->second;
    }
    ReleaseMapLock();

    return retval;
    ETCB();
}

list < pair < void *, locktype_enum > > Lock_Tokenizer::GetLocks(ACE_thread_t thr)
{
    BTCB();
    list < pair < void *, locktype_enum > > retval;
    map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > >::iterator i;

    map < ACE_thread_t, locktype_enum >::iterator j;

    if (!AcquireMapLock())
	return retval;
    for (i = LockMap.begin(); i != LockMap.end(); i++)
    {
	for (j = i->second.second.begin(); j != i->second.second.end(); j++)
	{
	    if (j->first == thr)
		retval.push_back(pair < void *, locktype_enum > (i->second.first, j->second));
	}
    }
    ReleaseMapLock();
    return retval;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
mLOCK::mLOCK(const locktype_enum type, const mVarArray & args) throw (E_Lock)
#else
mLOCK::mLOCK(const locktype_enum type, const mVarArray & args)
#endif
{
    BTCB();
    islocked = false;
    if (Magick::StartTime() == mDateTime(0.0) || !Magick::instance_exists() ||
	Magick::instance().ResetTime() == mDateTime(0.0))
	return;

    int i;

#ifdef MAGICK_TRACE_WORKS
    int count = 0;
#endif
    mstring lockname;
    mLock_Mutex *mutex_lock;
    mLock_Write *write_lock;
    mLock_Read *read_lock;

    for (i = 0; i < args.count() - 1; i++)
    {
	if (!lockname.empty())
	    lockname += "::";
	lockname += args[i].AsString();
	read_lock = Lock_Tokenizer::getReadLock(lockname);

	if (read_lock != NULL)
	{
	    if (read_lock->acquire() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("READ", lockname));
		Lock_Tokenizer::releaseLock(lockname);
#ifdef MAGICK_HAS_EXCEPTIONS
		throw (E_Lock(L_Read, E_Lock::T_Acquire, lockname.c_str()));
#endif
	    }
	    else
	    {
		locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		tlock[count++].open(L_Read, lockname);
#endif
	    }
	}
    }

    if (!lockname.empty())
	lockname += "::";
    lockname += args[i].AsString();
    read_lock = NULL;
    write_lock = NULL;
    mutex_lock = NULL;

    if (type == L_Read)
    {
	read_lock = Lock_Tokenizer::getReadLock(lockname);
	if (read_lock != NULL)
	{
	    if (read_lock->acquire() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("READ", lockname));
		Lock_Tokenizer::releaseLock(lockname);
#ifdef MAGICK_HAS_EXCEPTIONS
		throw (E_Lock(L_Read, E_Lock::T_Acquire, lockname.c_str()));
#endif
	    }
	    else
	    {
		islocked = true;
		locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		tlock[count++].open(L_Read, lockname);
#endif
	    }
	}
    }
    else if (type == L_Write)
    {
	if (Lock_Tokenizer::getType(lockname) == L_Read)
	    read_lock = Lock_Tokenizer::getReadLock(lockname, true);
	if (read_lock != NULL && read_lock->release() < 0)
	{
	    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", ("READ", lockname));
	    read_lock = NULL;
	    Lock_Tokenizer::releaseLock(lockname);
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_Lock(L_Read, E_Lock::T_Release, lockname.c_str()));
#endif
	}
	write_lock = Lock_Tokenizer::getWriteLock(lockname);
	if (write_lock != NULL)
	{
	    if (write_lock->acquire() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("WRITE", lockname));
		Lock_Tokenizer::releaseLock(lockname);
		if (read_lock != NULL && read_lock->acquire() < 0)
		{
		    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("READ", lockname));
		    Lock_Tokenizer::releaseLock(lockname);
		}
#ifdef MAGICK_HAS_EXCEPTIONS
		throw (E_Lock(L_Write, E_Lock::T_Acquire, lockname.c_str()));
#endif
	    }
	    else
	    {
		islocked = true;
		locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		tlock[count++].open(L_Write, lockname);
#endif
	    }
	}
    }
    else if (type == L_Mutex)
    {
	mutex_lock = Lock_Tokenizer::getMutexLock(lockname);
	if (mutex_lock != NULL)
	{
	    if (mutex_lock->acquire() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("MUTEX", lockname));
		Lock_Tokenizer::releaseLock(lockname);
#ifdef MAGICK_HAS_EXCEPTIONS
		throw (E_Lock(L_Mutex, E_Lock::T_Acquire, lockname.c_str()));
#endif
	    }
	    else
	    {
		islocked = true;
		locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		tlock[count++].open(L_Mutex, lockname);
#endif
	    }
	}
    }
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
mLOCK::~mLOCK() throw (E_Lock)
#else
mLOCK::~mLOCK()
#endif
{
    BTCB();
    if (Magick::StartTime() == mDateTime(0.0) || !Magick::instance_exists() ||
	Magick::instance().ResetTime() == mDateTime(0.0))
	return;

    int i;
    mLock_Mutex *mutex_lock;
    mLock_Write *write_lock;
    mLock_Read *read_lock;

    for (i = locks.size() - 1; i >= 0; i--)
    {
	read_lock = NULL;
	write_lock = NULL;
	mutex_lock = NULL;
	switch (Lock_Tokenizer::getType(locks[i]))
	{
	case L_Read:
	    read_lock = Lock_Tokenizer::getReadLock(locks[i], true);
	    break;
	case L_Write:
	    write_lock = Lock_Tokenizer::getWriteLock(locks[i], true);
	    break;
	case L_WriteUpgrade:
	    write_lock = Lock_Tokenizer::getWriteLock(locks[i], true);
	    read_lock = Lock_Tokenizer::getReadLock(locks[i], true);
	    break;
	case L_Mutex:
	    mutex_lock = Lock_Tokenizer::getMutexLock(locks[i], true);
	    break;
	default:
	    break;
	}

	if (write_lock != NULL)
	{
	    if (write_lock->release() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", ("WRITE", locks[i]));
		Lock_Tokenizer::releaseLock(locks[i]);
		if (read_lock != NULL && read_lock->acquire() < 0)
		{
		    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("READ", locks[i]));
		    Lock_Tokenizer::releaseLock(locks[i]);
		}
		read_lock = NULL;
#ifdef MAGICK_HAS_EXCEPTIONS
		throw (E_Lock(L_Write, E_Lock::T_Release, locks[i].c_str()));
#endif
	    }
	    else
		Lock_Tokenizer::releaseLock(locks[i]);
	    if (read_lock != NULL && read_lock->acquire() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("READ", locks[i]));
		Lock_Tokenizer::releaseLock(locks[i]);
#ifdef MAGICK_HAS_EXCEPTIONS
		throw (E_Lock(L_Read, E_Lock::T_Acquire, locks[i].c_str()));
#endif
	    }
	}
	else if (read_lock != NULL)
	{
	    if (read_lock->release() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", ("READ", locks[i]));
		Lock_Tokenizer::releaseLock(locks[i]);
#ifdef MAGICK_HAS_EXCEPTIONS
		throw (E_Lock(L_Read, E_Lock::T_Release, locks[i].c_str()));
#endif
	    }
	    else
		Lock_Tokenizer::releaseLock(locks[i]);
	}
	else if (mutex_lock != NULL)
	{
	    if (mutex_lock->release() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", ("MUTEX", locks[i]));
		Lock_Tokenizer::releaseLock(locks[i]);
#ifdef MAGICK_HAS_EXCEPTIONS
		throw (E_Lock(L_Mutex, E_Lock::T_Release, locks[i].c_str()));
#endif
	    }
	    else
		Lock_Tokenizer::releaseLock(locks[i]);
	}
    }
    ETCB();
}

#endif /* MAGICK_LOCKS_WORK */

unsigned short mSocket::FindAvailPort()
{
    BTCB();
    NFT("mSocket::FindAvailPort");

    ACE_INET_Addr la;
    ACE_SOCK_Acceptor acc(ACE_Addr::sap_any);

    acc.get_local_addr(la);
    unsigned short retval = la.get_port_number();

    acc.close();
    RET(retval);
    ETCB();
}

void mSocket::init()
{
    BTCB();
    NFT("mSocket::init");

    sock = NULL;
    last_error = 0;
    sockid = 0;
    DestroyMe = false;

    if (Magick::StartTime() == mDateTime(0.0) || !Magick::instance_exists() ||
	Magick::instance().ResetTime() == mDateTime(0.0))
	return;

    unsigned long i;

    MLOCK(("SockMap"));
    for (i = 1; i > 0; i++)
	if (SockMap.find(i) == SockMap.end())
	{
	    SockMap[i] = this;
	    sockid = i;
	    break;
	}
    ETCB();
}

mSocket::~mSocket()
{
    BTCB();
    NFT("mSocket::mSocket");
    if (sock != NULL)
	close();
    MLOCK(("SockMap"));
    SockMap.erase(sockid);
    ETCB();
}

mSocket &mSocket::operator=(const mSocket & in)
{
    BTCB();
    FT("mSocket::operator=", ("(const mSocket &) in"));

    sockid = in.sockid;
    local = in.local;
    remote = in.remote;
    last_error = in.last_error;
    DestroyMe = in.DestroyMe;
#ifdef MAGICK_TRACE_WORKS
    trace = in.trace;
#endif
    sock = in.sock;
    in.sock = NULL;

    {
	MLOCK2(("SockMap"));
	SockMap[sockid] = this;
    }
    NRET(mSocket &, *this);
    ETCB();
}

bool mSocket::Connect(const ACE_INET_Addr & addr, const mDateTime &timeout)
{
    BTCB();
    FT("mSocket::Connect", ("(ACE_INET_Addr) addr", timeout));

    WLOCK((lck_mSocket, sockid));
    if (sock != NULL)
	close();

    sock = new ACE_SOCK_Stream;
    if (sock == NULL)
	RET(false);

    DestroyMe = true;
    ACE_Time_Value tv(timeout.Second(), timeout.MSecond());
    ACE_SOCK_Connector tmp;
    int result = tmp.connect(*sock, addr, (timeout != mDateTime(0.0)) ? &tv : 0);

    if (result < 0)
    {
	last_error = errno;
#ifdef MAGICK_TRACE_WORKS
	trace.Failed(sockid, local.get_port_number(), remote.get_port_number(), mstring(remote.get_host_addr()),
		     Last_Error_String(), D_From);

#endif
	delete sock;

	sock = NULL;
	RET(false);
    }
    last_error = 0;
    sock->get_local_addr(local);
    sock->get_remote_addr(remote);
#ifdef MAGICK_TRACE_WORKS
    trace.Begin(sockid, local.get_port_number(), remote.get_port_number(), mstring(remote.get_host_addr()), D_From);

#endif
    RET(true);
    ETCB();
}

bool mSocket::Connect(const unsigned long host, const unsigned short port, const mDateTime &timeout)
{
    BTCB();
    FT("mSocket::Connect", (host, port, timeout));
    ACE_INET_Addr tmp;
    if (tmp.set(port, host) < 0)
    {
	last_error = EINVAL;
	RET(false);
    }
    bool retval = Connect(tmp, timeout);

    RET(retval);
    ETCB();
}

bool mSocket::Connect(const mstring & host, const unsigned short port, const mDateTime &timeout)
{
    BTCB();
    FT("mSocket::Connect", (host, port, timeout));
    ACE_INET_Addr tmp;
    if (tmp.set(port, host) < 0)
    {
	last_error = EINVAL;
	RET(false);
    }
    bool retval = Connect(tmp, timeout);

    RET(retval);
    ETCB();
}

bool mSocket::Accept(const unsigned short port, const mDateTime &timeout)
{
    BTCB();
    FT("mSocket::Accept", (port, timeout));
    ACE_INET_Addr addr;
    if (addr.set(port, Magick::instance().LocalHost()) < 0)
    {
	last_error = EINVAL;
	RET(false);
    }

    WLOCK((lck_mSocket, sockid));
    if (sock != NULL)
	close();

    sock = new ACE_SOCK_Stream;
    if (sock == NULL)
	RET(false);

    DestroyMe = true;
    ACE_Time_Value tv(timeout.Second(), timeout.MSecond());
    ACE_SOCK_Acceptor tmp(addr);
    int result = tmp.accept(*sock, NULL, (timeout != mDateTime(0.0)) ? &tv : 0);

    if (result < 0)
    {
	last_error = errno;
#ifdef MAGICK_TRACE_WORKS
	trace.Failed(sockid, local.get_port_number(), remote.get_port_number(), mstring(remote.get_host_addr()),
		     Last_Error_String(), D_To);
#endif
	delete sock;

	sock = NULL;
	RET(false);
    }

    last_error = 0;
    sock->get_local_addr(local);
    sock->get_remote_addr(remote);
#ifdef MAGICK_TRACE_WORKS
    trace.Begin(sockid, local.get_port_number(), remote.get_port_number(), mstring(remote.get_host_addr()), D_To);
#endif
    RET(true);
    ETCB();
}

bool mSocket::Bind(ACE_SOCK_Stream * in, const dir_enum direction, const bool alloc)
{
    BTCB();
    FT("mSocket::Bind", ("(ACE_SOCK_Stream *) in"));
    if (in == NULL)
	RET(false);

    WLOCK((lck_mSocket, sockid));
    if (sock != NULL)
	close();

    sock = in;
    in = NULL;
    DestroyMe = alloc;
    last_error = 0;
    sock->get_local_addr(local);
    sock->get_remote_addr(remote);

#ifdef MAGICK_TRACE_WORKS
    trace.Begin(sockid, local.get_port_number(), remote.get_port_number(), mstring(remote.get_host_addr()), direction);
#else
    static_cast < void > (direction);
#endif

    RET(true);
    ETCB();
}

ACE_SOCK_Stream *mSocket::Unbind()
{
    BTCB();
    NFT("mSocket::Unbind");
    WLOCK((lck_mSocket, sockid));
#ifdef MAGICK_TRACE_WORKS
    trace.End(Last_Error_String());
#endif
    ACE_SOCK_Stream *retval = sock;

    sock = NULL;
    last_error = 0;
    NRET(ACE_SOCK_Stream *, retval);
    ETCB();
}

mstring mSocket::Local_Host() const
{
    BTCB();
    NFT("mSocket::Local_Host");
    RLOCK((lck_mSocket, sockid));
    mstring retval(local.get_host_name());

    RET(retval);
    ETCB();
}

mstring mSocket::Local_IP() const
{
    BTCB();
    NFT("mSocket::Local_IP");
    RLOCK((lck_mSocket, sockid));
    mstring retval(local.get_host_addr());

    RET(retval);
    ETCB();
}

unsigned short mSocket::Local_Port() const
{
    BTCB();
    NFT("mSocket::Local_Port");
    RLOCK((lck_mSocket, sockid));
    unsigned short retval = local.get_port_number();

    RET(retval);
    ETCB();
}

mstring mSocket::Remote_Host() const
{
    BTCB();
    NFT("mSocket::Remote_Host");
    RLOCK((lck_mSocket, sockid));
    mstring retval(remote.get_host_name());

    RET(retval);
    ETCB();
}

mstring mSocket::Remote_IP() const
{
    BTCB();
    NFT("mSocket::Remote_IP");
    RLOCK((lck_mSocket, sockid));
    mstring retval(remote.get_host_addr());

    RET(retval);
    ETCB();
}

unsigned short mSocket::Remote_Port() const
{
    BTCB();
    NFT("mSocket::Remote_Port");
    RLOCK((lck_mSocket, sockid));
    unsigned short retval = remote.get_port_number();

    RET(retval);
    ETCB();
}

bool mSocket::IsConnected() const
{
    BTCB();
    NFT("mSocket::IsConnected");
    RLOCK((lck_mSocket, sockid));
    RET(sock != NULL);
    ETCB();
}

void mSocket::Resolve(const socktype_enum type, const mstring & info)
{
    BTCB();
    FT("mSocket::Resolve", (static_cast < int > (type), info));
#ifdef MAGICK_TRACE_WORKS
    WLOCK((lck_mSocket, sockid));
    trace.Resolve(type, info);
#else
    static_cast < void > (type);
    static_cast < void > (info);
#endif
    ETCB();
}

int mSocket::Last_Error() const
{
    BTCB();
    NFT("mSocket::Last_Error");
    RLOCK((lck_mSocket, sockid));
    RET(last_error);
    ETCB();
}

mstring mSocket::Last_Error_String() const
{
    BTCB();
    NFT("mSocket::Last_Error_String");
    RLOCK((lck_mSocket, sockid));
    mstring retval = strerror(last_error);

    RET(retval);
    ETCB();
}

ssize_t mSocket::send(void *buf, const size_t len, const mDateTime &timeout)
{
    BTCB();
    FT("mSocket::send", ("(void *) buf", len, timeout));
    ACE_Time_Value tv(timeout.Second(), timeout.MSecond());

    WLOCK((lck_mSocket, sockid));
    ssize_t retval = sock->send(buf, len, (timeout != mDateTime(0.0)) ? &tv : 0);

    if (retval < 0)
    {
	last_error = errno;
	// close();
    }
    else
	last_error = 0;

    RET(retval);
    ETCB();
}

ssize_t mSocket::recv(void *buf, const size_t len, const mDateTime &timeout)
{
    BTCB();
    FT("mSocket::recv", ("(void *) buf", len, timeout));
    ACE_Time_Value tv(timeout.Second(), timeout.MSecond());

    WLOCK((lck_mSocket, sockid));
    ssize_t retval = sock->recv(buf, len, (timeout != mDateTime(0.0)) ? &tv : 0);

    if (retval < 0)
    {
	last_error = errno;
	// close();
    }
    else
	last_error = 0;

    RET(retval);
    ETCB();
}

int mSocket::close()
{
    BTCB();
    NFT("mSocket::close");
    int retval = 0;

    WLOCK((lck_mSocket, sockid));
    if (sock != NULL)
    {
#ifdef MAGICK_TRACE_WORKS
	trace.End(Last_Error_String());
#endif
	retval = sock->close();
	if (retval < 0)
	    last_error = errno;
	else
	    last_error = 0;
	if (DestroyMe)
	    delete sock;

	sock = NULL;
    }
    RET(retval);
    ETCB();
}

mThread::selftothreadidmap_t mThread::selftothreadidmap;

#ifdef MAGICK_HAS_EXCEPTIONS
bool mThread::AcquireMapLock() throw (E_Lock)
#else
bool mThread::AcquireMapLock()
#endif
{
    BTCB();
    if (maplock == NULL)
	maplock = new mLock_Mutex("SelfToThreadIdMap");
    if (maplock == NULL)
    {
	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", ("MUTEX", "SelfToThreadIdMap"));
	return false;
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Lock(L_Mutex, E_Lock::T_Create, "SelfToThreadIdMap"));
#endif
    }
    if (maplock->acquire() < 0)
    {
	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("MUTEX", "SelfToThreadIdMap"));
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Lock(L_Mutex, E_Lock::T_Acquire, "SelfToThreadIdMap"));
#endif
	return false;
    }
    return true;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
bool mThread::ReleaseMapLock() throw (E_Lock)
#else
bool mThread::ReleaseMapLock()
#endif
{
    BTCB();
    if (maplock == NULL)
	return true;
    if (maplock->release() < 0)
    {
	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", ("MUTEX", "SelfToThreadIdMap"));
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Lock(L_Mutex, E_Lock::T_Release, "SelfToThreadIdMap"));
#endif
	return false;
    }
    return true;
    ETCB();
}

ThreadID *mThread::find(const ACE_thread_t thread)
{
    BTCB();
    if (Magick::StartTime() == mDateTime(0.0))
	return NULL;

    if (!AcquireMapLock())
	return NULL;

    ThreadID *tid = NULL;

    if (selftothreadidmap.find(thread) != selftothreadidmap.end())
	tid = selftothreadidmap[thread];

    ReleaseMapLock();
    return tid;
    ETCB();
}

vector < ThreadID * > mThread::findall()
{
    BTCB();
    vector < ThreadID * > threadlist;
    selftothreadidmap_t::const_iterator iter;

    if (!AcquireMapLock())
	return threadlist;

    for (iter = selftothreadidmap.begin(); iter != selftothreadidmap.end(); iter++)
	threadlist.push_back(iter->second);

    ReleaseMapLock();
    return threadlist;
    ETCB();
}

void mThread::Attach(const threadtype_enum ttype)
{
    BTCB();
    FT("mThread::Attach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid = new ThreadID(ttype);

    if (!AcquireMapLock())
	return;
    selftothreadidmap[ACE_Thread::self()] = tmpid;
    ReleaseMapLock();
    COM(("Thread ID has been attached."));
    ETCB();
}

void mThread::Detach()
{
    BTCB();
    NFT("mThread::Detach");
    ThreadID *tmpid = find();

    if (tmpid == NULL)
    {
	// todo ttype to typename
	CP(("mThread::Detach without valid mThread::Attach..."));
	return;
    }
    if (!AcquireMapLock())
	return;
    selftothreadidmap.erase(ACE_Thread::self());
    ReleaseMapLock();
    delete tmpid;

    COM(("Thread ID has been detached."));
    ETCB();
}

void mThread::ReAttach(const threadtype_enum ttype)
{
    BTCB();
    FT("mThread::ReAttach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid = find();

    if (tmpid == NULL)
    {
	// ReAttach does an attach if it wasnt there
	CP(("mThread::ReAttach without valid mThread::Attach... type: %s", threadname[ttype].c_str()));
	tmpid = new ThreadID();
	if (!AcquireMapLock())
	    return;
	selftothreadidmap[ACE_Thread::self()] = tmpid;
	ReleaseMapLock();
    }
#ifdef MAGICK_TRACE_WORKS	// Get rid of 'unused veriable' warning
    threadtype_enum oldtype = tmpid->type();
#endif
    COM(("Thread ID has been re-attached to %s.", threadname[ttype].c_str()));
    tmpid->assign(ttype);
    COM(("Thread ID has been re-attached from %s.", threadname[oldtype].c_str()));
    ETCB();
}
