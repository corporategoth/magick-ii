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
#ifndef _BASE_H
#define _BASE_H
#include "pch.h"
RCSID(base_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.80  2001/02/03 05:16:19  prez
** Fixed up mdatetime
**
** Revision 1.79  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.76  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.75  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.74  2000/12/09 11:24:25  prez
** Changed all privmsg/notice/send/announce calls to have pszFormat
** as a char *, to stop warnings from va_start.
**
** Revision 1.73  2000/09/02 07:20:44  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.72  2000/08/28 10:51:35  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.71  2000/08/22 08:43:39  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.70  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.69  2000/06/12 06:07:49  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.68  2000/06/06 08:57:54  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.67  2000/05/28 05:05:12  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.66  2000/05/17 07:47:57  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.65  2000/05/13 14:20:44  ungod
** no message
**
** Revision 1.64  2000/05/13 14:05:25  ungod
** no message
**
** Revision 1.63  2000/05/13 08:26:44  ungod
** no message
**
** Revision 1.62  2000/05/13 07:05:46  prez
** Added displaying of sizes to all file fields..
**
** Revision 1.61  2000/05/13 06:48:49  ungod
** no message
**
** Revision 1.60  2000/05/08 14:42:01  prez
** More on xmlisation of nickserv and chanserv
**
** Revision 1.59  2000/05/01 03:11:39  ungod
** xmlisation of entlist_t done
**
** Revision 1.58  2000/04/15 11:11:44  ungod
** starting xmlage of magick
**
** Revision 1.57  2000/04/02 07:25:05  prez
** Fixed low watermarks with threads, it all works now!
**
** Revision 1.56  2000/03/29 09:41:17  prez
** Attempting to fix thread problem with mBase, and added notification
** of new memos on join of channel or signon to network.
**
** Revision 1.55  2000/03/19 08:50:52  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.54  2000/03/14 13:37:35  prez
** *** empty log message ***
**
** Revision 1.53  2000/03/14 10:05:16  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.52  2000/03/02 11:59:45  prez
** More helpfile updates (slowly but surely writing it)
**
** Revision 1.51  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.50  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "lockable.h"
#include "utils.h"
#include "xml/sxp.h"

class mUserDef
{
protected:
    vector<mstring *> ud_array;
    map<mstring,mstring> i_UserDef;
public:
    virtual ~mUserDef() {}
    virtual mstring UserDef(mstring type) const;
    virtual void UserDef(mstring type, mstring val);
};

class mBaseTask : public ACE_Task<ACE_MT_SYNCH>
{
    friend class mBase;

    mstring PreParse(const mstring& message) const;
protected:
    ACE_Message_Queue<ACE_MT_SYNCH> message_queue_;
    size_t thread_count;
    int message_i(const mstring& message);
public:
    mBaseTask() {}
    virtual ~mBaseTask() {}
    virtual int open(void *in=0);
    virtual int close(unsigned long flags=0);
    virtual int svc(void);
    void message(const mstring& message);
    void i_shutdown();
};

class mBase
{
    friend mBaseTask;
protected:
    mstring names;		// Names of service (space delimited)
    mstring realname;		// 'Real Name' of service

    bool messages;		// Wether to process /MSG, /NOTICE.

    //deque<pair<mstring,mstring> > inputbuffer; // pair of sentto,datastring
    static bool TaskOpened;
    static mBaseTask BaseTask;
    virtual void AddCommands() {};
    virtual void RemCommands() {};

public:
    mBase() {}
    virtual ~mBase() {}
    static void init();
    static void shutdown();

    static void push_message(const mstring& message);
    static void push_message_immediately(const mstring& message);
    virtual void execute(const mstring& message) =0;

    virtual mstring FirstName() const { return names.Before(" "); }
    virtual mstring GetNames() const { return names; }
    virtual bool IsName(mstring in) const
    {
	mstring tmp = " "+names.UpperCase()+" ";
	return tmp.Contains(" "+in.UpperCase()+" ");
    };

    virtual threadtype_enum Get_TType() const =0;
    virtual mstring GetInternalName() const =0;

    virtual bool MSG() const	{ return messages; }
    virtual void MSG(bool on)	{ messages=on; } 

    bool signon(const mstring& nickname) const;
    bool signoff(const mstring& nickname) const;
    void privmsg(const mstring &source, const mstring &dest, const char *pszFormat, ...) const;
    void privmsg(const mstring &dest, const char *pszFormat, ...) const;
    void privmsgV(const mstring &source, const mstring &dest, const char *pszFormat, va_list argptr) const;
    void notice(const mstring &source, const mstring &dest, const char *pszFormat, ...) const;
    void notice(const mstring &dest, const char *pszFormat, ...) const;
    void noticeV(const mstring &source, const mstring &dest, const char *pszFormat, va_list argptr) const;
    void send(const mstring &source, const mstring &dest, const char *pszFormat, ...) const;
    void send(const mstring &dest, const char *pszFormat, ...) const;
    void sendV(const mstring &source, const mstring &dest, const char *pszFormat, va_list argptr) const;

    virtual operator mVariant() const
    {
	mVariant locvar(GetInternalName());
	locvar.truevaluetype=GetInternalName();
	return locvar;
    };
};

void privmsg(const mstring &source, const mstring &dest, const char *pszFormat, ...);
void notice(const mstring &source, const mstring &dest, const char *pszFormat, ...);
void send(const mstring &source, const mstring &dest, const char *pszFormat, ...);
void announce(const mstring &source, const char *pszFormat, ...);

extern SXP::Tag tag_entlist_val_t;
extern SXP::Tag tag_entlist_t;
extern SXP::Tag tag_Entry, tag_Value, tag_Last_Modify_Time,
	tag_Last_Modifier, tag_UserDef, tag_Stupid;
extern SXP::Tag tag_ValueFirst, tag_ValueSecond;

class entlist_t : public mUserDef, public SXP::IPersistObj
{
protected:
    mstring i_Entry;
    mDateTime i_Last_Modify_Time;
    mstring i_Last_Modifier;

public:
    entlist_t () {}
    entlist_t (const entlist_t& in) { *this = in; }
    entlist_t (mstring entry, mstring nick, mDateTime modtime = mDateTime::CurrentDateTime());
    virtual ~entlist_t () {}
    void operator=(const entlist_t &in);
    bool operator==(const entlist_t &in) const
	{ return (i_Entry == in.i_Entry); }
    bool operator!=(const entlist_t &in) const
	{ return (i_Entry != in.i_Entry); }
    bool operator<(const entlist_t &in) const
	{ return (i_Entry < in.i_Entry); }

    virtual mstring Entry()const		{ return i_Entry; }
    virtual mDateTime Last_Modify_Time()const	{ return i_Last_Modify_Time; }
    virtual mstring Last_Modifier()const	{ return i_Last_Modifier; }

    // XML handling section
    virtual SXP::Tag& GetClassTag() const { return tag_entlist_t; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);
    virtual void PostLoad();

    virtual size_t Usage() const;
    virtual void DumpB() const;
    virtual void DumpE() const;
};
typedef list<entlist_t>::iterator entlist_i;
typedef list<entlist_t>::const_iterator entlist_ci;
typedef set<entlist_t>::iterator entlist_ui;
typedef set<entlist_t>::const_iterator entlist_cui;

template<class T>
class entlist_val_t : public entlist_t
{
protected:
    T i_Value;
    bool i_Stupid;	// if TRUE, Value() does nothing.

public:
    entlist_val_t () {}
    virtual ~entlist_val_t () {}
    entlist_val_t (const entlist_val_t& in) { *this = in; }
    entlist_val_t (mstring entry, T value, mstring nick, mDateTime modtime = mDateTime::CurrentDateTime(), bool stupid = false)
	: entlist_t(entry,nick,modtime)
    {
	FT("entlist_val_t<T>::entlist_val_t", (entry, "(T) value", nick,
							modtime, stupid));
	i_Value = value;
	i_Stupid = stupid;
    }
    void operator=(const entlist_val_t &in)
    {
	FT("entlist_val_t<T>::operator=", ("(const entlist_val_t<T> &) in"));
	i_Entry=in.i_Entry;
	i_Value=in.i_Value;
	i_Last_Modify_Time=in.i_Last_Modify_Time;
	i_Last_Modifier=in.i_Last_Modifier;
	i_Stupid = in.i_Stupid;
	map<mstring,mstring>::const_iterator i;
	i_UserDef.clear();
	for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
	    i_UserDef[i->first]=i->second;
    }

    virtual bool Value(T value, mstring nick)
    {
	FT("entlist_val_t<T>::Change", ("(T) value", nick));
	if (i_Stupid)
	{
	    RET(false);
	}
	else
	{
	    i_Value = value;
	    i_Last_Modify_Time = mDateTime::CurrentDateTime();
	    i_Last_Modifier = nick;
	    RET(true);
	}
    }
    virtual T Value()const			{ return i_Value; }

    virtual SXP::Tag& GetClassTag() const { return tag_entlist_val_t; };
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
    {
	entlist_t::BeginElement(pIn, pElement);
    }
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
    {
	entlist_t::EndElement(pIn,pElement);
	if( pElement->IsA(tag_Value) )    pElement->Retrieve(i_Value);
	if( pElement->IsA(tag_Stupid) )   pElement->Retrieve(i_Stupid);
    }
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
    {
	pOut->BeginObject(tag_entlist_val_t, attribs);
	entlist_t::WriteElement(pOut,attribs);

	pOut->WriteElement(tag_Value, i_Value);
	pOut->WriteElement(tag_Stupid, i_Stupid);

	pOut->EndObject(tag_entlist_val_t);
    }

    virtual size_t Usage() const
    {
	size_t retval = entlist_t::Usage();
	retval += sizeof(i_Value);
	retval += sizeof(i_Stupid);
	return retval;
    }

    virtual void DumpB() const
    {
	entlist_t::DumpB();
	//MB(4, (i_Stupid));
    }
    virtual void DumpE() const
    {
	entlist_t::DumpE();
	//ME(4, (i_Stupid));
    }
};

/*  These dont work with templates
template<class T>
typedef list<entlist_val_t<T> >::iterator entlist_val_i;
template<class T>
typedef list<entlist_val_t<T> >::const_iterator entlist_val_ci;
template<class T>
typedef set<entlist_val_t<T> >::iterator entlist_val_ui;
template<class T>
typedef set<entlist_val_t<T> >::const_iterator entlist_val_cui;
*/

template<class T1, class T2>
class entlist_val_t< pair<T1,T2> > : public entlist_t
{
protected:
    pair<T1,T2> i_Value;
    bool i_Stupid;	// if TRUE, Value() does nothing.

public:
    entlist_val_t () {}
    virtual ~entlist_val_t () {}
    entlist_val_t (const entlist_val_t& in) { *this = in; }
    entlist_val_t (mstring entry, pair<T1,T2> value, mstring nick, mDateTime modtime = mDateTime::CurrentDateTime(), bool stupid = false)
	: entlist_t(entry,nick,modtime)
    {
	FT("entlist_val_t< pair<T1, T2> >::entlist_val_t", (entry, "( pair<T1,T2> ) value", nick,
							modtime, stupid));
	i_Value = value;
	i_Stupid = stupid;
    }
    void operator=(const entlist_val_t &in)
    {
	FT("entlist_val_t< pair<T1, T2> >::operator=", ("(const entlist_val_t< pair<T1,T2> > &) in"));
	i_Entry=in.i_Entry;
	i_Value=in.i_Value;
	i_Last_Modify_Time=in.i_Last_Modify_Time;
	i_Last_Modifier=in.i_Last_Modifier;
	i_Stupid = in.i_Stupid;
	map<mstring,mstring>::const_iterator i;
	i_UserDef.clear();
	for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
	    i_UserDef[i->first]=i->second;
    }

    virtual bool Value(pair<T1,T2> value, mstring nick)
    {
	FT("entlist_val_t< pair<T1,T2> >::Change", ("(pair<T1,T2>) value", nick));
	if (i_Stupid)
	{
	    RET(false);
	}
	else
	{
	    i_Value = value;
	    i_Last_Modify_Time = mDateTime::CurrentDateTime();
	    i_Last_Modifier = nick;
	    RET(true);
	}
    }
    pair<T1,T2> Value()const			{ return i_Value; }

    virtual SXP::Tag& GetClassTag() const { return tag_entlist_val_t; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement) { };
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
    {
	entlist_t::EndElement(pIn,pElement);
	if( pElement->IsA(tag_ValueFirst) )   pElement->Retrieve(i_Value.first);
	    if( pElement->IsA(tag_ValueSecond) )   pElement->Retrieve(i_Value.second);
	if( pElement->IsA(tag_Stupid) )   pElement->Retrieve(i_Stupid);
    }
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
    {
	pOut->BeginObject(tag_entlist_val_t, attribs);
	entlist_t::WriteElement(pOut,attribs);

	pOut->WriteElement(tag_ValueFirst, i_Value.first);
	pOut->WriteElement(tag_ValueSecond, i_Value.second);
	pOut->WriteElement(tag_Stupid, i_Stupid);

	pOut->EndObject(tag_entlist_val_t);
    }

    virtual size_t Usage() const
    {
	size_t retval = entlist_t::Usage();
	retval += sizeof(i_Value.first);
	retval += sizeof(i_Value.second);
	retval += sizeof(i_Stupid);
	return retval;
    }

    virtual void DumpB() const
    {
	entlist_t::DumpB();
	//MB(4, (i_Stupid));
    }
    virtual void DumpE() const
    {
	entlist_t::DumpE();
	//ME(4, (i_Stupid));
    }

};

class CommandMap
{
    friend class OperServ;
    friend class NickServ;
    friend class ChanServ;
    friend class MemoServ;
    friend class CommServ;
    friend class ServMsg;
    // friend class scripted;

    typedef void (*functor)(mstring, mstring, mstring);
    // map<service, map<command, pair<committees, functor> > >
    typedef list<triplet<mstring, mstring, functor> > ctype;
    typedef map<mstring, ctype> cmap;
    cmap i_user;
    cmap i_system;

    void AddSystemCommand(mstring service, mstring command,
	    mstring committees, functor function);
    void RemSystemCommand(mstring service, mstring command,
	    mstring committees);
public:
    void AddCommand(mstring service, mstring command,
	    mstring committees, functor function);
    void RemCommand(mstring service, mstring command,
	    mstring committees);
    pair<bool, functor> GetSystemCommand(mstring service, mstring command,
	    mstring user) const;
    pair<bool, functor> GetUserCommand(mstring service, mstring command,
	    mstring user) const;

    bool DoCommand(mstring mynick, mstring user, mstring command,
	    mstring params) const;
    bool DoUserCommand(mstring mynick, mstring user, mstring command,
	    mstring params) const;
    bool DoSystemCommand(mstring mynick, mstring user, mstring command,
	    mstring params) const;
};
void do_1_2param(mstring mynick, mstring source, mstring params);
void do_1_3param(mstring mynick, mstring source, mstring params);
void do_1_2paramswap(mstring mynick, mstring source, mstring params);
void do_1_3paramswap(mstring mynick, mstring source, mstring params);
void do_2param(mstring mynick, mstring source, mstring params);
void do_3param(mstring mynick, mstring source, mstring params);

#endif

