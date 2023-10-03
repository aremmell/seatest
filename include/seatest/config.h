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

/** The string to use in place of "0.0" if an elapsed time is too fast to measure. */
# define ST_MSEC_ZERO "<1"

/**
 * Command line argument thresholds
 */

# define ST_CL_MAX_FLAG  32
# define ST_CL_MAX_USAGE 96

/**
 * Command line argument flags
 */

# define ST_CL_WAIT_FLAG "--wait"
# define ST_CL_ONLY_FLAG "--only"
# define ST_CL_LIST_FLAG "--list"
# define ST_CL_VERS_FLAG "--version"
# define ST_CL_HELP_FLAG "--help"

/**
 * Command line argument usage messages
 */

# define ST_CL_ONLY_USAGE ULINE("name") " [, " ULINE("name") ", ...]"

/**
 * Command line argument descriptions
 */

# define ST_CL_WAIT_DESC "After running all test(s), wait for a keypress before exiting"
# define ST_CL_ONLY_DESC "Run only the test(s) specified"
# define ST_CL_LIST_DESC "Print a list of all available tests"
# define ST_CL_VERS_DESC "Displays version information"
# define ST_CL_HELP_DESC "Displays this message"

/** Command line configuration (flag, usage, description). If entries are changed,
 * st_parse_cmd_line and st_print_usage_info must be modified as well. */
# define ST_CL_CONFIG() \
    {ST_CL_WAIT_FLAG, "",               ST_CL_WAIT_DESC}, \
    {ST_CL_ONLY_FLAG, ST_CL_ONLY_USAGE, ST_CL_ONLY_DESC}, \
    {ST_CL_LIST_FLAG, "",               ST_CL_LIST_DESC}, \
    {ST_CL_VERS_FLAG, "",               ST_CL_VERS_DESC}, \
    {ST_CL_HELP_FLAG, "",               ST_CL_HELP_DESC}

#endif /* !_SEATEST_CONFIG_H_INCLUDED */
