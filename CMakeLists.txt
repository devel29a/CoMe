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

cmake_minimum_required(VERSION 3.31)

project(CoMe)

include(ExternalProject)

option(BUILD_DRIO "Build DynamoRIO from source" OFF)

if (BUILD_DRIO)
  ExternalProject_Add(DynamoRIO
    GIT_REPOSITORY  "https://github.com/DynamoRIO/dynamorio.git"
    GIT_TAG         "release_11.2.0"
    GIT_SHALLOW     TRUE
    PREFIX          "3rdparty/DynamoRIO"
  )
else()
  ExternalProject_Add(DynamoRIO
    URL "https://github.com/DynamoRIO/dynamorio/releases/download/release_11.2.0/DynamoRIO-Windows-11.2.0.zip"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
    PREFIX          "3rdparty/DynamoRIO"
  )
endif(BUILD_DRIO)

ExternalProject_Get_property(DynamoRIO SOURCE_DIR)
include_directories("${SOURCE_DIR}/include")

#find_package(DynamoRIO REQUIRED)

# if (NOT DynamoRIO_FOUND)
#   message(FATAL_ERROR "Required DynamoRIO dependency is not found")
# endif (NOT DynamoRIO_FOUND)

add_executable(CoMe src/main.cpp)
add_dependencies(CoMe DynamoRIO)

# Set platform type
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  target_compile_definitions(CoMe PUBLIC WINDOWS)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_compile_definitions(CoMe PUBLIC LINUX)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Macos")
  target_compile_definitions(CoMe PUBLIC MACOS)
else ()
  message (FATAL_ERROR "Unsupported system: '${CMAKE_SYSTEM_NAME}'")
endif ()

message (STATUS "Target system: '${CMAKE_SYSTEM_NAME}'")

# Set architecture type
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64")
  target_compile_definitions(CoMe PUBLIC X86_64)
else ()
  message (FATAL_ERROR "Unsupported architecture '${CMAKE_SYSTEM_PROCESSOR}'")
endif ()

message (STATUS "Target architecture: '${CMAKE_SYSTEM_PROCESSOR}'")

target_link_libraries(CoMe)

