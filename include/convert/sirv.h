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
#ifndef _CONVERT_SIRV_H
#define _CONVERT_SIRV_H
#include "pch.h"
RCSID(convert_sirv_h, "@(#) $Id$");

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

/* Based upon the DB loading routines from SirvNET Services 2.9.0
 * (c) 1998-2002 Trevor Klingbeil <priority1@dal.net>
 */

void sirv_load_nick();
void sirv_load_chan();
void sirv_load_memo();
void sirv_load_sa();
void sirv_load_sop();
void sirv_load_trigger();
void sirv_load_akill();
void sirv_load_ignore();

#endif /* CONVERT */
#endif /* _CONVERT_SIRV_H */
