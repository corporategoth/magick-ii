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

int Lockable::RLOCK()
{
	return m_mutex.acquire_read();
}
int Lockable::RUNLOCK()
{
	return m_mutex.release();
}
int Lockable::WLOCK()
{
	return m_mutex.acquire_write();
}
int Lockable::WUNLOCK()
{
	return m_mutex.release();
}
ACE_RW_Thread_Mutex& Lockable::Mutex()
{
	return m_mutex;
}
