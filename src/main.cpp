#include "pch.h"
#ifdef _MSC_VER
#pragma hdrstop
#endif
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

#include "magick.h"
#include "datetime.h"
#include "lockable.h"

mDateTime Start_Time, Reset_Time;

//Magick *MagickObject;

int main(int argc, char **argv)
{
#ifdef MAGICK_HAS_EXCEPTIONS
    try
    {
#endif
	// todo make it insert itself into the ThreadID data structures.
	mThread::Attach(tt_MAIN, 1);

#ifdef DEBUG
	for (int i=tt_MAIN; i<tt_MAX; i++)
	    Trace::TurnSet((threadtype_enum) i, 0xffff); // Full tracing.
#else
	for (int i=tt_MAIN; i<tt_MAX; i++)
	    Trace::TurnSet((threadtype_enum) i, 0xffff&(~Trace::Functions)); // Full tracing - !functions.
#endif

	int Result;
	StartTime=Now();
restart:
	{
	    Magick internalobject(argc, argv);
	    ResetTime=Now();
	    Result=internalobject.Start();
	}
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
    }
    catch(int i)
    {
	// old style c exceptions
	fprintf(stderr,"Unhandled exception: %d\n",i);
    }
    catch(...)
    {
	// even older style exceptions like SIGSEGV
	fprintf(stderr,"Unhandled exception: Unknown\n");
	return -1;
    }
#endif
    return 0;
}