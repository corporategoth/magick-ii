#ifndef WIN32
#pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
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
RCSID(servmsg_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.29  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.28  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.27  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.25  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.21  2000/12/22 19:50:19  prez
** Made all config options const.  Beginnings of securing all non-modifying
** commands to const.  also added serviceschk.
**
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
    ~ServMsg() {}
    class stats_t
    {
	friend class ServMsg;

	mDateTime i_ClearTime;
	unsigned long i_Global;
	unsigned long i_Ask;
	unsigned long i_Stats;
	unsigned long i_Credits;
	unsigned long i_file_Add;
	unsigned long i_file_Del;
	unsigned long i_file_Send;
	unsigned long i_file_Priv;
	unsigned long i_file_Rename;
	unsigned long i_file_Cancel;
    public:
	stats_t() { clear(); }
	void clear() {
	    i_ClearTime = mDateTime::CurrentDateTime();
	    i_Global = i_Ask = i_Stats = i_Credits = i_file_Add =
	    i_file_Del = i_file_Send = i_file_Priv = i_file_Rename =
	    i_file_Cancel = 0; }
	mDateTime ClearTime()const		{ return i_ClearTime; }
	unsigned long Global()const		{ return i_Global; }
	unsigned long Ask()const		{ return i_Ask; }
	unsigned long Stats()const		{ return i_Stats; }
	unsigned long Credits()const		{ return i_Credits; }
	unsigned long File_Add()const		{ return i_file_Add; }
	unsigned long File_Del()const		{ return i_file_Del; }
	unsigned long File_Send()const		{ return i_file_Send; }
	unsigned long File_Priv()const		{ return i_file_Priv; }
	unsigned long File_Rename()const	{ return i_file_Rename; }
	unsigned long File_Cancel()const	{ return i_file_Cancel; }
    } stats;

    bool ShowSync()const { return showsync; }

    ServMsg();
    virtual threadtype_enum Get_TType() const { return tt_OtherServ; }
    virtual mstring GetInternalName() const { return "ServMsg"; }
    virtual void execute(const mstring & message);

    static void do_Help(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Credits(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Contrib(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Languages(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_BreakDown(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_BreakDown2(map<mstring,pair<unsigned int,unsigned int> > ServCounts,
	const mstring &mynick, const mstring &source, const mstring &previndent, const mstring &server);
    static void do_Global(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Ask(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_stats_Nick(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_stats_Channel(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_stats_Oper(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_stats_Other(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_stats_Usage(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_stats_All(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_Stats(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_file_List(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_file_Add(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_file_Del(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_file_Rename(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_file_Priv(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_file_Send(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_file_Dcc(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_file_Cancel(const mstring &mynick, const mstring &source, const mstring &params);
    static void do_file_Lookup(const mstring &mynick, const mstring &source, const mstring &params);
};

#endif
