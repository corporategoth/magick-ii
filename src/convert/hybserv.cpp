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

/* Based upon the DB loading routines from HybServ 1.9.0
 * (c) 2001-2003 Dinko Korunic <kreator@srce.hr>
 */

#include "magick.h"
#include "convert/interface.h"
#include "convert/hybserv.h"

/* NickServ flags */
#define hybserv_NS_IDENTIFIED   0x00000001 /* nick has IDENTIFY'd */
#define hybserv_NS_PROTECTED    0x00000002 /* kill to regain nick */
#define hybserv_NS_NOEXPIRE     0x00000004 /* nickname which doesn't expire */
#define hybserv_NS_AUTOMASK     0x00000008 /* auto-add new hostmasks */
#define hybserv_NS_PRIVATE      0x00000010 /* nick won't show up in a LIST */
#define hybserv_NS_COLLIDE      0x00000020 /* stole a nick - kill them */
#define hybserv_NS_RELEASE      0x00000040 /* to release an enforced nick */
#define hybserv_NS_FORBID       0x00000080 /* nick is forbidden */
#define hybserv_NS_SECURE       0x00000100 /* nick is secure */
#define hybserv_NS_DELETE       0x00000200 /* delete after a RELOAD */
#define hybserv_NS_UNSECURE     0x00000400 /* don't need to IDENTIFY */
#define hybserv_NS_MEMOSIGNON   0x00000800 /* tell about memos on signon */
#define hybserv_NS_MEMONOTIFY   0x00001000 /* tell about new memos */
#define hybserv_NS_MEMOS        0x00002000 /* allow others to send memos */
#define hybserv_NS_HIDEALL      0x00004000 /* hide everything from INFO */
#define hybserv_NS_HIDEEMAIL    0x00008000 /* hide email from INFO */
#define hybserv_NS_HIDEURL      0x00010000 /* hide url from INFO */
#define hybserv_NS_HIDEQUIT     0x00020000 /* hide quitmsg from INFO */
#define hybserv_NS_HIDEADDR     0x00040000 /* hide last addy from INFO */
#define hybserv_NS_KILLIMMED    0x00080000 /* kill immediately to regain nick */
#define hybserv_NS_NOREGISTER   0x00100000 /* cannot register channels */
#define hybserv_NS_NOCHANOPS    0x00200000 /* not allowed to be opped */
#define hybserv_NS_NUMERIC      0x00400000 /* ignores 432 numeric */
#define hybserv_NS_PRIVMSG      0x00800000 /* PRIVMSG or NOTICE */

/* structure for registered nicknames */
struct hybserv_NickInfo
{
  mstring nick;                /* registered nickname */
  mstring password;            /* password */
  vector<mstring> hosts;    /* recognized hosts for this nick */
  time_t created;            /* timestamp when it was registered */
  time_t lastseen;           /* for expiration purposes */
  long flags;                /* nick flags */

  mstring email;               /* email address */
  mstring url;                 /* url */

  mstring gsm;                 /* GSM number */
  mstring phone;               /* Phone */
  mstring UIN;                 /* ICQ UIN */

  mstring lastu;               /* last seen username */
  mstring lasth;               /* last seen hostname */
  mstring lastqmsg;            /* last quit message */

  /*
   * If this is the "hub" nickname for a list of linked nicknames,
   * master will be NULL. If this nickname is a leaf, master will
   * point to the "hub" nickname of this list
   */
  mstring master;
};

/*************************************************************************/

#define hybserv_BAN_EXPIRE_TIME 10800

/* ChanServ flags */
#define hybserv_CS_PRIVATE      0x00000001 /* channel won't show up in LIST */
#define hybserv_CS_TOPICLOCK    0x00000002 /* must be changed via SET TOPIC */
#define hybserv_CS_SECURE       0x00000004 /* channel is secure */
#define hybserv_CS_SECUREOPS    0x00000008 /* only aop/sop/founders can be opped */
#define hybserv_CS_SUSPENDED    0x00000010 /* channel is suspended */
#define hybserv_CS_FORBID       0x00000020 /* channel is forbidden */
#define hybserv_CS_RESTRICTED   0x00000040 /* channel is restricted */
#define hybserv_CS_FORGET       0x00000080 /* channel is forgotten */
#define hybserv_CS_DELETE       0x00000100 /* delete after a RELOAD */
#define hybserv_CS_NOEXPIRE     0x00000200 /* never expires */
#define hybserv_CS_GUARD        0x00000400 /* have ChanServ join the channel */
#define hybserv_CS_SPLITOPS     0x00000800 /* let people keep ops from splits */
#define hybserv_CS_VERBOSE      0x00001000 /* notify chanops for access changes */
#define hybserv_CS_EXPIREBANS   0x00002000 /* expire bans after EXPIRETIME */

/* access_lvl[] indices */
/* We will happily FUBAR old databases by changing this. However, it had
 * to be done -kre && Janos
 * PS, I have added upgrade-chan target in Makefile for fixing this
 * properly - it relies on awk and DefaultAccess as well as ALVL in
 * chan.db -kre */
# define hybrid7_CA_AUTODEOP     0
# define hybrid7_CA_AUTOVOICE    1
# define hybrid7_CA_CMDVOICE     2
# define hybrid7_CA_ACCESS       3
# define hybrid7_CA_CMDINVITE    4
# define hybrid7_CA_AUTOHALFOP   5
# define hybrid7_CA_CMDHALFOP    6
# define hybrid7_CA_AUTOOP       7
# define hybrid7_CA_CMDOP        8
# define hybrid7_CA_CMDUNBAN     9
# define hybrid7_CA_AKICK        10
# define hybrid7_CA_CMDCLEAR     11
# define hybrid7_CA_SET          12
# define hybrid7_CA_SUPEROP      13
# define hybrid7_CA_FOUNDER      14
# define hybrid7_CA_SIZE         15 /* number of indices */
# define hybrid6_CA_AUTODEOP     0
# define hybrid6_CA_AUTOVOICE    1
# define hybrid6_CA_CMDVOICE     2
# define hybrid6_CA_ACCESS       3
# define hybrid6_CA_CMDINVITE    4
# define hybrid6_CA_AUTOOP       5
# define hybrid6_CA_CMDOP        6
# define hybrid6_CA_CMDUNBAN     7
# define hybrid6_CA_AKICK        8
# define hybrid6_CA_CMDCLEAR     9
# define hybrid6_CA_SET          10
# define hybrid6_CA_SUPEROP      11
# define hybrid6_CA_FOUNDER      12
# define hybrid6_CA_SIZE         13 /* number of indices */

#define hybserv_CMODE_L          0x000010 /* channel is +l */
#define hybserv_CMODE_K          0x000020 /* channel is +k */
#define hybserv_CMODE_S          0x000040 /* channel is +s */
#define hybserv_CMODE_P          0x000080 /* channel is +p */
#define hybserv_CMODE_N          0x000100 /* channel is +n */
#define hybserv_CMODE_T          0x000200 /* channel is +t */
#define hybserv_CMODE_M          0x000400 /* channel is +m */
#define hybserv_CMODE_I          0x000800 /* channel is +i */
#define hybserv_CMODE_C          0x001000 /* channel is +c */
#define hybserv_CMODE_F          0x002000 /* channel is +f */
#define hybserv_CMODE_A          0x004000 /* channel is +a - Janos */


struct hybserv_ChanAccess
{
  int level;               /* privs mask has */
  mstring entry;   /* nickname */

  /*
   * pointer to corresponding AccessChannel structure on nptr's
   * AccessChannels list - this way, when we delete a ChanAccess
   * structure, we don't have to loop through all of nptr's
   * access channels to find the corresponding pointer.
   */
  time_t created; /* time when this entry was added */
  time_t last_used; /* last time the person joined the channel while
                       identified */
};

struct hybserv_AutoKick
{
  mstring hostmask; /* mask to autokick */
  mstring reason;   /* reason for autokick */
};

struct hybserv_ChanInfo
{
  mstring name;                   /* channel name */
  mstring founder;                /* founder nick (must be registered) */
  time_t last_founder_active;   /* last time the founder joined/left */
  mstring successor;              /* successor nick (must be registered) */
  mstring password;               /* founder password */
  mstring topic;                  /* NULL if no topic lock */
  long limit;                   /* 0 if no limit */
  mstring key;                    /* NULL if no key */
  int modes_on,                 /* modes to enforce */
      modes_off;                /* modes to enforce off */
  vector<hybserv_ChanAccess> access;    /* access list */
  vector<hybserv_AutoKick> akick;       /* autokick list */

  mstring entrymsg;               /* msg to send to users upon entry to channel */
  mstring email;                  /* email address of channel */
  mstring url;                    /* url of channel */

  time_t created;               /* timestamp when it was registered */
  time_t lastused;              /* for expiration purposes */
  vector<int> access_lvl;              /* customized access levels for this channel */
  long flags;                   /* channel flags */
};


/*************************************************************************/

/* MemoServ flags */
#define hybserv_MS_READ         0x00000001 /* memo has been read */
#define hybserv_MS_DELETE       0x00000002 /* marked for deletion */
#define hybserv_MS_RDELETE      0x00000004 /* delete after a RELOAD */
#define hybserv_MS_REPLIED      0x00000008 /* has been replied */

struct hybserv_Memo
{
  mstring sender;
  time_t sent;    /* time it was sent */
  mstring text;
  long flags;
};

struct hybserv_MemoInfo
{
  mstring name;            /* who the memo was sent to */
  vector<hybserv_Memo> memos;    /* the actual memos */
};


/*************************************************************************/

const char *hybserv_NickDBName = "nick.db";
const char *hybserv_ChanDBName = "chan.db";
const char *hybserv_MemoDBName = "memo.db";
const char *hybserv_IgnoreDBName = "ignore.db";

/*************************************************************************/


void hybserv_load_nick()
{
    BTCB();
    hybserv_NickInfo *ni = NULL;

    mFile dbfile(hybserv_NickDBName);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (hybserv_NickDBName));

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
		Nick_Stored_t *nick = Convert::hybserv_CreateNickEntry(ni);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);

		// Previous entry ...
		delete ni;
		ni = NULL;
	    }

	    if (line.WordCount(" ") < 4)
		continue;

	    ni = new hybserv_NickInfo;
	    ni->nick = line.ExtractWord(1, " ");
	    ni->flags = atoi(line.ExtractWord(2, " ").c_str());
	    ni->created = atoi(line.ExtractWord(3, " ").c_str());
	    ni->lastseen = atoi(line.ExtractWord(4, " ").c_str());
	}
    }

    if (ni)
    {
	Nick_Stored_t *nick = Convert::hybserv_CreateNickEntry(ni);
	if (nick != NULL)
	    Magick::instance().nickserv.AddStored(nick);

	// Previous entry ...
	delete ni;
	ni = NULL;
    }

    ETCB();
}

void hybserv_load_chan(void)
{
    BTCB();
    hybserv_ChanInfo *ci = NULL;

    mFile dbfile(hybserv_ChanDBName);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (hybserv_ChanDBName));

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
		hybserv_ChanAccess ca;
		mstring ent = data.ExtractWord(1, " ");
		ca.entry = ent;
		ca.level = atoi(data.ExtractWord(2, " ").c_str());
		ca.created = atoi(data.ExtractWord(3, " ").c_str());
		ca.last_used = atoi(data.ExtractWord(4, " ").c_str());
		ci->access.push_back(ca);
	    }
	    else if (field.IsSameAs("AKICK", true))
	    {
		hybserv_AutoKick ca;
		ca.hostmask = data.ExtractWord(2, " ");
		ca.reason = data.After(":", 1);
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
		ci->topic = data.After(":", 1);
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
		ci->entrymsg = data.After(":", 1);
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
		Chan_Stored_t *chan = Convert::hybserv_CreateChanEntry(ci);
		if (chan != NULL)
		    Magick::instance().chanserv.AddStored(chan);

		// Previous entry ...
		delete ci;
		ci = NULL;
	    }

	    if (line.WordCount(" ") < 4)
		continue;

	    ci = new hybserv_ChanInfo;
	    ci->last_founder_active = 0;
	    ci->limit = 0;
	    ci->modes_on = ci->modes_off = 0;
	    ci->name = line.ExtractWord(1, " ");
	    ci->flags = atoi(line.ExtractWord(2, " ").c_str());
	    ci->created = atoi(line.ExtractWord(3, " ").c_str());
	    ci->lastused = atoi(line.ExtractWord(4, " ").c_str());
	}
    }

    ETCB();
}

void hybserv_load_memo(void)
{
    BTCB();
    hybserv_MemoInfo *mi = NULL;

    mFile dbfile(hybserv_MemoDBName);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (hybserv_MemoDBName));

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
		hybserv_Memo m;
		m.sender = data.ExtractWord(1, " ");
		m.sent = atoi(data.ExtractWord(2, " ").c_str());
		m.flags = atoi(data.ExtractWord(2, " ").c_str());
		m.text = data.After(":", 1);
	    }
	}
	else
	{
	    if (mi)
	    {
		MemoServ::nick_memo_t memo = Convert::hybserv_CreateMemoEntry(mi);
		if (memo.size())
		    Magick::instance().memoserv.AddNick(memo);

		// Previous entry ...
		delete mi;
		mi = NULL;
	    }

	    mi = new hybserv_MemoInfo;
	    mi->name = line.ExtractWord(1, " ");
	}
    }

    ETCB();
}

void hybserv_load_ignore(void)
{
    BTCB();

    mFile dbfile(hybserv_IgnoreDBName);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (hybserv_IgnoreDBName));

    while (!dbfile.Eof())
    {
	mstring line = dbfile.ReadLine();
	if (line.empty() || line[0u] == ';')
	    continue;

	if (line.WordCount(" ") < 2)
	    continue;

	Magick::instance().operserv.Ignore_insert(line.ExtractWord(1, " "),
		true, Magick::instance().operserv.FirstName());
    }

    ETCB();
}

/*************************************************************************/

Nick_Stored_t *Convert::hybserv_CreateNickEntry(hybserv_NickInfo * ni)
{
    BTCB();
    if (ni == NULL || ni->nick.empty())
	return NULL;

    if (ni->flags & hybserv_NS_FORBID)
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

	if (ni->flags & hybserv_NS_PROTECTED && !out->L_Protect())
	    out->setting.Protect = true;
	if (ni->flags & hybserv_NS_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ni->flags & hybserv_NS_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ni->flags & hybserv_NS_PRIVMSG && !out->L_PRIVMSG())
	    out->setting.PRIVMSG = true;
	if (ni->flags & hybserv_NS_NOEXPIRE && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if (!(ni->flags & hybserv_NS_MEMOS) && !out->L_NoMemo())
	    out->setting.NoMemo = true;

	return out;
    }
    ETCB();
}


mstring hybserv_getmodes(int modes)
{
    BTCB();
    mstring retval;

    if (modes & hybserv_CMODE_L)
	retval += "l";
    if (modes & hybserv_CMODE_K)
	retval += "k";
    if (modes & hybserv_CMODE_S)
	retval += "s";
    if (modes & hybserv_CMODE_P)
	retval += "p";
    if (modes & hybserv_CMODE_N)
	retval += "n";
    if (modes & hybserv_CMODE_T)
	retval += "t";
    if (modes & hybserv_CMODE_M)
	retval += "m";
    if (modes & hybserv_CMODE_I)
	retval += "i";
    if (modes & hybserv_CMODE_C)
	retval += "c";
    if (modes & hybserv_CMODE_F)
	retval += "f";
    if (modes & hybserv_CMODE_A)
	retval += "a";

    return retval;
    ETCB();
}

Chan_Stored_t *Convert::hybserv_CreateChanEntry(hybserv_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name.empty())
	return NULL;

    if (ci->flags & hybserv_CS_FORBID)
    {
	Chan_Stored_t *out = new Chan_Stored_t(ci->name);

	return out;
    }
    else
    {
	vector<hybserv_ChanAccess>::iterator access;
	vector<hybserv_AutoKick>::iterator akick;
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
		access->created ? mDateTime::CurrentDateTime() : mDateTime(access->created));
	}

	for (akick=ci->akick.begin(); akick!=ci->akick.end(); akick++)
	{
	    out->Akick_insert(akick->hostmask, akick->reason, Magick::instance().chanserv.FirstName());
	}

	if (!ci->topic.empty())
	{
	    out->i_Topic = ci->topic;
	    out->i_Topic_Setter = Magick::instance().chanserv.FirstName();
	    out->i_Topic_Set_Time = mDateTime::CurrentDateTime();
	}

	if (!ci->entrymsg.empty())
	    out->Message_insert(ci->entrymsg, Magick::instance().chanserv.FirstName());

	if (ci->flags & hybserv_CS_SECUREOPS && !out->L_Secureops())
	    out->setting.Secureops = true;
	if (ci->flags & hybserv_CS_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ci->flags & hybserv_CS_TOPICLOCK && !out->L_Topiclock())
	    out->setting.Topiclock = true;
	if (ci->flags & hybserv_CS_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & hybserv_CS_SPLITOPS && !out->L_Anarchy())
	    out->setting.Anarchy = true;
	if (ci->flags & hybserv_CS_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & hybserv_CS_GUARD && !out->L_Secure())
	    out->setting.Join = true;
	if (ci->flags & hybserv_CS_NOEXPIRE && !Magick::instance().chanserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if (ci->flags & hybserv_CS_EXPIREBANS && !out->L_Bantime())
	    out->setting.Bantime = hybserv_BAN_EXPIRE_TIME;

	if (ci->flags & hybserv_CS_SUSPENDED)
	{
	    out->i_Suspend_By = Magick::instance().chanserv.FirstName();
	    out->i_Suspend_Time = mDateTime::CurrentDateTime();
	}

	mstring modelock;

	if (ci->modes_on || !ci->key.empty() || ci->limit)
	{
	    mstring modes = hybserv_getmodes(ci->modes_on);

	    modes.Remove("k");
	    modes.Remove("l");
	    modelock << "+" << modes << (!ci->key.empty() ? "k" : "") << (ci->limit ? "l" : "");
	}
	if (ci->modes_off)
	{
	    modelock << "-" << hybserv_getmodes(ci->modes_off);
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

	    if (ci->access_lvl.size() == hybrid7_CA_SIZE)
	    {
		switch (i)
		{
		case hybrid7_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_CMDVOICE:
		    out->Level_change("CMDVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_ACCESS:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_CMDINVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_AUTOHALFOP:
		    out->Level_change("AUTOHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_CMDHALFOP:
		    out->Level_change("CMDHALFOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_CMDOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_CMDUNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_CMDCLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid7_CA_SUPEROP:
		    out->Level_change("SUPER", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	    else
	    {
		switch (i)
		{
		case hybrid6_CA_AUTODEOP:
		    out->Level_change("AUTODEOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_AUTOVOICE:
		    out->Level_change("AUTOVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_CMDVOICE:
		    out->Level_change("CMDVOICE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_ACCESS:
		    out->Level_change("ACCESS", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_CMDINVITE:
		    out->Level_change("CMDINVITE", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_AUTOOP:
		    out->Level_change("AUTOOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_CMDOP:
		    out->Level_change("CMDOP", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_CMDUNBAN:
		    out->Level_change("UNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    out->Level_change("CMDUNBAN", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_AKICK:
		    out->Level_change("AKICK", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_CMDCLEAR:
		    out->Level_change("CMDCLEAR", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_SET:
		    out->Level_change("SET", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		case hybrid6_CA_SUPEROP:
		    out->Level_change("SUPER", newlevel, Magick::instance().chanserv.FirstName());
		    break;
		}
	    }
	}

	return out;
    }
    ETCB();
}

MemoServ::nick_memo_t Convert::hybserv_CreateMemoEntry(hybserv_MemoInfo * mi)
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
	if (mi->memos[i].flags & hybserv_MS_READ)
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}

#endif /* CONVERT */
