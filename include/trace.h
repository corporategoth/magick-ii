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
#ifndef _TRACE_H
#define _TRACE_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
#include <map>
#include <utility>
using namespace std;
#include "mstream.h"
#include "string.h"
#include "variant.h"


#define FT(x) FuncTrace __ft(#x);
#define CP(x) { CheckPoint __cp(#x); }
#define MB(x) Modify __mod(#x);
#define ME(x) __mod.EndModify(#x);
#define CH(x) { Chatter __ch(#x); }

// forward declarations till we get them done
class Thread;
class ThreadID;

// OperServ TRACE Syntax:
//
// TRACE SET 0x000000B0		Set exact TraceTypes
// TRACE SET B			Just 3 flags
// TRACE SET Func*		Just 1 flag
// TRACE SET Func* Chat*	Just 2 flags
// TRACE SET NS B		Just 3 flags
// TRACE SET NS Func*		Just 1 flag
// TRACE SET NS Func* Chat*	Just 2 flags
// TRACE SET ALL B		Just 3 flags
// TRACE SET ALL Func*		Just 1 flag
// TRACE SET ALL Func* Chat*	Just 2 flags
// TRACE UP B			8 + 2 + 1 (3 flags)
// TRACE UP Func*		4 (1 flag)
// TRACE UP Func* Chat*		4 + 1 (2 flags)
// TRACE UP NS B		8 + 2 + 1 (3 flags)
// TRACE UP NS Func*		4 (1 flag)
// TRACE UP NS Func* Chat*	4 + 1 (2 flags)
// TRACE UP ALL B		8 + 2 + 1 (3 flags)
// TRACE UP ALL Func*		4 (1 flag)
// TRACE UP ALL Func* Chat*	4 + 1 (2 flags)
// TRACE DOWN B			8 + 2 + 1 (3 flags)
// TRACE DOWN Func*		4 (1 flag)
// TRACE DOWN Func* Chat*	4 + 1 (2 flags)
// TRACE DOWN NS B		8 + 2 + 1 (3 flags)
// TRACE DOWN NS Func*		4 (1 flag)
// TRACE DOWN NS Func* Chat*	4 + 1 (2 flags)
// TRACE DOWN ALL B		8 + 2 + 1 (3 flags)
// TRACE DOWN ALL Func*		4 (1 flag)
// TRACE DOWN ALL Func* Chat*	4 + 1 (2 flags)
//

enum threadtype_enum { MAIN = 0, NickServ, ChanServ, MemoServ, OperServ, OtherServ, ServNet, BOB, MAX };
char threadname[MAX] = { "", "NS", "CS", "MS", "OS", "XS", "NET", "BOB" };

class Trace
{

    static long TraceLevel;
    enum TraceTypes {
	TT_Off		= 0,
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

public:
    enum level_enum { Off = 0, Stats = 1, Source = 2, Locking = 8, Sockets = 8, Bind = 2,
	External = 8, Chatter = 1, CheckPoint = 2, Functions = 4, Modify = 8 };
    struct levelname_struct {
	mstring name;
	level_enum level;
    } levelname;
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

	// Thread* for now till we get it done
private:

    level_enum SLevel;
    map<pair<threadtype_enum,level_enum>,TraceTypes> tmap;
    typedef pair<threadtype_enum,level_enum> levelpair;

    bool IsOnBig(TraceTypes level)
	{ return (level & TraceLevel!=0); }

    TraceTypes resolve(level_enum level, threadtype_enum type);
    TraceTypes resolve(level_enum level, ThreadID *tid);
    TraceTypes resolve(threadtype_enum type);
    TraceTypes resolve(ThreadID *tid);

    void TurnUp(TraceTypes param)
	{ TraceLevel |= param; }
    void TurnDown(TraceTypes param)
	{ TraceLevel &= ~param; }

public:
    Trace();
    ~Trace();
    
    level_enum ShortLevel(level_enum level) { return (SLevel = level); }
    level_enum ShortLevel() { return SLevel; }

    void TurnSet(long param)
	{ TraceLevel = param; }

    bool IsOn(level_enum level, threadtype_enum type)
	{ return IsOnBig(resolve(level, type)); }
    bool IsOn(level_enum level, ThreadID *tid)
	{ return IsOnBig(resolve(level, tid)); }
    bool IsOn(threadtype_enum type)
	{ return IsOnBig(resolve(type)); }
    bool IsOn(ThreadID *tid)
	{ return IsOnBig(resolve(tid)); }

    void TurnUp(level_enum level, threadtype_enum type)
	{ TurnUp(resolve(level, type)); }
    void TurnUp(level_enum level, ThreadID *tid)
	{ TurnUp(resolve(level, tid)); }
    void TurnUp(threadtype_enum type)
	{ TurnUp(resolve(type)); }
    void TurnUp(ThreadID *tid)
	{ TurnUp(resolve(tid)); }

    void TurnDown(level_enum level, threadtype_enum type)
	{ TurnDown(resolve(level, type)); }
    void TurnDown(level_enum level, ThreadID *tid)
	{ TurnDown(resolve(level, tid)); }
    void TurnDown(threadtype_enum type)
	{ TurnDown(resolve(type)); }
    void TurnDown(ThreadID *tid)
	{ TurnDown(resolve(tid)); }
};

// ===================================================

class ThreadID {
private:
    threadtype_enum internaltype;
    int number;
    short indent;
    wxOutputStream out;
    mstring logtext[MAX];
    
    mstring logname();
    void init();

public:
    ThreadID();
    ThreadID(threadtype_enum Type, int Number);
    ~ThreadID();
    ThreadID assign(threadtype_enum Type, int Number);
    threadtype_enum type() { return internaltype; }
    void indentup() { indent++; }
    void indentdown() { indent--; }
    void WriteOut (const mstring &message);
};

// ===================================================

class FuncTrace : public Trace
{
    ThreadID *tid;
    FuncTrace() {} 
public:
    FuncTrace(const mstring &name, const mVarArray &args);
    ~FuncTrace() { tid->indentdown(); }
};

// ===================================================

class CheckPoint : public Trace
{
    ThreadID *tid;
    void common(const char *input);
public:
    CheckPoint();
    CheckPoint(const char *fmt, ...);
    ~CheckPoint() {}
};

// ===================================================

class Modify : public Trace
{
    ThreadID *tid;
    Modify() {} 
public:
    Modify(const mVarArray &args);
    EndModify(const mVarArray &args);
    ~Modify() {}
};

// ===================================================

class Chatter : public Trace
{
    ThreadID *tid;
    Chatter() {} 
public:
    enum dir_enum { From, To };
    Chatter(dir_enum direction, const mstring &input);
    ~Chatter() {}
};

// ===================================================

#endif /* _TRACE_H */
