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

#include "mstring.h"
#include "variant.h"
#include "base.h"

class Chan_Live_t : public mUserDef
{
    friend class Nick_Live_t;
    mstring i_Name;
    mDateTime i_Creation_Time;
    // below: .first == op .second==voice
    map<mstring, pair<bool, bool> > squit;
    map<mstring, pair<bool, bool> > users;
    set<mstring> bans;
    mstring i_Topic;
    mstring i_Topic_Setter;
    mDateTime i_Topic_Set_Time;
    mstring modes;
    int i_Limit;
    mstring i_Key;

    void Join(mstring nick); // Called by Nick_Live_t
    int Part(mstring nick); // Called by Nick_Live_t
    void Squit(mstring nick); // Called by Nick_Live_t
    void UnSquit(mstring nick); // Called by Nick_Live_t
    int Kick(mstring nick, mstring kicker); // Called by Nick_Live_t
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

    void Topic(mstring topic, mstring setter);
    void Topic(mstring topic, mstring setter, mDateTime time);
    mstring Topic()		{ return i_Topic; }
    mstring Topic_Setter()	{ return i_Topic_Setter; }
    mDateTime Topic_Set_Time()	{ return i_Topic_Set_Time; }

    int Squit();
    mstring Squit(int num);
    int Users();
    mstring User(int num);
    int Ops();
    mstring Op(int num);
    int Voices();
    mstring Voice(int num);
    pair<bool, bool> User(mstring name);
    bool IsSquit(mstring nick);
    bool IsIn(mstring nick);
    bool IsOp(mstring nick);
    bool IsVoice(mstring nick);

    void SendMode(mstring source, mstring in);	// out
    void SendMode(mstring in);			// out
    void Mode(mstring source, mstring in);	// in
    bool HasMode(mstring in)	{ return modes.Contains(in); }
    mstring Mode()		{ return modes; }
    mstring Key()		{ return i_Key; }
    int Limit()			{ return i_Limit; }

};

bool checkops(pair<const mstring, pair<bool,bool> > &in);
bool checkvoices(pair<const mstring, pair<bool,bool> > &in);

class Chan_Stored_t : public mUserDef
{
    friend class Nick_Live_t;
    friend class Chan_Live_t;
    friend wxOutputStream &operator<<(wxOutputStream& out,Chan_Stored_t& in);
    friend wxInputStream &operator>>(wxInputStream& in, Chan_Stored_t& out);

    mstring i_Name;
    mDateTime i_RegTime;
    mDateTime i_LastUsed;
    mstring i_Founder;
    mstring i_Description;
    mstring i_Password;
    mstring i_Email;
    mstring i_URL;
    mstring i_Comment;
    map<mstring, int> failed_passwds;

    mstring i_Mlock_On;
    mstring l_Mlock_On;
    mstring i_Mlock_Off;
    mstring l_Mlock_Off;
    mstring i_Mlock_Key;
    unsigned long i_Mlock_Limit;
    mstring i_Topic;
    mstring i_Topic_Setter;
    mDateTime i_Topic_Set_Time;
    
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
    bool i_Restricted;
    bool l_Restricted;
    bool i_Join;
    bool l_Join;
    bool i_Forbidden;
    mstring i_Revenge;
    bool l_Revenge;

    mstring i_Suspend_By;
    mDateTime i_Suspend_Time;

    set<entlist_val_t<long> > i_Access_Level;
    set<entlist_val_t<long> > i_Access;
    set<entlist_val_t<mstring> > i_Akick;
    list<entlist_t> i_Greet;

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
    mDateTime LastUsed()		{ return i_LastUsed; }
    void Description(mstring in)	{ i_Description = in; }
    mstring Description()		{ return i_Description; }
    void Password(mstring in)		{ i_Password = in; }
    mstring Password()			{ return i_Password; }
    int CheckPass(mstring nick, mstring pass);
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

    mstring Mlock();
    mstring Mlock(mstring mode);
    mstring L_Mlock();
    mstring L_Mlock(mstring mode);
    mstring Mlock_Key()			{ return i_Mlock_Key; }
    int Mlock_Limit()			{ return i_Mlock_Limit; }

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
    bool L_Revenge(mstring in);

    bool Suspended()			{ return (i_Suspend_By != ""); }
    mstring Suspend_By()		{ return i_Suspend_By; }
    mDateTime Suspend_Time()		{ return i_Suspend_Time; }
    bool Forbidden()			{ return i_Forbidden; }

    // FIND: Looks for EXACT MATCH of passed entry.
//  bool Access_Level_insert(mstring entry, long value, mstring nick);
//  bool Access_Level_erase();

//  entlist_val_ui<long> Access_Level_begin()	{ return i_Access_Level.begin(); }
    set<entlist_val_t<long> >::iterator Access_Level_begin()
	{ return i_Access_Level.begin(); }
//  entlist_val_ui<long> Access_Level_end()	{ return i_Access_Level.end(); }
    set<entlist_val_t<long> >::iterator Access_Level_end()
	{ return i_Access_Level.end(); }
    size_t Access_Level_size()			{ return i_Access_Level.size(); }
    bool Access_Level_find(mstring entry);
    long Access_Level_value(mstring entry);
//  entlist_val_ui<long> Access_Level;
    set<entlist_val_t<long> >::iterator Access_Level;

    // FIND: Looks for EXACT MATCH of passed entry, if !found,
    //       then if entry contains "@", REGEX match on entry,
    //       else REGEX match on mask of Nick_Live_t[entry].
    // INSERT: Adds if:
    //     1) Entry !has @ and is reg'd nick.
    //     2) Entry has (* or ?) and @.
    //     3) Entry has @ and no (* or ?).
    bool Access_insert(mstring entry, long value, mstring nick);
    bool Access_erase();
//  entlist_val_ui<long> Access_begin()		{ return i_Access.begin(); }
    set<entlist_val_t<long> >::iterator Access_begin()
	{ return i_Access.begin(); }
//  entlist_val_ui<long> Access_end()		{ return i_Access.end(); }
    set<entlist_val_t<long> >::iterator Access_end()
	{ return i_Access.end(); }
    size_t Access_size()			{ return i_Access.size(); }
    bool Access_find(mstring entry);
    long Access_value(mstring entry);
//  entlist_val_ui<long> Access;
    set<entlist_val_t<long> >::iterator Access;

    // FIND: Looks for EXACT MATCH of passed entry, if !found,
    //       then if entry contains "@", REGEX match on entry,
    //       else REGEX match on mask of Nick_Live_t[entry].
    // INSERT: Adds if:
    //     1) Entry !has @ and is reg'd nick.
    //     2) Entry has (* or ?) and @.
    //     3) Entry has @ and no (* or ?).
    bool Akick_insert(mstring entry, mstring value, mstring nick);
    bool Akick_insert(mstring entry, mstring nick);
    bool Akick_erase();
//  entlist_val_ui<mstring> Akick_begin()	{ return i_Akick.begin(); }
    set<entlist_val_t<mstring> >::iterator Akick_begin()
	{ return i_Akick.begin(); }
//  entlist_val_ui<mstring> Akick_end()		{ return i_Akick.end(); }
    set<entlist_val_t<mstring> >::iterator Akick_end()
	{ return i_Akick.end(); }
    size_t Akick_size()				{ return i_Akick.size(); }
    bool Akick_find(mstring entry);
    mstring Akick_string(mstring entry);
//  entlist_val_ui<mstring> Akick;
    set<entlist_val_t<mstring> >::iterator Akick;

    // FIND: Looks for REGEX MATCH of passed entry.
    // INSERT: Adds if not found.
    bool Greet_insert(mstring entry, mstring nick);
    bool Greet_erase();
    entlist_i Greet_begin()			{ return i_Greet.begin(); }
    entlist_i Greet_end()			{ return i_Greet.end(); }
    size_t Greet_size()				{ return i_Greet.size(); }
    bool Greet_find(mstring entry);
    entlist_i Greet;
    
};

wxOutputStream &operator<<(wxOutputStream& out,Chan_Stored_t& in);
wxInputStream &operator>>(wxInputStream& in, Chan_Stored_t& out);


class ChanServ : public mBase
{
    friend class Magick;
private:

    // Config Entries ...
    int expire;			// How long to keep channels
    mstring def_akick_reason;	// Default AKICK reason
    int passfail;		// How many times they can fail ident
    int chankeep;		// Time to keep channel after AKICK
    mstring def_mlock;		// Default MLOCK string
    mstring lck_mlock;		// Locked MLOCK modes
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
    bool def_restricted;	// Default value of RESTRICTED
    bool lck_restricted;	// RESTRICTED is locked?
    bool def_join;		// Default value of JOIN
    bool lck_join;		// JOIN is locked?
    mstring def_revenge;	// Default REVENGE level
    bool lck_revenge;		// REVENGE is locked?
    int level_min;		// Minimum access level
    int level_max;		// Maximum access level
    int lvl_autodeop;		// Default level for AUTODEOP
    int lvl_autovoice;		// Default level for AUTOVOICE
    int lvl_autoop;		// Default level for AUTOOP
    int lvl_readmemo;		// Default level for READMEMO
    int lvl_writememo;		// Default level for WRITEMEMO
    int lvl_delmemo;		// Default level for DELMEMO
    int lvl_akick;		// Default level for AKICK
    int lvl_starakick;		// Default level for STARAKICK
    int lvl_unban;		// Default level for UNBAN
    int lvl_access;		// Default level for ACCESS
    int lvl_set;		// Default level for SET
    int lvl_cmdinvite;		// Default level for CMDINVITE
    int lvl_cmdunban;		// Default level for CMDUNBAN
    int lvl_cmdvoice;		// Default level for CMDVOICE
    int lvl_cmdop;		// Default level for CMDOP
    int lvl_cmdclear;		// Default level for CMDCLEAR

protected:

public:
    int	Expire()		{ return expire; }
    mstring DEF_Akick_Reason()	{ return def_akick_reason; }
    int Passfail()		{ return passfail; }
    int	ChanKeep()		{ return chankeep; }
    mstring DEF_MLock()		{ return def_mlock; }
    mstring LCK_MLock()		{ return lck_mlock; }
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
    bool DEF_Restricted()	{ return def_restricted; }
    bool LCK_Restricted()	{ return lck_restricted; }
    bool DEF_Join()		{ return def_join; }
    bool LCK_Join()		{ return lck_join; }
    mstring DEF_Revenge()	{ return def_revenge; }
    bool LCK_Revenge()		{ return lck_revenge; }
    int	Level_Min()		{ return level_min; }
    int	Level_Max()		{ return level_max; }
    int	LVL_Autodeop()		{ return lvl_autodeop; }
    int	LVL_Autovoice()		{ return lvl_autovoice; }
    int	LVL_Autoop()		{ return lvl_autoop; }
    int	LVL_Readmemo()		{ return lvl_readmemo; }
    int	LVL_Writememo()		{ return lvl_writememo; }
    int	LVL_Delmemo()		{ return lvl_delmemo; }
    int	LVL_Akick()		{ return lvl_akick; }
    int	LVL_Starakick()		{ return lvl_starakick; }
    int	LVL_Unban()		{ return lvl_unban; }
    int	LVL_Access()		{ return lvl_access; }
    int	LVL_Set()		{ return lvl_set; }
    int	LVL_Cmdinvite()		{ return lvl_cmdinvite; }
    int	LVL_Cmdunban()		{ return lvl_cmdunban; }
    int	LVL_Cmdvoice()		{ return lvl_cmdvoice; }
    int	LVL_Cmdop()		{ return lvl_cmdop; }
    int	LVL_Cmdclear()		{ return lvl_cmdclear; }


    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);
    bool IsStored(mstring in);
    bool IsLive(mstring in);
    map<mstring,Chan_Stored_t> stored;
    map<mstring,Chan_Live_t> live;

    ChanServ();
    virtual threadtype_enum Get_TType() const { return tt_ChanServ; }
    virtual mstring GetInternalName() const { return "ChanServ"; }
    virtual void execute(const mstring & message);
};
#endif
