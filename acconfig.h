/* Define if the C++ compiler supports BOOL */
#undef HAVE_BOOL

#undef VERSION

#undef PACKAGE

/* Define if you need the GNU extensions to compile */
#undef _GNU_SOURCE

/* Define if you want to use DES encryption */
#ifndef HASCRYPT
#undef HASCRYPT
#endif

/* Define if your system can handle exceptions */
#ifndef MAGICK_HAS_EXCEPTIONS
#undef MAGICK_HAS_EXCEPTIONS
#endif

/* Define if your system's locking works */
#ifndef MAGICK_LOCKS_WORK
#undef MAGICK_LOCKS_WORK
#endif

/* Define if trace works on your system */
#ifndef MAGICK_TRACE_WORKS
#undef MAGICK_TRACE_WORKS
#endif

/* Define if you want to use mpatrol */
#ifndef MAGICK_USE_MPATROL
#undef MAGICK_USE_MPATROL
#endif

/* Define if you want to use Electric Fence */
#ifndef MAGICK_USE_EFENCE
#undef MAGICK_USE_EFENCE
#endif

/* Compile in the db conversion ability */
#ifndef CONVERT
#undef CONVERT
#endif

/* Define if we are on a windows system */
#ifndef WIN32
#undef WIN32
#endif

/* Length of database key */
#ifndef KEYLEN
#define KEYLEN 1024
#endif
