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
#ifndef _CONVERT_TRIRCD_H
#define _CONVERT_TRIRCD_H
#include "pch.h"
RCSID(convert_trircd_h, "@(#) $Id$");

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

#ifdef CONVERT

/* Based upon the DB loading routines from trircd-services 4.26
 * (c) 1999-2000 Andrew Church <achurch@achurch.org>
 */

void trircd_load_nick();
void trircd_load_nickext();
void trircd_load_chan();
void trircd_load_chanext();
void trircd_load_news();
void trircd_load_akill();
void trircd_load_oper();
void trircd_load_exception();

#endif /* CONVERT */
#endif /* _CONVERT_TRIRCD_H */
