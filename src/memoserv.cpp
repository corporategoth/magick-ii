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
** Revision 1.56  2000/05/17 07:47:59  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.55  2000/05/14 06:30:14  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.54  2000/05/14 04:02:54  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.53  2000/05/13 07:05:46  prez
** Added displaying of sizes to all file fields..
**
** Revision 1.52  2000/05/09 09:11:59  prez
** Added XMLisation to non-mapped structures ... still need to
** do the saving stuff ...
**
** Revision 1.51  2000/05/03 14:12:23  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.50  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.49  2000/04/04 03:21:35  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.48  2000/04/03 09:45:24  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.47  2000/03/29 09:41:19  prez
** Attempting to fix thread problem with mBase, and added notification
** of new memos on join of channel or signon to network.
**
** Revision 1.46  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.45  2000/03/26 14:59:37  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.44  2000/03/23 10:22:25  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.43  2000/03/19 08:50:55  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.42  2000/03/08 23:38:37  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.41  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.40  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.39  2000/02/17 12:55:05  ungod
** still working on borlandization
**
** Revision 1.38  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.37  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.36  2000/02/15 10:37:50  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#include "lockable.h"
#include "memoserv.h"
#include "magick.h"

Memo_t::Memo_t(mstring nick, mstring sender, mstring text, unsigned long file)
{
    FT("Memo_t::Memo_t", (nick, sender, text, file));
    i_Nick = nick;
    i_Time = Now();
    i_Sender = sender;
    i_Text = text;
    i_File = file;
    i_Read = false;
}


void Memo_t::operator=(const Memo_t &in)
{
    FT("Memo_t::operator=", ("(const Memo_t &) in"));
    i_Nick = in.i_Nick;
    i_Time = in.i_Time;
    i_Sender = in.i_Sender;
    i_Text = in.i_Text;
    i_File = in.i_File;
    i_Read = in.i_Read;
}


News_t::News_t(mstring channel, mstring sender, mstring text)
{
    FT("News_t::News_t", (channel, sender, text));
    i_Channel = channel;
    i_Time = Now();
    i_Sender = sender;
    i_Text = text;
}


void News_t::operator=(const News_t &in)
{
    FT("News_t::operator=", ("(const News_t &) in"));
    i_Channel = in.i_Channel;
    i_Time = in.i_Time;
    i_Sender = in.i_Sender;
    i_Text = in.i_Text;
    i_Read = in.i_Read;
}


bool News_t::IsRead(mstring name)
{
    FT("News_t::IsRead", (name));
    mstring target = name;
    if (!Parent->nickserv.IsStored(name))
	RET(false);
    if (Parent->nickserv.stored[name.LowerCase()].Host() != "")
	target = Parent->nickserv.stored[name.LowerCase()].Host();
    bool retval (i_Read.find(target.LowerCase())!=i_Read.end());
    RET(retval);
}


void News_t::Read(mstring name)
{
    FT("News_t::Read", (name));
    mstring target = name;
    if (!Parent->nickserv.IsStored(name))
	return;
    if (Parent->nickserv.stored[name.LowerCase()].Host() != "")
	target = Parent->nickserv.stored[name.LowerCase()].Host();
    i_Read.insert(target.LowerCase());
}


void News_t::Unread(mstring name)
{
    FT("News_t::Unread", (name));
    mstring target = name;
    if (!Parent->nickserv.IsStored(name))
	return;
    if (Parent->nickserv.stored[name.LowerCase()].Host() != "")
	target = Parent->nickserv.stored[name.LowerCase()].Host();
    i_Read.erase(target.LowerCase());
}

MemoServ::MemoServ()
{
    NFT("MemoServ::MemoServ");
    messages = true;
}

void MemoServ::AddCommands()
{
    NFT("MemoServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "H*LP", Parent->commserv.ALL_Name(), MemoServ::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "READ*", Parent->commserv.REGD_Name(), MemoServ::do_Read);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNREAD*", Parent->commserv.REGD_Name(), MemoServ::do_UnRead);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GET*", Parent->commserv.REGD_Name(), MemoServ::do_Get);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LIST*", Parent->commserv.REGD_Name(), MemoServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "VIEW*", Parent->commserv.REGD_Name(), MemoServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SEND*", Parent->commserv.REGD_Name(), MemoServ::do_Send);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FLUSH*", Parent->commserv.REGD_Name(), MemoServ::do_Flush);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FORW*", Parent->commserv.REGD_Name(), MemoServ::do_Forward);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "F*W*D", Parent->commserv.REGD_Name(), MemoServ::do_Forward);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "REP*", Parent->commserv.REGD_Name(), MemoServ::do_Reply);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CAN*", Parent->commserv.REGD_Name(), MemoServ::do_Cancel);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ABOR*", Parent->commserv.REGD_Name(), MemoServ::do_Cancel);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "DEL*", Parent->commserv.REGD_Name(), MemoServ::do_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ERA*", Parent->commserv.REGD_Name(), MemoServ::do_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CONT*", Parent->commserv.REGD_Name(), MemoServ::do_Continue);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE*", Parent->commserv.REGD_Name(), MemoServ::do_File);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*ATTACH", Parent->commserv.REGD_Name(), MemoServ::do_File);
}

void MemoServ::RemCommands()
{
    NFT("MemoServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "H*LP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "READ*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNREAD*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LIST*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "VIEW*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SEND*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FLUSH*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FORW*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "F*W*D", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "REP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CAN*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ABOR*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ERA*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CONT*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "*ATTACH", Parent->commserv.REGD_Name());
}

bool MemoServ::IsNick(mstring in)
{
    FT("MemoServ::IsNick", (in));
    bool retval = (nick.find(in.LowerCase())!=nick.end());
    RET(retval);
}

bool MemoServ::IsChannel(mstring in)
{
    FT("MemoServ::IsChannel", (in));
    bool retval = (channel.find(in.LowerCase())!=channel.end());
    RET(retval);
}

void MemoServ::execute(const mstring & data)
{
    mThread::ReAttach(tt_MemoServ);
    FT("MemoServ::execute", (data));
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
    else if (!Parent->commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }


    mThread::ReAttach(tt_mBase);

}

void MemoServ::do_Help(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    mstring HelpTopic = Parent->memoserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.Replace(" ", "/");
    vector<mstring> help = Parent->getHelp(source, HelpTopic.UpperCase());
					
    unsigned int i;
    for (i=0; i<help.size(); i++)
	::send(mynick, source, help[i]);
}

void MemoServ::do_Read(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Read", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.After(" ", 2);
	mstring whoami = source.LowerCase();
	if (Parent->nickserv.stored[whoami].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[whoami].Host()))
	{
	    whoami = Parent->nickserv.stored[whoami].Host().LowerCase();
	}

	if (!Parent->chanserv.IsStored(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
				who.c_str());
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "READMEMO"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MISC/NOACCESS"));
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/CS_EMPTY"),
				who.LowerCase().c_str());
	    return;
	}

	Parent->memoserv.stats.i_Read++;
	if (what.CmpNoCase("all")==0 ||
	    what.CmpNoCase("new")==0 || what.CmpNoCase("unread")==0)
	{
	    bool unread = (what.CmpNoCase("new")==0 || what.CmpNoCase("unread")==0);
	    list<News_t>::iterator iter;
	    int i = 1;
	    for (iter = Parent->memoserv.channel[who.LowerCase()].begin();
		    iter != Parent->memoserv.channel[who.LowerCase()].end(); iter++)
	    {
		if (unread && iter->IsRead(whoami))
		    continue;

		iter->Read(whoami);
		::send(mynick, source, Parent->getMessage("MS_COMMAND/NEWS"),
			i++, iter->Sender().c_str(), iter->Channel().c_str(),
			iter->Time().Ago().c_str());
		unsigned int sentsize;
		mstring output;
		for (sentsize = 0; sentsize < iter->Text().size(); sentsize+=450)
		{
		    if (sentsize + 450 < iter->Text().size())
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize, sentsize+450)
			       << "...";
		    }
		    else
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize, iter->Text().size()-1);
		    }
		    ::send(mynick, source, "    " + output);
		    output = "";
		}
	    }
	    if (i==1 && unread)
		::send(mynick, source, Parent->getMessage(source, "MS_STATUS/CS_NOUNREAD"),
			    who.LowerCase().c_str());
	}
	else
	{
	    vector<int> numbers = ParseNumbers(what);
	    unsigned int i;
	    int j=1;
	    mstring output;
	    bool triedabove = false, nonnumeric = false;
	    list<News_t>::iterator iter = Parent->memoserv.channel[who.LowerCase()].begin();
	    for (i=0; i<numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > Parent->memoserv.channel[who.LowerCase()].size())
		    triedabove = true;
		else
		{
		    while (numbers[i] < j &&
			iter != Parent->memoserv.channel[who.LowerCase()].begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j &&
			iter != Parent->memoserv.channel[who.LowerCase()].end())
		    {
			j++;
			iter++;
		    }
		    if (iter != Parent->memoserv.channel[who.LowerCase()].end())
		    {
			iter->Read(whoami);
			::send(mynick, source, Parent->getMessage("MS_COMMAND/NEWS"),
				j, iter->Sender().c_str(), iter->Sender().c_str(),
				iter->Time().Ago().c_str());
			unsigned int sentsize;
			for (sentsize = 0; sentsize < iter->Text().size(); sentsize+=450)
			{
			    if (sentsize + 450 < iter->Text().size())
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize, sentsize+450)
				    << "...";
			    }
			    else
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize, iter->Text().size()-1);
			    }
			    ::send(mynick, source, "    " + output);
			    output = "";
			}
		    }
		}
	    }
	    if (nonnumeric)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NONNUMERIC"));
	    if (triedabove)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/ABOVELIMIT"),
			Parent->memoserv.channel[who.LowerCase()].size());
	}
    }
    else
    {
	mstring who = source;
	mstring what = params.After(" ", 1);

	if (Parent->nickserv.stored[source.LowerCase()].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[source.LowerCase()].Host()))
	{
	    who = Parent->nickserv.stored[source.LowerCase()].Host();
	}

	if (!Parent->memoserv.IsNick(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/NS_EMPTY"));
	    return;
	}

	Parent->memoserv.stats.i_Read++;
	if (what.CmpNoCase("all")==0 ||
	    what.CmpNoCase("new")==0 || what.CmpNoCase("unread")==0)
	{
	    list<Memo_t>::iterator iter;
	    bool unread = (what.CmpNoCase("new")==0 || what.CmpNoCase("unread")==0);
	    int i = 1;
	    for (iter = Parent->memoserv.nick[who.LowerCase()].begin();
		    iter != Parent->memoserv.nick[who.LowerCase()].end(); iter++)
	    {
		if (unread && iter->IsRead())
		    continue;

		iter->Read();
		if (iter->File() && Parent->filesys.Exists(FileMap::MemoAttach, iter->File()))
		{
		    ::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/MEMO_FILE"),
			i++, iter->Sender().c_str(),
			iter->Time().Ago().c_str(),
			Parent->filesys.GetName(FileMap::MemoAttach, iter->File()).c_str(),
			ToHumanSpace(Parent->filesys.GetSize(FileMap::MemoAttach, iter->File())).c_str());
		}
		else
		{
		    ::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/MEMO"),
			i++, iter->Sender().c_str(),
			iter->Time().Ago().c_str());
		}
		unsigned int sentsize;
		mstring output;
		for (sentsize = 0; sentsize < iter->Text().size(); sentsize+=450)
		{
		    if (sentsize + 450 < iter->Text().size())
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize, sentsize+450)
			       << "...";
		    }
		    else
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize, iter->Text().size()-1);
		    }
		    ::send(mynick, source, "    " + output);
		    output = "";
		}
	    }
	}
	else
	{
	    vector<int> numbers = ParseNumbers(what);
	    unsigned int i;
	    int j=1;
	    bool triedabove = false, nonnumeric = false;
	    list<Memo_t>::iterator iter = Parent->memoserv.nick[who.LowerCase()].begin();
	    mstring output;
	    for (i=0; i<numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > Parent->memoserv.nick[who.LowerCase()].size())
		    triedabove = true;
		else
		{
		    while (numbers[i] < j &&
			iter != Parent->memoserv.nick[who.LowerCase()].begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j &&
			iter != Parent->memoserv.nick[who.LowerCase()].end())
		    {
			j++;
			iter++;
		    }
		    if (iter != Parent->memoserv.nick[who.LowerCase()].end())
		    {
			iter->Read();
			if (iter->File() && Parent->filesys.Exists(FileMap::MemoAttach, iter->File()))
			{
			    ::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/MEMO_FILE"),
				j, iter->Sender().c_str(),
				iter->Time().Ago().c_str(),
				Parent->filesys.GetName(FileMap::MemoAttach, iter->File()).c_str(),
				ToHumanSpace(Parent->filesys.GetSize(FileMap::MemoAttach, iter->File())).c_str());
			}
			else
			{
			    ::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/MEMO"),
				j, iter->Sender().c_str(),
				iter->Time().Ago().c_str());
			}
			unsigned int sentsize;
			for (sentsize = 0; sentsize < iter->Text().size(); sentsize+=450)
			{
			    if (sentsize + 450 < iter->Text().size())
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize, sentsize+450)
				    << "...";
			    }
			    else
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize, iter->Text().size()-1);
			    }
			    ::send(mynick, source, "    " + output);
			    output = "";
			}
		    }
		}
	    }
	    if (nonnumeric)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NONNUMERIC"));
	    if (triedabove)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/ABOVELIMIT"),
			Parent->memoserv.channel[who.LowerCase()].size());
	}
    }
}


void MemoServ::do_UnRead(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_UnRead", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.After(" ", 2);
	mstring whoami = source.LowerCase();
	if (Parent->nickserv.stored[whoami].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[whoami].Host()))
	{
	    whoami = Parent->nickserv.stored[whoami].Host().LowerCase();
	}

	if (!Parent->chanserv.IsStored(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		    who.c_str());
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "READMEMO"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MISC/NOACCESS"));
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/CS_EMPTY"),
				who.LowerCase().c_str());
	    return;
	}

	Parent->memoserv.stats.i_Unread++;
	if (what.CmpNoCase("all")==0)
	{
	    list<News_t>::iterator iter;
	    mstring output;
	    for (iter = Parent->memoserv.channel[who.LowerCase()].begin();
		    iter != Parent->memoserv.channel[who.LowerCase()].end(); iter++)
	    {
		iter->Unread(whoami);
	    }
	    ::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/UNREAD_ALL"),
				    who.LowerCase().c_str());
	}
	else
	{
	    vector<int> numbers = ParseNumbers(what);
	    unsigned int i;
	    int j=1;
	    bool triedabove = false, nonnumeric = false;
	    list<News_t>::iterator iter = Parent->memoserv.channel[who.LowerCase()].begin();
	    mstring output;
	    for (i=0; i<numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > Parent->memoserv.channel[who.LowerCase()].size())
		    triedabove = true;
		else
		{
		    while (numbers[i] < j &&
			iter != Parent->memoserv.channel[who.LowerCase()].begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j &&
			iter != Parent->memoserv.channel[who.LowerCase()].end())
		    {
			j++;
			iter++;
		    }
		    if (iter != Parent->memoserv.channel[who.LowerCase()].end())
		    {
			iter->Unread(whoami);
			if (output != "")
			    output << ", ";
			output << j;
		    }
		}
	    }
	    if (output != "")
	    {
		::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/CS_UNREAD"),
					    output.c_str(), who.c_str());
		output = "";
	    }
	    if (nonnumeric)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NONNUMERIC"));
	    if (triedabove)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/ABOVELIMIT"),
			Parent->memoserv.channel[who.LowerCase()].size());
	}
    }
    else
    {
	mstring who = source;
	mstring what = params.After(" ", 1);

	if (Parent->nickserv.stored[source.LowerCase()].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[source.LowerCase()].Host()))
	{
	    who = Parent->nickserv.stored[source.LowerCase()].Host();
	}

	if (!Parent->memoserv.IsNick(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/NS_EMPTY"));
	    return;
	}

	Parent->memoserv.stats.i_Unread++;
	if (what.CmpNoCase("all")==0)
	{
	    list<Memo_t>::iterator iter;
	    mstring output;
	    for (iter = Parent->memoserv.nick[who.LowerCase()].begin();
		    iter != Parent->memoserv.nick[who.LowerCase()].end(); iter++)
	    {
		iter->Unread();
	    }
	    ::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/NS_UNREAD_ALL"));
	}
	else
	{
	    vector<int> numbers = ParseNumbers(what);
	    unsigned int i;
	    int j=1;
	    bool triedabove = false, nonnumeric = false;
	    list<Memo_t>::iterator iter = Parent->memoserv.nick[who.LowerCase()].begin();
	    mstring output;
	    for (i=0; i<numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > Parent->memoserv.nick[who.LowerCase()].size())
		    triedabove = true;
		else
		{
		    while (numbers[i] < j &&
			iter != Parent->memoserv.nick[who.LowerCase()].begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j &&
			iter != Parent->memoserv.nick[who.LowerCase()].end())
		    {
			j++;
			iter++;
		    }
		    if (iter != Parent->memoserv.nick[who.LowerCase()].end())
		    {
			iter->Unread();
			if (output != "")
			    output << ", ";
			output << j;
		    }
		}
	    }
	    if (output != "")
	    {
		::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/NS_UNREAD"),
					output.c_str());
		output = "";
	    }
	    if (nonnumeric)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NONNUMERIC"));
	    if (triedabove)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/ABOVELIMIT"),
			Parent->memoserv.channel[who.LowerCase()].size());
	}
    }
}


void MemoServ::do_Get(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Get", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring who = source;
    mstring what = params.After(" ", 1);

    if (Parent->nickserv.stored[source.LowerCase()].Host() != "" &&
	Parent->nickserv.IsStored(Parent->nickserv.stored[source.LowerCase()].Host()))
    {
	who = Parent->nickserv.stored[source.LowerCase()].Host();
    }

    if (!Parent->memoserv.IsNick(who))
    {
	::send(mynick, source, Parent->getMessage(source, "MS_STATUS/NS_EMPTY"));
	return;
    }

    vector<int> numbers = ParseNumbers(what);
    unsigned int i;
    int j=1;
    bool triedabove = false, nonnumeric = false, nonfiles = false;
    list<Memo_t>::iterator iter = Parent->memoserv.nick[who.LowerCase()].begin();
    mstring output;
    for (i=0; i<numbers.size(); i++)
    {
	if (numbers[i] <= 0)
	    nonnumeric = true;
	else if (numbers[i] > Parent->memoserv.nick[who.LowerCase()].size())
	    triedabove = true;
	else
	{
	    while (numbers[i] < j &&
		iter != Parent->memoserv.nick[who.LowerCase()].begin())
	    {
		j--;
		iter--;
	    }
	    while (numbers[i] > j &&
		iter != Parent->memoserv.nick[who.LowerCase()].end())
	    {
		j++;
		iter++;
	    }
	    if (iter != Parent->memoserv.nick[who.LowerCase()].end())
	    {
		unsigned long filenum = iter->File();
		if (!filenum)
		{
		    nonfiles = true;
		    continue;
		}

		if (!Parent->filesys.Exists(FileMap::MemoAttach, filenum))
		{
		    nonfiles = true;
		    continue;
		}

		mstring filename = Parent->filesys.GetName(FileMap::MemoAttach, filenum);
		size_t filesize = Parent->filesys.GetSize(FileMap::MemoAttach, filenum);

		short port = FindAvailPort();
		::privmsg(mynick, source, DccEngine::encode("DCC SEND", filename +
			" " + mstring(ltoa(Parent->LocalHost())) + " " +
			mstring(itoa(port)) + " " + mstring(ltoa(filesize))));
		Parent->dcc->Accept(port, mynick, source, FileMap::MemoAttach, filenum);
	    }
	}
    }
    if (nonnumeric)
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NONNUMERIC"));
    if (triedabove)
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/ABOVELIMIT"),
		Parent->memoserv.channel[who.LowerCase()].size());
    if (nonfiles)
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NONFILES"));
 }


void MemoServ::do_List(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_List", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 1)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 2)
	{
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring whoami = source.LowerCase();
	if (Parent->nickserv.stored[whoami].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[whoami].Host()))
	{
	    whoami = Parent->nickserv.stored[whoami].Host().LowerCase();
	}

	if (!Parent->chanserv.IsStored(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		    who.c_str());
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "READMEMO"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MISC/NOACCESS"));
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/CS_EMPTY"),
				who.LowerCase().c_str());
	    return;
	}

	list<News_t>::iterator iter;
	int i = 1;
	mstring output;
	for (iter = Parent->memoserv.channel[who.LowerCase()].begin();
		iter != Parent->memoserv.channel[who.LowerCase()].end(); iter++)
	{
	    output = "";
	    if (iter->Text().size() > 20)
		output << iter->Text().SubString(0, 19) << "...";
	    else
		output << iter->Text().SubString(0, iter->Text().size()-1);

	    ::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/NEWS_LIST"),
		    iter->IsRead(whoami) ? ' ' : '*',
		    i++, iter->Sender().c_str(), iter->Channel().c_str(),
		    iter->Time().Ago().c_str(), output.c_str());
	}
    }
    else
    {
	mstring who = source;

	if (Parent->nickserv.stored[source.LowerCase()].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[source.LowerCase()].Host()))
	{
	    who = Parent->nickserv.stored[source.LowerCase()].Host();
	}

	if (!Parent->memoserv.IsNick(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/NS_EMPTY"));
	    return;
	}

	list<Memo_t>::iterator iter;
	int i = 1;
	mstring output;
	for (iter = Parent->memoserv.nick[who.LowerCase()].begin();
		iter != Parent->memoserv.nick[who.LowerCase()].end(); iter++)
	{
	    output = "";
	    if (iter->Text().size() > 20)
		output << iter->Text().SubString(0, 19) << "...";
	    else
		output << iter->Text().SubString(0, iter->Text().size()-1);

	    if (iter->File() && Parent->filesys.Exists(FileMap::MemoAttach, iter->File()))
	    {
		::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/MEMO_LIST_FILE"),
		    iter->IsRead() ? ' ' : '*',
		    i++, iter->Sender().c_str(),
		    iter->Time().Ago().c_str(), output.c_str(),
		    Parent->filesys.GetName(FileMap::MemoAttach, iter->File()).c_str(),
		    ToHumanSpace(Parent->filesys.GetSize(FileMap::MemoAttach, iter->File())).c_str());
	    }
	    else
	    {
		::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/MEMO_LIST"),
		    iter->IsRead() ? ' ' : '*',
		    i++, iter->Sender().c_str(),
		    iter->Time().Ago().c_str(), output.c_str());
	    }
	}
    }
}


void MemoServ::do_Send(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Send", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring name = params.ExtractWord(2, " ");
    mstring text = params.After(" ", 2);

    if (IsChan(name))
    {
	if (!Parent->chanserv.IsStored(name))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
			name.c_str());
	    return;
	}
	name = Parent->getSname(name);

	Chan_Stored_t *chan = &Parent->chanserv.stored[name.LowerCase()];
	if (!chan->GetAccess(source, "WRITEMEMO"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MISC/NOACCESS"));
	    return;
	}
    }
    else
    {
	if (!Parent->nickserv.IsStored(name))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
			name.c_str());
	    return;
	}
	name = Parent->getSname(name);

	if (Parent->nickserv.stored[name.LowerCase()].IsIgnore(source) ?
		!Parent->nickserv.stored[name.LowerCase()].NoMemo() :
		Parent->nickserv.stored[name.LowerCase()].NoMemo())
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/IGNORE"),
			    name.c_str());
	    return;
	}
    }

    if (text.size() > 450)
    {
	text.Truncate(450);
	::send(mynick, source, Parent->getMessage(source, "MS_STATUS/TRUNCATE"),
		    text.SubString(430, 450).c_str(), mynick.c_str());
    }

    Parent->memoserv.stats.i_Send++;
    Parent->nickserv.live[source.LowerCase()].InFlight.Memo(
					    false, mynick, name, text);
}


void MemoServ::do_Flush(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Flush", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->nickserv.live[source.LowerCase()].InFlight.Memo())
    {
	if (Parent->nickserv.live[source.LowerCase()].InFlight.File())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOFLUSH"));
	    return;
	}
	Parent->memoserv.stats.i_Flush++;
	Parent->nickserv.live[source.LowerCase()].InFlight.End(0u);
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "MS_STATUS/NOPENDING"));
    }
}

void MemoServ::do_Forward(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Forward", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 4)
	{
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.ExtractWord(3, " ");
	mstring dest = params.ExtractWord(4, " ");
	mstring whoami = source.LowerCase();

	if (Parent->nickserv.stored[whoami].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[whoami].Host()))
	{
	    whoami = Parent->nickserv.stored[whoami].Host().LowerCase();
	}

	if (!Parent->chanserv.IsStored(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		    who.c_str());
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "READ"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MISC/NOACCESS"));
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/CS_EMPTY"),
				who.LowerCase().c_str());
	    return;
	}

	if (!what.IsNumber() || what.Contains("."))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER"));
	    return;
	}

	unsigned int num = ACE_OS::atoi(what.c_str());
	if (num <= 0 || num > Parent->memoserv.channel[who.LowerCase()].size())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
			1, Parent->memoserv.channel[who.LowerCase()].size());
	    return;
	}

	unsigned int i;
	list<News_t>::iterator iter = Parent->memoserv.channel[who.LowerCase()].begin();
	for (i=1; i < num; iter++, i++) ;
	mstring output = "";
	output.Format(Parent->getMessage(dest, "MS_STATUS/FORWARD_ARG").c_str(),
		Parent->chanserv.stored[who.LowerCase()].Name().c_str(),
		iter->Sender().c_str(), iter->Text().c_str());

	do_Forward2(mynick, source, dest, output);
    }
    else
    {
	mstring who = source;
	mstring what = params.ExtractWord(2, " ");
	mstring dest = params.ExtractWord(3, " ");

	if (Parent->nickserv.stored[source.LowerCase()].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[source.LowerCase()].Host()))
	{
	    who = Parent->nickserv.stored[source.LowerCase()].Host();
	}

	if (!Parent->memoserv.IsNick(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/NS_EMPTY"));
	    return;
	}

	if (!what.IsNumber() || what.Contains("."))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER"));
	    return;
	}

	unsigned int num = ACE_OS::atoi(what.c_str());
	if (num <= 0 || num > Parent->memoserv.nick[who.LowerCase()].size())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
			1, Parent->memoserv.nick[who.LowerCase()].size());
	    return;
	}

	unsigned int i;
	list<Memo_t>::iterator iter = Parent->memoserv.nick[who.LowerCase()].begin();
	for (i=1; i < num; iter++, i++) ;

	mstring output = "";
	if (iter->File())
	    output.Format(Parent->getMessage(dest, "MS_STATUS/FORWARD_ARG").c_str(),
		"filename",
		iter->Sender().c_str(), iter->Text().c_str());
	else
	    output.Format(Parent->getMessage(dest, "MS_STATUS/FORWARD").c_str(),
		iter->Sender().c_str(), iter->Text().c_str());

	do_Forward2(mynick, source, dest, output);
    }
}


void MemoServ::do_Forward2(mstring mynick, mstring source, mstring dest,
						mstring text)
{
    FT("MemoServ::do_Forward2", (mynick, source, dest, text));

    if (IsChan(dest))
    {
	if (!Parent->chanserv.IsStored(dest))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
			dest.c_str());
	    return;
	}
	dest = Parent->getSname(dest);

	Chan_Stored_t *chan = &Parent->chanserv.stored[dest.LowerCase()];
	if (!chan->GetAccess(source, "WRITEMEMO"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MISC/NOACCESS"));
	    return;
	}
    }
    else
    {
	if (!Parent->nickserv.IsStored(dest))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
			dest.c_str());
	    return;
	}
	dest = Parent->getSname(dest);

	if (Parent->nickserv.stored[dest.LowerCase()].IsIgnore(source) ?
		!Parent->nickserv.stored[dest.LowerCase()].NoMemo() :
		Parent->nickserv.stored[dest.LowerCase()].NoMemo())
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/IGNORE"),
			    dest.c_str());
	    return;
	}
    }

    Parent->memoserv.stats.i_Forward++;
    Parent->nickserv.live[source.LowerCase()].InFlight.Memo(
					    false, mynick, dest, text);
    Parent->nickserv.live[source.LowerCase()].InFlight.End(0);
}


void MemoServ::do_Reply(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Reply", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 4)
	{
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.ExtractWord(3, " ");
	mstring text = params.After(" ", 3);
	mstring whoami = source.LowerCase();

	if (Parent->nickserv.stored[whoami].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[whoami].Host()))
	{
	    whoami = Parent->nickserv.stored[whoami].Host().LowerCase();
	}

	if (!Parent->chanserv.IsStored(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		    who.c_str());
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "WRITEMEMO"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MISC/NOACCESS"));
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/CS_EMPTY"),
				who.LowerCase().c_str());
	    return;
	}

	if (!what.IsNumber() || what.Contains("."))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER"));
	    return;
	}

	unsigned int num = ACE_OS::atoi(what.c_str());
	if (num <= 0 || num > Parent->memoserv.channel[who.LowerCase()].size())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
			1, Parent->memoserv.channel[who.LowerCase()].size());
	    return;
	}

	unsigned int i;
	list<News_t>::iterator iter = Parent->memoserv.channel[who.LowerCase()].begin();
	for (i=1; i < num; iter++, i++) ;
	mstring output = "";
	output.Format(Parent->getMessage("MS_STATUS/REPLY_ARG").c_str(),
		iter->Sender().c_str(),
		(iter->Text().size() < 21) ?
		    (iter->Text().SubString(0, 19) + "...").c_str() :
		    iter->Text().SubString(0, iter->Text().size()).c_str(),
		text.c_str());

	Parent->memoserv.stats.i_Reply++;
	Parent->nickserv.live[source.LowerCase()].InFlight.Memo(
					    false, mynick, who, output);
    }
    else
    {
	mstring who = source;
	mstring what = params.ExtractWord(2, " ");
	mstring text = params.After(" ", 2);

	if (Parent->nickserv.stored[source.LowerCase()].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[source.LowerCase()].Host()))
	{
	    who = Parent->nickserv.stored[source.LowerCase()].Host();
	}

	if (!Parent->memoserv.IsNick(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/NS_EMPTY"));
	    return;
	}
	who = Parent->getSname(who);

	if (!what.IsNumber() || what.Contains("."))
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/POSWHOLENUMBER"));
	    return;
	}

	unsigned int num = ACE_OS::atoi(what.c_str());
	if (num <= 0 || num > Parent->memoserv.nick[who.LowerCase()].size())
	{
	    ::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/MUSTBENUMBER"),
			1, Parent->memoserv.nick[who.LowerCase()].size());
	    return;
	}

	unsigned int i;
	list<Memo_t>::iterator iter = Parent->memoserv.nick[who.LowerCase()].begin();
	for (i=1; i < num; iter++, i++) ;

	if (!Parent->nickserv.IsStored(iter->Sender()))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
		    Parent->getSname(iter->Sender()).c_str());
	    return;
	}

	if (Parent->nickserv.stored[iter->Sender().LowerCase()].IsIgnore(source) ?
		!Parent->nickserv.stored[iter->Sender().LowerCase()].NoMemo() :
		Parent->nickserv.stored[iter->Sender().LowerCase()].NoMemo())
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/IGNORE"),
			    iter->Sender().c_str());
	    return;
	}

	mstring output = "";
	if (iter->File())
	    output.Format(Parent->getMessage("MS_STATUS/REPLY_ARG").c_str(),
		"filename",
		(iter->Text().size() < 21) ?
		    (iter->Text().SubString(0, 19) + "...").c_str() :
		    iter->Text().SubString(0, iter->Text().size()).c_str(),
		text.c_str());
	else
	    output.Format(Parent->getMessage("MS_STATUS/REPLY").c_str(),
		(iter->Text().size() < 21) ?
		    (iter->Text().SubString(0, 19) + "...").c_str() :
		    iter->Text().SubString(0, iter->Text().size()).c_str(),
		text.c_str());

	Parent->memoserv.stats.i_Reply++;
	Parent->nickserv.live[source.LowerCase()].InFlight.Memo(
					    false, mynick, who, output);
    }
}


void MemoServ::do_Cancel(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Cancel", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->nickserv.live[source.LowerCase()].InFlight.Memo())
    {
	Parent->memoserv.stats.i_Cancel++;
	Parent->nickserv.live[source.LowerCase()].InFlight.Cancel();
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "MS_STATUS/NOPENDING"));
    }
}


void MemoServ::do_Del(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Del", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.After(" ", 2);
	mstring whoami = source.LowerCase();

	if (Parent->nickserv.stored[whoami].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[whoami].Host()))
	{
	    whoami = Parent->nickserv.stored[whoami].Host().LowerCase();
	}

	if (!Parent->chanserv.IsStored(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "CS_STATUS/ISNOTSTORED"),
		    who.c_str());
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "WRITEMEMO"))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MISC/NOACCESS"));
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/CS_EMPTY"),
				who.LowerCase().c_str());
	    return;
	}

	Parent->memoserv.stats.i_Del++;
	if (what.CmpNoCase("all")==0)
	{
	    if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "DELMEMO"))
	    {
	    ::send(mynick, source, Parent->getMessage(source, "MISC/NOACCESS"));
		return;
	    }

	    list<News_t>::iterator iter;
	    mstring output;
	    for (iter = Parent->memoserv.channel[who.LowerCase()].begin();
		    iter != Parent->memoserv.channel[who.LowerCase()].end();)
	    {
		Parent->memoserv.channel[who.LowerCase()].erase(iter);
		iter = Parent->memoserv.channel[who.LowerCase()].begin();
	    }
	    Parent->memoserv.channel.erase(who.LowerCase());
	    ::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/CS_DEL_ALL"),
				    who.c_str());
	}
	else
	{
	    unsigned int i;
	    /* Adjust is to accomodate for memos that have
	     * been deleted before the current one, ie. if
	     * 1, 3, 5 and 7 are being deleted, once 1 has
	     * been deleted, adjust becomes 1, meaning that
	     * 3 has become 2, 5 has become 4, etc.  Once 3
	     * has been deleted, adjust becomes 2, meaning
	     * that 5 has become 3 and 7 has become 5.
	     */
	    int j=1, adjust=0;
	    vector<int> numbers1 = ParseNumbers(what);
	    set<int> numbers;
	    set<int>::iterator ni;
	    for (i=0; i<numbers1.size(); i++)
		numbers.insert(numbers1[i]);

	    bool triedabove = false, nonnumeric = false;
	    list<News_t>::iterator iter = Parent->memoserv.channel[who.LowerCase()].begin();
	    mstring output, denied;
	    for (ni = numbers.begin(); ni != numbers.end(); ni++)
	    {
		if (*ni - adjust <= 0)
		    nonnumeric = true;
		else if (*ni - adjust > Parent->memoserv.channel[who.LowerCase()].size())
		    triedabove = true;
		else
		{
		    while (*ni - adjust > j &&
			iter != Parent->memoserv.channel[who.LowerCase()].end())
		    {
			j++;
			iter++;
		    }
		    if (iter != Parent->memoserv.channel[who.LowerCase()].end())
		    {
			if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "DELMEMO") &&
			    iter->Sender().LowerCase() != who.LowerCase())
			{
			    if (denied != "")
				denied << ", ";
			    denied << j;
			}
			else
			{
			    if (output != "")
				output << ", ";
			    output << j;
			    Parent->memoserv.channel[who.LowerCase()].erase(iter);
			    iter = Parent->memoserv.channel[who.LowerCase()].begin();
			    j=1;
			    adjust++;
			}
		    }
		}
	    }
	    if (Parent->memoserv.channel[who.LowerCase()].size() == 0)
		Parent->memoserv.channel.erase(who.LowerCase());
	    if (denied != "")
	    {
		::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/CS_NOTDEL"),
			    denied.c_str(), who.c_str());
		denied = "";
	    }
	    if (output != "")
	    {
		::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/CS_DEL"),
			    output.c_str(), who.c_str());
		output = "";
	    }
	    if (nonnumeric)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NONNUMERIC"));
	    if (triedabove)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/ABOVELIMIT"),
			Parent->memoserv.channel[who.LowerCase()].size());
	}
    }
    else
    {
	mstring who = source;
	mstring what = params.After(" ", 1);

	if (Parent->nickserv.stored[source.LowerCase()].Host() != "" &&
	    Parent->nickserv.IsStored(Parent->nickserv.stored[source.LowerCase()].Host()))
	{
	    who = Parent->nickserv.stored[source.LowerCase()].Host();
	}

	if (!Parent->memoserv.IsNick(who))
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/NS_EMPTY"));
	    return;
	}

	Parent->memoserv.stats.i_Del++;
	if (what.CmpNoCase("all")==0)
	{
	    list<Memo_t>::iterator iter;
	    mstring output;
	    for (iter = Parent->memoserv.nick[who.LowerCase()].begin();
		    iter != Parent->memoserv.nick[who.LowerCase()].end();)
	    {
		Parent->memoserv.nick[who.LowerCase()].erase(iter);
		iter = Parent->memoserv.nick[who.LowerCase()].begin();
	    }
	    Parent->memoserv.nick.erase(who.LowerCase());
	    ::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/NS_DEL_ALL"));
	}
	else
	{
	    unsigned int i;
	    /* Adjust is to accomodate for memos that have
	     * been deleted before the current one, ie. if
	     * 1, 3, 5 and 7 are being deleted, once 1 has
	     * been deleted, adjust becomes 1, meaning that
	     * 3 has become 2, 5 has become 4, etc.  Once 3
	     * has been deleted, adjust becomes 2, meaning
	     * that 5 has become 3 and 7 has become 5.
	     */
	    int j=1, adjust = 0;
	    vector<int> numbers1 = ParseNumbers(what);
	    set<int> numbers;
	    set<int>::iterator ni;
	    for (i=0; i<numbers1.size(); i++)
		numbers.insert(numbers1[i]);
	    bool triedabove = false, nonnumeric = false;
	    list<Memo_t>::iterator iter = Parent->memoserv.nick[who.LowerCase()].begin();
	    mstring output;
	    for (ni = numbers.begin(); ni != numbers.end(); ni++)
	    {
		if (*ni - adjust <= 0)
		    nonnumeric = true;
		else if (*ni - adjust > Parent->memoserv.nick[who.LowerCase()].size())
		    triedabove = true;
		else
		{
		    while (*ni - adjust > j &&
			iter != Parent->memoserv.nick[who.LowerCase()].end())
		    {
			j++;
			iter++;
		    }
		    if (iter != Parent->memoserv.nick[who.LowerCase()].end())
		    {
			if (output != "")
			    output << ", ";
			output << j;
			Parent->memoserv.nick[who.LowerCase()].erase(iter);
			iter = Parent->memoserv.nick[who.LowerCase()].begin();
			j=1;
			adjust++;
		    }
		}
	    }
	    if (Parent->memoserv.nick[who.LowerCase()].size() == 0)
		Parent->memoserv.nick.erase(who.LowerCase());
	    if (output != "")
	    {
		::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/NS_DEL"),
			    output.c_str());
		output = "";
	    }
	    if (nonnumeric)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NONNUMERIC"));
	    if (triedabove)
		::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/ABOVELIMIT"),
			Parent->memoserv.channel[who.LowerCase()].size());
	}
    }
}


void MemoServ::do_Continue(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Continue", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring text = params.After(" ", 1);

    if (text.size() > 450)
    {
	text.Truncate(450);
	::send(mynick, source, Parent->getMessage(source, "MS_STATUS/TRUNCATE"),
		    text.SubString(430, 450).c_str(), mynick.c_str());
    }

    if (Parent->nickserv.live[source.LowerCase()].InFlight.Memo())
    {
	Parent->memoserv.stats.i_Continue++;
	Parent->nickserv.live[source.LowerCase()].InFlight.Continue(text);
    }
    else
    {
	::send(mynick, source, Parent->getMessage(source, "MS_STATUS/NOPENDING"));
    }
}


void MemoServ::do_File(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_File", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SYNTAX/NEED_PARAMS"),
				message.c_str(), mynick.c_str(), message.c_str());
	return;
    }

    mstring name = params.ExtractWord(2, " ");
    mstring text = params.After(" ", 2);

    if (IsChan(name))
    {
	::send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/NOATTACH"));
	return;
    }
    else
    {
	if (!Parent->nickserv.IsStored(name))
	{
	    ::send(mynick, source, Parent->getMessage(source, "NS_OTH_STATUS/ISNOTSTORED"),
			name.c_str());
	    return;
	}
	name = Parent->getSname(name);
	mstring target = name;
	if (Parent->nickserv.stored[name.LowerCase()].Host() != "")
	    target = Parent->getSname(Parent->nickserv.stored[name.LowerCase()].Host());

	if (Parent->memoserv.Files() > 0)
	{
	    if (Parent->memoserv.IsNick(target))
	    {
		unsigned int count = 0;
		list<Memo_t>::iterator iter;
		for (iter=Parent->memoserv.nick[target.LowerCase()].begin();
			iter!=Parent->memoserv.nick[target.LowerCase()].end();
			iter++)
		{
		    if (iter->File())
			count++;
		}
		if (count >= Parent->memoserv.Files())
		{
		    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/ATTACHLIMIT"),
					name.c_str());
		    return;
		}
	    }
	}
	else
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/FILEDISABLED"));
	    return;
	}

	if (Parent->nickserv.stored[name.LowerCase()].IsIgnore(source) ?
		!Parent->nickserv.stored[name.LowerCase()].NoMemo() :
		Parent->nickserv.stored[name.LowerCase()].NoMemo())
	{
	    ::send(mynick, source, Parent->getMessage(source, "MS_STATUS/IGNORE"),
			    name.c_str());
	    return;
	}
    }

    if (text.size() > 450)
    {
	text.Truncate(450);
	::send(mynick, source, Parent->getMessage(source, "MS_STATUS/TRUNCATE"),
		    text.SubString(430, 450).c_str(), mynick.c_str());
    }

    Parent->memoserv.stats.i_File++;
    Parent->nickserv.live[source.LowerCase()].InFlight.Memo(
					    true, mynick, name, text);
}


SXP::Tag Memo_t::tag_Memo_t("Memo_t");
SXP::Tag Memo_t::tag_Nick("Nick");
SXP::Tag Memo_t::tag_Sender("Sender");
SXP::Tag Memo_t::tag_Time("Time");
SXP::Tag Memo_t::tag_Text("Text");
SXP::Tag Memo_t::tag_Read("Read");
SXP::Tag Memo_t::tag_File("File");
SXP::Tag Memo_t::tag_UserDef("UserDef");
SXP::Tag News_t::tag_News_t("News_t");
SXP::Tag News_t::tag_Channel("Channel");
SXP::Tag News_t::tag_Sender("Sender");
SXP::Tag News_t::tag_Time("Time");
SXP::Tag News_t::tag_Text("Text");
SXP::Tag News_t::tag_Read("Read");
SXP::Tag News_t::tag_UserDef("UserDef");

void Memo_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("Memo_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
	if( pElement->IsA(tag_Nick) )		pElement->Retrieve(i_Nick);
	if( pElement->IsA(tag_Sender) )		pElement->Retrieve(i_Sender);
	if( pElement->IsA(tag_Time) )		pElement->Retrieve(i_Time);
	if( pElement->IsA(tag_Text) )		pElement->Retrieve(i_Text);
	if( pElement->IsA(tag_Read) )		pElement->Retrieve(i_Read);
	if( pElement->IsA(tag_File) )		pElement->Retrieve(i_File);

    if( pElement->IsA(tag_UserDef) )
    {
        mstring tmp;
        pElement->Retrieve(tmp);
        i_UserDef[tmp.Before("\n")]=tmp.After("\n");
    }
}

void Memo_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    //TODO: Add your source code here
	pOut->BeginObject(tag_Memo_t, attribs);

	pOut->WriteElement(tag_Nick, i_Nick);
	pOut->WriteElement(tag_Sender, i_Sender);
	pOut->WriteElement(tag_Time, i_Time);
	pOut->WriteElement(tag_Text, i_Text);
	pOut->WriteElement(tag_Read, i_Read);
	pOut->WriteElement(tag_File, i_File);

        map<mstring,mstring>::const_iterator iter;
        for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
        {
            pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
        }
	pOut->EndObject(tag_Memo_t);
}

void News_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("News_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
	if( pElement->IsA(tag_Channel) )	pElement->Retrieve(i_Channel);
	if( pElement->IsA(tag_Sender) )		pElement->Retrieve(i_Sender);
	if( pElement->IsA(tag_Time) )		pElement->Retrieve(i_Time);
	if( pElement->IsA(tag_Text) )		pElement->Retrieve(i_Text);

    if( pElement->IsA(tag_Read) )
    {
	mstring tmp;
	pElement->Retrieve(tmp);
	i_Read.insert(tmp);
    }

    if( pElement->IsA(tag_UserDef) )
    {
        mstring tmp;
        pElement->Retrieve(tmp);
        i_UserDef[tmp.Before("\n")]=tmp.After("\n");
    }
}

void News_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    //TODO: Add your source code here
	pOut->BeginObject(tag_News_t, attribs);

	pOut->WriteElement(tag_Channel, i_Channel);
	pOut->WriteElement(tag_Sender, i_Sender);
	pOut->WriteElement(tag_Time, i_Time);
	pOut->WriteElement(tag_Text, i_Text);

	set<mstring>::iterator iter2;
	for (iter2=i_Read.begin(); iter2!=i_Read.end(); iter2++)
	{
		pOut->WriteElement(tag_Read, *iter2);
	}

        map<mstring,mstring>::const_iterator iter;
        for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
        {
            pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
        }
	pOut->EndObject(tag_News_t);
}

SXP::Tag MemoServ::tag_MemoServ("MemoServ");

void MemoServ::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("MemoServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Memo_t d1;
    if( pElement->IsA( d1.GetClassTag() ) )
    {
	pIn->ReadTo(&d1);
	if (d1.Nick() != "")
	    nick[d1.Nick().LowerCase()].push_back(d1);
    }

    News_t d2;
    if( pElement->IsA( d2.GetClassTag() ) )
    {
	pIn->ReadTo(&d2);
	if (d2.Channel() != "")
	    channel[d2.Channel().LowerCase()].push_back(d2);
    }
}

void MemoServ::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("MemoServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void MemoServ::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("MemoServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_MemoServ, attribs);

    map<mstring, list<Memo_t> >::iterator i1;
    list<Memo_t>::iterator i2;
    map<mstring, list<News_t> >::iterator j1;
    list<News_t>::iterator j2;

    for (i1 = nick.begin(); i1 != nick.end(); i1++)
	for (i2=i1->second.begin(); i2!=i1->second.end(); i2++)
	{
	    pOut->WriteSubElement(&(*i2), attribs);
	}

    for (j1 = channel.begin(); j1 != channel.end(); j1++)
	for (j2=j1->second.begin(); j2!=j1->second.end(); j2++)
	{
	    pOut->WriteSubElement(&(*j2), attribs);
	}

    pOut->EndObject(tag_MemoServ);
}


void MemoServ::PostLoad()
{
    NFT("MemoServ::PostLoad");
    // Linkage, etc
}
