#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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
#include "lockable.h"
#include "utils.h"

#ifdef MAGICK_LOCKS_WORK

map<ACE_thread_t, map<mstring, pair<locktype_enum, void *> > > mLOCK::LockMap;

typedef ACE_RW_Thread_Mutex	mLock_Read;
typedef ACE_RW_Thread_Mutex	mLock_Write;
typedef ACE_Thread_Mutex	mLock_Mutex;

mLOCK::mLOCK(locktype_enum type, const mVarArray &args)
{
    int i, count = 0;
    map<mstring, pair<locktype_enum, void *> > *lockroot = NULL;
    mstring lockname;
    unsigned char hash[ACE_MAXTOKENNAMELEN];
    mLock_Mutex *mlock;
    mLock_Write *wlock;
    mLock_Read *rlock;

    mLock_Mutex maplock("LockMap");
    if (maplock.acquire() < 0)
    {
	Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "LockMap");
	return;
    }

    lockroot = &LockMap[ACE_Thread::self()];
    if (lockroot == NULL)
    {
	if (maplock.release() < 0)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
		"MUTEX", "LockMap");
	return;
    }

    for (i=0; i<args.count()-1; i++)
    {
	if (lockname != "")
	    lockname += "::";
	lockname += args[i].AsString();
	rlock = NULL;

	if ((*lockroot).find(lockname) == (*lockroot).end())
	{
	    ACE_OS::memset(hash, 0, sizeof(hash));
	    mHASH((unsigned char *) lockname.c_str(), lockname.Len(), hash);
	    rlock = new mLock_Read((const char *) hash);
	    if (rlock != NULL)
	    {
		if (rlock->acquire_read() < 0)
		{
		    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
			"READ", lockname.c_str());
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
		Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_OPEN"),
		    "READ", lockname.c_str());
	}
    }

    if (lockname != "")
	lockname += "::";
    lockname += args[i].AsString();
    rlock = NULL;
    wlock = NULL;
    mlock = NULL;

    if (type == L_Read)
    {
	if ((*lockroot).find(lockname) == (*lockroot).end())
	{
	    ACE_OS::memset(hash, 0, sizeof(hash));
	    mHASH((unsigned char *) lockname.c_str(), lockname.Len(), hash);
	    rlock = new mLock_Read((const char *) hash);
	    if (rlock != NULL)
	    {
		if (rlock->acquire_read() < 0)
		{
		    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
			"READ", lockname.c_str());
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
		Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_OPEN"),
		    "READ", lockname.c_str());
	}
    }
    else if (type == L_Write)
    {
	if ((*lockroot).find(lockname) != (*lockroot).end() &&
	    (*lockroot)[lockname].first == L_Read)
	{
	    rlock = (mLock_Read *) (*lockroot)[lockname].second;
	    if (rlock != NULL)
	    {
		if (rlock->release() < 0)
		    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
			"READ", lockname.c_str());
		delete rlock;
		rlock = NULL;
	    }
	    (*lockroot).erase(lockname);
	}
	if ((*lockroot).find(lockname) == (*lockroot).end())
	{
	    ACE_OS::memset(hash, 0, sizeof(hash));
	    mHASH((unsigned char *) lockname.c_str(), lockname.Len(), hash);
	    wlock = new mLock_Write((const char *) hash);
	    if (wlock != NULL)
	    {
		if (wlock->acquire_write() < 0)
		{
		    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
			"WRITE", lockname.c_str());
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
		Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_OPEN"),
		    "WRITE", lockname.c_str());
	}
    }
    else if (type == L_Mutex)
    {
	if ((*lockroot).find(lockname) != (*lockroot).end() &&
	    (*lockroot)[lockname].first == L_Read)
	{
	    rlock = (mLock_Read *) (*lockroot)[lockname].second;
	    if (rlock != NULL)
	    {
		if (rlock->release() < 0)
		    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
			"READ", lockname.c_str());
		delete rlock;
		rlock = NULL;
	    }
	    (*lockroot).erase(lockname);
	}
	if ((*lockroot).find(lockname) == (*lockroot).end())
	{
	    ACE_OS::memset(hash, 0, sizeof(hash));
	    mHASH((unsigned char *) lockname.c_str(), lockname.Len(), hash);
	    mlock = new mLock_Mutex((const char *) hash);
	    if (mlock != NULL)
	    {
		if (mlock->acquire() < 0)
		{
		    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
			"MUTEX", lockname.c_str());
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
		Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_OPEN"),
		    "MUTEX", lockname.c_str());
	}
    }
    if (maplock.release() < 0)
	Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
		"MUTEX", "LockMap");
    ACE_OS::memset(hash, 0, sizeof(hash));
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
	Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "LockMap");
	return;
    }

    lockroot = &LockMap[ACE_Thread::self()];
    if (lockroot == NULL)
    {
	if (maplock.release() < 0)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
		"MUTEX", "LockMap");
	return;
    }

    for (i=locks.size()-1; i>=0; i--)
    {
	if ((*lockroot).find(locks[i]) != (*lockroot).end())
	{
	    if ((*lockroot)[locks[i]].first == L_Read)
	    {
		rlock = (mLock_Read *) (*lockroot)[locks[i]].second;
		if (rlock != NULL)
		{
		    if (rlock->release() < 0)
			Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
				"READ", locks[i].c_str());
		    delete rlock;
		    rlock = NULL;
		}
	    }
	    else if ((*lockroot)[locks[i]].first == L_Write)
	    {
		wlock = (mLock_Write *) (*lockroot)[locks[i]].second;
		if (wlock != NULL)
		{
		    if (wlock->release() < 0)
			Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
				"WRITE", locks[i].c_str());
		    delete wlock;
		    wlock = NULL;
		}
	    }
	    else if ((*lockroot)[locks[i]].first == L_Mutex)
	    {
		mlock = (mLock_Mutex *) (*lockroot)[locks[i]].second;
		if (mlock != NULL)
		{
		    if (mlock->release() < 0)
			Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
				"MUTEX", locks[i].c_str());
		    delete mlock;
		    mlock = NULL;
		}
	    }
	    (*lockroot).erase(locks[i]);
	}
    }
    maplock.release();
}

bool mLOCK::Locked()
{
    bool retval = false;
    if (locks.size())
    {
	map<mstring, pair<locktype_enum, void *> > *lockroot = NULL;

	mLock_Mutex maplock("LockMap");
	if (maplock.acquire() < 0)
	{
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
		"MUTEX", "LockMap");
	    return false;
	}

	lockroot = &LockMap[ACE_Thread::self()];
	if (lockroot == NULL)
	{
	    if (maplock.release() < 0)
		Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_RELEASE"),
			"MUTEX", "LockMap");
	    return false;
	}

	if ((*lockroot).find(locks[locks.size()-1]) != (*lockroot).end() &&
		(*lockroot)[locks[locks.size()-1]].first != L_Invalid &&
		(*lockroot)[locks[locks.size()-1]].second != NULL)
	    retval = true;
	maplock.release();
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
	Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/LOCK_ACQUIRE"),
	    "MUTEX", "LockMap");
	return false;
    }

    for (iter=LockMap.begin(); iter!=LockMap.end(); iter++)
    {
	count += iter->second.size();
    }

    maplock.release();
    return count;
}

#endif /* MAGICK_LOCKS_WORK */

mThread::selftothreadidmap_t mThread::selftothreadidmap;

ThreadID* mThread::find(ACE_thread_t thread)
{
    if(selftothreadidmap.find(thread)!=selftothreadidmap.end()) {
	return selftothreadidmap[thread];
    }
    return NULL;
}

vector<ThreadID*> mThread::findall()
{
    vector<ThreadID*> threadlist;
    selftothreadidmap_t::iterator iter;
    for (iter=selftothreadidmap.begin(); iter!=selftothreadidmap.end(); iter++)
	threadlist.push_back(iter->second);

    return threadlist;
}

void mThread::Attach(threadtype_enum ttype)
{
    FT("mThread::Attach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid=new ThreadID(ttype);
    selftothreadidmap[ACE_Thread::self()]=tmpid;
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
    selftothreadidmap.erase(ACE_Thread::self());
    delete tmpid;
    COM(("Thread ID has been detached."));
}

void mThread::ReAttach(threadtype_enum ttype)
{
    FT("mThread::ReAttach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid=find();
    threadtype_enum oldtype = tmpid->type();
    if(tmpid==NULL)
    {
	// ReAttach does an attach if it wasnt there
	CP(("mThread::ReAttach without valid mThread::Attach... type: %s",threadname[ttype].c_str()));
	tmpid=new ThreadID();
    }
    COM(("Thread ID has been re-attached to %s.", threadname[ttype].c_str()));
    tmpid->assign(ttype);
    COM(("Thread ID has been re-attached from %s.", threadname[oldtype].c_str()));
}
