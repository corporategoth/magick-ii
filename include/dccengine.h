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

const char CTCP_DELIM_CHAR='\001';
const char CTCP_QUOTE_CHAR='\\';
const char CTCP_MQUOTE_CHAR='\020';
//#define CTCP_QUOTE_EM   "\r\n\001\\"

class DccEngine
{
public:
    static void DoDccSend(const mstring& mynick, const mstring& source,
		   ACE_INET_Addr addr, mstring filename, size_t size);
    static void DoDccChat(const mstring& mynick, const mstring& source,
		   ACE_INET_Addr addr);
    static void GotDCC(const mstring& mynick, const mstring& source,
		mstring in);
    static mstring encode(const mstring type, const mstring& in = "");
    static void decodeReply(const mstring& mynick, const mstring& source,
		mstring& in);
    static void decodeRequest(const mstring& mynick, const mstring& source,
		mstring& in);
    static mstring lowQuote(mstring& in);
    static mstring lowDequote(mstring& in);
    static mstring ctcpQuote(mstring& in);
    static mstring ctcpDequote(mstring& in);
    static vector<mstring> ctcpExtract(mstring& in);
};

#endif
