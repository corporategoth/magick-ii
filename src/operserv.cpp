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
#define RCSID(x,y) const char *rcsid_operserv_cpp_ ## x () { return y; }
RCSID(operserv_cpp, "@(#)$Id$");

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

bool OperServ::AddHost(const mstring & host)
{
    BTCB();
    FT("OperServ::AddHost", (host));
    bool retval = false;
    bool setAkill = false;

    {
	MLOCK((lck_OperServ, "CloneList"));
	MCB(CloneList.size());
	map < mstring, pair < unsigned int, list < mDateTime > > >::iterator iter = CloneList.find(host.LowerCase());
	if (iter == CloneList.end())
	{
	    CloneList[host.LowerCase()].first = 0;
	    iter = CloneList.find(host.LowerCase());
	}
	iter->second.first++;

	CP(("Finding clone list, %s = %d", host.c_str(), CloneList[host.LowerCase()].first));
	{
	    MLOCK2((lck_OperServ, "Clone"));
	    if (iter->second.first > (Clone_find(host) ? Clone->Value().first : Clone_Limit()))
	    {
		// Get rid of entries from the beginning...
		mDateTime thr(static_cast<time_t>(time(NULL) - Magick::instance().operserv.Clone_Time()));
		list<mDateTime>::iterator iter2 = lower_bound(iter->second.second.begin(), iter->second.second.end(), thr);
		iter->second.second.erase(iter->second.second.begin(), iter2);

		CP(("Event Size after purge is %d", iter->second.second.size()));

		iter->second.second.push_back(mDateTime::CurrentDateTime());
		bool burst = false;

		{
		    RLOCK((lck_IrcSvcHandler));
		    if (Magick::instance().ircsvchandler != NULL)
			burst = Magick::instance().ircsvchandler->Burst();
		}

		if (!burst && iter->second.second.size() > Magick::instance().operserv.Clone_Trigger())
		    setAkill = true;
		retval = true;
	    }
	}

	MCE(CloneList.size());
    }

    if (setAkill)
    {
	CP(("Reached MAX clone kills, adding AKILL ..."));

	MLOCK((lck_OperServ, "Akill"));
	if (!Akill_find("*@" + host))
	{
	    NickServ::live_t::iterator iter;
	    vector < mstring > killusers;
	    {
		RLOCK((lck_NickServ, lck_live));
		for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd();
		     iter++)
		{
		    map_entry < Nick_Live_t > nlive(iter->second);
		    if (nlive->Host().IsSameAs(host, true))
			killusers.push_back(nlive->Name());
		}
	    }

	    float percent =
		100.0 * static_cast < float > (killusers.size()) / static_cast < float >
		(Magick::instance().nickserv.LiveSize());

	    Akill_insert("*@" + host, Magick::instance().operserv.Clone_AkillTime(),
			 Magick::instance().operserv.Clone_Akill(), FirstName());
	    Magick::instance().server.AKILL("*@" + host, Magick::instance().operserv.Clone_Akill(),
					    Magick::instance().operserv.Clone_AkillTime(),
					    Magick::instance().nickserv.FirstName());
	    ANNOUNCE(FirstName(), "MISC/AKILL_ADD",
		     (FirstName(), host, ToHumanTime(Magick::instance().operserv.Clone_AkillTime()),
		      Magick::instance().operserv.Clone_Akill(), killusers.size(), fmstring("%.2f", percent)));
	    LOG(LM_INFO, "OPERSERV/AKILL_ADD",
		(FirstName(), host, ToHumanTime(Magick::instance().operserv.Clone_AkillTime()),
		 Magick::instance().operserv.Clone_Akill()));
	}
    }

    RET(retval);
    ETCB();
}

void OperServ::RemHost(const mstring & host)
{
    BTCB();
    FT("OperServ::RemHost", (host));

    MLOCK((lck_OperServ, "CloneList"));
    MCB(CloneList.size());
    map < mstring, pair < unsigned int, list < mDateTime > > >::iterator iter = CloneList.find(host.LowerCase());
    if (iter != CloneList.end())
    {
	CB(1, iter->second.first);
	if (iter->second.first > 1)
	{
	    iter->second.first--;
	    CE(1, iter->second.first);
	}
	else
	{
	    CloneList.erase(host.LowerCase());
	    CE(1, 0);
	}
    }
    MCE(CloneList.size());
    ETCB();
}

size_t OperServ::CloneList_sum() const
{
    BTCB();
    NFT("OperServ::ClonesList_sum");

    map < mstring, pair < unsigned int, list < mDateTime > > >::const_iterator i;
    size_t value = 0;

    RLOCK((lck_OperServ, "CloneList"));
    for (i = CloneList.begin(); i != CloneList.end(); i++)
    {
	value += i->second.first;
    }
    RET(value);
    ETCB();
}

size_t OperServ::CloneList_size(const unsigned int amt) const
{
    BTCB();
    FT("OperServ::CloneList_size", (amt));

    map < mstring, pair < unsigned int, list < mDateTime > > >::const_iterator i;
    size_t value = 0;

    RLOCK((lck_OperServ, "CloneList"));
    for (i = CloneList.begin(); i != CloneList.end(); i++)
    {
	if (i->second.first == amt)
	    value++;
    }
    RET(value);
    ETCB();
}

size_t OperServ::CloneList_Usage() const
{
    BTCB();
    size_t retval = 0;
    map < mstring, pair < unsigned int, list < mDateTime > > >::const_iterator i;

    MLOCK((lck_OperServ, "CloneList"));
    for (i = CloneList.begin(); i != CloneList.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second.first);
	list < mDateTime >::const_iterator j;
	for (j = i->second.second.begin(); j != i->second.second.end(); j++)
	    retval += sizeof(j->Internal());
    }
    return retval;
    ETCB();
}

void OperServ::CloneList_check()
{
    BTCB();
    NFT("OperServ::CloneList_check");

    map < mstring, pair < unsigned int, list < mDateTime > > >::iterator iter;

    MLOCK((lck_OperServ, "CloneList"));
    for (iter = CloneList.begin(); iter != CloneList.end(); iter++)
    {
	if (iter->second.second.size() > Magick::instance().operserv.Clone_Trigger())
	{
	    CP(("Reached MAX clone kills, adding AKILL ..."));

	    MLOCK2((lck_OperServ, "Akill"));
	    if (!Akill_find("*@" + iter->first))
	    {
		NickServ::live_t::iterator iter2;
		vector < mstring > killusers;
		{
		    RLOCK((lck_NickServ, lck_live));
		    for (iter2 = Magick::instance().nickserv.LiveBegin(); iter2 != Magick::instance().nickserv.LiveEnd();
			 iter2++)
		    {
			map_entry < Nick_Live_t > nlive(iter2->second);
			if (nlive->Host().IsSameAs(iter->first, true))
			    killusers.push_back(nlive->Name());
		    }
		}

		float percent =
		    100.0 * static_cast < float > (killusers.size()) / static_cast < float >
		    (Magick::instance().nickserv.LiveSize());

		Magick::instance().server.AKILL("*@" + iter->first, Magick::instance().operserv.Clone_Akill(),
						Magick::instance().operserv.Clone_AkillTime(),
						Magick::instance().nickserv.FirstName());

		Akill_insert("*@" + iter->first, Magick::instance().operserv.Clone_AkillTime(),
			     Magick::instance().operserv.Clone_Akill(), FirstName());
		ANNOUNCE(FirstName(), "MISC/AKILL_ADD",
			 (FirstName(), iter->first, ToHumanTime(Magick::instance().operserv.Clone_AkillTime()),
			  Magick::instance().operserv.Clone_Akill(), killusers.size(), fmstring("%.2f", percent)));
		LOG(LM_INFO, "OPERSERV/AKILL_ADD",
		    (FirstName(), iter->first, ToHumanTime(Magick::instance().operserv.Clone_AkillTime()),
		     Magick::instance().operserv.Clone_Akill()));
	    }
	}
    }
    ETCB();
}

bool OperServ::Clone_insert(const mstring & entry, const unsigned int value, const mstring & reason, const mstring & nick,
			    const mDateTime & added)
{
    BTCB();
    FT("OperServ::Clone_insert", (entry, value, reason, nick, added));

    // Wildcards but no @
    if (entry.Contains("@") || entry.Contains("!"))
    {
	RET(false);
    }

    MLOCK((lck_OperServ, "Clone"));
    if (!Clone_find(entry))
    {
	pair < set < Clone_Type >::iterator, bool > tmp;

	MCB(i_Clone.size());
	Clone_Type ent(entry, pair < unsigned int, mstring > (value, reason), nick, added);
	i_Clone.erase(ent);
	tmp = i_Clone.insert(ent);

	MCE(i_Clone.size());
	if (tmp.second)
	    Clone = tmp.first;
	else
	    Clone = i_Clone.end();
	RET(true);
    }
    else
    {
	Clone = i_Clone.end();
	RET(false);
    }
    ETCB();
}

bool OperServ::Clone_erase()
{
    BTCB();
    NFT("OperServ::Clone_erase");

    MLOCK((lck_OperServ, "Clone"));
    if (Clone != i_Clone.end())
    {
	MCB(i_Clone.size());
	i_Clone.erase(Clone++);
	MCE(i_Clone.size());
	RET(true);
    }
    else
    {
	RET(false);
    }
    ETCB();
}

size_t OperServ::Clone_Usage() const
{
    BTCB();

    MLOCK((lck_OperServ, "Clone"));
    size_t retval = accumulate(i_Clone.begin(), i_Clone.end(), 0, AddUsage());
    return retval;
    ETCB();
}

bool OperServ::Clone_find(const mstring & entry)
{
    BTCB();
    FT("OperServ::Clone_find", (entry));

//  entlist_val_ui<pair<int, mstring> > iter = i_Clone.end();
    MLOCK((lck_OperServ, "Clone"));
    set < Clone_Type >::iterator iter = i_Clone.end();
    if (!i_Clone.empty())
	iter = find_if(i_Clone.begin(), i_Clone.end(), EntryMatches(entry, true, true));

    if (iter != i_Clone.end())
    {
	Clone = iter;
	RET(true);
    }
    else
    {
	Clone = i_Clone.end();
	RET(false);
    }
    ETCB();
}

pair < unsigned int, mstring > OperServ::Clone_value(const mstring & entry)
{
    BTCB();
    FT("OperServ::Clone_value", (entry));

    pair < unsigned int, mstring > retval = pair < unsigned int, mstring > (0, "");

    MLOCK((lck_OperServ, "Clone"));
//  entlist_val_ui<pair<int, mstring> > iter = Clone;
    set < Clone_Type >::iterator iter = Clone;

    if (Clone_find(entry))
	retval = Clone->Value();
    Clone = iter;
    return retval;
    ETCB();
}

bool OperServ::Akill_insert(const mstring & entry, const unsigned long value, const mstring & reason, const mstring & nick,
			    const mDateTime & added)
{
    BTCB();
    FT("OperServ::Akill_insert", (entry, value, reason, nick, added));

    // no @
    if (!entry.Contains("@") || entry.Contains("!"))
    {
	RET(false);
    }

    MLOCK((lck_OperServ, "Akill"));
    if (!Akill_find(entry))
    {
	pair < set < Akill_Type >::iterator, bool > tmp;

	MCB(i_Akill.size());
	Akill_Type ent(entry, pair < unsigned long, mstring > (value, reason), nick, added);
	i_Akill.erase(ent);
	tmp = i_Akill.insert(ent);

	MCE(i_Akill.size());
	if (tmp.second)
	    Akill = tmp.first;
	else
	    Akill = i_Akill.end();
	RET(true);
    }
    else
    {
	Akill = i_Akill.end();
	RET(false);
    }
    ETCB();
}

bool OperServ::Akill_erase()
{
    BTCB();
    NFT("OperServ::Akill_erase");

    MLOCK((lck_OperServ, "Akill"));
    if (Akill != i_Akill.end())
    {
	MCB(i_Akill.size());
	i_Akill.erase(Akill++);
	MCE(i_Akill.size());
	RET(true);
    }
    else
    {
	RET(false);
    }

    ETCB();
}

size_t OperServ::Akill_Usage() const
{
    BTCB();

    MLOCK((lck_OperServ, "Akill"));
    size_t retval = accumulate(i_Akill.begin(), i_Akill.end(), 0, AddUsage());
    return retval;
    ETCB();
}

bool OperServ::Akill_find(const mstring & entry)
{
    BTCB();
    FT("OperServ::Akill_find", (entry));

    // no @
    if (!entry.Contains("@") || entry.Contains("!"))
    {
	RET(false);
    }

//  entlist_val_ui<pair<long, mstring> > iter = i_Akill.end();
    MLOCK((lck_OperServ, "Akill"));
    set < Akill_Type >::iterator iter = i_Akill.end();
    if (!i_Akill.empty())
	iter = find_if(i_Akill.begin(), i_Akill.end(), EntryMatches(entry, true, true));

    if (iter != i_Akill.end())
    {
	Akill = iter;
	RET(true);
    }
    else
    {
	Akill = i_Akill.end();
	RET(false);
    }
    ETCB();
}

pair < unsigned long, mstring > OperServ::Akill_value(const mstring & entry)
{
    BTCB();
    FT("OperServ::Akill_value", (entry));

    pair < unsigned long, mstring > retval = pair < unsigned long, mstring > (0, "");

    MLOCK((lck_OperServ, "Akill"));
//  entlist_val_ui<pair<long, mstring> > iter = Akill;
    set < Akill_Type >::iterator iter = Akill;

    if (Akill_find(entry))
	retval = Akill->Value();
    Akill = iter;
    return retval;
    ETCB();
}

bool OperServ::OperDeny_insert(const mstring & i_entry, const mstring & value, const mstring & nick)
{
    BTCB();
    FT("OperServ::OperDeny_insert", (i_entry, value, nick));

    // no @
    if (!i_entry.Contains("@"))
    {
	RET(false);
    }
    mstring entry(i_entry);

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.prepend("*!");

    MLOCK((lck_OperServ, "OperDeny"));
    if (!OperDeny_find(entry))
    {
	pair < set < OperDeny_Type >::iterator, bool > tmp;

	MCB(i_OperDeny.size());
	OperDeny_Type ent(entry, value, nick);
	i_OperDeny.erase(ent);
	tmp = i_OperDeny.insert(ent);
	MCE(i_OperDeny.size());
	if (tmp.second)
	    OperDeny = tmp.first;
	else
	    OperDeny = i_OperDeny.end();
	RET(true);
    }
    else
    {
	OperDeny = i_OperDeny.end();
	RET(false);
    }
    ETCB();
}

bool OperServ::OperDeny_erase()
{
    BTCB();
    NFT("OperServ::OperDeny_erase");

    MLOCK((lck_OperServ, "OperDeny"));
    if (OperDeny != i_OperDeny.end())
    {
	MCB(i_OperDeny.size());
	i_OperDeny.erase(OperDeny++);
	MCE(i_OperDeny.size());
	RET(true);
    }
    else
    {
	RET(false);
    }

    ETCB();
}

size_t OperServ::OperDeny_Usage() const
{
    BTCB();

    MLOCK((lck_OperServ, "OperDeny"));
    size_t retval = accumulate(i_OperDeny.begin(), i_OperDeny.end(), 0, AddUsage());
    return retval;
    ETCB();
}

bool OperServ::OperDeny_find(const mstring & i_entry)
{
    BTCB();
    FT("OperServ::OperDeny_find", (i_entry));

    // no @
    if (!i_entry.Contains("@"))
    {
	RET(false);
    }
    mstring entry(i_entry);

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.prepend("*!");

    MLOCK((lck_OperServ, "OperDeny"));
//  entlist_val_ui<mstring> iter = i_OperDeny.end();
    set < OperDeny_Type >::iterator iter = i_OperDeny.end();
    if (!i_OperDeny.empty())
	iter = find_if(i_OperDeny.begin(), i_OperDeny.end(), EntryMatches(entry, true, true));

    if (iter != i_OperDeny.end())
    {
	OperDeny = iter;
	RET(true);
    }
    else
    {
	OperDeny = i_OperDeny.end();
	RET(false);
    }
    ETCB();
}

mstring OperServ::OperDeny_value(const mstring & entry)
{
    BTCB();
    FT("OperServ::OperDeny_value", (entry));

    mstring retval;

    MLOCK((lck_OperServ, "OperDeny"));
//  entlist_val_ui<mstring> iter = OperDeny;
    set < OperDeny_Type >::iterator iter = OperDeny;

    if (OperDeny_find(entry))
	retval = OperDeny->Value();
    OperDeny = iter;
    return retval;
    ETCB();
}

bool OperServ::Ignore_insert(const mstring & i_entry, const bool perm, const mstring & nick,
			     const mDateTime & added)

{
    BTCB();
    FT("OperServ::Ignore_insert", (i_entry, perm, nick, added));

    // no @
    if (!i_entry.Contains("@"))
    {
	RET(false);
    }
    mstring entry(i_entry);

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.prepend("*!");

    MLOCK((lck_OperServ, "Ignore"));
    if (!Ignore_find(entry))
    {
	pair < set < Ignore_Type >::iterator, bool > tmp;

	MCB(i_Ignore.size());
	Ignore_Type ent(entry, perm, nick, added);
	i_Ignore.erase(ent);
	tmp = i_Ignore.insert(ent);
	MCE(i_Ignore.size());
	if (tmp.second)
	    Ignore = tmp.first;
	else
	    Ignore = i_Ignore.end();
	RET(true);
    }
    else
    {
	Ignore = i_Ignore.end();
	RET(false);
    }
    ETCB();
}

bool OperServ::Ignore_erase()
{
    BTCB();
    NFT("OperServ::Ignore_erase");

    MLOCK((lck_OperServ, "Ignore"));
    if (Ignore != i_Ignore.end())
    {
	MCB(i_Ignore.size());
	i_Ignore.erase(Ignore++);
	MCE(i_Ignore.size());
	RET(true);
    }
    else
    {
	RET(false);
    }

    ETCB();
}

size_t OperServ::Ignore_Usage() const
{
    BTCB();

    MLOCK((lck_OperServ, "Ignore"));
    size_t retval = accumulate(i_Ignore.begin(), i_Ignore.end(), 0, AddUsage());
    return retval;
    ETCB();
}

bool OperServ::Ignore_find(const mstring & i_entry)
{
    BTCB();
    FT("OperServ::Ignore_find", (i_entry));

    // no @
    if (!i_entry.Contains("@"))
    {
	RET(false);
    }
    mstring entry(i_entry);

    // Pre-pend *! if its missed
    if (!entry.Contains("!"))
	entry.prepend("*!");

    MLOCK((lck_OperServ, "Ignore"));
//  entlist_val_ui<pair<mDateTime, bool> > iter = i_Ignore.end();
    set < Ignore_Type >::iterator iter = i_Ignore.end();

    if (!i_Ignore.empty())
	iter = find_if(i_Ignore.begin(), i_Ignore.end(), EntryMatches(entry, true, true));

    if (iter != i_Ignore.end())
    {
	Ignore = iter;
	RET(true);
    }
    else
    {
	Ignore = i_Ignore.end();
	RET(false);
    }
    ETCB();
}

bool OperServ::Ignore_value(const mstring & entry)
{
    BTCB();
    FT("OperServ::Ignore_value", (entry));

    bool retval = false;

    MLOCK((lck_OperServ, "Ignore"));
//  entlist_val_ui<pair<mDateTime, bool> > iter = Ignore;
    set < Ignore_Type >::iterator iter = Ignore;

    if (Ignore_find(entry))
	retval = Ignore->Value();
    Ignore = iter;
    return retval;
    ETCB();
}

OperServ::OperServ()
{
    BTCB();
    NFT("OperServ::OperServ");
    messages = true;
    ETCB();
}

void OperServ::AddCommands()
{
    BTCB();
    NFT("OperServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* CONF*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_settings_Config);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* NICK*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_settings_Nick);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* CHAN*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_settings_Channel);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* OTH*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_settings_Other);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *ALL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_settings_All);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* ADD*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_clone_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* DEL*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_clone_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* ERA*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_clone_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_clone_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_clone_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL ADD*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_akill_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL DEL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_akill_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL ERA*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_akill_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_akill_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_akill_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* ADD*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_operdeny_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* DEL*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_operdeny_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* ERA*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_operdeny_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* LIST", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_operdeny_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* VIEW", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_operdeny_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* H*LP", Magick::instance().commserv.SOP_Name(),
						 do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* ADD*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_ignore_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* DEL*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_ignore_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* ERA*", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_ignore_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_ignore_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_ignore_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_2param);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name(),
						 OperServ::do_Help);
#ifdef MAGICK_TRACE_WORKS
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "TRACE", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Trace);
#endif
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "*MODE*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_Mode);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "Q*LINE*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_Qline);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNQ*LINE*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_UnQline);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "NO*OP*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_NOOP);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "KILL*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_Kill);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "HIDE*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_Hide);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "*PING*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), OperServ::do_Ping);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UPD*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Update);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SAVE*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Update);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SHUT*DOWN*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Shutdown);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "DIE*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Shutdown);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "RESTART*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Restart);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "RELOAD*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Reload);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SIGNON*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Signon);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOAD*", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Unload);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "JUPE*", Magick::instance().commserv.ADMIN_Name(),
						 OperServ::do_Jupe);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ID*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Identify);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "REG*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Register);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ON", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_On);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "OFF", Magick::instance().commserv.SADMIN_Name(),
						 OperServ::do_Off);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "HTM", Magick::instance().commserv.SOP_Name(),
						 OperServ::do_HTM);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CLONE*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "A*KILL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "O*DENY*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name(), do_1_2param);
    ETCB();
}

void OperServ::RemCommands()
{
    BTCB();
    NFT("OperServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* CONF*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* NICK*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* CHAN*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* OTH*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *ALL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* ADD*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* DEL*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* ERA*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL ADD*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL DEL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL ERA*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* ADD*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* DEL*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* ERA*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* LIST", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* VIEW", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* H*LP", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* ADD*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* DEL*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* ERA*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* LIST",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* VIEW",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* H*LP",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "TRACE", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "*MODE*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "Q*LINE*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNQ*LINE*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "NO*OP*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "KILL*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "HIDE*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "*PING*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UPD*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SAVE*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SHUT*DOWN*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "DIE*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "RESTART*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "RELOAD*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOAD*", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "JUPE*", Magick::instance().commserv.ADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ID*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "REG*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ON", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "OFF", Magick::instance().commserv.SADMIN_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "HTM", Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CLONE*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "A*KILL*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "O*DENY*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* *",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN*",
						 Magick::instance().commserv.OPER_Name() + " " +
						 Magick::instance().commserv.SOP_Name());
    ETCB();
}

void OperServ::execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    mThread::ReAttach(tt_OperServ);
    FT("OperServ::execute", (source, msgtype, params));
    //okay this is the main operserv command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message
    mstring mynick(Magick::instance().getLname(IrcParam(params, 1)));
    mstring message(params.After(" :"));
    mstring command(message.Before(" "));

    // We SHOULD still process THESE messages, ONLY.
    if (!MSG())
    {
	if (!(command.IsSameAs("ON", true) || command.IsSameAs("OFF", true) || command.Matches("ID*", true)))
	{
	    return;
	}
    }

    if (message[0U] == CTCP_DELIM_CHAR)
    {
	if (msgtype == "PRIVMSG")
	    DccEngine::decodeRequest(mynick, source, message);
	else
	    DccEngine::decodeReply(mynick, source, message);
    }
    else if (Secure() && !Magick::instance().nickserv.GetLive(source)->HasMode("o"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
    }
    else if (msgtype == "PRIVMSG" && !Magick::instance().commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);
    ETCB();
}

void OperServ::do_Help(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Help", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    mstring HelpTopic = Magick::instance().operserv.GetInternalName();

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

#ifdef MAGICK_TRACE_WORKS
void OperServ::do_Trace(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Trace", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    unsigned int i, j, k;
    mstring output;
    mstring action = params.ExtractWord(2, " ").UpperCase();
    mstring ttype = params.ExtractWord(3, " ").UpperCase();
    threadtype_enum type;

    vector < mstring > levels;
    for (i = 4; i < params.WordCount(" ") + 1; i++)
	levels.push_back(params.ExtractWord(i, " ").UpperCase());

    if (ttype.IsSameAs("ALL", true))
    {
	type = tt_MAX;
	ttype = "ALL";
    }
    else
    {
	for (i = 0 + 1; i < tt_MAX; i++)
	    if (ttype == threadname[i])
		break;
	if (i < tt_MAX)
	{
	    type = static_cast < threadtype_enum > (i);
	}
	else
	{
	    SEND(mynick, source, "OS_STATUS/INVALID_THREAD", (ttype));
	    return;
	}
    }

    if (action.IsSameAs("SET", true))
    {
	if (makehex(levels[0U]) != 0 || levels[0U] == "0" || levels[0U] == "0x0000")
	{
	    if (type == tt_MAX)
	    {
		for (i = 0; i < tt_MAX; i++)
		{
		    Trace::TurnSet(static_cast < threadtype_enum > (i), makehex(levels[0U]));
		    output.Format("%s SET: Trace level set to %#06x.", mDateTime::CurrentDateTime().DateTimeString().c_str(),
				  Trace::TraceLevel(static_cast < threadtype_enum > (i)));
		    {
			MLOCK(("ThreadMessageQueue"));
			ThreadMessageQueue.push_back(pair < threadtype_enum,
						     mstring > (static_cast < threadtype_enum > (i), output));
		    }
		}
	    }
	    else
	    {
		Trace::TurnSet(type, makehex(levels[0U]));
		output.Format("%s SET: Trace level set to %#06x.", mDateTime::CurrentDateTime().DateTimeString().c_str(),
			      Trace::TraceLevel(type));
		{
		    MLOCK(("ThreadMessageQueue"));
		    ThreadMessageQueue.push_back(pair < threadtype_enum, mstring > (type, output));
		}
	    }
	}
	else
	{
	    for (i = 0; i < levels.size(); i++)
	    {
		for (j = 0; j < Trace::levelname.size(); j++)
		{
		    if (levels[i].Matches(Trace::levelname[j].name, true))
		    {
			if (type == tt_MAX)
			{
			    for (k = 0; k < tt_MAX; k++)
			    {
				Trace::TurnSet(static_cast < threadtype_enum > (k), 0U);
				Trace::TurnUp(static_cast < threadtype_enum > (k), Trace::levelname[j].level);
				output.Format("%s SET: Trace level set to %#06x.",
					      mDateTime::CurrentDateTime().DateTimeString().c_str(),
					      Trace::TraceLevel(static_cast < threadtype_enum > (k)));
				{
				    MLOCK(("ThreadMessageQueue"));
				    ThreadMessageQueue.push_back(pair < threadtype_enum,
								 mstring > (static_cast < threadtype_enum > (k), output));
				}
			    }
			}
			else
			{
			    Trace::TurnSet(static_cast < threadtype_enum > (type), 0U);
			    Trace::TurnUp(type, Trace::levelname[j].level);
			    output.Format("%s SET: Trace level set to %#06x.",
					  mDateTime::CurrentDateTime().DateTimeString().c_str(), Trace::TraceLevel(type));
			    {
				MLOCK(("ThreadMessageQueue"));
				ThreadMessageQueue.push_back(pair < threadtype_enum, mstring > (type, output));
			    }
			}
			break;
		    }
		}
		if (j >= Trace::levelname.size())
		    SEND(mynick, source, "OS_STATUS/INVALID_LEVEL", (levels[i]));
	    }
	}
    }
    else if (action.Matches("*UP", true))
    {
	for (i = 0; i < levels.size(); i++)
	{
	    for (j = 0; j < Trace::levelname.size(); j++)
	    {
		if (levels[i].Matches(Trace::levelname[j].name, true))
		{
		    if (type == tt_MAX)
		    {
			for (k = 0; k < tt_MAX; k++)
			{
			    Trace::TurnUp(static_cast < threadtype_enum > (k), Trace::levelname[j].level);
			    output.Format("%s UP: Trace level set to %#06x.",
					  mDateTime::CurrentDateTime().DateTimeString().c_str(),
					  Trace::TraceLevel(static_cast < threadtype_enum > (k)));
			    {
				MLOCK(("ThreadMessageQueue"));
				ThreadMessageQueue.push_back(pair < threadtype_enum,
							     mstring > (static_cast < threadtype_enum > (k), output));
			    }
			}
		    }
		    else
		    {
			Trace::TurnUp(type, Trace::levelname[j].level);
			output.Format("%s UP: Trace level set to %#06x.",
				      mDateTime::CurrentDateTime().DateTimeString().c_str(), Trace::TraceLevel(type));
			{
			    MLOCK(("ThreadMessageQueue"));
			    ThreadMessageQueue.push_back(pair < threadtype_enum, mstring > (type, output));
			}
		    }
		    break;
		}
	    }
	    if (j >= Trace::levelname.size())
		SEND(mynick, source, "OS_STATUS/INVALID_LEVEL", (levels[i]));
	}
    }
    else if (action.Matches("*DOWN", true))
    {
	for (i = 0; i < levels.size(); i++)
	{
	    for (j = 0; j < Trace::levelname.size(); j++)
	    {
		if (levels[i].Matches(Trace::levelname[j].name, true))
		{
		    if (type == tt_MAX)
		    {
			for (k = 0; k < tt_MAX; k++)
			{
			    Trace::TurnDown(static_cast < threadtype_enum > (k), Trace::levelname[j].level);
			    output.Format("%s DOWN: Trace level set to %#06x.",
					  mDateTime::CurrentDateTime().DateTimeString().c_str(),
					  Trace::TraceLevel(static_cast < threadtype_enum > (k)));
			    {
				MLOCK(("ThreadMessageQueue"));
				ThreadMessageQueue.push_back(pair < threadtype_enum,
							     mstring > (static_cast < threadtype_enum > (k), output));
			    }
			}
		    }
		    else
		    {
			Trace::TurnDown(type, Trace::levelname[j].level);
			output.Format("%s DOWN: Trace level set to %#06x.",
				      mDateTime::CurrentDateTime().DateTimeString().c_str(), Trace::TraceLevel(type));
			{
			    MLOCK(("ThreadMessageQueue"));
			    ThreadMessageQueue.push_back(pair < threadtype_enum, mstring > (type, output));
			}
		    }
		    break;
		}
	    }
	    if (j >= Trace::levelname.size())
		SEND(mynick, source, "OS_STATUS/INVALID_LEVEL", (levels[i]));
	}
    }
    else if (action.Matches("VIEW*", true) || action.Matches("LIST*", true))
    {
	// Its taken care of below :)
    }
    else
    {
	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (message + " " + action, mynick, message));
	return;
    }

    mstring line1, line2, tmp;

    for (i = 0; i < tt_MAX; i++)
    {
	tmp.Format("%6s  ", threadname[i].c_str());
	line1 += tmp;
	tmp.Format("%#06x  ", Trace::TraceLevel(static_cast < threadtype_enum > (i)));
	line2 += tmp;
    }
    Magick::instance().operserv.stats.i_Trace++;
    ::send(mynick, source, line1);
    ::send(mynick, source, line2);
    if (!(action.Matches("VIEW*", true) || action.Matches("LIST*", true)))
    {
	NSLOG(LM_NOTICE, line1);
	NSLOG(LM_NOTICE, line2);
    }
    ETCB();
}
#endif

void OperServ::do_Mode(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Mode", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring mode = params.ExtractFrom(3, " ");

    if (IsChan(target))
    {
	if (Magick::instance().chanserv.IsLive(target))
	{
	    target = Magick::instance().getLname(target);
	    Magick::instance().server.MODE(mynick, target, mode);
	    Magick::instance().operserv.stats.i_Mode++;
	    ANNOUNCE(mynick, "MISC/CHAN_MODE", (source, mode, target));
	    SEND(mynick, source, "OS_COMMAND/CHAN_MODE", (mode, target));

	    LOG(LM_INFO, "OPERSERV/MODE",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), mode, target));
	}
	else
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTINUSE", (target));
	}
    }
    else
    {
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	    Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source))
	{
	    if (Magick::instance().nickserv.IsLive(target))
	    {
		if (!Magick::instance().server.proto.SVSMODE().empty())
		{
		    target = Magick::instance().getLname(target);
		    Magick::instance().server.SVSMODE(mynick, target, mode);
		    Magick::instance().operserv.stats.i_Mode++;
		    ANNOUNCE(mynick, "MISC/NICK_MODE", (source, mode, target));
		    SEND(mynick, source, "OS_COMMAND/NICK_MODE", (mode, target));
		    LOG(LM_INFO, "OPERSERV/MODE",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), mode, target));
		}
		else
		{
		    NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
		}
	    }
	    else
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	    }
	}
	else
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	}
    }
    ETCB();
}

void OperServ::do_Qline(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Qline", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (Magick::instance().server.proto.SQLINE().empty())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring reason;

    if (params.WordCount(" ") > 2)
	reason = params.ExtractFrom(3, " ");
    Magick::instance().server.SQLINE(mynick, target, reason);
    Magick::instance().operserv.stats.i_Qline++;
    SEND(mynick, source, "OS_COMMAND/QLINE", (target, Magick::instance().getMessage(source, "VALS/ON")));
    ANNOUNCE(mynick, "MISC/QLINE", (source, Magick::instance().getMessage("VALS/ON"), target));
    LOG(LM_INFO, "OPERSERV/QLINE", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target));
    ETCB();
}

void OperServ::do_UnQline(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_UnQline", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (Magick::instance().server.proto.UNSQLINE().empty())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    Magick::instance().server.UNSQLINE(mynick, target);
    Magick::instance().operserv.stats.i_Unqline++;
    SEND(mynick, source, "OS_COMMAND/QLINE", (target, Magick::instance().getMessage(source, "VALS/OFF")));
    ANNOUNCE(mynick, "MISC/QLINE", (source, Magick::instance().getMessage("VALS/OFF"), target));
    LOG(LM_INFO, "OPERSERV/UNQLINE", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target));
    ETCB();
}

void OperServ::do_NOOP(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_NOOP", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (Magick::instance().server.proto.SVSNOOP().empty())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring onoff = params.ExtractWord(3, " ");

    if (!Magick::instance().server.IsList(target))
    {
	SEND(mynick, source, "OS_STATUS/ISNOTLINKED", (target));
	return;
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    Magick::instance().server.SVSNOOP(mynick, target, onoff.GetBool());
    Magick::instance().operserv.stats.i_Noop++;
    SEND(mynick, source, "OS_COMMAND/NOOP",
	 ((onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF")), target));
    ANNOUNCE(mynick, "MISC/NOOP", (source, (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
							      Magick::instance().getMessage("VALS/OFF")), target));
    LOG(LM_INFO, "OPERSERV/NOOP", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
			    Magick::instance().getMessage("VALS/OFF"))));
    ETCB();
}

void OperServ::do_Kill(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Kill", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (Magick::instance().server.proto.SVSKILL().empty())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring reason = params.ExtractFrom(3, " ");

    if (Magick::instance().nickserv.IsLive(target))
    {
	target = Magick::instance().getLname(target);
	Magick::instance().server.SVSKILL(mynick, target, reason);
	Magick::instance().operserv.stats.i_Kill++;
	SEND(mynick, source, "OS_COMMAND/KILL", (target));
	ANNOUNCE(mynick, "MISC/KILL", (source, target));
	LOG(LM_INFO, "OPERSERV/KILL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, reason));
    }
    else
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
    }
    ETCB();
}

void OperServ::do_Hide(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Hide", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (Magick::instance().server.proto.SVSHOST().empty())
    {
	NSEND(mynick, source, "ERR_SITUATION/NOT_SUPPORTED");
	return;
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring newhost = params.ExtractWord(3, " ");

    if (!newhost.IsHostName())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEHOST");
	return;
    }

    if (Magick::instance().nickserv.IsLive(target))
    {
	target = Magick::instance().getLname(target);
	Magick::instance().server.SVSHOST(mynick, target, newhost);
	Magick::instance().operserv.stats.i_Hide++;
	SEND(mynick, source, "OS_COMMAND/HIDE", (target, newhost));
	ANNOUNCE(mynick, "MISC/HIDE", (source, target, newhost));
	LOG(LM_INFO, "OPERSERV/HIDE",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, newhost));
    }
    else
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
    }
    ETCB();
}

void OperServ::do_Ping(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Ping", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    {
	RLOCK((lck_Events));
	if (Magick::instance().events != NULL)
	{
	    Magick::instance().events->ForcePing();
	    Magick::instance().operserv.stats.i_Ping++;
	    NSEND(mynick, source, "OS_COMMAND/PING");
	    LOG(LM_DEBUG, "OPERSERV/PING", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H)));
	}
    }
    ETCB();
}

void OperServ::do_Update(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Update", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_Events));
	if (Magick::instance().events != NULL)
	{
	    Magick::instance().events->ForceSave();
	    Magick::instance().operserv.stats.i_Update++;
	    NSEND(mynick, source, "OS_COMMAND/UPDATE");
	    LOG(LM_DEBUG, "OPERSERV/UPDATE", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H)));
	}
    }
    ETCB();
}

void OperServ::do_Shutdown(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Shutdown", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring reason = params.ExtractFrom(2, " ");

    NSEND(mynick, source, "OS_COMMAND/SHUTDOWN");
    ANNOUNCE(mynick, "MISC/SHUTDOWN", (source, reason));
    LOG(LM_NOTICE, "OPERSERV/SHUTDOWN", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), reason));
    Magick::instance().Shutdown(true);
    Magick::instance().Die();
    ETCB();
}

void OperServ::do_Restart(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Restart", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring reason = params.ExtractFrom(2, " ");

    NSEND(mynick, source, "OS_COMMAND/RESTART");
    ANNOUNCE(mynick, "MISC/RESTART", (source, reason));
    LOG(LM_NOTICE, "OPERSERV/RESTART", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), reason));
    Magick::instance().Die();
    ETCB();
}

void OperServ::do_Reload(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Reload", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (Magick::instance().get_config_values())
    {
	Magick::instance().operserv.stats.i_Reload++;
	NSEND(mynick, source, "OS_COMMAND/RELOAD");
	ANNOUNCE(mynick, "MISC/RELOAD", (source));
	LOG(LM_NOTICE, "OPERSERV/RELOAD", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H)));
    }
    else
    {
	LOG(LM_ERROR, "COMMANDLINE/NO_CFG_FILE", (Magick::instance().Config_File()));
	NSEND(mynick, source, "OS_COMMAND/RELOAD_FAIL");
    }
    ETCB();
}

void OperServ::do_Signon(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Reload", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    Magick::instance().server.SignOnAll();
    NSEND(mynick, source, "OS_COMMAND/SIGNON");
    ETCB();
}

void OperServ::do_Unload(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Unload", (mynick, source, params));
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

    mstring language = params.ExtractWord(2, " ").UpperCase();

    bool unload1, unload2;

    unload1 = Magick::instance().UnloadExternalMessages(language);
    unload2 = Magick::instance().UnloadHelp(language);
    if (unload1 || unload2)
    {
	Magick::instance().operserv.stats.i_Unload++;
	SEND(mynick, source, "OS_COMMAND/UNLOAD", (language));
	LOG(LM_DEBUG, "OPERSERV/UNLOAD", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), language));
    }
    else
    {
	SEND(mynick, source, "OS_STATUS/ISNOTLANG", (language));
    }
    ETCB();
}

void OperServ::do_Jupe(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Jupe", (mynick, source, params));
    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring reason = params.ExtractFrom(3, " ");

    if (!target.Contains("."))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", ("JUPE", '.'));
	return;
    }

    Magick::instance().server.Jupe(target, reason);
    Magick::instance().operserv.stats.i_Jupe++;
    SEND(mynick, source, "OS_COMMAND/JUPE", (target));
    ANNOUNCE(mynick, "MISC/JUPE", (source, target));
    LOG(LM_NOTICE, "OPERSERV/JUPE", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, reason));
    ETCB();
}

void OperServ::do_On(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_On", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    // Later, make the ability to turn on/off specific services
    // also the ability to turn of either MSG, or AUTO or BOTH

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring type = params.ExtractWord(2, " ");

    if (type.Matches("AUTO*", true))
    {
	Magick::instance().AUTO(true);
	Magick::instance().operserv.stats.i_OnOff++;
	SEND(mynick, source, "OS_COMMAND/ONOFF",
	     (Magick::instance().getMessage(source, "VALS/SVC_AUTO"), Magick::instance().getMessage(source, "VALS/ON")));
	ANNOUNCE(mynick, "MISC/ONOFF",
		 (Magick::instance().getMessage("VALS/SVC_AUTO"), Magick::instance().getMessage("VALS/ON"), source));
	LOG(LM_NOTICE, "OPERSERV/ONOFF",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("VALS/ON"),
	     Magick::instance().getMessage("VALS/SVC_AUTO")));
    }
    else if (type.Matches("LOG*", true))
    {
	Magick::instance().ActivateLogger();
	Magick::instance().operserv.stats.i_OnOff++;
	SEND(mynick, source, "OS_COMMAND/ONOFF",
	     (Magick::instance().getMessage(source, "VALS/SVC_LOG"), Magick::instance().getMessage(source, "VALS/ON")));
	ANNOUNCE(mynick, "MISC/ONOFF",
		 (Magick::instance().getMessage("VALS/SVC_LOG"), Magick::instance().getMessage("VALS/ON"), source));
	LOG(LM_NOTICE, "OPERSERV/ONOFF",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("VALS/ON"),
	     Magick::instance().getMessage("VALS/SVC_LOG")));
    }
    else if (type.Matches("M*S*G*", true))
    {
	if (params.WordCount(" ") < 3)
	{
	    Magick::instance().MSG(true);
	    Magick::instance().operserv.stats.i_OnOff++;
	    SEND(mynick, source, "OS_COMMAND/ONOFF",
		 (Magick::instance().getMessage(source, "VALS/SVC_MSG"), Magick::instance().getMessage(source, "VALS/ON")));
	    ANNOUNCE(mynick, "MISC/ONOFF",
		     (Magick::instance().getMessage("VALS/SVC_MSG"), Magick::instance().getMessage("VALS/ON"), source));
	    LOG(LM_NOTICE, "OPERSERV/ONOFF",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().getMessage("VALS/ON"), Magick::instance().getMessage("VALS/SVC_MSG")));
	}
	else
	{
	    mstring service = params.ExtractWord(3, " ");
	    mBase *serv = mBase::GetByInternalName(service);
	    if (serv == NULL)
	    {
		SEND(mynick, source, "ERR_SITUATION/NOSERVICE", (service));
		return;
	    }

	    serv->MSG(true);
	    Magick::instance().operserv.stats.i_OnOff++;
	    SEND(mynick, source, "OS_COMMAND/ONOFF_ONE", (Magick::instance().getMessage(source, "VALS/SVC_MSG"), service,
							  Magick::instance().getMessage(source, "VALS/ON")));
	    ANNOUNCE(mynick, "MISC/ONOFF_ONE", (Magick::instance().getMessage("VALS/SVC_MSG"), service,
						Magick::instance().getMessage("VALS/ON"), source));
	    LOG(LM_NOTICE, "OPERSERV/ONOFF_ONE", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
						  Magick::instance().getMessage("VALS/ON"),
						  Magick::instance().getMessage("VALS/SVC_MSG"), service));
	}
    }
    ETCB();
}

void OperServ::do_Off(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_Off", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    // Later, make the ability to turn on/off specific services
    // also the ability to turn of either MSG, or AUTO or BOTH

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring type = params.ExtractWord(2, " ");

    if (type.Matches("AUTO*", true))
    {
	Magick::instance().AUTO(false);
	Magick::instance().operserv.stats.i_OnOff++;
	SEND(mynick, source, "OS_COMMAND/ONOFF",
	     (Magick::instance().getMessage(source, "VALS/SVC_AUTO"), Magick::instance().getMessage(source, "VALS/OFF")));
	ANNOUNCE(mynick, "MISC/ONOFF",
		 (Magick::instance().getMessage("VALS/SVC_AUTO"), Magick::instance().getMessage("VALS/OFF"), source));
	LOG(LM_NOTICE, "OPERSERV/ONOFF",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("VALS/OFF"), Magick::instance().getMessage("VALS/SVC_AUTO")));
    }
    else if (type.Matches("LOG*", true))
    {
	Magick::instance().operserv.stats.i_OnOff++;
	SEND(mynick, source, "OS_COMMAND/ONOFF",
	     (Magick::instance().getMessage(source, "VALS/SVC_LOG"), Magick::instance().getMessage(source, "VALS/OFF")));
	ANNOUNCE(mynick, "MISC/ONOFF",
		 (Magick::instance().getMessage("VALS/SVC_LOG"), Magick::instance().getMessage("VALS/OFF"), source));
	LOG(LM_NOTICE, "OPERSERV/ONOFF",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("VALS/OFF"), Magick::instance().getMessage("VALS/SVC_LOG")));
	Magick::instance().DeactivateLogger();
    }
    else if (type.Matches("M*S*G*", true))
    {
	if (params.WordCount(" ") < 3)
	{
	    Magick::instance().MSG(false);
	    Magick::instance().operserv.stats.i_OnOff++;
	    SEND(mynick, source, "OS_COMMAND/ONOFF",
		 (Magick::instance().getMessage(source, "VALS/SVC_MSG"), Magick::instance().getMessage(source, "VALS/OFF")));
	    ANNOUNCE(mynick, "MISC/ONOFF",
		     (Magick::instance().getMessage("VALS/SVC_MSG"), Magick::instance().getMessage("VALS/OFF"), source));
	    LOG(LM_NOTICE, "OPERSERV/ONOFF",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().getMessage("VALS/OFF"), Magick::instance().getMessage("VALS/SVC_MSG")));
	}
	else
	{
	    mstring service = params.ExtractWord(3, " ");
	    mBase *serv = mBase::GetByInternalName(service);
	    if (serv == NULL)
	    {
		SEND(mynick, source, "ERR_SITUATION/NOSERVICE", (service));
		return;
	    }

	    serv->MSG(false);
	    Magick::instance().operserv.stats.i_OnOff++;
	    SEND(mynick, source, "OS_COMMAND/ONOFF_ONE", (Magick::instance().getMessage(source, "VALS/SVC_MSG"), service,
							  Magick::instance().getMessage(source, "VALS/OFF")));
	    ANNOUNCE(mynick, "MISC/ONOFF_ONE", (Magick::instance().getMessage("VALS/SVC_MSG"), service,
						Magick::instance().getMessage("VALS/OFF"), source));
	    LOG(LM_NOTICE, "OPERSERV/ONOFF_ONE", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
						  Magick::instance().getMessage("VALS/OFF"),
						  Magick::instance().getMessage("VALS/SVC_MSG"), service));
	}
    }
    ETCB();
}

void OperServ::do_HTM(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_HTM", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "OS_STATUS/HTM",
	     (ToHumanSpace(Magick::instance().ircsvchandler->HTM_Threshold()),
	      fmstring("%.1f",
		       static_cast < float >
		       (Magick::instance().ircsvchandler->Average(Magick::instance().ircsvchandler->HTM_Gap())) / static_cast <
		       float > (1024))));
    }
    else
    {
	mstring command = params.ExtractWord(2, " ").UpperCase();

	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL)
	    {

		if (command.IsSameAs("ON", true))
		{
		    Magick::instance().ircsvchandler->HTM(true);
		    SEND(mynick, source, "OS_COMMAND/HTM", (Magick::instance().getMessage(source, "VALS/ON")));
		    ANNOUNCE(mynick, "MISC/HTM_ON_FORCE", (source));
		    LOG(LM_NOTICE, "OPERSERV/HTM_FORCE",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
			 Magick::instance().getMessage("VALS/ON")));
		}
		else if (command.IsSameAs("OFF", true))
		{
		    Magick::instance().ircsvchandler->HTM(false);
		    SEND(mynick, source, "OS_COMMAND/HTM", (Magick::instance().getMessage(source, "VALS/OFF")));
		    ANNOUNCE(mynick, "MISC/HTM_OFF_FORCE", (source));
		    LOG(LM_NOTICE, "OPERSERV/HTM_FORCE",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
			 Magick::instance().getMessage("VALS/OFF")));
		}
		else if (command.IsSameAs("SET", true))
		{
		    if (params.WordCount(" ") < 3)
		    {
			SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message + " " + command, mynick, message));
			return;
		    }
		    size_t newsize = FromHumanSpace(params.ExtractWord(3, " "));

		    if (!newsize)
		    {
			SEND(mynick, source, "ERR_SYNTAX/MUSTBESIZE", (ToHumanSpace(512)));
			return;
		    }
		    Magick::instance().ircsvchandler->HTM_Threshold(newsize);

		    SEND(mynick, source, "OS_COMMAND/HTM_SET", (ToHumanSpace(newsize)));
		    ANNOUNCE(mynick, "MISC/HTM_SET", (ToHumanSpace(newsize), source));
		    LOG(LM_NOTICE, "OPERSERV/HTM_SET",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), ToHumanSpace(newsize)));
		}
		else
		{
		    SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (message + " " + command, mynick, message));
		    return;
		}

	    }
	}
    }
    ETCB();
}

void OperServ::do_settings_Config(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_settings_Config", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

/*
-   Databases are (not) encrypted, and compressed at level ?.
-   Minimum threads active is ?, Current threads active is ?.
-   New thread will spawn each ? messages, and die when below ?.
*/
    SEND(mynick, source, "OS_SETTINGS/CFG_LEVEL", (Magick::instance().startup.Level(), Magick::instance().Level()));
    SEND(mynick, source, "OS_SETTINGS/CFG_LAG", (ToHumanTime(Magick::instance().startup.Lagtime(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_LAGCHECK", (ToHumanTime(Magick::instance().config.Ping_Frequency(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_SERVERS", (Magick::instance().startup.Server_size()));
    SEND(mynick, source, "OS_SETTINGS/CFG_RELINK", (ToHumanTime(Magick::instance().config.Server_Relink(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_SQUIT1", (ToHumanTime(Magick::instance().config.Squit_Protect(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_SQUIT2", (ToHumanTime(Magick::instance().config.Squit_Cancel(), source)));
    {
	RLOCK((lck_Events));
	if (Magick::instance().events != NULL)
	    SEND(mynick, source, "OS_SETTINGS/CFG_SYNC",
		 (ToHumanTime(Magick::instance().config.Savetime(), source), Magick::instance().events->SyncTime(source)));
    }
    SEND(mynick, source, "OS_SETTINGS/CFG_CYCLE",
	 (ToHumanTime(Magick::instance().config.Cycletime(), source),
	  ToHumanTime(Magick::instance().config.Checktime(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_DCC1",
	 (ToHumanSpace(Magick::instance().files.Blocksize()), ToHumanTime(Magick::instance().files.Timeout(), source)));
    SEND(mynick, source, "OS_SETTINGS/CFG_DCC2",
	 (ToHumanSpace(Magick::instance().files.Min_Speed()), ToHumanSpace(Magick::instance().files.Max_Speed()),
	  ToHumanTime(Magick::instance().files.Sampletime(), source)));
    ETCB();
}

void OperServ::do_settings_Nick(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_settings_Nick", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    SEND(mynick, source, "OS_SETTINGS/NICK_EXPIRE", (ToHumanTime(Magick::instance().nickserv.Expire(), source)));
    SEND(mynick, source, "OS_SETTINGS/NICK_IDENT", (ToHumanTime(Magick::instance().nickserv.Ident(), source)));
    SEND(mynick, source, "OS_SETTINGS/NICK_HOLD", (ToHumanTime(Magick::instance().nickserv.Release(), source)));
    SEND(mynick, source, "OS_SETTINGS/NICK_PASS", (Magick::instance().nickserv.Passfail()));

    mstring output;

    if (Magick::instance().nickserv.DEF_Protect())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_Protect())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/PROTECT");
	if (Magick::instance().nickserv.LCK_Protect())
	    output << IRC_Off;
    }

    if (Magick::instance().nickserv.DEF_Secure())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_Secure())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/SECURE");
	if (Magick::instance().nickserv.LCK_Secure())
	    output << IRC_Off;
    }

    if (Magick::instance().nickserv.DEF_NoExpire())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_NoExpire())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/NOEXPIRE");
	if (Magick::instance().nickserv.LCK_NoExpire())
	    output << IRC_Off;
    }

    if (Magick::instance().nickserv.DEF_NoMemo())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_NoMemo())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/NOMEMO");
	if (Magick::instance().nickserv.LCK_NoMemo())
	    output << IRC_Off;
    }

    if (Magick::instance().nickserv.DEF_Private())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_Private())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/PRIVATE");
	if (Magick::instance().nickserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Magick::instance().nickserv.DEF_PRIVMSG())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().nickserv.LCK_PRIVMSG())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/PRIVMSG");
	if (Magick::instance().nickserv.LCK_PRIVMSG())
	    output << IRC_Off;
    }

    SEND(mynick, source, "OS_SETTINGS/NICK_OPTIONS", (output));

    output.erase();
    if (Magick::instance().nickserv.LCK_Language())
	output << IRC_Bold;
    output << Magick::instance().nickserv.DEF_Language();
    if (Magick::instance().nickserv.LCK_Language())
	output << IRC_Off;

    SEND(mynick, source, "OS_SETTINGS/NICK_LANG", (output));

    SEND(mynick, source, "OS_SETTINGS/NICK_PICSIZE", (ToHumanSpace(Magick::instance().nickserv.PicSize())));
    SEND(mynick, source, "OS_SETTINGS/NICK_PICEXT", (Magick::instance().nickserv.PicExt()));
    SEND(mynick, source, "OS_SETTINGS/NICK_FILES",
	 (Magick::instance().memoserv.Files(), ToHumanSpace(Magick::instance().memoserv.FileSize())));
    ETCB();
}

void OperServ::do_settings_Channel(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_settings_Channel", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    SEND(mynick, source, "OS_SETTINGS/CHAN_EXPIRE", (ToHumanTime(Magick::instance().chanserv.Expire(), source)));
    SEND(mynick, source, "OS_SETTINGS/CHAN_IDENT", (Magick::instance().chanserv.Passfail()));
    SEND(mynick, source, "OS_SETTINGS/CHAN_KEEPTIME", (ToHumanTime(Magick::instance().chanserv.ChanKeep(), source)));

    mstring output;

    if (Magick::instance().chanserv.LCK_Bantime())
	output << IRC_Bold;
    output << ToHumanTime(Magick::instance().chanserv.DEF_Bantime(), source);
    if (Magick::instance().chanserv.LCK_Bantime())
	output << IRC_Off;
    SEND(mynick, source, "OS_SETTINGS/CHAN_BANTIME", (output));
    output.erase();
    if (Magick::instance().chanserv.LCK_Parttime())
	output << IRC_Bold;
    output << ToHumanTime(Magick::instance().chanserv.DEF_Parttime(), source);
    if (Magick::instance().chanserv.LCK_Parttime())
	output << IRC_Off;
    SEND(mynick, source, "OS_SETTINGS/CHAN_PARTTIME", (output));

    SEND(mynick, source, "OS_SETTINGS/CHAN_MLOCK",
	 (Magick::instance().chanserv.DEF_MLock(), Magick::instance().chanserv.LCK_MLock()));

    output.erase();
    if (Magick::instance().chanserv.DEF_Keeptopic())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Keeptopic())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/KEEPTOPIC");
	if (Magick::instance().chanserv.LCK_Keeptopic())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Topiclock())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Topiclock())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/TOPICLOCK");
	if (Magick::instance().chanserv.LCK_Topiclock())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Private())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Private())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/PRIVATE");
	if (Magick::instance().chanserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Secureops())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Secureops())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/SECUREOPS");
	if (Magick::instance().chanserv.LCK_Secureops())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Secure())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Secure())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/SECURE");
	if (Magick::instance().chanserv.LCK_Secure())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_NoExpire())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_NoExpire())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/NOEXPIRE");
	if (Magick::instance().chanserv.LCK_NoExpire())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Anarchy())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Anarchy())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/ANARCHY");
	if (Magick::instance().chanserv.LCK_Anarchy())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_KickOnBan())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_KickOnBan())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/KICKONBAN");
	if (Magick::instance().chanserv.LCK_KickOnBan())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Restricted())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Restricted())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/RESTRICTED");
	if (Magick::instance().chanserv.LCK_Restricted())
	    output << IRC_Off;
    }

    if (Magick::instance().chanserv.DEF_Join())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().chanserv.LCK_Join())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "CS_SET/JOIN");
	if (Magick::instance().chanserv.LCK_Join())
	    output << IRC_Off;
    }

    SEND(mynick, source, "OS_SETTINGS/CHAN_OPTIONS", (output));

    output.erase();
    if (Magick::instance().chanserv.LCK_Revenge())
	output << IRC_Bold;
    output << Magick::instance().chanserv.DEF_Revenge();
    if (Magick::instance().chanserv.LCK_Revenge())
	output << IRC_Off;
    SEND(mynick, source, "OS_SETTINGS/CHAN_REVENGE", (output));

    SEND(mynick, source, "OS_SETTINGS/CHAN_ACCESS",
	 (Magick::instance().chanserv.Level_Min(), Magick::instance().chanserv.Level_Max()));
    SEND(mynick, source, "OS_SETTINGS/CHAN_NEWS", (ToHumanTime(Magick::instance().memoserv.News_Expire(), source)));
    ETCB();
}

void OperServ::do_settings_Other(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_settings_Other", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    SEND(mynick, source, "OS_SETTINGS/MISC_INFLIGHT", (ToHumanTime(Magick::instance().memoserv.InFlight(), source)));
    SEND(mynick, source, "OS_SETTINGS/MISC_AKILL1",
	 (ToHumanTime(Magick::instance().operserv.Def_Expire(), source),
	  fmstring("%.2f", Magick::instance().operserv.Akill_Reject())));
    NSEND(mynick, source, "OS_SETTINGS/MISC_AKILL2");
    ::sendV(mynick, source, "%-20s: %s", Magick::instance().commserv.SADMIN_Name().c_str(),
	    ToHumanTime(Magick::instance().operserv.Expire_SAdmin(), source).c_str());
    ::sendV(mynick, source, "%-20s: %s", Magick::instance().commserv.SOP_Name().c_str(),
	    ToHumanTime(Magick::instance().operserv.Expire_Sop(), source).c_str());
    ::sendV(mynick, source, "%-20s: %s", Magick::instance().commserv.ADMIN_Name().c_str(),
	    ToHumanTime(Magick::instance().operserv.Expire_Admin(), source).c_str());
    ::sendV(mynick, source, "%-20s: %s", Magick::instance().commserv.OPER_Name().c_str(),
	    ToHumanTime(Magick::instance().operserv.Expire_Oper(), source).c_str());
    SEND(mynick, source, "OS_SETTINGS/MISC_CLONES",
	 (Magick::instance().operserv.Clone_Limit(), Magick::instance().operserv.Max_Clone()));
    SEND(mynick, source, "OS_SETTINGS/MISC_FLOOD1",
	 (Magick::instance().operserv.Flood_Msgs(), ToHumanTime(Magick::instance().operserv.Flood_Time(), source)));
    SEND(mynick, source, "OS_SETTINGS/MISC_FLOOD2", (ToHumanTime(Magick::instance().operserv.Ignore_Remove(), source)));
    SEND(mynick, source, "OS_SETTINGS/MISC_IGNORE",
	 (ToHumanTime(Magick::instance().operserv.Ignore_Time(), source), Magick::instance().operserv.Ignore_Limit()));
    mstring output;

    if (Magick::instance().commserv.DEF_OpenMemos())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().commserv.LCK_OpenMemos())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS");
	if (Magick::instance().commserv.LCK_OpenMemos())
	    output << IRC_Off;
    }

    if (Magick::instance().commserv.DEF_Private())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().commserv.LCK_Private())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE");
	if (Magick::instance().commserv.LCK_Private())
	    output << IRC_Off;
    }

    if (Magick::instance().commserv.DEF_Secure())
    {
	if (!output.empty())
	    output << ", ";
	if (Magick::instance().commserv.LCK_Secure())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE");
	if (Magick::instance().commserv.LCK_Secure())
	    output << IRC_Off;
    }
    SEND(mynick, source, "OS_SETTINGS/MISC_COMM_OPT", (output));
    ETCB();
}

void OperServ::do_settings_All(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_settings_All", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    do_settings_Config(mynick, source, params);
    do_settings_Nick(mynick, source, params);
    do_settings_Channel(mynick, source, params);
    do_settings_Other(mynick, source, params);
    ETCB();
}

void OperServ::do_clone_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_clone_Add", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    if (params.WordCount(" ") < 5)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();
    mstring amount = params.ExtractWord(4, " ");
    mstring reason = params.ExtractFrom(5, " ");

    if (host.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '!'));
	return;
    }
    else if (host.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '@'));
	return;
    }

    if (!amount.IsNumber() || amount.Contains("."))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }

    unsigned int i, num;
    bool super = (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source));
    // i+1 below because unsigned i will always be >= 0
    for (i = host.size() - 1, num = 0; i + 1 > 0; i--)
    {
	switch (host[i])
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
	     (Magick::instance().getMessage(source, "LIST/CLONE"), Magick::instance().config.Starthresh()));
	return;
    }
    else if (num <= 1)
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH", (Magick::instance().getMessage(source, "LIST/CLONE"), 1));
	return;
    }

    num = atoi(amount.c_str());
    if (num < 1 || num > Magick::instance().operserv.Max_Clone())
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().operserv.Max_Clone()));
	return;
    }

    MLOCK((lck_OperServ, "Clone"));
    if (Magick::instance().operserv.Clone_find(host))
    {
	mstring entry = Magick::instance().operserv.Clone->Entry();

	Magick::instance().operserv.Clone_erase();
	Magick::instance().operserv.Clone_insert(entry, num, reason, source);
	Magick::instance().operserv.stats.i_Clone++;
	SEND(mynick, source, "LIST/CHANGE_LEVEL", (entry, Magick::instance().getMessage(source, "LIST/CLONE"), num));
	LOG(LM_DEBUG, "OPERSERV/CLONE_ADD",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), entry, num));
    }
    else
    {
	Magick::instance().operserv.Clone_insert(host, num, reason, source);
	Magick::instance().operserv.stats.i_Clone++;
	SEND(mynick, source, "LIST/ADD_LEVEL", (host, Magick::instance().getMessage(source, "LIST/CLONE"), num));
	LOG(LM_DEBUG, "OPERSERV/CLONE_ADD",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), host, num));
    }
    ETCB();
}

void OperServ::do_clone_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_clone_Del", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();

    if (host.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '!'));
	return;
    }
    else if (host.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '@'));
	return;
    }

    if (!Magick::instance().operserv.Clone_size())
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/CLONE")));
	return;
    }

    MLOCK((lck_OperServ, "Clone"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int num = atoi(host.c_str());

	if (num < 1 || num > Magick::instance().operserv.Clone_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().operserv.Clone_size()));
	    return;
	}

	Magick::instance().operserv.Clone = find_if(Magick::instance().operserv.Clone_begin(),
		Magick::instance().operserv.Clone_begin(), FindNumberedEntry(num - 1));
	if (Magick::instance().operserv.Clone != Magick::instance().operserv.Clone_end())
	{
	    Magick::instance().operserv.stats.i_Clone++;
	    SEND(mynick, source, "LIST/DEL",
		 (Magick::instance().operserv.Clone->Entry(), Magick::instance().getMessage(source, "LIST/CLONE")));
	    LOG(LM_DEBUG, "OPERSERV/CLONE_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.Clone->Entry()));
	    Magick::instance().operserv.Clone_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS_NUMBER", (num, Magick::instance().getMessage(source, "LIST/CLONE")));
	}
    }
    else
    {
	int count = 0;

	for (Magick::instance().operserv.Clone = Magick::instance().operserv.Clone_begin();
		Magick::instance().operserv.Clone != Magick::instance().operserv.Clone_end(); )
	{
	    if (host.Matches(Magick::instance().operserv.Clone->Entry(), true))
	    {
		LOG(LM_DEBUG, "OPERSERV/CLONE_DEL",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		Magick::instance().operserv.Clone->Entry()));
		// Advances iterator ...
		Magick::instance().operserv.Clone_erase();
		count++;
	    }
	    else
		Magick::instance().operserv.Clone++;
	}

	if (count)
	{
	    Magick::instance().operserv.stats.i_Clone++;
	    SEND(mynick, source, "LIST/DEL_MATCH", (count, host, Magick::instance().getMessage(source, "LIST/CLONE")));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS", (host, Magick::instance().getMessage(source, "LIST/CLONE")));
	}
    }
    ETCB();
}

void OperServ::do_clone_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_clone_List", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

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

    mstring host = "*";

    if (params.WordCount(" ") > 2)
    {
	host = params.ExtractWord(3, " ").LowerCase();

	if (host.Contains("!"))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '!'));
	    return;
	}
	else if (host.Contains("@"))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/CLONE"), '@'));
	    return;
	}
    }

    if (Magick::instance().operserv.Clone_size())
    {
	SEND(mynick, source, "LIST/DISPLAY_MATCH", (host, Magick::instance().getMessage(source, "LIST/CLONE")));
    }
    else
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/CLONE")));
	return;
    }
    unsigned int i = 1;

    MLOCK((lck_OperServ, "Clone"));
    for (Magick::instance().operserv.Clone = Magick::instance().operserv.Clone_begin();
	 Magick::instance().operserv.Clone != Magick::instance().operserv.Clone_end(); Magick::instance().operserv.Clone++)
    {
	if (Magick::instance().operserv.Clone->Entry().Matches(host, true))
	{
	    ::sendV(mynick, source, "%3d. %s (%s)", i, Magick::instance().operserv.Clone->Entry().c_str(),
		    parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
				 mVarArray(Magick::instance().operserv.Clone->Last_Modify_Time().Ago(),
					   Magick::instance().operserv.Clone->Last_Modifier())).c_str());
	    ::sendV(mynick, source, "     [%4d] %s", Magick::instance().operserv.Clone->Value().first,
		    Magick::instance().operserv.Clone->Value().second.c_str());
	    i++;
	}
    }
    ETCB();
}

void OperServ::do_akill_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_akill_Add", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();
    mstring reason = params.ExtractFrom(4, " ");
    unsigned long expire = FromHumanTime(reason.Before(" "));

    if (expire)
    {
	if (params.WordCount(" ") < 5)
	{
	    SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	    return;
	}

	reason = reason.ExtractFrom(2, " ");
    }
    else
	expire = Magick::instance().operserv.Def_Expire();

    if (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source))
    {
	if (expire > Magick::instance().operserv.Expire_SAdmin())
	{
	    SEND(mynick, source, "ERR_SITUATION/AKILLTOOHIGH",
		 (ToHumanTime(Magick::instance().operserv.Expire_SAdmin(), source)));
	    return;
	}
    }
    else if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	     Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source))
    {
	if (expire > Magick::instance().operserv.Expire_Sop())
	{
	    SEND(mynick, source, "ERR_SITUATION/AKILLTOOHIGH",
		 (ToHumanTime(Magick::instance().operserv.Expire_Sop(), source)));
	    return;
	}
    }
    else if (Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
	     Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->IsOn(source))
    {
	if (expire > Magick::instance().operserv.Expire_Admin())
	{
	    SEND(mynick, source, "ERR_SITUATION/AKILLTOOHIGH",
		 (ToHumanTime(Magick::instance().operserv.Expire_Admin(), source)));
	    return;
	}
    }
    else if (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	     Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsOn(source))
    {
	if (expire > Magick::instance().operserv.Expire_Oper())
	{
	    SEND(mynick, source, "ERR_SITUATION/AKILLTOOHIGH",
		 (ToHumanTime(Magick::instance().operserv.Expire_Oper(), source)));
	    return;
	}
    }

    if (host.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/AKILL"), '!'));
	return;
    }

    if (!host.Contains("@"))
    {
	host.prepend("*@");
    }

    unsigned int num;
    int i;
    bool super = (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source));
    for (i = host.size() - 1, num = 0; i >= 0; i--)
    {
	switch (host[static_cast < size_t > (i)])
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
	     (Magick::instance().getMessage(source, "LIST/AKILL"), Magick::instance().config.Starthresh()));
	return;
    }
    else if (num <= 1)
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH", (Magick::instance().getMessage(source, "LIST/AKILL"), 1));
	return;
    }

    {
	MLOCK((lck_OperServ, "Akill"));
	if (Magick::instance().operserv.Akill_find(host))
	{
	    mstring entry = Magick::instance().operserv.Akill->Entry();

	    Magick::instance().operserv.Akill_erase();
	    Magick::instance().operserv.Akill_insert(entry, expire, reason, source);
	    Magick::instance().operserv.stats.i_Akill++;
	    SEND(mynick, source, "LIST/CHANGE_TIME",
		 (entry, Magick::instance().getMessage(source, "LIST/AKILL"), ToHumanTime(expire, source)));
	    ANNOUNCE(mynick, "MISC/AKILL_EXTEND", (source, entry, ToHumanTime(expire, source)));
	    LOG(LM_INFO, "OPERSERV/AKILL_ADD",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), entry, ToHumanTime(expire, source),
		 reason));
	}
	else
	{
	    NickServ::live_t::iterator iter;
	    vector < mstring > killusers;
	    {
		RLOCK((lck_NickServ, lck_live));
		for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
		{
		    map_entry < Nick_Live_t > nlive(iter->second);
		    if (!nlive->IsServices() && nlive->Mask(Nick_Live_t::N_U_P_H).After("!").Matches(host, true))
			killusers.push_back(nlive->Name());
		}
	    }

	    float percent =
		100.0 * static_cast < float > (killusers.size()) / static_cast < float >
		(Magick::instance().nickserv.LiveSize());
	    if (percent > Magick::instance().operserv.Akill_Reject() &&
		!(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source)))
	    {
		SEND(mynick, source, "ERR_SITUATION/AKILLTOOMANY",
		     (fmstring("%.2f", percent), fmstring("%.2f", Magick::instance().operserv.Akill_Reject())));
	    }
	    else
	    {
		Magick::instance().operserv.Akill_insert(host, expire, reason, source);
		Magick::instance().server.AKILL(host, reason, expire, source);
		Magick::instance().operserv.stats.i_Akill++;
		SEND(mynick, source, "LIST/ADD_TIME",
		     (host, Magick::instance().getMessage(source, "LIST/AKILL"), ToHumanTime(expire, source)));
		ANNOUNCE(mynick, "MISC/AKILL_ADD",
			 (source, host, ToHumanTime(expire, source), reason, killusers.size(), fmstring("%.2f", percent)));
		LOG(LM_INFO, "OPERSERV/AKILL_ADD",
		    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), host,
		     ToHumanTime(expire, source), reason));
	    }
	}
    }
    ETCB();
}

void OperServ::do_akill_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_akill_Del", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();

    if (host.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/AKILL"), '!'));
	return;
    }

    if (!Magick::instance().operserv.Akill_size())
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/AKILL")));
	return;
    }

    MLOCK((lck_OperServ, "Akill"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int num = atoi(host.c_str());

	if (num < 1 || num > Magick::instance().operserv.Akill_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().operserv.Akill_size()));
	    return;
	}

	Magick::instance().operserv.Akill = find_if(Magick::instance().operserv.Akill_begin(),
				Magick::instance().operserv.Akill_end(), FindNumberedEntry(num - 1));
	if (Magick::instance().operserv.Akill != Magick::instance().operserv.Akill_end())
	{
	    Magick::instance().operserv.stats.i_Akill++;
	    Magick::instance().server.RAKILL(Magick::instance().operserv.Akill->Entry());
	    SEND(mynick, source, "LIST/DEL",
		 (Magick::instance().operserv.Akill->Entry(), Magick::instance().getMessage(source, "LIST/AKILL")));
	    LOG(LM_INFO, "OPERSERV/AKILL_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.Akill->Entry()));
	    Magick::instance().operserv.Akill_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS_NUMBER", (num, Magick::instance().getMessage(source, "LIST/AKILL")));
	}
    }
    else
    {
	int count = 0;

	for (Magick::instance().operserv.Akill = Magick::instance().operserv.Akill_begin();
		Magick::instance().operserv.Akill != Magick::instance().operserv.Akill_end(); )
	{
	    if (host.Matches(Magick::instance().operserv.Akill->Entry(), true))
	    {
		Magick::instance().server.RAKILL(Magick::instance().operserv.Akill->Entry());
		LOG(LM_INFO, "OPERSERV/AKILL_DEL",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
			 Magick::instance().operserv.Akill->Entry()));
		// Advances iterator ...
		Magick::instance().operserv.Akill_erase();
		count++;
	    }
	    else
	    {
		Magick::instance().operserv.Akill++;
	    }
	}

	if (count)
	{
	    Magick::instance().operserv.stats.i_Akill++;
	    SEND(mynick, source, "LIST/DEL_MATCH", (count, host, Magick::instance().getMessage(source, "LIST/AKILL")));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS", (host, Magick::instance().getMessage(source, "LIST/AKILL")));
	}
    }
    ETCB();
}

void OperServ::do_akill_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_akill_List", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

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

    mstring host = "*";

    if (params.WordCount(" ") > 2)
    {
	host = params.ExtractWord(3, " ").LowerCase();

	if (host.Contains("!"))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/AKILL"), '!'));
	    return;
	}
    }

    if (Magick::instance().operserv.Akill_size())
    {
	SEND(mynick, source, "LIST/DISPLAY_MATCH", (host, Magick::instance().getMessage(source, "LIST/AKILL")));
    }
    else
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/AKILL")));
	return;
    }
    unsigned int i = 1;

    MLOCK((lck_OperServ, "Akill"));
    for (Magick::instance().operserv.Akill = Magick::instance().operserv.Akill_begin();
	 Magick::instance().operserv.Akill != Magick::instance().operserv.Akill_end(); Magick::instance().operserv.Akill++)
    {
	if (Magick::instance().operserv.Akill->Entry().Matches(host, true))
	{
	    ::sendV(mynick, source, "%3d. %s (%s)", i, Magick::instance().operserv.Akill->Entry().c_str(),
		    parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
				 mVarArray(Magick::instance().operserv.Akill->Last_Modify_Time().Ago(),
					   Magick::instance().operserv.Akill->Last_Modifier())).c_str());
	    ::sendV(mynick, source, "     [%s] %s",
		    ToHumanTime(Magick::instance().operserv.Akill->Value().first, source).c_str(),
		    Magick::instance().operserv.Akill->Value().second.c_str());
	    i++;
	}
    }
    ETCB();
}

void OperServ::do_operdeny_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_operdeny_Add", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();
    mstring reason = params.ExtractFrom(4, " ");

    if (!host.Contains("@"))
    {
	if (!Magick::instance().nickserv.IsLive(host))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
	    return;
	}
	host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
    }
    else if (!host.Contains("!"))
    {
	host.prepend("*!");
    }

    unsigned int i, num;

    // i+1 below because unsigned i will always be >= 0
    for (i = host.size() - 1, num = 0; i + 1 > 0; i--)
    {
	switch (host[i])
	{
	case '@':
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
    if (num <= Magick::instance().config.Starthresh())
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH",
	     (Magick::instance().getMessage(source, "LIST/OPERDENY"), Magick::instance().config.Starthresh()));
	return;
    }

    {
	MLOCK((lck_OperServ, "OperDeny"));
	if (Magick::instance().operserv.OperDeny_find(host))
	{
	    Magick::instance().operserv.OperDeny_erase();
	}
	Magick::instance().operserv.OperDeny_insert(host, reason, source);
    }
    Magick::instance().operserv.stats.i_OperDeny++;
    SEND(mynick, source, "LIST/ADD", (host, Magick::instance().getMessage(source, "LIST/OPERDENY")));
    ANNOUNCE(mynick, "MISC/OPERDENY_ADD", (source, host));
    LOG(LM_NOTICE, "OPERSERV/OPERDENY_ADD",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), host, reason));

    NickServ::live_t::iterator iter;
    vector < mstring > killusers;
    {
	map_entry < Committee_t > sadmin;
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()))
	    sadmin = Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name());
	RLOCK((lck_NickServ, lck_live));
	for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
	{
	    map_entry < Nick_Live_t > nlive(iter->second);
	    if (nlive->Mask(Nick_Live_t::N_U_P_H).Matches(host, true))
	    {
		// IF user is recognized and on sadmin, ignore.
		if (Magick::instance().nickserv.IsStored(nlive->Name()) &&
		    Magick::instance().nickserv.GetStored(nlive->Name())->IsOnline() && sadmin != NULL &&
		    sadmin->IsIn(nlive->Name()))
		    continue;
		if (!Magick::instance().server.proto.SVSMODE().empty())
		{
		    nlive->SendMode("-oAa");
		}
		else
		{
		    killusers.push_back(nlive->Name());
		}
	    }
	}
    }
    for (i = 0; i < killusers.size(); i++)
	Magick::instance().server.KILL(Magick::instance().operserv.FirstName(), killusers[i],
				       Magick::instance().getMessage("MISC/KILL_OPERDENY"));

    ETCB();
}

void OperServ::do_operdeny_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_operdeny_Del", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    if (!Magick::instance().operserv.OperDeny_size())
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/OPERDENY")));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();

    MLOCK((lck_OperServ, "OperDeny"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int num = atoi(host.c_str());

	if (num < 1 || num > Magick::instance().operserv.OperDeny_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().operserv.OperDeny_size()));
	    return;
	}

	Magick::instance().operserv.OperDeny = find_if(Magick::instance().operserv.OperDeny_begin(),
			Magick::instance().operserv.OperDeny_end(), FindNumberedEntry(num - 1));
	if (Magick::instance().operserv.OperDeny != Magick::instance().operserv.OperDeny_end())
	{
	    Magick::instance().operserv.stats.i_OperDeny++;
	    SEND(mynick, source, "LIST/DEL",
		 (Magick::instance().operserv.OperDeny->Entry(), Magick::instance().getMessage(source, "LIST/OPERDENY")));
	    LOG(LM_NOTICE, "OPERSERV/OPERDENY_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.OperDeny->Entry()));
	    Magick::instance().operserv.OperDeny_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS_NUMBER", (num, Magick::instance().getMessage(source, "LIST/OPERDENY")));
	}
    }
    else
    {
	if (!host.Contains("@"))
	{
	    if (!Magick::instance().nickserv.IsLive(host))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
		return;
	    }
	    host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.prepend("*!");
	}

	int count = 0;

	for (Magick::instance().operserv.OperDeny = Magick::instance().operserv.OperDeny_begin();
		Magick::instance().operserv.OperDeny != Magick::instance().operserv.OperDeny_end(); )
	{
	    if (host.Matches(Magick::instance().operserv.OperDeny->Entry(), true))
	    {
		LOG(LM_NOTICE, "OPERSERV/OPERDENY_DEL",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
			 Magick::instance().operserv.OperDeny->Entry()));
		// Advances Iterator
		Magick::instance().operserv.OperDeny_erase();
		count++;
	    }
	    else
	    {
		Magick::instance().operserv.OperDeny++;
	    }
	}

	if (count)
	{
	    Magick::instance().operserv.stats.i_OperDeny++;
	    SEND(mynick, source, "LIST/DEL_MATCH", (count, host, Magick::instance().getMessage(source, "LIST/OPERDENY")));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS", (host, Magick::instance().getMessage(source, "LIST/OPERDENY")));
	}
    }
    ETCB();
}

void OperServ::do_operdeny_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_operdeny_List", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

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

    mstring host = "*";

    if (params.WordCount(" ") > 2)
    {
	host = params.ExtractWord(3, " ").LowerCase();

	if (!host.Contains("@"))
	{
	    if (!Magick::instance().nickserv.IsLive(host))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
		return;
	    }
	    host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.prepend("*!");
	}
    }

    if (Magick::instance().operserv.OperDeny_size())
    {
	SEND(mynick, source, "LIST/DISPLAY_MATCH", (host, Magick::instance().getMessage(source, "LIST/OPERDENY")));
    }
    else
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/OPERDENY")));
	return;
    }
    unsigned int i = 1;

    MLOCK((lck_OperServ, "OperDeny"));
    for (Magick::instance().operserv.OperDeny = Magick::instance().operserv.OperDeny_begin();
	 Magick::instance().operserv.OperDeny != Magick::instance().operserv.OperDeny_end();
	 Magick::instance().operserv.OperDeny++)
    {
	if (Magick::instance().operserv.OperDeny->Entry().Matches(host, true))
	{
	    ::sendV(mynick, source, "%3d. %s (%s)", i, Magick::instance().operserv.OperDeny->Entry().c_str(),
		    parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
				 mVarArray(Magick::instance().operserv.OperDeny->Last_Modify_Time().Ago(),
					   Magick::instance().operserv.OperDeny->Last_Modifier())).c_str());
	    ::sendV(mynick, source, "     %s", Magick::instance().operserv.OperDeny->Value().c_str());
	    i++;
	}
    }
    ETCB();
}

void OperServ::do_ignore_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_ignore_Add", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();

    if (!host.Contains("@"))
    {
	if (!Magick::instance().nickserv.IsLive(host))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
	    return;
	}
	host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
    }
    else if (!host.Contains("!"))
    {
	host.prepend("*!");
    }

    unsigned int i, num;
    bool super = (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(source));
    // i+1 below because unsigned i will always be >= 0
    for (i = host.size() - 1, num = 0; i + 1 > 0; i--)
    {
	switch (host[i])
	{
	case '@':
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
	     (Magick::instance().getMessage(source, "LIST/SIGNORE"), Magick::instance().config.Starthresh()));
	return;
    }
    else if (num <= 1)
    {
	SEND(mynick, source, "ERR_SYNTAX/STARTHRESH", (Magick::instance().getMessage(source, "LIST/SIGNORE"), 1));
	return;
    }

    MLOCK((lck_OperServ, "Ignore"));
    if (Magick::instance().operserv.Ignore_find(host))
    {
	Magick::instance().operserv.Ignore_erase();
    }
    Magick::instance().operserv.Ignore_insert(host, true, source);
    Magick::instance().operserv.stats.i_Ignore++;
    SEND(mynick, source, "LIST/ADD", (host, Magick::instance().getMessage(source, "LIST/SIGNORE")));
    LOG(LM_DEBUG, "OPERSERV/IGNORE_ADD", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), host));
    ETCB();
}

void OperServ::do_ignore_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_ignore_Del", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    if (!Magick::instance().operserv.Ignore_size())
    {
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/IGNORE")));
	return;
    }

    mstring host = params.ExtractWord(3, " ").LowerCase();

    MLOCK((lck_OperServ, "Ignore"));
    if (host.IsNumber() && !host.Contains("."))
    {
	unsigned int num = atoi(host.c_str());

	if (num < 1 || num > Magick::instance().operserv.Ignore_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().operserv.Ignore_size()));
	    return;
	}

	Magick::instance().operserv.Ignore = find_if(Magick::instance().operserv.Ignore_begin(),
			Magick::instance().operserv.Ignore_end(), FindNumberedEntry(num - 1));
	if (Magick::instance().operserv.Ignore != Magick::instance().operserv.Ignore_end())
	{
	    Magick::instance().operserv.stats.i_Ignore++;
	    SEND(mynick, source, "LIST/DEL",
		 (Magick::instance().operserv.Ignore->Entry(), Magick::instance().getMessage(source, "LIST/SIGNORE")));
	    LOG(LM_DEBUG, "OPERSERV/IGNORE_DEL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().operserv.Ignore->Entry()));
	    Magick::instance().operserv.Ignore_erase();
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS_NUMBER", (num, Magick::instance().getMessage(source, "LIST/SIGNORE")));
	}
    }
    else
    {
	if (!host.Contains("@"))
	{
	    if (!Magick::instance().nickserv.IsLive(host))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
		return;
	    }
	    host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.prepend("*!");
	}

	int count = 0;

	for (Magick::instance().operserv.Ignore = Magick::instance().operserv.Ignore_begin();
		Magick::instance().operserv.Ignore != Magick::instance().operserv.Ignore_end(); )
	{
	    if (host.Matches(Magick::instance().operserv.Ignore->Entry(), true))
	    {
		LOG(LM_DEBUG, "OPERSERV/IGNORE_DEL",
			(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
			 Magick::instance().operserv.Ignore->Entry()));
		Magick::instance().operserv.Ignore_erase();
		count++;
	    }
	    else
	    {
		Magick::instance().operserv.Ignore++;
	    }
	}

	if (count)
	{
	    Magick::instance().operserv.stats.i_Ignore++;
	    SEND(mynick, source, "LIST/DEL_MATCH", (count, host, Magick::instance().getMessage(source, "LIST/SIGNORE")));
	}
	else
	{
	    SEND(mynick, source, "LIST/NOTEXISTS", (host, Magick::instance().getMessage(source, "LIST/SIGNORE")));
	}
    }
    ETCB();
}

void OperServ::do_ignore_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("OperServ::do_ignore_List", (mynick, source, params));

    mstring message = params.ExtractTo(2, " ").UpperCase();

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

    mstring host = "*";

    if (params.WordCount(" ") > 2)
    {
	host = params.ExtractWord(3, " ").LowerCase();

	if (!host.Contains("@"))
	{
	    if (!Magick::instance().nickserv.IsLive(host))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (host));
		return;
	    }
	    host = Magick::instance().nickserv.GetLive(host)->Mask(Magick::instance().operserv.Ignore_Method());
	}
	else if (!host.Contains("!"))
	{
	    host.prepend("*!");
	}
    }

    unsigned int i = 1;
    bool head = false;

    MLOCK((lck_OperServ, "Ignore"));
    for (Magick::instance().operserv.Ignore = Magick::instance().operserv.Ignore_begin();
	 Magick::instance().operserv.Ignore != Magick::instance().operserv.Ignore_end(); Magick::instance().operserv.Ignore++)
    {
	if (Magick::instance().operserv.Ignore->Entry().Matches(host, true) && Magick::instance().operserv.Ignore->Value())
	{
	    if (head == false)
	    {
		SEND(mynick, source, "LIST/DISPLAY_MATCH", (host, Magick::instance().getMessage(source, "LIST/SIGNORE")));
		head = true;
	    }
	    ::sendV(mynick, source, "%3d. %s (%s)", i, Magick::instance().operserv.Ignore->Entry().c_str(),
		    parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
				 mVarArray(Magick::instance().operserv.Ignore->Last_Modify_Time().Ago(),
					   Magick::instance().operserv.Ignore->Last_Modifier())).c_str());
	    i++;
	}
    }
    if (head == false)
	SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/SIGNORE")));
    ETCB();
}

SXP::Tag OperServ::tag_OperServ("OperServ");
SXP::Tag OperServ::tag_Clone("Clone");
SXP::Tag OperServ::tag_Akill("Akill");
SXP::Tag OperServ::tag_OperDeny("OperDeny");
SXP::Tag OperServ::tag_Ignore("Ignore");

void OperServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    FT("OperServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if (pElement->IsA(tag_Clone))
    {
	Clone_Type *tmp = new Clone_Type;

	c_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_Akill))
    {
	Akill_Type *tmp = new Akill_Type;

	a_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_OperDeny))
    {
	OperDeny_Type *tmp = new OperDeny_Type;

	o_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_Ignore))
    {
	Ignore_Type *tmp = new Ignore_Type;

	i_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }
    ETCB();
}

void OperServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);
    static_cast < void > (pElement);

    FT("OperServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    ETCB();
}

void OperServ::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    FT("OperServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));

    //TODO: Add your source code here
    pOut->BeginObject(tag_OperServ);

    {
	MLOCK((lck_OperServ, "Clone"));
	for_each(i_Clone.begin(), i_Clone.end(), SXP::WriteSubElement(pOut, tag_Clone));
    }

    {
	MLOCK((lck_OperServ, "Akill"));
	for_each(i_Akill.begin(), i_Akill.end(), SXP::WriteSubElement(pOut, tag_Akill));
    }

    {
	MLOCK((lck_OperServ, "OperDeny"));
	for_each(i_OperDeny.begin(), i_OperDeny.end(), SXP::WriteSubElement(pOut, tag_OperDeny));
    }

    {
	set < Ignore_Type >::iterator l;
	SXP::WriteSubElement wse(pOut, tag_Ignore);
	MLOCK((lck_OperServ, "Ignore"));
	for (l = i_Ignore.begin(); l != i_Ignore.end(); l++)
	{
	    // Only save PERM entries
	    if (l->Value())
		wse(*l);
	}
    }

    pOut->EndObject(tag_OperServ);
    ETCB();
}

void OperServ::PostLoad()
{
    BTCB();
    NFT("OperServ::PostLoad");
    // Linkage, etc

    unsigned int i;

    for (i = 0; i < c_array.size(); i++)
    {
	if (c_array[i] != NULL)
	{
	    i_Clone.insert(*c_array[i]);
	    delete c_array[i];
	}
    }
    c_array.clear();

    for (i = 0; i < a_array.size(); i++)
    {
	if (a_array[i] != NULL)
	{
	    i_Akill.insert(*a_array[i]);
	    delete a_array[i];
	}
    }
    a_array.clear();

    for (i = 0; i < o_array.size(); i++)
    {
	if (o_array[i] != NULL)
	{
	    i_OperDeny.insert(*o_array[i]);
	    delete o_array[i];
	}
    }
    o_array.clear();

    for (i = 0; i < i_array.size(); i++)
    {
	if (i_array[i] != NULL)
	{
	    i_Ignore.insert(*i_array[i]);
	    delete i_array[i];
	}
    }
    i_array.clear();

    for_each(Clone_begin(), Clone_end(), mem_fun_ref_void(&entlist_val_pair_t<unsigned int, mstring>::PostLoad));
    for_each(Akill_begin(), Akill_end(), mem_fun_ref_void(&entlist_val_pair_t<unsigned long, mstring>::PostLoad));
    for_each(OperDeny_begin(), OperDeny_end(), mem_fun_ref_void(&entlist_val_t<mstring>::PostLoad));
    for_each(Ignore_begin(), Ignore_end(), mem_fun_ref_void(&entlist_val_t<mstring>::PostLoad));

    ETCB();
}

void OperServ::DumpB() const
{
    BTCB();
    MB(0, (i_Clone.size(), CloneList.size(), i_Akill.size(), i_OperDeny.size(), i_Ignore.size()));
    ETCB();
}

void OperServ::DumpE() const
{
    BTCB();
    ME(0, (i_Clone.size(), CloneList.size(), i_Akill.size(), i_OperDeny.size(), i_Ignore.size()));
    ETCB();
}
