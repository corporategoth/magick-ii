#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#pragma implementation "version.h"
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
** Revision 1.103  2000/06/11 09:30:21  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.102  2000/06/10 07:01:04  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.101  2000/06/08 13:07:34  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.100  2000/06/06 08:57:58  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.99  2000/05/27 07:06:03  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.98  2000/05/22 13:00:10  prez
** Updated version.h and some other stuff
**
** Revision 1.97  2000/05/21 04:49:40  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.96  2000/05/20 03:28:12  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.95  2000/05/19 10:48:15  prez
** Finalized the DCC Sending (now uses the Action map properly)
**
** Revision 1.94  2000/05/18 10:13:15  prez
** Finished off the mFile structure, and the DCC system, it all works.
**
** Revision 1.93  2000/05/10 11:47:00  prez
** added back memo timers
**
** Revision 1.92  2000/04/30 03:48:30  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.91  2000/04/23 02:31:50  prez
** *** empty log message ***
**
** Revision 1.90  2000/04/18 14:34:23  prez
** Fixed the HELP system, it now loads into memory, and can be unloaded
** with the OS unload command.  The stats however are inaccurate.
**
** Revision 1.89  2000/04/06 12:52:50  prez
** Various code changes, but mainly added AUTOMAKE/AUTOCONF files :)
**
** Revision 1.88  2000/04/04 03:21:36  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.87  2000/04/04 03:13:51  prez
** Added support for masking hostnames.
**
** Revision 1.86  2000/04/03 09:45:24  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.85  2000/04/02 13:06:04  prez
** Fixed the channel TOPIC and MODE LOCK stuff ...
**
** Also fixed the setting of both on join...
**
** Revision 1.84  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.83  2000/03/27 21:26:13  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.82  2000/03/26 14:59:37  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.81  2000/03/24 15:35:18  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.80  2000/03/19 08:50:56  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.79  2000/03/15 14:42:59  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.78  2000/03/15 08:23:52  prez
** Added locking stuff for commserv options, and other stuff
**
** Revision 1.77  2000/03/14 15:10:16  prez
** OK -- more stuff with SJOIN/SNICK -- but it WORKS!!!
**
** Revision 1.76  2000/03/14 13:36:46  prez
** Finished P12 compliance (SJOIN, etc).
**
** Revision 1.75  2000/03/14 10:05:17  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.74  2000/03/14 10:02:48  prez
** Added SJOIN and SNICK
**
** Revision 1.73  2000/03/13 09:36:18  prez
** Completed help file, we now have full help text.
**
** Revision 1.72  2000/03/08 23:38:37  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.71  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.70  2000/02/23 12:21:04  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.69  2000/02/17 12:55:07  ungod
** still working on borlandization
**
** Revision 1.68  2000/02/16 12:59:40  ungod
** fixing for borland compilability
**
** Revision 1.67  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.66  2000/02/15 10:37:50  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "server.h"
#include "lockable.h"
#include "magick.h"

Protocol::Protocol()
{
    NFT("Protocol::Protocol");
    i_Number = 0;
    i_NickLen = 9;
    i_MaxLine = 450;
    i_Globops = false;
    i_Tokens = false;
    i_SVS = false;
    i_SVSHOST = false;
    i_P12 = false;
    i_Akill = 0;
    i_Signon = 0000;
    i_Modes = 3;
    i_Server = "SERVER %s %d :%s";
    i_Protoctl = "";

    tokens["!"] = "PRIVMSG";
    tokens["\\"] = "WHO";
    tokens["#"] = "WHOIS";
    tokens["$"] = "WHOWAS";
    tokens["%"] = "USER";
    tokens["&"] = "NICK";
    tokens["'"] = "SERVER";
    tokens["("] = "LIST";
    tokens[")"] = "TOPIC";
    tokens["*"] = "INVITE";
    tokens["+"] = "VERSION";
    tokens[","] = "QUIT";
    tokens["-"] = "SQUIT";
    tokens["."] = "KILL";
    tokens["/"] = "INFO";
    tokens["0"] = "LINKS";
    tokens["1"] = "SUMMON";
    tokens["2"] = "STATS";
    tokens["3"] = "USERS";
    tokens["4"] = "HELP";
    tokens["5"] = "ERROR";
    tokens["6"] = "AWAY";
    tokens["7"] = "CONNECT";
    tokens["8"] = "PING";
    tokens["9"] = "PONG";
    tokens[";"] = "OPER";
    tokens["<"] = "PASS";
    tokens[">"] = "TIME";
    tokens["="] = "WALLOPS";
    tokens["?"] = "NAMES";
    tokens["@"] = "ADMIN";
    tokens["B"] = "NOTICE";
    tokens["C"] = "JOIN";
    tokens["D"] = "PART";
    tokens["E"] = "LUSERS";
    tokens["F"] = "MOTD";
    tokens["G"] = "MODE";
    tokens["H"] = "KICK";
    tokens["I"] = "SERVICE";
    tokens["J"] = "USERHOST";
    tokens["K"] = "ISON";
    tokens["L"] = "SQUERY";
    tokens["M"] = "SERVLIST";
    tokens["N"] = "SERVSET";
    tokens["O"] = "REHASH";
    tokens["P"] = "RESTART";
    tokens["Q"] = "CLOSE";
    tokens["R"] = "DIE";
    tokens["S"] = "HASH";
    tokens["T"] = "DNS";
    tokens["U"] = "SILENCE";
    tokens["V"] = "AKILL";
    tokens["W"] = "KLINE";
    tokens["X"] = "UNKLINE";
    tokens["Y"] = "RAKILL";
    tokens["Z"] = "GNOTICE";
    tokens["["] = "GOPER";
    tokens["]"] = "GLOBOPS";
    tokens["^"] = "LOCOPS";
    tokens["_"] = "PROTOCTL";
    tokens["`"] = "WATCH";
    tokens["b"] = "TRACE";
    tokens["c"] = "SQLINE";
    tokens["d"] = "UNSQLINE";
    tokens["e"] = "SVSNICK";
    tokens["f"] = "SVSNOOP";
    tokens["g"] = "PRIVMSG NickServ :IDENTIFY";
    tokens["h"] = "SVSKILL";
    tokens["i"] = "PRIVMSG NickServ";
    tokens["j"] = "PRIVMSG ChanServ";
    tokens["k"] = "PRIVMSG OperServ";
    tokens["l"] = "PRIVMSG MemoServ";
    tokens["m"] = "SERVICES";
    tokens["n"] = "SVSMODE";
    tokens["o"] = "SAMODE";
    tokens["p"] = "CHATOPS";
    tokens["q"] = "ZLINE";
    tokens["r"] = "UNZLINE";
    tokens["s"] = "PRIVMSG HelpServ";
}

void Protocol::Set(unsigned int in)
{
    FT("Protocol::Set", (in));

    // WE NEVER set 'i_Tokens', thats set with the PROTCTL line.

    switch (in)
    {
    case 0: /* RFC */
	i_Signon = 0000;
	i_Akill = 2;
	break;
    case 1: /* RFC with TS8 */
	i_Signon = 0001;
	i_Akill = 2;
	break;
    case 10: /* DAL < 4.4.15 */
	i_NickLen = 32;
	i_Signon = 1000;
	i_Globops = true;
	i_Akill = 1;
	i_Modes = 4;
	break;
    case 11: /* DAL >= 4.4.15 */
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_SVS = true;
	i_Akill = 1;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN WATCH=128 SAFELIST";
	break;
    case 20: /* UnderNet < 2.8.10  */
	i_Signon = 1000;
	i_Akill = 3;
	break;
    case 30: /* Aurora */
	i_NickLen = 32;
	i_Signon = 1002;
	i_Globops = true;
	i_SVS = true;
	i_SVSHOST = true;
	i_Akill = 1;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN WATCH=128 SAFELIST";
	break;
    case 40: /* Elite */
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_SVS = true;
	i_Akill = 1;
	break;
    case 50: /* Relic 2.0 */
	i_NickLen = 32;
	i_SVS = true;
	i_Globops = true;
	i_Signon = 1001;
	i_Akill = 1;
	i_Modes = 6;
	i_Server = "SERVER %s %d relic2.0 :%s";
	i_Protoctl = "PROTOCTL NOQUIT TOKEN WATCH=128 SAFELIST";
	break;
    case 51: /* Relic */
	i_NickLen = 32;
	i_SVS = true;
	i_Globops = true;
	i_P12 = true;
	i_Signon = 1003;
	i_Akill = 4;
	i_Modes = 6;
	i_Server = "SERVER %s %d relic2.1 :%s";
	i_Protoctl = "PROTOCTL NOQUIT TOKEN WATCH=128 SAFELIST";

	tokens.erase("1");
	tokens.erase("3");
	tokens.erase("=");
	tokens["^"] = "WALLOPS";
	tokens["LO"] = "LOCOPS";
	tokens.erase("c");
	tokens.erase("d");
	tokens["QL"] = "QLINE";
	tokens["Uq"] = "UNQLINE";
	tokens["o"] = "OMODE";
	tokens.erase("p");
	tokens["s"] = "OPERMOTD";
	tokens["v"] = "RPING";
	tokens["w"] = "RPONG";
	tokens["{"] = "MAP";
	tokens["|"] = "SJOIN";
	tokens["}"] = "SNICK";
	tokens["~"] = "GLINE";
	tokens["y"] = "SETTIME";
	tokens["HM"] = "HTM";
	tokens["z"] = "ADCHAT";
	tokens["Rz"] = "RW";

	break;
    default:
	return;
    }
    i_Number = in;
}

mstring Protocol::GetToken(mstring in)
{
    FT("Protocol::GetToken", (in));
    if (tokens.find(in) == tokens.end())
	RET("");
    RET(tokens[in]);
}

mstring Protocol::GetNonToken(mstring in)
{
    FT("Protocol::GetNonToken", (in));
    mstring retval;
    map<mstring,mstring>::iterator iter;
    for (iter = tokens.begin(); iter != tokens.end(); iter++)
    {
	if (iter->second.CmpNoCase(in)==0)
	{
	    retval = iter->first;
	    break;
	}
    }
    RET(retval);
}

Server::Server(mstring name, mstring description)
{
    FT("Server::Server", (name, description));
    i_Name = name.LowerCase();
    i_AltName = name.LowerCase();
    i_Uplink = Parent->startup.Server_Name().LowerCase();
    i_Hops = 0;
    i_Description = description;
    i_Ping = i_Lag = 0;
    i_Jupe = true;
    Parent->server.OurUplink(i_Name);
}

Server::Server(mstring name, int hops, mstring description)
{
    FT("Server::Server", (name, hops, description));
    i_Name = name.LowerCase();
    i_AltName = name.LowerCase();
    i_Uplink = Parent->startup.Server_Name().LowerCase();
    i_Hops = hops;
    i_Description = description;
    i_Ping = i_Lag = 0;
    i_Jupe = false;
    Parent->server.OurUplink(i_Name);
}

Server::Server(mstring name, mstring uplink, int hops, mstring description)
{
    FT("Server::Server", (name, uplink, hops, description));
    i_Name = name.LowerCase();
    i_AltName = name.LowerCase();
    i_Uplink = uplink.LowerCase();
    i_Hops = hops;
    i_Description = description;
    i_Ping = i_Lag = 0;
    i_Jupe = false;
}

void Server::operator=(const Server &in)
{
    FT("Server::operator=", ("(const Server &) in"));
    i_Name = in.i_Name;
    i_AltName = in.i_AltName;
    i_Uplink = in.i_Uplink;
    i_Hops = in.i_Hops;
    i_Description = in.i_Description;
    i_Ping = in.i_Ping;
    i_Lag = in.i_Lag;
    i_Jupe = in.i_Jupe;
}

void Server::Ping()
{
    NFT("Server::Ping");

    if (!i_Ping)
    {
        Parent->server.sraw(((Parent->server.proto.Tokens() && Parent->server.proto.GetNonToken("PING") != "") ?
		Parent->server.proto.GetNonToken("PING") : mstring("PING")) +
		" " + Parent->startup.Server_Name() + " :" + i_Name);
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

unsigned int Server::Users()
{
    NFT("Server::Users");

    unsigned int count = 0;
    map<mstring,Nick_Live_t>::iterator k;
    for (k=Parent->nickserv.live.begin(); k!=Parent->nickserv.live.end(); k++)
	if (k->second.Server() == i_Name) count++;
    RET(count);
}

unsigned int Server::Opers()
{
    NFT("Server::Opers");

    unsigned int count = 0;
    map<mstring,Nick_Live_t>::iterator k;
    for (k=Parent->nickserv.live.begin(); k!=Parent->nickserv.live.end(); k++)
	if (k->second.Server() == i_Name && k->second.HasMode("o")) count++;
    RET(count);
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

    while (found)
    {
	found = false;
	for (unsigned int i=0; i<uplinks.size(); i++) 
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

    if (Parent->Shutdown())
	return;

    // Take my sublinks with me (who will take theirs ...)
    vector<mstring> Kill = Downlinks();
    COM(("Destroying %d more servers", Kill.size()));
    for (unsigned int i=0; i<Kill.size(); i++)
	Parent->server.ServerList.erase(Kill[i]);
    if (Kill.size() && Parent->server.OurUplink() == i_Name)
	Parent->server.OurUplink("");
}

void NetworkServ::raw(mstring text)
{
    FT("NetworkServ::raw", (text));
    Parent->send(text);
}

void NetworkServ::sraw(mstring text)
{
    raw(":" + Parent->startup.Server_Name() + " " + text);
}

void NetworkServ::SignOnAll()
{
    NFT("NetworkServ::SignOnAll");

    mstring doison;
    unsigned int i;
    for (i=1; i<=Parent->operserv.GetNames().WordCount(" "); i++)
    {
	doison += " " + Parent->operserv.GetNames().ExtractWord(i, " ");
	WaitIsOn.insert(Parent->operserv.GetNames().ExtractWord(i, " "));
    }
    for (i=1; i<=Parent->nickserv.GetNames().WordCount(" "); i++)
    {
	doison += " " + Parent->nickserv.GetNames().ExtractWord(i, " ");
	WaitIsOn.insert(Parent->nickserv.GetNames().ExtractWord(i, " "));
    }
    for (i=1; i<=Parent->chanserv.GetNames().WordCount(" "); i++)
    {
	doison += " " + Parent->chanserv.GetNames().ExtractWord(i, " ");
	WaitIsOn.insert(Parent->chanserv.GetNames().ExtractWord(i, " "));
    }
    for (i=1; i<=Parent->memoserv.GetNames().WordCount(" "); i++)
    {
	doison += " " + Parent->memoserv.GetNames().ExtractWord(i, " ");
	WaitIsOn.insert(Parent->memoserv.GetNames().ExtractWord(i, " "));
    }
    for (i=1; i<=Parent->commserv.GetNames().WordCount(" "); i++)
    {
	doison += " " + Parent->commserv.GetNames().ExtractWord(i, " ");
	WaitIsOn.insert(Parent->commserv.GetNames().ExtractWord(i, " "));
    }
    for (i=1; i<=Parent->servmsg.GetNames().WordCount(" "); i++)
    {
	doison += " " + Parent->servmsg.GetNames().ExtractWord(i, " ");
	WaitIsOn.insert(Parent->servmsg.GetNames().ExtractWord(i, " "));
    }

    if (doison != "")
	sraw(((proto.Tokens() && proto.GetNonToken("ISON") != "") ?
		proto.GetNonToken("ISON") : mstring("ISON")) +
		doison);
}


NetworkServ::NetworkServ()
{
    NFT("NetworkServ::NetworkServ");
    messages=true;
    i_UserMax = 0;
}

void NetworkServ::FlushMsgs(mstring nick)
{
    FT("NetworkServ::FlushMsgs", (nick));

    map<mstring, list<triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> > > >::iterator i;
    list<triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> > >::iterator j;

    // Dont report this, thats the point of the queue...
    if (!Parent->nickserv.IsLive(nick))
	return;

    for (i=ToBeSent.begin(); i!=ToBeSent.end(); i++)
    {
	if (nick.LowerCase() == i->first)
	{
	    for (j=i->second.begin(); j!=i->second.end(); j++)
	    {
		if (j->second.SecondsSince() > Parent->config.Squit_Protect())
		    continue;

		switch (j->first)
		{
		case t_GLOBOPS:
		    GLOBOPS(nick, j->third.first);
		    break;
		case t_INVITE:
		    INVITE(nick, j->third.first,
				j->third.second);
		    break;
		case t_KICK:
		    KICK(nick, j->third.first,
				j->third.second,
				j->third.third);
		    break;
		case t_KILL:
		    KILL(nick, j->third.first,
				j->third.second);
		    break;
		case t_NOTICE:
		    NOTICE(nick, j->third.first,
				j->third.second);
		    break;
		case t_PRIVMSG:
		    PRIVMSG(nick, j->third.first,
				j->third.second);
		    break;
		case t_QLINE:
		    QLINE(nick, j->third.first,
				j->third.second);
		    break;
		case t_SVSMODE:
		    SVSMODE(nick, j->third.first,
				j->third.second);
		    break;
		case t_SVSNICK:
		    SVSNICK(nick, j->third.first,
				j->third.second);
		    break;
		case t_SVSKILL:
		    SVSKILL(nick, j->third.first,
				j->third.second);
		    break;
		case t_SVSHOST:
		    SVSHOST(nick, j->third.first,
				j->third.second);
		    break;
		case t_TOPIC:
		    TOPIC(nick, j->third.first,
				j->third.second);
		    break;
		case t_UNQLINE:
		    UNQLINE(nick, j->third.first);
		    break;
		case t_WALLOPS:
		    WALLOPS(nick, j->third.first);
		    break;
		default:
		    Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_UNKNOWN"),
			(int) j->first, i->first.c_str(), j->third.first.c_str(),
			j->third.second.c_str(), j->third.third.c_str(),
			ToHumanTime(j->second.SecondsSince()).c_str());
		    break;
		}
	    }
	    ToBeSent.erase(i->first);
	    return;
	}
    }
}

bool NetworkServ::IsServer(mstring server)
{
    FT("NetworkServ::IsServer", (server));
    bool retval = (ServerList.find(server.LowerCase()) != ServerList.end());
    RET(retval);
}


bool NetworkServ::IsSquit(mstring server)
{
    FT("NetworkServ::IsSquit", (server));
    bool retval = (ServerSquit.find(server.LowerCase()) != ServerSquit.end());
    RET(retval);
}

void NetworkServ::Jupe(mstring server, mstring reason)
{
    FT("NetworkServ::Jupe", (server, reason));
    if (IsServer(server))
	raw(((proto.Tokens() && proto.GetNonToken("SQUIT") != "") ?
		proto.GetNonToken("SQUIT") : mstring("SQUIT")) +
		" " + server.LowerCase() + " :JUPE command used.");
	    // SERVER downlink hops :description
	    // :uplink SERVER downlink hops :description
    mstring tmp;
    tmp.Format(proto.Server().c_str(),
	    server.LowerCase().c_str(), 2,
	    ("JUPED (" + reason + ")").c_str());
    raw(tmp);
    Parent->server.ServerList[server.LowerCase()] =
		    Server(server.LowerCase(), "JUPED (" + reason + ")");
}

void NetworkServ::AKILL(mstring host, mstring reason, unsigned long time)
{
    FT("NetworkServ::AKILL", (host, reason, time));

    if (!host.Contains("@"))
	return;

    mstring line;
    switch (proto.Akill())
    {
    case 0:
	break;
    case 1:
	if (proto.Tokens() && proto.GetNonToken("AKILL") != "")
	    line << proto.GetNonToken("AKILL");
	else
	    line << "AKILL";
	line << " " << host.After("@") << " " << host.Before("@") << " :" << reason;
	break;
    case 2:
	if (proto.Tokens() && proto.GetNonToken("GLINE") != "")
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * +" << time << " " << host << " :" << reason;
	break;
    case 3:
	if (proto.Tokens() && proto.GetNonToken("GLINE") != "")
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * +" << host << " " << time << " :" << reason;
	break;
    case 4:
	if (proto.Tokens() && proto.GetNonToken("GLINE") != "")
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " +" << host << " " << time << " :" << reason;
	break;
    }
    if (line != "")
	sraw(line);
}

void NetworkServ::ANONKILL(mstring nick, mstring dest, mstring reason)
{
    FT("NetworkServ::ANONKILL", (nick, dest, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_KILL, Now(), triplet<mstring, mstring, mstring>(
		dest, reason, "")));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"KILL", nick.c_str());
    }
    else if (!Parent->nickserv.IsLive(dest))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"KILL", nick.c_str(), dest.c_str());
    }
    else
    {
	Parent->nickserv.live[dest.LowerCase()].Quit(
		"Killed (" + reason + ")");
	Parent->nickserv.live.erase(dest.LowerCase());
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("KILL") != "") ?
			proto.GetNonToken("KILL") : mstring("KILL")) +
		" " + dest + " :" + reason);
    }
}


void NetworkServ::AWAY(mstring nick, mstring reason)
{
    FT("NetworkServ::AWAY", (nick, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"AWAY", nick.c_str());
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"AWAY", nick.c_str());
    }
    else
    {
	mstring line;
	Parent->nickserv.live[nick.LowerCase()].Away(reason);
	if (proto.Tokens() && proto.GetNonToken("AWAY") != "")
	    line << proto.GetNonToken("AWAY");
	else
	    line << "AWAY";
	if(reason != "")
	    line=" : " + reason;
	raw(":" + nick + " " +  line);
    }
}


void NetworkServ::GLOBOPS(mstring nick, mstring message)
{
    FT("NetworkServ::GLOBOPS", (nick, message));

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_GLOBOPS, Now(), triplet<mstring, mstring, mstring>(
		message, "", "")));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"GLOBOPS", nick.c_str());
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Action();
	if (proto.Globops())
	    raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("GLOBOPS") != "") ?
			proto.GetNonToken("GLOBOPS") : mstring("GLOBOPS")) +
		" :" + message);
	else
	    raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("WALLOPS") != "") ?
			proto.GetNonToken("WALLOPS") : mstring("WALLOPS")) +
		" :" + message);
    }
}


void NetworkServ::INVITE(mstring nick, mstring dest, mstring channel)
{
    FT("NetworkServ::INVITE", (nick, dest, channel));

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_INVITE, Now(), triplet<mstring, mstring, mstring>(
		dest, channel, "")));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"INVITE", nick.c_str());
    }
    else if (!Parent->nickserv.IsLive(dest))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"INVITE", nick.c_str(), dest.c_str());
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"INVITE", nick.c_str(), channel.c_str());
    }
    else
    {
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("INVITE") != "") ?
			proto.GetNonToken("INVITE") : mstring("INVITE")) +
		" " + dest + " :" + channel);
    }
}


void NetworkServ::JOIN(mstring nick, mstring channel)
{
    FT("NetworkServ::JOIN", (nick, channel));

    if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"JOIN", nick.c_str());
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"JOIN", nick.c_str());
    }
    else
    {
	for (unsigned int i=0; i<channel.WordCount(", "); i++)
	    Parent->nickserv.live[nick.LowerCase()].Join(channel.ExtractWord(i+1, ", "));
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("JOIN") != "") ?
			proto.GetNonToken("JOIN") : mstring("JOIN")) +
		" :" + channel);
    }
}


void NetworkServ::KICK(mstring nick, mstring dest, mstring channel, mstring reason)
{
    FT("NetworkServ::KICK", (nick, dest, channel, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_KICK, Now(), triplet<mstring, mstring, mstring>(
		dest, channel, reason)));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"KICK", nick.c_str());
    }
    else if (!Parent->nickserv.IsLive(dest))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"KICK", nick.c_str(), dest.c_str());
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"KICK", nick.c_str(), channel.c_str());
    }
    else if (!Parent->chanserv.live[channel.LowerCase()].IsIn(dest))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_NOTINCHAN"),
		"KICK", nick.c_str(), dest.c_str(), channel.c_str());
    }
    else
    {
	Parent->nickserv.live[dest.LowerCase()].Kick(nick, channel);
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("KICK") != "") ?
			proto.GetNonToken("KICK") : mstring("KICK")) +
		" " + channel + " " + dest + " :" + reason);
    }
}


void NetworkServ::KILL(mstring nick, mstring dest, mstring reason)
{
    FT("NetworkServ::KILL", (nick, dest, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_KILL, Now(), triplet<mstring, mstring, mstring>(
		dest, reason, "")));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"KILL", nick.c_str());
    }
    else if (!Parent->nickserv.IsLive(dest))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"KILL", nick.c_str(), dest.c_str());
    }
    else
    {
	Parent->nickserv.live[dest.LowerCase()].Quit(
		"Killed (" + nick + " (" + reason + "))");
	Parent->nickserv.live.erase(dest.LowerCase());
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("KILL") != "") ?
			proto.GetNonToken("KILL") : mstring("KILL")) +
		" " + dest + " :" + Parent->nickserv.live[nick.LowerCase()].Host() +
		"!" + nick + " (" + reason + ")");
    }
}


void NetworkServ::MODE(mstring nick, mstring mode)
{
    FT("NetworkServ::MODE", (nick, mode));

    if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"MODE", nick.c_str());
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"MODE", nick.c_str());
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Mode(mode);
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("MODE") != "") ?
			proto.GetNonToken("MODE") : mstring("MODE")) +
		" " + nick + " :" + mode);
    }
}


void NetworkServ::MODE(mstring nick, mstring channel, mstring mode)
{
    FT("NetworkServ::MODE", (nick, channel, mode));

    if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"MODE", nick.c_str());
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"MODE", nick.c_str());
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"MODE", nick.c_str(), channel.c_str());
    }
    else
    {
	Parent->chanserv.live[channel.LowerCase()].Mode(nick, mode);
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("MODE") != "") ?
			proto.GetNonToken("MODE") : mstring("MODE")) +
		" " + channel + " " + mode.Before(" ") + " " +
		mode.After(" "));
    }
}


void NetworkServ::NICK(mstring nick, mstring user, mstring host,
    	mstring server, mstring realname)
{
    FT("NetworkServ::NICK", (nick, user, host, server, realname));

    if (Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_TOUSER"),
		"NICK", server.c_str(), nick.c_str());
    }
    else
    {
	mstring send;
	switch (proto.Signon())
	{
	case 0000:
	    if (proto.Tokens() && proto.GetNonToken("USER") != "")
		send << proto.GetNonToken("USER");
	    else
		send << "USER";
	    send << " " << nick  << " " << user << " " << host <<
		" " << server << " :" << realname;
	    break;
	case 0001:
	    if (proto.Tokens() && proto.GetNonToken("USER") != "")
		send << proto.GetNonToken("USER");
	    else
		send << "USER";
	    send << " " << nick  << Now().timetstring() <<
		" " << user << " " << host << " " << server <<
		" :" << realname;
	    break;
	case 1000:
	    if (proto.Tokens() && proto.GetNonToken("NICK") != "")
		send << proto.GetNonToken("NICK");
	    else
		send << "NICK";
	    send << " " << nick << " 1 " << Now().timetstring() <<
		" " << user << " " << host << " " << server <<
		" :" << realname;
	    break;
	case 1001:
	    if (proto.Tokens() && proto.GetNonToken("NICK") != "")
		send << proto.GetNonToken("NICK");
	    else
		send << "NICK";
	    send << " " << nick << " 1 " << Now().timetstring() <<
		" " << user << " " << host << " " << server <<
		" 1 :" << realname;
	    break;
	case 1002:
	    if (proto.Tokens() && proto.GetNonToken("NICK") != "")
		send << proto.GetNonToken("NICK");
	    else
		send << "NICK";
	    send << " " << nick << " 1 " << Now().timetstring() <<
		" " << user << " " << host << " " << server <<
		" 1 " << host << ":" << realname;
	    break;
	case 1003:
	    if (proto.Tokens() && proto.GetNonToken("NICK") != "")
		send << proto.GetNonToken("NICK");
	    else
		send << "NICK";
	    send << " " << nick << " 1 " << Now().timetstring() <<
		" " << user << " " << host << " " << host << " " <<
		server << " 1 " << ":" << realname;
	    break;
	}
	// Sign ourselves in ...
	Parent->nickserv.live[nick.LowerCase()] = Nick_Live_t(
		nick, user, host, realname);
	if (i_UserMax < Parent->nickserv.live.size())
	    i_UserMax = Parent->nickserv.live.size();
	raw(send);
    }
}


void NetworkServ::NICK(mstring oldnick, mstring newnick)
{
    FT("NetworkServ::NICK", (oldnick, newnick));

    if (!Parent->nickserv.IsLive(oldnick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"NICK", oldnick.c_str());
    }
    else if (!Parent->nickserv.live[oldnick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"NICK", oldnick.c_str());
    }
    else
    {
	if (!Parent->nickserv.IsLive(newnick))
	    Parent->nickserv.live.erase(newnick.LowerCase());
	Parent->nickserv.live[newnick.LowerCase()] =
		Parent->nickserv.live[oldnick.LowerCase()];
	Parent->nickserv.live.erase(oldnick.LowerCase());
	Parent->nickserv.live[newnick.LowerCase()].Name(newnick);
	raw(":" + oldnick + " " +
		((proto.Tokens() && proto.GetNonToken("NICK") != "") ?
			proto.GetNonToken("NICK") : mstring("NICK")) +
		" " + newnick);
    }
}


void NetworkServ::NOOP(mstring nick, mstring server, bool onoff)
{
    FT("NetworkServ::NOOP", (nick, server, onoff));

    if (!Parent->server.proto.SVS())
	return;

    if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"NOOP", nick.c_str());
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"NOOP", nick.c_str());
    }
    else if (!IsServer(server))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONSERVER"),
		"NOOP", nick.c_str(), server.c_str());
    }
    else
    {
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("NOOP") != "") ?
			proto.GetNonToken("NOOP") : mstring("NOOP")) +
		" " + server + " " + mstring(onoff ? "+" : "-"));
    }
}


void NetworkServ::NOTICE(mstring nick, mstring dest, mstring text)
{
    FT("NetworkServ::NOTICE", (nick, dest, text));

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_NOTICE, Now(), triplet<mstring, mstring, mstring>(
		dest, text, "")));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"NOTICE", nick.c_str());
    }
    else if (!(dest[0u] == '$' || dest[0u] == '#' ||
		Parent->nickserv.IsLive(dest) ||
		Parent->chanserv.IsLive(dest)))
    {
	if (IsChan(dest))
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"NOTICE", nick.c_str(), dest.c_str());
	else
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"NOTICE", nick.c_str(), dest.c_str());
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Action();
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
		" " + dest + " :" + text);
    }
}


void NetworkServ::PART(mstring nick, mstring channel, mstring reason)
{
    FT("NetworkServ::PART", (nick, channel, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"PART", nick.c_str());
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"PART", nick.c_str());
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"PART", nick.c_str(), channel.c_str());
    }
    else if (!Parent->chanserv.live[channel.LowerCase()].IsIn(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_NOTINCHAN"),
		"KICK", nick.c_str(), nick.c_str(), channel.c_str());
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Part(channel);
	mstring tmpResult;
	if(reason!="")
	    tmpResult=" :"+reason;
	else
	    tmpResult="";
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("PART") != "") ?
			proto.GetNonToken("PART") : mstring("PART")) +
		" " + channel + tmpResult);
    }
}


void NetworkServ::PRIVMSG(mstring nick, mstring dest, mstring text)
{
    FT("NetworkServ::PRIVMSG", (nick, dest, text));

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_PRIVMSG, Now(), triplet<mstring, mstring, mstring>(
		dest, text, "")));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"PRIVMSG", nick.c_str());
    }
    else if (!(dest[0u] == '$' || dest[0u] == '#' ||
		Parent->nickserv.IsLive(dest) ||
		Parent->chanserv.IsLive(dest)))
    {
	if (IsChan(dest))
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"PRIVMSG", nick.c_str(), dest.c_str());
	else
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"PRIVMSG", nick.c_str(), dest.c_str());
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Action();
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("PRIVMSG") != "") ?
			proto.GetNonToken("PRIVMSG") : mstring("PRIVMSG")) +
		" " + dest + " :" + text);
    }
}


void NetworkServ::QLINE(mstring nick, mstring target, mstring reason)
{
    FT("NetworkServ::QLINE", (nick, target,reason));

    if (!Parent->server.proto.SVS())
	return;

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_QLINE, Now(), triplet<mstring, mstring, mstring>(
		target, reason, "")));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"QLINE", nick.c_str());
    }
    else
    {
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("QLINE") != "") ?
			proto.GetNonToken("QLINE") : mstring("QLINE")) +
		" " + target + " :" + reason);
    }
}


void NetworkServ::QUIT(mstring nick, mstring reason)
{
    FT("NetworkServ::QUIT", (nick, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"QUIT", nick.c_str());
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"QUIT", nick.c_str());
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Quit(reason);
	Parent->nickserv.live.erase(nick.LowerCase());
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("QUIT") != "") ?
			proto.GetNonToken("QUIT") : mstring("QUIT")) +
		" :" + reason);
    }
}


void NetworkServ::RAKILL(mstring host)
{
    FT("NetworkServ::RAKILL", (host));

    if (!host.Contains("@"))
	return;

    mstring line;
    switch (proto.Akill())
    {
    case 0:
	break;
    case 1:
	if (proto.Tokens() && proto.GetNonToken("RAKILL") != "")
	    line << proto.GetNonToken("RAKILL");
	else
	    line << "RAKILL";
	line << " " << host.After("@") << " " << host.Before("@");
	break;
    case 2:
	if (proto.Tokens() && proto.GetNonToken("UNGLINE") != "")
	    line << proto.GetNonToken("UNGLINE");
	else
	    line << "UNGLINE";
	line << " * " << host;
	break;
    case 3:
	if (proto.Tokens() && proto.GetNonToken("GLINE") != "")
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * -" << host;
	break;
    case 4:
	if (proto.Tokens() && proto.GetNonToken("GLINE") != "")
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " -" << host;
	break;
    }
    if (line != "")
	sraw(line);
}

void NetworkServ::SVSMODE(mstring mynick, mstring nick, mstring mode)
{
    FT("NetworkServ::SVSMODE", (mynick, nick, mode));

    if (!Parent->server.proto.SVS())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSMODE, Now(), triplet<mstring, mstring, mstring>(
		nick, mode, "")));
	return;
    }
    else if (!Parent->nickserv.live[mynick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"SVSMODE", nick.c_str());
    }
    else if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"SVSMODE", mynick.c_str(), nick.c_str());
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Mode(mode);
	raw(":" + mynick + " " +
		((proto.Tokens() && proto.GetNonToken("SVSMODE") != "") ?
			proto.GetNonToken("SVSMODE") : mstring("SVSMODE")) +
		" " + nick + " " + mode);
    }
}


void NetworkServ::SVSKILL(mstring mynick, mstring nick, mstring reason)
{
    FT("NetworkServ::SVSKILL", (mynick, nick, reason));

    if (!Parent->server.proto.SVS())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSKILL, Now(), triplet<mstring, mstring, mstring>(
		nick, reason, "")));
	return;
    }
    else if (!Parent->nickserv.live[mynick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"SVSKILL", nick.c_str());
    }
    else if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"SVSKILL", mynick.c_str(), nick.c_str());
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Quit(reason);
	raw(":" + mynick + " " +
		((proto.Tokens() && proto.GetNonToken("SVSKILL") != "") ?
			proto.GetNonToken("SVSKILL") : mstring("SVSKILL")) +
		" " + nick + " :" + reason);
    }
}


void NetworkServ::SVSHOST(mstring mynick, mstring nick, mstring newhost)
{
    FT("NetworkServ::SVSHOST", (mynick, nick, newhost));

    if (!Parent->server.proto.SVSHOST())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSHOST, Now(), triplet<mstring, mstring, mstring>(
		nick, newhost, "")));
	return;
    }
    else if (!Parent->nickserv.live[mynick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"SVSHOST", nick.c_str());
    }
    else if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"SVSHOST", mynick.c_str(), nick.c_str());
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].AltHost(newhost);
	raw(":" + mynick + " " +
		((proto.Tokens() && proto.GetNonToken("SVSHOST") != "") ?
			proto.GetNonToken("SVSHOST") : mstring("SVSHOST")) +
		" " + nick + " " + newhost);
    }
}


void NetworkServ::SVSNICK(mstring mynick, mstring nick, mstring newnick)
{
    FT("NetworkServ::SVSNICK", (mynick, nick, newnick));

    if (!Parent->server.proto.SVS())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSNICK, Now(), triplet<mstring, mstring, mstring>(
		nick, newnick, "")));
	return;
    }
    else if (!Parent->nickserv.live[mynick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"SVSNICK", nick.c_str());
    }
    else if (!Parent->nickserv.IsLive(nick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"SVSNICK", mynick.c_str(), nick.c_str());
    }
    else if (Parent->nickserv.IsLive(newnick))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_TOUSER"),
		"SVSNICK", mynick.c_str(), newnick.c_str());
    }
    else
    {
	mstring output;
	output << ":" << mynick << " ";
	if (proto.Tokens() && proto.GetNonToken("SVSNICK") != "")
	    output << proto.GetNonToken("SVSNICK");
	else
	    output << "SVSNICK";
	output << " " << nick << " " <<
		    newnick << " :" << time_t(NULL);
	raw(output);
    }
}


void NetworkServ::TOPIC(mstring nick, mstring setter, mstring channel, mstring topic, mDateTime time)
{
    FT("NetworkServ::TOPIC", (nick, setter, channel, topic, time));

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_TOPIC, Now(), triplet<mstring, mstring, mstring>(
		channel, topic, "")));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"TOPIC", nick.c_str());
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"TOPIC", nick.c_str(), channel.c_str());
    }
    else
    {
	mstring send;
	send << ":" << nick << " ";
	if (proto.Tokens() && proto.GetNonToken("TOPIC") != "")
	    send << proto.GetNonToken("TOPIC");
	else
	    send << "TOPIC";
	if (topic == "")
	    send << " " << channel << " " << setter;
	else
	    send << " " << channel << " " <<
		setter << " " << time.timetstring() << " :" << topic;

	Parent->chanserv.live[channel.LowerCase()].Topic(nick, topic, setter, time);
	raw(send);
    }
}


void NetworkServ::UNQLINE(mstring nick, mstring target)
{
    FT("NetworkServ::UNQLINE", (nick, target));

    if (!Parent->server.proto.SVS())
	return;

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_UNQLINE, Now(), triplet<mstring, mstring, mstring>(
		target, "", "")));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"UNQLINE", nick.c_str());
    }
    else
    {
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("UNQLINE") != "") ?
			proto.GetNonToken("UNQLINE") : mstring("UNQLINE")) +
		" " + target);
    }
}


void NetworkServ::WALLOPS(mstring nick, mstring message)
{
    FT("NetworkServ::WALLOPS", (nick, message));

    if (!Parent->nickserv.IsLive(nick))
    {
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_WALLOPS, Now(), triplet<mstring, mstring, mstring>(
		message, "", "")));
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	Log(LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"WALLOPS", nick.c_str());
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Action();
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("WALLOPS") != "") ?
			proto.GetNonToken("WALLOPS") : mstring("WALLOPS")) +
		" :" + message);
    }
}


void NetworkServ::KillUnknownUser(mstring user)
{
    FT("NetworkServ::KillUnknownUser", (user));
    raw(":" + Parent->startup.Server_Name() + " " +
	((proto.Tokens() && proto.GetNonToken("KILL") != "") ?
		proto.GetNonToken("KILL") : mstring("KILL")) +
    	" " + user + " :" + Parent->startup.Server_Name() +
    	" (" + user + "(?) <- " + Parent->Server() + ")");
    Log(LM_ERROR, Parent->getLogMessage("OTHER/KILL_UNKNOWN"),
	user.c_str());
}


void NetworkServ::execute(const mstring & data)
{
    //okay this is the main networkserv command switcher
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

	    if (Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }
	    // :source ADMIN
	    sraw("256 " + source + " :Administrative info about " +
		Parent->startup.Server_Name());
	    sraw("257 " + source + " :" + Parent->startup.Server_Desc());
	    sraw("258 " + source + " :Admins - " + Parent->operserv.Services_Admin());
	    sraw("259 " + source + " :" + FULLNAME + " - " + EMAIL);
	}
	else if (msgtype=="AKILL")
	{
	    // We will ignore AKILLS because they're not relivant to us.
	    // we will not be akilling our own clients ;P
	}
	else if (msgtype=="AWAY")
	{
	    if (source.Contains(".") || source == "")
		return;

	    // :source AWAY
	    // :source AWAY :This is my reason
	    if (data.ExtractWord(3, ": ")=="")
	    {
		Parent->nickserv.live[sourceL].Away("");

		// HAS to be AFTER the nickname is added to map.
		map<mstring, Committee>::iterator iter;
		for (iter = Parent->commserv.list.begin();
				    iter != Parent->commserv.list.end();
				    iter++)
		{
		    if (iter->second.IsOn(sourceL))
		    {
			for (iter->second.message = iter->second.MSG_begin();
			    iter->second.message != iter->second.MSG_end();
			    iter->second.message++)
			{
			    Parent->servmsg.send(source, "[" + IRC_Bold +
					    iter->first + IRC_Off + "] " +
					    iter->second.message->Entry());
			}
		    }
		}
	    }
	    else
		Parent->nickserv.live[sourceL].Away(data.After(":", 2));
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'B':
	Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	break;
    case 'C':
	if (msgtype=="CONNECT")
	{
	    // :soul.darker.net 481 ChanServ :Permission Denied- You do not have the correct IRC operator privileges


	    // :source CONNECT some.server port :our.server
	    if (IsServer(data.ExtractWord(3, ": ")) ||
	    	data.ExtractWord(3, ": ").LowerCase() == Parent->startup.Server_Name().LowerCase())
	    {
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :Connect: Server " + data.ExtractWord(3, ": ") +
			" already exists from " + Parent->startup.Server_Name());
	    }
	    else
	    {
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :Connect: Host " +
			data.ExtractWord(3, ": ") + " not listed in irc.conf");
	    }
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'D':
	Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	break;
    case 'E':
	if (msgtype=="ERROR")
	{
	    // ERROR :This is my error
	    Log(LM_NOTICE, Parent->getLogMessage("OTHER/SERVER_MSG"),
		    msgtype.c_str(), data.After(":").c_str());
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'F':
	Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
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
	    Log(LM_NOTICE, Parent->getLogMessage("OTHER/SERVER_MSG"),
		    msgtype.c_str(), data.After(":").c_str());
	}
	else if (msgtype=="GOPER")
	{
	    // useless chatter ... can be ignored.
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'H':
	Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	break;
    case 'I':
	if (msgtype=="INFO")
	{
	    // :source INFO :server/nick
	    if (Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }
	    else
		for (int i=0; i<sizeof(contrib)/sizeof(mstring); i++)
		    sraw("371 " + source + " :" + contrib[i]);
	    sraw("374 " + source + " :End of /INFO report");
	}
	else if (msgtype=="INVITE")
	{
	    // :source INVITE target :channel
	    //:PreZ INVITE ChanServ :#chatzone

	    // We can ignore this, as our clients will not 'join on invite'
	    // and we dont really need an invitation to join ...
	}
	else if (msgtype=="ISON")
	{
	    // :heaven.darker.net ISON ChanServ
	    // :soul.darker.net 303 heaven.darker.net :ChanServ
	    // :heaven.darker.net ISON zBlerk
	    // :soul.darker.net 303 heaven.darker.net :

	    // repl: :our.server 303 source :local.nick
	    mstring isonstr = "";
	    for (unsigned int i=3; i<=data.WordCount(": "); i++)
	    {
		if (isonstr.Len() > proto.MaxLine())
		{
		    sraw("303 " + source + " :" + isonstr);
		    isonstr = "";
		}
		if (Parent->nickserv.IsLive(data.ExtractWord(i, ": ")))
		    isonstr += data.ExtractWord(i, ": ") + " ";
	    }
	    if (isonstr != "")
		sraw("303 " + source + " :" + isonstr);
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'J':
	if (msgtype=="JOIN")
	{
	    if (source.Contains(".") || source == "")
		return;

	    // :source JOIN :#channel
	    for (unsigned int i=3; i<=data.WordCount(":, "); i++)
		Parent->nickserv.live[sourceL].Join(data.ExtractWord(i, ":, "));
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
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

	    if (!(source.Contains(".") || source == "") &&
		!Parent->nickserv.live[sourceL].IsInChan(data.ExtractWord(3, ": ")))
		sraw(((proto.Tokens() && proto.GetNonToken("KICK") != "") ?
			proto.GetNonToken("KICK") : mstring("KICK")) +
			" " + data.ExtractWord(3, ": ") + " " + source + " :You are not in this channel");

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
		// sign on services again if they're killed.
		if (Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].IsServices())
		{
		    Log(LM_WARNING, Parent->getLogMessage("OTHER/KILLED"),
			    data.ExtractWord(3, ": ").c_str(),
			    Parent->nickserv.live[sourceL].Mask(Nick_Live_t::N_U_P_H).c_str());
		    WaitIsOn.insert(data.ExtractWord(3, ": "));
		    sraw(((proto.Tokens() && proto.GetNonToken("ISON") != "") ?
			proto.GetNonToken("ISON") : mstring("ISON")) +
			" " + data.ExtractWord(3, ": "));
		}
		int wc = data.After(":", 2).WordCount("!");
		Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].Quit(
			"Killed (" + data.After(":", 2).After("!", wc-1) + ")");
		Parent->nickserv.live.erase(data.ExtractWord(3, ": ").LowerCase());
	    }
	    else
	    {
		Log(LM_ERROR, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"KILL", source.c_str(), data.ExtractWord(3, ": ").c_str());
	    }

	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'L':
	if (msgtype=="LINKS")
	{
	    //:ChanServ LINKS :temple.magick.tm
	    //:temple.magick.tm 364 ChanServ temple.magick.tm temple.magick.tm :0 Magick IRC Services Test Network
	    //:temple.magick.tm 365 ChanServ temple.magick.tm :End of /LINKS list.

	    if (Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }
	    else
	    {
		sraw("364 " + source + " " + Parent->startup.Server_Name() + " " +
		    Parent->startup.Server_Name() + " :0 " + Parent->startup.Server_Desc());

		map<mstring,Server>::iterator serv;
		for(serv=Parent->server.ServerList.begin(); serv!=Parent->server.ServerList.end(); serv++)
		{
		    sraw("364 " + source + " " + serv->second.Name() + " " + serv->second.Uplink()
			+ " :" + serv->second.Hops() + " " + serv->second.Description());
		}
	    }

	    sraw("365 " + source + " " + Parent->startup.Server_Name() + " :End of /LINKS list.");

	}
	else if (msgtype=="LIST")
	{
	    sraw("321 " + source + " Channel :Users  Name");

	    if (Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }
	    else
	    {
		map<mstring,Chan_Live_t>::iterator chan;
		for (chan=Parent->chanserv.live.begin(); chan!=Parent->chanserv.live.end(); chan++)
		{
		    sraw("322 " + source + " " + chan->first + " " + mstring(chan->second.Users()) + 
			" :" + chan->second.Topic());
		}
	    }

	    sraw("323 " + source + " :End of /LIST");
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
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
		    Log(LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
			"MODE", source.c_str(), data.ExtractWord(3, ": ").c_str());
		}
	    }
	    else
	    {
		if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
		{
		    Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].Mode(data.ExtractWord(4, ": "));
		}
		else
		{
		    Log(LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"MODE", source.c_str(), data.ExtractWord(3, ": ").c_str());
		}
	    }
	}
	else if (msgtype=="MOTD")
	{
	    if (mFile::Exists(Parent->files.Motdfile()))
	    {
		sraw("375 " + source + " :Message Of The Day");
		if (Parent->ircsvchandler->HTM_Level() > 3)
		{
		    mstring tmp;
		    tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		    raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		    return;
		}
		else
		{
		    vector<mstring> tmp = mFile::UnDump(Parent->files.Motdfile());
		    int i;
		    for (i=0; i<tmp.size(); i++)
			sraw("372 " + source + " :" + tmp[i].c_str());
		}
		sraw("376 " + source + " :End of MOTD.");
	    }
	    else
	    {
		sraw("422 " + source + " :No MOTD exists.");
	    }
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'N':
	if (msgtype=="NAMES")
	{
	    // :source NAMES #channel our.server
//:soul.darker.net 353 ChanServ = #chatzone :killkickabuseme @Aimee Jupiter @Allanon Ghost_ wildrose
//:soul.darker.net 366 ChanServ #chatzone :End of /NAMES list.
	    sraw("366 " + source + " " + data.ExtractWord(3, ": ") + " :End of /NAMES list.");

	}
	else if (msgtype=="NICK")
	{
	    if (source.Contains("."))
		return;

	    if (source.IsEmpty()) {
		// NEW USER
		sourceL = data.ExtractWord(2, ": ").LowerCase();

		// DONT kill when we do SQUIT protection.
		map<mstring,list<mstring> >::iterator i;
		for (i=ToBeSquit.begin(); i!=ToBeSquit.end(); i++)
		{
		    list<mstring>::iterator k;
		    for (k=i->second.begin(); k!=i->second.end(); k++)
			if (*k == sourceL)
			{
			    list<mstring>::iterator j = k;  j--;
			    i->second.erase(k);
			    k=j;
			}
		}

		if (Parent->nickserv.IsLive(sourceL))
		{
		    // IF the squit server = us, and the signon time matches
		    if (Parent->nickserv.live[sourceL].Squit() == data.ExtractWord(7, ": ").LowerCase()
			&& Parent->nickserv.live[sourceL].SignonTime() == mDateTime((time_t) atol(data.ExtractWord(4, ": "))))
		    {
			Parent->nickserv.live[sourceL].ClearSquit();
			return;    // nice way to avoid repeating ones self :)
		    }
		    else
		    {
			Parent->nickserv.live[sourceL].Quit("SQUIT - " + Parent->nickserv.live[sourceL].Server());
			Parent->nickserv.live.erase(sourceL);
		    }
		}

		switch (proto.Signon())
		{
		case 0000:
		case 0001:
		    break;
		case 1000: // NICK nick hops time user host server :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		    break;
		case 1001: // NICK nick hops time user host server 1 :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		    break;
		case 1002: // NICK nick hops time user host server 0 real-host :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(9, ": "),
			    data.After(":")
			);
		    Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(6, ": "));
		    break;
		case 1003: // NICK nick hops time user real-host host server 0 :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(8, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		    Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(7, ": "));
		    break;
		}
		if (i_UserMax < Parent->nickserv.live.size())
		    i_UserMax = Parent->nickserv.live.size();

		// HAS to be AFTER the nickname is added to map.
		map<mstring, Committee>::iterator iter;
		for (iter = Parent->commserv.list.begin();
				    iter != Parent->commserv.list.end();
				    iter++)
		{
		    if (iter->second.IsOn(sourceL))
		    {
			for (iter->second.message = iter->second.MSG_begin();
			    iter->second.message != iter->second.MSG_end();
			    iter->second.message++)
			{
			    Parent->servmsg.send(sourceL, "[" + IRC_Bold +
					    iter->first + IRC_Off + "] " +
					    iter->second.message->Entry());
			}
		    }
		}
		if (Parent->nickserv.IsStored(sourceL))
		{
		
		    if (Parent->nickserv.stored[sourceL].Forbidden())
		    {
			Parent->nickserv.send(sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/FORBIDDEN"),
						ToHumanTime(Parent->nickserv.Ident()).c_str());
		    }
		    else if (Parent->nickserv.stored[sourceL].Protect() &&
		    	     !Parent->nickserv.stored[sourceL].IsOnline())
		    {
			Parent->nickserv.send(sourceL,
			    Parent->getMessage(sourceL, "ERR_SITUATION/PROTECTED"),
			    ToHumanTime(Parent->nickserv.Ident()).c_str());
		    }
		}
	    }
	    else
	    {
		// CHANGE NICK
		if (!Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
		{
		    Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()] =
			Parent->nickserv.live[sourceL];
		}
		else if (sourceL != data.ExtractWord(3, ": ").LowerCase())
		{
		    KillUnknownUser(data.ExtractWord(3, ": "));
		}
		Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].
		    Name(data.ExtractWord(3, ": "));

		if (sourceL != data.ExtractWord(3, ": ").LowerCase())
		{
		    Parent->nickserv.live.erase(sourceL);
		    // We just did a SVSNICK ...
		    if (Parent->nickserv.recovered.find(source.LowerCase()) !=
			    Parent->nickserv.recovered.end())
		    {
			Parent->server.NICK(source,
				(Parent->startup.Ownuser() ?
				sourceL : Parent->startup.Services_User()),
				Parent->startup.Services_Host(),
				Parent->startup.Server_Name(),
				"Nickname Enforcer");
		    }
		}
	    }
	}
	else if (msgtype=="NOTICE")
	{
	    // :source NOTICE target/#channel :message
	    // NOTICE target :message
	    if (!Parent->GotConnect())
		return;

	
	
	    if (source == "")
		if (!IsChan(data.ExtractWord(2, ": ")))
		    Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"NOTICE", Parent->startup.Server_Name().c_str(), data.ExtractWord(2, ": ").c_str());
	    else
		if (!IsChan(data.ExtractWord(3, ": ")))
		    Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"NOTICE", source.c_str(), data.ExtractWord(3, ": ").c_str());
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'O':
	if (msgtype=="OPER")
	{
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'P':
	if (msgtype=="PART")
	{
	    if (source.Contains(".") || source == "")
		return;

	    // :source PART #channel :reason
	    Parent->nickserv.live[sourceL].Part(data.ExtractWord(3, ": "));
	}
	else if (msgtype=="PASS")
	{
	    // PASS :password
	    if (data.ExtractWord(2, ": ") != Parent->startup.Server(Parent->Server()).second)
	    {
		Log(LM_ERROR, Parent->getLogMessage("OTHER/WRONGPASS"),
			Parent->Server().c_str());
		CP(("Server password mismatch.  Closing socket."));
		raw(((proto.Tokens() && proto.GetNonToken("ERROR") != "") ?
			proto.GetNonToken("ERROR") : mstring("ERROR")) +
			" :No Access (passwd mismatch) [" + Parent->Server() + "]");
		raw(((proto.Tokens() && proto.GetNonToken("ERROR") != "") ?
			proto.GetNonToken("ERROR") : mstring("ERROR")) +
			" :Closing Link: [" + Parent->Server() + "] (Bad Password)");

		Parent->Disconnect();
	    }
            else
	    {
		Parent->GotConnect(true);
		SignOnAll();
            }
	}
	else if (msgtype=="PING")
	{
	    // PING :some.server
	    // :some.server PING some.server :our.server
	    if (source != "")
		sraw(((proto.Tokens() && proto.GetNonToken("PONG") != "") ?
			proto.GetNonToken("PONG") : mstring("PONG")) +
			" " + Parent->startup.Server_Name() + " :" + source);
	    else
		sraw(((proto.Tokens() && proto.GetNonToken("PONG") != "") ?
			proto.GetNonToken("PONG") : mstring("PONG")) +
			" " + Parent->startup.Server_Name() + " :" +
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
	    if (!Parent->GotConnect())
		return;

	    if (source == "")
		if (!IsChan(data.ExtractWord(2, ": ")))
		    Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"PRIVMSG", Parent->startup.Server_Name().c_str(), data.ExtractWord(2, ": ").c_str());
	    else
		if (!IsChan(data.ExtractWord(3, ": ")))
		    Log(LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"PRIVMSG", source.c_str(), data.ExtractWord(3, ": ").c_str());
	}
	else if (msgtype=="PROTOCTL")
	{
	    // Turn on tokens dynamically ...
	    if ((data + " ").Contains(" TOKEN "))
		proto.Tokens(true);
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'Q':
	if (msgtype=="QUIT")
	{
	    // :source QUIT :reason
	    // :source QUIT :server server

	    if (source.Contains(".") || source == "")
		return;

	    // OK, 4 words (always for squit), the 4nd word is a server
	    // and the 3rd word is the uplink of the 4th word (a server)
	    if (data.WordCount(": ")==4 && IsServer(data.ExtractWord(4, ": ")) &&
		ServerList[data.ExtractWord(4, ": ").LowerCase()].Uplink() == data.ExtractWord(3, ": ").LowerCase())
	    {
		/* Suspected SQUIT
		 *
		 * - Add user to ToBeSquit map under servername
		 * - Add data(4) to ServerSquit map with a timer to clear
		 *
		 * IF no SQUIT message received, user is QUIT and server
		 * is removed from ServerSquit map -- ie. its FAKE!
		 */
		Parent->nickserv.live[sourceL].SetSquit();
		ToBeSquit[data.ExtractWord(4, ": ").LowerCase()].push_back(sourceL);
		Log(LM_NOTICE, Parent->getLogMessage("OTHER/SQUIT_FIRST"),
			data.ExtractWord(4, ": ").c_str(),
			data.ExtractWord(3, ": ").c_str());

		if (ServerSquit.find(Parent->nickserv.live[sourceL].Server().LowerCase()) == ServerSquit.end())
		{
		    ServerSquit[Parent->nickserv.live[sourceL].Server().LowerCase()] =
			ACE_Reactor::instance()->schedule_timer(&tobesquit,
				new mstring(Parent->nickserv.live[sourceL].Server().LowerCase()),
				ACE_Time_Value(10));
		}
	    }
	    else
	    {
		// Normal quit ...

		// Kind of illegal to do, but accomodate anyway, re-signon
		// services if someone quits them (how?!?)
		if (Parent->nickserv.live[sourceL].IsServices())
		    sraw(((proto.Tokens() && proto.GetNonToken("ISON") != "") ?
			proto.GetNonToken("ISON") : mstring("ISON")) +
			" " + sourceL);
		Parent->nickserv.live[sourceL].Quit(data.After(":", 2));
		Parent->nickserv.live.erase(sourceL);
	    }
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
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
	    // Will we ever get this via. net??  ignore.
	}
	else if (msgtype=="RESTART")
	{
	    // Will we ever get this via. net??  ignore.
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
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
			ACE_OS::atoi(data.ExtractWord(3, ": ").LowerCase().c_str()),
			data.After(":"));
		Log(LM_INFO, Parent->getLogMessage("OTHER/LINK"),
			data.ExtractWord(2, ": ").c_str(),
			Parent->startup.Server_Name().c_str());
	    }
	    else
	    {
		if (IsServer(sourceL))
		{
		    ServerList[data.ExtractWord(3, ": ").LowerCase()] = Server(
			data.ExtractWord(3, ": ").LowerCase(),
			sourceL,
			ACE_OS::atoi(data.ExtractWord(4, ": ").LowerCase().c_str()),
			data.After(":", 2));
		    Log(LM_INFO, Parent->getLogMessage("OTHER/LINK"),
			data.ExtractWord(3, ": ").c_str(), sourceL.c_str());
		}
		else
		{
		    // Uplink (source) does not exist, WTF?!?
		}
	    }
	}
	else if (msgtype=="SJOIN")
	{
	    //:server SJOIN chan-stamp #channel +modes extra-params :@opd_nick +voice_nick nick 
	    //:relic.devel.relic.net SJOIN 952608432 #blah + <none> :@Kwahraw +PreZ Zephyr
	    unsigned int i;

	    vector<mstring> users;
	    mstring modes = data.ExtractWord(5, ": ");
	    mstring mode_params = "", nick;
	    bool oped, voiced;	    if (modes.Contains("l") || modes.Contains("k"))
		mode_params = data.Before(":", 2).After(" ", 5);
	    for (i=0; i < data.After(":", 2).WordCount(" "); i++)
	    {
		nick = data.After(":", 2).ExtractWord(i+1, " ");
		if (nick != "")
		{
		    oped = voiced = false;
		    if (nick[0u] == '@')
		    {
			oped = true;
			nick.Replace("@", "");
		    }
		    if (nick[0u] == '+')
		    {
			voiced = true;
			nick.Replace("+", "");
		    }
		    if (Parent->nickserv.IsLive(nick))
		    {
			if (oped)
			{
			    modes += "o";
			    mode_params += " " + nick;
			}
			if (voiced)
			{
			    modes += "v";
			    mode_params += " " + nick;
			}
			users.push_back(nick);
		    }
		}
	    }

	    for (i=0; i<users.size(); i++)
		Parent->nickserv.live[users[i].LowerCase()].Join(data.ExtractWord(4, ": "));
	    CP(("MODE TO %s: %s", data.ExtractWord(4, ": ").LowerCase().c_str(),
			(modes + " " + mode_params).c_str()));
	    if (Parent->chanserv.IsLive(data.ExtractWord(4, ": ")))
		Parent->chanserv.live[data.ExtractWord(4, ": ").LowerCase()].Mode(source,
									modes + " " + mode_params);
	}
	else if (msgtype=="SNICK")
	{
	    if (source.Contains("."))
		return;

	    // NEW USER
	    sourceL = data.ExtractWord(2, ": ").LowerCase();

	    // DONT kill when we do SQUIT protection.
	    map<mstring,list<mstring> >::iterator i;
	    for (i=ToBeSquit.begin(); i!=ToBeSquit.end(); i++)
	    {
		list<mstring>::iterator k;
		for (k=i->second.begin(); k!=i->second.end(); k++)
		    if (*k == sourceL)
		    {
			i->second.erase(k);
			break;
		    }
	    }

	    if (Parent->nickserv.IsLive(sourceL))
	    {
		// IF the squit server = us, and the signon time matches
		if (Parent->nickserv.live[sourceL].Squit() == data.ExtractWord(7, ": ").LowerCase()
		    && Parent->nickserv.live[sourceL].SignonTime() == mDateTime((time_t) atol(data.ExtractWord(4, ": "))))
		{
		    Parent->nickserv.live[sourceL].ClearSquit();
		    return;    // nice way to avoid repeating ones self :)
		}
		else
		{
		    Parent->nickserv.live[sourceL].Quit("SQUIT - " + Parent->nickserv.live[sourceL].Server());
		    Parent->nickserv.live.erase(sourceL);
		}
	    }

	    // hops = servers from us
	    // services = 1 for service, 0 for user
	    // DAL4.4.15+ SNICK name hops time user host server services modes :real name

	    switch (proto.Signon())
	    {
	    case 0000:
	    case 0001:
		break;
	    case 1000: // SNICK nick hops time user host server modes :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(8, ": "));
		break;
	    case 1001: // SNICK nick hops time user host server 1 modes :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(9, ": "));
		break;
	    case 1002: // SNICK nick hops time user host server 0 real-host modes :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(9, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(6, ": "));
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(10, ": "));
		break;
	    case 1003: // SNICK nick hops time user real-host host server 0 modes :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(8, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(7, ": "));
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(10, ": "));
		break;
	    }
	    if (i_UserMax < Parent->nickserv.live.size())
		i_UserMax = Parent->nickserv.live.size();

	    // HAS to be AFTER the nickname is added to map.
	    map<mstring, Committee>::iterator iter;
	    for (iter = Parent->commserv.list.begin();
				    iter != Parent->commserv.list.end();
				    iter++)
	    {
		if (iter->second.IsOn(sourceL))
		{
		    for (iter->second.message = iter->second.MSG_begin();
			    iter->second.message != iter->second.MSG_end();
			    iter->second.message++)
		    {
			Parent->servmsg.send(sourceL, "[" + IRC_Bold +
					    iter->first + IRC_Off + "] " +
					    iter->second.message->Entry());
		    }
		}
	    }
	    if (Parent->nickserv.IsStored(sourceL))
	    {
		if (Parent->nickserv.stored[sourceL].Forbidden())
		{
		    Parent->nickserv.send(sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/FORBIDDEN"),
						ToHumanTime(Parent->nickserv.Ident()).c_str());
		}
		else if (Parent->nickserv.stored[sourceL].Protect() &&
			!Parent->nickserv.stored[sourceL].IsOnline())
		{
		    Parent->nickserv.send(sourceL,
			    Parent->getMessage(sourceL, "ERR_SITUATION/PROTECTED"),
					ToHumanTime(Parent->nickserv.Ident()).c_str());
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
	    // :PreZ SQUIT server :reason
	    mstring target;
	    if (source == "")
		target = data.ExtractWord(2, ": ").LowerCase();
	    else
		target = data.ExtractWord(3, ": ").LowerCase();

	    Log(LM_NOTICE, Parent->getLogMessage("OTHER/SQUIT_SECOND"),
		    target.c_str(),
		    ServerList[target.LowerCase()].Uplink().c_str());

	    ServerList.erase(target);
	    if (ServerSquit.find(target) != ServerSquit.end())
	    {
		mstring *arg = NULL;
		if (ACE_Reactor::instance()->cancel_timer(
			ServerSquit[target], (const void **) arg)
		    && arg != NULL)
		    delete arg;
		ServerSquit[target] =
		    ACE_Reactor::instance()->schedule_timer(&squit,
			new mstring(target),
			ACE_Time_Value(Parent->config.Squit_Protect()));
	    }
	    ToBeSquit.erase(target);
	    map<mstring,Nick_Live_t>::iterator iter;
	    vector<mstring> chunked;
	    for (iter=Parent->nickserv.live.begin(); iter != Parent->nickserv.live.end(); iter++)
	    {
		if (iter->second.IsServices() && ServerList.size() == 0)
		{
		    chunked.push_back(iter->first);
		}
		else if (iter->second.Server() == target)
		    iter->second.SetSquit();
	    }
	    // Sign off services if we have NO uplink
	    unsigned int i;
	    for (i=0; i<chunked.size(); i++)
		QUIT(chunked[i], "SQUIT - " + target);
	}
	else if (msgtype=="STATS")
	{
	    // :source STATS type :our.server
	    //:temple.magick.tm 219 ChanServ o :End of /STATS report
	    sraw("219 " + source + " " + data.ExtractWord(3, ": ") +
		" :End of /STATS report");
	}
	else if (msgtype=="SUMMON")
	{
	    // :source SUMMON user our.server *
	    sraw("445 " + source + " :SUMMON has been disabled");
	}
	else if (msgtype=="SVSHOST")
	{
	    // :source SVSHOST user newhost
	    if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].AltHost(data.ExtractWord(4, ": "));
	    }
	}
	else if (msgtype=="SVSKILL")
	{
	    // Same as KILL (but by services)
	}
	else if (msgtype=="SVSMODE")
	{
	    // Handle just as mode, WITHOUT sanity
	    if (IsChan(data.ExtractWord(3, ": ")))
	    {
		if (Parent->chanserv.IsLive(data.ExtractWord(3, ": ")))
		{
		    Parent->chanserv.live[data.ExtractWord(3, ": ").LowerCase()].Mode(source, data.After(" ", 3));
		}
		else
		{
		    Log(LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
			"SVSMODE", source.c_str(),  data.ExtractWord(3, ": ").c_str());
		}
	    }
	    else
	    {
		if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
		{
		    Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].Mode(data.ExtractWord(4, ": "));
		}
		else
		{
		    Log(LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"SVSMODE", source.c_str(),  data.ExtractWord(3, ": ").c_str());
		}
	    }
	}
	else if (msgtype=="SVSNICK")
	{
	    // forcably changed nicks (handle like nick)
	}
	else if (msgtype=="SVSNOOP")
	{
	    // Deny all OPERS on server, ignore.
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'T':
	if (msgtype=="TIME")
	{
	    // :source TIME :our.server
	    sraw("391 " + source + " :" + Now().DateTimeString());
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
			source,
		        data.After(":", 2),
		        data.ExtractWord(4, ": "),
		        (time_t) atol(data.ExtractWord(5, ": "))
		    );
		}
		else
		{ // Clearing
		    Parent->chanserv.live[data.ExtractWord(3, ": ").LowerCase()].Topic(
		        source, "", data.ExtractWord(4, ": "));
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
	    if (Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }



	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
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
	    if (source.Contains("."))
		return;

	    // NEW USER
	    sourceL = data.ExtractWord(2, ": ").LowerCase();

	    // DONT kill when we do SQUIT protection.
	    map<mstring,list<mstring> >::iterator i;
	    for (i=ToBeSquit.begin(); i!=ToBeSquit.end(); i++)
	    {
		list<mstring>::iterator k;
		for (k=i->second.begin(); k!=i->second.end(); k++)
		    if (*k == sourceL)
		    {
		        list<mstring>::iterator j = k;  j--;
		        i->second.erase(k);
		        k=j;
		    }
	    }

	    if (Parent->nickserv.IsLive(sourceL))
	    {
	        // IF the squit server = us, and the signon time matches
	        if (Parent->nickserv.live[sourceL].Squit() == data.ExtractWord(7, ": ").LowerCase()
	    	    && Parent->nickserv.live[sourceL].SignonTime() == mDateTime((time_t) atol(data.ExtractWord(4, ": "))))
	        {
	    	    Parent->nickserv.live[sourceL].ClearSquit();
	    	    return;    // nice way to avoid repeating ones self :)
	        }
	        else
	        {
	    	    Parent->nickserv.live[sourceL].Quit("SQUIT - " + Parent->nickserv.live[sourceL].Server());
	    	    Parent->nickserv.live.erase(sourceL);
	        }
	    }

	    switch (proto.Signon())
	    {
	    case 0000: // USER nick user host server :realname
	        Parent->nickserv.live[sourceL] =
		    Nick_Live_t(
			data.ExtractWord(2, ": "),
			time(NULL),
			data.ExtractWord(5, ": "),
			data.ExtractWord(3, ": "),
			data.ExtractWord(4, ": "),
			data.After(":")
		    );
		break;
	    case 0001: // USER nick time user host server :realname
	        Parent->nickserv.live[sourceL] =
		    Nick_Live_t(
			data.ExtractWord(2, ": "),
			(time_t) atol(data.ExtractWord(3, ": ")),
			data.ExtractWord(6, ": "),
			data.ExtractWord(4, ": "),
			data.ExtractWord(5, ": "),
			data.After(":")
		    );
		break;
	    case 1000:
	    case 1001:
	    case 1002:
	    case 1003:
		break;
	    }
	    if (i_UserMax < Parent->nickserv.live.size())
		i_UserMax = Parent->nickserv.live.size();

	    // HAS to be AFTER the nickname is added to map.
	    map<mstring, Committee>::iterator iter;
	    for (iter = Parent->commserv.list.begin();
				    iter != Parent->commserv.list.end();
				    iter++)
	    {
		if (iter->second.IsOn(sourceL))
		{
		    for (iter->second.message = iter->second.MSG_begin();
			    iter->second.message != iter->second.MSG_end();
			    iter->second.message++)
		    {
			Parent->servmsg.send(sourceL, "[" + IRC_Bold +
					    iter->first + IRC_Off + "] " +
					    iter->second.message->Entry());
		    }
		}
	    }
	    if (Parent->nickserv.IsStored(sourceL))
	    {
		if (Parent->nickserv.stored[sourceL.LowerCase()].Forbidden())
		{
		    Parent->nickserv.send(sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/FORBIDDEN"),
						ToHumanTime(Parent->nickserv.Ident()).c_str());
		}
		else if (Parent->nickserv.stored[sourceL].Protect() &&
			!Parent->nickserv.stored[sourceL].IsOnline())
		{
		    Parent->nickserv.send(sourceL,
			    Parent->getMessage(sourceL, "ERR_SITUATION/PROTECTED"),
					ToHumanTime(Parent->nickserv.Ident()).c_str());
		}
	    }
	}
	else if (msgtype=="USERHOST")
	{
	    if (data.WordCount(": ") > 2)
	    {
		sraw("461 " + source + " USERHOST :Not enough paramaters");
	    }
	    else if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		mstring target = Parent->getLname(data.ExtractWord(3, ": "));
		if (!Parent->nickserv.IsStored(target) ? 1 :
			!Parent->nickserv.stored[target.LowerCase()].Private())
		{
		    sraw("302 " + source + " :" +
			Parent->nickserv.live[target.LowerCase()].Name() +
			"*=-" +
			Parent->nickserv.live[target.LowerCase()].User() +
			"@" +
			Parent->nickserv.live[target.LowerCase()].AltHost());
		}
		else
		{
		    sraw("302 " + source + " :" +
			Parent->nickserv.live[target.LowerCase()].Name() +
			"*=-" +
			Parent->nickserv.live[target.LowerCase()].User() +
			"@" + Parent->getMessage("MISC/ONLINE"));
		}

	    }
	    else
	    {
		sraw("401 " + source + " " + data.ExtractWord(3, ": ") +
			" :No such nickname/channel.");
	    }

	}
	else if (msgtype=="USERS")
	{
	    // :source USERS :our.server
	    sraw("446 " + source + " :USERS has been disabled");
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'V':
	if (msgtype=="VERSION")
	{
	    // :source VERSION :our.server
	    //:temple.magick.tm 351 ChanServ dal4.4.17. temple.magick.tm :AiMnW
	    mstring tmp(VERSION);
	    if(RELEASE!="")
		tmp+="-"+RELEASE;
	    if(PATCH1!="")
		tmp+="+"+PATCH1;
	    if(PATCH2!="")
		tmp+="+"+PATCH2;
	    if(PATCH3!="")
		tmp+="+"+PATCH3;
	    if(PATCH4!="")
		tmp+="+"+PATCH4;
	    if(PATCH5!="")
		tmp+="+"+PATCH5;
	    if(PATCH6!="")
		tmp+="+"+PATCH6;
	    if(PATCH7!="")
		tmp+="+"+PATCH7;
	    if(PATCH8!="")
		tmp+="+"+PATCH8;
	    if(PATCH9!="")
		tmp+="+"+PATCH9;
	    tmp+=" [";
	    if(Parent->operserv.GetNames() != "")
		tmp+="O";
	    else
		tmp+="o";
	    if(Parent->nickserv.GetNames() != "")
		tmp+="N";
	    else
		tmp+="n";
	    if(Parent->chanserv.GetNames() != "")
		tmp+="C";
	    else
		tmp+="c";
	    if(Parent->memoserv.GetNames() != "")
		tmp+="M";
	    else
		tmp+="m";
	    if(Parent->commserv.GetNames() != "")
		tmp+="S";
	    else
		tmp+="s";
	    if(Parent->servmsg.GetNames() != "")
		tmp+="H";
	    else
		tmp+="h";
	    if(Parent->servmsg.ShowSync())
		tmp+="Y";
	    else
		tmp+="y";
	    tmp << Parent->startup.Level() << "] Build #" << BUILD_NUMBER <<
		" (" << BUILD_TIME << ") " << BUILD_SYS << "/" << BUILD_TYPE << ".";
	    sraw("351 " + source + " " + PACKAGE + " " + Parent->startup.Server_Name() + " :" + tmp);
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
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
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net ServMsg H :2 Global Noticer
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
	    if (Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }

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

	    mstring target = data.ExtractWord(3, ": ");
	    mstring targetL = target.LowerCase();
	    if (Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		sraw("318 " + source + " " + target + " :End of /WHOIS list.");
		return;
	    }
	    else if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		sraw("311 " + source + " " + target + " " + Parent->nickserv.live[targetL].User() +
			" " + Parent->nickserv.live[targetL].Host() + " * :" +
			Parent->nickserv.live[targetL].RealName());

		if (Parent->nickserv.live[targetL].IsRecognized())
		{
		    sraw("307 " + source + " " + target + " : is a registered nick");
		}

		set<mstring> chans = Parent->nickserv.live[targetL].Channels();
		set<mstring>::iterator iter;
		mstring outline = "319 " + source + " " + target + " :";
		for (iter=chans.begin(); iter!=chans.end(); iter++)
		{
		    if (outline.size() + iter->size() > proto.MaxLine())
		    {
			sraw(outline);
			outline = "319 " + source + " " + target + " :";
		    }

		    // Channel doesnt exist
		    if (!Parent->chanserv.IsLive(*iter))
			continue;

		    // Channel +p or +s, and source not in chan
		    if ((Parent->chanserv.live[*iter].HasMode("s") ||
				Parent->chanserv.live[*iter].HasMode("p")) &&
				!Parent->chanserv.live[*iter].IsIn(source))
			continue;

		    if (Parent->chanserv.live[*iter].IsOp(target))
			outline += "@" + *iter + " ";
		    else if (Parent->chanserv.live[*iter].IsVoice(target))
			outline += "+" + *iter + " ";
		    else
			outline += *iter + " ";
		}
		if (outline.After(":").Len() > 0)
		    sraw(outline);

		if (Parent->nickserv.live[targetL].IsServices())
		    sraw("312 " + source + " " + target + " " + Parent->startup.Server_Name() +
			" :" + Parent->startup.Server_Desc());
		else if (IsServer(Parent->nickserv.live[targetL].Server()))
		    sraw("312 " + source + " " + target + " " + Parent->nickserv.live[targetL].Server() +
			" :" + ServerList[Parent->nickserv.live[targetL].Server()].Description());


		if (Parent->nickserv.live[targetL].Away() != "")
		    sraw("301 " + source + " " + target + " :" + Parent->nickserv.live[targetL].Away());

		if (Parent->nickserv.live[targetL].HasMode("o"))
		    sraw("313 " + source + " " + target + " :is an IRC Operator");

		if (Parent->nickserv.live[targetL].HasMode("h"))
		    sraw("310 " + source + " " + target + " :looks very helpful.");

		if (Parent->nickserv.live[targetL].IsServices())
		{
    		    mstring signon_idletime;
		    signon_idletime << Parent->nickserv.live[targetL].IdleTime().timetstring()
			<< " " << Parent->nickserv.live[targetL].SignonTime().timetstring();
		    sraw("317 " + source + " " + target + " " + signon_idletime + " :seconds idle, signon time");
		}

		sraw("318 " + source + " " + target + " :End of /WHOIS list.");

	    }
	    else
	    {
		mstring target = data.ExtractWord(3, ": ");
		sraw("401 " + source + " " + target + " :No such nickname/channel.");
	    }

	}
	else if (msgtype=="WHOWAS")
	{
	    // Dont store 'previous user' info, so ignore.
	}
	else
	{
	    Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	}
	break;
    case 'X':
	Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	break;
    case 'Y':
	Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	break;
    case 'Z':
	Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	break;
    default:
	Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	break;
    }

}

void NetworkServ::numeric_execute(const mstring & data)
{
    FT("NetworkServ::numeric_execute", (data));
    unsigned int i;

    mstring source, sourceL;
    unsigned int msgtype;
    if (data[0u]==':')
    {
        source=data.ExtractWord(1,": ");
	sourceL=source.LowerCase();
        msgtype=ACE_OS::atoi(data.ExtractWord(2,": "));
	if (!(Parent->nickserv.IsLive(source) || source.Contains(".")))
	{
		KillUnknownUser(source);
		return;
	}
    }
    else
    {
        msgtype=ACE_OS::atoi(data.ExtractWord(1,": "));
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
	for (i=4; i<=data.WordCount(": "); i++)
	{
	    if (WaitIsOn.find(data.ExtractWord(i, ": ").LowerCase()) != WaitIsOn.end())
		WaitIsOn.erase(data.ExtractWord(i, ": "));
	}

	if (WaitIsOn.size())
	{
	    set<mstring>::iterator k;
	    for (k=WaitIsOn.begin(); k!=WaitIsOn.end(); k++)
	    {
		if (Parent->operserv.IsName(*k))
		    Parent->operserv.signon(*k);
		else if (Parent->nickserv.IsName(*k))
		    Parent->nickserv.signon(*k);
		else if (Parent->chanserv.IsName(*k))
		{
		    Parent->chanserv.signon(*k);

		    if (Parent->chanserv.FirstName() == *k)
		    {
			if (Parent->chanserv.Hide())
			    MODE(*k, "+s");
			mstring joinline;
			map<mstring,Chan_Stored_t>::iterator iter;
			for (iter=Parent->chanserv.stored.begin(); iter!=Parent->chanserv.stored.end(); iter++)
			{
			    // If its live and got JOIN on || not live and mlock +k or +i
			    if ((Parent->chanserv.IsLive(iter->first) && iter->second.Join()) ||
				(!Parent->chanserv.IsLive(iter->first) &&
				(iter->second.Mlock_On().Contains("k") ||
				iter->second.Mlock_On().Contains("i"))))
			    {
				if (joinline.Len())
				    joinline << ",";
				joinline << iter->first;
				if (joinline.Len() > proto.MaxLine())
				{
				    JOIN(Parent->chanserv.FirstName(), joinline);
				    joinline = "";
				}
			    }
			}
			if (joinline.Len())
			{
			    JOIN(Parent->chanserv.FirstName(), joinline);
			    joinline = "";
			}
		    }
		}
		else if (Parent->memoserv.IsName(*k))
		    Parent->memoserv.signon(*k);
		else if (Parent->commserv.IsName(*k))
		    Parent->commserv.signon(*k);
		else if (Parent->servmsg.IsName(*k))
		{
		    Parent->servmsg.signon(*k);
		    if (Parent->servmsg.FirstName() == *k)
			Parent->server.MODE(*k, "+o");
		}
		if (Parent->nickserv.IsLive(*k) && Parent->startup.Setmode() != "")
		    Parent->server.MODE(*k, "+" + Parent->startup.Setmode());
		FlushMsgs(*k);
	    }
	}
	WaitIsOn.clear();
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
	Log(LM_ERROR, Parent->getLogMessage("OTHER/WRONGPASS"),
		Parent->Server().c_str());
	Parent->Disconnect();
	break;
    case 465:     // ERR_YOUREBANNEDCREEP
	// MUST handle (Stop connecting).
	Log(LM_ERROR, Parent->getLogMessage("OTHER/WEAREBANNED"),
		Parent->Server().c_str());
	Parent->Disconnect();
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
	Log(LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str());
	break;
    }
}

void NetworkServ::load_database(wxInputStream& in)
{
}

void NetworkServ::save_database(wxOutputStream& out)
{
}
