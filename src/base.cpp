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
#include "log.h"


Magick *mBase::Parent;
bool mBase::TaskOpened;
mBaseTask mBase::BaseTask;

mBase::mBase(Magick* in_Parent)
{
    NFT("mBase::mBase");
    Parent=in_Parent;
    TaskOpened=false;
}

void mBase::push_message(const mstring& message)
{
    FT("mBase::push_message@mBase", (message));
    if(TaskOpened==false)
    {
	if(Parent!=NULL&&BaseTask.open(Parent)!=0)
	{
	    CP(("Failed to create initial thread"));
	    return;
	}
    }
    BaseTask.message(message);
}

void mBase::init(Magick *in)
{
    NFT("mBase::init");

    if(TaskOpened==false)
    {
	if(BaseTask.open((void *)in)!=0)
	{
	    CP(("Failed to create initial thread"));
	    return;
	}
    }
    BaseTask.message_queue_.high_water_mark(Parent->high_water_mark*sizeof(ACE_Method_Object *));
    BaseTask.message_queue_.low_water_mark(BaseTask.message_queue_.high_water_mark());
}

void mBase::send_cmd(const mstring & source, const mstring & fmt, ...)
{
    FT("send_cmd", (source, fmt));
    va_list args;
    va_start(args,fmt);
    //if(runflags & RUN_NOSEND)
	//return;
    mstring data1;
    data1.FormatV(fmt,args);
    va_end(args);

    if(source!="")
    {
    }
    else
    {
    }
}

void mBase::shutdown()
{
    int j=BaseTask.thr_count();
    for(int i=0;i<j;i++)
	BaseTask.shutdown();
}

NetworkServ::NetworkServ(Magick *in_Parent) : mBase(in_Parent)
{
    NFT("NetworkServ::NetworkServ");
    if (mThread::findbytype(Get_TType(), 1) == NULL) {
	messages=true;
	automation=true;
    }
}

void NetworkServ::execute(const mstring & data)
{
    FT("NetworkServ::execute", (data));
    //okay this is the main networkserv command switcher

    mstring source, msgtype, msgdata;
    if(data[0u]==':')
    {
	source=data.ExtractWord(1,": ");
	msgtype=data.ExtractWord(2,": ").UpperCase();
	msgdata=data.After(":",2);
    }
    else
    {
	source="";
	msgtype=data.ExtractWord(1,": ").UpperCase();
	msgdata=data.After(":");
    }
    if(msgtype=="PING")
    {
	if(msgdata!="")
	{
	    if(msgdata.WordCount(" ")>1)
		; //send_cmd(server_name,"PONG %s %s",msgdata.ExtractWord(2," "),msgdata.ExtractWord(1," "));
	    else
		; //send_cmd(server_name,"PONG %s %s",server_name,msgdata.ExtractWord(1," "));
	}
	else
	{
	    // error out?
	}
    }
	
}

int mBaseTask::open(void *in)
{
    Parent=(Magick *)in;
    return activate(/* todo enter the number of initial threads here*/);
}

int mBaseTask::svc(void)
{
    while(Parent->shutdown()==false)
    {
	auto_ptr<ACE_Method_Object> mo(this->activation_queue_.dequeue());
	if(mo->call()==-1)
	    break;
    }
    return 0;
}

class mBaseTaskmessage_MO : public ACE_Method_Object
{
public:
    mBaseTaskmessage_MO(mBaseTask *parent, const mstring& data)
    {
	i_parent=parent;
	i_data=data;
    }
    virtual int call()
    {
	i_parent->message_i(i_data);
	return 0;
    }
private:
    mBaseTask *i_parent;
    mstring i_data;
};

void mBaseTask::message(const mstring& message)
{
    if(message_queue_.is_full())
    {
	message_queue_.high_water_mark(Parent->high_water_mark*(thr_count()+1)*sizeof(ACE_Method_Object *));
	message_queue_.low_water_mark(message_queue_.high_water_mark());
	if(activate(THR_NEW_LWP | THR_JOINABLE, 1, 1)!=0)
	    CP(("Couldn't start new thread to handle excess load, will retry next message"));
    }
    activation_queue_.enqueue(new mBaseTaskmessage_MO(this,message));
}

void mBaseTask::message_i(const mstring& message)
{
    mstring tmp[2];
    if(message[0u]==':')
    {
	// with from prefix
	tmp[0]=message.ExtractWord(2,": ").UpperCase();
	tmp[1]=message.ExtractWord(3,": ");
    }
    else
    {
	// no from prefix
	tmp[0]=message.ExtractWord(1,": ").UpperCase();
	tmp[1]=message.ExtractWord(2,": ");
    }
	    
    // check if on ignore list and throw to the "ignore logging service" if log ignored user commands is on.
    // maybe we should have a hit count for logging? x ignores in x minutes = start logging that sucker. 

    if (tmp[0] == "PRIVMSG" || tmp[0] == "NOTICE") {
	mstring names;

    // Find out if the target nick is one of the services 'clones'
    // (and if it is, which one?)  Pass the message to them if so.
    // before even that, check if it's script overriden via Parent->checkifhandled(servername,command)
    // if so, Parent->doscripthandle(server,command,data);
//  if ((names=" "+OperServ::getnames()+" ").Find(" "+tmp[1]+" "))
//	    Parent->operserv.execute(message);
    if ((names=" "+Parent->nickserv.getnames()+" ").Find(" "+tmp[1]+" "))
	Parent->nickserv.execute(message);
    else if ((names=" "+Parent->chanserv.getnames()+" ").Find(" "+tmp[1]+" "))
	Parent->chanserv.execute(message);
//  else if ((names=" "+MemoServ::getnames()+" ").Find(" "+tmp[1]+" "))
//	Parent->memoserv.execute(message);
//  else if ((names=" "+HelpServ::getnames()+" ").Find(" "+tmp[1]+" "))
//	Parent->helpserv.execute(message);
    // else check if it's script handled, might do up a list of script servers
    // in the magick object to check against, else trash it.

    // Not a MSG/NOTICE to services, fall through
    // (it could be to a channel, or unrecognised nick).
    else
        Parent->server.execute (message);
    } 
    else 
    {
	// This handles all non-msgs/notices.
	Parent->server.execute (message);
    }

    /* Locking here because we dont want two threads
     * dieing because they both counted at the same
     * time and before the other had died.  Unlikely,
     * but it CAN happen.
     */
    {
	MLOCK("thread_handler", "LWM");
	if(message_queue_.message_count()<
	    Parent->high_water_mark*(thr_count()-2)+Parent->low_water_mark)
	{
	    message_queue_.high_water_mark(Parent->high_water_mark*(ACE_Thread_Manager::instance()->count_threads()-1)*sizeof(ACE_Method_Object *));
	    message_queue_.low_water_mark(message_queue_.high_water_mark());
	    COM(("Low water mark reached, killing thread."));
	    shutdown();
	}
    }
}

void mBaseTask::shutdown()
{
    activation_queue_.enqueue(new shutdown_MO);
}
