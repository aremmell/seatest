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

#  include <sys/types.h>
#  include <sys/time.h>
#  include <termios.h>
#  include <unistd.h>
#  include <errno.h>
#  include <stdlib.h>
#  include <string.h>

#  if !defined(__STDC_NO_ATOMICS__)
#   include <stdatomic.h>
#   define __HAVE_STDATOMICS__
#  endif

# else /* _WIN32 */
#  define __WIN__
#  define WIN32_LEAN_AND_MEAN
#  define _CRT_SECURE_NO_WARNINGS
#  define __WANT_STDC_SECURE_LIB__ 1
#  include <Windows.h>

#  if defined(_MSC_VER) && _MSC_VER >= 1933
#   include <stdatomic.h>
#   define __HAVE_STDATOMICS__
#  endif

#  define ST_MSEC_TIMER
#  define ST_MSEC_WIN32
#  define ST_WALLCLOCK 0
#  define ST_INTERVALCLOCK 1

# endif /* !_WIN32 */

# include <stdio.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdint.h>
# include <inttypes.h>
# include <assert.h>

# if defined(__WIN__) && defined(__STDC_SECURE_LIB__)
#  define __HAVE_STDC_SECURE_OR_EXT1__
# elif defined(__STDC_LIB_EXT1__)
#  define __HAVE_STDC_SECURE_OR_EXT1__
# elif defined(__STDC_ALLOC_LIB__)
#  define __HAVE_STDC_EXT2__
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

#  if (defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0) || \
       defined(__MACOS__) || defined(__OpenBSD__)
#   define ST_MSEC_TIMER
#   define ST_MSEC_POSIX
#  else
#   undef ST_MSEC_TIMER
#  endif

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
#  define __file__ __FILE__
# endif

#endif /* !_SEATEST_PLATFORM_H_INCLUDED */
