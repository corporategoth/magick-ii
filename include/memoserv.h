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
#ifndef _memoserv_h
#define _memoserv_h

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"
#include "variant.h"
#include "base.h"


class Memo_t : public mUserDef
{
    mstring i_Nick;
    mstring i_Sender;
    mDateTime i_Time;
    mstring i_Text;

    bool i_Read;
    long i_File;
public:
    Memo_t() {}
    Memo_t(const Memo_t &in) { *this = in; }
    Memo_t(mstring nick, mstring sender, mstring text, long file = 0);
    void operator=(const Memo_t &in);
    bool operator==(const Memo_t &in) const
    	{ return (i_Sender == in.i_Sender && i_Time == in.i_Time); }
    bool operator!=(const Memo_t &in) const
    	{ return !(i_Sender == in.i_Sender && i_Time == in.i_Time); }
    bool operator<(const Memo_t &in) const
    	{ return (i_Time < in.i_Time); }

    mstring Nick()	{ return i_Nick; }
    mstring Sender()	{ return i_Sender; }
    mDateTime Time()	{ return i_Time; }
    mstring Text()	{ return i_Text; }
    long File()		{ return i_File; }

    bool IsRead()	{ return i_Read; }
    void Read()		{ i_Read = true; }
    void Unread()	{ i_Read = false; }
};


class News_t : public mUserDef
{
    mstring i_Channel;
    mstring i_Sender;
    mDateTime i_Time;
    mstring i_Text;
    
    set<mstring> i_Read;
public:
    News_t() {}
    News_t(const News_t &in) { *this = in; }
    News_t(mstring channel, mstring sender, mstring text);
    void operator=(const News_t &in);
    bool operator==(const News_t &in) const
    	{ return (i_Sender == in.i_Sender && i_Time == in.i_Time); }
    bool operator!=(const News_t &in) const
    	{ return !(i_Sender == in.i_Sender && i_Time == in.i_Time); }
    bool operator<(const News_t &in) const
    	{ return (i_Time < in.i_Time); }

    mstring Channel()	{ return i_Channel; }
    mstring Sender()	{ return i_Sender; }
    mDateTime Time()	{ return i_Time; }
    mstring Text()	{ return i_Text; }

    bool IsRead(mstring name);
    void Read(mstring name);
    void Unread(mstring name);
};


// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class MemoServ : public mBase
{
    friend class Magick;
private:
    int news_expire;

    bool memo;
    bool news;

public:
    bool IsNick(mstring nick);
    bool IsChannel(mstring channel);
    map<mstring,list<Memo_t> > nick;
    map<mstring,list<News_t> > channel;

    int News_Expire() { return news_expire; }
    bool Memo() { return memo; }
    bool News() { return news; }

    virtual void load_database(void);
    virtual void save_database(void);
    MemoServ() {}
    virtual threadtype_enum Get_TType() const { return tt_MemoServ; }
    virtual mstring GetInternalName() const { return "MemoServ"; }
    virtual void execute(const mstring & message);
};

#endif
