#include "pch.h"
#ifdef WIN32
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

#include "lockable.h"
#include "operserv.h"
#include "magick.h"

OperServ::OperServ()
{
}

mstring OperServ::Services_Admin()
{
    return services_admin;
}

int OperServ::Flood_Time()
{
    return flood_time;
}

int OperServ::Flood_Msgs()
{
    return flood_msgs;
}

bool OperServ::Akill()
{
    return akill;
}

bool OperServ::Flood()
{
    return flood;
}

bool OperServ::OperDeny()
{
    return operdeny;
}

void OperServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_OperServ);
    FT("OperServ::execute", (data));
    //okay this is the main nickserv command switcher


    // Nick/Server PRIVMSG/NOTICE mynick :message

    mstring source, msgtype, mynick, message;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ");
    mynick  = data.ExtractWord(3, ": ");
    message = data.After(":", 2);

    mThread::ReAttach(tt_mBase);

}

