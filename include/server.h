#ifndef WIN32
#pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
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
RCSID(server_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.66  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.65  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.64  2001/03/27 07:04:30  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.63  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.62  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.61  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.59  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.55  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.54  2000/12/22 08:55:40  prez
** Made forbidden entries (chanserv or nickserv) show up as forbidden in
** a list (rather than (nick!) or whatever)
**
** Revision 1.53  2000/12/19 14:26:55  prez
** Bahamut has changed SVSNICK -> MODNICK, so i_SVS has been changed into
** several SVS command text strings, if blank, support isnt there.
**
** Revision 1.52  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.51  2000/09/27 11:21:37  prez
** Added a BURST mode ...
**
** Revision 1.50  2000/09/10 09:53:42  prez
** Added functionality to ensure the order of messages is kept.
**
** Revision 1.49  2000/09/07 08:13:17  prez
** Fixed some of the erronous messages (SVSHOST, SQLINE, etc).
** Also added CPU statistics and fixed problem with socket deletions.
**
** Revision 1.48  2000/09/02 07:20:44  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
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

    bool i_Globops;
    bool i_Helpops;
    bool i_Tokens;
    bool i_P12;
    bool i_TSora;

    /* AKILL types
     *
     * 0000 = KILL user :reason
     * 1000 = AKILL host user :reason
     *        RAKILL host user
     * 1001 = AKILL host user time killer now :reason
     *        RAKILL host user
     * 2000 = GLINE * +time user@host :reason
     *        UNGLINE * user@host
     * 2001 = GLINE * +user@host time :reason
     *        GLINE * -user@host
     * 2002 = GLINE +user@host time :reason
     *        GLINE -user@host
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
     */
    unsigned int i_Signon;
    unsigned int i_Modes; /* Modes per line */
    mstring i_ChanModeArg; /* Channel Modes that have arguments */

    mstring i_Server;	/* Should have %s %d %s in it (in order) */
    int i_Numeric;	/* # paramater that is numeric! */
    mstring i_Burst;	/* Simply do we need to announce a flood? */
    mstring i_EndBurst; /* and if we do, how do we tell em we're done */

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
     */

    mstring i_Protoctl; /* Verbatum (null if not sent) */

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
    map<mstring,mstring> tokens;
    void SetTokens(unsigned int type);

public:
    Protocol();
    ~Protocol() {}
    void Set(const unsigned int in);
    mstring GetToken(const mstring& in) const;
    mstring GetNonToken(const mstring& in) const;

    unsigned int Number() const   { return i_Number; }
    unsigned int NickLen() const  { return i_NickLen; }
    unsigned int MaxLine() const  { return i_MaxLine; }
    bool Globops() const	  { return i_Globops; }
    bool Helpops() const	  { return i_Helpops; }
    bool Tokens() const		  { return i_Tokens; }
    void Tokens(const bool in)	  { i_Tokens = in; }
    bool P12() const		  { return i_P12; }
    bool TSora() const		  { return i_TSora; }
    unsigned int Akill() const    { return i_Akill; }
    unsigned int Signon() const   { return i_Signon; }
    unsigned int Modes() const    { return i_Modes; }
    mstring ChanModeArg() const   { return i_ChanModeArg; }
    mstring Server() const	  { return i_Server; }
    int Numeric() const		  { return i_Numeric; }
    mstring SVSNICK() const	  { return i_SVSNICK; }
    mstring SVSMODE() const	  { return i_SVSMODE; }
    mstring SVSKILL() const	  { return i_SVSKILL; }
    mstring SVSNOOP() const	  { return i_SVSNOOP; }
    mstring SQLINE() const	  { return i_SQLINE; }
    mstring UNSQLINE() const	  { return i_UNSQLINE; }
    mstring SVSHOST() const	  { return i_SVSHOST; }
    mstring Burst() const	  { return i_Burst; }
    mstring EndBurst() const	  { return i_EndBurst; }
    mstring Protoctl() const	  { return i_Protoctl; }

    void DumpB() const;
    void DumpE() const;
};

class Server_t
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
public:
    Server_t() {}
    Server_t(const Server_t &in) { *this = in; }
    Server_t(const mstring& name, const mstring& description,
    				const unsigned long numeric = 0);
    Server_t(const mstring& name, const int hops, const mstring& description,
				const unsigned long numeric = 0);
    Server_t(const mstring& name, const mstring& uplink, const int hops,
		const mstring& description, const unsigned long numeric = 0);
    void operator=(const Server_t &in);
    bool operator==(const Server_t &in) const
	{ return (i_Name == in.i_Name); }
    bool operator!=(const Server_t &in) const
	{ return (i_Name == in.i_Name); }
    bool operator<(const Server_t &in) const
	{ return (i_Name < in.i_Name); }

    mstring Name() const	{ return i_Name; }
    mstring AltName() const;
    void AltName(const mstring& in);
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

    vector<mstring> Downlinks() const;
    vector<mstring> AllDownlinks() const;
    
    ~Server_t();

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

    void raw(const mstring& send) const;
    void sraw(const mstring& send) const;
    set<mstring> WaitIsOn;
    map<mstring, pair<unsigned int, mDateTime> > ReDoMessages;

    size_t i_UserMax;
    map<mstring,long> ServerSquit;
    map<mstring,list<mstring> > ToBeSquit;
    ToBeSquit_Handler tobesquit;
    Squit_Handler squit;
    mstring i_OurUplink;
    unsigned int proc_SERVER, proc_NICK, proc_CHAN;
    
    enum send_type {
	t_GLOBOPS, t_HELPOPS, t_INVITE, t_KICK, t_KILL, t_NOTICE,
	t_PRIVMSG, t_SQLINE, t_SVSMODE, t_SVSNICK,
	t_SVSKILL, t_SVSHOST, t_TOPIC, t_UNSQLINE, t_WALLOPS };
    map<mstring, list<triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> > > > ToBeSent;
    void FlushMsgs(const mstring& nick);
    map<mstring, list<triplet<mDateTime, mstring, mstring> > > ToBeDone;

public:
    typedef map<mstring,Server_t> list_t;

private:

    list_t i_list;

    void OurUplink(const mstring& server);
public:
    Server();
    ~Server() {}
    void FlushUser(const mstring& nick, const mstring& channel = "");
    void PushUser(const mstring& nick, const mstring& message,
		const mstring& channel = "");
    void PopUser(const mstring& nick, const mstring& channel = "");
    void SignOnAll();
    Protocol proto;
    size_t UserMax() const;

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddList(Server_t *in) throw(E_Server_List);
    Server_t &GetList(const mstring &in) const throw(E_Server_List);
    void RemList(const mstring &in) throw(E_Server_List);
#else
    void AddServer(Server_t *in);
    Server_t &GetServer(const mstring &in);
    void RemServer(const mstring &in);
#endif
    list_t::iterator ListBegin() { return i_list.begin(); }
    list_t::iterator ListEnd() { return i_list.end(); }
    list_t::const_iterator ListBegin() const { return i_list.begin(); }
    list_t::const_iterator ListEnd() const { return i_list.end(); }
    size_t ListSize() const { return i_list.size(); }
    bool IsList(const mstring& server) const;
    mstring ServerNumeric(const unsigned long num) const;

    mstring OurUplink() const;
    mstring GetServer(const mstring& server) const;
    unsigned long GetOurNumeric() const;
    // NOTE: This is NOT always accurate -- all it does is look
    // to see if there is a timer active to process the server's
    // squit, REGARDLESS of wether it is currently connected or not.
    bool IsSquit(const mstring& server) const;
    void Jupe(const mstring& server, const mstring& reason);

    void AKILL(const mstring& host, const mstring& reason = "",
	const unsigned long time = 0, const mstring& killer = "");
    void ANONKILL(const mstring& nick, const mstring& dest, const mstring& reason);
    void AWAY(const mstring& nick, const mstring& reason = "");
    void GLOBOPS(const mstring& nick, const mstring& message);
    void HELPOPS(const mstring& nick, const mstring& message);
    void INVITE(const mstring& nick, const mstring& dest, const mstring& channel);
    void JOIN(const mstring& nick, const mstring& channel);
    void KICK(const mstring& nick, const mstring& dest,
	const mstring& channel, const mstring& reason = "");
    void KILL(const mstring& nick, const mstring& dest, const mstring& reason);
    void MODE(const mstring& nick, const mstring& mode);
    void MODE(const mstring& nick, const mstring& channel, const mstring& mode);
    void NICK(const mstring& nick, const mstring& user, const mstring& host,
    	const mstring& server, const mstring& realname);
    void NICK(const mstring& oldnick, const mstring& newnick);
    void NOTICE(const mstring& nick, const mstring& dest, const mstring& text);
    void PART(const mstring& nick, const mstring& channel,
	const mstring& reason = "");
    void PRIVMSG(const mstring& nick, const mstring& dest, const mstring& text);
    void QUIT(const mstring& nick, const mstring& reason = "");
    void SQLINE(const mstring& nick, const mstring& target,
	const mstring& reason = "");
    void RAKILL(const mstring& host);
    void SVSMODE(const mstring& mynick, const mstring& nick, const mstring& mode);
    void SVSNICK(const mstring& mynick, const mstring& nick, const mstring& newnick);
    void SVSNOOP(const mstring& nick, const mstring& server, const bool onoff);
    void SVSKILL(const mstring& mynick, const mstring& nick, const mstring& reason);
    void SVSHOST(const mstring& mynick, const mstring& nick, const mstring& newhost);
    void TOPIC(const mstring& nick, const mstring& setter,
	const mstring& channel, const mstring& topic = "",
	const mDateTime& time = mDateTime::CurrentDateTime());
    void UNSQLINE(const mstring& nick, const mstring& target);
    void WALLOPS(const mstring& nick, const mstring& message);
    void KillUnknownUser(const mstring& user) const;
    unsigned int SeenMessage(const mstring& data);

    threadtype_enum Get_TType() const { return tt_ServNet; }
    mstring GetInternalName() const { return "Server"; }
    void execute(const mstring & message);
    void parse_A(const mstring & data);
    void parse_B(const mstring & data);
    void parse_C(const mstring & data);
    void parse_D(const mstring & data);
    void parse_E(const mstring & data);
    void parse_F(const mstring & data);
    void parse_G(const mstring & data);
    void parse_H(const mstring & data);
    void parse_I(const mstring & data);
    void parse_J(const mstring & data);
    void parse_K(const mstring & data);
    void parse_L(const mstring & data);
    void parse_M(const mstring & data);
    void parse_N(const mstring & data);
    void parse_O(const mstring & data);
    void parse_P(const mstring & data);
    void parse_Q(const mstring & data);
    void parse_R(const mstring & data);
    void parse_S(const mstring & data);
    void parse_T(const mstring & data);
    void parse_U(const mstring & data);
    void parse_V(const mstring & data);
    void parse_W(const mstring & data);
    void parse_X(const mstring & data);
    void parse_Y(const mstring & data);
    void parse_Z(const mstring & data);
    void numeric_execute(const mstring & message);

    void DumpB() const;
    void DumpE() const;
};

#endif
