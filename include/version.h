#ifndef WIN32
#pragma interface
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#ifndef _VERSION_H
#define _VERSION_H
#include "pch.h"
RCSID(version_h, "@(#) $Id$");

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

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"

const mstring BUILD_TIME = __DATE__ " " __TIME__;
const int BUILD_NUMBER = 1;

/* Versioning Information
 *
 * All of the below information should only EVER be
 * changed by the Magick Development Team except the
 * PATCH? tags, which may be updated using the
 * add-patch and rem-patch utilities provided in the
 * helper directory.  PLEASE do not change these
 * because you think it'd be funky to have 'MyNet
 * Services', we DO deserve SOME credit for what
 * we've done, don't you think?
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
 * are provided because often multiple patches are used
 * for various reasons, but if you need more than nine
 * patches on the original source, you should re-think.
 */
const mstring FULLNAME = "Magick IRC Services";
const mstring SLOGAN = "Power to the PEOPLE!";
const mstring EMAIL = "general@magick.tm";
const mstring HOMEPAGE = "http://www.magick.tm";
const mstring DOWNLOAD = "ftp://ftp.magick.tm/pub/magick.tm/Magick-II/beta";
const mstring RELEASE = "b12";
const mstring PATCH1 = "";
const mstring PATCH2 = "";
const mstring PATCH3 = "";
const mstring PATCH4 = "";
const mstring PATCH5 = "";
const mstring PATCH6 = "";
const mstring PATCH7 = "";
const mstring PATCH8 = "";
const mstring PATCH9 = "";

// This gets sent with /msg HelpServ CREDITS
const mstring credits[] = {
    FULLNAME + " - " + SLOGAN,
    "  (c) 1997-2002 Preston Elder <prez@magick.tm>",
    "  (c) 1998-2002 William King <ungod@magick.tm>",
    "",
    mstring(PACKAGE) + " is dedicated to the memory of PreZ's beloved",
    "sister, Stacey Louise Elder (Jan 1975 - Feb 1998) and",
    "Ungod's brother Edward Kevin King (Aug 1982 - Nov 1997).",
    "",
    "The " + mstring(PACKAGE) + " home page can be found at:",
    "    " + HOMEPAGE + "",
    "",
    mstring(PACKAGE) + " is freely downloadable from:",
    "    " + DOWNLOAD + "",
    "",
    "The " + mstring(PACKAGE) + " public mailing list is " + EMAIL + ".",
    "To subscribe, email majordomo@magick.tm with 'subscribe",
    mstring(PACKAGE) + "' (no quotes) in the text of the message.",
    "",
    "Please type /MSG %s CONTRIB for full credits (flood).",
    "---EOM---"
};

// This gets sent with /msg HelpServ CONTRIB
// or a /INFO request on the magick server.
const mstring contrib[] = {
    FULLNAME + " - " + SLOGAN,
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
    "    Nescafe Blend 43     See above (really, any kind of caffeine!)",
    "    Tschaicovski         If you don't understand ... you never will.",
    "",
    "Also Thanks To:",
    "    Kelly Hill, Emma Ellison, Marieke Bermon, Matt Heckaman",
    "",
    "Library Credits: ",
    "    ACE            http://www.cs.wustl.edu/~schmidt/ACE.html",
    "    cint           http://root.cern.ch/root/Cint.html",
    "    openssl        http://www.openssl.org (blowfish + md5)",
    "    expat          http://www.jclark.com/xml/expat.html",
    "    sxp            http://www.sourceforge.net/project/?group_id=2280",
    "---EOM---"
};

#endif
