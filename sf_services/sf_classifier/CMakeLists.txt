cmake_minimum_required(VERSION 3.8.2)

set (CMAKE_CXX_STANDARD 17)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")

SET( SRCS src/Criteria.cpp
   src/CriteriaManager.cpp
   src/RealEstateAd.cpp
   src/RealEstateAdClassifier.cpp
   src/PurgeRealEstateAd.cpp
   ../../../surfyng/sf_services/dynamodb_access/thrift_generated/dynamodb_access_constants.cpp
   ../../../surfyng/sf_services/dynamodb_access/thrift_generated/dynamodb_access_types.cpp
   ../../../surfyng/sf_services/dynamodb_access/thrift_generated/dynamodb_access.cpp
   src/Classifier.cpp
   src/Main.cpp  
)

project(sf_classifier)

set( CMAKE_PREFIX_PATH "${SF_UTILS_BUILD_DIR}")
find_library(SF_UTILS_LIB sf_utils)
find_package(Boost 1.65.1)

include_directories(../../..)
include_directories(../../../surfyng/sf_services/dynamodb_access/thrift_generated)
include_directories(inc)
include_directories(${Boost_INCLUDE_DIRS})

# The executable name and its sourcefiles
add_executable(sf_classifier ${SRCS})

# The libraries used by the executable.
target_link_libraries(sf_classifier  ${Boost_LIBRARIES} ${SF_UTILS_LIB} thrift pthread)