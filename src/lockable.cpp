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

void mThread::spawn(threadtype_enum type,ACE_THR_FUNC func, void *arg)
{
    //todo change the below to a triplet
    pair<ACE_THR_FUNC,void *> *args=new pair<ACE_THR_FUNC,void *>;
    args->first=func;
    args->second=arg;
    ACE_Thread::spawn(handler_hack,(void *)args);
}
void mThread::resumeself()
{
    ACE_hthread_t temp;
    ACE_Thread::self(temp);
    ACE_Thread::resume(temp);
}
void mThread::suspendself()
{
    ACE_hthread_t temp;
    ACE_Thread::self(temp);
    ACE_Thread::suspend(temp);
}
void mThread::resume(ThreadID* tid)
{
}
void mThread::resume(ACE_thread_t tid)
{
}
void mThread::suspend(ThreadID* tid)
{
}
void mThread::suspend(ACE_thread_t tid)
{
}
void mThread::yieldself()
{
    ACE_Thread::yield();
}
void *mThread::handler_hack(void *level)
{
    void *Result;
    //todo change the below to a triplet
    pair<ACE_THR_FUNC,void *> *args;
    args=(pair<ACE_THR_FUNC,void *> *)level;
    //todo: add ourselves to the maps
    Result=(*(args->first))((args->second));
    delete args;
    //todo remove ourselves from the maps

    return Result;
}
