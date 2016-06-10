cmake_minimum_required(VERSION 2.8)
project(EiTINotifications)



find_package(Boost REQUIRED COMPONENTS system thread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive")
#find_package(PostgreSQL REQUIRED COMPONENTS pqxx pq)
include_directories("/usr/local/include/pqxx /usr/local/pgsql/include /usr/local/pgsql/lib")
include_directories(include
		json
        ${Boost_INCLUDE_DIRS}
#        /usr/local/pgsql/include
#        /usr/local/include/pqxx
        )
link_directories(${Boost_LIBRARY_DIRS} /usr/local/pgsql/lib)
add_library(
        ${PROJECT_NAME}
        src/server.cpp
        src/socket.cpp
		src/RSA.cpp
		src/rc4.cpp
		src/md5.cpp
		src/Util.cpp
		src/security.cpp
        src/jsoncpp.cpp
		src/JsonBuilder.cpp
		src/JsonResolver.cpp
		src/DatabaseService.cpp
)


add_executable(server_test main.cpp)
target_link_libraries(server_test ${PROJECT_NAME} ${Boost_LIBRARIES} ${RapidJSON_LIBRARIES})

add_executable(application_test test/application_test.cpp)
target_link_libraries(application_test ${PROJECT_NAME} pqxx pq)
