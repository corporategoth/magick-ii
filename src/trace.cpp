#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_trace_cpp_ ## x () { return y; }
RCSID(trace_cpp, "@(#)$Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "magick.h"

mstring threadname[tt_MAX] =
{
"LOST", "MAIN", "NS", "CS", "MS", "OS", "XS", "NET", "SCRIPT", "MBASE"};

const char *strNonC__Exception = "NON-C++ EXCEPTION";

unsigned short makehex(const mstring & SLevel)
{
    BTCB();
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
    ETCB();
}

// ===================================================

ThreadID::ThreadID() : t_internaltype(tt_LOST), t_indent(0), t_intrace(false)
{
}

ThreadID::ThreadID(const threadtype_enum Type) : t_internaltype(Type), t_indent(0), t_intrace(false)
{
}

ThreadID::~ThreadID()
{
    BTCB();
    Flush();
    ETCB();
}

void ThreadID::assign(const threadtype_enum Type)
{
    BTCB();
    Flush();
    t_internaltype = Type;
    ETCB();
}

void ThreadID::WriteOut(const mstring & message)
{
    BTCB();
#ifndef MAGICK_TRACE_WORKS
    static_cast < void > (message);
#else
    mstring finalout;

    for (int i = 0; i < t_indent; i++)
	finalout += ".  ";
    finalout += message;

    messages.push_back(finalout);
#endif
    ETCB();
}

mstring ThreadID::logname() const
{
    BTCB();
    mstring name("trace");

    if (strlen(threadname[t_internaltype]))
	name << "_" << threadname[t_internaltype];
    name << ".log";
    return name;
    ETCB();
}

// ONLY trace function with trace codes, so people
// dont wonder why the ThreadMessageQueue and mFile
// locks are being set/removed.                                
void ThreadID::Flush()
{
    BTCB();
#ifdef MAGICK_TRACE_WORKS
    if (!Magick::instance_exists())
	return;

    ThreadID *tid = mThread::find();

    if (tid != NULL)
	tid->t_intrace = true;
    list < pair < threadtype_enum, mstring > >::iterator iter;
    list < pair < threadtype_enum, mstring > > ThreadMessageQueue2;
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
    ETCB();
}

#ifdef MAGICK_TRACE_WORKS
//
// Tracing functions -- Include making TraceMap's and
// receiving all trace information.

unsigned short Trace::traces[tt_MAX] =
{
0};

vector < Trace::levelname_struct > Trace::levelname;	// Initialised in main.cpp
list < pair < threadtype_enum, mstring > > ThreadMessageQueue;

int levelname_count()
{
    BTCB();
    return sizeof(Trace::levelname) / sizeof(Trace::levelname_struct);
    ETCB();
}

// ===================================================

//      \  function()
T_Function::T_Function(const char *file, const int line, const mstring & name) : m_name(name), file_name(file), return_line(0)
{
    BTCB();
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    i_prevfunc = tid->LastFunc();
    tid->LastFunc(name);
    if (IsOn(tid, Function))
    {
	mstring message;

	message << "\\  " << file_name << ":" << line << ": " << m_name << "()";

	tid->WriteOut(message);
    }
    tid->indentup();
    ETCB();
}

//      \  function( (char) T, (int) 5 )
T_Function::T_Function(const char *file, const int line, const mstring & name, const mVarArray & args) : m_name(name),
file_name(file), return_line(0)
{
    BTCB();
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    i_prevfunc = tid->LastFunc();
    tid->LastFunc(name);
    if (IsOn(tid, Function))
    {
	mstring message;

	message << "\\  " << file_name << ":" << line << ": " << m_name << "(";

	for (int i = 0; i < args.count(); i++)
	{
	    message << " (" << args[i].type() << ") " << args[i].AsString();
	    if (i < args.count() - 1)
		message << ", ";
	}
	message << " )";
	tid->WriteOut(message);
    }
    tid->indentup();
    ETCB();
}

//      /  (char) Y
T_Function::~T_Function()
{
    BTCB();
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    tid->indentdown();
    tid->LastFunc(i_prevfunc);
    if (IsOn(tid, Function))
    {
	mstring message;

	message << "/  ";
	if (return_line)
	    message << file_name << ":" << return_line;

	if (!return_value.type().empty())
	    message << ": (" << return_value.type() << ") " << return_value.AsString();
	tid->WriteOut(message);
    }
    ETCB();
}

// ===================================================

//      ** This is an important part!
T_CheckPoint::T_CheckPoint()
{
    BTCB();
    common("CheckPoint Reached");
    ETCB();
}

T_CheckPoint::T_CheckPoint(const char *fmt, ...)
{
    BTCB();
    mstring output;
    va_list args;

    va_start(args, fmt);
    output.FormatV(fmt, args);
    va_end(args);
    common(output);
    ETCB();
}

void T_CheckPoint::common(const mstring & input)
{
    BTCB();
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, CheckPoint))
    {
	tid->WriteOut("** " + input);
    }
    ETCB();
}

// ===================================================

//      ** file:line: details

T_Exception::T_Exception(const char *file, const int line, const char *what)
{
    BTCB();
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Exception))
    {
	mstring out;

	out << "!! " << file << ":" << line << ": " << what;
	tid->WriteOut(out);
    }
    ETCB();
}

// ===================================================

//      ## Loading value blah ...
T_Comment::T_Comment()
{
    BTCB();
    common("Comment Reached");
    ETCB();
}

T_Comment::T_Comment(const char *fmt, ...)
{
    BTCB();
    va_list args;
    mstring output;

    va_start(args, fmt);
    output.FormatV(fmt, args);
    va_end(args);
    common(output);
    ETCB();
}

void T_Comment::common(const mstring & input)
{
    BTCB();
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Comment))
    {
	tid->WriteOut("## " + input);
    }
    ETCB();
}

// ===================================================

//      << DE1(PreZ)
//      << DE2(prez)
//      << DE3(srealm.net.au)
T_Modify::T_Modify(const mVarArray & args, unsigned int offset) : i_args(args), i_offset(offset)
{
}

void T_Modify::Begin()
{
    BTCB();
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
    ETCB();
}

//      >> DE1(PreZ)
//      >> DE2(prez)
//      >> DE3(corewars.net)
void T_Modify::End()
{
    BTCB();
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
    ETCB();
}

// ===================================================

//      << DE1(PreZ)
//      << DE2(prez)
//      << DE3(srealm.net.au)
T_Changing::T_Changing(const mstring & name, const mVariant & arg) : i_name(name), i_arg(arg)
{
}

//      >> DE1(PreZ)
//      >> DE2(prez)
//      >> DE3(corewars.net)
void T_Changing::End(const mVariant & arg)
{
    BTCB();
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
    ETCB();
}

// ===================================================

//      <- PreZ :PRIVMSG ChanServ :help blah
//      -> ChanServ :PRIVMSG PreZ :Unknown command 'blah'
//      -- ChanServ :PRIVMSG ChanServ :WTF?!
T_Chatter::T_Chatter(const dir_enum direction, const mstring & input)
{
    BTCB();
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
    ETCB();
}

// ===================================================

// real, swap and usage/space are in kb (1024 byte blocks)

//      %% Magick v2.0 -- Up 1 month, 6 days, 13:47:26 (13:26m CPU).
//      %% CURRENT - CPU  2.6% | MEM     0.2% | REAL    2560 | SWAP       0 | USAGE       213
//      %% MAX     - CPU  5.7% | MEM     1.1% | REAL    8725 | SWAP       0 | USAGE      1526
//      %% SYSTEM  - CPU  0.32 | AVAILABLE:     REAL    2672 | SWAP  121512 | SPACE   1578294

// T_Stat::T_Stat() {}

// ===================================================

//      :+ R ChanInfo::#Magick
//      :+ W NickInfo::PreZ::Flags
//      :+ M Magick::LoadMessages
void T_Locking::open(const locktype_enum ltype, const mstring & lockname)
{
    BTCB();
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
    ETCB();
}

//      :- R ChanInfo::#Magick
//      :- W NickInfo::PreZ::Flags
//      :- M Magick::LoadMessages

T_Locking::~T_Locking()
{
    BTCB();
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
    ETCB();
}

// ===================================================

T_Source::T_Source(const mstring & text)
{
    BTCB();
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Source))
    {
	tid->WriteOut("$$ " + text);
    }
    ETCB();
}

T_Source::T_Source(const mstring & section, const mstring & key, const mstring & value)
{
    BTCB();
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Source))
    {
	mstring message;

	message << "$$ [" << section << "] " << key << " = " << value;
	tid->WriteOut(message);
    }
    ETCB();
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

// T_Socket::T_Socket() {}

void T_Socket::Begin(const unsigned long id, const unsigned short local, const unsigned short remote, const mstring & host,
		     const dir_enum direction)
{
    BTCB();
    s_id = id;
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Socket))
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
    ETCB();
}

void T_Socket::Failed(const unsigned long id, const unsigned short local, const unsigned short remote, const mstring & host,
		      const mstring & reason, const dir_enum direction)
{
    BTCB();
    s_id = id;
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Socket))
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
    ETCB();
}

void T_Socket::Resolve(const socktype_enum type, const mstring & info)
{
    BTCB();
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Socket))
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
    ETCB();
}

void T_Socket::End(const mstring & reason)
{
    BTCB();
    ThreadID *tid = mThread::find();

    if (tid == NULL || tid->InTrace())
	return;			// should throw an exception later
    if (IsOn(tid, Socket))
    {
	mstring message;

	message << "|- " << s_id << ": " << reason;
	tid->WriteOut(message);
    }
    ETCB();
}

// ===================================================

//      && VAR+ added_userinfo
//      && VAR- added_userinfo
//      && FUNC+ user_process( (char *) source, (char *) message )
//      && FUNC- user_process( (char *) source, (char *) message )
//      && BIND+ MSG "SET USERINFO" user_process
//      && BIND- MSG "SET USERINFO" user_process
//      && SERV+ new_service nick nick ...
//      && SERV- new_service

// T_Bind::T_Bind() {}

// ===================================================

//      ?? EXEC    ls /etc >/tmp/etcls
//      ?? OPEN(R) /tmp/etcls
//      ?? OPEN(W) /tmp/output
//      ?? CLOSE   /tmp/output

// T_External::T_External() {}

#endif /* MAGICK_TRACE_WORKS */

Logger::Logger()
{
    BTCB();
    NFT("Logger::Logger");
    ACE_Log_Msg::enable_debug_messages();

#ifdef TEST_MODE
    fout.Attach("STDERR", stderr, "a");
#else
    fout.Open(Magick::instance().files.Logfile(), "a");
#endif
    ETCB();
}

Logger::~Logger()
{
    BTCB();
    NFT("Logger::~Logger");

#ifdef TEST_MODE
    if (fout.IsOpened())
	fout.Detach();
#else
    if (fout.IsOpened())
	fout.Close();
#endif
    ETCB();
}

void Logger::log(ACE_Log_Record & log_record)
{
    BTCB();
    FT("Logger::log", ("(ACE_Log_Record &) log_record"));

    if (!Magick::instance_exists())
	return;

    if (!fout.IsOpened())
	return;

    mstring text_priority;

    switch (log_record.type())
    {

	// Stuff normal users dont wanna see
    case LM_TRACE:
#ifndef DEBUG
	return;
#endif
	text_priority = "TRACE";
	break;

	// Stuff you have to turn VERBOSE on to see
    case LM_DEBUG:
	if (!Magick::instance().Verbose())
	    return;
	text_priority = "DEBUG";
	break;

	// Normal information that most users want
    case LM_INFO:
	text_priority = "INFO";
	break;

	// Still normal, but notable (eg. SOP commands)
    case LM_NOTICE:
	text_priority = "NOTICE";
	break;

	// An attempt at data corruption (denied) or recoverable error
    case LM_WARNING:
	text_priority = "WARNING";
	break;

	// Startup messages only (ie. boot)
    case LM_STARTUP:
	text_priority = "STARTUP";
	break;

	// A data corruption that we repaird
    case LM_ERROR:
	text_priority = "ERROR";
	break;

	// A data corruption we could not repair (left in system or
	// more drastic action taken, eg. the record being removed)
    case LM_CRITICAL:
	text_priority = "CRITICAL";
	break;

	// A situation that caused us to hard abort, but did not kill
	// us but may have caused a thread to die, etc.
    case LM_ALERT:
	text_priority = "ALERT";
	break;

	// A situation we could not reover from, we're outtahere.
    case LM_EMERGENCY:
	text_priority = "FATAL";
	break;

	// Should NEVER get this, but its there for completeness.
    default:
	text_priority = "UNKNOWN";
	break;
    }

    /* Pulled directly from ACE ... */
    mDateTime ts(log_record.time_stamp().sec(), log_record.time_stamp().usec());

    mstring out;

    out.Format("%s | %-8s | ", ts.FormatString("dd mmm yyyy hh:nn:ss.uuu").c_str(), text_priority.c_str());
    mstring tmp(log_record.msg_data());

    unsigned int i;

    for (i = 1; i <= tmp.WordCount("\n\r"); i++)
	fout.Write(out + tmp.ExtractWord(i, "\n\r"));

    fout.Flush();

    if (!Magick::instance().files.Logchan().empty() && Magick::instance().Connected())
    {
	{
	    RLOCK((lck_IrcSvcHandler));
	    if (Magick::instance().ircsvchandler != NULL && !Magick::instance().ircsvchandler->Burst())
	    {
		if (Magick::instance().chanserv.IsLive(Magick::instance().files.Logchan()))
		{
		    for (i = 1; i <= tmp.WordCount("\n\r"); i++)
			Magick::instance().server.PRIVMSG(Magick::instance().operserv.FirstName(),
							  Magick::instance().files.Logchan(), out + tmp.ExtractWord(i,
														    "\n\r"));
		}
	    }
	}
    }

    if (log_record.type() == LM_EMERGENCY)
	exit(MAGICK_RET_ERROR);
    ETCB();
}

void Logger::close()
{
    BTCB();
    NFT("Logger::close");

    if (fout.IsOpened())
    {
	fout.Close();
    }
    ETCB();
}

void Logger::open()
{
    BTCB();
    NFT("Logger::open");

    if (!fout.IsOpened())
    {
	fout.Open(Magick::instance().files.Logfile(), "a");
    }
    ETCB();
}

bool Logger::opened() const
{
    BTCB();
    NFT("Logger::opened");
    bool retval = fout.IsOpened();

    RET(retval);
    ETCB();
}

void LOG2(ACE_Log_Priority type, const mstring & msg)
{
    BTCB();
    if (Magick::instance_exists() && Magick::instance().ValidateLogger(ACE_LOG_MSG))
    {
	ACE_DEBUG((type, (const ACE_TCHAR *) msg));
	if (ACE_LOG_MSG != NULL && ACE_LOG_MSG->flags() & ACE_Log_Msg::STDERR)
	    fprintf(stderr, "\n");
    }
    ETCB();
}
