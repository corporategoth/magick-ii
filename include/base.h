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
#include <algorithm>
#include <queue>
#include <utility>
#include <ace/Message_Queue.h>
#include <ace/Singleton.h>
#include <ace/Synch_T.h>
using namespace std;

class mBase
{
    friend void *thread_handler(void *dummyvar);
protected:
    //deque<pair<mstring,mstring> > inputbuffer; // pair of sentto,datastring
public:
    static map<unsigned long,mstring > Buffer_Tuple;
    static ACE_Message_Queue<ACE_MT_SYNCH> Buffer_Queue;
    //static ACE_Singleton<Buffer_Queue, ACE_Mutex> Request_Queue;
    // todo, create a static map of message_id's and buffer_tuple's. message_id get's put on the message_queue, 
    // which then is used to pull the message off the queue, this looks to be the easiest way to use
    // message_queue'ing with variable sized messages.
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
    
};
#endif