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
#include "pch.h"
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
** Revision 1.16  2000/06/08 13:07:33  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.15  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.14  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.13  2000/05/03 14:12:22  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.12  2000/04/04 03:21:34  prez
** Added support for SVSHOST where applicable.
**
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
    static void do_stats_Oper(mstring mynick, mstring source, mstring params);
    static void do_stats_Other(mstring mynick, mstring source, mstring params);
    static void do_stats_Usage(mstring mynick, mstring source, mstring params);
    static void do_stats_All(mstring mynick, mstring source, mstring params);
    static void do_Stats(mstring mynick, mstring source, mstring params);
    static void do_file_List(mstring mynick, mstring source, mstring params);
    static void do_file_Add(mstring mynick, mstring source, mstring params);
    static void do_file_Del(mstring mynick, mstring source, mstring params);
    static void do_file_Rename(mstring mynick, mstring source, mstring params);
    static void do_file_Priv(mstring mynick, mstring source, mstring params);
    static void do_file_Send(mstring mynick, mstring source, mstring params);
    static void do_file_Dcc(mstring mynick, mstring source, mstring params);
    static void do_file_Cancel(mstring mynick, mstring source, mstring params);
    static void do_file_Lookup(mstring mynick, mstring source, mstring params);
};

#endif
