/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** core
*/

#include <iostream>

#include "FlakkariMessage.hpp"
#include "Network/Socket.hpp"

int main() {
    Flakkari::Network::Socket socket("localhost", 8080, Flakkari::Network::Address::IpType::IPv4, Flakkari::Network::Address::SocketType::UDP);
    std::cout << socket << std::endl;
    socket.bind();
    while (true)
    {
        auto packet = socket.receiveFrom();
        std::cout << packet.first;
        std::cout << " : ";
        std::cout << packet.second.value_or("No packet") << std::endl;
    }
    FLAKKARI_LOG_INFO("Server: Hello, World!");
    FLAKKARI_LOG_LOG("Server: Hello, World!");
    FLAKKARI_LOG_DEBUG("Server: Hello, World!");
    FLAKKARI_LOG_WARNING("Server: Hello, World!");
    FLAKKARI_LOG_ERROR("Server: Hello, World!");
    FLAKKARI_LOG_FATAL("Server: Hello, World!");
    return 0;
}
