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
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
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
    users.clear();
    map < mstring, triplet < bool, bool, bool > >::const_iterator k;

    for (k = in.users.begin(); k != in.users.end(); k++)
	users.insert(*k);
    for (k = in.squit.begin(); k != in.squit.end(); k++)
	squit.insert(*k);
    bans.clear();
    exempt.clear();
    map < mstring, mDateTime >::const_iterator i;
    for (i = in.bans.begin(); i != in.bans.end(); i++)
	bans.insert(*i);
    for (i = in.exempt.begin(); i != in.exempt.end(); i++)
	exempt.insert(*i);
    i_Topic = in.i_Topic;
    i_Topic_Set_Time = in.i_Topic_Set_Time;
    i_Topic_Setter = in.i_Topic_Setter;
    modes = in.modes;
    i_Key = in.i_Key;
    i_Limit = in.i_Limit;
    p_modes_on = in.p_modes_on;
    p_modes_off = in.p_modes_off;
    p_modes_on_params.clear();
    p_modes_on_params = in.p_modes_on_params;
    p_modes_off_params.clear();
    p_modes_off_params = in.p_modes_off_params;
    ph_timer = in.ph_timer;
    recent_parts.clear();
    for (i = in.recent_parts.begin(); i != in.recent_parts.end(); i++)
	recent_parts.insert(*i);
    i_UserDef.clear();
    map < mstring, mstring >::const_iterator j;
    for (j = in.i_UserDef.begin(); j != in.i_UserDef.end(); j++)
	i_UserDef.insert(*j);
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
    unsigned int i;

    map < mstring, triplet < bool, bool, bool > >::const_iterator k;

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "squit"));
    for (i = 0, k = squit.begin(); k != squit.end(); k++, i++)
	if (i == num)
	{
	    RET(k->first);
	}

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
    unsigned int i;

    map < mstring, triplet < bool, bool, bool > >::const_iterator k;

    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
    for (i = 0, k = users.begin(); k != users.end(); k++, i++)
	if (i == num)
	{
	    RET(k->first);
	}

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
    unsigned int i;

    map < mstring, mDateTime >::const_iterator k;
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "bans"));
    for (i = 0, k = bans.begin(); k != bans.end(); k++, i++)
	if (i == num)
	{
	    RET(k->first);
	}

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
    unsigned int i;

    map < mstring, mDateTime >::const_iterator k;
    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "exempt"));
    for (i = 0, k = exempt.begin(); k != exempt.end(); k++, i++)
	if (i == num)
	{
	    RET(k->first);
	}

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
    map < mstring, mDateTime >::const_iterator i;
    for (i = bans.begin(); i != bans.end(); i++)
    {
	if (mask.Matches(i->first, true))
	{
	    RET(true);
	}
    }
    RET(false);
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
    map < mstring, mDateTime >::const_iterator i;
    for (i = exempt.begin(); i != exempt.end(); i++)
    {
	if (mask.Matches(i->first, true))
	{
	    RET(true);
	}
    }
    RET(false);
    ETCB();
}

void Chan_Live_t::LockDown()
{
    BTCB();
    NFT("Chan_Live_t::LockDown");

    Magick::instance().server.JOIN(Magick::instance().chanserv.FirstName(), i_Name);
    // Override the MLOCK checking.
    SendMode("+s");
    MLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "ph_timer"));
    MCB(ph_timer);
    while (Magick::instance().Pause())
	ACE_OS::sleep(1);
    ph_timer =
	Magick::instance().reactor().schedule_timer(&(Magick::instance().chanserv.ph), new mstring(i_Name),
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
	    if (reqmode == mode[i] && param < mode_params.size())
	    {
		if ((!reqparam.empty() && mode_params[param] == reqparam) || reqparam.empty())
		{
		    // Do nothing ... we want to delete it!
		}
		else
		{
		    new_mode += mode[i];
		    if (mode[i] != 'l' || change)
			new_params.push_back(mode_params[param]);
		}
	    }
	    if (mode[i] != 'l' || !change)
		param++;
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
    mstring s_key, s_mlock_on, s_mlock_off;
    int s_limit = 0;

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

			if (Magick::instance().server.proto.Numeric.User() && Magick::instance().nickserv.IsLive(arg))
			    arg =
				Magick::instance().server.proto.Numeric.UserNumeric(Magick::instance().nickserv.GetLive(arg)->
										    Numeric());

			if (add)
			{
			    if (!IsOp(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'o', arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'o', arg);
				if (!ModeExists(p_modes_on, p_modes_on_params, true, 'o', arg))
				{
				    p_modes_on += "o";
				    p_modes_on_params.push_back(arg);
				}
			    }
			}
			else
			{
			    if (IsOp(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'o', arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'o', arg);
				if (!ModeExists(p_modes_off, p_modes_off_params, false, 'o', arg))
				{
				    p_modes_off += "o";
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

			if (Magick::instance().server.proto.Numeric.User() && Magick::instance().nickserv.IsLive(arg))
			    arg =
				Magick::instance().server.proto.Numeric.UserNumeric(Magick::instance().nickserv.GetLive(arg)->
										    Numeric());

			if (add)
			{
			    if (!IsHalfOp(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'h', arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'h', arg);
				if (!ModeExists(p_modes_on, p_modes_on_params, true, 'h', arg))
				{
				    p_modes_on += "h";
				    p_modes_on_params.push_back(arg);
				}
			    }
			}
			else
			{
			    if (IsHalfOp(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'h', arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'h', arg);
				if (!ModeExists(p_modes_off, p_modes_off_params, false, 'h', arg))
				{
				    p_modes_off += "h";
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

			if (Magick::instance().server.proto.Numeric.User() && Magick::instance().nickserv.IsLive(arg))
			    arg =
				Magick::instance().server.proto.Numeric.UserNumeric(Magick::instance().nickserv.GetLive(arg)->
										    Numeric());

			if (add)
			{
			    if (!IsVoice(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'v', arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'v', arg);
				if (!ModeExists(p_modes_on, p_modes_on_params, true, 'v', arg))
				{
				    p_modes_on += "v";
				    p_modes_on_params.push_back(arg);
				}
			    }
			}
			else
			{
			    if (IsVoice(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'v', arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'v', arg);
				if (!ModeExists(p_modes_off, p_modes_off_params, false, 'v', arg))
				{
				    p_modes_off += "v";
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
			if (add)
			{
			    if (!IsBan(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(param, " ")))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(param, " "));
				if (!ModeExists(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(param, " ")))
				{
				    p_modes_on += "b";
				    p_modes_on_params.push_back(in.ExtractWord(param, " "));
				}
			    }
			}
			else
			{
			    if (IsBan(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(param, " ")))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(param, " "));
				if (!ModeExists(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(param, " ")))
				{
				    p_modes_off += "b";
				    p_modes_off_params.push_back(in.ExtractWord(param, " "));
				}
			    }
			}
			param++;
		    }
		    break;

		case 'e':
		    if (in.WordCount(" ") >= param)
		    {
			if (add)
			{
			    if (!IsExempt(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(param, " ")))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(param, " "));
				if (!ModeExists(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(param, " ")))
				{
				    p_modes_on += "e";
				    p_modes_on_params.push_back(in.ExtractWord(param, " "));
				}
			    }
			}
			else
			{
			    if (IsExempt(in.ExtractWord(param, " ")))
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(param, " ")))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(param, " "));
				if (!ModeExists(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(param, " ")))
				{
				    p_modes_off += "e";
				    p_modes_off_params.push_back(in.ExtractWord(param, " "));
				}
			    }
			}
			param++;
		    }
		    break;

		case 'l':
		    if (add)
		    {
			if (!(cstored && s_mlock_off.Contains("l")))
			{
			    if (in.WordCount(" ") >= param)
			    {
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'l');
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'l'))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'l');
				p_modes_on += "l";
				p_modes_on_params.push_back(in.ExtractWord(param, " "));
				param++;
			    }
			}
		    }
		    else
		    {
			if (!(cstored && s_limit))
			{
			    if (i_Limit)
			    {
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'l'))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'l');
				if (!ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
				    p_modes_off += "l";
			    }
			}
		    }
		    break;
		case 'k':
		    if (in.WordCount(" ") >= param)
		    {
			if (add)
			{
			    // ONLY allow +k if we've turned it off before, or one isnt set
			    if (i_Key.empty() || ModeExists(p_modes_off, p_modes_off_params, false, 'k'))
			    {
				if (!(cstored && s_mlock_off.Contains("k")))
				{
				    // DONT take off 'off' value, coz we can -k+k key1 key2
				    if (!ModeExists(p_modes_on, p_modes_on_params, true, 'k'))
				    {
					p_modes_on += "k";
					p_modes_on_params.push_back(in.ExtractWord(param, " "));
				    }
				}
			    }
			}
			else
			{
			    if (i_Key == in.ExtractWord(param, " "))
			    {
				if (!(cstored && !s_key.empty()))
				{
				    if (ModeExists(p_modes_on, p_modes_on_params, true, 'k'))
					RemoveMode(p_modes_on, p_modes_on_params, true, 'k');
				    if (!ModeExists(p_modes_off, p_modes_off_params, false, 'k'))
				    {
					p_modes_off += "k";
					p_modes_off_params.push_back(in.ExtractWord(param, " "));
				    }
				}
			    }
			}
			param++;
		    }
		    break;
		default:
		    if (in.WordCount(" ") >= param)
			param++;
		}
	    }
	    else
	    {
		if (add)
		{
		    if_RLOCK ((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"), !modes.Contains(mode[i]))
		    {
			if (!(cstored && s_mlock_off.Contains(mode[i])))
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
		    if_RLOCK ((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"), modes.Contains(mode[i]))
		    {
			if (!(cstored && s_mlock_on.Contains(mode[i])))
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

    mstring change(in.ExtractWord(1, ": "));
    mstring newmode, newmode_param;
    unsigned int fwdargs = 2, i, wc;
    bool add = true;

    wc = in.WordCount(": ");
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
	for (fwdargs = 2, i = 0; i < change.size(); i++)
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
		mstring arg(in.ExtractWord(fwdargs, ": "));

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
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'o', arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'o', arg);
			    }
			    else
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
				    users[arg.LowerCase()].first = false;
				}
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'o', arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'o', arg);
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
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'h', arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'h', arg);
			    }
			    else
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
				    users[arg.LowerCase()].second = false;
				}
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'h', arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'h', arg);
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
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'v', arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'v', arg);
			    }
			    else
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "users"));
				    users[arg.LowerCase()].third = false;
				}
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'v', arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'v', arg);
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
			    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "bans"));
			    CB(5, bans.size());
			    if (add)
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "bans"));
				    bans[arg.LowerCase()] = mDateTime::CurrentDateTime();
				}
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'b', arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'b', arg);
			    }
			    else
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "bans"));
				    bans.erase(arg.LowerCase());
				}
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'b', arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'b', arg);
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
			    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "exempt"));
			    CB(5, exempt.size());
			    if (add)
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "exempt"));
				    exempt[arg.LowerCase()] = mDateTime::CurrentDateTime();
				}
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'e', arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'e', arg);
			    }
			    else
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "exempt"));
				    exempt.erase(arg.LowerCase());
				}
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'e', arg))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'e', arg);
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
			    RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Key"));
			    CB(5, i_Key);
			    if (add)
			    {
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Key"));
				    i_Key = arg;
				}
				if (ModeExists(p_modes_on, p_modes_on_params, true, 'k', arg))
				    RemoveMode(p_modes_on, p_modes_on_params, true, 'k', arg);
			    }
			    else
			    {
				if (i_Key != arg)
				{
				    LOG(LM_ERROR, "ERROR/KEYMISMATCH", (i_Key, arg, i_Name, source));
				}
				{
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Key"));
				    i_Key.erase();
				}
				if (ModeExists(p_modes_off, p_modes_off_params, false, 'k'))
				    RemoveMode(p_modes_off, p_modes_off_params, false, 'k');
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
			    {
				RLOCK((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Limit"));
				CB(5, i_Limit);
				if (fwdargs > in.WordCount(": "))
				{
				    LOG(LM_ERROR, "ERROR/NOLIMIT", (i_Name, source));
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Limit"));
				    i_Limit = 0;
				}
				else if (!arg.IsNumber())
				{
				    LOG(LM_ERROR, "ERROR/NOLIMIT", (i_Name, source));
				    WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Limit"));
				    i_Limit = 0;
				}
				else
				{
				    {
					WLOCK5((lck_ChanServ, lck_live, i_Name.LowerCase(), "i_Limit"));
					i_Limit = atoi(arg.c_str());
				    }
				    if (ModeExists(p_modes_on, p_modes_on_params, true, 'l', arg))
					RemoveMode(p_modes_on, p_modes_on_params, true, 'l', arg);
				    newmode += change[i];
				    newmode_param += " " + arg;
				}
				CE(5, i_Limit);
			    }
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
			if (ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
			    RemoveMode(p_modes_off, p_modes_off_params, false, 'l');
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
		if_RLOCK ((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"), add && !modes.Contains(change[i]))
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
		    if_RLOCK2 ((lck_ChanServ, lck_live, i_Name.LowerCase(), "modes"), !add && modes.Contains(change[i]))
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
    }
    for (i = users.begin(); i != users.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second.first);
	retval += sizeof(i->second.second);
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

    map < mstring, unsigned int >::iterator iter;

    // Create a new one if we find the entry
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "failed_passwds"));
    MCB(failed_passwds.size());
    for (iter = failed_passwds.begin(); iter != failed_passwds.end(); iter++)
	if (iter->first.IsSameAs(nick, true))
	{
	    failed_passwds[newnick.LowerCase()] = iter->second;
	    break;
	}
    failed_passwds.erase(nick.LowerCase());
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
	clive->SendMode("+o " + nick);
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
		// If this user is the only user in channel
		if (users == 1)
		    clive->LockDown();

		RLOCK2((lck_ChanServ, lck_stored, i_Name, "setting.Mlock_On"));
		clive->SendMode("+" + setting.Mlock_On + "b " + nlive->AltMask(Magick::instance().operserv.Ignore_Method()));
	    }

	    // Can only insert with reason or default, so its safe.
	    mstring reason = parseMessage(Magick::instance().getMessage(nick, "CS_STATUS/ISFORBIDDEN"), mVarArray(i_Name));

	    Magick::instance().server.KICK(Magick::instance().chanserv.FirstName(), nick, i_Name, reason);
	    RET(false);
	}
	RET(true);
    }

    map_entry < Nick_Stored_t > nstored;
    if (Magick::instance().nickserv.IsStored(nick))
	nstored = Magick::instance().nickserv.GetStored(nick);

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
	bool rv = Akick_find(nick, C_IsOn, true);

	if (rv && ((Akick->Entry().Contains("@")) || (nstored.entry() != NULL && nstored->IsOnline())))
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
	{
	    clive->SendMode("+" + modes + " " + setting.Mlock_Key + " " +
			    (setting.Mlock_Limit ? mstring(setting.Mlock_Limit) : mstring("")));
	}

	{
	    RLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic"));
	    RLOCK2((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Setter"));
	    RLOCK3((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Topic_Set_Time"));
	    // Carry over topic ..
	    if (!burst && Keeptopic() && !i_Topic.empty())
	    {
		Magick::instance().server.TOPIC(Magick::instance().chanserv.FirstName(), i_Topic_Setter, i_Name, i_Topic,
						i_Topic_Set_Time);
	    }
	}
    }

    if (!burst && Join() && users == 1)
    {
	Magick::instance().server.JOIN(Magick::instance().chanserv.FirstName(), i_Name);
	users++;
    }

    if (GetAccess(nick) > 0)
    {
	WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_LastUsed"));
	MCB(i_LastUsed);
	i_LastUsed = mDateTime::CurrentDateTime();
	MCE(i_LastUsed);
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

    if (nstored.entry() != NULL)
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
	if (Join())
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
	    if (clive.entry() != NULL)
	    {
		// If this user is the only user in channel
		if (users == 1)
		    clive->LockDown();

		if (Akick->Entry() [0u] == '@' || Magick::instance().nickserv.IsLive(Akick->Entry()))
		    clive->SendMode("+b " +
				    Magick::instance().nickserv.GetLive(nick)->AltMask(Magick::instance().operserv.
										       Ignore_Method()));
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
					IRC_Off + "]", settime - (1.0 / (60.0 * 60.0 * 24.0)));
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
	Magick::instance().server.TOPIC(Magick::instance().chanserv.FirstName(), i_Topic_Setter, i_Name, i_Topic,
					settime - (1.0 / (60.0 * 60.0 * 24.0)));
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
				    Magick::instance().chanserv.GetLive(i_Name)->Topic_Set_Time() -
				    (1.0 / (60.0 * 60.0 * 24.0)));
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
    unsigned int fwdargs = 2, i, wc = mode.WordCount(": ");
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
	else if (Magick::instance().server.proto.ChanModeArg().Contains(change[i]))
	{
	    mstring arg = mode.ExtractWord(fwdargs, ": ");

	    switch (change[i])
	    {
	    case 'o':
		if (fwdargs <= wc)
		{
		    if (add)
		    {
			// IF not (a server set the mode and we've got anarchy set) and
			// not (services user set mode), AND (user is AUTODEOP OR
			// (channel is secure ops AND (user is not AUTOOP or CMDOP)))
			if (!(setter.Contains(".") && Anarchy()) &&
			    !(Magick::instance().nickserv.IsLive(arg) &&
			      Magick::instance().nickserv.GetLive(arg)->IsServices()) &&
			    (GetAccess(arg) <= Level_value("AUTODEOP") ||
			     (!(GetAccess(arg, "CMDOP") || GetAccess(arg, "AUTOOP")) && Secureops())))
			{
			    out_mode += "-o";
			    out_param += " " + arg;
			}
		    }
		    else if (!setter.Contains("."))
		    {
			// If user is services or a beneficiary of revenge
			// deop the setter ... bad boy ... tsk tsk ...
			if ((Magick::instance().nickserv.IsLive(arg) && Magick::instance().nickserv.GetLive(arg)->IsServices())
			    || DoRevenge("DEOP", setter, arg))
			{
			    out_mode += "-o";
			    out_param += " " + setter;
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
			// IF not (a server set the mode and we've got anarchy set) and
			// not (services user set mode), AND (user is AUTODEOP OR
			// (channel is secure ops AND (user is not AUTOHALFOP or CMDHALFOP)))
			if (!(setter.Contains(".") && Anarchy()) &&
			    !(Magick::instance().nickserv.IsLive(arg) &&
			      Magick::instance().nickserv.GetLive(arg)->IsServices()) &&
			    (GetAccess(arg) <= Level_value("AUTODEOP") ||
			     (!(GetAccess(arg, "CMDHALFOP") || GetAccess(arg, "AUTOHALFOP")) && Secureops())))
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
			// IF not (a server set the mode and we've got anarchy set) and
			// not (services user set mode), AND (user is AUTODEOP OR
			// (channel is secure ops AND (user is not AUTOVOICE or CMDVOICE)))
			if (!(setter.Contains(".") && Anarchy()) &&
			    !(Magick::instance().nickserv.IsLive(arg) &&
			      Magick::instance().nickserv.GetLive(arg)->IsServices()) &&
			    (GetAccess(arg) <= Level_value("AUTODEOP") ||
			     (!(GetAccess(arg, "CMDVOICE") || GetAccess(arg, "AUTOVOICE")) && Secureops())))
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
			    if (nlive.entry() != NULL &&
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
    setting.Mlock_On = "nits";
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

//  entlist_val_cui<long> j;
    set < entlist_val_t < long > >::const_iterator j;

    i_Level.clear();
    for (j = in.i_Level.begin(); j != in.i_Level.end(); j++)
	i_Level.insert(*j);

    i_Access.clear();
    for (j = in.i_Access.begin(); j != in.i_Access.end(); j++)
	i_Access.insert(*j);

//  entlist_val_cui<mstring> k;
    set < entlist_val_t < mstring > >::const_iterator k;
    i_Akick.clear();
    for (k = in.i_Akick.begin(); k != in.i_Akick.end(); k++)
	i_Akick.insert(*k);

    entlist_ci l;

    i_Greet.clear();
    for (l = in.i_Greet.begin(); l != in.i_Greet.end(); l++)
	i_Greet.push_back(*l);

    i_Message.clear();
    for (l = in.i_Message.begin(); l != in.i_Message.end(); l++)
	i_Message.push_back(*l);

    i_UserDef.clear();
    map < mstring, mstring >::const_iterator i;
    for (i = in.i_UserDef.begin(); i != in.i_UserDef.end(); i++)
	i_UserDef.insert(*i);
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
    char newpass[33];

    mHASH(in.c_str(), in.length(), newpass);
    i_Password = newpass;
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

#ifdef GETPASS
    mstring check(password);
#else
    char chkpass[33];

    mHASH(password.c_str(), password.length(), chkpass);
    mstring check(chkpass);
#endif
    WLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "failed_passwds"));
    MCB(failed_passwds.size());
    if_RLOCK ((lck_ChanServ, lck_stored, i_Name.LowerCase(), "i_Password"), i_Password == check)
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
    MCB(setting.Mlock_On);
    CB(1, setting.Mlock_Off);
    CB(2, setting.Mlock_Key);
    CB(3, setting.Mlock_Limit);
    setting.Mlock_On.erase();
    setting.Mlock_Off.erase();
    setting.Mlock_Key.erase();
    setting.Mlock_Limit = 0;
    vector < mstring > retval;
    mstring output, change(mode.ExtractWord(1, ": "));
    unsigned int i;
    unsigned int fwdargs = 2;
    bool add = true;
    bool ignorek = false;
    bool ignorel = false;

    if (change.WordCount("k") > 2)
    {
	ignorek = true;
	output.erase();
	output = parseMessage(Magick::instance().getMessage(source, "ERR_SYNTAX/MULTI_MODE"), mVarArray('k'));
	retval.push_back(output);
    }
    if (change.WordCount("l") > 2)
    {
	ignorel = true;
	output.erase();
	output = parseMessage(Magick::instance().getMessage(source, "ERR_SYNTAX/MULTI_MODE"), mVarArray('l'));
	retval.push_back(output);
    }

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
	    switch (change[i])
	    {
	    case 'k':
		if (!ignorek && add)
		{
		    if (fwdargs > mode.WordCount(": "))
		    {
			output.erase();
			output = Magick::instance().getMessage(source, "ERR_SYNTAX/NOKEY");
			retval.push_back(output);
			fwdargs--;
		    }
		    else
		    {
			setting.Mlock_Key = mode.ExtractWord(fwdargs, ": ");
		    }
		    fwdargs++;
		}
		break;

	    case 'l':
		if (!ignorel && add)
		{
		    if (fwdargs > mode.WordCount(": "))
		    {
			output.erase();
			output = Magick::instance().getMessage(source, "ERR_SYNTAX/NOLIMIT");
			retval.push_back(output);
			fwdargs--;
		    }
		    else if (!mode.ExtractWord(fwdargs, ": ").IsNumber() || mode.ExtractWord(fwdargs, ": ").Contains("."))
		    {
			output.erase();
			output = Magick::instance().getMessage(source, "ERR_SYNTAX/WHOLENUMBER");
			retval.push_back(output);
		    }
		    else if (atol(mode.ExtractWord(fwdargs, ": ")) < 1)
		    {
			output.erase();
			output =
			    parseMessage(Magick::instance().getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
					 mVarArray(1, 32768));
			retval.push_back(output);
		    }
		    else
		    {
			setting.Mlock_Limit = atol(mode.ExtractWord(fwdargs, ": ").c_str());
		    }
		    fwdargs++;
		}
		break;
	    default:
		fwdargs++;
	    }
	}
    }

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
		    if (!setting.Mlock_On.Contains(change[i]))
			setting.Mlock_On += change[i];
		    if (setting.Mlock_Off.Contains(change[i]))
			setting.Mlock_Off.Remove(change[i]);
		}
	    }
	    else
	    {
		// Nothing with arguments unless its k or l
		if (!Magick::instance().server.proto.ChanModeArg().Contains(change[i]) ||
		    ((change[i] == 'k' && !ignorek) || (change[i] == 'l' && !ignorel)))
		{
		    if (!setting.Mlock_Off.Contains(change[i]))
			setting.Mlock_Off += change[i];
		    if (setting.Mlock_On.Contains(change[i]))
			setting.Mlock_On.Remove(change[i]);
		}
	    }
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

	if (nlive.entry() != NULL)
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

	if (clive.entry() != NULL)
	{
	    mstring modes_param;

	    modes = "+";
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
	    if (modes.length() > 2 && clive.entry() != NULL)
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
    mstring output, change(mode.ExtractWord(1, ": "));
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
	for (iter = i_Level.begin(); iter != i_Level.end(); iter++)
	    if (iter->Entry().IsSameAs(entry, true))
		break;

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
	tmp = i_Access.insert(entlist_val_t < long > (entry, value, nick, modtime));

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
	for (iter = i_Access.begin(); iter != i_Access.end(); iter++)
	    if (iter->Entry().IsSameAs(entry, true))
		break;

	// FIND host nickname
	if (iter == i_Access.end() && Magick::instance().nickserv.IsStored(entry))
	{
	    mstring tmp = Magick::instance().nickserv.GetStored(entry)->Host();

	    if (!tmp.empty())
	    {
		for (iter = i_Access.begin(); iter != i_Access.end(); iter++)
		    if (iter->Entry().IsSameAs(tmp, true))
			break;
	    }
	    else
		tmp = entry;

	    // Check if user is on a committee on the acess list ...
	    if (commstat != C_None && iter == i_Access.end())
	    {
		set < entlist_val_t < long > >::iterator iter2 = i_Access.end();

		for (iter = i_Access.begin(); iter != i_Access.end(); iter++)
		    // It is indeed a committee entry ...
		    if (iter->Entry() [0u] == '@' && Magick::instance().commserv.IsList(iter->Entry().After("@")))
			// Verify that we do the right check ...
			if ((commstat == C_IsIn && Magick::instance().commserv.GetList(iter->Entry().After("@"))->IsIn(tmp)) ||
			    (commstat == C_IsOn && Magick::instance().commserv.GetList(iter->Entry().After("@"))->IsOn(tmp)))
			    // Update iter2 if we have a higher value
			    if (iter2 == i_Access.end() || iter->Value() > iter2->Value())
				iter2 = iter;
		iter = iter2;
	    }
	}

	// Not exact or host, try either match or live lookup
	if (iter == i_Access.end())
	{
	    set < entlist_val_t < long > >::iterator iter2 = i_Access.end();

	    if (entry.Contains("@"))
	    {
		for (iter = i_Access.begin(); iter != i_Access.end(); iter++)
		    // Its a hostmask matching what we were passed
		    if (iter->Entry().Contains("@") && entry.Matches(iter->Entry(), true))
			// It is more specific that what we have already
			if (iter2 == i_Access.end() || iter->Entry().Matches(iter2->Entry()))
			    iter2 = iter;
	    }
	    else if (livelook && Magick::instance().nickserv.IsLive(entry))
	    {
		mstring mask = Magick::instance().nickserv.GetLive(entry)->Mask(Nick_Live_t::N_U_P_H);

		for (iter = i_Access.begin(); iter != i_Access.end(); iter++)
		    // Its a hostmask matching the user
		    if (iter->Entry().Contains("@") && mask.Matches(iter->Entry(), true))
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
	tmp = i_Akick.insert(entlist_val_t < mstring > (entry, value, nick, modtime));
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
	for (iter = i_Akick.begin(); iter != i_Akick.end(); iter++)
	    if (iter->Entry().IsSameAs(entry, true))
		break;

	// FIND host nickname
	if (iter == i_Akick.end() && Magick::instance().nickserv.IsStored(entry))
	{
	    mstring tmp = Magick::instance().nickserv.GetStored(entry)->Host();

	    if (!tmp.empty())
	    {
		for (iter = i_Akick.begin(); iter != i_Akick.end(); iter++)
		    if (iter->Entry().IsSameAs(tmp, true))
			break;
	    }
	    else
		tmp = entry;

	    // Check if user is on a committee on the acess list ...
	    if (commstat != C_None && iter == i_Akick.end())
	    {
		for (iter = i_Akick.begin(); iter != i_Akick.end(); iter++)
		    if (iter->Entry() [0u] == '@' && Magick::instance().commserv.IsList(iter->Entry().After("@")))
			if ((commstat == C_IsIn && Magick::instance().commserv.GetList(iter->Entry().After("@"))->IsIn(tmp)) ||
			    (commstat == C_IsOn && Magick::instance().commserv.GetList(iter->Entry().After("@"))->IsOn(tmp)))
			    break;
	    }
	}

	// Not exact or host, try either match or live lookup
	if (iter == i_Akick.end())
	{
	    set < entlist_val_t < mstring > >::iterator iter2 = i_Akick.end();
	    if (entry.Contains("@"))
	    {
		for (iter = i_Akick.begin(); iter != i_Akick.end(); iter++)
		    if (iter->Entry().Contains("@") && entry.Matches(iter->Entry(), true))
			// It is more specific that what we have already
			if (iter2 == i_Akick.end() || iter->Entry().Matches(iter2->Entry()))
			    iter2 = iter;
	    }
	    else if (livelook && Magick::instance().nickserv.IsLive(entry))
	    {
		mstring mask(Magick::instance().nickserv.GetLive(entry)->Mask(Nick_Live_t::N_U_P_H));

		for (iter = i_Akick.begin(); iter != i_Akick.end(); iter++)
		    if (iter->Entry().Contains("@") && mask.Matches(iter->Entry(), true))
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
	for (iter = i_Greet.begin(); iter != i_Greet.end(); iter++)
	    if (nick.IsSameAs(iter->Last_Modifier(), true))
		break;

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
    if (num <= 0 || num > i_Message.size())
    {
	RET(false);
    }

    unsigned int i;
    entlist_i iter = i_Message.end();

    for (iter = i_Message.begin(), i = 1; iter != i_Message.end() && i < num; iter++, i++);

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
	// If password was stored clear, but we use one-way, change it.
	if (!atoi(pElement->Attrib("hash")))
	{
	    mstring clearpass;

	    pElement->Retrieve(clearpass);
	    char newpass[33];

	    mHASH(clearpass.c_str(), clearpass.length(), newpass);
	    i_Password = newpass;
	}
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

    set < entlist_val_t < long > >::iterator j;

    set < entlist_val_t < mstring > >::iterator k;
    entlist_i l;

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
	attr["hash"] = "1";
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
	for (j = i_Level.begin(); j != i_Level.end(); j++)
	{
	    pOut->BeginObject(tag_Level);
	    pOut->WriteSubElement(const_cast < entlist_val_t < long > * > (&(*j)));

	    pOut->EndObject(tag_Level);
	}
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Access"));
	for (j = i_Access.begin(); j != i_Access.end(); j++)
	{
	    pOut->BeginObject(tag_Access);
	    pOut->WriteSubElement(const_cast < entlist_val_t < long > * > (&(*j)));

	    pOut->EndObject(tag_Access);
	}
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
	for (k = i_Akick.begin(); k != i_Akick.end(); k++)
	{
	    pOut->BeginObject(tag_Akick);
	    pOut->WriteSubElement(const_cast < entlist_val_t < mstring > * > (&(*k)));
	    pOut->EndObject(tag_Akick);
	}
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Greet"));
	for (l = i_Greet.begin(); l != i_Greet.end(); l++)
	{
	    pOut->BeginObject(tag_Greet);
	    pOut->WriteSubElement(&(*l));
	    pOut->EndObject(tag_Greet);
	}
    }

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Message"));
	for (l = i_Message.begin(); l != i_Message.end(); l++)
	{
	    pOut->BeginObject(tag_Message);
	    pOut->WriteSubElement(&(*l));
	    pOut->EndObject(tag_Message);
	}
    }

    map < mstring, mstring >::const_iterator iter;
    for (iter = i_UserDef.begin(); iter != i_UserDef.end(); iter++)
    {
	pOut->WriteElement(tag_UserDef, iter->first + "\n" + iter->second);
    }

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

    set < entlist_val_t < long > >::const_iterator j;

    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Level"));
	for (j = i_Level.begin(); j != i_Level.end(); j++)
	{
	    j->Usage();
	}
    }
    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Access"));
	for (j = i_Access.begin(); j != i_Access.end(); j++)
	{
	    j->Usage();
	}
    }

    set < entlist_val_t < mstring > >::const_iterator k;
    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Akick"));
	for (k = i_Akick.begin(); k != i_Akick.end(); k++)
	{
	    k->Usage();
	}
    }

    list < entlist_t >::const_iterator l;
    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Greet"));
	for (l = i_Greet.begin(); l != i_Greet.end(); l++)
	{
	    retval += l->Usage();
	}
    }
    {
	MLOCK((lck_ChanServ, lck_stored, i_Name.LowerCase(), "Message"));
	for (l = i_Message.begin(); l != i_Message.end(); l++)
	{
	    retval += l->Usage();
	}
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
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_Op);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "D*OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_DeOp);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_HalfOp);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "D*H*OP*", Magick::instance().commserv.REGD_Name(),
						 ChanServ::do_DeHalfOp);
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
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LIVE*", Magick::instance().commserv.SOP_Name(),
						 ChanServ::do_Live);
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
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "D*OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "D*H*OP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "VOIC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "D*VOIC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "TOPIC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "KICK*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "REM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "USER*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "INV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNBAN*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LIVE*", Magick::instance().commserv.SOP_Name());
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

    RLOCK((lck_ChanServ, lck_stored));
    map_entry < Chan_Stored_t > old_entry(stored, in->Name().LowerCase());
    if (old_entry.entry() != NULL)
    {
	old_entry->setDelete();
	stored.erase(in->Name().LowerCase());
    }
    WLOCK((lck_ChanServ, lck_stored));
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
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Get", "NotFound", in));
	NRET(Chan_Stored_t &, GLOB_Chan_Stored_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Stored(E_ChanServ_Stored::W_Get, E_ChanServ_Stored::T_Invalid, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Get", "Invalid", in));
	NRET(Chan_Stored_t &, GLOB_Chan_Stored_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Stored(E_ChanServ_Stored::W_Get, E_ChanServ_Stored::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Stored", "Get", "Blank", in));
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

    RLOCK((lck_ChanServ, lck_stored));
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
    WLOCK((lck_ChanServ, lck_stored));
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

    RLOCK((lck_ChanServ, lck_live));
    map_entry < Chan_Live_t > old_entry(live, in->Name().LowerCase());
    if (old_entry.entry() != NULL)
    {
	old_entry->setDelete();
	live.erase(in->Name().LowerCase());
    }
    WLOCK((lck_ChanServ, lck_live));
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
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Get", "NotFound", in));
	NRET(Chan_Live_t &, GLOB_Chan_Live_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Live(E_ChanServ_Live::W_Get, E_ChanServ_Live::T_Invalid, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Get", "Invalid", in));
	NRET(Chan_Live_t &, GLOB_Chan_Live_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_ChanServ_Live(E_ChanServ_Live::W_Get, E_ChanServ_Live::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Chan", "Live", "Get", "Blank", in));
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

    RLOCK((lck_ChanServ, lck_live));
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
    WLOCK((lck_ChanServ, lck_live));
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
	HelpTopic += " " + params.After(" ");
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
    mstring desc = params.After(" ", 3);

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
	NSEND(mynick, source, "CS_STATUS/TOOMANY");
	return;
    }

    map_entry < Chan_Stored_t > cstored(new Chan_Stored_t(channel, founder, password, desc));
    Magick::instance().chanserv.AddStored(cstored);
    if (cstored->Join())
	Magick::instance().server.JOIN(Magick::instance().chanserv.FirstName(), channel);
    nlive->SetLastChanReg();
    cstored->Topic(clive->Topic(), clive->Topic_Setter(), clive->Topic_Set_Time());
    clive->SendMode(cstored->Mlock());
    nlive->ChanIdentify(channel, password);
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

    if (cstored->Join() && Magick::instance().chanserv.IsLive(channel) &&
	Magick::instance().chanserv.GetLive(channel)->IsIn(Magick::instance().chanserv.FirstName()))
    {
	Magick::instance().server.PART(Magick::instance().chanserv.FirstName(), channel);
    }
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
    mstring reason = params.After(" ", 2);

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
					"]", clive->Topic_Set_Time() - (1.0 / (60.0 * 60.0 * 24.0)));

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
					Magick::instance().chanserv.GetLive(channel)->Topic_Set_Time() -
					(1.0 / (60.0 * 60.0 * 24.0)));

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
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), chan.Name(), founder));
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
	if (cstored.entry() != NULL)
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
	if (cstored.entry() == NULL)
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
	mstring modes = params.After(" ", 2);

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
    mstring topic = params.After(" ", 2);

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
	reason = params.After(" ", 3);
    else
	reason = Magick::instance().chanserv.DEF_Akick_Reason();

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

    if (!chan->GetAccess(source, "CMDKICK"))
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
    mstring reason = params.After(" ", 3);

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

    if (!Magick::instance().chanserv.IsStored(channel))
    {
	SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (channel));
	return;
    }
    map_entry < Chan_Stored_t > chan = Magick::instance().chanserv.GetStored(channel);
    channel = Magick::instance().getSname(channel);

    if (chan->Forbidden())
    {
	SEND(mynick, source, "CS_STATUS/ISFORBIDDEN", (channel));
	return;
    }

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 &&
	(chan->GetAccess(source, "SUPER") ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Invite()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Invite())->IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	    return;
	}
	else if (Magick::instance().chanserv.GetLive(channel)->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/OTH_IN", (target, channel));
	    return;
	}
	SEND(mynick, target, "CS_COMMAND/OTH_INVITE", (channel, source));
    }
    else
    {
	if (Magick::instance().chanserv.GetLive(channel)->IsIn(target))
	{
	    SEND(mynick, source, "CS_STATUS/IN", (channel));
	    return;
	}
	else if (!chan->GetAccess(target, "CMDINVITE"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
	SEND(mynick, target, "CS_COMMAND/INVITE", (channel));
    }

    Magick::instance().chanserv.stats.i_Invite++;
    Magick::instance().server.INVITE(mynick, target, channel);
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
    if (params.WordCount(" ") > 2 &&
	(cstored->GetAccess(source, "SUPER") ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_CS_Unban()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_CS_Unban())->IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsLive(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (target));
	    return;
	}
    }
    else
    {
	if (!cstored->GetAccess(target, "CMDUNBAN"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }

    vector < mstring > bans;
    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(channel);
    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(target);
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

    if (message.After(" ").Matches("ALL", true))
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

    if (message.After(" ").Matches("ALL", true))
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

    if (message.After(" ").Matches("ALL", true))
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

    if (message.After(" ").Matches("ALL", true))
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

    if (message.After(" ").Matches("ALL", true))
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

    if (who.IsNumber())
    {
	if (who.Contains("."))
	{
	    NSEND(mynick, source, "ERR_STYNTAX/WHOLENUMBER");
	    return;
	}
	unsigned int i, num = atoi(who);

	if (num < 1 || num > cstored->Access_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, cstored->Access_size()));
	    return;
	}

	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Access"));
	for (i = 1, cstored->Access = cstored->Access_begin(); i < num && cstored->Access != cstored->Access_end();
	     i++, cstored->Access++);
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
	reason = params.After(" ", 4);

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

    if (who.IsNumber())
    {
	if (who.Contains("."))
	{
	    NSEND(mynick, source, "ERR_STYNTAX/WHOLENUMBER");
	    return;
	}
	unsigned int i, num = atoi(who);

	if (num < 1 || num > cstored->Akick_size())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, cstored->Akick_size()));
	    return;
	}

	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Akick"));
	for (i = 1, cstored->Akick = cstored->Akick_begin(); i < num && cstored->Akick != cstored->Akick_end();
	     i++, cstored->Akick++);
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
    mstring option = params.After(" ", 3);

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
	    option = params.After(" ", 4);
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
	for (cstored->Greet = cstored->Greet_begin(); cstored->Greet != cstored->Greet_end(); cstored->Greet++)
	{
	    if (all || cstored->Greet->Last_Modifier().IsSameAs(target, true))
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
    mstring text = params.After(" ", 3);

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
	SEND(mynick, source, "CS_STATUS/MAX_MESSAGES", (channel));
	return;
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

    int num = atoi(target);

    if (num < 1 || num > static_cast < int > (cstored->Message_size()))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, cstored->Message_size()));
	return;
    }

    if (!cstored->Message_size())
    {
	SEND(mynick, source, "LIST/EMPTY2", (channel, Magick::instance().getMessage(source, "LIST/JOINMSG")));
	return;
    }

    {
	MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Message"));
	if (cstored->Message_find(num))
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
	SEND(mynick, source, "CS_STATUS/OTH_TOOMANY", (founder));
	return;
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
	SEND(mynick, source, "CS_STATUS/OTH_TOOMANY", (founder));
	return;
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
    mstring option = params.After(" ", 3);

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
    mstring option = params.After(" ", 3);

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
    mstring option = params.After(" ", 3);

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
    mstring option = params.After(" ", 3);

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
    mstring option = params.After(" ", 3);

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

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	return;
    }

    retval = cstored->Mlock(source, option);
    Magick::instance().chanserv.stats.i_Set++;
    for (unsigned int i = 0; i < retval.size(); i++)
	::send(mynick, source, retval[i]);
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KEEPTOPIC"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KEEPTOPIC"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/PRIVATE"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/SECUREOPS"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/SECURE"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/NOEXPIRE"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/ANARCHY"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KICKONBAN"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/RESTRICTED"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("CS_SET/JOIN"),
	 channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
    mstring option = params.After(" ", 3);

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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KEEPTOPIC"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/TOPICLOCK"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/PRIVATE"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/SECUREOPS"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/SECURE"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/ANARCHY"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/KICKONBAN"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("CS_SET/RESTRICTED"), channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "CHANSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), Magick::instance().getMessage("CS_SET/JOIN"),
	 channel,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	  getMessage(source, "VALS/OFF"))));
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
	    pOut->WriteSubElement(cstored.entry());
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
	{
	    MLOCK((lck_ChanServ, lck_stored, iter->first, "Level"));
	    for (cstored->Level = cstored->Level_begin(); cstored->Level != cstored->Level_end(); cstored->Level++)
	    {
		cstored->Level->PostLoad();
	    }
	}
	{
	    MLOCK((lck_ChanServ, lck_stored, iter->first, "Access"));
	    for (cstored->Access = cstored->Access_begin(); cstored->Access != cstored->Access_end(); cstored->Access++)
	    {
		cstored->Access->PostLoad();
	    }
	}
	{
	    MLOCK((lck_ChanServ, lck_stored, iter->first, "Akick"));
	    for (cstored->Akick = cstored->Akick_begin(); cstored->Akick != cstored->Akick_end(); cstored->Akick++)
	    {
		cstored->Akick->PostLoad();
	    }
	}
	{
	    MLOCK((lck_ChanServ, lck_stored, iter->first, "Greet"));
	    for (cstored->Greet = cstored->Greet_begin(); cstored->Greet != cstored->Greet_end(); cstored->Greet++)
	    {
		cstored->Greet->PostLoad();
	    }
	}
	{
	    MLOCK((lck_ChanServ, lck_stored, iter->first, "Message"));
	    for (cstored->Message = cstored->Message_begin(); cstored->Message != cstored->Message_end(); cstored->Message++)
	    {
		cstored->Message->PostLoad();
	    }
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
