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
#define RCSID(x,y) const char *rcsid_base_cpp_ ## x () { return y; }
RCSID(base_cpp, "@(#)$Id$");

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

bool mBase::TaskOpened;
map < mMessage::type_t, map < mstring, set < unsigned long > > > mMessage::AllDependancies;
map < unsigned long, mMessage * > mMessage::MsgIdMap;
unsigned long mMessage::LastMsgId = 0;

entlist_t &entlist_t::operator=(const entlist_t & in)
{
    BTCB();
    FT("entlist_t::operator=", ("(const entlist_t &) in"));
    i_Entry = in.i_Entry;
    i_Last_Modify_Time = in.i_Last_Modify_Time;
    i_Last_Modifier = in.i_Last_Modifier;
    map < mstring, mstring >::const_iterator i;
    i_UserDef.clear();
    for (i = in.i_UserDef.begin(); i != in.i_UserDef.end(); i++)
	i_UserDef[i->first] = i->second;
    NRET(entlist_t &, *this);
    ETCB();
}

SXP::Tag tag_entlist_t("entlist_t");
SXP::Tag tag_entlist_val_t("entlist_val_t");
SXP::Tag tag_Entry("Entry");
SXP::Tag tag_Value("Value");
SXP::Tag tag_ValueFirst("ValueFirst");
SXP::Tag tag_ValueSecond("ValueSecond");
SXP::Tag tag_Last_Modify_Time("Last_Modify_Time");
SXP::Tag tag_Last_Modifier("Last_Modifier");
SXP::Tag tag_UserDef("UserDef");
SXP::Tag tag_Stupid("Stupid");

void entlist_t::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

    FT("entlist_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if (pElement->IsA(tag_UserDef))
    {
	mstring *tmp = new mstring;

	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
    ETCB();
}

void entlist_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

    FT("entlist_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    if (pElement->IsA(tag_Entry))
	pElement->Retrieve(i_Entry);
    if (pElement->IsA(tag_Last_Modify_Time))
	pElement->Retrieve(i_Last_Modify_Time);
    if (pElement->IsA(tag_Last_Modifier))
	pElement->Retrieve(i_Last_Modifier);
    ETCB();
}

void entlist_t::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    FT("entlist_t::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    pOut->BeginObject(tag_entlist_t);

    pOut->WriteElement(tag_Entry, i_Entry);
    pOut->WriteElement(tag_Last_Modify_Time, i_Last_Modify_Time);
    pOut->WriteElement(tag_Last_Modifier, i_Last_Modifier);

    map < mstring, mstring >::const_iterator iter;
    for (iter = i_UserDef.begin(); iter != i_UserDef.end(); iter++)
    {
	pOut->WriteElement(tag_UserDef, iter->first + "\n" + iter->second);
    }

    pOut->EndObject(tag_entlist_t);
    ETCB();
}

void entlist_t::PostLoad() const
{
    BTCB();
    NFT("entlist_t::PostLoad");

    unsigned int j;

    for (j = 0; j < ud_array.size(); j++)
    {
	if (ud_array[j] != NULL)
	{
	    if (ud_array[j]->Contains("\n"))
		i_UserDef[ud_array[j]->Before("\n")] = ud_array[j]->After("\n");
	    delete ud_array[j];
	}
    }
    ud_array.clear();
    ETCB();
}

size_t entlist_t::Usage() const
{
    BTCB();
    size_t retval = 0;

    retval += i_Entry.capacity();
    retval += i_Last_Modifier.capacity();
    retval += sizeof(i_Last_Modify_Time.Internal());
    map < mstring, mstring >::const_iterator i;
    for (i = i_UserDef.begin(); i != i_UserDef.end(); i++)
    {
	retval += i->first.capacity();
	retval += i->second.capacity();
    }
    return retval;
    ETCB();
}

void entlist_t::DumpB() const
{
    MB(0, (i_Entry, i_Last_Modifier, i_Last_Modify_Time, i_UserDef.size()));
}

void entlist_t::DumpE() const
{
    ME(0, (i_Entry, i_Last_Modifier, i_Last_Modify_Time, i_UserDef.size()));
}

// --------- end of entlist_t -----------------------------------

mMessage::mMessage(const mstring & p_source, const mstring & p_msgtype, const mstring & p_params,
		   const unsigned long p_priority) : ACE_Method_Request(p_priority), msgid_(0), sourceToken_(false),
source_(p_source), params_(p_params), creation_(mDateTime::CurrentDateTime())
{
    BTCB();
    FT("mMessage::mMessage", (p_source, p_msgtype, p_params, p_priority));

    if (source_ != " " && Magick::instance().server.proto.Tokens())
    {
	mstring tmp(Magick::instance().server.proto.GetToken(p_msgtype));

	if (!tmp.empty())
	    msgtype_ = tmp;
    }

    if (msgtype_.empty())
	msgtype_ = p_msgtype;

    msgtype_.MakeUpper();
    ETCB();
}

void mMessage::AddDependancies()
{
    BTCB();
    NFT("mMessage::AddDependancies");
    WLOCK(("Dependancies", this));
    int added = 0;

    if (!source_.empty())
    {
	if (source_[0u] == '@')
	{
	    AddDepend(ServerExists, source_);
	}
	else if (source_.Contains("."))
	{
	    AddDepend(ServerExists, source_.LowerCase());
	}
	else if (source_[0u] == '!')
	{
	    AddDepend(NickExists, source_);
	}
	else
	{
	    AddDepend(NickExists, source_.LowerCase());
	}
    }

    if (msgtype_ == "BURST")
    {
	if (params_.WordCount(" :") > 2)
	{
	    unsigned int i, offset = 3;
	    mstring modes = params_.ExtractWord(offset, " ");

	    if (modes[0u] != '+')
		modes.erase();
	    if (modes.length())
	    {
		offset++;
		for (i = 0; i < modes.length(); i++)
		    if (Magick::instance().server.proto.ChanModeArg().Contains(modes[i]))
			offset++;
	    }

	    for (i = offset; i <= params_.WordCount(" "); i++)
	    {
		mstring nick(params_.ExtractWord(i, " ").Before(":"));

		AddDepend(NickExists, "!" + nick);
	    }
	}
    }
    else if (msgtype_ == "JOIN")
    {
	// User is NOT in channel
	for (unsigned int i = 1; i <= params_.WordCount(":, "); i++)
	{
	    mstring chan = params_.ExtractWord(i, ":, ");

	    if (IsChan(chan))
		AddDepend(UserNoInChan, chan.LowerCase() + ":" + source_);
	    else
		AddDepend(UserNoInChan, chan + ":" + source_);
	}
    }
    else if (msgtype_ == "KICK")
    {
	// Channel exists
	// Target exists
	// Target in channel
	AddDepend(ChanExists, params_.ExtractWord(1, ": ").LowerCase());
	AddDepend(NickExists, params_.ExtractWord(2, ": ").LowerCase());
	AddDepend(UserInChan, params_.ExtractWord(1, ": ").LowerCase() + ":" + params_.ExtractWord(2, ": ").LowerCase());
    }
    else if (msgtype_ == "KILL")
    {
	// Target exists
	AddDepend(NickExists, params_.ExtractWord(1, ": ").LowerCase());
    }
    else if ((msgtype_ == "MODE" || msgtype_ == "SVSMODE") && IsChan(params_.ExtractWord(1, ": ")))
    {
	// Channel exists
	// ALL params_ that are users exist ... ?
	AddDepend(ChanExists, params_.ExtractWord(1, ": ").LowerCase());

	mstring mode = params_.ExtractWord(2, ": ");
	unsigned int fwdarg = 3;

	for (unsigned int i = 0; i < mode.length() && fwdarg <= params_.WordCount(": "); i++)
	{
	    if (Magick::instance().server.proto.ChanModeArg().Contains(mode[i]))
	    {
		switch (mode[i])
		{
		case 'o':
		case 'h':
		case 'v':
		    if (Magick::instance().server.proto.Numeric.User())
			AddDepend(NickExists, "!" + params_.ExtractWord(fwdarg, ": "));
		    else
			AddDepend(NickExists, params_.ExtractWord(fwdarg, ": "));
		    break;
		}
		fwdarg++;
	    }
	}
    }
    else if (msgtype_ == "NICK" || msgtype_ == "SNICK" || msgtype_ == "USER")
    {
	// Server exists OR
	// Target nick does NOT exist
	AddDepend(NickNoExists, params_.ExtractWord(1, ": ").LowerCase());
	if (source_.empty() || source_[0u] == '@')
	{
	    mstring server;

	    switch (Magick::instance().server.proto.Signon())
	    {
	    case 0000:
		if (Magick::instance().server.proto.Numeric.Server())
		    AddDepend(ServerExists, "@" + params_.ExtractWord(4, ": "));
		else
		    AddDepend(ServerExists, params_.ExtractWord(4, ": ").LowerCase());
		break;
	    case 0001:
		if (Magick::instance().server.proto.Numeric.Server())
		    AddDepend(ServerExists, "@" + params_.ExtractWord(5, ": "));
		else
		    AddDepend(ServerExists, params_.ExtractWord(5, ": ").LowerCase());
		break;
	    case 1000:
	    case 1001:
	    case 1002:
		if (Magick::instance().server.proto.Numeric.Server())
		    AddDepend(ServerExists, "@" + params_.ExtractWord(6, ": "));
		else
		    AddDepend(ServerExists, params_.ExtractWord(6, ": ").LowerCase());
		break;
	    case 1003:
	    case 2000:
	    case 2001:
		if (Magick::instance().server.proto.Numeric.Server())
		    AddDepend(ServerExists, "@" + params_.ExtractWord(7, ": "));
		else
		    AddDepend(ServerExists, params_.ExtractWord(7, ": ").LowerCase());
		break;
	    case 2002:
		AddDepend(ServerExists, params_.ExtractWord(8, ": ").LowerCase());
		break;
	    case 2003:
		if (Magick::instance().server.proto.Numeric.Server())
		    AddDepend(ServerExists, "@" + params_.ExtractWord(6, ": "));
		else
		    AddDepend(ServerExists, params_.ExtractWord(6, ": ").LowerCase());
		break;
	    case 3000:
		// Source is already added ...
		break;
	    }
	}
    }
    else if (msgtype_ == "PART")
    {
	// Channel exists
	// User in channel
	mstring chan = params_.ExtractWord(1, ": ");

	if (IsChan(chan))
	{
	    AddDepend(ChanExists, chan.LowerCase());
	    AddDepend(UserInChan, chan.LowerCase() + ":" + source_);
	}
	else
	{
	    AddDepend(ChanExists, chan);
	    AddDepend(UserInChan, chan + ":" + source_);
	}
    }
    else if (msgtype_ == "SERVER")
    {
	// Uplink exists
	AddDepend(ServerNoExists, params_.ExtractWord(1, ": ").LowerCase());
    }
    else if (msgtype_ == "SJOIN")
    {
	// ALL nick's mentioned
	if (source_[0u] == '@' || source_.Contains(".") || source_.empty())
	{
	    for (unsigned int i = 1; i <= params_.After(":").WordCount(" "); i++)
	    {
		mstring nick(params_.After(":").ExtractWord(i, " ").LowerCase());

		if (!nick.empty())
		{
		    char c = 0;
		    bool IsNotNick = false;

		    while (!IsNotNick && c != nick[0u])
		    {
			c = nick[0u];
			switch (nick[0u])
			{
			case '@':
			case '%':
			case '+':
			case '*':
			case '~':
			    nick.erase(0, 0);
			    break;
			case '&':
			case '"':
			    IsNotNick = true;
			    break;
			}
		    }
		    if (!IsNotNick)
		    {
			AddDepend(NickExists, nick);
			AddDepend(UserNoInChan, params_.ExtractWord(2, ": ").LowerCase() + ":" + nick);
		    }
		}
	    }
	}
	else
	{
	    for (unsigned int i = 2; i <= params_.WordCount(": "); i++)
	    {
		char c = 0;
		mstring chan = params_.ExtractWord(i, ": ").LowerCase();

		while (c != chan[0u])
		{
		    c = chan[0u];
		    switch (chan[0u])
		    {
		    case '@':
		    case '%':
		    case '+':
		    case '*':
		    case '~':
			chan.erase(0, 0);
			break;
		    }
		}
		AddDepend(UserNoInChan, chan + ":" + source_.After(":"));
	    }
	}
    }
    else if (msgtype_ == "SQUIT")
    {
	// Uplink exists
	mstring server = params_.ExtractWord(1, ": ");

	if (server.Contains("."))
	    AddDepend(ServerExists, server.LowerCase());
	else
	    AddDepend(ServerExists, "@" + server);
    }
    else if (msgtype_ == "TOPIC")
    {
	// Channel exists
	mstring chan = params_.ExtractWord(1, ": ");

	if (IsChan(chan))
	    AddDepend(ChanExists, chan.LowerCase());
	else
	    AddDepend(ChanExists, chan);
    }

    {
	MLOCK(("MsgIdMap"));
	msgid_ = LastMsgId++;
	while (MsgIdMap.find(msgid_) != MsgIdMap.end())
	    msgid_ = LastMsgId++;
    }

    list < triplet < type_t, mstring, bool > >::iterator iter;

    for (iter = dependancies.begin(); iter != dependancies.end(); iter++)
    {
	int oldadded = added;

	switch (iter->first)
	{
	case ServerExists:
	    if (Magick::instance().server.GetServer(iter->second).empty())
	    {
		added++;
		MLOCK2(("AllDependancies"));
		if (added == 1)
		{
		    MLOCK(("MsgIdMap"));
		    MsgIdMap[msgid_] = this;
		}
		AllDependancies[iter->first] [iter->second].insert(msgid_);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case ServerNoExists:
	    if (!Magick::instance().server.GetServer(iter->second).empty())
	    {
		added++;
		MLOCK2(("AllDependancies"));
		if (added == 1)
		{
		    MLOCK(("MsgIdMap"));
		    MsgIdMap[msgid_] = this;
		}
		AllDependancies[iter->first] [iter->second].insert(msgid_);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case NickExists:
	    if (Magick::instance().server.GetUser(iter->second).empty())
	    {
		added++;
		MLOCK2(("AllDependancies"));
		if (added == 1)
		{
		    MLOCK(("MsgIdMap"));
		    MsgIdMap[msgid_] = this;
		}
		AllDependancies[iter->first] [iter->second].insert(msgid_);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case NickNoExists:
	    if (!Magick::instance().server.GetUser(iter->second).empty())
	    {
		added++;
		MLOCK2(("AllDependancies"));
		if (added == 1)
		{
		    MLOCK(("MsgIdMap"));
		    MsgIdMap[msgid_] = this;
		}
		AllDependancies[iter->first] [iter->second].insert(msgid_);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case ChanExists:
	    if (Magick::instance().server.GetChannel(iter->second).empty())
	    {
		added++;
		MLOCK2(("AllDependancies"));
		if (added == 1)
		{
		    MLOCK(("MsgIdMap"));
		    MsgIdMap[msgid_] = this;
		}
		AllDependancies[iter->first] [iter->second].insert(msgid_);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case ChanNoExists:
	    if (!Magick::instance().server.GetChannel(iter->second).empty())
	    {
		added++;
		MLOCK2(("AllDependancies"));
		if (added == 1)
		{
		    MLOCK(("MsgIdMap"));
		    MsgIdMap[msgid_] = this;
		}
		AllDependancies[iter->first] [iter->second].insert(msgid_);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case UserInChan:
	    if (Magick::instance().server.GetChannel(iter->second.Before(":")).empty() ||
		Magick::instance().server.GetUser(iter->second.After(":")).empty() ||
		!Magick::instance().chanserv.GetLive(Magick::instance().server.GetChannel(iter->second.Before(":")))->
		IsIn(Magick::instance().server.GetUser(iter->second.After(":"))))
	    {
		added++;
		MLOCK2(("AllDependancies"));
		if (added == 1)
		{
		    MLOCK(("MsgIdMap"));
		    MsgIdMap[msgid_] = this;
		}
		AllDependancies[iter->first] [iter->second].insert(msgid_);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case UserNoInChan:
	    if (Magick::instance().server.GetChannel(iter->second.Before(":")).empty() ||
		Magick::instance().server.GetUser(iter->second.After(":")).empty() ||
		!Magick::instance().chanserv.GetLive(Magick::instance().server.GetChannel(iter->second.Before(":")))->
		IsIn(Magick::instance().server.GetUser(iter->second.After(":"))))
	    {
		iter->third = true;
	    }
	    else
	    {
		added++;
		MLOCK2(("AllDependancies"));
		if (added == 1)
		{
		    MLOCK(("MsgIdMap"));
		    MsgIdMap[msgid_] = this;
		}
		AllDependancies[iter->first] [iter->second].insert(msgid_);
	    }
	    break;
	}
	if (oldadded != added)
	{
	    CP(("(%d) Added dependancy on %d %s.", msgid_, static_cast < int > (iter->first), iter->second.c_str()));
	}
    }
    ETCB();
}

bool mMessage::RecheckDependancies()
{
    BTCB();
    NFT("mMessage::RecheckDependancies");
    {
	WLOCK(("Dependancies", this));
	bool resolved;

	list < triplet < type_t, mstring, bool > >::iterator iter;
	map < mstring, set < unsigned long > >::iterator i;
	set < unsigned long >::iterator j;

	for (iter = dependancies.begin(); iter != dependancies.end(); iter++)
	{
	    if (iter->third)
		continue;
	    resolved = false;
	    switch (iter->first)
	    {
	    case ServerExists:
		if (!Magick::instance().server.GetServer(iter->second).empty())
		{
		    resolved = true;
		    iter->third = true;
		    MLOCK2(("AllDependancies"));
		    if ((i = AllDependancies[iter->first].find(iter->second)) != AllDependancies[iter->first].end())
		    {
			if ((j = i->second.find(msgid_)) != i->second.end())
			    i->second.erase(j);
			if (!i->second.size())
			    AllDependancies[iter->first].erase(i);
		    }
		}
		break;
	    case ServerNoExists:
		if (Magick::instance().server.GetServer(iter->second).empty())
		{
		    resolved = true;
		    iter->third = true;
		    MLOCK2(("AllDependancies"));
		    if ((i = AllDependancies[iter->first].find(iter->second)) != AllDependancies[iter->first].end())
		    {
			if ((j = i->second.find(msgid_)) != i->second.end())
			    i->second.erase(j);
			if (!i->second.size())
			    AllDependancies[iter->first].erase(i);
		    }
		}
		break;
	    case NickExists:
		if (!Magick::instance().server.GetUser(iter->second).empty())
		{
		    resolved = true;
		    iter->third = true;
		    MLOCK2(("AllDependancies"));
		    if ((i = AllDependancies[iter->first].find(iter->second)) != AllDependancies[iter->first].end())
		    {
			if ((j = i->second.find(msgid_)) != i->second.end())
			    i->second.erase(j);
			if (!i->second.size())
			    AllDependancies[iter->first].erase(i);
		    }
		}
		break;
	    case NickNoExists:
		if (Magick::instance().server.GetUser(iter->second).empty())
		{
		    resolved = true;
		    iter->third = true;
		    MLOCK2(("AllDependancies"));
		    if ((i = AllDependancies[iter->first].find(iter->second)) != AllDependancies[iter->first].end())
		    {
			if ((j = i->second.find(msgid_)) != i->second.end())
			    i->second.erase(j);
			if (!i->second.size())
			    AllDependancies[iter->first].erase(i);
		    }
		}
		break;
	    case ChanExists:
		if (!Magick::instance().server.GetChannel(iter->second).empty())
		{
		    resolved = true;
		    iter->third = true;
		    MLOCK2(("AllDependancies"));
		    if ((i = AllDependancies[iter->first].find(iter->second)) != AllDependancies[iter->first].end())
		    {
			if ((j = i->second.find(msgid_)) != i->second.end())
			    i->second.erase(j);
			if (!i->second.size())
			    AllDependancies[iter->first].erase(i);
		    }
		}
		break;
	    case ChanNoExists:
		if (Magick::instance().server.GetChannel(iter->second).empty())
		{
		    resolved = true;
		    iter->third = true;
		    MLOCK2(("AllDependancies"));
		    if ((i = AllDependancies[iter->first].find(iter->second)) != AllDependancies[iter->first].end())
		    {
			if ((j = i->second.find(msgid_)) != i->second.end())
			    i->second.erase(j);
			if (!i->second.size())
			    AllDependancies[iter->first].erase(i);
		    }
		}
		break;
	    case UserInChan:
		if (!Magick::instance().server.GetChannel(iter->second.Before(":")).empty() &&
		    !Magick::instance().server.GetUser(iter->second.After(":")).empty() &&
		    Magick::instance().chanserv.GetLive(Magick::instance().server.GetChannel(iter->second.Before(":")))->
		    IsIn(Magick::instance().server.GetUser(iter->second.After(":"))))
		{
		    resolved = true;
		    iter->third = true;
		    MLOCK2(("AllDependancies"));
		    if ((i = AllDependancies[iter->first].find(iter->second)) != AllDependancies[iter->first].end())
		    {
			if ((j = i->second.find(msgid_)) != i->second.end())
			    i->second.erase(j);
			if (!i->second.size())
			    AllDependancies[iter->first].erase(i);
		    }
		}
		break;
	    case UserNoInChan:
		if (Magick::instance().server.GetChannel(iter->second.Before(":")).empty() ||
		    Magick::instance().server.GetUser(iter->second.After(":")).empty() ||
		    !Magick::instance().chanserv.GetLive(Magick::instance().server.GetChannel(iter->second.Before(":")))->
		    IsIn(Magick::instance().server.GetUser(iter->second.After(":"))))
		{
		    resolved = true;
		    iter->third = true;
		    MLOCK2(("AllDependancies"));
		    if ((i = AllDependancies[iter->first].find(iter->second)) != AllDependancies[iter->first].end())
		    {
			if ((j = i->second.find(msgid_)) != i->second.end())
			    i->second.erase(j);
			if (!i->second.size())
			    AllDependancies[iter->first].erase(i);
		    }
		}
		break;
	    }
	    if (resolved)
	    {
		CP(("(%d) Resolved dependancy on %d %s.", msgid_, static_cast < int > (iter->first), iter->second.c_str()));
	    }
	}
    }
    if (!OutstandingDependancies())
    {
	CP(("No more dependancies for %d.", msgid_));
	{
	    MLOCK(("MsgIdMap"));
	    map < unsigned long, mMessage * >::iterator k = MsgIdMap.find(msgid_);

	    if (k != MsgIdMap.end())
		MsgIdMap.erase(k);
	}
	RET(false);
    }
    RET(true);
    ETCB();
}

bool mMessage::OutstandingDependancies()
{
    BTCB();
    NFT("mMessage::OutstandingDependancies");

    if (!dependancies.size())
	AddDependancies();

    RLOCK(("Dependancies", this));
    list < triplet < type_t, mstring, bool > >::iterator iter;

    for (iter = dependancies.begin(); iter != dependancies.end(); iter++)
    {
	if (!iter->third)
	{
	    RET(true);
	}
    }

    RET(false);
    ETCB();
}

void mMessage::CheckDependancies(mMessage::type_t type, const mstring & param1, const mstring & param2)
{
    BTCB();
    FT("mMessage::CheckDependancies", (static_cast < int > (type), param1, param2));

    if (param1.empty())
	return;

    // ONE dependancy has been filled, find ALL messages that depended on it, and
    // check if all of its OTHER dependancies have been fulfilled, if so, execute
    // that message.  In either case, remove all dependancies on passed condition,
    // as this has been satisfied.

    mstring target;

    switch (type)
    {
    case ServerExists:
    case ServerNoExists:
	if (param1[0u] == '@')
	    target = param1;
	else
	    target = param1.LowerCase();
	break;
    case NickExists:
    case NickNoExists:
	if (param1[0u] == '!')
	    target = param1;
	else
	    target = param1.LowerCase();
	break;
    case ChanExists:
    case ChanNoExists:
	if (!IsChan(param1))
	    target = param1;
	else
	    target = param1.LowerCase();
	break;
    case UserInChan:
    case UserNoInChan:
	if (!IsChan(param1))
	    target = param1;
	else
	    target = param1.LowerCase();
	target += ":";
	if (param2[0u] == '!')
	    target += param2;
	else
	    target += param2.LowerCase();
	break;
    }

    set < unsigned long > mydep;

    {
	MLOCK(("AllDependancies"));
	map < type_t, map < mstring, set < unsigned long > > >::iterator i = AllDependancies.find(type);

	if (i != AllDependancies.end())
	{
	    map < mstring, set < unsigned long > >::iterator j = i->second.find(target);

	    if (j != i->second.end())
	    {
		mydep = j->second;
		i->second.erase(j);
	    }
	}
    }

    set < unsigned long >::iterator k;

    for (k = mydep.begin(); k != mydep.end(); k++)
    {
	mMessage *msg = NULL;

	{
	    MLOCK(("MsgIdMap"));
	    map < unsigned long, mMessage * >::iterator iter = MsgIdMap.find(*k);

	    if (iter != MsgIdMap.end())
	    {
		msg = iter->second;
		if (msg->validated())
		{
		    msg->DependancySatisfied(type, target);
		    if (!msg->OutstandingDependancies())
			MsgIdMap.erase(iter);
		    else
			msg = NULL;
		}
		else
		    msg = NULL;
	    }
	}
	if (msg != NULL && msg->validated())
	{
	    CP(("No more dependancies for %d.", msg->msgid()));
	    msg->priority(static_cast < unsigned long > (P_DepFilled));

	    RLOCK(("IrcSvcHandler"));
	    if (Magick::instance().ircsvchandler != NULL)
		Magick::instance().ircsvchandler->enqueue(msg);
	}
    }
    ETCB();
}

void mMessage::DependancySatisfied(mMessage::type_t type, const mstring & param)
{
    BTCB();
    FT("mMessage::DependancySatisfied", (static_cast < int > (type), param));

    WLOCK(("Dependancies", this));
    list < triplet < type_t, mstring, bool > >::iterator iter;

    for (iter = dependancies.begin(); iter != dependancies.end(); iter++)
    {
	if (iter->first == type && iter->second == param)
	{
	    CP(("(%d) Resolved dependancy on %d %s.", msgid_, static_cast < int > (iter->first), iter->second.c_str()));
	    iter->third = true;
	    break;
	}
    }
    ETCB();
}

int mMessage::call()
{
    BTCB();
    NFT("mMessage::call");

    if (source_ == " ")
    {
	if (msgtype_.IsSameAs("SHUTDOWN", false))
	{
	    RET(-1);
	}
	else if (msgtype_.IsSameAs("SLEEP", false))
	{
	    ACE_OS::sleep(FromHumanTime(params_) ? FromHumanTime(params_) : 1);
	}
	else if (msgtype_.IsSameAs("TEST", false))
	{
	    // Make sure we dont take any more test messages ...
	    ACE_Thread::yield();
	}
	RET(0);
    }

    mstring src;

    if (source_[0u] == '!')
	src = Magick::instance().server.GetUser(source_);
    else if (source_[0u] == '@')
	src = Magick::instance().server.GetServer(source_);
    else if (source_.empty())
    {
	if (Magick::instance().server.OurUplink().empty())
	    src = Magick::instance().startup.Server_Name();
	else
	    src = Magick::instance().server.OurUplink();
    }
    else
	src = source_;

    CP(("Processing message (%s) %s %s", src.c_str(), msgtype_.c_str(), params_.c_str()));

    try
    {

	if ((msgtype_ == "PRIVMSG" || msgtype_ == "NOTICE") && Magick::instance().nickserv.IsLive(src) &&
	    !IsChan(params_.ExtractWord(1, ": ")))
	{
	    mstring target(params_.ExtractWord(1, ": "));

	    if (target.Contains("@"))
	    {
		target.Truncate(target.Find("@"));
		params_.replace(0, params_.find(" ") - 1, target);
		CP(("Target changed, new params: %s", params_.c_str()));
	    }
	    else if (Magick::instance().server.proto.Numeric.User() &&
		     (Magick::instance().server.proto.Numeric.
		      Combine() ? (target.length() == static_cast < size_t >
				   (Magick::instance().server.proto.Numeric.Server() +
				    Magick::instance().server.proto.Numeric.User())) : (target.length() == static_cast <
											size_t >
											(Magick::instance().server.proto.
											 Numeric.User()))))
	    {
		mstring tmp = Magick::instance().server.GetUser("!" + target);

		if (!tmp.empty())
		{
		    target = tmp;
		    params_.replace(0, params_.find(" ") - 1, target);
		    CP(("Target changed, new params: %s", params_.c_str()));
		}
	    }

	    if (!Magick::instance().nickserv.GetLive(src)->FloodTrigger())
	    {
		// Find out if the target nick is one of the services 'clones'
		// Pass the message to them if so.
		// before even that, check if it's script overriden via
		//     Magick::instance().checkifhandled(servername,command)
		// if so, Magick::instance().doscripthandle(server,command,data);

		if (Magick::instance().operserv.IsName(target))
		    Magick::instance().operserv.execute(src, msgtype_, params_);

		else if (Magick::instance().nickserv.IsName(target) && Magick::instance().nickserv.MSG())
		    Magick::instance().nickserv.execute(src, msgtype_, params_);

		else if (Magick::instance().chanserv.IsName(target) && Magick::instance().chanserv.MSG())
		    Magick::instance().chanserv.execute(src, msgtype_, params_);

		else if (Magick::instance().memoserv.IsName(target) && Magick::instance().memoserv.MSG())
		    Magick::instance().memoserv.execute(src, msgtype_, params_);

		else if (Magick::instance().commserv.IsName(target) && Magick::instance().commserv.MSG())
		    Magick::instance().commserv.execute(src, msgtype_, params_);

		else if (Magick::instance().servmsg.IsName(target) && Magick::instance().servmsg.MSG())
		    Magick::instance().servmsg.execute(src, msgtype_, params_);

		// else check if it's script handled, might do up a list of script servers
		// in the magick object to check against, else trash it.

		else		// PRIVMSG or NOTICE to non-service
		    Magick::instance().server.execute(src, msgtype_, params_);

	    }
	    else if (Magick::instance().operserv.Log_Ignore())
	    {
		// Check if we're to log ignore messages, and log them here.
		LOG(LM_DEBUG, "OPERSERV/IGNORED", (src, msgtype_ + " " + params_));
	    }
	}
	else
	    Magick::instance().server.execute(src, msgtype_, params_);

    }
    catch (E_NickServ_Stored & e)
    {
	switch (e.where())
	{
	case E_NickServ_Stored::W_Get:
	    switch (e.type())
	    {
	    case E_NickServ_Stored::T_Invalid:
	    case E_NickServ_Stored::T_Blank:
		if (strlen(e.what()) && Magick::instance().nickserv.IsStored(e.what()))
		{
		    Magick::instance().nickserv.RemStored(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_NickServ_Live & e)
    {
	switch (e.where())
	{
	case E_NickServ_Live::W_Get:
	    switch (e.type())
	    {
	    case E_NickServ_Live::T_Invalid:
	    case E_NickServ_Live::T_Blank:
		if (strlen(e.what()) && Magick::instance().nickserv.IsLiveAll(e.what()))
		{
		    Magick::instance().nickserv.RemLive(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_NickServ_Recovered & e)
    {
	switch (e.where())
	{
	case E_NickServ_Recovered::W_Get:
	    switch (e.type())
	    {
	    case E_NickServ_Recovered::T_Invalid:
	    case E_NickServ_Recovered::T_Blank:
		if (strlen(e.what()) && Magick::instance().nickserv.IsRecovered(e.what()))
		{
		    Magick::instance().nickserv.RemRecovered(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_ChanServ_Stored & e)
    {
	switch (e.where())
	{
	case E_ChanServ_Stored::W_Get:
	    switch (e.type())
	    {
	    case E_ChanServ_Stored::T_Invalid:
	    case E_ChanServ_Stored::T_Blank:
		if (strlen(e.what()) && Magick::instance().chanserv.IsStored(e.what()))
		{
		    Magick::instance().chanserv.RemStored(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_ChanServ_Live & e)
    {
	switch (e.where())
	{
	case E_ChanServ_Live::W_Get:
	    switch (e.type())
	    {
	    case E_ChanServ_Live::T_Invalid:
	    case E_ChanServ_Live::T_Blank:
		if (strlen(e.what()) && Magick::instance().chanserv.IsLive(e.what()))
		{
		    Magick::instance().chanserv.RemLive(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_CommServ_List & e)
    {
	switch (e.where())
	{
	case E_CommServ_List::W_Get:
	    switch (e.type())
	    {
	    case E_CommServ_List::T_Invalid:
	    case E_CommServ_List::T_Blank:
		if (strlen(e.what()) && Magick::instance().commserv.IsList(e.what()))
		{
		    Magick::instance().commserv.RemList(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_Server_List & e)
    {
	switch (e.where())
	{
	case E_Server_List::W_Get:
	    switch (e.type())
	    {
	    case E_Server_List::T_Invalid:
	    case E_Server_List::T_Blank:
		if (strlen(e.what()) && Magick::instance().server.IsList(e.what()))
		{
		    Magick::instance().server.RemList(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_MemoServ_Nick & e)
    {
	e.what();
    }
    catch (E_MemoServ_Channel & e)
    {
	e.what();
    }
    catch (E_DccMap_Xfers & e)
    {
	switch (e.where())
	{
	case E_DccMap_Xfers::W_Get:
	    switch (e.type())
	    {
	    case E_DccMap_Xfers::T_Invalid:
	    case E_DccMap_Xfers::T_Blank:
		if (strlen(e.what()) && DccMap::IsXfers(atoi(e.what())))
		{
		    DccMap::RemXfers(atoi(e.what()));
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }

    RET(0);
    ETCB();
}

bool mBase::signon(const mstring & nickname) const
{
    BTCB();
    FT("mBase::signon", (nickname));

    if (Magick::instance().nickserv.IsLive(nickname))
    {
	RET(false);
    }
    else
    {
	Magick::instance().server.NICK(nickname,
				       (Magick::instance().startup.Ownuser() ? nickname.LowerCase() : Magick::instance().
					startup.Services_User()), Magick::instance().startup.Services_Host(),
				       Magick::instance().startup.Server_Name(), realname);
	RET(true);
    }
    ETCB();
}

bool mBase::signoff(const mstring & nickname, const mstring & msg) const
{
    BTCB();
    FT("mBase::signoff", (nickname));

    if (Magick::instance().nickserv.IsLive(nickname))
    {
	Magick::instance().server.QUIT(nickname, msg);
	RET(true);
    }
    else
    {
	RET(false);
    }
    ETCB();
}

void mBase::privmsgV(const mstring & dest, const char *pszFormat, ...) const
{
    BTCB();
    FT("mBase::privmsgV", (dest, pszFormat));

    va_list argptr;

    va_start(argptr, pszFormat);
    mstring message;

    message.FormatV(pszFormat, argptr);
    va_end(argptr);
    privmsg(FirstName(), dest, message);
    ETCB();
}

void mBase::privmsgV(const mstring & source, const mstring & dest, const char *pszFormat, ...) const
{
    BTCB();
    FT("mBase::privmsgV", (source, dest, pszFormat));

    va_list argptr;

    va_start(argptr, pszFormat);
    mstring message;

    message.FormatV(pszFormat, argptr);
    va_end(argptr);
    privmsg(source, dest, message);
    ETCB();
}

void mBase::privmsg(const mstring & dest, const mstring & message) const
{
    BTCB();
    FT("mBase::privmsg", (dest, message));
    privmsg(FirstName(), dest, message);
    ETCB();
}

void mBase::privmsg(const mstring & source, const mstring & dest, const mstring & message) const
{
    BTCB();
    FT("mBase::privmsg", (source, dest, message));

    if (IsName(source) && !Magick::instance().getLname(dest).empty())
	Magick::instance().server.PRIVMSG(source, dest, message);
    ETCB();
}

void mBase::noticeV(const mstring & dest, const char *pszFormat, ...) const
{
    BTCB();
    FT("mBase::noticeV", (dest, pszFormat));

    va_list argptr;

    va_start(argptr, pszFormat);
    mstring message;

    message.FormatV(pszFormat, argptr);
    va_end(argptr);
    notice(FirstName(), dest, message);
    ETCB();
}

void mBase::noticeV(const mstring & source, const mstring & dest, const char *pszFormat, ...) const
{
    BTCB();
    FT("mBase::noticeV", (source, dest, pszFormat));

    va_list argptr;

    va_start(argptr, pszFormat);
    mstring message;

    message.FormatV(pszFormat, argptr);
    va_end(argptr);
    notice(source, dest, message);
    ETCB();
}

void mBase::notice(const mstring & dest, const mstring & message) const
{
    BTCB();
    FT("mBase::notice", (dest, message));
    notice(FirstName(), dest, message);
    ETCB();
}

void mBase::notice(const mstring & source, const mstring & dest, const mstring & message) const
{
    BTCB();
    FT("mBase::notice", (source, dest, message));

    if (IsName(source) && !Magick::instance().getLname(dest).empty())
	Magick::instance().server.NOTICE(source, dest, message);
    ETCB();
}

void mBase::sendV(const mstring & dest, const char *pszFormat, ...) const
{
    BTCB();
    FT("mBase::sendV", (dest, pszFormat));

    va_list argptr;

    va_start(argptr, pszFormat);
    mstring message;

    message.FormatV(pszFormat, argptr);
    va_end(argptr);
    send(FirstName(), dest, message);
    ETCB();
}

void mBase::sendV(const mstring & source, const mstring & dest, const char *pszFormat, ...) const
{
    BTCB();
    FT("mBase::sendV", (source, dest, pszFormat));

    va_list argptr;

    va_start(argptr, pszFormat);
    mstring message;

    message.FormatV(pszFormat, argptr);
    va_end(argptr);
    send(source, dest, message);
    ETCB();
}

void mBase::send(const mstring & dest, const mstring & message) const
{
    BTCB();
    FT("mBase::send", (dest, message));
    send(FirstName(), dest, message);
    ETCB();
}

void mBase::send(const mstring & source, const mstring & dest, const mstring & message) const
{
    BTCB();
    FT("mBase::send", (source, dest, message));

    if (IsName(source) && Magick::instance().nickserv.IsLive(dest))
    {
	if (!Magick::instance().nickserv.LCK_PRIVMSG() && Magick::instance().nickserv.IsStored(dest) &&
	    Magick::instance().nickserv.GetStored(dest)->IsOnline())
	{
	    if (Magick::instance().nickserv.GetStored(dest)->PRIVMSG())
	    {
		privmsg(source, dest, message);
	    }
	    else
	    {
		notice(source, dest, message);
	    }
	}
	else
	{
	    if (Magick::instance().nickserv.DEF_PRIVMSG())
	    {
		privmsg(source, dest, message);
	    }
	    else
	    {
		notice(source, dest, message);
	    }
	}
    }
    ETCB();
}

void privmsgV(const mstring & source, const mstring & dest, const char *pszFormat, ...)
{
    BTCB();
    FT("privmsgV", (source, dest, pszFormat));

    va_list argptr;

    va_start(argptr, pszFormat);
    mstring message;

    message.FormatV(pszFormat, argptr);
    va_end(argptr);
    privmsg(source, dest, message);
    ETCB();
}

void privmsg(const mstring & source, const mstring & dest, const mstring & message)
{
    BTCB();
    FT("privmsg", (source, dest, message));

    if (Magick::instance().operserv.IsName(source))
	Magick::instance().operserv.privmsg(source, dest, message);

    else if (Magick::instance().nickserv.IsName(source))
	Magick::instance().nickserv.privmsg(source, dest, message);

    else if (Magick::instance().chanserv.IsName(source))
	Magick::instance().chanserv.privmsg(source, dest, message);

    else if (Magick::instance().memoserv.IsName(source))
	Magick::instance().memoserv.privmsg(source, dest, message);

    else if (Magick::instance().commserv.IsName(source))
	Magick::instance().commserv.privmsg(source, dest, message);

    else if (Magick::instance().servmsg.IsName(source))
	Magick::instance().servmsg.privmsg(source, dest, message);

    // scripted hosts ...
    else
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("PRIVMSG", source));
    }
    ETCB();
}

void noticeV(const mstring & source, const mstring & dest, const char *pszFormat, ...)
{
    BTCB();
    FT("noticeV", (source, dest, pszFormat));

    va_list argptr;

    va_start(argptr, pszFormat);
    mstring message;

    message.FormatV(pszFormat, argptr);
    va_end(argptr);
    notice(source, dest, message);
    ETCB();
}

void notice(const mstring & source, const mstring & dest, const mstring & message)
{
    BTCB();
    FT("notice", (source, dest, message));

    if (Magick::instance().operserv.IsName(source))
	Magick::instance().operserv.notice(source, dest, message);

    else if (Magick::instance().nickserv.IsName(source))
	Magick::instance().nickserv.notice(source, dest, message);

    else if (Magick::instance().chanserv.IsName(source))
	Magick::instance().chanserv.notice(source, dest, message);

    else if (Magick::instance().memoserv.IsName(source))
	Magick::instance().memoserv.notice(source, dest, message);

    else if (Magick::instance().commserv.IsName(source))
	Magick::instance().commserv.notice(source, dest, message);

    else if (Magick::instance().servmsg.IsName(source))
	Magick::instance().servmsg.notice(source, dest, message);

    // scripted hosts ...
    else
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("NOTICE", source));
    }
    ETCB();
}

void sendV(const mstring & source, const mstring & dest, const char *pszFormat, ...)
{
    BTCB();
    FT("sendV", (source, dest, pszFormat));

    va_list argptr;

    va_start(argptr, pszFormat);
    mstring message;

    message.FormatV(pszFormat, argptr);
    va_end(argptr);
    send(source, dest, message);
    ETCB();
}

void send(const mstring & source, const mstring & dest, const mstring & message)
{
    BTCB();
    FT("send", (source, dest, message));

    if (Magick::instance().operserv.IsName(source))
	Magick::instance().operserv.send(source, dest, message);

    else if (Magick::instance().nickserv.IsName(source))
	Magick::instance().nickserv.send(source, dest, message);

    else if (Magick::instance().chanserv.IsName(source))
	Magick::instance().chanserv.send(source, dest, message);

    else if (Magick::instance().memoserv.IsName(source))
	Magick::instance().memoserv.send(source, dest, message);

    else if (Magick::instance().commserv.IsName(source))
	Magick::instance().commserv.send(source, dest, message);

    else if (Magick::instance().servmsg.IsName(source))
	Magick::instance().servmsg.send(source, dest, message);

    // scripted hosts ...
    else
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("SEND", source));
    }
    ETCB();
}

void announceV(const mstring & source, const char *pszFormat, ...)
{
    BTCB();
    FT("announceV", (source, pszFormat));

    va_list argptr;

    va_start(argptr, pszFormat);
    mstring message;

    message.FormatV(pszFormat, argptr);
    va_end(argptr);
    announce(source, message);
    ETCB();
}

void announce(const mstring & source, const mstring & message)
{
    BTCB();
    FT("announce", (source, message));
    Magick::instance().server.GLOBOPS(source, message);
    ETCB();
}

// Command Map stuff ...

void CommandMap::AddSystemCommand(const mstring & service, const mstring & command, const mstring & committees,
				  functor function)
{
    BTCB();
    FT("CommandMap::AddSystemCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_system"));
    i_system[service.LowerCase()].push_back(triplet < mstring, mstring,
					    functor > (command.UpperCase(),
						       ((!committees.empty()) ? committees.LowerCase() : mstring("all")),
						       function));
    ETCB();
}

void CommandMap::RemSystemCommand(const mstring & service, const mstring & command, const mstring & committees)
{
    BTCB();
    FT("CommandMap::RemSystemCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_system"));
    if (i_system.find(service.LowerCase()) != i_system.end())
    {
	cmdtype::iterator iter;
	for (iter = i_system[service.LowerCase()].begin(); iter != i_system[service.LowerCase()].end(); iter++)
	{
	    if (iter->first == command.UpperCase() &&
		mstring(" " + iter->second + " ").Contains(mstring(" " + committees.LowerCase() + " ")))
	    {
		i_system[service.LowerCase()].erase(iter);
		if (!i_system[service.LowerCase()].size())
		    i_system.erase(service.LowerCase());
		return;
	    }
	}
    }
    ETCB();
}

void CommandMap::AddCommand(const mstring & service, const mstring & command, const mstring & committees, functor function)
{
    BTCB();
    FT("CommandMap::AddCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_user"));
    i_user[service.LowerCase()].push_back(triplet < mstring, mstring,
					  functor > (command.UpperCase(),
						     ((!committees.empty()) ? committees.LowerCase() : mstring("all")),
						     function));
    ETCB();
}

void CommandMap::RemCommand(const mstring & service, const mstring & command, const mstring & committees)
{
    BTCB();
    FT("CommandMap::RemCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_user"));
    if (i_user.find(service.LowerCase()) != i_user.end())
    {
	cmdtype::iterator iter;
	for (iter = i_user[service.LowerCase()].begin(); iter != i_user[service.LowerCase()].end(); iter++)
	{
	    if (iter->first == command.UpperCase() &&
		mstring(" " + iter->second + " ").Contains(mstring(" " + committees.LowerCase() + " ")))
	    {
		i_user[service.LowerCase()].erase(iter);
		if (!i_user[service.LowerCase()].size())
		    i_user.erase(service.LowerCase());
		return;
	    }
	}
    }
    ETCB();
}

pair < bool, CommandMap::functor > CommandMap::GetUserCommand(const mstring & service, const mstring & command,
							      const mstring & user) const
{
    BTCB();
    FT("CommandMap::GetUserCommand", (service, command, user));
    unsigned int i;

    pair < bool, functor > retval = pair < bool, functor > (false, NULL);

    cmdtype::const_iterator iter;
    mstring type, list;

    // IF i_system exists
    //   IF command (pattern) exists
    //     IF (" " + cmd_committees + " ") contains ANY OF committees
    //       RETURN true
    //     ENDIF
    //   ENDIF
    // ENDIF
    // RETURN false;

    if (Magick::instance().operserv.IsName(service))
	type = Magick::instance().operserv.GetInternalName().LowerCase();
    else if (Magick::instance().nickserv.IsName(service))
	type = Magick::instance().nickserv.GetInternalName().LowerCase();
    else if (Magick::instance().chanserv.IsName(service))
	type = Magick::instance().chanserv.GetInternalName().LowerCase();
    else if (Magick::instance().memoserv.IsName(service))
	type = Magick::instance().memoserv.GetInternalName().LowerCase();
    else if (Magick::instance().commserv.IsName(service))
	type = Magick::instance().commserv.GetInternalName().LowerCase();
    else if (Magick::instance().servmsg.IsName(service))
	type = Magick::instance().servmsg.GetInternalName().LowerCase();
    //else
    //  scripted stuff ...

    if (type.empty())
	NRET(pair < bool_functor >, retval);

    RLOCK(("CommandMap", "i_user"));
    cmdmap::const_iterator mi = i_user.find(type);
    if (mi != i_user.end())
    {
	for (iter = mi->second.begin(); iter != mi->second.end(); iter++)
	{
	    if (command.Matches(iter->first, true))
	    {
		for (i = 1; i <= iter->second.WordCount(" "); i++)
		{
		    list = iter->second.ExtractWord(i, " ").UpperCase();
		    retval.second = iter->third;
		    // If its a command for "ALL" users, OR
		    // its a valid committee AND a valid (reg'd + online) user
		    //       AND that user is on the committee
		    if (Magick::instance().commserv.IsList(list) && Magick::instance().commserv.GetList(list)->IsOn(user))
		    {
			retval.first = true;
			NRET(pair < bool_functor >, retval);
		    }
		}
	    }
	}
    }
    NRET(pair < bool_functor >, retval);
    ETCB();
}

pair < bool, CommandMap::functor > CommandMap::GetSystemCommand(const mstring & service, const mstring & command,
								const mstring & user) const
{
    BTCB();
    FT("CommandMap::GetSystemCommand", (service, command, user));
    unsigned int i;

    pair < bool, functor > retval = pair < bool, functor > (false, NULL);

    cmdtype::const_iterator iter;
    mstring type, list;

    // IF i_system exists
    //   IF command (pattern) exists
    //     IF (" " + cmd_committees + " ") contains ANY OF committees
    //       RETURN true
    //     ENDIF
    //   ENDIF
    // ENDIF
    // RETURN false;

    if (Magick::instance().operserv.IsName(service))
	type = Magick::instance().operserv.GetInternalName().LowerCase();
    else if (Magick::instance().nickserv.IsName(service))
	type = Magick::instance().nickserv.GetInternalName().LowerCase();
    else if (Magick::instance().chanserv.IsName(service))
	type = Magick::instance().chanserv.GetInternalName().LowerCase();
    else if (Magick::instance().memoserv.IsName(service))
	type = Magick::instance().memoserv.GetInternalName().LowerCase();
    else if (Magick::instance().commserv.IsName(service))
	type = Magick::instance().commserv.GetInternalName().LowerCase();
    else if (Magick::instance().servmsg.IsName(service))
	type = Magick::instance().servmsg.GetInternalName().LowerCase();
    //else
    //  scripted stuff ...

    if (type.empty())
	NRET(pair < bool_functor >, retval);

    RLOCK(("CommandMap", "i_system"));
    cmdmap::const_iterator mi = i_system.find(type);
    if (mi != i_system.end())
    {
	for (iter = mi->second.begin(); iter != mi->second.end(); iter++)
	{
	    if (command.Matches(iter->first, true))
	    {
		for (i = 1; i <= iter->second.WordCount(" "); i++)
		{
		    list = iter->second.ExtractWord(i, " ").UpperCase();
		    retval.second = iter->third;
		    // If its a command for "ALL" users, OR
		    // its a valid committee AND a valid (reg'd + online) user
		    //       AND that user is on the committee
		    if (Magick::instance().commserv.IsList(list) && Magick::instance().commserv.GetList(list)->IsOn(user))
		    {
			retval.first = true;
			NRET(pair < bool_functor >, retval);
		    }
		}
	    }
	}
    }
    NRET(pair < bool_functor >, retval);
    ETCB();
}

bool CommandMap::DoCommand(const mstring & mynick, const mstring & user, const mstring & command, const mstring & params) const
{
    BTCB();
    FT("CommandMap::DoCommand", (mynick, user, command, params));

    bool cmdfound = false;

    pair < bool, functor > cmd = GetUserCommand(mynick, command, user);

    if (cmd.second != NULL)
    {
	if (cmd.first)
	{
	    (*cmd.second) (mynick, user, params);
	    RET(true);
	}
	cmdfound = true;
    }
    else if (cmd.first)
    {
	if (command.WordCount(" ") < 2)
	    SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_COMMAND", (command.UpperCase(), mynick));
	else
	    SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_OPTION", (command.UpperCase(), mynick, command.Before(" ").UpperCase()));
	RET(false);
    }

    cmd = GetSystemCommand(mynick, command, user);
    if (cmd.second != NULL || cmdfound)
    {
	if (cmd.first)
	{
	    (*cmd.second) (mynick, user, params);
	    RET(true);
	}
	else
	{
	    if (command.WordCount(" ") < 2)
		SEND(mynick, user, "ERR_SYNTAX/ACCESS_COMMAND", (command.UpperCase(), mynick));
	    else
		SEND(mynick, user, "ERR_SYNTAX/ACCESS_OPTION", (command.UpperCase(), mynick, command.Before(" ").UpperCase()));
	}
    }
    else
    {
	if (command.WordCount(" ") < 2)
	    SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_COMMAND", (command.UpperCase(), mynick));
	else
	    SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_OPTION", (command.UpperCase(), mynick, command.Before(" ").UpperCase()));
    }
    RET(false);
    ETCB();
}

bool CommandMap::DoUserCommand(const mstring & mynick, const mstring & user, const mstring & command, const mstring & params) const
{
    BTCB();
    FT("CommandMap::DoUserCommand", (mynick, user, command, params));

    pair < bool, functor > cmd = GetUserCommand(mynick, command, user);

    if (cmd.second != NULL)
    {
	if (cmd.first)
	{
	    (*cmd.second) (mynick, user, params);
	    RET(true);
	}
	else
	{
	    if (command.WordCount(" ") < 2)
		SEND(mynick, user, "ERR_SYNTAX/ACCESS_COMMAND", (command.UpperCase(), mynick));
	    else
		SEND(mynick, user, "ERR_SYNTAX/ACCESS_OPTION", (command.UpperCase(), mynick, command.Before(" ").UpperCase()));
	}
    }
    else
    {
	if (command.WordCount(" ") < 2)
	    SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_COMMAND", (command.UpperCase(), mynick));
	else
	    SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_OPTION", (command.UpperCase(), mynick, command.Before(" ").UpperCase()));
    }
    RET(false);
    ETCB();
}

bool CommandMap::DoSystemCommand(const mstring & mynick, const mstring & user, const mstring & command, const mstring & params) const
{
    BTCB();
    FT("CommandMap::DoSystemCommand", (mynick, user, command, params));

    pair < bool, functor > cmd = GetSystemCommand(mynick, command, user);

    if (cmd.second != NULL)
    {
	if (cmd.first)
	{
	    (*cmd.second) (mynick, user, params);
	    RET(true);
	}
	else
	{
	    if (command.WordCount(" ") < 2)
		SEND(mynick, user, "ERR_SYNTAX/ACCESS_COMMAND", (command.UpperCase(), mynick));
	    else
		SEND(mynick, user, "ERR_SYNTAX/ACCESS_OPTION", (command.UpperCase(), mynick, command.Before(" ").UpperCase()));
	}
    }
    else
    {
	if (command.WordCount(" ") < 2)
	    SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_COMMAND", (command.UpperCase(), mynick));
	else
	    SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_OPTION", (command.UpperCase(), mynick, command.Before(" ").UpperCase()));
    }
    RET(false);
    ETCB();
}

void do_1_2param(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("do_1_2param", (mynick, source, params));
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.Before(" ").UpperCase(), mynick, params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.Before(" ", 2));

    command.MakeUpper();

    if (!Magick::instance().commands.DoCommand(mynick, source, command, params))
    {
	// we're not worthy...
//      SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//                      command, mynick,
//                      command.Before(" ")));
    }

    ETCB();
}

void do_1_3param(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("do_1_3param", (mynick, source, params));
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.Before(" ").UpperCase(), mynick, params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.Before(" ") + " " + params.ExtractWord(3, " "));

    command.MakeUpper();

    if (!Magick::instance().commands.DoCommand(mynick, source, command, params))
    {
	// we're not worthy...
//      SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//                      command, mynick,
//                      command.Before(" ")));
    }
    ETCB();
}

void do_1_4param(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("do_1_4param", (mynick, source, params));
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.Before(" ").UpperCase(), mynick, params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.Before(" ") + " " + params.ExtractWord(4, " "));

    command.MakeUpper();

    if (!Magick::instance().commands.DoCommand(mynick, source, command, params))
    {
	// we're not worthy...
//      SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//                      command, mynick,
//                      command.Before(" ")));
    }
    ETCB();
}

void do_1_2paramswap(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("do_1_2paramswap", (mynick, source, params));
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.Before(" ").UpperCase(), mynick, params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(2, " ") + " " + params.Before(" "));

    command.MakeUpper();

    mstring data(command);

    if (params.WordCount(" ") > 2)
	data += " " + params.After(" ", 2);

    if (!Magick::instance().commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//      SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//                      command, mynick,
//                      command.Before(" ")));
    }

    ETCB();
}

void do_1_3paramswap(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("do_1_3paramswap", (mynick, source, params));
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.Before(" ").UpperCase(), mynick, params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(3, " ") + " " + params.Before(" "));

    command.MakeUpper();

    mstring data(params.ExtractWord(3, " ") + " " + params.ExtractWord(2, " ") + " " + params.Before(" "));

    if (params.WordCount(" ") > 3)
	data += " " + params.After(" ", 3);

    if (!Magick::instance().commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//      SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//                      command, mynick,
//                      command.Before(" ")));
    }
    ETCB();
}

void do_1_4paramswap(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("do_1_3paramswap", (mynick, source, params));
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.Before(" ").UpperCase(), mynick, params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(4, " ") + " " + params.Before(" "));

    command.MakeUpper();

    mstring data(params.ExtractWord(4, " ") + " " + params.ExtractWord(2, " ") + params.ExtractWord(3, " ") + " " +
		 params.Before(" "));
    if (params.WordCount(" ") > 4)
	data += " " + params.After(" ", 4);

    if (!Magick::instance().commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//      SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//                      command, mynick,
//                      command.Before(" ")));
    }
    ETCB();
}

void do_2param(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("do_2param", (mynick, source, params));
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.Before(" ").UpperCase(), mynick, params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(2, " "));

    command.MakeUpper();

    mstring data(command + " " + params.Before(" "));

    if (params.WordCount(" ") > 2)
	data += " " + params.After(" ", 2);

    if (!Magick::instance().commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//      SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//                      command, mynick,
//                      command.Before(" ")));
    }

    ETCB();
}

void do_3param(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("do_3param", (mynick, source, params));
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.Before(" ").UpperCase(), mynick, params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(3, " "));

    command.MakeUpper();

    mstring data(params.ExtractWord(3, " ") + " " + params.ExtractWord(2, " ") + " " + params.Before(" "));

    if (params.WordCount(" ") > 3)
	data += " " + params.After(" ", 3);

    if (!Magick::instance().commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//      SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//                      command, mynick,
//                      command.Before(" ")));
    }
    ETCB();
}

void do_4param(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("do_3param", (mynick, source, params));
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.Before(" ").UpperCase(), mynick, params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(4, " "));

    command.MakeUpper();

    mstring data(params.ExtractWord(4, " ") + " " + params.ExtractWord(2, " ") + " " + params.ExtractWord(3, " ") + " " +
		 params.Before(" "));
    if (params.WordCount(" ") > 4)
	data += " " + params.After(" ", 4);

    if (!Magick::instance().commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//      SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//                      command, mynick,
//                      command.Before(" ")));
    }
    ETCB();
}
