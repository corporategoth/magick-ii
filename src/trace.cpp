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
//
// Tracing functions -- Include making TraceMap's and
// receiving all trace information.

#include "trace.h"

long Trace::TraceLevel=0;
mstring threadname[tt_MAX] = { "", "NS", "CS", "MS", "OS", "XS", "NET", "BOB" };


Trace::Trace()
{
	SLevel = Off;

	tmap[levelpair(tt_MAIN,Off)]		= TT_Off;
	tmap[levelpair(tt_MAIN,Stats)]		= G_Stats;
	tmap[levelpair(tt_MAIN,Source)]		= G_SourceFiles;
	tmap[levelpair(tt_MAIN,Functions)]	= G_Functions;
	tmap[levelpair(tt_MAIN,Locking)]	= G_Locking;

	tmap[levelpair(tt_NickServ,Off)]	= TT_Off;
	tmap[levelpair(tt_NickServ,Chatter)]	= NS_Chatter;
	tmap[levelpair(tt_NickServ,CheckPoint)]	= NS_CheckPoint;
	tmap[levelpair(tt_NickServ,Functions)]	= NS_Functions;
	tmap[levelpair(tt_NickServ,Modify)]	= NS_Modify;

	tmap[levelpair(tt_ChanServ,Off)]	= TT_Off;
	tmap[levelpair(tt_ChanServ,Chatter)]	= CS_Chatter;
	tmap[levelpair(tt_ChanServ,CheckPoint)]	= CS_CheckPoint;
	tmap[levelpair(tt_ChanServ,Functions)]	= CS_Functions;
	tmap[levelpair(tt_ChanServ,Modify)]	= CS_Modify;

	tmap[levelpair(tt_MemoServ,Off)]	= TT_Off;
	tmap[levelpair(tt_MemoServ,Chatter)]	= MS_Chatter;
	tmap[levelpair(tt_MemoServ,CheckPoint)]	= MS_CheckPoint;
	tmap[levelpair(tt_MemoServ,Functions)]	= MS_Functions;
	tmap[levelpair(tt_MemoServ,Modify)]	= MS_Modify;

	tmap[levelpair(tt_OperServ,Off)]	= TT_Off;
	tmap[levelpair(tt_OperServ,Chatter)]	= OS_Chatter;
	tmap[levelpair(tt_OperServ,CheckPoint)]	= OS_CheckPoint;
	tmap[levelpair(tt_OperServ,Functions)]	= OS_Functions;
	tmap[levelpair(tt_OperServ,Modify)]	= OS_Modify;

	tmap[levelpair(tt_OtherServ,Off)]	= TT_Off;
	tmap[levelpair(tt_OtherServ,Chatter)]	= XS_Chatter;
	tmap[levelpair(tt_OtherServ,CheckPoint)]= XS_CheckPoint;
	tmap[levelpair(tt_OtherServ,Functions)]	= XS_Functions;
	tmap[levelpair(tt_OtherServ,Modify)]	= XS_Modify;

	tmap[levelpair(tt_ServNet,Off)]		= TT_Off;
	tmap[levelpair(tt_ServNet,Chatter)]	= NET_Chatter;
	tmap[levelpair(tt_ServNet,CheckPoint)]	= NET_CheckPoint;
	tmap[levelpair(tt_ServNet,Functions)]	= NET_Functions;
	tmap[levelpair(tt_ServNet,Sockets)]	= NET_Sockets;

	tmap[levelpair(tt_BOB,Off)]		= TT_Off;
	tmap[levelpair(tt_BOB,Chatter)]		= BOB_Chatter;
	tmap[levelpair(tt_BOB,Bind)]		= BOB_Bind;
	tmap[levelpair(tt_BOB,Functions)]	= BOB_Functions;
	tmap[levelpair(tt_BOB,External)]	= BOB_External;

}
//todo change this to a vector< >
const struct Trace::levelname_struct Trace::levelname[] = {
	levelname_struct( "OFF", Off ),
	levelname_struct( "STAT*", Stats ),
	levelname_struct( "SOUR*", Source ),
	levelname_struct( "SRC*", Source ),
	levelname_struct( "L*CK*", Locking ),
	levelname_struct( "S*CK*", Sockets ),
	levelname_struct( "BIND*", Bind ),
	levelname_struct( "REG*", Bind ),
	levelname_struct( "HOOK*", Bind ),
	levelname_struct( "EXT*", External ),
	levelname_struct( "CHAT*", Chatter ),
	levelname_struct( "CHE*", CheckPoint ),
	levelname_struct( "C*P*", CheckPoint ),
	levelname_struct( "F*NC*", Functions ),
	levelname_struct( "MOD*", Modify ) };


int levelname_count()
{
    return sizeof(Trace::levelname)/sizeof(Trace::levelname_struct);
}

Trace::~Trace() {}

Trace::TraceTypes Trace::resolve(Trace::level_enum level, threadtype_enum type)
{ 
	return tmap[levelpair(type,level)]; 
}
Trace::TraceTypes Trace::resolve(Trace::level_enum level, ThreadID *tid)
{ 
	return resolve(level, tid->type());
}
Trace::TraceTypes Trace::resolve(threadtype_enum type)
{ 
	return resolve(SLevel, type);
}
Trace::TraceTypes Trace::resolve(ThreadID *tid)
{ 
	return resolve(SLevel, tid->type());
}


// ===================================================

ThreadID::ThreadID()
{
    indent = 0;
}

ThreadID::ThreadID(threadtype_enum Type, int Number)
{
    indent = 0;
    internaltype = Type;
    number = Number;
}

ThreadID ThreadID::assign(threadtype_enum Type, int Number)
{
    internaltype = Type;
    number = Number;
    return *this;
}

mstring ThreadID::logname()
{
    mstring name;
	if (internaltype)
	    name << "trace_" << threadname[internaltype] << "_" << number << ".log";
	else
	    name << "trace.log";
    return name;
}

void ThreadID::WriteOut(const mstring &message)
{
    //below for now till i get the operator bool happening.
    if (out.LastError()!=wxStream_NOERROR) 
    {
        out=wxFileOutputStream(logname());
        out.SeekO(0,wxFromEnd);
    }
    assert(out.LastError()==wxStream_NOERROR);
    mstring finalout;
    for (int i=0; i<indent; i++)
        finalout += "  ";
    finalout += message;
    out << finalout << wxEndL;
}

// ===================================================

//      \\ function( (char) T, (int) 5 )
T_Functions::T_Functions(const mstring &name, const mVarArray &args)
{
    ShortLevel(Functions);
    m_name=name;
    if (IsOn(tid)) {
	mstring message = "\\\\ " + m_name + "(";
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

//      // (char) Y
T_Functions::~T_Functions()
{ 

    ShortLevel(Functions);
    tid->indentdown(); 

    if (IsOn(tid)) {
	mstring message="// (" + return_value.type() + ") " + return_value.AsString();
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
    ShortLevel(CheckPoint);
    if (IsOn(tid)) {
	mstring message;
	message << "** " << input;
	tid->WriteOut(message);
    }
}

// ===================================================

//      << DE1(PreZ)
//      << DE2(prez)
//      << DE3(srealm.net.au)
T_Modify::T_Modify(const mVarArray &args)
{
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

T_Locking::~T_Locking() {

    ShortLevel(Locking);

    if (IsOn(tid)) {

	if (name) {

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
//      |- 3 (Socket Timeout)

// T_Sockets::T_Sockets() {}

// ===================================================

//      !! VAR+ added_userinfo
//      !! VAR- added_userinfo
//      !! FUNC+ user_process( (char *) source, (char *) message )
//      !! FUNC- user_process( (char *) source, (char *) message )
//      !! BIND+ MSG "SET USERINFO" user_process
//      !! BIND- MSG "SET USERINFO" user_process

// T_Bind::T_Bind() {}

// ===================================================

//      ?? EXEC    ls /etc >/tmp/etcls
//      ?? OPEN(R) /tmp/etcls
//      ?? OPEN(W) /tmp/output
//      ?? CLOSE   /tmp/output

// T_External::T_External() {}

