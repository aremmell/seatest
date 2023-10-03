/*
 * helpers.h
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
#ifndef _SEATEST_HELPERS_H_INCLUDED
# define _SEATEST_HELPERS_H_INCLUDED

# include "seatest/types.h"

/** Frees and sets pointer to NULL. */
static inline
void __st_safefree(void** pp)
{
    if (pp && *pp) {
        free(*pp);
        *pp = NULL;
    }
}

/** Wraps __st_safefree in order to accept any pointer type. */
# define _st_safefree(pp) __st_safefree((void**)pp)

/** Returns the last socket-related error code. */
static inline
int _st_last_sockerr(void)
{
#if !defined(__WIN__)
    return errno;
#else
    return WSAGetLastError();
#endif
}

/**
 * Internal helper macros
 */

# define _ST_STRIFY(val) #val

/** Blots out a variable, avoiding compiler warnings/errors in the event that it is unreferenced. */
# define _ST_UNUSED(var) (void)(var)

/** Returns the number of entries in an array. */
# define _ST_COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))

/** Returns the plural or singular form of a word based on the count supplied.
 * Only works for words whose plural form is just an 's'. */
# define _ST_PLURAL(word, count) ((!(count) || (count) > 1) ? word "s" : word)

# define _ST_DECLARE_CL_ARGS() \
    static const st_cl_arg st_cl_args[] = { \
        ST_CL_CONFIG() \
    }

# define _ST_EVALUATE_EXPR_RAW(expr, name, color, is_fatal) \
    do { \
        if (!(expr)) { \
            _retval.pass = false; \
            _retval.fatal = is_fatal; \
            (void)printf(FG_COLOR(0, color, name " (line %"PRIu32"):") \
                DGRAY(" expression") WHITE(" '" #expr "'") DGRAY(" is false\n"), __LINE__); \
        } \
    } while (false)

# define _ST_EVALUATE_EXPR(expr, name) \
    _ST_EVALUATE_EXPR_RAW(expr, name, 196, true)

# define _ST_PROCESS_TEST_CONDITION(condition, check_cond) \
    do { \
        if (!condition && (tests[n].conds & check_cond) == check_cond) { \
            _ST_WARNING("%s test #%zu (name: '%s') will be skipped due to " #check_cond, \
                _ST_WARN_PREFIX, n + 1, tests[n].name); \
            tests[n].res.skip_conds |= check_cond; \
            skip = true; \
        } \
    } while (false)

/** Converts a bitmask of conditions into a string. */
static inline
char* _st_conds_to_string(int conds, char str[ST_MAX_COND_STR])
{
    str[0] = '\0';

    bool first = true;
    if ((conds & COND_DISK) == COND_DISK) {
        (void)strncat(str, _ST_STRIFY(COND_DISK), ST_MAX_COND_STR);
        first = false;
    }
    if ((conds & COND_INET) == COND_INET) {
        if (!first) {
            (void)strncat(str, ", ", ST_MAX_COND_STR);
        }
        (void)strncat(str, _ST_STRIFY(COND_INET), ST_MAX_COND_STR);
        first = false;
    }
    if ((conds & COND_CPUS) == COND_CPUS) {
        if (!first) {
            (void)strncat(str, ", ", ST_MAX_COND_STR);
        }
        (void)strncat(str, _ST_STRIFY(COND_CPUS), ST_MAX_COND_STR);
        first = false;
    }

    return &str[0];
}

#endif /* !_SEATEST_HELPERS_H_INCLUDED */
