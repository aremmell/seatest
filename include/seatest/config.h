/*
 * config.h
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
#ifndef _SEATEST_CONFIG_H_INCLUDED
# define _SEATEST_CONFIG_H_INCLUDED

/** The maximum size, in characters, of an error message produced by the system. */
# define ST_MAX_ERROR 256

/** The maximum size, in characters, of a test's human-readable name. */
# define ST_MAX_TEST_NAME 32

/** The maximum size, in characters, of buffers containing condition bitmasks
 * in string format. */
# define ST_MAX_COND_STR 256

/** The minimum amount, in bytes, of available disk space for COND_DISK. Default
 * value: 500 MiB. */
# define ST_MIN_FS_AVAIL (500 * 1024 * 1024)

/** The TLD to use as the target when testing for an Internet connection. */
# define ST_INET_TARGET_HOST "example.com"

/** The service name/port number to use when testing for an Internet connection. */
# define ST_INET_TARGET_SRV "http"

/** The number of seconds to wait for a TCP connection before it times out. */
# define ST_INET_TIMEOUT 5

/**
 * i18n
 */

# define ST_LOC_SEATEST       "seatest"
# define ST_LOC_ERROR         "error"
# define ST_LOC_WARNING       "warning"
# define ST_LOC_DEBUG         "debug"
# define ST_LOC_IN            "in"
# define ST_LOC_OF            "of"
# define ST_LOC_LINE          "line"
# define ST_LOC_TEST          "test"
# define ST_LOC_NAME          "name"
# define ST_LOC_RUNNING       "running"
# define ST_LOC_FINISHED      "finished"
# define ST_LOC_TEST_SUITE    "test suite"
# define ST_LOC_BUILT_WITH    "built with"
# define ST_LOC_FAILED        "Failed"
# define ST_LOC_DONE          "done"
# define ST_LOC_PASSED        "passed"
# define ST_LOC_FAILED_L      "failed"
# define ST_LOC_MSEC_ABV      "ms"
# define ST_LOC_MSEC_ZERO     "\xe2\x89\xa41" /* ≤1 */
# define ST_LOC_SEC_ABV       "s"
# define ST_LOC_USAGE         "Usage"
# define ST_LOC_AVAIL_TESTS   "Available tests"
# define ST_LOC_UNK_OPT       "unknown option"
# define ST_LOC_VAL_EXPECT    "value expected for"
# define ST_LOC_INVAL_ARG     "invalid argument to"
# define ST_LOC_PRESS_KEY     "Press any key to continue..."
# define ST_LOC_EXPRESSION    "expression"
# define ST_LOC_IS_FALSE      "is false"
# define ST_LOC_SKIPPED_COND  "will be skipped due to"
# define ST_LOC_CONDITION     "condition"
# define ST_LOC_SKIPPED_UNMET "skipped due to unmet %s"
# define ST_LOC_INVAL_ANAME   "the app_name parameter to ST_MAIN_IMPL is" \
                              " invalid (may not be null or an empty string)"
# define ST_LOC_NO_SPACES     "is invalid (names may not contain spaces)"
# define ST_LOC_TOO_LONG      "is invalid (names may only be %d characters" \
                              " in length; counted %zu)"
# define ST_LOC_RECTIFY       "please rectify the above %s and recompile"
# define ST_LOC_CALC_DISK_ERR "failed to calculate available disk space! tests" \
                              " requiring COND_DISK will be skipped"
# define ST_LOC_DISK_SPACE    "available disk space (%"PRIu64" bytes) is less" \
                              " than the required %"PRIu64"; tests requiring" \
                              " COND_DISK will be skipped"
# define ST_LOC_NO_INTERNET   "no internet connection detected; tests requiring" \
                              " COND_INET will be skipped"
# define ST_LOC_FAIL_EARLY    "failed; exiting with code %d due to %s..."
# define ST_LOC_SKIP          "SKIP"
# define ST_LOC_PASS          "PASS"
# define ST_LOC_WARN          "WARN"
# define ST_LOC_FAIL          "FAIL"
# define ST_LOC_INDENT        "  "

# define ST_LOC_WAIT_FLAG     "--wait"
# define ST_LOC_WAIT_FLAG_S   "-w"
# define ST_LOC_ONLY_FLAG     "--only"
# define ST_LOC_ONLY_FLAG_S   "-o"
# define ST_LOC_LIST_FLAG     "--list"
# define ST_LOC_LIST_FLAG_S   "-l"
# define ST_LOC_FAIL_FLAG     "--fail-early"
# define ST_LOC_FAIL_FLAG_S   "-f"
# define ST_LOC_VERS_FLAG     "--version"
# define ST_LOC_VERS_FLAG_S   "-v"
# define ST_LOC_HELP_FLAG     "--help"
# define ST_LOC_HELP_FLAG_S   "-h"

# define ST_LOC_ONLY_USAGE    ULINE("name") " [, " ULINE("name") ", ...]"

# define ST_LOC_WAIT_DESC     "Wait for a key press before exiting"
# define ST_LOC_ONLY_DESC     "Run only the test(s) specified"
# define ST_LOC_LIST_DESC     "Print a list of all available tests"
# define ST_LOC_FAIL_DESC     "Exit immediately upon failure of any test"
# define ST_LOC_VERS_DESC     "Display version information"
# define ST_LOC_HELP_DESC     "Display this message"

/** Command line configuration (flag, usage, description). If entries are changed,
 * st_parse_cmd_line and st_print_usage_info must be modified as well. */
# define ST_CL_CONFIG() \
    {ST_LOC_WAIT_FLAG_S, ST_LOC_WAIT_FLAG, "",                ST_LOC_WAIT_DESC}, \
    {ST_LOC_ONLY_FLAG_S, ST_LOC_ONLY_FLAG, ST_LOC_ONLY_USAGE, ST_LOC_ONLY_DESC}, \
    {ST_LOC_LIST_FLAG_S, ST_LOC_LIST_FLAG, "",                ST_LOC_LIST_DESC}, \
    {ST_LOC_FAIL_FLAG_S, ST_LOC_FAIL_FLAG, "",                ST_LOC_FAIL_DESC}, \
    {ST_LOC_VERS_FLAG_S, ST_LOC_VERS_FLAG, "",                ST_LOC_VERS_DESC}, \
    {ST_LOC_HELP_FLAG_S, ST_LOC_HELP_FLAG, "",                ST_LOC_HELP_DESC}

#endif /* !_SEATEST_CONFIG_H_INCLUDED */
