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

#include "lockable.h"
#include "nickserv.h"
#include "magick.h"
#include "cryptstream.h"

void Nick_Live_t::InFlight_t::ChgNick(mstring newnick)
{
    FT("Nick_Live_t::InFlight_t::ChgNick", (newnick));
    nick = newnick;
    if (timer)
    {
	mstring *arg;
	if (ACE_Reactor::instance()->cancel_timer(timer,
					    (const void **) arg))
	{
	    delete arg;
	}
	timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(nick.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
    }
}

void Nick_Live_t::InFlight_t::operator=(const InFlight_t &in)
{
    NFT("Nick_Live_t::InFlight_t::operator=");
    nick        = in.nick;
    memo	= in.memo;
    fileattach	= in.fileattach;
    fileinprog	= in.fileinprog;
    service	= in.service;
    sender	= in.sender;
    recipiant	= in.recipiant;
    text	= in.text;
}

    
// NICK has been destructed, or is going to be.
// Send any pending memos with End(0), but because
// it may have an 'in progress' memo at the time of
// destruction, we make sure the timer is cancelled.
Nick_Live_t::InFlight_t::~InFlight_t()
{
    NFT("Nick_Live_t::InFlight_t::~InFlight_t");
    if (Exists())
	End(0);
    mstring *arg;
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
						(const void **) arg))
	{
	    delete arg;
	}
}


// Initialise all veriables (done in Cancel() or End())
void Nick_Live_t::InFlight_t::init()
{
    NFT("Nick_Live_t::InFlight_t::init");
    memo = false;
    timer = 0u;
    fileattach = false;
    fileinprog = false;
    service = "";
    sender = "";
    recipiant = "";
    text = "";
}


// We have completed a file transfer, or errored out.
// 0 if we errored, else its a file number.
void Nick_Live_t::InFlight_t::File(unsigned long filenum)
{
    FT("Nick_Live_t::InFlight_t::File", (filenum));
    fileinprog = false;
    if (filenum)
	End(filenum);
    else
	Cancel();
}


// Ok, we've started a file transfer, cancel timer,
// now we wait for the File() command.
void Nick_Live_t::InFlight_t::SetInProg()
{
    NFT("Nick_Live_t::InFlight_t::SetInProg");
    fileinprog = true;
    mstring *arg;
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
						(const void **) arg))
	{
	    delete arg;
	}
}


// New memo, send an old one if it isnt in-progress, and
// cancel it if it was never started.
void Nick_Live_t::InFlight_t::Memo (bool file, mstring mynick,
				    mstring who, mstring message)
{
    FT("Nick_Live_t::InFlight_t::Memo", (file, mynick, who, message));
    if (File())
    {
	if (InProg())
	{
	    send(service, nick,
		"Cannot begin a new memo while a file is in progress.");
	    return;
	}
	else
	{
	    Cancel();
	    send(service, nick,
		"Previous pending file transfer ABORTED.");
	}
    }
    else if (Exists())
    {
	End(0);
    }

    if (!Parent->nickserv.IsStored(nick))
    {
	send(mynick, nick,
	    "You must register your nickname before you can send a memo.");
	return;
    }

    if (IsChan(who))
    {
	if (!Parent->chanserv.IsStored(who))
	{
	    send(mynick, nick,
		"Channel " + who + " is not registered, cannot send memo.");
	    return;
	}

	if (file)
	{
	    send(mynick, nick, "File attachments are not allowed to channels.");
	    return;
	}
    }
    else
    {
	if (!Parent->nickserv.IsStored(who))
	{
	    send(mynick, nick,
		"Nickname " + who + " is not registered, cannot send memo.");
	    return;
	}

	if (file && !Parent->memoserv.Files())
	{
	    send(mynick, nick, "File attachments in MEMOs have been disabled.");
	    return;
	}
    }


    memo = true;
    fileattach = file;
    service = mynick;
    sender = nick;
    recipiant = who;
    text = message;

    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
    if (fileattach)
	::send(mynick, source, "Memo is now pending ... You have " +
	    ToHumanTime(Parent->memoserv.InFlight()) +
	    " to begin your file transfer, continue or cancel it.");
    else
	::send(mynick, nick, "Memo is now pending ... You have " +
	    ToHumanTime(Parent->memoserv.InFlight()) +
	    " to continue or cancel it before it is delivered.");
}


// Add text to a memo, and re-start the timer.
void Nick_Live_t::InFlight_t::Continue(mstring message)
{
    FT("Nick_Live_t::InFlight_t::Continue", (message));
    if (memo)
  	text += message;
    mstring *arg;
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
					    (const void **) arg))
	{
	    delete arg;
	}
    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(nick.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
    ::send(mynick, source, "Pending memo timer reset ... You have " +
	    ToHumanTime(Parent->memoserv.InFlight()) +
	    " to continue or cancel it before it is delivered.");
}


// Cancel a memo or picture send.
void Nick_Live_t::InFlight_t::Cancel()
{
    NFT("Nick_Live_t::InFlight_t::Cancel");

    mstring *arg;
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
					    (const void **) arg))
	{
	    delete arg;
	}
    if (memo)
	send(service, nick, "Memo has been cancelled.");
    else
	send(service, nick, "Picture transfer has been cancelled.");
    init();
}


// This is the final destination of all memos.
// It will do nothing if a file is in-progress, and
// will call Cancel() if no file was started, but requested.
// It accepts an argument of 'file number'.  Ignored if
// no file attachment was requested, but set if 
void Nick_Live_t::InFlight_t::End(unsigned long filenum)
{
    NFT("Nick_Live_t::InFlight_t::End");
    if (File() && InProg())
    {
	// We do nothing ...
    }
    else if (File() && filenum == 0u)
    {
	Cancel();
    }
    else
    {
	mstring *arg;
	if (timer)
	    if (ACE_Reactor::instance()->cancel_timer(timer,
						(const void **) arg))
	    {
		delete arg;
	    }
	if (Parent->nickserv.IsStored(sender))
	{
	    if (Parent->nickserv.stored[sender.LowerCase()].Host() != "" &&
		Parent->nickserv.IsStored(Parent->nickserv.stored[sender.LowerCase()].Host()))
	    {
		sender = Parent->nickserv.stored[Parent->nickserv.stored[sender.LowerCase()].Host()].Name();
	    }
	    if (memo)
	    {
		if (IsChan(recipiant))
		{
		    if (Parent->chanserv.IsStored(recipiant))
		    {
			Parent->memoserv.channel[recipiant.LowerCase()].push_back(
			    News_t(recipiant, sender, text));
			send(service, nick, "Memo has been sent to " + recipiant + " (" +
			    Parent->chanserv.stored[recipiant.LowerCase()].Founder() + ").");
			if (Parent->chanserv.IsLive(recipiant))
			{
			    Chan_Live_t *chan = &Parent->chanserv.live[recipiant.LowerCase()];
			    unsigned int i;
			    for(i=0; i<chan->Users(); i++)
			    {
				if (Parent->chanserv.stored[recipiant.LowerCase()].GetAccess(chan->User(i), "READMEMO"))
				{
				    send(service, nick, "There is a new channel news article (#" +
					mstring(itoa(Parent->memoserv.channel[recipiant.LowerCase()].size())) + ").");
				}
			    }
			}
		    }
		}
		else
		{
		    if (Parent->nickserv.IsStored(recipiant))
		    {
			mstring realrecipiant = Parent->nickserv.stored[recipiant.LowerCase()].Host();
			if (realrecipiant == "")
			    realrecipiant = recipiant;
			if (recipiant == realrecipiant ||
			    Parent->nickserv.IsStored(realrecipiant))
			{
			    Parent->memoserv.nick[realrecipiant.LowerCase()].push_back(
				Memo_t(realrecipiant, sender, text, filenum));
			    send(service, nick, "Memo has been sent to " + recipiant + " (" + realrecipiant + ").");
			    if (Parent->nickserv.stored[realrecipiant.LowerCase()].IsOnline())
				send(service, nick, "You have a new memo from " + sender + " (#" +
					mstring(itoa(Parent->memoserv.nick[realrecipiant.LowerCase()].size())) + ").");
			}
			else if (File())
			{
			    // Delete file from filesystem
			}
		    }
		    else if (File())
		    {
			// Delete file from filesystem
		    }
		}
	    }
	    else
	    {
		if (Parent->nickserv.PicSize())
		{
		    Parent->nickserv.stored[sender.LowerCase()].GotPic(filenum);
		    send(service, nick, "Your picture has been saved.");
		}
		else
		{
		    // Delete file from filesystem
		}
	    }
	}
	init();
    }
}


void Nick_Live_t::InFlight_t::Picture(mstring mynick)
{
    FT("Nick_Live_t::InFlight_t::Picture", (mynick));
    if (File())
    {
	if (InProg())
	{
	    send(service, nick,
		"Cannot begin picture transfer while a file is in progress.");
	    return;
	}
	else
	{
	    Cancel();
	    send(service, nick,
		"Previous pending file transfer ABORTED.");
	}
    }
    else if (Exists())
    {
	End(0);
    }

    if (Parent->nickserv.IsStored(nick))
    {
	send(service, nick,
	    "Your nickname is not registered.");
	return;
    }
    else if (!Parent->nickserv.PicSize())
    {
	send(service, nick,
	    "Setting pictures has been disabled.");
    }

    memo = false;
    fileattach = true;
    sender = nick;
    service = mynick;
    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
    send(service, nick, "Please send your picture now.");
}


Nick_Live_t::Nick_Live_t()
{
    NFT("Nick_Live_t::Nick_Live_t");
    identified = false;
    InFlight.init();
}


Nick_Live_t::Nick_Live_t(mstring name, mDateTime signon, mstring server,
	    mstring username, mstring hostname, mstring realname)
{
    FT("Nick_Live_t::Nick_Live_t",(name, signon, server, username, hostname, realname));

    i_Name = name;
    i_Signon_Time = signon;
    i_My_Signon_Time = i_Last_Action = Now();
    i_server = server;
    i_user = username;
    i_host = hostname;
    i_realname = realname;
    identified = false;
    InFlight.nick=i_Name;
    InFlight.init();

    if (Parent->operserv.AddHost(i_host))
    {
	// Set off timer in 1s to do this:
	// Parent->server.KILL(Parent->nickserv.FirstName(), i_Name, "Too many connections from one host");

        ACE_Reactor::instance()->schedule_timer(&(Parent->nickserv.kosh),new mstring(i_Name + ":" + "Too many connections from your host"),ACE_Time_Value(1));

	return;
    }
    if (Parent->nickserv.IsStored(i_Name))
    {
	if (IsRecognized() && !Parent->nickserv.stored[i_Name.LowerCase()].Secure())
	    Parent->nickserv.stored[i_Name.LowerCase()].Signon(i_realname, Mask(U_P_H).After("!"));
    }
}


Nick_Live_t::Nick_Live_t(mstring name, mstring username, mstring hostname,
	    mstring realname)
{
    FT("Nick_Live_t::Nick_Live_t",(name, username, hostname, realname));
    i_Name = name;
    i_Signon_Time = i_My_Signon_Time = Now();
    i_Last_Action = time(NULL);
    i_user = username;
    i_host = hostname;
    i_realname = realname;
    identified = true;
    InFlight.nick = i_Name;
    InFlight.init();
}


void Nick_Live_t::operator=(const Nick_Live_t &in)
{
    NFT("Nick_Live_t::operator=");
    i_Name=in.i_Name;
    i_Signon_Time=in.i_Signon_Time;
    i_My_Signon_Time=in.i_My_Signon_Time;
    i_Last_Action=in.i_Last_Action;
    i_realname=in.i_realname;
    i_user=in.i_user;
    i_host=in.i_host;
    i_server=in.i_server;
    modes=in.modes;
    set<mstring>::const_iterator i;
    joined_channels.clear();
    for(i=in.joined_channels.begin();i!=in.joined_channels.end();i++)
	joined_channels.insert(*i);
    last_msg_times.empty();
    last_msg_times.reserve(in.last_msg_times.size());
    unsigned int k;
    for(k=0;k<in.last_msg_times.size();k++)
	last_msg_times.push_back(in.last_msg_times[k]);
    last_msg_entries=in.last_msg_entries;
    flood_triggered_times=in.flood_triggered_times;
    failed_passwds=in.failed_passwds;
    chans_founder_identd.clear();
    for(i=in.chans_founder_identd.begin();i!=in.chans_founder_identd.end();i++)
	chans_founder_identd.insert(*i);
    identified=in.identified;
    services=in.services;
    map<mstring, mstring>::const_iterator j;
    i_UserDef.clear();
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
    InFlight.nick = i_Name;
    InFlight.init();
    InFlight=in.InFlight;
    InFlight.ChgNick(i_Name);
}

void Nick_Live_t::Join(mstring chan)
{
    FT("Nick_Live_t::Join", (chan));
    if (Parent->chanserv.IsLive(chan))
    {
	Parent->chanserv.live[chan.LowerCase()].Join(i_Name);
    }
    else
    {
	Parent->chanserv.live[chan.LowerCase()] = Chan_Live_t(chan, i_Name);
    }

    joined_channels.insert(chan.LowerCase());
}


void Nick_Live_t::Part(mstring chan)
{
    FT("Nick_Live_t::Part", (chan));
    if (Parent->chanserv.IsLive(chan))
    {
	if (Parent->chanserv.live[chan.LowerCase()].Part(i_Name) == 0)
	    Parent->chanserv.live.erase(chan.LowerCase());
    }
    else
    {
	wxLogWarning("User %s PART from non-existant channel %s", i_Name.c_str(), chan.c_str());
    }

    joined_channels.erase(chan.LowerCase());
}

void Nick_Live_t::Kick(mstring kicker, mstring chan)
{
    FT("Nick_Live_t::Kick", (kicker, chan));
    if (Parent->chanserv.IsLive(chan))
    {
	if (Parent->chanserv.live[chan.LowerCase()].Kick(i_Name, kicker) == 0)
	    Parent->chanserv.live.erase(chan.LowerCase());
    }
    else
	wxLogWarning("User %s KICKED from non-existant channel %s by %s", i_Name.c_str(), chan.c_str(), kicker.c_str());

    joined_channels.erase(chan.LowerCase());
}


void Nick_Live_t::Quit(mstring reason)
{
    FT("Nick_Live_t::Quit", (reason));

    if (!IsServices())
	Parent->operserv.RemHost(i_host);

    // Check if we're currently being TEMP ignored ...
    {
    MLOCK(("OperServ","Ignore"));
    if (Parent->operserv.Ignore_find(Mask(N_U_P_H)))
    {
	if (Parent->operserv.Ignore->Value() != true)
	{
	    Parent->operserv.Ignore_erase();
	}
    }}

    while (joined_channels.size())
	Part(*joined_channels.begin());

    if (InFlight.Exists())
	InFlight.End(0);

    // We successfully ident to all channels we tried to
    // ident for before, so that they 0 our count -- we dont
    // want it carrying over to next time we log on.
    for (unsigned int i=0; i<try_chan_ident.size(); i++)
	if (Parent->chanserv.IsStored(try_chan_ident[i]))
	    Parent->chanserv.stored[try_chan_ident[i]].CheckPass(i_Name,
		Parent->chanserv.stored[try_chan_ident[i]].Password());

    if (Parent->nickserv.IsStored(i_Name) &&
	Parent->nickserv.stored[i_Name].IsOnline())
	Parent->nickserv.stored[i_Name.LowerCase()].Quit(reason);
}


bool Nick_Live_t::IsInChan(mstring chan)
{
    FT("Nick_Live_t::IsInChan", (chan));
    RET((joined_channels.find(chan.LowerCase()) != joined_channels.end()));
}


bool Nick_Live_t::FloodTrigger()
{
    NFT("Nick_Live_t::FloodTrigger");

    // We DONT ignore OPER's
    if (HasMode("o"))
    {
	RET(false);
    }

    // Check if we're currently being ignored ...
    if (Parent->operserv.Ignore_size())
    {
	RLOCK(("OperServ","Ignore"));
	if (Parent->operserv.Ignore_find(Mask(N_U_P_H)))
	{
	    // IF we havnt ignored for long enough yet, or its perminant ...
	    if (mDateTime(0,0,Parent->operserv.Ignore_Time(),0) > Now() - Parent->operserv.Ignore->Last_Modify_Time()
		|| Parent->operserv.Ignore->Value() == true)
	    {
		RET(true);
	    }
	    else
	    {
		MLOCK(("OperServ","Ignore"));
		Parent->operserv.Ignore_erase();
	    }
	}
    }

    // Clean up previous entries and push current entry
    while (last_msg_times.size() && last_msg_times[0u] < (time(NULL) - Parent->operserv.Flood_Time()))
	last_msg_times.erase(last_msg_times.begin());
    last_msg_times.push_back(time(NULL));

    // Check if we just triggered ignore.
    if (last_msg_times.size() > Parent->operserv.Flood_Msgs())
    {
	flood_triggered_times++;
	// Add To ignore, they're naughty.
	{
	WLOCK(("OperServ","Ignore"));
	mstring message;
	if (flood_triggered_times >= Parent->operserv.Ignore_Limit()) {
	    Parent->operserv.Ignore_insert(Mask(Parent->operserv.Ignore_Method()), true, i_Name);
	    message << "You have triggered services IGNORE (" << Parent->operserv.Flood_Msgs()
		<< " messages in " << Parent->operserv.Flood_Time() << " seconds).";
	    Parent->nickserv.send(i_Name, message); message = "";
	    message << "You have been ignored the maximum " << Parent->operserv.Ignore_Limit()
		<< " times.  Services will no longer respond.";
	    Parent->nickserv.send(i_Name, message); message = "";
	} else {
	    Parent->operserv.Ignore_insert(Mask(Parent->operserv.Ignore_Method()), false, i_Name);
	    message << "You have triggered services IGNORE (" << Parent->operserv.Flood_Msgs()
		<< " messages in " << Parent->operserv.Flood_Time() << " seconds).";
	    Parent->nickserv.send(i_Name, message); message = "";
	    message << "You have been ignored " << flood_triggered_times << " times (of a maximum "
		<< Parent->operserv.Ignore_Limit() << ").  Services will not respond for "
		<< Parent->operserv.Ignore_Time() << " seconds.";
	    Parent->nickserv.send(i_Name, message); message = "";
	}}
 
	RET(true);
    }

    RET(false);

}


void Nick_Live_t::Name(mstring in)
{
    FT("Nick_Live_t::Name", (in));

    set<mstring>::iterator iter;
    vector<mstring> chunked;
    unsigned int i;

    // Store what committee's we WERE on ...
    set<mstring> wason;
    map<mstring, Committee>::iterator iter2;
    for (iter2 = Parent->commserv.list.begin(); iter2 != Parent->commserv.list.end();
								iter2++)
	if (iter2->second.IsOn(i_Name))
	    wason.insert(iter2->first);

    // Rename ourselves in all channels ...
    for (iter=joined_channels.begin(); iter!=joined_channels.end(); iter++)
    {
	if (Parent->chanserv.IsLive(*iter))
	{
	    Parent->chanserv.live[*iter].ChgNick(i_Name, in);
	}
	else
	{
	    chunked.push_back(*iter);
	    wxLogWarning("USER %s changed nick, tried to rename in non-existant channel %s", i_Name.c_str(), iter->c_str());
	}
    }

    // Clean up non-existant channels ...
    for (i=0; i<chunked.size(); i++)
	joined_channels.erase(chunked[i]);

    if (Parent->nickserv.IsStored(i_Name))
    {
	if (!Parent->nickserv.stored[i_Name.LowerCase()].IsSibling(in))
	{
	    identified = false;
	    chans_founder_identd.clear();
	    failed_passwds = 0;
	}
	// Last Seen and Last Quit
	Parent->nickserv.stored[i_Name.LowerCase()].ChgNick(in);
    }

    // Carry over failed attempts (so /nick doesnt cure all!)
    for (i=0; i<try_chan_ident.size(); i++)
	if (Parent->chanserv.IsStored(try_chan_ident[i]))
	    Parent->chanserv.stored[try_chan_ident[i]].ChgAttempt(i_Name, in);

    // WooHoo, we have a new nick!
    i_Name = in;
    i_My_Signon_Time = Now();

    if (Parent->nickserv.IsStored(i_Name))
    {
	if (Parent->nickserv.stored[i_Name.LowerCase()].IsOnline())
	    Parent->nickserv.stored[i_Name.LowerCase()].Signon(i_realname, Mask(U_P_H).After("!"));
	else if (Parent->nickserv.stored[i_Name.LowerCase()].Protect())
	    Parent->nickserv.send(i_Name,
			"Please identify or you will be killed.");

    }

    // Send notices for committees we were NOT on
    for (iter2 = Parent->commserv.list.begin(); iter2 != Parent->commserv.list.end();
								iter2++)
    {
	if (iter2->second.IsOn(i_Name) && wason.find(iter2->first) == wason.end())
	{
	    for (iter2->second.message = iter2->second.MSG_begin();
		iter2->second.message != iter2->second.MSG_end(); iter2->second.message++)
	    {
		Parent->servmsg.send(i_Name, "[" + IRC_Bold + iter2->first + IRC_Off +
					    "] " + iter2->second.message->Entry());
	    }
	}
    }
}


void Nick_Live_t::SendMode(mstring in)
{
    FT("Nick_Live_t::SendMode", (in));

    if (IsServices())
    {
	Parent->server.MODE(i_Name, in);
    }
    else
    {
	Parent->server.SVSMODE(Parent->nickserv.FirstName(), i_Name, in);
    }
}


void Nick_Live_t::Mode(mstring in)
{
    FT("Nick_Live_t::Mode", (in));

    bool add = true;
    for (unsigned int i=0; i<in.size(); i++)
    {
	switch(in[i])
	{
	case '+':
	    add = true;
	    break;

	case '-':
	    add = false;
	    break;

	case 'o':
	    // We check the existing modes incase we get
	    // duplicate +o/-o (dont want to overhost it!)
	    if (add && !modes.Contains(in[i]) && !IsServices())
		Parent->operserv.RemHost(i_host);
	    else if (modes.Contains(in[i]) && !IsServices())
		Parent->operserv.AddHost(i_host);

	default:
	    if (add && !modes.Contains(in[i]))
	    {
		modes += in[i];
	    }
	    else if (!add && modes.Contains(in[i]))
	    {
		modes.Remove((mstring) in[i]);
	    }
	    else
	    {
		wxLogNotice("MODE change %c%c received for %s that is currently in effect",
			add ? '+' : '-', in[i], i_Name.c_str());
	    }
	    break;
	}
    }    
}


mDateTime Nick_Live_t::IdleTime()
{
    NFT("Nick_Live_t::IdleTime");
    mDateTime tmp;
    if (IsServices())
	tmp = time(NULL) - (time_t) i_Last_Action;
    RET(tmp);
}


void Nick_Live_t::Action()
{
    NFT("Nick_Live_t::Action");
    if (IsServices())
	i_Last_Action = Now();
}


void Nick_Live_t::SetSquit()
{
    NFT("Nick_Live_t::SetSquit");
    i_squit = i_server;

    if (!IsServices())
	Parent->operserv.RemHost(i_host);

    set<mstring>::iterator i;
    for (i=joined_channels.begin(); i!=joined_channels.end(); i++)
	if (Parent->chanserv.IsLive(*i))
	    Parent->chanserv.live[i->LowerCase()].Squit(i_Name);

}


void Nick_Live_t::ClearSquit()
{
    NFT("Nick_Live_t::ClearSquit");
    i_squit = "";

    if (!IsServices())
	Parent->operserv.AddHost(i_host);

    // These will all be set again
    modes = "";

    set<mstring>::iterator i;
    for (i=joined_channels.begin(); i!=joined_channels.end(); i++)
	if (Parent->chanserv.IsLive(*i))
	    Parent->chanserv.live[i->LowerCase()].UnSquit(i_Name);

    joined_channels.clear();
}


mstring Nick_Live_t::Mask(Nick_Live_t::styles type)
{
    FT("Nick_Live_t::Mask", ((int) type));

    mstring retval;
    switch (type)
    {
    case N:		// nick!*@*
	retval = i_Name + "!*@*";
	break;

    case N_U_P_H:	// nick!user@port.host
	retval = i_Name + "!" + i_user + "@" + i_host;
	break;

    case N_U_H:		// nick!user@*.host
	if (i_host.IsNumber())
	    retval = i_Name + "!" + i_user + "@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains("."))
	    retval = i_Name + "!" + i_user + "@*." + i_host.After(".");
	else
	    retval = i_Name + "!" + i_user + "@" + i_host;
	break;

    case N_P_H:		// nick!*@port.host
	retval = i_Name + "!*@" + i_host;
	break;

    case N_H:		// nick!*@*.host
	if (i_host.IsNumber())
	    retval = i_Name + "!*@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains("."))
	    retval = i_Name + "!*@*." + i_host.After(".");
	else
	    retval = i_Name + "!*@" + i_host;
	break;

    case U_P_H:		// *!user@port.host
	retval = "*!" + i_user + "@" + i_host;
	break;

    case U_H:		// *!user@*.host
	if (i_host.IsNumber())
	    retval = "*!" + i_user + "@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains("."))
	    retval = "*!" + i_user + "@*." + i_host.After(".");
	else
	    retval = "*!" + i_user + "@" + i_host;
	break;

    case P_H:		// *!*@port.host
	retval = "*!*@" + i_host;
	break;

    case H:		// *!*@*.host
	if (i_host.IsNumber())
	    retval = "*!*@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains("."))
	    retval = "*!*@*." + i_host.After(".");
	else
	    retval = "*!*@" + i_host;
	break;

    default:
	retval = "*!*@*";
	break;
    }
    
    RET(retval);
}


mstring Nick_Live_t::ChanIdentify(mstring channel, mstring password)
{
    FT("Nick_Live_t::ChanIdentify", (channel, password));
    if (Parent->chanserv.IsStored(channel))
    {
	unsigned int failtimes = Parent->chanserv.stored[channel.LowerCase()].CheckPass(i_Name, password);
	if (!failtimes)
	{
	    chans_founder_identd.insert(channel.LowerCase());
	    RET("Password accepted - you are now identified as founder for " + channel + ".");
	}
	else
	{
	    // Keep record of all channel attempts, so that when
	    // we change nick or quit, we can let chanserv know.
	    vector<mstring>::iterator iter;
	    for (iter=try_chan_ident.begin(); iter!=try_chan_ident.end(); iter++) ;
	    if (iter == try_chan_ident.end())
		try_chan_ident.push_back(channel.LowerCase());

	    if (failtimes > Parent->chanserv.Passfail())
	    {
		Parent->server.KILL(Parent->nickserv.FirstName(), i_Name,
			"Too many password failures");
		RET("");
	    }
	    else
	    {
		RET("Password incorrect for channel " + channel + ".");
	    }
	}
    }
    else
    {
	RET("Channel " + channel + " is not registered.");
    }
}


bool Nick_Live_t::IsChanIdentified(mstring channel)
{
    FT("Nick_Live_t::IsChanIdentified", (channel));
    RET((chans_founder_identd.find(channel.LowerCase())!=chans_founder_identd.end()));
}


mstring Nick_Live_t::Identify(mstring password)
{
    FT("Nick_Live_t::Identify", (password));
    if (identified == true)
    {
	RET("You're already identified.");
    }
    if (Parent->nickserv.IsStored(i_Name))
    {
	if (Parent->nickserv.stored[i_Name.LowerCase()].Password() == password)
	{
	    set<mstring> wason;
	    map<mstring, Committee>::iterator iter;
	    for (iter = Parent->commserv.list.begin(); iter != Parent->commserv.list.end();
									    iter++)
		if (iter->second.IsOn(i_Name))
		    wason.insert(iter->first);

	    identified = true;
	    failed_passwds = 0;
	    if (Parent->nickserv.stored[i_Name.LowerCase()].Secure())
		Parent->nickserv.stored[i_Name.LowerCase()].Signon(i_realname, Mask(U_P_H).After("!"));

	    // Send notices for committees we were NOT on
	    for (iter = Parent->commserv.list.begin(); iter != Parent->commserv.list.end();
									    iter++)
	    {
		if (iter->second.IsOn(i_Name) && wason.find(iter->first) == wason.end())
		{
		    for (iter->second.message = iter->second.MSG_begin();
			iter->second.message != iter->second.MSG_end(); iter->second.message++)
		    {
			Parent->servmsg.send(i_Name, "[" + IRC_Bold + iter->first +
					IRC_Off + "] " + iter->second.message->Entry());
		    }
		}
	    }
	    RET("Password accepted - you are now identified.");
	}
	else
	{
	    failed_passwds++;
	    if (failed_passwds > Parent->nickserv.Passfail())
	    {
		Parent->server.KILL(Parent->nickserv.FirstName(), i_Name,
			"Too many password failures");
		RET("");
	    }
	    else
	    {
		RET("Password incorrect.");
	    }
	}
    }
    else
    {
	RET("Your nickname is not registered.");
    }
}


bool Nick_Live_t::IsRecognized()
{
    NFT("Nick_Live_t::IsRecognised");
    if (Parent->nickserv.IsStored(i_Name))
    {
	RET(Parent->nickserv.stored[i_Name.LowerCase()].IsAccess(Mask(U_P_H).After("!")));
    }
    RET(false);
}


// =======================================================================


void Nick_Stored_t::Signon(mstring realname, mstring mask)
{
    FT("Nick_Stored_t::Signon", (realname, mask));
    i_LastRealName = realname;
    i_LastMask = mask;
}


void Nick_Stored_t::ChgNick(mstring nick)
{
    FT("Nick_Stored_t::ChgNick", (nick));
    i_LastQuit = "NICK CHANGE -> " + nick;
    i_LastSeenTime = Now();
}


Nick_Stored_t::Nick_Stored_t()
{
    NFT("Nick_Stored_t::Nick_Stored_t");
}


Nick_Stored_t::Nick_Stored_t(mstring nick, mstring password)
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick, password));
    i_Name = nick;
    i_Password = password;
    i_RegTime = Now();
    i_Protect = Parent->nickserv.DEF_Protect();
    l_Protect = false;
    i_Secure = Parent->nickserv.DEF_Secure();
    l_Secure = false;
    i_NoExpire = Parent->nickserv.DEF_NoExpire();
    l_NoExpire = false;
    i_NoMemo = Parent->nickserv.DEF_NoMemo();
    l_NoMemo = false;
    i_Private = Parent->nickserv.DEF_Private();
    l_Private = false;
    i_PRIVMSG = Parent->nickserv.DEF_PRIVMSG();
    l_PRIVMSG = false;
    i_Language = Parent->nickserv.DEF_Language().UpperCase();
    l_Language = false;
    i_Forbidden = false;
    i_Picture = 0;

    if (Parent->nickserv.IsLive(i_Name))
    {
	i_LastRealName = Parent->nickserv.live[i_Name.LowerCase()].RealName();
	i_LastMask = Parent->nickserv.live[i_Name.LowerCase()].Mask(Nick_Live_t::U_P_H).After("!");
        Parent->nickserv.live[i_Name.LowerCase()].Identify(Password());
    }
}


Nick_Stored_t::Nick_Stored_t(mstring nick)
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick));
    i_Name = nick;
    i_Forbidden = true;
    i_RegTime = Now();
} 


Nick_Stored_t::Nick_Stored_t(mstring nick, mDateTime regtime, const Nick_Stored_t &in)
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick, "(const Nick_Stored_t &) in"));
    i_Name = nick;
    i_RegTime = regtime;
    i_Forbidden = false;
    i_Host = in.i_Name.LowerCase();

    if (Parent->nickserv.IsLive(i_Name))
    {
	i_LastRealName = Parent->nickserv.live[i_Name.LowerCase()].RealName();
	i_LastMask = Parent->nickserv.live[i_Name.LowerCase()].Mask(Nick_Live_t::U_P_H).After("!");
        Parent->nickserv.live[i_Name.LowerCase()].Identify(Password());
    }
}


void Nick_Stored_t::Drop()
{
    NFT("Nick_Stored_t::Drop");

    // When we go, we take all our slaves with us!
    unsigned int i;
    if (i_Host == "")
    {
	for (i=0; i<Siblings(); i++)
	{
	    Parent->nickserv.stored.erase(Sibling(i));
	}
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].i_slaves.erase(i_Name.LowerCase());
    }

    // Now we go for our channels ...
    vector<mstring> killchans;
    map<mstring, Chan_Stored_t>::iterator iter;
    for (iter = Parent->chanserv.stored.begin();
	    iter != Parent->chanserv.stored.end(); iter++)
    {
	if (iter->second.Founder() == i_Name)
	{
	    if (iter->second.CoFounder() != "" &&
		Parent->nickserv.IsStored(iter->second.CoFounder()))
	    {
		iter->second.Founder(iter->second.CoFounder());
	    }
	    else
		killchans.push_back(iter->first);
	}
	else
	    killchans.push_back(iter->first);
    }

    for (i=0; i<killchans.size(); i++)
    {
	Parent->chanserv.stored.erase(killchans[i]);
    }
}


void Nick_Stored_t::operator=(const Nick_Stored_t &in)
{
    NFT("Nick_Stored_t::operator=");
    i_Name=in.i_Name;
    i_RegTime=in.i_RegTime;
    i_Password=in.i_Password;
    i_Email=i_Email;
    i_URL=in.i_URL;
    i_ICQ=in.i_ICQ;
    i_Description=in.i_Description;
    i_Comment=in.i_Comment;
    i_Host=in.i_Host;
    set<mstring>::const_iterator i;
    i_slaves.clear();
    for(i=in.i_slaves.begin();i!=in.i_slaves.end();i++)
	i_slaves.insert(*i);
    i_access.clear();
    for(i=in.i_access.begin();i!=in.i_access.end();i++)
	i_access.insert(*i);
    i_ignore.clear();
    for(i=in.i_ignore.begin();i!=in.i_ignore.end();i++)
	i_ignore.insert(*i);
    i_Protect=in.i_Protect;
    l_Protect=in.l_Protect;
    i_Secure=in.i_Secure;
    l_Secure=in.l_Secure;
    i_NoExpire=in.i_NoExpire;
    l_NoExpire=in.l_NoExpire;
    i_NoMemo=in.i_NoMemo;
    l_NoMemo=in.l_NoMemo;
    i_Private=in.i_Private;
    l_Private=in.l_Private;
    i_PRIVMSG=in.i_PRIVMSG;
    l_PRIVMSG=in.l_PRIVMSG;
    i_Language=in.i_Language;
    l_Language=in.l_Language;
    i_Forbidden=in.i_Forbidden;
    i_Picture=in.i_Picture;
    i_Suspend_By=in.i_Suspend_By;
    i_Suspend_Time=in.i_Suspend_Time;

    // non-slave & offline
    i_LastSeenTime=in.i_LastSeenTime;
    i_LastRealName=in.i_LastRealName;
    i_LastMask=in.i_LastMask;
    i_LastQuit=in.i_LastQuit;
    map<mstring, mstring>::const_iterator j;
    i_UserDef.clear();
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
}


mstring Nick_Stored_t::Email()
{
    NFT("Nick_Stored_t::Email");
    if (i_Host == "")
    {
	RET(i_Email);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Email());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Email());
    }
}


void Nick_Stored_t::Email(mstring in)
{
    FT("Nick_Stored_t::Email", (in));
    if (i_Host == "")
    {
	i_Email = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Email(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	Email(in);
    }
}


mstring Nick_Stored_t::URL()
{
    NFT("Nick_Stored_t::URL");
    if (i_Host == "")
    {
	RET(i_URL);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].URL());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(URL());
    }
}


void Nick_Stored_t::URL(mstring in)
{
    FT("Nick_Stored_t::URL", (in));
    if (i_Host == "")
    {
	i_URL = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].URL(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	URL(in);
    }
}


mstring Nick_Stored_t::ICQ()
{
    NFT("Nick_Stored_t::ICQ");
    if (i_Host == "")
    {
	RET(i_ICQ);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].ICQ());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(ICQ());
    }
}


void Nick_Stored_t::ICQ(mstring in)
{
    FT("Nick_Stored_t::ICQ", (in));
    if (i_Host == "")
    {
	i_ICQ = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].ICQ(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	ICQ(in);
    }
}


mstring Nick_Stored_t::Description()
{
    NFT("Nick_Stored_t::Description");
    if (i_Host == "")
    {
	RET(i_Description);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Description());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Description());
    }
}


void Nick_Stored_t::Description(mstring in)
{
    FT("Nick_Stored_t::Description", (in));
    if (i_Host == "")
    {
	i_Description = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Description();
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	Description(in);
    }
}


mstring Nick_Stored_t::Comment()
{
    NFT("Nick_Stored_t::Comment");
    if (i_Host == "")
    {
	RET(i_Comment);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Comment());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Comment());
    }
}


void Nick_Stored_t::Comment(mstring in)
{
    FT("Nick_Stored_t::Comment", (in));
    if (i_Host == "")
    {
	i_Comment = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Comment(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	Comment(in);
    }
}


void Nick_Stored_t::Suspend(mstring name)
{
    FT("Nick_Stored_t::Suspend", (name));
    if (i_Host == "")
    {
	i_Suspend_By = name;
	i_Suspend_Time = Now();
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Suspend(name);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	Suspend(name);
    }
}


void Nick_Stored_t::UnSuspend()
{
    NFT("Nick_Stored_t::UnSuspend");
    if (i_Host == "")
    {
	i_Suspend_By = "";
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].UnSuspend();
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	UnSuspend();
    }
}


mstring Nick_Stored_t::Password()
{
    NFT("Nick_Stored_t::Password");
    if (i_Host == "")
    {
	RET(i_Password);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Password());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Password());
    }
}


void Nick_Stored_t::Password(mstring in)
{
    FT("Nick_Stored_t::Password", (in));
    if (i_Host == "")
    {
	i_Password = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Password(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	Password(in);
    }
}


bool Nick_Stored_t::Slave(mstring nick, mstring password, mDateTime regtime)
{
    FT("Nick_Stored_t::Slave", (nick, password, regtime));

    if (i_Host == "")
    {
	CP(("Checking \"%s\" == \"%s\" ...", password.c_str(), i_Password.c_str()));
	if (i_Forbidden  || password != i_Password)
	{
	    RET(false);
	}

	if (Parent->nickserv.IsStored(nick))
	{
	    ChangeOver(nick);
	    Parent->nickserv.stored.erase(nick.LowerCase());
	}

	Parent->nickserv.stored[nick.LowerCase()] = Nick_Stored_t(nick, regtime, *this);
	i_slaves.insert(nick.LowerCase());
	RET(true);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Slave(nick, password, regtime));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Slave(nick, password, regtime));
    }
}


unsigned int Nick_Stored_t::Siblings()
{
    NFT("Nick_Stored_t::Siblings");
    if (i_Host == "")
    {
	RET(i_slaves.size());
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Siblings());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Siblings());
    }
}


mstring Nick_Stored_t::Sibling(unsigned int count)
{
    FT("Nick_Stored_t::Siblings", (count));
    if (i_Host == "")
    {
	set<mstring>::iterator iter;
	unsigned int i;
	for (i=0, iter=i_slaves.begin(); iter!=i_slaves.end(); iter++, i++)
	{
	    if (i==count)
	    {
		RET(iter->LowerCase());
	    }
	}
	RET("");
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Sibling(count));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Sibling(count));
    }
}


bool Nick_Stored_t::IsSibling(mstring nick)
{
    FT("Nick_Stored_t::IsSibling", (nick));
    if (i_Name.LowerCase() == nick.LowerCase())
    {
	RET(true);
    }
    if (i_Host == "")
    {
	RET(i_slaves.find(nick.LowerCase()) != i_slaves.end());
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].IsSibling(nick));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(IsSibling(nick));
    }
}

void Nick_Stored_t::ChangeOver(mstring oldnick)
{
    FT("Nick_Stored_t::ChangeOver", (oldnick)); 

    bool found;
    mstring modifier, valueS;
    long valueL;
    mDateTime modtime;

    map<mstring, Committee>::iterator citer;
    for (citer = Parent->commserv.list.begin();
			    citer != Parent->commserv.list.end(); citer++)
    {
	MLOCK(("CommServ", "list", citer->first, "member"));
	found = false;
	if (citer->second.Name() != Parent->commserv.ALL_Name() &&
	    citer->second.Name() != Parent->commserv.REGD_Name() &&
	    citer->second.Name() != Parent->commserv.SADMIN_Name())
	{
	    if (citer->second.find(i_Name))
	    {
		modifier = citer->second.member->Last_Modifier();
		modtime = citer->second.member->Last_Modify_Time();
		citer->second.erase();
		found = true;
	    }
	    if (citer->second.find(oldnick))
	    {
		modifier = citer->second.member->Last_Modifier();
		modtime = citer->second.member->Last_Modify_Time();
		citer->second.erase();
		found = true;
	    }
	    if (citer->second.IsHead(i_Name) || citer->second.IsHead(oldnick))
		found = false;
	    if (found)
	    {
		citer->second.insert(i_Name, modifier, modtime);
		citer->second.member = citer->second.end();
	    }
	}
    }

    map<mstring, Chan_Stored_t>::iterator csiter;
    for (csiter = Parent->chanserv.stored.begin();
			csiter != Parent->chanserv.stored.end(); csiter++)
    {
	if (csiter->second.Founder().LowerCase() == oldnick.LowerCase())
	{
	    csiter->second.Founder(i_Name);
	}
	if (csiter->second.CoFounder().LowerCase() == oldnick.LowerCase())
	{
	    csiter->second.CoFounder(i_Name);
	}
	{ MLOCK(("ChanServ", "stored", csiter->first, "Access"));
	found = false;
	if (csiter->second.Access_find(i_Name))
	{
	    valueL = csiter->second.Access->Value();
	    modifier = csiter->second.Access->Last_Modifier();
	    modtime = csiter->second.Access->Last_Modify_Time();
	    csiter->second.Access_erase();
	    found = true;
	}
	if (csiter->second.Access_find(oldnick))
	{
	    valueL = csiter->second.Access->Value();
	    modifier = csiter->second.Access->Last_Modifier();
	    modtime = csiter->second.Access->Last_Modify_Time();
	    csiter->second.Access_erase();
	    found = true;
	}
	if (found)
	{
	    csiter->second.Access_insert(i_Name, valueL, modifier, modtime);
	    csiter->second.Access = csiter->second.Access_end();
	} }
	{ MLOCK(("ChanServ", "stored", csiter->first, "Akick"));
	found = false;
	if (csiter->second.Akick_find(i_Name))
	{
	    valueS = csiter->second.Akick->Value();
	    modifier = csiter->second.Akick->Last_Modifier();
	    modtime = csiter->second.Akick->Last_Modify_Time();
	    csiter->second.Akick_erase();
	    found = true;
	}
	if (csiter->second.Akick_find(oldnick))
	{
	    valueS = csiter->second.Akick->Value();
	    modifier = csiter->second.Akick->Last_Modifier();
	    modtime = csiter->second.Akick->Last_Modify_Time();
	    csiter->second.Akick_erase();
	    found = true;
	}
	if (found)
	{
	    csiter->second.Akick_insert(i_Name, valueS, modifier, modtime);
	    csiter->second.Akick = csiter->second.Akick_end();
	} }
	{ MLOCK(("ChanServ", "stored", csiter->first, "Greet"));
	found = false;
	if (csiter->second.Greet_find(i_Name))
	{
	    valueS = csiter->second.Greet->Entry();
	    modtime = csiter->second.Greet->Last_Modify_Time();
	    csiter->second.Greet_erase();
	    found = true;
	}
	if (csiter->second.Greet_find(oldnick))
	{
	    valueS = csiter->second.Greet->Entry();
	    modtime = csiter->second.Greet->Last_Modify_Time();
	    csiter->second.Greet_erase();
	    found = true;
	}
	if (found)
	{
	    csiter->second.Greet_insert(valueS, i_Name, modtime);
	    csiter->second.Greet = csiter->second.Greet_end();
	} }
    }

    
    map<mstring, Nick_Stored_t>::iterator niter;
    for (niter = Parent->nickserv.stored.begin();
			niter != Parent->nickserv.stored.end(); niter++)
    {
	if (niter->second.IsIgnore(oldnick))
	{
	    niter->second.IgnoreDel(oldnick);
	    niter->second.IgnoreAdd(i_Name);
	}
    }

    map<mstring, list<News_t> >::iterator cniter;
    list<News_t>::iterator cnliter;
    for (cniter = Parent->memoserv.channel.begin();
			cniter != Parent->memoserv.channel.end(); cniter++)
    {
	for (cnliter = cniter->second.begin();
			    cnliter != cniter->second.end(); cnliter++)
	{
	    if (cnliter->IsRead(oldnick))
	    {
		cnliter->Unread(oldnick);
		cnliter->Read(i_Name);
	    }
	}
    }
}

bool Nick_Stored_t::MakeHost()
{
    NFT("Nick_Stored_t::MakeHost"); 
    if (i_Host == "")
    {
	RET(false);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	// Re-point all slaves to me and copy the slaves list.
	// Then clear the host's slave list, point host to me,
	// and finally set my host pointer to "".
	for (unsigned int i=0; i<Parent->nickserv.stored[i_Host.LowerCase()].Siblings(); i++)
	{
	    if (Parent->nickserv.IsStored(Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i)))
	    {
		Parent->nickserv.stored[Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i)].i_Host = i_Name;
		i_slaves.insert(Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i));
	    }
	    else
	    {
		wxLogWarning("Nick %s was listed as slave of %s, but does not exist!!",
			Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i).c_str(), i_Name.c_str());
	    }
	}
	i_Password = Parent->nickserv.stored[i_Host.LowerCase()].i_Password;
	i_Email = Parent->nickserv.stored[i_Host.LowerCase()].i_Email;
	i_URL = Parent->nickserv.stored[i_Host.LowerCase()].i_URL;
	i_ICQ = Parent->nickserv.stored[i_Host.LowerCase()].i_ICQ;
	i_Description = Parent->nickserv.stored[i_Host.LowerCase()].i_Description;
	i_Comment = Parent->nickserv.stored[i_Host.LowerCase()].i_Comment;
	i_access = Parent->nickserv.stored[i_Host.LowerCase()].i_access;
	i_ignore = Parent->nickserv.stored[i_Host.LowerCase()].i_ignore;
	i_Protect = Parent->nickserv.stored[i_Host.LowerCase()].i_Protect;
	l_Protect = Parent->nickserv.stored[i_Host.LowerCase()].l_Protect;
	i_Secure = Parent->nickserv.stored[i_Host.LowerCase()].i_Secure;
	l_Secure = Parent->nickserv.stored[i_Host.LowerCase()].l_Secure;
	i_NoExpire = Parent->nickserv.stored[i_Host.LowerCase()].i_NoExpire;
	l_NoExpire = Parent->nickserv.stored[i_Host.LowerCase()].l_NoExpire;
	i_NoMemo = Parent->nickserv.stored[i_Host.LowerCase()].i_NoMemo;
	l_NoMemo = Parent->nickserv.stored[i_Host.LowerCase()].l_NoMemo;
	i_Private = Parent->nickserv.stored[i_Host.LowerCase()].i_Private;
	l_Private = Parent->nickserv.stored[i_Host.LowerCase()].l_Private;
	i_PRIVMSG = Parent->nickserv.stored[i_Host.LowerCase()].i_PRIVMSG;
	l_PRIVMSG = Parent->nickserv.stored[i_Host.LowerCase()].l_PRIVMSG;
	i_Language = Parent->nickserv.stored[i_Host.LowerCase()].i_Language;
	l_Language = Parent->nickserv.stored[i_Host.LowerCase()].l_Language;
	i_Picture = Parent->nickserv.stored[i_Host.LowerCase()].i_Picture;
	i_Suspend_By = Parent->nickserv.stored[i_Host.LowerCase()].i_Suspend_By;
	i_Suspend_Time = Parent->nickserv.stored[i_Host.LowerCase()].i_Suspend_Time;
	Parent->nickserv.stored[i_Host.LowerCase()].i_slaves.clear();
	Parent->nickserv.stored[i_Host.LowerCase()].i_Host = i_Name;
	ChangeOver(i_Host);
	i_Host = "";
	RET(true);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(MakeHost());
    }
}


bool Nick_Stored_t::Unlink()
{
    NFT("Nick_Stored_t::Unlink");
    if (i_Host == "")
    {
	RET(false)
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	i_slaves.clear();
	i_Password = Parent->nickserv.stored[i_Host.LowerCase()].i_Password;
	i_Email = Parent->nickserv.stored[i_Host.LowerCase()].i_Email;
	i_URL = Parent->nickserv.stored[i_Host.LowerCase()].i_URL;
	i_ICQ = Parent->nickserv.stored[i_Host.LowerCase()].i_ICQ;
	i_Description = Parent->nickserv.stored[i_Host.LowerCase()].i_Description;
	i_Comment = Parent->nickserv.stored[i_Host.LowerCase()].i_Comment;
	i_access = Parent->nickserv.stored[i_Host.LowerCase()].i_access;
	i_ignore = Parent->nickserv.stored[i_Host.LowerCase()].i_ignore;
	i_Protect = Parent->nickserv.stored[i_Host.LowerCase()].i_Protect;
	l_Protect = Parent->nickserv.stored[i_Host.LowerCase()].l_Protect;
	i_Secure = Parent->nickserv.stored[i_Host.LowerCase()].i_Secure;
	l_Secure = Parent->nickserv.stored[i_Host.LowerCase()].l_Secure;
	i_NoExpire = Parent->nickserv.stored[i_Host.LowerCase()].i_NoExpire;
	l_NoExpire = Parent->nickserv.stored[i_Host.LowerCase()].l_NoExpire;
	i_NoMemo = Parent->nickserv.stored[i_Host.LowerCase()].i_NoMemo;
	l_NoMemo = Parent->nickserv.stored[i_Host.LowerCase()].l_NoMemo;
	i_Private = Parent->nickserv.stored[i_Host.LowerCase()].i_Private;
	l_Private = Parent->nickserv.stored[i_Host.LowerCase()].l_Private;
	i_PRIVMSG = Parent->nickserv.stored[i_Host.LowerCase()].i_PRIVMSG;
	l_PRIVMSG = Parent->nickserv.stored[i_Host.LowerCase()].l_PRIVMSG;
	i_Language = Parent->nickserv.stored[i_Host.LowerCase()].i_Language;
	l_Language = Parent->nickserv.stored[i_Host.LowerCase()].l_Language;
	i_Picture = Parent->nickserv.stored[i_Host.LowerCase()].i_Picture;
	i_Suspend_By = Parent->nickserv.stored[i_Host.LowerCase()].i_Suspend_By;
	i_Suspend_Time = Parent->nickserv.stored[i_Host.LowerCase()].i_Suspend_Time;
	Parent->nickserv.stored[i_Host.LowerCase()].i_slaves.erase(i_Name.LowerCase());
	i_Host = "";
	RET(true);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Unlink());
    }
}


unsigned int Nick_Stored_t::Access()
{
    NFT("Nick_Stored_t::Access");
    if (i_Host == "")
    {
	RET(i_access.size());
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Access());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Access());
    }
}


mstring Nick_Stored_t::Access(unsigned int count)
{
    FT("Nick_Stored_t::Access", (count));
    if (i_Host == "")
    {
	set<mstring>::iterator iter;
	unsigned int i;
	for (i=0, iter=i_access.begin(); iter!=i_access.end(); i++, iter++)
	    if (i==count)
	    {
		RET(*iter);
	    }
	RET("");
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Access(count));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Access(count));
    }
}


bool Nick_Stored_t::AccessAdd(const mstring& in)
{
    FT("Nick_Stored_t::AccessAdd", (in));
    if (i_Host == "")
    {
	// Incorrect syntax
	if (!(in.Contains("@") && in.WordCount("@") == 2))
	{
	    RET(false);
	}
	
	// Already exists (or inclusive)
	set<mstring>::iterator iter;
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		RET(false);
	    }

	// NEW one includes existing ones ...
	// Remove all obsolite entries.
	vector<mstring> chunked;
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (iter->Matches(in.LowerCase()))
	    {
		chunked.push_back(*iter);
	    }
	for (unsigned int i=0; i<chunked.size(); i++)
	    i_access.erase(chunked[i]);

	i_access.insert(in.LowerCase());
	RET(true);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].AccessAdd(in));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(AccessAdd(in));
    }
}


unsigned int Nick_Stored_t::AccessDel(mstring in)
{
    FT("Nick_Stored_t::AccessDel", (in));
    if (i_Host == "")
    {
	vector<mstring> chunked;
	set<mstring>::iterator iter;
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		chunked.push_back(*iter);
	    }

	for (unsigned int i=0; i<chunked.size(); i++)
	    i_access.erase(chunked[i].LowerCase());
	RET(chunked.size());
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].AccessDel(in));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(AccessDel(in));
    }
}


bool Nick_Stored_t::IsAccess(mstring in)
{
    FT("Nick_Stored_t::IsAccess", (in));
    if (i_Host == "")
    {
	set<mstring>::iterator iter;
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		RET(true);
	    }
	RET(false);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].IsAccess(in));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(IsAccess(in));
    }
}


unsigned int Nick_Stored_t::Ignore()
{
    NFT("Nick_Stored_t::Ignore");
    if (i_Host == "")
    {
	RET(i_ignore.size());
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Ignore());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Ignore());
    }
}


mstring Nick_Stored_t::Ignore(unsigned int count)
{
    FT("Nick_Stored_t::Ignore", (count));
    if (i_Host == "")
    {
	set<mstring>::iterator iter;
	unsigned int i;
	for (i=0, iter=i_ignore.begin(); iter!=i_ignore.end(); i++, iter++)
	    if (i==count)
	    {
		RET(*iter);
	    }
	RET("");
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Ignore(count));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Ignore(count));
    }
}


bool Nick_Stored_t::IgnoreAdd(mstring in)
{
    FT("Nick_Stored_t::IgnoreAdd", (in));
    if (i_Host == "")
    {
	// Not stored nick
	if (!Parent->nickserv.IsStored(in))
	{
	    RET(false);
	}
	
	if (i_ignore.find(in.LowerCase())!=i_ignore.end())
	{
	    RET(false);
	}

	i_ignore.insert(in.LowerCase());
	RET(true);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].IgnoreAdd(in));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(IgnoreAdd(in));
    }
}


unsigned int Nick_Stored_t::IgnoreDel(mstring in)
{
    FT("Nick_Stored_t::IgnoreDel", (in));
    if (i_Host == "")
    {
	vector<mstring> chunked;
	set<mstring>::iterator iter;
	for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		chunked.push_back(*iter);
	    }

	for (unsigned int i=0; i<chunked.size(); i++)
	    i_ignore.erase(chunked[i].LowerCase());
	RET(chunked.size());
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].IgnoreDel(in));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(IgnoreDel(in));
    }
}


bool Nick_Stored_t::IsIgnore(mstring in)
{
    FT("Nick_Stored_t::IsIgnore", (in));
    if (i_Host == "")
    {
	set<mstring>::iterator iter;
	for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		RET(true);
	    }
	RET(false);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].IsIgnore(in));
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(IsIgnore(in));
    }
}


bool Nick_Stored_t::Protect()
{
    NFT("Nick_Stored_t::Protect");
    if (Parent->nickserv.LCK_Protect())
    {
	RET(Parent->nickserv.DEF_Protect());
    }
    else if (i_Host == "")
    {
	RET(i_Protect);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Protect());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Protect());
    }
}


void Nick_Stored_t::Protect(bool in)
{
    FT("Nick_Stored_t::Protect", (in));
    if (i_Host == "")
    {
	if (!L_Protect())
	    i_Protect = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Protect(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	Protect(in);
    }
}


bool Nick_Stored_t::L_Protect()
{
    NFT("Nick_Stored_t::L_Protect");
    if (Parent->nickserv.LCK_Protect())
    {
	RET(true);
    }
    else if (i_Host == "")
    {
	RET(l_Protect);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_Protect());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(L_Protect());
    }
}


void Nick_Stored_t::L_Protect(bool in)
{
    FT("Nick_Stored_t::L_Protect", (in));
    if (i_Host == "")
    {
	if (!Parent->nickserv.LCK_Protect())
	    l_Protect = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_Protect(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	L_Protect(in);
    }
}


bool Nick_Stored_t::Secure()
{
    NFT("Nick_Stored_t::Secure");
    if (Parent->nickserv.LCK_Secure())
    {
	RET(Parent->nickserv.DEF_Secure());
    }
    if (i_Host == "")
    {
	RET(i_Secure);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Secure());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Secure());
    }
}


void Nick_Stored_t::Secure(bool in)
{
    FT("Nick_Stored_t::Secure", (in));
    if (i_Host == "")
    {
	if (!L_Secure())
	    i_Secure = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Secure(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	Secure(in);
    }
}


bool Nick_Stored_t::L_Secure()
{
    NFT("Nick_Stored_t::L_Secure");
    if (Parent->nickserv.LCK_Secure())
    {
	RET(true);
    }
    if (i_Host == "")
    {
	RET(l_Secure);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_Secure());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(L_Secure());
    }
}


void Nick_Stored_t::L_Secure(bool in)
{
    FT("Nick_Stored_t::L_Secure", (in));
    if (i_Host == "")
    {
	if (!Parent->nickserv.LCK_Secure())
	    l_Secure = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_Secure(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	L_Secure(in);
    }
}


bool Nick_Stored_t::NoExpire()
{
    NFT("Nick_Stored_t::NoExpire");
    if (Parent->nickserv.LCK_NoExpire())
    {
	RET(Parent->nickserv.DEF_NoExpire());
    }
    if (i_Host == "")
    {
	RET(i_NoExpire);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].NoExpire());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(NoExpire());
    }
}


void Nick_Stored_t::NoExpire(bool in)
{
    FT("Nick_Stored_t::NoExpire", (in));
    if (i_Host == "")
    {
	if (!L_NoExpire())
	    i_NoExpire = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].NoExpire(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	NoExpire(in);
    }
}


bool Nick_Stored_t::L_NoExpire()
{
    NFT("Nick_Stored_t::L_NoExpire");
    if (Parent->nickserv.LCK_NoExpire())
    {
	RET(true);
    }
    if (i_Host == "")
    {
	RET(l_NoExpire);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_NoExpire());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(L_NoExpire());
    }
}


void Nick_Stored_t::L_NoExpire(bool in)
{
    FT("Nick_Stored_t::L_NoExpire", (in));
    if (i_Host == "")
    {
	if (!Parent->nickserv.LCK_NoExpire())
	    l_NoExpire = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_NoExpire(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	L_NoExpire(in);
    }
}


bool Nick_Stored_t::NoMemo()
{
    NFT("Nick_Stored_t::NoMemo");
    if (Parent->nickserv.LCK_NoMemo())
    {
	RET(Parent->nickserv.DEF_NoMemo());
    }
    if (i_Host == "")
    {
	RET(i_NoMemo);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].NoMemo());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(NoMemo());
    }
}


void Nick_Stored_t::NoMemo(bool in)
{
    FT("Nick_Stored_t::NoMemo", (in));
    if (i_Host == "")
    {
	if (!L_NoMemo())
	    i_NoMemo = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].NoMemo(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	NoMemo(in);
    }
}


bool Nick_Stored_t::L_NoMemo()
{
    NFT("Nick_Stored_t::L_NoMemo");
    if (Parent->nickserv.LCK_NoMemo())
    {
	RET(true);
    }
    if (i_Host == "")
    {
	RET(l_NoMemo);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_NoMemo());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(L_NoMemo());
    }
}


void Nick_Stored_t::L_NoMemo(bool in)
{
    FT("Nick_Stored_t::L_NoMemo", (in));
    if (i_Host == "")
    {
	if (!Parent->nickserv.LCK_NoMemo())
	    l_NoMemo = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_NoMemo(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	L_NoMemo(in);
    }
}


bool Nick_Stored_t::Private()
{
    NFT("Nick_Stored_t::Private");
    if (Parent->nickserv.LCK_Private())
    {
	RET(Parent->nickserv.DEF_Private());
    }
    if (i_Host == "")
    {
	RET(i_Private);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Private());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Private());
    }
}


void Nick_Stored_t::Private(bool in)
{
    FT("Nick_Stored_t::Private", (in));
    if (i_Host == "")
    {
	if (!L_Private())
	    i_Private = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Private(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	Private(in);
    }
}


bool Nick_Stored_t::L_Private()
{
    NFT("Nick_Stored_t::L_Private");
    if (Parent->nickserv.LCK_Private())
    {
	RET(true);
    }
    if (i_Host == "")
    {
	RET(l_Private);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_Private());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(L_Private());
    }
}


void Nick_Stored_t::L_Private(bool in)
{
    FT("Nick_Stored_t::L_Private", (in));
    if (i_Host == "")
    {
	if (!Parent->nickserv.LCK_Private())
	    l_Private = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_Private(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	L_Private(in);
    }
}


bool Nick_Stored_t::PRIVMSG()
{
    NFT("Nick_Stored_t::PRIVMSG");
    if (Parent->nickserv.LCK_PRIVMSG())
    {
	RET(Parent->nickserv.DEF_PRIVMSG());
    }
    if (i_Host == "")
    {
	RET(i_PRIVMSG);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].PRIVMSG());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(PRIVMSG());
    }
}


void Nick_Stored_t::PRIVMSG(bool in)
{
    FT("Nick_Stored_t::PRIVMSG", (in));
    if (i_Host == "")
    {
	if (!L_PRIVMSG())
	i_PRIVMSG = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].PRIVMSG(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	PRIVMSG(in);
    }
}


bool Nick_Stored_t::L_PRIVMSG()
{
    NFT("Nick_Stored_t::L_PRIVMSG");
    if (Parent->nickserv.LCK_PRIVMSG())
    {
	RET(true);
    }
    if (i_Host == "")
    {
	RET(l_PRIVMSG);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_PRIVMSG());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(L_PRIVMSG());
    }
}


void Nick_Stored_t::L_PRIVMSG(bool in)
{
    FT("Nick_Stored_t::L_PRIVMSG", (in));
    if (i_Host == "")
    {
	if (!Parent->nickserv.LCK_PRIVMSG())
	l_PRIVMSG = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_PRIVMSG(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	L_PRIVMSG(in);
    }
}


mstring Nick_Stored_t::Language()
{
    NFT("Nick_Stored_t::Language");
    if (Parent->nickserv.LCK_Language())
    {
	RET(Parent->nickserv.DEF_Language().UpperCase());
    }
    if (i_Host == "")
    {
	RET(i_Language);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Language());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Language());
    }
}


void Nick_Stored_t::Language(mstring in)
{
    FT("Nick_Stored_t::Language", (in));
    if (i_Host == "")
    {
	if (!L_Language())
	i_Language = in.UpperCase();
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Language(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	Language(in);
    }
}


bool Nick_Stored_t::L_Language()
{
    NFT("Nick_Stored_t::L_Language");
    if (Parent->nickserv.LCK_Language())
    {
	RET(true);
    }
    if (i_Host == "")
    {
	RET(l_Language);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_Language());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(L_Language());
    }
}


void Nick_Stored_t::L_Language(bool in)
{
    FT("Nick_Stored_t::L_Language", (in));
    if (i_Host == "")
    {
	if (!Parent->nickserv.LCK_Language())
	l_Language = in;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_Language(in);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	L_Language(in);
    }
}


bool Nick_Stored_t::Suspended()
{
    NFT("Nick_Stored_t::Suspended");
    if (i_Host == "")
    {
	RET(i_Suspend_By != "");
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Suspended());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Suspended());
    }
}


mstring Nick_Stored_t::Suspend_By()
{
    NFT("Nick_Stored_t::Suspend_By");
    if (i_Host == "")
    {
	RET(i_Suspend_By);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Suspend_By());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Suspend_By());
    }
}


mDateTime Nick_Stored_t::Suspend_Time()
{
    NFT("Nick_Stored_t::Suspend_Time");
    if (i_Host == "")
    {
	RET(i_Suspend_Time);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Suspend_Time());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(Suspend_Time());
    }
}


void Nick_Stored_t::SendPic(mstring nick)
{
    FT("Nick_Stored_t::SendPic", (nick));

    // DccEngine.PicSend(nick, PicNum());
}


unsigned long Nick_Stored_t::PicNum()
{
    NFT("PicNum");
    if (i_Host == "")
    {
	if (!Parent->nickserv.PicSize())
	{
	    RET(i_Picture);
	}
	RET(0);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].PicNum())
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(PicNum());
    }
}


void Nick_Stored_t::GotPic(unsigned long picnum)
{
    FT("Nick_Stored_t::GotPic", (picnum));
    if (i_Host == "")
    {
	if (!Parent->nickserv.PicSize())
	    i_Picture = picnum;
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	Parent->nickserv.stored[i_Host.LowerCase()].GotPic(picnum);
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	GotPic(picnum);
    }
}


bool Nick_Stored_t::IsOnline()
{
    NFT("Nick_Stored_t::IsOnline");

    if (Parent->nickserv.IsLive(i_Name))
    {
	if (Parent->nickserv.live[i_Name.LowerCase()].IsIdentified() ||
		(!Secure() && Parent->nickserv.live[i_Name.LowerCase()].IsRecognized()))
	{
	    RET(true);
	}
    }
    RET(false);
}


mDateTime Nick_Stored_t::LastAllSeenTime()
{
    NFT("Nick_Stored_t::LastAllSeenTime");
    if (IsOnline())
    {
	RET(Now());
    }
    else if (i_Host == "")
    {
	mDateTime lastseen = i_LastSeenTime;
	for (unsigned int i=0; i<Siblings(); i++)
	{
	    if (Parent->nickserv.IsStored(Sibling(i)))
	    {
		if (Parent->nickserv.stored[Sibling(i)].LastSeenTime() > lastseen)
		{
		    lastseen = Parent->nickserv.stored[Sibling(i)].LastSeenTime();
		}
	    }
	}
	RET(lastseen);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].LastAllSeenTime());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(LastAllSeenTime());
    }
}


mDateTime Nick_Stored_t::LastSeenTime()
{
    NFT("Nick_Stored_t::LastSeenTime");
    if (IsOnline())
    {
	RET(Now());
    }
    else
    {
	RET(i_LastSeenTime);
    }
}


mstring Nick_Stored_t::LastRealName()
{
    NFT("Nick_Stored_t::LastRealName");
    if (IsOnline())
    {
	RET(Parent->nickserv.live[i_Name.LowerCase()].RealName());
    }
    else
    {
	RET(i_LastRealName);
    }
}


mstring Nick_Stored_t::LastAllMask()
{
    NFT("Nick_Stored_t::LastAllMask");
    if (IsOnline())
    {
	RET("ONLINE");
    }
    else if (i_Host == "")
    {
	mDateTime lastseen = i_LastSeenTime;
	mstring lastmask = Name() + "!" + LastMask();
	for (unsigned int i=0; i<Siblings(); i++)
	{
	    if (Parent->nickserv.IsStored(Sibling(i)))
	    {
		if (Parent->nickserv.stored[Sibling(i)].IsOnline())
		{
		    RET("ONLINE");
		}
		if (Parent->nickserv.stored[Sibling(i)].LastSeenTime() > lastseen)
		{
		    lastseen = Parent->nickserv.stored[Sibling(i)].LastSeenTime();
		    lastmask = Parent->nickserv.stored[Sibling(i)].Name() + "!" +
				Parent->nickserv.stored[Sibling(i)].LastMask();
		}
	    }
	}
	RET(lastmask);
    }
    else if (Parent->nickserv.IsStored(i_Host))
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].LastAllMask());
    }
    else
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
	RET(LastAllMask());
    }
}


mstring Nick_Stored_t::LastMask()
{
    NFT("Nick_Stored_t::LastMask");
    if (IsOnline())
    {
	RET("ONLINE");
    }
    else
    {
	RET(i_LastMask);
    }
}


mstring Nick_Stored_t::LastQuit()
{
    NFT("Nick_Stored_t::LastQuit");
    if (IsOnline())
    {
	RET("");
    }
    else
    {
	RET(i_LastQuit);
    }
}


void Nick_Stored_t::Quit(mstring message)
{
    FT("Nick_Stored_t::Quit", (message));
    
    if (IsOnline())
    {
	i_LastSeenTime = Now();
	i_LastQuit = message;
    }
    else
    {
	wxLogWarning("STORED QUIT received for user %s who is NOT ONLINE", i_Name.c_str());
    }
}


wxOutputStream &operator<<(wxOutputStream& out,Nick_Stored_t& in)
{
    set<mstring>::const_iterator i;
    out<<in.i_Name<<in.i_RegTime<<in.i_Password<<in.i_Email<<in.i_URL<<in.i_ICQ<<in.i_Description<<in.i_Comment<<in.i_Host;
    out<<in.i_access.size();
    for(i=in.i_access.begin();i!=in.i_access.end();i++)
    {
	out<<(mstring)*i;
    }
    out<<in.i_ignore.size();
    for(i=in.i_ignore.begin();i!=in.i_ignore.end();i++)
    {
	out<<(mstring)*i;
    }
    out<<in.i_Protect<<in.i_Secure<<in.i_NoExpire<<in.i_NoMemo<<in.i_Private<<in.i_PRIVMSG<<in.i_Language<<in.i_Forbidden;
    out<<in.l_Protect<<in.l_Secure<<in.l_NoExpire<<in.l_NoMemo<<in.l_Private<<in.l_PRIVMSG<<in.l_Language<<in.i_Picture;
    out<<in.i_Suspend_By<<in.i_Suspend_Time;
    out<<in.i_LastSeenTime<<in.i_LastRealName<<in.i_LastMask<<in.i_LastQuit;
    out<<in.i_UserDef.size();
    map<mstring, mstring>::const_iterator j;
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	out<<(mstring)j->first<<(mstring)j->second;
    return out;
}


wxInputStream &operator>>(wxInputStream& in, Nick_Stored_t& out)
{
    set<mstring>::size_type i,count;
    mstring dummy,dummy2;

    in>>out.i_Name>>out.i_RegTime>>out.i_Password>>out.i_Email>>out.i_URL>>out.i_ICQ>>out.i_Description>>out.i_Comment>>out.i_Host;
    out.i_access.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>dummy;
	out.i_access.insert(dummy);
    }
    out.i_ignore.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>dummy;
	out.i_ignore.insert(dummy);
    }
    in>>out.i_Protect>>out.i_Secure>>out.i_NoExpire>>out.i_NoMemo>>out.i_Private>>out.i_PRIVMSG>>out.i_Language>>out.i_Forbidden;
    in>>out.l_Protect>>out.l_Secure>>out.l_NoExpire>>out.l_NoMemo>>out.l_Private>>out.l_PRIVMSG>>out.l_Language>>out.i_Picture;
    in>>out.i_Suspend_By>>out.i_Suspend_Time;
    in>>out.i_LastSeenTime>>out.i_LastRealName>>out.i_LastMask>>out.i_LastQuit;
    out.i_UserDef.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>dummy>>dummy2;
	out.i_UserDef[dummy]=dummy2;
    }

    return in;
}


// =======================================================================

NickServ::NickServ()
{
    NFT("NickServ::NickServ");
    messages=true;
    automation=true;
}

mstring NickServ::findnextnick(mstring in)
{
    FT("NickServ::findnextnick", (in));
    mstring retval = in;
    unsigned int i;

    for (i=0; i<Parent->nickserv.Suffixes().Len(); i++)
    {
	while (retval.Len() < Parent->nickserv.Maxlen())
	{
	    retval << Parent->nickserv.Suffixes()[i];
	    if (!Parent->nickserv.IsLive(retval) &&
		!Parent->nickserv.IsStored(retval))
	    {
		RET(retval);
	    }
	}
	retval = in;
    }
    RET("");
}

void NickServ::AddCommands()
{
    NFT("NickServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
		"H*LP", Parent->commserv.ALL_Name(), NickServ::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"REG*", Parent->commserv.ALL_Name(), NickServ::do_Register);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"DROP*", Parent->commserv.REGD_Name(), NickServ::do_Drop);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LINK*", Parent->commserv.ALL_Name(), NickServ::do_Link);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"U*LIN*", Parent->commserv.REGD_Name(), NickServ::do_UnLink);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"*HOST", Parent->commserv.REGD_Name(), NickServ::do_Host);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"*SLAV*", Parent->commserv.REGD_Name(), NickServ::do_Slaves);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ID*", Parent->commserv.ALL_Name(), NickServ::do_Identify);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"INF*", Parent->commserv.ALL_Name(), NickServ::do_Info);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"GHOST*", Parent->commserv.ALL_Name(), NickServ::do_Ghost);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"REC*", Parent->commserv.ALL_Name(), NickServ::do_Recover);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LIST*", Parent->commserv.ALL_Name(), NickServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SUSP*", Parent->commserv.SOP_Name(), NickServ::do_Suspend);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNSUS*", Parent->commserv.SOP_Name(), NickServ::do_UnSuspend);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"FORB*", Parent->commserv.SOP_Name(), NickServ::do_Forbid);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"GET*PASS*", Parent->commserv.SOP_Name(), NickServ::do_Getpass);

    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* CUR*", Parent->commserv.REGD_Name(), NickServ::do_access_Current);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* ADD", Parent->commserv.REGD_Name(), NickServ::do_access_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* DEL*", Parent->commserv.REGD_Name(), NickServ::do_access_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* ERA*", Parent->commserv.REGD_Name(), NickServ::do_access_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* LIST", Parent->commserv.REGD_Name(), NickServ::do_access_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* VIEW", Parent->commserv.REGD_Name(), NickServ::do_access_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* ADD", Parent->commserv.REGD_Name(), NickServ::do_ignore_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* DEL*", Parent->commserv.REGD_Name(), NickServ::do_ignore_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* ERA*", Parent->commserv.REGD_Name(), NickServ::do_ignore_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* LIST", Parent->commserv.REGD_Name(), NickServ::do_ignore_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* VIEW", Parent->commserv.REGD_Name(), NickServ::do_ignore_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PASS*", Parent->commserv.REGD_Name(), NickServ::do_set_Password);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* EMAIL", Parent->commserv.REGD_Name(), NickServ::do_set_Email);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* URL", Parent->commserv.REGD_Name(), NickServ::do_set_URL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* ICQ", Parent->commserv.REGD_Name(), NickServ::do_set_ICQ);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* DESC*", Parent->commserv.REGD_Name(), NickServ::do_set_Description);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* COMM*", Parent->commserv.SOP_Name(), NickServ::do_set_Comment);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PIC*", Parent->commserv.REGD_Name(), NickServ::do_set_Picture);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PROT*", Parent->commserv.REGD_Name(), NickServ::do_set_Protect);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* SEC*", Parent->commserv.REGD_Name(), NickServ::do_set_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* NOEX*", Parent->commserv.SOP_Name(), NickServ::do_set_NoExpire);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* NOMEMO", Parent->commserv.REGD_Name(), NickServ::do_set_NoMemo);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PRIVM*", Parent->commserv.REGD_Name(), NickServ::do_set_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* *MSG", Parent->commserv.REGD_Name(), NickServ::do_set_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* PRIV*", Parent->commserv.REGD_Name(), NickServ::do_set_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* LANG*", Parent->commserv.REGD_Name(), NickServ::do_set_Language);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK PROT*", Parent->commserv.SOP_Name(), NickServ::do_lock_Protect);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK SEC*", Parent->commserv.SOP_Name(), NickServ::do_lock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK NOMEMO", Parent->commserv.SOP_Name(), NickServ::do_lock_NoMemo);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK PRIVM*", Parent->commserv.SOP_Name(), NickServ::do_lock_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK *MSG", Parent->commserv.SOP_Name(), NickServ::do_lock_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK PRIV*", Parent->commserv.SOP_Name(), NickServ::do_lock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK LANG*", Parent->commserv.SOP_Name(), NickServ::do_lock_Language);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PROT*", Parent->commserv.SOP_Name(), NickServ::do_unlock_Protect);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Parent->commserv.SOP_Name(), NickServ::do_unlock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK NOMEMO", Parent->commserv.SOP_Name(), NickServ::do_unlock_NoMemo);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PRIVM*", Parent->commserv.SOP_Name(), NickServ::do_unlock_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK *MSG", Parent->commserv.SOP_Name(), NickServ::do_unlock_PRIVMSG);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Parent->commserv.SOP_Name(), NickServ::do_unlock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK LANG*", Parent->commserv.SOP_Name(), NickServ::do_unlock_Language);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SET*", Parent->commserv.REGD_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LOCK", Parent->commserv.REGD_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNLOCK", Parent->commserv.REGD_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"ACC*", Parent->commserv.REGD_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"IGN*", Parent->commserv.REGD_Name(), do_1_2param);

}

void NickServ::RemCommands()
{
    NFT("NickServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

}


bool NickServ::IsLive(mstring in)
{
    FT("NickServ::IsLive", (in));
    RET(live.find(in.LowerCase())!=live.end());
}

bool NickServ::IsStored(mstring in)
{
    FT("NickServ::IsStored", (in));
    RET(stored.find(in.LowerCase())!=stored.end());
}

void NickServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_NickServ);
    FT("NickServ::execute", (data));
    //okay this is the main nickserv command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message

    mstring source, msgtype, mynick, message, command;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ").UpperCase();
    mynick  = data.ExtractWord(3, ": ");
    message = data.After(":", 2);
    command = message.Before(" ");

    if (message[0U] == CTCP_DELIM_CHAR)
    {
	if (msgtype == "PRIVMSG")
	    DccEngine::decodeRequest(mynick, source, message);
	else
	    DccEngine::decodeReply(mynick, source, message);
    }
    else if (!Parent->commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
	send(mynick, source, "Invalid command.");
    }

    mThread::ReAttach(tt_mBase);
}

void NickServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

}

void NickServ::do_Register(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Register", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring password = params.ExtractWord(2, " ");

    if (Parent->nickserv.IsStored(source))
    {
	::send(mynick, source, "Your nickname is already registered or linked.");
    }
    else if (password.Len() < 5 || !password.CmpNoCase(source))
    {
	::send(mynick, source, "Please choose a more complex password.");
    }
    else
    {
	Parent->nickserv.stored[source.LowerCase()] = Nick_Stored_t(source, password);
	Parent->nickserv.stored[source.LowerCase()].AccessAdd("*" + 
	    Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::U_H).After("!"));
	Parent->nickserv.live[source.LowerCase()].Identify(password);
	::send(mynick, source, "Your nickname has been registered.");
    }
}

void NickServ::do_Drop(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Drop", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	if (!Parent->nickserv.live[source.LowerCase()].IsIdentified())
	{
	    ::send(mynick, source, "Please IDENTIFY first.");
	    return;
	}
	else
	{
	    Parent->nickserv.stored.erase(source.LowerCase());
	    ::send(mynick, source, "Your nickname has been dropped.");
	}
    }
    else if (Parent->nickserv.IsStored(params.ExtractWord(2, " ")))
    {
	mstring target = params.ExtractWord(2, " ");
	if ((Parent->nickserv.stored[target.LowerCase()].IsSibling(source) &&
	    Parent->nickserv.live[source.LowerCase()].IsIdentified()) ||
	    (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	     Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source) &&
	     !(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	     Parent->commserv.list[Parent->commserv.OPER_Name()].IsIn(target))))
	{
	    Parent->nickserv.stored[params.ExtractWord(2, " ").LowerCase()].Drop();
	    Parent->nickserv.stored.erase(params.ExtractWord(2, " ").LowerCase());
	    ::send(mynick, source, "Nickname " + params.ExtractWord(2, " ") + " has been dropped.");
	}
	else
	{
	    ::send(mynick, source, "You do not own the nick " + params.ExtractWord(2, " "));
	}
    }
    else
    {
	::send(mynick, source, "Specified nickname not registered.");
    }
}

void NickServ::do_Link(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Link", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring hostnick = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(hostnick))
    {
	::send(mynick, source, "Nickname " + hostnick + " is not registered.");
	return;
    }

    if (Parent->nickserv.stored[hostnick.LowerCase()].Forbidden())
    {
	::send(mynick, source, "Nickname " + hostnick + " is forbidden.");
	return;
    }

    mDateTime regtime = Now();
    if (Parent->nickserv.IsStored(source))
    {
	if (!Parent->nickserv.live[source.LowerCase()].IsIdentified())
	{
	    ::send(mynick, source, "Please identify first.");
	    return;
	}
	regtime = Parent->nickserv.stored[source.LowerCase()].RegTime();
    }

    if (Parent->nickserv.stored[hostnick.LowerCase()].Slave(source, password, regtime))
    {
	Parent->nickserv.live[source.LowerCase()].Identify(password);
	::send(mynick, source, "Your nickname has been linked to " + hostnick + ".");
    }
    else
    {
	::send(mynick, source, "Password incorrect.");
    }
}

void NickServ::do_UnLink(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_UnLink", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->nickserv.stored[source.LowerCase()].Host() == "")
    {
	::send(mynick, source, "Your nickname is not linked.");
	return;
    }

    if (!Parent->nickserv.live[source.LowerCase()].IsIdentified())
    {
	::send(mynick, source, "Please identify first");
	return;
    }

    if (Parent->nickserv.stored[source.LowerCase()].Unlink())
	::send(mynick, source, "Your nickname has been unlinked.");
    else
	::send(mynick, source, "Your nickname is not linked.");
}

void NickServ::do_Host(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Host", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    mstring newhost;
    if (params.WordCount(" ") > 1)
	newhost = params.ExtractWord(2, " ");

    if (newhost != "")
    {
	if (!Parent->nickserv.IsStored(newhost))
	{
	    ::send(mynick, source, "Nickname " + newhost + " is not registered.");
	    return;
	}
	else if (!(source.LowerCase() == newhost.LowerCase() ||
		   Parent->nickserv.stored[newhost.LowerCase()].IsSibling(source)))
	{
	    ::send(mynick, source, "Nickname " + newhost + " is not linked.");
	    return;
	}
    }

    if (!Parent->nickserv.live[source.LowerCase()].IsIdentified())
    {
	::send(mynick, source, "Please identify first.");
	return;
    }

    if (newhost != "")
    {
	Parent->nickserv.stored[newhost.LowerCase()].MakeHost();
	::send(mynick, source, "Nickname " + newhost + " is now the host nickname.");
    }
    else
    {
	Parent->nickserv.stored[source.LowerCase()].MakeHost();
	::send(mynick, source, "Your nickname is now the host nickname.");
    }
}

void NickServ::do_Slaves(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Slaves", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    mstring targetnick;
    if (params.WordCount(" ") < 2)
    {
	targetnick = source;
    }
    else
    {
	if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		    Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
	    targetnick = params.ExtractWord(2, " ");
	else
	    targetnick = source;
    }

    if (!Parent->nickserv.IsStored(targetnick))
    {
	::send(mynick, source, "Nickname " + targetnick + " is not registered.");
	return;
    }

    if (Parent->nickserv.stored[targetnick.LowerCase()].Forbidden())
    {
	::send(mynick, source, "Nickname " + targetnick + " is forbidden.");
	return;
    }

    mstring output;
    if (Parent->nickserv.stored[targetnick.LowerCase()].Host() != "" &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[targetnick.LowerCase()].Host()))
	targetnick = Parent->nickserv.stored[targetnick.LowerCase()].Host();

    output << IRC_Bold << targetnick << IRC_Off << " (" <<
	Parent->nickserv.stored[targetnick.LowerCase()].Siblings() << "):";

    for (unsigned int i=0; i<Parent->nickserv.stored[targetnick.LowerCase()].Siblings(); i++)
    {
	if (Parent->nickserv.stored[targetnick.LowerCase()].Sibling(i).Len() +
		output.Len() > 510)
	{
	    ::send(mynick, source, output);
	    output = "";
	    output << IRC_Bold << targetnick << IRC_Off << " (" <<
		Parent->nickserv.stored[targetnick.LowerCase()].Siblings() << "):";
	}
	output << " " << Parent->nickserv.stored[targetnick.LowerCase()].Sibling(i);
    }
    ::send(mynick, source, output);
}


void NickServ::do_Identify(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Identify", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring password = params.ExtractWord(2, " ");
    mstring output = Parent->nickserv.live[source.LowerCase()].Identify(password);
    if (output != "")
	::send(mynick, source, output);
}

void NickServ::do_Info(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Info", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    unsigned int i;
    mstring target   = params.ExtractWord(2, " ");
    Nick_Stored_t *nick;
    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, "Nickname " + target + " is not registered.");
	return;
    }
    else
    {
	nick = &Parent->nickserv.stored[target.LowerCase()];
    }

    if (nick->Forbidden())
    {
	::send(mynick, source, "Nickname " + target + " is forbidden.");
	return;
    }

	::send(mynick, source, nick->Name() + " is " + nick->LastRealName());
    if (nick->Host() != "" && Parent->nickserv.IsStored(nick->Host()))
	::send(mynick, source, "       Host: " +
		Parent->nickserv.stored[nick->Host().LowerCase()].Name());
	::send(mynick, source, " Registered: " + nick->RegTime().Ago());
    if (!nick->IsOnline())
    {
	mstring online;
	bool isonline = false;
	if (nick->Host() != "" && Parent->nickserv.IsStored(nick->Host())
	    && Parent->nickserv.stored[nick->Host().LowerCase()].IsOnline())
	    online = Parent->nickserv.live[nick->Host().LowerCase()].Name() + " ";
	for (i=0; i<nick->Siblings(); i++)
	{
	    if (Parent->nickserv.IsStored(nick->Sibling(i)) &&
		Parent->nickserv.stored[nick->Sibling(i).LowerCase()].IsOnline())
	    {
		if (online.Len() + nick->Sibling(i).Len() > 450)
		{
		    ::send(mynick, source, "  Online As: " + online);
		    online = "";
		    isonline = true;
		}
		online += Parent->nickserv.live[nick->Sibling(i).LowerCase()].Name() + " ";
	    }
	}
	if (online != "")
	{
	    ::send(mynick, source, "  Online As: " + online);
	    isonline = true;
	}
	if (!isonline)
	{
	    if (!nick->Private() || (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source)))
		::send(mynick, source, "  Last Mask: " + nick->LastAllMask());
	    ::send(mynick, source, "  Last Seen: " + nick->LastAllSeenTime().Ago());
	    if (nick->LastAllMask().UpperCase() !=
		mstring(nick->Name() + "!" + nick->LastMask()).UpperCase() &&
		(!nick->Private() || (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))))
		::send(mynick, source, "  Last Mask: " + nick->LastMask());
	    if (nick->LastAllSeenTime() != nick->LastSeenTime())
		::send(mynick, source, "  Last Seen: " + nick->LastSeenTime().Ago());
	    ::send(mynick, source, "   Quit MSG: " + nick->LastQuit());
	}
    }
    if (nick->Suspended())
    {
	::send(mynick, source, "  Suspended: " + nick->Suspend_Time().Ago());
	::send(mynick, source, "         By: " + nick->Suspend_By());
	::send(mynick, source, "        For: " + nick->Comment());
    }
    else
    {
	if (nick->Email() != "")
	    ::send(mynick, source, "     E-Mail: " + nick->Email());
	if (nick->URL() != "")
	    ::send(mynick, source, "        URL: " + nick->URL());
	if (nick->ICQ() != "")
	    ::send(mynick, source, "        ICQ: " + nick->ICQ());
	if (nick->Description() != "")
	    ::send(mynick, source, "Description: " + nick->Description());
	if (nick->Comment() != "" && Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	    Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
	    ::send(mynick, source, "    Comment: " + nick->Comment());
    }

    mstring output = "";
    map<mstring, Committee>::iterator iter;
    for (iter=Parent->commserv.list.begin();
		iter!=Parent->commserv.list.end(); iter++)
    {
	if (iter->first != Parent->commserv.ALL_Name() &&
	    iter->first != Parent->commserv.REGD_Name() &&
	    (iter->second.HeadCom() == "" ||
	    !(Parent->commserv.IsList(iter->second.HeadCom()) &&
	      Parent->commserv.list[iter->second.HeadCom().UpperCase()].IsIn(nick->Name()))) &&
	    iter->second.IsIn(target) && (!iter->second.Private() ||
	    (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	     Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))))
	{
	    if (output.Len() + iter->second.Name().Len() > 450)
	    {
		::send(mynick, source, " Committees: " + output);
		output = "";
	    }
	    if (output != "")
		output << ", ";
	    if (iter->second.IsHead(target) && iter->second.Head() != "")
		output << IRC_Bold;
	    output << iter->second.Name();
	    if (iter->second.IsHead(target) && iter->second.Head() != "")
		output << IRC_Off;
	}
    }
    if (output != "")
	::send(mynick, source, " Committees: " + output);

    output = "";
    bool firstoption = true;
    if (nick->Protect())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick->L_Protect())
	    output << IRC_Bold;
	output << "Kill Protect";
	if (nick->L_Protect())
	    output << IRC_Off;
    }

    if (nick->Secure())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick->L_Secure())
	    output << IRC_Bold;
	output << "Secure";
	if (nick->L_Secure())
	    output << IRC_Off;
    }

    if (nick->NoExpire())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick->L_NoExpire())
	    output << IRC_Bold;
	output << "NoExpire";
	if (nick->L_NoExpire())
	    output << IRC_Off;
    }

    if (nick->NoMemo())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick->L_NoMemo())
	    output << IRC_Bold;
	output << "Denying Memos";
	if (nick->L_NoMemo())
	    output << IRC_Off;
    }

    if (nick->Private())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick->L_Private())
	    output << IRC_Bold;
	output << "Private";
	if (nick->L_Private())
	    output << IRC_Off;
    }

    if (output != "")
	::send(mynick, source, "    Options: " + output);
    if (nick->PicNum())
	::send(mynick, source, "This user has a picture available.  Type /MSG " +
	    mynick + " SEND " + nick->Name() + " to download it.");
    if (nick->IsOnline())
	::send(mynick, source, "This user is online, type /WHOIS " +
	    Parent->nickserv.live[target.LowerCase()].Name() +
	    " for more information.");
}

void NickServ::do_Ghost(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Ghost", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nick = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nick))
    {
	::send(mynick, source, "Nickname " + nick + " is not registered.");
	return;
    }

    if (!Parent->nickserv.IsLive(nick))
    {
	::send(mynick, source, "Nickname " + nick + " is not being used.");
	return;
    }

    if (pass != Parent->nickserv.stored[nick.LowerCase()].Password())
    {
	::send(mynick, source, "Password Incorrect.");
	return;
    }

    Parent->server.KILL(mynick, nick, source + " (GHOST command used)");
    if (Parent->nickserv.recovered.find(nick.LowerCase()) !=
				    Parent->nickserv.recovered.end())
	Parent->nickserv.recovered.erase(nick.LowerCase());
    ::send(mynick, source, "Nickname using " + nick + " has been killed.");
}

void NickServ::do_Recover(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Recover", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nick = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nick))
    {
	::send(mynick, source, "Nickname " + nick + " is not registered.");
	return;
    }

    if (pass != Parent->nickserv.stored[nick.LowerCase()].Password())
    {
	::send(mynick, source, "Password Incorrect.");
	return;
    }

    if (Parent->nickserv.IsLive(nick))
    {
	Parent->server.KILL(mynick, nick, source + " (RECOVER command used)");
    }

    Parent->server.NICK(nick, Parent->startup.Ownuser() ? nick.LowerCase() :
				Parent->startup.Services_User(),
				Parent->startup.Services_Host(),
				Parent->startup.Server_Name(),
				"Nickname Enforcer");
    Parent->nickserv.recovered[nick.LowerCase()] = Now();
    ::send(mynick, source, "Nickname " + nick + " is now held.");
}

void NickServ::do_List(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_List", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	mask = "*";
	listsize = Parent->config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Parent->config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Parent->config.Maxlist())
	{
	    mstring output;
	    output << "You may only list up to " << Parent->config.Maxlist()
		    << " entries per line.";
	    ::send(mynick, source, output);
	    return;
	}
    }

    ::send(mynick, source, "Nicknames matching " + mask + ":");
    map<mstring, Nick_Stored_t>::iterator iter;

    for (iter = Parent->nickserv.stored.begin(), i=0, count = 0;
			iter != Parent->nickserv.stored.end(); iter++)
    {
	if (iter->second.Name().LowerCase().Matches(mask))
	{
	    if (i < listsize && iter->second.Host() == "" && (!iter->second.Private() ||
		(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))))
	    {
		::send(mynick, source, iter->second.Name() + "  (" +
					iter->second.LastAllMask() + ")");
		i++;
	    }
	    count++;
	}
    }
    mstring output;
    output << IRC_Bold << i << IRC_Off << " of " << count << " entries shown.";
    ::send(mynick, source, output);
}

void NickServ::do_Suspend(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Suspend", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring target   = params.ExtractWord(2, " ");
    mstring reason   = params.After(" ", 2);

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, "Nickname " + target + " is not registered.");
	return;
    }

    if (Parent->nickserv.stored[target.LowerCase()].Suspended())
    {
	::send(mynick, source, "Nickname " + target + " is already suspended.");
	return;
    }

    if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsIn(target))
    {
	::send(mynick, source, "You cannot suspend an OPER.");
	return;
    }

    Parent->nickserv.stored[target.LowerCase()].Suspend(source, reason);
}

void NickServ::do_UnSuspend(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_UnSuspend", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, "Nickname " + target + " is not registered.");
	return;
    }

    if (!Parent->nickserv.stored[target.LowerCase()].Suspended())
    {
	::send(mynick, source, "Nickname " + target + " is not suspended.");
	return;
    }

    Parent->nickserv.stored[target.LowerCase()].UnSuspend();
}

void NickServ::do_Forbid(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Forbid", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring target   = params.ExtractWord(2, " ");
    if (Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, "Nickname " + target + " is registered, please DROP first.");
	return;
    }

    Parent->nickserv.stored[target.LowerCase()] = Nick_Stored_t(target);
}


void NickServ::do_Getpass(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Getpass", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring target   = params.ExtractWord(2, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, "Nickname " + target + " is not registered.");
	return;
    }

    // If we are NOT a SADMIN, and target is a PRIV GROUP.
    if (Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	!Parent->commserv.list[Parent->commserv.SADMIN_Name().LowerCase()].IsIn(source) &&
	(Parent->commserv.list[Parent->commserv.SADMIN_Name().LowerCase()].IsIn(target) ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name().LowerCase()].IsIn(target)) ||
	(Parent->commserv.IsList(Parent->commserv.ADMIN_Name()) &&
	Parent->commserv.list[Parent->commserv.ADMIN_Name().LowerCase()].IsIn(target)) ||
	(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name().LowerCase()].IsIn(target))))
    {
	::send(mynick, source, "You may not execute " + message +
				" on someone in privilaged committees.");
	return;
    }

    Nick_Stored_t *nick = &Parent->nickserv.stored[target.LowerCase()];
    mstring host = nick->Name();
    if (nick->Host() != "" && Parent->nickserv.IsStored(nick->Host()))
	host = Parent->nickserv.stored[nick->Host().LowerCase()].Name();

    ::send(mynick, source, "Password for nickname " + nick->Name() + " (" +
	host + ") is " + IRC_Bold + nick->Password() + IRC_Off + ".");
}


void NickServ::do_access_Current(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_access_Current", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    if (Parent->nickserv.stored[source.LowerCase()].AccessAdd("*" + 
	    Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::U_H).After("!")))
    {
	::send(mynick, source, "Hostmask has been added to your access list.");
    }
    else
    {
	::send(mynick, source, "Hostmask already exists or is inclusive.");
    }
}

void NickServ::do_access_Add(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_access_Add", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring hostmask = params.ExtractWord(3, " ");

    if (!(hostmask.Contains("@") && hostmask.WordCount("@") == 2) ||
	  hostmask.Contains("!"))
    {
	::send(mynick, source, "Hostmask must contain '@', and not '!'.");
	return;
    }
    
    if (Parent->nickserv.stored[source.LowerCase()].AccessAdd(hostmask))
    {
	::send(mynick, source, "Hostmask has been added to your access list.");
    }
    else
    {
	::send(mynick, source, "Hostmask already exists or is inclusive.");
    }
}

void NickServ::do_access_Del(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_access_Del", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring hostmask = params.ExtractWord(3, " ");

    if ((!(hostmask.Contains("@") && hostmask.WordCount("@") == 2) ||
	hostmask.Contains("!")) && !hostmask.IsNumber())
    {
	::send(mynick, source, "Hostmask must contain '@', and not '!'.");
	return;
    }

    if (hostmask.IsNumber())
	hostmask = Parent->nickserv.stored[source.LowerCase()].Access(atoi(hostmask.c_str())-1);
    unsigned int count;
    if (count = Parent->nickserv.stored[source.LowerCase()].AccessDel(hostmask))
    {
	mstring retval = "";
	retval << count << " hostmasks removed from your access list.";
	::send(mynick, source, retval);
    }
    else
    {
	::send(mynick, source, "No hostmasks match your request.");
    }
}

void NickServ::do_access_List(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_access_List", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    mstring target = source;

    if (params.WordCount(" ") >= 3 &&
	Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
    {
	    target = params.ExtractWord(3, " ");
    }

    if (source != target && !Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, "Nickname " + target + " is not registered.");
	return;
    }

    if (Parent->nickserv.stored[target.LowerCase()].Access())
    {
	::send(mynick, source, "Access List for " + target);
    }
    else
    {
	::send(mynick, source, "No access list for " + target);
    }

    unsigned int i;
    mstring retval;
    for (i=0; i<Parent->nickserv.stored[target.LowerCase()].Access(); i++)
    {
	retval = "";
	retval << i+1 << ". " << Parent->nickserv.stored[target.LowerCase()].Access(i);
	::send(mynick, source, retval);
    }
}

void NickServ::do_ignore_Add(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_ignore_Add", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring target = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, "Your nickname is not registered.");
	return;
    }

    if (Parent->nickserv.stored[source.LowerCase()].IgnoreAdd(target))
    {
	::send(mynick, source, "Hostmask has been added to your access list.");
    }
    else
    {
	::send(mynick, source, "Hostmask already exists or is inclusive.");
    }
}

void NickServ::do_ignore_Del(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_ignore_Del", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring target = params.ExtractWord(3, " ");

    if (target.IsNumber())
	target = Parent->nickserv.stored[source.LowerCase()].Ignore(atoi(target.c_str())-1);
    unsigned int count;
    if (count = Parent->nickserv.stored[source.LowerCase()].IgnoreDel(target))
    {
	mstring retval = "";
	retval << count << " nicknames removed from your ignore list.";
	::send(mynick, source, retval);
    }
    else
    {
	::send(mynick, source, "No nickname is being ignored.");
    }

}

void NickServ::do_ignore_List(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_ignore_List", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    mstring target = source;

    if (params.WordCount(" ") >= 3 &&
	Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
    {
	    target = params.ExtractWord(3, " ");
    }

    if (source != target && !Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, "Nickname " + target + " is not registered.");
	return;
    }

    if (Parent->nickserv.stored[target.LowerCase()].Ignore())
    {
	::send(mynick, source, "Ignore List for " + target);
    }
    else
    {
	::send(mynick, source, "No ignore list for " + target);
    }

    unsigned int i;
    mstring retval;
    for (i=0; i<Parent->nickserv.stored[target.LowerCase()].Ignore(); i++)
    {
	retval = "";
	retval << i+1 << ". " << Parent->nickserv.stored[target.LowerCase()].Ignore(i);
	::send(mynick, source, retval);
    }
}

void NickServ::do_set_Password(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Password", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring newpass = params.ExtractWord(3, " ");

    if (!Parent->nickserv.live[source.LowerCase()].IsIdentified())
    {
	::send(mynick, source, "Please identify first.");
	return;
    }

    mstring oldpass = Parent->nickserv.stored[source.LowerCase()].Password();
    if (!newpass.CmpNoCase(oldpass))
    {
	::send(mynick, source, "Your new password is too similar to your old one.");
	return;
    }

    if (!newpass.CmpNoCase(source) || newpass.Len() < 5)
    {
	::send(mynick, source, "Please choose a more complex password.");
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].Password(newpass);
    ::send(mynick, source, "Your password has been changed.");
}

void NickServ::do_set_Email(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Email", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (!newvalue.CmpNoCase("none"))
	newvalue = "";

    Parent->nickserv.stored[source.LowerCase()].Email(newvalue);
    ::send(mynick, source, "Your email address has been set to \"" + newvalue + "\".");
}

void NickServ::do_set_URL(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_URL", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (!newvalue.CmpNoCase("none"))
	newvalue = "";

    Parent->nickserv.stored[source.LowerCase()].URL(newvalue);
    ::send(mynick, source, "Your URL has been set to \"" + newvalue + "\".");
}

void NickServ::do_set_ICQ(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_ICQ", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (!newvalue.CmpNoCase("none"))
	newvalue = "";

    Parent->nickserv.stored[source.LowerCase()].ICQ(newvalue);
    ::send(mynick, source, "Your ICQ number has been set to \"" + newvalue + "\".");
}

void NickServ::do_set_Description(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Description", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (!newvalue.CmpNoCase("none"))
	newvalue = "";

    Parent->nickserv.stored[source.LowerCase()].Description(newvalue);
    ::send(mynick, source, "Your description has been set to \"" + newvalue + "\".");
}

void NickServ::do_set_Comment(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Comment", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring target = params.ExtractWord(3, " ");
    mstring comment = params.After(" ", 3);

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, "Nickname " + target + " is not registered.");
	return;
    }

    if (!comment.CmpNoCase("none"))
	comment = "";

    Parent->nickserv.stored[target.LowerCase()].Comment(comment);
    ::send(mynick, source, "Comment for " + target + " has been set to \"" + comment + "\".");
}

void NickServ::do_set_Picture(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Picture", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    Parent->nickserv.live[source.LowerCase()].InFlight.Picture(mynick);
}

void NickServ::do_set_Protect(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Protect", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_Protect())
    {
	::send(mynick, source, "Protect is a LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].Protect(onoff.GetBool());
    ::send(mynick, source, "Kill Protect is now set to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}

void NickServ::do_set_Secure(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Secure", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_Secure())
    {
	::send(mynick, source, "Secure is a LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].Secure(onoff.GetBool());
    ::send(mynick, source, "Secure is now set to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}

void NickServ::do_set_NoExpire(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_NoExpire", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_NoExpire())
    {
	::send(mynick, source, "NoExpire is a SERVICE LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_NoExpire())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].NoExpire(onoff.GetBool());
    ::send(mynick, source, "NoExpire for " + nickname + " is now set to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}


void NickServ::do_set_NoMemo(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_NoMemo", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_NoMemo())
    {
	::send(mynick, source, "NoMemo is a LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].NoMemo(onoff.GetBool());
    ::send(mynick, source, "Denying Memos is now set to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}

void NickServ::do_set_Private(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Private", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_Private())
    {
	::send(mynick, source, "Private is a LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].Private(onoff.GetBool());
    ::send(mynick, source, "Private is now set to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}

void NickServ::do_set_PRIVMSG(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_PRIVMSG", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_PRIVMSG())
    {
	::send(mynick, source, "PRIVMSG is a LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].PRIVMSG(onoff.GetBool());
    ::send(mynick, source, "PRIVMSG is now set to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}

void NickServ::do_set_Language(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring lang = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_Language())
    {
	::send(mynick, source, "Language is a LOCKED value.");
	return;
    }

    if (!lang.CmpNoCase("default") || !lang.CmpNoCase("reset"))
    {
	lang = Parent->nickserv.DEF_Language().UpperCase();
    }
    else
    {
	// check for valid language ...
    }

    Parent->nickserv.stored[source.LowerCase()].Language(lang);
    ::send(mynick, source, "Language is now set to " + lang.UpperCase() + ".");
}

void NickServ::do_lock_Protect(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_Protect", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_Protect())
    {
	::send(mynick, source, "Kill Protect is a SERVICE LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Protect(false);
    Parent->nickserv.stored[nickname.LowerCase()].Protect(onoff.GetBool());
    Parent->nickserv.stored[nickname.LowerCase()].L_Protect(true);
    ::send(mynick, source, "Kill Protect for " + nickname + " is now locked to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}

void NickServ::do_lock_Secure(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_Secure", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_Secure())
    {
	::send(mynick, source, "Secure is a SERVICE LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Secure(false);
    Parent->nickserv.stored[nickname.LowerCase()].Secure(onoff.GetBool());
    Parent->nickserv.stored[nickname.LowerCase()].L_Secure(true);
    ::send(mynick, source, "Secure for " + nickname + " is now locked to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}

void NickServ::do_lock_NoMemo(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_NoMemo", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_NoMemo())
    {
	::send(mynick, source, "Denying Memos is a SERVICE LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_NoMemo(false);
    Parent->nickserv.stored[nickname.LowerCase()].NoMemo(onoff.GetBool());
    Parent->nickserv.stored[nickname.LowerCase()].L_NoMemo(true);
    ::send(mynick, source, "Denying Memos for " + nickname + " is now locked to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}

void NickServ::do_lock_Private(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_Private", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_Private())
    {
	::send(mynick, source, "Protect is a SERVICE LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Private(false);
    Parent->nickserv.stored[nickname.LowerCase()].Private(onoff.GetBool());
    Parent->nickserv.stored[nickname.LowerCase()].L_Private(true);
    ::send(mynick, source, "Private for " + nickname + " is now locked to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}

void NickServ::do_lock_PRIVMSG(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_PRIVMSG", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_PRIVMSG())
    {
	::send(mynick, source, "PRIVMSG is a SERVICE LOCKED value.");
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, "The value you have entered is not valid.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_PRIVMSG(false);
    Parent->nickserv.stored[nickname.LowerCase()].PRIVMSG(onoff.GetBool());
    Parent->nickserv.stored[nickname.LowerCase()].L_PRIVMSG(true);
    ::send(mynick, source, "PRIVMSG for " + nickname + " is now locked to " + mstring(onoff.GetBool() ? "ON." : "OFF."));
}

void NickServ::do_lock_Language(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring lang     = params.ExtractWord(4, " ").UpperCase();

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_Language())
    {
	::send(mynick, source, "Language is a SERVICE LOCKED value.");
	return;
    }

    if (!lang.CmpNoCase("default") || !lang.CmpNoCase("reset"))
    {
	lang = Parent->nickserv.DEF_Language().UpperCase();
    }
    else
    {
	// check for valid language ...
    }


    Parent->nickserv.stored[nickname.LowerCase()].L_Language(false);
    Parent->nickserv.stored[nickname.LowerCase()].Language(lang);
    Parent->nickserv.stored[nickname.LowerCase()].L_Language(true);
    ::send(mynick, source, "Language for " + nickname + " is now locked to " + lang.UpperCase() + ".");
}

void NickServ::do_unlock_Protect(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_Protect", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_Protect())
    {
	::send(mynick, source, "Kill Protect is a SERVICE LOCKED value.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Protect(false);
    ::send(mynick, source, "Kill Protect for " + nickname + " is now unlocked.");
}

void NickServ::do_unlock_Secure(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_Secure", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_Secure())
    {
	::send(mynick, source, "Secure is a SERVICE LOCKED value.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Secure(false);
    ::send(mynick, source, "Secure for " + nickname + " is now unlocked.");
}

void NickServ::do_unlock_NoMemo(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_NoMemo", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_NoMemo())
    {
	::send(mynick, source, "Denying Memos is a SERVICE LOCKED value.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_NoMemo(false);
    ::send(mynick, source, "Denying Memos for " + nickname + " is now unlocked.");
}

void NickServ::do_unlock_Private(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_Private", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_Private())
    {
	::send(mynick, source, "Protect is a SERVICE LOCKED value.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Private(false);
    ::send(mynick, source, "Private for " + nickname + " is now unlocked.");
}

void NickServ::do_unlock_PRIVMSG(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_PRIVMSG", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_PRIVMSG())
    {
	::send(mynick, source, "PRIVMSG is a SERVICE LOCKED value.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_PRIVMSG(false);
    ::send(mynick, source, "PRIVMSG for " + nickname + " is now unlocked.");
}

void NickServ::do_unlock_Language(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, "Nickname " + nickname + " is not registered.");
	return;
    }

    if (Parent->nickserv.LCK_Language())
    {
	::send(mynick, source, "Language is a SERVICE LOCKED value.");
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Language(false);
    ::send(mynick, source, "Language for " + nickname + " is now unlocked.");
}

void NickServ::save_database(wxOutputStream& out)
{
	//
	map<mstring,Nick_Stored_t>::iterator i;
	out<<stored.size();
        // todo call script saving hooks.
	for(i=stored.begin();i!=stored.end();i++)
	{
	    out<<i->second;
	    // todo call script saving hooks.
	}
}

void NickServ::load_database(wxInputStream& in)
{
    map<mstring,Nick_Stored_t>::size_type i,count;
    in>>count;
    stored.clear();
    for(i=0;i<count;i++)
    {
	Nick_Stored_t tmpstored;
	in>>tmpstored;
	stored[tmpstored.Name()]=tmpstored;
    }

    // Go through the map and populate 'slaves',
    // clean up if nessicary.
    map<mstring,Nick_Stored_t>::iterator iter;
    for (iter=stored.begin(); iter!=stored.end(); iter++)
    {
	if (IsStored(iter->second.i_Host))
	{
	    stored[iter->second.i_Host.LowerCase()].i_slaves.insert(iter->second.i_Name.LowerCase());
	}
	else
	{
	    iter->second.i_Host = "";
	}
    }

}
