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

	levelname = {{ "OFF", Off },
		{ "STAT*", Stats },
		{ "SOUR*", Source },
		{ "SRC*", Source },
		{ "L*CK*", Locking },
		{ "S*CK*", Sockets },
		{ "BIND*", Bind },
		{ "REG*", Bind },
		{ "HOOK*", Bind },
		{ "EXT*", External },
		{ "CHAT*", Chatter },
		{ "CHE*", CheckPoint },
		{ "C*P*", CheckPoint },
		{ "F*NC*", Functions },
		{ "MOD*", Modify }};

}

Trace::~Trace()
{}

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
    init();
}

ThreadID::ThreadID(threadtype_enum Type, int Number)
{
    init();
    internaltype = Type;
    number = Number;
}

void ThreadID::init()
{
    indent = 0;
    logtext[MAIN]	= "";
    logtext[NickServ]	= "_NS_";
    logtext[ChanServ]	= "_CS_";
    logtext[MemoServ]	= "_MS_";
    logtext[OperServ]	= "_OS_";
    logtext[OtherServ]	= "_XS_";
    logtext[ServNet]	= "_NET_";
    logtext[BOB]	= "_BOB_";
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
    name << "trace" << logtext[internaltype] << number << ".log";
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
    out << finalout;
}

// ===================================================

FuncTrace::FuncTrace(const mstring &name, const mVarArray &args)
{
    ShortLevel(Functions);
    if (IsOn(tid)) {
	mstring message = "` " + name + "(";
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

// ===================================================

CheckPoint::CheckPoint()
{
    common("CheckPoint Reached");
}

CheckPoint::CheckPoint(const char *fmt, ...)
{
    va_list args;
    va_start (args, fmt);

    mstring output;
    output.FormatV(fmt, args);
    common(output.c_str());
}

void CheckPoint::common(const char *input)
{
    ShortLevel(CheckPoint);
    if (IsOn(tid)) {
	mstring message;
	message << "** " << input;
	tid->WriteOut(message);
    }
}

// ===================================================

Modify::Modify(const mVarArray &args)
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

Modify::EndModify(const mVarArray &args)
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

Chatter::Chatter(dir_enum direction, const mstring &input)
{
    ShortLevel(Chatter);
    if (IsOn(tid)) {
	mstring message;
	if (direction == From)
	    message << "<- " << input;
	else
	    message << "-> " << input;
	tid->WriteOut(message);
    }
}

// ===================================================

