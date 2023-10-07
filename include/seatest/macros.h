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

/** The only code needed in main()/the entry point. `app_name` is a name to associate with the
 * test suite, such as a product or company name. */
# define ST_MAIN_IMPL(app_name) \
    st_main(argc, argv, app_name, st_cl_args, _ST_COUNTOF(st_cl_args), \
        st_tests, _ST_COUNTOF(st_tests))

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
        st_testres _retval = {0};

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

# define ST_TEST_IS_PASSING() (!_retval.fatal)
# define ST_TEST_HAS_WARNINGS() (!_retval.pass)
# define ST_TEST_IS_FAILED() (_retval.fatal)
# define ST_TEST_IS_SKIPPED() (_retval.skip)

# define ST_TEST_EXIT_IF_FAILED() \
    do { \
        if (ST_TEST_IS_FAILED()) { \
            ST_WARNING0("returning due to previous error(s)"); \
            return _retval; \
        } \
    } while (false)

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
        if (!(ptr)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(ptr), "ST_BITWISE_ZEROED"); \
            break; \
        } \
        for (size_t n = 0; n < size; n++) { \
            if (((const unsigned char*)(ptr))[n] != 0) { \
                _ST_EVALUATE_EXPR(((const unsigned char*)(ptr))[n] == 0, "ST_BITWISE_ZEROED"); \
                break; \
            } \
        } \
    } while (false)

# define ST_BITWISE_NOT_ZEROED(ptr, size) \
    do { \
        if (!(ptr)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(ptr), "ST_BITWISE_NOT_ZEROED"); \
            break; \
        } \
        bool all_bytes_zero = true; \
        for (size_t n = 0; n < size; n++) { \
            if (((const unsigned char*)(ptr))[n] != 0) { \
                all_bytes_zero = false; \
                break; \
            } \
        } \
        _ST_EVALUATE_EXPR(!all_bytes_zero, "ST_BITWISE_NOT_ZEROED"); \
    } while (false)

/**
 * String
 */

# define ST_STR_EMPTY(str) \
    _ST_EVALUATE_EXPR((str) == NULL || *(str) == '\0', "ST_STR_EMPTY")

# define ST_STR_NOT_EMPTY(str) \
    _ST_EVALUATE_EXPR(_ST_NOTNULL(str) && *(str) != '\0', "ST_STR_NOT_EMPTY")

# define ST_STR_EQUAL(str1, str2, len) \
    do { \
        if (!(str1) || !(str2)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str1) && _ST_NOTNULL(str2), "ST_STR_EQUAL"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!st_strncmp(str1, str2, len), "ST_STR_EQUAL"); \
    } while (false)

# define ST_STR_NOT_EQUAL(str1, str2, len) \
    do { \
        if (!(str1) || !(str2)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str1) && _ST_NOTNULL(str2), "ST_STR_NOT_EQUAL"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(st_strncmp(str1, str2, len), "ST_STR_NOT_EQUAL"); \
    } while (false)

# define ST_STR_EQUAL_I(str1, str2, len) \
    do { \
        if (!(str1) || !(str2)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str1) && _ST_NOTNULL(str2), "ST_STR_EQUAL_I"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!st_strnicmp(str1, str2, len), "ST_STR_EQUAL_I"); \
    } while (false)

# define ST_STR_NOT_EQUAL_I(str1, str2, len) \
    do { \
        if (!(str1) || !(str2)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str1) && _ST_NOTNULL(str2), "ST_STR_NOT_EQUAL_I"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(st_strnicmp((str1), (str2), (len)), "ST_STR_NOT_EQUAL_I"); \
    } while (false)

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

# define ST_STR_BEGINSWITH_WSPACE(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_BEGINSWITH_WSPACE"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(isspace((str)[0]), "ST_STR_BEGINSWITH_WSPACE"); \
    } while (false)

# define ST_STR_NOT_BEGINSWITH_WSPACE(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_NOT_BEGINSWITH_WSPACE"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!isspace((str)[0]), "ST_STR_NOT_BEGINSWITH_WSPACE"); \
    } while (false)

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

# define ST_STR_ENDSWITH_WSPACE(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_ENDSWITH_WSPACE"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(isspace((str)[strlen((str)) - 1]), "ST_STR_ENDSWITH_WSPACE"); \
    } while (false)

# define ST_STR_NOT_ENDSWITH_WSPACE(str) \
    do { \
        if (!(str)) { \
            _ST_EVALUATE_EXPR(_ST_NOTNULL(str), "ST_STR_NOT_ENDSWITH_WSPACE"); \
            break; \
        } \
        _ST_EVALUATE_EXPR(!isspace((str)[strlen((str)) - 1]), "ST_STR_NOT_ENDSWITH_WSPACE"); \
    } while (false)

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

# define ST_NUM_POSITIVE(num) \
    _ST_EVALUATE_EXPR((num) > 0, "ST_NUM_POSITIVE")

# define ST_NUM_NEGATIVE(num) \
    _ST_EVALUATE_EXPR((num) < 0, "ST_NUM_NEGATIVE")

# define ST_NUM_EVEN(num) \
    _ST_EVALUATE_EXPR(div((int)(num), 2).rem == 0, "ST_NUM_EVEN"); \

# define ST_NUM_ODD(num) \
    _ST_EVALUATE_EXPR(div((int)(num), 2).rem != 0, "ST_NUM_ODD"); \

# define ST_NUM_MULTIPLE_OF(num, mul) \
    _ST_EVALUATE_EXPR(div((int)(num), (int)(mul)).rem == 0, "ST_NUM_MULTIPLE_OF"); \

# define ST_NUM_NOT_MULTIPLE_OF(num, mul) \
    _ST_EVALUATE_EXPR(div((int)(num), (int)(mul)).rem != 0, "ST_NUM_NOT_MULTIPLE_OF"); \

# define ST_NUM_IN_RANGE(num, low, high) \
    _ST_EVALUATE_EXPR((num) >= (low) && (num) <= (high), "ST_NUM_IN_RANGE")

# define ST_NUM_NOT_IN_RANGE(num, low, high) \
    _ST_EVALUATE_EXPR((num) < (low) || (num) > (high), "ST_NUM_NOT_IN_RANGE")

/**
 * Array
 */

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
