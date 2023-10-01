/*
 * macros.h
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
#ifndef _SEATEST_MACROS_H_INCLUDED
# define _SEATEST_MACROS_H_INCLUDED

# include "seatest/types.h"

# define _ESC "\x1b[" /**< Begins an ANSI escape sequence. */
# define _ESC_M "m"   /**< Marks the end of a sequence. */

# define _ESC_SEQ(codes, s) _ESC codes _ESC_M s
# define _ESC_SEQE(codes)   _ESC_SEQ(codes, "")

/** Resets all previously applied colors and effects/attributes. */
# define _ESC_RST _ESC_SEQE("0")

/** Allows for setting of the attributes, foreground, and background colors
 * simultaneously. */
# define COLOR(attr, fg, bg, s) \
    _ESC_SEQ(#attr ";38;5;" #fg ";48;5;" #bg, s) _ESC_RST

/** Sets only the foreground color, setting background to its default. */
# define FG_COLOR(attr, fg, s) \
    _ESC_SEQ(#attr ";38;5;" #fg ";49;5", s) _ESC_RST

/** Sets only the background color, setting foreground to its default. */
# define BG_COLOR(attr, bg, s) \
    _ESC_SEQ(#attr ";39;5;48;5;" #bg, s) _ESC_RST

# define ULINE(s) _ESC_SEQ("4", s) _ESC_SEQE("24") /**< Underlined. */
# define EMPH(s)  _ESC_SEQ("3", s) _ESC_SEQE("23") /**< Emphasis/italic. */
# define BOLD(s)  _ESC_SEQ("1", s) _ESC_SEQE("22") /**< Bold. */

# define BLACK(s)     FG_COLOR(0, 0, s) /**< Black foreground text. */
# define BLACKB(s)    FG_COLOR(1, 0, s) /**< Bold black foreground text. */

# define RED(s)       FG_COLOR(0, 1, s) /**< Red foreground text. */
# define REDB(s)      FG_COLOR(1, 1, s) /**< Bold red foreground text. */

# define BRED(s)      FG_COLOR(0, 9, s) /**< Bright red foreground text. */
# define BREDB(s)     FG_COLOR(1, 9, s) /**< Bold bright red foreground text. */

# define GREEN(s)     FG_COLOR(0, 2, s) /**< Green foreground text. */
# define GREENB(s)    FG_COLOR(1, 2, s) /**< Bold green foreground text. */

# define BGREEN(s)    FG_COLOR(0, 10, s) /**< Bright green foreground text. */
# define BGREENB(s)   FG_COLOR(1, 10, s) /**< Bold bright green foreground text. */

# define YELLOW(s)    FG_COLOR(0, 3, s) /**< Yellow foreground text. */
# define YELLOWB(s)   FG_COLOR(1, 3, s) /**< Bold yellow foreground text. */

# define BYELLOW(s)   FG_COLOR(0, 11, s) /**< Bright yellow foreground text. */
# define BYELLOWB(s)  FG_COLOR(1, 11, s) /**< Bold bright yellow foreground text. */

# define BLUE(s)      FG_COLOR(0, 4, s) /**< Blue foreground text. */
# define BLUEB(s)     FG_COLOR(1, 4, s) /**< Bold blue foreground text. */

# define BBLUE(s)     FG_COLOR(0, 12, s) /**< Bright blue foreground text. */
# define BBLUEB(s)    FG_COLOR(1, 12, s) /**< Bold bright blue foreground text. */

# define MAGENTA(s)   FG_COLOR(0, 5, s) /**< Magenta foreground text. */
# define MAGENTAB(s)  FG_COLOR(1, 5, s) /**< Bold magenta foreground text. */

# define BMAGENTA(s)  FG_COLOR(0, 13, s) /**< Bright magenta foreground text. */
# define BMAGENTAB(s) FG_COLOR(1, 13, s) /**< Bold bright magenta foreground text. */

# define CYAN(s)      FG_COLOR(0, 6, s) /**< Cyan foreground text. */
# define CYANB(s)     FG_COLOR(1, 6, s) /**< Bold cyan foreground text. */

# define BCYAN(s)     FG_COLOR(0, 13, s) /**< Bright cyan foreground text. */
# define BCYANB(s)    FG_COLOR(1, 13, s) /**< Bold bright cyan foreground text. */

# define BGRAY(s)     FG_COLOR(0, 7, s) /**< Bright gray foreground text. */
# define BGRAYB(s)    FG_COLOR(1, 7, s) /**< Bold bright gray foreground text. */

# define DGRAY(s)     FG_COLOR(0, 8, s) /**< Dark gray foreground text. */
# define DGRAYB(s)    FG_COLOR(1, 8, s) /**< Bold dark gray foreground text. */

# define WHITE(s)     FG_COLOR(0, 15, s) /**< White foreground text. */
# define WHITEB(s)    FG_COLOR(1, 15, s) /**< Bold white foreground text. */

/** Blots out a variable, avoiding compiler warnings/errors. */
# define ST_UNUSED(var) (void)(var)

/** Declares an individual test function. The name must only contain characters
 * allowed in C function names. */
# define ST_DECLARE_TEST(name) \
    st_testres st_test_##name(void)

/** Begins the definition (implementation) of an individual test function. */
# define ST_BEGIN_TEST_IMPL(name) \
    st_testres st_test_##name(void) \
    { \
        st_testres _retval; \
        _retval.line_start = __LINE__; \
        _retval.pass = true; \
        _retval.fatal = false;

/** Ends the definition (implementation) of an individual test function. */
# define ST_END_TEST_IMPL(...) \
        return _retval; \
    }

# define _ST_INDENT "  " /**< Indentation placed before test message output. */

/** Outputs a test message. */
# define _ST_MESSAGE(msg, ...) (void)printf(_ST_INDENT msg "\n", __VA_ARGS__)

/** Outputs a formatted test message in white. */
# define ST_MESSAGE(msg, ...) _ST_MESSAGE(WHITE(msg), __VA_ARGS__)
# define ST_MESSAGE_0(msg)    ST_MESSAGE(msg "%s", "")

/** Outputs a formatted test message in red. */
# define ST_ERROR(msg, ...) _ST_MESSAGE(FG_COLOR(0, 196, msg), __VA_ARGS__)
# define ST_ERROR_0(msg)    ST_ERROR(msg "%s", "")

/** Outputs a formatted test message in orange. */
# define ST_WARNING(msg, ...) _ST_MESSAGE(FG_COLOR(0, 208, msg), __VA_ARGS__)
# define ST_WARNING_0(msg)    ST_WARNING(msg "%s", "")

/** Outputs a formatted test message in green. */
# define ST_SUCCESS(msg, ...) _ST_MESSAGE(FG_COLOR(0, 46, msg), __VA_ARGS__)
# define ST_SUCCESS_0(msg)    ST_SUCCESS(msg "%s", "")

# define _ST_TESTLINE() (__LINE__ - _retval.line_start)
# define _ST_FATAL() \
    _retval.pass = false; \
    _retval.fatal = true

# define ST_EXPECT(expr) \
    do { \
        if (!(expr)) { \
            _retval.pass = false; \
            ST_WARNING("at line %"PRIu32": " #expr " == false", _ST_TESTLINE()); \
        } \
    } while (false)

# define ST_REQUIRE(expr) \
    do { \
        if (!(expr)) { \
            _ST_FATAL(); \
            ST_ERROR("at line %"PRIu32": " #expr " == false", _ST_TESTLINE()); \
        } \
    } while (false)

#endif /* !_SEATEST_MACROS_H_INCLUDED */
