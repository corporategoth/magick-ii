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
** Revision 1.84  2000/04/06 12:44:09  prez
** Removed SXP and EXPAT directories
**
** Revision 1.83  2000/04/04 03:13:50  prez
** Added support for masking hostnames.
**
** Revision 1.82  2000/04/02 13:06:03  prez
** Fixed the channel TOPIC and MODE LOCK stuff ...
**
** Also fixed the setting of both on join...
**
** Revision 1.81  2000/04/02 07:25:05  prez
** Fixed low watermarks with threads, it all works now!
**
** Revision 1.80  2000/03/29 14:00:18  prez
** Fixed the thread pool system, and the watermarks.
**
** Revision 1.79  2000/03/28 16:20:57  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.78  2000/03/27 21:26:11  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.77  2000/03/26 14:59:36  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.11  2000/03/25 04:26:48  prez
** Added tracing into filesys for easier detection as to why it will not
** receive data.  Also put version number to a2 now, ready for release soon.
**
** Revision 1.10  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.9  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.8  2000/02/15 10:37:47  prez
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
const int BUILD_NUMBER = 111;
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
const mstring RELEASE		= "pre-a2";
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
"sister, Stacey Louise Elder (Jan 1975 - Feb 1998) and",
"Ungod's brother Edward Kevin King (Aug 1982 - Nov 1997).",
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
"    Kelly Hill, Emma Ellison, Marieke Bermon, Matt Heckaman",
"",
"Library Creditations: ",
"    ACE            http://www.cs.wustl.edu/~schmidt/ACE.html",
"    wxWindows      http://web.ukonline.co.uk/julian.smart/wxwin",
"    antlr          http://www.antlr.org",
"    libdes         http://www.psy.uq.oz.au/~ftp/Crypto",
"    zlib           http://www.cdrom.com/pub/infozip/zlib",
"    cint           http://root.cern.ch/root/Cint.html",
"---EOM---"
};

#endif
