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

class Trace
{
    long TraceLevel;
public:
    enum TraceTypes {
	Off		= 0,
	G_Stats		= 0x00000001,	// CPU/Memory and Global Flags
	G_SourceFiles	= 0x00000002,	// Config / Language / Bob Script
	G_Functions	= 0x00000004,	// Functions not covered below
	G_Locking	= 0x00000008,	// All read/write locks
	NS_Chatter	= 0x00000010,	// Nick & NickServ messages
	NS_CheckPoint	= 0x00000020,	// Nick & NickServ checkpoints / flags
	NS_Functions	= 0x00000040,	// Nick & NickServ functions
	NS_Modify	= 0x00000080,	// Nick & NickServ data modification
	CS_Chatter	= 0x00000100,	// Channel & ChanServ messages
	CS_CheckPoint	= 0x00000200,	// Channel & ChanServ checkpoints / flags
	CS_Functions	= 0x00000400,	// Channel & ChanServ functions
	CS_Modify	= 0x00000800,	// Channel & ChanServ data modification
	MS_Chatter	= 0x00001000,	// Memo & News messages
	MS_CheckPoint	= 0x00002000,	// Memo & News checkpoints / flags
	MS_Functions	= 0x00004000,	// Memo & News functions
	MS_Modify	= 0x00008000,	// Memo & News data modification
	OS_Chatter	= 0x00010000,	// OperServ messages
	OS_CheckPoint	= 0x00020000,	// OperServ checkpoints / flags
	OS_Functions	= 0x00040000,	// OperServ functions
	OS_Modify	= 0x00080000,	// OperServ data modification
	XS_Chatter	= 0x00100000,	// Other Services messages
	XS_CheckPoint	= 0x00200000,	// Other Services checkpoints / flags
	XS_Functions	= 0x00400000,	// Other Services functions
	XS_Modify	= 0x00800000,	// Other Services data modification
	NET_Chatter	= 0x01000000,	// Service Network messages
	NET_CheckPoint	= 0x02000000,	// Service Network checkpoints / flags
	NET_Functions	= 0x04000000,	// Service Network functions
	NET_Sockets	= 0x08000000,	// Service Network sockets / DCC's
	BOB_Chatter	= 0x10000000,	// BOB messages
	BOB_Bind	= 0x20000000,	// BOB Hooks, Binds & Registers
	BOB_Functions	= 0x40000000,	// BOB functions
	BOB_External	= 0x80000000	// BOB external input/output
    };
    enum { Off = 0, Stats = 1, Source = 2, Locking = 4, Sockets = 4, Bind = 2, External = 4,
	   Chatter = 1, CheckPoint = 2, Functions = 3, Modify = 4 };

    Trace();
    ~Trace();
    bool IsOn(long level);
    TraceTypes Resolve(short level, int threadid);
}

class FuncTrace() : public Trace
{
public:
    FuncTrace(const mstring name, mVarArray &args);
    ~FuncTrace() { indent--; }

}
