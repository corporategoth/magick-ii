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
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#ifndef _BASE_H
#define _BASE_H
#include "pch.h"
RCSID(base_h, "@(#) $Id$");

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

#include "lockable.h"
#include "utils.h"
#include "xml/sxp.h"

class mUserDef
{
protected:
    mutable vector < mstring * > ud_array;
    mutable map < mstring, mstring > i_UserDef;

public:
    virtual ~ mUserDef()
    {
    }
    mstring UserDef(const mstring & type) const
    {
	FT("mUserDef::UserDef", (type));
	map < mstring, mstring >::const_iterator iter = i_UserDef.find(type.LowerCase());
	if (iter != i_UserDef.end())
	{
	    RET(iter->second);
	}
	RET("");
    }
    void UserDef(const mstring & type, const mstring & val)
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
    friend class IrcSvcHandler;

    unsigned long msgid_;
    mstring source_, msgtype_, params_;
    mDateTime creation_;

public:
    enum type_t
    { ServerExists, ServerNoExists, NickExists, NickNoExists,
	ChanExists, ChanNoExists, UserInChan, UserNoInChan
    };

private:
    static map < type_t, map < mstring, set < unsigned long > > > AllDependancies;
    static map < unsigned long, mMessage * > MsgIdMap;
    static unsigned long LastMsgId;

    list < triplet < type_t, mstring, bool > > dependancies;
    void AddDepend(const type_t type, const mstring & param)
    {
	dependancies.push_back(triplet < type_t, mstring, bool > (type, param, false));
    }
    void AddDependancies();

public:
    mMessage(const mstring & source, const mstring & msgtype, const mstring & params, const u_long priority =
	     static_cast < u_long > (P_Normal));

    bool OutstandingDependancies();
    static void CheckDependancies(type_t type, const mstring & param1, const mstring & param2 = "");
    bool RecheckDependancies();
    void DependancySatisfied(type_t type, const mstring & param);

    unsigned long msgid()
    {
	return msgid_;
    }
    mstring source()
    {
	return source_;
    }
    mstring msgtype()
    {
	return msgtype_;
    }
    mstring params()
    {
	return params_;
    }
    mDateTime creation()
    {
	return creation_;
    }

    virtual int call();
};

class mBase
{
    friend class EventTask;

protected:
    mstring names;		// Names of service (space delimited)
    mstring realname;		// 'Real Name' of service

    bool messages;		// Wether to process /MSG, /NOTICE.

    //deque<pair<mstring,mstring> > inputbuffer; // pair of sentto,datastring
    static bool TaskOpened;
    virtual void AddCommands()
    {
    };
    virtual void RemCommands()
    {
    };

public:
    mBase()
    {
    }
    virtual ~ mBase()
    {
    }

    virtual void execute(mstring & source, const mstring & msgtype, const mstring & params) = 0;
    virtual mstring FirstName() const
    {
	return names.Before(" ");
    }
    virtual mstring GetNames() const
    {
	return names;
    }
    virtual bool IsName(mstring in) const
    {
	mstring tmp(" " + names.UpperCase() + " ");

	return tmp.Contains(" " + in.UpperCase() + " ");
    };

    virtual threadtype_enum Get_TType() const = 0;
    virtual mstring GetInternalName() const = 0;

    virtual bool MSG() const
    {
	return messages;
    }
    virtual void MSG(const bool on)
    {
	messages = on;
    }

    virtual bool signon(const mstring & nickname) const;
    virtual bool signoff(const mstring & nickname, const mstring & msg = "") const;
    virtual void privmsgV(const mstring & dest, const char *pszFormat, ...) const;
    virtual void privmsgV(const mstring & source, const mstring & dest, const char *pszFormat, ...) const;
    virtual void privmsg(const mstring & dest, const mstring & message) const;
    virtual void privmsg(const mstring & source, const mstring & dest, const mstring & message) const;
    virtual void noticeV(const mstring & dest, const char *pszFormat, ...) const;
    virtual void noticeV(const mstring & source, const mstring & dest, const char *pszFormat, ...) const;
    virtual void notice(const mstring & dest, const mstring & message) const;
    virtual void notice(const mstring & source, const mstring & dest, const mstring & message) const;
    virtual void sendV(const mstring & dest, const char *pszFormat, ...) const;
    virtual void sendV(const mstring & source, const mstring & dest, const char *pszFormat, ...) const;
    virtual void send(const mstring & dest, const mstring & message) const;
    virtual void send(const mstring & source, const mstring & dest, const mstring & message) const;

    virtual operator      mVariant() const
    {
	mVariant locvar(GetInternalName());

	locvar.truevaluetype = GetInternalName();
	return locvar;
    };
};

void privmsgV(const mstring & source, const mstring & dest, const char *pszFormat, ...);
void privmsg(const mstring & source, const mstring & dest, const mstring & message);
void noticeV(const mstring & source, const mstring & dest, const char *pszFormat, ...);
void notice(const mstring & source, const mstring & dest, const mstring & message);
void sendV(const mstring & source, const mstring & dest, const char *pszFormat, ...);
void send(const mstring & source, const mstring & dest, const mstring & message);
void announceV(const mstring & source, const char *pszFormat, ...);
void announce(const mstring & source, const mstring & message);

#define SEND(W, X, Y, Z) \
	::send(W, X, parseMessage(Magick::instance().getMessage(X, Y), mVarArray Z))
#define NSEND(W, X, Y) \
	::send(W, X, parseMessage(Magick::instance().getMessage(X, Y), mVarArray::EmptyArray()))
#define ANNOUNCE(X, Y, Z) \
	::announce(X, parseMessage(Magick::instance().getMessage(Y), mVarArray Z))
#define NANNOUNCE(X, Y) \
	::announce(X, parseMessage(Magick::instance().getMessage(Y), mVarArray::EmptyArray()))

extern SXP::Tag tag_entlist_val_t;
extern SXP::Tag tag_entlist_t;
extern SXP::Tag tag_Entry, tag_Value, tag_Last_Modify_Time, tag_Last_Modifier, tag_UserDef, tag_Stupid;
extern SXP::Tag tag_ValueFirst, tag_ValueSecond;

class entlist_t : public mUserDef, public SXP::IPersistObj
{
protected:
    mstring i_Entry;
    mDateTime i_Last_Modify_Time;
    mstring i_Last_Modifier;

public:
    entlist_t()
    {
    }
    entlist_t(const entlist_t & in) : mUserDef(in), SXP::IPersistObj(in)
    {
	*this = in;
    }
    entlist_t(const mstring & entry, const mstring & nick, const mDateTime & modtime =
	      mDateTime::CurrentDateTime()) : i_Entry(entry), i_Last_Modify_Time(modtime), i_Last_Modifier(nick)
    {
	FT("entlist_t::entlist_t", (entry, nick, modtime));
    }

    virtual ~ entlist_t()
    {
    }
    entlist_t &operator=(const entlist_t & in);
    virtual bool operator==(const entlist_t & in) const
    {
	return (i_Entry == in.i_Entry);
    }
    virtual bool operator!=(const entlist_t & in) const
    {
	return (i_Entry != in.i_Entry);
    }
    virtual bool operator<(const entlist_t & in) const
    {
	return (i_Entry < in.i_Entry);
    }

    virtual mstring Entry() const
    {
	return i_Entry;
    }
    virtual mDateTime Last_Modify_Time() const
    {
	return i_Last_Modify_Time;
    }
    virtual mstring Last_Modifier() const
    {
	return i_Last_Modifier;
    }

    // XML handling section
    virtual SXP::Tag & GetClassTag() const
    {
	return tag_entlist_t;
    }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);
    virtual void PostLoad() const;

    virtual size_t Usage() const;
    virtual void DumpB() const;
    virtual void DumpE() const;
};
typedef list < entlist_t >::iterator entlist_i;
typedef list < entlist_t >::const_iterator entlist_ci;
typedef set < entlist_t >::iterator entlist_ui;
typedef set < entlist_t >::const_iterator entlist_cui;

template < class T > class entlist_val_t : public entlist_t
{
protected:
    T i_Value;
    bool i_Stupid;		// if TRUE, Value() does nothing.

public:
    entlist_val_t()
    {
    }
    entlist_val_t(const entlist_val_t < T > & in) : entlist_t(in)
    {
	*this = in;
    }
    entlist_val_t(const mstring & entry, const T & value, const mstring & nick, const mDateTime & modtime =
		  mDateTime::CurrentDateTime(), const bool stupid =
		  false) : entlist_t(entry, nick, modtime), i_Value(value), i_Stupid(stupid)
    {
	FT("entlist_val_t<T>::entlist_val_t", (entry, "(T) value", nick, modtime, stupid));
    }
    virtual ~ entlist_val_t()
    {
    }
    virtual entlist_val_t < T > & operator=(const entlist_val_t < T > & in)
    {
	FT("entlist_val_t<T>::operator=", ("(const entlist_val_t<T> &) in"));
	entlist_t::operator=(in);
	i_Value = in.i_Value;
	i_Stupid = in.i_Stupid;
	NRET(entlist_val_t < T > &, *this);
    }

    virtual bool Value(const T & value, const mstring & nick)
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
    virtual T Value() const
    {
	return i_Value;
    }

    virtual SXP::Tag & GetClassTag() const
    {
	return tag_entlist_val_t;
    };
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
    {
	FT("entlist_val_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
	entlist_t::BeginElement(pIn, pElement);
    }
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
    {
	FT("entlist_val_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
	entlist_t::EndElement(pIn, pElement);
	if (pElement->IsA(tag_Value))
	    pElement->Retrieve(i_Value);
	if (pElement->IsA(tag_Stupid))
	    pElement->Retrieve(i_Stupid);
    }
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict)
    {
	FT("entlist_val_t::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::Dict &) attribs"));
	pOut->BeginObject(tag_entlist_val_t);
	entlist_t::WriteElement(pOut, attribs);

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

template < class X, class Y > class entlist_val_pair_t : public entlist_t
{
protected:
    pair < X, Y > i_Value;
    bool i_Stupid;		// if TRUE, Value() does nothing.

public:
    entlist_val_pair_t()
    {
    }
    entlist_val_pair_t(const entlist_val_pair_t < X, Y > & in) : entlist_t(in)
    {
	*this = in;
    }
    entlist_val_pair_t(const mstring & entry, const pair < X, Y > & value, const mstring & nick, const mDateTime & modtime =
		       mDateTime::CurrentDateTime(), const bool stupid =
		       false) : entlist_t(entry, nick, modtime), i_Value(value), i_Stupid(stupid)
    {
	FT("entlist_val_pair_t< pair<X, Y> >::entlist_val_pair_t", (entry, "( pair<X,Y> ) value", nick, modtime, stupid));
    }
    virtual ~ entlist_val_pair_t()
    {
    }
    virtual entlist_val_pair_t < X, Y > & operator=(const entlist_val_pair_t < X, Y > & in)
    {
	FT("entlist_val_pair_t< pair<X, X> >::operator=", ("(const entlist_val_pair_t< pair<X,Y> > &) in"));
	entlist_t::operator=(in);
	i_Value = in.i_Value;
	i_Stupid = in.i_Stupid;
	NRET(entlist_val_pair_t < X_Y > &, *this);
    }

    virtual bool Value(pair < X, Y > & value, const mstring & nick)
    {
	FT("entlist_val_pair_t< pair<X,Y> >::Change", ("(pair<X,Y>) value", nick));
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
    pair < X, Y > Value() const
    {
	return i_Value;
    }

    virtual SXP::Tag & GetClassTag() const
    {
	return tag_entlist_val_t;
    }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
    {
	FT("entlist_val_pair_t< pair<X,Y> >::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
	entlist_t::BeginElement(pIn, pElement);
    }
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
    {
	FT("entlist_val_pair_t< pair<X,Y> >::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
	entlist_t::EndElement(pIn, pElement);
	if (pElement->IsA(tag_ValueFirst))
	    pElement->Retrieve(i_Value.first);
	if (pElement->IsA(tag_ValueSecond))
	    pElement->Retrieve(i_Value.second);
	if (pElement->IsA(tag_Stupid))
	    pElement->Retrieve(i_Stupid);
    }
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict)
    {
	FT("entlist_val_pairt< pair<X,Y> >::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::Dict &) attribs"));
	pOut->BeginObject(tag_entlist_val_t);
	entlist_t::WriteElement(pOut, attribs);

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
    typedef void (*functor) (const mstring &, const mstring &, const mstring &);

    // map<service, map<command, pair<committees, functor> > >
    typedef list < triplet < mstring, mstring, functor > > cmdtype;
    typedef map < mstring, cmdtype > cmdmap;
    cmdmap i_user;
    cmdmap i_system;

public:
    void AddSystemCommand(const mstring & service, const mstring & command, const mstring & committees, functor function);
    void RemSystemCommand(const mstring & service, const mstring & command, const mstring & committees);
    void AddCommand(const mstring & service, const mstring & command, const mstring & committees, functor function);
    void RemCommand(const mstring & service, const mstring & command, const mstring & committees);
    pair < bool, functor > GetSystemCommand(const mstring & service, const mstring & command, const mstring & user) const;
    pair < bool, functor > GetUserCommand(const mstring & service, const mstring & command, const mstring & user) const;

    bool DoCommand(const mstring & mynick, const mstring & user, const mstring & command, const mstring & params) const;
    bool DoUserCommand(const mstring & mynick, const mstring & user, const mstring & command, const mstring & params) const;
    bool DoSystemCommand(const mstring & mynick, const mstring & user, const mstring & command, const mstring & params) const;
};
void do_1_2param(const mstring & mynick, const mstring & source, const mstring & params);
void do_1_3param(const mstring & mynick, const mstring & source, const mstring & params);
void do_1_4param(const mstring & mynick, const mstring & source, const mstring & params);
void do_1_2paramswap(const mstring & mynick, const mstring & source, const mstring & params);
void do_1_3paramswap(const mstring & mynick, const mstring & source, const mstring & params);
void do_1_4paramswap(const mstring & mynick, const mstring & source, const mstring & params);
void do_2param(const mstring & mynick, const mstring & source, const mstring & params);
void do_3param(const mstring & mynick, const mstring & source, const mstring & params);
void do_4param(const mstring & mynick, const mstring & source, const mstring & params);

#endif
