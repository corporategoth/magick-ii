#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
#define RCSID(x,y) const char *rcsid_convert_interface_cpp_ ## x () { return y; }
RCSID(convert_interface_cpp, "@(#)$Id$");

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
#include "magick.h"
#include "convert/interface.h"

#include "convert/magick.h"
#include "convert/ircservices.h"
#include "convert/epona.h"
#include "convert/hybserv.h"
#include "convert/auspice.h"
#include "convert/ptlink.h"
#include "convert/sirv.h"
#include "convert/wrecked.h"
#include "convert/trircd.h"
#include "convert/cygnus.h"
#include "convert/srvx.h"
#include "convert/daylight.h"
#include "convert/ircs.h"
#include "convert/bolivia.h"

void convert_db(convert_t c)
{
    switch (c)
    {
    case CONVERT_MAGICK:
	magick_load_nick();
	magick_load_chan();
	magick_load_memo();
	magick_load_news();
	magick_load_akill();
	magick_load_clone();
	magick_load_sop();
	magick_load_message();
	break;
    case CONVERT_IRCSERVICES:
	LOG(LM_EMERGENCY, "COMMANDLINE/CANNOT_CONVERT", ("ircservices"));
	break;
    case CONVERT_EPONA:
	epona_load_nick();
	epona_load_chan();
	epona_load_news();
	epona_load_oper();
	epona_load_exception();
	break;
    case CONVERT_HYBSERV:
	hybserv_load_nick();
	hybserv_load_chan();
	hybserv_load_memo();
	hybserv_load_ignore();
	break;
    case CONVERT_AUSPICE:
	auspice_load_nick();
	auspice_load_chan();
	auspice_load_memo();
	auspice_load_msg();
	auspice_load_admin();
	auspice_load_trigger();
	auspice_load_akill();
	auspice_load_ignore();
	break;
    case CONVERT_PTLINK:
	ptlink_load_nick();
	ptlink_load_chan();
	ptlink_load_news();
	ptlink_load_akill();
	ptlink_load_oper();
	break;
    case CONVERT_SIRV:
	LOG(LM_EMERGENCY, "COMMANDLINE/CANNOT_CONVERT", ("sirv"));
	break;
    case CONVERT_WRECKED:
	wrecked_load_nick();
	wrecked_load_chan();
	wrecked_load_memo();
	wrecked_load_news();
	wrecked_load_akill();
	wrecked_load_clone();
	wrecked_load_sop();
	wrecked_load_message();
	break;
    case CONVERT_TRIRCD:
	LOG(LM_EMERGENCY, "COMMANDLINE/CANNOT_CONVERT", ("trircd"));
	break;
    case CONVERT_CYGNUS:
	cygnus_load_nick();
	cygnus_load_chan();
	cygnus_load_root();
	break;
    case CONVERT_SRVX:
	LOG(LM_EMERGENCY, "COMMANDLINE/CANNOT_CONVERT", ("srvx"));
	break;
    case CONVERT_DAYLIGHT:
	LOG(LM_EMERGENCY, "COMMANDLINE/CANNOT_CONVERT", ("daylight"));
	break;
    case CONVERT_IRCS:
	LOG(LM_EMERGENCY, "COMMANDLINE/CANNOT_CONVERT", ("ircs"));
	break;
    case CONVERT_BOLIVIA:
	bolivia_load_nick();
	bolivia_load_chan();
	bolivia_load_memo();
	bolivia_load_sa();
	bolivia_load_sop();
	bolivia_load_trigger();
	bolivia_load_akill();
	bolivia_load_ignore();
	break;
    default:
	break;
    }
}

#endif /* CONVERT */
