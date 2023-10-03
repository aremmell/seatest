/*
 * seatest.h
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
#ifndef _SEATEST_H_INCLUDED
# define _SEATEST_H_INCLUDED

# include "seatest/console.h"
# include "seatest/config.h"
# include "seatest/types.h"
# include "seatest/helpers.h"
# include "seatest/macros.h"
# include "seatest/version.h"

int st_main(int argc, char** argv, const char* app_name, const st_cl_arg* args,
    size_t num_args, st_test* tests, size_t num_tests);

bool st_validate_config(const char* app_name, const st_test* tests, size_t num_tests);
bool st_prepare_tests(st_test* tests, size_t num_tests);

void st_print_intro(size_t to_run);
void st_print_test_intro(size_t num, size_t to_run, const char* name);
void st_print_test_outro(size_t num, size_t to_run, const char* name, const st_test* test);
void st_print_test_summary(size_t passed, size_t to_run, const st_test* tests,
    size_t num_tests, double elapsed);
void st_print_failed_test_intro(size_t passed, size_t to_run);
void st_print_failed_test(const char* const name);

/** Marks a test to be executed during the current run. Returns false if unable to
 * locate the specified test. */
bool st_mark_test_to_run(const char* const name, st_test* tests, size_t num_tests);

/** Prints the entire list of available tests. */
void st_print_test_list(const st_test* tests, size_t num_tests);

/** Prints usage information. */
void st_print_usage_info(const st_cl_arg* args, size_t num_args);

/** Prints version information. */
void st_print_version_info(void);

/** Prints 'seatests' in pretty colors. */
void st_print_seatest_ansi(bool bold);

/** Looks up a command line argument by flag. Returns NULL if no match was found. */
const st_cl_arg* st_find_cl_arg(const char* flag, const st_cl_arg* args, size_t num_args);

/**
 * Parses command line arguments given and calls the appropriate function(s) and/or
 * returns a configuration to be used by the test rig. Returns false if the test rig
 * should immediately exit with EXIT_FAILURE.
 */
bool st_parse_cmd_line(int argc, char** argv, const st_cl_arg* args, size_t num_args,
    st_test* tests, size_t num_tests, st_cl_config* config);

bool st_getchar(char* input);
void st_wait_for_keypress(void);

void st_timer_begin(st_timer* timer);
double st_timer_elapsed(const st_timer* timer);
long st_timer_getres(void);

bool st_clock_gettime(int clock, time_t* tbuf, long* msecbuf);
double st_msec_since(const st_timer* when, st_timer* out);

/** Retrieves a formatted error message for the specified code. */
char* st_format_error_msg(int code, char message[ST_MAX_ERROR]);

/** Puts the current thread to sleep for `msec` milliseconds. */
void st_sleep_msec(uint32_t msec);

/** Returns the current working directory. */
char* st_getcwd(void);

/** Retrieves the number of available bytes on the filesystem associated with `path`. */
bool st_get_avail_fs_bytes(const char* restrict path, uint64_t* bytes);

/** Determines if an Internet connection is available. */
bool st_have_inet_connection(void);

#endif /* !_SEATEST_H_INCLUDED */
