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

class Magick;

// fucking hell, the parent causes major breakage, gotta turn this into
// an ACE_Task active object.

class mBaseTask : public ACE_Task<ACE_MT_SYNCH>
{
    friend class mBase;
    friend class mBaseTaskmessage_MO;
protected:
    ACE_Activation_Queue activation_queue_;
    ACE_Message_Queue<ACE_MT_SYNCH> message_queue_;
    Magick *Parent;
    void message_i(const mstring& message);
public:
    mBaseTask() : activation_queue_(&message_queue_) {};
    virtual int open(void *in=0);
    virtual int svc(void);
    void message(const mstring& message);
    void shutdown();
};


class mBase
{
    friend void *thread_handler(void *dummyvar);
protected:
    //deque<pair<mstring,mstring> > inputbuffer; // pair of sentto,datastring
    static Magick *Parent;
    static bool TaskOpened;
    static mBaseTask BaseTask;
public:
	void send_cmd(const mstring& source, const mstring& fmt, ...);
    mBase(Magick *in_Parent);
    static void push_message(const mstring& message);
    static void init(Magick *in);

    virtual bool MSG() =0;
    virtual void MSG(bool on) =0;
    virtual bool AUTO() =0;
    virtual void AUTO(bool on) =0;

    virtual void execute(const mstring& message)=0;
    virtual threadtype_enum Get_TType() const=0;
    //virtual mBase *GetOwner()=0;
    virtual mstring GetInternalName() const=0;
    operator mVariant() const { mVariant locvar(GetInternalName()); locvar.truevaluetype=GetInternalName(); return locvar; };
    static void shutdown();
  
};

class NetworkServ : public mBase
{
    friend class Magick;
private:

    // Config Entries ...
    mstring names;		// Names of service (space delimited)
    mstring realname;		// 'Real Name' of service

    // Live Flags
    bool messages;		// Wether to process /MSG, /NOTICE.
    bool automation;		// Wether to do automatic tasks.
protected:

public:
    mstring getnames() { return names; }

    bool MSG() { return messages; }
    void MSG(bool on) { messages = on; }
    bool AUTO() { return automation; }
    void AUTO(bool on) { automation = on; }

    NetworkServ(Magick *in_Parent);
    virtual threadtype_enum Get_TType() const { return tt_ServNet; }
    virtual mstring GetInternalName() const { return "NetworkServ"; }
    void execute(const mstring & message);
};

#endif