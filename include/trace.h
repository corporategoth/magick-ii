#ifndef WIN32
#pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _TRACE_H
#define _TRACE_H
#include "pch.h"
static const char *ident_trace_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.63  2000/08/28 10:51:35  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.62  2000/08/22 08:43:39  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.61  2000/08/06 05:27:46  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.60  2000/07/30 09:04:04  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.59  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.58  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.57  2000/05/20 03:28:10  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.56  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.55  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "variant.h"
#include "mstring.h"

enum threadtype_enum { tt_MAIN = 0, tt_NickServ, tt_ChanServ, tt_MemoServ, tt_OperServ, tt_OtherServ, tt_ServNet, tt_Script, tt_mBase, tt_LOST, tt_MAX };
extern mstring threadname[tt_MAX];
extern unsigned short makehex(mstring SLevel);
enum locktype_enum { L_Invalid = 0, L_Read, L_Write, L_Mutex };

class ThreadID {
private:
    threadtype_enum t_internaltype;
    short t_indent;
    bool t_intrace;
    mstring t_lastfunc;
    list<mstring> messages;
    
public:
    ThreadID();
    ThreadID(threadtype_enum Type);
    ~ThreadID();
    bool InTrace() { return t_intrace; }
    mstring LastFunc() { return t_lastfunc; }
    void LastFunc(mstring in) { t_lastfunc = in; }
    void assign(threadtype_enum Type);
    threadtype_enum type() { return t_internaltype; }
    void indentup() { t_indent++; }
    void indentdown() { if (t_indent>0) t_indent--; }
    short indent() { return t_indent; }
    mstring logname();
    void WriteOut (const mstring &message);
    void Flush();
};

inline int do_nothing() { return 1; }

#ifndef MAGICK_TRACE_WORKS

#define FT(x,y) { ThreadID *tid = mThread::find(); if (tid != NULL) tid->LastFunc(x); }
#define NFT(x) { ThreadID *tid = mThread::find(); if (tid != NULL) tid->LastFunc(x); }
#define RET(x) return x
#define NRET(x,y) return y
#define DRET(x) return x
#define NDRET(x,y) return y
#define CP(x) do_nothing()
#define COM(x) do_nothing()
#define MB(x) do_nothing()
#define ME(x) do_nothing()
#define CH(x,y) do_nothing()
#define FLUSH() do_nothing()

#else /* MAGICK_TRACE_WORKS */

// NOTE: The following classes cannot be traced:
// mstring
// mVariant and mVarArray (AOC)
// mDateTime

// FunctionTrace -- FT("...", ());
#define FT(x,y)  mVarArray __ft_VarArray y; T_Functions __ft(x, __ft_VarArray)
#define NFT(x) T_Functions __ft(x)

// Set return value -- RET()
#define RET(x) {__ft.return_value=mVariant(x); return (x);}
#define NRET(x,y) {__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); return y;}
#define DRET(x) {__ft.return_value=mVariant(x); mThread::Detach(); return (x);}
#define NDRET(x,y) {__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); mThread::Detach(); return y;}
#if 0
// this is a good idea but in practice is majorly fubar'ed
#define CRET(x,y,z) {mstring __ft_val; __ft_val << y; \
    __ft.return_value=("(" + mstring(#x) + ") " + y).c_str(); return z;}
#endif

// CheckPoint definition -- CP(());
#define CP(x) { T_CheckPoint __cp x; }

// Comments definition -- COM(());
#define COM(x) { T_Comments __com x; }

// Modify begin -- MB(AOC());
#define MB(x) mVarArray __mb_VarArray x; T_Modify __mod(__mb_VarArray)

// Modify end -- ME(());
#define ME(x) mVarArray __me_VarArray x; __mod.End(__me_VarArray)

// In or Out chatter -- CH(enum, "...");
#define CH(x,y) { T_Chatter __ch(x,y); }

#define FLUSH() { ThreadID *tid = mThread::find(); if (tid != NULL && !tid->InTrace()) tid->Flush(); }

// OperServ TRACE Syntax:
//
// TRACE SET NS 0x00B0		Just 3 flags (0 works)
// TRACE SET NS Func*		Just 1 flag
// TRACE SET NS Func* Chat*	Just 2 flags
// TRACE SET ALL 0x00B0		Just 3 flags (0 works)
// TRACE SET ALL Func*		Just 1 flag
// TRACE SET ALL Func* Chat*	Just 2 flags
// TRACE UP NS Func*		4 (1 flag)
// TRACE UP NS Func* Chat*	4 + 1 (2 flags)
// TRACE UP ALL Func*		4 (1 flag)
// TRACE UP ALL Func* Chat*	4 + 1 (2 flags)
// TRACE DOWN NS Func*		4 (1 flag)
// TRACE DOWN NS Func* Chat*	4 + 1 (2 flags)
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

extern list<pair<threadtype_enum, mstring> > ThreadMessageQueue;

// Trace Codes
//   \   Down Function (T_Functions)
//   /   Up Function (T_Functions)
//   **  CheckPoint (T_CheckPoint)
//   ##  Comments (T_Comments)
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
//   ()  Trace Level Changes.

// ===================================================

class Trace
{
public:
    // For expansion -- 0x4C08
    enum level_enum {
	Off		= 0x0000,

	// Config/Stats
	Stats		= 0x0001,	// Cycle Statistics
	Source		= 0x0002,	// Config Files
	Bind		= 0x0004,	// Binding/Registering

	// Code Tracing
	CheckPoint	= 0x0010,	// CP(()) entries
	Comments	= 0x0020,	// More verbose checkpoints
	Locking		= 0x0040,	// READ/WRITE/MUTEX
	Functions	= 0x0080,	// Function Tracing

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
    // This is initialised in main.cpp
    static vector<levelname_struct> levelname;

    Trace() {};
    ~Trace() {};
    
    static level_enum ShortLevel(level_enum level) { return (SLevel = level); }
    static level_enum ShortLevel() { return SLevel; }

    static unsigned short TraceLevel(threadtype_enum type)
	{ return traces[type]; }
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

    static void TurnUp(threadtype_enum type)
	{ TurnUp(type, SLevel); }
    static void TurnUp(ThreadID *tid, level_enum level)
	{ TurnUp(tid->type(), level); }
    static void TurnUp(ThreadID *tid)
	{ TurnUp(tid->type(), SLevel); }

    static void TurnDown(threadtype_enum type)
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
    mstring i_prevfunc;

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

class T_Comments : public Trace
{
    ThreadID *tid;
    void common(const char *input);
    T_Comments();

public:
    T_Comments(const char *fmt, ...);
    ~T_Comments() {}
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
    ThreadID *tid;
    locktype_enum locktype;
    mstring name;

public:
    T_Locking() {}
    void open(locktype_enum ltype, mstring lockname);
    ~T_Locking();

};

// ===================================================

// class T_Sockets : public Trace {};

// ===================================================

// class T_Bind : public Trace {};

// ===================================================

// class T_External : public Trace {};

#endif /* MAGICK_TRACE_WORKS */
#endif /* _TRACE_H */
