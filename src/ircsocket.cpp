#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.96  2000/04/04 03:13:50  prez
** Added support for masking hostnames.
**
** Revision 1.95  2000/03/28 16:20:58  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.94  2000/03/27 21:26:12  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.93  2000/03/24 15:35:18  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.92  2000/03/19 08:50:55  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.91  2000/03/15 14:42:58  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.90  2000/03/15 08:23:51  prez
** Added locking stuff for commserv options, and other stuff
**
** Revision 1.89  2000/03/14 10:05:16  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.88  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.87  2000/03/02 07:25:11  prez
** Added stuff to do the chanserv greet timings (ie. only greet if a user has
** been OUT of channel over 'x' seconds).  New stored chanserv cfg item.
**
** Revision 1.86  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.85  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.84  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.83  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.82  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


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
    if(recvResult<=0 || Parent->Shutdown())
    {
	// sleep and then reconnect
	CP(("No data, scheduling reconnect, then closing down"));
	Parent->Connected(false);
	if(Parent->Reconnect() && !Parent->Shutdown())
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

	for(unsigned int i=1;i<data2.WordCount("\n\r");i++)
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
    int retval = handle_close();
    RET(retval);
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

    if(Parent->config.Server_Relink()<1 || !Parent->Reconnect() ||
	    Parent->Shutdown())
	return 0;

    mstring server;
    triplet<unsigned int,mstring,unsigned int> details;
    if (Parent->GotConnect()) {
	server = Parent->startup.PriorityList(1)[0];
    } else {
	server = FindNext(Parent->Server());
	if (server == "") {
	    server = Parent->startup.PriorityList(1)[0];
	}
    }
    if (server != "")
	details = Parent->startup.Server(server);
    ACE_INET_Addr addr(details.first, server);

    //IrcServer server(ACE_Reactor::instance(),ACE_NONBLOCK);

    Parent->GotConnect(false);
    Parent->i_server = server;
    Parent->ircsvchandler=new IrcSvcHandler;
    wxLogInfo(Parent->getLogMessage("OTHER/CONNECTING"),
		server.c_str(), details.first);
    if(Parent->ACO_server.connect(Parent->ircsvchandler,addr)==-1)
    {
	wxLogError(Parent->getLogMessage("OTHER/REFUSED"),
		server.c_str(), details.first);
	//okay we got a connection problem here. log it and try again
	ACE_Reactor::instance()->schedule_timer(&(Parent->rh),0,ACE_Time_Value(Parent->config.Server_Relink()));
    }
    else
    {
        ACE_INET_Addr localaddr;
	Parent->ircsvchandler->peer().get_local_addr(localaddr);
	CP(("Local connection point=%s port:%u",localaddr.get_host_name(),localaddr.get_port_number()));
	if (Parent->server.proto.Protoctl() != "")
	    Parent->server.raw(Parent->server.proto.Protoctl());
	Parent->server.raw("PASS " + details.second);
	mstring tmp;
	tmp.Format(Parent->server.proto.Server().c_str(),
	    Parent->startup.Server_Name().c_str(), 1,
	    Parent->startup.Server_Desc().c_str());
	Parent->server.raw(tmp);
	Parent->Connected(true);
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

    if (Parent->server.IsServer(*tmp))
	wxLogNotice(Parent->getLogMessage("OTHER/SQUIT_CANCEL"),
		tmp->c_str(),
		Parent->server.ServerList[tmp->LowerCase()].Uplink().c_str());
    else
	wxLogNotice(Parent->getLogMessage("OTHER/SQUIT_CANCEL"),
		tmp->c_str(), "?");

    // QUIT all user's who faked it ...
    if (Parent->server.ToBeSquit.find(*tmp) != Parent->server.ToBeSquit.end())
    {
	list<mstring>::iterator iter;
	for (iter=Parent->server.ToBeSquit[*tmp].begin(); iter!=Parent->server.ToBeSquit[*tmp].end(); iter++)
	{
	    if (Parent->nickserv.IsLive(*iter))
	    {
		Parent->nickserv.live[*iter].Quit("FAKE SQUIT - " + *tmp);
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
	    entry->InFlight.End(0u);
	}
    }
    delete tmp;
    RET(0);
}


int Part_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    FT("Part_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    mstring *tmp = (mstring *) arg;

    if (Parent->chanserv.IsLive(*tmp) &&
	Parent->chanserv.live[tmp->LowerCase()].IsIn(
			Parent->chanserv.FirstName()))
    {
	Parent->server.PART(Parent->chanserv.FirstName(), *tmp);
	Parent->chanserv.live[tmp->LowerCase()].ph_timer = 0;
    }
    delete tmp;
    RET(0);
}


mstring EventTask::SyncTime()
{
    NFT("EventTask::SyncTime");
    mstring retval = ToHumanTime(Parent->config.Cycletime() - (unsigned long)last_save.SecondsSince());
    RET(retval);
}

int EventTask::open(void *in)
{
    FT("EventTask::open", ("(void *) in"));
    int retval = activate();
    RET(retval);
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
    while(!Parent->Shutdown())
    {
	if (!Parent->AUTO())
	    continue;

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
	vector<mstring> chunked;

	if (last_expire.SecondsSince() >= Parent->config.Cycletime())
	{
	    CP(("Starting EXPIRATION check ..."));

	    // akills
	    {
		MLOCK(("OperServ","Akill"));
		bool firstgone = false;
		for (Parent->operserv.Akill = Parent->operserv.Akill_begin();
			Parent->operserv.Akill_size() &&
			(Parent->operserv.Akill != Parent->operserv.Akill_end() || firstgone);
			Parent->operserv.Akill++)
		{
		    if (firstgone)
		    {
			firstgone = false;
			Parent->operserv.Akill = Parent->operserv.Akill_begin();
		    }
		    if (Parent->operserv.Akill->Last_Modify_Time().SecondsSince() >
			    Parent->operserv.Akill->Value().first)
		    {
			if (Parent->operserv.Akill == Parent->operserv.Akill_begin())
			{
			    Parent->server.RAKILL(Parent->operserv.Akill->Entry());
			    wxLogInfo(Parent->getLogMessage("EVENT/EXPIRE_AKILL"),
				    Parent->operserv.Akill->Entry().c_str(),
				    Parent->operserv.Akill->Value().second.c_str(),
				    Parent->operserv.Akill->Last_Modifier().c_str(),
				    ToHumanTime(Parent->operserv.Akill->Value().first).c_str());
			    announce(Parent->operserv.FirstName(),
				    Parent->getLogMessage("EVENT/EXPIRE_AKILL"),
				    Parent->operserv.Akill->Entry().c_str(),
				    Parent->operserv.Akill->Value().second.c_str(),
				    Parent->operserv.Akill->Last_Modifier().c_str(),
				    ToHumanTime(Parent->operserv.Akill->Value().first).c_str());
			    Parent->operserv.Akill_erase();
			    firstgone = true;
			}
			else
			{
			    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator LastEnt = Parent->operserv.Akill;
			    LastEnt--;
			    Parent->server.RAKILL(Parent->operserv.Akill->Entry());
			    wxLogInfo(Parent->getLogMessage("EVENT/EXPIRE_AKILL"),
				    Parent->operserv.Akill->Entry().c_str(),
				    Parent->operserv.Akill->Value().second.c_str(),
				    Parent->operserv.Akill->Last_Modifier().c_str(),
				    ToHumanTime(Parent->operserv.Akill->Value().first).c_str());
			    announce(Parent->operserv.FirstName(),
				    Parent->getLogMessage("EVENT/EXPIRE_AKILL"),
				    Parent->operserv.Akill->Entry().c_str(),
				    Parent->operserv.Akill->Value().second.c_str(),
				    Parent->operserv.Akill->Last_Modifier().c_str(),
				    ToHumanTime(Parent->operserv.Akill->Value().first).c_str());
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
		    if (!nsi->second.NoExpire())
		    {
			if (nsi->second.Host() == "")
			{
			    if (nsi->second.LastAllSeenTime().SecondsSince() >
				Parent->nickserv.Expire())
				expired_nicks.push_back(nsi->first);
			}
			else
			{
			    if (nsi->second.LastSeenTime().SecondsSince() >
				Parent->nickserv.Expire())
				expired_nicks.push_back(nsi->first);
			}
		    }
		}
		for (i=0; i<expired_nicks.size(); i++)
		{
		    wxLogInfo(Parent->getLogMessage("EVENT/EXPIRE_NICK"),
			    Parent->nickserv.stored[expired_nicks[i]].Name().c_str(),
			    ((Parent->nickserv.stored[expired_nicks[i]].Host() != "") ?
				Parent->nickserv.stored[expired_nicks[i]].Host().c_str() :
				Parent->nickserv.stored[expired_nicks[i]].Name().c_str()));
		    Parent->nickserv.stored[expired_nicks[i]].Drop();
		    Parent->nickserv.stored.erase(expired_nicks[i]);
		}
	    }

	    // channels
	    {
		MLOCK(("ChanServ", "stored"));
		vector<mstring> expired_chans;
		for (csi = Parent->chanserv.stored.begin();
			csi != Parent->chanserv.stored.end(); csi++)
		{
		    if (!csi->second.NoExpire())
		    {
			if (csi->second.LastUsed().SecondsSince() >
			    Parent->chanserv.Expire())
			    expired_chans.push_back(csi->first);
		    }
		}
		for (i=0; i<expired_chans.size(); i++)
		{
		    wxLogInfo(Parent->getLogMessage("EVENT/EXPIRE_CHAN"),
			    Parent->chanserv.stored[expired_chans[i]].Name().c_str(),
			    Parent->chanserv.stored[expired_chans[i]].Founder().c_str());
		    Parent->chanserv.stored.erase(expired_chans[i]);
		}
	    }

	    // news articles
	    {
		MLOCK(("MemoServ", "channel"));
		for (ni=Parent->memoserv.channel.begin();
			ni!=Parent->memoserv.channel.end(); ni++)
		{
		    bool firstgone = false;
		    for (lni=ni->second.begin();
			ni->second.size() &&
			(lni != ni->second.end() || firstgone); lni++)
		    {
			if (firstgone)
			{
			    firstgone = false;
			    lni = ni->second.begin();
			}
			if (lni->Time().SecondsSince() >
			    Parent->memoserv.News_Expire())
			{
			    if (lni == ni->second.begin())
			    {
				wxLogVerbose(Parent->getLogMessage("EVENT/EXPIRE_NEWS"),
					lni->Text().c_str());
				ni->second.erase(lni);
				firstgone = true;
				lni = ni->second.end();
			    }
			    else
			    {
				list<News_t>::iterator lastnews = lni;
				wxLogVerbose(Parent->getLogMessage("EVENT/EXPIRE_NEWS"),
					lni->Text().c_str());
				lastnews--;
				ni->second.erase(lni);
				lni = lastnews;
			    }
			}
		    }
		}
	    }

	    // transaction ID's
	    TxnIds::Expire();

	    last_expire = Now();
	}

	if (last_save.SecondsSince() >= Parent->config.Cycletime())
	{
	    CP(("Starting DATABASE SAVE ..."));
	    Parent->save_databases();

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
		    Parent->chanserv.DEF_Bantime())
		{
		    unsigned long bantime = 0;
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

		chunked.clear();
		if (csi != Parent->chanserv.stored.end())
		{
		    for (di=cli->second.recent_parts.begin();
				di!=cli->second.recent_parts.end(); di++)
		    {
			if (di->second.SecondsSince() > csi->second.Parttime())
			    chunked.push_back(di->first);
		    }
		}
		else
		{
		    cli->second.recent_parts.clear();
		}
		for (i=0; i<chunked.size(); i++)
		    cli->second.recent_parts.erase(chunked[i]);

		// Send pending ChanServ modes ...
		// Make sure we got someone to send them first.
		if (Parent->nickserv.IsLive(Parent->chanserv.FirstName()))
		{
		    if (cli->second.p_modes_on != "" || cli->second.p_modes_off != "")
		    {
			unsigned int j, k;
			vector<mstring> modelines;
			mstring mode;
			mstring modeparam;

			for (i=0, j=0, k=0; i<cli->second.p_modes_off.size(); i++)
			{
			    if (j>=Parent->server.proto.Modes())
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
				j++; k++;
			    }
			}
			cli->second.p_modes_off = "";
			cli->second.p_modes_off_params.clear();
			if (mode.size() && cli->second.p_modes_on.size())
			    mode += "+";
			for (i=0, k=0; i<cli->second.p_modes_on.size(); i++)
			{
			    if (j>=Parent->server.proto.Modes())
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
				j++; k++;
			    }
			}
			cli->second.p_modes_on = "";
			cli->second.p_modes_on_params.clear();
			if (mode.size())
			    modelines.push_back(mode + " " + modeparam);
			for (i=0; i<modelines.size(); i++)
			{
			    Parent->server.MODE(Parent->chanserv.FirstName(),
				cli->first, modelines[i]);
			}
		    }
		}
	    }

	    // Check if we should rename people who are past their
	    // grace time on ident (if KillProtect is on, and they
	    // are not on access list or secure is on).
	    chunked.clear();
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
		wxLogInfo(Parent->getLogMessage("EVENT/KILLPROTECT"),
			nli->second.Mask(Nick_Live_t::N_U_P_H).c_str());
		if (newnick != "" && Parent->server.proto.SVS())
		{
		    Parent->server.SVSNICK(Parent->nickserv.FirstName(),
			oldnick, newnick);
		    send(Parent->nickserv.FirstName(), newnick,
			Parent->getMessage(newnick, "MISC/RENAMED"));
		}
		else
		{
		    Parent->server.KILL(Parent->nickserv.FirstName(),
			oldnick, Parent->getMessage("NS_SET/PROTECT"));
		    Parent->server.NICK(oldnick, (Parent->startup.Ownuser() ?
				    oldnick.LowerCase() :
				    Parent->startup.Services_User()),
				    Parent->startup.Services_Host(),
				    Parent->startup.Server_Name(),
				    Parent->nickserv.Enforcer_Name());
		}
		Parent->nickserv.recovered[oldnick.LowerCase()] = Now();
	    }

	    // Sign off clients we've decided to take.
	    chunked.clear();
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
	    double min = -1, max = 0, sum = 0, avg = 0;
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
		avg = (sum - min - max) / (double)(pingtimes.size() - 2);
	    else
		avg = sum / (double) pingtimes.size();

	    if (avg > (double)(Parent->startup.Lagtime() * (Parent->Level() - Parent->startup.Level() + 1)))
	    {
		Parent->LevelUp();
		wxLogWarning(Parent->getLogMessage("EVENT/LEVEL_UP"), avg);
	    }
	    else if (Parent->Level() > Parent->startup.Level() &&
		avg <= (double)(Parent->startup.Lagtime() * (Parent->Level() - Parent->startup.Level())))
	    {
		Parent->LevelDown();
		wxLogWarning(Parent->getLogMessage("EVENT/LEVEL_DOWN"), avg);
	    }

	    for (si=Parent->server.ServerList.begin();
		    si!=Parent->server.ServerList.end();si++)
		si->second.Ping();
	    wxLogVerbose(Parent->getLogMessage("EVENT/PING"));
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

