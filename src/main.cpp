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
