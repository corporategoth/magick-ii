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
#ifndef _chanserv_h
#define _chanserv_h

#include "base.h"
#include "ircsocket.h"

bool checkops(pair<const mstring, pair<bool,bool> > &in);
bool checkvoices(pair<const mstring, pair<bool,bool> > &in);

class Chan_Live_t : public mUserDef
{
    friend class Nick_Live_t;
    friend class EventTask;
    friend class Part_Handler;
    mstring i_Name;
    mDateTime i_Creation_Time;
    // below: .first == op .second==voice
    map<mstring, pair<bool, bool> > squit;
    map<mstring, pair<bool, bool> > users;
    map<mstring, mDateTime> bans;
    mstring i_Topic;
    mstring i_Topic_Setter;
    mDateTime i_Topic_Set_Time;
    mstring modes;
    unsigned int i_Limit;
    mstring i_Key;
    mstring p_modes_on;
    mstring p_modes_off;
    vector<mstring> p_modes_on_params;
    vector<mstring> p_modes_off_params;
    long ph_timer;

    bool ModeExists(mstring mode, vector<mstring> mode_params,
			bool change, char reqmode, mstring reqparam = "");
    void RemoveMode(mstring mode, vector<mstring> mode_params,
			bool change, char reqmode, mstring reqparam = "");

    void Join(mstring nick); // Called by Nick_Live_t
    unsigned int Part(mstring nick); // Called by Nick_Live_t
    void Squit(mstring nick); // Called by Nick_Live_t
    void UnSquit(mstring nick); // Called by Nick_Live_t
    unsigned int Kick(mstring nick, mstring kicker); // Called by Nick_Live_t
    void ChgNick(mstring nick, mstring newnick); // Called by Nick_Live_t
public:
    Chan_Live_t() {}
    Chan_Live_t(const Chan_Live_t& in) { *this = in; }
    Chan_Live_t(mstring name, mstring first_user);
    void operator=(const Chan_Live_t &in);
    bool operator==(const Chan_Live_t &in) const
	{ return (i_Name == in.i_Name); }
    bool operator!=(const Chan_Live_t &in) const
	{ return (i_Name != in.i_Name); }
    bool operator<(const Chan_Live_t &in) const
	{ return (i_Name < in.i_Name); }

    mstring Name()		{ return i_Name; }
    mDateTime Creation_Time()	{ return i_Creation_Time; }

    void Topic(mstring topic, mstring setter, mDateTime time = Now());
    mstring Topic()		{ return i_Topic; }
    mstring Topic_Setter()	{ return i_Topic_Setter; }
    mDateTime Topic_Set_Time()	{ return i_Topic_Set_Time; }

    unsigned int Squit();
    mstring Squit(unsigned int num);
    unsigned int Users();
    mstring User(unsigned int num);
    unsigned int Ops();
    mstring Op(unsigned int num);
    unsigned int Voices();
    mstring Voice(unsigned int num);
    pair<bool, bool> User(mstring name);
    unsigned int Bans();
    mstring Ban(unsigned int num);
    mDateTime Ban(mstring mask);
    bool IsSquit(mstring nick);
    bool IsIn(mstring nick);
    bool IsOp(mstring nick);
    bool IsVoice(mstring nick);
    bool IsBan(mstring mask);

    void LockDown();
    void UnLock();
    void SendMode(mstring in);			// out
    void Mode(mstring source, mstring in);	// in
    bool HasMode(mstring in)	{ return modes.Contains(in); }
    mstring Mode()		{ return modes; }
    mstring Key()		{ return i_Key; }
    unsigned int Limit()	{ return i_Limit; }

};

struct ChanInfo;

class Chan_Stored_t : public mUserDef
{
    friend class Nick_Live_t;
    friend class Chan_Live_t;
    friend class ChanServ;
    friend wxOutputStream &operator<<(wxOutputStream& out,Chan_Stored_t& in);
    friend wxInputStream &operator>>(wxInputStream& in, Chan_Stored_t& out);
    friend Chan_Stored_t CreateChanEntry(ChanInfo *ci);

    mstring i_Name;
    mDateTime i_RegTime;
    mDateTime i_LastUsed;
    mstring i_Founder;
    mstring i_CoFounder;
    mstring i_Description;
    mstring i_Password;
    mstring i_Email;
    mstring i_URL;
    mstring i_Comment;
    map<mstring, unsigned int> failed_passwds;

    mstring i_Mlock_On;
    mstring l_Mlock_On;
    mstring i_Mlock_Off;
    mstring l_Mlock_Off;
    mstring i_Mlock_Key;
    unsigned int i_Mlock_Limit;
    mstring i_Topic;
    mstring i_Topic_Setter;
    mDateTime i_Topic_Set_Time;
    
    unsigned long i_Bantime;
    bool l_Bantime;
    bool i_Keeptopic;
    bool l_Keeptopic;
    bool i_Topiclock;
    bool l_Topiclock;
    bool i_Private;
    bool l_Private;
    bool i_Secureops;
    bool l_Secureops;
    bool i_Secure;
    bool l_Secure;
    bool i_NoExpire;
    bool l_NoExpire;
    bool i_Anarchy;
    bool l_Anarchy;
    bool i_Restricted;
    bool l_Restricted;
    bool i_Join;
    bool l_Join;
    bool i_Forbidden;
    mstring i_Revenge;
    bool l_Revenge;

    mstring i_Suspend_By;
    mDateTime i_Suspend_Time;

    set<entlist_val_t<long> > i_Level;
    set<entlist_val_t<long> > i_Access;
    set<entlist_val_t<mstring> > i_Akick;
    list<entlist_t> i_Greet;
    list<entlist_t> i_Message;

    void ChgAttempt(mstring nick, mstring newnick);
    void Join(mstring nick);
    void Part(mstring nick);
    void Kick(mstring nick, mstring kicker);
    void Topic(mstring topic, mstring setter, mDateTime time);
    void Mode(mstring setter, mstring mode);
    void defaults();
public:
    Chan_Stored_t() {}
    Chan_Stored_t(const Chan_Stored_t& in) { *this = in; }
    Chan_Stored_t(mstring name, mstring founder, mstring password, mstring desc);
    Chan_Stored_t(mstring name); // Forbidden
    void operator=(const Chan_Stored_t &in);
    bool operator==(const Chan_Stored_t &in) const
	{ return (i_Name == in.i_Name); }
    bool operator!=(const Chan_Stored_t &in) const
	{ return (i_Name != in.i_Name); }
    bool operator<(const Chan_Stored_t &in) const
	{ return (i_Name < in.i_Name); }

    mstring Name()			{ return i_Name; }
    mDateTime RegTime()			{ return i_RegTime; }
    mDateTime LastUsed();
    void Founder(mstring in);
    void CoFounder(mstring in);
    mstring Founder()			{ return i_Founder; }
    mstring CoFounder()			{ return i_CoFounder; }
    void Description(mstring in)	{ i_Description = in; }
    mstring Description()		{ return i_Description; }
    void Password(mstring in)		{ i_Password = in; }
    mstring Password()			{ return i_Password; }
    unsigned int CheckPass(mstring nick, mstring pass);
    void Email(mstring in)		{ i_Email = in; }
    mstring Email()			{ return i_Email; }
    void URL(mstring in)		{ i_URL = in; }
    mstring URL()			{ return i_URL; }
    void Comment(mstring in)		{ i_Comment = in; }
    mstring Comment()			{ return i_Comment; }

    void Suspend(mstring name, mstring reason)
	{ Comment(reason); Suspend(name); }
    void Suspend(mstring name);
    void UnSuspend();

    mstring Mlock_Off()			{ return i_Mlock_Off + l_Mlock_Off; }
    mstring Mlock_On()			{ return i_Mlock_On + l_Mlock_On; }
    mstring Mlock();
    vector<mstring> Mlock(mstring source, mstring mode);
    mstring L_Mlock();
    vector<mstring> L_Mlock(mstring source, mstring mode);
    mstring Mlock_Key()			{ return i_Mlock_Key; }
    unsigned int Mlock_Limit()		{ return i_Mlock_Limit; }
    mstring Last_Topic()		{ return i_Topic; }
    mstring Last_Topic_Setter()		{ return i_Topic_Setter; }
    mDateTime Last_Topic_Set_Time()	{ return i_Topic_Set_Time; }

    unsigned long Bantime();
    void Bantime(unsigned long in);
    bool L_Bantime();
    void L_Bantime(bool in);
    bool Keeptopic();
    void Keeptopic(bool in);
    bool L_Keeptopic();
    void L_Keeptopic(bool in);
    bool Topiclock();
    void Topiclock(bool in);
    bool L_Topiclock();
    void L_Topiclock(bool in);
    bool Private();
    void Private(bool in);
    bool L_Private();
    void L_Private(bool in);
    bool Secureops();
    void Secureops(bool in);
    bool L_Secureops();
    void L_Secureops(bool in);
    bool Secure();
    void Secure(bool in);
    bool L_Secure();
    void L_Secure(bool in);
    bool NoExpire();
    void NoExpire(bool in);
    bool L_NoExpire();
    void L_NoExpire(bool in);
    bool Anarchy();
    void Anarchy(bool in);
    bool L_Anarchy();
    void L_Anarchy(bool in);
    bool Restricted();
    void Restricted(bool in);
    bool L_Restricted();
    void L_Restricted(bool in);
    bool Join();
    void Join(bool in);
    bool L_Join();
    void L_Join(bool in);
    mstring Revenge();
    bool Revenge(mstring in);
    bool L_Revenge();
    void L_Revenge(bool in);

    bool Suspended()			{ return (i_Suspend_By != ""); }
    mstring Suspend_By()		{ return i_Suspend_By; }
    mDateTime Suspend_Time()		{ return i_Suspend_Time; }
    bool Forbidden()			{ return i_Forbidden; }

    // FIND: Looks for EXACT MATCH of passed entry.
//  bool Level_insert(mstring entry, long value, mstring nick);
//  bool Level_erase();
    bool Level_change(mstring entry, long value, mstring nick);
    set<entlist_val_t<long> >::iterator Level_begin()
	{ return i_Level.begin(); }
    set<entlist_val_t<long> >::iterator Level_end()
	{ return i_Level.end(); }
    size_t Level_size()			{ return i_Level.size(); }
    bool Level_find(mstring entry);
    long Level_value(mstring entry);
    set<entlist_val_t<long> >::iterator Level;

    // FIND: Looks for EXACT MATCH of passed entry, if !found,
    //       then if entry contains "@", REGEX match on entry,
    //       else REGEX match on mask of Nick_Live_t[entry].
    // INSERT: Adds if:
    //     1) Entry !has @ and is reg'd nick.
    //     2) Entry has (* or ?) and @.
    //     3) Entry has @ and no (* or ?).
    bool Access_insert(mstring entry, long value, mstring nick, mDateTime modtime = Now());
    bool Access_erase();
    set<entlist_val_t<long> >::iterator Access_begin()
	{ return i_Access.begin(); }
    set<entlist_val_t<long> >::iterator Access_end()
	{ return i_Access.end(); }
    size_t Access_size()			{ return i_Access.size(); }
    bool Access_find(mstring entry, bool looklive = true);
    long Access_value(mstring entry, bool looklive = true);
    set<entlist_val_t<long> >::iterator Access;
    long GetAccess(mstring entry);
    bool GetAccess(mstring entry, mstring type);

    // FIND: Looks for EXACT MATCH of passed entry, if !found,
    //       then if entry contains "@", REGEX match on entry,
    //       else REGEX match on mask of Nick_Live_t[entry].
    // INSERT: Adds if:
    //     1) Entry !has @ and is reg'd nick.
    //     2) Entry has (* or ?) and @.
    //     3) Entry has @ and no (* or ?).
    bool Akick_insert(mstring entry, mstring value, mstring nick, mDateTime modtime = Now());
    bool Akick_insert(mstring entry, mstring nick, mDateTime modtime = Now());
    bool Akick_erase();
    set<entlist_val_t<mstring> >::iterator Akick_begin()
	{ return i_Akick.begin(); }
    set<entlist_val_t<mstring> >::iterator Akick_end()
	{ return i_Akick.end(); }
    size_t Akick_size()				{ return i_Akick.size(); }
    bool Akick_find(mstring entry, bool looklive = true);
    mstring Akick_string(mstring entry, bool looklive = true);
    set<entlist_val_t<mstring> >::iterator Akick;

    // FIND: Looks for EXACT MATCH of nick entry.
    // INSERT: Adds if not found.
    bool Greet_insert(mstring entry, mstring nick, mDateTime modtime = Now());
    bool Greet_erase();
    entlist_i Greet_begin()			{ return i_Greet.begin(); }
    entlist_i Greet_end()			{ return i_Greet.end(); }
    size_t Greet_size()				{ return i_Greet.size(); }
    bool Greet_find(mstring nick);
    entlist_i Greet;

    // FIND: Looks for NUMBER of entry.
    // INSERT: Adds if not found.
    bool Message_insert(mstring entry, mstring nick);
    bool Message_erase();
    entlist_i Message_begin()			{ return i_Message.begin(); }
    entlist_i Message_end()			{ return i_Message.end(); }
    size_t Message_size()			{ return i_Message.size(); }
    bool Message_find(int num);
    entlist_i Message;
};

wxOutputStream &operator<<(wxOutputStream& out,Chan_Stored_t& in);
wxInputStream &operator>>(wxInputStream& in, Chan_Stored_t& out);

class ChanServ : public mBase
{
    friend class Magick;
private:

    set<mstring> Revenge_Levels;

    // Config Entries ...
    unsigned long expire;			// How long to keep channels
    mstring def_akick_reason;	// Default AKICK reason
    unsigned int passfail;		// How many times they can fail ident
    unsigned long chankeep;	// Time to keep channel after AKICK
    mstring def_mlock;		// Default MLOCK string
    mstring lck_mlock;		// Locked MLOCK modes
    unsigned long def_bantime;	// Default time to keep bans (minutes)
    bool lck_bantime;		// BANTIME is locked?
    bool def_keeptopic;		// Default val of KEEPTOPIC
    bool lck_keeptopic;		// KEEPTOPIC is locked?
    bool def_topiclock;		// Default val of TOPICLOCK
    bool lck_topiclock;		// TOPICLOCK is locked?
    bool def_private;		// Default value of PRIVATE
    bool lck_private;		// PRIVATE is locked?
    bool def_secureops;		// Default value of SECUREOPS
    bool lck_secureops;		// SECUREOPS is locked?
    bool def_secure;		// Default value of SECURE
    bool lck_secure;		// SECURE is locked?
    bool def_noexpire;		// Default value of NOEXPIRE
    bool lck_noexpire;		// NOEXPIRE is locked?
    bool def_anarchy;		// Default value of ANARCHY
    bool lck_anarchy;		// ANARCHY is locked?
    bool def_restricted;	// Default value of RESTRICTED
    bool lck_restricted;	// RESTRICTED is locked?
    bool def_join;		// Default value of JOIN
    bool lck_join;		// JOIN is locked?
    mstring def_revenge;	// Default REVENGE level
    bool lck_revenge;		// REVENGE is locked?
    long level_min;		// Minimum access level
    long level_max;		// Maximum access level
    map<mstring, long> lvl;

    void AddCommands();
    void RemCommands();
protected:

public:
    bool IsRevengeLevel(mstring level)
	{ return (Revenge_Levels.find(level.UpperCase()) !=
				    Revenge_Levels.end()); }
    unsigned long Expire()	{ return expire; }
    mstring DEF_Akick_Reason()	{ return def_akick_reason; }
    unsigned int Passfail()	{ return passfail; }
    unsigned long ChanKeep()	{ return chankeep; }
    mstring DEF_MLock()		{ return def_mlock; }
    mstring LCK_MLock()		{ return lck_mlock; }
    unsigned long DEF_Bantime()	{ return def_bantime; }
    bool LCK_Bantime()	        { return lck_bantime; }
    bool DEF_Keeptopic()	{ return def_keeptopic; }
    bool LCK_Keeptopic()	{ return lck_keeptopic; }
    bool DEF_Topiclock()	{ return def_topiclock; }
    bool LCK_Topiclock()	{ return lck_topiclock; }
    bool DEF_Private()		{ return def_private; }
    bool LCK_Private()		{ return lck_private; }
    bool DEF_Secureops()	{ return def_secureops; }
    bool LCK_Secureops()	{ return lck_secureops; }
    bool DEF_Secure()		{ return def_secure; }
    bool LCK_Secure()		{ return lck_secure; }
    bool DEF_NoExpire()		{ return def_noexpire; }
    bool LCK_NoExpire()		{ return lck_noexpire; }
    bool DEF_Anarchy()		{ return def_anarchy; }
    bool LCK_Anarchy()		{ return lck_anarchy; }
    bool DEF_Restricted()	{ return def_restricted; }
    bool LCK_Restricted()	{ return lck_restricted; }
    bool DEF_Join()		{ return def_join; }
    bool LCK_Join()		{ return lck_join; }
    mstring DEF_Revenge()	{ return def_revenge; }
    bool LCK_Revenge()		{ return lck_revenge; }
    long Level_Min()		{ return level_min; }
    long Level_Max()		{ return level_max; }
    long LVL(mstring level);
    bool IsLVL(mstring level);
    vector<mstring> LVL();

    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);
    bool IsStored(mstring in);
    bool IsLive(mstring in);
    map<mstring,Chan_Stored_t> stored;
    map<mstring,Chan_Live_t> live;
    Part_Handler ph;

    ChanServ();
    virtual threadtype_enum Get_TType() const { return tt_ChanServ; }
    virtual mstring GetInternalName() const { return "ChanServ"; }
    virtual void execute(const mstring & message);

    static void do_Help(mstring mynick, mstring source, mstring params);
    static void do_Register(mstring mynick, mstring source, mstring params);
    static void do_Drop(mstring mynick, mstring source, mstring params);
    static void do_Identify(mstring mynick, mstring source, mstring params);
    static void do_Info(mstring mynick, mstring source, mstring params);
    static void do_List(mstring mynick, mstring source, mstring params);
    static void do_Suspend(mstring mynick, mstring source, mstring params);
    static void do_UnSuspend(mstring mynick, mstring source, mstring params);
    static void do_Forbid(mstring mynick, mstring source, mstring params);
    static void do_Getpass(mstring mynick, mstring source, mstring params);

    static void do_Mode(mstring mynick, mstring source, mstring params);
    static void do_Op(mstring mynick, mstring source, mstring params);
    static void do_DeOp(mstring mynick, mstring source, mstring params);
    static void do_Voice(mstring mynick, mstring source, mstring params);
    static void do_DeVoice(mstring mynick, mstring source, mstring params);
    static void do_Topic(mstring mynick, mstring source, mstring params);
    static void do_Kick(mstring mynick, mstring source, mstring params);
    static void do_AnonKick(mstring mynick, mstring source, mstring params);
    static void do_Users(mstring mynick, mstring source, mstring params);
    static void do_Invite(mstring mynick, mstring source, mstring params);
    static void do_Unban(mstring mynick, mstring source, mstring params);

    static void do_clear_Users(mstring mynick, mstring source, mstring params);
    static void do_clear_Modes(mstring mynick, mstring source, mstring params);
    static void do_clear_Ops(mstring mynick, mstring source, mstring params);
    static void do_clear_Voices(mstring mynick, mstring source, mstring params);
    static void do_clear_Bans(mstring mynick, mstring source, mstring params);
    static void do_clear_All(mstring mynick, mstring source, mstring params);
    static void do_level_Set(mstring mynick, mstring source, mstring params);
    static void do_level_Reset(mstring mynick, mstring source, mstring params);
    static void do_level_List(mstring mynick, mstring source, mstring params);
    static void do_access_Add(mstring mynick, mstring source, mstring params);
    static void do_access_Del(mstring mynick, mstring source, mstring params);
    static void do_access_List(mstring mynick, mstring source, mstring params);
    static void do_akick_Add(mstring mynick, mstring source, mstring params);
    static void do_akick_Del(mstring mynick, mstring source, mstring params);
    static void do_akick_List(mstring mynick, mstring source, mstring params);
    static void do_greet_Add(mstring mynick, mstring source, mstring params);
    static void do_greet_Del(mstring mynick, mstring source, mstring params);
    static void do_greet_List(mstring mynick, mstring source, mstring params);
    static void do_message_Add(mstring mynick, mstring source, mstring params);
    static void do_message_Del(mstring mynick, mstring source, mstring params);
    static void do_message_List(mstring mynick, mstring source, mstring params);
    static void do_set_Founder(mstring mynick, mstring source, mstring params);
    static void do_set_CoFounder(mstring mynick, mstring source, mstring params);
    static void do_set_Description(mstring mynick, mstring source, mstring params);
    static void do_set_Password(mstring mynick, mstring source, mstring params);
    static void do_set_Email(mstring mynick, mstring source, mstring params);
    static void do_set_URL(mstring mynick, mstring source, mstring params);
    static void do_set_Comment(mstring mynick, mstring source, mstring params);
    static void do_set_Mlock(mstring mynick, mstring source, mstring params);
    static void do_set_BanTime(mstring mynick, mstring source, mstring params);
    static void do_set_KeepTopic(mstring mynick, mstring source, mstring params);
    static void do_set_TopicLock(mstring mynick, mstring source, mstring params);
    static void do_set_Private(mstring mynick, mstring source, mstring params);
    static void do_set_SecureOps(mstring mynick, mstring source, mstring params);
    static void do_set_Secure(mstring mynick, mstring source, mstring params);
    static void do_set_NoExpire(mstring mynick, mstring source, mstring params);
    static void do_set_Anarchy(mstring mynick, mstring source, mstring params);
    static void do_set_Restricted(mstring mynick, mstring source, mstring params);
    static void do_set_Join(mstring mynick, mstring source, mstring params);
    static void do_set_Revenge(mstring mynick, mstring source, mstring params);
    static void do_lock_Mlock(mstring mynick, mstring source, mstring params);
    static void do_lock_BanTime(mstring mynick, mstring source, mstring params);
    static void do_lock_KeepTopic(mstring mynick, mstring source, mstring params);
    static void do_lock_TopicLock(mstring mynick, mstring source, mstring params);
    static void do_lock_Private(mstring mynick, mstring source, mstring params);
    static void do_lock_SecureOps(mstring mynick, mstring source, mstring params);
    static void do_lock_Secure(mstring mynick, mstring source, mstring params);
    static void do_lock_Anarchy(mstring mynick, mstring source, mstring params);
    static void do_lock_Restricted(mstring mynick, mstring source, mstring params);
    static void do_lock_Join(mstring mynick, mstring source, mstring params);
    static void do_lock_Revenge(mstring mynick, mstring source, mstring params);
    static void do_unlock_Mlock(mstring mynick, mstring source, mstring params);
    static void do_unlock_BanTime(mstring mynick, mstring source, mstring params);
    static void do_unlock_KeepTopic(mstring mynick, mstring source, mstring params);
    static void do_unlock_TopicLock(mstring mynick, mstring source, mstring params);
    static void do_unlock_Private(mstring mynick, mstring source, mstring params);
    static void do_unlock_SecureOps(mstring mynick, mstring source, mstring params);
    static void do_unlock_Secure(mstring mynick, mstring source, mstring params);
    static void do_unlock_Anarchy(mstring mynick, mstring source, mstring params);
    static void do_unlock_Restricted(mstring mynick, mstring source, mstring params);
    static void do_unlock_Join(mstring mynick, mstring source, mstring params);
    static void do_unlock_Revenge(mstring mynick, mstring source, mstring params);
};

#endif
