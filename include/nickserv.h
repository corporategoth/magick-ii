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
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#ifndef _NICKSERV_H
#define _NICKSERV_H
#include "pch.h"
RCSID(nickserv_h, "@(#) $Id$");

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

class Nick_Live_t : public mUserDef, public ref_class
{
    mstring i_Name;
    unsigned long i_Numeric;
    mDateTime i_Signon_Time;
    mDateTime i_My_Signon_Time;
    mDateTime i_Last_Action;
    mstring i_realname;
    mstring i_user;
    mstring i_host;
    mstring i_alt_host;
    mstring i_server;
    mstring i_squit;
    mstring i_away;
    mstring modes;

    set < mstring > joined_channels;
    vector < mDateTime > last_msg_times;
    unsigned int last_msg_entries;
    unsigned int flood_triggered_times;
    unsigned int failed_passwds;

    set < mstring > chans_founder_identd;
    vector < mstring > try_chan_ident;
    bool identified;
    bool services;
    mDateTime last_nick_reg, last_chan_reg, last_memo;

public:
    class InFlight_t
    {
	friend class Nick_Live_t;
	friend class DccXfer;
	friend class InFlight_Handler;

	mstring nick;

	FileMap::FileType type;
	long timer;
	bool fileattach;
	bool fileinprog;
	mstring service;
	mstring recipiant;
	mstring text;

	InFlight_t(const mstring & name) : nick(name)
	{
	    init();
	}
	InFlight_t()
	{
	    init();
	}
	~InFlight_t();

	void ChgNick(const mstring & newnick);
	InFlight_t &operator=(const InFlight_t & in);
	void init();

	// Called upon completion
	// 0 means it failed
	void File(const unsigned long filenum);

	// Called upon start
	void SetInProg();

    public:
	void Memo(const bool file, const mstring & mynick, const mstring & recipiant, const mstring & message,
		  const bool silent = false);
	void Continue(const mstring & message);
	void Cancel();
	void End(const unsigned long filenum);
	void Picture(const mstring & mynick);
	void Public(const mstring & mynick, const mstring & committees = "");
	mstring Text();
	mstring Recipiant();
	bool Memo() const;
	bool Picture() const;
	bool Public() const;
	bool Exists() const;
	bool File() const;
	bool InProg() const;
	size_t Usage() const;
	void DumpB() const;
	void DumpE() const;
    }
    InFlight;

    Nick_Live_t();
    Nick_Live_t(const Nick_Live_t & in) : mUserDef(in), ref_class()
    {
	*this = in;
    }
    Nick_Live_t(const mstring & name, const mDateTime & signon, const mstring & server, const mstring & username,
		const mstring & hostname, const mstring & realname);
    Nick_Live_t(const mstring & name, const mstring & username, const mstring & hostname, const mstring & realname);	// Services ONLY

    ~Nick_Live_t()
    {
    }
    Nick_Live_t &operator=(const Nick_Live_t & in);
    bool operator==(const Nick_Live_t & in) const
    {
	return (i_Name == in.i_Name);
    }
    bool operator!=(const Nick_Live_t & in) const
    {
	return (i_Name != in.i_Name);
    }
    bool operator<(const Nick_Live_t & in) const
    {
	return (i_Name < in.i_Name);
    }

    void PostSignon(const set<mstring> &wason, bool newsignon = true);

    bool validated() const
    {
	return (i_Name.length() != 0 && i_Name.validated());
    }

    // channel maintinance
    void Join(const mstring & channel);
    void Part(const mstring & channel);
    void Kick(const mstring & kicker, const mstring & channel);
    void Quit(const mstring & reason);
    bool IsInChan(const mstring & channel);
    set < mstring > Channels() const;

    // true if user ignored
    bool FloodTrigger();

    // Data maintinance
    set < mstring > Name(const mstring & in);
    mstring Name() const
    {
	return i_Name;
    }

    void SendMode(const mstring & in);
    void Mode(const mstring & in);
    mstring Mode() const;
    bool HasMode(const mstring & in) const;

    void Numeric(const unsigned long in);
    unsigned long Numeric() const;

    void Away(const mstring & in);
    mstring Away() const;

    mDateTime LastAction() const;
    void Action();

    mDateTime SignonTime() const;
    mDateTime MySignonTime() const;
    mstring RealName() const;
    mstring User() const;
    mstring Host() const;
    mstring AltHost() const;
    void AltHost(const mstring & in);
    mstring Server() const;
    void SetSquit();
    void ClearSquit(const mstring & inmodes = "");
    mstring Squit() const;

    enum styles
    {
	N = 1,			// nick!*@*
	N_U_P_H,		// nick!user@port.host
	N_U_H,			// nick!user@*.host
	N_P_H,			// nick!*@port.host
	N_H,			// nick!*@*.host
	U_P_H,			// *!user@port.host
	U_H,			// *!user@*.host
	P_H,			// *!*@port.host
	H			// *!*@*.host
    };

    mstring Mask(const styles type) const;
    mstring AltMask(const styles type) const;

    // Will KILL user if >PassFailMax
    mstring ChanIdentify(const mstring & channel, const mstring & password);
    void UnChanIdentify(const mstring & channel);
    bool IsChanIdentified(const mstring & channel);
    mstring Identify(const mstring & password);
    void UnIdentify();
    bool IsIdentified() const;
    bool IsRecognized() const;
    bool IsServices() const;

    void SetLastNickReg();
    mDateTime LastNickReg() const;
    void SetLastChanReg();
    mDateTime LastChanReg() const;
    void SetLastMemo();
    mDateTime LastMemo() const;

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

struct NickInfo_CUR;
struct ESP_NickInfo;
struct EPO_NickAlias;
struct EPO_NickCore;
struct HYB_NickInfo;

class Nick_Stored_t : public mUserDef, public SXP::IPersistObj, public ref_class
{
    friend class Nick_Live_t;
    friend class NickServ;
    friend Nick_Stored_t *CreateNickEntry(NickInfo_CUR * ni);
    friend Nick_Stored_t *ESP_CreateNickEntry(ESP_NickInfo * ni);
    friend Nick_Stored_t *EPO_CreateNickEntry(EPO_NickAlias * na, EPO_NickCore * nc);
    friend Nick_Stored_t *HYB_CreateNickEntry(HYB_NickInfo * ni);

    mstring i_Name;
    mDateTime i_RegTime;
    mstring i_Password;
    mstring i_Email;
    mstring i_URL;
    mstring i_ICQ;
    mstring i_AIM;
    mstring i_MSN;
    mstring i_Yahoo;
    mstring i_Description;
    mstring i_Comment;
    mstring i_Host;

    set < mstring > i_slaves;	// HOST only
    set < mstring > i_access;
    set < mstring > i_ignore;

    class setting_t
    {
	friend class Nick_Stored_t;
	friend Nick_Stored_t *CreateNickEntry(NickInfo_CUR * ni);
	friend Nick_Stored_t *ESP_CreateNickEntry(ESP_NickInfo * ni);
	friend Nick_Stored_t *EPO_CreateNickEntry(EPO_NickAlias * na, EPO_NickCore * nc);
	friend Nick_Stored_t *HYB_CreateNickEntry(HYB_NickInfo * ni);

	mstring Language;
	bool Protect:1;
	bool Secure:1;
	bool NoMemo:1;
	bool Private:1;
	bool PRIVMSG:1;
	bool NoExpire:1;
	bool Forbidden:1;
	unsigned long Picture;
    }
    setting;

    class lock_t
    {
	friend class Nick_Stored_t;
	bool Protect:1;
	bool Secure:1;
	bool NoMemo:1;
	bool Private:1;
	bool PRIVMSG:1;
	bool Language:1;
    }
    lock;

    mstring i_Suspend_By;
    mDateTime i_Suspend_Time;

    // offline
    mDateTime i_LastSeenTime;
    mstring i_LastRealName;
    mstring i_LastMask;
    mstring i_LastQuit;

    Nick_Stored_t(const mstring & nick, const mDateTime & regtime, const Nick_Stored_t & host);	// Slave
    void Signon(const mstring & realname, const mstring & mask);
    void ChgNick(const mstring & nick);
    void ChangeOver(const mstring & oldnick);
    void defaults();

    static SXP::Tag tag_Nick_Stored_t, tag_Name, tag_RegTime, tag_Password, tag_Email, tag_URL, tag_ICQ, tag_AIM, tag_MSN,
	tag_Yahoo, tag_Description, tag_Comment, tag_Host, tag_set_Protect, tag_set_Secure, tag_set_NoExpire, tag_set_NoMemo,
	tag_set_Private, tag_set_PRIVMSG, tag_set_Language, tag_Forbidden, tag_lock_Protect, tag_lock_Secure,
	tag_lock_NoExpire, tag_lock_NoMemo, tag_lock_Private, tag_lock_PRIVMSG, tag_lock_Language, tag_Picture, tag_Suspend_By,
	tag_Suspend_Time, tag_LastSeenTime, tag_LastRealName, tag_LastMask, tag_LastQuit, tag_Access, tag_Ignore, tag_UserDef;
public:
    Nick_Stored_t();
    Nick_Stored_t(const Nick_Stored_t & in) : mUserDef(in), SXP::IPersistObj(in), ref_class()
    {
	*this = in;
    }
    Nick_Stored_t(const mstring & nick, const mstring & password);
    Nick_Stored_t(const mstring & nick);	// Services Only (forbidden)

    ~Nick_Stored_t()
    {
    }
    Nick_Stored_t &operator=(const Nick_Stored_t & in);
    bool operator==(const Nick_Stored_t & in) const
    {
	return (i_Name == in.i_Name);
    }
    bool operator!=(const Nick_Stored_t & in) const
    {
	return (i_Name != in.i_Name);
    }
    bool operator<(const Nick_Stored_t & in) const
    {
	return (i_Name < in.i_Name);
    }

    bool validated() const
    {
	return (i_Name.length() != 0 && i_Name.validated());
    }
    mstring Name() const
    {
	return i_Name;
    }
    mDateTime RegTime() const;

    unsigned long Drop();
    mstring Password();
    void Password(const mstring & in);
    bool CheckPass(const mstring & pass);
    mstring Email();
    void Email(const mstring & in);
    mstring URL();
    void URL(const mstring & in);
    mstring ICQ();
    void ICQ(const mstring & in);
    mstring AIM();
    void AIM(const mstring & in);
    mstring MSN();
    void MSN(const mstring & in);
    mstring Yahoo();
    void Yahoo(const mstring & in);
    mstring Description();
    void Description(const mstring & in);
    mstring Comment();
    void Comment(const mstring & in);

    void Suspend(const mstring & name, const mstring & reason)
    {
	Comment(reason);
	Suspend(name);
    }
    void Suspend(const mstring & name);
    void UnSuspend();

    mstring Host();
    unsigned int Siblings();
    mstring Sibling(const unsigned int count);
    bool IsSibling(const mstring & nick);
    bool Slave(const mstring & nick, const mstring & password, const mDateTime & regtime = mDateTime::CurrentDateTime());
    bool MakeHost();
    bool Unlink();

    unsigned int Access();
    mstring Access(const unsigned int count);
    bool AccessAdd(const mstring & in);
    unsigned int AccessDel(const mstring & in);
    unsigned int AccessDel(const unsigned int in)
    {
	return AccessDel(Access(in));
    }
    bool IsAccess(const mstring & in);

    unsigned int Ignore();
    mstring Ignore(const unsigned int count);
    bool IgnoreAdd(const mstring & in);
    unsigned int IgnoreDel(const mstring & in);
    unsigned int IgnoreDel(const unsigned int in)
    {
	return IgnoreDel(Ignore(in));
    }
    bool IsIgnore(const mstring & in);

    // flags
    bool Protect();
    void Protect(const bool in);
    bool L_Protect();
    void L_Protect(const bool in);
    bool Secure();
    void Secure(const bool in);
    bool L_Secure();
    void L_Secure(const bool in);
    bool NoMemo();
    void NoMemo(const bool in);
    bool L_NoMemo();
    void L_NoMemo(const bool in);
    bool Private();
    void Private(const bool in);
    bool L_Private();
    void L_Private(const bool in);
    bool PRIVMSG();
    void PRIVMSG(const bool in);
    bool L_PRIVMSG();
    void L_PRIVMSG(const bool in);
    mstring Language();
    void Language(const mstring & in);
    bool L_Language();
    void L_Language(const bool in);
    bool Suspended();
    mstring Suspend_By();
    mDateTime Suspend_Time();
    bool Forbidden() const;
    bool NoExpire();
    void NoExpire(const bool in);
    unsigned long PicNum();
    void GotPic(const unsigned long picnum);

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
    void Quit(const mstring & message);

    size_t MyChannels() const;

    SXP::Tag & GetClassTag() const
    {
	return tag_Nick_Stored_t;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);

    size_t Usage();
    void DumpB();
    void DumpE();
};

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class NickServ : public mBase, public SXP::IPersistObj
{
    friend class Magick;
    friend int EventTask::svc(void);
    friend int IrcSvcHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask);

private:
    // Config Entries ...
    mstring enforcer_name;	// Realname of enforcer
    bool append_rename;		// Type of renaming scheme to use.
    mstring suffixes;		// What to add to unidentified nicks
    unsigned long expire;	// How long to keep nicknames
    unsigned long delay;	// How long between registrations
    unsigned long ident;	// How long to wait for IDENT
    unsigned long release;	// How long to keep after failed ident
    unsigned int passfail;	// Number of password fails before kill
    unsigned long picsize;	// Maximum picture size
    mstring picext;		// Valid PIC extensions

    class def_t
    {
	friend class NickServ;
	friend class Magick;
	bool Protect:1;
	bool Secure:1;
	bool NoMemo:1;
	bool Private:1;
	bool PRIVMSG:1;
	bool NoExpire:1;
	mstring Language;
    }
    def;

    class lock_t
    {
	friend class NickServ;
	friend class Magick;
	bool Protect:1;
	bool Secure:1;
	bool NoMemo:1;
	bool Private:1;
	bool PRIVMSG:1;
	bool NoExpire:1;
	bool Language:1;
    }
    lock;

    static SXP::Tag tag_NickServ;

    vector < Nick_Stored_t * > ns_array;
public:
#ifdef NO_HASH_MAP
    typedef map < mstring, Nick_Stored_t * > stored_t;
    typedef map < mstring, Nick_Live_t * > live_t;
#else
    typedef hash_map < mstring, Nick_Stored_t * > stored_t;
    typedef hash_map < mstring, Nick_Live_t * > live_t;
#endif
    typedef map < mstring, mDateTime > recovered_t;

private:

    stored_t stored;
    live_t live;
    recovered_t recovered;

    void AddCommands();
    void RemCommands();

public:
    NickServ();
    ~NickServ()
    {
    }
    class stats_t
    {
	friend class NickServ;

	mDateTime i_ClearTime;
	unsigned long i_Register;
	unsigned long i_Drop;
	unsigned long i_Link;
	unsigned long i_Unlink;
	unsigned long i_Host;
	unsigned long i_Identify;
	unsigned long i_Ghost;
	unsigned long i_Recover;
	unsigned long i_Suspend;
	unsigned long i_Unsuspend;
	unsigned long i_Forbid;
	unsigned long i_Getpass;
	unsigned long i_Access;
	unsigned long i_Ignore;
	unsigned long i_Set;
	unsigned long i_NoExpire;
	unsigned long i_Lock;
	unsigned long i_Unlock;
	unsigned long i_SetPicture;
	unsigned long i_Send;

    public:
	stats_t()
	{
	    clear();
	}
	void clear()
	{
	    i_ClearTime = mDateTime::CurrentDateTime();
	    i_Register = i_Drop = i_Link = i_Unlink = i_Host = i_Identify = i_Ghost = i_Recover = i_Suspend = i_Unsuspend =
		i_Forbid = i_Getpass = i_Access = i_Ignore = i_Set = i_NoExpire = i_Lock = i_Unlock = i_SetPicture = i_Send =
		0;
	}
	mDateTime ClearTime() const
	{
	    return i_ClearTime;
	}
	unsigned long Register() const
	{
	    return i_Register;
	}
	unsigned long Drop() const
	{
	    return i_Drop;
	}
	unsigned long Link() const
	{
	    return i_Link;
	}
	unsigned long Unlink() const
	{
	    return i_Unlink;
	}
	unsigned long Host() const
	{
	    return i_Host;
	}
	unsigned long Identify() const
	{
	    return i_Identify;
	}
	unsigned long Ghost() const
	{
	    return i_Ghost;
	}
	unsigned long Recover() const
	{
	    return i_Recover;
	}
	unsigned long Suspend() const
	{
	    return i_Suspend;
	}
	unsigned long Unsuspend() const
	{
	    return i_Unsuspend;
	}
	unsigned long Forbid() const
	{
	    return i_Forbid;
	}
	unsigned long Getpass() const
	{
	    return i_Getpass;
	}
	unsigned long Access() const
	{
	    return i_Access;
	}
	unsigned long Ignore() const
	{
	    return i_Ignore;
	}
	unsigned long Set() const
	{
	    return i_Set;
	}
	unsigned long NoExpire() const
	{
	    return i_NoExpire;
	}
	unsigned long Lock() const
	{
	    return i_Lock;
	}
	unsigned long Unlock() const
	{
	    return i_Unlock;
	}
	unsigned long SetPicture() const
	{
	    return i_SetPicture;
	}
	unsigned long Send() const
	{
	    return i_Send;
	}
    }
    stats;

    mstring Enforcer_Name() const
    {
	return enforcer_name;
    }
    bool Append_Rename() const
    {
	return append_rename;
    }
    mstring Suffixes() const
    {
	return suffixes;
    }
    unsigned long Expire() const
    {
	return expire;
    }
    unsigned long Delay() const
    {
	return delay;
    }
    unsigned long Ident() const
    {
	return ident;
    }
    unsigned long Release() const
    {
	return release;
    }
    unsigned int Passfail() const
    {
	return passfail;
    }
    bool DEF_Protect() const
    {
	return def.Protect;
    }
    bool LCK_Protect() const
    {
	return lock.Protect;
    }
    bool DEF_Secure() const
    {
	return def.Secure;
    }
    bool LCK_Secure() const
    {
	return lock.Secure;
    }
    bool DEF_NoExpire() const
    {
	return def.NoExpire;
    }
    bool LCK_NoExpire() const
    {
	return lock.NoExpire;
    }
    bool DEF_NoMemo() const
    {
	return def.NoMemo;
    }
    bool LCK_NoMemo() const
    {
	return lock.NoMemo;
    }
    bool DEF_Private() const
    {
	return def.Private;
    }
    bool LCK_Private() const
    {
	return lock.Private;
    }
    bool DEF_PRIVMSG() const
    {
	return def.PRIVMSG;
    }
    bool LCK_PRIVMSG() const
    {
	return lock.PRIVMSG;
    }
    mstring DEF_Language() const
    {
	return def.Language;
    }
    bool LCK_Language() const
    {
	return lock.Language;
    }
    unsigned long PicSize() const
    {
	return picsize;
    }
    mstring PicExt() const
    {
	return picext;
    }

    InFlight_Handler ifh;

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddStored(Nick_Stored_t * in) throw(E_NickServ_Stored);
    void AddStored(const Nick_Stored_t & in) throw(E_NickServ_Stored)
    {
	AddStored(new Nick_Stored_t(in));
    }
    map_entry < Nick_Stored_t > GetStored(const mstring & in) const throw(E_NickServ_Stored);
    void RemStored(const mstring & in) throw(E_NickServ_Stored);
#else
    void AddStored(Nick_Stored_t * in);
    void AddStored(const Nick_Stored_t & in)
    {
	AddStored(new Nick_Stored_t(in));
    }
    map_entry < Nick_Stored_t > GetStored(const mstring & in);
    void RemStored(const mstring & in);
#endif
    stored_t::iterator StoredBegin()
    {
	return stored.begin();
    }
    stored_t::iterator StoredEnd()
    {
	return stored.end();
    }
    stored_t::const_iterator StoredBegin() const
    {
	return stored.begin();
    }
    stored_t::const_iterator StoredEnd() const
    {
	return stored.end();
    }
    size_t StoredSize() const
    {
	return stored.size();
    }
    bool IsStored(const mstring & in) const;

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddLive(Nick_Live_t * in) throw(E_NickServ_Live);
    void AddLive(const Nick_Live_t & in) throw(E_NickServ_Live)
    {
	AddLive(new Nick_Live_t(in));
    }
    map_entry < Nick_Live_t > GetLive(const mstring & in) const throw(E_NickServ_Live);
    void RemLive(const mstring & in) throw(E_NickServ_Live);
#else
    void AddLive(Nick_Live_t * in);
    void AddLive(const Nick_Live_t & in)
    {
	AddLive(new Nick_Live_t(in));
    }
    map_entry < Nick_Live_t > GetLive(const mstring & in) const;
    void RemLive(const mstring & in);
#endif
    live_t::iterator LiveBegin()
    {
	return live.begin();
    }
    live_t::iterator LiveEnd()
    {
	return live.end();
    }
    live_t::const_iterator LiveBegin() const
    {
	return live.begin();
    }
    live_t::const_iterator LiveEnd() const
    {
	return live.end();
    }
    size_t LiveSize() const
    {
	return live.size();
    }
    bool IsLive(const mstring & in) const;
    bool IsLiveAll(const mstring & in) const;

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddRecovered(const mstring & name, const mDateTime & in) throw(E_NickServ_Recovered);
    const mDateTime &GetRecovered(const mstring & in) const throw(E_NickServ_Recovered);
    void RemRecovered(const mstring & in) throw(E_NickServ_Recovered);
#else
    void AddRecovered(const mstring & name, const mDateTime & in);
    const mDateTime &GetRecovered(const mstring & in) const;
    void RemRecovered(const mstring & in);
#endif
    recovered_t::iterator RecoveredBegin()
    {
	return recovered.begin();
    }
    recovered_t::iterator RecoveredEnd()
    {
	return recovered.end();
    }
    recovered_t::const_iterator RecoveredBegin() const
    {
	return recovered.begin();
    }
    recovered_t::const_iterator RecoveredEnd() const
    {
	return recovered.end();
    }
    size_t RecoveredSize() const
    {
	return recovered.size();
    }
    bool IsRecovered(const mstring & in) const;

    static mstring findnextnick(const mstring & in);

    threadtype_enum Get_TType() const
    {
	return tt_NickServ;
    }
    mstring GetInternalName() const
    {
	return "NickServ";
    }
    void execute(mstring & source, const mstring & msgtype, const mstring & params);

    static void do_Help(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Register(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Drop(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Link(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_UnLink(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Host(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Slaves(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Identify(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Info(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Ghost(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Recover(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_ListNoExp(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Send(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Suspend(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_UnSuspend(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Forbid(const mstring & mynick, const mstring & source, const mstring & params);

#ifdef GETPASS
    static void do_Getpass(const mstring & mynick, const mstring & source, const mstring & params);
#endif
    static void do_Setpass(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Live(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_LiveOper(const mstring & mynick, const mstring & source, const mstring & params);

    static void do_access_Current(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_access_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_access_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_access_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_ignore_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_ignore_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_ignore_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Password(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Email(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_URL(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_ICQ(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_AIM(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_MSN(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Yahoo(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Description(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Comment(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Picture(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Protect(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Secure(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_NoExpire(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_NoMemo(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Private(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_PRIVMSG(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Language(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Protect(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Secure(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_NoMemo(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Private(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_PRIVMSG(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Language(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Protect(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Secure(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_NoMemo(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Private(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_PRIVMSG(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Language(const mstring & mynick, const mstring & source, const mstring & params);

    SXP::Tag & GetClassTag() const
    {
	return tag_NickServ;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);
    void PostLoad();
};

#endif
