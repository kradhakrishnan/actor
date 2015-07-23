cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

include (iocore/cmake/DefaultCMakeLists.txt)

include_directories (include
					 iocore/third-party/gtest-1.7.0
					 ${Boost_INCLUDE_DIR})

LINK_DIRECTORIES(${CMAKE_BINARY_DIR})

add_executable (actor_test test/actor_test.cc)

target_link_libraries (actor_test gtest core pthread boost_regex)

add_test(${RUN_TEST_CASE} ${CMAKE_BINARY_DIR}/actor_test)
