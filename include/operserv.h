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
#ifndef _OPERSERV_H
#define _OPERSERV_H
#include "pch.h"
static const char *ident_operserv_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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


#include "base.h"
#include "nickserv.h"

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class OperServ : public mBase, public SXP::IPersistObj
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
    unsigned int max_clone;
    unsigned int clone_limit;
    mstring def_clone;
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

    // Mask (H), Limit (int), Reason (mstring)
    set<entlist_val_t<pair<unsigned int, mstring> > > i_Clone;
    map<mstring, unsigned int> CloneList;

    // Mask (U_H), Expire (long), Reason (mstring)
    set<entlist_val_t<pair<unsigned long, mstring> > > i_Akill;

    // Mask (N_U_H), Reason (mstring)
    set<entlist_val_t<mstring> > i_OperDeny;

    // Mask (N_U_H), Permanent (bool)
    set<entlist_val_t<bool> > i_Ignore;

    void AddCommands();
    void RemCommands();
    // Returns TRUE if KILL
    bool AddHost(mstring host);
    void RemHost(mstring host);

    static SXP::Tag tag_OperServ, tag_Clone, tag_Akill, tag_OperDeny,
	tag_Ignore;
public:
    class stats_t
    {
	friend class OperServ;

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
	stats_t() {
	    i_Trace = i_Mode = i_Qline = i_Unqline = i_Noop =
		i_Kill = i_Hide = i_Ping = i_Update = i_Reload =
		i_Unload = i_Jupe = i_OnOff = i_Clone =
		i_Akill = i_OperDeny = i_Ignore = 0; }
	unsigned long Trace()	    { return i_Trace; }
	unsigned long Mode()	    { return i_Mode; }
	unsigned long Qline()	    { return i_Qline; }
	unsigned long Unqline()	    { return i_Unqline; }
	unsigned long Noop()	    { return i_Noop; }
	unsigned long Kill()	    { return i_Kill; }
	unsigned long Hide()	    { return i_Hide; }
	unsigned long Ping()	    { return i_Ping; }
	unsigned long Update()	    { return i_Update; }
	unsigned long Reload()	    { return i_Reload; }
	unsigned long Unload()	    { return i_Unload; }
	unsigned long Jupe()	    { return i_Jupe; }
	unsigned long OnOff()	    { return i_OnOff; }
	unsigned long Clone()	    { return i_Clone; }
	unsigned long Akill()	    { return i_Akill; }
	unsigned long OperDeny()    { return i_OperDeny; }
	unsigned long Ignore()	    { return i_Ignore; }
    } stats;

    mstring Services_Admin()	    { return services_admin; }
    bool Secure()		    { return secure; }
    bool SecureOper()		    { return secureoper; }
    unsigned long Def_Expire()	    { return def_expire; }
    unsigned long Expire_Oper()	    { return expire_oper; }
    unsigned long Expire_Admin()	    { return expire_admin; }
    unsigned long Expire_Sop()	    { return expire_sop; }
    unsigned long Expire_SAdmin()	    { return expire_sadmin; }
    unsigned int Max_Clone()	    { return max_clone; }
    unsigned int Clone_Limit()	    { return clone_limit; }
    mstring Def_Clone()		    { return def_clone; }
    unsigned long Flood_Time()	    { return flood_time; }
    unsigned int Flood_Msgs()	    { return flood_msgs; }
    unsigned long Ignore_Time()	    { return ignore_time; }
    unsigned int Ignore_Limit()	    { return ignore_limit; }
    unsigned long Ignore_Remove()   { return ignore_remove; }
    Nick_Live_t::styles Ignore_Method()
	{ return (Nick_Live_t::styles) ignore_method; }
    bool Log_Ignore()		    { return log_ignore; }
    unsigned long Init_HTM_Gap()    { return init_htm_gap; }
    unsigned long Init_HTM_Thresh() { return init_htm_thresh; }
    unsigned long Max_HTM_Gap()     { return max_htm_gap; }
    unsigned long HTM_On_Gap()      { return htm_on_gap; }

    size_t CloneList_size() { return CloneList.size(); }
    size_t CloneList_sum();
    size_t CloneList_size(unsigned int amt);

    bool Clone_insert(mstring entry, unsigned int value, mstring reason, mstring nick, mDateTime added = Now());
    bool Clone_erase();
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator Clone_begin()
	{ return i_Clone.begin(); }
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator Clone_end()
	{ return i_Clone.end(); }
    size_t Clone_size()				{ return i_Clone.size(); }
    bool Clone_find(mstring entry);
    pair<unsigned int, mstring> Clone_value(mstring entry);
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator Clone;

    bool Akill_insert(mstring entry, unsigned long value, mstring reason, mstring nick, mDateTime added = Now());
    bool Akill_erase();
    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator Akill_begin()
	{ return i_Akill.begin(); }
    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator Akill_end()
	{ return i_Akill.end(); }
    size_t Akill_size()				{ return i_Akill.size(); }
    bool Akill_find(mstring entry);
    pair<unsigned long, mstring> Akill_value(mstring entry);
    set<entlist_val_t<pair<unsigned long, mstring> > >::iterator Akill;

    bool OperDeny_insert(mstring entry, mstring value, mstring nick);
    bool OperDeny_erase();
    set<entlist_val_t<mstring> >::iterator OperDeny_begin()
	{ return i_OperDeny.begin(); }
    set<entlist_val_t<mstring> >::iterator OperDeny_end()
	{ return i_OperDeny.end(); }
    size_t OperDeny_size()				{ return i_OperDeny.size(); }
    bool OperDeny_find(mstring entry);
    mstring OperDeny_value(mstring entry);
    set<entlist_val_t<mstring > >::iterator OperDeny;

    bool Ignore_insert(mstring entry, bool perm, mstring nick);
    bool Ignore_erase();
    set<entlist_val_t<bool> >::iterator Ignore_begin()
	{ return i_Ignore.begin(); }
    set<entlist_val_t<bool> >::iterator Ignore_end()
	{ return i_Ignore.end(); }
    size_t Ignore_size()				{ return i_Ignore.size(); }
    bool Ignore_find(mstring entry);
    bool Ignore_value(mstring entry);
    set<entlist_val_t<bool> >::iterator Ignore;


    OperServ();
    virtual threadtype_enum Get_TType() const { return tt_OperServ; }
    virtual mstring GetInternalName() const { return "OperServ"; }
    virtual void execute(const mstring & message);

    static void do_Help(mstring mynick, mstring source, mstring params);
    static void do_Trace(mstring mynick, mstring source, mstring params);
    static void do_Mode(mstring mynick, mstring source, mstring params);
    static void do_Qline(mstring mynick, mstring source, mstring params);
    static void do_UnQline(mstring mynick, mstring source, mstring params);
    static void do_NOOP(mstring mynick, mstring source, mstring params);
    static void do_Kill(mstring mynick, mstring source, mstring params);
    static void do_Hide(mstring mynick, mstring source, mstring params);
    static void do_Ping(mstring mynick, mstring source, mstring params);
    static void do_Update(mstring mynick, mstring source, mstring params);
    static void do_Shutdown(mstring mynick, mstring source, mstring params);
    static void do_Reload(mstring mynick, mstring source, mstring params);
    static void do_Unload(mstring mynick, mstring source, mstring params);
    static void do_Jupe(mstring mynick, mstring source, mstring params);
    static void do_On(mstring mynick, mstring source, mstring params);
    static void do_Off(mstring mynick, mstring source, mstring params);
    static void do_HTM(mstring mynick, mstring source, mstring params);
    static void do_settings_Config(mstring mynick, mstring source, mstring params);
    static void do_settings_Nick(mstring mynick, mstring source, mstring params);
    static void do_settings_Channel(mstring mynick, mstring source, mstring params);
    static void do_settings_Other(mstring mynick, mstring source, mstring params);
    static void do_settings_All(mstring mynick, mstring source, mstring params);
    static void do_clone_Add(mstring mynick, mstring source, mstring params);
    static void do_clone_Del(mstring mynick, mstring source, mstring params);
    static void do_clone_List(mstring mynick, mstring source, mstring params);
    static void do_akill_Add(mstring mynick, mstring source, mstring params);
    static void do_akill_Del(mstring mynick, mstring source, mstring params);
    static void do_akill_List(mstring mynick, mstring source, mstring params);
    static void do_operdeny_Add(mstring mynick, mstring source, mstring params);
    static void do_operdeny_Del(mstring mynick, mstring source, mstring params);
    static void do_operdeny_List(mstring mynick, mstring source, mstring params);
    static void do_ignore_Add(mstring mynick, mstring source, mstring params);
    static void do_ignore_Del(mstring mynick, mstring source, mstring params);
    static void do_ignore_List(mstring mynick, mstring source, mstring params);

    SXP::Tag& GetClassTag() const { return tag_OperServ; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement) { };
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);
    void PostLoad();
};

#endif
