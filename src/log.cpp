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

/////////////////////////////////////////////////////////////////////////////
// Name:        log.cpp
// Purpose:     Assorted wxLogXXX functions, and wxLog (sink for logs)
// Author:      Vadim Zeitlin
// Modified by: W. King for use with Magick II
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
  #pragma implementation "log.h"
#endif

#include "log.h"
#include <time.h>
#include <ctype.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// ----------------------------------------------------------------------------
// non member functions
// ----------------------------------------------------------------------------

// define this to enable wrapping of log messages
#define LOG_PRETTY_WRAP

#ifdef  LOG_PRETTY_WRAP
  static void wxLogWrap(FILE *f, const char *pszPrefix, const char *psz);
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// implementation of Log functions
//
// NB: unfortunately we need all these distinct functions, we can't make them
//     macros and not all compilers inline vararg functions.
// ----------------------------------------------------------------------------

// log functions can't allocate memory (LogError("out of memory...") should
// work!), so we use a static buffer for all log messages
#define LOG_BUFFER_SIZE   (4096)

// static buffer for error messages (@@@ MT-unsafe)
static char s_szBuf[LOG_BUFFER_SIZE];

// generic log function
void wxLogGeneric(wxLogLevel level, const char *szFormat, ...)
{
  if ( wxLog::GetActiveTarget() != NULL ) 
  {
    va_list argptr;
    va_start(argptr, szFormat);
    vsprintf(s_szBuf, szFormat, argptr);
    va_end(argptr);

    wxLog::OnLog(level, s_szBuf);
  }
}


#define IMPLEMENT_LOG_FUNCTION(level)                             \
  void wxLog##level(const char *szFormat, ...)                    \
  {                                                               \
    if ( wxLog::GetActiveTarget() != NULL ) {                     \
      va_list argptr;                                             \
      va_start(argptr, szFormat);                                 \
      vsprintf(s_szBuf, szFormat, argptr);                        \
      va_end(argptr);                                             \
                                                                  \
      wxLog::OnLog(wxLOG_##level, s_szBuf);                       \
    }                                                             \
  }

IMPLEMENT_LOG_FUNCTION(FatalError)
IMPLEMENT_LOG_FUNCTION(Error)
IMPLEMENT_LOG_FUNCTION(Warning)
IMPLEMENT_LOG_FUNCTION(Message)
IMPLEMENT_LOG_FUNCTION(Info)
IMPLEMENT_LOG_FUNCTION(Status)

// same as info, but only if 'verbose' mode is on
void wxLogVerbose(const char *szFormat, ...)
{
  wxLog *pLog = wxLog::GetActiveTarget();
  if ( pLog != NULL && pLog->GetVerbose() ) 
  {
    va_list argptr;
    va_start(argptr, szFormat);
    vsprintf(s_szBuf, szFormat, argptr);
    va_end(argptr);

    wxLog::OnLog(wxLOG_Info, s_szBuf);
  }
}

// debug functions
#ifdef DEBUG
#define IMPLEMENT_LOG_DEBUG_FUNCTION(level)                       \
  void wxLog##level(const char *szFormat, ...)                    \
  {                                                               \
    if ( wxLog::GetActiveTarget() != NULL ) {                     \
      va_list argptr;                                             \
      va_start(argptr, szFormat);                                 \
      vsprintf(s_szBuf, szFormat, argptr);                        \
      va_end(argptr);                                             \
                                                                  \
      wxLog::OnLog(wxLOG_##level, s_szBuf);                       \
    }                                                             \
  }

  void wxLogTrace(wxTraceMask mask, const char *szFormat, ...)
  {
    wxLog *pLog = wxLog::GetActiveTarget();

    // we check that all of mask bits are set in the current mask, so
    // that wxLogTrace(wxTraceRefCount | wxTraceOle) will only do something
    // if both bits are set.
    if ( pLog != NULL && ((pLog->GetTraceMask() & mask) == mask) ) 
	{
      va_list argptr;
      va_start(argptr, szFormat);
      vsprintf(s_szBuf, szFormat, argptr);
      va_end(argptr);

      wxLog::OnLog(wxLOG_Trace, s_szBuf);
    }
  }

#else // release
  #define IMPLEMENT_LOG_DEBUG_FUNCTION(level)
#endif

IMPLEMENT_LOG_DEBUG_FUNCTION(Debug)
IMPLEMENT_LOG_DEBUG_FUNCTION(Trace)

// wxLogSysError: one uses the last error code, for other  you must give it
// explicitly

// common part of both wxLogSysError
void wxLogSysErrorHelper(long lErrCode)
{
  char szErrMsg[LOG_BUFFER_SIZE / 2];
  sprintf(szErrMsg, _(" (error %ld: %s)"), lErrCode, wxSysErrorMsg(lErrCode));
  strncat(s_szBuf, szErrMsg, sizeof(s_szBuf) - strlen(s_szBuf));

  wxLog::OnLog(wxLOG_Error, s_szBuf);
}

void wxLogSysError(const char *szFormat, ...)
{
  va_list argptr;
  va_start(argptr, szFormat);
  vsprintf(s_szBuf, szFormat, argptr);
  va_end(argptr);

  wxLogSysErrorHelper(wxSysErrorCode());
}

void wxLogSysError(long lErrCode, const char *szFormat, ...)
{
  va_list argptr;
  va_start(argptr, szFormat);
  vsprintf(s_szBuf, szFormat, argptr);
  va_end(argptr);

  wxLogSysErrorHelper(lErrCode);
}

// ----------------------------------------------------------------------------
// wxLog class implementation
// ----------------------------------------------------------------------------

wxLog::wxLog()
{
  m_bHasMessages = false;
  m_bVerbose     = false;
  m_szTimeFormat = "[%d/%b/%y %H:%M:%S] ";
}

wxLog *wxLog::GetActiveTarget()
{
  if ( ms_bAutoCreate && ms_pLogger == NULL ) 
  {
    // prevent infinite recursion if someone calls wxLogXXX() from
    // wxApp::CreateLogTarget()
    static bool s_bInGetActiveTarget = false;
    if ( !s_bInGetActiveTarget ) 
	{
      s_bInGetActiveTarget = true;

        ms_pLogger = new wxLogStderr;

      s_bInGetActiveTarget = false;

      // do nothing if it fails - what can we do?
    }
  }

  return ms_pLogger;
}

wxLog *wxLog::SetActiveTarget(wxLog *pLogger)
{
  if ( ms_pLogger != NULL ) {
    // flush the old messages before changing because otherwise they might
    // get lost later if this target is not restored
    ms_pLogger->Flush();
  }

  wxLog *pOldLogger = ms_pLogger;
  ms_pLogger = pLogger;

  return pOldLogger;
}

mstring wxLog::TimeStamp() const
{

  mstring str;
  if ( m_szTimeFormat!="" ) {
    char szBuf[128];
    time_t timeNow;
    struct tm *ptmNow;

    time(&timeNow);
    ptmNow = localtime(&timeNow);

    strftime(szBuf, sizeof(szBuf), m_szTimeFormat, ptmNow);
    str = szBuf;
  }

  return str;
}

void wxLog::DoLog(wxLogLevel level, const char *szString)
{
  // prepend a timestamp if not disabled
  mstring str = TimeStamp();

  switch ( level ) {
    case wxLOG_FatalError:
      DoLogString(str << _("Fatal error: ") << szString);
      DoLogString(_("Program aborted."));
      Flush();
      abort();
      break;

    case wxLOG_Error:
      DoLogString(str << _("Error: ") << szString);
      break;

    case wxLOG_Warning:
      DoLogString(str << _("Warning: ") << szString);
      break;

    case wxLOG_Info:
      if ( GetVerbose() )
    case wxLOG_Message:
        DoLogString((str + szString).c_str());
      // fall through

    case wxLOG_Status:
      // nothing to do
      break;

    case wxLOG_Trace:
    case wxLOG_Debug:
      #ifdef __WXDEBUG__
      //        DoLogString(str << (level == wxLOG_Trace ? _("Trace") : _("Debug"))
      //                        << ": " << szString);
      // JACS: we don't really want to prefix with 'Debug'. It's just extra
      // verbiage.
              DoLogString(szString);
      #endif

      break;

    default:
      wxFAIL_MSG(_("unknown log level in wxLog::DoLog"));
  }
}

void wxLog::DoLogString(const char *szString)
{
  wxFAIL_MSG(_("DoLogString must be overrided if it's called."));
}

void wxLog::Flush()
{
  // do nothing
}

// ----------------------------------------------------------------------------
// wxLogStderr class implementation
// ----------------------------------------------------------------------------

wxLogStderr::wxLogStderr(FILE *fp)
{
	if ( fp == NULL )
		m_fp = stderr;
	else
		m_fp = fp;
	wxLog::SetActiveTarget(this);
	wxLog::EnableLogging(true);
}

void wxLogStderr::ChangeFile(FILE *fp)
{
  if ( fp == NULL )
    m_fp = stderr;
  else
    m_fp = fp;
}

void wxLogStderr::DoLogString(const char *szString)
{
  fputs(szString, m_fp);
  fputc('\n', m_fp);
  fflush(m_fp);
}

wxLogStderr::~wxLogStderr()
{
	wxLog::EnableLogging(false);
	wxLog::SetActiveTarget(NULL);
}

// ============================================================================
// Global functions/variables
// ============================================================================

// ----------------------------------------------------------------------------
// static variables
// ----------------------------------------------------------------------------
wxLog      *wxLog::ms_pLogger      = (wxLog *) NULL;
bool        wxLog::ms_doLog        = true;
bool        wxLog::ms_bAutoCreate  = true;
wxTraceMask wxLog::ms_ulTraceMask  = (wxTraceMask)0;

// ----------------------------------------------------------------------------
// stdout error logging helper
// ----------------------------------------------------------------------------

// helper function: wraps the message and justifies it under given position
// (looks more pretty on the terminal). Also adds newline at the end.
//
// @@ this is now disabled until I find a portable way of determining the
//    terminal window size (ok, I found it but does anybody really cares?)
#ifdef    LOG_PRETTY_WRAP
static void wxLogWrap(FILE *f, const char *pszPrefix, const char *psz)
{
  size_t nMax = 80; // @@@@
  size_t nStart = strlen(pszPrefix);
  fputs(pszPrefix, f);

  size_t n;
  while ( *psz != '\0' ) {
    for ( n = nStart; (n < nMax) && (*psz != '\0'); n++ )
      putc(*psz++, f);

    // wrapped?
    if ( *psz != '\0' ) {
      /*putc('\n', f);*/
      for ( n = 0; n < nStart; n++ )
        putc(' ', f);

      // as we wrapped, squeeze all white space
      while ( isspace(*psz) )
        psz++;
    }
  }

  putc('\n', f);
}
#endif  //LOG_PRETTY_WRAP

// ----------------------------------------------------------------------------
// error code/error message retrieval functions
// ----------------------------------------------------------------------------

// get error code from syste
unsigned long wxSysErrorCode()
{
#ifdef  WIN32
    return ::GetLastError();
#else
    return errno;
#endif  //Win/Unix
}

// get error message from system
const char *wxSysErrorMsg(unsigned long nErrCode)
{
  if ( nErrCode == 0 )
    nErrCode = wxSysErrorCode();

    #ifdef  WIN32
      static char s_szBuf[LOG_BUFFER_SIZE / 2];

      // get error message from system
      LPVOID lpMsgBuf;
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL, nErrCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR)&lpMsgBuf,
                    0, NULL);

      // copy it to our buffer and free memory
      strncpy(s_szBuf, (const char *)lpMsgBuf, sizeof(s_szBuf) - 1);
      s_szBuf[sizeof(s_szBuf) - 1] = '\0';
      LocalFree(lpMsgBuf);

      // returned string is capitalized and ended with '\r\n' - bad
      s_szBuf[0] = (char)tolower(s_szBuf[0]);
      size_t len = strlen(s_szBuf);
      if ( len > 0 ) {
        // truncate string
        if ( s_szBuf[len - 2] == '\r' )
          s_szBuf[len - 2] = '\0';
      }

      return s_szBuf;
  #else   // Unix
    return strerror(nErrCode);
  #endif  // Win/Unix
}

// ----------------------------------------------------------------------------
// debug helper
// ----------------------------------------------------------------------------

#ifdef  DEBUG

void Trap()
{
#ifdef WIN32
    DebugBreak();
#else // Unix
    raise(SIGTRAP);
#endif // Win/Unix
}

// this function is called when an assert fails
void wxOnAssert(const char *szFile, int nLine, const char *szMsg)
{
  // this variable can be set to true to suppress "assert failure" messages
  static bool s_bNoAsserts = false;
  static bool s_bInAssert = false;

  if ( s_bInAssert ) {
    // He-e-e-e-elp!! we're trapped in endless loop
    Trap();

    return;
  }

  s_bInAssert = true;

  char szBuf[LOG_BUFFER_SIZE];
  sprintf(szBuf, _("Assert failed in file %s at line %d"), szFile, nLine);
  if ( szMsg != NULL ) {
    strcat(szBuf, ": ");
    strcat(szBuf, szMsg);
  }
  else {
    strcat(szBuf, ".");
  }

  if ( !s_bNoAsserts ) {
    // send it to the normal log destination
    wxLogDebug(szBuf);

      Trap();
  }

  s_bInAssert = false;
}

#endif  //DEBUG
