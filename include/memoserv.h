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

#ifdef WIN32
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

    // Config Entries ...
    mstring names;		// Names of service (space delimited)
    mstring realname;		// 'Real Name' of service

    int news_expire;

    bool memo;
    bool news;

    bool messages;		// Wether to process /MSG, /NOTICE.
    bool automation;		// Wether to do automatic tasks.

public:
    mstring getnames() { return names; }

    bool MSG() { return messages; }
    void MSG(bool on) { messages = on; }
    bool AUTO() { return automation; }
    void AUTO(bool on) { automation = on; }

    bool Memo();
    bool News();

    MemoServ();
    virtual threadtype_enum Get_TType() const { return tt_MemoServ; }
    virtual mstring GetInternalName() const { return "MemoServ"; }
    void execute(const mstring & message);
};

#endif
