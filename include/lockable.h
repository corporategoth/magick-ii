#ifndef WIN32
#pragma interface
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
#ifndef _LOCKABLE_H
#define _LOCKABLE_H
#include "pch.h"
static const char *ident_lockable_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.33  2000/06/06 08:57:54  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.32  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.31  2000/05/20 03:28:10  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.30  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.29  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#include "mstring.h"
#include "trace.h"

#ifdef MAGICK_LOCKS_WORK
#define MAX_LOCKS 5
class mLOCK
{
    mstring lockname;
    // union {
    	ACE_Local_Mutex mlock;
    	ACE_Local_WLock wlock;
    	ACE_Local_RLock rlock;
    // } llock;
    ACE_Local_RLock lock[MAX_LOCKS-1];
    T_Locking tlock[MAX_LOCKS];
    T_Locking::type_enum last_type;

    int count;
public:
    mLOCK(T_Locking::type_enum, const mVarArray &args);
    ~mLOCK();
};

#define RLOCK(y)   mVarArray __lockR1_VarArray y; mLOCK __lockR1(T_Locking::Read,  __lockR1_VarArray)
#define RLOCK2(y)  mVarArray __lockR2_VarArray y; mLOCK __lockR2(T_Locking::Read,  __lockR2_VarArray)
#define RLOCK3(y)  mVarArray __lockR3_VarArray y; mLOCK __lockR3(T_Locking::Read,  __lockR3_VarArray)
#define RLOCK4(y)  mVarArray __lockR4_VarArray y; mLOCK __lockR4(T_Locking::Read,  __lockR4_VarArray)
#define RLOCK5(y)  mVarArray __lockR5_VarArray y; mLOCK __lockR5(T_Locking::Read,  __lockR5_VarArray)
#define WLOCK(y)   mVarArray __lockW1_VarArray y; mLOCK __lockW1(T_Locking::Write, __lockW1_VarArray)
#define WLOCK2(y)  mVarArray __lockW2_VarArray y; mLOCK __lockW2(T_Locking::Write, __lockW2_VarArray)
#define WLOCK3(y)  mVarArray __lockW3_VarArray y; mLOCK __lockW3(T_Locking::Write, __lockW3_VarArray)
#define WLOCK4(y)  mVarArray __lockW4_VarArray y; mLOCK __lockW4(T_Locking::Write, __lockW4_VarArray)
#define WLOCK5(y)  mVarArray __lockW5_VarArray y; mLOCK __lockW5(T_Locking::Write, __lockW5_VarArray)
#define MLOCK(y)   mVarArray __lockM1_VarArray y; mLOCK __lockM1(T_Locking::Mutex, __lockM1_VarArray)
#define MLOCK2(y)  mVarArray __lockM2_VarArray y; mLOCK __lockM2(T_Locking::Mutex, __lockM2_VarArray)
#define MLOCK3(y)  mVarArray __lockM3_VarArray y; mLOCK __lockM3(T_Locking::Mutex, __lockM3_VarArray)
#define MLOCK4(y)  mVarArray __lockM4_VarArray y; mLOCK __lockM4(T_Locking::Mutex, __lockM4_VarArray)
#define MLOCK5(y)  mVarArray __lockM5_VarArray y; mLOCK __lockM5(T_Locking::Mutex, __lockM5_VarArray)

#else /* MAGICK_LOCKS_WORK */
inline void do_nothing() {}

#define RLOCK(y)   do_nothing()
#define RLOCK2(y)  do_nothing()
#define RLOCK3(y)  do_nothing()
#define RLOCK4(y)  do_nothing()
#define RLOCK5(y)  do_nothing()
#define WLOCK(y)   do_nothing()
#define WLOCK2(y)  do_nothing()
#define WLOCK3(y)  do_nothing()
#define WLOCK4(y)  do_nothing()
#define WLOCK5(y)  do_nothing()
#define MLOCK(y)   do_nothing()
#define MLOCK2(y)  do_nothing()
#define MLOCK3(y)  do_nothing()
#define MLOCK4(y)  do_nothing()
#define MLOCK5(y)  do_nothing()

#endif /* MAGICK_LOCKS_WORK */

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
    static void Detach();
    static void ReAttach(threadtype_enum ttype);
};

#endif
