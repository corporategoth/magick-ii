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
** Revision 1.98  2000/06/15 11:28:04  ungod
** #include config.h so that the PACKAGE works properly
**
** Revision 1.97  2000/06/08 13:07:34  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.13  2000/05/22 13:00:08  prez
** Updated version.h and some other stuff
**
** Revision 1.12  2000/03/26 14:59:36  prez
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
#include "config.h"

const mstring BUILD_TIME = __DATE__ " " __TIME__;
const int BUILD_NUMBER = 1;
const mstring BUILD_TYPE = "Linux 2.2.14 i686 unknown";
const mstring BUILD_SYS = "haven";

/* Versioning Information
 *
 * All of the below information should only EVER be
 * changed by the Magick Development Team except the
 * PATCH? tags, which may be updated using the
 * add-patch and rem-patch utilies provided in the
 * helper directory.  PLEASE do not change these
 * because you think it'd be funky to have 'MyNet
 * Services', we DO diserve SOME creditation for what
 * we've done, dont you think?  The PACKAGE text may
 * only be ONE word (else it stuffs up version replies).
 *
 * For a FULL release product (non-alpha/beta), the
 * VERSION value will be blank (else it will contain
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
const mstring FULLNAME			= "Magick IRC Services";
const mstring SLOGAN			= "Power to the PEOPLE!";
const mstring EMAIL			= "magick@magick.tm";
const mstring HOMEPAGE			= "http://www.magick.tm";
const mstring DOWNLOAD			= "ftp://ftp.magick.tm/pub/Magick";
const mstring RELEASE			= "pre-a2";
const mstring PATCH1			= "";
const mstring PATCH2			= "";
const mstring PATCH3			= "";
const mstring PATCH4			= "";
const mstring PATCH5			= "";
const mstring PATCH6			= "";
const mstring PATCH7			= "";
const mstring PATCH8			= "";
const mstring PATCH9			= "";

// This gets sent with /msg HelpServ CREDITS
const mstring credits[] = {
FULLNAME+" - "+SLOGAN,
"  (c) 1997-2000 Preston Elder <prez@magick.tm>",
"  (c) 1998-2000 William King <ungod@magick.tm>",
"",
mstring(PACKAGE)+" is dedicated to the memory of PreZ's beloved",
"sister, Stacey Louise Elder (Jan 1975 - Feb 1998) and",
"Ungod's brother Edward Kevin King (Aug 1982 - Nov 1997).",
"",
"The "+mstring(PACKAGE)+" home page can be found at:",
"    "+HOMEPAGE+"",
"",
mstring(PACKAGE)+" is freely downloadable from:",
"    "+DOWNLOAD+"",
"",
"The "+mstring(PACKAGE)+" public mailing list is "+EMAIL+".",
"To subscribe, email majordomo@magick.tm with 'subscribe",
mstring(PACKAGE)+"' (no quotes) in the text of the message.",
"",
"Please type /MSG %s CONTRIB for full credits (flood).",
"---EOM---"
};

// This gets sent with /msg HelpServ CONTRIB
// or a /INFO request on the magick server.
const mstring contrib[] = {
FULLNAME+" - "+SLOGAN,
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
"    antlr          http://www.antlr.org",
"    libdes         http://www.psy.uq.oz.au/~ftp/Crypto",
"    zlib           http://www.cdrom.com/pub/infozip/zlib",
"    cint           http://root.cern.ch/root/Cint.html",
"---EOM---"
};

#endif
