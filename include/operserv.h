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

#ifdef WIN32
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

    // Config Entries ...
    mstring names;		// Names of service (space delimited)
    mstring realname;		// 'Real Name' of service

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

    bool messages;		// Wether to process /MSG, /NOTICE.
    bool automation;		// Wether to do automatic tasks.

public:
    mstring getnames() { return names; }

    bool MSG() { return messages; }
    void MSG(bool on) { messages = on; }
    bool AUTO() { return automation; }
    void AUTO(bool on) { automation = on; }

    mstring Services_Admin();
    int Flood_Time();
    int Flood_Msgs();
    bool Akill();
    bool Flood();
    bool OperDeny();

    OperServ();
    virtual threadtype_enum Get_TType() const { return tt_OperServ; }
    virtual mstring GetInternalName() const { return "OperServ"; }
    void execute(const mstring & message);
};

#endif
