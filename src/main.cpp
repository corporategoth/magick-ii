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

#include <exception>
using namespace std;
#include "magick.h"
#include "datetime.h"
#include "lockable.h"

mDateTime Start_Time, Reset_Time;

Magick *MagickObject;

int start_server(int argc, char **argv)
{
    FT("start_server", (argc, "(char **) argv"));
    Reset_Time=Now();
    Magick internalobject(argc, argv);
    MagickObject=&internalobject;
    RET(internalobject.Start());
}

int main(int argc, char **argv)
{
#ifdef MAGICK_HAS_EXCEPTIONS
    try
    {
#endif
        Trace internaltrace;
	TraceObject = &internaltrace;
#ifdef DEBUG
	TraceObject->TurnSet(0xffffffff); // Full tracing.
#else
	TraceObject->TurnSet(0xffffffff&(~Trace::ALL_Functions)); // Full tracing - functions.
#endif
	// todo make it insert itself into the ThreadID data structures.
	mThread::Attach(tt_MAIN, 1);

	Start_Time=Now();
	int Result;
restart:
	Result=start_server(argc,argv);
	if(Result==MAGICK_RET_RESTART)
	    goto restart;
	mThread::Detach(tt_MAIN,1);
	return Result;
#ifdef MAGICK_HAS_EXCEPTIONS
    }
    catch(exception &e)
    {
	// new style STL exceptions
	fprintf(stderr,"Unhandled exception: %s\n",e.what());
	delete mainthread;
    }
    catch(int i)
    {
	// old style c exceptions
	fprintf(stderr,"Unhandled exception: %d\n",i);
	delete mainthread;
    }
    catch(...)
    {
	// even older style exceptions like SIGSEGV
	fprintf(stderr,"Unhandled exception: Unknown\n");
	delete mainthread;
	return -1;
    }
#endif
    return 0;
}