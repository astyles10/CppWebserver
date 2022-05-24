cmake_minimum_required(VERSION 3.20)
set(PROJECT Sockets)
project(${PROJECT})

add_library(${PROJECT}
  Socket.cpp
  SocketIPv4.cpp)

target_include_directories(${PROJECT}
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
  /usr/include
)

install(TARGETS ${PROJECT} DESTINATION /usr/local/lib)
install(FILES Socket.hpp SocketIPv4.hpp DESTINATION /usr/local/include/Networking/Sockets)
