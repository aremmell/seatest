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

/** The only code needed in main()/the entry point. `app_name` is a name to associate
 * with the test rig, such as a product or company name. */
# define ST_MAIN_IMPL(app_name) \
    st_main(argc, argv, app_name, st_cl_args, _ST_COUNTOF(st_cl_args), \
        st_tests, _ST_COUNTOF(st_tests))

/** Declares static variables required by seatest. Place above the entry point routine. */
# define ST_DECLARE_STATIC_VARS() \
    _ST_DECLARE_CL_ARGS();

/** Declares an individual test function. `name` must only contain characters allowed
 * in C function names. Place these declarations in a header file, or above the entry
 *  point routine. */
# define ST_DECLARE_TEST(name) \
    st_testres st_test_##name(void);

/** Begins the definition (implementation) of an individual test function. */
# define ST_BEGIN_TEST_IMPL(name) \
    st_testres st_test_##name(void) \
    { \
        st_testres __retval = {0}; \
        __retval.pass = true;

/** Ends the definition (implementation) of an individual test function. */
# define ST_END_TEST_IMPL() \
        _ST_VALIDATE_RETURN(); \
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
# define ST_END_DECLARE_TEST_LIST() \
    };

# if defined(ST_DEBUG_MESSAGES)
/** Emits a diagnotic message in gray. Accepts printf-like variable arguments and requires a
 * minimum of two arguments; use ST_DEBUG0() if just a simple message is required. */
#  define ST_DEBUG(msg, ...)   __ST_MESSAGE(ST_LOC_INDENT DGRAY(msg) "\n", __VA_ARGS__)

/** Emits a diagnotic message in gray. Only accepts a single message argument; use ST_DEBUG()
 * for printf-like behavior. */
#  define ST_DEBUG0(msg)       __ST_MESSAGE(ST_LOC_INDENT DGRAY(msg) "\n")
# else
#  define ST_DEBUG(...)
#  define ST_DEBUG0(...)
# endif

/** Emits an informative message. Accepts printf-like variable arguments and requires a
 * minimum of two arguments; use ST_MESSAGE0() if just a simple message is required. */
# define ST_MESSAGE(msg, ...)  __ST_MESSAGE(ST_LOC_INDENT WHITE(msg) "\n", __VA_ARGS__)

/** Emits an informative message. Only accepts a single message argument; use ST_MESSAGE()
 * for printf-like behavior. */
# define ST_MESSAGE0(msg)      __ST_MESSAGE(ST_LOC_INDENT WHITE(msg) "\n")

/** Emits a message in green, indicating something positive. Accepts printf-like variable
 * arguments and requires a minimum of two arguments; use ST_SUCCESS0() if just a simple
 * message is required. */
# define ST_SUCCESS(msg, ...)  __ST_MESSAGE(ST_LOC_INDENT FG_COLOR(0, 40, msg) "\n", __VA_ARGS__)

/** Emits a message in green, indicating something positive. Only accepts a single message
 * argument; use ST_SUCCESS() for printf-like behavior. */
# define ST_SUCCESS0(msg)      __ST_MESSAGE(ST_LOC_INDENT FG_COLOR(0, 40, msg) "\n")

/** Emits a message in orange, indicating a warning condition. Accepts printf-like variable
 * arguments and requires a minimum of two arguments; use ST_WARNING0() if just a simple
 * message is required. */
# define ST_WARNING(msg, ...)  __ST_MESSAGE(ST_LOC_INDENT FG_COLOR(0, 208, msg) "\n", __VA_ARGS__)

/** Emits a message in orange, indicating a warning condition. Only accepts a single message
 * argument; use ST_WARNING() for printf-like behavior. */
# define ST_WARNING0(msg)      __ST_MESSAGE(ST_LOC_INDENT FG_COLOR(0, 208, msg) "\n")

/** Emits a message in red, indicating an error condition. Accepts printf-like variable
 * arguments and requires a minimum of two arguments; use ST_ERROR0() if just a simple
 * message is required. */
# define ST_ERROR(msg, ...)    __ST_MESSAGE(ST_LOC_INDENT FG_COLOR(0, 196, msg) "\n", __VA_ARGS__)

/** Emits a message in red, indicating an error condition. Only accepts a single message
 * argument; use ST_ERROR() for printf-like behavior. */
# define ST_ERROR0(msg)        __ST_MESSAGE(ST_LOC_INDENT FG_COLOR(0, 196, msg) "\n")

# define ST_TEST_IS_FAILED()       (__retval.fatal)
# define ST_TEST_IS_PASSING()      (!__retval.fatal)
# define ST_TEST_LAST_EVAL_FALSE() (__retval.last_fail)
# define ST_TEST_ERROR_COUNT()     (__retval.errors)
# define ST_TEST_WARNING_COUNT()   (__retval.warnings)

# define ST_TEST_EXIT_IF_FAILED() \
    do { \
        if (ST_TEST_IS_FAILED()) { \
            return __retval; \
        } \
    } while (false)

# define ST_OS_ERROR_MSG(code, context) \
    do { \
        char message[ST_MAX_ERROR] = {0}; \
        (void)st_format_error_msg(code, message); \
        ST_ERROR(ST_LOC_ERROR " ("ST_LOC_LINE" %"PRIu32"): %s: %d (%s) ", __LINE__, \
            context, code, message); \
    } while (false)

/** Evaluates whether `expr` evaluates to true. Emits a warning message and marks
 * the test with a warning otherwise. */
# define ST_EXPECT(expr) \
    _ST_EVALUATE_EXPR_RAW(expr, "ST_EXPECT", 208, false)

/** Evaluates whether `expr` evaluates to true. */
# define ST_TRUE(expr) _ST_EVALUATE_EXPR(expr, "ST_TRUE")

/** Evaluates whether `expr` evaluates to false. */
# define ST_FALSE(expr) _ST_EVALUATE_EXPR(!(expr), "ST_FALSE")

/** Evaluates whether `lhs` is equal to `rhs`. */
# define ST_EQUAL(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) == (rhs), "ST_EQUAL")

/** Evaluates whether `lhs` is not equal to `rhs`. */
# define ST_NOT_EQUAL(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) != (rhs), "ST_NOT_EQUAL")

/** Evaluates whether `lhs` is less than `rhs`. */
# define ST_LESS_THAN(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) < (rhs), "ST_LESS_THAN")

/** Evaluates whether `lhs` is less than or equal to `rhs`. */
# define ST_LESS_THAN_OR_EQUAL(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) <= (rhs), "ST_LESS_THAN_OR_EQUAL")

/** Evaluates whether `lhs` is greater than `rhs`. */
# define ST_GREATER_THAN(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) > (rhs), "ST_GREATER_THAN")

/** Evaluates whether `lhs` is greater than or equal to `rhs`. */
# define ST_GREATER_THAN_OR_EQUAL(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) >= (rhs), "ST_GREATER_THAN_OR_EQUAL")

/** Evaluates whether `ptr` is null. */
# define ST_NULL(ptr) \
    _ST_EVALUATE_EXPR((ptr) == NULL, "ST_NULL")

/** Evaluates whether `ptr` is non-null. */
# define ST_NOT_NULL(ptr) \
    _ST_EVALUATE_EXPR((ptr) != NULL, "ST_NOT_NULL")

/**
 * Bitwise
 */

/** Evaluates whether the bit(s) in `bits` are set to one in `bitmask`. */
# define ST_BITS_HIGH(bitmask, bits) \
    _ST_EVALUATE_EXPR(((bitmask) & (bits)) == (bits), "ST_BITS_HIGH")

/** Evaluates whether the bit(s) in `bits` are set to zero in `bitmask`. */
# define ST_BITS_LOW(bitmask, bits) \
    _ST_EVALUATE_EXPR(((bitmask) & (bits)) == 0, "ST_BITS_LOW")

/** Evaluates whether `lhs` and `rhs` contain exactly the same bytes (up to `size`). */
# define ST_BITWISE_EQUAL(lhs, rhs, size) \
    _ST_EVALUATE_EXPR(0 == memcmp(&(lhs), &(rhs), (size)), "ST_BITWISE_EQUAL")

/** Evaluates whether `lhs` and `rhs` do not contain exactly the same bytes
 * (up to `size`). */
# define ST_BITWISE_NOT_EQUAL(lhs, rhs, size) \
    _ST_EVALUATE_EXPR(0 != memcmp(&(lhs), &(rhs), (size)), "ST_BITWISE_NOT_EQUAL")

/** Evaluates whether `obj` is comprised of all zero bytes. */
# define ST_BITWISE_ZEROED(obj, size) \
    do { \
        for (size_t n = 0; n < size; n++) { \
            if (((const unsigned char*)(&(obj)) + n) != 0) { \
                _ST_EVALUATE_EXPR(((const unsigned char*)(&(obj)) + n) == 0, "ST_BITWISE_ZEROED"); \
                break; \
            } \
        } \
    } while (false)

/** Evaluates whether `obj` is not comprised of all zero bytes. */
# define ST_BITWISE_NOT_ZEROED(obj, size) \
    do { \
        bool all_bytes_zero = true; \
        for (size_t n = 0; n < size; n++) { \
            if (((const unsigned char*)(&(obj)) + n) != 0) { \
                all_bytes_zero = false; \
                break; \
            } \
        } \
        _ST_EVALUATE_EXPR(!all_bytes_zero, "ST_BITWISE_NOT_ZEROED"); \
    } while (false)

/**
 * String
 */

/** Evaluates whether `str` is null or contains a null terminator at index zero. */
# define ST_STR_EMPTY(str) \
    _ST_EVALUATE_EXPR((str) == NULL || *(str) == '\0', "ST_STR_EMPTY")

/** Evaluates whether `str` is non-null and contains a value other than a null
 * terminator at index zero. */
# define ST_STR_NOT_EMPTY(str) \
    _ST_EVALUATE_EXPR(_ST_NOTNULL(str) && *(str) != '\0', "ST_STR_NOT_EMPTY")

/** Evaluates whether `str1` and `str2` equivalent (up to `len` characters). The
 * comparison is case-sensitive. */
# define ST_STR_EQUAL(str1, str2, len) \
    do { \
        if (!(str1) || !(str2)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str1) && _ST_NOTNULL(str2), "ST_STR_EQUAL"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!st_strncmp(str1, str2, len), "ST_STR_EQUAL"); \
    } while (false)

/** Evaluates whether `str1` and `str2` are not equivalent (up to `len` characters).
 * The comparison is case-sensitive. */
# define ST_STR_NOT_EQUAL(str1, str2, len) \
    do { \
        if (!(str1) || !(str2)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str1) && _ST_NOTNULL(str2), "ST_STR_NOT_EQUAL"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(st_strncmp(str1, str2, len), "ST_STR_NOT_EQUAL"); \
    } while (false)

/** Evaluates whether `str1` and `str2` are equivalent (up to `len` characters).
 * The comparison is case-insensitive. */
# define ST_STR_EQUAL_I(str1, str2, len) \
    do { \
        if (!(str1) || !(str2)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str1) && _ST_NOTNULL(str2), "ST_STR_EQUAL_I"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!st_strnicmp(str1, str2, len), "ST_STR_EQUAL_I"); \
    } while (false)

/** Evaluates whether `str1` and `str2` are not equivalent (up to `len` characters).
 * The comparison is case-insensitive. */
# define ST_STR_NOT_EQUAL_I(str1, str2, len) \
    do { \
        if (!(str1) || !(str2)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str1) && _ST_NOTNULL(str2), "ST_STR_NOT_EQUAL_I"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(st_strnicmp((str1), (str2), (len)), "ST_STR_NOT_EQUAL_I"); \
    } while (false)

/** Evaluates whether `haystack` contains the sub-string `needle`. The search is
 * case-sensitive. */
# define ST_STR_CONTAINS(needle, haystack) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_CONTAINS" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR(st_strstr(haystack, needle), "ST_STR_CONTAINS"); \
    } while (false)

/** Evaluates whether `haystack` does not contain the sub-string `needle`. The
 * search is case-sensitive. */
# define ST_STR_NOT_CONTAINS(needle, haystack) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_NOT_CONTAINS" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!st_strstr(haystack, needle), "ST_STR_NOT_CONTAINS"); \
    } while (false)

/** Evaluates whether `haystack` contains the sub-string `needle`. The search is
 * case-insensitive. */
# define ST_STR_CONTAINS_I(needle, haystack) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_CONTAINS_I" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR(st_stristr((haystack), (needle)), "ST_STR_CONTAINS_I"); \
    } while (false)

/** Evaluates whether `haystack` does not contain the sub-string `needle`. The
 * search is case-insensitive. */
# define ST_STR_NOT_CONTAINS_I(needle, haystack) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_NOT_CONTAINS_I" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!st_stristr((haystack), (needle)), "ST_STR_NOT_CONTAINS_I"); \
    } while (false)

/** Evaluates whether `haystack` begins with the string `needle`. The search is
 * case-sensitive. */
# define ST_STR_BEGINSWITH(needle, haystack, needle_len) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_BEGINSWITH" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!st_strncmp((haystack), (needle), (needle_len)), "ST_STR_BEGINSWITH"); \
    } while (false)

/** Evaluates whether `haystack` does not begin with the string `needle`. The
 * search is case-sensitive. */
# define ST_STR_NOT_BEGINSWITH(needle, haystack, needle_len) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_NOT_BEGINSWITH" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR(st_strncmp((haystack), (needle), (needle_len)), "ST_STR_NOT_BEGINSWITH"); \
    } while (false)

/** Evaluates whether `haystack` begins with the string `needle`. The search is
 * case-insensitive. */
# define ST_STR_BEGINSWITH_I(needle, haystack, needle_len) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_BEGINSWITH_I" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!st_strnicmp((needle), (haystack), (needle_len)), "ST_STR_BEGINSWITH_I"); \
    } while (false)

/** Evaluates whether `haystack` does not begin with the string `needle`. The
 * search is case-insensitive. */
# define ST_STR_NOT_BEGINSWITH_I(needle, haystack, needle_len) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_NOT_BEGINSWITH_I" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR(st_strnicmp((needle), (haystack), (needle_len)), "ST_STR_NOT_BEGINSWITH_I"); \
    } while (false)

/** Evaluates whether `str` begins with a whitespace character. For further infor-
 * mation, consult the documentation for isspace(). */
# define ST_STR_BEGINSWITH_WSPACE(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_BEGINSWITH_WSPACE"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(isspace((str)[0]), "ST_STR_BEGINSWITH_WSPACE"); \
    } while (false)

/** Evaluates whether `str` does not begin with a whitespace character. For further
 * information, consult the documentation for isspace(). */
# define ST_STR_NOT_BEGINSWITH_WSPACE(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_NOT_BEGINSWITH_WSPACE"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!isspace((str)[0]), "ST_STR_NOT_BEGINSWITH_WSPACE"); \
    } while (false)

/** Evaluates whether `haystack` ends with the string `needle`. The search is case-
 * sensitive. */
# define ST_STR_ENDSWITH(needle, haystack, needle_len, haystack_len) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_ENDSWITH" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR( \
            !st_strncmp((haystack) + ((haystack_len) - (needle_len)), (needle), (needle_len)), \
            "ST_STR_ENDSWITH" \
        ); \
    } while (false)

/** Evaluates whether `haystack` does not end with the string `needle`. The search
 * is case-sensitive. */
# define ST_STR_NOT_ENDSWITH(needle, haystack, needle_len, haystack_len) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_NOT_ENDSWITH" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR( \
            st_strncmp((haystack) + ((haystack_len) - (needle_len)), (needle), (needle_len)), \
            "ST_STR_NOT_ENDSWITH" \
        ); \
    } while (false)

/** Evaluates whether `haystack` ends with the string `needle`. The search is
 * case-insensitive. */
# define ST_STR_ENDSWITH_I(needle, haystack, needle_len, haystack_len) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_ENDSWITH_I" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR( \
            !st_strnicmp((haystack) + ((haystack_len) - (needle_len)), (needle), (needle_len)), \
            "ST_STR_ENDSWITH_I" \
        ); \
    } while (false)

/** Evaluates whether `haystack` does not end with the string `needle`. The search
 * is case-insensitive. */
# define ST_STR_NOT_ENDSWITH_I(needle, haystack, needle_len, haystack_len) \
    do { \
        if (!(needle) || !(haystack)) { \
            _ST_EVALUATE_EXPR( \
                _ST_NOTNULL(needle) && _ST_NOTNULL(haystack), "ST_STR_NOT_ENDSWITH_I" \
            ); \
            break; \
        } \
        _ST_EVALUATE_EXPR( \
            st_strnicmp((haystack) + ((haystack_len) - (needle_len)), (needle), (needle_len)), \
            "ST_STR_NOT_ENDSWITH_I" \
        ); \
    } while (false)

/** Evaluates whether `str` ends with a whitespace character. For further infor-
 * mation, consult the documentation for isspace(). */
# define ST_STR_ENDSWITH_WSPACE(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_ENDSWITH_WSPACE"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(isspace((str)[strlen((str)) - 1]), "ST_STR_ENDSWITH_WSPACE"); \
    } while (false)

/** Evaluates whether `str` does not end with a whitespace character. For further
 * information, consult the documentation for isspace(). */
# define ST_STR_NOT_ENDSWITH_WSPACE(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_NOT_ENDSWITH_WSPACE"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!isspace((str)[strlen((str)) - 1]), "ST_STR_NOT_ENDSWITH_WSPACE"); \
    } while (false)

/** Evaluates whether `str` contains only characters in the alphabet. For further
 * information, consult the documentation for isalpha(). */
# define ST_STR_ALPHA(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_ALPHA"); \
            break; \
        } \
        for (size_t n = 0; n < strlen((str)); n++) { \
            if (!isalpha((str)[n])) { \
                _ST_EVALUATE_EXPR(isalpha((str)[n]), "ST_STR_ALPHA"); \
                break; \
            } \
        } \
    } while (false)

/** Evaluates whether `str` only contains the characters '0'-'9'. */
# define ST_STR_NUMERIC(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_NUMERIC"); \
            break; \
        } \
        for (size_t n = 0; n < strlen((str)); n++) { \
            if (!isdigit((str)[n])) { \
                _ST_EVALUATE_EXPR(isdigit((str)[n]), "ST_STR_NUMERIC"); \
                break; \
            } \
        } \
    } while (false)

/** Evaluates whether `str` contains only alphanumeric characters. For further
 * information, consult the documentation for isalnum(). */
# define ST_STR_ALPHANUMERIC(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_ALPHANUMERIC"); \
            break; \
        } \
        for (size_t n = 0; n < strlen((str)); n++) { \
            if (!isalnum((str)[n])) { \
                _ST_EVALUATE_EXPR(isalnum((str)[n]), "ST_STR_ALPHANUMERIC"); \
                break; \
            } \
        } \
    } while (false)

/**
 * Numeric
 */

/** Evaluates whether `num` is positive.*/
# define ST_NUM_POSITIVE(num) \
    _ST_EVALUATE_EXPR((num) > 0, "ST_NUM_POSITIVE")

/** Evaluates whether `num` is negative.*/
# define ST_NUM_NEGATIVE(num) \
    _ST_EVALUATE_EXPR((num) < 0, "ST_NUM_NEGATIVE")

/** Evaluates whether `num` is even.*/
# define ST_NUM_EVEN(num) \
    _ST_EVALUATE_EXPR(div((int)(num), 2).rem == 0, "ST_NUM_EVEN")

/** Evaluates whether `num` is odd.*/
# define ST_NUM_ODD(num) \
    _ST_EVALUATE_EXPR(div((int)(num), 2).rem != 0, "ST_NUM_ODD")

/** Evaluates whether `num` is a multiple of `mul`.*/
# define ST_NUM_MULTIPLE_OF(num, mul) \
    _ST_EVALUATE_EXPR(div((int)(num), (int)(mul)).rem == 0, "ST_NUM_MULTIPLE_OF")

/** Evaluates whether `num` is not a multiple of `mul`.*/
# define ST_NUM_NOT_MULTIPLE_OF(num, mul) \
    _ST_EVALUATE_EXPR(div((int)(num), (int)(mul)).rem != 0, "ST_NUM_NOT_MULTIPLE_OF")

/** Evaluates whether `num` lies inside the range `low`..`high`.*/
# define ST_NUM_IN_RANGE(num, low, high) \
    _ST_EVALUATE_EXPR((num) >= (low) && (num) <= (high), "ST_NUM_IN_RANGE")

/** Evaluates whether `num` lies outside the range `low`..`high`.*/
# define ST_NUM_NOT_IN_RANGE(num, low, high) \
    _ST_EVALUATE_EXPR((num) < (low) || (num) > (high), "ST_NUM_NOT_IN_RANGE")

/**
 * Array
 */

/** Evaluates whether `arr1` and `arr2` contain items of the same size, have the
 * same number of elements, and all elements at the corresponding indices have the
 * same value. */
# define ST_ARRAY_EQUAL(arr1, arr2) \
    do { \
        size_t elem_size1 = sizeof((arr1)[0]); \
        size_t elem_size2 = sizeof((arr2)[0]); \
        if (elem_size1 != elem_size2) { \
            _ST_EVALUATE_EXPR(elem_size1 == elem_size2, "ST_ARRAY_EQUAL"); \
            break; \
        } \
        size_t count1 = sizeof((arr1)) / elem_size1; \
        size_t count2 = sizeof((arr2)) / elem_size2; \
        if (count1 != count2) { \
            _ST_EVALUATE_EXPR(count1 == count2, "ST_ARRAY_EQUAL"); \
            break; \
        } \
        for (size_t n = 0; n < count1; n++) { \
            if ((arr1)[n] != (arr2)[n]) { \
                _ST_EVALUATE_EXPR((arr1)[n] == (arr2)[n], "ST_ARRAY_EQUAL"); \
                break; \
            } \
        } \
    } while (false)

/** Evaluates whether `arr1` and `arr2` contain elements differing in size,
 * have differing numbers of elements, or at least one element is not equal to
 * its correspdonding element in the other array. */
# define ST_ARRAY_NOT_EQUAL(arr1, arr2) \
    do { \
        size_t elem_size1 = sizeof((arr1)[0]); \
        size_t elem_size2 = sizeof((arr2)[0]); \
        if (elem_size1 != elem_size2) { \
            break; \
        } \
        size_t count1 = sizeof((arr1)) / elem_size1; \
        size_t count2 = sizeof((arr2)) / elem_size2; \
        if (count1 != count2) { \
            break; \
        } \
        bool all_elems_equal = true; \
        for (size_t n = 0; n < count1; n++) { \
            if ((arr1)[n] != (arr2)[n]) { \
                all_elems_equal = false; \
                break; \
            } \
        } \
        _ST_EVALUATE_EXPR(!all_elems_equal, "ST_ARRAY_NOT_EQUAL"); \
    } while (false)

/** Evaluates whether `arr` contains at least one instance of `val`. */
# define ST_ARRAY_CONTAINS(arr, val) \
    do { \
        size_t elem_size = sizeof((arr)[0]); \
        size_t val_size = sizeof((val)); \
        if (elem_size != val_size) { \
            _ST_EVALUATE_EXPR(elem_size == val_size, "ST_ARRAY_CONTAINS"); \
            break; \
        } \
        bool value_found = false; \
        for (size_t n = 0; n < _ST_COUNTOF(arr); n++) { \
            if ((arr)[n] == (val)) { \
                value_found = true; \
                break; \
            } \
        } \
        _ST_EVALUATE_EXPR(value_found, "ST_ARRAY_CONTAINS"); \
    } while (false)

/** Evaluates whether `arr` does not contain `val`. */
# define ST_ARRAY_NOT_CONTAINS(arr, val) \
    do { \
        size_t elem_size = sizeof((arr)[0]); \
        size_t val_size = sizeof((val)); \
        if (elem_size != val_size) { \
            break; \
        } \
        for (size_t n = 0; n < _ST_COUNTOF(arr); n++) { \
            if ((arr)[n] == (val)) { \
                _ST_EVALUATE_EXPR((arr)[n] != (val), "ST_ARRAY_NOT_CONTAINS"); \
                break; \
            } \
        } \
    } while (false)

/** Evaluates whether all of the elements in `arr` are unique in value. */
# define ST_ARRAY_UNIQUE(arr) \
    do { \
        for (size_t n = 0; n < _ST_COUNTOF(arr); n++) { \
            bool match = false; \
            for (size_t j = 0; j < _ST_COUNTOF(arr); j++) { \
                if (j != n && (arr)[j] == (arr)[n]) { \
                    _ST_EVALUATE_EXPR((arr)[j] != (arr)[n], "ST_ARRAY_UNIQUE"); \
                    match = true; \
                    break; \
                } \
            } \
            if (match) { \
                break; \
            } \
        } \
    } while (false)

#endif /* !_SEATEST_MACROS_H_INCLUDED */
