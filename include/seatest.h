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

# include "seatest/types.h"
# include "seatest/macros.h"
# include "seatest/timing.h"
# include "seatest/version.h"

int st_main(int argc, char** argv, const char* app_name, const st_cl_arg* args,
    size_t num_args, st_testinfo* tests, size_t num_tests);

void st_print_intro(const char* name, size_t to_run);
void st_print_test_intro(size_t num, size_t to_run, const char* name);
void st_print_test_outro(size_t num, size_t to_run, const char* name, st_testres* res);
void st_print_test_summary(size_t passed, size_t to_run, st_testinfo* tests,
    size_t num_tests, double elapsed, const char* app_name);

/** Marks a test to be executed during the current run. Returns false if unable to
 * locate the specified test. */
bool st_mark_test_to_run(const char* const name, st_testinfo* tests, size_t num_tests);

/** Prints the entire list of available tests. */
void st_print_test_list(const st_testinfo* tests, size_t num_tests);

/** Prints usage information. */
void st_print_usage_info(const st_cl_arg* args, size_t num_args);

/** Looks up a command line argument by flag. Returns NULL if no match was found. */
const st_cl_arg* st_find_cl_arg(const char* flag, const st_cl_arg* args, size_t num_args);

/**
 * Parses command line arguments given and calls the appropriate function(s) and/or
 * returns a configuration to be used by the test rig. Returns false if the test rig
 * should immediately exit with EXIT_FAILURE.
 */
bool st_parse_cmd_line(int argc, char** argv, const char* app_name, const st_cl_arg* args,
    size_t num_args, st_testinfo* tests, size_t num_tests, st_cl_config* config);

bool st_getchar(char* input);
void st_wait_for_keypress(void);

#endif /* !_SEATEST_H_INCLUDED */
