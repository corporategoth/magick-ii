#ifndef WIN32
#pragma interface
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
#ifndef _DCCENGINE_H
#define _DCCENGINE_H
static const char *ident_dccengine_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.12  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.11  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

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
