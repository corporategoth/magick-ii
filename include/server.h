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
#include "pch.h"
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
** Revision 1.47  2000/08/19 10:59:46  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.46  2000/08/06 05:27:46  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.45  2000/08/02 20:08:56  prez
** Minor code cleanups, added ACE installation instructions, updated the
** suggestions file and stopped people doing a whole bunch of stuff to
** forbidden nicknames.
**
** Revision 1.44  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.43  2000/06/25 07:58:48  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.42  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.41  2000/06/12 06:07:49  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.40  2000/06/11 09:30:20  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.39  2000/06/10 07:01:02  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.38  2000/06/06 08:57:54  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.37  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.36  2000/04/04 03:21:34  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.35  2000/04/04 03:13:50  prez
** Added support for masking hostnames.
**
** Revision 1.34  2000/04/03 09:45:21  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
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
#include "ircsocket.h"

class Protocol
{
    // Protocol number
    unsigned int i_Number;
    unsigned int i_NickLen;
    // Max length of line excluding nickname / comamnd.
    // ie. reasonable max length (def: 512 - 62 = 450).
    unsigned int i_MaxLine;
    map<mstring,mstring> tokens;

    bool i_Globops;
    bool i_Tokens;
    bool i_SVS;
    bool i_SVSHOST;
    bool i_P12;
    bool i_TSora;

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
     * 5 = AKILL host user time killer now :reason
     *     RAKILL host user
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
     * 1004 = NICK nick hops signon-time mode user host server service :realname
     */
    unsigned int i_Signon;
    unsigned int i_Modes; /* Modes per line */
    mstring i_ChanModeArg; /* Channel Modes that have arguments */

    mstring i_Server;	/* Should have %s %d %s in it (in order) */

    /* PROTOCTL and CAPAB identifiers
     *
     * NOQUIT      Do not quit each user on SQUIT (assume it)
     * TOKEN       Use one-char tokens instead of PRIVMSG, etc
     * WATCH=128   ?
     * SAFELIST    ?
     * TS3         Timestamp everything (aka. TSora)
     * SSJOIN      Use SJOIN as opposed to multi JOINS/modes
     * BURST       Allow the BURST keyword (to indicate sync)
     * UNCONNECT   ?
     */
    mstring i_Protoctl; /* Verbatum (null if not sent) */
public:
    Protocol();
    ~Protocol() {}
    void Set(unsigned int in);
    mstring GetToken(mstring in);
    mstring GetNonToken(mstring in);

    unsigned int Number()   { return i_Number; }
    unsigned int NickLen()  { return i_NickLen; }
    unsigned int MaxLine()  { return i_MaxLine; }
    bool Globops()	    { return i_Globops; }
    bool Tokens()	    { return i_Tokens; }
    void Tokens(bool in)    { i_Tokens = in; }
    bool SVS()		    { return i_SVS; }
    bool SVSHOST()	    { return i_SVSHOST; }
    bool P12()		    { return i_P12; }
    bool TSora()	    { return i_TSora; }
    unsigned int Akill()    { return i_Akill; }
    unsigned int Signon()   { return i_Signon; }
    unsigned int Modes()    { return i_Modes; }
    mstring ChanModeArg()   { return i_ChanModeArg; }
    mstring Server()	    { return i_Server; }
    mstring Protoctl()	    { return i_Protoctl; }
};

class Server
{
    mstring i_Name;
    mstring i_AltName;
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
    mstring AltName();
    void AltName(mstring in);
    mstring Uplink();
    int Hops();
    mstring Description();
    void Ping();
    void Pong();
    float Lag();
    bool Jupe();
    unsigned int Users();
    unsigned int Opers();

    vector<mstring> Downlinks();
    vector<mstring> AllDownlinks();
    
    ~Server();

    size_t Usage();
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
    set<mstring> WaitIsOn;
    map<mstring, pair<unsigned int, mDateTime> > ReDoMessages;

    size_t i_UserMax;
    map<mstring,long> ServerSquit;
    map<mstring,list<mstring> > ToBeSquit;
    ToBeSquit_Handler tobesquit;
    Squit_Handler squit;
    mstring i_OurUplink;
    
    enum send_type {
	t_GLOBOPS, t_INVITE, t_KICK, t_KILL, t_NOTICE,
	t_PRIVMSG, t_QLINE, t_SVSMODE, t_SVSNICK,
	t_SVSKILL, t_SVSHOST, t_TOPIC, t_UNQLINE, t_WALLOPS };
    map<mstring, list<triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> > > > ToBeSent;
    void FlushMsgs(mstring nick);

    void OurUplink(mstring server);
public:
    void SignOnAll();
    Protocol proto;
    size_t UserMax();
    map<mstring,Server> ServerList;
    mstring OurUplink();
    bool IsServer(mstring server);
    // NOTE: This is NOT always accurate -- all it does is look
    // to see if there is a timer active to process the server's
    // squit, REGARDLESS of wether it is currently connected or not.
    bool IsSquit(mstring server);
    void Jupe(mstring server, mstring reason);

    void AKILL(mstring host, mstring reason = "", unsigned long time = 0, mstring killer = "");
    void ANONKILL(mstring nick, mstring dest, mstring reason);
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
    void RAKILL(mstring host);
    void SVSMODE(mstring mynick, mstring nick, mstring mode);
    void SVSNICK(mstring mynick, mstring nick, mstring newnick);
    void SVSKILL(mstring mynick, mstring nick, mstring reason);
    void SVSHOST(mstring mynick, mstring nick, mstring newhost);
    void TOPIC(mstring nick, mstring setter, mstring channel, mstring topic = "", mDateTime time = Now());
    void UNQLINE(mstring nick, mstring target);
    void WALLOPS(mstring nick, mstring message);    
    void KillUnknownUser(mstring user);
    unsigned int SeenMessage(mstring data);

    NetworkServ();
    virtual threadtype_enum Get_TType() const { return tt_ServNet; }
    virtual mstring GetInternalName() const { return "NetworkServ"; }
    virtual void execute(const mstring & message);
    void numeric_execute(const mstring & message);
};

#endif
