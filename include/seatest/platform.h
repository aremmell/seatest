/*
 * platform.h
 *
 * Author:    Ryan M. Lederman <lederman@gmail.com>
 * Copyright: Copyright (c) 2023
 * Version:   0.1.0
 * License:   The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef _SEATEST_PLATFORM_H_INCLUDED
# define _SEATEST_PLATFORM_H_INCLUDED

# if !defined(_WIN32)
#  if defined(__APPLE__) && defined(__MACH__)
#   define __MACOS__
#   define _DARWIN_C_SOURCE
#  elif defined(_WIN32)
#   define __WIN__
#  elif defined(__FreeBSD__) || defined(__DragonFly__)
#   define __BSD__
#   define _BSD_SOURCE
#   if !defined(_DEFAULT_SOURCE)
#    define _DEFAULT_SOURCE
#   endif
#  elif defined(__NetBSD__)
#   define __BSD__
#   if !defined(_NETBSD_SOURCE)
#    define _NETBSD_SOURCE 1
#   endif
#  elif defined(__OpenBSD__)
#   define __BSD__
#  elif defined(__linux__)
#   define _GNU_SOURCE
#  else
#   if !defined(_POSIX_C_SOURCE)
#    define _POSIX_C_SOURCE 200809L
#   endif
#   if !defined(_DEFAULT_SOURCE)
#    define _DEFAULT_SOURCE
#   endif
#   if !defined(_XOPEN_SOURCE)
#    define _XOPEN_SOURCE 700
#   endif
#  endif

#  define __STDC_WANT_LIB_EXT1__ 1

#  include <sys/statvfs.h>
#  include <sys/types.h>
#  include <sys/time.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <netdb.h>
#  include <termios.h>
#  include <unistd.h>
#  include <errno.h>

#  if !defined(__STDC_NO_ATOMICS__)
#   include <stdatomic.h>
#   define __HAVE_STDATOMICS__
#  endif

#  define ST_E_INVALID EINVAL
#  define ST_BAD_DESCRIPTOR -1

typedef int st_descriptor;
typedef socklen_t st_optlen;

# else /* _WIN32 */
#  define __WIN__
#  define WIN32_LEAN_AND_MEAN
#  define _CRT_SECURE_NO_WARNINGS
#  define __WANT_STDC_SECURE_LIB__ 1
#  include <Windows.h>
#  include <Shlwapi.h>
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <direct.h>
#  include <conio.h>

#  if defined(_MSC_VER) && _MSC_VER >= 1933
#   include <stdatomic.h>
#   define __HAVE_STDATOMICS__
#  endif

#  define ST_MSEC_TIMER
#  define ST_MSEC_WIN32

#  define ST_E_INVALID ERROR_INVALID_PARAMETER
#  define ST_BAD_DESCRIPTOR INVALID_SOCKET

typedef SOCKET st_descriptor;
typedef int st_optlen;

# endif /* !_WIN32 */

# include <math.h>
# include <time.h>
# include <ctype.h>
# include <stdio.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <inttypes.h>
# include <assert.h>
# include <string.h>

# if defined(__WIN__) && defined(__STDC_SECURE_LIB__)
#  define __HAVE_STDC_SECURE_OR_EXT1__
# elif defined(__STDC_LIB_EXT1__)
#  define __HAVE_STDC_SECURE_OR_EXT1__
# elif defined(__STDC_ALLOC_LIB__)
#  define __HAVE_STDC_EXT2__
# endif

# if defined(__MACOS__) || defined(__BSD__) || \
     (defined(_GNU_SOURCE) && defined(__GLIBC__) && \
     __GLIBC__ >= 2 && __GLIBC_MINOR__ >= 38)
#  define __HAVE_STRLCPY__
#  define __HAVE_STRLCAT__
# elif defined(__WIN__) && defined(__HAVE_STDC_SECURE_OR_EXT1__)
#  define __HAVE_STRNCPY_S__
#  define __HAVE_STRNCAT_S__
# endif

# if defined(__MACOS__) || defined(__BSD__) || \
    (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L && !defined(_GNU_SOURCE)))
#  define __HAVE_XSI_STRERROR_R__
#  if defined(__GLIBC__)
#   if (__GLIBC__ >= 2 && __GLIBC_MINOR__ < 13)
#    define __HAVE_XSI_STRERROR_R_ERRNO__
#   endif
#  endif
# elif defined(_GNU_SOURCE) && defined(__GLIBC__)
#  define __HAVE_GNU_STRERROR_R__
# elif defined(__HAVE_STDC_SECURE_OR_EXT1__)
#  define __HAVE_STRERROR_S__
# endif

# if (defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0) || \
       defined(__MACOS__) || defined(__OpenBSD__)
#  define ST_MSEC_TIMER
#  define ST_MSEC_POSIX
# else
#  undef ST_MSEC_TIMER
# endif

# if defined(CLOCK_UPTIME)
#  define ST_INTERVALCLOCK CLOCK_UPTIME
# elif defined(CLOCK_BOOTTIME)
#  define ST_INTERVALCLOCK CLOCK_BOOTTIME
# elif defined(CLOCK_HIGHRES)
#  define ST_INTERVALCLOCK CLOCK_HIGHRES
# elif defined(CLOCK_MONOTONIC)
#  define ST_INTERVALCLOCK CLOCK_MONOTONIC
# else
#  define ST_INTERVALCLOCK CLOCK_REALTIME
# endif

# if (defined(__clang__) || defined(__GNUC__)) && defined(__FILE_NAME__)
#  define __file__ __FILE_NAME__
# elif defined(__BASE_FILE__)
#  define __file__ __BASE_FILE__
# else
#  if defined(__WIN__)
#   define ST_PATH_SEP_CHAR '\\'
#   define ST_PATH_SEP_STR "\\"
#  else
#   define ST_PATH_SEP_CHAR '/'
#   define ST_PATH_SEP_STR "/"
#  endif
#  define __file__ (strrchr(ST_PATH_SEP_STR __FILE__, ST_PATH_SEP_CHAR) + 1)
# endif

/*
 * SPDX-License-Identifier: ISC and MIT
 *
 * strlcat from: $OpenBSD: strlcat.c,v 1.19 2019/01/25 00:19:25 millert Exp $
 * strlcpy from: $OpenBSD: strlcpy.c,v 1.16 2019/01/25 00:19:25 millert Exp $
 *
 * Copyright (c) 1998, 2010, 2015 Todd C. Miller <millert@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#if !defined(__HAVE_STRLCPY__) && !defined(__HAVE_STRNCPY_S__)
/*
 * Copy string src to buffer dst of size dsize.  At most dsize-1
 * chars will be copied.  Always NUL terminates (unless dsize == 0).
 * Returns strlen(src); if retval >= dsize, truncation occurred.
 */
static inline size_t
__strlcpy(char *dst, const char *src, size_t dsize)
{
  const char * osrc   = src;
  size_t       nleft  = dsize;

  /* Copy as many bytes as will fit. */
  if (nleft != 0) {
    while (--nleft != 0) {
      if ((*dst++ = *src++) == '\0')
        break;
    }
  }

  /* Not enough room in dst, add NUL and traverse rest of src. */
  if (nleft == 0) {
    if (dsize != 0)
      *dst = '\0'; /* NUL-terminate dst */
    while (*src++);
  }

  return src - osrc - 1; /* count does not include NUL */
}
#endif

#if !defined(__HAVE_STRLCAT__) && !defined(__HAVE_STRNCAT_S__)
/*
 * Appends src to string dst of size dsize (unlike strncat, dsize is the
 * full size of dst, not space left).  At most dsize-1 characters
 * will be copied.  Always NUL terminates (unless dsize <= strlen(dst)).
 * Returns strlen(src) + MIN(dsize, strlen(initial dst)).
 * If retval >= dsize, truncation occurred.
 */
static inline size_t
__strlcat(char *dst, const char *src, size_t dsize)
{
  const char * odst  = dst;
  const char * osrc  = src;
  size_t       n     = dsize;
  size_t       dlen;

  /* Find the end of dst and adjust bytes left but don't go past end. */
  while (n-- != 0 && *dst != '\0')
    dst++;

  dlen  =   dst - odst;
  n     = dsize - dlen;

  if (n-- == 0)
    return dlen + strlen(src);

  while (*src != '\0') {
    if (n != 0) {
      *dst++ = *src;
      n--;
    }
    src++;
  }
  *dst = '\0';

  return dlen + (src - osrc); /* count does not include NUL */
}
#endif

#endif /* !_SEATEST_PLATFORM_H_INCLUDED */
