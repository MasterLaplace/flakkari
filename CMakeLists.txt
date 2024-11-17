# Flakkari/CMakeLists.txt
cmake_minimum_required(VERSION 3.2...3.22)

project(Flakkari-Server VERSION 0.4.0 LANGUAGES CXX)

# Add all your source and headers files:
set(SOURCES
    Flakkari/core.cpp

    Flakkari/Logger/Logger.cpp

    Flakkari/Network/Network.cpp
    Flakkari/Network/Address.cpp
    Flakkari/Network/Buffer.cpp
    Flakkari/Network/Socket.cpp
    Flakkari/Network/IOMultiplexer.cpp

    Flakkari/Engine/Math/Vector.cpp

    Flakkari/Engine/EntityComponentSystem/Systems/Systems.cpp
    Flakkari/Engine/EntityComponentSystem/Registry.cpp

    Flakkari/Server/UDPServer.cpp
    Flakkari/Server/Client/Client.cpp
    Flakkari/Server/Client/ClientManager.cpp

    Flakkari/Server/Game/Game.cpp
    Flakkari/Server/Game/GameManager.cpp
    Flakkari/Server/Game/ResourceManager.cpp

    Flakkari/Server/Internals/CommandManager.cpp
)

set(HEADERS
    Flakkari/Logger/Logger.hpp

    Flakkari/Network/Network.hpp
    Flakkari/Network/Address.hpp
    Flakkari/Network/Buffer.hpp
    Flakkari/Network/Socket.hpp
    Flakkari/Network/Serializer.hpp
    Flakkari/Network/PacketQueue.hpp
    Flakkari/Network/IOMultiplexer.hpp

    Flakkari/Protocol/Commands.hpp
    Flakkari/Protocol/Components.hpp
    Flakkari/Protocol/Events.hpp
    Flakkari/Protocol/Header.hpp
    Flakkari/Protocol/Packet.hpp
    Flakkari/Protocol/PacketFactory.hpp

    Flakkari/Engine/Math/Vector.hpp

    Flakkari/Engine/EntityComponentSystem/Components/Components2D.hpp
    Flakkari/Engine/EntityComponentSystem/Components/Components3D.hpp
    Flakkari/Engine/EntityComponentSystem/Components/ComponentsCommon.hpp
    Flakkari/Engine/EntityComponentSystem/Systems/Systems.hpp
    Flakkari/Engine/EntityComponentSystem/Entity.hpp
    Flakkari/Engine/EntityComponentSystem/SparseArrays.hpp
    Flakkari/Engine/EntityComponentSystem/Registry.hpp
    Flakkari/Engine/EntityComponentSystem/EntityFactory.hpp

    Flakkari/Server/UDPServer.hpp

    Flakkari/Server/Client/Client.hpp
    Flakkari/Server/Client/ClientManager.hpp

    Flakkari/Server/Game/Game.hpp
    Flakkari/Server/Game/GameManager.hpp
    Flakkari/Server/Game/ResourceManager.hpp

    Flakkari/Server/Internals/CommandManager.hpp
)

set(HEADER_LIB_LOGGER
    Flakkari/Logger/Logger.hpp
)

set(HEADER_LIB_NETWORK
    Flakkari/Network/Network.hpp
    Flakkari/Network/Address.hpp
    Flakkari/Network/Buffer.hpp
    Flakkari/Network/Socket.hpp
    Flakkari/Network/Serializer.hpp
    Flakkari/Network/PacketQueue.hpp
    Flakkari/Network/IOMultiplexer.hpp
)

set(HEADER_LIB_PROTOCOL
    Flakkari/Protocol/Commands.hpp
    Flakkari/Protocol/Components.hpp
    Flakkari/Protocol/Events.hpp
    Flakkari/Protocol/Header.hpp
    Flakkari/Protocol/Packet.hpp
    Flakkari/Protocol/PacketFactory.hpp
)

# CMake Modules:
INCLUDE(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_Declare(
    Singleton
    GIT_REPOSITORY https://github.com/MasterLaplace/Singleton.git
    GIT_TAG v1.0.2
)

FetchContent_MakeAvailable(nlohmann_json)
FetchContent_MakeAvailable(Singleton)

# Separate Build Artifacts:
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(FLAKKARI_GAME_DIR ${CMAKE_SOURCE_DIR}/Games)
message(STATUS "FLAKKARI_GAME_DIR: ${FLAKKARI_GAME_DIR}")

# Compiler Standards:
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler Warnings:
if (MSVC)
    add_compile_options(/W4)
elseif (WIN32)
    add_compile_options(-Wall -Wextra -ggdb -O0 -g3)
else()
    add_compile_options(-Wall -Wextra -ggdb -O0 -g3)
endif()

# Create the executable
add_executable(flakkari ${SOURCES} ${HEADERS})

# Include Directories:
target_include_directories(flakkari PRIVATE ${CMAKE_SOURCE_DIR}/Flakkari)
target_include_directories(flakkari PRIVATE ${singleton_SOURCE_DIR})

# Link Libraries:
target_link_libraries(flakkari PRIVATE nlohmann_json::nlohmann_json)

# Documentation: sudo apt-get install graphviz
find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile.cfg
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# Versioning:
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION_PATCH 0)

configure_file(${CMAKE_SOURCE_DIR}/Flakkari/config.h.in ${CMAKE_BINARY_DIR}/config.h)

# Install Targets:
install(TARGETS flakkari DESTINATION bin)
install(FILES ${CMAKE_BINARY_DIR}/config.h DESTINATION include/Flakkari)
install(FILES ${HEADER_LIB_LOGGER} DESTINATION include/Flakkari/Logger)
install(FILES ${HEADER_LIB_NETWORK} DESTINATION include/Flakkari/Network)
install(FILES ${HEADER_LIB_PROTOCOL} DESTINATION include/Flakkari/Protocol)

# Packaging:
set(CPACK_PACKAGE_NAME "flakkari")
set(CPACK_PACKAGE_VENDOR "R-Type")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "R-Type Server")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "R-Type Team")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "nlohmann-json3-dev (>= 3.9.1)" "doxygen (>= 1.8.17)" "graphviz (>= 2.40.1)")
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
add_library(flakkari_network SHARED ${SOURCES_LIB_NETWORK} ${HEADER_LIB_NETWORK} ${HEADER_LIB_LOGGER})

# Include Directories:
target_include_directories(flakkari_network PRIVATE ${CMAKE_SOURCE_DIR}/Flakkari)
target_include_directories(flakkari_network PRIVATE ${singleton_SOURCE_DIR})

# Link Libraries:
target_link_libraries(flakkari_network PRIVATE nlohmann_json::nlohmann_json)

# Install dynamic library:
install(TARGETS flakkari_network DESTINATION lib)
install(FILES ${HEADER_LIB_NETWORK} DESTINATION include/Flakkari/Network)
