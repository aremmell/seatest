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

/** Blots out a variable, avoiding compiler warnings/errors in the event that it is unreferenced. */
# define ST_UNUSED(var) (void)(var)

/** Returns the number of entries in an array. */
# define ST_COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))

/** Returns the plural or singular form of a word based on the count supplied.
 * Only works for words whose plural form is just an 's'. */
# define ST_PLURAL(word, count) ((!(count) || (count) > 1) ? word "s" : word)

/** The only code needed in main()/the entry point. `app_name` is a name to associate with your
 * test suite, such as the product or company name.. */
# define ST_MAIN_IMPL(app_name) \
    st_main(argc, argv, app_name, st_cl_args, ST_COUNTOF(st_cl_args), \
        st_tests, ST_COUNTOF(st_tests))

/** Declares static variables required by seatest. Place this above your entry point routine. */
# define ST_DECLARE_STATIC_VARS() \
    _ST_DECLARE_CL_ARGS();

/** Declares an individual test function. `name` must only contain characters allowed in C
 * function names. Place these declarations in a header file, or above your entry point routine. */
# define ST_DECLARE_TEST(name) \
    st_testres st_test_##name(void);

/** Begins the definition (implementation) of an individual test function. */
# define ST_BEGIN_TEST_IMPL(name) \
    st_testres st_test_##name(void) \
    { \
        st_testres _retval; \
        _retval.line_start = __LINE__; \
        _retval.skip = false; \
        _retval.pass = true; \
        _retval.fatal = false;

/** Ends the definition (implementation) of an individual test function. */
# define ST_END_TEST_IMPL() \
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
# define ST_END_DECLARE_TEST_LIST() \
    };

# define ST_TEST_PASSING() (_retval.pass)
# define ST_TEST_FAILED_FATALLY() (_retval.fatal)
# define ST_TEST_SKIPPED() (_retval.skip)

# define ST_EXPECT(expr) \
    _ST_EVALUATE_EXPR_RAW(expr, "ST_EXPECT", 208, false)

# define ST_REQUIRE(expr) _ST_EVALUATE_EXPR(expr, "ST_REQUIRE")

# define ST_EQUAL(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) == (rhs), "ST_EQUAL")

# define ST_NOT_EQUAL(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) != (rhs), "ST_NOT_EQUAL")

# define ST_LESS_THAN(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) < (rhs), "ST_LESS_THAN")

# define ST_LESS_THAN_OR_EQUAL(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) <= (rhs), "ST_LESS_THAN_OR_EQUAL")

# define ST_GREATER_THAN(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) > (rhs), "ST_GREATER_THAN")

# define ST_GREATER_THAN_OR_EQUAL(lhs, rhs) \
    _ST_EVALUATE_EXPR((lhs) >= (rhs), "ST_GREATER_THAN_OR_EQUAL")

# define ST_NULL(ptr) \
    _ST_EVALUATE_EXPR((ptr) == NULL, "ST_NULL")

# define ST_NOT_NULL(ptr) \
    _ST_EVALUATE_EXPR((ptr) != NULL, "ST_NOT_NULL")

/**
 * Bitwise
 */

# define ST_BITS_HIGH(bitmask, bits) \
    _ST_EVALUATE_EXPR(((bitmask) & (bits)) == (bits), "ST_BITS_HIGH")

# define ST_BITS_LOW(bitmask, bits) \
    _ST_EVALUATE_EXPR(((bitmask) & (bits)) == 0, "ST_BITS_LOW")

# define ST_BITWISE_EQUAL(lhs, rhs, size) \
    _ST_EVALUATE_EXPR(0 == memcmp(&(lhs), &(rhs), (size)), "ST_BITWISE_EQUAL")

# define ST_BITWISE_NOT_EQUAL(lhs, rhs, size) \
    _ST_EVALUATE_EXPR(0 != memcmp(&(lhs), &(rhs), (size)), "ST_BITWISE_NOT_EQUAL")

# define ST_BITWISE_ZEROED(ptr, size) \
    do { \
        const unsigned char* const p = (const unsigned char* const)ptr; \
        for (size_t n = 0; n < size; n++) { \
            if (p[n] != 0) { \
                _ST_EVALUATE_EXPR(p[n] == 0, "ST_BITWISE_ZEROED"); \
                break; \
            } \
        } \
    } while (false)

# define ST_BITWISE_NOT_ZEROED(ptr, size) \
    do { \
        const unsigned char* const p = (const unsigned char* const)ptr; \
        bool all_zero_bytes = true; \
        for (size_t n = 0; n < size; n++) { \
            if (p[n] != 0) { \
                all_zero_bytes = false; \
                break; \
            } \
        } \
        _ST_EVALUATE_EXPR(!all_zero_bytes, "ST_BITWISE_NOT_ZEROED"); \
    } while (false)

/**
 * String
 */

# define ST_STR_EQUAL(str1, str2, len) \
    _ST_EVALUATE_EXPR(0 == st_strncmp(str1, str2, len), "ST_STR_EQUAL")

# define ST_STR_NOT_EQUAL(str1, str2, len) \
    _ST_EVALUATE_EXPR(0 != st_strncmp(str1, str2, len), "ST_STR_NOT_EQUAL")

# define ST_STR_EQUAL_I(str1, str2, len) \
    _ST_EVALUATE_EXPR(0 == st_strnicmp(str1, str2, len), "ST_STR_EQUAL_I")

# define ST_STR_NOT_EQUAL_I(str1, str2, len) \
    _ST_EVALUATE_EXPR(0 != st_strnicmp(str1, str2, len), "ST_STR_NOT_EQUAL_I")

# define ST_STR_CONTAINS(needle, haystack) \
    _ST_EVALUATE_EXPR(NULL != st_strstr(haystack, needle), "ST_STR_CONTAINS")

# define ST_STR_NOT_CONTAINS(needle, haystack) \
    _ST_EVALUATE_EXPR(NULL == st_strstr(haystack, needle), "ST_STR_NOT_CONTAINS")

# define ST_STR_CONTAINS_I(needle, haystack) \
    _ST_EVALUATE_EXPR(NULL != st_stristr(haystack, needle), "ST_STR_CONTAINS_I")

# define ST_STR_NOT_CONTAINS_I(needle, haystack) \
    _ST_EVALUATE_EXPR(NULL == st_stristr(haystack, needle), "ST_STR_NOT_CONTAINS_I")

# define ST_STR_BEGINSWITH(needle, haystack, needle_len) \
    _ST_EVALUATE_EXPR(0 == st_strncmp(haystack, needle, needle_len), "ST_STR_BEGINSWITH")

# define ST_STR_NOT_BEGINSWITH(needle, haystack, needle_len) \
    _ST_EVALUATE_EXPR(0 != st_strncmp(haystack, needle, needle_len), "ST_STR_NOT_BEGINSWITH")

# define ST_STR_BEGINSWITH_I(needle, haystack, needle_len) \
    _ST_EVALUATE_EXPR(0 == st_strnicmp(needle, haystack, needle_len), "ST_STR_BEGINSWITH_I")

# define ST_STR_NOT_BEGINSWITH_I(needle, haystack, needle_len) \
    _ST_EVALUATE_EXPR(0 != st_strnicmp(needle, haystack, needle_len), "ST_STR_NOT_BEGINSWITH_I")

# define ST_STR_ENDSWITH(needle, haystack, needle_len, haystack_len) \
    _ST_EVALUATE_EXPR( \
        0 == st_strncmp((haystack) + ((haystack_len) - (needle_len)), (needle), (needle_len)), \
        "ST_STR_ENDSWITH" \
    )

# define ST_STR_NOT_ENDSWITH(needle, haystack, needle_len, haystack_len) \
    _ST_EVALUATE_EXPR( \
        0 != st_strncmp((haystack) + ((haystack_len) - (needle_len)), (needle), (needle_len)), \
        "ST_STR_NOT_ENDSWITH" \
    )

# define ST_STR_ENDSWITH_I(needle, haystack, needle_len, haystack_len) \
    _ST_EVALUATE_EXPR( \
        0 == st_strnicmp((haystack) + ((haystack_len) - (needle_len)), (needle), (needle_len)), \
        "ST_STR_ENDSWITH_I" \
    )

# define ST_STR_NOT_ENDSWITH_I(needle, haystack, needle_len, haystack_len) \
    _ST_EVALUATE_EXPR( \
        0 != st_strnicmp((haystack) + ((haystack_len) - (needle_len)), (needle), (needle_len)), \
        "ST_STR_NOT_ENDSWITH_I" \
    )

# define ST_STR_ALPHA(str) \
    do { \
        if (!(str) || !*(str)) { \
            _ST_EVALUATE_EXPR((str) && *(str) != '\0', "ST_STR_ALPHA"); \
            break; \
        } \
        const char* const p = (const char* const)(str); \
        for (size_t n = 0; n < strlen(str); n++) { \
            if (!isalpha(p[n])) { \
                _ST_EVALUATE_EXPR(isalpha(p[n]), "ST_STR_ALPHA"); \
                break; \
            } \
        } \
    } while (false)

# define ST_STR_NUMERIC(str) \
    do { \
        if (!(str) || !*(str)) { \
            _ST_EVALUATE_EXPR((str) && *(str) != '\0', "ST_STR_NUMERIC"); \
            break; \
        } \
        const char* const p = (const char* const)(str); \
        for (size_t n = 0; n < strlen(str); n++) { \
            if (!isdigit(p[n])) { \
                _ST_EVALUATE_EXPR(isdigit(p[n]), "ST_STR_NUMERIC"); \
                break; \
            } \
        } \
    } while (false)

# define ST_STR_ALPHANUMERIC(str) \
    do { \
        if (!(str) || !*(str)) { \
            _ST_EVALUATE_EXPR((str) && *(str) != '\0', "ST_STR_ALPHANUMERIC"); \
            break; \
        } \
        const char* const p = (const char* const)(str); \
        for (size_t n = 0; n < strlen(str); n++) { \
            if (!isalnum(p[n])) { \
                _ST_EVALUATE_EXPR(isalnum(p[n]), "ST_STR_ALPHANUMERIC"); \
                break; \
            } \
        } \
    } while (false)

/**
 * Numeric
 */

# define ST_NUM_POSITIVE(num) \
    _ST_EVALUATE_EXPR((num) > 0, "ST_NUM_POSITIVE")

# define ST_NUM_NEGATIVE(num) \
    _ST_EVALUATE_EXPR((num) < 0, "ST_NUM_NEGATIVE")

# define ST_NUM_EVEN(num) \
    do { \
        div_t dt = div((int)num, 2); \
        _ST_EVALUATE_EXPR(dt.rem == 0, "ST_NUM_EVEN"); \
    } while (false)

# define ST_NUM_ODD(num) \
    do { \
        div_t dt = div((int)num, 2); \
        _ST_EVALUATE_EXPR(dt.rem != 0, "ST_NUM_ODD"); \
    } while (false)

# define ST_NUM_MULTIPLE_OF(num, exp) \
    do { \
        div_t dt = div((int)num, (int)exp); \
        _ST_EVALUATE_EXPR(dt.rem == 0, "ST_NUM_MULTIPLE_OF"); \
    } while (false)

# define ST_NUM_NOT_MULTIPLE_OF(num, exp) \
    do { \
        div_t dt = div((int)num, (int)exp); \
        _ST_EVALUATE_EXPR(dt.rem != 0, "ST_NUM_NOT_MULTIPLE_OF"); \
    } while (false)

/**
 * Array
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

# define ST_ARRAY_NOT_EQUAL(arr1, arr2) \
    do { \
        size_t elem_size1 = sizeof(arr1[0]); \
        size_t elem_size2 = sizeof(arr2[0]); \
        if (elem_size1 != elem_size2) { \
            break; \
        } \
        size_t count1 = sizeof(arr1) / elem_size1; \
        size_t count2 = sizeof(arr2) / elem_size2; \
        if (count1 != count2) { \
            break; \
        } \
        bool all_elems_equal = true; \
        for (size_t n = 0; n < count1; n++) { \
            if (arr1[n] != arr2[n]) { \
                all_elems_equal = false; \
                break; \
            } \
        } \
        _ST_EVALUATE_EXPR(!all_elems_equal, "ST_ARRAY_NOT_EQUAL"); \
    } while (false)

# define ST_ARRAY_CONTAINS(arr, val) \
    do { \
        size_t elem_size = sizeof(arr[0]); \
        size_t val_size = sizeof(val); \
        if (elem_size != val_size) { \
            _ST_EVALUATE_EXPR(elem_size == val_size, "ST_ARRAY_CONTAINS"); \
            break; \
        } \
        bool val_found = false; \
        for (size_t n = 0; n < ST_COUNTOF(arr); n++) { \
            if (arr[n] == val) { \
                val_found = true; \
                break; \
            } \
        } \
        _ST_EVALUATE_EXPR(val_found, "ST_ARRAY_CONTAINS"); \
    } while (false)

# define ST_ARRAY_NOT_CONTAINS(arr, val) \
    do { \
        size_t elem_size = sizeof(arr[0]); \
        size_t val_size = sizeof(val); \
        if (elem_size != val_size) { \
            break; \
        } \
        for (size_t n = 0; n < ST_COUNTOF(arr); n++) { \
            if (arr[n] == val) { \
                _ST_EVALUATE_EXPR(arr[n] != val, "ST_ARRAY_NOT_CONTAINS"); \
                break; \
            } \
        } \
    } while (false)

#endif /* !_SEATEST_MACROS_H_INCLUDED */
