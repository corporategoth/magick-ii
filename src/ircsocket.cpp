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
#include <ace/Auto_Ptr.h>

process_MO::process_MO(IrcSocket *socket, mstring input)
{
    input_i=input;
    socket_i=socket;
}

int process_MO::call()
{
    socket_i->process_i(input_i);
    return 0;
}

process_MO::~process_MO()
{
}

shutdown_MO::shutdown_MO()
{
}

shutdown_MO::~shutdown_MO()
{
}

int shutdown_MO::call()
{
    return -1;
}

IrcSocket::IrcSocket()
{
}

IrcSocket::~IrcSocket()
{
}

int IrcSocket::open(void *)
{
    //todo startup the event objects and fire off the connection to the server
    return activate(THR_NEW_LWP);
}

int IrcSocket::close(unsigned long flags)
{
    //shutdown the connection to the server here.
    return 0;
}

int IrcSocket::svc()
{
    while(1)
    {
	auto_ptr<ACE_Method_Object> mo(activation_queue.dequeue());
	if(mo->call()==-1)
	    break;
    }
    return 0;
}

void IrcSocket::process(mstring input)
{
    activation_queue.enqueue(new process_MO(this,input));
}

void IrcSocket::shutdown()
{
    activation_queue.enqueue(new shutdown_MO());
}

void IrcSocket::process_i(mstring input)
{
    // do the heavy work here.
}
