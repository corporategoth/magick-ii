#ifndef WIN32
#pragma interface
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
#ifndef _SERVER_H
#define _SERVER_H
static const char *ident_server_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.33  2000/04/02 13:06:03  prez
** Fixed the channel TOPIC and MODE LOCK stuff ...
**
** Also fixed the setting of both on join...
**
** Revision 1.32  2000/03/15 14:42:58  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.31  2000/03/15 08:23:51  prez
** Added locking stuff for commserv options, and other stuff
**
** Revision 1.30  2000/03/14 13:37:35  prez
** *** empty log message ***
**
** Revision 1.29  2000/03/14 10:05:16  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.28  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.27  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#include "mstring.h"
#include "variant.h"
#include "base.h"
#include "mstream.h"
#include "ircsocket.h"

class Protocol
{
    unsigned int i_Number;
    map<mstring,mstring> tokens;

    bool i_Globops;
    bool i_Tokens;
    bool i_SVS;
    bool i_SVSHOST;
    bool i_P12;
    
    /* AKILL types
     *
     * 0 = none
     * 1 = AKILL host user :reason
     *     RAKILL host user
     * 2 = GLINE * +time user@host :reason
     *     UNGLINE * user@host
     * 3 = GLINE * +user@host time :reason
     *     GLINE * -user@host
     * 4 = GLINE +user@host time :reason
     *     GLINE -user@host
     */
    unsigned int i_Akill;

    /* Signon Types
     * 
     * 0000 = USER nick user host server :realname
     * 0001 = USER nick signon-time user host server :realname
     * 
     * 1000 = NICK nick hops signon-time user host server :realname
     * 1001 = NICK nick hops signon-time user host server service :realname
     * 1002 = NICK nick hops signon-time user host server service host :realname
     */
    unsigned int i_Signon;
    unsigned int i_Modes; /* Modes per line */

    mstring i_Server;	/* Should have %s %d %s in it */

    /* PROTOCTL identifiers
     *
     * NOQUIT      Do not quit each user on SQUIT (assume it)
     * TOKEN       Use one-char tokens instead of PRIVMSG, etc
     * WATCH=128   .
     * SAFELIST    .
     */
    mstring i_Protoctl; /* Verbatum (null if not sent) */
public:
    Protocol();
    ~Protocol() {}
    void Set(unsigned int in);
    mstring GetToken(mstring in);

    unsigned int Number()   { return i_Number; }
    bool Globops()	    { return i_Globops; }
    bool Tokens()	    { return i_Tokens; }
    bool SVS()		    { return i_SVS; }
    bool SVSHOST()	    { return i_SVSHOST; }
    bool P12()		    { return i_P12; }
    unsigned int Akill()    { return i_Akill; }
    unsigned int Signon()   { return i_Signon; }
    unsigned int Modes()    { return i_Modes; }
    mstring Server()	    { return i_Server; }
    mstring Protoctl()	    { return i_Protoctl; }
};

class Server
{
    mstring i_Name;
    mstring i_Uplink;
    int i_Hops;
    mstring i_Description;
    long i_Ping;
    long i_Lag;
    bool i_Jupe;
public:
    Server() {}
    Server(const Server &in) { *this = in; }
    Server(mstring name, mstring description);
    Server(mstring name, int hops, mstring description);
    Server(mstring name, mstring uplink, int hops, mstring description);
    void operator=(const Server &in);
    bool operator==(const Server &in) const
	{ return (i_Name == in.i_Name); }
    bool operator!=(const Server &in) const
	{ return (i_Name == in.i_Name); }
    bool operator<(const Server &in) const
	{ return (i_Name < in.i_Name); }

    mstring Name()		{ return i_Name; }
    mstring Uplink()		{ return i_Uplink; }
    int Hops()			{ return i_Hops; }
    mstring Description()	{ return i_Description; }
    void Ping();
    void Pong();
    float Lag()			{ return i_Lag / 1000.0; }
    bool Jupe()			{ return i_Jupe; }
    unsigned int Users();
    unsigned int Opers();

    vector<mstring> Downlinks();
    vector<mstring> AllDownlinks();
    
    ~Server();
};

class NetworkServ : public mBase
{
    friend class Server;
    friend class Magick;
    friend class Reconnect_Handler;
    friend class ToBeSquit_Handler;
    friend class Squit_Handler;
private:
    void raw(mstring send);
    void sraw(mstring send);
    void SignOnAll();
    set<mstring> WaitIsOn;

    size_t i_UserMax;
    map<mstring,long> ServerSquit;
    map<mstring,list<mstring> > ToBeSquit;
    ToBeSquit_Handler tobesquit;
    Squit_Handler squit;
    mstring i_OurUplink;
    
    enum send_type {
	t_GLOBOPS, t_INVITE, t_KICK, t_KILL, t_NOTICE,
	t_PRIVMSG, t_QLINE, t_SVSMODE, t_SVSNICK,
	t_SVSKILL, t_TOPIC, t_UNQLINE, t_WALLOPS };
    map<mstring, list<triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> > > > ToBeSent;
    void FlushMsgs(mstring nick);

    void OurUplink(mstring server) { i_OurUplink = server; }
public:
    Protocol proto;
    size_t UserMax() { return i_UserMax; }
    map<mstring,Server> ServerList;
    mstring OurUplink() { return i_OurUplink; }
    bool IsServer(mstring server);
    // NOTE: This is NOT always accurate -- all it does is look
    // to see if there is a timer active to process the server's
    // squit, REGARDLESS of wether it is currently connected or not.
    bool IsSquit(mstring server);
    void Jupe(mstring server, mstring reason);

    void AKILL(mstring host, mstring reason = "", unsigned long time = 0);
    void RAKILL(mstring host);

    void AWAY(mstring nick, mstring reason = "");
    void GLOBOPS(mstring nick, mstring message);
    void INVITE(mstring nick, mstring dest, mstring channel);
    void JOIN(mstring nick, mstring channel);
    void KICK(mstring nick, mstring dest, mstring channel, mstring reason = "");
    void KILL(mstring nick, mstring dest, mstring reason);
    void MODE(mstring nick, mstring mode);
    void MODE(mstring nick, mstring channel, mstring mode);
    void NICK(mstring nick, mstring user, mstring host,
    	mstring server, mstring realname);
    void NICK(mstring oldnick, mstring newnick);
    void NOTICE(mstring nick, mstring dest, mstring text);
    void NOOP(mstring nick, mstring server, bool onoff);
    void PART(mstring nick, mstring channel, mstring reason = "");
    void PRIVMSG(mstring nick, mstring dest, mstring text);
    void QUIT(mstring nick, mstring reason = "");
    void QLINE(mstring nick, mstring target, mstring reason = "");
    void SVSMODE(mstring mynick, mstring nick, mstring mode);
    void SVSNICK(mstring mynick, mstring nick, mstring newnick);
    void SVSKILL(mstring mynick, mstring nick, mstring reason);
    void TOPIC(mstring nick, mstring setter, mstring channel, mstring topic = "", mDateTime time = Now());
    void UNQLINE(mstring nick, mstring target);
    void WALLOPS(mstring nick, mstring message);    
    void KillUnknownUser(mstring user);

    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);
    NetworkServ();
    virtual threadtype_enum Get_TType() const { return tt_ServNet; }
    virtual mstring GetInternalName() const { return "NetworkServ"; }
    virtual void execute(const mstring & message);
    void numeric_execute(const mstring & message);
};

#endif
