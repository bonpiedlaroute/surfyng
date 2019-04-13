cmake_minimum_required(VERSION 3.8.2)

set (CMAKE_CXX_STANDARD 17)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2")

SET( SRCS src/TransformTable.cpp
   ../../sf_services/dynamodb_access/thrift_generated/dynamodb_access_constants.cpp
   ../../sf_services/dynamodb_access/thrift_generated/dynamodb_access_types.cpp
   ../../sf_services/dynamodb_access/thrift_generated/dynamodb_access.cpp
   ../../sf_services/sf_classifier/src/RealEstateAd.cpp
)

project(adapt_model)

set( CMAKE_PREFIX_PATH "${SF_UTILS_BUILD_DIR}")
find_library(SF_UTILS_LIB sf_utils)

include_directories(../..)
include_directories(../../sf_services/dynamodb_access/thrift_generated)
include_directories(inc)

# The executable name and its sourcefiles
add_executable(adapt_model ${SRCS})

# The libraries used by the executable.
target_link_libraries(adapt_model ${SF_UTILS_LIB} thrift pthread)
