#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
static const char *ident = "@(#) $Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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

//
// Tracing functions -- Include making TraceMap's and
// receiving all trace information.

#include "trace.h"
#include "lockable.h"
#include "magick.h"

mstring threadname[tt_MAX] = { "", "NS", "CS", "MS", "OS", "XS", "NET", "SCRIPT", "MBASE", "LOST" };
Trace::level_enum Trace::SLevel = Off;
unsigned short Trace::traces[tt_MAX] = { 0, 0, 0, 0, 0, 0, 0, 0 };
vector<Trace::levelname_struct> Trace::levelname; // Initialised in main.cpp
list<pair<threadtype_enum, mstring> > ThreadMessageQueue;

int levelname_count()
{
    return sizeof(Trace::levelname)/sizeof(Trace::levelname_struct);
}

unsigned short makehex (mstring SLevel)
{
    if (SLevel.Len() != 6 || SLevel[0u]!='0' ||
		    (SLevel[1u]!='x' && SLevel[1u]!='X'))
	return 0;

    short level;
    for (unsigned int j=2; j<6; j++) {
	switch(SLevel[j]) {
	case 'F':
	case 'f':
	    switch(j) {
	    case 5: level |= 0x000f; break;
	    case 4: level |= 0x00f0; break;
	    case 3: level |= 0x0f00; break;
	    case 2: level |= 0xf000; break;
	    } break;
	case 'E':
	case 'e':
	    switch(j) {
	    case 5: level |= 0x000e; break;
	    case 4: level |= 0x00e0; break;
	    case 3: level |= 0x0e00; break;
	    case 2: level |= 0xe000; break;
	    } break;
	case 'D':
	case 'd':
	    switch(j) {
	    case 5: level |= 0x000d; break;
	    case 4: level |= 0x00d0; break;
	    case 3: level |= 0x0d00; break;
	    case 2: level |= 0xd000; break;
	    } break;
	case 'C':
	case 'c':
	    switch(j) {
	    case 5: level |= 0x000c; break;
	    case 4: level |= 0x00c0; break;
	    case 3: level |= 0x0c00; break;
	    case 2: level |= 0xc000; break;
	    } break;
	case 'B':
	case 'b':
	    switch(j) {
	    case 5: level |= 0x000b; break;
	    case 4: level |= 0x00b0; break;
	    case 3: level |= 0x0b00; break;
	    case 2: level |= 0xb000; break;
	    } break;
	case 'A':
	case 'a':
	    switch(j) {
	    case 5: level |= 0x000a; break;
	    case 4: level |= 0x00a0; break;
	    case 3: level |= 0x0a00; break;
	    case 2: level |= 0xa000; break;
	    } break;
	case '9':
	    switch(j) {
	    case 5: level |= 0x0009; break;
	    case 4: level |= 0x0090; break;
	    case 3: level |= 0x0900; break;
	    case 2: level |= 0x9000; break;
	    } break;
	case '8':
	    switch(j) {
	    case 5: level |= 0x0008; break;
	    case 4: level |= 0x0080; break;
	    case 3: level |= 0x0800; break;
	    case 2: level |= 0x8000; break;
	    } break;
	case '7':
	    switch(j) {
	    case 5: level |= 0x0007; break;
	    case 4: level |= 0x0070; break;
	    case 3: level |= 0x0700; break;
	    case 2: level |= 0x7000; break;
	    } break;
	case '6':
	    switch(j) {
	    case 5: level |= 0x0006; break;
	    case 4: level |= 0x0060; break;
	    case 3: level |= 0x0600; break;
	    case 2: level |= 0x6000; break;
	    } break;
	case '5':
	    switch(j) {
	    case 5: level |= 0x0005; break;
	    case 4: level |= 0x0050; break;
	    case 3: level |= 0x0500; break;
	    case 2: level |= 0x5000; break;
	    } break;
	case '4':
	    switch(j) {
	    case 5: level |= 0x0004; break;
	    case 4: level |= 0x0040; break;
	    case 3: level |= 0x0400; break;
	    case 2: level |= 0x4000; break;
	    } break;
	case '3':
	    switch(j) {
	    case 5: level |= 0x0003; break;
	    case 4: level |= 0x0030; break;
	    case 3: level |= 0x0300; break;
	    case 2: level |= 0x3000; break;
	    } break;
	case '2':
	    switch(j) {
	    case 5: level |= 0x0002; break;
	    case 4: level |= 0x0020; break;
	    case 3: level |= 0x0200; break;
	    case 2: level |= 0x2000; break;
	    } break;
	case '1':
	    switch(j) {
	    case 5: level |= 0x0001; break;
	    case 4: level |= 0x0010; break;
	    case 3: level |= 0x0100; break;
	    case 2: level |= 0x1000; break;
	    } break;
	case '0':
	    break;
	default:
	    return 0;
	}
    }
    return level;
}

// ===================================================

ThreadID::ThreadID()
{
    t_indent = 0;
    t_internaltype = tt_MAIN;
}

ThreadID::ThreadID(threadtype_enum Type)
{
    t_indent = 0;
    t_internaltype = Type;
}

void ThreadID::assign(threadtype_enum Type)
{
    t_internaltype = Type;
}

void ThreadID::WriteOut(const mstring &message)
{
    mstring finalout = "";
    for (int i=0; i<t_indent; i++)
        finalout += ".  ";
    finalout += message;

    list<pair<threadtype_enum, mstring> >::iterator iter;
    if(Parent!=NULL)
    {
	for (iter=ThreadMessageQueue.begin(); iter!=ThreadMessageQueue.end(); iter++)
	    if (iter->first == t_internaltype)
	    {
		Parent->loggertask->logmessage(t_internaltype, iter->second);
		ThreadMessageQueue.erase(iter);
		iter = ThreadMessageQueue.begin();
	    }
	Parent->loggertask->logmessage(t_internaltype,finalout);
    }
}

// ===================================================

//      \  function()
T_Functions::T_Functions(const mstring &name)
{
    m_name=name;
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(Functions);
    if (IsOn(tid)) {
	mstring message = "\\  " + m_name + "()";
	tid->WriteOut(message);
    }
    tid->indentup();
}

//      \  function( (char) T, (int) 5 )
T_Functions::T_Functions(const mstring &name, const mVarArray &args)
{
    m_name=name;
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(Functions);
    if (IsOn(tid)) {
	mstring message = "\\  " + m_name + "(";
	for (int i=0; i<args.count(); i++) {
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
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    tid->indentdown(); 
    ShortLevel(Functions);
    if (IsOn(tid)) {
	mstring message;
	if (!return_value.type().IsEmpty())
		message="/  (" + return_value.type() + ") " + return_value.AsString();
	else
		message="/ ";
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
    va_list args;
    va_start (args, fmt);

    mstring output;
    output.FormatV(fmt, args);
    common(output.c_str());
}

void T_CheckPoint::common(const char *input)
{
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(CheckPoint);
    if (IsOn(tid)) {
	mstring message;
	message << "** " << input;
	tid->WriteOut(message);
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
    va_start (args, fmt);

    mstring output;
    output.FormatV(fmt, args);
    common(output.c_str());
}

void T_Comments::common(const char *input)
{
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(Comments);
    if (IsOn(tid)) {
	mstring message;
	message << "## " << input;
	tid->WriteOut(message);
    }
}

// ===================================================

//      << DE1(PreZ)
//      << DE2(prez)
//      << DE3(srealm.net.au)
T_Modify::T_Modify(const mVarArray &args)
{
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(Modify);
    if (IsOn(tid)) {
	for (int i=0; i<args.count(); i++) {
	    mstring message;
	    message << "<< " << "DE" << i+1 << "(" << args[i].AsString() << ")";
	    tid->WriteOut(message);
	}
    }
}

//      >> DE1(PreZ)
//      >> DE2(prez)
//      >> DE3(corewars.net)
void T_Modify::End(const mVarArray &args)
{
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(Modify);
    if (IsOn(tid)) {
	for (int i=0; i<args.count(); i++) {
	    mstring message;
	    message << ">> " << "DE" << i+1 << "(" << args[i].AsString() << ")";
	    tid->WriteOut(message);
	}
    }
}

// ===================================================

//      <- PreZ :PRIVMSG ChanServ :help blah
//      -> ChanServ :PRIVMSG PreZ :Unknown command 'blah'
//      -- ChanServ :PRIVMSG ChanServ :WTF?!
T_Chatter::T_Chatter(dir_enum direction, const mstring &input)
{
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(Chatter);
    if (IsOn(tid)) {
	mstring message;
	if (direction == From)
	    message << "<- " << input;
	else if(direction == To)
	    message << "-> " << input;
	else
	    message << "-- " << input; // Confused
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
void T_Locking::open(T_Locking::type_enum ltype, mstring lockname) 
{
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(Locking);
    if (IsOn(tid)) 
    {
	locktype = ltype;
	name = lockname;
	mstring message;
	if(locktype == Read)
	    message << ":+ " << "R " << name;
	else if(locktype == Write)
	    message << ":+ " << "W " << name;
	else
	    message << ":+ " << "M " << name;
	tid->WriteOut(message);
    }
}

//      :- R ChanInfo::#Magick
//      :- W NickInfo::PreZ::Flags
//      :- M Magick::LoadMessages

T_Locking::~T_Locking()
{
    // Prez: should this be an assign or a compare? if it's an assign, assign it,
    //   then compare it, not both at once, it's bad to read.
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(Locking);
    if (IsOn(tid)) {
	if (!name.IsEmpty())
	{
    	    mstring message;
	    if(locktype == Read)
	        message << ":- " << "R " << name;
	    else if(locktype == Write)
		message << ":- " << "W " << name;
	    else
		message << ":- " << "M " << name;
	    tid->WriteOut(message);
	}
    }
}

// ===================================================

//      |+ 3: 2478 / 6667 (203.30.145.2)
//      || 3: Server (styx.us.relic.net)
//      || 3: Telnet (Ungod)
//      || 3: DCC (PreZ)
//      || 3: ServNet (2)
//      |- 3: Socket Timeout

// T_Sockets::T_Sockets() {}
/*
T_Sockets::T_Sockets(unsigned int local, unsigned int remote, mstring host)
{
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(Sockets);
    if (IsOn(tid)) 
    {
	mstring message;
	message << "|+ " << local << " / " << remote << " (" << host << ")";
	tid->WriteOut(message);
    }
}

T_Sockets::End(mstring reason)
{
    if (!(tid = mThread::find()))
	return; // should throw an exception later
    ShortLevel(Sockets);
    if (IsOn(tid)) 
    {
	mstring message;
	message << "|- " << reason;
	tid->WriteOut(message);
    }
}
*/

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

// ===================================================

int LoggerTask::open(void *in)
{
    return activate();
}
int LoggerTask::close(unsigned long in)
{
    // dump all and close open file handles.
    return 0;
}

int LoggerTask::svc(void)
{
    // main service routine
    //this->activation_queue_.enqueue(new logMsg_MO(this,msg,resultant_future));
    while(1)
    {
	// Destructor automatically deletes it.
	try
	{
	    auto_ptr<ACE_Method_Object> mo(this->activation_queue_.dequeue ());
	    // Call it.
	    if (Parent->Shutdown())
		return -1;

	    if (mo->call () == -1)
		break;
	}
	catch(...)
	{
	    return 0;
	}
    }
    return 0;
}

void LoggerTask::i_shutdown()
{
    activation_queue_.enqueue(new shutdown_MO);
}

void LoggerTask::logmessage(threadtype_enum type,const mstring& data)
{
    activation_queue_.enqueue(new LoggerTask_logmessage_MO(this,type,data));
}

mstring LoggerTask::logname(threadtype_enum type)
{
    mstring name;
    if (type==tt_MAIN)
	name << "trace.log";
    else
	name << "trace_" << threadname[type] << ".log";
    return name;
}

void LoggerTask::logmessage_i(threadtype_enum type,const mstring& data)
{
    wxFileOutputStream out(Parent->Services_Dir()+DirSlash+logname(type),true); // true sets append to true.
    out.Write(data.c_str(), data.length());
    out << wxEndL;
}
