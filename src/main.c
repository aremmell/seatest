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

ST_DECLARE_CL_ARGS();

ST_DECLARE_TEST(test_tests);
ST_DECLARE_TEST(bar);
ST_DECLARE_TEST(baz);

ST_BEGIN_DECLARE_TEST_LIST()
    ST_DECLARE_TEST_LIST_ENTRY(testing-the-tests, test_tests)
    ST_DECLARE_TEST_LIST_ENTRY(jump-thru-hoops, bar)
    ST_DECLARE_TEST_LIST_ENTRY(triple-backflip, baz)
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
    ST_STR_CONTAINS_I("LLO", msg);
    ST_STR_BEGINSWITH("hell", msg, 4);
    ST_STR_BEGINSWITH_I("HELL", msg, 4);
    ST_STR_ENDSWITH("here", msg, 4, strlen(msg));
    ST_STR_ENDSWITH_I("HERE", msg, 4, strlen(msg));

    // should all succeed
    ST_NUM_EVEN(48);
    ST_NUM_ODD(49);
    ST_NUM_NEGATIVE(-4);
    ST_NUM_POSITIVE(4);
    ST_NUM_POWER_2(8192);
    ST_NUM_POWER_10(10000000);

    // should all fail
    ST_NUM_EVEN(49);
    ST_NUM_ODD(48);
    ST_NUM_NEGATIVE(4);
    ST_NUM_POSITIVE(-4);
    ST_NUM_POWER_2(8193);
    ST_NUM_POWER_10(10030000);

    // should fail
    ST_REQUIRE(strlen(msg) == 0);
    ST_EXPECT(0 > 1);

    // should all fail
    ST_STR_CONTAINS("sea", msg);
    ST_STR_CONTAINS_I("tes", msg);
    ST_STR_BEGINSWITH("s", msg, 1);
    ST_STR_BEGINSWITH_I("S", msg, 1);
    ST_STR_ENDSWITH("hell", msg, 4, strlen(msg));
    ST_STR_ENDSWITH_I("HELL", msg, 4, strlen(msg));

    // should succeed (same type, size,
    // and values)
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};
    ST_ARRAY_EQUAL(arr1, arr2);

    if (ST_TEST_PASSING())
        ST_MESSAGE0("arr1 == arr2");

    // should fail (different types)
    short arr3[] = {1, 2, 3, 4, 5};
    ST_ARRAY_EQUAL(arr1, arr3);

    // should fail (different values)
    arr1[2] = 6;
    ST_ARRAY_EQUAL(arr1, arr2);

    // should fail (different counts)
    int arr4[] = {1, 2, 3, 4};
    ST_ARRAY_EQUAL(arr2, arr4);
}
ST_END_TEST_IMPL()

ST_BEGIN_TEST_IMPL(bar)
{
    st_sleep_msec(50);
}
ST_END_TEST_IMPL()

ST_BEGIN_TEST_IMPL(baz)
{
    st_sleep_msec(560);
    int foo = 12345;
    ST_EXPECT(foo < 1200);
}
ST_END_TEST_IMPL()
