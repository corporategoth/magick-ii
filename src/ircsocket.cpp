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

#include "ircsocket.h"

int IrcSvcHandler::open(void *in)
{
    ACE_Reactor::instance()->register_handler(this,ACE_Event_Handler::READ_MASK);
    return 0;
}
int IrcSvcHandler::handle_input(ACE_HANDLE hin)
{
    //todo this is the sucker that get's data from the socket, so this is our main routine.
    // might set this up to be an active object here.
    return 0;
}

