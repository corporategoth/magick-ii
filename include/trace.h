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


// FunctionTrace -- FT("...", ());
#define FT(x,y) T_Functions __ft(x,AOC(y))
// Set return value -- RET()
#define RET(x) __ft.return_value(x); return x
#define NRET(x,y) __ft.return_value("(" + #x + ") " + #y); return y
// CheckPoint definition -- CP(());
#define CP(x) { T_CheckPoint __cp x; }
// Modify begin -- MB(AOC());
#define MB(x) T_Modify __mod(AOC(x))
// Modify end -- ME(());
#define ME(x) __mod.End(AOC(x))
// In or Out chatter -- CH(enum, "...");
#define CH(x) { T_Chatter __ch(x); }

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
// Above uses threadname and Trace::levelname for translation
// threadname follows threadtype_enum
// Trace::levelname is a struct translating level_enum enum's
//
// input[3] is first checked in reverse order against threadname,
// then against MAIN's thread levels (Locking, Functions, SourceFiles, Stata)
// then it gives a syntax error.

enum threadtype_enum { tt_MAIN = 0, tt_NickServ, tt_ChanServ, tt_MemoServ, tt_OperServ, tt_OtherServ, tt_ServNet, tt_BOB, tt_MAX };
extern mstring threadname[tt_MAX];

// Trace Codes
//   \\  Down Function (T_Functions)
//   //  Up Function (T_Functions)
//   **  CheckPoint (T_CheckPoint)
//   ->  Outbound Traffic (T_Chatter)
//   <-  Inbound Traffic (T_Chatter)
//   --  Unknown Traffic (T_Chatter)
//   <<  Before changes (T_Modify)
//   >>  After Changes (T_Modify)
//   :+  Read/Write Locking (T_Locking)
//   :-  Read/Write UnLocking (T_Locking)
//   %%  CPU/Memory Stats (T_Stats)
//   |+  Socket Establish (T_Sockets)
//   ||  Socket Definition (T_Sockets)
//   |-  Socket Dropout (T_Sockets)
//   !!  BOB Binds, Registrations (T_Bind)
//   ??  External commands (T_External)

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
	levelname_struct() {name=""; level=Off;};
	levelname_struct(const mstring& in, level_enum lin) {name=in; level=lin;};
	levelname_struct& operator=(const levelname_struct& in) {name=in.name; level=in.level; return *this;};
    };
    const static struct levelname_struct levelname[];

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

// ToDo -- A method to get the current ThreadID number
// in here, without specifying it everywhere.
class ThreadID {
private:
    threadtype_enum internaltype;
    int number;
    short indent;
    wxOutputStream out;
    
    mstring logname();

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

class T_Functions : public Trace
{
    ThreadID *tid;
    mstring m_name;
    T_Functions() {} 
public:
    mVariant return_value;
    T_Functions(const mstring &name, const mVarArray &args);
    ~T_Functions();
};

// ===================================================

class T_CheckPoint : public Trace
{
    ThreadID *tid;
    void common(const char *input);
public:
    T_CheckPoint();
    T_CheckPoint(const char *fmt, ...);
    ~T_CheckPoint() {}
};

// ===================================================

class T_Modify : public Trace
{
    ThreadID *tid;
    T_Modify() {}
public:
    T_Modify(const mVarArray &args);
    void End(const mVarArray &args);
    ~T_Modify() {}
};

// ===================================================

class T_Chatter : public Trace
{
    ThreadID *tid;
    T_Chatter() {} 
public:
    enum dir_enum { From, To, Unknown };
    T_Chatter(dir_enum direction, const mstring &input);
    ~T_Chatter() {}
};

// ===================================================

// class T_Stats : public Trace {};


// ===================================================

class T_Locking : public Trace {
public:
    enum type_enum { Read, Write, Mutex };

private:
    ThreadID *tid;
    type_enum locktype;
    mstring name;

public:
    T_Locking() {}
    void open(type_enum, mstring lockname);
    ~T_Locking();
};

// ===================================================

// class T_Sockets : public Trace {};

// ===================================================

// class T_Bind : public Trace {};

// ===================================================

// class T_External : public Trace {};


#endif /* _TRACE_H */
