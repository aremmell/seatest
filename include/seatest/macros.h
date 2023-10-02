/*
 * macros.h
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
#ifndef _SEATEST_MACROS_H_INCLUDED
# define _SEATEST_MACROS_H_INCLUDED

# include "seatest/types.h"

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

/** Blots out a variable, avoiding compiler warnings/errors in the event that it
 * is unreferenced. */
# define ST_UNUSED(var) (void)(var)

/** Returns the number of entries in an array. */
# define ST_COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))

/** Returns the plural or singular form of a word based on the count supplied.
 * Only works for words whose plural form is just an 's'. */
# define ST_PLURAL(word, count) ((!(count) || (count) > 1) ? word "s" : word)

/** The only code needed in main()/the entry point. `name` is a name to associate
 * with your test suite, like the product or company name. */
# define ST_MAIN_IMPL(name) \
    st_main(argc, argv, name, st_cl_args, ST_COUNTOF(st_cl_args), \
        st_tests, ST_COUNTOF(st_tests))

/** Declares an individual test function. `name` must only contain characters
 * allowed in C function names. */
# define ST_DECLARE_TEST(name) \
    st_testres st_test_##name(void)

/** Begins the definition (implementation) of an individual test function. */
# define ST_BEGIN_TEST_IMPL(name) \
    st_testres st_test_##name(void) \
    { \
        st_testres _retval; \
        _retval.line_start = __LINE__; \
        _retval.skipped = false; \
        _retval.pass = true; \
        _retval.fatal = false;

/** Ends the definition (implementation) of an individual test function. */
# define ST_END_TEST_IMPL(...) \
        return _retval; \
    }

/** Begins the declaration of the global list of available tests. */
# define ST_BEGIN_DECLARE_TEST_LIST(...) \
    static st_test st_tests[] = {

/** Adds an entry to the global list of tests. */
# define ST_DECLARE_TEST_LIST_ENTRY_COND(name, fn_name, conditions) \
    {#name, st_test_##fn_name, {0}, 0.0, conditions, false},

/** Adds an entry to the global list of tests. */
# define ST_DECLARE_TEST_LIST_ENTRY(name, fn_name) \
    ST_DECLARE_TEST_LIST_ENTRY_COND(name, fn_name, 0)

/** Ends the declaration of the global list of available tests. */
# define ST_END_DECLARE_TEST_LIST(...) \
    };

# define ST_CL_MAX_FLAG  32
# define ST_CL_MAX_USAGE 96
# define ST_CL_MAX_TEST  32

# define ST_CL_WAIT_FLAG "--wait"
# define ST_CL_ONLY_FLAG "--only"
# define ST_CL_LIST_FLAG "--list"
# define ST_CL_VERS_FLAG "--version"
# define ST_CL_HELP_FLAG "--help"

# define ST_CL_ONLY_USAGE ULINE("name") " [, " ULINE("name") ", ...]"

# define ST_CL_WAIT_DESC "After running all test(s), wait for a keypress before exiting"
# define ST_CL_ONLY_DESC "Run only the test(s) specified"
# define ST_CL_LIST_DESC "Print a list of all available tests"
# define ST_CL_VERS_DESC "Displays version information"
# define ST_CL_HELP_DESC "Displays this message"

# define ST_DECLARE_CL_ARGS(...) \
    static const st_cl_arg st_cl_args[] = { \
        {ST_CL_WAIT_FLAG, "", ST_CL_WAIT_DESC}, \
        {ST_CL_ONLY_FLAG, ST_CL_ONLY_USAGE, ST_CL_ONLY_DESC}, \
        {ST_CL_LIST_FLAG, "", ST_CL_LIST_DESC}, \
        {ST_CL_VERS_FLAG, "", ST_CL_VERS_DESC}, \
        {ST_CL_HELP_FLAG, "", ST_CL_HELP_DESC} \
    }

# define _ST_INDENT "  "
# define __ST_MESSAGE(...) (void)printf(__VA_ARGS__)

/** Outputs a diagnostic or informative test message. */
# define ST_MESSAGE(msg, ...) __ST_MESSAGE(_ST_INDENT WHITE(msg) "\n", __VA_ARGS__)
# define ST_MESSAGE0(msg)     __ST_MESSAGE(_ST_INDENT WHITE(msg) "\n")

/** Outputs a test message in green. */
# define ST_SUCCESS(msg, ...) __ST_MESSAGE(_ST_INDENT FG_COLOR(0, 40, msg) "\n", __VA_ARGS__)
# define ST_SUCCESS0(msg)     __ST_MESSAGE(_ST_INDENT FG_COLOR(0, 40, msg) "\n")

/** Outputs a test message in orange. */
# define ST_WARNING(msg, ...) __ST_MESSAGE(_ST_INDENT FG_COLOR(0, 208, msg) "\n", __VA_ARGS__)
# define ST_WARNING0(msg)     __ST_MESSAGE(_ST_INDENT FG_COLOR(0, 208, msg) "\n")

/** Outputs a test message in red. */
# define ST_ERROR(msg, ...) __ST_MESSAGE(_ST_INDENT FG_COLOR(0, 196, msg) "\n", __VA_ARGS__)
# define ST_ERROR0(msg)      __ST_MESSAGE(_ST_INDENT FG_COLOR(0, 196, msg) "\n")

# define _ST_MESSAGE(msg, ...) __ST_MESSAGE(WHITE(msg) "\n", __VA_ARGS__)
# define _ST_SUCCESS(msg, ...) __ST_MESSAGE(FG_COLOR(0, 40, msg) "\n", __VA_ARGS__)
# define _ST_SKIPPED(msg, ...) __ST_MESSSAGE(FG_COLOR(0, 178, msg) "\n", __VA_ARGS__)
# define _ST_WARNING(msg, ...) __ST_MESSAGE(FG_COLOR(0, 208, msg) "\n", __VA_ARGS__)
# define _ST_ERROR(msg, ...) __ST_MESSAGE(FG_COLOR(0, 196, msg) "\n", __VA_ARGS__)

# define ST_PASSFAILWARN(test) \
    (test->res.skipped ? FG_COLOR(1, 178, "SKIP") \
    : test->res.pass ? FG_COLOR(1, 40, "PASS") \
    : test->res.fatal ? FG_COLOR(1, 196, "FAIL") \
    : FG_COLOR(1, 208, "WARN"))

# if defined(ST_RELATIVE_LINE_NUMBERS)
#  define _ST_TESTLINE() (__LINE__ - _retval.line_start)
# else
#  define _ST_TESTLINE() __LINE__
# endif

# define _ST_EVALUATE_EXPR(expr, name) \
    do { \
        if (!(expr)) { \
            _retval.pass = false; \
            _retval.fatal = true; \
            ST_ERROR("in " name " (line %"PRIu32"): '" #expr "' evaluated to false", _ST_TESTLINE()); \
        } \
    } while (false)

# define ST_EXPECT(expr) \
    do { \
        if (!(expr)) { \
            _retval.pass = false; \
            ST_WARNING("in ST_EXPECT (line %"PRIu32"): '" #expr "' evaluated to false", _ST_TESTLINE()); \
        } \
    } while (false)

# define ST_IS_TEST_PASSED() (_retval.pass)
# define ST_IS_FAILURE_FATAL() (_retval.fatal)

# define ST_REQUIRE(expr) _ST_EVALUATE_EXPR(expr, "ST_REQUIRE")

# define ST_BITS_HIGH(bitmask, bits) \
    _ST_EVALUATE_EXPR((bitmask & bits) == bits, "ST_BITS_HIGH")

# define ST_BITS_LOW(bitmask, bits) \
    _ST_EVALUATE_EXPR((bitmask & bits) == 0, "ST_BITS_LOW")

# define ST_EQUAL(lhs, rhs) \
    _ST_EVALUATE_EXPR(lhs == rhs, "ST_EQUAL")

# define ST_NOT_EQUAL(lhs, rhs) \
    _ST_EVALUATE_EXPR(lhs != rhs, "ST_NOT_EQUAL")

# define ST_LESS_THAN(lhs, rhs) \
    _ST_EVALUATE_EXPR(lhs < rhs, "ST_LESS_THAN")

# define ST_EQUAL_OR_LESS(lhs, rhs) \
    _ST_EVALUATE_EXPR(lhs <= rhs, "ST_EQUAL_OR_LESS")

# define ST_GREATER_THAN(lhs, rhs) \
    _ST_EVALUATE_EXPR(lhs > rhs, "ST_GREATER_THAN")

# define ST_EQUAL_OR_GREATER(lhs, rhs) \
    _ST_EVALUATE_EXPR(lhs >= rhs, "ST_EQUAL_OR_GREATER")

/**
 * String-specific
 */

# define ST_STR_EQUAL(str1, str2, len) \
    _ST_EVALUATE_EXPR(0 == st_strncmp(str1, str2, len), "ST_STR_EQUAL")

# define ST_STR_EQUAL_I(str1, str2, len) \
    _ST_EVALUATE_EXPR(0 == st_strnicmp(str1, str2, len), "ST_STR_EQUAL_I")

# define ST_STR_CONTAINS(needle, haystack) \
    _ST_EVALUATE_EXPR(NULL != st_strstr(haystack, needle), "ST_STR_CONTAINS")

# define ST_STR_CONTAINS_I(needle, haystack) \
    _ST_EVALUATE_EXPR(NULL != st_stristr(haystack, needle), "ST_STR_CONTAINS_I")

# define ST_STR_BEGINSWITH(needle, haystack, needle_len) \
    _ST_EVALUATE_EXPR(0 == st_strncmp(haystack, needle, needle_len), "ST_STR_BEGINSWITH")

# define ST_STR_BEGINSWITH_I(needle, haystack, needle_len) \
    _ST_EVALUATE_EXPR(0 == st_strnicmp(needle, haystack, needle_len), "ST_STR_BEGINSWITH_I")

# define ST_STR_ENDSWITH(needle, haystack, needle_len, haystack_len) \
    _ST_EVALUATE_EXPR( \
        0 == st_strncmp(haystack + (haystack_len - needle_len), needle, needle_len), \
        "ST_STR_ENDSWITH" \
    )

# define ST_STR_ENDSWITH_I(needle, haystack, needle_len, haystack_len) \
    _ST_EVALUATE_EXPR( \
        0 == st_strnicmp(haystack + (haystack_len - needle_len), needle, needle_len), \
        "ST_STR_ENDSWITH_I" \
    )

/**
 * Numeric
 */

# define ST_NUM_POSITIVE(num) \
    _ST_EVALUATE_EXPR(num > 0, "ST_NUM_POSITIVE")

# define ST_NUM_NEGATIVE(num) \
    _ST_EVALUATE_EXPR(num < 0, "ST_NUM_NEGATIVE")

# define ST_NUM_EVEN(num) \
    do { \
        div_t dt = div(num, 2); \
        _ST_EVALUATE_EXPR(dt.rem == 0, "ST_NUM_EVEN"); \
    } while (false)

# define ST_NUM_ODD(num) \
    do { \
        div_t dt = div(num, 2); \
        _ST_EVALUATE_EXPR(dt.rem != 0, "ST_NUM_ODD"); \
    } while (false)

# define ST_NUM_POWER_2(num) \
    do { \
        div_t dt = div(num, 2); \
        _ST_EVALUATE_EXPR(dt.rem == 0, "ST_NUM_POWER_2"); \
    } while (false)

# define ST_NUM_POWER_10(num) \
    do { \
        div_t dt = div(num, 10); \
        _ST_EVALUATE_EXPR(dt.rem == 0, "ST_NUM_POWER_10"); \
    } while (false)

/**
 * Array-specific
 */

# define ST_ARRAY_EQUAL(arr1, arr2) \
    do { \
        size_t elem_size1 = sizeof(arr1[0]); \
        size_t elem_size2 = sizeof(arr2[0]); \
        if (elem_size1 != elem_size2) { \
            _ST_EVALUATE_EXPR(elem_size1 == elem_size2, "ST_ARRAY_EQUAL"); \
            break; \
        } \
        size_t count1 = sizeof(arr1) / elem_size1; \
        size_t count2 = sizeof(arr2) / elem_size2; \
        if (count1 != count2) { \
            _ST_EVALUATE_EXPR(count1 == count2, "ST_ARRAY_EQUAL"); \
            break; \
        } \
        for (size_t n = 0; n < count1; n++) { \
            if (arr1[n] != arr2[n]) { \
                _ST_EVALUATE_EXPR(arr1[n] == arr2[n], "ST_ARRAY_EQUAL"); \
                break; \
            } \
        } \
    } while (false)

#endif /* !_SEATEST_MACROS_H_INCLUDED */
