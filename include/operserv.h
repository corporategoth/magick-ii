#ifndef WIN32
#pragma interface
#endif

/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
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
#ifndef _OPERSERV_H
#define _OPERSERV_H
#include "pch.h"
RCSID(operserv_h, "@(#) $Id$");

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

#include "nickserv.h"

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class OperServ : public mBase, public SXP::IPersistObj
{
    friend class Magick;
    friend class Nick_Live_t;
    friend class Convert;

private:
    mstring services_admin;
    bool secure;
    bool secureoper;
    unsigned long def_expire;
    unsigned long expire_oper;
    unsigned long expire_admin;
    unsigned long expire_sop;
    unsigned long expire_sadmin;
    float akill_reject;
    unsigned int max_clone;
    unsigned int clone_limit;
    mstring def_clone;
    unsigned int clone_trigger;
    unsigned long clone_time;
    mstring clone_akill;
    unsigned long clone_akilltime;
    mstring def_bot;
    unsigned int bot_trigger;
    unsigned long bot_time;
    mstring bot_akill;
    unsigned long bot_akilltime;
    unsigned long flood_time;
    unsigned int flood_msgs;
    unsigned long ignore_time;
    unsigned int ignore_limit;
    unsigned long ignore_remove;
    unsigned int ignore_method;
    bool log_ignore;
    unsigned long init_htm_gap;
    unsigned long init_htm_thresh;
    unsigned long max_htm_gap;
    unsigned long htm_on_gap;

public:
    typedef entlist_val_pair_t < unsigned int, mstring > Clone_Type;
    typedef entlist_val_pair_t < unsigned long, mstring > Akill_Type;
    typedef entlist_val_t < mstring > OperDeny_Type;
    typedef entlist_val_t < unsigned long > KillPhrase_Type;
    typedef entlist_val_t < bool > Ignore_Type;
    typedef entlist_val_pair_t < unsigned long, mstring > KillChan_Type;

private:

    // Mask (H), Limit (int), Reason (mstring)
    set < Clone_Type > i_Clone;
    map < mstring, pair < unsigned int, list < mDateTime > > > CloneList;

    // Record of bots killed.
    map < mstring, list < mDateTime > > BotList;

    // Mask (U_H), Expire (long), Reason (mstring)
    set < Akill_Type > i_Akill;

    // Mask (N_U_H), Reason (mstring)
    set < OperDeny_Type > i_OperDeny;

    // String, Reason (mstring)
    set < KillPhrase_Type > i_KillPhrase;

    // Mask (N_U_H), Permanent (bool)
    set < Ignore_Type > i_Ignore;

    // Channel, Expire (long), Reason (mstring)
    set < KillChan_Type > i_KillChan;

    vector < Clone_Type * > c_array;
    vector < Akill_Type * > a_array;
    vector < KillChan_Type * > k_array;
    vector < OperDeny_Type * > o_array;
    vector < KillPhrase_Type * > kp_array;
    vector < Ignore_Type * > i_array;

    void AddCommands();
    void RemCommands();

    // Returns TRUE if KILL
    bool AddHost(const mstring & host);
    void RemHost(const mstring & host);

    static SXP::Tag tag_OperServ, tag_Clone, tag_Akill, tag_OperDeny, tag_KillPhrase, tag_Ignore, tag_KillChan;

public:
    OperServ();
    ~OperServ()
    {
    }

    class stats_t
    {
	friend class OperServ;

	mDateTime i_ClearTime;
	unsigned long i_Trace;
	unsigned long i_Mode;
	unsigned long i_Qline;
	unsigned long i_Unqline;
	unsigned long i_Noop;
	unsigned long i_Kill;
	unsigned long i_Hide;
	unsigned long i_Ping;
	unsigned long i_Update;
	unsigned long i_Reload;
	unsigned long i_Unload;
	unsigned long i_Jupe;
	unsigned long i_OnOff;
	unsigned long i_Clone;
	unsigned long i_Akill;
	unsigned long i_OperDeny;
	unsigned long i_KillPhrase;
	unsigned long i_Ignore;
	unsigned long i_KillChan;

    public:
	stats_t()
	{
	    clear();
	}
	void clear()
	{
	    i_ClearTime = mDateTime::CurrentDateTime();
	    i_Trace = i_Mode = i_Qline = i_Unqline = i_Noop = i_Kill = i_Hide = i_Ping = i_Update = i_Reload = i_Unload =
		i_Jupe = i_OnOff = i_Clone = i_Akill = i_OperDeny = i_KillPhrase = i_Ignore = i_KillChan = 0;
	}
	mDateTime ClearTime() const
	{
	    return i_ClearTime;
	}
	unsigned long Trace() const
	{
	    return i_Trace;
	}
	unsigned long Mode() const
	{
	    return i_Mode;
	}
	unsigned long Qline() const
	{
	    return i_Qline;
	}
	unsigned long Unqline() const
	{
	    return i_Unqline;
	}
	unsigned long Noop() const
	{
	    return i_Noop;
	}
	unsigned long Kill() const
	{
	    return i_Kill;
	}
	unsigned long Hide() const
	{
	    return i_Hide;
	}
	unsigned long Ping() const
	{
	    return i_Ping;
	}
	unsigned long Update() const
	{
	    return i_Update;
	}
	unsigned long Reload() const
	{
	    return i_Reload;
	}
	unsigned long Unload() const
	{
	    return i_Unload;
	}
	unsigned long Jupe() const
	{
	    return i_Jupe;
	}
	unsigned long OnOff() const
	{
	    return i_OnOff;
	}
	unsigned long Clone() const
	{
	    return i_Clone;
	}
	unsigned long Akill() const
	{
	    return i_Akill;
	}
	unsigned long OperDeny() const
	{
	    return i_OperDeny;
	}
	unsigned long KillPhrase() const
	{
	    return i_KillPhrase;
	}
	unsigned long Ignore() const
	{
	    return i_Ignore;
	}
	unsigned long KillChan() const
	{
	    return i_KillChan;
	}
    }
    stats;

    mstring Services_Admin() const
    {
	return services_admin;
    }
    bool Secure() const
    {
	return secure;
    }
    bool SecureOper() const
    {
	return secureoper;
    }
    unsigned long Def_Expire() const
    {
	return def_expire;
    }
    unsigned long Expire_Oper() const
    {
	return expire_oper;
    }
    unsigned long Expire_Admin() const
    {
	return expire_admin;
    }
    unsigned long Expire_Sop() const
    {
	return expire_sop;
    }
    unsigned long Expire_SAdmin() const
    {
	return expire_sadmin;
    }
    float Akill_Reject() const
    {
	return akill_reject;
    }
    unsigned int Max_Clone() const
    {
	return max_clone;
    }
    unsigned int Clone_Limit() const
    {
	return clone_limit;
    }
    mstring Def_Clone() const
    {
	return def_clone;
    }
    unsigned int Clone_Trigger() const
    {
	return clone_trigger;
    }
    unsigned long Clone_Time() const
    {
	return clone_time;
    }
    mstring Clone_Akill() const
    {
	return clone_akill;
    }
    unsigned long Clone_AkillTime() const
    {
	return clone_akilltime;
    }
    mstring Def_Bot() const
    {
	return def_bot;
    }
    unsigned int Bot_Trigger() const
    {
	return bot_trigger;
    }
    unsigned long Bot_Time() const
    {
	return bot_time;
    }
    mstring Bot_Akill() const
    {
	return bot_akill;
    }
    unsigned long Bot_AkillTime() const
    {
	return bot_akilltime;
    }
    unsigned long Flood_Time() const
    {
	return flood_time;
    }
    unsigned int Flood_Msgs() const
    {
	return flood_msgs;
    }
    unsigned long Ignore_Time() const
    {
	return ignore_time;
    }
    unsigned int Ignore_Limit() const
    {
	return ignore_limit;
    }
    unsigned long Ignore_Remove() const
    {
	return ignore_remove;
    }
    Nick_Live_t::styles Ignore_Method() const
    {
	return static_cast < Nick_Live_t::styles > (ignore_method);
    }
    bool Log_Ignore() const
    {
	return log_ignore;
    }
    unsigned long Init_HTM_Gap() const
    {
	return init_htm_gap;
    }
    unsigned long Init_HTM_Thresh() const
    {
	return init_htm_thresh;
    }
    unsigned long Max_HTM_Gap() const
    {
	return max_htm_gap;
    }
    unsigned long HTM_On_Gap() const
    {
	return htm_on_gap;
    }

    size_t CloneList_size() const
    {
	return CloneList.size();
    }
    size_t CloneList_sum() const;
    size_t CloneList_size(const unsigned int amt) const;
    size_t CloneList_Usage() const;
    void CloneList_check();

    bool Clone_insert(const mstring & entry, const unsigned int value, const mstring & reason, const mstring & nick,
		      const mDateTime & added = mDateTime::CurrentDateTime());
    bool Clone_erase();

    set < Clone_Type >::iterator Clone_begin()
    {
	return i_Clone.begin();
    }
    set < Clone_Type >::iterator Clone_end()
    {
	return i_Clone.end();
    }
    size_t Clone_size() const
    {
	return i_Clone.size();
    }
    size_t Clone_Usage() const;
    bool Clone_find(const mstring & entry);
    pair < unsigned int, mstring > Clone_value(const mstring & entry);

    set < Clone_Type >::iterator Clone;

    void KillBot(const mstring & name);

    bool Akill_insert(const mstring & entry, const unsigned long value, const mstring & reason, const mstring & nick,
		      const mDateTime & added = mDateTime::CurrentDateTime());
    bool Akill_erase();

    set < Akill_Type >::iterator Akill_begin()
    {
	return i_Akill.begin();
    }
    set < Akill_Type >::iterator Akill_end()
    {
	return i_Akill.end();
    }
    size_t Akill_size() const
    {
	return i_Akill.size();
    }
    size_t Akill_Usage() const;
    bool Akill_find(const mstring & entry);
    pair < unsigned long, mstring > Akill_value(const mstring & entry);

    set < Akill_Type >::iterator Akill;

    bool OperDeny_insert(const mstring & entry, const mstring & value, const mstring & nick);
    bool OperDeny_erase();

    set < OperDeny_Type >::iterator OperDeny_begin()
    {
	return i_OperDeny.begin();
    }
    set < OperDeny_Type >::iterator OperDeny_end()
    {
	return i_OperDeny.end();
    }
    size_t OperDeny_size() const
    {
	return i_OperDeny.size();
    }
    size_t OperDeny_Usage() const;
    bool OperDeny_find(const mstring & entry);
    mstring OperDeny_value(const mstring & entry);

    set < OperDeny_Type >::iterator OperDeny;

    bool KillPhrase_insert(const mstring & entry, unsigned long expiry, const mstring & nick);
    bool KillPhrase_erase();

    set < KillPhrase_Type >::iterator KillPhrase_begin()
    {
	return i_KillPhrase.begin();
    }
    set < KillPhrase_Type >::iterator KillPhrase_end()
    {
	return i_KillPhrase.end();
    }
    size_t KillPhrase_size() const
    {
	return i_KillPhrase.size();
    }
    size_t KillPhrase_Usage() const;
    bool KillPhrase_find(const mstring & entry);
    unsigned long KillPhrase_value(const mstring & entry);

    set < KillPhrase_Type >::iterator KillPhrase;

    bool Ignore_insert(const mstring & entry, const bool perm, const mstring & nick,
		       const mDateTime & added = mDateTime::CurrentDateTime());
    bool Ignore_erase();

    set < Ignore_Type >::iterator Ignore_begin()
    {
	return i_Ignore.begin();
    }
    set < Ignore_Type >::iterator Ignore_end()
    {
	return i_Ignore.end();
    }
    size_t Ignore_size() const
    {
	return i_Ignore.size();
    }
    size_t Ignore_Usage() const;
    bool Ignore_find(const mstring & entry);
    bool Ignore_value(const mstring & entry);

    set < Ignore_Type >::iterator Ignore;

    bool KillChan_insert(const mstring & entry, const unsigned long value, const mstring & reason, const mstring & nick,
		      const mDateTime & added = mDateTime::CurrentDateTime());
    bool KillChan_erase();

    set < KillChan_Type >::iterator KillChan_begin()
    {
	return i_KillChan.begin();
    }
    set < KillChan_Type >::iterator KillChan_end()
    {
	return i_KillChan.end();
    }
    size_t KillChan_size() const
    {
	return i_KillChan.size();
    }
    size_t KillChan_Usage() const;
    bool KillChan_find(const mstring & entry);
    pair < unsigned long, mstring > KillChan_value(const mstring & entry);

    set < KillChan_Type >::iterator KillChan;

    threadtype_enum Get_TType() const
    {
	return tt_OperServ;
    }
    mstring GetInternalName() const
    {
	return "OperServ";
    }
    void execute(mstring & source, const mstring & msgtype, const mstring & params);

    static void do_Help(const mstring & mynick, const mstring & source, const mstring & params);

#ifdef MAGICK_TRACE_WORKS
    static void do_Trace(const mstring & mynick, const mstring & source, const mstring & params);
#endif
    static void do_Mode(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Qline(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_UnQline(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_NOOP(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Kill(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Hide(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Ping(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Update(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Shutdown(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Restart(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Reload(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Signon(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Unload(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Jupe(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_On(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Off(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_HTM(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_settings_Config(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_settings_Nick(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_settings_Channel(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_settings_Other(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_settings_All(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_clone_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_clone_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_clone_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_akill_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_akill_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_akill_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_operdeny_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_operdeny_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_operdeny_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_killphrase_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_killphrase_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_killphrase_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_ignore_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_ignore_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_ignore_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_killchan_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_killchan_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_killchan_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_kill_nochan(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_akill_nochan(const mstring & mynick, const mstring & source, const mstring & params);

    SXP::Tag & GetClassTag() const
    {
	return tag_OperServ;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);
    void PostLoad();
    void DumpB() const;
    void DumpE() const;
};

#endif
