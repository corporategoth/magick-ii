#include "chanserv.h"
#include "magick.h"
#include <ace/Thread.h>

// ***************************************
// NOTE:this function has to be re-entrant
// ***************************************
void *chanserv_thread_handler(void *level)
{
    MagickObject->ThreadtoTypeMap[ACE_Thread::self()]=tt_ChanServ;
    while(false) // fix this a bit later to a proper check
    {
	// check for spawn limit hit
	// if so ACE_Thread::spawn(chanserv_thread_handler,(void *)((int)level+1));
	ACE_Thread::yield();
    }
    return NULL;
}

void init_chanserv()
{
    ACE_Thread::spawn(chanserv_thread_handler,(void *)0);
}
