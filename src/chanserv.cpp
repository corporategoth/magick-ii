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
** Revision 1.152  2000/03/26 14:59:36  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.151  2000/03/24 15:35:17  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.150  2000/03/19 08:50:53  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.149  2000/03/14 15:10:15  prez
** OK -- more stuff with SJOIN/SNICK -- but it WORKS!!!
**
** Revision 1.148  2000/03/14 10:02:48  prez
** Added SJOIN and SNICK
**
** Revision 1.147  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.146  2000/03/07 09:53:19  prez
** More helpfile updates (and associated updates).
**
** Revision 1.145  2000/03/02 07:25:10  prez
** Added stuff to do the chanserv greet timings (ie. only greet if a user has
** been OUT of channel over 'x' seconds).  New stored chanserv cfg item.
**
** Revision 1.144  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.143  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.142  2000/02/17 12:55:04  ungod
** still working on borlandization
**
** Revision 1.141  2000/02/16 12:59:38  ungod
** fixing for borland compilability
**
** Revision 1.140  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.139  2000/02/15 10:37:48  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#include "chanserv.h"
#include "lockable.h"
#include "magick.h"
#include "cryptstream.h"

#ifdef __BORLAND__
#pragma codeseg CODE2
#endif

// Private functions

void Chan_Live_t::Join(mstring nick)
{
    FT("Chan_Live_t::Join", (nick));

    if (users.find(nick.LowerCase())!=users.end())
    {
	wxLogWarning(Parent->getLogMessage("ERROR/DUP_CHAN"),
		"JOIN", nick.c_str(), i_Name.c_str());
    }
    else
    {
	users[nick.LowerCase()] = pair<bool,bool>(false,false);
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].Join(nick);
    }
}

unsigned int Chan_Live_t::Part(mstring nick)
{
    FT("Chan_Live_t::Part", (nick));
    if (users.find(nick.LowerCase())!=users.end())
    {
	mstring target = nick.LowerCase();
	if (Parent->nickserv.IsStored(nick) &&
	    Parent->nickserv.stored[nick.LowerCase()].IsOnline() &&
	    Parent->nickserv.stored[nick.LowerCase()].Host() != "")
	    target = Parent->nickserv.stored[nick.LowerCase()].Host().LowerCase();
	recent_parts[target] = Now();
	users.erase(nick.LowerCase());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].Part(nick);
    }
    else if (squit.find(nick.LowerCase())!=squit.end())
    {
	squit.erase(nick.LowerCase());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].Part(nick);
    }
    else
	wxLogDebug(Parent->getMessage("ERROR/REC_FORNOTINCHAN"),
	    "PART", nick.c_str(), i_Name.c_str());

    RET(users.size() + squit.size());
}

void Chan_Live_t::Squit(mstring nick)
{
    FT("Chan_Live_t::Squit", (nick));
    if (users.find(nick.LowerCase())!=users.end())
    {
	squit[nick.LowerCase()] = users[nick.LowerCase()];
	users.erase(nick.LowerCase());
    }
}

void Chan_Live_t::UnSquit(mstring nick)
{
    FT("Chan_Live_t::UnSquit", (nick));

    // We'll get ALL modes if all users are squit
    if (!users.size())
	modes = "";

    if (squit.find(nick.LowerCase())==squit.end())
    {
	wxLogWarning(Parent->getMessage("ERROR/REC_FORNOTINCHAN"),
		"UNSQUIT", nick.c_str(), i_Name.c_str());
    }
    else
	Part(nick);
}

unsigned int Chan_Live_t::Kick(mstring nick, mstring kicker)
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
	wxLogWarning("NICK CHANGE for %s received, and is not in %s.", nick.c_str(), i_Name.c_str());
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
    i_Limit = 0;
    users[first_user.LowerCase()] = pair<bool,bool>(false,false);
    if (Parent->chanserv.IsStored(i_Name))
	Parent->chanserv.stored[i_Name.LowerCase()].Join(first_user);
}


void Chan_Live_t::operator=(const Chan_Live_t &in)
{
    NFT("Chan_Live_t::operator=");
    bans.clear();
    map<mstring, mDateTime>::const_iterator i;
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
    for(k=in.squit.begin();k!=in.squit.end();k++)
	squit.insert(*k);
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


unsigned int Chan_Live_t::Squit()
{
    NFT("Chan_Livt_t::Squit");
    RET(squit.size());
}


mstring Chan_Live_t::Squit(unsigned int num)
{
    FT("Chan_Live_t::Squit", (num));
    unsigned int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    for(i=0, k=squit.begin();k!=squit.end();k++, i++)
	if (i==num)
	{
	    RET(k->first);
	}

    RET("");
}


unsigned int Chan_Live_t::Users()
{
    NFT("Chan_Livt_t::Users");
    RET(users.size());
}


mstring Chan_Live_t::User(unsigned int num)
{
    FT("Chan_Live_t::Users", (num));
    unsigned int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    for(i=0, k=users.begin();k!=users.end();k++, i++)
	if (i==num)
	{
	    RET(k->first);
	}
 
    RET("");
}


unsigned int Chan_Live_t::Ops()
{
    NFT("Chan_Live_t::Ops");
    RET(count_if(users.begin(),users.end(),checkops));
}


mstring Chan_Live_t::Op(unsigned int num)
{
    FT("Chan_Live_t::Op", (num));
    unsigned int i;
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


unsigned int Chan_Live_t::Voices()
{
    NFT("Chan_Live_t::Voices");
    RET(count_if(users.begin(),users.end(),checkvoices));
}


mstring Chan_Live_t::Voice(unsigned int num)
{
    FT("Chan_Live_t::Voice", (num));
    unsigned int i;
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


unsigned int Chan_Live_t::Bans()
{
    NFT("Chan_Live_t::Bans");
    RET(bans.size());
}


mstring Chan_Live_t::Ban(unsigned int num)
{
    FT("Chan_Live_t::Ban", (num));
    unsigned int i;
    map<mstring, mDateTime>::const_iterator k;
    for(i=0, k=bans.begin();k!=bans.end();k++, i++)
	if (i==num)
	{
	    RET(k->first);
	}
 
    RET("");
}


mDateTime Chan_Live_t::Ban(mstring mask)
{
   FT("Chan_Live_t::Ban", (mask));
   if (IsBan(mask))
   {
	RET(bans[mask.LowerCase()]);
   }
   else
   {
	RET(mDateTime(0.0));
   }
}


bool Chan_Live_t::IsSquit(mstring nick)
{
    FT("Chan_Live_t::IsSquit", (nick));
    RET((squit.find(nick.LowerCase()) != squit.end()));
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


bool Chan_Live_t::IsBan(mstring mask)
{
    FT("Chan_Live_t::IsBan", (mask));
    RET((bans.find(mask.LowerCase()) != bans.end()));
}

void Chan_Live_t::LockDown()
{
    NFT("Chan_Live_t::LockDown");

    Parent->server.JOIN(Parent->chanserv.FirstName(), i_Name);
    // Override the MLOCK checking.
    Parent->server.MODE(Parent->chanserv.FirstName(), i_Name, "+s");
    ph_timer = ACE_Reactor::instance()->schedule_timer(&(Parent->chanserv.ph),
			    new mstring(i_Name),
			    ACE_Time_Value(Parent->chanserv.ChanKeep()));
}

void Chan_Live_t::UnLock()
{
    NFT("Chan_Live_t::UnLock");

    if (Parent->chanserv.IsStored(i_Name))
    {
	Chan_Stored_t *chan = &Parent->chanserv.stored[i_Name.LowerCase()];
	if (!chan->Mlock_On().Contains("s"))
	    SendMode("-s");
    }

/*
    mstring *arg;
    if (ACE_Reactor::instance()->cancel_timer(timer,
					    (const void **) arg))
    {
	    delete arg;
    }
*/
    ph_timer = 0;
}

bool Chan_Live_t::ModeExists(mstring mode, vector<mstring> mode_params,
			bool change, char reqmode, mstring reqparam)
{
    FT("Chan_Live_t::ModeExists", (mode, "vector<mstring> mode_params", change, reqmode, reqparam));
    unsigned int i, param;

    if (reqparam == "")
	if (mode.Contains(reqmode))
	{
	    RET(true);
	}
	else
	{
	    RET(false);
	}

    for (param=0, i=0; i<mode.size(); i++)
    {
	switch (mode[i])
	{
	case 'o':
	case 'v':
	case 'b':
	case 'k':
	    if (mode[i] == reqmode && param < mode_params.size()
		&& mode_params[param] == reqparam)
	    {
		    RET(true);
	    }
	    param++;
	    break;
	case 'l':
	    if (mode[i] == reqmode && param < mode_params.size()
		&& mode_params[param] == reqparam)
	    {
		    RET(true);
	    }
	    if (change)
		param++;
	    break;
	default:
	    if (mode[i] == reqmode)
	    {
		RET(true);
	    }
	}
    }
    RET(false);
}


void Chan_Live_t::RemoveMode(mstring mode, vector<mstring> mode_params,
			bool change, char reqmode, mstring reqparam)
{
    FT("Chan_Live_t::RemoveExists", (mode, "vector<mstring> mode_params", change, reqmode, reqparam));
    unsigned int i, param;
    mstring new_mode;
    vector<mstring> new_params;

    for (param=0, i=0; i<mode.size(); i++)
    {
	switch (mode[i])
	{
	case 'o':
	case 'v':
	case 'b':
	case 'k':
	    if (reqmode == mode[i] && param < mode_params.size())
	    {
		if ((reqparam != "" && mode_params[param] == reqparam) ||
		    reqparam == "")
		{
		    // Do nothing ... we want to delete it!
		}
		else
		{
		    new_mode += mode[i];
		    new_params.push_back(mode_params[param]);
		}
	    }
	    param++;
	    break;
	case 'l':
	    if (reqmode == mode[i] && param < mode_params.size())
	    {
		if ((reqparam != "" && mode_params[param] == reqparam)
		    || reqparam == "")
		{
		    // Do nothing ... we want to delete it!
		    if (change)
			param++;
		}
		else
		{
		    new_mode += mode[i];
		    if (change)
		    {
			new_params.push_back(mode_params[param]);
			param++;
		    }
		}
	    }
	    break;
	default:
	    if (mode[i] == reqmode)
	    {
		// Do nothing ... we want to delete it!
	    }
	    else
	    {
		new_mode += mode[i];
	    }
	}
    }
}


void Chan_Live_t::SendMode(mstring in)
{
    FT("Chan_Live_t::SendMode", (in));
    unsigned int i, param = 2;
    mstring mode = in.Before(" ");

    bool add = true;

    for (i=0; i<mode.size(); i++)
    {
	switch (mode[i])
	{
	case '+':
	    add = true;
	    break;
	case '-':
	    add = false;
	    break;
	case 'o':
	    if (in.WordCount(" ") >= param)
	    {
		if (add)
		{
		    if (!IsOp(in.ExtractWord(param, " ")))
		    {
			if (ModeExists(p_modes_off, p_modes_off_params, false, 'o', in.ExtractWord(param, " ")))
			    RemoveMode(p_modes_off, p_modes_off_params, false, 'o', in.ExtractWord(param, " "));
			if (!ModeExists(p_modes_on, p_modes_on_params, true, 'o', in.ExtractWord(param, " ")))
			{
			    p_modes_on += "o";
			    p_modes_on_params.push_back(in.ExtractWord(param, " "));
			}
		    }
		}
		else
		{
		    if (IsOp(in.ExtractWord(param, " ")))
		    {
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'o', in.ExtractWord(param, " ")))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'o', in.ExtractWord(param, " "));
			if (!ModeExists(p_modes_off, p_modes_off_params, false, 'o', in.ExtractWord(param, " ")))
			{
			    p_modes_off += "o";
			    p_modes_off_params.push_back(in.ExtractWord(param, " "));
			}
		    }
		}
		param++;
	    }
	    break;
	case 'v':
	    if (in.WordCount(" ") >= param)
	    {
		if (add)
		{
		    if (!IsVoice(in.ExtractWord(param, " ")))
		    {
			if (ModeExists(p_modes_off, p_modes_off_params, false, 'v', in.ExtractWord(param, " ")))
			    RemoveMode(p_modes_off, p_modes_off_params, false, 'v', in.ExtractWord(param, " "));
			if (!ModeExists(p_modes_on, p_modes_on_params, true, 'v', in.ExtractWord(param, " ")))
			{
			    p_modes_on += "v";
			    p_modes_on_params.push_back(in.ExtractWord(param, " "));
			}
		    }
		}
		else
		{
		    if (IsVoice(in.ExtractWord(param, " ")))
		    {
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'v', in.ExtractWord(param, " ")))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'v', in.ExtractWord(param, " "));
			if (!ModeExists(p_modes_off, p_modes_off_params, false, 'v', in.ExtractWord(param, " ")))
			{
			    p_modes_off += "v";
			    p_modes_off_params.push_back(in.ExtractWord(param, " "));
			}
		    }
		}
		param++;
	    }
	    break;
	case 'b':
	    if (in.WordCount(" ") >= param)
	    {
		if (add)
		{
		    if (!IsBan(in.ExtractWord(param, " ")))
		    {
			if (ModeExists(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(param, " ")))
			    RemoveMode(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(param, " "));
			if (!ModeExists(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(param, " ")))
			{
			    p_modes_on += "b";
			    p_modes_on_params.push_back(in.ExtractWord(param, " "));
			}
		    }
		}
		else
		{
		    if (IsBan(in.ExtractWord(param, " ")))
		    {
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(param, " ")))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(param, " "));
			if (!ModeExists(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(param, " ")))
			{
			    p_modes_off += "b";
			    p_modes_off_params.push_back(in.ExtractWord(param, " "));
			}
		    }
		}
		param++;
	    }
	    break;
	case 'l':
	    if (add)
	    {
		if (in.WordCount(" ") >= param)
		{
		    if (ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
			RemoveMode(p_modes_off, p_modes_off_params, false, 'l');
		    if (!ModeExists(p_modes_on, p_modes_on_params, true, 'l', in.ExtractWord(param, " ")))
		    {
			p_modes_on += "l";
			p_modes_on_params.push_back(in.ExtractWord(param, " "));
		    }
		    param++;
		}
	    }
	    else
	    {
		if (ModeExists(p_modes_on, p_modes_on_params, true, 'l'))
		    RemoveMode(p_modes_on, p_modes_on_params, true, 'l');
		if (!ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
		{
		    p_modes_off += "l";
		}
	    }
	    break;
	case 'k':
	    if (in.WordCount(" ") >= param)
	    {
		if (add)
		{
		    // ONLY allow +k if we've turned it off before, or one isnt set
		    if (!i_Key || ModeExists(p_modes_off, p_modes_off_params, false, 'k'))
		    {
			// DONT take off 'off' value, coz we can -k+k key1 key2
			if (!ModeExists(p_modes_on, p_modes_on_params, true, 'k'))
			{
			    p_modes_on += "k";
			    p_modes_on_params.push_back(in.ExtractWord(param, " "));
			}
		    }
		}
		else
		{
		    if (i_Key == in.ExtractWord(param, " "))
		    {
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'k'))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'k');
			if (!ModeExists(p_modes_off, p_modes_off_params, false, 'k'))
			{
			    p_modes_off += "k";
			    p_modes_off_params.push_back(in.ExtractWord(param, " "));
			}
		    }
		}
		param++;
	    }
	    break;
	default:
		if (add)
		{
		    if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
			RemoveMode(p_modes_off, p_modes_off_params, false, mode[i]);
		    if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
			p_modes_on += mode[i];
		}
		else
		{
		    if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
			RemoveMode(p_modes_on, p_modes_on_params, true, mode[i]);
		    if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
		    {
			p_modes_off += mode[i];
		    }
		}
	    break;
	}
    }	
}


void Chan_Live_t::Mode(mstring source, mstring in)
{
    FT("Chan_Live_t::Mode", (source, in));

    mstring change = in.ExtractWord(1, ": ");
    unsigned int fwdargs = 2, i;
    bool add = true;
    CP(("MODE CHANGE (%s): %s", i_Name.c_str(), in.c_str()));
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
		bans[in.ExtractWord(fwdargs, ": ").LowerCase()] = Now();
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
		fwdargs++;
	    }
	    else
	    {
		i_Limit = 0;
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
		wxLogDebug("MODE change %c%c received from %s for %s that is currently in effect",
			add ? '+' : '-', change[i], source.c_str(), i_Name.c_str());
	    }
	    break;
	}
    }
    if (Parent->chanserv.IsStored(i_Name))
	Parent->chanserv.stored[i_Name.LowerCase()].Mode(source, in);
}


mDateTime Chan_Live_t::PartTime(mstring nick)
{
    FT("Chan_Live_t::PartTime", (nick));
    if (recent_parts.find(nick.LowerCase()) != recent_parts.end())
    {
	RET(recent_parts[nick.LowerCase()]);
    }
    RET(mDateTime(0.0));
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

    map<mstring, unsigned int>::iterator iter;

    // Create a new one if we find the entry
    for (iter=failed_passwds.begin(); iter!=failed_passwds.end(); iter++)
	if (iter->first == nick.LowerCase())
	    failed_passwds[newnick.LowerCase()] = iter->second;
    failed_passwds.erase(nick.LowerCase());
}


void Chan_Stored_t::Join(mstring nick)
{
    FT("Chan_Stored_t::Join", (nick));

    Chan_Live_t *clive = NULL;
    Nick_Live_t *nlive = NULL;
    Nick_Stored_t *nstored = NULL;

    if (Parent->chanserv.IsLive(i_Name))
	clive = &Parent->chanserv.live[i_Name.LowerCase()];
    else
	return;
    unsigned int users = clive->Users();

    if (Parent->nickserv.IsLive(nick))
	nlive = &Parent->nickserv.live[nick.LowerCase()];
    else
	return;

    if (Parent->nickserv.IsStored(nick))
	nstored = &Parent->nickserv.stored[nick.LowerCase()];

    if (nlive->IsServices())
    {
	clive->SendMode("+o " + nick);
	return;
    }

    if (Join() && users == 1)
    {
	Parent->server.JOIN(Parent->chanserv.FirstName(), i_Name);
	users++;
    }

    { // NOONE else can move our iterator
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    if (Akick_find(nick))
    {
	// If this user is the only user in channel
	if (users == 1)
	    clive->LockDown();

	if (Parent->nickserv.IsLive(Akick->Entry()))
	    clive->SendMode("+b " +
		nlive->Mask(Nick_Live_t::P_H));
	else
	    clive->SendMode("+b " + Akick->Entry());

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), nick,
		i_Name, Akick->Value());

	return;
    }}

    if (Restricted() && !Suspended() && GetAccess(nick) < (long) 1)
    {
	// If this user is the only user in channel
	if (users == 1)
	    clive->LockDown();

	if (Parent->nickserv.IsLive(nick))
	    clive->SendMode("+b " +
		nlive->Mask(Nick_Live_t::P_H));
	else
	    clive->SendMode("+b " + nick + "!*@*");

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), nick,
		i_Name, Parent->getMessage(nick, "MISC/KICK_RESTRICTED"));

	return;
    }

    clive->UnLock();
    if (!Join() && users == 2 &&
			clive->IsIn(Parent->chanserv.FirstName()))
    {
	Parent->server.PART(Parent->chanserv.FirstName(), i_Name);
	users--;
    }

    if (users == (Join() ? 2U : 1U))
    {
	if (i_Mlock_On.Contains("k") && i_Mlock_On.Contains("l"))
	{
	    if (i_Mlock_On.Before("k").Contains("l"))
	    {
		clive->SendMode(i_Mlock_On + " " + itoa(i_Mlock_Limit) + " " +
						    i_Mlock_Key);
	    }
	    else
	    {
		clive->SendMode(i_Mlock_On + " " + i_Mlock_Key + " " +
						    itoa(i_Mlock_Limit));
	    }
	}
	else
	{
	    clive->SendMode(i_Mlock_On + " " + i_Mlock_Key + " " +
			mstring(i_Mlock_Limit ? "" : itoa(i_Mlock_Limit)));
	}

	// Carry over topic ..
	if (Keeptopic() && i_Topic != "")
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

    if (GetAccess(nick)>0)
	i_LastUsed = Now();

    if (GetAccess(nick, "AUTOOP"))
	clive->SendMode("+o " + nick);
    else if (GetAccess(nick, "AUTOVOICE"))
	clive->SendMode("+v " + nick);

    mstring target = nick;
    if (nstored != NULL && nstored->Host() != "" &&
	Parent->nickserv.IsStored(nstored->Host()))
    {
	target = nstored->Host();
    }

    if (!Suspended()) {
	MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
	if (Greet_find(target) &&
		clive->PartTime(target).SecondsSince() > Parttime())
	{
	    if (Greet->Entry()[0U] == '!')
	    {
		if (Parent->nickserv.IsLive(Parent->chanserv.FirstName()))
		    Parent->chanserv.privmsg(i_Name, "[" + nick + "] " +
					Greet->Entry().After("!"));
	    }
	    else
	    {
		if (Parent->nickserv.IsLive(Parent->chanserv.FirstName()))
		    Parent->chanserv.privmsg(i_Name, "[" + nick + "] " +
						Greet->Entry());
	    }
	}
    }

    {
	MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
	for(Message = Message_begin(); Message != Message_end(); Message++)
	{
	    if (Parent->nickserv.IsLive(Parent->chanserv.FirstName()))
		Parent->chanserv.notice(nick, "[" + i_Name + "] " + Message->Entry());
	}
    }
}


void Chan_Stored_t::Part(mstring nick)
{
    FT("Chan_Stored_t::Part", (nick));
    if (GetAccess(nick)>0)
	i_LastUsed = Now();
}


void Chan_Stored_t::Kick(mstring nick, mstring kicker)
{
    FT("Chan_Stored_t::Kick", (nick, kicker));

    // Users shouldnt kick us, but we just rejoin!
    if (Parent->nickserv.IsLive(nick) &&
	Parent->nickserv.live[nick.LowerCase()].IsServices() &&
	Join())
    {
	Parent->server.JOIN(nick, i_Name);
    }

    // WE can kick anyone we want
    if (!Parent->nickserv.IsLive(kicker) ||
	Parent->nickserv.live[kicker.LowerCase()].IsServices())
	return;

    if (GetAccess(nick)>GetAccess(kicker))
    {
	// Check revenge, and do it.
    }
}


void Chan_Stored_t::Topic(mstring topic, mstring setter, mDateTime time)
{
    FT("Chan_Stored_t::Topic", (topic, setter, time));

    // Its us re-setting it!
    if (Parent->nickserv.IsLive(setter) &&
	Parent->nickserv.live[setter.LowerCase()].IsServices())
	return;

    if (Suspended())
    {
	Parent->server.TOPIC(Parent->chanserv.FirstName(), i_Name,
			"[" + IRC_Bold + Parent->getMessage("MISC/SUSPENDED") +
			IRC_Off + "] " + i_Comment + " [" + IRC_Bold +
			Parent->getMessage("MISC/SUSPENDED") + IRC_Off + "]");
	return;
    }

    if (Topiclock())
    {
	Parent->server.TOPIC(Parent->chanserv.FirstName(), i_Name,
		i_Topic + " (" + i_Topic_Setter + ")");
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
    if (Parent->nickserv.IsLive(setter) &&
	Parent->nickserv.live[setter.LowerCase()].IsServices())
	    return;


    mstring send_on;
    mstring send_off;
    mstring send_on_args;
    mstring send_off_args;

    mstring change = mode.ExtractWord(1, ": ");
    unsigned int fwdargs = 2, i;
    bool add = true;
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
		if (Access_value(mode.ExtractWord(fwdargs, ": ")) <= Level_value("AUTODEOP") ||
			(!(GetAccess(mode.ExtractWord(fwdargs, ": "), "CMDOP") ||
			  GetAccess(mode.ExtractWord(fwdargs, ": "), "AUTOOP")) &&
			Secureops()))
		{
		    send_off += "o";
		    send_off_args += " " + mode.ExtractWord(fwdargs, ": ");
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

	case 'v':
	    if (add)
	    {
		if (Access_value(mode.ExtractWord(fwdargs, ": ")) <= Level_value("AUTODEOP") ||
			(!(GetAccess(mode.ExtractWord(fwdargs, ": "), "CMDVOICE") ||
			  GetAccess(mode.ExtractWord(fwdargs, ": "), "AUTOVOICE")) &&
			Secureops()))
		{
		    send_off += "v";
		    send_off_args += " " + mode.ExtractWord(fwdargs, ": ");
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

	case 'b':
	    if (add && (GetAccess(mode.ExtractWord(fwdargs, ": ")) != 0 ||
			GetAccess(setter) != 0) &&
		GetAccess(mode.ExtractWord(fwdargs, ": ")) >= GetAccess(setter))
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

    l_Mlock_On = l_Mlock_Off = "";
    i_Bantime = Parent->chanserv.DEF_Bantime();
    l_Bantime = false;
    i_Parttime = Parent->chanserv.DEF_Parttime();
    l_Parttime = false;
    i_Keeptopic = Parent->chanserv.DEF_Keeptopic();
    l_Keeptopic = false;
    i_Topiclock = Parent->chanserv.DEF_Topiclock();
    l_Topiclock = false;
    i_Private = Parent->chanserv.DEF_Private();
    l_Private = false;
    i_Secureops = Parent->chanserv.DEF_Secureops();
    l_Secureops = false;
    i_Secure = Parent->chanserv.DEF_Secure();
    l_Secure = false;
    i_NoExpire = Parent->chanserv.DEF_NoExpire();
    l_NoExpire = false;
    i_Anarchy = Parent->chanserv.DEF_Anarchy();
    l_Anarchy = false;
    i_Restricted = Parent->chanserv.DEF_Restricted();
    l_Restricted = false;
    i_Join = Parent->chanserv.DEF_Join();
    l_Join = false;
    i_Revenge = Parent->chanserv.DEF_Revenge();
    l_Revenge = false;
    i_Forbidden = false;

    mstring defaulted = Parent->chanserv.DEF_MLock();
    mstring locked = Parent->chanserv.LCK_MLock();
    bool add = true;
    unsigned int i;

    for (i=0; i<defaulted.size(); i++)
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
    for (i=0; i<locked.size(); i++)
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

    vector<mstring> levels = Parent->chanserv.LVL();
    for (i=0; i<levels.size(); i++)
    {
	if (Parent->chanserv.LVL(levels[i]) >= Parent->chanserv.Level_Min())
	    i_Level.insert(entlist_val_t<long>(levels[i],
					Parent->chanserv.LVL(levels[i]),
					Parent->chanserv.FirstName()));
    }    
}


Chan_Stored_t::Chan_Stored_t(mstring name, mstring founder, mstring password, mstring desc)
{
    FT("Chan_Stored_t::Chan_Stored_t", (name, founder, password, desc));
    defaults();

    i_Name = name;
    i_RegTime = Now();
    i_Founder = founder;
    i_Password = password;
    i_Description = desc;

}


Chan_Stored_t::Chan_Stored_t(mstring name)
{
    FT("Chan_Stored_t::Chan_Stored_t", (name));
    defaults();

    i_Name = name;
    i_RegTime = Now();
    i_Forbidden = true;
}


void Chan_Stored_t::operator=(const Chan_Stored_t &in)
{
    NFT("Chan_Stored_t::operator=");
    i_Name=in.i_Name;
    i_RegTime=in.i_RegTime;
    i_Founder=in.i_Founder;
    i_CoFounder=in.i_CoFounder;
    i_Description=in.i_Description;
    i_Password=in.i_Password;
    i_URL=in.i_URL;

    i_Mlock_On=in.i_Mlock_On;
    l_Mlock_On=in.l_Mlock_On;
    i_Mlock_Off=in.i_Mlock_Off;
    l_Mlock_Off=in.l_Mlock_Off;
    i_Mlock_Key=in.i_Mlock_Key;
    i_Mlock_Limit=in.i_Mlock_Limit;

    i_Bantime=in.i_Bantime;
    l_Bantime=in.l_Bantime;
    i_Parttime=in.i_Parttime;
    l_Parttime=in.l_Parttime;
    i_Keeptopic=in.i_Keeptopic;
    l_Keeptopic=in.l_Keeptopic;
    i_Topiclock=in.i_Topiclock;
    l_Topiclock=in.l_Topiclock;
    i_Private=in.i_Private;
    l_Private=in.l_Private;
    i_Secureops=in.i_Secureops;
    l_Secureops=in.l_Secureops;
    i_Secure=in.i_Secure;
    l_Secure=in.l_Secure;
    i_NoExpire=in.i_NoExpire;
    l_NoExpire=in.l_NoExpire;
    i_Anarchy=in.i_Anarchy;
    l_Anarchy=in.l_Anarchy;
    i_Restricted=in.i_Restricted;
    l_Restricted=in.l_Restricted;
    i_Join=in.i_Join;
    l_Join=in.l_Join;
    i_Suspend_By=in.i_Suspend_By;
    i_Suspend_Time=in.i_Suspend_Time;
    i_Forbidden=in.i_Forbidden;

//  entlist_val_cui<long> j;
    set<entlist_val_t<long> >::const_iterator j;
    i_Level.clear();
    for(j=in.i_Level.begin();j!=in.i_Level.end();j++)
	i_Level.insert(*j);

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

    i_Message.clear();
    for(l=in.i_Message.begin();l!=in.i_Message.end();l++)
	i_Message.push_back(*l);

    i_UserDef.clear();
    map<mstring, mstring>::const_iterator i;
    for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
    i_UserDef.insert(*i);
}

mDateTime Chan_Stored_t::LastUsed()
{
    NFT("Chan_Stored_t::LastUsed");

    if (Parent->chanserv.IsLive(i_Name))
    {
	for (unsigned int i=0; i<Parent->chanserv.live[i_Name.LowerCase()].Users(); i++)
	{
	    if (GetAccess(Parent->chanserv.live[i_Name.LowerCase()].User(i)) > 0)
	    {
		RET(Now());
	    }
	}
    }
    RET(i_LastUsed);
}

void Chan_Stored_t::Founder(mstring in)
{
    FT("Chan_Stored_t::Founder", (in));

    if (!Parent->nickserv.IsStored(in))
	return;

    if (i_CoFounder == in)
	i_CoFounder = "";

    i_Founder = in;
}


void Chan_Stored_t::CoFounder(mstring in)
{
    FT("Chan_Stored_t::CoFounder", (in));

    if (!Parent->nickserv.IsStored(in))
	return;

    if (i_Founder == in)
	return;

    i_CoFounder = in;
}


unsigned int Chan_Stored_t::CheckPass(mstring nick, mstring password)
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
    NFT("Chan_Stored_t::Mlock");
    mstring Result;
    if(i_Mlock_On!="")
	Result+="+" + i_Mlock_On;
    if(i_Mlock_Off!="")
	Result+="-" + i_Mlock_Off;
    RET(Result);
}


vector<mstring> Chan_Stored_t::Mlock(mstring source, mstring mode)
{
    FT("Chan_Stored_t::Mlock", (source, mode));

    i_Mlock_On = i_Mlock_Off = i_Mlock_Key = "";
    i_Mlock_Limit = 0;
    vector<mstring> retval;
    mstring output, change = mode.ExtractWord(1, ": ");
    unsigned int i;
    unsigned int fwdargs = 2;
    bool add = true;
    bool ignorek = false;
    bool ignorel = false;

    if (change.WordCount("k") > 2)
    {
	ignorek = true;
	output = "";
	output.Format(Parent->getMessage(source, "ERR_SYNTAX/MULTI_MODE").c_str(),
		    'k');
	retval.push_back(output);
    }
    if (change.WordCount("l") > 2)
    {
	ignorel = true;
	output = "";
	output.Format(Parent->getMessage(source, "ERR_SYNTAX/MULTI_MODE").c_str(),
		    'l');
	retval.push_back(output);
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
		    output = "";
		    output.Format(Parent->getMessage(source, "ERR_SYNTAX/NOKEY").c_str());
		    retval.push_back(output);
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
		    output = "";
		    output.Format(Parent->getMessage(source, "ERR_SYNTAX/NOLIMIT").c_str());
		    retval.push_back(output);
		    fwdargs--;
		}
		else if (!mode.ExtractWord(fwdargs, ": ").IsNumber() ||
			mode.ExtractWord(fwdargs, ": ").Contains("."))
		{
		    output = "";
		    output.Format(Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER").c_str());
		    retval.push_back(output);
		}
		else if (atol(mode.ExtractWord(fwdargs, ": ")) < 1)
		{
		    output = "";
		    output.Format(Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER").c_str(),
			    1, 32768);
		    retval.push_back(output);
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

    mstring locked = Parent->chanserv.LCK_MLock()
	+ "+" + l_Mlock_On + "-" + l_Mlock_Off;
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

    mstring output2 = "";
    if (override_on != "" || override_off != "")
    {	
	if (output2 != "")
	    output2 += "  ";
	output = "";
	output.Format(Parent->getMessage(source, "CS_COMMAND/MLOCK_REVERSED").c_str(),
	    ((override_on  != "" ? ("+" + override_on ) : mstring("")) +
	     (override_off != "" ? ("-" + override_off) : mstring(""))).c_str());
	output2 += output;
    }
    if (forced_on != "" || forced_off != "")
    {
	if (output2 != "")
	    output2 += "  ";
	output = "";
	output.Format(Parent->getMessage(source, "CS_COMMAND/MLOCK_FORCED").c_str(),
	    ((forced_on  != "" ? ("+" + forced_on ) : mstring("")) +
	     (forced_off != "" ? ("-" + forced_off) : mstring(""))).c_str());
	output2 += output;
    }
    if (output2 != "")
	retval.push_back(output2);
    if (i_Mlock_On != "" || i_Mlock_Off != "")
    {
	output = "";
	output.Format(Parent->getMessage(source, "CS_COMMAND/MLOCK_SET").c_str(),
	    i_Name.c_str(),
	    ((i_Mlock_On  != "" ? ("+" + i_Mlock_On )  : mstring("")) +
	     (i_Mlock_Off != "" ? ("-" + i_Mlock_Off)  : mstring("")) +
	     (i_Mlock_Limit ? (" " + mstring(ltoa(i_Mlock_Limit))) : mstring(""))).c_str());
	retval.push_back(output);
    }
    else
    {	
	output = "";
	output.Format(Parent->getMessage(source, "CS_COMMAND/MLOCK_UNSET").c_str(),
	    i_Name.c_str());
	retval.push_back(output);
    }
    NRET(vector<mstring>, retval);
}


mstring Chan_Stored_t::L_Mlock()
{
    NFT("Chan_Stored_t::L_Mlock");
    mstring Result;
    mstring mode_on = l_Mlock_On;
    mstring mode_off = l_Mlock_Off;
    mstring locked = Parent->chanserv.LCK_MLock();
    bool add = true;
    unsigned int i;
    for (i=0; i<locked.size(); i++)
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
		// Can't lock +k or +l
		if (locked[i] == 'k' || locked[i] == 'l')
		    break;
		if (!mode_on.Contains(locked[i]))
		    mode_on += locked[i];
		if (mode_off.Contains(locked[i]))
		    mode_off.Remove((mstring) locked[i]);
	    }
	    else
	    {
		if (!mode_off.Contains(locked[i]))
		    mode_off += locked[i];
		if (mode_on.Contains(locked[i]))
		    mode_on.Remove((mstring) locked[i]);
	    }
	    break;
	}
    }

    if(mode_on!="")
	Result+="+" + mode_on;
    if(mode_off!="")
	Result+="-" + mode_off;
    RET(Result);
}


vector<mstring> Chan_Stored_t::L_Mlock(mstring source, mstring mode)
{
    FT("Chan_Stored_t::L_Mlock", (source, mode));

    l_Mlock_On = l_Mlock_Off = "";
    vector<mstring> retval;
    mstring output, change = mode.ExtractWord(1, ": ");
    bool add = true;
    unsigned int i;

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
		// Cant lock +k or +l
		if (change[i] == 'k' || change[i] == 'l')
		    break;
		if (!l_Mlock_On.Contains(change[i]))
		    l_Mlock_On += change[i];
		if (l_Mlock_Off.Contains(change[i]))
		    l_Mlock_Off.Remove((mstring) change[i]);
	    }
	    else
	    {
		if (!l_Mlock_Off.Contains(change[i]))
		    l_Mlock_Off += change[i];
		if (l_Mlock_On.Contains(change[i]))
		    l_Mlock_On.Remove((mstring) change[i]);
	    }
	    break;
	}
    }

    mstring locked = Parent->chanserv.LCK_MLock();
    mstring override_on;
    mstring override_off;
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
		// Can't lock +k or +l
		if (locked[i] == 'k' || locked[i] == 'l')
		    break;
		if (l_Mlock_Off.Contains(locked[i]))
		{
		    override_off += locked[i];
		    l_Mlock_Off.Remove((mstring) locked[i]);
		}
	    }
	    else
	    {
		if (l_Mlock_On.Contains(locked[i]))
		{
		    override_on += locked[i];
		    l_Mlock_On.Remove((mstring) locked[i]);
		}
	    }
	    break;
	}
    }


    if (override_on != "" || override_off != "")
    {
	output = "";
	output.Format(Parent->getMessage(source, "CS_COMMAND/MLOCK_REVERSED").c_str(),
	    ((override_on  != "" ? ("+" + override_on ) : mstring("")) +
	     (override_off != "" ? ("-" + override_off) : mstring(""))).c_str());
	retval.push_back(output);
    }
    if (l_Mlock_On != "" || l_Mlock_Off != "")
    {
	output = "";
	output.Format(Parent->getMessage(source, "CS_COMMAND/MLOCK_LOCK").c_str(),
	    i_Name.c_str(),
	    ((l_Mlock_On  != "" ? ("+" + l_Mlock_On )  : mstring("")) +
	     (l_Mlock_Off != "" ? ("-" + l_Mlock_Off)  : mstring(""))).c_str());
	retval.push_back(output);
    }
    else
    {
	output = "";
	output.Format(Parent->getMessage(source, "CS_COMMAND/MLOCK_UNLOCK").c_str(),
	    i_Name.c_str());
	retval.push_back(output);
    }
    NRET(vector<mstring>, retval);
}


void Chan_Stored_t::Bantime(unsigned long in)
{
    FT("Chan_Stored_t::Bantime", (in));
    if (!(Parent->chanserv.LCK_Bantime() || l_Bantime))
	i_Bantime = in;
}


unsigned long Chan_Stored_t::Bantime()
{
    NFT("Chan_Stored_t::Bantime");
    if (!Parent->chanserv.LCK_Bantime())
    {
	RET(i_Bantime);
    }
    RET(Parent->chanserv.DEF_Bantime());
}


void Chan_Stored_t::L_Bantime(bool in)
{
    FT("Chan_Stored_t::L_Bantime", (in));
    if (!Parent->chanserv.LCK_Bantime())
	l_Bantime = in;
}


bool Chan_Stored_t::L_Bantime()
{
    NFT("Chan_Stored_t::L_Bantime");
    if (!Parent->chanserv.LCK_Bantime())
    {
	RET(l_Bantime);
    }
    RET(true);
}


void Chan_Stored_t::Parttime(unsigned long in)
{
    FT("Chan_Stored_t::Parttime", (in));
    if (!(Parent->chanserv.LCK_Parttime() || l_Parttime))
	i_Parttime = in;
}


unsigned long Chan_Stored_t::Parttime()
{
    NFT("Chan_Stored_t::Parttime");
    if (!Parent->chanserv.LCK_Parttime())
    {
	RET(i_Parttime);
    }
    RET(Parent->chanserv.DEF_Parttime());
}


void Chan_Stored_t::L_Parttime(bool in)
{
    FT("Chan_Stored_t::L_Parttime", (in));
    if (!Parent->chanserv.LCK_Parttime())
	l_Parttime = in;
}


bool Chan_Stored_t::L_Parttime()
{
    NFT("Chan_Stored_t::L_Parttime");
    if (!Parent->chanserv.LCK_Parttime())
    {
	RET(l_Parttime);
    }
    RET(true);
}


void Chan_Stored_t::Keeptopic(bool in)
{
    FT("Chan_Stored_t::Keeptopic", (in));
    if (!(Parent->chanserv.LCK_Keeptopic() || l_Keeptopic))
	i_Keeptopic = in;
}


bool Chan_Stored_t::Keeptopic()
{
    NFT("Chan_Stored_t::Keeptopic");
    if (!Parent->chanserv.LCK_Keeptopic())
    {
	RET(i_Keeptopic);
    }
    RET(Parent->chanserv.DEF_Keeptopic());
}


void Chan_Stored_t::L_Keeptopic(bool in)
{
    FT("Chan_Stored_t::L_Keeptopic", (in));
    if (!Parent->chanserv.LCK_Keeptopic())
	l_Keeptopic = in;
}


bool Chan_Stored_t::L_Keeptopic()
{
    NFT("Chan_Stored_t::L_Keeptopic");
    if (!Parent->chanserv.LCK_Keeptopic())
    {
	RET(l_Keeptopic);
    }
    RET(true);
}


void Chan_Stored_t::Topiclock(bool in)
{
    FT("Chan_Stored_t::Topiclock", (in));
    if (!(Parent->chanserv.LCK_Topiclock() || l_Topiclock))
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


void Chan_Stored_t::L_Topiclock(bool in)
{
    FT("Chan_Stored_t::L_Topiclock", (in));
    if (!Parent->chanserv.LCK_Topiclock())
	l_Topiclock = in;
}


bool Chan_Stored_t::L_Topiclock()
{
    NFT("Chan_Stored_t::L_Topiclock");
    if (!Parent->chanserv.LCK_Topiclock())
    {
	RET(l_Topiclock);
    }
    RET(true);
}


void Chan_Stored_t::Private(bool in)
{
    FT("Chan_Stored_t::Private", (in));
    if (!(Parent->chanserv.LCK_Private() || l_Private))
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


void Chan_Stored_t::L_Private(bool in)
{
    FT("Chan_Stored_t::L_Private", (in));
    if (!Parent->chanserv.LCK_Private())
	l_Private = in;
}


bool Chan_Stored_t::L_Private()
{
    NFT("Chan_Stored_t::L_Private");
    if (!Parent->chanserv.LCK_Private())
    {
	RET(l_Private);
    }
    RET(true);
}


void Chan_Stored_t::Secureops(bool in)
{
    FT("Chan_Stored_t::Secureops", (in));
    if (!(Parent->chanserv.LCK_Secureops() || l_Secureops))
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


void Chan_Stored_t::L_Secureops(bool in)
{
    FT("Chan_Stored_t::L_Secureops", (in));
    if (!Parent->chanserv.LCK_Secureops())
	l_Secureops = in;
}


bool Chan_Stored_t::L_Secureops()
{
    NFT("Chan_Stored_t::L_Secureops");
    if (!Parent->chanserv.LCK_Secureops())
    {
	RET(l_Secureops);
    }
    RET(true);
}


void Chan_Stored_t::Secure(bool in)
{
    FT("Chan_Stored_t::Secure", (in));
    if (!(Parent->chanserv.LCK_Secure() || l_Secure))
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


void Chan_Stored_t::L_Secure(bool in)
{
    FT("Chan_Stored_t::L_Secure", (in));
    if (!Parent->chanserv.LCK_Secure())
	l_Secure = in;
}


bool Chan_Stored_t::L_Secure()
{
    NFT("Chan_Stored_t::L_Secure");
    if (!Parent->chanserv.LCK_Secure())
    {
	RET(l_Secure);
    }
    RET(true);
}


void Chan_Stored_t::NoExpire(bool in)
{
    FT("Chan_Stored_t::NoExpire", (in));
    if (!(Parent->chanserv.LCK_NoExpire() || l_NoExpire))
	i_NoExpire = in;
}


bool Chan_Stored_t::NoExpire()
{
    NFT("Chan_Stored_t::NoExpire");
    if (!Parent->chanserv.LCK_NoExpire())
    {
	RET(i_NoExpire);
    }
    RET(Parent->chanserv.DEF_NoExpire());
}


void Chan_Stored_t::L_NoExpire(bool in)
{
    FT("Chan_Stored_t::L_NoExpire", (in));
    if (!Parent->chanserv.LCK_NoExpire())
	l_NoExpire = in;
}


bool Chan_Stored_t::L_NoExpire()
{
    NFT("Chan_Stored_t::L_NoExpire");
    if (!Parent->chanserv.LCK_NoExpire())
    {
	RET(l_NoExpire);
    }
    RET(true);
}


void Chan_Stored_t::Anarchy(bool in)
{
    FT("Chan_Stored_t::Anarchy", (in));
    if (!(Parent->chanserv.LCK_Anarchy() || l_Anarchy))
	i_Anarchy = in;
}


bool Chan_Stored_t::Anarchy()
{
    NFT("Chan_Stored_t::Anarchy");
    if (!Parent->chanserv.LCK_Anarchy())
    {
	RET(i_Anarchy);
    }
    RET(Parent->chanserv.DEF_Anarchy());
}


void Chan_Stored_t::L_Anarchy(bool in)
{
    FT("Chan_Stored_t::L_Anarchy", (in));
    if (!Parent->chanserv.LCK_Anarchy())
	l_Anarchy = in;
}


bool Chan_Stored_t::L_Anarchy()
{
    NFT("Chan_Stored_t::L_Anarchy");
    if (!Parent->chanserv.LCK_Anarchy())
    {
	RET(l_Anarchy);
    }
    RET(true);
}


void Chan_Stored_t::Restricted(bool in)
{
    FT("Chan_Stored_t::Restricted", (in));
    if (!(Parent->chanserv.LCK_Restricted() || l_Restricted))
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


void Chan_Stored_t::L_Restricted(bool in)
{
    FT("Chan_Stored_t::L_Restricted", (in));
    if (!Parent->chanserv.LCK_Restricted())
	l_Restricted = in;
}


bool Chan_Stored_t::L_Restricted()
{
    NFT("Chan_Stored_t::L_Restricted");
    if (!Parent->chanserv.LCK_Restricted())
    {
	RET(l_Restricted);
    }
    RET(true);
}


void Chan_Stored_t::Join(bool in)
{
    FT("Chan_Stored_t::Join", (in));
    if (!(Parent->chanserv.LCK_Join() || l_Join))
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


void Chan_Stored_t::L_Join(bool in)
{
    FT("Chan_Stored_t::L_Join", (in));
    if (!Parent->chanserv.LCK_Join())
	l_Join = in;
}


bool Chan_Stored_t::L_Join()
{
    NFT("Chan_Stored_t::L_Join");
    if (!Parent->chanserv.LCK_Join())
    {
	RET(l_Join);
    }
    RET(true);
}


bool Chan_Stored_t::Revenge(mstring in)
{
    FT("Chan_Stored_t::Revenge", (in));
    if (!(Parent->chanserv.LCK_Revenge() || l_Revenge))
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


void Chan_Stored_t::L_Revenge(bool in)
{
    FT("Chan_Stored_t::L_Revenge", (in));
    if (!Parent->chanserv.LCK_Revenge())
	l_Revenge = in;
}


bool Chan_Stored_t::L_Revenge()
{
    NFT("Chan_Stored_t::L_Revenge");
    if (!Parent->chanserv.LCK_Revenge())
    {
	RET(l_Revenge);
    }
    RET(true);
}


bool Chan_Stored_t::Level_change(mstring entry, long value, mstring nick)
{
    FT("Chan_Stored_t::Level_change", (entry, value, nick));

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
    if (Level_find(entry))
    {
	pair<set<entlist_val_t<long> >::iterator, bool> tmp;
	i_Access.erase(Level);
	tmp = i_Level.insert(entlist_val_t<long>(
			entry.UpperCase(), value, nick));
	if (tmp.second)
	    Level = tmp.first;
	else
	    Level = i_Level.end();
	RET(true);
    }
    else
    {
	Level = i_Level.end();
	RET(false);
    }
}


bool Chan_Stored_t::Level_find(mstring entry)
{
    FT("Chan_Stored_t::Level_find", (entry));

    //  entlist_val_ui<long> iter = i_Level.end();
    set<entlist_val_t<long> >::iterator iter = i_Level.end();
    if (!i_Level.empty())
	for (iter=i_Level.begin(); iter!=i_Level.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
    if (iter != i_Level.end())
    {
	Level = iter;
	RET(true);
    }
    else
    {
	Level = i_Level.end();
	RET(false);
    }
}


long Chan_Stored_t::Level_value(mstring entry)
{
    FT("Chan_Stored_t::Level_value", (entry));

    long retval = 0;
//  entlist_val_ui<long> iter = Level;
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
    set<entlist_val_t<long> >::iterator iter = Level;

    if (Level_find(entry))
	retval=Level->Value();
    Level = iter;
    RET(retval);
}


bool Chan_Stored_t::Access_insert(mstring entry, long value, mstring nick, mDateTime modtime)
{
    FT("Chan_Stored_t::Access_insert", (entry, value, nick, modtime));

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
	else
	{
	    if (Parent->nickserv.stored[entry.LowerCase()].Host() != "")
		entry = Parent->nickserv.stored[Parent->nickserv.stored[entry.LowerCase()].Host().LowerCase()].Name();
	}
    }
    else
    {
	if (!entry.Contains("!"))
	    entry.Prepend("*!");
    }

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
    if (!Access_find(entry))
    {
	pair<set<entlist_val_t<long> >::iterator, bool> tmp;
	tmp = i_Access.insert(entlist_val_t<long>(
				entry, value, nick, modtime));
	if (tmp.second)
	    Access = tmp.first;
	else
	    Access = i_Access.end();
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

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
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


bool Chan_Stored_t::Access_find(mstring entry, bool livelook)
{
    FT("Chan_Stored_t::Access_find", (entry, livelook));

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
		if (livelook && Parent->nickserv.IsLive(entry))
		{
		    mstring mask = Parent->nickserv.live[entry.LowerCase()].Mask(Nick_Live_t::N_U_P_H);

		    for (iter=i_Access.begin(); iter!=i_Access.end(); iter++)
			if (mask.LowerCase().Matches(iter->Entry().LowerCase()))
			    break;
		}
	    }
	}
    }

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
    if (iter != i_Access.end())
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


long Chan_Stored_t::Access_value(mstring entry, bool looklive)
{
    FT("Chan_Stored_t::Access_value", (entry));

    long retval = 0;
//  entlist_val_ui<long> iter = Access;
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
    set<entlist_val_t<long> >::iterator iter = Access;

    if (Access_find(entry, looklive))
	retval=Access->Value();
    Access = iter;
    RET(retval);
}


long Chan_Stored_t::GetAccess(mstring entry)
{
    FT("Chan_Stored_t::GetAccess", (entry));

    Nick_Live_t *nlive;
    mstring realentry;
    if (Parent->nickserv.IsLive(entry))
    {
	nlive = &Parent->nickserv.live[entry.LowerCase()];
    }
    else
    {
	RET(0);
    }

    if (nlive->IsChanIdentified(i_Name) && !Suspended())
    {
	RET(Parent->chanserv.Level_Max() + 1);
    }

    if (Parent->nickserv.IsStored(entry))
    {
	realentry = entry.LowerCase();
	if (Parent->nickserv.stored[realentry].Host() != "")
	    realentry = Parent->nickserv.stored[realentry].Host().LowerCase();
    }
    else
    {
	RET(0);
    }

    if (Suspended())
    {
	if (Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	    Parent->commserv.list[Parent->commserv.SADMIN_Name()].IsOn(realentry))
	{
	    RET(Parent->chanserv.Level_Max() + 1);
	}
	else if (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	    Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(realentry))
	{
	    RET(Level_value("AUTOOP"));
	}
	else if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	    Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(realentry))
	{
	    RET(Level_value("AUTOVOICE"));
	}

	RET(0);
    }

    if (!Secure() || (Secure() && nlive->IsIdentified()))
    {
	if (i_Founder.LowerCase() == realentry.LowerCase())
	{
	    RET(Parent->chanserv.Level_Max() + 1);
	}
	RET(Access_value(realentry));
    }
    RET(0);
}

bool Chan_Stored_t::GetAccess(mstring entry, mstring type)
{
    FT("Chan_Stored_t::GetAccess", (entry, type));

    RET(GetAccess(entry) >= Level_value(type));
}


bool Chan_Stored_t::Akick_insert(mstring entry, mstring value, mstring nick, mDateTime modtime)
{
    FT("Chan_Stored_t::Akick_insert", (entry, value, nick, modtime));

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

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    if (!Akick_find(entry))
    {
	pair<set<entlist_val_t<mstring> >::iterator, bool> tmp;
	tmp = i_Akick.insert(entlist_val_t<mstring>(
				entry, value, nick, modtime));
	if (tmp.second)
	    Akick = tmp.first;
	else
	    Akick = i_Akick.end();
	RET(true);
    }
    else
    {
	Akick = i_Akick.end();
	RET(false);
    }
}

bool Chan_Stored_t::Akick_insert(mstring entry, mstring nick, mDateTime modtime)
{
    FT("Chan_Stored_t::Akick_insert", (entry, nick, modtime));
    RET(Akick_insert(entry, Parent->chanserv.DEF_Akick_Reason(), nick, modtime));
}

bool Chan_Stored_t::Akick_erase()
{
    NFT("Chan_Stored_t::Akick_erase");

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
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


bool Chan_Stored_t::Akick_find(mstring entry, bool livelook)
{
    FT("Chan_Stored_t::Akick_find", (entry, livelook));

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
		if (livelook && Parent->nickserv.IsLive(entry))
		{
		    mstring mask = Parent->nickserv.live[entry.LowerCase()].Mask(Nick_Live_t::N_U_P_H);

		    for (iter=i_Akick.begin(); iter!=i_Akick.end(); iter++)
			if (mask.LowerCase().Matches(iter->Entry().LowerCase()))
			    break;
		}
	    }
	}
    }

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    if (iter != i_Akick.end())
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


mstring Chan_Stored_t::Akick_string(mstring entry, bool looklive)
{
    FT("Chan_Stored_t::Akick_string", (entry));

    mstring retval;
//  entlist_val_ui<mstring> iter = Akick;
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    set<entlist_val_t<mstring> >::iterator iter = Akick;

    if (Akick_find(entry, looklive))
	retval=Akick->Value();
    Akick = iter;
    RET(retval);
}


bool Chan_Stored_t::Greet_insert(mstring entry, mstring nick, mDateTime modtime)
{
    FT("Chan_Stored_t::Greet_insert", (entry, nick, modtime));

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
    if (!Greet_find(entry))
    {
	i_Greet.push_back(entlist_t(entry, nick, modtime));
	Greet = i_Greet.end(); Greet--;
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

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
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


bool Chan_Stored_t::Greet_find(mstring nick)
{
    FT("Chan_Stored_t::Greet_find", (nick));

    entlist_i iter = i_Greet.end();
    if (!i_Greet.empty())
	for (iter=i_Greet.begin(); iter!=i_Greet.end(); iter++)
	    if (nick.LowerCase() == iter->Last_Modifier().LowerCase())
		break;

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
    if (iter != i_Greet.end())
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


bool Chan_Stored_t::Message_insert(mstring entry, mstring nick)
{
    FT("Chan_Stored_t::Message_insert", (entry, nick));

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    i_Message.push_back(entlist_t(entry, nick));
    Message = i_Message.end(); Message--;
    RET(true);
}


bool Chan_Stored_t::Message_erase()
{
    NFT("Chan_Stored_t::Message_erase");

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    if (Message != i_Message.end())
    {
	i_Message.erase(Message);
	Message = i_Message.end();
	RET(true);
    }
    else
    {
	RET(false);
    }

}


bool Chan_Stored_t::Message_find(unsigned int num)
{
    FT("Chan_Stored_t::Message_find", (num));

    if (num <= 0 || num > i_Message.size())
    {
	RET(false);
    }

    unsigned int i;
    entlist_i iter = i_Message.end();
    for (iter=i_Message.begin(), i=1; iter!=i_Message.end() && i<num;
							    iter++, i++) ;

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    if (iter != i_Message.end())
    {
	Message = iter;
	RET(true);
    }
    else
    {
	Message = i_Message.end();
	RET(false);
    }
}


wxOutputStream &operator<<(wxOutputStream& out,Chan_Stored_t& in)
{
    out<<in.i_Name<<in.i_RegTime<<in.i_Founder<<in.i_CoFounder<<in.i_Description<<in.i_Password<<in.i_URL<<in.i_Comment;
    out<<in.i_Mlock_On<<in.i_Mlock_Off<<in.i_Mlock_Key<<in.i_Mlock_Limit;
    out<<in.i_Bantime<<in.i_Parttime<<in.i_Keeptopic<<in.i_Topiclock<<in.i_Private<<in.i_Secureops
	<<in.i_Secure<<in.i_NoExpire<<in.i_Anarchy<<in.i_Restricted<<in.i_Join<<in.i_Forbidden;
    out<<in.l_Bantime<<in.l_Parttime<<in.l_Keeptopic<<in.l_Topiclock<<in.l_Private<<in.l_Secureops
	<<in.l_Secure<<in.l_NoExpire<<in.l_Anarchy<<in.l_Restricted<<in.l_Join<<in.l_Mlock_On<<in.l_Mlock_Off;
    out<<in.i_Suspend_By<<in.i_Suspend_Time;

//  entlist_val_cui<long> j;
    set<entlist_val_t<long> >::const_iterator j;
    out<<in.i_Level.size();
    for(j=in.i_Level.begin();j!=in.i_Level.end();j++)
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
	out<<*l;

    out<<in.i_Message.size();
    for(l=in.i_Message.begin();l!=in.i_Message.end();l++)
	out<<*l;

    map<mstring,mstring>::iterator i;
    out<<in.i_UserDef.size();
    for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
	out<<(mstring)i->first<<(mstring)i->second;
    return out;
}


wxInputStream &operator>>(wxInputStream& in, Chan_Stored_t& out)
{
    size_t i, count;
    set<entlist_t>::size_type ei,ecount;
    set<entlist_val_t<long> >::size_type vli,vlcount;
    set<entlist_val_t<long> >::size_type vsi,vscount;
    mstring dummy,dummy2;
    entlist_t edummy;
    entlist_val_t<long> eldummy;
    entlist_val_t<mstring> esdummy;
    in>>out.i_Name>>out.i_RegTime>>out.i_Founder>>out.i_CoFounder>>out.i_Description>>out.i_Password>>out.i_URL>>out.i_Comment;
    in>>out.i_Mlock_On>>out.i_Mlock_Off>>out.i_Mlock_Key>>out.i_Mlock_Limit;
    in>>out.i_Bantime>>out.i_Parttime>>out.i_Keeptopic>>out.i_Topiclock>>out.i_Private>>out.i_Secureops
	>>out.i_Secure>>out.i_NoExpire>>out.i_Anarchy>>out.i_Restricted>>out.i_Join>>out.i_Forbidden;
    in>>out.l_Bantime>>out.l_Parttime>>out.l_Keeptopic>>out.l_Topiclock>>out.l_Private>>out.l_Secureops
	>>out.l_Secure>>out.l_NoExpire>>out.l_Anarchy>>out.l_Restricted>>out.l_Join>>out.l_Mlock_On>>out.l_Mlock_Off;
    in>>out.i_Suspend_By>>out.i_Suspend_Time;

    out.i_Level.clear();
    in>>vlcount;
    for(vli=0;vli<vlcount;vli++)
    {
	in>>eldummy;
	out.i_Level.insert(eldummy);
    }

    out.i_Access.clear();
    in>>vlcount;
    for(vli=0;vli<vlcount;vli++)
    {
	in>>eldummy;
	out.i_Access.insert(eldummy);
    }

    out.i_Akick.clear();
    in>>vscount;
    for(vsi=0;vsi<vscount;vsi++)
    {
	in>>esdummy;
	out.i_Akick.insert(esdummy);
    }

    out.i_Greet.clear();
    in>>ecount;
    for(ei=0;ei<ecount;ei++)
    {
	in>>edummy;
	out.i_Greet.push_back(edummy);
    }

    out.i_Message.clear();
    in>>ecount;
    for(ei=0;ei<ecount;ei++)
    {
	in>>edummy;
	out.i_Message.push_back(edummy);
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
    Revenge_Levels.insert("NONE");
    Revenge_Levels.insert("REVERSE");
    Revenge_Levels.insert("DEOP");
    Revenge_Levels.insert("KICK");
    Revenge_Levels.insert("BAN1");
    Revenge_Levels.insert("BAN2");
    Revenge_Levels.insert("BAN3");
    Revenge_Levels.insert("BAN4");
    Revenge_Levels.insert("MIRROR");
}

void ChanServ::AddCommands()
{
    NFT("ChanServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "H*LP", Parent->commserv.ALL_Name(), ChanServ::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "REG*", Parent->commserv.REGD_Name(), ChanServ::do_Register);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "DROP", Parent->commserv.REGD_Name(), ChanServ::do_Drop);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ID*", Parent->commserv.REGD_Name(), ChanServ::do_Identify);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "INFO*", Parent->commserv.ALL_Name(), ChanServ::do_Info);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LIST", Parent->commserv.ALL_Name(), ChanServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SUSP*", Parent->commserv.SOP_Name(), ChanServ::do_Suspend);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNSUSP*", Parent->commserv.SOP_Name(), ChanServ::do_UnSuspend);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FORB*", Parent->commserv.SOP_Name(), ChanServ::do_Forbid);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GET*PASS*", Parent->commserv.SOP_Name(), ChanServ::do_Getpass);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*MODE", Parent->commserv.REGD_Name(), ChanServ::do_Mode);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "OP*", Parent->commserv.REGD_Name(), ChanServ::do_Op);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "D*OP*", Parent->commserv.REGD_Name(), ChanServ::do_DeOp);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "VOIC*", Parent->commserv.REGD_Name(), ChanServ::do_Voice);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "D*VOIC*", Parent->commserv.REGD_Name(), ChanServ::do_DeVoice);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*TOPIC*", Parent->commserv.REGD_Name(), ChanServ::do_Topic);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "KICK*", Parent->commserv.REGD_Name(), ChanServ::do_Kick);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "REM*", Parent->commserv.REGD_Name(), ChanServ::do_AnonKick);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "USER*", Parent->commserv.REGD_Name(), ChanServ::do_Users);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "INV*", Parent->commserv.REGD_Name(), ChanServ::do_Invite);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNBAN*", Parent->commserv.REGD_Name(), ChanServ::do_Unban);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LIVE*", Parent->commserv.SOP_Name(), ChanServ::do_Live);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *USER*", Parent->commserv.REGD_Name(), ChanServ::do_clear_Users);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *OP*", Parent->commserv.REGD_Name(), ChanServ::do_clear_Ops);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *VOICE*", Parent->commserv.REGD_Name(), ChanServ::do_clear_Voices);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *MODE*", Parent->commserv.REGD_Name(), ChanServ::do_clear_Modes);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *BAN*", Parent->commserv.REGD_Name(), ChanServ::do_clear_Bans);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *ALL*", Parent->commserv.REGD_Name(), ChanServ::do_clear_All);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* SET*", Parent->commserv.REGD_Name(), ChanServ::do_level_Set);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* RESET*", Parent->commserv.REGD_Name(), ChanServ::do_level_Reset);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* LIST", Parent->commserv.REGD_Name(), ChanServ::do_level_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* VIEW", Parent->commserv.REGD_Name(), ChanServ::do_level_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* ADD", Parent->commserv.REGD_Name(), ChanServ::do_access_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* DEL*", Parent->commserv.REGD_Name(), ChanServ::do_access_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* REM*", Parent->commserv.REGD_Name(), ChanServ::do_access_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* LIST", Parent->commserv.REGD_Name(), ChanServ::do_access_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* VIEW", Parent->commserv.REGD_Name(), ChanServ::do_access_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK ADD", Parent->commserv.REGD_Name(), ChanServ::do_akick_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK DEL*", Parent->commserv.REGD_Name(), ChanServ::do_akick_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK REM*", Parent->commserv.REGD_Name(), ChanServ::do_akick_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK LIST", Parent->commserv.REGD_Name(), ChanServ::do_akick_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK VIEW", Parent->commserv.REGD_Name(), ChanServ::do_akick_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* ADD", Parent->commserv.REGD_Name(), ChanServ::do_greet_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* DEL*", Parent->commserv.REGD_Name(), ChanServ::do_greet_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* REM*", Parent->commserv.REGD_Name(), ChanServ::do_greet_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* LIST", Parent->commserv.REGD_Name(), ChanServ::do_greet_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* VIEW", Parent->commserv.REGD_Name(), ChanServ::do_greet_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* ADD", Parent->commserv.REGD_Name(), ChanServ::do_message_Add);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* DEL*", Parent->commserv.REGD_Name(), ChanServ::do_message_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* REM*", Parent->commserv.REGD_Name(), ChanServ::do_message_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* LIST", Parent->commserv.REGD_Name(), ChanServ::do_message_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* VIEW", Parent->commserv.REGD_Name(), ChanServ::do_message_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* FOUND*", Parent->commserv.REGD_Name(), ChanServ::do_set_Founder);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* COFOUND*", Parent->commserv.REGD_Name(), ChanServ::do_set_CoFounder);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* DESC*", Parent->commserv.REGD_Name(), ChanServ::do_set_Description);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* PASS*", Parent->commserv.REGD_Name(), ChanServ::do_set_Password);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* EMAIL", Parent->commserv.REGD_Name(), ChanServ::do_set_Email);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* URL", Parent->commserv.REGD_Name(), ChanServ::do_set_URL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* WWW*", Parent->commserv.REGD_Name(), ChanServ::do_set_URL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* WEB*", Parent->commserv.REGD_Name(), ChanServ::do_set_URL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* COMM*", Parent->commserv.SOP_Name(), ChanServ::do_set_Comment);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* M*LOCK", Parent->commserv.REGD_Name(), ChanServ::do_set_Mlock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* BAN*TIME", Parent->commserv.REGD_Name(), ChanServ::do_set_BanTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* PART*TIME", Parent->commserv.REGD_Name(), ChanServ::do_set_PartTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* KEEP*", Parent->commserv.REGD_Name(), ChanServ::do_set_KeepTopic);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* TOPIC*", Parent->commserv.REGD_Name(), ChanServ::do_set_TopicLock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* PRIV*", Parent->commserv.REGD_Name(), ChanServ::do_set_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* SEC*OP*", Parent->commserv.REGD_Name(), ChanServ::do_set_SecureOps);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* SEC*", Parent->commserv.REGD_Name(), ChanServ::do_set_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* NOEX*", Parent->commserv.SOP_Name(), ChanServ::do_set_NoExpire);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* ANAR*", Parent->commserv.SOP_Name(), ChanServ::do_set_Anarchy);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* RES*", Parent->commserv.REGD_Name(), ChanServ::do_set_Restricted);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *JOIN*", Parent->commserv.REGD_Name(), ChanServ::do_set_Join);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* REV*", Parent->commserv.REGD_Name(), ChanServ::do_set_Revenge);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK M*LOCK", Parent->commserv.SOP_Name(), ChanServ::do_lock_Mlock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK BAN*TIME", Parent->commserv.SOP_Name(), ChanServ::do_lock_BanTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK PART*TIME", Parent->commserv.SOP_Name(), ChanServ::do_lock_PartTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK KEEP*", Parent->commserv.SOP_Name(), ChanServ::do_lock_KeepTopic);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK TOPIC*", Parent->commserv.SOP_Name(), ChanServ::do_lock_TopicLock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK PRIV*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK SEC*OP*", Parent->commserv.SOP_Name(), ChanServ::do_lock_SecureOps);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK SEC*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK ANAR*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Anarchy);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK RES*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Restricted);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK *JOIN*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Join);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK REV*", Parent->commserv.SOP_Name(), ChanServ::do_lock_Revenge);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK M*LOCK", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Mlock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK BAN*TIME", Parent->commserv.SOP_Name(), ChanServ::do_unlock_BanTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK PART*TIME", Parent->commserv.SOP_Name(), ChanServ::do_unlock_PartTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK KEEP*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_KeepTopic);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK TOPIC*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_TopicLock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK PRIV*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK SEC*OP*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_SecureOps);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK SEC*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK ANAR*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Anarchy);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK RES*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Restricted);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK *JOIN*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Join);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK REV*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_Revenge);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CLEAR*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "M*S*G*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK *", Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK", Parent->commserv.SOP_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK *", Parent->commserv.SOP_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK", Parent->commserv.SOP_Name(), do_1_3param);
}

void ChanServ::RemCommands()
{
    NFT("ChanServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

//  Parent->commands.RemSystemCommand(GetInternalName(),
//		    "TRACE", "ALL");
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

void ChanServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    mstring HelpTopic = Parent->chanserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.Replace(" ", "/");
    vector<mstring> help = Parent->getHelp(source, HelpTopic.UpperCase());
					
    unsigned int i;
    for (i=0; i<help.size(); i++)
	::send(mynick, source, help[i]);
}

void ChanServ::do_Register(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Register", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring password  = params.ExtractWord(3, " ");
    mstring desc      = params.After(" ", 3);

    if (Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISSTORED"),
		    channel.c_str());
 	return;
    }

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		    channel.c_str());
	return;
    }

    if (!Parent->chanserv.live[channel.LowerCase()].IsOp(source))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/NOTOPPED"),
		    channel.c_str());
	return;
    }

    if (password.Len() < 5 || password.CmpNoCase(channel) == 0 ||
	password.CmpNoCase(source) == 0)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/COMPLEX_PASS"));
	return;
    }

    mstring founder = Parent->getSname(source);
    if (Parent->nickserv.stored[source.LowerCase()].Host() != "" &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[source.LowerCase()].Host()))
    {
	founder = Parent->getSname(Parent->nickserv.stored[source.LowerCase()].Host());
    }

    Parent->chanserv.stored[channel.LowerCase()] =
		Chan_Stored_t(channel, founder, password, desc);
    Parent->chanserv.stored[channel.LowerCase()].Topic(
		Parent->chanserv.live[channel.LowerCase()].Topic(),
		Parent->chanserv.live[channel.LowerCase()].Topic_Setter(),
		Parent->chanserv.live[channel.LowerCase()].Topic_Set_Time());
    Parent->nickserv.live[source.LowerCase()].ChanIdentify(channel, password);
    Parent->chanserv.live[channel.LowerCase()].SendMode(Parent->chanserv.stored[channel.LowerCase()].Mlock());
    Parent->chanserv.stats.i_Register++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/REGISTERED"),
		channel.c_str(), founder.c_str());
}

void ChanServ::do_Drop(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Drop", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }
    channel = Parent->getSname(channel);

    if (!Parent->nickserv.live[source.LowerCase()].IsChanIdentified(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NEED_CHAN_IDENT"),
		message.c_str(), mynick.c_str(), channel.c_str());
	return;
    }

    Parent->chanserv.stored.erase(channel.LowerCase());
    Parent->nickserv.live[source.LowerCase()].UnChanIdentify(channel);
    Parent->chanserv.stats.i_Drop++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/DROPPED"),
	    channel.c_str());
}

void ChanServ::do_Identify(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Identify", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring pass      = params.ExtractWord(3, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }
    channel = Parent->getSname(channel);

    bool wasident = Parent->nickserv.live[source.LowerCase()].IsChanIdentified(channel);
    mstring output = Parent->nickserv.live[source.LowerCase()].ChanIdentify(channel, pass);
    if (!wasident)
	Parent->chanserv.stats.i_Identify++;
    if (output != "")
	::send(mynick, source, output, channel.c_str());
}

void ChanServ::do_Info(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Info", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    mstring output;
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    if (Parent->chanserv.stored[channel.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISFORBIDDEN"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *chan = &Parent->chanserv.stored[channel.LowerCase()];

    ::send(mynick, source, Parent->getMessage(source, "CS_INFO/IS"),
		chan->Name().c_str());
    ::send(mynick, source, Parent->getMessage(source, "CS_INFO/FOUNDER"),
		chan->Founder().c_str());
    if (chan->CoFounder() != "")
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/COFOUNDER"),
		chan->CoFounder().c_str());
    ::send(mynick, source, Parent->getMessage(source, "CS_INFO/DESCRIPTION"),
		chan->Description().c_str());

    output = "";
    if (chan->NoExpire() && Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
	output << " (" << Parent->getMessage(source, "CS_INFO/NOEXPIRE") << ")";
    ::send(mynick, source, Parent->getMessage(source, "CS_INFO/REGISTERED"),
		chan->RegTime().Ago().c_str(), output.c_str());
    if (!chan->Private())
    {
	if (Parent->chanserv.IsLive(channel))
	{
	    Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
	    output = "";
	    if (clive->Ops())
	    {
		output << clive->Ops() << " " << Parent->getMessage(source, "CS_INFO/OPS");
	    }
	    if (clive->Voices())
	    {
		if (output != "")
		    output << ", ";
		output << clive->Voices() << " " << Parent->getMessage(source, "CS_INFO/VOICES");
	    }
	    if (clive->Users())
	    {
		if (output != "")
		    output << ", ";
		output << clive->Users() << " " << Parent->getMessage(source, "CS_INFO/USERS");
	    }
	    if (clive->Squit())
	    {
		if (output != "")
		    output << ", ";
		output << clive->Squit() << " " << Parent->getMessage(source, "CS_INFO/SPLIT_USERS");
	    }
	    ::send(mynick, source, Parent->getMessage(source, "CS_INFO/INUSEBY"),
		output.c_str());
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_INFO/LASTUSED"),
		chan->LastUsed().Ago().c_str());
	}
    }
    if (chan->Email() != "")
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/EMAIL"),
		chan->Email().c_str());
    if (chan->URL() != "")
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/URL"),
		chan->URL().c_str());
    if (chan->Suspended())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/SUSPEND"),
		chan->Suspend_Time().Ago().c_str(),
		chan->Suspend_By().c_str());
	if (chan->Comment() != "")
	    ::send(mynick, source, Parent->getMessage(source, "CS_INFO/SUSPENDFOR"),
		chan->Comment().c_str());
    }
    else if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	    Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
    {
	if (chan->Comment() != "")
	    ::send(mynick, source, Parent->getMessage(source, "CS_INFO/COMMENT"),
		chan->Comment().c_str());
    }
    if (chan->Last_Topic() != "")
    {
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/TOPIC"),
		chan->Last_Topic().c_str());
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/TOPICBY"),
		chan->Last_Topic_Set_Time().Ago().c_str(),
		chan->Last_Topic_Setter().c_str());
    }
    if (chan->Mlock() != "")
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/MLOCK"),
		chan->Mlock().c_str());
    if (chan->Revenge() != "")
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/REVENGE"),
	    Parent->getMessage(source, "CS_SET/REV_" + chan->Revenge()).c_str());
    if (chan->Bantime())
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/BANTIME"),
		ToHumanTime(chan->Bantime()).c_str());
    if (chan->Parttime())
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/PARTTIME"),
		ToHumanTime(chan->Parttime()).c_str());

    output = "";
    if (chan->Keeptopic())
    {
	if (output != "")
	    output << ", ";
	if (chan->L_Keeptopic())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/KEEPTOPIC");
	if (chan->L_Keeptopic())
	    output << IRC_Off;
    }

    if (chan->Topiclock())
    {
	if (output != "")
	    output << ", ";
	if (chan->L_Topiclock())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/TOPICLOCK");
	if (chan->L_Topiclock())
	    output << IRC_Off;
    }

    if (chan->Private())
    {
	if (output != "")
	    output << ", ";
	if (chan->L_Private())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/PRIVATE");
	if (chan->L_Private())
	    output << IRC_Off;
    }

    if (chan->Secureops())
    {
	if (output != "")
	    output << ", ";
	if (chan->L_Secureops())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/SECUREOPS");
	if (chan->L_Secureops())
	    output << IRC_Off;
    }

    if (chan->Secure())
    {
	if (output != "")
	    output << ", ";
	if (chan->L_Secure())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/SECURE");
	if (chan->L_Secure())
	    output << IRC_Off;
    }

    if (chan->Restricted())
    {
	if (output != "")
	    output << ", ";
	if (chan->L_Restricted())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/RESTRICTED");
	if (chan->L_Restricted())
	    output << IRC_Off;
    }

    if (chan->Anarchy())
    {
	if (output != "")
	    output << ", ";
	if (chan->L_Anarchy())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/ANARCHY");
	if (chan->L_Anarchy())
	    output << IRC_Off;
    }

    if (output != "")
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/OPTIONS"),
	    output.c_str());
}

void ChanServ::do_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_List", (mynick, source, params));

    unsigned int listsize, i;
    int count;
    mstring mask;

    mstring message = params.Before(" ").UpperCase();
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
	    ::send(mynick, source, Parent->getMessage(source, "LIST/MAXLIST"),
		    Parent->config.Maxlist());
	    return;
	}
    }

    ::send(mynick, source, Parent->getMessage(source, "LIST/CHAN_LIST"),
					mask.c_str());
    map<mstring, Chan_Stored_t>::iterator iter;

    for (iter = Parent->chanserv.stored.begin(), i=0, count = 0;
			iter != Parent->chanserv.stored.end(); iter++)
    {
	if (iter->second.Name().LowerCase().Matches(mask))
	{
	    if (i < listsize && (!iter->second.Private() ||
		(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))))
	    {
		::send(mynick, source, iter->second.Name() + "  " +
					    iter->second.Description());
		i++;
	    }
	    count++;
	}
    }
    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAYED"),
							i, count);
}

void ChanServ::do_Suspend(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Suspend", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring reason    = params.After(" ", 2);

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }
    channel = Parent->getSname(channel);

    Parent->chanserv.stored[channel.LowerCase()].Suspend(source, reason);
    Parent->server.TOPIC(mynick, channel, "[" + IRC_Bold +
			Parent->getMessage("MISC/SUSPENDED") +
			IRC_Off + "] " + reason + " [" + IRC_Bold +
			Parent->getMessage("MISC/SUSPENDED") + IRC_Off + "]");
    Parent->chanserv.stats.i_Suspend++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SUSPENDED"),
	    channel.c_str());
}

void ChanServ::do_UnSuspend(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_UnSuspend", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }
    channel = Parent->getSname(channel);

    Parent->chanserv.stored[channel.LowerCase()].UnSuspend();
    Parent->chanserv.stats.i_Unsuspend++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNSUSPENDED"),
	    channel.c_str());
}

void ChanServ::do_Forbid(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Forbid", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISSTORED"),
		channel.c_str());
	return;
    }

    Parent->chanserv.stored[channel.LowerCase()] = Chan_Stored_t(channel);
    Parent->chanserv.stats.i_Forbid++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/FORBIDDEN"),
	    channel.c_str());
}


void ChanServ::do_Getpass(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Getpass", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *chan = &Parent->chanserv.stored[channel.LowerCase()];
    if (!Parent->nickserv.IsStored(chan->Founder()))
    {
	wxLogWarning("Channel %s had a founder of %s who was not registered (channel dropped)",
			chan->Name().c_str(), chan->Founder().c_str());
	Parent->chanserv.stored.erase(channel.LowerCase());
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Parent->chanserv.stats.i_Getpass++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/GETPASS"),
			chan->Name().c_str(),
			Parent->getSname(chan->Founder()).c_str(),
			chan->Password().c_str());
    announce(mynick, Parent->getMessage("MISC/CHAN_GETPASS"),
			source.c_str(), chan->Name().c_str(),
			Parent->getSname(chan->Founder()).c_str());
}

void ChanServ::do_Mode(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Mode", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(cstored->GetAccess(source, "CMDMODE") ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))))
    {
	mstring modes = params.After(" ", 2);
	clive->SendMode(modes);
	Parent->chanserv.stats.i_Mode++;
	return;
    }
    else if (cstored->GetAccess(source, "VIEW"))
    {
	mstring output;
	output << clive->Name() << ": +" << clive->Mode();
	if (clive->Key() != "" && clive->Limit())
	{
	    if (clive->Mode().Before("l").Contains("k"))
	    {
		output << " " << clive->Key() << " " << clive->Limit();
	    }
	    else
	    {
		output << " " << clive->Limit() << " " << clive->Key();
	    }
	}
	else if (clive->Key() != "")
	{
	    output << " " << clive->Key();
	}
	else if (clive->Limit())
	{
	    output << " " << clive->Limit();
	}

	Parent->chanserv.stats.i_Mode++;
	::send(mynick, source, output);
	return;
    }

    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
}

void ChanServ::do_Op(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Op", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *chan = &Parent->chanserv.stored[channel.LowerCase()];
    channel = chan->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(chan->GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
		    target.c_str());
	    return;
	}
	else if (!Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTIN"),
		    target.c_str(), channel.c_str());
	    return;
	}
	else if (Parent->chanserv.live[channel.LowerCase()].IsOp(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_OPPED"),
		    target.c_str(), channel.c_str());
	    return;
	}
	else if (chan->Secureops() &&
		!(chan->GetAccess(target, "CMDOP") ||
		chan->GetAccess(target, "AUTOOP")))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/RESTRICT"),
		    Parent->getMessage(source, "CS_SET/SECUREOPS").c_str());
	    return;
	}
    }
    else
    {
	if (!Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTIN"),
		    target.c_str(), channel.c_str());
	    return;
	}
	else if (Parent->chanserv.live[channel.LowerCase()].IsOp(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OPPED"),
		    channel.c_str());
	    return;
	}
	else if (!chan->GetAccess(target, "CMDOP"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	    return;
	}
    }

    Parent->chanserv.stats.i_Op++;
    Parent->chanserv.live[channel.LowerCase()].SendMode("+o " + target);
}

void ChanServ::do_DeOp(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_DeOp", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *chan = &Parent->chanserv.stored[channel.LowerCase()];
    channel = chan->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(chan->GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
		    target.c_str());
	    return;
	}
	else if (!Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTIN"),
		    target.c_str(), channel.c_str());
	    return;
	}
	else if (!Parent->chanserv.live[channel.LowerCase()].IsOp(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTOPPED"),
		    target.c_str(), channel.c_str());
	    return;
	}
    }
    else
    {
	if (!Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTIN"),
		    target.c_str(), channel.c_str());
	    return;
	}
	else if (!Parent->chanserv.live[channel.LowerCase()].IsOp(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/NOTOPPED"),
		    channel.c_str());
	    return;
	}
    }

    Parent->chanserv.stats.i_Deop++;
    Parent->chanserv.live[channel.LowerCase()].SendMode("-o " + target);
}

void ChanServ::do_Voice(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Voice", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *chan = &Parent->chanserv.stored[channel.LowerCase()];
    channel = chan->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(chan->GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
		    target.c_str());
	    return;
	}
	else if (!Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTIN"),
		    target.c_str(), channel.c_str());
	    return;
	}
	else if (Parent->chanserv.live[channel.LowerCase()].IsVoice(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_VOICED"),
		    target.c_str(), channel.c_str());
	    return;
	}
	else if (chan->Secureops() &&
		!(chan->GetAccess(target, "CMDVOICE") ||
		chan->GetAccess(target, "AUTOVOICE")))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/RESTRICT"),
		    Parent->getMessage(source, "CS_SET/SECUREOPS").c_str());
	    return;
	}
    }
    else
    {
	if (!Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTIN"),
		    target.c_str(), channel.c_str());
	    return;
	}
	else if (Parent->chanserv.live[channel.LowerCase()].IsVoice(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/VOICED"),
		    channel.c_str());
	    return;
	}
	else if (!chan->GetAccess(target, "CMDVOICE"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	    return;
	}
    }

    Parent->chanserv.stats.i_Voice++;
    Parent->chanserv.live[channel.LowerCase()].SendMode("+v " + target);
}

void ChanServ::do_DeVoice(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_DeVoice", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *chan = &Parent->chanserv.stored[channel.LowerCase()];
    channel = chan->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(chan->GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
		    target.c_str());
	    return;
	}
	else if (!Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTIN"),
		    target.c_str(), channel.c_str());
	    return;
	}
	else if (!Parent->chanserv.live[channel.LowerCase()].IsVoice(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTVOICED"),
		    target.c_str(), channel.c_str());
	    return;
	}
    }
    else
    {
	if (!Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTIN"),
		    target.c_str(), channel.c_str());
	    return;
	}
	else if (!Parent->chanserv.live[channel.LowerCase()].IsVoice(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/NOTVOICED"),
		    channel.c_str());
	    return;
	}
    }

    Parent->chanserv.stats.i_Devoice++;
    Parent->chanserv.live[channel.LowerCase()].SendMode("-v " + target);
}

void ChanServ::do_Topic(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Topic", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring topic     = params.After(" ", 2);

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *chan = &Parent->chanserv.stored[channel.LowerCase()];
    channel = chan->Name();

    if (!chan->GetAccess(source, "CMDMODE"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    Parent->chanserv.stats.i_Topic++;
    Parent->server.TOPIC(mynick, channel, topic);
}

void ChanServ::do_Kick(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Kick", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = params.ExtractWord(3, " ");
    mstring reason;
    if (params.WordCount(" ") >= 4)
	reason = params.After(" ", 3);
    else
	reason = Parent->chanserv.DEF_Akick_Reason();

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *chan = &Parent->chanserv.stored[channel.LowerCase()];
    channel = chan->Name();

    if (!chan->GetAccess(source, "CMDKICK"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (!Parent->nickserv.IsLive(target))
    {
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
		    target.c_str());
	return;
    }

    if (!Parent->chanserv.live[channel.LowerCase()].IsIn(target))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTIN"),
		    target.c_str(), channel.c_str());
	return;
    }

    if (chan->GetAccess(target) > chan->GetAccess(source))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/HIGHERACCESS"),
		    target.c_str(), channel.c_str());
	return;
    }

    mstring output;
    output.Format(Parent->getMessage(target, "CS_COMMAND/KICK").c_str(),
	    source.c_str(), reason.c_str());
    Parent->chanserv.stats.i_Kick++;
    Parent->server.KICK(mynick, target, channel, output);
}

void ChanServ::do_AnonKick(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_AnonKick", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = params.ExtractWord(3, " ");
    mstring reason    = params.After(" ", 3);

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *chan = &Parent->chanserv.stored[channel.LowerCase()];
    channel = chan->Name();

    if (!chan->GetAccess(source, "SUPER"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (!Parent->nickserv.IsLive(target))
    {
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
		    target.c_str());
	return;
    }

    if (!Parent->chanserv.live[channel.LowerCase()].IsIn(target))
    {
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTIN"),
		    target.c_str(), channel.c_str());
	return;
    }

    if (chan->GetAccess(target) > chan->GetAccess(source))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/HIGHERACCESS"),
		    target.c_str(), channel.c_str());
	return;
    }

    Parent->chanserv.stats.i_Anonkick++;
    Parent->server.KICK(mynick, target, channel, reason);
}

void ChanServ::do_Users(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Users", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.stored[channel.LowerCase()].GetAccess(source,
								"VIEW"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    Chan_Live_t *chan = &Parent->chanserv.live[channel.LowerCase()];
    channel = chan->Name();
    unsigned int i;
    mstring user, output = channel + ": ";

    for (i=0; i<chan->Users(); i++)
    {
	user = chan->User(i);
	if (output.size() + user.Len() > 450)
	{
	    ::send(mynick, source, output);
	    output = channel + ": ";
	}
	if (chan->IsOp(user))
	{
	    output << "@";
	}
	else if (chan->IsVoice(user))
	{
	    output << "+";
	}
	output << user << " ";
    }
    if (output.Len() > channel.Len() + 2)
	::send(mynick, source, output);

    output = channel + " (SPLIT): ";
    for (i=0; i<chan->Squit(); i++)
    {
	user = chan->Squit(i);
	if (output.size() + user.Len() > 450)
	{
	    ::send(mynick, source, output);
	    output = channel + " (SQUIT): ";
	}
	output << user << " ";
    }
    if (output.Len() > channel.Len() + 10)
	::send(mynick, source, output);
}

void ChanServ::do_Invite(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Invite", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *chan = &Parent->chanserv.stored[channel.LowerCase()];
    channel = chan->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(chan->GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))))
    {
	target = params.ExtractWord(2, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
		    target.c_str());
	    return;
	}
	else if (Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_IN"),
		    target.c_str(), channel.c_str());
	    return;
	}
	::send(mynick, target, Parent->getMessage(source, "CS_COMMAND/OTH_INVITE"),
		    channel.c_str(), source.c_str());
    }
    else
    {
	if (Parent->chanserv.live[channel.LowerCase()].IsIn(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/IN"),
		    channel.c_str());
	    return;
	}
	else if (!chan->GetAccess(target, "CMDINVITE"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	    return;
	}
	::send(mynick, target, Parent->getMessage(source, "CS_COMMAND/INVITE"),
		channel.c_str());
    }

    Parent->chanserv.stats.i_Invite++;
    Parent->server.INVITE(mynick, target, channel);
}

void ChanServ::do_Unban(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Unban", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 2 && 
	(cstored->GetAccess(source, "SUPER") ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source))))
    {
	target = params.ExtractWord(2, " ");
	if (!Parent->nickserv.IsLive(target))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTINUSE"),
		    target.c_str());
	    return;
	}
    }
    else
    {
	if (!cstored->GetAccess(target, "CMDUNBAN"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	    return;
	}
    }

    Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
    Nick_Live_t *nlive = &Parent->nickserv.live[target.LowerCase()];
    unsigned int i;
    bool found = false;
    for (i=0; i < clive->Bans(); i++)
    {
	if (nlive->Mask(Nick_Live_t::N_U_P_H).Matches(clive->Ban(i).LowerCase()))
	{
	    clive->SendMode("-b " + clive->Ban(i));
	    i--;
	    found = true;
	}
    }
    if (found)
    {
	Parent->chanserv.stats.i_Unban++;
	if (target == source)
	    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNBANNED"),
		    channel.c_str());
	else
	    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/OTH_UNBANNED"),
		    target.c_str(), channel.c_str());
    }
    else
    {
	if (target == source)
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/NOTBANNED"),
		    channel.c_str());
	else
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_NOTBANNED"),
		    target.c_str(), channel.c_str());
    }
}

void ChanServ::do_Live(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Live", (mynick, source, params));

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

    ::send(mynick, source, Parent->getMessage(source, "LIST/CHAN_LIST"),
					mask.c_str());
    map<mstring, Chan_Live_t>::iterator iter;

    for (iter = Parent->chanserv.live.begin(), i=0, count = 0;
			iter != Parent->chanserv.live.end(); iter++)
    {
	if (iter->second.Name().LowerCase().Matches(mask))
	{
	    if (i < listsize)
	    {
		::send(mynick, source, "%s (%du %do %dv %ds %db): +%s %s %d",
					iter->second.Name().c_str(),
					iter->second.Users(),
					iter->second.Ops(),
					iter->second.Voices(),
					iter->second.Squit(),
					iter->second.Bans(),
					iter->second.Mode().c_str(),
					iter->second.Key().c_str(),
					iter->second.Limit());
		i++;
	    }
	    count++;
	}
    }
    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAYED"),
							i, count);
}


void ChanServ::do_clear_Users(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_Users", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
    unsigned int i;
    Parent->chanserv.stats.i_Clear++;
    for (i=0; i<clive->Users(); i++)
    {
	mstring output;
	output.Format(Parent->getMessage(clive->User(i), "CS_COMMAND/CLEAR").c_str(),
		message.c_str(), source.c_str(), channel.c_str());
	Parent->server.KICK(mynick, clive->User(i), channel, output);
    }
}

void ChanServ::do_clear_Ops(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_Ops", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
    unsigned int i;
    for (i=0; i<clive->Ops(); i++)
    {
	if (!message.After(" ").Matches("*ALL*"))
	    ::send(mynick, clive->Op(i), Parent->getMessage(clive->Op(i), "CS_COMMAND/CLEAR"),
		    message.c_str(), source.c_str(), channel.c_str());
	clive->SendMode("-o " + clive->Op(i));
    }
    if (!message.After(" ").Matches("*ALL*"))
	Parent->chanserv.stats.i_Clear++;
}

void ChanServ::do_clear_Voices(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_Voices", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
    unsigned int i;

    for (i=0; i<clive->Voices(); i++)
    {
	if (!message.After(" ").Matches("*ALL*"))
	    ::send(mynick, clive->Voice(i), Parent->getMessage(clive->Voice(i), "CS_COMMAND/CLEAR"),
		    message.c_str(), source.c_str(), channel.c_str());
	clive->SendMode("-v " + clive->Voice(i));
    }
    if (!message.After(" ").Matches("*ALL*"))
    {
	for (i=0; i<clive->Ops(); i++)
	{
	    ::send(mynick, clive->Op(i), Parent->getMessage(clive->Op(i), "CS_COMMAND/CLEAR"),
		    message.c_str(), source.c_str(), channel.c_str());
	}
	Parent->chanserv.stats.i_Clear++;
    }
}

void ChanServ::do_clear_Modes(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_Modes", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
    unsigned int i;

    clive->SendMode("-" + clive->Mode() + " " + clive->Key());
    if (!message.After(" ").Matches("*ALL*"))
    {
	for (i=0; i<clive->Ops(); i++)
	{
	    ::send(mynick, clive->Op(i), Parent->getMessage(clive->Op(i), "CS_COMMAND/CLEAR"),
		    message.c_str(), source.c_str(), channel.c_str());
	}
	Parent->chanserv.stats.i_Clear++;
    }
}

void ChanServ::do_clear_Bans(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_Bans", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
    unsigned int i;

    for (i=0; i<clive->Bans(); i++)
    {
	clive->SendMode("-b " + clive->Ban(i));
    }
    if (!message.After(" ").Matches("*ALL*"))
    {
	for (i=0; i<clive->Ops(); i++)
	{
	    ::send(mynick, clive->Op(i), Parent->getMessage(clive->Op(i), "CS_COMMAND/CLEAR"),
		    message.c_str(), source.c_str(), channel.c_str());
	}
	Parent->chanserv.stats.i_Clear++;
    }
}

void ChanServ::do_clear_All(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_All", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTINUSE"),
		channel.c_str());
	return;
    }

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "CMDCLEAR") &&
	!(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    ChanServ::do_clear_Modes(mynick, source, params);
    ChanServ::do_clear_Ops(mynick, source, params);
    ChanServ::do_clear_Voices(mynick, source, params);
    ChanServ::do_clear_Bans(mynick, source, params);

    Parent->chanserv.stats.i_Clear++;
    ::notice(mynick, channel, Parent->getMessage("CS_COMMAND/CLEAR"),
		message.c_str(), source.c_str(), channel.c_str());
}

void ChanServ::do_level_Set(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_level_Set", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 5)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring what      = params.ExtractWord(4, " ");
    mstring level     = params.ExtractWord(5, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored->GetAccess(source) <= Parent->chanserv.Level_Max())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (!level.IsNumber() || level.Contains("."))
    {
        ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER"));
	return;
    }
    long num = atol(level.c_str());

    if (num < Parent->chanserv.Level_Min() ||
	num > Parent->chanserv.Level_Max()+1)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
		Parent->chanserv.Level_Min(),
		Parent->chanserv.Level_Max()+1);
	return;
    }

    MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Level"));
    if (cstored->Level_find(what))
    {
	cstored->Level_change(cstored->Level->Entry(), num, source);
	Parent->chanserv.stats.i_Level++;
	::send(mynick, source, Parent->getMessage(source, "LIST/CHANGE2_LEVEL"),
		    cstored->Level->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/LEVEL").c_str(),
		    cstored->Level->Value());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2"),
		    what.c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/LEVEL").c_str());
    }
}

void ChanServ::do_level_Reset(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_level_Reset", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring what      = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored->GetAccess(source) <= Parent->chanserv.Level_Max())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Level"));
    if (cstored->Level_find(what) &&
	Parent->chanserv.LVL(what) > Parent->chanserv.Level_Min())
    {
	cstored->Level_change(cstored->Level->Entry(),
				    Parent->chanserv.LVL(what), source);
	Parent->chanserv.stats.i_Level++;
	::send(mynick, source, Parent->getMessage(source, "LIST/CHANGE2_LEVEL"),
		    cstored->Level->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/LEVEL").c_str(),
		    cstored->Level->Value());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2"),
		    what.c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/LEVEL").c_str());
    }
}

void ChanServ::do_level_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_level_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring what = "";
    if (params.WordCount(" ") > 3)
	what = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LEVEL_HEAD"),
		channel.c_str());
    }
    long myaccess = cstored->GetAccess(source);
    bool haveset = cstored->GetAccess(source, "SET");

    MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Level"));
    for (cstored->Level = cstored->Level_begin();
		    cstored->Level != cstored->Level_end();
		    cstored->Level++)
    {
	if (haveset)
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LEVEL_LIST"),
		    cstored->Level->Value(),
		    cstored->Level->Entry().c_str(),
		    Parent->getMessage(source, "CS_SET/LVL_" + cstored->Level->Entry()).c_str());
	}
	else if(cstored->Level->Value() >= 0 &&
		cstored->Level->Value() <= myaccess)
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LEVEL_HAVE"),
		    Parent->getMessage(source, "CS_SET/LVL_" + cstored->Level->Entry()).c_str(),
		    channel.c_str());
	}
    }
}

void ChanServ::do_access_Add(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_access_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 5)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring who       = params.ExtractWord(4, " ");
    mstring level     = params.ExtractWord(5, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "ACCESS"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (!Parent->nickserv.IsStored(who))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
		who.c_str());
	return;
    }
    who = Parent->getSname(who);

    if (!level.IsNumber() || level.Contains("."))
    {
        ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER"));
	return;
    }
    long num = atol(level.c_str());

    if (num < Parent->chanserv.Level_Min() ||
	num > Parent->chanserv.Level_Max())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
		Parent->chanserv.Level_Min(),
		Parent->chanserv.Level_Max());
	return;
    }

    MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Access"));
    if (cstored->Access_find(who))
    {
	mstring entry = cstored->Access->Entry();
	cstored->Access_erase();
	cstored->Access_insert(entry, num, source);
	Parent->chanserv.stats.i_Access++;
	::send(mynick, source, Parent->getMessage(source, "LIST/CHANGE2_LEVEL"),
		    cstored->Access->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str(),
		    num);
    }
    else
    {
	cstored->Access_insert(who, num, source);
	Parent->chanserv.stats.i_Access++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD2_LEVEL"),
		    cstored->Access->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str(),
		    num);
    }
}

void ChanServ::do_access_Del(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_access_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring who       = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "ACCESS"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (who.IsNumber())
    {
	if (who.Contains(".") || who.Contains("-"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_STYNTAX/POSWHOLENUMBER"));
	    return;
	}
	unsigned int i, num = atoi(who);

	MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Access"));
	for (i=1, cstored->Access = cstored->Access_begin();
				i<num && cstored->Access != cstored->Access_end();
				i++, cstored->Access++) ;
	if (cstored->Access != cstored->Access_end())
	{
	    Parent->chanserv.stats.i_Access++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL2"),
		    cstored->Access->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str());
	    cstored->Access_erase();
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2_NUMBER"),
		    num, channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str());
	}
    }
    else
    {
	MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Access"));
	if (cstored->Access_find(who))
	{
	    Parent->chanserv.stats.i_Access++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL2"),
		    cstored->Access->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str());
	    cstored->Access_erase();
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2"),
		    who.c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str());
	}
    }
}

void ChanServ::do_access_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_access_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "VIEW") &&
	!(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->Access_size())
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY2"),
		    channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY2"),
		    channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str());
	return;
    }

    MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Access"));
    unsigned int i;
    mstring format = "%4d. %3d %s (" +
		    Parent->getMessage(source, "LIST/LASTMOD") + ")";

    for (i=1, cstored->Access = cstored->Access_begin();
	cstored->Access != cstored->Access_end(); cstored->Access++, i++)
    {
	::send(mynick, source, format, i, cstored->Access->Value(),
		    cstored->Access->Entry().c_str(),
		    cstored->Access->Last_Modify_Time().Ago().c_str(),
		    cstored->Access->Last_Modifier().c_str());
    }
}

void ChanServ::do_akick_Add(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_akick_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring who       = params.ExtractWord(4, " ");
    mstring reason    = Parent->chanserv.DEF_Akick_Reason();
    if (params.WordCount(" ") > 4)
	reason        = params.After(" ", 4);

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "AKICK"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (who.Contains("!") || who.Contains("@"))
    {
	if (!who.Contains("@"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAIN"),
		    Parent->getMessage(source, "LIST/AKICK").c_str(), '@');
	    return;
	}
	else if (!who.Contains("!"))
	{
	    who.Prepend("*!");
	}

	unsigned int i, num;
	bool super = cstored->GetAccess(source, "SUPER");
	// i+1 below because unsigned i will always be >= 0
	for (i=who.size()-1, num=0; i+1>0; i--)
	{
	    switch (who[i])
	    {
	    case '@':
		if (!super)
		    i=0;
		break;
	    case '!':	// ALL these constitute wildcards.
	    case '*':
	    case '?':
	    case '.':
		break;
	    default:
		num++;
	    }
	}
	// IF we have less than 1 char for 
	if (!super && num <= Parent->config.Starthresh())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/STARTHRESH"),
			Parent->getMessage(source, "LIST/AKICK").c_str(),
			Parent->config.Starthresh());
	    return;
	}
	else if (num <= 1)
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/STARTHRESH"),
			Parent->getMessage(source, "LIST/AKICK").c_str(), 1);
	    return;
	}
    }
    else if (!Parent->nickserv.IsStored(who))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }
    who = Parent->getSname(who);

    MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Akick"));
    if (cstored->Akick_find(who))
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EXISTS2"),
		who.c_str(), channel.c_str(),
		Parent->getMessage(source, "LIST/AKICK").c_str());
    }
    else
    {
	cstored->Akick_insert(who, reason, source);
	Parent->chanserv.stats.i_Akick++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD2"),
		who.c_str(), channel.c_str(),
		Parent->getMessage(source, "LIST/AKICK").c_str());
    }
}

void ChanServ::do_akick_Del(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_akick_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring who       = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "AKICK"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (who.IsNumber())
    {
	if (who.Contains(".") || who.Contains("-"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_STYNTAX/POSWHOLENUMBER"));
	    return;
	}
	unsigned int i, num = atoi(who);

	MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Akick"));
	for (i=1, cstored->Akick = cstored->Akick_begin();
				i<num && cstored->Akick != cstored->Akick_end();
				i++, cstored->Akick++) ;
	if (cstored->Akick != cstored->Akick_end())
	{
	    Parent->chanserv.stats.i_Akick++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL2"),
		    cstored->Akick->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/AKICK").c_str());
	    cstored->Akick_erase();
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2_NUMBER"),
		    num, channel.c_str(),
		    Parent->getMessage(source, "LIST/AKICK").c_str());
	}
    }
    else
    {
	if (!who.Contains("@"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAIN"),
		    Parent->getMessage(source, "LIST/AKICK").c_str(), '@');
	    return;
	}
	else if (!who.Contains("!"))
	{
	    who.Prepend("*!");
	}

	MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Akick"));
	if (cstored->Akick_find(who))
	{
	    Parent->chanserv.stats.i_Akick++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL2"),
		    cstored->Akick->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/AKICK").c_str());
	    cstored->Akick_erase();
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2"),
		    who.c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/AKICK").c_str());
	}
    }
}

void ChanServ::do_akick_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_akick_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "VIEW") &&
	!(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->Akick_size())
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY2"),
		    channel.c_str(),
		    Parent->getMessage(source, "LIST/AKICK").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY2"),
		    channel.c_str(),
		    Parent->getMessage(source, "LIST/AKICK").c_str());
	return;
    }

    MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Akick"));
    mstring format = "%4d. %s (" +
		    Parent->getMessage(source, "LIST/LASTMOD") + ")";
    int i;
    for (i=1, cstored->Akick = cstored->Akick_begin();
	cstored->Akick == cstored->Akick_end(); cstored->Akick++, i++)
    {
	::send(mynick, source, format, i, cstored->Akick->Entry().c_str(),
		    cstored->Akick->Last_Modify_Time().Ago().c_str(),
		    cstored->Akick->Last_Modifier().c_str());
	::send(mynick, source, "      %s", cstored->Akick->Value().c_str());
    }
}

void ChanServ::do_greet_Add(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_greet_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring target    = source;
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if ((option[0U] == '@' || option[0U] == '!') &&
	params.WordCount(" ") > 4 &&
	cstored->GetAccess(source, "OVERGREET"))
    {
	if (option[0U]=='@')
	{
	    target = params.ExtractWord(4, " ").After("@");
	    option = params.After(" ", 4);
	    if (!Parent->nickserv.IsStored(target))
	    {
		::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
		    target.c_str());
		return;
	    }
	}
	while (option[1U] == '!')
	    option = option.After("!");
    }
    else if (!cstored->GetAccess(source, "GREET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }
    else
    {
	while (option[0U] == '!')
	    option = option.After("!");
    }

    if (Parent->nickserv.stored[target.LowerCase()].Host() != "" &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[target.LowerCase()].Host()))
    {
	target = Parent->nickserv.stored[target.LowerCase()].Host();
    }
    target = Parent->getSname(target);

    if (cstored->Greet_find(target))
    {
	if (cstored->Greet->Entry()[0U] == '!' && source == target &&
	    !cstored->GetAccess(source, "OVERGREET"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/LOCKGREET"),
		    channel.c_str());
	    return;
	}
	cstored->Greet_erase();
    }
    cstored->Greet_insert(option, target);
    Parent->chanserv.stats.i_Greet++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/ADD2"),
		target.c_str(), channel.c_str(),
		Parent->getMessage(source, "LIST/GREET").c_str());
}

void ChanServ::do_greet_Del(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_greet_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = source;

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 3 &&
	cstored->GetAccess(source, "OVERGREET"))
    {
	target = params.ExtractWord(3, " ").After("!");
	if (!cstored->Greet_find(target))
	{
	    if (Parent->nickserv.IsStored(target) &&
		Parent->nickserv.stored[target.LowerCase()].Host() != "" &&
		Parent->nickserv.IsStored(Parent->nickserv.stored[target.LowerCase()].Host()))
	    {
		target = Parent->nickserv.stored[target.LowerCase()].Host();
	    }
	    target = Parent->getSname(target);

	    if (!cstored->Greet_find(target))
	    {
		::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2"),
			target.c_str(), channel.c_str(),
			Parent->getMessage(source, "LIST/GREET").c_str());
		return;
	    }
	}
    }
    else if (!cstored->GetAccess(source, "GREET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (source != target)
    {
	cstored->Greet_erase();
	Parent->chanserv.stats.i_Greet++;
	::send(mynick, source, Parent->getMessage(source, "LIST/DEL2"),
			target.c_str(), channel.c_str(),
			Parent->getMessage(source, "LIST/GREET").c_str());
    }
    else
    {
	if (Parent->nickserv.stored[target.LowerCase()].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[target.LowerCase()].Host()))
	{
	    target = Parent->nickserv.stored[target.LowerCase()].Host();
	}
	target = Parent->getSname(target);

	if (cstored->Greet_find(target))
	{
	    if (cstored->Greet->Entry()[0U] == '!' &&
		!cstored->GetAccess(source, "OVERGREET"))
	    {
		::send(mynick, source, Parent->getMessage(source, "CS_STATUS/LOCKGREET"),
			    channel.c_str());
		return;
	    }
	    cstored->Greet_erase();
	    Parent->chanserv.stats.i_Greet++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL2"),
			target.c_str(), channel.c_str(),
			Parent->getMessage(source, "LIST/GREET").c_str());
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2"),
			target.c_str(), channel.c_str(),
			Parent->getMessage(source, "LIST/GREET").c_str());
	}
    }
}

void ChanServ::do_greet_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_greet_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target = source;
    bool all = false;

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (params.WordCount(" ") > 3 &&
	cstored->GetAccess(source, "OVERGREET"))
    {
	if (params.ExtractWord(3, " ").CmpNoCase("all") == 0)
	    all = true;
    }
    else if (!cstored->GetAccess(source, "GREET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (Parent->nickserv.stored[target.LowerCase()].Host() != "" &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[target.LowerCase()].Host()))
    {
	target = Parent->nickserv.stored[target.LowerCase()].Host();
    }
    target = Parent->getSname(target);

    bool found = false;
    for (cstored->Greet = cstored->Greet_begin();
		    cstored->Greet != cstored->Greet_end(); cstored->Greet++)
    {
	if (cstored->Greet->Last_Modifier().LowerCase() == target.LowerCase()
	    || all)
	{
	    ::send(mynick, source, "[%s] %s",
				cstored->Greet->Last_Modifier().c_str(),
				cstored->Greet->Entry().c_str());
	    found = true;
	}
    }
    if (!found)
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2"),
			target.c_str(), channel.c_str(),
			Parent->getMessage(source, "LIST/GREET").c_str());
    }
}

void ChanServ::do_message_Add(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_message_Add", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring text      = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "MESSAGE"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    cstored->Message_insert(text, source);
    Parent->chanserv.stats.i_Message++;
    ::send(mynick, source, Parent->getMessage(source, "LIST/ADD2_NUMBER"),
		cstored->Message_size(), channel.c_str(),
		Parent->getMessage(source, "LIST/JOINMSG").c_str());
}

void ChanServ::do_message_Del(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_message_Del", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel = params.ExtractWord(2, " ");
    mstring target  = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "MESSAGE"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (!target.IsNumber() || target.Contains("."))
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/POSWHOLENUMBER"));
	return;
    }
    int num = atoi(target);

    if (!cstored->Message_size())
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY2"),
		channel.c_str(),
		Parent->getMessage(source, "LIST/JOINMSG").c_str());
	return;
    }

    if (cstored->Message_find(num))
    {
        cstored->Message_erase();
	Parent->chanserv.stats.i_Message++;
	::send(mynick, source, Parent->getMessage(source, "LIST/DEL2_NUMBER"),
		num, channel.c_str(),
		Parent->getMessage(source, "LIST/JOINMSG").c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/NOTEXISTS2_NUMBER"),
		num, channel.c_str(),
		Parent->getMessage(source, "LIST/JOINMSG").c_str());
    }
}

void ChanServ::do_message_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_message_List", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "MESSAGE"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (!cstored->Message_size())
    {
	::send(mynick, source, Parent->getMessage(source, "LIST/EMPTY2"),
		channel.c_str(),
		Parent->getMessage(source, "LIST/JOINMSG").c_str());
	return;
    }

    ::send(mynick, source, Parent->getMessage(source, "LIST/DISPLAY2"),
	    channel.c_str(), Parent->getMessage(source, "LIST/JOINMSG").c_str());
    int i;
    for (i=1, cstored->Message = cstored->Message_begin();
				cstored->Message != cstored->Message_end();
				cstored->Message++, i++)
    {
        ::send(mynick, source, "%d. %s", i, cstored->Message->Entry().c_str());
    }
}

void ChanServ::do_set_Founder(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Founder", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring founder   = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored->GetAccess(source) <= Parent->chanserv.Level_Max())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (!Parent->nickserv.IsStored(founder))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
		founder.c_str());
	return;
    }
    else if (Parent->nickserv.stored[founder.LowerCase()].Host() != "" &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[founder.LowerCase()].Host()))
    {
	founder = Parent->nickserv.stored[founder.LowerCase()].Host();
    }
    founder = Parent->getSname(founder);

    cstored->Founder(founder);
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/FOUNDER").c_str(),
	    channel.c_str(), founder.c_str());
}

void ChanServ::do_set_CoFounder(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_CoFounder", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring founder   = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored->GetAccess(source) <= Parent->chanserv.Level_Max())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (!Parent->nickserv.IsStored(founder))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
		founder.c_str());
	return;
    }
    else if (Parent->nickserv.stored[founder.LowerCase()].Host() != "" &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[founder.LowerCase()].Host()))
    {
	founder = Parent->nickserv.stored[founder.LowerCase()].Host();
    }
    founder = Parent->getSname(founder);

    if (cstored->Founder().LowerCase() == founder.LowerCase())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/COFOUNDER"));
	return;
    }

    cstored->CoFounder(founder);
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/COFOUNDER").c_str(),
	    channel.c_str(), founder.c_str());
}

void ChanServ::do_set_Description(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Description", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    cstored->Description(option);
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/DESCRIPTION").c_str(),
	    channel.c_str(), option.c_str());
}

void ChanServ::do_set_Password(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Password", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring password  = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (cstored->GetAccess(source) <= Parent->chanserv.Level_Max())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (password.Len() < 5 || password.CmpNoCase(cstored->Name()) == 0 ||
	password.CmpNoCase(source) == 0)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/COMPLEX_PASS"));
	return;
    }

    cstored->Password(password);
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/PASSWORD").c_str(),
	    channel.c_str(), password.c_str());
}

void ChanServ::do_set_Email(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Email", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (option.CmpNoCase("none") == 0)
	option = "";
    cstored->Email(option);
    Parent->chanserv.stats.i_Set++;
    if (option == "")
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNSET"),
	    Parent->getMessage(source, "CS_SET/EMAIL").c_str(),
	    channel.c_str());
    else
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/EMAIL").c_str(),
	    channel.c_str(), option.c_str());
}

void ChanServ::do_set_URL(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_URL", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (option.CmpNoCase("none") == 0)
	option = "";
    cstored->URL(option);
    Parent->chanserv.stats.i_Set++;
    if (option == "")
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNSET"),
	    Parent->getMessage(source, "CS_SET/URL").c_str(),
	    channel.c_str());
    else
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/URL").c_str(),
	    channel.c_str(), option.c_str());
}

void ChanServ::do_set_Comment(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Comment", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (option.CmpNoCase("none") == 0)
	option = "";
    cstored->Comment(option);
    Parent->chanserv.stats.i_Set++;
    if (option == "")
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNSET"),
	    Parent->getMessage(source, "CS_SET/COMMENT").c_str(),
	    channel.c_str());
    else
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/COMMENT").c_str(),
	    channel.c_str(), option.c_str());
}

void ChanServ::do_set_Mlock(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Mlock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (!option.CmpNoCase("default") || !option.CmpNoCase("reset"))
    {
	option = Parent->chanserv.DEF_MLock();
    }

    vector<mstring> retval = cstored->Mlock(source, option);
    Parent->chanserv.stats.i_Set++;
    for (unsigned int i=0; i<retval.size(); i++)
	::send(mynick, source, retval[i]);
}

void ChanServ::do_set_BanTime(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_BanTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring value     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Bantime())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/BANTIME").c_str(),
		channel.c_str());
	return;
    }

    unsigned long num = FromHumanTime(value);
    cstored->Bantime(num);
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/BANTIME").c_str(),
	    channel.c_str(), ToHumanTime(num).c_str());
}

void ChanServ::do_set_PartTime(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_PartTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring value     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Parttime())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/PARTTIME").c_str(),
		channel.c_str());
	return;
    }

    unsigned long num = FromHumanTime(value);
    cstored->Parttime(num);
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/PARTTIME").c_str(),
	    channel.c_str(), ToHumanTime(num).c_str());
}

void ChanServ::do_set_KeepTopic(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_KeepTopic", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Keeptopic())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/KEEPTOPIC").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Keeptopic())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->Keeptopic(onoff.GetBool());
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/KEEPTOPIC").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_set_TopicLock(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_TopicLock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Topiclock())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/TOPICLOCK").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Topiclock())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->Topiclock(onoff.GetBool());
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/TOPICLOCK").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_set_Private(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Private())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/PRIVATE").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->Private(onoff.GetBool());
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/PRIVATE").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_set_SecureOps(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_SecureOps", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Secureops())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/SECUREOPS").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Secureops())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->Secureops(onoff.GetBool());
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/SECUREOPS").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_set_Secure(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Secure())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/SECURE").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->Secure(onoff.GetBool());
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/SECURE").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_set_NoExpire(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_NoExpire", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (cstored->L_NoExpire())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/NOEXPIRE").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_NoExpire())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->NoExpire(onoff.GetBool());
    Parent->chanserv.stats.i_NoExpire++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/NOEXPIRE").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_set_Anarchy(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Anarchy", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Anarchy())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/ANARCHY").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Anarchy())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->Anarchy(onoff.GetBool());
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/ANARCHY").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_set_Restricted(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Restricted", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Restricted())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/RESTRICTED").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Restricted())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->Restricted(onoff.GetBool());
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/RESTRICTED").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_set_Join(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Join())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/JOIN").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Join())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->Join(onoff.GetBool());
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/JOIN").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
    if (onoff.GetBool() && Parent->chanserv.IsLive(channel) &&
	!Parent->chanserv.live[channel.LowerCase()].IsIn(
		Parent->chanserv.FirstName()))
    {
	Parent->server.JOIN(Parent->chanserv.FirstName(), channel);
    }
    else if (!onoff.GetBool() && Parent->chanserv.IsLive(channel) &&
	Parent->chanserv.live[channel.LowerCase()].IsIn(
		Parent->chanserv.FirstName()))
    {
	Parent->server.PART(Parent->chanserv.FirstName(), channel);
    }

}

void ChanServ::do_set_Revenge(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Revenge", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();
    // If we have 2 params, and we have SUPER access, or are a SOP
    if (!cstored->GetAccess(source, "SET"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    if (cstored->L_Revenge())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/REVENGE").c_str(),
		channel.c_str());
	return;
    }

    if (!option.CmpNoCase("default") || !option.CmpNoCase("reset"))
    {
	option = Parent->chanserv.DEF_Revenge();
    }
    else if (!Parent->chanserv.IsRevengeLevel(option))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/NOREVENGE"),
		option.UpperCase().c_str());
	return;
    }

    cstored->Revenge(option.UpperCase());
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/REVENGE").c_str(),
	    channel.c_str(),
	    Parent->getMessage(source, "CS_SET/REV_" + option.UpperCase()).c_str());
}

void ChanServ::do_lock_Mlock(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Mlock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.After(" ", 3);

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (!option.CmpNoCase("default") || !option.CmpNoCase("reset"))
    {
	option = Parent->chanserv.DEF_MLock();
    }

    vector<mstring> retval = cstored->L_Mlock(source, option);
    Parent->chanserv.stats.i_Lock++;
    for (unsigned int i=0; i<retval.size(); i++)
	::send(mynick, source, retval[i]);
}

void ChanServ::do_lock_BanTime(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_BanTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring value     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Bantime())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/BANTIME").c_str(),
		channel.c_str());
	return;
    }

    unsigned long num = FromHumanTime(value);
    cstored->L_Bantime(false);
    cstored->Bantime(num);
    cstored->L_Bantime(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/BANTIME").c_str(),
	    channel.c_str(), ToHumanTime(num).c_str());
}

void ChanServ::do_lock_PartTime(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_PartTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring value     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Parttime())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/PARTTIME").c_str(),
		channel.c_str());
	return;
    }

    unsigned long num = FromHumanTime(value);
    cstored->L_Parttime(false);
    cstored->Parttime(num);
    cstored->L_Parttime(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/PARTTIME").c_str(),
	    channel.c_str(), ToHumanTime(num).c_str());
}

void ChanServ::do_lock_KeepTopic(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_KeepTopic", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Keeptopic())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/KEEPTOPIC").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Keeptopic())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->L_Keeptopic(false);
    cstored->Keeptopic(onoff.GetBool());
    cstored->L_Keeptopic(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/KEEPTOPIC").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_lock_TopicLock(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_TopicLock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Topiclock())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/TOPICLOCK").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Topiclock())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->L_Topiclock(false);
    cstored->Topiclock(onoff.GetBool());
    cstored->L_Topiclock(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/TOPICLOCK").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_lock_Private(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Private())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/PRIVATE").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->L_Private(false);
    cstored->Private(onoff.GetBool());
    cstored->L_Private(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/PRIVATE").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_lock_SecureOps(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_SecureOps", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Secureops())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/SECUREOPS").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Secureops())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->L_Secureops(false);
    cstored->Secureops(onoff.GetBool());
    cstored->L_Secureops(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/SECUREOPS").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_lock_Secure(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Secure())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/SECURE").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->L_Secure(false);
    cstored->Secure(onoff.GetBool());
    cstored->L_Secure(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/SECURE").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_lock_Anarchy(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Anarchy", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Anarchy())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/ANARCHY").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Anarchy())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->L_Anarchy(false);
    cstored->Anarchy(onoff.GetBool());
    cstored->L_Anarchy(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/ANARCHY").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_lock_Restricted(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Restricted", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Restricted())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/RESTRICTED").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Restricted())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->L_Restricted(false);
    cstored->Restricted(onoff.GetBool());
    cstored->L_Restricted(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/RESTRICTED").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_lock_Join(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Join())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/JOIN").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_Join())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->L_Join(false);
    cstored->Join(onoff.GetBool());
    cstored->L_Join(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/JOIN").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "MISC/ON").c_str() :
		Parent->getMessage(source, "MISC/OFF").c_str());
}

void ChanServ::do_lock_Revenge(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Revenge", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring option    = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Revenge())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/REVENGE").c_str(),
		channel.c_str());
	return;
    }

    if (!option.CmpNoCase("default") || !option.CmpNoCase("reset"))
    {
	option = Parent->chanserv.DEF_Revenge();
    }
    else if (!Parent->chanserv.IsRevengeLevel(option))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/NOREVENGE"),
		option.UpperCase().c_str());
	return;
    }

    cstored->L_Revenge(false);
    cstored->Revenge(option.UpperCase());
    cstored->L_Revenge(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/REVENGE").c_str(),
	    channel.c_str(),
	    Parent->getMessage(source, "CS_SET/REV_" + option.UpperCase()).c_str());
}

void ChanServ::do_unlock_Mlock(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_Mlock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    vector<mstring> retval = cstored->L_Mlock(source, "");
    Parent->chanserv.stats.i_Unlock++;
    for (unsigned int i=0; i<retval.size(); i++)
	::send(mynick, source, retval[i]);
}

void ChanServ::do_unlock_BanTime(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_BanTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Bantime())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/BANTIME").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Bantime(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/BANTIME").c_str(),
	    channel.c_str());
}

void ChanServ::do_unlock_PartTime(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_PartTime", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Parttime())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/PARTTIME").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Parttime(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/PARTTIME").c_str(),
	    channel.c_str());
}

void ChanServ::do_unlock_KeepTopic(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_KeepTopic", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Keeptopic())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/KEEPTOPIC").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Keeptopic(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/KEEPTOPIC").c_str(),
	    channel.c_str());
}

void ChanServ::do_unlock_TopicLock(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_TopicLock", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring onoff     = params.ExtractWord(4, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Topiclock())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/TOPICLOCK").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Topiclock(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/TOPICLOCK").c_str(),
	    channel.c_str());
}

void ChanServ::do_unlock_Private(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_Private", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
 
    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Private())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/PRIVATE").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Private(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/PRIVATE").c_str(),
	    channel.c_str());
}

void ChanServ::do_unlock_SecureOps(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_SecureOps", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Secureops())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/SECUREOPS").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Secureops(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/SECUREOPS").c_str(),
	    channel.c_str());
}

void ChanServ::do_unlock_Secure(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_Secure", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Secure())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/SECURE").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Secure(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/SECURE").c_str(),
	    channel.c_str());
}

void ChanServ::do_unlock_Anarchy(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_Anarchy", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Anarchy())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/ANARCHY").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Anarchy(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/ANARCHY").c_str(),
	    channel.c_str());
}

void ChanServ::do_unlock_Restricted(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_Restricted", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Restricted())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/RESTRICTED").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Restricted(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/RESTRICTED").c_str(),
	    channel.c_str());
}

void ChanServ::do_unlock_Join(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Join())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/JOIN").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Join(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/JOIN").c_str(),
	    channel.c_str());
}

void ChanServ::do_unlock_Revenge(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_Revenge", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		channel.c_str());
	return;
    }

    Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
    channel = cstored->Name();

    if (Parent->chanserv.LCK_Revenge())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/REVENGE").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_Revenge(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/REVENGE").c_str(),
	    channel.c_str());
}


long ChanServ::LVL(mstring level)
{
    FT("ChanServ::LVL", (level));
    if (!IsLVL(level) ||
	lvl[level.UpperCase()] > Level_Max() + 1 ||
	lvl[level.UpperCase()] < Level_Min())
    {
	RET(Level_Min()-1);
    }
    else
    {
	RET(lvl[level.UpperCase()]);
    }
}


vector<mstring> ChanServ::LVL()
{
    NFT("ChanServ::LVL");

    vector<mstring> retval;
    map<mstring, long>::iterator iter;
    for (iter = lvl.begin(); iter != lvl.end(); iter++)
    {
	retval.push_back(iter->first.UpperCase());
    }
    NRET(vector<mstring>, retval);
}


bool ChanServ::IsLVL(mstring level)
{
    FT("ChanServ::IsLVL", (level));
    RET(lvl.find(level.UpperCase()) != lvl.end());
}


void ChanServ::save_database(wxOutputStream& out)
{
    FT("ChanServ::save_database", ("(wxOutputStream &) out"));
	map<mstring,Chan_Stored_t>::iterator i;
	CP(("Saving CHANNEL entries (%d) ...", stored.size()));
	out<<stored.size();
        // todo call script saving hooks.
	for(i=stored.begin();i!=stored.end();i++)
	{
	    out<<i->second;
	    // todo call script saving hooks.
	    COM(("Entry CHANNEL %s saved ...", i->second.Name().c_str()));
	}
}

void ChanServ::load_database(wxInputStream& in)
{
    FT("ChanServ::load_database", ("(wxInputStream &) in"));
    map<mstring,Chan_Stored_t>::size_type i,j;
    in>>i;
    CP(("Loading CHANNEL entries (%d) ...", i));
    Chan_Stored_t tmpstored;
    for(j=0;j<i;j++)
    {
	COM(("Loading CHANNEL entry %d ...", j));
	in>>tmpstored;
	stored[tmpstored.Name().LowerCase()]=tmpstored;
	COM(("Entry CHANNEL %s loaded ...", tmpstored.Name().c_str()));
    }
}


