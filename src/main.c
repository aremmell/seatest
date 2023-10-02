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

ST_DECLARE_TEST(foo);
ST_DECLARE_TEST(bar);
ST_DECLARE_TEST(baz);

ST_BEGIN_DECLARE_TEST_LIST()
    ST_DECLARE_TEST_INFO(do-headstand, foo)
    ST_DECLARE_TEST_INFO(jump-thru-hoops, bar)
    ST_DECLARE_TEST_INFO(triple-backflip, baz)
ST_END_DECLARE_TEST_LIST()

int main(int argc, char** argv)
{
    return ST_MAIN_IMPL("Acme Inc.");
}

ST_BEGIN_TEST_IMPL(foo)
{
    st_sleep_msec(200);

    const char* msg = "hello there";
    ST_REQUIRE(strlen(msg) > 10);

    //ST_STRCONTAINS("the", msg, strlen(msg));
    ST_STRBEGINSWITH("hell", msg, 4);
    //ST_STRENDSWITH("halp", msg, 4, strlen(msg));
    //ST_STRCONTAINS_I("llo", msg);
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
