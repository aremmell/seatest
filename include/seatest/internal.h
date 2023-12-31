/*
 * internal.h
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
#ifndef _SEATEST_INTERNAL_H_INCLUDED
# define _SEATEST_INTERNAL_H_INCLUDED

# include "seatest/platform.h"
# include "seatest/config.h"

/**
 * Types
 */

/** Global state container. */
typedef struct {
    const char* app_name;
    bool fail_early;
} st_state;

/** Data associated with a test. */
typedef struct {
    bool skip;      /**< true if the test is skipped. */
    int skip_conds; /**< If skipped, the condition(s) that caused skippage. */
    int errors;     /**< The number of evaulators that resulted in a false expression. */
    int warnings;   /**< The number of times ST_WARNING() or ST_EXPECT() were called. */
    bool last_fail; /**< true if the last evaluator executed was false. */
    bool pass;      /**< false if the test encountered error(s) or warning(s). */
    bool fatal;     /**< true if the test encountered error(s). */
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
    const char* const s_flag; /**< e.g. -w. */
    const char* const flag;   /**< e.g. --whiz-bang. */
    const char* const usage;  /**< e.g. [file, ...]. */
    const char* const desc;   /**< e.g. 'causes you to levitate'. */
} st_cl_arg;

/** Command line configuration. */
typedef struct {
    bool wait;     /**< true if --wait was passed, false otherwise. */
    bool only;     /**< true if --only was passed, false otherwise. */
    size_t to_run; /**< If --only was passed, how many tests to run. */
} st_cl_config;

/** Millisecond timer. */
typedef struct {
# if !defined(__WIN__)
    time_t sec;
    double msec;
# else /* __WIN__ */
    LARGE_INTEGER counter;
# endif
} st_timer;

# if !defined(__WIN__)
#  define st_strncmp  strncmp     /** Compares two strings for equality. */
#  define st_strnicmp strncasecmp /** Compares two strings for equality, ignoring case. */
#  define st_strstr   strstr      /** Searches a string for a sub-string. */
#  define st_stristr  strcasestr  /** Searches a string for a sub-string, ignoring case. */
# else
#  define st_strncmp  StrCmpNA    /** Compares two strings for equality. */
#  define st_strnicmp StrCmpNIA   /** Compares two strings for equality, ignoring case. */
#  define st_strstr   StrStrA     /** Searches a string for a sub-string. */
#  define st_stristr  StrStrIA    /** Searches a string for a sub-string, ignoring case. */
# endif

/**
 * Macros
 */

# define _ESC "\x1b[" /**< Begins an ANSI escape sequence. */
# define _ESC_M "m"   /**< Marks the end of a sequence. */

# define _ESC_SEQ(codes, s) _ESC codes _ESC_M s
# define _ESC_SEQE(codes)   _ESC_SEQ(codes, "")

/** Resets all previously applied colors and effects/attributes. */
# define _ESC_RST _ESC_SEQE("0")

/** Allows for setting of the attributes, foreground, and background colors
 * simultaneously. */
# define COLOR(attr, fg, bg, s) \
    _ESC_SEQ(#attr ";38;5;" #fg ";48;5;" #bg, s) _ESC_RST

/** Sets only the foreground color, ignoring background. Windows Terminal
 * does not behave if you issue a `49;5` (text starts flashing). */
# define FG_COLOR(attr, fg, s) \
    _ESC_SEQ(#attr ";38;5;" #fg, s) _ESC_RST

/** Sets only the background color, setting foreground to its default. */
# define BG_COLOR(attr, bg, s) \
    _ESC_SEQ(#attr ";39;5;48;5;" #bg, s) _ESC_RST

/** Bulleted list item symbol. */
# if !defined(__WIN__)
#  define ST_BULLET "\xe2\x80\xa2"
# else
#  define ST_BULLET "-"
# endif

# define ULINE(s) _ESC_SEQ("4", s) _ESC_SEQE("24") /**< Underlined. */
# define EMPH(s)  _ESC_SEQ("3", s) _ESC_SEQE("23") /**< Emphasis/italic. */
# define BOLD(s)  _ESC_SEQ("1", s) _ESC_SEQE("22") /**< Bold. */

# define BLACK(s)     FG_COLOR(0, 0, s) /**< Black foreground text. */
# define BLACKB(s)    FG_COLOR(1, 0, s) /**< Bold black foreground text. */

# define RED(s)       FG_COLOR(0, 1, s) /**< Red foreground text. */
# define REDB(s)      FG_COLOR(1, 1, s) /**< Bold red foreground text. */

# define BRED(s)      FG_COLOR(0, 9, s) /**< Bright red foreground text. */
# define BREDB(s)     FG_COLOR(1, 9, s) /**< Bold bright red foreground text. */

# define GREEN(s)     FG_COLOR(0, 2, s) /**< Green foreground text. */
# define GREENB(s)    FG_COLOR(1, 2, s) /**< Bold green foreground text. */

# define BGREEN(s)    FG_COLOR(0, 10, s) /**< Bright green foreground text. */
# define BGREENB(s)   FG_COLOR(1, 10, s) /**< Bold bright green foreground text. */

# define YELLOW(s)    FG_COLOR(0, 3, s) /**< Yellow foreground text. */
# define YELLOWB(s)   FG_COLOR(1, 3, s) /**< Bold yellow foreground text. */

# define BYELLOW(s)   FG_COLOR(0, 11, s) /**< Bright yellow foreground text. */
# define BYELLOWB(s)  FG_COLOR(1, 11, s) /**< Bold bright yellow foreground text. */

# define BLUE(s)      FG_COLOR(0, 4, s) /**< Blue foreground text. */
# define BLUEB(s)     FG_COLOR(1, 4, s) /**< Bold blue foreground text. */

# define BBLUE(s)     FG_COLOR(0, 12, s) /**< Bright blue foreground text. */
# define BBLUEB(s)    FG_COLOR(1, 12, s) /**< Bold bright blue foreground text. */

# define MAGENTA(s)   FG_COLOR(0, 5, s) /**< Magenta foreground text. */
# define MAGENTAB(s)  FG_COLOR(1, 5, s) /**< Bold magenta foreground text. */

# define BMAGENTA(s)  FG_COLOR(0, 13, s) /**< Bright magenta foreground text. */
# define BMAGENTAB(s) FG_COLOR(1, 13, s) /**< Bold bright magenta foreground text. */

# define CYAN(s)      FG_COLOR(0, 6, s) /**< Cyan foreground text. */
# define CYANB(s)     FG_COLOR(1, 6, s) /**< Bold cyan foreground text. */

# define BCYAN(s)     FG_COLOR(0, 13, s) /**< Bright cyan foreground text. */
# define BCYANB(s)    FG_COLOR(1, 13, s) /**< Bold bright cyan foreground text. */

# define BGRAY(s)     FG_COLOR(0, 7, s) /**< Bright gray foreground text. */
# define BGRAYB(s)    FG_COLOR(1, 7, s) /**< Bold bright gray foreground text. */

# define DGRAY(s)     FG_COLOR(0, 8, s) /**< Dark gray foreground text. */
# define DGRAYB(s)    FG_COLOR(1, 8, s) /**< Bold dark gray foreground text. */

# define WHITE(s)     FG_COLOR(0, 15, s) /**< White foreground text. */
# define WHITEB(s)    FG_COLOR(1, 15, s) /**< Bold white foreground text. */

/** Returns the plural or singular form of a word based on the count supplied.
 * Only works for words whose plural form is just an 's'. */
# define _ST_PLURAL(word, count) ((!(count) || (count) > 1) ? word "s" : word)

/** The base macro for all stdout macros. */
# define __ST_MESSAGE(...) (void)printf(__VA_ARGS__)

# define _ST_MESSAGE(msg, ...) __ST_MESSAGE(WHITE(msg) "\n", __VA_ARGS__)
# define _ST_SUCCESS(msg, ...) __ST_MESSAGE(FG_COLOR(0, 40, msg) "\n", __VA_ARGS__)
# define _ST_SKIPPED(msg, ...) __ST_MESSAGE(FG_COLOR(1, 178, EMPH(msg)) "\n", __VA_ARGS__)
# define _ST_WARNING(msg, ...) __ST_MESSAGE(FG_COLOR(0, 208, msg) "\n", __VA_ARGS__)
# define _ST_ERROR(msg, ...)   __ST_MESSAGE(FG_COLOR(0, 196, msg) "\n", __VA_ARGS__)

# if defined(ST_DEBUG_MESSAGES)
#  define _ST_DEBUG(msg, ...) \
    __ST_MESSAGE("%s %s (%s:%d): " msg "\n", _ST_DEBUG_PREFIX, __func__, __file__, \
        __LINE__, __VA_ARGS__)
# else
#  define _ST_DEBUG(...)
# endif

# define _ST_ERROR_PREFIX ST_LOC_SEATEST " " ST_LOC_ERROR ":"
# define _ST_WARN_PREFIX  ST_LOC_SEATEST " " ST_LOC_WARNING ":"
# define _ST_DEBUG_PREFIX ST_LOC_SEATEST " " ST_LOC_DEBUG ":"

# define _ST_SKIP_PASS_FAIL(test) \
     (test->res.skip ? FG_COLOR(1, 178, ST_LOC_SKIP) : test->res.pass \
                     ? FG_COLOR(1,  40, ST_LOC_PASS) : test->res.fatal \
                     ? FG_COLOR(1, 196, ST_LOC_FAIL) : FG_COLOR(1, 208, ST_LOC_WARN))

# define __ST_REPORT_ERROR(code, message) \
    _ST_ERROR("%s "ST_LOC_IN" %s (%s:%d): %d (%s)", _ST_ERROR_PREFIX, __func__, \
        __file__, __LINE__, code, message)

# define _ST_REPORT_ERROR(code) \
    do { \
        char message[ST_MAX_ERROR] = {0}; \
        __ST_REPORT_ERROR(code, st_format_error_msg(code, message)); \
    } while (false)

# define _ST_STRIFY(val) #val

/** Blots out a variable, avoiding compiler warnings/errors in the event that it is unreferenced. */
# define _ST_UNUSED(var) (void)(var)

/** Returns the number of entries in an array. */
# define _ST_COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))

/** Self-explanatory. */
# define _ST_NOTNULL(p) (p) != 0

# define _ST_DECLARE_CL_ARGS() \
    static const st_cl_arg st_cl_args[] = { \
        ST_CL_CONFIG() \
    }

# define _ST_VALIDATE_RETURN() \
    do { \
        if (__retval.warnings > 0 || !__retval.pass) { \
            assert(__retval.warnings > 0); \
            assert(!__retval.pass); \
        } \
        if (__retval.errors > 0 || __retval.fatal) { \
            assert(__retval.errors > 0); \
            assert(__retval.fatal); \
        } \
        return __retval; \
    } while (false)

# define _ST_EVALUATE_EXPR_RAW(expr, name, color, is_fatal) \
    do { \
        if (!(expr)) { \
            __retval.pass = false; \
            __retval.last_fail = true; \
            if ((is_fatal)) { \
                __retval.fatal = true; \
                __retval.errors++; \
            } else { \
                __retval.warnings++; \
            } \
            (void)printf(ST_LOC_INDENT FG_COLOR(0, color, name " ("ST_LOC_LINE \
                " %"PRIu32"):") DGRAY(" "ST_LOC_EXPRESSION) WHITE(" '" #expr "'") \
                DGRAY(" "ST_LOC_IS_FALSE"\n"), __LINE__); \
        } else { \
            __retval.last_fail = false; \
        } \
    } while (false)

# define _ST_EVALUATE_EXPR(expr, name) \
    _ST_EVALUATE_EXPR_RAW(expr, name, 196, true)

# define _ST_PROCESS_TEST_CONDITION(condition, check_cond) \
    do { \
        if (!condition && (tests[n].conds & check_cond) == check_cond) { \
            _ST_WARNING("%s "ST_LOC_TEST" #%zu ("ST_LOC_NAME": '%s') " \
            ST_LOC_SKIPPED_COND" " #check_cond, _ST_WARN_PREFIX, n + 1, tests[n].name); \
            tests[n].res.skip_conds |= check_cond; \
            skip = true; \
        } \
    } while (false)

/**
 * Utility functions
 */

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

/** Wrapper for 'safe' versions of strcpy, depending on what's available. */
static inline
void _st_strcpy(char* const restrict dst, size_t dstsz,
    const char* const restrict src, size_t count)
{
# if defined(__HAVE_STRLCPY__)
    size_t ret = strlcpy(dst, src, dstsz);
    assert(ret < dstsz);
    _ST_UNUSED(count);
    _ST_UNUSED(ret);
# elif defined(__HAVE_STRNCPY_S__)
    errno_t ret = strncpy_s(dst, dstsz, src, _TRUNCATE);
    assert(STRUNCATE != ret);
    _ST_UNUSED(count);
    _ST_UNUSED(ret);
# else
    size_t ret = __strlcpy(dst, src, dstsz);
    assert(ret < dstsz);
    _ST_UNUSED(count);
    _ST_UNUSED(ret);
# endif
}

/** Wrapper for 'safe' versions of strcat, depending on what's available. */
static inline
void _st_strcat(char* const restrict dst, size_t dstsz,
    const char* const restrict src, size_t count)
{
# if defined(__HAVE_STRLCAT__)
    size_t ret = strlcat(dst, src, dstsz);
    assert(ret < dstsz);
    _ST_UNUSED(count);
    _ST_UNUSED(ret);
# elif defined(__HAVE_STRNCAT_S__)
    errno_t ret = strncat_s(dst, dstsz, src, _TRUNCATE);
    assert(0 == ret);
    _ST_UNUSED(count);
    _ST_UNUSED(ret);
# else
    size_t ret = __strlcat(dst, src, dstsz);
    assert(ret < dstsz);
    _ST_UNUSED(count);
    _ST_UNUSED(ret);
# endif
}

/** Converts a bitmask of conditions into a string. */
static inline
char* _st_conds_to_string(int conds, char str[ST_MAX_COND_STR])
{
    str[0] = '\0';

    bool first = true;
    if ((conds & COND_DISK) == COND_DISK) {
        const char* cat = _ST_STRIFY(COND_DISK);
        _st_strcat(str, ST_MAX_COND_STR, cat, strlen(cat));
        first = false;
    }
    if ((conds & COND_INET) == COND_INET) {
        if (!first) {
            _st_strcat(str, ST_MAX_COND_STR, ", ", 2);
        }
        const char* cat = _ST_STRIFY(COND_INET);
        _st_strcat(str, ST_MAX_COND_STR, cat, strlen(cat));
        first = false;
    }
    if ((conds & COND_CPUS) == COND_CPUS) {
        if (!first) {
            _st_strcat(str, ST_MAX_COND_STR, ", ", 2);
        }
        const char* cat = _ST_STRIFY(COND_CPUS);
        _st_strcat(str, ST_MAX_COND_STR, cat, strlen(cat));
    }

    return &str[0];
}

/** Counts the number of high bits in a condition bitmask. */
static inline
int _st_conds_count(int conds) {
    int retval = 0;
    int mask = 1;
    for (size_t b = 0; b < (sizeof(int) * 8); b++) {
        if ((conds & mask) == mask) {
            retval++;
        }
        mask <<= 1;
    }
    return retval;
}

#endif /* !_SEATEST_INTERNAL_H_INCLUDED */
