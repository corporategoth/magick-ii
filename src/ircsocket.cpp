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
#include "lockable.h"

int IrcSvcHandler::open(void *in)
{
    FT("IrcSvcHandler::open", (in));
    CP(("Socket opened"));
    ACE_Reactor::instance()->register_handler(this,ACE_Event_Handler::READ_MASK);
    //activate();
    // todo activate the task
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
    flack = "";
    // if(recvResult==-1) major problem.
    // if(recvResult==0) socket has close down    

    if(data2.Contains("\n")||data2.Contains("\r"))
    {
	if (!(data2.Last() == '\n' || data2.Last() == '\r'))
	    flack = data2.ExtractWord(data2.WordCount("\n\r"), "\n\r");

	for(int i=1;i<data2.WordCount("\n\r");i++)
	    if(data2.ExtractWord(i,"\n\r")!="")
		mBase::push_message(data2.ExtractWord(i,"\n\r"));
	if (flack == "")
	    mBase::push_message(data2.ExtractWord(data2.WordCount("\n\r"),"\n\r"));

    }
    else
        flack = data2;

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


int InFlight_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    FT("InFlight_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    mstring *tmp = (mstring *) arg;
    Nick_Live_t *entry;

    if (Parent->nickserv.IsLive(*tmp))
    {
	entry = &Parent->nickserv.live[tmp->LowerCase()];
	if (entry->InFlight.File())
	{
	    if (!entry->InFlight.InProg())
		entry->InFlight.Cancel();
	}
	else
	{
	    entry->InFlight.End(0);
	}
    }
    delete tmp;
    RET(0);
}



int EventTask::open(void *in)
{
    FT("EventTask::open", ("(void *) in"));
    RET(activate());
}

int EventTask::close(unsigned long in)
{
    FT("EventTask::close", (in));
    // dump all and close open file handles.
    RET(0);
}

int EventTask::svc(void)
{
    NFT("EventTask::svc");
    mThread::Attach(tt_MAIN);

    last_expire = last_save = last_check = last_ping = Now();
    while(!Parent->shutdown())
    {
	// Main routine -- when we end this, we're done!!
	map<mstring, Nick_Live_t>::iterator nli;
	map<mstring, Nick_Stored_t>::iterator nsi;
	map<mstring, Chan_Live_t>::iterator cli;
	map<mstring, Chan_Stored_t>::iterator csi;
	map<mstring, list<Memo_t> >::iterator mi;
	list<Memo_t>::iterator lmi;
	map<mstring, list<News_t> >::iterator ni;
	list<News_t>::iterator lni;
	map<mstring, Committee>::iterator ci;
	map<mstring, Server>::iterator si;
	map<mstring, mDateTime>::iterator di;
	unsigned int i;

	if (last_expire.SecondsSince() >= Parent->config.Cycletime())
	{
	    CP(("Starting EXPIRATION check ..."));

	    // akills
	    {
		MLOCK(("OperServ","Akill"));
		bool firstgone = false;
		for (Parent->operserv.Akill = Parent->operserv.Akill_begin();
			(Parent->operserv.Akill != Parent->operserv.Akill_end() && !firstgone);
			Parent->operserv.Akill++)
		{
		    if (firstgone)
		    {
			Parent->operserv.Akill = Parent->operserv.Akill_begin();
			firstgone = false;
		    }
		    if (Parent->operserv.Akill->Last_Modify_Time().HoursSince() >
			    Parent->operserv.Akill->Value().first)
		    {
			if (Parent->operserv.Akill == Parent->operserv.Akill_begin())
			{
			    Parent->operserv.Akill_erase();
			    firstgone = true;
			}
			else
			{
			    set<entlist_val_t<pair<long, mstring> > >::iterator LastEnt = Parent->operserv.Akill;
			    LastEnt--;
			    Parent->operserv.Akill_erase();
			    Parent->operserv.Akill = LastEnt;
			}
		    }
		}
	    }

	    // nicknames
	    {
		MLOCK(("NickServ", "stored"));
		vector<mstring> expired_nicks;
		for (nsi = Parent->nickserv.stored.begin();
			nsi != Parent->nickserv.stored.end(); nsi++)
		{
		    if (nsi->second.Host() == "")
		    {
			if (nsi->second.LastAllSeenTime().DaysSince() >
			    Parent->nickserv.Expire())
			    expired_nicks.push_back(nsi->first);
		    }
		    else
		    {
			if (nsi->second.LastSeenTime().DaysSince() >
			    Parent->nickserv.Expire())
			    expired_nicks.push_back(nsi->first);
		    }
		}
		for (i=0; i<expired_nicks.size(); i++)
		{
		    Parent->nickserv.stored[expired_nicks[i]].Drop();
		    Parent->nickserv.stored.erase(expired_nicks[i]);
		}
	    }

	    // channels
	    {
		MLOCK(("ChanServ", "stored"));
		vector<mstring> expired_chans;
		for (csi = Parent->chanserv.stored.begin();
			csi != Parent->chanserv.stored.end(); nsi++)
		{
		    if (csi->second.LastUsed().DaysSince() >
			Parent->chanserv.Expire())
			expired_chans.push_back(csi->first);
		}
		for (i=0; i<expired_chans.size(); i++)
		    Parent->chanserv.stored.erase(expired_chans[i]);
	    }

	    // news articles
	    {
		MLOCK(("MemoServ", "channel"));
		for (ni=Parent->memoserv.channel.begin();
			ni!=Parent->memoserv.channel.end(); ni++)
		{
		    bool firstgone = false;
		    for (lni=ni->second.begin();
			(lni!=ni->second.end() && !firstgone); lni++)
		    {
			if (firstgone)
			{
			    lni = ni->second.begin();
			    firstgone = false;
			}
			if (lni->Time().DaysSince() >
			    Parent->memoserv.News_Expire())
			{
			    if (lni == ni->second.begin())
			    {
				ni->second.erase(lni);
				firstgone = true;
				lni = ni->second.end();
			    }
			    else
			    {
				list<News_t>::iterator lastnews = lni;
				lastnews--;
				ni->second.erase(lni);
				lni = lastnews;
			    }
			}
		    }
		}
	    }
	    last_expire = Now();
	}

	if (last_save.SecondsSince() >= Parent->config.Cycletime())
	{
	    CP(("Starting DATABASE SAVE ..."));

	    //Parent->operserv.save_database();
	    //Parent->nickserv.save_database();
	    //Parent->chanserv.save_database();
	    //Parent->memoserv.save_database();
	    //Parent->commserv.save_database();
	    //Parent->servmsg.save_database();
	    // Scripted services?

	    last_save = Now();
	}

	if (last_check.SecondsSince() >= Parent->config.Checktime())
	{
	    CP(("Starting CHECK cycle ..."));

	    for (cli=Parent->chanserv.live.begin();
		    cli!=Parent->chanserv.live.end(); cli++)
	    {
		csi = Parent->chanserv.stored.find(cli->first);
		// Removing bans ...
		if (!Parent->chanserv.LCK_Bantime() ||
		    !Parent->chanserv.DEF_Bantime())
		{
		    unsigned long bantime;
		    if (Parent->chanserv.LCK_Bantime())
			bantime = Parent->chanserv.DEF_Bantime();
		    else if (csi != Parent->chanserv.stored.end())
			bantime = csi->second.Bantime();
		    if (bantime)
		    {
			vector<mstring> remove;
			vector<mstring>::iterator ri;
			for (di=cli->second.bans.begin();
				di!=cli->second.bans.end(); di++)
			{
			    if (di->second.SecondsSince() > bantime)
			    {
				remove.push_back(di->first);
			    }
			}
			for (ri=remove.begin(); ri!=remove.end(); ri++)
			{
			    cli->second.SendMode("-b " + *ri);
			}
		    }
		}

		// Sending pending modes ...
		if (cli->second.p_modes_on != "" || cli->second.p_modes_off != "")
		{
		    int modesperline = 4, j, k;
		    vector<mstring> modelines;
		    mstring mode;
		    mstring modeparam;

		    for (i=0, j=0, k=0; i<cli->second.p_modes_off.size(); i++, j++)
		    {
			if (j>=modesperline)
			{
			    modelines.push_back(mode + " " + modeparam);
			    mode = modeparam = "";
			    j=0;
			}
			if (mode == "")
			    mode += "-";
			mode += cli->second.p_modes_off[i];
			switch (cli->second.p_modes_off[i])
			{
			case 'o':
			case 'v':
			case 'b':
			case 'k':
			    if (modeparam != "")
				modeparam += " ";
			    modeparam +=  cli->second.p_modes_off_params[k];
			    k++;
			}
		    }
		    if (j>0 && cli->second.p_modes_on.size())
			mode += "+";
		    for (i=0, k=0; i<cli->second.p_modes_on.size(); i++, j++)
		    {
			if (j>=modesperline)
			{
			    modelines.push_back(mode + " " + modeparam);
			    mode = modeparam = "";
			    j=0;
			}
			if (mode == "")
			    mode += "+";
			mode += cli->second.p_modes_on[i];
			switch (cli->second.p_modes_on[i])
			{
			case 'o':
			case 'v':
			case 'b':
			case 'k':
			case 'l':
			    if (modeparam != "")
				modeparam += " ";
			    modeparam += cli->second.p_modes_on_params[k];
			    k++;
			}
		    }
		    if (j>0)
			modelines.push_back(mode + " " + modeparam);
		    for (i=0; i<modelines.size(); i++)
		    {
			Parent->server.MODE(Parent->chanserv.FirstName(),
			    cli->first, modelines[i]);
		    }
		}
	    }

	    // Check if we should rename people who are past their
	    // grace time on ident (if KillProtect is on, and they
	    // are not on access list or secure is on).
	    vector<mstring> chunked;
	    for (nli = Parent->nickserv.live.begin();
			    nli != Parent->nickserv.live.end(); nli++)
	    {
		nsi = Parent->nickserv.stored.find(nli->first);
		if (!nli->second.IsServices() &&
		    nsi != Parent->nickserv.stored.end() &&
		    nsi->second.Protect() && !nsi->second.IsOnline() &&
		    nli->second.MySignonTime().SecondsSince() >=
					    Parent->nickserv.Ident())
		{
		    chunked.push_back(nli->first);
		}
	    }
	    for (i=0; i<chunked.size(); i++)
	    {
		nli = Parent->nickserv.live.find(chunked[i]);
		nsi = Parent->nickserv.stored.find(chunked[i]);
		mstring newnick = Parent->nickserv.findnextnick(nli->second.Name());
		mstring oldnick = nli->second.Name();
		if (newnick != "")
		    Parent->server.SVSNICK(Parent->nickserv.FirstName(),
			oldnick, newnick);
		else
		{
		    Parent->server.KILL(Parent->nickserv.FirstName(),
			oldnick, "Kill Protection enforced.");
		    Parent->server.NICK(oldnick, Parent->startup.Ownuser() ?
				    oldnick.LowerCase() :
				    Parent->startup.Services_User(),
				    Parent->startup.Services_Host(),
				    Parent->startup.Server_Name(),
				    "Nickname Enforcer");
		}
		Parent->nickserv.recovered[oldnick.LowerCase()] = Now();
	    }

	    chunked.clear();
	    // Sign off clients we've decided to take.
	    for (di = Parent->nickserv.recovered.begin();
			di != Parent->nickserv.recovered.end(); di++)
	    {
		if (di->second.SecondsSince() >= Parent->nickserv.Release())
		{
		    if (Parent->nickserv.IsLive(di->first) &&
			Parent->nickserv.live[di->first].IsServices())
		    {
			Parent->server.QUIT(di->first, "RECOVER period expired");
			chunked.push_back(di->first);
		    }
		}
	    }
	    for (i=0; i<chunked.size(); i++)
		Parent->nickserv.recovered.erase(chunked[i]);
	    last_check = Now();
	}

	if (last_ping.SecondsSince() >= Parent->config.Ping_Frequency())
	{
	    CP(("Starting SERVER PING ..."));

	    vector<double> pingtimes;
	    double min = -1, max = 0, sum, avg;
	    for (si=Parent->server.ServerList.begin();
		    si!=Parent->server.ServerList.end(); si++)
	    {
		if (min == -1 || si->second.Lag() < min)
		    min = si->second.Lag();
		if (si->second.Lag() > max)
		    max = si->second.Lag();
		pingtimes.push_back(si->second.Lag());
		sum += si->second.Lag();
	    }
	    if (pingtimes.size() >= 3)
		avg = (sum - min - max) / (pingtimes.size() - 2);
	    else
		avg = sum / pingtimes.size();

	    if (avg > (Parent->startup.Lagtime() * (Parent->level - Parent->startup.Level() + 1)))
		Parent->level++;
	    if (Parent->level > Parent->startup.Level() &&
		avg <= (Parent->startup.Lagtime() * (Parent->level - Parent->startup.Level() + 1)))
		Parent->level--;		

	    for (si=Parent->server.ServerList.begin();
		    si!=Parent->server.ServerList.end();si++)
		si->second.Ping();
	    last_ping = Now();
	}
	

#ifdef WIN32
	Sleep(1000);
#else
	sleep(1);
#endif
    }
    mThread::Detach(tt_MAIN);
    RET(0);
}

