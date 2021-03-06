#ifndef WIN32
#pragma interface
#endif

/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#ifndef _DCCENGINE_H
#define _DCCENGINE_H
#include "pch.h"
RCSID(dccengine_h, "@(#) $Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "mstring.h"

const char CTCP_DELIM_CHAR = '\001';
const char CTCP_QUOTE_CHAR = '\\';
const char CTCP_MQUOTE_CHAR = '\020';

//#define CTCP_QUOTE_EM   "\r\n\001\\"

class DccEngine
{
public:
    static void DoDccSend(const mstring & mynick, const mstring & source, const ACE_INET_Addr & addr, mstring & filename,
			  const size_t size);
    static void DoDccChat(const mstring & mynick, const mstring & source, const ACE_INET_Addr & addr);
    static void GotDCC(const mstring & mynick, const mstring & source, const mstring & in);
    static mstring encode(const mstring & type, const mstring & in = "");
    static void decodeReply(const mstring & mynick, const mstring & source, const mstring & in);
    static void decodeRequest(const mstring & mynick, const mstring & source, const mstring & in);
    static mstring lowQuote(const mstring & in);
    static mstring lowDequote(const mstring & in);
    static mstring ctcpQuote(const mstring & in);
    static mstring ctcpDequote(const mstring & in);
    static mstring addressEncode(const mstring & in);
    static mstring addressDecode(const mstring & in);
    static vector < mstring > ctcpExtract(const mstring & in);
};

#endif
