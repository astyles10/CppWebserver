add_library(Wrappers
  Socket.cpp
  SocketIPv4.cpp)

target_include_directories(Wrappers
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
  /usr/include
)

install(TARGETS Wrappers DESTINATION /usr/local/lib)
install(FILES Socket.hpp SocketIPv4.hpp DESTINATION /usr/local/include)
