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
mDateTime Reset_Time;

Magick *MagickObject;

int start_server(int argc, char **argv)
{
    Reset_Time=Now();
    Magick internalobject(argc, argv);
    MagickObject=&internalobject;
    return internalobject.Start();
}
int main(int argc, char **argv)
{
    try
    {
	int Result;
restart:
	Result=start_server(argc,argv);
	if(Result==MAGICK_RET_RESTART)
	    goto restart;
	return Result;
    }
    catch(...)
    {
	printf("Unhandled exception\n");
    }
}