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

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"
#include "variant.h"
#include "base.h"

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class OperServ : public mBase
{
    friend class Magick;
private:
    mstring services_admin;
    mstring expire_oper;
    mstring expire_admin;
    mstring expire_sop;
    mstring expire_sadmin;
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

public:
    mstring Services_Admin()	{ return services_admin; }
    mstring Expire_Oper()	{ return expire_oper; }
    mstring Expire_Admin()	{ return expire_admin; }
    mstring Expire_Sop()	{ return expire_sop; }
    mstring Expire_SAdmin()	{ return expire_sadmin; }
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

    void load_database(void);
    void save_database(void);
    OperServ();
    virtual threadtype_enum Get_TType() const { return tt_OperServ; }
    virtual mstring GetInternalName() const { return "OperServ"; }
    void execute(const mstring & message);
};

#endif
