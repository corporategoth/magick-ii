#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_ircsocket_cpp_ ## x () { return y; }
RCSID(ircsocket_cpp, "@(#)$Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "magick.h"

const char *Heartbeat_Handler::names[] =
{ "invalid", "worker", "main", "IRC server", "events", "DCC" };

static const char *immediate_process[] =
{ "PROTOCTL", "CAPAB", NULL };

ACE_THR_FUNC_RETURN IrcSvcHandler::worker(void *in)
{
    BTCB();
    Magick::register_instance(reinterpret_cast < Magick * > (in));
    mThread::Attach(tt_mBase);
    FT("IrcSvcHandler::worker", (in));
    bool active = true;

    Magick::instance().hh.AddThread(Heartbeat_Handler::H_Worker);
    try
    {
	while (!Magick::instance().Shutdown() && active)
	{
	    Magick::instance().hh.Heartbeat();

	    ACE_Message_Block *block = NULL;
	    {
		RLOCK((lck_IrcSvcHandler));
		if (Magick::instance().ircsvchandler == NULL)
		    break;
		
		if (Magick::instance().ircsvchandler->message_queue.dequeue(block) < 0)
		    continue;
	    }
	    while (Magick::instance().Pause())
		ACE_OS::sleep(1);
	    if (block != NULL)
	    {
		switch (block->msg_type())
		{
		case ACE_Message_Block::MB_DATA:
		{
		    if (block->base() == NULL)
		    {
			delete block;
			break;
		    }

		    unsigned long msgid = 0;
		    memcpy(&msgid, block->base(), sizeof(unsigned long));
		    delete block->base();
		    delete block;

		    map_entry<mMessage> msg;
		    // Lookup msg and do call ...
		    {
			RLOCK((lck_IrcSvcHandler));
			if (Magick::instance().ircsvchandler == NULL)
			    break;

			MLOCK((lck_IrcSvcHandler, lck_MsgIdMap));
			if (Magick::instance().ircsvchandler->IsMessage(msgid))
			{
			    msg = Magick::instance().ircsvchandler->GetMessage(msgid);
			    if (msg->creation().SecondsSince() > Magick::instance().config.MSG_Seen_Time() || !msg->OutstandingDependancies())
				Magick::instance().ircsvchandler->RemMessage(msgid);
			    else
				break;
			}
		    }
		    if (msg != NULL)
		    {
			try
			{
			    if (msg->call() < 0)
				active = false;
			}
			catch (E_Lock & e)
			{
			    static_cast < void > (e);

			    RLOCK((lck_IrcSvcHandler));
			    if (Magick::instance().ircsvchandler == NULL)
				break;
			    Magick::instance().ircsvchandler->enqueue(msgid, P_Retry);
			}
		    }

		    break;
		}
		case ACE_Message_Block::MB_PROTO:
		{
		    ACE_Message_Block::Message_Flags flags = block->flags();
		    delete block;
		    if (flags & (ACE_Message_Block::USER_FLAGS + IrcSvcHandler::FLAG_SLEEP))
		    {
			ACE_OS::sleep(1);
		    }
		    if (flags & (ACE_Message_Block::USER_FLAGS + IrcSvcHandler::FLAG_YIELD))
		    {
			ACE_Thread::yield();
		    }
		    break;
		}
		case ACE_Message_Block::MB_HANGUP:
		    delete block;
		    active = false;
		    break;
		default:
		    delete block;
		}
	    }

	    size_t msgcnt = 0, thrcnt = 0;

	    {
		RLOCK((lck_IrcSvcHandler));
		if (Magick::instance().ircsvchandler != NULL)
		{
		    msgcnt = Magick::instance().ircsvchandler->message_queue.message_count();
		    thrcnt = Magick::instance().ircsvchandler->tm.count_threads();
		}
	    }

	    CP(("thread count = %d, message queue = %d, lwm = %d, hwm = %d", thrcnt, msgcnt,
		Magick::instance().config.Low_Water_Mark() + (Magick::instance().config.High_Water_Mark() * (thrcnt - 2)),
		thrcnt * Magick::instance().config.High_Water_Mark()));

	    if (thrcnt > Magick::instance().config.Min_Threads() &&
		msgcnt <
		Magick::instance().config.Low_Water_Mark() + (Magick::instance().config.High_Water_Mark() * (thrcnt - 2)))
	    {
		COM(("Low water mark reached, killing thread."));
		NLOG(LM_NOTICE, "EVENT/KILL_THREAD");
		active = false;
	    }

	    FLUSH();
	}
    }
    catch (E_Thread & e)
    {
	e.what();
    }

    Magick::instance().hh.RemoveThread();
    Magick::deregister_instance();

    DTRET(0);
    ETCB();
}

int IrcSvcHandler::open(void *in)
{
    BTCB();
    static_cast < void > (in);

    //mThread::Attach(tt_MAIN);
    FT("IrcSvcHandler::open", (in));
#ifndef TEST_MODE
    this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
#endif
    Magick::instance().hh.AddThread(Heartbeat_Handler::H_IrcServer);

    sock.Bind(&Magick::instance().ircsvchandler->peer(), D_From, false);
    sock.Resolve(S_IrcServer, Magick::instance().CurrentServer().first);

    in_traffic = out_traffic = 0;
    connect_time = mDateTime::CurrentDateTime();
    htm_level = 0;
    htm_gap = Magick::instance().operserv.Init_HTM_Gap();
    htm_threshold = Magick::instance().operserv.Init_HTM_Thresh();
    last_htm_check = mDateTime::CurrentDateTime();
    i_burst = true;
    i_synctime = mDateTime(0.0);

    // Only activate the threads when we're ready.
    tm.spawn_n(Magick::instance().config.Min_Threads(), IrcSvcHandler::worker,
	       reinterpret_cast < void * > (&Magick::instance()), THR_NEW_LWP | THR_DETACHED);

    DumpB();
    CP(("IrcSvcHandler activated"));
    RET(0);
    ETCB();
}

int IrcSvcHandler::handle_input(ACE_HANDLE hin)
{
    BTCB();
    static_cast < void > (hin);

    FT("IrcSvcHandler::handle_input", ("(ACE_HANDLE) hin"));

    //todo this is the sucker that get's data from the socket, so this is our main routine.
    // might set this up to be an active object here.
    char data[513];
    int recvResult;

    memset(data, 0, 513);
    recvResult = sock.recv(data, 512);
    if (recvResult <= 0)
	RET(recvResult);

    recvResult = handle_input(data);
    RET(recvResult);
    ETCB();
}

int IrcSvcHandler::handle_input(const char *data)
{
    BTCB();
    mThread::Attach(tt_MAIN);
    FT("IrcSvcHandler::handle_input", ("(ACE_HANDLE) hin"));

    if (Magick::instance().Shutdown())
	DRET(-1);

    // Traffic Accounting ...
    map < time_t, size_t >::iterator iter;
    time_t now = time(NULL);

    DumpB();

    {
	WLOCK((lck_IrcSvcHandler, "traffic"));
	iter = traffic.lower_bound(now - static_cast < time_t > (Magick::instance().operserv.Max_HTM_Gap() + 2));
	traffic.erase(traffic.begin(), iter);
	if (!traffic.count(now))
	    traffic[now] = 0;
	traffic[now] += ACE_OS::strlen(data);
	in_traffic += ACE_OS::strlen(data);
    }

    // Check to see if we're in HTM.
    {
	WLOCK((lck_IrcSvcHandler, "htm_gap"));
	WLOCK2((lck_IrcSvcHandler, "htm_level"));
	WLOCK3((lck_IrcSvcHandler, "last_htm_check"));
	if (static_cast < time_t > (last_htm_check.SecondsSince()) > htm_gap)
	{
	    last_htm_check = mDateTime::CurrentDateTime();
	    size_t total = 0;
	    time_t i;

	    {
		RLOCK((lck_IrcSvcHandler, "traffic"));
		for (i = now - (htm_gap + 1); i < now; i++)
		    if (traffic.find(i) != traffic.end())
			total += traffic[i];
	    }
	    RLOCK((lck_IrcSvcHandler, "htm_threshold"));
	    if (total > (htm_gap * htm_threshold))
	    {
		if (htm_gap > static_cast < time_t > (Magick::instance().operserv.Max_HTM_Gap()))
		{
		    NANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/HTM_DIE");
		    CP(("HTM gap limit reached"));
		    return -1;
		}
		else
		{
		    if (!htm_level)
			ANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/HTM_ON",
				 (fmstring("%.1f", static_cast < float > (total) / static_cast < float > (htm_gap) / 1024.0),
				  fmstring("%.1f", static_cast < float > (htm_threshold) / 1024.0)));
		    else if (htm_level < 3)
			ANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/HTM_STILL",
				 (htm_level + 1, htm_gap,
				  fmstring("%.1f", static_cast < float > (total) / static_cast < float > (htm_gap) / 1024.0)));
		    else
			ANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/HTM_TURBO",
				 (htm_level + 1, htm_gap,
				  fmstring("%.1f", static_cast < float > (total) / static_cast < float > (htm_gap) / 1024.0)));
		    htm_level++;
		    htm_gap += 2;
		    LOG(LM_NOTICE, "OPERSERV/HTM_ON",
			(htm_level, htm_gap,
			 fmstring("%.1f", static_cast < float > (total) / static_cast < float > (htm_gap) / 1024.0),
			 fmstring("%.1f", static_cast < float > (htm_threshold) / 1024.0)));
		}
	    }
	    else if (htm_level)
	    {
		NANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/HTM_OFF");
		htm_level = 0;
		htm_gap = Magick::instance().operserv.Init_HTM_Gap();
		NLOG(LM_NOTICE, "OPERSERV/HTM_OFF");
	    }
	}
    }

    unsigned int i;
    mstring data2 = flack + data;

    flack.erase();
    // if(recvResult==-1) major problem.
    // if(recvResult==0) socket has close down    

    if (data2.Contains("\n") || data2.Contains("\r"))
    {
	for (i = 1; i < data2.WordCount("\n\r"); i++)
	{
	    mstring text = data2.ExtractWord(i, "\n\r");

	    if (!text.empty())
	    {
		enqueue(text);
	    }
	}

	if (data2.last() == '\n' || data2.last() == '\r')
	{
	    mstring text = data2.ExtractWord(i, "\n\r");

	    if (!text.empty())
	    {
		enqueue(text);
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
    DumpE();

    Magick::instance().hh.Heartbeat();
    DRET(0);
    ETCB();
}

int IrcSvcHandler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
    BTCB();
    static_cast < void > (h);
    static_cast < void > (mask);

    mThread::Attach(tt_MAIN);
    FT("IrcSvcHandler::handle_close", ("(ACE_HANDLE hin)", "(ACE_Reactor_Mask) mask"));
    CP(("IrcSvcHandler closed"));

    LOG(LM_ERROR, "OTHER/CLOSED", (Magick::instance().CurrentServer().first, Magick::instance().CurrentServer().second));

    unsigned int i;

    // We DONT want any processing once we're gone ...
    // Dump the queue and kill all our threads nicely.
    for (i = 0; i < static_cast < unsigned int > (tm.count_threads()); i++)
	enqueue_sleep();

    ACE_Time_Value tv(1);

    while (!message_queue.is_empty())
    {
	ACE_Message_Block *block;
	if (message_queue.dequeue(block) >= 0 && block != NULL)
	    delete block;
    }
    for (i = 0; i < static_cast < unsigned int > (tm.count_threads()); i++)
	enqueue_shutdown();

    {
	MLOCK((lck_AllDeps));
	mMessage::AllDependancies.clear();
    }
    {
	MLOCK((lck_IrcSvcHandler, lck_MsgIdMap));
	for_each(MsgIdMap.begin(), MsgIdMap.end(), DeleteMapData());
	MsgIdMap.clear();
    }

    // Should I do this with SQUIT protection ...?
    {
	WLOCK((lck_NickServ, "recovered"));
	Magick::instance().nickserv.recovered.clear();
    }

    // Essentially here, we enact SQUIT protection ...
    {
	WLOCK((lck_Server, "ToBeSquit"));
	WLOCK2((lck_Server, "ServerSquit"));
	Magick::instance().server.DumpB();
	CB(0, Magick::instance().server.ToBeSquit.size());
	CB(1, Magick::instance().server.ServerSquit.size());
	Server::list_t::iterator si;
	{
	    RLOCK((lck_Server, lck_list));
	    for (si = Magick::instance().server.ListBegin(); si != Magick::instance().server.ListEnd(); si++)
	    {
		map_entry < Server_t > serv (si->second);
		RLOCK2((lck_Server, lck_list, serv->Name().LowerCase()));
		if (Magick::instance().server.ToBeSquit.find(serv->Name().LowerCase()) != Magick::instance().server.ToBeSquit.end())
		    Magick::instance().server.ToBeSquit.erase(serv->Name().LowerCase());
		if (Magick::instance().server.ServerSquit.find(serv->Name().LowerCase()) != Magick::instance().server.ServerSquit.end())
		{
		    mstring *arg = NULL;
		    if (Magick::instance().reactor().cancel_timer(Magick::instance().server.ServerSquit[serv->Name().LowerCase()],
								   reinterpret_cast < const void ** > (arg)) && arg != NULL)
			delete arg;
		}
		if (!Magick::instance().Shutdown())
		{
		    while (Magick::instance().Pause())
			ACE_OS::sleep(1);

		    CP(("Scheduling SQUIT protect timer..."));
		    Magick::instance().server.ServerSquit[serv->Name().LowerCase()] = Magick::instance().reactor().schedule_timer(
								&Magick::instance().server.squit, new mstring(serv->Name().LowerCase()),
								ACE_Time_Value(Magick::instance().config.Squit_Protect()));
		}
	    }
	}
	CE(1, Magick::instance().server.ServerSquit.size());
	CE(0, Magick::instance().server.ToBeSquit.size());
	Magick::instance().server.DumpE();
    }

    if (!Magick::instance().Shutdown())
    {
	RLOCK((lck_NickServ, lck_live));
	NickServ::live_t::iterator iter;
	vector < mstring > chunked;
	for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
	{
	    map_entry < Nick_Live_t > nlive(iter->second);
	    if (nlive->IsServices())
	    {
		chunked.push_back(nlive->Name());
	    }
	    else if (Magick::instance().server.IsList(nlive->Server()))
	    {
		nlive->SetSquit();
	    }
	}
	// Sign off services if we have NO uplink
	for (i = 0; i < chunked.size(); i++)
	    Magick::instance().server.QUIT(chunked[i], "SQUIT - " + Magick::instance().startup.Server_Name());
    }

    {
	WLOCK((lck_Server, lck_list));
	Magick::instance().server.i_list.clear();
    }
    Magick::instance().server.OurUplink("");

    // Let other threads process shutdown instruction
    // Otherwise cancel them.

    if (tm.count_threads())
    {
	tv.set(time(NULL) + 4);

	tm.wait(&tv);
	if (tm.count_threads())
	{
#if defined(SIGIOT) && (SIGIOT != 0)
	    tm.kill_all(SIGIOT);
	    tv.set(time(NULL) + 4);
	    tm.wait(&tv);
	    if (tm.count_threads())
#endif
		tm.cancel_all();
	}
    }

    // This sets fini() == 1
    if (sock.IsConnected())
	sock.close();
    Magick::instance().Connected(false);

    if (!Magick::instance().Shutdown() && Magick::instance().Reconnect() && Magick::instance().config.Server_Relink() >= 1)
    {
	CP(("Scheduling reconnect"));
	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);

	Magick::instance().reactor().schedule_timer(&(Magick::instance().rh), NULL,
						    ACE_Time_Value(Magick::instance().config.Server_Relink()));
    }

    if (Magick::instance().hh.ThreadType() == Heartbeat_Handler::H_IrcServer)
	Magick::instance().hh.RemoveThread();
#ifndef TEST_MODE
    this->reactor()->remove_handler(this, ACE_Event_Handler::READ_MASK);
#endif
//  this->destroy();
    DRET(0);
    ETCB();
}

int IrcSvcHandler::fini()
{
    BTCB();
    return sock.IsConnected() ? 0 : 1;
    ETCB();
}

time_t IrcSvcHandler::HTM_Gap() const
{
    BTCB();
    NFT("IrcSvcHandler::HTM_Gap");
    RLOCK((lck_IrcSvcHandler, "htm_gap"));
    RET(htm_gap);
    ETCB();
}

unsigned short IrcSvcHandler::HTM_Level() const
{
    BTCB();
    NFT("IrcSvcHandler::HTM_Level");
    RLOCK((lck_IrcSvcHandler, "htm_level"));
    RET(htm_level);
    ETCB();
}

size_t IrcSvcHandler::HTM_Threshold() const
{
    BTCB();
    NFT("IrcSvcHandler::HTM_Threshold");
    RLOCK((lck_IrcSvcHandler, "htm_threshold"));
    RET(htm_threshold);
    ETCB();
}

void IrcSvcHandler::HTM_Threshold(const size_t in)
{
    BTCB();
    FT("IrcSvcHandler::HTM_Threshold", (in));
    WLOCK((lck_IrcSvcHandler, "htm_threshold"));
    MCB(htm_threshold);
    htm_threshold = in;
    MCE(htm_threshold);
    ETCB();
}

void IrcSvcHandler::HTM(const bool in)
{
    BTCB();
    FT("IrcSvcHandler::HTM", (in));
    WLOCK((lck_IrcSvcHandler, "last_htm_check"));
    WLOCK2((lck_IrcSvcHandler, "htm_level"));
    WLOCK3((lck_IrcSvcHandler, "htm_gap"));
    MCB(last_htm_check);
    CB(1, htm_level);
    CB(2, htm_gap);
    last_htm_check = mDateTime::CurrentDateTime();
    if (in)
    {
	htm_level = 4;
	htm_gap = Magick::instance().operserv.HTM_On_Gap();
    }
    else
    {
	htm_level = 0;
	htm_gap = Magick::instance().operserv.Init_HTM_Gap();
    }
    CE(1, htm_level);
    CE(2, htm_gap);
    MCE(last_htm_check);
    ETCB();
}

size_t IrcSvcHandler::Average(time_t secs) const
{
    BTCB();
    FT("IrcSvcHandler::Average", (secs));
    time_t now = time(NULL);
    size_t total = 0;
    int i = 0;

    map < time_t, size_t >::const_iterator iter;
    if (secs > static_cast < time_t > (Magick::instance().operserv.Max_HTM_Gap()))
	secs = 0;
    RLOCK((lck_IrcSvcHandler, "traffic"));
    for (iter = traffic.begin(); iter != traffic.end() && iter->first < now; iter++)
    {
	if (secs ? iter->first >= (now - 1) - secs : 1)
	{
	    total += iter->second;
	    i++;
	}
    }
    RET(total / (i ? i : 1));
    ETCB();
}

bool IrcSvcHandler::Burst() const
{
    BTCB();
    NFT("IrcSvcHandler::Burst");
    RLOCK((lck_IrcSvcHandler, "i_burst"));
    RET(i_burst);
    ETCB();
}

float IrcSvcHandler::BurstTime() const
{
    BTCB();
    NFT("IrcSvcHandler::BurstTime");
    RLOCK((lck_IrcSvcHandler, "connect_time"));
    RLOCK2((lck_IrcSvcHandler, "i_synctime"));
    float retval = static_cast < float > ((i_synctime - connect_time).asMSeconds() / 1000000.0);

    RET(retval);
    ETCB();
}

mDateTime IrcSvcHandler::SyncTime() const
{
    BTCB();
    NFT("IrcSvcHandler::SyncTime");
    RLOCK((lck_IrcSvcHandler, "i_synctime"));
    RET(i_synctime);
    ETCB();
}

void IrcSvcHandler::EndBurst()
{
    BTCB();
    NFT("IrcSvcHandler::EndBurst");
    WLOCK((lck_IrcSvcHandler, "i_burst"));
    WLOCK2((lck_IrcSvcHandler, "i_synctime"));
    MCB(i_burst);
    CB(1, i_synctime);
    i_burst = false;
    i_synctime = mDateTime::CurrentDateTime();
    CE(1, i_synctime);
    MCE(i_burst);
    ETCB();
}

int IrcSvcHandler::send(const mstring & data)
{
    BTCB();
    FT("IrcSvcHandler::send", (data));
    int recvResult = 0;
    mstring tmp(data);

    tmp.replace("\n", "");
    tmp.replace("\r", "");
    out_traffic += tmp.length() + 2;
#ifdef TEST_MODE
    ACE_OS::fprintf(stdout, "%s\n", tmp.c_str());
    ACE_OS::fflush(stdout);
#else
    recvResult = sock.send(const_cast < char * > ((tmp + "\n\r").c_str()), tmp.length() + 2);
#endif

    CH(D_To, tmp);
    RET(recvResult);
    ETCB();
}

void IrcSvcHandler::enqueue(unsigned long msgid, unsigned long priority)
{
    BTCB();
    FT("IrcSvcHandler::enqueue", (msgid));

    // Make sure we have at LEAST our minimum ...
    while (static_cast < unsigned int > (tm.count_threads()) < Magick::instance().config.Min_Threads())
    {
	if (tm.spawn(IrcSvcHandler::worker, reinterpret_cast < void * > (&Magick::instance()),
			 THR_NEW_LWP | THR_DETACHED) != -1)
	{
	    NLOG(LM_NOTICE, "EVENT/NEW_THREAD");
	}
	else
	    CP(("Could not start new thread (below thread threshold)!"));
    }

    // Only spawn if we are less than our maximum ... and need it :)
    if (static_cast<unsigned int>(message_queue.message_count()) > tm.count_threads() * Magick::instance().config.High_Water_Mark())
    {
	CP(("Queue is full - Starting new thread and increasing watermarks ..."));
	if (static_cast < unsigned int > (tm.count_threads()) >= Magick::instance().config.Max_Threads())
	{
	    NLOG(LM_WARNING, "EVENT/MAX_THREADS");
	}
	else
	{
	    if (tm.spawn(IrcSvcHandler::worker, (void *) &Magick::instance(), THR_NEW_LWP | THR_DETACHED) < 0)
	    {
		NLOG(LM_CRITICAL, "EVENT/NEW_THREAD_FAIL");
	    }
	    else
	    {
		NLOG(LM_NOTICE, "EVENT/NEW_THREAD");
	    }
	}
    }

    char *data = new char[sizeof(unsigned long)];
    memcpy(data, reinterpret_cast<void *>(&msgid), sizeof(unsigned long));
    ACE_Message_Block *block = new ACE_Message_Block(data);
    block->msg_priority(priority);

    message_queue.enqueue_prio(block);

    ETCB();
}

void IrcSvcHandler::enqueue(const mstring & message, unsigned long priority)
{
    BTCB();
    FT("IrcSvcHandler::enqueue", (message, priority));
    CH(D_From, message);

    mstring source, msgtype, params;

    source = IrcParam(message, 1);

    // IF we dont start with ':' or '@', and its not PING or SERVER ...
    if (message[0u] != ':' && message[0u] != '@' && Magick::instance().server.proto.Numeric.Prefix() &&
	!source.IsSameAs("PING", true) && !source.IsSameAs("SERVER", true))
    {
	// AND we have server numerics, AND the length of the message is <= the
	// server numeric maximum length ... assume its a server numeric ...
	if (Magick::instance().server.proto.Numeric.Server() && !source.Contains(".") &&
	    source.length() <= static_cast < size_t > (Magick::instance().server.proto.Numeric.Server()))
	    source.prepend("@");

	// AND we have user numerics, AND the length of the message is <= the
	// user numeric maximum length (combined with server numeric maximum
	// length if necessary) ... assume its a user numeric ...
	else if (Magick::instance().server.proto.Numeric.User() &&
		 (Magick::instance().server.proto.Numeric.Combine() ?
			(source.length() == static_cast < size_t > (Magick::instance().server.proto.Numeric.Server() +
								    Magick::instance().server.proto.Numeric.User())) :
			(source.length() == static_cast < size_t > (Magick::instance().server.proto.Numeric.User()))))
	    source.prepend("!");
    }

    if (message[0u] == ':' || source[0u] == '@' || source[0u] == '!')
    {
	msgtype = IrcParam(message, 2);
	if (message.WordCount(" ") > 2)
	    params = " " + message.ExtractFrom(3, " ");
    }
    else
    {
	source.erase();
	msgtype = IrcParam(message, 1);
	if (message.WordCount(" ") > 1)
	    params = " " + message.ExtractFrom(2, " ");
    }

    // Exception case ...
    // We dont want to end burst until everything else
    // is done ... sometimes this happens prematurely.
    if (msgtype == "303")
	priority = P_Delay;

    try
    {
	mMessage *msg = new mMessage(source, msgtype, params);
	if (msg != NULL && msg->validated())
	{
	    for (int i = 0; immediate_process[i] != NULL; i++)
		if (msg->msgtype().IsSameAs(immediate_process[i], true))
		{
		    map_entry<mMessage> tmp(msg);
		    tmp->setDelete();
		    tmp->call();
		    return;
		}
	    unsigned long msgid = msg->msgid();
	    if (msgid == 0)
		return;

	    {
		MLOCK((lck_IrcSvcHandler, lck_MsgIdMap));
		MsgIdMap[msgid] = msg;
	    }
	    enqueue(msgid, priority);
	}
    }
    catch (E_NickServ_Live & e)
    {
	switch (e.where())
	{
	case E_NickServ_Live::W_Get:
	    switch (e.type())
	    {
	    case E_NickServ_Live::T_Invalid:
	    case E_NickServ_Live::T_Blank:
		if (strlen(e.what()) && Magick::instance().nickserv.IsLiveAll(e.what()))
		{
		    Magick::instance().nickserv.RemLive(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_ChanServ_Live & e)
    {
	switch (e.where())
	{
	case E_ChanServ_Live::W_Get:
	    switch (e.type())
	    {
	    case E_ChanServ_Live::T_Invalid:
	    case E_ChanServ_Live::T_Blank:
		if (strlen(e.what()) && Magick::instance().chanserv.IsLive(e.what()))
		{
		    Magick::instance().chanserv.RemLive(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_Server_List & e)
    {
	switch (e.where())
	{
	case E_Server_List::W_Get:
	    switch (e.type())
	    {
	    case E_Server_List::T_Invalid:
	    case E_Server_List::T_Blank:
		if (strlen(e.what()) && Magick::instance().server.IsList(e.what()))
		{
		    Magick::instance().server.RemList(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (exception & e)
    {
	LOG(LM_CRITICAL, "EXCEPTIONS/UNHANDLED", (e.what()));
    }
    catch (...)
    {
	NLOG(LM_CRITICAL, "EXCEPTIONS/UNKNOWN");
    }
    ETCB();
}

void IrcSvcHandler::enqueue_shutdown()
{
    BTCB();
    NFT("IrcSvcHandler::enqueue_shutdown");

    ACE_Message_Block *block = new ACE_Message_Block(0, ACE_Message_Block::MB_HANGUP);
    block->msg_priority(P_System);
    message_queue.enqueue_prio(block);

    ETCB();
}

void IrcSvcHandler::enqueue_sleep()
{
    BTCB();
    NFT("IrcSvcHandler::enqueue_sleep");

    ACE_Message_Block *block = new ACE_Message_Block(0, ACE_Message_Block::MB_PROTO);
    block->set_flags(ACE_Message_Block::USER_FLAGS + FLAG_SLEEP);
    block->msg_priority(P_System);
    message_queue.enqueue_prio(block);

    ETCB();
}

void IrcSvcHandler::enqueue_test()
{
    BTCB();
    NFT("IrcSvcHandler::enqueue_test");

    ACE_Message_Block *block = new ACE_Message_Block(0, ACE_Message_Block::MB_PROTO);
    block->set_flags(ACE_Message_Block::USER_FLAGS + FLAG_YIELD);
    block->msg_priority(P_System);
    message_queue.enqueue_prio(block);

    ETCB();
}

unsigned long IrcSvcHandler::GetNewMsgid()
{
    BTCB();
    NFT("IrcSvcHandler::GetNewMsgid");
    unsigned long msgid = 0;
    {
	MLOCK((lck_IrcSvcHandler, lck_MsgIdMap));
	msgid = mMessage::LastMsgId++;
	while (msgid == 0 || MsgIdMap.find(msgid) != MsgIdMap.end())
	    msgid = mMessage::LastMsgId++;
    }
    RET(msgid);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
map_entry < mMessage > IrcSvcHandler::GetMessage(unsigned long in) const throw (E_IrcSvcHandler_Message)
#else
map_entry < mMessage > IrcSvcHandler::GetMessage(unsigned long in) const
#endif
{
    BTCB();
    FT("IrcSvcHandler::GetMessage", (in));

    MLOCK((lck_IrcSvcHandler, lck_MsgIdMap));
    IrcSvcHandler::msgidmap_t::const_iterator iter = MsgIdMap.find(in);
    if (iter == MsgIdMap.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_IrcSvcHandler_Message(E_IrcSvcHandler_Message::W_Get, E_IrcSvcHandler_Message::T_NotFound, in));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("IrcSvcHandler", "Message", "Get", "NotFound", in));
	RET(mMessage &, GLOB_mMessage);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_IrcSvcHandler_Message(E_IrcSvcHandler_Message::W_Get, E_IrcSvcHandler_Message::T_Invalid, in));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("IrcSvcHandler", "Message", "Get", "Invalid", in));
	RET(mMessage &, GLOB_mMessage);
#endif
    }
    map_entry<mMessage> entry(iter->second);
    if (!entry->validated())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_IrcSvcHandler_Message(E_IrcSvcHandler_Message::W_Get, E_IrcSvcHandler_Message::T_Blank, in));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("IrcSvcHandler", "Message", "Get", "Blank", in));
	RET(mMessage &, GLOB_mMessage);
#endif
    }

    NRET(map_entry < mMessage >, entry);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void IrcSvcHandler::RemMessage(unsigned long in) throw (E_IrcSvcHandler_Message)
#else
void IrcSvcHandler::RemMessage(unsigned long in)
#endif
{
    BTCB();
    FT("IrcSvcHandler::RemMessage", (in));

    MLOCK((lck_IrcSvcHandler, lck_MsgIdMap));
    IrcSvcHandler::msgidmap_t::iterator iter = MsgIdMap.find(in);
    if (iter == MsgIdMap.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_IrcSvcHandler_Message(E_IrcSvcHandler_Message::W_Rem, E_IrcSvcHandler_Message::T_NotFound, in));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("IrcSvcHandler", "Message", "Rem", "NotFound", in));
	return;
#endif
    }

    if (iter->second != NULL)
    {
	map_entry < mMessage > entry(iter->second);
	entry->setDelete();
    }
    MsgIdMap.erase(iter);
    ETCB();
}


bool IrcSvcHandler::IsMessage(unsigned long in) const
{
    BTCB();
    FT("IrcSvcHandler::IsMessage", (in));
    MLOCK((lck_IrcSvcHandler, lck_MsgIdMap));
    msgidmap_t::const_iterator iter = MsgIdMap.find(in);
    if (iter != MsgIdMap.end() && iter->second != NULL)
    {
	map_entry<mMessage> entry(iter->second);
	if (entry->validated())
	{
	    RET(true);
	}
    }
    RET(false);
    ETCB();
}

void IrcSvcHandler::DumpB() const
{
    BTCB();
    MB(0,
       (traffic.size(), in_traffic, out_traffic, connect_time, last_htm_check, htm_level, htm_gap, htm_threshold, i_burst,
	i_synctime));
    ETCB();
}

void IrcSvcHandler::DumpE() const
{
    BTCB();
    ME(0,
       (traffic.size(), in_traffic, out_traffic, connect_time, last_htm_check, htm_level, htm_gap, htm_threshold, i_burst,
	i_synctime));
    ETCB();
}

int Heartbeat_Handler::handle_timeout(const ACE_Time_Value & tv, const void *arg)
{
    BTCB();
    static_cast < void > (tv);
    static_cast < void > (arg);

    mThread::Attach(tt_MAIN);
    FT("Heartbeat_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));

    if (Magick::instance().Shutdown())
	DRET(0);

    vector < ACE_thread_t > dead;
    threads_t::iterator iter;
    unsigned int i;

    {
	RLOCK(("Heartbeat_Handler", "threads"));
	CP(("Starting HEARTBEAT for %d entries (%s) ...", threads.size(),
	    mDateTime::CurrentDateTime().DateTimeString().c_str()));
	for (iter = threads.begin(); iter != threads.end(); iter++)
	{
	    COM(("Checking %s thread (last checkin %s)", names[iter->second.first],
		 iter->second.second.DateTimeString().c_str()));
	    if (iter->second.second.SecondsSince() > Magick::instance().config.Heartbeat_Time())
	    {
		dead.push_back(iter->first);
	    }
	}
	if (dead.size() > (threads.size() / 2))
	{
	    FLUSH();
	    NANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/THREAD_DEAD_HALF");
	    NLOG(LM_EMERGENCY, "SYS_ERRORS/THREAD_DEAD_HALF");
	}
	else if (dead.size())
	{
	    for (i = 0; i < dead.size(); i++)
	    {
		iter = threads.find(dead[i]);
		if (iter != threads.end())
		{
		    if (iter->second.first != H_Main)
			LOG(LM_ALERT, "SYS_ERRORS/THREAD_DEAD", (names[iter->second.first]));
		    switch (iter->second.first)
		    {
		    case H_Worker:
			{
			    RLOCK2((lck_IrcSvcHandler));
			    ACE_Thread_Manager *thr_mgr = NULL;

			    if (Magick::instance().ircsvchandler != NULL)
				thr_mgr = & Magick::instance().ircsvchandler->tm;
			    else
				thr_mgr = & Magick::instance().thr_mgr();
#if defined(SIGIOT) && (SIGIOT != 0)
			    if (iter->second.third)
			    {
				thr_mgr->kill(dead[i], SIGIOT);
				iter->second.third = false;
			    }
			    else
#endif
			    {
				thr_mgr->cancel(dead[i]);
			    }
			}
			break;
		    case H_Main:
			NANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/THREAD_DEAD_MAIN");
			NLOG(LM_EMERGENCY, "SYS_ERRORS/THREAD_DEAD_MAIN");
			break;
		    case H_IrcServer:
			{
			    RLOCK2((lck_IrcSvcHandler));
			    if (Magick::instance().ircsvchandler != NULL)
			    {
				ACE_Thread_Manager *thr_mgr = Magick::instance().ircsvchandler->thr_mgr();

				if (thr_mgr == NULL)
				    thr_mgr = & Magick::instance().thr_mgr();
#if defined(SIGIOT) && (SIGIOT != 0)
				if (iter->second.third)
				{
				    thr_mgr->kill_task(Magick::instance().ircsvchandler, SIGIOT);
				    iter->second.third = false;
				}
				else
#endif
				{
				    Magick::instance().dh_timer = -1;
				    Magick::instance().Disconnect();
				}
			    }
			}
			break;
		    case H_Events:
			{
			    WLOCK2((lck_Events));
			    if (Magick::instance().events != NULL)
			    {
				ACE_Thread_Manager *thr_mgr = Magick::instance().events->thr_mgr();

				if (thr_mgr == NULL)
				    thr_mgr = & Magick::instance().thr_mgr();
#if defined(SIGIOT) && (SIGIOT != 0)
				if (iter->second.third)
				{
				    thr_mgr->kill_task(Magick::instance().events, SIGIOT);
				    iter->second.third = false;
				}
				else
#endif
				{
				    thr_mgr->cancel_task(Magick::instance().events);
				    {
					WLOCK(("Heartbeat_Handler", "threads"));
					threads.erase(iter);
				    }
				    if (!Magick::instance().events->fini())
					Magick::instance().events->wait();
				    delete Magick::instance().events;

				    Magick::instance().events = NULL;
				}
			    }
			}
			break;
		    case H_DCC:
			{
			    WLOCK2(("DCC"));
			    if (Magick::instance().dcc != NULL)
			    {
				ACE_Thread_Manager *thr_mgr = Magick::instance().dcc->thr_mgr();

				if (thr_mgr == NULL)
				    thr_mgr = & Magick::instance().thr_mgr();
#if defined(SIGIOT) && (SIGIOT != 0)
				if (iter->second.third)
				{
				    thr_mgr->kill_task(Magick::instance().dcc, SIGIOT);
				    iter->second.third = false;
				}
				else
#endif
				{
				    thr_mgr->cancel_task(Magick::instance().dcc);
				    {
					WLOCK(("Heartbeat_Handler", "threads"));
					threads.erase(iter);
				    }
				    if (!Magick::instance().dcc->fini())
					Magick::instance().dcc->wait();
				    delete Magick::instance().dcc;

				    Magick::instance().dcc = NULL;
				}
			    }
			}
			break;
		    default:
			break;
		    }
		}
	    }
	}
    }

    // Ensure we always have events and DCC tasks.
    {
	RLOCK((lck_Events));
	if (Magick::instance().events == NULL)
	{
	    WLOCK((lck_Events));
	    Magick::instance().events = new EventTask(&Magick::instance().thr_mgr());
	    Magick::instance().events->open();
	}
    }

    {
	RLOCK(("DCC"));
	if (Magick::instance().dcc == NULL)
	{
	    WLOCK(("DCC"));
	    Magick::instance().dcc = new DccMap(&Magick::instance().thr_mgr());
	    Magick::instance().dcc->open();
	}
    }

    // Force workers to checkin ...
    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL)
	    for (i = 0; i < threads.size(); i++)
		Magick::instance().ircsvchandler->enqueue_test();
    }

    while (Magick::instance().Pause())
	ACE_OS::sleep(1);

    Magick::instance().reactor().schedule_timer(this, 0, ACE_Time_Value(Magick::instance().config.Heartbeat_Time()));
    DRET(0);
    ETCB();
}

void Heartbeat_Handler::AddThread(heartbeat_enum type, ACE_thread_t id)
{
    BTCB();
    FT("Heartbeat_Handler::AddThread", (static_cast < int > (type), id));
    WLOCK(("Heartbeat_Handler", "threads"));
    threads[id] = triplet < heartbeat_enum, mDateTime, bool > (type, mDateTime::CurrentDateTime(), true);

    ETCB();
}

void Heartbeat_Handler::RemoveThread(ACE_thread_t id)
{
    BTCB();
    FT("Heartbeat_Handler::RemoveThread", (id));
    WLOCK(("Heartbeat_Handler", "threads"));
    threads_t::iterator iter = threads.find(id);
    if (iter != threads.end())
	threads.erase(iter);
    ETCB();
}

void Heartbeat_Handler::Heartbeat(ACE_thread_t id)
{
    BTCB();
    FT("Heartbeat_Handler::Heartbeat", (id));
    WLOCK(("Heartbeat_Handler", "threads"));
    threads_t::iterator iter = threads.find(id);
    if (iter != threads.end())
	iter->second.second = mDateTime::CurrentDateTime();
    ETCB();
}

Heartbeat_Handler::heartbeat_enum Heartbeat_Handler::ThreadType(ACE_thread_t id)
{
    BTCB();
    FT("Heartbeat_Handler::Heartbeat", (id));

    heartbeat_enum retval = H_Invalid;

    {
	RLOCK(("Heartbeat_Handler", "threads"));
	threads_t::iterator iter = threads.find(id);
	if (iter != threads.end())
	    retval = iter->second.first;
    }
    RET(retval);
    ETCB();
}

size_t Heartbeat_Handler::size()
{
    BTCB();
    NFT("Heartbeat_Handler::size");
    size_t retval = 0;

    {
	RLOCK(("Heartbeat_Handler", "threads"));
	retval = threads.size();
    }
    RET(retval);
    ETCB();
}

class CountThreadType
{
    Heartbeat_Handler::heartbeat_enum type;
public:
    CountThreadType(Heartbeat_Handler::heartbeat_enum t) : type(t) {}
    template<typename T> bool operator()(const T &in) const
    {
	return (in.second.first == type);
    }
};

size_t Heartbeat_Handler::count(heartbeat_enum type)
{
    BTCB();
    FT("Heartbeat_Handler::count", (static_cast < int > (type)));
    size_t retval = 0;

    {
	RLOCK(("Heartbeat_Handler", "threads"));
	retval = count_if(threads.begin(), threads.end(), CountThreadType(type));
    }

    RET(retval);
    ETCB();
}

int Disconnect_Handler::handle_timeout(const ACE_Time_Value & tv, const void *arg)
{
    BTCB();
    static_cast < void > (tv);
    static_cast < void > (arg);

    mThread::Attach(tt_MAIN);
    FT("Disconnect_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));

    Magick::instance().dh_timer = 0;
    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL)
	{
	    if (Magick::instance().hh.ThreadType() != Heartbeat_Handler::H_IrcServer)
	    {
		ACE_Thread_Manager *thr_mgr = Magick::instance().ircsvchandler->thr_mgr();

		if (thr_mgr == NULL)
		    thr_mgr = & Magick::instance().thr_mgr();
		ACE_thread_t id;

		thr_mgr->thread_list(Magick::instance().ircsvchandler, &id, 1);
		thr_mgr->cancel_task(Magick::instance().ircsvchandler);
		Magick::instance().hh.RemoveThread(id);
	    }
	    if (!Magick::instance().ircsvchandler->fini())
		Magick::instance().ircsvchandler->close(0);
	    WLOCK((lck_IrcSvcHandler));
	    delete Magick::instance().ircsvchandler;

	    Magick::instance().ircsvchandler = NULL;
	}
    }

    DRET(0);
    ETCB();
}

Connection_t Reconnect_Handler::FindNext(const mstring &server, unsigned short port)
{
    BTCB();
    FT("Reconnect_Handler::FindNext", (server, port));

    // This will be 0 if the server is not found/
    Connection_t svr = Magick::instance().startup.Server(server, port);

    // If it was found, get the next one of the same priority ...
    if (svr.Priority() != 0)
    {
	vector<Connection_t> conn = Magick::instance().startup.PriorityList(svr.Priority());
	vector<Connection_t>::iterator iter;
	for (iter=conn.begin(); iter!=conn.end(); iter++)
	    if (iter->Name().IsSameAs(server, true) && iter->Port() == port)
	    {
		if (++iter != conn.end())
		{
		    NRET(Connection_t, *iter);
		}
		break;
	    }
    }

    // If we couldnt find the 'next' one (ie. the server passed wasnt found, or we were at the end of
    // that priority).  Get the first connection from the next priority.
    vector<Connection_t> conn = Magick::instance().startup.PriorityList(svr.Priority() + 1);
    if (conn.size())
	NRET(Connection_t, *conn.begin());

    // Should not get here.
    Connection_t tmp;
    NRET(Connection_t, tmp);
    ETCB();
}

int Reconnect_Handler::handle_timeout(const ACE_Time_Value & tv, const void *arg)
{
    BTCB();
    static_cast < void > (tv);
    static_cast < void > (arg);

    mThread::Attach(tt_MAIN);
    FT("Reconnect_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));

    if (Magick::instance().config.Server_Relink() < 1 || !Magick::instance().Reconnect() || Magick::instance().Shutdown())
	DRET(0);

    Connection_t server;

    if (Magick::instance().startup.Server_size())
    {
	if (Magick::instance().GotConnect())
	{
	    server = FindNext("", 0);
	}
	else
	{
	    server = FindNext(Magick::instance().CurrentServer().first, Magick::instance().CurrentServer().second);
	}
    }
    if (server.Name().empty() || server.Port() == 0)
    {
	NLOG(LM_EMERGENCY, "OTHER/NOVALIDSERVERS");
    }

    ACE_INET_Addr addr;
    if (addr.set(server.Port(), server.Name()) < 0)
    {
	LOG(LM_EMERGENCY, "SYS_ERRORS/IPADDR_SERVER", (server.Name(), server.Port()));
    }

    Magick::instance().DumpB();
    Magick::instance().GotConnect(false);
    Magick::instance().CurrentServer(server.Name(), server.Port());
    Magick::instance().server.proto.Tokens(false);

    if (Magick::instance().Connected())
	Magick::instance().Disconnect();

    LOG(LM_INFO, "OTHER/CONNECTING", (server.Name(), server.Port()));

#ifdef TEST_MODE
    {
	WLOCK((lck_IrcSvcHandler));
	Magick::instance().ircsvchandler = new IrcSvcHandler();
	Magick::instance().ircsvchandler->open(NULL);
    }
#else
    IrcConnector C_server(&Magick::instance().reactor(), ACE_NONBLOCK);

    ACE_INET_Addr laddr;
    unsigned short port = mSocket::FindAvailPort();

    if (Magick::instance().startup.Bind().length())
    {
	if (laddr.set(port, Magick::instance().startup.Bind()) < 0)
	{
	    LOG(LM_EMERGENCY, "SYS_ERRORS/IPADDR_BIND", (Magick::instance().startup.Bind(), port));
	}
    }
    else
	laddr.set(port);

    int res = 0;

    {
	WLOCK((lck_IrcSvcHandler));
	res = C_server.connect(Magick::instance().ircsvchandler, addr, ACE_Synch_Options::defaults, laddr);
    }
    if (res == -1)
    {
	LOG(LM_ERROR, "OTHER/REFUSED", (server.Name(), server.Port()));
	//okay we got a connection problem here. log it and try again
	CP(("Refused connection, rescheduling and trying again ..."));

	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);
	Magick::instance().reactor().schedule_timer(&(Magick::instance().rh), 0,
						    ACE_Time_Value(Magick::instance().config.Server_Relink()));
    }
    else
    {
	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL)
	    {
		CB(2, Magick::instance().i_localhost);
		Magick::instance().i_localhost = Magick::instance().ircsvchandler->Local_IP();
		CE(2, Magick::instance().i_localhost);
	    }
	}
#endif /* TEST_MODE */

	if (!Magick::instance().server.proto.Protoctl().empty())
	    Magick::instance().server.raw(Magick::instance().server.proto.Protoctl());

	mstring tmp = "PASS " + server.Password();

	if (Magick::instance().server.proto.TSora())
	{
	    tmp += " :TS";
	    if (Magick::instance().server.proto.TSora() >= 7)
		tmp << Magick::instance().server.proto.TSora();
	}
	Magick::instance().server.raw(tmp);

	// 5 args - server name, hops, server desc and numeric (optional) and timestamp.

	Magick::instance().server.raw(parseMessage(Magick::instance().server.proto.Server(),
			mVarArray(Magick::instance().startup.Server_Name(), 1, Magick::instance().startup.Server_Desc(),
				  Magick::instance().server.proto.Numeric.ServerLineNumeric(server.Numeric()),
				  time(NULL), Magick::instance().StartTime().timetstring())));

	if (Magick::instance().server.proto.TSora())
	    // SVINFO <TS_CURRENT> <TS_MIN> <STANDALONE> :<UTC-TIME>
	    Magick::instance().server.raw("SVINFO " + mstring(Magick::instance().server.proto.TSora()) + " 1 0 :" + mDateTime::CurrentDateTime().timetstring());
	Magick::instance().Connected(true);
	LOG(LM_INFO, "OTHER/CONNECTED", (server.Name(), server.Port()));
#ifndef TEST_MODE
    }
#endif

    Magick::instance().DumpE();
    DRET(0);
    ETCB();
}

int ToBeSquit_Handler::handle_timeout(const ACE_Time_Value & tv, const void *arg)
{
    BTCB();
    mThread::Attach(tt_MAIN);
    // We ONLY get here if we didnt receive a SQUIT message in <10s
    // after any QUIT message with 2 valid servers in it
    FT("ToBeSquit_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    static_cast < void > (tv);
    mstring *tmp = reinterpret_cast < mstring * > (const_cast < void * > (arg));

    {
	WLOCK((lck_Server, "ServerSquit"));
	Magick::instance().server.DumpB();
	CB(1, Magick::instance().server.ServerSquit.size());
	Magick::instance().server.ServerSquit.erase(*tmp);
	CE(1, Magick::instance().server.ServerSquit.size());
    }

    if (Magick::instance().server.IsList(*tmp))
    {
	LOG(LM_NOTICE, "OTHER/SQUIT_CANCEL", (tmp, Magick::instance().server.GetList(*tmp)->Uplink()));
    }
    else
    {
	LOG(LM_NOTICE, "OTHER/SQUIT_CANCEL", (tmp, "?"));
    }

    // QUIT all user's who faked it ...
    set < mstring > chunked;
    {
	WLOCK2((lck_Server, "ToBeSquit"));
	map<mstring, set<mstring> >::iterator iter = Magick::instance().server.ToBeSquit.find(*tmp);
	if (iter != Magick::instance().server.ToBeSquit.end())
	{
	    CB(2, Magick::instance().server.ToBeSquit.size());
	    chunked = iter->second;
	    Magick::instance().server.ToBeSquit.erase(iter);
	    CE(2, Magick::instance().server.ToBeSquit.size());
	}
    }

    set < mstring >::iterator k;
    for (k = chunked.begin(); k != chunked.end(); k++)
    {
	try
	{
	    if (Magick::instance().nickserv.IsLiveAll(*k))
	    {
		map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(*k);
		nlive->Quit("FAKE SQUIT - " + *tmp);
		Magick::instance().nickserv.RemLive(*k);
		mMessage::CheckDependancies(mMessage::NickNoExists, *k);
		if (nlive->Numeric())
		    mMessage::CheckDependancies(mMessage::NickNoExists, "!" +
						Magick::instance().server.proto.Numeric.UserNumeric(nlive->Numeric()));
	    }
	}
	catch (E_NickServ_Live & e)
	{
	    switch (e.where())
	    {
	    case E_NickServ_Live::W_Get:
		switch (e.type())
		{
		case E_NickServ_Live::T_Invalid:
		case E_NickServ_Live::T_Blank:
		    if (strlen(e.what()) && Magick::instance().nickserv.IsLiveAll(e.what()))
		    {
			Magick::instance().nickserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (exception & e)
	{
	    LOG(LM_CRITICAL, "EXCEPTIONS/UNHANDLED", (e.what()));
	}
	catch (...)
	{
	    NLOG(LM_CRITICAL, "EXCEPTIONS/UNKNOWN");
	}
    }
    Magick::instance().server.DumpE();

    delete tmp;

    DRET(0);
    ETCB();
}

int Squit_Handler::handle_timeout(const ACE_Time_Value & tv, const void *arg)
{
    BTCB();
    mThread::Attach(tt_MAIN);
    // OK -- we get here after we've passwd Squit_Protect()
    // seconds after a REAL squit
    FT("Squit_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    static_cast < void > (tv);
    mstring *tmp = reinterpret_cast < mstring * > (const_cast < void * > (arg));

    {
	WLOCK((lck_Server, "ServerSquit"));
	WLOCK2((lck_Server, "ToBeSquit"));
	Magick::instance().server.DumpB();
	CB(1, Magick::instance().server.ServerSquit.size());
	CB(2, Magick::instance().server.ToBeSquit.size());
	Magick::instance().server.ServerSquit.erase(*tmp);
	Magick::instance().server.ToBeSquit.erase(*tmp);
	CE(1, Magick::instance().server.ServerSquit.size());
	CE(2, Magick::instance().server.ToBeSquit.size());
	Magick::instance().server.DumpE();
    }

    // QUIT all user's who did not come back from SQUIT
    vector < mstring > SquitMe;
    NickServ::live_t::iterator i;
    {
	RLOCK((lck_NickServ, lck_live));
	for (i = Magick::instance().nickserv.LiveBegin(); i != Magick::instance().nickserv.LiveEnd(); i++)
	{
	    map_entry < Nick_Live_t > nlive(i->second);
	    if (nlive->Squit() == *tmp)
		SquitMe.push_back(nlive->Name());
	}
    }

    vector < mstring >::iterator k;
    for (k = SquitMe.begin(); k != SquitMe.end(); k++)
    {
	try
	{
	    if (Magick::instance().nickserv.IsLiveAll(*k))
	    {
		map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(*k);
		nlive->Quit("SQUIT - " + *tmp);
		Magick::instance().nickserv.RemLive(*k);
		mMessage::CheckDependancies(mMessage::NickNoExists, *k);
		if (nlive->Numeric())
		    mMessage::CheckDependancies(mMessage::NickNoExists,
						"!" + Magick::instance().server.proto.Numeric.UserNumeric(nlive->Numeric()));
	    }
	}
	catch (E_NickServ_Live & e)
	{
	    switch (e.where())
	    {
	    case E_NickServ_Live::W_Get:
		switch (e.type())
		{
		case E_NickServ_Live::T_Invalid:
		case E_NickServ_Live::T_Blank:
		    if (strlen(e.what()) && Magick::instance().nickserv.IsLiveAll(e.what()))
		    {
			Magick::instance().nickserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (exception & e)
	{
	    LOG(LM_CRITICAL, "EXCEPTIONS/UNHANDLED", (e.what()));
	}
	catch (...)
	{
	    NLOG(LM_CRITICAL, "EXCEPTIONS/UNKNOWN");
	}
    }

    delete tmp;

    DRET(0);
    ETCB();
}

int InFlight_Handler::handle_timeout(const ACE_Time_Value & tv, const void *arg)
{
    BTCB();
    mThread::Attach(tt_MAIN);
    // Memo timed out, send it!
    // If its a file, and not inprogress, ignore.
    FT("InFlight_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    static_cast < void > (tv);
    mstring *tmp = reinterpret_cast < mstring * > (const_cast < void * > (arg));

    if (Magick::instance().nickserv.IsLiveAll(*tmp))
    {
	RLOCK((lck_NickServ, lck_live, tmp->LowerCase()));
	map_entry < Nick_Live_t > entry = Magick::instance().nickserv.GetLive(*tmp);
	if (entry->InFlight.File())
	{
	    if (!entry->InFlight.InProg())
	    {
		/* Already handled by DccXfer::DccXfer
		 * SEND(entry->InFlight.service, "DCC/NOCONNECT", ( "GET"));
		 */
		entry->InFlight.Cancel();
	    }
	}
	else
	{
	    entry->InFlight.End(0u);
	}
    }
    delete tmp;

    DRET(0);
    ETCB();
}

int Part_Handler::handle_timeout(const ACE_Time_Value & tv, const void *arg)
{
    BTCB();
    mThread::Attach(tt_MAIN);
    FT("Part_Handler::handle_timeout", ("(const ACE_Time_Value &) tv", "(const void *) arg"));
    static_cast < void > (tv);
    mstring *tmp = reinterpret_cast < mstring * > (const_cast < void * > (arg));

    // This is to part channels I'm not supposed to be
    // in (ie. dont have JOIN on, and I'm the only user
    // in them).  ie. after AKICK, etc.
    try
    {
	if (Magick::instance().chanserv.IsLive(*tmp) &&
	    Magick::instance().chanserv.GetLive(*tmp)->IsIn(Magick::instance().chanserv.FirstName()))
	{
	    Magick::instance().server.PART(Magick::instance().chanserv.FirstName(), *tmp);
	    if (Magick::instance().chanserv.IsLive(*tmp))
	    {
		map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(*tmp);
		clive->DumpB();
		{
		    MLOCK((lck_ChanServ, lck_live, tmp->LowerCase(), "ph_timer"));
		    CB(1, clive->ph_timer);
		    clive->ph_timer = 0;
		    CE(1, clive->ph_timer);
		}
		clive->DumpE();
	    }
	}
    }
    catch (E_NickServ_Live & e)
    {
	switch (e.where())
	{
	case E_NickServ_Live::W_Get:
	    switch (e.type())
	    {
	    case E_NickServ_Live::T_Invalid:
	    case E_NickServ_Live::T_Blank:
		if (strlen(e.what()) && Magick::instance().nickserv.IsLiveAll(e.what()))
		{
		    Magick::instance().nickserv.RemLive(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_ChanServ_Stored & e)
    {
	switch (e.where())
	{
	case E_ChanServ_Stored::W_Get:
	    switch (e.type())
	    {
	    case E_ChanServ_Stored::T_Invalid:
	    case E_ChanServ_Stored::T_Blank:
		if (strlen(e.what()) && Magick::instance().chanserv.IsStored(e.what()))
		{
		    Magick::instance().chanserv.RemStored(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (E_ChanServ_Live & e)
    {
	switch (e.where())
	{
	case E_ChanServ_Live::W_Get:
	    switch (e.type())
	    {
	    case E_ChanServ_Live::T_Invalid:
	    case E_ChanServ_Live::T_Blank:
		if (strlen(e.what()) && Magick::instance().chanserv.IsLive(e.what()))
		{
		    Magick::instance().chanserv.RemLive(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }
    catch (exception & e)
    {
	LOG(LM_CRITICAL, "EXCEPTIONS/UNHANDLED", (e.what()));
    }
    catch (...)
    {
	NLOG(LM_CRITICAL, "EXCEPTIONS/UNKNOWN");
    }
    delete tmp;

    DRET(0);
    ETCB();
}

ACE_THR_FUNC_RETURN EventTask::save_databases(void *in)
{
    BTCB();
    Magick::register_instance(reinterpret_cast < Magick * > (in));
    mThread::Attach(tt_MAIN);
    FT("EventTask::save_databases", (in));
    Magick::instance().save_databases();
    Magick::deregister_instance();
    
    DTRET(0);
    ETCB();
}

void EventTask::AddChannelModePending(const mstring & in)
{
    BTCB();
    FT("EventTask::AddChannelModePending", (in));
    WLOCK((lck_Events, "cmodes_pending"));
    MCB(cmodes_pending.size());
    cmodes_pending.insert(in);
    MCE(cmodes_pending.size());
    ETCB();
}

void EventTask::ForceSave()
{
    BTCB();
    NFT("EventTask::ForceSave");
    WLOCK((lck_Events, "last_save"));
    MCB(last_save);
    last_save = mDateTime(0.0);
    MCE(last_save);
    ETCB();
}

void EventTask::ForcePing()
{
    BTCB();
    NFT("EventTask::ForcePing");
    WLOCK((lck_Events, "last_ping"));
    MCB(last_ping);
    last_ping = mDateTime(0.0);
    MCE(last_ping);
    ETCB();
}

mstring EventTask::SyncTime(const mstring & source) const
{
    BTCB();
    FT("EventTask::SyncTime", (source));
    RLOCK((lck_Events, "last_save"));
    mstring retval = ToHumanTime(Magick::instance().config.Savetime() - last_save.SecondsSince(), source);

    RET(retval);
    ETCB();
}

int EventTask::open(void *in)
{
    BTCB();
    FT("EventTask::open", ("(void *) in"));
    magick_instance = reinterpret_cast < Magick * > (in);
    int retval = activate();

    RET(retval);
    ETCB();
}

int EventTask::close(u_long in)
{
    BTCB();
    static_cast < void > (in);

    FT("EventTask::close", (in));
    // dump all and close open file handles.
    RET(0);
    ETCB();
}

int EventTask::svc(void)
{
    BTCB();
    Magick::register_instance(magick_instance);
    mThread::Attach(tt_MAIN);
    // The biggie, so big, it has its own zip code ... uhh .. thread.
    NFT("EventTask::svc");
    Magick::instance().hh.AddThread(Heartbeat_Handler::H_Events);

    bool proc;

    {
	WLOCK((lck_Events, "last_expire"));
	WLOCK2((lck_Events, "last_save"));
	WLOCK3((lck_Events, "last_check"));
	WLOCK4((lck_Events, "last_ping"));
	WLOCK5((lck_Events, "last_msgcheck"));
	last_expire = last_save = last_check = last_ping = last_msgcheck = mDateTime::CurrentDateTime();
    }
    DumpB();

    mDateTime synctime;

    while (!Magick::instance().Shutdown())
    {
	Magick::instance().hh.Heartbeat();

	proc = true;
	// Make sure we're turned on ...
	if (!Magick::instance().AUTO() || Magick::instance().Pause())
	    proc = false;
	else
	{
	    // Make sure we're sync'd to network ...
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler == NULL || Magick::instance().ircsvchandler->Burst())
		proc = false;
	    else
		synctime = Magick::instance().ircsvchandler->SyncTime();
	}

	if (!proc)
	{
	    ACE_OS::sleep(1);
	    continue;
	}

	try
	{
	    // This is mainly used for 'only do this if users have had
	    // enough time to rectify the situation since sync' ...
	    if_RLOCK ((lck_Events, "last_expire"), last_expire.SecondsSince() >= Magick::instance().config.Cycletime())
	    {
		do_expire(synctime);
		WLOCK((lck_Events, "last_expire"));
		MCB(last_expire);
		last_expire = mDateTime::CurrentDateTime();
		MCE(last_expire);
	    }

	    if (Magick::instance().Saving())
	    {
		WLOCK((lck_Events, "last_save"));
		MCB(last_save);
		last_save = mDateTime::CurrentDateTime();
		MCE(last_save);
	    }
	    if_RLOCK2 ((lck_Events, "last_save"), last_save.SecondsSince() >= Magick::instance().config.Savetime())
	    {
		CP(("Starting DATABASE SAVE ..."));

		// A hack, since freebsd goes apeshit if we spawn a thread to do this ...
#ifdef __FreeBSD__
		Magick::instance().save_databases();
#else
		ACE_Thread_Manager *tm = thr_mgr();

		if (tm == NULL)
		    tm = & Magick::instance().thr_mgr();
		if (tm->spawn(save_databases, reinterpret_cast < void * > (&Magick::instance()), THR_NEW_LWP | THR_DETACHED) <
		    0)
		{
		    NLOG(LM_CRITICAL, "EVENT/NEW_THREAD_FAIL");
		}
		else
#endif
		{
		    WLOCK((lck_Events, "last_save"));
		    MCB(last_save);
		    last_save = mDateTime::CurrentDateTime();
		    MCE(last_save);
		}
	    }

	    if_RLOCK2 ((lck_Events, "last_check"), last_check.SecondsSince() >= Magick::instance().config.Checktime())
	    {
		do_check(synctime);
		WLOCK((lck_Events, "last_check"));
		MCB(last_check);
		last_check = mDateTime::CurrentDateTime();
		MCE(last_check);
	    }

	    if (Magick::instance().nickserv.IsLive(Magick::instance().chanserv.FirstName()))
	    {
		do_modes(synctime);
	    }

	    if_RLOCK2 ((lck_Events, "last_msgcheck"),
		       last_msgcheck.SecondsSince() > Magick::instance().config.MSG_Check_Time())
	    {
		do_msgcheck(synctime);
		WLOCK((lck_Events, "last_msgcheck"));
		MCB(last_msgcheck);
		last_msgcheck = mDateTime::CurrentDateTime();
		MCE(last_msgcheck);
	    }

	    if_RLOCK2 ((lck_Events, "last_ping"), last_ping.SecondsSince() >= Magick::instance().config.Ping_Frequency())
	    {
		do_ping(synctime);
		WLOCK((lck_Events, "last_ping"));
		MCB(last_ping);
		last_ping = mDateTime::CurrentDateTime();
		MCE(last_ping);
	    }
	}
	catch (E_NickServ_Stored & e)
	{
	    switch (e.where())
	    {
	    case E_NickServ_Stored::W_Get:
		switch (e.type())
		{
		case E_NickServ_Stored::T_Invalid:
		case E_NickServ_Stored::T_Blank:
		    if (strlen(e.what()) && Magick::instance().nickserv.IsStored(e.what()))
		    {
			Magick::instance().nickserv.RemStored(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_NickServ_Live & e)
	{
	    switch (e.where())
	    {
	    case E_NickServ_Live::W_Get:
		switch (e.type())
		{
		case E_NickServ_Live::T_Invalid:
		case E_NickServ_Live::T_Blank:
		    if (strlen(e.what()) && Magick::instance().nickserv.IsLiveAll(e.what()))
		    {
			Magick::instance().nickserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_NickServ_Recovered & e)
	{
	    switch (e.where())
	    {
	    case E_NickServ_Recovered::W_Get:
		switch (e.type())
		{
		case E_NickServ_Recovered::T_Invalid:
		case E_NickServ_Recovered::T_Blank:
		    if (strlen(e.what()) && Magick::instance().nickserv.IsRecovered(e.what()))
		    {
			Magick::instance().nickserv.RemRecovered(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_ChanServ_Stored & e)
	{
	    switch (e.where())
	    {
	    case E_ChanServ_Stored::W_Get:
		switch (e.type())
		{
		case E_ChanServ_Stored::T_Invalid:
		case E_ChanServ_Stored::T_Blank:
		    if (strlen(e.what()) && Magick::instance().chanserv.IsStored(e.what()))
		    {
			Magick::instance().chanserv.RemStored(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_ChanServ_Live & e)
	{
	    switch (e.where())
	    {
	    case E_ChanServ_Live::W_Get:
		switch (e.type())
		{
		case E_ChanServ_Live::T_Invalid:
		case E_ChanServ_Live::T_Blank:
		    if (strlen(e.what()) && Magick::instance().chanserv.IsLive(e.what()))
		    {
			Magick::instance().chanserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_CommServ_List & e)
	{
	    switch (e.where())
	    {
	    case E_CommServ_List::W_Get:
		switch (e.type())
		{
		case E_CommServ_List::T_Invalid:
		case E_CommServ_List::T_Blank:
		    if (strlen(e.what()) && Magick::instance().commserv.IsList(e.what()))
		    {
			Magick::instance().commserv.RemList(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_Server_List & e)
	{
	    switch (e.where())
	    {
	    case E_Server_List::W_Get:
		switch (e.type())
		{
		case E_Server_List::T_Invalid:
		case E_Server_List::T_Blank:
		    if (strlen(e.what()) && Magick::instance().server.IsList(e.what()))
		    {
			Magick::instance().server.RemList(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (E_MemoServ_Nick & e)
	{
	    e.what();
	}
	catch (E_MemoServ_Channel & e)
	{
	    e.what();
	}
	catch (E_DccMap_Xfers & e)
	{
	    switch (e.where())
	    {
	    case E_DccMap_Xfers::W_Get:
		switch (e.type())
		{
		case E_DccMap_Xfers::T_Invalid:
		case E_DccMap_Xfers::T_Blank:
		    if (strlen(e.what()) && DccMap::IsXfers(atoi(e.what())))
		    {
			DccMap::RemXfers(atoi(e.what()));
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	}
	catch (exception & e)
	{
	    LOG(LM_CRITICAL, "EXCEPTIONS/UNHANDLED", (e.what()));
	}
	catch (...)
	{
	    NLOG(LM_CRITICAL, "EXCEPTIONS/UNKNOWN");
	}

	COM(("Completed Events Cycle"));
	FLUSH();		// Force TRACE output dump
	ACE_OS::sleep(1);
    }
    Magick::instance().hh.RemoveThread();
    Magick::deregister_instance();
    DRET(0);
    ETCB();
}

void EventTask::do_expire(mDateTime & synctime)
{
    BTCB();
    CP(("Starting EXPIRATION check ..."));

    static_cast < void > (synctime);

    NickServ::stored_t::iterator nsi;
    ChanServ::stored_t::iterator csi;
    MemoServ::channel_t::iterator ni;
    MemoServ::channel_news_t::iterator lni;
    unsigned int i;

    // akills
    //try
    {
	MLOCK((lck_OperServ, "Akill"));
	for (Magick::instance().operserv.Akill = Magick::instance().operserv.Akill_begin();
	     Magick::instance().operserv.Akill != Magick::instance().operserv.Akill_end(); )
	{
	    if (Magick::instance().operserv.Akill->Last_Modify_Time().SecondsSince() >
		Magick::instance().operserv.Akill->Value().first)
	    {
		Magick::instance().server.RAKILL(Magick::instance().operserv.Akill->Entry());
		LOG(LM_INFO, "EVENT/EXPIRE_AKILL",
		    (Magick::instance().operserv.Akill->Entry(), Magick::instance().operserv.Akill->Value().second,
		     Magick::instance().operserv.Akill->Last_Modifier(),
		     ToHumanTime(Magick::instance().operserv.Akill->Value().first)));
		ANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/EXPIRE_AKILL",
			 (Magick::instance().operserv.Akill->Entry(), Magick::instance().operserv.Akill->Value().second,
			  Magick::instance().operserv.Akill->Last_Modifier(),
			  ToHumanTime(Magick::instance().operserv.Akill->Value().first)));
		// This will advance the iterator ...
		Magick::instance().operserv.Akill_erase();
	    }
	    else
	    {
		Magick::instance().operserv.Akill++;
	    }
	}
    }
    //catch(...)
    //{
    //}

    // nicknames
    try
    {
	vector < pair < mstring, mstring > > expired_nicks;
	{
	    RLOCK2((lck_NickServ, lck_stored));
	    for (nsi = Magick::instance().nickserv.StoredBegin(); nsi != Magick::instance().nickserv.StoredEnd(); nsi++)
	    {
		map_entry < Nick_Stored_t > nstored(nsi->second);
		if (!(nstored->NoExpire() || nstored->Forbidden() || nstored->Suspended()))
		{
		    if (nstored->Host().empty())
		    {
			if (nstored->LastAllSeenTime().SecondsSince() > Magick::instance().nickserv.Expire())
			{
			    expired_nicks.push_back(pair < mstring, mstring > (nstored->Name(), nstored->Name()));
			}
		    }
		    else
		    {
			if (nstored->LastSeenTime().SecondsSince() > Magick::instance().nickserv.Expire())
			{
			    expired_nicks.push_back(pair < mstring, mstring > (nstored->Name(), nstored->Host()));
			}
		    }
		}
	    }
	}
	for (i = 0; i < expired_nicks.size(); i++)
	{
	    if (Magick::instance().nickserv.IsStored(expired_nicks[i].first))
	    {
		Magick::instance().nickserv.GetStored(expired_nicks[i].first)->Drop();
		Magick::instance().nickserv.RemStored(expired_nicks[i].first);
		LOG(LM_INFO, "EVENT/EXPIRE_NICK", (expired_nicks[i].first, expired_nicks[i].second));
	    }
	}
    }
    catch (E_NickServ_Stored & e)
    {
	switch (e.where())
	{
	case E_NickServ_Stored::W_Get:
	    switch (e.type())
	    {
	    case E_NickServ_Stored::T_Invalid:
	    case E_NickServ_Stored::T_Blank:
		if (strlen(e.what()))
		{
		    Magick::instance().nickserv.RemStored(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }

    // channels
    try
    {
	vector < pair < mstring, mstring > > expired_chans;
	{
	    RLOCK2((lck_ChanServ, lck_stored));
	    for (csi = Magick::instance().chanserv.StoredBegin(); csi != Magick::instance().chanserv.StoredEnd(); csi++)
	    {
		map_entry < Chan_Stored_t > cstored(csi->second);
		if (!(cstored->NoExpire() || cstored->Forbidden() || cstored->Suspended()))
		{
		    if (cstored->LastUsed().SecondsSince() > Magick::instance().chanserv.Expire())
			expired_chans.push_back(pair < mstring, mstring > (cstored->Name(), cstored->Founder()));
		}
	    }
	}
	for (i = 0; i < expired_chans.size(); i++)
	{
	    if (Magick::instance().chanserv.IsStored(expired_chans[i].first))
	    {
		Magick::instance().chanserv.RemStored(expired_chans[i].first);
		LOG(LM_INFO, "EVENT/EXPIRE_CHAN", (expired_chans[i].first, expired_chans[i].second));
	    }
	}
    }
    catch (E_ChanServ_Stored & e)
    {
	switch (e.where())
	{
	case E_ChanServ_Stored::W_Get:
	    switch (e.type())
	    {
	    case E_ChanServ_Stored::T_Invalid:
	    case E_ChanServ_Stored::T_Blank:
		if (strlen(e.what()))
		{
		    Magick::instance().chanserv.RemStored(e.what());
		}
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }

    // news articles
    try
    {
	map < mstring, vector < size_t > > expired_news;
	map < mstring, vector < size_t > >::iterator iter;
	{
	    RLOCK2((lck_MemoServ, lck_channel));
	    for (ni = Magick::instance().memoserv.ChannelBegin(); ni != Magick::instance().memoserv.ChannelEnd(); ni++)
	    {
		size_t cnt = 0;

		RLOCK3((lck_MemoServ, lck_channel, ni->first));
		for (lni = ni->second.begin(), i = 0; lni != ni->second.end(); lni++, i++)
		{
		    if (!lni->NoExpire() && lni->Time().SecondsSince() > Magick::instance().memoserv.News_Expire())
		    {
			LOG(LM_DEBUG, "EVENT/EXPIRE_NEWS", (lni->Channel()));
			expired_news[ni->first].push_back(i);
			cnt++;
		    }
		}
		// If we see an entry in the map, but its empty, it will mean
		// ALL news articles are gone ... an efficiancy thing.
		if (cnt == ni->second.size())
		    expired_news[ni->first].clear();
	    }
	}
	for (iter = expired_news.begin(); iter != expired_news.end(); iter++)
	{
	    if (iter->second.size())
	    {
		size_t adjust = 0;

		for (i = 0; i < iter->second.size(); i++)
		{
		    Magick::instance().memoserv.RemChannelNews(iter->first, iter->second[i] - adjust);
		    adjust++;
		}
	    }
	    else
	    {
		Magick::instance().memoserv.RemChannel(iter->first);
	    }
	}
    }
    catch (E_MemoServ_Channel & e)
    {
	e.what();
    }
    ETCB();
}

void EventTask::do_check(mDateTime & synctime)
{
    BTCB();
    CP(("Starting CHECK cycle ..."));

    NickServ::live_t::iterator nli;
    NickServ::recovered_t::iterator di;
    ChanServ::live_t::iterator cli;
    unsigned int i;


    if (Magick::instance().nickserv.IsLive(Magick::instance().chanserv.FirstName()))
    {
	RLOCK((lck_ChanServ, lck_live));
	for (cli = Magick::instance().chanserv.LiveBegin(); cli != Magick::instance().chanserv.LiveEnd(); cli++)
	{
	    map_entry < Chan_Live_t > clive(cli->second);
	    bool found = false;
	    unsigned long bantime = 0, parttime = 0;

	    if (Magick::instance().chanserv.IsStored(clive->Name()))
	    {
		map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(clive->Name());
		bantime = cstored->Bantime();
		parttime = cstored->Parttime();
		if (cstored->LimitBump() && clive->Last_Limit_Change().SecondsSince() > cstored->LimitBumpTime() &&
		    cstored->LimitBump() + clive->Users() != clive->Limit())
		{
		    clive->SendMode("+l " + mstring(cstored->LimitBump() + clive->Users()));
		}

		found = true;
	    }
	    // Removing bans ...

	    {
		if (Magick::instance().chanserv.LCK_Bantime())
		    bantime = Magick::instance().chanserv.DEF_Bantime();
		if (bantime)
		{
		    vector < mstring > rem;
		    vector < mstring >::iterator ri;
		    {
			RLOCK2((lck_ChanServ, lck_live, clive->Name().LowerCase(), "bans"));
			for (di = clive->bans.begin(); di != clive->bans.end(); di++)
			{
			    if (di->second.SecondsSince() > bantime)
			    {
				rem.push_back(di->first);
			    }
			}
		    }
		    for (ri = rem.begin(); ri != rem.end(); ri++)
		    {
			LOG(LM_DEBUG, "EVENT/UNBAN", (*ri, clive->Name(), ToHumanTime(bantime)));
			clive->SendMode("-b " + *ri);
		    }
		}
	    }

	    if (found)
	    {
		WLOCK((lck_ChanServ, lck_live, clive->Name().LowerCase(), "recent_parts"));
		for (di = clive->recent_parts.begin(); di != clive->recent_parts.end(); )
		{
		    if (di->second.SecondsSince() > parttime)
			clive->recent_parts.erase(di++);
		    else
			di++;
		}
	    }
	    else if (clive->recent_parts.size())
	    {
		WLOCK((lck_ChanServ, lck_live, clive->Name().LowerCase(), "recent_parts"));
		clive->recent_parts.clear();
	    }
	}
    }

    // Check if we should rename people who are past their
    // grace time on ident (if KillProtect is on, and they
    // are not on access list or secure is on).
    if (synctime.SecondsSince() >= Magick::instance().nickserv.Ident() &&
	Magick::instance().nickserv.IsLive(Magick::instance().nickserv.FirstName()))
    {
	vector < mstring > chunked;
//	vector < mstring > noversion;
	{
	    RLOCK((lck_NickServ, lck_live));
	    for (nli = Magick::instance().nickserv.LiveBegin(); nli != Magick::instance().nickserv.LiveEnd(); nli++)
	    {
		map_entry < Nick_Live_t > nlive(nli->second);
//		if (nlive->Version().empty() && !nlive->IsServices() && !nlive->HasMode("o") &&
//		    nlive->MySignonTime().SecondsSince() >= Magick::instance().nickserv.Version())
//		{
//		    // If they identify to a valid nickname, skip it!
//		    if (!Magick::instance().nickserv.IsStored(nlive->Name()) ||
//			!Magick::instance().nickserv.GetStored(nlive->Name())->IsOnline())
//			noversion.push_back(nlive->Name());
//		}
		if (Magick::instance().nickserv.IsStored(nlive->Name()))
		{
		    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(nlive->Name());
		    if (!nstored->IsOnline() && nstored->Protect() && !nlive->IsServices() && nlive->Squit().empty() &&
			nlive->MySignonTime().SecondsSince() >= Magick::instance().nickserv.Ident())
		    {
			chunked.push_back(nstored->Name());
		    }
		}
	    }
	}
//	for (i = 0; i < noversion.size(); ++i)
//	{
//	    if (!Magick::instance().nickserv.IsLive(noversion[i]))
//		continue;
//	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(noversion[i]);
//
//	    LOG(LM_INFO, "EVENT/BOTKILL", (nlive->Mask(Nick_Live_t::N_U_P_H)));
//	    Magick::instance().server.KILL(Magick::instance().nickserv.FirstName(), noversion[i],
//					   Magick::instance().getMessage("MISC/ISBOT"));
//	}

	for (i = 0; i < chunked.size(); i++)
	{
	    if (!Magick::instance().nickserv.IsLive(chunked[i]) || !Magick::instance().nickserv.IsStored(chunked[i]))
		continue;

	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(chunked[i]);
	    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(chunked[i]);

	    mstring newnick = Magick::instance().nickserv.findnextnick(chunked[i]);

	    LOG(LM_INFO, "EVENT/KILLPROTECT", (nlive->Mask(Nick_Live_t::N_U_P_H)));
	    if (!newnick.empty() && !Magick::instance().server.proto.SVSNICK().empty())
	    {
		if (nstored->Forbidden())
		    NSEND(Magick::instance().nickserv.FirstName(), chunked[i], "MISC/RENAMED_FORBID");
		else
		    NSEND(Magick::instance().nickserv.FirstName(), chunked[i], "MISC/RENAMED_IDENT");
		Magick::instance().server.SVSNICK(Magick::instance().nickserv.FirstName(), chunked[i], newnick);
	    }
	    else
	    {
		if (nstored->Forbidden())
		    Magick::instance().server.KILL(Magick::instance().nickserv.FirstName(), chunked[i],
						   Magick::instance().getMessage("NS_YOU_STATUS/ISFORBIDDEN"));
		else
		    Magick::instance().server.KILL(Magick::instance().nickserv.FirstName(), chunked[i],
						   Magick::instance().getMessage("NS_SET/PROTECT"));
		Magick::instance().server.NICK(chunked[i],
					       (Magick::instance().startup.Ownuser() ? chunked[i].LowerCase() :
						Magick::instance().startup.Services_User()),
					       Magick::instance().startup.Services_Host(),
					       Magick::instance().startup.Server_Name(),
					       Magick::instance().nickserv.Enforcer_Name());
	    }
	    Magick::instance().nickserv.AddRecovered(chunked[i], mDateTime::CurrentDateTime());
	}

	// Sign off clients we've decided to take.
	chunked.clear();
	{
	    RLOCK((lck_NickServ, "recovered"));
	    for (di = Magick::instance().nickserv.RecoveredBegin(); di != Magick::instance().nickserv.RecoveredEnd(); di++)
	    {
		if (di->second.SecondsSince() >= Magick::instance().nickserv.Release())
		{
		    if (Magick::instance().nickserv.IsLive(di->first) &&
			Magick::instance().nickserv.GetLive(di->first)->IsServices())
		    {
			chunked.push_back(di->first);
		    }
		}
	    }
	}
	for (i = 0; i < chunked.size(); i++)
	{
	    Magick::instance().nickserv.RemRecovered(chunked[i]);
	    Magick::instance().server.QUIT(chunked[i], "RECOVER period expired");
	}
    }
    ETCB();
}

void EventTask::do_modes(mDateTime & synctime)
{
    BTCB();
    CP(("Starting PENDING MODES check ..."));

    static_cast < void > (synctime);

    set < mstring > cmp;
    set < mstring >::iterator iter;
    unsigned int i;

    {
	WLOCK((lck_Events, "cmodes_pending"));
	cmp = cmodes_pending;
	MCB(cmodes_pending.size());
	cmodes_pending.clear();
	MCE(cmodes_pending.size());
    }
    map < mstring, vector < mstring > > modelines;
    map < mstring, vector < mstring > >::iterator ml;
    for (iter = cmp.begin(); iter != cmp.end(); iter++)
    {
	if (Magick::instance().chanserv.IsLive(*iter))
	{
	    map_entry < Chan_Live_t > chan = Magick::instance().chanserv.GetLive(*iter);
	    COM(("Looking at channel %s", chan->Name().c_str()));
	    if (!chan->p_modes_on.empty() || !chan->p_modes_off.empty())
	    {
		unsigned int j, k;
		mstring mode;
		mstring modeparam;

		{
		    RLOCK2((lck_ChanServ, lck_live, *iter, "p_modes_off"));
		    RLOCK3((lck_ChanServ, lck_live, *iter, "p_modes_off_params"));
		    CP(("p_modes_off_size %d (%s)", chan->p_modes_off.size(), chan->p_modes_off.c_str()));
		    for (i = 0, j = 0, k = 0; i < chan->p_modes_off.size(); i++)
		    {
			COM(("i = %d (%c), j = %d, k = %d", i, chan->p_modes_off[i], j, k));
			if (j >= Magick::instance().server.proto.Modes())
			{
			    modelines[*iter].push_back(mode + " " + modeparam);
			    mode.erase();
			    modeparam.erase();
			    j = 0;
			}
			if (mode.empty())
			    mode += "-";
			mode += chan->p_modes_off[i];
			if (chan->p_modes_off[i] != 'l' &&
			    Magick::instance().server.proto.ChanModeArg().Contains(chan->p_modes_off[i]))
			{
			    if (!modeparam.empty())
				modeparam += " ";
			    modeparam += chan->p_modes_off_params[k];
			    j++;
			    k++;
			}
		    }
		    WLOCK2((lck_ChanServ, lck_live, *iter, "p_modes_off"));
		    WLOCK3((lck_ChanServ, lck_live, *iter, "p_modes_off_params"));
		    chan->p_modes_off.erase();
		    chan->p_modes_off_params.clear();
		}
		{
		    RLOCK2((lck_ChanServ, lck_live, *iter, "p_modes_on"));
		    RLOCK3((lck_ChanServ, lck_live, *iter, "p_modes_on_params"));
		    if (mode.size() && chan->p_modes_on.size())
			mode += "+";
		    CP(("p_modes_on_size %d (%s)", chan->p_modes_on.size(), chan->p_modes_on.c_str()));
		    for (i = 0, k = 0; i < chan->p_modes_on.size(); i++)
		    {
			COM(("i = %d (%c), j = %d, k = %d", i, chan->p_modes_on[i], j, k));
			if (j >= Magick::instance().server.proto.Modes())
			{
			    modelines[*iter].push_back(mode + " " + modeparam);
			    mode.erase();
			    modeparam.erase();
			    j = 0;
			}
			if (mode.empty())
			    mode += "+";
			mode += chan->p_modes_on[i];
			if (Magick::instance().server.proto.ChanModeArg().Contains(chan->p_modes_on[i]))
			{
			    if (!modeparam.empty())
				modeparam += " ";
			    modeparam += chan->p_modes_on_params[k];
			    j++;
			    k++;
			}
		    }
		    WLOCK2((lck_ChanServ, lck_live, *iter, "p_modes_on"));
		    WLOCK3((lck_ChanServ, lck_live, *iter, "p_modes_on_params"));
		    chan->p_modes_on.erase();
		    chan->p_modes_on_params.clear();
		}
		if (mode.size())
		    modelines[*iter].push_back(mode + " " + modeparam);
	    }
	}
    }
    for (ml = modelines.begin(); ml != modelines.end(); ml++)
    {
	for (i = 0; i < ml->second.size(); i++)
	    Magick::instance().server.MODE(Magick::instance().chanserv.FirstName(), ml->first, ml->second[i]);
    }
    ETCB();
}

void EventTask::do_msgcheck(mDateTime & synctime)
{
    BTCB();
    CP(("Starting EXPIRED MESSAGE check ..."));

    static_cast < void > (synctime);
    vector<unsigned long> chunked;
    unsigned int i;

    {
	MLOCK((lck_IrcSvcHandler, lck_MsgIdMap));
	if (Magick::instance().ircsvchandler != NULL)
	{
	    vector<unsigned long> chunked2;
	    IrcSvcHandler::msgidmap_t::iterator iter;
	    for (iter=Magick::instance().ircsvchandler->MsgIdMap.begin(); iter!=Magick::instance().ircsvchandler->MsgIdMap.end(); iter++)
	    {
		// iter->first is less reliable, but we have no choice now.
		if (iter->second == NULL)
		    chunked2.push_back(iter->first);

		map_entry<mMessage> msg(iter->second);
		if (!msg->validated())
		{
		    iter->second = NULL;
		    msg->setDelete();
		    chunked2.push_back(msg->msgid());
		}
		else if (msg->references() < 2 && msg->creation().SecondsSince() > Magick::instance().config.MSG_Seen_Time())
		{
		    chunked.push_back(msg->msgid());
		    Magick::instance().ircsvchandler->enqueue(msg->msgid(), P_Old);
		}
	    }
	    for (i=0; i<chunked2.size(); i++)
		Magick::instance().ircsvchandler->MsgIdMap.erase(chunked2[i]);
	}
    }

    if (chunked.size())
    {
	MLOCK((lck_AllDeps));
	map < mMessage::type_t, map < mstring, set < unsigned long > > >::iterator j;

	for (j = mMessage::AllDependancies.begin(); j != mMessage::AllDependancies.end(); j++)
	{
	    map < mstring, set < unsigned long > >::iterator k;
	    for (k = j->second.begin(); k != j->second.end(); )
	    {
		for (i=0; i<chunked.size(); i++)
		{
		    set<unsigned long>::iterator l = k->second.find(chunked[i]);
		    if (l != k->second.end())
			k->second.erase(l);
		}
		if (!k->second.size())
		    j->second.erase(k++);
		else
		    k++;
	    }
	}
    }
    ETCB();
}

void EventTask::do_ping(mDateTime & synctime)
{
    BTCB();
    CP(("Starting SERVER PING ..."));

    static_cast < void > (synctime);

    Server::list_t::iterator si;

    float min = -1, max = 0, sum = 0, avg = 0;
    size_t count = 0;

    {
	RLOCK2((lck_Server, lck_list));
	for (si = Magick::instance().server.ListBegin(); si != Magick::instance().server.ListEnd(); si++)
	{
	    map_entry < Server_t > server(si->second);
	    if (min == -1 || server->Lag() < min)
		min = server->Lag();
	    if (server->Lag() > max)
		max = server->Lag();
	    sum += server->Lag();
	    count++;
	}
    }
    if (count >= 3)
	avg = (sum - min - max) / static_cast<float>(count - 2);
    else
	avg = sum / static_cast<float>(count);

    if (avg < 0.0)
    {
	LOG(LM_ERROR, "EVENT/LEVEL_BAD", (fmstring("%.3f", avg)));
    }
    else if (avg >= static_cast < float >
	(Magick::instance().startup.Lagtime() * (Magick::instance().Level() - Magick::instance().startup.Level() + 1)))
    {
	Magick::instance().LevelUp();
	LOG(LM_WARNING, "EVENT/LEVEL_UP", (fmstring("%.3f", avg)));
	if (Magick::instance().Level() > Magick::instance().startup.Max_Level())
	{
	    LOG(LM_ERROR, "EVENT/LEVEL_MAX", (Magick::instance().startup.Max_Level()));
	    Magick::instance().Disconnect();
	    return;
	}
    }
    else if (Magick::instance().Level() > Magick::instance().startup.Level() &&
	     avg < static_cast < float >
	     (Magick::instance().startup.Lagtime() * (Magick::instance().Level() - Magick::instance().startup.Level())))
    {
	Magick::instance().LevelDown();
	LOG(LM_WARNING, "EVENT/LEVEL_DOWN", (fmstring("%.3f", avg)));
    }

    {
	RLOCK((lck_IrcSvcHandler));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() <= 3)
	{
	    RLOCK2((lck_Server, lck_list));
	    for (si = Magick::instance().server.ListBegin(); si != Magick::instance().server.ListEnd(); si++)
	    {
		map_entry < Server_t > server(si->second);
		server->Ping();
	    }
	    NLOG(LM_TRACE, "EVENT/PING");
	}
    }
    ETCB();
}

void EventTask::DumpB() const
{
    BTCB();
    MB(0, (last_expire, last_save, last_check, last_ping));
    ETCB();
}

void EventTask::DumpE() const
{
    BTCB();
    ME(0, (last_expire, last_save, last_check, last_ping));
    ETCB();
}
