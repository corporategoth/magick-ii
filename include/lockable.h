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

#ifndef _LOCKABLE_H
#define _LOCKABLE_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <ace/Local_Tokens.h>
#include <ace/Thread.h>
#include "mstring.h"
#include "trace.h"
#include <map>
using namespace std;

class RLOCK
{
    ACE_Local_RLock lock[5];

    T_Locking tlock[5];
    int count;
public:
    RLOCK(mstring x1);
    RLOCK(mstring x1, mstring x2);
    RLOCK(mstring x1, mstring x2, mstring x3);
    RLOCK(mstring x1, mstring x2, mstring x3, mstring x4);
    RLOCK(mstring x1, mstring x2, mstring x3, mstring x4, mstring x5);
    ~RLOCK();
};

class WLOCK
{
    ACE_Local_WLock wlock;
    ACE_Local_RLock lock[4];
    T_Locking tlock[5];

    int count;
public:
    WLOCK(mstring x1);
    WLOCK(mstring x1, mstring x2);
    WLOCK(mstring x1, mstring x2, mstring x3);
    WLOCK(mstring x1, mstring x2, mstring x3, mstring x4);
    WLOCK(mstring x1, mstring x2, mstring x3, mstring x4, mstring x5);
    ~WLOCK();
};

class MLOCK
{
    ACE_Local_Mutex mlock;
    ACE_Local_RLock lock[4];
    T_Locking tlock[5];

    int count;
public:
    MLOCK(mstring x1);
    MLOCK(mstring x1, mstring x2);
    MLOCK(mstring x1, mstring x2, mstring x3);
    MLOCK(mstring x1, mstring x2, mstring x3, mstring x4);
    MLOCK(mstring x1, mstring x2, mstring x3, mstring x4, mstring x5);
    ~MLOCK();
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
    typedef map<pair<threadtype_enum,int>,ThreadID*> threadtypetothreadidmap_t; 
    static threadtypetothreadidmap_t threadtypetothreadidmap;
    typedef map<threadtype_enum,int> threadtypecountmap_t;
    static threadtypecountmap_t threadtypecountmap;
    static void *handler_hack(void *level);
public:
    static void spawn(threadtype_enum type,ACE_THR_FUNC func, void *arg=0);
    static void resumeself();
    static void suspendself();
    static void resume(ThreadID* tid);
    static void resume(ACE_thread_t tid);
    static void suspend(ThreadID* tid);
    static void suspend(ACE_thread_t tid=ACE_Thread::self());
    static void yieldself();
    static ThreadID* find(ACE_thread_t thread=ACE_Thread::self());
    static int typecount(threadtype_enum ttype);
    static ThreadID* findbytype(threadtype_enum ttype, int level=1);
    static void Attach(threadtype_enum ttype, int level);
    static void Detach(threadtype_enum ttype, int level);
};

#endif