#include "chanserv.h"
#include "magick.h"

// ***************************************
// NOTE:this function has to be re-entrant
// ***************************************
void *chanserv_thread_handler(void *level)
{
    int ilevel=(int)level;  // 0 get's passed in for the first thread spawned, etc.
    MagickObject->ThreadtoTypeMap[ACE_Thread::self()]=tt_ChanServ;
    while(false) // fix this a bit later to a proper check
    {

	// check for spawn limit hit
	// if so ACE_Thread::spawn(chanserv_thread_handler,(void *)(ilevel+1));
	ACE_Local_RLock *inputbufferlock=new ACE_Local_RLock("chanserv::inputbuffer");
	// check the inputbuffer

	delete inputbufferlock;
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
	ACE_Local_WLock *inputbufferlock=new ACE_Local_WLock("chanserv::inputbuffer");
	inputbuffer.push_back(pair<mstring,mstring>(servicename,message));
	delete inputbufferlock;
}
