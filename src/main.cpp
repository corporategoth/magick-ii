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
    Trace::levelname.push_back(Trace::levelname_struct( "COM*", Trace::CheckPoint ));
    Trace::levelname.push_back(Trace::levelname_struct( "F*NC*", Trace::Functions ));
    Trace::levelname.push_back(Trace::levelname_struct( "MOD*", Trace::Modify ));
    Trace::levelname.push_back(Trace::levelname_struct( "CHANG*", Trace::Changing ));

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
	    mThread::Detach(tt_MAIN);
	    goto restart;
	}
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
