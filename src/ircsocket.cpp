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

#include "ircsocket.h"
#include "trace.h"

int IrcSvcHandler::open(void *in)
{
    FT("IrcSvcHandler::open", (in));
    ACE_Reactor::instance()->register_handler(this,ACE_Event_Handler::READ_MASK);
    // todo activate the task
    activate(THR_NEW_LWP,1);

    RET(0);
}

int IrcSvcHandler::handle_input(ACE_HANDLE hin)
{
    FT("IrcSvcHandler::handle_input", ("(ACE_HANDLE) hin"));
    //todo this is the sucker that get's data from the socket, so this is our main routine.
    // might set this up to be an active object here.
    char data[513];
    int recvResult;
    recvResult=peer().recv_n(data,512);
    CH(T_Chatter::From,mstring("IrcServer :")+mstring(data));
    // if(recvResult==-1) major problem.
    // if(recvResult==0) socket has close down
    
    // okay here's the meat.
    // push it onto the message queue and let the svc routine handle it from there.
    ACE_Message_Block *mb=new ACE_Message_Block(512);
    mb->copy(data);	// do check for this returning -1;
    // prez: should we do this, or should we keep trying till it get's queued?
    // ie  while(putq(mb)==-1);
    for(int i=0;i<50;i++)
	if(putq(mb)!=-1)   // this keeps trying 50 times or till it puts it on the queue
	    break;

    RET(0);
}

int IrcSvcHandler::svc(void)
{
    NFT("IrcSvcHandler::svc");
    ACE_Message_Block *mb;

    while(shutdown==false)
    {
	if(msg_queue()->message_count()>0)
	{
	    getq(mb);
            // okay check the message queue and if there are any messages send them on their
    	    // merry way to whichever server needs to know about them.
	    mstring data=mstring(mb->rd_ptr());
	    mb->release();
	    //todo here: processing on the message
	}
	else
	    ACE_OS::thr_yield();
    }
    RET(0);
}
