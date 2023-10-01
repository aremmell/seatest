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

ST_DECLARE_TEST(foo);

int main(int argc, char** argv)
{
    ST_UNUSED(argc);
    ST_UNUSED(argv);

    st_testres res = st_test_foo();
    ST_UNUSED(res);

    return EXIT_SUCCESS;
}

ST_BEGIN_TEST_IMPL(foo)
{
    int foo = 12345;
    ST_EXPECT(foo < 1200);

    const char* msg = "hello there";
    ST_REQUIRE(strlen(msg) > 15);

    const char* msg2 = "hello there, sir";
    ST_STREQUAL(msg, msg2);
    ST_STREQUAL("sups?", "sups?!");

    ST_STRIEQUAL("SUPS", "sUpS");
}
ST_END_TEST_IMPL()
