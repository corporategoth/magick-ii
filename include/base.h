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
** Revision 1.90  2001/05/02 02:35:26  prez
** Fixed dependancy system, and removed printf's - we no longer coredump on
** a 1000 user network.  As a bonus, we actually synd perfectly ;P
**
** Revision 1.89  2001/05/01 14:00:21  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.88  2001/04/09 07:52:22  prez
** Fixed /nickserv.  Fixed cordump in nick expiry.  Fixed slight bugs in mstring.
**
** Revision 1.87  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.86  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.85  2001/03/27 07:04:30  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.84  2001/03/20 14:22:13  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.83  2001/03/08 08:07:40  ungod
** fixes for bcc 5.5
**
** Revision 1.82  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.81  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
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
    mutable vector<mstring *> ud_array;
    mutable map<mstring,mstring> i_UserDef;
public:
    virtual ~mUserDef() {}
    mstring UserDef(const mstring &type) const
    {
	FT("mUserDef::UserDef", (type));
	map<mstring,mstring>::const_iterator iter = i_UserDef.find(type.LowerCase());
	if (iter != i_UserDef.end())
	{
	    RET(iter->second);
	}
	RET("");
    }
    void UserDef(const mstring &type, const mstring &val)
    {
	FT("mUserDef::UserDef", (type, val));
	if (val.IsSameAs("NONE", true))
	{
	    i_UserDef.erase(type.LowerCase());
	}
	else
	{
	    i_UserDef[type.LowerCase()] = val;
	}
    }
};

class mMessage : public ACE_Method_Request
{
    friend class EventTask;

    mstring source_, msgtype_, params_;
    mDateTime creation_;

public:
    enum type_t { ServerExists, ServerNoExists, NickExists, NickNoExists,
	ChanExists, ChanNoExists, UserInChan, UserNoInChan };

private:

    list<triplet<type_t, mstring, bool> > dependancies;
    static map<type_t, map<mstring, set<mMessage *> > > AllDependancies;
    void AddDepend(const type_t type, const mstring& param)
	{ dependancies.push_back(triplet<type_t,mstring,bool>(type, param, false)); }
    void AddDependancies();

public:
    mMessage(const mstring& source, const mstring& msgtype, const mstring& params, const u_long priority = 0)
	: ACE_Method_Request(priority), source_(source), msgtype_(msgtype), params_(params),
	  creation_(mDateTime::CurrentDateTime()) {}

    bool OutstandingDependancies();
    static void CheckDependancies(type_t type, const mstring& param1, const mstring& param2 = "");
    void DependancySatisfied(type_t type, const mstring& param);

    mstring source() { return source_; }
    mstring msgtype() { return msgtype_; }
    mstring params() { return params_; }
    mDateTime creation() { return creation_; }

    virtual int call();
};

class mBaseTask : public ACE_Task<ACE_MT_SYNCH>
{
    friend class mBase;

    void PreParse(mstring& message) const;
protected:
    ACE_Activation_Queue message_queue_;
    size_t thread_count;
public:
    mBaseTask() {}
    virtual ~mBaseTask() {}
    virtual int open(void *in=0);
    virtual int close(unsigned long flags=0);
    virtual int svc(void);
    void message(const mstring& message);
    int check_LWM();
    void i_shutdown();
};

class mBase
{
    friend int mBaseTask::open(void *in);
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
    static int check_LWM() { return BaseTask.check_LWM(); }

    static void push_message(const mstring& message);
    virtual void execute(mstring& source, const mstring& msgtype, const mstring& params) =0;

    virtual mstring FirstName() const { return names.Before(" "); }
    virtual mstring GetNames() const { return names; }
    virtual bool IsName(mstring in) const
    {
	mstring tmp(" "+names.UpperCase()+" ");
	return tmp.Contains(" "+in.UpperCase()+" ");
    };

    virtual threadtype_enum Get_TType() const =0;
    virtual mstring GetInternalName() const =0;

    virtual bool MSG() const		{ return messages; }
    virtual void MSG(const bool on)	{ messages=on; } 

    virtual bool signon(const mstring& nickname) const;
    virtual bool signoff(const mstring& nickname) const;
    virtual void privmsg(const mstring &source, const mstring &dest, const char *pszFormat, ...) const;
    virtual void privmsg(const mstring &dest, const char *pszFormat, ...) const;
    virtual void privmsgV(const mstring &source, const mstring &dest, const char *pszFormat, va_list argptr) const;
    virtual void notice(const mstring &source, const mstring &dest, const char *pszFormat, ...) const;
    virtual void notice(const mstring &dest, const char *pszFormat, ...) const;
    virtual void noticeV(const mstring &source, const mstring &dest, const char *pszFormat, va_list argptr) const;
    virtual void send(const mstring &source, const mstring &dest, const char *pszFormat, ...) const;
    virtual void send(const mstring &dest, const char *pszFormat, ...) const;
    virtual void sendV(const mstring &source, const mstring &dest, const char *pszFormat, va_list argptr) const;

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
    entlist_t(const mstring &entry, const mstring &nick, const mDateTime &modtime = mDateTime::CurrentDateTime())
	: i_Entry(entry), i_Last_Modify_Time(modtime), i_Last_Modifier(nick)
    {
	FT("entlist_t::entlist_t", (entry, nick, modtime));
    }

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
    virtual void PostLoad() const;

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
    entlist_val_t (const entlist_val_t& in) { *this = in; }
    entlist_val_t (const mstring &entry, const T &value, const mstring &nick, const mDateTime &modtime = mDateTime::CurrentDateTime(), const bool stupid = false)
	: entlist_t(entry,nick,modtime), i_Value(value), i_Stupid(stupid)
    {
	FT("entlist_val_t<T>::entlist_val_t", (entry, "(T) value", nick,
							modtime, stupid));
    }
    virtual ~entlist_val_t () {}
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

    virtual bool Value(const T &value, const mstring& nick)
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
	FT("entlist_val_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
	entlist_t::BeginElement(pIn, pElement);
    }
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
    {
	FT("entlist_val_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
	entlist_t::EndElement(pIn,pElement);
	if( pElement->IsA(tag_Value) )    pElement->Retrieve(i_Value);
	if( pElement->IsA(tag_Stupid) )   pElement->Retrieve(i_Stupid);
    }
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
    {
	FT("entlist_val_t::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::Dict &) attribs"));
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
    entlist_val_t (const entlist_val_t& in) { *this = in; }
    entlist_val_t (const mstring &entry, const pair<T1,T2> &value, const mstring &nick, const mDateTime &modtime = mDateTime::CurrentDateTime(), const bool stupid = false)
	: entlist_t(entry,nick,modtime), i_Value(value), i_Stupid(stupid)
    {
	FT("entlist_val_t< pair<T1, T2> >::entlist_val_t", (entry, "( pair<T1,T2> ) value", nick,
							modtime, stupid));
    }
    virtual ~entlist_val_t () {}
    virtual void operator=(const entlist_val_t &in)
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

    virtual bool Value(pair<T1,T2> &value, const mstring &nick)
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
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
    {
	FT("entlist_val_t< pair<T1,T2> >::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
	entlist_t::BeginElement(pIn, pElement);
    }
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
    {
	FT("entlist_val_t< pair<T1,T2> >::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
	entlist_t::EndElement(pIn,pElement);
	if( pElement->IsA(tag_ValueFirst) )   pElement->Retrieve(i_Value.first);
	    if( pElement->IsA(tag_ValueSecond) )   pElement->Retrieve(i_Value.second);
	if( pElement->IsA(tag_Stupid) )   pElement->Retrieve(i_Stupid);
    }
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
    {
	FT("entlist_val_t< pair<T1,T2> >::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::Dict &) attribs"));
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
    typedef void (*functor)(const mstring&, const mstring&, const mstring&);
    // map<service, map<command, pair<committees, functor> > >
	typedef list<triplet<mstring, mstring, functor> > cmdtype;
	typedef map<mstring, cmdtype> cmdmap;
	cmdmap i_user;
    cmdmap i_system;

public:
    void AddSystemCommand(const mstring &service, const mstring &command,
	    const mstring &committees, functor function);
    void RemSystemCommand(const mstring &service, const mstring &command,
	    const mstring &committees);
    void AddCommand(const mstring &service, const mstring &command,
	    const mstring &committees, functor function);
    void RemCommand(const mstring &service, const mstring &command,
	    const mstring &committees);
    pair<bool, functor> GetSystemCommand(const mstring &service,
    	    const mstring &command, const mstring &user) const;
    pair<bool, functor> GetUserCommand(const mstring &service,
	    const mstring &command, const mstring &user) const;

    bool DoCommand(const mstring &mynick, const mstring &user,
	    const mstring &command, const mstring &params) const;
    bool DoUserCommand(const mstring &mynick, const mstring &user,
	    const mstring &command, const mstring &params) const;
    bool DoSystemCommand(const mstring &mynick, const mstring &user,
	    const mstring &command, const mstring &params) const;
};
void do_1_2param(const mstring &mynick, const mstring &source, const mstring &params);
void do_1_3param(const mstring &mynick, const mstring &source, const mstring &params);
void do_1_4param(const mstring &mynick, const mstring &source, const mstring &params);
void do_1_2paramswap(const mstring &mynick, const mstring &source, const mstring &params);
void do_1_3paramswap(const mstring &mynick, const mstring &source, const mstring &params);
void do_1_4paramswap(const mstring &mynick, const mstring &source, const mstring &params);
void do_2param(const mstring &mynick, const mstring &source, const mstring &params);
void do_3param(const mstring &mynick, const mstring &source, const mstring &params);
void do_4param(const mstring &mynick, const mstring &source, const mstring &params);

#endif

