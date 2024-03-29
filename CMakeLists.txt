cmake_minimum_required(VERSION 3.1)
project(sylar)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

include(cmake/utils.cmake)


include_directories(.)
LINK_DIRECTORIES(/usr/local/lib)

set(LIB_SRC
    src/log.cpp
    src/utils.cpp
    src/config.cpp
    src/thread.cpp
    src/Fiber.cpp
    )

add_library(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar) #__FILE__


add_executable(test tests/test.cpp)
add_dependencies(test sylar)
force_redefine_file_macro_for_sources(test) #__FILE__
target_link_libraries(test sylar yaml-cpp)

add_executable(test_config tests/testconfig.cpp)
add_dependencies(test_config sylar)
force_redefine_file_macro_for_sources(test_config) #__FILE__
target_link_libraries(test_config sylar yaml-cpp)

add_executable(test_thread tests/testthread.cpp)
add_dependencies(test_thread sylar)
force_redefine_file_macro_for_sources(test_thread) #__FILE__
target_link_libraries(test_thread sylar yaml-cpp)

add_executable(test_utils tests/testutils.cpp)
add_dependencies(test_utils sylar)
force_redefine_file_macro_for_sources(test_utils) #__FILE__
target_link_libraries(test_utils sylar yaml-cpp)


add_executable(test_fiber tests/testfiber.cpp)
add_dependencies(test_fiber sylar)
force_redefine_file_macro_for_sources(test_fiber) #__FILE__
target_link_libraries(test_fiber sylar yaml-cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
