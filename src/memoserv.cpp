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
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	    ::send(mynick, source, "Not enough paramaters");
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
	    ::send(mynick, source, "No such channel " + who + ".");
	    return;
	}

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "READMEMO"))
	{
	    ::send(mynick, source, "Access denied.");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, "Channel " + who + " has no memos.");
	    return;
	}

	if (what.CmpNoCase("all")==0)
	{
	    list<News_t>::iterator iter;
	    int i = 1;
	    mstring output;
	    for (iter = Parent->memoserv.channel[who.LowerCase()].begin();
		    iter != Parent->memoserv.channel[who.LowerCase()].end(); iter++)
	    {
		iter->Read(whoami);
		output << "#" << i << " from " << iter->Sender() << " to "
		    << iter->Channel() << " sent " << iter->Time().Ago() << " ago.";
		::send(mynick, source, output);
		output = "";
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
	    int i, j=1;
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
			iter->Read(whoami);
			output << "#" << i << " from " << iter->Sender() << " to "
			    << iter->Channel() << " sent " << iter->Time().Ago() << " ago.";
			::send(mynick, source, output);
			output = "";
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
		output << "Non-numeric arguments specified, ignored.";
	    if (triedabove)
		output << "All entries above " <<
			Parent->memoserv.channel[who.LowerCase()].size() <<
			" ignored.";
	    if (output != "")
		::send(mynick, source, output);
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
	    ::send(mynick, source, "You have no memos.");
	    return;
	}

	if (what.CmpNoCase("all")==0)
	{
	    list<Memo_t>::iterator iter;
	    int i = 1;
	    mstring output;
	    for (iter = Parent->memoserv.nick[who.LowerCase()].begin();
		    iter != Parent->memoserv.nick[who.LowerCase()].end(); iter++)
	    {
		iter->Read();
		output << "#" << i << " from " << iter->Sender() <<
		    " sent " << iter->Time().Ago();
		::send(mynick, source, output);
		output = "";
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
	    int i, j=1;
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
			output << "#" << j << " from " << iter->Sender() <<
			    " sent " << iter->Time().Ago();
			::send(mynick, source, output);
			output = "";
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
		output << "Non-numeric arguments specified, ignored.";
	    if (triedabove)
		output << "All entries above " <<
			Parent->memoserv.nick[who.LowerCase()].size() <<
			" ignored.";
	    if (output != "")
		::send(mynick, source, output);
	}
    }
}


void MemoServ::do_UnRead(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_UnRead", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	    ::send(mynick, source, "Not enough paramaters");
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
	    ::send(mynick, source, "No such channel " + who + ".");
	    return;
	}

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "READMEMO"))
	{
	    ::send(mynick, source, "Access denied.");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, "Channel " + who + " has no memos.");
	    return;
	}

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
	    ::send(mynick, source, "All news articles for channel " +
				    who + " have been marked unread.");
	}
	else
	{
	    vector<int> numbers = ParseNumbers(what);
	    int i, j=1;
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
		::send(mynick, source, "News articles for channel " +
					    who + " (" + output +
					    ") have been marked unread.");
		output = "";
	    }
	    if (nonnumeric)
		output << "Non-numeric arguments specified, ignored.";
	    if (triedabove)
		output << "All entries above " <<
			Parent->memoserv.channel[who.LowerCase()].size() <<
			" ignored.";
	    if (output != "")
		::send(mynick, source, output);
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
	    ::send(mynick, source, "You have no memos.");
	    return;
	}

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
	    ::send(mynick, source, "All your memos have been marked unread.");
	}
	else
	{
	    vector<int> numbers = ParseNumbers(what);
	    int i, j=1;
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
		::send(mynick, source, "Your memos (" + output +
					    ") have been marked unread.");
		output = "";
	    }
	    if (nonnumeric)
		output << "Non-numeric arguments specified, ignored.";
	    if (triedabove)
		output << "All entries above " <<
			Parent->memoserv.nick[who.LowerCase()].size() <<
			" ignored.";
	    if (output != "")
		::send(mynick, source, output);
	}
    }
}


void MemoServ::do_List(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_List", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 1)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 2)
	{
	    ::send(mynick, source, "Not enough paramaters");
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
	    ::send(mynick, source, "No such channel " + who + ".");
	    return;
	}

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "READMEMO"))
	{
	    ::send(mynick, source, "Access denied.");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, "Channel " + who + " has no memos.");
	    return;
	}

	list<News_t>::iterator iter;
	int i = 1;
	mstring output;
	for (iter = Parent->memoserv.channel[who.LowerCase()].begin();
		iter != Parent->memoserv.channel[who.LowerCase()].end(); iter++)
	{
	    if (iter->IsRead(whoami))
		output << " ";
	    else
		output << "*";
	    output << "#" << i << " from " << iter->Sender() << " to "
		<< iter->Channel() << " sent " << iter->Time().Ago() << " ago - \"";
	    if (iter->Text().size() > 20)
		output << iter->Text().SubString(0, 19) << "...\"";
	    else
		output << iter->Text().SubString(0, iter->Text().size()-1) << "\"";
	    ::send(mynick, source, output);
	    output = "";
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
	    ::send(mynick, source, "You have no memos.");
	    return;
	}

	list<Memo_t>::iterator iter;
	int i = 1;
	mstring output;
	for (iter = Parent->memoserv.nick[who.LowerCase()].begin();
		iter != Parent->memoserv.nick[who.LowerCase()].end(); iter++)
	{
	    if (iter->IsRead())
		output << " ";
	    else
		output << "*";
	    output << "#" << i << " from " << iter->Sender() <<
		" sent " << iter->Time().Ago() << " ago - \"";
	    if (iter->Text().size() > 20)
		output << iter->Text().SubString(0, 19) << "...\"";
	    else
		output << iter->Text().SubString(0, iter->Text().size()-1) << "\"";
	    ::send(mynick, source, output);
	    output = "";
	}
    }
}


void MemoServ::do_Send(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Send", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring name = params.ExtractWord(2, " ");
    mstring text = params.After(" ", 2);

    if (IsChan(name))
    {
	if (!Parent->chanserv.IsStored(name))
	{
	    ::send(mynick, source, "Channel " + name + " is not registered.");
	    return;
	}

	Chan_Stored_t *chan = &Parent->chanserv.stored[name.LowerCase()];
	if (!chan->GetAccess(source, "WRITEMEMO"))
	{
	    ::send(mynick, source, "Access denied.");
	    return;
	}
    }
    else if (!Parent->nickserv.IsStored(name))
    {
	::send(mynick, source, "Nickname " + name + " is not registered.");
	return;
    }

    if (text.size() > 450)
    {
	text.Truncate(450);
	::send(mynick, source, "Message truncated.  Last characters of message were \"" +
				text.SubString(430, 450) + "\".");
	::send(mynick, source, "Use /MSG " + mynick +
			    " CONTINUE <message>    to extend message.");
    }

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
	    ::send(mynick, source, "Memos with file attachments cannot be flushed.");
	    return;
	}
	Parent->nickserv.live[source.LowerCase()].InFlight.End(0);
    }
    else
    {
	::send(mynick, source, "No memos are pending to be sent.");
    }
}

void MemoServ::do_Forward(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Forward", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 4)
	{
	    ::send(mynick, source, "Not enough paramaters");
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
	    ::send(mynick, source, "No such channel " + who + ".");
	    return;
	}

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "READ"))
	{
	    ::send(mynick, source, "Access denied.");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, "Channel " + who + " has no memos.");
	    return;
	}

	if (!what.IsNumber() || what.Contains(".") ||
	    atoi(what.c_str()) <= 0)
	{
	    ::send(mynick, source, "You must specify a posetive whole number.");
	    return;
	}

	if (atoi(what.c_str()) > Parent->memoserv.channel[who.LowerCase()].size())
	{
	    ::send(mynick, source, "News Article #" + what +
				    " does not exist for channel " + who);
	    return;
	}

	int i;
	list<News_t>::iterator iter = Parent->memoserv.channel[who.LowerCase()].begin();
	for (i=1; i < atoi(what.c_str()); iter++, i++) ;
	mstring output = "";
	output << "[" << IRC_Bold << "FORWARD" << IRC_Off << "(" <<
		Parent->chanserv.stored[who.LowerCase()].Name() <<
		"): " << iter->Sender() << "]" << iter->Text();

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
	    ::send(mynick, source, "You have no memos.");
	    return;
	}

	if (!what.IsNumber() || what.Contains(".") ||
	    atoi(what.c_str()) <= 0)
	{
	    ::send(mynick, source, "You must specify a posetive whole number.");
	    return;
	}

	if (atoi(what.c_str()) > Parent->memoserv.nick[who.LowerCase()].size())
	{
	    ::send(mynick, source, "Memo #" + what + " does not exist.");
	    return;
	}

	int i;
	list<Memo_t>::iterator iter = Parent->memoserv.nick[who.LowerCase()].begin();
	for (i=1; i < atoi(what.c_str()); iter++, i++) ;

	mstring output = "";
	output << "[" << IRC_Bold << "FORWARD" << IRC_Off;
	if (iter->File())
	    output << "(" << "filename" << ")";
	output << ": " << iter->Sender() << "] " << iter->Text();

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
	    ::send(mynick, source, "Channel " + dest + " is not registered.");
	    return;
	}

	Chan_Stored_t *chan = &Parent->chanserv.stored[dest.LowerCase()];
	if (!chan->GetAccess(source, "WRITEMEMO"))
	{
	    ::send(mynick, source, "Access denied.");
	    return;
	}
    }
    else if (!Parent->nickserv.IsStored(dest))
    {
	::send(mynick, source, "Nickname " + dest + " is not registered.");
	return;
    }

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
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 4)
	{
	    ::send(mynick, source, "Not enough paramaters");
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
	    ::send(mynick, source, "No such channel " + who + ".");
	    return;
	}

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "WRITEMEMO"))
	{
	    ::send(mynick, source, "Access denied.");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, "Channel " + who + " has no memos.");
	    return;
	}

	if (!what.IsNumber() || what.Contains(".") ||
	    atoi(what.c_str()) <= 0)
	{
	    ::send(mynick, source, "You must specify a posetive whole number.");
	    return;
	}

	if (atoi(what.c_str()) > Parent->memoserv.channel[who.LowerCase()].size())
	{
	    ::send(mynick, source, "News Article #" + what +
				    " does not exist for channel " + who);
	    return;
	}

	int i;
	list<News_t>::iterator iter = Parent->memoserv.channel[who.LowerCase()].begin();
	for (i=1; i < atoi(what.c_str()); iter++, i++) ;
	mstring output = "";
	output << "[" << IRC_Bold << "REPLY" << IRC_Off << "(" <<
		iter->Sender() << "): \"";
	if (iter->Text().size() < 21)
	    output << iter->Text().SubString(0, 19) << "...";
	else
	    output << iter->Text().SubString(0, iter->Text().size());
	output << "\"] " << text;

	Parent->nickserv.live[source.LowerCase()].InFlight.Memo(
					    false, mynick, who, text);
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
	    ::send(mynick, source, "You have no memos.");
	    return;
	}

	if (!what.IsNumber() || what.Contains(".") ||
	    atoi(what.c_str()) <= 0)
	{
	    ::send(mynick, source, "You must specify a posetive whole number.");
	    return;
	}

	if (atoi(what.c_str()) > Parent->memoserv.nick[who.LowerCase()].size())
	{
	    ::send(mynick, source, "Memo #" + what + " does not exist.");
	    return;
	}

	int i;
	list<Memo_t>::iterator iter = Parent->memoserv.nick[who.LowerCase()].begin();
	for (i=1; i < atoi(what.c_str()); iter++, i++) ;

	if (!Parent->nickserv.IsStored(iter->Sender()))
	{
	    ::send(mynick, source, "Nickname " + iter->Sender() +
					    " is no longer registered.");
	    return;
	}

	mstring output = "";
	output << "[" << IRC_Bold << "REPLY" << IRC_Off;
	if (iter->File())
	    output << "(" << "filename" << ")";
	output << ": \"";
	if (iter->Text().size() < 21)
	    output << iter->Text().SubString(0, 19) << "...";
	else
	    output << iter->Text().SubString(0, iter->Text().size());
	output << "\"] " << text;
	Parent->nickserv.live[source.LowerCase()].InFlight.Memo(
					    false, mynick, who, text);
    }
}


void MemoServ::do_Cancel(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Cancel", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->nickserv.live[source.LowerCase()].InFlight.IsMemo())
    {
	Parent->nickserv.live[source.LowerCase()].InFlight.Cancel();
    }
    else
    {
	::send(mynick, source, "No memos are pending to be sent.");
    }
}


void MemoServ::do_Del(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Del", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	    ::send(mynick, source, "Not enough paramaters");
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
	    ::send(mynick, source, "No such channel " + who + ".");
	    return;
	}

	if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "WRITEMEMO"))
	{
	    ::send(mynick, source, "Access denied.");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    ::send(mynick, source, "Channel " + who + " has no memos.");
	    return;
	}

	if (what.CmpNoCase("all")==0)
	{
	    if (!Parent->chanserv.stored[who.LowerCase()].GetAccess(whoami, "DELMEMO"))
	    {
		::send(mynick, source, "Access denied.");
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
	    ::send(mynick, source, "All news articles for channel " +
				    who + " have been deleted.");
	}
	else
	{
	    int i, j=1, adjust=0;
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
		::send(mynick, source, "News articles for channel " +
					    who + " (" + denied +
					    ") have NOT been deleted.");
		denied = "";
	    }
	    if (output != "")
	    {
		::send(mynick, source, "News articles for channel " +
					    who + " (" + output +
					    ") have been deleted.");
		output = "";
	    }
	    if (nonnumeric)
		output << "Non-numeric arguments specified, ignored.";
	    if (triedabove)
		output << "All entries above " <<
			Parent->memoserv.channel[who.LowerCase()].size() <<
			" ignored.";
	    if (output != "")
		::send(mynick, source, output);
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
	    ::send(mynick, source, "You have no memos.");
	    return;
	}

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
	    ::send(mynick, source, "All your memos have been deleted.");
	}
	else
	{
	    int i, j=1, adjust = 0;
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
		::send(mynick, source, "Your memos (" + output +
					    ") have been deleted.");
		output = "";
	    }
	    if (nonnumeric)
		output << "Non-numeric arguments specified, ignored.";
	    if (triedabove)
		output << "All entries above " <<
			Parent->memoserv.nick[who.LowerCase()].size() <<
			" ignored.";
	    if (output != "")
		::send(mynick, source, output);
	}
    }
}


void MemoServ::do_Continue(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_Continue", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring text = params.After(" ", 1);

    if (text.size() > 450)
    {
	text.Truncate(450);
	::send(mynick, source, "Message truncated.  Last characters of message were \"" +
			    text.SubString(430, 450) + "\".");
	::send(mynick, source, "Use /MSG " + mynick +
			    " CONTINUE <message>    to extend message.");
    }

    if (Parent->nickserv.live[source.LowerCase()].InFlight.IsMemo())
    {
	Parent->nickserv.live[source.LowerCase()].InFlight.Continue(text);
    }
    else
    {
	::send(mynick, source, "No memos are pending to be sent.");
    }
}


void MemoServ::do_File(mstring mynick, mstring source, mstring params)
{
    FT("MemoServ::do_File", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 3)
    {
	::send(mynick, source, "Not enough paramaters");
	return;
    }

    mstring name = params.ExtractWord(2, " ");
    mstring text = params.After(" ", 2);

    if (IsChan(name))
    {
	::send(mynick, source, "Cannot send file attachments to channels!");
	return;
    }
    else if (!Parent->nickserv.IsStored(name))
    {
	::send(mynick, source, "Nickname " + name + " is not registered.");
	return;
    }

    if (text.size() > 450)
    {
	text.Truncate(450);
	::send(mynick, source, "Message truncated.  Last characters of message were \"" +
				text.SubString(430, 450) + "\".");
	::send(mynick, source, "Use /MSG " + mynick +
			    " CONTINUE <message>    to extend message.");
    }

    Parent->nickserv.live[source.LowerCase()].InFlight.Memo(
					    true, mynick, name, text);
}


void MemoServ::load_database(wxInputStream& in)
{
    map<mstring,list<Memo_t> >::size_type i,count, i2, count2;
    in>>count;
    nick.clear();
    for(i=0;i<count;i++)
    {
	mstring temp;
	Memo_t tmpmemo;
	in>>temp>>count2;
	for(i2=0;i2<count2;i2++)
	{
	    in>>tmpmemo;
	    nick[temp].push_back(tmpmemo);
	}
    }
    in>>count;
    channel.clear();
    for(i=0;i<count;count++)
    {
	mstring temp;
	News_t tmpnews;
	in>>temp>>count2;
	for(i2=0;i2<count2;i2++)
	{
	    in>>tmpnews;
	    channel[temp].push_back(tmpnews);
	}
    }
}

void MemoServ::save_database(wxOutputStream& out)
{
    out<<nick.size();
    map<mstring,list<Memo_t> >::iterator i;
    for(i=nick.begin();i!=nick.end();i++)
    {
	out<<i->first;
	out<<i->second.size();
	list<Memo_t>::iterator j;
	for(j=i->second.begin();j!=i->second.end();j++)
	    out<<(*j);
    }
    out<<channel.size();
    map<mstring,list<News_t> >::iterator i2;
    for(i2=channel.begin();i2!=channel.end();i2++)
    {
	out<<i2->first;
	out<<i2->second.size();
	list<News_t>::iterator j2;
	for(j2=i2->second.begin();j2!=i2->second.end();j2++)
	    out<<(*j2);
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
