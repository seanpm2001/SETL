/*  ===  Declarations shared by programs in the SETL package  ======  */

/*  $Id: common.h,v 1.52 2022/12/10 23:35:26 setlorg Exp $  */

/*  Free software (c) dB - see file COPYING for license (GPL).  */

#ifndef _common_h
#define _common_h

/*
 *  We only require system support at the level of POSIX.1-2001 plus
 *  the XSI extension.  This equates to SUSv3 (UNIX 03), and to The
 *  Open Group Base Specifications Issue 6, IEEE Std 1003.1, 2004 Edition.
 */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

/*
 *  Pick up configuration data generated by "configure":
 */
#if HAVE_CONFIG_H
#include "config.h"
#endif

/*
 *  Pathname element separator:
 */
#ifndef DIRSEP
#define DIRSEP  "/"
#endif
#define dirsepchar  (DIRSEP[0])
/*
 *  Search path component separator:
 */
#ifndef PATHSEP
#define PATHSEP  ":"
#endif
#define pathsepchar  (PATHSEP[0])

/*
 *  Adapted from a tutorial at dbp-consulting.com.
 *
 *  Example, giving "-Wno-strict-prototypes" through a region:
 *
 *  GCC_DIAG_OFF(strict-prototypes)
 *   ...
 *  GCC_DIAG_ON(strict-prototypes)
 */
#if HAVE_PRAGMA_GCC_DIAGNOSTIC
# define GCC_DIAG_STR(s)        #s
# define GCC_DIAG_JOINSTR(x,y)  GCC_DIAG_STR(x ## y)
# define GCC_DIAG_DO_PRAGMA(x)  _Pragma (#x)
# define GCC_DIAG_PRAGMA(x)     GCC_DIAG_DO_PRAGMA(GCC diagnostic x)
/* If the push/pop pragmas are available, this mechanism can only
 * disable a warning through a region, not enable one that is off in
 * the environment - and that is how it is meant to be used.  */
# if HAVE_PRAGMA_GCC_DIAGNOSTIC_PUSHPOP
#  define GCC_DIAG_OFF(x)  GCC_DIAG_PRAGMA(push) \
                           GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
#  define GCC_DIAG_ON(x)   GCC_DIAG_PRAGMA(pop)
# else
#  define GCC_DIAG_OFF(x)  GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
#  define GCC_DIAG_ON(x)   GCC_DIAG_PRAGMA(warning GCC_DIAG_JOINSTR(-W,x))
# endif
#else
# define GCC_DIAG_OFF(x)
# define GCC_DIAG_ON(x)
#endif

/*
 *  For rare occasions where we wish to give branch prediction opinions:
 */
#if HAVE_BUILTIN_EXPECT
#define LIKELY(x)    __builtin_expect (x, true)
#define UNLIKELY(x)  __builtin_expect (x, false)
#else
#define LIKELY(x)    (x)
#define UNLIKELY(x)  (x)
#endif

/*
 *  Many of our conditional inclusions of system headers could be
 *  unconditional on the assumption of POSIX support.
 */

/*
 *  Try to get _POSIX_VERSION defined legitimately:
 */
#if HAVE_UNISTD_H
#include <sys/types.h>
#include <unistd.h>
#endif
/*
 *  The above #if should have caused _POSIX_VERSION to be defined if we
 *  are on a POSIX.1 system.  Herewith the notes from autoconf.info:
 *
 *  `_POSIX_VERSION' is defined when `unistd.h' is included on POSIX.1
 *  systems.  If there is no `unistd.h', it is definitely not a
 *  POSIX.1 system.  However, some non-POSIX.1 systems do have
 *  `unistd.h'.
 */

/*
 *  Some standard headers:
 */
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <stdio.h>
#include <ctype.h>
#if HAVE_STDARG_H
#include <stdarg.h>
#endif
#if HAVE_STDDEF_H
#include <stddef.h>
#endif

/*
 *  Make sure we have a va_copy() macro:
 */
#ifndef va_copy
# ifdef __va_copy
#  define va_copy(dst,src)  __va_copy(dst,src)
# else  /* per suggestion in Autoconf 2.69 manual:  */
#  define va_copy(dst,src)  memcpy(&(dst), &(src), sizeof (va_list))
# endif
#endif

/*
 *  C stack allocator (this way of getting it is from autoconf.info):
 */
#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#elif !defined alloca
# ifdef __GNUC__
#  define alloca  __builtin_alloca
# elif defined _AIX
#  define alloca  __alloca
# elif defined _MSC_VER
#  include <malloc.h>
#  define alloca  _alloca
# elif !defined HAVE_ALLOCA
#  ifdef  __cplusplus
extern "C"
#  endif
void *alloca (size_t);
# endif
#endif

/*
 *  In case we don't have <stddef.h>, make sure this macro is #defined:
 */
#ifndef offsetof
/* Surprising, as offsetof was in C89 already...  */
#warning redefining offsetof
#define offsetof(T,m)  ((size_t)((char *)&((T *)(0))->m - (char *)0))
#endif

/*
 *  Number of elements in a fixed-size array:
 */
#define numberof(a)  (sizeof (a) / sizeof ((a)[0]))

/*
 *  Size of element of array member m in struct type T:
 */
#define elmtsize(T,m)  (sizeof ((T *)0)->m[0])

/*
 *  Spam:
 */
#if HAVE_STDINT_H
#include <stdint.h>
#endif
#if HAVE_INTTYPES_H
#include <inttypes.h>
#endif
/* Adapted from autoconf.info:  */
#if HAVE_STDBOOL_H
# include <stdbool.h>
#else
# if ! HAVE__BOOL
typedef unsigned char  _Bool;
# endif
# define bool  _Bool
# define false  0
# define true  1
# define __bool_true_false_are_defined  1
#endif

/*
 *  Internal consistency checking:
 */
#ifdef USE_SYSTEM_ASSERT
#include <assert.h>
#endif

/*
 *  String/memory operations:
 */
#if HAVE_MEMORY_H
# include <memory.h>
#endif
/* This #if is adapted from autoconf.info (as of autoconf 2.10):  */
#if STDC_HEADERS
# include <string.h>
#else
# ifndef HAVE_STRCHR
#  define strchr  index
#  define strrchr  rindex
# endif
char *strchr(), *strrchr();
# ifndef HAVE_MEMCPY
#  define memcpy(d,s,n)  bcopy(s,d,n)
# endif
#endif

#if HAVE_STRINGS_H
/* For strcasecmp(), for example */
#include <strings.h>
#endif

#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#if HAVE_ERRNO_H
#include <errno.h>
#endif

#include <time.h>  /* everyone has this! */

#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

/*
 *  This is a useful struct to have even on very non-BSD systems:
 */
#if !STRUCT_TIMEVAL_IN_TIME_H && !STRUCT_TIMEVAL_IN_SYS_TIME_H && \
    !STRUCT_TIMEVAL_IN_LINUX_TIME_H
struct timeval {
  time_t  tv_sec;   /* seconds */
  long   tv_usec;   /* microseconds */
};
#endif
#define TV_SEC_BIT  (CHAR_BIT * sizeof ((struct timeval *)0)->tv_sec)
/*
 *  (That was some time ago.  In the case of struct timespec, we
 *  assume it comes in with <time.h>, per POSIX for a while now.)
 */

#if HAVE_SYS_TIMES_H
#include <sys/times.h>
#endif

/*
 *  Put this before sys/wait.h if you don't want AIX to give you pains:
 */
#if HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
# if !STRUCT_RUSAGE_IN_SYS_RESOURCE_H
#  if HAVE__USR_UCBINCLUDE_SYS_RUSAGE_H
#   include </usr/ucbinclude/sys/rusage.h>
#  endif
# endif
#elif HAVE_SYS_RUSAGE_H
# include <sys/rusage.h>
#endif

#if HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif

/*
 *  The kid glove treatment of <sys/wait.h>, per autoconf.info:
 */
#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val)  ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
# define WIFEXITED(stat_val)  (((stat_val) & 255) == 0)
#endif

/*
 *  This declares struct stat, several IS_S... macros to go with it,
 *  and functions chmod(), fchmod(), fstat(), lstat(), mkdir(),
 *  mkfifo(), mknod(), stat(), and umask():
 */
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

/*
 *  This declares glob_t, glob(), and globfree():
 */
#if HAVE_GLOB_H
#include <glob.h>
#endif

#include <fcntl.h>

#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#if HAVE_SYS_SIGNAL_H
#include <sys/signal.h>
#endif

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#if HAVE_GRP_H
#include <grp.h>
#endif

#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#if HAVE_NETINET_IN_H
#include <netinet/in.h>
/* Because otherwise 'n_long' may be undefined (e.g., in Solaris):  */
#if HAVE_NETINET_IN_SYSTM_H
#include <netinet/in_systm.h>
#endif
#include <netinet/ip.h>
#if HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif
#endif

#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#if HAVE_SYS_UN_H
#include <sys/un.h>
#endif

#if HAVE_NETDB_H
#include <netdb.h>
#endif
#ifndef NI_MAXHOST
#define NI_MAXHOST  1025
#endif
#ifndef NI_MAXSERV
#define NI_MAXSERV  32
#endif

#include <math.h>

/*
 *  Roughly the limit on how big a buffer we are willing to
 *  allocate for things like pathnames on the stack:
 */
#ifndef PATH_MAX
#define PATH_MAX  4096
#endif

/*
 *  The PATRICIA code, at least, needs updating if this ceases to hold:
 */
#ifdef CHAR_BIT
# if CHAR_BIT != 8
#  error CHAR_BIT is not 8!
# endif
#else
# warning Defining CHAR_BIT as 8 by assumption
# define CHAR_BIT  8  /* by assumption */
#endif

#define CHARSETSIZE  (1<<CHAR_BIT)

#if HAVE_LONG_LONG_INT != HAVE_UNSIGNED_LONG_LONG_INT
#error How can we have "long long" but not "unsigned long long" or converse?
#endif
/* Now can use HAVE_LONG_LONG_INT as the test for both.  */
#define HAVE_LLONG  HAVE_LONG_LONG_INT  /* abbrev. */

#ifdef SIZEOF_LONG_LONG
#define SIZEOF_LLONG  SIZEOF_LONG_LONG
#else
#define SIZEOF_LLONG  (-1)
#endif

#ifdef ALIGNOF_LONG_LONG
#define ALIGNOF_LLONG  ALIGNOF_LONG_LONG
#else
#define ALIGNOF_LLONG  0
#endif

#ifndef INT_BIT
#define INT_BIT  (CHAR_BIT * SIZEOF_INT)
#endif

#ifndef LONG_BIT
#define LONG_BIT  (CHAR_BIT * SIZEOF_LONG)
#endif

#ifndef LLONG_BIT
#define LLONG_BIT  (CHAR_BIT * SIZEOF_LLONG)
#endif

/*
 *  A size_t-sized word with just the high-order bit set, useful for
 *  biasing an ssize_t to put in a size_t:
 */
#define SIZE_T_HIGHBIT  (~(size_t)0 ^ (~(size_t)0 >> 1))

/*
 *  ENDIAN_BE*(x) for converting numbers to and from big-endian layout:
 */
#if HAVE_ENDIAN_H
#include <endian.h>
#endif
#if HAVE_BYTESWAP_H
#include <byteswap.h>
#endif
#if HAVE_GULLIVER_H  /* e.g., on QNX Neutrino */
#include <gulliver.h>
#endif
#if defined(_BIG_ENDIAN) || \
   (defined(__BYTE_ORDER) && (__BYTE_ORDER == __BIG_ENDIAN)) || \
   (defined(BYTE_ORDER) && (BYTE_ORDER == BIG_ENDIAN)) || \
    defined(WORDS_BIGENDIAN)
# ifndef ENDIAN_BE16
# define ENDIAN_BE16(x)  (x)
# endif
# ifndef ENDIAN_BE32
# define ENDIAN_BE32(x)  (x)
# endif
# ifndef ENDIAN_BE64
# define ENDIAN_BE64(x)  (x)
# endif
#elif defined(_LITTLE_ENDIAN) || \
     (defined(__BYTE_ORDER) && (__BYTE_ORDER == __LITTLE_ENDIAN)) || \
     (defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN))
# ifndef bswap_16  /* assume if bswap_16 is missing, they all are */
#  if HAVE_BUILTIN_BSWAP
#  define bswap_16(x)  __builtin_bswap16(x)
#  define bswap_32(x)  __builtin_bswap32(x)
#  define bswap_64(x)  __builtin_bswap64(x)
#  elif HAVE_LIBKERN_OSBYTEORDER_H  /* e.g., on Macintel */
#  include <libkern/OSByteOrder.h>
#  define bswap_16(x)  OSSwapInt16(x)
#  define bswap_32(x)  OSSwapInt32(x)
#  define bswap_64(x)  OSSwapInt64(x)
#  endif
# endif
# ifndef ENDIAN_BE16
# define ENDIAN_BE16(x)  (bswap_16(x))
# endif
# ifndef ENDIAN_BE32
# define ENDIAN_BE32(x)  (bswap_32(x))
# endif
# ifndef ENDIAN_BE64
# define ENDIAN_BE64(x)  (bswap_64(x))
# endif
#endif

#if SIZEOF_INT == 2
#define BE_INT(x)  ENDIAN_BE16(x)
#elif SIZEOF_INT == 4
#define BE_INT(x)  ENDIAN_BE32(x)
#elif SIZEOF_INT == 8
#define BE_INT(x)  ENDIAN_BE64(x)
#endif

#if SIZEOF_LONG == 4
#define BE_LONG(x)  ENDIAN_BE32(x)
#elif SIZEOF_LONG == 8
#define BE_LONG(x)  ENDIAN_BE64(x)
#endif

#if SIZEOF_LLONG == 8
#define BE_LLONG(x)  ENDIAN_BE64(x)
#endif

#if SIZEOF_SIZE_T == 4
#define BE_SIZE_T(x)  ENDIAN_BE32(x)
#elif SIZEOF_SIZE_T == 8
#define BE_SIZE_T(x)  ENDIAN_BE64(x)
#endif

#if SIZEOF_DOUBLE == SIZEOF_LONG
#define BE_DOUBLE(x)  BE_LONG(x)
#define int_double_holder  long
#elif SIZEOF_DOUBLE == SIZEOF_LLONG
#define BE_DOUBLE(x)  BE_LLONG(x)
#define int_double_holder  long long
#endif

/*
 *  Blatantly stolen from the stdint.h file of star-1.5.3 and
 *  modified only a tiny bit.
 */
/*
 * These macros may not work on all platforms but as we depend
 * on two's complement in many places, they do not reduce portability.
 * The macros below work with 2s complement and ones complement machines.
 * Verify with this table...
 *
 *  Bits  1's c.  2's complement.
 *  100     -3      -4
 *  101     -2      -3
 *  110     -1      -2
 *  111     -0      -1
 *  000     +0       0
 *  001     +1      +1
 *  010     +2      +2
 *  011     +3      +3
 *
 * Computing -TYPE_MINVAL(type) will not work on 2's complement machines
 * if 'type' is int or more. Use -(UIntmax_t)TYPE_MINVAL(type), it works
 * for both 1's complement and 2's complement machines.
 */
/* The original TYPE_ISSIGNED defn gave "warning: comparison of unsigned
 * expression < 0 is always false [-Wtype-limits]" under GCC:  */
/* #define TYPE_ISSIGNED(t) (((t)-1) < ((t)0)) */
#define TYPE_ISSIGNED(t)    ((t)-1 < (t)1)
#define TYPE_ISUNSIGNED(t)  (!TYPE_ISSIGNED(t))
/* The original TYPE_MSBVAL defn gave "warning: left shift of negative
 * value [-Wshift-negative-value]" under GCC:  */
/* #define TYPE_MSBVAL(t)   ((t)(~((t)0) << (sizeof (t)*CHAR_BIT - 1))) */
#define TYPE_MSBVAL(t)      ((t)1 << (sizeof (t)*CHAR_BIT - 1))
#define TYPE_MINVAL(t)      (TYPE_ISSIGNED(t)           \
                            ? TYPE_MSBVAL(t)            \
                            : ((t)0))
#define TYPE_MAXVAL(t)      ((t)(~((t)0) - TYPE_MINVAL(t)))

/*
 *  Please put NO_RETURN in the declaration of a function, just before
 *  the trailing semicolon, if that function does not return but
 *  instead calls exit(), abort(), etc.
 */
#ifndef NO_RETURN
 #if HAVE_ATTRIBUTE_NORETURN
  #define NO_RETURN  __attribute__ ((noreturn))
 #else
  #define NO_RETURN
 #endif
#endif

#ifndef PRINTF_ATTRIBUTE
 #if HAVE_ATTRIBUTE_FORMAT_PRINTF
  #define PRINTF_ATTRIBUTE(i, j)  __attribute__ ((format (printf, i, j)))
 #else
  #define PRINTF_ATTRIBUTE(i, j)
 #endif
#endif
/*
 *  Please put LIKE_PRINTF just before the semicolon in a function
 *  declaration if its signature resembles that of printf(3) in having
 *  a printf-type format in the first arg and format-dependent args
 *  after that.  Likewise use LIKE_FPRINTF if the printf-type format
 *  is in the second arg (like fprintf(3)), LIKE_VPRINTF if the
 *  function signature resembles that of vprintf(3) (the return type
 *  does not matter for any of these), and LIKE_VFPRINTF for a
 *  vfprintf(3)-like function.  Or just use PRINTF_ATTRIBUTE directly.
 */
#ifndef LIKE_PRINTF
#define LIKE_PRINTF PRINTF_ATTRIBUTE(1, 2)
#endif
#ifndef LIKE_FPRINTF
#define LIKE_FPRINTF PRINTF_ATTRIBUTE(2, 3)
#endif
#ifndef LIKE_VPRINTF
#define LIKE_VPRINTF PRINTF_ATTRIBUTE(1, 0)
#endif
#ifndef LIKE_VFPRINTF
#define LIKE_VFPRINTF PRINTF_ATTRIBUTE(2, 0)
#endif

#define NOISY_QUITTER  LIKE_PRINTF NO_RETURN  /* common abbr. */

#ifndef ALIGNMENT
 #if HAVE_ATTRIBUTE_ALIGNED
  #define ALIGNMENT(n)  __attribute__ ((aligned (n)))
 #else
  #define ALIGNMENT(n)
 #endif
#endif

/*
 *  MALIGN = most stringent alignment requirement in bytes:
 */
#ifndef MALIGN
#define MALIGN MAX(8,\
               MAX(ALIGNOF_DOUBLE,\
               MAX(ALIGNOF_VOID_P,\
                   ALIGNOF_LLONG)))
#endif

#ifndef ALIGNED
#define ALIGNED  ALIGNMENT(MALIGN)
#endif

/*
 *  Basename of __FILE__:
 */
#define FILEBASE \
  (strrchr(__FILE__,dirsepchar) ? \
  &strrchr(__FILE__,dirsepchar)[1] : __FILE__)

/*
 *  Some standard C nonsense:
 */
#ifndef NULL
#define NULL 0
#endif

/*
 *  The way we tell C we want a flexible array at the end of a struct
 *  (FLEXIBLE_ARRAY_MEMBER is #defined by configuration):
 */
#define FLEX  FLEXIBLE_ARRAY_MEMBER

/*
 *  VT100 and Xterm emulators, among others, recognize these sequences
 *  of ASCII characters for turning highlighting on and off:
 */
#ifndef BOLD
#define BOLD "\033[1m"  /* ANSI */
#endif
#ifndef NORM
#define NORM "\033[0m"  /* ANSI */
#endif

/*
 *  Predefined file descriptors:
 */
#ifndef fd_stdin
#define fd_stdin 0
#endif
#ifndef fd_stdout
#define fd_stdout 1
#endif
#ifndef fd_stderr
#define fd_stderr 2
#endif

/*
 *  SEEK reference points:
 */
#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif
#ifndef SEEK_END
#define SEEK_END 2
#endif

/*
 *  SHUTDOWN modes:
 */
#ifndef SHUT_RD
#define SHUT_RD 0
#endif
#ifndef SHUT_WR
#define SHUT_WR 1
#endif
#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif

#endif  /* _common_h */