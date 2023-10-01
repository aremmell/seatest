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

/** Type returned by each test function. */
typedef struct {
    uint32_t line_start;
    bool pass;
    bool fatal;
} st_testres;

/** Function type for seatest tests. */
typedef st_testres (*st_test_fn)(void);

/** Enumeration of conditions which must be met in order for a given test to run
 * successfully; if the conditions are not met, the test is skipped. */
enum {
    INET = 1 << 1, /**< An Internet connection is available. */
    DISK = 1 << 2, /**< At least n GiB of space is available on the drive. */
    CPUS = 1 << 3, /**< At least n logical CPUs must exist on the system. */
} st_conds; // TODO: Think of more conditions

/** An entry in the list of available tests. */
typedef struct {
    const char* const name;
    st_test_fn fn;
    st_testres res;
    double msec;
    int conds;
    bool run;
} st_testinfo;

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

/** Function comparing two strings for equality, ignoring case. */
# if !defined(__WIN__)
#  define st_strnicmp strncasecmp
# else
#  define st_strnicmp StrStrIA
# endif

#endif /* !_SEATEST_TYPES_H_INCLUDED */
