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
    //deque<pair<mstring,mstring> > inputbuffer; // pair of sentto,datastring
    static bool TaskOpened;
    static mBaseTask BaseTask;
public:
	void send_cmd(const mstring& source, const char *fmt, ...);
    mBase();
    static void push_message(const mstring& message);
    static void init();

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

#endif