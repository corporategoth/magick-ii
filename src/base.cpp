#include "pch.h"
#ifdef _MSC_VER
#pragma hdrstop
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
#pragma ident "$Id$"
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.93  2000/02/15 10:11:14  prez
** *** empty log message ***
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
	RET(i_UserDef[type.LowerCase()]);
    }
}


mstring mUserDef::UserDef(mstring type, mstring val)
{
    FT("mUserDef::UserDef", (type, val));
    if (val.UpperCase() == "NONE")
    {
	i_UserDef.erase(type.LowerCase());
	RET("Value of " + type.UpperCase() + " has been cleared.");
    }
    else
    {
	i_UserDef[type.LowerCase()] = val;
	RET("Value of " + type.UpperCase() + " has been set to " + val + ".");
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


wxOutputStream &operator<<(wxOutputStream& out,const entlist_t& in)
{
    out<<in.i_Entry<<in.i_Last_Modify_Time<<in.i_Last_Modifier;

    map<mstring,mstring>::const_iterator j;
    out<<in.i_UserDef.size();
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	out<<(mstring)j->first<<(mstring)j->second;
    return out;
}


wxInputStream &operator>>(wxInputStream& in, entlist_t& out)
{
    unsigned int i,count;
    mstring dummy,dummy2;

    in>>out.i_Entry>>out.i_Last_Modify_Time>>out.i_Last_Modifier;

    out.i_UserDef.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>dummy>>dummy2;
	out.i_UserDef[dummy]=dummy2;
    }
    return in;
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
    }
    BaseTask.message_queue_.high_water_mark(Parent->config.High_Water_Mark()*sizeof(ACE_Method_Object *));
    BaseTask.message_queue_.low_water_mark(BaseTask.message_queue_.high_water_mark());
}

bool mBase::signon(const mstring &nickname)
{
    FT("mBase::signon", (nickname));

    if (Parent->nickserv.IsLive(nickname))
    {
	RET(false);
    }
    else
    {
	Parent->server.NICK(nickname,
		Parent->startup.Ownuser() ? nickname.LowerCase() : Parent->startup.Services_User(),
		Parent->startup.Services_Host(), Parent->startup.Server_Name(), realname);
	RET(true);
    }
}

bool mBase::signoff(const mstring &nickname)
{
    FT("mBase::signoff", (nickname));

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
    privmsgV(FirstName(), dest, pszFormat, argptr);
    va_end(argptr);
}


void mBase::noticeV(const mstring &source, const mstring &dest, const mstring &pszFormat, va_list argptr)
{
    FT("mBase::noticeV", (source, dest, pszFormat));

    mstring message;
    message.FormatV(pszFormat.c_str(), argptr);
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

    if (IsName(source) && Parent->nickserv.IsLive(dest))
    {
	if (Parent->nickserv.IsStored(dest) && !Parent->nickserv.LCK_PRIVMSG() &&
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

    if (Parent->nickserv.IsLive(source) &&
	Parent->nickserv.live[source.LowerCase()].IsServices())
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
	// else
    }
    va_end(argptr);
}


void notice(const mstring& source, const mstring &dest, const mstring &pszFormat, ...)
{
    FT("notice", (source, dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    if (Parent->nickserv.IsLive(source) &&
	Parent->nickserv.live[source.LowerCase()].IsServices())
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
	// else
    }
    va_end(argptr);
}


void send(const mstring& source, const mstring &dest, const mstring &pszFormat, ...)
{
    FT("send", (source, dest, pszFormat));

    va_list argptr;
    const char *str = pszFormat.c_str();
    va_start(argptr, str);
    if (Parent->nickserv.IsLive(source) &&
	Parent->nickserv.live[source.LowerCase()].IsServices())
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
	// else
    }

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

    // Put logic in here to choose GLOBOP or WALLOP
    Parent->server.GLOBOPS(source, message);

    va_end(argptr);
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
    RET(activate());
}

int mBaseTask::svc(void)
{
    NFT("mBaseTask::svc");
    mThread::Attach(tt_mBase);
    while(!Parent->Shutdown())
    {
	auto_ptr<ACE_Method_Object> mo(this->activation_queue_.dequeue());
	if(mo->call()==-1)
	    break;
    }
    mThread::Detach(tt_mBase);
    RET(0);
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
	i_parent->message_i(i_data);
	RET(0);
    }
private:
    mBaseTask *i_parent;
    mstring i_data;
};

void mBaseTask::message(const mstring& message)
{
    FT("mBaseTask::message",(message));
    if(message_queue_.is_full())
    {
	CP(("Queue is full - Starting new thread and increasing watermarks ..."));
	message_queue_.high_water_mark(Parent->config.High_Water_Mark()*(thr_count()+1)*sizeof(ACE_Method_Object *));
	message_queue_.low_water_mark(message_queue_.high_water_mark());
	if(activate(THR_NEW_LWP | THR_JOINABLE, 1, 1)!=0)
	    CP(("Couldn't start new thread to handle excess load, will retry next message"));
    }
    activation_queue_.enqueue(new mBaseTaskmessage_MO(this,message));
}

void mBaseTask::message_i(const mstring& message)
{
    FT("mBaseTask::message_i",(message));
    // NOTE: No need to handle 'non-user messages' here, because
    // anything that is not a user PRIVMSG/NOTICE goes directly
    // to the server routine anyway.
    mstring source, type, target;
    source=message.ExtractWord(1,": ");
    type  =message.ExtractWord(2,": ").UpperCase();
    target=message.ExtractWord(3,": ");

    CH(T_Chatter::From,message);	    

    if (type == "PRIVMSG" || type == "NOTICE")
    {
	// Split stuff for NON-CHANNEL traffic.
	if (!IsChan(target) && Parent->nickserv.IsLive(source))
	{
	    // Execute if we DONT (or havnt) trigger ignore
	    if (!Parent->nickserv.live[source.LowerCase()].FloodTrigger())
	    {

		// Find out if the target nick is one of the services 'clones'
		// Pass the message to them if so.
		// before even that, check if it's script overriden via
		//     Parent->checkifhandled(servername,command)
		// if so, Parent->doscripthandle(server,command,data);

		if (Parent->operserv.IsName(target))
		    Parent->operserv.execute(message);

		else if (Parent->nickserv.IsName(target) && Parent->nickserv.MSG())
		    Parent->nickserv.execute(message);

		else if (Parent->chanserv.IsName(target) && Parent->chanserv.MSG())
		    Parent->chanserv.execute(message);

		else if (Parent->memoserv.IsName(target) && Parent->memoserv.MSG())
		    Parent->memoserv.execute(message);

		else if (Parent->commserv.IsName(target) && Parent->commserv.MSG())
		    Parent->commserv.execute(message);

		else if (Parent->servmsg.IsName(target) && Parent->servmsg.MSG())
		    Parent->servmsg.execute(message);

		// else check if it's script handled, might do up a list of script servers
		// in the magick object to check against, else trash it.

		else	// PRIVMSG or NOTICE to non-service
		    Parent->server.execute(message);
	    }
	    else
	    {
		// Check if we're to log ignore messages, and log them here.
		if (Parent->operserv.Log_Ignore())
		    wxLogInfo(Parent->getMessage("OPERSERV/IGNORED"),
			source.c_str(), message.After(" ").c_str());
	    }
	}
	else	// Channel messages
	    Parent->server.execute(message);
    } 
    else	// Non PRIVMSG and NOTICE
	Parent->server.execute(message);

    if(thr_count()>1&&message_queue_.message_count()<
	    Parent->config.High_Water_Mark()*(thr_count()-2)+Parent->config.Low_Water_Mark())
    {
	message_queue_.high_water_mark(Parent->config.High_Water_Mark()*(ACE_Thread_Manager::instance()->count_threads()-1)*sizeof(ACE_Method_Object *));
	message_queue_.low_water_mark(message_queue_.high_water_mark());
	COM(("Low water mark reached, killing thread."));
	i_shutdown();
    }
}

void mBaseTask::i_shutdown()
{
    NFT("mBaseTask::message_i");
    activation_queue_.enqueue(new shutdown_MO);
}

// Command Map stuff ...

void CommandMap::AddSystemCommand(mstring service, mstring command,
	    mstring committees, functor function)
{
    FT("CommandMap::AddSystemCommand", (service, command, committees));

    i_system[service.LowerCase()].push_back(triplet<mstring, mstring, functor>
		    (command.UpperCase(),
		    (committees != "") ? committees.LowerCase() : mstring("all"),
		    function));
}


void CommandMap::RemSystemCommand(mstring service, mstring command,
	    mstring committees)
{
    FT("CommandMap::RemSystemCommand", (service, command, committees));

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

    i_user[service.LowerCase()].push_back(triplet<mstring, mstring, functor>
		    (command.UpperCase(),
		    (committees != "") ? committees.LowerCase() : mstring("all"),
		    function));
}


void CommandMap::RemCommand(mstring service, mstring command,
	    mstring committees)
{
    FT("CommandMap::RemCommand", (service, command, committees));

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
    int i;
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
    int i;
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

    if (DoUserCommand(mynick, user, command, params))
    {
	RET(true);
    }
    else if (DoSystemCommand(mynick, user, command, params))
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
