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

class Memo_t : public mUserDef, public SXP::IPersistObj
{
    friend list<Memo_t> CreateMemoEntry(MemoList_CUR *ml);
    friend list<Memo_t> ESP_CreateMemoEntry(ESP_MemoInfo *ml, char *nick);
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
    Memo_t(const Memo_t &in) { *this = in; }
    Memo_t(mstring nick, mstring sender, mstring text, unsigned long file = 0);
    ~Memo_t() {}
    void operator=(const Memo_t &in);
    bool operator==(const Memo_t &in) const
    	{ return (i_Sender == in.i_Sender && i_Time == in.i_Time); }
    bool operator!=(const Memo_t &in) const
    	{ return !(i_Sender == in.i_Sender && i_Time == in.i_Time); }
    bool operator<(const Memo_t &in) const
    	{ return (i_Time < in.i_Time); }

    void ChgNick(mstring in);
    mstring Nick()const	    { return i_Nick; }
    mstring Sender() const;
    mDateTime Time() const;
    mstring Text() const;
    unsigned long File() const;

    bool IsRead() const;
    void Read()	;
    void Unread();

    SXP::Tag& GetClassTag() const { return tag_Memo_t; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

class News_t : public mUserDef, public SXP::IPersistObj
{
    friend list<News_t> CreateNewsEntry(NewsList_CUR *nl);
    friend list<News_t> ESP_CreateNewsEntry(ESP_MemoInfo *nl, char *chan);
    friend class MemoServ;

    mstring i_Channel;
    mstring i_Sender;
    mDateTime i_Time;
    mstring i_Text;
    
    set<mstring> i_Read;
    
    static SXP::Tag tag_News_t, tag_Channel, tag_Sender, tag_Time,
	tag_Text, tag_Read, tag_UserDef;
public:
    News_t() {}
    News_t(const News_t &in) { *this = in; }
    News_t(mstring channel, mstring sender, mstring text);
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

    bool IsRead(mstring name) const;
    void Read(mstring name);
    void Unread(mstring name);

    SXP::Tag& GetClassTag() const { return tag_News_t; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class MemoServ : public mBase, public SXP::IPersistObj
{
    friend class Magick;
private:
    unsigned long news_expire;
    unsigned long inflight;
    unsigned long delay;
    unsigned int files;
    unsigned long filesize;
    static SXP::Tag tag_MemoServ;

    vector<Memo_t *> m_array;
    vector<News_t *> n_array;

    void AddCommands();
    void RemCommands();
public:
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
	unsigned long i_Continue;
	unsigned long i_File;
    public:
	stats_t() { clear(); }
	void clear() {
	    i_ClearTime = Now();
	    i_Read = i_Unread = i_Send = i_Flush = i_Reply =
		i_Forward = i_Cancel = i_Del = i_Continue =
		i_File = 0; }
	mDateTime ClearTime()	    { return i_ClearTime; }
	unsigned long Read()	    { return i_Read; }
	unsigned long Unread()	    { return i_Unread; }
	unsigned long Send()	    { return i_Send; }
	unsigned long Flush()	    { return i_Flush; }
	unsigned long Reply()	    { return i_Reply; }
	unsigned long Forward()	    { return i_Forward; }
	unsigned long Cancel()	    { return i_Cancel; }
	unsigned long Del()	    { return i_Del; }
	unsigned long Continue()    { return i_Continue; }
	unsigned long File()	    { return i_File; }
    } stats;

    bool IsNick(mstring nick) const;
    bool IsChannel(mstring channel) const;
    map<mstring,list<Memo_t> > nick;
    map<mstring,list<News_t> > channel;

    unsigned long News_Expire()const	{ return news_expire; }
    unsigned long InFlight()const	{ return inflight; }
    unsigned long Delay()const		{ return delay; }
    unsigned int Files()const		{ return files; }
    unsigned long FileSize()const	{ return filesize; }

    MemoServ();
    virtual threadtype_enum Get_TType() const { return tt_MemoServ; }
    virtual mstring GetInternalName() const { return "MemoServ"; }
    virtual void execute(const mstring & message);

    static void do_Help(mstring mynick, mstring source, mstring params);
    static void do_Read(mstring mynick, mstring source, mstring params);
    static void do_UnRead(mstring mynick, mstring source, mstring params);
    static void do_Get(mstring nick, mstring source, mstring params);
    static void do_List(mstring mynick, mstring source, mstring params);
    static void do_Send(mstring mynick, mstring source, mstring params);
    static void do_Flush(mstring mynick, mstring source, mstring params);
    static void do_Reply(mstring mynick, mstring source, mstring params);
    static void do_Forward(mstring mynick, mstring source, mstring params);
    static void do_Forward2(mstring mynick, mstring source, mstring dest, mstring text);
    static void do_Cancel(mstring mynick, mstring source, mstring params);
    static void do_Del(mstring mynick, mstring source, mstring params);
    static void do_Continue(mstring mynick, mstring source, mstring params);
    static void do_File(mstring mynick, mstring source, mstring params);

    virtual SXP::Tag& GetClassTag() const { return tag_MemoServ; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);
    void PostLoad();
};

#endif
