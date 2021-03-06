/* Define if you have the type bool */
#ifndef HAVE_BOOL
#undef HAVE_BOOL
#endif

/* Define if you want to test only (no connections) */
#ifndef TEST_MODE
#undef TEST_MODE
#endif

/* Define if you want to use encryption */
#ifndef HASCRYPT
#undef HASCRYPT
#endif

/* Define if you want to use JP2 (XOR) for passwords */
#ifndef JP2CRYPT
#undef JP2CRYPT
#endif

/* Define if you want to use the DES form of crypt() */
#ifndef DESCRYPT
#undef DESCRYPT
#endif

/* Define if you want to use the MD5 form of crypt() */
#ifndef MD5CRYPT
#undef MD5CRYPT
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

/* Can we use 'return f()' where f has a void return type. */
#ifndef HAVE_RETURN_VOID
#undef HAVE_RETURN_VOID
#endif

/* Do we need versions of mem_fun (and friends) that accept const arguments */
#ifndef HAVE_MEM_FUN_CONST
#undef HAVE_MEM_FUN_CONST
#endif

/* Do we need to use mem_fun1 (and mem_fun1_ref) when we want a binary_function */
#ifndef HAVE_MEM_FUN1
#undef HAVE_MEM_FUN1
#endif

/* Crypto key 1 placeholder */
#ifndef CRYPTO_KEY1
#define CRYPTO_KEY1 ""
#endif

/* Crypto key 2 placeholder */
#ifndef CRYPTO_KEY2
#define CRYPTO_KEY2 ""
#endif

