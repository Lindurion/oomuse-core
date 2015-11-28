## Copyright 2015 Eric W. Barndollar. All rights reserved.
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.

cmake_minimum_required(VERSION 3.0.2)
project(oomuse-core)

enable_testing()

find_package(GTest REQUIRED)


################################################################################
# oomuse-core Library
################################################################################

set(OOMUSE_CORE_CPP_FILES
    src/oomuse/core/strings.cpp)

add_library(oomuse-core STATIC ${OOMUSE_CORE_CPP_FILES})
set_property(TARGET oomuse-core
    APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include)


################################################################################
# oomuse-core Tests
################################################################################

set(OOMUSE_CORE_TEST_FILES
    test/oomuse/core/FixedArray_test.cpp
    test/oomuse/core/Optional_test.cpp
    test/oomuse/core/Validators_test.cpp
    test/oomuse/core/strings_test.cpp)

add_executable(oomuse-core_test ${OOMUSE_CORE_TEST_FILES})

set_property(TARGET oomuse-core_test
    APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_property(TARGET oomuse-core_test
    APPEND PROPERTY INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS})

if(MSVC)
  # Suppress warnings about std::uninitialized_copy().
  set_property(TARGET oomuse-core_test
      APPEND PROPERTY COMPILE_DEFINITIONS _SCL_SECURE_NO_WARNINGS)
endif()

target_link_libraries(oomuse-core_test oomuse-core)
target_link_libraries(oomuse-core_test ${GTEST_BOTH_LIBRARIES})

add_test(run_oomuse-core_test oomuse-core_test)
