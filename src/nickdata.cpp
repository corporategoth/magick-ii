#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation "nickserv.h"
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
#define RCSID(x,y) const char *rcsid_nickdata_cpp_ ## x () { return y; }
RCSID(nickdata_cpp, "@(#)$Id$");

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

#include "magick.h"

#ifndef MAGICK_HAS_EXCEPTIONS
static Nick_Stored_t GLOB_Nick_Stored_t;
static Nick_Live_t GLOB_Nick_Live_t;
static mDateTime GLOB_mDateTime;
#endif

void Nick_Live_t::InFlight_t::ChgNick(const mstring & newnick)
{
    BTCB();
    FT("Nick_Live_t::InFlight_t::ChgNick", (newnick));
    WLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight"));
    WLOCK2((lck_NickServ, lck_live, newnick.LowerCase(), "InFlight"));
    MCB(nick);
    nick = newnick;
    MLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "timer"));
    if (timer)
    {
	CB(1, timer);
	mstring *arg = NULL;
	if (Magick::instance().reactor().cancel_timer(timer, reinterpret_cast < const void ** > (arg)) && arg != NULL)
	{
	    delete arg;
	}
	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);
	timer =
	    ACE_Reactor::instance()->schedule_timer(&Magick::instance().nickserv.ifh, new mstring(nick.LowerCase()),
						    ACE_Time_Value(Magick::instance().memoserv.InFlight()));
	CE(1, timer);
    }
    MCE(nick);
    ETCB();
}

Nick_Live_t::InFlight_t & Nick_Live_t::InFlight_t::operator=(const InFlight_t & in)
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::operator=");
    nick = in.nick;
    type = in.type;
    fileattach = in.fileattach;
    fileinprog = in.fileinprog;
    service = in.service;
    sender = in.sender;
    recipiant = in.recipiant;
    text = in.text;
    NRET(Nick_Live_t::InFlight_t &, *this);
    ETCB();
}

// NICK has been destructed, or is going to be.
// Send any pending memos with End(0), but because
// it may have an 'in progress' memo at the time of
// destruction, we make sure the timer is cancelled.
Nick_Live_t::InFlight_t::~InFlight_t()
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::~InFlight_t");
    if (Exists())
	End(0u);
    mstring *arg = NULL;

    if (timer)
    {
	if (Magick::instance().reactor().cancel_timer(timer, reinterpret_cast < const void ** > (arg)) && arg != NULL)
	{
	    delete arg;
	}
	timer = 0;
    }
    ETCB();
}

// Initialise all veriables (done in Cancel() or End())
void Nick_Live_t::InFlight_t::init()
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::init");
    WLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight"));
    type = FileMap::MemoAttach;
    timer = 0u;
    fileattach = false;
    fileinprog = false;
    service.erase();
    sender.erase();
    recipiant.erase();
    text.erase();
    ETCB();
}

// We have completed a file transfer, or errored out.
// 0 if we errored, else its a file number.
void Nick_Live_t::InFlight_t::File(const unsigned long filenum)
{
    BTCB();
    FT("Nick_Live_t::InFlight_t::File", (filenum));
    WLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "fileinprog"));
    MCB(fileinprog);
    fileinprog = false;
    MCE(fileinprog);
    if (filenum)
	End(filenum);
    else
	Cancel();
    ETCB();
}

// Ok, we've started a file transfer, cancel timer,
// now we wait for the File() command.
void Nick_Live_t::InFlight_t::SetInProg()
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::SetInProg");
    WLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "fileinprog"));
    MCB(fileinprog);
    fileinprog = true;
    mstring *arg = NULL;

    MLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "timer"));
    if (timer)
    {
	CB(1, timer);
	if (Magick::instance().reactor().cancel_timer(timer, reinterpret_cast < const void ** > (arg)) && arg != NULL)
	{
	    delete arg;
	}
	timer = 0;
	CE(1, timer);
    }
    MCE(fileinprog);
    ETCB();
}

// New memo, send an old one if it isnt in-progress, and
// cancel it if it was never started.
void Nick_Live_t::InFlight_t::Memo(const bool file, const mstring & mynick, const mstring & who, const mstring & message,
				   const bool silent)
{
    BTCB();
    FT("Nick_Live_t::InFlight_t::Memo", (file, mynick, who, message, silent));
    if (!Magick::instance().nickserv.IsStored(nick))
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/ISNOTSTORED");
	return;
    }

    if (IsChan(who))
    {
	if (!Magick::instance().chanserv.IsStored(who))
	{
	    SEND(mynick, nick, "CS_STATUS/ISNOTSTORED", (who));
	    return;
	}

	if (file)
	{
	    NSEND(mynick, nick, "ERR_SYNTAX/CHANFILEATTACH");
	    return;
	}
    }
    else
    {
	if (!Magick::instance().nickserv.IsStored(who))
	{
	    SEND(mynick, nick, "NS_OTH_STATUS/ISNOTSTORED", (who));
	    return;
	}
	if (Magick::instance().nickserv.GetStored(who)->Forbidden())
	{
	    SEND(mynick, nick, "NS_OTH_STATUS/ISFORBIDDEN", (Magick::instance().getSname(who)));
	    return;
	}

	if (file && !Magick::instance().memoserv.Files())
	{
	    NSEND(mynick, nick, "MS_SYNTAX/FILEDISABLED");
	    return;
	}
    }

    if (file &&
	!(Magick::instance().files.TempDirSize() == 0 ||
	  mFile::DirUsage(Magick::instance().files.TempDir()) <= Magick::instance().files.TempDirSize()))
    {
	NSEND(mynick, nick, "DCC/NOSPACE2");
	return;
    }

    if (File())
    {
	if (InProg())
	{
	    NSEND(service, nick, "ERR_SITUATION/FILEINPROG");
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

    DumpB();
    {
	WLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight"));
	type = FileMap::MemoAttach;
	fileattach = file;
	service = mynick;
	sender = nick;
	recipiant = who;
	text = message;
    }

    {
	MLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "timer"));
	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);
	timer =
	    Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh, new mstring(sender.LowerCase()),
							ACE_Time_Value(Magick::instance().memoserv.InFlight()));
    }
    DumpE();

    if (!silent)
    {
	if_RLOCK ((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "fileattach"), fileattach)
	    SEND(mynick, nick, "MS_COMMAND/PENDING_FILE", (ToHumanTime(Magick::instance().memoserv.InFlight(), nick)));
	else
	    SEND(mynick, nick, "MS_COMMAND/PENDING", (ToHumanTime(Magick::instance().memoserv.InFlight(), nick)));
    }
    ETCB();
}

// Add text to a memo, and re-start the timer.
void Nick_Live_t::InFlight_t::Continue(const mstring & message)
{
    BTCB();
    FT("Nick_Live_t::InFlight_t::Continue", (message));
    if (!Memo())
    {
	NSEND(service, nick, "MS_STATUS/NOPENDING");
	return;
    }
    MCB(text);
    {
	WLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "text"));
	text += message;
    }
    mstring *arg = NULL;

    {
	MLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "timer"));
	if (timer)
	{
	    CB(1, timer);
	    if (Magick::instance().reactor().cancel_timer(timer, reinterpret_cast < const void ** > (arg)) && arg != NULL)
	    {
		delete arg;
	    }
	    while (Magick::instance().Pause())
		ACE_OS::sleep(1);
	    timer =
		Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh, new mstring(nick.LowerCase()),
							    ACE_Time_Value(Magick::instance().memoserv.InFlight()));
	    CE(1, timer);
	}
    }
    MCE(text);
    SEND(service, nick, "MS_COMMAND/CONTINUE", (ToHumanTime(Magick::instance().memoserv.InFlight(), nick)));
    ETCB();
}

// Cancel a memo or picture send.
void Nick_Live_t::InFlight_t::Cancel()
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::Cancel");

    mstring *arg = NULL;

    {
	MLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "timer"));
	if (timer)
	{
	    MCB(timer);
	    if (Magick::instance().reactor().cancel_timer(timer, reinterpret_cast < const void ** > (arg)) && arg != NULL)
	    {
		delete arg;
	    }
	    timer = 0;
	    MCE(timer);
	}
    }
    if (Memo() && !File())
    {
	NSEND(service, nick, "MS_COMMAND/CANCEL");
    }
    init();
    ETCB();
}

// This is the final destination of all memos.
// It will do nothing if a file is in-progress, and
// will call Cancel() if no file was started, but requested.
// It accepts an argument of 'file number'.  Ignored if
// no file attachment was requested, but set if 
void Nick_Live_t::InFlight_t::End(const unsigned long filenum)
{
    BTCB();
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

	{
	    MLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "timer"));
	    if (timer)
	    {
		MCB(timer);
		if (Magick::instance().reactor().cancel_timer(timer, reinterpret_cast < const void ** > (arg)) && arg != NULL)
		{
		    delete arg;
		}
		timer = 0;
		MCE(timer);
	    }
	}
	RLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight"));
	if (Magick::instance().nickserv.IsStored(sender))
	{
	    map_entry < Nick_Stored_t > source = Magick::instance().nickserv.GetStored(sender);
	    if (!source->Host().empty())
	    {
		sender = source->Host();
	    }
	    if (Memo())
	    {
		if (IsChan(recipiant))
		{
		    if (Magick::instance().chanserv.IsStored(recipiant))
		    {
			News_t tmp(recipiant, sender, text);

			Magick::instance().memoserv.AddChannelNews(&tmp);

			map_entry < Chan_Stored_t > cstored = Magick::instance().chanserv.GetStored(recipiant);
			SEND(service, nick, "MS_COMMAND/SENT", (recipiant, cstored->Founder()));
			LOG(LM_DEBUG, "MEMOSERV/SEND",
			    (Magick::instance().nickserv.GetLive(sender)->Mask(Nick_Live_t::N_U_P_H), recipiant));

			RLOCK3((lck_MemoServ, lck_channel, recipiant.LowerCase()));
			MemoServ::channel_news_t & newslist = Magick::instance().memoserv.GetChannel(recipiant);
			if (Magick::instance().chanserv.IsLive(recipiant))
			{
			    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(recipiant);
			    unsigned int i;

			    for (i = 0; i < clive->Users(); i++)
			    {
				if (Magick::instance().nickserv.IsStored(clive->User(i)) &&
				    cstored->GetAccess(clive->User(i), "READMEMO"))
				{
				    SEND(service, clive->User(i), "MS_COMMAND/CS_NEW",
					 (newslist.size(), recipiant, nick, service, recipiant, newslist.size()));
				}
			    }
			}
		    }
		}
		else
		{
		    if (Magick::instance().nickserv.IsStored(recipiant))
		    {
			mstring realrecipiant = Magick::instance().nickserv.GetStored(recipiant)->Host();

			if (realrecipiant.empty())
			    realrecipiant = recipiant;

			if (!
			    (!filenum || Magick::instance().memoserv.FileSize() == 0 ||
			     Magick::instance().filesys.GetSize(FileMap::MemoAttach, filenum) <=
			     Magick::instance().memoserv.FileSize()))
			{
			    Magick::instance().filesys.EraseFile(FileMap::MemoAttach, filenum);
			    NSEND(service, nick, "MS_COMMAND/TOOBIG");
			}
			else if (!
				 (!filenum || Magick::instance().files.MemoAttachSize() == 0 ||
				  Magick::instance().filesys.FileSysSize(FileMap::MemoAttach) <=
				  Magick::instance().files.MemoAttachSize()))
			{
			    Magick::instance().filesys.EraseFile(FileMap::MemoAttach, filenum);
			    NSEND(service, nick, "MS_COMMAND/NOSPACE");
			}
			else if (recipiant.IsSameAs(realrecipiant, true) ||
				 Magick::instance().nickserv.IsStored(realrecipiant))
			{
			    Memo_t tmp(realrecipiant, sender, text, filenum);

			    Magick::instance().memoserv.AddNickMemo(&tmp);

			    if (filenum)
			    {
				LOG(LM_DEBUG, "MEMOSERV/FILE",
				    (Magick::instance().nickserv.GetLive(sender)->Mask(Nick_Live_t::N_U_P_H),
				     Magick::instance().filesys.GetName(FileMap::MemoAttach, filenum),
				     fmstring("%08x", filenum),
				     ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::MemoAttach, filenum)),
				     realrecipiant));
			    }
			    SEND(service, nick, "MS_COMMAND/SENT", (recipiant, realrecipiant));

			    RLOCK2((lck_MemoServ, lck_nick, realrecipiant.LowerCase()));
			    MemoServ::nick_memo_t & memolist = Magick::instance().memoserv.GetNick(realrecipiant);
			    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(realrecipiant);
			    if (nstored->IsOnline())
				SEND(service, realrecipiant, "MS_COMMAND/NS_NEW", (memolist.size(), service, memolist.size()));
			    unsigned int i;

			    for (i = 0; i < nstored->Siblings(); i++)
			    {
				if (Magick::instance().nickserv.IsStored(nstored->Sibling(i)) &&
				    Magick::instance().nickserv.GetStored(nstored->Sibling(i))->IsOnline())
				{
				    SEND(service, nstored->Sibling(i), "MS_COMMAND/NS_NEW",
					 (memolist.size(), service, memolist.size()));
				}
			    }
			}
			else if (File())
			{
			    Magick::instance().filesys.EraseFile(FileMap::MemoAttach, filenum);
			    NSEND(service, nick, "MS_COMMAND/CANCEL");
			}
		    }
		    else if (File())
		    {
			Magick::instance().filesys.EraseFile(FileMap::MemoAttach, filenum);
			NSEND(service, nick, "MS_COMMAND/CANCEL");
		    }
		}
	    }
	    else if (Picture())
	    {
		if (filenum &&
		    !(Magick::instance().memoserv.FileSize() == 0 ||
		      Magick::instance().filesys.GetSize(FileMap::Picture, filenum) <= Magick::instance().nickserv.PicSize()))
		{
		    Magick::instance().filesys.EraseFile(FileMap::Picture, filenum);
		    NSEND(service, nick, "NS_YOU_COMMAND/TOOBIG");
		}
		else if (filenum &&
			 !(Magick::instance().files.PictureSize() == 0 ||
			   Magick::instance().filesys.FileSysSize(FileMap::Picture) <= Magick::instance().files.PictureSize()))
		{
		    Magick::instance().filesys.EraseFile(FileMap::Picture, filenum);
		    NSEND(service, nick, "NS_YOU_COMMAND/NOSPACE");
		}
		else if (filenum)
		{
		    Magick::instance().nickserv.GetStored(sender)->GotPic(filenum);
		    NSEND(service, nick, "NS_YOU_COMMAND/SAVED");
		    LOG(LM_DEBUG, "NICKSERV/PICTURE_ADD",
			(Magick::instance().nickserv.GetLive(sender)->Mask(Nick_Live_t::N_U_P_H), sender,
			 fmstring("%08x", filenum),
			 ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::Picture, filenum))));
		}
		else
		{
		    SEND(service, nick, "DCC/FAILED", ("GET"));
		}
	    }
	    else if (Public())
	    {
		if (filenum &&
		    !(Magick::instance().files.PublicSize() == 0 ||
		      Magick::instance().filesys.FileSysSize(FileMap::Public) <= Magick::instance().files.PublicSize()))
		{
		    Magick::instance().filesys.EraseFile(FileMap::Public, filenum);
		    NSEND(service, nick, "DCC/NOSPACE");
		}
		else if (filenum)
		{
		    SEND(service, nick, "LIST/ADD",
			 (Magick::instance().filesys.GetName(FileMap::Public, filenum),
			  Magick::instance().getMessage(nick, "LIST/FILES")));
		    Magick::instance().filesys.SetPriv(FileMap::Public, filenum, text);
		    LOG(LM_DEBUG, "SERVMSG/FILE_ADD",
			(Magick::instance().nickserv.GetLive(sender)->Mask(Nick_Live_t::N_U_P_H),
			 Magick::instance().filesys.GetName(FileMap::Public, filenum), fmstring("%08x", filenum),
			 ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::Public, filenum)),
			 ((Magick::instance().filesys.GetPriv(FileMap::Public, filenum).empty()) ? "ALL" :
			   Magick::instance().filesys.GetPriv(FileMap::Public, filenum).c_str())));
		}
		else
		{
		    SEND(service, nick, "DCC/FAILED", ("GET"));
		}
	    }
	}
	init();
    }
    ETCB();
}

void Nick_Live_t::InFlight_t::Picture(const mstring & mynick)
{
    BTCB();
    FT("Nick_Live_t::InFlight_t::Picture", (mynick));
    if (!Magick::instance().nickserv.IsStored(nick))
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/ISNOTSTORED");
	return;
    }
    else if (Magick::instance().nickserv.PicExt().empty())
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/PICDISABLED");
	return;
    }

    if (!
	(Magick::instance().files.TempDirSize() == 0 ||
	 mFile::DirUsage(Magick::instance().files.TempDir()) <= Magick::instance().files.TempDirSize()))
    {
	NSEND(mynick, nick, "DCC/NOSPACE2");
	return;
    }

    if (File())
    {
	if (InProg())
	{
	    NSEND(mynick, nick, "ERR_SITUATION/FILEINPROG");
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

    DumpB();
    {
	WLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight"));
	type = FileMap::Picture;
	fileattach = true;
	sender = nick;
	service = mynick;
    }

    {
	MLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "timer"));
	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);
	timer =
	    Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh, new mstring(sender.LowerCase()),
							ACE_Time_Value(Magick::instance().memoserv.InFlight()));
    }
    DumpE();

    NSEND(mynick, nick, "NS_YOU_COMMAND/PENDING");
    ETCB();
}

void Nick_Live_t::InFlight_t::Public(const mstring & mynick, const mstring & committees)
{
    BTCB();
    FT("Nick_Live_t::InFlight_t::Public", (mynick, committees));
    if (!Magick::instance().nickserv.IsStored(nick))
    {
	NSEND(mynick, nick, "NS_YOU_STATUS/ISNOTSTORED");
	return;
    }

    if (!
	(Magick::instance().files.TempDirSize() == 0 ||
	 mFile::DirUsage(Magick::instance().files.TempDir()) <= Magick::instance().files.TempDirSize()))
    {
	NSEND(mynick, nick, "DCC/NOSPACE2");
	return;
    }

    if (File())
    {
	if (InProg())
	{
	    NSEND(mynick, nick, "ERR_SITUATION/FILEINPROG");
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

    DumpB();
    {
	WLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight"));
	type = FileMap::Public;
	fileattach = true;
	sender = nick;
	service = mynick;
	text = committees;
    }

    {
	MLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "timer"));
	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);
	timer =
	    Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh, new mstring(sender.LowerCase()),
							ACE_Time_Value(Magick::instance().memoserv.InFlight()));
    }
    DumpE();

    NSEND(mynick, nick, "NS_YOU_COMMAND/PUB_PENDING");
    ETCB();
}

mstring Nick_Live_t::InFlight_t::Text()
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::Memo");
    RLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "text"));
    RET(text);
    ETCB();
}

mstring Nick_Live_t::InFlight_t::Recipiant()
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::Memo");
    RLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "recipiant"));
    RET(recipiant);
    ETCB();
}

bool Nick_Live_t::InFlight_t::Memo() const
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::Memo");
    RLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "recipiant"));
    RET(!recipiant.empty());
    ETCB();
}

bool Nick_Live_t::InFlight_t::Picture() const
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::Picture");
    RLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "type"));
    RET(type == FileMap::Picture);
    ETCB();
}

bool Nick_Live_t::InFlight_t::Public() const
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::Public");
    RLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "type"));
    RET(type == FileMap::Public);
    ETCB();
}

bool Nick_Live_t::InFlight_t::Exists() const
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::Exists");
    RLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "recipiant"));
    RLOCK2((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "type"));
    RET(!recipiant.empty() || type != FileMap::MemoAttach);
    ETCB();
}

bool Nick_Live_t::InFlight_t::File() const
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::File");
    RLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "fileattach"));
    RET(fileattach);
    ETCB();
}

bool Nick_Live_t::InFlight_t::InProg() const
{
    BTCB();
    NFT("Nick_Live_t::InFlight_t::InProg");
    RLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight", "fileinprog"));
    RET(fileinprog);
    ETCB();
}

size_t Nick_Live_t::InFlight_t::Usage() const
{
    BTCB();
    size_t retval = 0;

    WLOCK((lck_NickServ, lck_live, nick.LowerCase(), "InFlight"));
    retval += nick.capacity();
    retval += sizeof(type);
    retval += sizeof(timer);
    retval += sizeof(fileattach);
    retval += sizeof(fileinprog);
    retval += sender.capacity();
    retval += recipiant.capacity();
    retval += text.capacity();

    return retval;
    ETCB();
}

void Nick_Live_t::InFlight_t::DumpB() const
{
    BTCB();
    // 8 Elements
    MB(0, (nick, type, timer, fileattach, fileinprog, sender, recipiant, text));
    ETCB();
}

void Nick_Live_t::InFlight_t::DumpE() const
{
    BTCB();
    // 8 Elements
    ME(0, (nick, type, timer, fileattach, fileinprog, sender, recipiant, text));
    ETCB();
}

Nick_Live_t::Nick_Live_t()
: i_Numeric(0), last_msg_entries(0), flood_triggered_times(0), failed_passwds(0), identified(false), services(true)
{
    BTCB();
    NFT("Nick_Live_t::Nick_Live_t");
    // Dont call anything that locks, no names!
    ref_class::lockData(mVarArray(lck_NickServ, lck_live, i_Name.LowerCase()));
    ETCB();
}

Nick_Live_t::Nick_Live_t(const mstring & name, const mDateTime & signon, const mstring & server, const mstring & username,
			 const mstring & hostname, const mstring & realname)
: i_Name(name), i_Numeric(0), i_Signon_Time(signon), i_My_Signon_Time(mDateTime::CurrentDateTime()),
i_Last_Action(mDateTime::CurrentDateTime()), i_realname(realname), i_user(username), i_host(hostname),
i_server(server.LowerCase()), last_msg_entries(0), flood_triggered_times(0), failed_passwds(0), identified(false),
services(false), InFlight(name)
{
    BTCB();
    FT("Nick_Live_t::Nick_Live_t", (name, signon, server, username, hostname, realname));

    ref_class::lockData(mVarArray(lck_NickServ, lck_live, i_Name.LowerCase()));
    InFlight.init();

    // User is on AKILL, add the mask, and No server will kill
    {
	MLOCK((lck_OperServ, "Akill"));
	if (Magick::instance().operserv.Akill_find(i_user + "@" + i_host))
	{
	    mstring reason(Magick::instance().operserv.Akill->Value().second);

	    // Do this cos it will be removed when we KILL,
	    // and we dont wanna get out of touch.
	    Magick::instance().operserv.AddHost(i_host);
	    i_server.erase();
	    i_realname = reason;
	    LOG(LM_INFO, "OTHER/KILL_AKILL", (Mask(N_U_P_H), Magick::instance().operserv.Akill->Entry(), reason));
	    Magick::instance().server.AKILL(Magick::instance().operserv.Akill->Entry(), reason,
					    Magick::instance().operserv.Akill->Value().first -
					    Magick::instance().operserv.Akill->Last_Modify_Time().SecondsSince(),
					    Magick::instance().operserv.Akill->Last_Modifier());
	    return;
	}
    }

    // User triggered CLONE protection, No server will kill
    if (Magick::instance().operserv.AddHost(i_host))
    {
	LOG(LM_INFO, "OTHER/KILL_CLONE", (Mask(N_U_P_H)));
	i_server.erase();
	i_realname = Magick::instance().operserv.Def_Clone();
	return;
    }

    if (Magick::instance().nickserv.IsStored(i_Name))
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(i_Name);
	if (IsRecognized() && !nstored->Secure())
	    nstored->Signon(i_realname, Mask(U_P_H).After("!"));
    }
    DumpE();
    ETCB();
}

Nick_Live_t::Nick_Live_t(const mstring & name, const mstring & username, const mstring & hostname,
			 const mstring & realname) : i_Name(name), i_Numeric(0), i_Signon_Time(mDateTime::CurrentDateTime()),
i_My_Signon_Time(mDateTime::CurrentDateTime()), i_Last_Action(mDateTime::CurrentDateTime()), i_realname(realname),
i_user(username), i_host(hostname), last_msg_entries(0), flood_triggered_times(0), failed_passwds(0),
identified(false), services(true), InFlight(name)
{
    BTCB();
    FT("Nick_Live_t::Nick_Live_t", (name, username, hostname, realname));
    ref_class::lockData(mVarArray(lck_NickServ, lck_live, i_Name.LowerCase()));
    InFlight.init();
    DumpE();
    ETCB();
}

Nick_Live_t &Nick_Live_t::operator=(const Nick_Live_t & in)
{
    BTCB();
    NFT("Nick_Live_t::operator=");
    i_Name = in.i_Name;
    ref_class::lockData(mVarArray(lck_NickServ, lck_live, i_Name.LowerCase()));
    i_Numeric = in.i_Numeric;
    i_Signon_Time = in.i_Signon_Time;
    i_My_Signon_Time = in.i_My_Signon_Time;
    i_Last_Action = in.i_Last_Action;
    i_realname = in.i_realname;
    i_user = in.i_user;
    i_host = in.i_host;
    i_alt_host = in.i_alt_host;
    i_server = in.i_server;
    i_squit = in.i_squit;
    i_away = in.i_away;
    modes = in.modes;
    set < mstring >::const_iterator i;
    joined_channels.clear();
    for (i = in.joined_channels.begin(); i != in.joined_channels.end(); i++)
	joined_channels.insert(*i);
    last_msg_times.empty();
    last_msg_times.reserve(in.last_msg_times.size());
    unsigned int k;

    for (k = 0; k < in.last_msg_times.size(); k++)
	last_msg_times.push_back(in.last_msg_times[k]);
    last_msg_entries = in.last_msg_entries;
    flood_triggered_times = in.flood_triggered_times;
    failed_passwds = in.failed_passwds;
    chans_founder_identd.clear();
    for (i = in.chans_founder_identd.begin(); i != in.chans_founder_identd.end(); i++)
	chans_founder_identd.insert(*i);
    identified = in.identified;
    services = in.services;
    last_nick_reg = in.last_nick_reg;
    last_chan_reg = in.last_chan_reg;
    last_memo = in.last_memo;
    map < mstring, mstring >::const_iterator j;
    i_UserDef.clear();
    for (j = in.i_UserDef.begin(); j != in.i_UserDef.end(); j++)
	i_UserDef.insert(*j);
    InFlight = in.InFlight;
    if (!InFlight.nick.IsSameAs(i_Name))
	InFlight.nick = i_Name;
    NRET(Nick_Live_t &, *this);
    ETCB();
}

void Nick_Live_t::Join(const mstring & chan)
{
    BTCB();
    FT("Nick_Live_t::Join", (chan));
    bool joined = true;

    if (Magick::instance().chanserv.IsLive(chan))
    {
	joined = Magick::instance().chanserv.GetLive(chan)->Join(i_Name);
    }
    else
    {
	map_entry < Chan_Live_t > tmp(new Chan_Live_t(chan, i_Name));
	Magick::instance().chanserv.AddLive(tmp);
    }
    // We do this seperately coz we require initialisation of
    // the channel to be completed.
    if (joined)
    {
	{
	    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
	    MCB(joined_channels.size());
	    joined_channels.insert(chan.LowerCase());
	    MCE(joined_channels.size());
	}
	if (Magick::instance().chanserv.IsStored(chan))
	    Magick::instance().chanserv.GetStored(chan)->Join(i_Name);
    }

    mMessage::CheckDependancies(mMessage::ChanExists, chan);
    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(chan);
    if (clive->Numeric())
	mMessage::CheckDependancies(mMessage::ChanExists,
				    Magick::instance().server.proto.Numeric.ChannelNumeric(clive->Numeric()));
    mMessage::CheckDependancies(mMessage::UserInChan, chan, i_Name);
    if (Numeric())
	mMessage::CheckDependancies(mMessage::UserInChan, chan,
				    "!" + Magick::instance().server.proto.Numeric.UserNumeric(Numeric()));
    if (clive->Numeric())
    {
	mMessage::CheckDependancies(mMessage::UserInChan,
				    Magick::instance().server.proto.Numeric.ChannelNumeric(clive->Numeric()), i_Name);
	if (Numeric())
	    mMessage::CheckDependancies(mMessage::UserInChan,
					Magick::instance().server.proto.Numeric.ChannelNumeric(clive->Numeric()),
					"!" + Magick::instance().server.proto.Numeric.UserNumeric(Numeric()));
    }
    ETCB();
}

void Nick_Live_t::Part(const mstring & chan)
{
    BTCB();
    FT("Nick_Live_t::Part", (chan));

    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
	MCB(joined_channels.size());
	joined_channels.erase(chan.LowerCase());
	MCE(joined_channels.size());
    }

    unsigned int res = 0;
    unsigned long chan_numeric = 0;

    if (Magick::instance().chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
	map_entry < Chan_Live_t > clive(Magick::instance().chanserv.GetLive(chan));
	chan_numeric = clive->Numeric();
	res = clive->Part(i_Name);
	if (!res)
	    Magick::instance().chanserv.RemLive(chan);
    }
    else
    {
	LOG(LM_TRACE, "ERROR/REC_FORNONCHAN", ("PART", i_Name, chan));
    }

    mMessage::CheckDependancies(mMessage::UserNoInChan, chan, i_Name);
    if (Numeric())
	mMessage::CheckDependancies(mMessage::UserInChan, chan,
				    "!" + Magick::instance().server.proto.Numeric.UserNumeric(Numeric()));
    if (chan_numeric)
    {
	mMessage::CheckDependancies(mMessage::UserInChan, Magick::instance().server.proto.Numeric.ChannelNumeric(chan_numeric),
				    i_Name);
	if (Numeric())
	    mMessage::CheckDependancies(mMessage::UserInChan,
					Magick::instance().server.proto.Numeric.ChannelNumeric(chan_numeric),
					"!" + Magick::instance().server.proto.Numeric.UserNumeric(Numeric()));
    }
    if (!res)
    {
	mMessage::CheckDependancies(mMessage::ChanNoExists, chan);
	if (chan_numeric)
	    mMessage::CheckDependancies(mMessage::ChanNoExists,
					Magick::instance().server.proto.Numeric.ChannelNumeric(chan_numeric));
    }
    ETCB();
}

void Nick_Live_t::Kick(const mstring & kicker, const mstring & chan)
{
    BTCB();
    FT("Nick_Live_t::Kick", (kicker, chan));

    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
	MCB(joined_channels.size());
	joined_channels.erase(chan.LowerCase());
	MCE(joined_channels.size());
    }

    unsigned int res = 0;
    unsigned long chan_numeric = 0;

    if (Magick::instance().chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
	map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(chan);
	chan_numeric = clive->Numeric();
	res = clive->Kick(i_Name, kicker);
	if (!res)
	    Magick::instance().chanserv.RemLive(chan);

    }
    else
    {
	LOG(LM_ERROR, "ERROR/REC_FORNONCHAN", ("KICK", kicker, chan));
    }

    mMessage::CheckDependancies(mMessage::UserNoInChan, chan, i_Name);
    if (Numeric())
	mMessage::CheckDependancies(mMessage::UserInChan, chan,
				    "!" + Magick::instance().server.proto.Numeric.UserNumeric(Numeric()));
    if (chan_numeric)
    {
	mMessage::CheckDependancies(mMessage::UserInChan, Magick::instance().server.proto.Numeric.ChannelNumeric(chan_numeric),
				    i_Name);
	if (Numeric())
	    mMessage::CheckDependancies(mMessage::UserInChan,
					Magick::instance().server.proto.Numeric.ChannelNumeric(chan_numeric),
					"!" + Magick::instance().server.proto.Numeric.UserNumeric(Numeric()));
    }
    if (!res)
    {
	mMessage::CheckDependancies(mMessage::ChanNoExists, chan);
	if (chan_numeric)
	    mMessage::CheckDependancies(mMessage::ChanNoExists,
					Magick::instance().server.proto.Numeric.ChannelNumeric(chan_numeric));
    }
    ETCB();
}

void Nick_Live_t::Quit(const mstring & reason)
{
    BTCB();
    FT("Nick_Live_t::Quit", (reason));

    {
	RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_host"));
	if (!(IsServices() || HasMode("o")))
	    Magick::instance().operserv.RemHost(i_host);
    }

    set < mstring > jc;
    set < mstring >::iterator c;
    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
	jc = joined_channels;
	joined_channels.clear();
    }
    for (c = jc.begin(); c != jc.end(); c++)
	Part(*c);

    if (IsServices())
	return;

    unsigned long i;

    {
	RLOCK(("DCC"));
	if (Magick::instance().dcc != NULL)
	{
	    vector < unsigned long > dccs = Magick::instance().dcc->GetList(i_Name);

	    for (i = 0; i < dccs.size(); i++)
		Magick::instance().dcc->Cancel(dccs[i], true);
	}
    }
    if (InFlight.Exists())
	InFlight.End(0u);

    // Ensure our failures dont affect future users ...
    {
	RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "try_chan_ident"));
	for (i = 0; i < try_chan_ident.size(); i++)
	    if (Magick::instance().chanserv.IsStored(try_chan_ident[i]))
		Magick::instance().chanserv.GetStored(try_chan_ident[i])->Quit(i_Name);
    }

    if (Magick::instance().nickserv.IsStored(i_Name))
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(i_Name);
	if (nstored->IsOnline())
	    nstored->Quit(reason);
    }
    ETCB();
}

bool Nick_Live_t::IsInChan(const mstring & chan)
{
    BTCB();
    FT("Nick_Live_t::IsInChan", (chan));
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
    bool retval = (joined_channels.find(chan.LowerCase()) != joined_channels.end());

    RET(retval);
    ETCB();
}

set < mstring > Nick_Live_t::Channels() const
{
    BTCB();
    NFT("Nick_Live_t::Channels");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
    NRET(set < mstring >, joined_channels);
    ETCB();
}

bool Nick_Live_t::FloodTrigger()
{
    BTCB();
    NFT("Nick_Live_t::FloodTrigger");
    bool retval = false;

    // We DONT ignore OPER's
    if (HasMode("o") || IsServices())
    {
	RET(false);
    }

    // Check if we're currently being ignored ...
    {
	MLOCK((lck_OperServ, "Ignore"));
	if (Magick::instance().operserv.Ignore_size())
	{
	    if (Magick::instance().operserv.Ignore_find(Mask(N_U_P_H)))
	    {
		// IF we havnt ignored for long enough yet, or its perminant ...
		if (Magick::instance().operserv.Ignore->Last_Modify_Time().SecondsSince() <=
		    Magick::instance().operserv.Ignore_Time() || Magick::instance().operserv.Ignore->Value() == true)
		{
		    RET(true);
		}
		else
		{
		    Magick::instance().operserv.Ignore_erase();
		}
	    }
	}
    }

    // Clean up previous entries and push current entry
    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "last_msg_times"));
	MCB(last_msg_times.size());
	while (last_msg_times.size() && last_msg_times[0u].SecondsSince() > Magick::instance().operserv.Flood_Time())
	    last_msg_times.erase(last_msg_times.begin());
	last_msg_times.push_back(mDateTime::CurrentDateTime());
	MCE(last_msg_times.size());
    }

    // Check if we just triggered ignore.
    if_RLOCK ((lck_NickServ, lck_live, i_Name.LowerCase(), "last_msg_times"),
	      last_msg_times.size() > Magick::instance().operserv.Flood_Msgs())
    {
	RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "flood_triggered_times"));
	CB(1, flood_triggered_times);
	{
	    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "flood_triggered_times"));
	    flood_triggered_times++;
	}
	// Add To ignore, they're naughty.
	if (flood_triggered_times >= Magick::instance().operserv.Ignore_Limit())
	{
	    Magick::instance().operserv.Ignore_insert(Mask(Magick::instance().operserv.Ignore_Method()), true, i_Name);
	    SEND(Magick::instance().servmsg.FirstName(), i_Name, "ERR_SITUATION/IGNORE_TRIGGER",
		 (Magick::instance().operserv.Flood_Msgs(), ToHumanTime(Magick::instance().operserv.Flood_Time(), i_Name)));
	    SEND(Magick::instance().servmsg.FirstName(), i_Name, "ERR_SITUATION/PERM_IGNORE",
		 (Magick::instance().operserv.Ignore_Limit()));
	    LOG(LM_NOTICE, "OTHER/PERM_IGNORE", (Mask(N_U_P_H)));
	    ANNOUNCE(Magick::instance().servmsg.FirstName(), "MISC/FLOOD_PERM", (i_Name));
	}
	else
	{
	    Magick::instance().operserv.Ignore_insert(Mask(Magick::instance().operserv.Ignore_Method()), false, i_Name);
	    SEND(Magick::instance().servmsg.FirstName(), i_Name, "ERR_SITUATION/IGNORE_TRIGGER",
		 (Magick::instance().operserv.Flood_Msgs(), ToHumanTime(Magick::instance().operserv.Flood_Time(), i_Name)));
	    SEND(Magick::instance().servmsg.FirstName(), i_Name, "ERR_SITUATION/TEMP_IGNORE",
		 (ToHumanNumber(flood_triggered_times), Magick::instance().operserv.Ignore_Limit(),
		  ToHumanTime(Magick::instance().operserv.Ignore_Time(), i_Name)));
	    LOG(LM_NOTICE, "OTHER/TEMP_IGNORE", (Mask(N_U_P_H)));
	    ANNOUNCE(Magick::instance().servmsg.FirstName(), "MISC/FLOOD_TEMP", (i_Name));
	}

	CE(1, flood_triggered_times);
	retval = true;
    }

    RET(retval);
    ETCB();
}

set < mstring > Nick_Live_t::Name(const mstring & in)
{
    BTCB();
    FT("Nick_Live_t::Name", (in));

    InFlight.ChgNick(in);
    set < mstring > wason;

    {
	RLOCK((lck_NickServ, lck_live, in.LowerCase()));
	if (i_Name.IsSameAs(in, true))
	{
	    i_Name = in;
	    NRET(set < mstring >, wason);
	}
    }

    unsigned long i;

    // Store what committee's we WERE on ...
    // This is needed to send logon notices ONLY for committees
    // we have joined by a nick change.
    CommServ::list_t::iterator iter2;
    {
	RLOCK2((lck_CommServ, lck_list));
	for (iter2 = Magick::instance().commserv.ListBegin(); iter2 != Magick::instance().commserv.ListEnd(); iter2++)
	{
	    map_entry < Committee_t > comm(iter2->second);
	    if (comm->IsOn(i_Name))
		wason.insert(comm->Name());
	}
    }

    {
	RLOCK(("DCC"));
	if (Magick::instance().dcc != NULL)
	{
	    vector < unsigned long > dccs = Magick::instance().dcc->GetList(i_Name);

	    for (i = 0; i < dccs.size(); i++)
		Magick::instance().dcc->GetXfers(dccs[i]).ChgNick(in);
	}
    }

    // Carry over failed attempts (so /nick doesnt cure all!)
    // We dont care if it doesnt exist, they can drop channels *shrug*
    {
	RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "try_chan_ident"));
	for (i = 0; i < try_chan_ident.size(); i++)
	{
	    if (Magick::instance().chanserv.IsStored(try_chan_ident[i]))
		Magick::instance().chanserv.GetStored(try_chan_ident[i])->ChgAttempt(i_Name, in);
	}
    }

    MCB(i_Name);

    bool isLinked = false;

    if (Magick::instance().nickserv.IsStored(i_Name))
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(i_Name);
	isLinked = (nstored->IsSibling(in) || nstored->Host().IsSameAs(in, true));
	nstored->ChgNick(in);
    }

    // We are not related (by brotherhood, or parentage)
    if (!isLinked)
    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "identified"));
	WLOCK2((lck_NickServ, lck_live, i_Name.LowerCase(), "chans_founder_identd"));
	WLOCK3((lck_NickServ, lck_live, i_Name.LowerCase(), "failed_paswds"));
	CB(2, identified);
	CB(3, chans_founder_identd.size());
	CB(4, failed_passwds);
	identified = false;
	chans_founder_identd.clear();
	failed_passwds = 0;
	CE(2, identified);
	CE(3, chans_founder_identd.size());
	CE(4, failed_passwds);
    }

    // WooHoo, we have a new nick!
    mstring oldnick(i_Name);

    i_Name = in;
    ref_class::lockData(mVarArray(lck_NickServ, lck_stored, i_Name));
    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_My_Signon_Time"));
	CB(1, i_My_Signon_Time);
	i_My_Signon_Time = mDateTime::CurrentDateTime();
	CE(1, i_My_Signon_Time);
    }

    set < mstring > jc;
    set < mstring >::iterator iter;
    vector < mstring > chunked;
    // Rename ourselves in all channels ...
    {
	RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
	jc = joined_channels;
    }
    for (iter = jc.begin(); iter != jc.end(); iter++)
    {
	if (Magick::instance().chanserv.IsLive(*iter))
	{
	    Magick::instance().chanserv.GetLive(*iter)->ChgNick(oldnick, i_Name);
	}
	else
	{
	    chunked.push_back(*iter);
	    LOG(LM_ERROR, "ERROR/REC_FORNOTINCHAN", ("NICK", oldnick, *iter));
	}
    }

    // Clean up non-existant channels ...
    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
	CB(5, joined_channels.size());
	for (i = 0; i < chunked.size(); i++)
	    joined_channels.erase(chunked[i]);
	CE(5, joined_channels.size());
    }
    MCE(i_Name);

    if (Magick::instance().nickserv.IsStored(i_Name))
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(i_Name);
	if (nstored->Secure() ? identified : IsRecognized())
	    nstored->Signon(i_realname, Mask(U_P_H).After("!"));
    }

    NRET(set < mstring >, wason);
    ETCB();
}

void Nick_Live_t::SendMode(const mstring & in)
{
    BTCB();
    FT("Nick_Live_t::SendMode", (in));

    if (IsServices())
    {
	Magick::instance().server.MODE(i_Name, in);
    }
    else if (!Magick::instance().server.proto.SVSMODE().empty())
    {
	Magick::instance().server.SVSMODE(Magick::instance().nickserv.FirstName(), i_Name, in);
    }
    ETCB();
}

void Nick_Live_t::Mode(const mstring & in)
{
    BTCB();
    FT("Nick_Live_t::Mode", (in));

    bool add = true;

    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "modes"));
    MCB(modes);
    for (unsigned int i = 0; i < in.size(); i++)
    {
	COM(("Processing mode %c%c", add ? '+' : '-', in[i]));
	switch (in[i])
	{
	case ':':
	    break;

	case '+':
	    add = true;
	    break;

	case '-':
	    add = false;
	    break;

	case 'o':
	    // We check the existing modes incase we get
	    // duplicate +o/-o (dont want to overhost it!)
	    if (!IsServices())
	    {
		if (add && !modes.Contains(in[i]))
		{
		    // Store what committee's we WERE on ...
		    // This is needed to send logon notices ONLY for committees
		    // we have joined by a nick change.
		    set < mstring > wason;
		    CommServ::list_t::iterator iter2;
		    {
			RLOCK((lck_CommServ, lck_list));
			for (iter2 = Magick::instance().commserv.ListBegin(); iter2 != Magick::instance().commserv.ListEnd();
			     iter2++)
			{
			    map_entry < Committee_t > comm(iter2->second);
			    if (comm->IsOn(i_Name))
				wason.insert(comm->Name());
			}
		    }

		    modes += in[i];
		    {
			RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_host"));
			Magick::instance().operserv.RemHost(i_host);
		    }
		    // IF we are SecureOper and NOT (on oper list && recoznized)
		    // OR user is on OperDeny and NOT (on sadmin list && recognized)
		    // Yeah, one UUUUUUGLY if.
		    map_entry < Nick_Stored_t > nstored;
		    if (Magick::instance().nickserv.IsStored(i_Name))
			nstored = Magick::instance().nickserv.GetStored(i_Name);
		    if_MLOCK ((lck_OperServ, "OperDeny"),
			      (Magick::instance().operserv.SecureOper() &&
			       !(nstored.entry() != NULL && nstored->IsOnline() &&
				 Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
				 Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(i_Name)))
			      || (Magick::instance().operserv.OperDeny_find(Mask(N_U_P_H)) &&
				  !(nstored.entry() != NULL && nstored->IsOnline() &&
				    Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
				    Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsIn(i_Name))))
		    {
			if (!Magick::instance().server.proto.SVSMODE().empty())
			{
			    SendMode("-o");
			    NSEND(Magick::instance().operserv.FirstName(), i_Name, "OS_STATUS/ISOPERDENY");
			}
			else
			{
			    Magick::instance().server.KILL(Magick::instance().operserv.FirstName(), i_Name,
							   Magick::instance().getMessage(i_Name, "MISC/KILL_OPERDENY"));
			    return;
			}
		    }
		    else
		    {
			mstring setmode;

			{
			    RLOCK((lck_CommServ, lck_list));
			    for (iter2 = Magick::instance().commserv.ListBegin();
				 iter2 != Magick::instance().commserv.ListEnd(); iter2++)
			    {
				map_entry < Committee_t > comm(iter2->second);
				if (comm->IsOn(i_Name) && wason.find(comm->Name()) == wason.end())
				{
				    if (!Magick::instance().server.proto.SVSMODE().empty())
				    {
					if (comm->Name() == Magick::instance().commserv.ALL_Name())
					    setmode += Magick::instance().commserv.ALL_SetMode();
					else if (comm->Name() == Magick::instance().commserv.REGD_Name())
					    setmode += Magick::instance().commserv.REGD_SetMode();
					else if (comm->Name() == Magick::instance().commserv.OPER_Name())
					    setmode += Magick::instance().commserv.OPER_SetMode();
					else if (comm->Name() == Magick::instance().commserv.ADMIN_Name())
					    setmode += Magick::instance().commserv.ADMIN_SetMode();
					else if (comm->Name() == Magick::instance().commserv.SOP_Name())
					    setmode += Magick::instance().commserv.SOP_SetMode();
					else if (comm->Name() == Magick::instance().commserv.SADMIN_Name())
					    setmode += Magick::instance().commserv.SADMIN_SetMode();
				    }

				    MLOCK((lck_CommServ, lck_list, comm->Name(), "message"));
				    for (comm->message = comm->MSG_begin(); comm->message != comm->MSG_end(); comm->message++)
				    {
					Magick::instance().servmsg.send(i_Name,
									"[" + IRC_Bold + comm->Name() + IRC_Off + "] " +
									comm->message->Entry());
				    }
				}
			    }
			}
			if (!setmode.empty())
			{
			    mstring setmode2;

			    for (unsigned int j = 0; j < setmode.size(); j++)
			    {
				if (setmode[j] != '+' && setmode[j] != '-' && setmode[j] != ' ' && !HasMode(setmode[j]))
				    setmode2 += setmode[j];
			    }
			    if (!setmode2.empty())
				SendMode("+" + setmode2);
			}
		    }
		}
		else if (!add && modes.Contains(in[i]))
		{
		    {
			RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_host"));
			Magick::instance().operserv.AddHost(i_host);
		    }
		    modes.Remove(in[i]);
		}
		else
		{
		    LOG(LM_TRACE, "ERROR/MODE_INEFFECT", (add ? '+' : '-', in[i], i_Name, i_Name));
		}
		break;
	    }
	    // WE dont break here coz services will fall through.

	default:
	    if (add && !modes.Contains(in[i]))
	    {
		modes += in[i];
	    }
	    else if (!add && modes.Contains(in[i]))
	    {
		modes.Remove(in[i]);
	    }
	    else
	    {
		LOG(LM_TRACE, "ERROR/MODE_INEFFECT", (add ? '+' : '-', in[i], i_Name, i_Name));
	    }
	    break;
	}
    }
    MCE(modes);
    ETCB();
}

mstring Nick_Live_t::Mode() const
{
    BTCB();
    NFT("Nick_Live_t::Mode");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "modes"));
    RET(modes);
    ETCB();
}

bool Nick_Live_t::HasMode(const mstring & in) const
{
    BTCB();
    FT("Nick_Live_t::HasMode", (in));
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "modes"));
    RET(modes.Contains(in));
    ETCB();
}

void Nick_Live_t::Numeric(const unsigned long in)
{
    BTCB();
    FT("Nick_Live_t::Numeric", (in));
    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_Numeric"));
    MCB(i_Numeric);
    i_Numeric = in;
    MCE(i_Numeric);
    ETCB();
}

unsigned long Nick_Live_t::Numeric() const
{
    BTCB();
    NFT("Nick_Live_t::Numeric");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_Numeric"));
    RET(i_Numeric);
    ETCB();
}

void Nick_Live_t::Away(const mstring & in)
{
    BTCB();
    FT("Nick_Live_t::Away", (in));
    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_away"));
    MCB(i_away);
    i_away = in;
    MCE(i_away);
    ETCB();
}

mstring Nick_Live_t::Away() const
{
    BTCB();
    NFT("Nick_Live_t::Away");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_away"));
    RET(i_away);
    ETCB();
}

mDateTime Nick_Live_t::LastAction() const
{
    BTCB();
    NFT("Nick_Live_t::LastAction");
    if (IsServices())
    {
	RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_Last_Action"));
	RET(i_Last_Action);
    }
    RET(mDateTime::CurrentDateTime());
    ETCB();
}

void Nick_Live_t::Action()
{
    BTCB();
    NFT("Nick_Live_t::Action");
    if (IsServices())
    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_Last_Action"));
	MCB(i_Last_Action);
	i_Last_Action = mDateTime::CurrentDateTime();
	MCE(i_Last_Action);
    }
    ETCB();
}

mDateTime Nick_Live_t::SignonTime() const
{
    BTCB();
    NFT("Nick_Live_t::SignonTime");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_Signon_Time"));
    RET(i_Signon_Time);
    ETCB();
}

mDateTime Nick_Live_t::MySignonTime() const
{
    BTCB();
    NFT("Nick_Live_t::MySignonTime");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_My_Signon_Time"));
    RET(i_My_Signon_Time);
    ETCB();
}

mstring Nick_Live_t::RealName() const
{
    BTCB();
    NFT("Nick_Live_t::RealName");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_realname"));
    RET(i_realname);
    ETCB();
}

mstring Nick_Live_t::User() const
{
    BTCB();
    NFT("Nick_Live_t::User");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_user"));
    RET(i_user);
    ETCB();
}

mstring Nick_Live_t::Host() const
{
    BTCB();
    NFT("Nick_Live_t::Host");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_host"));
    RET(i_host);
    ETCB();
}

mstring Nick_Live_t::AltHost() const
{
    BTCB();
    NFT("Nick_Live_t::AltHost");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_alt_host"));
    RET(i_alt_host);
    ETCB();
}

void Nick_Live_t::AltHost(const mstring & in)
{
    BTCB();
    FT("Nick_Live_t::AltHost", (in));
    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_alt_host"));
    MCB(i_alt_host);
    i_alt_host = in;
    MCE(i_alt_host);
    ETCB();
}

mstring Nick_Live_t::Server() const
{
    BTCB();
    NFT("Nick_Live_t::Server");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_server"));
    RET(i_server);
    ETCB();
}

mstring Nick_Live_t::Squit() const
{
    BTCB();
    NFT("Nick_Live_t::Squit");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_squit"));
    RET(i_squit);
    ETCB();
}

void Nick_Live_t::SetSquit()
{
    BTCB();
    NFT("Nick_Live_t::SetSquit");
    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_squit"));
	RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_server"));
	MCB(i_squit);
	i_squit = i_server;
	MCE(i_squit);
    }

    {
	RLOCK2((lck_NickServ, lck_live, i_Name.LowerCase(), "i_host"));
	if (!IsServices())
	    Magick::instance().operserv.RemHost(i_host);
    }

    set < mstring > jc;
    set < mstring >::iterator c;
    vector < mstring > chunked;
    {
	RLOCK3((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
	jc = joined_channels;
    }
    for (c = jc.begin(); c != jc.end(); c++)
	if (Magick::instance().chanserv.IsLive(*c))
	    Magick::instance().chanserv.GetLive(*c)->SquitUser(i_Name);
	else
	{
	    chunked.push_back(*c);
	    LOG(LM_ERROR, "ERROR/REC_FORNONCHAN", ("SQUIT", i_Name, *c));
	}

    // Clean up non-existant channels ...
    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
	MCB(joined_channels.size());
	for (unsigned int i = 0; i < chunked.size(); i++)
	    joined_channels.erase(chunked[i]);
	MCE(joined_channels.size());
    }
    ETCB();
}

void Nick_Live_t::ClearSquit(const mstring & inmodes)
{
    BTCB();
    NFT("Nick_Live_t::ClearSquit");

    MCB(i_squit);

    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_squit"));
	i_squit.erase();
    }

    // These will all be set again
    {
	WLOCK2((lck_NickServ, lck_live, i_Name.LowerCase(), "modes"));
	CB(1, modes);
	modes = inmodes;
	modes.replace("+", "");
	CE(1, modes);
    }

    set < mstring > jc;
    set < mstring >::iterator i;
    {
	WLOCK3((lck_NickServ, lck_live, i_Name.LowerCase(), "joined_channels"));
	CB(2, joined_channels.size());
	jc = joined_channels;
	joined_channels.clear();
	CE(2, joined_channels.size());
    }
    for (i = jc.begin(); i != jc.end(); i++)
	if (Magick::instance().chanserv.IsLive(*i))
	    Magick::instance().chanserv.GetLive(*i)->UnSquitUser(i_Name);
	else
	{
	    LOG(LM_ERROR, "ERROR/REC_FORNONCHAN", ("UNSQUIT", i_Name, *i));
	}

    if_RLOCK ((lck_NickServ, lck_live, i_Name.LowerCase(), "i_host"), !IsServices() &&
	      Magick::instance().operserv.AddHost(i_host))
    {
	LOG(LM_INFO, "OTHER/KILL_CLONE", (Mask(N_U_P_H)));
	Magick::instance().server.KILL(Magick::instance().nickserv.FirstName(), i_Name,
				       Magick::instance().operserv.Def_Clone());
	return;
    }
    ETCB();
}

mstring Nick_Live_t::Mask(const Nick_Live_t::styles type) const
{
    BTCB();
    FT("Nick_Live_t::Mask", (static_cast < int > (type)));

    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_user"));
    RLOCK2((lck_NickServ, lck_live, i_Name.LowerCase(), "i_host"));
    mstring retval;
    mstring user = i_user;

    switch (type)
    {
    case N:			// nick!*@*
	retval = i_Name + "!*@*";
	break;

    case N_U_P_H:		// nick!user@port.host
	retval = i_Name + "!" + user + "@" + i_host;
	break;

    case N_U_H:		// nick!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_host.IsIpv6Address())
	    retval = i_Name + "!*" + user + "@" + i_host.Before(":", i_host.Occurances(":")) + ":*";
	else if (i_host.IsIpv4Address())
	    retval = i_Name + "!*" + user + "@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
	    retval = i_Name + "!*" + user + "@*." + i_host.After(".");
	else
	    retval = i_Name + "!*" + user + "@" + i_host;
	break;

    case N_P_H:		// nick!*@port.host
	retval = i_Name + "!*@" + i_host;
	break;

    case N_H:			// nick!*@*.host
	if (i_host.IsIpv6Address())
	    retval = i_Name + "!*@" + i_host.Before(":", i_host.Occurances(":")) + ":*";
	else if (i_host.IsIpv4Address())
	    retval = i_Name + "!*@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
	    retval = i_Name + "!*@*." + i_host.After(".");
	else
	    retval = i_Name + "!*@" + i_host;
	break;

    case U_P_H:		// *!user@port.host
	retval = "*!" + user + "@" + i_host;
	break;

    case U_H:			// *!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_host.IsIpv6Address())
	    retval = "*!*" + user + "@" + i_host.Before(":", i_host.Occurances(":")) + ":*";
	else if (i_host.IsIpv4Address())
	    retval = "*!*" + user + "@" + i_host.Before(".", 3) + ".*";
	else if (i_host.Contains(".") && i_host.WordCount(".") > 2)
	    retval = "*!*" + user + "@*." + i_host.After(".");
	else
	    retval = "*!*" + user + "@" + i_host;
	break;

    case P_H:			// *!*@port.host
	retval = "*!*@" + i_host;
	break;

    case H:			// *!*@*.host
	if (i_host.IsIpv6Address())
	    retval = "*!*@" + i_host.Before(":", i_host.Occurances(":")) + ":*";
	else if (i_host.IsIpv4Address())
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
    ETCB();
}

mstring Nick_Live_t::AltMask(const Nick_Live_t::styles type) const
{
    BTCB();
    FT("Nick_Live_t::AltMask", (static_cast < int > (type)));

    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "i_alt_host"));
    mstring retval;

    // If we dont have an althost, use the normal one ...
    if (i_alt_host.empty())
    {
	retval = Mask(type);
	RET(retval);
    }

    RLOCK2((lck_NickServ, lck_live, i_Name.LowerCase(), "i_user"));
    mstring user = i_user;

    switch (type)
    {
    case N:			// nick!*@*
	retval = i_Name + "!*@*";
	break;

    case N_U_P_H:		// nick!user@port.host
	retval = i_Name + "!" + user + "@" + i_alt_host;
	break;

    case N_U_H:		// nick!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_alt_host.IsIpv6Address())
	    retval = i_Name + "!*" + user + "@" + i_alt_host.Before(":", i_alt_host.Occurances(":")) + ":*";
	else if (i_alt_host.IsIpv4Address())
	    retval = i_Name + "!*" + user + "@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = i_Name + "!*" + user + "@*." + i_alt_host.After(".");
	else
	    retval = i_Name + "!*" + user + "@" + i_alt_host;
	break;

    case N_P_H:		// nick!*@port.host
	retval = i_Name + "!*@" + i_alt_host;
	break;

    case N_H:			// nick!*@*.host
	if (i_alt_host.IsIpv6Address())
	    retval = i_Name + "!*@" + i_alt_host.Before(":", i_alt_host.Occurances(":")) + ":*";
	else if (i_alt_host.IsIpv4Address())
	    retval = i_Name + "!*@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = i_Name + "!*@*." + i_alt_host.After(".");
	else
	    retval = i_Name + "!*@" + i_alt_host;
	break;

    case U_P_H:		// *!user@port.host
	retval = "*!" + user + "@" + i_alt_host;
	break;

    case U_H:			// *!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_alt_host.IsIpv6Address())
	    retval = "*!*" + user + "@" + i_alt_host.Before(":", i_alt_host.Occurances(":")) + ":*";
	else if (i_alt_host.IsIpv4Address())
	    retval = "*!*" + user + "@" + i_alt_host.Before(".", 3) + ".*";
	else if (i_alt_host.Contains(".") && i_alt_host.WordCount(".") > 2)
	    retval = "*!*" + user + "@*." + i_alt_host.After(".");
	else
	    retval = "*!*" + user + "@" + i_alt_host;
	break;

    case P_H:			// *!*@port.host
	retval = "*!*@" + i_alt_host;
	break;

    case H:			// *!*@*.host
	if (i_alt_host.IsIpv6Address())
	    retval = "*!*@" + i_alt_host.Before(":", i_alt_host.Occurances(":")) + ":*";
	else if (i_alt_host.IsIpv4Address())
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
    ETCB();
}

mstring Nick_Live_t::ChanIdentify(const mstring & channel, const mstring & password)
{
    BTCB();
    FT("Nick_Live_t::ChanIdentify", (channel, password));
    mstring retval;

    if (Magick::instance().chanserv.IsStored(channel))
    {
	unsigned int failtimes = Magick::instance().chanserv.GetStored(channel)->CheckPass(i_Name, password);

	if (!failtimes)
	{
	    {
		WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "chans_founder_identd"));
		MCB(chans_founder_identd.size());
		chans_founder_identd.insert(channel.LowerCase());
		MCE(chans_founder_identd.size());
	    }
	    retval = parseMessage(Magick::instance().getMessage(i_Name, "CS_COMMAND/IDENTIFIED"), mVarArray(channel));
	}
	else
	{
	    // Keep record of all channel attempts, so that when
	    // we change nick or quit, we can let chanserv know.
	    vector < mstring >::iterator iter;
	    {
		WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "try_chan_ident"));
		for (iter = try_chan_ident.begin(); iter != try_chan_ident.end() && !iter->IsSameAs(channel); iter++);
		if (iter == try_chan_ident.end())
		{
		    MCB(try_chan_ident.size());
		    try_chan_ident.push_back(channel.LowerCase());
		    MCE(try_chan_ident.size());
		}
	    }

	    if (failtimes >= Magick::instance().chanserv.Passfail())
	    {
		LOG(LM_NOTICE, "OTHER/KILL_CHAN_PASS", (Mask(N_U_P_H), channel));
		Magick::instance().server.KILL(Magick::instance().nickserv.FirstName(), i_Name,
					       Magick::instance().getMessage(i_Name, "MISC/KILL_PASS_FAIL"));
		RET("");
	    }
	    else
	    {
		LOG(LM_INFO, "CHANSERV/IDENTIFY_FAILED", (Mask(N_U_P_H), channel));
		retval =
		    parseMessage(Magick::instance().getMessage(i_Name, "ERR_SITUATION/CHAN_WRONG_PASS"), mVarArray(channel));
	    }
	}
    }
    else
    {
	retval = parseMessage(Magick::instance().getMessage(i_Name, "CS_STATUS/ISNOTSTORED"), mVarArray(channel));
    }
    RET(retval);
    ETCB();
}

void Nick_Live_t::UnChanIdentify(const mstring & channel)
{
    BTCB();
    FT("Nick_Live_t::UnChanIdentify", (channel));

    if (IsChanIdentified(channel))
    {
	WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "chans_founder_identd"));
	MCB(chans_founder_identd.size());
	chans_founder_identd.erase(channel.LowerCase());
	MCE(chans_founder_identd.size());
    }
    ETCB();
}

bool Nick_Live_t::IsChanIdentified(const mstring & channel)
{
    BTCB();
    FT("Nick_Live_t::IsChanIdentified", (channel));
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "chans_founder_identd"));
    bool retval = (chans_founder_identd.find(channel.LowerCase()) != chans_founder_identd.end());

    RET(retval);
    ETCB();
}

mstring Nick_Live_t::Identify(const mstring & password)
{
    BTCB();
    FT("Nick_Live_t::Identify", (password));
    mstring retval;

    if_RLOCK ((lck_NickServ, lck_live, i_Name.LowerCase(), "identified"), identified)
    {
	retval = Magick::instance().getMessage(i_Name, "NS_YOU_STATUS/IDENTIFIED");
    }
    else if (Magick::instance().nickserv.IsStored(i_Name))
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(i_Name);
	if (nstored->CheckPass(password))
	{
	    set < mstring > wason;
	    CommServ::list_t::iterator iter;
	    {
		RLOCK((lck_CommServ, lck_list));
		for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		{
		    map_entry < Committee_t > comm(iter->second);
		    if (comm->IsOn(i_Name))
			wason.insert(comm->Name());

		}
	    }
	    {
		WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "identified"));
		WLOCK2((lck_NickServ, lck_live, i_Name.LowerCase(), "failed_passwds"));
		MCB(identified);
		CB(1, failed_passwds);
		identified = true;
		failed_passwds = 0;
		CE(1, failed_passwds);
		MCE(identified);
	    }
	    if (nstored->Secure())
		nstored->Signon(i_realname, Mask(U_P_H).After("!"));

	    // Send notices for committees we were NOT on
	    mstring setmode;

	    {
		RLOCK((lck_CommServ, lck_list));
		for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		{
		    map_entry < Committee_t > comm(iter->second);
		    if (comm->IsOn(i_Name) && wason.find(comm->Name()) == wason.end())
		    {
			if (!Magick::instance().server.proto.SVSMODE().empty())
			{
			    if (comm->Name() == Magick::instance().commserv.ALL_Name())
				setmode += Magick::instance().commserv.ALL_SetMode();
			    else if (comm->Name() == Magick::instance().commserv.REGD_Name())
				setmode += Magick::instance().commserv.REGD_SetMode();
			    else if (comm->Name() == Magick::instance().commserv.OPER_Name())
				setmode += Magick::instance().commserv.OPER_SetMode();
			    else if (comm->Name() == Magick::instance().commserv.ADMIN_Name())
				setmode += Magick::instance().commserv.ADMIN_SetMode();
			    else if (comm->Name() == Magick::instance().commserv.SOP_Name())
				setmode += Magick::instance().commserv.SOP_SetMode();
			    else if (comm->Name() == Magick::instance().commserv.SADMIN_Name())
				setmode += Magick::instance().commserv.SADMIN_SetMode();
			}

			MLOCK((lck_CommServ, lck_list, comm->Name(), "message"));
			for (comm->message = comm->MSG_begin(); comm->message != comm->MSG_end(); comm->message++)
			{
			    Magick::instance().servmsg.send(i_Name,
							    "[" + IRC_Bold + comm->Name() + IRC_Off + "] " +
							    comm->message->Entry());
			}
		    }
		}
	    }
	    if (!setmode.empty())
	    {
		mstring setmode2;

		for (unsigned int j = 0; j < setmode.size(); j++)
		{
		    if (setmode[j] != '+' && setmode[j] != '-' && setmode[j] != ' ' && !HasMode(setmode[j]))
			setmode2 += setmode[j];
		}
		if (!setmode2.empty())
		    SendMode("+" + setmode2);
	    }
	    retval = Magick::instance().getMessage(i_Name, "NS_YOU_COMMAND/IDENTIFIED");
	}
	else
	{
	    {
		WLOCK2((lck_NickServ, lck_live, i_Name.LowerCase(), "failed_passwds"));
		MCB(failed_passwds);
		failed_passwds++;
		MCE(failed_passwds);
	    }
	    if_RLOCK2 ((lck_NickServ, lck_live, i_Name.LowerCase(), "failed_passwds"),
		       failed_passwds >= Magick::instance().nickserv.Passfail())
	    {
		LOG(LM_NOTICE, "OTHER/KILL_NICK_PASS", (Mask(N_U_P_H), i_Name));
		Magick::instance().server.KILL(Magick::instance().nickserv.FirstName(), i_Name,
					       Magick::instance().getMessage(i_Name, "MISC/KILL_PASS_FAIL"));
		RET("");
	    }
	    else
	    {
		LOG(LM_INFO, "NICKSERV/IDENTIFY_FAILED", (Mask(N_U_P_H), i_Name));
		retval = Magick::instance().getMessage(i_Name, "ERR_SITUATION/NICK_WRONG_PASS");
	    }
	}
    }
    else
    {
	retval = Magick::instance().getMessage(i_Name, "NS_YOU_STATUS/ISNOTSTORED");
    }
    RET(retval);
    ETCB();
}

void Nick_Live_t::UnIdentify()
{
    BTCB();
    NFT("Nick_Live_t::UnIdentify");
    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "identified"));
    MCB(identified);
    identified = false;
    MCE(identified);
    ETCB();
}

bool Nick_Live_t::IsIdentified() const
{
    BTCB();
    NFT("Nick_Live_t::IsIdentified");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "identified"));
    RET(identified);
    ETCB();
}

bool Nick_Live_t::IsRecognized() const
{
    BTCB();
    NFT("Nick_Live_t::IsRecognised");
    bool retval = false;

    if (Magick::instance().nickserv.IsStored(i_Name))
    {
	retval = Magick::instance().nickserv.GetStored(i_Name)->IsAccess(Mask(U_P_H).After("!"));
    }
    RET(retval);
    ETCB();
}

bool Nick_Live_t::IsServices() const
{
    BTCB();
    NFT("Nick_Live_t::IsServices");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "services"));
    RET(services);
    ETCB();
}

void Nick_Live_t::SetLastNickReg()
{
    BTCB();
    NFT("Nick_Live_t::SetLastNickReg");
    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "last_nick_reg"));
    MCB(last_nick_reg);
    last_nick_reg = mDateTime::CurrentDateTime();
    MCE(last_nick_reg);
    ETCB();
}

mDateTime Nick_Live_t::LastNickReg() const
{
    BTCB();
    NFT("Nick_Live_t::LastNickReg");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "last_nick_reg"));
    RET(last_nick_reg);
    ETCB();
}

void Nick_Live_t::SetLastChanReg()
{
    BTCB();
    NFT("Nick_Live_t::SetLastChanReg");
    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "last_chan_reg"));
    MCB(last_chan_reg);
    last_chan_reg = mDateTime::CurrentDateTime();
    MCE(last_chan_reg);
    ETCB();
}

mDateTime Nick_Live_t::LastChanReg() const
{
    BTCB();
    NFT("Nick_Live_t::LastChanReg");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "last_chan_reg"));
    RET(last_chan_reg);
    ETCB();
}

void Nick_Live_t::SetLastMemo()
{
    BTCB();
    NFT("Nick_Live_t::SetLastMemo");
    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "last_memo"));
    MCB(last_memo);
    last_memo = mDateTime::CurrentDateTime();
    MCE(last_memo);
    ETCB();
}

mDateTime Nick_Live_t::LastMemo() const
{
    BTCB();
    NFT("Nick_Live_t::LastMemo");
    RLOCK((lck_NickServ, lck_live, i_Name.LowerCase(), "last_memo"));
    RET(last_memo);
    ETCB();
}

size_t Nick_Live_t::Usage() const
{
    BTCB();
    size_t retval = 0;

    // We write lock here coz its the only way to
    // ensure the values are NOT going to change.
    WLOCK((lck_NickServ, lck_live, i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_Numeric);
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
    set < mstring >::const_iterator i;
    for (i = joined_channels.begin(); i != joined_channels.end(); i++)
    {
	retval += i->capacity();
    }
    vector < mDateTime >::const_iterator j;
    for (j = last_msg_times.begin(); j != last_msg_times.end(); j++)
    {
	retval += sizeof(j->Internal());
    }
    retval += sizeof(last_msg_entries);
    retval += sizeof(flood_triggered_times);
    retval += sizeof(failed_passwds);
    for (i = chans_founder_identd.begin(); i != chans_founder_identd.end(); i++)
    {
	retval += i->capacity();
    }
    vector < mstring >::const_iterator k;
    for (k = try_chan_ident.begin(); k != try_chan_ident.end(); k++)
    {
	retval += k->capacity();
    }
    retval += sizeof(identified);
    retval += sizeof(services);
    retval += sizeof(last_nick_reg.Internal());
    retval += sizeof(last_chan_reg.Internal());
    retval += sizeof(last_memo.Internal());

    return retval;
    ETCB();
}

void Nick_Live_t::DumpB() const
{
    BTCB();
    // 16 Elements
    MB(0,
       (i_Name, i_Numeric, i_Signon_Time, i_My_Signon_Time, i_Last_Action, i_realname, i_user, i_host, i_alt_host, i_server,
	i_squit, i_away, modes, joined_channels.size(), last_msg_times.size(), last_msg_entries));
    // 9 Elements
    MB(16,
       (flood_triggered_times, failed_passwds, chans_founder_identd.size(), try_chan_ident.size(), identified, services,
	last_nick_reg, last_chan_reg, last_memo));
    ETCB();
}

void Nick_Live_t::DumpE() const
{
    BTCB();
    // 16 Elements
    ME(0,
       (i_Name, i_Numeric, i_Signon_Time, i_My_Signon_Time, i_Last_Action, i_realname, i_user, i_host, i_alt_host, i_server,
	i_squit, i_away, modes, joined_channels.size(), last_msg_times.size(), last_msg_entries));
    // 9 Elements
    ME(16,
       (flood_triggered_times, failed_passwds, chans_founder_identd.size(), try_chan_ident.size(), identified, services,
	last_nick_reg, last_chan_reg, last_memo));
    ETCB();
}

// =======================================================================

void Nick_Stored_t::Signon(const mstring & realname, const mstring & mask)
{
    BTCB();
    FT("Nick_Stored_t::Signon", (realname, mask));
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastRealName"));
	WLOCK2((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastMask"));
	WLOCK3((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastSeenTime"));
	MCB(i_LastRealName);
	CB(1, i_LastMask);
	CB(2, i_LastSeenTime);
	i_LastRealName = realname;
	i_LastMask = mask;
	i_LastSeenTime = mDateTime::CurrentDateTime();
	CE(1, i_LastMask);
	CE(2, i_LastSeenTime);
	MCE(i_LastRealName);
    }

    mstring who = Host();

    if (who.empty())
	who = i_Name;
    if (Magick::instance().memoserv.IsNick(who))
    {
	unsigned int count = Magick::instance().memoserv.NickMemoCount(who);

	if (count)
	    SEND(Magick::instance().memoserv.FirstName(), i_Name, "MS_STATUS/NS_UNREAD",
		 (count, Magick::instance().memoserv.FirstName()));
    }
    ETCB();
}

void Nick_Stored_t::ChgNick(const mstring & nick)
{
    BTCB();
    FT("Nick_Stored_t::ChgNick", (nick));
    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastQuit"));
    WLOCK2((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastSeenTime"));
    MCB(i_LastQuit);
    CB(1, i_LastSeenTime);
    i_LastQuit = "NICK CHANGE -> " + nick;
    i_LastSeenTime = mDateTime::CurrentDateTime();
    CE(1, i_LastSeenTime);
    MCE(i_LastQuit);
    ETCB();
}

void Nick_Stored_t::defaults()
{
    BTCB();
    NFT("Nick_Stored_t::defaults");

    ref_class::lockData(mVarArray(lck_NickServ, lck_stored, i_Name));
    setting.Protect = Magick::instance().nickserv.DEF_Protect();
    setting.Secure = Magick::instance().nickserv.DEF_Secure();
    setting.NoMemo = Magick::instance().nickserv.DEF_NoMemo();
    setting.Private = Magick::instance().nickserv.DEF_Private();
    setting.PRIVMSG = Magick::instance().nickserv.DEF_PRIVMSG();
    setting.Language = Magick::instance().nickserv.DEF_Language().UpperCase();
    setting.NoExpire = Magick::instance().nickserv.DEF_NoExpire();
    setting.Forbidden = false;
    setting.Picture = 0;

    lock.Protect = false;
    lock.Secure = false;
    lock.NoMemo = false;
    lock.Private = false;
    lock.PRIVMSG = false;
    lock.Language = false;

    ETCB();
}

Nick_Stored_t::Nick_Stored_t()
{
    BTCB();
    NFT("Nick_Stored_t::Nick_Stored_t");
    defaults();
    ETCB();
}

Nick_Stored_t::Nick_Stored_t(const mstring & nick, const mstring & password) : i_Name(nick),
i_RegTime(mDateTime::CurrentDateTime())
{
    BTCB();
    FT("Nick_Stored_t::Nick_Stored_t", (nick, password));

    defaults();
    Password(password);
    if (Magick::instance().nickserv.IsLive(i_Name))
    {
	map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(i_Name);
	i_LastRealName = nlive->RealName();
	i_LastMask = nlive->Mask(Nick_Live_t::U_P_H).After("!");
	i_LastSeenTime = mDateTime::CurrentDateTime();
    }
    DumpE();
    ETCB();
}

Nick_Stored_t::Nick_Stored_t(const mstring & nick) : i_Name(nick), i_RegTime(mDateTime::CurrentDateTime())
{
    BTCB();
    FT("Nick_Stored_t::Nick_Stored_t", (nick.LowerCase()));
    defaults();
    setting.Forbidden = true;
    DumpE();
    ETCB();
}

Nick_Stored_t::Nick_Stored_t(const mstring & nick, const mDateTime & regtime, const Nick_Stored_t & in) : i_Name(nick),
i_RegTime(regtime), i_Host(in.i_Name.LowerCase())
{
    BTCB();
    FT("Nick_Stored_t::Nick_Stored_t", (nick, "(const Nick_Stored_t &) in"));

    defaults();
    if (Magick::instance().nickserv.IsLive(i_Name))
    {
	map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(i_Name);
	i_LastRealName = nlive->RealName();
	i_LastMask = nlive->Mask(Nick_Live_t::U_P_H).After("!");
	i_LastSeenTime = mDateTime::CurrentDateTime();
    }
    DumpE();
    ETCB();
}

mDateTime Nick_Stored_t::RegTime() const
{
    BTCB();
    NFT("Nick_Stored_t::RegTime");
    RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_RegTime"));
    RET(i_RegTime);
    ETCB();
}

unsigned long Nick_Stored_t::Drop()
{
    BTCB();
    NFT("Nick_Stored_t::Drop");

    bool host = Host().empty();

    // When we go, we take all our slaves with us!
    unsigned long i, dropped = 1;

    if (host)
    {
	while (Siblings())
	{
	    mstring nick = Sibling(0);

	    if (!nick.empty())
	    {
		dropped += Magick::instance().nickserv.GetStored(nick)->Drop();
		Magick::instance().nickserv.RemStored(nick);
	    }
	}
    }
    else
    {
	WLOCK2((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_slaves"));
	Magick::instance().nickserv.GetStored(i_Host)->i_slaves.erase(i_Name.LowerCase());
    }

    if (Magick::instance().memoserv.IsNick(i_Name))
	Magick::instance().memoserv.RemNick(i_Name);

    // Now we go for our channels ...
    vector < mstring > killchans;
    ChanServ::stored_t::iterator iter;
    {
	RLOCK((lck_ChanServ, lck_stored));
	for (iter = Magick::instance().chanserv.StoredBegin(); iter != Magick::instance().chanserv.StoredEnd(); iter++)
	{
	    map_entry < Chan_Stored_t > cstored(iter->second);
	    if (cstored->Founder().IsSameAs(i_Name, true))
	    {
		// If we're not host, and own the channel, give it to our host
		// Otherwise it falls to the cofounder, or gets removed.
		if (!host)
		{
		    cstored->Founder(i_Host);
		}
		else if (!cstored->CoFounder().empty() && Magick::instance().nickserv.IsStored(cstored->CoFounder()))
		{
		    cstored->Founder(cstored->CoFounder());
		}
		else
		    killchans.push_back(cstored->Name().LowerCase());
	    }
	    else if (cstored->CoFounder().IsSameAs(i_Name, true))
	    {
		if (!host)
		    cstored->CoFounder(i_Host);
		else
		    cstored->CoFounder("");
	    }
	    MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Access"));
	    if (cstored->Access_find(i_Name))
	    {
		// It must be specifically ours, not a sibling/hosts
		if (cstored->Access->Entry().IsSameAs(i_Name, true))
		{
		    // If we're not a host, give access to the host,
		    // else just remove the access (ie. we are host).
		    if (!host)
		    {
			long value = cstored->Access->Value();
			mstring modifier = cstored->Access->Last_Modifier();
			mDateTime modtime = cstored->Access->Last_Modify_Time();

			cstored->Access_erase();
			cstored->Access_insert(i_Host, value, modifier, modtime);
		    }
		    else
			cstored->Access_erase();
		}
	    }
	}
    }

    for (i = 0; i < killchans.size(); i++)
    {
	Magick::instance().chanserv.RemStored(killchans[i]);
    }

    RET(dropped);
    ETCB();
}

Nick_Stored_t &Nick_Stored_t::operator=(const Nick_Stored_t & in)
{
    BTCB();
    NFT("Nick_Stored_t::operator=");
    i_Name = in.i_Name;
    ref_class::lockData(mVarArray(lck_NickServ, lck_stored, i_Name.LowerCase()));
    i_RegTime = in.i_RegTime;
    i_Password = in.i_Password;
    i_Email = in.i_Email;
    i_URL = in.i_URL;
    i_ICQ = in.i_ICQ;
    i_AIM = in.i_AIM;
    i_MSN = in.i_MSN;
    i_Yahoo = in.i_Yahoo;
    i_Description = in.i_Description;
    i_Comment = in.i_Comment;
    i_Host = in.i_Host;
    set < mstring >::const_iterator i;
    i_slaves.clear();
    for (i = in.i_slaves.begin(); i != in.i_slaves.end(); i++)
	i_slaves.insert(*i);
    i_access.clear();
    for (i = in.i_access.begin(); i != in.i_access.end(); i++)
	i_access.insert(*i);
    i_ignore.clear();
    for (i = in.i_ignore.begin(); i != in.i_ignore.end(); i++)
	i_ignore.insert(*i);
    setting.Protect = in.setting.Protect;
    lock.Protect = in.lock.Protect;
    setting.Secure = in.setting.Secure;
    lock.Secure = in.lock.Secure;
    setting.NoMemo = in.setting.NoMemo;
    lock.NoMemo = in.lock.NoMemo;
    setting.Private = in.setting.Private;
    lock.Private = in.lock.Private;
    setting.PRIVMSG = in.setting.PRIVMSG;
    lock.PRIVMSG = in.lock.PRIVMSG;
    setting.Language = in.setting.Language;
    lock.Language = in.lock.Language;
    setting.NoExpire = in.setting.NoExpire;
    setting.Forbidden = in.setting.Forbidden;
    setting.Picture = in.setting.Picture;
    i_Suspend_By = in.i_Suspend_By;
    i_Suspend_Time = in.i_Suspend_Time;

    // non-slave & offline
    i_LastSeenTime = in.i_LastSeenTime;
    i_LastRealName = in.i_LastRealName;
    i_LastMask = in.i_LastMask;
    i_LastQuit = in.i_LastQuit;
    map < mstring, mstring >::const_iterator j;
    i_UserDef.clear();
    for (j = in.i_UserDef.begin(); j != in.i_UserDef.end(); j++)
	i_UserDef.insert(*j);
    NRET(Nick_Stored_t &, *this);
    ETCB();
}

mstring Nick_Stored_t::Email()
{
    BTCB();
    NFT("Nick_Stored_t::Email");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Email"));
	RET(i_Email);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Email();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::Email(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::Email", (in));
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Email"));
	MCB(i_Email);
	i_Email = in;
	MCE(i_Email);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Email(in);
    }
    ETCB();
}

mstring Nick_Stored_t::URL()
{
    BTCB();
    NFT("Nick_Stored_t::URL");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_URL"));
	RET(i_URL);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->URL();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::URL(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::URL", (in));
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_URL"));
	MCB(i_URL);
	i_URL = in;
	MCE(i_URL);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->URL(in);
    }
    ETCB();
}

mstring Nick_Stored_t::ICQ()
{
    BTCB();
    NFT("Nick_Stored_t::ICQ");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_ICQ"));
	RET(i_ICQ);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->ICQ();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::ICQ(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::ICQ", (in));
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_ICQ"));
	MCB(i_ICQ);
	i_ICQ = in;
	MCE(i_ICQ);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->ICQ(in);
    }
    ETCB();
}

mstring Nick_Stored_t::AIM()
{
    BTCB();
    NFT("Nick_Stored_t::AIM");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_AIM"));
	RET(i_AIM);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->AIM();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::AIM(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::AIM", (in));
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_AIM"));
	MCB(i_AIM);
	i_AIM = in;
	MCE(i_AIM);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->AIM(in);
    }
    ETCB();
}

mstring Nick_Stored_t::MSN()
{
    BTCB();
    NFT("Nick_Stored_t::MSN");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_MSN"));
	RET(i_MSN);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->MSN();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::MSN(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::MSN", (in));
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_MSN"));
	MCB(i_MSN);
	i_MSN = in;
	MCE(i_MSN);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->MSN(in);
    }
    ETCB();
}

mstring Nick_Stored_t::Yahoo()
{
    BTCB();
    NFT("Nick_Stored_t::Yahoo");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Yahoo"));
	RET(i_Yahoo);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Yahoo();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::Yahoo(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::Yahoo", (in));
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Yahoo"));
	MCB(i_Yahoo);
	i_Yahoo = in;
	MCE(i_Yahoo);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Yahoo(in);
    }
    ETCB();
}

mstring Nick_Stored_t::Description()
{
    BTCB();
    NFT("Nick_Stored_t::Description");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Description"));
	RET(i_Description);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Description();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::Description(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::Description", (in));
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Description"));
	MCB(i_Description);
	i_Description = in;
	MCE(i_Description);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Description();
    }
    ETCB();
}

mstring Nick_Stored_t::Comment()
{
    BTCB();
    NFT("Nick_Stored_t::Comment");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Comment"));
	RET(i_Comment);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Comment();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::Comment(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::Comment", (in));
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Comment"));
	MCB(i_Comment);
	i_Comment = in;
	MCE(i_Comment);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Comment(in);
    }
    ETCB();
}

void Nick_Stored_t::Suspend(const mstring & name)
{
    BTCB();
    FT("Nick_Stored_t::Suspend", (name));
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Suspend_By"));
	WLOCK2((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Suspend_Time"));
	MCB(i_Suspend_By);
	CB(1, i_Suspend_Time);
	i_Suspend_By = name;
	i_Suspend_Time = mDateTime::CurrentDateTime();
	CE(1, i_Suspend_Time);
	MCE(i_Suspend_By);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Suspend(name);
    }
    ETCB();
}

void Nick_Stored_t::UnSuspend()
{
    BTCB();
    NFT("Nick_Stored_t::UnSuspend");
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Suspend_By"));
	MCB(i_Suspend_By);
	i_Suspend_By.erase();
	MCE(i_Suspend_By);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->UnSuspend();
    }
    ETCB();
}

mstring Nick_Stored_t::Host()
{
    BTCB();
    NFT("Nick_Stored_t::Host");
    mstring retval;

    RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Host"));
    if (!i_Host.empty())
	if (!Magick::instance().nickserv.IsStored(i_Host))
	{
	    LOG(LM_ERROR, "ERROR/HOST_NOTREGD", (i_Host, i_Name));
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Host"));
	    MCB(i_Host);
	    i_Host.erase();
	    MCE(i_Host);
	}
	else
	    retval = Magick::instance().nickserv.GetStored(i_Host)->Name();
    RET(retval);
    ETCB();
}

mstring Nick_Stored_t::Password()
{
    BTCB();
    NFT("Nick_Stored_t::Password");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Password"));
	RET(i_Password);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Password();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::Password(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::Password", (in));
    if (Host().empty())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Password"));
	MCB(i_Password);
#ifdef GETPASS
	i_Password = in;
#else
	char newpass[33];

	mHASH(in.c_str(), in.length(), newpass);
	i_Password = newpass;
#endif
	MCE(i_Password);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Password(in);
    }
    ETCB();
}

bool Nick_Stored_t::CheckPass(const mstring & password)
{
    BTCB();
    FT("Nick_Stored_t::CheckPass", (password));
    if (Host().empty())
    {
#ifdef GETPASS
	mstring check(password);
#else
	char chkpass[33];

	mHASH(password.c_str(), password.length(), chkpass);
	mstring check(chkpass);
#endif
	if_RLOCK ((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Password"), i_Password == check)
	{
	    RET(true);
	}
	RET(false);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->CheckPass(password);

	RET(retval);
    }
    ETCB();
}

bool Nick_Stored_t::Slave(const mstring & nick, const mstring & password, const mDateTime & regtime)
{
    BTCB();
    FT("Nick_Stored_t::Slave", (nick, password, regtime));

    if (Host().empty())
    {
	{
	    RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Forbidden"));
	    if (setting.Forbidden || !CheckPass(password))
	    {
		RET(false);
	    }
	}

	if (Magick::instance().nickserv.IsStored(nick))
	{
	    ChangeOver(nick);
	    Magick::instance().nickserv.RemStored(nick);
	}

	map_entry < Nick_Stored_t > tmp(new Nick_Stored_t(nick, regtime, *this));
	Magick::instance().nickserv.AddStored(tmp);
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_slaves"));
	MCB(i_slaves.size());
	i_slaves.insert(nick.LowerCase());
	MCE(i_slaves.size());
	RET(true);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->Slave(nick, password, regtime);

	RET(retval);
    }
    ETCB();
}

unsigned int Nick_Stored_t::Siblings()
{
    BTCB();
    NFT("Nick_Stored_t::Siblings");
    unsigned int retval = 0;

    if (Host().empty())
    {
	retval = i_slaves.size();
    }
    else
    {
	retval = Magick::instance().nickserv.GetStored(i_Host)->Siblings();
    }
    RET(retval);
    ETCB();
}

mstring Nick_Stored_t::Sibling(const unsigned int count)
{
    BTCB();
    FT("Nick_Stored_t::Siblings", (count));
    mstring retval;

    if (Host().empty())
    {
	vector < mstring > chunked;
	set < mstring >::iterator iter;
	unsigned int i;

	{
	    RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_slaves"));
	    for (i = 0, iter = i_slaves.begin(); iter != i_slaves.end(); iter++, i++)
	    {
		if (Magick::instance().nickserv.IsStored(*iter))
		{
		    if (i == count)
		    {
			retval = iter->LowerCase();
			break;
		    }
		}
		else
		{
		    LOG(LM_ERROR, "ERROR/SLAVE_NOTREGD", (*iter, i_Name));
		    chunked.push_back(iter->c_str());
		    i--;
		}
	    }
	}
	if (chunked.size())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_slaves"));
	    MCB(i_slaves.size());
	    for (i = 0; i < chunked.size(); i++)
		i_slaves.erase(chunked[i]);
	    MCE(i_slaves.size());
	}
    }
    else
    {
	retval = Magick::instance().nickserv.GetStored(i_Host)->Sibling(count);
    }
    RET(retval);
    ETCB();
}

bool Nick_Stored_t::IsSibling(const mstring & nick)
{
    BTCB();
    FT("Nick_Stored_t::IsSibling", (nick));
    if (i_Name.IsSameAs(nick, true))
    {
	RET(true);
    }
    else if (Host().empty())
    {
	bool retval;

	{
	    RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_slaves"));
	    retval = (i_slaves.find(nick.LowerCase()) != i_slaves.end());
	}
	if (retval && !Magick::instance().nickserv.IsStored(nick))
	{
	    LOG(LM_ERROR, "ERROR/SLAVE_NOTREGD", (nick, i_Name));
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_slaves"));
	    MCB(i_slaves.size());
	    i_slaves.erase(nick);
	    MCE(i_slaves.size());
	    retval = false;
	}
	RET(retval);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->IsSibling(nick);

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::ChangeOver(const mstring & oldnick)
{
    BTCB();
    FT("Nick_Stored_t::ChangeOver", (oldnick));

    bool found;
    mstring modifier, valueS;
    long valueL;
    mDateTime modtime;

    CommServ::list_t::iterator citer;
    {
	RLOCK((lck_CommServ, lck_list));
	for (citer = Magick::instance().commserv.ListBegin(); citer != Magick::instance().commserv.ListEnd(); citer++)
	{
	    map_entry < Committee_t > comm(citer->second);
	    found = false;
	    if (comm->Name() != Magick::instance().commserv.ALL_Name() &&
		comm->Name() != Magick::instance().commserv.REGD_Name() &&
		comm->Name() != Magick::instance().commserv.SADMIN_Name())
	    {
		MLOCK((lck_CommServ, lck_list, comm->Name(), "member"));
		if (comm->find(i_Name))
		{
		    modifier = comm->member->Last_Modifier();
		    modtime = comm->member->Last_Modify_Time();
		    comm->erase();
		    found = true;
		}
		if (comm->find(oldnick))
		{
		    modifier = comm->member->Last_Modifier();
		    modtime = comm->member->Last_Modify_Time();
		    comm->erase();
		    found = true;
		}
		if ((comm->HeadCom().empty()) && (comm->IsHead(i_Name) || comm->IsHead(oldnick)))
		{
		    comm->Head(i_Name);
		    found = false;
		}
		if (found)
		{
		    comm->insert(i_Name, modifier, modtime);
		    comm->member = comm->end();
		}
	    }
	}
    }

    ChanServ::stored_t::iterator csiter;
    {
	RLOCK((lck_ChanServ, lck_stored));
	for (csiter = Magick::instance().chanserv.StoredBegin(); csiter != Magick::instance().chanserv.StoredEnd(); csiter++)
	{
	    map_entry < Chan_Stored_t > cstored(csiter->second);
	    if (cstored->Founder().IsSameAs(oldnick, true))
	    {
		cstored->Founder(i_Name);
		if (cstored->CoFounder().IsSameAs(i_Name, true) || cstored->CoFounder().IsSameAs(oldnick, true))
		{
		    cstored->CoFounder("");
		}
	    }
	    else if (cstored->CoFounder().IsSameAs(oldnick, true))
	    {
		cstored->CoFounder(i_Name);
	    }
	    {
		MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Access"));
		found = false;
		valueL = 0;
		if (cstored->Access_find(i_Name))
		{
		    valueL = cstored->Access->Value();
		    modifier = cstored->Access->Last_Modifier();
		    modtime = cstored->Access->Last_Modify_Time();
		    cstored->Access_erase();
		    found = true;
		}
		if (cstored->Access_find(oldnick))
		{
		    if (cstored->Access->Value() > valueL)
			valueL = cstored->Access->Value();
		    modifier = cstored->Access->Last_Modifier();
		    modtime = cstored->Access->Last_Modify_Time();
		    cstored->Access_erase();
		    found = true;
		}
		if (found)
		{
		    cstored->Access_insert(i_Name, valueL, modifier, modtime);
		    cstored->Access = cstored->Access_end();
		}
	    }
	    {
		MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Akick"));
		found = false;
		if (cstored->Akick_find(i_Name))
		{
		    valueS = cstored->Akick->Value();
		    modifier = cstored->Akick->Last_Modifier();
		    modtime = cstored->Akick->Last_Modify_Time();
		    cstored->Akick_erase();
		    found = true;
		}
		if (cstored->Akick_find(oldnick))
		{
		    valueS = cstored->Akick->Value();
		    modifier = cstored->Akick->Last_Modifier();
		    modtime = cstored->Akick->Last_Modify_Time();
		    cstored->Akick_erase();
		    found = true;
		}
		if (found)
		{
		    cstored->Akick_insert(i_Name, valueS, modifier, modtime);
		    cstored->Akick = cstored->Akick_end();
		}
	    }
	    {
		MLOCK((lck_ChanServ, lck_stored, cstored->Name().LowerCase(), "Greet"));
		found = false;
		if (cstored->Greet_find(i_Name))
		{
		    valueS = cstored->Greet->Entry();
		    modtime = cstored->Greet->Last_Modify_Time();
		    cstored->Greet_erase();
		    found = true;
		}
		if (cstored->Greet_find(oldnick))
		{
		    valueS = cstored->Greet->Entry();
		    modtime = cstored->Greet->Last_Modify_Time();
		    cstored->Greet_erase();
		    found = true;
		}
		if (found)
		{
		    cstored->Greet_insert(valueS, i_Name, modtime);
		    cstored->Greet = cstored->Greet_end();
		}
	    }
	}
    }

    NickServ::stored_t::iterator niter;
    {
	RLOCK((lck_NickServ, lck_stored));
	for (niter = Magick::instance().nickserv.StoredBegin(); niter != Magick::instance().nickserv.StoredEnd(); niter++)
	{
	    map_entry < Nick_Stored_t > nstored(niter->second);
	    if (!nstored->Name().IsSameAs(i_Name, true) && !nstored->Host().IsSameAs(i_Name, true) &&
		!IsSibling(nstored->Name()) && nstored->IsIgnore(oldnick))
	    {
		nstored->IgnoreDel(oldnick);
		nstored->IgnoreAdd(i_Name);
	    }
	}
    }

    if (Magick::instance().memoserv.IsNick(oldnick))
	Magick::instance().memoserv.NickMemoConvert(oldnick, i_Name);

    MemoServ::channel_t::iterator cniter;
    MemoServ::channel_news_t::iterator cnliter;
    {
	RLOCK((lck_MemoServ, lck_channel));
	for (cniter = Magick::instance().memoserv.ChannelBegin(); cniter != Magick::instance().memoserv.ChannelEnd(); cniter++)
	{
	    RLOCK2((lck_MemoServ, lck_channel, cniter->first));
	    for (cnliter = cniter->second.begin(); cnliter != cniter->second.end(); cnliter++)
	    {
		if (cnliter->IsRead(oldnick))
		{
		    cnliter->Unread(oldnick);
		    cnliter->Read(i_Name);
		}
	    }
	}
    }
    ETCB();
}

bool Nick_Stored_t::MakeHost()
{
    BTCB();
    NFT("Nick_Stored_t::MakeHost");
    if (Host().empty())
    {
	RET(false);
    }
    else
    {
	// Re-point all slaves to me and copy the slaves list.
	// Then clear the host's slave list, point host to me,
	// and finally set my host pointer to "".
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase()));
	DumpB();
	map_entry < Nick_Stored_t > host = Magick::instance().nickserv.GetStored(i_Host);
	for (unsigned int i = 0; i < host->Siblings(); i++)
	{
	    if (!host->Sibling(i).IsSameAs(i_Name, true))
	    {
		i_slaves.insert(host->Sibling(i));
		WLOCK2((lck_NickServ, lck_stored, host->Sibling(i).LowerCase(), "i_Host"));
		Magick::instance().nickserv.GetStored(host->Sibling(i))->i_Host = i_Name.LowerCase();
	    }
	}
	i_slaves.insert(i_Host.LowerCase());
	i_Password = host->i_Password;
	i_Email = host->i_Email;
	i_URL = host->i_URL;
	i_ICQ = host->i_ICQ;
	i_AIM = host->i_AIM;
	i_MSN = host->i_MSN;
	i_Yahoo = host->i_Yahoo;
	i_Description = host->i_Description;
	i_Comment = host->i_Comment;
	i_access = host->i_access;
	i_ignore = host->i_ignore;
	setting.Protect = host->setting.Protect;
	lock.Protect = host->lock.Protect;
	setting.Secure = host->setting.Secure;
	lock.Secure = host->lock.Secure;
	setting.NoMemo = host->setting.NoMemo;
	lock.NoMemo = host->lock.NoMemo;
	setting.Private = host->setting.Private;
	lock.Private = host->lock.Private;
	setting.PRIVMSG = host->setting.PRIVMSG;
	lock.PRIVMSG = host->lock.PRIVMSG;
	setting.Language = host->setting.Language;
	lock.Language = host->lock.Language;
	setting.NoExpire = host->setting.NoExpire;
	setting.Forbidden = host->setting.Forbidden;
	setting.Picture = host->setting.Picture;
	i_Suspend_By = host->i_Suspend_By;
	i_Suspend_Time = host->i_Suspend_Time;
	{
	    WLOCK2((lck_NickServ, lck_stored, i_Host.LowerCase(), "i_slaves"));
	    host->i_slaves.clear();
	    host->i_Host = i_Name.LowerCase();
	}
	mstring tmp = i_Host;

	i_Host.erase();
	DumpE();
	ChangeOver(tmp);
	RET(true);
    }
    ETCB();
}

bool Nick_Stored_t::Unlink()
{
    BTCB();
    NFT("Nick_Stored_t::Unlink");
    if (Host().empty())
    {
	RET(false);
    }
    else
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase()));
	DumpB();
	map_entry < Nick_Stored_t > host = Magick::instance().nickserv.GetStored(i_Host);
	i_slaves.clear();
	i_Password = host->i_Password;
	i_Email = host->i_Email;
	i_URL = host->i_URL;
	i_ICQ = host->i_ICQ;
	i_AIM = host->i_AIM;
	i_MSN = host->i_MSN;
	i_Yahoo = host->i_Yahoo;
	i_Description = host->i_Description;
	i_Comment = host->i_Comment;
	i_access = host->i_access;
	i_ignore = host->i_ignore;
	setting.Protect = host->setting.Protect;
	lock.Protect = host->lock.Protect;
	setting.Secure = host->setting.Secure;
	lock.Secure = host->lock.Secure;
	setting.NoMemo = host->setting.NoMemo;
	lock.NoMemo = host->lock.NoMemo;
	setting.Private = host->setting.Private;
	lock.Private = host->lock.Private;
	setting.PRIVMSG = host->setting.PRIVMSG;
	lock.PRIVMSG = host->lock.PRIVMSG;
	setting.Language = host->setting.Language;
	lock.Language = host->lock.Language;
	setting.NoExpire = host->setting.NoExpire;
	setting.Forbidden = host->setting.Forbidden;
	setting.Picture = 0;
	i_Suspend_By = host->i_Suspend_By;
	i_Suspend_Time = host->i_Suspend_Time;
	{
	    WLOCK2((lck_NickServ, lck_stored, i_Host.LowerCase(), "i_slaves"));
	    host->i_slaves.erase(i_Name.LowerCase());
	}
	i_Host.erase();
	DumpE();
	RET(true);
    }
    ETCB();
}

unsigned int Nick_Stored_t::Access()
{
    BTCB();
    NFT("Nick_Stored_t::Access");
    unsigned int retval = 0;

    if (Host().empty())
    {
	retval = i_access.size();
    }
    else
    {
	retval = Magick::instance().nickserv.GetStored(i_Host)->Access();
    }
    RET(retval);
    ETCB();
}

mstring Nick_Stored_t::Access(const unsigned int count)
{
    BTCB();
    FT("Nick_Stored_t::Access", (count));
    if (Host().empty())
    {
	set < mstring >::iterator iter;
	unsigned int i;

	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_access"));
	for (i = 0, iter = i_access.begin(); iter != i_access.end(); i++, iter++)
	    if (i == count)
	    {
		RET(*iter);
	    }
	RET("");
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Access(count);

	RET(retval);
    }
    ETCB();
}

bool Nick_Stored_t::AccessAdd(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::AccessAdd", (in));
    if (Host().empty())
    {
	// Incorrect syntax
	if (!(in.Contains("@") && in.WordCount("@") == 2))
	{
	    RET(false);
	}

	// Already exists (or inclusive)
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_access"));
	set < mstring >::iterator iter;
	for (iter = i_access.begin(); iter != i_access.end(); iter++)
	    if (in.Matches(*iter, true))
	    {
		RET(false);
	    }

	// NEW one includes existing ones ...
	// Remove all obsolite entries.
	vector < mstring > chunked;
	for (iter = i_access.begin(); iter != i_access.end(); iter++)
	    if (iter->Matches(in, true))
	    {
		chunked.push_back(*iter);
	    }
	MCB(i_access.size());
	for (unsigned int i = 0; i < chunked.size(); i++)
	    i_access.erase(chunked[i]);

	i_access.insert(in.LowerCase());
	MCE(i_access.size());
	RET(true);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->AccessAdd(in);

	RET(retval);
    }
    ETCB();
}

unsigned int Nick_Stored_t::AccessDel(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::AccessDel", (in));
    unsigned int retval = 0;

    if (Host().empty())
    {
	vector < mstring > chunked;
	set < mstring >::iterator iter;
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_access"));
	for (iter = i_access.begin(); iter != i_access.end(); iter++)
	    if (in.Matches(*iter, true))
	    {
		chunked.push_back(*iter);
	    }

	MCB(i_access.size());
	for (unsigned int i = 0; i < chunked.size(); i++)
	    i_access.erase(chunked[i].LowerCase());
	MCE(i_access.size());
	retval = chunked.size();
    }
    else
    {
	retval = Magick::instance().nickserv.GetStored(i_Host)->AccessDel(in);
    }
    RET(retval);
    ETCB();
}

bool Nick_Stored_t::IsAccess(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::IsAccess", (in));
    if (Host().empty())
    {
	set < mstring >::iterator iter;
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_access"));
	for (iter = i_access.begin(); iter != i_access.end(); iter++)
	    if (in.Matches(*iter, true))
	    {
		RET(true);
	    }
	RET(false);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->IsAccess(in);

	RET(retval);
    }
    ETCB();
}

unsigned int Nick_Stored_t::Ignore()
{
    BTCB();
    NFT("Nick_Stored_t::Ignore");
    unsigned int retval = 0;

    if (Host().empty())
    {
	retval = i_ignore.size();
    }
    else
    {
	retval = Magick::instance().nickserv.GetStored(i_Host)->Ignore();
    }
    RET(retval);
    ETCB();
}

mstring Nick_Stored_t::Ignore(const unsigned int count)
{
    BTCB();
    FT("Nick_Stored_t::Ignore", (count));
    if (Host().empty())
    {
	set < mstring >::iterator iter;
	unsigned int i;

	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_ignore"));
	for (i = 0, iter = i_ignore.begin(); iter != i_ignore.end(); i++, iter++)
	    if (i == count)
	    {
		RET(*iter);
	    }
	RET("");
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Ignore(count);

	RET(retval);
    }
    ETCB();
}

bool Nick_Stored_t::IgnoreAdd(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::IgnoreAdd", (in));
    if (Host().empty())
    {
	mstring target(in.LowerCase());

	// Not stored nick
	if (!Magick::instance().nickserv.IsStored(target))
	{
	    RET(false);
	}

	if (!Magick::instance().nickserv.GetStored(target)->Host().empty())
	    target = Magick::instance().nickserv.GetStored(target)->Host();

	if (IsIgnore(target))
	{
	    RET(false);
	}

	MCB(i_ignore.size());
	i_ignore.insert(target);
	MCE(i_ignore.size());
	RET(true);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->IgnoreAdd(in);

	RET(retval);
    }
    ETCB();
}

unsigned int Nick_Stored_t::IgnoreDel(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::IgnoreDel", (in));
    unsigned int retval = 0;

    if (Host().empty())
    {
	mstring target(in.LowerCase());

	vector < mstring > chunked;
	set < mstring >::iterator iter;
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_ignore"));
	for (iter = i_ignore.begin(); iter != i_ignore.end(); iter++)
	    if (in.Matches(*iter, true))
	    {
		chunked.push_back(*iter);
	    }

	if (Magick::instance().nickserv.IsStored(in))
	{
	    target = Magick::instance().nickserv.GetStored(in)->Host();
	    if (!target.empty())
	    {
		for (iter = i_ignore.begin(); iter != i_ignore.end(); iter++)
		    if (target.Matches(*iter, true))
		    {
			chunked.push_back(*iter);
		    }
	    }
	}
	MCB(i_ignore.size());
	for (unsigned int i = 0; i < chunked.size(); i++)
	    i_ignore.erase(chunked[i].LowerCase());
	MCE(i_ignore.size());
	retval = chunked.size();
    }
    else
    {
	retval = Magick::instance().nickserv.GetStored(i_Host)->IgnoreDel(in);
    }
    RET(retval);
    ETCB();
}

bool Nick_Stored_t::IsIgnore(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::IsIgnore", (in));
    if (Host().empty())
    {
	mstring target(in.LowerCase());

	set < mstring >::iterator iter;
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_ignore"));
	for (iter = i_ignore.begin(); iter != i_ignore.end(); iter++)
	    if (target.Matches(*iter, true))
	    {
		RET(true);
	    }
	if (Magick::instance().nickserv.IsStored(target))
	{
	    target = Magick::instance().nickserv.GetStored(target)->Host();
	    if (!target.empty())
	    {
		for (iter = i_ignore.begin(); iter != i_ignore.end(); iter++)
		    if (target.Matches(*iter, true))
		    {
			RET(true);
		    }
	    }
	}
	RET(false);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->IsIgnore(in);

	RET(retval);
    }
    ETCB();
}

bool Nick_Stored_t::Protect()
{
    BTCB();
    NFT("Nick_Stored_t::Protect");
    if (Magick::instance().nickserv.LCK_Protect())
    {
	RET(Magick::instance().nickserv.DEF_Protect());
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Protect"));
	RET(setting.Protect);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->Protect();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::Protect(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::Protect", (in));
    if (Host().empty())
    {
	if (!L_Protect())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Protect"));
	    MCB(setting.Protect);
	    setting.Protect = in;
	    MCE(setting.Protect);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Protect(in);
    }
    ETCB();
}

bool Nick_Stored_t::L_Protect()
{
    BTCB();
    NFT("Nick_Stored_t::L_Protect");
    if (Magick::instance().nickserv.LCK_Protect())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.Protect"));
	RET(lock.Protect);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_Protect();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::L_Protect(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::L_Protect", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Protect())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.Protect"));
	    MCB(lock.Protect);
	    lock.Protect = in;
	    MCE(lock.Protect);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_Protect(in);
    }
    ETCB();
}

bool Nick_Stored_t::Secure()
{
    BTCB();
    NFT("Nick_Stored_t::Secure");
    if (Magick::instance().nickserv.LCK_Secure())
    {
	RET(Magick::instance().nickserv.DEF_Secure());
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Secure"));
	RET(setting.Secure);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->Secure();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::Secure(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::Secure", (in));
    if (Host().empty())
    {
	if (!L_Secure())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Secure"));
	    MCB(setting.Secure);
	    setting.Secure = in;
	    MCE(setting.Secure);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Secure(in);
    }
    ETCB();
}

bool Nick_Stored_t::L_Secure()
{
    BTCB();
    NFT("Nick_Stored_t::L_Secure");
    if (Magick::instance().nickserv.LCK_Secure())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.Secure"));
	RET(lock.Secure);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_Secure();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::L_Secure(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::L_Secure", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Secure())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.Secure"));
	    MCB(lock.Secure);
	    lock.Secure = in;
	    MCE(lock.Secure);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_Secure(in);
    }
    ETCB();
}

bool Nick_Stored_t::NoExpire()
{
    BTCB();
    NFT("Nick_Stored_t::NoExpire");
    if (Magick::instance().nickserv.LCK_NoExpire())
    {
	RET(Magick::instance().nickserv.DEF_NoExpire());
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.NoExpire"));
	RET(setting.NoExpire);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->NoExpire();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::NoExpire(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::NoExpire", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_NoExpire())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.NoExpire"));
	    MCB(setting.NoExpire);
	    setting.NoExpire = in;
	    MCE(setting.NoExpire);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->NoExpire(in);
    }
    ETCB();
}

bool Nick_Stored_t::NoMemo()
{
    BTCB();
    NFT("Nick_Stored_t::NoMemo");
    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	RET(Magick::instance().nickserv.DEF_NoMemo());
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.NoMemo"));
	RET(setting.NoMemo);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->NoMemo();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::NoMemo(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::NoMemo", (in));
    if (Host().empty())
    {
	if (!L_NoMemo())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.NoMemo"));
	    MCB(setting.NoMemo);
	    setting.NoMemo = in;
	    MCE(setting.NoMemo);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->NoMemo(in);
    }
    ETCB();
}

bool Nick_Stored_t::L_NoMemo()
{
    BTCB();
    NFT("Nick_Stored_t::L_NoMemo");
    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.NoMemo"));
	RET(lock.NoMemo);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_NoMemo();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::L_NoMemo(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::L_NoMemo", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_NoMemo())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.NoMemo"));
	    MCB(lock.NoMemo);
	    lock.NoMemo = in;
	    MCE(lock.NoMemo);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_NoMemo(in);
    }
    ETCB();
}

bool Nick_Stored_t::Private()
{
    BTCB();
    NFT("Nick_Stored_t::Private");
    if (Magick::instance().nickserv.LCK_Private())
    {
	RET(Magick::instance().nickserv.DEF_Private());
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Private"));
	RET(setting.Private);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->Private();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::Private(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::Private", (in));
    if (Host().empty())
    {
	if (!L_Private())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Private"));
	    MCB(setting.Private);
	    setting.Private = in;
	    MCE(setting.Private);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Private(in);
    }
    ETCB();
}

bool Nick_Stored_t::L_Private()
{
    BTCB();
    NFT("Nick_Stored_t::L_Private");
    if (Magick::instance().nickserv.LCK_Private())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.Private"));
	RET(lock.Private);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_Private();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::L_Private(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::L_Private", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Private())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.Private"));
	    MCB(lock.Private);
	    lock.Private = in;
	    MCE(lock.Private);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_Private(in);
    }
    ETCB();
}

bool Nick_Stored_t::PRIVMSG()
{
    BTCB();
    NFT("Nick_Stored_t::PRIVMSG");
    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	RET(Magick::instance().nickserv.DEF_PRIVMSG());
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.PRIVMSG"));
	RET(setting.PRIVMSG);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->PRIVMSG();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::PRIVMSG(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::PRIVMSG", (in));
    if (Host().empty())
    {
	if (!L_PRIVMSG())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.PRIVMSG"));
	    MCB(setting.PRIVMSG);
	    setting.PRIVMSG = in;
	    MCE(setting.PRIVMSG);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->PRIVMSG(in);
    }
    ETCB();
}

bool Nick_Stored_t::L_PRIVMSG()
{
    BTCB();
    NFT("Nick_Stored_t::L_PRIVMSG");
    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.PRIVMSG"));
	RET(lock.PRIVMSG);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_PRIVMSG();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::L_PRIVMSG(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::L_PRIVMSG", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_PRIVMSG())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.PRIVMSG"));
	    MCB(lock.PRIVMSG);
	    lock.PRIVMSG = in;
	    MCE(lock.PRIVMSG);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_PRIVMSG(in);
    }
    ETCB();
}

mstring Nick_Stored_t::Language()
{
    BTCB();
    NFT("Nick_Stored_t::Language");
    if (Magick::instance().nickserv.LCK_Language())
    {
	RET(Magick::instance().nickserv.DEF_Language().UpperCase());
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Language"));
	RET(setting.Language);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Language();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::Language(const mstring & in)
{
    BTCB();
    FT("Nick_Stored_t::Language", (in));
    if (Host().empty())
    {
	if (!L_Language())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Language"));
	    MCB(setting.Language);
	    setting.Language = in.UpperCase();
	    MCE(setting.Language);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Language(in);
    }
    ETCB();
}

bool Nick_Stored_t::L_Language()
{
    BTCB();
    NFT("Nick_Stored_t::L_Language");
    if (Magick::instance().nickserv.LCK_Language())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.Language"));
	RET(lock.Language);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_Language();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::L_Language(const bool in)
{
    BTCB();
    FT("Nick_Stored_t::L_Language", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Language())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "lock.Language"));
	    MCB(lock.Language);
	    lock.Language = in;
	    MCE(lock.Language);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_Language(in);
    }
    ETCB();
}

bool Nick_Stored_t::Suspended()
{
    BTCB();
    NFT("Nick_Stored_t::Suspended");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Suspend_By"));
	RET(!Suspend_By().empty());
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->Suspended();

	RET(retval);
    }
    ETCB();
}

mstring Nick_Stored_t::Suspend_By()
{
    BTCB();
    NFT("Nick_Stored_t::Suspend_By");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Suspend_By"));
	RET(i_Suspend_By);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Suspend_By();

	RET(retval);
    }
    ETCB();
}

mDateTime Nick_Stored_t::Suspend_Time()
{
    BTCB();
    NFT("Nick_Stored_t::Suspend_Time");
    if (Host().empty())
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_Suspend_Time"));
	RET(i_Suspend_Time);
    }
    else
    {
	mDateTime retval = Magick::instance().nickserv.GetStored(i_Host)->Suspend_Time();

	RET(retval);
    }
    ETCB();
}

bool Nick_Stored_t::Forbidden() const
{
    BTCB();
    NFT("Nick_Stored_t::Forbidden");
    RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Forbidden"));
    RET(setting.Forbidden);
    ETCB();
}

unsigned long Nick_Stored_t::PicNum()
{
    BTCB();
    NFT("PicNum");
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.PicExt().empty())
	{
	    RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Picture"));
	    RET(setting.Picture);
	}
	RET(0);
    }
    else
    {
	unsigned long retval = Magick::instance().nickserv.GetStored(i_Host)->PicNum();

	RET(retval);
    }
    ETCB();
}

void Nick_Stored_t::GotPic(const unsigned long picnum)
{
    BTCB();
    FT("Nick_Stored_t::GotPic", (picnum));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.PicExt().empty())
	{
	    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "setting.Picture"));
	    MCB(setting.Picture);
	    setting.Picture = picnum;
	    MCE(setting.Picture);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->GotPic(picnum);
    }
    ETCB();
}

bool Nick_Stored_t::IsOnline()
{
    BTCB();
    NFT("Nick_Stored_t::IsOnline");

    if (Magick::instance().nickserv.IsLive(i_Name))
    {
	map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(i_Name);

	if (nlive->IsServices())
	{
	    RET(false);
	}

	// Not secure and recognized
	// or not suspended and identified
	if ((!Suspended() && nlive->IsIdentified()) || (!Secure() && nlive->IsRecognized()))
	{
	    RET(true);
	}
    }
    RET(false);
    ETCB();
}

mDateTime Nick_Stored_t::LastAllSeenTime()
{
    BTCB();
    NFT("Nick_Stored_t::LastAllSeenTime");
    if (IsOnline())
    {
	RET(mDateTime::CurrentDateTime());
    }
    else if (Host().empty())
    {
	mDateTime lastseen;

	{
	    RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastSeenTime"));
	    lastseen = i_LastSeenTime;
	}
	unsigned int i;

	for (i = 0; i < Siblings(); i++)
	{
	    if (!Magick::instance().nickserv.IsStored(Sibling(i)))
		continue;

	    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(Sibling(i));
	    if (nstored->LastSeenTime() > lastseen)
		lastseen = nstored->LastSeenTime();
	}
	RET(lastseen);
    }
    else
    {
	mDateTime retval = Magick::instance().nickserv.GetStored(i_Host)->LastAllSeenTime();

	RET(retval);
    }
    ETCB();
}

mDateTime Nick_Stored_t::LastSeenTime()
{
    BTCB();
    NFT("Nick_Stored_t::LastSeenTime");
    if (IsOnline())
    {
	RET(mDateTime::CurrentDateTime());
    }
    else
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastSeenTime"));
	RET(i_LastSeenTime);
    }
    ETCB();
}

mstring Nick_Stored_t::LastRealName()
{
    BTCB();
    NFT("Nick_Stored_t::LastRealName");
    if (IsOnline())
    {
	mstring retval = Magick::instance().nickserv.GetLive(i_Name)->RealName();

	RET(retval);
    }
    else
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastRealName"));
	RET(i_LastRealName);
    }
    ETCB();
}

mstring Nick_Stored_t::LastAllMask()
{
    BTCB();
    NFT("Nick_Stored_t::LastAllMask");
    if (IsOnline())
    {
	RET(Magick::instance().getMessage("VALS/ONLINE"));
    }
    else if (Host().empty())
    {
	mDateTime lastseen;

	{
	    RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastSeenTime"));
	    lastseen = i_LastSeenTime;
	}
	mstring lastmask = Name() + "!" + LastMask();
	unsigned int i;

	for (i = 0; i < Siblings(); i++)
	{
	    if (!Magick::instance().nickserv.IsStored(Sibling(i)))
		continue;

	    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(Sibling(i));
	    if (nstored->IsOnline())
	    {
		RET(Magick::instance().getMessage("VALS/ONLINE"));
	    }
	    if (nstored->LastSeenTime() > lastseen)
	    {
		lastseen = nstored->LastSeenTime();
		lastmask = nstored->Name() + "!" + nstored->LastMask();
	    }
	}
	RET(lastmask);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->LastAllMask();

	RET(retval);
    }
    ETCB();
}

mstring Nick_Stored_t::LastMask()
{
    BTCB();
    NFT("Nick_Stored_t::LastMask");
    if (IsOnline())
    {
	RET(Magick::instance().getMessage("VALS/ONLINE"));
    }
    else
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastMask"));
	RET(i_LastMask);
    }
    ETCB();
}

mstring Nick_Stored_t::LastQuit()
{
    BTCB();
    NFT("Nick_Stored_t::LastQuit");
    if (IsOnline())
    {
	RET("");
    }
    else
    {
	RLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastQuit"));
	RET(i_LastQuit);
    }
    ETCB();
}

void Nick_Stored_t::Quit(const mstring & message)
{
    BTCB();
    FT("Nick_Stored_t::Quit", (message));

    // Dont whinge here, because they may not be
    // ident'd, in which case IsOnline() == false.
    if (IsOnline())
    {
	WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastSeenTime"));
	WLOCK2((lck_NickServ, lck_stored, i_Name.LowerCase(), "i_LastQuit"));
	MCB(i_LastSeenTime);
	CB(1, i_LastQuit);
	i_LastSeenTime = mDateTime::CurrentDateTime();
	i_LastQuit = message;
	CE(1, i_LastQuit);
	MCE(i_LastSeenTime);
    }
    ETCB();
}

size_t Nick_Stored_t::MyChannels() const
{
    BTCB();
    NFT("Nick_Stored_t::MyChannels");

    size_t count = 0;

    ChanServ::stored_t::const_iterator i;
    RLOCK((lck_ChanServ, lck_stored));
    for (i = Magick::instance().chanserv.StoredBegin(); i != Magick::instance().chanserv.StoredEnd(); i++)
    {
	map_entry < Chan_Stored_t > cstored(i->second);
	if (cstored->Founder().IsSameAs(i_Name, true) || cstored->CoFounder().IsSameAs(i_Name, true))
	    count++;
    }
    RET(count);
    ETCB();
}

SXP::Tag Nick_Stored_t::tag_Nick_Stored_t("Nick_Stored_t");
SXP::Tag Nick_Stored_t::tag_Name("Name");
SXP::Tag Nick_Stored_t::tag_RegTime("RegTime");
SXP::Tag Nick_Stored_t::tag_Password("Password");
SXP::Tag Nick_Stored_t::tag_Email("EMail");
SXP::Tag Nick_Stored_t::tag_URL("URL");
SXP::Tag Nick_Stored_t::tag_ICQ("ICQ");
SXP::Tag Nick_Stored_t::tag_AIM("AIM");
SXP::Tag Nick_Stored_t::tag_MSN("MSN");
SXP::Tag Nick_Stored_t::tag_Yahoo("Yahoo");
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
    BTCB();
    static_cast < void > (pIn);

    FT("Nick_Stored_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if (pElement->IsA(tag_UserDef))
    {
	mstring *tmp = new mstring;

	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
    ETCB();
}

void Nick_Stored_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

    FT("Nick_Stored_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
    if (pElement->IsA(tag_Name))
    {
	pElement->Retrieve(i_Name);
	ref_class::lockData(mVarArray(lck_NickServ, lck_stored, i_Name.LowerCase()));
    }
    if (pElement->IsA(tag_RegTime))
	pElement->Retrieve(i_RegTime);
    if (pElement->IsA(tag_Password))
    {
#ifdef GETPASS
	if (atoi(pElement->Attrib("hash")))
	{
	    NLOG(LM_EMERGENCY, "ERROR/WRONG_PASS_TYPE");
	}
#else
	// If password was stored clear, but we use one-way, change it.
	if (!atoi(pElement->Attrib("hash")))
	{
	    mstring clearpass;

	    pElement->Retrieve(clearpass);
	    char newpass[33];

	    mHASH(clearpass.c_str(), clearpass.length(), newpass);
	    i_Password = newpass;
	}
#endif
	else
	    pElement->Retrieve(i_Password);
    }
    if (pElement->IsA(tag_Email))
	pElement->Retrieve(i_Email);
    if (pElement->IsA(tag_URL))
	pElement->Retrieve(i_URL);
    if (pElement->IsA(tag_ICQ))
	pElement->Retrieve(i_ICQ);
    if (pElement->IsA(tag_AIM))
	pElement->Retrieve(i_AIM);
    if (pElement->IsA(tag_MSN))
	pElement->Retrieve(i_MSN);
    if (pElement->IsA(tag_Yahoo))
	pElement->Retrieve(i_Yahoo);
    if (pElement->IsA(tag_Description))
	pElement->Retrieve(i_Description);
    if (pElement->IsA(tag_Comment))
	pElement->Retrieve(i_Comment);
    if (pElement->IsA(tag_Host))
	pElement->Retrieve(i_Host);
    if (pElement->IsA(tag_set_Protect))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Protect = tmp;
    }
    if (pElement->IsA(tag_set_Secure))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Secure = tmp;
    }
    if (pElement->IsA(tag_set_NoExpire))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.NoExpire = tmp;
    }
    if (pElement->IsA(tag_set_NoMemo))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.NoMemo = tmp;
    }
    if (pElement->IsA(tag_set_Private))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Private = tmp;
    }
    if (pElement->IsA(tag_set_PRIVMSG))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.PRIVMSG = tmp;
    }
    if (pElement->IsA(tag_Forbidden))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	setting.Forbidden = tmp;
    }
    if (pElement->IsA(tag_lock_Protect))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Protect = tmp;
    }
    if (pElement->IsA(tag_lock_Secure))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Secure = tmp;
    }
    if (pElement->IsA(tag_lock_NoMemo))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.NoMemo = tmp;
    }
    if (pElement->IsA(tag_lock_Private))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Private = tmp;
    }
    if (pElement->IsA(tag_lock_PRIVMSG))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.PRIVMSG = tmp;
    }
    if (pElement->IsA(tag_lock_Language))
    {
	bool tmp;

	pElement->Retrieve(tmp);
	lock.Language = tmp;
    }
    if (pElement->IsA(tag_set_Language))
	pElement->Retrieve(setting.Language);
    if (pElement->IsA(tag_Picture))
	pElement->Retrieve(setting.Picture);
    if (pElement->IsA(tag_Suspend_By))
	pElement->Retrieve(i_Suspend_By);
    if (pElement->IsA(tag_Suspend_Time))
	pElement->Retrieve(i_Suspend_Time);
    if (pElement->IsA(tag_LastSeenTime))
	pElement->Retrieve(i_LastSeenTime);
    if (pElement->IsA(tag_LastRealName))
	pElement->Retrieve(i_LastRealName);
    if (pElement->IsA(tag_LastMask))
	pElement->Retrieve(i_LastMask);
    if (pElement->IsA(tag_LastQuit))
	pElement->Retrieve(i_LastQuit);

    if (pElement->IsA(tag_Access))
    {
	mstring tmp;

	pElement->Retrieve(tmp);
	i_access.insert(tmp);
    }
    if (pElement->IsA(tag_Ignore))
    {
	mstring tmp;

	pElement->Retrieve(tmp);
	i_ignore.insert(tmp);
    }
    ETCB();
}

void Nick_Stored_t::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    //TODO: Add your source code here
    pOut->BeginObject(tag_Nick_Stored_t);

    // Only way to ENSURE the data wont change.
    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase()));
    pOut->WriteElement(tag_Name, i_Name);
    pOut->WriteElement(tag_RegTime, i_RegTime);
    {
	SXP::dict attr;
#ifdef GETPASS
	attr["hash"] = "0";
#else
	attr["hash"] = "1";
#endif
	pOut->WriteElement(tag_Password, i_Password, attr);
    }
    pOut->WriteElement(tag_Email, i_Email);
    pOut->WriteElement(tag_URL, i_URL);
    pOut->WriteElement(tag_ICQ, i_ICQ);
    pOut->WriteElement(tag_AIM, i_AIM);
    pOut->WriteElement(tag_MSN, i_MSN);
    pOut->WriteElement(tag_Yahoo, i_Yahoo);
    pOut->WriteElement(tag_Description, i_Description);
    pOut->WriteElement(tag_Comment, i_Comment);
    pOut->WriteElement(tag_Host, i_Host);
    pOut->WriteElement(tag_set_Protect, setting.Protect);
    pOut->WriteElement(tag_set_Secure, setting.Secure);
    pOut->WriteElement(tag_set_NoExpire, setting.NoExpire);
    pOut->WriteElement(tag_set_NoMemo, setting.NoMemo);
    pOut->WriteElement(tag_set_Private, setting.Private);
    pOut->WriteElement(tag_set_PRIVMSG, setting.PRIVMSG);
    pOut->WriteElement(tag_set_Language, setting.Language);
    pOut->WriteElement(tag_Forbidden, setting.Forbidden);
    pOut->WriteElement(tag_lock_Protect, lock.Protect);
    pOut->WriteElement(tag_lock_Secure, lock.Secure);
    pOut->WriteElement(tag_lock_NoMemo, lock.NoMemo);
    pOut->WriteElement(tag_lock_Private, lock.Private);
    pOut->WriteElement(tag_lock_PRIVMSG, lock.PRIVMSG);
    pOut->WriteElement(tag_lock_Language, lock.Language);
    pOut->WriteElement(tag_Picture, setting.Picture);
    pOut->WriteElement(tag_Suspend_By, i_Suspend_By);
    pOut->WriteElement(tag_Suspend_Time, i_Suspend_Time);
    pOut->WriteElement(tag_LastSeenTime, i_LastSeenTime);
    pOut->WriteElement(tag_LastRealName, i_LastRealName);
    pOut->WriteElement(tag_LastMask, i_LastMask);
    pOut->WriteElement(tag_LastQuit, i_LastQuit);

    set < mstring >::const_iterator iter2;
    for (iter2 = i_access.begin(); iter2 != i_access.end(); iter2++)
    {
	pOut->WriteElement(tag_Access, *iter2);
    }
    for (iter2 = i_ignore.begin(); iter2 != i_ignore.end(); iter2++)
    {
	pOut->WriteElement(tag_Ignore, *iter2);
    }

    map < mstring, mstring >::const_iterator iter;
    for (iter = i_UserDef.begin(); iter != i_UserDef.end(); iter++)
    {
	pOut->WriteElement(tag_UserDef, iter->first + "\n" + iter->second);
    }

    pOut->EndObject(tag_Nick_Stored_t);
    ETCB();
}

size_t Nick_Stored_t::Usage()
{
    BTCB();
    size_t retval = 0;

    // Only way to ENSURE the data wont change.
    WLOCK((lck_NickServ, lck_stored, i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += sizeof(i_RegTime.Internal());
    retval += i_Password.capacity();
    retval += i_Email.capacity();
    retval += i_URL.capacity();
    retval += i_ICQ.capacity();
    retval += i_AIM.capacity();
    retval += i_MSN.capacity();
    retval += i_Yahoo.capacity();
    retval += i_Description.capacity();
    retval += i_Comment.capacity();
    retval += i_Host.capacity();

    set < mstring >::iterator i;
    for (i = i_slaves.begin(); i != i_slaves.end(); i++)
    {
	retval += i->capacity();
    }
    for (i = i_access.begin(); i != i_access.end(); i++)
    {
	retval += i->capacity();
    }
    for (i = i_ignore.begin(); i != i_ignore.end(); i++)
    {
	retval += i->capacity();
    }

    retval += sizeof(setting);
    retval += sizeof(lock);
    retval += setting.Language.capacity();

    retval += i_Suspend_By.capacity();
    retval += sizeof(i_Suspend_Time.Internal());

    retval += sizeof(i_LastSeenTime.Internal());
    retval += i_LastRealName.capacity();
    retval += i_LastMask.capacity();
    retval += i_LastQuit.capacity();

    map < mstring, mstring >::iterator j;
    for (j = i_UserDef.begin(); j != i_UserDef.end(); j++)
    {
	retval += j->first.capacity();
	retval += j->second.capacity();
    }

    return retval;
    ETCB();
}

void Nick_Stored_t::DumpB()
{
    BTCB();
    MB(0,
       (i_Name, i_RegTime, i_Password, i_Email, i_URL, i_ICQ, i_AIM, i_MSN, i_Yahoo, i_Description, i_Comment, i_Host,
	i_slaves.size(), i_access.size(), i_ignore.size(), setting.Protect));
    MB(16,
       (lock.Protect, setting.Secure, lock.Secure, setting.NoExpire, setting.NoMemo, lock.NoMemo, setting.Private,
	lock.Private, setting.PRIVMSG, lock.PRIVMSG, setting.Language, lock.Language, setting.Forbidden, setting.Picture,
	i_Suspend_By, i_Suspend_Time));
    MB(32, (i_LastSeenTime, i_LastRealName, i_LastMask, i_LastQuit, i_UserDef.size()));
    ETCB();
}

void Nick_Stored_t::DumpE()
{
    BTCB();
    ME(0,
       (i_Name, i_RegTime, i_Password, i_Email, i_URL, i_ICQ, i_AIM, i_MSN, i_Yahoo, i_Description, i_Comment, i_Host,
	i_slaves.size(), i_access.size(), i_ignore.size(), setting.Protect));
    ME(16,
       (lock.Protect, setting.Secure, lock.Secure, setting.NoExpire, setting.NoMemo, lock.NoMemo, setting.Private,
	lock.Private, setting.PRIVMSG, lock.PRIVMSG, setting.Language, lock.Language, setting.Forbidden, setting.Picture,
	i_Suspend_By, i_Suspend_Time));
    ME(32, (i_LastSeenTime, i_LastRealName, i_LastMask, i_LastQuit, i_UserDef.size()));
    ETCB();
}
