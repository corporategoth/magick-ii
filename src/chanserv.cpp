#include "chanserv.h"
#include "magick.h"

ChanServ::ChanServ()
{
    on=false;
}
// ***************************************
// NOTE:this function has to be re-entrant
// ***************************************

static int highestlevel=0;

void *chanserv_thread_handler(void *level)
{
    int ilevel=(int)level;  // 0 get's passed in for the first thread spawned, etc.
    pair<mstring,mstring> data;

    if(highestlevel<ilevel)
	highestlevel=ilevel;
    MagickObject->ThreadtoTypeMap[ACE_Thread::self()]=tt_ChanServ;
    while(MagickObject->chanserv.on==true)
    {

	// brackets are here so that the lock exists only as long as we need it.
	{
	    ACE_Local_RLock inputbufferlock("chanserv::inputbuffer");
	    // check the inputbuffer
	    if(MagickObject->chanserv.inputbuffer.size()!=0)
	    {
		data=MagickObject->chanserv.inputbuffer.front();
		MagickObject->chanserv.inputbuffer.pop_front();
	    }
	    if(ilevel==highestlevel)
	    {
		// less then the 1/2 the threshhold below it so that we dont shutdown the thread after reading the first message
		if(MagickObject->chanserv.inputbuffer.size()<(highestlevel-1)*MagickObject->chanserv.msg_thresh+MagickObject->chanserv.msg_thresh/2)
		{
		    if(highestlevel!=0)
		    {
			highestlevel--;
			return NULL;
		    }
		}
	    }
	}
	MagickObject->chanserv.execute(data.first,data.second);
	// if theres leftover time in the timeslice, yield it up to the processor.
	ACE_Thread::yield();
    }
    return NULL;
}

void ChanServ::init()
{
    ACE_Thread::spawn(chanserv_thread_handler,(void *)0);
}

void ChanServ::push_message(const mstring& servicename, const mstring& message)
{
    ACE_Local_WLock inputbufferlock("chanserv::inputbuffer");
    pair<mstring,mstring> dummyvar(servicename,message);
    inputbuffer.push_back(dummyvar);
    // put this here in case the processing loop get's hung up, it just *shrugs* and 
    // starts up another when the threshhold gets hit
    if(inputbuffer.size()>highestlevel*msg_thresh)
        ACE_Thread::spawn(chanserv_thread_handler,(void *)(highestlevel+1));
}

void ChanServ::execute(const mstring & servicename, const mstring & message)
{
    //okay this is the main chanserv command switcher
}

int ChanServ::listchans(vector<mstring> &args)
{
    return MAGICK_RET_TERMINATE;
}
