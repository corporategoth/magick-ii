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
#include <ace/Local_Tokens.h>
#include "mstring.h"

#if 0
#define RLOCK(x1) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1);

#define RLOCK(x1,x2) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_RLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_RLock> guard2(lock2);

#define RLOCK(x1,x2,x3) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_RLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_RLock> guard2(lock2); \
    ACE_Local_RLock lock3(x1 + "::" + x2 + "::" + x3); \
    ACE_Guard<ACE_Local_RLock> guard3(lock3);

#define RLOCK(x1,x2,x3,x4) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_RLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_RLock> guard2(lock2); \
    ACE_Local_RLock lock3(x1 + "::" + x2 + "::" + x3); \
    ACE_Guard<ACE_Local_RLock> guard3(lock3); \
    ACE_Local_RLock lock4(x1 + "::" + x2 + "::" + x3 + "::" + x4); \
    ACE_Guard<ACE_Local_RLock> guard4(lock4);

#define RLOCK(x1,x2,x3,x4) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_RLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_RLock> guard2(lock2); \
    ACE_Local_RLock lock3(x1 + "::" + x2 + "::" + x3); \
    ACE_Guard<ACE_Local_RLock> guard3(lock3); \
    ACE_Local_RLock lock4(x1 + "::" + x2 + "::" + x3 + "::" + x4); \
    ACE_Guard<ACE_Local_RLock> guard4(lock4); \
    ACE_Local_RLock lock5(x1 + "::" + x2 + "::" + x3 + "::" + x4 + "::" + x5); \
    ACE_Guard<ACE_Local_RLock> guard5(lock5);

#define WLOCK(x1) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_WLock> guard1(lock1);

#define WLOCK(x1,x2) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_WLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_WLock> guard2(lock2);

#define WLOCK(x1,x2,x3) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_RLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_RLock> guard2(lock2); \
    ACE_Local_WLock lock3(x1 + "::" + x2 + "::" + x3); \
    ACE_Guard<ACE_Local_WLock> guard3(lock3);

#define WLOCK(x1,x2,x3,x4) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_RLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_RLock> guard2(lock2); \
    ACE_Local_RLock lock3(x1 + "::" + x2 + "::" + x3); \
    ACE_Guard<ACE_Local_RLock> guard3(lock3); \
    ACE_Local_WLock lock4(x1 + "::" + x2 + "::" + x3 + "::" + x4); \
    ACE_Guard<ACE_Local_WLock> guard4(lock4);

#define WLOCK(x1,x2,x3,x4,x5) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_RLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_RLock> guard2(lock2); \
    ACE_Local_RLock lock3(x1 + "::" + x2 + "::" + x3); \
    ACE_Guard<ACE_Local_RLock> guard3(lock3); \
    ACE_Local_RLock lock4(x1 + "::" + x2 + "::" + x3 + "::" + x4); \
    ACE_Guard<ACE_Local_RLock> guard4(lock4); \
    ACE_Local_WLock lock5(x1 + "::" + x2 + "::" + x3 + "::" + x4 + "::" + x5); \
    ACE_Guard<ACE_Local_WLock> guard5(lock5);

#define MUTEX(x1) \
    ACE_Local_Mutex lock1(x1); \
    ACE_Guard<ACE_Local_Mutex> guard1(lock1);

#define MUTEX(x1,x2) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_Mutex lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_Mutex> guard2(lock2);

#define MUTEX(x1,x2,x3) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_RLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_RLock> guard2(lock2); \
    ACE_Local_Mutex lock3(x1 + "::" + x2 + "::" + x3); \
    ACE_Guard<ACE_Local_Mutex> guard3(lock3);

#define MUTEX(x1,x2,x3,x4) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_RLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_RLock> guard2(lock2); \
    ACE_Local_RLock lock3(x1 + "::" + x2 + "::" + x3); \
    ACE_Guard<ACE_Local_RLock> guard3(lock3); \
    ACE_Local_Mutex lock4(x1 + "::" + x2 + "::" + x3 + "::" + x4); \
    ACE_Guard<ACE_Local_Mutex> guard4(lock4);

#define MUTEX(x1,x2,x3,x4,x5) \
    ACE_Local_RLock lock1(x1); \
    ACE_Guard<ACE_Local_RLock> guard1(lock1); \
    ACE_Local_RLock lock2(x1 + "::" + x2); \
    ACE_Guard<ACE_Local_RLock> guard2(lock2); \
    ACE_Local_RLock lock3(x1 + "::" + x2 + "::" + x3); \
    ACE_Guard<ACE_Local_RLock> guard3(lock3); \
    ACE_Local_RLock lock4(x1 + "::" + x2 + "::" + x3 + "::" + x4); \
    ACE_Guard<ACE_Local_RLock> guard4(lock4); \
    ACE_Local_Mutex lock5(x1 + "::" + x2 + "::" + x3 + "::" + x4 + "::" + x5); \
    ACE_Guard<ACE_Local_Mutex> guard5(lock5);
#endif

class RLOCK
{
    ACE_Local_RLock lock1,lock2,lock3,lock4,lock5;
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
};

#endif