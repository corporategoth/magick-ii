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
** Revision 1.127  2000/06/25 07:58:48  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.126  2000/06/21 09:00:05  prez
** Fixed bug in mFile
**
** Revision 1.125  2000/06/18 13:31:47  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.124  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.123  2000/06/12 08:15:36  prez
** Added 'minimum threads' option to config (set to 2)
**
** Revision 1.122  2000/06/12 06:07:50  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.121  2000/06/11 08:20:11  prez
** More minor bug fixes, godda love testers.
**
** Revision 1.120  2000/06/06 08:57:54  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.119  2000/05/25 08:16:38  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.118  2000/05/21 04:49:39  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.117  2000/05/20 03:28:11  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.116  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.115  2000/05/13 06:48:23  ungod
** no message
**
** Revision 1.114  2000/05/03 14:12:22  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.113  2000/05/01 03:11:40  ungod
** xmlisation of entlist_t done
**
** Revision 1.112  2000/04/15 11:11:44  ungod
** starting xmlage of magick
**
** Revision 1.111  2000/04/02 07:25:05  prez
** Fixed low watermarks with threads, it all works now!
**
** Revision 1.110  2000/03/29 14:03:00  prez
** Fixed the failure to create thread condition ...
**
** Revision 1.109  2000/03/29 14:00:18  prez
** Fixed the thread pool system, and the watermarks.
**
** Revision 1.108  2000/03/29 09:41:17  prez
** Attempting to fix thread problem with mBase, and added notification
** of new memos on join of channel or signon to network.
**
** Revision 1.107  2000/03/28 16:20:57  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.106  2000/03/27 10:40:11  prez
** Started implementing revenge
**
** Revision 1.105  2000/03/24 15:35:17  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.104  2000/03/19 08:50:53  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.103  2000/03/14 13:36:46  prez
** Finished P12 compliance (SJOIN, etc).
**
** Revision 1.102  2000/03/14 10:05:16  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.101  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.100  2000/03/02 11:59:45  prez
** More helpfile updates (slowly but surely writing it)
**
** Revision 1.99  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.98  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.97  2000/02/17 12:55:04  ungod
** still working on borlandization
**
** Revision 1.96  2000/02/16 12:59:38  ungod
** fixing for borland compilability
**
** Revision 1.95  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.94  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "base.h"
#include "lockable.h"
#include "magick.h"

bool mBase::TaskOpened;
mBaseTask mBase::BaseTask;

mstring mUserDef::UserDef(mstring type)
{
    FT("mUserDef::UserDef", (type));
    if (i_UserDef.empty() || i_UserDef.find(type.LowerCase()) == i_UserDef.end())
    {
	RET("");
    }
    else
    {
	mstring retval = i_UserDef[type.LowerCase()];
	RET(retval);
    }
}


void mUserDef::UserDef(mstring type, mstring val)
{
    FT("mUserDef::UserDef", (type, val));
    if (val.UpperCase() == "NONE")
    {
	i_UserDef.erase(type.LowerCase());
    }
    else
    {
	i_UserDef[type.LowerCase()] = val;
    }
}

// --------- end of mUserDef -----------------------------------


entlist_t::entlist_t(mstring entry, mstring nick, mDateTime modtime)
{
    FT("entlist_t::entlist_t", (entry, nick));
    i_Entry = entry;
    i_Last_Modify_Time = modtime;
    i_Last_Modifier = nick;
}


void entlist_t::operator=(const entlist_t &in)
{
    FT("entlist_t::operator=", ("(const entlist_t &) in"));
    i_Entry=in.i_Entry;
    i_Last_Modify_Time=in.i_Last_Modify_Time;
    i_Last_Modifier=in.i_Last_Modifier;
    map<mstring,mstring>::const_iterator i;
    i_UserDef.clear();
    for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
	i_UserDef[i->first]=i->second;
}


size_t entlist_t::Usage()
{
    size_t retval = 0;
    retval += i_Entry.capacity();
    retval += i_Last_Modifier.capacity();
    retval += sizeof(i_Last_Modify_Time.Internal());
    map<mstring,mstring>::iterator i;
    for (i=i_UserDef.begin(); i!=i_UserDef.end(); i++)
    {
	retval += i->first.capacity();
	retval += i->second.capacity();
    }
    return retval;
}


// --------- end of entlist_t -----------------------------------


mBase::mBase()
{
    NFT("mBase::mBase");
    TaskOpened=false;
}

void mBase::push_message(const mstring& message)
{
    FT("mBase::push_message", (message));
    if(TaskOpened==false)
    {
	if(BaseTask.open()!=0)
	{
	    CP(("Failed to create initial thread"));
	    return;
	}
	while (BaseTask.thr_count() < Parent->config.Min_Threads())
	{
	    if(BaseTask.activate(THR_NEW_LWP | THR_JOINABLE, 1, 1)!=0)
	    {
		CP(("Failed to create additional (minimum) thread"));
		return;
	    }
	}
    }
    CH(T_Chatter::From,message);
    BaseTask.message(message);
}

void mBase::init()
{
    NFT("mBase::init");

    if(TaskOpened==false)
    {
	if(BaseTask.open()!=0)
	{
	    CP(("Failed to create initial thread"));
	    return;
	}
	while (BaseTask.thr_count() < Parent->config.Min_Threads())
	{
	    if(BaseTask.activate(THR_NEW_LWP | THR_JOINABLE, 1, 1)!=0)
	    {
		CP(("Failed to create additional (minimum) thread"));
		return;
	    }
	}
    }
    MLOCK(("MessageQueue"));
    BaseTask.message_queue_.high_water_mark(Parent->config.High_Water_Mark() * (sizeof(ACE_Method_Object *) * 2));
    BaseTask.message_queue_.low_water_mark(BaseTask.message_queue_.high_water_mark());
}

bool mBase::signon(const mstring &nickname)
{
    FT("mBase::signon", (nickname));

    RLOCK(("NickServ", "live", nickname.LowerCase()));
    if (Parent->nickserv.IsLive(nickname))
    {
	RET(false);
    }
    else
    {
	Parent->server.NICK(nickname,
		(Parent->startup.Ownuser() ? nickname.LowerCase() : Parent->startup.Services_User()),
		Parent->startup.Services_Host(), Parent->startup.Server_Name(), realname);
	RET(true);
    }
}

bool mBase::signoff(const mstring &nickname)
{
    FT("mBase::signoff", (nickname));

    RLOCK(("NickServ", "live", nickname.LowerCase()));
    if (Parent->nickserv.IsLive(nickname))
    {
	Parent->server.QUIT(nickname);
	RET(true);
    }
    else
    {
	RET(false);
    }
}


void mBase::privmsg(const mstring &source, const mstring &dest, const mstring &pszFormat, ...)
{
    FT("mBase::privmsg", (source, dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    privmsgV(source, dest, pszFormat, argptr);
    va_end(argptr);
}


void mBase::privmsg(const mstring &dest, const mstring &pszFormat, ...)
{
    FT("mBase::privmsg", (dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    privmsgV(FirstName(), dest, pszFormat, argptr);
    va_end(argptr);
}


void mBase::privmsgV(const mstring &source, const mstring &dest, const mstring &pszFormat, va_list argptr)
{
    FT("mBase::privmsgV", (source, dest, pszFormat));

    mstring message;
    message.FormatV(pszFormat.c_str(), argptr);

    RLOCK(("NickServ", "live", dest.LowerCase()));
    RLOCK2(("ChanServ", "live", dest.LowerCase()));
    if (IsName(source) && (Parent->nickserv.IsLive(dest) || Parent->chanserv.IsLive(dest)))
	Parent->server.PRIVMSG(source, dest, message);
}


void mBase::notice(const mstring &source, const mstring &dest, const mstring &pszFormat, ...)
{
    FT("mBase::notice", (source, dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    noticeV(source, dest, pszFormat, argptr);
    va_end(argptr);
}


void mBase::notice(const mstring &dest, const mstring &pszFormat, ...)
{
    FT("mBase::notice", (dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    noticeV(FirstName(), dest, pszFormat, argptr);
    va_end(argptr);
}


void mBase::noticeV(const mstring &source, const mstring &dest, const mstring &pszFormat, va_list argptr)
{
    FT("mBase::noticeV", (source, dest, pszFormat));

    mstring message;
    message.FormatV(pszFormat.c_str(), argptr);
    RLOCK(("NickServ", "live", dest.LowerCase()));
    RLOCK2(("ChanServ", "live", dest.LowerCase()));
    if (IsName(source) && (Parent->nickserv.IsLive(dest) || Parent->chanserv.IsLive(dest)))
	Parent->server.NOTICE(source, dest, message);
}


void mBase::send(const mstring &source, const mstring &dest, const mstring &pszFormat, ...)
{
    FT("mBase::send", (source, dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    sendV(source, dest, pszFormat, argptr);
    va_end(argptr);
}

void mBase::send(const mstring &dest, const mstring &pszFormat, ...)
{
    FT("mBase::send", (dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    sendV(FirstName(), dest, pszFormat, argptr);
    va_end(argptr);
}

void mBase::sendV(const mstring &source, const mstring &dest, const mstring &pszFormat, va_list argptr)
{
    FT("mBase::sendV", (source, dest, pszFormat));

    RLOCK(("NickServ", "live", dest.LowerCase()));
    if (IsName(source) && Parent->nickserv.IsLive(dest))
    {
	if (!Parent->nickserv.LCK_PRIVMSG() && Parent->nickserv.IsStored(dest) &&
		Parent->nickserv.stored[dest.LowerCase()].IsOnline())
	{
	    if (Parent->nickserv.stored[dest.LowerCase()].PRIVMSG()) {
		privmsgV(source, dest, pszFormat, argptr);
	    }
	    else
	    {
		noticeV(source, dest, pszFormat, argptr);
	    }
	}
	else
	{
	    if (Parent->nickserv.DEF_PRIVMSG())
	    {
		privmsgV(source, dest, pszFormat, argptr);
	    }
	    else
	    {
		noticeV(source, dest, pszFormat, argptr);
	    }
	}
    }
}


void privmsg(const mstring& source, const mstring &dest, const mstring &pszFormat, ...)
{
    FT("privmsg", (source, dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);

    RLOCK(("NickServ", "live", source.LowerCase()));
    if (!Parent->nickserv.IsLive(source))
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"PRIVMSG", source.c_str());
    else if (Parent->nickserv.live[source.LowerCase()].IsServices())
    {
	if (Parent->operserv.IsName(source))
	    Parent->operserv.privmsgV(source, dest, pszFormat, argptr);

	else if (Parent->nickserv.IsName(source))
	    Parent->nickserv.privmsgV(source, dest, pszFormat, argptr);

	else if (Parent->chanserv.IsName(source))
	    Parent->chanserv.privmsgV(source, dest, pszFormat, argptr);

	else if (Parent->memoserv.IsName(source))
	    Parent->memoserv.privmsgV(source, dest, pszFormat, argptr);

	else if (Parent->commserv.IsName(source))
	    Parent->commserv.privmsgV(source, dest, pszFormat, argptr);

	else if (Parent->servmsg.IsName(source))
	    Parent->servmsg.privmsgV(source, dest, pszFormat, argptr);

	// scripted hosts ...
	else
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"PRIVMSG", source.c_str());
    }
    else
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"PRIVMSG", source.c_str());
    va_end(argptr);
}


void notice(const mstring& source, const mstring &dest, const mstring &pszFormat, ...)
{
    FT("notice", (source, dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    RLOCK(("NickServ", "live", source.LowerCase()));
    if (!Parent->nickserv.IsLive(source))
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"NOTICE", source.c_str());
    else if (Parent->nickserv.live[source.LowerCase()].IsServices())
    {
	if (Parent->operserv.IsName(source))
	    Parent->operserv.noticeV(source, dest, pszFormat, argptr);

	else if (Parent->nickserv.IsName(source))
	    Parent->nickserv.noticeV(source, dest, pszFormat, argptr);

	else if (Parent->chanserv.IsName(source))
	    Parent->chanserv.noticeV(source, dest, pszFormat, argptr);

	else if (Parent->memoserv.IsName(source))
	    Parent->memoserv.noticeV(source, dest, pszFormat, argptr);

	else if (Parent->commserv.IsName(source))
	    Parent->commserv.noticeV(source, dest, pszFormat, argptr);

	else if (Parent->servmsg.IsName(source))
	    Parent->servmsg.noticeV(source, dest, pszFormat, argptr);

	// scripted hosts ...
	else
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"NOTICE", source.c_str());
    }
    else
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"NOTICE", source.c_str());
    va_end(argptr);
}


void send(const mstring& source, const mstring &dest, const mstring &pszFormat, ...)
{
    FT("send", (source, dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    RLOCK(("NickServ", "live", source.LowerCase()));
    if (!Parent->nickserv.IsLive(source))
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"SEND", source.c_str());
    else if (Parent->nickserv.live[source.LowerCase()].IsServices())
    {
	if (Parent->operserv.IsName(source))
	    Parent->operserv.sendV(source, dest, pszFormat, argptr);

	else if (Parent->nickserv.IsName(source))
	    Parent->nickserv.sendV(source, dest, pszFormat, argptr);

	else if (Parent->chanserv.IsName(source))
	    Parent->chanserv.sendV(source, dest, pszFormat, argptr);

	else if (Parent->memoserv.IsName(source))
	    Parent->memoserv.sendV(source, dest, pszFormat, argptr);

	else if (Parent->commserv.IsName(source))
	    Parent->commserv.sendV(source, dest, pszFormat, argptr);

	else if (Parent->servmsg.IsName(source))
	    Parent->servmsg.sendV(source, dest, pszFormat, argptr);

	// scripted hosts ...
	else
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"SEND", source.c_str());
    }
    else
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"SEND", source.c_str());

    va_end(argptr);
}

void announce(const mstring& source, const mstring &pszFormat, ...)
{
    FT("announce", (source, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    mstring message;
    message.FormatV(pszFormat.c_str(), argptr);
    va_end(argptr);

    if (!Parent->nickserv.IsLive(source))
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"ANNOUNCE", source.c_str());
    else if (Parent->nickserv.live[source.LowerCase()].IsServices())
    {
	if (Parent->server.proto.Globops())
	    Parent->server.GLOBOPS(source, message);
	else
	    Parent->server.WALLOPS(source, message);
    }
    else
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"ANNOUNCE", source.c_str());
}

void mBase::shutdown()
{
    NFT("mBase::shutdown");
    int j=BaseTask.thr_count();
    for(int i=0;i<j;i++)
	BaseTask.i_shutdown();
}

int mBaseTask::open(void *in)
{
    NFT("mBaseTask::open");
    mBase::TaskOpened=true;
    int retval = activate();
    RET(retval);
}

int mBaseTask::svc(void)
{
    mThread::Attach(tt_mBase);
    NFT("mBaseTask::svc");
    while(!Parent->Shutdown())
    {
	ACE_Method_Object *tmp;
	{
	    MLOCK(("ActivationQueue"));
	    tmp = this->activation_queue_.dequeue();
	}
	auto_ptr<ACE_Method_Object> mo(tmp);
	if(mo->call()<0)
	    break;
    }
    DRET(0);
}

class mBaseTaskmessage_MO : public ACE_Method_Object
{
public:
    mBaseTaskmessage_MO(mBaseTask *parent, const mstring& data)
    {
	FT("mBaseTaskmessage_MO::mBaseTaskmessage_MO",((void *)parent,data));
	i_parent=parent;
	i_data=data;
    }
    virtual int call()
    {
	NFT("mBaseTaskmessage_MO::call");
	int retval = i_parent->message_i(i_data);
	RET(retval);
    }
private:
    mBaseTask *i_parent;
    mstring i_data;
};

void mBaseTask::message(const mstring& message)
{
    FT("mBaseTask::message",(message));
    { MLOCK(("MessageQueue"));
    if(message_queue_.is_full())
    {
	CP(("Queue is full - Starting new thread and increasing watermarks ..."));
	if(activate(THR_NEW_LWP | THR_JOINABLE, 1, 1)!=0)
	{
	    CP(("Couldn't start new thread to handle excess load, will retry next message"));
	}
	else
	{
	    message_queue_.high_water_mark(Parent->config.High_Water_Mark() * (thr_count()) * (sizeof(ACE_Method_Object *) * 2));
	    message_queue_.low_water_mark(message_queue_.high_water_mark());
	    Log(LM_NOTICE, Parent->getLogMessage("EVENT/NEW_THREAD"));
	}
    }}
    MLOCK2(("ActivationQueue"));
    activation_queue_.enqueue(new mBaseTaskmessage_MO(this,message));
}

int mBaseTask::message_i(const mstring& message)
{
    FT("mBaseTask::message_i",(message));
    // NOTE: No need to handle 'non-user messages' here, because
    // anything that is not a user PRIVMSG/NOTICE goes directly
    // to the server routine anyway.

    mstring data = PreParse(message);

    mstring source, type, target;
    if (data == "")
	RET(0);
    source=data.ExtractWord(1,": ");
    type  =data.ExtractWord(2,": ").UpperCase();
    target=data.ExtractWord(3,": ");

    CH(T_Chatter::From,data);	    

    if ((type == "PRIVMSG" || type == "NOTICE") && !IsChan(target) &&
    	Parent->nickserv.IsLive(source))
    {
	if (target.Contains("@"))
	{
	    target = target.Before("@");
	    data = data.Before(" ", 2) + " " + target + " " + data.After(" ", 3);
	    CP(("Target changed, new data: %s", data.c_str()));
	}

	if (!Parent->nickserv.live[source.LowerCase()].FloodTrigger())
	{
	    // Find out if the target nick is one of the services 'clones'
	    // Pass the message to them if so.
	    // before even that, check if it's script overriden via
	    //     Parent->checkifhandled(servername,command)
	    // if so, Parent->doscripthandle(server,command,data);

	    if (Parent->operserv.IsName(target))
		Parent->operserv.execute(data);

	    else if (Parent->nickserv.IsName(target) && Parent->nickserv.MSG())
		Parent->nickserv.execute(data);

	    else if (Parent->chanserv.IsName(target) && Parent->chanserv.MSG())
		Parent->chanserv.execute(data);

	    else if (Parent->memoserv.IsName(target) && Parent->memoserv.MSG())
		Parent->memoserv.execute(data);

	    else if (Parent->commserv.IsName(target) && Parent->commserv.MSG())
		Parent->commserv.execute(data);

	    else if (Parent->servmsg.IsName(target) && Parent->servmsg.MSG())
		Parent->servmsg.execute(data);

	    // else check if it's script handled, might do up a list of script servers
	    // in the magick object to check against, else trash it.

	    else	// PRIVMSG or NOTICE to non-service
		Parent->server.execute(data);

	}
	else if (Parent->operserv.Log_Ignore())
	{
	    // Check if we're to log ignore messages, and log them here.
	    Log(LM_DEBUG, Parent->getLogMessage("OPERSERV/IGNORED"),
			source.c_str(), data.After(" ").c_str());
	}
    }
    else
	Parent->server.execute(data);

    {MLOCK(("MessageQueue"));
    if (thr_count() > 1)
    {
	CP(("thr_count = %d, message queue = %d, lwm = %d, hwm = %d",
		thr_count(), message_queue_.message_count(),
		Parent->config.Low_Water_Mark() + (Parent->config.High_Water_Mark() * (thr_count()-2)),
		thr_count() * Parent->config.High_Water_Mark()));
    }
    else
    {
	CP(("thr_count = %d, message queue = %d, lwm = %d, hwm = %d",
		thr_count(), message_queue_.message_count(), 0,
		thr_count() * Parent->config.High_Water_Mark()));
    }
    if(thr_count() > Parent->config.Min_Threads() &&
	message_queue_.message_count() < Parent->config.Low_Water_Mark() +
			(Parent->config.High_Water_Mark() * (thr_count()-2)))
    {
	COM(("Low water mark reached, killing thread."));
	message_queue_.high_water_mark(Parent->config.High_Water_Mark() * (thr_count()-1) * (sizeof(ACE_Method_Object *) * 2));
	message_queue_.low_water_mark(message_queue_.high_water_mark());
	Log(LM_NOTICE, Parent->getLogMessage("EVENT/KILL_THREAD"));
	RET(-1);
    }}
    FLUSH();
    RET(0);
}

mstring mBaseTask::PreParse(const mstring& message)
{
    FT("mBaseTask::PreParse", (message));
    mstring data = message;

    if (Parent->server.proto.Tokens())
    {
	if (message[0u] == ':' &&
	   Parent->server.proto.GetToken(message.ExtractWord(2, " ")) != "")
	{
	    data = "";
	    data << message.ExtractWord(1, " ") << " " <<
		Parent->server.proto.GetToken(message.ExtractWord(2, " ")) <<
		" " << message.After(" ", 2);
	}
	else if (Parent->server.proto.GetToken(message.ExtractWord(1, " ")) != "")
	{
	    data = "";
	    data << Parent->server.proto.GetToken(message.ExtractWord(1, " ")) <<
		" " << message.After(" ", 1);
	}
    }
    RET(data);
}

void mBaseTask::i_shutdown()
{
    NFT("mBaseTask::i_shutdown");
    MLOCK(("ActivationQueue"));
    activation_queue_.enqueue(new shutdown_MO);
}

// Command Map stuff ...

void CommandMap::AddSystemCommand(mstring service, mstring command,
	    mstring committees, functor function)
{
    FT("CommandMap::AddSystemCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_system"));
    i_system[service.LowerCase()].push_back(triplet<mstring, mstring, functor>
		    (command.UpperCase(),
		    ((committees != "") ? committees.LowerCase() : mstring("all")),
		    function));
}


void CommandMap::RemSystemCommand(mstring service, mstring command,
	    mstring committees)
{
    FT("CommandMap::RemSystemCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_system"));
    if (i_system.find(service.LowerCase()) != i_system.end())
    {
	clist_iter iter;
	for (iter = i_system[service.LowerCase()].begin();
		iter != i_system[service.LowerCase()].end(); iter++)
	{
	    if (iter->first == command.UpperCase() &&
		    mstring(" " + iter->second + " ").Contains(
		    mstring(" " + committees.LowerCase() + " ")))
	    {
		i_system[service.LowerCase()].erase(iter);
		if (!i_system[service.LowerCase()].size())
		    i_system.erase(service.LowerCase());
		return;
	    }
	}
    }
}


void CommandMap::AddCommand(mstring service, mstring command,
	    mstring committees, functor function)
{
    FT("CommandMap::AddCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_user"));
    i_user[service.LowerCase()].push_back(triplet<mstring, mstring, functor>
		    (command.UpperCase(),
		    ((committees != "") ? committees.LowerCase() : mstring("all")),
		    function));
}


void CommandMap::RemCommand(mstring service, mstring command,
	    mstring committees)
{
    FT("CommandMap::RemCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_user"));
    if (i_user.find(service.LowerCase()) != i_user.end())
    {
	clist_iter iter;
	for (iter = i_user[service.LowerCase()].begin();
		iter != i_user[service.LowerCase()].end(); iter++)
	{
	    if (iter->first == command.UpperCase() &&
		    mstring(" " + iter->second + " ").Contains(
		    mstring(" " + committees.LowerCase() + " ")))
	    {
		i_user[service.LowerCase()].erase(iter);
		if (!i_user[service.LowerCase()].size())
		    i_user.erase(service.LowerCase());
		return;
	    }
	}
    }
}


pair<bool, CommandMap::functor> CommandMap::GetUserCommand(mstring service, mstring command,
	    mstring user)
{
    FT("CommandMap::GetUserCommand", (service, command, user));
    unsigned int i;
    pair<bool, functor> retval = pair<bool, functor>(false, NULL);
    clist_iter iter;
    mstring type, list;

    // IF i_system exists
    //   IF command (pattern) exists
    //     IF (" " + cmd_committees + " ") contains ANY OF committees
    //       RETURN true
    //     ENDIF
    //   ENDIF
    // ENDIF
    // RETURN false;

    if (Parent->operserv.IsName(service))
	type = Parent->operserv.GetInternalName().LowerCase();
    else if (Parent->nickserv.IsName(service))
	type = Parent->nickserv.GetInternalName().LowerCase();
    else if (Parent->chanserv.IsName(service))
	type = Parent->chanserv.GetInternalName().LowerCase();
    else if (Parent->memoserv.IsName(service))
	type = Parent->memoserv.GetInternalName().LowerCase();
    else if (Parent->commserv.IsName(service))
	type = Parent->commserv.GetInternalName().LowerCase();
    else if (Parent->servmsg.IsName(service))
	type = Parent->servmsg.GetInternalName().LowerCase();
    //else
    //  scripted stuff ...

    if (type == "")
	NRET(pair<bool_functor>,retval);

    RLOCK(("CommandMap", "i_user"));
    if (i_user.find(type) != i_user.end())
    {
	for (iter=i_user[type].begin();
		iter!=i_user[type].end(); iter++)
	{
	    if (command.UpperCase().Matches(iter->first))
	    {
		for (i=1; i <= iter->second.WordCount(" "); i++)
		{
		    list = iter->second.ExtractWord(i, " ").UpperCase();
		    // If its a command for "ALL" users, OR
		    // its a valid committee AND a valid (reg'd + online) user
		    //       AND that user is on the committee
		    if (Parent->commserv.IsList(list)
			&& Parent->commserv.list[list].IsOn(user))
		    {
			retval.first = true;
			retval.second = iter->third;
			NRET(pair<bool_functor>,retval);
		    }
		}
	    }
	}
    }
    NRET(pair<bool_functor>,retval);
}

pair<bool, CommandMap::functor> CommandMap::GetSystemCommand(mstring service, mstring command,
	    mstring user)
{
    FT("CommandMap::GetSystemCommand", (service, command, user));
    unsigned int i;
    pair<bool, functor> retval = pair<bool, functor>(false, NULL);
    clist_iter iter;
    mstring type, list;

    // IF i_system exists
    //   IF command (pattern) exists
    //     IF (" " + cmd_committees + " ") contains ANY OF committees
    //       RETURN true
    //     ENDIF
    //   ENDIF
    // ENDIF
    // RETURN false;

    if (Parent->operserv.IsName(service))
	type = Parent->operserv.GetInternalName().LowerCase();
    else if (Parent->nickserv.IsName(service))
	type = Parent->nickserv.GetInternalName().LowerCase();
    else if (Parent->chanserv.IsName(service))
	type = Parent->chanserv.GetInternalName().LowerCase();
    else if (Parent->memoserv.IsName(service))
	type = Parent->memoserv.GetInternalName().LowerCase();
    else if (Parent->commserv.IsName(service))
	type = Parent->commserv.GetInternalName().LowerCase();
    else if (Parent->servmsg.IsName(service))
	type = Parent->servmsg.GetInternalName().LowerCase();
    //else
    //  scripted stuff ...

    if (type == "")
	NRET(pair<bool_functor>,retval);

    RLOCK(("CommandMap", "i_system"));
    if (i_system.find(type) != i_system.end())
    {
	for (iter=i_system[type].begin();
		iter!=i_system[type].end(); iter++)
	{
	    if (command.UpperCase().Matches(iter->first))
	    {
		for (i=1; i <= iter->second.WordCount(" "); i++)
		{
		    list = iter->second.ExtractWord(i, " ").UpperCase();
		    // If its a command for "ALL" users, OR
		    // its a valid committee AND a valid (reg'd + online) user
		    //       AND that user is on the committee
		    if (Parent->commserv.IsList(list)
			 && Parent->commserv.list[list].IsOn(user))
		    {
			retval.first = true;
			retval.second = iter->third;
			NRET(pair<bool_functor>,retval);
		    }
		}
	    }
	}
    }
    NRET(pair<bool_functor>,retval);
}

bool CommandMap::DoCommand(mstring mynick, mstring user, mstring command,
	    mstring params)
{
    FT("CommandMap::DoCommand", (mynick, user, command, params));

    if (DoUserCommand(mynick, user, command, params) ||
	DoSystemCommand(mynick, user, command, params))
    {
	RET(true);
    }
    if (command.WordCount(" ") < 2)
	send(mynick, user, Parent->getMessage(user, "ERR_SYNTAX/UNKNOWN_COMMAND"),
			command.UpperCase().c_str(), mynick.c_str());
    else
	send(mynick, user, Parent->getMessage(user, "ERR_SYNTAX/UNKNOWN_OPTION"),
			command.UpperCase().c_str(), mynick.c_str(),
			command.Before(" ").UpperCase().c_str());
    RET(false);
}


bool CommandMap::DoUserCommand(mstring mynick, mstring user, mstring command,
	    mstring params)
{
    FT("CommandMap::DoUserCommand", (mynick, user, command, params));

    pair<bool,functor> cmd = GetUserCommand(mynick, command, user);
    if (cmd.first)
    {
	if (cmd.second != NULL)
	    (*cmd.second)(mynick, user, params);
	else
	    if (command.WordCount(" ") < 2)
		send(mynick, user, Parent->getMessage(user, "ERR_SYNTAX/UNKNOWN_COMMAND"),
			command.UpperCase().c_str(), mynick.c_str());
	    else
		send(mynick, user, Parent->getMessage(user, "ERR_SYNTAX/UNKNOWN_OPTION"),
			command.UpperCase().c_str(), mynick.c_str(),
			command.Before(" ").UpperCase().c_str());
	RET(true);
    }
    RET(false);
}


bool CommandMap::DoSystemCommand(mstring mynick, mstring user, mstring command,
	    mstring params)
{
    FT("CommandMap::DoSystemCommand", (mynick, user, command, params));

    pair<bool,functor> cmd = GetSystemCommand(mynick, command, user);
    if (cmd.first)
    {
	if (cmd.second != NULL)
	    (*cmd.second)(mynick, user, params);
	else
	    if (command.WordCount(" ") < 2)
		send(mynick, user, Parent->getMessage(user, "ERR_SYNTAX/UNKNOWN_COMMAND"),
			command.UpperCase().c_str(), mynick.c_str());
	    else
		send(mynick, user, Parent->getMessage(user, "ERR_SYNTAX/UNKNOWN_OPTION"),
			command.UpperCase().c_str(), mynick.c_str(),
			command.Before(" ").UpperCase().c_str());
	RET(true);
    }
    RET(false);
}


void do_1_2param(mstring mynick, mstring source, mstring params)
{
    FT("do_1_2param", (mynick, source, params));
    if (params.WordCount(" ") < 2)
    {
	send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
			params.Before(" ").UpperCase().c_str(), mynick.c_str(),
			params.Before(" ").UpperCase().c_str());
	return;
    }
    mstring command = params.Before(" ", 2).UpperCase();
    if (!Parent->commands.DoCommand(mynick, source, command, params))
    {
	// we're not worthy...
//	send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/UNKNOWN_OPTION"),
//			command.c_str(), mynick.c_str(),
//			command.Before(" ").c_str());
    }

}

void do_1_3param(mstring mynick, mstring source, mstring params)
{
    FT("do_1_3param", (mynick, source, params));
    if (params.WordCount(" ") < 3)
    {
	send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
			params.Before(" ").UpperCase().c_str(), mynick.c_str(),
			params.Before(" ").UpperCase().c_str());
	return;
    }
    mstring command = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (!Parent->commands.DoCommand(mynick, source, command, params))
    {
	// we're not worthy...
//	send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/UNKNOWN_OPTION"),
//			command.c_str(), mynick.c_str(),
//			command.Before(" ").c_str());
    }
}

void entlist_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    //TODO: Add your source code here
	if( pElement->IsA(tag_Entry) )   pElement->Retrieve(i_Entry);
	if( pElement->IsA(tag_Last_Modify_Time) )   pElement->Retrieve(i_Last_Modify_Time);
	if( pElement->IsA(tag_Last_Modifier) )   pElement->Retrieve(i_Last_Modifier);
    if( pElement->IsA(tag_UserDef) )
    {
        mstring tmpUserDef;
        pElement->Retrieve(tmpUserDef);
        i_UserDef[tmpUserDef.Before("\n")]=tmpUserDef.After("\n");
    }
}

void entlist_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    //TODO: Add your source code here
	pOut->BeginObject(tag_entlist_t, attribs);

	pOut->WriteElement(tag_Entry, i_Entry);
	pOut->WriteElement(tag_Last_Modify_Time, i_Last_Modify_Time);
	pOut->WriteElement(tag_Last_Modifier, i_Last_Modifier);

    map<mstring,mstring>::const_iterator iter;
    for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
    {
        pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
    }

	pOut->EndObject(tag_entlist_t);
}

SXP::Tag tag_entlist_t("entlist_t");
SXP::Tag tag_entlist_val_t("entlist_val_t");
SXP::Tag tag_Entry("Entry");
SXP::Tag tag_Value("Value");
SXP::Tag tag_ValueFirst("ValueFirst");
SXP::Tag tag_ValueSecond("ValueSecond");
SXP::Tag tag_Last_Modify_Time("Last_Modify_Time");
SXP::Tag tag_Last_Modifier("Last_Modifier");
SXP::Tag tag_UserDef("UserDef");
SXP::Tag tag_Stupid("Stupid");

