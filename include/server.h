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
#ifndef _server_h
#define _server_h

#include "mstring.h"
#include "variant.h"
#include "base.h"
#include "mstream.h"
#include "ircsocket.h"

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

    map<mstring,long> ServerSquit;
    map<mstring,list<mstring> > ToBeSquit;
    ToBeSquit_Handler tobesquit;
    Squit_Handler squit;
    mstring i_OurUplink;

    void OurUplink(mstring server) { i_OurUplink = server; }
public:
    map<mstring,Server> ServerList;
    mstring OurUplink() { return i_OurUplink; }
    bool IsServer(mstring server);
    // NOTE: This is NOT always accurate -- all it does is look
    // to see if there is a timer active to process the server's
    // squit, REGARDLESS of wether it is currently connected or not.
    bool IsSquit(mstring server);
    void Jupe(mstring server, mstring reason);

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
    void NOTICE(mstring nick, mstring dest, mstring text);
    void PART(mstring nick, mstring channel, mstring reason = "");
    void PRIVMSG(mstring nick, mstring dest, mstring text);
    void QUIT(mstring nick, mstring reason = "");
    void SVSMODE(mstring mynick, mstring nick, mstring mode);
    void SVSNICK(mstring mynick, mstring nick, mstring newnick);
    void SVSKILL(mstring mynick, mstring nick, mstring reason);
    void TOPIC(mstring nick, mstring channel, mstring topic = "");
    void WALLOPS(mstring nick, mstring message);    

    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);
    NetworkServ();
    virtual threadtype_enum Get_TType() const { return tt_ServNet; }
    virtual mstring GetInternalName() const { return "NetworkServ"; }
    virtual void execute(const mstring & message);
    void numeric_execute(const mstring & message);
};

#endif
