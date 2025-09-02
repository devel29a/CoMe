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
add_executable(simple_multi_threaded_loop apps/simple_multi_threaded_loop.cpp)

if (MSVC)
    set(COME_LIB_BINARY    "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}CoMe${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(SIMPLE_LOOP_BINARY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/simple_loop${CMAKE_EXECUTABLE_SUFFIX}")
    set(SIMPLE_MULTI_THREADED_LOOP_BINARY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/simple_multi_threaded_loop${CMAKE_EXECUTABLE_SUFFIX}")
else ()
    set(COME_LIB_BINARY    "${CMAKE_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}CoMe${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(SIMPLE_LOOP_BINARY "${CMAKE_CURRENT_BINARY_DIR}/simple_loop${CMAKE_EXECUTABLE_SUFFIX}")
    set(SIMPLE_MULTI_THREADED_LOOP_BINARY "${CMAKE_CURRENT_BINARY_DIR}/simple_multi_threaded_loop${CMAKE_EXECUTABLE_SUFFIX}")
endif ()

add_executable(ModuleTest utest/module.cpp ${CMAKE_SOURCE_DIR}/src/profiler/module.cpp)
target_include_directories(ModuleTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(ModuleTest gtest)

add_executable(BasicBlockTest utest/basicblock.cpp ${CMAKE_SOURCE_DIR}/src/profiler/basicblock.cpp)
target_include_directories(BasicBlockTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(BasicBlockTest gtest)

add_executable(SymbolTest utest/symbol.cpp ${CMAKE_SOURCE_DIR}/src/profiler/symbol.cpp)
target_include_directories(SymbolTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(SymbolTest gtest)

add_executable(ThreadTest utest/thread.cpp ${CMAKE_SOURCE_DIR}/src/profiler/thread.cpp)
target_include_directories(ThreadTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(ThreadTest gtest)

add_executable(SampleTest utest/sample.cpp ${CMAKE_SOURCE_DIR}/src/profiler/sample.cpp)
target_include_directories(SampleTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(SampleTest gtest)

add_executable(LedgerTest utest/ledger.cpp ${CMAKE_SOURCE_DIR}/src/profiler/ledger.cpp ${CMAKE_SOURCE_DIR}/src/profiler/module.cpp ${CMAKE_SOURCE_DIR}/src/profiler/thread.cpp ${CMAKE_SOURCE_DIR}/src/profiler/sample.cpp)
target_include_directories(LedgerTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(LedgerTest gtest)

add_executable(FormatterTest utest/formatter.cpp ${CMAKE_SOURCE_DIR}/src/profiler/formatter.cpp ${CMAKE_SOURCE_DIR}/src/profiler/ledger.cpp ${CMAKE_SOURCE_DIR}/src/profiler/module.cpp ${CMAKE_SOURCE_DIR}/src/profiler/thread.cpp ${CMAKE_SOURCE_DIR}/src/profiler/sample.cpp)
target_include_directories(FormatterTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(FormatterTest gtest)

file(GLOB PROFILER_SOURCES "${CMAKE_SOURCE_DIR}/src/profiler/*.cpp")
add_executable(ProfilerTest utest/profiler.cpp ${PROFILER_SOURCES})
target_include_directories(ProfilerTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(ProfilerTest gtest)

add_test(NAME SmokeTestSingleThread
         COMMAND "${dynamorio_SOURCE_DIR}/bin64/drrun"
         "-c"
         "${COME_LIB_BINARY}"
         "--"
         "${SIMPLE_LOOP_BINARY}")

add_test(NAME SmokeTestMultiThread
         COMMAND "${dynamorio_SOURCE_DIR}/bin64/drrun"
         "-c"
         "${COME_LIB_BINARY}"
         "--"
         "${SIMPLE_MULTI_THREADED_LOOP_BINARY}")

add_test(NAME ModuleTest
         COMMAND ModuleTest)

add_test(NAME BasicBlockTest
         COMMAND BasicBlockTest)

add_test(NAME SymbolTest
         COMMAND SymbolTest)

add_test(NAME ThreadTest
         COMMAND ThreadTest)

add_test(NAME SampleTest
         COMMAND SampleTest)

add_test(NAME LedgerTest
         COMMAND LedgerTest)

add_test(NAME FormatterTest
         COMMAND FormatterTest)

add_test(NAME ProfilerTest
         COMMAND ProfilerTest)
