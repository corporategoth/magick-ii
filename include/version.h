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
** ==========================================================
#pragma ident "$Id$"
** ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.66  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#ifndef _VERSION_H
#define _VERSION_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"

const mstring BUILD_TIME = __DATE__ " " __TIME__;
const int BUILD_NUMBER = 0;
const mstring BUILD_TYPE = "Windows 95/98/NT";
const mstring BUILD_SYS = "localhost";
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
"    rx             http://www.gnu.org/software/rx",
"    antlr          http://www.antlr.org",
"    libdes         http://www.psy.uq.oz.au/~ftp/Crypto",
"    zlib           http://www.cdrom.com/pub/infozip/zlib",
"    cint           http://root.cern.ch/root/Cint.html",
"---EOM---"
};

#endif
