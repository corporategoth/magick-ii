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
#define RCSID(x,y) const char *rcsid_commserv_cpp_ ## x () { return y; }
RCSID(commserv_cpp, "@(#)$Id$");

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
static Committee_t GLOB_Committee_t;
#endif

void Committee_t::defaults()
{
    BTCB();
    NFT("Committee_t::defaults");

    ref_class::lockData(mVarArray(lck_CommServ, lck_list, i_Name.UpperCase()));
    setting.Private = Magick::instance().commserv.DEF_Private();
    lock.Private = false;
    setting.OpenMemos = Magick::instance().commserv.DEF_OpenMemos();
    lock.OpenMemos = false;
    setting.Secure = Magick::instance().commserv.DEF_Secure();
    lock.Secure = false;
    ETCB();
}

Committee_t::Committee_t() : i_RegTime(mDateTime::CurrentDateTime())
{
    BTCB();
    NFT("Committee_t::Committee_t");
    defaults();
    DumpE();
    ETCB();
}

Committee_t::Committee_t(const mstring & name, const mstring & head, const mstring & description) : i_Name(name.UpperCase()),
i_RegTime(mDateTime::CurrentDateTime()), i_Head(head.LowerCase()), i_Description(description)
{
    BTCB();
    FT("Committee_t::Committee_t", (name, head, description));
    defaults();
    DumpE();
    ETCB();
}

Committee_t::Committee_t(const mstring & name, const Committee_t & head,
			 const mstring & description) : i_Name(name.UpperCase()), i_RegTime(mDateTime::CurrentDateTime()),
i_HeadCom(head.Name()), i_Description(description)
{
    BTCB();
    FT("Committee_t::Committee_t", (name, "(Committee_t *) head", description));
    defaults();
    DumpE();
    ETCB();
}

Committee_t::Committee_t(const mstring & name, const mstring & description) : i_Name(name.UpperCase()),
i_RegTime(mDateTime::CurrentDateTime()), i_Description(description)
{
    BTCB();
    FT("Committee_t::Committee_t", (name, description));
    defaults();
    DumpE();
    ETCB();
}

Committee_t &Committee_t::operator=(const Committee_t & in)
{
    BTCB();
    FT("Committee_t::operator=", ("(const Committee_t &) in"));

    i_Name = in.i_Name;
    ref_class::lockData(mVarArray(lck_CommServ, lck_list, i_Name.UpperCase()));
    i_RegTime = in.i_RegTime;
    i_Head = in.i_Head;
    i_HeadCom = in.i_HeadCom;
    i_Description = in.i_Description;
    i_Email = in.i_Email;
    i_URL = in.i_URL;
    setting.OpenMemos = in.setting.OpenMemos;
    lock.OpenMemos = in.lock.OpenMemos;
    setting.Secure = in.setting.Secure;
    lock.Secure = in.lock.Secure;
    setting.Private = in.setting.Private;
    lock.Private = in.lock.Private;
    i_Members = in.i_Members;
    i_Messages = in.i_Messages;

    map < mstring, mstring >::const_iterator j;
    i_UserDef.clear();
    for (j = in.i_UserDef.begin(); j != in.i_UserDef.end(); j++)
	i_UserDef.insert(*j);
    NRET(Committee_t &, *this);
    ETCB();
}

mDateTime Committee_t::RegTime() const
{
    BTCB();
    NFT("Committee_t::RegTime");
    RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_RegTime"));
    RET(i_RegTime);
    ETCB();
}

unsigned long Committee_t::Drop()
{
    BTCB();
    unsigned long dropped = 1;

    {
	vector < mstring > chunked;
	RLOCK((lck_CommServ, lck_list));
	CommServ::list_t::iterator citer;
	for (citer = Magick::instance().commserv.ListBegin(); citer != Magick::instance().commserv.ListEnd(); citer++)
	{
	    map_entry < Committee_t > comm(citer->second);
	    if (comm->HeadCom().IsSameAs(i_Name))
		chunked.push_back(comm->Name());
	}
	for (unsigned int i = 0; i < chunked.size(); i++)
	{
	    dropped += Magick::instance().commserv.GetList(chunked[i])->Drop();
	    Magick::instance().commserv.RemList(chunked[i]);
	}
    }

    // Now we go for our channels ...
    ChanServ::stored_t::iterator iter;
    mstring entry = "@" + i_Name;

    {
	RLOCK((lck_ChanServ, lck_stored));
	for (iter = Magick::instance().chanserv.StoredBegin(); iter != Magick::instance().chanserv.StoredEnd(); iter++)
	{
	    map_entry < Chan_Stored_t > cstored(iter->second);
	    MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Access"));
	    if (cstored->Access_find(entry))
		cstored->Access_erase();
	}
    }
    return dropped;
    ETCB();
}

mstring Committee_t::HeadCom() const
{
    BTCB();
    NFT("Committee_t::HeadCom");
    RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_HeadCom"));
    RET(i_HeadCom);
    ETCB();
}

void Committee_t::HeadCom(const mstring & newhead)
{
    BTCB();
    FT("Committee_t::HeadCom", (newhead));

    WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_HeadCom"));
    MCB(i_HeadCom);
    {
	WLOCK2((lck_CommServ, lck_list, i_Name.UpperCase(), "i_Head"));
	if (!i_Head.empty())
	{
	    CB(1, i_Head);
	    i_Head.erase();
	    CE(1, i_Head);
	}
    }
    i_HeadCom = newhead.LowerCase();
    MCE(i_HeadCom);
    ETCB();
}

mstring Committee_t::Head() const
{
    BTCB();
    NFT("Committee_t::Head");
    RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_Head"));
    RET(i_Head);
    ETCB();
}

void Committee_t::Head(const mstring & newhead)
{
    BTCB();
    FT("Committee_t::Head", (newhead));

    WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_Head"));
    MCB(i_Head);
    {
	WLOCK2((lck_CommServ, lck_list, i_Name.UpperCase(), "i_HeadCom"));
	if (!i_HeadCom.empty())
	{
	    CB(1, i_HeadCom);
	    i_HeadCom.erase();
	    CE(1, i_HeadCom);
	}
    }
    i_Head = newhead.LowerCase();
    MCE(i_Head);
    ETCB();
}

bool Committee_t::insert(const mstring & entry, const mstring & nick, const mDateTime & modtime)
{
    BTCB();
    FT("Committee_t::insert", (entry, nick, modtime));

    entlist_ui iter;

    MLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "member"));
    if (!i_Members.empty())
	for (iter = i_Members.begin(); iter != i_Members.end(); iter++)
	    if (iter->Entry().IsSameAs(entry, true))
		break;
    if (i_Members.empty() || iter == i_Members.end())
    {
	pair < entlist_ui, bool > tmp;

	MCB(i_Members.size());
	entlist_t ent(entry, nick, modtime);
	i_Members.erase(ent);
	tmp = i_Members.insert(ent);
	MCE(i_Members.size());
	if (tmp.second)
	    member = tmp.first;
	else
	    member = i_Members.end();
	RET(true);
    }
    else
    {
	RET(false);
    }
    ETCB();
}

bool Committee_t::erase()
{
    BTCB();
    NFT("Committee_t::erase");

    MLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "member"));
    if (member != i_Members.end())
    {
	MCB(i_Members.size());
	i_Members.erase(member);
	MCE(i_Members.size());
	member = i_Members.end();
	RET(true);
    }
    else
    {
	RET(false);
    }
    ETCB();
}

bool Committee_t::find(const mstring & entry)
{
    BTCB();
    FT("Committee_t::find", (entry));

    MLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "member"));
    entlist_ui iter = i_Members.end();

    if (!i_Members.empty())
    {
	// FIND exact nickname
	for (iter = i_Members.begin(); iter != i_Members.end(); iter++)
	    if (iter->Entry().IsSameAs(entry, true))
		break;
    }

    if (iter != i_Members.end())
    {
	member = iter;
	RET(true);
    }
    else
    {
	member = i_Members.end();
	RET(false);
    }
    ETCB();
}

bool Committee_t::IsIn(const mstring & nick) const
{
    BTCB();
    FT("Committee_t::IsIn", (nick));

    if (i_Name == Magick::instance().commserv.ALL_Name() && Magick::instance().nickserv.IsLive(nick))
    {
	RET(true);
    }

    if (!Magick::instance().nickserv.IsStored(nick))
    {
	RET(false);
    }

    if (i_Name.IsSameAs(Magick::instance().commserv.REGD_Name(), true))
    {
	RET(true);
    }

    mstring target = Magick::instance().nickserv.GetStored(nick)->Host();

    if (target.empty())
	target = nick;

    // We're a HEAD, in by DEFAULT
    {
	RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_HeadCom"));
	if (!i_HeadCom.empty() && Magick::instance().commserv.IsList(i_HeadCom) &&
	    Magick::instance().commserv.GetList(i_HeadCom)->IsIn(target))
	{
	    RET(true);
	}
    }

    {
	RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_Head"));
	if (!i_Head.empty() && target.IsSameAs(i_Head, true))
	{
	    RET(true);
	}
    }

    vector < mstring > members;
    unsigned int i;

    {
	MLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "member"));
	entlist_cui iter;

	for (iter = i_Members.begin(); iter != i_Members.end(); iter++)
	{
	    members.push_back(iter->Entry());
	}
    }

    for (i = 0; i < members.size(); i++)
    {
	if (target.IsSameAs(members[i], true))
	{
	    RET(true);
	}
    }

    // If this committee is SADMIN, we could be a member
    // as a SLAVE since theres no way of online changing host.
    if (i_Name == Magick::instance().commserv.SADMIN_Name())
    {
	unsigned int j;

	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
	for (i = 0; i < nstored->Siblings(); i++)
	{
	    for (j = 0; j < members.size(); j++)
	    {
		if (nstored->Sibling(i).IsSameAs(members[j], true))
		{
		    RET(true);
		}
	    }
	}
    }

    RET(false);
    ETCB();
}

bool Committee_t::IsOn(const mstring & nick) const
{
    BTCB();
    FT("Committee_t::IsOn", (nick));

    // If we aint online, we aint in nothing.
    if (!Magick::instance().nickserv.IsLive(nick))
    {
	RET(false);
    }

    if (i_Name == Magick::instance().commserv.ALL_Name())
    {
	RET(true);
    }

    // The committee we're looking at has ModeO set,
    // but user doesnt have umode +o.
    if (((i_Name.IsSameAs(Magick::instance().commserv.SADMIN_Name(), true) && Magick::instance().commserv.SADMIN_ModeO()) ||
	 (i_Name.IsSameAs(Magick::instance().commserv.SOP_Name(), true) && Magick::instance().commserv.SOP_ModeO()) ||
	 (i_Name.IsSameAs(Magick::instance().commserv.ADMIN_Name(), true) && Magick::instance().commserv.ADMIN_ModeO()) ||
	 (i_Name.IsSameAs(Magick::instance().commserv.OPER_Name(), true) && Magick::instance().commserv.OPER_ModeO())) &&
	!Magick::instance().nickserv.GetLive(nick)->HasMode("o"))
    {
	RET(false);
    }

    // This returns wether we're CURRENTLY recognized
    // as 'in' the committee (secure, and nick secure
    // taken into account).
    if (IsIn(nick) && Magick::instance().nickserv.IsStored(nick) && Magick::instance().nickserv.GetStored(nick)->IsOnline())
    {
	if_RLOCK ((lck_CommServ, lck_list, i_Name.UpperCase(), "setting.Secure"), !setting.Secure ||
		  Magick::instance().nickserv.GetLive(nick)->IsIdentified())
	{
	    RET(true);
	}
    }
    RET(false);
    ETCB();
}

bool Committee_t::IsHead(const mstring & nick) const
{
    BTCB();
    FT("Committee_t::IsHead", (nick));

    if_RLOCK ((lck_CommServ, lck_list, i_Name.UpperCase(), "i_Head"), !i_Head.empty())
    {
	if (Magick::instance().nickserv.IsStored(nick))
	{
	    mstring target = Magick::instance().nickserv.GetStored(nick)->Host();
	    if (target.empty())
		target = nick;

	    if (i_Head.IsSameAs(target, true))
	    {
		RET(true);
	    }
	}
    }
    else
    {
	if_RLOCK2 ((lck_CommServ, lck_list, i_Name.UpperCase(), "i_HeadCom"), !i_HeadCom.empty())
	{
	    if (Magick::instance().commserv.IsList(i_HeadCom) &&
		Magick::instance().commserv.GetList(i_HeadCom)->IsIn(nick))
	    {
		RET(true);
	    }
	}
	else
	{
	    bool retval = IsIn(nick);

	    RET(retval);
	}
    }
    RET(false);
    ETCB();
}

void Committee_t::Description(const mstring & in)
{
    BTCB();
    FT("Committee_t::Description", (in));
    WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_Description"));
    MCB(i_Description);
    i_Description = in;
    MCE(i_Description);
    ETCB();
}

mstring Committee_t::Description() const
{
    BTCB();
    NFT("Committee_t::Description");
    RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_Description"));
    RET(i_Description);
    ETCB();
}

void Committee_t::Email(const mstring & in)
{
    BTCB();
    FT("Committee_t::Email", (in));
    WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_Email"));
    MCB(i_Email);
    i_Email = in;
    MCE(i_Email);
    ETCB();
}

mstring Committee_t::Email() const
{
    BTCB();
    NFT("Committee_t::Email");
    RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_Email"));
    RET(i_Email);
    ETCB();
}

void Committee_t::URL(const mstring & in)
{
    BTCB();
    FT("Committee_t::URL", (in));
    WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_URL"));
    MCB(i_URL);
    i_URL = in;
    MCE(i_URL);
    ETCB();
}

mstring Committee_t::URL() const
{
    BTCB();
    NFT("Committee_t::URL");
    RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "i_URL"));
    RET(i_URL);
    ETCB();
}

void Committee_t::Private(const bool in)
{
    BTCB();
    FT("Committee_t::Private", (in));
    if (!L_Private())
    {
	WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "setting.Private"));
	MCB(setting.Private);
	setting.Private = in;
	MCE(setting.Private);
    }
    ETCB();
}

bool Committee_t::Private() const
{
    BTCB();
    NFT("Committee_t::Private");
    if (!Magick::instance().commserv.LCK_Private())
    {
	RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "setting.Private"));
	RET(setting.Private);
    }
    RET(Magick::instance().commserv.DEF_Private());
    ETCB();
}

void Committee_t::L_Private(const bool in)
{
    BTCB();
    FT("Committee_t::L_Private", (in));
    if (!Magick::instance().commserv.LCK_Private())
    {
	WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "lock.Private"));
	MCB(lock.Private);
	lock.Private = in;
	MCE(lock.Private);
    }
    ETCB();
}

bool Committee_t::L_Private() const
{
    BTCB();
    NFT("Committee_t::L_Private");
    if (!Magick::instance().commserv.LCK_Private())
    {
	RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "lock.Private"));
	RET(lock.Private);
    }
    RET(true);
    ETCB();
}

void Committee_t::Secure(const bool in)
{
    BTCB();
    FT("Committee_t::Secure", (in));
    if (!L_Secure())
    {
	WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "setting.Secure"));
	MCB(setting.Secure);
	setting.Secure = in;
	MCE(setting.Secure);
    }
    ETCB();
}

bool Committee_t::Secure() const
{
    BTCB();
    NFT("Committee_t::Secure");
    if (!Magick::instance().commserv.LCK_Secure())
    {
	RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "setting.Secure"));
	RET(setting.Secure);
    }
    RET(Magick::instance().commserv.DEF_Secure());
    ETCB();
}

void Committee_t::L_Secure(const bool in)
{
    BTCB();
    FT("Committee_t::L_Secure", (in));
    if (!Magick::instance().commserv.LCK_Secure())
    {
	WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "lock.Secure"));
	MCB(lock.Secure);
	lock.Secure = in;
	MCE(lock.Secure);
    }
    ETCB();
}

bool Committee_t::L_Secure() const
{
    BTCB();
    NFT("Committee_t::L_Secure");
    if (!Magick::instance().commserv.LCK_Secure())
    {
	RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "lock.Secure"));
	RET(lock.Secure);
    }
    RET(true);
    ETCB();
}

void Committee_t::OpenMemos(const bool in)
{
    BTCB();
    FT("Committee_t::OpenMemos", (in));
    if (!L_OpenMemos())
    {
	WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "setting.OpenMemos"));
	MCB(setting.OpenMemos);
	setting.OpenMemos = in;
	MCE(setting.OpenMemos);
    }
    ETCB();
}

bool Committee_t::OpenMemos() const
{
    BTCB();
    NFT("Committee_t::OpenMemos");
    if (!Magick::instance().commserv.LCK_OpenMemos())
    {
	RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "setting.OpenMemos"));
	RET(setting.OpenMemos);
    }
    RET(Magick::instance().commserv.DEF_OpenMemos());
    ETCB();
}

void Committee_t::L_OpenMemos(const bool in)
{
    BTCB();
    FT("Committee_t::L_OpenMemos", (in));
    if (!Magick::instance().commserv.LCK_OpenMemos())
    {
	WLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "lock.OpenMemos"));
	MCB(lock.OpenMemos);
	lock.OpenMemos = in;
	MCE(lock.OpenMemos);
    }
    ETCB();
}

bool Committee_t::L_OpenMemos() const
{
    BTCB();
    NFT("Committee_t::L_OpenMemos");
    if (!Magick::instance().commserv.LCK_OpenMemos())
    {
	RLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "lock.OpenMemos"));
	RET(lock.OpenMemos);
    }
    RET(true);
    ETCB();
}

bool Committee_t::MSG_insert(const mstring & entry, const mstring & nick, const mDateTime & addtime)
{
    BTCB();
    FT("Committee_t::MSG_insert", (entry, nick, addtime));

    MLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "message"));
    if (IsHead(nick))
    {
	MCB(i_Messages.size());
	i_Messages.push_back(entlist_t(entry, nick, addtime));
	MCE(i_Messages.size());
	message = i_Messages.end();
	message--;
	RET(true);
    }
    else
    {
	message = i_Messages.end();
	RET(false);
    }
    ETCB();
}

bool Committee_t::MSG_erase()
{
    BTCB();
    NFT("Committee_t::MSG_erase");

    MLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "message"));
    if (message != i_Messages.end())
    {
	MCB(i_Messages.size());
	i_Messages.erase(message);
	MCE(i_Messages.size());
	message = i_Messages.end();
	RET(true);
    }
    else
    {
	RET(false);
    }
    ETCB();
}

bool Committee_t::MSG_find(const int number)
{
    BTCB();
    FT("Committee_t::MSG_find", (number));

    MLOCK((lck_CommServ, lck_list, i_Name.UpperCase(), "message"));
    entlist_i iter = i_Messages.end();
    int i;

    if (!i_Messages.empty())
    {
	// FIND exact nickname
	for (iter = i_Messages.begin(), i = 1; iter != i_Messages.end() && i != number; iter++, i++);
    }

    if (iter != i_Messages.end())
    {
	message = iter;
	RET(true);
    }
    else
    {
	message = i_Messages.end();
	RET(false);
    }
    ETCB();
}

size_t Committee_t::Usage() const
{
    BTCB();
    size_t retval = 0;

    WLOCK((lck_CommServ, lck_list, i_Name.UpperCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_RegTime.Internal());
    retval += i_HeadCom.capacity();
    retval += i_Head.capacity();
    retval += i_Description.capacity();
    retval += i_Email.capacity();
    retval += i_URL.capacity();

    entlist_cui i;

    for (i = i_Members.begin(); i != i_Members.end(); i++)
    {
	retval += i->Usage();
    }

    retval += sizeof(setting);
    retval += sizeof(lock);

    entlist_ci j;

    for (j = i_Messages.begin(); j != i_Messages.end(); j++)
    {
	retval += j->Usage();
    }

    map < mstring, mstring >::const_iterator l;
    for (l = i_UserDef.begin(); l != i_UserDef.end(); l++)
    {
	retval += l->first.capacity();
	retval += l->second.capacity();
    }

    return retval;
    ETCB();
}

void Committee_t::DumpB() const
{
    BTCB();
    MB(0,
       (i_Name, i_RegTime, i_HeadCom, i_Head, i_Description, i_Email, i_URL, i_Members.size(), setting.Private, lock.Private,
	setting.OpenMemos, lock.OpenMemos, setting.Secure, lock.Secure, i_Messages.size(), i_UserDef.size()));
    ETCB();
}

void Committee_t::DumpE() const
{
    BTCB();
    ME(0,
       (i_Name, i_RegTime, i_HeadCom, i_Head, i_Description, i_Email, i_URL, i_Members.size(), setting.Private, lock.Private,
	setting.OpenMemos, lock.OpenMemos, setting.Secure, lock.Secure, i_Messages.size(), i_UserDef.size()));
    ETCB();
}

CommServ::CommServ()
{
    BTCB();
    NFT("CommServ::CommServ");
    messages = true;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void CommServ::AddList(Committee_t * in) throw (E_CommServ_List)
#else
void CommServ::AddList(Committee_t * in)
#endif
{
    BTCB();
    FT("CommServ::AddList", ("(Committee_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_CommServ_List(E_CommServ_List::W_Add, E_CommServ_List::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Comm", "List", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_CommServ_List(E_CommServ_List::W_Add, E_CommServ_List::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Comm", "List", "Add", "Blank"));
	return;
#endif
    }

    if (in->doDelete())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_CommServ_List(E_CommServ_List::W_Add, E_CommServ_List::T_NotFound));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Comm", "List", "Add", "NotFound"));
	return;
#endif
    }

    WLOCK((lck_CommServ, lck_list));
    map_entry < Committee_t > old_entry(i_list, in->Name().LowerCase());
    if (old_entry != NULL)
    {
	old_entry->setDelete();
	i_list.erase(in->Name().LowerCase());
    }
    i_list[in->Name().UpperCase()] = in;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
map_entry < Committee_t > CommServ::GetList(const mstring & in) const throw (E_CommServ_List)
#else
map_entry < Committee_t > CommServ::GetList(const mstring & in) const
#endif
{
    BTCB();
    FT("CommServ::GetList", (in));

    RLOCK((lck_CommServ, lck_list, in.UpperCase()));
    CommServ::list_t::const_iterator iter = i_list.find(in.UpperCase());
    if (iter == i_list.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_CommServ_List(E_CommServ_List::W_Get, E_CommServ_List::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Comm", "List", "Get", "NotFound", in));
	NRET(Committee_t &, GLOB_Committee_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_CommServ_List(E_CommServ_List::W_Get, E_CommServ_List::T_Invalid, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Comm", "List", "Get", "Invalid", in));
	NRET(Committee_t &, GLOB_Committee_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_CommServ_List(E_CommServ_List::W_Get, E_CommServ_List::T_Blank, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Comm", "List", "Get", "Blank", in));
	NRET(Committee_t &, GLOB_Committee_t);
#endif
    }

    NRET(map_entry < Committee_t >, map_entry < Committee_t > (iter->second));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void CommServ::RemList(const mstring & in) throw (E_CommServ_List)
#else
void CommServ::RemList(const mstring & in)
#endif
{
    BTCB();
    FT("CommServ::RemList", (in));

    WLOCK((lck_CommServ, lck_list));
    CommServ::list_t::iterator iter = i_list.find(in.UpperCase());
    if (iter == i_list.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_CommServ_List(E_CommServ_List::W_Rem, E_CommServ_List::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Comm", "List", "Rem", "NotFound", in));
	return;
#endif
    }

    if (iter->second != NULL)
    {
	map_entry < Committee_t > entry(iter->second);
	entry->setDelete();
    }
    i_list.erase(iter);
    ETCB();
}

bool CommServ::IsList(const mstring & in) const
{
    BTCB();
    FT("CommServ::IsList", (in));
    RLOCK((lck_CommServ, lck_list));
    bool retval = (i_list.find(in.UpperCase()) != i_list.end());

    RET(retval);
    ETCB();
}

void CommServ::AddCommands()
{
    BTCB();
    NFT("CommServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "MEMB* ADD", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_member_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "MEMB* DEL*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_member_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "MEMB* ERA*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_member_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "MEMB* LIST", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_member_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "MEMB* VIEW", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_member_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "MEMB* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOG* ADD", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_logon_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOG* DEL*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_logon_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOG* ERA*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_logon_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOG* LIST", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_logon_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOG* VIEW", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_logon_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOG* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* HEAD*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_set_Head);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* DESC*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_set_Description);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* E*MAIL*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_set_Email);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* URL", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* WWW*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* WEB*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* SEC*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_set_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PRIV*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_set_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *MEMO*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_set_OpenMemos);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK SEC*", Magick::instance().commserv.SOP_Name(),
						 CommServ::do_lock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK PRIV*", Magick::instance().commserv.SOP_Name(),
						 CommServ::do_lock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK *MEMO*", Magick::instance().commserv.SOP_Name(),
						 CommServ::do_lock_OpenMemos);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK H*LP", Magick::instance().commserv.SOP_Name(),
						 do_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK SEC*", Magick::instance().commserv.SOP_Name(),
						 CommServ::do_unlock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK PRIV*", Magick::instance().commserv.SOP_Name(),
						 CommServ::do_unlock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK *MEMO*", Magick::instance().commserv.SOP_Name(),
						 CommServ::do_unlock_OpenMemos);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK H*LP", Magick::instance().commserv.SOP_Name(),
						 do_3param);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name(),
						 CommServ::do_Help);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ADD*", Magick::instance().commserv.SOP_Name(),
						 CommServ::do_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "DEL*", Magick::instance().commserv.SOP_Name(),
						 CommServ::do_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ERA*", Magick::instance().commserv.SOP_Name(),
						 CommServ::do_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LIST", Magick::instance().commserv.ALL_Name(),
						 CommServ::do_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "*MEMO*", Magick::instance().commserv.REGD_Name(),
						 CommServ::do_Memo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "INFO", Magick::instance().commserv.ALL_Name(),
						 CommServ::do_Info);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "MEMB* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "MEMB*", Magick::instance().commserv.REGD_Name(),
						 do_1_3param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOG* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOG*", Magick::instance().commserv.REGD_Name(),
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

void CommServ::RemCommands()
{
    BTCB();
    NFT("CommServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "MEMB* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "MEMB* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "MEMB* ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "MEMB* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "MEMB* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "MEMB* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOG* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOG* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOG* ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOG* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOG* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOG* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* HEAD*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* DESC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* E*MAIL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* URL", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* WWW*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* WEB*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* SEC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PRIV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *MEMO*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK *MEMO*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK H*LP", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK *MEMO*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK H*LP", Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ADD*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "DEL*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ERA*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LIST", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "*MEMO*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "INFO", Magick::instance().commserv.ALL_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "MEMB* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "MEMB*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOG* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOG*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK *", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK *", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK", Magick::instance().commserv.SOP_Name());
    ETCB();
}

void CommServ::execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    mThread::ReAttach(tt_OtherServ);
    FT("CommServ::execute", (source, msgtype, params));
    //okay this is the main commserv command switcher

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

void CommServ::do_Help(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_Help", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    mstring HelpTopic = Magick::instance().commserv.GetInternalName();

    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector < mstring > help = Magick::instance().getHelp(source, HelpTopic.UpperCase());

    unsigned int i;

    for (i = 0; i < help.size(); i++)
	::send(mynick, source, help[i]);
    ETCB();
}

void CommServ::do_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_Add", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring head = params.ExtractWord(3, " ");
    mstring desc = params.After(" ", 3);

    if (Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISSTORED", (committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    map_entry < Committee_t > tmp;
    if (head[0u] == '@')
    {
	head = head.After('@').UpperCase();

	if (!Magick::instance().commserv.IsList(head))
	{
	    SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	    return;
	}

	tmp = new Committee_t(committee, *(Magick::instance().commserv.GetList(head)), desc);
    }
    else
    {

	if (!Magick::instance().nickserv.IsStored(head))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (head));
	    return;
	}

	head = Magick::instance().getSname(head);
	if (Magick::instance().nickserv.GetStored(head)->Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (head));
	    return;
	}

	tmp = new Committee_t(committee, head, desc);
    }

    Magick::instance().commserv.AddList(tmp);
    Magick::instance().commserv.stats.i_Add++;
    SEND(mynick, source, "COMMSERV/ADD", (committee, head));
    LOG(LM_NOTICE, "COMMSERV/ADD", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), committee, head));
    ETCB();
}

void CommServ::do_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_Del", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    unsigned long dropped = Magick::instance().commserv.GetList(committee)->Drop();

    Magick::instance().commserv.RemList(committee);
    Magick::instance().commserv.stats.i_Del++;
    SEND(mynick, source, "COMMSERV/DEL", (committee));
    LOG(LM_NOTICE, "COMMSERV/DEL",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), committee, dropped - 1));
    ETCB();
}

void CommServ::do_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_List", (mynick, source, params));

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

    SEND(mynick, source, "LIST/COMM_LIST", (mask));
    CommServ::list_t::iterator iter;

    {
	RLOCK((lck_CommServ, lck_list));
	for (iter = Magick::instance().commserv.ListBegin(), i = 0, count = 0; iter != Magick::instance().commserv.ListEnd();
	     iter++)
	{
	    map_entry < Committee_t > comm(iter->second);
	    if (comm->Name().Matches(mask, true))
	    {
		if (i < listsize &&
		    (!comm->Private() ||
		     (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
		      Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))))
		{
		    ::send(mynick, source, comm->Name() + " (" + comm->size() + "): " + comm->Description());
		    i++;
		}
		count++;
	    }
	}
    }
    SEND(mynick, source, "LIST/DISPLAYED", (i, count));
    ETCB();
}

void CommServ::do_Memo(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_Memo", (mynick, source, params));

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

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring text = params.After(" ", 2);

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONDYNAMIC", (message));
	return;
    }

    {
	map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);
	if (!comm->IsOn(source))
	{
	    SEND(mynick, source, "COMMSERV/NOTMEMBER", (committee));
	    return;
	}

	if (!comm->OpenMemos() && !comm->IsHead(source))
	{
	    SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	    return;
	}
    }

    text.prepend("[" + IRC_Bold + committee + IRC_Off + "] ");
    CommServ::do_Memo2(source, committee, text);
    Magick::instance().commserv.stats.i_Memo++;
    SEND(mynick, source, "COMMSERV/MEMO", (committee));
    LOG(LM_INFO, "COMMSERV/MEMO", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), committee));
    ETCB();
}

void CommServ::do_Memo2(const mstring & source, const mstring & committee, const mstring & text)
{
    BTCB();
    FT("CommServ::do_Memo2", (source, committee, text));

    if (!Magick::instance().commserv.IsList(committee))
	return;
    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    mstring realme;

    if (Magick::instance().nickserv.IsStored(source))
	realme = Magick::instance().nickserv.GetStored(source)->Host();
    else
	return;
    if (realme.empty())
	realme = source;

    if (!comm->HeadCom().empty())
    {
	if (Magick::instance().commserv.IsList(comm->HeadCom()))
	{
	    CommServ::do_Memo2(source, comm->HeadCom(), text);
	}
    }
    else if (!comm->Head().empty())
    {
	if (Magick::instance().nickserv.IsStored(comm->Head()))
	{
	    mstring realrecipiant = Magick::instance().nickserv.GetStored(comm->Head())->Host();

	    if (realrecipiant.empty())
		realrecipiant = comm->Head();
	    if (!realme.IsSameAs(realrecipiant, true))
	    {
		Memo_t tmp(realrecipiant, source, text);

		Magick::instance().memoserv.AddNickMemo(&tmp);

		RLOCK2((lck_MemoServ, lck_nick, realrecipiant.LowerCase()));
		MemoServ::nick_memo_t & memolist = Magick::instance().memoserv.GetNick(realrecipiant);
		map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(realrecipiant);
		if (nick->IsOnline())
		    SEND(Magick::instance().memoserv.FirstName(), nick->Name(), "MS_COMMAND/NS_NEW",
			 (memolist.size(), Magick::instance().memoserv.FirstName(), memolist.size()));
		unsigned int i;

		for (i = 0; i < nick->Siblings(); i++)
		{
		    if (Magick::instance().nickserv.IsStored(nick->Sibling(i)) &&
			Magick::instance().nickserv.GetStored(nick->Sibling(i))->IsOnline())
		    {
			SEND(Magick::instance().memoserv.FirstName(), nick->Sibling(i), "MS_COMMAND/NS_NEW",
			     (memolist.size(), Magick::instance().memoserv.FirstName(), memolist.size()));
		    }
		}
	    }
	}
    }

    MLOCK((lck_CommServ, lck_list, comm->Name().UpperCase(), "member"));
    for (comm->member = comm->begin(); comm->member != comm->end(); comm->member++)
    {
	if (Magick::instance().nickserv.IsStored(comm->member->Entry()))
	{
	    mstring realrecipiant = Magick::instance().nickserv.GetStored(comm->member->Entry())->Host();

	    if (realrecipiant.empty())
		realrecipiant = comm->member->Entry();
	    if (!realme.IsSameAs(realrecipiant, true))
	    {
		Memo_t tmp(realrecipiant, source, text);

		Magick::instance().memoserv.AddNickMemo(&tmp);

		RLOCK2((lck_MemoServ, lck_nick, realrecipiant.LowerCase()));
		MemoServ::nick_memo_t & memolist = Magick::instance().memoserv.GetNick(realrecipiant);
		map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(realrecipiant);
		if (nick->IsOnline())
		    SEND(Magick::instance().memoserv.FirstName(), nick->Name(), "MS_COMMAND/NS_NEW",
			 (memolist.size(), Magick::instance().memoserv.FirstName(), memolist.size()));
		unsigned int i;

		for (i = 0; i < nick->Siblings(); i++)
		{
		    if (Magick::instance().nickserv.IsStored(nick->Sibling(i)) &&
			Magick::instance().nickserv.GetStored(nick->Sibling(i))->IsOnline())
		    {
			SEND(Magick::instance().memoserv.FirstName(), nick->Sibling(i), "MS_COMMAND/NS_NEW",
			     (memolist.size(), Magick::instance().memoserv.FirstName(), memolist.size()));
		    }
		}
	    }
	}
    }
    ETCB();
}

void CommServ::do_Info(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_Info", (mynick, source, params));

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

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONDYNAMIC", (message));
	return;
    }

    mstring output;

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);
    SEND(mynick, source, "COMMSERV_INFO/DESCRIPTION", (committee, comm->Description()));
    SEND(mynick, source, "COMMSERV_INFO/REGISTERED", (comm->RegTime().Ago()));
    if (!comm->HeadCom().empty())
    {
	SEND(mynick, source, "COMMSERV_INFO/HEADCOM", (comm->HeadCom().UpperCase()));
    }
    else if (!comm->Head().empty())
    {
	SEND(mynick, source, "COMMSERV_INFO/HEAD", (Magick::instance().getSname(comm->Head())));
    }

    if (!comm->Email().empty())
    {
	SEND(mynick, source, "COMMSERV_INFO/EMAIL", (comm->Email()));
    }

    if (!comm->URL().empty())
    {
	SEND(mynick, source, "COMMSERV_INFO/URL", (comm->URL()));
    }

    output.erase();
    if (comm->size())
    {
	if (output.size())
	    output << ", ";
	output << comm->size() << " " << Magick::instance().getMessage(source, "COMMSERV_INFO/MEMBERS");
    }
    if (comm->MSG_size())
    {
	if (output.size())
	    output << ", ";
	output << comm->MSG_size() << " " << Magick::instance().getMessage(source, "COMMSERV_INFO/MESSAGES");
    }
    if (output.size())
	SEND(mynick, source, "COMMSERV_INFO/STATS", (output));

    output.erase();
    if (comm->Secure())
    {
	if (output.size())
	    output << ", ";
	if (comm->L_Secure())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE");
	if (comm->L_Secure())
	    output << IRC_Off;
    }
    if (comm->Private())
    {
	if (output.size())
	    output << ", ";
	if (comm->L_Private())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE");
	if (comm->L_Private())
	    output << IRC_Off;
    }
    if (comm->OpenMemos())
    {
	if (output.size())
	    output << ", ";
	if (comm->L_OpenMemos())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS");
	if (comm->L_OpenMemos())
	    output << IRC_Off;
    }
    if (output.size())
	SEND(mynick, source, "COMMSERV_INFO/OPTIONS", (output));
    {
	RLOCK2((lck_Events));
	if (Magick::instance().servmsg.ShowSync() && Magick::instance().events != NULL)
	    SEND(mynick, source, "MISC/SYNC", (Magick::instance().events->SyncTime(source)));
    }
    ETCB();
}

void CommServ::do_member_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_member_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring member = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.ALL_Name() ||
	committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(member))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (member));
	return;
    }

    member = Magick::instance().getSname(member);
    if (Magick::instance().nickserv.GetStored(member)->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (member));
	return;
    }

    {
	RLOCK((lck_CommServ, lck_list, committee.UpperCase()));
	map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);
	committee = comm->Name();

	if (!comm->IsHead(source))
	{
	    SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	    return;
	}

	if (comm->IsIn(member))
	{
	    SEND(mynick, source, "LIST/EXISTS2", (member, committee, Magick::instance().getMessage(source, "LIST/MEMBER")));
	    return;
	}

	comm->insert(member, source);
    }
    Magick::instance().commserv.stats.i_Member++;
    SEND(mynick, source, "LIST/ADD2", (member, committee, Magick::instance().getMessage(source, "LIST/MEMBER")));
    LOG(LM_DEBUG, "COMMSERV/MEMBER_ADD",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), member, committee));
    ETCB();
}

void CommServ::do_member_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_member_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring member = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee.IsSameAs(Magick::instance().commserv.SADMIN_Name(), true) ||
	committee.IsSameAs(Magick::instance().commserv.ALL_Name(), true) ||
	committee.IsSameAs(Magick::instance().commserv.REGD_Name(), true))
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);
    committee = comm->Name();

    if (!comm->IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    if (comm->IsHead(member))
    {
	if (comm->HeadCom().empty())
	    SEND(mynick, source, "COMMSERV/OTH_HEAD", (member, committee));
	else
	    SEND(mynick, source, "COMMSERV/OTH_MEMBER", (member, comm->HeadCom()));
	return;
    }

    MLOCK((lck_CommServ, lck_list, committee.UpperCase(), "member"));
    if (comm->find(member))
    {
	Magick::instance().commserv.stats.i_Member++;
	SEND(mynick, source, "LIST/DEL2",
	     (comm->member->Entry(), committee, Magick::instance().getMessage(source, "LIST/MEMBER")));
	LOG(LM_DEBUG, "COMMSERV/MEMBER_DEL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), comm->member->Entry(), committee));
	comm->erase();
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS2", (member, committee, Magick::instance().getMessage(source, "LIST/MEMBER")));
    }
    ETCB();
}

void CommServ::do_member_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_member_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

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

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee.IsSameAs(Magick::instance().commserv.ALL_Name(), true) ||
	committee.IsSameAs(Magick::instance().commserv.REGD_Name(), true))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONDYNAMIC", (message));
	return;
    }

    {
	map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);
	if (comm->Private() &&
	    !(comm->IsOn(source) ||
	      (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	       Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))))
	{
	    SEND(mynick, source, "COMMSERV/NOTMEMBER", (committee));
	    return;
	}
    }

    SEND(mynick, source, "LIST/DISPLAY2", (committee, Magick::instance().getMessage(source, "LIST/MEMBER")));
    CommServ::do_member_List2(mynick, source, committee, true, 1);
    ETCB();
}

int CommServ::do_member_List2(const mstring & mynick, const mstring & source, const mstring & committee, const bool first,
			      const int number)
{
    BTCB();
    FT("CommServ::do_member_List2", (mynick, source, committee, first, number));

    int nextnum = number;
    mstring output;

    if (!Magick::instance().commserv.IsList(committee))
    {
	RET(0);
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    if (!comm->HeadCom().empty())
    {
	if (Magick::instance().commserv.IsList(comm->HeadCom()))
	{
	    nextnum += CommServ::do_member_List2(mynick, source, comm->HeadCom(), false, nextnum);
	}
    }
    else if (!comm->Head().empty())
    {
	output.erase();
	output << nextnum++ << ". " << IRC_Bold;
	if (Magick::instance().nickserv.IsStored(comm->Head()))
	    output << Magick::instance().getSname(comm->Head());
	else
	    output << Magick::instance().getMessage(source, "COMMSERV/DEFUNCT") << " " << Magick::instance().getMessage(source,
															"COMMSERV_INFO/SET_HEAD");
	output << IRC_Off;
	if (!Magick::instance().nickserv.IsStored(comm->Head()))
	    output << " [" << Magick::instance().getMessage(source, "COMMSERV/DEFUNCT") << "]";
	::send(mynick, source, output);
    }

    MLOCK((lck_CommServ, lck_list, comm->Name().UpperCase(), "member"));
    for (comm->member = comm->begin(); comm->member != comm->end(); comm->member++)
    {
	output.erase();
	output << nextnum++ << ". " << (first ? Blank : IRC_Bold) << comm->member->Entry() << (first ? Blank : IRC_Off);
	if (!Magick::instance().nickserv.IsStored(comm->member->Entry()))
	    output << " [" << Magick::instance().getMessage(source, "COMMSERV/DEFUNCT") << "]";
	::send(mynick, source, output);
	::send(mynick, source,
	       "    " + parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
				     mVarArray(ToHumanTime(comm->member->Last_Modify_Time().SecondsSince(), source),
					       comm->member->Last_Modifier())));
    }
    RET(nextnum - number);
    ETCB();
}

void CommServ::do_logon_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_logon_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring msgnum = params.After(" ", 3);

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    if (!comm->IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    if (comm->MSG_size() >= Magick::instance().commserv.Max_Logon())
    {
	SEND(mynick, source, "COMMSERV/MAX_MESSAGES", (committee));
	return;
    }

    comm->MSG_insert(msgnum, source);
    Magick::instance().commserv.stats.i_Logon++;
    SEND(mynick, source, "LIST/ADD2_NUMBER",
	 (comm->MSG_size(), committee, Magick::instance().getMessage(source, "LIST/MESSAGES")));
    LOG(LM_INFO, "COMMSERV/LOGON_ADD", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), committee));
    ETCB();
}

void CommServ::do_logon_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_logon_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring msgnum = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (!msgnum.IsNumber() || msgnum.Contains(".") || msgnum.Contains("-"))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    unsigned int num = atoi(msgnum.c_str());

    if (num <= 0 || num > comm->MSG_size())
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, comm->MSG_size()));
    }

    if (!comm->IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    MLOCK((lck_CommServ, lck_list, committee.UpperCase(), "message"));
    if (comm->MSG_find(num))
    {
	Magick::instance().commserv.stats.i_Logon++;
	SEND(mynick, source, "LIST/DEL2_NUMBER", (num, committee, Magick::instance().getMessage(source, "LIST/MESSAGES")));
	LOG(LM_INFO, "COMMSERV/LOGON_DEL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), committee));
	comm->MSG_erase();
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS2_NUMBER",
	     (num, committee, Magick::instance().getMessage(source, "LIST/MESSAGES")));
    }
    ETCB();
}

void CommServ::do_logon_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_logon_List", (mynick, source, params));

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

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    if (!comm->IsHead(source) &&
	!(Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source)))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    if (!comm->MSG_size())
    {
	SEND(mynick, source, "LIST/EMPTY2", (committee, Magick::instance().getMessage(source, "LIST/MESSAGES")));
	return;
    }

    int i;
    mstring output;

    SEND(mynick, source, "LIST/DISPLAY2", (committee, Magick::instance().getMessage(source, "LIST/MESSAGES")));
    MLOCK((lck_CommServ, lck_list, committee.UpperCase(), "message"));
    for (i = 1, comm->message = comm->MSG_begin(); comm->message != comm->MSG_end(); comm->message++, i++)
    {
	output.erase();
	output << i << ". " << comm->message->Entry();
	::send(mynick, source, output);
	::send(mynick, source,
	       "    " + parseMessage(Magick::instance().getMessage(source, "LIST/LASTMOD"),
				     mVarArray(ToHumanTime(comm->message->Last_Modify_Time().SecondsSince(), source),
					       comm->message->Last_Modifier())));
    }
    ETCB();
}

void CommServ::do_set_Head(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_set_Head", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring newhead = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    bool ovr = (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_Owner()) &&
		Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_Owner())->IsOn(source));

    if (!(comm->IsHead(source) || ovr))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    if (newhead[0u] == '@' && ovr)
    {
	newhead = newhead.After("@").UpperCase();

	if (!Magick::instance().commserv.IsList(newhead))
	{
	    SEND(mynick, source, "COMMSERV/ISNOTSTORED", (newhead));
	    return;
	}

	comm->HeadCom(newhead);
    }
    else
    {
	if (!Magick::instance().nickserv.IsStored(newhead))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (newhead));
	    return;
	}

	newhead = Magick::instance().getSname(newhead);
	if (Magick::instance().nickserv.GetStored(newhead)->Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (newhead));
	    return;
	}

	if (comm->Head().empty() && !ovr)
	{
	    SEND(mynick, source, "COMMSERV/MULTI_HEAD", (committee));
	    return;
	}

	if (newhead.IsSameAs(comm->Head(), true))
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (message));
	    return;
	}

	comm->Head(newhead);
    }

    Magick::instance().commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO",
	 (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_HEAD"), committee, newhead));
    LOG(LM_INFO, "COMMSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_HEAD"), committee, newhead));
    ETCB();
}

void CommServ::do_set_Description(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_set_Description", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring desc = params.After(" ", 3);

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    if (!comm->IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    comm->Description(desc);
    Magick::instance().commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO",
	 (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_DESCRIPTION"), committee, desc));
    LOG(LM_INFO, "COMMSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_DESCRIPTION"), committee, desc));
    ETCB();
}

void CommServ::do_set_Email(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_set_Email", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring email = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (email.IsSameAs("none", true))
	email.erase();
    else if (!email.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_EMAIL"), '@'));
	return;
    }
    else if (email.WordCount("@") != 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_EMAIL"), '@'));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    if (!comm->IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    comm->Email(email);
    Magick::instance().commserv.stats.i_Set++;
    if (email.empty())
    {
	SEND(mynick, source, "COMMSERV/UNSET", (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_EMAIL"), committee));
	LOG(LM_INFO, "COMMSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage(source, "COMMSERV_INFO/SET_EMAIL"), committee));
    }
    else
    {
	SEND(mynick, source, "COMMSERV/SET_TO",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_EMAIL"), committee, email));
	LOG(LM_INFO, "COMMSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("COMMSERV_INFO/SET_EMAIL"), committee, email));
    }
    ETCB();
}

void CommServ::do_set_URL(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_set_URL", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring url = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (url.IsSameAs("none", true))
	url.erase();

    if (url.SubString(0, 6).IsSameAs("http://", true))
    {
	url.erase(0, 6);
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    if (!comm->IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    comm->URL(url);
    Magick::instance().commserv.stats.i_Set++;
    if (url.empty())
    {
	SEND(mynick, source, "COMMSERV/UNSET", (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_URL"), committee));
	LOG(LM_INFO, "COMMSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage(source, "COMMSERV_INFO/SET_URL"), committee));
    }
    else
    {
	SEND(mynick, source, "COMMSERV/SET_TO",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_URL"), committee, "http://" + url));
	LOG(LM_INFO, "COMMSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("COMMSERV_INFO/SET_URL"), committee, "http://" + url));
    }
    ETCB();
}

void CommServ::do_set_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_set_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    if (!comm->IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    if (comm->L_Secure())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE"), committee));
	return;
    }

    comm->Secure(onoff.GetBool());
    Magick::instance().commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO",
	 (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE"), committee,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_SECURE"), committee,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void CommServ::do_set_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_set_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    if (!comm->IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    if (comm->L_Private())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE"), committee));
	return;
    }

    comm->Private(onoff.GetBool());
    Magick::instance().commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO",
	 (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE"), committee,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_PRIVATE"), committee,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void CommServ::do_set_OpenMemos(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_set_OpenMemos", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_OpenMemos())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    if (!comm->IsHead(source))
    {
	SEND(mynick, source, "COMMSERV/NOTHEAD", (committee));
	return;
    }

    if (comm->L_OpenMemos())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"), committee));
	return;
    }

    comm->OpenMemos(onoff.GetBool());
    Magick::instance().commserv.stats.i_Set++;
    SEND(mynick, source, "COMMSERV/SET_TO",
	 (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"), committee,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_OPENMEMOS"), committee,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void CommServ::do_lock_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_lock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    if (Magick::instance().commserv.LCK_Secure())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE"), committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    comm->L_Secure(false);
    comm->Secure(onoff.GetBool());
    comm->L_Secure(true);
    Magick::instance().commserv.stats.i_Lock++;
    SEND(mynick, source, "COMMSERV/LOCKED",
	 (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE"), committee,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/LOCKED",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_SECURE"), committee,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void CommServ::do_lock_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_lock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    if (Magick::instance().commserv.LCK_Private())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE"), committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    comm->L_Private(false);
    comm->Private(onoff.GetBool());
    comm->L_Private(true);
    Magick::instance().commserv.stats.i_Lock++;
    SEND(mynick, source, "COMMSERV/LOCKED",
	 (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE"), committee,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/LOCKED",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_PRIVATE"), committee,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void CommServ::do_lock_OpenMemos(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_lock_OpenMemos", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().commserv.DEF_OpenMemos())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    if (Magick::instance().commserv.LCK_OpenMemos())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"), committee));
	return;
    }

    map_entry < Committee_t > comm = Magick::instance().commserv.GetList(committee);

    comm->L_OpenMemos(false);
    comm->OpenMemos(onoff.GetBool());
    comm->L_OpenMemos(true);
    Magick::instance().commserv.stats.i_Lock++;
    SEND(mynick, source, "COMMSERV/LOCKED",
	 (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"), committee,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			     Magick::instance().getMessage(source, "VALS/OFF"))));
    LOG(LM_INFO, "COMMSERV/LOCKED",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_OPENMEMOS"), committee,
	 (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") :
			    Magick::instance().getMessage(source, "VALS/OFF"))));
    ETCB();
}

void CommServ::do_unlock_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_unlock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    if (Magick::instance().commserv.LCK_Secure())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE"), committee));
	return;
    }

    Magick::instance().commserv.GetList(committee)->L_Secure(false);
    Magick::instance().commserv.stats.i_Unlock++;
    SEND(mynick, source, "COMMSERV/UNLOCKED", (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_SECURE")));
    LOG(LM_INFO, "COMMSERV/UNLOCKED",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_SECURE"), committee));
    ETCB();
}

void CommServ::do_unlock_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_unlock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    if (Magick::instance().commserv.LCK_Private())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE"), committee));
	return;
    }

    Magick::instance().commserv.GetList(committee)->L_Private(false);
    Magick::instance().commserv.stats.i_Unlock++;
    SEND(mynick, source, "COMMSERV/UNLOCKED", (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_PRIVATE")));
    LOG(LM_INFO, "COMMSERV/UNLOCKED",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_PRIVATE"), committee));
    ETCB();
}

void CommServ::do_unlock_OpenMemos(const mstring & mynick, const mstring & source, const mstring & params)
{
    BTCB();
    FT("CommServ::do_unlock_OpenMemos", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " + params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring committee = params.ExtractWord(2, " ").UpperCase();

    if (!Magick::instance().commserv.IsList(committee))
    {
	SEND(mynick, source, "COMMSERV/ISNOTSTORED", (committee));
	return;
    }

    if (committee == Magick::instance().commserv.SADMIN_Name() || committee == Magick::instance().commserv.SOP_Name() ||
	committee == Magick::instance().commserv.ADMIN_Name() || committee == Magick::instance().commserv.OPER_Name() ||
	committee == Magick::instance().commserv.ALL_Name() || committee == Magick::instance().commserv.REGD_Name())
    {
	SEND(mynick, source, "COMMSERV/NOTMODIFY", (committee));
	return;
    }

    if (Magick::instance().commserv.LCK_OpenMemos())
    {
	SEND(mynick, source, "COMMSERV/ISLOCKED",
	     (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS"), committee));
	return;
    }

    Magick::instance().commserv.GetList(committee)->L_OpenMemos(false);
    Magick::instance().commserv.stats.i_Unlock++;
    SEND(mynick, source, "COMMSERV/UNLOCKED", (Magick::instance().getMessage(source, "COMMSERV_INFO/SET_OPENMEMOS")));
    LOG(LM_INFO, "COMMSERV/UNLOCKED",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("COMMSERV_INFO/SET_OPENMEMOS"), committee));
    ETCB();
}

SXP::Tag Committee_t::tag_Committee_t("Committee_t");
SXP::Tag Committee_t::tag_Name("Name");
SXP::Tag Committee_t::tag_RegTime("RegTime");
SXP::Tag Committee_t::tag_HeadCom("HeadCom");
SXP::Tag Committee_t::tag_Head("Head");
SXP::Tag Committee_t::tag_Description("Description");
SXP::Tag Committee_t::tag_Email("EMail");
SXP::Tag Committee_t::tag_URL("URL");
SXP::Tag Committee_t::tag_set_OpenMemos("SET_OpenMemos");
SXP::Tag Committee_t::tag_set_Private("SET_Private");
SXP::Tag Committee_t::tag_set_Secure("SET_Secure");
SXP::Tag Committee_t::tag_lock_OpenMemos("LOCK_OpenMemos");
SXP::Tag Committee_t::tag_lock_Private("LOCK_Private");
SXP::Tag Committee_t::tag_lock_Secure("LOCK_Secure");
SXP::Tag Committee_t::tag_Members("Members");
SXP::Tag Committee_t::tag_Messages("Messages");
SXP::Tag Committee_t::tag_UserDef("UserDef");

void Committee_t::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    FT("Committee_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    if (!
	(i_Name == Magick::instance().commserv.SADMIN_Name() || i_Name == Magick::instance().commserv.ALL_Name() ||
	 i_Name == Magick::instance().commserv.REGD_Name()))
    {
	if (pElement->IsA(tag_Members))
	{
	    entlist_t *tmp = new entlist_t;

	    members_array.push_back(tmp);
	    pIn->ReadTo(tmp);
	}
    }

    if (pElement->IsA(tag_Messages))
    {
	entlist_t *tmp = new entlist_t;

	messages_array.push_back(tmp);
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

void Committee_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

    FT("Committee_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
    if (pElement->IsA(tag_Name))
    {
	pElement->Retrieve(i_Name);
	ref_class::lockData(mVarArray("CommnServ", lck_list, i_Name.UpperCase()));
    }
    if (pElement->IsA(tag_RegTime))
	pElement->Retrieve(i_RegTime);
    if (pElement->IsA(tag_HeadCom))
	pElement->Retrieve(i_HeadCom);
    if (pElement->IsA(tag_Head))
	pElement->Retrieve(i_Head);
    if (pElement->IsA(tag_Description))
	pElement->Retrieve(i_Description);
    if (pElement->IsA(tag_Email))
	pElement->Retrieve(i_Email);
    if (pElement->IsA(tag_URL))
	pElement->Retrieve(i_URL);
    if (pElement->IsA(tag_set_OpenMemos))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.OpenMemos = tmp;
    }
    if (pElement->IsA(tag_set_Private))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Private = tmp;
    }
    if (pElement->IsA(tag_set_Secure))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Secure = tmp;
    }
    if (pElement->IsA(tag_lock_OpenMemos))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.OpenMemos = tmp;
    }
    if (pElement->IsA(tag_lock_Private))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Private = tmp;
    }
    if (pElement->IsA(tag_lock_Secure))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Secure = tmp;
    }
    ETCB();
}

void Committee_t::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    FT("Committee_t::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    //TODO: Add your source code here
    pOut->BeginObject(tag_Committee_t);

    WLOCK((lck_CommServ, lck_list, i_Name.UpperCase()));
    pOut->WriteElement(tag_Name, i_Name);
    pOut->WriteElement(tag_RegTime, i_RegTime);
    pOut->WriteElement(tag_HeadCom, i_HeadCom);
    pOut->WriteElement(tag_Head, i_Head);
    pOut->WriteElement(tag_Description, i_Description);
    pOut->WriteElement(tag_Email, i_Email);
    pOut->WriteElement(tag_URL, i_URL);
    pOut->WriteElement(tag_set_OpenMemos, setting.OpenMemos);
    pOut->WriteElement(tag_set_Private, setting.Private);
    pOut->WriteElement(tag_set_Secure, setting.Secure);
    pOut->WriteElement(tag_lock_OpenMemos, lock.OpenMemos);
    pOut->WriteElement(tag_lock_Private, lock.Private);
    pOut->WriteElement(tag_lock_Secure, lock.Secure);

    if (!
	(i_Name == Magick::instance().commserv.ALL_Name() || i_Name == Magick::instance().commserv.REGD_Name() ||
	 i_Name == Magick::instance().commserv.SADMIN_Name()))
    {
	entlist_ui l;

	for (l = i_Members.begin(); l != i_Members.end(); l++)
	{
	    pOut->BeginObject(tag_Members);
	    pOut->WriteSubElement(const_cast < entlist_t * > (&(*l)));
	    pOut->EndObject(tag_Members);
	}
    }

    entlist_i k;

    for (k = i_Messages.begin(); k != i_Messages.end(); k++)
    {
	pOut->BeginObject(tag_Messages);
	pOut->WriteSubElement(&(*k));
	pOut->EndObject(tag_Messages);
    }

    map < mstring, mstring >::const_iterator iter;
    for (iter = i_UserDef.begin(); iter != i_UserDef.end(); iter++)
    {
	pOut->WriteElement(tag_UserDef, iter->first + "\n" + iter->second);
    }

    pOut->EndObject(tag_Committee_t);
    ETCB();
}

SXP::Tag CommServ::tag_CommServ("CommServ");

void CommServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    FT("CommServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Committee_t *c = new Committee_t;

    if (pElement->IsA(c->GetClassTag()))
    {
	c_array.push_back(c);
	pIn->ReadTo(c);
    }
    else
    {
	delete c;
    }
    ETCB();
}

void CommServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);
    static_cast < void > (pElement);

    FT("CommServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
    ETCB();
}

void CommServ::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    FT("CommServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_CommServ);

    CommServ::list_t::iterator iter;
    {
	RLOCK((lck_CommServ, lck_list));
	for (iter = ListBegin(); iter != ListEnd(); iter++)
	{
	    map_entry < Committee_t > comm(iter->second);
	    pOut->WriteSubElement(comm);
	}
    }

    pOut->EndObject(tag_CommServ);
    ETCB();
}

void CommServ::PostLoad()
{
    BTCB();
    NFT("CommServ::PostLoad");
    // Linkage, etc
    unsigned int i, j;

    for (i = 0; i < c_array.size(); i++)
    {
	if (c_array[i] != NULL)
	{
	    for (j = 0; j < c_array[i]->members_array.size(); j++)
	    {
		c_array[i]->i_Members.insert(*c_array[i]->members_array[j]);
		delete c_array[i]->members_array[j];
	    }
	    c_array[i]->members_array.clear();
	    for (j = 0; j < c_array[i]->messages_array.size(); j++)
	    {
		c_array[i]->i_Messages.push_back(*c_array[i]->messages_array[j]);
		delete c_array[i]->messages_array[j];
	    }
	    c_array[i]->messages_array.clear();
	    for (j = 0; j < c_array[i]->ud_array.size(); j++)
	    {
		if (c_array[i]->ud_array[j] != NULL)
		{
		    if (c_array[i]->ud_array[j]->Contains("\n"))
			c_array[i]->i_UserDef[c_array[i]->ud_array[j]->Before("\n")] = c_array[i]->ud_array[j]->After("\n");
		    delete c_array[i]->ud_array[j];
		}
	    }
	    c_array[i]->ud_array.clear();
	    if (!c_array[i]->Name().empty())
		AddList(c_array[i]);
	    // delete c_array[i];
	}
    }
    c_array.clear();

    CommServ::list_t::iterator iter;
    RLOCK((lck_CommServ, lck_list));
    for (iter = i_list.begin(); iter != i_list.end(); iter++)
    {
	map_entry < Committee_t > comm(iter->second);
	mstring uname = comm->Name().UpperCase();
	{
	    MLOCK((lck_CommServ, lck_list, uname, "member"));
	    for (comm->member = comm->begin(); comm->member != comm->end(); comm->member++)
	    {
		comm->member->PostLoad();
	    }
	}
	{
	    MLOCK((lck_CommServ, lck_list, uname, "message"));
	    for (comm->message = comm->MSG_begin(); comm->message != comm->MSG_end(); comm->message++)
	    {
		comm->message->PostLoad();
	    }
	}

	// We must ensure certain settings in pre-defined committees ...
	// So check what committee this is, and if its one of them, then
	// ensure the values we need in place are what we have.  This
	// saves us from data tampering, and also allows us to change
	// committee names on the fly and have the correct knock-on effect.
	// Also ensures magick.ini settings are correctly set.
	if (uname == Magick::instance().commserv.SADMIN_Name())
	{
	    comm->i_Head.erase();
	    comm->i_HeadCom.erase();
	    comm->i_Members.clear();
	    {
		MLOCK((lck_CommServ, lck_list, uname, "member"));
		for (j = 1; j <= Magick::instance().operserv.Services_Admin().WordCount(", "); j++)
		    comm->i_Members.insert(entlist_t(Magick::instance().operserv.Services_Admin().ExtractWord(j, ", "),
						     Magick::instance().operserv.FirstName()));
	    }
	    comm->Secure(SADMIN_Secure());
	    comm->Private(SADMIN_Private());
	    comm->OpenMemos(SADMIN_OpenMemos());
	}
	else if (uname == Magick::instance().commserv.SOP_Name())
	{
	    comm->i_Head.erase();
	    comm->i_HeadCom = Magick::instance().commserv.SADMIN_Name();
	    comm->Secure(SOP_Secure());
	    comm->Private(SOP_Private());
	    comm->OpenMemos(SOP_OpenMemos());
	}
	else if (uname == Magick::instance().commserv.ADMIN_Name())
	{
	    comm->i_Head.erase();
	    comm->i_HeadCom = Magick::instance().commserv.SADMIN_Name();
	    comm->Secure(ADMIN_Secure());
	    comm->Private(ADMIN_Private());
	    comm->OpenMemos(ADMIN_OpenMemos());
	}
	else if (uname == Magick::instance().commserv.OPER_Name())
	{
	    comm->i_Head.erase();
	    comm->i_HeadCom = Magick::instance().commserv.ADMIN_Name();
	    comm->Secure(OPER_Secure());
	    comm->Private(OPER_Private());
	    comm->OpenMemos(OPER_OpenMemos());
	}
	else if (uname == Magick::instance().commserv.ALL_Name())
	{
	    comm->i_Head.erase();
	    comm->i_HeadCom = Magick::instance().commserv.ADMIN_Name();
	    comm->i_Members.clear();
	    comm->Secure(false);
	    comm->Private(true);
	    comm->OpenMemos(false);
	}
	else if (uname == Magick::instance().commserv.REGD_Name())
	{
	    comm->i_Head.erase();
	    comm->i_HeadCom = Magick::instance().commserv.SOP_Name();
	    comm->i_Members.clear();
	    comm->Secure(false);
	    comm->Private(true);
	    comm->OpenMemos(false);
	}
    }
    ETCB();
}
