/*
 * main.c
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

ST_DECLARE_CL_ARGS()

ST_DECLARE_TEST(test_tests)
ST_DECLARE_TEST(requires_inet)

ST_BEGIN_DECLARE_TEST_LIST()
    ST_DECLARE_TEST_LIST_ENTRY(testing-the-tests, test_tests)
    ST_DECLARE_TEST_LIST_ENTRY_COND(requires-inet, requires_inet, COND_INET)
ST_END_DECLARE_TEST_LIST()

int main(int argc, char** argv)
{
    return ST_MAIN_IMPL("Acme Inc.");
}

ST_BEGIN_TEST_IMPL(test_tests)
{
    st_sleep_msec(200);

    // should succeed
    const char* msg = "hello there";
    ST_REQUIRE(strlen(msg) > 10);
    ST_EXPECT(msg != NULL);

    // should all succeed
    ST_STR_CONTAINS("the", msg);
    ST_STR_NOT_CONTAINS("foo", msg);
    ST_STR_CONTAINS_I("LLO", msg);
    ST_STR_NOT_CONTAINS_I("FOO", msg);
    ST_STR_BEGINSWITH("hell", msg, 4);
    ST_STR_NOT_BEGINSWITH("lleh", msg, 4);
    ST_STR_BEGINSWITH_I("HELL", msg, 4);
    ST_STR_NOT_BEGINSWITH_I("LLEH", msg, 4);
    ST_STR_ENDSWITH("here", msg, 4, strlen(msg));
    ST_STR_NOT_ENDSWITH("ereh", msg, 4, strlen(msg));
    ST_STR_ENDSWITH_I("HERE", msg, 4, strlen(msg));
    ST_STR_NOT_ENDSWITH_I("EREH", msg, 4, strlen(msg));

    // should fail
    /* ST_REQUIRE(strlen(msg) == 0);
    ST_EXPECT(0 > 1); */

    // should all fail
   /* ST_STR_CONTAINS("sea", msg);
    ST_STR_NOT_CONTAINS("hell", msg);
    ST_STR_CONTAINS_I("tes", msg);
    ST_STR_NOT_CONTAINS_I("HELL", msg);
    ST_STR_BEGINSWITH("s", msg, 1);
    ST_STR_NOT_BEGINSWITH("hell", msg, 4);
    ST_STR_BEGINSWITH_I("S", msg, 1);
    ST_STR_NOT_BEGINSWITH_I("HELL", msg, 4);
    ST_STR_ENDSWITH("hell", msg, 4, strlen(msg));
    ST_STR_NOT_ENDSWITH("here", msg, 4, strlen(msg));
    ST_STR_ENDSWITH_I("HELL", msg, 4, strlen(msg));
    ST_STR_NOT_ENDSWITH_I("HERE", msg, 4, strlen(msg));*/

    // should all succeed
    ST_NUM_EVEN(48);
    ST_NUM_ODD(49);
    ST_NUM_NEGATIVE(-4);
    ST_NUM_POSITIVE(4);
    ST_NUM_MULTIPLE_OF(8192, 2);
    ST_NUM_NOT_MULTIPLE_OF(8193, 2);

    // should all fail
    /* ST_NUM_EVEN(49);
    ST_NUM_ODD(48);
    ST_NUM_NEGATIVE(4);
    ST_NUM_POSITIVE(-4);
    ST_NUM_MULTIPLE_OF(8193, 2);
    ST_NUM_MULTIPLE_OF(10000001, 10);
    ST_NUM_NOT_MULTIPLE_OF(8192, 2);
    ST_NUM_NOT_MULTIPLE_OF(10000000, 10); */

    struct foo {
        int one;
        int two;
    };

    struct foo zeroed = {0};
    struct foo not_zeroed = {0, 1};

    // should succeed (all bytes zero)
    ST_BITWISE_ZEROED(&zeroed, sizeof(zeroed));

    // should fail (some bytes not zero)
    //ST_BITWISE_ZEROED(&not_zeroed, sizeof(not_zeroed));

    // should succeed (any byte not zero)
    ST_BITWISE_NOT_ZEROED(&not_zeroed, sizeof(not_zeroed));

    // should fail (all bytes zero)
    //ST_BITWISE_NOT_ZEROED(&zeroed, sizeof(zeroed));

    struct foo lhs = {1234, 5678};
    struct foo rhs = {1234, 5678};

    // should succeed
    ST_BITWISE_EQUAL(lhs, rhs, sizeof(struct foo));

    // should fail
    lhs.one = 4321;
    //ST_BITWISE_EQUAL(lhs, rhs, sizeof(struct foo));

    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};

    // should succeed (same type, size, and values)
    ST_ARRAY_EQUAL(arr1, arr2);

    // should succeed
    ST_ARRAY_CONTAINS(arr1, 1);
    ST_ARRAY_CONTAINS(arr1, 3);
    ST_ARRAY_CONTAINS(arr1, 5);

    // should fail (value not present)
    //ST_ARRAY_CONTAINS(arr1, 6);

    // should succeed (different types)
    ST_ARRAY_NOT_CONTAINS(arr1, (char)1);

    // should succeed (value not present)
    ST_ARRAY_NOT_CONTAINS(arr1, 6);

    // should fail (value present)
    //ST_ARRAY_NOT_CONTAINS(arr1, 1);

    // should fail (same type, size, and values)
    //ST_ARRAY_NOT_EQUAL(arr1, arr2);

    short arr3[] = {1, 2, 3, 4, 5};

    // should succeed (?)
    ST_ARRAY_CONTAINS(arr3, (short)3);

    // should fail (different types, val = int)
    //ST_ARRAY_CONTAINS(arr3, 3);

    // should fail (different types)
    //ST_ARRAY_EQUAL(arr1, arr3);

    // should succeed (different types)
    ST_ARRAY_NOT_EQUAL(arr1, arr3);

    arr1[2] = 6;
    // should fail (different values)
    //ST_ARRAY_EQUAL(arr1, arr2);

    // should pass (different values)
    ST_ARRAY_NOT_EQUAL(arr1, arr2);

    int arr4[] = {1, 2, 3, 4};
    // should fail (different counts)
    //ST_ARRAY_EQUAL(arr2, arr4);

    // should pass (different counts)
    ST_ARRAY_NOT_EQUAL(arr2, arr4);
}
ST_END_TEST_IMPL()

ST_BEGIN_TEST_IMPL(requires_inet)
{
    ST_MESSAGE0("doing something that requires an internet connection...");
}
ST_END_TEST_IMPL()
