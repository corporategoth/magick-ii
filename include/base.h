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

    virtual void load_database(void) =0;
    virtual void save_database(void) =0;

    static void push_message(const mstring& message);
    virtual void execute(const mstring& message) =0;

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

    void privmsg(const mstring& source, mstring message);
    void notice(const mstring& source, mstring message);

    operator mVariant() const
    {
	mVariant locvar(GetInternalName());
	locvar.truevaluetype=GetInternalName();
	return locvar;
    };
    static void shutdown();
};

#endif