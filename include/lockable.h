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
#include "trace.h"

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

#endif