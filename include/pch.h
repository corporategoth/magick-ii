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
** Revision 1.20  2000/05/28 05:20:46  prez
** More stuff ..
**
** Revision 1.19  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.18  2000/05/27 15:14:45  prez
** *** empty log message ***
**
** Revision 1.17  2000/05/27 15:10:10  prez
** Misc changes, mainly re-did the makefile system, makes more sense.
** Also added a config.h file.
**
** Revision 1.16  2000/05/21 04:49:38  prez
** Removed all wxLog tags, now totally using our own logging.
**
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

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

/* Standard C Extensions */
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef STDC_HEADERS
#  include <stdlib.h>
#  include <stdarg.h>
#  include <string.h>
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/* ACE Extensions */
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

/* Standard C++ Extensions ... */
/* These all have IFDEF's because
 * unix machines tend to use .h files
 */
#ifdef MAGICK_HAS_EXCEPTIONS
#  ifdef HAVE_EXCEPTION_H
#    include <exception.h>
#  else
#    include <exception>
#  endif
#endif
#ifdef HAVE_ALGORITHM_H
#  include <algorithm.h>
#else
#  include <algorithm>
#endif
#ifdef HAVE_DEQUE_H
#  include <deque.h>
#else
#  include <deque>
#endif
#ifdef HAVE_IOSTREAM_H
#  include <iostream.h>
#else
#  include <iostream>
#endif
#ifdef HAVE_FSTREAM_H
#  include <fstream.h>
#else
#  include <fstream>
#endif
#ifdef HAVE_LIST_H
#  include <list.h>
#else
#  include <list>
#endif
#ifdef HAVE_MAP_H
#  include <map.h>
#else
#  include <map>
#endif
#ifdef HAVE_QUEUE_H
#  include <queue.h>
#else
#  include <queue>
#endif
#ifdef HAVE_STACK_H
#  include <stack.h>
#else
#  include <stack>
#endif
#ifdef HAVE_SET_H
#  include <set.h>
#else
#  include <set>
#endif
#ifdef HAVE_STRING_H
#  include <string.h>
#else
#  include <string>
#endif
#ifdef HAVE_STRSTREAM_H
#  include <strstream.h>
#else
#  include <strstream>
#endif
#ifdef HAVE_UTILITY_H
#  include <utility.h>
#else
#  include <utility>
#endif
#ifdef HAVE_VECTOR_H
#  include <vector.h>
#else
#  include <vector>
#endif
using namespace std;

#include <zlib.h>

extern size_t Log(ACE_Log_Priority priority, const char *messages, ...);
extern size_t LogV(ACE_Log_Priority priority, const char *messages, va_list argptr);

#endif
