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
    lock1.open(x1.c_str());
    lock1.acquire();
    count=1;
}

RLOCK::RLOCK(mstring x1, mstring x2)
{
    lock1.open(x1.c_str());
    lock1.acquire();
    lock2.open((x1+"::"+x2).c_str());
    lock2.acquire();
    count=2;
}

RLOCK::RLOCK(mstring x1, mstring x2, mstring x3)
{
    lock1.open(x1.c_str());
    lock1.acquire();
    lock2.open((x1+"::"+x2).c_str());
    lock2.acquire();
    lock3.open((x1+"::"+x2+"::"+x3).c_str());
    lock3.acquire();
    count=3;
}

RLOCK::RLOCK(mstring x1, mstring x2, mstring x3, mstring x4)
{
    lock1.open(x1.c_str());
    lock1.acquire();
    lock2.open((x1+"::"+x2).c_str());
    lock2.acquire();
    lock3.open((x1+"::"+x2+"::"+x3).c_str());
    lock3.acquire();
    lock4.open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    lock4.acquire();
    count=4;
}

RLOCK::RLOCK(mstring x1, mstring x2, mstring x3, mstring x4, mstring x5)
{
    lock1.open(x1.c_str());
    lock1.acquire();
    lock2.open((x1+"::"+x2).c_str());
    lock2.acquire();
    lock3.open((x1+"::"+x2+"::"+x3).c_str());
    lock3.acquire();
    lock4.open((x1+"::"+x2+"::"+x3+"::"+x4).c_str());
    lock4.acquire();
    lock5.open((x1+"::"+x2+"::"+x3+"::"+x4+"::"+x5).c_str());
    lock5.acquire();
    count=5;
}

RLOCK::~RLOCK()
{
    switch(count)
    {
    case 5:
	lock5.release();
    case 4:
	lock4.release();
    case 3:
	lock3.release();
    case 2:
	lock2.release();
    case 1:
	lock1.release();
    }
}
