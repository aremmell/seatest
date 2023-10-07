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
#include "phony.h"

/** Declares necessary static variables. Must appear before the entry point. */
ST_DECLARE_STATIC_VARS()

/** Test routine declarations. */
ST_DECLARE_TEST(widget_sanity)
ST_DECLARE_TEST(rest_client_get)

/** The global list of available tests (and associated metadata). For each entry,
 * a corresponding ST_DECLARE_TEST() must appear in an included header or above
 * this statement in the same file. */
ST_BEGIN_DECLARE_TEST_LIST()

    /** Ensures that widgets are functioning properly. */
    ST_DECLARE_TEST_LIST_ENTRY(widget-sanity, widget_sanity)

    /** Checks a REST client's get request functionality (requires an Internet
     * connection); if not connected to the Internet, the test will be skipped. */
    ST_DECLARE_TEST_LIST_ENTRY_COND(rest-client-get, rest_client_get, COND_INET)

ST_END_DECLARE_TEST_LIST()

/** The entry point is simple as can be: all data required by the test rig is
 * captured automatically (must have argc and argv parameters and return int). */
int main(int argc, char** argv)
{
    return ST_MAIN_IMPL("Acme Inc.");
}

/** Implementation of the widget-sanity test. */
ST_BEGIN_TEST_IMPL(widget_sanity)
{
    /** An informative message, to show that something is happening. */
    ST_MESSAGE("creating a %s widget...", "purple");

    /** Create a widget and validate its properties. */
    acme_widget widget;
    bool created = acme_create_widget_purple(&widget);

    /** Emits a message printed in green, to show that something postive happened. */
    if (created) {
        ST_SUCCESS0("successfully created widget");
    }

    /** The following emit red messages and cause the test to fail if the
     * expression they represent evaluates to false. For a complete list of
     * available Evaluators, see the documentation. */
    ST_TRUE(created);
    ST_NUM_POSITIVE(widget.color);
    ST_EQUAL(widget.class, WIDGET_CLASS_STANDARD);
    ST_BITS_HIGH(widget.flags, WIDGET_FLAGS_STANDARD);

    /** If it doesn't make sense to continue the test after the failure of any
     * of the above, causes the test to exit immediately with an error. */
    ST_TEST_EXIT_IF_FAILED();

    /** ... the rest of the test. */
}
ST_END_TEST_IMPL()

/** Implementation of the rest-client-get test. To see how the test rig behaves
 * when no Internet connection is available (this test depends on it), uncomment
 * the line in CMakeLists.txt that says ST_SIMULATE_INET_ERROR. */
ST_BEGIN_TEST_IMPL(rest_client_get)
{
    /** Perform an HTTP GET request, retrieving a list of available widgets.  */
    ST_MESSAGE0("retrieving list of widgets from server...");

    http_response response;
    bool result = acme_rest_get_widget_list(&response);

    /** As in the widget-sanity test, the following emit a red message and cause
     * the test to fail if the expression they represent evaluates to false. */
    ST_TRUE(result);
    ST_EQUAL(response.code, HTTP_OK);
    ST_STR_NOT_EMPTY(response.body);
    ST_STR_BEGINSWITH("{\"widgets\":", response.body, 11);

    /** If any of the Evaluators above have failed, exit the test now. */
    ST_TEST_EXIT_IF_FAILED();

    /** Visual confirmation that the network request succeeded. */
    ST_SUCCESS("got HTTP_OK; response len: %zu bytes", strlen(response.body));

    /** ... the rest of the test. */
}
ST_END_TEST_IMPL()
