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
#include "magick.h"
#include <ace/Auto_Ptr.h>

int IrcSvcHandler::open(void *in)
{
    FT("IrcSvcHandler::open", (in));
    ACE_Reactor::instance()->register_handler(this,ACE_Event_Handler::READ_MASK);
    // todo activate the task
    activate(0,1);

    RET(0);
}

int IrcSvcHandler::handle_input(ACE_HANDLE hin)
{
    FT("IrcSvcHandler::handle_input", ("(ACE_HANDLE) hin"));
    //todo this is the sucker that get's data from the socket, so this is our main routine.
    // might set this up to be an active object here.
    char data[513];
    int recvResult;
    recvResult=peer().recv(data,512);
    // possibly mstring(data,0,recvResult); rather than mstring(data)
    // depends on null terminators etc.
    CH(T_Chatter::From,mstring("IrcServer :")+mstring(data));
    // if(recvResult==-1) major problem.
    // if(recvResult==0) socket has close down    

    activation_queue_.enqueue(new handle_input_MO(this,mstring(data)));

    RET(0);
}

int IrcSvcHandler::svc(void)
{
    NFT("IrcSvcHandler::svc");
    while(1)
    {
	auto_ptr<ACE_Method_Object> mo(this->activation_queue_.dequeue());
	if(mo->call()==-1)
	    break;
    }
    RET(0);
}

int IrcSvcHandler::handle_input_i(const mstring& data)
{
#if 0
    // relevant code in mstring needs to be coded
    if(data.Contains("\n"))
    {
	int i;
	for(i=0;i<data.WordCount("\n");i++)
	    mBase::push_message(data.Word(i,"\n"));
    }
    else
        mBase::push_message (data);
#endif
    mBase::push_message (data);
    return 0;
}

handle_input_MO::handle_input_MO(IrcSvcHandler *parent, const mstring& data)
{
    i_parent=parent;
    i_data=data;
}
int handle_input_MO::call()
{
    i_parent->handle_input_i(i_data);
    return 0;
}
