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
    count=1;
}

RLOCK::RLOCK(mstring x1, mstring x2)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    count=2;
}

RLOCK::RLOCK(mstring x1, mstring x2, mstring x3)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    count=3;
}

RLOCK::RLOCK(mstring x1, mstring x2, mstring x3, mstring x4)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    lock[3].open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    lock[3].acquire();
    count=4;
}

RLOCK::RLOCK(mstring x1, mstring x2, mstring x3, mstring x4, mstring x5)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    lock[3].open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    lock[3].acquire();
    lock[4].open((x1+"::"+x2+"::"+x3+"::"+x4+"::"+x5).c_str());
    lock[4].acquire();
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
    count=0;
}

WLOCK::WLOCK(mstring x1, mstring x2)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    wlock.open((x1+"::"+x2).c_str());
    wlock.acquire();
    count=1;
}

WLOCK::WLOCK(mstring x1, mstring x2, mstring x3)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    wlock.open((x1+"::"+x2+"::"+x3).c_str());
    wlock.acquire();
    count=2;
}

WLOCK::WLOCK(mstring x1, mstring x2, mstring x3, mstring x4)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    wlock.open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    wlock.acquire();
    count=3;
}

WLOCK::WLOCK(mstring x1, mstring x2, mstring x3, mstring x4, mstring x5)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    lock[3].open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    lock[3].acquire();
    wlock.open((x1+"::"+x2+"::"+x3+"::"+x4+"::"+x5).c_str());
    wlock.acquire();
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
    count=0;
}

MLOCK::MLOCK(mstring x1, mstring x2)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    mlock.open((x1+"::"+x2).c_str());
    mlock.acquire();
    count=1;
}

MLOCK::MLOCK(mstring x1, mstring x2, mstring x3)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    mlock.open((x1+"::"+x2+"::"+x3).c_str());
    mlock.acquire();
    count=2;
}

MLOCK::MLOCK(mstring x1, mstring x2, mstring x3, mstring x4)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    mlock.open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    mlock.acquire();
    count=3;
}

MLOCK::MLOCK(mstring x1, mstring x2, mstring x3, mstring x4, mstring x5)
{
    lock[0].open(x1.c_str());
    lock[0].acquire();
    lock[1].open((x1+"::"+x2).c_str());
    lock[1].acquire();
    lock[2].open((x1+"::"+x2+"::"+x3).c_str());
    lock[2].acquire();
    lock[3].open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    lock[3].acquire();
    mlock.open((x1+"::"+x2+"::"+x3+"::"+x4+"::"+x5).c_str());
    mlock.acquire();
    count=4;
}

MLOCK::~MLOCK()
{
    mlock.release();
    for(;count;count--)
	lock[count-1].release();
}
