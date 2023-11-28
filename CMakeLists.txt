# Flakkari/CMakeLists.txt
cmake_minimum_required(VERSION 3.26.4)
project(R-Type_Server)

add_executable(r-type_server
    Flakkari/core.cpp
)

# Set the output directory for the server executable
set_target_properties(r-type_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Custom target for cleaning server build artifacts
add_custom_target(clean-server
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin/r-type_server
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/_deps/r-type_server
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/r-type_server/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/r-type_server/CMakeFiles
)
