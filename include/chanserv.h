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
#ifndef _CHANSERV_H
#define _CHANSERV_H
#include "pch.h"
static const char *ident_chanserv_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.50  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.49  2000/12/22 19:50:19  prez
** Made all config options const.  Beginnings of securing all non-modifying
** commands to const.  also added serviceschk.
**
** Revision 1.48  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.47  2000/09/11 10:58:18  prez
** Now saves in in GMT
**
** Revision 1.46  2000/09/10 09:53:42  prez
** Added functionality to ensure the order of messages is kept.
**
** Revision 1.45  2000/09/02 07:20:44  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.44  2000/08/19 10:59:46  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.43  2000/08/10 22:44:22  prez
** Added 'binding to IP' options for shell servers, etc.  Also added akick
** triggers for when a user changes their nick and suddenly matches akick.
**
** Revision 1.42  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.41  2000/06/15 13:41:10  prez
** Added my tasks to develop *grin*
** Also did all the chanserv live locking (stored to be done).
** Also made magick check if its running, and kill on startup if so.
**
** Revision 1.40  2000/06/12 06:07:49  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.39  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.38  2000/05/17 07:47:57  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.37  2000/05/14 04:02:52  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.36  2000/05/08 14:42:01  prez
** More on xmlisation of nickserv and chanserv
**
** Revision 1.35  2000/04/02 13:06:02  prez
** Fixed the channel TOPIC and MODE LOCK stuff ...
**
** Also fixed the setting of both on join...
**
** Revision 1.34  2000/03/27 21:26:11  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.33  2000/03/27 10:40:11  prez
** Started implementing revenge
**
** Revision 1.32  2000/03/08 23:38:35  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.31  2000/03/02 07:25:10  prez
** Added stuff to do the chanserv greet timings (ie. only greet if a user has
** been OUT of channel over 'x' seconds).  New stored chanserv cfg item.
**
** Revision 1.30  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.29  2000/02/17 12:55:03  ungod
** still working on borlandization
**
** Revision 1.28  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#include "base.h"
#include "ircsocket.h"

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
    map<mstring, mDateTime> exempt;
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
    map<mstring, mDateTime> recent_parts;

    static bool ModeExists(mstring mode, vector<mstring> mode_params,
			bool change, char reqmode, mstring reqparam = "");
    static void RemoveMode(mstring mode, vector<mstring> mode_params,
			bool change, char reqmode, mstring reqparam = "");

    bool Join(mstring nick); // Called by Nick_Live_t
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

    mstring Name() const	{ return i_Name; }
    mDateTime Creation_Time() const;

    void Topic(mstring source, mstring topic, mstring setter, mDateTime time = Now());
    mstring Topic() const;
    mstring Topic_Setter() const;
    mDateTime Topic_Set_Time() const;

    unsigned int Squit() const;
    mstring Squit(unsigned int num) const;
    unsigned int Users() const;
    mstring User(unsigned int num) const;
    unsigned int Ops() const;
    mstring Op(unsigned int num) const;
    unsigned int Voices() const;
    mstring Voice(unsigned int num) const;
    pair<bool, bool> User(mstring name) const;
    unsigned int Bans() const;
    mstring Ban(unsigned int num) const;
    mDateTime Ban(mstring mask) const;
    unsigned int Exempts() const;
    mstring Exempt(unsigned int num) const;
    mDateTime Exempt(mstring mask) const;
    bool IsSquit(mstring nick) const;
    bool IsIn(mstring nick) const;
    bool IsOp(mstring nick) const;
    bool IsVoice(mstring nick) const;
    bool IsBan(mstring mask) const;
    bool MatchBan(mstring mask) const;
    bool IsExempt(mstring mask) const;
    bool MatchExempt(mstring mask) const;

    void LockDown();
    void UnLock();
    void SendMode(mstring in);			// out
    void Mode(mstring source, mstring in);	// in
    bool HasMode(mstring in) const;
    mstring Mode() const;
    mstring Key() const;
    unsigned int Limit() const;
    mDateTime PartTime(mstring nick) const;

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

struct ChanInfo;

class Chan_Stored_t : public mUserDef, public SXP::IPersistObj
{
    friend class Nick_Live_t;
    friend class Chan_Live_t;
    friend class ChanServ;
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
    unsigned long i_Parttime;
    bool l_Parttime;
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
    bool i_KickOnBan;
    bool l_KickOnBan;
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

    vector<entlist_val_t<long> *> level_array;
    vector<entlist_val_t<long> *> access_array;
    vector<entlist_val_t<mstring> *> akick_array;
    vector<entlist_t *> greet_array;
    vector<entlist_t *> message_array;

    static SXP::Tag tag_Chan_Stored_t, tag_Name, tag_RegTime, tag_LastUsed,
	tag_Founder, tag_CoFounder, tag_Description, tag_Password, tag_Email,
	tag_URL, tag_Comment, tag_Topic, tag_Topic_Setter, tag_Topic_Set_Time,
	tag_set_Mlock_On, tag_set_Mlock_Off, tag_set_Mlock_Key, tag_set_Mlock_Limit,
	tag_set_Bantime, tag_set_Parttime, tag_set_KeepTopic, tag_set_TopicLock,
	tag_set_Private, tag_set_SecureOps, tag_set_Secure, tag_set_NoExpire,
	tag_set_Anarchy, tag_set_KickOnBan, tag_set_Restricted, tag_set_Join,
	tag_set_Revenge, tag_Forbidden, tag_lock_Mlock_On, tag_lock_Mlock_Off,
	tag_lock_Bantime, tag_lock_Parttime, tag_lock_KeepTopic, tag_lock_TopicLock,
	tag_lock_Private, tag_lock_SecureOps, tag_lock_Secure, tag_lock_NoExpire,
	tag_lock_Anarchy, tag_lock_KickOnBan, tag_lock_Restricted, tag_lock_Join,
	tag_lock_Revenge, tag_Suspend_By, tag_Suspend_Time, tag_Level, tag_Access,
	tag_Akick, tag_Greet, tag_Message, tag_UserDef;

    void ChgAttempt(mstring nick, mstring newnick);
    bool Join(mstring nick);
    void Part(mstring nick);
    void Kick(mstring nick, mstring kicker);
    void ChgNick(mstring nick, mstring newnick);
    void Topic(mstring source, mstring topic, mstring setter, mDateTime time = Now());
    void Mode(mstring setter, mstring mode);
    void defaults();
    bool DoRevenge(mstring type, mstring target, mstring source);
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

    mstring Name() const		{ return i_Name; }
    mDateTime RegTime() const;
    mDateTime LastUsed();
    void SetTopic(mstring mynick, mstring topic, mstring setter);
    void Founder(mstring in);
    mstring Founder() const;
    void CoFounder(mstring in);
    mstring CoFounder() const;
    void Description(mstring in);
    mstring Description() const;
    void Password(mstring in);
    mstring Password() const;
    unsigned int CheckPass(mstring nick, mstring pass);
    void Email(mstring in);
    mstring Email() const;
    void URL(mstring in);
    mstring URL() const;
    void Comment(mstring in);
    mstring Comment() const;

    void Suspend(mstring name, mstring reason)
	{ Comment(reason); Suspend(name); }
    void Suspend(mstring name);
    void UnSuspend();

    mstring Mlock_Off() const;
    mstring Mlock_On() const;
    mstring Mlock() const;
    vector<mstring> Mlock(mstring source, mstring mode);
    mstring L_Mlock() const;
    vector<mstring> L_Mlock(mstring source, mstring mode);
    mstring Mlock_Key()	const;
    unsigned int Mlock_Limit() const;
    mstring Last_Topic() const;
    mstring Last_Topic_Setter() const;
    mDateTime Last_Topic_Set_Time() const;

    unsigned long Bantime() const;
    void Bantime(unsigned long in);
    bool L_Bantime() const;
    void L_Bantime(bool in);
    unsigned long Parttime() const;
    void Parttime(unsigned long in);
    bool L_Parttime() const;
    void L_Parttime(bool in);
    bool Keeptopic() const;
    void Keeptopic(bool in);
    bool L_Keeptopic() const;
    void L_Keeptopic(bool in);
    bool Topiclock() const;
    void Topiclock(bool in);
    bool L_Topiclock() const;
    void L_Topiclock(bool in);
    bool Private() const;
    void Private(bool in);
    bool L_Private() const;
    void L_Private(bool in);
    bool Secureops() const;
    void Secureops(bool in);
    bool L_Secureops() const;
    void L_Secureops(bool in);
    bool Secure() const;
    void Secure(bool in);
    bool L_Secure() const;
    void L_Secure(bool in);
    bool NoExpire() const;
    void NoExpire(bool in);
    bool L_NoExpire() const;
    void L_NoExpire(bool in);
    bool Anarchy() const;
    void Anarchy(bool in);
    bool L_Anarchy() const;
    void L_Anarchy(bool in);
    bool KickOnBan() const;
    void KickOnBan(bool in);
    bool L_KickOnBan() const;
    void L_KickOnBan(bool in);
    bool Restricted() const;
    void Restricted(bool in);
    bool L_Restricted() const;
    void L_Restricted(bool in);
    bool Join() const;
    void Join(bool in);
    bool L_Join() const;
    void L_Join(bool in);
    mstring Revenge() const;
    bool Revenge(mstring in);
    bool L_Revenge() const;
    void L_Revenge(bool in);

    bool Suspended() const;
    mstring Suspend_By() const;
    mDateTime Suspend_Time() const;
    bool Forbidden() const;

    // FIND: Looks for EXACT MATCH of passed entry.
//  bool Level_insert(mstring entry, long value, mstring nick);
//  bool Level_erase();
    bool Level_change(mstring entry, long value, mstring nick);
    set<entlist_val_t<long> >::iterator Level_begin()
	{ return i_Level.begin(); }
    set<entlist_val_t<long> >::iterator Level_end()
	{ return i_Level.end(); }
    size_t Level_size() const		{ return i_Level.size(); }
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
    size_t Access_size() const			{ return i_Access.size(); }
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
    size_t Akick_size() const			{ return i_Akick.size(); }
    bool Akick_find(mstring entry, bool looklive = true);
    mstring Akick_string(mstring entry, bool looklive = true);
    set<entlist_val_t<mstring> >::iterator Akick;

    // FIND: Looks for EXACT MATCH of nick entry.
    // INSERT: Adds if not found.
    bool Greet_insert(mstring entry, mstring nick, mDateTime modtime = Now());
    bool Greet_erase();
    entlist_i Greet_begin()			{ return i_Greet.begin(); }
    entlist_i Greet_end()			{ return i_Greet.end(); }
    size_t Greet_size() const			{ return i_Greet.size(); }
    bool Greet_find(mstring nick);
    entlist_i Greet;

    // FIND: Looks for NUMBER of entry.
    // INSERT: Adds if not found.
    bool Message_insert(mstring entry, mstring nick);
    bool Message_erase();
    entlist_i Message_begin()			{ return i_Message.begin(); }
    entlist_i Message_end()			{ return i_Message.end(); }
    size_t Message_size() const			{ return i_Message.size(); }
    bool Message_find(unsigned int num);
    entlist_i Message;

    SXP::Tag& GetClassTag() const { return tag_Chan_Stored_t; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

class ChanServ : public mBase, public SXP::IPersistObj
{
    friend class Magick;
private:

    set<mstring> Revenge_Levels;

    // Config Entries ...
    bool hide;			// Wether to set +s
    unsigned long expire;	// How long to keep channels
    unsigned long delay;	// How long between registrations
    unsigned int max_per_nick;	// Max channels per nickname
    unsigned int max_messages;	// Max messages per channel
    mstring def_akick_reason;	// Default AKICK reason
    unsigned int passfail;	// How many times they can fail ident
    unsigned long chankeep;	// Time to keep channel after AKICK
    mstring def_mlock;		// Default MLOCK string
    mstring lck_mlock;		// Locked MLOCK modes
    unsigned long def_bantime;	// Default time to keep bans (minutes)
    bool lck_bantime;		// BANTIME is locked?
    unsigned long def_parttime;	// Default time to not greet
    bool lck_parttime;		// PARTTIME is locked?
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
    bool def_kickonban;		// Default value of KICKONBAN
    bool lck_kickonban;		// KICKONBAN is locked?
    bool def_restricted;	// Default value of RESTRICTED
    bool lck_restricted;	// RESTRICTED is locked?
    bool def_join;		// Default value of JOIN
    bool lck_join;		// JOIN is locked?
    mstring def_revenge;	// Default REVENGE level
    bool lck_revenge;		// REVENGE is locked?
    long level_min;		// Minimum access level
    long level_max;		// Maximum access level
    map<mstring, long> lvl;
    static SXP::Tag tag_ChanServ;

    vector<Chan_Stored_t *> cs_array;

    void AddCommands();
    void RemCommands();
protected:

public:
    class stats_t
    {
	friend class ChanServ;

	mDateTime i_ClearTime;
	unsigned long i_Register;
	unsigned long i_Drop;
	unsigned long i_Identify; //
	unsigned long i_Suspend;
	unsigned long i_Unsuspend;
	unsigned long i_Forbid;
	unsigned long i_Getpass;
	unsigned long i_Mode;
	unsigned long i_Topic;
	unsigned long i_Op;
	unsigned long i_Deop;
	unsigned long i_Voice;
	unsigned long i_Devoice;
	unsigned long i_Kick;
	unsigned long i_Anonkick;
	unsigned long i_Invite;
	unsigned long i_Unban;
	unsigned long i_Clear;
	unsigned long i_Akick;
	unsigned long i_Level;
	unsigned long i_Access;
	unsigned long i_Greet;
	unsigned long i_Message;
	unsigned long i_Set;
	unsigned long i_NoExpire;
	unsigned long i_Lock;
	unsigned long i_Unlock;
	    
    public:
	stats_t() { clear(); }
	void clear() {
	    i_ClearTime = Now();
	    i_Register = i_Drop = i_Identify = i_Suspend =
		i_Unsuspend = i_Forbid = i_Getpass = i_Mode =
		i_Topic = i_Op = i_Deop = i_Voice = i_Devoice =
		i_Kick = i_Anonkick = i_Invite = i_Unban =
		i_Clear = i_Akick = i_Level = i_Access =
		i_Greet = i_Message = i_Set = i_NoExpire =
		i_Lock = i_Unlock = 0; }
	mDateTime ClearTime()const	    { return i_ClearTime; }
	unsigned long Register()const    { return i_Register; }
	unsigned long Drop()const	    { return i_Drop; }
	unsigned long Identify()const    { return i_Identify; }
	unsigned long Suspend()const	    { return i_Suspend; }
	unsigned long Unsuspend()const   { return i_Unsuspend; }
	unsigned long Forbid()const	    { return i_Forbid; }
	unsigned long Getpass()const	    { return i_Getpass; }
	unsigned long Mode()const	    { return i_Mode; }
	unsigned long Topic()const	    { return i_Topic; }
	unsigned long Op()const	    { return i_Op; }
	unsigned long Deop()const	    { return i_Deop; }
	unsigned long Voice()const	    { return i_Voice; }
	unsigned long Devoice()const	    { return i_Devoice; }
	unsigned long Kick()const	    { return i_Kick; }
	unsigned long Anonkick()const    { return i_Anonkick; }
	unsigned long Invite()const	    { return i_Invite; }
	unsigned long Unban()const	    { return i_Unban; }
	unsigned long Clear()const	    { return i_Clear; }
	unsigned long Akick()const	    { return i_Akick; }
	unsigned long Level()const	    { return i_Level; }
	unsigned long Access()const	    { return i_Access; }
	unsigned long Greet()const	    { return i_Greet; }
	unsigned long Message()const	    { return i_Message; }
	unsigned long Set()const	    { return i_Set; }
	unsigned long NoExpire()const    { return i_NoExpire; }
	unsigned long Lock()const	    { return i_Lock; }
	unsigned long Unlock()const	    { return i_Unlock; }
    } stats;

    bool IsRevengeLevel(mstring level)const
	{ return (Revenge_Levels.find(level.UpperCase()) !=
				    Revenge_Levels.end()); }
    bool Hide()const			{ return hide; }
    unsigned long Expire()const	{ return expire; }
    unsigned long Delay()const	{ return delay; }
    unsigned int Max_Per_Nick()const	{ return max_per_nick; }
    unsigned int Max_Messages()const	{ return max_messages; }
    mstring DEF_Akick_Reason()const	{ return def_akick_reason; }
    unsigned int Passfail()const	{ return passfail; }
    unsigned long ChanKeep()const	{ return chankeep; }
    mstring DEF_MLock()const		{ return def_mlock; }
    mstring LCK_MLock()const		{ return lck_mlock; }
    unsigned long DEF_Bantime()const	{ return def_bantime; }
    bool LCK_Bantime()const	        { return lck_bantime; }
    unsigned long DEF_Parttime()const{ return def_bantime; }
    bool LCK_Parttime()const	        { return lck_bantime; }
    bool DEF_Keeptopic()const	{ return def_keeptopic; }
    bool LCK_Keeptopic()const	{ return lck_keeptopic; }
    bool DEF_Topiclock()const	{ return def_topiclock; }
    bool LCK_Topiclock()const	{ return lck_topiclock; }
    bool DEF_Private()const		{ return def_private; }
    bool LCK_Private()const		{ return lck_private; }
    bool DEF_Secureops()const	{ return def_secureops; }
    bool LCK_Secureops()const	{ return lck_secureops; }
    bool DEF_Secure()const		{ return def_secure; }
    bool LCK_Secure()const		{ return lck_secure; }
    bool DEF_NoExpire()const		{ return def_noexpire; }
    bool LCK_NoExpire()const		{ return lck_noexpire; }
    bool DEF_Anarchy()const		{ return def_anarchy; }
    bool LCK_Anarchy()const		{ return lck_anarchy; }
    bool DEF_KickOnBan()const	{ return def_kickonban; }
    bool LCK_KickOnBan()const	{ return lck_kickonban; }
    bool DEF_Restricted()const	{ return def_restricted; }
    bool LCK_Restricted()const	{ return lck_restricted; }
    bool DEF_Join()const		{ return def_join; }
    bool LCK_Join()const		{ return lck_join; }
    mstring DEF_Revenge()const	{ return def_revenge; }
    bool LCK_Revenge()const		{ return lck_revenge; }
    long Level_Min()const		{ return level_min; }
    long Level_Max()const		{ return level_max; }
    long LVL(mstring level) const;
    bool IsLVL(mstring level)const;
    vector<mstring> LVL()const;

    bool IsStored(mstring in)const;
    bool IsLive(mstring in)const;
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
    static void do_Live(mstring mynick, mstring source, mstring params);

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
    static void do_set_PartTime(mstring mynick, mstring source, mstring params);
    static void do_set_KeepTopic(mstring mynick, mstring source, mstring params);
    static void do_set_TopicLock(mstring mynick, mstring source, mstring params);
    static void do_set_Private(mstring mynick, mstring source, mstring params);
    static void do_set_SecureOps(mstring mynick, mstring source, mstring params);
    static void do_set_Secure(mstring mynick, mstring source, mstring params);
    static void do_set_NoExpire(mstring mynick, mstring source, mstring params);
    static void do_set_Anarchy(mstring mynick, mstring source, mstring params);
    static void do_set_KickOnBan(mstring mynick, mstring source, mstring params);
    static void do_set_Restricted(mstring mynick, mstring source, mstring params);
    static void do_set_Join(mstring mynick, mstring source, mstring params);
    static void do_set_Revenge(mstring mynick, mstring source, mstring params);
    static void do_lock_Mlock(mstring mynick, mstring source, mstring params);
    static void do_lock_BanTime(mstring mynick, mstring source, mstring params);
    static void do_lock_PartTime(mstring mynick, mstring source, mstring params);
    static void do_lock_KeepTopic(mstring mynick, mstring source, mstring params);
    static void do_lock_TopicLock(mstring mynick, mstring source, mstring params);
    static void do_lock_Private(mstring mynick, mstring source, mstring params);
    static void do_lock_SecureOps(mstring mynick, mstring source, mstring params);
    static void do_lock_Secure(mstring mynick, mstring source, mstring params);
    static void do_lock_Anarchy(mstring mynick, mstring source, mstring params);
    static void do_lock_KickOnBan(mstring mynick, mstring source, mstring params);
    static void do_lock_Restricted(mstring mynick, mstring source, mstring params);
    static void do_lock_Join(mstring mynick, mstring source, mstring params);
    static void do_lock_Revenge(mstring mynick, mstring source, mstring params);
    static void do_unlock_Mlock(mstring mynick, mstring source, mstring params);
    static void do_unlock_BanTime(mstring mynick, mstring source, mstring params);
    static void do_unlock_PartTime(mstring mynick, mstring source, mstring params);
    static void do_unlock_KeepTopic(mstring mynick, mstring source, mstring params);
    static void do_unlock_TopicLock(mstring mynick, mstring source, mstring params);
    static void do_unlock_Private(mstring mynick, mstring source, mstring params);
    static void do_unlock_SecureOps(mstring mynick, mstring source, mstring params);
    static void do_unlock_Secure(mstring mynick, mstring source, mstring params);
    static void do_unlock_Anarchy(mstring mynick, mstring source, mstring params);
    static void do_unlock_KickOnBan(mstring mynick, mstring source, mstring params);
    static void do_unlock_Restricted(mstring mynick, mstring source, mstring params);
    static void do_unlock_Join(mstring mynick, mstring source, mstring params);
    static void do_unlock_Revenge(mstring mynick, mstring source, mstring params);

    virtual SXP::Tag& GetClassTag() const { return tag_ChanServ; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);
    void PostLoad();
};

#endif
