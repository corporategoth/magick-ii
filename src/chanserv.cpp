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

#include "chanserv.h"
#include "lockable.h"
#include "magick.h"
#include "cryptstream.h"


// Private functions

void Chan_Live_t::Join(mstring nick)
{
    FT("Chan_Live_t::Join", (nick));
    if (users.find(nick.LowerCase())!=users.end())
	wxLogWarning("Duplicate JOIN message for %s in %s received.", nick.c_str(), i_Name.c_str());
    else
    {
	users[nick.LowerCase()] = pair<bool,bool>(false,false);
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].Join(nick);
    }
}

int Chan_Live_t::Part(mstring nick)
{
    FT("Chan_Live_t::Part", (nick));
    if (users.find(nick.LowerCase())==users.end())
	wxLogWarning("PART received for %s who is not in %s.", nick.c_str(), i_Name.c_str());
    else
    {
	users.erase(nick.LowerCase());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].Part(nick);
    }

    RET(users.size());
}

int Chan_Live_t::Kick(mstring nick, mstring kicker)
{
    FT("Chan_Live_t::Kick", (nick, kicker));
    if (users.find(nick.LowerCase())==users.end())
	wxLogWarning("KICK from %s received for %s who is not in %s.", kicker.c_str(), nick.c_str(), i_Name.c_str());
    else
    {
	users.erase(nick.LowerCase());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].Kick(nick, kicker);
    }

    RET(users.size());
}

void Chan_Live_t::ChgNick(mstring nick, mstring newnick)
{
    FT("Chan_Live_t::ChgNick", (nick, newnick));
    if (users.find(nick.LowerCase())==users.end())
    {
	wxLogWarning("NICK CHANGE for %s received, and is not in channel.", nick.c_str());
    }
    else
    {
	users[newnick.LowerCase()] = users[nick.LowerCase()];
	users.erase(nick.LowerCase());
    }
}


Chan_Live_t::Chan_Live_t(mstring name, mstring first_user)
{
    FT("Chan_Live_t::Chan_Live_t", (name, first_user));
    i_Name = name;
    users[first_user.LowerCase()] = pair<bool,bool>(false,false);
    if (Parent->chanserv.IsStored(i_Name))
	Parent->chanserv.stored[i_Name.LowerCase()].Join(first_user);
}


void Chan_Live_t::operator=(const Chan_Live_t &in)
{
    NFT("Chan_Live_t::operator=");
    bans.clear();
    set<mstring>::const_iterator i;
    for(i=in.bans.begin();i!=in.bans.end();i++)
	bans.insert(*i);
    i_Creation_Time=in.i_Creation_Time;
    i_Key=in.i_Key;
    i_Limit=in.i_Limit;
    i_Name=in.i_Name;
    i_Topic=in.i_Topic;
    i_Topic_Set_Time=in.i_Topic_Set_Time;
    i_Topic_Setter=in.i_Topic_Setter;
    i_UserDef.clear();
    map<mstring,mstring>::const_iterator j;
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
    modes=in.modes;
    users.clear();
    map<mstring, pair<bool, bool> >::const_iterator k;
    for(k=in.users.begin();k!=in.users.end();k++)
	users.insert(*k);
}


void Chan_Live_t::Topic(mstring topic, mstring setter)
{
    FT("Chan_Live_t::Topic", (topic, setter));
    i_Topic = topic;
    i_Topic_Setter = setter;
    i_Topic_Set_Time = Now();
    if (Parent->chanserv.IsStored(i_Name))
	Parent->chanserv.stored[i_Name.LowerCase()].Topic(i_Topic, i_Topic_Setter, i_Topic_Set_Time);
}


void Chan_Live_t::Topic(mstring topic, mstring setter, mDateTime time)
{
    FT("Chan_Live_t::Topic", (topic, setter, time));
    i_Topic = topic;
    i_Topic_Setter = setter;
    i_Topic_Set_Time = time;
    if (Parent->chanserv.IsStored(i_Name))
	Parent->chanserv.stored[i_Name.LowerCase()].Topic(i_Topic, i_Topic_Setter, i_Topic_Set_Time);
}


int Chan_Live_t::Users()
{
    NFT("Chan_Livt_t::Users");
    RET(users.size());
}


mstring Chan_Live_t::User(int num)
{
    FT("Chan_Live_t::Users", (num));
    int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    for(i=0, k=users.begin();k!=users.end();k++, i++)
	if (i==num)
	{
	    RET(k->first);
	}
 
    RET("");
}


int Chan_Live_t::Ops()
{
    NFT("Chan_Live_t::Ops");
    RET(count_if(users.begin(),users.end(),checkops));
}


mstring Chan_Live_t::Op(int num)
{
    FT("Chan_Live_t::Op", (num));
    int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    for(i=0, k=users.begin();k!=users.end();k++)
	if (IsOp(k->first))
	{
	    if (i==num)
	    {
		RET(k->first);
	    }
	    i++;
	}
 
    RET("");
}


int Chan_Live_t::Voices()
{
    NFT("Chan_Live_t::Voices");
    RET(count_if(users.begin(),users.end(),checkvoices));
}


mstring Chan_Live_t::Voice(int num)
{
    FT("Chan_Live_t::Voice", (num));
    int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    for(i=0, k=users.begin();k!=users.end();k++)
	if (IsVoice(k->first))
	{
	    if (i==num)
	    {
		RET(k->first);
	    }
	    i++;
	}
    RET("");
}



pair<bool,bool> Chan_Live_t::User(mstring name)
{
   FT("Chan_Live_t::User", (name));
   if (IsIn(name))
   {
	NRET(pair<bool.bool>, users[name.LowerCase()]);
   }
   else
   {
	pair<bool,bool> tmp(false,false);
	NRET(pair<bool.bool>, tmp);
   }
}


bool Chan_Live_t::IsIn(mstring nick)
{
    FT("Chan_Live_t::IsIn", (nick));
    RET((users.find(nick.LowerCase()) != users.end()));
}


bool Chan_Live_t::IsOp(mstring nick)
{
    FT("Chan_Live_t::IsOp", (nick));

    if (IsIn(nick))
	if (users[nick.LowerCase()].first == true)
	{
	    RET(true);
	}
    RET(false);
}


bool Chan_Live_t::IsVoice(mstring nick)
{
    FT("Chan_Live_t::IsVoice", (nick));

    if (IsIn(nick))
	if (users[nick.LowerCase()].second == true)
	{
	    RET(true);
	}
    RET(false);

}


void Chan_Live_t::SendMode(mstring source, mstring in)
{
    FT("Chan_Live_t::SendMode", (source, in));
}


void Chan_Live_t::SendMode(mstring in)
{
    SendMode(Parent->chanserv.FirstName(), in);
}


void Chan_Live_t::Mode(mstring source, mstring in)
{
    FT("Chan_Live_t::Mode", (source, in));

    mstring change = in.ExtractWord(1, ": ");
    int fwdargs = 2;
    bool add = true;
    unsigned int i;
    for (i=0; i<change.size(); i++)
    {
	switch(change[i])
	{
	case '+':
	    add = true;
	    break;

	case '-':
	    add = false;
	    break;

	case 'o':
	    if (IsIn(in.ExtractWord(fwdargs, ": ")))
	    {
		if (add)
		    users[in.ExtractWord(fwdargs, ": ").LowerCase()].first = true;
		else
		    users[in.ExtractWord(fwdargs, ": ").LowerCase()].first = false;
	    }
	    else
	    {
		wxLogWarning("Received MODE +o in %s for %s (who is not in channel) by %s",
			i_Name.c_str(), in.ExtractWord(fwdargs, ": ").c_str(), source.c_str());
	    } 
	    fwdargs++;
	    break;

	case 'v':
	    if (IsIn(in.ExtractWord(fwdargs, ": ")))
	    {
		if (add)
		    users[in.ExtractWord(fwdargs, ": ").LowerCase()].second = true;
		else
		    users[in.ExtractWord(fwdargs, ": ").LowerCase()].second = false;
	    }
	    else
	    {
		wxLogWarning("Received MODE +v in %s for %s (who is not in channel) by %s",
			i_Name.c_str(), in.ExtractWord(fwdargs, ": ").c_str(), source.c_str());
	    } 
	    fwdargs++;
	    break;

	case 'b':
	    if (add)
	    {
		bans.insert(in.ExtractWord(fwdargs, ": ").LowerCase());
	    }
	    else
	    {
		bans.erase(in.ExtractWord(fwdargs, ": ").LowerCase());
	    }
	    fwdargs++;
	    break;

	case 'k':
	    if ((i_Key && in.ExtractWord(fwdargs, ": ") == i_Key) || i_Key == "")
	    {
		if (add)
		    i_Key = in.ExtractWord(fwdargs, ": ");
		else
		    i_Key = "";
	    }
	    else
	    {
		wxLogWarning("Incorrect KEY on key-change from %s for %s", source.c_str(), i_Name.c_str());
	    }
	    fwdargs++;
	    break;

	case 'l':
	    if (add)
	    {
		if (fwdargs > in.WordCount(": "))
		{
		    wxLogWarning("No number specified for new LIMIT for %s, set by %s", i_Name.c_str(), source.c_str());
		    i_Limit = 0;
		}
		else if (!in.ExtractWord(fwdargs, ": ").IsNumber())
		{
		    wxLogWarning("New LIMIT for %s is not a number, set by %s", i_Name.c_str(), source.c_str());
		    i_Limit = 0;
		}
		else
		{
		    i_Limit = atoi(in.ExtractWord(fwdargs, ": ").c_str());
		}
	    }
	    else
	    {
		i_Limit = 0;
	    } 
	    fwdargs++;
	    break;

	default:
	    break;
	}
    }

    add = true;
    for (i=0; i<change.size(); i++)
    {
	switch(change[i])
	{
	case '+':
	    add = true;
	    break;

	case '-':
	    add = false;
	    break;

	case 'o':
	case 'v':
	case 'b':
	    break;

	default:
	    if (add && !modes.Contains(change[i]))
	    {
		modes += change[i];
	    }
	    else if (!add && modes.Contains(change[i]))
	    {
		modes.Remove((mstring) change[i]);
	    }
	    else
	    {
		wxLogNotice("MODE change %c%c received from %s for %s that is currently in effect",
			add ? '+' : '-', change[i], source.c_str(), i_Name.c_str());
	    }
	    break;
	}
    }
    if (Parent->chanserv.IsStored(i_Name))
	Parent->chanserv.stored[i_Name.LowerCase()].Mode(source, in);
}


bool checkops(pair<const mstring, pair<bool,bool> > &in)
{
    FT("checkops",(in.first,in.second.first,in.second.second));
    if(in.second.first==true)
    {
	RET(true);
    }
    else
    {
	RET(false);
    }
}


bool checkvoices(pair<const mstring, pair<bool,bool> > &in)
{
    FT("checkvoices",(in.first,in.second.first,in.second.second));
    if(in.second.second==true)
    {
	RET(true);
    }
    else
    {
	RET(false);
    }
}

// --------- end of Chan_Live_t -----------------------------------


void Chan_Stored_t::ChgAttempt(mstring nick, mstring newnick)
{
    FT("Chan_Stored_t::ChgAttempt", (nick, newnick));

    map<mstring, int>::iterator iter;

    // Create a new one if we find the entry
    for (iter=failed_passwds.begin(); iter!=failed_passwds.end(); iter++)
	if (iter->first == nick.LowerCase())
	    failed_passwds[newnick.LowerCase()] = iter->second;
    failed_passwds.erase(nick.LowerCase());
}


void Chan_Stored_t::Join(mstring nick)
{
    FT("Chan_Stored_t::Join", (nick));

    // Ignore us joining
    if(Parent->nickserv.IsLive(nick))
	if (Parent->nickserv.live[nick.LowerCase()].IsServices())
	{
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("+o " + nick);
	    return;
	}

    { // NOONE else can move our iterator
    MLOCK("ChanServ", "Akick", i_Name);
    if (Akick_find(nick))
    {
	// If this user is the only user in channel
	if (Parent->chanserv.IsLive(i_Name))
	    if (Parent->chanserv.live[i_Name.LowerCase()].Users() == 1)
		Parent->server.JOIN(Parent->chanserv.FirstName(), i_Name);

	if (Parent->nickserv.IsLive(Akick->Entry()))
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("+b " +
		Parent->nickserv.live[nick.LowerCase()].Mask(Nick_Live_t::P_H));
	else
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("+b " + Akick->Entry());

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), nick,
		i_Name, Akick->Value());
		
	if (Parent->chanserv.IsLive(i_Name))
	    if (Parent->chanserv.live[i_Name.LowerCase()].Users() == 1)
	    {
		Parent->chanserv.live[i_Name.LowerCase()].SendMode("+s");
		// Activate timer to PART in ? seconds ...
		// Probably should set something in live to say that
		// chanserv is only there to keep the channel (and probably
		// should store the timer ID that will make magick leave)
		// just incase someone else joins before it goes off.
	    }
		

	return;
    }}

    if (Secure() && Restricted() && Access_value(nick) < (long) 1)
    {
	// If this user is the only user in channel
	if (Parent->chanserv.IsLive(i_Name))
	    if (Parent->chanserv.live[i_Name.LowerCase()].Users() == 1)
		Parent->server.JOIN(Parent->chanserv.FirstName(), i_Name);

	if (Parent->nickserv.IsLive(nick))
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("+b " +
		Parent->nickserv.live[nick.LowerCase()].Mask(Nick_Live_t::P_H));
	else
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("+b " + nick + "!*@*");

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), nick,
		i_Name, Parent->chanserv.DEF_Akick_Reason());
		
	if (Parent->chanserv.IsLive(i_Name))
	    if (Parent->chanserv.live[i_Name.LowerCase()].Users() == 1)
	    {
		Parent->chanserv.live[i_Name.LowerCase()].SendMode("+s");
		// Activate timer to PART in ? seconds ...
		// Probably should set something in live to say that
		// chanserv is only there to keep the channel (and probably
		// should store the timer ID that will make magick leave)
		// just incase someone else joins before it goes off.
	    }
		

	return;
    }

    if (Parent->chanserv.IsLive(i_Name))
    {
	if (Access_value(nick) >= Access_Level_value("AUTOOP"))
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("+o " + nick);
	else if (Access_value(nick) >= Access_Level_value("AUTOVOICE"))
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("+v " + nick);
    }

    // Carry over topic ..
    if (Parent->chanserv.IsLive(i_Name))
    {
	if (Parent->chanserv.live[i_Name.LowerCase()].Users() == 1 &&
		Keeptopic() && i_Topic != "")
	{
	    if (i_Topic_Setter != "")
	    {
		Parent->server.TOPIC(Parent->chanserv.FirstName(), i_Name,
			i_Topic + "(" + i_Topic_Setter + ")");
	    }
	    else
	    {
		Parent->server.TOPIC(Parent->chanserv.FirstName(), i_Name, i_Topic);
	    }
	}
    }

}


void Chan_Stored_t::Part(mstring nick)
{
    FT("Chan_Stored_t::Part", (nick));
    if (Access_value(nick)>0)
	i_LastUsed = Now();
}


void Chan_Stored_t::Kick(mstring nick, mstring kicker)
{
    FT("Chan_Stored_t::Kick", (nick, kicker));

    // WE can kick anyone we want
    if (Parent->nickserv.IsLive(kicker))
	if (Parent->nickserv.live[kicker.LowerCase()].IsServices())
	    return;

    if (Access_value(nick)>Access_value(kicker))
    {
	// Check revenge, and do it.
    }
}


void Chan_Stored_t::Topic(mstring topic, mstring setter, mDateTime time)
{
    FT("Chan_Stored_t::Topic", (topic, setter, time));

    // Its us re-setting it!
    if (Parent->nickserv.IsLive(setter))
    {
	if (Parent->nickserv.live[setter.LowerCase()].IsServices())
	    return;
    }

    if (Topiclock())
    {
	Parent->server.TOPIC(Parent->chanserv.FirstName(), i_Name,
		i_Topic + "(" + i_Topic_Setter + ")");
    }
    else
    {
	i_Topic = topic;
	i_Topic_Setter = setter;
	i_Topic_Set_Time = time;
    }
}


void Chan_Stored_t::Mode(mstring setter, mstring mode)
{
    FT("Chan_Stored_t::Mode", (mode));
    // ENFORCE mlock

    if (!Parent->chanserv.IsLive(i_Name))
	return;
    if (Parent->nickserv.IsLive(setter))
	if (Parent->nickserv.live[setter.LowerCase()].IsServices())
	    return;


    mstring send_on;
    mstring send_off;
    mstring send_on_args;
    mstring send_off_args;

    mstring change = mode.ExtractWord(1, ": ");
    int fwdargs = 2;
    bool add = true;
    unsigned int i;
    for (i=0; i<change.size(); i++)
    {
	switch(change[i])
	{
	case '+':
	    add = true;
	    break;

	case '-':
	    add = false;
	    break;

	case 'o':
	    if (add)
	    {
		if (Access_value(mode.ExtractWord(fwdargs, ": ")) <= Access_Level_value("AUTODEOP") ||
			(Access_value(mode.ExtractWord(fwdargs, ": ")) < Access_Level_value("CMDOP") &&
			Secureops()))		
		send_off += "o";
		send_off_args += " " + mode.ExtractWord(fwdargs, ": ");
	    }

	    if ( mstring(send_on_args+send_off_args).WordCount(" ") >= 3)
	    {
		Parent->chanserv.live[i_Name.LowerCase()].SendMode("-" + send_off +
			"+" + send_on + " " + send_off_args + " " + send_on_args);
		send_off = send_on = send_off_args = send_on_args = "";
	    }

	    fwdargs++;
	    break;

	case 'v':
	    if (add)
	    {
		if (Access_value(mode.ExtractWord(fwdargs, ": ")) <= Access_Level_value("AUTODEOP") ||
			(Access_value(mode.ExtractWord(fwdargs, ": ")) < Access_Level_value("CMDVOICE") &&
			Secureops()))
		send_off += "v";
		send_off_args += " " + mode.ExtractWord(fwdargs, ": ");
	    }

	    if ( mstring(send_on_args+send_off_args).WordCount(" ") >= 3)
	    {
		Parent->chanserv.live[i_Name.LowerCase()].SendMode("-" + send_off +
			"+" + send_on + " " + send_off_args + " " + send_on_args);
		send_off = send_on = send_off_args = send_on_args = "";
	    }

	    fwdargs++;
	    break;

	case 'b':
	    if (Access_value(mode.ExtractWord(fwdargs, ": ")) >= Access_value(setter))
	    {
		send_off += "b";
		send_off_args += " " + mode.ExtractWord(fwdargs, ": ");
		// Do Revenge
	    }

	    if ( mstring(send_on_args+send_off_args).WordCount(" ") >= 3)
	    {
		Parent->chanserv.live[i_Name.LowerCase()].SendMode("-" + send_off +
			"+" + send_on + " " + send_off_args + " " + send_on_args);
		send_off = send_on = send_off_args = send_on_args = "";
	    }

	    fwdargs++;
	    break;

	case 'k':
	    if (add && i_Mlock_Off.Contains("k"))
	    {
		send_off += "k";
		send_off_args += " " + mode.ExtractWord(fwdargs, ": ");
	    }
	    else if (!add && i_Mlock_Key != "")
	    {
		send_on += "k";
		send_on_args += " " + i_Mlock_Key;
	    }

	    if ( mstring(send_on_args+send_off_args).WordCount(" ") >= 3)
	    {
		Parent->chanserv.live[i_Name.LowerCase()].SendMode("-" + send_off +
			"+" + send_on + " " + send_off_args + " " + send_on_args);
		send_off = send_on = send_off_args = send_on_args = "";
	    }

	    fwdargs++;
	    break;

	case 'l':
	    if (add && i_Mlock_Off.Contains("l") || i_Mlock_Limit)
	    {
		if (i_Mlock_Limit)
		{
		    send_on += "l";
		    send_on_args << " " << i_Mlock_Limit;
		}
		else
		{
		    send_off = "l";
		}
	    }

	    if ( mstring(send_on_args+send_off_args).WordCount(" ") >= 3)
	    {
		Parent->chanserv.live[i_Name.LowerCase()].SendMode("-" + send_off +
			"+" + send_on + " " + send_off_args + " " + send_on_args);
		send_off = send_on = send_off_args = send_on_args = "";
	    }

	    fwdargs++;
	    break;

	default:
	    if (add && i_Mlock_Off.Contains(change[i]))
		send_off += change[i];
	    else if (!add && i_Mlock_On.Contains(change[i]))
		send_on += change[i];
	}
    }
    if (mstring(send_on+send_off+send_on_args+send_off_args).Len() > 0)
    {
	Parent->chanserv.live[i_Name.LowerCase()].SendMode("-" + send_off +
		"+" + send_on + " " + send_off_args + " " + send_on_args);
	send_off = send_on = send_off_args = send_on_args = "";
    }
}


void Chan_Stored_t::defaults()
{
    NFT("Chan_Stored_t::defaults");

    i_Keeptopic = Parent->chanserv.DEF_Keeptopic();
    i_Topiclock = Parent->chanserv.DEF_Topiclock();
    i_Private = Parent->chanserv.DEF_Private();
    i_Secureops = Parent->chanserv.DEF_Secureops();
    i_Secure = Parent->chanserv.DEF_Secure();
    i_Restricted = Parent->chanserv.DEF_Restricted();
    i_Join = Parent->chanserv.DEF_Join();
    i_Revenge = Parent->chanserv.DEF_Revenge();

    mstring defaulted = Parent->chanserv.DEF_MLock();
    mstring locked = Parent->chanserv.LCK_MLock();
    bool add = true;
    unsigned int i;

    for (i; i<defaulted.size(); i++)
    {
	switch (defaulted[i])
	{
	case '+':
	    add = true;
	    break;
	case '-':
	    add = false;
	    break;
	case 'o':
	case 'v':
	case 'b':
	case 'k':
	case 'l':
	    break;
	default:
	    if (add)
	    {
		if (!i_Mlock_On.Contains(defaulted[i]))
		    i_Mlock_On += defaulted[i];
		if (i_Mlock_Off.Contains(defaulted[i]))
		    i_Mlock_Off.Remove((mstring) defaulted[i]);
	    }
	    else
	    {
		if (!i_Mlock_Off.Contains(defaulted[i]))
		    i_Mlock_Off += defaulted[i];
		if (i_Mlock_On.Contains(defaulted[i]))
		    i_Mlock_On.Remove((mstring) defaulted[i]);
	    }
	    break;
	}
    }

    add = true;
    for (i; i<locked.size(); i++)
    {
	switch (locked[i])
	{
	case '+':
	    add = true;
	    break;
	case '-':
	    add = false;
	    break;
	case 'o':
	case 'v':
	case 'b':
	case 'k':
	case 'l':
	    break;
	default:
	    if (add)
	    {
		if (!i_Mlock_On.Contains(locked[i]))
		    i_Mlock_On += locked[i];
		if (i_Mlock_Off.Contains(locked[i]))
		    i_Mlock_Off.Remove((mstring) locked[i]);
	    }
	    else
	    {
		if (!i_Mlock_Off.Contains(locked[i]))
		    i_Mlock_Off += locked[i];
		if (i_Mlock_On.Contains(locked[i]))
		    i_Mlock_On.Remove((mstring) locked[i]);
	    }
	    break;
	}
    }

    i_Access_Level.insert(entlist_val_t<long>("AUTODEOP",
	Parent->chanserv.LVL_Autodeop(),	Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("AUTOVOICE",
	Parent->chanserv.LVL_Autovoice(),	Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("AUTOOP",
	Parent->chanserv.LVL_Autoop(),		Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("READMEMO",
	Parent->chanserv.LVL_Readmemo(),	Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("WRITEMEMO",
	Parent->chanserv.LVL_Writememo(),	Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("DELMEMO",
	Parent->chanserv.LVL_Delmemo(),		Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("AKICK",
	Parent->chanserv.LVL_Akick(),		Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("STARAKICK",
	Parent->chanserv.LVL_Starakick(),	Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("UNBAN",
	Parent->chanserv.LVL_Unban(),		Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("ACCESS",
	Parent->chanserv.LVL_Access(),		Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("SET",
	Parent->chanserv.LVL_Set(),		Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("CMDINVITE",
	Parent->chanserv.LVL_Cmdinvite(),	Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("CMDUNBAN",
	Parent->chanserv.LVL_Cmdunban(),	Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("CMDVOICE",
	Parent->chanserv.LVL_Cmdvoice(),	Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("CMDOP",
	Parent->chanserv.LVL_Cmdop(),		Parent->chanserv.FirstName()));
    i_Access_Level.insert(entlist_val_t<long>("CMDCLEAR",
	Parent->chanserv.LVL_Cmdclear(),	Parent->chanserv.FirstName()));
    
}


Chan_Stored_t::Chan_Stored_t(mstring name, mstring founder, mstring password, mstring desc)
{
    FT("Chan_Stored_t::Chan_Stored_t", (name, founder, password, desc));
    i_Name = name;
    i_RegTime = Now();
    i_Founder = founder;
    i_Password = password;
    i_Description = desc;
    i_Forbidden = false;

    defaults();
}


Chan_Stored_t::Chan_Stored_t(mstring name)
{
    FT("Chan_Stored_t::Chan_Stored_t", (name));
    i_Name = name;
    i_RegTime = Now();
    i_Forbidden = true;

    defaults();
}


void Chan_Stored_t::operator=(const Chan_Stored_t &in)
{
    NFT("Chan_Stored_t::operator=");
    i_Name=in.i_Name;
    i_RegTime=in.i_RegTime;
    i_Founder=in.i_Founder;
    i_Description=in.i_Description;
    i_Password=in.i_Password;
    i_URL=in.i_URL;

    i_Mlock_On=in.i_Mlock_On;
    i_Mlock_Off=in.i_Mlock_Off;
    i_Mlock_Key=in.i_Mlock_Key;
    i_Mlock_Limit=in.i_Mlock_Limit;

    i_Keeptopic=in.i_Keeptopic;
    i_Topiclock=in.i_Topiclock;
    i_Private=in.i_Private;
    i_Secureops=in.i_Secureops;
    i_Secure=in.i_Secure;
    i_Restricted=in.i_Restricted;
    i_Join=in.i_Join;
    i_Suspend_By=in.i_Suspend_By;
    i_Suspend_Time=in.i_Suspend_Time;
    i_Forbidden=in.i_Forbidden;

//  entlist_val_cui<long> j;
    set<entlist_val_t<long> >::const_iterator j;
    i_Access_Level.clear();
    for(j=in.i_Access_Level.begin();j!=in.i_Access_Level.end();j++)
	i_Access_Level.insert(*j);

    i_Access.clear();
    for(j=in.i_Access.begin();j!=in.i_Access.end();j++)
	i_Access.insert(*j);

//  entlist_val_cui<mstring> k;
    set<entlist_val_t<mstring> >::const_iterator k;
    i_Akick.clear();
    for(k=in.i_Akick.begin();k!=in.i_Akick.end();k++)
	i_Akick.insert(*k);

    entlist_ci l;
    i_Greet.clear();
    for(l=in.i_Greet.begin();l!=in.i_Greet.end();l++)
	i_Greet.push_back(*l);

    i_UserDef.clear();
    map<mstring, mstring>::const_iterator i;
    for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
    i_UserDef.insert(*i);
}


int Chan_Stored_t::CheckPass(mstring nick, mstring password)
{
    FT("Chan_Stored_t::CheckPass", (nick, password));
    if (i_Password == password)
    {
	failed_passwds.erase(nick.LowerCase());
	RET(0);
    }
    else
    {
	failed_passwds[nick.LowerCase()]++;
	RET(failed_passwds[nick.LowerCase()]);
    }
}


void Chan_Stored_t::Suspend(mstring name)
{
    FT("Chan_Stored_t::Suspend", (name));
    i_Suspend_By = name;
    i_Suspend_Time = Now();
}


void Chan_Stored_t::UnSuspend()
{
    NFT("Chan_Stored_t::UnSuspend");
    i_Suspend_By = "";
}


mstring Chan_Stored_t::Mlock()
{
    NFT("Chan_Stored_t::MLock");
    mstring Result;
    if(i_Mlock_On!="")
	Result+="+" + i_Mlock_On;
    if(i_Mlock_Off!="")
	Result+="-" + i_Mlock_Off;
    RET(Result);
}


mstring Chan_Stored_t::Mlock(mstring mode)
{
    FT("Chan_Stored_t::MLock", (mode));

    i_Mlock_On = i_Mlock_Off = i_Mlock_Key = "";
    i_Mlock_Limit = 0;
    mstring retval = "";
    mstring change = mode.ExtractWord(1, ": ");
    int fwdargs = 2;
    bool add = true;
    bool ignorek = false;
    bool ignorel = false;
    unsigned int i;

    if (change.WordCount("k") > 2)
    {
	ignorek = true;
	if (retval != "")
	    retval += ", ";
	retval += "Multiple +/-k modes specified - ignoring";
    }
    if (change.WordCount("l") > 2)
    {
	ignorel = true;
	if (retval != "")
	    retval += ", ";
	retval += "Multiple +/-l modes specified - ignoring";
    }

    for (i=0; i<change.size(); i++)
    {
	switch(change[i])
	{
	case '+':
	    add = true;
	    break;

	case '-':
	    add = false;
	    break;

	case 'o':
	case 'v':
	case 'b':
	    break;

	case 'k':
	    if (!ignorek && add)
	    {
		if (fwdargs > mode.WordCount(": "))
		{
		    if (retval != "")
			retval += ", ";
		    retval += "No key specified for +k - ignoring";
		    fwdargs--;
		}
		else
		{
		    i_Mlock_Key = mode.ExtractWord(fwdargs, ": ");
		}
		fwdargs++;
	    }
	    break;

	case 'l':
	    if (!ignorel && add)
	    {
		if (fwdargs > mode.WordCount(": "))
		{
		    if (retval != "")
			retval += ", ";
		    retval += "No limit specified for +l - ignoring";
		    fwdargs--;
		}
		else if (!mode.ExtractWord(fwdargs, ": ").IsNumber())
		{
		    if (retval != "")
			retval += ", ";
		    retval += "Specified limit is not a number - ignoring";
		}
		else if (atol(mode.ExtractWord(fwdargs, ": ")) < 1)
		{
		    if (retval != "")
			retval += ", ";
		    retval += "Specified limit is less than one - ignoring";
		}
		else
		{
		    i_Mlock_Limit = atol(mode.ExtractWord(fwdargs, ": ").c_str());
		}
		fwdargs++;
	    }
	    break;

	default:
	    break;
	}
    }

    add = true;
    for (i=0; i<change.size(); i++)
    {
	switch(change[i])
	{
	case '+':
	    add = true;
	    break;

	case '-':
	    add = false;
	    break;

	case 'o':
	case 'v':
	case 'b':
	    break;

	default:
	    if (add)
	    {
		if (change[i]=='k' || change[i]=='l')
		    break;
		if (!i_Mlock_On.Contains(change[i]))
		    i_Mlock_On += change[i];
		if (i_Mlock_Off.Contains(change[i]))
		    i_Mlock_Off.Remove((mstring) change[i]);
	    }
	    else
	    {
		if ((change[i]=='k' && ignorek) ||
			(change[i]=='l' && ignorel))
		    break;
		if (!i_Mlock_Off.Contains(change[i]))
		    i_Mlock_Off += change[i];
		if (i_Mlock_On.Contains(change[i]))
		    i_Mlock_On.Remove((mstring) change[i]);
	    }
	    break;
	}
    }

    mstring locked = Parent->chanserv.LCK_MLock();
    mstring override_on;
    mstring override_off;
    mstring forced_on;
    mstring forced_off;
    add = true;
    for (i; i<locked.size(); i++)
    {
	switch (locked[i])
	{
	case '+':
	    add = true;
	    break;
	case '-':
	    add = false;
	    break;
	case 'o':
	case 'v':
	case 'b':
	    break;
	default:
	    if (add)
	    {
		// CANT force mlock +k or +l !!
		if (locked[i] == 'k' || locked[i] == 'l')
		    break;
		if (!i_Mlock_On.Contains(locked[i]))
		{
		    forced_on += locked[i];
		    i_Mlock_On += locked[i];
		}
		if (i_Mlock_Off.Contains(locked[i]))
		{
		    override_off += locked[i];
		    i_Mlock_Off.Remove((mstring) locked[i]);
		}
	    }
	    else
	    {
		if (locked[i] == 'k')
		    i_Mlock_Key = "";
		if (locked[i] == 'l')
		    i_Mlock_Limit = 0;
		if (!i_Mlock_Off.Contains(locked[i]))
		{
		    forced_off += locked[i];
		    i_Mlock_Off += locked[i];
		}
		if (i_Mlock_On.Contains(locked[i]))
		{
		    override_on += locked[i];
		    i_Mlock_On.Remove((mstring) locked[i]);
		}
	    }
	    break;
	}
    }


    if (retval != "")
	retval += ", ";
    if (override_on != "" || override_off != "")
    {
	retval += "MODE LOCK has reversed ";
	if (override_on != "")
	    retval += "+" + override_on;
	if (override_off != "")
	    retval += "-" + override_off;
    }
    if (retval != "")
	retval += ", ";
    if (forced_on != "" || forced_off != "")
    {
	retval += "MODE LOCK has forced ";
	if (forced_on != "")
	    retval += "+" + forced_on;
	if (forced_off != "")
	    retval += "-" + forced_off;
    }
    if (retval != "")
	retval += ", ";
    if (i_Mlock_On != "" || i_Mlock_Off != "")
    {
	retval += "MLOCK has been set to ";
	if(i_Mlock_On!="")
	    retval += "+" + i_Mlock_On;
	if(i_Mlock_Off!="")
	    retval += "-" + i_Mlock_Off;
	if (i_Mlock_Limit)
	    retval << " " << i_Mlock_Limit;
    }
    else
    {
	retval += "MLOCK has been turned off";
    }
    retval += ".";
    RET(retval);
}


void Chan_Stored_t::Keeptopic(bool in)
{
    FT("Chan_Stored_t::KeepTopic", (in));
    if (!Parent->chanserv.LCK_Keeptopic())
	i_Keeptopic = in;
}


bool Chan_Stored_t::Keeptopic()
{
    NFT("Chan_Stored_t::KeepTopic");
    if (!Parent->chanserv.LCK_Keeptopic())
    {
	RET(i_Keeptopic);
    }
    RET(Parent->chanserv.DEF_Keeptopic());
}


void Chan_Stored_t::Topiclock(bool in)
{
    FT("Chan_Stored_t::Topiclock", (in));
    if (!Parent->chanserv.LCK_Topiclock())
	i_Topiclock = in;
}


bool Chan_Stored_t::Topiclock()
{
    NFT("Chan_Stored_t::Topiclock");
    if (!Parent->chanserv.LCK_Topiclock())
    {
	RET(i_Topiclock);
    }
    RET(Parent->chanserv.DEF_Topiclock());
}


void Chan_Stored_t::Private(bool in)
{
    FT("Chan_Stored_t::Private", (in));
    if (!Parent->chanserv.LCK_Private())
	i_Private = in;
}


bool Chan_Stored_t::Private()
{
    NFT("Chan_Stored_t::Private");
    if (!Parent->chanserv.LCK_Private())
    {
	RET(i_Private);
    }
    RET(Parent->chanserv.DEF_Private());
}


void Chan_Stored_t::Secureops(bool in)
{
    FT("Chan_Stored_t::Secureops", (in));
    if (!Parent->chanserv.LCK_Secureops())
	i_Secureops = in;
}


bool Chan_Stored_t::Secureops()
{
    NFT("Chan_Stored_t::Secureops");
    if (!Parent->chanserv.LCK_Secureops())
    {
	RET(i_Secureops);
    }
    RET(Parent->chanserv.DEF_Secureops());
}


void Chan_Stored_t::Secure(bool in)
{
    FT("Chan_Stored_t::Secure", (in));
    if (!Parent->chanserv.LCK_Secure())
	i_Secure = in;
}


bool Chan_Stored_t::Secure()
{
    NFT("Chan_Stored_t::Secure");
    if (!Parent->chanserv.LCK_Secure())
    {
	RET(i_Secure);
    }
    RET(Parent->chanserv.DEF_Secure());
}


void Chan_Stored_t::Restricted(bool in)
{
    FT("Chan_Stored_t::Restricted", (in));
    if (!Parent->chanserv.LCK_Restricted())
	i_Restricted = in;
}


bool Chan_Stored_t::Restricted()
{
    NFT("Chan_Stored_t::Restricted");
    if (!Parent->chanserv.LCK_Restricted())
    {
	RET(i_Restricted);
    }
    RET(Parent->chanserv.DEF_Restricted());
}


void Chan_Stored_t::Join(bool in)
{
    FT("Chan_Stored_t::Join", (in));
    if (!Parent->chanserv.LCK_Join())
	i_Join = in;
}


bool Chan_Stored_t::Join()
{
    NFT("Chan_Stored_t::Join");
    if (!Parent->chanserv.LCK_Join())
    {
	RET(i_Join);
    }
    RET(Parent->chanserv.DEF_Join());
}


bool Chan_Stored_t::Revenge(mstring in)
{
    FT("Chan_Stored_t::Revenge", (in));
    if (!Parent->chanserv.LCK_Revenge())
    {
	// Sanity checks (valid case)
	i_Revenge = in;
	RET(true);
    }
    RET(false);
}


mstring Chan_Stored_t::Revenge()
{
    NFT("Chan_Stored_t::Revenge");
    if (!Parent->chanserv.LCK_Revenge())
    {
	RET(i_Revenge);
    }
    RET(Parent->chanserv.DEF_Revenge());
}


bool Chan_Stored_t::Access_Level_find(mstring entry)
{
    FT("Chan_Stored_t::Access_Level_find", (entry));

//  entlist_val_ui<long> iter = i_Access_Level.end();
    set<entlist_val_t<long> >::iterator iter = i_Access_Level.end();
    if (!i_Access_Level.empty())
	for (iter=i_Access_Level.begin(); iter!=i_Access_Level.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;

    if (iter == i_Access_Level.end())
    {
	Access_Level = iter;
	RET(true);
    }
    else
    {
	Access_Level = i_Access_Level.end();
	RET(false);
    }
}


long Chan_Stored_t::Access_Level_value(mstring entry)
{
    FT("Chan_Stored_t::Access_Level_value", (entry));

    long retval = 0;
//  entlist_val_ui<long> iter = Access_Level;
    set<entlist_val_t<long> >::iterator iter = Access_Level;

    if (Access_Level_find(entry))
	retval=Access_Level->Value();
    Access_Level = iter;
    return retval;
}


bool Chan_Stored_t::Access_insert(mstring entry, long value, mstring nick)
{
    FT("Chan_Stored_t::Access_insert", (entry, nick));

    // Wildcards but no @
    if ((entry.Contains("*") || entry.Contains("?")) &&
	!entry.Contains("@"))
    {
	    RET(false);
    }

    // ! without @
    if (entry.Contains("!") && !entry.Contains("@"))
    {
	    RET(false);
    }

    // Ensure its a stored nick if no @
    // Add *! if its not *!*@*
    if (!entry.Contains("@"))
    {
	if (!Parent->nickserv.IsStored(entry))
	{
	    RET(false);
	}
    }
    else
    {
	if (!entry.Contains("!"))
	    entry.Prepend("*!");
    }

    if (!Access_find(entry))
    {
	entlist_val_t<long> tmp(entry, value, nick);
	Access = i_Access.insert(i_Access.end(), tmp);
	RET(true);
    }
    else
    {
	Access = i_Access.end();
	RET(false);
    }
}


bool Chan_Stored_t::Access_erase()
{
    NFT("Chan_Stored_t::Access_erase");

    if (Access != i_Access.end())
    {
	i_Access.erase(Access);
	Access = i_Access.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


bool Chan_Stored_t::Access_find(mstring entry)
{
    FT("Chan_Stored_t::Access_find", (entry));

    mstring mask;
    if (Parent->nickserv.IsLive(entry))
	mask = Parent->nickserv.live[entry.LowerCase()].Mask(Nick_Live_t::N_U_P_H);

//  entlist_val_ui<long> iter = i_Access.end();
    set<entlist_val_t<long> >::iterator iter = i_Access.end();
    if (!i_Access.empty())
    {
	// FIND exact nickname
	for (iter=i_Access.begin(); iter!=i_Access.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;

	// Not exact, try either REGEX of entry, or
	// REGEX of live nickname if !Contains(@)
	if (iter == i_Access.end())
	{
	    if (entry.Contains("@"))
	    {
		for (iter=i_Access.begin(); iter!=i_Access.end(); iter++)
		    if (entry.LowerCase().Matches(iter->Entry().LowerCase()))
			break;
	    }
	    else
	    {
		if (Parent->nickserv.IsLive(entry))
		{
		    mstring mask = Parent->nickserv.live[entry.LowerCase()].Mask(Nick_Live_t::N_U_P_H);

		    for (iter=i_Access.begin(); iter!=i_Access.end(); iter++)
			if (mask.LowerCase().Matches(iter->Entry().LowerCase()))
			    break;
		}
	    }
	}
    }

    if (iter == i_Access.end())
    {
	Access = iter;
	RET(true);
    }
    else
    {
	Access = i_Access.end();
	RET(false);
    }
}


long Chan_Stored_t::Access_value(mstring entry)
{
    FT("Chan_Stored_t::Access_value", (entry));

    long retval = 0;
//  entlist_val_ui<long> iter = Access;
    set<entlist_val_t<long> >::iterator iter = Access;

    if (Access_find(entry))
	retval=Access->Value();
    Access = iter;
    return retval;
}


bool Chan_Stored_t::Akick_insert(mstring entry, mstring value, mstring nick)
{
    FT("Chan_Stored_t::Akick_insert", (entry, nick));

    // Wildcards but no @
    if ((entry.Contains("*") || entry.Contains("?")) &&
	!entry.Contains("@"))
    {
	    RET(false);
    }

    // ! without @
    if (entry.Contains("!") && !entry.Contains("@"))
    {
	    RET(false);
    }

    // Ensure its a stored nick if no @
    // Add *! if its not *!*@*
    if (!entry.Contains("@"))
    {
	if (!Parent->nickserv.IsStored(entry))
	{
	    RET(false);
	}
    }
    else
    {
	if (!entry.Contains("!"))
	    entry.Prepend("*!");
    }

    if (!Akick_find(entry))
    {
	entlist_val_t<mstring> tmp(entry, value, nick);
	Akick = i_Akick.insert(i_Akick.end(), tmp);
	RET(true);
    }
    else
    {
	Akick = i_Akick.end();
	RET(false);
    }
}

bool Chan_Stored_t::Akick_insert(mstring entry, mstring nick)
{
    return Akick_insert(entry, Parent->chanserv.DEF_Akick_Reason(), nick);
}

bool Chan_Stored_t::Akick_erase()
{
    NFT("Chan_Stored_t::Akick_erase");

    if (Akick != i_Akick.end())
    {
	i_Akick.erase(Akick);
	Akick = i_Akick.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


bool Chan_Stored_t::Akick_find(mstring entry)
{
    FT("Chan_Stored_t::Akick_find", (entry));

    mstring mask;
    if (Parent->nickserv.IsLive(entry))
	mask = Parent->nickserv.live[entry.LowerCase()].Mask(Nick_Live_t::N_U_P_H);

//  entlist_val_ui<mstring> iter = i_Akick.end();
    set<entlist_val_t<mstring> >::iterator iter = i_Akick.end();
    if (!i_Akick.empty())
    {
	// FIND exact nickname
	for (iter=i_Akick.begin(); iter!=i_Akick.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;

	// Not exact, try either REGEX of entry, or
	// REGEX of live nickname if !Contains(@)
	if (iter == i_Akick.end())
	{
	    if (entry.Contains("@"))
	    {
		for (iter=i_Akick.begin(); iter!=i_Akick.end(); iter++)
		    if (entry.LowerCase().Matches(iter->Entry().LowerCase()))
			break;
	    }
	    else
	    {
		if (Parent->nickserv.IsLive(entry))
		{
		    mstring mask = Parent->nickserv.live[entry.LowerCase()].Mask(Nick_Live_t::N_U_P_H);

		    for (iter=i_Akick.begin(); iter!=i_Akick.end(); iter++)
			if (mask.LowerCase().Matches(iter->Entry().LowerCase()))
			    break;
		}
	    }
	}
    }

    if (iter == i_Akick.end())
    {
	Akick = iter;
	RET(true);
    }
    else
    {
	Akick = i_Akick.end();
	RET(false);
    }
}


mstring Chan_Stored_t::Akick_string(mstring entry)
{
    FT("Chan_Stored_t::Akick_string", (entry));

    mstring retval;
//  entlist_val_ui<mstring> iter = Akick;
    set<entlist_val_t<mstring> >::iterator iter = Akick;

    if (Akick_find(entry))
	retval=Akick->Value();
    Akick = iter;
    return retval;
}


bool Chan_Stored_t::Greet_insert(mstring entry, mstring nick)
{
    FT("Chan_Stored_t::Greet_insert", (entry, nick));

    if (!Greet_find(entry))
    {
	entlist_t tmp(entry, nick);
	Greet = i_Greet.insert(i_Greet.end(), tmp);
	RET(true);
    }
    else
    {
	Greet = i_Greet.end();
	RET(false);
    }
}


bool Chan_Stored_t::Greet_erase()
{
    NFT("Chan_Stored_t::Greet_erase");

    if (Greet != i_Greet.end())
    {
	i_Greet.erase(Greet);
	Greet = i_Greet.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


bool Chan_Stored_t::Greet_find(mstring entry)
{
    FT("Chan_Stored_t::Greet_find", (entry));

    entlist_i iter = i_Greet.end();
    if (!i_Greet.empty())
	for (iter=i_Greet.begin(); iter!=i_Greet.end(); iter++)
	    if (entry.LowerCase().Matches(iter->Entry().LowerCase()))
		break;

    if (iter == i_Greet.end())
    {
	Greet = iter;
	RET(true);
    }
    else
    {
	Greet = i_Greet.end();
	RET(false);
    }
}


wxOutputStream &operator<<(wxOutputStream& out,Chan_Stored_t& in)
{
    out<<in.i_Name<<in.i_RegTime<<in.i_Founder<<in.i_Description<<in.i_Password<<in.i_URL<<in.i_Comment;
    out<<in.i_Mlock_On<<in.i_Mlock_Off<<in.i_Mlock_Key<<in.i_Mlock_Limit;
    out<<in.i_Keeptopic<<in.i_Topiclock<<in.i_Private<<in.i_Secureops<<
	in.i_Secure<<in.i_Restricted<<in.i_Join<<in.i_Forbidden;
    out<<in.i_Suspend_By<<in.i_Suspend_Time;

//  entlist_val_cui<long> j;
    set<entlist_val_t<long> >::const_iterator j;
    out<<in.i_Access_Level.size();
    for(j=in.i_Access_Level.begin();j!=in.i_Access_Level.end();j++)
	out<<*j;

    out<<in.i_Access.size();
    for(j=in.i_Access.begin();j!=in.i_Access.end();j++)
	out<<*j;

//  entlist_val_cui<mstring> k;
    set<entlist_val_t<mstring> >::const_iterator k;
    out<<in.i_Akick.size();
    for(k=in.i_Akick.begin();k!=in.i_Akick.end();k++)
	out<<*k;

    entlist_ci l;
    out<<in.i_Greet.size();
    for(l=in.i_Greet.begin();l!=in.i_Greet.end();l++)
	out<<*k;

    map<mstring,mstring>::iterator i;
    out<<in.i_UserDef.size();
    for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
	out<<(mstring)i->first<<(mstring)i->second;
    return out;
}


wxInputStream &operator>>(wxInputStream& in, Chan_Stored_t& out)
{
    set<entlist_val_t<long> >::size_type i,count;
    mstring dummy,dummy2;
    entlist_t edummy;
    entlist_val_t<long> eldummy;
    entlist_val_t<mstring> esdummy;
    in>>out.i_Name>>out.i_RegTime>>out.i_Founder>>out.i_Description>>out.i_Password>>out.i_URL>>out.i_Comment;
    in>>out.i_Mlock_On>>out.i_Mlock_Off>>out.i_Mlock_Key>>out.i_Mlock_Limit;
    in>>out.i_Keeptopic>>out.i_Topiclock>>out.i_Private>>out.i_Secureops>>
	out.i_Secure>>out.i_Restricted>>out.i_Join>>out.i_Forbidden;
    in>>out.i_Suspend_By>>out.i_Suspend_Time;

    out.i_Access_Level.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>eldummy;
	out.i_Access_Level.insert(eldummy);
    }

    out.i_Access.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>edummy;
	out.i_Access.insert(eldummy);
    }

    out.i_Akick.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>esdummy;
	out.i_Akick.insert(esdummy);
    }

    out.i_Greet.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>edummy;
	out.i_Greet.push_back(edummy);
    }

    out.i_UserDef.clear();
    in>>count;
    for(i=0;i<count;i++)
    {
	in>>dummy>>dummy2;
	out.i_UserDef[dummy]=dummy2;
    }
    return in;
}


// --------- end of Chan_Stored_t ---------------------------------

ChanServ::ChanServ()
{
    NFT("ChanServ::ChanServ");
    messages=true;
    automation=true;
}

bool ChanServ::IsLive(mstring in)
{
    FT("ChanServ::IsLive", (in));
    RET(live.find(in.LowerCase())!=live.end());
}

bool ChanServ::IsStored(mstring in)
{
    FT("ChanServ::IsStored", (in));
    RET(stored.find(in.LowerCase())!=stored.end());
}

void ChanServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_ChanServ);
    FT("ChanServ::execute", (data));
    //okay this is the main chanserv command switcher

    mstring source, msgtype, mynick, message;
    source  = data.ExtractWord(1, ": ");
    msgtype = data.ExtractWord(2, ": ").UpperCase();
    mynick  = data.ExtractWord(3, ": ");
    message = data.After(":", 2);


    mThread::ReAttach(tt_mBase);
}

void ChanServ::save_database(wxOutputStream& out)
{
	map<mstring,Chan_Stored_t>::iterator i;
	out<<stored.size();
        // todo call script saving hooks.
	for(i=stored.begin();i!=stored.end();i++)
	{
	    out<<i->second;
	    // todo call script saving hooks.
	}
}

void ChanServ::load_database(wxInputStream& in)
{
    map<mstring,Chan_Stored_t>::size_type i,j;
    in>>i;
    Chan_Stored_t tmpstored;
    for(j=0;j<i;j++)
    {
	in>>tmpstored;
	stored[tmpstored.Name()]=tmpstored;
    }
}
