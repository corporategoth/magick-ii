#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#define RCSID(x,y) const char *rcsid_dccengine_cpp_ ## x () { return y; }
RCSID(dccengine_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.40  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.39  2001/02/03 02:21:33  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.38  2001/01/15 23:31:38  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.37  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.36  2000/12/29 15:31:55  prez
** Added locking/checking for dcc/events threads.  Also for ACE_Log_Msg
**
** Revision 1.35  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.34  2000/12/11 11:19:46  prez
** Use Occurance not occurance (because occurance could later be private)
**
** Revision 1.33  2000/10/10 11:47:51  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.32  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.31  2000/06/08 13:07:34  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.30  2000/05/27 15:10:12  prez
** Misc changes, mainly re-did the makefile system, makes more sense.
** Also added a config.h file.
**
** Revision 1.29  2000/05/22 13:00:09  prez
** Updated version.h and some other stuff
**
** Revision 1.28  2000/05/17 14:08:11  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.27  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.26  2000/05/03 14:12:22  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.25  2000/04/03 09:45:23  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.24  2000/03/24 12:53:04  prez
** FileSystem Logging
**
** Revision 1.23  2000/03/23 10:22:24  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.22  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.21  2000/02/23 14:29:05  prez
** Added beginnings of a File Map for stored files.
** Also updated Help files (finished nickserv).
**
** Revision 1.20  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.19  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.18  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.17  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"
#include "dccengine.h"

mstring DccEngine::lowQuote(mstring& in)
{
    FT("DccEngine::lowQuote",((in)));
    mstring Result;
    for(unsigned int i=0;i<in.length();i++)
    {
	if(in[i]=='\0')
	    Result<<CTCP_MQUOTE_CHAR<<'0';
	else if(in[i]=='\n')
	    Result<<CTCP_MQUOTE_CHAR<<'n';
	else if(in[i]=='\r')
	    Result<<CTCP_MQUOTE_CHAR<<'r';
	else if(in[i]==CTCP_MQUOTE_CHAR)
	    Result<<CTCP_MQUOTE_CHAR<<CTCP_MQUOTE_CHAR;
	else
	    Result<<in[i];
    }
    RET(Result);
}

mstring DccEngine::lowDequote(mstring& in)
{
    FT("DccEngine::lowDequote",((in)));
    mstring Result;
    for(unsigned int i=0;i<in.length();i++)
    {
	if(in[i]==CTCP_MQUOTE_CHAR && i+1<in.length())
	{
	    i++;
	    if(in[i]=='0')
		Result<<'\0';
	    else if(in[i]=='n')
		Result<<'\n';
	    else if(in[i]=='r')
		Result<<'\r';
	    else if(in[i]==CTCP_MQUOTE_CHAR)
		Result<<CTCP_MQUOTE_CHAR;
	    else
		Result<<in[i];
	}
	else
	    Result<<in[i];
    }
    RET(Result);
}

mstring DccEngine::ctcpQuote(mstring& in)
{
    FT("DccEngine::ctcpQuote",((in)));
    mstring Result;
    for(unsigned int i=0; i<in.length(); i++)
    {
	if(in[i]==CTCP_DELIM_CHAR)
	    Result<<CTCP_QUOTE_CHAR<<'a';
	else if(in[i]==CTCP_QUOTE_CHAR)
	    Result<<CTCP_QUOTE_CHAR<<CTCP_QUOTE_CHAR;
	else
	    Result<<in[i];
    }
    RET(Result);
}

mstring DccEngine::ctcpDequote(mstring& in)
{
    FT("DccEngine::ctcpDequote",((in)));
    mstring Result;
    for(unsigned int i=0; i<in.length(); i++)
    {
	if(in[i]==CTCP_DELIM_CHAR && i+1<in.length())
	{
	    i++;
	    if(in[i]=='a')
		Result<<CTCP_DELIM_CHAR;
	    else if(in[i]==CTCP_QUOTE_CHAR)
		Result<<CTCP_QUOTE_CHAR;
	    else
		Result<<in[i];
	}
	else
	    Result<<in[i];
    }
    RET(Result);
}

vector<mstring> DccEngine::ctcpExtract(mstring& in)
{
    FT("DccEngine::ctcpExtract",((in)));
    // pull out /001...../001 pairs
    vector<mstring> Result;
    int Start,End,occ=1;
    Start=in.find(mstring(CTCP_DELIM_CHAR).c_str(), occ++);
    if(Start<0)
	NRET(vector<mstring>, Result);
    End=in.find(mstring(CTCP_DELIM_CHAR).c_str(), occ++);
    if (End<0)
	NRET(vector<mstring>, Result);
    while(Start >= 0 && Start < (int) in.length() &&
	  End >= 0 && End < (int) in.length())
    {
	// the +1,-1 removes the '\001' markers off front and back
	Result.push_back(in.SubString(Start+1,End-1));
	Start=End;
	End=in.find(mstring(CTCP_DELIM_CHAR).c_str(), occ++);
    }
    NRET(vector<mstring>, Result);
}

void DccEngine::decodeReply(const mstring& mynick, const mstring& source,
		       mstring& in)
{
    FT("DccEngine::decodeReply",((in)));
    vector<mstring> ResVector;
    mstring ResMid=lowDequote(in);
    if(in.Occurances(mstring(CTCP_DELIM_CHAR).c_str())<2)
	return;
    if(in.Occurances(mstring(CTCP_DELIM_CHAR).c_str())>8)
    {
	CP(("Hmm way too many ctcp's in a single line, flood? ignoring..."));
	return;
    }
    ResVector=ctcpExtract(ResMid);
    vector<mstring>::iterator i;
    for(i=ResVector.begin();i!=ResVector.end();i++)
    {
	mstring ResHigh=ctcpDequote(*i);
	// todo if( ) { } else if( ) {} where first word is SED etc.
    }
}

void DccEngine::decodeRequest(const mstring& mynick, const mstring& source,
			      mstring& in)
{
    FT("DccEngine::decodeRequest",((in)));
    vector<mstring> ResVector;
    mstring ResMid=lowDequote(in);
    if(in.Occurances(mstring(CTCP_DELIM_CHAR).c_str())<2)
	return;
    if(in.Occurances(mstring(CTCP_DELIM_CHAR).c_str())>8)
    {
	CP(("Hmm way too many ctcp's in a single line, flood? ignoring..."));
	return;
    }
    ResVector=ctcpExtract(ResMid);
    vector<mstring>::iterator i;
    for(i=ResVector.begin();i!=ResVector.end();i++)
    {
	mstring ResHigh=ctcpDequote(*i);
	// todo if( ) { } else if( ) {} where first word is SED etc.
	if(ResHigh.Before(" ").UpperCase()=="ACTION")
	{
	    CP(("Got ACTION ctcp request, ignoring"));
	}
	else if(ResHigh.Before(" ").UpperCase()=="SED")
	{
	    CP(("Got SED ctcp request, ignoring"));
	}
	else if(ResHigh.Before(" ").UpperCase()=="DCC")
	    GotDCC(mynick, source, ResHigh.After(" "));

	else if(ResHigh.Before(" ").UpperCase()=="FINGER")
	{
	    /*
	    while the reply is in a "notice" and looks like
		\001FINGER :#\001
	    where the # denotes contains information about the users real name,
	    login name at clientmachine and idle time and is of type X-N-AS.
		X-N-AS ::= '\000'  | '\002' .. '\037' | '\041' .. '\377'
	    */

	    Parent->server.NOTICE(mynick, source, encode("FINGER",
		mstring(PACKAGE) + " Service - " + mynick));
	}
	else if(ResHigh.Before(" ").UpperCase()=="VERSION")
	{
	    /*
	    and the reply
		\001VERSION #:#:#\001
	    where the first # denotes the name of the client, the second # denotes
	    the version of the client, the third # the enviroment the client is
	    running in.
	    Using
		X-N-CLN	::= '\000' .. '\071' | '\073' .. '\377' 
	    */

	    mstring tmp;
	    tmp << PACKAGE << ":" << VERSION;
	    if(!RELEASE.empty())
		tmp+="-" + RELEASE;
	    if(!PATCH1.empty())
		tmp+="+"+PATCH1;
	    if(!PATCH2.empty())
		tmp+="+"+PATCH2;
	    if(!PATCH3.empty())
		tmp+="+"+PATCH3;
	    if(!PATCH4.empty())
		tmp+="+"+PATCH4;
	    if(!PATCH5.empty())
		tmp+="+"+PATCH5;
	    if(!PATCH6.empty())
		tmp+="+"+PATCH6;
	    if(!PATCH7.empty())
		tmp+="+"+PATCH7;
	    if(!PATCH8.empty())
		tmp+="+"+PATCH8;
	    if(!PATCH9.empty())
		tmp+="+"+PATCH9;
	    tmp<<":"<<BUILD_TYPE;

	    Parent->server.NOTICE(mynick, source, encode("VERSION", tmp));
	}
	else if(ResHigh.Before(" ").UpperCase()=="SOURCE")
	{
	    /*
	    and the reply is zero or more CTCP replies of the form
		\001SOURCE #:#:#\001
	    followed by an end marker
		\001SOURCE\001
	    where the first # is the name of an Internet host where the client can
	    be gotten from with anonymous FTP the second # a directory names, and
	    the third # a space separated list of files to be gotten from that
	    directory.
	    Using
		X-N-SPC	::= '\000' .. '\037' | '\041' .. '\377' 
	    */
	    mstring tmp(VERSION);
	    if(!RELEASE.empty())
		tmp+="-" + RELEASE;

	    Parent->server.NOTICE(mynick, source, encode("SOURCE",
		    DOWNLOAD+":"+PACKAGE+tmp+".tar.gz"));
	}
	else if(ResHigh.Before(" ").UpperCase()=="USERINFO")
	{
	    /*
	    with the reply
		\001USERINFO :#\001
	    where the # is the value of the string the client's user has set.
	    */

	    Parent->server.NOTICE(mynick, source, encode("USERINFO",
		    FULLNAME+" - "+SLOGAN));
	}
	else if(ResHigh.Before(" ").UpperCase()=="CLIENTINFO")
	{
	    /*
	    The query is the word CLIENTINFO in a "privmsg" optionally followed by
	    a colon and one or more specifying words delimited by spaces, where
	    the word CLIENTINFO by itself,
		\001CLIENTINFO\001
	    should be replied to by giving a list of known tags (see above in
	    section TAGGED DATA). This is only intended to be read by humans.
	    With one argument, the reply should be a description of how to use
	    that tag. With two arguments, a description of how to use that
	    tag's subcommand. And so on.
	    */

	    Parent->server.NOTICE(mynick, source, encode("CLIENTINFO",
		    mstring("ACTION SED DCC FINGER VERSION SOURCE "
		    "USERINFO CLIENTINFO ERRMSG PING TIME")));
	}
	else if(ResHigh.Before(" ").UpperCase()=="ERRMSG")
	{
	    /*
	    This is used as a reply whenever an unknown query is seen. Also, when
	    used as a query, the reply should echo back the text in the query,
	    together with an indication that no error has happened. Should the
	    query form be used, it is
		\001ERRMSG #\001
	    where # is a string containing any character, with the reply
		\001ERRMSG # :#\001
	    where the first # is the same string as in the query and the second #
	    a short text notifying the user that no error has occurred.
	    A normal ERRMSG reply which is sent when a corrupted query or some
	    corrupted extended data is received, looks like
		\001ERRMSG # :#\001
	    where the first # is the the failed query or corrupted extended data
	    and the second # a text explaining what the problem is, like "unknown
	    query" or "failed decrypting text".
	    */

	    Parent->server.NOTICE(mynick, source, encode(ResHigh + " :No Error"));
	}
	else if(ResHigh.Before(" ").UpperCase()=="PING")
	{
	    /*
	    Ping is used to measure the time delay between clients on the IRC
	    network. A ping query is encoded in a privmsg, and has the form:
		\001PING timestamp\001
	    where `timestamp' is the current time encoded in any form the querying
	    client finds convienent. The replying client sends back an identical
	    message inside a notice:
		\001PING timestamp\001
	    The querying client can then subtract the recieved timestamp from the
	    current time to obtain the delay between clients over the IRC network
	    */

	    Parent->server.NOTICE(mynick, source, encode(ResHigh));
	}
	else if(ResHigh.Before(" ").UpperCase()=="TIME")
	{
	    /*
	    Time queries are used to determine what time it is where another
	    user's client is running. This can be useful to determine if someone
	    is probably awake or not, or what timezone they are in. A time query
	    has the form:
		\001TIME\001
	    On reciept of such a query in a privmsg, clients should reply with a
	    notice of the form:
		\001TIME :human-readable-time-string\001
	    For example:
		\001TIME :Thu Aug 11 22:52:51 1994 CST\001
	    */

	    Parent->server.NOTICE(mynick, source, encode("TIME", mDateTime::CurrentDateTime().DateTimeString()));
	}
	else
	{
	    CP(("Got Unknown ctcp request of "+ResHigh.Before(" ").UpperCase()+" ignoring"));
	}
    }

}

mstring DccEngine::encode(const mstring type, const mstring & in)
{
    FT("DccEngine::encode",((in)));
    mstring Result;
    Result << CTCP_DELIM_CHAR << type;
    if (!in.empty())
	Result << " " << in;
    Result << CTCP_DELIM_CHAR;
//    mstring ResMid=ctcpQuote(in);
//    Result=lowQuote(ResMid);
//    RET(Result);
    RET(Result);
}

void DccEngine::GotDCC(const mstring& mynick, const mstring& source,
		       mstring in)
{
    FT("DccEngine::GotDCC", (mynick, source, in));
    mstring type,argument,straddress,strport,strsize;
    unsigned short port;
    unsigned long address,size,longport;
    type	=in.ExtractWord(1, " ");
    argument	=in.ExtractWord(2, " ");
    straddress	=in.ExtractWord(3, " ");
    strport	=in.ExtractWord(4, " ");
    if (in.WordCount(" ") > 4)
	strsize = in.ExtractWord(5, " ");

    address = atoul(straddress.c_str());
    longport = atoul(strport.c_str());
    port = (unsigned short) longport;
    size = 0;
    if (!strsize.empty())
	size = atoul(strsize.c_str());

    ACE_INET_Addr Server(port,address);
    if(type.UpperCase()=="CHAT")
    {
	DoDccChat(mynick, source, Server);
    }
    else if(type.UpperCase()=="SEND")
    {
	DoDccSend(mynick, source, Server, argument, size);
    }
    else
    {
	CP(("Invalid Type in DCC Command of:"+type));
	return;
    }
}   

void DccEngine::DoDccChat(const mstring& mynick, const mstring& source,
			  ACE_INET_Addr addr)
{
    FT("DccEngine::DoDccChat", (mynick, source, "(ACE_INET_Addr) addr"));
    // todo: check if we should accept this dcc (ie is it an oper?)

    // create a new threaded connection that is nothing more than a standard
    // tcp/ip socket connection for two way chatting.
    // using addr.address and addr.port
    // the text "DCC CLOSE" accross the connection signal's end of session

    // ONLY the global noticer should accept DCC connections, and
    // it will then accept commands in an eggdrop-like manner, ie.
    // .ns for nickserv, .cs for chanserv, .os for operserv.  It
    // should just pass the commands through the same engine as the
    // telnet commands.

    send(mynick, source, Parent->getMessage(source, "DCC/NOACCESS"),
						"CHAT");
}

// INBOUND DCC!!
void DccEngine::DoDccSend(const mstring& mynick, const mstring& source,
			  ACE_INET_Addr addr, mstring filename,
			  size_t size)
{
    FT("DccEngine::DoDccSend", (mynick, source, "(ACE_INET_Addr) addr",
    			  filename, size));
    // todo: check if we should accept this dcc (ie is it an oper?)
    // create a new threaded connection that is nothing more than a standard
    // tcp/ip socket connection for sending files
    // upon receiving each packet, we transmit a 4byte "received bytes" count.
    // in network bytesex.
    // using addr.address and addr.port

    // For memoserv filesystem, check if there has been a 'FILE'
    // request recently (ie. that has not timed out) for the
    // filename they are sending, and that the size is less than
    // the MAX_BYTES allowed, and they are below their file #
    // threshold (which sould be checked in the actual memoserv
    // FILE command).  If we will allow the DCC, then add it to
    // the file map (file # -> name & user)
    if (!Parent->nickserv.IsLive(source))
	return;

    if (!(Parent->nickserv.live[source.LowerCase()].InFlight.File() &&
	!Parent->nickserv.live[source.LowerCase()].InFlight.InProg()))
    {
	send(mynick, source, Parent->getMessage(source, "DCC/NOREQUEST"),
						"GET");
	return;
    }


    if (Parent->nickserv.live[source.LowerCase()].InFlight.Picture())
    {
	if (size && Parent->nickserv.PicSize() && size > Parent->nickserv.PicSize())
	{
	    send(mynick, source, Parent->getMessage(source, "DCC/TOOBIG"), "GET");
	    return;
	}
	
	mstring extension = filename.ExtractWord(filename.WordCount("."), ".").LowerCase();
	if (!(filename.Contains(".") &&
	    (" " + Parent->nickserv.PicExt().LowerCase() + " ").Contains(" " + extension + " ")))
	{
	    send(mynick, source, Parent->getMessage(source, "NS_YOU_STATUS/INVALIDEXT"));
	    Parent->nickserv.live[source.LowerCase()].InFlight.Cancel();
	    return;
	}
	else
	{
	    filename = source.LowerCase() + "." + extension;
	}
    }

    if (Parent->nickserv.live[source.LowerCase()].InFlight.Memo())
    {
	if (size && Parent->memoserv.FileSize() && size > Parent->memoserv.FileSize())
	{
	    send(mynick, source, Parent->getMessage(source, "DCC/TOOBIG"), "GET");
	    return;
	}
    }

    // Spawn this in a new thread, and we're done, it takes over.

    { RLOCK(("DCC"));
    if (Parent->dcc != NULL)
	Parent->dcc->Connect(addr, mynick, source, filename, size);
    }
}
