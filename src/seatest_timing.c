/*
 * seatest_timing.c
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
#include "seatest/timing.h"

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

bool st_clock_gettime(int clock, time_t* tbuf, long* msecbuf)
{
    if (tbuf) {
#if defined(ST_MSEC_POSIX)
        struct timespec ts = {0};
        int ret = clock_gettime(clock, &ts);
        if (0 == ret) {
            *tbuf = ts.tv_sec;
            if (msecbuf)
                *msecbuf = ts.tv_nsec / 1000000L;
        } else {
            if (msecbuf)
                *msecbuf = 0L;
            return false;
        }
#elif defined(ST_MSEC_WIN32)
        ST_UNUSED(clock);
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
                *msecbuf = (long)st.wMilliseconds;
        } else {
            if (msecbuf)
                *msecbuf = 0L;
            return false;
        }
#else
        ST_UNUSED(clock);
        time(tbuf);
        if (msecbuf)
            *msecbuf = 0L;
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
    out->msec = 0L;

    bool gettime = st_clock_gettime(ST_INTERVALCLOCK, &out->sec, &out->msec);
    if (when == NULL || !gettime || (out->sec < when->sec ||
        (out->sec == when->sec && out->msec < when->msec)))
        return 0.0;

    return ((((double)out->sec) * 1e3) + (double)out->msec) -
           ((((double)when->sec) * 1e3) + (double)when->msec);
#else /* __WIN__ */
    LARGE_INTEGER perf_freq = {0};

    // TODO: store somewhere in static storage
    (void)QueryPerformanceFrequency(&perf_freq);
    (void)QueryPerformanceCounter(&out->counter);

    if (when == NULL || out->counter.QuadPart <= when->counter.QuadPart)
        return 0.0;

    double msec_ratio = ((double)_sir_perfcntr_freq.QuadPart) / 1e3;
    return ((double)(out->counter.QuadPart - when->counter.QuadPart)) / msec_ratio;
#endif
}

void st_sleep_msec(uint32_t msec) {
    if (0U == msec)
        return;

#if !defined(__WIN__)
    struct timespec ts = { msec / 1000, (msec % 1000) * 1000000 };
    (void)nanosleep(&ts, NULL);
#else /* __WIN__ */
    (void)SleepEx((DWORD)msec, TRUE);
#endif
}
