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
    ACE_Reactor::instance()->schedule_timer(&sph,0,ACE_Time_Value(Parent->config.Ping_Frequency()),ACE_Time_Value(Parent->config.Ping_Frequency()));
    CP(("IrcSvcHandler activated"));
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
	    ACE_Reactor::instance()->schedule_timer(&(Parent->rh),0,ACE_Time_Value(Parent->config.Server_Relink()));
	return -1;
    }
    // possibly mstring(data,0,recvResult); rather than mstring(data)
    // depends on null terminators etc.
    mstring data2 = flack + data;
    // if(recvResult==-1) major problem.
    // if(recvResult==0) socket has close down    

    if(data2.Contains("\n")||data2.Contains("\r"))
    {
	if (data2.Last() == '\n' || data2.Last() == '\r')
	    flack = "";
	else
	    flack = data2.ExtractWord(data2.WordCount("\n\r"), "\n\r");

	for(int i=1;i<data2.WordCount("\n\r");i++)
	    if(data2.ExtractWord(i,"\n\r")!="")
		mBase::push_message(data2.ExtractWord(i,"\n\r"));
	if (flack == "")
	    mBase::push_message(data2.ExtractWord(data2.WordCount("\n\r"),"\n\r"));

    }
    else
        flack += data2;

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
    // todo: shutdown the ping timer
    ACE_Reactor::instance()->cancel_timer(&sph);
    CP(("Socket closed"));
    RET(handle_close());
}

mstring Reconnect_Handler::FindNext(mstring server) {
    FT("Reconnect_Handler::FindNext", (server));
    server.MakeLower();
    mstring result;

    // IF current server is found
    //     IF last server of this priority
    //         IF size of serverlist of next priority > 0 size
    //             return first element of serverlist of next priority
    //         ELSE return NULL
    //     ELSE return next server of this priority
    // ELSE return NULL

    if (Parent->startup.IsServer(server)) {
	vector<mstring> serverlist = Parent->startup.PriorityList(Parent->startup.Server(server).third);
	vector<mstring>::iterator iter;
	for (iter=serverlist.begin(); iter!=serverlist.end(); iter++)
	    if (*iter == server) break;

	if (iter != serverlist.end()) iter++;

	if (iter == serverlist.end()) {
	    serverlist = Parent->startup.PriorityList(Parent->startup.Server(server).third+1);

	    if (serverlist.size()) {
		RET(*serverlist.begin());

	    } else {
		RET("");
	    }

	} else {
	    RET(*iter);
	}

    } else {
	RET("");
    }
}

int Reconnect_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    FT("Reconnect_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));

    if(Parent->config.Server_Relink()<1||Parent->reconnect!=true||Parent->shutdown()==true)
	return 0;

    mstring server;
    triplet<int,mstring,int> details;
    if (Parent->GotConnect) {
	server = Parent->startup.PriorityList(1)[0];
    } else {
	server = FindNext(Parent->Server);
	if (server == "") {
	    server = Parent->startup.PriorityList(1)[0];
	}
    }
    if (server != "")
	details = Parent->startup.Server(server);
    ACE_INET_Addr addr(details.first, server);

    //IrcServer server(ACE_Reactor::instance(),ACE_NONBLOCK);

    Parent->GotConnect = false;
    Parent->Server = server;
    Parent->ircsvchandler=new IrcSvcHandler;
    if(Parent->ACO_server.connect(Parent->ircsvchandler,addr)==-1)
    {
	//okay we got a connection problem here. log it and try again
	ACE_Reactor::instance()->schedule_timer(&(Parent->rh),0,ACE_Time_Value(Parent->config.Server_Relink()));
    }
    else
    {
        ACE_INET_Addr localaddr;
	Parent->ircsvchandler->peer().get_local_addr(localaddr);
	CP(("Local connection point=%s port:%u",localaddr.get_host_name(),localaddr.get_port_number()));
	Parent->server.raw("PASS " + details.second);
	Parent->server.raw("SERVER " + Parent->startup.Server_Name() + " 1 :" + Parent->startup.Server_Desc());
    }
    RET(0);
}

int ServerPing_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    FT("ServerPing_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    map<mstring,Server>::iterator i;
    for(i=Parent->server.ServerList.begin();i!=Parent->server.ServerList.end();i++)
    {
	i->second.Ping();
    }
    RET(0);
}

int KillOnSignon_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    // If nickserv isnt online yet, wait 1s
    if (!Parent->nickserv.IsLive(Parent->nickserv.FirstName()))
    {
        ACE_Reactor::instance()->schedule_timer(&(Parent->nickserv.kosh),arg,ACE_Time_Value(1));
	return 0;
    }
    FT("KillOnSignon_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    // Nick and Reason
    mstring *tmp = (mstring *) arg;

    Parent->server.KILL(Parent->nickserv.FirstName(), tmp->Before(":"), tmp->After(":"));
    delete tmp;
    RET(0);
}

int ToBeSquit_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    // We ONLY get here if we didnt receive a SQUIT message in <10s
    // after any QUIT message with 2 valid servers in it
    FT("ToBeSquit_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    // Nick and Reason
    mstring *tmp = (mstring *) arg;

    Parent->server.ServerSquit.erase(*tmp);

    // QUIT all user's who faked it ...
    if (Parent->server.ToBeSquit.find(*tmp) != Parent->server.ToBeSquit.end())
    {
	list<mstring>::iterator iter;
	for (iter=Parent->server.ToBeSquit[*tmp].begin(); iter!=Parent->server.ToBeSquit[*tmp].end(); iter++)
	{
	    if (Parent->nickserv.IsLive(*iter))
	    {
		Parent->nickserv.live[*iter].Quit("SQUIT - " + *tmp);
		Parent->nickserv.live.erase(*iter);
	    }
	}
	Parent->server.ToBeSquit.erase(*tmp);
    }   

    delete tmp;
    RET(0);
}

int Squit_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    // OK -- we get here after we've passwd Squit_Protect()
    // seconds after a REAL squit
    FT("Squit_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    // Nick and Reason
    mstring *tmp = (mstring *) arg;

    Parent->server.ServerSquit.erase(*tmp);
    Parent->server.ToBeSquit.erase(*tmp);

    // QUIT all user's who did not come back from SQUIT
    map<mstring,Nick_Live_t>::iterator i;
    vector<mstring> SquitMe;
    vector<mstring>::iterator k;
    for (i=Parent->nickserv.live.begin(); i != Parent->nickserv.live.end(); i++)
    {
	if (i->second.Squit() == *tmp)
	{
	    SquitMe.push_back(i->first);
	}
    }
    for (k=SquitMe.begin(); k != SquitMe.end(); k++)
    {
	if (Parent->nickserv.IsLive(*k))
	{
	    Parent->nickserv.live[*k].Quit("SQUIT - " + *tmp);
	    Parent->nickserv.live.erase(*k);
	}
    }

    delete tmp;
    RET(0);
}
