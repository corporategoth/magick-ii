#include "chanserv.h"
#include "magick.h"

// ***************************************
// NOTE:this function has to be re-entrant
// ***************************************
void *chanserv_thread_handler(void *level)
{
    int ilevel=(int)level;  // 0 get's passed in for the first thread spawned, etc.
    pair<mstring,mstring> data;
    MagickObject->ThreadtoTypeMap[ACE_Thread::self()]=tt_ChanServ;
    while(false) // fix this a bit later to a proper check
    {

	// brackets are here so that the lock exists only as long as we need it.
	{
	    ACE_Local_RLock inputbufferlock("chanserv::inputbuffer");
	    if(MagickObject->chanserv.inputbuffer.size()>ilevel*MagickObject->chanserv.msg_thresh)
	    {
	        ACE_Thread::spawn(chanserv_thread_handler,(void *)(ilevel+1));
	    }
	    else if(MagickObject->chanserv.inputbuffer.size()<(ilevel-1)*MagickObject->chanserv.msg_thresh)
		if(ilevel!=0)
		    return NULL;
	    // check the inputbuffer
	    if(MagickObject->chanserv.inputbuffer.size()!=0)
	    {
		data=MagickObject->chanserv.inputbuffer.front();
		MagickObject->chanserv.inputbuffer.pop_front();
	    }
	}
	MagickObject->chanserv.execute(data.first,data.second);
	ACE_Thread::yield();
    }
    return NULL;
}

void init_chanserv()
{
    ACE_Thread::spawn(chanserv_thread_handler,(void *)0);
}

void ChanServ::push_message(const mstring& servicename, const mstring& message)
{
    ACE_Local_WLock inputbufferlock("chanserv::inputbuffer");
    pair<mstring,mstring> dummyvar(servicename,message);
    inputbuffer.push_back(dummyvar);
}

void ChanServ::execute(const mstring & servicename, const mstring & message)
{
    //okay this is the main chanserv command switcher
}
