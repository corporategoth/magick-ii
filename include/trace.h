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

// NOTE: The following classes cannot be traced:
// mstring
// mVariant and mVarArray (AOC)
// wxOutputStream
// mDateTime

// FunctionTrace -- FT("...", ());
#define FT(x,y)  mVarArray __ft_VarArray y; T_Functions __ft(x, __ft_VarArray)
#define NFT(x) T_Functions __ft(x)

// Set return value -- RET()
#define RET(x) {__ft.return_value=mVariant(x); return (x);}
#define NRET(x,y) {__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); return y;}
#if 0
// this is a good idea but in practice is majorly fubar'ed
#define CRET(x,y,z) {mstring __ft_val; __ft_val << y; \
    __ft.return_value=("(" + mstring(#x) + ") " + y).c_str(); return z;}
#endif


// CheckPoint definition -- CP(());
#define CP(x) { T_CheckPoint __cp x; }

// Modify begin -- MB(AOC());
#define MB(x) mVarArray __mb_VarArray x; T_Modify __mod(__mb_VarArray)

// Modify end -- ME(());
#define ME(x) mVarArray __me_VarArray x; __mod.End(__me_VarArray)

// In or Out chatter -- CH(enum, "...");
#define CH(x,y) { T_Chatter __ch(x,y); }

// OperServ TRACE Syntax:
//
// TRACE SET NS 0x00B0		Just 3 flags (0 works)
// TRACE SET NS Func*		Just 1 flag
// TRACE SET NS Func* Chat*	Just 2 flags
// TRACE SET ALL 0x00B0		Just 3 flags (0 works)
// TRACE SET ALL Func*		Just 1 flag
// TRACE SET ALL Func* Chat*	Just 2 flags
// TRACE UP NS 0x00B0		8 + 2 + 1 (3 flags) (0 works)
// TRACE UP NS Func*		4 (1 flag)
// TRACE UP NS Func* Chat*	4 + 1 (2 flags)
// TRACE UP ALL 0x00B0		8 + 2 + 1 (3 flags) (0 works)
// TRACE UP ALL Func*		4 (1 flag)
// TRACE UP ALL Func* Chat*	4 + 1 (2 flags)
// TRACE DOWN NS 0x00B0		8 + 2 + 1 (3 flags) (0 works)
// TRACE DOWN NS Func*		4 (1 flag)
// TRACE DOWN NS Func* Chat*	4 + 1 (2 flags)
// TRACE DOWN ALL 0x00B0	8 + 2 + 1 (3 flags) (0 works)
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

// ===================================================

// ToDo -- A method to get the current ThreadID number

// in here, without specifying it everywhere.
// ACE_thread_t threadid = ACE_Thread::self();
// threadtype_enum Type=MagickObject->ThreadtoTypeMap[ACE_Thread::self()];
// or to set the Type in the map  eg: MagickObject->ThreadtoTypeMap[ACE_Thread::self()]=tt_ChanServ;

class ThreadID {
private:
    threadtype_enum t_internaltype;
    int t_number;
    short t_indent;
    wxOutputStream out;
    
    mstring logname();

public:
    ThreadID();
    ThreadID(threadtype_enum Type, int Number);
    ~ThreadID() {}
    ThreadID assign(threadtype_enum Type, int Number);
    threadtype_enum type() { return t_internaltype; }
    void indentup() { t_indent++; }
    void indentdown() { t_indent--; }
    int number() { return t_number;  }
    short indent() { return t_indent; }

    void WriteOut (const mstring &message);
};

class Trace
{
public:
    // For expansion -- 0x4C48
    enum level_enum {
	Off		= 0x0000,

	// Config/Stats
	Stats		= 0x0001,	// Cycle Statistics
	Source		= 0x0002,	// Config Files
	Bind		= 0x0004,	// Binding/Registering

	// Code Tracing
	CheckPoint	= 0x0100,	// CP(()) entries
	Locking		= 0x0200,	// READ/WRITE/MUTEX
	Functions	= 0x0800,	// Function Tracing

	// Data Tracing
	Changing	= 0x0100,	// WHATS being changed
	Modify		= 0x0200,	// IN / OUT difference

	// Live Tracing
	Sockets		= 0x1000,	// Inbound Connections
	Chatter 	= 0x2000,	// All text IN/OUT
	External	= 0x8000,	// External command/output

	Full		= 0xffff
	};

private:
    static unsigned short traces[tt_MAX];
    static level_enum SLevel;

public:
    struct levelname_struct {
	mstring name;
	level_enum level;
	levelname_struct() {name=""; level=Off;};
	levelname_struct(const mstring& in, level_enum lin) {name=in; level=lin;};
	levelname_struct& operator=(const levelname_struct& in) {name=in.name; level=in.level; return *this;};
    };
    const static struct levelname_struct levelname[];

    Trace() {};
    ~Trace() {};
    
    static level_enum ShortLevel(level_enum level) { return (SLevel = level); }
    static level_enum ShortLevel() { return SLevel; }

    static bool IsOn(threadtype_enum type, level_enum level)
	{ return (traces[type] & level) ? true : false; }
    static void TurnUp(threadtype_enum type, level_enum param)
	{ traces[type] |= param; }
    static void TurnDown(threadtype_enum type, level_enum param)
	{ traces[type] &= ~param; }
    static void TurnSet(threadtype_enum type, unsigned short param)
	{ traces[type] = param; }

    static bool IsOn(threadtype_enum type)
	{ return IsOn(type, SLevel); }
    static bool IsOn(ThreadID *tid, level_enum level)
	{ return IsOn(tid->type(), level); }
    static bool IsOn(ThreadID *tid)
	{ return IsOn(tid->type(), SLevel); }

    static bool TurnUp(threadtype_enum type)
	{ TurnUp(type, SLevel); }
    static void TurnUp(ThreadID *tid, level_enum level)
	{ TurnUp(tid->type(), level); }
    static void TurnUp(ThreadID *tid)
	{ TurnUp(tid->type(), SLevel); }

    static bool TurnDown(threadtype_enum type)
	{ TurnDown(type, SLevel); }
    static void TurnDown(ThreadID *tid, level_enum level)
	{ TurnDown(tid->type(), level); }
    static void TurnDown(ThreadID *tid)
	{ TurnDown(tid->type(), SLevel); }
};

extern Trace *TraceObject;

// ===================================================

class T_Functions : public Trace
{
    ThreadID *tid;
    mstring m_name;

    T_Functions() {}

public:
    mVariant return_value;
    T_Functions(const mstring &name);
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
    void open(T_Locking::type_enum ltype, mstring lockname);
    ~T_Locking();

};

// ===================================================

// class T_Sockets : public Trace {};

// ===================================================

// class T_Bind : public Trace {};

// ===================================================

// class T_External : public Trace {};


#endif /* _TRACE_H */
