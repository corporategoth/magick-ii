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
    {
	wxLogWarning("Duplicate JOIN message for %s in %s received.", nick.c_str(), i_Name.c_str());	
    }
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
    if (users.find(nick.LowerCase())!=users.end())
    {
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
	wxLogWarning("PART received for %s who is not in %s.", nick.c_str(), i_Name.c_str());

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
	wxLogWarning("UNSQUIT for %s received, and is not in %s.", nick.c_str(), i_Name.c_str());
    }
    else
	Part(nick);
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


int Chan_Live_t::Squit()
{
    NFT("Chan_Livt_t::Squit");
    RET(squit.size());
}


mstring Chan_Live_t::Squit(int num)
{
    FT("Chan_Live_t::Squit", (num));
    int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    for(i=0, k=squit.begin();k!=squit.end();k++, i++)
	if (i==num)
	{
	    RET(k->first);
	}

    RET("");
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


int Chan_Live_t::Bans()
{
    NFT("Chan_Live_t::Bans");
    RET(bans.size());
}


mstring Chan_Live_t::Ban(int num)
{
    FT("Chan_Live_t::Ban", (num));
    int i;
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

    for (i=0; i<in.size(); i++)
    {
	switch (in[i])
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
	    }
	    break;
	case 'l':
	    if (in.WordCount(" ") >= param)
	    {
		if (add)
		{
		    if (ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
			RemoveMode(p_modes_off, p_modes_off_params, false, 'l');
		    if (!ModeExists(p_modes_on, p_modes_on_params, true, 'l', in.ExtractWord(param, " ")))
		    {
			p_modes_on += "l";
			p_modes_on_params.push_back(in.ExtractWord(param, " "));
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
	    }
	    break;
	default:
		if (add)
		{
		    if (ModeExists(p_modes_off, p_modes_off_params, false, in[i]))
			RemoveMode(p_modes_off, p_modes_off_params, false, in[i]);
		    if (!ModeExists(p_modes_on, p_modes_on_params, true, in[i]))
			p_modes_on += in[i];
		}
		else
		{
		    if (ModeExists(p_modes_on, p_modes_on_params, true, in[i]))
			RemoveMode(p_modes_on, p_modes_on_params, true, in[i]);
		    if (!ModeExists(p_modes_off, p_modes_off_params, false, in[i]))
		    {
			p_modes_off += in[i];
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

    l_Mlock_On = l_Mlock_Off = "";
    i_Bantime = Parent->chanserv.DEF_Bantime();
    l_Bantime = false;
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
    i_Restricted=in.i_Restricted;
    l_Restricted=in.l_Restricted;
    i_Join=in.i_Join;
    l_Join=in.l_Join;
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


void Chan_Stored_t::Founder(mstring in)
{
    FT("Chan_Stored_t::Founder", (in));

    if (!Parent->chanserv.IsStored(in))
	return;

    if (i_CoFounder == in)
	i_CoFounder = "";

    i_Founder = in;
}


void Chan_Stored_t::CoFounder(mstring in)
{
    FT("Chan_Stored_t::CoFounder", (in));

    if (!Parent->chanserv.IsStored(in))
	return;

    if (i_Founder == in)
	return;

    i_CoFounder = in;
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
    NFT("Chan_Stored_t::Mlock");
    mstring Result;
    if(i_Mlock_On!="")
	Result+="+" + i_Mlock_On;
    if(i_Mlock_Off!="")
	Result+="-" + i_Mlock_Off;
    RET(Result);
}


mstring Chan_Stored_t::Mlock(mstring mode)
{
    FT("Chan_Stored_t::Mlock", (mode));

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


mstring Chan_Stored_t::L_Mlock()
{
    NFT("Chan_Stored_t::L_Mlock");
    mstring Result;
    mstring mode_on = l_Mlock_On;
    mstring mode_off = l_Mlock_Off;
    mstring locked = Parent->chanserv.LCK_MLock();
    bool add = true;
    unsigned int i;
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


mstring Chan_Stored_t::L_Mlock(mstring mode)
{
    FT("Chan_Stored_t::L_Mlock", (mode));

    l_Mlock_On = l_Mlock_Off = "";
    mstring retval = "";
    mstring change = mode.ExtractWord(1, ": ");
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
    if (l_Mlock_On != "" || l_Mlock_Off != "")
    {
	retval += "MLOCK has been set to ";
	if(l_Mlock_On!="")
	    retval += "+" + l_Mlock_On;
	if(l_Mlock_Off!="")
	    retval += "-" + l_Mlock_Off;
    }
    else
    {
	retval += "MLOCK has been turned off";
    }
    retval += ".";
    RET(retval);
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


bool Chan_Stored_t::Access_Level_find(mstring entry)
{
    FT("Chan_Stored_t::Access_Level_find", (entry));

//  entlist_val_ui<long> iter = i_Access_Level.end();
    set<entlist_val_t<long> >::iterator iter = i_Access_Level.end();
    if (!i_Access_Level.empty())
	for (iter=i_Access_Level.begin(); iter!=i_Access_Level.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;

    if (iter != i_Access_Level.end())
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


wxOutputStream &operator<<(wxOutputStream& out,Chan_Stored_t& in)
{
    out<<in.i_Name<<in.i_RegTime<<in.i_Founder<<in.i_CoFounder<<in.i_Description<<in.i_Password<<in.i_URL<<in.i_Comment;
    out<<in.i_Mlock_On<<in.i_Mlock_Off<<in.i_Mlock_Key<<in.i_Mlock_Limit;
    out<<in.i_Bantime<<in.i_Keeptopic<<in.i_Topiclock<<in.i_Private<<in.i_Secureops<<
	in.i_Secure<<in.i_NoExpire<<in.i_Restricted<<in.i_Join<<in.i_Forbidden;
    out<<in.l_Bantime<<in.l_Keeptopic<<in.l_Topiclock<<in.l_Private<<in.l_Secureops<<
	in.l_Secure<<in.l_NoExpire<<in.l_Restricted<<in.l_Join<<in.l_Mlock_On<<in.l_Mlock_Off;
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
    in>>out.i_Name>>out.i_RegTime>>out.i_Founder>>out.i_CoFounder>>out.i_Description>>out.i_Password>>out.i_URL>>out.i_Comment;
    in>>out.i_Mlock_On>>out.i_Mlock_Off>>out.i_Mlock_Key>>out.i_Mlock_Limit;
    in>>out.i_Bantime>>out.i_Keeptopic>>out.i_Topiclock>>out.i_Private>>out.i_Secureops>>
	out.i_Secure>>out.i_NoExpire>>out.i_Restricted>>out.i_Join>>out.i_Forbidden;
    in>>out.l_Bantime>>out.l_Keeptopic>>out.l_Topiclock>>out.l_Private>>out.l_Secureops>>
	out.l_Secure>>out.l_NoExpire>>out.l_Restricted>>out.l_Join>>out.l_Mlock_On>>out.l_Mlock_Off;
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

void ChanServ::AddCommands()
{
    NFT("ChanServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "HELP", Parent->commserv.ALL_Name(), ChanServ::do_Help);
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
	    "*CLEAR*", Parent->commserv.REGD_Name(), ChanServ::do_Clear);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* SET*", Parent->commserv.REGD_Name(), ChanServ::do_level_Set);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* RESET*", Parent->commserv.REGD_Name(), ChanServ::do_level_Reset);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV LIST", Parent->commserv.REGD_Name(), ChanServ::do_level_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV VIEW", Parent->commserv.REGD_Name(), ChanServ::do_level_List);
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
	    "SET* RES*", Parent->commserv.REGD_Name(), ChanServ::do_set_Restricted);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *JOIN*", Parent->commserv.REGD_Name(), ChanServ::do_set_Join);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* REV*", Parent->commserv.REGD_Name(), ChanServ::do_set_Revenge);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK M*LOCK", Parent->commserv.REGD_Name(), ChanServ::do_lock_Mlock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK BAN*TIME", Parent->commserv.REGD_Name(), ChanServ::do_lock_BanTime);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK KEEP*", Parent->commserv.REGD_Name(), ChanServ::do_lock_KeepTopic);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK TOPIC*", Parent->commserv.REGD_Name(), ChanServ::do_lock_TopicLock);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOOOCK PRIV*", Parent->commserv.REGD_Name(), ChanServ::do_lock_Private);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK SEC*OP*", Parent->commserv.REGD_Name(), ChanServ::do_lock_SecureOps);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK SEC*", Parent->commserv.REGD_Name(), ChanServ::do_lock_Secure);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK RES*", Parent->commserv.REGD_Name(), ChanServ::do_lock_Restricted);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK *JOIN*", Parent->commserv.REGD_Name(), ChanServ::do_lock_Join);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK REV*", Parent->commserv.REGD_Name(), ChanServ::do_lock_Revenge);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK", Parent->commserv.SOP_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ACC*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "A*KICK", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GREET", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LEV*", Parent->commserv.REGD_Name(), do_1_3param);
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

void ChanServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Help", (mynick, source, params));
}

void ChanServ::do_Register(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Register", (mynick, source, params));

    mstring message = params.Before(" ");
    if (params.WordCount(" ") < 4)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring password  = params.ExtractWord(3, " ");
    mstring desc      = params.After(" ", 3);

    if (Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, "Channel " + channel + " is already registered.");
	return;
    }

    if (!Parent->chanserv.IsLive(channel))
    {
	::send(mynick, source, "Channel " + channel + " is not being used.");
	return;
    }

    if (!Parent->chanserv.live[channel.LowerCase()].IsOp(source))
    {
	::send(mynick, source, "You are not opped in " + channel + ".");
	return;
    }

    if (password.Len() < 5 || password.CmpNoCase(channel) == 0 ||
	password.CmpNoCase(source) == 0)
    {
	::send(mynick, source, "Please choose a more complex password.");
	return;
    }

    Parent->chanserv.stored[channel.LowerCase()] =
		Chan_Stored_t(channel, source, password, desc);
    Parent->nickserv.live[source.LowerCase()].ChanIdentify(channel, password);
    ::send(mynick, source, "Channel " + channel + " has been registered.");
}

void ChanServ::do_Drop(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Drop", (mynick, source, params));

    mstring message = params.Before(" ");
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, "Channel " + channel + " is not registered.");
	return;
    }

    if (!Parent->nickserv.live[source.LowerCase()].IsChanIdentified(channel))
    {
	::send(mynick, source, "You are not identified for " + channel + ".");
	return;
    }

    Parent->chanserv.stored.erase(channel.LowerCase());
    ::send(mynick, source, "Channel " + channel + " has been dropped.");
}

void ChanServ::do_Identify(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Identify", (mynick, source, params));

    mstring message = params.Before(" ");
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters.");
	return;
    }

    mstring channel   = params.ExtractWord(2, " ");
    mstring pass      = params.ExtractWord(3, " ");

    if (!Parent->chanserv.IsStored(channel))
    {
	::send(mynick, source, "Channel " + channel + " is not registered.");
	return;
    }

    mstring output = Parent->nickserv.live[source.LowerCase()].ChanIdentify(channel, pass);
    if (output != "")
	::send(mynick, source, output);
}

void ChanServ::do_Info(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Info", (mynick, source, params));

}

void ChanServ::do_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_List", (mynick, source, params));

    int listsize, i, count;
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

    ::send(mynick, source, "Channels matching " + mask + ":");
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
    mstring output;
    output << IRC_Bold << i << IRC_Off << " of " << count << " entries shown.";
    ::send(mynick, source, output);
}

void ChanServ::do_Suspend(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Suspend", (mynick, source, params));
}

void ChanServ::do_UnSuspend(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_UnSuspend", (mynick, source, params));
}

void ChanServ::do_Forbid(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Forbid", (mynick, source, params));
}


void ChanServ::do_Mode(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Mode", (mynick, source, params));
}

void ChanServ::do_Op(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Op", (mynick, source, params));
}

void ChanServ::do_DeOp(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_DeOp", (mynick, source, params));
}

void ChanServ::do_Voice(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Voice", (mynick, source, params));
}

void ChanServ::do_DeVoice(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_DeVoice", (mynick, source, params));
}

void ChanServ::do_Topic(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Topic", (mynick, source, params));
}

void ChanServ::do_Kick(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Kick", (mynick, source, params));
}

void ChanServ::do_AnonKick(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_AnonKick", (mynick, source, params));
}

void ChanServ::do_Users(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Users", (mynick, source, params));
}

void ChanServ::do_Invite(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Invite", (mynick, source, params));
}

void ChanServ::do_Unban(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Unban", (mynick, source, params));
}

void ChanServ::do_Clear(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Clear", (mynick, source, params));
}


void ChanServ::do_level_Set(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_level_Set", (mynick, source, params));
}

void ChanServ::do_level_Reset(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_level_Reset", (mynick, source, params));
}

void ChanServ::do_level_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_level_List", (mynick, source, params));
}

void ChanServ::do_access_Add(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_access_Add", (mynick, source, params));
}

void ChanServ::do_access_Del(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_access_Del", (mynick, source, params));
}

void ChanServ::do_access_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_access_List", (mynick, source, params));
}

void ChanServ::do_akick_Add(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_akick_Add", (mynick, source, params));
}

void ChanServ::do_akick_Del(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_akick_Del", (mynick, source, params));
}

void ChanServ::do_akick_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_akick_List", (mynick, source, params));
}

void ChanServ::do_greet_Add(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_greet_Add", (mynick, source, params));
}

void ChanServ::do_greet_Del(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_greet_Del", (mynick, source, params));
}

void ChanServ::do_greet_List(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_greet_List", (mynick, source, params));
}

void ChanServ::do_set_Founder(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Founder", (mynick, source, params));
}

void ChanServ::do_set_CoFounder(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_CoFounder", (mynick, source, params));
}

void ChanServ::do_set_Description(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Description", (mynick, source, params));
}

void ChanServ::do_set_Password(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Password", (mynick, source, params));
}

void ChanServ::do_set_Email(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Email", (mynick, source, params));
}

void ChanServ::do_set_URL(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_URL", (mynick, source, params));
}

void ChanServ::do_set_Comment(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Comment", (mynick, source, params));
}

void ChanServ::do_set_Mlock(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Mlock", (mynick, source, params));
}

void ChanServ::do_set_BanTime(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_BanTime", (mynick, source, params));
}

void ChanServ::do_set_KeepTopic(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_KeepTopic", (mynick, source, params));
}

void ChanServ::do_set_TopicLock(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_TopicLock", (mynick, source, params));
}

void ChanServ::do_set_Private(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Private", (mynick, source, params));
}

void ChanServ::do_set_SecureOps(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_SecureOps", (mynick, source, params));
}

void ChanServ::do_set_Secure(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Secure", (mynick, source, params));
}

void ChanServ::do_set_NoExpire(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_NoExpire", (mynick, source, params));
}

void ChanServ::do_set_Restricted(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Restricted", (mynick, source, params));
}

void ChanServ::do_set_Join(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Join", (mynick, source, params));
}

void ChanServ::do_set_Revenge(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Revenge", (mynick, source, params));
}

void ChanServ::do_lock_Mlock(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Mlock", (mynick, source, params));
}

void ChanServ::do_lock_BanTime(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_BanTime", (mynick, source, params));
}

void ChanServ::do_lock_KeepTopic(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_KeepTopic", (mynick, source, params));
}

void ChanServ::do_lock_TopicLock(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_TopicLock", (mynick, source, params));
}

void ChanServ::do_lock_Private(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Private", (mynick, source, params));
}

void ChanServ::do_lock_SecureOps(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_SecureOps", (mynick, source, params));
}

void ChanServ::do_lock_Secure(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Secure", (mynick, source, params));
}

void ChanServ::do_lock_NoExpire(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_NoExpire", (mynick, source, params));
}

void ChanServ::do_lock_Restricted(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Restricted", (mynick, source, params));
}

void ChanServ::do_lock_Join(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Join", (mynick, source, params));
}

void ChanServ::do_lock_Revenge(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Revenge", (mynick, source, params));
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
