cmake_minimum_required(VERSION 3.0)

project(gaea)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
        -g
        -Wall
        -Wextra
        -Werror
        -Wconversion
        -Wno-unused-parameter
        -Wold-style-cast
        -Wsequence-point
        -Woverloaded-virtual
        -Wpointer-arith
        -Wshadow
        -Wthread-safety
        -Wwrite-strings
        -march=native
        -std=c++17
        -Wno-null-dereference
        -Wno-sign-conversion
        -Wno-unused-private-field
        -fmodules)

if(CMAKE_BUILD_BITS EQUAL 32)
    list(APPEND CXX_FLAGS "-m32")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include_directories("${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES
        gaea/utils/FileUtil.cc)

add_library(gaea SHARED ${SOURCE_FILES})

#add_executable(gaea_simple examples/main.cc)
#target_link_libraries(gaea_simple gaea)

add_executable(avl_test gaea/tree/test/AVLTreeTest.cc)

add_executable(bplus_test gaea/tree/test/BTreeTest.cc)

add_executable(bplus_map_test gaea/db/test/BPlusMapTest.cc)

add_executable(files_test gaea/utils/test/file_test.cc)
target_link_libraries(files_test gaea)

#add_executable(btree_unittest gaea/tree/test/BTreeTest.cc)
#target_link_libraries(btree_unittest gaea boost_unit_test_framework)
#add_test(NAME btree_unittest COMMAND btree_unittest)