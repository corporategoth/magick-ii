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
unsigned long mMessage::LastMsgId = 0;

entlist_t &entlist_t::operator=(const entlist_t & in)
{
    BTCB();
    FT("entlist_t::operator=", ("(const entlist_t &) in"));
    i_Entry = in.i_Entry;
    i_Last_Modify_Time = in.i_Last_Modify_Time;
    i_Last_Modifier = in.i_Last_Modifier;
    i_UserDef = in.i_UserDef;
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
    for_each(i_UserDef.begin(), i_UserDef.end(), SXP::WritePairElement(pOut, tag_UserDef));

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

mMessage::mMessage(const mstring & p_source, const mstring & p_msgtype, const mstring & p_params)
: msgid_(0), sourceToken_(false), source_(p_source), params_(p_params), creation_(mDateTime::CurrentDateTime())
{
    BTCB();
    FT("mMessage::mMessage", (p_source, p_msgtype, p_params));


    if (source_ != " " && Magick::instance().server.proto.Tokens())
    {
	mstring tmp(Magick::instance().server.proto.GetToken(p_msgtype));

	if (!tmp.empty())
	    msgtype_ = tmp;
    }

    if (msgtype_.empty())
	msgtype_ = p_msgtype;

    msgtype_.MakeUpper();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL)
	    msgid_ = Magick::instance().ircsvchandler->GetNewMsgid();
    }

    lockData(mVarArray("Message", msgid_));

    ETCB();
}

void mMessage::AddDependancies()
{
    BTCB();
    NFT("mMessage::AddDependancies");
    WLOCK(("Dependancies", this));
    int added = 0;

    mstring source;
    if (!source_.empty())
    {
	if (source_[0u] == '@')
	{
	    source = source_;
	    AddDepend(ServerExists, source);
	}
	else if (source_.Contains("."))
	{
	    source = source_.LowerCase();
	    AddDepend(ServerExists, source);
	}
	else if (source_[0u] == '!')
	{
	    source = source_;
	    AddDepend(NickExists, source);
	}
	else
	{
	    source = source_.LowerCase();
	    AddDepend(NickExists, source);
	}
    }

    if (msgtype_ == "BURST")
    {
	if (params_.WordCount(" ") > 2)
	{
	    size_t i, offset = 3;
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

	    mstring users = params_.After(" ", offset);
	    for (i=1; i<=users.WordCount(","); i++)
	    {
		mstring nick(users.ExtractWord(i, ",").Before(":"));

		if (Magick::instance().server.proto.Numeric.User())
		    AddDepend(NickExists, "!" + nick);
		else
		    AddDepend(NickExists, nick.LowerCase());
	    }
	}
    }
    else if (msgtype_ == "JOIN")
    {
	// User is NOT in channel
	unsigned int count = IrcParamCount(params_);
	for (unsigned int i = 1; i <= count; i++)
	{
	    unsigned int chans = IrcParam(params_, i).WordCount(",");
	    for (unsigned int j = 1; j <= chans; j++)
	    {
		mstring chan = IrcParam(params_, i).ExtractWord(j, ", ");

		if (IsChan(chan))
		    AddDepend(UserNoInChan, chan.LowerCase() + " " + source);
		else
		    AddDepend(UserNoInChan, chan + " " + source);
	    }
	}
    }
    else if (msgtype_ == "KICK")
    {
	// Channel exists
	// Target exists
	// Target in channel
	AddDepend(ChanExists, IrcParam(params_, 1).LowerCase());
	if (Magick::instance().server.proto.Numeric.User())
	{
	    AddDepend(NickExists, "!" + IrcParam(params_, 2));
	    AddDepend(UserInChan, IrcParam(params_, 1).LowerCase() + " " + "!" + IrcParam(params_, 2));
	}
	else
	{
	    AddDepend(NickExists, IrcParam(params_, 2).LowerCase());
	    AddDepend(UserInChan, IrcParam(params_, 1).LowerCase() + " " + IrcParam(params_, 2).LowerCase());
	}
    }
    else if (msgtype_ == "KILL")
    {
	// Target exists
	if (Magick::instance().server.proto.Numeric.User())
	    AddDepend(NickExists, "!" + IrcParam(params_, 1));
	else
	    AddDepend(NickExists, IrcParam(params_, 1).LowerCase());
    }
    else if ((msgtype_ == "MODE" || msgtype_ == "SVSMODE") && IsChan(IrcParam(params_, 1)))
    {
	// Channel exists
	// ALL params_ that are users exist ... ?
	AddDepend(ChanExists, IrcParam(params_, 1).LowerCase());

	mstring mode = IrcParam(params_, 2);
	unsigned int i, fwdarg = 1;

	unsigned int count = IrcParamCount(params_);
	mstring mode_arg;
	for (i=3; i<=count; i++)
	    mode_arg += " " + IrcParam(params_, i);

	for (i = 0; i < mode.length() && fwdarg <= mode_arg.WordCount(" "); i++)
	{
	    if (Magick::instance().server.proto.ChanModeArg().Contains(mode[i]))
	    {
		switch (mode[i])
		{
		case 'o':
		case 'h':
		case 'v':
		    if (Magick::instance().server.proto.Numeric.User())
			AddDepend(NickExists, "!" + mode_arg.ExtractWord(fwdarg, " "));
		    else
			AddDepend(NickExists, mode_arg.ExtractWord(fwdarg, " ").LowerCase());
		    break;
		}
		fwdarg++;
	    }
	}
    }
    else if (msgtype_ == "NICK" || msgtype_ == "USER")
    {
	// Server exists OR
	// Target nick does NOT exist
	AddDepend(NickNoExists, IrcParam(params_, 1).LowerCase());
	if (source.empty() || source[0u] == '@')
	{
	    mstring server;

	    switch (Magick::instance().server.proto.Signon())
	    {
	    case 0000:
		server = IrcParam(params_, 4);
		break;
	    case 0001:
		server = IrcParam(params_, 5);
		break;
	    case 1000:
	    case 1001:
	    case 1002:
		server = IrcParam(params_, 6);
		break;
	    case 1003:
	    case 2000:
	    case 2001:
		server = IrcParam(params_, 7);
		break;
	    case 2002:
		server = IrcParam(params_, 8);
		break;
	    case 2003:
		server = IrcParam(params_, 6);
		break;
	    case 3000:
		// Source is already added ...
		break;
	    }
	    if (!server.empty())
	    {
		if (Magick::instance().server.proto.Numeric.Server() && !server.Contains("."))
		    AddDepend(ServerExists, "@" + server);
		else
		    AddDepend(ServerExists, server.LowerCase());
	    }
	}
    }
    else if (msgtype_ == "PART")
    {
	// Channel exists
	// User in channel
	mstring chan = IrcParam(params_, 1);

	if (IsChan(chan))
	{
	    AddDepend(ChanExists, chan.LowerCase());
	    AddDepend(UserInChan, chan.LowerCase() + " " + source);
	}
	else
	{
	    AddDepend(ChanExists, chan);
	    AddDepend(UserInChan, chan + " " + source);
	}
    }
    else if (msgtype_ == "SERVER")
    {
	// Uplink exists
	AddDepend(ServerNoExists, IrcParam(params_, 1).LowerCase());
    }
    else if (msgtype_ == "SJOIN")
    {
	// ALL nick's mentioned
	if (source[0u] == '@' || source.Contains(".") || source.empty())
	{
	    mstring nicks = params_.After(" :");
	    for (unsigned int i = 1; i <= nicks.WordCount(" "); i++)
	    {
		mstring nick;
		if (Magick::instance().server.proto.Numeric.User())
		    nick = "!" + nicks.ExtractWord(i, " ");
		else
		    nick = nicks.ExtractWord(i, " ").LowerCase();

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
			case '.':
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
			AddDepend(UserNoInChan, IrcParam(params_, 2).LowerCase() + " " + nick);
		    }
		}
	    }
	}
	else
	{
	    unsigned int count = IrcParamCount(params_);
	    for (unsigned int i = 2; i <= count; i++)
	    {
		char c = 0;
		mstring chan = IrcParam(params_, i).LowerCase();

		while (c != chan[0u])
		{
		    c = chan[0u];
		    switch (chan[0u])
		    {
		    case '@':
		    case '%':
		    case '+':
		    case '*':
		    case '.':
		    case '~':
			chan.erase(0, 0);
			break;
		    }
		}
		AddDepend(UserNoInChan, chan + " " + source);
	    }
	}
    }
    else if (msgtype_ == "SQUIT")
    {
	// Uplink exists
	mstring server = IrcParam(params_, 1);

	if (server.Contains("."))
	    AddDepend(ServerExists, server.LowerCase());
	else
	    AddDepend(ServerExists, "@" + server);
    }
    else if (msgtype_ == "TOPIC")
    {
	// Channel exists
	mstring chan = IrcParam(params_, 1);

	if (IsChan(chan))
	    AddDepend(ChanExists, chan.LowerCase());
	else
	    AddDepend(ChanExists, chan);
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
		MLOCK2((lck_AllDeps));
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
		MLOCK2((lck_AllDeps));
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
		MLOCK2((lck_AllDeps));
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
		MLOCK2((lck_AllDeps));
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
		MLOCK2((lck_AllDeps));
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
		MLOCK2((lck_AllDeps));
		AllDependancies[iter->first] [iter->second].insert(msgid_);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case UserInChan:
	    if (Magick::instance().server.GetChannel(iter->second.Before(" ")).empty() ||
		Magick::instance().server.GetUser(iter->second.After(" ")).empty() ||
		!Magick::instance().chanserv.GetLive(Magick::instance().server.GetChannel(
			iter->second.Before(" ")))->IsIn(Magick::instance().server.GetUser(iter->second.After(" "))))
	    {
		added++;
		MLOCK2((lck_AllDeps));
		AllDependancies[iter->first] [iter->second].insert(msgid_);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case UserNoInChan:
	    if (Magick::instance().server.GetChannel(iter->second.Before(" ")).empty() ||
		Magick::instance().server.GetUser(iter->second.After(" ")).empty() ||
		!Magick::instance().chanserv.GetLive(Magick::instance().server.GetChannel(
			iter->second.Before(" ")))->IsIn(Magick::instance().server.GetUser(iter->second.After(" "))))
	    {
		iter->third = true;
	    }
	    else
	    {
		added++;
		MLOCK2((lck_AllDeps));
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
		    MLOCK2((lck_AllDeps));
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
		    MLOCK2((lck_AllDeps));
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
		    MLOCK2((lck_AllDeps));
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
		    MLOCK2((lck_AllDeps));
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
		    MLOCK2((lck_AllDeps));
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
		    MLOCK2((lck_AllDeps));
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
		if (!Magick::instance().server.GetChannel(iter->second.Before(" ")).empty() &&
		    !Magick::instance().server.GetUser(iter->second.After(" ")).empty() &&
		    Magick::instance().chanserv.GetLive(Magick::instance().server.GetChannel(
			iter->second.Before(" ")))->IsIn(Magick::instance().server.GetUser(iter->second.After(" "))))
		{
		    resolved = true;
		    iter->third = true;
		    MLOCK2((lck_AllDeps));
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
		if (Magick::instance().server.GetChannel(iter->second.Before(" ")).empty() ||
		    Magick::instance().server.GetUser(iter->second.After(" ")).empty() ||
		    !Magick::instance().chanserv.GetLive(Magick::instance().server.GetChannel(
			iter->second.Before(" ")))->IsIn(Magick::instance().server.GetUser(iter->second.After(" "))))
		{
		    resolved = true;
		    iter->third = true;
		    MLOCK2((lck_AllDeps));
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
	target += " ";
	if (param2[0u] == '!')
	    target += param2;
	else
	    target += param2.LowerCase();
	break;
    }

    set < unsigned long > mydep;

    {
	MLOCK((lck_AllDeps));
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
	map_entry<mMessage> msg;

	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->IsMessage(*k))
		msg = Magick::instance().ircsvchandler->GetMessage(*k);
	}

	if (msg != NULL)
	{
	    msg->DependancySatisfied(type, target);
	    if (!msg->OutstandingDependancies())
	    {
		RLOCK((lck_IrcSvcHandler));
		if (Magick::instance().ircsvchandler != NULL)
		    Magick::instance().ircsvchandler->enqueue(msg->msgid(), P_DepFilled);
	    }
	}
    }
    ETCB();
}

void mMessage::KillDependancies(mMessage::type_t type, const mstring & param1, const mstring & param2)
{
    BTCB();
    FT("mMessage::CheckDependancies", (static_cast < int > (type), param1, param2));

    if (param1.empty())
	return;

    // This dependancy will NEVER be filled, remove all messages relying on it.

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
	target += " ";
	if (param2[0u] == '!')
	    target += param2;
	else
	    target += param2.LowerCase();
	break;
    }

    set < unsigned long > mydep;

    {
	MLOCK((lck_AllDeps));
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
	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->IsMessage(*k))
		Magick::instance().ircsvchandler->RemMessage(*k);
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

    try
    {
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

	if ((msgtype_ == "PRIVMSG" || msgtype_ == "NOTICE") && Magick::instance().nickserv.IsLive(src) &&
	    !IsChan(IrcParam(params_, 1)))
	{
	    mstring target(IrcParam(params_, 1));

	    if (target.Contains("@"))
	    {
		params_.replace(target, target.Before("@"), false);
		target.Truncate(target.Find("@"));
		CP(("Target changed, new params: %s", params_.c_str()));
	    }
	    else if (Magick::instance().server.proto.Numeric.User() &&
		     (Magick::instance().server.proto.Numeric. Combine() ?
		      (target.length() == static_cast < size_t > (Magick::instance().server.proto.Numeric.Server() +
								  Magick::instance().server.proto.Numeric.User())) :
		      (target.length() == static_cast < size_t >(Magick::instance().server.proto.Numeric.User()))))
	    {
		mstring tmp = Magick::instance().server.GetUser("!" + target);

		if (!tmp.empty())
		{
		    params_.replace(target, tmp, false);
		    target = tmp;
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

		mBase *serv = mBase::GetByName(target);

		// Add special case for OperServ, because MSG() is handled later.
		if ((serv != NULL && serv->MSG()) || serv == &Magick::instance().operserv)
		    serv->execute(src, msgtype_, params_);
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

mBase *mBase::GetByName(const mstring & in)
{
    FT("mBase::GetByName", (in));

    mBase *serv = NULL;

    if (Magick::instance().operserv.IsName(in))
	serv = &Magick::instance().operserv;
    else if (Magick::instance().nickserv.IsName(in))
	serv = &Magick::instance().nickserv;
    else if (Magick::instance().chanserv.IsName(in))
	serv = &Magick::instance().chanserv;
    else if (Magick::instance().memoserv.IsName(in))
	serv = &Magick::instance().memoserv;
    else if (Magick::instance().commserv.IsName(in))
	serv = &Magick::instance().commserv;
    else if (Magick::instance().servmsg.IsName(in))
	serv = &Magick::instance().servmsg;

    NRET(mBase *, serv);
}

mBase *mBase::GetByInternalName(const mstring & in)
{
    FT("mBase::GetByInternalName", (in));

    mBase *serv = NULL;

    if (Magick::instance().operserv.GetInternalName().IsSameAs(in, true))
	serv = &Magick::instance().operserv;
    else if (Magick::instance().nickserv.GetInternalName().IsSameAs(in, true))
	serv = &Magick::instance().nickserv;
    else if (Magick::instance().chanserv.GetInternalName().IsSameAs(in, true))
	serv = &Magick::instance().chanserv;
    else if (Magick::instance().memoserv.GetInternalName().IsSameAs(in, true))
	serv = &Magick::instance().memoserv;
    else if (Magick::instance().commserv.GetInternalName().IsSameAs(in, true))
	serv = &Magick::instance().commserv;
    else if (Magick::instance().servmsg.GetInternalName().IsSameAs(in, true))
	serv = &Magick::instance().servmsg;

    NRET(mBase *, serv);
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
				       (Magick::instance().startup.Ownuser() ? nickname.LowerCase() :
					Magick::instance().startup.Services_User()), Magick::instance().startup.Services_Host(),
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

    mBase *serv = mBase::GetByName(source);

    if (serv != NULL)
	serv->privmsg(source, dest, message);
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

    mBase *serv = mBase::GetByName(source);

    if (serv != NULL)
	serv->notice(source, dest, message);
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

    mBase *serv = mBase::GetByName(source);

    if (serv != NULL)
	serv->send(source, dest, message);
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

void CommandMap::AddAnyCommand(const mstring & name, cmdmap & mymap, const mstring & service, const mstring & command,
				  const mstring & committees, functor function)
{
    BTCB();
    FT("CommandMap::AddAnyCommand", (name, "(CommandMap::cmdmap) &mymap", service, command, committees));

    WLOCK(("CommandMap", name));
    mymap[service.LowerCase()].push_back(triplet < mstring, mstring, functor > (command.UpperCase(),
					    ((!committees.empty()) ? committees.LowerCase() : mstring("all")), function));
    ETCB();
}

void CommandMap::RemAnyCommand(const mstring & name, cmdmap & mymap, const mstring & service,
				  const mstring & command, const mstring & committees)
{
    BTCB();
    FT("CommandMap::RemAnyCommand", (name, "(CommandMap::cmdmap) &mymap", service, command, committees));

    WLOCK(("CommandMap", name));
    if (mymap.find(service.LowerCase()) != mymap.end())
    {
	cmdtype::iterator iter;
	for (iter = mymap[service.LowerCase()].begin(); iter != mymap[service.LowerCase()].end(); iter++)
	{
	    if (iter->first == command.UpperCase() &&
		mstring(" " + iter->second + " ").Contains(mstring(" " + committees.LowerCase() + " ")))
	    {
		mymap[service.LowerCase()].erase(iter);
		if (!mymap[service.LowerCase()].size())
		    mymap.erase(service.LowerCase());
		return;
	    }
	}
    }
    ETCB();
}

pair < bool, CommandMap::functor > CommandMap::GetAnyCommand(const mstring & name, const cmdmap & mymap,
							     const mstring & service, const mstring & command,
							     const mstring & user)
{
    BTCB();
    FT("CommandMap::GetAnyCommand", (name, "(CommandMap::cmdmap) & mymap", service, command, user));
    unsigned int i;

    pair < bool, functor > retval = pair < bool, functor > (false, NULL);

    cmdtype::const_iterator iter;
    mstring type, list;

    // IF mymap exists
    //   IF command (pattern) exists
    //     IF (" " + cmd_committees + " ") contains ANY OF committees
    //       RETURN true
    //     ENDIF
    //   ENDIF
    // ENDIF
    // RETURN false;

    mBase *serv = mBase::GetByName(service);

    if (serv != NULL)
	type = serv->GetInternalName().LowerCase();

    if (type.empty())
	NRET(pair < bool_functor >, retval);

    RLOCK(("CommandMap", name));
    cmdmap::const_iterator mi = mymap.find(type);
    if (mi != mymap.end())
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

bool CommandMap::DoAnyCommand(pair < bool, functor > & cmd, const mstring & mynick, const mstring & user, const mstring & command,
			      const mstring & params)
{
    BTCB();
    FT("CommandMap::DoAnyCommand", ("(const mstring &) cmd", mynick, user, command, params));

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

void do_Xparam(const mstring & mynick, const mstring & source, const mstring & params, unsigned int x)
{
    BTCB();
    FT("do_Xparam", (mynick, source, params, x));
    if (params.WordCount(" ") < x)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.Before(" ").UpperCase(), mynick, params.Before(" ").UpperCase()));
	return;
    }

    mstring command(params.ExtractWord(x, " "));
    mstring data(params);
    int begin = data.WordPosition(x, " ");
    int end = data.WordPosition(x + 1, " ");
    if (end > 0)
	end--;
    data.erase(begin, end);
    data.prepend(command);
    command.MakeUpper();

    Magick::instance().commands.DoCommand(mynick, source, command, data);
    ETCB();
}

void do_X_Yparam(const mstring & mynick, const mstring & source, const mstring & params, unsigned int x, unsigned int y)
{
    BTCB();
    FT("do_X_Yparam", (mynick, source, params, x, y));

    int min = (x < y ? x : y);
    if (params.WordCount(" ") < (x < y ? y : x))
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS",
	     (params.ExtractWord(min, " ").UpperCase(), mynick, params.ExtractWord(min, " ").UpperCase()));
	return;
    }

    mstring command(params.ExtractWord(x, " ") + " " + params.ExtractWord(y, " "));
    command.MakeUpper();

    Magick::instance().commands.DoCommand(mynick, source, command, params);
    ETCB();
}
