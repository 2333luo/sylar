cmake_minimum_required(VERSION 3.1)
project(sylar)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

include_directories(.)
LINK_DIRECTORIES(/usr/local/lib)

set(LIB_SRC
    src/log.cpp
    src/utils.cpp
    src/config.cpp
    )

add_library(sylar SHARED ${LIB_SRC})

add_executable(test tests/test.cpp)
add_dependencies(test sylar)
target_link_libraries(test sylar)

add_executable(test_config tests/testconfig.cpp)
add_dependencies(test_config sylar)
target_link_libraries(test_config sylar yaml-cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
