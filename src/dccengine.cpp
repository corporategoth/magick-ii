#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
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
#define RCSID(x,y) const char *rcsid_dccengine_cpp_ ## x () { return y; }
RCSID(dccengine_cpp, "@(#)$Id$");

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

mstring DccEngine::lowQuote(const mstring & in)
{
    BTCB();
    FT("DccEngine::lowQuote", (in));
    mstring Result;

    for (unsigned int i = 0; i < in.length(); i++)
    {
	if (in[i] == '\0')
	    Result << CTCP_MQUOTE_CHAR << '0';
	else if (in[i] == '\n')
	    Result << CTCP_MQUOTE_CHAR << 'n';
	else if (in[i] == '\r')
	    Result << CTCP_MQUOTE_CHAR << 'r';
	else if (in[i] == CTCP_MQUOTE_CHAR)
	    Result << CTCP_MQUOTE_CHAR << CTCP_MQUOTE_CHAR;
	else
	    Result << in[i];
    }
    RET(Result);
    ETCB();
}

mstring DccEngine::lowDequote(const mstring & in)
{
    BTCB();
    FT("DccEngine::lowDequote", (in));
    mstring Result;

    for (unsigned int i = 0; i < in.length(); i++)
    {
	if (in[i] == CTCP_MQUOTE_CHAR && i + 1 < in.length())
	{
	    i++;
	    if (in[i] == '0')
		Result << '\0';
	    else if (in[i] == 'n')
		Result << '\n';
	    else if (in[i] == 'r')
		Result << '\r';
	    else if (in[i] == CTCP_MQUOTE_CHAR)
		Result << CTCP_MQUOTE_CHAR;
	    else
		Result << in[i];
	}
	else
	    Result << in[i];
    }
    RET(Result);
    ETCB();
}

mstring DccEngine::ctcpQuote(const mstring & in)
{
    BTCB();
    FT("DccEngine::ctcpQuote", (in));
    mstring Result;

    for (unsigned int i = 0; i < in.length(); i++)
    {
	if (in[i] == CTCP_DELIM_CHAR)
	    Result << CTCP_QUOTE_CHAR << 'a';
	else if (in[i] == CTCP_QUOTE_CHAR)
	    Result << CTCP_QUOTE_CHAR << CTCP_QUOTE_CHAR;
	else
	    Result << in[i];
    }
    RET(Result);
    ETCB();
}

mstring DccEngine::ctcpDequote(const mstring & in)
{
    BTCB();
    FT("DccEngine::ctcpDequote", (in));
    mstring Result;

    for (unsigned int i = 0; i < in.length(); i++)
    {
	if (in[i] == CTCP_DELIM_CHAR && i + 1 < in.length())
	{
	    i++;
	    if (in[i] == 'a')
		Result << CTCP_DELIM_CHAR;
	    else if (in[i] == CTCP_QUOTE_CHAR)
		Result << CTCP_QUOTE_CHAR;
	    else
		Result << in[i];
	}
	else
	    Result << in[i];
    }
    RET(Result);
    ETCB();
}

mstring DccEngine::addressEncode(const mstring &in)
{
    BTCB();
    FT("DccEngine::addressEncode", (in));

    ACE_INET_Addr addr;
    if (addr.set(static_cast<unsigned short>(0), in) < 0)
    {
	RET("");
    }
    mstring rv(addr.get_ip_address());

    RET(rv);
    ETCB();
}

mstring DccEngine::addressDecode(const mstring &in)
{
    BTCB();
    FT("DccEngine::addressDecode", (in));

    ACE_INET_Addr addr;
    if (addr.set(static_cast<unsigned short>(0), atoui(in.c_str())) < 0)
    {
	RET("");
    }
    mstring rv(addr.get_host_addr());

    RET(rv);
    ETCB();
}

vector < mstring > DccEngine::ctcpExtract(const mstring & in)
{
    BTCB();
    FT("DccEngine::ctcpExtract", (in));
    // pull out /001...../001 pairs
    vector < mstring > Result;
    int Start, End, occ = 1;

    Start = in.find(mstring(CTCP_DELIM_CHAR).c_str(), occ++);
    if (Start < 0)
	NRET(vector < mstring >, Result);
    End = in.find(mstring(CTCP_DELIM_CHAR).c_str(), occ++);
    if (End < 0)
	NRET(vector < mstring >, Result);
    while (Start >= 0 && Start < static_cast < int > (in.length()) && End >= 0 && End < static_cast < int > (in.length()))
    {
	// the +1,-1 removes the '\001' markers off front and back
	Result.push_back(in.SubString(Start + 1, End - 1));
	Start = End;
	End = in.find(mstring(CTCP_DELIM_CHAR).c_str(), occ++);
    }
    NRET(vector < mstring >, Result);
    ETCB();
}

void DccEngine::decodeReply(const mstring & mynick, const mstring & source, const mstring & in)
{
    BTCB();
    static_cast < void > (mynick);
    static_cast < void > (source);
    static_cast < void > (in);

#if 0 // Unused code, for now, why bother wasting cycles ...

    FT("DccEngine::decodeReply", (in));

    vector < mstring > ResVector;
    mstring ResMid = lowDequote(in);

    if (in.Occurances(mstring(CTCP_DELIM_CHAR).c_str()) < 2)
	return;
    if (in.Occurances(mstring(CTCP_DELIM_CHAR).c_str()) > 8)
    {
	CP(("Hmm way too many ctcp's in a single line, flood? ignoring..."));
	return;
    }
    ResVector = ctcpExtract(ResMid);
    vector < mstring >::iterator i;
    for (i = ResVector.begin(); i != ResVector.end(); i++)
    {
	mstring ResHigh = ctcpDequote(*i);

	// todo if( ) { } else if( ) {} where first word is SED etc.
    }

#endif // Unused code, for now, why bother wasting cycles ...

    ETCB();
}

void DccEngine::decodeRequest(const mstring & mynick, const mstring & source, const mstring & in)
{
    BTCB();
    FT("DccEngine::decodeRequest", ((in)));
    vector < mstring > ResVector;
    mstring ResMid = lowDequote(in);

    if (in.Occurances(mstring(CTCP_DELIM_CHAR).c_str()) < 2)
	return;
    if (in.Occurances(mstring(CTCP_DELIM_CHAR).c_str()) > 8)
    {
	CP(("Hmm way too many ctcp's in a single line, flood? ignoring..."));
	return;
    }
    ResVector = ctcpExtract(ResMid);
    vector < mstring >::iterator i;
    for (i = ResVector.begin(); i != ResVector.end(); i++)
    {
	mstring ResHigh = ctcpDequote(*i);

	// todo if( ) { } else if( ) {} where first word is SED etc.
	if (ResHigh.Before(" ").UpperCase() == "ACTION")
	{
	    CP(("Got ACTION ctcp request, ignoring"));
	}
	else if (ResHigh.Before(" ").UpperCase() == "SED")
	{
	    CP(("Got SED ctcp request, ignoring"));
	}
	else if (ResHigh.Before(" ").UpperCase() == "DCC")
	    GotDCC(mynick, source, ResHigh.After(" "));

	else if (ResHigh.Before(" ").UpperCase() == "FINGER")
	{
	    /*
	     * while the reply is in a "notice" and looks like
	     * \001FINGER :#\001
	     * where the # denotes contains information about the users real name,
	     * login name at clientmachine and idle time and is of type X-N-AS.
	     * X-N-AS ::= '\000'  | '\002' .. '\037' | '\041' .. '\377'
	     */

	    Magick::instance().server.NOTICE(mynick, source, encode("FINGER", mstring(PACKAGE) + " Service - " + mynick));
	}
	else if (ResHigh.Before(" ").UpperCase() == "VERSION")
	{
	    /*
	     * and the reply
	     * \001VERSION #:#:#\001
	     * where the first # denotes the name of the client, the second # denotes
	     * the version of the client, the third # the enviroment the client is
	     * running in.
	     * Using
	     * X-N-CLN  ::= '\000' .. '\071' | '\073' .. '\377' 
	     */

	    mstring tmp;

	    tmp << PACKAGE << ":" << version_string(false) << ":" << sysinfo_type();
	    Magick::instance().server.NOTICE(mynick, source, encode("VERSION", tmp));
	}
	else if (ResHigh.Before(" ").UpperCase() == "SOURCE")
	{
	    /*
	     * and the reply is zero or more CTCP replies of the form
	     * \001SOURCE #:#:#\001
	     * followed by an end marker
	     * \001SOURCE\001
	     * where the first # is the name of an Internet host where the client can
	     * be gotten from with anonymous FTP the second # a directory names, and
	     * the third # a space separated list of files to be gotten from that
	     * directory.
	     * Using
	     * X-N-SPC  ::= '\000' .. '\037' | '\041' .. '\377' 
	     */
	    mstring tmp;

	    tmp << PACKAGE << "-" << VERSION << RELEASE;
	    Magick::instance().server.NOTICE(mynick, source, encode("SOURCE", DOWNLOAD + ":" + tmp + ".tar.gz"));
	}
	else if (ResHigh.Before(" ").UpperCase() == "USERINFO")
	{
	    /*
	     * with the reply
	     * \001USERINFO :#\001
	     * where the # is the value of the string the client's user has set.
	     */

	    Magick::instance().server.NOTICE(mynick, source, encode("USERINFO", FULLNAME + " - " + SLOGAN));
	}
	else if (ResHigh.Before(" ").UpperCase() == "CLIENTINFO")
	{
	    /*
	     * The query is the word CLIENTINFO in a "privmsg" optionally followed by
	     * a colon and one or more specifying words delimited by spaces, where
	     * the word CLIENTINFO by itself,
	     * \001CLIENTINFO\001
	     * should be replied to by giving a list of known tags (see above in
	     * section TAGGED DATA). This is only intended to be read by humans.
	     * With one argument, the reply should be a description of how to use
	     * that tag. With two arguments, a description of how to use that
	     * tag's subcommand. And so on.
	     */

	    Magick::instance().server.NOTICE(mynick, source,
					     encode("CLIENTINFO",
						    mstring("ACTION SED DCC FINGER VERSION SOURCE "
							    "USERINFO CLIENTINFO ERRMSG PING TIME")));
	}
	else if (ResHigh.Before(" ").UpperCase() == "ERRMSG")
	{
	    /*
	     * This is used as a reply whenever an unknown query is seen. Also, when
	     * used as a query, the reply should echo back the text in the query,
	     * together with an indication that no error has happened. Should the
	     * query form be used, it is
	     * \001ERRMSG #\001
	     * where # is a string containing any character, with the reply
	     * \001ERRMSG # :#\001
	     * where the first # is the same string as in the query and the second #
	     * a short text notifying the user that no error has occurred.
	     * A normal ERRMSG reply which is sent when a corrupted query or some
	     * corrupted extended data is received, looks like
	     * \001ERRMSG # :#\001
	     * where the first # is the the failed query or corrupted extended data
	     * and the second # a text explaining what the problem is, like "unknown
	     * query" or "failed decrypting text".
	     */

	    Magick::instance().server.NOTICE(mynick, source, encode(ResHigh + " :No Error"));
	}
	else if (ResHigh.Before(" ").UpperCase() == "PING")
	{
	    /*
	     * Ping is used to measure the time delay between clients on the IRC
	     * network. A ping query is encoded in a privmsg, and has the form:
	     * \001PING timestamp\001
	     * where `timestamp' is the current time encoded in any form the querying
	     * client finds convienent. The replying client sends back an identical
	     * message inside a notice:
	     * \001PING timestamp\001
	     * The querying client can then subtract the recieved timestamp from the
	     * current time to obtain the delay between clients over the IRC network
	     */

	    Magick::instance().server.NOTICE(mynick, source, encode(ResHigh));
	}
	else if (ResHigh.Before(" ").UpperCase() == "TIME")
	{
	    /*
	     * Time queries are used to determine what time it is where another
	     * user's client is running. This can be useful to determine if someone
	     * is probably awake or not, or what timezone they are in. A time query
	     * has the form:
	     * \001TIME\001
	     * On reciept of such a query in a privmsg, clients should reply with a
	     * notice of the form:
	     * \001TIME :human-readable-time-string\001
	     * For example:
	     * \001TIME :Thu Aug 11 22:52:51 1994 CST\001
	     */

	    Magick::instance().server.NOTICE(mynick, source, encode("TIME", mDateTime::CurrentDateTime().DateTimeString()));
	}
	else
	{
	    CP(("Got Unknown ctcp request of " + ResHigh.Before(" ").UpperCase() + " ignoring"));
	}
    }

    ETCB();
}

mstring DccEngine::encode(const mstring & type, const mstring & in)
{
    BTCB();
    FT("DccEngine::encode", ((in)));
    mstring Result;

    Result << CTCP_DELIM_CHAR << type;
    if (!in.empty())
	Result << " " << in;
    Result << CTCP_DELIM_CHAR;
//    mstring ResMid=ctcpQuote(in);
//    Result=lowQuote(ResMid);
//    RET(Result);
    RET(Result);
    ETCB();
}

void DccEngine::GotDCC(const mstring & mynick, const mstring & source, const mstring & in)
{
    BTCB();
    FT("DccEngine::GotDCC", (mynick, source, in));
    mstring type, argument, straddress, strport, strsize;
    mstring address;
    unsigned short port;
    unsigned long size, longport;

    type = in.ExtractWord(1, " ").UpperCase();
    argument = in.ExtractWord(2, " ");
    straddress = in.ExtractWord(3, " ");
    strport = in.ExtractWord(4, " ");
    if (in.WordCount(" ") > 4)
	strsize = in.ExtractWord(5, " ");

    address = addressDecode(straddress.c_str());
    longport = atoul(strport.c_str());
    port = static_cast < unsigned short > (longport);

    ACE_INET_Addr server;
    if (address.empty() || port == static_cast<unsigned short>(0) || server.set(port, address) < 0)
    {
	SEND(mynick, source, "DCC/FAILED", (type));
	return;
    }

    size = 0;
    if (!strsize.empty())
	size = atoul(strsize.c_str());

    if (type == "CHAT")
    {
	DoDccChat(mynick, source, server);
    }
    else if (type == "SEND")
    {
	DoDccSend(mynick, source, server, argument, size);
    }
    else
    {
	CP(("Invalid Type in DCC Command of:" + type));
	return;
    }
    ETCB();
}

void DccEngine::DoDccChat(const mstring & mynick, const mstring & source, const ACE_INET_Addr & addr)
{
    BTCB();
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

    SEND(mynick, source, "DCC/NOACCESS", ("CHAT"));

    static_cast < void > (addr);

    ETCB();
}

// INBOUND DCC!!
void DccEngine::DoDccSend(const mstring & mynick, const mstring & source, const ACE_INET_Addr & addr, mstring & filename,
			  const size_t size)
{
    BTCB();
    FT("DccEngine::DoDccSend", (mynick, source, "(ACE_INET_Addr) addr", filename, size));
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
    if (!Magick::instance().nickserv.IsLive(source))
	return;

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
    if (!(nlive->InFlight.File() && !nlive->InFlight.InProg()))
    {
	SEND(mynick, source, "DCC/NOREQUEST", ("GET"));
	return;
    }

    if (nlive->InFlight.Picture())
    {
	if (size && Magick::instance().nickserv.PicSize() && size > Magick::instance().nickserv.PicSize())
	{
	    SEND(mynick, source, "DCC/TOOBIG", ("GET"));
	    return;
	}

	mstring extension = filename.ExtractWord(filename.WordCount("."), ".").LowerCase();

	if (!
	    (filename.Contains(".") &&
	     (" " + Magick::instance().nickserv.PicExt().LowerCase() + " ").Contains(" " + extension + " ")))
	{
	    NSEND(mynick, source, "NS_YOU_STATUS/INVALIDEXT");
	    nlive->InFlight.Cancel();
	    return;
	}
	else
	{
	    filename = source.LowerCase() + "." + extension;
	}
    }

    if (nlive->InFlight.Memo())
    {
	if (size && Magick::instance().memoserv.FileSize() && size > Magick::instance().memoserv.FileSize())
	{
	    SEND(mynick, source, "DCC/TOOBIG", ("GET"));
	    return;
	}
    }

    // Spawn this in a new thread, and we're done, it takes over.

    {
	RLOCK(("DCC"));
	if (Magick::instance().dcc != NULL)
	    Magick::instance().dcc->Connect(addr, mynick, source, filename, size);
    }
    ETCB();
}
