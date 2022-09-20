cmake_minimum_required(VERSION 3.18)
project(httpServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g2 -o0")
set(CMAKE_BUILD_TYPE Debug)

message(INFO ${CMAKE_CXX_FLAGS})

include_directories(include)
add_executable(httpServer main.cpp src/tcpServer.cpp src/tcpServerThread.cpp src/utils/epoll.cpp src/worker.cpp)
target_link_libraries(httpServer /usr/local/lib/libasync_simple.a)
