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

int IrcSvcHandler::open(void *in)
{
    FT("IrcSvcHandler::open", (in));
    CP(("Socket opened"));
    ACE_Reactor::instance()->register_handler(this,ACE_Event_Handler::READ_MASK);
    //activate();
    // todo activate the task
    CP(("SvcHandler activated"));
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
    if(recvResult==0)
    {
	// sleep and then reconnect
	CP(("No data, scheduling reconnect, then closing down"));
	if(Parent->reconnect==true||Parent->shutdown()==false)
	    ACE_Reactor::instance()->schedule_timer(&(Parent->rh),0,ACE_Time_Value(Parent->Config_SERVER_RELINK));
	return -1;
    }
    // possibly mstring(data,0,recvResult); rather than mstring(data)
    // depends on null terminators etc.
    mstring data2(data);
    // if(recvResult==-1) major problem.
    // if(recvResult==0) socket has close down    

    if(data2.Contains("\n")||data2.Contains("\r"))
    {
	int i;
	for(i=0;i<data2.WordCount("\n\r");i++)
	    if(data2.ExtractWord(i+1,"\n\r")!="")
		mBase::push_message(data2.ExtractWord(i+1,"\n\r"));
    }
    else
        mBase::push_message (data);

    RET(0);
}

int IrcSvcHandler::send(const mstring & data)
{
    FT("IrcSvcHandler::send",(data));
    //activation_queue_.enqueue(new send_MO(this,mstring(data)));
    int recvResult;
    recvResult=peer().send((data + "\r\n").c_str(),data.Len()+2);
    CH(T_Chatter::To,data);
    RET(recvResult);
}

int IrcSvcHandler::close(unsigned long in)
{
    FT("IrcSvcHandler::close",(in));
    CP(("Socket closed"));
    RET(handle_close());
}

int Reconnect_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    ACE_INET_Addr addr(Parent->Startup_REMOTE_PORT,Parent->Startup_REMOTE_SERVER);
    //IrcServer server(ACE_Reactor::instance(),ACE_NONBLOCK);
    if(Parent->Config_SERVER_RELINK==-1||Parent->reconnect!=true||Parent->shutdown()==true)
	return 0;
    Parent->ircsvchandler=new IrcSvcHandler;
    if(Parent->ACO_server.connect(Parent->ircsvchandler,addr)==-1)
    {
	//okay we got a connection problem here. log it and try again
	ACE_Reactor::instance()->schedule_timer(&(Parent->rh),0,ACE_Time_Value(Parent->Config_SERVER_RELINK));
    }
    return 0;
}
