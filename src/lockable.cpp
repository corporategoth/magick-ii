#include "pch.h"
#ifdef WIN32
#pragma hdrstop
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
#ifndef WIN32
#pragma ident "$Id$"
#endif
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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


#include "lockable.h"
#include "utils.h"

#ifdef MAGICK_LOCKS_WORK

mLOCK::mLOCK(T_Locking::type_enum type, const mVarArray &args)
{
    int i;
    mstring lockname;
    for (i=0; i<args.count()-1; i++) {
	if (lockname != "")
	    lockname += "::";
	lockname += args[i].AsString();
	lock[i].open(lockname.c_str());
	lock[i].acquire();
	tlock[i].open(T_Locking::Read, lockname);
    }

    if (lockname != "")
	lockname += "::";
    lockname += args[i].AsString();

    if (type == T_Locking::Write)
    {
	wlock.open(lockname.c_str());
	wlock.acquire();
    }
    else if (type == T_Locking::Mutex)
    {
	mlock.open(lockname.c_str());
	mlock.acquire();
    }
    else
    {
	rlock.open(lockname.c_str());
	rlock.acquire();
    }
    tlock[i].open(type, lockname);
    last_type = type;

    count=i+1;
}

mLOCK::~mLOCK()
{
    if (last_type == T_Locking::Write)
	wlock.release();
    else if (last_type == T_Locking::Mutex)
	mlock.release();
    else
	rlock.release();
    for(;count;count--)
	lock[count-1].release();
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

void mThread::Detach(threadtype_enum ttype)
{
    FT("mThread::Detach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid=find();
    if(tmpid==NULL)
    {
	// todo ttype to typename
	CP(("mThread::Detach without valid mThread::Attach... type: %s",threadname[ttype].c_str()));
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
