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
** Revision 1.210  2000/09/30 10:48:06  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.209  2000/09/27 11:21:38  prez
** Added a BURST mode ...
**
** Revision 1.208  2000/09/22 12:26:11  prez
** Fixed that pesky bug with chanserv not seeing modes *sigh*
**
** Revision 1.207  2000/09/13 12:45:33  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.206  2000/09/12 21:17:01  prez
** Added IsLiveAll (IsLive now checks to see if user is SQUIT).
**
** Revision 1.205  2000/09/11 10:58:19  prez
** Now saves in in GMT
**
** Revision 1.204  2000/09/10 09:53:43  prez
** Added functionality to ensure the order of messages is kept.
**
** Revision 1.203  2000/09/09 02:17:47  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.202  2000/09/05 10:53:06  prez
** Only have operserv.cpp and server.cpp to go with T_Changing / T_Modify
** tracing -- also modified keygen to allow for cmdline generation (ie.
** specify 1 option and enter keys, or 2 options and the key is read from
** a file).  This allows for paragraphs with \n's in them, and helps so you
** do not have to type out 1024 bytes :)
**
** Revision 1.201  2000/09/02 07:20:45  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.200  2000/08/28 10:51:35  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.199  2000/08/22 08:43:39  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.198  2000/08/19 14:45:02  prez
** Fixed mode settings upon commitee recognitition syntax checking
**
** Revision 1.197  2000/08/19 10:59:46  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.196  2000/08/10 22:44:22  prez
** Added 'binding to IP' options for shell servers, etc.  Also added akick
** triggers for when a user changes their nick and suddenly matches akick.
**
** Revision 1.195  2000/08/09 12:14:42  prez
** Ensured chanserv infinate loops wont occur, added 2 new cmdline
** paramaters, and added a manpage (you need to perl2pod it tho).
**
** Revision 1.194  2000/08/08 09:58:55  prez
** Added ModeO to 4 pre-defined committees.
** Also added back some deletes in xml in the hope that it
** will free up some memory ...
**
** Revision 1.193  2000/08/07 12:20:27  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.192  2000/08/06 07:25:10  prez
** Fixed some minor channel bugs
**
** Revision 1.191  2000/08/06 05:27:46  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.190  2000/08/03 13:06:30  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.189  2000/08/02 20:08:56  prez
** Minor code cleanups, added ACE installation instructions, updated the
** suggestions file and stopped people doing a whole bunch of stuff to
** forbidden nicknames.
**
** Revision 1.188  2000/07/30 09:04:05  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.187  2000/07/29 21:58:52  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.186  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.185  2000/06/28 12:20:47  prez
** Lots of encryption stuff, but essentially, we now have random
** key generation for the keyfile keys, and we can actually encrypt
** something, and get it back as we sent it in (specifically, the
** keyfile itself).
**
** Revision 1.184  2000/06/25 10:32:41  prez
** Fixed channel forbid.
**
** Revision 1.183  2000/06/25 07:58:49  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.182  2000/06/18 13:31:47  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.181  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.180  2000/06/16 14:47:31  prez
** Finished chanserv locking ...
**
** Revision 1.179  2000/06/15 13:41:11  prez
** Added my tasks to develop *grin*
** Also did all the chanserv live locking (stored to be done).
** Also made magick check if its running, and kill on startup if so.
**
** Revision 1.178  2000/06/13 14:11:53  prez
** Locking system has been re-written, it doent core anymore.
** So I have set 'MAGICK_LOCKS_WORK' define active :)
**
** Revision 1.177  2000/06/12 06:07:50  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.176  2000/06/11 09:30:20  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.175  2000/06/11 08:20:12  prez
** More minor bug fixes, godda love testers.
**
** Revision 1.174  2000/06/10 07:01:02  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.173  2000/06/06 08:57:55  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.172  2000/05/27 15:10:11  prez
** Misc changes, mainly re-did the makefile system, makes more sense.
** Also added a config.h file.
**
** Revision 1.171  2000/05/27 07:06:01  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.170  2000/05/25 08:16:38  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.169  2000/05/21 04:49:39  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.168  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.167  2000/05/14 06:30:13  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.166  2000/05/14 04:02:52  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.165  2000/05/10 11:46:59  prez
** added back memo timers
**
** Revision 1.164  2000/05/08 14:42:01  prez
** More on xmlisation of nickserv and chanserv
**
** Revision 1.163  2000/04/30 03:48:28  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.162  2000/04/16 14:29:43  prez
** Added stats for usage, and standardized grabbing paths, etc.
**
** Revision 1.161  2000/04/04 03:13:50  prez
** Added support for masking hostnames.
**
** Revision 1.160  2000/04/03 09:45:21  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.159  2000/04/02 13:06:03  prez
** Fixed the channel TOPIC and MODE LOCK stuff ...
**
** Also fixed the setting of both on join...
**
** Revision 1.158  2000/04/02 08:08:08  prez
** Misc Changes
**
** Revision 1.157  2000/03/29 09:41:17  prez
** Attempting to fix thread problem with mBase, and added notification
** of new memos on join of channel or signon to network.
**
** Revision 1.156  2000/03/28 16:20:57  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.155  2000/03/28 09:42:10  prez
** Changed CommServ, ADD/DEL/LIST -> MEMBER ADD/DEL/LIST
** and NEW/KILL -> ADD/DEL and created a new LIST
**
** Revision 1.154  2000/03/27 21:26:12  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.153  2000/03/27 10:40:11  prez
** Started implementing revenge
**
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

#ifdef __BORLAND__
#pragma codeseg CODE2
#endif

// Private functions

bool Chan_Live_t::Join(mstring nick)
{
    FT("Chan_Live_t::Join", (nick));

    WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    if (users.find(nick.LowerCase())!=users.end())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/DUP_CHAN"),
		"JOIN", nick.c_str(), i_Name.c_str());
	RET(false);
    }
    else
    {
	MCB(users.size());
	users[nick.LowerCase()] = pair<bool,bool>(false,false);
	MCE(users.size());
	RET(true);
    }
}

unsigned int Chan_Live_t::Part(mstring nick)
{
    FT("Chan_Live_t::Part", (nick));
    WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "squit"));
    if (users.find(nick.LowerCase())!=users.end())
    {
	mstring target = nick.LowerCase();
	if (Parent->nickserv.IsStored(nick) &&
	    Parent->nickserv.stored[nick.LowerCase()].IsOnline() &&
	    Parent->nickserv.stored[nick.LowerCase()].Host() != "")
	    target = Parent->nickserv.stored[nick.LowerCase()].Host().LowerCase();
	MCB(users.size());
	CB(1, recent_parts.size());
	recent_parts[target] = Now();
	users.erase(nick.LowerCase());
	if (!users.size())
	{
	    CB(2, modes);
	    CB(3, i_Limit);
	    CB(4, i_Key);
	    modes = "";
	    i_Limit = 0;
	    i_Key = "";
	    CE(2, modes);
	    CE(3, i_Limit);
	    CE(4, i_Key);
	}
	CE(1, recent_parts.size());
	MCE(users.size());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].Part(nick);
    }
    else if (squit.find(nick.LowerCase())!=squit.end())
    {
	MCB(squit.size());
	squit.erase(nick.LowerCase());
	MCE(squit.size());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].Part(nick);
    }
    else
	Log(LM_TRACE, Parent->getLogMessage("ERROR/REC_FORNOTINCHAN"),
	    "PART", nick.c_str(), i_Name.c_str());

    unsigned int retval = users.size() + squit.size();
    RET(retval);
}

void Chan_Live_t::Squit(mstring nick)
{
    FT("Chan_Live_t::Squit", (nick));
    WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "squit"));
    if (users.find(nick.LowerCase())!=users.end())
    {
	MCB(squit.size());
	CB(1, users.size());
	squit[nick.LowerCase()] = users[nick.LowerCase()];
	users.erase(nick.LowerCase());
	CE(1, users.size());
	MCE(squit.size());
    }
}

void Chan_Live_t::UnSquit(mstring nick)
{
    FT("Chan_Live_t::UnSquit", (nick));

    // We'll get ALL modes if all users are squit
    WLOCK(("ChanServ", "live", i_Name.LowerCase(), "modes"));
    if (!users.size())
    {
	MCB(modes);
	modes = "";
	MCE(modes);
    }

    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "squit"));
    if (squit.find(nick.LowerCase())==squit.end())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNOTINCHAN"),
		"UNSQUIT", nick.c_str(), i_Name.c_str());
    }
    else
	Part(nick);
}

unsigned int Chan_Live_t::Kick(mstring nick, mstring kicker)
{
    FT("Chan_Live_t::Kick", (nick, kicker));
    WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "squit"));
    if (users.find(nick.LowerCase())!=users.end())
    {
	MCB(users.size());
	users.erase(nick.LowerCase());
	MCE(users.size());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].Kick(nick, kicker);
    }
    else if (squit.find(nick.LowerCase())!=squit.end())
    {
	MCB(squit.size());
	squit.erase(nick.LowerCase());
	MCE(squit.size());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].Kick(nick, kicker);
    }
    else
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_NOTINCHAN"),
		"KICK", kicker.c_str(), nick.c_str(), i_Name.c_str());
    }

    unsigned int retval = users.size() + squit.size();
    RET(retval);
}

void Chan_Live_t::ChgNick(mstring nick, mstring newnick)
{
    FT("Chan_Live_t::ChgNick", (nick, newnick));
    WLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "squit"));
    if (users.find(nick.LowerCase())!=users.end())
    {
	MCB(users.size());
	users[newnick.LowerCase()] = users[nick.LowerCase()];
	users.erase(nick.LowerCase());
	MCE(users.size());
	if (Parent->chanserv.IsStored(i_Name))
	    Parent->chanserv.stored[i_Name.LowerCase()].ChgNick(nick, newnick);
    }
    else if (squit.find(nick.LowerCase())!=squit.end())
    {
	MCB(squit.size());
	squit[newnick.LowerCase()] = squit[nick.LowerCase()];
	squit.erase(nick.LowerCase());
	MCE(squit.size());
    }
    else
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNOTINCHAN"),
		"NICK", nick.c_str(), i_Name.c_str());
    }
}


Chan_Live_t::Chan_Live_t(mstring name, mstring first_user)
{
    FT("Chan_Live_t::Chan_Live_t", (name, first_user));
    WLOCK(("ChanServ", "live", i_Name.LowerCase()));
    i_Name = name;
    i_Limit = 0;
    ph_timer = 0;
    users[first_user.LowerCase()] = pair<bool,bool>(false,false);
    DumpB();
}


void Chan_Live_t::operator=(const Chan_Live_t &in)
{
    NFT("Chan_Live_t::operator=");
    WLOCK(("ChanServ", "live", in.i_Name.LowerCase()));

    i_Name=in.i_Name;
    i_Creation_Time=in.i_Creation_Time;
    users.clear();
    map<mstring, pair<bool, bool> >::const_iterator k;
    for(k=in.users.begin();k!=in.users.end();k++)
	users.insert(*k);
    for(k=in.squit.begin();k!=in.squit.end();k++)
	squit.insert(*k);
    bans.clear();
    exempt.clear();
    map<mstring, mDateTime>::const_iterator i;
    for(i=in.bans.begin();i!=in.bans.end();i++)
	bans.insert(*i);
    for(i=in.exempt.begin();i!=in.exempt.end();i++)
	exempt.insert(*i);
    i_Topic=in.i_Topic;
    i_Topic_Set_Time=in.i_Topic_Set_Time;
    i_Topic_Setter=in.i_Topic_Setter;
    modes=in.modes;
    i_Key=in.i_Key;
    i_Limit=in.i_Limit;
    p_modes_on=in.p_modes_on;
    p_modes_off=in.p_modes_off;
    p_modes_on_params.clear();
    p_modes_on_params=in.p_modes_on_params;
    p_modes_off_params.clear();
    p_modes_off_params=in.p_modes_off_params;
    ph_timer=in.ph_timer;
    recent_parts.clear();
    for(i=in.recent_parts.begin(); i!=in.recent_parts.end(); i++)
	recent_parts.insert(*i);
    i_UserDef.clear();
    map<mstring,mstring>::const_iterator j;
    for(j=in.i_UserDef.begin();j!=in.i_UserDef.end();j++)
	i_UserDef.insert(*j);
}

mDateTime Chan_Live_t::Creation_Time()
{
    NFT("Chan_Live_t::Creation_Time");
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Creation_Time"));
    RET(i_Creation_Time);
}


void Chan_Live_t::Topic(mstring source, mstring topic, mstring setter, mDateTime time)
{
    FT("Chan_Live_t::Topic", (source, topic, setter, time));
    WLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Topic"));
    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "i_Topic_Setter"));
    WLOCK3(("ChanServ", "live", i_Name.LowerCase(), "i_Topic_Set_Time"));
    MCB(i_Topic);
    CB(1, i_Topic_Setter);
    CB(2, i_Topic_Set_Time);
    i_Topic = topic;
    i_Topic_Setter = setter;
    i_Topic_Set_Time = time;
    CE(1, i_Topic_Setter);
    CE(2, i_Topic_Set_Time);
    MCE(i_Topic);
    if (Parent->chanserv.IsStored(i_Name))
	Parent->chanserv.stored[i_Name.LowerCase()].Topic(source, i_Topic, i_Topic_Setter, i_Topic_Set_Time);
}


mstring Chan_Live_t::Topic()
{
    NFT(("Chan_Live_t::Topic"));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Topic"));
    RET(i_Topic);
}

mstring Chan_Live_t::Topic_Setter()
{
    NFT(("Chan_Live_t::Topic_Setter"));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Topic_Setter"));
    RET(i_Topic_Setter);
}

mDateTime Chan_Live_t::Topic_Set_Time()
{
    NFT(("Chan_Live_t::Topic_Set_Time"));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Topic_Set_Time"));
    RET(i_Topic_Set_Time);
}


unsigned int Chan_Live_t::Squit()
{
    NFT("Chan_Livt_t::Squit");
    unsigned int retval = squit.size();
    RET(retval);
}


mstring Chan_Live_t::Squit(unsigned int num)
{
    FT("Chan_Live_t::Squit", (num));
    unsigned int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "squit"));
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
    unsigned int retval = users.size();
    RET(retval);
}


mstring Chan_Live_t::User(unsigned int num)
{
    FT("Chan_Live_t::User", (num));
    unsigned int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
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
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    unsigned int retval = count_if(users.begin(),users.end(),checkops);
    RET(retval);
}


mstring Chan_Live_t::Op(unsigned int num)
{
    FT("Chan_Live_t::Op", (num));
    unsigned int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
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
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    unsigned int retval = count_if(users.begin(),users.end(),checkvoices);
    RET(retval);
}


mstring Chan_Live_t::Voice(unsigned int num)
{
    FT("Chan_Live_t::Voice", (num));
    unsigned int i;
    map<mstring, pair<bool, bool> >::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
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
	RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
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
    unsigned int retval = bans.size();
    RET(retval);
}


mstring Chan_Live_t::Ban(unsigned int num)
{
    FT("Chan_Live_t::Ban", (num));
    unsigned int i;
    map<mstring, mDateTime>::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "bans"));
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
    mDateTime retval(0.0);
    if (IsBan(mask))
    {
	RLOCK(("ChanServ", "live", i_Name.LowerCase(), "bans"));
	retval = bans[mask.LowerCase()];
    }
    RET(retval);
}


unsigned int Chan_Live_t::Exempts()
{
    NFT("Chan_Live_t::Exempts");
    unsigned int retval = exempt.size();
    RET(retval);
}


mstring Chan_Live_t::Exempt(unsigned int num)
{
    FT("Chan_Live_t::Exempt", (num));
    unsigned int i;
    map<mstring, mDateTime>::const_iterator k;
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
    for(i=0, k=exempt.begin();k!=exempt.end();k++, i++)
	if (i==num)
	{
	    RET(k->first);
	}
 
    RET("");
}


mDateTime Chan_Live_t::Exempt(mstring mask)
{
    FT("Chan_Live_t::Exempt", (mask));
    mDateTime retval(0.0);
    if (IsExempt(mask))
    {
	RLOCK(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
	retval = exempt[mask.LowerCase()];
    }
    RET(retval);
}


bool Chan_Live_t::IsSquit(mstring nick)
{
    FT("Chan_Live_t::IsSquit", (nick));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "squit"));
    bool retval = (squit.find(nick.LowerCase()) != squit.end());
    RET(retval);
}


bool Chan_Live_t::IsIn(mstring nick)
{
    FT("Chan_Live_t::IsIn", (nick));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
    bool retval = (users.find(nick.LowerCase()) != users.end());
    RET(retval);
}


bool Chan_Live_t::IsOp(mstring nick)
{
    FT("Chan_Live_t::IsOp", (nick));

    if (IsIn(nick))
    {
	RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
	if (users[nick.LowerCase()].first == true)
	{
	    RET(true);
	}
    }
    RET(false);
}


bool Chan_Live_t::IsVoice(mstring nick)
{
    FT("Chan_Live_t::IsVoice", (nick));

    if (IsIn(nick))
    {
	RLOCK(("ChanServ", "live", i_Name.LowerCase(), "users"));
	if (users[nick.LowerCase()].second == true)
	{
	    RET(true);
	}
    }
    RET(false);

}


bool Chan_Live_t::IsBan(mstring mask)
{
    FT("Chan_Live_t::IsBan", (mask));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "bans"));
    bool retval = (bans.find(mask.LowerCase()) != bans.end());
    RET(retval);
}

bool Chan_Live_t::MatchBan(mstring mask)
{
    FT("Chan_Live_t::MatchBan", (mask));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "bans"));
    map<mstring, mDateTime>::iterator i;
    for (i=bans.begin(); i!=bans.end(); i++)
    {
	if (mask.Matches(i->first))
	{
	    RET(true);
	}
    }
    RET(false);
}

bool Chan_Live_t::IsExempt(mstring mask)
{
    FT("Chan_Live_t::IsExempt", (mask));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
    bool retval = (exempt.find(mask.LowerCase()) != exempt.end());
    RET(retval);
}

bool Chan_Live_t::MatchExempt(mstring mask)
{
    FT("Chan_Live_t::MatchExempt", (mask));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
    map<mstring, mDateTime>::iterator i;
    for (i=exempt.begin(); i!=exempt.end(); i++)
    {
	if (mask.Matches(i->first))
	{
	    RET(true);
	}
    }
    RET(false);
}

void Chan_Live_t::LockDown()
{
    NFT("Chan_Live_t::LockDown");

    Parent->server.JOIN(Parent->chanserv.FirstName(), i_Name);
    // Override the MLOCK checking.
    SendMode("+s");
    MLOCK(("ChanServ", "live", i_Name.LowerCase(), "ph_timer"));
    MCB(ph_timer);
    ph_timer = ACE_Reactor::instance()->schedule_timer(&(Parent->chanserv.ph),
			    new mstring(i_Name),
			    ACE_Time_Value(Parent->chanserv.ChanKeep()));
    MCE(ph_timer);
}

void Chan_Live_t::UnLock()
{
    NFT("Chan_Live_t::UnLock");

    if (Parent->chanserv.IsStored(i_Name))
    {
	Chan_Stored_t *chan = &Parent->chanserv.stored[i_Name.LowerCase()];
	if (!chan->Mlock_On().Contains("s") && modes.Contains("s"))
	    SendMode("-s");
    }

    mstring *arg = NULL;
    MLOCK(("ChanServ", "live", i_Name.LowerCase(), "ph_timer"));
    MCB(ph_timer);
    if (ph_timer &&
	ACE_Reactor::instance()->cancel_timer(ph_timer,
		(const void **) arg) &&
	arg != NULL)
    {
	    delete arg;
    }
    ph_timer = 0;
    MCE(ph_timer);
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
	if (Parent->server.proto.ChanModeArg().Contains(mode[i]))
	{
	    if (mode[i] == reqmode && param < mode_params.size()
		&& mode_params[param] == reqparam)
	    {
		    RET(true);
	    }
	    if (mode[i] != 'l' || change)
		param++;
	}
	else
	{
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
    FT("Chan_Live_t::RemoveMode", (mode, "vector<mstring> mode_params", change, reqmode, reqparam));
    unsigned int i, param;
    mstring new_mode;
    vector<mstring> new_params;

    for (param=0, i=0; i<mode.size(); i++)
    {
	if (Parent->server.proto.ChanModeArg().Contains(mode[i]))
	{
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
		    if (mode[i] != 'l' || change)
			new_params.push_back(mode_params[param]);
		}
	    }
	    if (mode[i] != 'l' || !change)
		param++;
	}
	else
	{
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

    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "modes"));
    WLOCK(("ChanServ", "live", i_Name.LowerCase(), "p_modes_on"));
    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "p_modes_on_params"));
    WLOCK3(("ChanServ", "live", i_Name.LowerCase(), "p_modes_off"));
    WLOCK4(("ChanServ", "live", i_Name.LowerCase(), "p_modes_off_params"));
    MCB(p_modes_on);
    CB(1, p_modes_on_params.size());
    CB(2, p_modes_off);
    CB(3, p_modes_off_params.size());
    for (i=0; i<mode.size(); i++)
    {
	if (mode[i] == '+')
	{
	    add = true;
	}
	else if (mode[i] == '-')
	{
	    add = false;
	}
	else if (Parent->server.proto.ChanModeArg().Contains(mode[i]))
	{
	    switch (mode[i])
	    {
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

	    case 'e':
		if (in.WordCount(" ") >= param)
		{
		    if (add)
		    {
			if (!IsExempt(in.ExtractWord(param, " ")))
			{
			    if (ModeExists(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(param, " ")))
				RemoveMode(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(param, " "));
			    if (!ModeExists(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(param, " ")))
			    {
				p_modes_on += "e";
				p_modes_on_params.push_back(in.ExtractWord(param, " "));
			    }
			}
		    }
		    else
		    {
			if (IsExempt(in.ExtractWord(param, " ")))
			{
			    if (ModeExists(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(param, " ")))
				RemoveMode(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(param, " "));
			    if (!ModeExists(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(param, " ")))
			    {
				p_modes_off += "e";
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
		    if (!(Parent->chanserv.IsStored(i_Name) &&
			Parent->chanserv.stored[i_Name.LowerCase()].Mlock_Off().Contains("l")))
		    {
		    if (in.WordCount(" ") >= param)
		    {
			if (ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
			    RemoveMode(p_modes_off, p_modes_off_params, false, 'l');
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'l'))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'l');
			p_modes_on += "l";
			p_modes_on_params.push_back(in.ExtractWord(param, " "));
			param++;
		    }}
		}
		else
		{
		    if (!(Parent->chanserv.IsStored(i_Name) &&
			Parent->chanserv.stored[i_Name.LowerCase()].Mlock_On().Contains("l")))
		    {
		    if (i_Limit)
		    {
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'l'))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'l');
			if (!ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
		        {
			    p_modes_off += "l";
			}
		    }}
		}
		break;
	    case 'k':
		if (in.WordCount(" ") >= param)
		{
		    if (add)
		    {
			// ONLY allow +k if we've turned it off before, or one isnt set
			if (i_Key == "" || ModeExists(p_modes_off, p_modes_off_params, false, 'k'))
			{
			    if (!(Parent->chanserv.IsStored(i_Name) &&
				Parent->chanserv.stored[i_Name.LowerCase()].Mlock_Off().Contains("k")))
			    {
				// DONT take off 'off' value, coz we can -k+k key1 key2
				if (!ModeExists(p_modes_on, p_modes_on_params, true, 'k'))
				{
				    p_modes_on += "k";
				    p_modes_on_params.push_back(in.ExtractWord(param, " "));
				}
			    }
			}
		    }
		    else
		    {
			if (i_Key == in.ExtractWord(param, " "))
			{
			    if (!(Parent->chanserv.IsStored(i_Name) &&
				Parent->chanserv.stored[i_Name.LowerCase()].Mlock_On().Contains("k")))
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
		    param++;
		}
		break;
	    default:
		if (in.WordCount(" ") >= param)
		    param++;
	    }
	}
	else
	{
	    if (add)
	    {
		if (!modes.Contains(mode[i]))
		{
		    if (!(Parent->chanserv.IsStored(i_Name) &&
			Parent->chanserv.stored[i_Name.LowerCase()].Mlock_Off().Contains(mstring(mode[i]))))
		    {
			if (ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
			    RemoveMode(p_modes_off, p_modes_off_params, false, mode[i]);
			if (!ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
			    p_modes_on += mode[i];
		    }
		}
	    }
	    else
	    {
		if (modes.Contains(mode[i]))
		{
		    if (!(Parent->chanserv.IsStored(i_Name) &&
			Parent->chanserv.stored[i_Name.LowerCase()].Mlock_On().Contains(mstring(mode[i]))))
		    {
			if (ModeExists(p_modes_on, p_modes_on_params, true, mode[i]))
			    RemoveMode(p_modes_on, p_modes_on_params, true, mode[i]);
			if (!ModeExists(p_modes_off, p_modes_off_params, false, mode[i]))
			    p_modes_off += mode[i];
		    }
		}
	    }
	}
    }	
    CE(1, p_modes_on_params.size());
    CE(2, p_modes_off);
    CE(3, p_modes_off_params.size());
    MCE(p_modes_on);
}


void Chan_Live_t::Mode(mstring source, mstring in)
{
    FT("Chan_Live_t::Mode", (source, in));

    mstring change = in.ExtractWord(1, ": ");
    mstring newmode, newmode_param, requeue, requeue_param;
    unsigned int fwdargs = 2, i, wc;
    bool add = true;
    CP(("MODE CHANGE (%s): %s", i_Name.c_str(), in.c_str()));
    wc = in.WordCount(": ");

    requeue << ":" << source << " MODE " << i_Name << " ";
    if (source.Contains(".") &&
	Parent->server.SeenMessage(requeue+in) >= Parent->config.MSG_Seen_Act())
    {
	for (i=0; i<change.size(); i++)
	{
	    if (change[i] == '+')
		add = true;
	    else if (change[i] == '-')
		add = false;
	    else if (Parent->server.proto.ChanModeArg().Contains(change[i]))
	    {
		switch(change[i])
		{
		case 'o':
		case 'v':
		    if (fwdargs <= wc)
		    {
		    Log(LM_ERROR, Parent->getLogMessage("ERROR/MODE_NOTINCHAN"),
			add ? '+' : '-', change[i], source.c_str(),
			in.ExtractWord(fwdargs, ": ").c_str(), i_Name.c_str());
		    }
		default:
		    fwdargs++;
		}
	    }
	}
	return;
    }

    WLOCK(("ChanServ", "live", i_Name.LowerCase(), "modes"));
    WLOCK2(("ChanServ", "live", i_Name.LowerCase(), "p_modes_on"));
    WLOCK3(("ChanServ", "live", i_Name.LowerCase(), "p_modes_on_params"));
    WLOCK4(("ChanServ", "live", i_Name.LowerCase(), "p_modes_off"));
    WLOCK5(("ChanServ", "live", i_Name.LowerCase(), "p_modes_off_params"));
    MCB(modes);
    CB(1, p_modes_on);
    CB(2, p_modes_on_params.size());
    CB(3, p_modes_off);
    CB(4, p_modes_off_params.size());
    for (fwdargs=2, i=0; i<change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	    newmode += change[i];
	    requeue += change[i];
	}
	else if (change[i] == '-')
	{
	    add = false;
	    newmode += change[i];
	    requeue += change[i];
	}
	else if (Parent->server.proto.ChanModeArg().Contains(change[i]))
	{
	    switch(change[i])
	    {
	    case 'o':
		if (fwdargs <= wc)
		{
		if (IsIn(in.ExtractWord(fwdargs, ": ")))
		{
		    WLOCK6(("ChanServ", "live", i_Name.LowerCase(), "users"));
		    if (add)
		    {
			users[in.ExtractWord(fwdargs, ": ").LowerCase()].first = true;
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'o', in.ExtractWord(fwdargs, ": ")))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'o', in.ExtractWord(fwdargs, ": "));
		    }
		    else
		    {
			users[in.ExtractWord(fwdargs, ": ").LowerCase()].first = false;
			if (ModeExists(p_modes_off, p_modes_off_params, false, 'o', in.ExtractWord(fwdargs, ": ")))
			    RemoveMode(p_modes_off, p_modes_off_params, false, 'o', in.ExtractWord(fwdargs, ": "));
		    }
		    newmode += change[i];
		    newmode_param += " " + in.ExtractWord(fwdargs, ": ");
		}
		else
		{
		    requeue += change[i];
		    requeue_param += " " + in.ExtractWord(fwdargs, ": ");
		}
		fwdargs++;
		}
		break;

	    case 'v':
		if (fwdargs <= wc)
		{
		if (IsIn(in.ExtractWord(fwdargs, ": ")))
		{
		    WLOCK6(("ChanServ", "live", i_Name.LowerCase(), "users"));
		    if (add)
		    {
			users[in.ExtractWord(fwdargs, ": ").LowerCase()].second = true;
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'v', in.ExtractWord(fwdargs, ": ")))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'v', in.ExtractWord(fwdargs, ": "));
		    }
		    else
		    {
			users[in.ExtractWord(fwdargs, ": ").LowerCase()].second = false;
			if (ModeExists(p_modes_off, p_modes_off_params, false, 'v', in.ExtractWord(fwdargs, ": ")))
			    RemoveMode(p_modes_off, p_modes_off_params, false, 'v', in.ExtractWord(fwdargs, ": "));
		    }
		    newmode += change[i];
		    newmode_param += " " + in.ExtractWord(fwdargs, ": ");
		}
		else
		{
		    requeue += change[i];
		    requeue_param += " " + in.ExtractWord(fwdargs, ": ");
		}
		fwdargs++;
		}
	        break;

	    case 'b':
		if (fwdargs <= wc)
		{
		WLOCK6(("ChanServ", "live", i_Name.LowerCase(), "bans"));
		CB(5, bans.size());
		if (add)
		{
		    bans[in.ExtractWord(fwdargs, ": ").LowerCase()] = Now();
		    if (ModeExists(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(fwdargs, ": ")))
			RemoveMode(p_modes_on, p_modes_on_params, true, 'b', in.ExtractWord(fwdargs, ": "));
		}
		else
		{
		    bans.erase(in.ExtractWord(fwdargs, ": ").LowerCase());
		    if (ModeExists(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(fwdargs, ": ")))
			RemoveMode(p_modes_off, p_modes_off_params, false, 'b', in.ExtractWord(fwdargs, ": "));
		}
		CE(5, bans.size());
		newmode += change[i];
		newmode_param += " " + in.ExtractWord(fwdargs, ": ");
		fwdargs++;
		}
		break;

	    case 'e':
		if (fwdargs <= wc)
		{
		WLOCK6(("ChanServ", "live", i_Name.LowerCase(), "exempt"));
		CB(5, exempt.size());
		if (add)
		{
		    exempt[in.ExtractWord(fwdargs, ": ").LowerCase()] = Now();
		    if (ModeExists(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(fwdargs, ": ")))
			RemoveMode(p_modes_on, p_modes_on_params, true, 'e', in.ExtractWord(fwdargs, ": "));
		}
		else
		{
		    exempt.erase(in.ExtractWord(fwdargs, ": ").LowerCase());
		    if (ModeExists(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(fwdargs, ": ")))
			RemoveMode(p_modes_off, p_modes_off_params, false, 'e', in.ExtractWord(fwdargs, ": "));
		}
		CE(5, exempt.size());
		newmode += change[i];
		newmode_param += " " + in.ExtractWord(fwdargs, ": ");
		fwdargs++;
		}
		break;

	    case 'k':
		if (fwdargs <= wc)
		{
		WLOCK6(("ChanServ", "live", i_Name.LowerCase(), "i_Key"));
		CB(5, i_Key);
		if (add)
		{
		    i_Key = in.ExtractWord(fwdargs, ": ");
		    if (ModeExists(p_modes_on, p_modes_on_params, true, 'k', in.ExtractWord(fwdargs, ": ")))
			RemoveMode(p_modes_on, p_modes_on_params, true, 'k', in.ExtractWord(fwdargs, ": "));
		}
		else
		{
		    if (i_Key != in.ExtractWord(fwdargs, ": "))
			Log(LM_ERROR, Parent->getLogMessage("ERROR/KEYMISMATCH"),
				i_Key.c_str(), in.ExtractWord(fwdargs, ": ").c_str(),
				i_Name.c_str(), source.c_str());
		    i_Key = "";
		    if (ModeExists(p_modes_off, p_modes_off_params, false, 'k'))
			RemoveMode(p_modes_off, p_modes_off_params, false, 'k');
		}
		CE(5, i_Key);
		newmode += change[i];
		newmode_param += " " + in.ExtractWord(fwdargs, ": ");
		fwdargs++;
		}
		break;

	    case 'l':
		if (add)
		{
		    if (fwdargs <= wc)
		    {
		    WLOCK6(("ChanServ", "live", i_Name.LowerCase(), "i_Limit"));
		    CB(5, i_Limit);
		    if (fwdargs > in.WordCount(": "))
		    {
			Log(LM_ERROR, Parent->getLogMessage("ERROR/NOLIMIT"),
				i_Name.c_str(), source.c_str());
			i_Limit = 0;
		    }
		    else if (!in.ExtractWord(fwdargs, ": ").IsNumber())
		    {
			Log(LM_ERROR, Parent->getLogMessage("ERROR/NOLIMIT"),
				i_Name.c_str(), source.c_str());
			i_Limit = 0;
		    }
		    else
		    {
			i_Limit = atoi(in.ExtractWord(fwdargs, ": ").c_str());
			if (ModeExists(p_modes_on, p_modes_on_params, true, 'l', in.ExtractWord(fwdargs, ": ")))
			    RemoveMode(p_modes_on, p_modes_on_params, true, 'l', in.ExtractWord(fwdargs, ": "));
			newmode += change[i];
			newmode_param += " " + in.ExtractWord(fwdargs, ": ");
		    }
		    CE(5, i_Limit);
		    fwdargs++;
		    }
		}
		else
		{
		    WLOCK6(("ChanServ", "live", i_Name.LowerCase(), "i_Limit"));
		    CB(5, i_Limit);
		    i_Limit = 0;
		    CE(5, i_Limit);
		    if (ModeExists(p_modes_off, p_modes_off_params, false, 'l'))
			RemoveMode(p_modes_off, p_modes_off_params, false, 'l');
		    newmode += change[i];
		}
		break;
	    default:
		if (fwdargs <= wc)
		{
		    newmode += change[i];
		    newmode_param += " " + in.ExtractWord(fwdargs, ": ");
		    fwdargs++;
		}
	    }
	}
	else
	{
	    if (add && !modes.Contains(change[i]))
	    {
		modes += change[i];
		if (ModeExists(p_modes_on, p_modes_on_params, true, change[i]));
		    RemoveMode(p_modes_on, p_modes_on_params, true, change[i]);
	    }
	    else if (!add && modes.Contains(change[i]))
	    {
		modes.Remove((mstring) change[i]);
		if (ModeExists(p_modes_off, p_modes_off_params, false, change[i]));
		    RemoveMode(p_modes_off, p_modes_off_params, false, change[i]);
	    }
	    else
	    {
		Log(LM_TRACE, Parent->getLogMessage("ERROR/INEFFECT"),
			add ? '+' : '-', change[i], source.c_str(), i_Name.c_str());
	    }
	    newmode += change[i];
	}
    }
    CE(1, p_modes_on);
    CE(2, p_modes_on_params.size());
    CE(3, p_modes_off);
    CE(4, p_modes_off_params.size());
    MCE(modes);

    if (Parent->chanserv.IsStored(i_Name))
	Parent->chanserv.stored[i_Name.LowerCase()].Mode(source,
						newmode + newmode_param);
    if (requeue_param != "")
    {
	if (source.Contains("."))
	{
	    mBase::push_message(requeue + requeue_param);
	}
	else
	{
	    Parent->server.PushUser(source, requeue + requeue_param, i_Name);
	}
    }
}

bool Chan_Live_t::HasMode(mstring in)
{
    FT("Chan_Live_t::HasMode", (in));
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "modes"));
    RET(modes.Contains(in));
}

mstring Chan_Live_t::Mode()
{
    NFT("Chan_Live_t::Mode");
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "modes"));
    RET(modes);
}

mstring Chan_Live_t::Key()
{
    NFT("Chan_Live_t::Key");
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Key"));
    RET(i_Key);
}

unsigned int Chan_Live_t::Limit()
{
    NFT("Chan_Live_t::Limit");
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "i_Limit"));
    RET(i_Limit);
}

mDateTime Chan_Live_t::PartTime(mstring nick)
{
    FT("Chan_Live_t::PartTime", (nick));
    mDateTime retval((time_t) 0);
    RLOCK(("ChanServ", "live", i_Name.LowerCase(), "recent_parts"));
    if (recent_parts.find(nick.LowerCase()) != recent_parts.end())
    {
	retval = recent_parts[nick.LowerCase()];
    }
    RET(retval);
}


size_t Chan_Live_t::Usage()
{
    size_t retval = 0;

    WLOCK(("ChanServ", "live", i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_Creation_Time.Internal());
    map<mstring, pair<bool,bool> >::iterator i;
    for (i=squit.begin(); i!=squit.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second.first);
	retval += sizeof(i->second.second);
    }
    for (i=users.begin(); i!=users.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second.first);
	retval += sizeof(i->second.second);
    }
    map<mstring, mDateTime>::iterator j;
    for (j=bans.begin(); j!=bans.end(); j++)
    {
	retval += j->first.capacity();
	retval += sizeof(j->second.Internal());
    }
    for (j=exempt.begin(); j!=exempt.end(); j++)
    {
	retval += j->first.capacity();
	retval += sizeof(j->second.Internal());
    }
    retval += i_Topic.capacity();
    retval += i_Topic_Setter.capacity();
    retval += sizeof(i_Topic_Set_Time.Internal());
    retval += modes.capacity();
    retval += sizeof(i_Limit);
    retval += i_Key.capacity();
    retval += p_modes_on.capacity();
    retval += p_modes_off.capacity();
    vector<mstring>::iterator k;
    for (k=p_modes_on_params.begin(); k!=p_modes_on_params.end(); k++)
    {
	retval += k->capacity();
    }
    for (k=p_modes_off_params.begin(); k!=p_modes_off_params.end(); k++)
    {
	retval += k->capacity();
    }
    retval += sizeof(ph_timer);
    for (j=recent_parts.begin(); j!=recent_parts.end(); j++)
    {
	retval += j->first.capacity();
	retval += sizeof(j->second.Internal());
    }

    return retval;
}


void Chan_Live_t::DumpB()
{
    MB(0, (i_Name, i_Creation_Time, squit.size(), users.size(), bans.size(),
	exempt.size(), i_Topic, i_Topic_Setter, i_Topic_Set_Time, modes,
	i_Limit, i_Key, p_modes_on, p_modes_off, p_modes_on_params.size(),
	p_modes_off_params.size()));
    MB(16, (ph_timer, recent_parts.size()));
}

void Chan_Live_t::DumpE()
{
    ME(0, (i_Name, i_Creation_Time, squit.size(), users.size(), bans.size(),
	exempt.size(), i_Topic, i_Topic_Setter, i_Topic_Set_Time, modes,
	i_Limit, i_Key, p_modes_on, p_modes_off, p_modes_on_params.size(),
	p_modes_off_params.size()));
    ME(16, (ph_timer, recent_parts.size()));
}


bool checkops(pair<const mstring, pair<bool,bool> > &in)
{
    FT("checkops",(in.first,in.second.first,in.second.second));
    RET(in.second.first);
}


bool checkvoices(pair<const mstring, pair<bool,bool> > &in)
{
    FT("checkvoices",(in.first,in.second.first,in.second.second));
    RET(in.second.second);
}

// --------- end of Chan_Live_t -----------------------------------


void Chan_Stored_t::ChgAttempt(mstring nick, mstring newnick)
{
    FT("Chan_Stored_t::ChgAttempt", (nick, newnick));

    map<mstring, unsigned int>::iterator iter;

    // Create a new one if we find the entry
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "failed_passwds"));
    MCB(failed_passwds.size());
    for (iter=failed_passwds.begin(); iter!=failed_passwds.end(); iter++)
	if (iter->first == nick.LowerCase())
	{
	    failed_passwds[newnick.LowerCase()] = iter->second;
	    break;
	}
    failed_passwds.erase(nick.LowerCase());
    MCE(failed_passwds.size());
}


bool Chan_Stored_t::Join(mstring nick)
{
    FT("Chan_Stored_t::Join", (nick));

    Chan_Live_t *clive = NULL;
    Nick_Live_t *nlive = NULL;
    Nick_Stored_t *nstored = NULL;
    bool burst = false;

    if (Parent->chanserv.IsLive(i_Name))
	clive = &Parent->chanserv.live[i_Name.LowerCase()];
    else
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
			"JOIN", nick.c_str(), i_Name.c_str());
	RET(false);
    }
    size_t users = clive->Users();

    if (Parent->nickserv.IsLive(nick))
	nlive = &Parent->nickserv.live[nick.LowerCase()];
    else
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"JOIN", i_Name.c_str(), nick.c_str());
	RET(false);
    }

    if (nlive->IsServices() &&
	Parent->chanserv.FirstName().CmpNoCase(nick)==0)
    {
	clive->SendMode("+o " + nick);
	RET(true);
    }

    if (Parent->nickserv.IsStored(nick))
	nstored = &Parent->nickserv.stored[nick.LowerCase()];
    else
	nstored = NULL;

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL)
	burst = Parent->ircsvchandler->Burst();
    }    

    if (Forbidden())
    {
	{ RLOCK(("ChanServ", "stored", i_Name, "i_Mlock_On"));
	clive->SendMode(i_Mlock_On);
	}
	if (!nlive->HasMode("o"))
	{
	    if (users == 1)
		clive->LockDown();

	    clive->SendMode("+b " + nlive->AltMask(Parent->operserv.Ignore_Method()));

	    // Can only insert with reason or default, so its safe.
	    mstring reason;
	    reason.Format(Parent->getMessage(nick, "CS_STATUS/ISFORBIDDEN"),
		i_Name.c_str());
	    Parent->server.KICK(Parent->chanserv.FirstName(), nick,
		i_Name, reason);
	    RET(false);
	}
	RET(true);
    }

    { MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    if (Akick_find(nick))
    {
	// If this user is the only user in channel
	if (users == 1)
	    clive->LockDown();

	if (Parent->nickserv.IsLive(Akick->Entry()))
	    clive->SendMode("+b " +
		nlive->AltMask(Parent->operserv.Ignore_Method()));
	else
	    clive->SendMode("+b " + Akick->Entry());

	Log(LM_DEBUG, Parent->getLogMessage("EVENT/AKICK"),
			nick.c_str(), i_Name.c_str(), Akick->Value().c_str());

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), nick,
		i_Name, Akick->Value());

	RET(false);
    }}

    if (Restricted() && !Suspended() && GetAccess(nick) < (long) 1)
    {
	// If this user is the only user in channel
	if (users == 1)
	    clive->LockDown();

	if (Parent->nickserv.IsLive(nick))
	    clive->SendMode("+b " +
		nlive->AltMask(Parent->operserv.Ignore_Method()));
	else
	    clive->SendMode("+b " + nick + "!*@*");

	Log(LM_DEBUG, Parent->getLogMessage("EVENT/RESTRICTED"),
			nick.c_str(), i_Name.c_str());

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), nick,
		i_Name, Parent->getMessage(nick, "MISC/KICK_RESTRICTED"));

	RET(false);
    }

    clive->UnLock();
    if (!Join() && users == 2 &&
			clive->IsIn(Parent->chanserv.FirstName()))
    {
	Parent->server.PART(Parent->chanserv.FirstName(), i_Name);
	users--;
    }

    if (users == 1)
    {
	{ RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
	RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
	RLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
	mstring modes = i_Mlock_On;
	if (i_Mlock_Key != "")
	    modes << "k";
	if (i_Mlock_Limit)
	    modes << "l";

	if (modes != "")
	{
	    clive->SendMode("+" + modes + " " + i_Mlock_Key + " " +
			mstring(i_Mlock_Limit ? itoa(i_Mlock_Limit) : ""));
	}}

	{ RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic"));
	RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Setter"));
	RLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Set_Time"));
	// Carry over topic ..
	if (!burst && Keeptopic() && i_Topic != "")
	{
	    Parent->server.TOPIC(Parent->chanserv.FirstName(),
		i_Topic_Setter, i_Name, i_Topic, i_Topic_Set_Time);
	}}
    }

    if (!burst && Join() && users == 1)
    {
	Parent->server.JOIN(Parent->chanserv.FirstName(), i_Name);
	users++;
    }

    if (GetAccess(nick)>0)
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_LastUsed"));
	MCB(i_LastUsed);
	i_LastUsed = Now();
	MCE(i_LastUsed);
    }

    if (GetAccess(nick, "AUTOOP"))
	clive->SendMode("+o " + nick);
    else if (GetAccess(nick, "AUTOVOICE"))
	clive->SendMode("+v " + nick);

    if (Suspended())
    {
	RET(true);
    }

    mstring target = nick;
    if (nstored != NULL && nstored->Host() != "")
    {
	target = nstored->Host();
    }

    {
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

    if (nstored != NULL && GetAccess(nick, "MEMOREAD") &&
	Parent->memoserv.IsChannel(i_Name))
    {
	list<News_t>::iterator iter;
	unsigned int count = 0;
	RLOCK(("MemoServ", "channel", i_Name.LowerCase()));
	for (iter = Parent->memoserv.channel[i_Name.LowerCase()].begin();
		iter != Parent->memoserv.channel[i_Name.LowerCase()].end();
		iter++)
	{
	    if (!iter->IsRead(target))
		count++;
	}
	if (count)
	    send(Parent->memoserv.FirstName(), nick,
		Parent->getMessage(nick, "MS_STATUS/CS_UNREAD"),
		i_Name.c_str(), count,
		Parent->memoserv.FirstName().c_str(), i_Name.c_str());
    }
    RET(true);
}


void Chan_Stored_t::Part(mstring nick)
{
    FT("Chan_Stored_t::Part", (nick));

    if (Parent->nickserv.IsLive(nick) &&
	Parent->nickserv.live[nick.LowerCase()].IsServices())
	return;

    if (GetAccess(nick)>0)
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_LastUsed"));
	MCB(i_LastUsed);
	i_LastUsed = Now();
	MCE(i_LastUsed);
    }

    Chan_Live_t *clive = NULL;
    if (Parent->chanserv.IsLive(i_Name))
	clive = &Parent->chanserv.live[i_Name.LowerCase()];

    if (clive != NULL && clive->Users() == 1 && Join())
    {
	Parent->server.PART(Parent->chanserv.FirstName(), i_Name);
	clive = NULL;
    }

    if ((clive == NULL || clive->Users() == 0) &&
	Mlock_On().Contains("i") || Mlock_Key() != "")
    {
	Parent->server.JOIN(Parent->chanserv.FirstName(), i_Name);
	if (Parent->chanserv.IsLive(i_Name))
	{
	    clive = &Parent->chanserv.live[i_Name.LowerCase()];
	    clive->SendMode("+s");
	    if (Mlock_On().Contains("i"))
		clive->SendMode("+i");
	    if (Mlock_Key() != "")
		clive->SendMode("+k " + Mlock_Key());
	}
    }

}


void Chan_Stored_t::Kick(mstring nick, mstring kicker)
{
    FT("Chan_Stored_t::Kick", (nick, kicker));

    // Users shouldnt kick us, but we just rejoin!
    if (Parent->nickserv.IsLive(nick) &&
	Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	if (Join())
	    Parent->server.JOIN(nick, i_Name);
	return;
    }

    if (DoRevenge("KICK", kicker, nick))
	Parent->server.INVITE(Parent->chanserv.FirstName(),
		nick, i_Name);
}

void Chan_Stored_t::ChgNick(mstring nick, mstring newnick)
{
    FT("Chan_Stored_t::ChgNick", (nick, newnick));

    Chan_Live_t *clive = NULL;
    Nick_Live_t *nlive = NULL;

    if (Parent->chanserv.IsLive(i_Name))
	clive = &Parent->chanserv.live[i_Name.LowerCase()];
    else
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
			"NICK", nick.c_str(), i_Name.c_str());
	return;
    }
    size_t users = clive->Users();

    if (Parent->nickserv.IsLiveAll(newnick))
	nlive = &Parent->nickserv.live[newnick.LowerCase()];
    else
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"NICK", i_Name.c_str(), newnick.c_str());
	return;
    }

    // Check we dont now trigger AKICK
    { MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    if (Akick_find(newnick))
    {
	// If this user is the only user in channel
	if (users == 1)
	    clive->LockDown();

	if (Parent->nickserv.IsLive(Akick->Entry()))
	    clive->SendMode("+b " +
		nlive->AltMask(Parent->operserv.Ignore_Method()));
	else
	    clive->SendMode("+b " + Akick->Entry());

	Log(LM_DEBUG, Parent->getLogMessage("EVENT/AKICK"),
			newnick.c_str(), i_Name.c_str(), Akick->Value().c_str());

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), newnick,
		i_Name, Akick->Value());

	return;
    }}

    // Check we're still alowed in here!
    if (Restricted() && !Suspended() && GetAccess(newnick) < (long) 1)
    {
	// If this user is the only user in channel
	if (users == 1)
	    clive->LockDown();

	if (Parent->nickserv.IsLive(newnick))
	    clive->SendMode("+b " +
		nlive->AltMask(Parent->operserv.Ignore_Method()));
	else
	    clive->SendMode("+b " + newnick + "!*@*");

	Log(LM_DEBUG, Parent->getLogMessage("EVENT/RESTRICTED"),
			newnick.c_str(), i_Name.c_str());

	// Can only insert with reason or default, so its safe.
	Parent->server.KICK(Parent->chanserv.FirstName(), newnick,
		i_Name, Parent->getMessage(newnick, "MISC/KICK_RESTRICTED"));

	return;
    }
}


void Chan_Stored_t::Topic(mstring source, mstring topic, mstring setter, mDateTime time)
{
    FT("Chan_Stored_t::Topic", (source, topic, setter, time));

    // Still in burst ...
    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL && Parent->ircsvchandler->Burst())
	return;
    }

    // Its us re-setting it!
    if (!source.Contains(".") && Parent->nickserv.IsLive(source) &&
	Parent->nickserv.live[source.LowerCase()].IsServices())
	return;

    if (!Parent->chanserv.IsLive(i_Name))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
			"TOPIC", source.c_str(), i_Name.c_str());
	return;
    }

    if (Suspended())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Comment"));
	Parent->server.TOPIC(Parent->chanserv.FirstName(),
			Parent->chanserv.FirstName(), i_Name, "[" + IRC_Bold +
			Parent->getMessage("VALS/SUSPENDED") + IRC_Off + "] " +
			i_Comment + " [" + IRC_Bold +
			Parent->getMessage("VALS/SUSPENDED") + IRC_Off + "]",
			time - (double) (1.0 / (60.0 * 60.0 * 24.0)));
	return;
    }

    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic"));
    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Setter"));
    WLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Set_Time"));
    if (Topiclock())
    {
	Parent->server.TOPIC(Parent->chanserv.FirstName(),
			i_Topic_Setter, i_Name, i_Topic,
			time - (double) (1.0 / (60.0 * 60.0 * 24.0)));
    }
    else
    {
	MCB(i_Topic);
	CB(1, i_Topic_Setter);
	CB(2, i_Topic_Set_Time);
	i_Topic = topic;
	i_Topic_Setter = setter;
	i_Topic_Set_Time = time;
	CE(1, i_Topic_Setter);
	CE(2, i_Topic_Set_Time);
	MCE(i_Topic);
	
    }
}


void Chan_Stored_t::SetTopic(mstring source, mstring setter, mstring topic)
{
    FT("Chan_Stored_t::SetTopic", (source, setter, topic));

    // Its us re-setting it!
    if (!setter.Contains(".") && Parent->nickserv.IsLive(setter) &&
	Parent->nickserv.live[setter.LowerCase()].IsServices())
	return;

    if (!Parent->chanserv.IsLive(i_Name))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
			"TOPIC", source.c_str(), i_Name.c_str());
	return;
    }

    if (Suspended())
	return;

    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic"));
    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Setter"));
    WLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Set_Time"));
    MCB(i_Topic);
    CB(1, i_Topic_Setter);
    CB(2, i_Topic_Set_Time);
    i_Topic = topic;
    i_Topic_Setter = setter;
    i_Topic_Set_Time = Now();
    CE(1, i_Topic_Setter);
    CE(2, i_Topic_Set_Time);
    MCE(i_Topic);
    Parent->server.TOPIC(source, setter, i_Name, topic,
	Parent->chanserv.live[i_Name.LowerCase()].Topic_Set_Time() -
		(double) (1.0 / (60.0 * 60.0 * 24.0)));
}


void Chan_Stored_t::Mode(mstring setter, mstring mode)
{
    FT("Chan_Stored_t::Mode", (setter, mode));
    // ENFORCE mlock

    if (!Parent->chanserv.IsLive(i_Name))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
			"MODE", setter.c_str(), i_Name.c_str());
	return;
    }

    Chan_Live_t *clive = &Parent->chanserv.live[i_Name.LowerCase()];
    if (Parent->nickserv.IsLive(setter) &&
	Parent->nickserv.live[setter.LowerCase()].IsServices())
	    return;

    mstring change = mode.Before(" ");
    unsigned int fwdargs = 2, i, wc;
    bool add = true;
    wc = mode.WordCount(": ");
    for (i=0; i<change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else if (Parent->server.proto.ChanModeArg().Contains(change[i]))
	{
	    switch(change[i])
	    {
	    case 'o':
		if (fwdargs <= wc)
		{
		if (add)
		{
		    if (Access_value(mode.ExtractWord(fwdargs, ": ")) <= Level_value("AUTODEOP") ||
			(!(GetAccess(mode.ExtractWord(fwdargs, ": "), "CMDOP") ||
			  GetAccess(mode.ExtractWord(fwdargs, ": "), "AUTOOP")) &&
			Secureops()))
		    {
			clive->SendMode("-o " + mode.ExtractWord(fwdargs, ": "));
		    }
		}
		else if (!setter.Contains("."))
		{
		    if ((Parent->nickserv.IsLive(mode.ExtractWord(fwdargs, ": ")) &&
			Parent->nickserv.live[mode.ExtractWord(fwdargs, ": ").LowerCase()].IsServices()) ||
			DoRevenge("DEOP", setter, mode.ExtractWord(fwdargs, ": ")))
		    {
			clive->SendMode("+o " + mode.ExtractWord(fwdargs, ": "));
		    }
		}

		fwdargs++;
		}
		break;

	    case 'v':
		if (fwdargs <= wc)
		{
		if (add)
		{
		    if (Access_value(mode.ExtractWord(fwdargs, ": ")) <= Level_value("AUTODEOP") ||
			(!(GetAccess(mode.ExtractWord(fwdargs, ": "), "CMDVOICE") ||
			  GetAccess(mode.ExtractWord(fwdargs, ": "), "AUTOVOICE")) &&
			Secureops()))
		    {
			clive->SendMode("-v " + mode.ExtractWord(fwdargs, ": "));
		    }
		}

		fwdargs++;
		}
		break;

	    case 'b':
		if (fwdargs <= wc)
		{
		if (add && !setter.Contains("."))
		{
		    vector<mstring> tobekicked;
		    bool DidRevenge = false;
		    mstring bantype = "BAN1";
		    unsigned int j;

		    mstring nick = mode.ExtractWord(fwdargs, ": ").Before("!");
		    mstring user = mode.ExtractWord(fwdargs, ": ").After("!").Before("@");
		    mstring host = mode.ExtractWord(fwdargs, ": ").After("!").After("@");

		    if (host.Contains("*") || host.Contains("?"))
			bantype = "BAN4";
		    else
			bantype = "BAN3";

		    for (j=0; bantype != "BAN2" && j<user.size(); j++)
			switch (user[j])
			{
			case '*':
			case '?':
			    break;
			default:
			    bantype = "BAN2";
			    break;
			}

		    for (j=0; bantype != "BAN1" && j<nick.size(); j++)
			switch (nick[j])
			{
			case '*':
			case '?':
			    break;
			default:
			    bantype = "BAN1";
			    break;
			}

		    for (j=0; !DidRevenge && j<clive->Users(); j++)
		    {
			if (Parent->nickserv.IsLive(clive->User(j)) &&
			    (Parent->nickserv.live[clive->User(j).LowerCase()].Mask(Nick_Live_t::N_U_P_H).Matches(mode.ExtractWord(fwdargs, ": ")) ||
			    Parent->nickserv.live[clive->User(j).LowerCase()].AltMask(Nick_Live_t::N_U_P_H).Matches(mode.ExtractWord(fwdargs, ": "))))
			{
			    // Only do revenge or kickonban if user
			    // is not exempt from bans (+e).
			    if (!(clive->MatchExempt(Parent->nickserv.live[clive->User(j).LowerCase()].Mask(Nick_Live_t::N_U_P_H)) ||
				clive->MatchExempt(Parent->nickserv.live[clive->User(j).LowerCase()].AltMask(Nick_Live_t::N_U_P_H))))
			    {
				if (DidRevenge = DoRevenge(bantype, setter, clive->User(j)))
				    clive->SendMode("-b " + mode.ExtractWord(fwdargs, ": "));
				else
				    tobekicked.push_back(clive->User(j));
			    }
			}
		    }
		    if (!DidRevenge && KickOnBan())
		    {
			for (j=0; j<tobekicked.size(); j++)
			{
			    Parent->server.KICK(Parent->chanserv.FirstName(), tobekicked[j], i_Name, Parent->chanserv.DEF_Akick_Reason());
		        }
		    }
		}
		fwdargs++;
		}
		break;

	    case 'k':
		if (fwdargs <= wc)
		{
		{ RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
		RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
		if (add && i_Mlock_Off.Contains("k"))
		{
		    clive->SendMode("-k " + mode.ExtractWord(fwdargs, ": "));
		}
		else if (!add && i_Mlock_Key != "")
		{
		    clive->SendMode("+k " + i_Mlock_Key);
		}}

		fwdargs++;
		}
		break;

	    case 'l':
		{ RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
		RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
		if (add ? i_Mlock_Off.Contains("l") : i_Mlock_Limit)
		{
		    if (add)
		    {
			clive->SendMode("-l");
		    }
		    else
		    {
			clive->SendMode("+l " + mstring(itoa(i_Mlock_Limit)));
		    }
		}}

		if (add && fwdargs <= wc)
		    fwdargs++;
		break;

	    default:
		if (fwdargs <= wc)
		{
		    fwdargs++;
		}
	    }
	}
	else
	{
	    { RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
	    RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
	    if (add && i_Mlock_Off.Contains(change[i]))
		clive->SendMode("-" + mstring(change[i]));
	    else if (!add && i_Mlock_On.Contains(change[i]))
		clive->SendMode("+" + mstring(change[i]));
	    }
	}
    }
}


void Chan_Stored_t::defaults()
{
    NFT("Chan_Stored_t::defaults");

    // Dont lock in here, we locked outside ...
    i_Mlock_On = i_Mlock_Off = i_Mlock_Key = "";
    l_Mlock_On = l_Mlock_Off = "";
    i_Mlock_Limit = 0;
    
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
    i_KickOnBan = Parent->chanserv.DEF_KickOnBan();
    l_KickOnBan = false;
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
	if (defaulted[i] == '+')
	{
	    add = true;
	}
	else if (defaulted[i] == '-')
	{
	    add = false;
	}
	else if (!Parent->server.proto.ChanModeArg().Contains(defaulted[i]))
	{
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
	}
    }

    add = true;
    for (i=0; i<locked.size(); i++)
    {
	if (locked[i] == '+')
	{
	    add = true;
	}
	else if (locked[i] == '-')
	{
	    add = false;
	}
	else if (!Parent->server.proto.ChanModeArg().Contains(locked[i]))
	{
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

bool Chan_Stored_t::DoRevenge(mstring type, mstring target, mstring source)
{
    FT("Chan_Stored_t::DoRevenge", (type, target, source));

    if (!(Parent->chanserv.IsLive(i_Name) &&
	Parent->nickserv.IsLive(source) &&
	Parent->nickserv.IsLive(target)))
	RET(false);

    if (GetAccess(source) > GetAccess(target))
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Revenge"));
	if (i_Revenge == "REVERSE")
	{
	    // All we need to do now is return 'screw em' (true)
	}
	else if (i_Revenge == "DEOP")
	{
DoRevenge_DeOp:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("-o " + target);
	    send(Parent->chanserv.FirstName(), target,
			Parent->getMessage(target, "MISC/REVENGE"),
			type.c_str(), Parent->getLname(source).c_str());
	}
	else if (i_Revenge == "KICK")
	{
DoRevenge_Kick:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    mstring reason;
	    reason.Format(Parent->getMessage(source, "MISC/REVENGE").c_str(),
			type.c_str(), Parent->getLname(source).c_str());
	    Parent->server.KICK(Parent->chanserv.FirstName(),
			target, i_Name, reason);
	}
	else if (i_Revenge == "BAN1")
	{
DoRevenge_Ban1:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("-o+b " + target + " " +
		Parent->nickserv.live[target.LowerCase()].AltMask(Nick_Live_t::N));
	    mstring reason;
	    reason.Format(Parent->getMessage(source, "MISC/REVENGE").c_str(),
			type.c_str(), Parent->getLname(source).c_str());
	    Parent->server.KICK(Parent->chanserv.FirstName(),
			target, i_Name, reason);
	}
	else if (i_Revenge == "BAN2")
	{
DoRevenge_Ban2:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("-o+b " + target + " " +
		Parent->nickserv.live[target.LowerCase()].AltMask(Nick_Live_t::U_H));
	    mstring reason;
	    reason.Format(Parent->getMessage(source, "MISC/REVENGE").c_str(),
			type.c_str(), Parent->getLname(source).c_str());
	    Parent->server.KICK(Parent->chanserv.FirstName(),
			target, i_Name, reason);
	}
	else if (i_Revenge == "BAN3")
	{
DoRevenge_Ban3:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("-o+b " + target + " " +
		Parent->nickserv.live[target.LowerCase()].AltMask(Nick_Live_t::P_H));
	    mstring reason;
	    reason.Format(Parent->getMessage(source, "MISC/REVENGE").c_str(),
			type.c_str(), Parent->getLname(source).c_str());
	    Parent->server.KICK(Parent->chanserv.FirstName(),
			target, i_Name, reason);
	}
	else if (i_Revenge == "BAN4")
	{
DoRevenge_Ban4:
	    if (type.SubString(0, 2) == "BAN")
		type = "BAN";
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode("-o+b " + target + " " +
		Parent->nickserv.live[target.LowerCase()].AltMask(Nick_Live_t::H));
	    mstring reason;
	    reason.Format(Parent->getMessage(source, "MISC/REVENGE").c_str(),
			type.c_str(), Parent->getLname(source).c_str());
	    Parent->server.KICK(Parent->chanserv.FirstName(),
			target, i_Name, reason);
	}
	else if (i_Revenge == "MIRROR")
	{
	    if (type == "DEOP")
		goto DoRevenge_DeOp;
	    else if (type == "KICK")
		goto DoRevenge_Kick;
	    else if (type == "BAN1")
		goto DoRevenge_Ban1;
	    else if (type == "BAN2")
		goto DoRevenge_Ban2;
	    else if (type == "BAN3")
		goto DoRevenge_Ban3;
	    else if (type == "BAN4")
		goto DoRevenge_Ban4;
	}
	else
	{
	    // Revenge is OFF or UNKNOWN...
	    RET(false);
	}
	Log(LM_INFO, Parent->getLogMessage("OTHER/REVENGE"),
			target.c_str(), i_Name.c_str(), type.c_str(), source.c_str());
	RET(true);
    }

    RET(false);
}

Chan_Stored_t::Chan_Stored_t(mstring name, mstring founder, mstring password, mstring desc)
{
    FT("Chan_Stored_t::Chan_Stored_t", (name, founder, password, desc));

    i_Name = name;
    WLOCK(("ChanServ", "stored", i_Name.LowerCase()));
    defaults();
    i_RegTime = i_LastUsed = Now();
    i_Founder = founder;
    i_Password = password;
    i_Description = desc;
    DumpE();
}


Chan_Stored_t::Chan_Stored_t(mstring name)
{
    FT("Chan_Stored_t::Chan_Stored_t", (name));

    i_Name = name;
    WLOCK(("ChanServ", "stored", i_Name.LowerCase()));
    defaults();
    i_Mlock_On = "nits";
    i_RegTime = i_LastUsed = Now();
    i_Forbidden = true;
    DumpE();
}


void Chan_Stored_t::operator=(const Chan_Stored_t &in)
{
    NFT("Chan_Stored_t::operator=");
    WLOCK(("ChanServ", "stored", in.i_Name.LowerCase()));
    i_Name=in.i_Name;
    i_RegTime=in.i_RegTime;
    i_LastUsed=in.i_LastUsed;
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
    i_KickOnBan=in.i_KickOnBan;
    l_KickOnBan=in.l_KickOnBan;
    i_Restricted=in.i_Restricted;
    l_Restricted=in.l_Restricted;
    i_Join=in.i_Join;
    l_Join=in.l_Join;
    i_Revenge=in.i_Revenge;
    l_Revenge=in.l_Revenge;
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
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_LastUsed"));
    RET(i_LastUsed);
}

mDateTime Chan_Stored_t::RegTime()
{
    NFT("Chan_Stored_t::RegTime");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_RegTime"));
    RET(i_RegTime);
}

void Chan_Stored_t::Founder(mstring in)
{
    FT("Chan_Stored_t::Founder", (in));

    if (!Parent->nickserv.IsStored(in))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/BADSET"),
		in.c_str(), "FOUNDER", i_Name.c_str());
	return;
    }

    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_CoFounder"));
    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Founder"));
    MCB(i_Founder);
    if (i_CoFounder.CmpNoCase(in)==0)
    {
	CB(1, i_CoFounder);
	i_CoFounder = "";
	CE(1, i_CoFounder);
    }

    i_Founder = in;
    MCE(i_Founder);
}


void Chan_Stored_t::CoFounder(mstring in)
{
    FT("Chan_Stored_t::CoFounder", (in));

    if (!Parent->nickserv.IsStored(in))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/BADSET"),
		in.c_str(), "COFOUNDER", i_Name.c_str());
	return;
    }

    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Founder"));
    if (i_Founder.CmpNoCase(in)==0)
	return;
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_CoFounder"));

    MCB(i_CoFounder);
    i_CoFounder = in;
    MCE(i_CoFounder);
}

mstring Chan_Stored_t::Founder()
{
    NFT("Chan_Stored_t::Founder");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Founder"));
    RET(i_Founder);
}

mstring Chan_Stored_t::CoFounder()
{
    NFT("Chan_Stored_t::CoFounder");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_CoFounder"));
    RET(i_CoFounder);
}

void Chan_Stored_t::Description(mstring in)
{
    FT("Chan_Stored_t::Description", (in));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Description"));
    MCB(i_Description);
    i_Description = in;
    MCE(i_Description);
}

mstring Chan_Stored_t::Description()
{
    NFT("Description(mstring in)	{ i_Description = in;");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Description"));
    RET(i_Description);
}

void Chan_Stored_t::Password(mstring in)
{
    FT("Chan_Stored_t::Password", (in));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Password"));
    MCB(i_Password);
    i_Password = in;
    MCE(i_Password);
}

mstring Chan_Stored_t::Password()
{
    NFT("Chan_Stored_t::Password");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Password"));
    RET(i_Password);
}

void Chan_Stored_t::Email(mstring in)
{
    FT("Chan_Stored_t::Email", (in));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Email"));
    MCB(i_Email);
    i_Email = in;
    MCE(i_Email);
}

mstring Chan_Stored_t::Email()
{
    NFT("Chan_Stored_t::Email");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Email"));
    RET(i_Email);
}

void Chan_Stored_t::URL(mstring in)
{
    FT("Chan_Stored_t::URL", (in));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_URL"));
    MCB(i_URL);
    i_URL = in;
    MCE(i_URL);
}

mstring Chan_Stored_t::URL()
{
    NFT("Chan_Stored_t::URL");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_URL"));
    RET(i_URL);
}

void Chan_Stored_t::Comment(mstring in)
{
    FT("Chan_Stored_t::Comment", (in));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Comment"));
    MCB(i_Comment);
    i_Comment = in;
    MCE(i_Comment);
}
	
mstring Chan_Stored_t::Comment()
{
    NFT("Chan_Stored_t::Comment");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Comment"));
    RET(i_Comment);
}


unsigned int Chan_Stored_t::CheckPass(mstring nick, mstring password)
{
    FT("Chan_Stored_t::CheckPass", (nick, password));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "failed_passwds"));
    unsigned int retval = 0;
    MCB(failed_passwds.size());
    if (i_Password == password)
	failed_passwds.erase(nick.LowerCase());
    else
    {
	if (failed_passwds.find(nick.LowerCase()) == failed_passwds.end())
	    failed_passwds[nick.LowerCase()]=0;
	retval = ++failed_passwds[nick.LowerCase()];
    }
    MCE(failed_passwds.size());
    RET(retval);
}


void Chan_Stored_t::Suspend(mstring name)
{
    FT("Chan_Stored_t::Suspend", (name));
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_Time"));
    MCB(i_Suspend_By);
    CB(1, i_Suspend_Time);
    i_Suspend_By = name;
    i_Suspend_Time = Now();
    CE(1, i_Suspend_Time);
    MCE(i_Suspend_By);
}


void Chan_Stored_t::UnSuspend()
{
    NFT("Chan_Stored_t::UnSuspend");
    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
    MCB(i_Suspend_By);
    i_Suspend_By = "";
    MCE(i_Suspend_By);
}


mstring Chan_Stored_t::Mlock_Off()
{
    NFT("Chan_Stored_t::Mlock_Off");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
    RET(i_Mlock_Off);
}

mstring Chan_Stored_t::Mlock_On()
{
    NFT("Chan_Stored_t::Mlock_On");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
    RET(i_Mlock_On);
}

mstring Chan_Stored_t::Mlock()
{
    NFT("Chan_Stored_t::Mlock");
    mstring Result;
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
    RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
    RLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
    RLOCK4(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
    if(i_Mlock_On != "" || i_Mlock_Key != "" || i_Mlock_Limit)
	Result << "+" << i_Mlock_On;
    if(i_Mlock_Key != "")
	Result << "k";
    if(i_Mlock_Limit)
	Result << "l";
    if(i_Mlock_Off != "")
	Result << "-" << i_Mlock_Off;
    RET(Result);
}


vector<mstring> Chan_Stored_t::Mlock(mstring source, mstring mode)
{
    FT("Chan_Stored_t::Mlock", (source, mode));

    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
    WLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
    WLOCK4(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
    MCB(i_Mlock_On);
    CB(1, i_Mlock_Off);
    CB(2, i_Mlock_Key);
    CB(3, i_Mlock_Limit);
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
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else if (Parent->server.proto.ChanModeArg().Contains(change[i]))
	{
	    switch (change[i])
	    {
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
			output.Format(Parent->getMessage(source, "ERR_SYNTAX/WHOLENUMBER").c_str());
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
		fwdargs++;
	    }
	}
    }

    add = true;
    for (i=0; i<change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Parent->server.proto.ChanModeArg().Contains(change[i]))
		{
		    if (!i_Mlock_On.Contains(change[i]))
			i_Mlock_On += change[i];
		    if (i_Mlock_Off.Contains(change[i]))
			i_Mlock_Off.Remove((mstring) change[i]);
		}
	    }
	    else
	    {
		// Nothing with arguments unless its k or l
		if (!Parent->server.proto.ChanModeArg().Contains(change[i]) ||
		    ((change[i]=='k' && !ignorek) || (change[i]=='l' && !ignorel)))
		{
		    if (!i_Mlock_Off.Contains(change[i]))
			i_Mlock_Off += change[i];
		    if (i_Mlock_On.Contains(change[i]))
			i_Mlock_On.Remove((mstring) change[i]);
		}
	    }
	}
    }

    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_Off"));
    RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_On"));
    mstring locked = Parent->chanserv.LCK_MLock() +
	"+" + l_Mlock_On + "-" + l_Mlock_Off;
    mstring override_on;
    mstring override_off;
    mstring forced_on;
    mstring forced_off;
    add = true;
    for (i=0; i<locked.size(); i++)
    {
	if (locked[i] == '+')
	{
	    add = true;
	}
	else if (locked[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
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
	    }
	    else
	    {
		if (locked[i] == 'k')
		    i_Mlock_Key = "";
		else if (locked[i] == 'l')
		    i_Mlock_Limit = 0;

		if (locked[i] == 'k' || locked[i] == 'l' ||
		    !Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
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
	    }
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
    if (i_Mlock_On != "" || i_Mlock_Key != "" || i_Mlock_Limit || i_Mlock_Off != "")
    {
	mstring modes;
	if (i_Mlock_On != "" || i_Mlock_Key != "" || i_Mlock_Limit)
	    modes << "+" << i_Mlock_On;
	if (i_Mlock_Key != "")
	    modes << "k";
	if (i_Mlock_Limit)
	    modes << "l";
	if (i_Mlock_Off != "")
	    modes << "-" << i_Mlock_Off;

	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/MLOCK").c_str(), i_Name.c_str(), modes.c_str());

	if (i_Mlock_Key != "")
	    modes << " " << i_Mlock_Key;
	if (i_Mlock_Limit)
	    modes << " " << i_Mlock_Limit;

	output = "";
	output.Format(Parent->getMessage(source, "CS_COMMAND/MLOCK_SET").c_str(),
	    i_Name.c_str(), modes.c_str());
	retval.push_back(output);
	
    }
    else
    {	
	output = "";
	output.Format(Parent->getMessage(source, "CS_COMMAND/MLOCK_UNSET").c_str(),
	    i_Name.c_str());
	retval.push_back(output);

	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/MLOCK").c_str(), i_Name.c_str());
    }
    CE(1, i_Mlock_Off);
    CE(2, i_Mlock_Key);
    CE(3, i_Mlock_Limit);
    MCE(i_Mlock_On);
    NRET(vector<mstring>, retval);
}


mstring Chan_Stored_t::L_Mlock()
{
    NFT("Chan_Stored_t::L_Mlock");
    mstring Result;
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_Off"));
    RLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_On"));
    mstring mode_on = l_Mlock_On;
    mstring mode_off = l_Mlock_Off;
    mstring locked = Parent->chanserv.LCK_MLock();
    bool add = true;
    unsigned int i;
    for (i=0; i<locked.size(); i++)
    {
	if (locked[i] == '+')
	{
	    add = true;
	}
	else if (locked[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (!mode_on.Contains(locked[i]))
			mode_on += locked[i];
		    if (mode_off.Contains(locked[i]))
			mode_off.Remove((mstring) locked[i]);
		}
	    }
	    else
	    {
		if (locked[i] == 'k' || locked[i] == 'l' ||
		    !Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (!mode_off.Contains(locked[i]))
			mode_off += locked[i];
		    if (mode_on.Contains(locked[i]))
			mode_on.Remove((mstring) locked[i]);
		}
	    }
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

    WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_Off"));
    WLOCK2(("ChanServ", "stored", i_Name.LowerCase(), "l_Mlock_On"));
    MCB(l_Mlock_Off);
    CB(1, l_Mlock_On);
    l_Mlock_On = l_Mlock_Off = "";
    vector<mstring> retval;
    mstring output, change = mode.ExtractWord(1, ": ");
    bool add = true;
    unsigned int i;

    add = true;
    for (i=0; i<change.size(); i++)
    {
	if (change[i] == '+')
	{
	    add = true;
	}
	else if (change[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Parent->server.proto.ChanModeArg().Contains(change[i]))
		{
		    if (!l_Mlock_On.Contains(change[i]))
			l_Mlock_On += change[i];
		    if (l_Mlock_Off.Contains(change[i]))
			l_Mlock_Off.Remove((mstring) change[i]);
		}
	    }
	    else
	    {
		if (change[i] == 'l' || change[i] == 'k' ||
		    !Parent->server.proto.ChanModeArg().Contains(change[i]))
		{
		    if (!l_Mlock_Off.Contains(change[i]))
			l_Mlock_Off += change[i];
		    if (l_Mlock_On.Contains(change[i]))
			l_Mlock_On.Remove((mstring) change[i]);
		}
	    }
	}
    }

    mstring locked = Parent->chanserv.LCK_MLock();
    mstring override_on;
    mstring override_off;
    add = true;
    for (i=0; i<locked.size(); i++)
    {
	if (locked[i] == '+')
	{
	    add = true;
	}
	else if (locked[i] == '-')
	{
	    add = false;
	}
	else
	{
	    if (add)
	    {
		if (!Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (l_Mlock_Off.Contains(locked[i]))
		    {
			override_off += locked[i];
			l_Mlock_Off.Remove((mstring) locked[i]);
		    }
		}
	    }
	    else
	    {
		if (locked[i] == 'l' || locked[i] == 'k' ||
		    !Parent->server.proto.ChanModeArg().Contains(locked[i]))
		{
		    if (l_Mlock_On.Contains(locked[i]))
		    {
			override_on += locked[i];
			l_Mlock_On.Remove((mstring) locked[i]);
		    }
		}
	    }
	}
    }

    { WLOCK3(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Off"));
    WLOCK4(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_On"));
    CB(2, i_Mlock_Off);
    CB(3, i_Mlock_On);
    // Have to change the REAL mlock
    for (i=0; i<l_Mlock_On.size(); i++)
    {
	if (!i_Mlock_On.Contains(l_Mlock_On[i]))
	{
	    i_Mlock_On += l_Mlock_On[i];
	}
	if (i_Mlock_Off.Contains(l_Mlock_On[i]))
	{
	    i_Mlock_Off.Remove((mstring) l_Mlock_On[i]);
	}
    }

    for (i=0; i<l_Mlock_Off.size(); i++)
    {
	if (l_Mlock_Off[i] == 'k')
	{
	    WLOCK5(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
	    i_Mlock_Key = "";
	}
	else if (l_Mlock_Off[i] == 'l')
	{
	    WLOCK5(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
	    i_Mlock_Limit = 0;
	}
	if (!i_Mlock_Off.Contains(l_Mlock_Off[i]))
	{
	    i_Mlock_Off += l_Mlock_Off[i];
	}
	if (i_Mlock_On.Contains(l_Mlock_Off[i]))
	{
	    i_Mlock_On.Remove((mstring) l_Mlock_Off[i]);
	}
    }
    CE(2, i_Mlock_Off);
    CE(3, i_Mlock_Off);
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
	if (Parent->chanserv.IsLive(i_Name))
	    Parent->chanserv.live[i_Name.LowerCase()].SendMode(
		"+" + i_Mlock_On + "-" + i_Mlock_Off);

	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/MLOCK").c_str(), i_Name.c_str(),
		((l_Mlock_On  != "" ? ("+" + l_Mlock_On )  : mstring("")) +
		(l_Mlock_Off != "" ? ("-" + l_Mlock_Off)  : mstring(""))).c_str());
    }
    else
    {
	output = "";
	output.Format(Parent->getMessage(source, "CS_COMMAND/MLOCK_UNLOCK").c_str(),
	    i_Name.c_str());
	retval.push_back(output);
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/MLOCK").c_str(), i_Name.c_str());
    }
    CE(1, l_Mlock_On);
    MCE(l_Mlock_Off);
    NRET(vector<mstring>, retval);
}

mstring Chan_Stored_t::Mlock_Key()
{
    NFT("Chan_Stored_t::Mlock_Key");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Key"));
    RET(i_Mlock_Key);
}

unsigned int Chan_Stored_t::Mlock_Limit()
{
    NFT("Chan_Stored_t::Mlock_Limit");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Mlock_Limit"));
    RET(i_Mlock_Limit);
}

mstring Chan_Stored_t::Last_Topic()
{
    NFT("Chan_Stored_t::i_Topic");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic"));
    RET(i_Topic);
}

mstring Chan_Stored_t::Last_Topic_Setter()
{
    NFT("Chan_Stored_t::i_Topic_Setter");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Setter"));
    RET(i_Topic_Setter);
}

mDateTime Chan_Stored_t::Last_Topic_Set_Time()
{
    NFT("Chan_Stored_t::i_Topic_Set_Time");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topic_Set_Time"));
    RET(i_Topic_Set_Time);
}

void Chan_Stored_t::Bantime(unsigned long in)
{
    FT("Chan_Stored_t::Bantime", (in));
    if (!L_Bantime())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Bantime"));
	MCB(i_Bantime);
	i_Bantime = in;
	MCE(i_Bantime);
    }
}


unsigned long Chan_Stored_t::Bantime()
{
    NFT("Chan_Stored_t::Bantime");
    if (!Parent->chanserv.LCK_Bantime())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Bantime"));
	RET(i_Bantime);
    }
    RET(Parent->chanserv.DEF_Bantime());
}


void Chan_Stored_t::L_Bantime(bool in)
{
    FT("Chan_Stored_t::L_Bantime", (in));
    if (!Parent->chanserv.LCK_Bantime())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Bantime"));
	MCB(l_Bantime);
	l_Bantime = in;
	MCE(l_Bantime);
    }
}


bool Chan_Stored_t::L_Bantime()
{
    NFT("Chan_Stored_t::L_Bantime");
    if (!Parent->chanserv.LCK_Bantime())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Bantime"));
	RET(l_Bantime);
    }
    RET(true);
}


void Chan_Stored_t::Parttime(unsigned long in)
{
    FT("Chan_Stored_t::Parttime", (in));
    if (!L_Parttime())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Parttime"));
	MCB(i_Parttime);
	i_Parttime = in;
	MCE(i_Parttime);
    }
}


unsigned long Chan_Stored_t::Parttime()
{
    NFT("Chan_Stored_t::Parttime");
    if (!Parent->chanserv.LCK_Parttime())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Parttime"));
	RET(i_Parttime);
    }
    RET(Parent->chanserv.DEF_Parttime());
}


void Chan_Stored_t::L_Parttime(bool in)
{
    FT("Chan_Stored_t::L_Parttime", (in));
    if (!Parent->chanserv.LCK_Parttime())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Parttime"));
	MCB(l_Parttime);
	l_Parttime = in;
	MCE(l_Parttime);
    }
}


bool Chan_Stored_t::L_Parttime()
{
    NFT("Chan_Stored_t::L_Parttime");
    if (!Parent->chanserv.LCK_Parttime())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Parttime"));
	RET(l_Parttime);
    }
    RET(true);
}


void Chan_Stored_t::Keeptopic(bool in)
{
    FT("Chan_Stored_t::Keeptopic", (in));
    if (!L_Keeptopic())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Keeptopic"));
	MCB(i_Keeptopic);
	i_Keeptopic = in;
	MCE(i_Keeptopic);
    }
}


bool Chan_Stored_t::Keeptopic()
{
    NFT("Chan_Stored_t::Keeptopic");
    if (!Parent->chanserv.LCK_Keeptopic())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Keeptopic"));
	RET(i_Keeptopic);
    }
    RET(Parent->chanserv.DEF_Keeptopic());
}


void Chan_Stored_t::L_Keeptopic(bool in)
{
    FT("Chan_Stored_t::L_Keeptopic", (in));
    if (!Parent->chanserv.LCK_Keeptopic())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Keeptopic"));
	MCB(l_Keeptopic);
	l_Keeptopic = in;
	MCE(l_Keeptopic);
    }
}


bool Chan_Stored_t::L_Keeptopic()
{
    NFT("Chan_Stored_t::L_Keeptopic");
    if (!Parent->chanserv.LCK_Keeptopic())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Keeptopic"));
	RET(l_Keeptopic);
    }
    RET(true);
}


void Chan_Stored_t::Topiclock(bool in)
{
    FT("Chan_Stored_t::Topiclock", (in));
    if (!L_Topiclock())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topiclock"));
	MCB(i_Topiclock);
	i_Topiclock = in;
	MCE(i_Topiclock);
    }
}


bool Chan_Stored_t::Topiclock()
{
    NFT("Chan_Stored_t::Topiclock");
    if (!Parent->chanserv.LCK_Topiclock())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Topiclock"));
	RET(i_Topiclock);
    }
    RET(Parent->chanserv.DEF_Topiclock());
}


void Chan_Stored_t::L_Topiclock(bool in)
{
    FT("Chan_Stored_t::L_Topiclock", (in));
    if (!Parent->chanserv.LCK_Topiclock())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Topiclock"));
	MCB(l_Topiclock);
	l_Topiclock = in;
	MCE(l_Topiclock);
    }
}


bool Chan_Stored_t::L_Topiclock()
{
    NFT("Chan_Stored_t::L_Topiclock");
    if (!Parent->chanserv.LCK_Topiclock())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Topiclock"));
	RET(l_Topiclock);
    }
    RET(true);
}


void Chan_Stored_t::Private(bool in)
{
    FT("Chan_Stored_t::Private", (in));
    if (!L_Private())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Private"));
	MCB(i_Private);
	i_Private = in;
	MCE(i_Private);
    }
}


bool Chan_Stored_t::Private()
{
    NFT("Chan_Stored_t::Private");
    if (!Parent->chanserv.LCK_Private())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Private"));
	RET(i_Private);
    }
    RET(Parent->chanserv.DEF_Private());
}


void Chan_Stored_t::L_Private(bool in)
{
    FT("Chan_Stored_t::L_Private", (in));
    if (!Parent->chanserv.LCK_Private())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Private"));
	MCB(l_Private);
	l_Private = in;
	MCE(l_Private);
    }
}


bool Chan_Stored_t::L_Private()
{
    NFT("Chan_Stored_t::L_Private");
    if (!Parent->chanserv.LCK_Private())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Private"));
	RET(l_Private);
    }
    RET(true);
}


void Chan_Stored_t::Secureops(bool in)
{
    FT("Chan_Stored_t::Secureops", (in));
    if (!L_Secureops())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Secureops"));
	MCB(i_Secureops);
	i_Secureops = in;
	MCE(i_Secureops);
    }
}


bool Chan_Stored_t::Secureops()
{
    NFT("Chan_Stored_t::Secureops");
    if (!Parent->chanserv.LCK_Secureops())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Secureops"));
	RET(i_Secureops);
    }
    RET(Parent->chanserv.DEF_Secureops());
}


void Chan_Stored_t::L_Secureops(bool in)
{
    FT("Chan_Stored_t::L_Secureops", (in));
    if (!Parent->chanserv.LCK_Secureops())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Secureops"));
	MCB(l_Secureops);
	l_Secureops = in;
	MCE(l_Secureops);
    }
}


bool Chan_Stored_t::L_Secureops()
{
    NFT("Chan_Stored_t::L_Secureops");
    if (!Parent->chanserv.LCK_Secureops())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Secureops"));
	RET(l_Secureops);
    }
    RET(true);
}


void Chan_Stored_t::Secure(bool in)
{
    FT("Chan_Stored_t::Secure", (in));
    if (!L_Secure())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Secure"));
	MCB(i_Secure);
	i_Secure = in;
	MCE(i_Secure);
    }
}


bool Chan_Stored_t::Secure()
{
    NFT("Chan_Stored_t::Secure");
    if (!Parent->chanserv.LCK_Secure())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Secure"));
	RET(i_Secure);
    }
    RET(Parent->chanserv.DEF_Secure());
}


void Chan_Stored_t::L_Secure(bool in)
{
    FT("Chan_Stored_t::L_Secure", (in));
    if (!Parent->chanserv.LCK_Secure())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Secure"));
	MCB(l_Secure);
	l_Secure = in;
	MCE(l_Secure);
    }
}


bool Chan_Stored_t::L_Secure()
{
    NFT("Chan_Stored_t::L_Secure");
    if (!Parent->chanserv.LCK_Secure())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Secure"));
	RET(l_Secure);
    }
    RET(true);
}


void Chan_Stored_t::NoExpire(bool in)
{
    FT("Chan_Stored_t::NoExpire", (in));
    if (!L_NoExpire())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_NoExpire"));
	MCB(i_NoExpire);
	i_NoExpire = in;
	MCE(i_NoExpire);
    }
}


bool Chan_Stored_t::NoExpire()
{
    NFT("Chan_Stored_t::NoExpire");
    if (!Parent->chanserv.LCK_NoExpire())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_NoExpire"));
	RET(i_NoExpire);
    }
    RET(Parent->chanserv.DEF_NoExpire());
}


void Chan_Stored_t::L_NoExpire(bool in)
{
    FT("Chan_Stored_t::L_NoExpire", (in));
    if (!Parent->chanserv.LCK_NoExpire())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_NoExpire"));
	MCB(l_NoExpire);
	l_NoExpire = in;
	MCE(l_NoExpire);
    }
}


bool Chan_Stored_t::L_NoExpire()
{
    NFT("Chan_Stored_t::L_NoExpire");
    if (!Parent->chanserv.LCK_NoExpire())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_NoExpire"));
	RET(l_NoExpire);
    }
    RET(true);
}


void Chan_Stored_t::Anarchy(bool in)
{
    FT("Chan_Stored_t::Anarchy", (in));
    if (!L_Anarchy())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Anarchy"));
	MCB(i_Anarchy);
	i_Anarchy = in;
	MCE(i_Anarchy);
    }
}


bool Chan_Stored_t::Anarchy()
{
    NFT("Chan_Stored_t::Anarchy");
    if (!Parent->chanserv.LCK_Anarchy())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Anarchy"));
	RET(i_Anarchy);
    }
    RET(Parent->chanserv.DEF_Anarchy());
}


void Chan_Stored_t::L_Anarchy(bool in)
{
    FT("Chan_Stored_t::L_Anarchy", (in));
    if (!Parent->chanserv.LCK_Anarchy())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Anarchy"));
	MCB(l_Anarchy);
	l_Anarchy = in;
	MCE(l_Anarchy);
    }
}


bool Chan_Stored_t::L_Anarchy()
{
    NFT("Chan_Stored_t::L_Anarchy");
    if (!Parent->chanserv.LCK_Anarchy())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Anarchy"));
	RET(l_Anarchy);
    }
    RET(true);
}


void Chan_Stored_t::KickOnBan(bool in)
{
    FT("Chan_Stored_t::KickOnBan", (in));
    if (!L_KickOnBan())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_KickOnBan"));
	MCB(i_KickOnBan);
	i_KickOnBan = in;
	MCE(i_KickOnBan);
    }
}


bool Chan_Stored_t::KickOnBan()
{
    NFT("Chan_Stored_t::KickOnBan");
    if (!Parent->chanserv.LCK_KickOnBan())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_KickOnBan"));
	RET(i_KickOnBan);
    }
    RET(Parent->chanserv.DEF_KickOnBan());
}


void Chan_Stored_t::L_KickOnBan(bool in)
{
    FT("Chan_Stored_t::L_KickOnBan", (in));
    if (!Parent->chanserv.LCK_KickOnBan())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_KickOnBan"));
	MCB(l_KickOnBan);
	l_KickOnBan = in;
	MCE(l_KickOnBan);
    }
}


bool Chan_Stored_t::L_KickOnBan()
{
    NFT("Chan_Stored_t::L_KickOnBan");
    if (!Parent->chanserv.LCK_KickOnBan())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_KickOnBan"));
	RET(l_KickOnBan);
    }
    RET(true);
}


void Chan_Stored_t::Restricted(bool in)
{
    FT("Chan_Stored_t::Restricted", (in));
    if (!L_Restricted())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Restricted"));
	MCB(i_Restricted);
	i_Restricted = in;
	MCE(i_Restricted);
    }
}


bool Chan_Stored_t::Restricted()
{
    NFT("Chan_Stored_t::Restricted");
    if (!Parent->chanserv.LCK_Restricted())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Restricted"));
	RET(i_Restricted);
    }
    RET(Parent->chanserv.DEF_Restricted());
}


void Chan_Stored_t::L_Restricted(bool in)
{
    FT("Chan_Stored_t::L_Restricted", (in));
    if (!Parent->chanserv.LCK_Restricted())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Restricted"));
	MCB(l_Restricted);
	l_Restricted = in;
	MCE(l_Restricted);
    }
}


bool Chan_Stored_t::L_Restricted()
{
    NFT("Chan_Stored_t::L_Restricted");
    if (!Parent->chanserv.LCK_Restricted())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Restricted"));
	RET(l_Restricted);
    }
    RET(true);
}


void Chan_Stored_t::Join(bool in)
{
    FT("Chan_Stored_t::Join", (in));
    if (!L_Join())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Join"));
	MCB(i_Join);
	i_Join = in;
	MCE(i_Join);
    }
}


bool Chan_Stored_t::Join()
{
    NFT("Chan_Stored_t::Join");
    if (!Parent->chanserv.LCK_Join())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Join"));
	RET(i_Join);
    }
    RET(Parent->chanserv.DEF_Join());
}


void Chan_Stored_t::L_Join(bool in)
{
    FT("Chan_Stored_t::L_Join", (in));
    if (!Parent->chanserv.LCK_Join())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Join"));
	MCB(l_Join);
	l_Join = in;
	MCE(l_Join);
    }
}


bool Chan_Stored_t::L_Join()
{
    NFT("Chan_Stored_t::L_Join");
    if (!Parent->chanserv.LCK_Join())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Join"));
	RET(l_Join);
    }
    RET(true);
}


bool Chan_Stored_t::Revenge(mstring in)
{
    FT("Chan_Stored_t::Revenge", (in));
    if (!L_Revenge())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Revenge"));
	MCB(i_Revenge);
	i_Revenge = in;
	MCE(i_Revenge);
	RET(true);
    }
    RET(false);
}


mstring Chan_Stored_t::Revenge()
{
    NFT("Chan_Stored_t::Revenge");
    if (!Parent->chanserv.LCK_Revenge())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Revenge"));
	RET(i_Revenge);
    }
    RET(Parent->chanserv.DEF_Revenge());
}


void Chan_Stored_t::L_Revenge(bool in)
{
    FT("Chan_Stored_t::L_Revenge", (in));
    if (!Parent->chanserv.LCK_Revenge())
    {
	WLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Revenge"));
	MCB(l_Revenge);
	l_Revenge = in;
	MCE(l_Revenge);
    }
}


bool Chan_Stored_t::L_Revenge()
{
    NFT("Chan_Stored_t::L_Revenge");
    if (!Parent->chanserv.LCK_Revenge())
    {
	RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "l_Revenge"));
	RET(l_Revenge);
    }
    RET(true);
}

bool Chan_Stored_t::Suspended()
{
    NFT("Chan_Stored_t::Suspended");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
    RET(i_Suspend_By != "");
}

mstring Chan_Stored_t::Suspend_By()
{
    NFT("Chan_Stored_t::Suspend_By");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
    RET(i_Suspend_By);
}

mDateTime Chan_Stored_t::Suspend_Time()
{
    NFT("Chan_Stored_t::Suspend_Time");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Suspend_Time"));
    RET(i_Suspend_Time);
}

bool Chan_Stored_t::Forbidden()
{
    NFT("Chan_Stored_t::Forbidden");
    RLOCK(("ChanServ", "stored", i_Name.LowerCase(), "i_Forbidden"));
    RET(i_Forbidden);
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
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
    set<entlist_val_t<long> >::iterator iter = i_Level.end();
    if (!i_Level.empty())
	for (iter=i_Level.begin(); iter!=i_Level.end(); iter++)
	    if (iter->Entry().LowerCase() == entry.LowerCase())
		break;

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
	MCB(i_Access.size());
	tmp = i_Access.insert(entlist_val_t<long>(
				entry, value, nick, modtime));
	MCE(i_Access.size());
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
	MCB(i_Access.size());
	i_Access.erase(Access);
	MCE(i_Access.size());
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
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
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
    long retval = 0;
    if (Parent->nickserv.IsLive(entry))
    {
	nlive = &Parent->nickserv.live[entry.LowerCase()];
    }
    else
    {
	RET(retval);
    }

    if (nlive->IsChanIdentified(i_Name) && !Suspended())
    {
	retval = Parent->chanserv.Level_Max() + 1;
	RET(retval);
    }

    if (Parent->nickserv.IsStored(entry) &&
	Parent->nickserv.stored[entry.LowerCase()].IsOnline())
    {
	realentry = entry.LowerCase();
	if (Parent->nickserv.stored[realentry].Host() != "")
	    realentry = Parent->nickserv.stored[realentry].Host().LowerCase();
    }
    else
    {
	RET(retval);
    }

    if (Suspended())
    {
	if (Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	    Parent->commserv.list[Parent->commserv.SADMIN_Name()].IsOn(realentry))
	{
	    retval = Parent->chanserv.Level_Max() + 1;
	}
	else if (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	    Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(realentry))
	{
	    retval = Level_value("SUPER");
	}
	else if (Parent->commserv.IsList(Parent->commserv.ADMIN_Name()) &&
	    Parent->commserv.list[Parent->commserv.ADMIN_Name()].IsOn(realentry))
	{
	    retval = Level_value("AUTOOP");
	}
	else if (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	    Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(realentry))
	{
	    retval = Level_value("AUTOVOICE");
	}
    }
    else if (Secure() ? nlive->IsIdentified() : 1)
    {
	if (i_Founder.LowerCase() == realentry.LowerCase())
	{
	    retval = Parent->chanserv.Level_Max() + 1;
	}
	else
	{
	    retval = Access_value(realentry);
	}
    }
    RET(retval);
}

bool Chan_Stored_t::GetAccess(mstring entry, mstring type)
{
    FT("Chan_Stored_t::GetAccess", (entry, type));

    bool retval = (GetAccess(entry) >= Level_value(type));
    RET(retval);
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
	MCB(i_Akick.size());
	tmp = i_Akick.insert(entlist_val_t<mstring>(
				entry, value, nick, modtime));
	MCE(i_Akick.size());
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
    MCB(i_Akick.size());
    bool retval = Akick_insert(entry, Parent->chanserv.DEF_Akick_Reason(), nick, modtime);
    MCE(i_Akick.size());
    RET(retval);
}

bool Chan_Stored_t::Akick_erase()
{
    NFT("Chan_Stored_t::Akick_erase");

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    if (Akick != i_Akick.end())
    {
	MCB(i_Akick.size());
	i_Akick.erase(Akick);
	MCE(i_Akick.size());
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
    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
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
	MCB(i_Greet.size());
	i_Greet.push_back(entlist_t(entry, nick, modtime));
	MCE(i_Greet.size());
	Greet = i_Greet.end();
	Greet--;
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
	MCB(i_Greet.size());
	i_Greet.erase(Greet);
	MCE(i_Greet.size());
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

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
    entlist_i iter = i_Greet.end();
    if (!i_Greet.empty())
	for (iter=i_Greet.begin(); iter!=i_Greet.end(); iter++)
	    if (nick.LowerCase() == iter->Last_Modifier().LowerCase())
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


bool Chan_Stored_t::Message_insert(mstring entry, mstring nick)
{
    FT("Chan_Stored_t::Message_insert", (entry, nick));

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    MCB(i_Message.size());
    i_Message.push_back(entlist_t(entry, nick));
    MCE(i_Message.size());
    Message = i_Message.end(); Message--;
    RET(true);
}


bool Chan_Stored_t::Message_erase()
{
    NFT("Chan_Stored_t::Message_erase");

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    if (Message != i_Message.end())
    {
	MCB(i_Message.size());
	i_Message.erase(Message);
	MCE(i_Message.size());
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

    MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    if (num <= 0 || num > i_Message.size())
    {
	RET(false);
    }

    unsigned int i;
    entlist_i iter = i_Message.end();
    for (iter=i_Message.begin(), i=1; iter!=i_Message.end() && i<num;
							    iter++, i++) ;

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


SXP::Tag Chan_Stored_t::tag_Chan_Stored_t("Chan_Stored_t");
SXP::Tag Chan_Stored_t::tag_Name("Name");
SXP::Tag Chan_Stored_t::tag_RegTime("RegTime");
SXP::Tag Chan_Stored_t::tag_LastUsed("LastUsed");
SXP::Tag Chan_Stored_t::tag_Founder("Founder");
SXP::Tag Chan_Stored_t::tag_CoFounder("CoFounder");
SXP::Tag Chan_Stored_t::tag_Description("Description");
SXP::Tag Chan_Stored_t::tag_Password("Password");
SXP::Tag Chan_Stored_t::tag_Email("Email");
SXP::Tag Chan_Stored_t::tag_URL("URL");
SXP::Tag Chan_Stored_t::tag_Comment("Comment");
SXP::Tag Chan_Stored_t::tag_Topic("Topic");
SXP::Tag Chan_Stored_t::tag_Topic_Setter("Topic_Setter");
SXP::Tag Chan_Stored_t::tag_Topic_Set_Time("Topic_Set_Time");
SXP::Tag Chan_Stored_t::tag_set_Mlock_On("SET_Mlock_On");
SXP::Tag Chan_Stored_t::tag_set_Mlock_Off("SET_Mlock_Off");
SXP::Tag Chan_Stored_t::tag_set_Mlock_Key("SET_Mlock_Key");
SXP::Tag Chan_Stored_t::tag_set_Mlock_Limit("SET_Mlock_Limit");
SXP::Tag Chan_Stored_t::tag_set_Bantime("SET_BanTime");
SXP::Tag Chan_Stored_t::tag_set_Parttime("SET_PartTime");
SXP::Tag Chan_Stored_t::tag_set_KeepTopic("SET_KeepTopic");
SXP::Tag Chan_Stored_t::tag_set_TopicLock("SET_TopicLock");
SXP::Tag Chan_Stored_t::tag_set_Private("SET_Private");
SXP::Tag Chan_Stored_t::tag_set_SecureOps("SET_SecureOps");
SXP::Tag Chan_Stored_t::tag_set_Secure("SET_Secure");
SXP::Tag Chan_Stored_t::tag_set_NoExpire("SET_NoExpire");
SXP::Tag Chan_Stored_t::tag_set_Anarchy("SET_Anarchy");
SXP::Tag Chan_Stored_t::tag_set_KickOnBan("SET_KickOnBan");
SXP::Tag Chan_Stored_t::tag_set_Restricted("SET_Restricted");
SXP::Tag Chan_Stored_t::tag_set_Join("SET_Join");
SXP::Tag Chan_Stored_t::tag_set_Revenge("SET_Revenge");
SXP::Tag Chan_Stored_t::tag_Forbidden("Forbidden");
SXP::Tag Chan_Stored_t::tag_lock_Mlock_On("LOCK_Mlock_On");
SXP::Tag Chan_Stored_t::tag_lock_Mlock_Off("LOCK_Mlock_Off");
SXP::Tag Chan_Stored_t::tag_lock_Bantime("LOCK_BanTime");
SXP::Tag Chan_Stored_t::tag_lock_Parttime("LOCK_PartTime");
SXP::Tag Chan_Stored_t::tag_lock_KeepTopic("LOCK_KeepTopic");
SXP::Tag Chan_Stored_t::tag_lock_TopicLock("LOCK_TopicLock");
SXP::Tag Chan_Stored_t::tag_lock_Private("LOCK_Private");
SXP::Tag Chan_Stored_t::tag_lock_SecureOps("LOCK_SecureOps");
SXP::Tag Chan_Stored_t::tag_lock_Secure("LOCK_Secure");
SXP::Tag Chan_Stored_t::tag_lock_NoExpire("LOCK_NoExpire");
SXP::Tag Chan_Stored_t::tag_lock_Anarchy("LOCK_Anarchy");
SXP::Tag Chan_Stored_t::tag_lock_KickOnBan("LOCK_KickOnBan");
SXP::Tag Chan_Stored_t::tag_lock_Restricted("LOCK_Restricted");
SXP::Tag Chan_Stored_t::tag_lock_Join("LOCK_Join");
SXP::Tag Chan_Stored_t::tag_lock_Revenge("LOCK_Revenge");
SXP::Tag Chan_Stored_t::tag_Suspend_By("Suspend_By");
SXP::Tag Chan_Stored_t::tag_Suspend_Time("Suspend_Time");
SXP::Tag Chan_Stored_t::tag_Level("Level");
SXP::Tag Chan_Stored_t::tag_Access("Access");
SXP::Tag Chan_Stored_t::tag_Akick("Akick");
SXP::Tag Chan_Stored_t::tag_Greet("Greet");
SXP::Tag Chan_Stored_t::tag_Message("Message");
SXP::Tag Chan_Stored_t::tag_UserDef("UserDef");

void Chan_Stored_t::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Chan_Stored_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    if( pElement->IsA(tag_Level) )
    {
	entlist_val_t<long> *tmp = new entlist_val_t<long>;
	level_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_Access) )
    {
	entlist_val_t<long> *tmp = new entlist_val_t<long>;
	access_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_Akick) )
    {
	entlist_val_t<mstring> *tmp = new entlist_val_t<mstring>;
	akick_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_Greet) )
    {
	entlist_t *tmp = new entlist_t;
	greet_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_Message) )
    {
	entlist_t *tmp = new entlist_t;
	message_array.push_back(tmp);
	pIn->ReadTo(tmp);
    }

    if( pElement->IsA(tag_UserDef) )
    {
	mstring *tmp = new mstring;
	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void Chan_Stored_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Chan_Stored_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

	if( pElement->IsA(tag_Name) )			pElement->Retrieve(i_Name);
	if( pElement->IsA(tag_RegTime) )		pElement->Retrieve(i_RegTime);
	if( pElement->IsA(tag_LastUsed) )		pElement->Retrieve(i_LastUsed);
	if( pElement->IsA(tag_Founder) )		pElement->Retrieve(i_Founder);
	if( pElement->IsA(tag_CoFounder) )		pElement->Retrieve(i_CoFounder);
	if( pElement->IsA(tag_Description) )		pElement->Retrieve(i_Description);
	if( pElement->IsA(tag_Password) )		pElement->Retrieve(i_Password);
	if( pElement->IsA(tag_Email) )			pElement->Retrieve(i_Email);
	if( pElement->IsA(tag_URL) )			pElement->Retrieve(i_URL);
	if( pElement->IsA(tag_Comment) )		pElement->Retrieve(i_Comment);
	if( pElement->IsA(tag_Topic) )			pElement->Retrieve(i_Topic);
	if( pElement->IsA(tag_Topic_Setter) )		pElement->Retrieve(i_Topic_Setter);
	if( pElement->IsA(tag_Topic_Set_Time) )		pElement->Retrieve(i_Topic_Set_Time);
	if( pElement->IsA(tag_set_Mlock_On) )		pElement->Retrieve(i_Mlock_On);
	if( pElement->IsA(tag_set_Mlock_Off) )		pElement->Retrieve(i_Mlock_Off);
	if( pElement->IsA(tag_set_Mlock_Key) )		pElement->Retrieve(i_Mlock_Key);
	if( pElement->IsA(tag_set_Mlock_Limit) )	pElement->Retrieve(i_Mlock_Limit);
	if( pElement->IsA(tag_set_Bantime) )		pElement->Retrieve(i_Bantime);
	if( pElement->IsA(tag_set_Parttime) )		pElement->Retrieve(i_Parttime);
	if( pElement->IsA(tag_set_KeepTopic) )		pElement->Retrieve(i_Keeptopic);
	if( pElement->IsA(tag_set_TopicLock) )		pElement->Retrieve(i_Topiclock);
	if( pElement->IsA(tag_set_Private) )		pElement->Retrieve(i_Private);
	if( pElement->IsA(tag_set_SecureOps) )		pElement->Retrieve(i_Secureops);
	if( pElement->IsA(tag_set_Secure) )		pElement->Retrieve(i_Secure);
	if( pElement->IsA(tag_set_NoExpire) )		pElement->Retrieve(i_NoExpire);
	if( pElement->IsA(tag_set_Anarchy) )		pElement->Retrieve(i_Anarchy);
	if( pElement->IsA(tag_set_KickOnBan) )		pElement->Retrieve(i_KickOnBan);
	if( pElement->IsA(tag_set_Restricted) )		pElement->Retrieve(i_Restricted);
	if( pElement->IsA(tag_set_Join) )		pElement->Retrieve(i_Join);
	if( pElement->IsA(tag_set_Revenge) )		pElement->Retrieve(i_Revenge);
	if( pElement->IsA(tag_Forbidden) )		pElement->Retrieve(i_Forbidden);
	if( pElement->IsA(tag_lock_Mlock_On) )		pElement->Retrieve(l_Mlock_On);
	if( pElement->IsA(tag_lock_Mlock_Off) )		pElement->Retrieve(l_Mlock_Off);
	if( pElement->IsA(tag_lock_Bantime) )		pElement->Retrieve(l_Bantime);
	if( pElement->IsA(tag_lock_Parttime) )		pElement->Retrieve(l_Parttime);
	if( pElement->IsA(tag_lock_KeepTopic) )		pElement->Retrieve(l_Keeptopic);
	if( pElement->IsA(tag_lock_TopicLock) )		pElement->Retrieve(l_Topiclock);
	if( pElement->IsA(tag_lock_Private) )		pElement->Retrieve(l_Private);
	if( pElement->IsA(tag_lock_SecureOps) )		pElement->Retrieve(l_Secureops);
	if( pElement->IsA(tag_lock_Secure) )		pElement->Retrieve(l_Secure);
	if( pElement->IsA(tag_lock_NoExpire) )		pElement->Retrieve(l_NoExpire);
	if( pElement->IsA(tag_lock_Anarchy) )		pElement->Retrieve(l_Anarchy);
	if( pElement->IsA(tag_lock_KickOnBan) )		pElement->Retrieve(l_KickOnBan);
	if( pElement->IsA(tag_lock_Restricted) )	pElement->Retrieve(l_Restricted);
	if( pElement->IsA(tag_lock_Join) )		pElement->Retrieve(l_Join);
	if( pElement->IsA(tag_lock_Revenge) )		pElement->Retrieve(l_Revenge);
	if( pElement->IsA(tag_Suspend_By) )		pElement->Retrieve(i_Suspend_By);
	if( pElement->IsA(tag_Suspend_Time) )		pElement->Retrieve(i_Suspend_Time);
}

void Chan_Stored_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    set<entlist_val_t<long> >::iterator j;
    set<entlist_val_t<mstring> >::iterator k;
    entlist_i l;

    //TODO: Add your source code here
	pOut->BeginObject(tag_Chan_Stored_t, attribs);

	WLOCK(("ChanServ", "stored", i_Name.LowerCase()));
	pOut->WriteElement(tag_Name, i_Name);
	pOut->WriteElement(tag_RegTime, i_RegTime);
	pOut->WriteElement(tag_LastUsed, i_LastUsed);
	pOut->WriteElement(tag_Founder, i_Founder);
	pOut->WriteElement(tag_CoFounder, i_CoFounder);
	pOut->WriteElement(tag_Description, i_Description);
	pOut->WriteElement(tag_Password, i_Password);
	pOut->WriteElement(tag_Email, i_Email);
	pOut->WriteElement(tag_URL, i_URL);
	pOut->WriteElement(tag_Comment, i_Comment);
	pOut->WriteElement(tag_Topic, i_Topic);
	pOut->WriteElement(tag_Topic_Setter, i_Topic_Setter);
	pOut->WriteElement(tag_Topic_Set_Time, i_Topic_Set_Time);
	pOut->WriteElement(tag_set_Mlock_On, i_Mlock_On);
	pOut->WriteElement(tag_set_Mlock_Off, i_Mlock_Off);
	pOut->WriteElement(tag_set_Mlock_Key, i_Mlock_Key);
	pOut->WriteElement(tag_set_Mlock_Limit, i_Mlock_Limit);
	pOut->WriteElement(tag_set_Bantime, i_Bantime);
	pOut->WriteElement(tag_set_Parttime, i_Parttime);
	pOut->WriteElement(tag_set_KeepTopic, i_Keeptopic);
	pOut->WriteElement(tag_set_TopicLock, i_Topiclock);
	pOut->WriteElement(tag_set_Private, i_Private);
	pOut->WriteElement(tag_set_SecureOps, i_Secureops);
	pOut->WriteElement(tag_set_Secure, i_Secure);
	pOut->WriteElement(tag_set_NoExpire, i_NoExpire);
	pOut->WriteElement(tag_set_Anarchy, i_Anarchy);
	pOut->WriteElement(tag_set_KickOnBan, i_KickOnBan);
	pOut->WriteElement(tag_set_Restricted, i_Restricted);
	pOut->WriteElement(tag_set_Join, i_Join);
	pOut->WriteElement(tag_set_Revenge, i_Revenge);
	pOut->WriteElement(tag_Forbidden, i_Forbidden);
	pOut->WriteElement(tag_lock_Mlock_On, l_Mlock_On);
	pOut->WriteElement(tag_lock_Mlock_Off, l_Mlock_Off);
	pOut->WriteElement(tag_lock_Bantime, l_Bantime);
	pOut->WriteElement(tag_lock_Parttime, l_Parttime);
	pOut->WriteElement(tag_lock_KeepTopic, l_Keeptopic);
	pOut->WriteElement(tag_lock_TopicLock, l_Topiclock);
	pOut->WriteElement(tag_lock_Private, l_Private);
	pOut->WriteElement(tag_lock_SecureOps, l_Secureops);
	pOut->WriteElement(tag_lock_Secure, l_Secure);
	pOut->WriteElement(tag_lock_NoExpire, l_NoExpire);
	pOut->WriteElement(tag_lock_Anarchy, l_Anarchy);
	pOut->WriteElement(tag_lock_KickOnBan, l_KickOnBan);
	pOut->WriteElement(tag_lock_Restricted, l_Restricted);
	pOut->WriteElement(tag_lock_Join, l_Join);
	pOut->WriteElement(tag_lock_Revenge, l_Revenge);
	pOut->WriteElement(tag_Suspend_By, i_Suspend_By);
	pOut->WriteElement(tag_Suspend_Time, i_Suspend_Time);

	{MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
	for(j=i_Level.begin(); j!=i_Level.end(); j++)
	{
	    pOut->BeginObject(tag_Level, attribs);
	    pOut->WriteSubElement((entlist_val_t<long> *) &(*j), attribs);
	    pOut->EndObject(tag_Level);
	}}

	{MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
	for(j=i_Access.begin(); j!=i_Access.end(); j++)
	{
	    pOut->BeginObject(tag_Access, attribs);
	    pOut->WriteSubElement((entlist_val_t<long> *) &(*j), attribs);
	    pOut->EndObject(tag_Access);
	}}

	{MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
	for(k=i_Akick.begin(); k!=i_Akick.end(); k++)
	{
	    pOut->BeginObject(tag_Akick, attribs);
	    pOut->WriteSubElement((entlist_val_t<mstring> *) &(*k), attribs);
	    pOut->EndObject(tag_Akick);
	}}

	{MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
	for(l=i_Greet.begin(); l!=i_Greet.end(); l++)
	{
	    pOut->BeginObject(tag_Greet, attribs);
	    pOut->WriteSubElement(&(*l), attribs);
	    pOut->EndObject(tag_Greet);
	}}

	{MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
	for(l=i_Message.begin(); l!=i_Message.end(); l++)
	{
	    pOut->BeginObject(tag_Message, attribs);
	    pOut->WriteSubElement(&(*l), attribs);
	    pOut->EndObject(tag_Message);
	}}

        map<mstring,mstring>::const_iterator iter;
        for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
        {
            pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
        }

	pOut->EndObject(tag_Chan_Stored_t);
}


size_t Chan_Stored_t::Usage()
{
    size_t retval = 0;

    WLOCK(("ChanServ", "stored", i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_RegTime.Internal());
    retval += sizeof(i_LastUsed.Internal());
    retval += i_Founder.capacity();
    retval += i_CoFounder.capacity();
    retval += i_Description.capacity();
    retval += i_Password.capacity();
    retval += i_Email.capacity();
    retval += i_URL.capacity();
    retval += i_Comment.capacity();
    map<mstring, unsigned int>::iterator i;
    for (i=failed_passwds.begin(); i!=failed_passwds.end(); i++)
    {
	retval += i->first.capacity();
	retval += sizeof(i->second);
    }

    retval += i_Mlock_On.capacity();
    retval += l_Mlock_On.capacity();
    retval += i_Mlock_Off.capacity();
    retval += l_Mlock_Off.capacity();
    retval += i_Mlock_Key.capacity();
    retval += sizeof(i_Mlock_Limit);
    retval += i_Topic.capacity();
    retval += i_Topic_Setter.capacity();
    retval += sizeof(i_Topic_Set_Time.Internal());
    
    retval += sizeof(i_Bantime);
    retval += sizeof(l_Bantime);
    retval += sizeof(i_Parttime);
    retval += sizeof(l_Parttime);
    retval += sizeof(i_Keeptopic);
    retval += sizeof(l_Keeptopic);
    retval += sizeof(i_Topiclock);
    retval += sizeof(l_Topiclock);
    retval += sizeof(i_Private);
    retval += sizeof(l_Private);
    retval += sizeof(i_Secureops);
    retval += sizeof(l_Secureops);
    retval += sizeof(i_Secure);
    retval += sizeof(l_Secure);
    retval += sizeof(i_NoExpire);
    retval += sizeof(l_NoExpire);
    retval += sizeof(i_Anarchy);
    retval += sizeof(l_Anarchy);
    retval += sizeof(i_KickOnBan);
    retval += sizeof(l_KickOnBan);
    retval += sizeof(i_Restricted);
    retval += sizeof(l_Restricted);
    retval += sizeof(i_Join);
    retval += sizeof(l_Join);
    retval += sizeof(i_Forbidden);
    retval += i_Revenge.capacity();
    retval += sizeof(l_Revenge);

    retval += i_Suspend_By.capacity();
    retval += sizeof(i_Suspend_Time.Internal());

    set<entlist_val_t<long> >::iterator j;
    entlist_val_t<long> *jp;
    {MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Level"));
    for (j=i_Level.begin(); j!=i_Level.end(); j++)
    {
	jp = (entlist_val_t<long> *) &(*j);
	retval += jp->Usage();
    }}
    {MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Access"));
    for (j=i_Access.begin(); j!=i_Access.end(); j++)
    {
	jp = (entlist_val_t<long> *) &(*j);
	retval += jp->Usage();
    }}

    set<entlist_val_t<mstring> >::iterator k;
    entlist_val_t<mstring> *kp;
    {MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Akick"));
    for (k=i_Akick.begin(); k!=i_Akick.end(); k++)
    {
	kp = (entlist_val_t<mstring> *) &(*k);
	retval += kp->Usage();
    }}

    list<entlist_t>::iterator l;
    {MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Greet"));
    for (l = i_Greet.begin(); l != i_Greet.end(); l++)
    {
	retval += l->Usage();
    }}
    {MLOCK(("ChanServ", "stored", i_Name.LowerCase(), "Message"));
    for (l = i_Message.begin(); l != i_Message.end(); l++)
    {
	retval += l->Usage();
    }}

    map<mstring,mstring>::iterator m;
    for (m=i_UserDef.begin(); m!=i_UserDef.end(); m++)
    {
	retval += m->first.capacity();
	retval += m->second.capacity();
    }

    return retval;
}

void Chan_Stored_t::DumpB()
{
    MB(0, (i_Name, i_RegTime, i_LastUsed, i_Founder, i_CoFounder,
	i_Description, i_Password, i_Email, i_URL, i_Comment,
	failed_passwds.size(), i_Mlock_On, l_Mlock_On, i_Mlock_Off,
	l_Mlock_Off, i_Mlock_Key));
    MB(16, (i_Mlock_Limit, i_Topic, i_Topic_Setter, i_Topic_Set_Time,
	i_Bantime, l_Bantime, i_Parttime, l_Parttime, i_Keeptopic,
	l_Keeptopic, i_Topiclock, l_Topiclock, i_Private, l_Private,
	i_Secureops, l_Secureops));
    MB(32, (i_Secure, l_Secure, i_NoExpire, l_NoExpire, i_Anarchy,
	l_Anarchy, i_KickOnBan, l_KickOnBan, i_Restricted, l_Restricted,
	i_Join, l_Join, i_Forbidden, i_Revenge, l_Revenge, i_Suspend_By));
    MB(48, (i_Suspend_Time, i_Level.size(), i_Access.size(),
	i_Akick.size(), i_Greet.size(), i_Message.size(), i_UserDef.size()));
}


void Chan_Stored_t::DumpE()
{
    ME(0, (i_Name, i_RegTime, i_LastUsed, i_Founder, i_CoFounder,
	i_Description, i_Password, i_Email, i_URL, i_Comment,
	failed_passwds.size(), i_Mlock_On, l_Mlock_On, i_Mlock_Off,
	l_Mlock_Off, i_Mlock_Key));
    ME(16, (i_Mlock_Limit, i_Topic, i_Topic_Setter, i_Topic_Set_Time,
	i_Bantime, l_Bantime, i_Parttime, l_Parttime, i_Keeptopic,
	l_Keeptopic, i_Topiclock, l_Topiclock, i_Private, l_Private,
	i_Secureops, l_Secureops));
    ME(32, (i_Secure, l_Secure, i_NoExpire, l_NoExpire, i_Anarchy,
	l_Anarchy, i_KickOnBan, l_KickOnBan, i_Restricted, l_Restricted,
	i_Join, l_Join, i_Forbidden, i_Revenge, l_Revenge, i_Suspend_By));
    ME(48, (i_Suspend_Time, i_Level.size(), i_Access.size(),
	i_Akick.size(), i_Greet.size(), i_Message.size(), i_UserDef.size()));
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
	    "MODE*", Parent->commserv.REGD_Name(), ChanServ::do_Mode);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "OP*", Parent->commserv.REGD_Name(), ChanServ::do_Op);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "D*OP*", Parent->commserv.REGD_Name(), ChanServ::do_DeOp);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "VOIC*", Parent->commserv.REGD_Name(), ChanServ::do_Voice);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "D*VOIC*", Parent->commserv.REGD_Name(), ChanServ::do_DeVoice);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "TOPIC*", Parent->commserv.REGD_Name(), ChanServ::do_Topic);
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
	    "SET* ANAR*", Parent->commserv.REGD_Name(), ChanServ::do_set_Anarchy);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* KICK*", Parent->commserv.REGD_Name(), ChanServ::do_set_KickOnBan);
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
	    "LOCK KICK*", Parent->commserv.SOP_Name(), ChanServ::do_lock_KickOnBan);
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
	    "UNLOCK KICK*", Parent->commserv.SOP_Name(), ChanServ::do_unlock_KickOnBan);
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

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "H*LP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "REG*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "DROP", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ID*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "INFO*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LIST", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SUSP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNSUSP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FORB*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GET*PASS*", Parent->commserv.SOP_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "MODE", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "OP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "D*OP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "VOIC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "D*VOIC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "TOPIC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "KICK*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "REM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "USER*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "INV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNBAN*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LIVE*", Parent->commserv.SOP_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *USER*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *OP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *VOICE*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *MODE*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *BAN*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *ALL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* SET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* RESET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* REM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK REM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* REM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* REM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* FOUND*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* COFOUND*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* DESC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* PASS*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* EMAIL", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* URL", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* WWW*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* WEB*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* COMM*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* M*LOCK", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* BAN*TIME", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* PART*TIME", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* KEEP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* TOPIC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* PRIV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* SEC*OP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* SEC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* NOEX*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* ANAR*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* KICK*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* RES*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* *JOIN*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* REV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK M*LOCK", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK BAN*TIME", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK PART*TIME", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK KEEP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK TOPIC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK SEC*OP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK ANAR*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK KICK*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK RES*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK *JOIN*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK REV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK M*LOCK", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK BAN*TIME", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK PART*TIME", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK KEEP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK TOPIC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK SEC*OP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK ANAR*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK KICK*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK RES*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK *JOIN*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK REV*", Parent->commserv.SOP_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CLEAR*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LEV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ACC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "A*KICK", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GREET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "M*S*G*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK *", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK *", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK", Parent->commserv.SOP_Name());
}

bool ChanServ::IsLive(mstring in)
{
    FT("ChanServ::IsLive", (in));
    RLOCK(("ChanServ", "live", in.LowerCase()));
    bool retval = live.find(in.LowerCase())!=live.end();
    RET(retval);
}

bool ChanServ::IsStored(mstring in)
{
    FT("ChanServ::IsStored", (in));
    RLOCK(("ChanServ", "stored", in.LowerCase()));
    bool retval = stored.find(in.LowerCase())!=stored.end();
    RET(retval);
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
    else if (msgtype == "PRIVMSG" &&
	!Parent->commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }


    mThread::ReAttach(tt_mBase);
}

void ChanServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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

    if (!Parent->nickserv.live[source.LowerCase()].HasMode("o") &&
	Parent->nickserv.live[source.LowerCase()].LastChanReg().SecondsSince() <
    		Parent->chanserv.Delay())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOTYET"),
		message.c_str(), ToHumanTime(Parent->chanserv.Delay() -
		Parent->nickserv.live[source.LowerCase()].LastChanReg().SecondsSince(),
		source).c_str());
	return;
    }

    if (password.Len() < 5 || password.CmpNoCase(channel.After(channel[0u])) == 0 ||
	password.CmpNoCase(channel) == 0 || password.CmpNoCase(source) == 0)
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

    if (Parent->chanserv.Max_Per_Nick() &&
	Parent->nickserv.stored[founder.LowerCase()].MyChannels() >=
	Parent->chanserv.Max_Per_Nick())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/TOOMANY"));
	return;
    }

    Parent->nickserv.live[source.LowerCase()].SetLastChanReg();
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
    Log(LM_INFO, Parent->getLogMessage("CHANSERV/REGISTER"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	channel.c_str());
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

    if (!((Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	 Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source)) ||
	Parent->nickserv.live[source.LowerCase()].IsChanIdentified(channel)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NEED_CHAN_IDENT"),
		message.c_str(), mynick.c_str(), channel.c_str());
	return;
    }

    mstring founder = Parent->chanserv.stored[channel.LowerCase()].Founder();
    { WLOCK(("ChanServ", "stored"));
    Parent->chanserv.stored.erase(channel.LowerCase());
    }
    Parent->nickserv.live[source.LowerCase()].UnChanIdentify(channel);
    Parent->chanserv.stats.i_Drop++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/DROPPED"),
	    channel.c_str());
    Log(LM_INFO, Parent->getLogMessage("CHANSERV/DROP"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	channel.c_str(), founder.c_str());
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
    if (!wasident &&
	Parent->nickserv.live[source.LowerCase()].IsChanIdentified(channel))
    {
	Parent->chanserv.stats.i_Identify++;
	Log(LM_INFO, Parent->getLogMessage("CHANSERV/IDENTIFY"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		channel.c_str());
    }
    if (output != "")
	::send(mynick, source, output, channel.c_str());
}

void ChanServ::do_Info(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Info", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    mstring output;

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
		ToHumanTime(chan->Bantime(), source).c_str());
    if (chan->Parttime())
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/PARTTIME"),
		ToHumanTime(chan->Parttime(), source).c_str());

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

    if (chan->KickOnBan())
    {
	if (output != "")
	    output << ", ";
	if (chan->L_KickOnBan())
	    output << IRC_Bold;
	output << Parent->getMessage(source, "CS_SET/KICKONBAN");
	if (chan->L_KickOnBan())
	    output << IRC_Off;
    }

    if (output != "")
	::send(mynick, source, Parent->getMessage(source, "CS_INFO/OPTIONS"),
	    output.c_str());
    if (Parent->servmsg.ShowSync())
	::send(mynick, source, Parent->getMessage("MISC/SYNC"),
			Parent->events->SyncTime(source).c_str());
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

    bool isoper = (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source));
    bool issop = (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
		Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source));
    for (iter = Parent->chanserv.stored.begin(), i=0, count = 0;
			iter != Parent->chanserv.stored.end(); iter++)
    {
	if (iter->second.Name().LowerCase().Matches(mask))
	{
	    if (i < listsize && (!iter->second.Private() || isoper))
	    {
		if (issop)
		{
		    if (message.Contains("NOEXP") && !iter->second.NoExpire())
			continue;
		    if (message.Contains("FORBID") && !iter->second.Forbidden())
			continue;
		    if (message.Contains("SUSP") && !iter->second.Suspended())
			continue;
		}

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
    Parent->chanserv.stats.i_Suspend++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SUSPENDED"),
	    channel.c_str());
    Log(LM_NOTICE, Parent->getLogMessage("CHANSERV/SUSPEND"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	channel.c_str(), reason.c_str());
    if (Parent->chanserv.IsLive(channel))
    {
	Parent->server.TOPIC(mynick, mynick, channel, "[" + IRC_Bold +
			Parent->getMessage("VALS/SUSPENDED") +
			IRC_Off + "] " + reason + " [" + IRC_Bold +
			Parent->getMessage("VALS/SUSPENDED") + IRC_Off + "]",
			Parent->chanserv.live[channel.LowerCase()].Topic_Set_Time() -
				(double) (1.0 / (60.0 * 60.0 * 24.0)));

	Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
	Chan_Stored_t *cstored = &Parent->chanserv.stored[channel.LowerCase()];
	clive->SendMode("-" + clive->Mode() + " " + clive->Key());
	if (cstored->Mlock() != "")
	    clive->SendMode(cstored->Mlock() + " " + cstored->Mlock_Key() + " " +
		cstored->Mlock_Limit());

	for (unsigned int i=0; i < clive->Users(); i++)
	{
	    
	    if (clive->IsOp(clive->User(i)) &&
		!(cstored->GetAccess(clive->User(i), "AUTOOP") ||
		cstored->GetAccess(clive->User(i), "CMDOP")))
	    {
		clive->SendMode("-o " + clive->User(i));
	    }
	    if (clive->IsVoice(clive->User(i)) &&
		!(cstored->GetAccess(clive->User(i), "AUTOVOICE") ||
		cstored->GetAccess(clive->User(i), "CMDVOICE")))
	    {
		clive->SendMode("-v " + clive->User(i));
	    }
	}
    }
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
    Log(LM_NOTICE, Parent->getLogMessage("CHANSERV/UNSUSPEND"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
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
    Log(LM_NOTICE, Parent->getLogMessage("CHANSERV/FORBID"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	channel.c_str());

    if (Parent->chanserv.IsLive(channel))
    {

	unsigned int i;
	vector<mstring> kickees;
	for (i=0; i<Parent->chanserv.live[channel.LowerCase()].Users(); i++)
	{
		kickees.push_back(Parent->chanserv.live[channel.LowerCase()].User(i));
	}
	for (i=0; i<kickees.size(); i++)
	{
	    mstring reason;
	    reason.Format(Parent->getMessage(kickees[i], "CS_STATUS/ISFORBIDDEN"),
		channel.c_str());
	    Parent->server.KICK(Parent->chanserv.FirstName(), kickees[i],
		channel, reason);
	}
    }
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
	Log(LM_CRITICAL, Parent->getLogMessage("ERROR/FOUNDER_NOTREGD"),
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
    Log(LM_NOTICE, Parent->getLogMessage("CHANSERV/GETPASS"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	chan->Name().c_str(), Parent->getSname(chan->Founder()).c_str());
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
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Mode()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Mode()].IsOn(source))))
    {
	mstring modes = params.After(" ", 2);
	clive->SendMode(modes);
	Parent->chanserv.stats.i_Mode++;
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/MODE"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		modes.c_str(), channel.c_str());
	return;
    }
    else if (cstored->GetAccess(source, "VIEW") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	 Parent->commserv.list[Parent->commserv.OVR_View()].IsOn(source)))
    {
	mstring output;
	output << clive->Name() << ": +" << clive->Mode();
	if (clive->Key() != "")
	    output << "k";
	if (clive->Limit())
	    output << "l";
	if (clive->Key() != "")
	    output << " " << clive->Key();
	if (clive->Limit())
	    output << " " << clive->Limit();

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
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Op()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Op()].IsOn(source))))
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/OP"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), channel.c_str());
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
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Op()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Op()].IsOn(source))))
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/DEOP"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), channel.c_str());
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
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Voice()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Voice()].IsOn(source))))
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/VOICE"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), channel.c_str());
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
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Voice()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Voice()].IsOn(source))))
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/DEVOICE"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), channel.c_str());
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
    chan->SetTopic(mynick, source, topic);
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/TOPIC"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	channel.c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/KICK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), channel.c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/ANONKICK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), channel.c_str());
}

void ChanServ::do_Users(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_Users", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
	user = Parent->getLname(chan->User(i));
	if (output.size() + user.Len() > Parent->server.proto.MaxLine())
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
	user = Parent->getLname(chan->Squit(i));
	if (output.size() + user.Len() > Parent->server.proto.MaxLine())
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
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Invite()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Invite()].IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/INVITE"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), channel.c_str());
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
	(Parent->commserv.IsList(Parent->commserv.OVR_CS_Unban()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Unban()].IsOn(source))))
    {
	target = params.ExtractWord(3, " ");
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
	if (nlive->Mask(Nick_Live_t::N_U_P_H).Matches(clive->Ban(i).LowerCase()) ||
	    nlive->AltMask(Nick_Live_t::N_U_P_H).Matches(clive->Ban(i).LowerCase()))
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
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNBAN"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
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

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Clear()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
    unsigned int i;
    vector<mstring> kickees;
    for (i=0; i<clive->Users(); i++)
    {
	mstring user = clive->User(i);
	if (user.CmpNoCase(source) != 0 && Parent->nickserv.IsLive(user) &&
		!Parent->nickserv.live[user.LowerCase()].IsServices())
	    kickees.push_back(clive->User(i));
    }
    for (i=0; i<kickees.size(); i++)
    {
	mstring output;
	output.Format(Parent->getMessage(kickees[i], "CS_COMMAND/CLEAR").c_str(),
		message.c_str(), source.c_str(), channel.c_str());
	Parent->server.KICK(mynick, kickees[i], channel, output);
    }
    Parent->chanserv.stats.i_Clear++;
    Log(LM_INFO, Parent->getLogMessage("CHANSERV/COMMAND"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	message.c_str(), channel.c_str());
}

void ChanServ::do_clear_Ops(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_Ops", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Clear()].IsOn(source)))
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
    {
	Parent->chanserv.stats.i_Clear++;
	Log(LM_INFO, Parent->getLogMessage("CHANSERV/COMMAND"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		message.c_str(), channel.c_str());
    }
}

void ChanServ::do_clear_Voices(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_Voices", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Clear()].IsOn(source)))
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
	Log(LM_INFO, Parent->getLogMessage("CHANSERV/COMMAND"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		message.c_str(), channel.c_str());
    }
}

void ChanServ::do_clear_Modes(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_Modes", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Clear()].IsOn(source)))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOACCESS"));
	return;
    }

    Chan_Live_t *clive = &Parent->chanserv.live[channel.LowerCase()];
    unsigned int i;
    mstring mode;

    mode << "-" << clive->Mode();
    if (clive->Limit())
	mode << "l";
    if (clive->Key() != "")
	mode << "k " << clive->Key();
    clive->SendMode(mode);
    if (cstored->Mlock_On() != "")
    {
	mode = "+" + cstored->Mlock_On();
	if (cstored->Mlock_Limit())
	    mode << "l";
	if (cstored->Mlock_Key() != "")
	    mode << "l";
	if (cstored->Mlock_Limit())
	    mode << " " << cstored->Mlock_Limit();
	if (cstored->Mlock_Key() != "")
	    mode << " " << cstored->Mlock_Key();
	
	clive->SendMode(mode);
    }
    if (!message.After(" ").Matches("*ALL*"))
    {
	for (i=0; i<clive->Ops(); i++)
	{
	    ::send(mynick, clive->Op(i), Parent->getMessage(clive->Op(i), "CS_COMMAND/CLEAR"),
		    message.c_str(), source.c_str(), channel.c_str());
	}
	Parent->chanserv.stats.i_Clear++;
	Log(LM_INFO, Parent->getLogMessage("CHANSERV/COMMAND"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		message.c_str(), channel.c_str());
    }
}

void ChanServ::do_clear_Bans(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_Bans", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Clear()].IsOn(source)))
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
	Log(LM_INFO, Parent->getLogMessage("CHANSERV/COMMAND"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		message.c_str(), channel.c_str());
    }
}

void ChanServ::do_clear_All(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_clear_All", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
	!(Parent->commserv.IsList(Parent->commserv.OVR_CS_Clear()) &&
	 Parent->commserv.list[Parent->commserv.OVR_CS_Clear()].IsOn(source)))
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
    Log(LM_INFO, Parent->getLogMessage("CHANSERV/COMMAND"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	message.c_str(), channel.c_str());
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
        ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/WHOLENUMBER"));
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
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LEVEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		cstored->Level->Entry().c_str(), cstored->Level->Value(),
		channel.c_str());
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
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LEVEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		cstored->Level->Entry().c_str(), cstored->Level->Value(),
		channel.c_str());
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

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
    long myaccess = cstored->GetAccess(source);
    bool haveset = cstored->GetAccess(source, "SET");
    if (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name().UpperCase()].IsIn(source))
	haveset = true;

    if (haveset)
    {
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LEVEL_HEAD"),
		channel.c_str());
    }

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
	else if(cstored->Level->Entry() != "AUTODEOP" &&
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
    if (Parent->nickserv.stored[who.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
		who.c_str());
	return;
    }

    if (!level.IsNumber() || level.Contains("."))
    {
        ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/WHOLENUMBER"));
	return;
    }
    long num = atol(level.c_str());

    if (num == 0)
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/NOTZERO"));
	return;
    }

    if (num < Parent->chanserv.Level_Min() ||
	num > Parent->chanserv.Level_Max())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
		Parent->chanserv.Level_Min(),
		Parent->chanserv.Level_Max());
	return;
    }

    if (num >= cstored->GetAccess(source))
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ONLYBELOW"));
	return;
    }

    MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Access"));
    if (cstored->Access_find(who))
    {
	mstring entry = cstored->Access->Entry();
	if (cstored->Access->Value() >= cstored->GetAccess(source))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/HIGHERACCESS"),
			entry.c_str(), channel.c_str());
	    return;
	}

	cstored->Access_erase();
	cstored->Access_insert(entry, num, source);
	Parent->chanserv.stats.i_Access++;
	::send(mynick, source, Parent->getMessage(source, "LIST/CHANGE2_LEVEL"),
		    cstored->Access->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str(),
		    num);
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/ACCESS_CHANGE"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		cstored->Access->Entry().c_str(), channel.c_str(), num);
    }
    else
    {
	cstored->Access_insert(who, num, source);
	Parent->chanserv.stats.i_Access++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD2_LEVEL"),
		    cstored->Access->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str(),
		    num);
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/ACCESS_ADD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		cstored->Access->Entry().c_str(), channel.c_str(), num);
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
	if (who.Contains("."))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_STYNTAX/WHOLENUMBER"));
	    return;
	}
	unsigned int i, num = atoi(who);
	if (num < 1 || num > cstored->Access_size())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
		1, cstored->Access_size());
	    return;
	}

	MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Access"));
	for (i=1, cstored->Access = cstored->Access_begin();
				i<num && cstored->Access != cstored->Access_end();
				i++, cstored->Access++) ;
	if (cstored->Access != cstored->Access_end())
	{
	    if (cstored->Access->Value() >= cstored->GetAccess(source))
	    {
		::send(mynick, source, Parent->getMessage(source, "CS_STATUS/HIGHERACCESS"),
			cstored->Access->Entry().c_str(), channel.c_str());
		return;
	    }

	    Parent->chanserv.stats.i_Access++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL2"),
		    cstored->Access->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str());
	    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/ACCESS_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		cstored->Access->Entry().c_str(), channel.c_str());
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
	    if (cstored->Access->Value() >= cstored->GetAccess(source))
	    {
		::send(mynick, source, Parent->getMessage(source, "CS_STATUS/HIGHERACCESS"),
			cstored->Access->Entry().c_str(), channel.c_str());
		return;
	    }

	    Parent->chanserv.stats.i_Access++;
	    ::send(mynick, source, Parent->getMessage(source, "LIST/DEL2"),
		    cstored->Access->Entry().c_str(), channel.c_str(),
		    Parent->getMessage(source, "LIST/ACCESS").c_str());
	    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/ACCESS_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		cstored->Access->Entry().c_str(), channel.c_str());
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

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
	!(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.list[Parent->commserv.OVR_View()].IsOn(source)))
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
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
		who.c_str());
	return;
    }
    else if (Parent->nickserv.stored[who.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
		Parent->getSname(who).c_str());
	return;
    }
    else
    {
	who = Parent->getSname(who);
	MLOCK(("ChanServ", "stored", cstored->Name().LowerCase(), "Access"));
	if (cstored->Access_find(who))
	{
	    // Reject if they're higher on access list, else erase them
	    // from the access list, AKICK doesnt play nice with ACCESS.
	    if (cstored->Access->Value() >= cstored->GetAccess(source))
	    {
		::send(mynick, source, Parent->getMessage(source, "CS_STATUS/HIGHERACCESS"),
			who.c_str(), channel.c_str());
		return;
	    }
	    else
	    {
		cstored->Access_erase();
	    }
	}
    }

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
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/AKICK_ADD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		who.c_str(), channel.c_str());

	if (Parent->chanserv.IsLive(channel.c_str()))
	{
	    unsigned int i;
	    if (who.Contains("@"))
	    {
		// Kick matching users ...
		vector<mstring> kickees;
		for (i=0; i<Parent->chanserv.live[channel.LowerCase()].Users(); i++)
		{
		    // MAN these commands can get REAL long .. ;)
		    if (Parent->nickserv.IsLive(Parent->chanserv.live[channel.LowerCase()].User(i)) &&
			Parent->nickserv.live[Parent->chanserv.live[channel.LowerCase()].User(i).LowerCase()].Mask(Nick_Live_t::N_U_P_H).LowerCase().Matches(who.LowerCase()))
		    {
			kickees.push_back(Parent->chanserv.live[channel.LowerCase()].User(i));
		    }
		}
		for (i=0; i<kickees.size(); i++)
		{
		    Parent->server.KICK(mynick, kickees[i], channel,
				((reason != "") ? reason : Parent->chanserv.DEF_Akick_Reason()));
		}
	    }
	    else
	    {
		// Kick stored user ...
		mstring realnick = who;
		if (Parent->nickserv.stored[who.LowerCase()].Host() != "")
		    realnick = Parent->nickserv.stored[who.LowerCase()].Host();
		if (Parent->chanserv.live[channel.LowerCase()].IsIn(realnick))
		{
		    Parent->server.KICK(mynick, realnick, channel,
			((reason != "") ? reason : Parent->chanserv.DEF_Akick_Reason()));
		}
		for (i=0; i<Parent->nickserv.stored[realnick.LowerCase()].Siblings(); i++)
		{
		    if (Parent->chanserv.live[channel.LowerCase()].IsIn(Parent->nickserv.stored[realnick.LowerCase()].Sibling(i)))
		    {
			Parent->server.KICK(mynick, Parent->nickserv.stored[realnick.LowerCase()].Sibling(i), channel,
				((reason != "") ? reason : Parent->chanserv.DEF_Akick_Reason()));
		    }
		}
	    }
	}
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
	if (who.Contains("."))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_STYNTAX/WHOLENUMBER"));
	    return;
	}
	unsigned int i, num = atoi(who);
	if (num < 1 || num > cstored->Akick_size())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
		1, cstored->Akick_size());
	    return;
	}


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
	    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/AKICK_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		cstored->Akick->Entry().c_str(), channel.c_str());
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
	    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/AKICK_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		cstored->Akick->Entry().c_str(), channel.c_str());
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

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
	!(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.list[Parent->commserv.OVR_View()].IsOn(source)))
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
	cstored->Akick != cstored->Akick_end(); cstored->Akick++, i++)
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
	    if (Parent->nickserv.stored[target.LowerCase()].Forbidden())
	    {
		::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
			Parent->getSname(target).c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/GREET_ADD"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), channel.c_str());
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
	    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/GREET_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		target.c_str(), channel.c_str());
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

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
	(cstored->GetAccess(source, "OVERGREET") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.list[Parent->commserv.OVR_View()].IsOn(source))))
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/MESSAGE_ADD"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	channel.c_str());
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
	::send(mynick, source, Parent->getMessage(source, "LIST/WHOLENUMBER"));
	return;
    }
    int num = atoi(target);
    if (num < 1 || num > (int) cstored->Message_size())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
		1, cstored->Message_size());
	return;
    }


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
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/MESSAGE_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		channel.c_str());
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

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
    if (!(cstored->GetAccess(source, "MESSAGE") ||
	(Parent->commserv.IsList(Parent->commserv.OVR_View()) &&
	Parent->commserv.list[Parent->commserv.OVR_View()].IsOn(source))))
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

    if (!(cstored->GetAccess(source) > Parent->chanserv.Level_Max() ||
	(Parent->commserv.IsList(Parent->commserv.OVR_Owner()) &&
	Parent->commserv.list[Parent->commserv.OVR_Owner()].IsOn(source))))
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
    else if (Parent->nickserv.stored[founder.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
		Parent->getSname(founder).c_str());
	return;
    }
    else if (Parent->nickserv.stored[founder.LowerCase()].Host() != "" &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[founder.LowerCase()].Host()))
    {
	founder = Parent->nickserv.stored[founder.LowerCase()].Host();
    }
    founder = Parent->getSname(founder);

    if (Parent->chanserv.Max_Per_Nick() &&
	Parent->nickserv.stored[founder.LowerCase()].MyChannels() >=
	Parent->chanserv.Max_Per_Nick())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_TOOMANY"),
				founder.c_str());
	return;
    }

    cstored->Founder(founder);
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/FOUNDER").c_str(),
	    channel.c_str(), founder.c_str());
    Log(LM_INFO, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/FOUNDER").c_str(),
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

    if (!(cstored->GetAccess(source) > Parent->chanserv.Level_Max() ||
	(Parent->commserv.IsList(Parent->commserv.OVR_Owner()) &&
	Parent->commserv.list[Parent->commserv.OVR_Owner()].IsOn(source))))
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
    else if (Parent->nickserv.stored[founder.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
		Parent->getSname(founder).c_str());
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

    if (Parent->chanserv.Max_Per_Nick() &&
	Parent->nickserv.stored[founder.LowerCase()].MyChannels() >=
	Parent->chanserv.Max_Per_Nick())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/OTH_TOOMANY"),
				founder.c_str());
	return;
    }

    cstored->CoFounder(founder);
    Parent->chanserv.stats.i_Set++;
    if (founder == "")
    {
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNSET"),
	    Parent->getMessage(source, "CS_SET/COFOUNDER").c_str(),
	    channel.c_str());
	Log(LM_INFO, Parent->getLogMessage("CHANSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/COFOUNDER").c_str(),
		channel.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/COFOUNDER").c_str(),
	    channel.c_str(), founder.c_str());
	Log(LM_INFO, Parent->getLogMessage("CHANSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/COFOUNDER").c_str(),
		channel.c_str(), founder.c_str());
    }
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/DESCRIPTION").c_str(),
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

    if (password.Len() < 5 || password.CmpNoCase(channel.After(channel[0u])) == 0 ||
	password.CmpNoCase(channel) == 0 || password.CmpNoCase(source) == 0)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/COMPLEX_PASS"));
	return;
    }

    cstored->Password(password);
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/PASSWORD").c_str(),
	    channel.c_str(), password.c_str());
    Log(LM_INFO, Parent->getLogMessage("CHANSERV/SET_PASSWORD"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	channel.c_str());
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
    else if (!option.Contains("@"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAIN"),
		Parent->getMessage(source, "CS_SET/EMAIL").c_str(), '@');
	return;
    }
    else if (option.WordCount("@") != 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAINONE"),
		Parent->getMessage(source, "CS_SET/EMAIL").c_str(), '@');
	return;
    }

    cstored->Email(option);
    Parent->chanserv.stats.i_Set++;
    if (option == "")
    {
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNSET"),
	    Parent->getMessage(source, "CS_SET/EMAIL").c_str(),
	    channel.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/EMAIL").c_str(),
		channel.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/EMAIL").c_str(),
	    channel.c_str(), option.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/EMAIL").c_str(),
		channel.c_str(), option.c_str());
    }
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
    {
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNSET"),
	    Parent->getMessage(source, "CS_SET/URL").c_str(),
	    channel.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/URL").c_str(),
		channel.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/URL").c_str(),
	    channel.c_str(), ("http://" + option).c_str());
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/URL").c_str(),
		channel.c_str(), ("http://" + option).c_str());
    }
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
    {
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNSET"),
	    Parent->getMessage(source, "CS_SET/COMMENT").c_str(),
	    channel.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/COMMENT").c_str(),
		channel.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/COMMENT").c_str(),
	    channel.c_str(), option.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("CS_SET/COMMENT").c_str(),
		channel.c_str(), option.c_str());
    }
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
	    channel.c_str(), ToHumanTime(num, source).c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/BANTIME").c_str(),
	channel.c_str(), ToHumanTime(num, source).c_str());
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
	    channel.c_str(), ToHumanTime(num, source).c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/PARTTIME").c_str(),
	channel.c_str(), ToHumanTime(num, source).c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/KEEPTOPIC").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/KEEPTOPIC").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/PRIVATE").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/SECUREOPS").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/SECURE").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/NOEXPIRE").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/ANARCHY").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
}

void ChanServ::do_set_KickOnBan(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_KickOnBan", (mynick, source, params));

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

    if (cstored->L_KickOnBan())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/KICKONBAN").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_KickOnBan())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->KickOnBan(onoff.GetBool());
    Parent->chanserv.stats.i_Set++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/SET_TO"),
	    Parent->getMessage(source, "CS_SET/KICKONBAN").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/KICKONBAN").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/RESTRICTED").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
}

void ChanServ::do_set_Join(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_set_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/JOIN").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/REVENGE").c_str(),
	channel.c_str(),
	Parent->getMessage("CS_SET/REV_" + option.UpperCase()).c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/BANTIME").c_str(),
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
	    channel.c_str(), ToHumanTime(num, source).c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/PARTTIME").c_str(),
	channel.c_str(), ToHumanTime(num, source).c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/KEEPTOPIC").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/TOPICLOCK").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/PRIVATE").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/SECUREOPS").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/SECURE").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/ANARCHY").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
}

void ChanServ::do_lock_KickOnBan(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_KickOnBan", (mynick, source, params));

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

    if (Parent->chanserv.LCK_KickOnBan())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/KICKONBAN").c_str(),
		channel.c_str());
	return;
    }

    if (!onoff.CmpNoCase("default") || !onoff.CmpNoCase("reset"))
    {
	if (Parent->chanserv.DEF_KickOnBan())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBEONOFF"));
	return;
    }

    cstored->L_KickOnBan(false);
    cstored->KickOnBan(onoff.GetBool());
    cstored->L_KickOnBan(true);
    Parent->chanserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/LOCKED"),
	    Parent->getMessage(source, "CS_SET/KICKONBAN").c_str(),
	    channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/KICKONBAN").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/RESTRICTED").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
}

void ChanServ::do_lock_Join(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_lock_Join", (mynick, source, params));

    mstring message = mstring(params.Before(" ") + " " +
		params.ExtractWord(3, " ")).UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }}

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
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/JOIN").c_str(),
	channel.c_str(), onoff.GetBool() ?
		Parent->getMessage(source, "VALS/ON").c_str() :
		Parent->getMessage(source, "VALS/OFF").c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/REVENGE").c_str(),
	channel.c_str(),
	Parent->getMessage("CS_SET/REV_" + option.UpperCase()).c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/BANTIME").c_str(),
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/PARTTIME").c_str(),
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/KEEPTOPIC").c_str(),
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/TOPICLOCK").c_str(),
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/PRIVATE").c_str(),
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/SECUREOPS").c_str(),
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/SECURE").c_str(),
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/ANARCHY").c_str(),
	channel.c_str());
}

void ChanServ::do_unlock_KickOnBan(mstring mynick, mstring source, mstring params)
{
    FT("ChanServ::do_unlock_KickOnBan", (mynick, source, params));

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

    if (Parent->chanserv.LCK_KickOnBan())
    {
	::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISLOCKED"),
		Parent->getMessage(source, "CS_SET/KICKONBAN").c_str(),
		channel.c_str());
	return;
    }

    cstored->L_KickOnBan(false);
    Parent->chanserv.stats.i_Unlock++;
    ::send(mynick, source, Parent->getMessage(source, "CS_COMMAND/UNLOCKED"),
	    Parent->getMessage(source, "CS_SET/KICKONBAN").c_str(),
	    channel.c_str());
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/KICKONBAN").c_str(),
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/RESTRICTED").c_str(),
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/JOIN").c_str(),
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
    Log(LM_DEBUG, Parent->getLogMessage("CHANSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("CS_SET/REVENGE").c_str(),
	channel.c_str());
}


long ChanServ::LVL(mstring level)
{
    FT("ChanServ::LVL", (level));
    long retval = 0;
    if (!IsLVL(level) ||
	lvl[level.UpperCase()] > Level_Max() + 1 ||
	lvl[level.UpperCase()] < Level_Min())
    {
	retval = Level_Min()-1;
    }
    else
    {
	retval = lvl[level.UpperCase()];
    }
    RET(retval);
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
    bool retval = lvl.find(level.UpperCase()) != lvl.end();
    RET(retval);
}


SXP::Tag ChanServ::tag_ChanServ("ChanServ");

void ChanServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("ChanServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Chan_Stored_t *cs = new Chan_Stored_t;

    if( pElement->IsA( cs->GetClassTag() ) )
    {
	cs_array.push_back(cs);
	pIn->ReadTo(cs);
    }
    else
    {
	delete cs;
    }
}

void ChanServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("ChanServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void ChanServ::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("ChanServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_ChanServ, attribs);

    map<mstring, Chan_Stored_t>::iterator iter;
    { RLOCK(("ChanServ", "stored"));
    for (iter = stored.begin(); iter != stored.end(); iter++)
	pOut->WriteSubElement(&iter->second, attribs);
    }

    pOut->EndObject(tag_ChanServ);
}

void ChanServ::PostLoad()
{
    NFT("ChanServ::PostLoad");
    // Linkage, etc
    unsigned int i, j;
    for (i=0; i<cs_array.size(); i++)
    {
	if (cs_array[i] != NULL)
	{
	    for (j=0; j<cs_array[i]->level_array.size(); j++)
	    {
		if (cs_array[i]->level_array[j] != NULL)
		{
		    cs_array[i]->i_Level.insert(*cs_array[i]->level_array[j]);
		    delete cs_array[i]->level_array[j];
		}
	    }
	    cs_array[i]->level_array.clear();
	    for (j=0; j<cs_array[i]->access_array.size(); j++)
	    {
		if (cs_array[i]->access_array[j] != NULL)
		{
		    cs_array[i]->i_Access.insert(*cs_array[i]->access_array[j]);
		    delete cs_array[i]->access_array[j];
		}
	    }
	    cs_array[i]->access_array.clear();
	    for (j=0; j<cs_array[i]->akick_array.size(); j++)
	    {
		if (cs_array[i]->akick_array[j] != NULL)
		{
		    cs_array[i]->i_Akick.insert(*cs_array[i]->akick_array[j]);
		    delete cs_array[i]->akick_array[j];
		}
	    }
	    cs_array[i]->akick_array.clear();
	    for (j=0; j<cs_array[i]->greet_array.size(); j++)
	    {
		if (cs_array[i]->greet_array[j] != NULL)
		{
		    cs_array[i]->i_Greet.push_back(*cs_array[i]->greet_array[j]);
		    delete cs_array[i]->greet_array[j];
		}
	    }
	    cs_array[i]->greet_array.clear();
	    for (j=0; j<cs_array[i]->message_array.size(); j++)
	    {
		if (cs_array[i]->message_array[j] != NULL)
		{
		    cs_array[i]->i_Message.push_back(*cs_array[i]->message_array[j]);
		    delete cs_array[i]->message_array[j];
		}
	    }
	    cs_array[i]->message_array.clear();
	    for (j=0; j<cs_array[i]->ud_array.size(); j++)
	    {
		if (cs_array[i]->ud_array[j] != NULL)
		{
		    if (cs_array[i]->ud_array[j]->Contains("\n"))
			cs_array[i]->i_UserDef[cs_array[i]->ud_array[j]->Before("\n")] =
				cs_array[i]->ud_array[j]->After("\n");
		    delete cs_array[i]->ud_array[j];
		}
	    }
	    cs_array[i]->ud_array.clear();
	    if (cs_array[i]->Name() != "")
		stored[cs_array[i]->Name().LowerCase()] = *cs_array[i];
	    delete cs_array[i];
	}
    }
    cs_array.clear();

    map<mstring,Chan_Stored_t>::iterator iter;
    entlist_val_t<long> *ptr1;
    entlist_val_t<mstring> *ptr2;
    for (iter=stored.begin(); iter!=stored.end(); iter++)
    {
	for (iter->second.Level = iter->second.Level_begin();
		iter->second.Level != iter->second.Level_end();
		iter->second.Level++)
	{
	    ptr1 = (entlist_val_t<long> *) &(*iter->second.Level);
	    ptr1->PostLoad();
	}
	for (iter->second.Access = iter->second.Access_begin();
		iter->second.Access != iter->second.Access_end();
		iter->second.Access++)
	{
	    ptr1 = (entlist_val_t<long> *) &(*iter->second.Access);
	    ptr1->PostLoad();
	}
	for (iter->second.Akick = iter->second.Akick_begin();
		iter->second.Akick != iter->second.Akick_end();
		iter->second.Akick++)
	{
	    ptr2 = (entlist_val_t<mstring> *) &(*iter->second.Akick);
	    ptr2->PostLoad();
	}
	for (iter->second.Greet = iter->second.Greet_begin();
		iter->second.Greet != iter->second.Greet_end();
		iter->second.Greet++)
	    iter->second.Greet->PostLoad();
	for (iter->second.Message = iter->second.Message_begin();
		iter->second.Message != iter->second.Message_end();
		iter->second.Message++)
	    iter->second.Message->PostLoad();
    }
}
