#include "pch.h"
#ifdef _MSC_VER
#pragma hdrstop
#endif
// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================

#include "lockable.h"
#include "utils.h"

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
