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
#define RCSID(x,y) const char *rcsid_trace_cpp_ ## x () { return y; }
RCSID(trace_cpp, "@(#)$Id$");

/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.116  2002/01/13 05:18:42  prez
** More formatting, changed style slightly
**
** Revision 1.115  2002/01/12 14:42:09  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.114  2001/12/20 08:02:33  prez
** Massive change -- 'Parent' has been changed to Magick::instance(), will
** soon also move the ACE_Reactor over, and will be able to have multipal
** instances of Magick in the same process if necessary.
**
** Revision 1.113  2001/12/16 01:30:46  prez
** More changes to fix up warnings ... added some new warning flags too!
**
** Revision 1.112  2001/11/18 03:26:53  prez
** More changes re: trace names, and made the command system know the
** difference between 'insufficiant access' and 'unknown command'.
**
** Revision 1.111  2001/11/18 01:03:29  prez
** Fixed up the trace names.
**
** Revision 1.110  2001/11/12 01:05:03  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.109  2001/05/03 04:40:18  prez
** Fixed locking mechanism (now use recursive mutexes) ...
** Also now have a deadlock/nonprocessing detection mechanism.
**
** Revision 1.108  2001/05/01 14:00:24  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.107  2001/03/20 14:22:15  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.106  2001/03/02 05:24:42  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.105  2001/02/11 07:41:28  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.104  2001/02/03 02:21:35  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.103  2001/01/01 05:32:45  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.102  2000/12/23 22:22:25  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.101  2000/10/10 11:47:53  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.100  2000/09/01 10:54:39  prez
** Added Changing and implemented Modify tracing, now just need to create
** DumpB() and DumpE() functions in all classes, and put MCB() / MCE() calls
** (or MB() / ME() or CB() / CE() where MCB() / MCE() not appropriate) in.
**
** Revision 1.99  2000/08/31 06:25:09  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.98  2000/08/22 08:43:41  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.97  2000/08/06 05:27:48  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.96  2000/07/29 21:58:55  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.95  2000/06/25 07:58:50  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.94  2000/06/21 09:00:06  prez
** Fixed bug in mFile
**
** Revision 1.93  2000/05/20 15:17:00  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.92  2000/05/20 03:28:12  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.91  2000/05/17 14:08:12  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.90  2000/05/17 09:10:36  ungod
** changed most wxOutputStream to ofstream and wxInputStream
** to ifstream
**
** Revision 1.89  2000/03/24 15:35:18  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.88  2000/03/19 08:50:56  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.87  2000/03/08 23:38:37  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.86  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.85  2000/02/23 12:21:04  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.84  2000/02/17 12:55:07  ungod
** still working on borlandization
**
** Revision 1.83  2000/02/16 12:59:41  ungod
** fixing for borland compilability
**
** Revision 1.82  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.81  2000/02/15 10:37:51  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"

mstring threadname[tt_MAX] = { "LOST", "MAIN", "NS", "CS", "MS", "OS", "XS", "NET", "SCRIPT", "MBASE" };

unsigned short makehex(const mstring & SLevel)
{
    if (SLevel.length() != 6 || SLevel[0u] != '0' || (SLevel[1u] != 'x' && SLevel[1u] != 'X'))
	return 0;

    unsigned short level = 0;

    for (unsigned int j = 2; j < 6; j++)
    {
	switch (SLevel[j])
	{
	case 'F':
	case 'f':
	    switch (j)
	    {
	    case 5:
		level |= 0x000f;
		break;
	    case 4:
		level |= 0x00f0;
		break;
	    case 3:
		level |= 0x0f00;
		break;
	    case 2:
		level |= 0xf000;
		break;
	    }
	    break;
	case 'E':
	case 'e':
	    switch (j)
	    {
	    case 5:
		level |= 0x000e;
		break;
	    case 4:
		level |= 0x00e0;
		break;
	    case 3:
		level |= 0x0e00;
		break;
	    case 2:
		level |= 0xe000;
		break;
	    }
	    break;
	case 'D':
	case 'd':
	    switch (j)
	    {
	    case 5:
		level |= 0x000d;
		break;
	    case 4:
		level |= 0x00d0;
		break;
	    case 3:
		level |= 0x0d00;
		break;
	    case 2:
		level |= 0xd000;
		break;
	    }
	    break;
	case 'C':
	case 'c':
	    switch (j)
	    {
	    case 5:
		level |= 0x000c;
		break;
	    case 4:
		level |= 0x00c0;
		break;
	    case 3:
		level |= 0x0c00;
		break;
	    case 2:
		level |= 0xc000;
		break;
	    }
	    break;
	case 'B':
	case 'b':
	    switch (j)
	    {
	    case 5:
		level |= 0x000b;
		break;
	    case 4:
		level |= 0x00b0;
		break;
	    case 3:
		level |= 0x0b00;
		break;
	    case 2:
		level |= 0xb000;
		break;
	    }
	    break;
	case 'A':
	case 'a':
	    switch (j)
	    {
	    case 5:
		level |= 0x000a;
		break;
	    case 4:
		level |= 0x00a0;
		break;
	    case 3:
		level |= 0x0a00;
		break;
	    case 2:
		level |= 0xa000;
		break;
	    }
	    break;
	case '9':
	    switch (j)
	    {
	    case 5:
		level |= 0x0009;
		break;
	    case 4:
		level |= 0x0090;
		break;
	    case 3:
		level |= 0x0900;
		break;
	    case 2:
		level |= 0x9000;
		break;
	    }
	    break;
	case '8':
	    switch (j)
	    {
	    case 5:
		level |= 0x0008;
		break;
	    case 4:
		level |= 0x0080;
		break;
	    case 3:
		level |= 0x0800;
		break;
	    case 2:
		level |= 0x8000;
		break;
	    }
	    break;
	case '7':
	    switch (j)
	    {
	    case 5:
		level |= 0x0007;
		break;
	    case 4:
		level |= 0x0070;
		break;
	    case 3:
		level |= 0x0700;
		break;
	    case 2:
		level |= 0x7000;
		break;
	    }
	    break;
	case '6':
	    switch (j)
	    {
	    case 5:
		level |= 0x0006;
		break;
	    case 4:
		level |= 0x0060;
		break;
	    case 3:
		level |= 0x0600;
		break;
	    case 2:
		level |= 0x6000;
		break;
	    }
	    break;
	case '5':
	    switch (j)
	    {
	    case 5:
		level |= 0x0005;
		break;
	    case 4:
		level |= 0x0050;
		break;
	    case 3:
		level |= 0x0500;
		break;
	    case 2:
		level |= 0x5000;
		break;
	    }
	    break;
	case '4':
	    switch (j)
	    {
	    case 5:
		level |= 0x0004;
		break;
	    case 4:
		level |= 0x0040;
		break;
	    case 3:
		level |= 0x0400;
		break;
	    case 2:
		level |= 0x4000;
		break;
	    }
	    break;
	case '3':
	    switch (j)
	    {
	    case 5:
		level |= 0x0003;
		break;
	    case 4:
		level |= 0x0030;
		break;
	    case 3:
		level |= 0x0300;
		break;
	    case 2:
		level |= 0x3000;
		break;
	    }
	    break;
	case '2':
	    switch (j)
	    {
	    case 5:
		level |= 0x0002;
		break;
	    case 4:
		level |= 0x0020;
		break;
	    case 3:
		level |= 0x0200;
		break;
	    case 2:
		level |= 0x2000;
		break;
	    }
	    break;
	case '1':
	    switch (j)
	    {
	    case 5:
		level |= 0x0001;
		break;
	    case 4:
		level |= 0x0010;
		break;
	    case 3:
		level |= 0x0100;
		break;
	    case 2:
		level |= 0x1000;
		break;
	    }
	    break;
	case '0':
	    break;
	default:
	    return 0;
	}
    }
    return level;
}

// ===================================================

ThreadID::ThreadID():t_internaltype(tt_LOST), t_indent(0), t_intrace(false)
{
}

ThreadID::ThreadID(const threadtype_enum Type):t_internaltype(Type), t_indent(0), t_intrace(false)
{
}

ThreadID::~ThreadID()
{
    Flush();
}

void ThreadID::assign(const threadtype_enum Type)
{
    Flush();
    t_internaltype = Type;
}

void ThreadID::WriteOut(const mstring & message)
{
#ifndef MAGICK_TRACE_WORKS
    static_cast < void >(message);
#else
    mstring finalout;

    for (int i = 0; i < t_indent; i++)
	finalout += ".  ";
    finalout += message;

    messages.push_back(finalout);
#endif
}

mstring ThreadID::logname() const
{
    mstring name("trace");

    if (strlen(threadname[t_internaltype]))
	name << "_" << threadname[t_internaltype];
    name << ".log";
    return name;
}

// ONLY trace function with trace codes, so people
// dont wonder why the ThreadMessageQueue and mFile
// locks are being set/removed.                                
void ThreadID::Flush()
{
#ifdef MAGICK_TRACE_WORKS
    if (!Magick::instance_exists())
	return;

    ThreadID *tid = mThread::find();

    if (tid != NULL)
	tid->t_intrace = true;
    list < pair < threadtype_enum, mstring > >::iterator iter;
    list < pair < threadtype_enum, mstring > >ThreadMessageQueue2;
    list < mstring > pre_messages;
    {
	MLOCK(("ThreadMessageQueue"));
	for (iter = ThreadMessageQueue.begin(); iter != ThreadMessageQueue.end(); iter++)
	{
	    if (iter->first == t_internaltype)
	    {
		pre_messages.push_back(iter->second);
	    }
	    else
	    {
		ThreadMessageQueue2.push_back(*iter);
	    }
	}
	if (ThreadMessageQueue2.size())
	    ThreadMessageQueue = ThreadMessageQueue2;
    }

    if (messages.size() || pre_messages.size())
    {
	mstring tmp;

	tmp.Format("OUTPUT FROM THREAD ID %p", this);
	pre_messages.push_front(tmp);
	{
	    MLOCK(("TraceDump", logname()));
	    mFile::Dump(pre_messages, Magick::instance().Services_Dir() + DirSlash + logname(), true, true);
	    mFile::Dump(messages, Magick::instance().Services_Dir() + DirSlash + logname(), true, true);
	}
	messages.clear();
    }
    if (tid != NULL)
	tid->t_intrace = false;
#endif
}

#ifdef MAGICK_TRACE_WORKS
//
// Tracing functions -- Include making TraceMap's and
// receiving all trace information.

unsigned short Trace::traces[tt_MAX] = { 0 };

vector < Trace::levelname_struct > Trace::levelname;	// Initialised in main.cpp
list < pair < threadtype_enum, mstring > >ThreadMessageQueue;

int levelname_count()
{
    return sizeof(Trace::levelname) / sizeof(Trace::levelname_struct);
}

// ===================================================

//      \  function()
T_Functions::T_Functions(const mstring & name):m_name(name)
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    i_prevfunc = tid->LastFunc();
    tid->LastFunc(name);
    if (IsOn(tid, Functions))
    {
	mstring message = "\\  " + m_name + "()";

	tid->WriteOut(message);
    }
    tid->indentup();
}

//      \  function( (char) T, (int) 5 )
T_Functions::T_Functions(const mstring & name, const mVarArray & args):m_name(name)
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    i_prevfunc = tid->LastFunc();
    tid->LastFunc(name);
    if (IsOn(tid, Functions))
    {
	mstring message = "\\  " + m_name + "(";

	for (int i = 0; i < args.count(); i++)
	{
	    message += " (" + args[i].type() + ") " + args[i].AsString();
	    if (i < args.count() - 1)
		message += ", ";
	}
	message += " )";
	tid->WriteOut(message);
    }
    tid->indentup();
}

//      /  (char) Y
T_Functions::~T_Functions()
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    tid->indentdown();
    tid->LastFunc(i_prevfunc);
    if (IsOn(tid, Functions))
    {
	mstring message;

	if (!return_value.type().empty())
	    message = "/  (" + return_value.type() + ") " + return_value.AsString();
	else
	    message = "/ ";
	tid->WriteOut(message);
    }
}

// ===================================================

//      ** This is an important part!
T_CheckPoint::T_CheckPoint()
{
    common("CheckPoint Reached");
}

T_CheckPoint::T_CheckPoint(const char *fmt, ...)
{
    mstring output;
    va_list args;

    va_start(args, fmt);
    output.FormatV(fmt, args);
    va_end(args);
    common(output);
}

void T_CheckPoint::common(const mstring & input)
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, CheckPoint))
    {
	tid->WriteOut("** " + input);
    }
}

// ===================================================

//      ## Loading value blah ...
T_Comments::T_Comments()
{
    common("Comment Reached");
}

T_Comments::T_Comments(const char *fmt, ...)
{
    va_list args;
    mstring output;

    va_start(args, fmt);
    output.FormatV(fmt, args);
    va_end(args);
    common(output);
}

void T_Comments::common(const mstring & input)
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Comments))
    {
	tid->WriteOut("## " + input);
    }
}

// ===================================================

//      << DE1(PreZ)
//      << DE2(prez)
//      << DE3(srealm.net.au)
T_Modify::T_Modify(const mVarArray & args, unsigned int offset):i_args(args), i_offset(offset)
{
}

void T_Modify::Begin()
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Modify))
    {
	for (int i = 0; i < i_args.count(); i++)
	{
	    mstring message;

	    message << "<< " << "DE" << i + i_offset + 1 << "(" << i_args[i].AsString() << ")";
	    tid->WriteOut(message);
	}
    }
}

//      >> DE1(PreZ)
//      >> DE2(prez)
//      >> DE3(corewars.net)
void T_Modify::End()
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Modify))
    {
	for (int i = 0; i < i_args.count(); i++)
	{
	    mstring message;

	    message << ">> " << "DE" << i + i_offset + 1 << "(" << i_args[i].AsString() << ")";
	    tid->WriteOut(message);
	}
    }
}

// ===================================================

//      << DE1(PreZ)
//      << DE2(prez)
//      << DE3(srealm.net.au)
T_Changing::T_Changing(const mstring & name, const mVariant & arg):i_name(name), i_arg(arg)
{
}

//      >> DE1(PreZ)
//      >> DE2(prez)
//      >> DE3(corewars.net)
void T_Changing::End(const mVariant & arg)
{
    if (i_name.empty())
	return;
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Changing))
    {
	mstring message;

	message << "== " << i_name << " = " << i_arg.AsString() << " -> " << arg.AsString();
	tid->WriteOut(message);
    }
}

// ===================================================

//      <- PreZ :PRIVMSG ChanServ :help blah
//      -> ChanServ :PRIVMSG PreZ :Unknown command 'blah'
//      -- ChanServ :PRIVMSG ChanServ :WTF?!
T_Chatter::T_Chatter(const dir_enum direction, const mstring & input)
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Chatter))
    {
	mstring message;

	if (direction == D_From)
	    message << "<- " << input;
	else if (direction == D_To)
	    message << "-> " << input;
	else
	    message << "-- " << input;	// Confused
	tid->WriteOut(message);
    }
}

// ===================================================

// real, swap and usage/space are in kb (1024 byte blocks)

//      %% Magick v2.0 -- Up 1 month, 6 days, 13:47:26 (13:26m CPU).
//      %% CURRENT - CPU  2.6% | MEM     0.2% | REAL    2560 | SWAP       0 | USAGE       213
//      %% MAX     - CPU  5.7% | MEM     1.1% | REAL    8725 | SWAP       0 | USAGE      1526
//      %% SYSTEM  - CPU  0.32 | AVAILABLE:     REAL    2672 | SWAP  121512 | SPACE   1578294

// T_Stats::T_Stats() {}

// ===================================================

//      :+ R ChanInfo::#Magick
//      :+ W NickInfo::PreZ::Flags
//      :+ M Magick::LoadMessages
void T_Locking::open(const locktype_enum ltype, const mstring & lockname)
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Locking))
    {
	locktype = ltype;
	name = lockname;
	mstring message;

	if (locktype == L_Read)
	    message << ":+ " << "R " << name;
	else if (locktype == L_Write)
	    message << ":+ " << "W " << name;
	else if (locktype == L_Mutex)
	    message << ":+ " << "M " << name;
	else
	    message << ":+ " << "I " << name;
	tid->WriteOut(message);
    }
}

//      :- R ChanInfo::#Magick
//      :- W NickInfo::PreZ::Flags
//      :- M Magick::LoadMessages

T_Locking::~T_Locking()
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Locking))
    {
	if (!name.empty())
	{
	    mstring message;

	    if (locktype == L_Read)
		message << ":- " << "R " << name;
	    else if (locktype == L_Write)
		message << ":- " << "W " << name;
	    else if (locktype == L_Mutex)
		message << ":- " << "M " << name;
	    else
		message << ":- " << "I " << name;
	    tid->WriteOut(message);
	}
    }
}

// ===================================================

T_Source::T_Source(const mstring & text)
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Source))
    {
	tid->WriteOut("$$ " + text);
    }
}

T_Source::T_Source(const mstring & section, const mstring & key, const mstring & value)
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Source))
    {
	mstring message;

	message << "$$ [" << section << "] " << key << " = " << value;
	tid->WriteOut(message);
    }
}

// ===================================================

//      |+ 3: 2478 <- 203.30.145.2:6667
//      |+ 3: 2478 -> 203.30.145.2:6667
//      || 3: 2478 -> 203.30.145.2:6667 (no route to host)
//      |= 3: Server (styx.us.relic.net)
//      |= 3: Telnet (Ungod)
//      |= 3: DCC (PreZ)
//      |= 3: ServNet (2)
//      |- 3: Socket Timeout

// T_Sockets::T_Sockets() {}

void T_Sockets::Begin(const unsigned long id, const unsigned short local, const unsigned short remote, const mstring & host,
		      const dir_enum direction)
{
    s_id = id;
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Sockets))
    {
	mstring message;

	message << "|+ " << s_id << ": " << local;
	if (direction == D_To)
	    message << " <- ";
	else if (direction == D_From)
	    message << " -> ";
	else
	    message << " <-> ";
	message << host << ":" << remote;
	tid->WriteOut(message);
    }
}

void T_Sockets::Failed(const unsigned long id, const unsigned short local, const unsigned short remote, const mstring & host,
		       const mstring & reason, const dir_enum direction)
{
    s_id = id;
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Sockets))
    {
	mstring message;

	message << "|| " << s_id << ": " << local;
	if (direction == D_To)
	    message << " <- ";
	else if (direction == D_From)
	    message << " -> ";
	else
	    message << " <-> ";
	message << host << ":" << remote << " (" << reason << ")";
	tid->WriteOut(message);
    }
}

void T_Sockets::Resolve(const socktype_enum type, const mstring & info)
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Sockets))
    {
	mstring message;

	message << "|= " << s_id << ": ";
	switch (type)
	{
	case S_IrcServer:
	    message << "IRC Server (" << info << ")";
	    break;
	case S_DCC:
	    message << "DCC Chat (" << info << ")";
	    break;
	case S_DCCFile:
	    message << "DCC File Transfer (" << info << ")";
	    break;
	case S_Client:
	    message << PACKAGE << " Client (" << info << ")";
	    break;
	case S_Services:
	    message << "Other " << PACKAGE << " (" << info << ")";
	    break;
	case S_Telnet:
	    message << "Telnet (" << info << ")";
	    break;
	default:
	    message << "Unknown (" << info << ")";
	}
	tid->WriteOut(message);
    }
}

void T_Sockets::End(const mstring & reason)
{
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Sockets))
    {
	mstring message;

	message << "|- " << s_id << ": " << reason;
	tid->WriteOut(message);
    }
}

// ===================================================

//      !! VAR+ added_userinfo
//      !! VAR- added_userinfo
//      !! FUNC+ user_process( (char *) source, (char *) message )
//      !! FUNC- user_process( (char *) source, (char *) message )
//      !! BIND+ MSG "SET USERINFO" user_process
//      !! BIND- MSG "SET USERINFO" user_process
//      !! SERV+ new_service nick nick ...
//      !! SERV- new_service

// T_Bind::T_Bind() {}

// ===================================================

//      ?? EXEC    ls /etc >/tmp/etcls
//      ?? OPEN(R) /tmp/etcls
//      ?? OPEN(W) /tmp/output
//      ?? CLOSE   /tmp/output

// T_External::T_External() {}

#endif /* MAGICK_TRACE_WORKS */
