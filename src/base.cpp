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
#define RCSID(x,y) const char *rcsid_base_cpp_ ## x () { return y; }
RCSID(base_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.168  2001/06/11 06:47:49  prez
** Fixed an undefined symbol
**
** Revision 1.167  2001/06/11 03:44:45  prez
** Re-wrote how burst works, and made the burst message a lower priority
** than normal.  Also removed the chance of a stray pointer being picked
** up in the dependancy system.
**
** Revision 1.166  2001/05/28 11:17:33  prez
** Added some more anti-deadlock stuff, and fixed nick ident warnings
**
** Revision 1.165  2001/05/25 01:59:31  prez
** Changed messaging system ...
**
** Revision 1.164  2001/05/17 19:18:53  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.163  2001/05/13 00:55:18  prez
** More patches to try and fix deadlocking ...
**
** Revision 1.162  2001/05/06 18:44:25  prez
** Fixed problem with kill/rejoin
**
** Revision 1.161  2001/05/06 03:03:07  prez
** Changed all language sends to use $ style tokens too (aswell as logs), so we're
** now standard.  most ::send calls are now SEND and NSEND.  ::announce has also
** been changed to ANNOUNCE and NANNOUNCE.  All language files modified already.
** Also added example lng and lfo file, so you can see the context of each line.
**
** Revision 1.160  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.159  2001/05/03 22:34:35  prez
** Fixed SQUIT protection ...
**
** Revision 1.158  2001/05/03 04:40:17  prez
** Fixed locking mechanism (now use recursive mutexes) ...
** Also now have a deadlock/nonprocessing detection mechanism.
**
** Revision 1.157  2001/05/02 02:35:26  prez
** Fixed dependancy system, and removed printf's - we no longer coredump on
** a 1000 user network.  As a bonus, we actually synd perfectly ;P
**
** Revision 1.156  2001/05/01 14:00:22  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.155  2001/04/09 07:52:22  prez
** Fixed /nickserv.  Fixed cordump in nick expiry.  Fixed slight bugs in mstring.
**
** Revision 1.154  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.153  2001/03/27 07:04:31  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.152  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.151  2001/03/08 08:07:40  ungod
** fixes for bcc 5.5
**
** Revision 1.150  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.149  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.148  2001/02/03 02:21:32  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.147  2001/01/15 23:31:38  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.146  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.145  2000/12/29 22:00:17  prez
** Some changes to the message dequeing (using release now
**
** Revision 1.144  2000/12/29 15:31:55  prez
** Added locking/checking for dcc/events threads.  Also for ACE_Log_Msg
**
** Revision 1.143  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.142  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.141  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.140  2000/12/09 11:24:25  prez
** Changed all privmsg/notice/send/announce calls to have pszFormat
** as a char *, to stop warnings from va_start.
**
** Revision 1.139  2000/10/10 11:47:50  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.138  2000/10/04 10:52:08  prez
** Fixed the memory pool and removed printf's.
**
** Revision 1.137  2000/10/04 07:39:45  prez
** Added MemCluster to speed up lockable, but it cores when we start
** getting real messages -- seemingly in an alloc in the events.
** Lots of printf's left in for debugging (so run as ./magick >output)
**
** Revision 1.136  2000/09/18 08:17:57  prez
** Intergrated mpatrol into the xml/des sublibs, and did
** some minor fixes as a result of mpatrol.
**
** Revision 1.135  2000/09/13 12:45:33  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.134  2000/09/02 07:20:44  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.133  2000/09/01 10:54:38  prez
** Added Changing and implemented Modify tracing, now just need to create
** DumpB() and DumpE() functions in all classes, and put MCB() / MCE() calls
** (or MB() / ME() or CB() / CE() where MCB() / MCE() not appropriate) in.
**
** Revision 1.132  2000/08/31 06:25:08  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.131  2000/08/28 10:51:35  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.130  2000/08/22 08:43:39  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.129  2000/08/19 10:59:46  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.128  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.127  2000/06/25 07:58:48  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.126  2000/06/21 09:00:05  prez
** Fixed bug in mFile
**
** Revision 1.125  2000/06/18 13:31:47  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.124  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.123  2000/06/12 08:15:36  prez
** Added 'minimum threads' option to config (set to 2)
**
** Revision 1.122  2000/06/12 06:07:50  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.121  2000/06/11 08:20:11  prez
** More minor bug fixes, godda love testers.
**
** Revision 1.120  2000/06/06 08:57:54  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.119  2000/05/25 08:16:38  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.118  2000/05/21 04:49:39  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.117  2000/05/20 03:28:11  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.116  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.115  2000/05/13 06:48:23  ungod
** no message
**
** Revision 1.114  2000/05/03 14:12:22  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.113  2000/05/01 03:11:40  ungod
** xmlisation of entlist_t done
**
** Revision 1.112  2000/04/15 11:11:44  ungod
** starting xmlage of magick
**
** Revision 1.111  2000/04/02 07:25:05  prez
** Fixed low watermarks with threads, it all works now!
**
** Revision 1.110  2000/03/29 14:03:00  prez
** Fixed the failure to create thread condition ...
**
** Revision 1.109  2000/03/29 14:00:18  prez
** Fixed the thread pool system, and the watermarks.
**
** Revision 1.108  2000/03/29 09:41:17  prez
** Attempting to fix thread problem with mBase, and added notification
** of new memos on join of channel or signon to network.
**
** Revision 1.107  2000/03/28 16:20:57  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.106  2000/03/27 10:40:11  prez
** Started implementing revenge
**
** Revision 1.105  2000/03/24 15:35:17  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.104  2000/03/19 08:50:53  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.103  2000/03/14 13:36:46  prez
** Finished P12 compliance (SJOIN, etc).
**
** Revision 1.102  2000/03/14 10:05:16  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.101  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.100  2000/03/02 11:59:45  prez
** More helpfile updates (slowly but surely writing it)
**
** Revision 1.99  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.98  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.97  2000/02/17 12:55:04  ungod
** still working on borlandization
**
** Revision 1.96  2000/02/16 12:59:38  ungod
** fixing for borland compilability
**
** Revision 1.95  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.94  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"

bool mBase::TaskOpened;
map<mMessage::type_t, map<mstring, set<unsigned long> > > mMessage::AllDependancies;
map<unsigned long, mMessage *> mMessage::MsgIdMap;
unsigned long mMessage::LastMsgId = 0;


void entlist_t::operator=(const entlist_t &in)
{
    FT("entlist_t::operator=", ("(const entlist_t &) in"));
    i_Entry=in.i_Entry;
    i_Last_Modify_Time=in.i_Last_Modify_Time;
    i_Last_Modifier=in.i_Last_Modifier;
    map<mstring,mstring>::const_iterator i;
    i_UserDef.clear();
    for(i=in.i_UserDef.begin();i!=in.i_UserDef.end();i++)
	i_UserDef[i->first]=i->second;
}


SXP::Tag tag_entlist_t("entlist_t");
SXP::Tag tag_entlist_val_t("entlist_val_t");
SXP::Tag tag_Entry("Entry");
SXP::Tag tag_Value("Value");
SXP::Tag tag_ValueFirst("ValueFirst");
SXP::Tag tag_ValueSecond("ValueSecond");
SXP::Tag tag_Last_Modify_Time("Last_Modify_Time");
SXP::Tag tag_Last_Modifier("Last_Modifier");
SXP::Tag tag_UserDef("UserDef");
SXP::Tag tag_Stupid("Stupid");

void entlist_t::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("entlist_t::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));

    if( pElement->IsA(tag_UserDef) )
    {
	mstring *tmp = new mstring;
	ud_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void entlist_t::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("entlist_t::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    //TODO: Add your source code here
	if( pElement->IsA(tag_Entry) )   pElement->Retrieve(i_Entry);
	if( pElement->IsA(tag_Last_Modify_Time) )   pElement->Retrieve(i_Last_Modify_Time);
	if( pElement->IsA(tag_Last_Modifier) )   pElement->Retrieve(i_Last_Modifier);
}

void entlist_t::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("entlist_t::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    //TODO: Add your source code here
	pOut->BeginObject(tag_entlist_t);

	pOut->WriteElement(tag_Entry, i_Entry);
	pOut->WriteElement(tag_Last_Modify_Time, i_Last_Modify_Time);
	pOut->WriteElement(tag_Last_Modifier, i_Last_Modifier);

    map<mstring,mstring>::const_iterator iter;
    for(iter=i_UserDef.begin();iter!=i_UserDef.end();iter++)
    {
        pOut->WriteElement(tag_UserDef,iter->first+"\n"+iter->second);
    }

	pOut->EndObject(tag_entlist_t);
}

void entlist_t::PostLoad() const
{
    NFT("entlist_t::PostLoad");

    unsigned int j;
    for (j=0; j<ud_array.size(); j++)
    {
	if (ud_array[j] != NULL)
	{
	    if (ud_array[j]->Contains("\n"))
		i_UserDef[ud_array[j]->Before("\n")] =
			ud_array[j]->After("\n");
	    delete ud_array[j];
	}
    }
    ud_array.clear();
}

size_t entlist_t::Usage() const
{
    size_t retval = 0;
    retval += i_Entry.capacity();
    retval += i_Last_Modifier.capacity();
    retval += sizeof(i_Last_Modify_Time.Internal());
    map<mstring,mstring>::const_iterator i;
    for (i=i_UserDef.begin(); i!=i_UserDef.end(); i++)
    {
	retval += i->first.capacity();
	retval += i->second.capacity();
    }
    return retval;
}

void entlist_t::DumpB() const
{
    MB(0, (i_Entry, i_Last_Modifier, i_Last_Modify_Time, i_UserDef.size()));
}

void entlist_t::DumpE() const
{
    ME(0, (i_Entry, i_Last_Modifier, i_Last_Modify_Time, i_UserDef.size()));
}


// --------- end of entlist_t -----------------------------------

mMessage::mMessage(const mstring& source, const mstring& msgtype, const mstring& params, const u_long priority)
	: ACE_Method_Request(priority), source_(source), params_(params),
	  creation_(mDateTime::CurrentDateTime())
{
    if (source != " " && Parent->server.proto.Tokens())
    {
	mstring tmp(Parent->server.proto.GetToken(msgtype));
	if (!tmp.empty())
	    msgtype_ = tmp;
    }

    if (msgtype_.empty())
	msgtype_ = msgtype;
}

void mMessage::AddDependancies()
{
    NFT("mMessage::AddDependancies");
    WLOCK(("Dependancies", this));
    bool added = false;

    if (!source_.empty())
    {
	if (source_[0u] == '@')
	{
	    mstring svr(str_to_base64(source_.After("@")));
	    AddDepend(ServerExists, svr);
	}
	else if (source_.Contains("."))
	{
	    AddDepend(ServerExists, source_.LowerCase());
	}
	else
	{
	    AddDepend(NickExists, source_.LowerCase());
	}
    }

    if (msgtype_ == "JOIN")
    {
	// User is NOT in channel
	for (unsigned int i=1; i<=params_.WordCount(":, "); i++)
	    AddDepend(UserNoInChan, params_.ExtractWord(i, ":, ").LowerCase() + ":" + source_.LowerCase());
    }
    else if (msgtype_ == "KICK")
    {
	// Channel exists
	// Target exists
	// Target in channel
	AddDepend(ChanExists, params_.ExtractWord(1, ": ").LowerCase());
	AddDepend(NickExists, params_.ExtractWord(2, ": ").LowerCase());
	AddDepend(UserInChan, params_.ExtractWord(1, ": ").LowerCase() + ":" + params_.ExtractWord(2, ": ").LowerCase());
    }
    else if (msgtype_ == "KILL")
    {
	// Target exists
	AddDepend(NickExists, params_.ExtractWord(1, ": ").LowerCase());
    }
    else if ((msgtype_ == "MODE" || msgtype_ == "SVSMODE") && IsChan(params_.ExtractWord(1, ": ")))
    {
	// Channel exists
	// ALL params_ that are users exist ... ?
	AddDepend(ChanExists, params_.ExtractWord(1, ": ").LowerCase());
    }
    else if (msgtype_ == "NICK" || msgtype_ == "SNICK" || msgtype_ == "USER")
    {
	// Server exists OR
	// Target nick does NOT exist
	AddDepend(NickNoExists, params_.ExtractWord(1, ": ").LowerCase());
	if (source_.empty())
	{
	    switch (Parent->server.proto.Signon())
	    {
	    case 0000:
		AddDepend(ServerExists, params_.ExtractWord(4, ": ").LowerCase());
		break;
	    case 0001:
		AddDepend(ServerExists, params_.ExtractWord(5, ": ").LowerCase());
		break;
	    case 1000:
	    case 1001:
	    case 1002:
		AddDepend(ServerExists, params_.ExtractWord(6, ": ").LowerCase());
		break;
	    case 1003:
	    case 2000:
	    case 2001:
		AddDepend(ServerExists, params_.ExtractWord(7, ": ").LowerCase());
		break;
	    case 2002:
		AddDepend(ServerExists, params_.ExtractWord(8, ": ").LowerCase());
		break;
	    case 2003:
		AddDepend(ServerExists, params_.ExtractWord(6, ": ").LowerCase());
		break;
	    }
	}
    }
    else if (msgtype_ == "PART")
    {
	// Channel exists
	// User in channel
	AddDepend(ChanExists, params_.ExtractWord(1, ": ").LowerCase());
	AddDepend(UserInChan, params_.ExtractWord(1, ": ").LowerCase() + ":" + source_.LowerCase());
    }
    else if (msgtype_ == "SERVER")
    {
	// Uplink exists
	AddDepend(ServerNoExists, params_.ExtractWord(1, ": ").LowerCase());
    }
    else if (msgtype_ == "SJOIN")
    {
	// ALL nick's mentioned
	if (source_[0u] == '@' || source_.Contains(".") || source_.empty())
	{
	    for (unsigned int i=1; i <= params_.After(":").WordCount(" "); i++)
	    {
		mstring nick(params_.After(":").ExtractWord(i, " ").LowerCase());
		if (!nick.empty())
		{
		    char c = 0;
		    bool IsNotNick = false;
		    while (c != nick[0u])
		    {
			c = nick[0u];
			switch(nick[0u])
			{
			case '@':
			case '%':
			case '+':
			case '*':
			case '~':
			    nick.erase(0, 0);
		            break;
			case '&':
			case '"':
			    IsNotNick = true;
			    break;
			}
		    }
		    if (!IsNotNick)
		    {
			AddDepend(NickExists, nick);
			AddDepend(UserNoInChan, params_.ExtractWord(2, ": ").LowerCase() + ":" + nick);
		    }
		}
	    }
	}
	else
	{
	    for (unsigned int i=2; i<=params_.WordCount(": "); i++)
	    {
		char c = 0;
		mstring chan = params_.ExtractWord(i, ": ").LowerCase();
		while (c != chan[0u])
		{
		    c = chan[0u];
		    switch(chan[0u])
		    {
		    case '@':
		    case '%':
		    case '+':
		    case '*':
		    case '~':
			chan.erase(0, 0);
			break;
		    }
		}
		AddDepend(UserNoInChan, chan + ":" + source_.LowerCase());
	    }
	}
    }
    else if (msgtype_ == "SQUIT")
    {
	// Uplink exists
	AddDepend(ServerExists, params_.ExtractWord(1, ": ").LowerCase());
    }
    else if (msgtype_ == "TOPIC")
    {
	// Channel exists
	AddDepend(ChanExists, params_.ExtractWord(1, ": ").LowerCase());
    }

    unsigned long msgid;
    { MLOCK(("MsgIdMap"));
    msgid = LastMsgId++;
    while (MsgIdMap.find(msgid) != MsgIdMap.end())
	msgid = LastMsgId++;
    }

    list<triplet<type_t, mstring, bool> >::iterator iter;
    for (iter=dependancies.begin(); iter != dependancies.end(); iter++)
    {
	switch (iter->first)
	{
	case ServerExists:
	    if (Parent->server.GetServer(iter->second).empty())
	    {
		added = true;
		WLOCK2(("AllDependancies"));
		AllDependancies[ServerExists][iter->second].insert(msgid);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case ServerNoExists:
	    if (!Parent->server.GetServer(iter->second).empty())
	    {
		added = true;
		WLOCK2(("AllDependancies"));
		AllDependancies[ServerNoExists][iter->second].insert(msgid);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case NickExists:
	    if (!Parent->nickserv.IsLive(iter->second))
	    {
		added = true;
		WLOCK2(("AllDependancies"));
		AllDependancies[NickExists][iter->second].insert(msgid);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case NickNoExists:
	    if (Parent->nickserv.IsLive(iter->second))
	    {
		added = true;
		WLOCK2(("AllDependancies"));
		AllDependancies[NickNoExists][iter->second].insert(msgid);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case ChanExists:
	    if (!Parent->chanserv.IsLive(iter->second))
	    {
		added = true;
		WLOCK2(("AllDependancies"));
		AllDependancies[ChanExists][iter->second].insert(msgid);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case ChanNoExists:
	    if (Parent->chanserv.IsLive(iter->second))
	    {
		added = true;
		WLOCK2(("AllDependancies"));
		AllDependancies[ChanNoExists][iter->second].insert(msgid);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	case UserInChan:
	    if (Parent->chanserv.IsLive(iter->second.Before(":")))
	    {
		if (!Parent->chanserv.GetLive(iter->second.Before(":")).IsIn(iter->second.After(":")))
		{
		    added = true;
		    WLOCK2(("AllDependancies"));
		    AllDependancies[UserInChan][iter->second].insert(msgid);
		}
		else
		{
		    iter->third = true;
		}
	    }
	    else
	    {
		WLOCK2(("AllDependancies"));
		AllDependancies[UserInChan][iter->second].insert(msgid);
	    }
	    break;
	case UserNoInChan:
	    if (Parent->chanserv.IsLive(iter->second.Before(":")) &&
		Parent->chanserv.GetLive(iter->second.Before(":")).IsIn(iter->second.After(":")))
	    {
		added = true;
		WLOCK2(("AllDependancies"));
		AllDependancies[UserNoInChan][iter->second].insert(msgid);
	    }
	    else
	    {
		iter->third = true;
	    }
	    break;
	}
    }
    if (added)
    {
	MLOCK(("MsgIdMap"));
	MsgIdMap[msgid] = this;
    }
}

bool mMessage::OutstandingDependancies()
{
    NFT("mMessage::OutstandingDependancies");

    if (!dependancies.size())
	AddDependancies();
    
    RLOCK(("Dependancies", this));
    list<triplet<type_t, mstring, bool> >::iterator iter;
    for (iter=dependancies.begin(); iter != dependancies.end(); iter++)
    {
	if (!iter->third)
	{
	    RET(true);
	}
    }

    RET(false);
}

void mMessage::CheckDependancies(mMessage::type_t type, const mstring& param1, const mstring& param2)
{
    FT("mMessage::CheckDependancies", ((int) type, param1, param2));

    if (param1.empty())
	return;

    // ONE dependancy has been filled, find ALL messages that depended on it, and
    // check if all of its OTHER dependancies have been fulfilled, if so, execute
    // that message.  In either case, remove all dependancies on passed condition,
    // as this has been satisfied.

    mstring target;
    if (!param2.empty())
	target = param1.LowerCase() + ":" + param2.LowerCase();
    else
	target = param1.LowerCase();
    set<unsigned long> mydep;

    { WLOCK(("AllDependancies"));
    map<type_t, map<mstring, set<unsigned long> > >::iterator i = AllDependancies.find(type);
    if (i != AllDependancies.end())
    {
	map<mstring, set<unsigned long> >::iterator j = i->second.find(target);
	if (j != i->second.end())
	{
	    mydep = j->second;
	    i->second.erase(j);
	}
    }}

    set<unsigned long>::iterator k;
    for (k=mydep.begin(); k!=mydep.end(); k++)
    {
	mMessage *msg = NULL;
	{ MLOCK(("MsgIdMap"));
	map<unsigned long, mMessage *>::iterator iter = MsgIdMap.find(*k);
	if (iter != MsgIdMap.end())
	{
	    msg = iter->second;
	    MsgIdMap.erase(iter);
	}}
	if (msg != NULL)
	{
	    msg->DependancySatisfied(type, target);
	    if (!msg->OutstandingDependancies())
	    {
		msg->priority(static_cast<u_long>(P_DepFilled));
		RLOCK(("IrcSvcHandler"));
		if (Parent->ircsvchandler != NULL)
		    Parent->ircsvchandler->enqueue(msg);
	    }
	}
    }
}

void mMessage::DependancySatisfied(mMessage::type_t type, const mstring& param)
{
    FT("mMessage::DependancySatisfied", ((int) type, param));

    WLOCK(("Dependancies", this));
    list<triplet<type_t, mstring, bool> >::iterator iter;
    for (iter=dependancies.begin(); iter != dependancies.end(); iter++)
    {
	if (iter->first == type && iter->second == param)
	{
	    iter->third = true;
	}
    }
}

int mMessage::call()
{
    NFT("mMessage::call");

    if (source_ == " ")
    {
	if (msgtype_.IsSameAs("SHUTDOWN", false))
	{
	    RET(-1);
	}
	else if (msgtype_.IsSameAs("SLEEP", false))
	{
	    sleep(FromHumanTime(params_) ? FromHumanTime(params_) : 1);
	}
	else if (msgtype_.IsSameAs("TEST", false))
	{
	    // Make sure we dont take any more test messages ...
	    ACE_Thread::yield();
	}
	RET(0);
    }

    CP(("Processing message (%s) %s %s", source_.c_str(), msgtype_.c_str(), params_.c_str()));

    try {

    if ((msgtype_ == "PRIVMSG" || msgtype_ == "NOTICE") && Parent->nickserv.IsLive(source_) &&
	!IsChan(params_.ExtractWord(1, ": ")))
    {
	mstring target(params_.ExtractWord(1, ": "));
	if (target.Contains("@"))
	{
	    target.Truncate(target.Find("@"));
	    params_.replace(0, params_.find(" ", 1)-1, target);
	    CP(("Target changed, new params: %s", params_.c_str()));
	}

	if (!Parent->nickserv.GetLive(source_).FloodTrigger())
	{
	    // Find out if the target nick is one of the services 'clones'
	    // Pass the message to them if so.
	    // before even that, check if it's script overriden via
	    //     Parent->checkifhandled(servername,command)
	    // if so, Parent->doscripthandle(server,command,data);

	    if (Parent->operserv.IsName(target))
		Parent->operserv.execute(source_, msgtype_, params_);

	    else if (Parent->nickserv.IsName(target) && Parent->nickserv.MSG())
		Parent->nickserv.execute(source_, msgtype_, params_);

	    else if (Parent->chanserv.IsName(target) && Parent->chanserv.MSG())
		Parent->chanserv.execute(source_, msgtype_, params_);

	    else if (Parent->memoserv.IsName(target) && Parent->memoserv.MSG())
		Parent->memoserv.execute(source_, msgtype_, params_);

	    else if (Parent->commserv.IsName(target) && Parent->commserv.MSG())
		Parent->commserv.execute(source_, msgtype_, params_);

	    else if (Parent->servmsg.IsName(target) && Parent->servmsg.MSG())
		Parent->servmsg.execute(source_, msgtype_, params_);

	    // else check if it's script handled, might do up a list of script servers
	    // in the magick object to check against, else trash it.

	    else	// PRIVMSG or NOTICE to non-service
		Parent->server.execute(source_, msgtype_, params_);

	}
	else if (Parent->operserv.Log_Ignore())
	{
	    // Check if we're to log ignore messages, and log them here.
	    LOG(LM_DEBUG, "OPERSERV/IGNORED", (
			source_, msgtype_ + " " + params_));
	}
    }
    else
	Parent->server.execute(source_, msgtype_, params_);

    }
    catch (E_NickServ_Stored &e)
    {
	switch(e.where())
	{
	    case E_NickServ_Stored::W_Get:
		switch (e.type())
		{
		case E_NickServ_Stored::T_Invalid:
		case E_NickServ_Stored::T_Blank:
		    if (strlen(e.what()))
		    {
			Parent->nickserv.RemStored(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_NickServ_Live &e)
    {
	switch(e.where())
	{
	    case E_NickServ_Live::W_Get:
		switch (e.type())
		{
		case E_NickServ_Live::T_Invalid:
		case E_NickServ_Live::T_Blank:
		    if (strlen(e.what()))
		    {
			Parent->nickserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_NickServ_Recovered &e)
    {
	switch(e.where())
	{
	    case E_NickServ_Recovered::W_Get:
		switch (e.type())
		{
		case E_NickServ_Recovered::T_Invalid:
		case E_NickServ_Recovered::T_Blank:
		    if (strlen(e.what()))
		    {
			Parent->nickserv.RemRecovered(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_ChanServ_Stored &e)
    {
	switch(e.where())
	{
	    case E_ChanServ_Stored::W_Get:
		switch (e.type())
		{
		case E_ChanServ_Stored::T_Invalid:
		case E_ChanServ_Stored::T_Blank:
		    if (strlen(e.what()))
		    {
			Parent->chanserv.RemStored(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_ChanServ_Live &e)
    {
	switch(e.where())
	{
	    case E_ChanServ_Live::W_Get:
		switch (e.type())
		{
		case E_ChanServ_Live::T_Invalid:
		case E_ChanServ_Live::T_Blank:
		    if (strlen(e.what()))
		    {
			Parent->chanserv.RemLive(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_CommServ_List &e)
    {
	switch(e.where())
	{
	    case E_CommServ_List::W_Get:
		switch (e.type())
		{
		case E_CommServ_List::T_Invalid:
		case E_CommServ_List::T_Blank:
		    if (strlen(e.what()))
		    {
			Parent->commserv.RemList(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_Server_List &e)
    {
	switch(e.where())
	{
	    case E_Server_List::W_Get:
		switch (e.type())
		{
		case E_Server_List::T_Invalid:
		case E_Server_List::T_Blank:
		    if (strlen(e.what()))
		    {
			Parent->server.RemList(e.what());
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }
    catch (E_MemoServ_Nick &e)
    {
    }
    catch (E_MemoServ_Channel &e)
    {
    }
    catch (E_DccMap_Xfers &e)
    {
	switch(e.where())
	{
	    case E_DccMap_Xfers::W_Get:
		switch (e.type())
		{
		case E_DccMap_Xfers::T_Invalid:
		case E_DccMap_Xfers::T_Blank:
		    if (strlen(e.what()))
		    {
			DccMap::RemXfers(atoi(e.what()));
		    }
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	}
    }

    RET(0);
}

bool mBase::signon(const mstring &nickname) const
{
    FT("mBase::signon", (nickname));

    if (Parent->nickserv.IsLive(nickname))
    {
	RET(false);
    }
    else
    {
	Parent->server.NICK(nickname,
		(Parent->startup.Ownuser() ? nickname.LowerCase() : Parent->startup.Services_User()),
		Parent->startup.Services_Host(), Parent->startup.Server_Name(), realname);
	RET(true);
    }
}

bool mBase::signoff(const mstring &nickname) const
{
    FT("mBase::signoff", (nickname));

    if (Parent->nickserv.IsLive(nickname))
    {
	Parent->server.QUIT(nickname);
	RET(true);
    }
    else
    {
	RET(false);
    }
}


void mBase::privmsg(const mstring &source, const mstring &dest, const char *pszFormat, ...) const
{
    FT("mBase::privmsg", (source, dest, pszFormat));

    va_list argptr;
    va_start(argptr, pszFormat);
    privmsgV(source, dest, pszFormat, argptr);
    va_end(argptr);
}


void mBase::privmsg(const mstring &dest, const char *pszFormat, ...) const
{
    FT("mBase::privmsg", (dest, pszFormat));

    va_list argptr;
    va_start(argptr, pszFormat);
    privmsgV(FirstName(), dest, pszFormat, argptr);
    va_end(argptr);
}


void mBase::privmsgV(const mstring &source, const mstring &dest, const char *pszFormat, va_list argptr) const
{
    FT("mBase::privmsgV", (source, dest, pszFormat));

    mstring message;
    message.FormatV(pszFormat, argptr);

    if (IsName(source) && !Parent->getLname(dest).empty())
	Parent->server.PRIVMSG(source, dest, message);
}


void mBase::notice(const mstring &source, const mstring &dest, const char *pszFormat, ...) const
{
    FT("mBase::notice", (source, dest, pszFormat));

    va_list argptr;
    va_start(argptr, pszFormat);
    noticeV(source, dest, pszFormat, argptr);
    va_end(argptr);
}


void mBase::notice(const mstring &dest, const char *pszFormat, ...) const
{
    FT("mBase::notice", (dest, pszFormat));

    va_list argptr;
    va_start(argptr, pszFormat);
    noticeV(FirstName(), dest, pszFormat, argptr);
    va_end(argptr);
}


void mBase::noticeV(const mstring &source, const mstring &dest, const char *pszFormat, va_list argptr) const
{
    FT("mBase::noticeV", (source, dest, pszFormat));

    mstring message;
    message.FormatV(pszFormat, argptr);
    if (IsName(source) && !Parent->getLname(dest).empty())
	Parent->server.NOTICE(source, dest, message);
}


void mBase::send(const mstring &source, const mstring &dest, const char *pszFormat, ...) const
{
    FT("mBase::send", (source, dest, pszFormat));

    va_list argptr;
    va_start(argptr, pszFormat);
    sendV(source, dest, pszFormat, argptr);
    va_end(argptr);
}

void mBase::send(const mstring &dest, const char *pszFormat, ...) const
{
    FT("mBase::send", (dest, pszFormat));

    va_list argptr;
    va_start(argptr, pszFormat);
    sendV(FirstName(), dest, pszFormat, argptr);
    va_end(argptr);
}

void mBase::sendV(const mstring &source, const mstring &dest, const char *pszFormat, va_list argptr) const
{
    FT("mBase::sendV", (source, dest, pszFormat));

    if (IsName(source) && Parent->nickserv.IsLive(dest))
    {
	if (!Parent->nickserv.LCK_PRIVMSG() && Parent->nickserv.IsStored(dest) &&
		Parent->nickserv.GetStored(dest).IsOnline())
	{
	    if (Parent->nickserv.GetStored(dest).PRIVMSG()) {
		privmsgV(source, dest, pszFormat, argptr);
	    }
	    else
	    {
		noticeV(source, dest, pszFormat, argptr);
	    }
	}
	else
	{
	    if (Parent->nickserv.DEF_PRIVMSG())
	    {
		privmsgV(source, dest, pszFormat, argptr);
	    }
	    else
	    {
		noticeV(source, dest, pszFormat, argptr);
	    }
	}
    }
}


void privmsg(const mstring& source, const mstring &dest, const char *pszFormat, ...)
{
    FT("privmsg", (source, dest, pszFormat));

    va_list argptr;
    va_start(argptr, pszFormat);
	if (Parent->operserv.IsName(source))
	    Parent->operserv.privmsgV(source, dest, pszFormat, argptr);

	else if (Parent->nickserv.IsName(source))
	    Parent->nickserv.privmsgV(source, dest, pszFormat, argptr);

	else if (Parent->chanserv.IsName(source))
	    Parent->chanserv.privmsgV(source, dest, pszFormat, argptr);

	else if (Parent->memoserv.IsName(source))
	    Parent->memoserv.privmsgV(source, dest, pszFormat, argptr);

	else if (Parent->commserv.IsName(source))
	    Parent->commserv.privmsgV(source, dest, pszFormat, argptr);

	else if (Parent->servmsg.IsName(source))
	    Parent->servmsg.privmsgV(source, dest, pszFormat, argptr);

	// scripted hosts ...
	else
	{
	    LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", (
		"PRIVMSG", source));
	}
    va_end(argptr);
}


void notice(const mstring& source, const mstring &dest, const char *pszFormat, ...)
{
    FT("notice", (source, dest, pszFormat));

    va_list argptr;
    va_start(argptr, pszFormat);
	if (Parent->operserv.IsName(source))
	    Parent->operserv.noticeV(source, dest, pszFormat, argptr);

	else if (Parent->nickserv.IsName(source))
	    Parent->nickserv.noticeV(source, dest, pszFormat, argptr);

	else if (Parent->chanserv.IsName(source))
	    Parent->chanserv.noticeV(source, dest, pszFormat, argptr);

	else if (Parent->memoserv.IsName(source))
	    Parent->memoserv.noticeV(source, dest, pszFormat, argptr);

	else if (Parent->commserv.IsName(source))
	    Parent->commserv.noticeV(source, dest, pszFormat, argptr);

	else if (Parent->servmsg.IsName(source))
	    Parent->servmsg.noticeV(source, dest, pszFormat, argptr);

	// scripted hosts ...
	else
	{
	    LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", (
		"NOTICE", source));
	}
    va_end(argptr);
}


void send(const mstring& source, const mstring &dest, const char *pszFormat, ...)
{
    FT("send", (source, dest, pszFormat));

    va_list argptr;
    va_start(argptr, pszFormat);
	if (Parent->operserv.IsName(source))
	    Parent->operserv.sendV(source, dest, pszFormat, argptr);

	else if (Parent->nickserv.IsName(source))
	    Parent->nickserv.sendV(source, dest, pszFormat, argptr);

	else if (Parent->chanserv.IsName(source))
	    Parent->chanserv.sendV(source, dest, pszFormat, argptr);

	else if (Parent->memoserv.IsName(source))
	    Parent->memoserv.sendV(source, dest, pszFormat, argptr);

	else if (Parent->commserv.IsName(source))
	    Parent->commserv.sendV(source, dest, pszFormat, argptr);

	else if (Parent->servmsg.IsName(source))
	    Parent->servmsg.sendV(source, dest, pszFormat, argptr);

	// scripted hosts ...
	else
	{
	    LOG(LM_WARNING, "ERROR/REQ_BYNONSERVICE", (
		"SEND", source));
	}

    va_end(argptr);
}

void announce(const mstring& source, const char *pszFormat, ...)
{
    FT("announce", (source, pszFormat));

    va_list argptr;
    va_start(argptr, pszFormat);
    mstring message;
    message.FormatV(pszFormat, argptr);
    va_end(argptr);
	if (Parent->server.proto.Globops())
	    Parent->server.GLOBOPS(source, message);
	else
	    Parent->server.WALLOPS(source, message);
}

// Command Map stuff ...

void CommandMap::AddSystemCommand(const mstring &service, const mstring &command,
	    const mstring &committees, functor function)
{
    FT("CommandMap::AddSystemCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_system"));
    i_system[service.LowerCase()].push_back(triplet<mstring, mstring, functor>
		    (command.UpperCase(),
		    ((!committees.empty()) ? committees.LowerCase() : mstring("all")),
		    function));
}


void CommandMap::RemSystemCommand(const mstring &service, const mstring &command,
	    const mstring &committees)
{
    FT("CommandMap::RemSystemCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_system"));
    if (i_system.find(service.LowerCase()) != i_system.end())
    {
	cmdtype::iterator iter;
	for (iter = i_system[service.LowerCase()].begin();
		iter != i_system[service.LowerCase()].end(); iter++)
	{
	    if (iter->first == command.UpperCase() &&
		    mstring(" " + iter->second + " ").Contains(
		    mstring(" " + committees.LowerCase() + " ")))
	    {
		i_system[service.LowerCase()].erase(iter);
		if (!i_system[service.LowerCase()].size())
		    i_system.erase(service.LowerCase());
		return;
	    }
	}
    }
}


void CommandMap::AddCommand(const mstring &service, const mstring &command,
	    const mstring &committees, functor function)
{
    FT("CommandMap::AddCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_user"));
    i_user[service.LowerCase()].push_back(triplet<mstring, mstring, functor>
		    (command.UpperCase(),
		    ((!committees.empty()) ? committees.LowerCase() : mstring("all")),
		    function));
}


void CommandMap::RemCommand(const mstring &service, const mstring &command,
	    const mstring &committees)
{
    FT("CommandMap::RemCommand", (service, command, committees));

    WLOCK(("CommandMap", "i_user"));
    if (i_user.find(service.LowerCase()) != i_user.end())
    {
	cmdtype::iterator iter;
	for (iter = i_user[service.LowerCase()].begin();
		iter != i_user[service.LowerCase()].end(); iter++)
	{
	    if (iter->first == command.UpperCase() &&
		    mstring(" " + iter->second + " ").Contains(
		    mstring(" " + committees.LowerCase() + " ")))
	    {
		i_user[service.LowerCase()].erase(iter);
		if (!i_user[service.LowerCase()].size())
		    i_user.erase(service.LowerCase());
		return;
	    }
	}
    }
}


pair<bool, CommandMap::functor> CommandMap::GetUserCommand(const mstring &service,
	    const mstring &command, const mstring &user) const
{
    FT("CommandMap::GetUserCommand", (service, command, user));
    unsigned int i;
    pair<bool, functor> retval = pair<bool, functor>(false, NULL);
	cmdtype::const_iterator iter;
    mstring type, list;

    // IF i_system exists
    //   IF command (pattern) exists
    //     IF (" " + cmd_committees + " ") contains ANY OF committees
    //       RETURN true
    //     ENDIF
    //   ENDIF
    // ENDIF
    // RETURN false;

    if (Parent->operserv.IsName(service))
	type = Parent->operserv.GetInternalName().LowerCase();
    else if (Parent->nickserv.IsName(service))
	type = Parent->nickserv.GetInternalName().LowerCase();
    else if (Parent->chanserv.IsName(service))
	type = Parent->chanserv.GetInternalName().LowerCase();
    else if (Parent->memoserv.IsName(service))
	type = Parent->memoserv.GetInternalName().LowerCase();
    else if (Parent->commserv.IsName(service))
	type = Parent->commserv.GetInternalName().LowerCase();
    else if (Parent->servmsg.IsName(service))
	type = Parent->servmsg.GetInternalName().LowerCase();
    //else
    //  scripted stuff ...

    if (type.empty())
	NRET(pair<bool_functor>,retval);

    RLOCK(("CommandMap", "i_user"));
    cmdmap::const_iterator mi = i_user.find(type);
    if (mi != i_user.end())
    {
	for (iter=mi->second.begin(); iter!=mi->second.end(); iter++)
	{
	    if (command.Matches(iter->first, true))
	    {
		for (i=1; i <= iter->second.WordCount(" "); i++)
		{
		    list = iter->second.ExtractWord(i, " ").UpperCase();
		    // If its a command for "ALL" users, OR
		    // its a valid committee AND a valid (reg'd + online) user
		    //       AND that user is on the committee
		    if (Parent->commserv.IsList(list)
			&& Parent->commserv.GetList(list).IsOn(user))
		    {
			retval.first = true;
			retval.second = iter->third;
			NRET(pair<bool_functor>,retval);
		    }
		}
	    }
	}
    }
    NRET(pair<bool_functor>,retval);
}

pair<bool, CommandMap::functor> CommandMap::GetSystemCommand(const mstring &service,
	    const mstring &command, const mstring &user) const
{
    FT("CommandMap::GetSystemCommand", (service, command, user));
    unsigned int i;
    pair<bool, functor> retval = pair<bool, functor>(false, NULL);
	cmdtype::const_iterator iter;
    mstring type, list;

    // IF i_system exists
    //   IF command (pattern) exists
    //     IF (" " + cmd_committees + " ") contains ANY OF committees
    //       RETURN true
    //     ENDIF
    //   ENDIF
    // ENDIF
    // RETURN false;

    if (Parent->operserv.IsName(service))
	type = Parent->operserv.GetInternalName().LowerCase();
    else if (Parent->nickserv.IsName(service))
	type = Parent->nickserv.GetInternalName().LowerCase();
    else if (Parent->chanserv.IsName(service))
	type = Parent->chanserv.GetInternalName().LowerCase();
    else if (Parent->memoserv.IsName(service))
	type = Parent->memoserv.GetInternalName().LowerCase();
    else if (Parent->commserv.IsName(service))
	type = Parent->commserv.GetInternalName().LowerCase();
    else if (Parent->servmsg.IsName(service))
	type = Parent->servmsg.GetInternalName().LowerCase();
    //else
    //  scripted stuff ...

    if (type.empty())
	NRET(pair<bool_functor>,retval);

    RLOCK(("CommandMap", "i_system"));
    cmdmap::const_iterator mi = i_system.find(type);
    if (mi != i_system.end())
    {
	for (iter=mi->second.begin(); iter!=mi->second.end(); iter++)
	{
	    if (command.Matches(iter->first, true))
	    {
		for (i=1; i <= iter->second.WordCount(" "); i++)
		{
		    list = iter->second.ExtractWord(i, " ").UpperCase();
		    // If its a command for "ALL" users, OR
		    // its a valid committee AND a valid (reg'd + online) user
		    //       AND that user is on the committee
		    if (Parent->commserv.IsList(list)
			 && Parent->commserv.GetList(list).IsOn(user))
		    {
			retval.first = true;
			retval.second = iter->third;
			NRET(pair<bool_functor>,retval);
		    }
		}
	    }
	}
    }
    NRET(pair<bool_functor>,retval);
}

bool CommandMap::DoCommand(const mstring &mynick, const mstring &user,
	    const mstring &command, const mstring &params) const
{
    FT("CommandMap::DoCommand", (mynick, user, command, params));

    if (DoUserCommand(mynick, user, command, params) ||
	DoSystemCommand(mynick, user, command, params))
    {
	RET(true);
    }
    if (command.WordCount(" ") < 2)
	SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_COMMAND", (
			command.UpperCase(), mynick));
    else
	SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_OPTION", (
			command.UpperCase(), mynick,
			command.Before(" ").UpperCase()));
    RET(false);
}


bool CommandMap::DoUserCommand(const mstring &mynick, const mstring &user,
	    const mstring &command, const mstring &params) const
{
    FT("CommandMap::DoUserCommand", (mynick, user, command, params));

    pair<bool,functor> cmd = GetUserCommand(mynick, command, user);
    if (cmd.first)
    {
	if (cmd.second != NULL)
	    (*cmd.second)(mynick, user, params);
	else
	    if (command.WordCount(" ") < 2)
		SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_COMMAND", (
			command.UpperCase(), mynick));
	    else
		SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_OPTION", (
			command.UpperCase(), mynick,
			command.Before(" ").UpperCase()));
	RET(true);
    }
    RET(false);
}


bool CommandMap::DoSystemCommand(const mstring &mynick, const mstring &user,
	    const mstring &command, const mstring &params) const
{
    FT("CommandMap::DoSystemCommand", (mynick, user, command, params));

    pair<bool,functor> cmd = GetSystemCommand(mynick, command, user);
    if (cmd.first)
    {
	if (cmd.second != NULL)
	    (*cmd.second)(mynick, user, params);
	else
	    if (command.WordCount(" ") < 2)
		SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_COMMAND", (
			command.UpperCase(), mynick));
	    else
		SEND(mynick, user, "ERR_SYNTAX/UNKNOWN_OPTION", (
			command.UpperCase(), mynick,
			command.Before(" ").UpperCase()));
	RET(true);
    }
    RET(false);
}


void do_1_2param(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("do_1_2param", (mynick, source, params));
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
			params.Before(" ").UpperCase(), mynick,
			params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.Before(" ", 2));
    command.MakeUpper();

    if (!Parent->commands.DoCommand(mynick, source, command, params))
    {
	// we're not worthy...
//	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//			command, mynick,
//			command.Before(" ")));
    }

}

void do_1_3param(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("do_1_3param", (mynick, source, params));
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
			params.Before(" ").UpperCase(), mynick,
			params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.Before(" ") + " " + params.ExtractWord(3, " "));
    command.MakeUpper();

    if (!Parent->commands.DoCommand(mynick, source, command, params))
    {
	// we're not worthy...
//	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//			command, mynick,
//			command.Before(" ")));
    }
}

void do_1_4param(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("do_1_4param", (mynick, source, params));
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
			params.Before(" ").UpperCase(), mynick,
			params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.Before(" ") + " " + params.ExtractWord(4, " "));
    command.MakeUpper();

    if (!Parent->commands.DoCommand(mynick, source, command, params))
    {
	// we're not worthy...
//	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//			command, mynick,
//			command.Before(" ")));
    }
}

void do_1_2paramswap(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("do_1_2paramswap", (mynick, source, params));
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
			params.Before(" ").UpperCase(), mynick,
			params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(2, " ") + " " + params.Before(" "));
    command.MakeUpper();

    mstring data(command);
    if (params.WordCount(" ") > 2)
	data += " " + params.After(" ", 2);

    if (!Parent->commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//			command, mynick,
//			command.Before(" ")));
    }

}

void do_1_3paramswap(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("do_1_3paramswap", (mynick, source, params));
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
			params.Before(" ").UpperCase(), mynick,
			params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(3, " ") + " " + params.Before(" "));
    command.MakeUpper();

    mstring data(params.ExtractWord(3, " ") + " " +
	params.ExtractWord(2, " ") + " " + params.Before(" "));
    if (params.WordCount(" ") > 3)
	data += " " + params.After(" ", 3);

    if (!Parent->commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//			command, mynick,
//			command.Before(" ")));
    }
}

void do_1_4paramswap(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("do_1_3paramswap", (mynick, source, params));
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
			params.Before(" ").UpperCase(), mynick,
			params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(4, " ") + " " + params.Before(" "));
    command.MakeUpper();

    mstring data(params.ExtractWord(4, " ") + " " +
	params.ExtractWord(2, " ") + params.ExtractWord(3, " ") +
	" " + params.Before(" "));
    if (params.WordCount(" ") > 4)
	data += " " + params.After(" ", 4);

    if (!Parent->commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//			command, mynick,
//			command.Before(" ")));
    }
}

void do_2param(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("do_2param", (mynick, source, params));
    if (params.WordCount(" ") < 2)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
			params.Before(" ").UpperCase(), mynick,
			params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(2, " "));
    command.MakeUpper();

    mstring data(command + " " + params.Before(" "));
    if (params.WordCount(" ") > 2)
	data += " " + params.After(" ", 2);

    if (!Parent->commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//			command, mynick,
//			command.Before(" ")));
    }

}

void do_3param(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("do_3param", (mynick, source, params));
    if (params.WordCount(" ") < 3)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
			params.Before(" ").UpperCase(), mynick,
			params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(3, " "));
    command.MakeUpper();

    mstring data(params.ExtractWord(3, " ") + " " +
	params.ExtractWord(2, " ") + " " + params.Before(" "));
    if (params.WordCount(" ") > 3)
	data += " " + params.After(" ", 3);

    if (!Parent->commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//			command, mynick,
//			command.Before(" ")));
    }
}

void do_4param(const mstring &mynick, const mstring &source, const mstring &params)
{
    FT("do_3param", (mynick, source, params));
    if (params.WordCount(" ") < 4)
    {
	SEND(mynick, source, "ERR_SYNTAX/NEED_PARAMS", (
			params.Before(" ").UpperCase(), mynick,
			params.Before(" ").UpperCase()));
	return;
    }
    mstring command(params.ExtractWord(4, " "));
    command.MakeUpper();

    mstring data(params.ExtractWord(4, " ") + " " +
	params.ExtractWord(2, " ") + " " + params.ExtractWord(3, " ") +
	" " + params.Before(" "));
    if (params.WordCount(" ") > 4)
	data += " " + params.After(" ", 4);

    if (!Parent->commands.DoCommand(mynick, source, command, data))
    {
	// we're not worthy...
//	SEND(mynick, source, "ERR_SYNTAX/UNKNOWN_OPTION", (
//			command, mynick,
//			command.Before(" ")));
    }
}

