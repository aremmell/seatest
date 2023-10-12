/*
 * sandbox.c
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
#include "seatest.h"

ST_DECLARE_STATIC_VARS()

ST_DECLARE_TEST(test_tests)
ST_DECLARE_TEST(requires_inet)

ST_BEGIN_DECLARE_TEST_LIST()
    ST_DECLARE_TEST_LIST_ENTRY(testing-the-tests, test_tests)
    ST_DECLARE_TEST_LIST_ENTRY_COND(requires-inet, requires_inet, COND_INET)
ST_END_DECLARE_TEST_LIST()

int main(int argc, char** argv)
{
    return ST_MAIN_IMPL("selftest");
}

ST_BEGIN_TEST_IMPL(test_tests)
{
    // console output helpers
    ST_DEBUG0("diagnostically diagnostic");
    ST_MESSAGE0("informationally informational");
    ST_WARNING0("a stern warning");
    ST_ERROR0("something has gone terribly wrong");

    // should all succeed
    const char* msg = "hello there";
    const char* msg2 = "\ni'm surrounded!\r\n";
    ST_TRUE(strlen(msg) > 10);
    ST_FALSE(strlen(msg) < 10);
    ST_EXPECT(msg != NULL);
    ST_EQUAL(1, 1);
    ST_NOT_EQUAL(1, 2);
    ST_LESS_THAN(1, 2);
    ST_LESS_THAN_OR_EQUAL(2, 2);
    ST_GREATER_THAN(2, 1);
    ST_GREATER_THAN_OR_EQUAL(2, 2);
    ST_NULL(NULL);
    ST_NOT_NULL(msg);

    // should all fail
    ST_MESSAGE0("expecting the next 12 to fail");
    ST_TRUE(strlen(msg) < 10);
    ST_FALSE(strlen(msg) > 10);
    ST_EXPECT(msg == NULL);
    ST_EQUAL(0, 1);
    ST_NOT_EQUAL(1, 1);
    ST_LESS_THAN(2, 2);
    ST_LESS_THAN_OR_EQUAL(3, 2);
    ST_GREATER_THAN(1, 2);
    ST_GREATER_THAN_OR_EQUAL(2, 3);
    ST_NULL(msg);
    ST_NOT_NULL(NULL);

    // should all succeed
    ST_STR_CONTAINS("the", msg);
    ST_STR_NOT_CONTAINS("foo", msg);
    ST_STR_CONTAINS_I("LLO", msg);
    ST_STR_NOT_CONTAINS_I("FOO", msg);
    ST_STR_BEGINSWITH("hell", msg, 4);
    ST_STR_NOT_BEGINSWITH("lleh", msg, 4);
    ST_STR_BEGINSWITH_I("HELL", msg, 4);
    ST_STR_NOT_BEGINSWITH_I("LLEH", msg, 4);
    ST_STR_BEGINSWITH_WSPACE(msg2);
    ST_STR_NOT_BEGINSWITH_WSPACE(msg);
    ST_STR_ENDSWITH("here", msg, 4, strlen(msg));
    ST_STR_NOT_ENDSWITH("ereh", msg, 4, strlen(msg));
    ST_STR_ENDSWITH_I("HERE", msg, 4, strlen(msg));
    ST_STR_NOT_ENDSWITH_I("EREH", msg, 4, strlen(msg));
    ST_STR_ENDSWITH_WSPACE(msg2);
    ST_STR_NOT_ENDSWITH_WSPACE(msg);
    ST_STR_ALPHA("loremipsum");
    ST_STR_NUMERIC("123456");
    ST_STR_ALPHANUMERIC("lorem123ipsum456");

    // should all fail
    ST_MESSAGE0("expecting the next 38 to fail");
    const char* null_msg = NULL;
    ST_STR_CONTAINS("sea", msg);
    ST_STR_NOT_CONTAINS("hell", msg);
    ST_STR_CONTAINS_I("tes", msg);
    ST_STR_NOT_CONTAINS_I("HELL", msg);
    ST_STR_BEGINSWITH("s", msg, 1);
    ST_STR_NOT_BEGINSWITH("hell", msg, 4);
    ST_STR_BEGINSWITH_I("S", msg, 1);
    ST_STR_NOT_BEGINSWITH_I("HELL", msg, 4);
    ST_STR_BEGINSWITH_WSPACE(msg);
    ST_STR_NOT_BEGINSWITH_WSPACE(msg2);
    ST_STR_ENDSWITH("hell", msg, 4, strlen(msg));
    ST_STR_NOT_ENDSWITH("here", msg, 4, strlen(msg));
    ST_STR_ENDSWITH_I("HELL", msg, 4, strlen(msg));
    ST_STR_NOT_ENDSWITH_I("HERE", msg, 4, strlen(msg));
    ST_STR_ENDSWITH_WSPACE(msg);
    ST_STR_NOT_ENDSWITH_WSPACE(msg2);
    ST_STR_ALPHA("not-alpha");
    ST_STR_NUMERIC("not-numeric");
    ST_STR_ALPHANUMERIC("not-alphanumeric");
    ST_STR_CONTAINS(null_msg, msg);
    ST_STR_NOT_CONTAINS(null_msg, msg);
    ST_STR_CONTAINS_I(null_msg, msg);
    ST_STR_NOT_CONTAINS_I(null_msg, msg);
    ST_STR_BEGINSWITH(null_msg, msg, 1);
    ST_STR_NOT_BEGINSWITH(null_msg, msg, 4);
    ST_STR_BEGINSWITH_I(null_msg, msg, 1);
    ST_STR_NOT_BEGINSWITH_I(null_msg, msg, 4);
    ST_STR_BEGINSWITH_WSPACE(null_msg);
    ST_STR_NOT_BEGINSWITH_WSPACE(null_msg);
    ST_STR_ENDSWITH(null_msg, msg, 4, strlen(msg));
    ST_STR_NOT_ENDSWITH(null_msg, msg, 4, strlen(msg));
    ST_STR_ENDSWITH_I(null_msg, msg, 4, strlen(msg));
    ST_STR_NOT_ENDSWITH_I(null_msg, msg, 4, strlen(msg));
    ST_STR_ENDSWITH_WSPACE(null_msg);
    ST_STR_NOT_ENDSWITH_WSPACE(null_msg);
    ST_STR_ALPHA(null_msg);
    ST_STR_NUMERIC(null_msg);
    ST_STR_ALPHANUMERIC(null_msg);

    static const int low = 10;
    static const int high = 100;

    // should all succeed
    ST_NUM_EVEN(48);
    ST_NUM_ODD(49);
    ST_NUM_NEGATIVE(-4);
    ST_NUM_POSITIVE(4);
    ST_NUM_MULTIPLE_OF(8192, 2);
    ST_NUM_NOT_MULTIPLE_OF(8193, 2);
    ST_NUM_IN_RANGE(50, low, high);
    ST_NUM_NOT_IN_RANGE(9, low, high);

    // should all fail
    ST_MESSAGE0("expecting the next 10 to fail");
    ST_NUM_EVEN(49);
    ST_NUM_ODD(48);
    ST_NUM_NEGATIVE(4);
    ST_NUM_POSITIVE(-4);
    ST_NUM_MULTIPLE_OF(8193, 2);
    ST_NUM_MULTIPLE_OF(10000001, 10);
    ST_NUM_NOT_MULTIPLE_OF(8192, 2);
    ST_NUM_NOT_MULTIPLE_OF(10000000, 10);
    ST_NUM_IN_RANGE(9, low, high);
    ST_NUM_NOT_IN_RANGE(50, low, high);

    struct foo {
        int one;
        int two;
    };

    struct foo zeroed = {0};
    struct foo not_zeroed = {0, 1};

    // should succeed (all bytes zero)
    ST_BITWISE_ZEROED(zeroed, sizeof(struct foo));

    // should succeed (any byte not zero)
    ST_BITWISE_NOT_ZEROED(not_zeroed, sizeof(struct foo));

    // should fail (some bytes not zero)
    ST_MESSAGE0("expecting the next 4 to fail");
    ST_BITWISE_ZEROED(not_zeroed, sizeof(struct foo));
    ST_BITWISE_NOT_ZEROED(zeroed, sizeof(struct foo));

    struct foo lhs = {1234, 5678};
    struct foo rhs = {1234, 5678};

    // should succeed
    ST_BITWISE_EQUAL(lhs, rhs, sizeof(struct foo));

    // should fail
    ST_MESSAGE0("expecting the next 1 to fail");
    lhs.one = 4321;
    ST_BITWISE_EQUAL(lhs, rhs, sizeof(struct foo));

    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};

    // should succeed (same type, size, and values)
    ST_ARRAY_EQUAL(arr1, arr2);

    // should succeed
    ST_ARRAY_CONTAINS(arr1, 1);
    ST_ARRAY_CONTAINS(arr1, 3);
    ST_ARRAY_CONTAINS(arr1, 5);

    // should fail (value not present)
    ST_MESSAGE0("expecting the next 1 to fail");
    ST_ARRAY_CONTAINS(arr1, 6);

    // should succeed (different types)
    ST_ARRAY_NOT_CONTAINS(arr1, (char)1);

    // should succeed (value not present)
    ST_ARRAY_NOT_CONTAINS(arr1, 6);

    // should fail (value present)
    ST_MESSAGE0("expecting the next 2 to fail");
    ST_ARRAY_NOT_CONTAINS(arr1, 1);

    // should fail (same type, size, and values)
    ST_ARRAY_NOT_EQUAL(arr1, arr2);

    // should succeed (all unique values)
    ST_ARRAY_UNIQUE(arr1);

    // should fail (duplicate value)
    ST_MESSAGE0("expecting the next 1 to fail");
    arr1[3] = 1;
    ST_ARRAY_UNIQUE(arr1);
    arr1[3] = 4;

    short arr3[] = {1, 2, 3, 4, 5};

    // should succeed (?)
    ST_ARRAY_CONTAINS(arr3, (short)3);

    // should fail (different types, val = int)
    ST_MESSAGE0("expecting the next 2 to fail");
    ST_ARRAY_CONTAINS(arr3, 3);

    // should fail (different types)
    ST_ARRAY_EQUAL(arr1, arr3);

    // should succeed (different types)
    ST_ARRAY_NOT_EQUAL(arr1, arr3);

    arr1[2] = 6;
    // should fail (different values)
    ST_MESSAGE0("expecting the next 1 to fail");
    ST_ARRAY_EQUAL(arr1, arr2);

    // should succeed (different values)
    ST_ARRAY_NOT_EQUAL(arr1, arr2);

    int arr4[] = {1, 2, 3, 4};
    // should fail (different counts)
    ST_MESSAGE0("expecting the next 1 to fail");
    ST_ARRAY_EQUAL(arr2, arr4);

    // should be true
    ST_TRUE(ST_TEST_LAST_EVAL_FALSE());

    // should succeed (different counts)
    ST_ARRAY_NOT_EQUAL(arr2, arr4);

    // should be false
    ST_FALSE(ST_TEST_LAST_EVAL_FALSE());

    // emits an error message with a formatted string from the OS.
    ST_MESSAGE0("testing OS error formatting");
    ST_OS_ERROR_MSG(22, "sysfunc123 failed!");

    // should be true
    int warnings = ST_TEST_WARNING_COUNT();
    ST_TRUE(warnings > 0);

    int errors = ST_TEST_ERROR_COUNT();
    ST_TRUE(errors > 0);

    // causes the test to end immediately
    ST_TEST_EXIT_IF_FAILED();
}
ST_END_TEST_IMPL()

ST_BEGIN_TEST_IMPL(requires_inet)
{
    ST_MESSAGE0("lorem ipsum");
}
ST_END_TEST_IMPL()
