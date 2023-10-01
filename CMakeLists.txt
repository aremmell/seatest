# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: Copyright (c) 2023 Ryan M. Lederman

cmake_minimum_required(
    VERSION 3.12.0
)

# variables
set(PROJECT_NAME seatest)

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
    DESCRIPTION "seatest: A test framework for C."
)
