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
#ifndef _PCH_H
#define _PCH_H
// RCSID(pch_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.41  2001/06/17 19:29:53  prez
** Fixed pch again to support 5.1.0 aswell as 5.1.17 ACE
**
** Revision 1.40  2001/06/17 05:22:12  prez
** Resolved compatability issues with ACE 5.1.17
**
** Revision 1.39  2001/06/02 16:27:04  prez
** Intergrated the staging system for dbase loading/saving.
**
** Revision 1.38  2001/05/28 11:17:33  prez
** Added some more anti-deadlock stuff, and fixed nick ident warnings
**
** Revision 1.37  2001/05/01 14:00:22  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.36  2001/04/08 18:53:09  prez
** It now all compiles and RUNS with -fno-default-inline OFF.
**
** Revision 1.35  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.34  2001/03/08 14:34:29  prez
** Fixed some OS incompatabilities
**
** Revision 1.33  2001/03/08 08:07:40  ungod
** fixes for bcc 5.5
**
** Revision 1.32  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.30  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.29  2000/12/29 14:00:40  prez
** needed to include ace/Version.h
**
** Revision 1.28  2000/12/29 13:55:09  prez
** Compiled with 5.1.11, some changes to accomodate (will work with older
** versions of ace still).
**
** Revision 1.27  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.26  2000/12/09 11:15:11  prez
** Changed string.h -> strings.h for sunos compliance.
**
** Revision 1.25  2000/10/18 18:46:33  prez
** Well, mstring still coredumps, but it gets past the initial loading of
** all the STATIC (or const) strings, etc -- now its coring on loading a
** file (or possibly language.h or something).  Still investigating.
**
** Revision 1.24  2000/10/03 05:36:27  prez
** Updated some makefiles, helper stuff, and headers -- nothing
** too earth shattering.
**
** Revision 1.23  2000/09/30 10:48:06  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.22  2000/09/18 08:17:57  prez
** Intergrated mpatrol into the xml/des sublibs, and did
** some minor fixes as a result of mpatrol.
**
** Revision 1.21  2000/09/13 12:45:33  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
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

#ifdef STDC_HEADERS
#  ifdef  __cplusplus
#    include <cstdlib>
#    include <cstdarg>
#    include <cstring>
#  else
#    include <stdlib.h>
#    include <stdio.h>
#    include <string.h>
#  endif
#endif

/* Standard C Extensions */
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <sys/stat.h>

#if TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
#else
#  if HAVE_SYS_TIME_H
#    include <sys/time.h>
#  else
#    include <time.h>
#  endif
#endif

#ifdef WIN32
#  define WIN32_LEAN_AND_MEAN
#  define STRICT
#  include <windows.h>
#endif

/* Standard C++ Extensions ... */
/* These all have IFDEF's because
 * some unix machines dont have the
 * standard ones, only a .h
 */
#ifdef __cplusplus
#ifdef MAGICK_HAS_EXCEPTIONS
#  ifdef HAVE_EXCEPTION
#    include <exception>
#  else
#    include <exception.h>
#  endif
#endif
#ifdef HAVE_LIST
#  include <list>
#else
#  include <list.h>
#endif
#ifdef HAVE_MAP
#  include <map>
#else
#  include <map.h>
#endif
#ifdef HAVE_QUEUE
#  include <queue>
#else
#  include <queue.h>
#endif
#ifdef HAVE_STACK
#  include <stack>
#else
#  include <stack.h>
#endif
#ifdef HAVE_SET
#  include <set>
#else
#  include <set.h>
#endif
#ifdef HAVE_STRING
#  include <string>
#else
#  include <string.h>
#endif
#ifdef HAVE_UTILITY
#  include <utility>
#else
#  include <utility.h>
#endif
#ifdef HAVE_VECTOR
#  include <vector>
#else
#  include <vector.h>
#endif
#ifdef HAVE_IOSTREAM
#  include <iostream>
#else
#  include <iostream.h>
#endif

/* ACE Extensions */
#include <ace/config.h>
#include <ace/Version.h>
#include <ace/Reactor.h>
#include <ace/Connector.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/Activation_Queue.h>
/* Added in 5.1.11, so accomodate it ... */
#if ACE_MAJOR_VERSION > 5 || (ACE_MAJOR_VERSION == 5 && \
	(ACE_MINOR_VERSION > 1 || (ACE_MINOR_VERSION == 1 && \
	ACE_BETA_VERSION >= 11)))
#include <ace/Log_Msg_Callback.h>
#endif

/* DIR -> ACE_DIR in 5.1.16 */
#if !(ACE_MAJOR_VERSION > 5 || (ACE_MAJOR_VERSION == 5 && \
	(ACE_MINOR_VERSION > 1 || (ACE_MINOR_VERSION == 1 && \
	ACE_BETA_VERSION >= 16))))
typedef DIR ACE_DIR;
typedef struct utsname ACE_utsname;
#endif

/* Alter this when it is intergrated
 * Below is an example if it was implemented in 6.5 */
#if ACE_MAJOR_VERSION < 6 || (ACE_MAJOR_VERSION == 6 && ACE_MINOR_VERSION < 5)
#include "ace_memory.h"
#endif /* ifdef __cplusplus */

#define atoi(x)		ACE_OS::strtol(x, NULL, 10)
#define atol(x)		ACE_OS::strtol(x, NULL, 10)
#define atof(x)		ACE_OS::strtod(x, NULL)
#define atod(x)		ACE_OS::strtod(x, NULL)

/* ACE's linux exclusion is too large, so use the
 * underlying OS strtoul for this, else you'll get
 * a standard long that doesnt convert to an unsigned!
 *
#define atoui(x)	ACE_OS::strtoul(x, NULL, 10)
#define atoul(x)	ACE_OS::strtoul(x, NULL, 10)
*/
#define atoui(x)	strtoul(x, NULL, 10)
#define atoul(x)	strtoul(x, NULL, 10)

/* Debugging software -- if we dont use it, use ACE equivs */
#if defined(HAVE_MPATROL_H) && defined(MAGICK_USE_MPATROL)
#  include <mpatrol.h>
#else
#    define malloc	ACE_OS::malloc
#    define calloc	ACE_OS::calloc
// # define memalign	ACE_OS::memalign
// # define valloc	ACE_OS::valloc
// # define pvalloc	ACE_OS::pvalloc
#    define strdup	ACE_OS::strdup
// # define strndup	ACE_OS::strndup
// # define strsave	ACE_OS::strsave
// # define strnsave	ACE_OS::strnsave
#    define realloc	ACE_OS::realloc
// # define recalloc	ACE_OS::recalloc
// # define expand	ACE_OS::expand
#    define free	ACE_OS::free
// # define cfree	ACE_OS::cfree
// # define new		ACE_OS::new
// # define new[]	ACE_OS::new[]
// # define delete	ACE_OS::delete
// # define delete[]	ACE_OS::delete[]
#    define memset	ACE_OS::memset
// # define bzero	ACE_OS::bzero
// # define memccpy	ACE_OS::memccpy
#    define memcpy	ACE_OS::memcpy
#    define memmove	ACE_OS::memmove
// # define bcopy	ACE_OS::bcopy
#    define memchr	ACE_OS::memchr
// # define memmem	ACE_OS::memmem
#    define memcmp	ACE_OS::memcmp
// # define bcmp	ACE_OS::bcmp
#endif

using namespace std;

#else /* __cplusplus */

#if defined(HAVE_MPATROL_H) && defined(MAGICK_USE_MPATROL)
#  include <mpatrol.h>
#endif

#endif /* __cplusplus */

#include <zlib.h>

#endif
