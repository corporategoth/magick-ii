#ifndef WIN32
#pragma interface
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
#ifndef _MEMOSERV_H
#define _MEMOSERV_H
#include "pch.h"
RCSID(memoserv_h, "@(#) $Id$");

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

#include "base.h"

struct MemoList_CUR;
struct NewsList_CUR;
struct ESP_MemoInfo;
struct EPO_MemoInfo;
struct HYB_MemoInfo;

class Memo_t : public mUserDef, public SXP::IPersistObj
{
    friend list < Memo_t > CreateMemoEntry(MemoList_CUR * ml);
    friend list < Memo_t > ESP_CreateMemoEntry(ESP_MemoInfo * ml, char *nick);
    friend list < Memo_t > EPO_CreateMemoEntry(EPO_MemoInfo * ml, char *nick);
    friend list < Memo_t > HYB_CreateMemoEntry(HYB_MemoInfo * mi);
    friend class MemoServ;

    mstring i_Nick;
    mstring i_Sender;
    mDateTime i_Time;
    mstring i_Text;

    bool i_Read;
    unsigned long i_File;

    static SXP::Tag tag_Memo_t, tag_Nick, tag_Sender, tag_Time, tag_Text, tag_Read, tag_File, tag_UserDef;

public:
    Memo_t()
    {
    }
    Memo_t(const Memo_t & in) : mUserDef(in), SXP::IPersistObj(in)
    {
	*this = in;
    }
    Memo_t(const mstring & nick, const mstring & sender, const mstring & text, const unsigned long file = 0);

    ~Memo_t()
    {
    }
    Memo_t &operator=(const Memo_t & in);
    bool operator==(const Memo_t & in) const
    {
	return (i_Sender == in.i_Sender && i_Time == in.i_Time);
    }
    bool operator!=(const Memo_t & in) const
    {
	return !(i_Sender == in.i_Sender && i_Time == in.i_Time);
    }
    bool operator<(const Memo_t & in) const
    {
	return (i_Time < in.i_Time);
    }

    bool validated() const
    {
	return (i_Nick.length() != 0 && i_Nick.validated());
    }
    void ChgNick(const mstring & in);
    mstring Nick() const
    {
	return i_Nick;
    }
    mstring Sender() const;
    mDateTime Time() const;
    mstring Text() const;
    unsigned long File() const;

    bool IsRead() const;
    void Read();
    void Unread();

    SXP::Tag & GetClassTag() const
    {
	return tag_Memo_t;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

class News_t : public mUserDef, public SXP::IPersistObj
{
    friend list < News_t > CreateNewsEntry(NewsList_CUR * nl);
    friend list < News_t > ESP_CreateNewsEntry(ESP_MemoInfo * nl, char *chan);
    friend list < News_t > EPO_CreateNewsEntry(EPO_MemoInfo * nl, char *chan);
    friend class MemoServ;

    mstring i_Channel;
    mstring i_Sender;
    mDateTime i_Time;
    mstring i_Text;
    bool i_NoExpire;

    set < mstring > i_Read;

    static SXP::Tag tag_News_t, tag_Channel, tag_Sender, tag_Time, tag_Text, tag_set_NoExpire, tag_Read, tag_UserDef;

public:
    News_t()
    {
    }
    News_t(const News_t & in) : mUserDef(in), SXP::IPersistObj(in)
    {
	*this = in;
    }
    News_t(const mstring & channel, const mstring & sender, const mstring & text, const bool noexpire = false);

    ~News_t()
    {
    }
    News_t &operator=(const News_t & in);
    bool operator==(const News_t & in) const
    {
	return (i_Sender == in.i_Sender && i_Time == in.i_Time);
    }
    bool operator!=(const News_t & in) const
    {
	return !(i_Sender == in.i_Sender && i_Time == in.i_Time);
    }
    bool operator<(const News_t & in) const
    {
	return (i_Time < in.i_Time);
    }

    bool validated() const
    {
	return (i_Channel.length() != 0 && i_Channel.validated());
    }
    mstring Channel() const
    {
	return i_Channel;
    }
    mstring Sender() const;
    mDateTime Time() const;
    mstring Text() const;

    bool NoExpire() const;
    void NoExpire(const bool in);
    bool IsRead(const mstring name) const;
    void Read(const mstring & name);
    void Unread(const mstring name);

    SXP::Tag & GetClassTag() const
    {
	return tag_News_t;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

class MemoServ : public mBase, public SXP::IPersistObj
{
    friend class Magick;
    friend class Nick_Stored_t;

private:
    unsigned long news_expire;
    unsigned long inflight;
    unsigned long delay;
    unsigned int files;
    unsigned long filesize;
    static SXP::Tag tag_MemoServ;

    vector < Memo_t * > m_array;
    vector < News_t * > n_array;

public:
    typedef list < Memo_t > nick_memo_t;
    typedef map < mstring, nick_memo_t > nick_t;
    typedef list < News_t > channel_news_t;
    typedef map < mstring, channel_news_t > channel_t;

private:
    nick_t nick;
    channel_t channel;

    void NickMemoConvert(const mstring & source, const mstring & target);
    void AddCommands();
    void RemCommands();

public:
    MemoServ();
    ~MemoServ()
    {
    }
    class stats_t
    {
	friend class MemoServ;

	mDateTime i_ClearTime;
	unsigned long i_Read;
	unsigned long i_Unread;
	unsigned long i_Send;
	unsigned long i_Flush;
	unsigned long i_Reply;
	unsigned long i_Forward;
	unsigned long i_Cancel;
	unsigned long i_Del;
	unsigned long i_Set;
	unsigned long i_Continue;
	unsigned long i_File;
	unsigned long i_Get;

    public:
	stats_t()
	{
	    clear();
	}
	void clear()
	{
	    i_ClearTime = mDateTime::CurrentDateTime();
	    i_Read = i_Unread = i_Send = i_Flush = i_Reply = i_Forward = i_Cancel = i_Del = i_Set = i_Continue = i_File =
		i_Get = 0;
	}
	mDateTime ClearTime()
	{
	    return i_ClearTime;
	}
	unsigned long Read()
	{
	    return i_Read;
	}
	unsigned long Unread()
	{
	    return i_Unread;
	}
	unsigned long Send()
	{
	    return i_Send;
	}
	unsigned long Flush()
	{
	    return i_Flush;
	}
	unsigned long Reply()
	{
	    return i_Reply;
	}
	unsigned long Forward()
	{
	    return i_Forward;
	}
	unsigned long Cancel()
	{
	    return i_Cancel;
	}
	unsigned long Del()
	{
	    return i_Del;
	}
	unsigned long Set()
	{
	    return i_Set;
	}
	unsigned long Continue()
	{
	    return i_Continue;
	}
	unsigned long File()
	{
	    return i_File;
	}
	unsigned long Get()
	{
	    return i_Get;
	}
    }
    stats;

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddNick(nick_memo_t in) throw(E_MemoServ_Nick);
    void AddNickMemo(Memo_t * in) throw(E_MemoServ_Nick);
    nick_memo_t &GetNick(const mstring & in) const throw(E_MemoServ_Nick);
    Memo_t &GetNickMemo(const mstring & in, const size_t num) const throw(E_MemoServ_Nick);
    void RemNick(const mstring & in) throw(E_MemoServ_Nick);
    void RemNickMemo(const mstring & in, const size_t num) throw(E_MemoServ_Nick);
#else
    void AddNick(nick_memo_t in);
    void AddNickMemo(Memo_t * in);
    nick_memo_t &GetNick(const mstring & in) const;
    Memo_t &GetNickMemo(const mstring & in, const size_t num) const;
    void RemNick(const mstring & in);
    void RemNickMemo(const mstring & in, const size_t num);
#endif
    nick_t::iterator NickBegin()
    {
	return nick.begin();
    }
    nick_t::iterator NickEnd()
    {
	return nick.end();
    }
    nick_t::const_iterator NickBegin() const
    {
	return nick.begin();
    }
    nick_t::const_iterator NickEnd() const
    {
	return nick.end();
    }
    size_t NickSize() const
    {
	return nick.size();
    }
    nick_memo_t::iterator NickMemoBegin(const mstring & in)
    {
	return GetNick(in).begin();
    }
    nick_memo_t::iterator NickMemoEnd(const mstring & in)
    {
	return GetNick(in).end();
    }
    nick_memo_t::const_iterator NickMemoBegin(const mstring & in) const
    {
	return GetNick(in).begin();
    }
    nick_memo_t::const_iterator NickMemoEnd(const mstring & in) const
    {
	return GetNick(in).end();
    }
    size_t NickMemoSize(const mstring & in) const
    {
	return GetNick(in).size();
    }
    size_t NickMemoCount(const mstring & in, const bool isread = false) const;
    bool IsNick(const mstring & in) const;
    bool IsNickMemo(const mstring & in, const size_t num) const;

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddChannel(channel_news_t in) throw(E_MemoServ_Channel);
    void AddChannelNews(News_t * in) throw(E_MemoServ_Channel);
    channel_news_t &GetChannel(const mstring & in) const throw(E_MemoServ_Channel);
    News_t &GetChannelNews(const mstring & in, const size_t num) const throw(E_MemoServ_Channel);
    void RemChannel(const mstring & in) throw(E_MemoServ_Channel);
    void RemChannelNews(const mstring & in, const size_t num) throw(E_MemoServ_Channel);
#else
    void AddChannel(channel_news_t in);
    void AddChannelNews(News_t * in);
    channel_news_t &GetChannel(const mstring & in) const;
    News_t &GetChannelNews(const mstring & in, const size_t num) const;
    void RemChannel(const mstring & in);
    void RemChannelNews(const mstring & in, const size_t num);
#endif
    channel_t::iterator ChannelBegin()
    {
	return channel.begin();
    }
    channel_t::iterator ChannelEnd()
    {
	return channel.end();
    }
    channel_t::const_iterator ChannelBegin() const
    {
	return channel.begin();
    }
    channel_t::const_iterator ChannelEnd() const
    {
	return channel.end();
    }
    size_t ChannelSize() const
    {
	return channel.size();
    }
    channel_news_t::iterator ChannelNewsBegin(const mstring & in)
    {
	return GetChannel(in).begin();
    }
    channel_news_t::iterator ChannelNewsEnd(const mstring & in)
    {
	return GetChannel(in).end();
    }
    channel_news_t::const_iterator ChannelNewsBegin(const mstring & in) const
    {
	return GetChannel(in).begin();
    }
    channel_news_t::const_iterator ChannelNewsEnd(const mstring & in) const
    {
	return GetChannel(in).end();
    }
    size_t ChannelNewsSize(const mstring & in) const
    {
	return GetChannel(in).size();
    }
    size_t ChannelNewsCount(const mstring & in, const mstring & user, const bool isread = false) const;
    bool IsChannel(const mstring & in) const;
    bool IsChannelNews(const mstring & in, const size_t num) const;

    unsigned long News_Expire() const
    {
	return news_expire;
    }
    unsigned long InFlight() const
    {
	return inflight;
    }
    unsigned long Delay() const
    {
	return delay;
    }
    unsigned int Files() const
    {
	return files;
    }
    unsigned long FileSize() const
    {
	return filesize;
    }

    threadtype_enum Get_TType() const
    {
	return tt_MemoServ;
    }
    mstring GetInternalName() const
    {
	return "MemoServ";
    }
    void execute(mstring & source, const mstring & msgtype, const mstring & params);

    static void do_Help(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Read(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_UnRead(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Get(const mstring & nick, const mstring & source, const mstring & params);
    static void do_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Send(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Flush(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Reply(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Forward(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Forward2(const mstring & mynick, const mstring & source, mstring dest, const mstring & text);
    static void do_Cancel(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Continue(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Preview(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_File(const mstring & mynick, const mstring & source, const mstring & params);

    static void do_set_NoExpire(const mstring & mynick, const mstring & source, const mstring & params);

    SXP::Tag & GetClassTag() const
    {
	return tag_MemoServ;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);
    void PostLoad();
};

#endif
