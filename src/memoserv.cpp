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
#define RCSID(x,y) const char *rcsid_memoserv_cpp_ ## x () { return y; }
RCSID(memoserv_cpp, "@(#)$Id$");

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
static MemoServ::nick_memo_t GLOB_nick_memo_t;
static Memo_t GLOB_Memo_t;
static MemoServ::channel_news_t GLOB_channel_news_t;
static News_t GLOB_News_t;
#endif

Memo_t::Memo_t(const mstring & nick, const mstring & sender, const mstring & text, const unsigned long file) : i_Nick(nick),
i_Sender(sender), i_Time(mDateTime::CurrentDateTime()), i_Text(text), i_Read(false), i_File(file)
{
    BTCB();
    FT("Memo_t::Memo_t", (nick, sender, text, file));
    DumpE();
    ETCB();
}

Memo_t &Memo_t::operator=(const Memo_t & in)
{
    BTCB();
    FT("Memo_t::operator=", ("(const Memo_t &) in"));
    i_Nick = in.i_Nick;
    i_Time = in.i_Time;
    i_Sender = in.i_Sender;
    i_Text = in.i_Text;
    i_File = in.i_File;
    i_Read = in.i_Read;
    NRET(Memo_t &, *this);
    ETCB();
}

void Memo_t::ChgNick(const mstring & in)
{
    BTCB();
    FT("Memo_t::ChgNick", (in));
    WLOCK((lck_MemoServ, lck_nick, i_Nick.LowerCase(), "i_Nick"));
    MCB(i_Nick);
    i_Nick = in;
    MCE(i_Nick);
    ETCB();
}

mstring Memo_t::Sender() const
{
    BTCB();
    NFT(("Memo_t::Sender"));
    RLOCK((lck_MemoServ, lck_nick, i_Nick.LowerCase(), "i_Sender"));
    RET(i_Sender);
    ETCB();
}

mDateTime Memo_t::Time() const
{
    BTCB();
    NFT(("Memo_t::Time"));
    RLOCK((lck_MemoServ, lck_nick, i_Nick.LowerCase(), "i_Time"));
    RET(i_Time);
    ETCB();
}

mstring Memo_t::Text() const
{
    BTCB();
    NFT(("Memo_t::Text"));
    RLOCK((lck_MemoServ, lck_nick, i_Nick.LowerCase(), "i_Text"));
    RET(i_Text);
    ETCB();
}

unsigned long Memo_t::File() const
{
    BTCB();
    NFT(("Memo_t::File"));
    RLOCK((lck_MemoServ, lck_nick, i_Nick.LowerCase(), "i_File"));
    RET(i_File);
    ETCB();
}

bool Memo_t::IsRead() const
{
    BTCB();
    NFT(("Memo_t::IsRead"));
    RLOCK((lck_MemoServ, lck_nick, i_Nick.LowerCase(), "i_Read"));
    RET(i_Read);
    ETCB();
}

void Memo_t::Read()
{
    BTCB();
    NFT(("Memo_t::Read"));
    WLOCK((lck_MemoServ, lck_nick, i_Nick.LowerCase(), "i_Read"));
    MCB(i_Read);
    i_Read = true;
    MCE(i_Read);
    ETCB();
}

void Memo_t::Unread()
{
    BTCB();
    NFT(("Memo_t::Unread"));
    WLOCK((lck_MemoServ, lck_nick, i_Nick.LowerCase(), "i_Read"));
    MCB(i_Read);
    i_Read = false;
    MCE(i_Read);
    ETCB();
}

size_t Memo_t::Usage() const
{
    BTCB();
    size_t retval = 0;

    WLOCK((lck_MemoServ, lck_nick, i_Nick.LowerCase()));
    retval += i_Nick.capacity();
    retval += i_Sender.capacity();
    retval += i_Text.capacity();
    retval += sizeof(i_Time.Internal());
    retval += sizeof(i_File);
    retval += sizeof(i_Read);
    map < mstring, mstring >::const_iterator j;
    for (j = i_UserDef.begin(); j != i_UserDef.end(); j++)
    {
	retval += j->first.capacity();
	retval += j->second.capacity();
    }
    return retval;
    ETCB();
}

void Memo_t::DumpB() const
{
    BTCB();
    MB(0, (i_Nick, i_Sender, i_Text, i_Time, i_Read, i_File, i_UserDef.size()));
    ETCB();
}

void Memo_t::DumpE() const
{
    BTCB();
    ME(0, (i_Nick, i_Sender, i_Text, i_Time, i_Read, i_File, i_UserDef.size()));
    ETCB();
}

News_t::News_t(const mstring & channel, const mstring & sender, const mstring & text,
	       const bool noexpire) : i_Channel(channel), i_Sender(sender), i_Time(mDateTime::CurrentDateTime()), i_Text(text),
i_NoExpire(noexpire)
{
    BTCB();
    FT("News_t::News_t", (channel, sender, text));
    DumpE();
    ETCB();
}

News_t &News_t::operator=(const News_t & in)
{
    BTCB();
    FT("News_t::operator=", ("(const News_t &) in"));
    i_Channel = in.i_Channel;
    i_Time = in.i_Time;
    i_Sender = in.i_Sender;
    i_Text = in.i_Text;
    i_Read = in.i_Read;
    i_NoExpire = in.i_NoExpire;
    NRET(News_t &, *this);
    ETCB();
}

mstring News_t::Sender() const
{
    BTCB();
    NFT("News_t::Sender");
    RLOCK((lck_MemoServ, lck_channel, i_Channel.LowerCase(), "i_Sender"));
    RET(i_Sender);
    ETCB();
}

mDateTime News_t::Time() const
{
    BTCB();
    NFT("News_t::Time");
    RLOCK((lck_MemoServ, lck_channel, i_Channel.LowerCase(), "i_Time"));
    RET(i_Time);
    ETCB();
}

mstring News_t::Text() const
{
    BTCB();
    NFT("News_t::Text");
    RLOCK((lck_MemoServ, lck_channel, i_Channel.LowerCase(), "i_Text"));
    RET(i_Text);
    ETCB();
}

bool News_t::NoExpire() const
{
    BTCB();
    NFT("News_t::NoExpire");
    RLOCK((lck_MemoServ, lck_channel, i_Channel.LowerCase(), "i_NoExpire"));
    RET(i_NoExpire);
    ETCB();
}

void News_t::NoExpire(const bool in)
{
    BTCB();
    NFT("News_t::NoExpire");
    WLOCK((lck_MemoServ, lck_channel, i_Channel.LowerCase(), "i_NoExpire"));
    MCB(i_NoExpire);
    i_NoExpire = in;
    MCE(i_NoExpire);
    ETCB();
}

bool News_t::IsRead(const mstring & name)
{
    BTCB();
    FT("News_t::IsRead", (name));
    mstring target(name);

    if (!Magick::instance().nickserv.IsStored(name))
	RET(false);
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(name);
	if (!nstored->Host().empty())
	    target = nstored->Host();
    }
    RLOCK((lck_MemoServ, lck_channel, i_Channel.LowerCase(), "i_Read"));
    bool retval(i_Read.find(target.LowerCase()) != i_Read.end());

    RET(retval);
    ETCB();
}

void News_t::Read(const mstring & name)
{
    BTCB();
    FT("News_t::Read", (name));
    mstring target = name;

    if (!Magick::instance().nickserv.IsStored(name))
	return;
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(name);
	if (!nstored->Host().empty())
	    target = nstored->Host();
    }
    WLOCK((lck_MemoServ, lck_channel, i_Channel.LowerCase(), "i_Read"));
    MCB(i_Read.size());
    i_Read.insert(target.LowerCase());
    MCE(i_Read.size());
    ETCB();
}

void News_t::Unread(const mstring & name)
{
    BTCB();
    FT("News_t::Unread", (name));
    mstring target = name;

    if (!Magick::instance().nickserv.IsStored(name))
	return;
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(name);
	if (!nstored->Host().empty())
	    target = nstored->Host();
    }
    WLOCK((lck_MemoServ, lck_channel, i_Channel.LowerCase(), "i_Read"));
    MCB(i_Read.size());
    i_Read.erase(name.LowerCase());
    i_Read.erase(target.LowerCase());
    MCE(i_Read.size());
    ETCB();
}

size_t News_t::Usage() const
{
    BTCB();
    size_t retval = 0;

    WLOCK((lck_MemoServ, lck_channel, i_Channel.LowerCase()));
    retval += i_Channel.capacity();
    retval += i_Sender.capacity();
    retval += i_Text.capacity();
    retval += sizeof(i_Time.Internal());
    retval += sizeof(i_NoExpire);
    set < mstring >::const_iterator i;
    for (i = i_Read.begin(); i != i_Read.end(); i++)
	retval += i->capacity();
    map < mstring, mstring >::const_iterator j;
    for (j = i_UserDef.begin(); j != i_UserDef.end(); j++)
    {
	retval += j->first.capacity();
	retval += j->second.capacity();
    }
    return retval;
    ETCB();
}

void News_t::DumpB() const
{
    BTCB();
    MB(0, (i_Channel, i_Sender, i_Text, i_Time, i_NoExpire, i_Read.size(), i_UserDef.size()));
    ETCB();
}

void News_t::DumpE() const
{
    BTCB();
    ME(0, (i_Channel, i_Sender, i_Text, i_Time, i_NoExpire, i_Read.size(), i_UserDef.size()));
    ETCB();
}

MemoServ::MemoServ()
{
    BTCB();
    NFT("MemoServ::MemoServ");
    messages = true;
    ETCB();
}

void MemoServ::AddCommands()
{
    BTCB();
    NFT("MemoServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name(),
						 MemoServ::do_Help);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "READ*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Read);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNREAD*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_UnRead);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GET*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Get);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LIST*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "VIEW*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SEND*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Send);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FLUSH*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Flush);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FORW*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Forward);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "F*W*D", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Forward);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "REP*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Reply);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CAN*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Cancel);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ABOR*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Cancel);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "DEL*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ERA*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "CONT*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Continue);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "P*VIEW*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_Preview);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FILE*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_File);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "*ATTACH", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_File);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* NOEX*", Magick::instance().commserv.REGD_Name(),
						 MemoServ::do_set_NoExpire);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* #*", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET*", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK #*", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK #*", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    ETCB();
}

void MemoServ::RemCommands()
{
    BTCB();
    NFT("MemoServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "READ*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNREAD*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LIST*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "VIEW*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SEND*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FLUSH*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FORW*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "F*W*D", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "REP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CAN*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ABOR*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "CONT*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "P*VIEW*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FILE*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "*ATTACH", Magick::instance().commserv.REGD_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* NOEX*", Magick::instance().commserv.REGD_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* #*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK #*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK #*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK", Magick::instance().commserv.REGD_Name());
    ETCB();
}

void MemoServ::NickMemoConvert(const mstring & source, const mstring & target)
{
    BTCB();
    FT("MemoServ::NickMemoConvert", (source, target));

    WLOCK((lck_MemoServ, lck_nick, source));

    MemoServ::nick_t::iterator iter = nick.find(source.LowerCase());
    if (iter != nick.end())
    {
	MemoServ::nick_memo_t::iterator iter2;
	for (iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
	{
	    /* (*iter2)->ChgNick(target); */
	    iter2->ChgNick(target);
	    /* AddNickMemo(*iter2); */
	    AddNickMemo(&(*iter2));
	}
	// DELIBERATELY dont delete the members of the MemoServ::nick_memo_t
	// for the source nick ... we're just renaming and moving.
	WLOCK2((lck_MemoServ, lck_nick));
	nick.erase(source.LowerCase());
    }
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::AddNick(nick_memo_t in) throw (E_MemoServ_Nick)
#else
void MemoServ::AddNick(nick_memo_t in)
#endif
{
    BTCB();
    FT("MemoServ::AddNick", ("(nick_memo_t) in"));

    if (!in.size())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Add, E_MemoServ_Nick::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Memo", "Nick", "Add", "Blank"));
	return;
#endif
    }

#if 0
    if (in.begin() == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Add, E_MemoServ_Nick::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Memo", "Nick", "Add", "Invalid"));
	return;
#endif
    }
#endif

    if (in.begin()->Nick().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Add, E_MemoServ_Nick::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Memo", "Nick", "Add", "Blank"));
	return;
#endif
    }

    WLOCK((lck_MemoServ, lck_nick));
    nick[in.begin()->Nick().LowerCase()] = in;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::AddNickMemo(Memo_t * in) throw (E_MemoServ_Nick)
#else
void MemoServ::AddNickMemo(Memo_t * in)
#endif
{
    BTCB();
    FT("MemoServ::AddNickMemo", ("(Memo_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Add, E_MemoServ_Nick::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Memo", "NickMemo", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Nick().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Add, E_MemoServ_Nick::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Memo", "NickMemo", "Add", "Blank"));
	return;
#endif
    }

    WLOCK((lck_MemoServ, lck_nick));
    WLOCK2((lck_MemoServ, lck_nick, in->Nick().LowerCase()));
    /* nick[in->Nick().LowerCase()].push_back(in); */
    nick[in->Nick().LowerCase()].push_back(*in);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
MemoServ::nick_memo_t & MemoServ::GetNick(const mstring & in) const throw (E_MemoServ_Nick)
#else
MemoServ::nick_memo_t & MemoServ::GetNick(const mstring & in) const
#endif
{
    BTCB();
    FT("MemoServ::GetNick", (in));

    RLOCK((lck_MemoServ, lck_nick, in.LowerCase()));
    MemoServ::nick_t::const_iterator iter = nick.find(in.LowerCase());
    if (iter == nick.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Get, E_MemoServ_Nick::T_NotFound, in.c_str()));
#else
	NLOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Memo", "Nick", "Get", "NotFound", in));
	NRET(MemoServ::nick_memo_t &, GLOB_nick_memo_t);
#endif
    }
    if (!iter->second.size())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Get, E_MemoServ_Nick::T_Blank, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Memo", "Nick", "Get", "Blank", in));
	NRET(MemoServ::nick_memo_t &, GLOB_nick_memo_t);
#endif
    }

    NRET(MemoServ::nick_memo_t &, const_cast < MemoServ::nick_memo_t & > (iter->second));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
Memo_t &MemoServ::GetNickMemo(const mstring & in, const size_t num) const throw (E_MemoServ_Nick)
#else
Memo_t &MemoServ::GetNickMemo(const mstring & in, const size_t num) const
#endif
{
    BTCB();
    FT("MemoServ::GetNickMemo", (in, num));

    RLOCK((lck_MemoServ, lck_nick, in.LowerCase()));
    MemoServ::nick_memo_t & ent = GetNick(in);

    size_t i;

    MemoServ::nick_memo_t::const_iterator iter;
    for (iter = ent.begin(), i = 0; iter != ent.end() && i < num; iter++, i++);

    if (i < num || iter == ent.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Get, E_MemoServ_Nick::T_NotFound, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("Memo", "NickMemo", "Get", "NotFound", in, num));
	NRET(Memo_t &, GLOB_Memo_t);
#endif
    }
    /* if (*iter == NULL)
     * {
     * #ifdef MAGICK_HAS_EXCEPTIONS
     * throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Get, E_MemoServ_Nick::T_Invalid, in.c_str(), num))
     * #else
     * LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("Memo", "NickMemo", "Get", "Invalid", in, num));
     * NRET(Memo_t &, GLOB_Memo_t);
     * #endif
     * }
     * if ((*iter)->Nick().empty()) */
    if (iter->Nick().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Get, E_MemoServ_Nick::T_Blank, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("Memo", "NickMemo", "Get", "Blank", in, num));
	NRET(Memo_t &, GLOB_Memo_t);
#endif
    }

    /* NRET(Memo_t &, const_cast<Memo_t &>(**iter)); */
    NRET(Memo_t &, const_cast < Memo_t & > (*iter));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::RemNick(const mstring & in) throw (E_MemoServ_Nick)
#else
void MemoServ::RemNick(const mstring & in)
#endif
{
    BTCB();
    FT("MemoServ::RemNick", (in));

    WLOCK((lck_MemoServ, lck_nick));
    MemoServ::nick_t::iterator iter = nick.find(in.LowerCase());
    if (iter == nick.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Rem, E_MemoServ_Nick::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Memo", "Nick", "Rem", "NotFound", in));
	return;
#endif
    }
    WLOCK2((lck_MemoServ, lck_nick, iter->first));
    /* nick_memo_t::iterator iter2;
     * for (iter2=iter->second.begin(); iter2!=iter->second.end(); iter2++)
     * if (*iter2 != NULL)
     * delete *iter2;
     */
    nick.erase(iter);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::RemNickMemo(const mstring & in, const size_t num) throw (E_MemoServ_Nick)
#else
void MemoServ::RemNickMemo(const mstring & in, const size_t num)
#endif
{
    BTCB();
    FT("MemoServ::RemNickMemo", (in, num));

    WLOCK((lck_MemoServ, lck_nick));
    MemoServ::nick_t::iterator iter = nick.find(in.LowerCase());
    if (iter == nick.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Rem, E_MemoServ_Nick::T_NotFound, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("Memo", "Nick", "Rem", "NotFound", in, num));
	return;
#endif
    }
    WLOCK2((lck_MemoServ, lck_nick, iter->first));

    size_t i;

    MemoServ::nick_memo_t::iterator iter2;
    for (iter2 = iter->second.begin(), i = 0; iter2 != iter->second.end() && i < num; iter2++, i++);

    if (i < num || iter2 == iter->second.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Nick(E_MemoServ_Nick::W_Rem, E_MemoServ_Nick::T_NotFound, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("Memo", "NickMemo", "Rem", "NotFound", in, num));
	return;
#endif
    }
    /* if (*iter2 != NULL)
     * delete *iter2 */
    iter->second.erase(iter2);

    if (!iter->second.size())
	nick.erase(iter);
    ETCB();
}

bool MemoServ::IsNick(const mstring & in) const
{
    BTCB();
    FT("MemoServ::IsNick", (in));
    RLOCK((lck_MemoServ, lck_nick));
    bool retval = (nick.find(in.LowerCase()) != nick.end());

    RET(retval);
    ETCB();
}

bool MemoServ::IsNickMemo(const mstring & in, const size_t num) const
{
    BTCB();
    FT("MemoServ::IsNickMemo", (in, num));

    RLOCK((lck_MemoServ, lck_nick));
    MemoServ::nick_t::const_iterator iter = nick.find(in.LowerCase());
    if (iter != nick.end())
    {
	size_t i;

	MemoServ::nick_memo_t::const_iterator iter2;
	for (iter2 = iter->second.begin(), i = 0; iter2 != iter->second.end() && i < num; iter2++, i++);

	if (!(i < num || iter2 == iter->second.end()))
	{
	    RET(true);
	}
    }
    RET(false);
    ETCB();
}

size_t MemoServ::NickMemoCount(const mstring & in, const bool isread) const
{
    BTCB();
    FT("MemoServ::NickMemoCount", (in, isread));

    size_t retval = 0;

    RLOCK((lck_MemoServ, lck_nick, in.LowerCase()));
    MemoServ::nick_t::const_iterator iter = nick.find(in.LowerCase());
    if (iter != nick.end())
    {
	MemoServ::nick_memo_t::const_iterator iter2;
	for (iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
	{
	    if (iter2->IsRead() == isread)
		retval++;
	}
    }
    RET(retval);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::AddChannel(channel_news_t in) throw (E_MemoServ_Channel)
#else
void MemoServ::AddChannel(channel_news_t in)
#endif
{
    BTCB();
    FT("MemoServ::AddChannel", ("(channel_news_t) in"));

    if (!in.size())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Add, E_MemoServ_Channel::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("News", "Channel", "Add", "Blank"));
	return;
#endif
    }

#if 0
    if (in.begin() == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Add, E_MemoServ_Channel::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("News", "Channel", "Add", "Invalid"));
	return;
#endif
    }
#endif

    if (in.begin()->Channel().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Add, E_MemoServ_Channel::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("News", "Channel", "Add", "Blank"));
	return;
#endif
    }

    WLOCK((lck_MemoServ, lck_channel));
    channel[in.begin()->Channel().LowerCase()] = in;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::AddChannelNews(News_t * in) throw (E_MemoServ_Channel)
#else
void MemoServ::AddChannelNews(News_t * in)
#endif
{
    BTCB();
    FT("MemoServ::AddChannelNews", ("(News_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Add, E_MemoServ_Channel::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("News", "ChannelNews", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Channel().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Add, E_MemoServ_Channel::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("News", "ChannelNews", "Add", "Blank"));
	return;
#endif
    }

    WLOCK((lck_MemoServ, lck_channel));
    WLOCK2((lck_MemoServ, lck_channel, in->Channel().LowerCase()));
    /* channel[in->Channel().LowerCase()].push_back(in); */
    channel[in->Channel().LowerCase()].push_back(*in);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
MemoServ::channel_news_t & MemoServ::GetChannel(const mstring & in) const throw (E_MemoServ_Channel)
#else
MemoServ::channel_news_t & MemoServ::GetChannel(const mstring & in) const
#endif
{
    BTCB();
    FT("MemoServ::GetChannel", (in));

    RLOCK((lck_MemoServ, lck_channel, in.LowerCase()));
    MemoServ::channel_t::const_iterator iter = channel.find(in.LowerCase());
    if (iter == channel.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Get, E_MemoServ_Channel::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("News", "Channel", "Get", "NotFound", in));
	NRET(MemoServ::channel_news_t &, GLOB_channel_news_t);
#endif
    }
    if (!iter->second.size())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Get, E_MemoServ_Channel::T_Blank, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("News", "Channel", "Get", "Blank", in));
	NRET(MemoServ::channel_news_t &, GLOB_channel_news_t);
#endif
    }

    NRET(MemoServ::channel_news_t &, const_cast < MemoServ::channel_news_t & > (iter->second));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
News_t &MemoServ::GetChannelNews(const mstring & in, const size_t num) const throw (E_MemoServ_Channel)
#else
News_t &MemoServ::GetChannelNews(const mstring & in, const size_t num) const
#endif
{
    BTCB();
    FT("MemoServ::GetChannelNews", (in, num));

    RLOCK((lck_MemoServ, lck_channel, in.LowerCase()));
    MemoServ::channel_news_t & ent = GetChannel(in);

    size_t i;

    MemoServ::channel_news_t::const_iterator iter;
    for (iter = ent.begin(), i = 0; iter != ent.end() && i < num; iter++, i++);

    if (i < num || iter == ent.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Get, E_MemoServ_Channel::T_NotFound, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("News", "ChannelNews", "Get", "NotFound", in, num));
	NRET(News_t &, GLOB_News_t);
#endif
    }
    /* if (*iter == NULL)
     * {
     * #ifdef MAGICK_HAS_EXCEPTIONS
     * throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Get, E_MemoServ_Channel::T_Invalid, in.c_str(), num))
     * #else
     * LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("News", "ChannelNews", "Get", "Invalid", in, num));
     * NRET(News_t &, GLOB_News_t);
     * #endif
     * }
     * if ((*iter)->Channel().empty()) */
    if (iter->Channel().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Get, E_MemoServ_Channel::T_Blank, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("News", "ChannelNews", "Get", "Blank", in, num));
	NRET(News_t &, GLOB_News_t);
#endif
    }

    /* NRET(News_t &, const_cast<News_t &>(**iter)); */
    NRET(News_t &, const_cast < News_t & > (*iter));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::RemChannel(const mstring & in) throw (E_MemoServ_Channel)
#else
void MemoServ::RemChannel(const mstring & in)
#endif
{
    BTCB();
    FT("MemoServ::RemChannel", (in));

    WLOCK((lck_MemoServ, lck_channel));
    MemoServ::channel_t::iterator iter = channel.find(in.LowerCase());
    if (iter == channel.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Rem, E_MemoServ_Channel::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("News", "Channel", "Rem", "NotFound", in));
	return;
#endif
    }
    WLOCK2((lck_MemoServ, lck_channel, iter->first));
    /* channel_news_t::iterator iter2;
     * for (iter2=iter->second.begin(); iter2!=iter->second.end(); iter2++)
     * if (*iter2 != NULL)
     * delete *iter2;
     */
    channel.erase(iter);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::RemChannelNews(const mstring & in, const size_t num) throw (E_MemoServ_Channel)
#else
void MemoServ::RemChannelNews(const mstring & in, const size_t num)
#endif
{
    BTCB();
    FT("MemoServ::RemChannelNews", (in, num));

    WLOCK((lck_MemoServ, lck_channel));
    MemoServ::channel_t::iterator iter = channel.find(in.LowerCase());
    if (iter == channel.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Rem, E_MemoServ_Channel::T_NotFound, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("News", "Channel", "Rem", "NotFound", in, num));
	return;
#endif
    }
    WLOCK2((lck_MemoServ, lck_channel, iter->first));

    size_t i;

    MemoServ::channel_news_t::iterator iter2;
    for (iter2 = iter->second.begin(), i = 0; iter2 != iter->second.end() && i < num; iter2++, i++);

    if (i < num || iter2 == iter->second.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_MemoServ_Channel(E_MemoServ_Channel::W_Rem, E_MemoServ_Channel::T_NotFound, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("News", "ChannelNews", "Rem", "NotFound", in, num));
	return;
#endif
    }
    /* if (*iter2 != NULL)
     * delete *iter2 */
    iter->second.erase(iter2);

    if (!iter->second.size())
	channel.erase(iter);
    ETCB();
}

bool MemoServ::IsChannel(const mstring & in) const
{
    BTCB();
    FT("MemoServ::IsChannel", (in));
    RLOCK((lck_MemoServ, lck_channel));
    bool retval = (channel.find(in.LowerCase()) != channel.end());

    RET(retval);
    ETCB();
}

bool MemoServ::IsChannelNews(const mstring & in, const size_t num) const
{
    BTCB();
    FT("MemoServ::IsChannelNews", (in, num));

    RLOCK((lck_MemoServ, lck_channel));
    MemoServ::channel_t::const_iterator iter = channel.find(in.LowerCase());
    if (iter != channel.end())
    {
	size_t i;

	MemoServ::channel_news_t::const_iterator iter2;
	for (iter2 = iter->second.begin(), i = 0; iter2 != iter->second.end() && i < num; iter2++, i++);

	if (!(i < num || iter2 == iter->second.end()))
	{
	    RET(true);
	}
    }
    RET(false);
    ETCB();
}

size_t MemoServ::ChannelNewsCount(const mstring & in, const mstring & user, const bool isread)
{
    BTCB();
    FT("MemoServ::ChannelNewsCount", (in, user, isread));

    size_t retval = 0;

    RLOCK((lck_MemoServ, lck_channel, in.LowerCase()));
    MemoServ::channel_t::iterator iter = channel.find(in.LowerCase());
    if (iter != channel.end())
    {
	MemoServ::channel_news_t::iterator iter2;
	for (iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
	{
	    if (iter2->IsRead(user) == isread)
		retval++;
	}
    }
    RET(retval);
    ETCB();
}

void MemoServ::execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    mThread::ReAttach(tt_MemoServ);
    FT("MemoServ::execute", (source, msgtype, params));
    //okay this is the main memoserv command switcher

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

void MemoServ::do_Help(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Help", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    mstring HelpTopic = Magick::instance().memoserv.GetInternalName();

    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector < mstring > help = Magick::instance().getHelp(source, HelpTopic.UpperCase());

    unsigned int i;

    for (i = 0; i < help.size(); i++)
	::send(mynick, source, help[i]);
    ETCB();
}

void MemoServ::do_Read(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Read", (mynick, source, params));

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

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	    SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.After(" ", 2);
	mstring whoami = Magick::instance().nickserv.GetStored(source)->Host().LowerCase();

	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Magick::instance().chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (who));
	    return;
	}
	who = Magick::instance().getSname(who);

	if (!Magick::instance().chanserv.GetStored(who)->GetAccess(whoami, "READMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Magick::instance().memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (who.LowerCase()));
	    return;
	}

	Magick::instance().memoserv.stats.i_Read++;
	if (what.IsSameAs("all", true) || what.IsSameAs("new", true) || what.IsSameAs("unread", true))
	{
	    bool unread = (what.IsSameAs("new", true) || what.IsSameAs("unread", true));

	    MemoServ::channel_news_t::iterator iter;
	    int i = 0;

	    RLOCK((lck_MemoServ, lck_channel, who.LowerCase()));
	    for (iter = Magick::instance().memoserv.ChannelNewsBegin(who);
		 iter != Magick::instance().memoserv.ChannelNewsEnd(who); iter++)
	    {
		i++;
		if (unread && iter->IsRead(whoami))
		    continue;

		iter->Read(whoami);
		SEND(mynick, source, "MS_COMMAND/NEWS", (i, iter->Sender(), iter->Channel(), iter->Time().Ago()));
		unsigned int sentsize;
		mstring output;

		for (sentsize = 0; sentsize < iter->Text().size(); sentsize += Magick::instance().server.proto.MaxLine())
		{
		    if (sentsize + Magick::instance().server.proto.MaxLine() < iter->Text().size())
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize,
							 sentsize + Magick::instance().server.proto.MaxLine()) << "...";
		    }
		    else
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize, iter->Text().size() - 1);
		    }
		    ::send(mynick, source, "    " + output);
		    output.erase();
		}
	    }
	    if (i == 1 && unread)
		SEND(mynick, source, "MS_STATUS/CS_NOUNREAD", (who.LowerCase()));
	}
	else
	{
	    vector < int > numbers = ParseNumbers(what);
	    unsigned int i;
	    int j = 1;
	    mstring output;
	    bool triedabove = false, nonnumeric = false;
	    size_t amt = 0;

	    RLOCK((lck_MemoServ, lck_channel, who.LowerCase()));
	    MemoServ::channel_news_t & newslist = Magick::instance().memoserv.GetChannel(who);
	    amt = newslist.size();
	    MemoServ::channel_news_t::iterator iter = newslist.begin();
	    for (i = 0; i < numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > static_cast < int > (newslist.size()))
		    triedabove = true;
		else
		{
		    while (numbers[i] < j && iter != newslist.begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j && iter != newslist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != newslist.end())
		    {
			iter->Read(whoami);
			SEND(mynick, source, "MS_COMMAND/NEWS", (j, iter->Sender(), iter->Sender(), iter->Time().Ago()));
			unsigned int sentsize;

			for (sentsize = 0; sentsize < iter->Text().size();
			     sentsize += Magick::instance().server.proto.MaxLine())
			{
			    if (sentsize + Magick::instance().server.proto.MaxLine() < iter->Text().size())
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize,
								 sentsize +
								 Magick::instance().server.proto.MaxLine()) << "...";
			    }
			    else
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize, iter->Text().size() - 1);
			    }
			    ::send(mynick, source, "    " + output);
			    output.erase();
			}
		    }
		}
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", (amt));
	}
    }
    else
    {
	mstring who = Magick::instance().nickserv.GetStored(source)->Host();
	mstring what = params.After(" ", 1);

	if (who.empty())
	    who = source;

	if (!Magick::instance().memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	Magick::instance().memoserv.stats.i_Read++;
	if (what.IsSameAs("all", true) || what.IsSameAs("new", true) || what.IsSameAs("unread", true))
	{
	    MemoServ::nick_memo_t::iterator iter;
	    bool unread = (what.IsSameAs("new", true) || what.IsSameAs("unread", true));
	    int i = 0;

	    RLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
	    for (iter = Magick::instance().memoserv.NickMemoBegin(who); iter != Magick::instance().memoserv.NickMemoEnd(who);
		 iter++)
	    {
		i++;
		if (unread && iter->IsRead())
		    continue;

		iter->Read();
		if (iter->File() && Magick::instance().filesys.Exists(FileMap::MemoAttach, iter->File()))
		{
		    SEND(mynick, source, "MS_COMMAND/MEMO_FILE",
			 (i, iter->Sender(), iter->Time().Ago(),
			  Magick::instance().filesys.GetName(FileMap::MemoAttach, iter->File()),
			  ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::MemoAttach, iter->File()))));
		}
		else
		{
		    SEND(mynick, source, "MS_COMMAND/MEMO", (i, iter->Sender(), iter->Time().Ago()));
		}
		unsigned int sentsize;
		mstring output;

		for (sentsize = 0; sentsize < iter->Text().size(); sentsize += Magick::instance().server.proto.MaxLine())
		{
		    if (sentsize + Magick::instance().server.proto.MaxLine() < iter->Text().size())
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize,
							 sentsize + Magick::instance().server.proto.MaxLine()) << "...";
		    }
		    else
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize, iter->Text().size() - 1);
		    }
		    ::send(mynick, source, "    " + output);
		    output.erase();
		}
	    }
	}
	else
	{
	    vector < int > numbers = ParseNumbers(what);
	    unsigned int i;
	    int j = 1;
	    bool triedabove = false, nonnumeric = false;
	    size_t amt = 0;

	    RLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
	    MemoServ::nick_memo_t & memolist = Magick::instance().memoserv.GetNick(who);
	    amt = memolist.size();
	    MemoServ::nick_memo_t::iterator iter = memolist.begin();
	    mstring output;

	    for (i = 0; i < numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > static_cast < int > (memolist.size()))
		    triedabove = true;
		else
		{
		    while (numbers[i] < j && iter != memolist.begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j && iter != memolist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != memolist.end())
		    {
			iter->Read();
			if (iter->File() && Magick::instance().filesys.Exists(FileMap::MemoAttach, iter->File()))
			{
			    SEND(mynick, source, "MS_COMMAND/MEMO_FILE",
				 (j, iter->Sender(), iter->Time().Ago(),
				  Magick::instance().filesys.GetName(FileMap::MemoAttach, iter->File()),
				  ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::MemoAttach, iter->File()))));
			}
			else
			{
			    SEND(mynick, source, "MS_COMMAND/MEMO", (j, iter->Sender(), iter->Time().Ago()));
			}
			unsigned int sentsize;

			for (sentsize = 0; sentsize < iter->Text().size();
			     sentsize += Magick::instance().server.proto.MaxLine())
			{
			    if (sentsize + Magick::instance().server.proto.MaxLine() < iter->Text().size())
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize,
								 sentsize +
								 Magick::instance().server.proto.MaxLine()) << "...";
			    }
			    else
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize, iter->Text().size() - 1);
			    }
			    ::send(mynick, source, "    " + output);
			    output.erase();
			}
		    }
		}
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", (amt));
	}
    }
    ETCB();
}

void MemoServ::do_UnRead(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_UnRead", (mynick, source, params));

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

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	    SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.After(" ", 2);
	mstring whoami = Magick::instance().nickserv.GetStored(source)->Host().LowerCase();

	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Magick::instance().chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (who));
	    return;
	}
	who = Magick::instance().getSname(who);

	if (!Magick::instance().chanserv.GetStored(who)->GetAccess(whoami, "READMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Magick::instance().memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (who.LowerCase()));
	    return;
	}

	Magick::instance().memoserv.stats.i_Unread++;
	if (what.IsSameAs("all", true))
	{
	    MemoServ::channel_news_t::iterator iter;
	    mstring output;

	    RLOCK((lck_MemoServ, lck_channel, who.LowerCase()));
	    for (iter = Magick::instance().memoserv.ChannelNewsBegin(who);
		 iter != Magick::instance().memoserv.ChannelNewsEnd(who); iter++)
	    {
		iter->Unread(whoami);
	    }
	    SEND(mynick, source, "MS_COMMAND/UNREAD_ALL", (who.LowerCase()));
	}
	else
	{
	    vector < int > numbers = ParseNumbers(what);
	    unsigned int i;
	    int j = 1;
	    bool triedabove = false, nonnumeric = false;
	    size_t amt = 0;

	    RLOCK((lck_MemoServ, lck_channel, who.LowerCase()));
	    MemoServ::channel_news_t & newslist = Magick::instance().memoserv.GetChannel(who);
	    amt = newslist.size();
	    MemoServ::channel_news_t::iterator iter = newslist.begin();
	    mstring output;

	    for (i = 0; i < numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > static_cast < int > (newslist.size()))
		    triedabove = true;
		else
		{
		    while (numbers[i] < j && iter != newslist.begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j && iter != newslist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != newslist.end())
		    {
			iter->Unread(whoami);
			if (!output.empty())
			    output << ", ";
			output << j;
		    }
		}
	    }
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/CS_UNREAD", (output, who));
		output.erase();
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", (amt));
	}
    }
    else
    {
	mstring who = Magick::instance().nickserv.GetStored(source)->Host();
	mstring what = params.After(" ", 1);

	if (who.empty())
	    who = source;

	if (!Magick::instance().memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	Magick::instance().memoserv.stats.i_Unread++;
	if (what.IsSameAs("all", true))
	{
	    MemoServ::nick_memo_t::iterator iter;
	    mstring output;

	    RLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
	    for (iter = Magick::instance().memoserv.NickMemoBegin(who); iter != Magick::instance().memoserv.NickMemoEnd(who);
		 iter++)
	    {
		iter->Unread();
	    }
	    NSEND(mynick, source, "MS_COMMAND/NS_UNREAD_ALL");
	}
	else
	{
	    vector < int > numbers = ParseNumbers(what);
	    unsigned int i;
	    int j = 1;
	    bool triedabove = false, nonnumeric = false;
	    size_t amt = 0;

	    RLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
	    MemoServ::nick_memo_t & memolist = Magick::instance().memoserv.GetNick(who);
	    amt = memolist.size();
	    MemoServ::nick_memo_t::iterator iter = memolist.begin();
	    mstring output;

	    for (i = 0; i < numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > static_cast < int > (memolist.size()))
		    triedabove = true;
		else
		{
		    while (numbers[i] < j && iter != memolist.begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j && iter != memolist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != memolist.end())
		    {
			iter->Unread();
			if (!output.empty())
			    output << ", ";
			output << j;
		    }
		}
	    }
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/NS_UNREAD", (output));
		output.erase();
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", (amt));
	}
    }
    ETCB();
}

void MemoServ::do_Get(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Get", (mynick, source, params));

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

    mstring who = Magick::instance().nickserv.GetStored(source)->Host();
    mstring what = params.After(" ", 1);

    if (who.empty())
	who = source;

    if (!Magick::instance().memoserv.IsNick(who))
    {
	NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	return;
    }

    vector < int > numbers = ParseNumbers(what);
    unsigned int i;
    int j = 1;
    bool triedabove = false, nonnumeric = false, nonfiles = false;
    size_t amt = 0;

    RLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
    MemoServ::nick_memo_t & memolist = Magick::instance().memoserv.GetNick(who);
    amt = memolist.size();
    MemoServ::nick_memo_t::iterator iter = memolist.begin();
    mstring output;

    Magick::instance().memoserv.stats.i_Get++;
    for (i = 0; i < numbers.size(); i++)
    {
	if (numbers[i] <= 0)
	    nonnumeric = true;
	else if (numbers[i] > static_cast < int > (memolist.size()))
	    triedabove = true;
	else
	{
	    while (numbers[i] < j && iter != memolist.begin())
	    {
		j--;
		iter--;
	    }
	    while (numbers[i] > j && iter != memolist.end())
	    {
		j++;
		iter++;
	    }
	    if (iter != memolist.end())
	    {
		unsigned long filenum = iter->File();

		if (!(filenum && Magick::instance().filesys.Exists(FileMap::MemoAttach, filenum)))
		{
		    nonfiles = true;
		    continue;
		}

		mstring filename = Magick::instance().filesys.GetName(FileMap::MemoAttach, filenum);
		size_t fsize = Magick::instance().filesys.GetSize(FileMap::MemoAttach, filenum);

		if (filename.empty() || fsize <= 0)
		{
		    nonfiles = true;
		    continue;
		}

		if (!
		    (Magick::instance().files.TempDirSize() == 0 ||
		     mFile::DirUsage(Magick::instance().files.TempDir()) <= Magick::instance().files.TempDirSize()))
		{
		    NSEND(mynick, source, "DCC/NOSPACE2");
		    return;
		}

		{
		    RLOCK2(("DCC"));
		    if (Magick::instance().dcc != NULL)
		    {
			unsigned short port = mSocket::FindAvailPort();

			::privmsg(mynick, source,
				  DccEngine::encode("DCC SEND",
						    filename + " " + mstring(Magick::instance().LocalHost()) + " " +
						    mstring(port) + " " + mstring(fsize)));
			Magick::instance().dcc->Accept(port, mynick, source, FileMap::MemoAttach, filenum);
		    }
		}
	    }
	}
    }
    if (nonnumeric)
	NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
    if (triedabove)
	SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", (amt));
    if (nonfiles)
	NSEND(mynick, source, "ERR_SYNTAX/NONFILES");
    ETCB();
}

void MemoServ::do_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_List", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") > 1 && IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 2)
	{
	    SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring whoami = Magick::instance().nickserv.GetStored(source)->Host().LowerCase();

	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Magick::instance().chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (who));
	    return;
	}
	who = Magick::instance().getSname(who);

	if (!Magick::instance().chanserv.GetStored(who)->GetAccess(whoami, "READMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Magick::instance().memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (who.LowerCase()));
	    return;
	}

	MemoServ::channel_news_t::iterator iter;
	int i = 1;
	mstring output;

	RLOCK((lck_MemoServ, lck_channel, who.LowerCase()));
	for (iter = Magick::instance().memoserv.ChannelNewsBegin(who); iter != Magick::instance().memoserv.ChannelNewsEnd(who);
	     iter++)
	{
	    output.erase();
	    if (iter->Text().size() > 20)
		output << iter->Text().SubString(0, 19) << "...";
	    else
		output << iter->Text();

	    SEND(mynick, source, "MS_COMMAND/NEWS_LIST",
		 (iter->IsRead(whoami) ? ' ' : '*', i++, iter->Sender(), iter->Channel(), iter->Time().Ago(), output));
	}
    }
    else
    {
	mstring who = Magick::instance().nickserv.GetStored(source)->Host();

	if (who.empty())
	    who = source;

	if (!Magick::instance().memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	MemoServ::nick_memo_t::iterator iter;
	int i = 1;
	mstring output;

	RLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
	for (iter = Magick::instance().memoserv.NickMemoBegin(who); iter != Magick::instance().memoserv.NickMemoEnd(who);
	     iter++)
	{
	    output.erase();
	    if (iter->Text().size() > 20)
		output << iter->Text().SubString(0, 19) << "...";
	    else
		output << iter->Text();

	    if (iter->File() && Magick::instance().filesys.Exists(FileMap::MemoAttach, iter->File()))
	    {
		SEND(mynick, source, "MS_COMMAND/MEMO_LIST_FILE",
		     (iter->IsRead() ? ' ' : '*', i++, iter->Sender(), iter->Time().Ago(), output,
		      Magick::instance().filesys.GetName(FileMap::MemoAttach, iter->File()),
		      ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::MemoAttach, iter->File()))));
	    }
	    else
	    {
		SEND(mynick, source, "MS_COMMAND/MEMO_LIST",
		     (iter->IsRead() ? ' ' : '*', i++, iter->Sender(), iter->Time().Ago(), output));
	    }
	}
    }
    ETCB();
}

void MemoServ::do_Send(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Send", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

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

    mstring name = params.ExtractWord(2, " ");
    mstring text = params.After(" ", 2);

    if (IsChan(name))
    {
	if (!Magick::instance().chanserv.IsStored(name))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (name));
	    return;
	}
	name = Magick::instance().getSname(name);

	if (!Magick::instance().chanserv.GetStored(name)->GetAccess(source, "WRITEMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }
    else
    {
	if (!Magick::instance().nickserv.IsStored(name))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (name));
	    return;
	}

	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(name);
	name = nstored->Name();
	if (nstored->Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (name));
	    return;
	}

	if (nstored->IsIgnore(source) ? !nstored->NoMemo() : nstored->NoMemo())
	{
	    SEND(mynick, source, "MS_STATUS/IGNORE", (name));
	    return;
	}
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->HasMode("o") && nlive->LastMemo().SecondsSince() < Magick::instance().memoserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET",
	     (message, ToHumanTime(Magick::instance().memoserv.Delay() - nlive->LastMemo().SecondsSince(), source)));
	return;
    }

    if (text.size() > Magick::instance().server.proto.MaxLine())
    {
	text.Truncate(Magick::instance().server.proto.MaxLine());
	SEND(mynick, source, "MS_STATUS/TRUNCATE",
	     (text.SubString(Magick::instance().server.proto.MaxLine() - 20, Magick::instance().server.proto.MaxLine()),
	      mynick));
    }

    Magick::instance().memoserv.stats.i_Send++;
    nlive->InFlight.Memo(false, mynick, name, text);
    ETCB();
}

void MemoServ::do_Flush(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Flush", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (nlive->InFlight.Memo())
    {
	if (nlive->InFlight.File())
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOFLUSH");
	    return;
	}
	Magick::instance().memoserv.stats.i_Flush++;
	nlive->InFlight.End(0u);
    }
    else
    {
	NSEND(mynick, source, "MS_STATUS/NOPENDING");
    }
    ETCB();
}

void MemoServ::do_Forward(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Forward", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

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

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 4)
	{
	    SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.ExtractWord(3, " ");
	mstring dest = params.ExtractWord(4, " ");
	mstring whoami = Magick::instance().nickserv.GetStored(source)->Host().LowerCase();

	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Magick::instance().chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (who));
	    return;
	}
	who = Magick::instance().getSname(who);

	if (!Magick::instance().chanserv.GetStored(who)->GetAccess(whoami, "READ"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Magick::instance().memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (who.LowerCase()));
	    return;
	}

	if (!what.IsNumber() || what.Contains("."))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(what.c_str());

	if (num <= 0 || num > Magick::instance().memoserv.ChannelNewsSize(who))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().memoserv.ChannelNewsSize(who)));
	    return;
	}

	map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
	if (!nlive->HasMode("o") && nlive->LastMemo().SecondsSince() < Magick::instance().memoserv.Delay())
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTYET",
		 (message, ToHumanTime(Magick::instance().memoserv.Delay() - nlive->LastMemo().SecondsSince(), source)));
	    return;
	}

	unsigned int i;
	mstring output;

	{
	    RLOCK((lck_MemoServ, lck_channel, who.LowerCase()));
	    MemoServ::channel_news_t::iterator iter = Magick::instance().memoserv.ChannelNewsEnd(who);
	    for (i = 1; i < num; iter++, i++);
	    output =
		parseMessage(Magick::instance().getMessage(dest, "MS_STATUS/FORWARD_ARG"),
			     mVarArray(Magick::instance().chanserv.GetStored(who)->Name(), iter->Sender(), iter->Text()));
	}

	do_Forward2(mynick, source, dest, output);
    }
    else
    {
	mstring who = Magick::instance().nickserv.GetStored(source)->Host();
	mstring what = params.ExtractWord(2, " ");
	mstring dest = params.ExtractWord(3, " ");

	if (who.empty())
	    who = source;

	if (!Magick::instance().memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	if (!what.IsNumber() || what.Contains("."))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(what.c_str());

	if (num <= 0 || num > Magick::instance().memoserv.NickMemoSize(who))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().memoserv.NickMemoSize(who)));
	    return;
	}

	map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
	if (!nlive->HasMode("o") && nlive->LastMemo().SecondsSince() < Magick::instance().memoserv.Delay())
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTYET",
		 (message, ToHumanTime(Magick::instance().memoserv.Delay() - nlive->LastMemo().SecondsSince(), source)));
	    return;
	}

	unsigned int i;
	mstring output;

	{
	    RLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
	    MemoServ::nick_memo_t::iterator iter = Magick::instance().memoserv.NickMemoBegin(who);
	    for (i = 1; i < num; iter++, i++);

	    if (iter->File())
		output =
		    parseMessage(Magick::instance().getMessage(dest, "MS_STATUS/FORWARD_ARG"),
				 mVarArray(Magick::instance().filesys.GetName(FileMap::MemoAttach, iter->File()),
					   iter->Sender(), iter->Text()));
	    else
		output =
		    parseMessage(Magick::instance().getMessage(dest, "MS_STATUS/FORWARD"),
				 mVarArray(iter->Sender(), iter->Text()));
	}

	do_Forward2(mynick, source, dest, output);
    }
    ETCB();
}

void MemoServ::do_Forward2(const mstring & mynick, const mstring & source, mstring dest, const mstring & text)
{
    BTCB();
    FT("MemoServ::do_Forward2", (mynick, source, dest, text));

    if (IsChan(dest))
    {
	if (!Magick::instance().chanserv.IsStored(dest))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (dest));
	    return;
	}
	dest = Magick::instance().getSname(dest);

	if (!Magick::instance().chanserv.GetStored(dest)->GetAccess(source, "WRITEMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }
    else
    {
	if (!Magick::instance().nickserv.IsStored(dest))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (dest));
	    return;
	}

	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(dest);
	dest = nstored->Name();
	if (nstored->Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (dest));
	    return;
	}

	if (nstored->IsIgnore(source) ? !nstored->NoMemo() : nstored->NoMemo())
	{
	    SEND(mynick, source, "MS_STATUS/IGNORE", (dest));
	    return;
	}
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    Magick::instance().memoserv.stats.i_Forward++;
    nlive->InFlight.Memo(false, mynick, dest, text, true);
    nlive->InFlight.End(0);
    ETCB();
}

void MemoServ::do_Reply(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Reply", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

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

    mstring output, recipiant;

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 4)
	{
	    SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.ExtractWord(3, " ");
	mstring text = params.After(" ", 3);

	mstring whoami = Magick::instance().nickserv.GetStored(source)->Host().LowerCase();

	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Magick::instance().chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (who));
	    return;
	}
	who = Magick::instance().getSname(who);

	if (!Magick::instance().chanserv.GetStored(who)->GetAccess(whoami, "READMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Magick::instance().memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (who.LowerCase()));
	    return;
	}

	if (!what.IsNumber() || what.Contains("."))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(what.c_str());

	if (num <= 0 || num > Magick::instance().memoserv.ChannelNewsSize(who))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().memoserv.ChannelNewsSize(who)));
	    return;
	}

	if (!nlive->HasMode("o") && nlive->LastMemo().SecondsSince() < Magick::instance().memoserv.Delay())
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTYET",
		 (message, ToHumanTime(Magick::instance().memoserv.Delay() - nlive->LastMemo().SecondsSince(), source)));
	    return;
	}

	unsigned int i;

	{
	    RLOCK((lck_MemoServ, lck_channel, who.LowerCase()));
	    MemoServ::channel_news_t::iterator iter = Magick::instance().memoserv.ChannelNewsBegin(who);
	    for (i = 1; i < num; iter++, i++);

	    recipiant = iter->Sender();
	    output =
		parseMessage(Magick::instance().getMessage("MS_STATUS/REPLY_ARG"),
			     mVarArray(who,
				       ((iter->Text().size() > 20) ? (iter->Text().SubString(0, 19) + "...") : iter->Text()),
				       text));
	}
    }
    else
    {
	mstring who = Magick::instance().nickserv.GetStored(source)->Host();
	mstring what = params.ExtractWord(2, " ");
	mstring text = params.After(" ", 2);

	if (who.empty())
	    who = source;

	if (!Magick::instance().memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}
	who = Magick::instance().getSname(who);

	if (!what.IsNumber() || what.Contains("."))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(what.c_str());

	if (num <= 0 || num > Magick::instance().memoserv.NickMemoSize(who))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, Magick::instance().memoserv.NickMemoSize(who)));
	    return;
	}

	if (!nlive->HasMode("o") && nlive->LastMemo().SecondsSince() < Magick::instance().memoserv.Delay())
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTYET",
		 (message, ToHumanTime(Magick::instance().memoserv.Delay() - nlive->LastMemo().SecondsSince(), source)));
	    return;
	}

	unsigned int i;

	{
	    RLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
	    MemoServ::nick_memo_t::iterator iter = Magick::instance().memoserv.NickMemoBegin(who);
	    for (i = 1; i < num; iter++, i++);
	    if (iter->File())
		output =
		    parseMessage(Magick::instance().getMessage("MS_STATUS/REPLY_ARG"),
				 mVarArray(Magick::instance().filesys.GetName(FileMap::MemoAttach, iter->File()),
					   ((iter->Text().length() >
					     20) ? (iter->Text().SubString(0, 19) + "...") : iter->Text()), text));
	    else
		output =
		    parseMessage(Magick::instance().getMessage("MS_STATUS/REPLY"),
				 mVarArray(((iter->Text().length() >
					     20) ? (iter->Text().SubString(0, 19) + "...") : iter->Text()), text));

	}
    }

    if (!Magick::instance().nickserv.IsStored(recipiant))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (Magick::instance().getSname(recipiant)));
	return;
    }
    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(recipiant);
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (recipiant));
	return;
    }
    if (nstored->IsIgnore(source) ? !nstored->NoMemo() : nstored->NoMemo())
    {
	SEND(mynick, source, "MS_STATUS/IGNORE", (recipiant));
	return;
    }
    Magick::instance().memoserv.stats.i_Reply++;
    nlive->InFlight.Memo(false, mynick, recipiant, output);

    ETCB();
}

void MemoServ::do_Cancel(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Cancel", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (nlive->InFlight.Memo())
    {
	Magick::instance().memoserv.stats.i_Cancel++;
	if (nlive->InFlight.File())
	    NSEND(mynick, source, "MS_COMMAND/CANCEL");

	nlive->InFlight.Cancel();
    }
    else
    {
	NSEND(mynick, source, "MS_STATUS/NOPENDING");
    }
    ETCB();
}

void MemoServ::do_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Del", (mynick, source, params));

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

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	    SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.After(" ", 2);
	mstring whoami = Magick::instance().nickserv.GetStored(source)->Host().LowerCase();

	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Magick::instance().chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (who));
	    return;
	}
	who = Magick::instance().getSname(who);

	map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(who);
	if (!cstored->GetAccess(whoami, "WRITEMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Magick::instance().memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (who.LowerCase()));
	    return;
	}

	Magick::instance().memoserv.stats.i_Del++;
	if (what.IsSameAs("all", true))
	{
	    if (!cstored->GetAccess(whoami, "DELMEMO"))
	    {
		NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
		return;
	    }

	    Magick::instance().memoserv.RemChannel(who);
	    SEND(mynick, source, "MS_COMMAND/CS_DEL_ALL", (who));
	    LOG(LM_DEBUG, "MEMOSERV/DEL_ALL", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), who));
	}
	else
	{
	    unsigned int i;

	    /* Adjust is to accomodate for memos that have
	     * been deleted before the current one, ie. if
	     * 1, 3, 5 and 7 are being deleted, once 1 has
	     * been deleted, adjust becomes 1, meaning that
	     * 3 has become 2, 5 has become 4, etc.  Once 3
	     * has been deleted, adjust becomes 2, meaning
	     * that 5 has become 3 and 7 has become 5.
	     */
	    int j = 1, adjust = 0;
	    vector < int > numbers1 = ParseNumbers(what);
	    set < int > numbers;
	    set < int >::iterator ni;

	    for (i = 0; i < numbers1.size(); i++)
		numbers.insert(numbers1[i]);

	    bool triedabove = false, nonnumeric = false;
	    mstring output, denied;
	    size_t amt = 0;

	    {
		WLOCK((lck_MemoServ, lck_channel, who.LowerCase()));
		MemoServ::channel_news_t & newslist = Magick::instance().memoserv.GetChannel(who);
		amt = newslist.size();
		MemoServ::channel_news_t::iterator iter = newslist.begin();
		for (ni = numbers.begin(); ni != numbers.end(); ni++)
		{
		    if (*ni - adjust <= 0)
			nonnumeric = true;
		    else if (*ni - adjust > static_cast < int > (newslist.size()))
			triedabove = true;
		    else
		    {
			while (*ni - adjust > j && iter != newslist.end())
			{
			    j++;
			    iter++;
			}
			if (iter != newslist.end())
			{
			    if (!iter->Sender().IsSameAs(who, true) && !cstored->GetAccess(whoami, "DELMEMO"))
			    {
				if (!denied.empty())
				    denied << ", ";
				denied << * ni;
			    }
			    else
			    {
				if (!output.empty())
				    output << ", ";
				output << * ni;
				Magick::instance().memoserv.RemChannelNews(who, j - 1);
				if (adjust + 2 == static_cast < int > (amt))
				{
				    if (numbers.size() > amt)
					triedabove = true;
				    break;
				}
				iter = newslist.begin();
				j = 1;
				adjust++;
			    }
			}
		    }
		}
	    }
	    if (!denied.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/CS_NOTDEL", (denied, who));
		denied.erase();
	    }
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/CS_DEL", (output, who));
		output.erase();
		LOG(LM_DEBUG, "MEMOSERV/DEL",
		    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), adjust, who));
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", (amt));
	}
    }
    else
    {
	mstring who = Magick::instance().nickserv.GetStored(source)->Host();
	mstring what = params.After(" ", 1);

	if (who.empty())
	    who = source;

	if (!Magick::instance().memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	Magick::instance().memoserv.stats.i_Del++;
	if (what.IsSameAs("all", true))
	{
	    MemoServ::nick_memo_t::iterator iter;
	    {
		RLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
		for (iter = Magick::instance().memoserv.NickMemoBegin(who);
		     iter != Magick::instance().memoserv.NickMemoEnd(who); iter++)
		{
		    if (iter->File())
			Magick::instance().filesys.EraseFile(FileMap::MemoAttach, iter->File());
		}
	    }
	    Magick::instance().memoserv.RemNick(who);
	    NSEND(mynick, source, "MS_COMMAND/NS_DEL_ALL");
	}
	else
	{
	    unsigned int i;

	    /* Adjust is to accomodate for memos that have
	     * been deleted before the current one, ie. if
	     * 1, 3, 5 and 7 are being deleted, once 1 has
	     * been deleted, adjust becomes 1, meaning that
	     * 3 has become 2, 5 has become 4, etc.  Once 3
	     * has been deleted, adjust becomes 2, meaning
	     * that 5 has become 3 and 7 has become 5.
	     */
	    int j = 1, adjust = 0;
	    vector < int > numbers1 = ParseNumbers(what);
	    set < int > numbers;
	    set < int >::iterator ni;

	    for (i = 0; i < numbers1.size(); i++)
		numbers.insert(numbers1[i]);
	    bool triedabove = false, nonnumeric = false;
	    mstring output;
	    size_t amt = 0;

	    {
		WLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
		MemoServ::nick_memo_t & memolist = Magick::instance().memoserv.GetNick(who);
		amt = memolist.size();
		MemoServ::nick_memo_t::iterator iter = memolist.begin();
		for (ni = numbers.begin(); ni != numbers.end(); ni++)
		{
		    if (*ni - adjust <= 0)
			nonnumeric = true;
		    else if (*ni - adjust > static_cast < int > (memolist.size()))
			triedabove = true;
		    else
		    {
			while (*ni - adjust > j && iter != memolist.end())
			{
			    j++;
			    iter++;
			}
			if (iter != memolist.end())
			{
			    if (!output.empty())
				output << ", ";
			    output << * ni;
			    if (iter->File())
				Magick::instance().filesys.EraseFile(FileMap::MemoAttach, iter->File());
			    Magick::instance().memoserv.RemNickMemo(who, j - 1);
			    if (adjust + 2 == static_cast < int > (amt))
			    {
				if (numbers.size() > amt)
				    triedabove = true;
				break;
			    }
			    iter = memolist.begin();
			    j = 1;
			    adjust++;
			}
		    }
		}
	    }
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/NS_DEL", (output));
		output.erase();
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", (amt));
	}
    }
    ETCB();
}

void MemoServ::do_Continue(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Continue", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring text = params.After(" ", 1);

    if (text.size() > Magick::instance().server.proto.MaxLine())
    {
	text.Truncate(Magick::instance().server.proto.MaxLine());
	SEND(mynick, source, "MS_STATUS/TRUNCATE",
	     (text.SubString(Magick::instance().server.proto.MaxLine() - 20, Magick::instance().server.proto.MaxLine() - 1),
	      mynick));
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (nlive->InFlight.Memo())
    {
	Magick::instance().memoserv.stats.i_Continue++;
	nlive->InFlight.Continue(text);
    }
    else
    {
	NSEND(mynick, source, "MS_STATUS/NOPENDING");
    }
    ETCB();
}

void MemoServ::do_Preview(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_Continue", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (nlive->InFlight.Memo())
    {
	mstring text(nlive->InFlight.Text());
	size_t shown = 0, length = text.length();

	SEND(mynick, source, "MS_COMMAND/PREVIEW", (nlive->InFlight.Recipiant()));
	while (shown < length)
	{
	    if (length - shown > Magick::instance().server.proto.MaxLine())
	    {
		::send(mynick, source,
		       (shown ? "    ..." : "    ") + text.SubString(shown,
								     Magick::instance().server.proto.MaxLine() - 1) + "...");
		shown += Magick::instance().server.proto.MaxLine();
	    }
	    else
	    {
		::send(mynick, source, (shown ? "    ..." : "    ") + text.SubString(shown, length - 1));
		shown += length - shown;
	    }
	}
    }
    else
    {
	NSEND(mynick, source, "MS_STATUS/NOPENDING");
    }
    ETCB();
}

void MemoServ::do_File(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_File", (mynick, source, params));

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

    mstring name = params.ExtractWord(2, " ");
    mstring text;

    if (params.WordCount(" ") > 2)
	text = params.After(" ", 2);

    if (IsChan(name))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOATTACH");
	return;
    }
    else
    {
	if (!Magick::instance().nickserv.IsStored(name))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (name));
	    return;
	}
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(name);
	name = nstored->Name();
	if (nstored->Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (name));
	    return;
	}
	mstring target = nstored->Host();

	if (target.empty())
	    target = name;

	if (Magick::instance().memoserv.Files() > 0)
	{
	    if (Magick::instance().memoserv.IsNick(target))
	    {
		unsigned int count = 0;

		MemoServ::nick_memo_t::iterator iter;
		{
		    RLOCK((lck_MemoServ, lck_nick, target.LowerCase()));
		    for (iter = Magick::instance().memoserv.NickMemoBegin(target);
			 iter != Magick::instance().memoserv.NickMemoEnd(target); iter++)
		    {
			if (iter->File())
			    count++;
		    }
		}
		if (count >= Magick::instance().memoserv.Files())
		{
		    SEND(mynick, source, "MS_STATUS/ATTACHLIMIT", (name));
		    return;
		}
	    }
	}
	else
	{
	    NSEND(mynick, source, "MS_STATUS/FILEDISABLED");
	    return;
	}

	if (nstored->IsIgnore(source) ? !nstored->NoMemo() : nstored->NoMemo())
	{
	    SEND(mynick, source, "MS_STATUS/IGNORE", (name));
	    return;
	}
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->HasMode("o") && nlive->LastMemo().SecondsSince() < Magick::instance().memoserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET",
	     (message, ToHumanTime(Magick::instance().memoserv.Delay() - nlive->LastMemo().SecondsSince(), source)));
	return;
    }

    if (text.size() > Magick::instance().server.proto.MaxLine())
    {
	text.Truncate(Magick::instance().server.proto.MaxLine());
	SEND(mynick, source, "MS_STATUS/TRUNCATE",
	     (text.SubString(Magick::instance().server.proto.MaxLine() - 20, Magick::instance().server.proto.MaxLine() - 1),
	      mynick));
    }

    Magick::instance().memoserv.stats.i_File++;
    nlive->InFlight.Memo(true, mynick, name, text);
    ETCB();
}

void MemoServ::do_set_NoExpire(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("MemoServ::do_set_NoExpire", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

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

    if (IsChan(params.ExtractWord(2, " ")))
    {
	message += " " + params.ExtractWord(3, " ").UpperCase();
	if (params.WordCount(" ") < 5)
	{
	    SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring onoff = params.ExtractWord(4, " ");
	mstring what = params.After(" ", 4);
	mstring whoami = Magick::instance().nickserv.GetStored(source)->Host().LowerCase();

	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Magick::instance().chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (who));
	    return;
	}
	who = Magick::instance().getSname(who);

	if (!Magick::instance().chanserv.GetStored(who)->GetAccess(whoami, "WRITEMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Magick::instance().memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (who.LowerCase()));
	    return;
	}

	if (!onoff.IsBool())
	{
	    NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	    return;
	}

	if (!Magick::instance().chanserv.GetStored(who)->GetAccess(whoami, "DELMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	Magick::instance().memoserv.stats.i_Set++;
	if (what.IsSameAs("all", true))
	{
	    MemoServ::channel_news_t::iterator iter;
	    mstring output;

	    {
		WLOCK((lck_MemoServ, lck_channel, who.LowerCase()));
		for (iter = Magick::instance().memoserv.ChannelNewsBegin(who);
		     iter != Magick::instance().memoserv.ChannelNewsEnd(who); iter++)
		{
		    iter->NoExpire(onoff.GetBool());
		}
	    }
	    SEND(mynick, source, "MS_COMMAND/CS_SET_ALL",
		 (Magick::instance().getMessage(source, "MS_STATUS/SET_NOEXPIRE"), who,
		  (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));

	    LOG(LM_DEBUG, "MEMOSERV/SET_ALL",
		(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		 Magick::instance().getMessage(source, "MS_STATUS/SET_NOEXPIRE"), who,
		 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
	}
	else
	{
	    unsigned int i;
	    int j = 1, count = 0;
	    vector < int > numbers1 = ParseNumbers(what);
	    set < int > numbers;
	    set < int >::iterator ni;

	    for (i = 0; i < numbers1.size(); i++)
		numbers.insert(numbers1[i]);

	    bool triedabove = false, nonnumeric = false;
	    mstring output, denied;
	    size_t amt = 0;

	    {
		WLOCK((lck_MemoServ, lck_channel, who.LowerCase()));
		MemoServ::channel_news_t & newslist = Magick::instance().memoserv.GetChannel(who);
		amt = newslist.size();
		MemoServ::channel_news_t::iterator iter = newslist.begin();
		for (ni = numbers.begin(); ni != numbers.end(); ni++)
		{
		    if (*ni <= 0)
			nonnumeric = true;
		    else if (*ni > static_cast < int > (newslist.size()))
			triedabove = true;
		    else
		    {
			while (*ni > j && iter != newslist.end())
			{
			    j++;
			    iter++;
			}
			if (iter != newslist.end())
			{
			    if (!output.empty())
				output << ", ";
			    output << j;
			    iter->NoExpire(onoff.GetBool());
			    count++;
			}
		    }
		}
	    }
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/CS_SET",
		     (Magick::instance().getMessage(source, "MS_STATUS/SET_NOEXPIRE"), output, who,
		      (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
					 Magick::instance().getMessage("VALS/OFF"))));
		output.erase();
		LOG(LM_DEBUG, "MEMOSERV/SET",
		    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
		     Magick::instance().getMessage(source, "MS_STATUS/SET_NOEXPIRE"), count, who,
		     (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") :
					Magick::instance().getMessage("VALS/OFF"))));
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", (amt));
	}
    }
    else
    {
	message += " " + params.ExtractWord(2, " ").UpperCase();
	SEND(mynick, source, "ERR_SITUATION/NOTONANICK", (message));

/* No NoExpire setting for nicknames.

	mstring who = Magick::instance().nickserv.GetStored(name)->Host();
	mstring onoff = params.ExtractWord(3, " ");
	mstring what = params.After(" ", 3);

	if (who.empty())
	    who = source;

	if (!Magick::instance().memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	if (!onoff.IsBool())
	{
	    NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	    return;
	}

	Magick::instance().memoserv.stats.i_Set++;
	if (what.IsSameAs("all", true))
	{
	    MemoServ::nick_memo_t::iterator iter;
	    mstring output;
	    { WLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
	    for (iter = Magick::instance().memoserv.NickMemoBegin(who);
		    iter != Magick::instance().memoserv.NickMemoEnd(who); iter++)
	    {
		iter->NoExpire(onoff.GetBool());
	    }}
	    SEND(mynick, source, "MS_COMMAND/NS_SET_ALL", (
		Magick::instance().getMessage(source, "MS_STATUS/SET_NOEXPIRE"),
		(onoff.GetBool() ?
			Magick::instance().getMessage("VALS/ON") :
			Magick::instance().getMessage("VALS/OFF"))));
	}
	else
	{
	    unsigned int i;
	    int j=1, count = 0;
	    vector<int> numbers1 = ParseNumbers(what);
	    set<int> numbers;
	    set<int>::iterator ni;
	    for (i=0; i<numbers1.size(); i++)
		numbers.insert(numbers1[i]);
	    bool triedabove = false, nonnumeric = false;
	    mstring output;
	    size_t amt = 0;
	    { WLOCK((lck_MemoServ, lck_nick, who.LowerCase()));
	    MemoServ::nick_memo_t &memolist = Magick::instance().memoserv.GetNick(who);
	    amt = memolist.size();
	    nick_memo_t::iterator iter = memolist.begin();
	    for (ni = numbers.begin(); ni != numbers.end(); ni++)
	    {
		if (*ni <= 0)
		    nonnumeric = true;
		else if (*ni > static_cast<int>(memolist.size()))
		    triedabove = true;
		else
		{
		    while (*ni > j &&
			iter != memolist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != memolist.end())
		    {
			if (!output.empty())
			    output << ", ";
			output << j;
			iter->NoExpire(onoff.GetBool());
			count++;
		    }
		}
	    }}
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/NS_SET", (
			Magick::instance().getMessage(source, "MS_STATUS/SET_NOEXPIRE"),
			output, (onoff.GetBool() ?
				Magick::instance().getMessage("VALS/ON") :
				Magick::instance().getMessage("VALS/OFF"))));
		output.erase();
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", ( amt));
	}
*/
    }
    ETCB();
}

SXP::Tag Memo_t::tag_Memo_t("Memo_t");
SXP::Tag Memo_t::tag_Nick("Nick");
SXP::Tag Memo_t::tag_Sender("Sender");
SXP::Tag Memo_t::tag_Time("Time");
SXP::Tag Memo_t::tag_Text("Text");
SXP::Tag Memo_t::tag_Read("Read");
SXP::Tag Memo_t::tag_File("File");
SXP::Tag Memo_t::tag_UserDef("UserDef");
SXP::Tag News_t::tag_News_t("News_t");
SXP::Tag News_t::tag_Channel("Channel");
SXP::Tag News_t::tag_Sender("Sender");
SXP::Tag News_t::tag_Time("Time");
SXP::Tag News_t::tag_Text("Text");
SXP::Tag News_t::tag_set_NoExpire("SET_NoExpire");
SXP::Tag News_t::tag_Read("Read");
SXP::Tag News_t::tag_UserDef("UserDef");

void Memo_t::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

    FT("Memo_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if (pElement->IsA(tag_UserDef))
    {
	mstring *tmp = new mstring;

	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
    ETCB();
}

void Memo_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

    FT("Memo_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
    if (pElement->IsA(tag_Nick))
	pElement->Retrieve(i_Nick);
    if (pElement->IsA(tag_Sender))
	pElement->Retrieve(i_Sender);
    if (pElement->IsA(tag_Time))
	pElement->Retrieve(i_Time);
    if (pElement->IsA(tag_Text))
	pElement->Retrieve(i_Text);
    if (pElement->IsA(tag_Read))
	pElement->Retrieve(i_Read);
    if (pElement->IsA(tag_File))
	pElement->Retrieve(i_File);
    ETCB();
}

void Memo_t::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    FT("Memo_t::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    //TODO: Add your source code here
    pOut->BeginObject(tag_Memo_t);

    WLOCK((lck_MemoServ, lck_nick, i_Nick.LowerCase()));
    pOut->WriteElement(tag_Nick, i_Nick);
    pOut->WriteElement(tag_Sender, i_Sender);
    pOut->WriteElement(tag_Time, i_Time);
    pOut->WriteElement(tag_Text, i_Text);
    pOut->WriteElement(tag_Read, i_Read);
    pOut->WriteElement(tag_File, i_File);

    map < mstring, mstring >::const_iterator iter;
    for (iter = i_UserDef.begin(); iter != i_UserDef.end(); iter++)
    {
	pOut->WriteElement(tag_UserDef, iter->first + "\n" + iter->second);
    }
    pOut->EndObject(tag_Memo_t);
    ETCB();
}

void News_t::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

    FT("News_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if (pElement->IsA(tag_UserDef))
    {
	mstring *tmp = new mstring;

	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
    ETCB();
}

void News_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

    FT("News_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
    if (pElement->IsA(tag_Channel))
	pElement->Retrieve(i_Channel);
    if (pElement->IsA(tag_Sender))
	pElement->Retrieve(i_Sender);
    if (pElement->IsA(tag_Time))
	pElement->Retrieve(i_Time);
    if (pElement->IsA(tag_Text))
	pElement->Retrieve(i_Text);
    if (pElement->IsA(tag_set_NoExpire))
	pElement->Retrieve(i_NoExpire);

    if (pElement->IsA(tag_Read))
    {
	mstring tmp;

	pElement->Retrieve(tmp);
	i_Read.insert(tmp);
    }
    ETCB();
}

void News_t::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    FT("News_t::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    //TODO: Add your source code here
    pOut->BeginObject(tag_News_t);

    WLOCK((lck_MemoServ, lck_channel, i_Channel.LowerCase()));
    pOut->WriteElement(tag_Channel, i_Channel);
    pOut->WriteElement(tag_Sender, i_Sender);
    pOut->WriteElement(tag_Time, i_Time);
    pOut->WriteElement(tag_Text, i_Text);
    pOut->WriteElement(tag_set_NoExpire, i_NoExpire);

    set < mstring >::iterator iter2;
    for (iter2 = i_Read.begin(); iter2 != i_Read.end(); iter2++)
    {
	pOut->WriteElement(tag_Read, *iter2);
    }

    map < mstring, mstring >::const_iterator iter;
    for (iter = i_UserDef.begin(); iter != i_UserDef.end(); iter++)
    {
	pOut->WriteElement(tag_UserDef, iter->first + "\n" + iter->second);
    }
    pOut->EndObject(tag_News_t);
    ETCB();
}

SXP::Tag MemoServ::tag_MemoServ("MemoServ");

void MemoServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    FT("MemoServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Memo_t *m = new Memo_t;

    if (pElement->IsA(m->GetClassTag()))
    {
	m_array.push_back(m);
	pIn->ReadTo(m);
    }
    else
    {
	delete m;
    }

    News_t *n = new News_t;

    if (pElement->IsA(n->GetClassTag()))
    {
	n_array.push_back(n);
	pIn->ReadTo(n);
    }
    else
    {
	delete n;
    }
    ETCB();
}

void MemoServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);
    static_cast < void > (pElement);

    FT("MemoServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
    ETCB();
}

void MemoServ::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    FT("MemoServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_MemoServ);

    MemoServ::nick_t::iterator i1;
    MemoServ::nick_memo_t::iterator i2;
    MemoServ::channel_t::iterator j1;
    MemoServ::channel_news_t::iterator j2;

    {
	RLOCK((lck_MemoServ, lck_nick));
	for (i1 = NickBegin(); i1 != NickEnd(); i1++)
	{
	    RLOCK2((lck_MemoServ, lck_nick, i1->first));
	    for (i2 = i1->second.begin(); i2 != i1->second.end(); i2++)
	    {
		pOut->WriteSubElement(&(*i2));
	    }
	}
    }

    {
	RLOCK((lck_MemoServ, lck_channel));
	for (j1 = ChannelBegin(); j1 != ChannelEnd(); j1++)
	{
	    RLOCK2((lck_MemoServ, lck_channel, j1->first));
	    for (j2 = j1->second.begin(); j2 != j1->second.end(); j2++)
	    {
		pOut->WriteSubElement(&(*j2));
	    }
	}
    }

    pOut->EndObject(tag_MemoServ);
    ETCB();
}

void MemoServ::PostLoad()
{
    BTCB();
    NFT("MemoServ::PostLoad");
    // Linkage, etc

    unsigned int i, j;

    for (i = 0; i < m_array.size(); i++)
    {
	if (m_array[i] != NULL)
	{
	    for (j = 0; j < m_array[i]->ud_array.size(); j++)
	    {
		if (m_array[i]->ud_array[j] != NULL)
		{
		    if (m_array[i]->ud_array[j]->Contains("\n"))
			m_array[i]->i_UserDef[m_array[i]->ud_array[j]->Before("\n")] = m_array[i]->ud_array[j]->After("\n");
		    delete m_array[i]->ud_array[j];
		}
	    }
	    m_array[i]->ud_array.clear();
	    if (!m_array[i]->Nick().empty())
		AddNickMemo(m_array[i]);
	    delete m_array[i];
	}
    }
    m_array.clear();

    for (i = 0; i < n_array.size(); i++)
    {
	if (n_array[i] != NULL)
	{
	    for (j = 0; j < n_array[i]->ud_array.size(); j++)
	    {
		if (n_array[i]->ud_array[j] != NULL)
		{
		    if (n_array[i]->ud_array[j]->Contains("\n"))
			n_array[i]->i_UserDef[n_array[i]->ud_array[j]->Before("\n")] = n_array[i]->ud_array[j]->After("\n");
		    delete n_array[i]->ud_array[j];
		}
	    }
	    n_array[i]->ud_array.clear();
	    if (!n_array[i]->Channel().empty())
		AddChannelNews(n_array[i]);
	    delete n_array[i];
	}
    }
    n_array.clear();

    ETCB();
}
