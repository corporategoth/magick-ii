#include "pch.h"
#ifdef WIN32
#pragma hdrstop
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
#define RCSID(x,y) const char *rcsid_main_cpp_ ## x () { return y; }
RCSID(main_cpp, "@(#)$Id$");

/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.56  2002/01/14 07:16:55  prez
** More pretty printing with a newer indent with C++ fixes (not totally done)
**
** Revision 1.55  2002/01/12 14:42:09  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.54  2001/12/21 05:02:29  prez
** Changed over from using a global ACE_Reactor to using an instance inside
** of the Magick instance.
**
** Revision 1.53  2001/12/20 08:02:32  prez
** Massive change -- 'Parent' has been changed to Magick::instance(), will
** soon also move the ACE_Reactor over, and will be able to have multipal
** instances of Magick in the same process if necessary.
**
** Revision 1.52  2001/11/30 09:01:56  prez
** Changed Magick to have Init(), Start(), Run(), Stop(), Finish() and
** Pause(bool) functions. This should help if/when we decide to implement
** Magick running as an NT service.
**
** Revision 1.51  2001/11/30 07:30:07  prez
** Added some windows stuff ...
**
** Revision 1.50  2001/11/12 01:05:03  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.49  2001/11/03 21:02:53  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.48  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.47  2001/04/08 18:53:09  prez
** It now all compiles and RUNS with -fno-default-inline OFF.
**
** Revision 1.46  2001/02/03 02:21:34  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.45  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.44  2000/12/05 14:49:53  prez
** Added fflush to exceptions.
**
** Revision 1.43  2000/10/14 04:25:31  prez
** Added mmemory.h -- MemCluster and the MemoryManager are now in it.
** TODO - make mstring use MemoryManager.
**
** Revision 1.42  2000/08/06 05:27:47  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.41  2000/05/20 03:28:12  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.40  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.39  2000/03/13 09:36:18  prez
** Completed help file, we now have full help text.
**
** Revision 1.38  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.37  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.36  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.35  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.34  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"

#ifdef WIN32
bool isWinNT()
{
#ifdef __BORLANDC__
    return (Win32Platform == VER_PLATFORM_WIN32_NT)
#elif _MSC_VER
    OSVERSIONINFO osvi;

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (GetVersionEx(&osvi))
	return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);
    else
	return false;
#else
    return false;
#endif
}
#endif

int main(int argc, char **argv)
{
#ifdef MAGICK_TRACE_WORKS
    Trace::levelname.push_back(Trace::levelname_struct("OFF", Trace::Off));
    Trace::levelname.push_back(Trace::levelname_struct("NONE", Trace::Off));
    Trace::levelname.push_back(Trace::levelname_struct("FULL", Trace::Full));
    Trace::levelname.push_back(Trace::levelname_struct("ALL", Trace::Full));
    Trace::levelname.push_back(Trace::levelname_struct("STAT*", Trace::Stats));
    Trace::levelname.push_back(Trace::levelname_struct("SOUR*", Trace::Source));
    Trace::levelname.push_back(Trace::levelname_struct("SRC*", Trace::Source));
    Trace::levelname.push_back(Trace::levelname_struct("L*CK*", Trace::Locking));
    Trace::levelname.push_back(Trace::levelname_struct("S*CK*", Trace::Sockets));
    Trace::levelname.push_back(Trace::levelname_struct("BIND*", Trace::Bind));
    Trace::levelname.push_back(Trace::levelname_struct("REG*", Trace::Bind));
    Trace::levelname.push_back(Trace::levelname_struct("HOOK*", Trace::Bind));
    Trace::levelname.push_back(Trace::levelname_struct("EXT*", Trace::External));
    Trace::levelname.push_back(Trace::levelname_struct("CHAT*", Trace::Chatter));
    Trace::levelname.push_back(Trace::levelname_struct("CHE*", Trace::CheckPoint));
    Trace::levelname.push_back(Trace::levelname_struct("C*P*", Trace::CheckPoint));
    Trace::levelname.push_back(Trace::levelname_struct("COM*", Trace::Comments));
    Trace::levelname.push_back(Trace::levelname_struct("F*NC*", Trace::Functions));
    Trace::levelname.push_back(Trace::levelname_struct("MOD*", Trace::Modify));
    Trace::levelname.push_back(Trace::levelname_struct("CH*G*", Trace::Changing));
#endif

#ifdef MAGICK_HAS_EXCEPTIONS
    try
    {
#endif
	// Globals ...
	Magick::StartTime(mDateTime::CurrentDateTime());

	int Result = MAGICK_RET_RESTART;

	mThread::Attach(tt_MAIN);
	while (Result == MAGICK_RET_RESTART)
	{
	    Magick instance(argc, argv);

	    Magick::register_instance(&instance);

	    Result = instance.Init();
	    if (Result != MAGICK_RET_NORMAL)
		continue;

	    Result = instance.Start();
	    if (Result != MAGICK_RET_NORMAL)
	    {
		instance.Finish();
		continue;
	    }
	    Result = instance.Run();
	    if (Result != MAGICK_RET_NORMAL)
	    {
		instance.Stop();
		instance.Finish();
		continue;
	    }

	    Result = instance.Stop();
	    if (Result != MAGICK_RET_NORMAL)
	    {
		instance.Finish();
		continue;
	    }

	    Result = instance.Finish();
	}
	mThread::Detach();

	// Specicl case, the bin is locked, and we aint got a key.
	if (Result == MAGICK_RET_LOCKED)
	    mFile::Erase(argv[0]);
#ifdef WIN32
	WSACleanup();
#endif
	return Result;
#ifdef MAGICK_HAS_EXCEPTIONS
    }
    catch(exception & e)
    {
	// new style STL exceptions
	ACE_OS::fprintf(stderr, "(EXC) Unhandled exception: %s\n", e.what());
	ACE_OS::fflush(stderr);
    }
    catch(char *str)
    {
	// exceptions from memory management
	ACE_OS::fprintf(stderr, "(STR) Unhandled exception: %s\n", str);
	ACE_OS::fflush(stderr);
    }
    catch(int i)
    {
	// old style c exceptions
	ACE_OS::fprintf(stderr, "(INT) Unhandled exception: %d\n", i);
	ACE_OS::fflush(stderr);
    }
    catch(...)
    {
	// even older style exceptions like SIGSEGV
	ACE_OS::fprintf(stderr, "(OTH) Unhandled exception.\n");
	ACE_OS::fflush(stderr);
    }
#endif
#ifdef WIN32
    WSACleanup();
#endif
    return MAGICK_RET_ERROR;
}
