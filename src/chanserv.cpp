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

#include "chanserv.h"
#include "lockable.h"
#include "magick.h"

mstring ChanServ::names = "";
ChanServ::ChanServ()
{
    NFT("ChanServ::ChanServ");
    on=false;
}
void ChanServ::execute(const mstring & servicename, const mstring & message)
{
    FT("ChanServ::execute", (servicename, message));
    //okay this is the main chanserv command switcher
}
