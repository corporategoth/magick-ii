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
	tmap[levelpair(MAIN,LT_Off)]		= TT_Off;
	tmap[levelpair(MAIN,Stats)]		= G_Stats;
	tmap[levelpair(MAIN,Source)]	= G_SourceFiles;
	tmap[levelpair(MAIN,Functions)]		= G_Functions;
	tmap[levelpair(MAIN,Locking)]		= G_Locking;

	tmap[levelpair(NickServ,LT_Off)]		= TT_Off;
	tmap[levelpair(NickServ,Chatter)]	= NS_Chatter;
	tmap[levelpair(NickServ,CheckPoint)]	= NS_CheckPoint;
	tmap[levelpair(NickServ,Functions)]	= NS_Functions;
	tmap[levelpair(NickServ,Modify)]	= NS_Modify;

	tmap[levelpair(ChanServ,LT_Off)]		= TT_Off;
	tmap[levelpair(ChanServ,Chatter)]	= CS_Chatter;
	tmap[levelpair(ChanServ,CheckPoint)]	= CS_CheckPoint;
	tmap[levelpair(ChanServ,Functions)]	= CS_Functions;
	tmap[levelpair(ChanServ,Modify)]	= CS_Modify;

	tmap[levelpair(MemoServ,LT_Off)]		= TT_Off;
	tmap[levelpair(MemoServ,Chatter)]	= MS_Chatter;
	tmap[levelpair(MemoServ,CheckPoint)]	= MS_CheckPoint;
	tmap[levelpair(MemoServ,Functions)]	= MS_Functions;
	tmap[levelpair(MemoServ,Modify)]	= MS_Modify;

	tmap[levelpair(OperServ,LT_Off)]		= TT_Off;
	tmap[levelpair(OperServ,Chatter)]	= OS_Chatter;
	tmap[levelpair(OperServ,CheckPoint)]	= OS_CheckPoint;
	tmap[levelpair(OperServ,Functions)]	= OS_Functions;
	tmap[levelpair(OperServ,Modify)]	= OS_Modify;

	tmap[levelpair(OtherServ,LT_Off)]		= TT_Off;
	tmap[levelpair(OtherServ,Chatter)]	= XS_Chatter;
	tmap[levelpair(OtherServ,CheckPoint)]	= XS_CheckPoint;
	tmap[levelpair(OtherServ,Functions)]	= XS_Functions;
	tmap[levelpair(OtherServ,Modify)]	= XS_Modify;

	tmap[levelpair(ServNet,LT_Off)]		= TT_Off;
	tmap[levelpair(ServNet,Chatter)]	= NET_Chatter;
	tmap[levelpair(ServNet,CheckPoint)]	= NET_CheckPoint;
	tmap[levelpair(ServNet,Functions)]	= NET_Functions;
	tmap[levelpair(ServNet,Sockets)]	= NET_Sockets;

	tmap[levelpair(BOB,LT_Off)]		= TT_Off;
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

void ThreadID::WriteOut(Trace::level_enum level, mstring &message)
{
    mstring finalout;
    int i;

    if (IsOn(level, this)) 
	{
		//below for now till i get the operator bool happening.
		if (out.LastError()!=wxStream_NOERROR) 
		{
			out=wxFileOutputStream(logname());
			out.SeekO(0,wxFromEnd);
		}
		assert(out.LastError()==wxStream_NOERROR);
		for (i=0; i<indent; i++)
			finalout += "  ";
		finalout += message;
		out << finalout;
    }
}

FuncTrace::FuncTrace(const mstring &name, const mVarArray &args)
{
    tid->indentup();

    if (IsOn(Functions, tid)) {
	mstring message = name + "(";
	for (int i=0; i<args.count(); i++) {
	    message += " (" + args[i].type() + ") " + args[i].AsString();
	    if (i < args.count() - 1)
		message += ", ";
	}
	message += " )";

	// write out from where? either declare it in this object, or use tid->WriteOut(
	//WriteOut(Functions, tid, message);
    }
}

// enum is declared inside object so to access enum type it's objecttype::enumtype
Trace::TraceTypes Trace::Resolve(Trace::level_enum level, ThreadID *tid)
{ 
	return tmap[levelpair(tid->type(),level)]; 
}
