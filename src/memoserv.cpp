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

#include "lockable.h"
#include "memoserv.h"
#include "magick.h"

MemoServ::MemoServ()
{
}

bool MemoServ::Memo()
{
    return memo;
}

bool MemoServ::News()
{
    return news;
}

void MemoServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_MemoServ);
    FT("MemoServ::execute", (data));
    //okay this is the main nickserv command switcher


    // Nick/Server PRIVMSG/NOTICE mynick :message

    mstring source, msgtype, mynick, message;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ");
    mynick  = data.ExtractWord(3, ": ");
    message = data.After(":", 2);

    mThread::ReAttach(tt_mBase);

}
