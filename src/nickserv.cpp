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
** Revision 1.121  2000/08/07 12:20:28  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.120  2000/08/06 21:56:14  prez
** Fixed some small problems in akill/clone protection
**
** Revision 1.119  2000/08/06 08:06:41  prez
** Fixed loading of logon messages in committee ..
**
** Revision 1.118  2000/08/06 05:27:47  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.117  2000/08/03 13:06:31  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.116  2000/08/02 20:08:57  prez
** Minor code cleanups, added ACE installation instructions, updated the
** suggestions file and stopped people doing a whole bunch of stuff to
** forbidden nicknames.
**
** Revision 1.115  2000/07/30 09:04:06  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.114  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.113  2000/07/21 00:18:49  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.112  2000/06/28 12:20:48  prez
** Lots of encryption stuff, but essentially, we now have random
** key generation for the keyfile keys, and we can actually encrypt
** something, and get it back as we sent it in (specifically, the
** keyfile itself).
**
** Revision 1.111  2000/06/25 11:58:03  prez
** Fixed problem where messages from nickserv about killing user would not
** be sent out (people would not know a nick was forbidden).
**
** Revision 1.110  2000/06/25 10:32:41  prez
** Fixed channel forbid.
**
** Revision 1.109  2000/06/18 13:31:48  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.108  2000/06/18 12:49:27  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.107  2000/06/15 13:41:11  prez
** Added my tasks to develop *grin*
** Also did all the chanserv live locking (stored to be done).
** Also made magick check if its running, and kill on startup if so.
**
** Revision 1.106  2000/06/13 14:11:54  prez
** Locking system has been re-written, it doent core anymore.
** So I have set 'MAGICK_LOCKS_WORK' define active :)
**
** Revision 1.105  2000/06/12 06:07:50  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.104  2000/06/11 09:30:21  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.103  2000/06/11 08:20:12  prez
** More minor bug fixes, godda love testers.
**
** Revision 1.102  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.101  2000/06/08 13:07:34  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.100  2000/06/06 08:57:57  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.99  2000/05/28 02:37:16  prez
** Minor bug fixes (help system and changing nicks)
**
** Revision 1.98  2000/05/27 07:06:02  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.97  2000/05/25 08:16:39  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.96  2000/05/22 13:00:09  prez
** Updated version.h and some other stuff
**
** Revision 1.95  2000/05/21 04:49:40  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.94  2000/05/19 10:48:14  prez
** Finalized the DCC Sending (now uses the Action map properly)
**
** Revision 1.93  2000/05/18 11:41:46  prez
** Fixed minor front-end issues with the filesystem...
**
** Revision 1.92  2000/05/17 14:08:12  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.91  2000/05/17 12:39:55  prez
** Fixed DCC Sending and file lookups (bypassed the DccMap for now).
** Still to fix DCC Receiving.  Looks like a wxFile::Length() issue.
**
** Revision 1.90  2000/05/17 07:47:59  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.89  2000/05/14 06:30:14  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.88  2000/05/14 04:02:54  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.87  2000/05/13 08:26:44  ungod
** no message
**
** Revision 1.86  2000/05/13 07:05:46  prez
** Added displaying of sizes to all file fields..
**
** Revision 1.85  2000/05/10 11:46:59  prez
** added back memo timers
**
** Revision 1.84  2000/05/08 14:42:02  prez
** More on xmlisation of nickserv and chanserv
**
** Revision 1.83  2000/05/03 14:12:23  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.82  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.81  2000/04/18 14:34:23  prez
** Fixed the HELP system, it now loads into memory, and can be unloaded
** with the OS unload command.  The stats however are inaccurate.
**
** Revision 1.80  2000/04/16 14:29:44  prez
** Added stats for usage, and standardized grabbing paths, etc.
**
** Revision 1.79  2000/04/16 06:12:13  prez
** Started adding body to the documentation...
**
** Revision 1.78  2000/04/04 03:21:35  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.77  2000/04/04 03:13:51  prez
** Added support for masking hostnames.
**
** Revision 1.76  2000/04/03 09:45:24  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.75  2000/04/02 13:06:04  prez
** Fixed the channel TOPIC and MODE LOCK stuff ...
**
** Also fixed the setting of both on join...
**
** Revision 1.74  2000/04/02 07:25:05  prez
** Fixed low watermarks with threads, it all works now!
**
** Revision 1.73  2000/03/29 09:41:19  prez
** Attempting to fix thread problem with mBase, and added notification
** of new memos on join of channel or signon to network.
**
** Revision 1.72  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.71  2000/03/27 21:26:12  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.70  2000/03/26 14:59:37  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.69  2000/03/24 15:35:18  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.68  2000/03/24 12:53:05  prez
** FileSystem Logging
**
** Revision 1.67  2000/03/23 10:22:25  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.66  2000/03/19 08:50:55  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.65  2000/03/15 14:42:59  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.64  2000/03/14 10:05:17  prez
** Added Protocol class (now we can accept multi IRCD's)
**
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

void Nick_Live_t::InFlight_t::ChgNick(mstring newnick)
{
    FT("Nick_Live_t::InFlight_t::ChgNick", (newnick));
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    WLOCK2(("NickServ", "live", newnick.LowerCase(), "InFlight"));
    nick = newnick;
    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    if (timer)
    {
	mstring *arg = NULL;
	if (ACE_Reactor::instance()->cancel_timer(timer,
		(const void **) arg) &&
	    arg != NULL)
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
    WLOCK(("NickServ", "live", in.nick.LowerCase(), "InFlight"));
    nick        = in.nick;
    type	= in.type;
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
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    if (Exists())
	End(0u);
    mstring *arg = NULL;
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
		(const void **) arg) &&
	    arg != NULL)
	{
	    delete arg;
	}
}


// Initialise all veriables (done in Cancel() or End())
void Nick_Live_t::InFlight_t::init()
{
    NFT("Nick_Live_t::InFlight_t::init");
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    type = FileMap::MemoAttach;
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
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileinprog"));
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
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileinprog"));
    fileinprog = true;
    mstring *arg = NULL;
    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
		(const void **) arg) &&
	    arg != NULL)
	{
	    delete arg;
	}
}


// New memo, send an old one if it isnt in-progress, and
// cancel it if it was never started.
void Nick_Live_t::InFlight_t::Memo (bool file, mstring mynick,
			mstring who, mstring message, bool silent)
{
    FT("Nick_Live_t::InFlight_t::Memo", (file, mynick, who, message, silent));
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
	if (Parent->nickserv.stored[who.LowerCase()].Forbidden())
	{
	    ::send(mynick, nick, Parent->getMessage(nick, "NS_OTH_STATUS/ISFORBIDDEN"),
			Parent->getSname(who).c_str());
	    return;
	}

	if (file && !Parent->memoserv.Files())
	{
	    send(mynick, nick, Parent->getMessage(nick, "MS_SYNTAX/FILEDISABLED"));
	    return;
	}
    }

    if (file && !(Parent->files.TempDirSize() == 0 ||
	mFile::DirUsage(Parent->files.TempDir()) <=
		Parent->files.TempDirSize()))
    {
	send(mynick, nick, Parent->getMessage(nick, "DCC/NOSPACE2"));
	return;
    }

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

    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    type = FileMap::MemoAttach;
    fileattach = file;
    service = mynick;
    sender = nick;
    recipiant = who;
    text = message;

    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));

    if (!silent)
    {
	if (fileattach)
	    send(service, nick, Parent->getMessage(nick, "MS_COMMAND/PENDING_FILE"),
		ToHumanTime(Parent->memoserv.InFlight()).c_str());
	else
	    send(service, nick, Parent->getMessage(nick, "MS_COMMAND/PENDING"),
		ToHumanTime(Parent->memoserv.InFlight()).c_str());
    }
}


// Add text to a memo, and re-start the timer.
void Nick_Live_t::InFlight_t::Continue(mstring message)
{
    FT("Nick_Live_t::InFlight_t::Continue", (message));
    if (!Memo())
    {
	send(service, nick, Parent->getMessage(nick, "MS_STATUS/NOPENDING"));
	return;
    }
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "text"));
    text += message;
    mstring *arg = NULL;
    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
		(const void **) arg) &&
	    arg != NULL)
	{
	    delete arg;
	}
    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(nick.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));
    send(service, nick, Parent->getMessage(nick, "MS_COMMAND/CONTINUE"),
	    ToHumanTime(Parent->memoserv.InFlight()).c_str());
}


// Cancel a memo or picture send.
void Nick_Live_t::InFlight_t::Cancel()
{
    NFT("Nick_Live_t::InFlight_t::Cancel");

    mstring *arg = NULL;
    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    if (timer)
	if (ACE_Reactor::instance()->cancel_timer(timer,
		(const void **) arg) &&
	    arg != NULL)
	{
	    delete arg;
	}
    if (Memo() && !File())
    {
	send(service, nick, Parent->getMessage(nick, "MS_COMMAND/CANCEL"));
    }
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
	mstring *arg = NULL;
	MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
	if (timer)
	    if (ACE_Reactor::instance()->cancel_timer(timer,
		(const void **) arg) &&
		arg != NULL)
	    {
		delete arg;
	    }
	RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
	if (Parent->nickserv.IsStored(sender))
	{
	    if (Parent->nickserv.stored[sender.LowerCase()].Host() != "" &&
		Parent->nickserv.IsStored(Parent->nickserv.stored[sender.LowerCase()].Host()))
	    {
		sender = Parent->nickserv.stored[Parent->nickserv.stored[sender.LowerCase()].Host()].Name();
	    }
	    if (Memo())
	    {
		if (IsChan(recipiant))
		{
		    if (Parent->chanserv.IsStored(recipiant))
		    {
			{ WLOCK(("MemoServ", "channel", recipiant.LowerCase()));
			Parent->memoserv.channel[recipiant.LowerCase()].push_back(
			    News_t(recipiant, sender, text));
			}
			send(service, nick, Parent->getMessage(nick, "MS_COMMAND/SENT"),
			    recipiant.c_str(),
			    Parent->chanserv.stored[recipiant.LowerCase()].Founder().c_str());
			Log(LM_DEBUG, Parent->getLogMessage("MEMOSERV/SEND"),
				Parent->nickserv.live[sender.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
				recipiant.c_str());
			if (Parent->chanserv.IsLive(recipiant))
			{
			    Chan_Live_t *chan = &Parent->chanserv.live[recipiant.LowerCase()];
			    unsigned int i;
			    for(i=0; i<chan->Users(); i++)
			    {
				if (Parent->nickserv.IsStored(chan->User(i)) &&
				    Parent->chanserv.stored[recipiant.LowerCase()].GetAccess(chan->User(i), "READMEMO"))
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

			if (!(!filenum || Parent->memoserv.FileSize() == 0 ||
			    Parent->filesys.GetSize(FileMap::MemoAttach, filenum) <=
			    Parent->memoserv.FileSize()))
			{
			    Parent->filesys.EraseFile(FileMap::MemoAttach, filenum);
			    send(service, nick, Parent->getMessage(nick, "MS_COMMAND/TOOBIG"));
			}
			else if (!(!filenum || Parent->files.MemoAttachSize() == 0 ||
			    Parent->filesys.FileSysSize(FileMap::MemoAttach) <=
			    Parent->files.MemoAttachSize()))
			{
			    Parent->filesys.EraseFile(FileMap::MemoAttach, filenum);
			    send(service, nick, Parent->getMessage(nick, "MS_COMMAND/NOSPACE"));
			}
			else if (recipiant == realrecipiant ||
			    Parent->nickserv.IsStored(realrecipiant))
			{
			    { WLOCK(("MemoServ", "nick", realrecipiant.LowerCase()));
			    Parent->memoserv.nick[realrecipiant.LowerCase()].push_back(
				Memo_t(realrecipiant, sender, text, filenum));
			    }
			    if (filenum)
				Log(LM_DEBUG, Parent->getLogMessage("MEMOSERV/FILE"),
					Parent->nickserv.live[sender.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
					Parent->filesys.GetName(FileMap::MemoAttach, filenum).c_str(),
					filenum, ToHumanSpace(Parent->filesys.GetSize(FileMap::MemoAttach, filenum)).c_str(),
					realrecipiant.c_str());
			    send(service, nick, Parent->getMessage(nick, "MS_COMMAND/SENT"),
				recipiant.c_str(), realrecipiant.c_str());

			    Nick_Stored_t *nstored = &Parent->nickserv.stored[realrecipiant.LowerCase()];
			    if (nstored->IsOnline())
				send(service, realrecipiant, Parent->getMessage(realrecipiant, "MS_COMMAND/NS_NEW"),
				    Parent->memoserv.nick[realrecipiant.LowerCase()].size(),
				    service.c_str(),
				    Parent->memoserv.nick[realrecipiant.LowerCase()].size());
			    unsigned int i;
			    for (i=0; i < nstored->Siblings(); i++)
			    {
				if (Parent->nickserv.stored[nstored->Sibling(i).LowerCase()].IsOnline())
				{
				    send(service, nstored->Sibling(i), Parent->getMessage(nstored->Sibling(i), "MS_COMMAND/NS_NEW"),
					Parent->memoserv.nick[realrecipiant.LowerCase()].size(),
					service.c_str(),
					Parent->memoserv.nick[realrecipiant.LowerCase()].size());
				}
			    }
			}
			else if (File())
			{
			    Parent->filesys.EraseFile(FileMap::MemoAttach, filenum);
			    send(service, nick, Parent->getMessage(nick, "MS_COMMAND/CANCEL"));
			}
		    }
		    else if (File())
		    {
			Parent->filesys.EraseFile(FileMap::MemoAttach, filenum);
			send(service, nick, Parent->getMessage(nick, "MS_COMMAND/CANCEL"));
		    }
		}
	    }
	    else if (Picture())
	    {
		if (filenum && !(Parent->memoserv.FileSize() == 0 ||
		    Parent->filesys.GetSize(FileMap::Picture, filenum) <=
		    Parent->nickserv.PicSize()))
		{
		    Parent->filesys.EraseFile(FileMap::Picture, filenum);
		    send(service, nick, Parent->getMessage(nick, "NS_YOU_COMMAND/TOOBIG"));
		}
		else if (filenum && !(Parent->files.PictureSize() == 0 ||
		    Parent->filesys.FileSysSize(FileMap::Picture) <=
		    Parent->files.PictureSize()))
		{
		    Parent->filesys.EraseFile(FileMap::Picture, filenum);
		    send(service, nick, Parent->getMessage(nick, "NS_YOU_COMMAND/NOSPACE"));
		}
		else if (filenum)
		{
		    Parent->nickserv.stored[sender.LowerCase()].GotPic(filenum);
		    send(service, nick, Parent->getMessage(nick, "NS_YOU_COMMAND/SAVED"));
		    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/PICTURE_ADD"),
			Parent->nickserv.live[sender.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
			sender.c_str(), filenum,
			ToHumanSpace(Parent->filesys.GetSize(FileMap::Picture, filenum)).c_str());
		}
		else
		{
		    send(service, nick, Parent->getMessage(nick, "DCC/FAILED"), "GET");
		}
	    }
	    else if (Public())
	    {
		if (filenum && !(Parent->files.PublicSize() == 0 ||
		    Parent->filesys.FileSysSize(FileMap::Public) <=
		    Parent->files.PublicSize()))
		{
		    Parent->filesys.EraseFile(FileMap::Public, filenum);
		    send(service, nick, Parent->getMessage(nick, "DCC/NOSPACE"));
		}
		else if (filenum)
		{
		    send(service, nick, Parent->getMessage(nick, "LIST/ADD"),
    			Parent->filesys.GetName(FileMap::Public, filenum).c_str(),
    			Parent->getMessage(nick,"LIST/FILES").c_str());
		    Parent->filesys.SetPriv(FileMap::Public, filenum, text);
		    Log(LM_DEBUG, Parent->getLogMessage("SERVMSG/FILE_ADD"),
			Parent->nickserv.live[sender.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
			Parent->filesys.GetName(FileMap::Public, filenum).c_str(),
			filenum, ToHumanSpace(Parent->filesys.GetSize(FileMap::Public, filenum)).c_str(),
			((Parent->filesys.GetPriv(FileMap::Public, filenum) == "") ?
				"ALL" : Parent->filesys.GetPriv(FileMap::Public, filenum).c_str()));
		}
		else
		{
		    send(service, nick, Parent->getMessage(nick, "DCC/FAILED"), "GET");
		}
	    }
	}
	init();
    }
}


void Nick_Live_t::InFlight_t::Picture(mstring mynick)
{
    FT("Nick_Live_t::InFlight_t::Picture", (mynick));
    if (!Parent->nickserv.IsStored(nick))
    {
	send(mynick, nick, Parent->getMessage(nick, "NS_YOU_STATUS/ISNOTSTORED"));
	return;
    }
    else if (Parent->nickserv.PicExt() == "")
    {
	send(mynick, nick, Parent->getMessage(nick, "NS_YOU_STATUS/PICDISABLED"));
	return;
    }

    if (!(Parent->files.TempDirSize() == 0 ||
	mFile::DirUsage(Parent->files.TempDir()) <=
		Parent->files.TempDirSize()))
    {
	send(mynick, nick, Parent->getMessage(nick, "DCC/NOSPACE2"));
	return;
    }

    if (File())
    {
	if (InProg())
	{
	    send(mynick, nick, Parent->getMessage(nick, "ERR_SITUATION/FILEINPROG"));
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

    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    type = FileMap::Picture;
    fileattach = true;
    sender = nick;
    service = mynick;

    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));

    send(service, nick, Parent->getMessage(nick, "NS_YOU_COMMAND/PENDING"));
}


void Nick_Live_t::InFlight_t::Public(mstring mynick, mstring committees)
{
    FT("Nick_Live_t::InFlight_t::Public", (mynick, committees));
    if (!Parent->nickserv.IsStored(nick))
    {
	send(mynick, nick, Parent->getMessage(nick, "NS_YOU_STATUS/ISNOTSTORED"));
	return;
    }

    if (!(Parent->files.TempDirSize() == 0 ||
	mFile::DirUsage(Parent->files.TempDir()) <=
		Parent->files.TempDirSize()))
    {
	send(mynick, nick, Parent->getMessage(nick, "DCC/NOSPACE2"));
	return;
    }

    if (File())
    {
	if (InProg())
	{
	    send(mynick, nick, Parent->getMessage(nick, "ERR_SITUATION/FILEINPROG"));
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

    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    type = FileMap::Public;
    fileattach = true;
    sender = nick;
    service = mynick;
    text = committees;

    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
    timer = ACE_Reactor::instance()->schedule_timer(&Parent->nickserv.ifh,
			new mstring(sender.LowerCase()),
			ACE_Time_Value(Parent->memoserv.InFlight()));

    send(service, nick, Parent->getMessage(nick, "NS_YOU_COMMAND/PUB_PENDING"));
}

bool Nick_Live_t::InFlight_t::Memo()
{
    NFT("Nick_Live_t::InFlight_t::Memo");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "recipiant"));
    RET(recipiant != "");
}

bool Nick_Live_t::InFlight_t::Picture()
{
    NFT("Nick_Live_t::InFlight_t::Picture");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "type"));
    RET(type == FileMap::Picture);
}

bool Nick_Live_t::InFlight_t::Public()
{
    NFT("Nick_Live_t::InFlight_t::Public");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "type"));
    RET(type == FileMap::Public);
}

bool Nick_Live_t::InFlight_t::Exists()
{
    NFT("Nick_Live_t::InFlight_t::Exists");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "recipiant"));
    RLOCK2(("NickServ", "live", nick.LowerCase(), "InFlight", "type"));
    RET(recipiant != "" || type != FileMap::MemoAttach);
}

bool Nick_Live_t::InFlight_t::File()
{
    NFT("Nick_Live_t::InFlight_t::File");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileattach"));
    RET(fileattach);
}

bool Nick_Live_t::InFlight_t::InProg()
{
    NFT("Nick_Live_t::InFlight_t::InProg");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileinprog"));
    RET(fileinprog);
}

size_t Nick_Live_t::InFlight_t::Usage()
{
    size_t retval = 0;

    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    retval += nick.capacity();
    retval += sizeof(type);
    retval += sizeof(timer);
    retval += sizeof(fileattach);
    retval += sizeof(fileinprog);
    retval += sender.capacity();
    retval += recipiant.capacity();
    retval += text.capacity();

    return retval;
}


Nick_Live_t::Nick_Live_t()
{
    NFT("Nick_Live_t::Nick_Live_t");
    // Dont call anything that locks, no names!
    identified = false;
    services = true;
    last_msg_entries = flood_triggered_times = failed_passwds = 0;
}


Nick_Live_t::Nick_Live_t(mstring name, mDateTime signon, mstring server,
	    mstring username, mstring hostname, mstring realname)
{
    FT("Nick_Live_t::Nick_Live_t",(name, signon, server, username, hostname, realname));

    i_Name = name;
    WLOCK(("NickServ", "live", name.LowerCase()));
    i_Signon_Time = signon;
    i_My_Signon_Time = i_Last_Action = Now();
    i_server = server;
    i_user = username;
    i_host = hostname;
    i_alt_host = hostname;
    i_realname = realname;
    identified = false;
    services = false;
    InFlight.nick=i_Name;
    InFlight.init();
    last_msg_entries = flood_triggered_times = failed_passwds = 0;

    // User is on AKILL, add the mask, and No server will kill
    { MLOCK(("OperServ", "Akill"));
    if (Parent->operserv.Akill_find(i_user + "@" + i_host))
    {
	mstring reason = Parent->operserv.Akill->Value().second;
	Log(LM_INFO, Parent->getLogMessage("OTHER/KILL_AKILL"),
		Mask(N_U_P_H).c_str(), Parent->operserv.Akill->Entry().c_str(),
		reason.c_str());
	Parent->server.AKILL(Parent->operserv.Akill->Entry(), reason,
			Parent->operserv.Akill->Value().first -
				Parent->operserv.Akill->Last_Modify_Time().SecondsSince(),
			Parent->operserv.Akill->Last_Modifier());
	// Do this cos it will be removed when we KILL,
	// and we dont wanna get out of touch.
	Parent->operserv.AddHost(i_host);
	i_server = "";
	i_realname = reason;
	return;
    }}

    // User triggered CLONE protection, No server will kill
    if (Parent->operserv.AddHost(i_host))
    {
	Log(LM_INFO, Parent->getLogMessage("OTHER/KILL_CLONE"),
		Mask(N_U_P_H).c_str());
	i_server = "";
	i_realname = Parent->operserv.Def_Clone();
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
    WLOCK(("NickServ", "live", name.LowerCase()));
    i_Signon_Time = i_My_Signon_Time = Now();
    i_Last_Action = time(NULL);
    i_user = username;
    i_host = hostname;
    i_alt_host = hostname;
    i_realname = realname;
    identified = true;
    services = true;
    InFlight.nick = i_Name;
    InFlight.init();
    last_msg_entries = flood_triggered_times = failed_passwds = 0;
}


void Nick_Live_t::operator=(const Nick_Live_t &in)
{
    NFT("Nick_Live_t::operator=");
    WLOCK(("NickServ", "live", in.i_Name.LowerCase()));
    i_Name=in.i_Name;
    i_Signon_Time=in.i_Signon_Time;
    i_My_Signon_Time=in.i_My_Signon_Time;
    i_Last_Action=in.i_Last_Action;
    i_realname=in.i_realname;
    i_user=in.i_user;
    i_host=in.i_host;
    i_alt_host=in.i_alt_host;
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
}

void Nick_Live_t::Join(mstring chan)
{
    FT("Nick_Live_t::Join", (chan));
    bool joined = true;
    if (Parent->chanserv.IsLive(chan))
    {
	joined = Parent->chanserv.live[chan.LowerCase()].Join(i_Name);
    }
    else
    {
	Parent->chanserv.live[chan.LowerCase()] = Chan_Live_t(chan, i_Name);
    }
    // We do this seperately coz we require initialisation of
    // the channel to be completed.
    if (joined && Parent->chanserv.IsStored(chan))
	Parent->chanserv.stored[chan.LowerCase()].Join(i_Name);

    WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    joined_channels.insert(chan.LowerCase());
}


void Nick_Live_t::Part(mstring chan)
{
    FT("Nick_Live_t::Part", (chan));
    if (Parent->chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
	if (Parent->chanserv.live[chan.LowerCase()].Part(i_Name) == 0)
	{
	    WLOCK(("ChanServ", "live"));
	    Parent->chanserv.live.erase(chan.LowerCase());
	}
    }
    else
    {
	Log(LM_TRACE, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
		"PART", i_Name.c_str(), chan.c_str());
    }

    WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    joined_channels.erase(chan.LowerCase());
}

void Nick_Live_t::Kick(mstring kicker, mstring chan)
{
    FT("Nick_Live_t::Kick", (kicker, chan));
    if (Parent->chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
	if (Parent->chanserv.live[chan.LowerCase()].Kick(i_Name, kicker) == 0)
	{
	    WLOCK(("ChanServ", "live"));
	    Parent->chanserv.live.erase(chan.LowerCase());
	}
    }
    else
    {
	Log(LM_ERROR, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
		"KICK", kicker.c_str(), chan.c_str());
    }

    WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    joined_channels.erase(chan.LowerCase());
}


void Nick_Live_t::Quit(mstring reason)
{
    FT("Nick_Live_t::Quit", (reason));

    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    if (!(IsServices() || HasMode("o")))
	Parent->operserv.RemHost(i_host);
    }

    // Check if we're currently being TEMP ignored ...
    { MLOCK(("OperServ","Ignore"));
    if (Parent->operserv.Ignore_find(Mask(N_U_P_H)))
    {
	if (Parent->operserv.Ignore->Value() != true)
	{
	    Parent->operserv.Ignore_erase();
	}
    }}

    { RLOCK2(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    while (joined_channels.size())
	Part(*joined_channels.begin());
    }

    unsigned long i;
    vector<unsigned long> dccs = Parent->dcc->GetList(i_Name);
    for (i=0; i<dccs.size(); i++)
	Parent->dcc->Cancel(dccs[i], true);
    if (InFlight.Exists())
	InFlight.End(0u);

    // We successfully ident to all channels we tried to
    // ident for before, so that they 0 our count -- we dont
    // want it carrying over to next time we log on.
    { RLOCK3(("NickServ", "live", i_Name.LowerCase(), "try_chan_ident"));
    for (i=0; i<try_chan_ident.size(); i++)
	if (Parent->chanserv.IsStored(try_chan_ident[i]))
	    Parent->chanserv.stored[try_chan_ident[i]].CheckPass(i_Name,
		Parent->chanserv.stored[try_chan_ident[i]].Password());
    }

    if (Parent->nickserv.IsStored(i_Name) &&
	Parent->nickserv.stored[i_Name.LowerCase()].IsOnline())
	Parent->nickserv.stored[i_Name.LowerCase()].Quit(reason);
}


bool Nick_Live_t::IsInChan(mstring chan)
{
    FT("Nick_Live_t::IsInChan", (chan));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    bool retval = (joined_channels.find(chan.LowerCase()) != joined_channels.end());
    RET(retval);
}

set<mstring> Nick_Live_t::Channels()
{
    NFT("Nick_Live_t::Channels");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    NRET(set<mstring>, joined_channels);
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
    { MLOCK(("OperServ", "Ignore"));
    if (Parent->operserv.Ignore_size())
    {
	if (Parent->operserv.Ignore_find(Mask(N_U_P_H)))
	{
	    // IF we havnt ignored for long enough yet, or its perminant ...
	    if (Parent->operserv.Ignore->Last_Modify_Time().SecondsSince() <= Parent->operserv.Ignore_Time()
		|| Parent->operserv.Ignore->Value() == true)
	    {
		RET(true);
	    }
	    else
	    {
		Parent->operserv.Ignore_erase();
	    }
	}
    }}

    // Clean up previous entries and push current entry
    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "last_msg_times"));
    while (last_msg_times.size() && last_msg_times[0u].SecondsSince() > Parent->operserv.Flood_Time())
	last_msg_times.erase(last_msg_times.begin());
    last_msg_times.push_back(Now());
    }

    // Check if we just triggered ignore.
    if (last_msg_times.size() > Parent->operserv.Flood_Msgs())
    {
	WLOCK2(("NickServ", "live", i_Name.LowerCase(), "flood_triggered_times"));
	flood_triggered_times++;
	// Add To ignore, they're naughty.
	if (flood_triggered_times >= Parent->operserv.Ignore_Limit())
	{
	    Parent->operserv.Ignore_insert(Mask(Parent->operserv.Ignore_Method()), true, i_Name);
	    send(Parent->servmsg.FirstName(), i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/IGNORE_TRIGGER"),
			Parent->operserv.Flood_Msgs(), ToHumanTime(Parent->operserv.Flood_Time()).c_str());
	    send(Parent->servmsg.FirstName(), i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/PERM_IGNORE"),
			Parent->operserv.Ignore_Limit());
	    Log(LM_NOTICE, Parent->getLogMessage("OTHER/PERM_IGNORE"),
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
	    Log(LM_NOTICE, Parent->getLogMessage("OTHER/TEMP_IGNORE"),
			Mask(N_U_P_H).c_str());
	    announce(Parent->servmsg.FirstName(), Parent->getMessage("MISC/FLOOD_TEMP"),
			i_Name.c_str());
	}
 
	RET(true);
    }

    RET(false);

}


void Nick_Live_t::Name(mstring in)
{
    FT("Nick_Live_t::Name", (in));

    InFlight.ChgNick(in);

    WLOCK(("NickServ", "live", i_Name.LowerCase()));
    WLOCK2(("NickServ", "live", in.LowerCase()));
    if (i_Name.CmpNoCase(in)==0)
    {
	i_Name = in;
	return;
    }

    set<mstring>::iterator iter;
    vector<mstring> chunked;
    unsigned long i;

    // Store what committee's we WERE on ...
    // This is needed to send logon notices ONLY for committees
    // we have joined by a nick change.
    set<mstring> wason;
    map<mstring, Committee>::iterator iter2;
    for (iter2 = Parent->commserv.list.begin(); iter2 != Parent->commserv.list.end();
								iter2++)
    {
	if (iter2->second.IsOn(i_Name))
	    wason.insert(iter2->first);
    }

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
	    Log(LM_ERROR, Parent->getLogMessage("REC_FORNOTINCHAN"),
		"NICK", i_Name.c_str(), iter->c_str());
	}
    }

    // Clean up non-existant channels ...
    for (i=0; i<chunked.size(); i++)
	joined_channels.erase(chunked[i]);

    vector<unsigned long> dccs = Parent->dcc->GetList(i_Name);
    for (i=0; i<dccs.size(); i++)
	Parent->dcc->xfers[dccs[i]]->ChgNick(in);

    // Carry over failed attempts (so /nick doesnt cure all!)
    // We dont care if it doesnt exist, they can drop channels *shrug*
    for (i=0; i<try_chan_ident.size(); i++)
    {
	if (Parent->chanserv.IsStored(try_chan_ident[i]))
	    Parent->chanserv.stored[try_chan_ident[i]].ChgAttempt(i_Name, in);
    }

    if (Parent->nickserv.IsStored(i_Name))
    {
	// We are not related (by brotherhood, or parentage)
	if (!(Parent->nickserv.stored[i_Name.LowerCase()].IsSibling(in) ||
	    Parent->nickserv.stored[i_Name.LowerCase()].Host().LowerCase() == i_Name.LowerCase()))
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
	if (Parent->nickserv.stored[i_Name.LowerCase()].Forbidden())
	{
	    Parent->nickserv.send(Parent->nickserv.FirstName(),
		i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/FORBIDDEN"),
		ToHumanTime(Parent->nickserv.Ident()).c_str());
	    return;
	}
	else if (Parent->nickserv.stored[i_Name.LowerCase()].IsOnline())
	    Parent->nickserv.stored[i_Name.LowerCase()].Signon(i_realname, Mask(U_P_H).After("!"));
	else if (Parent->nickserv.stored[i_Name.LowerCase()].Protect())
	    Parent->nickserv.send(Parent->nickserv.FirstName(),
		i_Name, Parent->getMessage(i_Name, "ERR_SITUATION/PROTECTED"),
		ToHumanTime(Parent->nickserv.Ident()).c_str());
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
    else if (Parent->server.proto.SVS())
    {
	Parent->server.SVSMODE(Parent->nickserv.FirstName(), i_Name, in);
    }
}


void Nick_Live_t::Mode(mstring in)
{
    FT("Nick_Live_t::Mode", (in));

    bool add = true;
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "modes"));
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
		RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
		Parent->operserv.RemHost(i_host);
		MLOCK(("OperServ", "OperDeny"));
		// IF we are SecureOper and NOT on oper list
		// OR user is on OperDeny and NOT on sadmin list
		if ((Parent->operserv.SecureOper() &&
		    !(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		    Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(i_Name))) ||
		    (Parent->operserv.OperDeny_find(Mask(N_U_P_H)) &&
		    !IsServices() && !(Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
		    Parent->commserv.list[Parent->commserv.SADMIN_Name()].IsOn(i_Name))))
		{
		    if (Parent->server.proto.SVS())
		    {
			SendMode("-oAa");
			send(Parent->operserv.FirstName(), i_Name,
				Parent->getMessage(i_Name, "OS_STATUS/ISOPERDENY"));
		    }
		    else
		    {
			Parent->server.KILL(Parent->operserv.FirstName(),
		    	    i_Name, Parent->getMessage(i_Name, "MISC/KILL_OPERDENY"));
			return;
		    }
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
		Log(LM_TRACE, Parent->getLogMessage("ERROR/MODE_INEFFECT"),
			add ? '+' : '-', in[i], i_Name.c_str(), i_Name.c_str());
	    }
	    break;
	}
    }    
}


mstring Nick_Live_t::Mode()
{
    NFT("Nick_Live_t::Mode");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "modes"));
    RET(modes);
}

bool Nick_Live_t::HasMode(mstring in)
{
    FT("Nick_Live_t::HasMode", (in));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "modes"));
    RET(modes.Contains(in));
}

void Nick_Live_t::Away(mstring in)
{
    FT("Nick_Live_t::Away", (in));
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_away"));
    i_away = in;
}

mstring Nick_Live_t::Away()
{
    NFT("Nick_Live_t::Away");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_away"));
    RET(i_away);
}

mDateTime Nick_Live_t::IdleTime()
{
    NFT("Nick_Live_t::IdleTime");
    mDateTime tmp;
    if (IsServices())
    {
	RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_Last_Action"));
	tmp = time(NULL) - (time_t) i_Last_Action;
    }
    RET(tmp);
}


void Nick_Live_t::Action()
{
    NFT("Nick_Live_t::Action");
    if (IsServices())
    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_Last_Action"));
	i_Last_Action = Now();
    }
}


mDateTime Nick_Live_t::SignonTime()
{
    NFT("Nick_Live_t::SignonTime");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_Signon_Time"));
    RET(i_Signon_Time);
}

mDateTime Nick_Live_t::MySignonTime()
{
    NFT("Nick_Live_t::MySignonTime");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_My_Signon_Time"));
    RET(i_My_Signon_Time);
}

mstring Nick_Live_t::RealName()
{
    NFT("Nick_Live_t::RealName");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_realname"));
    RET(i_realname);
}

mstring Nick_Live_t::User()
{
    NFT("Nick_Live_t::User");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_user"));
    RET(i_user);
}

mstring Nick_Live_t::Host()
{
    NFT("Nick_Live_t::Host");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    RET(i_host);
}

mstring Nick_Live_t::AltHost()
{
    NFT("Nick_Live_t::AltHost");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_alt_host"));
    RET(i_alt_host);
}

void Nick_Live_t::AltHost(mstring in)
{
    FT("Nick_Live_t::AltHost", (in));
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_alt_host"));
    i_alt_host = in;
}

mstring Nick_Live_t::Server()
{
    NFT("Nick_Live_t::Server");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_server"));
    RET(i_server);
} 

mstring Nick_Live_t::Squit()
{
    NFT("Nick_Live_t::Squit");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_squit"));
    RET(i_squit);
}


void Nick_Live_t::SetSquit()
{
    NFT("Nick_Live_t::SetSquit");
    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_squit"));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_server"));
    i_squit = i_server;
    }

    { RLOCK2(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    if (!IsServices())
	Parent->operserv.RemHost(i_host);
    }

    set<mstring>::iterator i;
    RLOCK3(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    for (i=joined_channels.begin(); i!=joined_channels.end(); i++)
	if (Parent->chanserv.IsLive(*i))
	    Parent->chanserv.live[i->LowerCase()].Squit(i_Name);
	else
	    Log(LM_ERROR, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
		"SQUIT", i_Name.c_str(), i->c_str());
}


void Nick_Live_t::ClearSquit()
{
    NFT("Nick_Live_t::ClearSquit");
    { WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_squit"));
    i_squit = "";
    }

    // These will all be set again
    { WLOCK2(("NickServ", "live", i_Name.LowerCase(), "modes"));
    modes = "";
    }

    { WLOCK3(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    set<mstring>::iterator i;
    for (i=joined_channels.begin(); i!=joined_channels.end(); i++)
	if (Parent->chanserv.IsLive(*i))
	    Parent->chanserv.live[i->LowerCase()].UnSquit(i_Name);
	else
	    Log(LM_ERROR, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
		"UNSQUIT", i_Name.c_str(), i->c_str());

    joined_channels.clear();
    }

    { RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    if (!IsServices())
	if (Parent->operserv.AddHost(i_host))
	{
	    Log(LM_INFO, Parent->getLogMessage("OTHER/KILL_CLONE"),
		Mask(N_U_P_H).c_str());
	    Parent->server.KILL(Parent->nickserv.FirstName(), i_Name,
			Parent->operserv.Def_Clone());
	    return;
	}
    }
}


mstring Nick_Live_t::Mask(Nick_Live_t::styles type)
{
    FT("Nick_Live_t::Mask", ((int) type));

    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_user"));
    RLOCK2(("NickServ", "live", i_Name.LowerCase(), "i_host"));
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
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
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
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
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
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
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
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
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


mstring Nick_Live_t::AltMask(Nick_Live_t::styles type)
{
    FT("Nick_Live_t::AltMask", ((int) type));

    RLOCK2(("NickServ", "live", i_Name.LowerCase(), "i_user"));
    RLOCK3(("NickServ", "live", i_Name.LowerCase(), "i_alt_host"));
    mstring retval;
    mstring user = i_user;
    switch (type)
    {
    case N:		// nick!*@*
	retval = i_Name + "!*@*";
	break;

    case N_U_P_H:	// nick!user@port.host
	retval = i_Name + "!" + user + "@" + i_alt_host;
	break;

    case N_U_H:		// nick!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_alt_host.IsNumber())
	    retval = i_Name + "!*" + user + "@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = i_Name + "!*" + user + "@*." + i_alt_host.After(".");
	else
	    retval = i_Name + "!*" + user + "@" + i_alt_host;
	break;

    case N_P_H:		// nick!*@port.host
	retval = i_Name + "!*@" + i_alt_host;
	break;

    case N_H:		// nick!*@*.host
	if (i_alt_host.IsNumber())
	    retval = i_Name + "!*@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = i_Name + "!*@*." + i_alt_host.After(".");
	else
	    retval = i_Name + "!*@" + i_alt_host;
	break;

    case U_P_H:		// *!user@port.host
	retval = "*!" + user + "@" + i_alt_host;
	break;

    case U_H:		// *!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_alt_host.IsNumber())
	    retval = "*!*" + user + "@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = "*!*" + user + "@*." + i_alt_host.After(".");
	else
	    retval = "*!*" + user + "@" + i_alt_host;
	break;

    case P_H:		// *!*@port.host
	retval = "*!*@" + i_alt_host;
	break;

    case H:		// *!*@*.host
	if (i_alt_host.IsNumber())
	    retval = "*!*@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = "*!*@*." + i_alt_host.After(".");
	else
	    retval = "*!*@" + i_alt_host;
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
    mstring retval;
    if (Parent->chanserv.IsStored(channel))
    {
	unsigned int failtimes = Parent->chanserv.stored[channel.LowerCase()].CheckPass(i_Name, password);
	if (!failtimes)
	{
	    WLOCK(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
	    chans_founder_identd.insert(channel.LowerCase());
	    retval = Parent->getMessage(i_Name, "CS_COMMAND/IDENTIFIED");
	}
	else
	{
	    // Keep record of all channel attempts, so that when
	    // we change nick or quit, we can let chanserv know.
	    vector<mstring>::iterator iter;
	    WLOCK(("NickServ", "live", i_Name.LowerCase(), "try_chan_ident"));
	    for (iter=try_chan_ident.begin(); iter!=try_chan_ident.end(); iter++) ;
	    if (iter == try_chan_ident.end())
		try_chan_ident.push_back(channel.LowerCase());

	    if (failtimes >= Parent->chanserv.Passfail())
	    {
		Parent->server.KILL(Parent->nickserv.FirstName(), i_Name,
			Parent->getMessage(i_Name, "MISC/KILL_PASS_FAIL"));
		Log(LM_NOTICE, Parent->getLogMessage("OTHER/KLLL_CHAN_PASS"),
			Mask(N_U_P_H).c_str(), channel.c_str());
		RET("");
	    }
	    else
	    {
		retval = Parent->getMessage(i_Name, "ERR_SITUATION/CHAN_WRONG_PASS");
	    }
	}
    }
    else
    {
	retval = Parent->getMessage(i_Name, "CS_STATUS/ISNOTSTORED");
    }
    RET(retval);
}


void Nick_Live_t::UnChanIdentify(mstring channel)
{
    FT("Nick_Live_t::UnChanIdentify", (channel));

    if (IsChanIdentified(channel))
    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
	chans_founder_identd.erase(channel.LowerCase());
    }
}

bool Nick_Live_t::IsChanIdentified(mstring channel)
{
    FT("Nick_Live_t::IsChanIdentified", (channel));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
    bool retval = (chans_founder_identd.find(channel.LowerCase())!=chans_founder_identd.end());
    RET(retval);
}


mstring Nick_Live_t::Identify(mstring password)
{
    FT("Nick_Live_t::Identify", (password));
    mstring retval;
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
    if (identified == true)
    {
	retval = Parent->getMessage(i_Name, "NS_YOU_STATUS/IDENTIFIED");
    }
    else if (Parent->nickserv.IsStored(i_Name))
    {
	WLOCK2(("NickServ", "live", i_Name.LowerCase(), "failed_passwds"));
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
	    retval = Parent->getMessage(i_Name, "NS_YOU_COMMAND/IDENTIFIED");
	}
	else
	{
	    failed_passwds++;
	    if (failed_passwds >= Parent->nickserv.Passfail())
	    {
		Parent->server.KILL(Parent->nickserv.FirstName(), i_Name,
			Parent->getMessage(i_Name, "MISC/KILL_PASS_FAIL"));
		Log(LM_NOTICE, Parent->getLogMessage("OTHER/KLLL_NICK_PASS"),
			Mask(N_U_P_H).c_str(), i_Name.c_str());
		RET("");
	    }
	    else
	    {
		retval = Parent->getMessage(i_Name, "ERR_SITUATION/NICK_WRONG_PASS");
	    }
	}
    }
    else
    {
	retval = Parent->getMessage(i_Name, "NS_YOU_STATUS/ISNOTSTORED");
    }
    RET(retval);
}

void Nick_Live_t::UnIdentify()
{
    NFT("Nick_Live_t::UnIdentify");
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
    identified = false;
}

bool Nick_Live_t::IsIdentified()
{
    NFT("Nick_Live_t::IsIdentified");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
    RET(identified);
}

bool Nick_Live_t::IsRecognized()
{
    NFT("Nick_Live_t::IsRecognised");
    bool retval = false;
    if (Parent->nickserv.IsStored(i_Name))
    {
	retval = Parent->nickserv.stored[i_Name.LowerCase()].IsAccess(Mask(U_P_H).After("!"));
    }
    RET(retval);
}

bool Nick_Live_t::IsServices()
{
    NFT("Nick_Live_t::IsServices");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "services"));
    RET(services);
}

size_t Nick_Live_t::Usage()
{
    size_t retval = 0;

    // We write lock here coz its the only way to
    // ensure the values are NOT going to change.
    WLOCK(("NickServ", "live", i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += InFlight.Usage();
    retval += sizeof(i_Signon_Time.Internal());
    retval += sizeof(i_My_Signon_Time.Internal());
    retval += sizeof(i_Last_Action.Internal());
    retval += i_realname.capacity();
    retval += i_user.capacity();
    retval += i_host.capacity();
    retval += i_alt_host.capacity();
    retval += i_server.capacity();
    retval += i_squit.capacity();
    retval += i_away.capacity();
    retval += modes.capacity();
    set<mstring>::iterator i;
    for (i=joined_channels.begin(); i!=joined_channels.end(); i++)
    {
	retval += i->capacity();
    }
    vector<mDateTime>::iterator j;
    for (j=last_msg_times.begin(); j!=last_msg_times.end(); j++)
    {
	retval += sizeof(j->Internal());
    }
    retval += sizeof(last_msg_entries);
    retval += sizeof(flood_triggered_times);
    retval += sizeof(failed_passwds);
    for (i=chans_founder_identd.begin(); i!=chans_founder_identd.end(); i++)
    {
	retval += i->capacity();
    }
    vector<mstring>::iterator k;
    for (k=try_chan_ident.begin(); k!=try_chan_ident.end(); k++)
    {
	retval += k->capacity();
    }
    retval += sizeof(identified);
    retval += sizeof(services);

    return retval;
}

// =======================================================================


void Nick_Stored_t::Signon(mstring realname, mstring mask)
{
    FT("Nick_Stored_t::Signon", (realname, mask));
    { WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastRealName"));
    WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_LastMask"));
    i_LastRealName = realname;
    i_LastMask = mask;
    }

    if (Parent->memoserv.IsNick(i_Name))
    {
	list<Memo_t>::iterator iter;
	unsigned int count = 0;
	for (iter = Parent->memoserv.nick[i_Name.LowerCase()].begin();
		iter != Parent->memoserv.nick[i_Name.LowerCase()].end();
		iter++)
	{
	    if (!iter->IsRead())
		count++;
	}
	if (count)
	    send(Parent->memoserv.FirstName(), i_Name,
		Parent->getMessage(i_Name, "MS_STATUS/NS_UNREAD"),
		count, Parent->memoserv.FirstName().c_str());
    }
}


void Nick_Stored_t::ChgNick(mstring nick)
{
    FT("Nick_Stored_t::ChgNick", (nick));
    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastQuit"));
    WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
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
    WLOCK(("NickServ", "stored", i_Name.LowerCase()));
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
    WLOCK(("NickServ", "stored", i_Name.LowerCase()));
    i_Forbidden = true;
    i_Picture = 0;
    i_RegTime = Now();
} 


Nick_Stored_t::Nick_Stored_t(mstring nick, mDateTime regtime, const Nick_Stored_t &in)
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick, "(const Nick_Stored_t &) in"));
    i_Name = nick;
    WLOCK(("NickServ", "stored", i_Name.LowerCase()));
    i_RegTime = regtime;
    i_Forbidden = false;
    i_Picture = 0;
    i_Host = in.i_Name.LowerCase();

    if (Parent->nickserv.IsLive(i_Name))
    {
	i_LastRealName = Parent->nickserv.live[i_Name.LowerCase()].RealName();
	i_LastMask = Parent->nickserv.live[i_Name.LowerCase()].Mask(Nick_Live_t::U_P_H).After("!");
        Parent->nickserv.live[i_Name.LowerCase()].Identify(Password());
    }
}


mDateTime Nick_Stored_t::RegTime()
{
    NFT("Nick_Stored_t::RegTime");
    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_RegTime"));
    RET(i_RegTime);
}

unsigned long Nick_Stored_t::Drop()
{
    NFT("Nick_Stored_t::Drop");

    // When we go, we take all our slaves with us!
    unsigned long i, dropped = 1;
    if (Host() == "")
    {
	while(Siblings())
	{
	    mstring nick = Sibling(0);
	    if (nick != "")
	    {
		
		dropped += Parent->nickserv.stored[nick.LowerCase()].Drop();
		WLOCK(("NickServ", "stored"));
		Parent->nickserv.stored.erase(nick.LowerCase());
	    }
	}
    }
    else
    {
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	Parent->nickserv.stored[i_Host.LowerCase()].i_slaves.erase(i_Name.LowerCase());
    }

    // Now we go for our channels ...
    vector<mstring> killchans;
    map<mstring, Chan_Stored_t>::iterator iter;
    for (iter = Parent->chanserv.stored.begin();
	    iter != Parent->chanserv.stored.end(); iter++)
    {
	if (iter->second.Founder().CmpNoCase(i_Name) == 0)
	{
	    if (iter->second.CoFounder() != "" &&
		Parent->nickserv.IsStored(iter->second.CoFounder()))
	    {
		iter->second.Founder(iter->second.CoFounder());
	    }
	    else
		killchans.push_back(iter->first);
	}
    }

    WLOCK3(("ChanServ", "stored"));
    for (i=0; i<killchans.size(); i++)
    {
	Parent->chanserv.stored.erase(killchans[i]);
    }

    RET(dropped);
}


void Nick_Stored_t::operator=(const Nick_Stored_t &in)
{
    NFT("Nick_Stored_t::operator=");
    WLOCK(("NickServ", "stored", in.i_Name.LowerCase()));
    i_Name=in.i_Name;
    i_RegTime=in.i_RegTime;
    i_Password=in.i_Password;
    i_Email=in.i_Email;
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Email"));
	RET(i_Email);
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].Email();
	RET(retval);
    }
}


void Nick_Stored_t::Email(mstring in)
{
    FT("Nick_Stored_t::Email", (in));
    if (Host() == "")
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Email"));
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_URL"));
	RET(i_URL);
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].URL();
	RET(retval);
    }
}


void Nick_Stored_t::URL(mstring in)
{
    FT("Nick_Stored_t::URL", (in));
    if (Host() == "")
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_URL"));
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ICQ"));
	RET(i_ICQ);
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].ICQ();
	RET(retval);
    }
}


void Nick_Stored_t::ICQ(mstring in)
{
    FT("Nick_Stored_t::ICQ", (in));
    if (Host() == "")
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ICQ"));
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Description"));
	RET(i_Description);
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].Description();
	RET(retval);
    }
}


void Nick_Stored_t::Description(mstring in)
{
    FT("Nick_Stored_t::Description", (in));
    if (Host() == "")
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Description"));
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Comment"));
	RET(i_Comment);
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].Comment();
	RET(retval);
    }
}


void Nick_Stored_t::Comment(mstring in)
{
    FT("Nick_Stored_t::Comment", (in));
    if (Host() == "")
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Comment"));
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
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_Time"));
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
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
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
    mstring retval;
    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Host"));
    if (i_Host != "" && !Parent->nickserv.IsStored(i_Host))
    {
	Log(LM_ERROR, Parent->getLogMessage("ERROR/HOST_NOTREGD"),
		i_Host.c_str(), i_Name.c_str());
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Host"));
	i_Host = "";
    }
    else
	retval = Parent->nickserv.stored[i_Host.LowerCase()].Name();
    RET(retval);
}

mstring Nick_Stored_t::Password()
{
    NFT("Nick_Stored_t::Password");
    if (Host() == "")
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Password"));
	RET(i_Password);
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].Password();
	RET(retval);
    }
}


void Nick_Stored_t::Password(mstring in)
{
    FT("Nick_Stored_t::Password", (in));
    if (Host() == "")
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Password"));
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Forbidden"));
	RLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_Password"));
	CP(("Checking \"%s\" == \"%s\" ...", password.c_str(), i_Password.c_str()));
	if (i_Forbidden  || password != i_Password)
	{
	    RET(false);
	}

	if (Parent->nickserv.IsStored(nick))
	{
	    ChangeOver(nick);
	    WLOCK(("NickServ", "stored"));
	    Parent->nickserv.stored.erase(nick.LowerCase());
	}

	Parent->nickserv.stored[nick.LowerCase()] = Nick_Stored_t(nick, regtime, *this);
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	i_slaves.insert(nick.LowerCase());
	RET(true);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].Slave(nick, password, regtime);
	RET(retval);
    }
}


unsigned int Nick_Stored_t::Siblings()
{
    NFT("Nick_Stored_t::Siblings");
    unsigned int retval = 0;
    if (Host() == "")
    {
	retval = i_slaves.size();
    }
    else
    {
	retval = Parent->nickserv.stored[i_Host.LowerCase()].Siblings();
    }
    RET(retval);
}


mstring Nick_Stored_t::Sibling(unsigned int count)
{
    FT("Nick_Stored_t::Siblings", (count));
    mstring retval = "";
    if (Host() == "")
    {
	vector<mstring> chunked;
	set<mstring>::iterator iter;
	unsigned int i;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	for (i=0, iter=i_slaves.begin(); iter!=i_slaves.end(); iter++, i++)
	{
	    if (Parent->nickserv.IsStored(*iter))
	    {
		if (i==count)
		{
		    retval = iter->LowerCase();
		    break;
		}
	    }
	    else
	    {
		Log(LM_ERROR, Parent->getLogMessage("ERROR/SLAVE_NOTREGD"),
			iter->c_str(), i_Name.c_str());
		chunked.push_back(iter->c_str());
		i--;
	    }
	}
	for (i=0; i<chunked.size(); i++)
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	    i_slaves.erase(chunked[i]);
	}
    }
    else
    {
	retval = Parent->nickserv.stored[i_Host.LowerCase()].Sibling(count);
    }
    RET(retval);
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	bool retval = (i_slaves.find(nick.LowerCase()) != i_slaves.end());
	if (retval && !Parent->nickserv.IsStored(nick))
	{
	    Log(LM_ERROR, Parent->getLogMessage("ERROR/SLAVE_NOTREGD"),
			nick.c_str(), i_Name.c_str());
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	    i_slaves.erase(nick);
	    retval = false;
	}
	RET(retval);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].IsSibling(nick);
	RET(retval);
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
	    if (!(citer->second.HeadCom() != "") &&
		(citer->second.IsHead(i_Name) || citer->second.IsHead(oldnick)))
	    {
		citer->second.Head(i_Name);
		found = false;
	    }
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
	if (csiter->second.CoFounder().LowerCase() == oldnick.LowerCase())
	{
	    csiter->second.CoFounder(i_Name);
	}
	if (csiter->second.Founder().LowerCase() == oldnick.LowerCase())
	{
	    csiter->second.Founder(i_Name);
	}
	{ MLOCK(("ChanServ", "stored", csiter->first, "Access"));
	found = false;
	valueL = 0;
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
	    if (csiter->second.Access->Value() > valueL)
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
	    niter->second.Host().LowerCase() != i_Name.LowerCase() &&
	    !IsSibling(niter->first) &&
	    niter->second.IsIgnore(oldnick))
	{
	    niter->second.IgnoreDel(oldnick);
	    niter->second.IgnoreAdd(i_Name);
	}
    }

    if (Parent->memoserv.IsNick(oldnick))
    {
	WLOCK(("MemoServ", "nick"));
	list<Memo_t>::iterator miter;
	for (miter=Parent->memoserv.nick[oldnick.LowerCase()].begin();
		miter!=Parent->memoserv.nick[oldnick.LowerCase()].end();
		miter++)
	    miter->ChgNick(i_Name);
	if (Parent->memoserv.IsNick(i_Name))
	{
	    Parent->memoserv.nick[i_Name.LowerCase()].insert(
		Parent->memoserv.nick[i_Name.LowerCase()].end(),
		Parent->memoserv.nick[oldnick.LowerCase()].begin(),
		Parent->memoserv.nick[oldnick.LowerCase()].end());
	}
	else
	{
	    Parent->memoserv.nick[i_Name.LowerCase()] =
		Parent->memoserv.nick[oldnick.LowerCase()];
	}
	Parent->memoserv.nick.erase(oldnick.LowerCase());
    }

    map<mstring, list<News_t> >::iterator cniter;
    list<News_t>::iterator cnliter;
    for (cniter = Parent->memoserv.channel.begin();
			cniter != Parent->memoserv.channel.end(); cniter++)
    {
	WLOCK(("MemoServ", "channel", cniter->first));
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
	WLOCK(("NickServ", "stored", i_Name.LowerCase()));
	WLOCK2(("NickServ", "stored", i_Host.LowerCase()));
	for (unsigned int i=0; i<Parent->nickserv.stored[i_Host.LowerCase()].Siblings(); i++)
	{
	    if (Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i).LowerCase() != i_Name.LowerCase())
	    {
		i_slaves.insert(Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i));
		Parent->nickserv.stored[Parent->nickserv.stored[i_Host.LowerCase()].Sibling(i)].i_Host = i_Name.LowerCase();
	    }
	}
	i_slaves.insert(i_Host.LowerCase());
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
	Parent->nickserv.stored[i_Host.LowerCase()].i_Host = i_Name.LowerCase();
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
	RET(false);
    }
    else
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase()));
	WLOCK2(("NickServ", "stored", i_Host.LowerCase()));
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
    unsigned int retval = 0;
    if (Host() == "")
    {
	retval = i_access.size();
    }
    else
    {
	retval = Parent->nickserv.stored[i_Host.LowerCase()].Access();
    }
    RET(retval);
}


mstring Nick_Stored_t::Access(unsigned int count)
{
    FT("Nick_Stored_t::Access", (count));
    if (Host() == "")
    {
	set<mstring>::iterator iter;
	unsigned int i;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
	for (i=0, iter=i_access.begin(); iter!=i_access.end(); i++, iter++)
	    if (i==count)
	    {
		RET(*iter);
	    }
	RET("");
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].Access(count);
	RET(retval);
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
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
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
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].AccessAdd(in);
	RET(retval);
    }
}


unsigned int Nick_Stored_t::AccessDel(mstring in)
{
    FT("Nick_Stored_t::AccessDel", (in));
    unsigned int retval = 0;
    if (Host() == "")
    {
	vector<mstring> chunked;
	set<mstring>::iterator iter;
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		chunked.push_back(*iter);
	    }

	for (unsigned int i=0; i<chunked.size(); i++)
	    i_access.erase(chunked[i].LowerCase());
	retval = chunked.size();
    }
    else
    {
	retval = Parent->nickserv.stored[i_Host.LowerCase()].AccessDel(in);
    }
    RET(retval);
}


bool Nick_Stored_t::IsAccess(mstring in)
{
    FT("Nick_Stored_t::IsAccess", (in));
    if (Host() == "")
    {
	set<mstring>::iterator iter;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
	for (iter=i_access.begin(); iter!=i_access.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		RET(true);
	    }
	RET(false);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].IsAccess(in);
	RET(retval);
    }
}


unsigned int Nick_Stored_t::Ignore()
{
    NFT("Nick_Stored_t::Ignore");
    unsigned int retval = 0;
    if (Host() == "")
    {
	retval = i_ignore.size();
    }
    else
    {
	retval = Parent->nickserv.stored[i_Host.LowerCase()].Ignore();
    }
    RET(retval);
}


mstring Nick_Stored_t::Ignore(unsigned int count)
{
    FT("Nick_Stored_t::Ignore", (count));
    if (Host() == "")
    {
	set<mstring>::iterator iter;
	unsigned int i;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
	for (i=0, iter=i_ignore.begin(); iter!=i_ignore.end(); i++, iter++)
	    if (i==count)
	    {
		RET(*iter);
	    }
	RET("");
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].Ignore(count);
	RET(retval);
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
	
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
	if (i_ignore.find(in.LowerCase())!=i_ignore.end())
	{
	    RET(false);
	}

	i_ignore.insert(in.LowerCase());
	RET(true);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].IgnoreAdd(in);
	RET(retval);
    }
}


unsigned int Nick_Stored_t::IgnoreDel(mstring in)
{
    FT("Nick_Stored_t::IgnoreDel", (in));
    unsigned int retval = 0;
    if (Host() == "")
    {
	vector<mstring> chunked;
	set<mstring>::iterator iter;
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
	for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		chunked.push_back(*iter);
	    }

	for (unsigned int i=0; i<chunked.size(); i++)
	    i_ignore.erase(chunked[i].LowerCase());
	retval = chunked.size();
    }
    else
    {
	retval = Parent->nickserv.stored[i_Host.LowerCase()].IgnoreDel(in);
    }
    RET(retval);
}


bool Nick_Stored_t::IsIgnore(mstring in)
{
    FT("Nick_Stored_t::IsIgnore", (in));
    if (Host() == "")
    {
	set<mstring>::iterator iter;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
	for (iter=i_ignore.begin(); iter!=i_ignore.end(); iter++)
	    if (in.LowerCase().Matches(*iter))
	    {
		RET(true);
	    }
	RET(false);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].IsIgnore(in);
	RET(retval);
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Protect"));
	RET(i_Protect);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].Protect();
	RET(retval);
    }
}


void Nick_Stored_t::Protect(bool in)
{
    FT("Nick_Stored_t::Protect", (in));
    if (Host() == "")
    {
	if (!L_Protect())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Protect"));
	    i_Protect = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Protect"));
	RET(l_Protect);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].L_Protect();
	RET(retval);
    }
}


void Nick_Stored_t::L_Protect(bool in)
{
    FT("Nick_Stored_t::L_Protect", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_Protect())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Protect"));
	    l_Protect = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Secure"));
	RET(i_Secure);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].Secure();
	RET(retval);
    }
}


void Nick_Stored_t::Secure(bool in)
{
    FT("Nick_Stored_t::Secure", (in));
    if (Host() == "")
    {
	if (!L_Secure())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Secure"));
	    i_Secure = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Secure"));
	RET(l_Secure);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].L_Secure();
	RET(retval);
    }
}


void Nick_Stored_t::L_Secure(bool in)
{
    FT("Nick_Stored_t::L_Secure", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_Secure())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Secure"));
	    l_Secure = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_NoExpire"));
	RET(i_NoExpire);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].NoExpire();
	RET(retval);
    }
}


void Nick_Stored_t::NoExpire(bool in)
{
    FT("Nick_Stored_t::NoExpire", (in));
    if (Host() == "")
    {
	if (!L_NoExpire())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_NoExpire"));
	    i_NoExpire = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_NoExpire"));
	RET(l_NoExpire);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].L_NoExpire();
	RET(retval);
    }
}


void Nick_Stored_t::L_NoExpire(bool in)
{
    FT("Nick_Stored_t::L_NoExpire", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_NoExpire())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_NoExpire"));
	    l_NoExpire = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_NoMemo"));
	RET(i_NoMemo);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].NoMemo();
	RET(retval);
    }
}


void Nick_Stored_t::NoMemo(bool in)
{
    FT("Nick_Stored_t::NoMemo", (in));
    if (Host() == "")
    {
	if (!L_NoMemo())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_NoMemo"));
	    i_NoMemo = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_NoMemo"));
	RET(l_NoMemo);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].L_NoMemo();
	RET(retval);
    }
}


void Nick_Stored_t::L_NoMemo(bool in)
{
    FT("Nick_Stored_t::L_NoMemo", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_NoMemo())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_NoMemo"));
	    l_NoMemo = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Private"));
	RET(i_Private);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].Private();
	RET(retval);
    }
}


void Nick_Stored_t::Private(bool in)
{
    FT("Nick_Stored_t::Private", (in));
    if (Host() == "")
    {
	if (!L_Private())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Private"));
	    i_Private = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Private"));
	RET(l_Private);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].L_Private();
	RET(retval);
    }
}


void Nick_Stored_t::L_Private(bool in)
{
    FT("Nick_Stored_t::L_Private", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_Private())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Private"));
	    l_Private = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_PRIVMSG"));
	RET(i_PRIVMSG);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].PRIVMSG();
	RET(retval);
    }
}


void Nick_Stored_t::PRIVMSG(bool in)
{
    FT("Nick_Stored_t::PRIVMSG", (in));
    if (Host() == "")
    {
	if (!L_PRIVMSG())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_PRIVMSG"));
	    i_PRIVMSG = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_PRIVMSG"));
	RET(l_PRIVMSG);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].L_PRIVMSG();
	RET(retval);
    }
}


void Nick_Stored_t::L_PRIVMSG(bool in)
{
    FT("Nick_Stored_t::L_PRIVMSG", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_PRIVMSG())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_PRIVMSG"));
	    l_PRIVMSG = in;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Language"));
	RET(i_Language);
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].Language();
	RET(retval);
    }
}


void Nick_Stored_t::Language(mstring in)
{
    FT("Nick_Stored_t::Language", (in));
    if (Host() == "")
    {
	if (!L_Language())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Language"));
	    i_Language = in.UpperCase();
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Language"));
	RET(l_Language);
    }
    else
    {
	bool retval = Parent->nickserv.stored[i_Host.LowerCase()].L_Language();
	RET(retval);
    }
}


void Nick_Stored_t::L_Language(bool in)
{
    FT("Nick_Stored_t::L_Language", (in));
    if (Host() == "")
    {
	if (!Parent->nickserv.LCK_Language())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "l_Language"));
	    l_Language = in;
	}
    }
    else
    {
	Parent->nickserv.stored[i_Host.LowerCase()].L_Language(in);
    }
}


bool Nick_Stored_t::Suspended()
{
    NFT("Nick_Stored_t::Suspended");
    RET(Suspend_By() != "");
}


mstring Nick_Stored_t::Suspend_By()
{
    NFT("Nick_Stored_t::Suspend_By");
    if (Host() == "")
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
	RET(i_Suspend_By);
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].Suspend_By();
	RET(retval);
    }
}


mDateTime Nick_Stored_t::Suspend_Time()
{
    NFT("Nick_Stored_t::Suspend_Time");
    if (Host() == "")
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_Time"));
	RET(i_Suspend_Time);
    }
    else
    {
	mDateTime retval = Parent->nickserv.stored[i_Host.LowerCase()].Suspend_Time();
	RET(retval);
    }
}


bool Nick_Stored_t::Forbidden()
{
    NFT("Nick_Stored_t::Forbidden");
    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Forbidden"));
    RET(i_Forbidden);
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
	if (Parent->nickserv.PicExt() != "")
	{
	    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Picture"));
	    RET(i_Picture);
	}
	RET(0);
    }
    else
    {
	unsigned long retval = Parent->nickserv.stored[i_Host.LowerCase()].PicNum();
	RET(retval);
    }
}


void Nick_Stored_t::GotPic(unsigned long picnum)
{
    FT("Nick_Stored_t::GotPic", (picnum));
    if (Host() == "")
    {
	if (Parent->nickserv.PicExt() != "")
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Picture"));
	    i_Picture = picnum;
	}
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	mDateTime lastseen = i_LastSeenTime;
	unsigned int i;
	for (i=0; i<Siblings(); i++)
	{
	    if (Parent->nickserv.stored[Sibling(i).LowerCase()].LastSeenTime() > lastseen)
		lastseen = Parent->nickserv.stored[Sibling(i).LowerCase()].LastSeenTime();
	}
	RET(lastseen);
    }
    else
    {
	mDateTime retval = Parent->nickserv.stored[i_Host.LowerCase()].LastAllSeenTime();
	RET(retval);
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	RET(i_LastSeenTime);
    }
}


mstring Nick_Stored_t::LastRealName()
{
    NFT("Nick_Stored_t::LastRealName");
    if (IsOnline())
    {
	mstring retval = Parent->nickserv.live[i_Name.LowerCase()].RealName();
	RET(retval);
    }
    else
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastRealName"));
	RET(i_LastRealName);
    }
}


mstring Nick_Stored_t::LastAllMask()
{
    NFT("Nick_Stored_t::LastAllMask");
    if (IsOnline())
    {
	RET(Parent->getMessage("MISC/ONLINE"));
    }
    else if (Host() == "")
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	mDateTime lastseen = i_LastSeenTime;
	mstring lastmask = Name() + "!" + LastMask();
	unsigned int i;
	for (i=0; i<Siblings(); i++)
	{
	    if (Parent->nickserv.stored[Sibling(i)].IsOnline())
	    {
		RET(Parent->getMessage("MISC/ONLINE"));
	    }
	    if (Parent->nickserv.stored[Sibling(i)].LastSeenTime() > lastseen)
	    {
		lastseen = Parent->nickserv.stored[Sibling(i)].LastSeenTime();
		lastmask = Parent->nickserv.stored[Sibling(i)].Name() + "!" +
				Parent->nickserv.stored[Sibling(i)].LastMask();
	    }
	}
	RET(lastmask);
    }
    else
    {
	mstring retval = Parent->nickserv.stored[i_Host.LowerCase()].LastAllMask();
	RET(retval);
    }
}


mstring Nick_Stored_t::LastMask()
{
    NFT("Nick_Stored_t::LastMask");
    if (IsOnline())
    {
	RET(Parent->getMessage("MISC/ONLINE"));
    }
    else
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastMask"));
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
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastQuit"));
	RET(i_LastQuit);
    }
}


void Nick_Stored_t::Quit(mstring message)
{
    FT("Nick_Stored_t::Quit", (message));
    
    // Dont whinge here, because they may not be
    // ident'd, in which case IsOnline() == false.
    if (IsOnline())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_LastQuit"));
	i_LastSeenTime = Now();
	i_LastQuit = message;
    }
}


SXP::Tag Nick_Stored_t::tag_Nick_Stored_t("Nick_Stored_t");
SXP::Tag Nick_Stored_t::tag_Name("Name");
SXP::Tag Nick_Stored_t::tag_RegTime("RegTime");
SXP::Tag Nick_Stored_t::tag_Password("Password");
SXP::Tag Nick_Stored_t::tag_Email("EMail");
SXP::Tag Nick_Stored_t::tag_URL("URL");
SXP::Tag Nick_Stored_t::tag_ICQ("ICQ");
SXP::Tag Nick_Stored_t::tag_Description("Description");
SXP::Tag Nick_Stored_t::tag_Comment("Comment");
SXP::Tag Nick_Stored_t::tag_Host("Host");
SXP::Tag Nick_Stored_t::tag_set_Protect("SET_Protect");
SXP::Tag Nick_Stored_t::tag_set_Secure("SET_Secure");
SXP::Tag Nick_Stored_t::tag_set_NoExpire("SET_NoExpire");
SXP::Tag Nick_Stored_t::tag_set_NoMemo("SET_NoMemo");
SXP::Tag Nick_Stored_t::tag_set_Private("SET_Private");
SXP::Tag Nick_Stored_t::tag_set_PRIVMSG("SET_PRIVMSG");
SXP::Tag Nick_Stored_t::tag_set_Language("SET_Language");
SXP::Tag Nick_Stored_t::tag_Forbidden("Forbidden");
SXP::Tag Nick_Stored_t::tag_lock_Protect("LOCK_Protect");
SXP::Tag Nick_Stored_t::tag_lock_Secure("LOCK_Secure");
SXP::Tag Nick_Stored_t::tag_lock_NoExpire("LOCK_NoExpire");
SXP::Tag Nick_Stored_t::tag_lock_NoMemo("LOCK_NoMemo");
SXP::Tag Nick_Stored_t::tag_lock_Private("LOCK_Private");
SXP::Tag Nick_Stored_t::tag_lock_PRIVMSG("LOCK_PRIVMSG");
SXP::Tag Nick_Stored_t::tag_lock_Language("LOCK_Language");
SXP::Tag Nick_Stored_t::tag_Picture("Picture");
SXP::Tag Nick_Stored_t::tag_Suspend_By("Suspend_By");
SXP::Tag Nick_Stored_t::tag_Suspend_Time("Suspend_Time");
SXP::Tag Nick_Stored_t::tag_LastSeenTime("LastSeenTime");
SXP::Tag Nick_Stored_t::tag_LastRealName("LastRealName");
SXP::Tag Nick_Stored_t::tag_LastMask("LastMask");
SXP::Tag Nick_Stored_t::tag_LastQuit("LastQuit");
SXP::Tag Nick_Stored_t::tag_Access("Access");
SXP::Tag Nick_Stored_t::tag_Ignore("Ignore");
SXP::Tag Nick_Stored_t::tag_UserDef("UserDef");

void Nick_Stored_t::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Nick_Stored_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if( pElement->IsA(tag_UserDef) )
    {
	mstring *tmp = new mstring;
	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void Nick_Stored_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Nick_Stored_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
	if( pElement->IsA(tag_Name) )		pElement->Retrieve(i_Name);
	if( pElement->IsA(tag_RegTime) )	pElement->Retrieve(i_RegTime);
	if( pElement->IsA(tag_Password) )	pElement->Retrieve(i_Password);
	if( pElement->IsA(tag_Email) )		pElement->Retrieve(i_Email);
	if( pElement->IsA(tag_URL) )		pElement->Retrieve(i_URL);
	if( pElement->IsA(tag_ICQ) )		pElement->Retrieve(i_ICQ);
	if( pElement->IsA(tag_Description) )	pElement->Retrieve(i_Description);
	if( pElement->IsA(tag_Comment) )	pElement->Retrieve(i_Comment);
	if( pElement->IsA(tag_Host) )		pElement->Retrieve(i_Host);
	if( pElement->IsA(tag_set_Protect) )	pElement->Retrieve(i_Protect);
	if( pElement->IsA(tag_set_Secure) )	pElement->Retrieve(i_Secure);
	if( pElement->IsA(tag_set_NoExpire) )	pElement->Retrieve(i_NoExpire);
	if( pElement->IsA(tag_set_NoMemo) )	pElement->Retrieve(i_NoMemo);
	if( pElement->IsA(tag_set_Private) )	pElement->Retrieve(i_Private);
	if( pElement->IsA(tag_set_PRIVMSG) )	pElement->Retrieve(i_PRIVMSG);
	if( pElement->IsA(tag_set_Language) )	pElement->Retrieve(i_Language);
	if( pElement->IsA(tag_Forbidden) )	pElement->Retrieve(i_Forbidden);
	if( pElement->IsA(tag_lock_Protect) )	pElement->Retrieve(l_Protect);
	if( pElement->IsA(tag_lock_Secure) )	pElement->Retrieve(l_Secure);
	if( pElement->IsA(tag_lock_NoExpire) )	pElement->Retrieve(l_NoExpire);
	if( pElement->IsA(tag_lock_NoMemo) )	pElement->Retrieve(l_NoMemo);
	if( pElement->IsA(tag_lock_Private) )	pElement->Retrieve(l_Private);
	if( pElement->IsA(tag_lock_PRIVMSG) )	pElement->Retrieve(l_PRIVMSG);
	if( pElement->IsA(tag_lock_Language) )	pElement->Retrieve(l_Language);
	if( pElement->IsA(tag_Picture) )	pElement->Retrieve(i_Picture);
	if( pElement->IsA(tag_Suspend_By) )	pElement->Retrieve(i_Suspend_By);
	if( pElement->IsA(tag_Suspend_Time) )	pElement->Retrieve(i_Suspend_Time);
	if( pElement->IsA(tag_LastSeenTime) )	pElement->Retrieve(i_LastSeenTime);
	if( pElement->IsA(tag_LastRealName) )	pElement->Retrieve(i_LastRealName);
	if( pElement->IsA(tag_LastMask) )	pElement->Retrieve(i_LastMask);
	if( pElement->IsA(tag_LastQuit) )	pElement->Retrieve(i_LastQuit);

    if( pElement->IsA(tag_Access) )
    {
        mstring tmp;
        pElement->Retrieve(tmp);
        i_access.insert(tmp);
    }
    if( pElement->IsA(tag_Ignore) )
    {
        mstring tmp;
        pElement->Retrieve(tmp);
        i_ignore.insert(tmp);
    }
}

void Nick_Stored_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    //TODO: Add your source code here
	pOut->BeginObject(tag_Nick_Stored_t, attribs);

	// Only way to ENSURE the data wont change.
	WLOCK(("NickServ", "stored", i_Name.LowerCase()));
	pOut->WriteElement(tag_Name, i_Name);
	pOut->WriteElement(tag_RegTime, i_RegTime);
	pOut->WriteElement(tag_Password, i_Password);
	pOut->WriteElement(tag_Email, i_Email);
	pOut->WriteElement(tag_URL, i_URL);
	pOut->WriteElement(tag_ICQ, i_ICQ);
	pOut->WriteElement(tag_Description, i_Description);
	pOut->WriteElement(tag_Comment, i_Comment);
	pOut->WriteElement(tag_Host, i_Host);
	pOut->WriteElement(tag_set_Protect, i_Protect);
	pOut->WriteElement(tag_set_Secure, i_Secure);
	pOut->WriteElement(tag_set_NoExpire, i_NoExpire);
	pOut->WriteElement(tag_set_NoMemo, i_NoMemo);
	pOut->WriteElement(tag_set_Private, i_Private);
	pOut->WriteElement(tag_set_PRIVMSG, i_PRIVMSG);
	pOut->WriteElement(tag_set_Language, i_Language);
	pOut->WriteElement(tag_Forbidden, i_Forbidden);
	pOut->WriteElement(tag_lock_Protect, l_Protect);
	pOut->WriteElement(tag_lock_Secure, l_Secure);
	pOut->WriteElement(tag_lock_NoExpire, l_NoExpire);
	pOut->WriteElement(tag_lock_NoMemo, l_NoMemo);
	pOut->WriteElement(tag_lock_Private, l_Private);
	pOut->WriteElement(tag_lock_PRIVMSG, l_PRIVMSG);
	pOut->WriteElement(tag_lock_Language, l_Language);
	pOut->WriteElement(tag_Picture, i_Picture);
	pOut->WriteElement(tag_Suspend_By, i_Suspend_By);
	pOut->WriteElement(tag_Suspend_Time, i_Suspend_Time);
	pOut->WriteElement(tag_LastSeenTime, i_LastSeenTime);
	pOut->WriteElement(tag_LastRealName, i_LastRealName);
	pOut->WriteElement(tag_LastMask, i_LastMask);
	pOut->WriteElement(tag_LastQuit, i_LastQuit);

	set<mstring>::const_iterator iter2;
	for(iter2=i_access.begin(); iter2!=i_access.end(); iter2++)
	{
	    pOut->WriteElement(tag_Access, *iter2);
	}	
	for(iter2=i_ignore.begin(); iter2!=i_ignore.end(); iter2++)
	{
	    pOut->WriteElement(tag_Ignore, *iter2);
	}	

        map<mstring,mstring>::const_iterator iter;
        for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
        {
            pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
        }

	pOut->EndObject(tag_Nick_Stored_t);
}

size_t Nick_Stored_t::Usage()
{
    size_t retval = 0;

    // Only way to ENSURE the data wont change.
    WLOCK(("NickServ", "stored", i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_RegTime.Internal());
    retval += i_Password.capacity();
    retval += i_Email.capacity();
    retval += i_URL.capacity();
    retval += i_ICQ.capacity();
    retval += i_Description.capacity();
    retval += i_Comment.capacity();
    retval += i_Host.capacity();
    
    set<mstring>::iterator i;
    for (i=i_slaves.begin(); i!=i_slaves.end(); i++)
    {
	retval += i->capacity();
    }
    for (i=i_access.begin(); i!=i_access.end(); i++)
    {
	retval += i->capacity();
    }
    for (i=i_ignore.begin(); i!=i_ignore.end(); i++)
    {
	retval += i->capacity();
    }

    retval += sizeof(i_Protect);
    retval += sizeof(l_Protect);
    retval += sizeof(i_Secure);
    retval += sizeof(l_Secure);
    retval += sizeof(i_NoExpire);
    retval += sizeof(l_NoExpire);
    retval += sizeof(i_NoMemo);
    retval += sizeof(l_NoMemo);
    retval += sizeof(i_Private);
    retval += sizeof(l_Private);
    retval += sizeof(i_PRIVMSG);
    retval += sizeof(l_PRIVMSG);
    retval += i_Language.capacity();
    retval += sizeof(l_Language);
    retval += sizeof(i_Forbidden);
    retval += sizeof(i_Picture);

    retval += i_Suspend_By.capacity();
    retval += sizeof(i_Suspend_Time.Internal());

    retval += sizeof(i_LastSeenTime.Internal());
    retval += i_LastRealName.capacity();
    retval += i_LastMask.capacity();
    retval += i_LastQuit.capacity();

    map<mstring,mstring>::iterator j;
    for (j=i_UserDef.begin(); j!=i_UserDef.end(); j++)
    {
	retval += j->first.capacity();
	retval += j->second.capacity();
    }

    return retval;
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
    // Amountof nicknames it will try, only
    // for the guest????? method.
    unsigned int i, attempts = 64;

    if (Parent->nickserv.Append_Rename())
    {
	for (i=0; i<Parent->nickserv.Suffixes().Len(); i++)
	{
	    while (retval.Len() < Parent->server.proto.NickLen())
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
    }
    else
    {
	srand(time(NULL));
	for (i=0; i<attempts; i++)
	{
	    retval = "";
	    retval.Format("%s%05d",
		    Parent->nickserv.Suffixes().c_str(),
		    rand() % 99999);
	    if (!Parent->nickserv.IsLive(retval) &&
		!Parent->nickserv.IsStored(retval))
	    {
		RET(retval);
	    }
	}
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
		"SID*", Parent->commserv.ALL_Name(), NickServ::do_Identify);
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

    Parent->commands.RemSystemCommand(GetInternalName(),
		"H*LP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"REG*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"DROP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LINK*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"U*LIN*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"HOST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"*SLAV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ID*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"INF*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"GHOST*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"REC*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LIST*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SEND*", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SUSP*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNSUS*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"FORB*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"GET*PASS*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LIVE*", Parent->commserv.SOP_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* CUR*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* ERA*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* ADD", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* ERA*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* LIST", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* VIEW", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PASS*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* EMAIL", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* URL", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* ICQ", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* DESC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* COMM*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PIC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PROT*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* SEC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* NOEX*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* NOMEMO", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PRIVM*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* *MSG", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* PRIV*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* LANG*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK PROT*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK NOMEMO", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK PRIVM*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK *MSG", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK LANG*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PROT*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK SEC*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK NOMEMO", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PRIVM*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK *MSG", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK PRIV*", Parent->commserv.SOP_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK LANG*", Parent->commserv.SOP_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"SET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"LOCK", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"UNLOCK", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"ACC*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
		"IGN*", Parent->commserv.REGD_Name());
}


bool NickServ::IsLive(mstring in)
{
    FT("NickServ::IsLive", (in));
    RLOCK(("NickServ", "live"));
    bool retval = (live.find(in.LowerCase())!=live.end());
    RET(retval);
}

bool NickServ::IsStored(mstring in)
{
    FT("NickServ::IsStored", (in));
    RLOCK(("NickServ", "stored"));
    bool retval = (stored.find(in.LowerCase())!=stored.end());
    RET(retval);
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
    else if (msgtype == "PRIVMSG" &&
	!Parent->commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);
}

void NickServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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
	Log(LM_INFO, Parent->getLogMessage("NICKSERV/REGISTER"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		source.c_str());
    }
}

void NickServ::do_Drop(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Drop", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    unsigned long dropped = 0;
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
	    dropped = Parent->nickserv.stored[source.LowerCase()].Drop();
	    { WLOCK(("NickServ", "stored"));
	    Parent->nickserv.stored.erase(source.LowerCase());
	    }
	    Parent->nickserv.live[source.LowerCase()].UnIdentify();
	    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/DROPPED"));
	    Log(LM_INFO, Parent->getLogMessage("NICKSERV/DROP"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		source.c_str(), dropped-1);
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
	    target = Parent->getSname(target);
	    Parent->nickserv.stats.i_Drop++;
	    dropped = Parent->nickserv.stored[target.LowerCase()].Drop();
	    { WLOCK(("NickServ", "stored"));
	    Parent->nickserv.stored.erase(target.LowerCase());
	    }
	    if (!Parent->nickserv.IsStored(source))
		Parent->nickserv.live[source.LowerCase()].UnIdentify();
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/DROPPED"),
				target.c_str());
	    Log(LM_INFO, Parent->getLogMessage("NICKSERV/DROP"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		target.c_str(), dropped-1);
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
    hostnick = Parent->getSname(hostnick);

    if (Parent->nickserv.stored[hostnick.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
					hostnick.c_str());
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
					hostnick.c_str());
	Log(LM_INFO, Parent->getLogMessage("NICKSERV/LINK"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		source.c_str(), hostnick.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NICK_WRONG_PASS"));
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

	if (!(Parent->nickserv.stored[target.LowerCase()].IsSibling(source) ||
		Parent->nickserv.stored[target.LowerCase()].Host().CmpNoCase(source)==0))
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
	    Log(LM_INFO, Parent->getLogMessage("NICKSERV/UNLINK"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		source.c_str(), target.c_str());
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
	    mstring target = Parent->getSname(Parent->nickserv.stored[source.LowerCase()].Host());
	    ::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/UNLINKED"));
	    Log(LM_INFO, Parent->getLogMessage("NICKSERV/UNLINK"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		source.c_str(), target.c_str());
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
		Parent->nickserv.stored[newhost.LowerCase()].IsSibling(source) ||
		Parent->nickserv.stored[newhost.LowerCase()].Host().CmpNoCase(source)==0))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTLINKED"),
						newhost.c_str());
	    return;
	}

	mstring oldhost = Parent->nickserv.stored[newhost.LowerCase()].Host();
	Parent->nickserv.stats.i_Host++;
	Parent->nickserv.stored[newhost.LowerCase()].MakeHost();
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/NEWHOST"),
						newhost.c_str());
	Log(LM_INFO, Parent->getLogMessage("NICKSERV/HOST"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		oldhost.c_str(), newhost.c_str());
    }
    else
    {
	mstring oldhost = Parent->nickserv.stored[source.LowerCase()].Host();
	Parent->nickserv.stats.i_Host++;
	Parent->nickserv.stored[source.LowerCase()].MakeHost();
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/NEWHOST"));
	Log(LM_INFO, Parent->getLogMessage("NICKSERV/HOST"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		oldhost.c_str(), source.c_str());
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

	target = Parent->getSname(target);
	if (Parent->nickserv.stored[target.LowerCase()].Forbidden())
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
					target.c_str());
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
    {
	Parent->nickserv.stats.i_Identify++;
	Log(LM_INFO, Parent->getLogMessage("NICKSERV/IDENTIFY"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		source.c_str());
    }
    if (output != "")
	::send(mynick, source, output);
}

void NickServ::do_Info(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Info", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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
						nick->Name().c_str());
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
		if (output.Len() + nick->Sibling(i).Len() > Parent->server.proto.MaxLine())
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
	// IF committee is not ALL or REGD
	// AND if it has a headcom, we're not in it
	// AND the committee isnt private or the requesting user is in OPER
	if (iter->first != Parent->commserv.ALL_Name() &&
	    iter->first != Parent->commserv.REGD_Name() &&
	    (iter->second.HeadCom() == "" ||
	    !(Parent->commserv.IsList(iter->second.HeadCom()) &&
	      Parent->commserv.list[iter->second.HeadCom().UpperCase()].IsIn(nick->Name()))) &&
	    iter->second.IsIn(target) && (!iter->second.Private() ||
	    (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	     Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))))
	{
	    if (output.Len() + iter->second.Name().Len() > Parent->server.proto.MaxLine())
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
    if (nick->PicNum() &&
	Parent->filesys.Exists(FileMap::Picture, nick->PicNum()))
	::send(mynick, source, Parent->getMessage(source, "NS_INFO/HASPIC"),
				ToHumanSpace(Parent->filesys.GetSize(FileMap::Picture, nick->PicNum())).c_str(),
				mynick.c_str(), nick->Name().c_str());
    if (nick->IsOnline())
	::send(mynick, source,  Parent->getMessage(source, "NS_INFO/ISONLINE"),
		Parent->getLname(nick->Name()).c_str());
    if (Parent->servmsg.ShowSync())
	::send(mynick, source, Parent->getMessage("MISC/SYNC"),
			Parent->events->SyncTime().c_str());
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

    if (nick.CmpNoCase(source)==0)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOTONYOURSELF"),
						message.c_str());
	return;
    }

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

    nick = Parent->getLname(nick);
    if (Parent->nickserv.stored[nick.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nick.c_str());
	return;
    }

    if (pass != Parent->nickserv.stored[nick.LowerCase()].Password())
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NICK_WRONG_PASS"));
	return;
    }

    Parent->server.ANONKILL(mynick, nick, source + " (" +
				Parent->getMessage(nick, "MISC/KILL_GHOST") + ")");
    if (Parent->nickserv.recovered.find(nick.LowerCase()) !=
				Parent->nickserv.recovered.end())
    {
	MLOCK(("NickServ", "recovered"));
	Parent->nickserv.recovered.erase(nick.LowerCase());
    }
    Parent->nickserv.stats.i_Ghost++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/RELEASED"),
				nick.c_str());
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/GHOST"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
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

    if (nick.CmpNoCase(source)==0)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOTONYOURSELF"),
						message.c_str());
	return;
    }

    if (!Parent->nickserv.IsStored(nick))
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
				nick.c_str());
	return;
    }

    nick = Parent->getSname(nick);
    if (Parent->nickserv.stored[nick.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
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
				Parent->nickserv.Enforcer_Name());
    { MLOCK(("NickServ", "recovered"));
    Parent->nickserv.recovered[nick.LowerCase()] = Now();
    }
    Parent->nickserv.stats.i_Recover++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/HELD"),
				nick.c_str());
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/RECOVER"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	nick.c_str());
}

void NickServ::do_List(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_List", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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
	listsize = ACE_OS::atoi(params.ExtractWord(3, " ").c_str());
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


    bool isoper = (Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
		Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source));
    bool issop = (Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
		Parent->commserv.list[Parent->commserv.SOP_Name()].IsOn(source));

    RLOCK(("NickServ", "stored"));
    for (iter = Parent->nickserv.stored.begin(), i=0, count = 0;
			iter != Parent->nickserv.stored.end(); iter++)
    {
	if (iter->second.Name().LowerCase().Matches(mask))
	{
	    if (iter->second.Host() != "")
		continue;

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

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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
    if (Parent->nickserv.stored[target.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						target.c_str());
	return;
    }

    unsigned long picnum = Parent->nickserv.stored[target.LowerCase()].PicNum();
    if (!picnum)
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/NOPIC"),
							target.c_str());
	return;
    }

    if (!Parent->filesys.Exists(FileMap::Picture, picnum))
    {
	Parent->nickserv.stored[target.LowerCase()].GotPic(0);
	::send(mynick, source, Parent->getMessage(source, "DCC/NOFILE"),
							"SEND");
	return;
    }

    if (!(Parent->files.TempDirSize() == 0 ||
	mFile::DirUsage(Parent->files.TempDir()) <=
		Parent->files.TempDirSize()))
    {
	::send(mynick, source, Parent->getMessage(source, "DCC/NOSPACE2"));
	return;
    }

    mstring filename = 	Parent->filesys.GetName(FileMap::Picture, picnum);
    size_t filesize = Parent->filesys.GetSize(FileMap::Picture, picnum);

    unsigned short port = FindAvailPort();
    ::privmsg(mynick, source, DccEngine::encode("DCC SEND", filename +
		" " + mstring(ultoa(Parent->LocalHost())) + " " +
		mstring(ultoa(port)) + " " + mstring(ultoa(filesize))));
    Parent->dcc->Accept(port, mynick, source, FileMap::Picture, picnum);
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
    if (Parent->nickserv.stored[target.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						target.c_str());
	return;
    }

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
    Log(LM_NOTICE, Parent->getLogMessage("NICKSERV/SUSPEND"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	target.c_str(), reason.c_str());
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
    if (Parent->nickserv.stored[target.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						target.c_str());
	return;
    }

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
    Log(LM_NOTICE, Parent->getLogMessage("NICKSERV/UNSUSPEND"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
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
    Log(LM_NOTICE, Parent->getLogMessage("NICKSERV/FORBID"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
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
    if (Parent->nickserv.stored[target.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						target.c_str());
	return;
    }

    // If we are NOT a SADMIN, and target is a PRIV GROUP.
    if (Parent->commserv.IsList(Parent->commserv.SADMIN_Name()) &&
	!Parent->commserv.list[Parent->commserv.SADMIN_Name()].IsIn(source) &&
	(Parent->commserv.list[Parent->commserv.SADMIN_Name()].IsIn(target) ||
	(Parent->commserv.IsList(Parent->commserv.SOP_Name()) &&
	Parent->commserv.list[Parent->commserv.SOP_Name()].IsIn(target)) ||
	(Parent->commserv.IsList(Parent->commserv.ADMIN_Name()) &&
	Parent->commserv.list[Parent->commserv.ADMIN_Name()].IsIn(target)) ||
	(Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsIn(target))))
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
    Log(LM_NOTICE, Parent->getLogMessage("NICKSERV/GETPASS"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	nick->Name().c_str(), host.c_str());
}


void NickServ::do_Live(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_Live", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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
	listsize = ACE_OS::atoi(params.ExtractWord(3, " ").c_str());
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

    RLOCK(("NickServ", "live"));
    for (iter = Parent->nickserv.live.begin(), i=0, count = 0;
			iter != Parent->nickserv.live.end(); iter++)
    {
	if (iter->second.Mask(Nick_Live_t::N_U_P_H).LowerCase().Matches(mask))
	{
	    if (i < listsize)
	    {
		if (message.Contains("OPER") && !iter->second.HasMode("o"))
		    continue;

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
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::U_H).After("!").c_str(),
		Parent->getMessage(source, "LIST/ACCESS").c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/ACCESS_ADD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::U_H).After("!").c_str(),
		source.c_str());

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
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/ACCESS_ADD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		hostmask.c_str(), source.c_str());
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
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/WHOLENUMBER"));
	    return;
	}

	unsigned int num = ACE_OS::atoi(hostmask.c_str());
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

    unsigned int count = Parent->nickserv.stored[source.LowerCase()].AccessDel(hostmask);
    if (count)
    {
	Parent->nickserv.stats.i_Access++;
	::send(mynick, source, Parent->getMessage(source, "LIST/DEL_MATCH"),
			count, hostmask.c_str(),
			Parent->getMessage(source, "LIST/ACCESS").c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/ACCESS_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		hostmask.c_str(), source.c_str());
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

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

    if (params.WordCount(" ") >= 3 &&
	Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
    {
	    target = params.ExtractWord(3, " ");
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

    target = Parent->getSname(target);
    if (Parent->nickserv.stored[target.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						target.c_str());
	return;
    }

    if (Parent->nickserv.stored[source.LowerCase()].IgnoreAdd(target))
    {
	Parent->nickserv.stats.i_Ignore++;
	::send(mynick, source, Parent->getMessage(source, "LIST/ADD"),
			target.c_str(), Parent->getMessage(source, "LIST/IGNORE").c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/IGNORE_ADD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		target.c_str(), source.c_str());
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
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/WHOLENUMBER"));
	    return;
	}

	unsigned int num = ACE_OS::atoi(target.c_str());
	if (num <= 0 || num > Parent->nickserv.stored[source.LowerCase()].Ignore())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
			1, Parent->nickserv.stored[source.LowerCase()].Ignore());
	    return;
	}

	target = Parent->nickserv.stored[source.LowerCase()].Ignore(num-1);
    }

    unsigned int count = Parent->nickserv.stored[source.LowerCase()].IgnoreDel(target);
    if (count)
    {
	Parent->nickserv.stats.i_Ignore++;
	::send(mynick, source, Parent->getMessage(source, "LIST/DEL_MATCH"),
			count, target.c_str(),
			Parent->getMessage(source, "LIST/IGNORE").c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/IGNORE_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		target.c_str(), source.c_str());
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

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

    if (params.WordCount(" ") >= 3 &&
	Parent->commserv.IsList(Parent->commserv.OPER_Name()) &&
	Parent->commserv.list[Parent->commserv.OPER_Name()].IsOn(source))
    {
	    target = params.ExtractWord(3, " ");
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
    Log(LM_INFO, Parent->getLogMessage("NICKSERV/SET_PASSWORD"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		source.c_str());
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
    else if (!newvalue.Contains("@"))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAIN"),
		Parent->getMessage(source, "NS_SET/EMAIL").c_str(), '@');
	return;
    }
    else if (newvalue.WordCount("@") != 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTCONTAINONE"),
		Parent->getMessage(source, "NS_SET/EMAIL").c_str(), '@');
	return;
    }

    Parent->nickserv.stored[source.LowerCase()].Email(newvalue);
    Parent->nickserv.stats.i_Set++;
    if (newvalue != "")
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/EMAIL").c_str(), newvalue.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("NS_SET/EMAIL").c_str(),
		source.c_str(), newvalue.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/UNSET"),
		Parent->getMessage(source, "NS_SET/EMAIL").c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("NS_SET/EMAIL").c_str(),
		source.c_str());
    }
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
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/URL").c_str(),
		("http://" + newvalue).c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("NS_SET/URL").c_str(),
		source.c_str(), ("http://" + newvalue).c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/UNSET"),
		Parent->getMessage(source, "NS_SET/URL").c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("NS_SET/URL").c_str(),
		source.c_str());
    }
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
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/ICQ").c_str(), newvalue.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("NS_SET/ICQ").c_str(),
		source.c_str(), newvalue.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/UNSET"),
		Parent->getMessage(source, "NS_SET/ICQ").c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("NS_SET/ICQ").c_str(),
		source.c_str());
    }
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
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/DESCRIPTION").c_str(), newvalue.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("NS_SET/DESCRIPTION").c_str(),
		source.c_str(), newvalue.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/UNSET"),
		Parent->getMessage(source, "NS_SET/DESCRIPTION").c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("NS_SET/DESCRIPTION").c_str(),
		source.c_str());
    }
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

    target = Parent->getSname(target);
    if (Parent->nickserv.stored[target.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						target.c_str());
	return;
    }

    if (comment.CmpNoCase("none") == 0)
	comment = "";

    Parent->nickserv.stored[target.LowerCase()].Comment(comment);
    Parent->nickserv.stats.i_Set++;
    if (comment != "")
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/SET_TO"),
		Parent->getMessage(source, "NS_SET/COMMENT").c_str(),
		target.c_str(), comment.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("NS_SET/COMMENT").c_str(),
		target.c_str(), comment.c_str());
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/UNSET"),
		Parent->getMessage(source, "NS_SET/COMMENT").c_str(),
		target.c_str());
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNSET"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		Parent->getMessage("NS_SET/COMMENT").c_str(),
		target.c_str());
    }
}

void NickServ::do_set_Picture(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Picture", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    if (Parent->nickserv.PicExt() == "")
    {
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/PICDISABLED"));
	return;
    }

    if (params.WordCount(" ") > 2 &&
	params.ExtractWord(3, " ").CmpNoCase("NONE")==0)
    {
	if (Parent->nickserv.stored[source.LowerCase()].PicNum())
	    Parent->filesys.EraseFile(FileMap::Picture,
		Parent->nickserv.stored[source.LowerCase()].PicNum());
	Parent->nickserv.stored[source.LowerCase()].GotPic(0u);
	::send(mynick, source, Parent->getMessage(source, "NS_YOU_COMMAND/REMOVED"));
	Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/PICTURE_DEL"),
		Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
		source.c_str());
    }
    else
    {
	if (Parent->nickserv.stored[source.LowerCase()].PicNum())
	    Parent->filesys.EraseFile(FileMap::Picture,
		Parent->nickserv.stored[source.LowerCase()].PicNum());
	Parent->nickserv.stored[source.LowerCase()].GotPic(0u);
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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/PROTECT").c_str(),
	source.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/SECURE").c_str(),
	source.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
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

    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_NOTICE, Parent->getLogMessage("NICKSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/NOEXPIRE").c_str(),
	nickname.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/NOMEMO").c_str(),
	source.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/PRIVATE").c_str(),
	source.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/PRIVMSG").c_str(),
	source.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
}

void NickServ::do_set_Language(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_set_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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
	// check for valid language ...
	if (!mFile::Exists(Parent->files.Langdir()+DirSlash+lang.LowerCase()+".lng"))
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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/SET"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/LANGUAGE").c_str(),
	source.c_str(), lang.c_str());
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/PROTECT").c_str(),
	nickname.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/SECURE").c_str(),
	nickname.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/NOMEMO").c_str(),
	nickname.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/PRIVATE").c_str(),
	nickname.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/PRIVMSG").c_str(),
	nickname.c_str(), onoff.GetBool() ?
		Parent->getMessage("MISC/ON").c_str() :
		Parent->getMessage("MISC/OFF").c_str());
}

void NickServ::do_lock_Language(mstring mynick, mstring source, mstring params)
{
    FT("NickServ::do_lock_Language", (mynick, source, params));

    mstring message  = params.Before(" ", 2).UpperCase();

    if (Parent->ircsvchandler->HTM_Level() > 3)
    {
	::send(mynick, source, Parent->getMessage(source, "MISC/HTM"),
							message.c_str());
	return;
    }

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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    else if (!mFile::Exists(Parent->files.Langdir()+DirSlash+lang.LowerCase()+".lng"))
    {
	::send(mynick, source, Parent->getMessage(source, "OS_STATUS/NOLANG"),
			lang.c_str());
	return;
    }

    Parent->nickserv.stored[nickname.LowerCase()].L_Language(false);
    Parent->nickserv.stored[nickname.LowerCase()].Language(lang);
    Parent->nickserv.stored[nickname.LowerCase()].L_Language(true);
    Parent->nickserv.stats.i_Lock++;
    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_COMMAND/LOCKED"),
			Parent->getMessage(source, "NS_SET/LANGUAGE").c_str(),
			nickname.c_str(), mstring(lang + " (" +
			Parent->getMessageL(lang, "ERR_SYNTAX/TRANSLATED") + ")").c_str());
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/LOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/LANGUAGE").c_str(),
	nickname.c_str(), lang.c_str());
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/PROTECT").c_str(),
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/SECURE").c_str(),
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/NOMEMO").c_str(),
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/PRIVATE").c_str(),
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/PRIVMSG").c_str(),
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
    if (Parent->nickserv.stored[nickname.LowerCase()].Forbidden())
    {
	::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISFORBIDDEN"),
						nickname.c_str());
	return;
    }

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
    Log(LM_DEBUG, Parent->getLogMessage("NICKSERV/UNLOCK"),
	Parent->nickserv.live[source.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str(),
	Parent->getMessage("NS_SET/LANGUAGE").c_str(),
	nickname.c_str());
}

SXP::Tag NickServ::tag_NickServ("NickServ");

void NickServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("NickServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Nick_Stored_t *ns = new Nick_Stored_t;

    if( pElement->IsA( ns->GetClassTag() ) )
    {
	ns_array.push_back(ns);
	pIn->ReadTo(ns);
    }
    else
    {
	delete ns;
    }
}

void NickServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("NickServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void NickServ::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("NickServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_NickServ, attribs);

    map<mstring, Nick_Stored_t>::iterator iter;
    { RLOCK(("NickServ", "stored"));
    for (iter = stored.begin(); iter != stored.end(); iter++)
	pOut->WriteSubElement(&iter->second, attribs);
    }

    pOut->EndObject(tag_NickServ);
}

void NickServ::PostLoad()
{
    NFT("NickServ::PostLoad");
    unsigned int i, j;
    for (i=0; i<ns_array.size(); i++)
    {
	if (ns_array[i] != NULL)
	{
	    for (j=0; j<ns_array[i]->ud_array.size(); j++)
	    {
		if (ns_array[i]->ud_array[j] != NULL)
		{
		    if (ns_array[i]->ud_array[j]->Contains("\n"))
			ns_array[i]->i_UserDef[ns_array[i]->ud_array[j]->Before("\n")] =
				ns_array[i]->ud_array[j]->After("\n");
		    delete ns_array[i]->ud_array[j];
		}
	    }
	    ns_array[i]->ud_array.clear();
	    if (ns_array[i]->Name() != "")
		stored[ns_array[i]->Name().LowerCase()] = *ns_array[i];
	    delete ns_array[i];
	}
    }
    ns_array.clear();

    map<mstring,Nick_Stored_t>::iterator iter;
    CP(("Linking nickname entries ..."));
    WLOCK(("NickServ", "stored"));
    for (iter=stored.begin(); iter!=stored.end(); iter++)
    {
	if (iter->second.i_Host != "")
	{
	    if (IsStored(iter->second.i_Host))
	    {
		COM(("Nickname %s has been linked to %s ...",
		    iter->first.c_str(), iter->second.i_Host.c_str()));
		stored[iter->second.i_Host.LowerCase()].i_slaves.insert(iter->first);
	    }
	    else
	    {
		Log(LM_WARNING, Parent->getLogMessage("ERROR/HOST_NOTREGD"),
		    iter->second.i_Host.c_str(), iter->first.c_str());
		iter->second.i_Host = "";
	    }
	}
    }
}
