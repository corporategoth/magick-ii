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
#include "memoserv.h"
#include "magick.h"

Memo_t::Memo_t(mstring nick, mstring sender, mstring text, unsigned long file)
{
    FT("Memo_t::Memo_t", (nick, sender, text, file));
    i_Nick = nick;
    i_Time = Now();
    i_Sender = sender;
    i_Text = text;
    i_File = file;
    i_Read = false;
}


void Memo_t::operator=(const Memo_t &in)
{
    FT("Memo_t::operator=", ("(const Memo_t &) in"));
    i_Nick = in.i_Nick;
    i_Time = in.i_Time;
    i_Sender = in.i_Sender;
    i_Text = in.i_Text;
    i_File = in.i_File;
    i_Read = in.i_Read;
}


News_t::News_t(mstring channel, mstring sender, mstring text)
{
    FT("News_t::News_t", (channel, sender, text));
    i_Channel = channel;
    i_Time = Now();
    i_Sender = sender;
    i_Text = text;
}


void News_t::operator=(const News_t &in)
{
    FT("News_t::operator=", ("(const News_t &) in"));
    i_Channel = in.i_Channel;
    i_Time = in.i_Time;
    i_Sender = in.i_Sender;
    i_Text = in.i_Text;
    i_Read = in.i_Read;
}


bool News_t::IsRead(mstring name)
{
    FT("News_t::IsRead", (name));
    RET(i_Read.find(name.LowerCase())!=i_Read.end());
}


void News_t::Read(mstring name)
{
    FT("News_t::Read", (name));
    i_Read.insert(name.LowerCase());
}


void News_t::Unread(mstring name)
{
    FT("News_t::Unread", (name));
    i_Read.erase(name.LowerCase());
}

void MemoServ::AddCommands()
{
    NFT("MemoServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

//  Parent->commands.AddSystemCommand(GetInternalName(),
//		    "TRACE", "ALL", OperServ::do_Trace);
}

void MemoServ::RemCommands()
{
    NFT("MemoServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

//  Parent->commands.RemSystemCommand(GetInternalName(),
//		    "TRACE", "ALL");
}

bool MemoServ::IsNick(mstring in)
{
    FT("MemoServ::IsNick", (in));
    RET(nick.find(in.LowerCase())!=nick.end());
}

bool MemoServ::IsChannel(mstring in)
{
    FT("MemoServ::IsChannel", (in));
    RET(channel.find(in.LowerCase())!=channel.end());
}

void MemoServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_MemoServ);
    FT("MemoServ::execute", (data));
    //okay this is the main nickserv command switcher


    // Nick/Server PRIVMSG/NOTICE mynick :message

    mstring source, msgtype, mynick, message, command;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ").UpperCase();
    mynick  = data.ExtractWord(3, ": ");
    message = data.After(":", 2);
    command = message.Before(" ");

    if (message[0U] == CTCP_DELIM_CHAR)
    {
	if (msgtype == "PRIVMSG")
	    DccEngine::decodeRequest(mynick, source, message);
	else
	    DccEngine::decodeReply(mynick, source, message);
    }
    else if (!Parent->commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
	send(mynick, source, "Invalid command.");
    }


    mThread::ReAttach(tt_mBase);

}

void MemoServ::load_database(wxInputStream& in)
{
    map<mstring,list<Memo_t> >::size_type i,count, i2, count2;
    in>>count;
    nick.clear();
    for(i=0;i<count;i++)
    {
	mstring temp;
	Memo_t tmpmemo;
	in>>temp>>count2;
	for(i2=0;i2<count2;i2++)
	{
	    in>>tmpmemo;
	    nick[temp].push_back(tmpmemo);
	}
    }
    in>>count;
    channel.clear();
    for(i=0;i<count;count++)
    {
	mstring temp;
	News_t tmpnews;
	in>>temp>>count2;
	for(i2=0;i2<count2;i2++)
	{
	    in>>tmpnews;
	    channel[temp].push_back(tmpnews);
	}
    }
}

void MemoServ::save_database(wxOutputStream& out)
{
    out<<nick.size();
    map<mstring,list<Memo_t> >::iterator i;
    for(i=nick.begin();i!=nick.end();i++)
    {
	out<<i->first;
	out<<i->second.size();
	list<Memo_t>::iterator j;
	for(j=i->second.begin();j!=i->second.end();j++)
	    out<<(*j);
    }
    out<<channel.size();
    map<mstring,list<News_t> >::iterator i2;
    for(i2=channel.begin();i2!=channel.end();i2++)
    {
	out<<i2->first;
	out<<i2->second.size();
	list<News_t>::iterator j2;
	for(j2=i2->second.begin();j2!=i2->second.end();j2++)
	    out<<(*j2);
    }
}

wxOutputStream &operator<<(wxOutputStream& out,Memo_t& in)
{
    out<<in.i_Nick<<in.i_Sender<<in.i_Time<<in.i_Text<<in.i_Read<<in.i_File;
    return out;
}
wxInputStream &operator>>(wxInputStream& in, Memo_t& out)
{
    in>>out.i_Nick>>out.i_Sender>>out.i_Time>>out.i_Text>>out.i_Read>>out.i_File;
    return in;
}
wxOutputStream &operator<<(wxOutputStream& out,News_t& in)
{
    out<<in.i_Channel<<in.i_Sender<<in.i_Time<<in.i_Text;
    out<<in.i_Read.size();
    
    set<mstring>::iterator i;
    for(i=in.i_Read.begin();i!=in.i_Read.end();i++)
	out<<(*i);

    return out;
}
wxInputStream &operator>>(wxInputStream& in, News_t& out)
{
    in>>out.i_Channel>>out.i_Sender>>out.i_Time>>out.i_Text;
    out.i_Read.clear();
    set<mstring>::size_type i,j;
    in>>i;
    for(j=0;j<i;j++)
    {
	mstring temp;
	in>>temp;
	out.i_Read.insert(temp);
    }

    return in;
}
