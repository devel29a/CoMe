# MIT License
#
# Copyright (c) 2024 devel29a
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.20)

project(CoMe)

include(FetchContent)

option(BUILD_DRIO "Build DynamoRIO from source" OFF)
option(DRIO_VER   "Use specific DynamoRIO version" OFF)
option(DRIO_URL   "Use custom DynamoRIO URL" OFF)
option(BUILD_TEST "Build CoMe tests" OFF)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release")
endif ()

message(STATUS "Selected build mode: ${CMAKE_BUILD_TYPE}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(DynamoRIOPackage)

# Configure CoMe target
add_library(CoMe SHARED src/main.cpp)
target_include_directories(CoMe PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Configure DynamoRIO
configure_DynamoRIO_client(CoMe)

# TODO Configure DynamoRIO extensions
# use_DynamoRIO_extension()

# Set platform type for DynamoRIO
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  target_compile_definitions(CoMe PUBLIC WINDOWS)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_compile_definitions(CoMe PUBLIC LINUX)
else ()
  message (FATAL_ERROR "Unsupported system: '${CMAKE_SYSTEM_NAME}'")
endif ()

message (STATUS "Target system: '${CMAKE_SYSTEM_NAME}'")

# Set architecture type for DynamoRIO
message(STATUS "PROCESSOR ${CMAKE_SYSTEM_PROCESSOR}")
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" )
  target_compile_definitions(CoMe PUBLIC X86_64)
else ()
  message (FATAL_ERROR "Unsupported architecture '${CMAKE_SYSTEM_PROCESSOR}'")
endif ()

message (STATUS "Target architecture: '${CMAKE_SYSTEM_PROCESSOR}'")

if (BUILD_TEST)
    include(CTest)
    find_package(GTest)
    add_subdirectory(test)
endif()
