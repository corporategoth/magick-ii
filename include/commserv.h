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
#ifndef _commserv_h
#define _commserv_h

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"
#include "variant.h"
#include "base.h"

class CommServ : public mBase
{
    friend class Magick;
private:
    bool secure_oper;
protected:
public:
    bool Secure_Oper() { return secure_oper; }


    virtual void load_database(void);
    virtual void save_database(void);

    CommServ();
    virtual threadtype_enum Get_TType() const { return tt_CommServ; };
    virtual mstring GetInternalName() const { return "CommServ"; };
    virtual void execute(const mstring & message);
};

#endif