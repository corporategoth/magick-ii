#include "pch.h"
#ifdef _MSC_VER
#pragma hdrstop
#endif
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

class handle_input_MO : public ACE_Method_Object
{
public:
    handle_input_MO(IrcSvcHandler *parent, const mstring& data)
    {
	i_parent=parent;
	i_data=data;
    }
    virtual int call()
    {
	i_parent->handle_input_i(i_data);
	return 0;
    }
private:
    IrcSvcHandler *i_parent;
    mstring i_data;
};

class send_MO : public ACE_Method_Object
{
public:
    send_MO(IrcSvcHandler *parent, const mstring& data)
    {
	i_parent=parent;
	i_data=data;
    }
    virtual int call()
    {
	i_parent->send_i(i_data);
	return 0;
    }
private:
    IrcSvcHandler *i_parent;
    mstring i_data;
};

int IrcSvcHandler::open(void *in)
{
    FT("IrcSvcHandler::open", (in));
    ACE_Reactor::instance()->register_handler(this,ACE_Event_Handler::READ_MASK|ACE_Event_Handler::WRITE_MASK);
    // todo activate the task
    CP(("Socket opened"));
    activate(THR_NEW_LWP | THR_JOINABLE,1);
    CP(("SvcHandler activated"));
    // do we do the server command here?
    mstring passcmd="PASS "+Parent->Startup_PASSWORD;
    send(passcmd.c_str());
    mstring servercmd="SERVER "+Parent->Startup_SERVER_NAME+" 1 :["+Parent->Startup_SERVER_DESC+"]";
    send(servercmd.c_str());
    RET(0);
}

int IrcSvcHandler::handle_input(ACE_HANDLE hin)
{
    FT("IrcSvcHandler::handle_input", ("(ACE_HANDLE) hin"));
    //todo this is the sucker that get's data from the socket, so this is our main routine.
    // might set this up to be an active object here.
    char data[513];
    int recvResult;
    memset(data,0,513);
    recvResult=peer().recv(data,512);
    // possibly mstring(data,0,recvResult); rather than mstring(data)
    // depends on null terminators etc.
    int i;
    mstring data2(data);
    for(i=0;i<data2.WordCount("\n");i++)
        if(data2.ExtractWord(i,"\n")!="")
	    CH(T_Chatter::From,mstring("IrcServer :")+data2.ExtractWord(i,"\n"));
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
    // relevant code in mstring needs to be coded
    if(data.Contains("\n"))
    {
	int i;
	for(i=0;i<data.WordCount("\n");i++)
	    if(data.ExtractWord(i,"\n")!="")
		mBase::push_message(data.ExtractWord(i,"\n"));
    }
    else
        mBase::push_message (data);
    mBase::push_message (data);
    return 0;
}

int IrcSvcHandler::send(const mstring & data)
{
    activation_queue_.enqueue(new send_MO(this,mstring(data)));
    return 0;
}

int IrcSvcHandler::send_i(const mstring & data)
{
    int recvResult;
    recvResult=peer().send(data.c_str(),data.Len());
    CH(T_Chatter::To,data);
    return 0;
}

int IrcSvcHandler::shutdown()
{
    activation_queue_.enqueue(new shutdown_MO);
    return 0;
}

int IrcSvcHandler::close(unsigned long in)
{
    CP(("Socket closed"));
    return 0;
}
