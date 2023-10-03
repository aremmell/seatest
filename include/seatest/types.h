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

/** Type returned by each test function. */
typedef struct {
    uint32_t line_start;
    bool skipped;
    bool pass;
    bool fatal;
} st_testres;

/** Function type for seatest tests. */
typedef st_testres (*st_test_fn)(void);

/**
 * Enumeration of conditions which must be met in order for a given test to run
 * successfully; if the specified conditions are not met, the test is skipped.
 * See config.h for thresholds.
 */
enum {
    COND_INET = 1 << 1, /**< An Internet connection is available. */
    COND_DISK = 1 << 2, /**< At least n GiB of space is available. */
    COND_CPUS = 1 << 3, /**< At least n logical CPUs are available. */
}; // TODO: Think of more conditions

/** Type representing the current state of potentially required conditions. */
typedef struct {
    bool have_inet;      /**< Whether or not an Internet connection is available. */
    uint64_t disk_avail; /**< Available disk space, in bytes. */
    int ncpu;            /**< Number of logical CPUs. */
} st_cond_state;

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

/** Function comparing two strings for equality. */
# if !defined(__WIN__)
#  define st_strncmp strncmp
# else
#  define st_strncmp StrCmpNA
# endif

/** Function comparing two strings for equality, ignoring case. */
# if !defined(__WIN__)
#  define st_strnicmp strncasecmp
# else
#  define st_strnicmp StrCmpNIA
# endif

/** Function searching a string for a sub-string. */
# if !defined(__WIN__)
#  define st_strstr strstr
# else
#  define st_strstr StrStrA
# endif

/** Function searching a string for a sub-string, ignoring case. */
# if !defined(__WIN__)
#  define st_stristr strcasestr
# else
#  define st_stristr StrStrIA
# endif

#endif /* !_SEATEST_TYPES_H_INCLUDED */
