/*
 * example.c
 *
 * A bare-bones example implementation of a seatest test rig. Can be found at
 * build/seatest_example after running CMake.
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
#include <seatest.h>
#include "pseudocode.h"

/** Declares static variables required by seatest. Must appear before the
 * entry point. */
ST_DECLARE_STATIC_VARS()

/** Individual test declarations. */
ST_DECLARE_TEST(widget_sanity)
ST_DECLARE_TEST(rest_client_get)

/** The global list of tests available. For each entry, a corresponding
 * ST_DECLARE_TEST() must appear in an included header, or above this statement
 * in the same file. */
ST_BEGIN_DECLARE_TEST_LIST()
    /** Ensures that widgets are functioning properly. */
    ST_DECLARE_TEST_LIST_ENTRY(widget-sanity, widget_sanity)
    /** Checks a REST client's get request functionality (requires an Internet
     * connection); if not connected to the Internet, the test will be skipped. */
    ST_DECLARE_TEST_LIST_ENTRY_COND(rest-client-get, rest_client_get, COND_INET)
ST_END_DECLARE_TEST_LIST()

/** Implementing the entry point couldn't really be any easier! All data required
 * for the test rig to function is captured automatically. */
int main(int argc, char** argv)
{
    return ST_MAIN_IMPL("Acme Inc.");
}

/** Implementation of the widget-sanity test. */
ST_BEGIN_TEST_IMPL(widget_sanity)
    /** An informative message, to show that something is happening. */
    ST_MESSAGE("creating a %s widget...", "purple");

    /** Create a widget and validate its properties. */
    acme_widget widget;
    bool created = acme_create_widget_purple(&widget);

    /** Emits a message printed in green, to show that something postive happened. */
    if (created) {
        ST_SUCCESS0("successfully created widget");
    }

    /** Emits an orange message, and adds a warning to the test. ST_REQUIRE()'s
     * little brother. */
    ST_EXPECT(widget.class != 109);

    /** All of the following emit red messages and cause the test to fail if the
     * expression passed to them evaluates to false. */
    ST_REQUIRE(created);
    ST_NUM_POSITIVE(widget.color);
    ST_GREATER_THAN_OR_EQUAL(widget.class, 100);
    ST_BITS_HIGH(widget.flags, 4);

    /** If it does not make sense to continue the test upon the failure of any
     * of these conditions, the following causes the test to stop and return
     * an error. */
    ST_TEST_EXIT_IF_FAILED();

    /** ... the rest of the test. */

ST_END_TEST_IMPL()

/** Implementation of the rest-client-get test. */
ST_BEGIN_TEST_IMPL(rest_client_get)
ST_END_TEST_IMPL()
