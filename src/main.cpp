#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#endif

/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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
#include "datetime.h"
#include "lockable.h"

mDateTime Start_Time, Reset_Time;

//Magick *MagickObject;

int main(int argc, char **argv)
{
#ifdef MAGICK_TRACE_WORKS
    Trace::levelname.push_back(Trace::levelname_struct( "OFF", Trace::Off ));
    Trace::levelname.push_back(Trace::levelname_struct( "FULL", Trace::Full ));
    Trace::levelname.push_back(Trace::levelname_struct( "ALL", Trace::Full ));
    Trace::levelname.push_back(Trace::levelname_struct( "STAT*", Trace::Stats ));
    Trace::levelname.push_back(Trace::levelname_struct( "SOUR*", Trace::Source ));
    Trace::levelname.push_back(Trace::levelname_struct( "SRC*", Trace::Source ));
    Trace::levelname.push_back(Trace::levelname_struct( "L*CK*", Trace::Locking ));
    Trace::levelname.push_back(Trace::levelname_struct( "S*CK*", Trace::Sockets ));
    Trace::levelname.push_back(Trace::levelname_struct( "BIND*", Trace::Bind ));
    Trace::levelname.push_back(Trace::levelname_struct( "REG*", Trace::Bind ));
    Trace::levelname.push_back(Trace::levelname_struct( "HOOK*", Trace::Bind ));
    Trace::levelname.push_back(Trace::levelname_struct( "EXT*", Trace::External ));
    Trace::levelname.push_back(Trace::levelname_struct( "CHAT*", Trace::Chatter ));
    Trace::levelname.push_back(Trace::levelname_struct( "CHE*", Trace::CheckPoint ));
    Trace::levelname.push_back(Trace::levelname_struct( "C*P*", Trace::CheckPoint ));
    Trace::levelname.push_back(Trace::levelname_struct( "COM*", Trace::Comments ));
    Trace::levelname.push_back(Trace::levelname_struct( "F*NC*", Trace::Functions ));
    Trace::levelname.push_back(Trace::levelname_struct( "MOD*", Trace::Modify ));
    Trace::levelname.push_back(Trace::levelname_struct( "CHANG*", Trace::Changing ));
#endif

#ifdef MAGICK_HAS_EXCEPTIONS
    try
    {
#endif

	int Result;
        mThread::Attach(tt_MAIN);
	StartTime=Now();
restart:
	{
	    Magick internalobject(argc, argv);
	    Result=internalobject.Start();
	}
	if(Result==MAGICK_RET_RESTART) {
	    mThread::Detach();
	    goto restart;
	}
	return Result;
#ifdef MAGICK_HAS_EXCEPTIONS
    }
    catch(exception &e)
    {
	// new style STL exceptions
	ACE_OS::fprintf(stderr,"(EXC) Unhandled exception: %s\n",e.what()); ACE_OS::fflush(stderr);
    }
    catch(char *str)
    {
	// exceptions from memory management
	ACE_OS::fprintf(stderr,"(STR) Unhandled exception: %s\n",str); ACE_OS::fflush(stderr);
    }
    catch(int i)
    {
	// old style c exceptions
	ACE_OS::fprintf(stderr,"(INT) Unhandled exception: %d\n",i); ACE_OS::fflush(stderr);
    }
    catch(...)
    {
	// even older style exceptions like SIGSEGV
	ACE_OS::fprintf(stderr,"(OTH) Unhandled exception.\n"); ACE_OS::fflush(stderr);
	return -1;
    }
#endif
    return 0;
}
