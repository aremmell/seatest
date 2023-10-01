/*
 * seatest.c
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

static st_state _state = {0};

int st_main(int argc, char** argv, const char* app_name, const st_cl_arg* args,
    size_t num_args, st_test* tests, size_t num_tests)
{
    if (!st_sanity_check(tests, num_tests)) {
        return EXIT_FAILURE;
    }

    _state.app_name = app_name;

    st_cl_config cl_cfg = {0};
    if (!st_parse_cmd_line(argc, argv, args, num_args, tests,
        num_tests, &cl_cfg))
        return EXIT_FAILURE;

    size_t to_run = cl_cfg.only ? cl_cfg.to_run : num_tests;
    size_t passed = 0;

    st_print_intro(to_run);

    st_timer timer;
    st_timer_begin(&timer);

    for (size_t n = 0; n < to_run; n++) {
        if (cl_cfg.only && !tests[n].run)
            continue;

        double started_at = st_timer_elapsed(&timer);
        st_print_test_intro(n + 1, to_run, tests[n].name);

        tests[n].res = tests[n].fn();
        tests[n].msec = st_timer_elapsed(&timer) - started_at;

        if (tests[n].res.pass || !tests[n].res.fatal)
            passed++;

        st_print_test_outro(n + 1, to_run, tests[n].name, &tests[n]);
    }

    st_print_test_summary(passed, to_run, tests, num_tests, st_timer_elapsed(&timer));

    if (cl_cfg.wait)
        st_wait_for_keypress();

    return passed == to_run ? EXIT_SUCCESS : EXIT_FAILURE;
}

bool st_sanity_check(const st_test* tests, size_t num_tests)
{
    bool all_valid = true;
    for (size_t n = 0; n < num_tests; n++) {
        if (strnstr(tests[n].name, " ", ST_CL_MAX_TEST)) {
            _ST_ERROR("seatest error: test name '%s' is invalid (test names may"
                " not contain spaces)", tests[n].name);
            all_valid = false;
        }
    }
    return all_valid;
}

void st_print_intro(size_t to_run)
{
    (void)printf("\n" WHITEB("running %zu " ULINE("%s") " %s ("),
        to_run, _state.app_name, ST_PLURAL("test", to_run));
    st_print_seatest_ansi(true);
    (void)printf(WHITEB(" %s)...") "\n", st_get_version_string());
}

void st_print_test_intro(size_t num, size_t to_run, const char* name)
{
    (void)printf("\n" WHITEB("(%zu/%zu) '%s' ...") "\n\n", num, to_run, name);
}

void st_print_test_outro(size_t num, size_t to_run, const char* name, const st_test* test)
{
    (void)printf("\n" WHITEB("(%zu/%zu) '%s' finished (") WHITE("%.fms") WHITEB("): ")
        "%s\n", num, to_run, name, test->msec, ST_PASSFAILWARN(test));
}

void st_print_test_summary(size_t passed, size_t to_run, const st_test* tests,
    size_t num_tests, double elapsed)
{
    elapsed = (elapsed / 1e3);
    if (passed == to_run) {
        (void)printf("\n" WHITEB("done: ")
            FG_COLOR(1, 40, "%s%zu " ULINE("%s") " %s " EMPH("passed") " in %.03fsec!") "\n\n",
                to_run > 1 ? "all " : "", to_run, _state.app_name, ST_PLURAL("test", to_run),
                elapsed);
    } else {
        (void)printf("\n" WHITEB("done: ")
            FG_COLOR(1, 196, "%zu of %zu " ULINE("%s") " %s " EMPH("failed") " in %.03fsec") "\n\n",
                to_run - passed, to_run, _state.app_name, ST_PLURAL("test", to_run), elapsed);
    }

    ST_UNUSED(tests);
    ST_UNUSED(num_tests);
}

bool st_mark_test_to_run(const char* const name, st_test* tests, size_t num_tests)
{
    for (size_t n = 0; n < num_tests; n++) {
        if (0 == strncmp(name, tests[n].name, strlen(tests[n].name))) {
            tests[n].run = true;
            return true;
        }
    }
    return false;
}

void st_print_test_list(const st_test* tests, size_t num_tests)
{
    static const size_t tab_size = 4;

    size_t longest = 0;
    for (size_t n = 0; n < num_tests; n++) {
        size_t len = strnlen(tests[n].name, ST_CL_MAX_TEST);
        if (len > longest)
            longest = len;
    }

    (void)printf("\n" WHITE("Available tests:") "\n\n");

    for (size_t n = 0; n < num_tests; n++) {
        (void)printf("\t%s", tests[n].name);

        size_t len = strnlen(tests[n].name, ST_CL_MAX_TEST);
        if (len < longest + tab_size) {
            for (size_t j = len; j < longest + tab_size; j++)
                (void)printf(" ");
        }

        if ((n % 2) != 0 || n == num_tests - 1)
            (void)printf("\n");
    }

    (void)printf("\n");
}

void st_print_usage_info(const st_cl_arg* args, size_t num_args)
{
    size_t longest = 0;
    for (size_t n = 0; n < num_args; n++) {
        size_t len = strnlen(args[n].flag, ST_CL_MAX_FLAG);
        if (len > longest)
            longest = len;
    }

    (void)fprintf(stderr, "\n" WHITE("Usage:") "\n\n");

    for (size_t n = 0; n < num_args; n++) {
        (void)fprintf(stderr, "\t%s ", args[n].flag);

        size_t len = strnlen(args[n].flag, ST_CL_MAX_FLAG);
        if (len < longest) {
            for (size_t j = len; j < longest; j++) {
                (void)fprintf(stderr, " ");
            }
        }

        (void)fprintf(stderr, "%s%s%s\n", args[n].usage,
            strnlen(args[n].usage, ST_CL_MAX_USAGE) > 0 ? " " : "", args[n].desc);
    }

    (void)fprintf(stderr, "\n");
}

void st_print_version_info(void)
{

    (void)printf("\n" ULINE("%s") " test suite " EMPH("(built with") " ",
        _state.app_name);
    st_print_seatest_ansi(false);
    (void)printf(EMPH("%s)")" \n\n", st_get_version_string());

}

void st_print_seatest_ansi(bool bold)
{
    static const struct {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        char c;
    } colors[] = {
        { 42,  64, 199, 's'},
        { 54,  82, 255, 'e'},
        { 65, 113, 255, 'a'},
        { 72, 145, 255, 't'},
        { 74, 171, 255, 'e'},
        { 74, 190, 255, 's'},
        { 68, 211, 241, 't'},
    };

    for (size_t n = 0; n < ST_COUNTOF(colors); n++) {
        (void)printf("\x1b[%d;38;2;%03d;%03d;%03d;49;2m%c",
            bold ? 1 : 0, colors[n].r, colors[n].g, colors[n].b, colors[n].c);
    }
}

const st_cl_arg* st_find_cl_arg(const char* flag, const st_cl_arg* args, size_t num_args)
{
    for (size_t n = 0; n < num_args; n++) {
        if (0 == strncmp(flag, args[n].flag, ST_CL_MAX_FLAG)) {
            return &args[n];
        }
    }
    return NULL;
}

bool st_parse_cmd_line(int argc, char** argv, const st_cl_arg* args, size_t num_args,
    st_test* tests, size_t num_tests, st_cl_config* config)
{
    if (!argv || !args || !tests || !config)
        return false;

    (void)memset(config, 0, sizeof(st_cl_config));

    for (int n = 1; n < argc; n++) {
        const st_cl_arg* this_arg = st_find_cl_arg(argv[n], args, num_args);
        if (!this_arg) {
            _ST_ERROR("unknown option '%s'", argv[n]);
            st_print_usage_info(args, num_args);
            return false;
        }
        if (0 == strncmp(this_arg->flag, ST_CL_WAIT_FLAG, strlen(ST_CL_WAIT_FLAG))) {
            config->wait = true;
        } else if (0 == strncmp(this_arg->flag, ST_CL_ONLY_FLAG, strlen(ST_CL_ONLY_FLAG))) {
            while (++n < argc) {
                if (!argv[n] || !*argv[n])
                    continue;
                if (*argv[n] == '-' || !st_mark_test_to_run(argv[n], tests, num_tests)) {
                    _ST_ERROR("invalid argument to %s: '%s'", ST_CL_ONLY_FLAG, argv[n]);
                    st_print_usage_info(args, num_args);
                    return false;
                }
                config->to_run++;
            }
            if (0 == config->to_run) {
                _ST_ERROR("value expected for '%s'", ST_CL_ONLY_FLAG);
                st_print_usage_info(args, num_args);
                return false;
            }
        } else if (0 == strncmp(this_arg->flag, ST_CL_LIST_FLAG, strlen(ST_CL_LIST_FLAG))) {
            st_print_test_list(tests, num_tests);
            return false;
        } else if (0 == strncmp(this_arg->flag, ST_CL_VERS_FLAG, strlen(ST_CL_VERS_FLAG))) {
            st_print_version_info();
            return false;
        } else if (0 == strncmp(this_arg->flag, ST_CL_HELP_FLAG, strlen(ST_CL_HELP_FLAG))) {
            st_print_usage_info(args, num_args);
            return false;
        } else {
            _ST_ERROR("unknown option '%s'", this_arg->flag);
            return false;
        }
    }
    return true;
}

bool st_getchar(char* input) {
#if defined(__WIN__)
    if (input)
        *input = (char)_getch();
     return true;
#else /* !__WIN__ */
    struct termios cur = {0};
    if (0 != tcgetattr(STDIN_FILENO, &cur))
        return false;

    struct termios new = cur;
    new.c_lflag &= ~(ICANON | ECHO);

    if (0 != tcsetattr(STDIN_FILENO, TCSANOW, &new))
        return false;

    int ch = getchar();

    if (NULL != input)
        *input = (char)ch;

    return 0 == tcsetattr(STDIN_FILENO, TCSANOW, &cur) ? true : false;
#endif
}

void st_wait_for_keypress(void)
{
    (void)printf(WHITEB("Press any key to continue...\n"));
    bool get = st_getchar(NULL);
    ST_UNUSED(get);
}
