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

mLOCK::mLOCK(T_Locking::type_enum type, const mVarArray &args)
{
    int i;
    count=0;
    for (i=0; i<args.count()-1; i++)
    {
	if (lockname != "")
	    lockname += "::";
	lockname += args[i].AsString();
	lock[i] = NULL;
	lock[i] = new ACE_RW_Thread_Mutex(lockname.c_str());
	if (lock[i] == NULL)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_OPEN"),
		"READ", lockname.c_str());
	else if (lock[i]->acquire_read() < 0)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_ACQUIRE"),
		"READ", lockname.c_str());
	else count++;
	tlock[i].open(T_Locking::Read, lockname);
    }

    if (lockname != "")
	lockname += "::";
    lockname += args[i].AsString();

    if (type == T_Locking::Read)
    {
	rwlock = NULL;
	rwlock = new ACE_RW_Thread_Mutex(lockname.c_str());
	if (rwlock == NULL)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_OPEN"),
		"READ", lockname.c_str());
	else if (rwlock->acquire_read() < 0)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_ACQUIRE"),
		"READ", lockname.c_str());
	else count++;
    }
    else if (type == T_Locking::Write)
    {
	rwlock = NULL;
	rwlock = new ACE_RW_Thread_Mutex(lockname.c_str());
	if (rwlock == NULL)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_OPEN"),
		"WRITE", lockname.c_str());
	else if (rwlock->acquire_write() < 0)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_ACQUIRE"),
		"WRITE", lockname.c_str());
	else count++;
    }
    else if (type == T_Locking::Mutex)
    {
	mlock = NULL;
	mlock = new ACE_Recursive_Thread_Mutex(lockname.c_str());
	if (mlock == NULL)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_OPEN"),
		"MUTEX", lockname.c_str());
	else if (mlock->acquire() < 0)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_ACQUIRE"),
		"MUTEX", lockname.c_str());
	else count++;
    }
    else
    {
	// Unknown Lock Type
    }
    tlock[i].open(type, lockname);
    last_type = type;
}

mLOCK::~mLOCK()
{
    if (last_type == T_Locking::Read && rwlock != NULL)
    {
	if (rwlock->release() < 0)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_RELEASE"),
		"READ", lockname.c_str());
	else
	{
	    delete rwlock;
	    count--;
	}
    }
    else if (last_type == T_Locking::Write && rwlock != NULL)
    {
	if (rwlock->release() < 0)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_RELEASE"),
		"WRITE", lockname.c_str());
	else
	{
	    delete rwlock;
	    count--;
	}
    }
    else if (last_type == T_Locking::Mutex && mlock != NULL)
    {
	if (mlock->release() < 0)
	    Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_RELEASE"),
		"MUTEX", lockname.c_str());
	else
	{
	    delete mlock;
	    count--;
	}
    }
    else
    {
	// Unknown lock type
    }
    for(;count;count--)
    {
	if (lockname.Contains("::"))
	    lockname.Truncate(lockname.Find(':', true)-1);
	if (lock[count-1] != NULL)
	{
	    if (lock[count-1]->release() < 0)
		Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/LOCK_RELEASE"),
			"READ", lockname.c_str());
	    else
		delete lock[count-1];
	}
    }
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
