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

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class OperServ : public mBase
{
    friend class Magick;
    friend class Nick_Live_t;
private:
    mstring services_admin;
    mstring expire_oper;
    mstring expire_admin;
    mstring expire_sop;
    mstring expire_sadmin;
    int max_clone;
    int clone_limit;
    mstring def_clone;
    int flood_time;
    int flood_msgs;
    int ignore_time;
    int ignore_limit;
    int ignore_remove;
    int ignore_method;

    bool flood;
    bool akill;
    bool operdeny;

    // Mask (H), Limit (int), Reason (mstring)
    set<entlist_val_t<pair<int, mstring> > > i_Clone;
    map<mstring, int> CloneList;

    // Mask (U_H), Expire (long), Reason (mstring)
    set<entlist_val_t<pair<long, mstring> > > i_Akill;

    // Mask (N_U_H), Reason (mstring)
    set<entlist_val_t<mstring> > i_OperDeny;

    // Mask (N_U_H), AddTime (mDateTime), Permanent (bool)
    set<entlist_val_t<pair<mDateTime, bool> > > i_Ignore;

    // Returns TRUE if KILL
    bool AddHost(mstring host);
    void RemHost(mstring host);
    void DoBreakdown(mstring mynick, mstring source, mstring previndent, mstring server);
public:
    mstring Services_Admin()	{ return services_admin; }
    mstring Expire_Oper()	{ return expire_oper; }
    mstring Expire_Admin()	{ return expire_admin; }
    mstring Expire_Sop()	{ return expire_sop; }
    mstring Expire_SAdmin()	{ return expire_sadmin; }
    int Max_Clone()		{ return max_clone; }
    int Clone_Limit()		{ return clone_limit; }
    mstring Def_Clone()		{ return def_clone; }
    int Flood_Time()		{ return flood_time; }
    int Flood_Msgs()		{ return flood_msgs; }
    int Ignore_Time()		{ return ignore_time; }
    int Ignore_Limit()		{ return ignore_limit; }
    int Ignore_Remove()		{ return ignore_remove; }
    int Ignore_Method()		{ return ignore_method; }

    bool Flood()		{ return flood; }
    bool Akill()		{ return akill; }
    bool OperDeny()		{ return operdeny; }

    bool Clone_insert(mstring entry, int value, mstring reason, mstring nick);
    bool Clone_erase();
//  entlist_val_ui<int, mstring> Clone_begin()	{ return i_Clone.begin(); }
    set<entlist_val_t<pair<int, mstring> > >::iterator Clone_begin()
	{ return i_Clone.begin(); }
//  entlist_val_ui<int, mstring> Clone_end()	{ return i_Clone.end(); }
    set<entlist_val_t<pair<int, mstring> > >::iterator Clone_end()
	{ return i_Clone.end(); }
    size_t Clone_size()				{ return i_Clone.size(); }
    bool Clone_find(mstring entry);
    pair<int,mstring> Clone_value(mstring entry);
//  entlist_val_ui<int> Clone;
    set<entlist_val_t<pair<int, mstring> > >::iterator Clone;


    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);
    OperServ();
    virtual threadtype_enum Get_TType() const { return tt_OperServ; }
    virtual mstring GetInternalName() const { return "OperServ"; }
    virtual void execute(const mstring & message);
};

#endif
