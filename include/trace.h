#ifndef WIN32
#pragma interface
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#ifndef _TRACE_H
#define _TRACE_H
#include "pch.h"
RCSID(trace_h, "@(#) $Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#define JUST_MFILE 1
#include "filesys.h"
#undef JUST_MFILE
#include "mexceptions.h"

enum threadtype_enum
{ tt_LOST =
	0, tt_MAIN, tt_NickServ, tt_ChanServ, tt_MemoServ, tt_OperServ, tt_OtherServ, tt_ServNet, tt_Script, tt_mBase, tt_MAX
};
extern mstring threadname[tt_MAX];
unsigned short makehex(const mstring & SLevel);
enum socktype_enum
{ S_Unknown = 0, S_IrcServer, S_DCC, S_DCCFile,
    S_Client, S_Services, S_Telnet
};
enum dir_enum
{ D_Unknown = 0, D_From, D_To };
enum priority_enum
{ P_Lowest = 0, P_Delay, P_Normal, P_DepFilled, P_Retry, P_System, P_Highest };

class ThreadID
{
private:
    threadtype_enum t_internaltype;
    short t_indent;
    bool t_intrace;
    mstring t_lastfunc;

    list < mstring > messages;

public:
    ThreadID();
    ThreadID(const threadtype_enum Type);

    ~ThreadID();
    bool InTrace() const
    {
	return t_intrace;
    }
    mstring LastFunc() const
    {
	return t_lastfunc;
    }
    void LastFunc(const mstring & in)
    {
	t_lastfunc = in;
    }
    void assign(const threadtype_enum Type);
    threadtype_enum type() const
    {
	return t_internaltype;
    }
    void indentup()
    {
	t_indent++;
    }
    void indentdown()
    {
	if (t_indent > 0)
	    t_indent--;
    }
    short indent() const
    {
	return t_indent;
    }
    mstring logname() const;
    void WriteOut(const mstring & message);
    void Flush();
};

class Logger : public ACE_Log_Msg_Callback
{
    mFile fout;

public:
    Logger();
    ~Logger();

    void log(ACE_Log_Record & log_record);
    void close();
    void open();
    bool opened() const;
};

void LOG2(ACE_Log_Priority type, const mstring & msg);

#define LOG(X, Y, Z) \
	{ LOG2(X, parseMessage(Magick::instance().getLogMessage(Y), mVarArray Z)); }
#define NLOG(X, Y) \
	{ LOG2(X, parseMessage(Magick::instance().getLogMessage(Y))); }
#define SLOG(X, Y, Z) \
	{ LOG2(X, parseMessage(Y, mVarArray Z)); }
#define NSLOG(X, Y) \
	{ LOG2(X, parseMessage(Y)); }

#ifndef MAGICK_TRACE_WORKS

#define FT(x,y) do { \
	ThreadID *tid = mThread::find(); \
	if (tid != NULL) tid->LastFunc(x); \
	} while (0)
#define NFT(x) do { \
	ThreadID *tid = mThread::find(); \
	if (tid != NULL) tid->LastFunc(x); \
	} while (0)
#define RET(x) return (x)
#define NRET(x,y) return (y)
#define DRET(x) return (x)
#define NDRET(x,y) return (y)
#define TRET(x) do { ACE_Thread::exit(x); return (x); } while (0)
#define NTRET(x,y) do { ACE_Thread::exit(y); return (y); } while (0)
#define DTRET(x) do { ACE_Thread::exit(x); return (x); } while (0)
#define NDTRET(x,y) do { ACE_Thread::exit(y); return (y); } while (0)
#define CP(x)
#define COM(x)
#define SRC(x)
#define CSRC(x,y,z)
#define MB(x,y)
#define ME(x,y)
#define CB(x,y)
#define CE(x,y)
#define MCB(x)
#define MCE(x)
#define CH(x,y)
#define BTCB()
#define ETCB()
#define FLUSH()
#define MASSERT(truth,message)

#else /* MAGICK_TRACE_WORKS */

// NOTE: The following classes cannot be traced:
// mstring
// mVariant and mVarArray (AOC)
// mDateTime

// FunctionTrace -- FT("...", ());
#define FT(x,y) mVarArray __ft_data y; T_Function __ft(__FILE__, __LINE__, x, __ft_data)
#define NFT(x) T_Function __ft(__FILE__, __LINE__, x)

// Set return value -- RET()
#define RET(x) do { \
	__ft.return_line=__LINE__; \
	__ft.return_value=mVariant(x); \
	return (x); \
	} while (0)
#define NRET(x,y) do { \
	__ft.return_line=__LINE__; \
	__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); \
	return (y); \
	} while (0)
#define DRET(x) do { \
	__ft.return_line=__LINE__; \
	__ft.return_value=mVariant(x); \
	mThread::Detach(); \
	return (x); \
	} while (0)
#define NDRET(x,y) do { \
	__ft.return_line=__LINE__; \
	__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); \
	mThread::Detach(); \
	return (y); \
	} while (0)
#define TRET(x) do { \
	__ft.return_line=__LINE__; \
	__ft.return_value=mVariant(x); \
	ACE_Thread::exit(x); \
	return (x); \
	} while (0)
#define NTRET(x,y) do { \
	__ft.return_line=__LINE__; \
	__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); \
	ACE_Thread::exit(y); \
	return (y); \
	} while (0)
#define DTRET(x) do { \
	__ft.return_line=__LINE__; \
	__ft.return_value=mVariant(x); \
	mThread::Detach(); \
	ACE_Thread::exit(x); \
	return (x); \
	} while (0)
#define NTDRET(x,y) do { \
	__ft.return_line=__LINE__; \
	__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); \
	mThread::Detach(); \
	ACE_Thread::exit(y); \
	return (y); \
	} while (0)

// CheckPoint definition -- CP(());
#define CP(x) do { T_CheckPoint __cp x; } while (0)

// Comments definition -- COM(());
#define COM(x) do { T_Comment __com x; } while (0)

// Config file load
#define SRC(x) do { T_Source(x); } while (0)
#define CSRC(x,y,z) do { T_Source(x, y, z); } while (0)

// Modify begin -- MB((), offs = 0);
// Modify end -- ME(());
#define MB(x,y) do { \
	T_Modify __mod(mVarArray y, x); \
	__mod.Begin(); \
	} while (0)
#define ME(x,y) do { \
	T_Modify __mod(mVarArray y, x); \
	__mod.End(); \
	} while (0)

// Changing begin -- CB(item, stuff);
// Changing end -- CE(item, stuff);
#define CB(x,y) T_Changing __chg_ ## x ## _ (mstring(#y), mVariant(y))
#define CE(x,y) __chg_ ## x ## _.End(mVariant(y))

// Changing with DumpE/B calls
#define MCB(x) DumpB(); CB(0, x)
#define MCE(x) CE(0, x); DumpE()

// In or Out chatter -- CH(enum, "...");
#define CH(x,y) do { T_Chatter __ch(x,y); } while (0)

// Exception catching ...
#define BTCB() try {
#define ETCB() } catch(exception &E) { \
		T_Exception __tcb(__FILE__, __LINE__, typeid(E).name(), E.what()); \
		FLUSH(); \
                throw; \
        } catch(...) { \
		T_Exception __tcb(__FILE__, __LINE__, NULL, NULL); \
		FLUSH(); \
                throw; \
        }

#define MASSERT(truth,message) if((truth)==false) \
	throw new exception("Assertion in file "__FILE__" at line "__LINE__": "message);

#define FLUSH() do { \
	ThreadID *tid = mThread::find(); \
	if (tid != NULL && !tid->InTrace()) tid->Flush(); \
	} while (0)

// OperServ TRACE Syntax:
//
// TRACE SET NS 0x00B0          Just 3 flags (0 works)
// TRACE SET NS Func*           Just 1 flag
// TRACE SET NS Func* Chat*     Just 2 flags
// TRACE SET ALL 0x00B0         Just 3 flags (0 works)
// TRACE SET ALL Func*          Just 1 flag
// TRACE SET ALL Func* Chat*    Just 2 flags
// TRACE UP NS Func*            4 (1 flag)
// TRACE UP NS Func* Chat*      4 + 1 (2 flags)
// TRACE UP ALL Func*           4 (1 flag)
// TRACE UP ALL Func* Chat*     4 + 1 (2 flags)
// TRACE DOWN NS Func*          4 (1 flag)
// TRACE DOWN NS Func* Chat*    4 + 1 (2 flags)
// TRACE DOWN ALL Func*         4 (1 flag)
// TRACE DOWN ALL Func* Chat*   4 + 1 (2 flags)
//
// Above uses threadname and Trace::levelname for translation
// threadname follows threadtype_enum
// Trace::levelname is a struct translating level_enum enum's
//
// input[3] is first checked in reverse order against threadname,
// then against MAIN's thread levels (Locking, Function, SourceFiles, Stat)
// then it gives a syntax error.

extern list < pair < threadtype_enum, mstring > > ThreadMessageQueue;

// Trace Codes
//   \   Down Function (T_Function)
//   /   Up Function (T_Function)
//   **  CheckPoint (T_CheckPoint)
//   ##  Comment (T_Comment)
//   $$  Config Load (T_Source)
//   ->  Outbound Traffic (T_Chatter)
//   <-  Inbound Traffic (T_Chatter)
//   --  Unknown Traffic (T_Chatter)
//   <<  Before Changes (T_Modify)
//   >>  After Changes (T_Modify)
//   ==  Value Changes (T_Changing)
//   !!  Exception Thrown (T_Exception)
//   :+  Read/Write Locking (T_Locking)
//   :-  Read/Write UnLocking (T_Locking)
//   %%  CPU/Memory Stats (T_Stat)
//   |+  Socket Establish (T_Socket)
//   ||  Socket Failed Establish (T_Socket)
//   |=  Socket Definition (T_Socket)
//   |-  Socket Dropout (T_Socket)
//   &&  BOB Binds, Registrations (T_Bind)
//   ??  External commands (T_External)
//   ()  Trace Level Changes.

// ===================================================

class Trace
{
public:
    // For expansion -- 0x002C
    enum level_enum
    {
	Off = 0x0000,

	// Code Tracing
	CheckPoint = 0x0001,	// CP(()) entries
	Comment = 0x0002,	// More verbose checkpoints

	Exception = 0x0010,	// Exceptions being thrown
	Locking = 0x0040,	// READ/WRITE/MUTEX
	Function = 0x0080,	// Function Tracing

	// Data Tracing
	Changing = 0x0100,	// WHATS being changed
	Modify = 0x0200,	// IN / OUT difference
	Source = 0x0400,	// Config Files
	Bind = 0x0800,		// Binding/Registering

	// Live Tracing
	Stat = 0x1000,		// Cycle Statistics
	Socket = 0x2000,	// Inbound Connections
	Chatter = 0x4000,	// All text IN/OUT
	External = 0x8000,	// External command/output

	Full = 0xffff
    };

private:
    static unsigned short traces[tt_MAX];

public:
    struct levelname_struct
    {
	mstring name;
	level_enum level;

	levelname_struct()
	{
	    name = "";
	    level = Off;
	};
	levelname_struct(const mstring & in, level_enum lin)
	{
	    name = in;
	    level = lin;
	};
	levelname_struct &operator=(const levelname_struct & in)
	{
	    name = in.name;
	    level = in.level;
	    return *this;
	};
    };
    // This is initialised in main.cpp
    static vector < levelname_struct > levelname;

    Trace()
    {
    };
    ~Trace()
    {
    };

    static unsigned short TraceLevel(const threadtype_enum type)
    {
	return traces[type];
    }
    static bool IsOn(const threadtype_enum type, const level_enum level)
    {
	return (traces[type] & level) ? true : false;
    }
    static void TurnUp(const threadtype_enum type, const level_enum param)
    {
	traces[type] |= param;
    }
    static void TurnDown(const threadtype_enum type, const level_enum param)
    {
	traces[type] &= ~param;
    }
    static void TurnSet(const threadtype_enum type, const unsigned short param)
    {
	traces[type] = param;
    }

    static bool IsOn(ThreadID * tid, const level_enum level)
    {
	return IsOn(tid->type(), level);
    }
    static void TurnUp(ThreadID * tid, const level_enum level)
    {
	TurnUp(tid->type(), level);
    }
    static void TurnDown(ThreadID * tid, const level_enum level)
    {
	TurnDown(tid->type(), level);
    }
};

extern Trace *TraceObject;

// ===================================================

class T_Function : public Trace
{
    mstring m_name;
    mstring i_prevfunc;
    const char *file_name;

    T_Function()
    {
    }

public:
    mVariant return_value;
    int return_line;

    T_Function(const char *file, int line, const mstring & name);
    T_Function(const char *file, int line, const mstring & name, const mVarArray & args);

    ~T_Function();
};

// ===================================================

class T_Exception : public Trace
{
public:
    T_Exception(const char *file, const int line, const char *type, const char *what);

    ~T_Exception()
    {
    }
};

// ===================================================

class T_CheckPoint : public Trace
{
    void common(const mstring & input);

public:
    T_CheckPoint();
    T_CheckPoint(const char *fmt, ...);

    ~T_CheckPoint()
    {
    }
};

// ===================================================

class T_Comment : public Trace
{
    void common(const mstring & input);

    T_Comment();

public:
    T_Comment(const char *fmt, ...);

    ~T_Comment()
    {
    }
};

// ===================================================

class T_Modify : public Trace
{
    mVarArray i_args;
    unsigned int i_offset;

    T_Modify()
    {
    }
public:

    T_Modify(const mVarArray & args, unsigned int offset = 0);
    void Begin();
    void End();

    ~T_Modify()
    {
    }
};

// ===================================================

class T_Changing : public Trace
{
    mstring i_name;
    mVariant i_arg;

    T_Changing()
    {
    }

public:
    T_Changing(const mstring & name, const mVariant & arg);
    void End(const mVariant & arg);

    ~T_Changing()
    {
    }
};

// ===================================================

class T_Chatter : public Trace
{
    T_Chatter()
    {
    }

public:
    T_Chatter(const dir_enum direction, const mstring & input);

    ~T_Chatter()
    {
    }
};

// ===================================================

class T_Locking : public Trace
{
    locktype_enum locktype;
    mstring name;

public:
    T_Locking()
    {
    }
    void open(const locktype_enum ltype, const mstring & lockname);

    ~T_Locking();

};

// ===================================================

class T_Source : public Trace
{
public:
    T_Source(const mstring & text);
    T_Source(const mstring & section, const mstring & key, const mstring & value);
};

// ===================================================

// class T_Stat : public Trace {};

// ===================================================

class T_Socket : public Trace
{
    unsigned long s_id;

public:
    void Begin(const unsigned long id, const unsigned short local, const unsigned short remote, const mstring & host,
	       const dir_enum direction = D_Unknown);
    void Failed(const unsigned long id, const unsigned short local, const unsigned short remote, const mstring & host,
		const mstring & reason, const dir_enum direction = D_Unknown);
    void Resolve(const socktype_enum type, const mstring & info);
    void End(const mstring & reason);
};

// ===================================================

// class T_Bind : public Trace {};

// ===================================================

// class T_External : public Trace {};

#endif /* MAGICK_TRACE_WORKS */
#endif /* _TRACE_H */
