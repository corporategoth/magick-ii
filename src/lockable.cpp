#include "pch.h"
#ifdef WIN32
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

RLOCK::RLOCK(mstring x1)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    count=1;
}

RLOCK::RLOCK(mstring x1, mstring x2)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    tlock[1].open(T_Locking::Read, x1+"::"+x2);

    count=2;
}

RLOCK::RLOCK(mstring x1, mstring x2, mstring x3)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    tlock[1].open(T_Locking::Read, x1+"::"+x2);

    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    tlock[2].open(T_Locking::Read, x1+"::"+x2+"::"+x3);

    count=3;
}

RLOCK::RLOCK(mstring x1, mstring x2, mstring x3, mstring x4)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    tlock[1].open(T_Locking::Read, x1+"::"+x2);

    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    tlock[2].open(T_Locking::Read, x1+"::"+x2+"::"+x3);

    lock[3].open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    lock[3].acquire();
    tlock[3].open(T_Locking::Read, x1+"::"+x2+"::"+x3+"::"+x4);

    count=4;
}

RLOCK::RLOCK(mstring x1, mstring x2, mstring x3, mstring x4, mstring x5)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    tlock[1].open(T_Locking::Read, x1+"::"+x2);

    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    tlock[2].open(T_Locking::Read, x1+"::"+x2+"::"+x3);

    lock[3].open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    lock[3].acquire();
    tlock[3].open(T_Locking::Read, x1+"::"+x2+"::"+x3+"::"+x4);

    lock[4].open((x1+"::"+x2+"::"+x3+"::"+x4+"::"+x5).c_str());
    lock[4].acquire();
    tlock[4].open(T_Locking::Read, x1+"::"+x2+"::"+x3+"::"+x4+"::"+x5);

    count=5;
}

RLOCK::~RLOCK()
{
    for(;count;count--)
	lock[count-1].release();
}

WLOCK::WLOCK(mstring x1)
{
    wlock.open(x1.c_str());
    wlock.acquire();
    tlock[0].open(T_Locking::Write, x1);

    count=0;
}

WLOCK::WLOCK(mstring x1, mstring x2)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    wlock.open((x1+"::"+x2).c_str());
    wlock.acquire();
    tlock[1].open(T_Locking::Write, x1+"::"+x2);

    count=1;
}

WLOCK::WLOCK(mstring x1, mstring x2, mstring x3)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    tlock[1].open(T_Locking::Read, x1+"::"+x2);

    wlock.open((x1+"::"+x2+"::"+x3).c_str());
    wlock.acquire();
    tlock[2].open(T_Locking::Write, x1+"::"+x2+"::"+x3);

    count=2;
}

WLOCK::WLOCK(mstring x1, mstring x2, mstring x3, mstring x4)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    tlock[1].open(T_Locking::Read, x1+"::"+x2);

    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    tlock[2].open(T_Locking::Read, x1+"::"+x2+"::"+x3);

    wlock.open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    wlock.acquire();
    tlock[3].open(T_Locking::Write, x1+"::"+x2+"::"+x3+"::"+x4);

    count=3;
}

WLOCK::WLOCK(mstring x1, mstring x2, mstring x3, mstring x4, mstring x5)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    tlock[1].open(T_Locking::Read, x1+"::"+x2);

    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    tlock[2].open(T_Locking::Read, x1+"::"+x2+"::"+x3);

    lock[3].open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    lock[3].acquire();
    tlock[3].open(T_Locking::Read, x1+"::"+x2+"::"+x3+"::"+x4);

    wlock.open((x1+"::"+x2+"::"+x3+"::"+x4+"::"+x5).c_str());
    wlock.acquire();
    tlock[4].open(T_Locking::Write, x1+"::"+x2+"::"+x3+"::"+x4+"::"+x5);

    count=4;
}

WLOCK::~WLOCK()
{
    wlock.release();
    for(;count;count--)
	lock[count-1].release();
}

MLOCK::MLOCK(mstring x1)
{
    mlock.open(x1.c_str());
    mlock.acquire();
    tlock[0].open(T_Locking::Mutex, x1);

    count=0;
}

MLOCK::MLOCK(mstring x1, mstring x2)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    mlock.open((x1+"::"+x2).c_str());
    mlock.acquire();
    tlock[1].open(T_Locking::Mutex, x1+"::"+x2);

    count=1;
}

MLOCK::MLOCK(mstring x1, mstring x2, mstring x3)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    tlock[1].open(T_Locking::Read, x1+"::"+x2);

    mlock.open((x1+"::"+x2+"::"+x3).c_str());
    mlock.acquire();
    tlock[2].open(T_Locking::Mutex, x1+"::"+x2+"::"+x3);

    count=2;
}

MLOCK::MLOCK(mstring x1, mstring x2, mstring x3, mstring x4)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    tlock[1].open(T_Locking::Read, x1+"::"+x2);

    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    tlock[2].open(T_Locking::Read, x1+"::"+x2+"::"+x3);

    mlock.open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    mlock.acquire();
    tlock[3].open(T_Locking::Mutex, x1+"::"+x2+"::"+x3+"::"+x4);

    count=3;
}

MLOCK::MLOCK(mstring x1, mstring x2, mstring x3, mstring x4, mstring x5)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    tlock[0].open(T_Locking::Read, x1);

    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    tlock[0].open(T_Locking::Read, x1+"::"+x2);

    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    tlock[0].open(T_Locking::Read, x1+"::"+x2+"::"+x3);

    lock[3].open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    lock[3].acquire();
    tlock[0].open(T_Locking::Read, x1+"::"+x2+"::"+x3+"::"+x4);

    mlock.open((x1+"::"+x2+"::"+x3+"::"+x4+"::"+x5).c_str());
    mlock.acquire();
    tlock[4].open(T_Locking::Mutex, x1+"::"+x2+"::"+x3+"::"+x4+"::"+x5);

    count=4;
}

MLOCK::~MLOCK()
{
    mlock.release();
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

void mThread::Attach(threadtype_enum ttype)
{
    FT("mThread::Attach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid=new ThreadID(ttype);
    selftothreadidmap[ACE_Thread::self()]=tmpid;
    COM(("%s Thread ID has been attached.", threadname[ttype].c_str()));
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
    COM(("%s Thread ID has been detached.", threadname[ttype].c_str()));
}

void mThread::ReAttach(threadtype_enum ttype)
{
    FT("mThread::ReAttach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid=find();
    if(tmpid==NULL)
    {
	// ReAttach does an attach if it wasnt there
	CP(("mThread::ReAttach without valid mThread::Attach... type: %s",threadname[ttype].c_str()));
	tmpid=new ThreadID();
    }
    COM(("%s Thread ID has been re-attached to %s.", threadname[tmpid->type()].c_str(),
	threadname[ttype].c_str()));
    tmpid->assign(ttype);
}
