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
** Revision 1.63  2000/03/08 23:38:37  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.62  2000/03/02 07:25:11  prez
** Added stuff to do the chanserv greet timings (ie. only greet if a user has
** been OUT of channel over 'x' seconds).  New stored chanserv cfg item.
**
** Revision 1.61  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.60  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.59  2000/02/21 03:27:39  prez
** Updated language files ...
**
** Revision 1.58  2000/02/17 12:55:06  ungod
** still working on borlandization
**
** Revision 1.57  2000/02/16 12:59:40  ungod
** fixing for borland compilability
**
** Revision 1.56  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.55  2000/02/15 10:37:50  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


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
/*
	mstring *arg;
	if (ACE_Reactor::instance()->cancel_timer(timer,
					    (const void **) arg))
	{
	    delete arg;
	}
*/
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
	End(0u);
/*
    mstring *arg;
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
						(const void **) arg))
	{
	    delete arg;
	}
*/
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
/*
    mstring *arg;
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
						(const void **) arg))
	{
	    delete arg;
	}
*/
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
	    send(service, nick, Parent->getMessage(nick, "ERR_SITUATION/FILEINPROG"));
	    return;
	}
	else
	{
	    Cancel();
	}
    }
    else if (Exists())
    {
	End(0u);
    }

    if (!Parent->nickserv.IsStored(nick))
    {
	send(mynick, nick, Parent->getMessage(nick, "NS_YOU_STATUS/ISNOTSTORED"));
	return;
    }

    if (IsChan(who))
    {
	if (!Parent->chanserv.IsStored(who))
	{
	    send(mynick, nick, Parent->getMessage(nick, "CS_STATUS/ISNOTSTORED"),
		    who.c_str());
	    return;
	}

	if (file)
	{
	    send(mynick, nick, Parent->getMessage(nick, "ERR_SYNTAX/CHANFILEATTACH"));
	    return;
	}
    }
    else
    {
	if (!Parent->nickserv.IsStored(who))
	{
	    send(mynick, nick, Parent->getMessage(nick, "NS_OTH_STATUS/ISNOTSTORED"),
			who.c_str());
	    return;
	}

	if (file && !Parent->memoserv.Files())
	{
	    send(mynick, nick, Parent->getMessage(nick, "MS_SYNTAX/FILEDISABLED"));
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
	send(service, nick, Parent->getMessage(nick, "MS_COMMAND/PENDING_FILE"),
	    ToHumanTime(Parent->memoserv.InFlight()).c_str());
    else
	send(service, nick, Parent->getMessage(nick, "MS_COMMAND/PENDING"),
	    ToHumanTime(Parent->memoserv.InFlight()).c_str());
}


// Add text to a memo, and re-start the timer.
void Nick_Live_t::InFlight_t::Continue(mstring message)
{
    FT("Nick_Live_t::InFlight_t::Continue", (message));
    if (!memo)
    {
	send(service, nick, Parent->getMessage(nick, "MS_STATUS/NOPENDING"));
	return;
    }
    text += message;
/*
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
*/
    send(service, nick, Parent->getMessage(nick, "MS_COMMAND/CONTINUE"),
	    ToHumanTime(Parent->memoserv.InFlight()).c_str());
}


// Cancel a memo or picture send.
void Nick_Live_t::InFlight_t::Cancel()
{
    NFT("Nick_Live_t::InFlight_t::Cancel");

/*
    mstring *arg;
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
					    (const void **) arg))
	{
	    delete arg;
	}
*/
    if (memo)
	send(service, nick, Parent->getMessage(nick, "MS_COMMAND/CANCEL"));
    else
	send(service, nick, Parent->getMessage(nick, "NS_YOU_COMMAND/CANCEL"));
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
/*
	mstring *arg;
	if (timer)
	    if (ACE_Reactor::instance()->cancel_timer(timer,
						(const void **) arg))
	    {
		delete arg;
	    }
*/
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
			send(service, nick, Parent->getMessage(nick, "MS_COMMAND/SENT"),
			    recipiant.c_str(),
			    Parent->chanserv.stored[recipiant.LowerCase()].Founder().c_str());
			if (Parent->chanserv.IsLive(recipiant))
			{
			    Chan_Live_t *chan = &Parent->chanserv.live[recipiant.LowerCase()];
			    unsigned int i;
			    for(i=0; i<chan->Users(); i++)
			    {
				if (Parent->chanserv.stored[recipiant.LowerCase()].GetAccess(chan->User(i), "READMEMO"))
				{
				    send(service, chan->User(i), Parent->getMessage(chan->User(i), "MS_COMMAND/CS_NEW"),
					    Parent->memoserv.channel[recipiant.LowerCase()].size(),
					    recipiant.c_str(), nick.c_str(),
					    service.c_str(), recipiant.c_str(),
					    Parent->memoserv.channel[recipiant.LowerCase()].size());
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
			    send(service, nick, Parent->getMessage(nick, "MS_COMMAND/SENT"),
				recipiant.c_str(), realrecipiant.c_str());

			    Nick_Stored_t *nick = &Parent->nickserv.stored[realrecipiant.LowerCase()];
			    if (nick->IsOnline())
				send(service, realrecipiant, Parent->getMessage(realrecipiant, "MS_COMMAND/NS_NEW"),
				    Parent->memoserv.nick[realrecipiant.LowerCase()].size(),
				    service.c_str(),
				    Parent->memoserv.nick[realrecipiant.LowerCase()].size());
			    unsigned int i;
			    for (i=0; i < nick->Siblings(); i++)
			    {
				if (Parent->nickserv.IsStored(nick->Sibling(i)) &&
				    Parent->nickserv.stored[nick->Sibling(i).LowerCase()].IsOnline())
				{
				    send(service, nick->Sibling(i), Parent->getMessage(nick->Sibling(i), "MS_COMMAND/NS_NEW"),
					Parent->memoserv.nick[realrecipiant.LowerCase()].size(),
					service.c_str(),
					Parent->memoserv.nick[realrecipiant.LowerCase()].size());
				}
			    }
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
		    send(service, nick, Parent->getMessage(nick, "NS_YOU_COMMAND/SAVED"));
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
	    send(service, nick, Parent->getMessage(nick, "ERR_SITUATION/FILEINPROG"));
	    return;
	}
	else
	{
	    Cancel();
	}
    }
    else if (Exists())
    {
	End(0u);
    }

    if (Parent->nickserv.IsStored(nick))
    {
	send(service, nick, Parent->getMessage(nick, "NS_YOU_STATUS/ISNOTSTORED"));
	return;
    }
    else if (!Parent->nickserv.PicSize())
    {
	send(service, nick, Parent->getMessage(nick, "NS_YOU_STATUS/PICDISABLED"));
    }

    memo = false;
    fileattach = true;
    sender = nick;
    service = mynick;
    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
    send(service, nick, Parent->getMessage(nick, "NS_YOU_COMMAND/PENDING"));
}


Nick_Live_t::Nick_Live_t()
{
    NFT("Nick_Live_t::Nick_Live_t");
    identified = false;
    InFlight.init();
    last_msg_entries = flood_triggered_times = failed_passwds = 0;
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
    last_msg_entries = flood_triggered_times = failed_passwds = 0;

    { MLOCK(("OperServ", "Akill"));
    if (Parent->operserv.Akill_find(i_user + "@" + i_host))
    {
        ACE_Reactor::instance()->schedule_timer(&(Parent->nickserv.kosh),
	    new mstring(i_Name + ":" + Parent->operserv.Akill->Value().second),
	    ACE_Time_Value(1));
	return;
    }}

    if (Parent->operserv.AddHost(i_host))
    {
	// Set off timer in 1s to do this:
	// Parent->server.KILL(Parent->nickserv.FirstName(), i_Name, "Too many connections from one host");

        ACE_Reactor::instance()->schedule_timer(&(Parent->nickserv.kosh),
	    new mstring(i_Name + ":" + Parent->operserv.Def_Clone()),
	    ACE_Time_Value(1));

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
    last_msg_entries = flood_triggered_times = failed_passwds = 0;
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
	// If this returns 0, then the channel is empty.
	if (Parent->chanserv.live[chan.LowerCase()].Part(i_Name) == 0)
	    Parent->chanserv.live.erase(chan.LowerCase());
    }
    else
    {
	wxLogDebug("User %s PART from non-existant channel %s", i_Name.c_str(), chan.c_str());
    }

    joined_channels.erase(chan.LowerCase());
}

void Nick_Live_t::Kick(mstring kicker, mstring chan)
{
    FT("Nick_Live_t::Kick", (kicker, chan));
    if (Parent->chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
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
	InFlight.End(0u);

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
    if (HasMode("o") || IsServices())
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
	if (flood_triggered_times >= Parent->operserv.Ignore_Limit())
	{
	    Parent->operserv.Ignore_insert(Mask(Parent->operserv.Ignore_Method()), true, i_Name);
	    send(Parent->servmsg.FirstName(), i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/IGNORE_TRIGGER"),
			Parent->operserv.Flood_Msgs(), ToHumanTime(Parent->operserv.Flood_Time()).c_str());
	    send(Parent->servmsg.FirstName(), i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/PERM_IGNORE"),
			Parent->operserv.Ignore_Limit());
	    wxLogNotice(Parent->getLogMessage("OTHER/PERM_IGNORE"),
			Mask(N_U_P_H).c_str());
	    announce(Parent->servmsg.FirstName(), Parent->getMessage("MISC/FLOOD_PERM"),
			i_Name.c_str());
	}
	else
	{
	    Parent->operserv.Ignore_insert(Mask(Parent->operserv.Ignore_Method()), false, i_Name);
	    send(Parent->servmsg.FirstName(), i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/IGNORE_TRIGGER"),
			Parent->operserv.Flood_Msgs(), ToHumanTime(Parent->operserv.Flood_Time()).c_str());
	    send(Parent->servmsg.FirstName(), i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/TEMP_IGNORE"),
			ToHumanNumber(flood_triggered_times).c_str(), Parent->operserv.Ignore_Limit(),
			ToHumanTime(Parent->operserv.Ignore_Time()).c_str());
	    wxLogNotice(Parent->getLogMessage("OTHER/TEMP_IGNORE"),
			Mask(N_U_P_H).c_str());
	    announce(Parent->servmsg.FirstName(), Parent->getMessage("MISC/FLOOD_TEMP"),
			i_Name.c_str());
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
    // This is needed to send logon notices ONLY for committees
    // we have joined by a nick change.
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

    // Carry over failed attempts (so /nick doesnt cure all!)
    for (i=0; i<try_chan_ident.size(); i++)
	if (Parent->chanserv.IsStored(try_chan_ident[i]))
	    Parent->chanserv.stored[try_chan_ident[i]].ChgAttempt(i_Name, in);

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

    // WooHoo, we have a new nick!
    i_Name = in;
    i_My_Signon_Time = Now();

    if (Parent->nickserv.IsStored(i_Name))
    {
	if (Parent->nickserv.stored[i_Name.LowerCase()].IsOnline())
	    Parent->nickserv.stored[i_Name.LowerCase()].Signon(i_realname, Mask(U_P_H).After("!"));
	else if (Parent->nickserv.stored[i_Name.LowerCase()].Protect())
	{
	    Parent->nickserv.send(i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/PROTECTED"));
	}

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
	    {
		Parent->operserv.RemHost(i_host);
		MLOCK(("OperServ", "OperDeny"));
		if (Parent->operserv.OperDeny_find(Mask(N_U_P_H)) &&
		    !IsServices())
		{
		    // if (Parent->ircd.Has_SVSMODE)
		    // {
		    SendMode("-o");
		    // }
		    // else
		    // {
		    // Parent->server.KILL(Parent->operserv.FirstName(),
		    //		    i_Name, Parent->getMessage(i_Name, "MISC/KILL_OPERDENY"));
		    // }
		}
	    }
	    else if (modes.Contains(in[i]) && !IsServices())
	    {
		Parent->operserv.AddHost(i_host);
	    }

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
		wxLogDebug("MODE change %c%c received for %s that is currently in effect",
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
    mstring user = i_user;
    switch (type)
    {
    case N:		// nick!*@*
	retval = i_Name + "!*@*";
	break;

    case N_U_P_H:	// nick!user@port.host
	retval = i_Name + "!" + user + "@" + i_host;
	break;

    case N_U_H:		// nick!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_host.IsNumber())
	    retval = i_Name + "!*" + user + "@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains("."))
	    retval = i_Name + "!*" + user + "@*." + i_host.After(".");
	else
	    retval = i_Name + "!*" + user + "@" + i_host;
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
	retval = "*!" + user + "@" + i_host;
	break;

    case U_H:		// *!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_host.IsNumber())
	    retval = "*!*" + user + "@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains("."))
	    retval = "*!*" + user + "@*." + i_host.After(".");
	else
	    retval = "*!*" + user + "@" + i_host;
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
	    RET(Parent->getMessage(i_Name, "CS_COMMAND/IDENTIFIED"));
	}
	else
	{
	    // Keep record of all channel attempts, so that when
	    // we change nick or quit, we can let chanserv know.
	    vector<mstring>::iterator iter;
	    for (iter=try_chan_ident.begin(); iter!=try_chan_ident.end(); iter++) ;
	    if (iter == try_chan_ident.end())
		try_chan_ident.push_back(channel.LowerCase());

	    if (failtimes >= Parent->chanserv.Passfail())
	    {
		Parent->server.KILL(Parent->nickserv.FirstName(), i_Name,
			Parent->getMessage(i_Name, "MISC/KILL_PASS_FAIL"));
		wxLogNotice(Parent->getLogMessage("OTHER/KLLL_CHAN_PASS"),
			Mask(N_U_P_H).c_str(), channel.c_str());
		RET("");
	    }
	    else
	    {
		RET(Parent->getMessage(i_Name, "ERR_SITUATION/CHAN_WRONG_PASS"));
	    }
	}
    }
    else
    {
	RET(Parent->getMessage(i_Name, "CS_STATUS/ISNOTSTORED"));
    }
}


void Nick_Live_t::UnChanIdentify(mstring channel)
{
    FT("Nick_Live_t::UnChanIdentify", (channel));

    if (IsChanIdentified(channel))
    {
	chans_founder_identd.erase(channel.LowerCase());
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
	RET(Parent->getMessage(i_Name, "NS_YOU_STATUS/IDENTIFIED"));
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
	    RET(Parent->getMessage(i_Name, "NS_YOU_COMMAND/IDENTIFIED"));
	}
	else
	{
	    failed_passwds++;
	    if (failed_passwds >= Parent->nickserv.Passfail())
	    {
		Parent->server.KILL(Parent->nickserv.FirstName(), i_Name,
			Parent->getMessage(i_Name, "MISC/KILL_PASS_FAIL"));
		wxLogNotice(Parent->getLogMessage("OTHER/KLLL_NICK_PASS"),
			Mask(N_U_P_H).c_str(), i_Name.c_str());
		RET("");
	    }
	    else
	    {
		RET(Parent->getMessage(i_Name, "ERR_SITUATION/NICK_WRONG_PASS"));
	    }
	}
    }
    else
    {
	RET(Parent->getMessage(i_Name, "NS_YOU_STATUS/ISNOTSTORED"));
    }
}

void Nick_Live_t::UnIdentify()
{
    NFT("Nick_Live_t::UnIdentify");
    identified = false;
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
    if (Host() == "")
    {
	for (i=0; i<Siblings(); i++)
	{
	    Parent->nickserv.stored.erase(Sibling(i));
	}
    }
    else
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

    if (Parent->memoserv.IsNick(i_Name))
	Parent->memoserv.nick.erase(i_Name.LowerCase());
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
    if (Host() == "")
    {
	RET(i_Email);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Email());
    }
}


void Nick_Stored_t::Email(mstring in)
{
    FT("Nick_Stored_t::Email", (in));
    if (Host() == "")
    {
	i_Email = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Email(in);
    }
}


mstring Nick_Stored_t::URL()
{
    NFT("Nick_Stored_t::URL");
    if (Host() == "")
    {
	RET(i_URL);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].URL());
    }
}


void Nick_Stored_t::URL(mstring in)
{
    FT("Nick_Stored_t::URL", (in));
    if (Host() == "")
    {
	i_URL = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].URL(in);
    }
}


mstring Nick_Stored_t::ICQ()
{
    NFT("Nick_Stored_t::ICQ");
    if (Host() == "")
    {
	RET(i_ICQ);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].ICQ());
    }
}


void Nick_Stored_t::ICQ(mstring in)
{
    FT("Nick_Stored_t::ICQ", (in));
    if (Host() == "")
    {
	i_ICQ = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].ICQ(in);
    }
}


mstring Nick_Stored_t::Description()
{
    NFT("Nick_Stored_t::Description");
    if (Host() == "")
    {
	RET(i_Description);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Description());
    }
}


void Nick_Stored_t::Description(mstring in)
{
    FT("Nick_Stored_t::Description", (in));
    if (Host() == "")
    {
	i_Description = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Description();
    }
}


mstring Nick_Stored_t::Comment()
{
    NFT("Nick_Stored_t::Comment");
    if (Host() == "")
    {
	RET(i_Comment);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Comment());
    }
}


void Nick_Stored_t::Comment(mstring in)
{
    FT("Nick_Stored_t::Comment", (in));
    if (Host() == "")
    {
	i_Comment = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Comment(in);
    }
}


void Nick_Stored_t::Suspend(mstring name)
{
    FT("Nick_Stored_t::Suspend", (name));
    if (Host() == "")
    {
	i_Suspend_By = name;
	i_Suspend_Time = Now();
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Suspend(name);
    }
}


void Nick_Stored_t::UnSuspend()
{
    NFT("Nick_Stored_t::UnSuspend");
    if (Host() == "")
    {
	i_Suspend_By = "";
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].UnSuspend();
    }
}

mstring Nick_Stored_t::Host()
{
    NFT("Nick_Stored_t::Host");
    if (i_Host != "" && !Parent->nickserv.IsStored(i_Host))
    {
	wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		i_Host.c_str(), i_Name.c_str());
	i_Host = "";
    }
    RET(i_Host);
}

mstring Nick_Stored_t::Password()
{
    NFT("Nick_Stored_t::Password");
    if (Host() == "")
    {
	RET(i_Password);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Password());
    }
}


void Nick_Stored_t::Password(mstring in)
{
    FT("Nick_Stored_t::Password", (in));
    if (Host() == "")
    {
	i_Password = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Password(in);
    }
}


bool Nick_Stored_t::Slave(mstring nick, mstring password, mDateTime regtime)
{
    FT("Nick_Stored_t::Slave", (nick, password, regtime));

    if (Host() == "")
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
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Slave(nick, password, regtime));
    }
}


unsigned int Nick_Stored_t::Siblings()
{
    NFT("Nick_Stored_t::Siblings");
    if (Host() == "")
    {
	RET(i_slaves.size());
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Siblings());
    }
}


mstring Nick_Stored_t::Sibling(unsigned int count)
{
    FT("Nick_Stored_t::Siblings", (count));
    if (Host() == "")
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
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Sibling(count));
    }
}


bool Nick_Stored_t::IsSibling(mstring nick)
{
    FT("Nick_Stored_t::IsSibling", (nick));
    if (i_Name.LowerCase() == nick.LowerCase())
    {
	RET(true);
    }
    else if (Host() == "")
    {
	RET(i_slaves.find(nick.LowerCase()) != i_slaves.end());
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].IsSibling(nick));
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
	if (niter->first != i_Name.LowerCase() &&
	    !IsSibling(niter->first) &&
	    niter->second.IsIgnore(oldnick))
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
    if (Host() == "")
    {
	RET(false);
    }
    else
    {
	// Re-point all slaves to me and copy the slaves list.
	// Then clear the host's slave list, point host to me,
	// and finally set my host pointer to "".
	for (unsigned int i=0; i<Parent->nickserv.stored[i_Host.LowerCase()].Siblings(); i++)
	{
	    if (Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i).LowerCase() != i_Name.LowerCase())
		if (Parent->nickserv.IsStored(Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i)))
		{
		    i_slaves.insert(Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i));
		    Parent->nickserv.stored[Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i)].i_Host = i_Name;
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
	mstring tmp = i_Host;
	i_Host = "";
	ChangeOver(tmp);
	RET(true);
    }
}


bool Nick_Stored_t::Unlink()
{
    NFT("Nick_Stored_t::Unlink");
    if (Host() == "")
    {
	RET(false)
    }
    else
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
	i_Picture = 0;
	i_Suspend_By = Parent->nickserv.stored[i_Host.LowerCase()].i_Suspend_By;
	i_Suspend_Time = Parent->nickserv.stored[i_Host.LowerCase()].i_Suspend_Time;
	Parent->nickserv.stored[i_Host.LowerCase()].i_slaves.erase(i_Name.LowerCase());
	i_Host = "";
	RET(true);
    }
}


unsigned int Nick_Stored_t::Access()
{
    NFT("Nick_Stored_t::Access");
    if (Host() == "")
    {
	RET(i_access.size());
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Access());
    }
}


mstring Nick_Stored_t::Access(unsigned int count)
{
    FT("Nick_Stored_t::Access", (count));
    if (Host() == "")
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
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Access(count));
    }
}


bool Nick_Stored_t::AccessAdd(const mstring& in)
{
    FT("Nick_Stored_t::AccessAdd", (in));
    if (Host() == "")
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
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].AccessAdd(in));
    }
}


unsigned int Nick_Stored_t::AccessDel(mstring in)
{
    FT("Nick_Stored_t::AccessDel", (in));
    if (Host() == "")
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
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].AccessDel(in));
    }
}


bool Nick_Stored_t::IsAccess(mstring in)
{
    FT("Nick_Stored_t::IsAccess", (in));
    if (Host() == "")
    {
	set<mstring>::iterator iter;
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		RET(true);
	    }
	RET(false);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].IsAccess(in));
    }
}


unsigned int Nick_Stored_t::Ignore()
{
    NFT("Nick_Stored_t::Ignore");
    if (Host() == "")
    {
	RET(i_ignore.size());
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Ignore());
    }
}


mstring Nick_Stored_t::Ignore(unsigned int count)
{
    FT("Nick_Stored_t::Ignore", (count));
    if (Host() == "")
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
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Ignore(count));
    }
}


bool Nick_Stored_t::IgnoreAdd(mstring in)
{
    FT("Nick_Stored_t::IgnoreAdd", (in));
    if (Host() == "")
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
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].IgnoreAdd(in));
    }
}


unsigned int Nick_Stored_t::IgnoreDel(mstring in)
{
    FT("Nick_Stored_t::IgnoreDel", (in));
    if (Host() == "")
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
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].IgnoreDel(in));
    }
}


bool Nick_Stored_t::IsIgnore(mstring in)
{
    FT("Nick_Stored_t::IsIgnore", (in));
    if (Host() == "")
    {
	set<mstring>::iterator iter;
	for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		RET(true);
	    }
	RET(false);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].IsIgnore(in));
    }
}


bool Nick_Stored_t::Protect()
{
    NFT("Nick_Stored_t::Protect");
    if (Parent->nickserv.LCK_Protect())
    {
	RET(Parent->nickserv.DEF_Protect());
    }
    else if (Host() == "")
    {
	RET(i_Protect);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Protect());
    }
}


void Nick_Stored_t::Protect(bool in)
{
    FT("Nick_Stored_t::Protect", (in));
    if (Host() == "")
    {
	if (!L_Protect())
	    i_Protect = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Protect(in);
    }
}


bool Nick_Stored_t::L_Protect()
{
    NFT("Nick_Stored_t::L_Protect");
    if (Parent->nickserv.LCK_Protect())
    {
	RET(true);
    }
    else if (Host() == "")
    {
	RET(l_Protect);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_Protect());
    }
}


void Nick_Stored_t::L_Protect(bool in)
{
    FT("Nick_Stored_t::L_Protect", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_Protect())
	    l_Protect = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_Protect(in);
    }
}


bool Nick_Stored_t::Secure()
{
    NFT("Nick_Stored_t::Secure");
    if (Parent->nickserv.LCK_Secure())
    {
	RET(Parent->nickserv.DEF_Secure());
    }
    else if (Host() == "")
    {
	RET(i_Secure);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Secure());
    }
}


void Nick_Stored_t::Secure(bool in)
{
    FT("Nick_Stored_t::Secure", (in));
    if (Host() == "")
    {
	if (!L_Secure())
	    i_Secure = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Secure(in);
    }
}


bool Nick_Stored_t::L_Secure()
{
    NFT("Nick_Stored_t::L_Secure");
    if (Parent->nickserv.LCK_Secure())
    {
	RET(true);
    }
    else if (Host() == "")
    {
	RET(l_Secure);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_Secure());
    }
}


void Nick_Stored_t::L_Secure(bool in)
{
    FT("Nick_Stored_t::L_Secure", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_Secure())
	    l_Secure = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_Secure(in);
    }
}


bool Nick_Stored_t::NoExpire()
{
    NFT("Nick_Stored_t::NoExpire");
    if (Parent->nickserv.LCK_NoExpire())
    {
	RET(Parent->nickserv.DEF_NoExpire());
    }
    else if (Host() == "")
    {
	RET(i_NoExpire);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].NoExpire());
    }
}


void Nick_Stored_t::NoExpire(bool in)
{
    FT("Nick_Stored_t::NoExpire", (in));
    if (Host() == "")
    {
	if (!L_NoExpire())
	    i_NoExpire = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].NoExpire(in);
    }
}


bool Nick_Stored_t::L_NoExpire()
{
    NFT("Nick_Stored_t::L_NoExpire");
    if (Parent->nickserv.LCK_NoExpire())
    {
	RET(true);
    }
    else if (Host() == "")
    {
	RET(l_NoExpire);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_NoExpire());
    }
}


void Nick_Stored_t::L_NoExpire(bool in)
{
    FT("Nick_Stored_t::L_NoExpire", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_NoExpire())
	    l_NoExpire = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_NoExpire(in);
    }
}


bool Nick_Stored_t::NoMemo()
{
    NFT("Nick_Stored_t::NoMemo");
    if (Parent->nickserv.LCK_NoMemo())
    {
	RET(Parent->nickserv.DEF_NoMemo());
    }
    else if (Host() == "")
    {
	RET(i_NoMemo);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].NoMemo());
    }
}


void Nick_Stored_t::NoMemo(bool in)
{
    FT("Nick_Stored_t::NoMemo", (in));
    if (Host() == "")
    {
	if (!L_NoMemo())
	    i_NoMemo = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].NoMemo(in);
    }
}


bool Nick_Stored_t::L_NoMemo()
{
    NFT("Nick_Stored_t::L_NoMemo");
    if (Parent->nickserv.LCK_NoMemo())
    {
	RET(true);
    }
    else if (Host() == "")
    {
	RET(l_NoMemo);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_NoMemo());
    }
}


void Nick_Stored_t::L_NoMemo(bool in)
{
    FT("Nick_Stored_t::L_NoMemo", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_NoMemo())
	    l_NoMemo = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_NoMemo(in);
    }
}


bool Nick_Stored_t::Private()
{
    NFT("Nick_Stored_t::Private");
    if (Parent->nickserv.LCK_Private())
    {
	RET(Parent->nickserv.DEF_Private());
    }
    else if (Host() == "")
    {
	RET(i_Private);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Private());
    }
}


void Nick_Stored_t::Private(bool in)
{
    FT("Nick_Stored_t::Private", (in));
    if (Host() == "")
    {
	if (!L_Private())
	    i_Private = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Private(in);
    }
}


bool Nick_Stored_t::L_Private()
{
    NFT("Nick_Stored_t::L_Private");
    if (Parent->nickserv.LCK_Private())
    {
	RET(true);
    }
    else if (Host() == "")
    {
	RET(l_Private);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_Private());
    }
}


void Nick_Stored_t::L_Private(bool in)
{
    FT("Nick_Stored_t::L_Private", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_Private())
	    l_Private = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_Private(in);
    }
}


bool Nick_Stored_t::PRIVMSG()
{
    NFT("Nick_Stored_t::PRIVMSG");
    if (Parent->nickserv.LCK_PRIVMSG())
    {
	RET(Parent->nickserv.DEF_PRIVMSG());
    }
    else if (Host() == "")
    {
	RET(i_PRIVMSG);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].PRIVMSG());
    }
}


void Nick_Stored_t::PRIVMSG(bool in)
{
    FT("Nick_Stored_t::PRIVMSG", (in));
    if (Host() == "")
    {
	if (!L_PRIVMSG())
	i_PRIVMSG = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].PRIVMSG(in);
    }
}


bool Nick_Stored_t::L_PRIVMSG()
{
    NFT("Nick_Stored_t::L_PRIVMSG");
    if (Parent->nickserv.LCK_PRIVMSG())
    {
	RET(true);
    }
    else if (Host() == "")
    {
	RET(l_PRIVMSG);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_PRIVMSG());
    }
}


void Nick_Stored_t::L_PRIVMSG(bool in)
{
    FT("Nick_Stored_t::L_PRIVMSG", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_PRIVMSG())
	l_PRIVMSG = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_PRIVMSG(in);
    }
}


mstring Nick_Stored_t::Language()
{
    NFT("Nick_Stored_t::Language");
    if (Parent->nickserv.LCK_Language())
    {
	RET(Parent->nickserv.DEF_Language().UpperCase());
    }
    else if (Host() == "")
    {
	RET(i_Language);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Language());
    }
}


void Nick_Stored_t::Language(mstring in)
{
    FT("Nick_Stored_t::Language", (in));
    if (Host() == "")
    {
	if (!L_Language())
	i_Language = in.UpperCase();
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].Language(in);
    }
}


bool Nick_Stored_t::L_Language()
{
    NFT("Nick_Stored_t::L_Language");
    if (Parent->nickserv.LCK_Language())
    {
	RET(true);
    }
    else if (Host() == "")
    {
	RET(l_Language);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].L_Language());
    }
}


void Nick_Stored_t::L_Language(bool in)
{
    FT("Nick_Stored_t::L_Language", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_Language())
	l_Language = in;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_Language(in);
    }
}


bool Nick_Stored_t::Suspended()
{
    NFT("Nick_Stored_t::Suspended");
    if (Host() == "")
    {
	RET(i_Suspend_By != "");
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Suspended());
    }
}


mstring Nick_Stored_t::Suspend_By()
{
    NFT("Nick_Stored_t::Suspend_By");
    if (Host() == "")
    {
	RET(i_Suspend_By);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Suspend_By());
    }
}


mDateTime Nick_Stored_t::Suspend_Time()
{
    NFT("Nick_Stored_t::Suspend_Time");
    if (Host() == "")
    {
	RET(i_Suspend_Time);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].Suspend_Time());
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
    if (Host() == "")
    {
	if (!Parent->nickserv.PicSize())
	{
	    RET(i_Picture);
	}
	RET(0);
    }
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].PicNum())
    }
}


void Nick_Stored_t::GotPic(unsigned long picnum)
{
    FT("Nick_Stored_t::GotPic", (picnum));
    if (Host() == "")
    {
	if (!Parent->nickserv.PicSize())
	    i_Picture = picnum;
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].GotPic(picnum);
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
    else if (Host() == "")
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
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].LastAllSeenTime());
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
    else if (Host() == "")
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
    else
    {
	RET(Parent->nickserv.stored[i_Host.LowerCase()].LastAllMask());
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
		"HOST", Parent->commserv.REGD_Name(), NickServ::do_Host);
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
		"SEND*", Parent->commserv.ALL_Name(), NickServ::do_Send);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"SUSP*", Parent->commserv.SOP_Name(), NickServ::do_Suspend);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"UNSUS*", Parent->commserv.SOP_Name(), NickServ::do_UnSuspend);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"FORB*", Parent->commserv.SOP_Name(), NickServ::do_Forbid);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"GET*PASS*", Parent->commserv.SOP_Name(), NickServ::do_Getpass);
    Parent->commands.AddSystemCommand(GetInternalName(),
		"LIVE*", Parent->commserv.SOP_Name(), NickServ::do_Live);

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
    mynick  = Parent->getLname(data.ExtractWord(3, ": "));
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
    }

    mThread::ReAttach(tt_mBase);
}

void NickServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    mstring HelpTopic = Parent->nickserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.Replace(" ", "/");
    vector<mstring> help = Parent->getHelp(source, HelpTopic.UpperCase());
					
    unsigned int i;
    for (i=0; i<help.size(); i++)
	::send(mynick, source, help[i]);
}

void NickServ::do_Register(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Register", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring password = params.ExtractWord(2, " ");

    if (Parent->nickserv.IsStored(source))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/ISSTORED"));
    }
    else if (password.Len() < 5 || !password.CmpNoCase(source))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/COMPLEX_PASS"));
    }
    else
    {
	Parent->nickserv.stored[source.LowerCase()] = Nick_Stored_t(source, password);
	Parent->nickserv.stored[source.LowerCase()].AccessAdd(
	    Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::U_H).After("!"));
	Parent->nickserv.live[source.LowerCase()].Identify(password);
	Parent->nickserv.stats.i_Register++;
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/REGISTERED"),
		mstring(Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::U_H).After("!")).c_str());
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
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NEED_NICK_IDENT"),
				message.c_str(), mynick.c_str());
	    return;
	}
	else
	{
	    Parent->nickserv.stats.i_Drop++;
	    Parent->nickserv.stored[source.LowerCase()].Drop();
	    Parent->nickserv.stored.erase(source.LowerCase());
	    Parent->nickserv.live[source.LowerCase()].UnIdentify();
	    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/DROPPED"));
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
	    Parent->nickserv.stats.i_Drop++;
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/DROPPED"),
				Parent->getSname(params.ExtractWord(2, " ")).c_str());

	    Parent->nickserv.stored[params.ExtractWord(2, " ").LowerCase()].Drop();
	    Parent->nickserv.stored.erase(params.ExtractWord(2, " ").LowerCase());
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTYOURS"),
				Parent->getSname(params.ExtractWord(2, " ")).c_str());
	}
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
					params.ExtractWord(2, " ").c_str());
    }
}

void NickServ::do_Link(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Link", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring hostnick = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(hostnick))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
							hostnick.c_str());
	return;
    }

    if (Parent->nickserv.stored[hostnick.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
					Parent->getSname(hostnick).c_str());
	return;
    }

    mDateTime regtime = Now();
    if (Parent->nickserv.IsStored(source))
    {
	if (!Parent->nickserv.live[source.LowerCase()].IsIdentified())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NEED_NICK_IDENT"),
					message.c_str(), mynick.c_str());
	    return;
	}
	regtime = Parent->nickserv.stored[source.LowerCase()].RegTime();
    }

    if (Parent->nickserv.stored[hostnick.LowerCase()].Slave(source, password, regtime))
    {
	Parent->nickserv.stats.i_Link++;
	Parent->nickserv.live[source.LowerCase()].Identify(password);
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/LINKED"),
					Parent->getSname(hostnick).c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUAION/NICK_WRONG_PASS"));
    }
}

void NickServ::do_UnLink(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_UnLink", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (!Parent->nickserv.live[source.LowerCase()].IsIdentified())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NEED_NICK_IDENT"),
						message.c_str(), mynick.c_str());
	return;
    }

    if (params.WordCount(" ") > 1)
    {
	mstring target = params.ExtractWord(2, " ");

	
	if (!Parent->nickserv.IsStored(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
						target.c_str());
	    return;
	}

	target = Parent->getSname(target);
	if (Parent->nickserv.stored[target.LowerCase()].Host() == "")
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISHOST"),
							target.c_str());
	    return;
	}

	if (!Parent->nickserv.stored[target.LowerCase()].IsSibling(source))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTYOURS"),
							target.c_str());
	    return;
	}
	
	if (Parent->nickserv.stored[target.LowerCase()].Unlink())
	{
	    Parent->nickserv.stats.i_Unlink++;
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/UNLINKED"),
							target.c_str());
	}
	else
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTLINKED"),
							target.c_str());
    }
    else
    {
	if (Parent->nickserv.stored[source.LowerCase()].Host() == "")
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/ISHOST"));
	    return;
	}

	if (Parent->nickserv.stored[source.LowerCase()].Unlink())
	{
	    Parent->nickserv.stats.i_Unlink++;
	    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/UNLINKED"));
	}
	else
	    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/ISNOTLINKED"));
    }
}

void NickServ::do_Host(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Host", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    mstring newhost;
    if (params.WordCount(" ") > 1)
	newhost = params.ExtractWord(2, " ");

    if (!Parent->nickserv.live[source.LowerCase()].IsIdentified())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NEED_NICK_IDENT"),
						message.c_str(), mynick.c_str());
	return;
    }

    if (newhost != "")
    {
	if (!Parent->nickserv.IsStored(newhost))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
						newhost.c_str());
	    return;
	}

	newhost = Parent->getSname(newhost);
	if (Parent->nickserv.stored[newhost.LowerCase()].Host() == "")
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISHOST"),
						newhost.c_str());
	    return;
	}

	if (!(source.LowerCase() == newhost.LowerCase() ||
		   Parent->nickserv.stored[newhost.LowerCase()].IsSibling(source)))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTLINKED"),
						newhost.c_str());
	    return;
	}

	Parent->nickserv.stats.i_Host++;
	Parent->nickserv.stored[newhost.LowerCase()].MakeHost();
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/NEWHOST"),
						newhost.c_str());
    }
    else
    {
	Parent->nickserv.stats.i_Host++;
	Parent->nickserv.stored[source.LowerCase()].MakeHost();
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/NEWHOST"));
    }
}

void NickServ::do_Slaves(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Slaves", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    mstring target;
    if (params.WordCount(" ") < 2)
    {
	target = source;
    }
    else
    {
	if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		    Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
	    target = params.ExtractWord(2, " ");
	else
	    target = source;
    }

    if (source != target)
    {
	if (!Parent->nickserv.IsStored(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
						target.c_str());
	    return;
	}

	if (Parent->nickserv.stored[target.LowerCase()].Forbidden())
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
					Parent->getSname(target).c_str());
	    return;
	}
    }


    mstring output;
    if (Parent->nickserv.stored[target.LowerCase()].Host() != "" &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[target.LowerCase()].Host()))
	target = Parent->getSname(Parent->nickserv.stored[target.LowerCase()].Host());

    output << IRC_Bold << target << IRC_Off << " (" <<
	Parent->nickserv.stored[target.LowerCase()].Siblings() << "):";

    for (unsigned int i=0; i<Parent->nickserv.stored[target.LowerCase()].Siblings(); i++)
    {
	if (Parent->nickserv.stored[target.LowerCase()].Sibling(i).Len() +
		output.Len() > 510)
	{
	    ::send(mynick, source, output);
	    output = "";
	    output << IRC_Bold << target << IRC_Off << " (" <<
		Parent->nickserv.stored[target.LowerCase()].Siblings() << "):";
	}
	output << " " << Parent->nickserv.stored[target.LowerCase()].Sibling(i);
    }
    ::send(mynick, source, output);
}


void NickServ::do_Identify(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Identify", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    bool wasident = Parent->nickserv.live[source.LowerCase()].IsIdentified();
    mstring password = params.ExtractWord(2, " ");
    mstring output = Parent->nickserv.live[source.LowerCase()].Identify(password);
    if (!wasident &&
	Parent->nickserv.live[source.LowerCase()].IsIdentified())
	Parent->nickserv.stats.i_Identify++;
    if (output != "")
	::send(mynick, source, output);
}

void NickServ::do_Info(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Info", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    unsigned int i;
    mstring target   = params.ExtractWord(2, " ");
    mstring output;
    Nick_Stored_t *nick;
    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
						target.c_str());
	return;
    }
    else
    {
	nick = &Parent->nickserv.stored[target.LowerCase()];
    }

    if (nick->Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						target.c_str());
	return;
    }

	::send(mynick, source, Parent->getMessage(source, "NS_INFO/REALNAME"),
			nick->Name().c_str(), nick->LastRealName().c_str());
    if (nick->Host() != "" && Parent->nickserv.IsStored(nick->Host()))
	::send(mynick, source, Parent->getMessage(source, "NS_INFO/HOST"),
		Parent->nickserv.stored[nick->Host().LowerCase()].Name().c_str());
    output = "";
    if (nick->NoExpire() && Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
	output << " (" << Parent->getMessage(source, "NS_INFO/NOEXPIRE") << ")";
	::send(mynick, source, Parent->getMessage(source, "NS_INFO/REGISTERED"),
		nick->RegTime().Ago().c_str(), output.c_str());

    if (!nick->IsOnline())
    {
	output = "";
	bool isonline = false;
	if (nick->Host() != "" && Parent->nickserv.IsStored(nick->Host())
	    && Parent->nickserv.stored[nick->Host().LowerCase()].IsOnline())
	    output = Parent->nickserv.live[nick->Host().LowerCase()].Name() + " ";
	for (i=0; i<nick->Siblings(); i++)
	{
	    if (Parent->nickserv.IsStored(nick->Sibling(i)) &&
		Parent->nickserv.stored[nick->Sibling(i).LowerCase()].IsOnline())
	    {
		if (output.Len() + nick->Sibling(i).Len() > 450)
		{
		    ::send(mynick, source, Parent->getMessage(source, "NS_INFO/ONLINEAS"),
						output.c_str());
		    output = "";
		    isonline = true;
		}
		output += Parent->nickserv.live[nick->Sibling(i).LowerCase()].Name() + " ";
	    }
	}
	if (output != "")
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_INFO/ONLINEAS"),
						output.c_str());
	    isonline = true;
	}
	if (!isonline)
	{
	    if (!nick->Private() || (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source)))
		::send(mynick, source, Parent->getMessage(source, "NS_INFO/LASTALLMASK"),
						nick->LastAllMask().c_str());
	    ::send(mynick, source, Parent->getMessage(source, "NS_INFO/LASTALLSEEN"),
					nick->LastAllSeenTime().Ago().c_str());
	    if (nick->LastAllMask().UpperCase() !=
		mstring(nick->Name() + "!" + nick->LastMask()).UpperCase() &&
		(!nick->Private() || (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))))
		::send(mynick, source, Parent->getMessage(source, "NS_INFO/LASTMASK"),
					 nick->LastMask().c_str());
	    if (nick->LastAllSeenTime() != nick->LastSeenTime())
		::send(mynick, source, Parent->getMessage(source, "NS_INFO/LASTSEEN"),
					nick->LastSeenTime().Ago().c_str());
	    ::send(mynick, source, Parent->getMessage(source, "NS_INFO/QUITMSG"),
					nick->LastQuit().c_str());
	}
    }
    if (nick->Suspended())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_INFO/SUSPEND"),
					nick->Suspend_Time().Ago().c_str(),
					nick->Suspend_By().c_str());
	::send(mynick, source, Parent->getMessage(source, "NS_INFO/SUSPENDFOR"),
					nick->Comment().c_str());
    }
    else
    {
	if (nick->Email() != "")
	    ::send(mynick, source, Parent->getMessage(source, "NS_INFO/EMAIL"),
					nick->Email().c_str());
	if (nick->URL() != "")
	    ::send(mynick, source, Parent->getMessage(source, "NS_INFO/URL"),
					nick->URL().c_str());
	if (nick->ICQ() != "")
	    ::send(mynick, source, Parent->getMessage(source, "NS_INFO/ICQ"),
					nick->ICQ().c_str());
	if (nick->Description() != "")
	    ::send(mynick, source, Parent->getMessage(source, "NS_INFO/DESCRIPTION"),
					nick->Description().c_str());
	if (nick->Comment() != "" && Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	    Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
	    ::send(mynick, source, Parent->getMessage(source, "NS_INFO/COMMENT"),
					nick->Comment().c_str());
    }

    output = "";
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
		::send(mynick, source, Parent->getMessage(source, "NS_INFO/COMMITTEES"),
						output.c_str());
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
	::send(mynick, source, Parent->getMessage(source, "NS_INFO/COMMITTEES"),
					output.c_str());

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
	output << Parent->getMessage(source, "NS_SET/PROTECT");
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
	output << Parent->getMessage(source, "NS_SET/SECURE");
	if (nick->L_Secure())
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
	output << Parent->getMessage(source, "NS_SET/NOMEMO");
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
	output << Parent->getMessage(source, "NS_SET/PRIVATE");
	if (nick->L_Private())
	    output << IRC_Off;
    }

    if (output != "")
	::send(mynick, source, Parent->getMessage(source, "NS_INFO/OPTIONS"),
						output.c_str());
    if (nick->PicNum())
	::send(mynick, source, Parent->getMessage(source, "NS_INFO/HASPIC"),
				mynick.c_str(), nick->Name().c_str());
    if (nick->IsOnline())
	::send(mynick, source,  Parent->getMessage(source, "NS_INFO/ISONLINE"),
		Parent->getLname(nick->Name()).c_str());
}

void NickServ::do_Ghost(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Ghost", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nick = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nick))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
						nick.c_str());
	return;
    }

    if (!Parent->nickserv.IsLive(nick))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
						nick.c_str());
	return;
    }

    if (pass != Parent->nickserv.stored[nick.LowerCase()].Password())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NICK_WRONG_PASS"));
	return;
    }

    nick = Parent->getLname(nick);
    Parent->server.KILL(mynick, nick, source + " (" +
				Parent->getMessage(nick, "MISC/KILL_GHOST") + ")");
    if (Parent->nickserv.recovered.find(nick.LowerCase()) !=
				Parent->nickserv.recovered.end())
	Parent->nickserv.recovered.erase(nick.LowerCase());
    Parent->nickserv.stats.i_Ghost++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/RELEASED"),
				nick.c_str());
}

void NickServ::do_Recover(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Recover", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nick = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nick))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nick.c_str());
	return;
    }

    if (pass != Parent->nickserv.stored[nick.LowerCase()].Password())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NICK_WRONG_PASS"));
	return;
    }

    if (Parent->nickserv.IsLive(nick))
    {
	Parent->server.KILL(mynick, Parent->getLname(nick), source + " (" +
			Parent->getMessage(nick, "MISC/KILL_RECOVER") + ")");
    }

    Parent->server.NICK(nick, (Parent->startup.Ownuser() ? nick.LowerCase() :
				Parent->startup.Services_User()),
				Parent->startup.Services_Host(),
				Parent->startup.Server_Name(),
				"Nickname Enforcer");
    Parent->nickserv.recovered[nick.LowerCase()] = Now();
    Parent->nickserv.stats.i_Recover++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/HELD"),
				nick.c_str());
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
	    ::send(mynick, source, Parent->getMessage(source, "LIST/MAXLIST"),
					Parent->config.Maxlist());
	    return;
	}
    }

    ::send(mynick, source, Parent->getMessage(source, "LIST/NICK_LIST"),
					mask.c_str());
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
    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAYED"),
							i, count);
}

void NickServ::do_Send(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Send", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target   = params.ExtractWord(2, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
							target.c_str());
	return;
    }
    target = Parent->getSname(target);

}

void NickServ::do_Suspend(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Suspend", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target   = params.ExtractWord(2, " ");
    mstring reason   = params.After(" ", 2);

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
							target.c_str());
	return;
    }

    target = Parent->getSname(target);
    if (Parent->nickserv.stored[target.LowerCase()].Suspended())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISSUSPENDED"),
							target.c_str());
	return;
    }

    if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsIn(target))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOTONCOMMITTEE"),
				message.c_str(), Parent->commserv.OPER_Name().c_str());
	return;
    }

    Parent->nickserv.stored[target.LowerCase()].Suspend(source, reason);
    Parent->nickserv.stats.i_Suspend++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/SUSPENDED"),
						target.c_str());
}

void NickServ::do_UnSuspend(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_UnSuspend", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
							target.c_str());
	return;
    }

    target = Parent->getSname(target);
    if (!Parent->nickserv.stored[target.LowerCase()].Suspended())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSUSPENDED"),
							target.c_str());
	return;
    }

    Parent->nickserv.stored[target.LowerCase()].UnSuspend();
    Parent->nickserv.stats.i_Unsuspend++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/UNSUSPENDED"),
						target.c_str());
}

void NickServ::do_Forbid(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Forbid", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target   = params.ExtractWord(2, " ");
    if (Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISSTORED"),
					Parent->getSname(target).c_str());
	return;
    }

    Parent->nickserv.stored[target.LowerCase()] = Nick_Stored_t(target);
    Parent->nickserv.stats.i_Forbid++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/FORBIDDEN"),
					target.c_str());
}


void NickServ::do_Getpass(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Getpass", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target   = params.ExtractWord(2, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
						target.c_str());
	return;
    }

    target = Parent->getSname(target);
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOTONPRIVCOMMITTEE"),
						message.c_str());
	return;
    }

    Nick_Stored_t *nick = &Parent->nickserv.stored[target.LowerCase()];
    mstring host = nick->Name();
    if (nick->Host() != "" && Parent->nickserv.IsStored(nick->Host()))
	host = Parent->getSname(nick->Host());

    Parent->nickserv.stats.i_Getpass++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/GETPASS"),
			nick->Name().c_str(), host.c_str(), nick->Password().c_str());
    announce(mynick, Parent->getMessage("MISC/NICK_GETPASS"),
			source.c_str(), nick->Name().c_str(), host.c_str());
}


void NickServ::do_Live(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Live", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	mask = "*!*@*";
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
	    ::send(mynick, source, Parent->getMessage(source, "LIST/MAXLIST"),
					Parent->config.Maxlist());
	    return;
	}
    }

    if (!mask.Contains("@") && !mask.Contains("!"))
	mask += "!*@*";
    else if (!mask.Contains("@"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAIN"),
				Parent->getMessage(source, "LIST/NICKNAME").c_str(), '@');
	return;
    }
    else if (!mask.Contains("!"))
	mask.Prepend("*!");

    ::send(mynick, source, Parent->getMessage(source, "LIST/NICK_LIST"),
					mask.c_str());
    map<mstring, Nick_Live_t>::iterator iter;

    for (iter = Parent->nickserv.live.begin(), i=0, count = 0;
			iter != Parent->nickserv.live.end(); iter++)
    {
	if (iter->second.Mask(Nick_Live_t::N_U_P_H).LowerCase().Matches(mask))
	{
	    if (i < listsize)
	    {
		::send(mynick, source, iter->second.Mask(Nick_Live_t::N_U_P_H) + " (" +
					((iter->second.Server() != "") ?
						iter->second.Server() :
						Parent->startup.Server_Name()) +
					((iter->second.Squit() != "") ? " (SQUIT)" : "") +
					"): +" + iter->second.Mode());
		i++;
	    }
	    count++;
	}
    }
    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAYED"),
							i, count);
}


void NickServ::do_access_Current(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_access_Current", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    if (Parent->nickserv.stored[source.LowerCase()].AccessAdd(
	    Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::U_H).After("!")))
    {
	Parent->nickserv.stats.i_Access++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD"),
		mstring(Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::U_H).After("!")).c_str(),
		Parent->getMessage(source, "LIST/ACCESS").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EXISTS"),
		mstring(Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::U_H).After("!")).c_str(),
		Parent->getMessage(source, "LIST/ACCESS").c_str());
    }
}

void NickServ::do_access_Add(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_access_Add", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring hostmask = params.ExtractWord(3, " ");

    if (hostmask.Contains("!"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
				Parent->getMessage(source, "LIST/ACCESS").c_str(), '!');
	return;
    }

    if (!hostmask.Contains("@"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAIN"),
				Parent->getMessage(source, "LIST/ACCESS").c_str(), '@');
	return;
    }

    if (Parent->nickserv.stored[source.LowerCase()].AccessAdd(hostmask))
    {
	Parent->nickserv.stats.i_Access++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD"),
			hostmask.c_str(), Parent->getMessage(source, "LIST/ACCESS").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EXISTS"),
			hostmask.c_str(), Parent->getMessage(source, "LIST/ACCESS").c_str());
    }
}

void NickServ::do_access_Del(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_access_Del", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring hostmask = params.ExtractWord(3, " ");

    if (hostmask.IsNumber())
    {
	if (hostmask.Contains(".") || hostmask.Contains("-"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER"));
	    return;
	}

	unsigned int num = atoi(hostmask.c_str());
	if (num <= 0 || num > Parent->nickserv.stored[source.LowerCase()].Access())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
			1, Parent->nickserv.stored[source.LowerCase()].Access());
	    return;
	}

	hostmask = Parent->nickserv.stored[source.LowerCase()].Access(num-1);
    }

    if (hostmask.Contains("!"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MAYNOTCONTAIN"),
				Parent->getMessage(source, "LIST/ACCESS").c_str(), '!');
	return;
    }

    if (!hostmask.Contains("@"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAIN"),
				Parent->getMessage(source, "LIST/ACCESS").c_str(), '@');
	return;
    }

    unsigned int count;
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (count = Parent->nickserv.stored[source.LowerCase()].AccessDel(hostmask))
    {
	Parent->nickserv.stats.i_Access++;
	::send(mynick, source, Parent->getMessage(source, "LIST/DEL_MATCH"),
			count, hostmask.c_str(),
			Parent->getMessage(source, "LIST/ACCESS").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS"),
			hostmask.c_str(), Parent->getMessage(source, "LIST/ACCESS").c_str());
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
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
					target.c_str());
	return;
    }

    target = Parent->getSname(target);
    if (Parent->nickserv.stored[target.LowerCase()].Access())
    {
	if (source != target)
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY2"),
			target.c_str(), Parent->getMessage(source, "LIST/ACCESS").c_str());
	else
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY"),
			Parent->getMessage(source, "LIST/ACCESS").c_str());
    }
    else
    {
	if (source != target)
	    ::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY2"),
			target.c_str(), Parent->getMessage(source, "LIST/ACCESS").c_str());
	else
	    ::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY"),
			Parent->getMessage(source, "LIST/ACCESS").c_str());
	return;
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
						target.c_str());
	return;
    }

    if (Parent->nickserv.stored[source.LowerCase()].IgnoreAdd(target))
    {
	Parent->nickserv.stats.i_Ignore++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD"),
			target.c_str(), Parent->getMessage(source, "LIST/IGNORE").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EXISTS"),
			target.c_str(), Parent->getMessage(source, "LIST/IGNORE").c_str());
    }
}

void NickServ::do_ignore_Del(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_ignore_Del", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target = params.ExtractWord(3, " ");

    if (target.IsNumber())
    {
	if (target.Contains(".") || target.Contains("-"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER"));
	    return;
	}

	unsigned int num = atoi(target.c_str());
	if (num <= 0 || num > Parent->nickserv.stored[source.LowerCase()].Ignore())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
			1, Parent->nickserv.stored[source.LowerCase()].Ignore());
	    return;
	}

	target = Parent->nickserv.stored[source.LowerCase()].Ignore(num-1);
    }
    unsigned int count;

    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (count = Parent->nickserv.stored[source.LowerCase()].IgnoreDel(target))
    {
	Parent->nickserv.stats.i_Ignore++;
	::send(mynick, source, Parent->getMessage(source, "LIST/DEL_MATCH"),
			count, target.c_str(),
			Parent->getMessage(source, "LIST/IGNORE").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS"),
			target.c_str(), Parent->getMessage(source, "LIST/IGNORE").c_str());
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
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				target.c_str());
	return;
    }

    target = Parent->getSname(target);
    if (Parent->nickserv.stored[target.LowerCase()].Ignore())
    {
	if (source != target)
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY2"),
			target.c_str(), Parent->getMessage(source, "LIST/IGNORE").c_str());
	else
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY"),
			Parent->getMessage(source, "LIST/IGNORE").c_str());
    }
    else
    {
	if (source != target)
	    ::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY2"),
			target.c_str(), Parent->getMessage(source, "LIST/IGNORE").c_str());
	else
	    ::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY"),
			Parent->getMessage(source, "LIST/IGNORE").c_str());
	return;
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
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring newpass = params.ExtractWord(3, " ");

    if (!Parent->nickserv.live[source.LowerCase()].IsIdentified())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NEED_NICK_IDENT"),
			message.c_str(), mynick.c_str());
	return;
    }

    mstring oldpass = Parent->nickserv.stored[source.LowerCase()].Password();
    if (newpass.CmpNoCase(oldpass) == 0 || newpass.CmpNoCase(source) == 0 ||
	newpass.Len() < 5)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/COMPLEX_PASS"));
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].Password(newpass);
    Parent->nickserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/PASSWORD").c_str(), newpass.c_str());
}

void NickServ::do_set_Email(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Email", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.CmpNoCase("none") == 0)
	newvalue = "";

    Parent->nickserv.stored[source.LowerCase()].Email(newvalue);
    Parent->nickserv.stats.i_Set++;
    if (newvalue != "")
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/EMAIL").c_str(), newvalue.c_str());
    else
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/UNSET"),
		Parent->getMessage(source, "NS_SET/EMAIL").c_str());
}

void NickServ::do_set_URL(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_URL", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.CmpNoCase("none") == 0)
	newvalue = "";

    Parent->nickserv.stored[source.LowerCase()].URL(newvalue);
    Parent->nickserv.stats.i_Set++;
    if (newvalue != "")
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/URL").c_str(), newvalue.c_str());
    else
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/UNSET"),
		Parent->getMessage(source, "NS_SET/URL").c_str());
}

void NickServ::do_set_ICQ(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_ICQ", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.CmpNoCase("none") == 0)
	newvalue = "";

    Parent->nickserv.stored[source.LowerCase()].ICQ(newvalue);
    Parent->nickserv.stats.i_Set++;
    if (newvalue != "")
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/ICQ").c_str(), newvalue.c_str());
    else
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/UNSET"),
		Parent->getMessage(source, "NS_SET/ICQ").c_str());
}

void NickServ::do_set_Description(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Description", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring newvalue = params.After(" ", 2);

    if (newvalue.CmpNoCase("none") == 0)
	newvalue = "";

    Parent->nickserv.stored[source.LowerCase()].Description(newvalue);
    Parent->nickserv.stats.i_Set++;
    if (newvalue != "")
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/DESCRIPTION").c_str(), newvalue.c_str());
    else
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/UNSET"),
		Parent->getMessage(source, "NS_SET/DESCRIPTION").c_str());
}

void NickServ::do_set_Comment(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Comment", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring target = params.ExtractWord(3, " ");
    mstring comment = params.After(" ", 3);

    if (!Parent->nickserv.IsStored(target))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
		target.c_str());
	return;
    }

    if (comment.CmpNoCase("none") == 0)
	comment = "";

    Parent->nickserv.stored[target.LowerCase()].Comment(comment);
    Parent->nickserv.stats.i_Set++;
    if (comment != "")
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/COMMENT").c_str(),
		target.c_str(), comment.c_str());
    else
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/UNSET"),
		Parent->getMessage(source, "NS_SET/COMMENT").c_str(), target.c_str());
}

void NickServ::do_set_Picture(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Picture", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    if (params.WordCount(" ") > 1 &&
	params.ExtractWord(2, " ").CmpNoCase("NONE")==0)
    {
	Parent->nickserv.stored[source.LowerCase()].GotPic(0u);
    }
    else
    {
	Parent->nickserv.live[source.LowerCase()].InFlight.Picture(mynick);
    }
}

void NickServ::do_set_Protect(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Protect", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_Protect())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/PROTECT").c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].Protect(onoff.GetBool());
    Parent->nickserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/OPT_SET_TO"),
			Parent->getMessage(source, "NS_SET/PROTECT").c_str(),
			onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}

void NickServ::do_set_Secure(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Secure", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_Secure())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/SECURE").c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].Secure(onoff.GetBool());
    Parent->nickserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/OPT_SET_TO"),
			Parent->getMessage(source, "NS_SET/SECURE").c_str(),
			onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}

void NickServ::do_set_NoExpire(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_NoExpire", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
		nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_NoExpire())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/NOEXPIRE").c_str(),
				nickname.c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_NoExpire())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].NoExpire(onoff.GetBool());
    Parent->nickserv.stats.i_NoExpire++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_SET_TO"),
			Parent->getMessage(source, "NS_SET/NOEXPIRE").c_str(),
			nickname.c_str(), onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}


void NickServ::do_set_NoMemo(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_NoMemo", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_NoMemo())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/NOMEMO").c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].NoMemo(onoff.GetBool());
    Parent->nickserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/OPT_SET_TO"),
			Parent->getMessage(source, "NS_SET/NOMEMO").c_str(),
			onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}

void NickServ::do_set_Private(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Private", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_Private())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/PRIVATE").c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].Private(onoff.GetBool());
    Parent->nickserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/OPT_SET_TO"),
			Parent->getMessage(source, "NS_SET/PRIVATE").c_str(),
			onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}

void NickServ::do_set_PRIVMSG(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_PRIVMSG", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    if (Parent->nickserv.stored[source.LowerCase()].L_PRIVMSG())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/PRIVMSG").c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].PRIVMSG(onoff.GetBool());
    Parent->nickserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/OPT_SET_TO"),
			Parent->getMessage(source, "NS_SET/PRIVMSG").c_str(),
			onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}

void NickServ::do_set_Language(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring lang = params.ExtractWord(3, " ").UpperCase();

    if (Parent->nickserv.stored[source.LowerCase()].L_Language())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/LANGUAGE").c_str());
	return;
    }

    if (!lang.CmpNoCase("default") || !lang.CmpNoCase("reset"))
    {
	lang = Parent->nickserv.DEF_Language().UpperCase();
    }
    else
    {
	wxFileConfig fconf("magick","",wxGetCwd()+DirSlash+"lang"+DirSlash+lang.LowerCase()+".lng");
	// check for valid language ...
	if (!fconf.GetNumberOfGroups())
	{
	    ::send(mynick, source, Parent->getMessage(source, "OS_STATUS/NOLANG"),
			lang.c_str());
	    return;
	}
    }

    Parent->nickserv.stored[source.LowerCase()].Language(lang);
    Parent->nickserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/SET_TO"),
			Parent->getMessage(source, "NS_SET/LANGUAGE").c_str(),
			mstring(lang + " (" +
			Parent->getMessage(source, "ERR_SYNTAX/TRANSLATED") + ")").c_str());
}

void NickServ::do_lock_Protect(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_Protect", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_Protect())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/PROTECT").c_str(),
				nickname.c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Protect(false);
    Parent->nickserv.stored[nickname.LowerCase()].Protect(onoff.GetBool());
    Parent->nickserv.stored[nickname.LowerCase()].L_Protect(true);
    Parent->nickserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_LOCKED"),
			Parent->getMessage(source, "NS_SET/PROTECT").c_str(),
			nickname.c_str(), onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}

void NickServ::do_lock_Secure(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_Secure", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_Secure())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/SECURE").c_str(),
				nickname.c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Secure(false);
    Parent->nickserv.stored[nickname.LowerCase()].Secure(onoff.GetBool());
    Parent->nickserv.stored[nickname.LowerCase()].L_Secure(true);
    Parent->nickserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_LOCKED"),
			Parent->getMessage(source, "NS_SET/SECURE").c_str(),
			nickname.c_str(), onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}

void NickServ::do_lock_NoMemo(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_NoMemo", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_NoMemo())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/NOMEMO").c_str(),
				nickname.c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_NoMemo(false);
    Parent->nickserv.stored[nickname.LowerCase()].NoMemo(onoff.GetBool());
    Parent->nickserv.stored[nickname.LowerCase()].L_NoMemo(true);
    Parent->nickserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_LOCKED"),
			Parent->getMessage(source, "NS_SET/NOMEMO").c_str(),
			nickname.c_str(), onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}

void NickServ::do_lock_Private(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_Private", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_Private())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/PRIVATE").c_str(),
				nickname.c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Private(false);
    Parent->nickserv.stored[nickname.LowerCase()].Private(onoff.GetBool());
    Parent->nickserv.stored[nickname.LowerCase()].L_Private(true);
    Parent->nickserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_LOCKED"),
			Parent->getMessage(source, "NS_SET/PRIVATE").c_str(),
			nickname.c_str(), onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}

void NickServ::do_lock_PRIVMSG(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_PRIVMSG", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff    = params.ExtractWord(4, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_PRIVMSG())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/PRIVMSG").c_str(),
				nickname.c_str());
	return;
    }

    if (onoff.CmpNoCase("default") == 0 || onoff.CmpNoCase("reset") == 0)
    {
	if (Parent->nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_PRIVMSG(false);
    Parent->nickserv.stored[nickname.LowerCase()].PRIVMSG(onoff.GetBool());
    Parent->nickserv.stored[nickname.LowerCase()].L_PRIVMSG(true);
    Parent->nickserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_LOCKED"),
			Parent->getMessage(source, "NS_SET/PRIVMSG").c_str(),
			nickname.c_str(), onoff.GetBool() ?
				Parent->getMessage(source, "MISC/ON").c_str() :
				Parent->getMessage(source, "MISC/OFF").c_str());
}

void NickServ::do_lock_Language(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring lang     = params.ExtractWord(4, " ").UpperCase();

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_Language())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/LANGUAGE").c_str(),
				nickname.c_str());
	return;
    }

    if (!lang.CmpNoCase("default") || !lang.CmpNoCase("reset"))
    {
	lang = Parent->nickserv.DEF_Language().UpperCase();
    }
    else
    {
	wxFileConfig fconf("magick","",wxGetCwd()+DirSlash+"lang"+DirSlash+lang.LowerCase()+".lng");
	// check for valid language ...
	if (!fconf.GetNumberOfGroups())
	{
	    ::send(mynick, source, Parent->getMessage(source, "OS_STATUS/NOLANG"),
			lang.c_str());
	    return;
	}
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Language(false);
    Parent->nickserv.stored[nickname.LowerCase()].Language(lang);
    Parent->nickserv.stored[nickname.LowerCase()].L_Language(true);
    Parent->nickserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/LOCKED"),
			Parent->getMessage(source, "NS_SET/LANGUAGE").c_str(),
			nickname.c_str(), mstring(lang + " (" +
			Parent->getMessage(source, "ERR_SYNTAX/TRANSLATED") + ")").c_str());
}

void NickServ::do_unlock_Protect(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_Protect", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_Protect())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/PROTECT").c_str(),
				nickname.c_str());
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Protect(false);
    Parent->nickserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_UNLOCKED"),
			Parent->getMessage(source, "NS_SET/PROTECT").c_str(),
			nickname.c_str());
}

void NickServ::do_unlock_Secure(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_Secure", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_Secure())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/SECURE").c_str(),
				nickname.c_str());
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Secure(false);
    Parent->nickserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_UNLOCKED"),
			Parent->getMessage(source, "NS_SET/SECURE").c_str(),
			nickname.c_str());
}

void NickServ::do_unlock_NoMemo(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_NoMemo", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_NoMemo())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/NOMEMO").c_str(),
				nickname.c_str());
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_NoMemo(false);
    Parent->nickserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_UNLOCKED"),
			Parent->getMessage(source, "NS_SET/NOMEMO").c_str(),
			nickname.c_str());
}

void NickServ::do_unlock_Private(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_Private", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_Private())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/PRIVATE").c_str(),
				nickname.c_str());
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Private(false);
    Parent->nickserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_UNLOCKED"),
			Parent->getMessage(source, "NS_SET/PRIVATE").c_str(),
			nickname.c_str());
}

void NickServ::do_unlock_PRIVMSG(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_PRIVMSG", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_PRIVMSG())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/PRIVMSG").c_str(),
				nickname.c_str());
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_PRIVMSG(false);
    Parent->nickserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/OPT_UNLOCKED"),
			Parent->getMessage(source, "NS_SET/PRIVMSG").c_str(),
			nickname.c_str());
}

void NickServ::do_unlock_Language(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_unlock_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Parent->nickserv.IsStored(nickname))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nickname.c_str());
	return;
    }

    nickname = Parent->getSname(nickname);
    if (Parent->nickserv.LCK_Language())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISLOCKED"),
				Parent->getMessage(source, "NS_SET/LANGUAGE").c_str(),
				nickname.c_str());
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Language(false);
    Parent->nickserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/UNLOCKED"),
			Parent->getMessage(source, "NS_SET/LANGUAGE").c_str(),
			nickname.c_str());
}

void NickServ::save_database(wxOutputStream& out)
{
    FT("NickServ::save_database", ("(wxOutputStream &) out"));
	//
	map<mstring,Nick_Stored_t>::iterator i;
	CP(("Saving NICK entries (%d) ...", stored.size()));
	out<<stored.size();
        // todo call script saving hooks.
	for(i=stored.begin();i!=stored.end();i++)
	{
	    out<<i->second;
	    // todo call script saving hooks.
	    COM(("Entry NICK %s saved ...", i->second.Name().c_str()));
	}
}

void NickServ::load_database(wxInputStream& in)
{
    FT("NickServ::load_database", ("(wxInputStream &) in"));
    map<mstring,Nick_Stored_t>::size_type i,count;
    in>>count;
    CP(("Loading NICK entries (%d) ...", count));
    stored.clear();
    Nick_Stored_t tmpstored;
    for(i=0;i<count;i++)
    {
	COM(("Loading NICK entry %d ...", i));
	in>>tmpstored;
	stored[tmpstored.Name().LowerCase()]=tmpstored;
	COM(("Entry NICK %s loaded ...", tmpstored.Name().c_str()));
    }

    // Go through the map and populate 'slaves',
    // clean up if nessicary.
    map<mstring,Nick_Stored_t>::iterator iter;
    CP(("Linking nickname entries ..."));
    for (iter=stored.begin(); iter!=stored.end(); iter++)
    {
	if (IsStored(iter->second.i_Host))
	{
	    COM(("Nickname %s has been linked to %s ...",
		iter->first.c_str(), iter->second.i_Host.c_str()));
	    stored[iter->second.i_Host.LowerCase()].i_slaves.insert(iter->first);
	}
	else if (iter->second.i_Host != "")
	{
	    wxLogWarning("Nick %s was listed as host of %s, but did not exist!!",
		iter->second.i_Host.c_str(), iter->first.c_str());
	    iter->second.i_Host = "";
	}
    }
}
