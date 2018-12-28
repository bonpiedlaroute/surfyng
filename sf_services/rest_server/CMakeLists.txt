cmake_minimum_required (VERSION 3.8.2)

project(rest_server)

set(CMAKE_CXX_STANDARD 17)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")


include_directories(inc)



add_executable(rest_server
  src/main.cpp
  src/HttpRequestHandler.cpp
  src/DBaccess.cpp
  )

target_link_libraries(rest_server boost_system crypto ssl cpprest)







