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
#ifndef _NICKSERV_H
#define _NICKSERV_H
#include "pch.h"
static const char *ident_nickserv_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.50  2000/09/12 21:17:01  prez
** Added IsLiveAll (IsLive now checks to see if user is SQUIT).
**
** Revision 1.49  2000/09/01 10:54:38  prez
** Added Changing and implemented Modify tracing, now just need to create
** DumpB() and DumpE() functions in all classes, and put MCB() / MCE() calls
** (or MB() / ME() or CB() / CE() where MCB() / MCE() not appropriate) in.
**
** Revision 1.48  2000/08/19 10:59:46  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.47  2000/08/06 05:27:46  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.46  2000/08/03 13:06:29  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.45  2000/07/28 14:49:34  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.44  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.43  2000/06/15 13:41:10  prez
** Added my tasks to develop *grin*
** Also did all the chanserv live locking (stored to be done).
** Also made magick check if its running, and kill on startup if so.
**
** Revision 1.42  2000/06/12 06:07:49  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.41  2000/06/11 08:20:11  prez
** More minor bug fixes, godda love testers.
**
** Revision 1.40  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.39  2000/05/25 08:16:38  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.38  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.37  2000/05/14 04:02:52  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.36  2000/05/08 14:42:01  prez
** More on xmlisation of nickserv and chanserv
**
** Revision 1.35  2000/05/03 14:12:22  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.34  2000/04/04 03:21:34  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.33  2000/04/04 03:13:50  prez
** Added support for masking hostnames.
**
** Revision 1.32  2000/04/03 09:45:21  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.31  2000/03/23 10:22:24  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.30  2000/03/19 08:50:53  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.29  2000/03/15 14:42:58  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.28  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.27  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.26  2000/02/21 03:27:38  prez
** Updated language files ...
**
** Revision 1.25  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#include "base.h"
#include "ircsocket.h"
#include "filesys.h"
#include "lockable.h"

class Nick_Live_t : public mUserDef
{
    mstring i_Name;
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
    set<mstring> joined_channels;
    vector<mDateTime> last_msg_times;
    unsigned int last_msg_entries;
    unsigned int flood_triggered_times;
    unsigned int failed_passwds;
    set<mstring> chans_founder_identd;
    vector<mstring> try_chan_ident;
    bool identified;
    bool services;
    mDateTime last_nick_reg, last_chan_reg, last_memo;

public:

    class InFlight_t {
	friend class Nick_Live_t;
	friend class DccXfer;
	friend class InFlight_Handler;

	mstring nick;
	FileMap::FileType type;
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
	void Memo (bool file, mstring mynick, mstring recipiant, mstring message, bool silent = false);
	void Continue(mstring message);
	void Cancel();
	void End(unsigned long filenum);
	void Picture (mstring mynick);
	void Public (mstring mynick, mstring committees = "");
	bool Memo();
	bool Picture();
	bool Public();
	bool Exists();
	bool File();
	bool InProg();
	size_t Usage();
	void DumpB();
	void DumpE();
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
    set<mstring> Channels();

    // true if user ignored
    bool FloodTrigger();

    // Data maintinance
    void Name(mstring in);
    mstring Name()		{ return i_Name; }

    void SendMode(mstring in);
    void Mode(mstring in);
    mstring Mode();
    bool HasMode(mstring in);

    void Away(mstring in);
    mstring Away();

    mDateTime IdleTime();
    void Action();

    mDateTime SignonTime();
    mDateTime MySignonTime();
    mstring RealName();
    mstring User();
    mstring Host();
    mstring AltHost();
    void AltHost(mstring in);
    mstring Server();
    void SetSquit();
    void ClearSquit();
    mstring Squit();

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
    mstring AltMask(styles type);

    // Will KILL user if >PassFailMax
    mstring ChanIdentify(mstring channel, mstring password);
    void UnChanIdentify(mstring channel);
    bool IsChanIdentified(mstring channel);
    mstring Identify(mstring password);
    void UnIdentify();
    bool IsIdentified();
    bool IsRecognized();
    bool IsServices();

    void SetLastNickReg();
    mDateTime LastNickReg();
    void SetLastChanReg();
    mDateTime LastChanReg();
    void SetLastMemo();
    mDateTime LastMemo();

    size_t Usage();
    void DumpB();
    void DumpE();
};

struct NickInfo;

class Nick_Stored_t : public mUserDef, public SXP::IPersistObj
{
    friend class Nick_Live_t;
    friend class NickServ;
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

    static SXP::Tag tag_Nick_Stored_t, tag_Name, tag_RegTime,
	tag_Password, tag_Email, tag_URL, tag_ICQ, tag_Description,
	tag_Comment, tag_Host, tag_set_Protect, tag_set_Secure,
	tag_set_NoExpire, tag_set_NoMemo, tag_set_Private,
	tag_set_PRIVMSG, tag_set_Language, tag_Forbidden,
	tag_lock_Protect, tag_lock_Secure, tag_lock_NoExpire,
	tag_lock_NoMemo, tag_lock_Private, tag_lock_PRIVMSG,
	tag_lock_Language, tag_Picture, tag_Suspend_By,
	tag_Suspend_Time, tag_LastSeenTime, tag_LastRealName,
	tag_LastMask, tag_LastQuit, tag_Access, tag_Ignore, tag_UserDef;
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

    mstring Name()		{ return i_Name; }
    mDateTime RegTime();

    unsigned long Drop();
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

    mstring Host();
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
    bool Forbidden();
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

    size_t MyChannels();

    SXP::Tag& GetClassTag() const { return tag_Nick_Stored_t; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);

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
    static SXP::Tag tag_NickServ;

    vector<Nick_Stored_t *> ns_array;

    void AddCommands();
    void RemCommands();
public:
    class stats_t
    {
	friend class NickServ;

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
    public:
	stats_t() {
	    i_Register = i_Drop = i_Link = i_Unlink = i_Host =
		i_Identify = i_Ghost = i_Recover = i_Suspend =
		i_Unsuspend = i_Forbid = i_Getpass = i_Access =
		i_Ignore = i_Set = i_NoExpire = i_Lock =
		i_Unlock = 0; }
	unsigned long Register()    { return i_Register; }
	unsigned long Drop()	    { return i_Drop; }
	unsigned long Link()	    { return i_Link; }
	unsigned long Unlink()	    { return i_Unlink; }
	unsigned long Host()	    { return i_Host; }
	unsigned long Identify()    { return i_Identify; }
	unsigned long Ghost()	    { return i_Ghost; }
	unsigned long Recover()	    { return i_Recover; }
	unsigned long Suspend()	    { return i_Suspend; }
	unsigned long Unsuspend()   { return i_Unsuspend; }
	unsigned long Forbid()	    { return i_Forbid; }
	unsigned long Getpass()	    { return i_Getpass; }
	unsigned long Access()	    { return i_Access; }
	unsigned long Ignore()	    { return i_Ignore; }
	unsigned long Set()	    { return i_Set; }
	unsigned long NoExpire()    { return i_NoExpire; }
	unsigned long Lock()	    { return i_Lock; }
	unsigned long Unlock()	    { return i_Unlock; }
    } stats;

    mstring Enforcer_Name()	{ return enforcer_name; }
    bool Append_Rename()	{ return append_rename; }
    mstring Suffixes()		{ return suffixes; }
    unsigned long Expire()	{ return expire; }
    unsigned long Delay()	{ return delay; }
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

    bool IsStored(mstring in);
    bool IsLive(mstring in);
    bool IsLiveAll(mstring in);
    map<mstring,Nick_Stored_t> stored;
    map<mstring,Nick_Live_t> live;
    map<mstring,mDateTime> recovered;
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
    static void do_ListNoExp(mstring mynick, mstring source, mstring params);
    static void do_Send(mstring mynick, mstring source, mstring params);
    static void do_Suspend(mstring mynick, mstring source, mstring params);
    static void do_UnSuspend(mstring mynick, mstring source, mstring params);
    static void do_Forbid(mstring mynick, mstring source, mstring params);
    static void do_Getpass(mstring mynick, mstring source, mstring params);
    static void do_Live(mstring mynick, mstring source, mstring params);
    static void do_LiveOper(mstring mynick, mstring source, mstring params);

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

    virtual SXP::Tag& GetClassTag() const { return tag_NickServ; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);
    void PostLoad();
};

#endif
