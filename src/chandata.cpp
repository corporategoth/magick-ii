#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation "chanserv.h"
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
#define RCSID(x,y) const char *rcsid_chandata_cpp_ ## x () { return y; }
RCSID(chandata_cpp, "@(#)$Id$");

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

#ifndef MAGICK_HAS_EXCEPTIONS
static Chan_Stored_t GLOB_Chan_Stored_t;
static Chan_Live_t GLOB_Chan_Live_t;
#endif

#ifdef __BORLAND__
#pragma codeseg CODE2
#endif

// Private functions

bool Chan_Live_t::Join(const mstring & nick)
{
    BTCB();
    FT("Chan_Live_t::Join", (nick));

    if_RLOCK ((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"), users.find(nick.LowerCase()) != users.end())
    {
	LOG(LM_WARNING, "ERROR/DUP_CHAN", ("JOIN", nick, i_Name));
	RET(false);
    }
    else
    {
	MCB(users.size());
	{
	    WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"));
	    if (squit.find(nick.LowerCase()) != squit.end())
		squit.erase(nick.LowerCase());
	}
	{
	    WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
	    users[nick.LowerCase()] = triplet < bool, bool, bool > (false, false, false);
	}
	MCE(users.size());
	RET(true);
    }
    ETCB();
}

unsigned int Chan_Live_t::Part(const mstring & nick)
{
    BTCB();
    FT("Chan_Live_t::Part", (nick));

    if_RLOCK ((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"), users.find(nick.LowerCase()) != users.end())
    {
	MCB(users.size());
	CB(1, recent_parts.size());
	if (Magick::instance().nickserv.IsStored(nick))
	{
	    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(nick);
	    if (nstored->IsOnline())
	    {
		mstring target = nstored->Host();

		if (target.empty())
		    target = nstored->Name();
		WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "recent_parts"));
		recent_parts[target.LowerCase()] = mDateTime::CurrentDateTime();
	    }
	}
	{
	    WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
	    users.erase(nick.LowerCase());
	}
	if (!users.size())
	{
	    CB(2, modes);
	    CB(3, i_Limit);
	    CB(4, i_Key);
	    {
		WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"));
		WLOCK2((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Limit"));
		WLOCK3((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Key"));
		modes.erase();
		i_Limit = 0;
		i_Key.erase();
	    }
	    CE(2, modes);
	    CE(3, i_Limit);
	    CE(4, i_Key);
	}
	CE(1, recent_parts.size());
	MCE(users.size());
	if (Magick::instance().chanserv.IsStored(i_Name))
	    Magick::instance().chanserv.GetStored(i_Name)->Part(nick);
    }
    else
    {
	if_RLOCK2 ((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"), squit.find(nick.LowerCase()) != squit.end())
	{
	    MCB(squit.size());
	    {
		WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
		squit.erase(nick.LowerCase());
	    }
	    MCE(squit.size());
	    if (Magick::instance().chanserv.IsStored(i_Name))
		Magick::instance().chanserv.GetStored(i_Name)->Part(nick);
	}
	else
	{
	    LOG(LM_TRACE, "ERROR/REC_FORNOTINCHAN", ("PART", nick, i_Name));
	}
    }

    unsigned int retval = users.size() + squit.size();

    RET(retval);
    ETCB();
}

void Chan_Live_t::SquitUser(const mstring & nick)
{
    BTCB();
    FT("Chan_Live_t::SquitUser", (nick));
    if_RLOCK ((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"), users.find(nick.LowerCase()) != users.end())
    {
	MCB(squit.size());
	CB(1, users.size());
	{
	    WLOCK2((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"));
	    squit[nick.LowerCase()] = users[nick.LowerCase()];
	}
	{
	    WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
	    users.erase(nick.LowerCase());
	}
	CE(1, users.size());
	MCE(squit.size());
    }
    ETCB();
}

void Chan_Live_t::UnSquitUser(const mstring & nick)
{
    BTCB();
    FT("Chan_Live_t::UnSquitUser", (nick));

    // We'll get ALL modes if all users are squit
    if (!users.size())
    {
	MCB(modes);
	{
	    WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"));
	    modes.erase();
	}
	MCE(modes);
    }

    if_RLOCK ((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"), squit.find(nick.LowerCase()) == squit.end())
    {
	LOG(LM_WARNING, "ERROR/REC_FORNOTINCHAN", ("UNSQUIT", nick, i_Name));
    }
    else
	Part(nick);
    ETCB();
}

unsigned int Chan_Live_t::Kick(const mstring & nick, const mstring & kicker)
{
    BTCB();
    FT("Chan_Live_t::Kick", (nick, kicker));
    if_RLOCK ((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"), users.find(nick.LowerCase()) != users.end())
    {
	MCB(users.size());
	{
	    WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
	    users.erase(nick.LowerCase());
	}
	MCE(users.size());
	if (Magick::instance().chanserv.IsStored(i_Name))
	    Magick::instance().chanserv.GetStored(i_Name)->Kick(nick, kicker);
    }
    else
    {
	if_RLOCK2 ((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"), squit.find(nick.LowerCase()) != squit.end())
	{
	    MCB(squit.size());
	    {
		WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"));
		squit.erase(nick.LowerCase());
	    }
	    MCE(squit.size());
	    if (Magick::instance().chanserv.IsStored(i_Name))
		Magick::instance().chanserv.GetStored(i_Name)->Kick(nick, kicker);
	}
	else
	{
	    LOG(LM_WARNING, "ERROR/REC_NOTINCHAN", ("KICK", kicker, nick, i_Name));
	}
    }

    unsigned int retval = users.size() + squit.size();

    RET(retval);
    ETCB();
}

void Chan_Live_t::ChgNick(const mstring & nick, const mstring & newnick)
{
    BTCB();
    FT("Chan_Live_t::ChgNick", (nick, newnick));
    if_RLOCK ((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"), users.find(nick.LowerCase()) != users.end())
    {
	MCB(users.size());
	{
	    WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
	    users[newnick.LowerCase()] = users[nick.LowerCase()];
	    users.erase(nick.LowerCase());
	}
	MCE(users.size());
	if (Magick::instance().chanserv.IsStored(i_Name))
	    Magick::instance().chanserv.GetStored(i_Name)->ChgNick(nick, newnick);
    }
    else
    {
	if_RLOCK2 ((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"), squit.find(nick.LowerCase()) != squit.end())
	{
	    MCB(squit.size());
	    {
		WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"));
		squit[newnick.LowerCase()] = squit[nick.LowerCase()];
		squit.erase(nick.LowerCase());
	    }
	    MCE(squit.size());
	}
	else
	{
	    LOG(LM_WARNING, "ERROR/REC_FORNOTINCHAN", ("NICK", nick, i_Name));
	}
    }
    ETCB();
}

Chan_Live_t::Chan_Live_t() : i_Numeric(0), i_Limit(0), ph_timer(0)
{
    BTCB();
    NFT("Chan_Live_t::Chan_Live_t");
    ref_class::lockData(mVarArray(lck_ChanServ, lck_live, i_Name.LowerCase()));
    DumpB();
    ETCB();
}

Chan_Live_t::Chan_Live_t(const mstring & name, const mstring & first_user, const mDateTime & creation) : i_Name(name),
i_Numeric(0), i_Creation_Time(creation), i_Limit(0), ph_timer(0)
{
    BTCB();
    FT("Chan_Live_t::Chan_Live_t", (name, first_user));
    ref_class::lockData(mVarArray(lck_ChanServ, lck_live, i_Name.LowerCase()));
    users[first_user.LowerCase()] = triplet < bool, bool, bool > (false, false, false);

    DumpB();
    ETCB();
}

Chan_Live_t &Chan_Live_t::operator=(const Chan_Live_t & in)
{
    BTCB();
    NFT("Chan_Live_t::operator=");

    i_Name = in.i_Name;
    ref_class::lockData(mVarArray(lck_ChanServ, lck_live, i_Name.LowerCase()));
    i_Numeric = in.i_Numeric;
    i_Creation_Time = in.i_Creation_Time;

    users = in.users;
    squit = in.squit;
    bans = in.bans;
    exempt = in.exempt;
    i_Topic = in.i_Topic;
    i_Topic_Set_Time = in.i_Topic_Set_Time;
    i_Topic_Setter = in.i_Topic_Setter;
    modes = in.modes;
    i_Key = in.i_Key;
    i_Limit = in.i_Limit;
    p_modes_on = in.p_modes_on;
    p_modes_off = in.p_modes_off;
    p_modes_on_params = in.p_modes_on_params;
    p_modes_off_params = in.p_modes_off_params;
    ph_timer = in.ph_timer;
    recent_parts = in.recent_parts;
    i_UserDef = in.i_UserDef;
    NRET(Chan_Live_t &, *this);
    ETCB();
}

mDateTime Chan_Live_t::Creation_Time() const
{
    BTCB();
    NFT("Chan_Live_t::Creation_Time");
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Creation_Time"));
    RET(i_Creation_Time);
    ETCB();
}

void Chan_Live_t::Creation_Time(const mDateTime & in)
{
    BTCB();
    FT("Chan_Live_t::Creation_Time", (in));
    WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Creation_Time"));
    i_Creation_Time = in;
    ETCB();
}

void Chan_Live_t::Topic(const mstring & source, const mstring & topic, const mstring & setter, const mDateTime & settime)
{
    BTCB();
    FT("Chan_Live_t::Topic", (source, topic, setter, settime));
    {
	WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Topic"));
	WLOCK2((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Topic_Setter"));
	WLOCK3((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Topic_Set_Time"));
	MCB(i_Topic);
	CB(1, i_Topic_Setter);
	CB(2, i_Topic_Set_Time);
	i_Topic = topic;
	i_Topic_Setter = setter;
	i_Topic_Set_Time = settime;
	CE(1, i_Topic_Setter);
	CE(2, i_Topic_Set_Time);
	MCE(i_Topic);
    }
    if (Magick::instance().chanserv.IsStored(i_Name))
	Magick::instance().chanserv.GetStored(i_Name)->Topic(source, topic, setter, settime);
    ETCB();
}

mstring Chan_Live_t::Topic() const
{
    BTCB();
    NFT(("Chan_Live_t::Topic"));
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Topic"));
    RET(i_Topic);
    ETCB();
}

mstring Chan_Live_t::Topic_Setter() const
{
    BTCB();
    NFT(("Chan_Live_t::Topic_Setter"));
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Topic_Setter"));
    RET(i_Topic_Setter);
    ETCB();
}

mDateTime Chan_Live_t::Topic_Set_Time() const
{
    BTCB();
    NFT(("Chan_Live_t::Topic_Set_Time"));
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Topic_Set_Time"));
    RET(i_Topic_Set_Time);
    ETCB();
}

unsigned int Chan_Live_t::Squit() const
{
    BTCB();
    NFT("Chan_Live_t::Squit");
    unsigned int retval = squit.size();

    RET(retval);
    ETCB();
}

void Chan_Live_t::Numeric(const unsigned long in)
{
    BTCB();
    FT("Chan_Live_t::Numeric", (in));
    WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Numeric"));
    MCB(i_Numeric);
    i_Numeric = in;
    MCE(i_Numeric);
    ETCB();
}

unsigned long Chan_Live_t::Numeric() const
{
    BTCB();
    NFT("Chan_Live_t::Numeric");
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Numeric"));
    RET(i_Numeric);
    ETCB();
}

mstring Chan_Live_t::Squit(const unsigned int num) const
{
    BTCB();
    FT("Chan_Live_t::Squit", (num));

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"));
    map<mstring, triplet < bool, bool, bool > >::const_iterator iter = find_if(squit.begin(), squit.end(), FindNumberedEntry(num));
    if (iter != squit.end())
	RET(iter->first);

    RET("");
    ETCB();
}

unsigned int Chan_Live_t::Users() const
{
    BTCB();
    NFT("Chan_Live_t::Users");
    unsigned int retval = users.size();

    RET(retval);
    ETCB();
}

mstring Chan_Live_t::User(const unsigned int num) const
{
    BTCB();
    FT("Chan_Live_t::User", (num));

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    map<mstring, triplet < bool, bool, bool > >::const_iterator iter = find_if(users.begin(), users.end(), FindNumberedEntry(num));
    if (iter != users.end())
	RET(iter->first);

    RET("");
    ETCB();
}

unsigned int Chan_Live_t::Ops() const
{
    BTCB();
    NFT("Chan_Live_t::Ops");
    unsigned int count = 0;

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    map < mstring, triplet < bool, bool, bool > >::const_iterator i;
    for (i = users.begin(); i != users.end(); i++)
	if (i->second.first)
	    count++;
    RET(count);
    ETCB();
}

mstring Chan_Live_t::Op(const unsigned int num) const
{
    BTCB();
    FT("Chan_Live_t::Op", (num));
    unsigned int i;

    map < mstring, triplet < bool, bool, bool > >::const_iterator k;

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    for (i = 0, k = users.begin(); k != users.end(); k++)
	if (k->second.first)
	{
	    if (i == num)
	    {
		RET(k->first);
	    }
	    i++;
	}

    RET("");
    ETCB();
}

unsigned int Chan_Live_t::HalfOps() const
{
    BTCB();
    NFT("Chan_Live_t::HalfOps");
    unsigned int count = 0;

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    map < mstring, triplet < bool, bool, bool > >::const_iterator i;

    for (i = users.begin(); i != users.end(); i++)
	if (i->second.second)
	    count++;
    RET(count);
    ETCB();
}

mstring Chan_Live_t::HalfOp(const unsigned int num) const
{
    BTCB();
    FT("Chan_Live_t::HalfOp", (num));
    unsigned int i;

    map < mstring, triplet < bool, bool, bool > >::const_iterator k;

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    for (i = 0, k = users.begin(); k != users.end(); k++)
	if (k->second.second)
	{
	    if (i == num)
	    {
		RET(k->first);
	    }
	    i++;
	}

    RET("");
    ETCB();
}

unsigned int Chan_Live_t::Voices() const
{
    BTCB();
    NFT("Chan_Live_t::Voices");
    unsigned int count = 0;

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    map < mstring, triplet < bool, bool, bool > >::const_iterator i;

    for (i = users.begin(); i != users.end(); i++)
	if (i->second.third)
	    count++;
    RET(count);
    ETCB();
}

mstring Chan_Live_t::Voice(const unsigned int num) const
{
    BTCB();
    FT("Chan_Live_t::Voice", (num));
    unsigned int i;

    map < mstring, triplet < bool, bool, bool > >::const_iterator k;

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    for (i = 0, k = users.begin(); k != users.end(); k++)
	if (k->second.third)
	{
	    if (i == num)
	    {
		RET(k->first);
	    }
	    i++;
	}
    RET("");
    ETCB();
}

triplet < bool, bool, bool > Chan_Live_t::User(const mstring & name) const
{
    BTCB();
    FT("Chan_Live_t::User", (name));
    if (IsIn(name))
    {
	RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
	map < mstring, triplet < bool, bool, bool > >::const_iterator i = users.find(name.LowerCase());
	NRET(triplet < bool.bool.bool >, i->second);
    }
    else
    {
	triplet < bool, bool, bool > tmp(false, false, false);
	NRET(triplet < bool.bool.bool >, tmp);
    }
    ETCB();
}

unsigned int Chan_Live_t::Bans() const
{
    BTCB();
    NFT("Chan_Live_t::Bans");
    unsigned int retval = bans.size();

    RET(retval);
    ETCB();
}

mstring Chan_Live_t::Ban(const unsigned int num) const
{
    BTCB();
    FT("Chan_Live_t::Ban", (num));

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "bans"));
    map<mstring, mDateTime >::const_iterator iter = find_if(bans.begin(), bans.end(), FindNumberedEntry(num));
    if (iter != bans.end())
	RET(iter->first);

    RET("");
    ETCB();
}

mDateTime Chan_Live_t::Ban(const mstring & mask) const
{
    BTCB();
    FT("Chan_Live_t::Ban", (mask));
    mDateTime retval(0.0);

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "bans"));
    map < mstring, mDateTime >::const_iterator i = bans.find(mask.LowerCase());
    if (i != bans.end())
    {
	retval = i->second;
    }
    RET(retval);
    ETCB();
}

unsigned int Chan_Live_t::Exempts() const
{
    BTCB();
    NFT("Chan_Live_t::Exempts");
    unsigned int retval = exempt.size();

    RET(retval);
    ETCB();
}

mstring Chan_Live_t::Exempt(const unsigned int num) const
{
    BTCB();
    FT("Chan_Live_t::Exempt", (num));

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "exempt"));
    map<mstring, mDateTime >::const_iterator iter = find_if(exempt.begin(), exempt.end(), FindNumberedEntry(num));
    if (iter != exempt.end())
	RET(iter->first);

    RET("");
    ETCB();
}

mDateTime Chan_Live_t::Exempt(const mstring & mask) const
{
    BTCB();
    FT("Chan_Live_t::Exempt", (mask));
    mDateTime retval(0.0);

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "exempt"));
    map < mstring, mDateTime >::const_iterator i = exempt.find(mask.LowerCase());
    if (i != exempt.end())
    {
	retval = i->second;
    }
    RET(retval);
    ETCB();
}

bool Chan_Live_t::IsSquit(const mstring & nick) const
{
    BTCB();
    FT("Chan_Live_t::IsSquit", (nick));
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"));
    bool retval = (squit.find(nick.LowerCase()) != squit.end());

    RET(retval);
    ETCB();
}

bool Chan_Live_t::IsIn(const mstring & nick) const
{
    BTCB();
    FT("Chan_Live_t::IsIn", (nick));
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    bool retval = (users.find(nick.LowerCase()) != users.end());

    RET(retval);
    ETCB();
}

bool Chan_Live_t::IsOp(const mstring & nick) const
{
    BTCB();
    FT("Chan_Live_t::IsOp", (nick));

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    map < mstring, triplet < bool, bool, bool > >::const_iterator i = users.find(nick.LowerCase());

    if (i != users.end() && i->second.first)
    {
	RET(true);
    }
    RET(false);
    ETCB();
}

bool Chan_Live_t::IsHalfOp(const mstring & nick) const
{
    BTCB();
    FT("Chan_Live_t::IsHalfOp", (nick));

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    map < mstring, triplet < bool, bool, bool > >::const_iterator i = users.find(nick.LowerCase());

    if (i != users.end() && i->second.second)
    {
	RET(true);
    }
    RET(false);
    ETCB();
}

bool Chan_Live_t::IsVoice(const mstring & nick) const
{
    BTCB();
    FT("Chan_Live_t::IsVoice", (nick));

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    map < mstring, triplet < bool, bool, bool > >::const_iterator i = users.find(nick.LowerCase());

    if (i != users.end() && i->second.third)
    {
	RET(true);
    }
    RET(false);
    ETCB();
}

bool Chan_Live_t::IsBan(const mstring & mask) const
{
    BTCB();
    FT("Chan_Live_t::IsBan", (mask));
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "bans"));
    bool retval = (bans.find(mask.LowerCase()) != bans.end());

    RET(retval);
    ETCB();
}

bool Chan_Live_t::MatchBan(const mstring & mask) const
{
    BTCB();
    FT("Chan_Live_t::MatchBan", (mask));

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "bans"));
    map<mstring, mDateTime>::const_iterator iter = find_if(bans.begin(), bans.end(), KeyMatches(mask, true));
    RET(iter != bans.end());

    ETCB();
}

bool Chan_Live_t::IsExempt(const mstring & mask) const
{
    BTCB();
    FT("Chan_Live_t::IsExempt", (mask));
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "exempt"));
    bool retval = (exempt.find(mask.LowerCase()) != exempt.end());

    RET(retval);
    ETCB();
}

bool Chan_Live_t::MatchExempt(const mstring & mask) const
{
    BTCB();
    FT("Chan_Live_t::MatchExempt", (mask));

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "exempt"));
    map<mstring, mDateTime>::const_iterator iter = find_if(exempt.begin(), exempt.end(), KeyMatches(mask, true));
    RET(iter != exempt.end());

    ETCB();
}

void Chan_Live_t::LockDown()
{
    BTCB();
    NFT("Chan_Live_t::LockDown");

    if (!Magick::instance().chanserv.IsLive(i_Name) ||
	!Magick::instance().chanserv.GetLive(i_Name)->IsIn(Magick::instance().chanserv.FirstName()))
	Magick::instance().server.JOIN(Magick::instance().chanserv.FirstName(), i_Name);

    // Override the MLOCK checking.
    if (!modes.Contains("s"))
	SendMode("+s");

    MLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "ph_timer"));
    MCB(ph_timer);
    while (Magick::instance().Pause())
	ACE_OS::sleep(1);
    mstring *arg = NULL;
    if (ph_timer && Magick::instance().reactor().cancel_timer(ph_timer, reinterpret_cast < const void ** > (arg)) &&
	arg != NULL)
    {
	delete arg;
    }

    ph_timer = 0;

    bool set_timer = true;
    if (Magick::instance().chanserv.IsStored(i_Name) && 
	!Magick::instance().chanserv.GetStored(i_Name)->Mlock_On().Contains("i") &&
	Magick::instance().chanserv.GetStored(i_Name)->Mlock_Key().empty())
	set_timer = false;
    if (set_timer)
	ph_timer = Magick::instance().reactor().schedule_timer(&(Magick::instance().chanserv.ph), new mstring(i_Name),
						    ACE_Time_Value(Magick::instance().chanserv.ChanKeep()));
    MCE(ph_timer);
    ETCB();
}

void Chan_Live_t::UnLock()
{
    BTCB();
    NFT("Chan_Live_t::UnLock");

    if (modes.Contains("s") && Magick::instance().chanserv.IsStored(i_Name) &&
	!Magick::instance().chanserv.GetStored(i_Name)->Mlock_On().Contains("s"))
	SendMode("-s");

    mstring *arg = NULL;

    MLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "ph_timer"));
    MCB(ph_timer);
    if (ph_timer && Magick::instance().reactor().cancel_timer(ph_timer, reinterpret_cast < const void ** > (arg)) &&
	arg != NULL)
    {
	delete arg;
    }
    ph_timer = 0;
    MCE(ph_timer);
    ETCB();
}

bool Chan_Live_t::ModeExists(const mstring & mode, const vector < mstring > & mode_params, const bool change,
			     const char reqmode, const mstring & reqparam)
{
    BTCB();
    FT("Chan_Live_t::ModeExists", (mode, "vector<mstring> mode_params", change, reqmode, reqparam));
    unsigned int i, param;

    if (reqparam.empty())
	if (mode.Contains(reqmode))
	{
	    RET(true);
	}
	else
	{
	    RET(false);
	}

    for (param = 0, i = 0; i < mode.size(); i++)
    {
	if (Magick::instance().server.proto.ChanModeArg().Contains(mode[i]))
	{
	    if (mode[i] == reqmode && param < mode_params.size() && mode_params[param] == reqparam)
	    {
		RET(true);
	    }
	    if (mode[i] != 'l' || change)
		param++;
	}
	else
	{
	    if (mode[i] == reqmode)
	    {
		RET(true);
	    }
	}
    }
    RET(false);
    ETCB();
}

void Chan_Live_t::RemoveMode(mstring & mode, vector < mstring > & mode_params, const bool change, const char reqmode,
			     const mstring & reqparam)
{
    BTCB();
    FT("Chan_Live_t::RemoveMode", (mode, "vector<mstring> mode_params", change, reqmode, reqparam));
    unsigned int i, param;
    mstring new_mode;

    vector < mstring > new_params;

    for (param = 0, i = 0; i < mode.size(); i++)
    {
	if (Magick::instance().server.proto.ChanModeArg().Contains(mode[i]))
	{
	    if (reqmode == mode[i])
	    {
		// Treat -l like a standard mode ...
		if (mode[i] == 'l' && !change)
		    continue;

 		if (param < mode_params.size() && !reqparam.empty() && mode_params[param] != reqparam)
		{
		    new_mode += mode[i];
		    new_params.push_back(mode_params[param]);
		}
		param++;
	    }
	    else
	    {

		new_mode += mode[i];
		if (mode[i] != 'l' || change)
		{
		    new_params.push_back(mode_params[param]);
		    param++;
		}
	    }
	}
	else
	{
	    if (mode[i] == reqmode)
	    {
		// Do nothing ... we want to delete it!
	    }
	    else
	    {
		new_mode += mode[i];
	    }
	}
    }
    mode = new_mode;
    mode_params = new_params;
    ETCB();
}

void Chan_Live_t::SendMode(const mstring & in)
{
    BTCB();
    FT("Chan_Live_t::SendMode", (in));
    unsigned int i, param = 2;
    mstring mode(in.Before(" "));

    bool add = true;
    bool cstored = false;
    mstring s_key, s_mlock_on, s_mlock_off, mymodes;
    int s_limit = 0;

    {
	RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"));
	mymodes = modes;
    }
    
    if (Magick::instance().chanserv.IsStored(i_Name))
    {
	cstored = true;
	map_entry < Chan_Stored_t > cs = Magick::instance().chanserv.GetStored(i_Name);
	s_key = cs->Mlock_Key();
	s_mlock_on = cs->Mlock_On();
	s_mlock_off = cs->Mlock_Off();
	s_limit = cs->Mlock_Limit();
    }

    {
	WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "p_modes_on"));
	WLOCK2((lck_ChanServ, lck_live, i_Name.LowerCase(), "p_modes_on_params"));
	WLOCK3((lck_ChanServ, lck_live, i_Name.LowerCase(), "p_modes_off"));
	WLOCK4((lck_ChanServ, lck_live, i_Name.LowerCase(), "p_modes_off_params"));
	MCB(p_modes_on);
	CB(1, p_modes_on_params.size());
	CB(2, p_modes_off);
	CB(3, p_modes_off_params.size());
	for (i = 0; i < mode.size(); i++)
	{
	    if (mode[i] == '+')
	    {
		add = true;
	    }
	    else if (mode[i] == '-')
	    {
		add = false;
	    }
	    else if (Magick::instance().server.proto.ChanModeArg().Contains(mode[i]))
	    {
		switch (mode[i])
		{
		case 'o':
		    if (in.WordCount(" ") >= param)
		    {
			mstring arg = in.ExtractWord(param, " ");

			if (add)
			{
			    if (!IsOp(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, mode[i], arg);
				if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
				{
				    p_modes_on += mode[i];
				    p_modes_on_params.push_back(arg);
				}
			    }
			}
			else
			{
			    if (IsOp(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, mode[i], arg);
				if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
				{
				    p_modes_off += mode[i];
				    p_modes_off_params.push_back(arg);
				}
			    }
			}
			param++;
		    }
		    break;

		case 'h':
		    if (in.WordCount(" ") >= param)
		    {
			mstring arg = in.ExtractWord(param, " ");

			if (add)
			{
			    if (!IsHalfOp(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, mode[i], arg);
				if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
				{
				    p_modes_on += mode[i];
				    p_modes_on_params.push_back(arg);
				}
			    }
			}
			else
			{
			    if (IsHalfOp(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, mode[i], arg);
				if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
				{
				    p_modes_off += mode[i];
				    p_modes_off_params.push_back(arg);
				}
			    }
			}
			param++;
		    }
		    break;

		case 'v':
		    if (in.WordCount(" ") >= param)
		    {
			mstring arg = in.ExtractWord(param, " ");

			if (add)
			{
			    if (!IsVoice(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, mode[i], arg);
				if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
				{
				    p_modes_on += mode[i];
				    p_modes_on_params.push_back(arg);
				}
			    }
			}
			else
			{
			    if (IsVoice(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, mode[i], arg);
				if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
				{
				    p_modes_off += mode[i];
				    p_modes_off_params.push_back(arg);
				}
			    }
			}
			param++;
		    }
		    break;

		case 'b':
		    if (in.WordCount(" ") >= param)
		    {
			mstring arg = in.ExtractWord(param, " ");

			if (add)
			{
			    if (!IsBan(arg))
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, mode[i], arg);
				if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
				{
				    p_modes_on += mode[i];
				    p_modes_on_params.push_back(arg);
				}
			    }
			}
			else
			{
			    if (IsBan(arg))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, mode[i], arg);
				if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
				{
				    p_modes_off += mode[i];
				    p_modes_off_params.push_back(arg);
				}
			    }
			}
			param++;
		    }
		    break;

		case 'e':
		    if (in.WordCount(" ") >= param)
		    {
			mstring arg = in.ExtractWord(param, " ");

			if (add)
			{
			    if (!IsExempt(arg))
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, mode[i], arg);
				if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
				{
				    p_modes_on += mode[i];
				    p_modes_on_params.push_back(arg);
				}
			    }
			}
			else
			{
			    if (IsExempt(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, mode[i], arg);
				if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
				{
				    p_modes_off += mode[i];
				    p_modes_off_params.push_back(arg);
				}
			    }
			}
			param++;
		    }
		    break;

		case 'l':
		    if (add)
		    {
			if (!cstored || !s_mlock_off.Contains("l"))
			{
			    if (in.WordCount(" ") >= param)
			    {
				int new_limit = atoi(in.ExtractWord(param, " "));
				if (new_limit > 0 && static_cast<unsigned int>(new_limit) != i_Limit)
				{
				    if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
					RemoveMode(p_modes_off, p_modes_off_params, false, mode[i]);
				    if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
					RemoveMode(p_modes_on, p_modes_on_params, true, mode[i]);
				    p_modes_on += mode[i];
				    p_modes_on_params.push_back(in.ExtractWord(param, " "));
				 }
				 param++;
			    }
			}
		    }
		    else
		    {
			if (!cstored || !s_limit)
			{
			    if (i_Limit)
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
				    RemoveMode(p_modes_on, p_modes_on_params, true, mode[i]);
				if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
				    p_modes_off += mode[i];
			    }
			}
		    }
		    break;
		case 'k':
		    if (in.WordCount(" ") >= param)
		    {
			mstring new_key = in.ExtractWord(param, " ");
			if (add)
			{
			    if (!cstored || s_key == new_key || !s_mlock_off.Contains(mode[i]))
			    {
				// ONLY allow +k if we've turned it off before, or one isnt set
				if (i_Key.empty() || ModeExists(p_modes_off, p_modes_off_params, false, mode[i], i_Key))
				{
				    // DONT take off 'off' value, coz we can -k+k key1 key2
				    if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
				    {
					p_modes_on += mode[i];
					p_modes_on_params.push_back(new_key);
				    }
				}
			    }
			}
			else
			{
			    if (i_Key == new_key)
			    {
				if (!cstored || s_key.empty())
				{
				    if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
					RemoveMode(p_modes_on, p_modes_on_params, true, mode[i]);
				    if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
				    {
					p_modes_off += mode[i];
					p_modes_off_params.push_back(new_key);
				    }
				}
			    }
			}
			param++;
		    }
		    break;
		default:
		    if (in.WordCount(" ") >= param)
		    {
			mstring arg = in.ExtractWord(param, " ");
			if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i], arg))
			    RemoveMode(p_modes_off, p_modes_off_params, false, mode[i], arg);
			if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i], arg))
			{
			    p_modes_on += mode[i];
			    p_modes_on_params.push_back(arg);
			}
			param++;
		    }
		}
	    }
	    else
	    {
		if (add)
		{
		    if (!mymodes.Contains(mode[i]))
		    {
			if (!cstored || !s_mlock_off.Contains(mode[i]))
			{
			    if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
				RemoveMode(p_modes_off, p_modes_off_params, false, mode[i]);
			    if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
				p_modes_on += mode[i];
			}
		    }
		}
		else
		{
		    if (mymodes.Contains(mode[i]))
		    {
			if (!cstored || !s_mlock_on.Contains(mode[i]))
			{
			    if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
				RemoveMode(p_modes_on, p_modes_on_params, true, mode[i]);
			    if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
				p_modes_off += mode[i];
			}
		    }
		}
	    }
	}
	CE(1, p_modes_on_params.size());
	CE(2, p_modes_off);
	CE(3, p_modes_off_params.size());
	MCE(p_modes_on);
    }
    RLOCK2((lck_Events));
    if (Magick::instance().events != NULL)
	Magick::instance().events->AddChannelModePending(i_Name);
    ETCB();
}

void Chan_Live_t::Mode(const mstring & source, const mstring & in)
{
    BTCB();
    FT("Chan_Live_t::Mode", (source, in));

    mstring change(IrcParam(in, 1));
    mstring mymodes, newmode, newmode_param;
    unsigned int fwdargs = 1, i, wc;
    bool add = true;

    {
	RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"));
	mymodes = modes;
    }

    wc = IrcParamCount(in);
    mstring mode_param;
    for (i=2; i<=wc; i++)
	mode_param += " " + IrcParam(in, i);

    wc = mode_param.WordCount(" ");
    CP(("MODE CHANGE (%s): %s", i_Name.c_str(), in.c_str()));

    {
	WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "p_modes_on"));
	WLOCK2((lck_ChanServ, lck_live, i_Name.LowerCase(), "p_modes_on_params"));
	WLOCK3((lck_ChanServ, lck_live, i_Name.LowerCase(), "p_modes_off"));
	WLOCK4((lck_ChanServ, lck_live, i_Name.LowerCase(), "p_modes_off_params"));
	MCB(modes);
	CB(1, p_modes_on);
	CB(2, p_modes_on_params.size());
	CB(3, p_modes_off);
	CB(4, p_modes_off_params.size());
	for (i = 0; i < change.size(); i++)
	{
	    if (change[i] == '+')
	    {
		add = true;
		newmode += change[i];
	    }
	    else if (change[i] == '-')
	    {
		add = false;
		newmode += change[i];
	    }
	    else if (Magick::instance().server.proto.ChanModeArg().Contains(change[i]))
	    {
		mstring arg(mode_param.ExtractWord(fwdargs, " "));

		switch (change[i])
		{
		case 'o':
		    if (fwdargs <= wc)
		    {
			if (IsIn(arg))
			{
			    if (add)
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
				    users[arg.LowerCase()].first = true;
				}
				if (ModeExists(p_modes_on, p_modes_on_params, true, change[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, change[i], arg);
			    }
			    else
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
				    users[arg.LowerCase()].first = false;
				}
				if (ModeExists(p_modes_off, p_modes_off_params, false, change[i], arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, change[i], arg);
			    }
			    newmode += change[i];
			    newmode_param += " " + arg;
			}
			else
			{
			    LOG(LM_WARNING, "ERROR/MODE_NOTINCHAN", (add ? '+' : '-', change[i], source, arg, i_Name));
			}
			fwdargs++;
		    }
		    break;

		case 'h':
		    if (fwdargs <= wc)
		    {
			if (IsIn(arg))
			{
			    if (add)
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
				    users[arg.LowerCase()].second = true;
				}
				if (ModeExists(p_modes_on, p_modes_on_params, true, change[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, change[i], arg);
			    }
			    else
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
				    users[arg.LowerCase()].second = false;
				}
				if (ModeExists(p_modes_off, p_modes_off_params, false, change[i], arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, change[i], arg);
			    }
			    newmode += change[i];
			    newmode_param += " " + arg;
			}
			else
			{
			    LOG(LM_WARNING, "ERROR/MODE_NOTINCHAN", (add ? '+' : '-', change[i], source, arg, i_Name));
			}
			fwdargs++;
		    }
		    break;

		case 'v':
		    if (fwdargs <= wc)
		    {
			if (IsIn(arg))
			{
			    if (add)
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
				    users[arg.LowerCase()].third = true;
				}
				if (ModeExists(p_modes_on, p_modes_on_params, true, change[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, change[i], arg);
			    }
			    else
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
				    users[arg.LowerCase()].third = false;
				}
				if (ModeExists(p_modes_off, p_modes_off_params, false, change[i], arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, change[i], arg);
			    }
			    newmode += change[i];
			    newmode_param += " " + arg;
			}
			else
			{
			    LOG(LM_WARNING, "ERROR/MODE_NOTINCHAN", (add ? '+' : '-', change[i], source, arg, i_Name));
			}
			fwdargs++;
		    }
		    break;

		case 'b':
		    if (fwdargs <= wc)
		    {
			{
			    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "bans"));
			    CB(5, bans.size());
			    if (add)
			    {
				bans[arg.LowerCase()] = mDateTime::CurrentDateTime();
				if (ModeExists(p_modes_on, p_modes_on_params, true, change[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, change[i], arg);
			    }
			    else
			    {
				bans.erase(arg.LowerCase());
				if (ModeExists(p_modes_off, p_modes_off_params, false, change[i], arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, change[i], arg);
			    }
			    CE(5, bans.size());
			}
			newmode += change[i];
			newmode_param += " " + arg;
			fwdargs++;
		    }
		    break;

		case 'e':
		    if (fwdargs <= wc)
		    {
			{
			    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "exempt"));
			    CB(5, exempt.size());
			    if (add)
			    {
				exempt[arg.LowerCase()] = mDateTime::CurrentDateTime();
				if (ModeExists(p_modes_on, p_modes_on_params, true, change[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, change[i], arg);
			    }
			    else
			    {
				exempt.erase(arg.LowerCase());
				if (ModeExists(p_modes_off, p_modes_off_params, false, change[i], arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, change[i], arg);
			    }
			    CE(5, exempt.size());
			}
			newmode += change[i];
			newmode_param += " " + arg;
			fwdargs++;
		    }
		    break;

		case 'k':
		    if (fwdargs <= wc)
		    {
			{
			    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Key"));
			    CB(5, i_Key);
			    if (add)
			    {
				i_Key = arg;
				if (ModeExists(p_modes_on, p_modes_on_params, true, change[i], arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, change[i], arg);
			    }
			    else
			    {
				if (i_Key != arg)
				    LOG(LM_ERROR, "ERROR/KEYMISMATCH", (i_Key, arg, i_Name, source));
				i_Key.erase();
				if (ModeExists(p_modes_off, p_modes_off_params, false, change[i]))
				    RemoveMode(p_modes_off, p_modes_off_params, false, change[i]);
			    }
			    CE(5, i_Key);
			}
			newmode += change[i];
			newmode_param += " " + arg;
			fwdargs++;
		    }
		    break;

		case 'l':
		    if (add)
		    {
			if (fwdargs <= wc)
			{
			    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Limit"));
			    CB(5, i_Limit);
			    if (!arg.IsNumber() || arg.Contains("."))
			    {
				LOG(LM_ERROR, "ERROR/NOLIMIT", (i_Name, source));
				i_Limit = 0;
				newmode += "-" + mstring(change[i]) + "+";
			    }
			    else
			    {
				i_Limit = atoi(arg.c_str());
				newmode += change[i];
				newmode_param += " " + arg;
			    }
			    CE(5, i_Limit);
			    fwdargs++;
			}
		    }
		    else
		    {
			{
			    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Limit"));
			    CB(5, i_Limit);
			    i_Limit = 0;
			    CE(5, i_Limit);
			}
			if (ModeExists(p_modes_off, p_modes_off_params, false, change[i]))
			    RemoveMode(p_modes_off, p_modes_off_params, false, change[i]);
			newmode += change[i];
		    }
		    break;
		default:
		    if (fwdargs <= wc)
		    {
			newmode += change[i];
			newmode_param += " " + arg;
			fwdargs++;
		    }
		}
	    }
	    else
	    {
		if (add && !mymodes.Contains(change[i]))
		{
		    {
			WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"));
			modes += change[i];
		    }
		    if (ModeExists(p_modes_on, p_modes_on_params, true, change[i]))
			RemoveMode(p_modes_on, p_modes_on_params, true, change[i]);
		}
		else
		{
		    if (!add && mymodes.Contains(change[i]))
		    {
			{
			    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"));
			    modes.Remove(change[i]);
			}
			if (ModeExists(p_modes_off, p_modes_off_params, false, change[i]))
			    RemoveMode(p_modes_off, p_modes_off_params, false, change[i]);
		    }
		    else
		    {
			LOG(LM_TRACE, "ERROR/INEFFECT", (add ? '+' : '-', change[i], source, i_Name));
		    }
		}
		newmode += change[i];
	    }
	}
	CE(1, p_modes_on);
	CE(2, p_modes_on_params.size());
	CE(3, p_modes_off);
	CE(4, p_modes_off_params.size());
	MCE(modes);
    }

    if (Magick::instance().chanserv.IsStored(i_Name))
	Magick::instance().chanserv.GetStored(i_Name)->Mode(source, newmode + newmode_param);
    ETCB();
}

bool Chan_Live_t::HasMode(const mstring & in) const
{
    BTCB();
    FT("Chan_Live_t::HasMode", (in));
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"));
    RET(modes.Contains(in));
    ETCB();
}

mstring Chan_Live_t::Mode() const
{
    BTCB();
    NFT("Chan_Live_t::Mode");
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"));
    RET(modes);
    ETCB();
}

mstring Chan_Live_t::Key() const
{
    BTCB();
    NFT("Chan_Live_t::Key");
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Key"));
    RET(i_Key);
    ETCB();
}

unsigned int Chan_Live_t::Limit() const
{
    BTCB();
    NFT("Chan_Live_t::Limit");
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Limit"));
    RET(i_Limit);
    ETCB();
}

mDateTime Chan_Live_t::PartTime(const mstring & nick) const
{
    BTCB();
    FT("Chan_Live_t::PartTime", (nick));
    mDateTime retval(0.0);

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "recent_parts"));
    map < mstring, mDateTime >::const_iterator i = recent_parts.find(nick.LowerCase());
    if (i != recent_parts.end())
    {
	retval = i->second;
    }
    RET(retval);
    ETCB();
}

size_t Chan_Live_t::Usage() const
{
    BTCB();
    size_t retval = 0;

    WLOCK((lck_ChanServ, lck_live, i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_Numeric);
    retval += sizeof(i_Creation_Time.Internal());
    map < mstring, triplet < bool, bool, bool > >::const_iterator i;

    for (i = squit.begin(); i != squit.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second.first);
	retval += sizeof(i->second.second);
	retval += sizeof(i->second.third);
    }
    for (i = users.begin(); i != users.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second.first);
	retval += sizeof(i->second.second);
	retval += sizeof(i->second.third);
    }
    map < mstring, mDateTime >::const_iterator j;
    for (j = bans.begin(); j != bans.end(); j++)
    {
	retval += j->first.capacity();
	retval += sizeof(j->second.Internal());
    }
    for (j = exempt.begin(); j != exempt.end(); j++)
    {
	retval += j->first.capacity();
	retval += sizeof(j->second.Internal());
    }
    retval += i_Topic.capacity();
    retval += i_Topic_Setter.capacity();
    retval += sizeof(i_Topic_Set_Time.Internal());
    retval += modes.capacity();
    retval += sizeof(i_Limit);
    retval += i_Key.capacity();
    retval += p_modes_on.capacity();
    retval += p_modes_off.capacity();
    vector < mstring >::const_iterator k;
    for (k = p_modes_on_params.begin(); k != p_modes_on_params.end(); k++)
    {
	retval += k->capacity();
    }
    for (k = p_modes_off_params.begin(); k != p_modes_off_params.end(); k++)
    {
	retval += k->capacity();
    }
    retval += sizeof(ph_timer);
    for (j = recent_parts.begin(); j != recent_parts.end(); j++)
    {
	retval += j->first.capacity();
	retval += sizeof(j->second.Internal());
    }

    return retval;
    ETCB();
}

void Chan_Live_t::DumpB() const
{
    BTCB();
    MB(0,
       (i_Name, i_Numeric, i_Creation_Time, squit.size(), users.size(), bans.size(), exempt.size(), i_Topic, i_Topic_Setter,
	i_Topic_Set_Time, modes, i_Limit, i_Key, p_modes_on, p_modes_off, p_modes_on_params.size()));
    MB(16, (p_modes_off_params.size(), ph_timer, recent_parts.size()));
    ETCB();
}

void Chan_Live_t::DumpE() const
{
    BTCB();
    ME(0,
       (i_Name, i_Numeric, i_Creation_Time, squit.size(), users.size(), bans.size(), exempt.size(), i_Topic, i_Topic_Setter,
	i_Topic_Set_Time, modes, i_Limit, i_Key, p_modes_on, p_modes_off, p_modes_on_params.size()));
    ME(16, (p_modes_off_params.size(), ph_timer, recent_parts.size()));
    ETCB();
}

// --------- end of Chan_Live_t -----------------------------------

void Chan_Stored_t::ChgAttempt(const mstring & nick, const mstring & newnick)
{
    BTCB();
    FT("Chan_Stored_t::ChgAttempt", (nick, newnick));

    // Create a new one if we find the entry
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "failed_passwds"));
    MCB(failed_passwds.size());
    map < mstring, unsigned int >::iterator iter = failed_passwds.find(nick.LowerCase());
    if (iter != failed_passwds.end())
    {
	failed_passwds[newnick.LowerCase()] = iter->second;
	failed_passwds.erase(nick.LowerCase());
    }
    MCE(failed_passwds.size());
    ETCB();
}

bool Chan_Stored_t::Join(const mstring & nick)
{
    BTCB();
    FT("Chan_Stored_t::Join", (nick));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONUSER", ("JOIN", i_Name, nick));
	RET(false);
    }
    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(nick);

    if (!Magick::instance().chanserv.IsLive(i_Name))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", ("JOIN", nick, i_Name));
	RET(false);
    }
    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(i_Name);
    size_t users = clive->Users();

    if (nlive->IsServices() && Magick::instance().chanserv.FirstName().IsSameAs(nick, true))
    {
	Magick::instance().server.MODE(Magick::instance().operserv.FirstName(), i_Name, "+o " + nick);
	RET(true);
    }

    bool burst = false;

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL)
	    burst = Magick::instance().ircsvchandler->Burst();
    }

    if (Forbidden())
    {
	if (!nlive->HasMode("o"))
	{
	    if (Magick::instance().chanserv.IsLive(i_Name))
	    {
		// This will extend the time we stay in channel if already active ...
		clive->LockDown();

		RLOCK2((lck_ChanServ, lck_stored, i_Name, "setting.Mlock_On"));
		clive->SendMode("+" + setting.Mlock_On + "b " + nlive->AltMask(Magick::instance().operserv.Ignore_Method()));
	    }

	    // Can only insert with reason or default, so its safe.
	    mstring reason = parseMessage(Magick::instance().getMessage(nick, "CS_STATUS/ISFORBIDDEN"), mVarArray(i_Name));

	    Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), nick, i_Name, reason);
	    RET(false);
	}

	mstring modes;

	{
	    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_On"));
	    RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Key"));
	    RLOCK3((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Limit"));
	    modes = setting.Mlock_On;
	    if (!setting.Mlock_Key.empty())
		modes << "k";
	    if (setting.Mlock_Limit)
		modes << "l";
	    if (!setting.Mlock_Key.empty())
		modes << " " << setting.Mlock_Key;
	    if (setting.Mlock_Limit)
		modes << " " << setting.Mlock_Limit;
	}

	if (!modes.empty() && Magick::instance().chanserv.IsLive(i_Name))
	{
	    clive->SendMode("+" + modes + " " + setting.Mlock_Key + " " +
			    (setting.Mlock_Limit ? mstring(setting.Mlock_Limit) : mstring("")));
	}
	RET(true);
    }

    map_entry < Nick_Stored_t > nstored;
    if (Magick::instance().nickserv.IsStored(nick))
	nstored = Magick::instance().nickserv.GetStored(nick);

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
	bool rv = Akick_find(nick, C_IsOn, true);

	if (rv && ((Akick->Entry().Contains("@")) || (nstored != NULL && nstored->IsOnline())))
	{
	    if (Magick::instance().chanserv.IsLive(i_Name))
	    {
		// If this user is the only user in channel
		if (users == 1)
		    clive->LockDown();

		// Committee or stored entry
		if (Akick->Entry() [0u] == '@' || Magick::instance().nickserv.IsStored(Akick->Entry()))
		    clive->SendMode("+b " + nlive->AltMask(Magick::instance().operserv.Ignore_Method()));
		else
		    clive->SendMode("+b " + Akick->Entry());
	    }

	    LOG(LM_DEBUG, "EVENT/AKICK", (nick, i_Name, Akick->Value()));

	    // Can only insert with reason or default, so its safe.
	    Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), nick, i_Name, Akick->Value());

	    RET(false);
	}
    }

    if (Restricted() && !Suspended() && GetAccess(nick) < 1)
    {
	if (Magick::instance().chanserv.IsLive(i_Name))
	{
	    // If this user is the only user in channel
	    if (users == 1)
		clive->LockDown();

	    clive->SendMode("+b " + nlive->AltMask(Magick::instance().operserv.Ignore_Method()));
	}

	LOG(LM_DEBUG, "EVENT/RESTRICTED", (nick, i_Name));

	// Can only insert with reason or default, so its safe.
	Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), nick, i_Name,
				       Magick::instance().getMessage(nick, "MISC/KICK_RESTRICTED"));

	RET(false);
    }

    clive->UnLock();
    if (!Join() && users == 2 && clive->IsIn(Magick::instance().chanserv.FirstName()))
    {
	Magick::instance().server.PART(Magick::instance().chanserv.FirstName(), i_Name);
	users--;
    }

    if (users == 1)
    {
	mstring modes;

	{
	    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_On"));
	    RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Key"));
	    RLOCK3((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Limit"));
	    modes = setting.Mlock_On;
	    if (!setting.Mlock_Key.empty())
		modes << "k";
	    if (setting.Mlock_Limit)
		modes << "l";
	    if (!setting.Mlock_Key.empty())
		modes << " " << setting.Mlock_Key;
	    if (setting.Mlock_Limit)
		modes << " " << setting.Mlock_Limit;
	}

	if (!modes.empty() && Magick::instance().chanserv.IsLive(i_Name))
	    clive->SendMode("+" + modes);

	if (!burst)
	{
	    if (Join())
	    {
		Magick::instance().server.JOIN(Magick::instance().chanserv.FirstName(), i_Name);
		users++;
	    }

	    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic"));
	    RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Setter"));
	    RLOCK3((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Set_Time"));
	    // Carry over topic ..
	    if (Keeptopic() && !i_Topic.empty() && clive->Topic().empty())
	    {
		Magick::instance().server.TOPIC(Magick::instance().chanserv.FirstName(), i_Topic_Setter, i_Name, i_Topic,
						i_Topic_Set_Time);
	    }
	}
    }


    if (GetAccess(nick) > 0)
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_LastUsed"));
	MCB(i_LastUsed);
	i_LastUsed = mDateTime::CurrentDateTime();
	MCE(i_LastUsed);
    }

    // If the access level the user has is above the 'maximum', they're a founder
    if (!Magick::instance().server.proto.FounderMode().empty() && GetAccess(nick) > Magick::instance().chanserv.Level_Max())
    {
	for (unsigned int i = 0; i < Magick::instance().server.proto.FounderMode().length(); i++)
	    clive->SendMode("+" + mstring(Magick::instance().server.proto.FounderMode()[i]) + " " + nick);
    }

    if (Magick::instance().chanserv.IsLive(i_Name))
    {
	if (GetAccess(nick, "AUTOOP"))
	    clive->SendMode("+o " + nick);
	else if (Magick::instance().server.proto.ChanModeArg().Contains('h') && GetAccess(nick, "AUTOHALFOP"))
	    clive->SendMode("+h " + nick);
	else if (GetAccess(nick, "AUTOVOICE"))
	    clive->SendMode("+v " + nick);
    }

    if (Suspended())
    {
	RET(true);
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Message"));
	for (Message = Message_begin(); Message != Message_end(); Message++)
	{
	    if (Magick::instance().nickserv.IsLive(Magick::instance().chanserv.FirstName()))
		Magick::instance().chanserv.notice(nick, "[" + i_Name + "] " + Message->Entry());
	}
    }

    mstring target;

    if (nstored != NULL)
    {
	target = nstored->Host();
	if (target.empty())
	    target = nstored->Name();

	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Greet"));
	if (Greet_find(target) && clive->PartTime(target).SecondsSince() > Parttime())
	{
	    if (Greet->Entry() [0U] == '!')
	    {
		if (Magick::instance().nickserv.IsLive(Magick::instance().chanserv.FirstName()))
		    Magick::instance().chanserv.privmsg(i_Name, "[" + nick + "] " + Greet->Entry().After("!"));
	    }
	    else
	    {
		if (Magick::instance().nickserv.IsLive(Magick::instance().chanserv.FirstName()))
		    Magick::instance().chanserv.privmsg(i_Name, "[" + nick + "] " + Greet->Entry());
	    }
	}
    }

    if (!target.empty() && GetAccess(nick, "READMEMO") && Magick::instance().memoserv.IsChannel(i_Name))
    {
	size_t count = Magick::instance().memoserv.ChannelNewsCount(i_Name, nick);

	if (count)
	    SEND(Magick::instance().memoserv.FirstName(), nick, "MS_STATUS/CS_UNREAD",
		 (i_Name, count, Magick::instance().memoserv.FirstName(), i_Name));
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::Part(const mstring & nick)
{
    BTCB();
    FT("Chan_Stored_t::Part", (nick));

    if (Forbidden())
	return;

    if (Magick::instance().nickserv.IsLive(nick) && Magick::instance().nickserv.GetLive(nick)->IsServices())
	return;

    if (GetAccess(nick) > 0)
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_LastUsed"));
	MCB(i_LastUsed);
	i_LastUsed = mDateTime::CurrentDateTime();
	MCE(i_LastUsed);
    }

    size_t users = 0;

    if (Magick::instance().chanserv.IsLive(i_Name))
	users = Magick::instance().chanserv.GetLive(i_Name)->Users();

    if (users == 1 && Join())
    {
	Magick::instance().server.PART(Magick::instance().chanserv.FirstName(), i_Name);
	users--;
    }

    if (users == 0 && (Mlock_On().Contains("i") || !Mlock_Key().empty()))
    {
	Magick::instance().server.JOIN(Magick::instance().chanserv.FirstName(), i_Name);
	if (Magick::instance().chanserv.IsLive(i_Name))
	{
	    mstring mode("+s");

	    if (Mlock_On().Contains("i"))
		mode << "+i";
	    if (!Mlock_Key().empty())
		mode << "+k " + Mlock_Key();
	    Magick::instance().chanserv.GetLive(i_Name)->SendMode(mode);
	}
    }
    ETCB();
}

void Chan_Stored_t::Kick(const mstring & nick, const mstring & kicker)
{
    BTCB();
    FT("Chan_Stored_t::Kick", (nick, kicker));

    // Users shouldnt kick us, but we just rejoin!
    if (Magick::instance().nickserv.IsLive(nick) && Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	if (Join() || (Forbidden() && (Mlock_On().Contains("i") || !Mlock_Key().empty())))
	    Magick::instance().server.JOIN(nick, i_Name);
	return;
    }

    if (DoRevenge("KICK", kicker, nick))
	Magick::instance().server.INVITE(Magick::instance().chanserv.FirstName(), nick, i_Name);
    ETCB();
}

void Chan_Stored_t::ChgNick(const mstring & nick, const mstring & newnick)
{
    BTCB();
    FT("Chan_Stored_t::ChgNick", (nick, newnick));

    if (!Magick::instance().chanserv.IsLive(i_Name))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", ("NICK", nick, i_Name));
	return;
    }

    if (!Magick::instance().nickserv.IsLiveAll(nick))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONUSER", ("NICK", i_Name, nick));
	return;
    }

    size_t users = 0;

    map_entry < Chan_Live_t > clive;

    if (Magick::instance().chanserv.IsLive(i_Name))
    {
	clive = Magick::instance().chanserv.GetLive(i_Name);
	users = clive->Users();
    }

    // Check we dont now trigger AKICK
    // We supply the OLD nick to check the mask, and then the
    // new nick to check nick only (livelook is off).
    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
	bool rv = (Akick_find(nick, C_IsOn, true) || Akick_find(newnick, C_IsOn));

	if (rv &&
	    ((Akick->Entry().Contains("@")) ||
	     (Magick::instance().nickserv.IsStored(nick) && Magick::instance().nickserv.GetStored(nick)->IsOnline())))
	{
	    if (clive != NULL)
	    {
		// If this user is the only user in channel
		if (users == 1)
		    clive->LockDown();

		if (Akick->Entry() [0u] == '@' || Magick::instance().nickserv.IsLive(Akick->Entry()))
		    clive->SendMode("+b " + Magick::instance().nickserv.GetLive(nick)->AltMask(
							Magick::instance().operserv.Ignore_Method()));
		else
		    clive->SendMode("+b " + Akick->Entry());
	    }

	    LOG(LM_DEBUG, "EVENT/AKICK", (newnick, i_Name, Akick->Value()));

	    // Can only insert with reason or default, so its safe.
	    Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), newnick, i_Name, Akick->Value());

	    return;
	}
    }
    ETCB();
}

void Chan_Stored_t::Quit(const mstring & nick)
{
    BTCB();
    FT("Chan_Stored_t::Quit", (nick));
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "failed_passwds"));
    MCB(failed_passwds.size());
    failed_passwds.erase(nick.LowerCase());
    MCE(failed_passwds.size());
    ETCB();
}

void Chan_Stored_t::Topic(const mstring & source, const mstring & topic, const mstring & setter, const mDateTime & settime)
{
    BTCB();
    FT("Chan_Stored_t::Topic", (source, topic, setter, settime));

    bool burst = false;

    // Still in burst ...
    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->Burst())
	    burst = true;
    }

    // Its us re-setting it!
    if (!source.Contains(".") && Magick::instance().nickserv.IsLive(source) &&
	Magick::instance().nickserv.GetLive(source)->IsServices())
	return;

    if (!Magick::instance().chanserv.IsLive(i_Name))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", ("TOPIC", source, i_Name));
	return;
    }

    if (Suspended())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Comment"));
	Magick::instance().server.TOPIC(Magick::instance().chanserv.FirstName(), Magick::instance().chanserv.FirstName(),
					i_Name,
					"[" + IRC_Bold + Magick::instance().getMessage("VALS/SUSPENDED") + IRC_Off + "] " +
					i_Comment + " [" + IRC_Bold + Magick::instance().getMessage("VALS/SUSPENDED") +
					IRC_Off + "]", settime);
	return;
    }

    if (Topiclock())
    {
	// Lets handle this later ...
	if (burst)
	    return;

	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic"));
	RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Setter"));
	RLOCK3((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Set_Time"));

	// Its already the same ...
	if (i_Topic == topic)
	    return;

	Magick::instance().server.TOPIC(Magick::instance().chanserv.FirstName(), i_Topic_Setter, i_Name, i_Topic,
					settime);
    }
    else
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic"));
	WLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Setter"));
	WLOCK3((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Set_Time"));
	MCB(i_Topic);
	CB(1, i_Topic_Setter);
	CB(2, i_Topic_Set_Time);
	i_Topic = topic;
	i_Topic_Setter = setter;
	i_Topic_Set_Time = settime;
	CE(1, i_Topic_Setter);
	CE(2, i_Topic_Set_Time);
	MCE(i_Topic);
    }
    ETCB();
}

void Chan_Stored_t::SetTopic(const mstring & source, const mstring & setter, const mstring & topic)
{
    BTCB();
    FT("Chan_Stored_t::SetTopic", (source, setter, topic));

    // Its us re-setting it!
    if (!setter.Contains(".") && Magick::instance().nickserv.IsLive(setter) &&
	Magick::instance().nickserv.GetLive(setter)->IsServices())
	return;

    if (!Magick::instance().chanserv.IsLive(i_Name))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", ("TOPIC", source, i_Name));
	return;
    }

    if (Suspended())
	return;

    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic"));
	WLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Setter"));
	WLOCK3((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Set_Time"));
	MCB(i_Topic);
	CB(1, i_Topic_Setter);
	CB(2, i_Topic_Set_Time);
	i_Topic = topic;
	i_Topic_Setter = setter;
	i_Topic_Set_Time = mDateTime::CurrentDateTime();
	CE(1, i_Topic_Setter);
	CE(2, i_Topic_Set_Time);
	MCE(i_Topic);
    }
    Magick::instance().server.TOPIC(source, setter, i_Name, topic,
				    Magick::instance().chanserv.GetLive(i_Name)->Topic_Set_Time());
    ETCB();
}

void Chan_Stored_t::Mode(const mstring & setter, const mstring & mode)
{
    BTCB();
    FT("Chan_Stored_t::Mode", (setter, mode));
    // ENFORCE mlock

    if (!Magick::instance().chanserv.IsLive(i_Name))
    {
	LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", ("MODE", setter, i_Name));
	return;
    }

    if (Magick::instance().nickserv.IsLive(setter) && Magick::instance().nickserv.GetLive(setter)->IsServices())
	return;

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(i_Name);
    mstring out_mode, out_param;
    mstring change(mode.Before(" "));
    unsigned int fwdargs = 1, i, wc;
    bool add = true;

    wc = IrcParamCount(mode);
    mstring mode_param;
    for (i=2; i<=wc; i++)
	mode_param += " " + IrcParam(mode, i);
    wc = mode_param.WordCount(" ");

    for (i = 0; i < change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else if (Magick::instance().server.proto.ChanModeArg().Contains(change[i]))
	{
	    mstring arg = mode_param.ExtractWord(fwdargs, " ");

	    switch (change[i])
	    {
	    case 'o':
		if (fwdargs <= wc)
		{
		    if (add)
		    {
			// If all of the following are true:
			//    The mode was not set on services
			//    The mode was not set on someone with CMDOP or AUTOOP access
			//    The mode was either:
			//       Set by a server, and anarchy is off
			//       Not set by a server, and secureops is on
			// or the mode was set on someone at or below AUTODEOP
			if ((
			     !(Magick::instance().nickserv.IsLive(arg) && Magick::instance().nickserv.GetLive(arg)->IsServices()) &&
			     !(GetAccess(arg, "CMDOP") || GetAccess(arg, "AUTOOP")) &&
			     (setter.Contains(".") ? !Anarchy() : Secureops())
			    ) || GetAccess(arg) <= Level_value("AUTODEOP"))
			{
			    out_mode += "-o";
			    out_param += " " + arg;
			}
		    }
		    else if (!setter.Contains("."))
		    {
			// If user is services or a beneficiary of revenge
			// re-op the user (revenge takes care of the rest)
			if ((Magick::instance().nickserv.IsLive(arg) && Magick::instance().nickserv.GetLive(arg)->IsServices())
			    || DoRevenge("DEOP", setter, arg))
			{
			    out_mode += "+o";
			    out_param += " " + arg;
			}
		    }

		    fwdargs++;
		}
		break;

	    case 'h':
		if (fwdargs <= wc)
		{
		    if (add)
		    {
			// If all of the following are true:
			//    The mode was not set on services
			//    The mode was not set on someone with CMDHALFOP or AUTOHALFOP access
			//    The mode was either:
			//       Set by a server, and anarchy is off
			//       Not set by a server, and secureops is on
			// or the mode was set on someone at or below AUTODEOP
			if ((
			     !(Magick::instance().nickserv.IsLive(arg) && Magick::instance().nickserv.GetLive(arg)->IsServices()) &&
			     !(GetAccess(arg, "CMDHALFOP") || GetAccess(arg, "AUTOHALFOP")) &&
			     (setter.Contains(".") ? !Anarchy() : Secureops())
			    ) || GetAccess(arg) <= Level_value("AUTODEOP"))
			{
			    out_mode += "-h";
			    out_param += " " + arg;
			}
		    }

		    fwdargs++;
		}
		break;

	    case 'v':
		if (fwdargs <= wc)
		{
		    if (add)
		    {
			// If all of the following are true:
			//    The mode was not set on services
			//    The mode was not set on someone with CMDVOICE or AUTOVOICE access
			//    The mode was either:
			//       Set by a server, and anarchy is off
			//       Not set by a server, and secureops is on
			// or the mode was set on someone at or below AUTODEOP
			if ((
			     !(Magick::instance().nickserv.IsLive(arg) && Magick::instance().nickserv.GetLive(arg)->IsServices()) &&
			     !(GetAccess(arg, "CMDVOICE") || GetAccess(arg, "AUTOVOICE")) &&
			     (setter.Contains(".") ? !Anarchy() : Secureops())
			    ) || GetAccess(arg) <= Level_value("AUTODEOP"))
			{
			    out_mode += "-v";
			    out_param += " " + arg;
			}
		    }

		    fwdargs++;
		}
		break;

	    case 'b':
		if (fwdargs <= wc)
		{
		    if (add && !setter.Contains("."))
		    {
			vector < mstring > tobekicked;
			bool DidRevenge = false;
			mstring bantype = "BAN1";
			unsigned int j;

			mstring nick = arg.Before("!");
			mstring user = arg.After("!").Before("@");
			mstring host = arg.After("!").After("@");

			if (host.Contains("*") || host.Contains("?"))
			    bantype = "BAN4";
			else
			    bantype = "BAN3";

			for (j = 0; bantype != "BAN2" && j < user.size(); j++)
			    switch (user[j])
			    {
			    case '*':
			    case '?':
				break;
			    default:
				bantype = "BAN2";
				break;
			    }

			for (j = 0; bantype != "BAN1" && j < nick.size(); j++)
			    switch (nick[j])
			    {
			    case '*':
			    case '?':
				break;
			    default:
				bantype = "BAN1";
				break;
			    }

			for (j = 0; !DidRevenge && j < clive->Users(); j++)
			{
			    map_entry < Nick_Live_t > nlive;
			    if (Magick::instance().nickserv.IsLive(clive->User(j)))
				nlive = Magick::instance().nickserv.GetLive(clive->User(j));
			    if (nlive != NULL &&
				(nlive->Mask(Nick_Live_t::N_U_P_H).Matches(arg, true) ||
				 nlive->AltMask(Nick_Live_t::N_U_P_H).Matches(arg, true)))
			    {
				// Only do revenge or kickonban if user
				// is not exempt from bans (+e).
				if (!
				    (clive->MatchExempt(nlive->Mask(Nick_Live_t::N_U_P_H)) ||
				     clive->MatchExempt(nlive->AltMask(Nick_Live_t::N_U_P_H))))
				{
				    DidRevenge = DoRevenge(bantype, setter, clive->User(j));
				    if (DidRevenge)
				    {
					out_mode += "-b";
					out_param += " " + arg;
				    }
				    else
					tobekicked.push_back(clive->User(j));
				}
			    }
			}
			if (!DidRevenge && KickOnBan())
			{
			    for (j = 0; j < tobekicked.size(); j++)
			    {
				Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), tobekicked[j], i_Name,
							       Magick::instance().chanserv.DEF_Akick_Reason());
			    }
			}
		    }
		    fwdargs++;
		}
		break;

	    case 'k':
		if (fwdargs <= wc)
		{
		    {
			RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Off"));
			RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Key"));
			if (add && setting.Mlock_Off.Contains("k"))
			{
			    out_mode += "-k";
			    out_param += " " + arg;
			}
			else if (!add && !setting.Mlock_Key.empty())
			{
			    out_mode += "+k";
			    out_param += " " + setting.Mlock_Key;
			}
		    }

		    fwdargs++;
		}
		break;

	    case 'l':
		{
		    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Off"));
		    RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Limit"));
		    if (add ? setting.Mlock_Off.Contains("l") : setting.Mlock_Limit)
		    {
			if (add)
			{
			    out_mode += "-l";
			}
			else
			{
			    out_mode += "+l";
			    out_param += " " + setting.Mlock_Limit;
			}
		    }
		}

		if (add && fwdargs <= wc)
		    fwdargs++;
		break;

	    default:
		if (fwdargs <= wc)
		{
		    fwdargs++;
		}
	    }
	}
	else
	{
	    {
		RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Off"));
		RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_On"));
		if (add && setting.Mlock_Off.Contains(change[i]))
		    out_mode += "-" + mstring(change[i]);
		else if (!add && setting.Mlock_On.Contains(change[i]))
		    out_mode += "+" + mstring(change[i]);
	    }
	}
    }
    if (out_mode.size() && Magick::instance().chanserv.IsLive(i_Name))
	clive->SendMode(out_mode + out_param);
    ETCB();
}

void Chan_Stored_t::defaults()
{
    BTCB();
    NFT("Chan_Stored_t::defaults");

    ref_class::lockData(mVarArray(lck_ChanServ, lck_stored, i_Name));
    // Dont lock in here, we locked outside ...
    setting.Mlock_On.erase();
    setting.Mlock_Off.erase();
    setting.Mlock_Key.erase();
    lock.Mlock_On.erase();
    lock.Mlock_Off.erase();
    setting.Mlock_Limit = 0;

    setting.Bantime = Magick::instance().chanserv.DEF_Bantime();
    lock.Bantime = false;
    setting.Parttime = Magick::instance().chanserv.DEF_Parttime();
    lock.Parttime = false;
    setting.Keeptopic = Magick::instance().chanserv.DEF_Keeptopic();
    lock.Keeptopic = false;
    setting.Topiclock = Magick::instance().chanserv.DEF_Topiclock();
    lock.Topiclock = false;
    setting.Private = Magick::instance().chanserv.DEF_Private();
    lock.Private = false;
    setting.Secureops = Magick::instance().chanserv.DEF_Secureops();
    lock.Secureops = false;
    setting.Secure = Magick::instance().chanserv.DEF_Secure();
    lock.Secure = false;
    setting.Anarchy = Magick::instance().chanserv.DEF_Anarchy();
    lock.Anarchy = false;
    setting.KickOnBan = Magick::instance().chanserv.DEF_KickOnBan();
    lock.KickOnBan = false;
    setting.Restricted = Magick::instance().chanserv.DEF_Restricted();
    lock.Restricted = false;
    setting.Join = Magick::instance().chanserv.DEF_Join();
    lock.Join = false;
    setting.Revenge = Magick::instance().chanserv.DEF_Revenge();
    lock.Revenge = false;
    setting.Forbidden = false;
    setting.NoExpire = Magick::instance().chanserv.DEF_NoExpire();

    mstring defaulted = Magick::instance().chanserv.DEF_MLock();
    mstring locked = Magick::instance().chanserv.LCK_MLock();
    bool add = true;
    unsigned int i;

    for (i = 0; i < defaulted.size(); i++)
    {
	if (defaulted[i] == '+')
	{
	    add = true;
	}
	else if (defaulted[i] == '-')
	{
	    add = false;
	}
	else if (!Magick::instance().server.proto.ChanModeArg().Contains(defaulted[i]))
	{
	    if (add)
	    {
		if (!setting.Mlock_On.Contains(defaulted[i]))
		    setting.Mlock_On += defaulted[i];
		if (setting.Mlock_Off.Contains(defaulted[i]))
		    setting.Mlock_Off.Remove(defaulted[i]);
	    }
	    else
	    {
		if (!setting.Mlock_Off.Contains(defaulted[i]))
		    setting.Mlock_Off += defaulted[i];
		if (setting.Mlock_On.Contains(defaulted[i]))
		    setting.Mlock_On.Remove(defaulted[i]);
	    }
	}
    }

    add = true;
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
	else if (!Magick::instance().server.proto.ChanModeArg().Contains(locked[i]))
	{
	    if (add)
	    {
		if (!setting.Mlock_On.Contains(locked[i]))
		    setting.Mlock_On += locked[i];
		if (setting.Mlock_Off.Contains(locked[i]))
		    setting.Mlock_Off.Remove(locked[i]);
	    }
	    else
	    {
		if (!setting.Mlock_Off.Contains(locked[i]))
		    setting.Mlock_Off += locked[i];
		if (setting.Mlock_On.Contains(locked[i]))
		    setting.Mlock_On.Remove(locked[i]);
	    }
	}
    }

    vector < mstring > levels = Magick::instance().chanserv.LVL();
    i_Level.clear();
    for (i = 0; i < levels.size(); i++)
    {
	if (Magick::instance().chanserv.LVL(levels[i]) >= Magick::instance().chanserv.Level_Min())
	    i_Level.insert(entlist_val_t < long >
			   (levels[i], Magick::instance().chanserv.LVL(levels[i]), Magick::instance().chanserv.FirstName()));
    }
    ETCB();
}

bool Chan_Stored_t::DoRevenge(const mstring & i_type, const mstring & target, const mstring & source)
{
    BTCB();
    FT("Chan_Stored_t::DoRevenge", (i_type, target, source));

    if (!
	(Magick::instance().chanserv.IsLive(i_Name) && Magick::instance().nickserv.IsLive(source) &&
	 Magick::instance().nickserv.IsLive(target)))
	RET(false);

    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(i_Name);

    mstring type(i_type);

    if (GetAccess(source) > GetAccess(target))
    {
	mstring tmp;

	{
	    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Revenge"));
	    tmp = setting.Revenge;
	}
	if (tmp == "REVERSE")
	{
	    // All we need to do now is return 'screw em' (true)
	}
	else if (tmp == "DEOP")
	{
	DoRevenge_DeOp:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    clive->SendMode("-o " + target);
	    SEND(Magick::instance().chanserv.FirstName(), target, "MISC/REVENGE", (type, Magick::instance().getLname(source)));
	}
	else if (tmp == "KICK")
	{
	DoRevenge_Kick:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    mstring reason =
		parseMessage(Magick::instance().getMessage(source, "MISC/REVENGE"),
			     mVarArray(type, Magick::instance().getLname(source)));

	    Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), target, i_Name, reason);
	}
	else if (tmp == "BAN1")
	{
	DoRevenge_Ban1:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    clive->SendMode("-o+b " + target + " " + Magick::instance().nickserv.GetLive(target)->AltMask(Nick_Live_t::N));
	    mstring reason =
		parseMessage(Magick::instance().getMessage(source, "MISC/REVENGE"),
			     mVarArray(type, Magick::instance().getLname(source)));

	    Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), target, i_Name, reason);
	}
	else if (tmp == "BAN2")
	{
	DoRevenge_Ban2:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    clive->SendMode("-o+b " + target + " " + Magick::instance().nickserv.GetLive(target)->AltMask(Nick_Live_t::U_H));
	    mstring reason =
		parseMessage(Magick::instance().getMessage(source, "MISC/REVENGE"),
			     mVarArray(type, Magick::instance().getLname(source)));

	    Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), target, i_Name, reason);
	}
	else if (tmp == "BAN3")
	{
	DoRevenge_Ban3:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    clive->SendMode("-o+b " + target + " " + Magick::instance().nickserv.GetLive(target)->AltMask(Nick_Live_t::P_H));
	    mstring reason =
		parseMessage(Magick::instance().getMessage(source, "MISC/REVENGE"),
			     mVarArray(type, Magick::instance().getLname(source)));

	    Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), target, i_Name, reason);
	}
	else if (tmp == "BAN4")
	{
	DoRevenge_Ban4:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    clive->SendMode("-o+b " + target + " " + Magick::instance().nickserv.GetLive(target)->AltMask(Nick_Live_t::H));
	    mstring reason =
		parseMessage(Magick::instance().getMessage(source, "MISC/REVENGE"),
			     mVarArray(type, Magick::instance().getLname(source)));

	    Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), target, i_Name, reason);
	}
	else if (tmp == "MIRROR")
	{
	    if (type == "DEOP")
		goto DoRevenge_DeOp;
	    else if (type == "KICK")
		goto DoRevenge_Kick;
	    else if (type == "BAN1")
		goto DoRevenge_Ban1;
	    else if (type == "BAN2")
		goto DoRevenge_Ban2;
	    else if (type == "BAN3")
		goto DoRevenge_Ban3;
	    else if (type == "BAN4")
		goto DoRevenge_Ban4;
	}
	else
	{
	    // Revenge is OFF or UNKNOWN...
	    RET(false);
	}
	LOG(LM_INFO, "OTHER/REVENGE", (target, i_Name, type, source));
	RET(true);
    }

    RET(false);
    ETCB();
}

Chan_Stored_t::Chan_Stored_t() : i_RegTime(mDateTime::CurrentDateTime()), i_LastUsed(mDateTime::CurrentDateTime())
{
    BTCB();
    NFT("Chan_Stored_t::Chan_Stored_t");

    defaults();
    DumpE();
    ETCB();
}

Chan_Stored_t::Chan_Stored_t(const mstring & name, const mstring & founder, const mstring & password,
			     const mstring & desc) : i_Name(name), i_RegTime(mDateTime::CurrentDateTime()),
i_LastUsed(mDateTime::CurrentDateTime()), i_Founder(founder), i_Description(desc)
{
    BTCB();
    FT("Chan_Stored_t::Chan_Stored_t", (name, founder, password, desc));

    defaults();
    Password(password);
    DumpE();
    ETCB();
}

Chan_Stored_t::Chan_Stored_t(const mstring & name) : i_Name(name), i_RegTime(mDateTime::CurrentDateTime()),
i_LastUsed(mDateTime::CurrentDateTime())
{
    BTCB();
    FT("Chan_Stored_t::Chan_Stored_t", (name));

    defaults();
    setting.Mlock_On += "s";
    setting.Forbidden = true;
    DumpE();
    ETCB();
}

Chan_Stored_t &Chan_Stored_t::operator=(const Chan_Stored_t & in)
{
    BTCB();
    NFT("Chan_Stored_t::operator=");
    i_Name = in.i_Name;
    ref_class::lockData(mVarArray(lck_ChanServ, lck_stored, i_Name.LowerCase()));
    i_RegTime = in.i_RegTime;
    i_LastUsed = in.i_LastUsed;
    i_Founder = in.i_Founder;
    i_CoFounder = in.i_CoFounder;
    i_Description = in.i_Description;
    i_Password = in.i_Password;
    i_URL = in.i_URL;
    i_Comment = in.i_Comment;

    setting.Mlock_On = in.setting.Mlock_On;
    lock.Mlock_On = in.lock.Mlock_On;
    setting.Mlock_Off = in.setting.Mlock_Off;
    lock.Mlock_Off = in.lock.Mlock_Off;
    setting.Mlock_Key = in.setting.Mlock_Key;
    setting.Mlock_Limit = in.setting.Mlock_Limit;
    i_Topic = in.i_Topic;
    i_Topic_Setter = in.i_Topic_Setter;
    i_Topic_Set_Time = in.i_Topic_Set_Time;

    setting.Bantime = in.setting.Bantime;
    lock.Bantime = in.lock.Bantime;
    setting.Parttime = in.setting.Parttime;
    lock.Parttime = in.lock.Parttime;
    setting.Keeptopic = in.setting.Keeptopic;
    lock.Keeptopic = in.lock.Keeptopic;
    setting.Topiclock = in.setting.Topiclock;
    lock.Topiclock = in.lock.Topiclock;
    setting.Private = in.setting.Private;
    lock.Private = in.lock.Private;
    setting.Secureops = in.setting.Secureops;
    lock.Secureops = in.lock.Secureops;
    setting.Secure = in.setting.Secure;
    lock.Secure = in.lock.Secure;
    setting.NoExpire = in.setting.NoExpire;
    setting.Anarchy = in.setting.Anarchy;
    lock.Anarchy = in.lock.Anarchy;
    setting.KickOnBan = in.setting.KickOnBan;
    lock.KickOnBan = in.lock.KickOnBan;
    setting.Restricted = in.setting.Restricted;
    lock.Restricted = in.lock.Restricted;
    setting.Join = in.setting.Join;
    lock.Join = in.lock.Join;
    setting.Revenge = in.setting.Revenge;
    lock.Revenge = in.lock.Revenge;
    i_Suspend_By = in.i_Suspend_By;
    i_Suspend_Time = in.i_Suspend_Time;
    setting.Forbidden = in.setting.Forbidden;
    i_Level = in.i_Level;
    i_Access = in.i_Access;
    i_Akick = in.i_Akick;
    i_Greet = in.i_Greet;
    i_Message = in.i_Message;
    i_UserDef = in.i_UserDef;
    NRET(Chan_Stored_t &, *this);
    ETCB();
}

mDateTime Chan_Stored_t::LastUsed()
{
    BTCB();
    NFT("Chan_Stored_t::LastUsed");

    if (Magick::instance().chanserv.IsLive(i_Name))
    {
	map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(i_Name);
	for (unsigned int i = 0; i < clive->Users(); i++)
	{
	    if (GetAccess(clive->User(i)) > 0)
	    {
		RET(mDateTime::CurrentDateTime());
	    }
	}
    }
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_LastUsed"));
    RET(i_LastUsed);
    ETCB();
}

mDateTime Chan_Stored_t::RegTime() const
{
    BTCB();
    NFT("Chan_Stored_t::RegTime");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_RegTime"));
    RET(i_RegTime);
    ETCB();
}

unsigned long Chan_Stored_t::Drop()
{
    BTCB();
    NFT("Chan_Stored_t::Drop");

    // If I'm in there, get out!
    if (Magick::instance().chanserv.IsLive(i_Name))
    {
	map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(i_Name);
	clive->UnLock();

	if (clive->IsIn(Magick::instance().chanserv.FirstName()))
	    Magick::instance().server.PART(Magick::instance().chanserv.FirstName(), i_Name);
    }

    RET(0u);
    ETCB();
}


void Chan_Stored_t::Founder(const mstring & in)
{
    BTCB();
    FT("Chan_Stored_t::Founder", (in));

    if (!Magick::instance().nickserv.IsStored(in))
    {
	LOG(LM_WARNING, "ERROR/BADSET", (in, "FOUNDER", i_Name));
	return;
    }

    WLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Founder"));
    MCB(i_Founder);
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_CoFounder"));
	if (i_CoFounder.IsSameAs(in, true))
	{
	    CB(1, i_CoFounder);
	    i_CoFounder.erase();
	    CE(1, i_CoFounder);
	}
    }

    i_Founder = in;
    MCE(i_Founder);
    ETCB();
}

mstring Chan_Stored_t::Founder() const
{
    BTCB();
    NFT("Chan_Stored_t::Founder");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Founder"));
    RET(i_Founder);
    ETCB();
}

void Chan_Stored_t::CoFounder(const mstring & in)
{
    BTCB();
    FT("Chan_Stored_t::CoFounder", (in));

    if (in.length() && !Magick::instance().nickserv.IsStored(in))
    {
	LOG(LM_WARNING, "ERROR/BADSET", (in, "COFOUNDER", i_Name));
	return;
    }

    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Founder"));
	if (i_Founder.IsSameAs(in, true))
	    return;
    }

    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_CoFounder"));
    MCB(i_CoFounder);
    i_CoFounder = in;
    MCE(i_CoFounder);
    ETCB();
}

mstring Chan_Stored_t::CoFounder() const
{
    BTCB();
    NFT("Chan_Stored_t::CoFounder");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_CoFounder"));
    RET(i_CoFounder);
    ETCB();
}

void Chan_Stored_t::Description(const mstring & in)
{
    BTCB();
    FT("Chan_Stored_t::Description", (in));
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Description"));
    MCB(i_Description);
    i_Description = in;
    MCE(i_Description);
    ETCB();
}

mstring Chan_Stored_t::Description() const
{
    BTCB();
    NFT("Description(mstring in)	{ i_Description = in;");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Description"));
    RET(i_Description);
    ETCB();
}

void Chan_Stored_t::Password(const mstring & in)
{
    BTCB();
    FT("Chan_Stored_t::Password", (in));
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Password"));
    MCB(i_Password);
#ifdef GETPASS
    i_Password = in;
#else
#if defined(JP2CRYPT)
    char *newpass = new char[(in.length() * 2) + 1];
    mJP2HASH(in.c_str(), in.length(), newpass);
#elif defined(DESCRYPT) || defined(MD5CRYPT)
    char newpass[35];
    mCRYPTHASH(in.c_str(), newpass, NULL);
#else
    char newpass[33];
    mHASH(in.c_str(), in.length(), newpass);
#endif
    i_Password = newpass;
#if defined(JP2CRYPT)
    delete [] newpass;
#endif
#endif
    MCE(i_Password);
    ETCB();
}

mstring Chan_Stored_t::Password() const
{
    BTCB();
    NFT("Chan_Stored_t::Password");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Password"));
    RET(i_Password);
    ETCB();
}

void Chan_Stored_t::Email(const mstring & in)
{
    BTCB();
    FT("Chan_Stored_t::Email", (in));
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Email"));
    MCB(i_Email);
    i_Email = in;
    MCE(i_Email);
    ETCB();
}

mstring Chan_Stored_t::Email() const
{
    BTCB();
    NFT("Chan_Stored_t::Email");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Email"));
    RET(i_Email);
    ETCB();
}

void Chan_Stored_t::URL(const mstring & in)
{
    BTCB();
    FT("Chan_Stored_t::URL", (in));
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_URL"));
    MCB(i_URL);
    i_URL = in;
    MCE(i_URL);
    ETCB();
}

mstring Chan_Stored_t::URL() const
{
    BTCB();
    NFT("Chan_Stored_t::URL");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_URL"));
    RET(i_URL);
    ETCB();
}

void Chan_Stored_t::Comment(const mstring & in)
{
    BTCB();
    FT("Chan_Stored_t::Comment", (in));
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Comment"));
    MCB(i_Comment);
    i_Comment = in;
    MCE(i_Comment);
    ETCB();
}

mstring Chan_Stored_t::Comment() const
{
    BTCB();
    NFT("Chan_Stored_t::Comment");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Comment"));
    RET(i_Comment);
    ETCB();
}

unsigned int Chan_Stored_t::CheckPass(const mstring & nick, const mstring & password)
{
    BTCB();
    FT("Chan_Stored_t::CheckPass", (nick, password));
    unsigned int retval = 0;

    RLOCK ((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Password"));
#ifdef GETPASS
    mstring check(password);
#else
#if defined(JP2CRYPT)
    char *newpass = new char[(password.length() * 2) + 1];
    mJP2HASH(password.c_str(), password.length(), newpass);
#elif defined(DESCRYPT) || defined(MD5CRYPT)
    char newpass[35];
    mCRYPTHASH(password.c_str(), newpass, i_Password.empty() ? NULL : i_Password.c_str());
#else
    char newpass[33];
    mHASH(password.c_str(), password.length(), newpass);
#endif
    mstring check(newpass);
#if defined(JP2CRYPT)
    delete [] newpass;
#endif
#endif
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "failed_passwds"));
    MCB(failed_passwds.size());
    if (i_Password == check)
	failed_passwds.erase(nick.LowerCase());
    else
    {
	if (failed_passwds.find(nick.LowerCase()) == failed_passwds.end())
	    failed_passwds[nick.LowerCase()] = 0;
	retval = ++failed_passwds[nick.LowerCase()];
    }
    MCE(failed_passwds.size());
    RET(retval);
    ETCB();
}

void Chan_Stored_t::Suspend(const mstring & name)
{
    BTCB();
    FT("Chan_Stored_t::Suspend", (name));
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Suspend_By"));
    WLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Suspend_Time"));
    MCB(i_Suspend_By);
    CB(1, i_Suspend_Time);
    i_Suspend_By = name;
    i_Suspend_Time = mDateTime::CurrentDateTime();
    CE(1, i_Suspend_Time);
    MCE(i_Suspend_By);
    ETCB();
}

void Chan_Stored_t::UnSuspend()
{
    BTCB();
    NFT("Chan_Stored_t::UnSuspend");
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Suspend_By"));
    MCB(i_Suspend_By);
    i_Suspend_By.erase();
    MCE(i_Suspend_By);
    ETCB();
}

mstring Chan_Stored_t::Mlock_Off() const
{
    BTCB();
    NFT("Chan_Stored_t::Mlock_Off");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Off"));
    RET(setting.Mlock_Off);
    ETCB();
}

mstring Chan_Stored_t::Mlock_On() const
{
    BTCB();
    NFT("Chan_Stored_t::Mlock_On");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_On"));
    RET(setting.Mlock_On);
    ETCB();
}

mstring Chan_Stored_t::Mlock() const
{
    BTCB();
    NFT("Chan_Stored_t::Mlock");
    mstring Result;

    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Off"));
    RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_On"));
    RLOCK3((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Key"));
    RLOCK4((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Limit"));
    if (!setting.Mlock_On.empty() || !setting.Mlock_Key.empty() || setting.Mlock_Limit)
	Result << "+" << setting.Mlock_On;
    if (!setting.Mlock_Key.empty())
	Result << "k";
    if (setting.Mlock_Limit)
	Result << "l";
    if (!setting.Mlock_Off.empty())
	Result << "-" << setting.Mlock_Off;
    RET(Result);
    ETCB();
}

vector < mstring > Chan_Stored_t::Mlock(const mstring & source, const mstring & mode)
{
    BTCB();
    FT("Chan_Stored_t::Mlock", (source, mode));

    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Off"));
    WLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_On"));
    WLOCK3((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Key"));
    WLOCK4((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Limit"));
    mstring old_key = setting.Mlock_Key;
    MCB(setting.Mlock_On);
    CB(1, setting.Mlock_Off);
    CB(2, setting.Mlock_Key);
    CB(3, setting.Mlock_Limit);
    setting.Mlock_On.erase();
    setting.Mlock_Off.erase();
    setting.Mlock_Key.erase();
    setting.Mlock_Limit = 0;
    vector < mstring > retval;
    mstring ignore, output, change(mode.ExtractWord(1, " "));
    unsigned int i;
    unsigned int fwdargs = 2;
    bool add = true;

    for (i = 0; i < change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else if (setting.Mlock_On.Contains(change[i]) || setting.Mlock_Off.Contains(change[i]))
	{
	    ignore += change[i];
	    if (add && Magick::instance().server.proto.ChanModeArg().Contains(change[i]))
		fwdargs++;
	}
	else if (Magick::instance().server.proto.ChanModeArg().Contains(change[i]))
	{
	    switch (change[i])
	    {
	    case 'k':
		if (!setting.Mlock_Key.empty())
		{
		    ignore += change[i];
		    if (add)
			fwdargs++;
		}
		else if (add)
		{
		    if (fwdargs > mode.WordCount(" "))
		    {
			output.erase();
			output = Magick::instance().getMessage(source, "ERR_SYNTAX/NOKEY");
			retval.push_back(output);
			fwdargs--;
		    }
		    else
		    {
			setting.Mlock_Key = mode.ExtractWord(fwdargs, " ");
		    }
		    fwdargs++;
		}
		else
		{
		     setting.Mlock_Off += change[i];
		}
		break;

	    case 'l':
		if (setting.Mlock_Limit != 0)
		{
		    ignore += change[i];
		    if (add)
			fwdargs++;
		}
		else if (add)
		{
		    if (fwdargs > mode.WordCount(" "))
		    {
			output.erase();
			output = Magick::instance().getMessage(source, "ERR_SYNTAX/NOLIMIT");
			retval.push_back(output);
			fwdargs--;
		    }
		    else if (!mode.ExtractWord(fwdargs, " ").IsNumber() || mode.ExtractWord(fwdargs, " ").Contains("."))
		    {
			output.erase();
			output = Magick::instance().getMessage(source, "ERR_SYNTAX/WHOLENUMBER");
			retval.push_back(output);
		    }
		    else if (atol(mode.ExtractWord(fwdargs, " ")) < 1)
		    {
			output.erase();
			output =
			    parseMessage(Magick::instance().getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
					 mVarArray(1, 32768));
			retval.push_back(output);
		    }
		    else
		    {
			setting.Mlock_Limit = atol(mode.ExtractWord(fwdargs, " ").c_str());
		    }
		    fwdargs++;
		}
		else
		{
		    setting.Mlock_Off += change[i];
		}
		break;
	    default:
		output = parseMessage(Magick::instance().getMessage(source, "ERR_SYNTAX/ARG_MODE"), mVarArray(add ? '+' : '-', change[i]));
		retval.push_back(output);
		if (add)
		    fwdargs++;
	    }
	}
	else
	{
	    if (add)
		setting.Mlock_On += change[i];
	    else
		setting.Mlock_Off += change[i];
	}
    }

    for (i=0; i<ignore.length(); i++)
    {
	output = parseMessage(Magick::instance().getMessage(source, "ERR_SYNTAX/MULTI_MODE"), mVarArray(ignore[i]));
	retval.push_back(output);

	switch (ignore[i])
	{
	case 'k':
	    setting.Mlock_Key.erase();
	    setting.Mlock_Off.Remove(ignore[i]);
	    break;
	case 'l':
	    setting.Mlock_Limit = 0;
	    setting.Mlock_Off.Remove(ignore[i]);
	    break;
	default:
	    setting.Mlock_On.Remove(ignore[i]);
	    setting.Mlock_Off.Remove(ignore[i]);
	    break;
	}
    }

    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Mlock_Off"));
    RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Mlock_On"));
    mstring locked(Magick::instance().chanserv.LCK_MLock() + "+" + lock.Mlock_On + "-" + lock.Mlock_Off);
    mstring override_on;
    mstring override_off;
    mstring forced_on;
    mstring forced_off;

    add = true;
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
		    if (!setting.Mlock_On.Contains(locked[i]))
		    {
			forced_on += locked[i];
			setting.Mlock_On += locked[i];
		    }
		    if (setting.Mlock_Off.Contains(locked[i]))
		    {
			override_off += locked[i];
			setting.Mlock_Off.Remove(locked[i]);
		    }
		}
	    }
	    else
	    {
		if (locked[i] == 'k')
		    setting.Mlock_Key.erase();
		else if (locked[i] == 'l')
		    setting.Mlock_Limit = 0;

		if (locked[i] == 'k' || locked[i] == 'l' || !Magick::instance().server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (!setting.Mlock_Off.Contains(locked[i]))
		    {
			forced_off += locked[i];
			setting.Mlock_Off += locked[i];
		    }
		    if (setting.Mlock_On.Contains(locked[i]))
		    {
			override_on += locked[i];
			setting.Mlock_On.Remove(locked[i]);
		    }
		}
	    }
	}
    }

    mstring output2;

    if (!override_on.empty() || !override_off.empty())
    {
	if (!output2.empty())
	    output2 += "  ";
	output.erase();
	output =
	    parseMessage(Magick::instance().getMessage(source, "CS_COMMAND/MLOCK_REVERSED"),
			 mVarArray((!override_on.empty() ? ("+" + override_on) : mstring("")) +
				   (!override_off.empty() ? ("-" + override_off) : mstring(""))));
	output2 += output;
    }
    if (!forced_on.empty() || !forced_off.empty())
    {
	if (!output2.empty())
	    output2 += "  ";
	output.erase();
	output =
	    parseMessage(Magick::instance().getMessage(source, "CS_COMMAND/MLOCK_FORCED"),
			 mVarArray((!forced_on.empty() ? ("+" + forced_on) : mstring("")) +
				   (!forced_off.empty() ? ("-" + forced_off) : mstring(""))));
	output2 += output;
    }
    if (!output2.empty())
	retval.push_back(output2);

    map_entry < Nick_Live_t > nlive;
    map_entry < Chan_Live_t > clive;
    if (Magick::instance().nickserv.IsLive(source))
	nlive = Magick::instance().nickserv.GetLive(source);
    if (Magick::instance().chanserv.IsLive(i_Name))
	clive = Magick::instance().chanserv.GetLive(i_Name);

    if (!setting.Mlock_On.empty() || !setting.Mlock_Key.empty() || setting.Mlock_Limit || !setting.Mlock_Off.empty())
    {
	mstring modes;

	if (!setting.Mlock_On.empty() || !setting.Mlock_Key.empty() || setting.Mlock_Limit)
	    modes << "+" << setting.Mlock_On;
	if (!setting.Mlock_Key.empty())
	    modes << "k";
	if (setting.Mlock_Limit)
	    modes << "l";
	if (!setting.Mlock_Off.empty())
	    modes << "-" << setting.Mlock_Off;

	if (nlive != NULL)
	{
	    LOG(LM_DEBUG, "CHANSERV/SET",
		(nlive->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("CS_SET/MLOCK"), i_Name, modes));
	}

	if (!setting.Mlock_Key.empty())
	    modes << " " << setting.Mlock_Key;
	if (setting.Mlock_Limit)
	    modes << " " << setting.Mlock_Limit;

	output.erase();
	output = parseMessage(Magick::instance().getMessage(source, "CS_COMMAND/MLOCK_SET"), mVarArray(i_Name, modes));
	retval.push_back(output);

	if (clive != NULL)
	{
	    mstring modes_param;

	    modes.erase();
	    if (old_key != setting.Mlock_Key)
	    {
		modes << "-k";
		modes_param << " " << old_key;
	    }

	    modes << "+";
	    for (i = 0; i < setting.Mlock_On.size(); i++)
	    {
		if (!clive->HasMode(setting.Mlock_On[i]))
		{
		    modes << setting.Mlock_On[i];
		}
	    }
	    modes << "-";
	    for (i = 0; i < setting.Mlock_Off.size(); i++)
	    {
		if (setting.Mlock_Off[i] == 'k' && !clive->Key().empty())
		{
		    modes << "k";
		    modes_param << " " << clive->Key();
		}
		else if (clive->HasMode(setting.Mlock_Off[i]))
		{
		    modes << setting.Mlock_Off[i];
		}
	    }
	    if (!setting.Mlock_Key.empty())
	    {
		modes << "+k";
		modes_param << " " << setting.Mlock_Key;
	    }
	    if (setting.Mlock_Limit)
	    {
		modes << "+l";
		modes_param << " " << setting.Mlock_Limit;
	    }
	    if (modes.length() > 2 && clive != NULL)
		clive->SendMode(modes + modes_param);
	}
    }
    else
    {
	output.erase();
	output = parseMessage(Magick::instance().getMessage(source, "CS_COMMAND/MLOCK_UNSET"), mVarArray(i_Name));
	retval.push_back(output);

	if (Magick::instance().nickserv.IsLive(source))
	{
	    LOG(LM_DEBUG, "CHANSERV/UNSET",
		(nlive->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("CS_SET/MLOCK"), i_Name));
	}
    }
    CE(1, setting.Mlock_Off);
    CE(2, setting.Mlock_Key);
    CE(3, setting.Mlock_Limit);
    MCE(setting.Mlock_On);
    NRET(vector < mstring >, retval);
    ETCB();
}

mstring Chan_Stored_t::L_Mlock() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Mlock");
    mstring Result;

    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Mlock_Off"));
    RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Mlock_On"));
    mstring mode_on(lock.Mlock_On);
    mstring mode_off(lock.Mlock_Off);
    mstring locked(Magick::instance().chanserv.LCK_MLock());
    bool add = true;
    unsigned int i;

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
		    if (!mode_on.Contains(locked[i]))
			mode_on += locked[i];
		    if (mode_off.Contains(locked[i]))
			mode_off.Remove(locked[i]);
		}
	    }
	    else
	    {
		if (locked[i] == 'k' || locked[i] == 'l' || !Magick::instance().server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (!mode_off.Contains(locked[i]))
			mode_off += locked[i];
		    if (mode_on.Contains(locked[i]))
			mode_on.Remove(locked[i]);
		}
	    }
	}
    }

    if (!mode_on.empty())
	Result += "+" + mode_on;
    if (!mode_off.empty())
	Result += "-" + mode_off;
    RET(Result);
    ETCB();
}

vector < mstring > Chan_Stored_t::L_Mlock(const mstring & source, const mstring & mode)
{
    BTCB();
    FT("Chan_Stored_t::L_Mlock", (source, mode));

    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Mlock_Off"));
    WLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Mlock_On"));
    MCB(lock.Mlock_Off);
    CB(1, lock.Mlock_On);
    lock.Mlock_On.erase();
    lock.Mlock_Off.erase();
    vector < mstring > retval;
    mstring output, change(mode.ExtractWord(1, " "));
    bool add = true;
    unsigned int i;

    add = true;
    for (i = 0; i < change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Magick::instance().server.proto.ChanModeArg().Contains(change[i]))
		{
		    if (!lock.Mlock_On.Contains(change[i]))
			lock.Mlock_On += change[i];
		    if (lock.Mlock_Off.Contains(change[i]))
			lock.Mlock_Off.Remove(change[i]);
		}
	    }
	    else
	    {
		if (change[i] == 'l' || change[i] == 'k' || !Magick::instance().server.proto.ChanModeArg().Contains(change[i]))
		{
		    if (!lock.Mlock_Off.Contains(change[i]))
			lock.Mlock_Off += change[i];
		    if (lock.Mlock_On.Contains(change[i]))
			lock.Mlock_On.Remove(change[i]);
		}
	    }
	}
    }

    mstring locked(Magick::instance().chanserv.LCK_MLock());
    mstring override_on;
    mstring override_off;

    add = true;
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
		    if (lock.Mlock_Off.Contains(locked[i]))
		    {
			override_off += locked[i];
			lock.Mlock_Off.Remove(locked[i]);
		    }
		}
	    }
	    else
	    {
		if (locked[i] == 'l' || locked[i] == 'k' || !Magick::instance().server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (lock.Mlock_On.Contains(locked[i]))
		    {
			override_on += locked[i];
			lock.Mlock_On.Remove(locked[i]);
		    }
		}
	    }
	}
    }

    {
	WLOCK3((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Off"));
	WLOCK4((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_On"));
	CB(2, setting.Mlock_Off);
	CB(3, setting.Mlock_On);
	// Have to change the REAL mlock
	for (i = 0; i < lock.Mlock_On.size(); i++)
	{
	    if (!setting.Mlock_On.Contains(lock.Mlock_On[i]))
	    {
		setting.Mlock_On += lock.Mlock_On[i];
	    }
	    if (setting.Mlock_Off.Contains(lock.Mlock_On[i]))
	    {
		setting.Mlock_Off.Remove(lock.Mlock_On[i]);
	    }
	}

	for (i = 0; i < lock.Mlock_Off.size(); i++)
	{
	    if (lock.Mlock_Off[i] == 'k')
	    {
		WLOCK5((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Key"));
		setting.Mlock_Key.erase();
	    }
	    else if (lock.Mlock_Off[i] == 'l')
	    {
		WLOCK5((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Limit"));
		setting.Mlock_Limit = 0;
	    }
	    if (!setting.Mlock_Off.Contains(lock.Mlock_Off[i]))
	    {
		setting.Mlock_Off += lock.Mlock_Off[i];
	    }
	    if (setting.Mlock_On.Contains(lock.Mlock_Off[i]))
	    {
		setting.Mlock_On.Remove(lock.Mlock_Off[i]);
	    }
	}
	CE(2, setting.Mlock_Off);
	CE(3, setting.Mlock_Off);
    }

    if (!override_on.empty() || !override_off.empty())
    {
	output.erase();
	output =
	    parseMessage(Magick::instance().getMessage(source, "CS_COMMAND/MLOCK_REVERSED"),
			 mVarArray((!override_on.empty() ? ("+" + override_on) : mstring("")) +
				   (!override_off.empty() ? ("-" + override_off) : mstring(""))));
	retval.push_back(output);
    }
    if (!lock.Mlock_On.empty() || !lock.Mlock_Off.empty())
    {
	output.erase();
	output =
	    parseMessage(Magick::instance().getMessage(source, "CS_COMMAND/MLOCK_LOCK"),
			 mVarArray(i_Name.c_str(),
				   ((!lock.Mlock_On.empty() ? ("+" + lock.Mlock_On) : mstring("")) +
				    (!lock.Mlock_Off.empty() ? ("-" + lock.Mlock_Off) : mstring("")))));
	retval.push_back(output);
	if (Magick::instance().chanserv.IsLive(i_Name))
	    Magick::instance().chanserv.GetLive(i_Name)->SendMode("+" + setting.Mlock_On + "-" + setting.Mlock_Off);

	LOG(LM_DEBUG, "CHANSERV/LOCK",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("CS_SET/MLOCK"), i_Name,
	     ((!lock.Mlock_On.empty() ? ("+" + lock.Mlock_On) : mstring("")) +
	      (!lock.Mlock_Off.empty() ? ("-" + lock.Mlock_Off) : mstring("")))));
    }
    else
    {
	output.erase();
	output = parseMessage(Magick::instance().getMessage(source, "CS_COMMAND/MLOCK_UNLOCK"), mVarArray(i_Name));
	retval.push_back(output);
	LOG(LM_DEBUG, "CHANSERV/UNLOCK",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("CS_SET/MLOCK"), i_Name));
    }
    CE(1, lock.Mlock_On);
    MCE(lock.Mlock_Off);
    NRET(vector < mstring >, retval);
    ETCB();
}

mstring Chan_Stored_t::Mlock_Key() const
{
    BTCB();
    NFT("Chan_Stored_t::Mlock_Key");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Key"));
    RET(setting.Mlock_Key);
    ETCB();
}

unsigned int Chan_Stored_t::Mlock_Limit() const
{
    BTCB();
    NFT("Chan_Stored_t::Mlock_Limit");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Mlock_Limit"));
    RET(setting.Mlock_Limit);
    ETCB();
}

mstring Chan_Stored_t::Last_Topic() const
{
    BTCB();
    NFT("Chan_Stored_t::Last_Topic");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic"));
    RET(i_Topic);
    ETCB();
}

mstring Chan_Stored_t::Last_Topic_Setter() const
{
    BTCB();
    NFT("Chan_Stored_t::Last_Topic_Setter");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Setter"));
    RET(i_Topic_Setter);
    ETCB();
}

mDateTime Chan_Stored_t::Last_Topic_Set_Time() const
{
    BTCB();
    NFT("Chan_Stored_t::Last_Topic_Set_Time");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Set_Time"));
    RET(i_Topic_Set_Time);
    ETCB();
}

void Chan_Stored_t::Bantime(const unsigned long in)
{
    BTCB();
    FT("Chan_Stored_t::Bantime", (in));
    if (!L_Bantime())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Bantime"));
	MCB(setting.Bantime);
	setting.Bantime = in;
	MCE(setting.Bantime);
    }
    ETCB();
}

unsigned long Chan_Stored_t::Bantime() const
{
    BTCB();
    NFT("Chan_Stored_t::Bantime");
    if (!Magick::instance().chanserv.LCK_Bantime())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Bantime"));
	RET(setting.Bantime);
    }
    RET(Magick::instance().chanserv.DEF_Bantime());
    ETCB();
}

void Chan_Stored_t::L_Bantime(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Bantime", (in));
    if (!Magick::instance().chanserv.LCK_Bantime())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Bantime"));
	MCB(lock.Bantime);
	lock.Bantime = in;
	MCE(lock.Bantime);
    }
    ETCB();
}

bool Chan_Stored_t::L_Bantime() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Bantime");
    if (!Magick::instance().chanserv.LCK_Bantime())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Bantime"));
	RET(lock.Bantime);
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::Parttime(const unsigned long in)
{
    BTCB();
    FT("Chan_Stored_t::Parttime", (in));
    if (!L_Parttime())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Parttime"));
	MCB(setting.Parttime);
	setting.Parttime = in;
	MCE(setting.Parttime);
    }
    ETCB();
}

unsigned long Chan_Stored_t::Parttime() const
{
    BTCB();
    NFT("Chan_Stored_t::Parttime");
    if (!Magick::instance().chanserv.LCK_Parttime())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Parttime"));
	RET(setting.Parttime);
    }
    RET(Magick::instance().chanserv.DEF_Parttime());
    ETCB();
}

void Chan_Stored_t::L_Parttime(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Parttime", (in));
    if (!Magick::instance().chanserv.LCK_Parttime())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Parttime"));
	MCB(lock.Parttime);
	lock.Parttime = in;
	MCE(lock.Parttime);
    }
    ETCB();
}

bool Chan_Stored_t::L_Parttime() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Parttime");
    if (!Magick::instance().chanserv.LCK_Parttime())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Parttime"));
	RET(lock.Parttime);
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::Keeptopic(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::Keeptopic", (in));
    if (!L_Keeptopic())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Keeptopic"));
	MCB(setting.Keeptopic);
	setting.Keeptopic = in;
	MCE(setting.Keeptopic);
    }
    ETCB();
}

bool Chan_Stored_t::Keeptopic() const
{
    BTCB();
    NFT("Chan_Stored_t::Keeptopic");
    if (!Magick::instance().chanserv.LCK_Keeptopic())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Keeptopic"));
	RET(setting.Keeptopic);
    }
    RET(Magick::instance().chanserv.DEF_Keeptopic());
    ETCB();
}

void Chan_Stored_t::L_Keeptopic(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Keeptopic", (in));
    if (!Magick::instance().chanserv.LCK_Keeptopic())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Keeptopic"));
	MCB(lock.Keeptopic);
	lock.Keeptopic = in;
	MCE(lock.Keeptopic);
    }
    ETCB();
}

bool Chan_Stored_t::L_Keeptopic() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Keeptopic");
    if (!Magick::instance().chanserv.LCK_Keeptopic())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Keeptopic"));
	RET(lock.Keeptopic);
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::Topiclock(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::Topiclock", (in));
    if (!L_Topiclock())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Topiclock"));
	MCB(setting.Topiclock);
	setting.Topiclock = in;
	MCE(setting.Topiclock);
    }
    ETCB();
}

bool Chan_Stored_t::Topiclock() const
{
    BTCB();
    NFT("Chan_Stored_t::Topiclock");
    if (!Magick::instance().chanserv.LCK_Topiclock())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Topiclock"));
	RET(setting.Topiclock);
    }
    RET(Magick::instance().chanserv.DEF_Topiclock());
    ETCB();
}

void Chan_Stored_t::L_Topiclock(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Topiclock", (in));
    if (!Magick::instance().chanserv.LCK_Topiclock())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Topiclock"));
	MCB(lock.Topiclock);
	lock.Topiclock = in;
	MCE(lock.Topiclock);
    }
    ETCB();
}

bool Chan_Stored_t::L_Topiclock() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Topiclock");
    if (!Magick::instance().chanserv.LCK_Topiclock())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Topiclock"));
	RET(lock.Topiclock);
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::Private(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::Private", (in));
    if (!L_Private())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Private"));
	MCB(setting.Private);
	setting.Private = in;
	MCE(setting.Private);
    }
    ETCB();
}

bool Chan_Stored_t::Private() const
{
    BTCB();
    NFT("Chan_Stored_t::Private");
    if (!Magick::instance().chanserv.LCK_Private())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Private"));
	RET(setting.Private);
    }
    RET(Magick::instance().chanserv.DEF_Private());
    ETCB();
}

void Chan_Stored_t::L_Private(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Private", (in));
    if (!Magick::instance().chanserv.LCK_Private())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Private"));
	MCB(lock.Private);
	lock.Private = in;
	MCE(lock.Private);
    }
    ETCB();
}

bool Chan_Stored_t::L_Private() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Private");
    if (!Magick::instance().chanserv.LCK_Private())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Private"));
	RET(lock.Private);
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::Secureops(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::Secureops", (in));
    if (!L_Secureops())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Secureops"));
	MCB(setting.Secureops);
	setting.Secureops = in;
	MCE(setting.Secureops);
    }
    ETCB();
}

bool Chan_Stored_t::Secureops() const
{
    BTCB();
    NFT("Chan_Stored_t::Secureops");
    if (!Magick::instance().chanserv.LCK_Secureops())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Secureops"));
	RET(setting.Secureops);
    }
    RET(Magick::instance().chanserv.DEF_Secureops());
    ETCB();
}

void Chan_Stored_t::L_Secureops(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Secureops", (in));
    if (!Magick::instance().chanserv.LCK_Secureops())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Secureops"));
	MCB(lock.Secureops);
	lock.Secureops = in;
	MCE(lock.Secureops);
    }
    ETCB();
}

bool Chan_Stored_t::L_Secureops() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Secureops");
    if (!Magick::instance().chanserv.LCK_Secureops())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Secureops"));
	RET(lock.Secureops);
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::Secure(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::Secure", (in));
    if (!L_Secure())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Secure"));
	MCB(setting.Secure);
	setting.Secure = in;
	MCE(setting.Secure);
    }
    ETCB();
}

bool Chan_Stored_t::Secure() const
{
    BTCB();
    NFT("Chan_Stored_t::Secure");
    if (!Magick::instance().chanserv.LCK_Secure())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Secure"));
	RET(setting.Secure);
    }
    RET(Magick::instance().chanserv.DEF_Secure());
    ETCB();
}

void Chan_Stored_t::L_Secure(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Secure", (in));
    if (!Magick::instance().chanserv.LCK_Secure())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Secure"));
	MCB(lock.Secure);
	lock.Secure = in;
	MCE(lock.Secure);
    }
    ETCB();
}

bool Chan_Stored_t::L_Secure() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Secure");
    if (!Magick::instance().chanserv.LCK_Secure())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Secure"));
	RET(lock.Secure);
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::NoExpire(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::NoExpire", (in));
    if (!Magick::instance().chanserv.LCK_NoExpire())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.NoExpire"));
	MCB(setting.NoExpire);
	setting.NoExpire = in;
	MCE(setting.NoExpire);
    }
    ETCB();
}

bool Chan_Stored_t::NoExpire() const
{
    BTCB();
    NFT("Chan_Stored_t::NoExpire");
    if (!Magick::instance().chanserv.LCK_NoExpire())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.NoExpire"));
	RET(setting.NoExpire);
    }
    RET(Magick::instance().chanserv.DEF_NoExpire());
    ETCB();
}

void Chan_Stored_t::Anarchy(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::Anarchy", (in));
    if (!L_Anarchy())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Anarchy"));
	MCB(setting.Anarchy);
	setting.Anarchy = in;
	MCE(setting.Anarchy);
    }
    ETCB();
}

bool Chan_Stored_t::Anarchy() const
{
    BTCB();
    NFT("Chan_Stored_t::Anarchy");
    if (!Magick::instance().chanserv.LCK_Anarchy())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Anarchy"));
	RET(setting.Anarchy);
    }
    RET(Magick::instance().chanserv.DEF_Anarchy());
    ETCB();
}

void Chan_Stored_t::L_Anarchy(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Anarchy", (in));
    if (!Magick::instance().chanserv.LCK_Anarchy())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Anarchy"));
	MCB(lock.Anarchy);
	lock.Anarchy = in;
	MCE(lock.Anarchy);
    }
    ETCB();
}

bool Chan_Stored_t::L_Anarchy() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Anarchy");
    if (!Magick::instance().chanserv.LCK_Anarchy())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Anarchy"));
	RET(lock.Anarchy);
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::KickOnBan(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::KickOnBan", (in));
    if (!L_KickOnBan())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.KickOnBan"));
	MCB(setting.KickOnBan);
	setting.KickOnBan = in;
	MCE(setting.KickOnBan);
    }
    ETCB();
}

bool Chan_Stored_t::KickOnBan() const
{
    BTCB();
    NFT("Chan_Stored_t::KickOnBan");
    if (!Magick::instance().chanserv.LCK_KickOnBan())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.KickOnBan"));
	RET(setting.KickOnBan);
    }
    RET(Magick::instance().chanserv.DEF_KickOnBan());
    ETCB();
}

void Chan_Stored_t::L_KickOnBan(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_KickOnBan", (in));
    if (!Magick::instance().chanserv.LCK_KickOnBan())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.KickOnBan"));
	MCB(lock.KickOnBan);
	lock.KickOnBan = in;
	MCE(lock.KickOnBan);
    }
    ETCB();
}

bool Chan_Stored_t::L_KickOnBan() const
{
    BTCB();
    NFT("Chan_Stored_t::L_KickOnBan");
    if (!Magick::instance().chanserv.LCK_KickOnBan())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.KickOnBan"));
	RET(lock.KickOnBan);
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::Restricted(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::Restricted", (in));
    if (!L_Restricted())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Restricted"));
	MCB(setting.Restricted);
	setting.Restricted = in;
	MCE(setting.Restricted);
    }
    ETCB();
}

bool Chan_Stored_t::Restricted() const
{
    BTCB();
    NFT("Chan_Stored_t::Restricted");
    if (!Magick::instance().chanserv.LCK_Restricted())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Restricted"));
	RET(setting.Restricted);
    }
    RET(Magick::instance().chanserv.DEF_Restricted());
    ETCB();
}

void Chan_Stored_t::L_Restricted(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Restricted", (in));
    if (!Magick::instance().chanserv.LCK_Restricted())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Restricted"));
	MCB(lock.Restricted);
	lock.Restricted = in;
	MCE(lock.Restricted);
    }
    ETCB();
}

bool Chan_Stored_t::L_Restricted() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Restricted");
    if (!Magick::instance().chanserv.LCK_Restricted())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Restricted"));
	RET(lock.Restricted);
    }
    RET(true);
    ETCB();
}

void Chan_Stored_t::Join(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::Join", (in));
    if (!L_Join())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Join"));
	MCB(setting.Join);
	setting.Join = in;
	MCE(setting.Join);
    }
    ETCB();
}

bool Chan_Stored_t::Join() const
{
    BTCB();
    NFT("Chan_Stored_t::Join");
    if (!Magick::instance().chanserv.LCK_Join())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Join"));
	RET(setting.Join);
    }
    RET(Magick::instance().chanserv.DEF_Join());
    ETCB();
}

void Chan_Stored_t::L_Join(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Join", (in));
    if (!Magick::instance().chanserv.LCK_Join())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Join"));
	MCB(lock.Join);
	lock.Join = in;
	MCE(lock.Join);
    }
    ETCB();
}

bool Chan_Stored_t::L_Join() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Join");
    if (!Magick::instance().chanserv.LCK_Join())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Join"));
	RET(lock.Join);
    }
    RET(true);
    ETCB();
}

bool Chan_Stored_t::Revenge(const mstring & in)
{
    BTCB();
    FT("Chan_Stored_t::Revenge", (in));
    if (!L_Revenge())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Revenge"));
	MCB(setting.Revenge);
	setting.Revenge = in;
	MCE(setting.Revenge);
	RET(true);
    }
    RET(false);
    ETCB();
}

mstring Chan_Stored_t::Revenge() const
{
    BTCB();
    NFT("Chan_Stored_t::Revenge");
    if (!Magick::instance().chanserv.LCK_Revenge())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Revenge"));
	RET(setting.Revenge);
    }
    RET(Magick::instance().chanserv.DEF_Revenge());
    ETCB();
}

void Chan_Stored_t::L_Revenge(const bool in)
{
    BTCB();
    FT("Chan_Stored_t::L_Revenge", (in));
    if (!Magick::instance().chanserv.LCK_Revenge())
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Revenge"));
	MCB(lock.Revenge);
	lock.Revenge = in;
	MCE(lock.Revenge);
    }
    ETCB();
}

bool Chan_Stored_t::L_Revenge() const
{
    BTCB();
    NFT("Chan_Stored_t::L_Revenge");
    if (!Magick::instance().chanserv.LCK_Revenge())
    {
	RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "lock.Revenge"));
	RET(lock.Revenge);
    }
    RET(true);
    ETCB();
}

bool Chan_Stored_t::Suspended() const
{
    BTCB();
    NFT("Chan_Stored_t::Suspended");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Suspend_By"));
    RET(!i_Suspend_By.empty());
    ETCB();
}

mstring Chan_Stored_t::Suspend_By() const
{
    BTCB();
    NFT("Chan_Stored_t::Suspend_By");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Suspend_By"));
    RET(i_Suspend_By);
    ETCB();
}

mDateTime Chan_Stored_t::Suspend_Time() const
{
    BTCB();
    NFT("Chan_Stored_t::Suspend_Time");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Suspend_Time"));
    RET(i_Suspend_Time);
    ETCB();
}

bool Chan_Stored_t::Forbidden() const
{
    BTCB();
    NFT("Chan_Stored_t::Forbidden");
    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "setting.Forbidden"));
    RET(setting.Forbidden);
    ETCB();
}

bool Chan_Stored_t::Level_change(const mstring & entry, const long value, const mstring & nick)
{
    BTCB();
    FT("Chan_Stored_t::Level_change", (entry, value, nick));

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Level"));
    if (Level_find(entry))
    {
	const_cast < entlist_val_t < long > * > (&(*Level))->Value(value, nick);

	RET(true);
    }
    else
    {
	Level = i_Level.end();
	RET(false);
    }
    ETCB();
}

bool Chan_Stored_t::Level_find(const mstring & entry)
{
    BTCB();
    FT("Chan_Stored_t::Level_find", (entry));

    //  entlist_val_ui<long> iter = i_Level.end();
    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Level"));
    set < entlist_val_t < long > >::iterator iter = i_Level.end();

    if (!i_Level.empty())
	iter = find_if(i_Level.begin(), i_Level.end(), EntryIsSameAs(entry, true));

    if (iter != i_Level.end())
    {
	Level = iter;
	RET(true);
    }
    else
    {
	Level = i_Level.end();
	RET(false);
    }
    ETCB();
}

long Chan_Stored_t::Level_value(const mstring & entry)
{
    BTCB();
    FT("Chan_Stored_t::Level_value", (entry));

    long retval = 0;

//  entlist_val_ui<long> iter = Level;
    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Level"));
    set < entlist_val_t < long > >::iterator iter = Level;

    if (Level_find(entry))
	retval = Level->Value();
    Level = iter;
    RET(retval);
    ETCB();
}

bool Chan_Stored_t::Access_insert(const mstring & i_entry, const long value, const mstring & nick, const mDateTime & modtime)
{
    BTCB();
    FT("Chan_Stored_t::Access_insert", (i_entry, value, nick, modtime));

    // Wildcards but no @
    if ((i_entry.Contains("*") || i_entry.Contains("?")) && !i_entry.Contains("@"))
    {
	RET(false);
    }

    // ! without @
    if (i_entry.Contains("!") && !i_entry.Contains("@"))
    {
	RET(false);
    }
    mstring entry(i_entry.LowerCase());

    // Ensure its a stored nick if no @
    // Add *! if its not *!*@*
    if (!entry.Contains("@"))
    {
	if (!Magick::instance().nickserv.IsStored(entry))
	{
	    RET(false);
	}
	else
	{
	    entry = Magick::instance().nickserv.GetStored(entry)->Host();
	    if (entry.empty())
		entry = i_entry.LowerCase();
	}
    }
    else
    {
	if (entry[0u] != '@' && !entry.Contains("!"))
	    entry.prepend("*!");
    }

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Access"));
    bool rv = Access_find(entry);

    // Result is false if its a mask, but not the same one
    if (rv && entry.Contains("@") && entry[0u] != '@' && !Access->Entry().IsSameAs(entry, true))
	rv = false;
    if (!rv)
    {
	pair < set < entlist_val_t < long > >::iterator, bool > tmp;

	MCB(i_Access.size());
	entlist_val_t < long > ent(entry, value, nick, modtime);
	i_Access.erase(ent);
	tmp = i_Access.insert(ent);

	MCE(i_Access.size());
	if (tmp.second)
	    Access = tmp.first;
	else
	    Access = i_Access.end();
	RET(true);
    }
    else
    {
	Access = i_Access.end();
	RET(false);
    }
    ETCB();
}

bool Chan_Stored_t::Access_erase()
{
    BTCB();
    NFT("Chan_Stored_t::Access_erase");

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Access"));
    if (Access != i_Access.end())
    {
	MCB(i_Access.size());
	i_Access.erase(Access);
	MCE(i_Access.size());
	Access = i_Access.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

    ETCB();
}

bool Chan_Stored_t::Access_find(const mstring & entry, const Chan_Stored_t::commstat_t commstat, const bool livelook)
{
    BTCB();
    FT("Chan_Stored_t::Access_find", (entry, livelook));

//  entlist_val_ui<long> iter = i_Access.end();
    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Access"));
    set < entlist_val_t < long > >::iterator iter = i_Access.end();

    if (!i_Access.empty())
    {
	// FIND exact nickname
	iter = find_if(i_Access.begin(), i_Access.end(), EntryIsSameAs(entry, true));

	// FIND host nickname
	if (iter == i_Access.end() && Magick::instance().nickserv.IsStored(entry))
	{
	    mstring tmp = Magick::instance().nickserv.GetStored(entry)->Host();

	    if (!tmp.empty())
		iter = find_if(i_Access.begin(), i_Access.end(), EntryIsSameAs(tmp, true));
	}

	// Check if user is on a committee on the access list ...
	if (commstat != C_None && iter == i_Access.end())
	{
	    set < entlist_val_t < long > >::iterator iter2 = i_Access.end();

	    for (iter = i_Access.begin(); iter != i_Access.end(); iter++)
	    {
		// It is indeed a committee entry ...
		if (iter->Entry() [0u] == '@' && Magick::instance().commserv.IsList(iter->Entry().After("@")))
		{
		    if ((commstat == C_IsIn && Magick::instance().commserv.GetList(iter->Entry().After("@"))->IsIn(entry)) ||
			(commstat == C_IsOn && Magick::instance().commserv.GetList(iter->Entry().After("@"))->IsOn(entry)))
		    {
			// Update iter2 if we have a higher value
			if (iter2 == i_Access.end() || iter->Value() > iter2->Value())
			    iter2 = iter;
		    }
		}
	    }
	    iter = iter2;
	}

	// Not exact or host, try either match or live lookup
	if (iter == i_Access.end())
	{
	    set < entlist_val_t < long > >::iterator iter2 = i_Access.end();

	    if (entry.Contains("@") && entry[0u] != '@')
	    {
		for (iter = i_Access.begin(); iter != i_Access.end(); iter++)
		    // Its a hostmask matching what we were passed
		    if (iter->Entry().Contains("@") && iter->Entry()[0u] != '@' && entry.Matches(iter->Entry(), true))
			// It is more specific that what we have already
			if (iter2 == i_Access.end() || iter->Entry().Matches(iter2->Entry()))
			    iter2 = iter;
	    }
	    else if (livelook && Magick::instance().nickserv.IsLive(entry))
	    {
		mstring mask = Magick::instance().nickserv.GetLive(entry)->Mask(Nick_Live_t::N_U_P_H);

		for (iter = i_Access.begin(); iter != i_Access.end(); iter++)
		    // Its a hostmask matching the user
		    if (iter->Entry().Contains("@") && iter->Entry()[0u] != '@' && mask.Matches(iter->Entry(), true))
			// It is more specific that what we have already
			if (iter2 == i_Access.end() || iter->Entry().Matches(iter2->Entry()))
			    iter2 = iter;
	    }
	    iter = iter2;
	}
    }

    if (iter != i_Access.end())
    {
	Access = iter;
	RET(true);
    }
    else
    {
	Access = i_Access.end();
	RET(false);
    }
    ETCB();
}

long Chan_Stored_t::Access_value(const mstring & entry, const Chan_Stored_t::commstat_t commstat, const bool livelook)
{
    BTCB();
    FT("Chan_Stored_t::Access_value", (entry));

    long retval = 0;

//  entlist_val_ui<long> iter = Access;
    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Access"));
    set < entlist_val_t < long > >::iterator iter = Access;

    if (Access_find(entry, commstat, livelook))
	retval = Access->Value();
    Access = iter;
    RET(retval);
    ETCB();
}

long Chan_Stored_t::GetAccess(const mstring & entry)
{
    BTCB();
    FT("Chan_Stored_t::GetAccess", (entry));

    long retval = 0;

    if (!Magick::instance().nickserv.IsLive(entry))
    {
	RET(retval);
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(entry);
    if (nlive->IsChanIdentified(i_Name) && !Suspended())
    {
	retval = Magick::instance().chanserv.Level_Max() + 1;
	RET(retval);
    }

    mstring realentry;
    bool isregd = false;

    if (Magick::instance().nickserv.IsStored(entry))
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(entry);
	isregd = nstored->IsOnline();
	if (isregd)
	{
	    realentry = nstored->Host().LowerCase();
	    if (realentry.empty())
		realentry = entry.LowerCase();

	    if (Suspended())
	    {
		if (Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
		    Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsOn(realentry))
		{
		    retval = Magick::instance().chanserv.Level_Max() + 1;
		}
		else if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
			 Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(realentry))
		{
		    retval = Level_value("SUPER");
		}
		else if (Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
			 Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->IsOn(realentry))
		{
		    retval = Level_value("AUTOOP");
		}
		else if (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
			 Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsOn(realentry))
		{
		    retval = Level_value("AUTOVOICE");
		}
		RET(retval);
	    }
	}
    }

    if (!Secure() || nlive->IsIdentified())
    {
	if (isregd && Founder().IsSameAs(realentry, true))
	{
	    retval = Magick::instance().chanserv.Level_Max() + 1;
	}
	else
	{
	    // Deliberately duplicating some of Access_value functionality.
	    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Access"));
	    set < entlist_val_t < long > >::iterator iter = Access;

	    if (Access_find(entry, C_IsOn, true))
	    {
		if (isregd || Access->Entry().Contains("@"))
		    retval = Access->Value();
	    }
	    Access = iter;
	}
    }
    RET(retval);
    ETCB();
}

bool Chan_Stored_t::GetAccess(const mstring & entry, const mstring & type)
{
    BTCB();
    FT("Chan_Stored_t::GetAccess", (entry, type));

    bool retval = (GetAccess(entry) >= Level_value(type));

    RET(retval);
    ETCB();
}

bool Chan_Stored_t::Akick_insert(const mstring & i_entry, const mstring & value, const mstring & nick,
				 const mDateTime & modtime)
{
    BTCB();
    FT("Chan_Stored_t::Akick_insert", (i_entry, value, nick, modtime));

    // Wildcards but no @
    if ((i_entry.Contains("*") || i_entry.Contains("?")) && !i_entry.Contains("@"))
    {
	RET(false);
    }

    // ! without @
    if (i_entry.Contains("!") && !i_entry.Contains("@"))
    {
	RET(false);
    }
    mstring entry(i_entry.LowerCase());

    // Ensure its a stored nick if no @
    // Add *! if its not *!*@*
    if (!entry.Contains("@"))
    {
	if (!Magick::instance().nickserv.IsStored(entry))
	{
	    RET(false);
	}
	else
	{
	    entry = Magick::instance().nickserv.GetStored(entry)->Host();
	    if (entry.empty())
		entry = i_entry.LowerCase();
	}
    }
    else
    {
	if (entry[0u] != '@' && !entry.Contains("!"))
	    entry.prepend("*!");
    }

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
    bool rv = Akick_find(entry);

    // Result is false if its a mask, but not the same one
    if (rv && entry.Contains("@") && entry[0u] != '@' && !Akick->Entry().IsSameAs(entry, true))
	rv = false;
    if (!rv)
    {
	pair < set < entlist_val_t < mstring > >::iterator, bool > tmp;

	MCB(i_Akick.size());
	entlist_val_t < mstring > ent(entry, value, nick, modtime);
	i_Akick.erase(ent);
	tmp = i_Akick.insert(ent);
	MCE(i_Akick.size());
	if (tmp.second)
	    Akick = tmp.first;
	else
	    Akick = i_Akick.end();
	RET(true);
    }
    else
    {
	Akick = i_Akick.end();
	RET(false);
    }
    ETCB();
}

bool Chan_Stored_t::Akick_insert(const mstring & entry, const mstring & nick, const mDateTime & modtime)
{
    BTCB();
    FT("Chan_Stored_t::Akick_insert", (entry, nick, modtime));
    MCB(i_Akick.size());
    bool retval = Akick_insert(entry, Magick::instance().chanserv.DEF_Akick_Reason(), nick, modtime);

    MCE(i_Akick.size());
    RET(retval);
    ETCB();
}

bool Chan_Stored_t::Akick_erase()
{
    BTCB();
    NFT("Chan_Stored_t::Akick_erase");

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
    if (Akick != i_Akick.end())
    {
	MCB(i_Akick.size());
	i_Akick.erase(Akick);
	MCE(i_Akick.size());
	Akick = i_Akick.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

    ETCB();
}

bool Chan_Stored_t::Akick_find(const mstring & entry, const Chan_Stored_t::commstat_t commstat, const bool livelook)
{
    BTCB();
    FT("Chan_Stored_t::Akick_find", (entry, livelook));

//  entlist_val_ui<mstring> iter = i_Akick.end();
    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
    set < entlist_val_t < mstring > >::iterator iter = i_Akick.end();
    if (!i_Akick.empty())
    {
	// FIND exact nickname
	iter = find_if(i_Akick.begin(), i_Akick.end(), EntryIsSameAs(entry, true));

	// FIND host nickname
	if (iter == i_Akick.end() && Magick::instance().nickserv.IsStored(entry))
	{
	    mstring tmp = Magick::instance().nickserv.GetStored(entry)->Host();

	    if (!tmp.empty())
		iter = find_if(i_Akick.begin(), i_Akick.end(), EntryIsSameAs(tmp, true));
	}

	    // Check if user is on a committee on the acess list ...
	if (commstat != C_None && iter == i_Akick.end())
	{
	    for (iter = i_Akick.begin(); iter != i_Akick.end(); iter++)
		if (iter->Entry() [0u] == '@' && Magick::instance().commserv.IsList(iter->Entry().After("@")))
		    if ((commstat == C_IsIn && Magick::instance().commserv.GetList(iter->Entry().After("@"))->IsIn(entry)) ||
			(commstat == C_IsOn && Magick::instance().commserv.GetList(iter->Entry().After("@"))->IsOn(entry)))
			break;
	}

	// Not exact or host, try either match or live lookup
	if (iter == i_Akick.end())
	{
	    set < entlist_val_t < mstring > >::iterator iter2 = i_Akick.end();
	    if (entry.Contains("@") && entry[0u] != '@')
	    {
		for (iter = i_Akick.begin(); iter != i_Akick.end(); iter++)
		    if (iter->Entry().Contains("@") && iter->Entry()[0u] != '@' && entry.Matches(iter->Entry(), true))
			// It is more specific that what we have already
			if (iter2 == i_Akick.end() || iter->Entry().Matches(iter2->Entry()))
			    iter2 = iter;
	    }
	    else if (livelook && Magick::instance().nickserv.IsLive(entry))
	    {
		mstring mask(Magick::instance().nickserv.GetLive(entry)->Mask(Nick_Live_t::N_U_P_H));

		for (iter = i_Akick.begin(); iter != i_Akick.end(); iter++)
		    if (iter->Entry().Contains("@") && iter->Entry()[0u] != '@' && mask.Matches(iter->Entry(), true))
			// It is more specific that what we have already
			if (iter2 == i_Akick.end() || iter->Entry().Matches(iter2->Entry()))
			    iter2 = iter;
	    }
	    iter = iter2;
	}
    }

    if (iter != i_Akick.end())
    {
	Akick = iter;
	RET(true);
    }
    else
    {
	Akick = i_Akick.end();
	RET(false);
    }
    ETCB();
}

mstring Chan_Stored_t::Akick_string(const mstring & entry, const Chan_Stored_t::commstat_t commstat, const bool livelook)
{
    BTCB();
    FT("Chan_Stored_t::Akick_string", (entry));

    mstring retval;

//  entlist_val_ui<mstring> iter = Akick;
    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
    set < entlist_val_t < mstring > >::iterator iter = Akick;

    if (Akick_find(entry, commstat, livelook))
	retval = Akick->Value();
    Akick = iter;
    RET(retval);
    ETCB();
}

bool Chan_Stored_t::Greet_insert(const mstring & entry, const mstring & nick, const mDateTime & modtime)
{
    BTCB();
    FT("Chan_Stored_t::Greet_insert", (entry, nick, modtime));

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Greet"));
    if (!Greet_find(entry))
    {
	MCB(i_Greet.size());
	i_Greet.push_back(entlist_t(entry, nick, modtime));
	MCE(i_Greet.size());
	Greet = i_Greet.end();
	Greet--;
	RET(true);
    }
    else
    {
	Greet = i_Greet.end();
	RET(false);
    }
    ETCB();
}

bool Chan_Stored_t::Greet_erase()
{
    BTCB();
    NFT("Chan_Stored_t::Greet_erase");

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Greet"));
    if (Greet != i_Greet.end())
    {
	MCB(i_Greet.size());
	i_Greet.erase(Greet);
	MCE(i_Greet.size());
	Greet = i_Greet.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

    ETCB();
}

bool Chan_Stored_t::Greet_find(const mstring & nick)
{
    BTCB();
    FT("Chan_Stored_t::Greet_find", (nick));

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Greet"));
    entlist_i iter = i_Greet.end();

    if (!i_Greet.empty())
	iter = find_if(i_Greet.begin(), i_Greet.end(), ModifierIsSameAs(nick, true));

    if (iter != i_Greet.end())
    {
	Greet = iter;
	RET(true);
    }
    else
    {
	Greet = i_Greet.end();
	RET(false);
    }
    ETCB();
}

bool Chan_Stored_t::Message_insert(const mstring & entry, const mstring & nick)
{
    BTCB();
    FT("Chan_Stored_t::Message_insert", (entry, nick));

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Message"));
    MCB(i_Message.size());
    i_Message.push_back(entlist_t(entry, nick));
    MCE(i_Message.size());
    Message = i_Message.end();
    Message--;
    RET(true);
    ETCB();
}

bool Chan_Stored_t::Message_erase()
{
    BTCB();
    NFT("Chan_Stored_t::Message_erase");

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Message"));
    if (Message != i_Message.end())
    {
	MCB(i_Message.size());
	i_Message.erase(Message);
	MCE(i_Message.size());
	Message = i_Message.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

    ETCB();
}

bool Chan_Stored_t::Message_find(const unsigned int num)
{
    BTCB();
    FT("Chan_Stored_t::Message_find", (num));

    MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Message"));
    entlist_i iter = i_Message.end();

    if (!i_Message.empty())
	iter = find_if(i_Message.begin(), i_Message.end(), FindNumberedEntry(num));

    if (iter != i_Message.end())
    {
	Message = iter;
	RET(true);
    }
    else
    {
	Message = i_Message.end();
	RET(false);
    }
    ETCB();
}

SXP::Tag Chan_Stored_t::tag_Chan_Stored_t("Chan_Stored_t");
SXP::Tag Chan_Stored_t::tag_Name("Name");
SXP::Tag Chan_Stored_t::tag_RegTime("RegTime");
SXP::Tag Chan_Stored_t::tag_LastUsed("LastUsed");
SXP::Tag Chan_Stored_t::tag_Founder("Founder");
SXP::Tag Chan_Stored_t::tag_CoFounder("CoFounder");
SXP::Tag Chan_Stored_t::tag_Description("Description");
SXP::Tag Chan_Stored_t::tag_Password("Password");
SXP::Tag Chan_Stored_t::tag_Email("Email");
SXP::Tag Chan_Stored_t::tag_URL("URL");
SXP::Tag Chan_Stored_t::tag_Comment("Comment");
SXP::Tag Chan_Stored_t::tag_Topic("Topic");
SXP::Tag Chan_Stored_t::tag_Topic_Setter("Topic_Setter");
SXP::Tag Chan_Stored_t::tag_Topic_Set_Time("Topic_Set_Time");
SXP::Tag Chan_Stored_t::tag_set_Mlock_On("SET_Mlock_On");
SXP::Tag Chan_Stored_t::tag_set_Mlock_Off("SET_Mlock_Off");
SXP::Tag Chan_Stored_t::tag_set_Mlock_Key("SET_Mlock_Key");
SXP::Tag Chan_Stored_t::tag_set_Mlock_Limit("SET_Mlock_Limit");
SXP::Tag Chan_Stored_t::tag_set_Bantime("SET_BanTime");
SXP::Tag Chan_Stored_t::tag_set_Parttime("SET_PartTime");
SXP::Tag Chan_Stored_t::tag_set_KeepTopic("SET_KeepTopic");
SXP::Tag Chan_Stored_t::tag_set_TopicLock("SET_TopicLock");
SXP::Tag Chan_Stored_t::tag_set_Private("SET_Private");
SXP::Tag Chan_Stored_t::tag_set_SecureOps("SET_SecureOps");
SXP::Tag Chan_Stored_t::tag_set_Secure("SET_Secure");
SXP::Tag Chan_Stored_t::tag_set_NoExpire("SET_NoExpire");
SXP::Tag Chan_Stored_t::tag_set_Anarchy("SET_Anarchy");
SXP::Tag Chan_Stored_t::tag_set_KickOnBan("SET_KickOnBan");
SXP::Tag Chan_Stored_t::tag_set_Restricted("SET_Restricted");
SXP::Tag Chan_Stored_t::tag_set_Join("SET_Join");
SXP::Tag Chan_Stored_t::tag_set_Revenge("SET_Revenge");
SXP::Tag Chan_Stored_t::tag_Forbidden("Forbidden");
SXP::Tag Chan_Stored_t::tag_lock_Mlock_On("LOCK_Mlock_On");
SXP::Tag Chan_Stored_t::tag_lock_Mlock_Off("LOCK_Mlock_Off");
SXP::Tag Chan_Stored_t::tag_lock_Bantime("LOCK_BanTime");
SXP::Tag Chan_Stored_t::tag_lock_Parttime("LOCK_PartTime");
SXP::Tag Chan_Stored_t::tag_lock_KeepTopic("LOCK_KeepTopic");
SXP::Tag Chan_Stored_t::tag_lock_TopicLock("LOCK_TopicLock");
SXP::Tag Chan_Stored_t::tag_lock_Private("LOCK_Private");
SXP::Tag Chan_Stored_t::tag_lock_SecureOps("LOCK_SecureOps");
SXP::Tag Chan_Stored_t::tag_lock_Secure("LOCK_Secure");
SXP::Tag Chan_Stored_t::tag_lock_NoExpire("LOCK_NoExpire");
SXP::Tag Chan_Stored_t::tag_lock_Anarchy("LOCK_Anarchy");
SXP::Tag Chan_Stored_t::tag_lock_KickOnBan("LOCK_KickOnBan");
SXP::Tag Chan_Stored_t::tag_lock_Restricted("LOCK_Restricted");
SXP::Tag Chan_Stored_t::tag_lock_Join("LOCK_Join");
SXP::Tag Chan_Stored_t::tag_lock_Revenge("LOCK_Revenge");
SXP::Tag Chan_Stored_t::tag_Suspend_By("Suspend_By");
SXP::Tag Chan_Stored_t::tag_Suspend_Time("Suspend_Time");
SXP::Tag Chan_Stored_t::tag_Level("Level");
SXP::Tag Chan_Stored_t::tag_Access("Access");
SXP::Tag Chan_Stored_t::tag_Akick("Akick");
SXP::Tag Chan_Stored_t::tag_Greet("Greet");
SXP::Tag Chan_Stored_t::tag_Message("Message");
SXP::Tag Chan_Stored_t::tag_UserDef("UserDef");

void Chan_Stored_t::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    FT("Chan_Stored_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    if (pElement->IsA(tag_Level))
    {
	entlist_val_t < long > * tmp = new entlist_val_t < long >;

	level_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_Access))
    {
	entlist_val_t < long > * tmp = new entlist_val_t < long >;

	access_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_Akick))
    {
	entlist_val_t < mstring > * tmp = new entlist_val_t < mstring >;
	akick_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_Greet))
    {
	entlist_t *tmp = new entlist_t;

	greet_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_Message))
    {
	entlist_t *tmp = new entlist_t;

	message_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if (pElement->IsA(tag_UserDef))
    {
	mstring *tmp = new mstring;

	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
    ETCB();
}

void Chan_Stored_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

    FT("Chan_Stored_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if (pElement->IsA(tag_Name))
    {
	pElement->Retrieve(i_Name);
	ref_class::lockData(mVarArray(lck_ChanServ, lck_stored, i_Name.LowerCase()));
    }
    if (pElement->IsA(tag_RegTime))
	pElement->Retrieve(i_RegTime);
    if (pElement->IsA(tag_LastUsed))
	pElement->Retrieve(i_LastUsed);
    if (pElement->IsA(tag_Founder))
	pElement->Retrieve(i_Founder);
    if (pElement->IsA(tag_CoFounder))
	pElement->Retrieve(i_CoFounder);
    if (pElement->IsA(tag_Description))
	pElement->Retrieve(i_Description);
    if (pElement->IsA(tag_Password))
    {
#ifdef GETPASS
	if (atoi(pElement->Attrib("hash")))
	{
	    NLOG(LM_EMERGENCY, "ERROR/WRONG_PASS_TYPE");
	}
#else
	int hash = atoi(pElement->Attrib("hash"));
	// If password was stored clear, but we use one-way, change it.
	if (!hash)
	{
	    mstring clearpass;
	    pElement->Retrieve(clearpass);

#if defined(JP2CRYPT)
	    char *newpass = new char[(clearpass.length() * 2) + 1];
	    mJP2HASH(clearpass.c_str(), clearpass.length(), newpass);
#elif defined(DESCRYPT) || defined(MD5CRYPT)
	    char newpass[35];
	    mCRYPTHASH(clearpass.c_str(), newpass, NULL);
#else
	    char newpass[33];
	    mHASH(clearpass.c_str(), clearpass.length(), newpass);
#endif
	    i_Password = newpass;
#if defined(JP2CRYPT)
	    delete [] newpass;
#endif
	}
#if defined(JP2CRYPT)
	else if (hash != 2)
	{
	    NLOG(LM_EMERGENCY, "ERROR/WRONG_PASS_TYPE");
	}
#elif defined(DESCRYPT) || defined(MD5CRYPT)
	else if (hash != 3)
	{
	    NLOG(LM_EMERGENCY, "ERROR/WRONG_PASS_TYPE");
	}
#else
	else if (hash != 1)
	{
	    NLOG(LM_EMERGENCY, "ERROR/WRONG_PASS_TYPE");
	}
#endif
#endif
	else
	    pElement->Retrieve(i_Password);
    }
    if (pElement->IsA(tag_Email))
	pElement->Retrieve(i_Email);
    if (pElement->IsA(tag_URL))
	pElement->Retrieve(i_URL);
    if (pElement->IsA(tag_Comment))
	pElement->Retrieve(i_Comment);
    if (pElement->IsA(tag_Topic))
	pElement->Retrieve(i_Topic);
    if (pElement->IsA(tag_Topic_Setter))
	pElement->Retrieve(i_Topic_Setter);
    if (pElement->IsA(tag_Topic_Set_Time))
	pElement->Retrieve(i_Topic_Set_Time);
    if (pElement->IsA(tag_set_Mlock_On))
	pElement->Retrieve(setting.Mlock_On);
    if (pElement->IsA(tag_set_Mlock_Off))
	pElement->Retrieve(setting.Mlock_Off);
    if (pElement->IsA(tag_set_Mlock_Key))
	pElement->Retrieve(setting.Mlock_Key);
    if (pElement->IsA(tag_set_Mlock_Limit))
	pElement->Retrieve(setting.Mlock_Limit);
    if (pElement->IsA(tag_set_Bantime))
	pElement->Retrieve(setting.Bantime);
    if (pElement->IsA(tag_set_Parttime))
	pElement->Retrieve(setting.Parttime);
    if (pElement->IsA(tag_set_KeepTopic))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Keeptopic = tmp;
    }
    if (pElement->IsA(tag_set_TopicLock))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Topiclock = tmp;
    }
    if (pElement->IsA(tag_set_Private))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Private = tmp;
    }
    if (pElement->IsA(tag_set_SecureOps))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Secureops = tmp;
    }
    if (pElement->IsA(tag_set_Secure))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Secure = tmp;
    }
    if (pElement->IsA(tag_set_NoExpire))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.NoExpire = tmp;
    }
    if (pElement->IsA(tag_set_Anarchy))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Anarchy = tmp;
    }
    if (pElement->IsA(tag_set_KickOnBan))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.KickOnBan = tmp;
    }
    if (pElement->IsA(tag_set_Restricted))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Restricted = tmp;
    }
    if (pElement->IsA(tag_set_Join))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Join = tmp;
    }
    if (pElement->IsA(tag_set_Revenge))
	pElement->Retrieve(setting.Revenge);
    if (pElement->IsA(tag_Forbidden))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Forbidden = tmp;
    }
    if (pElement->IsA(tag_lock_Mlock_On))
	pElement->Retrieve(lock.Mlock_On);
    if (pElement->IsA(tag_lock_Mlock_Off))
	pElement->Retrieve(lock.Mlock_Off);
    if (pElement->IsA(tag_lock_Bantime))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Bantime = tmp;
    }
    if (pElement->IsA(tag_lock_Parttime))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Parttime = tmp;
    }
    if (pElement->IsA(tag_lock_KeepTopic))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Keeptopic = tmp;
    }
    if (pElement->IsA(tag_lock_TopicLock))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Topiclock = tmp;
    }
    if (pElement->IsA(tag_lock_Private))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Private = tmp;
    }
    if (pElement->IsA(tag_lock_SecureOps))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Secureops = tmp;
    }
    if (pElement->IsA(tag_lock_Secure))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Secure = tmp;
    }
    if (pElement->IsA(tag_lock_Anarchy))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Anarchy = tmp;
    }
    if (pElement->IsA(tag_lock_KickOnBan))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.KickOnBan = tmp;
    }
    if (pElement->IsA(tag_lock_Restricted))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Restricted = tmp;
    }
    if (pElement->IsA(tag_lock_Join))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Join = tmp;
    }
    if (pElement->IsA(tag_lock_Revenge))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Revenge = tmp;
    }
    if (pElement->IsA(tag_Suspend_By))
	pElement->Retrieve(i_Suspend_By);
    if (pElement->IsA(tag_Suspend_Time))
	pElement->Retrieve(i_Suspend_Time);
    ETCB();
}

void Chan_Stored_t::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    //TODO: Add your source code here
    pOut->BeginObject(tag_Chan_Stored_t);

    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase()));
    pOut->WriteElement(tag_Name, i_Name);
    pOut->WriteElement(tag_RegTime, i_RegTime);
    pOut->WriteElement(tag_LastUsed, i_LastUsed);
    pOut->WriteElement(tag_Founder, i_Founder);
    pOut->WriteElement(tag_CoFounder, i_CoFounder);
    pOut->WriteElement(tag_Description, i_Description);
    {
	SXP::dict attr;
#ifdef GETPASS
	attr["hash"] = "0";
#else
#if defined(JP2CRYPT)
	attr["hash"] = "2";
#elif defined(DESCRYPT) || defined(MD5CRYPT)
	attr["hash"] = "3";
#else
	attr["hash"] = "1";
#endif
#endif
	pOut->WriteElement(tag_Password, i_Password, attr);
    }
    pOut->WriteElement(tag_Email, i_Email);
    pOut->WriteElement(tag_URL, i_URL);
    pOut->WriteElement(tag_Comment, i_Comment);
    pOut->WriteElement(tag_Topic, i_Topic);
    pOut->WriteElement(tag_Topic_Setter, i_Topic_Setter);
    pOut->WriteElement(tag_Topic_Set_Time, i_Topic_Set_Time);
    pOut->WriteElement(tag_set_Mlock_On, setting.Mlock_On);
    pOut->WriteElement(tag_set_Mlock_Off, setting.Mlock_Off);
    pOut->WriteElement(tag_set_Mlock_Key, setting.Mlock_Key);
    pOut->WriteElement(tag_set_Mlock_Limit, setting.Mlock_Limit);
    pOut->WriteElement(tag_set_Bantime, setting.Bantime);
    pOut->WriteElement(tag_set_Parttime, setting.Parttime);
    pOut->WriteElement(tag_set_KeepTopic, setting.Keeptopic);
    pOut->WriteElement(tag_set_TopicLock, setting.Topiclock);
    pOut->WriteElement(tag_set_Private, setting.Private);
    pOut->WriteElement(tag_set_SecureOps, setting.Secureops);
    pOut->WriteElement(tag_set_Secure, setting.Secure);
    pOut->WriteElement(tag_set_NoExpire, setting.NoExpire);
    pOut->WriteElement(tag_set_Anarchy, setting.Anarchy);
    pOut->WriteElement(tag_set_KickOnBan, setting.KickOnBan);
    pOut->WriteElement(tag_set_Restricted, setting.Restricted);
    pOut->WriteElement(tag_set_Join, setting.Join);
    pOut->WriteElement(tag_set_Revenge, setting.Revenge);
    pOut->WriteElement(tag_Forbidden, setting.Forbidden);
    pOut->WriteElement(tag_lock_Mlock_On, lock.Mlock_On);
    pOut->WriteElement(tag_lock_Mlock_Off, lock.Mlock_Off);
    pOut->WriteElement(tag_lock_Bantime, lock.Bantime);
    pOut->WriteElement(tag_lock_Parttime, lock.Parttime);
    pOut->WriteElement(tag_lock_KeepTopic, lock.Keeptopic);
    pOut->WriteElement(tag_lock_TopicLock, lock.Topiclock);
    pOut->WriteElement(tag_lock_Private, lock.Private);
    pOut->WriteElement(tag_lock_SecureOps, lock.Secureops);
    pOut->WriteElement(tag_lock_Secure, lock.Secure);
    pOut->WriteElement(tag_lock_Anarchy, lock.Anarchy);
    pOut->WriteElement(tag_lock_KickOnBan, lock.KickOnBan);
    pOut->WriteElement(tag_lock_Restricted, lock.Restricted);
    pOut->WriteElement(tag_lock_Join, lock.Join);
    pOut->WriteElement(tag_lock_Revenge, lock.Revenge);
    pOut->WriteElement(tag_Suspend_By, i_Suspend_By);
    pOut->WriteElement(tag_Suspend_Time, i_Suspend_Time);

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Level"));
	for_each(i_Level.begin(), i_Level.end(), SXP::WriteSubElement(pOut, tag_Level));
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Access"));
	for_each(i_Access.begin(), i_Access.end(), SXP::WriteSubElement(pOut, tag_Access));
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
	for_each(i_Akick.begin(), i_Akick.end(), SXP::WriteSubElement(pOut, tag_Akick));
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Greet"));
	for_each(i_Greet.begin(), i_Greet.end(), SXP::WriteSubElement(pOut, tag_Greet));
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Message"));
	for_each(i_Message.begin(), i_Message.end(), SXP::WriteSubElement(pOut, tag_Message));
    }

    for_each(i_UserDef.begin(), i_UserDef.end(), SXP::WritePairElement(pOut, tag_UserDef));

    pOut->EndObject(tag_Chan_Stored_t);
    ETCB();
}

size_t Chan_Stored_t::Usage() const
{
    BTCB();
    size_t retval = 0;

    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_RegTime.Internal());
    retval += sizeof(i_LastUsed.Internal());
    retval += i_Founder.capacity();
    retval += i_CoFounder.capacity();
    retval += i_Description.capacity();
    retval += i_Password.capacity();
    retval += i_Email.capacity();
    retval += i_URL.capacity();
    retval += i_Comment.capacity();
    map < mstring, unsigned int >::const_iterator i;

    for (i = failed_passwds.begin(); i != failed_passwds.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second);
    }

    retval += sizeof(setting);
    retval += sizeof(lock);
    retval += setting.Mlock_On.capacity();
    retval += lock.Mlock_On.capacity();
    retval += setting.Mlock_Off.capacity();
    retval += lock.Mlock_Off.capacity();
    retval += setting.Mlock_Key.capacity();
    retval += setting.Revenge.capacity();

    retval += i_Topic.capacity();
    retval += i_Topic_Setter.capacity();
    retval += sizeof(i_Topic_Set_Time.Internal());
    retval += i_Suspend_By.capacity();
    retval += sizeof(i_Suspend_Time.Internal());

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Level"));
	retval = accumulate(i_Level.begin(), i_Level.end(), retval, AddUsage());
    }
    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Access"));
	retval = accumulate(i_Access.begin(), i_Access.end(), retval, AddUsage());
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
	retval = accumulate(i_Akick.begin(), i_Akick.end(), retval, AddUsage());
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Greet"));
	retval = accumulate(i_Greet.begin(), i_Greet.end(), retval, AddUsage());
    }
    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Message"));
	retval = accumulate(i_Message.begin(), i_Message.end(), retval, AddUsage());
    }

    map < mstring, mstring >::const_iterator m;
    for (m = i_UserDef.begin(); m != i_UserDef.end(); m++)
    {
	retval += m->first.capacity();
	retval += m->second.capacity();
    }

    return retval;
    ETCB();
}

void Chan_Stored_t::DumpB() const
{
    BTCB();
    MB(0,
       (i_Name, i_RegTime, i_LastUsed, i_Founder, i_CoFounder, i_Description, i_Password, i_Email, i_URL, i_Comment,
	failed_passwds.size(), setting.Mlock_On, lock.Mlock_On, setting.Mlock_Off, lock.Mlock_Off, setting.Mlock_Key));
    MB(16,
       (setting.Mlock_Limit, i_Topic, i_Topic_Setter, i_Topic_Set_Time, setting.Bantime, lock.Bantime, setting.Parttime,
	lock.Parttime, setting.Keeptopic, lock.Keeptopic, setting.Topiclock, lock.Topiclock, setting.Private, lock.Private,
	setting.Secureops, lock.Secureops));
    MB(32,
       (setting.Secure, lock.Secure, setting.NoExpire, setting.Anarchy, lock.Anarchy, setting.KickOnBan, lock.KickOnBan,
	setting.Restricted, lock.Restricted, setting.Join, lock.Join, setting.Forbidden, setting.Revenge, lock.Revenge,
	i_Suspend_By, i_Suspend_Time));
    MB(48, (i_Level.size(), i_Access.size(), i_Akick.size(), i_Greet.size(), i_Message.size(), i_UserDef.size()));
    ETCB();
}

void Chan_Stored_t::DumpE() const
{
    BTCB();
    ME(0,
       (i_Name, i_RegTime, i_LastUsed, i_Founder, i_CoFounder, i_Description, i_Password, i_Email, i_URL, i_Comment,
	failed_passwds.size(), setting.Mlock_On, lock.Mlock_On, setting.Mlock_Off, lock.Mlock_Off, setting.Mlock_Key));
    ME(16,
       (setting.Mlock_Limit, i_Topic, i_Topic_Setter, i_Topic_Set_Time, setting.Bantime, lock.Bantime, setting.Parttime,
	lock.Parttime, setting.Keeptopic, lock.Keeptopic, setting.Topiclock, lock.Topiclock, setting.Private, lock.Private,
	setting.Secureops, lock.Secureops));
    ME(32,
       (setting.Secure, lock.Secure, setting.NoExpire, setting.Anarchy, lock.Anarchy, setting.KickOnBan, lock.KickOnBan,
	setting.Restricted, lock.Restricted, setting.Join, lock.Join, setting.Forbidden, setting.Revenge, lock.Revenge,
	i_Suspend_By, i_Suspend_Time));
    ME(48, (i_Level.size(), i_Access.size(), i_Akick.size(), i_Greet.size(), i_Message.size(), i_UserDef.size()));
    ETCB();
}

// --------- end of Chan_Stored_t ---------------------------------
