#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_main_cpp_ ## x () { return y; }
RCSID(main_cpp, "@(#)$Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

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
    catch (exception & e)
    {
	// new style STL exceptions
	ACE_OS::fprintf(stderr, "(EXC) Unhandled exception: %s\n", e.what());
	ACE_OS::fflush(stderr);
    }
    catch (char *str)
    {
	// exceptions from memory management
	ACE_OS::fprintf(stderr, "(STR) Unhandled exception: %s\n", str);
	ACE_OS::fflush(stderr);
    }
    catch (int i)
    {
	// old style c exceptions
	ACE_OS::fprintf(stderr, "(INT) Unhandled exception: %d\n", i);
	ACE_OS::fflush(stderr);
    }
    catch (...)
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
