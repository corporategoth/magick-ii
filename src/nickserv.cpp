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
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_nickserv_cpp_ ## x () { return y; }
RCSID(nickserv_cpp, "@(#)$Id$");

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
#include "dccengine.h"

#ifndef MAGICK_HAS_EXCEPTIONS
static Nick_Stored_t GLOB_Nick_Stored_t;
static Nick_Live_t GLOB_Nick_Live_t;
static mDateTime GLOB_mDateTime;
#endif

void Nick_Live_t::InFlight_t::ChgNick(const mstring & newnick)
{
    FT("Nick_Live_t::InFlight_t::ChgNick", (newnick));
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
    WLOCK2(("NickServ", "live", newnick.LowerCase(), "InFlight"));
    MCB(nick);
    nick = newnick;
    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
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
}

Nick_Live_t::InFlight_t & Nick_Live_t::InFlight_t::operator=(const InFlight_t & in)
{
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
    mstring *arg = NULL;

    if (timer)
    {
	if (Magick::instance().reactor().cancel_timer(timer, reinterpret_cast < const void ** > (arg)) && arg != NULL)
	{
	    delete arg;
	}
	timer = 0;
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
    service.erase();
    sender.erase();
    recipiant.erase();
    text.erase();
}

// We have completed a file transfer, or errored out.
// 0 if we errored, else its a file number.
void Nick_Live_t::InFlight_t::File(const unsigned long filenum)
{
    FT("Nick_Live_t::InFlight_t::File", (filenum));
    WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileinprog"));
    MCB(fileinprog);
    fileinprog = false;
    MCE(fileinprog);
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
    MCB(fileinprog);
    fileinprog = true;
    mstring *arg = NULL;

    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
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
}

// New memo, send an old one if it isnt in-progress, and
// cancel it if it was never started.
void Nick_Live_t::InFlight_t::Memo(const bool file, const mstring & mynick, const mstring & who, const mstring & message,
				   const bool silent)
{
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
	WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
	type = FileMap::MemoAttach;
	fileattach = file;
	service = mynick;
	sender = nick;
	recipiant = who;
	text = message;
    }

    {
	MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);
	timer =
	    Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh, new mstring(sender.LowerCase()),
							ACE_Time_Value(Magick::instance().memoserv.InFlight()));
    }
    DumpE();

    if (!silent)
    {
	if_RLOCK (("NickServ", "live", nick.LowerCase(), "InFlight", "fileattach"), fileattach)
	    SEND(mynick, nick, "MS_COMMAND/PENDING_FILE", (ToHumanTime(Magick::instance().memoserv.InFlight(), nick)));
	else
	    SEND(mynick, nick, "MS_COMMAND/PENDING", (ToHumanTime(Magick::instance().memoserv.InFlight(), nick)));
    }
}

// Add text to a memo, and re-start the timer.
void Nick_Live_t::InFlight_t::Continue(const mstring & message)
{
    FT("Nick_Live_t::InFlight_t::Continue", (message));
    if (!Memo())
    {
	NSEND(service, nick, "MS_STATUS/NOPENDING");
	return;
    }
    MCB(text);
    {
	WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "text"));
	text += message;
    }
    mstring *arg = NULL;

    {
	MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
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
}

// Cancel a memo or picture send.
void Nick_Live_t::InFlight_t::Cancel()
{
    NFT("Nick_Live_t::InFlight_t::Cancel");

    mstring *arg = NULL;

    {
	MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
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
}

// This is the final destination of all memos.
// It will do nothing if a file is in-progress, and
// will call Cancel() if no file was started, but requested.
// It accepts an argument of 'file number'.  Ignored if
// no file attachment was requested, but set if 
void Nick_Live_t::InFlight_t::End(const unsigned long filenum)
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

	{
	    MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
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
	RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
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

			RLOCK3(("MemoServ", "channel", recipiant.LowerCase()));
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
			     Magick::instance().filesys.GetSize(FileMap::MemoAttach,
								filenum) <= Magick::instance().memoserv.FileSize()))
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
				     Magick::instance().filesys.GetName(FileMap::MemoAttach, filenum), fmstring("%08x",
														filenum),
				     ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::MemoAttach, filenum)),
				     realrecipiant));
			    }
			    SEND(service, nick, "MS_COMMAND/SENT", (recipiant, realrecipiant));

			    RLOCK2(("MemoServ", "nick", realrecipiant.LowerCase()));
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
			 ((Magick::instance().filesys.GetPriv(FileMap::Public, filenum).empty()) ? "ALL" : Magick::instance().
			  filesys.GetPriv(FileMap::Public, filenum).c_str())));
		}
		else
		{
		    SEND(service, nick, "DCC/FAILED", ("GET"));
		}
	    }
	}
	init();
    }
}

void Nick_Live_t::InFlight_t::Picture(const mstring & mynick)
{
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
	WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
	type = FileMap::Picture;
	fileattach = true;
	sender = nick;
	service = mynick;
    }

    {
	MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);
	timer =
	    Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh, new mstring(sender.LowerCase()),
							ACE_Time_Value(Magick::instance().memoserv.InFlight()));
    }
    DumpE();

    NSEND(mynick, nick, "NS_YOU_COMMAND/PENDING");
}

void Nick_Live_t::InFlight_t::Public(const mstring & mynick, const mstring & committees)
{
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
	WLOCK(("NickServ", "live", nick.LowerCase(), "InFlight"));
	type = FileMap::Public;
	fileattach = true;
	sender = nick;
	service = mynick;
	text = committees;
    }

    {
	MLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "timer"));
	while (Magick::instance().Pause())
	    ACE_OS::sleep(1);
	timer =
	    Magick::instance().reactor().schedule_timer(&Magick::instance().nickserv.ifh, new mstring(sender.LowerCase()),
							ACE_Time_Value(Magick::instance().memoserv.InFlight()));
    }
    DumpE();

    NSEND(mynick, nick, "NS_YOU_COMMAND/PUB_PENDING");
}

mstring Nick_Live_t::InFlight_t::Text()
{
    NFT("Nick_Live_t::InFlight_t::Memo");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "text"));
    RET(text);
}

mstring Nick_Live_t::InFlight_t::Recipiant()
{
    NFT("Nick_Live_t::InFlight_t::Memo");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "recipiant"));
    RET(recipiant);
}

bool Nick_Live_t::InFlight_t::Memo() const
{
    NFT("Nick_Live_t::InFlight_t::Memo");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "recipiant"));
    RET(!recipiant.empty());
}

bool Nick_Live_t::InFlight_t::Picture() const
{
    NFT("Nick_Live_t::InFlight_t::Picture");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "type"));
    RET(type == FileMap::Picture);
}

bool Nick_Live_t::InFlight_t::Public() const
{
    NFT("Nick_Live_t::InFlight_t::Public");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "type"));
    RET(type == FileMap::Public);
}

bool Nick_Live_t::InFlight_t::Exists() const
{
    NFT("Nick_Live_t::InFlight_t::Exists");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "recipiant"));
    RLOCK2(("NickServ", "live", nick.LowerCase(), "InFlight", "type"));
    RET(!recipiant.empty() || type != FileMap::MemoAttach);
}

bool Nick_Live_t::InFlight_t::File() const
{
    NFT("Nick_Live_t::InFlight_t::File");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileattach"));
    RET(fileattach);
}

bool Nick_Live_t::InFlight_t::InProg() const
{
    NFT("Nick_Live_t::InFlight_t::InProg");
    RLOCK(("NickServ", "live", nick.LowerCase(), "InFlight", "fileinprog"));
    RET(fileinprog);
}

size_t Nick_Live_t::InFlight_t::Usage() const
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

void Nick_Live_t::InFlight_t::DumpB() const
{
    // 8 Elements
    MB(0, (nick, type, timer, fileattach, fileinprog, sender, recipiant, text));
}

void Nick_Live_t::InFlight_t::DumpE() const
{
    // 8 Elements
    ME(0, (nick, type, timer, fileattach, fileinprog, sender, recipiant, text));
}

Nick_Live_t::Nick_Live_t() : last_msg_entries(0), flood_triggered_times(0), failed_passwds(0), identified(false),
services(true)
{
    NFT("Nick_Live_t::Nick_Live_t");
    // Dont call anything that locks, no names!
    ref_class::lockData(mVarArray("NickServ", "live", i_Name.LowerCase()));
}

Nick_Live_t::Nick_Live_t(const mstring & name, const mDateTime & signon, const mstring & server, const mstring & username,
			 const mstring & hostname, const mstring & realname) : i_Name(name), i_Signon_Time(signon),
i_My_Signon_Time(mDateTime::CurrentDateTime()), i_Last_Action(mDateTime::CurrentDateTime()), i_realname(realname),
i_user(username), i_host(hostname), i_alt_host(hostname), i_server(server.LowerCase()), last_msg_entries(0),
flood_triggered_times(0), failed_passwds(0), identified(false), services(false), InFlight(name)
{
    FT("Nick_Live_t::Nick_Live_t", (name, signon, server, username, hostname, realname));

    ref_class::lockData(mVarArray("NickServ", "live", i_Name.LowerCase()));
    InFlight.init();

    // User is on AKILL, add the mask, and No server will kill
    {
	MLOCK(("OperServ", "Akill"));
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
}

Nick_Live_t::Nick_Live_t(const mstring & name, const mstring & username, const mstring & hostname,
			 const mstring & realname) : i_Name(name), i_Signon_Time(mDateTime::CurrentDateTime()),
i_My_Signon_Time(mDateTime::CurrentDateTime()), i_Last_Action(mDateTime::CurrentDateTime()), i_realname(realname),
i_user(username), i_host(hostname), i_alt_host(hostname), last_msg_entries(0), flood_triggered_times(0), failed_passwds(0),
identified(true), services(true), InFlight(name)
{
    FT("Nick_Live_t::Nick_Live_t", (name, username, hostname, realname));
    ref_class::lockData(mVarArray("NickServ", "live", i_Name.LowerCase()));
    InFlight.init();
    DumpE();
}

Nick_Live_t &Nick_Live_t::operator=(const Nick_Live_t & in)
{
    NFT("Nick_Live_t::operator=");
    i_Name = in.i_Name;
    ref_class::lockData(mVarArray("NickServ", "live", i_Name.LowerCase()));
    i_Signon_Time = in.i_Signon_Time;
    i_My_Signon_Time = in.i_My_Signon_Time;
    i_Last_Action = in.i_Last_Action;
    i_realname = in.i_realname;
    i_user = in.i_user;
    i_host = in.i_host;
    i_alt_host = in.i_alt_host;
    i_server = in.i_server;
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
}

void Nick_Live_t::Join(const mstring & chan)
{
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
	    WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
	    MCB(joined_channels.size());
	    joined_channels.insert(chan.LowerCase());
	    MCE(joined_channels.size());
	}
	if (Magick::instance().chanserv.IsStored(chan))
	    Magick::instance().chanserv.GetStored(chan)->Join(i_Name);
    }
    mMessage::CheckDependancies(mMessage::ChanExists, chan);
    mMessage::CheckDependancies(mMessage::UserInChan, chan, i_Name);
}

void Nick_Live_t::Part(const mstring & chan)
{
    FT("Nick_Live_t::Part", (chan));

    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
	MCB(joined_channels.size());
	joined_channels.erase(chan.LowerCase());
	MCE(joined_channels.size());
    }

    unsigned int res = 0;

    if (Magick::instance().chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
	res = Magick::instance().chanserv.GetLive(chan)->Part(i_Name);
	if (!res)
	    Magick::instance().chanserv.RemLive(chan);
    }
    else
    {
	LOG(LM_TRACE, "ERROR/REC_FORNONCHAN", ("PART", i_Name, chan));
    }

    mMessage::CheckDependancies(mMessage::UserNoInChan, chan, i_Name);
    if (!res)
	mMessage::CheckDependancies(mMessage::ChanNoExists, chan);
}

void Nick_Live_t::Kick(const mstring & kicker, const mstring & chan)
{
    FT("Nick_Live_t::Kick", (kicker, chan));

    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
	MCB(joined_channels.size());
	joined_channels.erase(chan.LowerCase());
	MCE(joined_channels.size());
    }

    unsigned int res = 0;

    if (Magick::instance().chanserv.IsLive(chan))
    {
	// If this returns 0, then the channel is empty.
	res = Magick::instance().chanserv.GetLive(chan)->Kick(i_Name, kicker);
	if (!res)
	    Magick::instance().chanserv.RemLive(chan);

    }
    else
    {
	LOG(LM_ERROR, "ERROR/REC_FORNONCHAN", ("KICK", kicker, chan));
    }

    mMessage::CheckDependancies(mMessage::UserNoInChan, chan, i_Name);
    if (!res)
	mMessage::CheckDependancies(mMessage::ChanNoExists, chan);
}

void Nick_Live_t::Quit(const mstring & reason)
{
    FT("Nick_Live_t::Quit", (reason));

    {
	RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
	if (!(IsServices() || HasMode("o")))
	    Magick::instance().operserv.RemHost(i_host);
    }

    // Check if we're currently being TEMP ignored ...
    {
	MLOCK(("OperServ", "Ignore"));
	if (Magick::instance().operserv.Ignore_find(Mask(N_U_P_H)))
	{
	    if (Magick::instance().operserv.Ignore->Value() != true)
	    {
		Magick::instance().operserv.Ignore_erase();
	    }
	}
    }

    set < mstring > jc;
    set < mstring >::iterator c;
    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
	jc = joined_channels;
	joined_channels.clear();
    }
    for (c = jc.begin(); c != jc.end(); c++)
	Part(*c);

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
	RLOCK(("NickServ", "live", i_Name.LowerCase(), "try_chan_ident"));
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
}

bool Nick_Live_t::IsInChan(const mstring & chan)
{
    FT("Nick_Live_t::IsInChan", (chan));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    bool retval = (joined_channels.find(chan.LowerCase()) != joined_channels.end());

    RET(retval);
}

set < mstring > Nick_Live_t::Channels() const
{
    NFT("Nick_Live_t::Channels");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
    NRET(set < mstring >, joined_channels);
}

bool Nick_Live_t::FloodTrigger()
{
    NFT("Nick_Live_t::FloodTrigger");
    bool retval = false;

    // We DONT ignore OPER's
    if (HasMode("o") || IsServices())
    {
	RET(false);
    }

    // Check if we're currently being ignored ...
    {
	MLOCK(("OperServ", "Ignore"));
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
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "last_msg_times"));
	MCB(last_msg_times.size());
	while (last_msg_times.size() && last_msg_times[0u].SecondsSince() > Magick::instance().operserv.Flood_Time())
	    last_msg_times.erase(last_msg_times.begin());
	last_msg_times.push_back(mDateTime::CurrentDateTime());
	MCE(last_msg_times.size());
    }

    // Check if we just triggered ignore.
    if_RLOCK (("NickServ", "live", i_Name.LowerCase(), "last_msg_times"),
	      last_msg_times.size() > Magick::instance().operserv.Flood_Msgs())
    {
	RLOCK(("NickServ", "live", i_Name.LowerCase(), "flood_triggered_times"));
	CB(1, flood_triggered_times);
	{
	    WLOCK(("NickServ", "live", i_Name.LowerCase(), "flood_triggered_times"));
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
}

set < mstring > Nick_Live_t::Name(const mstring & in)
{
    FT("Nick_Live_t::Name", (in));

    InFlight.ChgNick(in);
    set < mstring > wason;

    {
	RLOCK(("NickServ", "live", in.LowerCase()));
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
	RLOCK2(("CommServ", "list"));
	for (iter2 = Magick::instance().commserv.ListBegin(); iter2 != Magick::instance().commserv.ListEnd(); iter2++)
	{
	    map_entry < Committee_t > comm(iter2->second);
	    if (comm->IsOn(i_Name))
		wason.insert(iter2->first);
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
	RLOCK(("NickServ", "live", i_Name.LowerCase(), "try_chan_ident"));
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
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
	WLOCK2(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
	WLOCK3(("NickServ", "live", i_Name.LowerCase(), "failed_paswds"));
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
    ref_class::lockData(mVarArray("NickServ", "stored", i_Name));
    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_My_Signon_Time"));
	CB(1, i_My_Signon_Time);
	i_My_Signon_Time = mDateTime::CurrentDateTime();
	CE(1, i_My_Signon_Time);
    }

    set < mstring > jc;
    set < mstring >::iterator iter;
    vector < mstring > chunked;
    // Rename ourselves in all channels ...
    {
	RLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
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
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
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
}

void Nick_Live_t::SendMode(const mstring & in)
{
    FT("Nick_Live_t::SendMode", (in));

    if (IsServices())
    {
	Magick::instance().server.MODE(i_Name, in);
    }
    else if (!Magick::instance().server.proto.SVSMODE().empty())
    {
	Magick::instance().server.SVSMODE(Magick::instance().nickserv.FirstName(), i_Name, in);
    }
}

void Nick_Live_t::Mode(const mstring & in)
{
    FT("Nick_Live_t::Mode", (in));

    bool add = true;

    WLOCK(("NickServ", "live", i_Name.LowerCase(), "modes"));
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
			RLOCK(("CommServ", "list"));
			for (iter2 = Magick::instance().commserv.ListBegin(); iter2 != Magick::instance().commserv.ListEnd();
			     iter2++)
			{
			    map_entry < Committee_t > comm(iter2->second);
			    if (comm->IsOn(i_Name))
				wason.insert(iter2->first);
			}
		    }

		    modes += in[i];
		    {
			RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
			Magick::instance().operserv.RemHost(i_host);
		    }
		    // IF we are SecureOper and NOT (on oper list && recoznized)
		    // OR user is on OperDeny and NOT (on sadmin list && recognized)
		    // Yeah, one UUUUUUGLY if.
		    map_entry < Nick_Stored_t > nstored;
		    if (Magick::instance().nickserv.IsStored(i_Name))
			nstored = Magick::instance().nickserv.GetStored(i_Name);
		    if_MLOCK (("OperServ", "OperDeny"),
			      (Magick::instance().operserv.SecureOper() &&
			       !(nstored.entry() != NULL && nstored->IsOnline() &&
				 Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
				 Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(i_Name)))
			      || (Magick::instance().operserv.OperDeny_find(Mask(N_U_P_H)) &&
				  !(nstored.entry() != NULL && nstored->IsOnline() &&
				    Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
				    Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->
				    IsIn(i_Name))))
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
			    RLOCK(("CommServ", "list"));
			    for (iter2 = Magick::instance().commserv.ListBegin();
				 iter2 != Magick::instance().commserv.ListEnd(); iter2++)
			    {
				map_entry < Committee_t > comm(iter2->second);
				if (comm->IsOn(i_Name) && wason.find(iter2->first) == wason.end())
				{
				    if (!Magick::instance().server.proto.SVSMODE().empty())
				    {
					if (iter2->first == Magick::instance().commserv.ALL_Name())
					    setmode += Magick::instance().commserv.ALL_SetMode();
					else if (iter2->first == Magick::instance().commserv.REGD_Name())
					    setmode += Magick::instance().commserv.REGD_SetMode();
					else if (iter2->first == Magick::instance().commserv.OPER_Name())
					    setmode += Magick::instance().commserv.OPER_SetMode();
					else if (iter2->first == Magick::instance().commserv.ADMIN_Name())
					    setmode += Magick::instance().commserv.ADMIN_SetMode();
					else if (iter2->first == Magick::instance().commserv.SOP_Name())
					    setmode += Magick::instance().commserv.SOP_SetMode();
					else if (iter2->first == Magick::instance().commserv.SADMIN_Name())
					    setmode += Magick::instance().commserv.SADMIN_SetMode();
				    }

				    MLOCK(("CommServ", "list", iter2->first, "message"));
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
			    SendMode("+" + setmode2);
			}
		    }
		}
		else if (!add && modes.Contains(in[i]))
		{
		    {
			RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
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
}

mstring Nick_Live_t::Mode() const
{
    NFT("Nick_Live_t::Mode");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "modes"));
    RET(modes);
}

bool Nick_Live_t::HasMode(const mstring & in) const
{
    FT("Nick_Live_t::HasMode", (in));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "modes"));
    RET(modes.Contains(in));
}

void Nick_Live_t::Away(const mstring & in)
{
    FT("Nick_Live_t::Away", (in));
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_away"));
    MCB(i_away);
    i_away = in;
    MCE(i_away);
}

mstring Nick_Live_t::Away() const
{
    NFT("Nick_Live_t::Away");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_away"));
    RET(i_away);
}

mDateTime Nick_Live_t::LastAction() const
{
    NFT("Nick_Live_t::LastAction");
    if (IsServices())
    {
	RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_Last_Action"));
	RET(i_Last_Action);
    }
    RET(mDateTime::CurrentDateTime());
}

void Nick_Live_t::Action()
{
    NFT("Nick_Live_t::Action");
    if (IsServices())
    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_Last_Action"));
	MCB(i_Last_Action);
	i_Last_Action = mDateTime::CurrentDateTime();
	MCE(i_Last_Action);
    }
}

mDateTime Nick_Live_t::SignonTime() const
{
    NFT("Nick_Live_t::SignonTime");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_Signon_Time"));
    RET(i_Signon_Time);
}

mDateTime Nick_Live_t::MySignonTime() const
{
    NFT("Nick_Live_t::MySignonTime");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_My_Signon_Time"));
    RET(i_My_Signon_Time);
}

mstring Nick_Live_t::RealName() const
{
    NFT("Nick_Live_t::RealName");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_realname"));
    RET(i_realname);
}

mstring Nick_Live_t::User() const
{
    NFT("Nick_Live_t::User");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_user"));
    RET(i_user);
}

mstring Nick_Live_t::Host() const
{
    NFT("Nick_Live_t::Host");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_host"));
    RET(i_host);
}

mstring Nick_Live_t::AltHost() const
{
    NFT("Nick_Live_t::AltHost");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_alt_host"));
    RET(i_alt_host);
}

void Nick_Live_t::AltHost(const mstring & in)
{
    FT("Nick_Live_t::AltHost", (in));
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_alt_host"));
    MCB(i_alt_host);
    i_alt_host = in;
    MCE(i_alt_host);
}

mstring Nick_Live_t::Server() const
{
    NFT("Nick_Live_t::Server");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_server"));
    RET(i_server);
}

mstring Nick_Live_t::Squit() const
{
    NFT("Nick_Live_t::Squit");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_squit"));
    RET(i_squit);
}

void Nick_Live_t::SetSquit()
{
    NFT("Nick_Live_t::SetSquit");
    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_squit"));
	RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_server"));
	MCB(i_squit);
	i_squit = i_server;
	MCE(i_squit);
    }

    {
	RLOCK2(("NickServ", "live", i_Name.LowerCase(), "i_host"));
	if (!IsServices())
	    Magick::instance().operserv.RemHost(i_host);
    }

    set < mstring > jc;
    set < mstring >::iterator c;
    vector < mstring > chunked;
    {
	RLOCK3(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
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
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
	MCB(joined_channels.size());
	for (unsigned int i = 0; i < chunked.size(); i++)
	    joined_channels.erase(chunked[i]);
	MCE(joined_channels.size());
    }
}

void Nick_Live_t::ClearSquit(const mstring & inmodes)
{
    NFT("Nick_Live_t::ClearSquit");

    MCB(i_squit);

    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "i_squit"));
	i_squit.erase();
    }

    // These will all be set again
    {
	WLOCK2(("NickServ", "live", i_Name.LowerCase(), "modes"));
	CB(1, modes);
	modes = inmodes;
	CE(1, modes);
    }

    set < mstring > jc;
    set < mstring >::iterator i;
    {
	WLOCK3(("NickServ", "live", i_Name.LowerCase(), "joined_channels"));
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

    if_RLOCK (("NickServ", "live", i_Name.LowerCase(), "i_host"), !IsServices() && Magick::instance().operserv.AddHost(i_host))
    {
	LOG(LM_INFO, "OTHER/KILL_CLONE", (Mask(N_U_P_H)));
	Magick::instance().server.KILL(Magick::instance().nickserv.FirstName(), i_Name,
				       Magick::instance().operserv.Def_Clone());
	return;
    }
}

mstring Nick_Live_t::Mask(const Nick_Live_t::styles type) const
{
    FT("Nick_Live_t::Mask", (static_cast < int > (type)));

    RLOCK(("NickServ", "live", i_Name.LowerCase(), "i_user"));
    RLOCK2(("NickServ", "live", i_Name.LowerCase(), "i_host"));
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

    case N_H:			// nick!*@*.host
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

    case U_H:			// *!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_host.IsNumber())
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

mstring Nick_Live_t::AltMask(const Nick_Live_t::styles type) const
{
    FT("Nick_Live_t::AltMask", (static_cast < int > (type)));

    RLOCK2(("NickServ", "live", i_Name.LowerCase(), "i_user"));
    RLOCK3(("NickServ", "live", i_Name.LowerCase(), "i_alt_host"));
    mstring retval;
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

    case N_H:			// nick!*@*.host
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

    case U_H:			// *!*user@*.host
	if (user[0u] == '~')
	    user = user.After("~");
	if (i_alt_host.IsNumber())
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

mstring Nick_Live_t::ChanIdentify(const mstring & channel, const mstring & password)
{
    FT("Nick_Live_t::ChanIdentify", (channel, password));
    mstring retval;

    if (Magick::instance().chanserv.IsStored(channel))
    {
	unsigned int failtimes = Magick::instance().chanserv.GetStored(channel)->CheckPass(i_Name, password);

	if (!failtimes)
	{
	    {
		WLOCK(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
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
		WLOCK(("NickServ", "live", i_Name.LowerCase(), "try_chan_ident"));
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
}

void Nick_Live_t::UnChanIdentify(const mstring & channel)
{
    FT("Nick_Live_t::UnChanIdentify", (channel));

    if (IsChanIdentified(channel))
    {
	WLOCK(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
	MCB(chans_founder_identd.size());
	chans_founder_identd.erase(channel.LowerCase());
	MCE(chans_founder_identd.size());
    }
}

bool Nick_Live_t::IsChanIdentified(const mstring & channel)
{
    FT("Nick_Live_t::IsChanIdentified", (channel));
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "chans_founder_identd"));
    bool retval = (chans_founder_identd.find(channel.LowerCase()) != chans_founder_identd.end());

    RET(retval);
}

mstring Nick_Live_t::Identify(const mstring & password)
{
    FT("Nick_Live_t::Identify", (password));
    mstring retval;

    if_RLOCK (("NickServ", "live", i_Name.LowerCase(), "identified"), identified)
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
		RLOCK(("CommServ", "list"));
		for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		{
		    map_entry < Committee_t > comm(iter->second);
		    if (comm->IsOn(i_Name))
			wason.insert(iter->first);

		}
	    }
	    {
		WLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
		WLOCK2(("NickServ", "live", i_Name.LowerCase(), "failed_passwds"));
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
		RLOCK(("CommServ", "list"));
		for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		{
		    map_entry < Committee_t > comm(iter->second);
		    if (comm->IsOn(i_Name) && wason.find(iter->first) == wason.end())
		    {
			if (!Magick::instance().server.proto.SVSMODE().empty())
			{
			    if (iter->first == Magick::instance().commserv.ALL_Name())
				setmode += Magick::instance().commserv.ALL_SetMode();
			    else if (iter->first == Magick::instance().commserv.REGD_Name())
				setmode += Magick::instance().commserv.REGD_SetMode();
			    else if (iter->first == Magick::instance().commserv.OPER_Name())
				setmode += Magick::instance().commserv.OPER_SetMode();
			    else if (iter->first == Magick::instance().commserv.ADMIN_Name())
				setmode += Magick::instance().commserv.ADMIN_SetMode();
			    else if (iter->first == Magick::instance().commserv.SOP_Name())
				setmode += Magick::instance().commserv.SOP_SetMode();
			    else if (iter->first == Magick::instance().commserv.SADMIN_Name())
				setmode += Magick::instance().commserv.SADMIN_SetMode();
			}

			MLOCK(("CommServ", "list", iter->first, "message"));
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
		SendMode("+" + setmode2);
	    }
	    retval = Magick::instance().getMessage(i_Name, "NS_YOU_COMMAND/IDENTIFIED");
	}
	else
	{
	    {
		WLOCK2(("NickServ", "live", i_Name.LowerCase(), "failed_passwds"));
		MCB(failed_passwds);
		failed_passwds++;
		MCE(failed_passwds);
	    }
	    if_RLOCK2 (("NickServ", "live", i_Name.LowerCase(), "failed_passwds"),
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
}

void Nick_Live_t::UnIdentify()
{
    NFT("Nick_Live_t::UnIdentify");
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
    MCB(identified);
    identified = false;
    MCE(identified);
}

bool Nick_Live_t::IsIdentified() const
{
    NFT("Nick_Live_t::IsIdentified");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "identified"));
    RET(identified);
}

bool Nick_Live_t::IsRecognized() const
{
    NFT("Nick_Live_t::IsRecognised");
    bool retval = false;

    if (Magick::instance().nickserv.IsStored(i_Name))
    {
	retval = Magick::instance().nickserv.GetStored(i_Name)->IsAccess(Mask(U_P_H).After("!"));
    }
    RET(retval);
}

bool Nick_Live_t::IsServices() const
{
    NFT("Nick_Live_t::IsServices");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "services"));
    RET(services);
}

void Nick_Live_t::SetLastNickReg()
{
    NFT("Nick_Live_t::SetLastNickReg");
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "last_nick_reg"));
    MCB(last_nick_reg);
    last_nick_reg = mDateTime::CurrentDateTime();
    MCE(last_nick_reg);
}

mDateTime Nick_Live_t::LastNickReg() const
{
    NFT("Nick_Live_t::LastNickReg");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "last_nick_reg"));
    RET(last_nick_reg);
}

void Nick_Live_t::SetLastChanReg()
{
    NFT("Nick_Live_t::SetLastChanReg");
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "last_chan_reg"));
    MCB(last_chan_reg);
    last_chan_reg = mDateTime::CurrentDateTime();
    MCE(last_chan_reg);
}

mDateTime Nick_Live_t::LastChanReg() const
{
    NFT("Nick_Live_t::LastChanReg");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "last_chan_reg"));
    RET(last_chan_reg);
}

void Nick_Live_t::SetLastMemo()
{
    NFT("Nick_Live_t::SetLastMemo");
    WLOCK(("NickServ", "live", i_Name.LowerCase(), "last_memo"));
    MCB(last_memo);
    last_memo = mDateTime::CurrentDateTime();
    MCE(last_memo);
}

mDateTime Nick_Live_t::LastMemo() const
{
    NFT("Nick_Live_t::LastMemo");
    RLOCK(("NickServ", "live", i_Name.LowerCase(), "last_memo"));
    RET(last_memo);
}

size_t Nick_Live_t::Usage() const
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
}

void Nick_Live_t::DumpB() const
{
    // 16 Elements
    MB(0,
       (i_Name, i_Signon_Time, i_My_Signon_Time, i_Last_Action, i_realname, i_user, i_host, i_alt_host, i_server, i_squit,
	i_away, modes, joined_channels.size(), last_msg_times.size(), last_msg_entries, flood_triggered_times));
    // 8 Elements
    MB(16,
       (failed_passwds, chans_founder_identd.size(), try_chan_ident.size(), identified, services, last_nick_reg, last_chan_reg,
	last_memo));
}

void Nick_Live_t::DumpE() const
{
    // 16 Elements
    ME(0,
       (i_Name, i_Signon_Time, i_My_Signon_Time, i_Last_Action, i_realname, i_user, i_host, i_alt_host, i_server, i_squit,
	i_away, modes, joined_channels.size(), last_msg_times.size(), last_msg_entries, flood_triggered_times));
    // 8 Elements
    ME(16,
       (failed_passwds, chans_founder_identd.size(), try_chan_ident.size(), identified, services, last_nick_reg, last_chan_reg,
	last_memo));
}

// =======================================================================

void Nick_Stored_t::Signon(const mstring & realname, const mstring & mask)
{
    FT("Nick_Stored_t::Signon", (realname, mask));
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastRealName"));
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_LastMask"));
	WLOCK3(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
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
}

void Nick_Stored_t::ChgNick(const mstring & nick)
{
    FT("Nick_Stored_t::ChgNick", (nick));
    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastQuit"));
    WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
    MCB(i_LastQuit);
    CB(1, i_LastSeenTime);
    i_LastQuit = "NICK CHANGE -> " + nick;
    i_LastSeenTime = mDateTime::CurrentDateTime();
    CE(1, i_LastSeenTime);
    MCE(i_LastQuit);
}

void Nick_Stored_t::defaults()
{
    NFT("Nick_Stored_t::defaults");

    ref_class::lockData(mVarArray("NickServ", "stored", i_Name));
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

}

Nick_Stored_t::Nick_Stored_t()
{
    NFT("Nick_Stored_t::Nick_Stored_t");
    defaults();
}

Nick_Stored_t::Nick_Stored_t(const mstring & nick, const mstring & password) : i_Name(nick),
i_RegTime(mDateTime::CurrentDateTime())
{
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
}

Nick_Stored_t::Nick_Stored_t(const mstring & nick) : i_Name(nick), i_RegTime(mDateTime::CurrentDateTime())
{
    FT("Nick_Stored_t::Nick_Stored_t", (nick.LowerCase()));
    defaults();
    setting.Forbidden = true;
    DumpE();
}

Nick_Stored_t::Nick_Stored_t(const mstring & nick, const mDateTime & regtime, const Nick_Stored_t & in) : i_Name(nick),
i_RegTime(regtime), i_Host(in.i_Name.LowerCase())
{
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
}

mDateTime Nick_Stored_t::RegTime() const
{
    NFT("Nick_Stored_t::RegTime");
    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_RegTime"));
    RET(i_RegTime);
}

unsigned long Nick_Stored_t::Drop()
{
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
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	Magick::instance().nickserv.GetStored(i_Host)->i_slaves.erase(i_Name.LowerCase());
    }

    if (Magick::instance().memoserv.IsNick(i_Name))
	Magick::instance().memoserv.RemNick(i_Name);

    // Now we go for our channels ...
    vector < mstring > killchans;
    ChanServ::stored_t::iterator iter;
    {
	RLOCK(("ChanServ", "stored"));
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
		    killchans.push_back(iter->first);
	    }
	    else if (cstored->CoFounder().IsSameAs(i_Name, true))
	    {
		if (!host)
		    cstored->CoFounder(i_Host);
		else
		    cstored->CoFounder("");
	    }
	    MLOCK(("ChanServ", "stored", iter->first, "Access"));
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
}

Nick_Stored_t &Nick_Stored_t::operator=(const Nick_Stored_t & in)
{
    NFT("Nick_Stored_t::operator=");
    i_Name = in.i_Name;
    ref_class::lockData(mVarArray("NickServ", "stored", i_Name.LowerCase()));
    i_RegTime = in.i_RegTime;
    i_Password = in.i_Password;
    i_Email = in.i_Email;
    i_URL = in.i_URL;
    i_ICQ = in.i_ICQ;
    i_AIM = in.i_AIM;
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
}

mstring Nick_Stored_t::Email()
{
    NFT("Nick_Stored_t::Email");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Email"));
	RET(i_Email);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Email();

	RET(retval);
    }
}

void Nick_Stored_t::Email(const mstring & in)
{
    FT("Nick_Stored_t::Email", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Email"));
	MCB(i_Email);
	i_Email = in;
	MCE(i_Email);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Email(in);
    }
}

mstring Nick_Stored_t::URL()
{
    NFT("Nick_Stored_t::URL");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_URL"));
	RET(i_URL);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->URL();

	RET(retval);
    }
}

void Nick_Stored_t::URL(const mstring & in)
{
    FT("Nick_Stored_t::URL", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_URL"));
	MCB(i_URL);
	i_URL = in;
	MCE(i_URL);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->URL(in);
    }
}

mstring Nick_Stored_t::ICQ()
{
    NFT("Nick_Stored_t::ICQ");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ICQ"));
	RET(i_ICQ);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->ICQ();

	RET(retval);
    }
}

void Nick_Stored_t::ICQ(const mstring & in)
{
    FT("Nick_Stored_t::ICQ", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ICQ"));
	MCB(i_ICQ);
	i_ICQ = in;
	MCE(i_ICQ);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->ICQ(in);
    }
}

mstring Nick_Stored_t::AIM()
{
    NFT("Nick_Stored_t::AIM");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_AIM"));
	RET(i_AIM);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->AIM();

	RET(retval);
    }
}

void Nick_Stored_t::AIM(const mstring & in)
{
    FT("Nick_Stored_t::AIM", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_AIM"));
	MCB(i_AIM);
	i_AIM = in;
	MCE(i_AIM);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->AIM(in);
    }
}

mstring Nick_Stored_t::Description()
{
    NFT("Nick_Stored_t::Description");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Description"));
	RET(i_Description);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Description();

	RET(retval);
    }
}

void Nick_Stored_t::Description(const mstring & in)
{
    FT("Nick_Stored_t::Description", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Description"));
	MCB(i_Description);
	i_Description = in;
	MCE(i_Description);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Description();
    }
}

mstring Nick_Stored_t::Comment()
{
    NFT("Nick_Stored_t::Comment");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Comment"));
	RET(i_Comment);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Comment();

	RET(retval);
    }
}

void Nick_Stored_t::Comment(const mstring & in)
{
    FT("Nick_Stored_t::Comment", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Comment"));
	MCB(i_Comment);
	i_Comment = in;
	MCE(i_Comment);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Comment(in);
    }
}

void Nick_Stored_t::Suspend(const mstring & name)
{
    FT("Nick_Stored_t::Suspend", (name));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_Time"));
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
}

void Nick_Stored_t::UnSuspend()
{
    NFT("Nick_Stored_t::UnSuspend");
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
	MCB(i_Suspend_By);
	i_Suspend_By.erase();
	MCE(i_Suspend_By);
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->UnSuspend();
    }
}

mstring Nick_Stored_t::Host()
{
    NFT("Nick_Stored_t::Host");
    mstring retval;

    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Host"));
    if (!i_Host.empty())
	if (!Magick::instance().nickserv.IsStored(i_Host))
	{
	    LOG(LM_ERROR, "ERROR/HOST_NOTREGD", (i_Host, i_Name));
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Host"));
	    MCB(i_Host);
	    i_Host.erase();
	    MCE(i_Host);
	}
	else
	    retval = Magick::instance().nickserv.GetStored(i_Host)->Name();
    RET(retval);
}

mstring Nick_Stored_t::Password()
{
    NFT("Nick_Stored_t::Password");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Password"));
	RET(i_Password);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Password();

	RET(retval);
    }
}

void Nick_Stored_t::Password(const mstring & in)
{
    FT("Nick_Stored_t::Password", (in));
    if (Host().empty())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Password"));
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
}

bool Nick_Stored_t::CheckPass(const mstring & password)
{
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
	if_RLOCK (("NickServ", "stored", i_Name.LowerCase(), "i_Password"), i_Password == check)
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
}

bool Nick_Stored_t::Slave(const mstring & nick, const mstring & password, const mDateTime & regtime)
{
    FT("Nick_Stored_t::Slave", (nick, password, regtime));

    if (Host().empty())
    {
	{
	    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Forbidden"));
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
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
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
}

unsigned int Nick_Stored_t::Siblings()
{
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
}

mstring Nick_Stored_t::Sibling(const unsigned int count)
{
    FT("Nick_Stored_t::Siblings", (count));
    mstring retval;

    if (Host().empty())
    {
	vector < mstring > chunked;
	set < mstring >::iterator iter;
	unsigned int i;

	{
	    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
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
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
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
}

bool Nick_Stored_t::IsSibling(const mstring & nick)
{
    FT("Nick_Stored_t::IsSibling", (nick));
    if (i_Name.IsSameAs(nick, true))
    {
	RET(true);
    }
    else if (Host().empty())
    {
	bool retval;

	{
	    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
	    retval = (i_slaves.find(nick.LowerCase()) != i_slaves.end());
	}
	if (retval && !Magick::instance().nickserv.IsStored(nick))
	{
	    LOG(LM_ERROR, "ERROR/SLAVE_NOTREGD", (nick, i_Name));
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_slaves"));
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
}

void Nick_Stored_t::ChangeOver(const mstring & oldnick)
{
    FT("Nick_Stored_t::ChangeOver", (oldnick));

    bool found;
    mstring modifier, valueS;
    long valueL;
    mDateTime modtime;

    CommServ::list_t::iterator citer;
    {
	RLOCK(("CommServ", "list"));
	for (citer = Magick::instance().commserv.ListBegin(); citer != Magick::instance().commserv.ListEnd(); citer++)
	{
	    map_entry < Committee_t > comm(citer->second);
	    found = false;
	    if (citer->first != Magick::instance().commserv.ALL_Name() &&
		citer->first != Magick::instance().commserv.REGD_Name() &&
		citer->first != Magick::instance().commserv.SADMIN_Name())
	    {
		MLOCK(("CommServ", "list", citer->first, "member"));
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
	RLOCK(("ChanServ", "stored"));
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
		MLOCK(("ChanServ", "stored", csiter->first, "Access"));
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
		MLOCK(("ChanServ", "stored", csiter->first, "Akick"));
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
		MLOCK(("ChanServ", "stored", csiter->first, "Greet"));
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
	RLOCK(("NickServ", "stored"));
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
	RLOCK(("MemoServ", "channel"));
	for (cniter = Magick::instance().memoserv.ChannelBegin(); cniter != Magick::instance().memoserv.ChannelEnd(); cniter++)
	{
	    RLOCK2(("MemoServ", "channel", cniter->first));
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
}

bool Nick_Stored_t::MakeHost()
{
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
	WLOCK(("NickServ", "stored", i_Name.LowerCase()));
	DumpB();
	map_entry < Nick_Stored_t > host = Magick::instance().nickserv.GetStored(i_Host);
	for (unsigned int i = 0; i < host->Siblings(); i++)
	{
	    if (!host->Sibling(i).IsSameAs(i_Name, true))
	    {
		i_slaves.insert(host->Sibling(i));
		WLOCK2(("NickServ", "stored", host->Sibling(i).LowerCase(), "i_Host"));
		Magick::instance().nickserv.GetStored(host->Sibling(i))->i_Host = i_Name.LowerCase();
	    }
	}
	i_slaves.insert(i_Host.LowerCase());
	i_Password = host->i_Password;
	i_Email = host->i_Email;
	i_URL = host->i_URL;
	i_ICQ = host->i_ICQ;
	i_AIM = host->i_AIM;
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
	    WLOCK2(("NickServ", "stored", i_Host.LowerCase(), "i_slaves"));
	    host->i_slaves.clear();
	    host->i_Host = i_Name.LowerCase();
	}
	mstring tmp = i_Host;

	i_Host.erase();
	DumpE();
	ChangeOver(tmp);
	RET(true);
    }
}

bool Nick_Stored_t::Unlink()
{
    NFT("Nick_Stored_t::Unlink");
    if (Host().empty())
    {
	RET(false);
    }
    else
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase()));
	DumpB();
	map_entry < Nick_Stored_t > host = Magick::instance().nickserv.GetStored(i_Host);
	i_slaves.clear();
	i_Password = host->i_Password;
	i_Email = host->i_Email;
	i_URL = host->i_URL;
	i_ICQ = host->i_ICQ;
	i_AIM = host->i_AIM;
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
	    WLOCK2(("NickServ", "stored", i_Host.LowerCase(), "i_slaves"));
	    host->i_slaves.erase(i_Name.LowerCase());
	}
	i_Host.erase();
	DumpE();
	RET(true);
    }
}

unsigned int Nick_Stored_t::Access()
{
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
}

mstring Nick_Stored_t::Access(const unsigned int count)
{
    FT("Nick_Stored_t::Access", (count));
    if (Host().empty())
    {
	set < mstring >::iterator iter;
	unsigned int i;

	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
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
}

bool Nick_Stored_t::AccessAdd(const mstring & in)
{
    FT("Nick_Stored_t::AccessAdd", (in));
    if (Host().empty())
    {
	// Incorrect syntax
	if (!(in.Contains("@") && in.WordCount("@") == 2))
	{
	    RET(false);
	}

	// Already exists (or inclusive)
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
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
}

unsigned int Nick_Stored_t::AccessDel(const mstring & in)
{
    FT("Nick_Stored_t::AccessDel", (in));
    unsigned int retval = 0;

    if (Host().empty())
    {
	vector < mstring > chunked;
	set < mstring >::iterator iter;
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
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
}

bool Nick_Stored_t::IsAccess(const mstring & in)
{
    FT("Nick_Stored_t::IsAccess", (in));
    if (Host().empty())
    {
	set < mstring >::iterator iter;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_access"));
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
}

unsigned int Nick_Stored_t::Ignore()
{
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
}

mstring Nick_Stored_t::Ignore(const unsigned int count)
{
    FT("Nick_Stored_t::Ignore", (count));
    if (Host().empty())
    {
	set < mstring >::iterator iter;
	unsigned int i;

	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
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
}

bool Nick_Stored_t::IgnoreAdd(const mstring & in)
{
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
}

unsigned int Nick_Stored_t::IgnoreDel(const mstring & in)
{
    FT("Nick_Stored_t::IgnoreDel", (in));
    unsigned int retval = 0;

    if (Host().empty())
    {
	mstring target(in.LowerCase());

	vector < mstring > chunked;
	set < mstring >::iterator iter;
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
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
}

bool Nick_Stored_t::IsIgnore(const mstring & in)
{
    FT("Nick_Stored_t::IsIgnore", (in));
    if (Host().empty())
    {
	mstring target(in.LowerCase());

	set < mstring >::iterator iter;
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_ignore"));
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
}

bool Nick_Stored_t::Protect()
{
    NFT("Nick_Stored_t::Protect");
    if (Magick::instance().nickserv.LCK_Protect())
    {
	RET(Magick::instance().nickserv.DEF_Protect());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Protect"));
	RET(setting.Protect);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->Protect();

	RET(retval);
    }
}

void Nick_Stored_t::Protect(const bool in)
{
    FT("Nick_Stored_t::Protect", (in));
    if (Host().empty())
    {
	if (!L_Protect())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Protect"));
	    MCB(setting.Protect);
	    setting.Protect = in;
	    MCE(setting.Protect);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Protect(in);
    }
}

bool Nick_Stored_t::L_Protect()
{
    NFT("Nick_Stored_t::L_Protect");
    if (Magick::instance().nickserv.LCK_Protect())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Protect"));
	RET(lock.Protect);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_Protect();

	RET(retval);
    }
}

void Nick_Stored_t::L_Protect(const bool in)
{
    FT("Nick_Stored_t::L_Protect", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Protect())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Protect"));
	    MCB(lock.Protect);
	    lock.Protect = in;
	    MCE(lock.Protect);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_Protect(in);
    }
}

bool Nick_Stored_t::Secure()
{
    NFT("Nick_Stored_t::Secure");
    if (Magick::instance().nickserv.LCK_Secure())
    {
	RET(Magick::instance().nickserv.DEF_Secure());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Secure"));
	RET(setting.Secure);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->Secure();

	RET(retval);
    }
}

void Nick_Stored_t::Secure(const bool in)
{
    FT("Nick_Stored_t::Secure", (in));
    if (Host().empty())
    {
	if (!L_Secure())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Secure"));
	    MCB(setting.Secure);
	    setting.Secure = in;
	    MCE(setting.Secure);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Secure(in);
    }
}

bool Nick_Stored_t::L_Secure()
{
    NFT("Nick_Stored_t::L_Secure");
    if (Magick::instance().nickserv.LCK_Secure())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Secure"));
	RET(lock.Secure);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_Secure();

	RET(retval);
    }
}

void Nick_Stored_t::L_Secure(const bool in)
{
    FT("Nick_Stored_t::L_Secure", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Secure())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Secure"));
	    MCB(lock.Secure);
	    lock.Secure = in;
	    MCE(lock.Secure);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_Secure(in);
    }
}

bool Nick_Stored_t::NoExpire()
{
    NFT("Nick_Stored_t::NoExpire");
    if (Magick::instance().nickserv.LCK_NoExpire())
    {
	RET(Magick::instance().nickserv.DEF_NoExpire());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.NoExpire"));
	RET(setting.NoExpire);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->NoExpire();

	RET(retval);
    }
}

void Nick_Stored_t::NoExpire(const bool in)
{
    FT("Nick_Stored_t::NoExpire", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_NoExpire())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.NoExpire"));
	    MCB(setting.NoExpire);
	    setting.NoExpire = in;
	    MCE(setting.NoExpire);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->NoExpire(in);
    }
}

bool Nick_Stored_t::NoMemo()
{
    NFT("Nick_Stored_t::NoMemo");
    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	RET(Magick::instance().nickserv.DEF_NoMemo());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.NoMemo"));
	RET(setting.NoMemo);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->NoMemo();

	RET(retval);
    }
}

void Nick_Stored_t::NoMemo(const bool in)
{
    FT("Nick_Stored_t::NoMemo", (in));
    if (Host().empty())
    {
	if (!L_NoMemo())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.NoMemo"));
	    MCB(setting.NoMemo);
	    setting.NoMemo = in;
	    MCE(setting.NoMemo);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->NoMemo(in);
    }
}

bool Nick_Stored_t::L_NoMemo()
{
    NFT("Nick_Stored_t::L_NoMemo");
    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.NoMemo"));
	RET(lock.NoMemo);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_NoMemo();

	RET(retval);
    }
}

void Nick_Stored_t::L_NoMemo(const bool in)
{
    FT("Nick_Stored_t::L_NoMemo", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_NoMemo())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.NoMemo"));
	    MCB(lock.NoMemo);
	    lock.NoMemo = in;
	    MCE(lock.NoMemo);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_NoMemo(in);
    }
}

bool Nick_Stored_t::Private()
{
    NFT("Nick_Stored_t::Private");
    if (Magick::instance().nickserv.LCK_Private())
    {
	RET(Magick::instance().nickserv.DEF_Private());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Private"));
	RET(setting.Private);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->Private();

	RET(retval);
    }
}

void Nick_Stored_t::Private(const bool in)
{
    FT("Nick_Stored_t::Private", (in));
    if (Host().empty())
    {
	if (!L_Private())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Private"));
	    MCB(setting.Private);
	    setting.Private = in;
	    MCE(setting.Private);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Private(in);
    }
}

bool Nick_Stored_t::L_Private()
{
    NFT("Nick_Stored_t::L_Private");
    if (Magick::instance().nickserv.LCK_Private())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Private"));
	RET(lock.Private);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_Private();

	RET(retval);
    }
}

void Nick_Stored_t::L_Private(const bool in)
{
    FT("Nick_Stored_t::L_Private", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Private())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Private"));
	    MCB(lock.Private);
	    lock.Private = in;
	    MCE(lock.Private);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_Private(in);
    }
}

bool Nick_Stored_t::PRIVMSG()
{
    NFT("Nick_Stored_t::PRIVMSG");
    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	RET(Magick::instance().nickserv.DEF_PRIVMSG());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.PRIVMSG"));
	RET(setting.PRIVMSG);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->PRIVMSG();

	RET(retval);
    }
}

void Nick_Stored_t::PRIVMSG(const bool in)
{
    FT("Nick_Stored_t::PRIVMSG", (in));
    if (Host().empty())
    {
	if (!L_PRIVMSG())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.PRIVMSG"));
	    MCB(setting.PRIVMSG);
	    setting.PRIVMSG = in;
	    MCE(setting.PRIVMSG);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->PRIVMSG(in);
    }
}

bool Nick_Stored_t::L_PRIVMSG()
{
    NFT("Nick_Stored_t::L_PRIVMSG");
    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.PRIVMSG"));
	RET(lock.PRIVMSG);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_PRIVMSG();

	RET(retval);
    }
}

void Nick_Stored_t::L_PRIVMSG(const bool in)
{
    FT("Nick_Stored_t::L_PRIVMSG", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_PRIVMSG())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.PRIVMSG"));
	    MCB(lock.PRIVMSG);
	    lock.PRIVMSG = in;
	    MCE(lock.PRIVMSG);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_PRIVMSG(in);
    }
}

mstring Nick_Stored_t::Language()
{
    NFT("Nick_Stored_t::Language");
    if (Magick::instance().nickserv.LCK_Language())
    {
	RET(Magick::instance().nickserv.DEF_Language().UpperCase());
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Language"));
	RET(setting.Language);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Language();

	RET(retval);
    }
}

void Nick_Stored_t::Language(const mstring & in)
{
    FT("Nick_Stored_t::Language", (in));
    if (Host().empty())
    {
	if (!L_Language())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Language"));
	    MCB(setting.Language);
	    setting.Language = in.UpperCase();
	    MCE(setting.Language);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->Language(in);
    }
}

bool Nick_Stored_t::L_Language()
{
    NFT("Nick_Stored_t::L_Language");
    if (Magick::instance().nickserv.LCK_Language())
    {
	RET(true);
    }
    else if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Language"));
	RET(lock.Language);
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->L_Language();

	RET(retval);
    }
}

void Nick_Stored_t::L_Language(const bool in)
{
    FT("Nick_Stored_t::L_Language", (in));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.LCK_Language())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "lock.Language"));
	    MCB(lock.Language);
	    lock.Language = in;
	    MCE(lock.Language);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->L_Language(in);
    }
}

bool Nick_Stored_t::Suspended()
{
    NFT("Nick_Stored_t::Suspended");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
	RET(!Suspend_By().empty());
    }
    else
    {
	bool retval = Magick::instance().nickserv.GetStored(i_Host)->Suspended();

	RET(retval);
    }
}

mstring Nick_Stored_t::Suspend_By()
{
    NFT("Nick_Stored_t::Suspend_By");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_By"));
	RET(i_Suspend_By);
    }
    else
    {
	mstring retval = Magick::instance().nickserv.GetStored(i_Host)->Suspend_By();

	RET(retval);
    }
}

mDateTime Nick_Stored_t::Suspend_Time()
{
    NFT("Nick_Stored_t::Suspend_Time");
    if (Host().empty())
    {
	RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_Suspend_Time"));
	RET(i_Suspend_Time);
    }
    else
    {
	mDateTime retval = Magick::instance().nickserv.GetStored(i_Host)->Suspend_Time();

	RET(retval);
    }
}

bool Nick_Stored_t::Forbidden() const
{
    NFT("Nick_Stored_t::Forbidden");
    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Forbidden"));
    RET(setting.Forbidden);
}

unsigned long Nick_Stored_t::PicNum()
{
    NFT("PicNum");
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.PicExt().empty())
	{
	    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Picture"));
	    RET(setting.Picture);
	}
	RET(0);
    }
    else
    {
	unsigned long retval = Magick::instance().nickserv.GetStored(i_Host)->PicNum();

	RET(retval);
    }
}

void Nick_Stored_t::GotPic(const unsigned long picnum)
{
    FT("Nick_Stored_t::GotPic", (picnum));
    if (Host().empty())
    {
	if (!Magick::instance().nickserv.PicExt().empty())
	{
	    WLOCK(("NickServ", "stored", i_Name.LowerCase(), "setting.Picture"));
	    MCB(setting.Picture);
	    setting.Picture = picnum;
	    MCE(setting.Picture);
	}
    }
    else
    {
	Magick::instance().nickserv.GetStored(i_Host)->GotPic(picnum);
    }
}

bool Nick_Stored_t::IsOnline()
{
    NFT("Nick_Stored_t::IsOnline");

    if (Magick::instance().nickserv.IsLive(i_Name))
    {
	map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(i_Name);
	// Not secure and recognized
	// or not suspended and identified
	if ((!Suspended() && nlive->IsIdentified()) || (!Secure() && nlive->IsRecognized()))
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
	RET(mDateTime::CurrentDateTime());
    }
    else if (Host().empty())
    {
	mDateTime lastseen;

	{
	    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
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
}

mDateTime Nick_Stored_t::LastSeenTime()
{
    NFT("Nick_Stored_t::LastSeenTime");
    if (IsOnline())
    {
	RET(mDateTime::CurrentDateTime());
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
	mstring retval = Magick::instance().nickserv.GetLive(i_Name)->RealName();

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
	RET(Magick::instance().getMessage("VALS/ONLINE"));
    }
    else if (Host().empty())
    {
	mDateTime lastseen;

	{
	    RLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
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
}

mstring Nick_Stored_t::LastMask()
{
    NFT("Nick_Stored_t::LastMask");
    if (IsOnline())
    {
	RET(Magick::instance().getMessage("VALS/ONLINE"));
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

void Nick_Stored_t::Quit(const mstring & message)
{
    FT("Nick_Stored_t::Quit", (message));

    // Dont whinge here, because they may not be
    // ident'd, in which case IsOnline() == false.
    if (IsOnline())
    {
	WLOCK(("NickServ", "stored", i_Name.LowerCase(), "i_LastSeenTime"));
	WLOCK2(("NickServ", "stored", i_Name.LowerCase(), "i_LastQuit"));
	MCB(i_LastSeenTime);
	CB(1, i_LastQuit);
	i_LastSeenTime = mDateTime::CurrentDateTime();
	i_LastQuit = message;
	CE(1, i_LastQuit);
	MCE(i_LastSeenTime);
    }
}

size_t Nick_Stored_t::MyChannels() const
{
    NFT("Nick_Stored_t::MyChannels");

    size_t count = 0;

    ChanServ::stored_t::const_iterator i;
    RLOCK(("ChanServ", "stored"));
    for (i = Magick::instance().chanserv.StoredBegin(); i != Magick::instance().chanserv.StoredEnd(); i++)
    {
	map_entry < Chan_Stored_t > cstored(i->second);
	if (cstored->Founder().IsSameAs(i_Name, true) || cstored->CoFounder().IsSameAs(i_Name, true))
	    count++;
    }
    RET(count);
}

SXP::Tag Nick_Stored_t::tag_Nick_Stored_t("Nick_Stored_t");
SXP::Tag Nick_Stored_t::tag_Name("Name");
SXP::Tag Nick_Stored_t::tag_RegTime("RegTime");
SXP::Tag Nick_Stored_t::tag_Password("Password");
SXP::Tag Nick_Stored_t::tag_Email("EMail");
SXP::Tag Nick_Stored_t::tag_URL("URL");
SXP::Tag Nick_Stored_t::tag_ICQ("ICQ");
SXP::Tag Nick_Stored_t::tag_AIM("AIM");
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
    static_cast < void > (pIn);

    FT("Nick_Stored_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if (pElement->IsA(tag_UserDef))
    {
	mstring *tmp = new mstring;

	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void Nick_Stored_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    static_cast < void > (pIn);

    FT("Nick_Stored_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
    if (pElement->IsA(tag_Name))
    {
	pElement->Retrieve(i_Name);
	ref_class::lockData(mVarArray("NickServ", "stored", i_Name.LowerCase()));
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
}

void Nick_Stored_t::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    static_cast < void > (attribs);

    //TODO: Add your source code here
    pOut->BeginObject(tag_Nick_Stored_t);

    // Only way to ENSURE the data wont change.
    WLOCK(("NickServ", "stored", i_Name.LowerCase()));
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
    retval += i_AIM.capacity();
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
}

void Nick_Stored_t::DumpB()
{
    MB(0,
       (i_Name, i_RegTime, i_Password, i_Email, i_URL, i_ICQ, i_AIM, i_Description, i_Comment, i_Host, i_slaves.size(),
	i_access.size(), i_ignore.size(), setting.Protect, lock.Protect, setting.Secure));
    MB(16,
       (lock.Secure, setting.NoExpire, setting.NoMemo, lock.NoMemo, setting.Private, lock.Private, setting.PRIVMSG,
	lock.PRIVMSG, setting.Language, lock.Language, setting.Forbidden, setting.Picture, i_Suspend_By, i_Suspend_Time,
	i_LastSeenTime, i_LastRealName));
    MB(32, (i_LastMask, i_LastQuit, i_UserDef.size()));
}

void Nick_Stored_t::DumpE()
{
    ME(0,
       (i_Name, i_RegTime, i_Password, i_Email, i_URL, i_ICQ, i_AIM, i_Description, i_Comment, i_Host, i_slaves.size(),
	i_access.size(), i_ignore.size(), setting.Protect, lock.Protect, setting.Secure));
    ME(16,
       (lock.Secure, setting.NoExpire, setting.NoMemo, lock.NoMemo, setting.Private, lock.Private, setting.PRIVMSG,
	lock.PRIVMSG, setting.Language, lock.Language, setting.Forbidden, setting.Picture, i_Suspend_By, i_Suspend_Time,
	i_LastSeenTime, i_LastRealName));
    ME(32, (i_LastMask, i_LastQuit, i_UserDef.size()));
}

// =======================================================================

NickServ::NickServ()
{
    NFT("NickServ::NickServ");
    messages = true;
}

mstring NickServ::findnextnick(const mstring & in)
{
    FT("NickServ::findnextnick", (in));

    // Amount of nicknames it will try, only
    // for the guest????? method.
    unsigned int i, attempts = 64;

    CP(("Renaming nickname %s", in.c_str()));
    if (Magick::instance().nickserv.Append_Rename())
    {
	for (i = 0; i < Magick::instance().nickserv.Suffixes().length(); i++)
	{
	    mstring retval(in);

	    while (retval.length() < Magick::instance().server.proto.NickLen())
	    {
		retval += Magick::instance().nickserv.Suffixes() [i];
		COM(("Attempting to use %s", retval.c_str()));
		if (!Magick::instance().nickserv.IsLiveAll(retval) && !Magick::instance().nickserv.IsStored(retval))
		{
		    RET(retval);
		}
	    }
	}
    }
    else
    {
	srand(time(NULL));
	for (i = 0; i < attempts; i++)
	{
	    mstring retval;

	    retval.Format("%s%05d", Magick::instance().nickserv.Suffixes().c_str(), rand() % 99999);
	    COM(("Attempting to use %s", retval.c_str()));
	    if (!Magick::instance().nickserv.IsLiveAll(retval) && !Magick::instance().nickserv.IsStored(retval))
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

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* CUR*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_Current);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* ADD", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* DEL*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* ERA*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* LIST", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* VIEW", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_access_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* ADD", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_ignore_Add);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* DEL*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_ignore_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* ERA*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_ignore_Del);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* LIST", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_ignore_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* VIEW", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_ignore_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PASS*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Password);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* E*MAIL*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Email);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* URL", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* WWW*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* WEB*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_URL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* ICQ*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_ICQ);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* AIM*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_AIM);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* DESC*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Description);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* COMM*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_set_Comment);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PIC*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Picture);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PROT*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Protect);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* SEC*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* NOEX*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_set_NoExpire);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* NOMEMO", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_NoMemo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PRIVM*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *MSG", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* PRIV*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* LANG*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_set_Language);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* H*LP", Magick::instance().commserv.REGD_Name(),
						 do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK PROT*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_Protect);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK SEC*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK NOMEMO", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_NoMemo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK PRIVM*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK *MSG", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK PRIV*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK LANG*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_lock_Language);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK H*LP", Magick::instance().commserv.SOP_Name(),
						 do_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK PROT*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_Protect);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK SEC*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_Secure);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK NOMEMO", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_NoMemo);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK PRIVM*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK *MSG", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_PRIVMSG);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK PRIV*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_Private);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK LANG*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_unlock_Language);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK H*LP", Magick::instance().commserv.SOP_Name(),
						 do_2param);

    Magick::instance().commands.AddSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Help);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "REG*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Register);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "DROP*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_Drop);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LINK*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Link);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "U*LIN*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_UnLink);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "HOST", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_Host);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "*SLAV*", Magick::instance().commserv.REGD_Name(),
						 NickServ::do_Slaves);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ID*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Identify);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SID*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Identify);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "INF*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Info);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GHOST*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Ghost);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "REC*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Recover);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LIST*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_List);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SEND*", Magick::instance().commserv.ALL_Name(),
						 NickServ::do_Send);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SUSP*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_Suspend);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNSUS*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_UnSuspend);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "FORB*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_Forbid);
#ifdef GETPASS
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "GETPASS*", Magick::instance().commserv.SADMIN_Name(),
						 NickServ::do_Getpass);
#endif
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SETPASS*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_Setpass);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LIVE*", Magick::instance().commserv.SOP_Name(),
						 NickServ::do_Live);

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "SET*", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "LOCK", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "UNLOCK", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "ACC*", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN* *", Magick::instance().commserv.REGD_Name(), NULL);
    Magick::instance().commands.AddSystemCommand(GetInternalName(), "IGN*", Magick::instance().commserv.REGD_Name(),
						 do_1_2param);
}

void NickServ::RemCommands()
{
    NFT("NickServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* CUR*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* ADD", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* DEL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* ERA*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* LIST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* VIEW", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PASS*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* E*MAIL*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* URL", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* WWW*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* WEB*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* ICQ*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* AIM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* DESC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* COMM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PIC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PROT*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* SEC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* NOEX*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* NOMEMO", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PRIVM*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *MSG", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* PRIV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* LANG*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* H*LP", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK PROT*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK NOMEMO", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK PRIVM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK *MSG", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK LANG*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK H*LP", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK PROT*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK SEC*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK NOMEMO", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK PRIVM*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK *MSG", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK PRIV*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK LANG*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK H*LP", Magick::instance().commserv.SOP_Name());

    Magick::instance().commands.RemSystemCommand(GetInternalName(), "H*LP", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "REG*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "DROP*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LINK*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "U*LIN*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "HOST", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "*SLAV*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ID*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "INF*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GHOST*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "REC*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LIST*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SEND*", Magick::instance().commserv.ALL_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SUSP*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNSUS*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "FORB*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "GET*PASS*", Magick::instance().commserv.SOP_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LIVE*", Magick::instance().commserv.SOP_Name());

    // These 'throw' the command back onto the map with
    // more paramaters.  IF you want to put wildcards in
    // it, you must add a terminator command (ie. "CMD* *"
    // in the command map, and NULL as the function).
    // This must be BEFORE the wildcarded map ("CMD*")
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "SET*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "LOCK", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "UNLOCK", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "ACC*", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN* *", Magick::instance().commserv.REGD_Name());
    Magick::instance().commands.RemSystemCommand(GetInternalName(), "IGN*", Magick::instance().commserv.REGD_Name());
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::AddStored(Nick_Stored_t * in) throw(E_NickServ_Stored)
#else
void NickServ::AddStored(Nick_Stored_t * in)
#endif
{
    FT("NickServ::AddStored", ("(Nick_Stored_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Add, E_NickServ_Stored::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Stored", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Add, E_NickServ_Stored::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Stored", "Add", "Blank"));
	return;
#endif
    }

    if (in->doDelete())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Add, E_NickServ_Stored::T_NotFound));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Stored", "Add", "NotFound"));
	return;
#endif
    }

    RLOCK(("NickServ", "stored"));
    map_entry < Nick_Stored_t > old_entry(stored, in->Name().LowerCase());
    if (old_entry.entry() != NULL)
    {
	old_entry->setDelete();
	stored.erase(in->Name().LowerCase());
    }
    WLOCK(("NickServ", "stored"));
    stored[in->Name().LowerCase()] = in;
}

#ifdef MAGICK_HAS_EXCEPTIONS
map_entry < Nick_Stored_t > NickServ::GetStored(const mstring & in) const throw(E_NickServ_Stored)
#else
map_entry < Nick_Stored_t > NickServ::GetStored(const mstring & in) const
#endif
{
    FT("NickServ::GetStored", (in));

    RLOCK(("NickServ", "stored"));
    NickServ::stored_t::const_iterator iter = stored.find(in.LowerCase());
    if (iter == stored.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Get, E_NickServ_Stored::T_NotFound, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Get", "NotFound", in));
	NRET(Nick_Stored_t &, GLOB_Nick_Stored_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Get, E_NickServ_Stored::T_Invalid, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Get", "Invalid", in));
	NRET(Nick_Stored_t &, GLOB_Nick_Stored_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Get, E_NickServ_Stored::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Get", "Blank", in));
	NRET(Nick_Stored_t &, GLOB_Nick_Stored_t);
#endif
    }

    NRET(map_entry < Nick_Stored_t >, map_entry < Nick_Stored_t > (iter->second));
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::RemStored(const mstring & in) throw(E_NickServ_Stored)
#else
void NickServ::RemStored(const mstring & in)
#endif
{
    FT("NickServ::RemStored", (in));

    RLOCK(("NickServ", "stored"));
    NickServ::stored_t::iterator iter = stored.find(in.LowerCase());
    if (iter == stored.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Stored(E_NickServ_Stored::W_Rem, E_NickServ_Stored::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Stored", "Rem", "NotFound", in));
	return;
#endif
    }

    if (iter->second != NULL)
    {
	map_entry < Nick_Stored_t > entry(iter->second);
	entry->setDelete();
    }
    WLOCK(("NickServ", "stored"));
    stored.erase(iter);
}

bool NickServ::IsStored(const mstring & in) const
{
    FT("NickServ::IsStored", (in));
    RLOCK(("NickServ", "stored"));
    bool retval = (stored.find(in.LowerCase()) != stored.end());

    RET(retval);
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::AddLive(Nick_Live_t * in) throw(E_NickServ_Live)
#else
void NickServ::AddLive(Nick_Live_t * in)
#endif
{
    FT("NickServ::AddLive", ("(Nick_Live_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Add, E_NickServ_Live::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Live", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Add, E_NickServ_Live::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Live", "Add", "Blank"));
	return;
#endif
    }

    if (in->doDelete())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Add, E_NickServ_Live::T_NotFound));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Live", "Add", "NotFound"));
	return;
#endif
    }

    RLOCK(("NickServ", "live"));
    map_entry < Nick_Live_t > old_entry(live, in->Name().LowerCase());
    if (old_entry.entry() != NULL)
    {
	old_entry->setDelete();
	live.erase(in->Name().LowerCase());
    }
    WLOCK(("NickServ", "live"));
    live[in->Name().LowerCase()] = in;
}

#ifdef MAGICK_HAS_EXCEPTIONS
map_entry < Nick_Live_t > NickServ::GetLive(const mstring & in) const throw(E_NickServ_Live)
#else
map_entry < Nick_Live_t > NickServ::GetLive(const mstring & in) const
#endif
{
    FT("NickServ::GetLive", (in));

    RLOCK(("NickServ", "live"));
    NickServ::live_t::const_iterator iter = live.find(in.LowerCase());
    if (iter == live.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Get, E_NickServ_Live::T_NotFound, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Get", "NotFound", in));
	RET(Nick_Live_t &, GLOB_Nick_Live_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Get, E_NickServ_Live::T_Invalid, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Get", "Invalid", in));
	RET(Nick_Live_t &, GLOB_Nick_Live_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Get, E_NickServ_Live::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Get", "Blank", in));
	RET(Nick_Live_t &, GLOB_Nick_Live_t);
#endif
    }

    NRET(map_entry < Nick_Live_t >, map_entry < Nick_Live_t > (iter->second));
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::RemLive(const mstring & in) throw(E_NickServ_Live)
#else
void NickServ::RemLive(const mstring & in)
#endif
{
    FT("NickServ::RemLive", (in));

    RLOCK(("NickServ", "live"));
    NickServ::live_t::iterator iter = live.find(in.LowerCase());
    if (iter == live.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Live(E_NickServ_Live::W_Rem, E_NickServ_Live::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Live", "Rem", "NotFound", in));
	return;
#endif
    }

    if (iter->second != NULL)
    {
	map_entry < Nick_Live_t > entry(iter->second);
	entry->setDelete();
    }
    WLOCK(("NickServ", "live"));
    live.erase(iter);
}

bool NickServ::IsLive(const mstring & in) const
{
    FT("NickServ::IsLive", (in));
    RLOCK(("NickServ", "live"));
    map_entry < Nick_Live_t > ent(live, in.LowerCase());
    if (ent.entry() != NULL && ent->Squit().empty())
	RET(true);
    RET(false);
}

bool NickServ::IsLiveAll(const mstring & in) const
{
    FT("NickServ::IsLiveAll", (in));
    RLOCK(("NickServ", "live"));
    bool retval = (live.find(in.LowerCase()) != live.end());

    RET(retval);
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::AddRecovered(const mstring & name, const mDateTime & in) throw(E_NickServ_Recovered)
#else
void NickServ::AddRecovered(const mstring & name, const mDateTime & in)
#endif
{
    FT("NickServ::AddRecovered", (name, in));

    if (name.empty() || in == mDateTime(0.0))
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Recovered(E_NickServ_Recovered::W_Add, E_NickServ_Recovered::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Recovered", "Add", "Blank"));
	return;
#endif
    }

    WLOCK(("NickServ", "recovered"));
    recovered[name.LowerCase()] = in;
}

#ifdef MAGICK_HAS_EXCEPTIONS
const mDateTime &NickServ::GetRecovered(const mstring & in) const throw(E_NickServ_Recovered)
#else
const mDateTime &NickServ::GetRecovered(const mstring & in) const
#endif
{
    FT("NickServ::GetRecovered", (in));

    RLOCK(("NickServ", "recovered"));
    map < mstring, mDateTime >::const_iterator iter = recovered.find(in.LowerCase());
    if (iter == recovered.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Recovered(E_NickServ_Recovered::W_Get, E_NickServ_Recovered::T_NotFound, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Recovered", "Get", "NotFound", in));
	RET(mDateTime &, GLOB_mDateTime);
#endif
    }
    if (iter->second == mDateTime(0.0))
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Recovered(E_NickServ_Recovered::W_Get, E_NickServ_Recovered::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Nick", "Recovered", "Get", "Blank", in));
	RET(mDateTime &, GLOB_mDateTime);
#endif
    }

    NRET(mDateTime &, iter->second);
}

#ifdef MAGICK_HAS_EXCEPTIONS
void NickServ::RemRecovered(const mstring & in) throw(E_NickServ_Recovered)
#else
void NickServ::RemRecovered(const mstring & in)
#endif
{
    FT("NickServ::RemRecovered", (in));

    WLOCK(("NickServ", "recovered"));
    map < mstring, mDateTime >::iterator iter = recovered.find(in.LowerCase());
    if (iter == recovered.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_NickServ_Recovered(E_NickServ_Recovered::W_Rem, E_NickServ_Recovered::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Nick", "Recovered", "Rem", "NotFound", in));
	return;
#endif
    }
    recovered.erase(iter);
}

bool NickServ::IsRecovered(const mstring & in) const
{
    FT("NickServ::IsRecovered", (in));
    RLOCK(("NickServ", "recovered"));
    bool retval = (recovered.find(in.LowerCase()) != recovered.end());

    RET(retval);
}

void NickServ::execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    mThread::ReAttach(tt_NickServ);
    FT("NickServ::execute", (source, msgtype, params));
    //okay this is the main nickserv command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message
    mstring mynick(Magick::instance().getLname(params.ExtractWord(1, ": ")));
    mstring message(params.After(":"));
    mstring command(message.Before(" "));

    if (message[0U] == CTCP_DELIM_CHAR)
    {
	if (msgtype == "PRIVMSG")
	    DccEngine::decodeRequest(mynick, source, message);
	else
	    DccEngine::decodeReply(mynick, source, message);
    }
    else if (msgtype == "PRIVMSG" && !Magick::instance().commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }

    mThread::ReAttach(tt_mBase);
}

void NickServ::do_Help(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Help", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    mstring HelpTopic = Magick::instance().nickserv.GetInternalName();

    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector < mstring > help = Magick::instance().getHelp(source, HelpTopic.UpperCase());

    unsigned int i;

    for (i = 0; i < help.size(); i++)
	::send(mynick, source, help[i]);
}

void NickServ::do_Register(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Register", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring password = params.ExtractWord(2, " ");
    mstring email;

    if (params.WordCount(" ") > 2)
	email = params.ExtractWord(3, " ");

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (Magick::instance().nickserv.IsStored(source))
    {
	NSEND(mynick, source, "NS_YOU_STATUS/ISSTORED");
    }
    else if (!nlive->HasMode("o") && nlive->LastNickReg().SecondsSince() < Magick::instance().nickserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET",
	     (message, ToHumanTime(Magick::instance().nickserv.Delay() - nlive->LastNickReg().SecondsSince(), source)));
    }
    else if (password.length() < 5 || password.IsSameAs(source, true))
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
    }
    else
    {
	if (!email.empty())
	{
	    if (!email.Contains("@"))
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
		return;
	    }
	    else if (email.WordCount("@") != 2)
	    {
		SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE",
		     (Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
		return;
	    }
	}

	nlive->SetLastNickReg();
	map_entry < Nick_Stored_t > nstored(new Nick_Stored_t(source, password));
	nstored->AccessAdd(nlive->Mask(Nick_Live_t::U_H).After("!"));
	if (!email.empty())
	    nstored->Email(email);
	Magick::instance().nickserv.AddStored(nstored);
	nlive->Identify(password);
	Magick::instance().nickserv.stats.i_Register++;
	SEND(mynick, source, "NS_YOU_COMMAND/REGISTERED", (nlive->Mask(Nick_Live_t::U_H).After("!")));
	LOG(LM_INFO, "NICKSERV/REGISTER", (nlive->Mask(Nick_Live_t::N_U_P_H), source));
    }
}

void NickServ::do_Drop(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Drop", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    mstring target;

    if (params.WordCount(" ") < 2)
    {
	target = source;
    }
    else if (!Magick::instance().nickserv.IsStored(params.ExtractWord(2, " ")))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (params.ExtractWord(2, " ")));
	return;
    }
    else
	target = Magick::instance().getSname(params.ExtractWord(2, " "));

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);

    // Our sibling, and we're not identified ...
    if (!nlive->IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (message, mynick));
	return;
    }

    if (!target.IsSameAs(source, true) && !nstored->IsSibling(source))
    {
	map_entry < Committee_t > comm;
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()))
	    comm = Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name());
	if (comm.entry() != NULL && comm->IsOn(source))
	{
	    // NOT our sibling, and its either a sop or oper ...
	    if ((comm->IsIn(target)) ||
		(Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		 Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(target)))
	    {
		SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (message));
		return;
	    }
	}
	else
	{
	    // Not ours at all, and we're not a sop!
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTYOURS", (target));
	    return;
	}
    }

    Magick::instance().nickserv.stats.i_Drop++;
    unsigned long dropped = nstored->Drop();

    Magick::instance().nickserv.RemStored(target);
    if (!Magick::instance().nickserv.IsStored(source))
	nlive->UnIdentify();
    if (source.IsSameAs(target, true))
	NSEND(mynick, source, "NS_YOU_COMMAND/DROPPED");
    else
	SEND(mynick, source, "NS_OTH_COMMAND/DROPPED", (target));

    LOG(LM_INFO, "NICKSERV/DROP", (nlive->Mask(Nick_Live_t::N_U_P_H), target, dropped - 1));

}

void NickServ::do_Link(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Link", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring hostnick = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(hostnick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (hostnick));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(hostnick);
    if (!nstored->Host().empty())
	hostnick = nstored->Host();
    else
	hostnick = nstored->Name();

    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (hostnick));
	return;
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->HasMode("o") && nlive->LastNickReg().SecondsSince() < Magick::instance().nickserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET",
	     (message, ToHumanTime(Magick::instance().nickserv.Delay() - nlive->LastNickReg().SecondsSince(), source)));
	return;
    }

    mDateTime regtime = mDateTime::CurrentDateTime();

    if (Magick::instance().nickserv.IsStored(source))
    {
	map_entry < Nick_Stored_t > orig = Magick::instance().nickserv.GetStored(source);
	mstring myhost = orig->Host();

	if (myhost.empty())
	{
	    if (source.IsSameAs(hostnick))
	    {
		SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (message));
		return;
	    }
	}
	else
	{
	    if (myhost.IsSameAs(hostnick))
	    {
		SEND(mynick, source, "ERR_SITUATION/NOTONSIBLING", (message));
		return;
	    }
	}

	if (!nlive->IsIdentified())
	{
	    SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (message, mynick));
	    return;
	}
	regtime = orig->RegTime();
    }

    if (nstored->Slave(source, password, regtime))
    {
	nlive->SetLastNickReg();
	Magick::instance().nickserv.stats.i_Link++;
	nlive->Identify(password);
	SEND(mynick, source, "NS_YOU_COMMAND/LINKED", (hostnick));
	LOG(LM_INFO, "NICKSERV/LINK", (nlive->Mask(Nick_Live_t::N_U_P_H), source, hostnick));
    }
    else
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
    }
}

void NickServ::do_UnLink(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_UnLink", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (message, mynick));
	return;
    }

    if (params.WordCount(" ") > 1)
    {
	mstring target = params.ExtractWord(2, " ");

	if (!Magick::instance().nickserv.IsStored(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	    return;
	}

	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
	target = nstored->Name();
	if (nstored->Host().empty())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISHOST", (target));
	    return;
	}

	if (!(nstored->IsSibling(source) || nstored->Host().IsSameAs(source, true)))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTYOURS", (target));
	    return;
	}

	if (nstored->Unlink())
	{
	    Magick::instance().nickserv.stats.i_Unlink++;
	    SEND(mynick, source, "NS_OTH_COMMAND/UNLINKED", (target));
	    LOG(LM_INFO, "NICKSERV/UNLINK", (nlive->Mask(Nick_Live_t::N_U_P_H), source, target));
	}
	else
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTLINKED", (target));
    }
    else
    {
	if (!Magick::instance().nickserv.IsStored(source))
	{
	    NSEND(mynick, source, "NS_YOU_STATUS/ISNOTSTORED");
	    return;
	}

	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
	if (nstored->Host().empty())
	{
	    NSEND(mynick, source, "NS_YOU_STATUS/ISHOST");
	    return;
	}

	mstring target = nstored->Host();

	if (nstored->Unlink())
	{
	    Magick::instance().nickserv.stats.i_Unlink++;
	    NSEND(mynick, source, "NS_YOU_COMMAND/UNLINKED");
	    LOG(LM_INFO, "NICKSERV/UNLINK", (nlive->Mask(Nick_Live_t::N_U_P_H), source, target));
	}
	else
	    NSEND(mynick, source, "NS_YOU_STATUS/ISNOTLINKED");
    }
}

void NickServ::do_Host(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Host", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    mstring newhost;

    if (params.WordCount(" ") > 1)
	newhost = params.ExtractWord(2, " ");

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (message, mynick));
	return;
    }

    if (!newhost.empty())
    {
	if (!Magick::instance().nickserv.IsStored(newhost))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (newhost));
	    return;
	}

	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(newhost);
	newhost = nstored->Name();
	if (nstored->Host().empty())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISHOST", (newhost));
	    return;
	}

	if (source.IsSameAs(newhost, true))
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (message));
	    return;
	}

	if (!nstored->IsSibling(source) && !nstored->Host().IsSameAs(source, true))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTLINKED", (newhost));
	    return;
	}

	mstring oldhost = nstored->Host();

	Magick::instance().nickserv.stats.i_Host++;
	nstored->MakeHost();
	SEND(mynick, source, "NS_OTH_COMMAND/NEWHOST", (newhost));
	LOG(LM_INFO, "NICKSERV/HOST", (nlive->Mask(Nick_Live_t::N_U_P_H), oldhost, newhost));
    }
    else
    {
	map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
	mstring oldhost = nstored->Host();

	Magick::instance().nickserv.stats.i_Host++;
	nstored->MakeHost();
	NSEND(mynick, source, "NS_YOU_COMMAND/NEWHOST");
	LOG(LM_INFO, "NICKSERV/HOST", (nlive->Mask(Nick_Live_t::N_U_P_H), oldhost, source));
    }
}

void NickServ::do_Slaves(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Slaves", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();
    mstring target;

    map_entry < Nick_Stored_t > nick;
    if (params.WordCount(" ") < 2)
    {
	nick = Magick::instance().nickserv.GetStored(source);
	target = nick->Name();
    }
    else
    {
	if (Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	    Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))
	{
	    target = params.ExtractWord(2, " ");
	    if (!Magick::instance().nickserv.IsStored(target))
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
		return;
	    }

	    nick = Magick::instance().nickserv.GetStored(target);
	    target = nick->Name();
	    if (nick->Forbidden())
	    {
		SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
		return;
	    }
	}
	else
	{
	    nick = Magick::instance().nickserv.GetStored(source);
	    target = nick->Name();
	}
    }

    if (!nick->Host().empty())
	target = nick->Host();

    mstring output;

    output << IRC_Bold << target << IRC_Off << " (" << nick->Siblings() << "):";

    for (unsigned int i = 0; i < nick->Siblings(); i++)
    {
	if (nick->Sibling(i).length() + output.length() > 510)
	{
	    ::send(mynick, source, output);
	    output.erase();
	    output << IRC_Bold << target << IRC_Off << " (" << nick->Siblings() << "):";
	}
	output << " " << nick->Sibling(i);
    }
    ::send(mynick, source, output);
}

void NickServ::do_Identify(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Identify", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring password = params.ExtractWord(2, " ");

    if (!Magick::instance().nickserv.IsStored(source))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (source));
	return;
    }

    if (Magick::instance().nickserv.GetStored(source)->Suspended())
    {
	NSEND(mynick, source, "NS_YOU_STATUS/ISSUSPENDED");
	return;
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    bool wasident = nlive->IsIdentified();
    mstring output = nlive->Identify(password);

    if (!wasident && nlive->IsIdentified())
    {
	Magick::instance().nickserv.stats.i_Identify++;
	LOG(LM_INFO, "NICKSERV/IDENTIFY", (nlive->Mask(Nick_Live_t::N_U_P_H), source));
    }
    if (!output.empty())
	::send(mynick, source, output);
}

void NickServ::do_Info(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Info", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    unsigned int i;
    mstring target = params.ExtractWord(2, " ");
    mstring output;

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(target);
    map_entry < Nick_Stored_t > host;
    bool isoper = (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		   Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsOn(source));
    bool issop = (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source));

    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nick->Name()));
	return;
    }

    SEND(mynick, source, "NS_INFO/REALNAME", (nick->Name(), nick->LastRealName()));
    if (!nick->Host().empty())
    {
	host = Magick::instance().nickserv.GetStored(nick->Host());
	SEND(mynick, source, "NS_INFO/HOST", (host->Name()));
    }

    output.erase();
    if (nick->NoExpire() && isoper)
	output << " (" << Magick::instance().getMessage(source, "NS_INFO/NOEXPIRE") << ")";
    SEND(mynick, source, "NS_INFO/REGISTERED", (nick->RegTime().Ago(), output));

    if (!nick->IsOnline())
    {
	output.erase();
	bool isonline = false;

	if (nick.entry() != NULL && host->IsOnline())
	    output = Magick::instance().nickserv.GetLive(nick->Host())->Name() + " ";
	for (i = 0; i < nick->Siblings(); i++)
	{
	    if (Magick::instance().nickserv.IsStored(nick->Sibling(i)) &&
		Magick::instance().nickserv.GetStored(nick->Sibling(i))->IsOnline())
	    {
		if (output.length() + nick->Sibling(i).length() > Magick::instance().server.proto.MaxLine())
		{
		    SEND(mynick, source, "NS_INFO/ONLINEAS", (output));
		    output.erase();
		    isonline = true;
		}
		output += Magick::instance().nickserv.GetLive(nick->Sibling(i))->Name() + " ";
	    }
	}
	if (!output.empty())
	{
	    SEND(mynick, source, "NS_INFO/ONLINEAS", (output));
	    isonline = true;
	}
	if (!isonline)
	{
	    if (isoper || !nick->Private())
		SEND(mynick, source, "NS_INFO/LASTALLMASK", (nick->LastAllMask()));
	    SEND(mynick, source, "NS_INFO/LASTALLSEEN", (nick->LastAllSeenTime().Ago()));
	    if (!nick->LastAllMask().IsSameAs(nick->Name() + "!" + nick->LastMask(), true) && (isoper || !nick->Private()))
		SEND(mynick, source, "NS_INFO/LASTMASK", (nick->LastMask()));
	    if (nick->LastAllSeenTime() != nick->LastSeenTime())
		SEND(mynick, source, "NS_INFO/LASTSEEN", (nick->LastSeenTime().Ago()));
	    SEND(mynick, source, "NS_INFO/QUITMSG", (nick->LastQuit()));
	}
    }
    if (nick->Suspended())
    {
	SEND(mynick, source, "NS_INFO/SUSPEND", (nick->Suspend_Time().Ago(), nick->Suspend_By()));
	SEND(mynick, source, "NS_INFO/SUSPENDFOR", (nick->Comment()));
    }
    else
    {
	if (!nick->Email().empty())
	    SEND(mynick, source, "NS_INFO/EMAIL", (nick->Email()));
	if (!nick->URL().empty())
	    SEND(mynick, source, "NS_INFO/URL", (nick->URL()));
	if (!nick->ICQ().empty())
	    SEND(mynick, source, "NS_INFO/ICQ", (nick->ICQ()));
	if (!nick->AIM().empty())
	    SEND(mynick, source, "NS_INFO/AIM", (nick->AIM()));
	if (!nick->Description().empty())
	    SEND(mynick, source, "NS_INFO/DESCRIPTION", (nick->Description()));
	if (!nick->Comment().empty() && (isoper || issop))
	    SEND(mynick, source, "NS_INFO/COMMENT", (nick->Comment()));
    }

    output.erase();
    CommServ::list_t::iterator iter;
    {
	RLOCK(("CommServ", "list"));
	for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
	{
	    map_entry < Committee_t > comm(iter->second);

	    // IF we're in the committee
	    if (!comm->IsIn(target))
		continue;

	    // AND committee is not ALL or REGD
	    if (iter->first == Magick::instance().commserv.ALL_Name() &&
		iter->first == Magick::instance().commserv.REGD_Name())
		continue;

	    // AND if it has a headcom, we're not in it
	    if (!comm->HeadCom().empty() && Magick::instance().commserv.IsList(comm->HeadCom()) &&
		Magick::instance().commserv.GetList(comm->HeadCom())->IsIn(nick->Name()))
		continue;

	    // AND the committee isnt private or the requesting user is in OPER
	    if (isoper || !comm->Private())
	    {
		if (output.length() + comm->Name().length() > Magick::instance().server.proto.MaxLine())
		{
		    SEND(mynick, source, "NS_INFO/COMMITTEES", (output));
		    output.erase();
		}
		if (!output.empty())
		    output << ", ";
		if (comm->IsHead(target) && !comm->Head().empty())
		    output << IRC_Bold;
		output << comm->Name();
		if (comm->IsHead(target) && !comm->Head().empty())
		    output << IRC_Off;
	    }
	}
    }
    if (!output.empty())
	SEND(mynick, source, "NS_INFO/COMMITTEES", (output));

    output.erase();
    bool firstoption = true;

    if (nick->Protect())
    {
	if (!firstoption)
	    output << ", ";
	else
	    firstoption = false;
	if (nick->L_Protect())
	    output << IRC_Bold;
	output << Magick::instance().getMessage(source, "NS_SET/PROTECT");
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
	output << Magick::instance().getMessage(source, "NS_SET/SECURE");
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
	output << Magick::instance().getMessage(source, "NS_SET/NOMEMO");
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
	output << Magick::instance().getMessage(source, "NS_SET/PRIVATE");
	if (nick->L_Private())
	    output << IRC_Off;
    }

    if (!output.empty())
	SEND(mynick, source, "NS_INFO/OPTIONS", (output));
    if (nick->PicNum() && Magick::instance().filesys.Exists(FileMap::Picture, nick->PicNum()))
	SEND(mynick, source, "NS_INFO/HASPIC",
	     (ToHumanSpace(Magick::instance().filesys.GetSize(FileMap::Picture, nick->PicNum())), mynick, nick->Name()));
    if (nick->IsOnline())
	SEND(mynick, source, "NS_INFO/ISONLINE", (Magick::instance().getLname(nick->Name())));
    {
	RLOCK(("Events"));
	if (Magick::instance().servmsg.ShowSync() && Magick::instance().events != NULL)
	    SEND(mynick, source, "MISC/SYNC", (Magick::instance().events->SyncTime(source)));
    }
}

void NickServ::do_Ghost(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Ghost", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nick = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (nick.IsSameAs(source, true))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (message));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nick));
	return;
    }

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTINUSE", (nick));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(nick);
    nick = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nick));
	return;
    }

    if (!nstored->CheckPass(pass))
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
	return;
    }

    Magick::instance().server.ANONKILL(mynick, nick,
				       source + " (" + Magick::instance().getMessage(nick, "MISC/KILL_GHOST") + ")");
    if (Magick::instance().nickserv.IsRecovered(nick))
	Magick::instance().nickserv.RemRecovered(nick);

    Magick::instance().nickserv.stats.i_Ghost++;
    SEND(mynick, source, "NS_OTH_COMMAND/RELEASED", (nick));
    LOG(LM_DEBUG, "NICKSERV/GHOST", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), nick));
}

void NickServ::do_Recover(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Recover", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nick = params.ExtractWord(2, " ");
    mstring pass = params.ExtractWord(3, " ");

    if (nick.IsSameAs(source, true))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONYOURSELF", (message));
	return;
    }

    if (!Magick::instance().nickserv.IsStored(nick))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nick));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(nick);
    nick = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nick));
	return;
    }

    if (!nstored->CheckPass(pass))
    {
	NSEND(mynick, source, "ERR_SITUATION/NICK_WRONG_PASS");
	return;
    }

    if (Magick::instance().nickserv.IsLive(nick))
    {
	Magick::instance().server.KILL(mynick, nick,
				       source + " (" + Magick::instance().getMessage(nick, "MISC/KILL_RECOVER") + ")");
    }

    Magick::instance().server.NICK(nick,
				   (Magick::instance().startup.Ownuser() ? nick.LowerCase() : Magick::instance().startup.
				    Services_User()), Magick::instance().startup.Services_Host(),
				   Magick::instance().startup.Server_Name(), Magick::instance().nickserv.Enforcer_Name());

    Magick::instance().nickserv.AddRecovered(nick, mDateTime::CurrentDateTime());
    Magick::instance().nickserv.stats.i_Recover++;
    SEND(mynick, source, "NS_OTH_COMMAND/HELD", (nick));
    LOG(LM_DEBUG, "NICKSERV/RECOVER", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), nick));
}

void NickServ::do_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_List", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	mask = "*";
	listsize = Magick::instance().config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Magick::instance().config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Magick::instance().config.Maxlist())
	{
	    mstring output;

	    SEND(mynick, source, "LIST/MAXLIST", (Magick::instance().config.Maxlist()));
	    return;
	}
    }

    SEND(mynick, source, "LIST/NICK_LIST", (mask));
    NickServ::stored_t::iterator iter;

    bool isoper = (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
		   Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsOn(source));
    bool issop = (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
		  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsOn(source));

    {
	RLOCK(("NickServ", "stored"));
	for (iter = Magick::instance().nickserv.StoredBegin(), i = 0, count = 0;
	     iter != Magick::instance().nickserv.StoredEnd(); iter++)
	{
	    map_entry < Nick_Stored_t > nstored(iter->second);
	    if (nstored->Name().Matches(mask, true))
	    {
		if (!nstored->Host().empty())
		    continue;

		if (i < listsize && (!nstored->Private() || isoper))
		{
		    if (issop)
		    {
			if (message.Contains("NOEXP") && !nstored->NoExpire())
			    continue;
			if (message.Contains("FORBID") && !nstored->Forbidden())
			    continue;
			if (message.Contains("SUSP") && !nstored->Suspended())
			    continue;
		    }

		    if (nstored->Forbidden())
		    {
			::send(mynick, source,
			       nstored->Name() + "  (" + Magick::instance().getMessage(source, "VALS/FORBIDDEN") + ")");
		    }
		    else
		    {
			::send(mynick, source, nstored->Name() + "  (" + nstored->LastAllMask() + ")");
		    }
		    i++;
		}
		count++;
	    }
	}
    }
    SEND(mynick, source, "LIST/DISPLAYED", (i, count));
}

void NickServ::do_Send(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Send", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    unsigned long picnum = nstored->PicNum();

    if (!picnum)
    {
	SEND(mynick, source, "NS_OTH_STATUS/NOPIC", (target));
	return;
    }

    if (!Magick::instance().filesys.Exists(FileMap::Picture, picnum))
    {
	nstored->GotPic(0);
	SEND(mynick, source, "DCC/NOFILE", ("SEND"));
	return;
    }

    mstring filename = Magick::instance().filesys.GetName(FileMap::Picture, picnum);
    size_t filesize = Magick::instance().filesys.GetSize(FileMap::Picture, picnum);

    if (filename.empty() || filesize <= 0)
    {
	nstored->GotPic(0);
	SEND(mynick, source, "DCC/NOFILE", ("SEND"));
	return;
    }

    if (!
	(Magick::instance().files.TempDirSize() == 0 ||
	 mFile::DirUsage(Magick::instance().files.TempDir()) <= Magick::instance().files.TempDirSize()))
    {
	NSEND(mynick, source, "DCC/NOSPACE2");
	return;
    }

    {
	RLOCK(("DCC"));
	if (Magick::instance().dcc != NULL)
	{
	    Magick::instance().nickserv.stats.i_Send++;
	    unsigned short port = mSocket::FindAvailPort();

	    ::privmsg(mynick, source,
		      DccEngine::encode("DCC SEND",
					filename + " " + mstring(Magick::instance().LocalHost()) + " " + mstring(port) + " " +
					mstring(filesize)));
	    Magick::instance().dcc->Accept(port, mynick, source, FileMap::Picture, picnum);
	}
    }
}

void NickServ::do_Suspend(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Suspend", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring reason = params.After(" ", 2);

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    if (nstored->Suspended())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISSUSPENDED", (target));
	return;
    }

    if (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(target))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONCOMMITTEE", (message, Magick::instance().commserv.OPER_Name()));
	return;
    }

    nstored->Suspend(source, reason);
    Magick::instance().nickserv.stats.i_Suspend++;
    SEND(mynick, source, "NS_OTH_COMMAND/SUSPENDED", (target));
    LOG(LM_NOTICE, "NICKSERV/SUSPEND",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, reason));
}

void NickServ::do_UnSuspend(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_UnSuspend", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    if (!nstored->Suspended())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSUSPENDED", (target));
	return;
    }

    nstored->UnSuspend();
    Magick::instance().nickserv.stats.i_Unsuspend++;
    SEND(mynick, source, "NS_OTH_COMMAND/UNSUSPENDED", (target));
    LOG(LM_NOTICE, "NICKSERV/UNSUSPEND", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target));
}

void NickServ::do_Forbid(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Forbid", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    if (Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISSTORED", (Magick::instance().getSname(target)));
	return;
    }

    if ((Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	 Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(target)) ||
	(Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	 Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsIn(target)))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (message));
	return;
    }

    map_entry < Nick_Stored_t > tmp(new Nick_Stored_t(target));
    Magick::instance().nickserv.AddStored(tmp);
    Magick::instance().nickserv.stats.i_Forbid++;
    SEND(mynick, source, "NS_OTH_COMMAND/FORBIDDEN", (target));
    LOG(LM_NOTICE, "NICKSERV/FORBID", (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target));
}

#ifdef GETPASS

void NickServ::do_Getpass(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Getpass", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    // If we are NOT a SADMIN, and target is a PRIV GROUP.
    if (!
	(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsIn(source)) &&
	(Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsIn(target) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsIn(target)) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->IsIn(target)) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(target))))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (message));
	return;
    }

    mstring password = nstored->Password();
    mstring host = nstored->Host();

    if (host.empty())
	host = nstored->Name();

    Magick::instance().nickserv.stats.i_Getpass++;
    SEND(mynick, source, "NS_OTH_COMMAND/GETPASS", (target, host, password));
    ANNOUNCE(mynick, "MISC/NICK_GETPASS", (source, target, host));
    LOG(LM_NOTICE, "NICKSERV/GETPASS",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, host));
}

#endif /* GETPASS */

void NickServ::do_Setpass(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Setpass", (mynick, source, params));

    mstring message = params.Before(" ").UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(2, " ");
    mstring password = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    // If we are NOT a SADMIN, and target is a PRIV GROUP.
    if (!
	(Magick::instance().commserv.IsList(Magick::instance().commserv.SADMIN_Name()) &&
	 Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsIn(source)) &&
	(Magick::instance().commserv.GetList(Magick::instance().commserv.SADMIN_Name())->IsIn(target) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.SOP_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.SOP_Name())->IsIn(target)) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.ADMIN_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.ADMIN_Name())->IsIn(target)) ||
	 (Magick::instance().commserv.IsList(Magick::instance().commserv.OPER_Name()) &&
	  Magick::instance().commserv.GetList(Magick::instance().commserv.OPER_Name())->IsIn(target))))
    {
	SEND(mynick, source, "ERR_SITUATION/NOTONPRIVCOMMITTEE", (message));
	return;
    }

    mstring host = nstored->Host();

    if (host.empty())
	host = nstored->Name();
    nstored->Password(password);

    Magick::instance().nickserv.stats.i_Getpass++;
    SEND(mynick, source, "NS_OTH_COMMAND/SETPASS", (target, host, password));
    ANNOUNCE(mynick, "MISC/NICK_SETPASS", (source, target, host));
    LOG(LM_NOTICE, "NICKSERV/SETPASS",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, host));
}

void NickServ::do_Live(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_Live", (mynick, source, params));

    unsigned int listsize, i, count;
    mstring mask;

    mstring message = params.Before(" ").UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 2)
    {
	mask = "*!*@*";
	listsize = Magick::instance().config.Listsize();
    }
    else if (params.WordCount(" ") < 3)
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = Magick::instance().config.Listsize();
    }
    else
    {
	mask = params.ExtractWord(2, " ").LowerCase();
	listsize = atoi(params.ExtractWord(3, " ").c_str());
	if (listsize > Magick::instance().config.Maxlist())
	{
	    mstring output;

	    SEND(mynick, source, "LIST/MAXLIST", (Magick::instance().config.Maxlist()));
	    return;
	}
    }

    if (!mask.Contains("@") && !mask.Contains("!"))
	mask += "!*@*";
    else if (!mask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "LIST/NICKNAME"), '@'));
	return;
    }
    else if (!mask.Contains("!"))
	mask.prepend("*!");

    SEND(mynick, source, "LIST/NICK_LIST", (mask));
    NickServ::live_t::iterator iter;

    {
	RLOCK(("NickServ", "live"));
	for (iter = Magick::instance().nickserv.LiveBegin(), i = 0, count = 0; iter != Magick::instance().nickserv.LiveEnd();
	     iter++)
	{
	    map_entry < Nick_Live_t > nlive(iter->second);
	    if (nlive->Mask(Nick_Live_t::N_U_P_H).Matches(mask, true))
	    {
		if (i < listsize)
		{
		    if (message.Contains("OPER") && !nlive->HasMode("o"))
			continue;

		    ::send(mynick, source,
			   nlive->Mask(Nick_Live_t::N_U_P_H) + " (" +
			   ((!nlive->Server().empty()) ? nlive->Server() : Magick::instance().startup.Server_Name()) +
			   ((!nlive->Squit().empty()) ? " (SQUIT)" : "") + "): +" + nlive->Mode());
		    i++;
		}
		count++;
	    }
	}
    }
    SEND(mynick, source, "LIST/DISPLAYED", (i, count));
}

void NickServ::do_access_Current(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_access_Current", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (Magick::instance().nickserv.GetStored(source)->AccessAdd(nlive->Mask(Nick_Live_t::U_H).After("!")))
    {
	Magick::instance().nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/ADD",
	     (nlive->Mask(Nick_Live_t::U_H).After("!"), Magick::instance().getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_ADD",
	    (nlive->Mask(Nick_Live_t::N_U_P_H), nlive->Mask(Nick_Live_t::U_H).After("!"), source));

    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS",
	     (nlive->Mask(Nick_Live_t::U_H).After("!"), Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
}

void NickServ::do_access_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_access_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring hostmask = params.ExtractWord(3, " ");

    if (hostmask.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/ACCESS"), '!'));
	return;
    }

    if (!hostmask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "LIST/ACCESS"), '@'));
	return;
    }

    if (Magick::instance().nickserv.GetStored(source)->AccessAdd(hostmask))
    {
	Magick::instance().nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/ADD", (hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_ADD",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), hostmask, source));
    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS", (hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
}

void NickServ::do_access_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_access_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring hostmask = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (hostmask.IsNumber())
    {
	if (hostmask.Contains(".") || hostmask.Contains("-"))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(hostmask.c_str());

	if (num <= 0 || num > nstored->Access())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, nstored->Access()));
	    return;
	}

	hostmask = nstored->Access(num - 1);
    }

    if (hostmask.Contains("!"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MAYNOTCONTAIN", (Magick::instance().getMessage(source, "LIST/ACCESS"), '!'));
	return;
    }

    if (!hostmask.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "LIST/ACCESS"), '@'));
	return;
    }

    unsigned int count = nstored->AccessDel(hostmask);

    if (count)
    {
	Magick::instance().nickserv.stats.i_Access++;
	SEND(mynick, source, "LIST/DEL_MATCH", (count, hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
	LOG(LM_DEBUG, "NICKSERV/ACCESS_DEL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), hostmask, source));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (hostmask, Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
}

void NickServ::do_access_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_access_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    mstring target = Magick::instance().getSname(source);

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    map_entry < Nick_Stored_t > nstored;
    if (params.WordCount(" ") >= 3 && Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsStored(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	    return;
	}

	nstored = Magick::instance().nickserv.GetStored(target);
	target = nstored->Name();
	if (nstored->Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	    return;
	}
    }
    else
	nstored = Magick::instance().nickserv.GetStored(target);

    if (nstored->Access())
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/DISPLAY2", (target, Magick::instance().getMessage(source, "LIST/ACCESS")));
	else
	    SEND(mynick, source, "LIST/DISPLAY", (Magick::instance().getMessage(source, "LIST/ACCESS")));
    }
    else
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/EMPTY2", (target, Magick::instance().getMessage(source, "LIST/ACCESS")));
	else
	    SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/ACCESS")));
	return;
    }

    unsigned int i;
    mstring retval;

    for (i = 0; i < nstored->Access(); i++)
    {
	retval.erase();
	retval << i + 1 << ". " << nstored->Access(i);
	::send(mynick, source, retval);
    }
}

void NickServ::do_ignore_Add(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_ignore_Add", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    if (Magick::instance().nickserv.GetStored(source)->IgnoreAdd(target))
    {
	Magick::instance().nickserv.stats.i_Ignore++;
	SEND(mynick, source, "LIST/ADD", (target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	LOG(LM_DEBUG, "NICKSERV/IGNORE_ADD",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, source));
    }
    else
    {
	SEND(mynick, source, "LIST/EXISTS", (target, Magick::instance().getMessage(source, "LIST/IGNORE")));
    }
}

void NickServ::do_ignore_Del(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_ignore_Del", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (target.IsNumber())
    {
	if (target.Contains(".") || target.Contains("-"))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(target.c_str());

	if (num <= 0 || num > nstored->Ignore())
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (1, nstored->Ignore()));
	    return;
	}

	target = nstored->Ignore(num - 1);
    }

    unsigned int count = nstored->IgnoreDel(target);

    if (count)
    {
	Magick::instance().nickserv.stats.i_Ignore++;
	SEND(mynick, source, "LIST/DEL_MATCH", (count, target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	LOG(LM_DEBUG, "NICKSERV/IGNORE_DEL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), target, source));
    }
    else
    {
	SEND(mynick, source, "LIST/NOTEXISTS", (target, Magick::instance().getMessage(source, "LIST/IGNORE")));
    }

}

void NickServ::do_ignore_List(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_ignore_List", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();
    mstring target = Magick::instance().getSname(source);

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    map_entry < Nick_Stored_t > nstored;
    if (params.WordCount(" ") >= 3 && Magick::instance().commserv.IsList(Magick::instance().commserv.OVR_View()) &&
	Magick::instance().commserv.GetList(Magick::instance().commserv.OVR_View())->IsOn(source))
    {
	target = params.ExtractWord(3, " ");
	if (!Magick::instance().nickserv.IsStored(target))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	    return;
	}

	nstored = Magick::instance().nickserv.GetStored(target);
	target = nstored->Name();
	if (nstored->Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	    return;
	}
    }
    else
	nstored = Magick::instance().nickserv.GetStored(target);

    if (nstored->Ignore())
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/DISPLAY2", (target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	else
	    SEND(mynick, source, "LIST/DISPLAY", (Magick::instance().getMessage(source, "LIST/IGNORE")));
    }
    else
    {
	if (!source.IsSameAs(target, true))
	    SEND(mynick, source, "LIST/EMPTY2", (target, Magick::instance().getMessage(source, "LIST/IGNORE")));
	else
	    SEND(mynick, source, "LIST/EMPTY", (Magick::instance().getMessage(source, "LIST/IGNORE")));
	return;
    }

    unsigned int i;
    mstring retval;

    for (i = 0; i < nstored->Ignore(); i++)
    {
	retval.erase();
	retval << i + 1 << ". " << nstored->Ignore(i);
	::send(mynick, source, retval);
    }
}

void NickServ::do_set_Password(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_Password", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newpass = params.ExtractWord(3, " ");

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!nlive->IsIdentified())
    {
	SEND(mynick, source, "ERR_SITUATION/NEED_NICK_IDENT", (message, mynick));
	return;
    }

#ifdef GETPASS
    mstring oldpass = Magick::instance().nickserv.GetStored(source).Password();

    if (newpass.IsSameAs(oldpass, true) ||
#else
    if (
#endif
	   newpass.IsSameAs(source, true) || newpass.length() < 5)
    {
	NSEND(mynick, source, "ERR_SITUATION/COMPLEX_PASS");
	return;
    }

    Magick::instance().nickserv.GetStored(source)->Password(newpass);
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/PASSWORD"), newpass));
    LOG(LM_INFO, "NICKSERV/SET_PASSWORD", (nlive->Mask(Nick_Live_t::N_U_P_H), source));
}

void NickServ::do_set_Email(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_Email", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();
    else if (!newvalue.Contains("@"))
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAIN", (Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
	return;
    }
    else if (newvalue.WordCount("@") != 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTCONTAINONE", (Magick::instance().getMessage(source, "NS_SET/EMAIL"), '@'));
	return;
    }

    Magick::instance().nickserv.GetStored(source)->Email(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/EMAIL"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/EMAIL"), source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/EMAIL")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/EMAIL"), source));
    }
}

void NickServ::do_set_URL(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_URL", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();

    if (newvalue.SubString(0, 6).IsSameAs("http://", true))
    {
	newvalue.erase(0, 6);
    }

    Magick::instance().nickserv.GetStored(source)->URL(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO",
	     (Magick::instance().getMessage(source, "NS_SET/URL"), "http://" + newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/URL"), source, "http://" + newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/URL")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/URL"), source));
    }
}

void NickServ::do_set_ICQ(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_ICQ", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();
    else if (!newvalue.IsNumber() || newvalue.Contains("."))
    {
	NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	return;
    }
    else if (newvalue.length() < 6 || newvalue.length() > 9)
    {
	SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (100000, 999999999));
	return;
    }

    Magick::instance().nickserv.GetStored(source)->ICQ(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/ICQ"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/ICQ"), source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/ICQ")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/ICQ"), source));
    }
}

void NickServ::do_set_AIM(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_AIM", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.ExtractWord(3, " ");

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();

    Magick::instance().nickserv.GetStored(source)->AIM(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/AIM"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/AIM"), source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/AIM")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/AIM"), source));
    }
}

void NickServ::do_set_Description(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_Description", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring newvalue = params.After(" ", 2);

    if (newvalue.IsSameAs("none", true))
	newvalue.erase();

    Magick::instance().nickserv.GetStored(source)->Description(newvalue);
    Magick::instance().nickserv.stats.i_Set++;
    if (!newvalue.empty())
    {
	SEND(mynick, source, "NS_YOU_COMMAND/SET_TO", (Magick::instance().getMessage(source, "NS_SET/DESCRIPTION"), newvalue));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/DESCRIPTION"), source, newvalue));
    }
    else
    {
	SEND(mynick, source, "NS_YOU_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/DESCRIPTION")));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/DESCRIPTION"), source));
    }
}

void NickServ::do_set_Comment(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_Comment", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring target = params.ExtractWord(3, " ");
    mstring comment = params.After(" ", 3);

    if (!Magick::instance().nickserv.IsStored(target))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (target));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(target);
    target = nstored->Name();
    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (target));
	return;
    }

    if (comment.IsSameAs("none", true))
	comment.erase();

    nstored->Comment(comment);
    Magick::instance().nickserv.stats.i_Set++;
    if (!comment.empty())
    {
	SEND(mynick, source, "NS_OTH_COMMAND/SET_TO",
	     (Magick::instance().getMessage(source, "NS_SET/COMMENT"), target, comment));
	LOG(LM_DEBUG, "NICKSERV/SET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/COMMENT"), target, comment));
    }
    else
    {
	SEND(mynick, source, "NS_OTH_COMMAND/UNSET", (Magick::instance().getMessage(source, "NS_SET/COMMENT"), target));
	LOG(LM_DEBUG, "NICKSERV/UNSET",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	     Magick::instance().getMessage("NS_SET/COMMENT"), target));
    }
}

void NickServ::do_set_Picture(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_Picture", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    if (Magick::instance().nickserv.PicExt().empty())
    {
	NSEND(mynick, source, "NS_YOU_STATUS/PICDISABLED");
	return;
    }

    Magick::instance().nickserv.stats.i_SetPicture++;
    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (params.WordCount(" ") > 2 && params.ExtractWord(3, " ").IsSameAs("none", true))
    {
	if (nstored->PicNum())
	    Magick::instance().filesys.EraseFile(FileMap::Picture, nstored->PicNum());
	nstored->GotPic(0u);
	NSEND(mynick, source, "NS_YOU_COMMAND/REMOVED");
	LOG(LM_DEBUG, "NICKSERV/PICTURE_DEL",
	    (Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H), source));
    }
    else
    {
	if (nstored->PicNum())
	    Magick::instance().filesys.EraseFile(FileMap::Picture, nstored->PicNum());
	nstored->GotPic(0u);
	Magick::instance().nickserv.GetLive(source)->InFlight.Picture(mynick);
    }
}

void NickServ::do_set_Protect(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_Protect", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_Protect())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/PROTECT")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->Protect(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/PROTECT"),
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PROTECT"), source,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_set_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_Secure", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_Secure())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/SECURE")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->Secure(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/SECURE"),
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/SECURE"), source,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_set_NoExpire(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_NoExpire", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(nickname);
    nickname = nstored->Name();

    if (nstored->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_NoExpire())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/NOEXPIRE"), nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_NoExpire())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->NoExpire(onoff.GetBool());
    Magick::instance().nickserv.stats.i_NoExpire++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/NOEXPIRE"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_NOTICE, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/NOEXPIRE"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_set_NoMemo(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_NoMemo", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_NoMemo())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/NOMEMO")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->NoMemo(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/NOMEMO"),
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/NOMEMO"), source,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_set_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_Private", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_Private())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/PRIVATE")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->Private(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/PRIVATE"),
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVATE"), source,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_set_PRIVMSG(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_PRIVMSG", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring onoff = params.ExtractWord(3, " ");

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_PRIVMSG())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/PRIVMSG")));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    nstored->PRIVMSG(onoff.GetBool());
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/OPT_SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/PRIVMSG"),
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVMSG"), source,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_set_Language(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_set_Language", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring lang = params.ExtractWord(3, " ").UpperCase();

    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(source);
    if (nstored->L_Language())
    {
	SEND(mynick, source, "NS_YOU_STATUS/ISLOCKED", (Magick::instance().getMessage(source, "NS_SET/LANGUAGE")));
	return;
    }

    if (lang.IsSameAs("default", true) || lang.IsSameAs("reset", true))
    {
	lang = Magick::instance().nickserv.DEF_Language().UpperCase();
    }
    else
    {
	// check for valid language ...
	if (!mFile::Exists(Magick::instance().files.Langdir() + DirSlash + lang.LowerCase() + ".lng"))
	{
	    SEND(mynick, source, "OS_STATUS/NOLANG", (lang));
	    return;
	}
    }

    nstored->Language(lang);
    Magick::instance().nickserv.stats.i_Set++;
    SEND(mynick, source, "NS_YOU_COMMAND/SET_TO",
	 (Magick::instance().getMessage(source, "NS_SET/LANGUAGE"),
	  mstring(lang + " (" + Magick::instance().getMessage(source, "ERR_SYNTAX/TRANSLATED") + ")")));
    LOG(LM_DEBUG, "NICKSERV/SET",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/LANGUAGE"), source, lang));
}

void NickServ::do_lock_Protect(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_lock_Protect", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Protect())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_Protect())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PROTECT")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Protect(false);
    nick->Protect(onoff.GetBool());
    nick->L_Protect(true);

    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/PROTECT"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PROTECT"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_lock_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_lock_Secure", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Secure())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_Secure())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/SECURE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Secure(false);
    nick->Secure(onoff.GetBool());
    nick->L_Secure(true);
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/SECURE"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/SECURE"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_lock_NoMemo(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_lock_NoMemo", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_NoMemo())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/NOMEMO")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_NoMemo(false);
    nick->NoMemo(onoff.GetBool());
    nick->L_NoMemo(true);
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/NOMEMO"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/NOMEMO"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_lock_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_lock_Private", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_Private())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_Private())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PRIVATE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Private(false);
    nick->Private(onoff.GetBool());
    nick->L_Private(true);
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/PRIVATE"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVATE"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_lock_PRIVMSG(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_lock_PRIVMSG", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring onoff = params.ExtractWord(4, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (onoff.IsSameAs("default", true) || onoff.IsSameAs("reset", true))
    {
	if (Magick::instance().nickserv.DEF_PRIVMSG())
	    onoff = "TRUE";
	else
	    onoff = "FALSE";
    }

    if (!onoff.IsBool())
    {
	NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	return;
    }

    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PRIVMSG")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_PRIVMSG(false);
    nick->PRIVMSG(onoff.GetBool());
    nick->L_PRIVMSG(true);
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/PRIVMSG"), nickname,
	  (onoff.GetBool() ? Magick::instance().getMessage(source, "VALS/ON") : Magick::instance().
	   getMessage(source, "VALS/OFF"))));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVMSG"), nickname,
	 (onoff.GetBool() ? Magick::instance().getMessage("VALS/ON") : Magick::instance().getMessage("VALS/OFF"))));
}

void NickServ::do_lock_Language(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_lock_Language", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    {
	RLOCK(("IrcSvcHandler"));
	if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	{
	    SEND(mynick, source, "MISC/HTM", (message));
	    return;
	}
    }

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");
    mstring lang = params.ExtractWord(4, " ").UpperCase();

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (lang.IsSameAs("default", true) || lang.IsSameAs("reset", true))
    {
	lang = Magick::instance().nickserv.DEF_Language().UpperCase();
    }
    else if (!mFile::Exists(Magick::instance().files.Langdir() + DirSlash + lang.LowerCase() + ".lng"))
    {
	SEND(mynick, source, "OS_STATUS/NOLANG", (lang));
	return;
    }

    if (Magick::instance().nickserv.LCK_Language())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/LANGUAGE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Language(false);
    nick->Language(lang);
    nick->L_Language(true);
    Magick::instance().nickserv.stats.i_Lock++;
    SEND(mynick, source, "NS_OTH_COMMAND/LOCKED",
	 (Magick::instance().getMessage(source, "NS_SET/LANGUAGE"), nickname,
	  mstring(lang + " (" + Magick::instance().getMessageL(lang, "ERR_SYNTAX/TRANSLATED") + ")")));
    LOG(LM_DEBUG, "NICKSERV/LOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/LANGUAGE"), nickname, lang));
}

void NickServ::do_unlock_Protect(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_unlock_Protect", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Protect())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PROTECT")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Protect(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/PROTECT"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PROTECT"), nickname));
}

void NickServ::do_unlock_Secure(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_unlock_Secure", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Secure())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/SECURE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Secure(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/SECURE"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/SECURE"), nickname));
}

void NickServ::do_unlock_NoMemo(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_unlock_NoMemo", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_NoMemo())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/NOMEMO")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_NoMemo(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/NOMEMO"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/NOMEMO"), nickname));
}

void NickServ::do_unlock_Private(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_unlock_Private", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Private())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PRIVATE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Private(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/PRIVATE"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVATE"), nickname));
}

void NickServ::do_unlock_PRIVMSG(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_unlock_PRIVMSG", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_PRIVMSG())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/PRIVMSG")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_PRIVMSG(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/OPT_UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/PRIVMSG"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/PRIVMSG"), nickname));
}

void NickServ::do_unlock_Language(const mstring & mynick, const mstring & source, const mstring & params)
{
    FT("NickServ::do_unlock_Language", (mynick, source, params));

    mstring message = params.Before(" ", 2).UpperCase();

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (message, mynick, message));
	return;
    }

    mstring nickname = params.ExtractWord(3, " ");

    if (!Magick::instance().nickserv.IsStored(nickname))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (nickname));
	return;
    }

    if (Magick::instance().nickserv.LCK_Language())
    {
	SEND(mynick, source, "ERR_SITUATION/ISLOCKED_NICK", (Magick::instance().getMessage(source, "NS_SET/LANGUAGE")));
	return;
    }

    map_entry < Nick_Stored_t > nick = Magick::instance().nickserv.GetStored(nickname);
    nickname = nick->Name();
    if (nick->Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (nickname));
	return;
    }

    nick->L_Language(false);
    Magick::instance().nickserv.stats.i_Unlock++;
    SEND(mynick, source, "NS_OTH_COMMAND/UNLOCKED", (Magick::instance().getMessage(source, "NS_SET/LANGUAGE"), nickname));
    LOG(LM_DEBUG, "NICKSERV/UNLOCK",
	(Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H),
	 Magick::instance().getMessage("NS_SET/LANGUAGE"), nickname));
}

SXP::Tag NickServ::tag_NickServ("NickServ");

void NickServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("NickServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Nick_Stored_t *ns = new Nick_Stored_t;

    if (pElement->IsA(ns->GetClassTag()))
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
    static_cast < void > (pIn);
    static_cast < void > (pElement);

    FT("NickServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void NickServ::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    static_cast < void > (attribs);

    FT("NickServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_NickServ);

    NickServ::stored_t::iterator iter;
    {
	RLOCK(("NickServ", "stored"));
	for (iter = StoredBegin(); iter != StoredEnd(); iter++)
	{
	    map_entry < Nick_Stored_t > nstored(iter->second);
	    pOut->WriteSubElement(nstored.entry());
	}
    }

    pOut->EndObject(tag_NickServ);
}

void NickServ::PostLoad()
{
    NFT("NickServ::PostLoad");
    unsigned int i, j;

    for (i = 0; i < ns_array.size(); i++)
    {
	if (ns_array[i] != NULL)
	{
	    for (j = 0; j < ns_array[i]->ud_array.size(); j++)
	    {
		if (ns_array[i]->ud_array[j] != NULL)
		{
		    if (ns_array[i]->ud_array[j]->Contains("\n"))
			ns_array[i]->i_UserDef[ns_array[i]->ud_array[j]->Before("\n")] = ns_array[i]->ud_array[j]->After("\n");
		    delete ns_array[i]->ud_array[j];
		}
	    }
	    ns_array[i]->ud_array.clear();

	    if (!ns_array[i]->Name().empty())
		AddStored(ns_array[i]);
	    // delete ns_array[i];
	}
    }
    ns_array.clear();

    NickServ::stored_t::iterator iter;
    CP(("Linking nickname entries ..."));
    RLOCK(("NickServ", "stored"));
    for (iter = StoredBegin(); iter != StoredEnd(); iter++)
    {
	map_entry < Nick_Stored_t > nstored(iter->second);
	if (!nstored->i_Host.empty())
	{
	    if (IsStored(nstored->i_Host))
	    {
		COM(("Nickname %s has been linked to %s ...", iter->first.c_str(), nstored->i_Host.c_str()));
		WLOCK(("NickServ", "stored", nstored->i_Host));
		GetStored(nstored->i_Host)->i_slaves.insert(iter->first);
	    }
	    else
	    {
		LOG(LM_WARNING, "ERROR/HOST_NOTREGD", (nstored->i_Host, iter->first));
		WLOCK(("NickServ", "stored", iter->first));
		nstored->i_Host.erase();
	    }
	}
    }
}
