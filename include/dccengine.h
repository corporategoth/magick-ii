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
#ifndef _DCCENGINE_H
#define _DCCENGINE_H
#include "mstring.h"

class DccEngine
{
public:
    void DoDccSend(ACE_INET_Addr addr, size_t size);
    void DoDccChat(ACE_INET_Addr addr);
    void GotDCC(const mstring& in);
    mstring encode(mstring& in);
    void decode(mstring& in);
    mstring lowQuote(mstring& in);
    mstring lowDequote(mstring& in);
    mstring ctcpQuote(mstring& in);
    mstring ctcpDequote(mstring& in);
    vector<mstring> ctcpExtract(mstring& in);
};

#endif
