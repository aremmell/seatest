/*
 * types.h
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
#ifndef _SEATEST_TYPES_H_INCLUDED
# define _SEATEST_TYPES_H_INCLUDED

# include "seatest/platform.h"

/** Global state container. */
typedef struct {
    const char* app_name;
} st_state;

/** Data associated with a test. */
typedef struct {
    uint32_t line_start; /**< Line number at which the test starts. */
    bool skip;           /**< true if the test is skipped. */
    int skip_conds;      /**< If skipped, the condition(s) that caused skippage. */
    bool pass;           /**< true if the test encountered error(s) or warning(s). */
    bool fatal;          /**< true if the test encountered error(s). */
} st_testres;

/** Function typedef for test routines. */
typedef st_testres (*st_test_fn)(void);

/**
 * Enumeration of conditions which must be met in order for a given test to run
 * successfully; if the specified conditions are not met, the test is skipped.
 * See config.h for thresholds.
 */
enum {
    COND_INET = 1 << 1, /**< Internet connection available. */
    COND_DISK = 1 << 2, /**< At least ST_MIN_FS_AVAIL bytes of free space available. */
    COND_CPUS = 1 << 3, /**< At least n logical CPUs available. */
}; // TODO: Think of more conditions

/** An entry in the list of available tests. */
typedef struct {
    const char* const name;
    st_test_fn fn;
    st_testres res;
    double msec;
    int conds;
    bool run;
} st_test;

/** A command line argument. */
typedef struct {
    const char* const flag;  /**< e.g. --whiz-bang. */
    const char* const usage; /**< e.g. [file, ...]. */
    const char* const desc;  /**< e.g. 'causes you to levitate'. */
} st_cl_arg;

/** Command line configuration. */
typedef struct {
    bool wait;
    bool only;
    size_t to_run;
} st_cl_config;

/** Millisecond timer. */
typedef struct {
# if !defined(__WIN__)
    time_t sec;
    long msec;
# else /* __WIN__ */
    LARGE_INTEGER counter;
# endif
} st_timer;

# if !defined(__WIN__)
#  define st_strncmp strncmp      /** Compares two strings for equality. */
#  define st_strnicmp strncasecmp /** Compares two strings for equality, ignoring case. */
#  define st_strstr strstr        /** Searches a string for a sub-string. */
#  define st_stristr strcasestr   /** Searches a string for a sub-string, ignoring case. */
# else
#  define st_strncmp StrCmpNA     /** Compares two strings for equality. */
#  define st_strnicmp StrCmpNIA   /** Compares two strings for equality, ignoring case. */
#  define st_strstr StrStrA       /** Searches a string for a sub-string. */
#  define st_stristr StrStrIA     /** Searches a string for a sub-string, ignoring case. */
# endif

#endif /* !_SEATEST_TYPES_H_INCLUDED */
