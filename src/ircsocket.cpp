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
    char data[512];
    int recvResult;
    recvResult=peer().recv_n(data,512);
    T_Chatter chatter(T_Chatter::From,mstring("IrcServer :")+mstring(data));
    // if(recvResult==-1) major problem.
    // if(recvResult==0) socket has close down
    
    // okay here's the meat.
    // push it onto the message queue and let the svc routine handle it from there.
    ACE_Message_Block *mb=new ACE_Message_Block(512);
    mb->copy(data);	// do check for this returning -1;
    putq(mb);		// do check for this returning -1;

    RET(0);
}

int IrcSvcHandler::svc(void)
{
    NFT("IrcSvcHandler::svc");
    // okay check the message queue and if there are any messages send them on their
    // merry way to whichever server needs to know about them.
    ACE_Message_Block *mb;
    getq(mb);	// do check for this returning -1;
    mstring data=mstring(mb->rd_ptr());
    mb->release();
    //todo here: processing on the message

    RET(0);
}