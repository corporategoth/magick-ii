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


#include "dccengine.h"
#include "trace.h"
#include "magick.h"

mstring DccEngine::lowQuote(mstring& in)
{
    FT("DccEngine::lowQuote",((in)));
    mstring Result;
    mstring::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos=='\0')
	    Result<<CTCP_MQUOTE_CHAR<<'0';
	else if(*pos=='\n')
	    Result<<CTCP_MQUOTE_CHAR<<'n';
	else if(*pos=='\r')
	    Result<<CTCP_MQUOTE_CHAR<<'r';
	else if(*pos==CTCP_MQUOTE_CHAR)
	    Result<<CTCP_MQUOTE_CHAR<<CTCP_MQUOTE_CHAR;
	else
	    Result<<*pos;
    }
    RET(Result);
}

mstring DccEngine::lowDequote(mstring& in)
{
    FT("DccEngine::lowDequote",((in)));
    mstring Result;
    mstring::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos==CTCP_MQUOTE_CHAR)
	{
	    if(*(pos+1)=='0')
    	    {
		Result<<'\0';
		pos++;
	    }
	    else if(*(pos+1)=='n')
    	    {
		Result<<'\n';
		pos++;
	    }
	    else if(*(pos+1)=='r')
	    {
		Result<<'\r';
		pos++;
	    }
	    else if(*(pos+1)==CTCP_MQUOTE_CHAR)
	    {
		Result<<CTCP_MQUOTE_CHAR;
		pos++;
	    }
	    else
	    {
		Result<<*(pos+1);
		pos++;
	    }
	}
	else
	    Result<<*pos;
    }
    RET(Result);
}

mstring DccEngine::ctcpQuote(mstring& in)
{
    FT("DccEngine::ctcpQuote",((in)));
    mstring Result;
    mstring::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos==CTCP_DELIM_CHAR)
	    Result<<CTCP_QUOTE_CHAR<<'a';
	else if(*pos==CTCP_QUOTE_CHAR)
	    Result<<CTCP_QUOTE_CHAR<<CTCP_QUOTE_CHAR;
	else
	    Result<<*pos;
    }
    RET(Result);
}

mstring DccEngine::ctcpDequote(mstring& in)
{
    FT("DccEngine::ctcpDequote",((in)));
    mstring Result;
    mstring::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos==CTCP_DELIM_CHAR)
	{
	    if(*(pos+1)=='a')
	    {
		Result<<CTCP_DELIM_CHAR;
		pos++;
	    }
	    else if(*(pos+1)==CTCP_QUOTE_CHAR)
	    {
		Result<<CTCP_QUOTE_CHAR;
		pos++;
	    }
	    else
	    {
		Result<<*(pos+1);
		pos++;
	    }
	}
	else
	    Result<<*pos;
    }
    RET(Result);
}

vector<mstring> DccEngine::ctcpExtract(mstring& in)
{
    FT("DccEngine::ctcpExtract",((in)));
    // pull out /001...../001 pairs
    vector<mstring> Result;
    mstring tmpstring;
    mstring::iterator Start,End;
    Start=find(in.begin(),in.end(),CTCP_DELIM_CHAR);
    if(Start==in.end())
	return Result;
    End=find(Start+1,in.end(),CTCP_DELIM_CHAR);
    while(Start != in.end() && End != in.end())
    {
	// the +1,-1 removes the '\001' markers off front and back
	tmpstring.assign(Start+1,End);
	Result.push_back(tmpstring);
	Start=End;
	if(Start!=in.end())
	    End=find(Start+1,in.end(),CTCP_DELIM_CHAR);
    }
    return Result;
}

void DccEngine::decodeReply(const mstring& mynick, const mstring& source,
		       mstring& in)
{
    FT("DccEngine::decodeReply",((in)));
    vector<mstring> ResVector;
    mstring ResMid=lowDequote(in);
    if(count(in.begin(),in.end(),CTCP_DELIM_CHAR)<2)
	return;
    if(count(in.begin(),in.end(),CTCP_DELIM_CHAR)>8)
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
    if(count(in.begin(),in.end(),CTCP_DELIM_CHAR)<2)
	return;
    if(count(in.begin(),in.end(),CTCP_DELIM_CHAR)>8)
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
		"Magick II Service - " + mynick));
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
	    tmp << "Magick:";
	    tmp << Magick_Major_Ver << "." << Magick_Minor_Ver;
	    if(RELEASE!="")
		tmp+="-" + RELEASE;
	    if(PATCH1!="")
		tmp+="+"+PATCH1;
	    if(PATCH2!="")
		tmp+="+"+PATCH1;
	    if(PATCH3!="")
		tmp+="+"+PATCH1;
	    if(PATCH4!="")
		tmp+="+"+PATCH1;
	    if(PATCH5!="")
		tmp+="+"+PATCH1;
	    if(PATCH6!="")
		tmp+="+"+PATCH1;
	    if(PATCH7!="")
		tmp+="+"+PATCH1;
	    if(PATCH8!="")
		tmp+="+"+PATCH1;
	    if(PATCH9!="")
		tmp+="+"+PATCH1;
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
	    mstring tmp;
	    tmp << Magick_Major_Ver << "." << Magick_Minor_Ver;
	    if(RELEASE!="")
		tmp+="-" + RELEASE;

	    Parent->server.NOTICE(mynick, source, encode("SOURCE",
		    "ftp.magick.tm:/pub/Magick:Magick-"+tmp+".tar.gz"));
	    Parent->server.NOTICE(mynick, source, encode("SOURCE"));
	}
	else if(ResHigh.Before(" ").UpperCase()=="USERINFO")
	{
	    /*
	    with the reply
		\001USERINFO :#\001
	    where the # is the value of the string the client's user has set.
	    */

	    Parent->server.NOTICE(mynick, source, encode("USERINFO",
		    mstring("Magick II - Power to the PEOPLE!!")));
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

	    Parent->server.NOTICE(mynick, source, encode(ResHigh));
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

	    Parent->server.NOTICE(mynick, source, encode("TIME", Now().DateTimeString()));
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
    if (!in.IsEmpty())
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
    mstring type,argument,straddress,strport,strsize;
    unsigned short port;
    unsigned long address,size,longport;
    type	=in.ExtractWord(1, " ");
    argument	=in.ExtractWord(2, " ");
    straddress	=in.ExtractWord(3, " ");
    if(count(strport.begin(),strport.end(),' ')>=1)
    {
	strport=in.ExtractWord(4, " ");
	strsize=in.After(" ", 4);
    }
    else
    {
	strport=in.After(" ", 3);
	strsize="";
    }
    sscanf(straddress.c_str(),"%u",&address);
    sscanf(strport.c_str(),"%u",&longport);
    port=(unsigned short)longport;
    if(strsize!="")
	sscanf(strsize.c_str(),"%u",&size);
    else
	size=0;
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
}

// INBOUND DCC!!
void DccEngine::DoDccSend(const mstring& mynick, const mstring& source,
			  ACE_INET_Addr addr, mstring filename,
			  size_t size)
{
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

    if (!Parent->nickserv.live[source.LowerCase()].InFlight.File())
    {
	send(mynick, source, Parent->getMessage(source, "ERR_SITUATION/DCCSENDREFUSE"));
	return;
    }



}
