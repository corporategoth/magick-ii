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

    void AddCommands();
    void RemCommands();
public:
    class stats_t
    {
	friend class ServMsg;

	unsigned long i_Global;
	unsigned long i_Credits;
    public:
	stats_t() {
	    i_Global = i_Credits = 0; }
	unsigned long Global()	{ return i_Global; }
	unsigned long Credits()	{ return i_Credits; }
    } stats;

    bool ShowSync() { return showsync; }

    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);
    ServMsg();
    virtual threadtype_enum Get_TType() const { return tt_OtherServ; }
    virtual mstring GetInternalName() const { return "ServMsg"; }
    virtual void execute(const mstring & message);

    static void do_Help(mstring mynick, mstring source, mstring params);
    static void do_Credits(mstring mynick, mstring source, mstring params);
    static void do_Contrib(mstring mynick, mstring source, mstring params);
    static void do_BreakDown(mstring mynick, mstring source, mstring params);
    static void do_BreakDown2(mstring mynick, mstring source, mstring previndent, mstring server);
    static void do_Global(mstring mynick, mstring source, mstring params);
    static void do_stats_Nick(mstring mynick, mstring source, mstring params);
    static void do_stats_Channel(mstring mynick, mstring source, mstring params);
    static void do_stats_Other(mstring mynick, mstring source, mstring params);
    static void do_stats_Usage(mstring mynick, mstring source, mstring params);
    static void do_stats_All(mstring mynick, mstring source, mstring params);
    static void do_Stats(mstring mynick, mstring source, mstring params);
};

#endif
