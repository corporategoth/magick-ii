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
