#include "pch.h"
#ifdef WIN32
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
#include "cryptstream.h"

Chan_Live_t::Chan_Live_t()
{
    NFT("Chan_Live_t::Chan_Live_t");
}

Chan_Live_t::Chan_Live_t(const Chan_Live_t& in)
{
    NFT("Chan_Live_t::Chan_Live_t");
    *this=in;
}

Chan_Live_t::Chan_Live_t(mstring name, mstring first_user)
{
    FT("Chan_Live_t::Chan_Live_t", (name, first_user));
    i_Name = name;
    users[first_user.LowerCase()] = pair<bool,bool>(false,false);
}

void Chan_Live_t::Join(mstring nick)
{
    FT("Chan_Live_t::Join", (nick));
    if (users.find(nick.LowerCase())!=users.end())
	wxLogWarning("Duplicate JOIN message for %s in %s received.", nick.c_str(), i_Name.c_str());
    else
	users[nick.LowerCase()] = pair<bool,bool>(false,false);
}

int Chan_Live_t::Part(mstring nick)
{
    FT("Chan_Live_t::Part", (nick));
    if (users.find(nick.LowerCase())==users.end())
	wxLogWarning("PART received for %s who is not in %s.", nick.c_str(), i_Name.c_str());
    else
	users.erase(nick.LowerCase());

    RET(users.size());
}

int Chan_Live_t::Kick(mstring nick, mstring kicker)
{
    FT("Chan_Live_t::Kick", (nick));
    if (users.find(nick.LowerCase())==users.end())
	wxLogWarning("KICK from %s received for %s who is not in %s.", kicker.c_str(), nick.c_str(), i_Name.c_str());
    else
	users.erase(nick.LowerCase());

    RET(users.size());
}

bool Chan_Live_t::IsIn(mstring nick)
{
    FT("Chan_Live_t::IsIn", (nick));
    if (users.empty()) RET(false);
    RET((users.find(nick.LowerCase()) != users.end()));
}

void Chan_Live_t::operator=(const Chan_Live_t &in)
{
    NFT("Chan_Live_t::operator=");
    bans.clear();
    set<mstring>::const_iterator i;
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
    map<mstring,mstring>::const_iterator j;
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
    modes=in.modes;
    users.clear();
    map<mstring, pair<bool, bool> >::const_iterator k;
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

void Chan_Live_t::Topic(mstring topic, mstring setter)
{
    FT("Chan_Live_t::Topic", (topic, setter));
    i_Topic = topic;
    i_Topic_Setter = setter;
    i_Topic_Set_Time = Now();
}

void Chan_Live_t::Topic(mstring topic, mstring setter, mDateTime time)
{
    FT("Chan_Live_t::Topic", (topic, setter, time));
    i_Topic = topic;
    i_Topic_Setter = setter;
    i_Topic_Set_Time = time;
}

mstring Chan_Live_t::Topic()
{
    NFT("Chan_Live_t::Topic");
    RET(i_Topic);
}

mstring Chan_Live_t::Topic_Setter()
{
    NFT("Chan_Live_t::Topic_Setter");
    RET(i_Topic_Setter);
}

mDateTime Chan_Live_t::Topic_Set_Time()
{
    NFT("Chan_Live_t::Topic_Set_Time");
    RET(i_Topic_Set_Time);
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
    map<mstring,mstring>::const_iterator i;
    i_UserDef.clear();
    for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
	i_UserDef[i->first]=i->second;
}

bool userlist_t::operator==(const userlist_t &in) const
{
    NFT("userlist_t::operator==");
    RET(i_Entry==in.i_Entry&&i_Last_Modify_Time==in.i_Last_Modify_Time&&
	i_Last_Modifier==in.i_Last_Modifier&&i_UserDef==in.i_UserDef);
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

    list<userlist_t>::const_iterator j;
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
    map<mstring, mstring>::const_iterator i;
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

ChanServ::ChanServ()
{
    NFT("ChanServ::ChanServ");
    messages=true;
    automation=true;
}

bool ChanServ::IsLive(mstring in)
{
    return (live.find(in.LowerCase())!=live.end());
}

bool ChanServ::IsStored(mstring in)
{
    return (stored.find(in.LowerCase())!=stored.end());
}

void ChanServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_ChanServ);
    FT("ChanServ::execute", (data));
    //okay this is the main chanserv command switcher

    mstring source, msgtype, mynick, message;
    source  = data.Before(" ");
    msgtype = data.After(" ").Before(" ");
    mynick  = data.After(" ").After(" ").Before(" ");
    message = data.After(":");


    mThread::ReAttach(tt_mBase);
}

wxOutputStream &operator<<(wxOutputStream& out,Chan_Stored_t& in)
{
    out<<in.i_Name<<in.i_RegTime<<in.i_Description<<in.i_Password<<in.i_URL;
    out<<in.i_Mlock_On<<in.i_Mlock_Off<<in.i_Mlock_Limit;

    list<userlist_t>::iterator i;
    out<<in.i_Access_Level.size();
    for(i=in.i_Access_Level.begin();i!=in.i_Access_Level.end();i++)
	out<<*i;
    out<<in.i_Access.size();
    for(i=in.i_Access.begin();i!=in.i_Access.end();i++)
	out<<*i;
    out<<in.i_Akick.size();
    for(i=in.i_Akick.begin();i!=in.i_Akick.end();i++)
	out<<*i;
    out<<in.i_Greet.size();
    for(i=in.i_Greet.begin();i!=in.i_Greet.end();i++)
	out<<*i;

    map<mstring,mstring>::iterator j;
    out<<in.i_UserDef.size();
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	out<<(mstring)j->first<<(mstring)j->second;
    return out;
}

wxInputStream &operator>>(wxInputStream& in, Chan_Stored_t& out)
{
    unsigned int i,count;
    mstring dummy,dummy2;
    userlist_t udummy;
    in>>out.i_Name>>out.i_RegTime>>out.i_Description>>out.i_Password>>out.i_URL;
    in>>out.i_Mlock_On>>out.i_Mlock_Off>>out.i_Mlock_Key>>out.i_Mlock_Limit;

    out.i_Access_Level.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>udummy;
	out.i_Access_Level.push_back(udummy);
    }
    out.i_Access.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>udummy;
	out.i_Access.push_back(udummy);
    }
    out.i_Akick.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>udummy;
	out.i_Akick.push_back(udummy);
    }
    out.i_Greet.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>udummy;
	out.i_Greet.push_back(udummy);
    }

    out.i_UserDef.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>dummy>>dummy2;
	out.i_UserDef[dummy]=dummy2;
    }
    return in;
}

wxOutputStream &operator<<(wxOutputStream& out,userlist_t& in)
{
    out<<in.i_Entry<<in.i_Last_Modify_Time<<in.i_Last_Modifier;

    map<mstring,mstring>::iterator j;
    out<<in.i_UserDef.size();
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	out<<(mstring)j->first<<(mstring)j->second;
    return out;
    return out;
}
wxInputStream &operator>>(wxInputStream& in, userlist_t& out)
{
    unsigned int i,count;
    mstring dummy,dummy2;

    in>>out.i_Entry>>out.i_Last_Modify_Time>>out.i_Last_Modifier;

    out.i_UserDef.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>dummy>>dummy2;
	out.i_UserDef[dummy]=dummy2;
    }
    return in;
}

userlist_t::userlist_t ()
{
    // these are probably not needed but best to be safe.
    i_Entry=i_Last_Modifier="";
    i_Last_Modify_Time=mDateTime(0.0);
    i_UserDef.clear();
}

void ChanServ::load_database(void)
{
}

void ChanServ::save_database(void)
{
    mstring savename=Parent->Files_CHAN_DB+".save";
    if(wxFile::Exists(savename.c_str()))
	remove(savename.c_str());
    try
    {
        wxFileOutputStream outf(savename);
	wxDataOutputStream *outd;
	mEncryptStream *outc;
	wxZlibOutputStream *outz;
	wxDataOutputStream flagout(outf);
	 
	flagout<<FileVersionNumber;

	if(Parent->Password!=""&&Parent->Files_COMPRESS_STREAMS==true)
	{
	    outc=new mEncryptStream(outf,Parent->Password);
	    outz=new wxZlibOutputStream(*outc);
	    outd=new wxDataOutputStream(*outz);
	    flagout<<(char)3;
	}
	else if(Parent->Files_COMPRESS_STREAMS==true)
	{
	    outz=new wxZlibOutputStream(outf);
	    outd=new wxDataOutputStream(*outz);
	    flagout<<(char)2;
	}
	else if(Parent->Password!="")
	{
	    outc=new mEncryptStream(outf,Parent->Password);
	    outd=new wxDataOutputStream(*outc);
	    flagout<<(char)1;
	}
	else
	{
	    outd=new wxDataOutputStream(outf);
	    flagout<<(char)0;
	}
	
	//
	map<mstring,Chan_Stored_t>::iterator i;
	(*outd)<<stored.size();
        // todo call script saving hooks.
	for(i=stored.begin();i!=stored.end();i++)
	{
	    (*outd)<<i->second;
	    // todo call script saving hooks.
	}
	 
	if(Parent->Password!=""&&Parent->Files_COMPRESS_STREAMS==true)
	{
	    if(outd!=NULL)
		delete outd;
	    if(outz!=NULL)
		delete outd;
	    if(outc!=NULL)
		delete outd;
	}
	else if(Parent->Files_COMPRESS_STREAMS==true)
	{
	    if(outd!=NULL)
		delete outd;
	    if(outz!=NULL)
		delete outd;
	}
	else if(Parent->Password!="")
	{
	    if(outd!=NULL)
		delete outd;
	    if(outc!=NULL)
		delete outd;
	}
	else
	{
	    if(outd!=NULL)
		delete outd;
	}

    }
    catch(...)
    {
        if(wxFile::Exists(savename.c_str()))
	    remove(savename.c_str());
	return;
    }
    if(wxFile::Exists(savename.c_str()))
    {
        if(wxFile::Exists(Parent->Files_CHAN_DB.c_str()))
	    remove(Parent->Files_CHAN_DB.c_str());
	rename(savename.c_str(),Parent->Files_CHAN_DB.c_str());
    }
}
