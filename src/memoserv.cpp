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
    RET(i_Read.find(name.LowerCase())!=i_Read.end());
}


void News_t::Read(mstring name)
{
    FT("News_t::Read", (name));
    i_Read.insert(name.LowerCase());
}


void News_t::Unread(mstring name)
{
    FT("News_t::Unread", (name));
    i_Read.erase(name.LowerCase());
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

//  Parent->commands.RemSystemCommand(GetInternalName(),
//		    "TRACE", "ALL");
}

bool MemoServ::IsNick(mstring in)
{
    FT("MemoServ::IsNick", (in));
    RET(nick.find(in.LowerCase())!=nick.end());
}

bool MemoServ::IsChannel(mstring in)
{
    FT("MemoServ::IsChannel", (in));
    RET(channel.find(in.LowerCase())!=channel.end());
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
		unsigned int sentsize = 0;
		mstring output;
		for (sentsize=0; sentsize < iter->Text().size(); sentsize+=450)
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
	    bool displayed = false, triedabove = false, nonnumeric = false;
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
				i, iter->Sender().c_str(), iter->Sender().c_str(),
				iter->Time().Ago().c_str());
			unsigned int sentsize = 0;
			for (sentsize=0; sentsize < iter->Text().size(); sentsize+=450)
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
			displayed = true;
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
		::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/MEMO"),
			i++, iter->Sender().c_str(),
			iter->Time().Ago().c_str());
		unsigned int sentsize = 0;
		mstring output;
		for (sentsize=0; sentsize < iter->Text().size(); sentsize+=450)
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
	    bool displayed = false, triedabove = false, nonnumeric = false;
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
			::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/MEMO"),
				i++, iter->Sender().c_str(),
				iter->Time().Ago().c_str());
			unsigned int sentsize = 0;
			for (sentsize=0; sentsize < iter->Text().size(); sentsize+=450)
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
			displayed = true;
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
	    int i = 1;
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
	    bool displayed = false, triedabove = false, nonnumeric = false;
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
			displayed = true;
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
	    int i = 1;
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
	    bool displayed = false, triedabove = false, nonnumeric = false;
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
			displayed = true;
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
		output << iter->Text().SubString(0, 19) << "...\"";
	    else
		output << iter->Text().SubString(0, iter->Text().size()-1) << "\"";

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
		output << iter->Text().SubString(0, 19) << "...\"";
	    else
		output << iter->Text().SubString(0, iter->Text().size()-1) << "\"";

	    ::send(mynick, source, Parent->getMessage(source, "MS_COMMAND/MEMO_LIST"),
		    iter->IsRead() ? ' ' : '*',
		    i++, iter->Sender().c_str(),
		    iter->Time().Ago().c_str(), output.c_str());
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

	bool ignored = Parent->nickserv.stored[name.LowerCase()].IsIgnore(source);
	if ((ignored && !Parent->nickserv.stored[name.LowerCase()].NoMemo()) ||
	    (!ignored && Parent->nickserv.stored[name.LowerCase()].NoMemo()))
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

    if (Parent->nickserv.live[source.LowerCase()].InFlight.IsMemo())
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

	unsigned int num = atoi(what.c_str());
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

	unsigned int num = atoi(what.c_str());
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

	bool ignored = Parent->nickserv.stored[dest.LowerCase()].IsIgnore(source);
	if ((ignored && !Parent->nickserv.stored[dest.LowerCase()].NoMemo()) ||
	    (!ignored && Parent->nickserv.stored[dest.LowerCase()].NoMemo()))
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

	unsigned int num = atoi(what.c_str());
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

	unsigned int num = atoi(what.c_str());
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

	bool ignored = Parent->nickserv.stored[iter->Sender().LowerCase()].IsIgnore(source);
	if ((ignored && !Parent->nickserv.stored[iter->Sender().LowerCase()].NoMemo()) ||
	    (!ignored && Parent->nickserv.stored[iter->Sender().LowerCase()].NoMemo()))
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

    if (Parent->nickserv.live[source.LowerCase()].InFlight.IsMemo())
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
	    int i = 1;
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
	    int j=1, adjust=0;
	    vector<int> numbers1 = ParseNumbers(what);
	    set<int> numbers;
	    set<int>::iterator ni;
	    for (i=0; i<numbers1.size(); i++)
		numbers.insert(numbers1[i]);

	    bool displayed = false, triedabove = false, nonnumeric = false;
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
			    displayed = true;
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
	    int i = 1;
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
	    int j=1, adjust = 0;
	    vector<int> numbers1 = ParseNumbers(what);
	    set<int> numbers;
	    set<int>::iterator ni;
	    for (i=0; i<numbers1.size(); i++)
		numbers.insert(numbers1[i]);
	    bool displayed = false, triedabove = false, nonnumeric = false;
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
			displayed = true;
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

    if (Parent->nickserv.live[source.LowerCase()].InFlight.IsMemo())
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

	bool ignored = Parent->nickserv.stored[name.LowerCase()].IsIgnore(source);
	if ((ignored && !Parent->nickserv.stored[name.LowerCase()].NoMemo()) ||
	    (!ignored && Parent->nickserv.stored[name.LowerCase()].NoMemo()))
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


void MemoServ::load_database(wxInputStream& in)
{
    FT("MemoServ::load_database", ("(wxInputStream &) in"));
    map<mstring,list<Memo_t> >::size_type i,count, i2, count2;
    in>>count;
    CP(("Loading MEMO LIST entries (%d) ...", count));
    nick.clear();
    for(i=0;i<count;i++)
    {
	COM(("Loading MEMO LIST entry %d ...", i));
	mstring temp;
	Memo_t tmpmemo;
	in>>temp>>count2;
	CP(("Loading MEMO entries (%d) ...", count2));
	for(i2=0;i2<count2;i2++)
	{
	    COM(("Loading MEMO entry %d ...", i2));
	    in>>tmpmemo;
	    nick[temp].push_back(tmpmemo);
	    COM(("Entry MEMO %d loaded ...", i2));
	}
	COM(("Entry MEMO LIST %s loaded ...", temp.c_str()));
    }
    in>>count;
    CP(("Loading NEWS LIST entries (%d) ...", count));
    channel.clear();
    for(i=0;i<count;i++)
    {
	COM(("Loading NEWS LIST entry %d ...", i));
	mstring temp;
	News_t tmpnews;
	in>>temp>>count2;
	CP(("Loading NEWS entries (%d) ...", count2));
	for(i2=0;i2<count2;i2++)
	{
	    COM(("Loading NEWS entry %d ...", i2));
	    in>>tmpnews;
	    channel[temp].push_back(tmpnews);
	    COM(("Entry NEWS %d loaded ...", i2));
	}
	COM(("Entry NEWS LIST %s loaded ...", temp.c_str()));
    }
}

void MemoServ::save_database(wxOutputStream& out)
{
    FT("MemoServ::save_database", ("(wxOutputStream &) out"));
    CP(("Saving MEMO LIST entries (%d) ...", nick.size()));
    out<<nick.size();
    map<mstring,list<Memo_t> >::iterator i;
    int cntr;
    for(i=nick.begin();i!=nick.end();i++)
    {
	out<<i->first;
	CP(("Saving MEMO entries (%d) ...", i->second.size()));
	out<<i->second.size();
	list<Memo_t>::iterator j;
	for(cntr=0, j=i->second.begin();j!=i->second.end();cntr++, j++)
	{
	    out<<(*j);
	    COM(("Entry MEMO %d saved ...", cntr));
	}
	COM(("Entry MEMO LIST %s saved ...", i->first.c_str()));
    }
    CP(("Saving NEWS LIST entries (%d) ...", channel.size()));
    out<<channel.size();
    map<mstring,list<News_t> >::iterator i2;
    for(i2=channel.begin();i2!=channel.end();i2++)
    {
	out<<i2->first;
	CP(("Saving NEWS entries (%d) ...", i2->second.size()));
	out<<i2->second.size();
	list<News_t>::iterator j2;
	for(cntr=0, j2=i2->second.begin();j2!=i2->second.end();cntr++, j2++)
	{
	    out<<(*j2);
	    COM(("Entry NEWS %d saved ...", cntr));
	}
	COM(("Entry NEWS LIST %s saved ...", i2->first.c_str()));
    }
}

wxOutputStream &operator<<(wxOutputStream& out,Memo_t& in)
{
    out<<in.i_Nick<<in.i_Sender<<in.i_Time<<in.i_Text<<in.i_Read<<in.i_File;
    return out;
}
wxInputStream &operator>>(wxInputStream& in, Memo_t& out)
{
    in>>out.i_Nick>>out.i_Sender>>out.i_Time>>out.i_Text>>out.i_Read>>out.i_File;
    return in;
}
wxOutputStream &operator<<(wxOutputStream& out,News_t& in)
{
    out<<in.i_Channel<<in.i_Sender<<in.i_Time<<in.i_Text;
    out<<in.i_Read.size();
    
    set<mstring>::iterator i;
    for(i=in.i_Read.begin();i!=in.i_Read.end();i++)
	out<<(*i);

    return out;
}
wxInputStream &operator>>(wxInputStream& in, News_t& out)
{
    in>>out.i_Channel>>out.i_Sender>>out.i_Time>>out.i_Text;
    out.i_Read.clear();
    set<mstring>::size_type i,j;
    in>>i;
    for(j=0;j<i;j++)
    {
	mstring temp;
	in>>temp;
	out.i_Read.insert(temp);
    }

    return in;
}
