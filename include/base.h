#ifndef WIN32
#pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _BASE_H
#define _BASE_H
static const char *ident_base_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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

#include "mstring.h"
#include "trace.h"
#include "dccengine.h"
#include "utils.h"
#include "xml/sxp.h"

class mUserDef
{
protected:
    map<mstring,mstring> i_UserDef;
public:
    mstring UserDef(mstring type);
    mstring UserDef(mstring source, mstring type, mstring val);
};

class mBaseTask : public ACE_Task<ACE_MT_SYNCH>
{
    friend class mBase;
    friend class mBaseTaskmessage_MO;

    mstring PreParse(const mstring& message);
protected:
    ACE_Activation_Queue activation_queue_;
    ACE_Message_Queue<ACE_MT_SYNCH> message_queue_;
    int message_i(const mstring& message);
public:
    mBaseTask() :  activation_queue_(&message_queue_) {}
    virtual int open(void *in=0);
    virtual int svc(void);
    void message(const mstring& message);
    void i_shutdown();
};

extern SXP::Tag tag_entlist_val_t;
extern SXP::Tag tag_entlist_t;
extern SXP::Tag tag_Entry, tag_Value, tag_Last_Modify_Time,
	tag_Last_Modifier, tag_UserDef, tag_Stupid;
extern SXP::Tag tag_ValueFirst, tag_ValueSecond;

class entlist_t : public mUserDef, public SXP::IPersistObj
{
    friend wxOutputStream &operator<<(wxOutputStream& out,const entlist_t& in);
    friend wxInputStream &operator>>(wxInputStream& in, entlist_t& out);
protected:
    mstring i_Entry;
    mDateTime i_Last_Modify_Time;
    mstring i_Last_Modifier;

public:
    entlist_t () {}
    entlist_t (const entlist_t& in) { *this = in; }
    entlist_t (mstring entry, mstring nick, mDateTime modtime = Now());
    void operator=(const entlist_t &in);
    bool operator==(const entlist_t &in) const
	{ return (i_Entry == in.i_Entry); }
    bool operator!=(const entlist_t &in) const
	{ return (i_Entry != in.i_Entry); }
    bool operator<(const entlist_t &in) const
	{ return (i_Entry < in.i_Entry); }

    mstring Entry()const		{ return i_Entry; }
    mDateTime Last_Modify_Time()const	{ return i_Last_Modify_Time; }
    mstring Last_Modifier()const	{ return i_Last_Modifier; }

    // XML handling section
    virtual SXP::Tag& GetClassTag() const { return tag_entlist_t; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement) { };
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);
};
wxOutputStream &operator<<(wxOutputStream& out,const entlist_t& in);
wxInputStream &operator>>(wxInputStream& in, entlist_t& out);
typedef list<entlist_t>::iterator entlist_i;
typedef list<entlist_t>::const_iterator entlist_ci;
typedef set<entlist_t>::iterator entlist_ui;
typedef set<entlist_t>::const_iterator entlist_cui;

template<class T>
class entlist_val_t : public entlist_t
{
#ifdef WIN32
    friend wxOutputStream &operator<<(wxOutputStream& out, const entlist_val_t<T>& in);
    friend wxInputStream &operator>>(wxInputStream& in, entlist_val_t<T>& out);
#else
    friend wxOutputStream &operator<<<T>(wxOutputStream& out, const entlist_val_t<T>& in);
    friend wxInputStream &operator>><T>(wxInputStream& in, entlist_val_t<T>& out);
#endif
    T i_Value;
    bool i_Stupid;	// if TRUE, Value() does nothing.

public:
    entlist_val_t () {}
    entlist_val_t (const entlist_val_t& in) { *this = in; }
    entlist_val_t (mstring entry, T value, mstring nick, mDateTime modtime = Now(), bool stupid = false);
    void operator=(const entlist_val_t &in);

    bool Value(T value, mstring nick);
    T Value()const			{ return i_Value; }

    virtual SXP::Tag& GetClassTag() const { return tag_entlist_val_t; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement) { };
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);
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

template<class T> inline
entlist_val_t<T>::entlist_val_t(mstring entry, T value, mstring nick,
					mDateTime modtime, bool stupid)
    : entlist_t(entry,nick,modtime)
{
    FT("entlist_val_t<T>::entlist_val_t", (entry, "(T) value", nick,
							modtime, stupid));
    i_Value = value;
    i_Stupid = stupid;
}

template<class T> inline
void entlist_val_t<T>::operator=(const entlist_val_t<T> &in)
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


template<class T> inline
bool entlist_val_t<T>::Value(T value, mstring nick)
{
    FT("entlist_val_t<T>::Change", ("(T) value", nick));
    if (i_Stupid)
    {
	RET(false);
    }
    else
    {
	i_Value = value;
	i_Last_Modify_Time = Now();
	i_Last_Modifier = nick;
	RET(true);
    }
}

template<class T> inline
void entlist_val_t<T>::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    entlist_t::EndElement(pIn,pElement);
	if( pElement->IsA(tag_Value) )   pElement->Retrieve(i_Value);
	if( pElement->IsA(tag_Stupid) )   pElement->Retrieve(i_Stupid);
}

template<class T> inline
void entlist_val_t<T>::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
	pOut->BeginObject(tag_entlist_t, attribs);
    entlist_t::WriteElement(pOut,attribs);

	pOut->WriteElement(tag_Value, i_Value);
	pOut->WriteElement(tag_Stupid, i_Stupid);

	pOut->EndObject(tag_entlist_t);
}

template<class T1, class T2>
class entlist_val_t< std::pair<T1,T2> > : public entlist_t
{
#ifdef WIN32
    friend wxOutputStream &operator<<(wxOutputStream& out, const entlist_val_t< std::pair<T1,T2> >& in);
    friend wxInputStream &operator>>(wxInputStream& in, entlist_val_t<  std::pair<T1,T2> >& out);
#else
    friend wxOutputStream &operator<< < std::pair<T1,T2> >(wxOutputStream& out, const entlist_val_t< std::pair<T1,T2> >& in);
    friend wxInputStream &operator>> < std::pair<T1,T2> >(wxInputStream& in, entlist_val_t< std::pair<T1,T2> >& out);
#endif
    std::pair<T1,T2> i_Value;
    bool i_Stupid;	// if TRUE, Value() does nothing.

public:
    entlist_val_t () {}
    entlist_val_t (const entlist_val_t& in) { *this = in; }
    entlist_val_t (mstring entry, std::pair<T1,T2> value, mstring nick, mDateTime modtime = Now(), bool stupid = false);
    void operator=(const entlist_val_t &in);

    bool Value(std::pair<T1,T2> value, mstring nick);
    std::pair<T1,T2> Value()const			{ return i_Value; }

    virtual SXP::Tag& GetClassTag() const { return tag_entlist_val_t; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement) { };
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);
};

template<class T1, class T2>
inline void entlist_val_t< std::pair<T1,T2> >::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    entlist_t::EndElement(pIn,pElement);
	if( pElement->IsA(tag_ValueFirst) )   pElement->Retrieve(i_Value.first);
	if( pElement->IsA(tag_ValueSecond) )   pElement->Retrieve(i_Value.second);
	if( pElement->IsA(tag_Stupid) )   pElement->Retrieve(i_Stupid);
}

template<class T1, class T2>
inline void entlist_val_t< std::pair<T1,T2> >::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
	pOut->BeginObject(tag_entlist_t, attribs);
    entlist_t::WriteElement(pOut,attribs);

	pOut->WriteElement(tag_ValueFirst, i_Value.first);
	pOut->WriteElement(tag_ValueSecond, i_Value.second);
	pOut->WriteElement(tag_Stupid, i_Stupid);

	pOut->EndObject(tag_entlist_t);
}

template<class T> inline
wxOutputStream &operator<<(wxOutputStream& out, const entlist_val_t<T>& in)
{
    out<<in.i_Entry<<in.i_Value<<in.i_Last_Modify_Time<<in.i_Last_Modifier<<in.i_Stupid;

    map<mstring,mstring>::const_iterator j;
    out<<in.i_UserDef.size();
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	out<<(mstring)j->first<<(mstring)j->second;
    return out;
}


template<class T> inline
wxInputStream &operator>>(wxInputStream& in, entlist_val_t<T>& out)
{
    unsigned int i,count;
    mstring dummy,dummy2;

    in>>out.i_Entry>>out.i_Value>>out.i_Last_Modify_Time>>out.i_Last_Modifier>>out.i_Stupid;

    out.i_UserDef.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>dummy>>dummy2;
	out.i_UserDef[dummy]=dummy2;
    }
    return in;
}

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
    mBase();
    static void init();
    static void shutdown();

    virtual void load_database(wxInputStream& in) =0;
    virtual void save_database(wxOutputStream& out) =0;

    static void push_message(const mstring& message);
    virtual void execute(const mstring& message) =0;

    mstring FirstName() { return names.Before(" "); }
    mstring GetNames() { return names; }
    bool IsName(mstring in)
    {
        mstring tmp = " "+names.UpperCase()+" ";
	return tmp.Contains(" "+in.UpperCase()+" ");
    };

    virtual threadtype_enum Get_TType() const =0;
    virtual mstring GetInternalName() const =0;

    virtual bool MSG()		{ return messages; }
    virtual void MSG(bool on)	{ messages=on; } 

    bool signon(const mstring& nickname);
    bool signoff(const mstring& nickname);
    void privmsg(const mstring &source, const mstring &dest, const mstring &pszFormat, ...);
    void privmsg(const mstring &dest, const mstring &pszFormat, ...);
    void privmsgV(const mstring &source, const mstring &dest, const mstring &pszFormat, va_list argptr);
    void notice(const mstring &source, const mstring &dest, const mstring &pszFormat, ...);
    void notice(const mstring &dest, const mstring &pszFormat, ...);
    void noticeV(const mstring &source, const mstring &dest, const mstring &pszFormat, va_list argptr);
    void send(const mstring &source, const mstring &dest, const mstring &pszFormat, ...);
    void send(const mstring &dest, const mstring &pszFormat, ...);
    void sendV(const mstring &source, const mstring &dest, const mstring &pszFormat, va_list argptr);

    operator mVariant() const
    {
	mVariant locvar(GetInternalName());
	locvar.truevaluetype=GetInternalName();
	return locvar;
    };
};

void privmsg(const mstring &source, const mstring &dest, const mstring &pszFormat, ...);
void notice(const mstring &source, const mstring &dest, const mstring &pszFormat, ...);
void send(const mstring &source, const mstring &dest, const mstring &pszFormat, ...);
void announce(const mstring &source, const mstring &pszFormat, ...);

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
    typedef map<mstring, list<triplet<mstring, mstring, functor> > > cmap;
    typedef list<triplet<mstring, mstring, functor> >::iterator clist_iter;
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
	    mstring user);
    pair<bool, functor> GetUserCommand(mstring service, mstring command,
	    mstring user);

    bool DoCommand(mstring mynick, mstring user, mstring command,
	    mstring params);
    bool DoUserCommand(mstring mynick, mstring user, mstring command,
	    mstring params);
    bool DoSystemCommand(mstring mynick, mstring user, mstring command,
	    mstring params);
};
void do_1_2param(mstring mynick, mstring source, mstring params);
void do_1_3param(mstring mynick, mstring source, mstring params);

#endif

