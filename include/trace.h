#ifndef WIN32
#pragma interface
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
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
RCSID(trace_h, "@(#) $Id$");

/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.82  2002/01/12 14:42:08  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.81  2002/01/01 22:16:55  prez
** Fixed memory leak properly in db saving ...
**
** Revision 1.80  2001/12/30 21:27:29  prez
** Some trace code beautification, and added ACE_Thread::exit() to worker
** and save_databases threads ..
**
** Revision 1.79  2001/11/16 20:27:33  prez
** Added a MAX_THREADS option, and made the thread heartbeat a timer based
** operation, instead of part of the threads.
**
** Revision 1.78  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.77  2001/06/20 06:07:01  prez
** ome GCC 3.0 and solaris fixes
**
** Revision 1.76  2001/06/11 03:44:45  prez
** Re-wrote how burst works, and made the burst message a lower priority
** than normal.  Also removed the chance of a stray pointer being picked
** up in the dependancy system.
**
** Revision 1.75  2001/05/25 01:59:31  prez
** Changed messaging system ...
**
** Revision 1.74  2001/05/01 14:00:22  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.73  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.72  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.71  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.70  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.68  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.67  2000/12/12 02:51:58  prez
** Moved the do_nothing function
**
** Revision 1.66  2000/10/14 04:25:31  prez
** Added mmemory.h -- MemCluster and the MemoryManager are now in it.
** TODO - make mstring use MemoryManager.
**
** Revision 1.65  2000/09/01 10:54:38  prez
** Added Changing and implemented Modify tracing, now just need to create
** DumpB() and DumpE() functions in all classes, and put MCB() / MCE() calls
** (or MB() / ME() or CB() / CE() where MCB() / MCE() not appropriate) in.
**
** Revision 1.64  2000/08/31 06:25:08  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
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

enum threadtype_enum
{ tt_LOST =
	0, tt_MAIN, tt_NickServ, tt_ChanServ, tt_MemoServ, tt_OperServ, tt_OtherServ, tt_ServNet, tt_Script, tt_mBase, tt_MAX };
extern mstring threadname[tt_MAX];
unsigned short makehex(const mstring & SLevel);
enum locktype_enum
{ L_Invalid = 0, L_Read, L_Write, L_WriteUpgrade, L_Mutex };
enum socktype_enum
{ S_Unknown = 0, S_IrcServer, S_DCC, S_DCCFile,
    S_Client, S_Services, S_Telnet
};
enum dir_enum
{ D_Unknown = 0, D_From, D_To };
enum priority_enum
{ P_Lowest = 0, P_Delay, P_Normal, P_DepFilled, P_System, P_Highest };

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

#ifndef MAGICK_TRACE_WORKS

#define FT(x,y) do { \
	ThreadID *tid = mThread::find(); \
	if (tid != NULL) tid->LastFunc(x); \
	} while (0)
#define NFT(x) do { \
	ThreadID *tid = mThread::find(); \
	if (tid != NULL) tid->LastFunc(x); \
	} while (0)
#define RET(x) return x
#define NRET(x,y) return y
#define DRET(x) return x
#define NDRET(x,y) return y
#define TRET(x) do { ACE_Thread::exit(x); return x; } while (0)
#define NTRET(x,y) do { ACE_Thread::exit(y); return y; } while (0)
#define DTRET(x) do { ACE_Thread::exit(x); return x; } while (0)
#define NDTRET(x,y) do { ACE_Thread::exit(y); return y; } while (0)
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
#define FLUSH()

#else /* MAGICK_TRACE_WORKS */

// NOTE: The following classes cannot be traced:
// mstring
// mVariant and mVarArray (AOC)
// mDateTime

// FunctionTrace -- FT("...", ());
#define FT(x,y) T_Functions __ft(x, mVarArray y)
#define NFT(x) T_Functions __ft(x)

// Set return value -- RET()
#define RET(x) do { \
	__ft.return_value=mVariant(x); \
	return x; \
	} while (0)
#define NRET(x,y) do { \
	__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); \
	return y; \
	} while (0)
#define DRET(x) do { \
	__ft.return_value=mVariant(x); \
	mThread::Detach(); \
	return x; \
	} while (0)
#define NDRET(x,y) do { \
	__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); \
	mThread::Detach(); \
	return y; \
	} while (0)
#define TRET(x) do { \
	__ft.return_value=mVariant(x); \
	ACE_Thread::exit(x); \
	return x; \
	} while (0)
#define NTRET(x,y) do { \
	__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); \
	ACE_Thread::exit(y); \
	return y; \
	} while (0)
#define DTRET(x) do { \
	__ft.return_value=mVariant(x); \
	mThread::Detach(); \
	ACE_Thread::exit(x); \
	return x; \
	} while (0)
#define NTDRET(x,y) do { \
	__ft.return_value=("(" + mstring(#x) + ") " + mstring(#y)).c_str(); \
	mThread::Detach(); \
	ACE_Thread::exit(y); \
	return y; \
	} while (0)

// CheckPoint definition -- CP(());
#define CP(x) do { T_CheckPoint __cp x; } while (0)

// Comments definition -- COM(());
#define COM(x) do { T_Comments __com x; } while (0)

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
// then against MAIN's thread levels (Locking, Functions, SourceFiles, Stata)
// then it gives a syntax error.

extern list < pair < threadtype_enum, mstring > >ThreadMessageQueue;

// Trace Codes
//   \   Down Function (T_Functions)
//   /   Up Function (T_Functions)
//   **  CheckPoint (T_CheckPoint)
//   ##  Comments (T_Comments)
//   $$  Config Load (T_Source)
//   ->  Outbound Traffic (T_Chatter)
//   <-  Inbound Traffic (T_Chatter)
//   --  Unknown Traffic (T_Chatter)
//   <<  Before Changes (T_Modify)
//   >>  After Changes (T_Modify)
//   ==  Value Changes (T_Changing)
//   :+  Read/Write Locking (T_Locking)
//   :-  Read/Write UnLocking (T_Locking)
//   %%  CPU/Memory Stats (T_Stats)
//   |+  Socket Establish (T_Sockets)
//   ||  Socket Failed Establish (T_Sockets)
//   |=  Socket Definition (T_Sockets)
//   |-  Socket Dropout (T_Sockets)
//   !!  BOB Binds, Registrations (T_Bind)
//   ??  External commands (T_External)
//   ()  Trace Level Changes.

// ===================================================

class Trace
{
  public:
    // For expansion -- 0x4C08
    enum level_enum
    {
	Off = 0x0000,

	// Config/Stats
	Stats = 0x0001,		// Cycle Statistics
	Source = 0x0002,	// Config Files
	Bind = 0x0004,		// Binding/Registering

	// Code Tracing
	CheckPoint = 0x0010,	// CP(()) entries
	Comments = 0x0020,	// More verbose checkpoints
	Locking = 0x0040,	// READ/WRITE/MUTEX
	Functions = 0x0080,	// Function Tracing

	// Data Tracing
	Changing = 0x0100,	// WHATS being changed
	Modify = 0x0200,	// IN / OUT difference

	// Live Tracing
	Sockets = 0x1000,	// Inbound Connections
	Chatter = 0x2000,	// All text IN/OUT
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
	levelname_struct & operator=(const levelname_struct & in)
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

class T_Functions:public Trace
{
    mstring m_name;
    mstring i_prevfunc;

      T_Functions()
    {
    }

  public:
      mVariant return_value;
    T_Functions(const mstring & name);
    T_Functions(const mstring & name, const mVarArray & args);

    ~T_Functions();
};

// ===================================================

class T_CheckPoint:public Trace
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

class T_Comments:public Trace
{
    void common(const mstring & input);
      T_Comments();

  public:
      T_Comments(const char *fmt, ...);
     ~T_Comments()
    {
    }
};

// ===================================================

class T_Modify:public Trace
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

class T_Changing:public Trace
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

class T_Chatter:public Trace
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

class T_Locking:public Trace
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

class T_Source:public Trace
{
  public:
    T_Source(const mstring & text);
      T_Source(const mstring & section, const mstring & key, const mstring & value);
};

// ===================================================

// class T_Stats : public Trace {};

// ===================================================

class T_Sockets:public Trace
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
