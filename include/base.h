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
    entlist_t (mstring entry, mstring nick);
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
    template<T>
    friend wxOutputStream &operator<<(wxOutputStream& out,const entlist_val_t<T>& in);
    template<T>
    friend wxInputStream &operator>>(wxInputStream& in, entlist_val_t<T>& out);
    mstring i_Entry;
    T i_Value;
    mDateTime i_Last_Modify_Time;
    mstring i_Last_Modifier;
    bool i_Stupid;	// if TRUE, Value() does nothing.
public:
    entlist_val_t () {}
    entlist_val_t (const entlist_val_t& in) { *this = in; }
    entlist_val_t (mstring entry, T value, mstring nick, bool stupid = false);
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

template<class T>
wxOutputStream &operator<<(wxOutputStream& out,const entlist_val_t<T>& in);
template<class T>
wxInputStream &operator>>(wxInputStream& in, entlist_val_t<T>& out);
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
    void privmsg(const mstring source, mstring dest, mstring message);
    void privmsg(mstring dest, mstring message); // FIRST nick
    void notice(const mstring& source, mstring dest, mstring message);
    void notice(mstring dest, mstring message); // FIRST nick

    operator mVariant() const
    {
	mVariant locvar(GetInternalName());
	locvar.truevaluetype=GetInternalName();
	return locvar;
    };
    static void shutdown();
};

#endif
