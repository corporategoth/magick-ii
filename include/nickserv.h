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
#ifndef _nickserv_h
#define _nickserv_h

#include "mstring.h"
#include "variant.h"
#include "base.h"
#include "mstream.h"
#include "ircsocket.h"

class Nick_Live_t : public mUserDef
{
    mstring i_Name;
    mDateTime i_Signon_Time;
    mDateTime i_My_Signon_Time;
    mDateTime i_Last_Action;
    mstring i_realname;
    mstring i_user;
    mstring i_host;
    mstring i_server;
    mstring i_squit;
    mstring i_away;
    mstring modes;
    set<mstring> joined_channels;
    vector<time_t> last_msg_times;
    int last_msg_entries;
    int flood_triggered_times;
    int failed_passwds;
    set<mstring> chans_founder_identd;
    vector<mstring> try_chan_ident;
    bool identified;
    bool services;
public:
    Nick_Live_t();
    Nick_Live_t(const Nick_Live_t &in) { *this = in; }
    Nick_Live_t(mstring name, mDateTime signon, mstring server,
	    mstring username, mstring hostname, mstring realname);
    Nick_Live_t(mstring name, mstring username, mstring hostname,
	    mstring realname); // Services ONLY
    void operator=(const Nick_Live_t &in);
    bool operator==(const Nick_Live_t &in) const
    	{ return (i_Name == in.i_Name); }
    bool operator!=(const Nick_Live_t &in) const
    	{ return (i_Name != in.i_Name); }
    bool operator<(const Nick_Live_t &in) const
    	{ return (i_Name < in.i_Name); }

    // channel maintinance
    void Join(mstring channel);
    void Part(mstring channel);
    void Kick(mstring kicker, mstring channel);
    void Quit(mstring reason);
    bool IsInChan(mstring channel);
    set<mstring> Channels() { return joined_channels; }

    // true if user ignored
    bool FloodTrigger();

    // Data maintinance
    void Name(mstring in);
    mstring Name() { return i_Name; }

    void SendMode(mstring source, mstring in);
    void Mode(mstring in);
    mstring Mode() { return modes; }
    bool HasMode(mstring in) { return modes.Contains(in); }

    void Away(mstring in)	{ i_away = in; };
    mstring Away()		{ return i_away; };

    mDateTime IdleTime();
    void Action();

    mDateTime SignonTime()	{ return i_Signon_Time; }
    mstring RealName()		{ return i_realname; }
    mstring User()		{ return i_user; }
    mstring Host()		{ return i_host; }
    mstring Server()		{ return i_server; } 
    void SetSquit();
    void ClearSquit();
    mstring Squit()		{ return i_squit; }

    enum styles {
    	N = 1,		// nick!*@*
    	N_U_P_H,	// nick!user@port.host
    	N_U_H,		// nick!user@*.host
    	N_P_H,		// nick!*@port.host
    	N_H,		// nick!*@*.host
    	U_P_H,		// *!user@port.host
    	U_H,		// *!user@*.host
    	P_H,		// *!*@port.host
    	H		// *!*@*.host
    	};

    mstring Mask(styles type);

    // Will KILL user if >PassFailMax
    mstring ChanIdentify(mstring channel, mstring password);
    bool IsChanIdentified(mstring channel);
    mstring Identify(mstring password);
    bool IsIdentified()		{ return identified; }
    bool IsRecognized();
    bool IsServices()		{ return (i_server == ""); }
};

class Nick_Stored_t : public mUserDef
{
    friend class Nick_Live_t;
    friend wxOutputStream &operator<<(wxOutputStream& out,Nick_Stored_t& in);
    friend wxInputStream &operator>>(wxInputStream& in, Nick_Stored_t& out);
    friend class NickServ;

    mstring i_Name;
    mDateTime i_RegTime;
    mstring i_Password;
    mstring i_Email;
    mstring i_URL;
    mstring i_ICQ;
    mstring i_Description;
    mstring i_Comment;
    // i_Host should not be needed or used, we have a map there, 
    // mstring var and use the map instead. that way we have no stale pointers
    // slaps prez's wrist *again*, eliminate all unnecessary pointers
    // and we eliminate 99% of our sigsegv's
    Nick_Stored_t *i_Host;
    set<mstring> i_slaves; // HOST only
    set<mstring> i_access;
    set<mstring> i_ignore;
    bool i_Protect;
    bool i_Secure;
    bool i_NoExpire;
    bool i_NoMemo;
    bool i_Private;
    bool i_PRIVMSG;
    bool i_Forbidden;

    mstring i_Suspend_By;
    mDateTime i_Suspend_Time;

    // offline
    mDateTime i_LastSeenTime;
    mstring i_LastRealName;
    mstring i_LastMask;
    mstring i_LastQuit;

    void Signon(mstring realname, mstring mask);
    void ChgNick(mstring nick);
public:
    Nick_Stored_t();
    Nick_Stored_t(const Nick_Stored_t &in) { *this = in; }
    Nick_Stored_t(mstring nick, mstring password);
    Nick_Stored_t(mstring nick, const Nick_Stored_t &host); // Slave
    Nick_Stored_t(mstring nick); // Services Only (forbidden)
    ~Nick_Stored_t();
    void operator=(const Nick_Stored_t &in);
    bool operator==(const Nick_Stored_t &in) const
	{ return (i_Name == in.i_Name); }
    bool operator!=(const Nick_Stored_t &in) const
	{ return (i_Name != in.i_Name); }
    bool operator<(const Nick_Stored_t &in) const
	{ return (i_Name < in.i_Name); }

    mstring Name() { return i_Name; }
    mDateTime RegTime() { return i_RegTime; }

    mstring Password();
    void Password(mstring in);
    mstring Email();
    void Email(mstring in);
    mstring URL();
    void URL(mstring in);
    mstring ICQ();
    void ICQ(mstring in);
    mstring Description();
    void Description(mstring in);
    mstring Comment();
    void Comment(mstring in);

    void Suspend(mstring name, mstring reason)
	{ Comment(reason); Suspend(name); }
    void Suspend(mstring name);
    void UnSuspend();

    Nick_Stored_t *Host() { return i_Host; }
    bool Slave(mstring nick, mstring password);
    int Siblings();
    mstring Sibling(int count);
    bool IsSibling(mstring nick);
    bool MakeHost();
    bool Link(mstring nick, mstring password);
    bool Unlink();

    int Access();
    mstring Access(int count);
    bool AccessAdd(const mstring& in);
    int AccessDel(mstring in);
    int AccessDel(int in)
	{ return AccessDel(Access(in)); }
    bool IsAccess(mstring in);

    int Ignore();
    mstring Ignore(int count);
    bool IgnoreAdd(mstring in);
    int IgnoreDel(mstring in);
    int IgnoreDel(int in)
	{ return IgnoreDel(Ignore(in)); }
    bool IsIgnore(mstring in);

    // flags
    bool Protect();
    void Protect(bool in);
    bool Secure();
    void Secure(bool in);
    bool NoExpire();
    void NoExpire(bool in);
    bool NoMemo();
    void NoMemo(bool in);
    bool Private();
    void Private(bool in);
    bool PRIVMSG();
    void PRIVMSG(bool in);
    bool Suspended();
    mstring Suspend_By();
    mDateTime Suspend_Time();
    bool Forbidden() { return i_Forbidden; }

    // IF Online, returns live realname and ONLINE.
    // IF non-slave and Offline, returns local data.
    // IF slave and Offline, returns host data.
    bool IsOnline();
    mDateTime LastAllSeenTime();
    mDateTime LastSeenTime();
    mstring LastRealName();
    mstring LastAllMask();
    mstring LastMask();
    mstring LastQuit();
    void Quit(mstring message);
};
wxOutputStream &operator<<(wxOutputStream& out,Nick_Stored_t& in);
wxInputStream &operator>>(wxInputStream& in, Nick_Stored_t& out);

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class NickServ : public mBase
{
    friend class Magick;
private:

    // Config Entries ...
    int expire;			// How long to keep nicknames
    int ident;			// How long to wait for IDENT
    int release;		// How long to keep after failed ident
    int passfail;		// Number of password fails before kill
    bool def_protect;		// Default val of PROTECT
    bool lck_protect;		// PROTECT is locked?
    bool def_secure;		// Default val of SECURE
    bool lck_secure;		// SECURE is locked?
    bool def_noexpire;		// Default val of NOEXPIRE
    bool lck_noexpire;		// NOEXPIRE is locked?
    bool def_nomemo;		// Default val of NOMEMO
    bool lck_nomemo;		// NOMEMO is locked?
    bool def_private;		// Default val of PRIVATE
    bool lck_private;		// PRIVATE is locked?
    bool def_privmsg;		// Default val of PRIVMSG
    bool lck_privmsg;		// PRIVMSG is locked?

public:
    int Expire()	{ return expire; }
    int Ident()		{ return ident; }
    int Release()	{ return release; }
    int Passfail()	{ return passfail; }
    bool DEF_Protect()	{ return def_protect; }
    bool LCK_Protect()	{ return lck_protect; }
    bool DEF_Secure()	{ return def_secure; }
    bool LCK_Secure()	{ return lck_secure; }
    bool DEF_NoExpire()	{ return def_noexpire; }
    bool LCK_NoExpire()	{ return lck_noexpire; }
    bool DEF_NoMemo()	{ return def_nomemo; }
    bool LCK_NoMemo()	{ return lck_nomemo; }
    bool DEF_Private()	{ return def_private; }
    bool LCK_Private()	{ return lck_private; }
    bool DEF_PRIVMSG()	{ return def_privmsg; }
    bool LCK_PRIVMSG()	{ return lck_privmsg; }

    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);
    bool IsStored(mstring in);
    bool IsLive(mstring in);
    map<mstring,Nick_Stored_t> stored;
    map<mstring,Nick_Live_t> live;
    KillOnSignon_Handler kosh;

    NickServ();
    virtual threadtype_enum Get_TType() const { return tt_NickServ; }
    virtual mstring GetInternalName() const { return "NickServ"; }
    virtual void execute(const mstring & message);
};
#endif
