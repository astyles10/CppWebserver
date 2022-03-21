cmake_minimum_required(VERSION 3.10)

# Project Name and Version
project(UNP VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Project executable & source files
add_executable(UNP unp.cpp)

# Auto generate 'unpConfig.h', copying defines from unpConfig.h.in
configure_file(unpConfig.h.in unpConfig.h)

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Wrappers)
list(APPEND MYLIBS Wrappers)

target_link_libraries(UNP PUBLIC ${MYLIBS})

target_include_directories(UNP PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/libs/Wrappers"
                          )

# cmake --install .
# Default install path is to /usr/local/bin. Override with
# cmake --install . --prefix "<directory>"
install(TARGETS UNP DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/unpConfig.h" DESTINATION include)

# Example default output:
# % cmake --install .
# -- Install configuration: ""
# -- Installing: /usr/local/bin/UNP
# -- Installing: /usr/local/include/unpConfig.h
# -- Installing: /usr/local/lib/libWrappers.a
# -- Installing: /usr/local/include/Socket.hpp

# Example output override:
# % cmake --install . --prefix "$(pwd)/bin"
# -- Install configuration: ""
# -- Installing: /home/beefmince/Projects/GB_Emulator_Stuff/CppWebserver/UNP_Exercises/build/bin/bin/UNP
# -- Installing: /home/beefmince/Projects/GB_Emulator_Stuff/CppWebserver/UNP_Exercises/build/bin/include/unpConfig.h
# -- Installing: /home/beefmince/Projects/GB_Emulator_Stuff/CppWebserver/UNP_Exercises/build/bin/lib/libWrappers.a
# -- Installing: /home/beefmince/Projects/GB_Emulator_Stuff/CppWebserver/UNP_Exercises/build/bin/include/Socket.hpp
