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
#ifndef _operserv_h
#define _operserv_h

#include "mstring.h"
#include "variant.h"
#include "base.h"
#include "nickserv.h"

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class OperServ : public mBase
{
    friend class Magick;
    friend class Nick_Live_t;
private:
    mstring services_admin;
    bool secure;
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

    bool flood;
    bool akill;
    bool operdeny;

    // Mask (H), Limit (int), Reason (mstring)
    set<entlist_val_t<pair<unsigned int, mstring> > > i_Clone;
    map<mstring, int> CloneList;

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
public:
    mstring Services_Admin()	    { return services_admin; }
    bool Secure()		    { return secure; }
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

    bool Flood()		{ return flood; }
    bool oAkill()		{ return akill; }
    bool oOperDeny()		{ return operdeny; }

    bool Clone_insert(mstring entry, unsigned int value, mstring reason, mstring nick);
    bool Clone_erase();
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator Clone_begin()
	{ return i_Clone.begin(); }
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator Clone_end()
	{ return i_Clone.end(); }
    size_t Clone_size()				{ return i_Clone.size(); }
    bool Clone_find(mstring entry);
    pair<unsigned int, mstring> Clone_value(mstring entry);
    set<entlist_val_t<pair<unsigned int, mstring> > >::iterator Clone;

    bool Akill_insert(mstring entry, unsigned long value, mstring reason, mstring nick);
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


    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);
    OperServ();
    virtual threadtype_enum Get_TType() const { return tt_OperServ; }
    virtual mstring GetInternalName() const { return "OperServ"; }
    virtual void execute(const mstring & message);

    static void do_Help(mstring mynick, mstring source, mstring params);
    static void do_Trace(mstring mynick, mstring source, mstring params);
    static void do_Mode(mstring mynick, mstring source, mstring params);
    static void do_Kick(mstring mynick, mstring source, mstring params);
    static void do_Qline(mstring mynick, mstring source, mstring params);
    static void do_UnQline(mstring mynick, mstring source, mstring params);
    static void do_NOOP(mstring mynick, mstring source, mstring params);
    static void do_Kill(mstring mynick, mstring source, mstring params);
    static void do_Ping(mstring mynick, mstring source, mstring params);
    static void do_Update(mstring mynick, mstring source, mstring params);
    static void do_Shutdown(mstring mynick, mstring source, mstring params);
    static void do_Reload(mstring mynick, mstring source, mstring params);
    static void do_Jupe(mstring mynick, mstring source, mstring params);
    static void do_On(mstring mynick, mstring source, mstring params);
    static void do_Off(mstring mynick, mstring source, mstring params);
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
};

#endif
