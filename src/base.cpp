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
#include "log.h"

#if 0
map<unsigned long,mstring > mBase::Buffer_Tuple;
ACE_Message_Queue<ACE_MT_SYNCH> mBase::Buffer_Queue;
#endif

Magick *mBase::Parent;
bool mBase::TaskOpened;
mBaseTask mBase::BaseTask;

mBase::mBase(Magick* in_Parent)
{
    NFT("mBase::mBase");
    Parent=in_Parent;
    TaskOpened=false;
}

void mBase::push_message(const mstring& message)
{
    FT("mBase::push_message@mBase", (message));
    if(TaskOpened==false)
    {
	if(Parent!=NULL&&BaseTask.open(Parent)!=0)
	{
	    CP(("Failed to create initial thread"));
	    return;
	}
    }
    BaseTask.message(message);


#if 0
    static unsigned long counter=0;
    // Braces, so that if we spawn/resume a thread, we make sure it can process
    {
	WLOCK lock("mBase","inputbuffer");
	counter++;
        {
	    WLOCK("thread_handler", "pop_message");
	    Buffer_Tuple[counter]=message;
	}
	//Buffer_Queue
	if(Buffer_Queue.is_full())
	{
	    // set new water marks and spawn off a new thread.
	    Buffer_Queue.high_water_mark(Parent->high_water_mark*(ACE_Thread_Manager::instance()->count_threads())*sizeof(unsigned long));
	    // below is set to the same as high_water_mark so that Buffer_Queue, doesn't block on add.
	    Buffer_Queue.low_water_mark(Buffer_Queue.high_water_mark());
	    if(ACE_Thread_Manager::instance()->spawn(thread_handler)==-1) {
		CP(("High water mark reached, failed to create a new thread"));
	    } else {
		COM(("High water mark reached, creating new thread"));
	    }
	}
	ACE_Message_Block *mb=new ACE_Message_Block();
	//todo: enqueue_prio
	mb->copy((const char *)&counter,sizeof(unsigned long));
	Buffer_Queue.enqueue(mb);
    }
#endif
}

void mBase::init(Magick *in)
{
    NFT("mBase::init");

    if(TaskOpened==false)
    {
	if(BaseTask.open((void *)in)!=0)
	{
	    CP(("Failed to create initial thread"));
	    return;
	}
    }
    BaseTask.message_queue_.high_water_mark(Parent->high_water_mark*sizeof(ACE_Method_Object *));
    BaseTask.message_queue_.low_water_mark(BaseTask.message_queue_.high_water_mark());
#if 0
    //mThread::spawn(Get_TType(),thread_handler,(void *)this);
    //todo: setup the first high water mark and first low water mark
    //and spawn off x threads.
    Buffer_Queue.high_water_mark(Parent->high_water_mark*sizeof(unsigned long));
    // below is set to the same as high_water_mark so that Buffer_Queue, doesn't block on add.
    Buffer_Queue.low_water_mark(Buffer_Queue.high_water_mark());
    if(ACE_Thread_Manager::instance()->spawn(thread_handler)==-1)
	CP(("Failed to create initial thread"));
#endif
}

void mBase::send_cmd(const mstring & source, const mstring & fmt, ...)
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
}

#if 0
void *mBase::thread_handler(void *owner)
{
    NFT("thread_handler");
    //todo ugh, major revamp Owner will be unknown till we figure out who
    // the thread is to go to (via the message)
    // we then execute the message, finally we (locked) check if we are below
    // low water mark, if so, then we return.
    // if Owner->MSG() == false then we toss message, else we execute it.
    while(Parent->shutdown()==false)
    {
	COM(("Attempting to read another thread ..."));
        ACE_Message_Block *mb;
	mBase::Buffer_Queue.dequeue(mb);
	if(mb!=NULL)
	{
	    unsigned long messageid=*(mb->rd_ptr());
	    delete mb; // as it's no longer needed once we get the message id.
	    mstring message;
	    {
		WLOCK("thread_handler", "pop_message");
		message=mBase::Buffer_Tuple[messageid];
		mBase::Buffer_Tuple.erase(messageid);
	    }

	    mstring tmp[2];
	    if(message[0u]==':')
	    {
		// with from prefix
		tmp[0]=message.ExtractWord(2,": ").UpperCase();
		tmp[1]=message.ExtractWord(3,": ");
	    }
	    else
	    {
		// no from prefix
		tmp[0]=message.ExtractWord(1,": ").UpperCase();
		tmp[1]=message.ExtractWord(2,": ");
	    }
	    
	    // check if on ignore list and throw to the "ignore logging service" if log ignored user commands is on.
	    // maybe we should have a hit count for logging? x ignores in x minutes = start logging that sucker. 

	    if (tmp[0] == "PRIVMSG" || tmp[0] == "NOTICE") {
		mstring names;

		// Find out if the target nick is one of the services 'clones'
		// (and if it is, which one?)  Pass the message to them if so.
//		     if ((names=" "+OperServ::getnames()+" ").Find(" "+tmp[1]+" "))
//		    Parent->operserv.execute(message);
		     if ((names=" "+Parent->nickserv.getnames()+" ").Find(" "+tmp[1]+" "))
		    Parent->nickserv.execute(message);
		else if ((names=" "+Parent->chanserv.getnames()+" ").Find(" "+tmp[1]+" "))
		    Parent->chanserv.execute(message);
//		else if ((names=" "+MemoServ::getnames()+" ").Find(" "+tmp[1]+" "))
//		    Parent->memoserv.execute(message);
//		else if ((names=" "+HelpServ::getnames()+" ").Find(" "+tmp[1]+" "))
//		    Parent->helpserv.execute(message);

		// How do we want to handle custom services (BOB created)?

		// Not a MSG/NOTICE to services, fall through
		// (it could be to a channel, or unrecognised nick).
		else
		    Parent->Server.execute (message);

	    } else {
		// This handles all non-msgs/notices.
		Parent->Server.execute (message);
	    }

	    /* Locking here because we dont want two threads
	     * dieing because they both counted at the same
	     * time and before the other had died.  Unlikely,
	     * but it CAN happen.
	     */
	    {
		MLOCK("thread_handler", "LWM");
		if(mBase::Buffer_Queue.message_count()<
			Parent->high_water_mark*(ACE_Thread_Manager::instance()->count_threads()-2)*sizeof(unsigned long)+Parent->low_water_mark*sizeof(unsigned long))
		{
		    mBase::Buffer_Queue.high_water_mark(Parent->high_water_mark*(ACE_Thread_Manager::instance()->count_threads()-1)*sizeof(unsigned long));
		    mBase::Buffer_Queue.low_water_mark(mBase::Buffer_Queue.high_water_mark());
		    COM(("Low water mark reached, killing thread."));
		    return NULL;
		}
	    }
	}
    }
    CP(("Shutdown code received, killing thread."));
    return NULL;
}
#endif

NetworkServ::NetworkServ(Magick *in_Parent) : mBase(in_Parent)
{
    NFT("NetworkServ::NetworkServ");
    if (mThread::findbytype(Get_TType(), 1) == NULL) {
	messages=true;
	automation=true;
    }
}

void NetworkServ::execute(const mstring & data)
{
    FT("NetworkServ::execute", (data));
    //okay this is the main networkserv command switcher

    mstring source, msgtype, msgdata;
    if(data[0u]==':')
    {
	source=data.ExtractWord(1,": ");
	msgtype=data.ExtractWord(2,": ").UpperCase();
	msgdata=data.After(":",2);
    }
    else
    {
	source="";
	msgtype=data.ExtractWord(1,": ").UpperCase();
	msgdata=data.After(":");
    }
    if(msgtype=="PING")
    {
	if(msgdata!="")
	{
	    if(msgdata.WordCount(" ")>1)
		; //send_cmd(server_name,"PONG %s %s",msgdata.ExtractWord(2," "),msgdata.ExtractWord(1," "));
	    else
		; //send_cmd(server_name,"PONG %s %s",server_name,msgdata.ExtractWord(1," "));
	}
	else
	{
	    // error out?
	}
    }
	
}

int mBaseTask::open(void *in)
{
    Parent=(Magick *)in;
    return activate(/* todo enter the number of initial threads here*/);
}

int mBaseTask::svc(void)
{
    while(Parent->shutdown()==false)
    {
	auto_ptr<ACE_Method_Object> mo(this->activation_queue_.dequeue());
	if(mo->call()==-1)
	    break;
    }
    return 0;
}

class mBaseTaskmessage_MO : public ACE_Method_Object
{
public:
    mBaseTaskmessage_MO(mBaseTask *parent, const mstring& data)
    {
	i_parent=parent;
	i_data=data;
    }
    virtual int call()
    {
	i_parent->message_i(i_data);
	return 0;
    }
private:
    mBaseTask *i_parent;
    mstring i_data;
};

void mBaseTask::message(const mstring& message)
{
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
}

void mBaseTask::shutdown()
{
    activation_queue_.enqueue(new shutdown_MO);
}
