#ifndef WIN32
#pragma interface
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _DCCENGINE_H
#define _DCCENGINE_H
#include "pch.h"
RCSID(dccengine_h, "@(#) $Id$");

/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.20  2002/01/14 07:16:54  prez
** More pretty printing with a newer indent with C++ fixes (not totally done)
**
** Revision 1.19  2002/01/12 14:42:08  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.18  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.17  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.16  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.13  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
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
    static vector < mstring > ctcpExtract(const mstring & in);
};

#endif
