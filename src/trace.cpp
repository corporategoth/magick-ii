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
	tmap[levelpair(MAIN,Off)]		= Off;
	tmap[levelpair(MAIN,Stats)]		= G_Stats;
	tmap[levelpair(MAIN,SourcrFiles)]	= G_SourceFiles;
	tmap[levelpair(MAIN,Functions)]		= G_Functions;
	tmap[levelpair(MAIN,Locking)]		= G_Locking;

	tmap[levelpair(NickServ,Off)]		= Off;
	tmap[levelpair(NickServ,Chatter)]	= NS_Chatter;
	tmap[levelpair(NickServ,CheckPoint)]	= NS_CheckPoint;
	tmap[levelpair(NickServ,Functions)]	= NS_Functions;
	tmap[levelpair(NickServ,Modify)]	= NS_Modify;

	tmap[levelpair(ChanServ,Off)]		= Off;
	tmap[levelpair(ChanServ,Chatter)]	= CS_Chatter;
	tmap[levelpair(ChanServ,CheckPoint)]	= CS_CheckPoint;
	tmap[levelpair(ChanServ,Functions)]	= CS_Functions;
	tmap[levelpair(ChanServ,Modify)]	= CS_Modify;

	tmap[levelpair(MemoServ,Off)]		= Off;
	tmap[levelpair(MemoServ,Chatter)]	= MS_Chatter;
	tmap[levelpair(MemoServ,CheckPoint)]	= MS_CheckPoint;
	tmap[levelpair(MemoServ,Functions)]	= MS_Functions;
	tmap[levelpair(MemoServ,Modify)]	= MS_Modify;

	tmap[levelpair(OperServ,Off)]		= Off;
	tmap[levelpair(OperServ,Chatter)]	= OS_Chatter;
	tmap[levelpair(OperServ,CheckPoint)]	= OS_CheckPoint;
	tmap[levelpair(OperServ,Functions)]	= OS_Functions;
	tmap[levelpair(OperServ,Modify)]	= OS_Modify;

	tmap[levelpair(OtherServ,Off)]		= Off;
	tmap[levelpair(OtherServ,Chatter)]	= XS_Chatter;
	tmap[levelpair(OtherServ,CheckPoint)]	= XS_CheckPoint;
	tmap[levelpair(OtherServ,Functions)]	= XS_Functions;
	tmap[levelpair(OtherServ,Modify)]	= XS_Modify;

	tmap[levelpair(ServNet,Off)]		= Off;
	tmap[levelpair(ServNet,Chatter)]	= NET_Chatter;
	tmap[levelpair(ServNet,CheckPoint)]	= NET_CheckPoint;
	tmap[levelpair(ServNet,Functions)]	= NET_Functions;
	tmap[levelpair(ServNet,Sockets)]	= NET_Sockets;

	tmap[levelpair(BOB,Off)]		= Off;
	tmap[levelpair(BOB,Chatter)]		= BOB_Chatter;
	tmap[levelpair(BOB,Bind)]		= BOB_Bind;
	tmap[levelpair(BOB,Functions)]		= BOB_Functions;
	tmap[levelpair(BOB,External)]		= BOB_External;
}

Trace::~Trace()
{
}

ThreadID::ThreadID()
{
    init();
}

ThreadID::ThreadID(short Type, int Number)
{
    init();
    type = Type;
    number = Number;
}

ThreadID::init()
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

ThreadID ThreadID::assign(short Type, int Number)
{
    type = Type;
    number = Number;
    return *this;
}

mstring ThreadID::logname()
{
    mstring name;
    name << "trace" << logtext[type] << number << ".log";
    return name;
}

void ThreadID::WriteOut(short level, mstring &message)
{
    mstring finalout;
    int i;

    if (IsOn(level, this)) {
	if (!out) out.open(logname(), ios::app);
	assert(out);
	for (i=0; i<indent; i++)
	    finalout += "  ";
	finalout += message;
	out << finalout;
    }
}

FuncTrace::FuncTrace(const mstring &name, mVarArray &args) {
    tid.indentup();

    if (IsOn(Functions, tid)) {
	mstring message = name + "(";
	for (int i=0; i<args.count(); i++) {
	    message += " (" + args[i].type() + ") " + args[i].AsString();
	    if (i < args.count() - 1)
		message += ", ";
	}
	message += " )";

	WriteOut(Functions, tid, message);
    }
}
