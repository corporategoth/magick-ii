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

#include "chanserv.h"
#include "lockable.h"
#include "magick.h"

ChanServ::ChanServ(Magick *in_Parent) : mBase(in_Parent)
{
    NFT("ChanServ::ChanServ");
    messages=true;
    automation=true;
}
void ChanServ::execute(const mstring & data)
{
    FT("ChanServ::execute", (data));
    //okay this is the main chanserv command switcher

    mThread::Detach(tt_mBase);
    mThread::Attach(Parent, tt_ChanServ);

    mstring source, msgtype, mynick, message;
    source  = data.Before(" ");
    msgtype = data.After(" ").Before(" ");
    mynick  = data.After(" ").After(" ").Before(" ");
    message = data.After(":");


    mThread::Detach(tt_ChanServ);
    mThread::Attach(Parent,tt_mBase);
}
