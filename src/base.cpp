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

map<unsigned long,mstring > mBase::Buffer_Tuple;
ACE_Message_Queue<ACE_MT_SYNCH> mBase::Buffer_Queue;

mBase::mBase()
{
    NFT("mBase::mBase");
}

void mBase::push_message(const mstring& message)
{
    FT("mBase::push_message@mBase", (message));
    static unsigned long counter=0;
    // Braces, so that if we spawn/resume a thread, we make sure it can process
    {
	WLOCK lock("mBase","inputbuffer");
	counter++;
	Buffer_Tuple[counter]=message;
	//Buffer_Queue
	if(Buffer_Queue.is_full())
	{
	    // set new water marks and spawn off a new thread.
	    Buffer_Queue.high_water_mark(MagickObject->high_water_mark*(ACE_Thread_Manager::instance()->count_threads())*sizeof(unsigned long));
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
}

void mBase::init()
{
    //NFT("mBase::init@"+GetInternalName());
    //mThread::spawn(Get_TType(),thread_handler,(void *)this);
    //todo: setup the first high water mark and first low water mark
    //and spawn off x threads.
    Buffer_Queue.high_water_mark(MagickObject->high_water_mark*sizeof(unsigned long));
    // below is set to the same as high_water_mark so that Buffer_Queue, doesn't block on add.
    Buffer_Queue.low_water_mark(Buffer_Queue.high_water_mark());
    if(ACE_Thread_Manager::instance()->spawn(thread_handler)==-1)
	CP(("Failed to create initial thread"));
}

void *thread_handler(void *owner)
{
    //todo ugh, major revamp Owner will be unknown till we figure out who
    // the thread is to go to (via the message)
    // we then execute the message, finally we (locked) check if we are below
    // low water mark, if so, then we return.
    // if Owner->MSG() == false then we toss message, else we execute it.
    while(MagickObject->shutdown()==false)
    {
	NFT("thread_handler");
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
	    tmp[0]=message.After(" ").Before(" ");		// 2nd arg
	    tmp[1]=message.After(" ").After(" ").Before(" ");	// 3rd arg
	    tmp[0].MakeUpper();

	    if (tmp[0] == "PRIVMSG" || tmp[0] == "NOTICE") {
		mstring names;

		// Find out if the target nick is one of the services 'clones'
		// (and if it is, which one?)  Pass the message to them if so.
//		     if ((names=" "+OperServ::getnames()+" ").Find(" "+tmp[1]+" "))
//		    MagickObject->operserv.execute(message);
		     if ((names=" "+MagickObject->nickserv.getnames()+" ").Find(" "+tmp[1]+" "))
		    MagickObject->nickserv.execute(message);
		else if ((names=" "+MagickObject->chanserv.getnames()+" ").Find(" "+tmp[1]+" "))
		    MagickObject->chanserv.execute(message);
//		else if ((names=" "+MemoServ::getnames()+" ").Find(" "+tmp[1]+" "))
//		    MagickObject->memoserv.execute(message);
//		else if ((names=" "+HelpServ::getnames()+" ").Find(" "+tmp[1]+" "))
//		    MagickObject->helpserv.execute(message);

		// How do we want to handle custom services (BOB created)?

		// Not a MSG/NOTICE to services, fall through
		// (it could be to a channel, or unrecognised nick).
//		else
//		    Server.execute (message);

	    } else {
		// This handles all non-msgs/notices.
//		Server.execute (message);
	    }

	    /* Locking here because we dont want two threads
	     * dieing because they both counted at the same
	     * time and before the other had died.  Unlikely,
	     * but it CAN happen.
	     */
	    {
		MLOCK("thread_handler", "LWM");
		if(mBase::Buffer_Queue.message_count()<
			MagickObject->high_water_mark*(ACE_Thread_Manager::instance()->count_threads()-2)*sizeof(unsigned long)+MagickObject->low_water_mark*sizeof(unsigned long))
		{
		    mBase::Buffer_Queue.high_water_mark(MagickObject->high_water_mark*(ACE_Thread_Manager::instance()->count_threads()-1)*sizeof(unsigned long));
		    mBase::Buffer_Queue.low_water_mark(mBase::Buffer_Queue.high_water_mark());
		    COM(("Low water mark reached, killing thread."));
		    return NULL;
		}
	    }
	}
    }

#if 0
    mBase *Owner=(mBase *)owner;
    wxASSERT(Owner);
    FT("thread_handler@"+Owner->GetInternalName(), (owner));
    int ilevel=mThread::find()->number();
    pair<mstring,mstring> data;
    bool gotdata;

    while(Owner->MSG())
    {

	gotdata=false;
	// brackets are here so that the lock exists only as long as we need it.
	{
	    RLOCK lock(Owner->GetInternalName(),"inputbuffer");
	    // check the inputbuffer
	    if(Owner->inputbuffer.size()!=0)
	    {
		data=Owner->inputbuffer.front();
		Owner->inputbuffer.pop_front();
		gotdata=true;
	    }

	}
	if (gotdata)
	    Owner->execute(data.first,data.second);

	// less then the 1/2 the threshhold below it so that we dont shutdown the thread after reading the first message
        if(mThread::typecount(Owner->Get_TType())!=1 && ilevel==mThread::typecount(Owner->Get_TType()) && Owner->inputbuffer.size() <
		(mThread::typecount(Owner->Get_TType()) - 1) * MagickObject->high_water_mark + MagickObject->low_water_mark)
        {
    	    CP(("%s has reached lowtide mark, dropping a thread", Owner->GetInternalName().c_str()));
	    return NULL;
	}

	// if theres leftover time in the timeslice, yield it up to the processor.
        if(mThread::typecount(Owner->Get_TType()) == 1 && Owner->inputbuffer.size() == 0)
        {
            COM(("%s has no more messages left going to suspended state...", Owner->GetInternalName().c_str()));
	    mThread::suspend();
//	    VERY VERY temporary measure!!
#ifdef WIN32
	    Sleep(1000);
#else
	    sleep(1);
#endif
	}
    }
#endif
    return NULL;
}
