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

mBase::mBase()
{
    NFT("mBase::mBase");
}

void mBase::push_message(const mstring& servicename, const mstring& message)
{
    FT("mBase::push_message@"+GetInternalName(), (servicename, message));
    // Braces, so that if we spawn/resume a thread, we make sure it can process
    {
	WLOCK lock(GetInternalName(),"inputbuffer");
	pair<mstring,mstring> dummyvar(servicename,message);
	inputbuffer.push_back(dummyvar);
    }
    // put this here in case the processing loop get's hung up, it just *shrugs* and 
    // starts up another when the threshhold gets hit
    if(inputbuffer.size()>mThread::typecount(Get_TType())*MagickObject->high_water_mark)
    {
        CP(("%s has reached hightide mark, starting a new thread", GetInternalName().c_str()));
        mThread::spawn(Get_TType(),thread_handler,(void *)this);
    }
    if(mThread::typecount(Get_TType())==1&&inputbuffer.size()==1)
    {
        COM(("%s has new messages resuming thread...", GetInternalName().c_str()));
	mThread::resume(mThread::findbytype(Get_TType()));
    }

}

void mBase::init()
{
    NFT("mBase::init@"+GetInternalName());
    mThread::spawn(Get_TType(),thread_handler,(void *)this);
}

void *thread_handler(void *owner)
{
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
    RET((void *) NULL);
}
