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
** Revision 1.20  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.19  2000/06/25 07:58:48  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.18  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.17  2000/06/09 13:57:00  prez
** Added tracing to mconfig
**
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

	mDateTime i_ClearTime;
	unsigned long i_Global;
	unsigned long i_Credits;
	unsigned long i_file_AddDel;
	unsigned long i_file_Send;
	unsigned long i_file_Change;
	unsigned long i_file_Cancel;
    public:
	stats_t() { clear(); }
	void clear() {
	    i_ClearTime = Now();
	    i_Global = i_Credits = i_file_AddDel = i_file_Send =
	    i_file_Change = i_file_Cancel = 0; }
	mDateTime ClearTime()		{ return i_ClearTime; }
	unsigned long Global()		{ return i_Global; }
	unsigned long Credits()		{ return i_Credits; }
	unsigned long File_AddDel()	{ return i_file_AddDel; }
	unsigned long File_Send()	{ return i_file_Send; }
	unsigned long File_Change()	{ return i_file_Change; }
	unsigned long File_Cancel()	{ return i_file_Cancel; }
    } stats;

    bool ShowSync() { return showsync; }

    ServMsg();
    virtual threadtype_enum Get_TType() const { return tt_OtherServ; }
    virtual mstring GetInternalName() const { return "ServMsg"; }
    virtual void execute(const mstring & message);

    static void do_Help(mstring mynick, mstring source, mstring params);
    static void do_Credits(mstring mynick, mstring source, mstring params);
    static void do_Contrib(mstring mynick, mstring source, mstring params);
    static void do_Languages(mstring mynick, mstring source, mstring params);
    static void do_BreakDown(mstring mynick, mstring source, mstring params);
    static void do_BreakDown2(map<mstring,pair<unsigned int,unsigned int> > ServCounts,
	mstring mynick, mstring source, mstring previndent, mstring server);
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
