cmake_minimum_required(VERSION 3.8.2)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")

project(sf_utils_test)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


SET(SRCS test_algorithm.cpp	
)

set( CMAKE_PREFIX_PATH "${SF_UTILS_BUILD_DIR}")
find_library(SF_UTILS_LIB sf_utils)

include_directories(../../../..)
include_directories(../inc)


# The executable name and its sourcefiles
add_executable(sf_utils_test ${SRCS})

# The libraries used by the executable.
target_link_libraries(sf_utils_test ${GTEST_BOTH_LIBRARIES} ${SF_UTILS_LIB})