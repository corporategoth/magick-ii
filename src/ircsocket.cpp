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
** Revision 1.126  2000/08/28 10:51:37  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.125  2000/08/22 08:43:41  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.124  2000/08/19 10:59:47  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.123  2000/08/10 22:44:23  prez
** Added 'binding to IP' options for shell servers, etc.  Also added akick
** triggers for when a user changes their nick and suddenly matches akick.
**
** Revision 1.122  2000/08/07 22:38:50  prez
** Stopped the 'expiring nickname  ()' messages.
**
** Revision 1.121  2000/08/07 12:20:27  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.120  2000/08/06 08:06:41  prez
** Fixed loading of logon messages in committee ..
**
** Revision 1.119  2000/08/06 05:27:47  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.118  2000/08/03 13:06:31  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.117  2000/07/30 09:04:05  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.116  2000/07/29 21:58:53  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.115  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.114  2000/06/25 11:58:03  prez
** Fixed problem where messages from nickserv about killing user would not
** be sent out (people would not know a nick was forbidden).
**
** Revision 1.113  2000/06/25 10:32:41  prez
** Fixed channel forbid.
**
** Revision 1.112  2000/06/25 07:58:50  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.111  2000/06/18 13:31:48  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.110  2000/06/18 12:49:27  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.109  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.108  2000/06/08 13:07:34  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.107  2000/06/06 08:57:56  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.106  2000/05/27 15:10:12  prez
** Misc changes, mainly re-did the makefile system, makes more sense.
** Also added a config.h file.
**
** Revision 1.105  2000/05/27 07:06:01  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.104  2000/05/26 11:21:28  prez
** Implemented HTM (High Traffic Mode) -- Can be used at a later date.
**
** Revision 1.103  2000/05/22 13:00:09  prez
** Updated version.h and some other stuff
**
** Revision 1.102  2000/05/21 04:49:40  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.101  2000/05/20 15:17:00  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.100  2000/05/20 03:28:11  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.99  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.98  2000/05/14 06:30:14  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.97  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
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
    mThread::Attach(tt_MAIN);
    FT("IrcSvcHandler::open", (in));
    CP(("Socket opened"));
    ACE_Reactor::instance()->register_handler(this,ACE_Event_Handler::READ_MASK);
    //activate();
    // todo activate the task
    in_traffic = out_traffic = 0;
    connect_time = Now();
    htm_level = 0;
    htm_gap = Parent->operserv.Init_HTM_Gap();
    htm_threshold = Parent->operserv.Init_HTM_Thresh();
    last_htm_check = Now();

    // Only activate the threads when we're ready.
    mBase::init();
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
    ACE_OS::memset(data,0,513);
    recvResult=peer().recv(data,512);
    if(recvResult<=0 || Parent->Shutdown())
	return -1;

    // possibly mstring(data,0,recvResult); rather than mstring(data)
    // depends on null terminators etc.

    // Traffic Accounting ...
    map<time_t, size_t>::iterator iter;
    time_t now = time(NULL);
    
    { WLOCK(("IrcSvcHandler", "traffic"));
    for (iter=traffic.begin(); iter != traffic.end() &&
		iter->first < now - (Parent->operserv.Max_HTM_Gap()+2); iter = traffic.begin())
	traffic.erase(iter->first);
    if (traffic.find(now) == traffic.end())
	traffic[now] = 0;
    traffic[now] += ACE_OS::strlen(data);
    in_traffic += ACE_OS::strlen(data);
    }

    // Check to see if we're in HTM.
    { WLOCK(("IrcSvcHandler", "htm_gap"));
    WLOCK2(("IrcSvcHandler", "htm_level"));
    WLOCK3(("IrcSvcHandler", "last_htm_check"));
    if (last_htm_check.SecondsSince() > htm_gap)
    {
	last_htm_check = Now();
	size_t total = 0;
	time_t i;
	{ RLOCK(("IrcSvcHandler", "traffic"));
	for (i = now - (htm_gap + 1); i<now; i++)
	    if (traffic.find(i) != traffic.end())
		total += traffic[i];
	}
	RLOCK(("IrcSvcHandler", "htm_threshold"));
	if (total > (htm_gap * htm_threshold))
	{
	    if (htm_gap > Parent->operserv.Max_HTM_Gap())
	    {
		announce(Parent->operserv.FirstName(),
			Parent->getMessage("MISC/HTM_DIE"));
		CP(("HTM gap limit reached"));
		return -1;
	    }
	    else
	    {
		if (!htm_level)
		    announce(Parent->operserv.FirstName(),
			Parent->getMessage("MISC/HTM_ON"),
			(float) total / (float) htm_gap / 1024.0,
			(float) htm_threshold / 1024.0);
		else if (htm_level < 3)
		    announce(Parent->operserv.FirstName(),
			Parent->getMessage("MISC/HTM_STILL"),
			htm_level + 1, htm_gap,
			(float) total / (float) htm_gap / 1024.0);
		else
		    announce(Parent->operserv.FirstName(),
			Parent->getMessage("MISC/HTM_TURBO"),
			htm_level + 1, htm_gap,
			(float) total / (float) htm_gap / 1024.0);
		htm_level++;
		htm_gap += 2;
		Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/HTM_ON"),
			htm_level, htm_gap,
			(float) total / (float) htm_gap / 1024.0,
			(float) htm_threshold / 1024.0);
	    }
	}
	else if (htm_level)
	{
	    announce(Parent->operserv.FirstName(),
		Parent->getMessage("MISC/HTM_OFF"));
	    htm_level = 0;
	    htm_gap = Parent->operserv.Init_HTM_Gap();
	    Log(LM_NOTICE, Parent->getLogMessage("OPERSERV/HTM_OFF"));
	}
    }}

    unsigned int i;
    mstring data2 = flack + data;
    flack = "";
    // if(recvResult==-1) major problem.
    // if(recvResult==0) socket has close down    

    if(data2.Contains("\n")||data2.Contains("\r"))
    {
	for(i=1;i<data2.WordCount("\n\r");i++)
	{
	    mstring text = data2.ExtractWord(i,"\n\r");
	    if(text!="")
	    {
		if (text.SubString(0, 4) == "PING ")
		    mBase::push_message_immediately(text);
		else
		    mBase::push_message(text);
	    }
	}

	if (data2.Last() == '\n' || data2.Last() == '\r')
	{
	    mstring text = data2.ExtractWord(i,"\n\r");
	    if(text!="")
	    {
		if (text.SubString(0, 4) == "PING ")
		    mBase::push_message_immediately(text);
		else
		    mBase::push_message(text);
	    }
	}
	else
	{
	    flack = data2.ExtractWord(i, "\n\r");
	}

	FLUSH();
    }
    else
        flack = data2;

    RET(0);
}

int IrcSvcHandler::handle_close(ACE_HANDLE hin, ACE_Reactor_Mask mask)
{
    FT("IrcSvcHandler::handle_close", ("(ACE_HANDLE hin)", "(ACE_Reactor_Mask) mask"));
    CP(("IrcSvcHandler closed, scheduling reconnect, then closing down"));

    // We DONT want any processing once we're gone ... nowhere to send
    // back the messages (duh!).
    mBase::shutdown();
    // Should I do this with SQUIT protection ...?
    Parent->nickserv.live.clear();
    Parent->chanserv.live.clear();

    if(!(Parent->config.Server_Relink()<1 || !Parent->Reconnect() ||
	    Parent->Shutdown()) && Parent->Connected())
    {
	Parent->Connected(false);
	if(Parent->Reconnect() && !Parent->Shutdown())
	    ACE_Reactor::instance()->schedule_timer(&(Parent->rh),0,ACE_Time_Value(Parent->config.Server_Relink()));
    }

    DRET(0);
}

time_t IrcSvcHandler::HTM_Gap()
{
    NFT("IrcSvcHandler::HTM_Gap");
    RLOCK(("IrcSvcHandler", "htm_gap"));
    RET(htm_gap);
}

unsigned short IrcSvcHandler::HTM_Level()
{
    NFT("IrcSvcHandler::HTM_Level");
    RLOCK(("IrcSvcHandler", "htm_level"));
    RET(htm_level);
}

size_t IrcSvcHandler::HTM_Threshold()
{
    NFT("IrcSvcHandler::HTM_Threshold");
    RLOCK(("IrcSvcHandler", "htm_threshold"));
    RET(htm_threshold);
}

void IrcSvcHandler::HTM_Threshold(size_t in)
{
    FT("IrcSvcHandler::HTM_Threshold", (in));
    WLOCK(("IrcSvcHandler", "htm_threshold"));
    htm_threshold = in;
}


void IrcSvcHandler::HTM(bool in)
{
    FT("IrcSvcHandler::HTM", (in));
    WLOCK(("IrcSvcHandler", "last_htm_check"));
    WLOCK2(("IrcSvcHandler", "htm_level"));
    WLOCK3(("IrcSvcHandler", "htm_gap"));
    last_htm_check = Now();
    if (in)
    {
	htm_level = 4;
	htm_gap = Parent->operserv.HTM_On_Gap();
    }
    else
    {
	htm_level = 0;
	htm_gap = Parent->operserv.Init_HTM_Gap();
    }
}

size_t IrcSvcHandler::Average(time_t secs)
{
    FT("IrcSvcHandler::Average", (secs));
    time_t now = time(NULL);
    size_t total = 0;
    int i = 0;
    map<time_t, size_t>::iterator iter;
    if (secs > Parent->operserv.Max_HTM_Gap())
	secs = 0;
    RLOCK(("IrcSvcHandler", "traffic"));
    for (iter=traffic.begin(); iter != traffic.end() &&
			iter->first < now; iter++)
    {
	if (secs ? iter->first >= (now-1) - secs : 1)
	{
	    total += iter->second;
	    i++;
	}
    }
    RET(total / (i ? i : 1));
}

int IrcSvcHandler::send(const mstring & data)
{
    FT("IrcSvcHandler::send",(data));
    int recvResult;
    out_traffic += data.Len();
    recvResult=peer().send((data + "\r\n").c_str(),data.Len()+2);
    CH(T_Chatter::To,data);
    RET(recvResult);
}

int IrcSvcHandler::close(unsigned long in)
{
    FT("IrcSvcHandler::close",(in));
    // todo: shutdown the ping timer
    CP(("Socket closed"));
    int retval = 0;
//    int retval = handle_close();
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
	    if (*iter == server)
		break;

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
	if (Parent->Server() != "")
	    server = FindNext(Parent->Server());
	if (server == "") {
	    server = Parent->startup.PriorityList(1)[0];
	}
    }
    if (server != "")
	details = Parent->startup.Server(server);

    ACE_INET_Addr addr(details.first, server);

    Parent->GotConnect(false);
    Parent->i_server = server;
    Parent->server.proto.Tokens(false);
    WLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL)
    {
	Parent->ircsvchandler->shutdown();
	delete Parent->ircsvchandler;
	Parent->ircsvchandler = NULL;
    }
    Parent->Connected(false);
    Parent->ircsvchandler=new IrcSvcHandler;
    Log(LM_INFO, Parent->getLogMessage("OTHER/CONNECTING"),
		server.c_str(), details.first);

    IrcServer C_server(ACE_Reactor::instance(),ACE_NONBLOCK);

    unsigned int i;
    for (i=1; i<5; i++)
    {
        mstring octet = Parent->startup.Bind().ExtractWord(i, ".", false);
	if (!octet.IsNumber() || atoi(octet.c_str()) < 0 ||
		atoi(octet.c_str()) > 255)
	    break;
    }

    ACE_INET_Addr laddr;
    unsigned short port = FindAvailPort();
    if (i==5)
	laddr.set(port, Parent->startup.Bind().c_str());
    else
	laddr.set(port);
    if(C_server.connect(Parent->ircsvchandler,addr,
    		ACE_Synch_Options::defaults, laddr)==-1)
    {
	if (Parent->ircsvchandler != NULL)
	{
	    Parent->ircsvchandler->shutdown();
	    delete Parent->ircsvchandler;
	    Parent->ircsvchandler = NULL;
	}
	Log(LM_ERROR, Parent->getLogMessage("OTHER/REFUSED"),
		server.c_str(), details.first);
	//okay we got a connection problem here. log it and try again
	CP(("Refused connection, rescheduling and trying again ..."));
	ACE_Reactor::instance()->schedule_timer(&(Parent->rh),0,ACE_Time_Value(Parent->config.Server_Relink()));
    }
    else
    {
        ACE_INET_Addr localaddr;
	Parent->ircsvchandler->peer().get_local_addr(localaddr);
	CP(("Local connection point=%s port:%u",localaddr.get_host_name(),localaddr.get_port_number()));
	Parent->i_localhost = localaddr.get_ip_address();
	//Parent->ircsvchandler->open(NULL);
	if (Parent->server.proto.TSora())
	    Parent->server.raw("PASS " + details.second + " :TS");
	else
	    Parent->server.raw("PASS " + details.second);
	if (Parent->server.proto.Protoctl() != "")
	    Parent->server.raw(Parent->server.proto.Protoctl());
	mstring tmp;
	tmp.Format(Parent->server.proto.Server().c_str(),
	    Parent->startup.Server_Name().c_str(), 1,
	    Parent->startup.Server_Desc().c_str());
	Parent->server.raw(tmp);
	if (Parent->server.proto.TSora())
	    // SVINFO <TS_CURRENT> <TS_MIN> <STANDALONE> :<UTC-TIME>
	    Parent->server.raw("SVINFO 3 1 0 :" + mstring(itoa(time(NULL))));
	Parent->Connected(true);
    }
    RET(0);
}

int ToBeSquit_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    // We ONLY get here if we didnt receive a SQUIT message in <10s
    // after any QUIT message with 2 valid servers in it
    FT("ToBeSquit_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    mstring *tmp = (mstring *) arg;

    { WLOCK(("Server", "ServerSquit"));
    Parent->server.ServerSquit.erase(*tmp);
    }

    if (Parent->server.IsServer(*tmp))
	Log(LM_NOTICE, Parent->getLogMessage("OTHER/SQUIT_CANCEL"),
		tmp->c_str(),
		Parent->server.ServerList[tmp->LowerCase()].Uplink().c_str());
    else
	Log(LM_NOTICE, Parent->getLogMessage("OTHER/SQUIT_CANCEL"),
		tmp->c_str(), "?");

    // QUIT all user's who faked it ...
    WLOCK2(("Server", "ToBeSquit"));
    if (Parent->server.ToBeSquit.find(*tmp) != Parent->server.ToBeSquit.end())
    {
	list<mstring>::iterator iter;
	for (iter=Parent->server.ToBeSquit[*tmp].begin(); iter!=Parent->server.ToBeSquit[*tmp].end(); iter++)
	{
	    if (Parent->nickserv.IsLive(*iter))
	    {
		Parent->nickserv.live[*iter].Quit("FAKE SQUIT - " + *tmp);
		WLOCK(("NickServ", "live"));
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
    mstring *tmp = (mstring *) arg;

    { WLOCK(("Server", "ServerSquit"));
    WLOCK2(("Server", "ToBeSquit"));
    Parent->server.ServerSquit.erase(*tmp);
    Parent->server.ToBeSquit.erase(*tmp);
    }

    // QUIT all user's who did not come back from SQUIT
    map<mstring,Nick_Live_t>::iterator i;
    vector<mstring> SquitMe;
    vector<mstring>::iterator k;
    { RLOCK(("NickServ", "live"));
    for (i=Parent->nickserv.live.begin(); i != Parent->nickserv.live.end(); i++)
    {
	if (i->second.Squit() == *tmp)
	{
	    SquitMe.push_back(i->first);
	}
    }}
    for (k=SquitMe.begin(); k != SquitMe.end(); k++)
    {
	if (Parent->nickserv.IsLive(*k))
	{
	    Parent->nickserv.live[*k].Quit("SQUIT - " + *tmp);
	    WLOCK(("NickServ", "live"));
	    Parent->nickserv.live.erase(*k);
	}
    }

    delete tmp;
    RET(0);
}


int InFlight_Handler::handle_timeout (const ACE_Time_Value &tv, const void *arg)
{
    // Memo timed out, send it!
    // If its a file, and not inprogress, ignore.
    FT("InFlight_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    mstring *tmp = (mstring *) arg;
    Nick_Live_t *entry;

    if (Parent->nickserv.IsLive(*tmp))
    {
	entry = &Parent->nickserv.live[tmp->LowerCase()];
	if (entry->InFlight.File())
	{
	    if (!entry->InFlight.InProg())
	    {
		send(entry->InFlight.service, entry->Name(),
			Parent->getMessage(entry->Name(), "DCC/NOCONNECT"), "GET");
		entry->InFlight.Cancel();
	    }
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

    // This is to part channels I'm not supposed to be
    // in (ie. dont have JOIN on, and I'm the only user
    // in them).  ie. after AKICK, etc.
    if (Parent->chanserv.IsLive(*tmp) &&
	Parent->chanserv.live[tmp->LowerCase()].IsIn(
			Parent->chanserv.FirstName()))
    {
	Parent->server.PART(Parent->chanserv.FirstName(), *tmp);
	MLOCK(("ChanServ", "live", tmp->LowerCase(), "ph_timer"));
	Parent->chanserv.live[tmp->LowerCase()].ph_timer = 0;
    }
    delete tmp;
    RET(0);
}

void *EventTask::save_databases(void *in)
{
    Parent->save_databases();
    return NULL;
}

void EventTask::ForceSave()
{
    NFT("EventTask::ForceSave");
    WLOCK(("Events", "last_save"));
    last_save = mDateTime(0.0);
}

void EventTask::ForcePing()
{
    NFT("EventTask::ForcePing");
    WLOCK(("Events", "last_ping"));
    last_ping = mDateTime(0.0);
}

mstring EventTask::SyncTime()
{
    NFT("EventTask::SyncTime");
    RLOCK(("Events", "last_save"));
    mstring retval = ToHumanTime(Parent->config.Savetime() - (unsigned long)last_save.SecondsSince());
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
    // The biggie, so big, it has its own zip code ... uhh .. thread.
    mThread::Attach(tt_MAIN);
    NFT("EventTask::svc");

    { WLOCK(("Events", "last_expire"));
    WLOCK2(("Events", "last_save"));
    WLOCK3(("Events", "last_check"));
    WLOCK4(("Events", "last_ping"));
    last_expire = last_save = last_check = last_ping = Now();
    }

    ACE_Thread_Manager tm;
    while(!Parent->Shutdown())
    {
	// Make sure we're turned on ...
	if (!Parent->AUTO())
	{
	    ACE_OS::sleep(1);
	    continue;
	}

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

	{ RLOCK(("Events", "last_expire"));
	if (last_expire.SecondsSince() >= Parent->config.Cycletime())
	{
	    CP(("Starting EXPIRATION check ..."));

	    // akills
	    {
		MLOCK(("OperServ","Akill"));
		bool firstgone = false;
		for (Parent->operserv.Akill = Parent->operserv.Akill_begin();
			(Parent->operserv.Akill_size()) &&
			(firstgone || Parent->operserv.Akill != Parent->operserv.Akill_end());
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
			    Log(LM_INFO, Parent->getLogMessage("EVENT/EXPIRE_AKILL"),
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
			    Log(LM_INFO, Parent->getLogMessage("EVENT/EXPIRE_AKILL"),
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
		vector<mstring> expired_nicks;
		{ RLOCK2(("NickServ", "stored"));
		for (nsi = Parent->nickserv.stored.begin();
			nsi != Parent->nickserv.stored.end(); nsi++)
		{
		    if (!(nsi->second.NoExpire() || nsi->second.Forbidden() ||
			nsi->second.Suspended()))
		    {
			if (nsi->second.Host() == "")
			{
			    if (nsi->second.LastAllSeenTime().SecondsSince() >
				Parent->nickserv.Expire())
			    {
				expired_nicks.push_back(nsi->first);
			    }
			}
			else
			{
			    if (nsi->second.LastSeenTime().SecondsSince() >
				Parent->nickserv.Expire())
			    {
				expired_nicks.push_back(nsi->first);
			    }
			}
		    }
		}}
		WLOCK(("NickServ", "stored"));
		for (i=0; i<expired_nicks.size(); i++)
		{
		    if (Parent->nickserv.stored[expired_nicks[i]].Name() != "")
			Log(LM_INFO, Parent->getLogMessage("EVENT/EXPIRE_NICK"),
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
		vector<mstring> expired_chans;
		{ RLOCK2(("ChanServ", "stored"));
		for (csi = Parent->chanserv.stored.begin();
			csi != Parent->chanserv.stored.end(); csi++)
		{
		    if (!(csi->second.NoExpire() || csi->second.Forbidden() ||
			csi->second.Suspended()))
		    {
			if (csi->second.LastUsed().SecondsSince() >
			    Parent->chanserv.Expire())
			    expired_chans.push_back(csi->first);
		    }
		}}
		WLOCK(("ChanServ", "stored"));
		for (i=0; i<expired_chans.size(); i++)
		{
		    if (Parent->chanserv.stored[expired_chans[i]].Name() != "")
			Log(LM_INFO, Parent->getLogMessage("EVENT/EXPIRE_CHAN"),
			    Parent->chanserv.stored[expired_chans[i]].Name().c_str(),
			    Parent->chanserv.stored[expired_chans[i]].Founder().c_str());
		    Parent->chanserv.stored.erase(expired_chans[i]);
		}
	    }

	    // news articles
	    {
		vector<mstring> expired_news;
		{ RLOCK2(("MemoServ", "channel"));
		for (ni=Parent->memoserv.channel.begin();
			ni!=Parent->memoserv.channel.end(); ni++)
		{
		    bool firstgone = false;
		    WLOCK(("MemoServ", "channel", ni->first));
		    for (lni=ni->second.begin();
			(ni->second.size()) &&
			(firstgone || lni != ni->second.end()); lni++)
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
				Log(LM_DEBUG, Parent->getLogMessage("EVENT/EXPIRE_NEWS"),
					lni->Text().c_str());
				ni->second.erase(lni);
				firstgone = true;
				lni = ni->second.end();
			    }
			    else
			    {
				list<News_t>::iterator lastnews = lni;
				Log(LM_DEBUG, Parent->getLogMessage("EVENT/EXPIRE_NEWS"),
					lni->Text().c_str());
				lastnews--;
				ni->second.erase(lni);
				lni = lastnews;
			    }
			}
		    }
		    if (!ni->second.size())
			expired_news.push_back(ni->first);
		}}
		{ WLOCK(("MemoServ", "channel"));
		for (i=0; i<expired_news.size(); i++)
		{
		    Parent->memoserv.channel.erase(expired_news[i]);
		}}
	    }

	    // transaction ID's (for inter-magick)
	    TxnIds::Expire();

	    WLOCK(("Events", "last_expire"));
	    last_expire = Now();
	}}

	{ RLOCK(("Events", "last_save"));
	if (Parent->Saving())
	    last_save = Now();
	if (last_save.SecondsSince() >= Parent->config.Savetime())
	{
	    CP(("Starting DATABASE SAVE ..."));
	    tm.spawn(save_databases, NULL);

	    WLOCK(("Events", "last_save"));
	    last_save = Now();
	}}

	{ RLOCK(("Events", "last_check"));
	if (last_check.SecondsSince() >= Parent->config.Checktime())
	{
	    CP(("Starting CHECK cycle ..."));

	    if (Parent->nickserv.IsLive(Parent->chanserv.FirstName()))
	    { RLOCK2(("ChanServ", "live"));
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
			RLOCK3(("ChanServ", "live", cli->first, "bans"));
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
		    WLOCK(("ChanServ", "live", cli->first, "recent_parts"));
		    for (di=cli->second.recent_parts.begin();
				di!=cli->second.recent_parts.end(); di++)
		    {
			if (di->second.SecondsSince() > csi->second.Parttime())
			    chunked.push_back(di->first);
		    }
		    for (i=0; i<chunked.size(); i++)
			cli->second.recent_parts.erase(chunked[i]);
		}
		else
		{
		    WLOCK(("ChanServ", "live", cli->first, "recent_parts"));
		    cli->second.recent_parts.clear();
		}

		// Send pending ChanServ modes ...
		// Make sure we got someone to send them first.
		{
		    RLOCK(("ChanServ", "live", cli->first, "p_modes_on"));
		    RLOCK2(("ChanServ", "live", cli->first, "p_modes_off"));
		    RLOCK3(("ChanServ", "live", cli->first, "p_modes_on_params"));
		    RLOCK4(("ChanServ", "live", cli->first, "p_modes_on_params"));
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
			    if (cli->second.p_modes_off[i] != 'l' &&
				Parent->server.proto.ChanModeArg().Contains(cli->second.p_modes_off[i]))
			    {
				if (modeparam != "")
				    modeparam += " ";
				modeparam +=  cli->second.p_modes_off_params[k];
				j++; k++;
			    }
			}
			{ WLOCK(("ChanServ", "live", cli->first, "p_modes_off"));
			WLOCK2(("ChanServ", "live", cli->first, "p_modes_off_params"));
			cli->second.p_modes_off = "";
			cli->second.p_modes_off_params.clear();
			}
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
			    if (Parent->server.proto.ChanModeArg().Contains(cli->second.p_modes_on[i]))
			    {
				if (modeparam != "")
				    modeparam += " ";
				modeparam += cli->second.p_modes_on_params[k];
				j++; k++;
			    }
			}
			{ WLOCK(("ChanServ", "live", cli->first, "p_modes_on"));
			WLOCK2(("ChanServ", "live", cli->first, "p_modes_on_params"));
			cli->second.p_modes_on = "";
			cli->second.p_modes_on_params.clear();
			}
			if (mode.size())
			    modelines.push_back(mode + " " + modeparam);
			for (i=0; i<modelines.size(); i++)
			{
			    Parent->server.MODE(Parent->chanserv.FirstName(),
				cli->first, modelines[i]);
			}
		    }
		}
	    }}

	    // Check if we should rename people who are past their
	    // grace time on ident (if KillProtect is on, and they
	    // are not on access list or secure is on).
	    chunked.clear();
	    if (Parent->nickserv.IsLive(Parent->nickserv.FirstName()))
	    { RLOCK(("NickServ", "live"));
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
	    }}
	    for (i=0; i<chunked.size(); i++)
	    {
		nli = Parent->nickserv.live.find(chunked[i]);
		nsi = Parent->nickserv.stored.find(chunked[i]);
		mstring newnick = Parent->nickserv.findnextnick(nli->second.Name());
		mstring oldnick = nli->second.Name();
		Log(LM_INFO, Parent->getLogMessage("EVENT/KILLPROTECT"),
			nli->second.Mask(Nick_Live_t::N_U_P_H).c_str());
		if (newnick != "" && Parent->server.proto.SVS())
		{
		    if (nsi->second.Forbidden())
			send(Parent->nickserv.FirstName(), oldnick,
				Parent->getMessage("MISC/RENAMED_FORBID"));
		    else
			send(Parent->nickserv.FirstName(), oldnick,
				Parent->getMessage("MISC/RENAMED_IDENT"));
		    Parent->server.SVSNICK(Parent->nickserv.FirstName(),
			oldnick, newnick);
		}
		else
		{
		    if (nsi->second.Forbidden())
			Parent->server.KILL(Parent->nickserv.FirstName(),
				oldnick, Parent->getMessage("NS_YOU_STATUS/ISFORBIDDEN"));
		    else
			Parent->server.KILL(Parent->nickserv.FirstName(),
				oldnick, Parent->getMessage("NS_SET/PROTECT"));
		    Parent->server.NICK(oldnick, (Parent->startup.Ownuser() ?
				    oldnick.LowerCase() :
				    Parent->startup.Services_User()),
				    Parent->startup.Services_Host(),
				    Parent->startup.Server_Name(),
				    Parent->nickserv.Enforcer_Name());
		}
		WLOCK(("NickServ", "recovered"));
		Parent->nickserv.recovered[oldnick.LowerCase()] = Now();
	    }

	    // Sign off clients we've decided to take.
	    chunked.clear();
	    { RLOCK(("NickServ", "recovered"));
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
	    }}
	    { WLOCK(("NickServ", "recovered"));
	    for (i=0; i<chunked.size(); i++)
		Parent->nickserv.recovered.erase(chunked[i]);
	    }
	    WLOCK(("Events", "last_check"));
	    last_check = Now();
	}}

	{ RLOCK(("Events", "last_ping"));
	if (last_ping.SecondsSince() >= Parent->config.Ping_Frequency())
	{
	    CP(("Starting SERVER PING ..."));

	    vector<double> pingtimes;
	    double min = -1, max = 0, sum = 0, avg = 0;
	    { RLOCK2(("Server", "ServerList"));
	    for (si=Parent->server.ServerList.begin();
		    si!=Parent->server.ServerList.end(); si++)
	    {
		if (min == -1 || si->second.Lag() < min)
		    min = si->second.Lag();
		if (si->second.Lag() > max)
		    max = si->second.Lag();
		pingtimes.push_back(si->second.Lag());
		sum += si->second.Lag();
	    }}
	    if (pingtimes.size() >= 3)
		avg = (sum - min - max) / (double)(pingtimes.size() - 2);
	    else
		avg = sum / (double) pingtimes.size();

	    if (avg > (double)(Parent->startup.Lagtime() * (Parent->Level() - Parent->startup.Level() + 1)))
	    {
		Parent->LevelUp();
		Log(LM_WARNING, Parent->getLogMessage("EVENT/LEVEL_UP"), avg);
	    }
	    else if (Parent->Level() > Parent->startup.Level() &&
		avg <= (double)(Parent->startup.Lagtime() * (Parent->Level() - Parent->startup.Level())))
	    {
		Parent->LevelDown();
		Log(LM_WARNING, Parent->getLogMessage("EVENT/LEVEL_DOWN"), avg);
	    }

	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL)
		if (Parent->ircsvchandler->HTM_Level() <= 3)
		{
		    RLOCK2(("Server", "ServerList"));
		    for (si=Parent->server.ServerList.begin();
				si!=Parent->server.ServerList.end();si++)
			si->second.Ping();
		    Log(LM_TRACE, Parent->getLogMessage("EVENT/PING"));
		}
	    }
	    WLOCK(("Events", "last_ping"));
	    last_ping = Now();
	}}
	
	FLUSH(); // Force TRACE output dump
	ACE_OS::sleep(1);
    }
    DRET(0);
}

