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

#include "chanserv.h"
#include "lockable.h"
#include "magick.h"

ChanServ::ChanServ()
{
    NFT("ChanServ::ChanServ");
    messages=true;
    automation=true;
}

void ChanServ::execute(const mstring & data)
{
    mThread::Detach(tt_mBase);
    mThread::Attach(tt_ChanServ);
    FT("ChanServ::execute", (data));
    //okay this is the main chanserv command switcher

    mstring source, msgtype, mynick, message;
    source  = data.Before(" ");
    msgtype = data.After(" ").Before(" ");
    mynick  = data.After(" ").After(" ").Before(" ");
    message = data.After(":");


    mThread::Detach(tt_ChanServ);
    mThread::Attach(tt_mBase);
}

Chan_Live_t::Chan_Live_t(const Chan_Live_t& in)
{
    NFT("Chan_Live_t::Chan_Live_t");
    *this=in;
}

void Chan_Live_t::operator=(const Chan_Live_t &in)
{
    NFT("Chan_Live_t::operator=");
    bans.clear();
    set<mstring>::iterator i;
    for(i=in.bans.begin();i!=in.bans.end();i++)
	bans.insert(*i);
    i_Creation_Time=in.i_Creation_Time;
    i_Key=in.i_Key;
    i_Limit=in.i_Limit;
    i_Name=in.i_Name;
    i_Topic=in.i_Topic;
    i_Topic_Set_Time=in.i_Topic_Set_Time;
    i_Topic_Setter=in.i_Topic_Setter;
    i_UserDef.clear();
    map<mstring,mstring>::iterator j;
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
    modes=in.modes;
    users.clear();
    map<mstring, pair<bool, bool> >::iterator k;
    for(k=in.users.begin();k!=in.users.end();k++)
	users.insert(*k);
}

bool Chan_Live_t::operator==(const Chan_Live_t &in) const
{
    NFT("Chan_Live_t::operator==");
    RET(i_Name==in.i_Name);
}

bool Chan_Live_t::operator<(const Chan_Live_t &in) const
{
    NFT("Chan_Live_t::operator<");
    RET(i_Name<in.i_Name);
}

bool checkops(pair<mstring, pair<bool,bool> > &in)
{
    FT("checkops",(in.first,in.second.first,in.second.second));
    if(in.second.first==true)
    {
	RET(true);
    }
    else
    {
	RET(false);
    }
}

int Chan_Live_t::Ops()
{
    NFT("Chan_Live_t::Ops");
    RET(count_if(users.begin(),users.end(),checkops));
}

bool checkvoices(pair<mstring, pair<bool,bool> > &in)
{
    FT("checkvoices",(in.first,in.second.first,in.second.second));
    if(in.second.second==true)
    {
	RET(true);
    }
    else
    {
	RET(false);
    }
}

int Chan_Live_t::Voices()
{
    NFT("Chan_Live_t::Voices");
    RET(count_if(users.begin(),users.end(),checkvoices));
}

Chan_Stored_t::Chan_Stored_t(const Chan_Stored_t& in)
{
    NFT("Chan_Stored_t::Chan_Stored_t");
    *this=in;
}

void Chan_Stored_t::operator=(const Chan_Stored_t &in)
{
    NFT("Chan_Stored_t::operator=");
    i_Name=in.i_Name;
    i_RegTime=in.i_RegTime;
    i_Description=in.i_Description;
    i_Password=in.i_Password;
    i_URL=in.i_URL;

    i_Mlock_On=in.i_Mlock_On;
    i_Mlock_Off=in.i_Mlock_Off;
    i_Mlock_Key=in.i_Mlock_Key;
    i_Mlock_Limit=in.i_Mlock_Limit;

    list<userlist_t>::iterator j;
    i_Access_Level.clear();
    for(j=in.i_Access_Level.begin();j!=in.i_Access_Level.end();j++)
	i_Access_Level.push_back(*j);
    i_Access.clear();
    for(j=in.i_Access.begin();j!=in.i_Access.end();j++)
	i_Access.push_back(*j);
    i_Akick.clear();
    for(j=in.i_Akick.begin();j!=in.i_Akick.end();j++)
	i_Akick.push_back(*j);
    i_Greet.clear();
    for(j=in.i_Greet.begin();j!=in.i_Greet.end();j++)
	i_Greet.push_back(*j);

    i_UserDef.clear();
    map<mstring, mstring>::iterator i;
    for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
    i_UserDef.insert(*i);

}

bool Chan_Stored_t::operator==(const Chan_Stored_t &in) const
{
    NFT("Chan_Stored_t::operator==");
    RET(i_Name==in.i_Name);
}

bool Chan_Stored_t::operator<(const Chan_Stored_t &in) const
{
    NFT("Chan_Stored_t::operator<");
    RET(i_Name<in.i_Name);
}

userlist_t::userlist_t(const userlist_t& in)
{
    NFT("userlist_t::userlist_t");
    *this=in;
}

void userlist_t::operator=(const userlist_t &in)
{
    NFT("userlist_t::operator=");
    i_Entry=in.i_Entry;
    i_Last_Modify_Time=in.i_Last_Modify_Time;
    i_Last_Modifier=in.i_Last_Modifier;
    map<mstring,mstring>::iterator i;
    i_UserDef.clear();
    for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
	i_UserDef.insert(*i);
}

bool userlist_t::operator==(const userlist_t &in) const
{
    NFT("userlist_t::operator==");
    RET(i_Entry==in.i_Entry&&i_Last_Modify_Time==in.i_Last_Modify_Time&&
	i_Last_Modifier==in.i_Last_Modifier&&i_UserDef==in.i_UserDef);
}