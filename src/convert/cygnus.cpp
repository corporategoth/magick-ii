#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
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
#define RCSID(x,y) const char *rcsid_convert_cygnus_cpp_ ## x () { return y; }
RCSID(convert_cygnus_cpp, "@(#)$Id$");

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

/* Based upon the DB loading routines from Cygnus 0.1.1
 * (c) 2001-2002 Darcy Grexton <skold@habber.net>
 */

#include "magick.h"
#include "convert/interface.h"
#include "convert/cygnus.h"

#define cygnus_NICKSERV_DB "nickserv.db"
#define cygnus_CHANSERV_DB "chanserv.db"
#define cygnus_ROOTSERV_DB "rootserv.db"

typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

/* Channel Access List:
      0: Not in use
      1: VOP
      2: HOP (Unreal only)
      3: AOP
      4: SOP
      5: Founder
 */
struct cygnus_ChanAccess {
    int32 level;		/* Level of this entry		*/
    mstring mask;		/* Mask of entry		*/
};

/* Channel AKick List */
struct cygnus_Akick {
    mstring mask;		/* Mask of entry		*/
    mstring reason;		/* Reason for the AKick		*/
    mstring setter;		/* Who set the AKick		*/
    time_t time;		/* When it was set		*/
    int32 level;		/* Level of this entry		*/
};

/* Verification structure */
struct cygnus_Verify {
    mstring name;		/* Object waiting to be verified*/
    time_t since;		/* How long it's been waiting	*/
};

/* Memo structure */
struct cygnus_Memo {
    int32 number;		/* Memo Number			*/
    int32 flags;		/* Memos flags			*/
    time_t time;		/* When it was sent		*/
    mstring sender;		/* Who sent it			*/
    mstring text;		/* Actual memo			*/
};

/* Memo Info */
struct cygnus_MemoInfo {
    int memolimit;		/* Max memos they can have	*/
    vector<cygnus_Memo> memos;	/* Memo data			*/
};


/* NickName Information structure */
struct cygnus_NickInfo {
    mstring nick;		/* Registered NickName		*/
    mstring pass;		/* Password for NickName	*/
    mstring host;		/* Nick this one is linked to   */
    mstring usermask;		/* Last user@host mask		*/
    mstring real;		/* Last RealName		*/
    vector<mstring> access;	/* NickName's Access List	*/
    mstring email;		/* E-Mail address		*/
    mstring url;		/* URL				*/
    mstring name;		/* Real Name			*/
    mstring sex;		/* Sex				*/
    mstring location;		/* Location			*/
    mstring forward;		/* NickName to forward memos to	*/
    mstring freezereason;	/* Optional reason for Freeze	*/
    mstring temp;		/* For use with temp emails	*/
    unsigned long key;		/* NickServ Key			*/
    int32 age;			/* Age				*/
    int32 uin;			/* ICQ UIN			*/
    int32 flags;		/* NickName Flags		*/
    int32 sstamp;		/* Services' Stamp		*/
    int32 zone;			/* TimeZone this user prefers	*/
    time_t timestamp;		/* TimeStamp			*/
    time_t registered;		/* Time of Registration		*/
    time_t lastseen;		/* Last logoff after ID		*/
    time_t temptime;		/* For use with temp emails	*/
    cygnus_MemoInfo memos;	/* NickName's Memos		*/
};

/* Channel Information structure */
struct cygnus_ChanInfo {
    mstring name;		/* Channel name			*/
    mstring founder;		/* Channel founder		*/
    mstring pass;		/* Channel password		*/
    mstring topic;		/* Channel topic		*/
    mstring topicsetter;	/* Who set the topic		*/
    mstring successor;		/* Channel successor		*/
    mstring greet;		/* Channel Greet message	*/
    mstring url;		/* Channel URL			*/
    mstring mlock_key;		/* MLock key			*/
    mstring mlock_flood;	/* MLock flood			*/
    mstring mlock_link;		/* MLock link			*/
    vector<cygnus_ChanAccess> access;	/* Access list			*/
    vector<cygnus_Akick> akick;	/* AutoKick List		*/
    int32 mlock_on;		/* Modes locked on		*/
    int32 mlock_off;		/* Modes locked off		*/
    int32 mlock_limit;		/* MLock limit			*/
    int32 topiclock;		/* TopicLock setting		*/    
    int32 memolevel;		/* MemoLevel setting		*/
    int32 flags;		/* Channel Flags		*/
    time_t topictime;		/* When the topic was set	*/
    time_t registered;		/* Time of Registration		*/
    time_t lastused;		/* Time of last usage		*/
    unsigned long key;		/* ChanServ Key			*/
};

/* NickName Flags */
#define cygnus_NF_ENFORCE	0x00000001	/* NickName Enforcement	*/
#define cygnus_NF_SECURE	0x00000002	/* NickName Security	*/
#define cygnus_NF_HIDEMAIL	0x00000004	/* Hide E-Mail		*/
#define cygnus_NF_SHOWMAIL	0x00000008	/* Show E-Mail		*/
#define cygnus_NF_NEVEROP	0x00000010	/* Never Op (ChanServ)	*/
#define cygnus_NF_NOOP		0x00000020	/* Can't add to CS Lists*/
#define cygnus_NF_NOSUCCESSOR	0x00000040	/* Can't be successor	*/
#define cygnus_NF_PRIVATE	0x00000080	/* Privacy Settings	*/
#define cygnus_NF_NOTICE	0x00000100	/* Use NOTICE		*/
#define cygnus_NF_PRIVMSG	0x00000200	/* Use PRIVMSG		*/
#define cygnus_NF_NOMEMO	0x00000400	/* No Memos		*/
#define cygnus_NF_RECEIPTS	0x00000800	/* Memo Receipts	*/
#define cygnus_NF_MEMOMAIL	0x00001000	/* MemoMail		*/
#define cygnus_NF_FROZEN	0x00002000	/* NickName is frozen	*/
#define cygnus_NF_HELD		0x00004000	/* NickName is held	*/
#define cygnus_NF_MARKED	0x00008000	/* NickName is marked	*/

/* Special NickName Flags */
#define cygnus_NF_LINKED	0x10000000	/* Linked NickName	*/
#define cygnus_NF_CSOP		0x20000000	/* Services Operator	*/
#define cygnus_NF_NEWS		0x40000000	/* Hasn't read the NEWS	*/
#define cygnus_NF_WAITAUTH	0x80000000	/* Waiting for Auth	*/

/* ChanServ Flags */
#define cygnus_CF_VERBOSE	0x00000001	/* Verbose reporting	*/
#define cygnus_CF_VOPALL	0x00000002	/* Auto +v on join	*/
#define cygnus_CF_SECURE	0x00000004	/* Channel Security	*/
#define cygnus_CF_RESTRICTED	0x00000008	/* Restricted Channel	*/
#define cygnus_CF_FROZEN	0x00000010	/* Channel is frozen	*/
#define cygnus_CF_HELD		0x00000020	/* Channel is held	*/
#define cygnus_CF_MARKED	0x00000040	/* Channel is marked	*/
#define cygnus_CF_LIMITED	0x00000080	/* Maintain +l		*/

/* Special Channel Flags */
#define cygnus_CF_WAITAUTH	0x10000000	/* Waiting for Auth	*/
#define cygnus_CF_WAITING	0x20000000	/* Waiting for Verify	*/

/* Memo Status Flags */
#define cygnus_MF_UNREAD	0x00000001	/* Memo is Unread	*/
#define cygnus_MF_EMAILED	0x00000002	/* Memo was E-Mailed	*/

/* Channel Modes */
#define cygnus_CMODE_t		0x00000001	/* Only ops change topic*/
#define cygnus_CMODE_n		0x00000002	/* No external messages	*/
#define cygnus_CMODE_s		0x00000004	/* Secret Channel	*/
#define cygnus_CMODE_m		0x00000008	/* Moderated		*/
#define cygnus_CMODE_l		0x00000010	/* Limited users	*/	
#define cygnus_CMODE_i		0x00000020	/* Invite only		*/
#define cygnus_CMODE_p		0x00000040	/* Private		*/
#define cygnus_CMODE_k		0x00000080	/* Keyed		*/
#define cygnus_CMODE_o		0x00000100	/* Op			*/
#define cygnus_CMODE_v		0x00000200	/* Voice		*/
#define cygnus_CMODE_R		0x00000400	/* Registered users only*/
#define cygnus_CMODE_r		0x00000800	/* Registered channel	*/
#define cygnus_CMODE_c		0x00001000	/* No colors allowed	*/
#define cygnus_CMODE_O		0x00002000	/* Oper Only channel	*/
#define cygnus_CMODE_Q		0x00004000	/* No non-U:Lined kicks	*/
#define cygnus_CMODE_S		0x00008000	/* Strip color codes	*/
#define cygnus_CMODE_K		0x00010000	/* No knocking		*/
#define cygnus_CMODE_V		0x00020000	/* No inviting		*/
#define cygnus_CMODE_f		0x00040000	/* Flood trigger	*/
#define cygnus_CMODE_H		0x00080000	/* No hidden users	*/
#define cygnus_CMODE_G		0x00100000	/* Strip bad words	*/
#define cygnus_CMODE_C		0x00200000	/* No CTCPs/Ctrl Codes	*/
#define cygnus_CMODE_u		0x00400000	/* Auditorium		*/
#define cygnus_CMODE_z		0x00800000	/* Secure users only	*/
#define cygnus_CMODE_N		0x01000000	/* No nick changes	*/
#define cygnus_CMODE_L		0x02000000	/* Linked Channel	*/
#define cygnus_CMODE_A		0x04000000	/* Admin Only channel	*/
#define cygnus_CMODE_h		0x08000000	/* Half-Op		*/
#define cygnus_CMODE_a		0x10000000	/* Protected		*/
#define cygnus_CMODE_q		0x20000000	/* Channel Founder	*/

/* Channel access list levels */
#define cygnus_VOP		1		/* Voice Op		*/
#define cygnus_HOP		2		/* Half Op		*/
#define cygnus_AOP		3		/* Auto Op		*/
#define cygnus_SOP		4		/* Super Op		*/
#define cygnus_FOUNDER		5		/* Channel founder	*/


void cygnus_load_nick()
{
    BTCB();
    cygnus_NickInfo *ni = NULL;

    mFile dbfile(cygnus_NICKSERV_DB);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (cygnus_NICKSERV_DB));

    int version = 0;
    while (!dbfile.Eof())
    {
	mstring line = dbfile.ReadLine();
	if (line.empty() || line[0u] == '#' || line[0u] == ' ' || line[0u] == '\t')
	    continue;

	mstring field = line.ExtractWord(1, " ");
	mstring data = line.After(" ");

	if (field.IsSameAs("NV"))
	{
	    version = atoi(data.ExtractWord(1, " ").c_str());
	}
	else if (field.IsSameAs("SS") || field.IsSameAs("DE") || (version < 3 && field.IsSameAs("NS")))
	{
	    // Ignore it ...
	}
	else if (field.IsSameAs("NI"))
	{
	    if (ni)
	    {
		Nick_Stored_t *nick = Convert::cygnus_CreateNickEntry(ni);
		if (nick != NULL)
		{
		    Magick::instance().nickserv.AddStored(nick);
		    MemoServ::nick_memo_t memo = Convert::cygnus_CreateMemoEntry(&ni->memos, ni->nick.c_str());
		    if (memo.size())
			Magick::instance().memoserv.AddNick(memo);
		}

		// Previous entry ...
		delete ni;
		ni = NULL;
	    }

	    if (line.WordCount(" ") < 11)
		continue;

	    ni = new cygnus_NickInfo;
	    ni->age = 0;
	    ni->uin = 0;
	    ni->temptime = 0;
	    ni->nick = data.ExtractWord(1, " ");
	    ni->pass = data.ExtractWord(2, " ");
	    ni->registered = atoi(data.ExtractWord(3, " "));
	    ni->lastseen = atoi(data.ExtractWord(4, " "));
	    ni->timestamp = atoi(data.ExtractWord(5, " "));
	    ni->sstamp = atoi(data.ExtractWord(6, " "));
	    ni->flags = atoi(data.ExtractWord(7, " "));
	    ni->memos.memolimit = atoi(data.ExtractWord(8, " "));
	    if (version > 2)
	    {
		ni->zone = atoi(data.ExtractWord(9, " "));
		ni->key = atoi(data.ExtractWord(10, " "));
		ni->usermask = data.ExtractWord(11, " ");
		ni->real = data.After(" ", 11);
	    }
	    else
	    {
		ni->zone = 0;
		ni->key = atoi(data.ExtractWord(9, " "));
		ni->usermask = data.ExtractWord(10, " ");
		ni->real = data.After(" ", 10);
	    }
	}
	else if (field.IsSameAs("FR"))
	{
	    if (!ni)
		continue;

	    ni->freezereason = data;
	}
	else if (field.IsSameAs("EM"))
	{
	    if (!ni)
		continue;

	    ni->email = data.ExtractWord(1, " ");
	}
	else if (field.IsSameAs("TE"))
	{
	    ni->temp = data.ExtractWord(1, " ");
	}
	else if (field.IsSameAs("TT"))
	{
	    ni->temptime = atoi(data.ExtractWord(1, " ").c_str());
	}
	else if (field.IsSameAs("UR"))
	{
	    if (!ni)
		continue;

	    ni->url = data.ExtractWord(1, " ");
	}
	else if (field.IsSameAs("UN"))
	{
	    if (!ni)
		continue;

	    ni->uin = atoi(data.ExtractWord(1, " ").c_str());
	}
	else if (field.IsSameAs("NA"))
	{
	    if (!ni)
		continue;

	    ni->name = data;
	}
	else if (field.IsSameAs("AG"))
	{
	    if (!ni)
		continue;

	    ni->age = atoi(data.ExtractWord(1, " ").c_str());
	}
	else if (field.IsSameAs("SX"))
	{
	    if (!ni)
		continue;

	    ni->sex = data.ExtractWord(1, " ");
	}
	else if (field.IsSameAs("LO"))
	{
	    if (!ni)
		continue;

	    ni->location = data;
	}
	else if (field.IsSameAs("AC"))
	{
	    if (!ni)
		continue;

	    ni->access.push_back(data.ExtractWord(1, " "));
	}
	else if (field.IsSameAs("LN"))
	{
	    if (!ni)
		continue;

	    for (unsigned int i=1; i<=data.WordCount(" "); i++)
	    {
		cygnus_NickInfo tmp = *ni;
		tmp.host = tmp.nick;
		tmp.nick = data.ExtractWord(i, " ");
		Nick_Stored_t *nick = Convert::cygnus_CreateNickEntry(&tmp);
		if (nick != NULL)
		    Magick::instance().nickserv.AddStored(nick);
	    }
	}
	else if (field.IsSameAs("MO"))
	{
	    if (!ni)
		continue;

	    cygnus_Memo m;
	    m.number = atoi(data.ExtractWord(1, " ").c_str());
	    m.flags = atoi(data.ExtractWord(2, " ").c_str());
	    m.time = atoi(data.ExtractWord(3, " ").c_str());
	    m.sender = data.ExtractWord(4, " ");
	    m.text = data.After(" ", 4);

	    ni->memos.memos.push_back(m);
	}
	else if (field.IsSameAs("FW"))
	{
	    ni->forward = data.ExtractWord(1, " ");
	}
    }

    if (ni)
    {
	Nick_Stored_t *nick = Convert::cygnus_CreateNickEntry(ni);
	if (nick != NULL)
	    Magick::instance().nickserv.AddStored(nick);

	// Previous entry ...
	delete ni;
	ni = NULL;
    }

    ETCB();
}

void cygnus_load_chan()
{
    BTCB();
    cygnus_ChanInfo *ci = NULL;

    mFile dbfile(cygnus_CHANSERV_DB);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (cygnus_CHANSERV_DB));

    int version = 0;
    while (!dbfile.Eof())
    {
	mstring line = dbfile.ReadLine();
	if (line.empty() || line[0u] == '#' || line[0u] == ' ' || line[0u] == '\t')
	    continue;

	mstring field = line.ExtractWord(1, " ");
	mstring data = line.After(" ");

	if (field.IsSameAs("CV"))
	{
	    version = atoi(data.ExtractWord(1, " ").c_str());
	}
	else if (field.IsSameAs("VF") || field.IsSameAs("DE") || (version < 3 && field.IsSameAs("CS")))
	{
	    // Ignore it ...
	}
	else if (field.IsSameAs("CI"))
	{
	    if (ci)
	    {
		Chan_Stored_t *chan = Convert::cygnus_CreateChanEntry(ci);
		if (chan != NULL)
		    Magick::instance().chanserv.AddStored(chan);

		// Previous entry ...
		delete ci;
		ci = NULL;
	    }

	    if (line.WordCount(" ") < 10)
		continue;

	    ci = new cygnus_ChanInfo;
	    ci->mlock_limit = 0;
	    ci->topictime = 0;
	    ci->name = data.ExtractWord(1, " ");
	    ci->founder = data.ExtractWord(2, " ");
	    ci->pass = data.ExtractWord(3, " ");
	    ci->registered = atoi(data.ExtractWord(4, " ").c_str());
	    ci->lastused = atoi(data.ExtractWord(5, " ").c_str());
	    ci->flags = atoi(data.ExtractWord(6, " ").c_str());
	    ci->mlock_on = atoi(data.ExtractWord(7, " ").c_str());
	    ci->mlock_off = atoi(data.ExtractWord(8, " ").c_str());
	    ci->topiclock = atoi(data.ExtractWord(9, " ").c_str());
	    if (version > 1)
	    {
		ci->memolevel = atoi(data.ExtractWord(10, " ").c_str());
		ci->key = atoi(data.ExtractWord(11, " ").c_str());
	    }
	    else
	    {
		ci->memolevel = cygnus_SOP;
		ci->key = atoi(data.ExtractWord(10, " ").c_str());
	    }
	}
	else if (field.IsSameAs("CA"))
	{
	    if (!ci)
		continue;

	    cygnus_ChanAccess ca;
	    ca.mask = data.ExtractWord(1, " ");
	    ca.level = atoi(data.ExtractWord(2, " ").c_str());
	    ci->access.push_back(ca);
	}
	else if (field.IsSameAs("AK"))
	{
	    if (!ci)
		continue;

	    cygnus_Akick ak;
	    ak.mask = data.ExtractWord(1, " ");
	    ak.setter = data.ExtractWord(2, " ");
	    if (version < 3)
	    {
		ak.level = atoi(data.ExtractWord(3, " ").c_str());
		ak.time = atoi(data.ExtractWord(4, " ").c_str());
		ak.reason = data.After(" ", 4);
	    }
	    else
	    {
		ak.level = 1;
		ak.time = atoi(data.ExtractWord(3, " ").c_str());
		ak.reason = data.After(" ", 3);
	    }

	    ci->akick.push_back(ak);	    
	}
	else if (field.IsSameAs("SU"))
	{
	    if (!ci)
		continue;

	    ci->successor = data.ExtractWord(1, " ");
	}
	else if (field.IsSameAs("GR"))
	{
	    if (!ci)
		continue;

	    ci->greet = data;
	}
	else if (field.IsSameAs("UR"))
	{
	    if (!ci)
		continue;

	    ci->url = data.ExtractWord(1, " ");
	}
	else if (field.IsSameAs("CT"))
	{
	    if (!ci)
		continue;

	    ci->topicsetter = data.ExtractWord(1, " ");
	    ci->topictime = atoi(data.ExtractWord(2, " ").c_str());
	    ci->topic = data.After(" ", 2);
	}
	else if (field.IsSameAs("KY"))
	{
	    if (!ci)
		continue;

	    ci->mlock_key = data.ExtractWord(1, " ");
	}
	else if (field.IsSameAs("LM"))
	{
	    if (!ci)
		continue;

	    ci->mlock_limit = atoi(data.ExtractWord(1, " ").c_str());
	}
	else if (field.IsSameAs("FL"))
	{
	    if (!ci)
		continue;

	    ci->mlock_flood = data.ExtractWord(1, " ");
	}
	else if (field.IsSameAs("LK"))
	{
	    if (!ci)
		continue;

	    ci->mlock_link = data.ExtractWord(1, " ");
	}
    }

    if (ci)
    {
	Chan_Stored_t *chan = Convert::cygnus_CreateChanEntry(ci);
	if (chan != NULL)
	    Magick::instance().chanserv.AddStored(chan);

	// Previous entry ...
	delete ci;
	ci = NULL;
    }

    ETCB();
}

void cygnus_load_root()
{
    BTCB();

    mFile dbfile(cygnus_ROOTSERV_DB);
    if (!dbfile.IsOpened())
	SLOG(LM_EMERGENCY, "Could not open file $1", (cygnus_ROOTSERV_DB));

    int version = 0;
    while (!dbfile.Eof())
    {
	mstring line = dbfile.ReadLine();
	if (line.empty() || line[0u] == '#' || line[0u] == ' ' || line[0u] == '\t')
	    continue;

	mstring field = line.ExtractWord(1, " ");
	mstring data = line.After(" ");

	if (field.IsSameAs("RV"))
	{
	    version = atoi(data.ExtractWord(1, " ").c_str());
	}
	else if (field.IsSameAs("NW") || field.IsSameAs("DE") || field.IsSameAs("RS"))
	{
	    // Ignore it ...
	}
	else if (field.IsSameAs("AK"))
	{
	    time_t set = atoi(data.ExtractWord(4, " ").c_str());
	    Magick::instance().operserv.Akill_insert(data.ExtractWord(1, " "),
		atoi(data.ExtractWord(5, " ").c_str())-set, data.After(" ", 5),
		data.ExtractWord(2, " "), mDateTime(set));
	}
	else if (field.IsSameAs("TR"))
	{
	    Magick::instance().operserv.Clone_insert(data.ExtractWord(1, " ").After("@"),
		atoi(data.ExtractWord(2, " ").c_str()), "Trigger converted from Cygnus",
		Magick::instance().operserv.FirstName());
	}
	else if (field.IsSameAs("EX"))
	{
	    Magick::instance().operserv.Clone_insert(data.ExtractWord(1, " ").After("@"),
		Magick::instance().operserv.Max_Clone(), "Exception converted from Cygnus",
		Magick::instance().operserv.FirstName());
	}
    }

    ETCB();

}

Nick_Stored_t *Convert::cygnus_CreateNickEntry(cygnus_NickInfo * ni)
{
    BTCB();
    if (ni == NULL || ni->nick.empty())
	return NULL;

    if (!ni->host.empty())
    {
	Nick_Stored_t tmp(ni->host);
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick, mDateTime(ni->registered), tmp);

	if (out == NULL)
	    return NULL;
	if (!ni->usermask.empty())
	    out->i_LastMask = ni->usermask;
	out->i_LastSeenTime = mDateTime(ni->lastseen);
	return out;
    }
    else
    {
	Nick_Stored_t *out = new Nick_Stored_t(ni->nick, ni->pass);
	if (out == NULL)
	    return NULL;

	out->i_LastMask = ni->usermask;
	out->i_LastRealName = ni->real;
	out->i_RegTime = mDateTime(ni->registered);
	out->i_LastSeenTime = mDateTime(ni->lastseen);

	if (!ni->email.empty())
	    out->i_Email = ni->email;
	if (!ni->url.empty())
	    out->i_URL = ni->url;
	if (out->i_URL.Contains("http://"))
	    out->i_URL.Remove("http://", false);
	if (out->i_URL.Contains("HTTP://"))
	    out->i_URL.Remove("HTTP://", false);
	if (ni->uin)
	    out->i_ICQ = mstring(ni->uin);

	unsigned int i;
	for (i=0; i<ni->access.size(); i++)
	    out->i_access.insert(ni->access[i]);

	if (ni->flags & cygnus_NF_ENFORCE && !out->L_Protect())
	    out->setting.Protect = true;
	if (ni->flags & cygnus_NF_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ni->flags & cygnus_NF_PRIVATE && !out->L_Private())
	    out->setting.Private = true;
	if (ni->flags & cygnus_NF_PRIVMSG && !out->L_PRIVMSG())
	    out->setting.PRIVMSG = true;
	if (ni->flags & cygnus_NF_HELD && !Magick::instance().nickserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if (ni->flags & cygnus_NF_NOMEMO && !out->L_NoMemo())
	    out->setting.NoMemo = true;

	if (ni->flags & cygnus_NF_FROZEN)
	{
	    out->i_Suspend_By = Magick::instance().nickserv.FirstName();
	    out->i_Suspend_Time = mDateTime::CurrentDateTime();
	    out->i_Comment = ni->freezereason;
	}

	return out;
    }

    ETCB();
}

mstring cygnus_getmodes(int modes)
{
    BTCB();
    mstring retval;

    if (modes & cygnus_CMODE_t)
	retval += "t";
    if (modes & cygnus_CMODE_n)
	retval += "n";
    if (modes & cygnus_CMODE_s)
	retval += "s";
    if (modes & cygnus_CMODE_m)
	retval += "m";
    if (modes & cygnus_CMODE_l)
	retval += "l";
    if (modes & cygnus_CMODE_i)
	retval += "i";
    if (modes & cygnus_CMODE_p)
	retval += "p";
    if (modes & cygnus_CMODE_k)
	retval += "k";
    if (modes & cygnus_CMODE_o)
	retval += "o";
    if (modes & cygnus_CMODE_v)
	retval += "v";
    if (modes & cygnus_CMODE_R)
	retval += "R";
    if (modes & cygnus_CMODE_r)
	retval += "r";
    if (modes & cygnus_CMODE_c)
	retval += "c";
    if (modes & cygnus_CMODE_O)
	retval += "O";
    if (modes & cygnus_CMODE_Q)
	retval += "Q";
    if (modes & cygnus_CMODE_S)
	retval += "S";
    if (modes & cygnus_CMODE_K)
	retval += "K";
    if (modes & cygnus_CMODE_V)
	retval += "V";
    if (modes & cygnus_CMODE_f)
	retval += "f";
    if (modes & cygnus_CMODE_H)
	retval += "H";
    if (modes & cygnus_CMODE_G)
	retval += "G";
    if (modes & cygnus_CMODE_C)
	retval += "C";
    if (modes & cygnus_CMODE_u)
	retval += "u";
    if (modes & cygnus_CMODE_z)
	retval += "z";
    if (modes & cygnus_CMODE_N)
	retval += "N";
    if (modes & cygnus_CMODE_L)
	retval += "L";
    if (modes & cygnus_CMODE_A)
	retval += "A";
    if (modes & cygnus_CMODE_h)
	retval += "h";
    if (modes & cygnus_CMODE_a)
	retval += "a";
    if (modes & cygnus_CMODE_q)
	retval += "q";

    return retval;
    ETCB();
}

Chan_Stored_t *Convert::cygnus_CreateChanEntry(cygnus_ChanInfo * ci)
{
    BTCB();
    if (ci == NULL || ci->name.empty())
	return NULL;

    {
	vector<cygnus_ChanAccess>::iterator access;
	vector<cygnus_Akick>::iterator akick;

	if (ci->founder.empty() || ci->pass.empty())
	{
	    return NULL;
	}

	Chan_Stored_t *out =
	    new Chan_Stored_t(ci->name, ci->founder, ci->pass, ci->name);
	if (out == NULL)
	    return NULL;

	if (!ci->successor.empty())
	    out->i_CoFounder = ci->successor;
	if (!ci->url.empty())
	    out->i_URL = ci->url;
	if (out->i_URL.Contains("http://"))
	    out->i_URL.Remove("http://", false);
	if (out->i_URL.Contains("HTTP://"))
	    out->i_URL.Remove("HTTP://", false);

	out->i_RegTime = mDateTime(ci->registered);
	out->i_LastUsed = mDateTime(ci->lastused);

	for (access=ci->access.begin(); access!=ci->access.end(); access++)
	{
	    if (access->mask.empty())
		continue;

	    switch (access->level)
	    {
	    case cygnus_VOP:
		out->Access_insert(access->mask, Magick::instance().chanserv.LVL("AUTOVOICE"),
			Magick::instance().chanserv.FirstName(),
			mDateTime::CurrentDateTime());
		break;
	    case cygnus_HOP:
		out->Access_insert(access->mask, Magick::instance().chanserv.LVL("AUTOHALFOP"),
			Magick::instance().chanserv.FirstName(),
			mDateTime::CurrentDateTime());
		break;
	    case cygnus_AOP:
		out->Access_insert(access->mask, Magick::instance().chanserv.LVL("AUTOOP"),
			Magick::instance().chanserv.FirstName(),
			mDateTime::CurrentDateTime());
		break;
	    case cygnus_SOP:
		out->Access_insert(access->mask, Magick::instance().chanserv.LVL("SUPER"),
			Magick::instance().chanserv.FirstName(),
			mDateTime::CurrentDateTime());
		break;
	    }
	}

	for (akick=ci->akick.begin(); akick!=ci->akick.end(); akick++)
	{
	    out->Akick_insert(akick->mask, akick->reason, akick->setter, mDateTime(akick->time));
	}

	if (!ci->topic.empty())
	{
	    out->i_Topic = ci->topic;
	    out->i_Topic_Setter = ci->topicsetter;
	    out->i_Topic_Set_Time = mDateTime(ci->topictime);
	}

	if (!ci->greet.empty())
	    out->Message_insert(ci->greet, Magick::instance().chanserv.FirstName());

	if (ci->flags & cygnus_CF_RESTRICTED && !out->L_Restricted())
	    out->setting.Restricted = true;
	if (ci->flags & cygnus_CF_SECURE && !out->L_Secure())
	    out->setting.Secure = true;
	if (ci->flags & cygnus_CF_HELD && !Magick::instance().chanserv.LCK_NoExpire())
	    out->setting.NoExpire = true;
	if (ci->flags & cygnus_CF_VOPALL)
	    out->Level_change("AUTOVOICE", 0, Magick::instance().chanserv.FirstName());
	if (ci->topiclock)
	    out->setting.Topiclock = true;

	switch (ci->memolevel)
	{
	case cygnus_VOP:
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("AUTOVOICE"),
		Magick::instance().chanserv.FirstName());
	    break;
	case cygnus_HOP:
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("AUTOHALFOP"),
		Magick::instance().chanserv.FirstName());
	    break;
	case cygnus_AOP:
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("AUTOOP"),
		Magick::instance().chanserv.FirstName());
	    break;
	case cygnus_SOP:
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.LVL("SUPER"),
		Magick::instance().chanserv.FirstName());
	    break;
	case cygnus_FOUNDER:
	    out->Level_change("WRITEMEMO", Magick::instance().chanserv.Level_Max() + 1,
		Magick::instance().chanserv.FirstName());
	    break;
	}

	if (ci->flags & cygnus_CF_FROZEN)
	{
	    out->i_Suspend_By = Magick::instance().chanserv.FirstName();
	    out->i_Suspend_Time = mDateTime::CurrentDateTime();
	}

	mstring modelock;

	if (ci->mlock_on || !ci->mlock_key.empty() || ci->mlock_limit)
	{
	    mstring modes = cygnus_getmodes(ci->mlock_on);

	    modes.Remove("k");
	    modes.Remove("l");
	    modelock << "+" << modes << (!ci->mlock_key.empty() ? "k" : "") << (ci->mlock_limit ? "l" : "");
	}
	if (ci->mlock_off)
	{
	    modelock << "-" << cygnus_getmodes(ci->mlock_off);
	}
	if (!ci->mlock_key.empty())
	{
	    modelock << " " << ci->mlock_key;
	}
	if (ci->mlock_limit)
	{
	    modelock << " " << ci->mlock_limit;
	}
	if (modelock.length())
	    out->Mlock(Magick::instance().chanserv.FirstName(), modelock);

	return out;
    }

    ETCB();
}

MemoServ::nick_memo_t Convert::cygnus_CreateMemoEntry(cygnus_MemoInfo *mi, const char *name)
{
    BTCB();
    unsigned int i;

    MemoServ::nick_memo_t out;
    Memo_t *tmp;

    for (i = 0; i < mi->memos.size(); i++)
    {
	if (mi->memos[i].sender.empty() || mi->memos[i].text.empty())
	    continue;

	tmp = new Memo_t(name, mi->memos[i].sender, mi->memos[i].text);
	tmp->i_Time = mDateTime(mi->memos[i].time);
	if (!(mi->memos[i].flags & cygnus_MF_UNREAD))
	    tmp->i_Read = true;
	out.push_back(*tmp);
	delete tmp;
    }
    return out;
    ETCB();
}


#endif /* CONVERT */
