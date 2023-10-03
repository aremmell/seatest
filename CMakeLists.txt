# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: Copyright (c) 2023 Ryan M. Lederman

cmake_minimum_required(
    VERSION 3.12.0
)

# variables
set(PROJECT_NAME seatest)
set(MAIN_EXECUTABLE_NAME seatest)
set(STATIC_LIBRARY_NAME seatest_static)
set(SHARED_LIBRARY_NAME seatest_shared)

# versioning
set(PROJECT_VERSION_SUFFIX -dev)
set(PROJECT_IS_REL_BUILD true)

# compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# C17 standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED true)

# policy settings, so CMake doesn't whine.
cmake_policy(SET CMP0025 NEW) # AppleClang
cmake_policy(SET CMP0048 NEW) # project VERSION
cmake_policy(SET CMP0065 NEW) # ENABLE_EXPORTS
cmake_policy(SET CMP0056 NEW) # CMAKE_EXE_LINKER_FLAGS
cmake_policy(SET CMP0066 NEW) # CMAKE_<LANG>_FLAGS_<BUILDTYPE>

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    cmake_policy(SET CMP0077 NEW) # option
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.17")
    cmake_policy(SET CMP0102 NEW) # mark_as_advanced
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.21")
    cmake_policy(SET CMP0126 NEW) # cache
endif()

# define the project
project(
    ${PROJECT_NAME}
    VERSION 0.1.0
    LANGUAGES C
    DESCRIPTION "seatest: a test framework for c."
)

# preprocessor defines
add_compile_definitions(
    # simulate failure to determine available disk space
    #ST_SIMULATE_DISK_ERROR
    # simulate low disk space (mutually exclusive w/ ST_SIMULATE_DISK_ERROR)
    #ST_SIMULATE_DISK_INSUFFICIENT
    # simulate failure to detect internet connection
    ST_SIMULATE_INET_ERROR
    # verbose debug messages
    ST_DEBUG_MESSAGES
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# compiler options
if (MSVC)
    add_compile_options(
        /W4 /MP /GS /experimental:c11atomics /wd4267
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Release>:/Ox>
    )
else()
    add_compile_options( # -Wformat-nonliteral is also an option
        -Wall -Wextra -Wpedantic -Werror -Wdouble-promotion -Wconversion -Wno-sign-conversion -Wno-string-conversion -Wshift-overflow
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-g3>
        $<$<CONFIG:Release>:-O3>
    )

    if (CMAKE_COMPILER_IS_GNUCC)
        add_compile_options(
            -Wformat-truncation
        )
    endif()

    add_compile_definitions(
        $<$<CONFIG:Release>:_FORTIFY_SOURCE=2>
    )
endif()

execute_process(
    COMMAND git rev-parse --short --verify HEAD
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ECHO_OUTPUT_VARIABLE
)

configure_file(
    include/seatest/version.h.in
    include/seatest/version.h
    NEWLINE_STYLE LF
)

add_executable(
    ${MAIN_EXECUTABLE_NAME}
    src/main.c
)

add_library(
    ${STATIC_LIBRARY_NAME}
    STATIC
    src/seatest.c
)

add_library(
    ${SHARED_LIBRARY_NAME}
    SHARED
    src/seatest.c
)

if(!WIN32)
    set_target_properties(
        ${STATIC_LIBRARY_NAME}
        PROPERTIES OUTPUT_NAME
        ${PROJECT_NAME}
    )
    set_target_properties(
        ${SHARED_LIBRARY_NAME}
        PROPERTIES OUTPUT_NAME
        ${PROJECT_NAME}
    )
endif()

target_include_directories(
    ${MAIN_EXECUTABLE_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_include_directories(
    ${STATIC_LIBRARY_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_include_directories(
    ${SHARED_LIBRARY_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries(
    ${MAIN_EXECUTABLE_NAME}
    ${STATIC_LIBRARY_NAME}
)

target_compile_features(
    ${MAIN_EXECUTABLE_NAME}
    PUBLIC
    ${C_STANDARD}
)

target_compile_features(
    ${STATIC_LIBRARY_NAME}
    PUBLIC
    ${C_STANDARD}
)

target_compile_features(
    ${SHARED_LIBRARY_NAME}
    PUBLIC
    ${C_STANDARD}
)
