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

# Configure test application targets
add_executable(simple_loop apps/simple_loop.cpp)

if (MSVC)
    set(COME_LIB_BINARY    "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}CoMe${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(SIMPLE_LOOP_BINARY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/simple_loop${CMAKE_EXECUTABLE_SUFFIX}")
else ()
    set(COME_LIB_BINARY    "${CMAKE_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}CoMe${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(SIMPLE_LOOP_BINARY "${CMAKE_CURRENT_BINARY_DIR}/simple_loop${CMAKE_EXECUTABLE_SUFFIX}")
endif ()

add_executable(ModuleTest utest/module.cpp ${CMAKE_SOURCE_DIR}/src/module.cpp ${CMAKE_SOURCE_DIR}/src/profiler.cpp)
target_include_directories(ModuleTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(ModuleTest gtest)

add_executable(ProfilerTest utest/profiler.cpp ${CMAKE_SOURCE_DIR}/src/module.cpp ${CMAKE_SOURCE_DIR}/src/profiler.cpp)
target_include_directories(ProfilerTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(ProfilerTest gtest)

add_test(NAME SmokeTest
         COMMAND "${dynamorio_SOURCE_DIR}/bin64/drrun"
         "-c"
         "${COME_LIB_BINARY}"
         "--"
         "${SIMPLE_LOOP_BINARY}")

add_test(NAME ModuleTest
         COMMAND ModuleTest)

add_test(NAME ProfilerTest
         COMMAND ProfilerTest)