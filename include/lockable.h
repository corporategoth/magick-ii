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
#include <ace/OS.h>
#include <ace/Task.h>
#include <ace/Synch.h>

class Lockable
{
private:
	ACE_RW_Thread_Mutex m_mutex;
public:
	ACE_RW_Thread_Mutex& Mutex();
	int RLOCK();
	int RUNLOCK();
	int WLOCK();
	int WUNLOCK();
};

#endif