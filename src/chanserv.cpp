// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================

#include "chanserv.h"
#include "lockable.h"
#include "magick.h"

mstring ChanServ::names;		// Names of service (space delimited)
mstring ChanServ::realname;		// 'Real Name' of service
int ChanServ::expire;			// How long to keep channels
mstring ChanServ::def_akick_reason;	// Default AKICK reason
int ChanServ::chankeep;			// Time to keep channel after AKICK
mstring ChanServ::def_mlock;		// Default MLOCK string
mstring ChanServ::lck_mlock;		// Locked MLOCK modes
bool ChanServ::def_keeptopic;		// Default val of KEEPTOPIC
bool ChanServ::lck_keeptopic;		// KEEPTOPIC is locked?
bool ChanServ::def_topiclock;		// Default val of TOPICLOCK
bool ChanServ::lck_topiclock;		// TOPICLOCK is locked?
bool ChanServ::def_private;		// Default value of PRIVATE
bool ChanServ::lck_private;		// PRIVATE is locked?
bool ChanServ::def_secureops;		// Default value of SECUREOPS
bool ChanServ::lck_secureops;		// SECUREOPS is locked?
bool ChanServ::def_secure;		// Default value of SECURE
bool ChanServ::lck_secure;		// SECURE is locked?
bool ChanServ::def_restricted;		// Default value of RESTRICTED
bool ChanServ::lck_restricted;		// RESTRICTED is locked?
bool ChanServ::def_join;		// Default value of JOIN
bool ChanServ::lck_join;		// JOIN is locked?
mstring ChanServ::def_revenge;		// Default REVENGE level
bool ChanServ::lck_revenge;		// REVENGE is locked?
int ChanServ::level_min;		// Minimum access level
int ChanServ::level_max;		// Maximum access level
int ChanServ::lvl_autodeop;		// Default level for AUTODEOP
int ChanServ::lvl_autovoice;		// Default level for AUTOVOICE
int ChanServ::lvl_autoop;		// Default level for AUTOOP
int ChanServ::lvl_readmemo;		// Default level for READMEMO
int ChanServ::lvl_writememo;		// Default level for WRITEMEMO
int ChanServ::lvl_delmemo;		// Default level for DELMEMO
int ChanServ::lvl_akick;		// Default level for AKICK
int ChanServ::lvl_starakick;		// Default level for STARAKICK
int ChanServ::lvl_unban;		// Default level for UNBAN
int ChanServ::lvl_access;		// Default level for ACCESS
int ChanServ::lvl_set;			// Default level for SET
int ChanServ::lvl_cmdinvite;		// Default level for CMDINVITE
int ChanServ::lvl_cmdunban;		// Default level for CMDUNBAN
int ChanServ::lvl_cmdvoice;		// Default level for CMDVOICE
int ChanServ::lvl_cmdop;		// Default level for CMDOP
int ChanServ::lvl_cmdclear;		// Default level for CMDCLEAR

bool ChanServ::messages;		// Wether to process /MSG, /NOTICE.
bool ChanServ::automation;		// Wether to do automatic tasks.

ChanServ::ChanServ()
{
    NFT("ChanServ::ChanServ");
    if (mThread::findbytype(Get_TType(), 1) == NULL) {
	messages=true;
	automation=true;
    }
}
void ChanServ::execute(const mstring & servicename, const mstring & message)
{
    FT("ChanServ::execute", (servicename, message));
    //okay this is the main chanserv command switcher
}
