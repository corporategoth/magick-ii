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
#define RCSID(x,y) const char *rcsid_chanserv_cpp_ ## x () { return y; }
RCSID(chanserv_cpp, "@(#)$Id$");

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

#include "magick.h"
#include "dccengine.h"

#ifdef __BORLAND__
#pragma codeseg CODE2
#endif

ChanServ::ChanServ()
{
    BTCB();
    NFT("ChanServ::ChanServ");
    messages = true;
    Revenge_Levels.insert("NONE");
    Revenge_Levels.insert("REVERSE");
    Revenge_Levels.insert("DEOP");
    Revenge_Levels.insert("KICK");
    Revenge_Levels.insert("BAN1");
    Revenge_Levels.insert("BAN2");
    Revenge_Levels.insert("BAN3");
    Revenge_Levels.insert("BAN4");
    Revenge_Levels.insert("MIRROR");
    ETCB();
}

void ChanServ::AddCommands()
{
    BTCB();
    NFT("ChanServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLEAR* USER*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_clear_Users);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLEAR* OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_clear_Ops);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLEAR* H*OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_clear_HalfOps);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLEAR* VOICE*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_clear_Voices);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLEAR* MODE*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_clear_Modes);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLEAR* BAN*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_clear_Bans);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLEAR* ALL", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_clear_All);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLEAR* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LEV* SET*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_level_Set);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LEV* RESET*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_level_Reset);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LEV* LIST", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_level_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LEV* VIEW", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_level_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LEV* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* ADD", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_access_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* DEL*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_access_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* REM*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_access_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* LIST", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_access_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* VIEW", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_access_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KICK ADD", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_akick_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KICK DEL*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_akick_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KICK REM*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_akick_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KICK LIST", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_akick_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KICK VIEW", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_akick_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KICK H*LP", Magick::instance().commserv.REGD_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GREET* ADD", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_greet_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GREET* DEL*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_greet_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GREET* REM*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_greet_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GREET* LIST", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_greet_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GREET* VIEW", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_greet_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GREET* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "M*S*G* ADD", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_message_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "M*S*G* DEL*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_message_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "M*S*G* REM*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_message_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "M*S*G* LIST", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_message_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "M*S*G* VIEW", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_message_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "M*S*G* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* FOUND*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Founder);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* COFOUND*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_CoFounder);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* DESC*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Description);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PASS*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Password);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* E*MAIL*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Email);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* URL", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* WWW*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* WEB*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* COMM*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_set_Comment);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* M*LOCK", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Mlock);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* BAN*TIME", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_BanTime);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PART*TIME", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_PartTime);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* KEEP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_KeepTopic);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* TOPIC*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_TopicLock);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PRIV*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* SEC*OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_SecureOps);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* SEC*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* NOEX*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_set_NoExpire);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* ANAR*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Anarchy);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* KICK*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_KickOnBan);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* RES*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Restricted);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *JOIN*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Join);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* REV*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_set_Revenge);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK M*LOCK", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_Mlock);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK BAN*TIME", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_BanTime);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK PART*TIME", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_PartTime);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK KEEP*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_KeepTopic);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK TOPIC*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_TopicLock);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK PRIV*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK SEC*OP*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_SecureOps);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK SEC*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK ANAR*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_Anarchy);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK KICK*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_KickOnBan);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK RES*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_Restricted);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK *JOIN*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_Join);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK REV*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_lock_Revenge);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK H*LP", Magick::instance().commserv.SOP_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK M*LOCK", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_Mlock);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK BAN*TIME", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_BanTime);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK PART*TIME", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_PartTime);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK KEEP*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_KeepTopic);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK TOPIC*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_TopicLock);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK PRIV*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK SEC*OP*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_SecureOps);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK SEC*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK ANAR*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_Anarchy);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK KICK*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_KickOnBan);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK RES*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_Restricted);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK *JOIN*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_Join);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK REV*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_unlock_Revenge);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK H*LP", Magick::instance().commserv.SOP_Name(),
						 do_3param);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name(),
						 ChanServ::do_Help);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "REG*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Register);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "DROP", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Drop);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ID*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Identify);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "INFO*", Magick::instance().commserv.ALL_Name(),
						 ChanServ::do_Info);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LIST*", Magick::instance().commserv.ALL_Name(),
						 ChanServ::do_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SUSP*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_Suspend);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNSUSP*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_UnSuspend);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FORB*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_Forbid);
#ifdef GETPASS
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GETPASS*", Magick::instance().commserv.SADMIN_Name(),
						 ChanServ::do_Getpass);
#endif
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SETPASS*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_Setpass);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "MODE*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Mode);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_HalfOp);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "D*H*OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_DeHalfOp);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*D*OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_DeHalfOp);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Op);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "D*OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_DeOp);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "VOIC*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Voice);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "D*VOIC*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_DeVoice);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "TOPIC*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Topic);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "KICK*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Kick);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "REM*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_AnonKick);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "USER*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Users);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "INV*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Invite);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNBAN*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Unban);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LIVE*", Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), ChanServ::do_Live);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "DETAIL*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_Detail);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLEAR* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLEAR*", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LEV* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LEV*", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC*", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KICK *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KICK", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GREET* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GREET*", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "M*S*G* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "M*S*G*", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET*", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK *", Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK", Magick::instance().commserv.SOP_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK *", Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK", Magick::instance().commserv.SOP_Name(),
						 do_1_3param);
    ETCB();
}

void ChanServ::RemCommands()
{
    BTCB();
    NFT("ChanServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLEAR* USER*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLEAR* OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLEAR* H*OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLEAR* VOICE*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLEAR* MODE*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLEAR* BAN*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLEAR* ALL", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLEAR* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LEV* SET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LEV* RESET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LEV* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LEV* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LEV* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* REM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KICK ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KICK DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KICK REM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KICK LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KICK VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KICK H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GREET* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GREET* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GREET* REM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GREET* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GREET* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GREET* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "M*S*G* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "M*S*G* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "M*S*G* REM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "M*S*G* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "M*S*G* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "M*S*G* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* FOUND*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* COFOUND*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* DESC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PASS*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* E*MAIL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* URL", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* WWW*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* WEB*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* COMM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* M*LOCK", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* BAN*TIME", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PART*TIME", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* KEEP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* TOPIC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PRIV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* SEC*OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* SEC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* NOEX*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* ANAR*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* KICK*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* RES*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *JOIN*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* REV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK M*LOCK", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK BAN*TIME", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK PART*TIME", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK KEEP*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK TOPIC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK SEC*OP*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK ANAR*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK KICK*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK RES*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK *JOIN*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK REV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK H*LP", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK M*LOCK", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK BAN*TIME", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK PART*TIME",
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK KEEP*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK TOPIC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK SEC*OP*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK ANAR*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK KICK*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK RES*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK *JOIN*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK REV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK H*LP", Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "REG*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "DROP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ID*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "INFO*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LIST*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SUSP*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNSUSP*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FORB*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GET*PASS*", Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "MODE", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "D*H*OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*D*OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "D*OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "VOIC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "D*VOIC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "TOPIC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "KICK*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "REM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "USER*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "INV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNBAN*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LIVE*", Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "DETAIL*", Magick::instance().commserv.SOP_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLEAR* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLEAR*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LEV* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LEV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KICK *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KICK", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GREET* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GREET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "M*S*G* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "M*S*G*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK *", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK *", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK", Magick::instance().commserv.SOP_Name());
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void ChanServ::AddStored(Chan_Stored_t * in) throw (E_ChanServ_Stored)
#else
void ChanServ::AddStored(Chan_Stored_t * in)
#endif
{
    BTCB();
    FT("ChanServ::AddStored", ("(Chan_Stored_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Stored(E_ChanServ_Stored::W_Add, E_ChanServ_Stored::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Chan", "Stored", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Stored(E_ChanServ_Stored::W_Add, E_ChanServ_Stored::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Chan", "Stored", "Add", "Blank"));
	return;
#endif
    }

    if (in->doDelete())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Stored(E_ChanServ_Stored::W_Add, E_ChanServ_Stored::T_NotFound));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Chan", "Stored", "Add", "NotFound"));
	return;
#endif
    }

    WLOCK((lck_ChanServ, lck_stored));
    map_entry < Chan_Stored_t > old_entry(stored, in->Name().LowerCase());
    if (old_entry != NULL)
    {
	old_entry->setDelete();
	stored.erase(in->Name().LowerCase());
    }
    stored[in->Name().LowerCase()] = in;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
map_entry < Chan_Stored_t > ChanServ::GetStored(const mstring & in) const throw (E_ChanServ_Stored)
#else
map_entry < Chan_Stored_t > ChanServ::GetStored(const mstring & in) const
#endif
{
    BTCB();
    FT("ChanServ::GetStored", (in));

    RLOCK((lck_ChanServ, lck_stored));
    ChanServ::stored_t::const_iterator iter = stored.find(in.LowerCase());
    if (iter == stored.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Stored(E_ChanServ_Stored::W_Get, E_ChanServ_Stored::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Get", "NotFound", in));
	NRET(Chan_Stored_t &, GLOB_Chan_Stored_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Stored(E_ChanServ_Stored::W_Get, E_ChanServ_Stored::T_Invalid, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Get", "Invalid", in));
	NRET(Chan_Stored_t &, GLOB_Chan_Stored_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Stored(E_ChanServ_Stored::W_Get, E_ChanServ_Stored::T_Blank, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Get", "Blank", in));
	NRET(Chan_Stored_t &, GLOB_Chan_Stored_t);
#endif
    }

    NRET(map_entry < Chan_Stored_t >, map_entry < Chan_Stored_t > (iter->second));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void ChanServ::RemStored(const mstring & in) throw (E_ChanServ_Stored)
#else
void ChanServ::RemStored(const mstring & in)
#endif
{
    BTCB();
    FT("ChanServ::RemStored", (in));

    WLOCK((lck_ChanServ, lck_stored));
    ChanServ::stored_t::iterator iter = stored.find(in.LowerCase());
    if (iter == stored.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Stored(E_ChanServ_Stored::W_Rem, E_ChanServ_Stored::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Rem", "NotFound", in));
	return;
#endif
    }

    if (iter->second != NULL)
    {
	map_entry < Chan_Stored_t > entry(iter->second);
	entry->setDelete();
    }
    stored.erase(iter);
    ETCB();
}

bool ChanServ::IsStored(const mstring & in) const
{
    BTCB();
    FT("ChanServ::IsStored", (in));
    RLOCK((lck_ChanServ, lck_stored));
    bool retval = stored.find(in.LowerCase()) != stored.end();

    RET(retval);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void ChanServ::AddLive(Chan_Live_t * in) throw (E_ChanServ_Live)
#else
void ChanServ::AddLive(Chan_Live_t * in)
#endif
{
    BTCB();
    FT("ChanServ::AddLive", ("(Chan_Live_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Live(E_ChanServ_Live::W_Add, E_ChanServ_Live::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Chan", "Live", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Live(E_ChanServ_Live::W_Add, E_ChanServ_Live::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Chan", "Live", "Add", "Blank"));
	return;
#endif
    }

    WLOCK((lck_ChanServ, lck_live));
    map_entry < Chan_Live_t > old_entry(live, in->Name().LowerCase());
    if (old_entry != NULL)
    {
	old_entry->setDelete();
	live.erase(in->Name().LowerCase());
    }
    live[in->Name().LowerCase()] = in;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
map_entry < Chan_Live_t > ChanServ::GetLive(const mstring & in) const throw (E_ChanServ_Live)
#else
map_entry < Chan_Live_t > ChanServ::GetLive(const mstring & in) const
#endif
{
    BTCB();
    FT("ChanServ::GetLive", (in));

    RLOCK((lck_ChanServ, lck_live, in.LowerCase()));
    ChanServ::live_t::const_iterator iter = live.find(in.LowerCase());
    if (iter == live.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Live(E_ChanServ_Live::W_Get, E_ChanServ_Live::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Get", "NotFound", in));
	NRET(Chan_Live_t &, GLOB_Chan_Live_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Live(E_ChanServ_Live::W_Get, E_ChanServ_Live::T_Invalid, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Get", "Invalid", in));
	NRET(Chan_Live_t &, GLOB_Chan_Live_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Live(E_ChanServ_Live::W_Get, E_ChanServ_Live::T_Blank, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Get", "Blank", in));
	NRET(Chan_Live_t &, GLOB_Chan_Live_t);
#endif
    }

    NRET(map_entry < Chan_Live_t >, map_entry < Chan_Live_t > (iter->second));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void ChanServ::RemLive(const mstring & in) throw (E_ChanServ_Live)
#else
void ChanServ::RemLive(const mstring & in)
#endif
{
    BTCB();
    FT("ChanServ::RemLive", (in));

    WLOCK((lck_ChanServ, lck_live));
    ChanServ::live_t::iterator iter = live.find(in.LowerCase());
    if (iter == live.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Live(E_ChanServ_Live::W_Rem, E_ChanServ_Live::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Rem", "NotFound", in));
	return;
#endif
    }

    if (iter->second != NULL)
    {
	map_entry < Chan_Live_t > entry(iter->second);
	entry->setDelete();
    }
    live.erase(iter);
    ETCB();
}

bool ChanServ::IsLive(const mstring & in) const
{
    BTCB();
    FT("ChanServ::IsLive", (in));
    RLOCK((lck_ChanServ, lck_live));
    bool retval = live.find(in.LowerCase()) != live.end();

    RET(retval);
    ETCB();
}

void ChanServ::execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    mThread::ReAttach(tt_ChanServ);
    FT("ChanServ::execute", (source, msgtype, params));
    //okay this is the main chanserv command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message
    mstring mynick(Magick::instance().getLname(IrcParam(params, 1)));
    mstring message(params.After(" :"));
    mstring command(message.Before(" "));

    if (message[0U] == CTCP_DELIM_CHAR)
    {
	if (msgtype == "PRIVMSG")
	    DccEngine::decodeRequest(mynick, source, message);
	else
	    DccEngine::decodeReply(mynick, source, message);
    }
    else if (msgtype == "PRIVMSG" && !Magick::instance().commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);
    ETCB();
}

void ChanServ::do_Help(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Help", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    mstring HelpTopic = Magick::instance().chanserv.GetInternalName();

    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.ExtractFrom(2, " ");
    HelpTopic.Trim();
    while (HelpTopic.find("  ") >= 0)
	HelpTopic.replace("  ", " ");
    HelpTopic.replace(" ", "/");
    vector < mstring > help = Magick::instance().getHelp(source, HelpTopic.UpperCase());

    unsigned int i;

    for (i = 0; i < help.size(); i++)
	::send(mynick, source, help[i]);
    ETCB();
}

void ChanServ::do_Register(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Register", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(3, " ");
    mstring desc = params.ExtractFrom(4, " ");

    if (Magick::instance().chanserv.IsStored(channel))
    {
	if (Magick::instance().chanserv.GetStored(channel)->Forbidden())
	    SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	else
	    SEND(mynick, source, "CS_STATUS/ISSTORED", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
    if (!clive->IsOp(source))
    {
	SEND(mynick, source, "CS_STATUS/NOTOPPED", (channel));
	return;
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->HasMode("o") && nlive->LastChanReg().SecondsSince() < Magick::instance().chanserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET",
	     (message, ToHumanTime(Magick::instance().chanserv.Delay() - nlive->LastChanReg().SecondsSince(), source)));
	return;
    }

    if (password.length() < 5 || password.IsSameAs(channel.After(channel[0u]), true) || password.IsSameAs(channel, true) ||
	password.IsSameAs(source, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    mstring founder = nstored->Host();

    if (founder.empty())
	founder = nstored->Name();

    if (Magick::instance().chanserv.Max_Per_Nick() && nstored->MyChannels() >= Magick::instance().chanserv.Max_Per_Nick())
    {
	unsigned int i;
	for (i=0; i<Magick::instance().chanserv.Ovr_Per_Nick().WordCount(" "); i++)
	    if (Magick::instance().commserv.IsList(Magick::instance().chanserv.Ovr_Per_Nick().ExtractWord(i, " ")) &&
		Magick::instance().commserv.GetList(Magick::instance().chanserv.Ovr_Per_Nick().ExtractWord(i, " "))->IsOn(source))
		break;
	if (!Magick::instance().chanserv.Ovr_Per_Nick().size() ||
	    i == Magick::instance().chanserv.Ovr_Per_Nick().WordCount(" "))
	{
	    NSEND(mynick, source, "CS_STATUS/TOOMANY");
	    return;
	}
    }

    map_entry < Chan_Stored_t > cstored(new Chan_Stored_t(channel, founder, password, desc));
    Magick::instance().chanserv.AddStored(cstored);
    if (cstored->Join())
	Magick::instance().server.JOIN(Magick::instance().chanserv.FirstName(), channel);
    nlive->SetLastChanReg();
    cstored->Topic(clive->Topic(), clive->Topic_Setter(), clive->Topic_Set_Time());
    clive->SendMode(cstored->Mlock());
    nlive->ChanIdentify(channel, password);
    if (!Magick::instance().server.proto.FounderMode().empty())
    {
	for (unsigned int i = 0; i < Magick::instance().server.proto.FounderMode().length(); i++)
	    clive->SendMode("+" + mstring(Magick::instance().server.proto.FounderMode()[i]) + " " + source);
    }
    Magick::instance().chanserv.stats.i_Register++;
    SEND(mynick, source, "CS_COMMAND/REGISTERED", (channel, founder));
    LOG(LM_INFO, "CHANSERV/REGISTER", (nlive->Mask(Nick_Live_t::N_U_P_H), channel));
    ETCB();
}

void ChanServ::do_Drop(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Drop", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!
	((Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source)) ||
	 nlive->IsChanIdentified(channel)))
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_CHAN_IDENT", (message, mynick, channel));
	return;
    }

    mstring founder = cstored->Founder();
    cstored->Drop();
    Magick::instance().chanserv.RemStored(channel);
    nlive->UnChanIdentify(channel);
    Magick::instance().chanserv.stats.i_Drop++;
    SEND(mynick, source, "CS_COMMAND/DROPPED", (channel));
    LOG(LM_INFO, "CHANSERV/DROP", (nlive->Mask(Nick_Live_t::N_U_P_H), channel, founder));
    ETCB();
}

void ChanServ::do_Identify(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Identify", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (cstored->Suspended())
    {
	SEND(mynick, source, "CS_STATUS/ISSUSPENDED", (channel));
	return;
    }

    mstring output;

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    bool wasident = nlive->IsChanIdentified(channel);

    output = nlive->ChanIdentify(channel, pass);
    if (!wasident && nlive->IsChanIdentified(channel))
    {
	Magick::instance().chanserv.stats.i_Identify++;
	LOG(LM_INFO, "CHANSERV/IDENTIFY", (nlive->Mask(Nick_Live_t::N_U_P_H), channel));
	if (!Magick::instance().server.proto.FounderMode().empty() && Magick::instance().chanserv.IsLive(channel))
	{
	    map_entry<Chan_Live_t> clive = Magick::instance().chanserv.GetLive(channel);
	    for (unsigned int i = 0; i < Magick::instance().server.proto.FounderMode().length(); i++)
		clive->SendMode("+" + mstring(Magick::instance().server.proto.FounderMode()[i]) + " " + source);
	}
    }
    if (!output.empty())
	::send(mynick, source, output);
    ETCB();
}

void ChanServ::do_Info(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Info", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    mstring output;

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);

    bool isoper = (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		   Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsOn(source));
    bool issop = (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source));

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    SEND(mynick, source, "CS_INFO/IS", (chan->Name()));
    SEND(mynick, source, "CS_INFO/FOUNDER", (chan->Founder()));
    if (!chan->CoFounder().empty())
	SEND(mynick, source, "CS_INFO/COFOUNDER", (chan->CoFounder()));
    SEND(mynick, source, "CS_INFO/DESCRIPTION", (chan->Description()));

    output.erase();
    if (chan->NoExpire() && isoper)
	output << " (" << Magick::instance().getMessage(source, "CS_INFO/NOEXPIRE") << ")";
    SEND(mynick, source, "CS_INFO/REGISTERED", (chan->RegTime().Ago(), output));
    if (isoper || !chan->Private())
    {
	if (Magick::instance().chanserv.IsLive(channel))
	{
	    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
	    output.erase();
	    if (clive->Ops())
	    {
		output << clive->Ops() << " " << Magick::instance().getMessage(source, "CS_INFO/OPS");
	    }
	    if (clive->HalfOps())
	    {
		if (!output.empty())
		    output << ", ";
		output << clive->HalfOps() << " " << Magick::instance().getMessage(source, "CS_INFO/HALFOPS");
	    }
	    if (clive->Voices())
	    {
		if (!output.empty())
		    output << ", ";
		output << clive->Voices() << " " << Magick::instance().getMessage(source, "CS_INFO/VOICES");
	    }
	    if (clive->Users())
	    {
		if (!output.empty())
		    output << ", ";
		output << clive->Users() << " " << Magick::instance().getMessage(source, "CS_INFO/USERS");
	    }
	    if (clive->Squit())
	    {
		if (!output.empty())
		    output << ", ";
		output << clive->Squit() << " " << Magick::instance().getMessage(source, "CS_INFO/SPLIT_USERS");
	    }
	    SEND(mynick, source, "CS_INFO/INUSEBY", (output));
	}
	else
	{
	    SEND(mynick, source, "CS_INFO/LASTUSED", (chan->LastUsed().Ago()));
	}
    }
    if (chan->Suspended())
    {
	SEND(mynick, source, "CS_INFO/SUSPEND", (chan->Suspend_Time().Ago(), chan->Suspend_By()));
	if (!chan->Comment().empty())
	    SEND(mynick, source, "CS_INFO/SUSPENDFOR", (chan->Comment()));
    }
    else
    {
	if (!chan->Email().empty())
	    SEND(mynick, source, "CS_INFO/EMAIL", (chan->Email()));
	if (!chan->URL().empty())
	    SEND(mynick, source, "CS_INFO/URL", (chan->URL()));
	if (!chan->Comment().empty() && (isoper || issop))
	    SEND(mynick, source, "CS_INFO/COMMENT", (chan->Comment()));
    }
    if (!chan->Last_Topic().empty())
    {
	SEND(mynick, source, "CS_INFO/TOPIC", (chan->Last_Topic()));
	SEND(mynick, source, "CS_INFO/TOPICBY", (chan->Last_Topic_Set_Time().Ago(), chan->Last_Topic_Setter()));
    }
    if (!chan->Mlock().empty())
	SEND(mynick, source, "CS_INFO/MLOCK", (chan->Mlock()));
    if (!chan->Revenge().empty())
	SEND(mynick, source, "CS_INFO/REVENGE", (Magick::instance().getMessage(source, "CS_SET/REV_" + chan->Revenge())));
    if (chan->Bantime())
	SEND(mynick, source, "CS_INFO/BANTIME", (ToHumanTime(chan->Bantime(), source)));
    if (chan->Parttime())
	SEND(mynick, source, "CS_INFO/PARTTIME", (ToHumanTime(chan->Parttime(), source)));

    output.erase();
    if (chan->Keeptopic())
    {
	if (!output.empty())
	    output << ", ";
	if (chan->L_Keeptopic())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/KEEPTOPIC");
	if (chan->L_Keeptopic())
	    output << IRC_Off;
    }

    if (chan->Topiclock())
    {
	if (!output.empty())
	    output << ", ";
	if (chan->L_Topiclock())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/TOPICLOCK");
	if (chan->L_Topiclock())
	    output << IRC_Off;
    }

    if (chan->Private())
    {
	if (!output.empty())
	    output << ", ";
	if (chan->L_Private())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/PRIVATE");
	if (chan->L_Private())
	    output << IRC_Off;
    }

    if (chan->Secureops())
    {
	if (!output.empty())
	    output << ", ";
	if (chan->L_Secureops())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/SECUREOPS");
	if (chan->L_Secureops())
	    output << IRC_Off;
    }

    if (chan->Secure())
    {
	if (!output.empty())
	    output << ", ";
	if (chan->L_Secure())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/SECURE");
	if (chan->L_Secure())
	    output << IRC_Off;
    }

    if (chan->Restricted())
    {
	if (!output.empty())
	    output << ", ";
	if (chan->L_Restricted())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/RESTRICTED");
	if (chan->L_Restricted())
	    output << IRC_Off;
    }

    if (chan->Anarchy())
    {
	if (!output.empty())
	    output << ", ";
	if (chan->L_Anarchy())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/ANARCHY");
	if (chan->L_Anarchy())
	    output << IRC_Off;
    }

    if (chan->KickOnBan())
    {
	if (!output.empty())
	    output << ", ";
	if (chan->L_KickOnBan())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/KICKONBAN");
	if (chan->L_KickOnBan())
	    output << IRC_Off;
    }

    if (!output.empty())
	SEND(mynick, source, "CS_INFO/OPTIONS", (output));
    {
	RLOCK((lck_Events));
	if (Magick::instance().servmsg.ShowSync() && Magick::instance().events != NULL)
	    SEND(mynick, source, "MISC/SYNC", (Magick::instance().events->SyncTime(source)));
    }
    ETCB();
}

void ChanServ::do_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_List", (mynick, source, params));

    unsigned int listsize, i;
    int count;
    mstring mask;

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	mask = "*";
	listsize = Magick::instance().config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Magick::instance().config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Magick::instance().config.Maxlist())
	{
	    SEND(mynick, source, "LIST/MAXLIST", (Magick::instance().config.Maxlist()));
	    return;
	}
    }

    SEND(mynick, source, "LIST/CHAN_LIST", (mask));
    ChanServ::stored_t::iterator iter;

    bool isoper = (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		   Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsOn(source));
    bool issop = (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source));
    {
	RLOCK((lck_ChanServ, lck_stored));
	for (iter = Magick::instance().chanserv.StoredBegin(), i = 0, count = 0;
	     iter != Magick::instance().chanserv.StoredEnd(); iter++)
	{
	    map_entry < Chan_Stored_t > cstored(iter->second);
	    if (cstored->Name().Matches(mask, true))
	    {
		if (i < listsize && (!cstored->Private() || isoper))
		{
		    if (issop)
		    {
			if (message.Contains("NOEXP") && !cstored->NoExpire())
			    continue;
			if (message.Contains("FORBID") && !cstored->Forbidden())
			    continue;
			if (message.Contains("SUSP") && !cstored->Suspended())
			    continue;
		    }

		    if (cstored->Forbidden())
		    {
			::send(mynick, source,
			       cstored->Name() + "  " + Magick::instance().getMessage(source, "VALS/FORBIDDEN"));
		    }
		    else
		    {
			::send(mynick, source, cstored->Name() + "  " + cstored->Description());
		    }
		    i++;
		}
		count++;
	    }
	}
    }
    SEND(mynick, source, "LIST/DISPLAYED", (i, count));
    ETCB();
}

void ChanServ::do_Suspend(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Suspend", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring reason = params.ExtractFrom(3, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    cstored->Suspend(source, reason);
    Magick::instance().chanserv.stats.i_Suspend++;
    SEND(mynick, source, "CS_COMMAND/SUSPENDED", (channel));
    LOG(LM_NOTICE, "CHANSERV/SUSPEND",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), channel, reason));
    if (Magick::instance().chanserv.IsLive(channel))
    {
	map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);

	Magick::instance().server.TOPIC(mynick, mynick, channel,
					"[" + IRC_Bold + Magick::instance().getMessage("VALS/SUSPENDED") + IRC_Off + "] " +
					reason + " [" + IRC_Bold + Magick::instance().getMessage("VALS/SUSPENDED") + IRC_Off +
					"]", clive->Topic_Set_Time());

	clive->SendMode("-" + clive->Mode() + " " + clive->Key());
	if (!cstored->Mlock().empty())
	    clive->SendMode(cstored->Mlock() + " " + cstored->Mlock_Key() + " " + cstored->Mlock_Limit());

	for (unsigned int i = 0; i < clive->Users(); i++)
	{
	    if (clive->IsOp(clive->User(i)) &&
		!(cstored->GetAccess(clive->User(i), "AUTOOP") || cstored->GetAccess(clive->User(i), "CMDOP")))
	    {
		clive->SendMode("-o " + clive->User(i));
	    }
	    if (clive->IsHalfOp(clive->User(i)) &&
		!(cstored->GetAccess(clive->User(i), "AUTOHALFOP") || cstored->GetAccess(clive->User(i), "CMDHALFOP")))
	    {
		clive->SendMode("-h " + clive->User(i));
	    }
	    if (clive->IsVoice(clive->User(i)) &&
		!(cstored->GetAccess(clive->User(i), "AUTOVOICE") || cstored->GetAccess(clive->User(i), "CMDVOICE")))
	    {
		clive->SendMode("-v " + clive->User(i));
	    }
	}
    }
    ETCB();
}

void ChanServ::do_UnSuspend(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_UnSuspend", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.IsLive(channel))
	Magick::instance().server.TOPIC(mynick, mynick, channel, "",
					Magick::instance().chanserv.GetLive(channel)->Topic_Set_Time());

    cstored->UnSuspend();
    Magick::instance().chanserv.stats.i_Unsuspend++;
    SEND(mynick, source, "CS_COMMAND/UNSUSPENDED", (channel));
    LOG(LM_NOTICE, "CHANSERV/UNSUSPEND", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), channel));
    ETCB();
}

void ChanServ::do_Forbid(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Forbid", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > tmp(new Chan_Stored_t(channel));
    Magick::instance().chanserv.AddStored(tmp);
    Magick::instance().chanserv.stats.i_Forbid++;
    SEND(mynick, source, "CS_COMMAND/FORBIDDEN", (channel));
    LOG(LM_NOTICE, "CHANSERV/FORBID", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), channel));

    if (Magick::instance().chanserv.IsLive(channel))
    {
	unsigned int i;

	vector < mstring > kickees;
	map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
	for (i = 0; i < clive->Users(); i++)
	{
	    kickees.push_back(clive->User(i));
	}
	for (i = 0; i < kickees.size(); i++)
	{
	    mstring reason =
		parseMessage(Magick::instance().getMessage(kickees[i], "CS_STATUS/ISFORBIDDEN"), mVarArray(channel));

	    Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), kickees[i], channel, reason);
	}
    }
    ETCB();
}

#ifdef GETPASS
void ChanServ::do_Getpass(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Getpass", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();
    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(chan->Founder()))
    {
	LOG(LM_CRITICAL, "ERROR/FOUNDER_NOTREGD", (chan->Name(), chan->Founder()));
	Magick::instance().chanserv.RemStored(channel);
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    Magick::instance().chanserv.stats.i_Getpass++;
    mstring founder = Magick::instance().getSname(chan->Founder());

    SEND(mynick, source, "CS_COMMAND/GETPASS", (chan->Name(), founder, chan->Password()));
    ANNOUNCE(mynick, "MISC/CHAN_GETPASS", (source, chan->Name(), founder));
    LOG(LM_NOTICE, "CHANSERV/GETPASS",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), chan->Name(), founder));
    ETCB();
}

#endif /* GETPASS */

void ChanServ::do_Setpass(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Setpass", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(3, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (password.length() < 5 || password.IsSameAs(channel.After(channel[0u]), true) || password.IsSameAs(channel, true) ||
	password.IsSameAs(source, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
	return;
    }

    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();
    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(chan->Founder()))
    {
	LOG(LM_CRITICAL, "ERROR/FOUNDER_NOTREGD", (chan->Name(), chan->Founder()));
	Magick::instance().chanserv.RemStored(channel);
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    Magick::instance().chanserv.stats.i_Getpass++;
    chan->Password(password);
    mstring founder = Magick::instance().getSname(chan->Founder());

    SEND(mynick, source, "CS_COMMAND/SETPASS", (chan->Name(), founder, password));
    ANNOUNCE(mynick, "MISC/CHAN_SETPASS", (source, chan->Name(), founder));
    LOG(LM_NOTICE, "CHANSERV/SETPASS",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), chan->Name(), founder));
    ETCB();
}

void ChanServ::do_Mode(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Mode", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored;
    if (Magick::instance().chanserv.IsStored(channel))
	cstored = Magick::instance().chanserv.GetStored(channel);

    bool change = false;

    if (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))
    {
	if (cstored != NULL)
	    channel = cstored->Name();
	else
	    channel = Magick::instance().getLname(channel);

	// If we have 2 params, and we have SUPER access, or are a SOP
	if (params.WordCount(" ") > 2 && Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Mode()) &&
	    Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Mode())->IsOn(source))
	    change = true;
    }
    else
    {
	if (cstored == NULL)
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	    return;
	}
	channel = cstored->Name();

	if (cstored->Forbidden())
	{
	    SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	    return;
	}

	if (!cstored->GetAccess(source, "VIEW"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	// If we have 2 params, and we have SUPER access, or are a SOP
	if (params.WordCount(" ") > 2 && cstored->GetAccess(source, "CMDMODE"))
	    change = true;

    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (change)
    {
	mstring modes = params.ExtractFrom(3, " ");

	Magick::instance().chanserv.GetLive(channel)->SendMode(modes);
	Magick::instance().chanserv.stats.i_Mode++;
	LOG(LM_DEBUG, "CHANSERV/MODE",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), modes, channel));
    }
    else
    {
	mstring output;

	map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
	output << clive->Name() << ": +" << clive->Mode();
	if (!clive->Key().empty())
	    output << "k";
	if (clive->Limit())
	    output << "l";
	if (!clive->Key().empty())
	    output << " " << clive->Key();
	if (clive->Limit())
	    output << " " << clive->Limit();

	Magick::instance().chanserv.stats.i_Mode++;
	::send(mynick, source, output);
    }
    ETCB();
}

void ChanServ::do_Op(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Op", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = source;

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
    // If we have 2 params, and we have SUPER access, or are a SOP

    bool isoper = (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Op()) &&
		   Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Op())->IsOn(source));
    if (params.WordCount(" ") > 2 && (chan->GetAccess(source, "SUPER") || isoper))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	    return;
	}
	else if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (clive->IsOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_OPPED", (target, channel));
	    return;
	}
	else if (!isoper && chan->Secureops() && !(chan->GetAccess(target, "CMDOP") || chan->GetAccess(target, "AUTOOP")))
	{
	    SEND(mynick, source, "CS_STATUS/RESTRICT", (channel, Magick::instance().getMessage(source, "CS_SET/SECUREOPS")));
	    return;
	}
    }
    else
    {
	if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (clive->IsOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/OPPED", (channel));
	    return;
	}
	else if (!chan->GetAccess(target, "CMDOP"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }

    Magick::instance().chanserv.stats.i_Op++;
    clive->SendMode("+o " + target);
    LOG(LM_DEBUG, "CHANSERV/OP", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    ETCB();
}

void ChanServ::do_DeOp(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_DeOp", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = source;

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 &&
	(chan->GetAccess(source, "SUPER") ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Op()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Op())->IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	    return;
	}
	else if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (!clive->IsOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTOPPED", (target, channel));
	    return;
	}
    }
    else
    {
	if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (!clive->IsOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/NOTOPPED", (channel));
	    return;
	}
    }

    Magick::instance().chanserv.stats.i_Deop++;
    clive->SendMode("-o " + target);
    LOG(LM_DEBUG, "CHANSERV/DEOP", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    ETCB();
}

void ChanServ::do_HalfOp(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_HalfOp", (mynick, source, params));

    if (!Magick::instance().server.proto.ChanModeArg().Contains('h'))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = source;

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    bool isoper = (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_HalfOp()) &&
		   Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_HalfOp())->IsOn(source));
    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && (chan->GetAccess(source, "SUPER") || isoper))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	    return;
	}
	else if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (clive->IsHalfOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_HALFOPPED", (target, channel));
	    return;
	}
	else if (!isoper && chan->Secureops() &&
		 !(chan->GetAccess(target, "CMDHALFOP") || chan->GetAccess(target, "AUTOHALFOP")))
	{
	    SEND(mynick, source, "CS_STATUS/RESTRICT", (channel, Magick::instance().getMessage(source, "CS_SET/SECUREOPS")));
	    return;
	}
    }
    else
    {
	if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (clive->IsHalfOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/HALFOPPED", (channel));
	    return;
	}
	else if (!chan->GetAccess(target, "CMDHALFOP"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }

    Magick::instance().chanserv.stats.i_Halfop++;
    clive->SendMode("+h " + target);
    LOG(LM_DEBUG, "CHANSERV/HALFOP",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    ETCB();
}

void ChanServ::do_DeHalfOp(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_DeHalfOp", (mynick, source, params));

    if (!Magick::instance().server.proto.ChanModeArg().Contains('h'))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = source;

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 &&
	(chan->GetAccess(source, "SUPER") ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_HalfOp()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_HalfOp())->IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	    return;
	}
	else if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (!clive->IsHalfOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTHALFOPPED", (target, channel));
	    return;
	}
    }
    else
    {
	if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (!clive->IsHalfOp(target))
	{
	    SEND(mynick, source, "CS_STATUS/NOTHALFOPPED", (channel));
	    return;
	}
    }

    Magick::instance().chanserv.stats.i_Dehalfop++;
    clive->SendMode("-h " + target);
    LOG(LM_DEBUG, "CHANSERV/DEHALFOP",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    ETCB();
}

void ChanServ::do_Voice(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Voice", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = source;

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    bool isoper = (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Voice()) &&
		   Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Voice())->IsOn(source));
    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && (chan->GetAccess(source, "SUPER") || isoper))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	    return;
	}
	else if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (clive->IsVoice(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_VOICED", (target, channel));
	    return;
	}
	else if (!isoper && chan->Secureops() &&
		 !(chan->GetAccess(target, "CMDVOICE") || chan->GetAccess(target, "AUTOVOICE")))
	{
	    SEND(mynick, source, "CS_STATUS/RESTRICT", (channel, Magick::instance().getMessage(source, "CS_SET/SECUREOPS")));
	    return;
	}
    }
    else
    {
	if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (clive->IsVoice(target))
	{
	    SEND(mynick, source, "CS_STATUS/VOICED", (channel));
	    return;
	}
	else if (!chan->GetAccess(target, "CMDVOICE"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }

    Magick::instance().chanserv.stats.i_Voice++;
    clive->SendMode("+v " + target);
    LOG(LM_DEBUG, "CHANSERV/VOICE",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    ETCB();
}

void ChanServ::do_DeVoice(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_DeVoice", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = source;

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 &&
	(chan->GetAccess(source, "SUPER") ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Voice()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Voice())->IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	    return;
	}
	else if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (!clive->IsVoice(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTVOICED", (target, channel));
	    return;
	}
    }
    else
    {
	if (!clive->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	    return;
	}
	else if (!clive->IsVoice(target))
	{
	    SEND(mynick, source, "CS_STATUS/NOTVOICED", (channel));
	    return;
	}
    }

    Magick::instance().chanserv.stats.i_Devoice++;
    clive->SendMode("-v " + target);
    LOG(LM_DEBUG, "CHANSERV/DEVOICE",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    ETCB();
}

void ChanServ::do_Topic(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Topic", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring topic = params.ExtractFrom(3, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!chan->GetAccess(source, "CMDMODE"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (topic.IsSameAs("none", true))
    {
	topic.erase();
    }

    Magick::instance().chanserv.stats.i_Topic++;
    chan->SetTopic(mynick, source, topic);
    LOG(LM_DEBUG, "CHANSERV/TOPIC", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), channel));
    ETCB();
}

void ChanServ::do_Kick(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Kick", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = params.ExtractWord(3, " ");
    mstring reason;

    if (params.WordCount(" ") >= 4)
	reason = params.ExtractFrom(4, " ");
    else
	reason = Magick::instance().chanserv.DEF_Akick_Reason();

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored;
    if (Magick::instance().chanserv.IsStored(channel))
    {
	cstored = Magick::instance().chanserv.GetStored(channel);
	channel = cstored->Name();
    }
    else
    {
	channel = Magick::instance().getLname(channel);
    }

    if (!Magick::instance().nickserv.IsLive(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	return;
    }

    target = Magick::instance().getLname(target);
    if (!Magick::instance().chanserv.GetLive(channel)->IsIn(target))
    {
	SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	return;
    }

    if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Kick()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Kick())->IsOn(source)))
    {
	if (cstored == NULL)
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	    return;
	}

	if (cstored->Forbidden())
	{
	    SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	    return;
	}

	if (!cstored->GetAccess(source, "CMDKICK"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (cstored->GetAccess(target) > cstored->GetAccess(source))
	{
	    SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (target, channel));
	    return;
	}
    }

    mstring output = parseMessage(Magick::instance().getMessage(target, "CS_COMMAND/KICK"), mVarArray(source, reason));

    Magick::instance().chanserv.stats.i_Kick++;
    Magick::instance().server.KICK(mynick, target, channel, output);
    LOG(LM_DEBUG, "CHANSERV/KICK", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    ETCB();
}

void ChanServ::do_AnonKick(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_AnonKick", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = params.ExtractWord(3, " ");
    mstring reason = params.ExtractFrom(4, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!(chan->GetAccess(source, "CMDKICK") && chan->GetAccess(source, "SUPER")))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (!Magick::instance().nickserv.IsLive(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	return;
    }

    if (!Magick::instance().chanserv.GetLive(channel)->IsIn(target))
    {
	SEND(mynick, source, "CS_STATUS/OTH_NOTIN", (target, channel));
	return;
    }

    if (chan->GetAccess(target) > chan->GetAccess(source))
    {
	SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (target, channel));
	return;
    }

    Magick::instance().chanserv.stats.i_Anonkick++;
    Magick::instance().server.KICK(mynick, target, channel, reason);
    LOG(LM_DEBUG, "CHANSERV/ANONKICK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    ETCB();
}

void ChanServ::do_Users(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Users", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    map_entry < Chan_Live_t > chan = Magick::instance().chanserv.GetLive(channel);
    channel = chan->Name();
    if (!
	(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	 Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source)))
    {
	if (!Magick::instance().chanserv.IsStored(channel))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	    return;
	}

	map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
	if (cstored->Forbidden())
	{
	    SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	    return;
	}

	if (!cstored->GetAccess(source, "VIEW"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }

    unsigned int i;
    mstring user, output = channel + ": ";

    for (i = 0; i < chan->Users(); i++)
    {
	user = Magick::instance().getLname(chan->User(i));
	if (output.size() + user.length() > Magick::instance().server.proto.MaxLine())
	{
	    ::send(mynick, source, output);
	    output = channel + ": ";
	}
	if (chan->IsOp(user))
	{
	    output << "@";
	}
	else if (chan->IsHalfOp(user))
	{
	    output << "%";
	}
	else if (chan->IsVoice(user))
	{
	    output << "+";
	}
	output << user << " ";
    }
    if (output.length() > channel.length() + 2)
	::send(mynick, source, output);

    output = channel + " (SPLIT): ";
    for (i = 0; i < chan->Squit(); i++)
    {
	user = Magick::instance().getLname(chan->Squit(i));
	if (output.size() + user.length() > Magick::instance().server.proto.MaxLine())
	{
	    ::send(mynick, source, output);
	    output = channel + " (SQUIT): ";
	}
	output << user << " ";
    }
    if (output.length() > channel.length() + 10)
	::send(mynick, source, output);
    ETCB();
}

void ChanServ::do_Invite(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Invite", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = source;

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored;
    if (Magick::instance().chanserv.IsStored(channel))
    {
	cstored = Magick::instance().chanserv.GetStored(channel);
	channel = cstored->Name();
    }
    else
    {
	channel = Magick::instance().getLname(channel);
    }

    if (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Invite()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Invite())->IsOn(source) &&
	params.WordCount(" ") > 2)
    {
	target = params.ExtractWord(3, " ");
    }
    else
    {
	if (cstored == NULL)
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	    return;
	}

	if (cstored->Forbidden())
	{
	    SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	    return;
	}

	if (params.WordCount(" ") > 2 && cstored->GetAccess(source, "SUPER"))
	    target = params.ExtractWord(3, " ");
	else if (!cstored->GetAccess(target, "CMDINVITE"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }

    if (!Magick::instance().nickserv.IsLive(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	return;
    }

    target = Magick::instance().getLname(target);
    if (Magick::instance().chanserv.GetLive(channel)->IsIn(target))
    {
	if (source.IsSameAs(target, true))
	    SEND(mynick, source, "CS_STATUS/IN", (channel));
	else
	    SEND(mynick, source, "CS_STATUS/OTH_IN", (target, channel));
	return;
    }

    Magick::instance().chanserv.stats.i_Invite++;
    Magick::instance().server.INVITE(mynick, target, channel);
    if (source.IsSameAs(target, true))
	SEND(mynick, target, "CS_COMMAND/INVITE", (channel));
    else
	SEND(mynick, target, "CS_COMMAND/OTH_INVITE", (channel, source));
    LOG(LM_DEBUG, "CHANSERV/INVITE",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    ETCB();
}

void ChanServ::do_Unban(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Unban", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = source;

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored;
    if (Magick::instance().chanserv.IsStored(channel))
    {
	cstored = Magick::instance().chanserv.GetStored(channel);
	channel = cstored->Name();
    }
    else
    {
	channel = Magick::instance().getLname(channel);
    }

    if (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Unban()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Unban())->IsOn(source) &&
	params.WordCount(" ") > 2)
    {
	target = params.ExtractWord(3, " ");
    }
    else
    {
	if (cstored == NULL)
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	    return;
	}

	if (cstored->Forbidden())
	{
	    SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	    return;
	}

	if (params.WordCount(" ") > 2 && cstored->GetAccess(source, "SUPER"))
	    target = params.ExtractWord(3, " ");
	else if (!cstored->GetAccess(target, "CMDUNBAN"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }

    if (!Magick::instance().nickserv.IsLive(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	return;
    }

    vector < mstring > bans;
    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(target);
    target = nlive->Name();
    unsigned int i;

    for (i = 0; i < clive->Bans(); i++)
    {
	if (nlive->Mask(Nick_Live_t::N_U_P_H).Matches(clive->Ban(i), true) ||
	    nlive->AltMask(Nick_Live_t::N_U_P_H).Matches(clive->Ban(i), true))
	{
	    bans.push_back(clive->Ban(i));
	}
    }
    for (i = 0; i < bans.size(); i++)
	clive->SendMode("-b " + bans[i]);
    if (bans.size())
    {
	Magick::instance().chanserv.stats.i_Unban++;
	if (source.IsSameAs(target, true))
	    SEND(mynick, source, "CS_COMMAND/UNBANNED", (channel));
	else
	    SEND(mynick, source, "CS_COMMAND/OTH_UNBANNED", (target, channel));
	LOG(LM_DEBUG, "CHANSERV/UNBAN",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    }
    else
    {
	if (source.IsSameAs(target, true))
	    SEND(mynick, source, "CS_STATUS/NOTBANNED", (channel));
	else
	    SEND(mynick, source, "CS_STATUS/OTH_NOTBANNED", (target, channel));
    }
    ETCB();
}

void ChanServ::do_Live(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Live", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	mask = "*";
	listsize = Magick::instance().config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Magick::instance().config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Magick::instance().config.Maxlist())
	{
	    mstring output;

	    SEND(mynick, source, "LIST/MAXLIST", (Magick::instance().config.Maxlist()));
	    return;
	}
    }

    SEND(mynick, source, "LIST/CHAN_LIST", (mask));
    ChanServ::live_t::iterator iter;

    {
	RLOCK((lck_ChanServ, lck_live));
	for (iter = Magick::instance().chanserv.LiveBegin(), i = 0, count = 0; iter != Magick::instance().chanserv.LiveEnd();
	     iter++)
	{
	    map_entry < Chan_Live_t > clive(iter->second);
	    if (clive->Name().Matches(mask, true))
	    {
		if (i < listsize)
		{
		    mstring mode = clive->Mode();

		    if (clive->Key().length())
			mode << "k";
		    if (clive->Limit())
			mode << "l";
		    if (clive->Key().length())
			mode << " " << clive->Key();
		    if (clive->Limit())
			mode << " " << clive->Limit();
		    ::sendV(mynick, source, "%s (%du %do %dh %dv %ds %db): +%s", clive->Name().c_str(), clive->Users(),
			    clive->Ops(), clive->HalfOps(), clive->Voices(), clive->Squit(), clive->Bans(), mode.c_str());
		    i++;
		}
		count++;
	    }
	}
    }
    SEND(mynick, source, "LIST/DISPLAYED", (i, count));
    ETCB();
}

void ChanServ::do_Detail(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_Detail", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nick = params.ExtractWord(2, " ");

    if (!Magick::instance().nickserv.IsStored(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nick));
	return;
    }
    nick = Magick::instance().getSname(nick);

    bool displayed = false;
    mstring output;

    ChanServ::stored_t::iterator iter;
    {
	RLOCK((lck_ChanServ, lck_stored));
	for (iter = Magick::instance().chanserv.StoredBegin(); iter != Magick::instance().chanserv.StoredEnd(); iter++)
	{
	    map_entry < Chan_Stored_t > cstored(iter->second);
	    if (output.length() + cstored->Name().length() + 10 > Magick::instance().server.proto.MaxLine())
	    {
		displayed = true;
		::send(mynick, source, nick + ": " + output);
		output.erase();
	    }

	    if (cstored->Founder().IsSameAs(nick, true))
	    {
		if (output.length())
		    output += ", ";
		output += cstored->Name() + " (F)";
	    }
	    else
	    {
		MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Access"));
		if (cstored->Access_find(nick))
		{
		    if (output.length())
			output += ", ";
		    output += cstored->Name() + " (" + cstored->Access->Value();
		    if (cstored->CoFounder().IsSameAs(nick, true))
			output += "C";
		    output += ")";
		}
		else if (cstored->CoFounder().IsSameAs(nick, true))
		{
		    if (output.length())
			output += ", ";
		    output += cstored->Name() + " (0C)";
		}
	    }
	}
    }
    if (output.length())
	::send(mynick, source, nick + ": " + output);
    else if (!displayed)
	SEND(mynick, source, "CS_STATUS/HASNOACCESS", (nick));
    ETCB();
}

void ChanServ::do_clear_Users(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_clear_Users", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!chan->GetAccess(source, "CMDCLEAR") &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Clear()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Clear())->IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    unsigned int i;

    vector < mstring > kickees;
    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
    for (i = 0; i < clive->Users(); i++)
    {
	mstring user = clive->User(i);

	if (!user.IsSameAs(source) && Magick::instance().nickserv.IsLive(user) &&
	    !Magick::instance().nickserv.GetLive(user)->IsServices())
	    kickees.push_back(user);
    }
    for (i = 0; i < kickees.size(); i++)
    {
	mstring output =
	    parseMessage(Magick::instance().getMessage(kickees[i], "CS_COMMAND/CLEAR"), mVarArray(message, source, channel));

	Magick::instance().server.KICK(mynick, kickees[i], channel, output);
    }
    Magick::instance().chanserv.stats.i_Clear++;
    LOG(LM_INFO, "CHANSERV/COMMAND",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), message, channel));
    ETCB();
}

void ChanServ::do_clear_Ops(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_clear_Ops", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!chan->GetAccess(source, "CMDCLEAR") &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Clear()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Clear())->IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    bool allmode = false;

    if (message.ExtractWord(2, " ").Matches("ALL", true))
	allmode = true;

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
    vector < mstring > deop;
    unsigned int i;

    for (i = 0; i < clive->Ops(); i++)
    {
	if (!Magick::instance().nickserv.IsLive(clive->Op(i)) ||
	    Magick::instance().nickserv.GetLive(clive->Op(i))->IsServices())
	    continue;
	deop.push_back(clive->Op(i));
	if (!allmode)
	    SEND(mynick, clive->Op(i), "CS_COMMAND/CLEAR", (message, source, channel));
    }

    for (i = 0; i < deop.size(); i++)
    {
	clive->SendMode("-o " + deop[i]);
    }
    if (!allmode)
    {
	Magick::instance().chanserv.stats.i_Clear++;
	LOG(LM_INFO, "CHANSERV/COMMAND",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), message, channel));
    }
    ETCB();
}

void ChanServ::do_clear_HalfOps(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_clear_HalfOps", (mynick, source, params));

    if (!Magick::instance().server.proto.ChanModeArg().Contains('h'))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (Magick::instance().chanserv.GetStored(channel)->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!chan->GetAccess(source, "CMDCLEAR") &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Clear()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Clear())->IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    bool allmode = false;

    if (message.ExtractWord(2, " ").Matches("ALL", true))
	allmode = true;

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
    vector < mstring > dehalfop, ops;
    unsigned int i;

    for (i = 0; i < clive->HalfOps(); i++)
    {
	if (!Magick::instance().nickserv.IsLive(clive->HalfOp(i)) ||
	    Magick::instance().nickserv.GetLive(clive->HalfOp(i))->IsServices())
	    continue;
	dehalfop.push_back(clive->HalfOp(i));
	if (!allmode)
	    SEND(mynick, clive->HalfOp(i), "CS_COMMAND/CLEAR", (message, source, channel));
    }
    for (i = 0; i < clive->Ops(); i++)
    {
	if (!allmode)
	    SEND(mynick, clive->Op(i), "CS_COMMAND/CLEAR", (message, source, channel));
    }

    for (i = 0; i < dehalfop.size(); i++)
    {
	clive->SendMode("-h " + dehalfop[i]);
    }

    if (!allmode)
    {
	Magick::instance().chanserv.stats.i_Clear++;
	LOG(LM_INFO, "CHANSERV/COMMAND",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), message, channel));
    }
    ETCB();
}

void ChanServ::do_clear_Voices(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_clear_Voices", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!chan->GetAccess(source, "CMDCLEAR") &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Clear()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Clear())->IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    bool allmode = false;

    if (message.ExtractWord(2, " ").Matches("ALL", true))
	allmode = true;

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
    vector < mstring > devoice, ops;
    unsigned int i;

    for (i = 0; i < clive->Voices(); i++)
    {
	if (!Magick::instance().nickserv.IsLive(clive->Voice(i)) ||
	    Magick::instance().nickserv.GetLive(clive->Voice(i))->IsServices())
	    continue;
	devoice.push_back(clive->Voice(i));
	if (!allmode)
	    SEND(mynick, clive->Voice(i), "CS_COMMAND/CLEAR", (message, source, channel));
    }
    for (i = 0; i < clive->Ops(); i++)
    {
	if (!allmode)
	    SEND(mynick, clive->Op(i), "CS_COMMAND/CLEAR", (message, source, channel));
    }

    for (i = 0; i < devoice.size(); i++)
    {
	clive->SendMode("-v " + devoice[i]);
    }

    if (!allmode)
    {
	Magick::instance().chanserv.stats.i_Clear++;
	LOG(LM_INFO, "CHANSERV/COMMAND",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), message, channel));
    }
    ETCB();
}

void ChanServ::do_clear_Modes(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_clear_Modes", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!chan->GetAccess(source, "CMDCLEAR") &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Clear()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Clear())->IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    bool allmode = false;

    if (message.ExtractWord(2, " ").Matches("ALL", true))
	allmode = true;

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
    unsigned int i;
    mstring mode;

    mode << "-" << clive->Mode();
    if (clive->Limit())
	mode << "l";
    if (!clive->Key().empty())
	mode << "k " << clive->Key();
    clive->SendMode(mode);
    if (!allmode)
    {
	for (i = 0; i < clive->Ops(); i++)
	{
	    if (!Magick::instance().nickserv.IsLive(clive->Op(i)) ||
		Magick::instance().nickserv.GetLive(clive->Op(i))->IsServices())
		continue;
	    SEND(mynick, clive->Op(i), "CS_COMMAND/CLEAR", (message, source, channel));
	}
    }
    if (!chan->Mlock_On().empty())
    {
	mode = "+" + chan->Mlock_On();
	if (chan->Mlock_Limit())
	    mode << "l";
	if (!chan->Mlock_Key().empty())
	    mode << "k";
	if (chan->Mlock_Limit())
	    mode << " " << chan->Mlock_Limit();
	if (!chan->Mlock_Key().empty())
	    mode << " " << chan->Mlock_Key();

	clive->SendMode(mode);
    }
    if (!allmode)
    {
	Magick::instance().chanserv.stats.i_Clear++;
	LOG(LM_INFO, "CHANSERV/COMMAND",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), message, channel));
    }
    ETCB();
}

void ChanServ::do_clear_Bans(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_clear_Bans", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!chan->GetAccess(source, "CMDCLEAR") &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Clear()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Clear())->IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    bool allmode = false;

    if (message.ExtractWord(2, " ").Matches("ALL", true))
	allmode = true;

    vector < mstring > bans;
    unsigned int i;

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);

    // Need to get a list first, else it may not work.
    for (i = 0; i < clive->Bans(); i++)
	bans.push_back(clive->Ban(i));
    for (i = 0; i < bans.size(); i++)
	clive->SendMode("-b " + bans[i]);
    if (!allmode)
    {
	for (i = 0; i < clive->Ops(); i++)
	{
	    if (!Magick::instance().nickserv.IsLive(clive->Op(i)) ||
		Magick::instance().nickserv.GetLive(clive->Op(i))->IsServices())
		continue;
	    SEND(mynick, clive->Op(i), "CS_COMMAND/CLEAR", (message, source, channel));
	}
    }

    if (!allmode)
    {
	Magick::instance().chanserv.stats.i_Clear++;
	LOG(LM_INFO, "CHANSERV/COMMAND",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), message, channel));
    }
    ETCB();
}

void ChanServ::do_clear_All(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_clear_All", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsLive(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (channel));
	return;
    }

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = chan->Name();

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!chan->GetAccess(source, "CMDCLEAR") &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Clear()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Clear())->IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    ChanServ::do_clear_Modes(mynick, source, params);
    ChanServ::do_clear_Ops(mynick, source, params);
    ChanServ::do_clear_HalfOps(mynick, source, params);
    ChanServ::do_clear_Voices(mynick, source, params);
    ChanServ::do_clear_Bans(mynick, source, params);

    Magick::instance().chanserv.stats.i_Clear++;
    ::notice(mynick, channel,
	     parseMessage(Magick::instance().getMessage("CS_COMMAND/CLEAR"), mVarArray(message, source, channel)));
    LOG(LM_INFO, "CHANSERV/COMMAND",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), message, channel));
    ETCB();
}

void ChanServ::do_level_Set(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_level_Set", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 5)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring what = params.ExtractWord(4, " ");
    mstring level = params.ExtractWord(5, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    long num = 0;

    if (level.Matches("F*ND*R", true))
    {
	num = Magick::instance().chanserv.Level_Max() + 1;
    }
    else if (level.Matches("DIS*", true))
    {
	num = Magick::instance().chanserv.Level_Max() + 2;
    }
    else if (!level.IsNumber() || level.Contains("."))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }
    else
    {
	num = atol(level.c_str());
    }

    if (num < Magick::instance().chanserv.Level_Min() || num > Magick::instance().chanserv.Level_Max() + 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER",
	     (Magick::instance().chanserv.Level_Min(), Magick::instance().chanserv.Level_Max() + 2));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored->GetAccess(source) <= Magick::instance().chanserv.Level_Max())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    MLOCK((lck_ChanServ, lck_stored, channel.LowerCase(), "Level"));
    if (cstored->Level_find(what))
    {
	const_cast < entlist_val_t < long > * > (&(*cstored->Level))->Value(num, source);

	Magick::instance().chanserv.stats.i_Level++;
	SEND(mynick, source, "LIST/CHANGE2_LEVEL",
	     (cstored->Level->Entry(), channel, Magick::instance().getMessage(source, "LIST/LEVEL"), cstored->Level->Value()));
	LOG(LM_DEBUG, "CHANSERV/LEVEL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), cstored->Level->Entry(),
	     cstored->Level->Value(), channel));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS2", (what, channel, Magick::instance().getMessage(source, "LIST/LEVEL")));
    }
    ETCB();
}

void ChanServ::do_level_Reset(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_level_Reset", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring what = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored->GetAccess(source) <= Magick::instance().chanserv.Level_Max())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    MLOCK((lck_ChanServ, lck_stored, channel.LowerCase(), "Level"));
    if (!what.Matches("ALL", true))
    {
	if (cstored->Level_find(what) && Magick::instance().chanserv.LVL(what) > Magick::instance().chanserv.Level_Min())
	{
	    const_cast < entlist_val_t < long > * >
		(&(*cstored->Level))->Value(Magick::instance().chanserv.LVL(cstored->Level->Entry()), source);
	    Magick::instance().chanserv.stats.i_Level++;
	    SEND(mynick, source, "LIST/CHANGE2_LEVEL",
		 (cstored->Level->Entry(), channel, Magick::instance().getMessage(source, "LIST/LEVEL"),
		  cstored->Level->Value()));
	    LOG(LM_DEBUG, "CHANSERV/LEVEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), cstored->Level->Entry(),
		 cstored->Level->Value(), channel));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2", (what, channel, Magick::instance().getMessage(source, "LIST/LEVEL")));
	}
    }
    else
    {
	for (cstored->Level = cstored->Level_begin(); cstored->Level != cstored->Level_end(); cstored->Level++)
	{
	    const_cast < entlist_val_t < long > * >
		(&(*cstored->Level))->Value(Magick::instance().chanserv.LVL(cstored->Level->Entry()), source);
	}
	Magick::instance().chanserv.stats.i_Level++;
	SEND(mynick, source, "LIST/CHANGE2_ALL", (channel, Magick::instance().getMessage(source, "LIST/LEVEL")));
	LOG(LM_DEBUG, "CHANSERV/LEVEL_ALL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), channel));
    }
    ETCB();
}

void ChanServ::do_level_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_level_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring what;

    if (params.WordCount(" ") > 3)
	what = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    long myaccess = cstored->GetAccess(source);
    bool haveset = cstored->GetAccess(source, "SET");

    if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsIn(source))
	haveset = true;

    if (haveset)
    {
	SEND(mynick, source, "CS_COMMAND/LEVEL_HEAD", (channel));
    }

    MLOCK((lck_ChanServ, lck_stored, channel.LowerCase(), "Level"));
    for (cstored->Level = cstored->Level_begin(); cstored->Level != cstored->Level_end(); cstored->Level++)
    {
	if (haveset)
	{
	    mstring value;

	    if (cstored->Level->Value() == Magick::instance().chanserv.Level_Max() + 2)
		value = Magick::instance().getMessage(source, "VALS/LVL_DISABLED");
	    else if (cstored->Level->Value() == Magick::instance().chanserv.Level_Max() + 1)
		value = Magick::instance().getMessage(source, "VALS/LVL_FOUNDER");
	    else
		value = cstored->Level->Value();
	    ::sendV(mynick, source, "%10s  %-15s  %s", value.c_str(), cstored->Level->Entry().c_str(),
		    Magick::instance().getMessage(source, "CS_SET/LVL_" + cstored->Level->Entry()).c_str());
	}
	else if (!cstored->Level->Entry().IsSameAs("AUTODEOP", true) && cstored->Level->Value() <= myaccess)
	{
	    SEND(mynick, source, "CS_COMMAND/LEVEL_HAVE",
		 (Magick::instance().getMessage(source, "CS_SET/LVL_" + cstored->Level->Entry()), channel));
	}
    }
    ETCB();
}

void ChanServ::do_access_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_access_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 5)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring who = params.ExtractWord(4, " ");
    mstring level = params.ExtractWord(5, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    if (who[0u] == '@')
    {
	who.MakeUpper();
	if (!Magick::instance().commserv.IsList(who.After("@")))
	{
	    SEND(mynick, source, "COMMSERV/ISNOTSTORED", (who.After("@")));
	    return;
	}
    }
    else if (who.Contains("!") || who.Contains("@"))
    {
	if (!who.Contains("@"))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "LIST/ACCESS"), '@'));
	    return;
	}
	else if (!who.Contains("!"))
	{
	    who.prepend("*!");
	}

	unsigned int i, num;
	bool super = cstored->GetAccess(source, "SUPER");

	// i+1 below because unsigned i will always be >= 0
	for (i = who.size() - 1, num = 0; i + 1 > 0; i--)
	{
	    switch (who[i])
	    {
	    case '@':
		if (!super)
		    i = 0;
		break;
	    case '!':		// ALL these constitute wildcards.
	    case '*':
	    case '?':
	    case '.':
		break;
	    default:
		num++;
	    }
	}
	// IF we have less than 1 char for 
	if (!super && num <= Magick::instance().config.Starthresh())
	{
	    SEND(mynick, source, "ERR_SYNTAX/STARTHRESH",
		 (Magick::instance().getMessage(source, "LIST/ACCESS"), Magick::instance().config.Starthresh()));
	    return;
	}
	else if (num <= 1)
	{
	    SEND(mynick, source, "ERR_SYNTAX/STARTHRESH", (Magick::instance().getMessage(source, "LIST/ACCESS"), 1));
	    return;
	}
    }
    else if (!Magick::instance().nickserv.IsStored(who))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (who));
	return;
    }
    else
    {
	who = Magick::instance().getSname(who);
	if (cstored->Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (who));
	    return;
	}
    }

    if (!level.IsNumber() || level.Contains("."))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }
    long num = atol(level.c_str());

    if (num == 0)
    {
	NSEND(mynick, source, "CS_STATUS/NOTZERO");
	return;
    }

    if (num < Magick::instance().chanserv.Level_Min() || num > Magick::instance().chanserv.Level_Max())
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER",
	     (Magick::instance().chanserv.Level_Min(), Magick::instance().chanserv.Level_Max()));
	return;
    }

    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "ACCESS"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (num >= cstored->GetAccess(source))
    {
	NSEND(mynick, source, "CS_STATUS/ONLYBELOW");
	return;
    }

    MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Access"));
    if (cstored->Access_find(who, Chan_Stored_t::C_IsIn))
    {
	if (cstored->Access->Value() >= cstored->GetAccess(source))
	{
	    SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (cstored->Access->Entry(), channel));
	    return;
	}
    }

    bool rv = cstored->Access_find(who);

    // Result is false if its a mask, but not the same one
    if (rv && who.Contains("@") && who[0u] != '@' && !cstored->Access->Entry().IsSameAs(who, true))
	rv = false;
    if (rv)
    {
	mstring entry = cstored->Access->Entry();

	if (entry[0u] == '@')
	    entry.MakeUpper();
	else if (Magick::instance().nickserv.IsStored(entry))
	    entry = Magick::instance().getSname(entry);
	cstored->Access_erase();
	cstored->Access_insert(entry, num, source);
	Magick::instance().chanserv.stats.i_Access++;
	SEND(mynick, source, "LIST/CHANGE2_LEVEL",
	     (entry, channel, Magick::instance().getMessage(source, "LIST/ACCESS"), num));
	LOG(LM_DEBUG, "CHANSERV/ACCESS_CHANGE",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), entry, channel, num));
    }
    else
    {
	cstored->Access_insert(who, num, source);
	Magick::instance().chanserv.stats.i_Access++;
	SEND(mynick, source, "LIST/ADD2_LEVEL", (who, channel, Magick::instance().getMessage(source, "LIST/ACCESS"), num));
	LOG(LM_DEBUG, "CHANSERV/ACCESS_ADD",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), who, channel, num));
    }
    ETCB();
}

void ChanServ::do_access_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_access_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring who = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "ACCESS"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (!cstored->Access_size())
    {
	SEND(mynick, source, "LIST/EMPTY2", (channel, Magick::instance().getMessage(source, "LIST/ACCESS")));
	return;
    }

    if (who.IsNumber())
    {
	if (who.Contains("."))
	{
	    NSEND(mynick, source, "ERR_STYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(who);
	if (num < 1 || num > cstored->Access_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, cstored->Access_size()));
	    return;
	}

	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Access"));
	cstored->Access = find_if(cstored->Access_begin(), cstored->Access_end(), FindNumberedEntry(num - 1));
	if (cstored->Access != cstored->Access_end())
	{
	    if (cstored->Access->Value() >= cstored->GetAccess(source))
	    {
		SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (cstored->Access->Entry(), channel));
		return;
	    }

	    Magick::instance().chanserv.stats.i_Access++;
	    mstring entry = cstored->Access->Entry();

	    if (entry[0u] == '@')
		entry.MakeUpper();
	    else if (Magick::instance().nickserv.IsStored(entry))
		entry = Magick::instance().getSname(entry);
	    SEND(mynick, source, "LIST/DEL2", (entry, channel, Magick::instance().getMessage(source, "LIST/ACCESS")));
	    LOG(LM_DEBUG, "CHANSERV/ACCESS_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), entry, channel));
	    cstored->Access_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2_NUMBER",
		 (num, channel, Magick::instance().getMessage(source, "LIST/ACCESS")));
	}
    }
    else
    {
	if (who.Contains("@") || who.Contains("!"))
	{
	    if (!who.Contains("@"))
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "LIST/AKICK"), '@'));
		return;
	    }
	    else if (who[0u] != '@' && !who.Contains("!"))
	    {
		who.prepend("*!");
	    }
	}

	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Access"));
	if (cstored->Access_find(who, Chan_Stored_t::C_IsIn))
	{
	    if (cstored->Access->Value() >= cstored->GetAccess(source))
	    {
		SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (cstored->Access->Entry(), channel));
		return;
	    }
	}

	bool rv = cstored->Access_find(who);

	// Result is false if its a mask, but not the same one
	if (rv && who.Contains("@") && who[0u] != '@' && !cstored->Access->Entry().IsSameAs(who, true))
	    rv = false;
	if (rv)
	{
	    Magick::instance().chanserv.stats.i_Access++;
	    mstring entry = cstored->Access->Entry();

	    if (entry[0u] == '@')
		entry.MakeUpper();
	    else if (Magick::instance().nickserv.IsStored(entry))
		entry = Magick::instance().getSname(entry);
	    SEND(mynick, source, "LIST/DEL2", (entry, channel, Magick::instance().getMessage(source, "LIST/ACCESS")));
	    LOG(LM_DEBUG, "CHANSERV/ACCESS_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), entry, channel));
	    cstored->Access_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2", (who, channel, Magick::instance().getMessage(source, "LIST/ACCESS")));
	}
    }
    ETCB();
}

void ChanServ::do_access_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_access_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "VIEW") &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->Access_size())
    {
	SEND(mynick, source, "LIST/DISPLAY2", (channel, Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
    else
    {
	SEND(mynick, source, "LIST/EMPTY2", (channel, Magick::instance().getMessage(source, "LIST/ACCESS")));
	return;
    }

    MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Access"));
    unsigned int i;

    for (i = 1, cstored->Access = cstored->Access_begin(); cstored->Access != cstored->Access_end(); cstored->Access++, i++)
    {
	mstring entry = cstored->Access->Entry();

	if (entry[0u] == '@')
	    entry.MakeUpper();
	else if (Magick::instance().nickserv.IsStored(entry))
	    entry = Magick::instance().getSname(entry);
	::sendV(mynick, source, "%4d. %3d %s (%s)", i, cstored->Access->Value(), entry.c_str(),
		parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
			     mVarArray(cstored->Access->Last_Modify_Time().Ago(), cstored->Access->Last_Modifier())).c_str());
    }
    ETCB();
}

void ChanServ::do_akick_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_akick_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring who = params.ExtractWord(4, " ");
    mstring reason = Magick::instance().chanserv.DEF_Akick_Reason();

    if (params.WordCount(" ") > 4)
	reason = params.ExtractFrom(5, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);

    if (who[0u] == '@')
    {
	who.MakeUpper();
	if (!Magick::instance().commserv.IsList(who.After("@")))
	{
	    SEND(mynick, source, "COMMSERV/ISNOTSTORED", (who.After("@")));
	    return;
	}
    }
    else if (who.Contains("!") || who.Contains("@"))
    {
	if (!who.Contains("@"))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "LIST/AKICK"), '@'));
	    return;
	}
	else if (!who.Contains("!"))
	{
	    who.prepend("*!");
	}

	unsigned int i, num;
	bool super = cstored->GetAccess(source, "SUPER");

	// i+1 below because unsigned i will always be >= 0
	for (i = who.size() - 1, num = 0; i + 1 > 0; i--)
	{
	    switch (who[i])
	    {
	    case '@':
		if (!super)
		    i = 0;
		break;
	    case '!':		// ALL these constitute wildcards.
	    case '*':
	    case '?':
	    case '.':
		break;
	    default:
		num++;
	    }
	}
	// IF we have less than 1 char for 
	if (!super && num <= Magick::instance().config.Starthresh())
	{
	    SEND(mynick, source, "ERR_SYNTAX/STARTHRESH",
		 (Magick::instance().getMessage(source, "LIST/AKICK"), Magick::instance().config.Starthresh()));
	    return;
	}
	else if (num <= 1)
	{
	    SEND(mynick, source, "ERR_SYNTAX/STARTHRESH", (Magick::instance().getMessage(source, "LIST/AKICK"), 1));
	    return;
	}
    }
    else if (!Magick::instance().nickserv.IsStored(who))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (who));
	return;
    }
    else
    {
	who = Magick::instance().getSname(who);
	if (Magick::instance().nickserv.GetStored(who)->Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (Magick::instance().getSname(who)));
	    return;
	}
    }

    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "AKICK"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    {
	MLOCK((lck_ChanServ, lck_stored, channel.LowerCase(), "Access"));
	if (cstored->Access_find(who, Chan_Stored_t::C_IsIn))
	{
	    // Reject if they're higher on access list, else erase them
	    // from the access list, AKICK doesnt play nice with ACCESS.
	    if (cstored->Access->Value() >= cstored->GetAccess(source))
	    {
		SEND(mynick, source, "CS_STATUS/HIGHERACCESS", (who, channel));
		return;
	    }

	    if (cstored->Access_find(who))
	    {
		cstored->Access_erase();
	    }
	}
    }

    {
	MLOCK((lck_ChanServ, lck_stored, channel.LowerCase(), "Akick"));
	bool rv = cstored->Akick_find(who);

	// Result is false if its a mask, but not the same one
	if (rv && who.Contains("@") && who[0u] != '@' && !cstored->Akick->Entry().IsSameAs(who, true))
	    rv = false;
	if (rv)
	{
	    SEND(mynick, source, "LIST/EXISTS2", (who, channel, Magick::instance().getMessage(source, "LIST/AKICK")));
	    return;
	}
	cstored->Akick_insert(who, reason, source);
	who = cstored->Akick->Entry();
    }

    Magick::instance().chanserv.stats.i_Akick++;
    SEND(mynick, source, "LIST/ADD2", (who, channel, Magick::instance().getMessage(source, "LIST/AKICK")));
    LOG(LM_DEBUG, "CHANSERV/AKICK_ADD",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), who, channel));

    if (Magick::instance().chanserv.IsLive(channel.c_str()))
    {
	unsigned int i;

	vector < mstring > kickees;
	map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);

	if (who.Contains("@"))
	{
	    if (who[0u] == '@')
	    {
		mstring committee = who.After("@");

		if (Magick::instance().commserv.IsList(committee))
		{
		    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);
		    for (i = 0; i < clive->Users(); i++)
		    {
			if (comm->IsOn(clive->User(i)))
			    kickees.push_back(clive->User(i));
		    }
		}
	    }
	    else
	    {
		// Kick matching users ...
		for (i = 0; i < clive->Users(); i++)
		{
		    // MAN these commands can get REAL long .. ;)
		    if (Magick::instance().nickserv.IsLive(clive->User(i)) &&
			Magick::instance().nickserv.GetLive(clive->User(i))->Mask(Nick_Live_t::N_U_P_H).Matches(who, true))
		    {
			kickees.push_back(clive->User(i));
		    }
		}
	    }
	}
	else
	{
	    // Kick stored user ...
	    if (Magick::instance().nickserv.IsStored(who))
	    {
		mstring realnick = Magick::instance().nickserv.GetStored(who)->Host();

		if (realnick.empty())
		    realnick = who;
		map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(realnick);
		if (clive->IsIn(realnick))
		    kickees.push_back(realnick);
		for (i = 0; i < nick->Siblings(); i++)
		{
		    if (clive->IsIn(nick->Sibling(i)))
			kickees.push_back(nick->Sibling(i));
		}
	    }
	}
	for (i = 0; i < kickees.size(); i++)
	{
	    Magick::instance().server.KICK(mynick, kickees[i], channel,
					   ((!reason.empty()) ? reason : Magick::instance().chanserv.DEF_Akick_Reason()));
	}
    }
    ETCB();
}

void ChanServ::do_akick_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_akick_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring who = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "AKICK"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (!cstored->Akick_size())
    {
	SEND(mynick, source, "LIST/EMPTY2", (channel, Magick::instance().getMessage(source, "LIST/AKICK")));
	return;
    }

    if (who.IsNumber())
    {
	if (who.Contains("."))
	{
	    NSEND(mynick, source, "ERR_STYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(who);
	if (num < 1 || num > cstored->Akick_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, cstored->Akick_size()));
	    return;
	}

	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Akick"));
	cstored->Akick = find_if(cstored->Akick_begin(), cstored->Akick_end(), FindNumberedEntry(num - 1));
	if (cstored->Akick != cstored->Akick_end())
	{
	    Magick::instance().chanserv.stats.i_Akick++;
	    mstring entry = cstored->Akick->Entry();

	    if (entry[0u] == '@')
		entry.MakeUpper();
	    else if (Magick::instance().nickserv.IsStored(entry))
		entry = Magick::instance().getSname(entry);
	    SEND(mynick, source, "LIST/DEL2", (entry, channel, Magick::instance().getMessage(source, "LIST/AKICK")));
	    LOG(LM_DEBUG, "CHANSERV/AKICK_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), entry, channel));
	    cstored->Akick_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2_NUMBER",
		 (num, channel, Magick::instance().getMessage(source, "LIST/AKICK")));
	}
    }
    else
    {
	if (who.Contains("@") || who.Contains("!"))
	{
	    if (!who.Contains("@"))
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "LIST/AKICK"), '@'));
		return;
	    }
	    else if (who[0u] != '@' && !who.Contains("!"))
	    {
		who.prepend("*!");
	    }
	}

	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Akick"));
	bool rv = cstored->Akick_find(who);

	// Result is false if its a mask, but not the same one
	if (rv && who.Contains("@") && who[0u] != '@' && !cstored->Akick->Entry().IsSameAs(who, true))
	    rv = false;
	if (rv)
	{
	    Magick::instance().chanserv.stats.i_Akick++;
	    mstring entry = cstored->Akick->Entry();

	    if (entry[0u] == '@')
		entry.MakeUpper();
	    else if (Magick::instance().nickserv.IsStored(entry))
		entry = Magick::instance().getSname(entry);
	    SEND(mynick, source, "LIST/DEL2", (entry, channel, Magick::instance().getMessage(source, "LIST/AKICK")));
	    LOG(LM_DEBUG, "CHANSERV/AKICK_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), entry, channel));
	    cstored->Akick_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2", (who, channel, Magick::instance().getMessage(source, "LIST/AKICK")));
	}
    }
    ETCB();
}

void ChanServ::do_akick_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_akick_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "VIEW") &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source)))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->Akick_size())
    {
	SEND(mynick, source, "LIST/DISPLAY2", (channel, Magick::instance().getMessage(source, "LIST/AKICK")));
    }
    else
    {
	SEND(mynick, source, "LIST/EMPTY2", (channel, Magick::instance().getMessage(source, "LIST/AKICK")));
	return;
    }

    MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Akick"));
    int i;

    for (i = 1, cstored->Akick = cstored->Akick_begin(); cstored->Akick != cstored->Akick_end(); cstored->Akick++, i++)
    {
	mstring entry = cstored->Akick->Entry();

	if (entry[0u] == '@')
	    entry.MakeUpper();
	else if (Magick::instance().nickserv.IsStored(entry))
	    entry = Magick::instance().getSname(entry);
	::sendV(mynick, source, "%4d. %s (%s)", i, entry.c_str(),
		parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
			     mVarArray(cstored->Akick->Last_Modify_Time().Ago(), cstored->Akick->Last_Modifier())).c_str());
	::send(mynick, source, "      " + cstored->Akick->Value());
    }
    ETCB();
}

void ChanServ::do_greet_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_greet_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = Magick::instance().getSname(source);
    mstring option = params.ExtractFrom(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    map_entry < Nick_Stored_t > nstored;
    // If we have 2 params, and we have SUPER access, or are a SOP
    if ((option[0U] == '@' || option[0U] == '!') && params.WordCount(" ") > 4 && cstored->GetAccess(source, "OVERGREET"))
    {
	if (option[0U] == '@')
	{
	    target = params.ExtractWord(4, " ").After("@");
	    option = params.ExtractFrom(5, " ");
	    if (!Magick::instance().nickserv.IsStored(target))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
		return;
	    }
	    nstored = Magick::instance().nickserv.GetStored(target);
	    target = nstored->Name();
	    if (nstored->Forbidden())
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
		return;
	    }
	}
	else
	    nstored = Magick::instance().nickserv.GetStored(target);
	while (option[1U] == '!')
	    option = option.After("!");
    }
    else if (!cstored->GetAccess(source, "GREET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }
    else
    {
	while (option[0U] == '!')
	    option = option.After("!");
	nstored = Magick::instance().nickserv.GetStored(target);
    }

    if (!nstored->Host().empty())
    {
	target = nstored->Host();
    }

    {
	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Greet"));
	if (cstored->Greet_find(target))
	{
	    if (cstored->Greet->Entry() [0U] == '!' && source.IsSameAs(target, true) &&
		!cstored->GetAccess(source, "OVERGREET"))
	    {
		SEND(mynick, source, "CS_STATUS/LOCKGREET", (channel));
		return;
	    }
	    cstored->Greet_erase();
	}
    }
    cstored->Greet_insert(option, target);
    Magick::instance().chanserv.stats.i_Greet++;
    SEND(mynick, source, "LIST/ADD2", (target, channel, Magick::instance().getMessage(source, "LIST/GREET")));
    LOG(LM_DEBUG, "CHANSERV/GREET_ADD",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
    ETCB();
}

void ChanServ::do_greet_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_greet_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = Magick::instance().getSname(source);

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 3 && cstored->GetAccess(source, "OVERGREET"))
    {
	target = params.ExtractWord(4, " ").After("@");
    }
    else if (!cstored->GetAccess(source, "GREET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (Magick::instance().nickserv.IsStored(target))
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
	target = nstored->Name();
	if (!nstored->Host().empty())
	    target = nstored->Host();
    }

    if (!source.IsSameAs(target, true))
    {
	cstored->Greet_erase();
	Magick::instance().chanserv.stats.i_Greet++;
	SEND(mynick, source, "LIST/DEL2", (target, channel, Magick::instance().getMessage(source, "LIST/GREET")));
    }
    else
    {
	{
	    MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Greet"));
	    if (cstored->Greet_find(target))
	    {
		if (cstored->Greet->Entry() [0U] == '!' && !cstored->GetAccess(source, "OVERGREET"))
		{
		    SEND(mynick, source, "CS_STATUS/LOCKGREET", (channel));
		    return;
		}
		cstored->Greet_erase();
		Magick::instance().chanserv.stats.i_Greet++;
		SEND(mynick, source, "LIST/DEL2", (target, channel, Magick::instance().getMessage(source, "LIST/GREET")));
		LOG(LM_DEBUG, "CHANSERV/GREET_DEL",
		    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, channel));
	    }
	    else
	    {
		SEND(mynick, source, "LIST/NOTEXISTS2",
		     (target, channel, Magick::instance().getMessage(source, "LIST/GREET")));
	    }
	}
    }
    ETCB();
}

void ChanServ::do_greet_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_greet_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target;
    bool all = false;

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 3 &&
	(cstored->GetAccess(source, "OVERGREET") ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))))
    {
	if (params.ExtractWord(4, " ").IsSameAs("all", true))
	    all = true;
	else
	    target = params.ExtractWord(4, " ").After("@");
    }
    else if (!cstored->GetAccess(source, "GREET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }
    else
    {
	target = Magick::instance().getSname(source);
    }

    if (!target.empty() && Magick::instance().nickserv.IsStored(target))
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
	if (!nstored->Host().empty())
	    target = nstored->Host();
    }

    bool found = false;

    {
	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Greet"));
	if (all)
	{
	    for (cstored->Greet = cstored->Greet_begin(); cstored->Greet != cstored->Greet_end(); cstored->Greet++)
	    {
		::sendV(mynick, source, "[%s] %s", cstored->Greet->Last_Modifier().c_str(), cstored->Greet->Entry().c_str());
		found = true;
	    }
	}
	else
	{
	    cstored->Greet = find_if(cstored->Greet_begin(), cstored->Greet_end(), ModifierIsSameAs(target, true));
	    if (cstored->Greet != cstored->Greet_end())
	    {
		::sendV(mynick, source, "[%s] %s", cstored->Greet->Last_Modifier().c_str(), cstored->Greet->Entry().c_str());
		found = true;
	    }
	}
    }
    if (!found)
    {
	if (all)
	    SEND(mynick, source, "LIST/EMPTY2", (channel, Magick::instance().getMessage(source, "LIST/GREET")));
	else
	    SEND(mynick, source, "LIST/NOTEXISTS2", (target, channel, Magick::instance().getMessage(source, "LIST/GREET")));
    }
    ETCB();
}

void ChanServ::do_message_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_message_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring text = params.ExtractFrom(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "MESSAGE"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->Message_size() >= Magick::instance().chanserv.Max_Messages())
    {
	unsigned int i;
	for (i=0; i<Magick::instance().chanserv.Ovr_Messages().WordCount(" "); i++)
	    if (Magick::instance().commserv.IsList(Magick::instance().chanserv.Ovr_Messages().ExtractWord(i, " ")) &&
		Magick::instance().commserv.GetList(Magick::instance().chanserv.Ovr_Messages().ExtractWord(i, " "))->IsOn(source))
		break;
	if (!Magick::instance().chanserv.Ovr_Messages().size() ||
	    i == Magick::instance().chanserv.Ovr_Messages().WordCount(" "))
	{
	    SEND(mynick, source, "CS_STATUS/MAX_MESSAGES", (channel));
	    return;
	}
    }

    cstored->Message_insert(text, source);
    Magick::instance().chanserv.stats.i_Message++;
    SEND(mynick, source, "LIST/ADD2_NUMBER",
	 (cstored->Message_size(), channel, Magick::instance().getMessage(source, "LIST/JOINMSG")));
    LOG(LM_DEBUG, "CHANSERV/MESSAGE_ADD", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), channel));
    ETCB();
}

void ChanServ::do_message_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_message_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (!target.IsNumber() || target.Contains("."))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "MESSAGE"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (!cstored->Message_size())
    {
	SEND(mynick, source, "LIST/EMPTY2", (channel, Magick::instance().getMessage(source, "LIST/JOINMSG")));
	return;
    }

    int num = atoi(target);
    if (num < 1 || num > static_cast < int > (cstored->Message_size()))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, cstored->Message_size()));
	return;
    }

    {
	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Message"));
	if (cstored->Message_find(num - 1))
	{
	    cstored->Message_erase();
	    Magick::instance().chanserv.stats.i_Message++;
	    SEND(mynick, source, "LIST/DEL2_NUMBER", (num, channel, Magick::instance().getMessage(source, "LIST/JOINMSG")));
	    LOG(LM_DEBUG, "CHANSERV/MESSAGE_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), channel));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS2_NUMBER",
		 (num, channel, Magick::instance().getMessage(source, "LIST/JOINMSG")));
	}
    }
    ETCB();
}

void ChanServ::do_message_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_message_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!
	(cstored->GetAccess(source, "MESSAGE") ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (!cstored->Message_size())
    {
	SEND(mynick, source, "LIST/EMPTY2", (channel, Magick::instance().getMessage(source, "LIST/JOINMSG")));
	return;
    }

    SEND(mynick, source, "LIST/DISPLAY2", (channel, Magick::instance().getMessage(source, "LIST/JOINMSG")));

    {
	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Message"));
	int i;

	for (i = 1, cstored->Message = cstored->Message_begin(); cstored->Message != cstored->Message_end();
	     cstored->Message++, i++)
	{
	    ::sendV(mynick, source, "%d. %s", i, cstored->Message->Entry().c_str());
	}
    }
    ETCB();
}

void ChanServ::do_set_Founder(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Founder", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring founder = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(founder))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (founder));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(founder);
    founder = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (founder));
	return;
    }
    else if (!nstored->Host().empty())
    {
	founder = nstored->Host();
    }

    if (Magick::instance().chanserv.Max_Per_Nick() && nstored->MyChannels() >= Magick::instance().chanserv.Max_Per_Nick())
    {
	unsigned int i;
	for (i=0; i<Magick::instance().chanserv.Ovr_Per_Nick().WordCount(" "); i++)
	    if (Magick::instance().commserv.IsList(Magick::instance().chanserv.Ovr_Per_Nick().ExtractWord(i, " ")) &&
		Magick::instance().commserv.GetList(Magick::instance().chanserv.Ovr_Per_Nick().ExtractWord(i, " "))->IsOn(source))
		break;
	if (!Magick::instance().chanserv.Ovr_Per_Nick().size() ||
	    i == Magick::instance().chanserv.Ovr_Per_Nick().WordCount(" "))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_TOOMANY", (founder));
	    return;
	}
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (!
	(cstored->GetAccess(source) > Magick::instance().chanserv.Level_Max() ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_Owner()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_Owner())->IsOn(source))))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored->Founder(founder);
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (Magick::instance().getMessage(source, "CS_SET/FOUNDER"), channel, founder));
    LOG(LM_INFO, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/FOUNDER"), channel, founder));
    ETCB();
}

void ChanServ::do_set_CoFounder(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_CoFounder", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring founder = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(founder))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (founder));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(founder);
    founder = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (founder));
	return;
    }
    else if (!nstored->Host().empty())
    {
	founder = nstored->Host();
    }

    if (Magick::instance().chanserv.Max_Per_Nick() && nstored->MyChannels() >= Magick::instance().chanserv.Max_Per_Nick())
    {
	unsigned int i;
	for (i=0; i<Magick::instance().chanserv.Ovr_Per_Nick().WordCount(" "); i++)
	    if (Magick::instance().commserv.IsList(Magick::instance().chanserv.Ovr_Per_Nick().ExtractWord(i, " ")) &&
		Magick::instance().commserv.GetList(Magick::instance().chanserv.Ovr_Per_Nick().ExtractWord(i, " "))->IsOn(source))
		break;
	if (!Magick::instance().chanserv.Ovr_Per_Nick().size() ||
	    i == Magick::instance().chanserv.Ovr_Per_Nick().WordCount(" "))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_TOOMANY", (founder));
	    return;
	}
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (cstored->Founder().IsSameAs(founder, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COFOUNDER");
	return;
    }

    if (!
	(cstored->GetAccess(source) > Magick::instance().chanserv.Level_Max() ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_Owner()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_Owner())->IsOn(source))))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (founder.IsSameAs("none", true))
	founder.erase();

    cstored->CoFounder(founder);
    Magick::instance().chanserv.stats.i_Set++;
    if (founder.empty())
    {
	SEND(mynick, source, "CS_COMMAND/UNSET", (Magick::instance().getMessage(source, "CS_SET/COFOUNDER"), channel));
	LOG(LM_INFO, "CHANSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("CS_SET/COFOUNDER"), channel));
    }
    else
    {
	SEND(mynick, source, "CS_COMMAND/SET_TO",
	     (Magick::instance().getMessage(source, "CS_SET/COFOUNDER"), channel, founder));
	LOG(LM_INFO, "CHANSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("CS_SET/COFOUNDER"), channel, founder));
    }
    ETCB();
}

void ChanServ::do_set_Description(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Description", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring option = params.ExtractFrom(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored->Description(option);
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (Magick::instance().getMessage(source, "CS_SET/DESCRIPTION"), channel, option));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/DESCRIPTION"), channel, option));
    ETCB();
}

void ChanServ::do_set_Password(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Password", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (password.length() < 5 || password.IsSameAs(channel.After(channel[0u]), true) || password.IsSameAs(channel, true) ||
	password.IsSameAs(source, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored->GetAccess(source) <= Magick::instance().chanserv.Level_Max())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored->Password(password);
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO", (Magick::instance().getMessage(source, "CS_SET/PASSWORD"), channel, password));
    LOG(LM_INFO, "CHANSERV/SET_PASSWORD", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), channel));
    ETCB();
}

void ChanServ::do_set_Email(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Email", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring option = params.ExtractFrom(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (option.IsSameAs("none", true))
	option.erase();
    else if (!option.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "CS_SET/EMAIL"), '@'));
	return;
    }
    else if (option.WordCount("@") != 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE", (Magick::instance().getMessage(source, "CS_SET/EMAIL"), '@'));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored->Email(option);
    Magick::instance().chanserv.stats.i_Set++;
    if (option.empty())
    {
	SEND(mynick, source, "CS_COMMAND/UNSET", (Magick::instance().getMessage(source, "CS_SET/EMAIL"), channel));
	LOG(LM_DEBUG, "CHANSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("CS_SET/EMAIL"), channel));
    }
    else
    {
	SEND(mynick, source, "CS_COMMAND/SET_TO", (Magick::instance().getMessage(source, "CS_SET/EMAIL"), channel, option));
	LOG(LM_DEBUG, "CHANSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("CS_SET/EMAIL"), channel, option));
    }
    ETCB();
}

void ChanServ::do_set_URL(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_URL", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring option = params.ExtractFrom(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (option.IsSameAs("none", true))
	option.erase();

    if (option.SubString(0, 6).IsSameAs("http://", true))
    {
	option.erase(0, 6);
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    cstored->URL(option);
    Magick::instance().chanserv.stats.i_Set++;
    if (option.empty())
    {
	SEND(mynick, source, "CS_COMMAND/UNSET", (Magick::instance().getMessage(source, "CS_SET/URL"), channel));
	LOG(LM_DEBUG, "CHANSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("CS_SET/URL"), channel));
    }
    else
    {
	SEND(mynick, source, "CS_COMMAND/SET_TO",
	     (Magick::instance().getMessage(source, "CS_SET/URL"), channel, "http://" + option));
	LOG(LM_DEBUG, "CHANSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("CS_SET/URL"), channel, "http://" + option));
    }
    ETCB();
}

void ChanServ::do_set_Comment(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Comment", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring option = params.ExtractFrom(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (option.IsSameAs("none", true))
	option.erase();

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    cstored->Comment(option);
    Magick::instance().chanserv.stats.i_Set++;
    if (option.empty())
    {
	SEND(mynick, source, "CS_COMMAND/UNSET", (Magick::instance().getMessage(source, "CS_SET/COMMENT"), channel));
	LOG(LM_DEBUG, "CHANSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("CS_SET/COMMENT"), channel));
    }
    else
    {
	SEND(mynick, source, "CS_COMMAND/SET_TO", (Magick::instance().getMessage(source, "CS_SET/COMMENT"), channel, option));
	LOG(LM_DEBUG, "CHANSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("CS_SET/COMMENT"), channel, option));
    }
    ETCB();
}

void ChanServ::do_set_Mlock(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Mlock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring option = params.ExtractFrom(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (option.IsSameAs("default", true) || option.IsSameAs("reset", true))
    {
	option = Magick::instance().chanserv.DEF_MLock();
    }

    vector < mstring > retval;
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    // If you have OVR_CS_Mode, then you can still change the mlock ...
    if (cstored->Forbidden())
    {
	if (!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Mode()) &&
	      Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Mode())->IsOn(source)))
	{
	    SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	    return;
	}
    }
    else if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    bool wasin = (Magick::instance().chanserv.IsLive(channel) &&
		  Magick::instance().chanserv.GetLive(channel)->IsIn(Magick::instance().chanserv.FirstName()));
    retval = cstored->Mlock(source, option);
    Magick::instance().chanserv.stats.i_Set++;
    for (unsigned int i = 0; i < retval.size(); i++)
	::send(mynick, source, retval[i]);
    if (cstored->Forbidden())
    {
	if (wasin && !cstored->Mlock_On().Contains("i") && cstored->Mlock_Key().empty())
	    Magick::instance().server.PART(Magick::instance().chanserv.FirstName(), channel);
	else if (!wasin && (cstored->Mlock_On().Contains("i") || !cstored->Mlock_Key().empty()))
	    Magick::instance().server.JOIN(Magick::instance().chanserv.FirstName(), channel);
    }
    ETCB();
}

void ChanServ::do_set_BanTime(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_BanTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring value = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    unsigned long num = FromHumanTime(value);

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Bantime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/BANTIME"), channel));
	return;
    }

    cstored->Bantime(num);
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/BANTIME"), channel, ToHumanTime(num, source)));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/BANTIME"), channel, ToHumanTime(num, source)));
    ETCB();
}

void ChanServ::do_set_PartTime(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_PartTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring value = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    unsigned long num = FromHumanTime(value);

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Parttime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/PARTTIME"), channel));
	return;
    }

    cstored->Parttime(num);
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/PARTTIME"), channel, ToHumanTime(num, source)));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/PARTTIME"), channel, ToHumanTime(num, source)));
    ETCB();
}

void ChanServ::do_set_KeepTopic(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_KeepTopic", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Keeptopic())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Keeptopic())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/KEEPTOPIC"), channel));
	return;
    }

    cstored->Keeptopic(onoff.GetBool());
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/KEEPTOPIC"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KEEPTOPIC"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_set_TopicLock(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_TopicLock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Topiclock())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Topiclock())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/TOPICLOCK"), channel));
	return;
    }

    cstored->Topiclock(onoff.GetBool());
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/TOPICLOCK"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KEEPTOPIC"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_set_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Private())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/PRIVATE"), channel));
	return;
    }

    cstored->Private(onoff.GetBool());
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/PRIVATE"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/PRIVATE"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_set_SecureOps(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_SecureOps", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Secureops())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Secureops())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/SECUREOPS"), channel));
	return;
    }

    cstored->Secureops(onoff.GetBool());
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/SECUREOPS"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/SECUREOPS"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_set_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Secure())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/SECURE"), channel));
	return;
    }

    cstored->Secure(onoff.GetBool());
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/SECURE"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/SECURE"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_set_NoExpire(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_NoExpire", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_NoExpire())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_NoExpire())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/NOEXPIRE"), channel));
	return;
    }

    cstored->NoExpire(onoff.GetBool());
    Magick::instance().chanserv.stats.i_NoExpire++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/NOEXPIRE"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/NOEXPIRE"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_set_Anarchy(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Anarchy", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Anarchy())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Anarchy())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/ANARCHY"), channel));
	return;
    }

    cstored->Anarchy(onoff.GetBool());
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/ANARCHY"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/ANARCHY"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_set_KickOnBan(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_KickOnBan", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_KickOnBan())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_KickOnBan())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/KICKONBAN"), channel));
	return;
    }

    cstored->KickOnBan(onoff.GetBool());
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/KICKONBAN"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KICKONBAN"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_set_Restricted(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Restricted", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Restricted())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Restricted())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/RESTRICTED"), channel));
	return;
    }

    cstored->Restricted(onoff.GetBool());
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/RESTRICTED"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/RESTRICTED"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_set_Join(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Join())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Join())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/JOIN"), channel));
	return;
    }

    cstored->Join(onoff.GetBool());
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/JOIN"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("CS_SET/JOIN"),
	 channel, (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
				     Magick::instance().getMessage(source, "VALS/OFF"))));
    if (onoff.GetBool() && Magick::instance().chanserv.IsLive(channel) &&
	!Magick::instance().chanserv.GetLive(channel)->IsIn(Magick::instance().chanserv.FirstName()))
    {
	Magick::instance().server.JOIN(Magick::instance().chanserv.FirstName(), channel);
    }
    else if (!onoff.GetBool() && Magick::instance().chanserv.IsLive(channel) &&
	     Magick::instance().chanserv.GetLive(channel)->IsIn(Magick::instance().chanserv.FirstName()))
    {
	Magick::instance().server.PART(Magick::instance().chanserv.FirstName(), channel);
    }
    ETCB();
}

void ChanServ::do_set_Revenge(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_set_Revenge", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring option = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (option.IsSameAs("default", true) || option.IsSameAs("reset", true))
    {
	option = Magick::instance().chanserv.DEF_Revenge();
    }
    else if (!Magick::instance().chanserv.IsRevengeLevel(option))
    {
	SEND(mynick, source, "CS_STATUS/NOREVENGE", (option.UpperCase()));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    if (cstored->L_Revenge())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/REVENGE"), channel));
	return;
    }

    cstored->Revenge(option.UpperCase());
    Magick::instance().chanserv.stats.i_Set++;
    SEND(mynick, source, "CS_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "CS_SET/REVENGE"), channel,
	  Magick::instance().getMessage(source, "CS_SET/REV_" + option.UpperCase())));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/REVENGE"), channel,
	 Magick::instance().getMessage("CS_SET/REV_" + option.UpperCase())));
    ETCB();
}

void ChanServ::do_lock_Mlock(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_Mlock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring option = params.ExtractFrom(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (option.IsSameAs("default", true) || option.IsSameAs("reset", true))
    {
	option = Magick::instance().chanserv.DEF_MLock();
    }

    vector < mstring > retval;
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    retval = cstored->L_Mlock(source, option);
    Magick::instance().chanserv.stats.i_Lock++;
    for (unsigned int i = 0; i < retval.size(); i++)
	::send(mynick, source, retval[i]);
    ETCB();
}

void ChanServ::do_lock_BanTime(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_BanTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring value = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    unsigned long num = FromHumanTime(value);

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Bantime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/BANTIME"), channel));
	return;
    }

    cstored->L_Bantime(false);
    cstored->Bantime(num);
    cstored->L_Bantime(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/BANTIME"), channel, ToHumanTime(num)));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/BANTIME"), channel, ToHumanTime(num)));
    ETCB();
}

void ChanServ::do_lock_PartTime(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_PartTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring value = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    unsigned long num = FromHumanTime(value);

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Parttime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/PARTTIME"), channel));
	return;
    }

    cstored->L_Parttime(false);
    cstored->Parttime(num);
    cstored->L_Parttime(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/PARTTIME"), channel, ToHumanTime(num, source)));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/PARTTIME"), channel, ToHumanTime(num, source)));
    ETCB();
}

void ChanServ::do_lock_KeepTopic(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_KeepTopic", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Keeptopic())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Keeptopic())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/KEEPTOPIC"), channel));
	return;
    }

    cstored->L_Keeptopic(false);
    cstored->Keeptopic(onoff.GetBool());
    cstored->L_Keeptopic(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/KEEPTOPIC"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KEEPTOPIC"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_lock_TopicLock(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_TopicLock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Topiclock())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Topiclock())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/TOPICLOCK"), channel));
	return;
    }

    cstored->L_Topiclock(false);
    cstored->Topiclock(onoff.GetBool());
    cstored->L_Topiclock(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/TOPICLOCK"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/TOPICLOCK"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_lock_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Private())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/PRIVATE"), channel));
	return;
    }

    cstored->L_Private(false);
    cstored->Private(onoff.GetBool());
    cstored->L_Private(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/PRIVATE"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/PRIVATE"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_lock_SecureOps(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_SecureOps", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Secureops())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Secureops())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/SECUREOPS"), channel));
	return;
    }

    cstored->L_Secureops(false);
    cstored->Secureops(onoff.GetBool());
    cstored->L_Secureops(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/SECUREOPS"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/SECUREOPS"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_lock_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Secure())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/SECURE"), channel));
	return;
    }

    cstored->L_Secure(false);
    cstored->Secure(onoff.GetBool());
    cstored->L_Secure(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/SECURE"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/SECURE"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_lock_Anarchy(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_Anarchy", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Anarchy())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Anarchy())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/ANARCHY"), channel));
	return;
    }

    cstored->L_Anarchy(false);
    cstored->Anarchy(onoff.GetBool());
    cstored->L_Anarchy(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/ANARCHY"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/ANARCHY"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_lock_KickOnBan(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_KickOnBan", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_KickOnBan())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_KickOnBan())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/KICKONBAN"), channel));
	return;
    }

    cstored->L_KickOnBan(false);
    cstored->KickOnBan(onoff.GetBool());
    cstored->L_KickOnBan(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/KICKONBAN"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KICKONBAN"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_lock_Restricted(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_Restricted", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Restricted())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Restricted())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/RESTRICTED"), channel));
	return;
    }

    cstored->L_Restricted(false);
    cstored->Restricted(onoff.GetBool());
    cstored->L_Restricted(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/RESTRICTED"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/RESTRICTED"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void ChanServ::do_lock_Join(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().chanserv.DEF_Join())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Join())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/JOIN"), channel));
	return;
    }

    cstored->L_Join(false);
    cstored->Join(onoff.GetBool());
    cstored->L_Join(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/JOIN"), channel,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("CS_SET/JOIN"),
	 channel, (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
				     Magick::instance().getMessage(source, "VALS/OFF"))));
    if (onoff.GetBool() && Magick::instance().chanserv.IsLive(channel) &&
	!Magick::instance().chanserv.GetLive(channel)->IsIn(Magick::instance().chanserv.FirstName()))
    {
	Magick::instance().server.JOIN(Magick::instance().chanserv.FirstName(), channel);
    }
    else if (!onoff.GetBool() && Magick::instance().chanserv.IsLive(channel) &&
	     Magick::instance().chanserv.GetLive(channel)->IsIn(Magick::instance().chanserv.FirstName()))
    {
	Magick::instance().server.PART(Magick::instance().chanserv.FirstName(), channel);
    }
    ETCB();
}

void ChanServ::do_lock_Revenge(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_lock_Revenge", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring option = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    if (option.IsSameAs("default", true) || option.IsSameAs("reset", true))
    {
	option = Magick::instance().chanserv.DEF_Revenge();
    }
    else if (!Magick::instance().chanserv.IsRevengeLevel(option))
    {
	SEND(mynick, source, "CS_STATUS/NOREVENGE", (option.UpperCase()));
	return;
    }

    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Revenge())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/REVENGE"), channel));
	return;
    }

    cstored->L_Revenge(false);
    cstored->Revenge(option.UpperCase());
    cstored->L_Revenge(true);
    Magick::instance().chanserv.stats.i_Lock++;
    SEND(mynick, source, "CS_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "CS_SET/REVENGE"), channel,
	  Magick::instance().getMessage(source, "CS_SET/REV_" + option.UpperCase())));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/REVENGE"), channel,
	 Magick::instance().getMessage("CS_SET/REV_" + option.UpperCase())));
    ETCB();
}

void ChanServ::do_unlock_Mlock(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_Mlock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }

    vector < mstring > retval;
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    retval = cstored->L_Mlock(source, "");
    Magick::instance().chanserv.stats.i_Unlock++;
    for (unsigned int i = 0; i < retval.size(); i++)
	::send(mynick, source, retval[i]);
    ETCB();
}

void ChanServ::do_unlock_BanTime(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_BanTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Bantime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/BANTIME"), channel));
	return;
    }

    cstored->L_Bantime(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/BANTIME"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/BANTIME"), channel));
    ETCB();
}

void ChanServ::do_unlock_PartTime(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_PartTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Parttime())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/PARTTIME"), channel));
	return;
    }

    cstored->L_Parttime(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/PARTTIME"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/PARTTIME"), channel));
    ETCB();
}

void ChanServ::do_unlock_KeepTopic(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_KeepTopic", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Keeptopic())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/KEEPTOPIC"), channel));
	return;
    }

    cstored->L_Keeptopic(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/KEEPTOPIC"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KEEPTOPIC"), channel));
    ETCB();
}

void ChanServ::do_unlock_TopicLock(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_TopicLock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Topiclock())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/TOPICLOCK"), channel));
	return;
    }

    cstored->L_Topiclock(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/TOPICLOCK"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/TOPICLOCK"), channel));
    ETCB();
}

void ChanServ::do_unlock_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Private())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/PRIVATE"), channel));
	return;
    }

    cstored->L_Private(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/PRIVATE"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/PRIVATE"), channel));
    ETCB();
}

void ChanServ::do_unlock_SecureOps(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_SecureOps", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Secureops())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/SECUREOPS"), channel));
	return;
    }

    cstored->L_Secureops(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/SECUREOPS"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/SECUREOPS"), channel));
    ETCB();
}

void ChanServ::do_unlock_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Secure())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/SECURE"), channel));
	return;
    }

    cstored->L_Secure(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/SECURE"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/SECURE"), channel));
    ETCB();
}

void ChanServ::do_unlock_Anarchy(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_Anarchy", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Anarchy())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/ANARCHY"), channel));
	return;
    }

    cstored->L_Anarchy(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/ANARCHY"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/ANARCHY"), channel));
    ETCB();
}

void ChanServ::do_unlock_KickOnBan(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_KickOnBan", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_KickOnBan())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/KICKONBAN"), channel));
	return;
    }

    cstored->L_KickOnBan(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/KICKONBAN"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KICKONBAN"), channel));
    ETCB();
}

void ChanServ::do_unlock_Restricted(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_Restricted", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Restricted())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/RESTRICTED"), channel));
	return;
    }

    cstored->L_Restricted(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/RESTRICTED"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/RESTRICTED"), channel));
    ETCB();
}

void ChanServ::do_unlock_Join(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Join())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/JOIN"), channel));
	return;
    }

    cstored->L_Join(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/JOIN"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("CS_SET/JOIN"),
	 channel));
    ETCB();
}

void ChanServ::do_unlock_Revenge(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("ChanServ::do_unlock_Revenge", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(channel);
    channel = cstored->Name();

    if (cstored->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    if (Magick::instance().chanserv.LCK_Revenge())
    {
	SEND(mynick, source, "CS_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "CS_SET/REVENGE"), channel));
	return;
    }

    cstored->L_Revenge(false);
    Magick::instance().chanserv.stats.i_Unlock++;
    SEND(mynick, source, "CS_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "CS_SET/REVENGE"), channel));
    LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/REVENGE"), channel));
    ETCB();
}

long ChanServ::LVL(const mstring & level) const
{
    BTCB();
    FT("ChanServ::LVL", (level));
    long retval = 0;
    map < mstring, long >::const_iterator i = lvl.find(level.UpperCase());

    if (i == lvl.end() || i->second > Level_Max() + 1 || i->second < Level_Min())
    {
	retval = Level_Min() - 1;
    }
    else
    {
	retval = i->second;
    }
    RET(retval);
    ETCB();
}

vector < mstring > ChanServ::LVL() const
{
    BTCB();
    NFT("ChanServ::LVL");

    vector < mstring > retval;
    map < mstring, long >::const_iterator iter;

    for (iter = lvl.begin(); iter != lvl.end(); iter++)
    {
	retval.push_back(iter->first.UpperCase());
    }
    NRET(vector < mstring >, retval);
    ETCB();
}

bool ChanServ::IsLVL(const mstring & level) const
{
    BTCB();
    FT("ChanServ::IsLVL", (level));
    bool retval = lvl.find(level.UpperCase()) != lvl.end();

    RET(retval);
    ETCB();
}

SXP::Tag ChanServ::tag_ChanServ("ChanServ");

void ChanServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    FT("ChanServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Chan_Stored_t *cs = new Chan_Stored_t;

    if (pElement->IsA(cs->GetClassTag()))
    {
	cs_array.push_back(cs);
	pIn->ReadTo(cs);
    }
    else
    {
	delete cs;
    }
    ETCB();
}

void ChanServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);
    static_cast < void > (pElement);

    FT("ChanServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
    ETCB();
}

void ChanServ::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    FT("ChanServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_ChanServ);

    ChanServ::stored_t::iterator iter;
    {
	RLOCK((lck_ChanServ, lck_stored));
	for (iter = StoredBegin(); iter != StoredEnd(); iter++)
	{
	    map_entry < Chan_Stored_t > cstored(iter->second);
	    pOut->WriteSubElement(cstored);
	}
    }

    pOut->EndObject(tag_ChanServ);
    ETCB();
}

void ChanServ::PostLoad()
{
    BTCB();
    NFT("ChanServ::PostLoad");
    // Linkage, etc
    unsigned int i, j;

    for (i = 0; i < cs_array.size(); i++)
    {
	if (cs_array[i] != NULL)
	{
	    for (j = 0; j < cs_array[i]->level_array.size(); j++)
	    {
		if (cs_array[i]->level_array[j] != NULL)
		{
		    // MUST delete it, coz it might already exist from defaults.
		    cs_array[i]->i_Level.erase(*cs_array[i]->level_array[j]);
		    cs_array[i]->i_Level.insert(*cs_array[i]->level_array[j]);
		    delete cs_array[i]->level_array[j];
		}
	    }
	    cs_array[i]->level_array.clear();
	    for (j = 0; j < cs_array[i]->access_array.size(); j++)
	    {
		if (cs_array[i]->access_array[j] != NULL)
		{
		    cs_array[i]->i_Access.insert(*cs_array[i]->access_array[j]);
		    delete cs_array[i]->access_array[j];
		}
	    }
	    cs_array[i]->access_array.clear();
	    for (j = 0; j < cs_array[i]->akick_array.size(); j++)
	    {
		if (cs_array[i]->akick_array[j] != NULL)
		{
		    cs_array[i]->i_Akick.insert(*cs_array[i]->akick_array[j]);
		    delete cs_array[i]->akick_array[j];
		}
	    }
	    cs_array[i]->akick_array.clear();
	    for (j = 0; j < cs_array[i]->greet_array.size(); j++)
	    {
		if (cs_array[i]->greet_array[j] != NULL)
		{
		    cs_array[i]->i_Greet.push_back(*cs_array[i]->greet_array[j]);
		    delete cs_array[i]->greet_array[j];
		}
	    }
	    cs_array[i]->greet_array.clear();
	    for (j = 0; j < cs_array[i]->message_array.size(); j++)
	    {
		if (cs_array[i]->message_array[j] != NULL)
		{
		    cs_array[i]->i_Message.push_back(*cs_array[i]->message_array[j]);
		    delete cs_array[i]->message_array[j];
		}
	    }
	    cs_array[i]->message_array.clear();
	    for (j = 0; j < cs_array[i]->ud_array.size(); j++)
	    {
		if (cs_array[i]->ud_array[j] != NULL)
		{
		    if (cs_array[i]->ud_array[j]->Contains("\n"))
			cs_array[i]->i_UserDef[cs_array[i]->ud_array[j]->Before("\n")] = cs_array[i]->ud_array[j]->After("\n");
		    delete cs_array[i]->ud_array[j];
		}
	    }
	    cs_array[i]->ud_array.clear();
	    if (!cs_array[i]->Name().empty())
		AddStored(cs_array[i]);
	    // delete cs_array[i];
	}
    }
    cs_array.clear();

    mstring locked = Magick::instance().chanserv.LCK_MLock();

    ChanServ::stored_t::iterator iter;
    RLOCK((lck_ChanServ, lck_stored));
    for (iter = StoredBegin(); iter != StoredEnd(); iter++)
    {
	map_entry < Chan_Stored_t > cstored(iter->second);
	mstring lname = cstored->Name().LowerCase();
	{
	    MLOCK((lck_ChanServ, lck_stored, lname, "Level"));
	    for_each(cstored->Level_begin(), cstored->Level_end(), mem_fun_ref_void(&entlist_val_t<long>::PostLoad));
	}
	{
	    MLOCK((lck_ChanServ, lck_stored, lname, "Access"));
	    for_each(cstored->Access_begin(), cstored->Access_end(), mem_fun_ref_void(&entlist_val_t<long>::PostLoad));
	}
	{
	    MLOCK((lck_ChanServ, lck_stored, lname, "Akick"));
	    for_each(cstored->Akick_begin(), cstored->Akick_end(), mem_fun_ref_void(&entlist_val_t<mstring>::PostLoad));
	}
	{
	    MLOCK((lck_ChanServ, lck_stored, lname, "Greet"));
	    for_each(cstored->Greet_begin(), cstored->Greet_end(), mem_fun_ref_void(&entlist_t::PostLoad));
	}
	{
	    MLOCK((lck_ChanServ, lck_stored, lname, "Message"));
	    for_each(cstored->Message_begin(), cstored->Message_end(), mem_fun_ref_void(&entlist_t::PostLoad));
	}

	// Now, we're fully loaded, do sanity checks from CFG ...
	bool add = true;

	for (i = 0; i < locked.size(); i++)
	{
	    if (locked[i] == '+')
	    {
		add = true;
	    }
	    else if (locked[i] == '-')
	    {
		add = false;
	    }
	    else
	    {
		if (add)
		{
		    if (!Magick::instance().server.proto.ChanModeArg().Contains(locked[i]))
		    {
			if (!cstored->setting.Mlock_On.Contains(locked[i]))
			    cstored->setting.Mlock_On += locked[i];
			if (cstored->setting.Mlock_Off.Contains(locked[i]))
			    cstored->setting.Mlock_Off.Remove(locked[i]);
		    }
		}
		else
		{
		    if (locked[i] == 'k')
			cstored->setting.Mlock_Key.erase();
		    else if (locked[i] == 'l')
			cstored->setting.Mlock_Limit = 0;

		    if (locked[i] == 'k' || locked[i] == 'l' ||
			!Magick::instance().server.proto.ChanModeArg().Contains(locked[i]))
		    {
			if (!cstored->setting.Mlock_Off.Contains(locked[i]))
			    cstored->setting.Mlock_Off += locked[i];
			if (cstored->setting.Mlock_On.Contains(locked[i]))
			    cstored->setting.Mlock_On.Remove(locked[i]);
		    }
		}
	    }
	}
    }
    ETCB();
}
