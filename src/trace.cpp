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
mstring threadname[MAX] = { "", "NS", "CS", "MS", "OS", "XS", "NET", "BOB" };


Trace::Trace()
{
	SLevel = Off;

	tmap[levelpair(MAIN,Off)]		= TT_Off;
	tmap[levelpair(MAIN,Stats)]		= G_Stats;
	tmap[levelpair(MAIN,Source)]		= G_SourceFiles;
	tmap[levelpair(MAIN,Functions)]		= G_Functions;
	tmap[levelpair(MAIN,Locking)]		= G_Locking;

	tmap[levelpair(NickServ,Off)]		= TT_Off;
	tmap[levelpair(NickServ,Chatter)]	= NS_Chatter;
	tmap[levelpair(NickServ,CheckPoint)]	= NS_CheckPoint;
	tmap[levelpair(NickServ,Functions)]	= NS_Functions;
	tmap[levelpair(NickServ,Modify)]	= NS_Modify;

	tmap[levelpair(ChanServ,Off)]		= TT_Off;
	tmap[levelpair(ChanServ,Chatter)]	= CS_Chatter;
	tmap[levelpair(ChanServ,CheckPoint)]	= CS_CheckPoint;
	tmap[levelpair(ChanServ,Functions)]	= CS_Functions;
	tmap[levelpair(ChanServ,Modify)]	= CS_Modify;

	tmap[levelpair(MemoServ,Off)]		= TT_Off;
	tmap[levelpair(MemoServ,Chatter)]	= MS_Chatter;
	tmap[levelpair(MemoServ,CheckPoint)]	= MS_CheckPoint;
	tmap[levelpair(MemoServ,Functions)]	= MS_Functions;
	tmap[levelpair(MemoServ,Modify)]	= MS_Modify;

	tmap[levelpair(OperServ,Off)]		= TT_Off;
	tmap[levelpair(OperServ,Chatter)]	= OS_Chatter;
	tmap[levelpair(OperServ,CheckPoint)]	= OS_CheckPoint;
	tmap[levelpair(OperServ,Functions)]	= OS_Functions;
	tmap[levelpair(OperServ,Modify)]	= OS_Modify;

	tmap[levelpair(OtherServ,Off)]		= TT_Off;
	tmap[levelpair(OtherServ,Chatter)]	= XS_Chatter;
	tmap[levelpair(OtherServ,CheckPoint)]	= XS_CheckPoint;
	tmap[levelpair(OtherServ,Functions)]	= XS_Functions;
	tmap[levelpair(OtherServ,Modify)]	= XS_Modify;

	tmap[levelpair(ServNet,Off)]		= TT_Off;
	tmap[levelpair(ServNet,Chatter)]	= NET_Chatter;
	tmap[levelpair(ServNet,CheckPoint)]	= NET_CheckPoint;
	tmap[levelpair(ServNet,Functions)]	= NET_Functions;
	tmap[levelpair(ServNet,Sockets)]	= NET_Sockets;

	tmap[levelpair(BOB,Off)]		= TT_Off;
	tmap[levelpair(BOB,Chatter)]		= BOB_Chatter;
	tmap[levelpair(BOB,Bind)]		= BOB_Bind;
	tmap[levelpair(BOB,Functions)]		= BOB_Functions;
	tmap[levelpair(BOB,External)]		= BOB_External;

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
// prez: with this the NULL is unneeded, besides, you can't have a NULL static
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

T_Functions::T_Functions(const mstring &name, const mVarArray &args)
{
    ShortLevel(Functions);
    m_name=name;
    if (IsOn(tid)) {
	mstring message = "Entering: " + m_name + "(";
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

T_Functions::~T_Functions()
{ 
    mstring message="Leaving: "+m_name+"()";
    tid->WriteOut(message);
    tid->indentdown(); 
}

// ===================================================

T_CheckPoint::T_CheckPoint()
{
    common("T_CheckPoint Reached");
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
    ShortLevel(Trace::CheckPoint);
    if (IsOn(tid)) {
	mstring message;
	message << "** " << input;
	tid->WriteOut(message);
    }
}

// ===================================================

T_Modify::T_Modify(const mVarArray &args)
{
    ShortLevel(Trace::Modify);
    if (IsOn(tid)) {
	for (int i=0; i<args.count(); i++) {
	    mstring message;
	    message << "<< " << "DE" << i+1 << "(" << args[i].AsString() << ")";
	    tid->WriteOut(message);
	}
    }
}

void T_Modify::End(const mVarArray &args)
{
    ShortLevel(Trace::Modify);
    if (IsOn(tid)) {
	for (int i=0; i<args.count(); i++) {
	    mstring message;
	    message << ">> " << "DE" << i+1 << "(" << args[i].AsString() << ")";
	    tid->WriteOut(message);
	}
    }
}

// ===================================================

T_Chatter::T_Chatter(dir_enum direction, const mstring &input)
{
    ShortLevel(Trace::Chatter);
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

// T_Stats::T_Stats() {}

// ===================================================

// T_Locking::T_Locking() {}

// ===================================================

// T_Sockets::T_Sockets() {}

// ===================================================

// T_Bind::T_Bind() {}

// ===================================================

// T_External::T_External() {}

