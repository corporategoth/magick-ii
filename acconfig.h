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

/* Define if we are on a windows system */
#ifndef WIN32
#undef WIN32
#endif

/* Amount of chars to rotate crypto keys by */
#ifndef CRYPTO_SCRAMBLE
#define CRYPTO_SCRAMBLE 0
#endif

/* Keyfile key 1 (rotated) */
#ifndef CRYPTO_KEY1
#define CRYPTO_KEY1 "localhost"
#endif

/* Keyfile key 2 (rotated) */
#ifndef CRYPTO_KEY2
#define CRYPTO_KEY2 "Windows"
#endif

/* Length of database key */
#ifndef KEYLEN
#define KEYLEN 1024
#endif
