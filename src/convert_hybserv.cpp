#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
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
#define RCSID(x,y) const char *rcsid_convert_hybserv_cpp_ ## x () { return y; }
RCSID(convert_hybserv_cpp, "@(#)$Id$");

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

#ifdef CONVERT
#include "convert_hybserv.h"
#include "magick.h"

const char *HYB_NickDBName = "nick.db";
const char *HYB_ChanDBName = "chan.db";
const char *HYB_MemoDBName = "memo.db";
const char *HYB_IgnoreDBName = "ignore.db";

/*************************************************************************/


void HYB_load_ns_dbase()
{
    BTCB();
    HYB_NickInfo *ni = NULL;

    mFile dbfile(HYB_NickDBName);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (HYB_NickDBName));

    while (!dbfile.Eof())
    {
	mstring line = dbfile.ReadLine();
	if (line.empty() || line[0u] == ';')
	    continue;

	else if (line[0u] == '-' && line[1u] == '>')
	{
	    if (!ni)
		continue;

	    mstring field = line.After(">").Before(" ");
	    mstring data = line.After(" ");

	    if (field.IsSameAs("PASS", true))
	    {
		ni->password = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("HOST", true))
	    {
		ni->hosts.push_back(data.ExtractWord(1, " "));
	    }
	    else if (field.IsSameAs("EMAIL", true))
	    {
		ni->email = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("URL", true))
	    {
		ni->url = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("GSM", true))
	    {
		ni->gsm = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("PHONE", true))
	    {
		ni->phone = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("UIN", true))
	    {
		ni->UIN = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("LASTUH", true))
	    {
		ni->lastu = data.ExtractWord(1, " ");
		ni->lasth = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("LASTQMSG", true))
	    {
		ni->lastqmsg = data.After(":", 1);
	    }
	    else if (field.IsSameAs("LINK", true))
	    {
		ni->master = data.ExtractWord(1, " ");
	    }
	}
	else
	{
	    if (ni)
	    {
		Nick_Stored_t *nick = HYB_CreateNickEntry(ni);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);

		// Previous entry ...
		delete ni;
		ni = NULL;
	    }

	    if (line.WordCount(" ") < 4)
		continue;

	    ni = new HYB_NickInfo;
	    ni->nick = line.ExtractWord(1, " ");
	    ni->flags = atoi(line.ExtractWord(2, " ").c_str());
	    ni->created = atoi(line.ExtractWord(3, " ").c_str());
	    ni->lastseen = atoi(line.ExtractWord(4, " ").c_str());
	}
    }

    ETCB();
}

void HYB_load_cs_dbase(void)
{
    BTCB();
    HYB_ChanInfo *ci = NULL;

    mFile dbfile(HYB_ChanDBName);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (HYB_ChanDBName));

    while (!dbfile.Eof())
    {
	mstring line = dbfile.ReadLine();
	if (line.empty() || line[0u] == ';')
	    continue;

	else if (line[0u] == '-' && line[1u] == '>')
	{
	    if (!ci)
		continue;

	    mstring field = line.After(">").Before(" ");
	    mstring data = line.After(" ");

	    if (field.IsSameAs("PASS", true))
	    {
		ci->password = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("FNDR", true))
	    {
		ci->founder = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("SUCCESSOR", true))
	    {
		ci->successor = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("ACCESS", true))
	    {
		HYB_ChanAccess ca;
		mstring ent = data.ExtractWord(1, " ");
		ca.entry = ent;
		ca.level = atoi(data.ExtractWord(2, " ").c_str());
		ca.created = atoi(data.ExtractWord(3, " ").c_str());
		ca.last_used = atoi(data.ExtractWord(4, " ").c_str());
		ci->access.push_back(ca);
	    }
	    else if (field.IsSameAs("AKICK", true))
	    {
		HYB_AutoKick ca;
		ca.hostmask = data.ExtractWord(2, " ");
		ca.reason = data.After(" :", 1);
		ci->akick.push_back(ca);
	    }
	    else if (field.IsSameAs("ALVL", true))
	    {
		vector<mstring> slvl = data.Vector(" ");
		for (unsigned int i=0; i<slvl.size(); i++)
		    ci->access_lvl.push_back(atoi(slvl[i].c_str()));
	    }
	    else if (field.IsSameAs("TOPIC", true))
	    {
		ci->topic = data.After(" :", 1);
	    }
	    else if (field.IsSameAs("LIMIT", true))
	    {
		ci->limit = atoi(data.ExtractWord(1, " ").c_str());
	    }
	    else if (field.IsSameAs("KEY", true))
	    {
		ci->key = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("MON", true))
	    {
		ci->modes_on = atoi(data.ExtractWord(1, " ").c_str());
	    }
	    else if (field.IsSameAs("MOFF", true))
	    {
		ci->modes_off = atoi(data.ExtractWord(1, " ").c_str());
	    }
	    else if (field.IsSameAs("ENTRYMSG", true))
	    {
		ci->entrymsg = data.After(" :", 1);
	    }
	    else if (field.IsSameAs("EMAIL", true))
	    {
		ci->email = data.ExtractWord(1, " ");
	    }
	    else if (field.IsSameAs("URL", true))
	    {
		ci->url = data.ExtractWord(1, " ");
	    }
	}
	else
	{
	    if (ci)
	    {
		Chan_Stored_t *chan = HYB_CreateChanEntry(ci);
		if (chan != NULL)
		    Magick::instance().chanserv.AddStored(chan);

		// Previous entry ...
		delete ci;
		ci = NULL;
	    }

	    if (line.WordCount(" ") < 4)
		continue;

	    ci = new HYB_ChanInfo;
	    ci->name = line.ExtractWord(1, " ");
	    ci->flags = atoi(line.ExtractWord(2, " ").c_str());
	    ci->created = atoi(line.ExtractWord(3, " ").c_str());
	    ci->lastused = atoi(line.ExtractWord(4, " ").c_str());
	}
    }

    ETCB();
}

void HYB_load_ms_dbase(void)
{
    BTCB();
    HYB_MemoInfo *mi = NULL;

    mFile dbfile(HYB_MemoDBName);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (HYB_MemoDBName));

    while (!dbfile.Eof())
    {
	mstring line = dbfile.ReadLine();
	if (line.empty() || line[0u] == ';')
	    continue;

	else if (line[0u] == '-' && line[1u] == '>')
	{
	    if (!mi)
		continue;

	    mstring field = line.After(">").Before(" ");
	    mstring data = line.After(" ");

	    if (field.IsSameAs("TEXT", true))
	    {
		HYB_Memo m;
		m.sender = data.ExtractWord(1, " ");
		m.sent = atoi(data.ExtractWord(2, " ").c_str());
		m.flags = atoi(data.ExtractWord(2, " ").c_str());
		m.text = data.After(" :", 1);
	    }
	}
	else
	{
	    if (mi)
	    {
		MemoServ::nick_memo_t memo = HYB_CreateMemoEntry(mi);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);

		// Previous entry ...
		delete mi;
		mi = NULL;
	    }

	    mi = new HYB_MemoInfo;
	    mi->name = line.ExtractWord(1, " ");
	}
    }

    ETCB();
}

void HYB_load_ignore_dbase(void)
{
    BTCB();

    mFile dbfile(HYB_IgnoreDBName);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (HYB_IgnoreDBName));

    while (!dbfile.Eof())
    {
	mstring line = dbfile.ReadLine();
	if (line.empty() || line[0u] == ';')
	    continue;

	if (line.WordCount(" ") < 2)
	    continue;

	Magick::instance().operserv.Ignore_insert(line.ExtractWord(1, " "),
		line.ExtractWord(2, " ").IsSameAs("0"),
		Magick::instance().operserv.FirstName());
    }

    ETCB();
}

/*************************************************************************/

Nick_Stored_t *HYB_CreateNickEntry(HYB_NickInfo * ni)
{
    BTCB();
    if (ni == NULL || ni->nick.empty())
	return NULL;

    if (ni->flags & HYB_NS_FORBID)
    {
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick);

	return out;
    }
    else if (!ni->master.empty())
    {
	Nick_Stored_t tmp(ni->master);
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick, mDateTime(ni->created), tmp);

	if (out == NULL)
	    return NULL;
	if (!ni->lastu.empty() && !ni->lasth.empty())
	    out->i_LastMask = ni->lastu + "@" + ni->lasth;
	if (!ni->lastqmsg.empty())
	    out->i_LastQuit = ni->lastqmsg;
	out->i_LastSeenTime = mDateTime(ni->lastseen);
	return out;
    }
    else
    {
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick, ni->password);

	if (out == NULL)
	    return NULL;

	out->i_LastRealName = ni->nick;
	if (!ni->lastu.empty() && !ni->lasth.empty())
	    out->i_LastMask = ni->lastu + "@" + ni->lasth;
	if (!ni->lastqmsg.empty())
	    out->i_LastQuit = ni->lastqmsg;
	out->i_RegTime = mDateTime(ni->created);
	out->i_LastSeenTime = mDateTime(ni->lastseen);

	if (!ni->email.empty())
	    out->i_Email = ni->email;
	if (!ni->url.empty())
	    out->i_URL = ni->url;
	if (out->i_URL.Contains("http://"))
	    out->i_URL.Remove("http://", false);
	if (out->i_URL.Contains("HTTP://"))
	    out->i_URL.Remove("HTTP://", false);
	if (!ni->UIN.empty())
	    out->i_ICQ = ni->UIN;

	unsigned int i;
	for (i=0; i<ni->hosts.size(); i++)
	    out->i_access.insert(ni->hosts[i]);

	if (ni->flags & HYB_NS_PROTECTED && !out->L_Protect())
	    out->setting.Protect = true;
	if (ni->flags & HYB_NS_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ni->flags & HYB_NS_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ni->flags & HYB_NS_PRIVMSG && !out->L_PRIVMSG())
	    out->setting.PRIVMSG = true;
	if (ni->flags & HYB_NS_NOEXPIRE && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if (!(ni->flags & HYB_NS_MEMOS) && !out->L_NoMemo())
	    out->setting.NoMemo = true;

	return out;
    }
    ETCB();
}


mstring HYB_getmodes(int modes)
{
    BTCB();
    mstring retval;

    if (modes & HYB_CMODE_L)
	retval += "l";
    if (modes & HYB_CMODE_K)
	retval += "k";
    if (modes & HYB_CMODE_S)
	retval += "s";
    if (modes & HYB_CMODE_P)
	retval += "p";
    if (modes & HYB_CMODE_N)
	retval += "n";
    if (modes & HYB_CMODE_T)
	retval += "t";
    if (modes & HYB_CMODE_M)
	retval += "m";
    if (modes & HYB_CMODE_I)
	retval += "i";
    if (modes & HYB_CMODE_C)
	retval += "c";
    if (modes & HYB_CMODE_F)
	retval += "f";
    if (modes & HYB_CMODE_A)
	retval += "a";

    return retval;
    ETCB();
}

Chan_Stored_t *HYB_CreateChanEntry(HYB_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name.empty())
	return NULL;

    if (ci->flags & HYB_CS_FORBID)
    {
	Chan_Stored_t *out = new Chan_Stored_t(ci->name);

	return out;
    }
    else
    {
	vector<HYB_ChanAccess>::iterator access;
	vector<HYB_AutoKick>::iterator akick;
	unsigned int i;

	if (ci->founder.empty() || ci->password.empty())
	{
	    return NULL;
	}

	Chan_Stored_t *out =
	    new Chan_Stored_t(ci->name, ci->founder, ci->password, ci->name);
	if (out == NULL)
	    return NULL;

	if (!ci->successor.empty())
	    out->i_CoFounder = ci->successor;
	if (!ci->email.empty())
	    out->i_Email = ci->email;
	if (!ci->url.empty())
	    out->i_URL = ci->url;
	if (out->i_URL.Contains("http://"))
	    out->i_URL.Remove("http://", false);
	if (out->i_URL.Contains("HTTP://"))
	    out->i_URL.Remove("HTTP://", false);
	out->i_RegTime = mDateTime(ci->created);
	out->i_LastUsed = mDateTime(ci->lastused);

	long founder_level = ci->access_lvl[ci->access_lvl.size() - 1];
	long newlevel;
	float mod = (float) Magick::instance().chanserv.Level_Max() / (float) founder_level;

	for (access=ci->access.begin(); access!=ci->access.end(); access++)
	{
	    if (access->entry.empty())
		continue;
	    if (access->level < 0)
		newlevel = -1;
	    else
		newlevel = (long) ((float) access->level * mod);
	    if (newlevel == 0)
		newlevel = 1;
	    out->Access_insert(access->entry, newlevel, Magick::instance().chanserv.FirstName(),
		mDateTime(access->created));
	}

	for (akick=ci->akick.begin(); akick!=ci->akick.end(); akick++)
	{
	    out->Akick_insert(akick->hostmask, akick->reason, Magick::instance().chanserv.FirstName());
	}

	if (!ci->topic.empty())
	    out->i_Topic = ci->topic;

	if (!ci->entrymsg.empty())
	    out->Message_insert(ci->entrymsg, Magick::instance().chanserv.FirstName());

	if (ci->flags & HYB_CS_SECUREOPS && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & HYB_CS_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & HYB_CS_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & HYB_CS_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & HYB_CS_SPLITOPS && !out->L_Anarchy())
	    out->setting.Anarchy = true;
	if (ci->flags & HYB_CS_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & HYB_CS_GUARD && !out->L_Secure())
	    out->setting.Join = true;
	if (ci->flags & HYB_CS_NOEXPIRE && !Magick::instance().chanserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if (ci->flags & HYB_CS_EXPIREBANS && !out->L_Bantime())
	    out->setting.Bantime = HYB_BAN_EXPIRE_TIME;

	if (ci->flags & HYB_CS_SUSPENDED)
	{
	    out->i_Suspend_By = Magick::instance().chanserv.FirstName();
	    out->i_Suspend_Time = mDateTime::CurrentDateTime();
	}

	mstring modelock;

	if (ci->modes_on || !ci->key.empty() || ci->limit)
	{
	    mstring modes = HYB_getmodes(ci->modes_on);

	    modes.Remove("k");
	    modes.Remove("l");
	    modelock << "+" << modes << (!ci->key.empty() ? "k" : "") << (ci->limit ? "l" : "");
	}
	if (ci->modes_off)
	{
	    modelock << "-" << HYB_getmodes(ci->modes_off);
	}
	if (!ci->key.empty())
	{
	    modelock << " " << ci->key;
	}
	if (ci->limit)
	{
	    modelock << " " << ci->limit;
	}
	if (modelock.length())
	    out->Mlock(Magick::instance().chanserv.FirstName(), modelock);

	for (i=0; i<ci->access_lvl.size(); i++)
	{
	    if (ci->access_lvl[i] == founder_level)
		newlevel = Magick::instance().chanserv.Level_Max() + 1;
	    else if (ci->access_lvl[i] < 0)
		newlevel = -1;
	    else
		newlevel = (long) ((float) ci->access_lvl[i] * mod);

	    if (ci->access_lvl.size() == HYB7_CA_SIZE)
	    {
		switch (i)
		{
		case HYB7_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_CMDVOICE:
		    out->Level_change("CMDVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_ACCESS:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_CMDINVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_AUTOHALFOP:
		    out->Level_change("AUTOHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_CMDHALFOP:
		    out->Level_change("CMDHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_CMDOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_CMDUNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_CMDCLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB7_CA_SUPEROP:
		    out->Level_change("SUPER", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	    else
	    {
		switch (i)
		{
		case HYB6_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_CMDVOICE:
		    out->Level_change("CMDVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_ACCESS:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_CMDINVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_CMDOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_CMDUNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_CMDCLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case HYB6_CA_SUPEROP:
		    out->Level_change("SUPER", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}

	return out;
    }
    ETCB();
}

list < Memo_t > HYB_CreateMemoEntry(HYB_MemoInfo * mi)
{
    BTCB();
    unsigned int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;

    for (i = 0; i < mi->memos.size(); i++)
    {
	if (mi->memos[i].sender.empty() || mi->memos[i].text.empty())
	    continue;

	tmp = new Memo_t(mi->name, mi->memos[i].sender, mi->memos[i].text);
	tmp->i_Time = mDateTime(mi->memos[i].sent);
	if (mi->memos[i].flags & HYB_MS_READ)
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}

#endif /* CONVERT */
