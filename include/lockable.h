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

#include "mstring.h"
#include "trace.h"

class mLOCK
{
    // union {
    	ACE_Local_Mutex mlock;
    	ACE_Local_WLock wlock;
    	ACE_Local_RLock rlock;
    // } llock;
    ACE_Local_RLock lock[14];
    T_Locking tlock[15];
    T_Locking::type_enum last_type;

    int count;
public:
    mLOCK(T_Locking::type_enum, const mVarArray &args);
    ~mLOCK();
};

#define RLOCK(y)  mVarArray __lock_VarArray y; mLOCK __lock(T_Locking::Read, __lock_VarArray)
#define WLOCK(y)  mVarArray __lock_VarArray y; mLOCK __lock(T_Locking::Write, __lock_VarArray)
#define MLOCK(y)  mVarArray __lock_VarArray y; mLOCK __lock(T_Locking::Mutex, __lock_VarArray)

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
public:
    static ThreadID* find(ACE_thread_t thread=ACE_Thread::self());
    static vector<ThreadID*> findall();
    static size_t size() { return selftothreadidmap.size(); }
    static void Attach(threadtype_enum ttype);
    static void Detach(threadtype_enum ttype);
    static void ReAttach(threadtype_enum ttype);
};

#endif
