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

#include "magickmemory.hpp"
#include "magick.h"
#include "datetime.h"
#include <exception>
using namespace std;
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
    Trace internaltrace;
    TraceObject = &internaltrace;
    TraceObject->TurnSet(0x44444444);
    mainthread = new ThreadID(tt_MAIN, 1);
    Start_Time=Now();
    try
    {
	int Result;
restart:
	Result=start_server(argc,argv);
	if(Result==MAGICK_RET_RESTART)
	    goto restart;
	delete mainthread;
	return Result;
    }
    catch(exception &e)
    {
	fprintf(stderr,"Unhandled exception: %s\n",e.what());
	delete mainthread;
    }
    catch(...)
    {
	fprintf(stderr,"Unhandled exception: Unknown\n");
	delete mainthread;
	return -1;
    }
}