#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
#define RCSID(x,y) const char *rcsid_lockable_cpp_ ## x () { return y; }
RCSID(lockable_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.65  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.64  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.63  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.62  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.61  2001/02/03 02:21:33  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.60  2001/01/15 23:31:39  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.59  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.58  2000/12/25 06:36:14  prez
** Added locking around the threadtoself map, and removed a bunch of
** defines from mstring (while keeping it the same!)
**
** Revision 1.57  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.56  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.55  2000/10/18 18:46:33  prez
** Well, mstring still coredumps, but it gets past the initial loading of
** all the STATIC (or const) strings, etc -- now its coring on loading a
** file (or possibly language.h or something).  Still investigating.
**
** Revision 1.54  2000/10/10 11:47:51  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.53  2000/10/07 11:01:13  prez
** Took out placement new's from lockable.cpp, now using derived classes.
**
** Revision 1.52  2000/10/04 10:52:08  prez
** Fixed the memory pool and removed printf's.
**
** Revision 1.51  2000/10/04 07:39:46  prez
** Added MemCluster to speed up lockable, but it cores when we start
** getting real messages -- seemingly in an alloc in the events.
** Lots of printf's left in for debugging (so run as ./magick >output)
**
** Revision 1.50  2000/09/13 12:45:34  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.49  2000/09/07 08:13:17  prez
** Fixed some of the erronous messages (SVSHOST, SQLINE, etc).
** Also added CPU statistics and fixed problem with socket deletions.
**
** Revision 1.48  2000/09/06 11:27:33  prez
** Finished the T_Modify / T_Changing traces, fixed a bug in clone
** adding (was adding clone liimt as the mask length), updated docos
** a little more, and added a response to SIGINT to signon clients.
**
** Revision 1.47  2000/09/05 10:53:07  prez
** Only have operserv.cpp and server.cpp to go with T_Changing / T_Modify
** tracing -- also modified keygen to allow for cmdline generation (ie.
** specify 1 option and enter keys, or 2 options and the key is read from
** a file).  This allows for paragraphs with \n's in them, and helps so you
** do not have to type out 1024 bytes :)
**
** Revision 1.46  2000/09/01 10:54:38  prez
** Added Changing and implemented Modify tracing, now just need to create
** DumpB() and DumpE() functions in all classes, and put MCB() / MCE() calls
** (or MB() / ME() or CB() / CE() where MCB() / MCE() not appropriate) in.
**
** Revision 1.45  2000/08/31 06:25:09  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.44  2000/08/28 10:51:37  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.43  2000/08/22 08:43:41  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.42  2000/08/19 10:59:47  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.41  2000/08/06 05:27:47  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.40  2000/08/03 13:06:31  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.39  2000/07/21 00:18:49  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.38  2000/06/13 14:11:54  prez
** Locking system has been re-written, it doent core anymore.
** So I have set 'MAGICK_LOCKS_WORK' define active :)
**
** Revision 1.37  2000/06/06 08:57:56  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.36  2000/05/20 03:28:11  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.35  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.34  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.33  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.32  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.31  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"

#ifdef MAGICK_LOCKS_WORK

map<ACE_thread_t, map<mstring, pair<locktype_enum, void *> > > mLOCK::LockMap;
ACE_Expandable_Cached_Fixed_Allocator<ACE_Thread_Mutex> mLOCK::memory_area(
	(sizeof(mLock_Read) > sizeof(mLock_Write) ?
	    (sizeof(mLock_Read) > sizeof(mLock_Mutex) ?
		sizeof(mLock_Read) : sizeof(mLock_Mutex)) :
	    (sizeof(mLock_Write) > sizeof(mLock_Mutex) ?
		sizeof(mLock_Write) : sizeof(mLock_Mutex))), LOCK_SEGMENT);

map<unsigned long, mSocket *> mSocket::SockMap;

mLOCK::mLOCK(const locktype_enum type, const mVarArray &args)
{
    int i;
#ifdef MAGICK_TRACE_WORKS
    int count = 0;
#endif
    map<mstring, pair<locktype_enum, void *> > *lockroot = NULL;
    mstring lockname;
    unsigned char hash[ACE_MAXTOKENNAMELEN];
    mLock_Mutex *mlock;
    mLock_Write *wlock;
    mLock_Read *rlock;

    mLock_Mutex maplock("LockMap");
    if (maplock.acquire() < 0)
    {
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "LockMap"));
	return;
    }

    lockroot = &LockMap[ACE_Thread::self()];
    if (lockroot == NULL)
    {
	if (maplock.release() < 0)
	{
	    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
		"MUTEX", "LockMap"));
	}
	return;
    }

    for (i=0; i<args.count()-1; i++)
    {
	if (!lockname.empty())
	    lockname += "::";
	lockname += args[i].AsString();
	rlock = NULL;

	if ((*lockroot).find(lockname) == (*lockroot).end())
	{
	    memset(hash, 0, sizeof(hash));
	    mHASH(const_cast<unsigned char *>(lockname.uc_str()), lockname.length(), hash);
	    rlock = new mLock_Read(reinterpret_cast<const char *>(hash));
	    if (rlock != NULL)
	    {
		if (rlock->acquire() < 0)
		{
		    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
			"READ", lockname.c_str()));
		    delete rlock;
		    rlock = NULL;
		}
		else
		{
		    (*lockroot)[lockname] = pair<locktype_enum, void *>(L_Read, rlock);
		    locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		    tlock[count++].open(L_Read, lockname);
#endif
		}
	    }
	    else
	    {
		LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_OPEN"),
		    "READ", lockname.c_str()));
	    }
	}
    }

    if (!lockname.empty())
	lockname += "::";
    lockname += args[i].AsString();
    rlock = NULL;
    wlock = NULL;
    mlock = NULL;

    if (type == L_Read)
    {
	if ((*lockroot).find(lockname) == (*lockroot).end())
	{
	    memset(hash, 0, sizeof(hash));
	    mHASH(const_cast<unsigned char *>(lockname.uc_str()), lockname.length(), hash);
	    rlock = new mLock_Read(reinterpret_cast<const char *>(hash));
	    if (rlock != NULL)
	    {
		if (rlock->acquire() < 0)
		{
		    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
			"READ", lockname.c_str()));
		    delete rlock;
		    rlock = NULL;
		}
		else
		{
		    (*lockroot)[lockname] = pair<locktype_enum, void *>(L_Read, rlock);
		    locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		    tlock[count++].open(L_Read, lockname);
#endif
		}
	    }
	    else
	    {
		LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_OPEN"),
		    "READ", lockname.c_str()));
	    }
	}
    }
    else if (type == L_Write)
    {
	if ((*lockroot).find(lockname) != (*lockroot).end() &&
	    (*lockroot)[lockname].first == L_Read)
	{
	    rlock = reinterpret_cast<mLock_Read *>((*lockroot)[lockname].second);
	    (*lockroot)[lockname].second = NULL;
	    if (rlock != NULL)
	    {
		if (rlock->release() < 0)
		{
		    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
			"READ", lockname.c_str()));
		}
		delete rlock;
		rlock = NULL;
	    }
	    (*lockroot).erase(lockname);
	}
	if ((*lockroot).find(lockname) == (*lockroot).end())
	{
	    memset(hash, 0, sizeof(hash));
	    mHASH(const_cast<unsigned char *>(lockname.uc_str()), lockname.length(), hash);
	    wlock = new mLock_Write(reinterpret_cast<const char *>(hash));
	    if (wlock != NULL)
	    {
		if (wlock->acquire() < 0)
		{
		    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
			"WRITE", lockname.c_str()));
		    delete wlock;
		    wlock = NULL;
		}
		else
		{
		    (*lockroot)[lockname] = pair<locktype_enum, void *>(L_Write, wlock);
		    locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		    tlock[count++].open(L_Write, lockname);
#endif
		}
	    }
	    else
	    {
		LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_OPEN"),
		    "WRITE", lockname.c_str()));
	    }
	}
    }
    else if (type == L_Mutex)
    {
	if ((*lockroot).find(lockname) != (*lockroot).end() &&
	    (*lockroot)[lockname].first == L_Read)
	{
	    rlock = reinterpret_cast<mLock_Read *>((*lockroot)[lockname].second);
	    (*lockroot)[lockname].second = NULL;
	    if (rlock != NULL)
	    {
		if (rlock->release() < 0)
		{
		    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
			"READ", lockname.c_str()));
		}
		delete rlock;
		rlock = NULL;
	    }
	    (*lockroot).erase(lockname);
	}
	if ((*lockroot).find(lockname) == (*lockroot).end())
	{
	    memset(hash, 0, sizeof(hash));
	    mHASH(const_cast<unsigned char *>(lockname.uc_str()), lockname.length(), hash);
	    mlock = new mLock_Mutex(reinterpret_cast<const char *>(hash));
	    if (mlock != NULL)
	    {
		if (mlock->acquire() < 0)
		{
		    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
			"MUTEX", lockname.c_str()));
		    delete mlock;
		    mlock = NULL;
		}
		else
		{
		    (*lockroot)[lockname] = pair<locktype_enum, void *>(L_Mutex, mlock);
		    locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		    tlock[count++].open(L_Mutex, lockname);
#endif
		}
	    }
	    else
	    {
		LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_OPEN"),
		    "MUTEX", lockname.c_str()));
	    }
	}
    }

    if (maplock.release() < 0)
    {
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
		"MUTEX", "LockMap"));
    }
    memset(hash, 0, sizeof(hash));
}

mLOCK::~mLOCK()
{
    int i;
    map<mstring, pair<locktype_enum, void *> > *lockroot = NULL;
    mLock_Mutex *mlock;
    mLock_Write *wlock;
    mLock_Read *rlock;

    mLock_Mutex maplock("LockMap");
    if (maplock.acquire() < 0)
    {
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "LockMap"));
	return;
    }

    lockroot = &LockMap[ACE_Thread::self()];
    if (lockroot == NULL)
    {
	if (maplock.release() < 0)
	{
	    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
		"MUTEX", "LockMap"));
	}
	return;
    }

    for (i=locks.size()-1; i>=0; i--)
    {
	if ((*lockroot).find(locks[i]) != (*lockroot).end())
	{

	    if ((*lockroot)[locks[i]].first == L_Read)
	    {
		rlock = reinterpret_cast<mLock_Read *>((*lockroot)[locks[i]].second);
		(*lockroot)[locks[i]].second = NULL;
		if (rlock != NULL)
		{
		    if (rlock->release() < 0)
		    {
			LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
				"READ", locks[i].c_str()));
		    }
		    delete rlock;
		    rlock = NULL;
		}
	    }
	    else if ((*lockroot)[locks[i]].first == L_Write)
	    {
		wlock = reinterpret_cast<mLock_Write *>((*lockroot)[locks[i]].second);
		(*lockroot)[locks[i]].second = NULL;
		if (wlock != NULL)
		{
		    if (wlock->release() < 0)
		    {
			LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
				"WRITE", locks[i].c_str()));
		    }
		    delete wlock;
		    wlock = NULL;
		}
	    }
	    else if ((*lockroot)[locks[i]].first == L_Mutex)
	    {
		mlock = reinterpret_cast<mLock_Mutex *>((*lockroot)[locks[i]].second);
		(*lockroot)[locks[i]].second = NULL;
		if (mlock != NULL)
		{
		    if (mlock->release() < 0)
		    {
			LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
				"MUTEX", locks[i].c_str()));
		    }
		    delete mlock;
		    mlock = NULL;
		}
	    }
	    (*lockroot).erase(locks[i]);
	}
    }

    if (maplock.release() < 0)
    {
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
		"MUTEX", "LockMap"));
    }
}

bool mLOCK::Locked() const
{
    bool retval = false;
    if (locks.size())
    {
	map<mstring, pair<locktype_enum, void *> > *lockroot = NULL;

	mLock_Mutex maplock("LockMap");
	if (maplock.acquire() < 0)
	{
	    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "LockMap"));
	    return false;
	}

	lockroot = &LockMap[ACE_Thread::self()];
	if (lockroot == NULL)
	{
	    if (maplock.release() < 0)
	    {
		LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
			"MUTEX", "LockMap"));
	    }
	    return false;
	}

	if ((*lockroot).find(locks[locks.size()-1]) != (*lockroot).end() &&
		(*lockroot)[locks[locks.size()-1]].first != L_Invalid &&
		(*lockroot)[locks[locks.size()-1]].second != NULL)
	    retval = true;
	if (maplock.release() < 0)
	{
	    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
		"MUTEX", "LockMap"));
	}
    }
    return retval;
}

size_t mLOCK::AllLocks()
{
    map<ACE_thread_t, map<mstring, pair<locktype_enum, void *> > >::iterator iter;
    size_t count = 0;

    mLock_Mutex maplock("LockMap");
    if (maplock.acquire() < 0)
    {
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
	    "MUTEX", "LockMap"));
	return false;
    }

    for (iter=LockMap.begin(); iter!=LockMap.end(); iter++)
    {
	count += iter->second.size();
    }

    if (maplock.release() < 0)
    {
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
		"MUTEX", "LockMap"));
    }
    return count;
}

#endif /* MAGICK_LOCKS_WORK */

unsigned short mSocket::FindAvailPort()
{
    NFT("mSocket::FindAvailPort");

    ACE_INET_Addr local;
    ACE_SOCK_Acceptor accept(ACE_Addr::sap_any);
    accept.get_local_addr(local);
    unsigned short retval = local.get_port_number();
    accept.close();
    RET(retval);
}

void mSocket::init()
{
    NFT("mSocket::init");

    sock = NULL;
    last_error = 0;
    sockid = 0;
    DestroyMe = false;

    unsigned long i;
    MLOCK(("SockMap"));
    for (i=1; i>0; i++)
	if (SockMap.find(i) == SockMap.end())
	{
	    SockMap[i] = this;
	    sockid = i;
	    break;
	}
}

mSocket::~mSocket()
{
    NFT("mSocket::mSocket");
    MLOCK(("mSocket", sockid));
    if (sock != NULL)
	close();
    MLOCK2(("SockMap"));
    SockMap.erase(sockid);
}

void mSocket::operator=(const mSocket &in)
{
    FT("mSocket::operator=", ("(const mSocket &) in"));

    MLOCK(("mSocket", in.sockid));
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

    MLOCK2(("SockMap"));
    SockMap[sockid] = this;
}


bool mSocket::Connect(const ACE_INET_Addr &addr, const unsigned long timeout)
{
    FT("mSocket::Connect", ("(ACE_INET_Addr) addr", timeout));

    MLOCK(("mSocket", sockid));
    if (sock != NULL)
	close();

    sock = new ACE_SOCK_Stream;
    if (sock == NULL)
	RET(false);

    DestroyMe = true;
    ACE_Time_Value tv(timeout);
    ACE_SOCK_Connector tmp;
    int result = tmp.connect(*sock, addr, timeout ? &tv : 0);

    if (result < 0)
    {
	last_error = errno;
#ifdef MAGICK_TRACE_WORKS
	trace.Failed(sockid, local.get_port_number(), remote.get_port_number(),
	    mstring(remote.get_host_addr()), Last_Error_String(), D_From);
		
#endif	
	delete sock;
	sock = NULL;
	RET(false);
    }
    last_error = 0;
    sock->get_local_addr(local);
    sock->get_remote_addr(remote);
#ifdef MAGICK_TRACE_WORKS
    trace.Begin(sockid, local.get_port_number(), remote.get_port_number(),
	mstring(remote.get_host_addr()), D_From);
		
#endif	
    RET(true);
}

bool mSocket::Connect(const unsigned long host, const unsigned short port, const unsigned long timeout)
{
    FT("mSocket::Connect", (host, port, timeout));
    ACE_INET_Addr tmp(port, host);
    bool retval = Connect(tmp, timeout);
    RET(retval);
}

bool mSocket::Connect(const mstring &host, const unsigned short port, const unsigned long timeout)
{
    FT("mSocket::Connect", (host, port, timeout));
    ACE_INET_Addr tmp(port, host);
    bool retval = Connect(tmp, timeout);
    RET(retval);
}

bool mSocket::Accept(const unsigned short port, const unsigned long timeout)
{
    FT("mSocket::Accept", (port, timeout));
    ACE_INET_Addr addr(port, Parent->LocalHost());

    MLOCK(("mSocket", sockid));
    if (sock != NULL)
	close();

    sock = new ACE_SOCK_Stream;
    if (sock == NULL)
	RET(false);

    DestroyMe = true;
    ACE_Time_Value tv(timeout);
    ACE_SOCK_Acceptor tmp(addr);
    int result = tmp.accept(*sock, NULL, timeout ? &tv : 0);

    if (result < 0)
    {
	last_error = errno;
#ifdef MAGICK_TRACE_WORKS
	trace.Failed(sockid, local.get_port_number(), remote.get_port_number(),
	    mstring(remote.get_host_addr()), Last_Error_String(), D_To);
#endif	
	delete sock;
	sock = NULL;
	RET(false);
    }

    last_error = 0;
    sock->get_local_addr(local);
    sock->get_remote_addr(remote);
#ifdef MAGICK_TRACE_WORKS
    trace.Begin(sockid, local.get_port_number(), remote.get_port_number(),
	mstring(remote.get_host_addr()), D_To);
#endif	
    RET(true);
}

bool mSocket::Bind(ACE_SOCK_Stream *in, const dir_enum direction, const bool alloc)
{
    FT("mSocket::Bind", ("(ACE_SOCK_Stream *) in"));
    if (in == NULL)
	RET(false);

    MLOCK(("mSocket", sockid));
    if (sock != NULL)
	close();

    sock = in;
    in = NULL;
    DestroyMe = alloc;
    last_error = 0;
    sock->get_local_addr(local);
    sock->get_remote_addr(remote);

#ifdef MAGICK_TRACE_WORKS
    trace.Begin(sockid, local.get_port_number(), remote.get_port_number(),
	mstring(remote.get_host_addr()), direction);
#endif	

    RET(true);
}

ACE_SOCK_Stream *mSocket::Unbind()
{
    NFT("mSocket::Unbind");
    MLOCK(("mSocket", sockid));
#ifdef MAGICK_TRACE_WORKS
    trace.End(Last_Error_String());
#endif
    ACE_SOCK_Stream *retval = sock;
    sock = NULL;
    last_error = 0;
    NRET(ACE_SOCK_Stream *, retval);
}

mstring mSocket::Local_Host() const
{
    NFT("mSocket::Local_Host");
    MLOCK(("mSocket", sockid));
    mstring retval(local.get_host_addr());
    RET(retval);
}

unsigned long mSocket::Local_IP() const
{
    NFT("mSocket::Local_IP");
    MLOCK(("mSocket", sockid));
    unsigned long retval = local.get_ip_address();
    RET(retval);
}

unsigned short mSocket::Local_Port() const
{
    NFT("mSocket::Local_Port");
    MLOCK(("mSocket", sockid));
    unsigned short retval = local.get_port_number();
    RET(retval);
}

mstring mSocket::Remote_Host() const
{
    NFT("mSocket::Remote_Host");
    MLOCK(("mSocket", sockid));
    mstring retval = remote.get_host_addr();
    RET(retval);
}

unsigned long mSocket::Remote_IP() const
{
    NFT("mSocket::Remote_IP");
    MLOCK(("mSocket", sockid));
    unsigned long retval = remote.get_ip_address();
    RET(retval);
}

unsigned short mSocket::Remote_Port() const
{
    NFT("mSocket::Remote_Port");
    MLOCK(("mSocket", sockid));
    unsigned short retval = remote.get_port_number();
    RET(retval);
}

bool mSocket::IsConnected() const
{
    NFT("mSocket::IsConnected");
    MLOCK(("mSocket", sockid));
    RET(sock != NULL);
}

void mSocket::Resolve(const socktype_enum type, const mstring &info)
{
    FT("mSocket::Resolve", (static_cast<int>(type), info));
    MLOCK(("mSocket", sockid));
#ifdef MAGICK_TRACE_WORKS
    trace.Resolve(type, info);
#endif
}

int mSocket::Last_Error() const
{
    NFT("mSocket::Last_Error");
    MLOCK(("mSocket", sockid));
    RET(last_error);
}

mstring mSocket::Last_Error_String() const
{
    NFT("mSocket::Last_Error_String");
    MLOCK(("mSocket", sockid));
    mstring retval = strerror(last_error);
    RET(retval);
}

ssize_t mSocket::send(void *buf, const size_t len, const unsigned long timeout)
{
    FT("mSocket::send", ("(void *) buf", len, timeout));
    ACE_Time_Value tv(timeout);
    MLOCK(("mSocket", sockid));
    ssize_t retval = sock->send(buf, len, timeout ? &tv : 0);

    if (retval < 0)
    {
	last_error = errno;
	// close();
    }
    else
	last_error = 0;

    RET(retval);
}

ssize_t mSocket::recv(void *buf, const size_t len, const unsigned long timeout)
{
    FT("mSocket::recv", ("(void *) buf", len, timeout));
    ACE_Time_Value tv(timeout);
    MLOCK(("mSocket", sockid));
    ssize_t retval = sock->recv(buf, len, timeout ? &tv : 0);

    if (retval < 0)
    {
	last_error = errno;
	// close();
    }
    else
	last_error = 0;

    RET(retval);
}

int mSocket::close()
{
    NFT("mSocket::close");
    int retval = 0;
    MLOCK(("mSocket", sockid));
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
}

mThread::selftothreadidmap_t mThread::selftothreadidmap;

ThreadID* mThread::find(const ACE_thread_t thread)
{
    ThreadID *tid = NULL;
    mLock_Mutex lock("SelfToThreadMap");
    if (lock.acquire() < 0)
    {
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "SelfToThreadMap"));
	return tid;
    }
    if(selftothreadidmap.find(thread) != selftothreadidmap.end())
    {
	tid = selftothreadidmap[thread];
    }
    lock.release();
    return tid;
}

vector<ThreadID*> mThread::findall()
{
    vector<ThreadID*> threadlist;
    selftothreadidmap_t::const_iterator iter;
    mLock_Mutex lock("SelfToThreadMap");
    if (lock.acquire() < 0)
    {
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "SelfToThreadMap"));
	return threadlist;
    }
    for (iter=selftothreadidmap.begin(); iter!=selftothreadidmap.end(); iter++)
	threadlist.push_back(iter->second);
    lock.release();

    return threadlist;
}

void mThread::Attach(const threadtype_enum ttype)
{
    FT("mThread::Attach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid=new ThreadID(ttype);
    mLock_Mutex lock("SelfToThreadMap");
    if (lock.acquire() < 0)
    {
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "SelfToThreadMap"));
	return;
    }
    selftothreadidmap[ACE_Thread::self()]=tmpid;
    lock.release();
    COM(("Thread ID has been attached."));
}

void mThread::Detach()
{
    NFT("mThread::Detach");
    ThreadID *tmpid=find();
    if(tmpid==NULL)
    {
	// todo ttype to typename
	CP(("mThread::Detach without valid mThread::Attach..."));
	return;
    }
    mLock_Mutex lock("SelfToThreadMap");
    if (lock.acquire() < 0)
    {
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "SelfToThreadMap"));
	return;
    }
    selftothreadidmap.erase(ACE_Thread::self());
    lock.release();
    delete tmpid;
    COM(("Thread ID has been detached."));
}

void mThread::ReAttach(const threadtype_enum ttype)
{
    FT("mThread::ReAttach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid=find();
    if(tmpid==NULL)
    {
	// ReAttach does an attach if it wasnt there
	CP(("mThread::ReAttach without valid mThread::Attach... type: %s",threadname[ttype].c_str()));
	mLock_Mutex lock("SelfToThreadMap");
	if (lock.acquire() < 0)
	{
	    LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "SelfToThreadMap"));
	    return;
	}
	tmpid=new ThreadID();
	selftothreadidmap[ACE_Thread::self()]=tmpid;
	lock.release();
    }
#ifdef MAGICK_TRACE_WORKS	// Get rid of 'unused veriable' warning
    threadtype_enum oldtype = tmpid->type();
#endif
    COM(("Thread ID has been re-attached to %s.", threadname[ttype].c_str()));
    tmpid->assign(ttype);
    COM(("Thread ID has been re-attached from %s.", threadname[oldtype].c_str()));
}
