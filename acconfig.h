
/* Define if you want to test only (no connections) */
#ifndef TEST_MODE
#undef TEST_MODE
#endif

/* Define if you want to use encryption */
#ifndef HASCRYPT
#undef HASCRYPT
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

/* Enable two-way encryption on passwords */
#ifndef GETPASS
#undef GETPASS
#endif

/* Crypto key 1 placeholder */
#ifndef CRYPTO_KEY1
#define CRYPTO_KEY1 ""
#endif

/* Crypto key 2 placeholder */
#ifndef CRYPTO_KEY2
#define CRYPTO_KEY2 ""
#endif
