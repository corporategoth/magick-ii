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
#ifndef _memoserv_h
#define _memoserv_h

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"
#include "variant.h"
#include "base.h"

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class MemoServ : public mBase
{
    friend class Magick;
private:
    int news_expire;

    bool memo;
    bool news;

public:
    int News_Expire() { return news_expire; }
    bool Memo() { return memo; }
    bool News() { return news; }

    virtual void load_database(void);
    virtual void save_database(void);
    MemoServ();
    virtual threadtype_enum Get_TType() const { return tt_MemoServ; }
    virtual mstring GetInternalName() const { return "MemoServ"; }
    virtual void execute(const mstring & message);
};

#endif
