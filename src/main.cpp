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
    Start_Time=Now();
    try
    {
	int Result;
restart:
	Result=start_server(argc,argv);
	if(Result==MAGICK_RET_RESTART)
	    goto restart;
	return Result;
    }
#ifdef WIN32  // prez tell me if you can find the below constants in gcc
    catch(int E)
    {
	mstring type;
	switch(E)
	{
	    case EXCEPTION_ACCESS_VIOLATION:
		type="EXCEPTION_ACCESS_VIOLATION";
		break;
	    case EXCEPTION_BREAKPOINT:
		type="EXCEPTION_BREAKPOINT";
		break;
	    case EXCEPTION_DATATYPE_MISALIGNMENT:
		type="EXCEPTION_DATATYPE_MISALIGNMENT";
		break;
	    case EXCEPTION_SINGLE_STEP:
		type="EXCEPTION_SINGLE_STEP";
		break;
	    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		type="EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		break;
	    case EXCEPTION_FLT_DENORMAL_OPERAND:
		type="EXCEPTION_FLT_DENORMAL_OPERAND";
		break;
	    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		type="EXCEPTION_FLT_DIVIDE_BY_ZERO";
		break;
	    case EXCEPTION_FLT_INEXACT_RESULT:
		type="EXCEPTION_FLT_INEXACT_RESULT";
		break;
	    case EXCEPTION_FLT_INVALID_OPERATION:
		type="EXCEPTION_FLT_INVALID_OPERATION";
		break;
	    case EXCEPTION_FLT_OVERFLOW:
		type="EXCEPTION_FLT_OVERFLOW";
		break;
	    case EXCEPTION_FLT_STACK_CHECK:
		type="EXCEPTION_FLT_STACK_CHECK";
		break;
	    case EXCEPTION_FLT_UNDERFLOW:
		type="EXCEPTION_FLT_UNDERFLOW";
		break;
	    case EXCEPTION_INT_DIVIDE_BY_ZERO:
		type="EXCEPTION_INT_DIVIDE_BY_ZERO";
		break;
	    case EXCEPTION_INT_OVERFLOW:
		type="EXCEPTION_INT_OVERFLOW";
		break;
	    case EXCEPTION_PRIV_INSTRUCTION:
		type="EXCEPTION_PRIV_INSTRUCTION";
		break;
	    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		type="EXCEPTION_NONCONTINUABLE_EXCEPTION";
		break;
	    default:
		type="Unknown Exception";
		break;
	}
	printf("Unhandled exception: %s\n",type.c_str());
	return -1;
    }
#endif
    catch(...)
    {
	printf("Unhandled exception\n");
	return -1;
    }
}