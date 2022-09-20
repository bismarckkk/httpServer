cmake_minimum_required(VERSION 3.18)
project(httpServer)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_executable(httpServer main.cpp src/tcpServer.cpp src/tcpServerThread.cpp src/utils/epoll.cpp src/worker.cpp)
target_link_libraries(httpServer async_simple)
