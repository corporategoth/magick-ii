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
#ifndef _VERSION_H
#define _VERSION_H
static const char *ident_version_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.74  2000/03/15 14:42:58  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.73  2000/03/14 15:10:15  prez
** OK -- more stuff with SJOIN/SNICK -- but it WORKS!!!
**
** Revision 1.72  2000/03/14 13:37:35  prez
** *** empty log message ***
**
** Revision 1.71  2000/03/14 10:02:48  prez
** Added SJOIN and SNICK
**
** Revision 1.70  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.69  2000/03/02 07:25:10  prez
** Added stuff to do the chanserv greet timings (ie. only greet if a user has
** been OUT of channel over 'x' seconds).  New stored chanserv cfg item.
**
** Revision 1.68  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.67  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.66  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"

const mstring BUILD_TIME = __DATE__ " " __TIME__;
const int BUILD_NUMBER = 165;
const mstring BUILD_TYPE = "SunOS 5.7 sun4u sparc";
const mstring BUILD_SYS = "castle";
const unsigned short Magick_Major_Ver=2;
const unsigned short Magick_Minor_Ver=0;
const unsigned long FileVersionNumber=(Magick_Major_Ver*0x10000)+Magick_Minor_Ver;
const mstring FileIdentificationTag = "DFLY";

/* Versioning Information
 *
 * The PRODUCT and VERSION should only ever be changed
 * by the Magick Development Team.  PLEASE do not change
 * this because you think it'd be funky to have 'MyNet
 * services', we DO diserve SOME creditation for what
 * we've done, dont you think?  The PRODUCT text may
 * only be ONE word (else it stuffs up version replies).
 *
 * For a FULL release product (non-alpha/beta), the
 * RELEASE value will be blank (else it will contain
 * either aN or bN (where n is the alpha or beta number
 * respectively).  It may also contain pre for pre-
 * release code.
 *
 * Up to 9 patch levels are allowed for, so if you write
 * a patch for magick that has to alter the actual code,
 * add your patch name to one of the patch levels.  Nine
 * are provided because often multipal patches are used
 * for various reasons, but if you need more than nine
 * patches on the original source, you should re-think.
 */
const mstring FULL_NAME		= "Magick IRC Services";
const mstring FULL_EMAIL	= "magick@magick.tm";
const mstring FULL_URL		= "http://www.magick.tm";
const mstring PRODUCT		= "Magick";
const mstring RELEASE		= "a1";
const mstring PATCH1		= "";
const mstring PATCH2		= "";
const mstring PATCH3		= "";
const mstring PATCH4		= "";
const mstring PATCH5		= "";
const mstring PATCH6		= "";
const mstring PATCH7		= "";
const mstring PATCH8		= "";
const mstring PATCH9		= "";

// This gets sent with /msg HelpServ CREDITS
const mstring credits[] = {
"Magick IRC Services",
"",
"Magick is dedicated to the memory of PreZ's beloved",
"sister, Stacey Louise Elder (Jan 1975 - Feb 1998).",
"",
"The Magick home page can be found at:",
"    http://www.magick.tm",
"",
"Magick is freely downloadable from:",
"    ftp://ftp.magick.tm/pub/Magick, or",
"    http://www.magick.tm/download",
"",
"The Magick public mailing list is magick@magick.tm.",
"To subscribe, email majordomo@magick.tm with 'subscribe",
"magick' (no quotes) in the text of the message.",
"",
"Please type /MSG %s CONTRIB for full credits (flood).",
"---EOM---"
};

// This gets sent with /msg HelpServ CONTRIB
// or a /INFO request on the magick server.
const mstring contrib[] = {
"Magick IRC Services",
"",
"Programmers: ",
"    PreZ           Preston A. Elder        prez@magick.tm",
"    Ungod          William King            ungod@magick.tm",
"",
"Design Team: ",
"    Lord_Striker   Patrick Prins           striker@magick.tm",
"",
"Special Thanks To:",
"    Coca Cola Amatil     Life support ... *bleep, bleep*",
"    Nescafe Blend 43     See above (really, any kind of caffine!)",
"    Tschaicovski         If you dont understand ... you never will.",
"",
"Also Thanks To:",
"    Kelly Hill, Emma Ellison, Marieke Bermon",
"",
"Library Creditations: ",
"    ACE            http://www.cs.wustl.edu/~schmidt",
"    wxWindows      http://web.ukonline.co.uk/julian.smart/wxwin",
"    antlr          http://www.antlr.org",
"    libdes         http://www.psy.uq.oz.au/~ftp/Crypto",
"    zlib           http://www.cdrom.com/pub/infozip/zlib",
"    cint           http://root.cern.ch/root/Cint.html",
"---EOM---"
};

#endif
