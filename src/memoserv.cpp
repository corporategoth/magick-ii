#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
#define RCSID(x,y) const char *rcsid_memoserv_cpp_ ## x () { return y; }
RCSID(memoserv_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.104  2001/11/03 21:02:53  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.103  2001/06/17 09:39:07  prez
** Hopefully some more changes that ensure uptime (mainly to do with locking
** entries in an iterated search, and using copies of data instead of references
** where we can get away with it -- reducing the need to lock the data).
**
** Revision 1.102  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.101  2001/05/23 02:43:48  prez
** Fixed the NOACCESS bug, the chanserv getpass/setpass bug and nickserv failed
** passwords kill bug.
**
** Revision 1.100  2001/05/17 19:18:55  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.99  2001/05/06 03:03:07  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.98  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.97  2001/05/01 14:00:24  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.96  2001/04/13 08:50:48  prez
** Fixed const for unix
**
** Revision 1.95  2001/04/13 03:10:02  ungod
** more changes to make borland compilable
** (still not so in ide, but command line compile works)
**
** Revision 1.94  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.93  2001/03/27 07:04:31  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.92  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.91  2001/03/08 08:07:41  ungod
** fixes for bcc 5.5
**
** Revision 1.90  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.89  2001/02/11 07:41:28  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.88  2001/02/03 02:21:34  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.87  2001/01/15 23:31:39  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.86  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.85  2000/12/31 18:17:17  prez
** Fixed MS reply
**
** Revision 1.84  2000/12/29 15:31:55  prez
** Added locking/checking for dcc/events threads.  Also for ACE_Log_Msg
**
** Revision 1.83  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.82  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.81  2000/12/10 13:06:12  prez
** Ditched alot of the *toa's since mstring can do it internally now.
**
** Revision 1.80  2000/12/09 20:16:41  prez
** Fixed SubString and Left to have correct count/end possitions.  Also
** adjusted rest of source to follow suit.
**
** Revision 1.79  2000/10/10 11:47:52  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.78  2000/09/30 10:48:08  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.77  2000/09/18 08:17:57  prez
** Intergrated mpatrol into the xml/des sublibs, and did
** some minor fixes as a result of mpatrol.
**
** Revision 1.76  2000/09/09 02:17:48  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.75  2000/09/05 10:53:07  prez
** Only have operserv.cpp and server.cpp to go with T_Changing / T_Modify
** tracing -- also modified keygen to allow for cmdline generation (ie.
** specify 1 option and enter keys, or 2 options and the key is read from
** a file).  This allows for paragraphs with \n's in them, and helps so you
** do not have to type out 1024 bytes :)
**
** Revision 1.74  2000/09/02 07:20:46  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.73  2000/08/28 10:51:38  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.72  2000/08/19 10:59:47  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.71  2000/08/07 12:20:28  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.70  2000/08/02 20:08:57  prez
** Minor code cleanups, added ACE installation instructions, updated the
** suggestions file and stopped people doing a whole bunch of stuff to
** forbidden nicknames.
**
** Revision 1.69  2000/07/29 21:58:54  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.68  2000/07/21 00:18:49  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.67  2000/06/18 13:31:48  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.66  2000/06/18 12:49:27  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.65  2000/06/12 06:07:50  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.64  2000/06/11 09:30:21  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.63  2000/06/11 08:20:12  prez
** More minor bug fixes, godda love testers.
**
** Revision 1.62  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.61  2000/06/06 08:57:57  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.60  2000/05/27 07:06:02  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.59  2000/05/25 08:16:39  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.58  2000/05/18 11:41:46  prez
** Fixed minor front-end issues with the filesystem...
**
** Revision 1.57  2000/05/17 14:08:11  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.56  2000/05/17 07:47:59  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.55  2000/05/14 06:30:14  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.54  2000/05/14 04:02:54  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.53  2000/05/13 07:05:46  prez
** Added displaying of sizes to all file fields..
**
** Revision 1.52  2000/05/09 09:11:59  prez
** Added XMLisation to non-mapped structures ... still need to
** do the saving stuff ...
**
** Revision 1.51  2000/05/03 14:12:23  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.50  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.49  2000/04/04 03:21:35  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.48  2000/04/03 09:45:24  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.47  2000/03/29 09:41:19  prez
** Attempting to fix thread problem with mBase, and added notification
** of new memos on join of channel or signon to network.
**
** Revision 1.46  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.45  2000/03/26 14:59:37  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.44  2000/03/23 10:22:25  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.43  2000/03/19 08:50:55  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.42  2000/03/08 23:38:37  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.41  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.40  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.39  2000/02/17 12:55:05  ungod
** still working on borlandization
**
** Revision 1.38  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.37  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.36  2000/02/15 10:37:50  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"
#include "dccengine.h"

#ifndef MAGICK_HAS_EXCEPTIONS
static MemoServ::nick_memo_t GLOB_nick_memo_t;
static Memo_t GLOB_Memo_t;
static MemoServ::channel_news_t GLOB_channel_news_t;
static News_t GLOB_News_t;
#endif

Memo_t::Memo_t(const mstring& nick, const mstring& sender, const mstring& text,
	const unsigned long file)
	: i_Nick(nick), i_Sender(sender), i_Time(mDateTime::CurrentDateTime()),
	  i_Text(text), i_Read(false), i_File(file)
{
    FT("Memo_t::Memo_t", (nick, sender, text, file));
    DumpE();
}


void Memo_t::operator=(const Memo_t &in)
{
    FT("Memo_t::operator=", ("(const Memo_t &) in"));
    i_Nick = in.i_Nick;
    i_Time = in.i_Time;
    i_Sender = in.i_Sender;
    i_Text = in.i_Text;
    i_File = in.i_File;
    i_Read = in.i_Read;
}

void Memo_t::ChgNick(const mstring& in)
{
    FT("Memo_t::ChgNick", (in));
    WLOCK(("MemoServ", "nick", i_Nick.LowerCase(), "i_Nick"));
    MCB(i_Nick);
    i_Nick = in;
    MCE(i_Nick);
}

mstring Memo_t::Sender() const
{
    NFT(("Memo_t::Sender"));
    RLOCK(("MemoServ", "nick", i_Nick.LowerCase(), "i_Sender"));
    RET(i_Sender);
}

mDateTime Memo_t::Time() const
{
    NFT(("Memo_t::Time"));
    RLOCK(("MemoServ", "nick", i_Nick.LowerCase(), "i_Time"));
    RET(i_Time);
}

mstring Memo_t::Text() const
{
    NFT(("Memo_t::Text"));
    RLOCK(("MemoServ", "nick", i_Nick.LowerCase(), "i_Text"));
    RET(i_Text);
}

unsigned long Memo_t::File() const
{
    NFT(("Memo_t::File"));
    RLOCK(("MemoServ", "nick", i_Nick.LowerCase(), "i_File"));
    RET(i_File);
}

bool Memo_t::IsRead() const
{
    NFT(("Memo_t::IsRead"));
    RLOCK(("MemoServ", "nick", i_Nick.LowerCase(), "i_Read"));
    RET(i_Read);
}
void Memo_t::Read()
{
    NFT(("Memo_t::Read"));
    WLOCK(("MemoServ", "nick", i_Nick.LowerCase(), "i_Read"));
    MCB(i_Read);
    i_Read = true;
    MCE(i_Read);
}
void Memo_t::Unread()
{
    NFT(("Memo_t::Unread"));
    WLOCK(("MemoServ", "nick", i_Nick.LowerCase(), "i_Read"));
    MCB(i_Read);
    i_Read = false;
    MCE(i_Read);
}

size_t Memo_t::Usage() const
{
    size_t retval = 0;
    WLOCK(("MemoServ", "nick", i_Nick.LowerCase()));
    retval += i_Nick.capacity();
    retval += i_Sender.capacity();
    retval += i_Text.capacity();
    retval += sizeof(i_Time.Internal());
    retval += sizeof(i_File);
    retval += sizeof(i_Read);
    map<mstring,mstring>::const_iterator j;
    for (j=i_UserDef.begin(); j!=i_UserDef.end(); j++)
    {
	retval += j->first.capacity();
	retval += j->second.capacity();
    }
    return retval;
}

void Memo_t::DumpB() const
{
    MB(0, (i_Nick, i_Sender, i_Text, i_Time, i_Read, i_File,
	i_UserDef.size()));
}

void Memo_t::DumpE() const
{
    ME(0, (i_Nick, i_Sender, i_Text, i_Time, i_Read, i_File,
	i_UserDef.size()));
}


News_t::News_t(const mstring& channel, const mstring& sender, const mstring& text,
	const bool noexpire)
	: i_Channel(channel), i_Sender(sender), i_Time(mDateTime::CurrentDateTime()),
	  i_Text(text), i_NoExpire(noexpire)
{
    FT("News_t::News_t", (channel, sender, text));
    DumpE();
}


void News_t::operator=(const News_t &in)
{
    FT("News_t::operator=", ("(const News_t &) in"));
    i_Channel = in.i_Channel;
    i_Time = in.i_Time;
    i_Sender = in.i_Sender;
    i_Text = in.i_Text;
    i_Read = in.i_Read;
    i_NoExpire = in.i_NoExpire;
}


mstring News_t::Sender() const
{
    NFT("News_t::Sender");
    RLOCK(("MemoServ", "channel", i_Channel.LowerCase(), "i_Sender"));
    RET(i_Sender);
}

mDateTime News_t::Time() const
{
    NFT("News_t::Time");
    RLOCK(("MemoServ", "channel", i_Channel.LowerCase(), "i_Time"));
    RET(i_Time);
}

mstring News_t::Text() const
{
    NFT("News_t::Text");
    RLOCK(("MemoServ", "channel", i_Channel.LowerCase(), "i_Text"));
    RET(i_Text);
}


bool News_t::NoExpire() const
{
    NFT("News_t::NoExpire");
    RLOCK(("MemoServ", "channel", i_Channel.LowerCase(), "i_NoExpire"));
    RET(i_NoExpire);
}

void News_t::NoExpire(const bool in)
{
    NFT("News_t::NoExpire");
    WLOCK(("MemoServ", "channel", i_Channel.LowerCase(), "i_NoExpire"));
    MCB(i_NoExpire);
    i_NoExpire = in;
    MCE(i_NoExpire);
}

bool News_t::IsRead(const mstring& name)
{
    FT("News_t::IsRead", (name));
    mstring target(name);
    if (!Parent->nickserv.IsStored(name))
	RET(false);
    if (!Parent->nickserv.GetStored(name).Host().empty())
	target = Parent->nickserv.GetStored(name).Host();
    RLOCK(("MemoServ", "channel", i_Channel.LowerCase(), "i_Read"));
    bool retval (i_Read.find(target.LowerCase())!=i_Read.end());
    RET(retval);
}


void News_t::Read(const mstring& name)
{
    FT("News_t::Read", (name));
    mstring target = name;
    if (!Parent->nickserv.IsStored(name))
	return;
    if (!Parent->nickserv.GetStored(name).Host().empty())
	target = Parent->nickserv.GetStored(name).Host();
    WLOCK(("MemoServ", "channel", i_Channel.LowerCase(), "i_Read"));
    MCB(i_Read.size());
    i_Read.insert(target.LowerCase());
    MCE(i_Read.size());
}


void News_t::Unread(const mstring& name)
{
    FT("News_t::Unread", (name));
    mstring target = name;
    if (!Parent->nickserv.IsStored(name))
	return;
    if (!Parent->nickserv.GetStored(name).Host().empty())
	target = Parent->nickserv.GetStored(name).Host();
    WLOCK(("MemoServ", "channel", i_Channel.LowerCase(), "i_Read"));
    MCB(i_Read.size());
    i_Read.erase(name.LowerCase());
    i_Read.erase(target.LowerCase());
    MCE(i_Read.size());
}

size_t News_t::Usage() const
{
    size_t retval = 0;
    WLOCK(("MemoServ", "channel", i_Channel.LowerCase()));
    retval += i_Channel.capacity();
    retval += i_Sender.capacity();
    retval += i_Text.capacity();
    retval += sizeof(i_Time.Internal());
    retval += sizeof(i_NoExpire);
    set<mstring>::const_iterator i;
    for (i=i_Read.begin(); i!=i_Read.end(); i++)
	retval += i->capacity();
    map<mstring,mstring>::const_iterator j;
    for (j=i_UserDef.begin(); j!=i_UserDef.end(); j++)
    {
	retval += j->first.capacity();
	retval += j->second.capacity();
    }
    return retval;
}

void News_t::DumpB() const
{
    MB(0, (i_Channel, i_Sender, i_Text, i_Time, i_NoExpire, i_Read.size(),
	i_UserDef.size()));
}

void News_t::DumpE() const
{
    ME(0, (i_Channel, i_Sender, i_Text, i_Time, i_NoExpire, i_Read.size(),
	i_UserDef.size()));
}


MemoServ::MemoServ()
{
    NFT("MemoServ::MemoServ");
    messages = true;
}

void MemoServ::AddCommands()
{
    NFT("MemoServ::AddCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "H*LP", Parent->commserv.ALL_Name(), MemoServ::do_Help);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "READ*", Parent->commserv.REGD_Name(), MemoServ::do_Read);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNREAD*", Parent->commserv.REGD_Name(), MemoServ::do_UnRead);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "GET*", Parent->commserv.REGD_Name(), MemoServ::do_Get);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LIST*", Parent->commserv.REGD_Name(), MemoServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "VIEW*", Parent->commserv.REGD_Name(), MemoServ::do_List);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SEND*", Parent->commserv.REGD_Name(), MemoServ::do_Send);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FLUSH*", Parent->commserv.REGD_Name(), MemoServ::do_Flush);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FORW*", Parent->commserv.REGD_Name(), MemoServ::do_Forward);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "F*W*D", Parent->commserv.REGD_Name(), MemoServ::do_Forward);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "REP*", Parent->commserv.REGD_Name(), MemoServ::do_Reply);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CAN*", Parent->commserv.REGD_Name(), MemoServ::do_Cancel);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ABOR*", Parent->commserv.REGD_Name(), MemoServ::do_Cancel);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "DEL*", Parent->commserv.REGD_Name(), MemoServ::do_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "ERA*", Parent->commserv.REGD_Name(), MemoServ::do_Del);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "CONT*", Parent->commserv.REGD_Name(), MemoServ::do_Continue);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "P*VIEW*", Parent->commserv.REGD_Name(), MemoServ::do_Preview);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "FILE*", Parent->commserv.REGD_Name(), MemoServ::do_File);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "*ATTACH", Parent->commserv.REGD_Name(), MemoServ::do_File);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* NOEX*", Parent->commserv.REGD_Name(), MemoServ::do_set_NoExpire);

    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* #*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET* *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "SET*", Parent->commserv.REGD_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK #*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "LOCK", Parent->commserv.REGD_Name(), do_1_2param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK #*", Parent->commserv.REGD_Name(), do_1_3param);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK *", Parent->commserv.REGD_Name(), NULL);
    Parent->commands.AddSystemCommand(GetInternalName(),
	    "UNLOCK", Parent->commserv.REGD_Name(), do_1_2param);
}

void MemoServ::RemCommands()
{
    NFT("MemoServ::RemCommands");
    // Put in ORDER OF RUN.  ie. most specific to least specific.

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "H*LP", Parent->commserv.ALL_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "READ*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNREAD*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "GET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LIST*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "VIEW*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SEND*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FLUSH*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FORW*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "F*W*D", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "REP*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CAN*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ABOR*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "DEL*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "ERA*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "CONT*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "P*VIEW*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "FILE*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "*ATTACH", Parent->commserv.REGD_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* NOEX*", Parent->commserv.REGD_Name());

    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* #*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET* *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "SET*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK #*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "LOCK", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK #*", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK *", Parent->commserv.REGD_Name());
    Parent->commands.RemSystemCommand(GetInternalName(),
	    "UNLOCK", Parent->commserv.REGD_Name());
}

void MemoServ::NickMemoConvert(const mstring &source, const mstring &target)
{
    FT("MemoServ::NickMemoConvert", (source, target));

    WLOCK(("MemoServ", "nick", source));

    MemoServ::nick_t::iterator iter = nick.find(source.LowerCase());
    if (iter != nick.end())
    {
	MemoServ::nick_memo_t::iterator iter2;
	for (iter2=iter->second.begin(); iter2 != iter->second.end(); iter2++)
	{
	    /* (*iter2)->ChgNick(target); */
	    iter2->ChgNick(target);
	    /* AddNickMemo(*iter2); */
	    AddNickMemo(&(*iter2));
	}
	// DELIBERATELY dont delete the members of the MemoServ::nick_memo_t
	// for the source nick ... we're just renaming and moving.
	WLOCK2(("MemoServ", "nick"));
	nick.erase(source.LowerCase());
    }
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::AddNick(nick_memo_t in) throw(E_MemoServ_Nick)
#else
void MemoServ::AddNick(nick_memo_t in)
#endif
{
    FT("MemoServ::AddNick", ("(nick_memo_t) in"));

    if (!in.size())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Add, E_MemoServ_Nick::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Memo", "Nick", "Add", "Blank"));
	return;
#endif
    }

#if 0
	if (in.begin() == NULL)
	{
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Add, E_MemoServ_Nick::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Memo", "Nick", "Add", "Invalid"));
	return;
#endif
	}
#endif

    if (in.begin()->Nick().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Add, E_MemoServ_Nick::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Memo", "Nick", "Add", "Blank"));
	return;
#endif
    }

    WLOCK(("MemoServ", "nick"));
    nick[in.begin()->Nick().LowerCase()] = in;
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::AddNickMemo(Memo_t *in) throw(E_MemoServ_Nick)
#else
void MemoServ::AddNickMemo(Memo_t *in)
#endif
{
    FT("MemoServ::AddNickMemo", ("(Memo_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Add, E_MemoServ_Nick::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Memo", "NickMemo", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Nick().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Add, E_MemoServ_Nick::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Memo", "NickMemo", "Add", "Blank"));
	return;
#endif
    }

    WLOCK(("MemoServ", "nick"));
    WLOCK2(("MemoServ", "nick", in->Nick().LowerCase()));
    /* nick[in->Nick().LowerCase()].push_back(in); */
    nick[in->Nick().LowerCase()].push_back(*in);
}

#ifdef MAGICK_HAS_EXCEPTIONS
MemoServ::nick_memo_t &MemoServ::GetNick(const mstring &in) const throw(E_MemoServ_Nick)
#else
MemoServ::nick_memo_t &MemoServ::GetNick(const mstring &in) const
#endif
{
    FT("MemoServ::GetNick", (in));

    RLOCK(("MemoServ", "nick", in.LowerCase()));
    MemoServ::nick_t::const_iterator iter = nick.find(in.LowerCase());
    if (iter == nick.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Get, E_MemoServ_Nick::T_NotFound, in.c_str()));
#else
	NLOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Memo", "Nick", "Get", "NotFound", in));
	NRET(MemoServ::nick_memo_t &, GLOB_nick_memo_t);
#endif
    }
    if (!iter->second.size())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Get, E_MemoServ_Nick::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("Memo", "Nick", "Get", "Blank", in));
	NRET(MemoServ::nick_memo_t &, GLOB_nick_memo_t);
#endif
    }

    NRET(MemoServ::nick_memo_t &, const_cast<MemoServ::nick_memo_t &>(iter->second));
}

#ifdef MAGICK_HAS_EXCEPTIONS
Memo_t &MemoServ::GetNickMemo(const mstring &in, const size_t num) const throw(E_MemoServ_Nick)
#else
Memo_t &MemoServ::GetNickMemo(const mstring &in, const size_t num) const
#endif
{
    FT("MemoServ::GetNickMemo", (in, num));

    RLOCK(("MemoServ", "nick", in.LowerCase()));
    MemoServ::nick_memo_t &ent = GetNick(in);

    size_t i;
    MemoServ::nick_memo_t::const_iterator iter;
    for (iter=ent.begin(), i=0; iter != ent.end() && i < num; iter++, i++) ;

    if (i < num || iter==ent.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Get, E_MemoServ_Nick::T_NotFound, in.c_str(), num));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC2", ("Memo", "NickMemo", "Get", "NotFound", in, num));
	NRET(Memo_t &, GLOB_Memo_t);
#endif
    }
    /* if (*iter == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Get, E_MemoServ_Nick::T_Invalid, in.c_str(), num))
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC2", ("Memo", "NickMemo", "Get", "Invalid", in, num));
	NRET(Memo_t &, GLOB_Memo_t);
#endif
    }
    if ((*iter)->Nick().empty()) */
    if (iter->Nick().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Get, E_MemoServ_Nick::T_Blank, in.c_str(), num));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC2", ("Memo", "NickMemo", "Get", "Blank", in, num));
	NRET(Memo_t &, GLOB_Memo_t);
#endif
    }

    /* NRET(Memo_t &, const_cast<Memo_t &>(**iter)); */
    NRET(Memo_t &, const_cast<Memo_t &>(*iter));
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::RemNick(const mstring &in) throw(E_MemoServ_Nick)
#else
void MemoServ::RemNick(const mstring &in)
#endif
{
    FT("MemoServ::RemNick", (in));

    WLOCK(("MemoServ", "nick"));
    MemoServ::nick_t::iterator iter = nick.find(in.LowerCase());
    if (iter == nick.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Rem, E_MemoServ_Nick::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("Memo", "Nick", "Rem", "NotFound", in));
	return;
#endif
    }
    WLOCK2(("MemoServ", "nick", iter->first));
    /* nick_memo_t::iterator iter2;
    for (iter2=iter->second.begin(); iter2!=iter->second.end(); iter2++)
	if (*iter2 != NULL)
	    delete *iter2;
    */
    nick.erase(iter);
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::RemNickMemo(const mstring &in, const size_t num) throw(E_MemoServ_Nick)
#else
void MemoServ::RemNickMemo(const mstring &in, const size_t num)
#endif
{
    FT("MemoServ::RemNickMemo", (in, num));

    WLOCK(("MemoServ", "nick"));
    MemoServ::nick_t::iterator iter = nick.find(in.LowerCase());
    if (iter == nick.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Rem, E_MemoServ_Nick::T_NotFound, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("Memo", "Nick", "Rem", "NotFound", in, num));
	return;
#endif
    }
    WLOCK2(("MemoServ", "nick", iter->first));

    size_t i;
    MemoServ::nick_memo_t::iterator iter2;
    for (iter2=iter->second.begin(), i=0; iter2 != iter->second.end() && i < num; iter2++, i++) ;

    if (i < num || iter2==iter->second.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Nick(E_MemoServ_Nick::W_Rem, E_MemoServ_Nick::T_NotFound, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("Memo", "NickMemo", "Rem", "NotFound", in, num));
	return;
#endif
    }
    /* if (*iter2 != NULL)
	delete *iter2 */
    iter->second.erase(iter2);

    if (!iter->second.size())
	nick.erase(iter);
}

bool MemoServ::IsNick(const mstring& in) const
{
    FT("MemoServ::IsNick", (in));
    RLOCK(("MemoServ", "nick"));
    bool retval = (nick.find(in.LowerCase())!=nick.end());
    RET(retval);
}

bool MemoServ::IsNickMemo(const mstring &in, const size_t num) const
{
    FT("MemoServ::IsNickMemo", (in, num));

    RLOCK(("MemoServ", "nick"));
    MemoServ::nick_t::const_iterator iter = nick.find(in.LowerCase());
    if (iter != nick.end())
    {
	size_t i;
	MemoServ::nick_memo_t::const_iterator iter2;
	for (iter2=iter->second.begin(), i=0; iter2 != iter->second.end() && i < num; iter2++, i++) ;

	if (!(i < num || iter2==iter->second.end()))
	{
	    RET(true);
	}
    }
    RET(false);
}


size_t MemoServ::NickMemoCount(const mstring &in, const bool read) const
{
    FT("MemoServ::NickMemoCount", (in, read));

    size_t retval = 0;
    RLOCK(("MemoServ", "nick", in.LowerCase()));
    MemoServ::nick_t::const_iterator iter = nick.find(in.LowerCase());
    if (iter != nick.end())
    {
	MemoServ::nick_memo_t::const_iterator iter2;
	for (iter2=iter->second.begin(); iter2 != iter->second.end(); iter2++)
	{
	    if (iter2->IsRead() == read)
		retval++;
	}
    }
    RET(retval);
}


#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::AddChannel(channel_news_t in) throw(E_MemoServ_Channel)
#else
void MemoServ::AddChannel(channel_news_t in)
#endif
{
    FT("MemoServ::AddChannel", ("(channel_news_t) in"));

    if (!in.size())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Add, E_MemoServ_Channel::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("News", "Channel", "Add", "Blank"));
	return;
#endif
    }

#if 0
	if (in.begin() == NULL)
	{
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Add, E_MemoServ_Channel::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("News", "Channel", "Add", "Invalid"));
	return;
#endif
	}
#endif	

    if (in.begin()->Channel().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Add, E_MemoServ_Channel::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("News", "Channel", "Add", "Blank"));
	return;
#endif
    }

    WLOCK(("MemoServ", "channel"));
    channel[in.begin()->Channel().LowerCase()] = in;
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::AddChannelNews(News_t *in) throw(E_MemoServ_Channel)
#else
void MemoServ::AddChannelNews(News_t *in)
#endif
{
    FT("MemoServ::AddChannelNews", ("(News_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Add, E_MemoServ_Channel::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("News", "ChannelNews", "Add", "Invalid"));
	return;
#endif
    }

    if (in->Channel().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Add, E_MemoServ_Channel::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("News", "ChannelNews", "Add", "Blank"));
	return;
#endif
    }

    WLOCK(("MemoServ", "channel"));
    WLOCK2(("MemoServ", "channel", in->Channel().LowerCase()));
    /* channel[in->Channel().LowerCase()].push_back(in); */
    channel[in->Channel().LowerCase()].push_back(*in);
}

#ifdef MAGICK_HAS_EXCEPTIONS
MemoServ::channel_news_t &MemoServ::GetChannel(const mstring &in) const throw(E_MemoServ_Channel)
#else
MemoServ::channel_news_t &MemoServ::GetChannel(const mstring &in) const
#endif
{
    FT("MemoServ::GetChannel", (in));

    RLOCK(("MemoServ", "channel", in.LowerCase()));
    MemoServ::channel_t::const_iterator iter = channel.find(in.LowerCase());
    if (iter == channel.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Get, E_MemoServ_Channel::T_NotFound, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("News", "Channel", "Get", "NotFound", in));
	NRET(MemoServ::channel_news_t &, GLOB_channel_news_t);
#endif
    }
    if (!iter->second.size())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Get, E_MemoServ_Channel::T_Blank, in.c_str()));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC1", ("News", "Channel", "Get", "Blank", in));
	NRET(MemoServ::channel_news_t &, GLOB_channel_news_t);
#endif
    }

    NRET(MemoServ::channel_news_t &, const_cast<MemoServ::channel_news_t &>(iter->second));
}

#ifdef MAGICK_HAS_EXCEPTIONS
News_t &MemoServ::GetChannelNews(const mstring &in, const size_t num) const throw(E_MemoServ_Channel)
#else
News_t &MemoServ::GetChannelNews(const mstring &in, const size_t num) const
#endif
{
    FT("MemoServ::GetChannelNews", (in, num));

    RLOCK(("MemoServ", "channel", in.LowerCase()));
    MemoServ::channel_news_t &ent = GetChannel(in);

    size_t i;
    MemoServ::channel_news_t::const_iterator iter;
    for (iter=ent.begin(), i=0; iter != ent.end() && i < num; iter++, i++) ;

    if (i < num || iter==ent.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Get, E_MemoServ_Channel::T_NotFound, in.c_str(), num));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC2", ("News", "ChannelNews", "Get", "NotFound", in, num));
	NRET(News_t &, GLOB_News_t);
#endif
    }
    /* if (*iter == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Get, E_MemoServ_Channel::T_Invalid, in.c_str(), num))
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC2", ("News", "ChannelNews", "Get", "Invalid", in, num));
	NRET(News_t &, GLOB_News_t);
#endif
    }
    if ((*iter)->Channel().empty()) */
    if (iter->Channel().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Get, E_MemoServ_Channel::T_Blank, in.c_str(), num));
#else
	LOG(LM_EMERGENCY, "EXCEPTIONS/GENERIC2", ("News", "ChannelNews", "Get", "Blank", in, num));
	NRET(News_t &, GLOB_News_t);
#endif
    }

    /* NRET(News_t &, const_cast<News_t &>(**iter)); */
    NRET(News_t &, const_cast<News_t &>(*iter));
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::RemChannel(const mstring &in) throw(E_MemoServ_Channel)
#else
void MemoServ::RemChannel(const mstring &in)
#endif
{
    FT("MemoServ::RemChannel", (in));

    WLOCK(("MemoServ", "channel"));
    MemoServ::channel_t::iterator iter = channel.find(in.LowerCase());
    if (iter == channel.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Rem, E_MemoServ_Channel::T_NotFound, in.c_str()));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC1", ("News", "Channel", "Rem", "NotFound", in));
	return;
#endif
    }
    WLOCK2(("MemoServ", "channel", iter->first));
    /* channel_news_t::iterator iter2;
    for (iter2=iter->second.begin(); iter2!=iter->second.end(); iter2++)
	if (*iter2 != NULL)
	    delete *iter2;
    */
    channel.erase(iter);
}

#ifdef MAGICK_HAS_EXCEPTIONS
void MemoServ::RemChannelNews(const mstring &in, const size_t num) throw(E_MemoServ_Channel)
#else
void MemoServ::RemChannelNews(const mstring &in, const size_t num)
#endif
{
    FT("MemoServ::RemChannelNews", (in, num));

    WLOCK(("MemoServ", "channel"));
    MemoServ::channel_t::iterator iter = channel.find(in.LowerCase());
    if (iter == channel.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Rem, E_MemoServ_Channel::T_NotFound, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("News", "Channel", "Rem", "NotFound", in, num));
	return;
#endif
    }
    WLOCK2(("MemoServ", "channel", iter->first));

    size_t i;
    MemoServ::channel_news_t::iterator iter2;
    for (iter2=iter->second.begin(), i=0; iter2 != iter->second.end() && i < num; iter2++, i++) ;

    if (i < num || iter2==iter->second.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_MemoServ_Channel(E_MemoServ_Channel::W_Rem, E_MemoServ_Channel::T_NotFound, in.c_str(), num));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC2", ("News", "ChannelNews", "Rem", "NotFound", in, num));
	return;
#endif
    }
    /* if (*iter2 != NULL)
	delete *iter2 */
    iter->second.erase(iter2);

    if (!iter->second.size())
	channel.erase(iter);
}

bool MemoServ::IsChannel(const mstring& in) const
{
    FT("MemoServ::IsChannel", (in));
    RLOCK(("MemoServ", "channel"));
    bool retval = (channel.find(in.LowerCase())!=channel.end());
    RET(retval);
}

bool MemoServ::IsChannelNews(const mstring &in, const size_t num) const
{
    FT("MemoServ::IsChannelNews", (in, num));

    RLOCK(("MemoServ", "channel"));
    MemoServ::channel_t::const_iterator iter = channel.find(in.LowerCase());
    if (iter != channel.end())
    {
	size_t i;
	MemoServ::channel_news_t::const_iterator iter2;
	for (iter2=iter->second.begin(), i=0; iter2 != iter->second.end() && i < num; iter2++, i++) ;

	if (!(i < num || iter2==iter->second.end()))
	{
	    RET(true);
	}
    }
    RET(false);
}

size_t MemoServ::ChannelNewsCount(const mstring &in, const mstring &user, const bool read)
{
    FT("MemoServ::ChannelNewsCount", (in, user, read));

    size_t retval = 0;
    RLOCK(("MemoServ", "channel", in.LowerCase()));
    MemoServ::channel_t::iterator iter = channel.find(in.LowerCase());
    if (iter != channel.end())
    {
	MemoServ::channel_news_t::iterator iter2;
	for (iter2=iter->second.begin(); iter2 != iter->second.end(); iter2++)
	{
	    if (iter2->IsRead(user) == read)
		retval++;
	}
    }
    RET(retval);
}

void MemoServ::execute(mstring& source, const mstring& msgtype, const mstring& params)
{
    mThread::ReAttach(tt_MemoServ);
    FT("MemoServ::execute", (source, msgtype, params));
    //okay this is the main memoserv command switcher

    // Nick/Server PRIVMSG/NOTICE mynick :message
    mstring mynick(Parent->getLname(params.ExtractWord(1, ": ")));
    mstring message(params.After(":"));
    mstring command(message.Before(" "));

    if (message[0U] == CTCP_DELIM_CHAR)
    {
	if (msgtype == "PRIVMSG")
	    DccEngine::decodeRequest(mynick, source, message);
	else
	    DccEngine::decodeReply(mynick, source, message);
    }
    else if (msgtype == "PRIVMSG" &&
	!Parent->commands.DoCommand(mynick, source, command, message))
    {
	// Invalid command or not enough privs.
    }


    mThread::ReAttach(tt_mBase);

}

void MemoServ::do_Help(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Help", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    mstring HelpTopic = Parent->memoserv.GetInternalName();
    if (params.WordCount(" ") > 1)
	HelpTopic += " " + params.After(" ");
    HelpTopic.replace(" ", "/");
    vector<mstring> help = Parent->getHelp(source, HelpTopic.UpperCase());
					
    unsigned int i;
    for (i=0; i<help.size(); i++)
	::send(mynick, source, help[i]);
}

void MemoServ::do_Read(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Read", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.After(" ", 2);
	mstring whoami = Parent->nickserv.GetStored(source).Host().LowerCase();
	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Parent->chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
				who));
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.GetStored(who).GetAccess(whoami, "READMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (
				who.LowerCase()));
	    return;
	}

	Parent->memoserv.stats.i_Read++;
	if (what.IsSameAs("all", true) ||
	    what.IsSameAs("new", true) || what.IsSameAs("unread", true))
	{
	    bool unread = (what.IsSameAs("new", true) ||
				what.IsSameAs("unread", true));
	    MemoServ::channel_news_t::iterator iter;
	    int i = 0;
	    RLOCK(("MemoServ", "channel", who.LowerCase()));
	    for (iter = Parent->memoserv.ChannelNewsBegin(who);
		    iter != Parent->memoserv.ChannelNewsEnd(who); iter++)
	    {
		i++;
		if (unread && iter->IsRead(whoami))
		    continue;

		iter->Read(whoami);
		SEND(mynick, source, "MS_COMMAND/NEWS", (
			i, iter->Sender(), iter->Channel(),
			iter->Time().Ago()));
		unsigned int sentsize;
		mstring output;
		for (sentsize = 0; sentsize < iter->Text().size();
				sentsize+=Parent->server.proto.MaxLine())
		{
		    if (sentsize + Parent->server.proto.MaxLine() < iter->Text().size())
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize, sentsize+Parent->server.proto.MaxLine())
			       << "...";
		    }
		    else
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize, iter->Text().size()-1);
		    }
		    ::send(mynick, source, "    " + output);
		    output.erase();
		}
	    }
	    if (i==1 && unread)
		SEND(mynick, source, "MS_STATUS/CS_NOUNREAD", (
			    who.LowerCase()));
	}
	else
	{
	    vector<int> numbers = ParseNumbers(what);
	    unsigned int i;
	    int j=1;
	    mstring output;
	    bool triedabove = false, nonnumeric = false;
	    size_t amt = 0;
	    RLOCK(("MemoServ", "channel", who.LowerCase()));
	    MemoServ::channel_news_t &newslist = Parent->memoserv.GetChannel(who);
	    amt = newslist.size();
	    MemoServ::channel_news_t::iterator iter = newslist.begin();
	    for (i=0; i<numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > static_cast<int>(newslist.size()))
		    triedabove = true;
		else
		{
		    while (numbers[i] < j &&
			iter != newslist.begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j &&
			iter != newslist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != newslist.end())
		    {
			iter->Read(whoami);
			SEND(mynick, source, "MS_COMMAND/NEWS", (
				j, iter->Sender(), iter->Sender(),
				iter->Time().Ago()));
			unsigned int sentsize;
			for (sentsize = 0; sentsize < iter->Text().size();
					sentsize+=Parent->server.proto.MaxLine())
			{
			    if (sentsize + Parent->server.proto.MaxLine() < iter->Text().size())
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize, sentsize+Parent->server.proto.MaxLine())
				    << "...";
			    }
			    else
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize, iter->Text().size()-1);
			    }
			    ::send(mynick, source, "    " + output);
			    output.erase();
			}
		    }
		}
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", ( amt));
	}
    }
    else
    {
	mstring who = Parent->nickserv.GetStored(source).Host();
	mstring what = params.After(" ", 1);

	if (who.empty())
	    who = source;

	if (!Parent->memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	Parent->memoserv.stats.i_Read++;
	if (what.IsSameAs("all", true) ||
	    what.IsSameAs("new", true) || what.IsSameAs("unread", true))
	{
	    MemoServ::nick_memo_t::iterator iter;
	    bool unread = (what.IsSameAs("new", true) ||
				what.IsSameAs("unread", true));
	    int i = 0;
	    RLOCK(("MemoServ", "nick", who.LowerCase()));
	    for (iter = Parent->memoserv.NickMemoBegin(who);
		    iter != Parent->memoserv.NickMemoEnd(who); iter++)
	    {
		i++;
		if (unread && iter->IsRead())
		    continue;

		iter->Read();
		if (iter->File() && Parent->filesys.Exists(FileMap::MemoAttach, iter->File()))
		{
		    SEND(mynick, source, "MS_COMMAND/MEMO_FILE", (
			i, iter->Sender(),
			iter->Time().Ago(),
			Parent->filesys.GetName(FileMap::MemoAttach, iter->File()),
			ToHumanSpace(Parent->filesys.GetSize(FileMap::MemoAttach, iter->File()))));
		}
		else
		{
		    SEND(mynick, source, "MS_COMMAND/MEMO", (
			i, iter->Sender(),
			iter->Time().Ago()));
		}
		unsigned int sentsize;
		mstring output;
		for (sentsize = 0; sentsize < iter->Text().size();
				sentsize+=Parent->server.proto.MaxLine())
		{
		    if (sentsize + Parent->server.proto.MaxLine() < iter->Text().size())
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize, sentsize+Parent->server.proto.MaxLine())
			       << "...";
		    }
		    else
		    {
			if (sentsize > 0)
			    output << "...";
			output << iter->Text().SubString(sentsize, iter->Text().size()-1);
		    }
		    ::send(mynick, source, "    " + output);
		    output.erase();
		}
	    }
	}
	else
	{
	    vector<int> numbers = ParseNumbers(what);
	    unsigned int i;
	    int j=1;
	    bool triedabove = false, nonnumeric = false;
	    size_t amt = 0;
	    RLOCK(("MemoServ", "nick", who.LowerCase()));
	    MemoServ::nick_memo_t &memolist = Parent->memoserv.GetNick(who);
	    amt = memolist.size();
	    MemoServ::nick_memo_t::iterator iter = memolist.begin();
	    mstring output;
	    for (i=0; i<numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > static_cast<int>(memolist.size()))
		    triedabove = true;
		else
		{
		    while (numbers[i] < j &&
			iter != memolist.begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j &&
			iter != memolist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != memolist.end())
		    {
			iter->Read();
			if (iter->File() && Parent->filesys.Exists(FileMap::MemoAttach, iter->File()))
			{
			    SEND(mynick, source, "MS_COMMAND/MEMO_FILE", (
				j, iter->Sender(),
				iter->Time().Ago(),
				Parent->filesys.GetName(FileMap::MemoAttach, iter->File()),
				ToHumanSpace(Parent->filesys.GetSize(FileMap::MemoAttach, iter->File()))));
			}
			else
			{
			    SEND(mynick, source, "MS_COMMAND/MEMO", (
				j, iter->Sender(),
				iter->Time().Ago()));
			}
			unsigned int sentsize;
			for (sentsize = 0; sentsize < iter->Text().size();
					sentsize+=Parent->server.proto.MaxLine())
			{
			    if (sentsize + Parent->server.proto.MaxLine() < iter->Text().size())
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize, sentsize+Parent->server.proto.MaxLine())
				    << "...";
			    }
			    else
			    {
				if (sentsize > 0)
				    output << "...";
				output << iter->Text().SubString(sentsize, iter->Text().size()-1);
			    }
			    ::send(mynick, source, "    " + output);
			    output.erase();
			}
		    }
		}
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", ( amt));
	}
    }
}


void MemoServ::do_UnRead(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_UnRead", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.After(" ", 2);
	mstring whoami = Parent->nickserv.GetStored(source).Host().LowerCase();
	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Parent->chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		    who));
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.GetStored(who).GetAccess(whoami, "READMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (
				who.LowerCase()));
	    return;
	}

	Parent->memoserv.stats.i_Unread++;
	if (what.IsSameAs("all", true))
	{
	    MemoServ::channel_news_t::iterator iter;
	    mstring output;
	    RLOCK(("MemoServ", "channel", who.LowerCase()));
	    for (iter = Parent->memoserv.ChannelNewsBegin(who);
		    iter != Parent->memoserv.ChannelNewsEnd(who); iter++)
	    {
		iter->Unread(whoami);
	    }
	    SEND(mynick, source, "MS_COMMAND/UNREAD_ALL", (
				    who.LowerCase()));
	}
	else
	{
	    vector<int> numbers = ParseNumbers(what);
	    unsigned int i;
	    int j=1;
	    bool triedabove = false, nonnumeric = false;
	    size_t amt = 0;
	    RLOCK(("MemoServ", "channel", who.LowerCase()));
	    MemoServ::channel_news_t &newslist = Parent->memoserv.GetChannel(who);
	    amt = newslist.size();
	    MemoServ::channel_news_t::iterator iter = newslist.begin();
	    mstring output;
	    for (i=0; i<numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > static_cast<int>(newslist.size()))
		    triedabove = true;
		else
		{
		    while (numbers[i] < j &&
			iter != newslist.begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j &&
			iter != newslist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != newslist.end())
		    {
			iter->Unread(whoami);
			if (!output.empty())
			    output << ", ";
			output << j;
		    }
		}
	    }
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/CS_UNREAD", (
					    output, who));
		output.erase();
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", ( amt));
	}
    }
    else
    {
	mstring who = Parent->nickserv.GetStored(source).Host();
	mstring what = params.After(" ", 1);

	if (who.empty())
	    who = source;

	if (!Parent->memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	Parent->memoserv.stats.i_Unread++;
	if (what.IsSameAs("all", true))
	{
	    MemoServ::nick_memo_t::iterator iter;
	    mstring output;
	    RLOCK(("MemoServ", "nick", who.LowerCase()));
	    for (iter = Parent->memoserv.NickMemoBegin(who);
		    iter != Parent->memoserv.NickMemoEnd(who); iter++)
	    {
		iter->Unread();
	    }
	    NSEND(mynick, source, "MS_COMMAND/NS_UNREAD_ALL");
	}
	else
	{
	    vector<int> numbers = ParseNumbers(what);
	    unsigned int i;
	    int j=1;
	    bool triedabove = false, nonnumeric = false;
	    size_t amt = 0;
	    RLOCK(("MemoServ", "nick", who.LowerCase()));
	    MemoServ::nick_memo_t &memolist = Parent->memoserv.GetNick(who);
	    amt = memolist.size();
	    MemoServ::nick_memo_t::iterator iter = memolist.begin();
	    mstring output;
	    for (i=0; i<numbers.size(); i++)
	    {
		if (numbers[i] <= 0)
		    nonnumeric = true;
		else if (numbers[i] > static_cast<int>(memolist.size()))
		    triedabove = true;
		else
		{
		    while (numbers[i] < j &&
			iter != memolist.begin())
		    {
			j--;
			iter--;
		    }
		    while (numbers[i] > j &&
			iter != memolist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != memolist.end())
		    {
			iter->Unread();
			if (!output.empty())
			    output << ", ";
			output << j;
		    }
		}
	    }
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/NS_UNREAD", (
					output));
		output.erase();
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", ( amt));
	}
    }
}


void MemoServ::do_Get(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Get", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring who = Parent->nickserv.GetStored(source).Host();
    mstring what = params.After(" ", 1);

    if (who.empty())
	who = source;

    if (!Parent->memoserv.IsNick(who))
    {
	NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	return;
    }

    vector<int> numbers = ParseNumbers(what);
    unsigned int i;
    int j=1;
    bool triedabove = false, nonnumeric = false, nonfiles = false;
    size_t amt = 0;
    RLOCK(("MemoServ", "nick", who.LowerCase()));
    MemoServ::nick_memo_t &memolist = Parent->memoserv.GetNick(who);
    amt = memolist.size();
    MemoServ::nick_memo_t::iterator iter = memolist.begin();
    mstring output;
    Parent->memoserv.stats.i_Get++;
    for (i=0; i<numbers.size(); i++)
    {
	if (numbers[i] <= 0)
	    nonnumeric = true;
	else if (numbers[i] > static_cast<int>(memolist.size()))
	    triedabove = true;
	else
	{
	    while (numbers[i] < j &&
		iter != memolist.begin())
	    {
		j--;
		iter--;
	    }
	    while (numbers[i] > j &&
		iter != memolist.end())
	    {
		j++;
		iter++;
	    }
	    if (iter != memolist.end())
	    {
		unsigned long filenum = iter->File();
		if (!(filenum &&
		    Parent->filesys.Exists(FileMap::MemoAttach, filenum)))
		{
		    nonfiles = true;
		    continue;
		}

		mstring filename = Parent->filesys.GetName(FileMap::MemoAttach, filenum);
		size_t filesize = Parent->filesys.GetSize(FileMap::MemoAttach, filenum);
		if (filename.empty() || filesize <= 0)
		{
		    nonfiles = true;
		    continue;
		}

		if (!(Parent->files.TempDirSize() == 0 ||
		    mFile::DirUsage(Parent->files.TempDir()) <=
		    Parent->files.TempDirSize()))
		{
		    NSEND(mynick, source, "DCC/NOSPACE2");
		    return;
		}

		{ RLOCK(("DCC"));
		if (Parent->dcc != NULL)
		{
		    unsigned short port = mSocket::FindAvailPort();
		    ::privmsg(mynick, source, DccEngine::encode("DCC SEND", filename +
			" " + mstring(Parent->LocalHost()) + " " +
			mstring(port) + " " + mstring(filesize)));
		    Parent->dcc->Accept(port, mynick, source, FileMap::MemoAttach, filenum);
		}}
	    }
	}
    }
    if (nonnumeric)
	NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
    if (triedabove)
	SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", ( amt));
    if (nonfiles)
	NSEND(mynick, source, "ERR_SYNTAX/NONFILES");
 }


void MemoServ::do_List(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_List", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") > 1 && IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 2)
	{
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring whoami = Parent->nickserv.GetStored(source).Host().LowerCase();
	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Parent->chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		    who));
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.GetStored(who).GetAccess(whoami, "READMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (
				who.LowerCase()));
	    return;
	}

	MemoServ::channel_news_t::iterator iter;
	int i = 1;
	mstring output;
	RLOCK(("MemoServ", "channel", who.LowerCase()));
	for (iter = Parent->memoserv.ChannelNewsBegin(who);
		iter != Parent->memoserv.ChannelNewsEnd(who); iter++)
	{
	    output.erase();
	    if (iter->Text().size() > 20)
		output << iter->Text().SubString(0, 19) << "...";
	    else
		output << iter->Text();

	    SEND(mynick, source, "MS_COMMAND/NEWS_LIST", (
		    iter->IsRead(whoami) ? ' ' : '*',
		    i++, iter->Sender(), iter->Channel(),
		    iter->Time().Ago(), output));
	}
    }
    else
    {
	mstring who = Parent->nickserv.GetStored(source).Host();
	if (who.empty())
	    who = source;

	if (!Parent->memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	MemoServ::nick_memo_t::iterator iter;
	int i = 1;
	mstring output;
	RLOCK(("MemoServ", "nick", who.LowerCase()));
	for (iter = Parent->memoserv.NickMemoBegin(who);
		iter != Parent->memoserv.NickMemoEnd(who); iter++)
	{
	    output.erase();
	    if (iter->Text().size() > 20)
		output << iter->Text().SubString(0, 19) << "...";
	    else
		output << iter->Text();

	    if (iter->File() && Parent->filesys.Exists(FileMap::MemoAttach, iter->File()))
	    {
		SEND(mynick, source, "MS_COMMAND/MEMO_LIST_FILE", (
		    iter->IsRead() ? ' ' : '*',
		    i++, iter->Sender(),
		    iter->Time().Ago(), output,
		    Parent->filesys.GetName(FileMap::MemoAttach, iter->File()),
		    ToHumanSpace(Parent->filesys.GetSize(FileMap::MemoAttach, iter->File()))));
	    }
	    else
	    {
		SEND(mynick, source, "MS_COMMAND/MEMO_LIST", (
		    iter->IsRead() ? ' ' : '*',
		    i++, iter->Sender(),
		    iter->Time().Ago(), output));
	    }
	}
    }
}


void MemoServ::do_Send(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Send", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring name = params.ExtractWord(2, " ");
    mstring text = params.After(" ", 2);

    if (IsChan(name))
    {
	if (!Parent->chanserv.IsStored(name))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
			name));
	    return;
	}
	name = Parent->getSname(name);

	if (!Parent->chanserv.GetStored(name).GetAccess(source, "WRITEMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }
    else
    {
	if (!Parent->nickserv.IsStored(name))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
			name));
	    return;
	}
	name = Parent->getSname(name);
	if (Parent->nickserv.GetStored(name).Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
			name));
	    return;
	}

	if (Parent->nickserv.GetStored(name).IsIgnore(source) ?
		!Parent->nickserv.GetStored(name).NoMemo() :
		Parent->nickserv.GetStored(name).NoMemo())
	{
	    SEND(mynick, source, "MS_STATUS/IGNORE", (
			    name));
	    return;
	}
    }

    if (!Parent->nickserv.GetLive(source).HasMode("o") &&
	Parent->nickserv.GetLive(source).LastMemo().SecondsSince() <
    		Parent->memoserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Parent->memoserv.Delay() -
		Parent->nickserv.GetLive(source).LastMemo().SecondsSince(),
		source)));
	return;
    }

    if (text.size() > Parent->server.proto.MaxLine())
    {
	text.Truncate(Parent->server.proto.MaxLine());
	SEND(mynick, source, "MS_STATUS/TRUNCATE", (
			text.SubString(Parent->server.proto.MaxLine()-20,
		    	Parent->server.proto.MaxLine()), mynick));
    }

    Parent->memoserv.stats.i_Send++;
    Parent->nickserv.GetLive(source).InFlight.Memo(
					    false, mynick, name, text);
}


void MemoServ::do_Flush(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Flush", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->nickserv.GetLive(source).InFlight.Memo())
    {
	if (Parent->nickserv.GetLive(source).InFlight.File())
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOFLUSH");
	    return;
	}
	Parent->memoserv.stats.i_Flush++;
	Parent->nickserv.GetLive(source).InFlight.End(0u);
    }
    else
    {
	NSEND(mynick, source, "MS_STATUS/NOPENDING");
    }
}

void MemoServ::do_Forward(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Forward", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 4)
	{
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.ExtractWord(3, " ");
	mstring dest = params.ExtractWord(4, " ");
	mstring whoami = Parent->nickserv.GetStored(source).Host().LowerCase();
	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Parent->chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		    who));
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.GetStored(who).GetAccess(whoami, "READ"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (
				who.LowerCase()));
	    return;
	}

	if (!what.IsNumber() || what.Contains("."))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(what.c_str());
	if (num <= 0 || num > Parent->memoserv.ChannelNewsSize(who))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
			1, Parent->memoserv.ChannelNewsSize(who)));
	    return;
	}

	if (!Parent->nickserv.GetLive(source).HasMode("o") &&
		Parent->nickserv.GetLive(source).LastMemo().SecondsSince() <
    		Parent->memoserv.Delay())
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Parent->memoserv.Delay() -
		Parent->nickserv.GetLive(source).LastMemo().SecondsSince(),
		source)));
	    return;
	}

	unsigned int i;
	mstring output;
	{ RLOCK(("MemoServ", "channel", who.LowerCase()));
	MemoServ::channel_news_t::iterator iter = Parent->memoserv.ChannelNewsEnd(who);
	for (i=1; i < num; iter++, i++) ;
	output = parseMessage(Parent->getMessage(dest, "MS_STATUS/FORWARD_ARG"),
		mVarArray(Parent->chanserv.GetStored(who).Name(),
		iter->Sender(), iter->Text()));
	}

	do_Forward2(mynick, source, dest, output);
    }
    else
    {
	mstring who = Parent->nickserv.GetStored(source).Host();
	mstring what = params.ExtractWord(2, " ");
	mstring dest = params.ExtractWord(3, " ");

	if (who.empty())
	    who = source;

	if (!Parent->memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	if (!what.IsNumber() || what.Contains("."))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(what.c_str());
	if (num <= 0 || num > Parent->memoserv.NickMemoSize(who))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
			1, Parent->memoserv.NickMemoSize(who)));
	    return;
	}

	if (!Parent->nickserv.GetLive(source).HasMode("o") &&
		Parent->nickserv.GetLive(source).LastMemo().SecondsSince() <
    		Parent->memoserv.Delay())
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Parent->memoserv.Delay() -
		Parent->nickserv.GetLive(source).LastMemo().SecondsSince(),
		source)));
	    return;
	}

	unsigned int i;
	mstring output;
	{ RLOCK(("MemoServ", "nick", who.LowerCase()));
	MemoServ::nick_memo_t::iterator iter = Parent->memoserv.NickMemoBegin(who);
	for (i=1; i < num; iter++, i++) ;

	if (iter->File())
	    output = parseMessage(Parent->getMessage(dest, "MS_STATUS/FORWARD_ARG"),
		mVarArray(Parent->filesys.GetName(FileMap::MemoAttach, iter->File()),
		iter->Sender(), iter->Text()));
	else
	    output = parseMessage(Parent->getMessage(dest, "MS_STATUS/FORWARD"),
		mVarArray(iter->Sender(), iter->Text()));
	}

	do_Forward2(mynick, source, dest, output);
    }
}


void MemoServ::do_Forward2(const mstring &mynick, const mstring &source,
		mstring dest, const mstring &text)
{
    FT("MemoServ::do_Forward2", (mynick, source, dest, text));

    if (IsChan(dest))
    {
	if (!Parent->chanserv.IsStored(dest))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
			dest));
	    return;
	}
	dest = Parent->getSname(dest);

	if (!Parent->chanserv.GetStored(dest).GetAccess(source, "WRITEMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}
    }
    else
    {
	if (!Parent->nickserv.IsStored(dest))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
			dest));
	    return;
	}
	dest = Parent->getSname(dest);
	if (Parent->nickserv.GetStored(dest).Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
			dest));
	    return;
	}

	if (Parent->nickserv.GetStored(dest).IsIgnore(source) ?
		!Parent->nickserv.GetStored(dest).NoMemo() :
		Parent->nickserv.GetStored(dest).NoMemo())
	{
	    SEND(mynick, source, "MS_STATUS/IGNORE", (
			    dest));
	    return;
	}
    }

    Parent->memoserv.stats.i_Forward++;
    Parent->nickserv.GetLive(source).InFlight.Memo(
					false, mynick, dest, text, true);
    Parent->nickserv.GetLive(source).InFlight.End(0);
}


void MemoServ::do_Reply(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Reply", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring output, recipiant;
    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 4)
	{
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.ExtractWord(3, " ");
	mstring text = params.After(" ", 3);
	mstring whoami = Parent->nickserv.GetStored(source).Host().LowerCase();
	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Parent->chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		    who));
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.GetStored(who).GetAccess(whoami, "READMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (
				who.LowerCase()));
	    return;
	}

	if (!what.IsNumber() || what.Contains("."))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(what.c_str());
	if (num <= 0 || num > Parent->memoserv.ChannelNewsSize(who))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
			1, Parent->memoserv.ChannelNewsSize(who)));
	    return;
	}

	if (!Parent->nickserv.GetLive(source).HasMode("o") &&
		Parent->nickserv.GetLive(source).LastMemo().SecondsSince() <
    		Parent->memoserv.Delay())
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Parent->memoserv.Delay() -
		Parent->nickserv.GetLive(source).LastMemo().SecondsSince(),
		source)));
	    return;
	}

	unsigned int i;
	{ RLOCK(("MemoServ", "channel", who.LowerCase()));
	MemoServ::channel_news_t::iterator iter = Parent->memoserv.ChannelNewsBegin(who);
	for (i=1; i < num; iter++, i++) ;

	recipiant = iter->Sender();
	output = parseMessage(Parent->getMessage("MS_STATUS/REPLY_ARG"),
		mVarArray(who,
		(iter->Text().size() > 20) ?
		    (iter->Text().SubString(0, 19) + "...") :
		    iter->Text(),
		text));
	}
    }
    else
    {
	mstring who = Parent->nickserv.GetStored(source).Host();
	mstring what = params.ExtractWord(2, " ");
	mstring text = params.After(" ", 2);

	if (who.empty())
	    who = source;

	if (!Parent->memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}
	who = Parent->getSname(who);

	if (!what.IsNumber() || what.Contains("."))
	{
	    NSEND(mynick, source, "ERR_SYNTAX/WHOLENUMBER");
	    return;
	}

	unsigned int num = atoi(what.c_str());
	if (num <= 0 || num > Parent->memoserv.NickMemoSize(who))
	{
	    SEND(mynick, source, "ERR_SYNTAX/MUSTBENUMBER", (
			1, Parent->memoserv.NickMemoSize(who)));
	    return;
	}

	if (!Parent->nickserv.GetLive(source).HasMode("o") &&
		Parent->nickserv.GetLive(source).LastMemo().SecondsSince() <
    		Parent->memoserv.Delay())
	{
	    SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Parent->memoserv.Delay() -
		Parent->nickserv.GetLive(source).LastMemo().SecondsSince(),
		source)));
	    return;
	}

	unsigned int i;
	{ RLOCK(("MemoServ", "nick", who.LowerCase()));
	MemoServ::nick_memo_t::iterator iter = Parent->memoserv.NickMemoBegin(who);
	for (i=1; i < num; iter++, i++) ;
	if (iter->File())
	    output = parseMessage(Parent->getMessage("MS_STATUS/REPLY_ARG"),
		mVarArray(Parent->filesys.GetName(FileMap::MemoAttach, iter->File()),
		(iter->Text().length() > 20) ?
		    (iter->Text().SubString(0, 19) + "...") :
		    iter->Text(),
		text));
	else
	    output = parseMessage(Parent->getMessage("MS_STATUS/REPLY"),
		mVarArray((iter->Text().length() > 20) ?
		    (iter->Text().SubString(0, 19) + "...") :
		    iter->Text(),
		text));

	}
    }

    if (!Parent->nickserv.IsStored(recipiant))
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
		    Parent->getSname(recipiant)));
	return;
    }
    if (Parent->nickserv.GetStored(recipiant).Forbidden())
    {
	SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
			recipiant));
	return;
    }
    if (Parent->nickserv.GetStored(recipiant).IsIgnore(source) ?
		!Parent->nickserv.GetStored(recipiant).NoMemo() :
		Parent->nickserv.GetStored(recipiant).NoMemo())
    {
	SEND(mynick, source, "MS_STATUS/IGNORE", (recipiant));
	return;
    }
    Parent->memoserv.stats.i_Reply++;
    Parent->nickserv.GetLive(source).InFlight.Memo(
				false, mynick, recipiant, output);

}


void MemoServ::do_Cancel(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Cancel", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->nickserv.GetLive(source).InFlight.Memo())
    {
	Parent->memoserv.stats.i_Cancel++;
	if (Parent->nickserv.GetLive(source).InFlight.File())
	    NSEND(mynick, source, "MS_COMMAND/CANCEL");
	    
	Parent->nickserv.GetLive(source).InFlight.Cancel();
    }
    else
    {
	NSEND(mynick, source, "MS_STATUS/NOPENDING");
    }
}


void MemoServ::do_Del(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Del", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	if (params.WordCount(" ") < 3)
	{
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring what = params.After(" ", 2);
	mstring whoami = Parent->nickserv.GetStored(source).Host().LowerCase();
	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Parent->chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		    who));
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.GetStored(who).GetAccess(whoami, "WRITEMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (
				who.LowerCase()));
	    return;
	}

	Parent->memoserv.stats.i_Del++;
	if (what.IsSameAs("all", true))
	{
	    if (!Parent->chanserv.GetStored(who).GetAccess(whoami, "DELMEMO"))
	    {
		NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
		return;
	    }

	    Parent->memoserv.RemChannel(who);
	    SEND(mynick, source, "MS_COMMAND/CS_DEL_ALL", (
				    who));
	    LOG(LM_DEBUG, "MEMOSERV/DEL_ALL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		who));
	}
	else
	{
	    unsigned int i;
	    /* Adjust is to accomodate for memos that have
	     * been deleted before the current one, ie. if
	     * 1, 3, 5 and 7 are being deleted, once 1 has
	     * been deleted, adjust becomes 1, meaning that
	     * 3 has become 2, 5 has become 4, etc.  Once 3
	     * has been deleted, adjust becomes 2, meaning
	     * that 5 has become 3 and 7 has become 5.
	     */
	    int j=1, adjust=0;
	    vector<int> numbers1 = ParseNumbers(what);
	    set<int> numbers;
	    set<int>::iterator ni;
	    for (i=0; i<numbers1.size(); i++)
		numbers.insert(numbers1[i]);

	    bool triedabove = false, nonnumeric = false;
	    mstring output, denied;
	    size_t amt = 0;
	    { WLOCK(("MemoServ", "channel", who.LowerCase()));
	    MemoServ::channel_news_t &newslist = Parent->memoserv.GetChannel(who);
	    amt = newslist.size();
	    MemoServ::channel_news_t::iterator iter = newslist.begin();
	    for (ni = numbers.begin(); ni != numbers.end(); ni++)
	    {
		if (*ni - adjust <= 0)
		    nonnumeric = true;
		else if (*ni - adjust > static_cast<int>(newslist.size()))
		    triedabove = true;
		else
		{
		    while (*ni - adjust > j &&
			iter != newslist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != newslist.end())
		    {
			if (!Parent->chanserv.GetStored(who).GetAccess(whoami, "DELMEMO") &&
			    iter->Sender().LowerCase() != who.LowerCase())
			{
			    if (!denied.empty())
				denied << ", ";
			    denied << *ni;
			}
			else
			{
			    if (!output.empty())
				output << ", ";
			    output << *ni;
			    Parent->memoserv.RemChannelNews(who, j - 1);
			    if (adjust + 2 == static_cast<int>(amt))
			    {
				if (numbers.size() > amt)
				    triedabove = true;
				break;
			    }
			    iter = newslist.begin();
			    j=1;
			    adjust++;
			}
		    }
		}
	    }}
	    if (!denied.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/CS_NOTDEL", (
			    denied, who));
		denied.erase();
	    }
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/CS_DEL", (
			    output, who));
		output.erase();
		LOG(LM_DEBUG, "MEMOSERV/DEL", (
			Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
			adjust, who));
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", ( amt));
	}
    }
    else
    {
	mstring who = Parent->nickserv.GetStored(source).Host();
	mstring what = params.After(" ", 1);

	if (who.empty())
	    who = source;

	if (!Parent->memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	Parent->memoserv.stats.i_Del++;
	if (what.IsSameAs("all", true))
	{
	    MemoServ::nick_memo_t::iterator iter;
	    { RLOCK(("MemoServ", "nick", who.LowerCase()));
	    for (iter = Parent->memoserv.NickMemoBegin(who);
		    iter != Parent->memoserv.NickMemoEnd(who);iter++)
	    {
		if (iter->File())
		    Parent->filesys.EraseFile(FileMap::MemoAttach, iter->File());
	    }}
	    Parent->memoserv.RemNick(who);
	    NSEND(mynick, source, "MS_COMMAND/NS_DEL_ALL");
	}
	else
	{
	    unsigned int i;
	    /* Adjust is to accomodate for memos that have
	     * been deleted before the current one, ie. if
	     * 1, 3, 5 and 7 are being deleted, once 1 has
	     * been deleted, adjust becomes 1, meaning that
	     * 3 has become 2, 5 has become 4, etc.  Once 3
	     * has been deleted, adjust becomes 2, meaning
	     * that 5 has become 3 and 7 has become 5.
	     */
	    int j=1, adjust = 0;
	    vector<int> numbers1 = ParseNumbers(what);
	    set<int> numbers;
	    set<int>::iterator ni;
	    for (i=0; i<numbers1.size(); i++)
		numbers.insert(numbers1[i]);
	    bool triedabove = false, nonnumeric = false;
	    mstring output;
	    size_t amt = 0;
	    { WLOCK(("MemoServ", "nick", who.LowerCase()));
	    MemoServ::nick_memo_t &memolist = Parent->memoserv.GetNick(who);
	    amt = memolist.size();
	    MemoServ::nick_memo_t::iterator iter = memolist.begin();
	    for (ni = numbers.begin(); ni != numbers.end(); ni++)
	    {
		if (*ni - adjust <= 0)
		    nonnumeric = true;
		else if (*ni - adjust > static_cast<int>(memolist.size()))
		    triedabove = true;
		else
		{
		    while (*ni - adjust > j &&
			iter != memolist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != memolist.end())
		    {
			if (!output.empty())
			    output << ", ";
			output << *ni;
			if (iter->File())
			    Parent->filesys.EraseFile(FileMap::MemoAttach, iter->File());
			Parent->memoserv.RemNickMemo(who, j - 1);
			if (adjust + 2 == static_cast<int>(amt))
			{
			    if (numbers.size() > amt)
				triedabove = true;
			    break;
			}
			iter = memolist.begin();
			j=1;
			adjust++;
		    }
		}
	    }}
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/NS_DEL", (
			    output));
		output.erase();
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", ( amt));
	}
    }
}


void MemoServ::do_Continue(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Continue", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring text = params.After(" ", 1);

    if (text.size() > Parent->server.proto.MaxLine())
    {
	text.Truncate(Parent->server.proto.MaxLine());
	SEND(mynick, source, "MS_STATUS/TRUNCATE", (
			text.SubString(Parent->server.proto.MaxLine()-20,
			Parent->server.proto.MaxLine()-1), mynick));
    }

    if (Parent->nickserv.GetLive(source).InFlight.Memo())
    {
	Parent->memoserv.stats.i_Continue++;
	Parent->nickserv.GetLive(source).InFlight.Continue(text);
    }
    else
    {
	NSEND(mynick, source, "MS_STATUS/NOPENDING");
    }
}


void MemoServ::do_Preview(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_Continue", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    if (Parent->nickserv.GetLive(source).InFlight.Memo())
    {
	mstring text(Parent->nickserv.GetLive(source).InFlight.Text());
	size_t shown = 0, length = text.length();

	SEND(mynick, source, "MS_COMMAND/PREVIEW", (
		Parent->nickserv.GetLive(source).InFlight.Recipiant()));
	while (shown < length)
	{
	    if (length-shown > Parent->server.proto.MaxLine())
	    {
		::send(mynick, source, (shown ? "    ..." : "    ") +
			text.SubString(shown,
			Parent->server.proto.MaxLine()-1) + "...");
		shown += Parent->server.proto.MaxLine();
	    }
	    else
	    {
		::send(mynick, source, (shown ? "    ..." : "    ") +
			text.SubString(shown, length-1));
		shown += length-shown;
	    }
	}
    }
    else
    {
	NSEND(mynick, source, "MS_STATUS/NOPENDING");
    }
}


void MemoServ::do_File(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_File", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    mstring name = params.ExtractWord(2, " ");
    mstring text;
    if (params.WordCount(" ") > 2)
	text = params.After(" ", 2);

    if (IsChan(name))
    {
	NSEND(mynick, source, "ERR_SITUATION/NOATTACH");
	return;
    }
    else
    {
	if (!Parent->nickserv.IsStored(name))
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISNOTSTORED", (
			name));
	    return;
	}
	name = Parent->getSname(name);
	if (Parent->nickserv.GetStored(name).Forbidden())
	{
	    SEND(mynick, source, "NS_OTH_STATUS/ISFORBIDDEN", (
			name));
	    return;
	}
	mstring target = Parent->nickserv.GetStored(name).Host();
	if (target.empty())
	    target = name;

	if (Parent->memoserv.Files() > 0)
	{
	    if (Parent->memoserv.IsNick(target))
	    {
		unsigned int count = 0;
		MemoServ::nick_memo_t::iterator iter;
		{ RLOCK(("MemoServ", "nick", target.LowerCase()));
		for (iter=Parent->memoserv.NickMemoBegin(target);
			iter!=Parent->memoserv.NickMemoEnd(target);
			iter++)
		{
		    if (iter->File())
			count++;
		}}
		if (count >= Parent->memoserv.Files())
		{
		    SEND(mynick, source, "MS_STATUS/ATTACHLIMIT", (
					name));
		    return;
		}
	    }
	}
	else
	{
	    NSEND(mynick, source, "MS_STATUS/FILEDISABLED");
	    return;
	}

	if (Parent->nickserv.GetStored(name).IsIgnore(source) ?
		!Parent->nickserv.GetStored(name).NoMemo() :
		Parent->nickserv.GetStored(name).NoMemo())
	{
	    SEND(mynick, source, "MS_STATUS/IGNORE", (
			    name));
	    return;
	}
    }

    if (!Parent->nickserv.GetLive(source).HasMode("o") &&
	Parent->nickserv.GetLive(source).LastMemo().SecondsSince() <
    		Parent->memoserv.Delay())
    {
	SEND(mynick, source, "ERR_SITUATION/NOTYET", (
		message, ToHumanTime(Parent->memoserv.Delay() -
		Parent->nickserv.GetLive(source).LastMemo().SecondsSince(),
		source)));
	return;
    }

    if (text.size() > Parent->server.proto.MaxLine())
    {
	text.Truncate(Parent->server.proto.MaxLine());
	SEND(mynick, source, "MS_STATUS/TRUNCATE", (
			text.SubString(Parent->server.proto.MaxLine()-20,
			Parent->server.proto.MaxLine()-1), mynick));
    }

    Parent->memoserv.stats.i_File++;
    Parent->nickserv.GetLive(source).InFlight.Memo(
					    true, mynick, name, text);
}


void MemoServ::do_set_NoExpire(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("MemoServ::do_set_NoExpire", (mynick, source, params));

    mstring message  = params.Before(" ").UpperCase();

    { RLOCK(("IrcSvcHandler"));
    if (Parent->ircsvchandler != NULL &&
	Parent->ircsvchandler->HTM_Level() > 3)
    {
	SEND(mynick, source, "MISC/HTM", (
							message));
	return;
    }}

    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	return;
    }

    if (IsChan(params.ExtractWord(2, " ")))
    {
	message += " " + params.ExtractWord(3, " ").UpperCase();
	if (params.WordCount(" ") < 5)
	{
	    SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
				message, mynick, message));
	    return;
	}
	mstring who = params.ExtractWord(2, " ");
	mstring onoff = params.ExtractWord(4, " ");
	mstring what = params.After(" ", 4);
	mstring whoami = Parent->nickserv.GetStored(source).Host().LowerCase();
	if (whoami.empty())
	    whoami = source.LowerCase();

	if (!Parent->chanserv.IsStored(who))
	{
	    SEND(mynick, source, "CS_STATUS/ISNOTSTORED", (
		    who));
	    return;
	}
	who = Parent->getSname(who);

	if (!Parent->chanserv.GetStored(who).GetAccess(whoami, "WRITEMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	if (!Parent->memoserv.IsChannel(who))
	{
	    SEND(mynick, source, "MS_STATUS/CS_EMPTY", (
				who.LowerCase()));
	    return;
	}

	if (!onoff.IsBool())
	{
	    NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	    return;
	}

	if (!Parent->chanserv.GetStored(who).GetAccess(whoami, "DELMEMO"))
	{
	    NSEND(mynick, source, "ERR_SITUATION/NOACCESS");
	    return;
	}

	Parent->memoserv.stats.i_Set++;
	if (what.IsSameAs("all", true))
	{
	    MemoServ::channel_news_t::iterator iter;
	    mstring output;
	    { WLOCK(("MemoServ", "channel", who.LowerCase()));
	    for (iter = Parent->memoserv.ChannelNewsBegin(who);
		    iter != Parent->memoserv.ChannelNewsEnd(who); iter++)
	    {
		iter->NoExpire(onoff.GetBool());
	    }}
	    SEND(mynick, source, "MS_COMMAND/CS_SET_ALL", (
			Parent->getMessage(source, "MS_STATUS/SET_NOEXPIRE"),
			who, onoff.GetBool() ?
				Parent->getMessage("VALS/ON") :
				Parent->getMessage("VALS/OFF")));

	    LOG(LM_DEBUG, "MEMOSERV/SET_ALL", (
		Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
		Parent->getMessage(source, "MS_STATUS/SET_NOEXPIRE"),
		who, onoff.GetBool() ?
			Parent->getMessage("VALS/ON") :
			Parent->getMessage("VALS/OFF")));
	}
	else
	{
	    unsigned int i;
	    int j=1, count = 0;
	    vector<int> numbers1 = ParseNumbers(what);
	    set<int> numbers;
	    set<int>::iterator ni;
	    for (i=0; i<numbers1.size(); i++)
		numbers.insert(numbers1[i]);

	    bool triedabove = false, nonnumeric = false;
	    mstring output, denied;
	    size_t amt = 0;
	    { WLOCK(("MemoServ", "channel", who.LowerCase()));
	    MemoServ::channel_news_t &newslist = Parent->memoserv.GetChannel(who);
	    amt = newslist.size();
	    MemoServ::channel_news_t::iterator iter = newslist.begin();
	    for (ni = numbers.begin(); ni != numbers.end(); ni++)
	    {
		if (*ni <= 0)
		    nonnumeric = true;
		else if (*ni > static_cast<int>(newslist.size()))
		    triedabove = true;
		else
		{
		    while (*ni > j &&
			iter != newslist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != newslist.end())
		    {
			if (!output.empty())
			    output << ", ";
			output << j;
			iter->NoExpire(onoff.GetBool());
			count++;
		    }
		}
	    }}
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/CS_SET", (
			Parent->getMessage(source, "MS_STATUS/SET_NOEXPIRE"),
			output, who, onoff.GetBool() ?
				Parent->getMessage("VALS/ON") :
				Parent->getMessage("VALS/OFF")));
		output.erase();
		LOG(LM_DEBUG, "MEMOSERV/SET", (
			Parent->nickserv.GetLive(source).Mask(Nick_Live_t::N_U_P_H),
			Parent->getMessage(source, "MS_STATUS/SET_NOEXPIRE"),
			count, who, onoff.GetBool() ?
				Parent->getMessage("VALS/ON") :
				Parent->getMessage("VALS/OFF")));
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", ( amt));
	}
    }
    else
    {
	message += " " + params.ExtractWord(2, " ").UpperCase();
	SEND(mynick, source, "ERR_SITUATION/NOTONANICK", (
		message));

/* No NoExpire setting for nicknames.

	mstring who = Parent->nickserv.GetStored(name).Host();
	mstring onoff = params.ExtractWord(3, " ");
	mstring what = params.After(" ", 3);

	if (who.empty())
	    who = source;

	if (!Parent->memoserv.IsNick(who))
	{
	    NSEND(mynick, source, "MS_STATUS/NS_EMPTY");
	    return;
	}

	if (!onoff.IsBool())
	{
	    NSEND(mynick, source, "ERR_SYNTAX/MUSTBEONOFF");
	    return;
	}

	Parent->memoserv.stats.i_Set++;
	if (what.IsSameAs("all", true))
	{
	    MemoServ::nick_memo_t::iterator iter;
	    mstring output;
	    { WLOCK(("MemoServ", "nick", who.LowerCase()));
	    for (iter = Parent->memoserv.NickMemoBegin(who);
		    iter != Parent->memoserv.NickMemoEnd(who); iter++)
	    {
		iter->NoExpire(onoff.GetBool());
	    }}
	    SEND(mynick, source, "MS_COMMAND/NS_SET_ALL", (
		Parent->getMessage(source, "MS_STATUS/SET_NOEXPIRE"),
		onoff.GetBool() ?
			Parent->getMessage("VALS/ON") :
			Parent->getMessage("VALS/OFF")));
	}
	else
	{
	    unsigned int i;
	    int j=1, count = 0;
	    vector<int> numbers1 = ParseNumbers(what);
	    set<int> numbers;
	    set<int>::iterator ni;
	    for (i=0; i<numbers1.size(); i++)
		numbers.insert(numbers1[i]);
	    bool triedabove = false, nonnumeric = false;
	    mstring output;
	    size_t amt = 0;
	    { WLOCK(("MemoServ", "nick", who.LowerCase()));
	    MemoServ::nick_memo_t &memolist = Parent->memoserv.GetNick(who);
	    amt = memolist.size();
	    nick_memo_t::iterator iter = memolist.begin();
	    for (ni = numbers.begin(); ni != numbers.end(); ni++)
	    {
		if (*ni <= 0)
		    nonnumeric = true;
		else if (*ni > static_cast<int>(memolist.size()))
		    triedabove = true;
		else
		{
		    while (*ni > j &&
			iter != memolist.end())
		    {
			j++;
			iter++;
		    }
		    if (iter != memolist.end())
		    {
			if (!output.empty())
			    output << ", ";
			output << j;
			iter->NoExpire(onoff.GetBool());
			count++;
		    }
		}
	    }}
	    if (!output.empty())
	    {
		SEND(mynick, source, "MS_COMMAND/NS_SET", (
			Parent->getMessage(source, "MS_STATUS/SET_NOEXPIRE"),
			output, onoff.GetBool() ?
				Parent->getMessage("VALS/ON") :
				Parent->getMessage("VALS/OFF")));
		output.erase();
	    }
	    if (nonnumeric)
		NSEND(mynick, source, "ERR_SYNTAX/NONNUMERIC");
	    if (triedabove)
		SEND(mynick, source, "ERR_SYNTAX/ABOVELIMIT", ( amt));
	}
*/
    }
}


SXP::Tag Memo_t::tag_Memo_t("Memo_t");
SXP::Tag Memo_t::tag_Nick("Nick");
SXP::Tag Memo_t::tag_Sender("Sender");
SXP::Tag Memo_t::tag_Time("Time");
SXP::Tag Memo_t::tag_Text("Text");
SXP::Tag Memo_t::tag_Read("Read");
SXP::Tag Memo_t::tag_File("File");
SXP::Tag Memo_t::tag_UserDef("UserDef");
SXP::Tag News_t::tag_News_t("News_t");
SXP::Tag News_t::tag_Channel("Channel");
SXP::Tag News_t::tag_Sender("Sender");
SXP::Tag News_t::tag_Time("Time");
SXP::Tag News_t::tag_Text("Text");
SXP::Tag News_t::tag_set_NoExpire("SET_NoExpire");
SXP::Tag News_t::tag_Read("Read");
SXP::Tag News_t::tag_UserDef("UserDef");

void Memo_t::BeginElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    FT("Memo_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if( pElement->IsA(tag_UserDef) )
    {
	mstring *tmp = new mstring;
	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void Memo_t::EndElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    FT("Memo_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
	if( pElement->IsA(tag_Nick) )		pElement->Retrieve(i_Nick);
	if( pElement->IsA(tag_Sender) )		pElement->Retrieve(i_Sender);
	if( pElement->IsA(tag_Time) )		pElement->Retrieve(i_Time);
	if( pElement->IsA(tag_Text) )		pElement->Retrieve(i_Text);
	if( pElement->IsA(tag_Read) )		pElement->Retrieve(i_Read);
	if( pElement->IsA(tag_File) )		pElement->Retrieve(i_File);
}

void Memo_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("Memo_t::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    //TODO: Add your source code here
	pOut->BeginObject(tag_Memo_t);

	WLOCK(("MemoServ", "nick", i_Nick.LowerCase()));
	pOut->WriteElement(tag_Nick, i_Nick);
	pOut->WriteElement(tag_Sender, i_Sender);
	pOut->WriteElement(tag_Time, i_Time);
	pOut->WriteElement(tag_Text, i_Text);
	pOut->WriteElement(tag_Read, i_Read);
	pOut->WriteElement(tag_File, i_File);

        map<mstring,mstring>::const_iterator iter;
        for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
        {
            pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
        }
	pOut->EndObject(tag_Memo_t);
}

void News_t::BeginElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    FT("News_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if( pElement->IsA(tag_UserDef) )
    {
	mstring *tmp = new mstring;
	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void News_t::EndElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    FT("News_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
	if( pElement->IsA(tag_Channel) )	pElement->Retrieve(i_Channel);
	if( pElement->IsA(tag_Sender) )		pElement->Retrieve(i_Sender);
	if( pElement->IsA(tag_Time) )		pElement->Retrieve(i_Time);
	if( pElement->IsA(tag_Text) )		pElement->Retrieve(i_Text);
	if( pElement->IsA(tag_set_NoExpire) )	pElement->Retrieve(i_NoExpire);

    if( pElement->IsA(tag_Read) )
    {
	mstring tmp;
	pElement->Retrieve(tmp);
	i_Read.insert(tmp);
    }
}

void News_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("News_t::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    //TODO: Add your source code here
	pOut->BeginObject(tag_News_t);

	WLOCK(("MemoServ", "channel", i_Channel.LowerCase()));
	pOut->WriteElement(tag_Channel, i_Channel);
	pOut->WriteElement(tag_Sender, i_Sender);
	pOut->WriteElement(tag_Time, i_Time);
	pOut->WriteElement(tag_Text, i_Text);
	pOut->WriteElement(tag_set_NoExpire, i_NoExpire);

	set<mstring>::iterator iter2;
	for (iter2=i_Read.begin(); iter2!=i_Read.end(); iter2++)
	{
		pOut->WriteElement(tag_Read, *iter2);
	}

        map<mstring,mstring>::const_iterator iter;
        for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
        {
            pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
        }
	pOut->EndObject(tag_News_t);
}

SXP::Tag MemoServ::tag_MemoServ("MemoServ");

void MemoServ::BeginElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    FT("MemoServ::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    Memo_t *m = new Memo_t;
    if( pElement->IsA( m->GetClassTag() ) )
    {
	m_array.push_back(m);
	pIn->ReadTo(m);
    }
    else
    {
	delete m;
    }

    News_t *n = new News_t;
    if( pElement->IsA( n->GetClassTag() ) )
    {
	n_array.push_back(n);
	pIn->ReadTo(n);
    }
    else
    {
	delete n;
    }
}

void MemoServ::EndElement(const SXP::IParser * pIn, const SXP::IElement * pElement)
{
    FT("MemoServ::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void MemoServ::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("MemoServ::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_MemoServ);

    MemoServ::nick_t::iterator i1;
    MemoServ::nick_memo_t::iterator i2;
    MemoServ::channel_t::iterator j1;
    MemoServ::channel_news_t::iterator j2;

    { RLOCK(("MemoServ", "nick"));
    for (i1 = NickBegin(); i1 != NickEnd(); i1++)
    {
	RLOCK2(("MemoServ", "nick", i1->first));
	for (i2=i1->second.begin(); i2!=i1->second.end(); i2++)
	{
	    pOut->WriteSubElement(&(*i2));
	}
    }}

    { RLOCK(("MemoServ", "channel"));
    for (j1 = ChannelBegin(); j1 != ChannelEnd(); j1++)
    {
	RLOCK2(("MemoServ", "channel", j1->first));
	for (j2=j1->second.begin(); j2!=j1->second.end(); j2++)
	{
	    pOut->WriteSubElement(&(*j2));
	}
    }}

    pOut->EndObject(tag_MemoServ);
}


void MemoServ::PostLoad()
{
    NFT("MemoServ::PostLoad");
    // Linkage, etc

    unsigned int i, j;
    for (i=0; i<m_array.size(); i++)
    {
	if (m_array[i] != NULL)
	{
	    for (j=0; j<m_array[i]->ud_array.size(); j++)
	    {
		if (m_array[i]->ud_array[j] != NULL)
		{
		    if (m_array[i]->ud_array[j]->Contains("\n"))
			m_array[i]->i_UserDef[m_array[i]->ud_array[j]->Before("\n")] =
				m_array[i]->ud_array[j]->After("\n");
		    delete m_array[i]->ud_array[j];
		}
	    }
	    m_array[i]->ud_array.clear();
	    if (!m_array[i]->Nick().empty())
		AddNickMemo(m_array[i]);
	    delete m_array[i];
	}
    }
    m_array.clear();

    for (i=0; i<n_array.size(); i++)
    {
	if (n_array[i] != NULL)
	{
	    for (j=0; j<n_array[i]->ud_array.size(); j++)
	    {
		if (n_array[i]->ud_array[j] != NULL)
		{
		    if (n_array[i]->ud_array[j]->Contains("\n"))
			n_array[i]->i_UserDef[n_array[i]->ud_array[j]->Before("\n")] =
				n_array[i]->ud_array[j]->After("\n");
		    delete n_array[i]->ud_array[j];
		}
	    }
	    n_array[i]->ud_array.clear();
	    if (!n_array[i]->Channel().empty())
		AddChannelNews(n_array[i]);
	    delete n_array[i];
	}
    }
    n_array.clear();

}
