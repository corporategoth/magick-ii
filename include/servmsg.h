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
#ifndef _servmsg_h
#define _servmsg_h

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"
#include "variant.h"
#include "base.h"
// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class ServMsg : public mBase
{
    friend class Magick;
private:
    bool showsync;

public:
    bool ShowSync() { return showsync; }

    void load_database(void);
    void save_database(void);
    ServMsg();
    virtual threadtype_enum Get_TType() const { return tt_OtherServ; }
    virtual mstring GetInternalName() const { return "ServMsg"; }
    void execute(const mstring & message);
};

#endif
