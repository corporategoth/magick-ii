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
#ifndef _CONVERT_PTLINK_H
#define _CONVERT_PTLINK_H
#include "pch.h"
RCSID(convert_ptlink_h, "@(#) $Id$");

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

/* Based upon the DB loading routines from PTlink Services 2.22.3
 * (c) 1999-2002 PTlink Coders Team <coders@ptlink.net>
 */

void ptlink_load_nick();
void ptlink_load_chan();
void ptlink_load_news();
void ptlink_load_akill();
void ptlink_load_oper();

#endif /* CONVERT */
#endif /* _CONVERT_PTLINK_H */
