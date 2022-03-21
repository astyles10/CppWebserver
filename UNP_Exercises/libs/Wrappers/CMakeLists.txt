add_library(Wrappers Socket.cpp)

target_include_directories(Wrappers
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
  /usr/include
)

install(TARGETS Wrappers DESTINATION lib)
install(FILES Socket.hpp DESTINATION include)
