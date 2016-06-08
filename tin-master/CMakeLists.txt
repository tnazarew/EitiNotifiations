cmake_minimum_required(VERSION 3.5)
project(tin_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
include_directories("/usr/local/include/pqxx /usr/local/pgsql/include /usr/local/pgsql/lib")

set(SOURCE_FILES main.cpp jsoncpp.cpp json/json.h json/json-forwards.h DatabaseService.cpp DatabaseService.h DatabaseObjects.h JsonResolver.cpp JsonResolver.h JsonBuilder.cpp JsonBuilder.h)
add_executable(tin_server ${SOURCE_FILES})
target_link_libraries(tin_server pqxx)
target_link_libraries(tin_server pq)