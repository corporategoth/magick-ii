#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#define RCSID(x,y) const char *rcsid_lockable_cpp_ ## x () { return y; }
RCSID(lockable_cpp, "@(#)$Id$");

/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.79  2002/01/12 14:42:09  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.78  2002/01/10 19:30:38  prez
** FINALLY finished a MAJOR overhaul ... now have a 'safe pointer', that
** ensures that data being used cannot be deleted while still being used.
**
** Revision 1.77  2001/12/20 08:02:32  prez
** Massive change -- 'Parent' has been changed to Magick::instance(), will
** soon also move the ACE_Reactor over, and will be able to have multipal
** instances of Magick in the same process if necessary.
**
** Revision 1.76  2001/12/16 01:30:45  prez
** More changes to fix up warnings ... added some new warning flags too!
**
** Revision 1.75  2001/11/28 13:40:47  prez
** Added UMASK option to config.  Also made the 'dead thread' protection
** send a SIGIOT signal to try and get the thread to die gracefully, else
** it will do the cancel it used to do.
**
** Revision 1.74  2001/11/12 01:05:02  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.73  2001/11/03 21:02:53  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.72  2001/05/17 19:18:54  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.71  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.70  2001/05/03 04:40:17  prez
** Fixed locking mechanism (now use recursive mutexes) ...
** Also now have a deadlock/nonprocessing detection mechanism.
**
** Revision 1.69  2001/05/02 04:57:07  prez
** Fixed up problems in failing to release locks
**
** Revision 1.68  2001/05/02 02:35:27  prez
** Fixed dependancy system, and removed printf's - we no longer coredump on
** a 1000 user network.  As a bonus, we actually synd perfectly ;P
**
** Revision 1.67  2001/05/01 14:00:23  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.66  2001/04/08 18:53:09  prez
** It now all compiles and RUNS with -fno-default-inline OFF.
**
** Revision 1.65  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.64  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.63  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.62  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.61  2001/02/03 02:21:33  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.60  2001/01/15 23:31:39  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.59  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.58  2000/12/25 06:36:14  prez
** Added locking around the threadtoself map, and removed a bunch of
** defines from mstring (while keeping it the same!)
**
** Revision 1.57  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.56  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.55  2000/10/18 18:46:33  prez
** Well, mstring still coredumps, but it gets past the initial loading of
** all the STATIC (or const) strings, etc -- now its coring on loading a
** file (or possibly language.h or something).  Still investigating.
**
** Revision 1.54  2000/10/10 11:47:51  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.53  2000/10/07 11:01:13  prez
** Took out placement new's from lockable.cpp, now using derived classes.
**
** Revision 1.52  2000/10/04 10:52:08  prez
** Fixed the memory pool and removed printf's.
**
** Revision 1.51  2000/10/04 07:39:46  prez
** Added MemCluster to speed up lockable, but it cores when we start
** getting real messages -- seemingly in an alloc in the events.
** Lots of printf's left in for debugging (so run as ./magick >output)
**
** Revision 1.50  2000/09/13 12:45:34  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.49  2000/09/07 08:13:17  prez
** Fixed some of the erronous messages (SVSHOST, SQLINE, etc).
** Also added CPU statistics and fixed problem with socket deletions.
**
** Revision 1.48  2000/09/06 11:27:33  prez
** Finished the T_Modify / T_Changing traces, fixed a bug in clone
** adding (was adding clone liimt as the mask length), updated docos
** a little more, and added a response to SIGINT to signon clients.
**
** Revision 1.47  2000/09/05 10:53:07  prez
** Only have operserv.cpp and server.cpp to go with T_Changing / T_Modify
** tracing -- also modified keygen to allow for cmdline generation (ie.
** specify 1 option and enter keys, or 2 options and the key is read from
** a file).  This allows for paragraphs with \n's in them, and helps so you
** do not have to type out 1024 bytes :)
**
** Revision 1.46  2000/09/01 10:54:38  prez
** Added Changing and implemented Modify tracing, now just need to create
** DumpB() and DumpE() functions in all classes, and put MCB() / MCE() calls
** (or MB() / ME() or CB() / CE() where MCB() / MCE() not appropriate) in.
**
** Revision 1.45  2000/08/31 06:25:09  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.44  2000/08/28 10:51:37  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.43  2000/08/22 08:43:41  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.42  2000/08/19 10:59:47  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.41  2000/08/06 05:27:47  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.40  2000/08/03 13:06:31  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.39  2000/07/21 00:18:49  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.38  2000/06/13 14:11:54  prez
** Locking system has been re-written, it doent core anymore.
** So I have set 'MAGICK_LOCKS_WORK' define active :)
**
** Revision 1.37  2000/06/06 08:57:56  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.36  2000/05/20 03:28:11  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.35  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.34  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.33  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.32  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.31  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"

#ifdef MAGICK_LOCKS_WORK

mLock_Mutex *mLOCK::maplock = NULL;
mLock_Mutex *mThread::maplock = NULL;
map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > >mLOCK::LockMap;

ACE_Expandable_Cached_Fixed_Allocator < ACE_Thread_Mutex >
    mLOCK::
memory_area((sizeof(mLock_Read) >
	     sizeof(mLock_Write) ? (sizeof(mLock_Read) >
				    sizeof(mLock_Mutex) ? sizeof(mLock_Read) : sizeof(mLock_Mutex)) : (sizeof(mLock_Write) >
												       sizeof(mLock_Mutex) ?
												       sizeof(mLock_Write) :
												       sizeof(mLock_Mutex))),
	    LOCK_SEGMENT);

map < unsigned long, mSocket * >mSocket::SockMap;

bool mLOCK::AcquireMapLock()
{
    if (maplock == NULL)
	maplock = new mLock_Mutex("LockMap");
    if (maplock == NULL)
    {

/*	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", (
		"MUTEX", "LockMap")); */
	return false;
    }
    if (maplock->acquire() < 0)
    {

/*	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", (
		"MUTEX", "LockMap")); */
	return false;
    }
    return true;
}

bool mLOCK::ReleaseMapLock()
{
    if (maplock == NULL)
	return true;
    if (maplock->release() < 0)
    {

/*	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", (
		"MUTEX", "LockMap")); */
	return false;
    }
    return true;
}

mLOCK::mLOCK(const locktype_enum type, const mVarArray & args)
{
    islocked = false;
    if (Magick::StartTime() == mDateTime(0.0) || !Magick::instance_exists() || Magick::instance().ResetTime() == mDateTime(0.0))
	return;

    int i;

#ifdef MAGICK_TRACE_WORKS
    int count = 0;
#endif
    map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > >::iterator lockiter;
    mstring lockname;
    char hash[33];
    mLock_Mutex *mutex_lock;
    mLock_Write *write_lock;
    mLock_Read *read_lock;

    for (i = 0; i < args.count() - 1; i++)
    {
	if (!lockname.empty())
	    lockname += "::";
	lockname += args[i].AsString();
	read_lock = NULL;

	if (!AcquireMapLock())
	    return;
	if ((lockiter = LockMap.find(lockname)) != LockMap.end())
	{
	    if (lockiter->second.second.find(ACE_Thread::self()) == lockiter->second.second.end())
	    {
		lockiter->second.second[ACE_Thread::self()] = L_Read;
		read_lock = reinterpret_cast < mLock_Read * >(lockiter->second.first);
	    }
	    else if (lockiter->second.second[ACE_Thread::self()] == L_Mutex)
	    {
		ReleaseMapLock();
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_DUP", ("READ", lockname));
		return;
	    }
	}
	else
	{
	    mHASH(lockname.c_str(), lockname.length(), hash);
	    read_lock = new mLock_Read(reinterpret_cast < const char *>(hash));

	    if (read_lock == NULL)
	    {
		ReleaseMapLock();
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", ("READ", lockname));
		return;
	    }
	    map < ACE_thread_t, locktype_enum > tmap;
	    LockMap[lockname] = pair < void *, map < ACE_thread_t, locktype_enum > >(read_lock, tmap);

	    LockMap[lockname].second[ACE_Thread::self()] = L_Read;
	}
	ReleaseMapLock();
	if (read_lock != NULL)
	{
	    if (read_lock->acquire() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("READ", lockname));
		read_lock = NULL;
		if (!AcquireMapLock())
		    return;
		if ((lockiter = LockMap.find(lockname)) != LockMap.end())
		{
		    lockiter->second.second.erase(ACE_Thread::self());
		    if (!lockiter->second.second.size())
		    {
			read_lock = reinterpret_cast < mLock_Read * >(lockiter->second.first);
			if (read_lock != NULL)
			    delete read_lock;

			read_lock = NULL;
			LockMap.erase(lockiter);
		    }
		}
		ReleaseMapLock();
	    }
	    else
	    {
		locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		tlock[count++].open(L_Read, lockname);
#endif
	    }
	}
    }

    if (!lockname.empty())
	lockname += "::";
    lockname += args[i].AsString();
    read_lock = NULL;
    write_lock = NULL;
    mutex_lock = NULL;


    if (type == L_Read)
    {
	if (!AcquireMapLock())
	    return;
	if ((lockiter = LockMap.find(lockname)) != LockMap.end())
	{
	    if (lockiter->second.second.find(ACE_Thread::self()) == lockiter->second.second.end())
	    {
		lockiter->second.second[ACE_Thread::self()] = L_Read;
		read_lock = reinterpret_cast < mLock_Read * >(lockiter->second.first);
	    }
	    else if (lockiter->second.second[ACE_Thread::self()] == L_Mutex)
	    {
		ReleaseMapLock();
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_DUP", ("READ", lockname));
		return;
	    }
	}
	else
	{
	    mHASH(lockname.c_str(), lockname.length(), hash);
	    read_lock = new mLock_Read(reinterpret_cast < const char *>(hash));

	    if (read_lock == NULL)
	    {
		ReleaseMapLock();
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", ("READ", lockname));
		return;
	    }
	    map < ACE_thread_t, locktype_enum > tmap;
	    LockMap[lockname] = pair < void *, map < ACE_thread_t, locktype_enum > >(read_lock, tmap);

	    LockMap[lockname].second[ACE_Thread::self()] = L_Read;
	}
	ReleaseMapLock();
	if (read_lock != NULL)
	{
	    if (read_lock->acquire() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("READ", lockname));
		read_lock = NULL;
		if (!AcquireMapLock())
		    return;
		if ((lockiter = LockMap.find(lockname)) != LockMap.end())
		{
		    lockiter->second.second.erase(ACE_Thread::self());
		    if (!lockiter->second.second.size())
		    {
			read_lock = reinterpret_cast < mLock_Read * >(lockiter->second.first);
			if (read_lock != NULL)
			    delete read_lock;

			read_lock = NULL;
			LockMap.erase(lockiter);
		    }
		}
		ReleaseMapLock();
	    }
	    else
	    {
		islocked = true;
		locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		tlock[count++].open(L_Read, lockname);
#endif
	    }
	}
    }
    else if (type == L_Write)
    {
	if (!AcquireMapLock())
	    return;
	if ((lockiter = LockMap.find(lockname)) != LockMap.end())
	{
	    if (lockiter->second.second.find(ACE_Thread::self()) != lockiter->second.second.end())
	    {
		switch (lockiter->second.second[ACE_Thread::self()])
		{
		case L_Read:
		    lockiter->second.second[ACE_Thread::self()] = L_WriteUpgrade;
		    read_lock = reinterpret_cast < mLock_Read * >(lockiter->second.first);
		    write_lock = reinterpret_cast < mLock_Write * >(lockiter->second.first);
		    break;
		case L_Mutex:
		    ReleaseMapLock();
		    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_DUP", ("WRITE", lockname));
		    return;
		default:
		    break;
		}
	    }
	    else
	    {
		lockiter->second.second[ACE_Thread::self()] = L_Write;
		write_lock = reinterpret_cast < mLock_Write * >(lockiter->second.first);
	    }
	}
	else
	{
	    mHASH(lockname.c_str(), lockname.length(), hash);
	    write_lock = new mLock_Write(reinterpret_cast < const char *>(hash));

	    if (write_lock == NULL)
	    {
		ReleaseMapLock();
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", ("WRITE", lockname));
		return;
	    }
	    map < ACE_thread_t, locktype_enum > tmap;
	    LockMap[lockname] = pair < void *, map < ACE_thread_t, locktype_enum > >(write_lock, tmap);

	    LockMap[lockname].second[ACE_Thread::self()] = L_Write;
	}
	ReleaseMapLock();
	if (read_lock != NULL)
	{
	    if (read_lock->release() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", ("READ", lockname));
	    }
	    read_lock = NULL;
	}
	if (write_lock != NULL)
	{
	    if (write_lock->acquire() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("WRITE", lockname));
		write_lock = NULL;
		if (!AcquireMapLock())
		    return;
		if ((lockiter = LockMap.find(lockname)) != LockMap.end())
		{
		    lockiter->second.second.erase(ACE_Thread::self());
		    if (!lockiter->second.second.size())
		    {
			write_lock = reinterpret_cast < mLock_Write * >(lockiter->second.first);
			if (write_lock != NULL)
			    delete read_lock;

			write_lock = NULL;
			LockMap.erase(lockiter);
		    }
		}
		ReleaseMapLock();
	    }
	    else
	    {
		islocked = true;
		locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		tlock[count++].open(L_Write, lockname);
#endif
	    }
	}
    }
    else if (type == L_Mutex)
    {
	if (!AcquireMapLock())
	    return;
	if ((lockiter = LockMap.find(lockname)) != LockMap.end())
	{
	    map < ACE_thread_t, locktype_enum >::iterator iter;
	    for (iter = lockiter->second.second.begin(); iter != lockiter->second.second.end(); iter++)
	    {
		if (iter->second != L_Mutex)
		{
		    ReleaseMapLock();
		    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_DUP", ("MUTEX", lockname));
		    return;
		}
	    }

	    if (lockiter->second.second.find(ACE_Thread::self()) == lockiter->second.second.end())
	    {
		lockiter->second.second[ACE_Thread::self()] = L_Mutex;
		mutex_lock = reinterpret_cast < mLock_Mutex * >(lockiter->second.first);
	    }
	}
	else
	{
	    mHASH(lockname.c_str(), lockname.length(), hash);
	    mutex_lock = new mLock_Mutex(reinterpret_cast < const char *>(hash));

	    if (mutex_lock == NULL)
	    {
		ReleaseMapLock();
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", ("MUTEX", lockname));
		return;
	    }
	    map < ACE_thread_t, locktype_enum > tmap;
	    LockMap[lockname] = pair < void *, map < ACE_thread_t, locktype_enum > >(mutex_lock, tmap);

	    LockMap[lockname].second[ACE_Thread::self()] = L_Mutex;
	}
	ReleaseMapLock();
	if (mutex_lock != NULL)
	{
	    if (mutex_lock->acquire() < 0)
	    {
		LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("MUTEX", lockname));
		mutex_lock = NULL;
		if (!AcquireMapLock())
		    return;
		if ((lockiter = LockMap.find(lockname)) != LockMap.end())
		{
		    lockiter->second.second.erase(ACE_Thread::self());
		    if (!lockiter->second.second.size())
		    {
			mutex_lock = reinterpret_cast < mLock_Mutex * >(lockiter->second.first);
			if (mutex_lock != NULL)
			    delete mutex_lock;

			mutex_lock = NULL;
			LockMap.erase(lockiter);
		    }
		}
		ReleaseMapLock();
	    }
	    else
	    {
		islocked = true;
		locks.push_back(lockname);
#ifdef MAGICK_TRACE_WORKS
		tlock[count++].open(L_Mutex, lockname);
#endif
	    }
	}
    }

    memset(hash, 0, sizeof(hash));
}

mLOCK::~mLOCK()
{
    if (Magick::StartTime() == mDateTime(0.0) || !Magick::instance_exists() || Magick::instance().ResetTime() == mDateTime(0.0))
	return;

    int i;
    map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > >::iterator lockiter;
    mLock_Mutex *mutex_lock;
    mLock_Write *write_lock;
    mLock_Read *read_lock;

    for (i = locks.size() - 1; i >= 0; i--)
    {
	if (!AcquireMapLock())
	    return;
	if ((lockiter = LockMap.find(locks[i])) != LockMap.end())
	{
	    read_lock = NULL;
	    write_lock = NULL;
	    mutex_lock = NULL;
	    bool killit = false;

	    map < ACE_thread_t, locktype_enum >::iterator iter = lockiter->second.second.find(ACE_Thread::self());
	    if (iter != lockiter->second.second.end())
	    {
		switch (iter->second)
		{
		case L_Read:
		    read_lock = reinterpret_cast < mLock_Read * >(lockiter->second.first);
		    break;
		case L_Write:
		case L_WriteUpgrade:
		    write_lock = reinterpret_cast < mLock_Write * >(lockiter->second.first);
		    break;
		case L_Mutex:
		    mutex_lock = reinterpret_cast < mLock_Mutex * >(lockiter->second.first);
		    break;
		default:
		    break;
		}

		if (iter->second == L_WriteUpgrade)
		    lockiter->second.second[ACE_Thread::self()] = L_Read;
		else
		    lockiter->second.second.erase(ACE_Thread::self());

		if (!lockiter->second.second.size())
		{
		    killit = true;
		    LockMap.erase(lockiter);
		}
	    }
	    ReleaseMapLock();

	    if (read_lock != NULL)
	    {
		if (read_lock->release() < 0)
		{
		    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", ("READ", locks[i]));
		}
		if (killit)
		    delete read_lock;

		read_lock = NULL;
	    }
	    if (write_lock != NULL)
	    {
		if (write_lock->release() < 0)
		{
		    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", ("WRITE", locks[i]));
		}
		if (killit)
		    delete write_lock;

		write_lock = NULL;
	    }
	    if (mutex_lock != NULL)
	    {
		if (mutex_lock->release() < 0)
		{
		    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", ("MUTEX", locks[i]));
		}
		if (killit)
		    delete mutex_lock;

		mutex_lock = NULL;
	    }

	    if (!AcquireMapLock())
		return;
	    if ((lockiter = LockMap.find(locks[i])) != LockMap.end())
	    {
		if ((iter = lockiter->second.second.find(ACE_Thread::self())) != lockiter->second.second.end())
		{
		    // We're still here, must have been an upgraded lock,
		    // time to downgrade it ...
		    if (iter->second == L_Read)
		    {
			read_lock = reinterpret_cast < mLock_Read * >(lockiter->second.first);
			ReleaseMapLock();
			if (read_lock->acquire() < 0)
			{
			    LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", ("READ", locks[i]));
			    read_lock = NULL;
			    if (!AcquireMapLock())
				return;
			    if ((lockiter = LockMap.find(locks[i])) != LockMap.end())
			    {
				lockiter->second.second.erase(ACE_Thread::self());
				if (!lockiter->second.second.size())
				{
				    read_lock = reinterpret_cast < mLock_Read * >(lockiter->second.first);
				    if (read_lock != NULL)
					delete read_lock;

				    read_lock = NULL;
				    LockMap.erase(lockiter);
				}
			    }
			    ReleaseMapLock();
			}
			read_lock = NULL;
		    }
		    else	/* Its not a read lock */
			ReleaseMapLock();
		}
		else		/* Our thread isnt listed in the threads with this lock */
		    ReleaseMapLock();
	    }
	    else		/* This lock doesnt exist */
		ReleaseMapLock();
	}
	else			/* This lock NEVER existed */
	    ReleaseMapLock();
    }
}

list < pair < void *, locktype_enum > >mLOCK::GetLocks(ACE_thread_t thr)
{
    list < pair < void *, locktype_enum > >retval;
    map < mstring, pair < void *, map < ACE_thread_t, locktype_enum > > >::iterator i;

    map < ACE_thread_t, locktype_enum >::iterator j;

    if (!AcquireMapLock())
	return retval;
    for (i = LockMap.begin(); i != LockMap.end(); i++)
    {
	for (j = i->second.second.begin(); j != i->second.second.end(); j++)
	{
	    if (j->first == thr)
		retval.push_back(pair < void *, locktype_enum > (i->second.first, j->second));
	}
    }
    ReleaseMapLock();
    return retval;
}


#endif /* MAGICK_LOCKS_WORK */

unsigned short mSocket::FindAvailPort()
{
    NFT("mSocket::FindAvailPort");

    ACE_INET_Addr la;
    ACE_SOCK_Acceptor acc(ACE_Addr::sap_any);

    acc.get_local_addr(la);
    unsigned short retval = la.get_port_number();

    acc.close();
    RET(retval);
}

void mSocket::init()
{
    NFT("mSocket::init");

    sock = NULL;
    last_error = 0;
    sockid = 0;
    DestroyMe = false;

    if (Magick::StartTime() == mDateTime(0.0) || !Magick::instance_exists() || Magick::instance().ResetTime() == mDateTime(0.0))
	return;

    unsigned long i;

    MLOCK(("SockMap"));
    for (i = 1; i > 0; i++)
	if (SockMap.find(i) == SockMap.end())
	{
	    SockMap[i] = this;
	    sockid = i;
	    break;
	}
}

mSocket::~mSocket()
{
    NFT("mSocket::mSocket");
    if (sock != NULL)
	close();
    MLOCK(("SockMap"));
    SockMap.erase(sockid);
}

mSocket & mSocket::operator=(const mSocket & in)
{
    FT("mSocket::operator=", ("(const mSocket &) in"));

    sockid = in.sockid;
    local = in.local;
    remote = in.remote;
    last_error = in.last_error;
    DestroyMe = in.DestroyMe;
#ifdef MAGICK_TRACE_WORKS
    trace = in.trace;
#endif
    sock = in.sock;
    in.sock = NULL;

    {
	MLOCK2(("SockMap"));
	SockMap[sockid] = this;
    }
    NRET(mSocket &, *this);
}


bool mSocket::Connect(const ACE_INET_Addr & addr, const unsigned long timeout)
{
    FT("mSocket::Connect", ("(ACE_INET_Addr) addr", timeout));

    WLOCK(("mSocket", sockid));
    if (sock != NULL)
	close();

    sock = new ACE_SOCK_Stream;
    if (sock == NULL)
	RET(false);

    DestroyMe = true;
    ACE_Time_Value tv(timeout);
    ACE_SOCK_Connector tmp;
    int result = tmp.connect(*sock, addr, timeout ? &tv : 0);

    if (result < 0)
    {
	last_error = errno;
#ifdef MAGICK_TRACE_WORKS
	trace.Failed(sockid, local.get_port_number(), remote.get_port_number(), mstring(remote.get_host_addr()),
		     Last_Error_String(), D_From);

#endif
	delete sock;

	sock = NULL;
	RET(false);
    }
    last_error = 0;
    sock->get_local_addr(local);
    sock->get_remote_addr(remote);
#ifdef MAGICK_TRACE_WORKS
    trace.Begin(sockid, local.get_port_number(), remote.get_port_number(), mstring(remote.get_host_addr()), D_From);

#endif
    RET(true);
}

bool mSocket::Connect(const unsigned long host, const unsigned short port, const unsigned long timeout)
{
    FT("mSocket::Connect", (host, port, timeout));
    ACE_INET_Addr tmp(port, host);
    bool retval = Connect(tmp, timeout);

    RET(retval);
}

bool mSocket::Connect(const mstring & host, const unsigned short port, const unsigned long timeout)
{
    FT("mSocket::Connect", (host, port, timeout));
    ACE_INET_Addr tmp(port, host);
    bool retval = Connect(tmp, timeout);

    RET(retval);
}

bool mSocket::Accept(const unsigned short port, const unsigned long timeout)
{
    FT("mSocket::Accept", (port, timeout));
    ACE_INET_Addr addr(port, Magick::instance().LocalHost());

    WLOCK(("mSocket", sockid));
    if (sock != NULL)
	close();

    sock = new ACE_SOCK_Stream;
    if (sock == NULL)
	RET(false);

    DestroyMe = true;
    ACE_Time_Value tv(timeout);
    ACE_SOCK_Acceptor tmp(addr);
    int result = tmp.accept(*sock, NULL, timeout ? &tv : 0);

    if (result < 0)
    {
	last_error = errno;
#ifdef MAGICK_TRACE_WORKS
	trace.Failed(sockid, local.get_port_number(), remote.get_port_number(), mstring(remote.get_host_addr()),
		     Last_Error_String(), D_To);
#endif
	delete sock;

	sock = NULL;
	RET(false);
    }

    last_error = 0;
    sock->get_local_addr(local);
    sock->get_remote_addr(remote);
#ifdef MAGICK_TRACE_WORKS
    trace.Begin(sockid, local.get_port_number(), remote.get_port_number(), mstring(remote.get_host_addr()), D_To);
#endif
    RET(true);
}

bool mSocket::Bind(ACE_SOCK_Stream * in, const dir_enum direction, const bool alloc)
{
    FT("mSocket::Bind", ("(ACE_SOCK_Stream *) in"));
    if (in == NULL)
	RET(false);

    WLOCK(("mSocket", sockid));
    if (sock != NULL)
	close();

    sock = in;
    in = NULL;
    DestroyMe = alloc;
    last_error = 0;
    sock->get_local_addr(local);
    sock->get_remote_addr(remote);

#ifdef MAGICK_TRACE_WORKS
    trace.Begin(sockid, local.get_port_number(), remote.get_port_number(), mstring(remote.get_host_addr()), direction);
#else
    static_cast < void >(direction);
#endif

    RET(true);
}

ACE_SOCK_Stream *mSocket::Unbind()
{
    NFT("mSocket::Unbind");
    WLOCK(("mSocket", sockid));
#ifdef MAGICK_TRACE_WORKS
    trace.End(Last_Error_String());
#endif
    ACE_SOCK_Stream *retval = sock;

    sock = NULL;
    last_error = 0;
    NRET(ACE_SOCK_Stream *, retval);
}

mstring mSocket::Local_Host() const
{
    NFT("mSocket::Local_Host");
    RLOCK(("mSocket", sockid));
    mstring retval(local.get_host_addr());

    RET(retval);
}

unsigned long mSocket::Local_IP() const
{
    NFT("mSocket::Local_IP");
    RLOCK(("mSocket", sockid));
    unsigned long retval = local.get_ip_address();

    RET(retval);
}

unsigned short mSocket::Local_Port() const
{
    NFT("mSocket::Local_Port");
    RLOCK(("mSocket", sockid));
    unsigned short retval = local.get_port_number();

    RET(retval);
}

mstring mSocket::Remote_Host() const
{
    NFT("mSocket::Remote_Host");
    RLOCK(("mSocket", sockid));
    mstring retval = remote.get_host_addr();

    RET(retval);
}

unsigned long mSocket::Remote_IP() const
{
    NFT("mSocket::Remote_IP");
    RLOCK(("mSocket", sockid));
    unsigned long retval = remote.get_ip_address();

    RET(retval);
}

unsigned short mSocket::Remote_Port() const
{
    NFT("mSocket::Remote_Port");
    RLOCK(("mSocket", sockid));
    unsigned short retval = remote.get_port_number();

    RET(retval);
}

bool mSocket::IsConnected() const
{
    NFT("mSocket::IsConnected");
    RLOCK(("mSocket", sockid));
    RET(sock != NULL);
}

void mSocket::Resolve(const socktype_enum type, const mstring & info)
{
    FT("mSocket::Resolve", (static_cast < int >(type), info));
#ifdef MAGICK_TRACE_WORKS
    WLOCK(("mSocket", sockid));
    trace.Resolve(type, info);
#else
    static_cast < void >(type);
    static_cast < void >(info);
#endif
}

int mSocket::Last_Error() const
{
    NFT("mSocket::Last_Error");
    RLOCK(("mSocket", sockid));
    RET(last_error);
}

mstring mSocket::Last_Error_String() const
{
    NFT("mSocket::Last_Error_String");
    RLOCK(("mSocket", sockid));
    mstring retval = strerror(last_error);

    RET(retval);
}

ssize_t mSocket::send(void *buf, const size_t len, const unsigned long timeout)
{
    FT("mSocket::send", ("(void *) buf", len, timeout));
    ACE_Time_Value tv(timeout);

    WLOCK(("mSocket", sockid));
    ssize_t retval = sock->send(buf, len, timeout ? &tv : 0);

    if (retval < 0)
    {
	last_error = errno;
	// close();
    }
    else
	last_error = 0;

    RET(retval);
}

ssize_t mSocket::recv(void *buf, const size_t len, const unsigned long timeout)
{
    FT("mSocket::recv", ("(void *) buf", len, timeout));
    ACE_Time_Value tv(timeout);

    WLOCK(("mSocket", sockid));
    ssize_t retval = sock->recv(buf, len, timeout ? &tv : 0);

    if (retval < 0)
    {
	last_error = errno;
	// close();
    }
    else
	last_error = 0;

    RET(retval);
}

int mSocket::close()
{
    NFT("mSocket::close");
    int retval = 0;

    WLOCK(("mSocket", sockid));
    if (sock != NULL)
    {
#ifdef MAGICK_TRACE_WORKS
	trace.End(Last_Error_String());
#endif
	retval = sock->close();
	if (retval < 0)
	    last_error = errno;
	else
	    last_error = 0;
	if (DestroyMe)
	    delete sock;

	sock = NULL;
    }
    RET(retval);
}

mThread::selftothreadidmap_t mThread::selftothreadidmap;

bool mThread::AcquireMapLock()
{
    if (maplock == NULL)
	maplock = new mLock_Mutex("SelfToThreadIdMap");
    if (maplock == NULL)
    {

/*	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_OPEN", (
		"MUTEX", "SelfToThreadIdMap")); */
	return false;
    }
    if (maplock->acquire() < 0)
    {

/*	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_ACQUIRE", (
		"MUTEX", "SelfToThreadIdMap")); */
	return false;
    }
    return true;
}

bool mThread::ReleaseMapLock()
{
    if (maplock == NULL)
	return true;
    if (maplock->release() < 0)
    {

/*	LOG(LM_CRITICAL, "SYS_ERRORS/LOCK_RELEASE", (
		"MUTEX", "SelfToThreadIdMap")); */
	return false;
    }
    return true;
}



ThreadID *mThread::find(const ACE_thread_t thread)
{
    if (Magick::StartTime() == mDateTime(0.0))
	return NULL;

    if (!AcquireMapLock())
	return NULL;

    ThreadID *tid = NULL;

    if (selftothreadidmap.find(thread) != selftothreadidmap.end())
	tid = selftothreadidmap[thread];

    ReleaseMapLock();
    return tid;
}

vector < ThreadID * >mThread::findall()
{
    vector < ThreadID * >threadlist;
    selftothreadidmap_t::const_iterator iter;

    if (!AcquireMapLock())
	return threadlist;

    for (iter = selftothreadidmap.begin(); iter != selftothreadidmap.end(); iter++)
	threadlist.push_back(iter->second);

    ReleaseMapLock();
    return threadlist;
}

void mThread::Attach(const threadtype_enum ttype)
{
    FT("mThread::Attach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid = new ThreadID(ttype);

    if (!AcquireMapLock())
	return;
    selftothreadidmap[ACE_Thread::self()] = tmpid;
    ReleaseMapLock();
    COM(("Thread ID has been attached."));
}

void mThread::Detach()
{
    NFT("mThread::Detach");
    ThreadID *tmpid = find();

    if (tmpid == NULL)
    {
	// todo ttype to typename
	CP(("mThread::Detach without valid mThread::Attach..."));
	return;
    }
    if (!AcquireMapLock())
	return;
    selftothreadidmap.erase(ACE_Thread::self());
    ReleaseMapLock();
    delete tmpid;

    COM(("Thread ID has been detached."));
}

void mThread::ReAttach(const threadtype_enum ttype)
{
    FT("mThread::ReAttach", ("(threadtype_enum) ttype"));
    ThreadID *tmpid = find();

    if (tmpid == NULL)
    {
	// ReAttach does an attach if it wasnt there
	CP(("mThread::ReAttach without valid mThread::Attach... type: %s", threadname[ttype].c_str()));
	tmpid = new ThreadID();
	if (!AcquireMapLock())
	    return;
	selftothreadidmap[ACE_Thread::self()] = tmpid;
	ReleaseMapLock();
    }
#ifdef MAGICK_TRACE_WORKS	// Get rid of 'unused veriable' warning
    threadtype_enum oldtype = tmpid->type();
#endif
    COM(("Thread ID has been re-attached to %s.", threadname[ttype].c_str()));
    tmpid->assign(ttype);
    COM(("Thread ID has been re-attached from %s.", threadname[oldtype].c_str()));
}
