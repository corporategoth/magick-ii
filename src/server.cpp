#include "pch.h"
#ifdef WIN32
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
#include "server.h"
#include "lockable.h"
#include "magick.h"

Server::Server()
{
    NFT("Server::Server");
}

Server::Server(const Server &in)
{
    FT("Server::Server", ("(const Server &) in"));
    *this = in;
}

Server::Server(mstring name, int hops, mstring description)
{
    FT("Server::Server", (name, hops, description));
    i_Name = name.LowerCase();
    i_Uplink = Parent->Startup_SERVER_NAME.LowerCase();
    i_Hops = hops;
    i_Description = description;
}

Server::Server(mstring name, mstring uplink, int hops, mstring description)
{
    FT("Server::Server", (name, uplink, hops, description));
    i_Name = name.LowerCase();
    i_Uplink = uplink.LowerCase();
    i_Hops = hops;
    i_Description = description;
}

void Server::operator=(const Server &in)
{
    FT("Server::operator=", ("(const Server &) in"));
    i_Name = in.i_Name;
    i_Uplink = in.i_Uplink;
    i_Hops = in.i_Hops;
    i_Description = in.i_Description;
    i_Ping = in.i_Ping;
    i_Lag = in.i_Lag;
}

bool Server::operator==(const Server &in) const
{
    FT("Server::operator==", ("(const Server &) in"));
    RET(i_Name == in.i_Name);
}

bool Server::operator<(const Server &in) const
{
    FT("Server::operator<", ("(const Server &) in"));
    RET(i_Name < in.i_Name);
}

mstring Server::Name()
{
    NFT("Server::Name");
    RET(i_Name);
}

mstring Server::Uplink()
{
    NFT("Server::Uplink");
    RET(i_Uplink);
}

int Server::Hops()
{
    NFT("Server::Hops");
    RET(i_Hops);
}

mstring Server::Description()
{
    NFT("Server::Description");
    RET(i_Description);
}

void Server::Ping()
{
    NFT("Server::Ping");
    if (!i_Ping)
    {
        SendSVR("PING " + Parent->Startup_SERVER_NAME + " :" + i_Name);
#ifndef WIN32
	timeval *tmp;
	gettimeofday(tmp, NULL);
	i_Ping = (double)tmp->tv_sec + ((double)tmp->tv_usec / 1000.0);
#else
	SYSTEMTIME lpSystemTime;
	GetSystemTime(&lpSystemTime);
	i_Ping=((double)lpSystemTime.wHour*3600.0)+((double)lpSystemTime.wMinute*60.0)+(double)lpSystemTime.wSecond+((double)lpSystemTime.wMilliseconds/1000.0);
#endif
   }
}

void Server::Pong()
{
    NFT("Server::Pong");
    if (i_Ping)
    {
#ifndef WIN32
	timeval *tmp;
	gettimeofday(tmp, NULL);
	i_Lag = ((double) tmp->tv_sec + ((double) tmp->tv_usec / 1000.0)) - i_Ping;
#else
        SYSTEMTIME lpSystemTime;
	GetSystemTime(&lpSystemTime);
	i_Lag=(((double)lpSystemTime.wHour*3600.0)+((double)lpSystemTime.wMinute*60.0)+(double)lpSystemTime.wSecond+((double)lpSystemTime.wMilliseconds/1000.0))-i_Ping;
#endif
	i_Ping = 0.0;
    }
}

double Server::Lag()
{
    NFT("Server::Lag");
    RET(i_Lag);
}

vector<mstring> Server::Downlinks()
{
    NFT("Server::Downlinks");
    vector<mstring> downlinks;
    map<mstring,Server>::iterator serv;

    for(serv=Parent->server.ServerList.begin(); serv!=Parent->server.ServerList.end(); serv++)
    {
	if (serv->second.Uplink() == i_Name)
	    downlinks.push_back(serv->first);
    }
    NRET(vector<mstring>, downlinks);
    
}

vector<mstring> Server::AllDownlinks()
{
    NFT("Server::AllDownlinks");
    vector<mstring> downlinks, uplinks, uplinks2;
    map<mstring,Server>::iterator serv;
    bool found = false;

    for(serv=Parent->server.ServerList.begin(); serv!=Parent->server.ServerList.end(); serv++)
    {
	if (serv->second.Uplink() == i_Name)
	{
	    downlinks.push_back(serv->first);
	    uplinks.push_back(serv->first);
	    found = true;
	}
    }

    while (found == true)
    {
	found = false;
	for (int i=0; i<uplinks.size(); i++) 
	{
	    for(serv=Parent->server.ServerList.begin(); serv!=Parent->server.ServerList.end(); serv++)
	    {
		if (serv->second.Uplink() == uplinks[i])
		{
		    downlinks.push_back(serv->first);
		    uplinks2.push_back(serv->first);
		    found = true;
		}
	    }
	}
	uplinks.clear();
	uplinks = uplinks2;
	uplinks2.clear();
    }

    NRET(vector<mstring>, downlinks);
}

Server::~Server()
{
    NFT("Server::~Server");

    // Take my sublinks with me (who will take theirs ...)
    vector<mstring> Kill = Downlinks();
    for (int i=0; i<Kill.size(); i++)
	Parent->server.ServerList.erase(Kill[i]);
}

NetworkServ::NetworkServ()
{
    NFT("NetworkServ::NetworkServ");
    messages=true;
    automation=true;
}

bool NetworkServ::IsServer(mstring server)
{
    FT("IsServer", (server));
    if (ServerList.empty()) RET(false);
    RET((ServerList.find(server.LowerCase()) != ServerList.end()));
}

void NetworkServ::execute(const mstring & data)
{
    //okay this is the main networkserv command switcher
//    mThread::ReAttach(tt_ServNet);
    FT("NetworkServ::execute", (data));

    mstring source, sourceL, msgtype;
    if (data[0u]==':')
    {
        source=data.ExtractWord(1,": ");
	sourceL=source.LowerCase();
        msgtype=data.ExtractWord(2,": ").UpperCase();
	if (!(Parent->nickserv.IsLive(source) || source.Contains(".")))
	{
		KillUnknownUser(source);
		return;
	}
    }
    else
    {
        msgtype=data.ExtractWord(1,": ").UpperCase();
    }


    // Message names direct from RFC1459, with DAL4.4.15+
    // extensions.  Will add to for other ircd's.

    switch (msgtype[0U])
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
	numeric_execute(data);
	break;
    case 'A':
	if (msgtype=="ADMIN")
	{
	    // :source ADMIN
	    SendSVR("256 " + source + " :Administrative info about " +
		Parent->Startup_SERVER_NAME);
	    SendSVR("257 " + source + " :" + Parent->Startup_SERVER_DESC);
	    SendSVR("258 " + source + " :Admins - " + Parent->operserv.Services_Admin());
	    SendSVR("259 " + source + " :" + FULL_NAME + " - " + FULL_EMAIL);
	}
	else if (msgtype=="AKILL")
	{
	}
	else if (msgtype=="AWAY")
	{
	    // :source AWAY
	    // :source AWAY :This is my reason
	    if (data.ExtractWord(3, ": ")=="")
		Parent->nickserv.live[sourceL].Away("");
	    else
		Parent->nickserv.live[sourceL].Away(data.After(":", 2));
	}
	break;
    case 'B':
	break;
    case 'C':
	if (msgtype=="CONNECT")
	{
	    // :source CONNECT some.server port :our.server
	    Send("NOTICE " + source + " :Connect: Host " +
		data.ExtractWord(3, ": ") + " not listed in irc.conf");
	}
	break;
    case 'D':
	break;
    case 'E':
	if (msgtype=="ERROR")
	{
	    // ERROR :This is my error
	    wxLogWarning("SERVER reported ERROR: %s", data.After(":").c_str());
	}
	break;
    case 'F':
	break;
    case 'G':
	if (msgtype=="GLINE")
	{
	}
	else if (msgtype=="GLOBOPS")
	{
	    // :source GLOBOPS :This message
	}
	else if (msgtype=="GNOTICE")
	{
	    // :server GNOTICE :This message
	}
	else if (msgtype=="GOPER")
	{
	}
	break;
    case 'H':
	break;
    case 'I':
	if (msgtype=="INFO")
	{
	    // :source INFO :server/nick
	    for (int i=0; i<sizeof(credits)/sizeof(mstring); i++)
		SendSVR("371 " + source + " :" + credits[i]);
	    SendSVR("374 " + source + " :End of /INFO report");
	}
	else if (msgtype=="INVITE")
	{
	    // :source INVITE target :channel
	}
	else if (msgtype=="ISON")
	{
	    // repl: :our.server 303 source :local.nick
	}
	break;
    case 'J':
	if (msgtype=="JOIN")
	{
	    // :source JOIN :#channel
	    for (int i=3; i<=data.WordCount(":, "); i++)
		Parent->nickserv.live[sourceL].Join(data.ExtractWord(i, ":, "));
	}
	break;
    case 'K':
	if (msgtype=="KICK")
	{
	    // :source KICK #channel target :reason

	    // KICK for UNKNOWN USER.
	    if (!Parent->nickserv.IsLive(data.ExtractWord(4, ": ")))
	    {
		KillUnknownUser(data.ExtractWord(4, ": "));
		return;
	    }

	    if (!(Parent->nickserv.live[sourceL].IsInChan(data.ExtractWord(3, ": ")) || source.Contains(".")))
		SendSVR("KICK " + data.ExtractWord(3, ": ") + " " + source + " :You are not in this channel");

	    Parent->nickserv.live[data.ExtractWord(4, ": ").LowerCase()].Kick(source, data.ExtractWord(3, ": "));
	}
	else if (msgtype=="KILL")
	{
	    // :source/server KILL target :reason
	    // LOCAL clients ONLY (remotes are a QUIT).
	}
	break;
    case 'L':
	if (msgtype=="LINKS")
	{
	    //:ChanServ LINKS :temple.magick.tm
	    //:temple.magick.tm 364 ChanServ temple.magick.tm temple.magick.tm :0 Magick IRC Services Test Network
	    //:temple.magick.tm 365 ChanServ temple.magick.tm :End of /LINKS list.

	}
	else if (msgtype=="LIST")
	{
	}
	break;
    case 'M':
	if (msgtype=="MODE")
	{
	    // :source MODE source :mode
	    // :source MODE #channel mode params...
	    // :server MODE #channel mode params... creationtime
	}
	break;
    case 'N':
	if (msgtype=="NAMES")
	{
	    // :source NAMES #channel our.server
	    SendSVR("336 " + source + " " + data.ExtractWord(3, ": ") + " :End of /NAMES list.");
	}
	else if (msgtype=="NICK")
	{
	    if (source.IsEmpty()) {
		// NEW USER
	        // hops = servers from us
		// services = 1 for service, 0 for user
	        // DAL4.4.15+ NICK name hops time user host server services :real name
		Parent->nickserv.live[data.ExtractWord(2, ": ").LowerCase()] =
		    Nick_Live_t(
			data.ExtractWord(2, ": "),
//			(time_t) atof(data.ExtractWord(4, ": ")),
			Now(),
			data.ExtractWord(7, ": "),
			data.ExtractWord(5, ": "),
			data.ExtractWord(6, ": "),
			data.After(":")
		    );

	    }
	    else
	    {
		// CHANGE NICK
		if (!Parent->nickserv.IsLive(data.ExtractWord(3, ": ").LowerCase()))
		{
		    Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()] =
			Parent->nickserv.live[sourceL];

		    Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].
			Name(data.ExtractWord(3, ": "));
		}
		else
		{
		    KillUnknownUser(data.ExtractWord(3, ": "));
		}

		Parent->nickserv.live.erase(sourceL);
	    }
	}
	else if (msgtype=="NOTICE")
	{
	    // :source NOTICE target/#channel :message
	    // NOTICE target :message
	}
	break;
    case 'O':
	if (msgtype=="OPER")
	{
	}
	break;
    case 'P':
	if (msgtype=="PART")
	{
	    // :source PART #channel :reason
	    Parent->nickserv.live[sourceL].Part(data.ExtractWord(3, ": "));
	}
	else if (msgtype=="PASS")
	{
	    // PASS :password
	    if (data.ExtractWord(2, ": ") != Parent->Startup_PASSWORD)
	    {
		CP(("Server password mismatch.  Closing socket."));
		Send("ERROR :No Access (passwd mismatch) [" + Parent->Startup_REMOTE_SERVER + "]");
		Send("ERROR :Closing Link: [" + Parent->Startup_REMOTE_SERVER + "] (Bad Password)");

		Parent->reconnect=false;
		Parent->ircsvchandler->shutdown();
	    }
	}
	else if (msgtype=="PING")
	{
	    // PING :some.server
	    // :some.server PING some.server :our.server
	    if (source)
		SendSVR("PONG " + Parent->Startup_SERVER_NAME + " :" + source);
	    else
		SendSVR("PONG " + Parent->Startup_SERVER_NAME + " :" +
			data.ExtractWord(2, ": "));
	}
	else if (msgtype=="PONG")
	{
	    // Receive info back for LAG MONITOR.
	    // we'll send out ':our.server PING our.server :remote.server'
	    // for EACH server, at the same time execute Ping() on it.

	    // :server PONG server :our.server
	    if (IsServer(source))
		ServerList[sourceL].Pong();

	}
	else if (msgtype=="PRIVMSG")
	{
	    // :source PRIVMSG target/#channel :message
	    /*
	    if (!IsChan(data.ExtractWord(3, ": ")) 
		wxLogWarning("Received message for unknown user " + data.ExtractWord(3, ": "));
	    */
	}
	break;
    case 'Q':
	if (msgtype=="QUIT")
	{
	    // :source QUIT :reason

	    // TODO: Call stored.Quit(reason) for live.IsRecognized().
	    Parent->nickserv.live.erase(sourceL);
	}
	break;
    case 'R':
	if (msgtype=="RAKILL")
	{
	}
	else if (msgtype=="REHASH")
	{
	}
	else if (msgtype=="RESTART")
	{
	}
	break;
    case 'S':
	if (msgtype=="SERVER")
	{
	    // SERVER downlink hops :description
	    // :uplink SERVER downlink hops :description
	    if (source.IsEmpty())
	    {
		ServerList[data.ExtractWord(2, ": ").LowerCase()] = Server(
			data.ExtractWord(2, ": ").LowerCase(),
			atoi(data.ExtractWord(3, ": ").LowerCase().c_str()),
			data.After(":"));
	    }
	    else
	    {
		if (IsServer(sourceL))
		{
		    ServerList[data.ExtractWord(3, ": ").LowerCase()] = Server(
			data.ExtractWord(3, ": ").LowerCase(),
			sourceL,
			atoi(data.ExtractWord(4, ": ").LowerCase().c_str()),
			data.After(":", 2));
		}
		else
		{
		    // Uplink (source) does not exist, WTF?!?
		}
	    }
	}
	else if (msgtype=="SQLINE")
	{
	}
	else if (msgtype=="SQUIT")
	{
	    // SQUIT shadow.darker.net :soul.darker.net lifestone.darker.net
	    // SQUIT lifestone.darker.net :Ping timeout
	    ServerList.erase(data.ExtractWord(2, ": ").LowerCase());
	}
	else if (msgtype=="STATS")
	{
	    // :source STATS type :our.server
	    //:temple.magick.tm 219 ChanServ o :End of /STATS report
	    SendSVR("219 " + source + " " + data.ExtractWord(3, ": ") +
		" :End of /STATS report");
	}
	else if (msgtype=="SUMMON")
	{
	    // :source SUMMON user our.server *
	    SendSVR("445 " + source + " :SUMMON has been disabled");
	}
	else if (msgtype=="SVSKILL")
	{
	}
	else if (msgtype=="SVSMODE")
	{
	}
	else if (msgtype=="SVSMODE")
	{
	}
	else if (msgtype=="SVSNICK")
	{
	}
	else if (msgtype=="SVSNOOP")
	{
	}
	break;
    case 'T':
	if (msgtype=="TIME")
	{
	    // :source TIME :our.server
	    SendSVR("391 " + source + " :" + Now().DateTimeString());
	}
	else if (msgtype=="TOPIC")
	{
	    // :server/user TOPIC #channel setter time :topic
	    // :server/user TOPIC #channel setter
	    // TIME is not standard (time is optional);
	    if (Parent->chanserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		if (data.ExtractWord(5, ": ") != "")
		{ // Setting
		    Parent->chanserv.live[data.ExtractWord(3, ": ").LowerCase()].Topic(
		        data.After(":", 2),
		        data.ExtractWord(4, ": "),
		        (time_t) atol(data.ExtractWord(5, ": "))
		    );
		}
		else
		{ // Clearing
		    Parent->chanserv.live[data.ExtractWord(3, ": ").LowerCase()].Topic(
		        "",
		        data.ExtractWord(4, ": "),
		        Now()
		    );
		}
	    }
	}
	else if (msgtype=="TRACE")
	{
	    // :source TRACE :server/target
	    // repl:
	    //:ChanServ TRACE temple.magick.tm
	    //:temple.magick.tm 206 ChanServ Server 0 0S 0C temple.magick.tm *!*@temple.magick.tm 1313208
	    //:ChanServ TRACE PreZ
	    //:temple.magick.tm 206 ChanServ Server 0 0S 0C temple.magick.tm *!*@temple.magick.tm 1313208

	}
	break;
    case 'U':
	if (msgtype=="UNGLINE")
	{
	}
	else if (msgtype=="UNSQLINE")
	{
	}
	else if (msgtype=="USER")
	{
	}
	else if (msgtype=="USERHOST")
	{
	}
	else if (msgtype=="USERS")
	{
	    // :source USERS :our.server
	    SendSVR("446 " + source + " :USERS has been disabled");
	}
	break;
    case 'V':
	if (msgtype=="VERSION")
	{
	    // :source VERSION :our.server
	    //:temple.magick.tm 351 ChanServ dal4.4.17. temple.magick.tm :AiMnW
	    mstring tmp;
	    SendSVR("351 " + source + " " + PRODUCT + " " + Parent->Startup_SERVER_NAME +
			" :" + VERSION + ((RELEASE != "") ? ("-" + RELEASE).c_str() : "") +
			((PATCH1 != "") ? ("+" + PATCH1).c_str() : "") +
			((PATCH2 != "") ? ("+" + PATCH2).c_str() : "") +
			((PATCH3 != "") ? ("+" + PATCH3).c_str() : "") +
			((PATCH4 != "") ? ("+" + PATCH4).c_str() : "") +
			((PATCH5 != "") ? ("+" + PATCH5).c_str() : "") +
			((PATCH6 != "") ? ("+" + PATCH6).c_str() : "") +
			((PATCH7 != "") ? ("+" + PATCH7).c_str() : "") +
			((PATCH8 != "") ? ("+" + PATCH8).c_str() : "") +
			((PATCH9 != "") ? ("+" + PATCH9).c_str() : "") + " (" +
			((Parent->operserv.getnames() != "")	? "O" : "o") +
			((Parent->operserv.getnames() != "" &&
				Parent->operserv.Akill())	? "A" : "a") +
			((Parent->operserv.getnames() != "" &&
				Parent->operserv.Flood())	? "F" : "f") +
			((Parent->operserv.getnames() != "" &&
				Parent->operserv.OperDeny())	? "D" : "d") +
			((Parent->nickserv.getnames() != "")	? "N" : "n") +
			((Parent->chanserv.getnames() != "")	? "C" : "c") +
			((Parent->memoserv.getnames() != "" &&
				Parent->memoserv.Memo())	? "M" : "m") +
			((Parent->memoserv.getnames() != "" &&
				Parent->memoserv.News())	? "W" : "w") +
			((Parent->helpserv.getnames() != "")	? "H" : "h") +
			(Parent->Services_SHOWSYNC		? "Y" : "y") +
			(tmp << Parent->Startup_LEVEL) + ")");
	}
	break;
    case 'W':
	if (msgtype=="WALLOPS")
	{
	    // :source WALLOPS :text
	}
	else if (msgtype=="WHO")
	{
	}
	else if (msgtype=="WHOIS")
	{
	    // :source WHOIS target :our.server
	}
	else if (msgtype=="WHOWAS")
	{
	}
	break;
    case 'X':
	break;
    case 'Y':
	break;
    case 'Z':
	break;
    default:
	break;
    }

//    mThread::ReAttach(tt_mBase);   
}

void NetworkServ::numeric_execute(const mstring & data)
{
    FT("NetworkServ::numeric_execute", (data));

    mstring source, sourceL;
    unsigned int msgtype;
    if (data[0u]==':')
    {
        source=data.ExtractWord(1,": ");
	sourceL=source.LowerCase();
        msgtype=atoi(data.ExtractWord(2,": "));
	if (!Parent->nickserv.IsLive(source))
	    KillUnknownUser(source);
    }
    else
    {
        msgtype=atoi(data.ExtractWord(1,": "));
    }

    // Numerics direct from RFC1459
    // MOST we can (and will) ignore.

    switch (msgtype)
    {
    case 200:     // RPL_TRACELINK
	break;
    case 201:     // RPL_TRACECONNECTING
	break;
    case 202:     // RPL_TRACEHANDSHAKE
	break;
    case 203:     // RPL_TRACEUNKNOWN
	break;
    case 204:     // RPL_TRACEOPERATOR
	break;
    case 205:     // RPL_TRACEUSER
	break;
    case 206:     // RPL_TRACESERVER
	break;
    case 208:     // RPL_TRACENEWTYPE
	break;
    case 211:     // RPL_STATSLINKINFO
	break;
    case 212:     // RPL_STATSCOMMANDS
	break;
    case 213:     // RPL_STATSCLINE
	break;
    case 214:     // RPL_STATSNLINE
	break;
    case 215:     // RPL_STATSILINE
	break;
    case 216:     // RPL_STATSKLINE
	break;
    case 218:     // RPL_STATSYLINE
	break;
    case 219:     // RPL_ENDOFSTATS
	break;
    case 221:     // RPL_UMODEIS
	break;
    case 241:     // RPL_STATSLLINE
	break;
    case 242:     // RPL_STATSUPTIME
	break;
    case 243:     // RPL_STATSOLINE
	break;
    case 244:     // RPL_STATSHLINE
	break;
    case 251:     // RPL_LUSERCLIENT
	break;
    case 252:     // RPL_LUSEROP
	break;
    case 253:     // RPL_LUSERUNKNOWN
	break;
    case 254:     // RPL_LUSERCHANNELS
	break;
    case 255:     // RPL_LUSERME
	break;
    case 256:     // RPL_ADMINME
	break;
    case 257:     // RPL_ADMINLOC1
	break;
    case 258:     // RPL_ADMINLOC2
	break;
    case 259:     // RPL_ADMINEMAIL
	break;
    case 261:     // RPL_TRACELOG
	break;
    case 300:     // RPL_NONE
	break;
    case 301:     // RPL_AWAY
	break;
    case 302:     // RPL_USERHOST
	break;
    case 303:     // RPL_ISON
	break;
    case 305:     // RPL_UNAWAY
	break;
    case 306:     // RPL_NOWAWAY
	break;
    case 311:     // RPL_WHOISUSER
	break;
    case 312:     // RPL_WHOISSERVER
	break;
    case 313:     // RPL_WHOISOPERATOR
	break;
    case 314:     // RPL_WHOWASUSER
	break;
    case 315:     // RPL_ENDOFWHO
	break;
    case 317:     // RPL_WHOISIDLE
	break;
    case 318:     // RPL_ENDOFWHOIS
	break;
    case 319:     // RPL_WHOISCHANNELS
	break;
    case 321:     // RPL_LISTSTART
	break;
    case 322:     // RPL_LIST
	break;
    case 323:     // RPL_LISTEND
	break;
    case 324:     // RPL_CHANNELMODEIS
	break;
    case 331:     // RPL_NOTOPIC
	break;
    case 332:     // RPL_TOPIC
	break;
    case 341:     // RPL_INVITING
	break;
    case 342:     // RPL_SUMMONING
	break;
    case 351:     // RPL_VERSION
	break;
    case 352:     // RPL_WHOREPLY
	break;
    case 353:     // RPL_NAMREPLY
	break;
    case 364:     // RPL_LINKS
	break;
    case 365:     // RPL_ENDOFLINKS
	break;
    case 366:     // RPL_ENDOFNAMES
	break;
    case 367:     // RPL_BANLIST
	break;
    case 368:     // RPL_ENDOFBANLIST
	break;
    case 369:     // RPL_ENDOFWHOWAS
	break;
    case 371:     // RPL_INFO
	break;
    case 372:     // RPL_MOTD
	break;
    case 374:     // RPL_ENDOFINFO
	break;
    case 375:     // RPL_MOTDSTART
	break;
    case 376:     // RPL_ENDOFMOTD
	break;
    case 381:     // RPL_YOUREOPER
	break;
    case 382:     // RPL_REHASHING
	break;
    case 391:     // RPL_TIME
	break;
    case 392:     // RPL_USERSSTART
	break;
    case 393:     // RPL_USERS
	break;
    case 394:     // RPL_ENDOFUSERS
	break;
    case 395:     // RPL_NOUSERS
	break;
    case 401:     // ERR_NOSUCHNICK
	break;
    case 402:     // ERR_NOSUCHSERVER
	break;
    case 403:     // ERR_NOSUCHCHANNEL
	break;
    case 404:     // ERR_CANNOTSENDTOCHAN
	break;
    case 405:     // ERR_TOOMANYCHANNELS
	break;
    case 406:     // ERR_WASNOSUCHNICK
	break;
    case 407:     // ERR_TOOMANYTARGETS
	break;
    case 409:     // ERR_NOORIGIN
	break;
    case 411:     // ERR_NORECIPIENT
	break;
    case 412:     // ERR_NOTEXTTOSEND
	break;
    case 413:     // ERR_NOTOPLEVEL
	break;
    case 414:     // ERR_WILDTOPLEVEL
	break;
    case 421:     // ERR_UNKNOWNCOMMAND
	break;
    case 422:     // ERR_NOMOTD
	break;
    case 423:     // ERR_NOADMININFO
	break;
    case 424:     // ERR_FILEERROR
	break;
    case 431:     // ERR_NONICKNAMEGIVEN
	break;
    case 432:     // ERR_ERRONEUSNICKNAME
	break;
    case 433:     // ERR_NICKNAMEINUSE
	break;
    case 436:     // ERR_NICKCOLLISION
	// MUST handle.
	break;
    case 441:     // ERR_USERNOTINCHANNEL
	break;
    case 442:     // ERR_NOTONCHANNEL
	break;
    case 443:     // ERR_USERONCHANNEL
	break;
    case 444:     // ERR_NOLOGIN
	break;
    case 445:     // ERR_SUMMONDISABLED
	break;
    case 446:     // ERR_USERSDISABLED
	break;
    case 451:     // ERR_NOTREGISTERED
	break;
    case 461:     // ERR_NEEDMOREPARAMS
	break;
    case 462:     // ERR_ALREADYREGISTRED
	break;
    case 463:     // ERR_NOPERMFORHOST
	break;
    case 464:     // ERR_PASSWDMISMATCH
	// MUST handle (Stop connecting).
	break;
    case 465:     // ERR_YOUREBANNEDCREEP
	// MUST handle (Stop connecting).
	break;
    case 467:     // ERR_KEYSET
	break;
    case 471:     // ERR_CHANNELISFULL
	break;
    case 472:     // ERR_UNKNOWNMODE
	break;
    case 473:     // ERR_INVITEONLYCHAN
	break;
    case 474:     // ERR_BANNEDFROMCHAN
	break;
    case 475:     // ERR_BADCHANNELKEY
	break;
    case 481:     // ERR_NOPRIVILEGES
	break;
    case 482:     // ERR_CHANOPRIVSNEEDED
	break;
    case 483:     // ERR_CANTKILLSERVER
	break;
    case 491:     // ERR_NOOPERHOST
	break;
    case 501:     // ERR_UMODEUNKNOWNFLAG
	break;
    case 502:     // ERR_USERSDONTMATCH
	break;
    default:
	break;
    }
}
