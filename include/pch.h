
/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#ifndef _PCH_H
#define _PCH_H
// RCSID(pch_h, "@(#) $Id$");

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

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

/* Set us up, so that everything else knows about thread safety,
 * extensions, etc.  Most of these options are the same as ACE.
 */
#define _REENTRANT
#define _THREAD_SAFE
#ifdef __cplusplus
#include <ace/config.h>
#include <ace/Version.h>
#ifdef ACE_HAS_PTHREADS
#  define __STL_PTHREADS
#endif
#ifdef ACE_HAS_STHREADS
#  define __STL_UITHREADS
#endif
#ifdef ACE_HAS_IRIX62_THREADS
#  define __STL_SGI_THREADS
#endif
#ifdef ACE_HAS_WTHREADS
#  define __STL_WIN32THREADS
#endif
#ifdef HAVE_BITS_STL_CONFIG_H
#  include <bits/stl_config.h>
#else
#  ifdef HAVE_STL_CONFIG_H
#    include <stl_config.h>
#  endif
#endif
#endif /* __cplusplus */

#ifdef STDC_HEADERS
#  ifdef  __cplusplus
#    include <cstdlib>
#    include <cstddef>
#    include <cstdarg>
#    include <cstring>
#  else
#    include <stdlib.h>
#    include <stddef.h>
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
#  include <windows.h>
#endif

/* Standard C++ Extensions ... */

/* These all have IFDEF's because
 * some unix machines dont have the
 * standard ones, only a .h
 */
#ifdef __cplusplus
#ifndef HAVE_BOOL
enum bool
{ false, true };
#endif
#ifdef ACE_HAS_EXCEPTIONS
#  define MAGICK_HAS_EXCEPTIONS
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

/* ACE Extensions */
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

/* Alter this when it is intergrated */
#include "ace_memory.h"

#define atoi(x)		ACE_OS::strtol(x, NULL, 10)
#define atol(x)		ACE_OS::strtol(x, NULL, 10)
#define atof(x)		ACE_OS::strtod(x, NULL)
#define atod(x)		ACE_OS::strtod(x, NULL)

/* ACE cast strtoul to long on Linux until v5.1.5 */
#if ACE_MAJOR_VERSION > 5 || (ACE_MAJOR_VERSION == 5 && \
	(ACE_MINOR_VERSION > 1 || (ACE_MINOR_VERSION == 1 && \
	ACE_BETA_VERSION >= 5)))
#define atoui(x)	strtoul(x, NULL, 10)
#define atoul(x)	strtoul(x, NULL, 10)
#else
#define atoui(x)	ACE_OS::strtoul(x, NULL, 10)
#define atoul(x)	ACE_OS::strtoul(x, NULL, 10)
#endif

/* Debugging software -- if we dont use it, use ACE equivs */
#if defined(HAVE_MPATROL_H) && defined(MAGICK_USE_MPATROL)
#  include <mpatrol.h>
#else
#    define malloc	ACE_OS::malloc
#    define calloc	ACE_OS::calloc
// # define memalign    ACE_OS::memalign
// # define valloc      ACE_OS::valloc
// # define pvalloc     ACE_OS::pvalloc
#    define strdup	ACE_OS::strdup
// # define strndup     ACE_OS::strndup
// # define strsave     ACE_OS::strsave
// # define strnsave    ACE_OS::strnsave
#    define realloc	ACE_OS::realloc
// # define recalloc    ACE_OS::recalloc
// # define expand      ACE_OS::expand
#    define free	ACE_OS::free
// # define cfree       ACE_OS::cfree
// # define new         ACE_OS::new
// # define new[]       ACE_OS::new[]
// # define delete      ACE_OS::delete
// # define delete[]    ACE_OS::delete[]
#    define memset	ACE_OS::memset
// # define bzero       ACE_OS::bzero
// # define memccpy     ACE_OS::memccpy
#    define memcpy	ACE_OS::memcpy
#    define memmove	ACE_OS::memmove
// # define bcopy       ACE_OS::bcopy
#    define memchr	ACE_OS::memchr
// # define memmem      ACE_OS::memmem
#    define memcmp	ACE_OS::memcmp
// # define bcmp        ACE_OS::bcmp
#endif

using namespace std;

#else /* __cplusplus */

#if defined(HAVE_MPATROL_H) && defined(MAGICK_USE_MPATROL)
#  include <mpatrol.h>
#endif

#endif /* __cplusplus */

#include <zlib.h>

#endif
