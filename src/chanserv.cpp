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

#include "chanserv.h"
#include "lockable.h"
#include "magick.h"

ChanServ::ChanServ()
{
    NFT("ChanServ::ChanServ");
    on=false;
}
// ***************************************
// NOTE:this function has to be re-entrant
// ***************************************

void *chanserv_thread_handler(void *level)
{
    FT("chanserv_thread_handler", (level));
    int ilevel=(int)level;
    pair<mstring,mstring> data;

    while(MagickObject->chanserv.on==true)
    {

	// brackets are here so that the lock exists only as long as we need it.
	{
	    RLOCK lock("ChanServ","inputbuffer");
	    // check the inputbuffer
	    if(MagickObject->chanserv.inputbuffer.size()!=0)
	    {
		data=MagickObject->chanserv.inputbuffer.front();
		MagickObject->chanserv.inputbuffer.pop_front();
	    }

	}
	MagickObject->chanserv.execute(data.first,data.second);
	// less then the 1/2 the threshhold below it so that we dont shutdown the thread after reading the first message
        if(mThread::typecount(tt_ChanServ)!=1 && ilevel==mThread::typecount(tt_ChanServ) && MagickObject->chanserv.inputbuffer.size() <
		(mThread::typecount(tt_ChanServ) - 1) * MagickObject->high_water_mark + MagickObject->low_water_mark)
        {
    	    CP(("chanserv has reached lowtide mark, dropping a thread"));
	    return NULL;
	}

	// if theres leftover time in the timeslice, yield it up to the processor.
        if(mThread::typecount(tt_ChanServ) == 1 && MagickObject->chanserv.inputbuffer.size() == 0)
        {
            CP(("chanserv has no more messages left going to suspended state..."));
	    mThread::suspend();
	}
    }
    return NULL;
}

void ChanServ::init()
{
    NFT("ChanServ::init()");
    mThread::spawn(tt_ChanServ,chanserv_thread_handler,(void *)(mThread::typecount(tt_ChanServ)+1));
}

void ChanServ::push_message(const mstring& servicename, const mstring& message)
{
    FT("ChanServ::push_message", (servicename, message));
    // Braces, so that if we spawn/resume a thread, we make sure it can process
    {
	WLOCK lock("ChanServ","inputbuffer");
	pair<mstring,mstring> dummyvar(servicename,message);
	inputbuffer.push_back(dummyvar);
    }
    // put this here in case the processing loop get's hung up, it just *shrugs* and 
    // starts up another when the threshhold gets hit
    if(inputbuffer.size()>mThread::typecount(tt_ChanServ)*MagickObject->high_water_mark)
    {
        CP(("chanserv has reached hightide mark, starting a new thread"));
        mThread::spawn(tt_ChanServ,chanserv_thread_handler,(void *)(mThread::typecount(tt_ChanServ)+1));
    }
    if(mThread::typecount(tt_ChanServ)==1&&MagickObject->chanserv.inputbuffer.size()==1)
    {
        CP(("chanserv has new messages resuming thread..."));
	mThread::resume(mThread::findbytype(tt_ChanServ));
    }

}

void ChanServ::execute(const mstring & servicename, const mstring & message)
{
    FT("ChanServ::execute", (servicename, message));
    //okay this is the main chanserv command switcher
}

/*int ChanServ::listchans(vector<mstring> &args)
{
    FT("ChanServ::listchans", ("(vector<mstring>) args"));
    RET(MAGICK_RET_TERMINATE);
}
*/