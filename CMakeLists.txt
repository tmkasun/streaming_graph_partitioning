cmake_minimum_required(VERSION 3.10)
project(knnectCPP)

set(CMAKE_CXX_STANDARD 14)
# Using -g for debugging
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -pthread")
link_libraries('sqlite3')
link_libraries('rdkafka')
link_libraries('cppkafka')
link_libraries('pthread')
link_libraries('stdc++')

add_executable(knnectCPP main.cpp util/logger/Logger.cpp libs/graphStore/PropertyLink.cpp libs/graphStore/RelationBlock.cpp libs/graphStore/NodeBlock.cpp libs/graphStore/NodeManager.cpp libs/headers/KafkaTest.h libs/KafkaTest.cpp libs/Partition.cpp libs/Partitioner.cpp libs/transports/websocket/WebSocketServer.cpp)
add_executable(restAPI restAPI.cpp util/logger/Logger.cpp libs/graphStore/PropertyLink.cpp libs/graphStore/RelationBlock.cpp libs/graphStore/NodeBlock.cpp libs/graphStore/NodeManager.cpp)

target_include_directories(knnectCPP PUBLIC "libs/external/spdlog-1.8.1/include") # For logging library
target_include_directories(knnectCPP PUBLIC "libs/external/json-3.9.1/include") # For logging library


target_include_directories(restAPI PUBLIC "libs/external/spdlog-1.8.1/include") # For logging library
target_include_directories(restAPI PUBLIC "libs/external/json-3.9.1/include") # For logging library
find_package(Pistache 0.0.2 REQUIRED)
target_link_libraries(restAPI pistache_shared)