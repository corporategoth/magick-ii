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
#ifndef _CONVERT_AUSPICE_H
#define _CONVERT_AUSPICE_H
#include "pch.h"
RCSID(convert_auspice_h, "@(#) $Id$");

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

/* Based upon the DB loading routines from Auspice 2.8
 * (c) 2000-2001 In Mean <auspice@auspice.org>
 */

void auspice_load_nick();
void auspice_load_chan();
void auspice_load_memo();
void auspice_load_msg();
void auspice_load_trigger();
void auspice_load_akill();
void auspice_load_ignore();
void auspice_load_admin();

#endif /* CONVERT */
#endif /* _CONVERT_AUSPICE_H */
