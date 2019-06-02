cmake_minimum_required(VERSION 3.10)
project(knnectCPP)

set(CMAKE_CXX_STANDARD 11)
# Using -g for debugging
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -pthread")
link_libraries('sqlite3')
link_libraries('rdkafka')
link_libraries('cppkafka')
link_libraries('pthread')
link_libraries('stdc++')

add_executable(knnectCPP main.cpp libs/headers/KafkaTest.h libs/KafkaTest.cpp libs/Partition.cpp libs/Partitioner.cpp libs/transports/websocket/WebSocketServer.cpp)

