#ifndef WIN32
#pragma interface
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
#ifndef _SERVER_H
#define _SERVER_H
#include "pch.h"
RCSID(server_h, "@(#) $Id$");

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

#include "base.h"
#include "ircsocket.h"

class Protocol
{
    // Max nickname length
    unsigned int i_NickLen;

    // Max length of line excluding nickname / comamnd.
    // ie. reasonable max length (def: 512 - 62 = 450).
    unsigned int i_MaxLine;

    bool i_Globops;		// GLOBOPS supported
    bool i_Helpops;		// HELPOPS supported
    bool i_Chatops;		// CHATOPS supported
    bool i_Tokens;		// Tokenized messages supported
    bool i_P12;			// P12 (SNICK) support
    bool i_TSora;		// Extra timestamping (and SVINFO line)
    bool i_SJoin;		// Use SJOIN instead of JOIN
    bool i_BigTopic;		// Topic includes setter and timestamp
    bool i_TopicJoin;		// ChanServ must join to set topic
    bool i_ServerModes;		// Server must set modes, not ChanServ

    /* AKILL types
     *
     * 0000 = KILL user :reason
     * 1000 = AKILL host user :reason
     *        RAKILL host user
     * 1001 = AKILL host user time killer now :reason
     *        RAKILL host user
     * 1002 = TKL + G user host killer expiry set :reason
     *        TKL - G user host killer
     * 2000 = GLINE * +time user@host :reason
     *        UNGLINE * user@host
     * 2001 = GLINE * +user@host time :reason
     *        GLINE * -user@host
     * 2002 = GLINE +user@host time :reason
     *        GLINE -user@host
     * 3000 = GLINE oper operuser operhost operserver user host :reason
     *            (above repeated 3 times, for 3 different opers/servers)
     *        NO UNGLINE
     * 3001 = KLINE killer * time user host :reason
     *        NO UNKLINE
     */
    unsigned int i_Akill;

    /* Signon Types
     * 
     * 0000 = USER nick user host server :realname
     * 0001 = USER nick signon-time user host server :realname
     * 
     * 1000 = NICK nick hops signon-time user host server :realname
     * 1001 = NICK nick hops signon-time user host server service :realname
     * 1002 = NICK nick hops signon-time user host server service althost :realname
     * 1003 - NICK nick hops signon-time user host althost server service :realname
     *
     * 2000 = NICK nick hops signon-time mode user host server :realname
     * 2001 = NICK nick hops signon-time mode user host server service :realname
     * 2002 = NICK nick hops signon-time mode user host althost server helper :realname
     * 2003 = NICK nick hops signon-time user host server service mode althost :realname
     *
     * 3000 series gets its server from the SOURCE ... not part of the message
     * NOTE: 3000 makes the mode OPTIONAL ... damn ircu ...
     * 3000 = NICK nick hops signon-time user host [mode] ipaddress nicknumeric :realname
     */
    unsigned int i_Signon;

    unsigned int i_Modes;	// Modes per line
    mstring i_ServicesModes;	// Modes services MUST set to be services
    mstring i_ChanModeArg;	// Channel Modes that have arguments (def. ovbkl)

    /* The server line will have 6 replacements ($1 ... $6) done.
     * $1 = Server name              $2 = hop count
     * $3 = Server description       $4 = Server numeric
     * $5 = Current time (time_t)    $6 = Services start time (time_t)
     */
    mstring i_Server;

    mstring i_Burst;		// How do we announce a flood (if at all)
    mstring i_EndBurst;		// How do we announce end of a flood (if at all)

    /* PROTOCTL and CAPAB identifiers
     *
     * NOQUIT      Do not quit each user on SQUIT (assume it)
     * TOKEN       Use one-char tokens instead of PRIVMSG, etc
     * WATCH=128   Allow the WATCH command (for signon/off notification)
     * SAFELIST    Currently does nothing.
     * TS3         Timestamp everything (aka. TSora)
     * SSJOIN      Use SJOIN for joins, includes: @ (op) + (voice)
     * SJOIN       Same as SSJOIN, includes: % (halfop) * (owner) ~ (prot)
     * SJOIN2      Same as SJOIN, but <none> is now <->
     * SJ3         Same as SJOIN2, includes: & (ban) " (except)
     * BURST       Allow the BURST keyword (to indicate sync)
     * UNCONNECT   Support UNCONNECT (at the moment, does nothing)
     * NICKv2      Add UMODE & v.host (if supported) to nick signon
     * UMODE2      Allow the UMODE2 command (aka. :user MODE user mode)
     * NS          Allow use of server numerics (not just server name)
     * VHP         Enable virtual host support
     * VL          v:line support (protocol based deny info)
     * SJB64       Support 64-bit timestamps  (*)
     *
     * ZIP         Compress stream to irc server using zlib  (*)
     * QS          Quit Storm (ie. assume quit's on SQUIT)
     * EX          Channel mode +/-e (ban exception)
     * CHW         Allow '@' or '+' before #channel in notice/privmsg
     * DE          Channel mode +/-d (deny, ie. regex ban)  (*)
     *
     * (*) = We do not support this at the moment.
     */

    mstring i_Protoctl;		/* Verbatum (null if not sent) */

    /* Strings for each of the following commands ... */
    mstring i_SVSNICK;
    mstring i_SVSMODE;
    mstring i_SVSKILL;
    mstring i_SVSNOOP;
    mstring i_SQLINE;
    mstring i_UNSQLINE;
    mstring i_SVSHOST;

    // This is a map of real commands -> tokenized commands
    // to save bandwidth.
    map < mstring, mstring > tokens;

public:
    Protocol()
    {
    }
    ~Protocol()
    {
    }
    bool Set(const mstring & filename);
    mstring GetToken(const mstring & in) const;
    mstring GetNonToken(const mstring & in) const;

    class Numeric_t
    {
	friend class Protocol;

	bool i_Trim:1;		/* Trim numeric results ... */
	bool i_ServerNumber:1;	/* Use decimal number (not base64) in SERVER line */
	bool i_Combine:1;	/* Combine server and nick numeric in messages */
	bool i_Prefix:1;	/* All messages are prefixed with the server numeric */
	bool i_UnrealNumeric:1;	/* The numeric for our neighbour is in the description */
	int i_Server;		/* Length of server numerics ... */
	int i_Field;		/* Field in SERVER line that contains numeric */
	int i_User;		/* Length of nickname numerics ... */
	int i_Channel;		/* Length of channel numerics ... */

	char base64_to_char[64], char_to_base64[256];

	unsigned long str_to_base64(const mstring & in) const;
	mstring base64_to_str(unsigned long in) const;

    public:
	Numeric_t() : i_Trim(false), i_ServerNumber(false), i_Combine(false), i_Prefix(false), i_UnrealNumeric(false),
	    i_Server(0), i_Field(0), i_User(0), i_Channel(false)
	{
	    memset(base64_to_char, 0, sizeof(base64_to_char));
	    memset(char_to_base64, 0, sizeof(char_to_base64));
	}
	bool Trim() const
	{
	    return i_Trim;
	}
	bool ServerNumber() const
	{
	    return i_ServerNumber;
	}
	bool Combine() const
	{
	    return i_Combine;
	}
	bool Prefix() const
	{
	    return i_Prefix;
	}
	bool UnrealNumeric() const
	{
	    return i_UnrealNumeric;
	}
	int Server() const
	{
	    return i_Server;
	}
	int Field() const
	{
	    return i_Field;
	}
	int User() const
	{
	    return i_User;
	}
	int Channel() const
	{
	    return i_Channel;
	}

	unsigned long ServerNumeric(const mstring & in) const;
	mstring ServerNumeric(unsigned long in) const;
	unsigned long UserNumeric(const mstring & in) const;
	mstring UserNumeric(unsigned long in) const;
	unsigned long ChannelNumeric(const mstring & in) const;
	mstring ChannelNumeric(unsigned long in) const;

	mstring FindServerNumeric(unsigned long in) const;
	mstring FindUserNumeric(unsigned long in) const;
	mstring FindChannelNumeric(unsigned long in) const;

	unsigned long FindServerNumeric() const;
	unsigned long FindUserNumeric() const;
	unsigned long FindChannelNumeric() const;

	mstring ServerLineNumeric(unsigned long n) const;
	unsigned long ServerLineNumeric(const mstring & n) const;
    }
    Numeric;

    unsigned int NickLen() const
    {
	return i_NickLen;
    }
    unsigned int MaxLine() const
    {
	return i_MaxLine;
    }
    bool Globops() const
    {
	return i_Globops;
    }
    bool Helpops() const
    {
	return i_Helpops;
    }
    bool Chatops() const
    {
	return i_Helpops;
    }
    bool Tokens() const
    {
	return i_Tokens;
    }
    void Tokens(const bool in)
    {
	i_Tokens = in;
    }
    bool SJoin() const
    {
	return i_SJoin;
    }
    void SJoin(const bool in)
    {
	i_SJoin = in;
    }
    bool P12() const
    {
	return i_P12;
    }
    bool TSora() const
    {
	return i_TSora;
    }
    bool BigTopic() const
    {
	return i_BigTopic;
    }
    bool TopicJoin() const
    {
	return i_TopicJoin;
    }
    bool ServerModes() const
    {
	return i_ServerModes;
    }
    unsigned int Akill() const
    {
	return i_Akill;
    }
    unsigned int Signon() const
    {
	return i_Signon;
    }
    unsigned int Modes() const
    {
	return i_Modes;
    }
    mstring ServicesModes() const
    {
	return i_ServicesModes;
    }
    mstring ChanModeArg() const
    {
	return i_ChanModeArg;
    }
    mstring Server() const
    {
	return i_Server;
    }
    mstring SVSNICK() const
    {
	return i_SVSNICK;
    }
    mstring SVSMODE() const
    {
	return i_SVSMODE;
    }
    mstring SVSKILL() const
    {
	return i_SVSKILL;
    }
    mstring SVSNOOP() const
    {
	return i_SVSNOOP;
    }
    mstring SQLINE() const
    {
	return i_SQLINE;
    }
    mstring UNSQLINE() const
    {
	return i_UNSQLINE;
    }
    mstring SVSHOST() const
    {
	return i_SVSHOST;
    }
    mstring Burst() const
    {
	return i_Burst;
    }
    mstring EndBurst() const
    {
	return i_EndBurst;
    }
    mstring Protoctl() const
    {
	return i_Protoctl;
    }

    void DumpB() const;
    void DumpE() const;
};

class Server_t : public mUserDef, public ref_class
{
    mstring i_Name;
    mstring i_AltName;
    unsigned long i_Numeric;
    mstring i_Uplink;
    int i_Hops;
    mstring i_Description;
    long i_Ping;
    long i_Lag;
    bool i_Jupe;

    void defaults();

public:
    Server_t();
    Server_t(const Server_t & in) : mUserDef(in), ref_class()
    {
	*this = in;
    }
    Server_t(const mstring & name, const mstring & description, const unsigned long numeric = 0);
    Server_t(const mstring & name, const int hops, const mstring & description, const unsigned long numeric = 0);
    Server_t(const mstring & name, const mstring & uplink, const int hops, const mstring & description,
	     const unsigned long numeric = 0);
    ~Server_t()
    {
    }

    Server_t &operator=(const Server_t & in);
    bool operator==(const Server_t & in) const
    {
	return (i_Name == in.i_Name);
    }
    bool operator!=(const Server_t & in) const
    {
	return (i_Name == in.i_Name);
    }
    bool operator<(const Server_t & in) const
    {
	return (i_Name < in.i_Name);
    }

    mstring Name() const
    {
	return i_Name;
    }
    mstring AltName() const;
    void AltName(const mstring & in);
    unsigned long Numeric() const;
    void Numeric(const unsigned long num);
    mstring Uplink() const;
    int Hops() const;
    mstring Description() const;
    void Ping();
    void Pong();
    float Lag() const;
    bool Jupe() const;
    unsigned int Users() const;
    unsigned int Opers() const;

    vector < mstring > Downlinks() const;
    vector < mstring > AllDownlinks() const;

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

class Server : public mBase
{
    friend class Magick;
    friend class Server_t;
    friend class Reconnect_Handler;
    friend class ToBeSquit_Handler;
    friend class Squit_Handler;
    friend int IrcSvcHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask);

    void raw(const mstring & send) const;
    void sraw(const mstring & send) const;
    void nraw(const mstring & nick, const mstring & send) const;

    set < mstring > WaitIsOn;

    size_t i_UserMax;
    map < mstring, long > ServerSquit;

    map < mstring, list < mstring > > ToBeSquit;
    ToBeSquit_Handler tobesquit;
    Squit_Handler squit;
    mstring i_OurUplink;

    enum send_type
    {
	t_GLOBOPS, t_HELPOPS, t_CHATOPS, t_INVITE, t_KICK, t_KILL,
	t_NOTICE, t_PRIVMSG, t_SQLINE, t_SVSMODE, t_SVSNICK,
	t_SVSKILL, t_SVSHOST, t_TOPIC, t_UNSQLINE, t_WALLOPS
    };
    map < mstring, list < triplet < send_type, mDateTime, triplet < mstring, mstring, mstring > > > > ToBeSent;
    void FlushMsgs(const mstring & nick);

public:
    typedef map < mstring, Server_t * > list_t;

private:

    list_t i_list;

    void OurUplink(const mstring & server);

public:
    Server();
    ~Server()
    {
    }
    void SignOnAll();
    void SignOffAll(const mstring & reason = "");
    Protocol proto;
    size_t UserMax() const;

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddList(Server_t * in) throw(E_Server_List);
    void AddList(const Server_t & in) throw(E_Server_List)
    {
	AddList(new Server_t(in));
    }
    void AddList(const map_entry < Server_t > & in) throw(E_Server_List)
    {
	AddList(in.entry());
    }
    map_entry < Server_t > GetList(const mstring & in) const throw(E_Server_List);
    void RemList(const mstring & in, bool downlinks = true) throw(E_Server_List);
#else
    void AddList(Server_t * in);
    void AddList(const Server_t & in)
    {
	AddList(new Server_t(in));
    }
    void AddList(const map_entry < Server_t > & in)
    {
	AddList(in.entry());
    }
    map_entry < Server_t > GetList(const mstring & in);
    void RemList(const mstring & in, bool downlinks = true);
#endif
    list_t::iterator ListBegin()
    {
	return i_list.begin();
    }
    list_t::iterator ListEnd()
    {
	return i_list.end();
    }
    list_t::const_iterator ListBegin() const
    {
	return i_list.begin();
    }
    list_t::const_iterator ListEnd() const
    {
	return i_list.end();
    }
    size_t ListSize() const
    {
	return i_list.size();
    }
    bool IsList(const mstring & server) const;

    mstring OurUplink() const;
    mstring GetServer(const mstring & server) const;
    mstring GetUser(const mstring & user) const;
    mstring GetChannel(const mstring & channel) const;

    // NOTE: This is NOT always accurate -- all it does is look
    // to see if there is a timer active to process the server's
    // squit, REGARDLESS of wether it is currently connected or not.
    bool IsSquit(const mstring & server) const;
    void Jupe(const mstring & server, const mstring & reason);

    void AKILL(const mstring & host, const mstring & reason = "", const unsigned long time = 0, const mstring & killer = "");
    void ANONKILL(const mstring & nick, const mstring & dest, const mstring & reason);
    void AWAY(const mstring & nick, const mstring & reason = "");
    void GLOBOPS(const mstring & nick, const mstring & message);
    void HELPOPS(const mstring & nick, const mstring & message);
    void CHATOPS(const mstring & nick, const mstring & message);
    void INVITE(const mstring & nick, const mstring & dest, const mstring & channel);
    void JOIN(const mstring & nick, const mstring & channel);
    void KICK(const mstring & nick, const mstring & dest, const mstring & channel, const mstring & reason = "");
    void KILL(const mstring & nick, const mstring & dest, const mstring & reason);
    void MODE(const mstring & nick, const mstring & mode);
    void MODE(const mstring & nick, const mstring & channel, const mstring & mode);
    void NICK(const mstring & nick, const mstring & user, const mstring & host, const mstring & server,
	      const mstring & realname);
    void NICK(const mstring & oldnick, const mstring & newnick);
    void NOTICE(const mstring & nick, const mstring & dest, const mstring & text);
    void PART(const mstring & nick, const mstring & channel, const mstring & reason = "");
    void PRIVMSG(const mstring & nick, const mstring & dest, const mstring & text);
    void QUIT(const mstring & nick, const mstring & reason = "");
    void SQLINE(const mstring & nick, const mstring & target, const mstring & reason = "");
    void RAKILL(const mstring & host);
    void SVSMODE(const mstring & mynick, const mstring & nick, const mstring & mode);
    void SVSNICK(const mstring & mynick, const mstring & nick, const mstring & newnick);
    void SVSNOOP(const mstring & nick, const mstring & server, const bool onoff);
    void SVSKILL(const mstring & mynick, const mstring & nick, const mstring & reason);
    void SVSHOST(const mstring & mynick, const mstring & nick, const mstring & newhost);
    void TOPIC(const mstring & nick, const mstring & setter, const mstring & channel, const mstring & topic =
	       "", const mDateTime & time = mDateTime::CurrentDateTime());
    void UNSQLINE(const mstring & nick, const mstring & target);
    void WALLOPS(const mstring & nick, const mstring & message);
    void KillUnknownUser(const mstring & user) const;
    unsigned int SeenMessage(const mstring & data);

    threadtype_enum Get_TType() const
    {
	return tt_ServNet;
    }
    mstring GetInternalName() const
    {
	return "Server";
    }
    void execute(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_A(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_B(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_C(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_D(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_E(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_F(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_G(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_H(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_I(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_J(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_K(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_L(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_M(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_N(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_O(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_P(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_Q(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_R(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_S(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_T(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_U(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_V(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_W(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_X(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_Y(mstring & source, const mstring & msgtype, const mstring & params);
    void parse_Z(mstring & source, const mstring & msgtype, const mstring & params);
    void numeric_execute(mstring & source, const mstring & msgtype, const mstring & params);

    void DumpB() const;
    void DumpE() const;
};

#endif
