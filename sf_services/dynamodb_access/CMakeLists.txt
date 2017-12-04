cmake_minimum_required(VERSION 3.8.2)

set (CMAKE_CXX_STANDARD 17)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")

SET( SRCS thrift_generated/dynamodb_access_constants.cpp
   thrift_generated/dynamodb_access.cpp
   thrift_generated/dynamodb_access_types.cpp
   thrift_generated/dynamodb_client_constants.cpp
   thrift_generated/dynamodb_client_types.cpp
   thrift_generated/dynamodb_client.cpp
   src/dynamodb_access_handler.cpp
   src/dynamodb_access_server.cpp  
)

project(dynamodb_access)

include_directories(../../..)
include_directories(inc)
include_directories(thrift_generated)

set( CMAKE_PREFIX_PATH "${AWS_BUILD_DIR}" "${SF_UTILS_BUILD_DIR}")
find_package(aws-sdk-cpp)
find_library(SF_UTILS_LIB sf_utils)

# Link to the SDK shared libraries.
add_definitions(-DUSE_IMPORT_EXPORT)

# The executable name and its sourcefiles
add_executable(dynamodb_access ${SRCS})

# The libraries used by the executable.
target_link_libraries(dynamodb_access aws-cpp-sdk-core aws-cpp-sdk-dynamodb thrift ${SF_UTILS_LIB})


