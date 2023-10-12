# seatest

<!-- SPDX-License-Identifier: MIT -->
<!-- Copyright (c) 2023 Ryan M. Lederman <lederman@gmail.com> -->

seatest (ˈsiːtɪst)&mdash;a featherweight cross-platform C17 test framework for C libraries and applications.

<img src="https://rml.dev/pub/seatest-logo.png" alt="logo" width="500px" height="auto" />

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=aremmell_seatest&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=aremmell_seatest)
[![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=aremmell_seatest&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=aremmell_seatest)
[![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=aremmell_seatest&metric=reliability_rating)](https://sonarcloud.io/summary/new_code?id=aremmell_seatest)
[![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=aremmell_seatest&metric=security_rating)](https://sonarcloud.io/summary/new_code?id=aremmell_seatest)

Designed with *simplicity*, *completeness*, and *practicality* in mind, perhaps seatest is the right test framework for *your* project.

## Highlights

  * Won't even make a dent in the size of your deployables–the static library weighs in at 16KiB, and the shared library at 52KiB
  * A built-in command-line interface which makes both manual and automated (*i.e. CI*) invocation more flexible and customizable
  * A comprehensive collection of evaluator and helper macros are available in order to facilitate writing expressive, complete, and straightforward tests
  * Provides for special "conditions" that may optionally be used to prevent certain tests from running at all when the condition(s) are not present. For example, if a test absolutely requires an Internet connection in order to pass, that test can be skipped instead of failing.
  * When a test fails, you will know precisely why, as well as where.

## How it works

The public interface to seatest is primarily macro-driven. This is by design, and allows repetitive, easily-forgettable code to be replaced by one-liners. This applies not only to the setup and configuration of the framework itself, but in the implementation of the tests.

If you are anxious to just see an example test rig, there's one one right below. If you would rather take your time and learn about the features and functionality available using the seatest framework, check out the [documentation](https://seatest.rml.dev/).

## Example

See for yourself how simple and straightforward it is to implement a test rig using seatest:

```c
#include <seatest.h>

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
```

## Why
I designed a simplistic version of what is now seatest for [libsir](https://github.com/aremmell/libsir), because I didn't want *any* dependencies, even if it was just a test framework. Later on, I found myself porting that test rig multiple times, and on the third time, I decided the right thing to do is to create something reusable that others can utilize as well.
