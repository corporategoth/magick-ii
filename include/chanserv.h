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
#ifndef _CHANSERV_H
#define _CHANSERV_H
#include "pch.h"
RCSID(chanserv_h, "@(#) $Id$");

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

class Chan_Live_t : public mUserDef, public ref_class
{
    friend class Nick_Live_t;
    friend class Part_Handler;
    friend void EventTask::do_check(mDateTime & synctime);
    friend void EventTask::do_modes(mDateTime & synctime);

    mstring i_Name;
    unsigned long i_Numeric;
    mDateTime i_Creation_Time;

    // below: .first == op .second == halfop .third == voice
    map < mstring, triplet < bool, bool, bool > > squit;
    map < mstring, triplet < bool, bool, bool > > users;

    map < mstring, mDateTime > bans;
    map < mstring, mDateTime > exempt;
    mstring i_Topic;
    mstring i_Topic_Setter;
    mDateTime i_Topic_Set_Time;
    mstring modes;
    unsigned int i_Limit;
    mstring i_Key;
    mstring p_modes_on;
    mstring p_modes_off;

    vector < mstring > p_modes_on_params;
    vector < mstring > p_modes_off_params;
    long ph_timer;

    map < mstring, mDateTime > recent_parts;

    static bool ModeExists(const mstring & mode, const vector < mstring > & mode_params, const bool change, const char reqmode,
			   const mstring & reqparam = "");
    static void RemoveMode(mstring & mode, vector < mstring > & mode_params, const bool change, const char reqmode,
			   const mstring & reqparam = "");

    bool Join(const mstring & nick);	// Called by Nick_Live_t
    unsigned int Part(const mstring & nick);	// Called by Nick_Live_t
    void SquitUser(const mstring & nick);	// Called by Nick_Live_t
    void UnSquitUser(const mstring & nick);	// Called by Nick_Live_t
    unsigned int Kick(const mstring & nick, const mstring & kicker);	// Called by Nick_Live_t
    void ChgNick(const mstring & nick, const mstring & newnick);	// Called by Nick_Live_t

public:

    Chan_Live_t();
    Chan_Live_t(const Chan_Live_t & in) : mUserDef(in), ref_class()
    {
	*this = in;
    }
    Chan_Live_t(const mstring & name, const mstring & first_user, const mDateTime & creation = mDateTime::CurrentDateTime());

    ~Chan_Live_t()
    {
    }
    Chan_Live_t &operator=(const Chan_Live_t & in);
    bool operator==(const Chan_Live_t & in) const
    {
	return (i_Name == in.i_Name);
    }
    bool operator!=(const Chan_Live_t & in) const
    {
	return (i_Name != in.i_Name);
    }
    bool operator<(const Chan_Live_t & in) const
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
    void Creation_Time(const mDateTime & in);
    mDateTime Creation_Time() const;

    void Topic(const mstring & source, const mstring & topic, const mstring & setter, const mDateTime & time =
	       mDateTime::CurrentDateTime());
    mstring Topic() const;
    mstring Topic_Setter() const;
    mDateTime Topic_Set_Time() const;

    unsigned long Numeric() const;
    void Numeric(const unsigned long num);

    unsigned int Squit() const;
    mstring Squit(const unsigned int num) const;
    unsigned int Users() const;
    mstring User(const unsigned int num) const;
    unsigned int Ops() const;
    mstring Op(const unsigned int num) const;
    unsigned int HalfOps() const;
    mstring HalfOp(const unsigned int num) const;
    unsigned int Voices() const;
    mstring Voice(const unsigned int num) const;
    triplet < bool, bool, bool > User(const mstring & name) const;
    unsigned int Bans() const;
    mstring Ban(const unsigned int num) const;
    mDateTime Ban(const mstring & mask) const;
    unsigned int Exempts() const;
    mstring Exempt(const unsigned int num) const;
    mDateTime Exempt(const mstring & mask) const;
    bool IsSquit(const mstring & nick) const;
    bool IsIn(const mstring & nick) const;
    bool IsOp(const mstring & nick) const;
    bool IsHalfOp(const mstring & nick) const;
    bool IsVoice(const mstring & nick) const;
    bool IsBan(const mstring & mask) const;
    bool MatchBan(const mstring & mask) const;
    bool IsExempt(const mstring & mask) const;
    bool MatchExempt(const mstring & mask) const;

    void LockDown();
    void UnLock();
    void SendMode(const mstring & in);	// out
    void Mode(const mstring & source, const mstring & in);	// in
    bool HasMode(const mstring & in) const;
    mstring Mode() const;
    mstring Key() const;
    unsigned int Limit() const;
    mDateTime PartTime(const mstring & nick) const;

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

struct ChanInfo_CUR;
struct ESP_ChannelInfo;
struct EPO_ChannelInfo;
struct HYB_ChanInfo;

class Chan_Stored_t : public mUserDef, public SXP::IPersistObj, public ref_class
{
    friend void Nick_Live_t::Join(const mstring & chan);
    friend set < mstring > Nick_Live_t::Name(const mstring & chan);
    friend void Nick_Live_t::Quit(const mstring & reason);
    friend class Chan_Live_t;
    friend class ChanServ;
    friend Chan_Stored_t *CreateChanEntry(ChanInfo_CUR * ci);
    friend Chan_Stored_t *ESP_CreateChanEntry(ESP_ChannelInfo * ci);
    friend Chan_Stored_t *EPO_CreateChanEntry(EPO_ChannelInfo * ci);
    friend Chan_Stored_t *HYB_CreateChanEntry(HYB_ChanInfo * ci);

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
    map < mstring, unsigned int > failed_passwds;

    mstring i_Topic;
    mstring i_Topic_Setter;
    mDateTime i_Topic_Set_Time;

    class setting_t
    {
	friend class Chan_Stored_t;
	friend class ChanServ;
	friend Chan_Stored_t *CreateChanEntry(ChanInfo_CUR * ci);
	friend Chan_Stored_t *ESP_CreateChanEntry(ESP_ChannelInfo * ci);
	friend Chan_Stored_t *EPO_CreateChanEntry(EPO_ChannelInfo * ci);
	friend Chan_Stored_t *HYB_CreateChanEntry(HYB_ChanInfo * ci);

	bool Keeptopic:1;
	bool Topiclock:1;
	bool Private:1;
	bool Secureops:1;
	bool Secure:1;
	bool Anarchy:1;
	bool KickOnBan:1;
	bool Restricted:1;
	bool Join:1;
	bool Forbidden:1;
	bool NoExpire:1;
	unsigned long Bantime;
	unsigned long Parttime;
	mstring Revenge;
	mstring Mlock_On;
	mstring Mlock_Off;
	mstring Mlock_Key;
	unsigned int Mlock_Limit;
    }
    setting;

    class lock_t
    {
	friend class Chan_Stored_t;
	bool Keeptopic:1;
	bool Topiclock:1;
	bool Private:1;
	bool Secureops:1;
	bool Secure:1;
	bool Anarchy:1;
	bool KickOnBan:1;
	bool Restricted:1;
	bool Join:1;
	bool Bantime:1;
	bool Parttime:1;
	bool Revenge:1;
	mstring Mlock_On;
	mstring Mlock_Off;
    }
    lock;

    mstring i_Suspend_By;
    mDateTime i_Suspend_Time;

    set < entlist_val_t < long > > i_Level;
    set < entlist_val_t < long > > i_Access;

    set < entlist_val_t < mstring > > i_Akick;
    list < entlist_t > i_Greet;
    list < entlist_t > i_Message;

    vector < entlist_val_t < long > * > level_array;
    vector < entlist_val_t < long > * > access_array;

    vector < entlist_val_t < mstring > * > akick_array;
    vector < entlist_t * > greet_array;
    vector < entlist_t * > message_array;

    static SXP::Tag tag_Chan_Stored_t, tag_Name, tag_RegTime, tag_LastUsed, tag_Founder, tag_CoFounder, tag_Description,
	tag_Password, tag_Email, tag_URL, tag_Comment, tag_Topic, tag_Topic_Setter, tag_Topic_Set_Time, tag_set_Mlock_On,
	tag_set_Mlock_Off, tag_set_Mlock_Key, tag_set_Mlock_Limit, tag_set_Bantime, tag_set_Parttime, tag_set_KeepTopic,
	tag_set_TopicLock, tag_set_Private, tag_set_SecureOps, tag_set_Secure, tag_set_NoExpire, tag_set_Anarchy,
	tag_set_KickOnBan, tag_set_Restricted, tag_set_Join, tag_set_Revenge, tag_Forbidden, tag_lock_Mlock_On,
	tag_lock_Mlock_Off, tag_lock_Bantime, tag_lock_Parttime, tag_lock_KeepTopic, tag_lock_TopicLock, tag_lock_Private,
	tag_lock_SecureOps, tag_lock_Secure, tag_lock_NoExpire, tag_lock_Anarchy, tag_lock_KickOnBan, tag_lock_Restricted,
	tag_lock_Join, tag_lock_Revenge, tag_Suspend_By, tag_Suspend_Time, tag_Level, tag_Access, tag_Akick, tag_Greet,
	tag_Message, tag_UserDef;

    void ChgAttempt(const mstring & nick, const mstring & newnick);
    bool Join(const mstring & nick);
    void Part(const mstring & nick);
    void Kick(const mstring & nick, const mstring & kicker);
    void ChgNick(const mstring & nick, const mstring & newnick);
    void Quit(const mstring & nick);
    void Topic(const mstring & source, const mstring & topic, const mstring & setter, const mDateTime & time =
	       mDateTime::CurrentDateTime());
    void Mode(const mstring & setter, const mstring & mode);
    bool DoRevenge(const mstring & type, const mstring & target, const mstring & source);
    void defaults();

public:
    Chan_Stored_t();
    Chan_Stored_t(const Chan_Stored_t & in) : mUserDef(in), SXP::IPersistObj(in), ref_class()
    {
	*this = in;
    }
    Chan_Stored_t(const mstring & name, const mstring & founder, const mstring & password, const mstring & desc);
    Chan_Stored_t(const mstring & name);	// Forbidden

    ~Chan_Stored_t()
    {
    }
    Chan_Stored_t &operator=(const Chan_Stored_t & in);
    bool operator==(const Chan_Stored_t & in) const
    {
	return (i_Name == in.i_Name);
    }
    bool operator!=(const Chan_Stored_t & in) const
    {
	return (i_Name != in.i_Name);
    }
    bool operator<(const Chan_Stored_t & in) const
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

    mDateTime LastUsed();
    void SetTopic(const mstring & mynick, const mstring & topic, const mstring & setter);
    void Founder(const mstring & in);
    mstring Founder() const;
    void CoFounder(const mstring & in);
    mstring CoFounder() const;
    void Description(const mstring & in);
    mstring Description() const;
    void Password(const mstring & in);
    mstring Password() const;
    unsigned int CheckPass(const mstring & nick, const mstring & pass);
    void Email(const mstring & in);
    mstring Email() const;
    void URL(const mstring & in);
    mstring URL() const;
    void Comment(const mstring & in);
    mstring Comment() const;

    void Suspend(const mstring & name, const mstring & reason)
    {
	Comment(reason);
	Suspend(name);
    }
    void Suspend(const mstring & name);
    void UnSuspend();

    mstring Mlock_Off() const;
    mstring Mlock_On() const;
    mstring Mlock() const;
    vector < mstring > Mlock(const mstring & source, const mstring & mode);
    mstring L_Mlock() const;
    vector < mstring > L_Mlock(const mstring & source, const mstring & mode);
    mstring Mlock_Key() const;
    unsigned int Mlock_Limit() const;
    mstring Last_Topic() const;
    mstring Last_Topic_Setter() const;
    mDateTime Last_Topic_Set_Time() const;

    unsigned long Bantime() const;
    void Bantime(const unsigned long in);
    bool L_Bantime() const;
    void L_Bantime(const bool in);
    unsigned long Parttime() const;
    void Parttime(const unsigned long in);
    bool L_Parttime() const;
    void L_Parttime(const bool in);
    bool Keeptopic() const;
    void Keeptopic(const bool in);
    bool L_Keeptopic() const;
    void L_Keeptopic(const bool in);
    bool Topiclock() const;
    void Topiclock(const bool in);
    bool L_Topiclock() const;
    void L_Topiclock(const bool in);
    bool Private() const;
    void Private(const bool in);
    bool L_Private() const;
    void L_Private(const bool in);
    bool Secureops() const;
    void Secureops(const bool in);
    bool L_Secureops() const;
    void L_Secureops(const bool in);
    bool Secure() const;
    void Secure(const bool in);
    bool L_Secure() const;
    void L_Secure(const bool in);
    bool NoExpire() const;
    void NoExpire(const bool in);
    bool Anarchy() const;
    void Anarchy(const bool in);
    bool L_Anarchy() const;
    void L_Anarchy(const bool in);
    bool KickOnBan() const;
    void KickOnBan(const bool in);
    bool L_KickOnBan() const;
    void L_KickOnBan(const bool in);
    bool Restricted() const;
    void Restricted(const bool in);
    bool L_Restricted() const;
    void L_Restricted(const bool in);
    bool Join() const;
    void Join(const bool in);
    bool L_Join() const;
    void L_Join(const bool in);
    mstring Revenge() const;
    bool Revenge(const mstring & in);
    bool L_Revenge() const;
    void L_Revenge(const bool in);

    bool Suspended() const;
    mstring Suspend_By() const;
    mDateTime Suspend_Time() const;
    bool Forbidden() const;

    enum commstat_t
    { C_None, C_IsIn, C_IsOn };

    // FIND: Looks for EXACT MATCH of passed entry.
//  bool Level_insert(const mstring& entry, const long value, const mstring& nick);
//  bool Level_erase();
    bool Level_change(const mstring & entry, const long value, const mstring & nick);
    set < entlist_val_t < long > >::iterator Level_begin()
    {
	return i_Level.begin();
    }
    set < entlist_val_t < long > >::iterator Level_end()
    {
	return i_Level.end();
    }
    size_t Level_size() const
    {
	return i_Level.size();
    }
    bool Level_find(const mstring & entry);
    long Level_value(const mstring & entry);
    set < entlist_val_t < long > >::iterator Level;

    // FIND: Looks for EXACT MATCH of passed entry, if !found,
    //       then if entry contains "@", REGEX match on entry,
    //       else REGEX match on mask of Nick_Live_t[entry].
    // INSERT: Adds if:
    //     1) Entry !has @ and is reg'd nick.
    //     2) Entry has (* or ?) and @.
    //     3) Entry has @ and no (* or ?).
    bool Access_insert(const mstring & entry, const long value, const mstring & nick, const mDateTime & modtime =
		       mDateTime::CurrentDateTime());
    bool Access_erase();
    set < entlist_val_t < long > >::iterator Access_begin()
    {
	return i_Access.begin();
    }
    set < entlist_val_t < long > >::iterator Access_end()
    {
	return i_Access.end();
    }
    size_t Access_size() const
    {
	return i_Access.size();
    }
    bool Access_find(const mstring & entry, const commstat_t commstat = C_None, const bool looklive = false);
    long Access_value(const mstring & entry, const commstat_t commstat = C_None, const bool looklive = false);
    set < entlist_val_t < long > >::iterator Access;
    long GetAccess(const mstring & entry);
    bool GetAccess(const mstring & entry, const mstring & type);

    // FIND: Looks for EXACT MATCH of passed entry, if !found,
    //       then if entry contains "@", REGEX match on entry,
    //       else REGEX match on mask of Nick_Live_t[entry].
    // INSERT: Adds if:
    //     1) Entry !has @ and is reg'd nick.
    //     2) Entry has (* or ?) and @.
    //     3) Entry has @ and no (* or ?).
    bool Akick_insert(const mstring & entry, const mstring & value, const mstring & nick, const mDateTime & modtime =
		      mDateTime::CurrentDateTime());
    bool Akick_insert(const mstring & entry, const mstring & nick, const mDateTime & modtime = mDateTime::CurrentDateTime());
    bool Akick_erase();

    set < entlist_val_t < mstring > >::iterator Akick_begin()
    {
	return i_Akick.begin();
    }
    set < entlist_val_t < mstring > >::iterator Akick_end()
    {
	return i_Akick.end();
    }
    size_t Akick_size() const
    {
	return i_Akick.size();
    }
    bool Akick_find(const mstring & entry, const commstat_t commstat = C_None, const bool looklive = false);
    mstring Akick_string(const mstring & entry, const commstat_t commstat = C_None, const bool looklive = false);

    set < entlist_val_t < mstring > >::iterator Akick;

    // FIND: Looks for EXACT MATCH of nick entry.
    // INSERT: Adds if not found.
    bool Greet_insert(const mstring & entry, const mstring & nick, const mDateTime & modtime = mDateTime::CurrentDateTime());
    bool Greet_erase();
    entlist_i Greet_begin()
    {
	return i_Greet.begin();
    }
    entlist_i Greet_end()
    {
	return i_Greet.end();
    }
    size_t Greet_size() const
    {
	return i_Greet.size();
    }
    bool Greet_find(const mstring & nick);
    entlist_i Greet;

    // FIND: Looks for NUMBER of entry.
    // INSERT: Adds if not found.
    bool Message_insert(const mstring & entry, const mstring & nick);
    bool Message_erase();
    entlist_i Message_begin()
    {
	return i_Message.begin();
    }
    entlist_i Message_end()
    {
	return i_Message.end();
    }
    size_t Message_size() const
    {
	return i_Message.size();
    }
    bool Message_find(const unsigned int num);
    entlist_i Message;

    SXP::Tag & GetClassTag() const
    {
	return tag_Chan_Stored_t;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

class ChanServ : public mBase, public SXP::IPersistObj
{
    friend class Magick;
    friend int EventTask::svc();
    friend int IrcSvcHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask);

private:

    set < mstring > Revenge_Levels;

    // Config Entries ...
    bool hide;			// Wether to set +s
    unsigned long expire;	// How long to keep channels
    unsigned long delay;	// How long between registrations
    unsigned int max_per_nick;	// Max channels per nickname
    mstring ovr_per_nick;	// Override committees for max_per_nick
    unsigned int max_messages;	// Max messages per channel
    mstring ovr_messages;	// Override committees for max_messages
    mstring def_akick_reason;	// Default AKICK reason
    unsigned int passfail;	// How many times they can fail ident
    unsigned long chankeep;	// Time to keep channel after AKICK
    long level_min;		// Minimum access level
    long level_max;		// Maximum access level
    map < mstring, long > lvl;

    class def_t
    {
	friend class ChanServ;
	friend class Magick;
	bool Keeptopic:1;
	bool Topiclock:1;
	bool Private:1;
	bool Secureops:1;
	bool Secure:1;
	bool NoExpire:1;
	bool Anarchy:1;
	bool KickOnBan:1;
	bool Restricted:1;
	bool Join:1;
	unsigned long Bantime;
	unsigned long Parttime;
	mstring Revenge;
	mstring Mlock;
    }
    def;

    class lock_t
    {
	friend class ChanServ;
	friend class Magick;
	bool Keeptopic:1;
	bool Topiclock:1;
	bool Private:1;
	bool Secureops:1;
	bool Secure:1;
	bool NoExpire:1;
	bool Anarchy:1;
	bool KickOnBan:1;
	bool Restricted:1;
	bool Join:1;
	bool Bantime:1;
	bool Parttime:1;
	bool Revenge:1;
	mstring Mlock;
    }
    lock;

    static SXP::Tag tag_ChanServ;

public:
#ifdef NO_HASH_MAP
    typedef map < mstring, Chan_Stored_t * > stored_t;
    typedef map < mstring, Chan_Live_t * > live_t;
#else
    typedef hash_map < mstring, Chan_Stored_t * > stored_t;
    typedef hash_map < mstring, Chan_Live_t * > live_t;
#endif

private:

    vector < Chan_Stored_t * > cs_array;
    stored_t stored;
    live_t live;

    void AddCommands();
    void RemCommands();

public:
    ChanServ();
    ~ChanServ()
    {
    }
    class stats_t
    {
	friend class ChanServ;

	mDateTime i_ClearTime;
	unsigned long i_Register;
	unsigned long i_Drop;
	unsigned long i_Identify;	//
	unsigned long i_Suspend;
	unsigned long i_Unsuspend;
	unsigned long i_Forbid;
	unsigned long i_Getpass;
	unsigned long i_Mode;
	unsigned long i_Topic;
	unsigned long i_Op;
	unsigned long i_Deop;
	unsigned long i_Halfop;
	unsigned long i_Dehalfop;
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
	stats_t()
	{
	    clear();
	}
	void clear()
	{
	    i_ClearTime = mDateTime::CurrentDateTime();
	    i_Register = i_Drop = i_Identify = i_Suspend = i_Unsuspend = i_Forbid = i_Getpass = i_Mode = i_Topic = i_Op =
		i_Deop = i_Halfop = i_Dehalfop = i_Voice = i_Devoice = i_Kick = i_Anonkick = i_Invite = i_Unban = i_Clear =
		i_Akick = i_Level = i_Access = i_Greet = i_Message = i_Set = i_NoExpire = i_Lock = i_Unlock = 0;
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
	unsigned long Identify() const
	{
	    return i_Identify;
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
	unsigned long Mode() const
	{
	    return i_Mode;
	}
	unsigned long Topic() const
	{
	    return i_Topic;
	}
	unsigned long Op() const
	{
	    return i_Op;
	}
	unsigned long Deop() const
	{
	    return i_Deop;
	}
	unsigned long Halfop() const
	{
	    return i_Halfop;
	}
	unsigned long Dehalfop() const
	{
	    return i_Dehalfop;
	}
	unsigned long Voice() const
	{
	    return i_Voice;
	}
	unsigned long Devoice() const
	{
	    return i_Devoice;
	}
	unsigned long Kick() const
	{
	    return i_Kick;
	}
	unsigned long Anonkick() const
	{
	    return i_Anonkick;
	}
	unsigned long Invite() const
	{
	    return i_Invite;
	}
	unsigned long Unban() const
	{
	    return i_Unban;
	}
	unsigned long Clear() const
	{
	    return i_Clear;
	}
	unsigned long Akick() const
	{
	    return i_Akick;
	}
	unsigned long Level() const
	{
	    return i_Level;
	}
	unsigned long Access() const
	{
	    return i_Access;
	}
	unsigned long Greet() const
	{
	    return i_Greet;
	}
	unsigned long Message() const
	{
	    return i_Message;
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
    }
    stats;

    bool IsRevengeLevel(const mstring & level)
    {
	return (Revenge_Levels.find(level.UpperCase()) != Revenge_Levels.end());
    }
    bool Hide() const
    {
	return hide;
    }
    unsigned long Expire() const
    {
	return expire;
    }
    unsigned long Delay() const
    {
	return delay;
    }
    unsigned int Max_Per_Nick() const
    {
	return max_per_nick;
    }
    mstring Ovr_Per_Nick() const
    {
	return max_per_nick;
    }
    unsigned int Max_Messages() const
    {
	return max_messages;
    }
    mstring Ovr_Messages() const
    {
	return max_messages;
    }
    mstring DEF_Akick_Reason() const
    {
	return def_akick_reason;
    }
    unsigned int Passfail() const
    {
	return passfail;
    }
    unsigned long ChanKeep() const
    {
	return chankeep;
    }
    long Level_Min() const
    {
	return level_min;
    }
    long Level_Max() const
    {
	return level_max;
    }
    mstring DEF_MLock() const
    {
	return def.Mlock;
    }
    mstring LCK_MLock() const
    {
	return lock.Mlock;
    }
    unsigned long DEF_Bantime() const
    {
	return def.Bantime;
    }
    bool LCK_Bantime() const
    {
	return lock.Bantime;
    }
    unsigned long DEF_Parttime() const
    {
	return def.Parttime;
    }
    bool LCK_Parttime() const
    {
	return lock.Parttime;
    }
    bool DEF_Keeptopic() const
    {
	return def.Keeptopic;
    }
    bool LCK_Keeptopic() const
    {
	return lock.Keeptopic;
    }
    bool DEF_Topiclock() const
    {
	return def.Topiclock;
    }
    bool LCK_Topiclock() const
    {
	return lock.Topiclock;
    }
    bool DEF_Private() const
    {
	return def.Private;
    }
    bool LCK_Private() const
    {
	return lock.Private;
    }
    bool DEF_Secureops() const
    {
	return def.Secureops;
    }
    bool LCK_Secureops() const
    {
	return lock.Secureops;
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
    bool DEF_Anarchy() const
    {
	return def.Anarchy;
    }
    bool LCK_Anarchy() const
    {
	return lock.Anarchy;
    }
    bool DEF_KickOnBan() const
    {
	return def.KickOnBan;
    }
    bool LCK_KickOnBan() const
    {
	return lock.KickOnBan;
    }
    bool DEF_Restricted() const
    {
	return def.Restricted;
    }
    bool LCK_Restricted() const
    {
	return lock.Restricted;
    }
    bool DEF_Join() const
    {
	return def.Join;
    }
    bool LCK_Join() const
    {
	return lock.Join;
    }
    mstring DEF_Revenge() const
    {
	return def.Revenge;
    }
    bool LCK_Revenge() const
    {
	return lock.Revenge;
    }
    long LVL(const mstring & level) const;
    bool IsLVL(const mstring & level) const;
    vector < mstring > LVL() const;

    Part_Handler ph;

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddStored(Chan_Stored_t * in) throw(E_ChanServ_Stored);
    void AddStored(const Chan_Stored_t & in) throw(E_ChanServ_Stored)
    {
	AddStored(new Chan_Stored_t(in));
    }
    map_entry < Chan_Stored_t > GetStored(const mstring & in) const throw(E_ChanServ_Stored);
    void RemStored(const mstring & in) throw(E_ChanServ_Stored);
#else
    void AddStored(Chan_Stored_t * in);
    void AddStored(const Chan_Stored_t & in)
    {
	AddStored(new Chan_Stored_t(in));
    }
    map_entry < Chan_Stored_t > GetStored(const mstring & in);
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
    void AddLive(Chan_Live_t * in) throw(E_ChanServ_Live);
    void AddLive(const Chan_Live_t & in) throw(E_ChanServ_Live)
    {
	AddLive(new Chan_Live_t(in));
    }
    map_entry < Chan_Live_t > GetLive(const mstring & in) const throw(E_ChanServ_Live);
    void RemLive(const mstring & in) throw(E_ChanServ_Live);
#else
    void AddLive(Chan_Live_t * in);
    void AddLive(const Chan_Live_t & in)
    {
	AddLive(new Chan_Live_t(in));
    }
    map_entry < Chan_Live_t > GetLive(const mstring & in) const;
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

    threadtype_enum Get_TType() const
    {
	return tt_ChanServ;
    }
    mstring GetInternalName() const
    {
	return "ChanServ";
    }
    void execute(mstring & source, const mstring & msgtype, const mstring & params);

    static void do_Help(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Register(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Drop(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Identify(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Info(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Suspend(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_UnSuspend(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Forbid(const mstring & mynick, const mstring & source, const mstring & params);

#ifdef GETPASS
    static void do_Getpass(const mstring & mynick, const mstring & source, const mstring & params);
#endif
    static void do_Setpass(const mstring & mynick, const mstring & source, const mstring & params);

    static void do_Mode(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Op(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_DeOp(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_HalfOp(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_DeHalfOp(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Voice(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_DeVoice(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Topic(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Kick(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_AnonKick(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Users(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Invite(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Unban(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Live(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Detail(const mstring & mynick, const mstring & source, const mstring & params);

    static void do_clear_Users(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_clear_Modes(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_clear_Ops(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_clear_HalfOps(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_clear_Voices(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_clear_Bans(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_clear_All(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_level_Set(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_level_Reset(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_level_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_access_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_access_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_access_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_akick_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_akick_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_akick_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_greet_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_greet_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_greet_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_message_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_message_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_message_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Founder(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_CoFounder(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Description(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Password(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Email(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_URL(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Comment(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Mlock(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_BanTime(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_PartTime(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_KeepTopic(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_TopicLock(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Private(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_SecureOps(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Secure(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_NoExpire(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Anarchy(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_KickOnBan(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Restricted(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Join(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Revenge(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Mlock(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_BanTime(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_PartTime(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_KeepTopic(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_TopicLock(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Private(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_SecureOps(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Secure(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Anarchy(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_KickOnBan(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Restricted(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Join(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Revenge(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Mlock(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_BanTime(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_PartTime(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_KeepTopic(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_TopicLock(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Private(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_SecureOps(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Secure(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Anarchy(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_KickOnBan(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Restricted(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Join(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Revenge(const mstring & mynick, const mstring & source, const mstring & params);

    SXP::Tag & GetClassTag() const
    {
	return tag_ChanServ;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);
    void PostLoad();
};

#endif
