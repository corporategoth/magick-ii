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
#ifndef _OPERSERV_H
#define _OPERSERV_H
#include "pch.h"
RCSID(operserv_h, "@(#) $Id$");

/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.63  2002/01/12 14:42:08  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.62  2001/12/25 08:43:12  prez
** Fixed XML support properly ... it now works again with new version of
** expat (1.95.2) and sxp (1.1).  Also removed some of my const hacks.
**
** Revision 1.61  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.60  2001/11/03 21:02:51  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.59  2001/07/29 21:22:27  prez
** Delayed clone akills on sync until AFTER we're synced
**
** Revision 1.58  2001/06/15 07:20:39  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.57  2001/05/17 19:18:53  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.56  2001/05/01 14:00:22  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.55  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.54  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.53  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.52  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.51  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.49  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.46  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.45  2000/12/22 19:50:19  prez
** Made all config options const.  Beginnings of securing all non-modifying
** commands to const.  also added serviceschk.
**
** Revision 1.44  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.43  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.42  2000/09/02 07:20:44  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.41  2000/08/06 05:27:46  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.40  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.39  2000/06/26 11:23:17  prez
** Added auto-akill on clone triggers
**
** Revision 1.38  2000/06/12 06:07:49  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.37  2000/06/08 13:07:33  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.36  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.35  2000/05/26 11:21:28  prez
** Implemented HTM (High Traffic Mode) -- Can be used at a later date.
**
** Revision 1.34  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.33  2000/05/14 04:02:52  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.32  2000/04/04 03:21:34  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.31  2000/04/03 09:45:21  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.30  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.29  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "nickserv.h"

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class OperServ:public mBase, public SXP::IPersistObj
{
    friend class Magick;
    friend class Nick_Live_t;
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
    typedef entlist_val_t < bool > Ignore_Type;

  private:

    // Mask (H), Limit (int), Reason (mstring)
    set < Clone_Type > i_Clone;
    map < mstring, pair < unsigned int, list < mDateTime > > >CloneList;

    // Mask (U_H), Expire (long), Reason (mstring)
    set < Akill_Type > i_Akill;

    // Mask (N_U_H), Reason (mstring)
    set < OperDeny_Type > i_OperDeny;

    // Mask (N_U_H), Permanent (bool)
    set < Ignore_Type > i_Ignore;

    vector < Clone_Type * >c_array;
    vector < Akill_Type * >a_array;
    vector < OperDeny_Type * >o_array;
    vector < Ignore_Type * >i_array;

    void AddCommands();
    void RemCommands();
    // Returns TRUE if KILL
    bool AddHost(const mstring & host);
    void RemHost(const mstring & host);

    static SXP::Tag tag_OperServ, tag_Clone, tag_Akill, tag_OperDeny, tag_Ignore;
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
	unsigned long i_Ignore;
      public:
	stats_t()
	{
	    clear();
	}
	void clear()
	{
	    i_ClearTime = mDateTime::CurrentDateTime();
	    i_Trace = i_Mode = i_Qline = i_Unqline = i_Noop = i_Kill = i_Hide = i_Ping = i_Update = i_Reload = i_Unload =
		i_Jupe = i_OnOff = i_Clone = i_Akill = i_OperDeny = i_Ignore = 0;
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
	unsigned long Ignore() const
	{
	    return i_Ignore;
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

    bool Ignore_insert(const mstring & entry, const bool perm, const mstring & nick);
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
    static void do_ignore_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_ignore_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_ignore_List(const mstring & mynick, const mstring & source, const mstring & params);

    SXP::Tag & GetClassTag()const
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
