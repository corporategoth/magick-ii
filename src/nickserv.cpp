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
#define RCSID(x,y) const char *rcsid_nickserv_cpp_ ## x () { return y; }
RCSID(nickserv_cpp, "@(#)$Id$");

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

NickServ::NickServ()
{
    BTCB();
    NFT("NickServ::NickServ");
    messages = true;
    ETCB();
}

mstring NickServ::findnextnick(const mstring & in)
{
    BTCB();
    FT("NickServ::findnextnick", (in));

    // Amount of nicknames it will try, only
    // for the guest????? method.
    unsigned int i, attempts = 64;

    CP(("Renaming nickname %s", in.c_str()));
    if (Magick::instance().nickserv.Append_Rename())
    {
	for (i = 0; i < Magick::instance().nickserv.Suffixes().length(); i++)
	{
	    mstring retval(in);

	    while (retval.length() < Magick::instance().server.proto.NickLen())
	    {
		retval += Magick::instance().nickserv.Suffixes() [i];
		COM(("Attempting to use %s", retval.c_str()));
		if (!Magick::instance().nickserv.IsLiveAll(retval) && !Magick::instance().nickserv.IsStored(retval))
		{
		    RET(retval);
		}
	    }
	}
    }
    else
    {
	srand(time(NULL));
	for (i = 0; i < attempts; i++)
	{
	    mstring retval;

	    retval.Format("%s%05d", Magick::instance().nickserv.Suffixes().c_str(), rand() % 99999);
	    COM(("Attempting to use %s", retval.c_str()));
	    if (!Magick::instance().nickserv.IsLiveAll(retval) && !Magick::instance().nickserv.IsStored(retval))
	    {
		RET(retval);
	    }
	}
    }
    RET("");
    ETCB();
}

void NickServ::AddCommands()
{
    BTCB();
    NFT("NickServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* CUR*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_Current);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* ADD", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* DEL*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* ERA*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* LIST", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* VIEW", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* ADD", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_ignore_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* DEL*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_ignore_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* ERA*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_ignore_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* LIST", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_ignore_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* VIEW", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_ignore_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PASS*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Password);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* E*MAIL*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Email);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* URL", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* WWW*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* WEB*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* ICQ*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_ICQ);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* AIM*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_AIM);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* MSN*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_MSN);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* MIM*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_MSN);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* YAHOO*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Yahoo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* YIM*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Yahoo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* DESC*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Description);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* COMM*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_set_Comment);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PIC*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Picture);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PROT*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Protect);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* SEC*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* NOEX*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_set_NoExpire);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* NOMEMO", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_NoMemo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PRIVM*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *MSG", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PRIV*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* LANG*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Language);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK PROT*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_Protect);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK SEC*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK NOMEMO", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_NoMemo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK PRIVM*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK *MSG", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK PRIV*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK LANG*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_Language);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK H*LP", Magick::instance().commserv.SOP_Name(),
						 do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK PROT*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_Protect);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK SEC*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK NOMEMO", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_NoMemo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK PRIVM*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK *MSG", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK PRIV*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK LANG*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_Language);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK H*LP", Magick::instance().commserv.SOP_Name(),
						 do_2param);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Help);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "REG*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Register);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "DROP*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_Drop);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LINK*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Link);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "U*LIN*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_UnLink);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "HOST", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_Host);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "*SLAV*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_Slaves);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ID*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Identify);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SID*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Identify);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "INF*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Info);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GHOST*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Ghost);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "REC*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Recover);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LIST*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SEND*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Send);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SUSP*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_Suspend);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNSUS*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_UnSuspend);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FORB*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_Forbid);
#ifdef GETPASS
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GETPASS*", Magick::instance().commserv.SADMIN_Name(),
						 NickServ::do_Getpass);
#endif
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SETPASS*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_Setpass);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LIVE*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_Live);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET*", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC*", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN*", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    ETCB();
}

void NickServ::RemCommands()
{
    BTCB();
    NFT("NickServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* CUR*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PASS*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* E*MAIL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* URL", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* WWW*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* WEB*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* ICQ*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* AIM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* MSN*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* MIM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* YAHOO*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* YIM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* DESC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* COMM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PIC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PROT*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* SEC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* NOEX*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* NOMEMO", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PRIVM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *MSG", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PRIV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* LANG*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK PROT*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK NOMEMO", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK PRIVM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK *MSG", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK LANG*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK H*LP", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK PROT*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK NOMEMO", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK PRIVM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK *MSG", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK LANG*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK H*LP", Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "REG*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "DROP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LINK*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "U*LIN*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "HOST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "*SLAV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ID*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "INF*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GHOST*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "REC*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LIST*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SEND*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SUSP*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNSUS*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FORB*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GET*PASS*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LIVE*", Magick::instance().commserv.SOP_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN*", Magick::instance().commserv.REGD_Name());
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::AddStored(Nick_Stored_t * in) throw (E_NickServ_Stored)
#else
void NickServ::AddStored(Nick_Stored_t * in)
#endif
{
    BTCB();
    FT("NickServ::AddStored", ("(Nick_Stored_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Stored(E_NickServ_Stored::W_Add, E_NickServ_Stored::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Stored", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Stored(E_NickServ_Stored::W_Add, E_NickServ_Stored::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Stored", "Add", "Blank"));
	return;
#endif
    }

    if (in->doDelete())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Stored(E_NickServ_Stored::W_Add, E_NickServ_Stored::T_NotFound));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Stored", "Add", "NotFound"));
	return;
#endif
    }

    WLOCK((lck_NickServ, lck_stored));
    map_entry < Nick_Stored_t > old_entry(stored, in->Name().LowerCase());
    if (old_entry.entry() != NULL)
    {
	old_entry->setDelete();
	stored.erase(in->Name().LowerCase());
    }
    stored[in->Name().LowerCase()] = in;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
map_entry < Nick_Stored_t > NickServ::GetStored(const mstring & in) const throw (E_NickServ_Stored)
#else
map_entry < Nick_Stored_t > NickServ::GetStored(const mstring & in) const
#endif
{
    BTCB();
    FT("NickServ::GetStored", (in));

    RLOCK((lck_NickServ, lck_stored));
    NickServ::stored_t::const_iterator iter = stored.find(in.LowerCase());
    if (iter == stored.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Stored(E_NickServ_Stored::W_Get, E_NickServ_Stored::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Get", "NotFound", in));
	NRET(Nick_Stored_t &, GLOB_Nick_Stored_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Stored(E_NickServ_Stored::W_Get, E_NickServ_Stored::T_Invalid, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Get", "Invalid", in));
	NRET(Nick_Stored_t &, GLOB_Nick_Stored_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Stored(E_NickServ_Stored::W_Get, E_NickServ_Stored::T_Blank, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Get", "Blank", in));
	NRET(Nick_Stored_t &, GLOB_Nick_Stored_t);
#endif
    }

    NRET(map_entry < Nick_Stored_t >, map_entry < Nick_Stored_t > (iter->second));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::RemStored(const mstring & in) throw (E_NickServ_Stored)
#else
void NickServ::RemStored(const mstring & in)
#endif
{
    BTCB();
    FT("NickServ::RemStored", (in));

    WLOCK((lck_NickServ, lck_stored));
    NickServ::stored_t::iterator iter = stored.find(in.LowerCase());
    if (iter == stored.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Stored(E_NickServ_Stored::W_Rem, E_NickServ_Stored::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Rem", "NotFound", in));
	return;
#endif
    }

    if (iter->second != NULL)
    {
	map_entry < Nick_Stored_t > entry(iter->second);
	entry->setDelete();
    }
    stored.erase(iter);
    ETCB();
}

bool NickServ::IsStored(const mstring & in) const
{
    BTCB();
    FT("NickServ::IsStored", (in));
    RLOCK((lck_NickServ, lck_stored));
    bool retval = (stored.find(in.LowerCase()) != stored.end());

    RET(retval);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::AddLive(Nick_Live_t * in) throw (E_NickServ_Live)
#else
void NickServ::AddLive(Nick_Live_t * in)
#endif
{
    BTCB();
    FT("NickServ::AddLive", ("(Nick_Live_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Live(E_NickServ_Live::W_Add, E_NickServ_Live::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Live", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Live(E_NickServ_Live::W_Add, E_NickServ_Live::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Live", "Add", "Blank"));
	return;
#endif
    }

    if (in->doDelete())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Live(E_NickServ_Live::W_Add, E_NickServ_Live::T_NotFound));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Live", "Add", "NotFound"));
	return;
#endif
    }

    WLOCK((lck_NickServ, lck_live));
    map_entry < Nick_Live_t > old_entry(live, in->Name().LowerCase());
    if (old_entry.entry() != NULL)
    {
	old_entry->setDelete();
	live.erase(in->Name().LowerCase());
    }
    live[in->Name().LowerCase()] = in;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
map_entry < Nick_Live_t > NickServ::GetLive(const mstring & in) const throw (E_NickServ_Live)
#else
map_entry < Nick_Live_t > NickServ::GetLive(const mstring & in) const
#endif
{
    BTCB();
    FT("NickServ::GetLive", (in));

    RLOCK((lck_NickServ, lck_live));
    NickServ::live_t::const_iterator iter = live.find(in.LowerCase());
    if (iter == live.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Live(E_NickServ_Live::W_Get, E_NickServ_Live::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Get", "NotFound", in));
	RET(Nick_Live_t &, GLOB_Nick_Live_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Live(E_NickServ_Live::W_Get, E_NickServ_Live::T_Invalid, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Get", "Invalid", in));
	RET(Nick_Live_t &, GLOB_Nick_Live_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Live(E_NickServ_Live::W_Get, E_NickServ_Live::T_Blank, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Get", "Blank", in));
	RET(Nick_Live_t &, GLOB_Nick_Live_t);
#endif
    }

    NRET(map_entry < Nick_Live_t >, map_entry < Nick_Live_t > (iter->second));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::RemLive(const mstring & in) throw (E_NickServ_Live)
#else
void NickServ::RemLive(const mstring & in)
#endif
{
    BTCB();
    FT("NickServ::RemLive", (in));

    WLOCK((lck_NickServ, lck_live));
    NickServ::live_t::iterator iter = live.find(in.LowerCase());
    if (iter == live.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Live(E_NickServ_Live::W_Rem, E_NickServ_Live::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Rem", "NotFound", in));
	return;
#endif
    }

    if (iter->second != NULL)
    {
	map_entry < Nick_Live_t > entry(iter->second);
	entry->setDelete();
    }
    live.erase(iter);
    ETCB();
}

bool NickServ::IsLive(const mstring & in) const
{
    BTCB();
    FT("NickServ::IsLive", (in));
    RLOCK((lck_NickServ, lck_live));
    map_entry < Nick_Live_t > ent(live, in.LowerCase());
    if (ent.entry() != NULL && ent->Squit().empty())
	RET(true);
    RET(false);
    ETCB();
}

bool NickServ::IsLiveAll(const mstring & in) const
{
    BTCB();
    FT("NickServ::IsLiveAll", (in));
    RLOCK((lck_NickServ, lck_live));
    bool retval = (live.find(in.LowerCase()) != live.end());

    RET(retval);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::AddRecovered(const mstring & name, const mDateTime & in) throw (E_NickServ_Recovered)
#else
void NickServ::AddRecovered(const mstring & name, const mDateTime & in)
#endif
{
    BTCB();
    FT("NickServ::AddRecovered", (name, in));

    if (name.empty() || in == mDateTime(0.0))
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Recovered(E_NickServ_Recovered::W_Add, E_NickServ_Recovered::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Recovered", "Add", "Blank"));
	return;
#endif
    }

    WLOCK((lck_NickServ, "recovered"));
    recovered[name.LowerCase()] = in;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
const mDateTime &NickServ::GetRecovered(const mstring & in) const throw (E_NickServ_Recovered)
#else
const mDateTime &NickServ::GetRecovered(const mstring & in) const
#endif
{
    BTCB();
    FT("NickServ::GetRecovered", (in));

    RLOCK((lck_NickServ, "recovered"));
    map < mstring, mDateTime >::const_iterator iter = recovered.find(in.LowerCase());
    if (iter == recovered.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Recovered(E_NickServ_Recovered::W_Get, E_NickServ_Recovered::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Recovered", "Get", "NotFound", in));
	RET(mDateTime &, GLOB_mDateTime);
#endif
    }
    if (iter->second == mDateTime(0.0))
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Recovered(E_NickServ_Recovered::W_Get, E_NickServ_Recovered::T_Blank, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Recovered", "Get", "Blank", in));
	RET(mDateTime &, GLOB_mDateTime);
#endif
    }

    NRET(mDateTime &, iter->second);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::RemRecovered(const mstring & in) throw (E_NickServ_Recovered)
#else
void NickServ::RemRecovered(const mstring & in)
#endif
{
    BTCB();
    FT("NickServ::RemRecovered", (in));

    WLOCK((lck_NickServ, "recovered"));
    map < mstring, mDateTime >::iterator iter = recovered.find(in.LowerCase());
    if (iter == recovered.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_NickServ_Recovered(E_NickServ_Recovered::W_Rem, E_NickServ_Recovered::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Recovered", "Rem", "NotFound", in));
	return;
#endif
    }
    recovered.erase(iter);
    ETCB();
}

bool NickServ::IsRecovered(const mstring & in) const
{
    BTCB();
    FT("NickServ::IsRecovered", (in));
    RLOCK((lck_NickServ, "recovered"));
    bool retval = (recovered.find(in.LowerCase()) != recovered.end());

    RET(retval);
    ETCB();
}

void NickServ::execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    mThread::ReAttach(tt_NickServ);
    FT("NickServ::execute", (source, msgtype, params));
    //okay this is the main nickserv command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message
    mstring mynick(Magick::instance().getLname(params.ExtractWord(1, ": ")));
    mstring message(params.After(":"));
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

void NickServ::do_Help(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Help", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    mstring HelpTopic = Magick::instance().nickserv.GetInternalName();

    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector < mstring > help = Magick::instance().getHelp(source, HelpTopic.UpperCase());

    unsigned int i;

    for (i = 0; i < help.size(); i++)
	::send(mynick, source, help[i]);
    ETCB();
}

void NickServ::do_Register(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Register", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring password = params.ExtractWord(2, " ");
    mstring email;

    if (params.WordCount(" ") > 2)
	email = params.ExtractWord(3, " ");

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (Magick::instance().nickserv.IsStored(source))
    {
	NSEND(mynick, source, "NS_YOU_STATUS/ISSTORED");
    }
    else if (!nlive->HasMode("o") && nlive->LastNickReg().SecondsSince() < Magick::instance().nickserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET",
	     (message, ToHumanTime(Magick::instance().nickserv.Delay() - nlive->LastNickReg().SecondsSince(), source)));
    }
    else if (password.length() < 5 || password.IsSameAs(source, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
    }
    else
    {
	if (!email.empty())
	{
	    if (!email.Contains("@"))
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
		return;
	    }
	    else if (email.WordCount("@") != 2)
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE",
		     (Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
		return;
	    }
	}

	nlive->SetLastNickReg();
	map_entry < Nick_Stored_t > nstored(new Nick_Stored_t(source, password));
	nstored->AccessAdd(nlive->Mask(Nick_Live_t::U_H).After("!"));
	if (!email.empty())
	    nstored->Email(email);
	Magick::instance().nickserv.AddStored(nstored);
	nlive->Identify(password);
	Magick::instance().nickserv.stats.i_Register++;
	SEND(mynick, source, "NS_YOU_COMMAND/REGISTERED", (nlive->Mask(Nick_Live_t::U_H).After("!")));
	LOG(LM_INFO, "NICKSERV/REGISTER", (nlive->Mask(Nick_Live_t::N_U_P_H), source));
    }
    ETCB();
}

void NickServ::do_Drop(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Drop", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    mstring target;

    if (params.WordCount(" ") < 2)
    {
	target = source;
    }
    else if (!Magick::instance().nickserv.IsStored(params.ExtractWord(2, " ")))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (params.ExtractWord(2, " ")));
	return;
    }
    else
	target = Magick::instance().getSname(params.ExtractWord(2, " "));

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);

    // Our sibling, and we're not identified ...
    if (!nlive->IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (message, mynick));
	return;
    }

    if (!target.IsSameAs(source, true) && !nstored->IsSibling(source))
    {
	map_entry < Committee_t > comm;
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()))
	    comm = Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name());
	if (comm.entry() != NULL && comm->IsOn(source))
	{
	    // NOT our sibling, and its either a sop or oper ...
	    if ((comm->IsIn(target)) ||
		(Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(target)))
	    {
		SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (message));
		return;
	    }
	}
	else
	{
	    // Not ours at all, and we're not a sop!
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTYOURS", (target));
	    return;
	}
    }

    Magick::instance().nickserv.stats.i_Drop++;
    unsigned long dropped = nstored->Drop();

    Magick::instance().nickserv.RemStored(target);
    if (!Magick::instance().nickserv.IsStored(source))
	nlive->UnIdentify();
    if (source.IsSameAs(target, true))
	NSEND(mynick, source, "NS_YOU_COMMAND/DROPPED");
    else
	SEND(mynick, source, "NS_OTH_COMMAND/DROPPED", (target));

    LOG(LM_INFO, "NICKSERV/DROP", (nlive->Mask(Nick_Live_t::N_U_P_H), target, dropped - 1));

    ETCB();
}

void NickServ::do_Link(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Link", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring hostnick = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(hostnick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (hostnick));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(hostnick);
    if (!nstored->Host().empty())
	hostnick = nstored->Host();
    else
	hostnick = nstored->Name();

    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (hostnick));
	return;
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->HasMode("o") && nlive->LastNickReg().SecondsSince() < Magick::instance().nickserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET",
	     (message, ToHumanTime(Magick::instance().nickserv.Delay() - nlive->LastNickReg().SecondsSince(), source)));
	return;
    }

    mDateTime regtime = mDateTime::CurrentDateTime();

    if (Magick::instance().nickserv.IsStored(source))
    {
	map_entry < Nick_Stored_t > orig = Magick::instance().nickserv.GetStored(source);
	mstring myhost = orig->Host();

	if (myhost.empty())
	{
	    if (source.IsSameAs(hostnick))
	    {
		SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (message));
		return;
	    }
	}
	else
	{
	    if (myhost.IsSameAs(hostnick))
	    {
		SEND(mynick, source, "ERR_SITUATION/NOTONSIBLING", (message));
		return;
	    }
	}

	if (!nlive->IsIdentified())
	{
	    SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (message, mynick));
	    return;
	}
	regtime = orig->RegTime();
    }

    if (nstored->Slave(source, password, regtime))
    {
	nlive->SetLastNickReg();
	Magick::instance().nickserv.stats.i_Link++;
	nlive->Identify(password);
	SEND(mynick, source, "NS_YOU_COMMAND/LINKED", (hostnick));
	LOG(LM_INFO, "NICKSERV/LINK", (nlive->Mask(Nick_Live_t::N_U_P_H), source, hostnick));
    }
    else
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
    }
    ETCB();
}

void NickServ::do_UnLink(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_UnLink", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (message, mynick));
	return;
    }

    if (params.WordCount(" ") > 1)
    {
	mstring target = params.ExtractWord(2, " ");

	if (!Magick::instance().nickserv.IsStored(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	    return;
	}

	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
	target = nstored->Name();
	if (nstored->Host().empty())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISHOST", (target));
	    return;
	}

	if (!(nstored->IsSibling(source) || nstored->Host().IsSameAs(source, true)))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTYOURS", (target));
	    return;
	}

	if (nstored->Unlink())
	{
	    Magick::instance().nickserv.stats.i_Unlink++;
	    SEND(mynick, source, "NS_OTH_COMMAND/UNLINKED", (target));
	    LOG(LM_INFO, "NICKSERV/UNLINK", (nlive->Mask(Nick_Live_t::N_U_P_H), source, target));
	}
	else
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTLINKED", (target));
    }
    else
    {
	if (!Magick::instance().nickserv.IsStored(source))
	{
	    NSEND(mynick, source, "NS_YOU_STATUS/ISNOTSTORED");
	    return;
	}

	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
	if (nstored->Host().empty())
	{
	    NSEND(mynick, source, "NS_YOU_STATUS/ISHOST");
	    return;
	}

	mstring target = nstored->Host();

	if (nstored->Unlink())
	{
	    Magick::instance().nickserv.stats.i_Unlink++;
	    NSEND(mynick, source, "NS_YOU_COMMAND/UNLINKED");
	    LOG(LM_INFO, "NICKSERV/UNLINK", (nlive->Mask(Nick_Live_t::N_U_P_H), source, target));
	}
	else
	    NSEND(mynick, source, "NS_YOU_STATUS/ISNOTLINKED");
    }
    ETCB();
}

void NickServ::do_Host(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Host", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    mstring newhost;

    if (params.WordCount(" ") > 1)
	newhost = params.ExtractWord(2, " ");

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (message, mynick));
	return;
    }

    if (!newhost.empty())
    {
	if (!Magick::instance().nickserv.IsStored(newhost))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (newhost));
	    return;
	}

	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(newhost);
	newhost = nstored->Name();
	if (nstored->Host().empty())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISHOST", (newhost));
	    return;
	}

	if (source.IsSameAs(newhost, true))
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (message));
	    return;
	}

	if (!nstored->IsSibling(source) && !nstored->Host().IsSameAs(source, true))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTLINKED", (newhost));
	    return;
	}

	mstring oldhost = nstored->Host();

	Magick::instance().nickserv.stats.i_Host++;
	nstored->MakeHost();
	SEND(mynick, source, "NS_OTH_COMMAND/NEWHOST", (newhost));
	LOG(LM_INFO, "NICKSERV/HOST", (nlive->Mask(Nick_Live_t::N_U_P_H), oldhost, newhost));
    }
    else
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
	mstring oldhost = nstored->Host();

	Magick::instance().nickserv.stats.i_Host++;
	nstored->MakeHost();
	NSEND(mynick, source, "NS_YOU_COMMAND/NEWHOST");
	LOG(LM_INFO, "NICKSERV/HOST", (nlive->Mask(Nick_Live_t::N_U_P_H), oldhost, source));
    }
    ETCB();
}

void NickServ::do_Slaves(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Slaves", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    mstring target;

    map_entry < Nick_Stored_t > nick;
    if (params.WordCount(" ") < 2)
    {
	nick = Magick::instance().nickserv.GetStored(source);
	target = nick->Name();
    }
    else
    {
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	    Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))
	{
	    target = params.ExtractWord(2, " ");
	    if (!Magick::instance().nickserv.IsStored(target))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
		return;
	    }

	    nick = Magick::instance().nickserv.GetStored(target);
	    target = nick->Name();
	    if (nick->Forbidden())
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
		return;
	    }
	}
	else
	{
	    nick = Magick::instance().nickserv.GetStored(source);
	    target = nick->Name();
	}
    }

    if (!nick->Host().empty())
	target = nick->Host();

    mstring output;

    output << IRC_Bold << target << IRC_Off << " (" << nick->Siblings() << "):";

    for (unsigned int i = 0; i < nick->Siblings(); i++)
    {
	if (nick->Sibling(i).length() + output.length() > 510)
	{
	    ::send(mynick, source, output);
	    output.erase();
	    output << IRC_Bold << target << IRC_Off << " (" << nick->Siblings() << "):";
	}
	output << " " << nick->Sibling(i);
    }
    ::send(mynick, source, output);
    ETCB();
}

void NickServ::do_Identify(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Identify", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring password = params.ExtractWord(2, " ");

    if (!Magick::instance().nickserv.IsStored(source))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (source));
	return;
    }

    if (Magick::instance().nickserv.GetStored(source)->Suspended())
    {
	NSEND(mynick, source, "NS_YOU_STATUS/ISSUSPENDED");
	return;
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    bool wasident = nlive->IsIdentified();
    mstring output = nlive->Identify(password);

    if (!wasident && nlive->IsIdentified())
    {
	Magick::instance().nickserv.stats.i_Identify++;
	LOG(LM_INFO, "NICKSERV/IDENTIFY", (nlive->Mask(Nick_Live_t::N_U_P_H), source));
    }
    if (!output.empty())
	::send(mynick, source, output);
    ETCB();
}

void NickServ::do_Info(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Info", (mynick, source, params));

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

    unsigned int i;
    mstring target = params.ExtractWord(2, " ");
    mstring output;

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(target);
    map_entry < Nick_Stored_t > host;
    bool isoper = (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		   Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsOn(source));
    bool issop = (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source));

    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nick->Name()));
	return;
    }

    SEND(mynick, source, "NS_INFO/REALNAME", (nick->Name(), nick->LastRealName()));
    if (!nick->Host().empty())
    {
	host = Magick::instance().nickserv.GetStored(nick->Host());
	SEND(mynick, source, "NS_INFO/HOST", (host->Name()));
    }

    output.erase();
    if (nick->NoExpire() && isoper)
	output << " (" << Magick::instance().getMessage(source, "NS_INFO/NOEXPIRE") << ")";
    SEND(mynick, source, "NS_INFO/REGISTERED", (nick->RegTime().Ago(), output));

    if (!nick->IsOnline())
    {
	output.erase();
	bool isonline = false;

	if (host.entry() != NULL && host->IsOnline())
	    output = Magick::instance().nickserv.GetLive(nick->Host())->Name() + " ";
	for (i = 0; i < nick->Siblings(); i++)
	{
	    if (Magick::instance().nickserv.IsStored(nick->Sibling(i)) &&
		Magick::instance().nickserv.GetStored(nick->Sibling(i))->IsOnline())
	    {
		if (output.length() + nick->Sibling(i).length() > Magick::instance().server.proto.MaxLine())
		{
		    SEND(mynick, source, "NS_INFO/ONLINEAS", (output));
		    output.erase();
		    isonline = true;
		}
		output += Magick::instance().nickserv.GetLive(nick->Sibling(i))->Name() + " ";
	    }
	}
	if (!output.empty())
	{
	    SEND(mynick, source, "NS_INFO/ONLINEAS", (output));
	    isonline = true;
	}
	if (!isonline)
	{
	    if (isoper || !nick->Private())
		SEND(mynick, source, "NS_INFO/LASTALLMASK", (nick->LastAllMask()));
	    SEND(mynick, source, "NS_INFO/LASTALLSEEN", (nick->LastAllSeenTime().Ago()));
	    if (!nick->LastAllMask().IsSameAs(nick->Name() + "!" + nick->LastMask(), true) && (isoper || !nick->Private()))
		SEND(mynick, source, "NS_INFO/LASTMASK", (nick->LastMask()));
	    if (nick->LastAllSeenTime() != nick->LastSeenTime())
		SEND(mynick, source, "NS_INFO/LASTSEEN", (nick->LastSeenTime().Ago()));
	    SEND(mynick, source, "NS_INFO/QUITMSG", (nick->LastQuit()));
	}
    }
    if (nick->Suspended())
    {
	SEND(mynick, source, "NS_INFO/SUSPEND", (nick->Suspend_Time().Ago(), nick->Suspend_By()));
	SEND(mynick, source, "NS_INFO/SUSPENDFOR", (nick->Comment()));
    }
    else
    {
	if (!nick->Email().empty())
	    SEND(mynick, source, "NS_INFO/EMAIL", (nick->Email()));
	if (!nick->URL().empty())
	    SEND(mynick, source, "NS_INFO/URL", (nick->URL()));
	if (!nick->ICQ().empty())
	    SEND(mynick, source, "NS_INFO/ICQ", (nick->ICQ()));
	if (!nick->AIM().empty())
	    SEND(mynick, source, "NS_INFO/AIM", (nick->AIM()));
	if (!nick->MSN().empty())
	    SEND(mynick, source, "NS_INFO/MSN", (nick->MSN()));
	if (!nick->Yahoo().empty())
	    SEND(mynick, source, "NS_INFO/YAHOO", (nick->Yahoo()));
	if (!nick->Description().empty())
	    SEND(mynick, source, "NS_INFO/DESCRIPTION", (nick->Description()));
	if (!nick->Comment().empty() && (isoper || issop))
	    SEND(mynick, source, "NS_INFO/COMMENT", (nick->Comment()));
    }

    output.erase();
    CommServ::list_t::iterator iter;
    {
	RLOCK((lck_CommServ, lck_list));
	for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
	{
	    map_entry < Committee_t > comm(iter->second);

	    // IF we're in the committee
	    if (!comm->IsIn(target))
		continue;

	    // AND committee is not ALL or REGD
	    if (iter->first == Magick::instance().commserv.ALL_Name() &&
		iter->first == Magick::instance().commserv.REGD_Name())
		continue;

	    // AND if it has a headcom, we're not in it
	    if (!comm->HeadCom().empty() && Magick::instance().commserv.IsList(comm->HeadCom()) &&
		Magick::instance().commserv.GetList(comm->HeadCom())->IsIn(nick->Name()))
		continue;

	    // AND the committee isnt private or the requesting user is in OPER
	    if (isoper || !comm->Private())
	    {
		if (output.length() + comm->Name().length() > Magick::instance().server.proto.MaxLine())
		{
		    SEND(mynick, source, "NS_INFO/COMMITTEES", (output));
		    output.erase();
		}
		if (!output.empty())
		    output << ", ";
		if (comm->IsHead(target) && !comm->Head().empty())
		    output << IRC_Bold;
		output << comm->Name();
		if (comm->IsHead(target) && !comm->Head().empty())
		    output << IRC_Off;
	    }
	}
    }
    if (!output.empty())
	SEND(mynick, source, "NS_INFO/COMMITTEES", (output));

    output.erase();
    bool firstoption = true;

    if (nick->Protect())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick->L_Protect())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/PROTECT");
	if (nick->L_Protect())
	    output << IRC_Off;
    }

    if (nick->Secure())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick->L_Secure())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/SECURE");
	if (nick->L_Secure())
	    output << IRC_Off;
    }

    if (nick->NoMemo())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick->L_NoMemo())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/NOMEMO");
	if (nick->L_NoMemo())
	    output << IRC_Off;
    }

    if (nick->Private())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick->L_Private())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/PRIVATE");
	if (nick->L_Private())
	    output << IRC_Off;
    }

    if (!output.empty())
	SEND(mynick, source, "NS_INFO/OPTIONS", (output));
    if (nick->PicNum() && Magick::instance().filesys.Exists(FileMap::Picture, nick->PicNum()))
	SEND(mynick, source, "NS_INFO/HASPIC",
	     (ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::Picture, nick->PicNum())), mynick, nick->Name()));
    if (nick->IsOnline())
	SEND(mynick, source, "NS_INFO/ISONLINE", (Magick::instance().getLname(nick->Name())));
    {
	RLOCK((lck_Events));
	if (Magick::instance().servmsg.ShowSync() && Magick::instance().events != NULL)
	    SEND(mynick, source, "MISC/SYNC", (Magick::instance().events->SyncTime(source)));
    }
    ETCB();
}

void NickServ::do_Ghost(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Ghost", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nick = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (nick.IsSameAs(source, true))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (message));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nick));
	return;
    }

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (nick));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(nick);
    nick = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nick));
	return;
    }

    if (!nstored->CheckPass(pass))
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
	return;
    }

    Magick::instance().server.ANONKILL(mynick, nick,
				       source + " (" + Magick::instance().getMessage(nick, "MISC/KILL_GHOST") + ")");
    if (Magick::instance().nickserv.IsRecovered(nick))
	Magick::instance().nickserv.RemRecovered(nick);

    Magick::instance().nickserv.stats.i_Ghost++;
    SEND(mynick, source, "NS_OTH_COMMAND/RELEASED", (nick));
    LOG(LM_DEBUG, "NICKSERV/GHOST", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), nick));
    ETCB();
}

void NickServ::do_Recover(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Recover", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nick = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (nick.IsSameAs(source, true))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (message));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nick));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(nick);
    nick = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nick));
	return;
    }

    if (!nstored->CheckPass(pass))
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
	return;
    }

    if (Magick::instance().nickserv.IsLive(nick))
    {
	Magick::instance().server.KILL(mynick, nick,
				       source + " (" + Magick::instance().getMessage(nick, "MISC/KILL_RECOVER") + ")");
    }

    Magick::instance().server.NICK(nick, (Magick::instance().startup.Ownuser() ? nick.LowerCase() :
										 Magick::instance().startup.Services_User()),
					  Magick::instance().startup.Services_Host(), Magick::instance().startup.Server_Name(),
					  Magick::instance().nickserv.Enforcer_Name());

    Magick::instance().nickserv.AddRecovered(nick, mDateTime::CurrentDateTime());
    Magick::instance().nickserv.stats.i_Recover++;
    SEND(mynick, source, "NS_OTH_COMMAND/HELD", (nick));
    LOG(LM_DEBUG, "NICKSERV/RECOVER", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), nick));
    ETCB();
}

void NickServ::do_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_List", (mynick, source, params));

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

    SEND(mynick, source, "LIST/NICK_LIST", (mask));
    NickServ::stored_t::iterator iter;

    bool isoper = (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		   Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsOn(source));
    bool issop = (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source));

    {
	RLOCK((lck_NickServ, lck_stored));
	for (iter = Magick::instance().nickserv.StoredBegin(), i = 0, count = 0;
	     iter != Magick::instance().nickserv.StoredEnd(); iter++)
	{
	    map_entry < Nick_Stored_t > nstored(iter->second);
	    if (nstored->Name().Matches(mask, true))
	    {
		if (!nstored->Host().empty())
		    continue;

		if (i < listsize && (!nstored->Private() || isoper))
		{
		    if (issop)
		    {
			if (message.Contains("NOEXP") && !nstored->NoExpire())
			    continue;
			if (message.Contains("FORBID") && !nstored->Forbidden())
			    continue;
			if (message.Contains("SUSP") && !nstored->Suspended())
			    continue;
		    }

		    if (nstored->Forbidden())
		    {
			::send(mynick, source,
			       nstored->Name() + "  (" + Magick::instance().getMessage(source, "VALS/FORBIDDEN") + ")");
		    }
		    else
		    {
			::send(mynick, source, nstored->Name() + "  (" + nstored->LastAllMask() + ")");
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

void NickServ::do_Send(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Send", (mynick, source, params));

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

    mstring target = params.ExtractWord(2, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    unsigned long picnum = nstored->PicNum();

    if (!picnum)
    {
	SEND(mynick, source, "NS_OTH_STATUS/NOPIC", (target));
	return;
    }

    if (!Magick::instance().filesys.Exists(FileMap::Picture, picnum))
    {
	nstored->GotPic(0);
	SEND(mynick, source, "DCC/NOFILE", ("SEND"));
	return;
    }

    mstring filename = Magick::instance().filesys.GetName(FileMap::Picture, picnum);
    size_t filesize = Magick::instance().filesys.GetSize(FileMap::Picture, picnum);

    if (filename.empty() || filesize <= 0)
    {
	nstored->GotPic(0);
	SEND(mynick, source, "DCC/NOFILE", ("SEND"));
	return;
    }

    if (!
	(Magick::instance().files.TempDirSize() == 0 ||
	 mFile::DirUsage(Magick::instance().files.TempDir()) <= Magick::instance().files.TempDirSize()))
    {
	NSEND(mynick, source, "DCC/NOSPACE2");
	return;
    }

    {
	RLOCK(("DCC"));
	if (Magick::instance().dcc != NULL)
	{
	    Magick::instance().nickserv.stats.i_Send++;
	    unsigned short port = mSocket::FindAvailPort();

	    ::privmsg(mynick, source,
		      DccEngine::encode("DCC SEND",
					filename + " " + mstring(Magick::instance().LocalHost()) + " " + mstring(port) + " " +
					mstring(filesize)));
	    Magick::instance().dcc->Accept(port, mynick, source, FileMap::Picture, picnum);
	}
    }
    ETCB();
}

void NickServ::do_Suspend(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Suspend", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring reason = params.After(" ", 2);

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    if (nstored->Suspended())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISSUSPENDED", (target));
	return;
    }

    if (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(target))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONCOMMITTEE", (message, Magick::instance().commserv.OPER_Name()));
	return;
    }

    nstored->Suspend(source, reason);
    Magick::instance().nickserv.stats.i_Suspend++;
    SEND(mynick, source, "NS_OTH_COMMAND/SUSPENDED", (target));
    LOG(LM_NOTICE, "NICKSERV/SUSPEND",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, reason));
    ETCB();
}

void NickServ::do_UnSuspend(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_UnSuspend", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    if (!nstored->Suspended())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSUSPENDED", (target));
	return;
    }

    nstored->UnSuspend();
    Magick::instance().nickserv.stats.i_Unsuspend++;
    SEND(mynick, source, "NS_OTH_COMMAND/UNSUSPENDED", (target));
    LOG(LM_NOTICE, "NICKSERV/UNSUSPEND", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target));
    ETCB();
}

void NickServ::do_Forbid(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Forbid", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    if (Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISSTORED", (Magick::instance().getSname(target)));
	return;
    }

    if ((Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	 Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(target)) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	 Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsIn(target)))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (message));
	return;
    }

    map_entry < Nick_Stored_t > tmp(new Nick_Stored_t(target));
    Magick::instance().nickserv.AddStored(tmp);
    Magick::instance().nickserv.stats.i_Forbid++;
    SEND(mynick, source, "NS_OTH_COMMAND/FORBIDDEN", (target));
    LOG(LM_NOTICE, "NICKSERV/FORBID", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target));
    ETCB();
}

#ifdef GETPASS

void NickServ::do_Getpass(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Getpass", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    // If we are NOT a SADMIN, and target is a PRIV GROUP.
    if (!
	(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsIn(source)) &&
	(Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsIn(target) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsIn(target)) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->IsIn(target)) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(target))))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (message));
	return;
    }

    mstring password = nstored->Password();
    mstring host = nstored->Host();

    if (host.empty())
	host = nstored->Name();

    Magick::instance().nickserv.stats.i_Getpass++;
    SEND(mynick, source, "NS_OTH_COMMAND/GETPASS", (target, host, password));
    ANNOUNCE(mynick, "MISC/NICK_GETPASS", (source, target, host));
    LOG(LM_NOTICE, "NICKSERV/GETPASS",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, host));
    ETCB();
}

#endif /* GETPASS */

void NickServ::do_Setpass(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Setpass", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    // If we are NOT a SADMIN, and target is a PRIV GROUP.
    if (!
	(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsIn(source)) &&
	(Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsIn(target) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsIn(target)) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->IsIn(target)) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(target))))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (message));
	return;
    }

    mstring host = nstored->Host();

    if (host.empty())
	host = nstored->Name();
    nstored->Password(password);

    Magick::instance().nickserv.stats.i_Getpass++;
    SEND(mynick, source, "NS_OTH_COMMAND/SETPASS", (target, host, password));
    ANNOUNCE(mynick, "MISC/NICK_SETPASS", (source, target, host));
    LOG(LM_NOTICE, "NICKSERV/SETPASS",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, host));
    ETCB();
}

void NickServ::do_Live(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_Live", (mynick, source, params));

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
	mask = "*!*@*";
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

    if (!mask.Contains("@") && !mask.Contains("!"))
	mask += "!*@*";
    else if (!mask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "LIST/NICKNAME"), '@'));
	return;
    }
    else if (!mask.Contains("!"))
	mask.prepend("*!");

    SEND(mynick, source, "LIST/NICK_LIST", (mask));
    NickServ::live_t::iterator iter;

    {
	RLOCK((lck_NickServ, lck_live));
	for (iter = Magick::instance().nickserv.LiveBegin(), i = 0, count = 0; iter != Magick::instance().nickserv.LiveEnd();
	     iter++)
	{
	    map_entry < Nick_Live_t > nlive(iter->second);
	    if (nlive->Mask(Nick_Live_t::N_U_P_H).Matches(mask, true))
	    {
		if (i < listsize)
		{
		    if (message.Contains("OPER") && !nlive->HasMode("o"))
			continue;

		    ::send(mynick, source,
			   nlive->Mask(Nick_Live_t::N_U_P_H) + " (" +
			   ((!nlive->Server().empty()) ? nlive->Server() : Magick::instance().startup.Server_Name()) +
			   ((!nlive->Squit().empty()) ? " (SQUIT)" : "") + "): +" + nlive->Mode());
		    i++;
		}
		count++;
	    }
	}
    }
    SEND(mynick, source, "LIST/DISPLAYED", (i, count));
    ETCB();
}

void NickServ::do_access_Current(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_access_Current", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (Magick::instance().nickserv.GetStored(source)->AccessAdd(nlive->Mask(Nick_Live_t::U_H).After("!")))
    {
	Magick::instance().nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/ADD",
	     (nlive->Mask(Nick_Live_t::U_H).After("!"), Magick::instance().getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_ADD",
	    (nlive->Mask(Nick_Live_t::N_U_P_H), nlive->Mask(Nick_Live_t::U_H).After("!"), source));

    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS",
	     (nlive->Mask(Nick_Live_t::U_H).After("!"), Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
    ETCB();
}

void NickServ::do_access_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_access_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring hostmask = params.ExtractWord(3, " ");

    if (hostmask.IsSameAs("CURRENT", true))
    {
	do_access_Current(mynick, source, params);
	return;
    }

    if (hostmask.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/ACCESS"), '!'));
	return;
    }

    if (!hostmask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "LIST/ACCESS"), '@'));
	return;
    }

    if (Magick::instance().nickserv.GetStored(source)->AccessAdd(hostmask))
    {
	Magick::instance().nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/ADD", (hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_ADD",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), hostmask, source));
    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS", (hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
    ETCB();
}

void NickServ::do_access_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_access_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring hostmask = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (hostmask.IsNumber())
    {
	if (hostmask.Contains(".") || hostmask.Contains("-"))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(hostmask.c_str());

	if (num <= 0 || num > nstored->Access())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, nstored->Access()));
	    return;
	}

	hostmask = nstored->Access(num - 1);
    }

    if (hostmask.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/ACCESS"), '!'));
	return;
    }

    if (!hostmask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "LIST/ACCESS"), '@'));
	return;
    }

    unsigned int count = nstored->AccessDel(hostmask);

    if (count)
    {
	Magick::instance().nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/DEL_MATCH", (count, hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_DEL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), hostmask, source));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
    ETCB();
}

void NickServ::do_access_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_access_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    mstring target = Magick::instance().getSname(source);

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    map_entry < Nick_Stored_t > nstored;
    if (params.WordCount(" ") >= 3 && Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))
    {
	target = params.ExtractWord(3, " ");
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

    if (nstored->Access())
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/DISPLAY2", (target, Magick::instance().getMessage(source, "LIST/ACCESS")));
	else
	    SEND(mynick, source, "LIST/DISPLAY", (Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
    else
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/EMPTY2", (target, Magick::instance().getMessage(source, "LIST/ACCESS")));
	else
	    SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/ACCESS")));
	return;
    }

    unsigned int i;
    mstring retval;

    for (i = 0; i < nstored->Access(); i++)
    {
	retval.erase();
	retval << i + 1 << ". " << nstored->Access(i);
	::send(mynick, source, retval);
    }
    ETCB();
}

void NickServ::do_ignore_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_ignore_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    if (Magick::instance().nickserv.GetStored(source)->IgnoreAdd(target))
    {
	Magick::instance().nickserv.stats.i_Ignore++;
	SEND(mynick, source, "LIST/ADD", (target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	LOG(LM_DEBUG, "NICKSERV/IGNORE_ADD",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, source));
    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS", (target, Magick::instance().getMessage(source, "LIST/IGNORE")));
    }
    ETCB();
}

void NickServ::do_ignore_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_ignore_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (target.IsNumber())
    {
	if (target.Contains(".") || target.Contains("-"))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(target.c_str());

	if (num <= 0 || num > nstored->Ignore())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, nstored->Ignore()));
	    return;
	}

	target = nstored->Ignore(num - 1);
    }

    unsigned int count = nstored->IgnoreDel(target);

    if (count)
    {
	Magick::instance().nickserv.stats.i_Ignore++;
	SEND(mynick, source, "LIST/DEL_MATCH", (count, target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	LOG(LM_DEBUG, "NICKSERV/IGNORE_DEL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, source));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (target, Magick::instance().getMessage(source, "LIST/IGNORE")));
    }

    ETCB();
}

void NickServ::do_ignore_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_ignore_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    mstring target = Magick::instance().getSname(source);

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    map_entry < Nick_Stored_t > nstored;
    if (params.WordCount(" ") >= 3 && Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))
    {
	target = params.ExtractWord(3, " ");
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

    if (nstored->Ignore())
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/DISPLAY2", (target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	else
	    SEND(mynick, source, "LIST/DISPLAY", (Magick::instance().getMessage(source, "LIST/IGNORE")));
    }
    else
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/EMPTY2", (target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	else
	    SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/IGNORE")));
	return;
    }

    unsigned int i;
    mstring retval;

    for (i = 0; i < nstored->Ignore(); i++)
    {
	retval.erase();
	retval << i + 1 << ". " << nstored->Ignore(i);
	::send(mynick, source, retval);
    }
    ETCB();
}

void NickServ::do_set_Password(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_Password", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newpass = params.ExtractWord(3, " ");

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (message, mynick));
	return;
    }

#ifdef GETPASS
    mstring oldpass = Magick::instance().nickserv.GetStored(source).Password();

    if (newpass.IsSameAs(oldpass, true) ||
#else
    if (
#endif
	   newpass.IsSameAs(source, true) || newpass.length() < 5)
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
	return;
    }

    Magick::instance().nickserv.GetStored(source)->Password(newpass);
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/PASSWORD"), newpass));
    LOG(LM_INFO, "NICKSERV/SET_PASSWORD", (nlive->Mask(Nick_Live_t::N_U_P_H), source));
    ETCB();
}

void NickServ::do_set_Email(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_Email", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();
    else if (!newvalue.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
	return;
    }
    else if (newvalue.WordCount("@") != 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE", (Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
	return;
    }

    Magick::instance().nickserv.GetStored(source)->Email(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/EMAIL"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/EMAIL"), source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/EMAIL")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/EMAIL"), source));
    }
    ETCB();
}

void NickServ::do_set_URL(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_URL", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();

    if (newvalue.SubString(0, 6).IsSameAs("http://", true))
    {
	newvalue.erase(0, 6);
    }

    Magick::instance().nickserv.GetStored(source)->URL(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO",
	     (Magick::instance().getMessage(source, "NS_SET/URL"), "http://" + newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/URL"), source, "http://" + newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/URL")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/URL"), source));
    }
    ETCB();
}

void NickServ::do_set_ICQ(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_ICQ", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();
    else if (!newvalue.IsNumber() || newvalue.Contains("."))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }
    else if (newvalue.length() < 6 || newvalue.length() > 9)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (100000, 999999999));
	return;
    }

    Magick::instance().nickserv.GetStored(source)->ICQ(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/ICQ"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/ICQ"), source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/ICQ")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/ICQ"), source));
    }
    ETCB();
}

void NickServ::do_set_AIM(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_AIM", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();

    Magick::instance().nickserv.GetStored(source)->AIM(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/AIM"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/AIM"), source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/AIM")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/AIM"), source));
    }
    ETCB();
}

void NickServ::do_set_MSN(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_MSN", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();

    Magick::instance().nickserv.GetStored(source)->MSN(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/MSN"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/MSN"), source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/MSN")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/MSN"), source));
    }
    ETCB();
}

void NickServ::do_set_Yahoo(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_Yahoo", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();

    Magick::instance().nickserv.GetStored(source)->Yahoo(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/YAHOO"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/YAHOO"), source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/YAHOO")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/YAHOO"), source));
    }
    ETCB();
}

void NickServ::do_set_Description(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_Description", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.After(" ", 2);

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();

    Magick::instance().nickserv.GetStored(source)->Description(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/DESCRIPTION"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/DESCRIPTION"), source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/DESCRIPTION")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/DESCRIPTION"), source));
    }
    ETCB();
}

void NickServ::do_set_Comment(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_Comment", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(3, " ");
    mstring comment = params.After(" ", 3);

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    if (comment.IsSameAs("none", true))
	comment.erase();

    nstored->Comment(comment);
    Magick::instance().nickserv.stats.i_Set++;
    if (!comment.empty())
    {
	SEND(mynick, source, "NS_OTH_COMMAND/SET_TO",
	     (Magick::instance().getMessage(source, "NS_SET/COMMENT"), target, comment));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/COMMENT"), target, comment));
    }
    else
    {
	SEND(mynick, source, "NS_OTH_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/COMMENT"), target));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/COMMENT"), target));
    }
    ETCB();
}

void NickServ::do_set_Picture(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_Picture", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    if (Magick::instance().nickserv.PicExt().empty())
    {
	NSEND(mynick, source, "NS_YOU_STATUS/PICDISABLED");
	return;
    }

    Magick::instance().nickserv.stats.i_SetPicture++;
    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (params.WordCount(" ") > 2 && params.ExtractWord(3, " ").IsSameAs("none", true))
    {
	if (nstored->PicNum())
	    Magick::instance().filesys.EraseFile(FileMap::Picture, nstored->PicNum());
	nstored->GotPic(0u);
	NSEND(mynick, source, "NS_YOU_COMMAND/REMOVED");
	LOG(LM_DEBUG, "NICKSERV/PICTURE_DEL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), source));
    }
    else
    {
	if (nstored->PicNum())
	    Magick::instance().filesys.EraseFile(FileMap::Picture, nstored->PicNum());
	nstored->GotPic(0u);
	Magick::instance().nickserv.GetLive(source)->InFlight.Picture(mynick);
    }
    ETCB();
}

void NickServ::do_set_Protect(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_Protect", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_Protect())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/PROTECT")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->Protect(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/PROTECT"),
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PROTECT"), source,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_set_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_Secure", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_Secure())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/SECURE")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->Secure(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/SECURE"),
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/SECURE"), source,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_set_NoExpire(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_NoExpire", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(nickname);
    nickname = nstored->Name();

    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_NoExpire())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/NOEXPIRE"), nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_NoExpire())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->NoExpire(onoff.GetBool());
    Magick::instance().nickserv.stats.i_NoExpire++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/NOEXPIRE"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_NOTICE, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/NOEXPIRE"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_set_NoMemo(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_NoMemo", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_NoMemo())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/NOMEMO")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->NoMemo(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/NOMEMO"),
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/NOMEMO"), source,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_set_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_Private", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_Private())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/PRIVATE")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->Private(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/PRIVATE"),
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVATE"), source,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_set_PRIVMSG(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_PRIVMSG", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_PRIVMSG())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/PRIVMSG")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->PRIVMSG(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/PRIVMSG"),
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVMSG"), source,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_set_Language(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_set_Language", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

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

    mstring lang = params.ExtractWord(3, " ").UpperCase();

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_Language())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/LANGUAGE")));
	return;
    }

    if (lang.IsSameAs("default", true) || lang.IsSameAs("reset", true))
    {
	lang = Magick::instance().nickserv.DEF_Language().UpperCase();
    }
    else
    {
	// check for valid language ...
	if (!mFile::Exists(Magick::instance().files.Langdir() + DirSlash + lang.LowerCase() + ".lng"))
	{
	    SEND(mynick, source, "OS_STATUS/NOLANG", (lang));
	    return;
	}
    }

    nstored->Language(lang);
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/LANGUAGE"),
	  mstring(lang + " (" + Magick::instance().getMessage(source, "ERR_SYNTAX/TRANSLATED") + ")")));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/LANGUAGE"), source, lang));
    ETCB();
}

void NickServ::do_lock_Protect(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_lock_Protect", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_Protect())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PROTECT")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Protect(false);
    nick->Protect(onoff.GetBool());
    nick->L_Protect(true);

    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/PROTECT"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PROTECT"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_lock_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_lock_Secure", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_Secure())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/SECURE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Secure(false);
    nick->Secure(onoff.GetBool());
    nick->L_Secure(true);
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/SECURE"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/SECURE"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_lock_NoMemo(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_lock_NoMemo", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/NOMEMO")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_NoMemo(false);
    nick->NoMemo(onoff.GetBool());
    nick->L_NoMemo(true);
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/NOMEMO"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/NOMEMO"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_lock_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_lock_Private", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_Private())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PRIVATE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Private(false);
    nick->Private(onoff.GetBool());
    nick->L_Private(true);
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/PRIVATE"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVATE"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_lock_PRIVMSG(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_lock_PRIVMSG", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PRIVMSG")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_PRIVMSG(false);
    nick->PRIVMSG(onoff.GetBool());
    nick->L_PRIVMSG(true);
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/PRIVMSG"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVMSG"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void NickServ::do_lock_Language(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_lock_Language", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

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

    mstring nickname = params.ExtractWord(3, " ");
    mstring lang = params.ExtractWord(4, " ").UpperCase();

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (lang.IsSameAs("default", true) || lang.IsSameAs("reset", true))
    {
	lang = Magick::instance().nickserv.DEF_Language().UpperCase();
    }
    else if (!mFile::Exists(Magick::instance().files.Langdir() + DirSlash + lang.LowerCase() + ".lng"))
    {
	SEND(mynick, source, "OS_STATUS/NOLANG", (lang));
	return;
    }

    if (Magick::instance().nickserv.LCK_Language())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/LANGUAGE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Language(false);
    nick->Language(lang);
    nick->L_Language(true);
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/LANGUAGE"), nickname,
	  mstring(lang + " (" + Magick::instance().getMessageL(lang, "ERR_SYNTAX/TRANSLATED") + ")")));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/LANGUAGE"), nickname, lang));
    ETCB();
}

void NickServ::do_unlock_Protect(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_unlock_Protect", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Protect())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PROTECT")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Protect(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/PROTECT"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PROTECT"), nickname));
    ETCB();
}

void NickServ::do_unlock_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_unlock_Secure", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Secure())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/SECURE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Secure(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/SECURE"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/SECURE"), nickname));
    ETCB();
}

void NickServ::do_unlock_NoMemo(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_unlock_NoMemo", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/NOMEMO")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_NoMemo(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/NOMEMO"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/NOMEMO"), nickname));
    ETCB();
}

void NickServ::do_unlock_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_unlock_Private", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Private())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PRIVATE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Private(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/PRIVATE"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVATE"), nickname));
    ETCB();
}

void NickServ::do_unlock_PRIVMSG(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_unlock_PRIVMSG", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PRIVMSG")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_PRIVMSG(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/PRIVMSG"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVMSG"), nickname));
    ETCB();
}

void NickServ::do_unlock_Language(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("NickServ::do_unlock_Language", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Language())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/LANGUAGE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Language(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/LANGUAGE"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/LANGUAGE"), nickname));
    ETCB();
}

SXP::Tag NickServ::tag_NickServ("NickServ");

void NickServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    FT("NickServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Nick_Stored_t *ns = new Nick_Stored_t;

    if (pElement->IsA(ns->GetClassTag()))
    {
	ns_array.push_back(ns);
	pIn->ReadTo(ns);
    }
    else
    {
	delete ns;
    }
    ETCB();
}

void NickServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);
    static_cast < void > (pElement);

    FT("NickServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
    ETCB();
}

void NickServ::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    FT("NickServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_NickServ);

    NickServ::stored_t::iterator iter;
    {
	RLOCK((lck_NickServ, lck_stored));
	for (iter = StoredBegin(); iter != StoredEnd(); iter++)
	{
	    map_entry < Nick_Stored_t > nstored(iter->second);
	    pOut->WriteSubElement(nstored.entry());
	}
    }

    pOut->EndObject(tag_NickServ);
    ETCB();
}

void NickServ::PostLoad()
{
    BTCB();
    NFT("NickServ::PostLoad");
    unsigned int i, j;

    for (i = 0; i < ns_array.size(); i++)
    {
	if (ns_array[i] != NULL)
	{
	    for (j = 0; j < ns_array[i]->ud_array.size(); j++)
	    {
		if (ns_array[i]->ud_array[j] != NULL)
		{
		    if (ns_array[i]->ud_array[j]->Contains("\n"))
			ns_array[i]->i_UserDef[ns_array[i]->ud_array[j]->Before("\n")] = ns_array[i]->ud_array[j]->After("\n");
		    delete ns_array[i]->ud_array[j];
		}
	    }
	    ns_array[i]->ud_array.clear();

	    if (!ns_array[i]->Name().empty())
		AddStored(ns_array[i]);
	    // delete ns_array[i];
	}
    }
    ns_array.clear();

    NickServ::stored_t::iterator iter;
    CP(("Linking nickname entries ..."));
    RLOCK((lck_NickServ, lck_stored));
    for (iter = StoredBegin(); iter != StoredEnd(); iter++)
    {
	map_entry < Nick_Stored_t > nstored(iter->second);
	if (!nstored->i_Host.empty())
	{
	    if (IsStored(nstored->i_Host))
	    {
		COM(("Nickname %s has been linked to %s ...", iter->first.c_str(), nstored->i_Host.c_str()));
		WLOCK((lck_NickServ, lck_stored, nstored->i_Host));
		GetStored(nstored->i_Host)->i_slaves.insert(iter->first);
	    }
	    else
	    {
		LOG(LM_WARNING, "ERROR/HOST_NOTREGD", (nstored->i_Host, iter->first));
		WLOCK((lck_NickServ, lck_stored, iter->first));
		nstored->i_Host.erase();
	    }
	}
    }
    ETCB();
}
