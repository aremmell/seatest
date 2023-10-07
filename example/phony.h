/*
 * phony.h
 *
 * Phony types and functions for demonstration purposes.
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
#ifndef _SEATEST_EXAMPLE_PHONY_H_INCLUDED
# define _SEATEST_EXAMPLE_PHONY_H_INCLUDED

# include <stdbool.h>
# include <stdint.h>

/**
 * Widgets.
 */

#define WIDGET_COLOR_PURPLE     1
#define WIDGET_CLASS_STANDARD 102
#define WIDGET_FLAGS_STANDARD (0x1 | 0x2 | 0x4)

typedef struct {
    int32_t color;
    int32_t class;
    uint32_t flags;
} acme_widget;

static inline
bool acme_create_widget_purple(acme_widget* out) {
    if (!out) {
        return false;
    }

    out->color = WIDGET_COLOR_PURPLE;
    out->class = WIDGET_CLASS_STANDARD;
    out->flags = WIDGET_FLAGS_STANDARD;

    return true;
}

/**
 * REST client.
 */

#define HTTP_OK 200

typedef struct {
    int code;
    const char* body;
} http_response;

static inline
bool acme_rest_get_widget_list(http_response* out) {
    if (!out) {
        return false;
    }

    out->code = HTTP_OK;
    out->body = "{\"widgets\": [{\"color\": 1, \"class\": 102, \"flags\": 7}]}";

    return true;
}

#endif /* ! _SEATEST_EXAMPLE_PHONY_H_INCLUDED */
