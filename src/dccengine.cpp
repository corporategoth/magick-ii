#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
#include "dccengine.h"
#include <algorithm>
using namespace std;
#include "trace.h"

const char CTCP_DELIM_CHAR='\001';
const char CTCP_QUOTE_CHAR='\\';
const char CTCP_MQUOTE_CHAR='\020';
//#define CTCP_QUOTE_EM   "\r\n\001\\"

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
    mstring::iterator start,end;
    start=find(in.begin(),in.end(),CTCP_DELIM_CHAR);
    if(start==in.end())
	return Result;
    end=find(start+1,in.end(),CTCP_DELIM_CHAR);
    while(start!=in.end()&&end!=in.end())
    {
	// the +1,-1 removes the '\001' markers off front and back
	tmpstring.assign(start+1,end-1);
	Result.push_back(tmpstring);
	start=end;
	if(start!=in.end())
	    end=find(start+1,in.end(),CTCP_DELIM_CHAR);
    }
    return Result;
}

void DccEngine::decode(mstring& in)
{
    FT("DccEngine::decode",((in)));
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
	    GotDCC(ResHigh.After(" "));
	else if(ResHigh.Before(" ").UpperCase()=="FINGER")
	{
// X-N-AS ::= '\000'  | '\002' .. '\037' | '\041' .. '\377' */
/*
while the reply is in a "notice" and looks like
	\001FINGER :#\001
where the # denotes contains information about the users real name,
login name at clientmachine and idle time and is of type X-N-AS.
*/
	    CP(("Got FINGER ctcp request, ignoring till codebase has more bits to it"));
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
	    CP(("Got VERSION ctcp request, ignoring till codebase has more bits to it"));
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
	    CP(("Got SOURCE ctcp request, ignoring till codebase has more bits to it"));
	}
	else if(ResHigh.Before(" ").UpperCase()=="USERINFO")
	{
/*
with the reply
	\001USERINFO :#\001
where the # is the value of the string the client's user has set.
*/
	    CP(("Got USERINFO ctcp request, ignoring till codebase has more bits to it"));
	}
	else if(ResHigh.Before(" ").UpperCase()=="USERINFO")
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
	    CP(("Got CLIENTINFO ctcp request, ignoring till codebase has more bits to it"));
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
	    CP(("Got ERRMSG ctcp request, ignoring till codebase has more bits to it"));
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
	    CP(("Got PING ctcp request, ignoring till codebase has more bits to it"));
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
	    CP(("Got TIME ctcp request, ignoring till codebase has more bits to it"));
	}
	else
	{
	    CP(("Got Unknown ctcp request of "+ResHigh.Before(" ").UpperCase()+" ignoring"));
	}
    }

}

mstring DccEngine::encode(mstring & in)
{
    FT("DccEngine::encode",((in)));
    mstring Result;
    mstring ResMid=ctcpQuote(in);
    Result=lowQuote(ResMid);
    RET(Result);
}

void DccEngine::GotDCC(const mstring & in)
{
    mstring type,argument,straddress,strport,strsize;
    unsigned short port;
    unsigned long address,size,longport;
    type=in.Before(" ");
    argument=in.After(" ").Before(" ");
    straddress=in.After(" ").After(" ").Before(" ");
    if(count(strport.begin(),strport.end(),' ')>=1)
    {
	strport=in.After(" ").After(" ").After(" ").Before(" ");
	strsize=in.After(" ").After(" ").After(" ").After(" ");
    }
    else
    {
	strport=in.After(" ").After(" ").After(" ");
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
	DoDccChat(Server);
    }
    else if(type.UpperCase()=="SEND")
    {
	DoDccSend(Server,size);
    }
    else
    {
	CP(("Invalid Type in DCC Command of:"+type));
	return;
    }
}   

void DccEngine::DoDccChat(ACE_INET_Addr addr)
{
    //todo: check if we should accept this dcc (ie is it an oper?)

    // create a new threaded connection that is nothing more than a standard
    // tcp/ip socket connection for two way chatting.
    // using addr.address and addr.port
    // the text "DCC CLOSE" accross the connection signal's end of session
}

void DccEngine::DoDccSend(ACE_INET_Addr addr, size_t size)
{
    //todo: check if we should accept this dcc (ie is it an oper?)
    // create a new threaded connection that is nothing more than a standard
    // tcp/ip socket connection for sending files
    // upon receiving each packet, we transmit a 4byte "received bytes" count.
    // in network bytesex.
    // using addr.address and addr.port
}
