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
#include "servmsg.h"
#include "magick.h"

ServMsg::ServMsg()
{
}

void ServMsg::execute(const mstring & data)
{
    mThread::ReAttach(tt_OtherServ);
    FT("ServMsg::execute", (data));
    //okay this is the main nickserv command switcher


    // Nick/Server PRIVMSG/NOTICE mynick :message

    mstring source, msgtype, mynick, message;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ").UpperCase();
    mynick  = data.ExtractWord(3, ": ");
    message = data.After(":", 2);

    if (message[0U] == CTCP_DELIM_CHAR)
    {
	if (msgtype == "PRIVMSG")
	    DccEngine::decodeRequest(mynick, source, message);
	else
	    DccEngine::decodeReply(mynick, source, message);
    }

    mThread::ReAttach(tt_mBase);

}

void ServMsg::load_database(wxInputStream& in)
{
}

void ServMsg::save_database(wxOutputStream& out)
{
}
