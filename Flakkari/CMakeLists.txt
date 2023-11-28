# src/CMakeLists.txt

# Add all source files in the src directory
add_library(core_library core.cpp)

# Create an executable for the source code
add_executable(my_executable core.cpp)

# Link the executable with the core library
target_link_libraries(my_executable PRIVATE core_library)

# Specify the output directory for the executable
set_target_properties(my_executable PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
