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
	i_Ping = ACE_OS::gettimeofday().msec();
   }
}

void Server::Pong()
{
    NFT("Server::Pong");
    if (i_Ping)
    {
	i_Lag = ACE_OS::gettimeofday().msec() - i_Ping;
	COM(("The lag time of %s is %3.3f seconds.", i_Name.c_str(), i_Lag / 1000.0));
	i_Ping = 0;
    }
}

double Server::Lag()
{
    NFT("Server::Lag");
    RET(i_Lag / 1000.0);
}

vector<mstring> Server::Downlinks()
{
    NFT("Server::Downlinks");
    vector<mstring> downlinks;
    map<mstring,Server>::iterator serv;

    for(serv=Parent->server.ServerList.begin(); serv!=Parent->server.ServerList.end(); serv++)
    {
	if (serv->second.Uplink() == i_Name && i_Name != "")
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
    COM(("Destroying %d more servers", Kill.size()));
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
    FT("NetworkServ::IsServer", (server));
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
	    // We will ignore AKILLS because they're not relivant to us.
	    // we will not be akilling our own clients ;P
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
	    // :soul.darker.net 481 ChanServ :Permission Denied- You do not have the correct IRC operator privileges


	    // :source CONNECT some.server port :our.server
	    if (IsServer(data.ExtractWord(3, ": ")) ||
	    	data.ExtractWord(3, ": ").LowerCase() == Parent->Startup_SERVER_NAME.LowerCase())
	    {
		SendSVR("NOTICE " + source + " :Connect: Server " + data.ExtractWord(3, ": ") +
			" already exists from " + Parent->Startup_SERVER_NAME);
	    }
	    else
	    {
		Send("NOTICE " + source + " :Connect: Host " +
			data.ExtractWord(3, ": ") + " not listed in irc.conf");
	    }
	}
	break;
    case 'D':
	break;
    case 'E':
	if (msgtype=="ERROR")
	{
	    // ERROR :This is my error
	    wxLogNotice("SERVER reported ERROR: %s", data.After(":").c_str());
	}
	break;
    case 'F':
	break;
    case 'G':
	if (msgtype=="GLINE")
	{
	    // We will ignore GLINES because they're not relivant to us.
	    // we will not be glining our own clients ;P
	}
	else if (msgtype=="GLOBOPS")
	{
	    // :source GLOBOPS :This message
	    // useless chatter ... can be ignored.
	}
	else if (msgtype=="GNOTICE")
	{
	    // :server GNOTICE :This message
	    wxLogInfo("SERVER MESSAGE: %s", data.After(":").c_str());
	}
	else if (msgtype=="GOPER")
	{
	    // useless chatter ... can be ignored.
	}
	break;
    case 'H':
	break;
    case 'I':
	if (msgtype=="INFO")
	{
	    // :source INFO :server/nick
	    for (int i=0; i<sizeof(contrib)/sizeof(mstring); i++)
		SendSVR("371 " + source + " :" + contrib[i]);
	    SendSVR("374 " + source + " :End of /INFO report");
	}
	else if (msgtype=="INVITE")
	{
	    // :source INVITE target :channel
	    //:PreZ INVITE ChanServ :#chatzone

	    // We can ignore this, as our clients will not 'join on invite'

	}
	else if (msgtype=="ISON")
	{
	    // repl: :our.server 303 source :local.nick
	    if (Parent->nickserv.IsLive(source))
		SendSVR("303 " + source + " :" + data.ExtractWord(3, ": "));
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

	    // NOTE: as the message has already been broadcasted,
	    // we still need to acomodate for it.
	    Parent->nickserv.live[data.ExtractWord(4, ": ").LowerCase()].Kick(source, data.ExtractWord(3, ": "));
	}
	else if (msgtype=="KILL")
	{
	    // :source/server KILL target :reason
	    // LOCAL clients ONLY (remotes are a QUIT).
	    //:PreZ KILL kick`kill`abuse :castle.srealm.net.au!PreZ (blah)
	    if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		int wc = data.After(":", 2).WordCount("!");
		Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].Quit(
			"Killed (" + data.After(":", 2).After("!", wc) + ")");
		Parent->nickserv.live.erase(data.ExtractWord(3, ": ").LowerCase());
	    }
	    else
	    {
		wxLogWarning("Received KILL message for user %s who does not exist, from %s",
			data.ExtractWord(3, ": ").c_str(), source.c_str());
	    }

	}
	break;
    case 'L':
	if (msgtype=="LINKS")
	{
	    //:ChanServ LINKS :temple.magick.tm
	    //:temple.magick.tm 364 ChanServ temple.magick.tm temple.magick.tm :0 Magick IRC Services Test Network
	    //:temple.magick.tm 365 ChanServ temple.magick.tm :End of /LINKS list.
	    SendSVR("364 " + source + " " + Parent->Startup_SERVER_NAME + " " +
		Parent->Startup_SERVER_NAME + " :0 " + Parent->Startup_SERVER_DESC);

	    map<mstring,Server>::iterator serv;
	    for(serv=Parent->server.ServerList.begin(); serv!=Parent->server.ServerList.end(); serv++)
	    {
		SendSVR("364 " + source + " " + serv->second.Name() + " " + serv->second.Uplink()
			+ " :" + serv->second.Hops() + " " + serv->second.Description());
	    }

	    SendSVR("365 " + source + " " + Parent->Startup_SERVER_NAME + " :End of /LINKS list.");

	}
	else if (msgtype=="LIST")
	{
	    SendSVR("321 " + source + " Channel :Users  Name");

	    map<mstring,Chan_Live_t>::iterator chan;
	    for (chan=Parent->chanserv.live.begin(); chan!=Parent->chanserv.live.end(); chan++)
	    {
		SendSVR("322 " + source + " " + chan->first + " " + mstring(chan->second.Users()) + 
		    " :" + chan->second.Topic());
	    }

	    SendSVR("323 " + source + " :End of /LIST");
	}
	break;
    case 'M':
	if (msgtype=="MODE")
	{
	    // :source MODE source :mode
	    // :source MODE #channel mode params...
	    // :server MODE #channel mode params... creationtime
	    if (IsChan(data.ExtractWord(3, ": ")))
	    {
		if (Parent->chanserv.IsLive(data.ExtractWord(3, ": ")))
		{
		    Parent->chanserv.live[data.ExtractWord(3, ": ").LowerCase()].Mode(source, data.After(" ", 3));
		}
		else
		{
		    wxLogWarning("MODE from %s received for non-existant channel %s", source.c_str(),
			data.ExtractWord(3, ": ").c_str());
		}
	    }
	    else
	    {
		if (source!=data.ExtractWord(3, ": "))
		{
		    wxLogWarning("MODE for another nick received from %s for %s",
			source.c_str(), data.ExtractWord(3, ": ").c_str());
		}
		else
		{
		    Parent->nickserv.live[sourceL].Mode(data.ExtractWord(4, ": "));
		}
	    }
	}
	break;
    case 'N':
	if (msgtype=="NAMES")
	{
	    // :source NAMES #channel our.server
//:soul.darker.net 353 ChanServ = #chatzone :killkickabuseme @Aimee Jupiter @Allanon Ghost_ wildrose
//:soul.darker.net 366 ChanServ #chatzone :End of /NAMES list.
	    SendSVR("366 " + source + " " + data.ExtractWord(3, ": ") + " :End of /NAMES list.");

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

	    Parent->nickserv.live[sourceL].Quit(data.After(":", 2));
	    Parent->nickserv.live.erase(sourceL);
	}
	break;
    case 'R':
	if (msgtype=="RAKILL")
	{
	    // We will ignore AKILLS because they're not relivant to us.
	    // we will not be akilling our own clients ;P
	}
	else if (msgtype=="REHASH")
	{
	    // Will we ever get this via. net??
	}
	else if (msgtype=="RESTART")
	{
	    // Will we ever get this via. net??
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
	    // We will ignore SQLINES because they're not relivant to us.
	    // we will not be qlining our own clients ;P
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

//:ChanServ TRACE Aimee
//:soul.darker.net 200 ChanServ Link Aurora2.2-r Aimee lifestone.darker.net
//:lifestone.darker.net 200 ChanServ Link Aurora2.2-r Aimee :requiem.darker.net
//:requiem.darker.net 204 ChanServ Operator 10 Aimee[ABD2CEFD.ipt.aol.com] :128

//:ChanServ TRACE vampire
//:soul.darker.net 200 ChanServ Link Aurora2.2-r vampire.darker.net lifestone.darker.net
//:lifestone.darker.net 200 ChanServ Link Aurora2.2-r vampire.darker.net :vampire.darker.net
//:vampire.darker.net 206 ChanServ Server 50 6S 19C lifestone.darker.net[0.0.0.0] AutoConn.!*@vampire.darker.net :0
//:vampire.darker.net 204 ChanServ Operator 10 Alien[pc134.net19.ktv.koping.se] :64
//:vampire.darker.net 205 ChanServ User 1 Nadu[bservice.org] :35
//:vampire.darker.net 205 ChanServ User 1 tomaway[adsl1453.turboline.be] :106
//:vampire.darker.net 205 ChanServ User 1 Wau|oK[axe.net.au] :46
//:vampire.darker.net 205 ChanServ User 1 Ghost_[203.58.108.129] :15
//:vampire.darker.net 205 ChanServ User 1 wildrose[cr1003083-a.crdva1.bc.wave.home.com] :48
//:vampire.darker.net 205 ChanServ User 1 Jupiter[modem035.memnoch.comcen.com.au] :76
//:vampire.darker.net 209 ChanServ Class 50 :1
//:vampire.darker.net 209 ChanServ Class 10 :1
//:vampire.darker.net 209 ChanServ Class 1 :7



	}
	break;
    case 'U':
	if (msgtype=="UNGLINE")
	{
	    // We will ignore GLINES because they're not relivant to us.
	    // we will not be glining our own clients ;P
	}
	else if (msgtype=="UNSQLINE")
	{
	    // We will ignore SQLINES because they're not relivant to us.
	    // we will not be qlining our own clients ;P
	}
	else if (msgtype=="USER")
	{
	}
	else if (msgtype=="USERHOST")
	{
	    if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		SendSVR("302 " + source + " :" +
			Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].Name() +
			"*=-" +
			Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].UserName() +
			"@" +
			Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].HostName());

	    }
	    else
	    {
		SendSVR("461 " + source + " USERHOST :Not enough paramaters");
	    }

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
	    // useless chatter ... can be ignored.
	}
	else if (msgtype=="WHO")
	{
//:ChanServ WHO
//:soul.darker.net 352 ChanServ #chatzone ~striker syd-56K-162.tpgi.com.au requiem.darker.net killkickabuseme H :2 Pain looks good on other people...
//:soul.darker.net 352 ChanServ * blah darker.net heaven.darker.net ChanServ H* :1 <bad-realname>
//:soul.darker.net 352 ChanServ #operzone ~somebody ABD2CEFD.ipt.aol.com requiem.darker.net Aimee H+ :2 anybody
//:soul.darker.net 352 ChanServ #chatzone jupiter modem035.memnoch.comcen.com.au vampire.darker.net Jupiter G :2 Jupiter
//:soul.darker.net 352 ChanServ * jason axe.net.au shadow.darker.net WauloK G :2 Nothing really matters..
//:soul.darker.net 352 ChanServ #chatzone allanon indra.darkshadow.net shadow.darker.net Allanon H*@ :2 God..
//:soul.darker.net 352 ChanServ #chatzone roamer shadow.darker.net shadow.darker.net FastinI G% :2 *
//:soul.darker.net 352 ChanServ * Tico-Rulez adsl1453.turboline.be vampire.darker.net tomaway H% :2 Captain Spartan, Dead Man Walking
//:soul.darker.net 352 ChanServ #darkernet ~ghost 203.58.108.129 vampire.darker.net Ghost_ G :2 fuck off
//:soul.darker.net 352 ChanServ #bothouse ~Nadu bservice.org vampire.darker.net Nadu H@ :2 BService Bot - #Bothouse - Owner: nicki
//:soul.darker.net 352 ChanServ #chatzone wildinbed cr1003083-a.crdva1.bc.wave.home.com vampire.darker.net wildrose H :2 Prairie Flower
//:soul.darker.net 352 ChanServ #operzone ~striker syd-56K-162.tpgi.com.au requiem.darker.net Lord_Striker G*@ :2 Pain looks good on other people...
//:soul.darker.net 352 ChanServ #chatzone ~jason axe.net.au vampire.darker.net Wau|oK G%@ :2 Nothing really matters..
//:soul.darker.net 352 ChanServ #operzone satan680 pc134.net19.ktv.koping.se vampire.darker.net Alien G*@ :2 am I GOD ?
//:soul.darker.net 352 ChanServ #chatzone prez castle.srealm.net.au soul.darker.net PreZ G*@ :0 I am what the people fear, I need not fear them.
//:soul.darker.net 352 ChanServ * ~bo pc34.net20.ktv.koping.se requiem.darker.net DarkMagic G% :2 Dark not Black
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net Magick-1 H% :2 Magick Outlet
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net Death H* :2 Global Noticer
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net OperServ H% :2 Operator Server
//:soul.darker.net 352 ChanServ #complaints reaper darker.net hell.darker.net DevNull H% :2 /dev/null -- message sink
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net MemoServ H :2 Memo Server
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net IrcIIHelp H :2 ircII Help Server
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net HelpServ H :2 Help Server
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net NickServ H :2 Nickname Server
//:soul.darker.net 352 ChanServ #magick prez castle.srealm.net.au soul.darker.net DarkMgk H%@ :0 Magick BridgeBot
//:soul.darker.net 315 ChanServ * :End of /WHO list.

//:ChanServ WHO PreZ
//:soul.darker.net 352 ChanServ #chatzone prez castle.srealm.net.au soul.darker.net PreZ G*@ :0 I am what the people fear, I need not fear them.
//:soul.darker.net 352 ChanServ #magick prez castle.srealm.net.au soul.darker.net DarkMgk H%@ :0 Magick BridgeBot
//:soul.darker.net 315 ChanServ PreZ :End of /WHO list.

//:ChanServ WHO #chatzone
//:soul.darker.net 352 ChanServ #chatzone ~striker syd-56K-162.tpgi.com.au requiem.darker.net killkickabuseme H :2 Pain looks good on other people...
//:soul.darker.net 352 ChanServ #chatzone ~somebody ABD2CEFD.ipt.aol.com requiem.darker.net Aimee H@ :2 anybody
//:soul.darker.net 352 ChanServ #chatzone jupiter modem035.memnoch.comcen.com.au vampire.darker.net Jupiter G :2 Jupiter
//:soul.darker.net 352 ChanServ #chatzone allanon indra.darkshadow.net shadow.darker.net Allanon H*@ :2 God..
//:soul.darker.net 352 ChanServ #chatzone ~ghost 203.58.108.129 vampire.darker.net Ghost_ G :2 fuck off
//:soul.darker.net 352 ChanServ #chatzone wildinbed cr1003083-a.crdva1.bc.wave.home.com vampire.darker.net wildrose H :2 Prairie Flower
//:soul.darker.net 315 ChanServ #chatzone :End of /WHO list.

//:ChanServ WHO vampire.darker.net
//:soul.darker.net 352 ChanServ #chatzone jupiter modem035.memnoch.comcen.com.au vampire.darker.net Jupiter G :2 Jupiter
//:soul.darker.net 352 ChanServ * Tico-Rulez adsl1453.turboline.be vampire.darker.net tomaway H% :2 Captain Spartan, Dead Man Walking
//:soul.darker.net 352 ChanServ #darkernet ~ghost 203.58.108.129 vampire.darker.net Ghost_ G :2 fuck off
//:soul.darker.net 352 ChanServ #bothouse ~Nadu bservice.org vampire.darker.net Nadu H@ :2 BService Bot - #Bothouse - Owner: nicki
//:soul.darker.net 352 ChanServ #chatzone wildinbed cr1003083-a.crdva1.bc.wave.home.com vampire.darker.net wildrose H :2 Prairie Flower
//:soul.darker.net 352 ChanServ #chatzone ~jason axe.net.au vampire.darker.net Wau|oK G%@ :2 Nothing really matters..
//:soul.darker.net 352 ChanServ #operzone satan680 pc134.net19.ktv.koping.se vampire.darker.net Alien G*@ :2 am I GOD ?
//:soul.darker.net 315 ChanServ vampire.darker.net :End of /WHO list.

	}
	else if (msgtype=="WHOIS")
	{
	    // :source WHOIS target :target
//:ChanServ WHOIS PreZ :PreZ
//:soul.darker.net 311 ChanServ PreZ prez castle.srealm.net.au * :I am what the people fear, I need not fear them.
//:soul.darker.net 307 ChanServ PreZ :is a registered nick
//:soul.darker.net 319 ChanServ PreZ :#chatzone #darkernet @#mrcoffee #pagan @#wicca
//:soul.darker.net 312 ChanServ PreZ soul.darker.net :Show me a dream .. I'll show you my nightmare
//:soul.darker.net 301 ChanServ PreZ :Automatically set away [SZon]  Away since Sun Jul 18 20:15
//:soul.darker.net 313 ChanServ PreZ :is an IRC Operator
//:soul.darker.net 310 ChanServ PreZ :looks very helpful.
//:soul.darker.net 317 ChanServ PreZ 557 932291863 :seconds idle, signon time
//:soul.darker.net 318 ChanServ PreZ :End of /WHOIS list.

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
