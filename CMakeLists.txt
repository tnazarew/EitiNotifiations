cmake_minimum_required(VERSION 2.8)
project(EiTINotifications)

find_package(Boost REQUIRED COMPONENTS system thread)

find_package(RapidJSON REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive")


include_directories(include
        ${Boost_INCLUDE_DIRS}
        ${RapidJSON_INCLUDE_DIRS}
        )
link_directories(${Boost_LIBRARY_DIRS} ${RapidJSON_LIBRARY_DIRS})
add_library(
        ${PROJECT_NAME}
        src/server.cpp
        src/socket.cpp
	src/RSA.cpp
	src/rc4.cpp
	src/md5.cpp
	src/Util.cpp
)


add_executable(server_test main.cpp)

target_link_libraries(server_test ${PROJECT_NAME} ${Boost_LIBRARIES} ${RapidJSON_LIBRARIES})

