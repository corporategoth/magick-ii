#include "pch.h"
#ifdef _MSC_VER
#pragma hdrstop
#endif
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
#include "base.h"
#include "lockable.h"
#include "magick.h"


bool mBase::TaskOpened;
mBaseTask mBase::BaseTask;

mBase::mBase()
{
    NFT("mBase::mBase");
    TaskOpened=false;
}

void mBase::push_message(const mstring& message)
{
    FT("mBase::push_message", (message));
    if(TaskOpened==false)
    {
	if(BaseTask.open()!=0)
	{
	    CP(("Failed to create initial thread"));
	    return;
	}
    }
    CH(T_Chatter::From,message);
    BaseTask.message(message);
}

void mBase::init()
{
    NFT("mBase::init");

    if(TaskOpened==false)
    {
	if(BaseTask.open()!=0)
	{
	    CP(("Failed to create initial thread"));
	    return;
	}
    }
    BaseTask.message_queue_.high_water_mark(Parent->high_water_mark*sizeof(ACE_Method_Object *));
    BaseTask.message_queue_.low_water_mark(BaseTask.message_queue_.high_water_mark());
}

void mBase::send_cmd(const mstring & source, const char *fmt, ...)
{
    FT("send_cmd", (source, fmt));
    va_list args;
    va_start(args,fmt);
    //if(runflags & RUN_NOSEND)
	//return;
    mstring data1;
    data1.FormatV(fmt,args);
    va_end(args);

    if(source!="")
    {
    }
    else
    {
    }
}

void mBase::shutdown()
{
    NFT("mBase::shutdown");
    int j=BaseTask.thr_count();
    for(int i=0;i<j;i++)
	BaseTask.i_shutdown();
}

int mBaseTask::open(void *in)
{
    NFT("mBaseTask::open");
    mBase::TaskOpened=true;
    RET(activate());
}

int mBaseTask::svc(void)
{
    NFT("mBaseTask::svc");
    mThread::Attach(tt_mBase);
    while(Parent->shutdown()==false)
    {
	auto_ptr<ACE_Method_Object> mo(this->activation_queue_.dequeue());
	if(mo->call()==-1)
	    break;
    }
    mThread::Detach(tt_mBase);
    RET(0);
}

class mBaseTaskmessage_MO : public ACE_Method_Object
{
public:
    mBaseTaskmessage_MO(mBaseTask *parent, const mstring& data)
    {
	FT("mBaseTaskmessage_MO::mBaseTaskmessage_MO",((void *)parent,data));
	i_parent=parent;
	i_data=data;
    }
    virtual int call()
    {
	NFT("mBaseTaskmessage_MO::call");
	i_parent->message_i(i_data);
	RET(0);
    }
private:
    mBaseTask *i_parent;
    mstring i_data;
};

void mBaseTask::message(const mstring& message)
{
    FT("mBaseTask::message",(message));
    if(message_queue_.is_full())
    {
	message_queue_.high_water_mark(Parent->high_water_mark*(thr_count()+1)*sizeof(ACE_Method_Object *));
	message_queue_.low_water_mark(message_queue_.high_water_mark());
	if(activate(THR_NEW_LWP | THR_JOINABLE, 1, 1)!=0)
	    CP(("Couldn't start new thread to handle excess load, will retry next message"));
    }
    activation_queue_.enqueue(new mBaseTaskmessage_MO(this,message));
}

void mBaseTask::message_i(const mstring& message)
{
    FT("mBaseTask::message_i",(message));
    // NOTE: No need to handle 'non-user messages' here, because
    // anything that is not a user PRIVMSG/NOTICE goes directly
    // to the server routine anyway.
    mstring tmp[2];
    tmp[0]=message.ExtractWord(2,": ").UpperCase();
    tmp[1]=message.ExtractWord(3,": ").UpperCase();

    CH(T_Chatter::From,message);	    
    // check if on ignore list and throw to the "ignore logging service" if log ignored user commands is on.
    // maybe we should have a hit count for logging? x ignores in x minutes = start logging that sucker. 

    if (tmp[0] == "PRIVMSG" || tmp[0] == "NOTICE")
    {
	mstring names;
	if (Parent->nickserv.IsLive(message.ExtractWord(1, ": ")))
	    Parent->nickserv.live[message.ExtractWord(1, ": ")].FloodTrigger();

	// Find out if the target nick is one of the services 'clones'
	// (and if it is, which one?)  Pass the message to them if so.
	// before even that, check if it's script overriden via Parent->checkifhandled(servername,command)
	// if so, Parent->doscripthandle(server,command,data);
	if ((names=" "+Parent->operserv.getnames().UpperCase()+" ").Contains(" "+tmp[1]+" "))
	    Parent->operserv.execute(message);
	else if ((names=" "+Parent->nickserv.getnames().UpperCase()+" ").Contains(" "+tmp[1]+" "))
	    Parent->nickserv.execute(message);
	else if ((names=" "+Parent->chanserv.getnames().UpperCase()+" ").Contains(" "+tmp[1]+" "))
	    Parent->chanserv.execute(message);
	else if ((names=" "+Parent->memoserv.getnames().UpperCase()+" ").Contains(" "+tmp[1]+" "))
	    Parent->memoserv.execute(message);
	else if ((names=" "+Parent->helpserv.getnames().UpperCase()+" ").Contains(" "+tmp[1]+" "))
	    Parent->helpserv.execute(message);

	// else check if it's script handled, might do up a list of script servers
	// in the magick object to check against, else trash it.

	// Not a MSG/NOTICE to services, fall through
	// (it could be to a channel, or unrecognised nick).
	else
	    Parent->server.execute (message);
    } 
    else    // This handles all non-msgs/notices.
	Parent->server.execute (message);

    /* Locking here because we dont want two threads
     * dieing because they both counted at the same
     * time and before the other had died.  Unlikely,
     * but it CAN happen.
     */
    {
	MLOCK("thread_handler", "LWM");
	if(thr_count()>1&&message_queue_.message_count()<
	    Parent->high_water_mark*(thr_count()-2)+Parent->low_water_mark)
	{
	    message_queue_.high_water_mark(Parent->high_water_mark*(ACE_Thread_Manager::instance()->count_threads()-1)*sizeof(ACE_Method_Object *));
	    message_queue_.low_water_mark(message_queue_.high_water_mark());
	    COM(("Low water mark reached, killing thread."));
	    i_shutdown();
	}
    }
}

void mBaseTask::i_shutdown()
{
    NFT("mBaseTask::message_i");
    activation_queue_.enqueue(new shutdown_MO);
}
