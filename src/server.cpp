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
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
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

#ifndef MAGICK_HAS_EXCEPTIONS
static Server_t GLOB_Server_t;
#endif

void Protocol::SetTokens(const unsigned int type)
{
    FT("Protocol::SetTokens", (type));

    tokens.clear();
    switch (type)
    {
    case 1000:			// ircu >= 2.10.x
	tokens["AB"] = "ADMIN";
	tokens["A"] = "AWAY";
	tokens["CN"] = "CNOTICE";
	tokens["CO"] = "CONNECT";
	tokens["CP"] = "CPRIVMSG";
	tokens["C"] = "CREATE";
	tokens["DE"] = "DESTRUCT";
	tokens["DS"] = "DESYNCH";
	tokens["EB"] = "END_OF_BURST";
	tokens["EA"] = "EOB_ACK";
	tokens["Y"] = "ERROR";
	tokens["GL"] = "GLINE";
	tokens["F"] = "INFO";
	tokens["I"] = "INVITE";
	tokens["J"] = "JOIN";
	tokens["K"] = "KICK";
	tokens["D"] = "KILL";
	tokens["LI"] = "LINKS";
	tokens["LU"] = "LUSERS";
	tokens["M"] = "MODE";
	tokens["MO"] = "MOTD";
	tokens["E"] = "NAMES";
	tokens["N"] = "NICK";
	tokens["O"] = "NOTICE";
	tokens["L"] = "PART";
	tokens["G"] = "PING";
	tokens["Z"] = "PONG";
	tokens["P"] = "PRIVMSG";
	tokens["Q"] = "QUIT";
	tokens["RI"] = "RPING";
	tokens["RO"] = "RPONG";
	tokens["SE"] = "SETTIME";
	tokens["U"] = "SILENCE";
	tokens["SQ"] = "SQUIT";
	tokens["R"] = "STATS";
	tokens["TI"] = "TIME";
	tokens["TR"] = "TOPIC";
	tokens["V"] = "VERSION";
	tokens["WC"] = "WALLCHOPS";
	tokens["WA"] = "WALLOPS";
	tokens["H"] = "WHO";
	tokens["W"] = "WHOIS";
	tokens["X"] = "WHOWAS";
//      tokens["ACTION"] = CTCP_DELIM_CHAR + "ACTION";
//      tokens["VERSION"] = CTCP_DELIM_CHAR + "VERSION";
//      tokens["PING"] = CTCP_DELIM_CHAR + "PING";
//      tokens["PONG"] = CTCP_DELIM_CHAR + "PONG";
//      tokens["CLIENTINFO"] = CTCP_DELIM_CHAR + "CLIENTINFO";
//      tokens["USERINFO"] = CTCP_DELIM_CHAR + "USERINFO";
//      tokens["TIME"] = CTCP_DELIM_CHAR + "TIME";
//      tokens["ERRMSG"] = CTCP_DELIM_CHAR + "ERRMSG";
	break;

    case 0001:			// DAL >= 4.4.15
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
	break;

    case 0002:			// Relic >= 2.1
	SetTokens(0001);
	tokens.erase("1");
	tokens.erase("3");
	tokens.erase("=");
	tokens["^"] = "WALLOPS";
	tokens["LO"] = "LOCOPS";
	tokens.erase("c");
	tokens.erase("d");
	tokens["QL"] = "SQLINE";
	tokens["Uq"] = "UNSQLINE";
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

    case 0003:			// Relic 4.0
	SetTokens(0001);
	tokens["e"] = "MODNICK";
	break;

    case 0004:			// Unreal
	SetTokens(0001);
	tokens["t"] = "RULES";
	tokens["u"] = "MAP";
	tokens["v"] = "SVS2MODE";
	tokens["w"] = "DALINFO";
	tokens["x"] = "ADCHAT";
	tokens["y"] = "MKPASSWD";
	tokens["z"] = "ADDLINE";
	tokens["}"] = "GLINE";
	tokens["~"] = "SJOIN";
	tokens["AA"] = "SETHOST";
	tokens["AB"] = "TECHAT";
	tokens["AC"] = "NACHAT";
	tokens["AD"] = "SETIDENT";
	tokens["AE"] = "SETNAME";
	tokens["AF"] = "LAG";
	tokens["AG"] = "SDESC";
	tokens["AH"] = "PRIVMSG StatServ";
	tokens["AI"] = "KNOCK";
	tokens["AJ"] = "CREDITS";
	tokens["AK"] = "LICENSE";
	tokens["AL"] = "CHGHOST";
	tokens["AM"] = "RPING";
	tokens["AN"] = "RPONG";
	tokens["AO"] = "NETINFO";
	tokens["AP"] = "SENDUMODE";
	tokens["AQ"] = "ADDMOTD";
	tokens["AR"] = "ADDOMOTD";
	tokens["AS"] = "SVSMOTD";
	tokens["AU"] = "SMO";
	tokens["AV"] = "OPERMOTD";
	tokens["AW"] = "TSCTL";
	tokens["AX"] = "SVSJOIN";
	tokens["AY"] = "SVSPART";
	tokens["AZ"] = "CHGIDENT";
	tokens["BA"] = "SWHOIS";
	tokens["BB"] = "SVSO";
	tokens["BC"] = "SVSFLINE";
	tokens["BD"] = "TKL";
	tokens["BE"] = "VHOST";
	tokens["BF"] = "BOTMOTD";
	tokens["BG"] = "REMGLINE";
	tokens["BH"] = "HTM";
	tokens["|"] = "UMODE2";
	tokens["BI"] = "DCCDENY";
	tokens["BJ"] = "UNDCCDENY";
	tokens["BK"] = "CHGNAME";
	tokens["BL"] = "SHUN";
	tokens["BM"] = "CRYPTO";
	tokens["BN"] = "POST";
	tokens["BO"] = "PRIVMSG InfoServ";
	break;
    }
}

void Protocol::Numeric_t::SetBase64(unsigned int type)
{
    FT("Protocol::Numeric_t::SetBase64", (type));

    memset(base64_to_char, 0, sizeof(base64_to_char));
    memset(char_to_base64, 0, sizeof(char_to_base64));

    static const char ircu_b2c[] =
    {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
	    'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '[', ']'};

    static const char ircu_c2b[] =
    {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8,
	    9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 62, 0, 63, 0, 0, 0, 26, 27, 28, 29, 30, 31,
	    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    static const char unreal_b2c[] =
    {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
	    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
	    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '}'};

    static const char unreal_c2b[] =
    {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	    19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 0, 0, 0, 0, 0, 0, 36, 37, 38, 39, 40, 41,
	    42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 0, 63, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    switch (type)
    {
    case 0000:
	break;
    case 0001:
	memcpy(base64_to_char, ircu_b2c, sizeof(base64_to_char));
	memcpy(char_to_base64, ircu_c2b, sizeof(char_to_base64));
	break;
    case 0002:
	memcpy(base64_to_char, unreal_b2c, sizeof(base64_to_char));
	memcpy(char_to_base64, unreal_c2b, sizeof(char_to_base64));
	break;
    }
}

unsigned long Protocol::Numeric_t::str_to_base64(const mstring & in) const
{
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
}

mstring Protocol::Numeric_t::base64_to_str(unsigned long in) const
{
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
}

unsigned long Protocol::Numeric_t::ServerNumeric(const mstring & in) const
{
    FT("Protocol::Numeric_t::ServerNumeric", (in));
    unsigned long rv = str_to_base64(in.Right(Server()));

    RET(rv);
}

mstring Protocol::Numeric_t::ServerNumeric(unsigned long in) const
{
    FT("Protocol::Numeric_t::ServerNumeric", (in));
    mstring rv = base64_to_str(in).Right(Server());

    RET(rv);
}

unsigned long Protocol::Numeric_t::UserNumeric(const mstring & in) const
{
    FT("Protocol::Numeric_t::UserNumeric", (in));
    unsigned long rv = str_to_base64(in.Right(User() + (Combine() ? Server() : 0)));

    RET(rv);
}

mstring Protocol::Numeric_t::UserNumeric(unsigned long in) const
{
    FT("Protocol::Numeric_t::UserNumeric", (in));
    mstring rv = base64_to_str(in).Right(User() + (Combine() ? Server() : 0));

    RET(rv);
}

unsigned long Protocol::Numeric_t::ChannelNumeric(const mstring & in) const
{
    FT("Protocol::Numeric_t::ChannelNumeric", (in));
    unsigned long rv = str_to_base64(in.Right(Channel()));

    RET(rv);
}

mstring Protocol::Numeric_t::ChannelNumeric(unsigned long in) const
{
    FT("Protocol::Numeric_t::ChannelNumeric", (in));
    mstring rv = base64_to_str(in).Right(Channel());

    RET(rv);
}

Protocol::Protocol() : i_Number(0), i_NickLen(9), i_MaxLine(450), i_Globops(false), i_Helpops(false), i_Chatops(false),
i_Tokens(false), i_P12(false), i_TSora(false), i_SJoin(false), i_BigTopic(false), i_TopicJoin(false), i_ServerModes(false),
i_Akill(0), i_Signon(0), i_Modes(3), i_ChanModeArg("ovbkl"), i_Server("SERVER $1 $2 :$3")
{
    NFT("Protocol::Protocol");
    DumpB();
}

void Protocol::Set(const unsigned int in)
{
    FT("Protocol::Set", (in));

    // WE NEVER set 'i_Tokens', thats set with the PROTCTL line.

    DumpB();
    switch (in)
    {
    case 0:			// RFC
	i_Signon = 0000;
	i_Akill = 2000;
	SetTokens(0000);
	Numeric.SetBase64(0000);
	break;

    case 1:			// RFC with TS8
	i_Signon = 0001;
	i_Akill = 2000;
	SetTokens(0000);
	Numeric.SetBase64(0000);
	break;

    case 10:			// DAL < 4.4.15
	i_BigTopic = true;
	i_NickLen = 32;
	i_Signon = 1000;
	i_Globops = true;
	i_Akill = 1000;
	i_Modes = 4;
	SetTokens(0000);
	Numeric.SetBase64(0000);
	break;

    case 11:			// DAL >= 4.4.15
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_Helpops = true;
	i_BigTopic = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	Numeric.SetBase64(0000);
	break;

    case 12:			// Bahamut
	i_NickLen = 32;
	i_Signon = 2001;
	i_Globops = true;
	i_Chatops = true;
	i_BigTopic = true;
	i_Akill = 1001;
	i_Modes = 6;
	i_TSora = true;
	i_SJoin = true;
	i_Protoctl = "CAPAB NOQUIT TS3 SSJOIN BURST UNCONNECT";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	i_Burst = "BURST";
	i_EndBurst = "BURST 0";
	SetTokens(0001);
	Numeric.SetBase64(0000);
	break;

    case 20:			// UnderNet < 2.10.x
	i_Signon = 1000;
	i_Akill = 2000;
	SetTokens(0000);
	Numeric.SetBase64(0000);
	break;

    case 21:			// UnderNet >= 2.10.00
	i_Signon = 3000;
	i_Akill = 2001;
	Numeric.i_Server = 1;
	Numeric.i_User = 2;
	Numeric.i_Combine = true;
	Numeric.i_Field = 6;
	Numeric.i_Prefix = true;
	i_EndBurst = "END_OF_BURST";
	i_ServicesModes = "k";
	i_Tokens = true;	// Nothing tells us to turn it on ...
	i_ServerModes = true;
	i_TSora = true;
	SetTokens(1000);
	Numeric.SetBase64(0001);
	i_Server = "SERVER $1 $2 $6 $5 J10 $4 :$3";
	break;

    case 22:			// UnderNet >= 2.10.00 w/Extended
	i_Signon = 3000;
	i_Akill = 2001;
	Numeric.i_Server = 2;
	Numeric.i_User = 2;
	Numeric.i_Combine = true;
	Numeric.i_Field = 6;
	Numeric.i_Prefix = true;
	i_EndBurst = "END_OF_BURST";
	i_ServicesModes = "k";
	i_Tokens = true;	// Nothing tells us to turn it on ...
	i_ServerModes = true;
	i_TSora = true;
	SetTokens(1000);
	Numeric.SetBase64(0001);
	i_Server = "SERVER $1 $2 $6 $5 J10 $4 0 :$3";
	break;

    case 30:			// Hybrid 5/6
	i_NickLen = 9;
	i_Signon = 2000;
	i_TopicJoin = true;
	i_Akill = 3000;
	i_ChanModeArg = "ovbekld";
	i_TSora = true;
	i_Protoctl = "CAPAB QS EX CHW";
	SetTokens(0000);
	Numeric.SetBase64(0000);
	break;

    case 31:			// Hybrid 7
	i_NickLen = 9;
	i_Signon = 2000;
	i_TopicJoin = true;
	i_Akill = 3001;
	i_ChanModeArg = "ovbeklIh";
	i_TSora = true;
	i_Protoctl = "CAPAB QS EX CHW";
	SetTokens(0000);
	Numeric.SetBase64(0000);
	break;

    case 40:			// Elite
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_Helpops = true;
	i_BigTopic = true;
	i_Akill = 1000;
	i_ChanModeArg = "ovbehklqa";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	Numeric.SetBase64(0000);
	break;

    case 50:			// Relic 2.0 (dreamforge based)
	i_NickLen = 32;
	i_Globops = true;
	i_Helpops = true;
	i_BigTopic = true;
	i_Signon = 1001;
	i_Akill = 1000;
	i_Modes = 6;
	i_Server = "SERVER $1 $2 relic2.0 :$3";
	i_Protoctl = "PROTOCTL NOQUIT TOKEN SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	Numeric.SetBase64(0000);
	break;

    case 51:			// Relic 2.1 (dreamforge based)
	i_NickLen = 32;
	i_Globops = true;
	i_Helpops = true;
	i_BigTopic = true;
	i_P12 = true;
	i_SJoin = true;
	i_Signon = 1003;
	i_Akill = 2002;
	i_Modes = 6;
	i_ChanModeArg = "ovbekl";
	i_Server = "SERVER $1 $2 relic2.1 :$3";
	i_Protoctl = "PROTOCTL NOQUIT TOKEN SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0002);
	Numeric.SetBase64(0000);
	break;

    case 53:			// Relic 4.0 (bahamut based)
	i_NickLen = 32;
	i_Signon = 2002;
	i_Globops = true;
	i_BigTopic = true;
	i_Akill = 2002;
	i_Modes = 6;
	i_TSora = true;
	i_Protoctl = "CAPAB NOQUIT TS3 SSJOIN BURST UNCONNECT";
	i_SVSNICK = "MODNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	i_Burst = "BURST";
	i_EndBurst = "EOB";
	SetTokens(0003);
	Numeric.SetBase64(0000);
	break;

    case 54:			// Relic 5.0 (hybrid based)
	i_NickLen = 32;
	i_Signon = 2000;
	i_Akill = 2003;
	i_ChanModeArg = "ovbekld";
	i_TSora = true;
	i_Protoctl = "CAPAB QS EX";
	SetTokens(0000);
	Numeric.SetBase64(0000);
	break;

    case 60:			// Aurora
	i_NickLen = 32;
	i_Signon = 1002;
	i_Globops = true;
	i_Helpops = true;
	i_BigTopic = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	i_SVSHOST = "SVSHOST";
	SetTokens(0001);
	Numeric.SetBase64(0000);
	break;

    case 70:			// Unreal
	i_NickLen = 32;
	i_Signon = 2003;
	i_Globops = true;
	i_Helpops = true;
	i_BigTopic = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN NICKv2 SJOIN SJOIN2 UMODE2 VL SJ3 NS VHP";
	// Check serveropts in s_debug.c for what the letters are
	i_Server = "SERVER $1 $2 :U2303-CFhiIpnXS-$4 $3";
	Numeric.i_Trim = true;
	Numeric.i_Server = 6;
	Numeric.i_ServerNumber = true;
	Numeric.i_Field = 3;
	i_ChanModeArg = "ovbehklLfqa";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0004);
	Numeric.SetBase64(0002);
	break;

    case 80:			// UltimateIRCD
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_Helpops = true;
	i_BigTopic = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	Numeric.SetBase64(0000);
	break;

    default:
	DumpE();
	return;
    }
    i_Number = in;
    DumpE();
}

mstring Protocol::GetToken(const mstring & in) const
{
    FT("Protocol::GetToken", (in));
    map < mstring, mstring >::const_iterator iter = tokens.find(in);
    if (iter == tokens.end())
	RET("");
    RET(iter->second);
}

mstring Protocol::GetNonToken(const mstring & in) const
{
    FT("Protocol::GetNonToken", (in));
    mstring retval;

    map < mstring, mstring >::const_iterator iter;
    for (iter = tokens.begin(); iter != tokens.end(); iter++)
    {
	if (iter->second.IsSameAs(in))
	{
	    retval = iter->first;
	    break;
	}
    }
    RET(retval);
}

void Protocol::DumpB() const
{
    MB(0,
       (i_Number, i_NickLen, i_MaxLine, i_Globops, i_Helpops, i_Chatops, i_Tokens, i_P12, i_TSora, i_SJoin, i_BigTopic,
	i_TopicJoin, i_Akill, i_Signon, i_Modes, i_ChanModeArg));
    MB(16,
       (i_Server, i_Burst, i_EndBurst, i_Protoctl, i_SVSNICK, i_SVSMODE, i_SVSKILL, i_SVSNOOP, i_SQLINE, i_UNSQLINE, i_SVSHOST,
	tokens.size()));
}

void Protocol::DumpE() const
{
    ME(0,
       (i_Number, i_NickLen, i_MaxLine, i_Globops, i_Helpops, i_Chatops, i_Tokens, i_P12, i_TSora, i_SJoin, i_BigTopic,
	i_TopicJoin, i_Akill, i_Signon, i_Modes, i_ChanModeArg));
    ME(16,
       (i_Server, i_Burst, i_EndBurst, i_Protoctl, i_SVSNICK, i_SVSMODE, i_SVSKILL, i_SVSNOOP, i_SQLINE, i_UNSQLINE, i_SVSHOST,
	tokens.size()));
}

void Server_t::defaults()
{
    NFT("Server_t::defaults");
    ref_class::lockData(mVarArray("Server", "list", i_Name.LowerCase()));
    i_Hops = 0;
    i_Ping = 0;
    i_Lag = 0;
    i_Jupe = false;
}

Server_t::Server_t()
{
    NFT("Server_t::Server_t");
    defaults();
    DumpE();
}

Server_t::Server_t(const mstring & name, const mstring & description, const unsigned long numeric) : i_Name(name.LowerCase()),
i_AltName(name.LowerCase()), i_Numeric(numeric), i_Uplink(Magick::instance().startup.Server_Name().LowerCase()),
i_Description(description)
{
    FT("Server_t::Server_t", (name, description, numeric));
    defaults();
    i_Jupe = true;
    DumpE();
}

Server_t::Server_t(const mstring & name, const int hops, const mstring & description,
		   const unsigned long numeric) : i_Name(name.LowerCase()), i_AltName(name.LowerCase()), i_Numeric(numeric),
i_Uplink(Magick::instance().startup.Server_Name().LowerCase()), i_Description(description)
{
    FT("Server_t::Server_t", (name, hops, description, numeric));
    defaults();
    i_Hops = hops;
    Magick::instance().server.OurUplink(i_Name);
    DumpE();
}

Server_t::Server_t(const mstring & name, const mstring & uplink, const int hops, const mstring & description,
		   const unsigned long numeric) : i_Name(name.LowerCase()), i_AltName(name.LowerCase()), i_Numeric(numeric),
i_Uplink(uplink.LowerCase()), i_Description(description)
{
    FT("Server_t::Server_t", (name, uplink, hops, description, numeric));
    defaults();
    i_Hops = hops;
    DumpE();
}

Server_t &Server_t::operator=(const Server_t & in)
{
    FT("Server_t::operator=", ("(const Server_t &) in"));
    i_Name = in.i_Name;
    ref_class::lockData(mVarArray("Server", "list", i_Name.LowerCase()));
    i_AltName = in.i_AltName;
    i_Numeric = in.i_Numeric;
    i_Uplink = in.i_Uplink;
    i_Hops = in.i_Hops;
    i_Description = in.i_Description;
    i_Ping = in.i_Ping;
    i_Lag = in.i_Lag;
    i_Jupe = in.i_Jupe;
    NRET(Server_t &, *this);
}

mstring Server_t::AltName() const
{
    NFT("Server_t::AltName");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_AltName"));
    RET(i_AltName);
}

void Server_t::AltName(const mstring & in)
{
    FT("Server_t::AltName", (in));
    WLOCK(("Server", "list", i_Name.LowerCase(), "i_AltName"));
    MCB(i_AltName);
    i_AltName = in;
    MCE(i_AltName);
}

unsigned long Server_t::Numeric() const
{
    NFT("Server_t::Numeric");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Numeric"));
    RET(i_Numeric);
}

void Server_t::Numeric(const unsigned long in)
{
    FT("Server_t::Numeric", (in));
    WLOCK(("Server", "list", i_Name.LowerCase(), "i_Numeric"));
    MCB(i_Numeric);
    i_Numeric = in;
    MCE(i_Numeric);
}

mstring Server_t::Uplink() const
{
    NFT("Server_t::Uplink");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Uplink"));
    RET(i_Uplink);
}

int Server_t::Hops() const
{
    NFT("Server_t::Hops");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Hops"));
    RET(i_Hops);
}

mstring Server_t::Description() const
{
    NFT("Server_t::Description");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Description"));
    RET(i_Description);
}

void Server_t::Ping()
{
    NFT("Server_t::Ping");

    WLOCK(("Server", "list", i_Name.LowerCase(), "i_Ping"));
    if (!i_Ping)
    {
	Magick::instance().server.
	    sraw(((Magick::instance().server.proto.Tokens() &&
		   !Magick::instance().server.proto.GetNonToken("PING").empty()) ? Magick::instance().server.proto.
		  GetNonToken("PING") : mstring("PING")) + " " + Magick::instance().startup.Server_Name() + " :" + i_Name);
	MCB(i_Ping);
	i_Ping = ACE_OS::gettimeofday().msec();
	MCE(i_Ping);
    }
}

void Server_t::Pong()
{
    NFT("Server_t::Pong");
    WLOCK(("Server", "list", i_Name.LowerCase(), "i_Ping"));
    if (i_Ping)
    {
	WLOCK2(("Server", "list", i_Name.LowerCase(), "i_Lag"));
	MCB(i_Lag);
	CB(1, i_Ping);
	i_Lag = ACE_OS::gettimeofday().msec() - i_Ping;
	COM(("The lag time of %s is %3.3f seconds.", i_Name.c_str(), i_Lag / 1000.0));
	i_Ping = 0;
	CE(1, i_Ping);
	MCE(i_Lag);
    }
}

float Server_t::Lag() const
{
    NFT("Server_t::Lag");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Lag"));
    float retval = i_Lag / 1000.0;

    RET(retval);
}

bool Server_t::Jupe() const
{
    NFT("Server_t::Jupe");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Jupe"));
    RET(i_Jupe);
}

unsigned int Server_t::Users() const
{
    NFT("Server_t::Users");

    unsigned int count = 0;

    NickServ::live_t::iterator k;
    {
	RLOCK(("NickServ", "live"));
	for (k = Magick::instance().nickserv.LiveBegin(); k != Magick::instance().nickserv.LiveEnd(); k++)
	{
	    map_entry < Nick_Live_t > nlive(k->second);
	    if (nlive->Server().IsSameAs(i_Name, true))
		count++;
	}
    }
    RET(count);
}

unsigned int Server_t::Opers() const
{
    NFT("Server_t::Opers");

    unsigned int count = 0;

    NickServ::live_t::iterator k;
    {
	RLOCK(("NickServ", "live"));
	for (k = Magick::instance().nickserv.LiveBegin(); k != Magick::instance().nickserv.LiveEnd(); k++)
	{
	    map_entry < Nick_Live_t > nlive(k->second);
	    if (nlive->Server().IsSameAs(i_Name, true) && nlive->HasMode("o"))
		count++;
	}
    }
    RET(count);
}

vector < mstring > Server_t::Downlinks() const
{
    NFT("Server_t::Downlinks");
    vector < mstring > downlinks;
    Server::list_t::iterator iter;

    {
	RLOCK(("Server", "list"));
	for (iter = Magick::instance().server.ListBegin(); iter != Magick::instance().server.ListEnd(); iter++)
	{
	    map_entry < Server_t > server(iter->second);
	    if (!i_Name.empty() && server->Uplink().IsSameAs(i_Name, true))
		downlinks.push_back(iter->first);
	}
    }
    NRET(vector < mstring >, downlinks);
}

vector < mstring > Server_t::AllDownlinks() const
{
    NFT("Server_t::AllDownlinks");
    vector < mstring > downlinks, uplinks, uplinks2;
    Server::list_t::iterator iter;
    bool found = false;

    {
	RLOCK(("Server", "list"));
	for (iter = Magick::instance().server.ListBegin(); iter != Magick::instance().server.ListEnd(); iter++)
	{
	    map_entry < Server_t > server(iter->second);
	    if (server->Uplink().IsSameAs(i_Name, true))
	    {
		downlinks.push_back(iter->first);
		uplinks.push_back(iter->first);
		found = true;
	    }
	}
    }

    while (found)
    {
	found = false;
	for (unsigned int i = 0; i < uplinks.size(); i++)
	{
	    RLOCK(("Server", "list"));
	    for (iter = Magick::instance().server.ListBegin(); iter != Magick::instance().server.ListEnd(); iter++)
	    {
		map_entry < Server_t > server(iter->second);
		if (server->Uplink().IsSameAs(uplinks[i], true))
		{
		    downlinks.push_back(iter->first);
		    uplinks2.push_back(iter->first);
		    found = true;
		}
	    }
	}
	uplinks.clear();
	uplinks = uplinks2;
	uplinks2.clear();
    }

    NRET(vector < mstring >, downlinks);
}

size_t Server_t::Usage() const
{
    size_t retval = 0;

    WLOCK(("Server", "list", i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += i_AltName.capacity();
    retval += i_Uplink.capacity();
    retval += sizeof(i_Hops);
    retval += i_Description.capacity();
    retval += sizeof(i_Ping);
    retval += sizeof(i_Lag);
    retval += sizeof(i_Jupe);

    return retval;
}

mstring Protocol::Numeric_t::FindServerNumeric(unsigned long n) const
{
    FT("Protocol::Numeric_t::FindServerNumeric", (n));
    mstring retval;

    Server::list_t::const_iterator iter;

    RLOCK(("Server", "list"));
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
}

mstring Protocol::Numeric_t::FindUserNumeric(unsigned long n) const
{
    FT("Protocol::Numeric_t::FindUserNumeric", (n));

    mstring retval;

    NickServ::live_t::const_iterator iter;

    RLOCK(("NickServ", "live"));
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
}

mstring Protocol::Numeric_t::FindChannelNumeric(unsigned long n) const
{
    FT("Protocol::Numeric_t::FindChannelNumeric", (n));

    mstring retval;

    ChanServ::live_t::const_iterator iter;

    RLOCK(("ChanServ", "live"));
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
}

unsigned long Protocol::Numeric_t::FindServerNumeric() const
{
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
}

unsigned long Protocol::Numeric_t::FindUserNumeric() const
{
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
}

unsigned long Protocol::Numeric_t::FindChannelNumeric() const
{
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
}

mstring Protocol::Numeric_t::ServerLineNumeric(unsigned long n) const
{
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
}

unsigned long Protocol::Numeric_t::ServerLineNumeric(const mstring & n) const
{
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
}

void Server_t::DumpB() const
{
    MB(0, (i_Name, i_AltName, i_Numeric, i_Uplink, i_Hops, i_Description, i_Ping, i_Lag, i_Jupe));
}

void Server_t::DumpE() const
{
    ME(0, (i_Name, i_AltName, i_Numeric, i_Uplink, i_Hops, i_Description, i_Ping, i_Lag, i_Jupe));
}

void Server::raw(const mstring & text) const
{
    FT("Server::raw", (text));
    Magick::instance().send(text);
}

void Server::sraw(const mstring & text) const
{
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
}

void Server::nraw(const mstring & nick, const mstring & text) const
{
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
}

void Server::SignOnAll()
{
    NFT("Server::SignOnAll");

    mstring doison;
    unsigned int i;

    WLOCK(("Server", "WaitIsOn"));
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
}

void Server::SignOffAll(const mstring & reason)
{
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
}

Server::Server()
{
    NFT("Server::Server");
    messages = true;
    WLOCK(("Server", "i_UserMax"));
    i_UserMax = 0;
    DumpE();
}

size_t Server::UserMax() const
{
    NFT("Server::UserMax");
    RLOCK(("Server", "i_UserMax"));
    RET(i_UserMax);
}

void Server::OurUplink(const mstring & server)
{
    FT("Server::OurUplink", (server));
    WLOCK(("Server", "i_OurUplink"));
    MCB(i_OurUplink);
    i_OurUplink = server;
    MCE(i_OurUplink);
}

mstring Server::OurUplink() const
{
    NFT("Server::OurUplink");
    RLOCK(("Server", "i_OurUplink"));
    RET(i_OurUplink);
}

void Server::FlushMsgs(const mstring & nick)
{
    FT("Server::FlushMsgs", (nick));

    map < mstring, list < triplet < send_type, mDateTime, triplet < mstring, mstring, mstring > > > >::iterator i;
    list < triplet < send_type, mDateTime, triplet < mstring, mstring, mstring > > >::iterator j;

    // Dont report this, thats the point of the queue...
    if (!Magick::instance().nickserv.IsLive(nick))
	return;

    RLOCK(("Server", "ToBeSent", nick.LowerCase()));
    MCB(ToBeSent.size());
    if ((i = ToBeSent.find(nick.LowerCase())) != ToBeSent.end())
    {
	WLOCK(("Server", "ToBeSent", i->first));
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
}

#ifdef MAGICK_HAS_EXCEPTIONS
void Server::AddList(Server_t * in) throw (E_Server_List)
#else
void Server::AddList(Server_t * in)
#endif
{
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

    RLOCK(("Server", "list"));
    map_entry < Server_t > old_entry(i_list, in->Name().LowerCase());
    if (old_entry.entry() != NULL)
    {
	old_entry->setDelete();
	i_list.erase(in->Name().LowerCase());
    }
    WLOCK(("Server", "list"));
    i_list[in->Name().LowerCase()] = in;
}

#ifdef MAGICK_HAS_EXCEPTIONS
map_entry < Server_t > Server::GetList(const mstring & in) const throw (E_Server_List)
#else
map_entry < Server_t > Server::GetList(const mstring & in) const
#endif
{
    FT("Server::GetList", (in));

    RLOCK(("Server", "list", in.LowerCase()));
    Server::list_t::const_iterator iter = i_list.find(in.LowerCase());
    if (iter == i_list.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Server_List(E_Server_List::W_Get, E_Server_List::T_NotFound, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Server", "List", "Get", "NotFound", in));
	NRET(Server_t &, GLOB_Server_t);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Server_List(E_Server_List::W_Get, E_Server_List::T_Invalid, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Server", "List", "Get", "Invalid", in));
	NRET(Server_t &, GLOB_Server_t);
#endif
    }
    if (iter->second->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_Server_List(E_Server_List::W_Get, E_Server_List::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Server", "List", "Get", "Blank", in));
	NRET(Server_t &, GLOB_Server_t);
#endif
    }

    NRET(map_entry < Server_t >, map_entry < Server_t > (iter->second));
}

#ifdef MAGICK_HAS_EXCEPTIONS
void Server::RemList(const mstring & in, bool downlinks) throw (E_Server_List)
#else
void Server::RemList(const mstring & in, bool downlinks)
#endif
{
    FT("Server::RemList", (in, downlinks));

    RLOCK(("Server", "list"));
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

    WLOCK(("Server", "list"));
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
}

bool Server::IsList(const mstring & server) const
{
    FT("Server::IsList", (server));
    RLOCK(("Server", "list"));
    bool retval = (i_list.find(server.LowerCase()) != i_list.end());

    RET(retval);
}

mstring Server::GetServer(const mstring & server) const
{
    FT("Server::GetServer", (server));
    mstring retval;

    if (proto.Numeric.Server() && server[0u] == '@')
	retval = proto.Numeric.FindServerNumeric(proto.Numeric.ServerNumeric(server.After("@")));
    else if (IsList(server) || server.IsSameAs(Magick::instance().startup.Server_Name(), true))
	retval = server;
    RET(retval);
}

mstring Server::GetUser(const mstring & user) const
{
    FT("Server::GetUser", (user));
    mstring retval;

    if (proto.Numeric.User() && user[0u] == '!')
	retval = proto.Numeric.FindUserNumeric(proto.Numeric.UserNumeric(user.After("!")));
    else if (Magick::instance().nickserv.IsLive(user))
	retval = user;
    RET(retval);
}

mstring Server::GetChannel(const mstring & channel) const
{
    FT("Server::GetChannel", (channel));
    mstring retval;

    if (proto.Numeric.Channel() && !IsChan(channel))
	retval = proto.Numeric.FindUserNumeric(proto.Numeric.ChannelNumeric(channel));
    else if (Magick::instance().chanserv.IsLive(channel))
	retval = channel;
    RET(retval);
}

bool Server::IsSquit(const mstring & server) const
{
    FT("Server::IsSquit", (server));
    RLOCK(("Server", "ServerSquit"));
    bool retval = (ServerSquit.find(server.LowerCase()) != ServerSquit.end());

    RET(retval);
}

void Server::Jupe(const mstring & server, const mstring & reason)
{
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

    Magick::instance().server.
	raw(parseMessage
	    (Magick::instance().server.proto.Server(),
	     mVarArray(server.LowerCase(), 2, "JUPED (" + reason + ")", proto.Numeric.ServerLineNumeric(numeric), time(NULL),
		       Magick::instance().StartTime().timetstring())));

    map_entry < Server_t > jupe(new Server_t(server.LowerCase(), "JUPED (" + reason + ")", numeric));
    Magick::instance().server.AddList(jupe);
}

void Server::AKILL(const mstring & host, const mstring & reason, const unsigned long exptime, const mstring & killer)
{
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
	line << " " << host.After("@") << " " << host.
	    Before("@") << " " << exptime << " " << ((!killer.empty()) ? killer : Magick::instance().operserv.
						     FirstName()) << " " << static_cast < time_t >
	    (mDateTime::CurrentDateTime()) << " :" << reason;
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
	    line << " " << os->Name() << " " << os->User() << " " << os->Host() << " " << Magick::instance().startup.
		Server_Name() << " " << host.Before("@") << " " << host.After("@") << " :" << reason;
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
	RLOCK(("NickServ", "live"));
	for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
	{
	    map_entry < Nick_Live_t > nlive(iter->second);
	    if (nlive->Mask(Nick_Live_t::U_P_H).After("!").Matches(host, true))
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
	if (proto.Akill() < 2000)
	    Magick::instance().server.KILL(Magick::instance().operserv.FirstName(), killusers[j].first, reason);
	else
	{
	    mMessage::CheckDependancies(mMessage::NickNoExists, killusers[j].first);
	    if (killusers[j].second)
		mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(killusers[j].second));
	}
    }
}

void Server::ANONKILL(const mstring & nick, const mstring & dest, const mstring & reason)
{
    FT("Server::ANONKILL", (nick, dest, reason));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
}

void Server::AWAY(const mstring & nick, const mstring & reason)
{
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
}

void Server::GLOBOPS(const mstring & nick, const mstring & message)
{
    FT("Server::GLOBOPS", (nick, message));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
}

void Server::HELPOPS(const mstring & nick, const mstring & message)
{
    FT("Server::HELPOPS", (nick, message));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
		  !proto.GetNonToken("HELPOPS").empty()) ? proto.GetNonToken("HELPOPS") : mstring("HELPOPS")) + " :";
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
}

void Server::CHATOPS(const mstring & nick, const mstring & message)
{
    FT("Server::CHATOPS", (nick, message));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
}

void Server::INVITE(const mstring & nick, const mstring & dest, const mstring & channel)
{
    FT("Server::INVITE", (nick, dest, channel));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
}

void Server::JOIN(const mstring & nick, const mstring & channel)
{
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
	    out << ((proto.Tokens() && !proto.GetNonToken("SJOIN").empty()) ? proto.
		    GetNonToken("SJOIN") : mstring("SJOIN")) << " " << mDateTime::CurrentDateTime().timetstring() << " ";

	    for (ci = channels.begin(); ci != channels.end(); ci++)
		nraw(nick, out + *ci);
	}
	else
	{
	    out << ((proto.Tokens() && !proto.GetNonToken("JOIN").empty()) ? proto.
		    GetNonToken("JOIN") : mstring("JOIN")) << " :";

	    bool firstchan = true;

	    for (ci = channels.begin(); ci != channels.end(); ci++)
	    {
		if (out.length() + ci->length() + 1 > proto.MaxLine())
		{
		    nraw(nick, out);
		    out.erase();
		    out << ((proto.Tokens() && !proto.GetNonToken("JOIN").empty()) ? proto.
			    GetNonToken("JOIN") : mstring("JOIN")) << " :";
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
	for (ci = channels.begin(); ci != channels.end(); ci++)
	    nlive->Join(*ci);
    }
}

void Server::KICK(const mstring & nick, const mstring & dest, const mstring & channel, const mstring & reason)
{
    FT("Server::KICK", (nick, dest, channel, reason));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
}

void Server::KILL(const mstring & nick, const mstring & dest, const mstring & reason)
{
    FT("Server::KILL", (nick, dest, reason));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
}

void Server::MODE(const mstring & nick, const mstring & mode)
{
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
}

void Server::MODE(const mstring & nick, const mstring & channel, const mstring & mode)
{
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
}

void Server::NICK(const mstring & nick, const mstring & user, const mstring & host, const mstring & i_server,
		  const mstring & name)
{
    FT("Server::NICK", (nick, user, host, i_server, name));

    if (Magick::instance().nickserv.IsLive(nick))
    {
	LOG(LM_WARNING, "ERROR/REQ_TOUSER", ("NICK", i_server, nick));
    }
    else
    {
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
	if (proto.P12() || (proto.Signon() >= 2000 && proto.Signon() < 4000))
	    tmp->Mode(Magick::instance().startup.Setmode() + proto.ServicesModes());
	if (proto.Numeric.User())
	    tmp->Numeric(proto.Numeric.FindUserNumeric());
	Magick::instance().nickserv.AddLive(tmp);
	{
	    WLOCK2(("Server", "i_UserMax"));
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
	    out << " " << nick << mDateTime::CurrentDateTime().
		timetstring() << " " << user << " " << host << " " << server << " :" << name;
	    break;
	case 1000:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().
		timetstring() << " " << user << " " << host << " " << server;
	    if (proto.P12())
		out << " +" << Magick::instance().startup.Setmode() << proto.ServicesModes();
	    out << " :" << name;
	    break;
	case 1001:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().
		timetstring() << " " << user << " " << host << " " << server << " 1";
	    if (proto.P12())
		out << " +" << Magick::instance().startup.Setmode() << proto.ServicesModes();
	    out << " :" << name;
	    break;
	case 1002:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().
		timetstring() << " " << user << " " << host << " " << server << " 1 " << host;
	    if (proto.P12())
		out << " +" << Magick::instance().startup.Setmode() << proto.ServicesModes();
	    out << " :" << name;
	    break;
	case 1003:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().
		timetstring() << " " << user << " " << host << " " << host << " " << server << " 1";
	    if (proto.P12())
		out << " +" << Magick::instance().startup.Setmode() << proto.ServicesModes();
	    out << " :" << name;
	    break;
	case 2000:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " +" << Magick::instance().startup.
		Setmode() << proto.ServicesModes() << " " << user << " " << host << " " << server << " :" << name;
	    break;
	case 2001:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " +" << Magick::instance().startup.
		Setmode() << proto.ServicesModes() << " " << user << " " << host << " " << server << " 1 :" << name;
	    break;
	case 2002:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " +" << Magick::instance().startup.
		Setmode() << proto.
		ServicesModes() << " " << user << " " << host << " " << host << " " << server << " 1 :" << name;
	    break;
	case 2003:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().
		timetstring() << " " << user << " " << host << " " << server << " 1 +" << Magick::instance().startup.
		Setmode() << proto.ServicesModes() << " " << host << " :" << name;
	    break;
	case 3000:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		out << proto.GetNonToken(token);
	    else
		out << token;
	    out << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() << " " << user << " " << host << " ";
	    if (!Magick::instance().startup.Setmode().empty() || !proto.ServicesModes().empty())
		out << "+" << Magick::instance().startup.Setmode() << proto.ServicesModes() << " ";

	    unsigned long ournumeric = Magick::instance().startup.Server(Magick::instance().CurrentServer()).second.third;

	    // B]AAAB == 127.0.0.1 for ipaddress.
	    out << "B]AAAB " << proto.Numeric.ServerNumeric(ournumeric) << " " << proto.Numeric.UserNumeric(tmp->
													    Numeric()) << " :"
		<< name;
	    break;
	}
	// Sign ourselves in ...

	sraw(out);
	mMessage::CheckDependancies(mMessage::NickExists, nick);
	if (tmp->Numeric())
	    mMessage::CheckDependancies(mMessage::NickExists, "!" + proto.Numeric.UserNumeric(tmp->Numeric()));
    }
}

void Server::NICK(const mstring & oldnick, const mstring & newnick)
{
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
}

void Server::NOTICE(const mstring & nick, const mstring & dest, const mstring & text)
{
    FT("Server::NOTICE", (nick, dest, text));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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

	if (IsChan(dest))
	{
	    unsigned long numeric = Magick::instance().chanserv.GetLive(dest)->Numeric();

	    if (numeric && proto.Numeric.Channel())
		line << proto.Numeric.ChannelNumeric(numeric);
	    else
		line << dest;
	}
	else
	{
	    unsigned long numeric = Magick::instance().nickserv.GetLive(dest)->Numeric();

	    if (numeric && proto.Numeric.User())
		line << proto.Numeric.UserNumeric(numeric);
	    else
		line << dest;
	}
	line << " :";
	for (unsigned int i = 1; i <= text.WordCount("\n\r"); i++)
	    nraw(nick, line + text.ExtractWord(i, "\n\r"));
    }
}

void Server::PART(const mstring & nick, const mstring & channel, const mstring & reason)
{
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
}

void Server::PRIVMSG(const mstring & nick, const mstring & dest, const mstring & text)
{
    FT("Server::PRIVMSG", (nick, dest, text));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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

	if (IsChan(dest))
	{
	    unsigned long numeric = Magick::instance().chanserv.GetLive(dest)->Numeric();

	    if (numeric && proto.Numeric.Channel())
		line << proto.Numeric.ChannelNumeric(numeric);
	    else
		line << dest;
	}
	else
	{
	    unsigned long numeric = Magick::instance().nickserv.GetLive(dest)->Numeric();

	    if (numeric && proto.Numeric.User())
		line << proto.Numeric.UserNumeric(numeric);
	    else
		line << dest;
	}

	line << " :";
	for (unsigned int i = 1; i <= text.WordCount("\n\r"); i++)
	    nraw(nick, line + text.ExtractWord(i, "\n\r"));
    }
}

void Server::SQLINE(const mstring & nick, const mstring & target, const mstring & reason)
{
    FT("Server::SQLINE", (nick, target, reason));

    if (proto.SQLINE().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
}

void Server::QUIT(const mstring & nick, const mstring & reason)
{
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
	Magick::instance().nickserv.RemLive(nick);
	nraw(nick,
	     ((proto.Tokens() &&
	       !proto.GetNonToken("QUIT").empty()) ? proto.GetNonToken("QUIT") : mstring("QUIT")) + " :" + reason);
	mMessage::CheckDependancies(mMessage::NickNoExists, nick);
	if (numeric)
	    mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(numeric));
    }
}

void Server::RAKILL(const mstring & host)
{
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
}

void Server::SVSHOST(const mstring & mynick, const mstring & nick, const mstring & newhost)
{
    FT("Server::SVSHOST", (mynick, nick, newhost));

    if (proto.SVSHOST().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
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
}

void Server::SVSKILL(const mstring & mynick, const mstring & nick, const mstring & reason)
{
    FT("Server::SVSKILL", (mynick, nick, reason));

    if (proto.SVSKILL().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
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
	output << " " << nick << " " << reason;
	nraw(mynick, output);
	mMessage::CheckDependancies(mMessage::NickNoExists, nick);
	if (numeric)
	    mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(numeric));
    }
}

void Server::SVSNICK(const mstring & mynick, const mstring & nick, const mstring & newnick)
{
    FT("Server::SVSNICK", (mynick, nick, newnick));

    if (proto.SVSNICK().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
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
}

void Server::SVSNOOP(const mstring & nick, const mstring & server, const bool onoff)
{
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
}

void Server::SVSMODE(const mstring & mynick, const mstring & nick, const mstring & mode)
{
    FT("Server::SVSMODE", (mynick, nick, mode));

    if (proto.SVSMODE().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
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
}

void Server::TOPIC(const mstring & nick, const mstring & setter, const mstring & channel, const mstring & topic,
		   const mDateTime & settime)
{
    FT("Server::TOPIC", (nick, setter, channel, topic, settime));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
		out << " " << settime.timetstring();
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
}

void Server::UNSQLINE(const mstring & nick, const mstring & target)
{
    FT("Server::UNSQLINE", (nick, target));

    if (proto.UNSQLINE().empty())
	return;

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
}

void Server::WALLOPS(const mstring & nick, const mstring & message)
{
    FT("Server::WALLOPS", (nick, message));

    if (!Magick::instance().nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
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
}

void Server::KillUnknownUser(const mstring & user) const
{
    FT("Server::KillUnknownUser", (user));
    sraw(((proto.Tokens() &&
	   !proto.GetNonToken("KILL").empty()) ? proto.GetNonToken("KILL") : mstring("KILL")) + " " + user + " :" +
	 Magick::instance().startup.Server_Name() + " (" + user + "(?) <- " + Magick::instance().CurrentServer() + ")");
    LOG(LM_ERROR, "OTHER/KILL_UNKNOWN", (user));
}

void Server::execute(mstring & source, const mstring & msgtype, const mstring & params)
{
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
}

void Server::parse_A(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_A", (source, msgtype, params));

    if (msgtype == "ADCHAT")
    {
	// Useless admin chatter.
    }
    else if (msgtype == "ADMIN")
    {
	{
	    RLOCK(("IrcSvcHandler"));
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
		RLOCK2(("CommServ", "list"));
		for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		{
		    map_entry < Committee_t > comm(iter->second);
		    if (comm->IsOn(source))
		    {
			MLOCK(("CommServ", "list", iter->first, "message"));
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
	    Magick::instance().nickserv.GetLive(source)->Away(params.After(":"));
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_B(mstring & source, const mstring & msgtype, const mstring & params)
{
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
		clive->Creation_Time(static_cast < time_t > (atoul(params.ExtractWord(2, ": "))));
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
}

void Server::parse_C(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_C", (source, msgtype, params));

    if (msgtype == "CAPAB")
    {
	// Bahamut version of the PROTOCTL line
	if ((" " + params + " ").Contains(" TOKEN "))
	    proto.Tokens(true);
	if ((" " + params + " ").Contains(" SSJOIN ") || (" " + params + " ").Contains(" SJOIN "))
	    proto.SJoin(true);
    }
    else if (msgtype == "CHATOPS")
    {
    }
    else if (msgtype == "CONNECT")
    {
	// :soul.darker.net 481 ChanServ :Permission Denied- You do not have the correct IRC operator privileges
	// :source CONNECT some.server port :our.server
	mstring server = GetServer(params.ExtractWord(1, ": "));

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
		 " :Connect: Host " + params.ExtractWord(1, ": ") + " not listed in irc.conf");
	}
    }
    else if (msgtype == "CREATE")
    {
	if (source.Contains("."))
	    return;

	// :source JOIN :#channel
	mstring chan(params.ExtractWord(1, ": "));

	Magick::instance().nickserv.GetLive(source)->Join(chan);
	Magick::instance().chanserv.GetLive(chan)->Mode(source, "+o " + source);
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_D(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_D", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
}

void Server::parse_E(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_E", (source, msgtype, params));

    static_cast < void > (source);

    if (msgtype == "END_OF_BURST" || msgtype == "EOB")
    {
	// Tis only nice, afterall ...
	Magick::instance().server.
	    sraw(((proto.Tokens() &&
		   !proto.GetNonToken("EOB_ACK").empty()) ? proto.GetNonToken("EOB_ACK") : mstring("EOB_ACK")));
    }
    else if (msgtype == "EOB_ACK")
    {
	// Thank you, drive through ...
    }
    else if (msgtype == "ERROR")
    {
	// ERROR :This is my error
	LOG(LM_NOTICE, "OTHER/SERVER_MSG", (msgtype, params.After(":")));
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_F(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_F", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
}

void Server::parse_G(mstring & source, const mstring & msgtype, const mstring & params)
{
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
	LOG(LM_NOTICE, "OTHER/SERVER_MSG", (msgtype, params.After(":")));
    }
    else if (msgtype == "GOPER")
    {
	// useless chatter ... can be ignored.
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_H(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_H", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    if (msgtype == "HELP")
    {
	// ignore ...
    }
    else if (msgtype == "HELPOP")
    {
	// ignore ...
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_I(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_I", (source, msgtype, params));

    if (msgtype == "INFO")
    {
	// :source INFO :server/nick
	{
	    RLOCK(("IrcSvcHandler"));
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

	for (unsigned int i = 1; i <= params.WordCount(": "); i++)
	{
	    if (isonstr.length() > proto.MaxLine())
	    {
		sraw("303 " + source + " :" + isonstr);
		isonstr.erase();
	    }
	    if (Magick::instance().nickserv.IsLive(params.ExtractWord(i, ": ")))
		isonstr += params.ExtractWord(i, ": ") + " ";
	}
	if (!isonstr.empty())
	    sraw("303 " + source + " :" + isonstr);
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_J(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_J", (source, msgtype, params));

    if (msgtype == "JOIN")
    {
	if (source.Contains("."))
	    return;

	// :source JOIN :#channel
	for (unsigned int i = 1; i <= params.WordCount(":, "); i++)
	{
	    mstring chan(params.ExtractWord(i, ":, "));

	    Magick::instance().nickserv.GetLive(source)->Join(chan);
	}
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_K(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_K", (source, msgtype, params));

    if (msgtype == "KICK")
    {
	// :source KICK #channel target :reason
	// KICK #channel target :reason

	// KICK for UNKNOWN USER.
	mstring nick;

	if (proto.Numeric.User())
	{
	    nick = GetUser("!" + params.ExtractWord(2, ": "));
	    if (nick.empty())
	    {
		KillUnknownUser(params.ExtractWord(2, ": "));
		return;
	    }
	}
	else
	{
	    nick = GetUser(params.ExtractWord(2, ": "));
	    if (nick.empty())
	    {
		KillUnknownUser(params.ExtractWord(2, ": "));
		return;
	    }
	}

	mstring chan = GetChannel(params.ExtractWord(1, ": "));

	if (!source.Contains(".") && !chan.empty() && !Magick::instance().nickserv.GetLive(source)->IsInChan(chan))
	    KICK(Magick::instance().chanserv.FirstName(), params.ExtractWord(1, ": "), source, "You are not in this channel");

	// NOTE: as the message has already been broadcast,
	// we still need to acomodate for it.
	if (!chan.empty() && !nick.empty())
	    Magick::instance().nickserv.GetLive(nick)->Kick(chan, params.After(":"));
    }
    else if (msgtype == "KILL")
    {
	// :source/server KILL target :reason
	// LOCAL clients ONLY (remotes are a QUIT).
	//:PreZ KILL kick`kill`abuse :castle.srealm.net.au!PreZ (blah)
	mstring target = GetUser(params.ExtractWord(1, ": "));

	if (!target.empty())
	{
	    // sign on services again if they're killed.
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(target);
	    if (nlive->IsServices())
	    {
		LOG(LM_WARNING, "OTHER/KILLED",
		    (params.ExtractWord(1, ": "),
		     (!Magick::instance().nickserv.IsLive(source) ? source : Magick::instance().nickserv.GetLive(source)->
		      Mask(Nick_Live_t::N_U_P_H))));
		WLOCK2(("Server", "WaitIsOn"));
		WaitIsOn.insert(params.ExtractWord(1, ": "));
		sraw(((proto.Tokens() &&
		       !proto.GetNonToken("ISON").empty()) ? proto.GetNonToken("ISON") : mstring("ISON")) + " " +
		     params.ExtractWord(1, ": "));
	    }
	    int wc = params.After(":").WordCount("!");

	    nlive->Quit("Killed (" + params.After(":").After("!", wc - 1) + ")");
	    Magick::instance().nickserv.RemLive(nlive->Name());
	    mMessage::CheckDependancies(mMessage::NickNoExists, nlive->Name());
	    if (nlive->Numeric())
		mMessage::CheckDependancies(mMessage::NickNoExists, "!" + proto.Numeric.UserNumeric(nlive->Numeric()));
	}
	else
	{
	    LOG(LM_ERROR, "ERROR/REC_FORNONUSER", ("KILL", source, params.ExtractWord(1, ": ")));
	}

    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_L(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_L", (source, msgtype, params));

    static_cast < void > (params);

    if (msgtype == "LINKS")
    {
	//:ChanServ LINKS :temple.magick.tm
	//:temple.magick.tm 364 ChanServ temple.magick.tm temple.magick.tm :0 Magick IRC Services Test Network
	//:temple.magick.tm 365 ChanServ temple.magick.tm :End of /LINKS list.

	{
	    RLOCK(("IrcSvcHandler"));
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
		RLOCK2(("Server", "list"));
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
	    RLOCK(("IrcSvcHandler"));
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
		    RLOCK2(("ChanServ", "live"));
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
}

void Server::parse_M(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_M", (source, msgtype, params));

    if (msgtype == "MODE")
    {
	// :source MODE source :mode
	// :source MODE #channel mode params...
	// :server MODE #channel mode params... creationtime
	if (!GetChannel(params.ExtractWord(1, ": ")).empty())
	{
	    mstring chan = GetChannel(params.ExtractWord(1, ": "));

	    if (!chan.empty())
	    {
		mstring mode = params.ExtractWord(2, ": ");
		mstring mode_param = params.After(" ", 2);

		if (proto.Numeric.User())
		{
		    unsigned int fwdarg = 1;
		    mstring new_param;

		    for (unsigned int i = 0; i < mode.length() && fwdarg <= mode_param.WordCount(": "); i++)
		    {
			if (proto.ChanModeArg().Contains(mode[i]))
			{
			    // We MUST convert numerics to users ... *sigh*
			    switch (mode[i])
			    {
			    case 'o':
			    case 'h':
			    case 'v':
				new_param += " " + GetUser("!" + mode_param.ExtractWord(fwdarg, ": "));
				break;
			    default:
				new_param += " " + mode_param.ExtractWord(fwdarg, ": ");
			    }
			    fwdarg++;
			}
		    }
		    mode_param = new_param;
		}
		else
		    mode_param.replace(":", " ");
		Magick::instance().chanserv.GetLive(chan)->Mode(source, mode + " " + mode_param);
	    }
	    else
	    {
		LOG(LM_CRITICAL, "ERROR/REC_FORNONCHAN", ("MODE", source, params.ExtractWord(1, ": ")));
	    }
	}
	else
	{
	    mstring nick = GetUser(params.ExtractWord(1, ": "));

	    if (!nick.empty())
	    {
		Magick::instance().nickserv.GetLive(nick)->Mode(params.ExtractWord(2, ": "));
	    }
	    else
	    {
		LOG(LM_CRITICAL, "ERROR/REC_FORNONCHAN", ("MODE", source, params.ExtractWord(1, ": ")));
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
		RLOCK(("IrcSvcHandler"));
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
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_N(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_N", (source, msgtype, params));

    if (msgtype == "NAMES")
    {
	// :source NAMES #channel our.server
//:soul.darker.net 353 ChanServ = #chatzone :killkickabuseme @Aimee Jupiter @Allanon Ghost_ wildrose
//:soul.darker.net 366 ChanServ #chatzone :End of /NAMES list.
	sraw("366 " + source + " " + params.ExtractWord(1, ": ") + " :End of /NAMES list.");

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
	    mstring newnick = params.ExtractWord(1, ": ");

	    // DONT kill when we do SQUIT protection.
	    map < mstring, list < mstring > >::iterator i;
	    {
		RLOCK(("Server", "ToBeSquit"));
		for (i = ToBeSquit.begin(); i != ToBeSquit.end(); i++)
		{
		    list < mstring >::iterator k;
		    WLOCK2(("Server", "ToBeSquit", i->first.LowerCase()));
		    for (k = i->second.begin(); k != i->second.end(); k++)
			if (k->IsSameAs(newnick, true))
			{
			    list < mstring >::iterator j = k;
			    j--;
			    i->second.erase(k);
			    k = j;
			}
		}
	    }

	    mstring server;
	    mstring modes;

	    if (proto.Numeric.Server())
		server = "@";

	    switch (proto.Signon())
	    {
	    case 0000:
	    case 0001:
		break;
	    case 1000:		// NICK nick hops time user host server :realname
	    case 1001:		// NICK nick hops time user host server 1 :realname
	    case 1002:		// NICK nick hops time user host server 0 real-host :realname
		server += params.ExtractWord(6, ": ");
		break;
	    case 1003:		// NICK nick hops time user real-host host server 0 :realname
		server += params.ExtractWord(7, ": ");
		break;
	    case 2000:		// NICK nick hops time mode user host server :realname
	    case 2001:		// NICK nick hops time mode user host server 0 :realname
		modes = params.ExtractWord(4, ": ");
		server += params.ExtractWord(7, ": ");
		break;
	    case 2002:		// NICK nick hops time mode user host maskhost server 0 :realname
		modes = params.ExtractWord(4, ": ");
		server += params.ExtractWord(8, ": ");
		break;
	    case 2003:		// NICK nick hops time user host server 0 mode maskhost :realname
		modes = params.ExtractWord(8, ": ");
		server += params.ExtractWord(6, ": ");
		break;
	    case 3000:		// server NICK nick hops time user host [mode] ipaddr numeric :realname
		modes = params.ExtractWord(6, ": ");
		if (modes[0u] != '+')
		    modes.erase();
		server = source;
		break;
	    }
	    server = GetServer(server);

	    if (Magick::instance().nickserv.IsLiveAll(newnick))
	    {
		map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(newnick);
		COM(("Previous SQUIT checking if %s == %s and %s == %s", nlive->Squit().c_str(), server.c_str(),
		     nlive->SignonTime().DateTimeString().c_str(),
		     mDateTime(static_cast < time_t > (atoul(params.ExtractWord(3, ": ")))).DateTimeString().c_str()));
		// IF the squit server = us, and the signon time matches
		if (nlive->Squit().IsSameAs(server, true) &&
		    nlive->SignonTime() == mDateTime(static_cast < time_t > (atoul(params.ExtractWord(3, ": ")))))
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
			    Nick_Live_t(params.ExtractWord(2, ": "),
					static_cast < time_t > (atoul(params.ExtractWord(3, ": "))), server,
					params.ExtractWord(4, ": "), params.ExtractWord(5, ": "), params.After(":")));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 1001:		// NICK nick hops time user host server 1 :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(params.ExtractWord(1, ": "),
					static_cast < time_t > (atoul(params.ExtractWord(3, ": "))), server,
					params.ExtractWord(4, ": "), params.ExtractWord(5, ": "), params.After(":")));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 1002:		// NICK nick hops time user host server 0 real-host :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(params.ExtractWord(1, ": "),
					static_cast < time_t > (atoul(params.ExtractWord(3, ": "))), server,
					params.ExtractWord(4, ": "), params.ExtractWord(8, ": "), params.After(":")));
		    tmp->AltHost(params.ExtractWord(5, ": "));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 1003:		// NICK nick hops time user real-host host server 0 :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(params.ExtractWord(1, ": "),
					static_cast < time_t > (atoul(params.ExtractWord(4, ": "))), server,
					params.ExtractWord(4, ": "), params.ExtractWord(5, ": "), params.After(":")));
		    tmp->AltHost(params.ExtractWord(6, ": "));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 2000:		// NICK nick hops time mode user host server :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(params.ExtractWord(1, ": "),
					static_cast < time_t > (atoul(params.ExtractWord(3, ": "))), server,
					params.ExtractWord(5, ": "), params.ExtractWord(6, ": "), params.After(":")));
		    tmp->Mode(modes);
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 2001:		// NICK nick hops time mode user host server 0 :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(params.ExtractWord(1, ": "),
					static_cast < time_t > (atoul(params.ExtractWord(3, ": "))), server,
					params.ExtractWord(5, ": "), params.ExtractWord(6, ": "), params.After(":")));
		    tmp->Mode(modes);
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 2002:		// NICK nick hops time mode user host maskhost server 0 :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(params.ExtractWord(1, ": "),
					static_cast < time_t > (atoul(params.ExtractWord(3, ": "))), server,
					params.ExtractWord(5, ": "), params.ExtractWord(6, ": "), params.After(":")));
		    tmp->Mode(modes);
		    tmp->AltHost(params.ExtractWord(7, ": "));
		    Magick::instance().nickserv.AddLive(tmp);
		}
		break;
	    case 2003:		// NICK nick hops time user host server 0 mode maskhost :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(params.ExtractWord(1, ": "),
					static_cast < time_t > (atoul(params.ExtractWord(3, ": "))), server,
					params.ExtractWord(4, ": "), params.ExtractWord(5, ": "), params.After(":")));
		    tmp->Mode(modes);
		    tmp->AltHost(params.ExtractWord(9, ": "));
		    Magick::instance().nickserv.AddLive(tmp);
		}
	    case 3000:		// server NICK nick hops time user host [mode] ipaddr numeric :realname
		{
		    map_entry < Nick_Live_t >
			tmp(new
			    Nick_Live_t(params.ExtractWord(1, ": "),
					static_cast < time_t > (atoul(params.ExtractWord(3, ": "))), server,
					params.ExtractWord(4, ": "), params.ExtractWord(5, ": "), params.After(":")));
		    if (modes.length())
		    {
			tmp->Mode(modes);
			tmp->Numeric(proto.Numeric.UserNumeric(params.ExtractWord(8, ": ")));
		    }
		    else
			tmp->Numeric(proto.Numeric.UserNumeric(params.ExtractWord(7, ": ")));

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
		    WLOCK2(("Server", "i_UserMax"));
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
		    RLOCK2(("CommServ", "list"));
		    for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		    {
			map_entry < Committee_t > comm(iter->second);
			if (comm->IsOn(newnick))
			{
			    if (iter->first == Magick::instance().commserv.ALL_Name())
				setmode += Magick::instance().commserv.ALL_SetMode();
			    else if (iter->first == Magick::instance().commserv.REGD_Name())
				setmode += Magick::instance().commserv.REGD_SetMode();
			    else if (iter->first == Magick::instance().commserv.OPER_Name())
				setmode += Magick::instance().commserv.OPER_SetMode();
			    else if (iter->first == Magick::instance().commserv.ADMIN_Name())
				setmode += Magick::instance().commserv.ADMIN_SetMode();
			    else if (iter->first == Magick::instance().commserv.SOP_Name())
				setmode += Magick::instance().commserv.SOP_SetMode();
			    else if (iter->first == Magick::instance().commserv.SADMIN_Name())
				setmode += Magick::instance().commserv.SADMIN_SetMode();

			    MLOCK(("CommServ", "list", iter->first, "message"));
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
		if (!source.IsSameAs(params.ExtractWord(1, ": ")))
		{
		    mstring newnick(params.ExtractWord(1, ": "));

		    set < mstring > wason = nlive->Name(newnick);
		    Magick::instance().nickserv.RemLive(source);
		    nlive->setDelete(false);
		    mMessage::CheckDependancies(mMessage::NickNoExists, source);
		    Magick::instance().nickserv.AddLive(nlive);
		    mMessage::CheckDependancies(mMessage::NickExists, newnick);
		    if (Magick::instance().nickserv.IsRecovered(source))
		    {
			Magick::instance().server.NICK(source,
						       (Magick::instance().startup.Ownuser() ? source : Magick::instance().
							startup.Services_User()), Magick::instance().startup.Services_Host(),
						       Magick::instance().startup.Server_Name(), "Nickname Enforcer");
		    }

		    CommServ::list_t::iterator iter;
		    mstring setmode;

		    {
			RLOCK2(("CommServ", "list"));
			for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd();
			     iter++)
			{
			    map_entry < Committee_t > comm(iter->second);
			    if (wason.find(iter->first) == wason.end() && comm->IsOn(newnick))
			    {
				if (iter->first == Magick::instance().commserv.ALL_Name())
				    setmode += Magick::instance().commserv.ALL_SetMode();
				else if (iter->first == Magick::instance().commserv.REGD_Name())
				    setmode += Magick::instance().commserv.REGD_SetMode();
				else if (iter->first == Magick::instance().commserv.OPER_Name())
				    setmode += Magick::instance().commserv.OPER_SetMode();
				else if (iter->first == Magick::instance().commserv.ADMIN_Name())
				    setmode += Magick::instance().commserv.ADMIN_SetMode();
				else if (iter->first == Magick::instance().commserv.SOP_Name())
				    setmode += Magick::instance().commserv.SOP_SetMode();
				else if (iter->first == Magick::instance().commserv.SADMIN_Name())
				    setmode += Magick::instance().commserv.SADMIN_SetMode();

				MLOCK(("CommServ", "list", iter->first, "message"));
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
		    nlive->Name(params.ExtractWord(1, ": "));
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

	mstring dest = params.ExtractWord(1, ": ");
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

	if (!IsChan(dest))
	{
	    {
		RLOCK(("IrcSvcHandler"));
		if (Magick::instance().ircsvchandler != NULL && !Magick::instance().ircsvchandler->Burst())
		{
		    LOG(LM_WARNING, "ERROR/REC_FORNONUSER", ("NOTICE", source, params.ExtractWord(1, ": ")));
		}
	    }
	}
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_O(mstring & source, const mstring & msgtype, const mstring & params)
{
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
}

void Server::parse_P(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_P", (source, msgtype, params));

    if (msgtype == "PART")
    {
	if (source.Contains("."))
	    return;

	// :source PART #channel :reason
	Magick::instance().nickserv.GetLive(source)->Part(params.ExtractWord(1, ": "));
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
	if (params.ExtractWord(1, ": ") != Magick::instance().startup.Server(Magick::instance().CurrentServer()).second.second)
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

	mstring dest = params.ExtractWord(1, ": ");
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

	if (!IsChan(dest))
	{
	    {
		RLOCK(("IrcSvcHandler"));
		if (Magick::instance().ircsvchandler != NULL && !Magick::instance().ircsvchandler->Burst())
		{
		    LOG(LM_WARNING, "ERROR/REC_FORNONUSER", ("PRIVMSG", source, params.ExtractWord(1, ": ")));
		}
	    }
	}
    }
    else if (msgtype == "PROTOCTL")
    {
	if (!source.Contains("."))
	    return;

	// Turn on tokens dynamically ...
	if ((" " + params + " ").Contains(" TOKEN "))
	    proto.Tokens(true);
	if ((" " + params + " ").Contains(" SSJOIN ") || (" " + params + " ").Contains(" SJOIN "))
	    proto.SJoin(true);
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_Q(mstring & source, const mstring & msgtype, const mstring & params)
{
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
	mstring server1 = GetServer(params.ExtractWord(1, ": "));
	mstring server2 = GetServer(params.ExtractWord(2, ": "));

	if (params.WordCount(": ") == 2 && !server1.empty() && !server2.empty() &&
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
	    WLOCK2(("Server", "ToBeSquit"));
	    MCB(ToBeSquit.size());
	    ToBeSquit[server2.LowerCase()].push_back(source.LowerCase());
	    LOG(LM_NOTICE, "OTHER/SQUIT_FIRST", (server2, server1));

	    WLOCK3(("Server", "ServerSquit"));
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

	    // Kind of illegal to do, but accomodate anyway, re-signon
	    // services if someone quits them (how?!?)

	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(source);
	    numeric = nlive->Numeric();
	    if (nlive->IsServices())
		sraw(((proto.Tokens() &&
		       !proto.GetNonToken("ISON").empty()) ? proto.GetNonToken("ISON") : mstring("ISON")) + " " + source);
	    nlive->Quit(params.After(":"));
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
}

void Server::parse_R(mstring & source, const mstring & msgtype, const mstring & params)
{
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
    else if (msgtype == "RQLINE")
    {
	// We will ignore RQLINES because they're not relivant to us.
	// we will not be qlining our own clients ;P
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_S(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_S", (source, msgtype, params));

    if (msgtype == "SETHOST")
    {
	if (source.Contains("."))
	    return;

	// From UnrealIRCD
	// :source SVSHOST newhost
	Magick::instance().nickserv.GetLive(source)->AltHost(params.ExtractWord(1, ": "));
    }
    else if (msgtype == "SETTIME")
    {
	// RWORLDism -- ignore.
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
	    if (Magick::instance().startup.IsAllowed(params.ExtractWord(1, ": "), source))
	    {
		// unreal puts the numeric in the desctiption, ie.
		// SERVER downlink hops :Uversion-servopts-numeric description
		//        Only this way for direct links --^^^^^^^
		if (proto.Number() >= 70 && proto.Number() <= 79)
		{
		    numeric = proto.Numeric.ServerLineNumeric(params.After(":").ExtractWord(3, "-"));
		    map_entry < Server_t >
			tmp(new
			    Server_t(params.ExtractWord(1, ": ").LowerCase(),
				     atoi(params.ExtractWord(2, ": ").LowerCase().c_str()), params.After(":").After(" "),
				     numeric));
		    AddList(tmp);
		}
		else
		{
		    map_entry < Server_t >
			tmp(new
			    Server_t(params.ExtractWord(1, ": ").LowerCase(),
				     atoi(params.ExtractWord(2, ": ").LowerCase().c_str()), params.After(":")));
		    if (proto.Numeric.Server())
		    {
			numeric = proto.Numeric.ServerLineNumeric(params.ExtractWord(proto.Numeric.Field(), ": "));
			tmp->Numeric(numeric);
		    }
		    AddList(tmp);
		}

		LOG(LM_INFO, "OTHER/LINK", (params.ExtractWord(1, ": "), Magick::instance().startup.Server_Name()));
		Magick::instance().GotConnect(true);
		SignOnAll();
		mMessage::CheckDependancies(mMessage::ServerExists, params.ExtractWord(1, ": "));
		if (numeric)
		    mMessage::CheckDependancies(mMessage::ServerExists, "@" + proto.Numeric.ServerNumeric(numeric));
	    }
	    else
	    {
//                  ANNOUNCE(Magick::instance().operserv.FirstName(),
//                      "MISC/INVALIDLINK", (
//                      params.ExtractWord(1, ": "),
//                      Magick::instance().startup.Server_Name()));
		LOG(LM_ERROR, "OTHER/INVALIDLINK", (params.ExtractWord(1, ": "), Magick::instance().startup.Server_Name()));
		raw("SQUIT " + params.ExtractWord(1, ": ") + " :" + Magick::instance().getMessage("MISC/MAYNOTLINK"));
		Magick::instance().Disconnect();
	    }
	}
	else
	{
	    if (Magick::instance().startup.IsAllowed(params.ExtractWord(1, ": "), source))
	    {
		if (IsList(source))
		{
		    map_entry < Server_t >
			tmp(new
			    Server_t(params.ExtractWord(1, ": ").LowerCase(), source,
				     atoi(params.ExtractWord(2, ": ").LowerCase().c_str()), params.After(":")));
		    if (proto.Numeric.Server())
		    {
			numeric = proto.Numeric.ServerLineNumeric(params.ExtractWord(proto.Numeric.Field(), ": "));
			tmp->Numeric(numeric);
		    }
		    AddList(tmp);
		    LOG(LM_INFO, "OTHER/LINK", (params.ExtractWord(1, ": "), source));

		    mMessage::CheckDependancies(mMessage::ServerExists, params.ExtractWord(1, ": "));
		    if (numeric)
			mMessage::CheckDependancies(mMessage::ServerExists, "@" + proto.Numeric.ServerNumeric(numeric));
		}
		else
		{
		    LOG(LM_ERROR, "ERROR/REC_FORNONSERVER", ("SERVER", params.ExtractWord(1, ": "), source));
		}
	    }
	    else
	    {
		ANNOUNCE(Magick::instance().operserv.FirstName(), "MISC/INVALIDLINK", (params.ExtractWord(1, ": "), source));
		LOG(LM_ERROR, "OTHER/INVALIDLINK", (params.ExtractWord(1, ": "), source));
		sraw("SQUIT " + params.ExtractWord(1, ": ") + " :" + Magick::instance().getMessage("MISC/MAYNOTLINK"));
	    }
	}
    }
    else if (msgtype == "SGLINE")
    {
	// like we care ...
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
	    mstring chan(params.ExtractWord(2, ": "));

	    unsigned int i;

	    vector < mstring > users;
	    mstring modes;

	    if (params.Before(":").WordCount(" ") >= 3)
		modes = params.Before(":").ExtractWord(3, " ");
	    mstring mode_params, nick;
	    bool oped, halfoped, voiced, owner, prot, isban, isexcept;

	    if (modes.length())
	    {
		for (i = 0; i < proto.ChanModeArg().length(); i++)
		    if (modes.Contains(proto.ChanModeArg() [i]))
		    {
			mode_params = params.Before(":").After(" ", 3);
			break;
		    }
	    }
	    for (i = 1; i <= params.After(":").WordCount(" "); i++)
	    {
		nick = params.After(":").ExtractWord(i, " ");
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
		    if (!(isban || isexcept) && !GetUser(nick).empty())
		    {
			nick = GetUser(nick);
			if (nick.empty())
			    continue;

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
		    else if (isban)
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
		clive->Creation_Time(static_cast < time_t > (atoul(params.ExtractWord(1, ": "))));
		clive->Mode(source, modes + " " + mode_params);
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
		for (i = 2; i <= params.WordCount(":, "); i++)
		{
		    char c = 0;

		    chan = params.ExtractWord(i, ":, ");
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
    else if (msgtype == "SNICK")
    {
	if (!source.Contains("."))
	    return;

	// NEW USER
	mstring newnick = params.ExtractWord(1, ": ");

	// DONT kill when we do SQUIT protection.
	map < mstring, list < mstring > >::iterator i;
	{
	    RLOCK(("Server", "ToBeSquit"));
	    for (i = ToBeSquit.begin(); i != ToBeSquit.end(); i++)
	    {
		list < mstring >::iterator k;
		WLOCK2(("Server", "ToBeSquit", i->first.LowerCase()));
		for (k = i->second.begin(); k != i->second.end(); k++)
		    if (k->IsSameAs(newnick, true))
		    {
			i->second.erase(k);
			break;
		    }
	    }
	}

	mstring modes;
	mstring server;

	if (proto.Numeric.Server())
	    server = "@";

	switch (proto.Signon())
	{
	case 0000:
	case 0001:
	    break;
	case 1000:		// SNICK nick hops time user host server modes :realname
	    modes = params.ExtractWord(7, ": ");
	    server += params.ExtractWord(6, ": ");
	    break;
	case 1001:		// SNICK nick hops time user host server 1 modes :realname
	    modes = params.ExtractWord(8, ": ");
	    server += params.ExtractWord(6, ": ");
	    break;
	case 1002:		// SNICK nick hops time user host server 0 real-host modes :realname
	    modes = params.ExtractWord(9, ": ");
	    server += params.ExtractWord(6, ": ");
	    break;
	case 1003:		// SNICK nick hops time user real-host host server 0 modes :realname
	    modes = params.ExtractWord(9, ": ");
	    server += params.ExtractWord(7, ": ");
	    break;
	case 2000:		// SNICK nick hops time mode user host server :realname
	case 2001:		// SNICK nick hops time mode user host server 0 :realname
	    modes = params.ExtractWord(4, ": ");
	    server += params.ExtractWord(7, ": ");
	    break;
	case 2002:		// SNICK nick hops time mode user host maskhost server 0 :realname
	    modes += params.ExtractWord(4, ": ");
	    server += params.ExtractWord(8, ": ");
	    break;
	case 2003:		// SNICK nick hops time user host server 0 mode maskhost :realname
	    modes = params.ExtractWord(8, ": ");
	    server += params.ExtractWord(6, ": ");
	    break;
	case 3000:		// SNICK nick hops time user host [mode] ipaddr numeric :realname
	    modes = params.ExtractWord(6, ": ");
	    if (!modes.length())
		modes.erase();
	    server = source;
	    break;
	}
	server = GetServer(server);

	if (Magick::instance().nickserv.IsLiveAll(newnick))
	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(newnick);
	    COM(("Previous SQUIT checking if %s == %s and %s == %s", nlive->Squit().c_str(), server.c_str(),
		 nlive->SignonTime().DateTimeString().c_str(),
		 mDateTime(static_cast < time_t > (atoul(params.ExtractWord(3, ": ")))).DateTimeString().c_str()));
	    // IF the squit server = us, and the signon time matches
	    if (nlive->Squit().IsSameAs(server, true) &&
		nlive->SignonTime() == mDateTime(static_cast < time_t > (atoul(params.ExtractWord(3, ": ")))))
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

	// hops = servers from us
	// services = 1 for service, 0 for user
	// DAL4.4.15+ SNICK name hops time user host server services modes :real name

	switch (proto.Signon())
	{
	case 0000:
	case 0001:
	    break;
	case 1000:		// SNICK nick hops time user host server modes :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(params.ExtractWord(1, ": "), static_cast < time_t > (atoul(params.ExtractWord(3, ": "))),
				    server, params.ExtractWord(4, ": "), params.ExtractWord(5, ": "), params.After(":")));
		tmp->Mode(modes);
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 1001:		// SNICK nick hops time user host server 1 modes :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(params.ExtractWord(1, ": "), static_cast < time_t > (atoul(params.ExtractWord(3, ": "))),
				    server, params.ExtractWord(4, ": "), params.ExtractWord(5, ": "), params.After(":")));
		tmp->Mode(modes);
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 1002:		// SNICK nick hops time user host server 0 real-host modes :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(params.ExtractWord(1, ": "), static_cast < time_t > (atoul(params.ExtractWord(3, ": "))),
				    server, params.ExtractWord(4, ": "), params.ExtractWord(8, ": "), params.After(":")));
		tmp->Mode(modes);
		tmp->AltHost(params.ExtractWord(5, ": "));
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 1003:		// SNICK nick hops time user real-host host server 0 modes :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(params.ExtractWord(1, ": "), static_cast < time_t > (atoul(params.ExtractWord(3, ": "))),
				    server, params.ExtractWord(4, ": "), params.ExtractWord(5, ": "), params.After(":")));
		tmp->Mode(modes);
		tmp->AltHost(params.ExtractWord(6, ": "));
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 2000:		// SNICK nick hops time mode user host server :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(params.ExtractWord(1, ": "), static_cast < time_t > (atoul(params.ExtractWord(3, ": "))),
				    server, params.ExtractWord(5, ": "), params.ExtractWord(6, ": "), params.After(":")));
		tmp->Mode(modes);
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 2001:		// SNICK nick hops time mode user host server 0 :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(params.ExtractWord(1, ": "), static_cast < time_t > (atoul(params.ExtractWord(3, ": "))),
				    server, params.ExtractWord(5, ": "), params.ExtractWord(6, ": "), params.After(":")));
		tmp->Mode(params.ExtractWord(4, ": "));
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 2002:		// SNICK nick hops time mode user host maskhost server 0 :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(params.ExtractWord(1, ": "), static_cast < time_t > (atoul(params.ExtractWord(3, ": "))),
				    server, params.ExtractWord(5, ": "), params.ExtractWord(6, ": "), params.After(":")));
		tmp->Mode(modes);
		tmp->AltHost(params.ExtractWord(7, ": "));
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 2003:		// SNICK nick hops time user host server 0 mode maskhost :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(params.ExtractWord(1, ": "), static_cast < time_t > (atoul(params.ExtractWord(3, ": "))),
				    server, params.ExtractWord(4, ": "), params.ExtractWord(5, ": "), params.After(":")));
		tmp->Mode(modes);
		tmp->AltHost(params.ExtractWord(9, ": "));
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 3000:		// server NICK nick hops time user host [mode] ipaddr numeric :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(params.ExtractWord(1, ": "), static_cast < time_t > (atoul(params.ExtractWord(3, ": "))),
				    server, params.ExtractWord(4, ": "), params.ExtractWord(5, ": "), params.After(":")));
		if (modes.length())
		{
		    tmp->Mode(modes);
		    tmp->Numeric(proto.Numeric.UserNumeric(params.ExtractWord(8, ": ")));
		}
		else
		    tmp->Numeric(proto.Numeric.UserNumeric(params.ExtractWord(7, ": ")));
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
		WLOCK2(("Server", "i_UserMax"));
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
		RLOCK2(("CommServ", "list"));
		for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		{
		    map_entry < Committee_t > comm(iter->second);
		    if (comm->IsOn(newnick))
		    {
			if (iter->first == Magick::instance().commserv.ALL_Name())
			    setmode += Magick::instance().commserv.ALL_SetMode();
			else if (iter->first == Magick::instance().commserv.REGD_Name())
			    setmode += Magick::instance().commserv.REGD_SetMode();
			else if (iter->first == Magick::instance().commserv.OPER_Name())
			    setmode += Magick::instance().commserv.OPER_SetMode();
			else if (iter->first == Magick::instance().commserv.ADMIN_Name())
			    setmode += Magick::instance().commserv.ADMIN_SetMode();
			else if (iter->first == Magick::instance().commserv.SOP_Name())
			    setmode += Magick::instance().commserv.SOP_SetMode();
			else if (iter->first == Magick::instance().commserv.SADMIN_Name())
			    setmode += Magick::instance().commserv.SADMIN_SetMode();

			MLOCK(("CommServ", "list", iter->first, "message"));
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

	mstring server = GetServer(params.ExtractWord(1, ": "));

	if (!server.empty())
	{
	    map_entry < Server_t > svr = GetList(server);
	    if (svr->Jupe())
		raw(((proto.Tokens() &&
		      !proto.GetNonToken("SQUIT").empty()) ? proto.GetNonToken("SQUIT") : mstring("SQUIT")) + " " +
		    svr->Name() + " :" + params.After(": ", 2));

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
		WLOCK2(("Server", "ToBeSquit"));
		WLOCK3(("Server", "ServerSquit"));
		MCB(ToBeSquit.size());
		CB(1, ServerSquit.size());
		for (i = 0; i < tlist.size(); i++)
		{
		    if (ToBeSquit.find(tlist[i]) != ToBeSquit.end())
			ToBeSquit.erase(tlist[i]);
		    if (ServerSquit.find(tlist[i]) != ServerSquit.end())
		    {
			mstring *arg = NULL;
			if (Magick::instance().reactor().
			    cancel_timer(ServerSquit[tlist[i]], reinterpret_cast < const void ** > (arg)) && arg != NULL)
			    delete arg;
		    }
		    while (Magick::instance().Pause())
			ACE_OS::sleep(1);
		    ServerSquit[tlist[i]] =
			Magick::instance().reactor().schedule_timer(&squit, new mstring(tlist[i]),
								    ACE_Time_Value(Magick::instance().config.Squit_Protect()));
		}
		CE(1, ServerSquit.size());
		MCE(ToBeSquit.size());
	    }

	    NickServ::live_t::iterator iter;
	    vector < mstring > chunked;
	    vector < pair < mstring, unsigned long > > chunked2;

	    {
		RLOCK(("NickServ", "live"));
		for (iter = Magick::instance().nickserv.LiveBegin(); iter != Magick::instance().nickserv.LiveEnd(); iter++)
		{
		    map_entry < Nick_Live_t > nlive(iter->second);
		    if (nlive->IsServices() && ListSize() == 0)
		    {
			chunked.push_back(iter->first);
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
	sraw("219 " + source + " " + params.ExtractWord(1, ": ") + " :End of /STATS report");
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
	mstring nick = GetUser(params.ExtractWord(1, ": "));

	if (!nick.empty())
	    Magick::instance().nickserv.GetLive(nick)->AltHost(params.ExtractWord(2, ": "));
    }
    else if (msgtype == "SVSKILL")
    {
	// Same as KILL (but by services)
    }
    else if (msgtype == "SVSMODE")
    {
	// Handle just as mode, WITHOUT sanity
	if (!GetChannel(params.ExtractWord(1, ": ")).empty())
	{
	    mstring chan = GetChannel(params.ExtractWord(1, ": "));

	    if (!chan.empty())
	    {
		mstring mode = params.ExtractWord(2, ": ");
		mstring mode_param = params.After(" ", 2);

		if (proto.Numeric.User())
		{
		    unsigned int fwdarg = 1;
		    mstring new_param;

		    for (unsigned int i = 0; i < mode.length() && fwdarg <= mode_param.WordCount(": "); i++)
		    {
			if (proto.ChanModeArg().Contains(mode[i]))
			{
			    // We MUST convert numerics to users ... *sigh*
			    switch (mode[i])
			    {
			    case 'o':
			    case 'h':
			    case 'v':
				new_param += " " + GetUser("!" + mode_param.ExtractWord(fwdarg, ": "));
				break;
			    default:
				new_param += " " + mode_param.ExtractWord(fwdarg, ": ");
			    }
			    fwdarg++;
			}
		    }
		    mode_param = new_param;
		}
		else
		    mode_param.replace(":", " ");
		Magick::instance().chanserv.GetLive(chan)->Mode(source, mode + " " + mode_param);
	    }
	    else
	    {
		LOG(LM_CRITICAL, "ERROR/REC_FORNONCHAN", ("SVSMODE", source, params.ExtractWord(1, ": ")));
	    }
	}
	else
	{
	    mstring nick = GetUser(params.ExtractWord(1, ": "));

	    if (!nick.empty())
	    {
		Magick::instance().nickserv.GetLive(nick)->Mode(params.ExtractWord(2, ": "));
	    }
	    else
	    {
		LOG(LM_CRITICAL, "ERROR/REC_FORNONUSER", ("SVSMODE", source, params.ExtractWord(1, ": ")));
	    }
	}
    }
    else if (msgtype == "SVSNICK")
    {
	// forcably changed nicks (handle like nick)
    }
    else if (msgtype == "SVSNOOP")
    {
	// Deny all OPERS on server, ignore.
    }
    else if (msgtype == "SZLINE")
    {
	// zzzzZzzzZzzZzZZzZzZZZZzzzZzzz....
    }
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_T(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_T", (source, msgtype, params));

    if (msgtype == "TIME")
    {
	// :source TIME :our.server
	sraw("391 " + source + " :" + mDateTime::CurrentDateTime().DateTimeString());
    }
    else if (msgtype == "TOPIC")
    {
	// :user TOPIC #channel :topic
	// :user TOPIC #channel
	// -OR-
	// :server/user TOPIC #channel setter time :topic
	// :server/user TOPIC #channel setter
	// TIME is not standard (time is optional);

	mstring chan = GetChannel(params.ExtractWord(1, ": "));

	if (!chan.empty())
	{
	    map_entry < Chan_Live_t > clive = Magick::instance().chanserv.GetLive(chan);
	    if (params.Contains(":"))
	    {			// Setting
		if (params.Before(":").WordCount(" ") < 2)
		    clive->Topic(source, params.After(":"), source, mDateTime::CurrentDateTime());
		else if (params.Before(":").WordCount(" ") < 3)
		    clive->Topic(source, params.After(":"), params.ExtractWord(2, ": "), mDateTime::CurrentDateTime());
		else
		    clive->Topic(source, params.After(":"), params.ExtractWord(2, ": "),
				 static_cast < time_t > (atoul(params.ExtractWord(3, ": "))));
	    }
	    else
	    {			// Clearing
		if (params.WordCount(" ") < 2)
		    clive->Topic(source, "", source);
		else
		    clive->Topic(source, "", params.ExtractWord(2, ": "));
	    }
	}
	else
	{
	    LOG(LM_CRITICAL, "ERROR/REC_FORNONCHAN", ("TOPIC", source, params.ExtractWord(1, ": ")));
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
	    RLOCK(("IrcSvcHandler"));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		raw(((proto.Tokens() &&
		      !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " " + source +
		    " :" + tmp.c_str());
		return;
	    }
	}
	mstring out;

	mstring nick = GetUser(params.ExtractWord(1, ": "));

	if (!nick.empty() && Magick::instance().nickserv.GetLive(nick)->IsServices())
	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(nick);
	    if (nlive->HasMode("o"))
		out << "204 " << source << " Operator 10 ";
	    else
		out << "205 " << source << " User 1 ";
	    out << nlive->Name() << " [" << nlive->User() << "@" << nlive->Host() << "] :" << nlive->LastAction().
		SecondsSince();
	    sraw(out);
	}
	else
	{
	    unsigned long opers = 0, users = 0;

	    out << "206 " << source << "Server 50 " << ListSize() +
		1 << "S " << Magick::instance().nickserv.
		LiveSize() << "C " << OurUplink() << "[0.0.0.0] AutoConn.!*@" << Magick::instance().startup.
		Server_Name() << " :0";
	    sraw(out);

	    NickServ::live_t::iterator iter;
	    {
		RLOCK2(("NickServ", "live"));
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
			out << nlive->Name() << " [" << nlive->User() << "@" << nlive->Host() << "] :" << nlive->LastAction().
			    SecondsSince();
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
    else
    {
	LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
    }
}

void Server::parse_U(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_U", (source, msgtype, params));

    if (msgtype == "UMODE2")
    {
	if (source.Contains("."))
	    return;

	Magick::instance().nickserv.GetLive(source)->Mode(params.ExtractWord(1, ": "));
    }
    else if (msgtype == "UNGLINE")
    {
	// We will ignore GLINES because they're not relivant to us.
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
	mstring newnick = params.ExtractWord(1, ": ");

	// DONT kill when we do SQUIT protection.
	map < mstring, list < mstring > >::iterator i;
	{
	    RLOCK(("Server", "ToBeSquit"));
	    for (i = ToBeSquit.begin(); i != ToBeSquit.end(); i++)
	    {
		list < mstring >::iterator k;
		WLOCK2(("Server", "ToBeSquit", i->first.LowerCase()));
		for (k = i->second.begin(); k != i->second.end(); k++)
		    if (k->IsSameAs(newnick, true))
		    {
			list < mstring >::iterator j = k;
			j--;
			i->second.erase(k);
			k = j;
		    }
	    }
	}

	mstring server;
	mstring modes;

	if (proto.Numeric.Server())
	    server = "@";

	switch (proto.Signon())
	{
	case 0000:		// USER nick user host server :realname
	    server += params.ExtractWord(4, ": ");
	    break;
	case 0001:		// USER nick time user host server :realname
	    server += params.ExtractWord(5, ": ");
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

	server = GetServer(server);

	if (Magick::instance().nickserv.IsLiveAll(newnick))
	{
	    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(newnick);
	    COM(("Previous SQUIT checking if %s == %s and %s == %s", nlive->Squit().c_str(), server.c_str(),
		 nlive->SignonTime().DateTimeString().c_str(),
		 mDateTime(static_cast < time_t > (atoul(params.ExtractWord(3, ": ")))).DateTimeString().c_str()));
	    // IF the squit server = us, and the signon time matches
	    if (nlive->Squit().IsSameAs(server, true) &&
		nlive->SignonTime() == mDateTime(static_cast < time_t > (atoul(params.ExtractWord(3, ": ")))))
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
			Nick_Live_t(params.ExtractWord(1, ": "), time(NULL), server, params.ExtractWord(2, ": "),
				    params.ExtractWord(3, ": "), params.After(":")));
		Magick::instance().nickserv.AddLive(tmp);
	    }
	    break;
	case 0001:		// USER nick time user host server :realname
	    {
		map_entry < Nick_Live_t >
		    tmp(new
			Nick_Live_t(params.ExtractWord(1, ": "), static_cast < time_t > (atoul(params.ExtractWord(2, ": "))),
				    server, params.ExtractWord(3, ": "), params.ExtractWord(4, ": "), params.After(":")));
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
		WLOCK2(("Server", "i_UserMax"));
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
		RLOCK2(("CommServ", "list"));
		for (iter = Magick::instance().commserv.ListBegin(); iter != Magick::instance().commserv.ListEnd(); iter++)
		{
		    map_entry < Committee_t > comm(iter->second);
		    if (comm->IsOn(newnick))
		    {
			if (iter->first == Magick::instance().commserv.ALL_Name())
			    setmode += Magick::instance().commserv.ALL_SetMode();
			else if (iter->first == Magick::instance().commserv.REGD_Name())
			    setmode += Magick::instance().commserv.REGD_SetMode();
			else if (iter->first == Magick::instance().commserv.OPER_Name())
			    setmode += Magick::instance().commserv.OPER_SetMode();
			else if (iter->first == Magick::instance().commserv.ADMIN_Name())
			    setmode += Magick::instance().commserv.ADMIN_SetMode();
			else if (iter->first == Magick::instance().commserv.SOP_Name())
			    setmode += Magick::instance().commserv.SOP_SetMode();
			else if (iter->first == Magick::instance().commserv.SADMIN_Name())
			    setmode += Magick::instance().commserv.SADMIN_SetMode();

			MLOCK(("CommServ", "list", iter->first, "message"));
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
	    mstring nick = GetUser(params.ExtractWord(1, ": "));

	    if (!nick.empty())
	    {
		map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(nick);
		if (!Magick::instance().nickserv.IsStored(nlive->Name()) ? 1 : !Magick::instance().nickserv.
		    GetStored(nlive->Name())->Private())
		{
		    sraw("302 " + source + " :" + nlive->Name() + "*=-" + nlive->User() + "@" + nlive->AltHost());
		}
		else
		{
		    sraw("302 " + source + " :" + nlive->Name() + "*=-" + nlive->User() + "@" +
			 Magick::instance().getMessage("VALS/ONLINE"));
		}
	    }
	    else
	    {
		sraw("401 " + source + " " + params.ExtractWord(1, ": ") + " :No such nickname/channel.");
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
}

void Server::parse_V(mstring & source, const mstring & msgtype, const mstring & params)
{
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
}

void Server::parse_W(mstring & source, const mstring & msgtype, const mstring & params)
{
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
	    RLOCK(("IrcSvcHandler"));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		raw(((proto.Tokens() &&
		      !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " " + source +
		    " :" + tmp.c_str());
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
	    RLOCK(("IrcSvcHandler"));
	    if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp = parseMessage(Magick::instance().getMessage(source, "MISC/HTM"), mVarArray(msgtype));

		raw(((proto.Tokens() &&
		      !proto.GetNonToken("NOTICE").empty()) ? proto.GetNonToken("NOTICE") : mstring("NOTICE")) + " " + source +
		    " :" + tmp.c_str());
		sraw("318 " + source + " " + params.ExtractWord(1, ": ") + " :End of /WHOIS list.");
		return;
	    }
	    else
	    {
		mstring nick = GetUser(params.After(":"));

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
		    if (outline.After(":").length() > 0)
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
		    sraw("401 " + source + " " + params.ExtractWord(1, ": ") + " :No such nickname/channel.");
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
}

void Server::parse_X(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_X", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
}

void Server::parse_Y(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::parse_Y", (source, msgtype, params));

    static_cast < void > (source);
    static_cast < void > (params);

    LOG(LM_WARNING, "ERROR/UNKNOWN_MSG", (msgtype));
}

void Server::parse_Z(mstring & source, const mstring & msgtype, const mstring & params)
{
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
}

void Server::numeric_execute(mstring & source, const mstring & msgtype, const mstring & params)
{
    FT("Server::numeric_execute", (source, msgtype, params));

    static_cast < void > (source);

    int numeric = atoi(msgtype);

    // Numerics direct from RFC1459
    // MOST we can (and will) ignore.

    switch (numeric)
    {
    case 303:			// RPL_ISON
	{
	    {
		RLOCK(("IrcSvcHandler"));
		if (Magick::instance().ircsvchandler != NULL && Magick::instance().ircsvchandler->Burst())
		{
		    Magick::instance().ircsvchandler->EndBurst();
		    if (!proto.Burst().empty())
			Magick::instance().server.
			    sraw(((proto.Tokens() &&
				   !proto.GetNonToken(proto.Burst()).empty()) ? proto.GetNonToken(proto.
												  Burst()) : mstring(proto.
														     Burst
														     ())));
		}
	    }

	    Magick::instance().operserv.CloneList_check();
	    for (unsigned int i = 1; i <= params.WordCount(": "); i++)
	    {
		// Remove clients from 'signon list' who are
		// already on the network.
		WLOCK(("Server", "WaitIsOn"));
		if (WaitIsOn.find(params.ExtractWord(i, ": ").LowerCase()) != WaitIsOn.end())
		    WaitIsOn.erase(params.ExtractWord(i, ": "));
	    }
	    if (WaitIsOn.size())
	    {
		set < mstring >::reverse_iterator k;
		RLOCK(("Server", "WaitIsOn"));
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
				RLOCK2(("ChanServ", "stored"));
				for (iter = Magick::instance().chanserv.StoredBegin();
				     iter != Magick::instance().chanserv.StoredEnd(); iter++)
				{
				    map_entry < Chan_Stored_t > cstored(iter->second);
				    map_entry < Chan_Live_t > clive;
				    if (Magick::instance().chanserv.IsLive(iter->first))
					clive = Magick::instance().chanserv.GetLive(iter->first);

				    // If its live and got JOIN on || not live and mlock +k or +i
				    if ((clive.entry() != NULL && cstored->Join()) ||
					(clive.entry() == NULL && !cstored->Forbidden() &&
					 (!cstored->Mlock_Key().empty() || cstored->Mlock_On().Contains("i"))))
				    {
					joins.push_back(iter->first);
					if (clive.entry() == NULL)
					{
					    modes[iter->first] = "+s";
					    if (cstored->Mlock_On().Contains("i"))
						modes[iter->first] += "i";
					    if (!cstored->Mlock_Key().empty())
						modes[iter->first] += "k " + cstored->Mlock_Key();
					}
				    }

				    if (clive.entry() != NULL && !cstored->Last_Topic().empty() && !cstored->Suspended())
				    {
					if ((cstored->Topiclock() && clive->Topic() != cstored->Last_Topic()) ||
					    (cstored->Keeptopic() && clive->Topic().empty()))
					{
					    topics[iter->first] =
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

		    if (!(proto.P12() || (proto.Signon() >= 2000 && proto.Signon() < 4000)) &&
			Magick::instance().nickserv.IsLive(*k) && (!Magick::instance().startup.Setmode().empty() ||
								   !proto.ServicesModes().empty()))
			Magick::instance().server.MODE(*k, "+" + Magick::instance().startup.Setmode() + proto.ServicesModes());
		    FlushMsgs(*k);
		}
	    }
	    {
		WLOCK(("Server", "WaitIsOn"));
		WaitIsOn.clear();
	    }
	    if (!proto.EndBurst().empty())
		Magick::instance().server.
		    sraw(((proto.Tokens() &&
			   !proto.GetNonToken(proto.EndBurst()).empty()) ? proto.GetNonToken(proto.EndBurst()) : mstring(proto.
															 EndBurst
															 ())));
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
}

void Server::DumpB() const
{
    MB(0, (i_UserMax, ServerSquit.size(), ToBeSquit.size(), i_OurUplink, ToBeSent.size()));
}

void Server::DumpE() const
{
    ME(0, (i_UserMax, ServerSquit.size(), ToBeSquit.size(), i_OurUplink, ToBeSent.size()));
}
