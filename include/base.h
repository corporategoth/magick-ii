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
#ifndef _base_h
#define _base_h
#include "mstring.h"
#include "trace.h"
#include "dccengine.h"
#include "utils.h"

class mUserDef
{
protected:
    map<mstring,mstring> i_UserDef;
public:
    mstring UserDef(mstring type);
    mstring UserDef(mstring type, mstring val);
};

class mBaseTask : public ACE_Task<ACE_MT_SYNCH>
{
    friend class mBase;
    friend class mBaseTaskmessage_MO;
protected:
    ACE_Activation_Queue activation_queue_;
    ACE_Message_Queue<ACE_MT_SYNCH> message_queue_;
    void message_i(const mstring& message);
public:
    mBaseTask() : activation_queue_(&message_queue_) {};
    virtual int open(void *in=0);
    virtual int svc(void);
    void message(const mstring& message);
    void i_shutdown();
};

class entlist_t : public mUserDef
{
    friend wxOutputStream &operator<<(wxOutputStream& out,const entlist_t& in);
    friend wxInputStream &operator>>(wxInputStream& in, entlist_t& out);
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
};
wxOutputStream &operator<<(wxOutputStream& out,const entlist_t& in);
wxInputStream &operator>>(wxInputStream& in, entlist_t& out);
typedef list<entlist_t>::iterator entlist_i;
typedef list<entlist_t>::const_iterator entlist_ci;
typedef set<entlist_t>::iterator entlist_ui;
typedef set<entlist_t>::const_iterator entlist_cui;

template<class T>
class entlist_val_t : public mUserDef
{
#ifdef WIN32
    friend wxOutputStream &operator<<(wxOutputStream& out,const entlist_val_t<T>& in);
    friend wxInputStream &operator>>(wxInputStream& in, entlist_val_t<T>& out);
#else
    friend wxOutputStream &operator<<<T>(wxOutputStream& out,const entlist_val_t<T>& in);
    friend wxInputStream &operator>><T>(wxInputStream& in, entlist_val_t<T>& out);
#endif
    mstring i_Entry;
    T i_Value;
    mDateTime i_Last_Modify_Time;
    mstring i_Last_Modifier;
    bool i_Stupid;	// if TRUE, Value() does nothing.
public:
    entlist_val_t () {}
    entlist_val_t (const entlist_val_t& in) { *this = in; }
    entlist_val_t (mstring entry, T value, mstring nick, mDateTime modtime = Now(), bool stupid = false);
    void operator=(const entlist_val_t &in);
    bool operator==(const entlist_val_t &in) const
	{ return (i_Entry == in.i_Entry); }
    bool operator!=(const entlist_val_t &in) const
	{ return (i_Entry != in.i_Entry); }
    bool operator<(const entlist_val_t &in) const
	{ return (i_Entry < in.i_Entry); }

    bool Value(T value, mstring nick);
    mstring Entry()const		{ return i_Entry; }
    T Value()const			{ return i_Value; }
    mDateTime Last_Modify_Time()const	{ return i_Last_Modify_Time; }
    mstring Last_Modifier()const	{ return i_Last_Modifier; }
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

class mBase
{
    friend mBaseTask;

protected:
    mstring names;		// Names of service (space delimited)
    mstring realname;		// 'Real Name' of service

    bool messages;		// Wether to process /MSG, /NOTICE.
    bool automation;		// Wether to do automatic tasks.

    //deque<pair<mstring,mstring> > inputbuffer; // pair of sentto,datastring
    static bool TaskOpened;
    static mBaseTask BaseTask;
    virtual void AddCommands() {};
    virtual void RemCommands() {};

public:
    mBase();
    static void init();

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
    virtual bool AUTO()		{ return automation; }
    virtual void AUTO(bool on)	{ automation=on; }

    bool signon(const mstring& nickname);
    bool signoff(const mstring& nickname);
    void privmsg(const mstring& source, const mstring &dest, const mstring &message);
    void privmsg(const mstring &dest, const mstring &message); // FIRST nick
    void notice(const mstring& source, const mstring &dest, const mstring &message);
    void notice(const mstring &dest, const mstring &message); // FIRST nick
    void send(const mstring& source, const mstring &dest, const mstring &message);
    void send(const mstring &dest, const mstring &message); // FIRST nick

    operator mVariant() const
    {
	mVariant locvar(GetInternalName());
	locvar.truevaluetype=GetInternalName();
	return locvar;
    };
    static void shutdown();
};

void privmsg(const mstring& source, const mstring &dest, const mstring &message);
void notice(const mstring& source, const mstring &dest, const mstring &message);
void send(const mstring& source, const mstring &dest, const mstring &message);


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
