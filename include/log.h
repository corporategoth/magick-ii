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
#ifndef _LOG_H
#define _LOG_H
static const char *ident_log_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.18  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.17  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#include "mstring.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// different standard log levels (you may also define your own)
enum
{
  wxLOG_Fatal, // program can't continue, abort immediately
  wxLOG_Error,      // a serious error, user must be informed about it
  wxLOG_Warning,    // user is normally informed about it but may be ignored
  wxLOG_Notice,     // normal message (i.e. normal output of a non GUI app)
  wxLOG_Info,       // informational message (a.k.a. 'Verbose')
  wxLOG_Debug,      // outputted only when run in debug mode
  wxLOG_Progress,   // used for progress indicator (not yet)
  wxLOG_User = 100  // user defined levels start here
};

typedef unsigned long wxLogLevel;

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------

class wxLog
{
public:
  // ctor
  wxLog();

  // these functions allow to completely disable all log messages
    // is logging disabled now?
  static bool IsEnabled() { return ms_doLog; }
    // change the flag state, return the previous one
  static bool EnableLogging(bool doIt = true)
    { bool doLogOld = ms_doLog; ms_doLog = doIt; return doLogOld; }

  // sink function
  static void OnLog(wxLogLevel level, const char *szString)
  {
    if ( IsEnabled() ) {
      wxLog *pLogger = GetActiveTarget();
      if ( pLogger )
        pLogger->DoLog(level, szString);
    }
  }

  // message buffering
    // flush shows all messages if they're not logged immediately
    // (FILE and iostream logs don't need it, but wxGuiLog does to avoid
    //  showing 17 modal dialogs one after another)
  virtual void Flush();
    // call to Flush() may be optimized: call it only if this function
    // returns true (although Flush() also returns immediately if there
    // is no messages, this functions is more efficient because inline)
  bool HasPendingMessages() const { return m_bHasMessages; }

  // only one sink is active at each moment
    // get current log target, will call wxApp::CreateLogTarget() to create one
    // if none exists
  static wxLog *GetActiveTarget();
    // change log target, pLogger may be NULL
  static wxLog *SetActiveTarget(wxLog *pLogger);

    // functions controlling the default wxLog behaviour
    // verbose mode is activated by standard command-line '-verbose' option
  void SetVerbose(bool bVerbose = true) { m_bVerbose = bVerbose; }
    // sets the format for timestamp prepended by wxLog::DoLog(): it's
    // passed to strftime() function, see it's documentation for details.
    // no time stamp at all if szTF is NULL or empty
    // NB: the string is not copied, so it's lifetime must be long enough!
  void SetTimeStampFormat(const char *szTF) { m_szTimeFormat = szTF; }
    // should GetActiveTarget() try to create a new log object if the current
    // is NULL?
  static void DontCreateOnDemand() { ms_bAutoCreate = false; }

  // accessors
    // gets the verbose status
  bool GetVerbose() const { return m_bVerbose; }
    // get current time format
  const char *GetTimeStampFormat() const { return m_szTimeFormat; }

  // make dtor virtual for all derived classes
  virtual ~wxLog() { }

protected:
  bool m_bHasMessages;

  bool          m_bVerbose;     // false => ignore LogInfo messages
  const char   *m_szTimeFormat; // format for strftime()

  // the logging functions that can be overriden
    // default DoLog() prepends the time stamp and a prefix corresponding
    // to the message to szString and then passes it to DoLogString()
  virtual void DoLog(wxLogLevel level, const char *szString);
    // default DoLogString does nothing but is not pure virtual because if
    // you override DoLog() you might not need it at all
  virtual void DoLogString(const char *szString);

  // helpers
    // put the time stamp in the current format into the string
  mstring TimeStamp() const;

private:
  // static variables
  // ----------------
  static wxLog      *ms_pLogger;      // currently active log sink
  static bool        ms_doLog;        // false => all logging disabled
  static bool        ms_bAutoCreate;  // automatically create new log targets?
};

// ----------------------------------------------------------------------------
// "trivial" derivations of wxLog
// ----------------------------------------------------------------------------

// log everything to a "FILE *", stderr by default
class wxLogStderr : public wxLog
{
public:
  // redirect log output to a FILE
  wxLogStderr(FILE *fp = (FILE *) NULL);
  ~wxLogStderr();

  void ChangeFile(FILE *fp = (FILE *) NULL);
private:
  // implement sink function
  virtual void DoLogString(const char *szString);

  FILE *m_fp;
};

// ----------------------------------------------------------------------------
// /dev/null log target: suppress logging until this object goes out of scope
// ----------------------------------------------------------------------------

// example of usage:
/*
void Foo() {
  wxFile file;

  // wxFile.Open() normally complains if file can't be opened, we don't want it
  wxLogNull logNo;
  if ( !file.Open("bar") )
    ... process error ourselves ...

  // ~wxLogNull called, old log sink restored
}
*/
class wxLogNull
{
public:
  wxLogNull() { m_flagOld = wxLog::EnableLogging(false); }
 ~wxLogNull() { (void)wxLog::EnableLogging(m_flagOld);   }

private:
  bool m_flagOld; // the previous value of the wxLog::ms_doLog
};

// ============================================================================
// global functions
// ============================================================================

// ----------------------------------------------------------------------------
// Log functions should be used by application instead of stdio, iostream &c
// for log messages for easy redirection
// ----------------------------------------------------------------------------

// define wxLog<level>
// -------------------

#define DECLARE_LOG_FUNCTION(level)                                 \
        extern void wxLog##level(const char *szFormat, ...)
#define DECLARE_LOG_FUNCTION2(level, arg1)                          \
        extern void wxLog##level(arg1, const char *szFormat, ...)

// a generic function for all levels (level is passes as parameter)
DECLARE_LOG_FUNCTION2(Generic, wxLogLevel level);

// one function per each level
DECLARE_LOG_FUNCTION(Fatal);
DECLARE_LOG_FUNCTION(Error);
DECLARE_LOG_FUNCTION(Warning);
DECLARE_LOG_FUNCTION(Notice);
DECLARE_LOG_FUNCTION(Info);
DECLARE_LOG_FUNCTION(Verbose);

// this function sends the log message to the status line of the top level
// application frame, if any
DECLARE_LOG_FUNCTION(Debug);

// additional one: as wxLogError, but also logs last system call error code
// and the corresponding error message if available
DECLARE_LOG_FUNCTION(SysError);

// and another one which also takes the error code (for those broken APIs
// that don't set the errno (like registry APIs in Win32))
DECLARE_LOG_FUNCTION2(SysError, long lErrCode);

// are we in 'verbose' mode?
// (note that it's often handy to change this var manually from the
//  debugger, thus enabling/disabling verbose reporting for some
//  parts of the program only)
extern bool g_bVerbose;

// ----------------------------------------------------------------------------
// get error code/error message from system in a portable way
// ----------------------------------------------------------------------------

// return the last system error code
extern unsigned long wxSysErrorCode();
// return the error message for given (or last if 0) error code
extern const char* wxSysErrorMsg(unsigned long nErrCode = 0);

// ----------------------------------------------------------------------------
// debug only logging functions: use them with API name and error code
// ----------------------------------------------------------------------------

#ifdef  DEBUG
  #define wxLogApiError(api, rc)                                              \
                    wxLogDebug("At %s(%d) '%s' failed with error %lx (%s).",  \
                               __FILE__, __LINE__, api,                       \
                               rc, wxSysErrorMsg(rc))
  #define wxLogLastError(api) wxLogApiError(api, wxSysErrorCode())
#else   //!debug
  inline void wxLogApiError(const char *, long) { }
  inline void wxLogLastError(const char *) { }
#endif  //debug/!debug

#ifdef  DEBUG
  /**
  this function may be redefined to do something non trivial and is called
  whenever one of debugging macros fails (i.e. condition is false in an
  assertion)
  @param   szFile and nLine - file name and line number of the ASSERT
           szMsg            - optional message explaining the reason
  */
  void wxOnAssert(const char *szFile, int nLine, const char *szMsg = (const char *)NULL);

  /// generic assert macro
  #define   wxASSERT(cond)   if ( !(cond) ) wxOnAssert(__FILE__, __LINE__)

  /// assert with additional message explaining it's cause
  #define   wxASSERT_MSG(x, m)  if ( !(x) ) wxOnAssert(__FILE__, __LINE__, m)

#else
  // nothing to do in release modes (hopefully at this moment there are
  // no more bugs ;-)
  #define   wxASSERT(cond)
  #define   wxASSERT_MSG(x, m)
#endif  //__WXDEBUG__

  /// special form of assert: always triggers it (in debug mode)
#define   wxFAIL                 wxASSERT(false)

  /// FAIL with some message
#define   wxFAIL_MSG(msg)        wxASSERT_MSG(false, msg)

  /// check that expression is true, "return" if not (also FAILs in debug mode)
#define   wxCHECK(x, rc)            if (!(x)) {wxFAIL; return rc; }
  /// as wxCHECK but with a message explaining why we fail
#define   wxCHECK_MSG(x, rc, msg)   if (!(x)) {wxFAIL_MSG(msg); return rc; }
  /// check that expression is true, perform op if not
#define   wxCHECK2(x, op)           if (!(x)) {wxFAIL; op; }
  /// as wxCHECK2 but with a message explaining why we fail
#define   wxCHECK2_MSG(x, op, msg)  if (!(x)) {wxFAIL_MSG(msg); op; }
  /// special form of wxCHECK2: as wxCHECK, but for use in void functions
  //  NB: there is only one form (with msg parameter) and it's intentional:
  //      there is no other way to tell the caller what exactly went wrong
  //      from the void function (of course, the function shouldn't be void
  //      to begin with...)
#define   wxCHECK_RET(x, msg)       if (!(x)) {wxFAIL_MSG(msg); return; }

//#define _(str) str

#endif

