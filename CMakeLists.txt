# Flakkari/CMakeLists.txt
cmake_minimum_required(VERSION 3.26.4)

project(R-Type_Server)

# Add all your source and headers files:
set(SOURCES
    Flakkari/core.cpp
    Flakkari/Logger/Logger.cpp
    Flakkari/Network/Address.cpp
    Flakkari/Network/Buffer.cpp
    Flakkari/Network/Socket.cpp
    Flakkari/Network/IOMultiplexer.cpp
    Flakkari/Protocol/Header.cpp
    Flakkari/Protocol/Packet.cpp
    Flakkari/Engine/EntityComponentSystem/Registry.cpp
    Flakkari/Server/UDPServer.cpp
    Flakkari/Server/Client/Client.cpp
    Flakkari/Server/Client/ClientManager.cpp
    Flakkari/Server/Internals/CommandManager.cpp
)

set(HEADERS
    Flakkari/Logger/Logger.hpp
    Flakkari/Network/Address.hpp
    Flakkari/Network/Buffer.hpp
    Flakkari/Network/Socket.hpp
    Flakkari/Network/IOMultiplexer.hpp
    Flakkari/Protocol/Header.hpp
    Flakkari/Protocol/Packet.hpp
    Flakkari/Engine/EntityComponentSystem/Entity.hpp
    Flakkari/Engine/EntityComponentSystem/SparseArrays.hpp
    Flakkari/Engine/EntityComponentSystem/Registry.hpp
    Flakkari/Server/UDPServer.hpp
    Flakkari/Server/Client/Client.hpp
    Flakkari/Server/Client/ClientManager.hpp
    Flakkari/Server/Internals/CommandManager.hpp
)

set(HEADER_LIB_LOGGER
    Flakkari/Logger/Logger.hpp
)

set(HEADER_LIB_NETWORK
    Flakkari/Network/Address.hpp
    Flakkari/Network/Buffer.hpp
    Flakkari/Network/Socket.hpp
    Flakkari/Network/IOMultiplexer.hpp
)

# Separate Build Artifacts:
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Compiler Standards:
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler Warnings:
if(MSVC)
    add_compile_options(/W4)
elseif(WIN32)
    add_compile_options(-Wall -Wextra -pedantic)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Create the executable
add_executable(r-type_server ${SOURCES} ${HEADERS})

# Include Directories:
target_include_directories(r-type_server PRIVATE ${CMAKE_SOURCE_DIR}/Flakkari)

# Documentation: sudo apt-get install graphviz
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile.cfg
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# Versioning:
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

configure_file(${CMAKE_SOURCE_DIR}/Flakkari/config.h.in ${CMAKE_BINARY_DIR}/config.h)

# Install Targets:
install(TARGETS r-type_server DESTINATION bin)
install(FILES ${CMAKE_BINARY_DIR}/config.h DESTINATION include/Flakkari)
install(FILES ${HEADER_LIB_LOGGER} DESTINATION include/Flakkari/Logger)
install(FILES ${HEADER_LIB_NETWORK} DESTINATION include/Flakkari/Network)

# Packaging:
set(CPACK_PACKAGE_NAME "r-type_server")
set(CPACK_PACKAGE_VENDOR "R-Type")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "R-Type Server")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "R-Type Team")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")

include(CPack)

# Create a dynamic library for the Network components
set(SOURCES_LIB_NETWORK
    Flakkari/Logger/Logger.cpp
    Flakkari/Network/Address.cpp
    Flakkari/Network/Buffer.cpp
    Flakkari/Network/Socket.cpp
    Flakkari/Network/IOMultiplexer.cpp
)
add_library(r-type_server_network SHARED ${SOURCES_LIB_NETWORK} ${HEADER_LIB_NETWORK} ${HEADER_LIB_LOGGER})

# Include Directories:
target_include_directories(r-type_server_network PRIVATE ${CMAKE_SOURCE_DIR}/Flakkari)
# Install dynamic library:
install(TARGETS r-type_server_network DESTINATION lib)
install(FILES ${HEADER_LIB_NETWORK} DESTINATION include/Flakkari/Network)
