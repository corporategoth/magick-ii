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

Memo_t::Memo_t(mstring nick, mstring sender, mstring text, long file)
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

    mstring source, msgtype, mynick, message;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ");
    mynick  = data.ExtractWord(3, ": ");
    message = data.After(":", 2);

    mThread::ReAttach(tt_mBase);

}

void MemoServ::load_database(void)
{
}
void MemoServ::save_database(void)
{
}
