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


entlist_t::entlist_t(mstring entry, mstring nick)
{
    FT("entlist_t::entlist_t", (entry, nick));
    i_Entry = entry;
    i_Last_Modify_Time = Now();
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
    BaseTask.message_queue_.high_water_mark(Parent->high_water_mark*sizeof(ACE_Method_Object *));
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
    while(Parent->shutdown()==false)
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
	message_queue_.high_water_mark(Parent->high_water_mark*(thr_count()+1)*sizeof(ACE_Method_Object *));
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
	if (!IsChan(target))
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

		else if (Parent->nickserv.IsName(target))
		    Parent->nickserv.execute(message);

		else if (Parent->chanserv.IsName(target))
		    Parent->chanserv.execute(message);

		else if (Parent->memoserv.IsName(target))
		    Parent->memoserv.execute(message);

		else if (Parent->commserv.IsName(target))
		    Parent->commserv.execute(message);

		else if (Parent->servmsg.IsName(target))
		    Parent->servmsg.execute(message);

		// else check if it's script handled, might do up a list of script servers
		// in the magick object to check against, else trash it.

		else	// PRIVMSG or NOTICE to non-service
		    Parent->server.execute(message);
	    }
	    else
	    {
		// Check if we're to log ignore messages, and log them here.
		//if Parent->operserv.LogIgnored())
		//    wxLogInfo("IGNORED: ", message.c_str());
	    }
	}
	else	// Channel messages
	    Parent->server.execute(message);
    } 
    else	// Non PRIVMSG and NOTICE
	Parent->server.execute(message);

    if(thr_count()>1&&message_queue_.message_count()<
	    Parent->high_water_mark*(thr_count()-2)+Parent->low_water_mark)
    {
	message_queue_.high_water_mark(Parent->high_water_mark*(ACE_Thread_Manager::instance()->count_threads()-1)*sizeof(ACE_Method_Object *));
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
