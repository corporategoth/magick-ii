#include "pch.h"
#ifdef _MSC_VER
#pragma hdrstop
#endif
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

#include "lockable.h"
#include "commserv.h"
#include "magick.h"

Committee::Committee(mstring name, mstring head, mstring description)
{
    FT("Committee::Committee", (name, head, description));
    i_Name = name;
    i_Head = head.LowerCase();
    i_HeadCom = NULL;
    i_Description = description;
    i_OpenMemos = true;
}


Committee::Committee(mstring name, Committee *head, mstring description)
{
    FT("Committee::Committee", (name, "(Committee *) head", description));
    i_Name = name;
    i_Head = "";
    i_HeadCom = head;
    i_Description = description;
    i_OpenMemos = true;
}


Committee::Committee(mstring name, mstring description)
{
    FT("Committee::Committee", (name, description));
    i_Name = name;
    i_Head = "";
    i_HeadCom = NULL;
    i_Description = description;
    i_OpenMemos = true;
}


void Committee::operator=(const Committee &in)
{
    FT("Committee::operator=", ("(const Committee &) in"));

    i_Name = in.i_Name;
    i_Head = in.i_Head;
    i_HeadCom = in.i_HeadCom;
    i_Description = in.i_Description;
    i_OpenMemos = in.i_OpenMemos;

    map<mstring, mstring>::const_iterator j;
    i_UserDef.clear();
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
}


bool Committee::insert(mstring entry, mstring nick)
{
    FT("Committee::insert", (entry, nick));

    entlist_ui iter;
    if (!i_Members.empty())
	for (iter=i_Members.begin(); iter!=i_Members.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;
    if (i_Members.empty() || iter == i_Members.end())
    {
	entlist_t tmp(entry, nick, true);
	member = i_Members.insert(i_Members.end(), tmp);
	RET(true);
    }
    else
    {
	RET(false);
    }
}


bool Committee::erase()
{
    NFT("Committee::erase");

    if (member != i_Members.end())
    {
	i_Members.erase(member);
	member = i_Members.end();
	RET(true);
    }
    else
    {
	RET(false);
    }
}


bool Committee::IsIn(mstring nick)
{
    FT("Committee::IsIn", (nick));

    entlist_ui iter;
    for (iter=i_Members.begin(); iter!=i_Members.end(); iter++)
    {
	if (nick.LowerCase() = iter->Entry().LowerCase())
	{
	    RET(true);
	}
    }
    RET(false);
}


bool Committee::IsHead(mstring nick)
{
    FT("Committee::IsHead", (nick));

    if (i_Head != "" && i_Head == nick.LowerCase())
    {
	RET(true);
    }
    else if (i_HeadCom != NULL && i_HeadCom->IsIn(nick))
    {
	RET(true);
    }
    else if (i_Head == "" && i_HeadCom == NULL)
    {
	RET(IsIn(nick));
    }
    RET(false);
}


bool Committee::MSG_insert(mstring entry, mstring nick)
{
    FT("Committee::MSG_insert", (entry, nick));

    if (IsHead(nick))
    {
	entlist_t tmp(entry, nick, true);
	message = i_Messages.insert(i_Messages.end(), tmp);
	RET(true);
    }
    else
    {
	RET(false);
    }
}


bool Committee::MSG_erase()
{
    NFT("Committee::MSG_erase");

    if (message != i_Messages.end())
    {
	i_Messages.erase(message);
	message = i_Messages.end();
	RET(true);
    }
    else
    {
	RET(false);
    }
}


bool CommServ::IsList(mstring in)
{
    FT("CommServ::IsList", (in));
    RET(list.find(in.LowerCase())!=list.end());
}

void CommServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_OtherServ);
    FT("CommServ::execute", (data));
    //okay this is the main nickserv command switcher


    // Nick/Server PRIVMSG/NOTICE mynick :message

    mstring source, msgtype, mynick, message;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ");
    mynick  = data.ExtractWord(3, ": ");
    message = data.After(":", 2);

    mThread::ReAttach(tt_mBase);

}

void CommServ::load_database(wxInputStream& in)
{
    int i;
    in>>i;
    Committee tmpcommitee;
    for(int j=0;j<i;j++)
    {
	in>>tmpcommitee;
	list[tmpcommitee.Name()]=tmpcommitee;
    }
}

void CommServ::save_database(wxOutputStream& out)
{
    out<<secure_oper;
    out<<list.size();
    for(map<mstring,Committee>::iterator i=list.begin();i!=list.end();i++)
	out<<i->second;
}

wxOutputStream &operator<<(wxOutputStream& out,Committee& in)
{
    out<<in.i_Name;
    if(in.i_HeadCom!=NULL)
	out<<in.i_HeadCom->i_Name;
    else
	out<<mstring("");
    out<<in.i_Head<<in.i_Description;

    out<<in.i_Members.size();
    for(in.member=in.i_Members.begin();in.member!=in.i_Members.end();in.member++)
	out<<(*in.member);

    out<<in.i_OpenMemos;

    out<<in.i_Messages.size();
    for(in.message=in.i_Messages.begin();in.message!=in.i_Messages.end();in.message++)
	out<<(*in.message);

    return out;
}
wxInputStream &operator>>(wxInputStream& in, Committee& out)
{
    set<entlist_t>::size_type locsize,i;
    entlist_t locent;
    // need to write lock out.

    in>>out.i_Name;
    in>>out.i_HeadCom->i_Name;
    in>>out.i_Head>>out.i_Description;

    in>>locsize;
    out.i_Members.clear();
    for(i=0;i<locsize;i++)
    {
	in>>locent;
	out.i_Members.insert(locent);
    }

    in>>out.i_OpenMemos;

    in>>locsize;
    out.i_Messages.clear();
    for(i=0;i<locsize;i++)
    {
	in>>locent;
	out.i_Messages.push_back(locent);
    }

    return in;
}
