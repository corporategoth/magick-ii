#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#pragma implementation "version.h"
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
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
#define RCSID(x,y) const char *rcsid_server_cpp_ ## x () { return y; }
RCSID(server_cpp, "@(#)$Id$");

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
#include "mconfig.h"

#ifndef MAGICK_HAS_EXCEPTIONS
static Server_t GLOB_Server_t;
#endif

unsigned long Protocol::Numeric_t::str_to_base64(const mstring & in) const
{
    BTCB();
    FT("Protocol::Numeric_t::str_to_base64", (in));

    if (!in.length())
	RET(0);

    unsigned long i = 0, v = char_to_base64[static_cast < unsigned char > (in[static_cast < size_t > (i++)])];

    while (i < in.length())
    {
	v <<= 6;
	v += char_to_base64[static_cast < unsigned char > (in[static_cast < size_t > (i++)])];
    }

    RET(v);
    ETCB();
}

mstring Protocol::Numeric_t::base64_to_str(unsigned long in) const
{
    BTCB();
    FT("Protocol::Numeric_t::base64_to_str", (in));

    // 32/6 == max 6 bytes for representation, +1 for the null
    char base64buf[7];

    memset(base64buf, base64_to_char[63], sizeof(base64buf));
    unsigned long i = sizeof(base64buf) - 1;

    base64buf[i] = '\0';

    do
    {
	base64buf[--i] = base64_to_char[in & 63];
    }
    while ((i > 0) && (in >>= 6));

    mstring retval = (base64buf + (Trim() ? i : 0));

    RET(retval);
    ETCB();
}

unsigned long Protocol::Numeric_t::ServerNumeric(const mstring & in) const
{
    BTCB();
    FT("Protocol::Numeric_t::ServerNumeric", (in));
    unsigned long rv = str_to_base64(in.Right(Server()));

    RET(rv);
    ETCB();
}

mstring Protocol::Numeric_t::ServerNumeric(unsigned long in) const
{
    BTCB();
    FT("Protocol::Numeric_t::ServerNumeric", (in));
    mstring rv = base64_to_str(in).Right(Server());

    RET(rv);
    ETCB();
}

unsigned long Protocol::Numeric_t::UserNumeric(const mstring & in) const
{
    BTCB();
    FT("Protocol::Numeric_t::UserNumeric", (in));
    unsigned long rv = str_to_base64(in.Right(User() + (Combine() ? Server() : 0)));

    RET(rv);
    ETCB();
}

mstring Protocol::Numeric_t::UserNumeric(unsigned long in) const
{
    BTCB();
    FT("Protocol::Numeric_t::UserNumeric", (in));
    mstring rv = base64_to_str(in).Right(User() + (Combine() ? Server() : 0));

    RET(rv);
    ETCB();
}

unsigned long Protocol::Numeric_t::ChannelNumeric(const mstring & in) const
{
    BTCB();
    FT("Protocol::Numeric_t::ChannelNumeric", (in));
    unsigned long rv = str_to_base64(in.Right(Channel()));

    RET(rv);
    ETCB();
}

mstring Protocol::Numeric_t::ChannelNumeric(unsigned long in) const
{
    BTCB();
    FT("Protocol::Numeric_t::ChannelNumeric", (in));
    mstring rv = base64_to_str(in).Right(Channel());

    RET(rv);
    ETCB();
}

bool Protocol::Set(const mstring & filename)
{
    BTCB();
    FT("Protocol::Set", (filename));

    DumpB();
    set < unsigned int > AkillTypes, SignonTypes;

    AkillTypes.insert(0000);
    AkillTypes.insert(1000);
    AkillTypes.insert(1001);
    AkillTypes.insert(1002);
    AkillTypes.insert(2000);
    AkillTypes.insert(2001);
    AkillTypes.insert(2002);
    AkillTypes.insert(3000);
    AkillTypes.insert(3001);
    SignonTypes.insert(0000);
    SignonTypes.insert(0001);
    SignonTypes.insert(1000);
    SignonTypes.insert(1001);
    SignonTypes.insert(1002);
    SignonTypes.insert(1003);
    SignonTypes.insert(2000);
    SignonTypes.insert(2001);
    SignonTypes.insert(2002);
    SignonTypes.insert(2003);
    SignonTypes.insert(3000);

    if (!mFile::Exists(filename))
    {
	RET(false);
    }
    mConfigEngine cfg(filename);

    unsigned int value_uint;
    mstring value_mstring;
    bool value_bool;

    mstring ts_Protocol = "Protocol/";
    mstring ts_Numeric = "Numeric/";
    mstring ts_Tokens = "Tokens/";

    cfg.Read(ts_Protocol + "NICKLEN", value_uint, 9);
    if (value_uint < 1)
    {
	LOG(LM_WARNING, "COMMANDLINE/CFG_SYNTAX", (ts_Protocol + "NICKLEN"));
	RET(false);
    }
    i_NickLen = value_uint;

    cfg.Read(ts_Protocol + "MAXLINE", value_uint, 450);
    if (value_uint < 64)
    {
	LOG(LM_WARNING, "COMMANDLINE/CFG_SYNTAX", (ts_Protocol + "MAXLINE"));
	RET(false);
    }
    i_MaxLine = value_uint;

    cfg.Read(ts_Protocol + "GLOBOPS", i_Globops, false);
    cfg.Read(ts_Protocol + "HELPOPS", i_Helpops, false);
    cfg.Read(ts_Protocol + "CHATOPS", i_Chatops, false);
    cfg.Read(ts_Protocol + "TOKENS", i_Tokens, false);
    cfg.Read(ts_Protocol + "TSORA", i_TSora, false);
    cfg.Read(ts_Protocol + "SJOIN", i_SJoin, false);
    cfg.Read(ts_Protocol + "BIGTOPIC", i_BigTopic, false);
    cfg.Read(ts_Protocol + "TOPICJOIN", i_TopicJoin, false);
    cfg.Read(ts_Protocol + "TOPICCURRENT", i_TopicCurrent, false);
    cfg.Read(ts_Protocol + "SERVERMODES", i_ServerModes, false);

    cfg.Read(ts_Protocol + "AKILL", value_uint, 0000);
    if (AkillTypes.find(value_uint) == AkillTypes.end())
    {
	NLOG(LM_WARNING, "COMMANDLINE/UNKNOWN_AKILL");
	RET(false);
    }
    i_Akill = value_uint;

    cfg.Read(ts_Protocol + "KILLAFTERAKILL", i_KillAfterAkill, false);

    cfg.Read(ts_Protocol + "SIGNON", value_uint, 0000);
    if (SignonTypes.find(value_uint) == SignonTypes.end())
    {
	NLOG(LM_WARNING, "COMMANDLINE/UNKNOWN_SIGNON");
	RET(false);
    }
    i_Signon = value_uint;

    cfg.Read(ts_Protocol + "MODES", value_uint, 3);
    if (value_uint < 1)
    {
	LOG(LM_WARNING, "COMMANDLINE/CFG_SYNTAX", (ts_Protocol + "MODES"));
	RET(false);
    }
    i_Modes = value_uint;

    cfg.Read(ts_Protocol + "SERVICESMODES", i_ServicesModes, "");
    cfg.Read(ts_Protocol + "CHANMODEARG", i_ChanModeArg, "ovbkl");
    cfg.Read(ts_Protocol + "FOUNDERMODE", i_FounderMode, "");
    cfg.Read(ts_Protocol + "SERVER", i_Server, "SERVER $1 $2 :$3");
    cfg.Read(ts_Protocol + "BURST", i_Burst, "");
    cfg.Read(ts_Protocol + "ENDBURST", i_EndBurst, "");
    cfg.Read(ts_Protocol + "PROTOCTL", i_Protoctl, "");

    cfg.Read(ts_Protocol + "SVSNICK", i_SVSNICK, "");
    cfg.Read(ts_Protocol + "SVSMODE", i_SVSMODE, "");
    cfg.Read(ts_Protocol + "SVSKILL", i_SVSKILL, "");
    cfg.Read(ts_Protocol + "SVSNOOP", i_SVSNOOP, "");
    cfg.Read(ts_Protocol + "SQLINE", i_SQLINE, "");
    cfg.Read(ts_Protocol + "UNSQLINE", i_UNSQLINE, "");
    cfg.Read(ts_Protocol + "SVSHOST", i_SVSHOST, "");

    cfg.Read(ts_Numeric + "TRIM", value_bool, false);
    Numeric.i_Trim = value_bool;
    cfg.Read(ts_Numeric + "SERVERNUMBER", value_bool, false);
    Numeric.i_ServerNumber = value_bool;
    cfg.Read(ts_Numeric + "COMBINE", value_bool, false);
    Numeric.i_Combine = value_bool;
    cfg.Read(ts_Numeric + "PREFIX", value_bool, false);
    Numeric.i_Prefix = value_bool;
    cfg.Read(ts_Numeric + "UNREALNUMERIC", value_bool, false);
    Numeric.i_UnrealNumeric = value_bool;
    cfg.Read(ts_Numeric + "SERVER", Numeric.i_Server, 0);
    cfg.Read(ts_Numeric + "FIELD", Numeric.i_Field, 0);
    cfg.Read(ts_Numeric + "USER", Numeric.i_User, 0);
    cfg.Read(ts_Numeric + "CHANNEL", Numeric.i_Channel, 0);

    cfg.Read(ts_Numeric + "BASE64", value_mstring, 0);
    vector < mstring > characters = value_mstring.Vector(" ,:");
    memset(Numeric.base64_to_char, 0, sizeof(Numeric.base64_to_char));
    memset(Numeric.char_to_base64, 0, sizeof(Numeric.char_to_base64));

    for (unsigned int i = 0; i < characters.size(); i++)
    {
	Numeric.base64_to_char[i] = characters[i] [0U];
	Numeric.char_to_base64[characters[i] [0U]] = (char) i;
    }

    map < mstring, mstring > file = cfg.GetMap();
    map < mstring, mstring >::iterator iter;
    for (iter = file.begin(); iter != file.end(); iter++)
    {
	if (iter->first.Left(ts_Tokens.length()).IsSameAs(ts_Tokens, true))
	{
	    tokens[iter->first.SubString(ts_Tokens.length())] = iter->second;
	}
    }

    DumpE();

    RET(true);
    ETCB();
}

mstring Protocol::GetToken(const mstring & in) const
{
    BTCB();
    FT("Protocol::GetToken", (in));
    map < mstring, mstring >::const_iterator iter = tokens.find(in);
    if (iter == tokens.end())
	RET("");
    RET(iter->second);
    ETCB();
}

mstring Protocol::GetNonToken(const mstring & in) const
{
    BTCB();
    FT("Protocol::GetNonToken", (in));
    mstring retval;

    map < mstring, mstring >::const_iterator iter = find_if(tokens.begin(), tokens.end(), ValueIsSameAs(in));
    if (iter != tokens.end())
	retval = iter->first;
    RET(retval);
    ETCB();
}

void Protocol::DumpB() const
{
    BTCB();
    MB(0,
       (i_NickLen, i_MaxLine, i_Globops, i_Helpops, i_Chatops, i_Tokens, i_TSora, i_SJoin, i_BigTopic, i_TopicJoin,
	i_TopicCurrent, i_Akill, i_KillAfterAkill, i_Signon, i_Modes, i_ChanModeArg));
    MB(16,
       (i_Server, i_Burst, i_EndBurst, i_Protoctl, i_SVSNICK, i_SVSMODE, i_SVSKILL, i_SVSNOOP, i_SQLINE, i_UNSQLINE,
        i_SVSHOST, tokens.size()));
    ETCB();
}

void Protocol::DumpE() const
{
    BTCB();
    ME(0,
       (i_NickLen, i_MaxLine, i_Globops, i_Helpops, i_Chatops, i_Tokens, i_TSora, i_SJoin, i_BigTopic, i_TopicJoin,
	i_TopicCurrent, i_Akill, i_KillAfterAkill, i_Signon, i_Modes, i_ChanModeArg));
    ME(16,
       (i_Server, i_Burst, i_EndBurst, i_Protoctl, i_SVSNICK, i_SVSMODE, i_SVSKILL, i_SVSNOOP, i_SQLINE, i_UNSQLINE,
        i_SVSHOST, tokens.size()));
    ETCB();
}

void Server_t::defaults()
{
    BTCB();
    NFT("Server_t::defaults");
    ref_class::lockData(mVarArray(lck_Server, lck_list, i_Name.LowerCase()));
    i_Hops = 0;
    i_Ping = 0.0;
    i_Lag = 0;
    i_Jupe = false;
    ETCB();
}

Server_t::Server_t()
{
    BTCB();
    NFT("Server_t::Server_t");
    defaults();
    DumpE();
    ETCB();
}

Server_t::Server_t(const mstring & name, const mstring & description, const unsigned long numeric) : i_Name(name.LowerCase()),
i_AltName(name.LowerCase()), i_Numeric(numeric), i_Uplink(Magick::instance().startup.Server_Name().LowerCase()),
i_Description(description)
{
    BTCB();
    FT("Server_t::Server_t", (name, description, numeric));
    defaults();
    i_Jupe = true;
    DumpE();
    ETCB();
}

Server_t::Server_t(const mstring & name, const int hops, const mstring & description,
		   const unsigned long numeric) : i_Name(name.LowerCase()), i_AltName(name.LowerCase()), i_Numeric(numeric),
i_Uplink(Magick::instance().startup.Server_Name().LowerCase()), i_Description(description)
{
    BTCB();
    FT("Server_t::Server_t", (name, hops, description, numeric));
    defaults();
    i_Hops = hops;
    Magick::instance().server.OurUplink(i_Name);
    DumpE();
    ETCB();
}

Server_t::Server_t(const mstring & name, const mstring & uplink, const int hops, const mstring & description,
		   const unsigned long numeric) : i_Name(name.LowerCase()), i_AltName(name.LowerCase()), i_Numeric(numeric),
i_Uplink(uplink.LowerCase()), i_Description(description)
{
    BTCB();
    FT("Server_t::Server_t", (name, uplink, hops, description, numeric));
    defaults();
    i_Hops = hops;
    DumpE();
    ETCB();
}

Server_t &Server_t::operator=(const Server_t & in)
{
    BTCB();
    FT("Server_t::operator=", ("(const Server_t &) in"));
    i_Name = in.i_Name;
    ref_class::lockData(mVarArray(lck_Server, lck_list, i_Name.LowerCase()));
    i_AltName = in.i_AltName;
    i_Numeric = in.i_Numeric;
    i_Uplink = in.i_Uplink;
    i_Hops = in.i_Hops;
    i_Description = in.i_Description;
    i_Ping = in.i_Ping;
    i_Lag = in.i_Lag;
    i_Jupe = in.i_Jupe;
    NRET(Server_t &, *this);
    ETCB();
}

mstring Server_t::AltName() const
{
    BTCB();
    NFT("Server_t::AltName");
    RLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_AltName"));
    RET(i_AltName);
    ETCB();
}

void Server_t::AltName(const mstring & in)
{
    BTCB();
    FT("Server_t::AltName", (in));
    WLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_AltName"));
    MCB(i_AltName);
    i_AltName = in;
    MCE(i_AltName);
    ETCB();
}

unsigned long Server_t::Numeric() const
{
    BTCB();
    NFT("Server_t::Numeric");
    RLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_Numeric"));
    RET(i_Numeric);
    ETCB();
}

void Server_t::Numeric(const unsigned long in)
{
    BTCB();
    FT("Server_t::Numeric", (in));
    WLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_Numeric"));
    MCB(i_Numeric);
    i_Numeric = in;
    MCE(i_Numeric);
    ETCB();
}

mstring Server_t::Uplink() const
{
    BTCB();
    NFT("Server_t::Uplink");
    RLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_Uplink"));
    RET(i_Uplink);
    ETCB();
}

int Server_t::Hops() const
{
    BTCB();
    NFT("Server_t::Hops");
    RLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_Hops"));
    RET(i_Hops);
    ETCB();
}

mstring Server_t::Description() const
{
    BTCB();
    NFT("Server_t::Description");
    RLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_Description"));
    RET(i_Description);
    ETCB();
}

void Server_t::Ping()
{
    BTCB();
    NFT("Server_t::Ping");

    WLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_Ping"));
    if (i_Ping == mDateTime(0.0))
    {
	Magick::instance().server.sraw(((Magick::instance().server.proto.Tokens() &&
					 !Magick::instance().server.proto.GetNonToken("PING").empty()) ?
					 Magick::instance().server.proto.GetNonToken("PING") :
					 mstring("PING")) + " " + Magick::instance().startup.Server_Name() + " :" + i_Name);
	MCB(i_Ping);
	i_Ping = mDateTime::CurrentDateTime();
	MCE(i_Ping);
    }
    ETCB();
}

void Server_t::Pong()
{
    BTCB();
    NFT("Server_t::Pong");
    WLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_Ping"));
    if (i_Ping != mDateTime(0.0))
    {
	WLOCK2((lck_Server, lck_list, i_Name.LowerCase(), "i_Lag"));
	MCB(i_Lag);
	CB(1, i_Ping);
	i_Lag = static_cast < float > (i_Ping.MSecondsSince() / 1000000.0);

	COM(("The lag time of %s is %3.3f seconds.", i_Name.c_str(), i_Lag));
	i_Ping = mDateTime(0.0);
	CE(1, i_Ping);
	MCE(i_Lag);
    }
    ETCB();
}

float Server_t::Lag() const
{
    BTCB();
    NFT("Server_t::Lag");
    RLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_Lag"));
    RET(i_Lag);
    ETCB();
}

bool Server_t::Jupe() const
{
    BTCB();
    NFT("Server_t::Jupe");
    RLOCK((lck_Server, lck_list, i_Name.LowerCase(), "i_Jupe"));
    RET(i_Jupe);
    ETCB();
}

unsigned int Server_t::Users() const
{
    BTCB();
    NFT("Server_t::Users");

    unsigned int count = 0;

    NickServ::live_t::iterator k;
    {
	RLOCK((lck_NickServ, lck_live));
	for (k = Magick::instance().nickserv.LiveBegin(); k != Magick::instance().nickserv.LiveEnd(); k++)
	{
	    map_entry < Nick_Live_t > nlive(k->second);
	    if (nlive->Server().IsSameAs(i_Name, true))
		count++;
	}
    }
    RET(count);
    ETCB();
}

unsigned int Server_t::Opers() const
{
    BTCB();
    NFT("Server_t::Opers");

    unsigned int count = 0;

    NickServ::live_t::iterator k;
    {
	RLOCK((lck_NickServ, lck_live));
	for (k = Magick::instance().nickserv.LiveBegin(); k != Magick::instance().nickserv.LiveEnd(); k++)
	{
	    map_entry < Nick_Live_t > nlive(k->second);
	    if (nlive->Server().IsSameAs(i_Name, true) && nlive->HasMode("o"))
		count++;
	}
    }
    RET(count);
    ETCB();
}

vector < mstring > Server_t::Downlinks() const
{
    BTCB();
    NFT("Server_t::Downlinks");
    vector < mstring > downlinks;
    Server::list_t::iterator iter;

    {
	RLOCK((lck_Server, lck_list));
	for (iter = Magick::instance().server.ListBegin(); iter != Magick::instance().server.ListEnd(); iter++)
	{
	    map_entry < Server_t > server(iter->second);
	    if (!i_Name.empty() && server->Uplink().IsSameAs(i_Name, true))
		downlinks.push_back(server->Name().LowerCase());
	}
    }
    NRET(vector < mstring >, downlinks);
    ETCB();
}

vector < mstring > Server_t::AllDownlinks() const
{
    BTCB();
    NFT("Server_t::AllDownlinks");
    vector < mstring > downlinks, uplinks, uplinks2;
    Server::list_t::iterator iter;
    bool found = false;

    {
	RLOCK((lck_Server, lck_list));
	for (iter = Magick::instance().server.ListBegin(); iter != Magick::instance().server.ListEnd(); iter++)
	{
	    map_entry < Server_t > server(iter->second);
	    if (server->Uplink().IsSameAs(i_Name, true))
	    {
		downlinks.push_back(server->Name().LowerCase());
		uplinks.push_back(server->Name().LowerCase());
		found = true;
	    }
	}
    }

    while (found)
    {
	found = false;
	for (unsigned int i = 0; i < uplinks.size(); i++)
	{
	    RLOCK((lck_Server, lck_list));
	    for (iter = Magick::instance().server.ListBegin(); iter != Magick::instance().server.ListEnd(); iter++)
	    {
		map_entry < Server_t > server(iter->second);
		if (server->Uplink().IsSameAs(uplinks[i], true))
		{
		    downlinks.push_back(server->Name().LowerCase());
		    uplinks2.push_back(server->Name().LowerCase());
		    found = true;
		}
	    }
	}
	uplinks.clear();
	uplinks = uplinks2;
	uplinks2.clear();
    }

    NRET(vector < mstring >, downlinks);
    ETCB();
}

size_t Server_t::Usage() const
{
    BTCB();
    size_t retval = 0;

    WLOCK((lck_Server, lck_list, i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += i_AltName.capacity();
    retval += i_Uplink.capacity();
    retval += sizeof(i_Hops);
    retval += i_Description.capacity();
    retval += sizeof(i_Ping);
    retval += sizeof(i_Lag);
    retval += sizeof(i_Jupe);

    return retval;
    ETCB();
}

mstring Protocol::Numeric_t::FindServerNumeric(unsigned long n) const
{
    BTCB();
    FT("Protocol::Numeric_t::FindServerNumeric", (n));
    mstring retval;

    Server::list_t::const_iterator iter;

    RLOCK((lck_Server, lck_list));
    for (iter = Magick::instance().server.ListBegin(); iter != Magick::instance().server.ListEnd(); iter++)
    {
	map_entry < Server_t > server(iter->second);
	if (server->Numeric() == n)
	{
	    retval = server->Name();
	    break;
	}
    }
    RET(retval);
    ETCB();
}

mstring Protocol::Numeric_t::FindUserNumeric(unsigned long n) const
{
    BTCB();
    FT("Protocol::Numeric_t::FindUserNumeric", (n));

    mstring retval;

    NickServ::live_t::const_iterator iter;

    RLOCK((lck_NickServ, lck_live));
    for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
    {
	map_entry < Nick_Live_t > nlive(iter->second);
	if (nlive->Numeric() == n)
	{
	    retval = nlive->Name();
	    break;
	}
    }
    RET(retval);
    ETCB();
}

mstring Protocol::Numeric_t::FindChannelNumeric(unsigned long n) const
{
    BTCB();
    FT("Protocol::Numeric_t::FindChannelNumeric", (n));

    mstring retval;

    ChanServ::live_t::const_iterator iter;

    RLOCK((lck_ChanServ, lck_live));
    for (iter = Magick::instance().chanserv.LiveBegin(); iter != Magick::instance().chanserv.LiveEnd(); iter++)
    {
	map_entry < Chan_Live_t > clive(iter->second);
	if (clive->Numeric() == n)
	{
	    retval = clive->Name();
	    break;
	}
    }
    RET(retval);
    ETCB();
}

unsigned long Protocol::Numeric_t::FindServerNumeric() const
{
    BTCB();
    NFT("Protocol::Numeric_t::FindServerNumeric");

    unsigned long ournumeric = Magick::instance().startup.Server(Magick::instance().CurrentServer()).second.third;

    unsigned long max = 1;

    for (int j = 0; j < Server(); j++)
	max *= 64;

    for (unsigned long i = 1; i < max; i++)
    {
	if (i == ournumeric)
	    continue;
	if (FindServerNumeric(i).empty())
	    RET(i);
    }

    RET(0);
    ETCB();
}

unsigned long Protocol::Numeric_t::FindUserNumeric() const
{
    BTCB();
    NFT("Protocol::Numeric_t::FindUserNumeric");

    unsigned long ournumeric = Magick::instance().startup.Server(Magick::instance().CurrentServer()).second.third;
    unsigned long i, max;
    int j;

    if (Combine())
    {
	i = ournumeric;
	for (j = 0; j < User(); j++)
	    i *= 64;

	max = ournumeric + 1;
	for (j = 0; j < User(); j++)
	    max *= 64;
    }
    else
    {
	i = 1;
	max = 1;
	for (j = 0; j < User(); j++)
	    max *= 64;
    }

    for (; i < max; i++)
    {
	if (FindUserNumeric(i).empty())
	    RET(i);
    }

    RET(0);
    ETCB();
}

unsigned long Protocol::Numeric_t::FindChannelNumeric() const
{
    BTCB();
    NFT("Protocol::Numeric_t::FindChannelNumeric");

    unsigned long max = 1;

    for (int j = 0; j < Channel(); j++)
	max *= 64;

    for (unsigned long i = 1; i < max; i++)
    {
	if (FindChannelNumeric(i).empty())
	    RET(i);
    }

    RET(0);
    ETCB();
}

mstring Protocol::Numeric_t::ServerLineNumeric(unsigned long n) const
{
    BTCB();
    FT("Protocol::Numeric_t::ServerLineNumeric", (n));
    mstring retval;

    if (ServerNumber())
	retval = n;
    else
    {
	if (Combine())
	    retval = ServerNumeric(n) + UserNumeric(255).Right(User());
	else
	    retval = ServerNumeric(n);
    }

    RET(retval);
    ETCB();
}

unsigned long Protocol::Numeric_t::ServerLineNumeric(const mstring & n) const
{
    BTCB();
    FT("Protocol::Numeric_t::ServerLineNumeric", (n));
    unsigned long retval = 0;

    if (ServerNumber())
	retval = atoi(n.c_str());
    else
    {
	if (Combine())
	    retval = ServerNumeric(n.Left(n.length() - User()));
	else
	    retval = ServerNumeric(n);
    }

    RET(retval);
    ETCB();
}

void Server_t::DumpB() const
{
    BTCB();
    MB(0, (i_Name, i_AltName, i_Numeric, i_Uplink, i_Hops, i_Description, i_Ping, i_Lag, i_Jupe));
    ETCB();
}

void Server_t::DumpE() const
{
    BTCB();
    ME(0, (i_Name, i_AltName, i_Numeric, i_Uplink, i_Hops, i_Description, i_Ping, i_Lag, i_Jupe));
    ETCB();
}

void Server::raw(const mstring & text) const
{
    BTCB();
    FT("Server::raw", (text));
    Magick::instance().send(text);
    ETCB();
}

void Server::sraw(const mstring & text) const
{
    BTCB();
    mstring out;

    if (!proto.Numeric.Server())
	out << ":" << Magick::instance().startup.Server_Name() << " ";
    else if (proto.Numeric.Server() && proto.Numeric.Prefix())
    {
	unsigned long ournumeric = Magick::instance().startup.Server(Magick::instance().CurrentServer()).second.third;

	out << proto.Numeric.ServerNumeric(ournumeric) + " ";
    }
    out << text;
    raw(out);
    ETCB();
}

void Server::nraw(const mstring & nick, const mstring & text) const
{
    BTCB();
    mstring out;

    mstring n = GetUser(nick);

    if (n.empty())
	return;

    if (proto.Numeric.User())
    {
	map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(n);
	if (nlive->Numeric())
	    out << proto.Numeric.UserNumeric(nlive->Numeric());
	else
	    out << ":" << nlive->Name();
    }
    else
	out << ":" << n;

    out << " " << text;
    raw(out);
    ETCB();
}

void Server::SignOnAll()
{
    BTCB();
    NFT("Server::SignOnAll");

    mstring doison;
    unsigned int i;

    WLOCK((lck_Server, "WaitIsOn"));
    for (i = 1; i <= Magick::instance().operserv.GetNames().WordCount(" "); i++)
    {
	if (!Magick::instance().nickserv.IsLive(Magick::instance().operserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += Magick::instance().operserv.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Magick::instance().operserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i = 1; i <= Magick::instance().nickserv.GetNames().WordCount(" "); i++)
    {
	if (!Magick::instance().nickserv.IsLive(Magick::instance().nickserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += Magick::instance().nickserv.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Magick::instance().nickserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i = 1; i <= Magick::instance().chanserv.GetNames().WordCount(" "); i++)
    {
	if (!Magick::instance().nickserv.IsLive(Magick::instance().chanserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += Magick::instance().chanserv.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Magick::instance().chanserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i = 1; i <= Magick::instance().memoserv.GetNames().WordCount(" "); i++)
    {
	if (!Magick::instance().nickserv.IsLive(Magick::instance().memoserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += Magick::instance().memoserv.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Magick::instance().memoserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i = 1; i <= Magick::instance().commserv.GetNames().WordCount(" "); i++)
    {
	if (!Magick::instance().nickserv.IsLive(Magick::instance().commserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += Magick::instance().commserv.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Magick::instance().commserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i = 1; i <= Magick::instance().servmsg.GetNames().WordCount(" "); i++)
    {
	if (!Magick::instance().nickserv.IsLive(Magick::instance().servmsg.GetNames().ExtractWord(i, " ")))
	{
	    doison += Magick::instance().servmsg.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Magick::instance().servmsg.GetNames().ExtractWord(i, " "));
	}
    }

    if (!doison.empty())
	sraw(((proto.Tokens() &&
	       !proto.GetNonToken("ISON").empty()) ? proto.GetNonToken("ISON") : mstring("ISON")) + " :" + doison);
    ETCB();
}

void Server::SignOffAll(const mstring & reason)
{
    BTCB();
    FT("Server::SignOffAll", (reason));

    unsigned int i;

    for (i = 1; i <= Magick::instance().operserv.GetNames().WordCount(" "); i++)
    {
	if (Magick::instance().nickserv.IsLive(Magick::instance().operserv.GetNames().ExtractWord(i, " ")))
	    Magick::instance().operserv.signoff(Magick::instance().operserv.GetNames().ExtractWord(i, " "), reason);
    }
    for (i = 1; i <= Magick::instance().nickserv.GetNames().WordCount(" "); i++)
    {
	if (Magick::instance().nickserv.IsLive(Magick::instance().nickserv.GetNames().ExtractWord(i, " ")))
	    Magick::instance().nickserv.signoff(Magick::instance().nickserv.GetNames().ExtractWord(i, " "), reason);
    }
    for (i = 1; i <= Magick::instance().chanserv.GetNames().WordCount(" "); i++)
    {
	if (Magick::instance().nickserv.IsLive(Magick::instance().chanserv.GetNames().ExtractWord(i, " ")))
	    Magick::instance().chanserv.signoff(Magick::instance().chanserv.GetNames().ExtractWord(i, " "), reason);
    }
    for (i = 1; i <= Magick::instance().memoserv.GetNames().WordCount(" "); i++)
    {
	if (Magick::instance().nickserv.IsLive(Magick::instance().memoserv.GetNames().ExtractWord(i, " ")))
	    Magick::instance().memoserv.signoff(Magick::instance().memoserv.GetNames().ExtractWord(i, " "), reason);
    }
    for (i = 1; i <= Magick::instance().commserv.GetNames().WordCount(" "); i++)
    {
	if (Magick::instance().nickserv.IsLive(Magick::instance().commserv.GetNames().ExtractWord(i, " ")))
	    Magick::instance().commserv.signoff(Magick::instance().commserv.GetNames().ExtractWord(i, " "), reason);
    }
    for (i = 1; i <= Magick::instance().servmsg.GetNames().WordCount(" "); i++)
    {
	if (Magick::instance().nickserv.IsLive(Magick::instance().servmsg.GetNames().ExtractWord(i, " ")))
	    Magick::instance().servmsg.signoff(Magick::instance().servmsg.GetNames().ExtractWord(i, " "), reason);
    }
    ETCB();
}

Server::Server()
{
    BTCB();
    NFT("Server::Server");
    WLOCK((lck_Server, "i_UserMax"));
    i_UserMax = 0;
    DumpE();
    ETCB();
}

size_t Server::UserMax() const
{
    BTCB();
    NFT("Server::UserMax");
    RLOCK((lck_Server, "i_UserMax"));
    RET(i_UserMax);
    ETCB();
}

void Server::OurUplink(const mstring & server)
{
    BTCB();
    FT("Server::OurUplink", (server));
    WLOCK((lck_Server, "i_OurUplink"));
    MCB(i_OurUplink);
    i_OurUplink = server;
    MCE(i_OurUplink);
    ETCB();
}

mstring Server::OurUplink() const
{
    BTCB();
    NFT("Server::OurUplink");
    RLOCK((lck_Server, "i_OurUplink"));
    RET(i_OurUplink);
    ETCB();
}

void Server::FlushMsgs(const mstring & nick)
{
    BTCB();
    FT("Server::FlushMsgs", (nick));

    map < mstring, list < triplet < send_type, mDateTime, triplet < mstring, mstring, mstring > > > >::iterator i;
    list < triplet < send_type, mDateTime, triplet < mstring, mstring, mstring > > >::iterator j;

    // Dont report this, thats the point of the queue...
    if (!Magick::instance().nickserv.IsLive(nick))
	return;

    RLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
    MCB(ToBeSent.size());
    if ((i = ToBeSent.find(nick.LowerCase())) != ToBeSent.end())
    {
	WLOCK((lck_Server, "ToBeSent", i->first));
	for (j = i->second.begin(); j != i->second.end(); j++)
	{
	    if (j->second.SecondsSince() > Magick::instance().config.Squit_Protect())
		continue;

	    switch (j->first)
	    {
	    case t_GLOBOPS:
		GLOBOPS(nick, j->third.first);
		break;
	    case t_HELPOPS:
		HELPOPS(nick, j->third.first);
		break;
	    case t_CHATOPS:
		CHATOPS(nick, j->third.first);
		break;
	    case t_INVITE:
		INVITE(nick, j->third.first, j->third.second);
		break;
	    case t_KICK:
		KICK(nick, j->third.first, j->third.second, j->third.third);
		break;
	    case t_KILL:
		KILL(nick, j->third.first, j->third.second);
		break;
	    case t_NOTICE:
		NOTICE(nick, j->third.first, j->third.second);
		break;
	    case t_PRIVMSG:
		PRIVMSG(nick, j->third.first, j->third.second);
		break;
	    case t_SQLINE:
		SQLINE(nick, j->third.first, j->third.second);
		break;
	    case t_SVSMODE:
		SVSMODE(nick, j->third.first, j->third.second);
		break;
	    case t_SVSNICK:
		SVSNICK(nick, j->third.first, j->third.second);
		break;
	    case t_SVSKILL:
		SVSKILL(nick, j->third.first, j->third.second);
		break;
	    case t_SVSHOST:
		SVSHOST(nick, j->third.first, j->third.second);
		break;
	    case t_TOPIC:
		TOPIC(nick, j->third.first, j->third.second, j->third.third);
		break;
	    case t_UNSQLINE:
		UNSQLINE(nick, j->third.first);
		break;
	    case t_WALLOPS:
		WALLOPS(nick, j->third.first);
		break;
	    default:
		LOG(LM_WARNING, "ERROR/REQ_UNKNOWN",
		    (static_cast < int > (j->first), i->first, j->third.first, j->third.second, j->third.third,
		     ToHumanTime(j->second.SecondsSince())));
		break;
	    }
	}
	ToBeSent.erase(i->first);
    }
    MCE(ToBeSent.size());
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void Server::AddList(Server_t * in) throw (E_Server_List)
#else
void Server::AddList(Server_t * in)
#endif
{
    BTCB();
    FT("Server::AddList", ("(Server_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Server_List(E_Server_List::W_Add, E_Server_List::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Server", "List", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Server_List(E_Server_List::W_Add, E_Server_List::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Server", "List", "Add", "Blank"));
	return;
#endif
    }

    if (in->doDelete())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Server_List(E_Server_List::W_Add, E_Server_List::T_NotFound));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Server", "List", "Add", "NotFound"));
	return;
#endif
    }

    WLOCK((lck_Server, lck_list));
    map_entry < Server_t > old_entry(i_list, in->Name().LowerCase());
    if (old_entry != NULL)
    {
	old_entry->setDelete();
	i_list.erase(in->Name().LowerCase());
    }
    i_list[in->Name().LowerCase()] = in;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
map_entry < Server_t > Server::GetList(const mstring & in) const throw (E_Server_List)
#else
map_entry < Server_t > Server::GetList(const mstring & in) const
#endif
{
    BTCB();
    FT("Server::GetList", (in));

    RLOCK((lck_Server, lck_list, in.LowerCase()));
    Server::list_t::const_iterator iter = i_list.find(in.LowerCase());
    if (iter == i_list.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Server_List(E_Server_List::W_Get, E_Server_List::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Server", "List", "Get", "NotFound", in));
	NRET(Server_t &, GLOB_Server_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Server_List(E_Server_List::W_Get, E_Server_List::T_Invalid, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Server", "List", "Get", "Invalid", in));
	NRET(Server_t &, GLOB_Server_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Server_List(E_Server_List::W_Get, E_Server_List::T_Blank, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Server", "List", "Get", "Blank", in));
	NRET(Server_t &, GLOB_Server_t);
#endif
    }

    NRET(map_entry < Server_t >, map_entry < Server_t > (iter->second));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void Server::RemList(const mstring & in, bool downlinks) throw (E_Server_List)
#else
void Server::RemList(const mstring & in, bool downlinks)
#endif
{
    BTCB();
    FT("Server::RemList", (in, downlinks));

    WLOCK((lck_Server, lck_list));
    Server::list_t::iterator iter = i_list.find(in.LowerCase());
    if (iter == i_list.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Server_List(E_Server_List::W_Rem, E_Server_List::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Server", "List", "Rem", "NotFound", in));
	return;
#endif
    }

    vector < mstring > Kill;
    if (iter->second != NULL)
    {
	map_entry < Server_t > me(iter->second);

	if (downlinks)
	{
	    // Take my downlinks and all theirs with me
	    Kill = iter->second->AllDownlinks();
	    CP(("Destroying %d more servers", Kill.size()));
	}
	// We are the uplink, so notify system we have no uplink!
	if (Magick::instance().server.OurUplink() == me->Name())
	    OurUplink("");

	me->setDelete();
    }

    i_list.erase(iter);

    for (unsigned int i = 0; i < Kill.size(); i++)
    {
	iter = i_list.find(Kill[i].LowerCase());
	if (iter == i_list.end())
	    continue;

	if (iter->second != NULL)
	{
	    map_entry < Server_t > ent(iter->second);
	    ent->setDelete();
	}
	i_list.erase(iter);
    }
    ETCB();
}

bool Server::IsList(const mstring & server) const
{
    BTCB();
    FT("Server::IsList", (server));
    RLOCK((lck_Server, lck_list));
    bool retval = (i_list.find(server.LowerCase()) != i_list.end());

    RET(retval);
    ETCB();
}

mstring Server::GetServer(const mstring & server) const
{
    BTCB();
    FT("Server::GetServer", (server));
    mstring retval;

    if (proto.Numeric.Server() && server[0u] == '@' && !server.Contains("."))
	retval = proto.Numeric.FindServerNumeric(proto.Numeric.ServerNumeric(server.After("@")));
    else if (IsList(server.After("@")) || server.IsSameAs(Magick::instance().startup.Server_Name(), true))
	retval = server;
    RET(retval);
    ETCB();
}

mstring Server::GetUser(const mstring & user) const
{
    BTCB();
    FT("Server::GetUser", (user));
    mstring retval;

    if (proto.Numeric.User() && user[0u] == '!')
	retval = proto.Numeric.FindUserNumeric(proto.Numeric.UserNumeric(user.After("!")));
    else if (Magick::instance().nickserv.IsLive(user))
	retval = user;
    RET(retval);
    ETCB();
}

mstring Server::GetChannel(const mstring & channel) const
{
    BTCB();
    FT("Server::GetChannel", (channel));
    mstring retval;

    if (proto.Numeric.Channel() && !IsChan(channel))
	retval = proto.Numeric.FindUserNumeric(proto.Numeric.ChannelNumeric(channel));
    else if (Magick::instance().chanserv.IsLive(channel))
	retval = channel;
    RET(retval);
    ETCB();
}

void Server::Jupe(const mstring & server, const mstring & reason)
{
    BTCB();
    FT("Server::Jupe", (server, reason));
    if (IsList(server))
	raw(((proto.Tokens() &&
	      !proto.GetNonToken("SQUIT").empty()) ? proto.GetNonToken("SQUIT") : mstring("SQUIT")) + " " +
	    server.LowerCase() + " :JUPE command used.");
    // SERVER downlink hops :description
    // :uplink SERVER downlink hops :description
    mstring tmp;

    // Find an unused numeric ...
    unsigned long numeric = 0;

    if (proto.Numeric.Server())
	numeric = proto.Numeric.FindServerNumeric();

    Magick::instance().server.raw(parseMessage(Magick::instance().server.proto.Server(), mVarArray(
				  server.LowerCase(), 2, "JUPED (" + reason + ")", proto.Numeric.ServerLineNumeric(numeric),
				  time(NULL), Magick::instance().StartTime().timetstring())));

    map_entry < Server_t > jupe(new Server_t(server.LowerCase(), "JUPED (" + reason + ")", numeric));
    Magick::instance().server.AddList(jupe);
    ETCB();
}

void Server::AKILL(const mstring & host, const mstring & reason, const unsigned long exptime, const mstring & killer)
{
    BTCB();
    FT("Server::AKILL", (host, reason, exptime, killer));

    if (!host.Contains("@"))
	return;

    mstring line;

    switch (proto.Akill())
    {
    case 0000:
	break;
    case 1000:
	if (proto.Tokens() && !proto.GetNonToken("AKILL").empty())
	    line << proto.GetNonToken("AKILL");
	else
	    line << "AKILL";
	line << " " << host.After("@") << " " << host.Before("@") << " :" << reason;
	break;
    case 1001:
	if (proto.Tokens() && !proto.GetNonToken("AKILL").empty())
	    line << proto.GetNonToken("AKILL");
	else
	    line << "AKILL";
	line << " " << host.After("@") << " " << host.Before("@") << " " << exptime << " " <<
		((!killer.empty()) ? killer : Magick::instance().operserv.FirstName()) << " " <<
		static_cast < time_t > (mDateTime::CurrentDateTime()) << " :" << reason;
	break;
    case 1002:
	if (proto.Tokens() && !proto.GetNonToken("TKL").empty())
	    line << proto.GetNonToken("TKL");
	else
	    line << "TKL";
	line << " + G " << host.Before("@") << " " << host.After("@") << " " <<
		((!killer.empty()) ? killer : Magick::instance().operserv.FirstName()) << " " <<
		static_cast < time_t > (mDateTime::CurrentDateTime()) + exptime << " " <<
		static_cast < time_t > (mDateTime::CurrentDateTime()) << " :" << reason;
	break;
    case 2000:
	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * +" << exptime << " " << host << " :" << reason;
	break;
    case 2001:
	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * +" << host << " " << exptime << " :" << reason;
	break;
    case 2002:
	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " +" << host << " " << exptime << " :" << reason;
	break;
    case 3000:
	// Hybrid 5/6 is EXTREMELY ugly with its gline -- it seems
	// to require 3 different opers, on 3 different hosts, from
	// 3 different servers to set the gline.  Luckily it doesnt
	// actually check that, so we just fudge it.

	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";

	sraw(line + "VOnick1 VOuser1 VOhost1 VOserver1 " + host.Before("@") + " " + host.After("@") + " :" + reason);

	sraw(line + "VOnick2 VOuser2 VOhost2 VOserver2 " + host.Before("@") + " " + host.After("@") + " :" + reason);

	if (Magick::instance().nickserv.IsLive(Magick::instance().operserv.FirstName()))
	{
	    map_entry < Nick_Live_t > os = Magick::instance().nickserv.GetLive(Magick::instance().operserv.FirstName());
	    line << " " << os->Name() << " " << os->User() << " " << os->Host() << " " <<
		    Magick::instance().startup.Server_Name() << " " << host.Before("@") << " " << host.After("@") << " :" <<
		    reason;
	}
	break;
    case 3001:
	if (proto.Tokens() && !proto.GetNonToken("KLINE").empty())
	    line << proto.GetNonToken("KLINE");
	else
	    line << "KLINE";
	line << " " << killer << " * " << exptime << " " << host.Before("@") << " " << host.After("@") << " :" << reason;
	break;
    }

    // GLINING clients do this for us ...
    NickServ::live_t::iterator iter;
    vector < pair < mstring, unsigned long > > killusers;

    {
	RLOCK((lck_NickServ, lck_live));
	for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
	{
	    map_entry < Nick_Live_t > nlive(iter->second);
	    if (!nlive->IsServices() && nlive->Mask(Nick_Live_t::U_P_H).After("!").Matches(host, true))
		killusers.push_back(pair < mstring, unsigned long > (nlive->Name(), nlive->Numeric()));
	}
    }

    if (!line.empty())
    {
	line.replace("\n", "");
	line.replace("\r", "");
	sraw(line);
    }

    unsigned int j;

    for (j = 0; j < killusers.size(); j++)
    {
	if (proto.KillAfterAkill())
	    Magick::instance().server.KILL(Magick::instance().operserv.FirstName(), killusers[j].first, reason);
	else
	{
	    mMessage::CheckDependancies(mMessage::NickNoExists, killusers[j].first);
	    if (killusers[j].second)
		mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(killusers[j].second));
	}
    }
    ETCB();
}

void Server::ANONKILL(const mstring & nick, const mstring & dest, const mstring & reason)
{
    BTCB();
    FT("Server::ANONKILL", (nick, dest, reason));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_KILL, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (dest, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("KILL", nick));
    }
    else if (!Magick::instance().nickserv.IsLive(dest))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONUSER", ("KILL", nick, dest));
    }
    else
    {
	unsigned long numeric = 0;

	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(dest);
	    numeric = nlive->Numeric();
	    nlive->Quit("Killed (" + reason + ")");
	}
	Magick::instance().nickserv.RemLive(dest);
	nraw(nick,
	     ((proto.Tokens() &&
	       !proto.GetNonToken("KILL").empty()) ? proto.GetNonToken("KILL") : mstring("KILL")) + " " + dest + " :" +
	     reason);
	mMessage::CheckDependancies(mMessage::NickNoExists, dest);
	if (numeric)
	    mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(numeric));
    }
    ETCB();
}

void Server::AWAY(const mstring & nick, const mstring & reason)
{
    BTCB();
    FT("Server::AWAY", (nick, reason));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONUSER", ("AWAY", nick));
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("AWAY", nick));
    }
    else
    {
	mstring line;

	Magick::instance().nickserv.GetLive(nick)->Away(reason);
	if (proto.Tokens() && !proto.GetNonToken("AWAY").empty())
	    line << proto.GetNonToken("AWAY");
	else
	    line << "AWAY";
	if (!reason.empty())
	    line = " : " + reason;
	nraw(nick, line);
    }
    ETCB();
}

void Server::GLOBOPS(const mstring & nick, const mstring & message)
{
    BTCB();
    FT("Server::GLOBOPS", (nick, message));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_GLOBOPS, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (message, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("GLOBOPS", nick));
    }
    else
    {
	Magick::instance().nickserv.GetLive(nick)->Action();
	mstring line;

	if (proto.Globops())
	    line +=
		((proto.Tokens() &&
		  !proto.GetNonToken("GLOBOPS").empty()) ? proto.GetNonToken("GLOBOPS") : mstring("GLOBOPS")) + " :";
	else
	    line +=
		((proto.Tokens() &&
		  !proto.GetNonToken("WALLOPS").empty()) ? proto.GetNonToken("WALLOPS") : mstring("WALLOPS")) + " :";

	for (unsigned int i = 1; i <= message.WordCount("\n\r"); i++)
	    nraw(nick, line + message.ExtractWord(i, "\n\r"));
    }
    ETCB();
}

void Server::HELPOPS(const mstring & nick, const mstring & message)
{
    BTCB();
    FT("Server::HELPOPS", (nick, message));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_HELPOPS, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (message, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("HELPOPS", nick));
    }
    else
    {
	Magick::instance().nickserv.GetLive(nick)->Action();

	mstring line;

	if (proto.Helpops())
	    line +=
		((proto.Tokens() &&
		  !proto.GetNonToken("HELPOP").empty()) ? proto.GetNonToken("HELPOP") : mstring("HELPOP")) + " :";
	else if (proto.Globops())
	    line +=
		((proto.Tokens() &&
		  !proto.GetNonToken("GLOBOPS").empty()) ? proto.GetNonToken("GLOBOPS") : mstring("GLOBOPS")) + " :";
	else
	    line +=
		((proto.Tokens() &&
		  !proto.GetNonToken("WALLOPS").empty()) ? proto.GetNonToken("WALLOPS") : mstring("WALLOPS")) + " :";

	for (unsigned int i = 1; i <= message.WordCount("\n\r"); i++)
	    nraw(nick, line + message.ExtractWord(i, "\n\r"));
    }
    ETCB();
}

void Server::CHATOPS(const mstring & nick, const mstring & message)
{
    BTCB();
    FT("Server::CHATOPS", (nick, message));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_CHATOPS, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (message, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("CHATOPS", nick));
    }
    else
    {
	Magick::instance().nickserv.GetLive(nick)->Action();

	mstring line;

	if (proto.Chatops())
	    line +=
		((proto.Tokens() &&
		  !proto.GetNonToken("CHATOPS").empty()) ? proto.GetNonToken("CHATOPS") : mstring("CHATOPS")) + " :";
	else if (proto.Globops())
	    line +=
		((proto.Tokens() &&
		  !proto.GetNonToken("GLOBOPS").empty()) ? proto.GetNonToken("GLOBOPS") : mstring("GLOBOPS")) + " :";
	else
	    line +=
		((proto.Tokens() &&
		  !proto.GetNonToken("WALLOPS").empty()) ? proto.GetNonToken("WALLOPS") : mstring("WALLOPS")) + " :";

	for (unsigned int i = 1; i <= message.WordCount("\n\r"); i++)
	    nraw(nick, line + message.ExtractWord(i, "\n\r"));
    }
    ETCB();
}

void Server::INVITE(const mstring & nick, const mstring & dest, const mstring & channel)
{
    BTCB();
    FT("Server::INVITE", (nick, dest, channel));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_INVITE, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (dest, channel, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("INVITE", nick));
    }
    else if (!Magick::instance().nickserv.IsLive(dest))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONUSER", ("INVITE", nick, dest));
    }
    else if (!Magick::instance().chanserv.IsLive(channel))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONCHAN", ("INVITE", nick, channel));
    }
    else
    {
	nraw(nick,
	     ((proto.Tokens() &&
	       !proto.GetNonToken("INVITE").empty()) ? proto.GetNonToken("INVITE") : mstring("INVITE")) + " " + dest + " :" +
	     channel);
    }
    ETCB();
}

void Server::JOIN(const mstring & nick, const mstring & channel)
{
    BTCB();
    FT("Server::JOIN", (nick, channel));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONUSER", ("JOIN", nick));
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("JOIN", nick));
    }
    else
    {
	// Make sure we split the channels here, so that we can
	// do the Join's AFTER the command it sent to the network,
	// else we'll end up senind modes before joins.
	vector < mstring > channels;
	vector < mstring >::iterator ci;
	for (unsigned int i = 1; i <= channel.WordCount(", "); i++)
	    channels.push_back(channel.ExtractWord(i, ", "));

	mstring out;

	// If we have SJOIN ability, then we need to join each
	// channel individually, else we do a standard JOIN.
	if (proto.SJoin())
	{
	    out << ((proto.Tokens() && !proto.GetNonToken("SJOIN").empty()) ? proto.GetNonToken("SJOIN") :
									      mstring("SJOIN")) << " " <<
		    mDateTime::CurrentDateTime().timetstring() << " ";

	    for (ci = channels.begin(); ci != channels.end(); ci++)
		nraw(nick, out + *ci);
	}
	else
	{
	    out << ((proto.Tokens() && !proto.GetNonToken("JOIN").empty()) ? proto.GetNonToken("JOIN") :
									     mstring("JOIN")) << " :";

	    bool firstchan = true;

	    for (ci = channels.begin(); ci != channels.end(); ci++)
	    {
		if (out.length() + ci->length() + 1 > proto.MaxLine())
		{
		    nraw(nick, out);
		    out.erase();
		    out << ((proto.Tokens() && !proto.GetNonToken("JOIN").empty()) ? proto.GetNonToken("JOIN") :
										     mstring("JOIN")) << " :";
		    firstchan = true;
		}
		if (firstchan)
		    firstchan = false;
		else
		    out << ',';
		out << * ci;
	    }
	    nraw(nick, out);
	}

	map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(nick);
	for_each(channels.begin(), channels.end(), CallMemberFunction<Nick_Live_t, void, mstring>(nlive, &Nick_Live_t::Join));
    }
    ETCB();
}

void Server::KICK(const mstring & nick, const mstring & dest, const mstring & channel, const mstring & reason)
{
    BTCB();
    FT("Server::KICK", (nick, dest, channel, reason));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_KICK, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (dest, channel, reason)));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("KICK", nick));
    }
    else if (!Magick::instance().nickserv.IsLive(dest))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONUSER", ("KICK", nick, dest));
    }
    else if (!Magick::instance().chanserv.IsLive(channel))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONCHAN", ("KICK", nick, channel));
    }
    else if (!Magick::instance().chanserv.GetLive(channel)->IsIn(dest))
    {
	LOG(LM_WARNING, "ERROR/REQ_NOTINCHAN", ("KICK", nick, dest, channel));
    }
    else
    {
	Magick::instance().nickserv.GetLive(dest)->Kick(nick, channel);
	mstring out =
	    ((proto.Tokens() &&
	      !proto.GetNonToken("KICK").empty()) ? proto.GetNonToken("KICK") : mstring("KICK")) + " " + channel + " ";
	unsigned long numeric = Magick::instance().nickserv.GetLive(dest)->Numeric();

	if (numeric && proto.Numeric.User())
	    out << proto.Numeric.UserNumeric(numeric);
	else
	    out << dest;
	out << " :" + reason;

	nraw(nick, out);
    }
    ETCB();
}

void Server::KILL(const mstring & nick, const mstring & dest, const mstring & reason)
{
    BTCB();
    FT("Server::KILL", (nick, dest, reason));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_KILL, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (dest, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("KILL", nick));
    }
    else if (!Magick::instance().nickserv.IsLive(dest))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONUSER", ("KILL", nick, dest));
    }
    else
    {
	unsigned long numeric = 0;

	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(dest);
	    numeric = nlive->Numeric();
	    nlive->Quit("Killed (" + nick + " (" + reason + "))");
	}
	Magick::instance().nickserv.RemLive(dest);
	nraw(nick,
	     ((proto.Tokens() &&
	       !proto.GetNonToken("KILL").empty()) ? proto.GetNonToken("KILL") : mstring("KILL")) + " " + dest + " :" +
	     Magick::instance().nickserv.GetLive(nick)->Host() + "!" + nick + " (" + reason + ")");
	mMessage::CheckDependancies(mMessage::NickNoExists, dest);
	if (numeric)
	    mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(numeric));
    }
    ETCB();
}

void Server::MODE(const mstring & nick, const mstring & mode)
{
    BTCB();
    FT("Server::MODE", (nick, mode));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONUSER", ("MODE", nick));
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("MODE", nick));
    }
    else
    {
	Magick::instance().nickserv.GetLive(nick)->Mode(mode);
	nraw(nick,
	     ((proto.Tokens() &&
	       !proto.GetNonToken("MODE").empty()) ? proto.GetNonToken("MODE") : mstring("MODE")) + " " + nick + " :" + mode);
    }
    ETCB();
}

void Server::MODE(const mstring & nick, const mstring & channel, const mstring & mode)
{
    BTCB();
    FT("Server::MODE", (nick, channel, mode));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONUSER", ("MODE", nick));
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("MODE", nick));
    }
    else if (!Magick::instance().chanserv.IsLive(channel))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONCHAN", ("MODE", nick, channel));
    }
    else
    {
	Magick::instance().chanserv.GetLive(channel)->Mode(nick, mode);
	if (proto.ServerModes())
	    sraw(((proto.Tokens() &&
		   !proto.GetNonToken("MODE").empty()) ? proto.GetNonToken("MODE") : mstring("MODE")) + " " + channel + " " +
		 mode.Before(" ") + " " + mode.After(" "));
	else
	    nraw(nick,
		 ((proto.Tokens() &&
		   !proto.GetNonToken("MODE").empty()) ? proto.GetNonToken("MODE") : mstring("MODE")) + " " + channel + " " +
		 mode.Before(" ") + " " + mode.After(" "));
    }
    ETCB();
}

void Server::NICK(const mstring & nick, const mstring & user, const mstring & host, const mstring & i_server,
		  const mstring & name)
{
    BTCB();
    FT("Server::NICK", (nick, user, host, i_server, name));

    if (Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_TOUSER", ("NICK", i_server, nick));
    }
    else
    {
	bool sendmode = true;
	mstring outmodes = Magick::instance().startup.Setmode() + proto.ServicesModes();
	mstring server = GetServer(i_server);

	if (proto.Numeric.Server() && !server.empty())
	{
	    if (IsList(server))
	    {
		map_entry < Server_t > serv = GetList(server);
		if (serv->Numeric())
		    server = proto.Numeric.ServerNumeric(serv->Numeric());
	    }
	    else
	    {
		unsigned long ournumeric = Magick::instance().startup.Server(Magick::instance().CurrentServer()).second.third;

		server = proto.Numeric.ServerNumeric(ournumeric);
	    }
	}
	mstring out, token;

	if (Magick::instance().nickserv.IsLiveAll(nick.LowerCase()))
	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(nick);
	    nlive->Quit("SQUIT - " + nlive->Server());
	    Magick::instance().nickserv.RemLive(nick);
	    mMessage::CheckDependancies(mMessage::NickNoExists, nick);
	    if (nlive->Numeric())
		mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
	}
	map_entry < Nick_Live_t > tmp(new Nick_Live_t(nick, user, host, name));
	if (proto.Numeric.User())
	    tmp->Numeric(proto.Numeric.FindUserNumeric());
	Magick::instance().nickserv.AddLive(tmp);
	{
	    WLOCK2((lck_Server, "i_UserMax"));
	    if (i_UserMax < Magick::instance().nickserv.LiveSize())
	    {
		MCB(i_UserMax);
		i_UserMax = Magick::instance().nickserv.LiveSize();
		MCE(i_UserMax);
	    }
	}

	switch (proto.Signon())
	{
	case 0000:
	    token = "USER";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " " << user << " " << host << " " << server << " :" << name;
	    break;
	case 0001:
	    token = "USER";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << mDateTime::CurrentDateTime().timetstring() << " " << user << " " << host << " " << server <<
		   " :" << name;
	    break;
	case 1000:
	    token = "NICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " " << user << " " << host << " " <<
		   server;
	    out << " :" << name;
	    break;
	case 1001:
	    token = "NICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " " << user << " " << host << " " <<
		   server << " 1";
	    out << " :" << name;
	    break;
	case 1002:
	    token = "NICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " " << user << " " << host << " " <<
		   server << " 1 " << host;
	    out << " :" << name;
	    break;
	case 1003:
	    token = "NICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " " << user << " " << host << " " <<
		   host << " " << server << " 1";
	    out << " :" << name;
	    break;
	case 2000:
	    token = "NICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " +" << outmodes << " " << user << " " <<
		   host << " " << server << " :" << name;
	    tmp->Mode(outmodes);
	    sendmode = false;
	    break;
	case 2001:
	    token = "NICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " +" << outmodes << " " << user << " " << 
		   host << " " << server << " 1 :" << name;
	    tmp->Mode(outmodes);
	    sendmode = false;
	    break;
	case 2002:
	    token = "NICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " +" << outmodes << " " << user << " " <<
		   host << " " << host << " " << server << " 1 :" << name;
	    tmp->Mode(outmodes);
	    sendmode = false;
	    break;
	case 2003:
	    token = "NICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " " << user << " " << host << " " <<
		   server << " 1 +" << outmodes << " " << host << " :" << name;
	    tmp->Mode(outmodes);
	    sendmode = false;
	    break;
	case 3000:
	    token = "NICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " " << user << " " << host << " ";
	    if (!outmodes.empty())
	    {
		out << "+" << outmodes << " ";
		tmp->Mode(outmodes);
	    }
	    sendmode = false;

	    unsigned long ournumeric = Magick::instance().startup.Server(Magick::instance().CurrentServer()).second.third;

	    // B]AAAB == 127.0.0.1 for ipaddress.
	    out << "B]AAAB " << proto.Numeric.ServerNumeric(ournumeric) << " " <<
		   proto.Numeric.UserNumeric(tmp->Numeric()) << " :" << name;
	    break;
	}
	// Sign ourselves in ...

	sraw(out);
	mMessage::CheckDependancies(mMessage::NickExists, nick);
	if (tmp->Numeric())
	    mMessage::CheckDependancies(mMessage::NickExists, "!" + proto.Numeric.UserNumeric(tmp->Numeric()));

	if (sendmode && !outmodes.empty())
	    MODE(nick, "+" + Magick::instance().startup.Setmode() + proto.ServicesModes());
    }
    ETCB();
}

void Server::NICK(const mstring & oldnick, const mstring & newnick)
{
    BTCB();
    FT("Server::NICK", (oldnick, newnick));

    if (!Magick::instance().nickserv.IsLive(oldnick))
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONUSER", ("NICK", oldnick));
    }
    else if (!Magick::instance().nickserv.GetLive(oldnick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("NICK", oldnick));
    }
    else
    {
	map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(oldnick);
	Magick::instance().nickserv.RemLive(oldnick);
	nlive->setDelete(false);
	nlive->Name(newnick);
	if (Magick::instance().nickserv.IsLiveAll(newnick))
	{
	    map_entry < Nick_Live_t > tmp = Magick::instance().nickserv.GetLive(newnick);
	    tmp->Quit("SQUIT - " + tmp->Server());
	    Magick::instance().nickserv.RemLive(newnick);
	    mMessage::CheckDependancies(mMessage::NickNoExists, newnick);
	    if (nlive->Numeric())
		mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
	}
	Magick::instance().nickserv.AddLive(nlive);
	nraw(oldnick,
	     ((proto.Tokens() &&
	       !proto.GetNonToken("NICK").empty()) ? proto.GetNonToken("NICK") : mstring("NICK")) + " " + newnick +
	     (proto.TSora() ? " :" + mDateTime::CurrentDateTime().timetstring() : mstring("")));

	// NO numeric equivalents, since that doesnt change!
	mMessage::CheckDependancies(mMessage::NickNoExists, oldnick);
	mMessage::CheckDependancies(mMessage::NickExists, newnick);
    }
    ETCB();
}

void Server::NOTICE(const mstring & nick, const mstring & dest, const mstring & text)
{
    BTCB();
    FT("Server::NOTICE", (nick, dest, text));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_NOTICE, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (dest, text, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("NOTICE", nick));
    }
    else if (!
	     (dest[0u] == '$' || dest[0u] == '#' || Magick::instance().nickserv.IsLive(dest) ||
	      Magick::instance().chanserv.IsLive(dest)))
    {
	if (IsChan(dest))
	{
	    LOG(LM_WARNING, "ERROR/REQ_FORNONCHAN", ("NOTICE", nick, dest));
	}
	else
	{
	    LOG(LM_WARNING, "ERROR/REQ_FORNONUSER", ("NOTICE", nick, dest));
	}
    }
    else if (Magick::instance().nickserv.IsLive(dest) && Magick::instance().nickserv.GetLive(dest)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_TOSERVICE", ("NOTICE", nick));
    }
    else
    {
	Magick::instance().nickserv.GetLive(nick)->Action();

	mstring line =
	    ((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " ";

	if (proto.Numeric.Channel() && Magick::instance().chanserv.IsLive(dest))
	{

	    unsigned long numeric = Magick::instance().chanserv.GetLive(dest)->Numeric();

	    if (numeric)
		line << proto.Numeric.ChannelNumeric(numeric);
	    else
		line << dest;
	}
	else if (proto.Numeric.User() && Magick::instance().nickserv.IsLive(dest))
	{

	    unsigned long numeric = Magick::instance().nickserv.GetLive(dest)->Numeric();

	    if (numeric)
		line << proto.Numeric.UserNumeric(numeric);
	    else
		line << dest;
	}
	else
	    line << dest;

	line << " :";
	for (unsigned int i = 1; i <= text.WordCount("\n\r"); i++)
	    nraw(nick, line + text.ExtractWord(i, "\n\r"));
    }
    ETCB();
}

void Server::PART(const mstring & nick, const mstring & channel, const mstring & reason)
{
    BTCB();
    FT("Server::PART", (nick, channel, reason));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONUSER", ("PART", nick));
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("PART", nick));
    }
    else if (!Magick::instance().chanserv.IsLive(channel))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONCHAN", ("PART", nick, channel));
    }
    else if (!Magick::instance().chanserv.GetLive(channel)->IsIn(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_NOTINCHAN", ("KICK", nick, nick, channel));
    }
    else
    {
	Magick::instance().nickserv.GetLive(nick)->Part(channel);
	mstring tmpResult;

	if (!reason.empty())
	    tmpResult = " :" + reason;
	else
	    tmpResult = "";
	nraw(nick,
	     ((proto.Tokens() &&
	       !proto.GetNonToken("PART").empty()) ? proto.GetNonToken("PART") : mstring("PART")) + " " + channel + tmpResult);
    }
    ETCB();
}

void Server::PRIVMSG(const mstring & nick, const mstring & dest, const mstring & text)
{
    BTCB();
    FT("Server::PRIVMSG", (nick, dest, text));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_PRIVMSG, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (dest, text, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("PRIVMSG", nick));
    }
    else if (!
	     (dest[0u] == '$' || dest[0u] == '#' || Magick::instance().nickserv.IsLive(dest) ||
	      Magick::instance().chanserv.IsLive(dest)))
    {
	if (IsChan(dest))
	{
	    LOG(LM_WARNING, "ERROR/REQ_FORNONCHAN", ("PRIVMSG", nick, dest));
	}
	else
	{
	    LOG(LM_WARNING, "ERROR/REQ_FORNONUSER", ("PRIVMSG", nick, dest));
	}
    }
    else if (Magick::instance().nickserv.IsLive(dest) && Magick::instance().nickserv.GetLive(dest)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_TOSERVICE", ("PRIVMSG", nick));
    }
    else
    {
	Magick::instance().nickserv.GetLive(nick)->Action();

	mstring line =
	    ((proto.Tokens() &&
	      !proto.GetNonToken("PRIVMSG").empty()) ? proto.GetNonToken("PRIVMSG") : mstring("PRIVMSG")) + " ";

	if (proto.Numeric.Channel() && Magick::instance().chanserv.IsLive(dest))
	{

	    unsigned long numeric = Magick::instance().chanserv.GetLive(dest)->Numeric();

	    if (numeric)
		line << proto.Numeric.ChannelNumeric(numeric);
	    else
		line << dest;
	}
	else if (proto.Numeric.User() && Magick::instance().nickserv.IsLive(dest))
	{

	    unsigned long numeric = Magick::instance().nickserv.GetLive(dest)->Numeric();

	    if (numeric)
		line << proto.Numeric.UserNumeric(numeric);
	    else
		line << dest;
	}
	else
	    line << dest;

	line << " :";
	for (unsigned int i = 1; i <= text.WordCount("\n\r"); i++)
	    nraw(nick, line + text.ExtractWord(i, "\n\r"));
    }
    ETCB();
}

void Server::SQLINE(const mstring & nick, const mstring & target, const mstring & reason)
{
    BTCB();
    FT("Server::SQLINE", (nick, target, reason));

    if (proto.SQLINE().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_SQLINE, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (target, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", (proto.SQLINE(), nick));
    }
    else
    {
	mstring output;

	if (proto.Tokens() && !proto.GetNonToken(proto.SQLINE()).empty())
	    output << proto.GetNonToken(proto.SQLINE());
	else
	    output << proto.SQLINE();
	output << " " << target << " :" << reason;
	nraw(nick, output);
    }
    ETCB();
}

void Server::QUIT(const mstring & nick, const mstring & reason)
{
    BTCB();
    FT("Server::QUIT", (nick, reason));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONUSER", ("QUIT", nick));
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("QUIT", nick));
    }
    else
    {
	unsigned long numeric = 0;

	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(nick);
	    numeric = nlive->Numeric();
	    nlive->Quit(reason);
	}
	nraw(nick,
	     ((proto.Tokens() &&
	       !proto.GetNonToken("QUIT").empty()) ? proto.GetNonToken("QUIT") : mstring("QUIT")) + " :" + reason);
	Magick::instance().nickserv.RemLive(nick);
	mMessage::CheckDependancies(mMessage::NickNoExists, nick);
	if (numeric)
	    mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(numeric));
    }
    ETCB();
}

void Server::RAKILL(const mstring & host)
{
    BTCB();
    FT("Server::RAKILL", (host));

    if (!host.Contains("@"))
	return;

    mstring line;

    switch (proto.Akill())
    {
    case 0000:
	break;
    case 1000:
	if (proto.Tokens() && !proto.GetNonToken("RAKILL").empty())
	    line << proto.GetNonToken("RAKILL");
	else
	    line << "RAKILL";
	line << " " << host.After("@") << " " << host.Before("@");
	break;
    case 1001:
	if (proto.Tokens() && !proto.GetNonToken("RAKILL").empty())
	    line << proto.GetNonToken("RAKILL");
	else
	    line << "RAKILL";
	line << " " << host.After("@") << " " << host.Before("@");
	break;
    case 1002:
	if (proto.Tokens() && !proto.GetNonToken("TKL").empty())
	    line << proto.GetNonToken("TKL");
	else
	    line << "TKL";
	line << " - G " << host.Before("@") << " " << host.After("@") << " " << Magick::instance().operserv.FirstName();
	break;
    case 2000:
	if (proto.Tokens() && !proto.GetNonToken("UNGLINE").empty())
	    line << proto.GetNonToken("UNGLINE");
	else
	    line << "UNGLINE";
	line << " * " << host;
	break;
    case 2001:
	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * -" << host;
	break;
    case 2002:
	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " -" << host;
	break;
    case 3000:
	// Complete no-op.
	break;
    case 3001:
	// Complete no-op.
	break;
    }
    if (!line.empty())
	sraw(line);
    ETCB();
}

void Server::SVSHOST(const mstring & mynick, const mstring & nick, const mstring & newhost)
{
    BTCB();
    FT("Server::SVSHOST", (mynick, nick, newhost));

    if (proto.SVSHOST().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(mynick))
    {
	WLOCK((lck_Server, "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					       mstring > > (t_SVSHOST, mDateTime::CurrentDateTime(), triplet < mstring,
							    mstring, mstring > (nick, newhost, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(mynick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", (proto.SVSHOST(), nick));
    }
    else if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONUSER", (proto.SVSHOST(), mynick, nick));
    }
    else
    {
	Magick::instance().nickserv.GetLive(nick)->AltHost(newhost);
	mstring output;

	if (proto.Tokens() && !proto.GetNonToken(proto.SVSHOST()).empty())
	    output << proto.GetNonToken(proto.SVSHOST());
	else
	    output << proto.SVSHOST();
	output << " " << nick << " " << newhost << " :" << mDateTime::CurrentDateTime().timetstring();
	nraw(mynick, output);
    }
    ETCB();
}

void Server::SVSKILL(const mstring & mynick, const mstring & nick, const mstring & reason)
{
    BTCB();
    FT("Server::SVSKILL", (mynick, nick, reason));

    if (proto.SVSKILL().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(mynick))
    {
	WLOCK((lck_Server, "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					       mstring > > (t_SVSKILL, mDateTime::CurrentDateTime(), triplet < mstring,
							    mstring, mstring > (nick, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(mynick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", (proto.SVSKILL(), nick));
    }
    else if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONUSER", (proto.SVSKILL(), mynick, nick));
    }
    else
    {
	unsigned long numeric = 0;

	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(nick);
	    numeric = nlive->Numeric();
	    nlive->Quit(reason);
	}
	Magick::instance().nickserv.RemLive(nick);
	mstring output;

	if (proto.Tokens() && !proto.GetNonToken(proto.SVSKILL()).empty())
	    output << proto.GetNonToken(proto.SVSKILL());
	else
	    output << proto.SVSKILL();
	output << " " << nick << " :" << reason;
	nraw(mynick, output);
	mMessage::CheckDependancies(mMessage::NickNoExists, nick);
	if (numeric)
	    mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(numeric));
    }
    ETCB();
}

void Server::SVSNICK(const mstring & mynick, const mstring & nick, const mstring & newnick)
{
    BTCB();
    FT("Server::SVSNICK", (mynick, nick, newnick));

    if (proto.SVSNICK().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(mynick))
    {
	WLOCK((lck_Server, "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					       mstring > > (t_SVSNICK, mDateTime::CurrentDateTime(), triplet < mstring,
							    mstring, mstring > (nick, newnick, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(mynick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", (proto.SVSNICK(), nick));
    }
    else if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONUSER", (proto.SVSNICK(), mynick, nick));
    }
    else if (Magick::instance().nickserv.IsLive(newnick))
    {
	LOG(LM_WARNING, "ERROR/REQ_TOUSER", (proto.SVSNICK(), mynick, newnick));
    }
    else
    {
	mstring output;

	if (proto.Tokens() && !proto.GetNonToken(proto.SVSNICK()).empty())
	    output << proto.GetNonToken(proto.SVSNICK());
	else
	    output << proto.SVSNICK();
	output << " " << nick << " " << newnick << " :" << time_t(NULL);
	nraw(mynick, output);
    }
    ETCB();
}

void Server::SVSNOOP(const mstring & nick, const mstring & server, const bool onoff)
{
    BTCB();
    FT("Server::SVSNOOP", (nick, server, onoff));

    if (proto.SVSNOOP().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONUSER", (proto.SVSNOOP(), nick));
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", (proto.SVSNOOP(), nick));
    }
    else if (!IsList(server))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONSERVER", (proto.SVSNOOP(), nick, server));
    }
    else
    {
	mstring output;

	if (proto.Tokens() && !proto.GetNonToken(proto.SVSNOOP()).empty())
	    output << proto.GetNonToken(proto.SVSNOOP());
	else
	    output << proto.SVSNOOP();
	output << " " + server + " " + mstring(onoff ? "+" : "-");
	nraw(nick, output);
    }
    ETCB();
}

void Server::SVSMODE(const mstring & mynick, const mstring & nick, const mstring & mode)
{
    BTCB();
    FT("Server::SVSMODE", (mynick, nick, mode));

    if (proto.SVSMODE().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(mynick))
    {
	WLOCK((lck_Server, "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					       mstring > > (t_SVSMODE, mDateTime::CurrentDateTime(), triplet < mstring,
							    mstring, mstring > (nick, mode, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(mynick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", (proto.SVSMODE(), nick));
    }
    else if (!Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONUSER", (proto.SVSMODE(), mynick, nick));
    }
    else
    {
	Magick::instance().nickserv.GetLive(nick)->Mode(mode);
	mstring output;

	if (proto.Tokens() && !proto.GetNonToken(proto.SVSMODE()).empty())
	    output << proto.GetNonToken(proto.SVSMODE());
	else
	    output << proto.SVSMODE();
	output << " " << nick << " " << mode;
	nraw(mynick, output);
    }
    ETCB();
}

void Server::TOPIC(const mstring & nick, const mstring & setter, const mstring & channel, const mstring & topic,
		   const mDateTime & settime)
{
    BTCB();
    FT("Server::TOPIC", (nick, setter, channel, topic, settime));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_TOPIC, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (setter, channel, topic)));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("TOPIC", nick));
    }
    else if (!Magick::instance().chanserv.IsLive(channel))
    {
	LOG(LM_WARNING, "ERROR/REQ_FORNONCHAN", ("TOPIC", nick, channel));
    }
    else
    {
	mstring out;

	if (proto.Tokens() && !proto.GetNonToken("TOPIC").empty())
	    out << proto.GetNonToken("TOPIC");
	else
	    out << "TOPIC";

	out << " " << channel;
	if (proto.BigTopic())
	    out << " " << setter;
	if (!topic.empty())
	{
	    if (proto.BigTopic())
	    {
		out << " ";
		if (proto.TopicCurrent())
		    out << mDateTime::CurrentDateTime().timetstring();
		else
		    out << settime.timetstring();
	    }
	    out << " :" << topic;
	}

	bool dojoin = false;

	map_entry < Chan_Live_t > chan = Magick::instance().chanserv.GetLive(channel);
	chan->Topic(nick, topic, setter, settime);
	if (proto.TopicJoin() && !chan->IsIn(Magick::instance().chanserv.FirstName()))
	    dojoin = true;

	if (dojoin)
	    JOIN(Magick::instance().chanserv.FirstName(), channel);
	nraw(nick, out);
	if (dojoin)
	    PART(Magick::instance().chanserv.FirstName(), channel);
    }
    ETCB();
}

void Server::UNSQLINE(const mstring & nick, const mstring & target)
{
    BTCB();
    FT("Server::UNSQLINE", (nick, target));

    if (proto.UNSQLINE().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_UNSQLINE, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (target, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", (proto.UNSQLINE(), nick));
    }
    else
    {
	mstring output;

	if (proto.Tokens() && !proto.GetNonToken(proto.UNSQLINE()).empty())
	    output << proto.GetNonToken(proto.UNSQLINE());
	else
	    output << proto.UNSQLINE();
	output << " " << target;
	nraw(nick, output);
    }
    ETCB();
}

void Server::WALLOPS(const mstring & nick, const mstring & message)
{
    BTCB();
    FT("Server::WALLOPS", (nick, message));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK((lck_Server, "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(triplet < send_type, mDateTime, triplet < mstring, mstring,
					     mstring > > (t_WALLOPS, mDateTime::CurrentDateTime(), triplet < mstring, mstring,
							  mstring > (message, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Magick::instance().nickserv.GetLive(nick)->IsServices())
    {
	LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", ("WALLOPS", nick));
    }
    else
    {
	Magick::instance().nickserv.GetLive(nick)->Action();

	mstring line =
	    ((proto.Tokens() &&
	      !proto.GetNonToken("WALLOPS").empty()) ? proto.GetNonToken("WALLOPS") : mstring("WALLOPS")) + " :";

	for (unsigned int i = 1; i <= message.WordCount("\n\r"); i++)
	    nraw(nick, line + message.ExtractWord(i, "\n\r"));
    }
    ETCB();
}

void Server::KillUnknownUser(const mstring & user) const
{
    BTCB();
    FT("Server::KillUnknownUser", (user));
    sraw(((proto.Tokens() &&
	   !proto.GetNonToken("KILL").empty()) ? proto.GetNonToken("KILL") : mstring("KILL")) + " " + user + " :" +
	 Magick::instance().startup.Server_Name() + " (" + user + "(?) <- " + Magick::instance().CurrentServer() + ")");
    LOG(LM_ERROR, "OTHER/KILL_UNKNOWN", (user));
    ETCB();
}

void Server::execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    //okay this is the main networkserv command switcher
    FT("Server::execute", (source, msgtype, params));

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
	numeric_execute(source, msgtype, params);
	break;
    case 'A':
	parse_A(source, msgtype, params);
	break;
    case 'B':
	parse_B(source, msgtype, params);
	break;
    case 'C':
	parse_C(source, msgtype, params);
	break;
    case 'D':
	parse_D(source, msgtype, params);
	break;
    case 'E':
	parse_E(source, msgtype, params);
	break;
    case 'F':
	parse_F(source, msgtype, params);
	break;
    case 'G':
	parse_G(source, msgtype, params);
	break;
    case 'H':
	parse_H(source, msgtype, params);
	break;
    case 'I':
	parse_I(source, msgtype, params);
	break;
    case 'J':
	parse_J(source, msgtype, params);
	break;
    case 'K':
	parse_K(source, msgtype, params);
	break;
    case 'L':
	parse_L(source, msgtype, params);
	break;
    case 'M':
	parse_M(source, msgtype, params);
	break;
    case 'N':
	parse_N(source, msgtype, params);
	break;
    case 'O':
	parse_O(source, msgtype, params);
	break;
    case 'P':
	parse_P(source, msgtype, params);
	break;
    case 'Q':
	parse_Q(source, msgtype, params);
	break;
    case 'R':
	parse_R(source, msgtype, params);
	break;
    case 'S':
	parse_S(source, msgtype, params);
	break;
    case 'T':
	parse_T(source, msgtype, params);
	break;
    case 'U':
	parse_U(source, msgtype, params);
	break;
    case 'V':
	parse_V(source, msgtype, params);
	break;
    case 'W':
	parse_W(source, msgtype, params);
	break;
    case 'X':
	parse_X(source, msgtype, params);
	break;
    case 'Y':
	parse_Y(source, msgtype, params);
	break;
    case 'Z':
	parse_Z(source, msgtype, params);
	break;
    default:
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
	break;
    }
    ETCB();
}

void Server::parse_A(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_A", (source, msgtype, params));

    if (msgtype == "ADCHAT")
    {
	// Useless admin chatter.
    }
    else if (msgtype == "ADMIN")
    {
	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		raw(((proto.Tokens() &&
		      !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " " + source +
		    " :" + tmp.c_str());
		return;
	    }
	}
	// :source ADMIN
	sraw("256 " + source + " :Administrative info about " + Magick::instance().startup.Server_Name());
	sraw("257 " + source + " :" + Magick::instance().startup.Server_Desc());
	sraw("258 " + source + " :Admins - " + Magick::instance().operserv.Services_Admin());
	sraw("259 " + source + " :" + FULLNAME + " - " + EMAIL);
    }
    else if (msgtype == "AKILL")
    {
	// We will ignore AKILLS because they're not relivant to us.
	// we will not be akilling our own clients ;P
    }
    else if (msgtype == "AWAY")
    {
	if (source.Contains("."))
	    return;

	// :source AWAY
	// :source AWAY :This is my reason
	if (params.empty())
	{
	    Magick::instance().nickserv.GetLive(source)->Away("");

	    // HAS to be AFTER the nickname is added to map.
	    CommServ::list_t::iterator iter;
	    {
		RLOCK2((lck_CommServ, lck_list));
		for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		{
		    map_entry < Committee_t > comm(iter->second);
		    if (comm->IsOn(source))
		    {
			MLOCK((lck_CommServ, lck_list, comm->Name(), "message"));
			for (comm->message = comm->MSG_begin(); comm->message != comm->MSG_end(); comm->message++)
			{
			    Magick::instance().servmsg.send(source,
							    "[" + IRC_Bold + comm->Name() + IRC_Off + "] " +
							    comm->message->Entry());
			}
		    }
		}
	    }

	    if (Magick::instance().nickserv.IsStored(source))
	    {
		mstring who = Magick::instance().nickserv.GetStored(source)->Host().LowerCase();

		if (who.empty())
		    who = source;
		if (Magick::instance().memoserv.IsNick(who))
		{
		    size_t count = Magick::instance().memoserv.NickMemoCount(who);

		    if (count)
			SEND(Magick::instance().memoserv.FirstName(), source, "MS_STATUS/NS_UNREAD",
			     (count, Magick::instance().memoserv.FirstName()));
		}
	    }
	}
	else
	    Magick::instance().nickserv.GetLive(source)->Away(params.After(" :"));
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_B(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_B", (source, msgtype, params));

    static_cast < void > (params);

    if (msgtype == "BURST")
    {
	if (params.WordCount(" ") > 2)
	{
	    mstring chan(params.ExtractWord(1, " "));
	    unsigned int i, offset = 3;

	    vector < mstring > users;

	    mstring modes = params.ExtractWord(offset, " ");

	    if (modes[0u] != '+')
		modes.erase();

	    mstring mode_params, nick, opts;

	    if (!modes.empty())
	    {
		offset++;
		for (i = 0; i < modes.length(); i++)
		    if (proto.ChanModeArg().Contains(modes[i]))
			mode_params += " " + params.ExtractWord(offset++, " ");
	    }

	    vector < mstring > parsed;
	    mstring usermodes;

	    for (i = offset; i <= params.WordCount(", "); i++)
		parsed.push_back(params.ExtractWord(i, " "));

	    for (i = parsed.size() - 1; i < parsed.size(); i--)
	    {
		if (parsed[i].Contains(":"))
		    usermodes = parsed[i].After(":");
		else
		    parsed[i] += ":" + usermodes;
	    }

	    for (i = 0; i <= parsed.size(); i++)
	    {
		nick = GetUser("!" + parsed[i].Before(":"));
		opts = parsed[i].After(":");

		if (!nick.empty())
		{
		    if (!opts.empty())
		    {
			if (modes.empty())
			    modes = "+";

			unsigned int j;

			for (j = 0; j < opts.length(); j++)
			{
			    modes += opts[i];
			    mode_params += " " + nick;
			}
		    }
		    users.push_back(nick);
		}
	    }

	    for (i = 0; i < users.size(); i++)
	    {
		Magick::instance().nickserv.GetLive(users[i])->Join(chan);
	    }
	    CP(("MODE TO %s: %s", chan.LowerCase().c_str(), (modes + " " + mode_params).c_str()));
	    if (Magick::instance().chanserv.IsLive(chan))
	    {
		map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(chan);
		clive->Creation_Time(static_cast < time_t > (atoul(IrcParam(params, 2))));
		clive->Mode(source, modes + " " + mode_params);
	    }
	    else if (modes.length() > 1)
	    {
		LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", ("MODE", source, chan));
	    }
	}
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_C(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_C", (source, msgtype, params));

    if (msgtype == "CAPAB")
    {
	if (!source.Contains("."))
	    return;

	mstring tmp = (" " + params + " ");
	// Bahamut version of the PROTOCTL line
	if (tmp.Contains(" TOKEN "))
	    proto.Tokens(true);
//	if (tmp.Contains(" SSJOIN ") || tmp.Contains(" SJOIN ") || tmp.Contains(" SJOIN2 ") || tmp.Contains(" SJ3 "))
//	    proto.SJoin(true);
    }
    else if (msgtype == "CHATOPS")
    {
    }
    else if (msgtype == "CHGHOST")
    {
	mstring nick = GetUser(IrcParam(params, 1));

	if (!nick.empty())
	    Magick::instance().nickserv.GetLive(nick)->AltHost(IrcParam(params, 2));
    }
    else if (msgtype == "CHGIDENT")
    {
    }
    else if (msgtype == "CHGNAME")
    {
    }
    else if (msgtype == "CONNECT")
    {
	// :soul.darker.net 481 ChanServ :Permission Denied- You do not have the correct IRC operator privileges
	// :source CONNECT some.server port :our.server
	mstring server = GetServer(IrcParam(params, 1));

	if (!server.empty() && server.IsSameAs(Magick::instance().startup.Server_Name(), true))
	{
	    sraw(((proto.Tokens() &&
		   !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " " + source +
		 " :Connect: Server " + server + " already exists from " + Magick::instance().startup.Server_Name());
	}
	else
	{
	    sraw(((proto.Tokens() &&
		   !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " " + source +
		 " :Connect: Host " + IrcParam(params, 1) + " not listed in irc.conf");
	}
    }
    else if (msgtype == "CREATE")
    {
	if (source.Contains("."))
	    return;

	// :source JOIN :#channel
	mstring chan(IrcParam(params, 1));

	Magick::instance().nickserv.GetLive(source)->Join(chan);
	Magick::instance().chanserv.GetLive(chan)->Mode(source, "+o " + source);
    }
    else if (msgtype == "CYCLE")
    {
	if (source.Contains("."))
	    return;

	map_entry<Nick_Live_t> clive = Magick::instance().nickserv.GetLive(source);
	for (unsigned long i = 1; i <= IrcParam(params, 1).WordCount(","); i++)
	{
	    mstring chan(IrcParam(params, 1).ExtractWord(i, ","));
	    clive->Part(chan);
	    clive->Join(chan);
	}
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_D(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_D", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    if (msgtype == "DUMMY")
    {
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_E(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_E", (source, msgtype, params));

    static_cast < void > (source);

    if (msgtype == "END_OF_BURST" || msgtype == "EOB")
    {
	// Tis only nice, afterall ...
	Magick::instance().server.sraw(((proto.Tokens() && !proto.GetNonToken("EOB_ACK").empty()) ? proto.GetNonToken("EOB_ACK") :
												    mstring("EOB_ACK")));
    }
    else if (msgtype == "EOB_ACK")
    {
	// Thank you, drive through ...
    }
    else if (msgtype == "ERROR")
    {
	// ERROR :This is my error
	LOG(LM_NOTICE, "OTHER/SERVER_MSG", (msgtype, params.After(" :")));
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_F(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_F", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    ETCB();
}

void Server::parse_G(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_G", (source, msgtype, params));

    static_cast < void > (source);

    if (msgtype == "GLINE")
    {
	// We will ignore GLINES because they're not relivant to us.
	// we will not be glining our own clients ;P
    }
    else if (msgtype == "GLOBOPS")
    {
	// :source GLOBOPS :This message
	// useless chatter ... can be ignored.
    }
    else if (msgtype == "GNOTICE")
    {
	// :server GNOTICE :This message
	LOG(LM_NOTICE, "OTHER/SERVER_MSG", (msgtype, params.After(" :")));
    }
    else if (msgtype == "GOPER")
    {
	// useless chatter ... can be ignored.
    }
    else if (msgtype == "GZLINE")
    {
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_H(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_H", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    if (msgtype == "HELP" || msgtype == "HELPOP")
    {
	// ignore ...
    }
    else if (msgtype == "HTM")
    {
	// ignore ...
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_I(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_I", (source, msgtype, params));

    if (msgtype == "INFO")
    {
	// :source INFO :server/nick
	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		raw(((proto.Tokens() &&
		      !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " " + source +
		    " :" + tmp.c_str());
		return;
	    }
	    else
		for (unsigned int i = 0; i < sizeof(contrib) / sizeof(mstring); i++)
		    sraw("371 " + source + " :" + contrib[i]);
	}
	sraw("374 " + source + " :End of /INFO report");
    }
    else if (msgtype == "INVITE")
    {
	// :source INVITE target :channel
	//:PreZ INVITE ChanServ :#chatzone

	// We can ignore this, as our clients will not 'join on invite'
	// and we dont really need an invitation to join ...
    }
    else if (msgtype == "ISON")
    {
	// :heaven.darker.net ISON ChanServ
	// :soul.darker.net 303 heaven.darker.net :ChanServ
	// :heaven.darker.net ISON zBlerk
	// :soul.darker.net 303 heaven.darker.net :

	// repl: :our.server 303 source :local.nick
	mstring isonstr;

	mstring request;
	unsigned int i, cnt = IrcParamCount(params);
	for (i=1; i<=cnt; i++)
	    request += " " + IrcParam(params, i);

	for (i = 1; i <= request.WordCount(" "); i++)
	{
	    if (isonstr.length() > proto.MaxLine())
	    {
		sraw("303 " + source + " :" + isonstr);
		isonstr.erase();
	    }
	    if (Magick::instance().nickserv.IsLive(request.ExtractWord(i, " ")))
		isonstr += request.ExtractWord(i, " ") + " ";
	}
	if (!isonstr.empty())
	    sraw("303 " + source + " :" + isonstr);
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_J(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_J", (source, msgtype, params));

    if (msgtype == "JOIN")
    {
	if (source.Contains("."))
	    return;

	map_entry<Nick_Live_t> clive = Magick::instance().nickserv.GetLive(source);
	for (unsigned long i = 1; i <= IrcParam(params, 1).WordCount(","); i++)
	{
	    mstring chan(IrcParam(params, 1).ExtractWord(i, ","));
	    clive->Join(chan);
	}
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_K(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_K", (source, msgtype, params));

    if (msgtype == "KICK")
    {
	// :source KICK #channel target :reason
	// KICK #channel target :reason

	// KICK for UNKNOWN USER.
	mstring nick;

	if (proto.Numeric.User())
	{
	    nick = GetUser("!" + IrcParam(params, 2));
	    if (nick.empty())
	    {
		KillUnknownUser(IrcParam(params, 2));
		return;
	    }
	}
	else
	{
	    nick = GetUser(IrcParam(params, 2));
	    if (nick.empty())
	    {
		KillUnknownUser(IrcParam(params, 2));
		return;
	    }
	}

	mstring chan = GetChannel(IrcParam(params, 1));

	if (!source.Contains(".") && !chan.empty() && !Magick::instance().nickserv.GetLive(source)->IsInChan(chan))
	    KICK(Magick::instance().chanserv.FirstName(), IrcParam(params, 1), source, "You are not in this channel");

	// NOTE: as the message has already been broadcast,
	// we still need to acomodate for it.
	if (!chan.empty() && !nick.empty())
	    Magick::instance().nickserv.GetLive(nick)->Kick(nick, chan);
    }
    else if (msgtype == "KILL")
    {
	// :source/server KILL target :reason
	// LOCAL clients ONLY (remotes are a QUIT).
	//:PreZ KILL kick`kill`abuse :castle.srealm.net.au!PreZ (blah)
	mstring target = GetUser(IrcParam(params, 1));

	if (!target.empty())
	{
	    // sign on services again if they're killed.
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(target);
	    if (nlive->IsServices())
	    {
		LOG(LM_WARNING, "OTHER/KILLED", (target,
						 (!Magick::instance().nickserv.IsLive(source) ? source :
						  Magick::instance().nickserv.GetLive(source)->Mask(Nick_Live_t::N_U_P_H))));
		if (mBase::IsAnyName(target))
		{
		    WLOCK2((lck_Server, "WaitIsOn"));
		    WaitIsOn.insert(target);
		    sraw(((proto.Tokens() && !proto.GetNonToken("ISON").empty()) ? proto.GetNonToken("ISON") :
										   mstring("ISON")) + " :" + target);
		}
		else if (Magick::instance().nickserv.IsRecovered(target))
		    Magick::instance().nickserv.RemRecovered(target);
	    }
	    int wc = params.After(" :").WordCount("!");

	    nlive->Quit("Killed (" + params.After(" :").After("!", wc - 1) + ")");
	    Magick::instance().nickserv.RemLive(nlive->Name());
	    mMessage::CheckDependancies(mMessage::NickNoExists, nlive->Name());
	    if (nlive->Numeric())
		mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
	}
	else
	{
	    LOG(LM_ERROR, "ERROR/REC_FORNONUSER", ("KILL", source, IrcParam(params, 1)));
	}

    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_L(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_L", (source, msgtype, params));

    static_cast < void > (params);

    if (msgtype == "LAG")
    {
    }
    else if (msgtype == "LINKS")
    {
	//:ChanServ LINKS :temple.magick.tm
	//:temple.magick.tm 364 ChanServ temple.magick.tm temple.magick.tm :0 Magick IRC Services Test Network
	//:temple.magick.tm 365 ChanServ temple.magick.tm :End of /LINKS list.

	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		raw(((proto.Tokens() &&
		      !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " " + source +
		    " :" + tmp.c_str());
		return;
	    }
	    else
	    {
		sraw("364 " + source + " " + Magick::instance().startup.Server_Name() + " " +
		     Magick::instance().startup.Server_Name() + " :0 " + Magick::instance().startup.Server_Desc());

		Server::list_t::iterator iter;
		RLOCK2((lck_Server, lck_list));
		for (iter = Magick::instance().server.ListBegin(); iter != Magick::instance().server.ListEnd(); iter++)
		{
		    map_entry < Server_t > server(iter->second);
		    sraw("364 " + source + " " + server->Name() + " " + server->Uplink() + " :" + server->Hops() + " " +
			 server->Description());
		}
	    }
	}

	sraw("365 " + source + " " + Magick::instance().startup.Server_Name() + " :End of /LINKS list.");

    }
    else if (msgtype == "LIST")
    {
	sraw("321 " + source + " Channel :Users  Name");

	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		raw(((proto.Tokens() &&
		      !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " " + source +
		    " :" + tmp.c_str());
		return;
	    }
	    else
	    {
		ChanServ::live_t::iterator iter;
		{
		    RLOCK2((lck_ChanServ, lck_live));
		    for (iter = Magick::instance().chanserv.LiveBegin(); iter != Magick::instance().chanserv.LiveEnd(); iter++)
		    {
			map_entry < Chan_Live_t > clive(iter->second);
			if (!(clive->HasMode("s") || clive->HasMode("p")))
			    sraw("322 " + source + " " + clive->Name() + " " + mstring(clive->Users()) + " :" +
				 clive->Topic());
		    }
		}
	    }
	}

	sraw("323 " + source + " :End of /LIST");
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_M(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_M", (source, msgtype, params));

    if (msgtype == "MKPASSWD")
    {
    }
    else if (msgtype == "MODE")
    {
	// :source MODE source :mode
	// :source MODE #channel mode params...
	// :server MODE #channel mode params... creationtime
	if (!GetChannel(IrcParam(params, 1)).empty())
	{
	    mstring chan = GetChannel(IrcParam(params, 1));

	    if (!chan.empty())
	    {
		mstring mode = IrcParam(params, 2);

		unsigned int i, wc = IrcParamCount(params);
		mstring mode_param;
		for (i=3; i<=wc; i++)
		    mode_param += " " + IrcParam(params, i);

		if (proto.Numeric.User())
		{
		    unsigned int fwdarg = 1;
		    mstring new_param;

		    for (unsigned int i = 0; i < mode.length() && fwdarg <= mode_param.WordCount(" "); i++)
		    {
			if (proto.ChanModeArg().Contains(mode[i]))
			{
			    // We MUST convert numerics to users ... *sigh*
			    switch (mode[i])
			    {
			    case 'o':
			    case 'h':
			    case 'v':
				new_param += " " + GetUser("!" + mode_param.ExtractWord(fwdarg, " "));
				break;
			    default:
				new_param += " " + mode_param.ExtractWord(fwdarg, " ");
			    }
			    fwdarg++;
			}
		    }
		    mode_param = new_param;
		}
		Magick::instance().chanserv.GetLive(chan)->Mode(source, mode + " " + mode_param);
	    }
	    else
	    {
		LOG(LM_CRITICAL, "ERROR/REC_FORNONCHAN", ("MODE", source, IrcParam(params, 1)));
	    }
	}
	else
	{
	    mstring nick = GetUser(IrcParam(params, 1));

	    if (!nick.empty())
	    {
		Magick::instance().nickserv.GetLive(nick)->Mode(IrcParam(params, 2));
	    }
	    else
	    {
		LOG(LM_CRITICAL, "ERROR/REC_FORNONCHAN", ("MODE", source, IrcParam(params, 1)));
	    }
	}
    }
    else if (msgtype == "MODNICK")
    {
	// forcably changed nicks (handle like nick)
    }
    else if (msgtype == "MOTD")
    {
	if (mFile::Exists(Magick::instance().files.Motdfile()))
	{
	    sraw("375 " + source + " :Message Of The Day");
	    {
		RLOCK((lck_IrcSvcHandler));
		if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
		{
		    mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		    raw(((proto.Tokens() &&
			  !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " " +
			source + " :" + tmp.c_str());
		    return;
		}
		else
		{
		    vector < mstring > tmp = mFile::UnDump(Magick::instance().files.Motdfile());
		    for (unsigned int i = 0; i < tmp.size(); i++)
			sraw("372 " + source + " :" + tmp[i].c_str());
		}
	    }
	    sraw("376 " + source + " :End of MOTD.");
	}
	else
	{
	    sraw("422 " + source + " :No MOTD exists.");
	}
    }
    else if (msgtype == "MODULE")
    {
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_N(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_N", (source, msgtype, params));

    if (msgtype == "NACHAT")
    {
    }
    else if (msgtype == "NAMES")
    {
	// :source NAMES #channel our.server
//:soul.darker.net 353 ChanServ = #chatzone :killkickabuseme @Aimee Jupiter @Allanon Ghost_ wildrose
//:soul.darker.net 366 ChanServ #chatzone :End of /NAMES list.
	sraw("366 " + source + " " + IrcParam(params, 1) + " :End of /NAMES list.");

    }
    else if (msgtype == "NETINFO")
    {
	// Ignore -- unrealism
    }
    else if (msgtype == "NICK")
    {
	if (source.Contains("."))
	{
	    // NEW USER
	    mstring newnick = IrcParam(params, 1);

	    // DONT kill when we do SQUIT protection.
	    map < mstring, set < mstring > >::iterator i;
	    {
		RLOCK((lck_Server, "ToBeSquit"));
		for (i = ToBeSquit.begin(); i != ToBeSquit.end(); i++)
		{
		    WLOCK2((lck_Server, "ToBeSquit", i->first));
		    set < mstring >::iterator k = i->second.find(newnick.LowerCase());
		    if (k != i->second.end())
			i->second.erase(k);
		}
	    }

	    mstring server;
	    mstring modes;

	    switch (proto.Signon())
	    {
	    case 0000:
	    case 0001:
		break;
	    case 1000:		// NICK nick hops time user host server :realname
	    case 1001:		// NICK nick hops time user host server 1 :realname
	    case 1002:		// NICK nick hops time user host server 0 real-host :realname
		server = IrcParam(params, 6);
		break;
	    case 1003:		// NICK nick hops time user real-host host server 0 :realname
		server = IrcParam(params, 7);
		break;
	    case 2000:		// NICK nick hops time mode user host server :realname
	    case 2001:		// NICK nick hops time mode user host server 0 :realname
		modes = IrcParam(params, 4);
		server = IrcParam(params, 7);
		break;
	    case 2002:		// NICK nick hops time mode user host maskhost server 0 :realname
		modes = IrcParam(params, 4);
		server = IrcParam(params, 8);
		break;
	    case 2003:		// NICK nick hops time user host server 0 mode maskhost :realname
		modes = IrcParam(params, 8);
		server = IrcParam(params, 6);
		break;
	    case 3000:		// server NICK nick hops time user host [mode] ipaddr numeric :realname
		modes = IrcParam(params, 6);
		if (modes[0u] != '+')
		    modes.erase();
		server = source;
		break;
	    }
	    if (proto.Numeric.Server() && !server.Contains("."))
		server.prepend("@");
	    server = GetServer(server);

	    if (Magick::instance().nickserv.IsLiveAll(newnick))
	    {
		map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(newnick);
		COM(("Previous SQUIT checking if %s == %s and %s == %s", nlive->Squit().c_str(), server.c_str(),
		     nlive->SignonTime().DateTimeString().c_str(),
		     mDateTime(static_cast < time_t > (atoul(IrcParam(params, 3)))).DateTimeString().c_str()));
		// IF the squit server = us, and the signon time matches
		if (nlive->Squit().IsSameAs(server, true) &&
		    nlive->SignonTime() == mDateTime(static_cast < time_t > (atoul(IrcParam(params, 3)))))
		{
		    nlive->ClearSquit(modes);
		    mMessage::CheckDependancies(mMessage::NickExists, newnick);
		    if (nlive->Numeric())
			mMessage::CheckDependancies(mMessage::NickExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
		    return;	// nice way to avoid repeating ones self :)
		}
		else
		{
		    nlive->Quit("SQUIT - " + nlive->Server());
		    Magick::instance().nickserv.RemLive(newnick);
		    mMessage::CheckDependancies(mMessage::NickNoExists, newnick);
		    if (nlive->Numeric())
			mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
		}
	    }

	    switch (proto.Signon())
	    {
	    case 0000:
	    case 0001:
		break;
	    case 1000:		// NICK nick hops time user host server :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(IrcParam(params, 2),
					static_cast < time_t > (atoul(IrcParam(params, 3))), server,
					IrcParam(params, 4), IrcParam(params, 5), params.After(" :")));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 1001:		// NICK nick hops time user host server 1 :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(IrcParam(params, 1),
					static_cast < time_t > (atoul(IrcParam(params, 3))), server,
					IrcParam(params, 4), IrcParam(params, 5), params.After(" :")));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 1002:		// NICK nick hops time user host server 0 real-host :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(IrcParam(params, 1),
					static_cast < time_t > (atoul(IrcParam(params, 3))), server,
					IrcParam(params, 4), IrcParam(params, 8), params.After(" :")));
		    tmp->AltHost(IrcParam(params, 5));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 1003:		// NICK nick hops time user real-host host server 0 :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(IrcParam(params, 1),
					static_cast < time_t > (atoul(IrcParam(params, 4))), server,
					IrcParam(params, 4), IrcParam(params, 5), params.After(" :")));
		    tmp->AltHost(IrcParam(params, 6));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 2000:		// NICK nick hops time mode user host server :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(IrcParam(params, 1),
					static_cast < time_t > (atoul(IrcParam(params, 3))), server,
					IrcParam(params, 5), IrcParam(params, 6), params.After(" :")));
		    tmp->Mode(modes);
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 2001:		// NICK nick hops time mode user host server 0 :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(IrcParam(params, 1),
					static_cast < time_t > (atoul(IrcParam(params, 3))), server,
					IrcParam(params, 5), IrcParam(params, 6), params.After(" :")));
		    tmp->Mode(modes);
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 2002:		// NICK nick hops time mode user host maskhost server 0 :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(IrcParam(params, 1),
					static_cast < time_t > (atoul(IrcParam(params, 3))), server,
					IrcParam(params, 5), IrcParam(params, 6), params.After(" :")));
		    tmp->Mode(modes);
		    tmp->AltHost(IrcParam(params, 7));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 2003:		// NICK nick hops time user host server 0 mode maskhost :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(IrcParam(params, 1),
					static_cast < time_t > (atoul(IrcParam(params, 3))), server,
					IrcParam(params, 4), IrcParam(params, 5), params.After(" :")));
		    tmp->Mode(modes);
		    tmp->AltHost(IrcParam(params, 9));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 3000:		// server NICK nick hops time user host [mode] ipaddr numeric :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(IrcParam(params, 1),
					static_cast < time_t > (atoul(IrcParam(params, 3))), server,
					IrcParam(params, 4), IrcParam(params, 5), params.After(" :")));
		    if (modes.length())
		    {
			tmp->Mode(modes);
			tmp->Numeric(proto.Numeric.UserNumeric(IrcParam(params, 8)));
		    }
		    else
			tmp->Numeric(proto.Numeric.UserNumeric(IrcParam(params, 7)));

		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    }

	    if (Magick::instance().nickserv.IsLive(newnick))
	    {
		map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(newnick);
		if (nlive->Server().empty())
		{
		    mMessage::CheckDependancies(mMessage::NickExists, newnick);
		    if (nlive->Numeric())
			mMessage::CheckDependancies(mMessage::NickExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
		    KILL(Magick::instance().nickserv.FirstName(), newnick, nlive->RealName());
		    return;
		}

		{
		    WLOCK2((lck_Server, "i_UserMax"));
		    if (i_UserMax < Magick::instance().nickserv.LiveSize())
		    {
			MCB(i_UserMax);
			i_UserMax = Magick::instance().nickserv.LiveSize();
			MCE(i_UserMax);
		    }
		}

		mMessage::CheckDependancies(mMessage::NickExists, newnick);
		if (nlive->Numeric())
		    mMessage::CheckDependancies(mMessage::NickExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
		// HAS to be AFTER the nickname is added to map.
		CommServ::list_t::iterator iter;
		mstring setmode;

		{
		    RLOCK2((lck_CommServ, lck_list));
		    for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		    {
			map_entry < Committee_t > comm(iter->second);
			if (comm->IsOn(newnick))
			{
			    if (comm->Name() == Magick::instance().commserv.ALL_Name())
				setmode += Magick::instance().commserv.ALL_SetMode();
			    else if (comm->Name() == Magick::instance().commserv.REGD_Name())
				setmode += Magick::instance().commserv.REGD_SetMode();
			    else if (comm->Name() == Magick::instance().commserv.OPER_Name())
				setmode += Magick::instance().commserv.OPER_SetMode();
			    else if (comm->Name() == Magick::instance().commserv.ADMIN_Name())
				setmode += Magick::instance().commserv.ADMIN_SetMode();
			    else if (comm->Name() == Magick::instance().commserv.SOP_Name())
				setmode += Magick::instance().commserv.SOP_SetMode();
			    else if (comm->Name() == Magick::instance().commserv.SADMIN_Name())
				setmode += Magick::instance().commserv.SADMIN_SetMode();

			    MLOCK((lck_CommServ, lck_list, comm->Name(), "message"));
			    for (comm->message = comm->MSG_begin(); comm->message != comm->MSG_end(); comm->message++)
			    {
				Magick::instance().servmsg.send(newnick,
								"[" + IRC_Bold + comm->Name() + IRC_Off + "] " +
								comm->message->Entry());
			    }
			}
		    }
		}
		if (!setmode.empty())
		{
		    mstring setmode2;

		    for (unsigned int j = 0; j < setmode.size(); j++)
		    {
			if (setmode[j] != '+' && setmode[j] != '-' && setmode[j] != ' ' && !nlive->HasMode(setmode[j]))
			    setmode2 += setmode[j];
		    }
		    if (!setmode2.empty())
			SVSMODE(Magick::instance().nickserv.FirstName(), newnick, "+" + setmode2);
		}
		if (Magick::instance().nickserv.IsStored(newnick))
		{
		    map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(newnick);
		    if (nstored->Forbidden())
		    {
			SEND(Magick::instance().nickserv.FirstName(), newnick, "ERR_SITUATION/FORBIDDEN",
			     (ToHumanTime(Magick::instance().nickserv.Ident(), newnick)));
		    }
		    else if (nstored->Protect() && !nstored->IsOnline())
		    {
			SEND(Magick::instance().nickserv.FirstName(), newnick, "ERR_SITUATION/PROTECTED",
			     (ToHumanTime(Magick::instance().nickserv.Ident(), newnick)));
		    }
		}
	    }
	}
	else
	{
	    if (Magick::instance().nickserv.IsLive(source))
	    {
		map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
		// CHANGE NICK
		if (!source.IsSameAs(IrcParam(params, 1)))
		{
		    mstring newnick(IrcParam(params, 1));

		    set < mstring > wason = nlive->Name(newnick);
		    Magick::instance().nickserv.RemLive(source);
		    nlive->setDelete(false);
		    mMessage::CheckDependancies(mMessage::NickNoExists, source);
		    Magick::instance().nickserv.AddLive(nlive);
		    mMessage::CheckDependancies(mMessage::NickExists, newnick);
		    if (Magick::instance().nickserv.IsRecovered(source))
		    {
			Magick::instance().server.NICK(source,
						       (Magick::instance().startup.Ownuser() ? source :
							Magick::instance().startup.Services_User()),
						       Magick::instance().startup.Services_Host(),
						       Magick::instance().startup.Server_Name(), "Nickname Enforcer");
		    }

		    CommServ::list_t::iterator iter;
		    mstring setmode;

		    {
			RLOCK2((lck_CommServ, lck_list));
			for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd();
			     iter++)
			{
			    map_entry < Committee_t > comm(iter->second);
			    if (wason.find(comm->Name()) == wason.end() && comm->IsOn(newnick))
			    {
				if (comm->Name() == Magick::instance().commserv.ALL_Name())
				    setmode += Magick::instance().commserv.ALL_SetMode();
				else if (comm->Name() == Magick::instance().commserv.REGD_Name())
				    setmode += Magick::instance().commserv.REGD_SetMode();
				else if (comm->Name() == Magick::instance().commserv.OPER_Name())
				    setmode += Magick::instance().commserv.OPER_SetMode();
				else if (comm->Name() == Magick::instance().commserv.ADMIN_Name())
				    setmode += Magick::instance().commserv.ADMIN_SetMode();
				else if (comm->Name() == Magick::instance().commserv.SOP_Name())
				    setmode += Magick::instance().commserv.SOP_SetMode();
				else if (comm->Name() == Magick::instance().commserv.SADMIN_Name())
				    setmode += Magick::instance().commserv.SADMIN_SetMode();

				MLOCK((lck_CommServ, lck_list, comm->Name(), "message"));
				for (comm->message = comm->MSG_begin(); comm->message != comm->MSG_end(); comm->message++)
				{
				    Magick::instance().servmsg.send(newnick,
								    "[" + IRC_Bold + comm->Name() + IRC_Off + "] " +
								    comm->message->Entry());
				}
			    }
			}
		    }
		    if (!setmode.empty())
		    {
			mstring setmode2;

			for (unsigned int j = 0; j < setmode.size(); j++)
			{
			    if (setmode[j] != '+' && setmode[j] != '-' && setmode[j] != ' ' && !nlive->HasMode(setmode[j]))
				setmode2 += setmode[j];
			}
			if (!setmode2.empty())
			    SVSMODE(Magick::instance().nickserv.FirstName(), newnick, "+" + setmode2);
		    }

		    if (Magick::instance().nickserv.IsStored(newnick))
		    {
			map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(newnick);
			if (nstored->Forbidden())
			{
			    SEND(Magick::instance().nickserv.FirstName(), newnick, "ERR_SITUATION/FORBIDDEN",
				 (ToHumanTime(Magick::instance().nickserv.Ident(), newnick)));
			}
			else if (nstored->Protect() && !nstored->IsOnline())
			{
			    SEND(Magick::instance().nickserv.FirstName(), newnick, "ERR_SITUATION/PROTECTED",
				 (ToHumanTime(Magick::instance().nickserv.Ident(), newnick)));
			}
		    }

		}
		else
		{
		    nlive->Name(IrcParam(params, 1));
		}
	    }
	}
    }
    else if (msgtype == "NOTICE")
    {
	// :source NOTICE target/#channel :message
	// NOTICE target :message
	if (!Magick::instance().GotConnect())
	    return;

	mstring dest = IrcParam(params, 1);
	char c = 0;

	while (c != dest.first())
	{
	    c = dest.first();
	    switch (c)
	    {
	    case '@':		// Op Msg
	    case '%':		// HalfOp Msg
	    case '+':		// Voice Msg
		dest.erase(0, 0);
		break;
	    }
	}

	if (!IsChan(dest) && dest[0u] != '$' && dest[0u] != '#')
	{
	    {
		RLOCK((lck_IrcSvcHandler));
		if (Magick::instance().ircsvchandler != NULL && !Magick::instance().ircsvchandler->Burst())
		{
		    LOG(LM_WARNING, "ERROR/REC_FORNONUSER", ("NOTICE", source, IrcParam(params, 1)));
		}
	    }
	}
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_O(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_O", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    if (msgtype == "OPER")
    {
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_P(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_P", (source, msgtype, params));

    if (msgtype == "PART")
    {
	if (source.Contains("."))
	    return;

	map_entry<Nick_Live_t> clive = Magick::instance().nickserv.GetLive(source);
	for (unsigned long i = 1; i <= IrcParam(params, 1).WordCount(","); i++)
	{
	    mstring chan(IrcParam(params, 1).ExtractWord(i, ","));
	    clive->Part(chan);
	}
    }
    else if (msgtype == "PASS")
    {
	if (!source.Contains("."))
	    return;

	/* TRACE ... (usually leave off) */
	/*
	 * for (int i=0; i<tt_MAX; i++)
	 * Trace::TurnSet(static_cast<threadtype_enum>(i), 0xffff);
	 */

	// PASS :password
	if (IrcParam(params, 1) != Magick::instance().startup.Server(Magick::instance().CurrentServer()).second.second)
	{
	    LOG(LM_ERROR, "OTHER/WRONGPASS", (Magick::instance().CurrentServer()));
	    CP(("Server password mismatch.  Closing socket."));
	    raw(((proto.Tokens() &&
		  !proto.GetNonToken("ERROR").empty()) ? proto.GetNonToken("ERROR") : mstring("ERROR")) +
		" :No Access (passwd mismatch) [" + Magick::instance().CurrentServer() + "]");
	    raw(((proto.Tokens() &&
		  !proto.GetNonToken("ERROR").empty()) ? proto.GetNonToken("ERROR") : mstring("ERROR")) + " :Closing Link: [" +
		Magick::instance().CurrentServer() + "] (Bad Password)");
	    sraw("464 " + Magick::instance().CurrentServer() + " :" + "Password Incorrect");
	    Magick::instance().Disconnect();
	}
    }
    else if (msgtype == "PING")
    {
	// PING :some.server
	// :some.server PING some.server :our.server
	sraw(((proto.Tokens() &&
	       !proto.GetNonToken("PONG").empty()) ? proto.GetNonToken("PONG") : mstring("PONG")) + " " +
	     Magick::instance().startup.Server_Name() + " :" + source);
    }
    else if (msgtype == "PONG")
    {
	// Receive info back for LAG MONITOR.
	// we'll send out ':our.server PING our.server :remote.server'
	// for EACH server, at the same time execute Ping() on it.

	// :server PONG server :our.server
	if (IsList(source))
	    GetList(source)->Pong();

    }
    else if (msgtype == "PRIVMSG")
    {
	// :source PRIVMSG target/#channel :message
	if (!Magick::instance().GotConnect())
	    return;

	mstring dest = IrcParam(params, 1);
	char c = 0;

	while (c != dest.first())
	{
	    c = dest.first();
	    switch (c)
	    {
	    case '@':		// Op Msg
	    case '%':		// HalfOp Msg
	    case '+':		// Voice Msg
		dest.erase(0, 0);
		break;
	    }
	}

	if (!IsChan(dest) && dest[0u] != '$' && dest[0u] != '#')
	{
	    {
		RLOCK((lck_IrcSvcHandler));
		if (Magick::instance().ircsvchandler != NULL && !Magick::instance().ircsvchandler->Burst())
		{
		    LOG(LM_WARNING, "ERROR/REC_FORNONUSER", ("PRIVMSG", source, IrcParam(params, 1)));
		}
	    }
	}
    }
    else if (msgtype == "PROTOCTL")
    {
	if (!source.Contains("."))
	    return;

	mstring tmp = (" " + params + " ");
	// Bahamut version of the PROTOCTL line
	if (tmp.Contains(" TOKEN "))
	    proto.Tokens(true);
//	if (tmp.Contains(" SSJOIN ") || tmp.Contains(" SJOIN ") || tmp.Contains(" SJOIN2 ") || tmp.Contains(" SJ3 "))
//	    proto.SJoin(true);
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_Q(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_Q", (source, msgtype, params));

    if (msgtype == "QUIT")
    {
	// :source QUIT :reason
	// :source QUIT :server server

	if (source.Contains("."))
	    return;

	unsigned long numeric = 0;

	// OK, 4 words (always for squit), the 4nd word is a server
	// and the 3rd word is the uplink of the 4th word (a server)
	mstring server1 = GetServer(params.After(" :").ExtractWord(1, " "));
	mstring server2 = GetServer(params.After(" :").ExtractWord(2, " "));

	if (params.After(" :").WordCount(" ") == 2 && !server1.empty() && !server2.empty() &&
	    server1.IsSameAs(GetList(server2)->Uplink(), true))
	{
	    // Suspected SQUIT
	    //
	    // - Add user to ToBeSquit map under servername
	    // - Add data(4) to ServerSquit map with a timer to clear
	    //
	    // IF no SQUIT message received, user is QUIT and server
	    // is removed from ServerSquit map -- ie. its FAKE!
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
	    numeric = nlive->Numeric();
	    nlive->SetSquit();
	    WLOCK2((lck_Server, "ToBeSquit"));
	    MCB(ToBeSquit.size());
	    ToBeSquit[server2.LowerCase()].insert(source.LowerCase());
	    LOG(LM_NOTICE, "OTHER/SQUIT_FIRST", (server2, server1));

	    WLOCK3((lck_Server, "ServerSquit"));
	    if (ServerSquit.find(nlive->Server()) == ServerSquit.end())
	    {
		CB(1, ServerSquit.size());
		while (Magick::instance().Pause())
		    ACE_OS::sleep(1);
		ServerSquit[nlive->Server()] =
		    Magick::instance().reactor().schedule_timer(&tobesquit, new mstring(nlive->Server()), ACE_Time_Value(10));
		CE(1, ServerSquit.size());
	    }
	    MCE(ToBeSquit.size());
	}
	else
	{
	    // Normal quit ...

	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
	    numeric = nlive->Numeric();
	    nlive->Quit(params.After(" :"));

	    // Kind of illegal to do, but accomodate anyway, re-signon
	    // services if someone quits them (how?!?)
	    if (nlive->IsServices())
	    {
		if (mBase::IsAnyName(source))
		{
		    WLOCK2((lck_Server, "WaitIsOn"));
		    WaitIsOn.insert(source);
		    sraw(((proto.Tokens() &&
			   !proto.GetNonToken("ISON").empty()) ? proto.GetNonToken("ISON") : mstring("ISON")) + " :" + source);
		}
		else if (Magick::instance().nickserv.IsRecovered(source))
		    Magick::instance().nickserv.RemRecovered(source);
	    }
	    Magick::instance().nickserv.RemLive(source);
	}
	mMessage::CheckDependancies(mMessage::NickNoExists, source);
	if (numeric)
	    mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(numeric));
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_R(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_R", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    if (msgtype == "RAKILL")
    {
	// We will ignore AKILLS because they're not relivant to us.
	// we will not be akilling our own clients ;P
    }
    else if (msgtype == "REHASH")
    {
	// Will we ever get this via. net??  ignore.
    }
    else if (msgtype == "RESTART")
    {
	// Will we ever get this via. net??  ignore.
    }
    else if (msgtype == "RPING")
    {
	// :server RPING <pinged-server> <original-sender> <start-time-in-sec> <start-time-in-usec> :<remark>
	sraw(((proto.Tokens() && !proto.GetNonToken("RPONG").empty()) ? proto.GetNonToken("RPONG") : mstring("RPONG")) +
	     source + " " + IrcParam(params, 2) + " " + IrcParam(params, 3) + " " + IrcParam(params, 4) + " :" + 
	     IrcParam(params, 5));
    }
    else if (msgtype == "RPONG")
    {
    }
    else if (msgtype == "RQLINE")
    {
	// We will ignore RQLINES because they're not relivant to us.
	// we will not be qlining our own clients ;P
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_S(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_S", (source, msgtype, params));

    if (msgtype == "SDESC")
    {
    }
    else if (msgtype == "SENDUMODE")
    {
    }
    else if (msgtype == "SETHOST")
    {
	if (source.Contains("."))
	    return;

	// From Unreal IRCD
	// :source SVSHOST newhost
	Magick::instance().nickserv.GetLive(source)->AltHost(IrcParam(params, 1));
    }
    else if (msgtype == "SETIDENT")
    {
    }
    else if (msgtype == "SETNAME")
    {
    }
    else if (msgtype == "SETTIME")
    {
    }
    else if (msgtype == "SERVER")
    {
	if (!source.Contains("."))
	    return;

	// SERVER downlink hops :description
	// :uplink SERVER downlink hops :description

	unsigned long numeric = 0;

	if (source.IsSameAs(Magick::instance().startup.Server_Name(), true))
	{
	    if (Magick::instance().startup.IsAllowed(IrcParam(params, 1), source))
	    {
		// unreal puts the numeric in the desctiption, ie.
		// SERVER downlink hops :Uversion-servopts-numeric description
		//        Only this way for direct links --^^^^^^^
		if (proto.Numeric.UnrealNumeric())
		{
		    numeric = proto.Numeric.ServerLineNumeric(params.After(" :").ExtractWord(3, "-"));
		    map_entry < Server_t >
			tmp(new
			    Server_t(IrcParam(params, 1).LowerCase(),
				     atoi(IrcParam(params, 2).LowerCase().c_str()), params.After(" :").After(" "),
				     numeric));
		    AddList(tmp);
		}
		else
		{
		    map_entry < Server_t >
			tmp(new
			    Server_t(IrcParam(params, 1).LowerCase(),
				     atoi(IrcParam(params, 2).LowerCase().c_str()), params.After(" :")));
		    if (proto.Numeric.Server())
		    {
			numeric = proto.Numeric.ServerLineNumeric(IrcParam(params, proto.Numeric.Field()));
			tmp->Numeric(numeric);
		    }
		    AddList(tmp);
		}

		LOG(LM_INFO, "OTHER/LINK", (IrcParam(params, 1), Magick::instance().startup.Server_Name()));
		Magick::instance().GotConnect(true);
		SignOnAll();
		mMessage::CheckDependancies(mMessage::ServerExists, IrcParam(params, 1));
		if (numeric)
		    mMessage::CheckDependancies(mMessage::ServerExists, "@" + proto.Numeric.ServerNumeric(numeric));
	    }
	    else
	    {
//                  ANNOUNCE(Magick::instance().operserv.FirstName(),
//                      "MISC/INVALIDLINK", (
//                      IrcParam(params, 1),
//                      Magick::instance().startup.Server_Name()));
		LOG(LM_ERROR, "OTHER/INVALIDLINK", (IrcParam(params, 1), Magick::instance().startup.Server_Name()));
		raw("SQUIT " + IrcParam(params, 1) + " :" + Magick::instance().getMessage("MISC/MAYNOTLINK"));
		Magick::instance().Disconnect();
	    }
	}
	else
	{
	    if (Magick::instance().startup.IsAllowed(IrcParam(params, 1), source))
	    {
		if (IsList(source))
		{
		    map_entry < Server_t >
			tmp(new
			    Server_t(IrcParam(params, 1).LowerCase(), source,
				     atoi(IrcParam(params, 2).LowerCase().c_str()), params.After(" :")));
		    if (proto.Numeric.Server())
		    {
			numeric = proto.Numeric.ServerLineNumeric(IrcParam(params, proto.Numeric.Field()));
			tmp->Numeric(numeric);
		    }
		    AddList(tmp);
		    LOG(LM_INFO, "OTHER/LINK", (IrcParam(params, 1), source));

		    mMessage::CheckDependancies(mMessage::ServerExists, IrcParam(params, 1));
		    if (numeric)
			mMessage::CheckDependancies(mMessage::ServerExists, "@" + proto.Numeric.ServerNumeric(numeric));
		}
		else
		{
		    LOG(LM_ERROR, "ERROR/REC_FORNONSERVER", ("SERVER", IrcParam(params, 1), source));
		}
	    }
	    else
	    {
		ANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/INVALIDLINK", (IrcParam(params, 1), source));
		LOG(LM_ERROR, "OTHER/INVALIDLINK", (IrcParam(params, 1), source));
		sraw("SQUIT " + IrcParam(params, 1) + " :" + Magick::instance().getMessage("MISC/MAYNOTLINK"));
	    }
	}
    }
    else if (msgtype == "SGLINE")
    {
    }
    else if (msgtype == "SHUN")
    {
    }
    else if (msgtype == "SILENCE")
    {
	// Another one to ignore ...
    }
    else if (msgtype == "SJOIN")
    {
	//:server SJOIN chan-stamp #channel +modes extra-params :@opd_nick +voice_nick nick 
	//:relic.devel.relic.net SJOIN 952608432 #blah + <none> :@Kwahraw +PreZ Zephyr

	//:user SJOIN chan-stamp #channel #channel etc...

	if (source.Contains("."))
	{
	    mstring chan(IrcParam(params, 2));

	    unsigned int i;

	    vector < mstring > users;
	    mstring modes;

	    if (params.Before(" :").WordCount(" ") >= 3)
		modes = params.Before(" :").ExtractWord(3, " ");
	    mstring mode_params, nick;
	    bool oped, halfoped, voiced, owner, prot, isban, isexcept;

	    if (modes.length())
	    {
		for (i = 0; i < proto.ChanModeArg().length(); i++)
		    if (modes.Contains(proto.ChanModeArg() [i]))
		    {
			// NOTE: muse be 4, not 3, because line STARTS with a space.
			mode_params = params.Before(" :").After(" ", 4);
			break;
		    }
	    }
	    for (i = 1; i <= params.After(" :").WordCount(" "); i++)
	    {
		nick = params.After(" :").ExtractWord(i, " ");
		if (!nick.empty())
		{
		    char c = 0;

		    oped = halfoped = voiced = owner = prot = false;
		    isban = isexcept = false;
		    while (c != nick[0u])
		    {
			c = nick[0u];
			switch (nick[0u])
			{
			case '@':
			    oped = true;
			    nick.erase(0, 0);
			    break;
			case '%':
			    halfoped = true;
			    nick.erase(0, 0);
			    break;
			case '+':
			    voiced = true;
			    nick.erase(0, 0);
			    break;
			case '*':
			    owner = true;
			    nick.erase(0, 0);
			    break;
			case '~':
			    prot = true;
			    nick.erase(0, 0);
			    break;
			case '&':
			    isban = true;
			    nick.erase(0, 0);
			    break;
			case '"':
			    isexcept = true;
			    nick.erase(0, 0);
			    break;
			}
		    }
		    if (isban)
		    {
			modes += "b";
			mode_params += " " + nick;
			nick.erase();
		    }
		    else if (isexcept)
		    {
			modes += "e";
			mode_params += " " + nick;
			nick.erase();
		    }
		    else
		    {
			mstring newnick = GetUser(nick);
			if (newnick.empty())
			{
			    LOG(LM_WARNING, "ERROR/REC_FORNONUSER", ("SJOIN", source, nick));
			    if (oped)
				LOG(LM_WARNING, "ERROR/MODE_NOTINCHAN", ('+', 'o', source, nick, chan));
			    if (halfoped)
				LOG(LM_WARNING, "ERROR/MODE_NOTINCHAN", ('+', 'h', source, nick, chan));
			    if (voiced)
				LOG(LM_WARNING, "ERROR/MODE_NOTINCHAN", ('+', 'v', source, nick, chan));
			    if (owner)
				LOG(LM_WARNING, "ERROR/MODE_NOTINCHAN", ('+', 'q', source, nick, chan));
			    if (prot)
				LOG(LM_WARNING, "ERROR/MODE_NOTINCHAN", ('+', 'a', source, nick, chan));
			    continue;
			}
			nick = newnick;

			if (oped)
			{
			    modes += "o";
			    mode_params += " " + nick;
			}
			if (halfoped)
			{
			    modes += "h";
			    mode_params += " " + nick;
			}
			if (voiced)
			{
			    modes += "v";
			    mode_params += " " + nick;
			}
			if (owner)
			{
			    modes += "q";
			    mode_params += " " + nick;
			}
			if (prot)
			{
			    modes += "a";
			    mode_params += " " + nick;
			}

			users.push_back(nick);
		    }
		}
	    }

	    for (i = 0; i < users.size(); i++)
	    {
		Magick::instance().nickserv.GetLive(users[i])->Join(chan);
	    }
	    CP(("MODE TO %s: %s", chan.LowerCase().c_str(), (modes + " " + mode_params).c_str()));
	    if (Magick::instance().chanserv.IsLive(chan))
	    {
		map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(chan);
		clive->Creation_Time(static_cast < time_t > (atoul(IrcParam(params, 1))));
		clive->Mode(source, "+" + modes + " " + mode_params);
	    }
	    else if (modes.length() > 1)
	    {
		LOG(LM_WARNING, "ERROR/REC_FORNONCHAN", ("MODE", source, chan));
	    }
	}
	else
	{
	    unsigned int i;
	    bool oped, halfoped, voiced, owner, prot;
	    mstring chan;

	    if (Magick::instance().nickserv.IsLive(source))
	    {
		unsigned int count = IrcParamCount(params);
		for (i = 2; i <= count; i++)
		{
		    char c = 0;

		    chan = IrcParam(params, i);
		    oped = halfoped = voiced = owner = prot = false;
		    while (c != chan[0u])
		    {
			c = chan[0u];
			switch (chan[0u])
			{
			case '@':
			    oped = true;
			    chan.erase(0, 0);
			    break;
			case '%':
			    halfoped = true;
			    chan.erase(0, 0);
			    break;
			case '+':
			    voiced = true;
			    chan.erase(0, 0);
			    break;
			case '*':
			    owner = true;
			    chan.erase(0, 0);
			    break;
			case '~':
			    prot = true;
			    chan.erase(0, 0);
			    break;
			}
		    }
		    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
		    nlive->Join(chan);
		    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(chan);
		    if (oped)
			clive->Mode(nlive->Server(), "+o " + source);
		    if (halfoped)
			clive->Mode(nlive->Server(), "+h " + source);
		    if (voiced)
			clive->Mode(nlive->Server(), "+v " + source);
		    if (owner)
			clive->Mode(nlive->Server(), "+q " + source);
		    if (prot)
			clive->Mode(nlive->Server(), "+a " + source);
		}
	    }
	    else
	    {
		LOG(LM_WARNING, "ERROR/REC_FORNONUSER", ("SJOIN", source, source));
	    }
	}
    }
    else if (msgtype == "SMO")
    {
	// Thanks to unreal for ANOTHER chatter message ..
    }
    else if (msgtype == "SQLINE")
    {
	// We will ignore SQLINES because they're not relivant to us.
	// we will not be qlining our own clients ;P
    }
    else if (msgtype == "SQUIT")
    {
	// SQUIT shadow.darker.net :soul.darker.net lifestone.darker.net
	// SQUIT lifestone.darker.net :Ping timeout
	// :PreZ SQUIT server :reason

	mstring server = GetServer(IrcParam(params, 1));

	if (!server.empty())
	{
	    map_entry < Server_t > svr = GetList(server);
	    if (svr->Jupe())
		raw(((proto.Tokens() &&
		      !proto.GetNonToken("SQUIT").empty()) ? proto.GetNonToken("SQUIT") : mstring("SQUIT")) + " " +
		    svr->Name() + " :" + params.After(":").ExtractWord(2, " "));

	    unsigned int i;

	    vector < mstring > tlist = svr->AllDownlinks();
	    tlist.push_back(svr->Name());
	    vector < unsigned long > nlist;

	    for (i = 0; i < tlist.size(); i++)
	    {
		map_entry < Server_t > s = GetList(tlist[i]);
		if (s->Numeric())
		    nlist.push_back(s->Numeric());
	    }

	    LOG(LM_NOTICE, "OTHER/SQUIT_SECOND", (svr->Name(), svr->Uplink()));

	    RemList(svr->Name());

	    {
		WLOCK2((lck_Server, "ToBeSquit"));
		WLOCK3((lck_Server, "ServerSquit"));
		MCB(ToBeSquit.size());
		CB(1, ServerSquit.size());
		for (i = 0; i < tlist.size(); i++)
		{
		    if (ToBeSquit.find(tlist[i]) != ToBeSquit.end())
			ToBeSquit.erase(tlist[i]);
		    if (ServerSquit.find(tlist[i]) != ServerSquit.end())
		    {
			mstring *arg = NULL;
			if (Magick::instance().reactor().cancel_timer(ServerSquit[tlist[i]],
								      reinterpret_cast < const void ** > (arg)) && arg != NULL)
			    delete arg;
		    }
		    while (Magick::instance().Pause())
			ACE_OS::sleep(1);
		    ServerSquit[tlist[i]] = Magick::instance().reactor().schedule_timer(&squit, new mstring(tlist[i]),
								    ACE_Time_Value(Magick::instance().config.Squit_Protect()));
		}
		CE(1, ServerSquit.size());
		MCE(ToBeSquit.size());
	    }

	    NickServ::live_t::iterator iter;
	    vector < mstring > chunked;
	    vector < pair < mstring, unsigned long > > chunked2;

	    {
		RLOCK((lck_NickServ, lck_live));
		for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
		{
		    map_entry < Nick_Live_t > nlive(iter->second);
		    if (nlive->IsServices() && ListSize() == 0)
		    {
			chunked.push_back(nlive->Name().LowerCase());
		    }
		    else
		    {
			for (i = 0; i < tlist.size(); i++)
			{
			    if (nlive->Server().IsSameAs(tlist[i], true))
			    {
				nlive->SetSquit();
				chunked2.push_back(pair < mstring, unsigned long > (nlive->Name(), nlive->Numeric()));

				break;
			    }
			}
		    }
		}
	    }
	    // Sign off services if we have NO uplink
	    for (i = 0; i < chunked.size(); i++)
		QUIT(chunked[i], "SQUIT - " + svr->Name());
	    for (i = 0; i < chunked2.size(); i++)
	    {
		mMessage::CheckDependancies(mMessage::NickNoExists, chunked2[i].first);
		if (chunked2[i].second)
		    mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(chunked2[i].second));
	    }
	    for (i = 0; i < tlist.size(); i++)
		mMessage::CheckDependancies(mMessage::ServerNoExists, tlist[i]);
	    for (i = 0; i < nlist.size(); i++)
		mMessage::CheckDependancies(mMessage::ServerNoExists, "@" + proto.Numeric.ServerNumeric(nlist[i]));
	}
    }
    else if (msgtype == "STATS")
    {
	// :source STATS type :our.server
	//:temple.magick.tm 219 ChanServ o :End of /STATS report
	sraw("219 " + source + " " + IrcParam(params, 1) + " :End of /STATS report");
    }
    else if (msgtype == "SUMMON")
    {
	// :source SUMMON user our.server *
	sraw("445 " + source + " :SUMMON has been disabled");
    }
    else if (msgtype == "SVINFO")
    {
	// Bahamut compatable - ignore.
    }
    else if (msgtype == "SVSHOST")
    {
	// :source SVSHOST user newhost
	mstring nick = GetUser(IrcParam(params, 1));

	if (!nick.empty())
	    Magick::instance().nickserv.GetLive(nick)->AltHost(IrcParam(params, 2));
    }
    else if (msgtype == "SVSKILL")
    {
	// Same as KILL (but by services)
    }
    else if (msgtype == "SVSMOTD")
    {
    }
    else if (msgtype == "SVSMODE" || msgtype == "SVS2MODE")
    {
	// Handle just as mode, WITHOUT sanity
	if (!GetChannel(IrcParam(params, 1)).empty())
	{
	    mstring chan = GetChannel(IrcParam(params, 1));

	    if (!chan.empty())
	    {
		mstring mode = IrcParam(params, 2);

		unsigned int i, wc = IrcParamCount(params);
		mstring mode_param;
		for (i=3; i<=wc; i++)
		    mode_param += " " + IrcParam(params, i);

		if (proto.Numeric.User())
		{
		    unsigned int fwdarg = 1;
		    mstring new_param;

		    for (unsigned int i = 0; i < mode.length() && fwdarg <= mode_param.WordCount(" "); i++)
		    {
			if (proto.ChanModeArg().Contains(mode[i]))
			{
			    // We MUST convert numerics to users ... *sigh*
			    switch (mode[i])
			    {
			    case 'o':
			    case 'h':
			    case 'v':
				new_param += " " + GetUser("!" + mode_param.ExtractWord(fwdarg, " "));
				break;
			    default:
				new_param += " " + mode_param.ExtractWord(fwdarg, " ");
			    }
			    fwdarg++;
			}
		    }
		    mode_param = new_param;
		}
		Magick::instance().chanserv.GetLive(chan)->Mode(source, mode + " " + mode_param);
	    }
	    else
	    {
		LOG(LM_CRITICAL, "ERROR/REC_FORNONCHAN", ("SVSMODE", source, IrcParam(params, 1)));
	    }
	}
	else
	{
	    mstring nick = GetUser(IrcParam(params, 1));

	    if (!nick.empty())
	    {
		Magick::instance().nickserv.GetLive(nick)->Mode(IrcParam(params, 2));
	    }
	    else
	    {
		LOG(LM_CRITICAL, "ERROR/REC_FORNONUSER", ("SVSMODE", source, IrcParam(params, 1)));
	    }
	}
    }
    else if (msgtype == "SVSNICK")
    {
	// forcably changed nicks (handle like nick)
    }
    else if (msgtype == "SVSNLINE")
    {
    }
    else if (msgtype == "SVSNOOP")
    {
    }
    else if (msgtype == "SVSO")
    {
    }
    else if (msgtype == "SVSPART")
    {
    }
    else if (msgtype == "SWHOIS")
    {
    }
    else if (msgtype == "SZLINE")
    {
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_T(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_T", (source, msgtype, params));

    if (msgtype == "TSCTL")
    {
    }
    else if (msgtype == "TIME")
    {
	// :source TIME :our.server
	sraw("391 " + source + " :" + mDateTime::CurrentDateTime().DateTimeString());
    }
    else if (msgtype == "TKL" || msgtype == "TKLINE")
    {
    }
    else if (msgtype == "TOPIC")
    {
	// :user TOPIC #channel :topic
	// :user TOPIC #channel
	// -OR-
	// :server/user TOPIC #channel setter time :topic
	// :server/user TOPIC #channel setter
	// TIME is not standard (time is optional);

	mstring chan = GetChannel(IrcParam(params, 1));

	if (!chan.empty())
	{
	    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(chan);
	    if (params.Contains(" :"))
	    {			// Setting
		if (params.Before(" :").WordCount(" ") < 2)
		    clive->Topic(source, params.After(" :"), source, mDateTime::CurrentDateTime());
		else if (params.Before(" :").WordCount(" ") < 3)
		    clive->Topic(source, params.After(" :"), IrcParam(params, 2), mDateTime::CurrentDateTime());
		else
		    clive->Topic(source, params.After(" :"), IrcParam(params, 2),
				 static_cast < time_t > (atoul(IrcParam(params, 3))));
	    }
	    else
	    {			// Clearing
		if (params.WordCount(" ") < 2)
		    clive->Topic(source, "", source);
		else
		    clive->Topic(source, "", IrcParam(params, 2));
	    }
	}
	else
	{
	    LOG(LM_CRITICAL, "ERROR/REC_FORNONCHAN", ("TOPIC", source, IrcParam(params, 1)));
	}
    }
    else if (msgtype == "TRACE")
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
	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") :
					mstring("NOTICE")) + " " + source + " :" + tmp.c_str());
		return;
	    }
	}
	mstring out;

	mstring nick = GetUser(IrcParam(params, 1));

	if (!nick.empty() && Magick::instance().nickserv.GetLive(nick)->IsServices())
	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(nick);
	    if (nlive->HasMode("o"))
		out << "204 " << source << " Operator 10 ";
	    else
		out << "205 " << source << " User 1 ";
	    out << nlive->Name() << " [" << nlive->User() << "@" << nlive->Host() << "] :" <<
		   nlive->LastAction().SecondsSince();
	    sraw(out);
	}
	else
	{
	    unsigned long opers = 0, users = 0;

	    out << "206 " << source << "Server 50 " << ListSize() + 1 << "S " << Magick::instance().nickserv.LiveSize() << "C " <<
		   OurUplink() << "[0.0.0.0] AutoConn.!*@" << Magick::instance().startup.Server_Name() << " :0";
	    sraw(out);

	    NickServ::live_t::iterator iter;
	    {
		RLOCK2((lck_NickServ, lck_live));
		for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
		{
		    map_entry < Nick_Live_t > nlive(iter->second);
		    if (nlive->IsServices())
		    {
			out.erase();
			if (nlive->HasMode("o"))
			{
			    out << "204 " << source << " Operator 10 ";
			    opers++;
			}
			else
			{
			    out << "205 " << source << " User 1 ";
			    users++;
			}
			out << nlive->Name() << " [" << nlive->User() << "@" << nlive->Host() << "] :" <<
			       nlive->LastAction().SecondsSince();
			sraw(out);

		    }
		}
	    }
	    out.erase();
	    out << "209 " << source << " Class 50 :1";
	    sraw(out);
	    if (opers)
	    {
		out.erase();
		out << "209 " << source << " Class 10 :" << opers;
		sraw(out);
	    }
	    if (users)
	    {
		out.erase();
		out << "209 " << source << " Class 1 :" << users;
		sraw(out);
	    }
	}
	out.erase();
	out << "262 " << source << " " << Magick::instance().startup.Server_Name() << " :End of TRACE";
	sraw(out);
    }
    else if (msgtype == "TZLINE")
    {
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_U(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_U", (source, msgtype, params));

    if (msgtype == "UMODE2")
    {
	if (source.Contains("."))
	    return;

	Magick::instance().nickserv.GetLive(source)->Mode(IrcParam(params, 1));
    }
    else if (msgtype == "UNGLINE")
    {
	// We will ignore GLINES because they're not relivant to us.
	// we will not be glining our own clients ;P
    }
    else if (msgtype == "UNGZLINE")
    {
	// We will ignore GZLINES because they're not relivant to us.
	// we will not be glining our own clients ;P
    }
    else if (msgtype == "UNRQLINE")
    {
	// We will ignore RQLINES because they're not relivant to us.
	// we will not be qlining our own clients ;P
    }
    else if (msgtype == "UNSQLINE")
    {
	// We will ignore SQLINES because they're not relivant to us.
	// we will not be qlining our own clients ;P
    }
    else if (msgtype == "UNZLINE")
    {
	// We will ignore ZLINES because they're not relivant to us.
	// we will not be zlining our own clients ;P
    }
    else if (msgtype == "USER")
    {
	if (!source.Contains("."))
	    return;

	// NEW USER
	mstring newnick = IrcParam(params, 1);

	// DONT kill when we do SQUIT protection.
	map < mstring, set < mstring > >::iterator i;
	{
	    RLOCK((lck_Server, "ToBeSquit"));
	    for (i = ToBeSquit.begin(); i != ToBeSquit.end(); i++)
	    {
		WLOCK2((lck_Server, "ToBeSquit", i->first));
		set < mstring >::iterator k = i->second.find(newnick.LowerCase());
		if (k != i->second.end())
		    i->second.erase(k);
	    }
	}

	mstring server;
	mstring modes;

	switch (proto.Signon())
	{
	case 0000:		// USER nick user host server :realname
	    server = IrcParam(params, 4);
	    break;
	case 0001:		// USER nick time user host server :realname
	    server = IrcParam(params, 5);
	    break;
	case 1000:		// NICK nick hops time user host server :realname
	case 1001:		// NICK nick hops time user host server 1 :realname
	case 1002:		// NICK nick hops time user host server 0 real-host :realname
	case 1003:		// NICK nick hops time user real-host host server 0 :realname
	case 2000:		// NICK nick hops time mode user host server :realname
	case 2001:		// NICK nick hops time mode user host server 0 :realname
	case 2002:		// NICK nick hops time mode user host maskhost server 0 :realname
	case 2003:		// NICK nick hops time user host server 0 mode maskhost :realname
	case 3000:		// NICK nick hops time user host [mode] ipaddr numeric :realname
	    break;
	}
	if (proto.Numeric.Server() && !server.Contains("."))
	    server.prepend("@");
	server = GetServer(server);

	if (Magick::instance().nickserv.IsLiveAll(newnick))
	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(newnick);
	    COM(("Previous SQUIT checking if %s == %s and %s == %s", nlive->Squit().c_str(), server.c_str(),
		 nlive->SignonTime().DateTimeString().c_str(),
		 mDateTime(static_cast < time_t > (atoul(IrcParam(params, 3)))).DateTimeString().c_str()));
	    // IF the squit server = us, and the signon time matches
	    if (nlive->Squit().IsSameAs(server, true) &&
		nlive->SignonTime() == mDateTime(static_cast < time_t > (atoul(IrcParam(params, 3)))))
	    {
		nlive->ClearSquit(modes);
		mMessage::CheckDependancies(mMessage::NickExists, newnick);
		if (nlive->Numeric())
		    mMessage::CheckDependancies(mMessage::NickExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
		return;		// nice way to avoid repeating ones self :)
	    }
	    else
	    {
		nlive->Quit("SQUIT - " + nlive->Server());
		Magick::instance().nickserv.RemLive(newnick);
		mMessage::CheckDependancies(mMessage::NickNoExists, newnick);
		if (nlive->Numeric())
		    mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
	    }
	}

	switch (proto.Signon())
	{
	case 0000:		// USER nick user host server :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(IrcParam(params, 1), mDateTime::CurrentDateTime(), server,
				    IrcParam(params, 2), IrcParam(params, 3), params.After(" :")));
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 0001:		// USER nick time user host server :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(IrcParam(params, 1), static_cast < time_t > (atoul(IrcParam(params, 2))),
				    server, IrcParam(params, 3), IrcParam(params, 4), params.After(" :")));
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 1000:		// NICK nick hops time user host server :realname
	case 1001:		// NICK nick hops time user host server 1 :realname
	case 1002:		// NICK nick hops time user host server 0 real-host :realname
	case 1003:		// NICK nick hops time user real-host host server 0 :realname
	case 2000:		// NICK nick hops time mode user host server :realname
	case 2001:		// NICK nick hops time mode user host server 0 :realname
	case 2002:		// NICK nick hops time mode user host maskhost server 0 :realname
	case 2003:		// NICK nick hops time user host server 0 mode maskhost :realname
	case 3000:		// NICK nick hops time user host [mode] ipaddr numeric :realname
	    break;
	}

	if (Magick::instance().nickserv.IsLive(newnick))
	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(newnick);
	    if (nlive->Server().empty())
	    {
		mMessage::CheckDependancies(mMessage::NickExists, newnick);
		if (nlive->Numeric())
		    mMessage::CheckDependancies(mMessage::NickExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
		KILL(Magick::instance().nickserv.FirstName(), newnick, nlive->RealName());
		return;
	    }

	    {
		WLOCK2((lck_Server, "i_UserMax"));
		if (i_UserMax < Magick::instance().nickserv.LiveSize())
		{
		    MCB(i_UserMax);
		    i_UserMax = Magick::instance().nickserv.LiveSize();
		    MCE(i_UserMax);
		}
	    }

	    mMessage::CheckDependancies(mMessage::NickExists, newnick);
	    if (nlive->Numeric())
		mMessage::CheckDependancies(mMessage::NickExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
	    // HAS to be AFTER the nickname is added to map.
	    CommServ::list_t::iterator iter;
	    mstring setmode;

	    {
		RLOCK2((lck_CommServ, lck_list));
		for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		{
		    map_entry < Committee_t > comm(iter->second);
		    if (comm->IsOn(newnick))
		    {
			if (comm->Name() == Magick::instance().commserv.ALL_Name())
			    setmode += Magick::instance().commserv.ALL_SetMode();
			else if (comm->Name() == Magick::instance().commserv.REGD_Name())
			    setmode += Magick::instance().commserv.REGD_SetMode();
			else if (comm->Name() == Magick::instance().commserv.OPER_Name())
			    setmode += Magick::instance().commserv.OPER_SetMode();
			else if (comm->Name() == Magick::instance().commserv.ADMIN_Name())
			    setmode += Magick::instance().commserv.ADMIN_SetMode();
			else if (comm->Name() == Magick::instance().commserv.SOP_Name())
			    setmode += Magick::instance().commserv.SOP_SetMode();
			else if (comm->Name() == Magick::instance().commserv.SADMIN_Name())
			    setmode += Magick::instance().commserv.SADMIN_SetMode();

			MLOCK((lck_CommServ, lck_list, comm->Name(), "message"));
			for (comm->message = comm->MSG_begin(); comm->message != comm->MSG_end(); comm->message++)
			{
			    Magick::instance().servmsg.send(newnick,
							    "[" + IRC_Bold + comm->Name() + IRC_Off + "] " +
							    comm->message->Entry());
			}
		    }
		}
	    }
	    if (!setmode.empty())
	    {
		mstring setmode2;

		for (unsigned int j = 0; j < setmode.size(); j++)
		{
		    if (setmode[j] != '+' && setmode[j] != '-' && setmode[j] != ' ' && !nlive->HasMode(setmode[j]))
			setmode2 += setmode[j];
		}
		if (!setmode2.empty())
		    SVSMODE(Magick::instance().nickserv.FirstName(), newnick, "+" + setmode2);
	    }
	    if (Magick::instance().nickserv.IsStored(newnick))
	    {
		map_entry < Nick_Stored_t > nstored = Magick::instance().nickserv.GetStored(newnick);
		if (nstored->Forbidden())
		{
		    SEND(Magick::instance().nickserv.FirstName(), newnick, "ERR_SITUATION/FORBIDDEN",
			 (ToHumanTime(Magick::instance().nickserv.Ident(), newnick)));
		}
		else if (nstored->Protect() && !nstored->IsOnline())
		{
		    SEND(Magick::instance().nickserv.FirstName(), newnick, "ERR_SITUATION/PROTECTED",
			 (ToHumanTime(Magick::instance().nickserv.Ident(), newnick)));
		}
	    }
	}
    }
    else if (msgtype == "USERHOST")
    {
	if (!params.empty())
	{
	    sraw("461 " + source + " USERHOST :Not enough paramaters");
	}
	else
	{
	    mstring nick = GetUser(IrcParam(params, 1));

	    if (!nick.empty())
	    {
		map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(nick);
		if (!Magick::instance().nickserv.IsStored(nlive->Name()) ||
		    !Magick::instance().nickserv.GetStored(nlive->Name())->Private())
		{
		    sraw("302 " + source + " :" + nlive->Name() + "*=-" + nlive->User() + "@" +
						(nlive->AltHost().empty() ? nlive->Host() : nlive->AltHost()));
		}
		else
		{
		    sraw("302 " + source + " :" + nlive->Name() + "*=-" + nlive->User() + "@" +
			 Magick::instance().getMessage("VALS/ONLINE"));
		}
	    }
	    else
	    {
		sraw("401 " + source + " " + IrcParam(params, 1) + " :No such nickname/channel.");
	    }
	}
    }
    else if (msgtype == "USERS")
    {
	// :source USERS :our.server
	sraw("446 " + source + " :USERS has been disabled");
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_V(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_V", (source, msgtype, params));

    static_cast < void > (params);

    if (msgtype == "VERSION")
    {
	// :source VERSION :our.server
	//:temple.magick.tm 351 ChanServ dal4.4.17. temple.magick.tm :AiMnW

	mstring tmp = version_string();

	tmp << " " << sysinfo_type() << " " << sysinfo_rel() << ".";
	sraw("351 " + source + " " + PACKAGE + " " + Magick::instance().startup.Server_Name() + " :" + tmp);
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_W(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_W", (source, msgtype, params));

    if (msgtype == "WALLOPS")
    {
	// :source WALLOPS :text
	// useless chatter ... can be ignored.
    }
    else if (msgtype == "WHO")
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
	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
		    " " + source + " :" + tmp.c_str());
		return;
	    }
	}

    }
    else if (msgtype == "WHOIS")
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

	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
		    " " + source + " :" + tmp.c_str());
		sraw("318 " + source + " " + IrcParam(params, 1) + " :End of /WHOIS list.");
		return;
	    }
	    else
	    {
		mstring nick = GetUser(IrcParam(params, 1));

		if (!nick.empty())
		{
		    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(nick);
		    sraw("311 " + source + " " + nlive->Name() + " " + nlive->User() + " " + nlive->Host() + " * :" +
			 nlive->RealName());

		    if (nlive->IsRecognized())
		    {
			sraw("307 " + source + " " + nlive->Name() + " : is a registered nick");
		    }

		    set < mstring > chans = nlive->Channels();
		    set < mstring >::iterator iter;
		    mstring outline = "319 " + source + " " + nlive->Name() + " :";

		    for (iter = chans.begin(); iter != chans.end(); iter++)
		    {
			if (outline.size() + iter->size() > proto.MaxLine())
			{
			    sraw(outline);
			    outline = "319 " + source + " " + nlive->Name() + " :";
			}

			// Channel doesnt exist
			if (!Magick::instance().chanserv.IsLive(*iter))
			    continue;

			map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(*iter);
			// Channel +p or +s, and source not in chan
			if ((clive->HasMode("s") || clive->HasMode("p")) && !clive->IsIn(source))
			    continue;

			if (clive->IsOp(nlive->Name()))
			    outline += "@";
			else if (clive->IsHalfOp(nlive->Name()))
			    outline += "%";
			else if (clive->IsVoice(nlive->Name()))
			    outline += "+";
			outline += * iter + " ";
		    }
		    if (outline.After(" :").length() > 0)
			sraw(outline);

		    if (nlive->IsServices())
			sraw("312 " + source + " " + nlive->Name() + " " + Magick::instance().startup.Server_Name() + " :" +
			     Magick::instance().startup.Server_Desc());
		    else if (IsList(nlive->Server()))
			sraw("312 " + source + " " + nlive->Name() + " " + nlive->Server() + " :" +
			     GetList(nlive->Server())->Description());

		    if (!nlive->Away().empty())
			sraw("301 " + source + " " + nlive->Name() + " :" + nlive->Away());

		    if (nlive->HasMode("o"))
		    {
			if (nlive->HasMode("A"))
			    sraw("313 " + source + " " + nlive->Name() + " :is an IRC Operator (Server Administrator)");
			else if (nlive->HasMode("a"))
			    sraw("313 " + source + " " + nlive->Name() + " :is an IRC Operator (Services Operator)");
			else
			    sraw("313 " + source + " " + nlive->Name() + " :is an IRC Operator");
		    }

		    if (nlive->HasMode("h"))
			sraw("310 " + source + " " + nlive->Name() + " :looks very helpful.");

		    if (nlive->IsServices())
		    {
			mstring signon_idletime;

			signon_idletime << nlive->LastAction().SecondsSince() << " " << nlive->SignonTime().timetstring();
			sraw("317 " + source + " " + nlive->Name() + " " + signon_idletime + " :seconds idle, signon time");
		    }

		    sraw("318 " + source + " " + nlive->Name() + " :End of /WHOIS list.");
		}
		else
		{
		    sraw("401 " + source + " " + IrcParam(params, 1) + " :No such nickname/channel.");
		}
	    }
	}

    }
    else if (msgtype == "WHOWAS")
    {
	// Dont store 'previous user' info, so ignore.
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::parse_X(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_X", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    ETCB();
}

void Server::parse_Y(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_Y", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    ETCB();
}

void Server::parse_Z(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::parse_Z", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    if (msgtype == "ZLINE")
    {
	// We will ignore ZLINES because they're not relivant to us.
	// we will not be zlining our own clients ;P
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
    ETCB();
}

void Server::numeric_execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    BTCB();
    FT("Server::numeric_execute", (source, msgtype, params));

    static_cast < void > (source);

    int numeric = atoi(msgtype);

    // Numerics direct from RFC1459
    // MOST we can (and will) ignore.

    switch (numeric)
    {
    case 303:			// RPL_ISON
	{
	    bool wasburst = false;
	    {
		RLOCK((lck_IrcSvcHandler));
		if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->Burst())
		{
		    wasburst = true;
		    Magick::instance().ircsvchandler->EndBurst();
		    if (!proto.Burst().empty())
			Magick::instance().server.sraw(((proto.Tokens() && !proto.GetNonToken(proto.Burst()).empty()) ?
									   proto.GetNonToken(proto.Burst()) :
									   mstring(proto.Burst())));
		    LOG(LM_INFO, "EVENT/NETSYNCED", (fmstring("%.3f", Magick::instance().ircsvchandler->BurstTime())));
		}
	    }

	    Magick::instance().operserv.CloneList_check();

	    unsigned int i, wc = IrcParamCount(params);
	    mstring nicks;
	    for (i=1; i<=wc; i++)
		nicks += " " + IrcParam(params, i);

	    for (i = 1; i <= nicks.WordCount(" "); i++)
	    {
		// Remove clients from 'signon list' who are
		// already on the network.
		WLOCK((lck_Server, "WaitIsOn"));
		if (WaitIsOn.find(nicks.ExtractWord(i, " ").LowerCase()) != WaitIsOn.end())
		    WaitIsOn.erase(nicks.ExtractWord(i, " "));
	    }
	    if (WaitIsOn.size())
	    {
		set < mstring >::reverse_iterator k;
		RLOCK((lck_Server, "WaitIsOn"));
		for (k = WaitIsOn.rbegin(); k != WaitIsOn.rend(); k++)
		{
		    if (Magick::instance().operserv.IsName(*k) && !Magick::instance().nickserv.IsLive(*k))
		    {
			Magick::instance().operserv.signon(*k);
		    }
		    else if (Magick::instance().nickserv.IsName(*k) && !Magick::instance().nickserv.IsLive(*k))
		    {
			Magick::instance().nickserv.signon(*k);
		    }
		    else if (Magick::instance().chanserv.IsName(*k) && !Magick::instance().nickserv.IsLive(*k))
		    {
			Magick::instance().chanserv.signon(*k);

			if (Magick::instance().chanserv.FirstName() == * k)
			{
			    if (Magick::instance().chanserv.Hide())
				MODE(*k, "+i");
			    vector < mstring > joins;
			    ChanServ::stored_t::iterator iter;
			    map < mstring, mstring > modes;
			    map < mstring, triplet < mstring, mstring, mDateTime > > topics;

			    // Should be fact finding ONLY ...
			    {
				RLOCK2((lck_ChanServ, lck_stored));
				for (iter = Magick::instance().chanserv.StoredBegin();
				     iter != Magick::instance().chanserv.StoredEnd(); iter++)
				{
				    map_entry < Chan_Stored_t > cstored(iter->second);
				    mstring lname = cstored->Name().LowerCase();
				    map_entry < Chan_Live_t > clive;
				    if (Magick::instance().chanserv.IsLive(lname))
					clive = Magick::instance().chanserv.GetLive(lname);

				    // If its live and got JOIN on || not live and mlock +k or +i
				    if ((clive != NULL && cstored->Join()) ||
					(clive == NULL &&
					 (!cstored->Mlock_Key().empty() || cstored->Mlock_On().Contains("i"))))
				    {
					joins.push_back(lname);
					if (clive == NULL)
					{
					    modes[lname] = "+s";
					    if (cstored->Mlock_On().Contains("i"))
						modes[lname] += "i";
					    if (!cstored->Mlock_Key().empty())
						modes[lname] += "k " + cstored->Mlock_Key();
					}
				    }

				    if (clive != NULL && !cstored->Last_Topic().empty() && !cstored->Suspended())
				    {
					if ((cstored->Topiclock() && clive->Topic() != cstored->Last_Topic()) ||
					    (cstored->Keeptopic() && clive->Topic().empty()))
					{
					    topics[lname] =
						triplet < mstring, mstring, mDateTime > (cstored->Last_Topic_Setter(),
											 cstored->Last_Topic(),
											 cstored->Last_Topic_Set_Time());
					}
				    }
				}
			    }

			    vector < mstring >::iterator j;
			    mstring joinline;

			    for (j = joins.begin(); j != joins.end(); j++)
			    {
				if (joinline.length() + j->length() > proto.MaxLine())
				{
				    JOIN(Magick::instance().chanserv.FirstName(), joinline);
				    joinline.erase();
				}
				if (joinline.length())
				    joinline << ",";
				joinline << * j;
			    }
			    if (joinline.length())
			    {
				JOIN(Magick::instance().chanserv.FirstName(), joinline);
				joinline.erase();
			    }

			    map < mstring, mstring >::iterator m;
			    for (m = modes.begin(); m != modes.end(); m++)
			    {
				Magick::instance().chanserv.GetLive(m->first)->SendMode(m->second);
			    }

			    map < mstring, triplet < mstring, mstring, mDateTime > >::iterator t;;
			    for (t = topics.begin(); t != topics.end(); t++)
			    {
				Magick::instance().server.TOPIC(*k, t->second.first, t->first, t->second.second,
								t->second.third);
			    }
			}
		    }
		    else if (Magick::instance().memoserv.IsName(*k) && !Magick::instance().nickserv.IsLive(*k))
		    {
			Magick::instance().memoserv.signon(*k);
		    }
		    else if (Magick::instance().commserv.IsName(*k) && !Magick::instance().nickserv.IsLive(*k))
		    {
			Magick::instance().commserv.signon(*k);
		    }
		    else if (Magick::instance().servmsg.IsName(*k) && !Magick::instance().nickserv.IsLive(*k))
		    {
			Magick::instance().servmsg.signon(*k);
			if (Magick::instance().servmsg.FirstName() == * k)
			    Magick::instance().server.MODE(*k, "+o");
		    }

		    FlushMsgs(*k);
		}
	    }
	    {
		WLOCK((lck_Server, "WaitIsOn"));
		WaitIsOn.clear();
	    }
	    if (wasburst && !proto.EndBurst().empty())
		Magick::instance().server.sraw(((proto.Tokens() && !proto.GetNonToken(proto.EndBurst()).empty()) ?
								    proto.GetNonToken(proto.EndBurst()) :
								    mstring(proto.EndBurst())));
	}
	break;
    case 436:			// ERR_NICKCOLLISION
	// MUST handle.
	break;
    case 464:			// ERR_PASSWDMISMATCH
	// MUST handle (Stop connecting).
	LOG(LM_ERROR, "OTHER/WRONGPASS", (Magick::instance().CurrentServer()));
	Magick::instance().Disconnect();
	break;
    case 465:			// ERR_YOUREBANNEDCREEP
	// MUST handle (Stop connecting).
	LOG(LM_ERROR, "OTHER/WEAREBANNED", (Magick::instance().CurrentServer()));
	Magick::instance().Disconnect();
	break;
    default:
	// We dont bother with what we get back -- useless anyway.
//      LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
	break;
    }
    ETCB();
}

void Server::DumpB() const
{
    BTCB();
    MB(0, (i_UserMax, ServerSquit.size(), ToBeSquit.size(), i_OurUplink, ToBeSent.size()));
    ETCB();
}

void Server::DumpE() const
{
    BTCB();
    ME(0, (i_UserMax, ServerSquit.size(), ToBeSquit.size(), i_OurUplink, ToBeSent.size()));
    ETCB();
}
