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
#ifndef _PCH_H
#define _PCH_H
// static const char *ident_pch_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.15  2000/05/20 16:05:07  prez
** Finished off the log conversion (still via. wrappers)
**
** Revision 1.14  2000/05/20 15:17:00  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.13  2000/05/17 09:10:35  ungod
** changed most wxOutputStream to ofstream and wxInputStream
** to ifstream
**
** Revision 1.12  2000/03/23 10:22:24  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.11  2000/03/19 08:50:53  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.10  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.9  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <ace/Activation_Queue.h>
#include <ace/Auto_Ptr.h>
#include <ace/Connector.h>
#include <ace/Event_Handler.h>
#include <ace/INET_Addr.h>
#include <ace/Local_Tokens.h>
#include <ace/Log_Msg.h>
#include <ace/Message_Queue.h>
#include <ace/Method_Object.h>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Singleton.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/Svc_Handler.h>
#include <ace/Synch_T.h>
#include <ace/Synch.h>
#include <ace/Task.h>
#include <ace/Thread_Manager.h>
#include <ace/Thread.h>


#include <algorithm>
#include <deque>
#include <exception>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <strstream>
#include <utility>
#include <vector>
using namespace std;

#include <zlib.h>

// THIS IS A TEMPORARY FIX!!
extern void wxLogFatal(const char *szFormat, ...);
extern void wxLogError(const char *szFormat, ...);
extern void wxLogWarning(const char *szFormat, ...);
extern void wxLogNotice(const char *szFormat, ...);
extern void wxLogInfo(const char *szFormat, ...);
extern void wxLogVerbose(const char *szFormat, ...);
extern void wxLogDebug(const char *szFormat, ...);
extern void wxLogSysError(const char *szFormat, ...);

#endif
