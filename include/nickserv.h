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
    unsigned int last_msg_entries;
    unsigned int flood_triggered_times;
    unsigned int failed_passwds;
    set<mstring> chans_founder_identd;
    vector<mstring> try_chan_ident;
    bool identified;
    bool services;

public:

    class InFlight_t {
	friend class Nick_Live_t;
	friend class DccEngine;
	friend class InFlight_Handler;

	mstring nick;
	bool memo;
	long timer;
	bool fileattach;
	bool fileinprog;
	mstring service;
	mstring sender;
	mstring recipiant;
	mstring text;

	InFlight_t() {}
	~InFlight_t();

	void ChgNick(mstring newnick);
	void operator=(const InFlight_t &in);
	void init();
	// Called upon completion
	// 0 means it failed
	void File(unsigned long filenum);
	// Called upon start
	void SetInProg();
    public:
	void Memo (bool file, mstring mynick, mstring recipiant, mstring message);
	void Continue(mstring message);
	void Cancel();
	void End(unsigned long filenum);
	void Picture (mstring mynick);
	bool IsMemo()	{ return memo; }
	bool Exists()	{ return (memo || fileattach); }
	bool File()	{ return fileattach; }
	bool InProg()	{ return fileinprog; }

    } InFlight;

    Nick_Live_t();
    Nick_Live_t(const Nick_Live_t &in)
	{ *this = in; }
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

    void SendMode(mstring in);
    void Mode(mstring in);
    mstring Mode() { return modes; }
    bool HasMode(mstring in) { return modes.Contains(in); }

    void Away(mstring in)	{ i_away = in; };
    mstring Away()		{ return i_away; };

    mDateTime IdleTime();
    void Action();

    mDateTime SignonTime()	{ return i_Signon_Time; }
    mDateTime MySignonTime()	{ return i_My_Signon_Time; }
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
    void UnChanIdentify(mstring channel);
    bool IsChanIdentified(mstring channel);
    mstring Identify(mstring password);
    void UnIdentify();
    bool IsIdentified()		{ return identified; }
    bool IsRecognized();
    bool IsServices()		{ return (i_server == ""); }
};

struct NickInfo;

class Nick_Stored_t : public mUserDef
{
    friend class Nick_Live_t;
    friend class NickServ;
    friend wxOutputStream &operator<<(wxOutputStream& out,Nick_Stored_t& in);
    friend wxInputStream &operator>>(wxInputStream& in, Nick_Stored_t& out);
    friend Nick_Stored_t CreateNickEntry(NickInfo *ni);

    mstring i_Name;
    mDateTime i_RegTime;
    mstring i_Password;
    mstring i_Email;
    mstring i_URL;
    mstring i_ICQ;
    mstring i_Description;
    mstring i_Comment;
    mstring i_Host;
    set<mstring> i_slaves; // HOST only
    set<mstring> i_access;
    set<mstring> i_ignore;
    bool i_Protect;
    bool l_Protect;
    bool i_Secure;
    bool l_Secure;
    bool i_NoExpire;
    bool l_NoExpire;
    bool i_NoMemo;
    bool l_NoMemo;
    bool i_Private;
    bool l_Private;
    bool i_PRIVMSG;
    bool l_PRIVMSG;
    mstring i_Language;
    bool l_Language;
    bool i_Forbidden;
    unsigned long i_Picture;

    mstring i_Suspend_By;
    mDateTime i_Suspend_Time;

    // offline
    mDateTime i_LastSeenTime;
    mstring i_LastRealName;
    mstring i_LastMask;
    mstring i_LastQuit;

    Nick_Stored_t(mstring nick, mDateTime regtime, const Nick_Stored_t &host); // Slave
    void Signon(mstring realname, mstring mask);
    void ChgNick(mstring nick);
    void ChangeOver(mstring oldnick);
public:
    Nick_Stored_t();
    Nick_Stored_t(const Nick_Stored_t &in) { *this = in; }
    Nick_Stored_t(mstring nick, mstring password);
    Nick_Stored_t(mstring nick); // Services Only (forbidden)
    void operator=(const Nick_Stored_t &in);
    bool operator==(const Nick_Stored_t &in) const
	{ return (i_Name == in.i_Name); }
    bool operator!=(const Nick_Stored_t &in) const
	{ return (i_Name != in.i_Name); }
    bool operator<(const Nick_Stored_t &in) const
	{ return (i_Name < in.i_Name); }

    mstring Name() { return i_Name; }
    mDateTime RegTime() { return i_RegTime; }

    void Drop();
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

    mstring Host() { return i_Host; }
    unsigned int Siblings();
    mstring Sibling(unsigned int count);
    bool IsSibling(mstring nick);
    bool Slave(mstring nick, mstring password, mDateTime regtime = Now());
    bool MakeHost();
    bool Unlink();

    unsigned int Access();
    mstring Access(unsigned int count);
    bool AccessAdd(const mstring& in);
    unsigned int AccessDel(mstring in);
    unsigned int AccessDel(unsigned int in)
	{ return AccessDel(Access(in)); }
    bool IsAccess(mstring in);

    unsigned int Ignore();
    mstring Ignore(unsigned int count);
    bool IgnoreAdd(mstring in);
    unsigned int IgnoreDel(mstring in);
    unsigned int IgnoreDel(unsigned int in)
	{ return IgnoreDel(Ignore(in)); }
    bool IsIgnore(mstring in);

    // flags
    bool Protect();
    void Protect(bool in);
    bool L_Protect();
    void L_Protect(bool in);
    bool Secure();
    void Secure(bool in);
    bool L_Secure();
    void L_Secure(bool in);
    bool NoExpire();
    void NoExpire(bool in);
    bool L_NoExpire();
    void L_NoExpire(bool in);
    bool NoMemo();
    void NoMemo(bool in);
    bool L_NoMemo();
    void L_NoMemo(bool in);
    bool Private();
    void Private(bool in);
    bool L_Private();
    void L_Private(bool in);
    bool PRIVMSG();
    void PRIVMSG(bool in);
    bool L_PRIVMSG();
    void L_PRIVMSG(bool in);
    mstring Language();
    void Language(mstring in);
    bool L_Language();
    void L_Language(bool in);
    bool Suspended();
    mstring Suspend_By();
    mDateTime Suspend_Time();
    bool Forbidden() { return i_Forbidden; }
    void SendPic(mstring nick);
    unsigned long PicNum();
    void GotPic(unsigned long picnum);

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
    unsigned int maxlen;	// Maximum length of a nickname
    mstring suffixes;		// What to add to unidentified nicks
    unsigned long expire;	// How long to keep nicknames
    unsigned long ident;	// How long to wait for IDENT
    unsigned long release;	// How long to keep after failed ident
    unsigned int passfail;	// Number of password fails before kill
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
    mstring def_language;	// Default val of Language
    bool lck_language;		// Language is locked?
    unsigned long picsize;	// MAX size of a personal pic
    mstring picext;		// Valid PIC extensions

    void AddCommands();
    void RemCommands();
public:
    unsigned int Maxlen()	{ return maxlen; }
    mstring Suffixes()		{ return suffixes; }
    unsigned long Expire()	{ return expire; }
    unsigned long Ident()	{ return ident; }
    unsigned long Release()	{ return release; }
    unsigned int Passfail()	{ return passfail; }
    bool DEF_Protect()		{ return def_protect; }
    bool LCK_Protect()		{ return lck_protect; }
    bool DEF_Secure()		{ return def_secure; }
    bool LCK_Secure()		{ return lck_secure; }
    bool DEF_NoExpire()		{ return def_noexpire; }
    bool LCK_NoExpire()		{ return lck_noexpire; }
    bool DEF_NoMemo()		{ return def_nomemo; }
    bool LCK_NoMemo()		{ return lck_nomemo; }
    bool DEF_Private()		{ return def_private; }
    bool LCK_Private()		{ return lck_private; }
    bool DEF_PRIVMSG()		{ return def_privmsg; }
    bool LCK_PRIVMSG()		{ return lck_privmsg; }
    mstring DEF_Language()	{ return def_language; }
    bool LCK_Language()		{ return lck_language; }
    unsigned long PicSize()	{ return picsize; }
    mstring PicExt()		{ return picext; }

    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);
    bool IsStored(mstring in);
    bool IsLive(mstring in);
    map<mstring,Nick_Stored_t> stored;
    map<mstring,Nick_Live_t> live;
    map<mstring,mDateTime> recovered;
    KillOnSignon_Handler kosh;
    InFlight_Handler ifh;

    static mstring findnextnick(mstring in);

    NickServ();
    virtual threadtype_enum Get_TType() const { return tt_NickServ; }
    virtual mstring GetInternalName() const { return "NickServ"; }
    virtual void execute(const mstring & message);

    static void do_Help(mstring mynick, mstring source, mstring params);
    static void do_Register(mstring mynick, mstring source, mstring params);
    static void do_Drop(mstring mynick, mstring source, mstring params);
    static void do_Link(mstring mynick, mstring source, mstring params);
    static void do_UnLink(mstring mynick, mstring source, mstring params);
    static void do_Host(mstring mynick, mstring source, mstring params);
    static void do_Slaves(mstring mynick, mstring source, mstring params);
    static void do_Identify(mstring mynick, mstring source, mstring params);
    static void do_Info(mstring mynick, mstring source, mstring params);
    static void do_Ghost(mstring mynick, mstring source, mstring params);
    static void do_Recover(mstring mynick, mstring source, mstring params);
    static void do_List(mstring mynick, mstring source, mstring params);
    static void do_Suspend(mstring mynick, mstring source, mstring params);
    static void do_UnSuspend(mstring mynick, mstring source, mstring params);
    static void do_Forbid(mstring mynick, mstring source, mstring params);
    static void do_Getpass(mstring mynick, mstring source, mstring params);

    static void do_access_Current(mstring mynick, mstring source, mstring params);
    static void do_access_Add(mstring mynick, mstring source, mstring params);
    static void do_access_Del(mstring mynick, mstring source, mstring params);
    static void do_access_List(mstring mynick, mstring source, mstring params);
    static void do_ignore_Add(mstring mynick, mstring source, mstring params);
    static void do_ignore_Del(mstring mynick, mstring source, mstring params);
    static void do_ignore_List(mstring mynick, mstring source, mstring params);
    static void do_set_Password(mstring mynick, mstring source, mstring params);
    static void do_set_Email(mstring mynick, mstring source, mstring params);
    static void do_set_URL(mstring mynick, mstring source, mstring params);
    static void do_set_ICQ(mstring mynick, mstring source, mstring params);
    static void do_set_Description(mstring mynick, mstring source, mstring params);
    static void do_set_Comment(mstring mynick, mstring source, mstring params);
    static void do_set_Picture(mstring mynick, mstring source, mstring params);
    static void do_set_Protect(mstring mynick, mstring source, mstring params);
    static void do_set_Secure(mstring mynick, mstring source, mstring params);
    static void do_set_NoExpire(mstring mynick, mstring source, mstring params);
    static void do_set_NoMemo(mstring mynick, mstring source, mstring params);
    static void do_set_Private(mstring mynick, mstring source, mstring params);
    static void do_set_PRIVMSG(mstring mynick, mstring source, mstring params);
    static void do_set_Language(mstring mynick, mstring source, mstring params);
    static void do_lock_Protect(mstring mynick, mstring source, mstring params);
    static void do_lock_Secure(mstring mynick, mstring source, mstring params);
    static void do_lock_NoMemo(mstring mynick, mstring source, mstring params);
    static void do_lock_Private(mstring mynick, mstring source, mstring params);
    static void do_lock_PRIVMSG(mstring mynick, mstring source, mstring params);
    static void do_lock_Language(mstring mynick, mstring source, mstring params);
    static void do_unlock_Protect(mstring mynick, mstring source, mstring params);
    static void do_unlock_Secure(mstring mynick, mstring source, mstring params);
    static void do_unlock_NoMemo(mstring mynick, mstring source, mstring params);
    static void do_unlock_Private(mstring mynick, mstring source, mstring params);
    static void do_unlock_PRIVMSG(mstring mynick, mstring source, mstring params);
    static void do_unlock_Language(mstring mynick, mstring source, mstring params);
};

#endif
