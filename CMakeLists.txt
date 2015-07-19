cmake_minimum_required(VERSION 3.0)

project("event-system")

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(core/include)

set(CMAKE_CXX_FLAGS "-fPIC -Wall -std=c++11 -Werror -D__STDC_LIMIT_MACROS")

add_executable(event-system-test test/event-system-test.cc)
