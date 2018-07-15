cmake_minimum_required(VERSION 3.8.2)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")

project(sf_classifier_test)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


SET( SRCS ../src/Criteria.cpp
   ../src/CriteriaManager.cpp
   ../src/RealEstateAd.cpp
   ../src/RealEstateAdClassifier.cpp
   sf_classifier_test.cpp	
)

set( CMAKE_PREFIX_PATH "${SF_UTILS_BUILD_DIR}")
find_library(SF_UTILS_LIB sf_utils)
find_package(Boost 1.65.1)

include_directories(../../../..)
include_directories(../inc)
include_directories(${Boost_INCLUDE_DIRS})

# The executable name and its sourcefiles
add_executable(sf_classifier_test ${SRCS})

# The libraries used by the executable.
target_link_libraries(sf_classifier_test ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES} ${SF_UTILS_LIB} pthread)