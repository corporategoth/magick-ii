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
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#ifndef _CONVERT_WRECKED_H
#define _CONVERT_WRECKED_H
#include "pch.h"
RCSID(convert_wrecked_h, "@(#) $Id$");

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

/* Based upon the DB loading routines from Wrecked 1.2.0
 * (c) 2000 Darcy Grexton <skold@wrecked.net>
 */

void wrecked_load_nick();
void wrecked_load_chan();
void wrecked_load_memo();
void wrecked_load_news();
void wrecked_load_sop();
void wrecked_load_message();
void wrecked_load_akill();
void wrecked_load_clone();

#endif /* CONVERT */
#endif /* _CONVERT_WRECKED */