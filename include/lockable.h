// RCS-ID:      $Id$
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