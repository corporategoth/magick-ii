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
#ifndef _SERVMSG_H
#define _SERVMSG_H
static const char *ident_servmsg_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.11  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.10  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


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
