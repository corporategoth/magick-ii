#ifndef WIN32
  #pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _MEMOSERV_H
#define _MEMOSERV_H
#include "pch.h"
RCSID(memoserv_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.55  2001/12/25 08:43:12  prez
** Fixed XML support properly ... it now works again with new version of
** expat (1.95.2) and sxp (1.1).  Also removed some of my const hacks.
**
** Revision 1.54  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.53  2001/11/03 21:02:50  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.52  2001/06/15 07:20:39  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.51  2001/05/17 19:18:53  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.50  2001/05/01 14:00:22  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.49  2001/04/13 08:50:48  prez
** Fixed const for unix
**
** Revision 1.48  2001/04/13 03:10:02  ungod
** more changes to make borland compilable
** (still not so in ide, but command line compile works)
**
** Revision 1.47  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.46  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.45  2001/03/27 07:04:30  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.44  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.43  2001/03/08 08:07:40  ungod
** fixes for bcc 5.5
**
** Revision 1.42  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.41  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.39  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.36  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.35  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.34  2000/09/02 07:20:44  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.33  2000/08/19 10:59:46  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.32  2000/08/07 12:20:26  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.31  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.30  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.29  2000/06/12 06:07:49  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.28  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.27  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.26  2000/05/14 04:02:52  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.25  2000/05/09 09:11:59  prez
** Added XMLisation to non-mapped structures ... still need to
** do the saving stuff ...
**
** Revision 1.24  2000/04/03 09:45:21  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.23  2000/03/23 10:22:24  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.22  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.21  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#include "base.h"

struct MemoList_CUR;
struct NewsList_CUR;
struct ESP_MemoInfo;
struct EPO_MemoInfo;

class Memo_t : public mUserDef, public SXP::IPersistObj
{
    friend list<Memo_t> CreateMemoEntry(MemoList_CUR *ml);
    friend list<Memo_t> ESP_CreateMemoEntry(ESP_MemoInfo *ml, char *nick);
    friend list<Memo_t> EPO_CreateMemoEntry(EPO_MemoInfo *ml, char *nick);
    friend class MemoServ;

    mstring i_Nick;
    mstring i_Sender;
    mDateTime i_Time;
    mstring i_Text;

    bool i_Read;
    unsigned long i_File;

    static SXP::Tag tag_Memo_t, tag_Nick, tag_Sender, tag_Time,
	tag_Text, tag_Read, tag_File, tag_UserDef;
public:
    Memo_t() {}
    Memo_t(const Memo_t &in) : mUserDef(in), SXP::IPersistObj(in)
	{ *this = in; }
    Memo_t(const mstring& nick, const mstring& sender, const mstring& text,
		const unsigned long file = 0);
    ~Memo_t() {}
    void operator=(const Memo_t &in);
    bool operator==(const Memo_t &in) const
    	{ return (i_Sender == in.i_Sender && i_Time == in.i_Time); }
    bool operator!=(const Memo_t &in) const
    	{ return !(i_Sender == in.i_Sender && i_Time == in.i_Time); }
    bool operator<(const Memo_t &in) const
    	{ return (i_Time < in.i_Time); }

    void ChgNick(const mstring& in);
    mstring Nick()const	    { return i_Nick; }
    mstring Sender() const;
    mDateTime Time() const;
    mstring Text() const;
    unsigned long File() const;

    bool IsRead() const;
    void Read()	;
    void Unread();

    SXP::Tag& GetClassTag() const { return tag_Memo_t; }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs = SXP::blank_dict);

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

class News_t : public mUserDef, public SXP::IPersistObj
{
    friend list<News_t> CreateNewsEntry(NewsList_CUR *nl);
    friend list<News_t> ESP_CreateNewsEntry(ESP_MemoInfo *nl, char *chan);
    friend list<News_t> EPO_CreateNewsEntry(EPO_MemoInfo *nl, char *chan);
    friend class MemoServ;

    mstring i_Channel;
    mstring i_Sender;
    mDateTime i_Time;
    mstring i_Text;
    bool i_NoExpire;
    
    set<mstring> i_Read;
    
    static SXP::Tag tag_News_t, tag_Channel, tag_Sender, tag_Time,
	tag_Text, tag_set_NoExpire, tag_Read, tag_UserDef;
public:
    News_t() {}
    News_t(const News_t &in) : mUserDef(in), SXP::IPersistObj(in)
	{ *this = in; }
    News_t(const mstring& channel, const mstring& sender, const mstring& text,
	const bool noexpire = false);
    ~News_t() {}
    void operator=(const News_t &in);
    bool operator==(const News_t &in) const
    	{ return (i_Sender == in.i_Sender && i_Time == in.i_Time); }
    bool operator!=(const News_t &in) const
    	{ return !(i_Sender == in.i_Sender && i_Time == in.i_Time); }
    bool operator<(const News_t &in) const
    	{ return (i_Time < in.i_Time); }

    mstring Channel()const	{ return i_Channel; }
    mstring Sender() const;
    mDateTime Time() const;
    mstring Text() const;

    bool NoExpire() const;
    void NoExpire(const bool in);
    bool IsRead(const mstring& name);
    void Read(const mstring& name);
    void Unread(const mstring& name);

    SXP::Tag& GetClassTag() const { return tag_News_t; }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs = SXP::blank_dict);

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

    vector<Memo_t *> m_array;
    vector<News_t *> n_array;

public:
    typedef list<Memo_t> nick_memo_t;
    typedef map<mstring, nick_memo_t> nick_t;
    typedef list<News_t> channel_news_t;
    typedef map<mstring, channel_news_t> channel_t;

private:
    nick_t nick;
    channel_t channel;

    void NickMemoConvert(const mstring &source, const mstring &target);
    void AddCommands();
    void RemCommands();
public:
    MemoServ();
    ~MemoServ() {}
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
	stats_t() { clear(); }
	void clear() {
	    i_ClearTime = mDateTime::CurrentDateTime();
	    i_Read = i_Unread = i_Send = i_Flush = i_Reply = i_Forward =
	    	i_Cancel = i_Del = i_Set = i_Continue = i_File = i_Get = 0; }
	mDateTime ClearTime()	    { return i_ClearTime; }
	unsigned long Read()	    { return i_Read; }
	unsigned long Unread()	    { return i_Unread; }
	unsigned long Send()	    { return i_Send; }
	unsigned long Flush()	    { return i_Flush; }
	unsigned long Reply()	    { return i_Reply; }
	unsigned long Forward()	    { return i_Forward; }
	unsigned long Cancel()	    { return i_Cancel; }
	unsigned long Del()	    { return i_Del; }
	unsigned long Set()	    { return i_Set; }
	unsigned long Continue()    { return i_Continue; }
	unsigned long File()	    { return i_File; }
	unsigned long Get()	    { return i_Get; }
    } stats;

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddNick(nick_memo_t in) throw(E_MemoServ_Nick);
    void AddNickMemo(Memo_t *in) throw(E_MemoServ_Nick);
    nick_memo_t &GetNick(const mstring &in) const throw(E_MemoServ_Nick);
    Memo_t &GetNickMemo(const mstring &in, const size_t num) const throw(E_MemoServ_Nick);
    void RemNick(const mstring &in) throw(E_MemoServ_Nick);
    void RemNickMemo(const mstring &in, const size_t num) throw(E_MemoServ_Nick);
#else
    void AddNick(nick_memo_t in);
    void AddNickMemo(Memo_t *in);
    nick_memo_t &GetNick(const mstring &in) const;
    Memo_t &GetNickMemo(const mstring &in, const size_t num) const;
    void RemNick(const mstring &in);
    void RemNickMemo(const mstring &in, const size_t num);
#endif
    nick_t::iterator NickBegin() { return nick.begin(); }
    nick_t::iterator NickEnd() { return nick.end(); }
    nick_t::const_iterator NickBegin() const { return nick.begin(); }
    nick_t::const_iterator NickEnd()  const { return nick.end(); }
    size_t NickSize() const { return nick.size(); }
    nick_memo_t::iterator NickMemoBegin(const mstring &in) { return GetNick(in).begin(); }
    nick_memo_t::iterator NickMemoEnd(const mstring &in) { return GetNick(in).end(); }
    nick_memo_t::const_iterator NickMemoBegin(const mstring &in) const { return GetNick(in).begin(); }
    nick_memo_t::const_iterator NickMemoEnd(const mstring &in)  const { return GetNick(in).end(); }
    size_t NickMemoSize(const mstring &in) const { return GetNick(in).size(); }
    size_t NickMemoCount(const mstring &in, const bool isread = false) const;
    bool IsNick(const mstring &in) const;
    bool IsNickMemo(const mstring &in, const size_t num) const;

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddChannel(channel_news_t in) throw(E_MemoServ_Channel);
    void AddChannelNews(News_t *in) throw(E_MemoServ_Channel);
    channel_news_t &GetChannel(const mstring &in) const throw(E_MemoServ_Channel);
    News_t &GetChannelNews(const mstring &in, const size_t num) const throw(E_MemoServ_Channel);
    void RemChannel(const mstring &in) throw(E_MemoServ_Channel);
    void RemChannelNews(const mstring &in, const size_t num) throw(E_MemoServ_Channel);
#else
    void AddChannel(channel_news_t in);
    void AddChannelNews(News_t *in);
    channel_news_t &GetChannel(const mstring &in) const;
    News_t &GetChannelNews(const mstring &in, const size_t num) const;
    void RemChannel(const mstring &in);
    void RemChannelNews(const mstring &in, const size_t num);
#endif
    channel_t::iterator ChannelBegin() { return channel.begin(); }
    channel_t::iterator ChannelEnd() { return channel.end(); }
    channel_t::const_iterator ChannelBegin() const { return channel.begin(); }
    channel_t::const_iterator ChannelEnd()  const { return channel.end(); }
    size_t ChannelSize() const { return channel.size(); }
    channel_news_t::iterator ChannelNewsBegin(const mstring &in) { return GetChannel(in).begin(); }
    channel_news_t::iterator ChannelNewsEnd(const mstring &in) { return GetChannel(in).end(); }
    channel_news_t::const_iterator ChannelNewsBegin(const mstring &in) const { return GetChannel(in).begin(); }
    channel_news_t::const_iterator ChannelNewsEnd(const mstring &in)  const { return GetChannel(in).end(); }
    size_t ChannelNewsSize(const mstring &in) const { return GetChannel(in).size(); }
    size_t ChannelNewsCount(const mstring &in, const mstring &user, const bool isread = false);
    bool IsChannel(const mstring &in) const;
    bool IsChannelNews(const mstring &in, const size_t num) const;

    unsigned long News_Expire()const	{ return news_expire; }
    unsigned long InFlight()const	{ return inflight; }
    unsigned long Delay()const		{ return delay; }
    unsigned int Files()const		{ return files; }
    unsigned long FileSize()const	{ return filesize; }

    threadtype_enum Get_TType() const { return tt_MemoServ; }
    mstring GetInternalName() const { return "MemoServ"; }
    void execute(mstring& source, const mstring& msgtype, const mstring& params);

    static void do_Help(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Read(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_UnRead(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Get(const mstring &nick, const mstring &source, const mstring &params);
    static void do_List(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Send(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Flush(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Reply(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Forward(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Forward2(const mstring &mynick, const mstring &source, mstring dest, const mstring &text);
    static void do_Cancel(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Del(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Continue(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Preview(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_File(const mstring &mynick, const mstring &source, const mstring &params);

    static void do_set_NoExpire(const mstring &mynick, const mstring &source, const mstring &params);

    SXP::Tag& GetClassTag() const { return tag_MemoServ; }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs = SXP::blank_dict);
    void PostLoad();
};

#endif
