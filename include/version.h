#ifndef WIN32
  #pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copyright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _VERSION_H
#define _VERSION_H
#include "pch.h"
RCSID(version_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.127  2001/12/12 03:31:15  prez
** Re-wrote the occurances/find/replace functions in mstring to actually work
** with contents that includes a binary 0.  Also fixed PreParse in mconfig.
**
** Revision 1.36  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.35  2001/11/03 21:02:51  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.34  2001/07/24 02:51:13  prez
** Added ability to do JOIN or SJOIN
**
** Revision 1.33  2001/07/05 06:00:39  prez
** Up'd version to b11
**
** Revision 1.32  2001/05/28 11:17:33  prez
** Added some more anti-deadlock stuff, and fixed nick ident warnings
**
** Revision 1.31  2001/05/17 19:18:53  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.30  2001/05/06 03:03:07  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.29  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.28  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.24  2000/12/10 07:49:26  prez
** *** empty log message ***
**
** Revision 1.23  2000/10/03 05:36:27  prez
** Updated some makefiles, helper stuff, and headers -- nothing
** too earth shattering.
**
** Revision 1.22  2000/09/27 11:21:37  prez
** Added a BURST mode ...
**
** Revision 1.21  2000/09/19 08:05:24  prez
** Ran text files through spell checker.
**
** Revision 1.20  2000/08/22 08:43:39  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.19  2000/08/19 10:59:46  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.18  2000/08/07 22:40:52  prez
** *** empty log message ***
**
** Revision 1.17  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.16  2000/07/23 20:53:13  prez
** removing of antlr
**
** Revision 1.15  2000/07/11 13:22:18  prez
** Fixed loading/saving -- they now work with encryption and compression.
** Tested, it works too!  Now all we need to do is fix the loading, and
** we're set ... :))
**
** Revision 1.14  2000/06/08 13:07:34  prez
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

const mstring BUILD_TIME = __DATE__ " " __TIME__;
const int BUILD_NUMBER = 0;

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
const mstring FULLNAME			= "Magick IRC Services";
const mstring SLOGAN			= "Power to the PEOPLE!";
const mstring EMAIL			= "general@magick.tm";
const mstring HOMEPAGE			= "http://www.magick.tm";
const mstring DOWNLOAD			= "ftp://ftp.magick.tm/pub/magick.tm/Magick-II/snapshot";
const mstring RELEASE			= "s20011210";
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
"  (c) 1997-2001 Preston Elder <prez@magick.tm>",
"  (c) 1998-2001 William King <ungod@magick.tm>",
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
