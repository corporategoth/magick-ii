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

Trace::Trace()
{
}

Trace::~Trace()
{
}


bool Trace::IsOn(long level)
{
    return (level & TraceLevel);
}

TraceTypes Trace::Resolve(short level, int threadid)
{
    ThreadID *tid = ThreadMap.find(threadid);

    switch (tid.type) {
	case NickServ:
	    switch (level) {
		Off:		return Off;
		Chatter:	return NS_Chatter;
		CheckPoint:	return NS_CheckPoint;
		Functions:	return NS_Functions;
		Modify:		return NS_Locking;
	    }
	case ChanServ:
	    switch (level) {
		Off:		return Off;
		Chatter:	return CS_Chatter;
		CheckPoint:	return CS_CheckPoint;
		Functions:	return CS_Functions;
		Modify:		return CS_Locking;
	    }
	case MemoServ:
	    switch (level) {
		Off:		return Off;
		Chatter:	return MS_Chatter;
		CheckPoint:	return MS_CheckPoint;
		Functions:	return MS_Functions;
		Modify:		return MS_Locking;
	    }
	case OperServ:
	    switch (level) {
		Off:		return Off;
		Chatter:	return OS_Chatter;
		CheckPoint:	return OS_CheckPoint;
		Functions:	return OS_Functions;
		Modify:		return OS_Locking;
	    }
	case OtherServ:
	    switch (level) {
		Off:		return Off;
		Chatter:	return XS_Chatter;
		CheckPoint:	return XS_CheckPoint;
		Functions:	return XS_Functions;
		Modify:		return XS_Locking;
	    }
	case ServNet:
	    switch (level) {
		Off:		return Off;
		Chatter:	return NET_Chatter;
		CheckPoint:	return NET_CheckPoint;
		Functions:	return NET_Functions;
		Sockets:	return NET_Sockets;
	    }
	case BOB:
	    switch (level) {
		Off:		return Off;
		Chatter:	return BOB_Chatter;
		Bind:		return BOB_Bind;
		Functions:	return BOB_Functions;
		External:	return BOB_External;
	    }
	default:
	    switch (level) {
		Off:		return Off;
		Stats:		return G_Stats;
		Source:		return G_Source;
		Functions:	return G_Functions;
		Locking:	return G_Locking;
	    }
    }
}

FuncTrace::FuncTrace(const mstring &name, mVarArray &args) {
    indent++;

    if (IsOn(FuncTrace)) {
	mstring message = name + "(";
	for (int i=0; i<args.count(); i++) {
	    message += " (" + args[i].type() + ") " + args[i];
	    if (i < args.count() - 1)
		message +=", ";
	}
	message += " )";

	WriteOut(FuncTrace, ThreadID, message);
    }
}
