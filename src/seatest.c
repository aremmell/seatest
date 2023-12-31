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

#if defined(__WIN__)
# pragma comment(lib, "Shlwapi.lib")
# pragma comment(lib, "Ws2_32.lib")
#endif

static st_state _state = {0};

int st_main(int argc, char** argv, const char* app_name, const st_cl_arg* args,
    size_t num_args, st_test* tests, size_t num_tests)
{
    if (!st_validate_config(app_name, tests, num_tests)) {
        return EXIT_FAILURE;
    }

    _state.app_name = app_name;

    st_cl_config cl_cfg = {0};
    if (!st_parse_cmd_line(argc, argv, args, num_args, tests, num_tests, &cl_cfg) ||
        !st_prepare_tests(tests, num_tests)) {
        return EXIT_FAILURE;
    }

    size_t to_run = cl_cfg.only ? cl_cfg.to_run : num_tests;
    size_t passed = 0;

    st_print_intro(to_run);

    st_timer timer;
    st_timer_begin(&timer);

    for (size_t n = 0; n < to_run; n++) {
        if (cl_cfg.only && !tests[n].run) {
            continue;
        }
        double started_at = st_timer_elapsed(&timer);
        st_print_test_intro(n + 1, to_run, tests[n].name);

        if (!tests[n].res.skip) {
            tests[n].res = tests[n].fn();
        } else {
            char conds[ST_MAX_COND_STR] = {0};
            _ST_SKIPPED(ST_LOC_INDENT ST_LOC_SKIPPED_UNMET": %s",
                _ST_PLURAL(ST_LOC_CONDITION, _st_conds_count(tests[n].res.skip_conds)),
                _st_conds_to_string(tests[n].res.skip_conds, conds));
        }

        tests[n].msec = st_timer_elapsed(&timer) - started_at;
        st_print_test_outro(n + 1, to_run, tests[n].name, &tests[n]);

        if (tests[n].res.pass || !tests[n].res.fatal || tests[n].res.skip) {
            passed++;
        } else if (_state.fail_early) {
            _ST_WARNING("%s '%s' "ST_LOC_FAIL_EARLY, _ST_WARN_PREFIX, tests[n].name,
                EXIT_FAILURE, ST_LOC_FAIL_FLAG);
            return EXIT_FAILURE;
        }
    }

    st_print_test_summary(passed, to_run, tests, num_tests, st_timer_elapsed(&timer));

    if (cl_cfg.wait) {
        st_wait_for_keypress();
    }

    return passed == to_run ? EXIT_SUCCESS : EXIT_FAILURE;
}

bool st_validate_config(const char* app_name, const st_test* tests, size_t num_tests)
{
    /* before doing any other work, ensure that the test configuration is sane. */
    int errors = 0;

    /* any debug information related to the configuration/system. */
    _ST_DEBUG("clock resolution: ~%ldns", st_timer_getres());

    /* app name. */
    if (!app_name || !*app_name) {
        _ST_ERROR("%s "ST_LOC_INVAL_ANAME, _ST_ERROR_PREFIX);
        errors++;
    }

    for (size_t n = 0; n < num_tests; n++) {
        /* spaces in test names. */
        if (st_strstr(tests[n].name, " ")) {
            _ST_ERROR("%s "ST_LOC_TEST" #%zu ("ST_LOC_NAME": '%s') "
                ST_LOC_NO_SPACES, _ST_ERROR_PREFIX, n + 1, tests[n].name);
            errors++;
        }
        /* length of test names. */
        size_t name_len = strlen(tests[n].name);
        if (name_len > ST_MAX_TEST_NAME) {
            _ST_ERROR("%s "ST_LOC_TEST" #%zu ("ST_LOC_NAME": '%s') "ST_LOC_TOO_LONG,
                _ST_ERROR_PREFIX, n + 1, tests[n].name, ST_MAX_TEST_NAME, name_len);
            errors++;
        }
    }

    if (errors > 0) {
        _ST_ERROR("%s "ST_LOC_RECTIFY, _ST_ERROR_PREFIX, _ST_PLURAL(ST_LOC_ERROR, errors));
    }

    return errors == 0;
}

bool st_prepare_tests(st_test* tests, size_t num_tests)
{
    uint64_t fs_avail = 0;
    char* cwd = st_getcwd();
    bool cond_disk = st_get_avail_fs_bytes(cwd, &fs_avail);
    _st_safefree(&cwd);

#if defined(ST_SIMULATE_FS_ERROR)
    cond_disk = false;
    fs_avail = 0;
#elif defined (ST_SIMULATE_FS_INSUFFICIENT)
    cond_disk = true;
    fs_avail = ST_MIN_FS_AVAIL - 1;
#endif

    if (!cond_disk) {
        _ST_ERROR("%s "ST_LOC_CALC_DISK_ERR, _ST_ERROR_PREFIX);
    } else if (fs_avail < ST_MIN_FS_AVAIL) {
        _ST_WARNING("%s "ST_LOC_DISK_SPACE, _ST_WARN_PREFIX, fs_avail,
            (uint64_t)ST_MIN_FS_AVAIL);
    }

    bool cond_inet = st_have_inet_connection();

#if defined(ST_SIMULATE_INET_ERROR)
    cond_inet = false;
#endif

    if (!cond_inet) {
        _ST_WARNING("%s "ST_LOC_NO_INTERNET, _ST_WARN_PREFIX);
    }

    for (size_t n = 0; n < num_tests; n++) {
        _ST_DEBUG("test #%zu (name: '%s') has conds %08x", n + 1, tests[n].name,
            tests[n].conds);
        if (tests[n].conds != 0) {
            bool skip = false;
            _ST_PROCESS_TEST_CONDITION(cond_disk, COND_DISK);
            _ST_PROCESS_TEST_CONDITION(cond_inet, COND_INET);
            tests[n].res.skip = skip;
            if (!skip) {
                _ST_DEBUG("test #%zu (name: '%s') will not be skipped; all conditions met",
                    n + 1, tests[n].name);
            }
        }
    }

    return true;
}

void st_print_intro(size_t to_run)
{
    (void)printf("\n" WHITEB(ST_LOC_RUNNING " %zu " ULINE("%s") " %s " DGRAY("(")),
        to_run, _state.app_name, _ST_PLURAL(ST_LOC_TEST, to_run));
    st_print_seatest_ansi(true);
    (void)printf(DGRAY(" %s)...") "\n", st_get_version_string());
}

void st_print_test_intro(size_t num, size_t to_run, const char* name)
{
    (void)printf("\n" WHITEB("(%zu/%zu) '%s' ...") "\n\n", num, to_run, name);
}

void st_print_test_outro(size_t num, size_t to_run, const char* name, const st_test* test)
{
    char* msec_str = NULL;
    if (test->msec > 1.0 + DBL_EPSILON) {
        int len = snprintf(NULL, 0, "%.f", test->msec);
        if (len > 0) {
            msec_str = calloc(len + 1, sizeof(char));
            if (msec_str) {
                (void)snprintf(msec_str, len + 1, "%.f", test->msec);
            }
        }
    }

   char* warn_str = NULL;
    if (test->res.warnings > 0) {
        int len = snprintf(NULL, 0, " (%d %s%s", test->res.warnings,
            _ST_PLURAL(ST_LOC_WARNING, test->res.warnings),
            (test->res.errors > 0 ? "" : ")"));
        if (len > 0) {
            warn_str = calloc(len + 1, sizeof(char));
            if (warn_str) {
                (void)snprintf(warn_str, len + 1, " (%d %s%s", test->res.warnings,
                    _ST_PLURAL(ST_LOC_WARNING, test->res.warnings),
                    (test->res.errors > 0 ? "" : ")"));
            }
        }
    }

    char* err_str = NULL;
    if (test->res.errors > 0) {
        int len = snprintf(NULL, 0, "%s%d %s)", (test->res.warnings > 0 ? ", " : " ("),
            test->res.errors, _ST_PLURAL(ST_LOC_ERROR, test->res.errors));
        if (len > 0) {
            err_str = calloc(len + 1, sizeof(char));
            if (err_str) {
                (void)snprintf(err_str, len + 1, "%s%d %s)",
                    (test->res.warnings > 0 ? ", " : " ("), test->res.errors,
                    _ST_PLURAL(ST_LOC_ERROR, test->res.errors));
            }
        }
    }

    (void)printf("\n" WHITEB("(%zu/%zu) '%s' "ST_LOC_FINISHED)
        WHITE(" [%s"ST_LOC_MSEC_ABV"]") WHITEB(":") " %s", num, to_run, name,
        (msec_str ? msec_str : ST_LOC_MSEC_ZERO), _ST_SKIP_PASS_FAIL(test));

    (void)printf(test->res.errors > 0 ?
        FG_COLOR(0, 196, "%s%s\n") : FG_COLOR(0, 208, "%s%s\n"),
        (warn_str ? warn_str : ""), (err_str ? err_str : ""));

    _st_safefree(&msec_str);
    _st_safefree(&warn_str);
    _st_safefree(&err_str);
}

void st_print_test_summary(size_t passed, size_t to_run, const st_test* tests,
    size_t num_tests, double elapsed)
{
    elapsed = (elapsed / 1e3);
    if (passed == to_run) {
        (void)printf("\n" WHITEB(ST_LOC_DONE": ")
            FG_COLOR(1, 40, "%s%zu " ULINE("%s") " %s " EMPH(ST_LOC_PASSED) " "ST_LOC_IN
            " %.03f"ST_LOC_SEC_ABV"!") "\n\n", to_run > 1 ? "all " : "", to_run,
            _state.app_name, _ST_PLURAL(ST_LOC_TEST, to_run), elapsed);
    } else {
        (void)printf("\n" WHITEB("done: ")
            FG_COLOR(1, 196, "%zu "ST_LOC_OF" %zu " ULINE("%s") " %s " EMPH(ST_LOC_FAILED_L)
            " "ST_LOC_IN" %.03f"ST_LOC_SEC_ABV) "\n\n", to_run - passed, to_run,
            _state.app_name, _ST_PLURAL(ST_LOC_TEST, to_run), elapsed);
    }

    if (passed != to_run) {
        st_print_failed_test_intro(passed, to_run);
        for (size_t t = 0; t < num_tests; t++) {
            if (!tests[t].res.pass && tests[t].res.fatal) {
                st_print_failed_test(tests[t].name);
            }
        }
        (void)printf("\n");
    }
}

void st_print_failed_test_intro(size_t passed, size_t to_run)
{
    _ST_ERROR(ST_LOC_FAILED" %s:\n", _ST_PLURAL(ST_LOC_TEST, to_run - passed));
}

void st_print_failed_test(const char* const name)
{
    _ST_ERROR("\t  " ST_BULLET " %s", name);
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
        size_t len = strnlen(tests[n].name, ST_MAX_TEST_NAME);
        if (len > longest)
            longest = len;
    }

    (void)printf("\n" WHITE(ST_LOC_AVAIL_TESTS":") "\n\n");

    for (size_t n = 0; n < num_tests; n++) {
        (void)printf("\t%s", tests[n].name);

        size_t len = strnlen(tests[n].name, ST_MAX_TEST_NAME);
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
        size_t len = strlen(args[n].flag) + strlen(args[n].s_flag);
        if (len > longest) {
            longest = len;
        }
    }

    (void)fprintf(stderr, "\n" WHITE(ST_LOC_USAGE":") "\n\n");

    for (size_t n = 0; n < num_args; n++) {
        (void)fprintf(stderr, "\t%s, %s  ", args[n].s_flag, args[n].flag);

        size_t len = strlen(args[n].flag) + strlen(args[n].s_flag);
        if (len < longest) {
            for (size_t j = len; j < longest; j++) {
                (void)fprintf(stderr, " ");
            }
        }

        (void)fprintf(stderr, "%s%s%s\n", args[n].usage,
            strlen(args[n].usage) > 0 ? " " : "", args[n].desc);
    }

    (void)fprintf(stderr, "\n");
}

void st_print_version_info(void)
{
    (void)printf("\n" ULINE("%s") " "ST_LOC_TEST_SUITE" " EMPH("("ST_LOC_BUILT_WITH)
        " ", _state.app_name);
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
    } elements[] = {
        { 65, 105, 238, 's'},
        { 63, 125, 238, 'e'},
        { 71, 145, 237, 'a'},
        { 74, 165, 236, 't'},
        { 77, 185, 235, 'e'},
        { 80, 205, 234, 's'},
        { 83, 225, 233, 't'},
    };

    for (size_t n = 0; n <_ST_COUNTOF(elements); n++) {
        (void)printf("\x1b[%d;38;2;%03d;%03d;%03d;49;2m%c\x1b[0m", bold ? 1 : 0,
            elements[n].r, elements[n].g, elements[n].b, elements[n].c);
    }
}

bool st_is_cl_arg(const st_cl_arg* arg, const char* flag)
{
    return 0 == st_strncmp(flag, arg->flag, strlen(arg->flag)) ||
           0 == st_strncmp(flag, arg->s_flag, strlen(arg->s_flag));
}

const st_cl_arg* st_find_cl_arg(const char* flag, const st_cl_arg* args, size_t num_args)
{
    for (size_t n = 0; n < num_args; n++) {
        if (st_is_cl_arg(&args[n], flag)) {
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
        const st_cl_arg* cur = st_find_cl_arg(argv[n], args, num_args);
        if (!cur) {
            _ST_ERROR(ST_LOC_UNK_OPT" '%s'", argv[n]);
            st_print_usage_info(args, num_args);
            return false;
        }
        if (st_is_cl_arg(cur, ST_LOC_WAIT_FLAG)) {
            config->wait = true;
        } else if (st_is_cl_arg(cur, ST_LOC_ONLY_FLAG)) {
            while (++n < argc) {
                if (!argv[n] || !*argv[n])
                    continue;
                if (*argv[n] == '-' || !st_mark_test_to_run(argv[n], tests, num_tests)) {
                    _ST_ERROR(ST_LOC_INVAL_ARG" %s: '%s'", ST_LOC_ONLY_FLAG, argv[n]);
                    st_print_usage_info(args, num_args);
                    return false;
                }
                config->to_run++;
            }
            if (0 == config->to_run) {
                _ST_ERROR(ST_LOC_VAL_EXPECT" '%s'", ST_LOC_ONLY_FLAG);
                st_print_usage_info(args, num_args);
                return false;
            }
        } else if (st_is_cl_arg(cur, ST_LOC_LIST_FLAG)) {
            st_print_test_list(tests, num_tests);
            return false;
        } else if (st_is_cl_arg(cur, ST_LOC_FAIL_FLAG)) {
            _ST_DEBUG("will exit immediately upon any falied test due to %s",
                ST_LOC_FAIL_FLAG);
            _state.fail_early = true;
        } else if (st_is_cl_arg(cur, ST_LOC_VERS_FLAG)) {
            st_print_version_info();
            return false;
        } else if (st_is_cl_arg(cur, ST_LOC_HELP_FLAG)) {
            st_print_usage_info(args, num_args);
            return false;
        } else {
            _ST_ERROR(ST_LOC_UNK_OPT" '%s'", cur->flag);
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
    (void)printf(WHITEB(ST_LOC_PRESS_KEY"\n"));
    bool get = st_getchar(NULL);
   _ST_UNUSED(get);
}

void st_timer_begin(st_timer* timer)
{
    (void)st_msec_since(NULL, timer);
}

double st_timer_elapsed(const st_timer* timer)
{
    st_timer now;
    return st_msec_since(timer, &now);
}

long st_timer_getres(void)
{
    long retval = 0L;
#if !defined(__WIN__)
    struct timespec res;
    int get = clock_getres(ST_INTERVALCLOCK, &res);
    if (0 == get)
        retval = res.tv_nsec;
#else /* __WIN__ */
    LARGE_INTEGER perf_freq;
    (void)QueryPerformanceFrequency(&perf_freq);
    if (perf_freq.QuadPart <= 0)
        retval = 0L;
    else
        retval = (long)(ceil(((double)perf_freq.QuadPart) / 1e9));
#endif
    return retval;
}

bool st_clock_gettime(int clock, time_t* tbuf, double* msecbuf)
{
    if (tbuf) {
#if defined(ST_MSEC_POSIX)
        struct timespec ts = {0};
        int ret = clock_gettime(clock, &ts);
        if (0 == ret) {
            *tbuf = ts.tv_sec;
            if (msecbuf) {
                *msecbuf = (double)(ts.tv_nsec) / 1e6;
            }
        } else {
            if (msecbuf) {
                *msecbuf = 0.0;
            }
            return false;
        }
#elif defined(ST_MSEC_WIN32)
       _ST_UNUSED(clock);
        static const ULONGLONG uepoch = (ULONGLONG)116444736e9;

        FILETIME ftutc = {0};
        GetSystemTimePreciseAsFileTime(&ftutc);

        ULARGE_INTEGER ftnow = {0};
        ftnow.HighPart = ftutc.dwHighDateTime;
        ftnow.LowPart  = ftutc.dwLowDateTime;
        ftnow.QuadPart = (ULONGLONG)((ftnow.QuadPart - uepoch) / 10000000ULL);

        *tbuf = (time_t)ftnow.QuadPart;

        SYSTEMTIME st = {0};
        if (FileTimeToSystemTime(&ftutc, &st)) {
            if (msecbuf)
                *msecbuf = (double)st.wMilliseconds;
        } else {
            if (msecbuf)
                *msecbuf = 0.0;
            return false;
        }
#else
       _ST_UNUSED(clock);
        time(tbuf);
        if (msecbuf)
            *msecbuf = 0.0;
#endif
        return true;
    }
    return false;
}

double st_msec_since(const st_timer* when, st_timer* out)
{
    if (out == NULL)
        return 0.0;

#if !defined(__WIN__)
    out->sec = 0;
    out->msec = 0.0;

    bool gettime = st_clock_gettime(ST_INTERVALCLOCK, &out->sec, &out->msec);
    if (when == NULL || !gettime || (out->sec < when->sec ||
        (out->sec == when->sec && out->msec < when->msec)))
        return 0.0;



    return ((((double)out->sec) * 1e3) + out->msec) -
           ((((double)when->sec) * 1e3) + when->msec);
#else /* __WIN__ */
    LARGE_INTEGER perf_freq = {0};

    // TODO: store somewhere in static storage
    (void)QueryPerformanceFrequency(&perf_freq);
    (void)QueryPerformanceCounter(&out->counter);

    if (when == NULL || out->counter.QuadPart <= when->counter.QuadPart)
        return 0.0;

    double msec_ratio = ((double)perf_freq.QuadPart) / 1e3;
    return ((double)(out->counter.QuadPart - when->counter.QuadPart)) / msec_ratio;
#endif
}

char* st_format_error_msg(int code, char message[ST_MAX_ERROR])
{
    message[0] = '\0';

#if !defined(__WIN__)
    int finderr = -1;
# if defined(__HAVE_XSI_STRERROR_R__)
    finderr = strerror_r(code, message, ST_MAX_ERROR);
#  if defined(__HAVE_XSI_STRERROR_R_ERRNO__)
    if (finderr == -1) {
        finderr = errno;
    }
#  endif
# elif defined(__HAVE_GNU_STRERROR_R__)
    const char* tmp = strerror_r(code, message, ST_MAX_ERROR);
    if (tmp != message)
        _st_strcpy(message, ST_MAX_ERROR, tmp, strnlen(tmp, ST_MAX_ERROR));
# elif defined(__HAVE_STRERROR_S__)
    finderr = (int)strerror_s(msg, ST_MAX_ERROR, code);
# else
    const char* tmp = strerror(code);
    _st_strcpy(message, ST_MAX_ERROR, tmp, strnlen(tmp, ST_MAX_ERROR));
# endif
# if defined(__HAVE_XSI_STRERROR_R__) || defined(__HAVE_STRERROR_S__)
    assert(0 == finderr);
    _ST_UNUSED(finderr);
# else
   _ST_UNUSED(finderr);
# endif
#else /* __WIN__ */
    DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
                  FORMAT_MESSAGE_MAX_WIDTH_MASK;
    DWORD fmt = FormatMessageA(flags, NULL, (DWORD)code, 0UL, message, ST_MAX_ERROR, NULL);
    assert(0UL != fmt);
    if (fmt > 0UL) {
        if (message[fmt - 1] == '\n' || message[fmt - 1] == ' ') {
            message[fmt - 1] = '\0';
        }
    }
#endif

    return &message[0];
}

void st_sleep_msec(uint32_t msec)
{
    if (0U == msec) {
        return;
    }
#if !defined(__WIN__)
    struct timespec ts = { msec / 1000, (msec % 1000) * 1000000 };
    (void)nanosleep(&ts, NULL);
#else /* __WIN__ */
    (void)SleepEx((DWORD)msec, TRUE);
#endif
}

char* st_getcwd(void)
{
#if !defined(__WIN__)
# if defined(__linux__) && defined(_GNU_SOURCE)
    return get_current_dir_name();
# else
    return getcwd(NULL, 0);
# endif
#else /* __WIN__ */
    return _getcwd(NULL, 0);
#endif
}

bool st_get_avail_fs_bytes(const char* restrict path, uint64_t* bytes)
{
    if (bytes) {
        *bytes = 0;
    }
    if (!bytes || !path || !*path) {
        _ST_REPORT_ERROR(ST_E_INVALID);
        return false;
    }
    _ST_DEBUG("getting available fs space (path: '%s')...", path);
#if !defined(__WIN__)
    struct statvfs stvfs = {0};
    if (-1 == statvfs(path, &stvfs)) {
        _ST_REPORT_ERROR(errno);
        return false;
    }
    *bytes = (stvfs.f_bavail * (stvfs.f_frsize ? stvfs.f_frsize : stvfs.f_bsize));
#else
    ULARGE_INTEGER free_bytes = {0};
    if (!GetDiskFreeSpaceExA(NULL, &free_bytes, NULL, NULL)) {
        _ST_REPORT_ERROR(GetLastError());
        return false;
    }
    *bytes = free_bytes.QuadPart;
#endif
    _ST_DEBUG("available fs space: %"PRIu64" bytes (%.02f GiB)", *bytes,
        ((double)(*bytes) / 1024.0 / 1024.0));
    return true;
}

bool st_have_inet_connection(void)
{
#if defined(__WIN__)
    WSADATA wsad = {0};
    (void)WSAStartup(MAKEWORD(2, 2), &wsad);
#endif
    struct addrinfo hints = {
        .ai_flags = 0,
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM
    };
    struct addrinfo* result = NULL;
    int get = getaddrinfo(ST_INET_TARGET_HOST, ST_INET_TARGET_SRV,
        (const struct addrinfo*)&hints, &result);
    if (0 != get) {
        __ST_REPORT_ERROR(get, gai_strerror(get));
#if defined(__WIN__)
        (void)WSACleanup();
#endif
        return false;
    }

    _ST_DEBUG("getaddrinfo for %s [%s] OK; creating a suitable socket...",
        ST_INET_TARGET_HOST, ST_INET_TARGET_SRV);

    bool connected = false;
    struct addrinfo* cur = result;
    while (cur != NULL) {
        st_descriptor sock = socket(AF_INET, cur->ai_socktype, cur->ai_protocol);
        if (sock == ST_BAD_DESCRIPTOR) {
            _ST_DEBUG("socket failed (%d)", st_last_sockerr());
            cur = cur->ai_next;
            continue;
        }

        _ST_DEBUG("got socket descriptor %d; connecting to %s [%s]...", (int)sock,
            ST_INET_TARGET_HOST, ST_INET_TARGET_SRV);

        const struct timeval timeout = {
            .tv_sec = ST_INET_TIMEOUT,
            .tv_usec = 0
        };

        (void)setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const void*)&timeout,
            sizeof(timeout));
        (void)setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const void*)&timeout,
            sizeof(timeout));

        int conn = connect(sock, (const struct sockaddr*)cur->ai_addr,
            (st_optlen)cur->ai_addrlen);
        if (conn == 0) {
            _ST_DEBUG("connected to %s!", ST_INET_TARGET_HOST);
            connected = true;
        } else {
            _ST_DEBUG("connect failed (%d)", st_last_sockerr());
        }
#if !defined(__WIN__)
        close(sock);
#else
        closesocket(sock);
#endif
        if (connected) {
            break;
        }
        cur = cur->ai_next;
    }
    freeaddrinfo(result);
#if defined(__WIN__)
    (void)WSACleanup();
#endif
    return connected;
}
