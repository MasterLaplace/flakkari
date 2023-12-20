/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** core
*/

#include <iostream>

#include "FlakkariMessage.hpp"
#include "Network/Address.hpp"



int main() {
    Flakkari::Network::Address address(-1, Flakkari::Network::Address::SocketType::UDP, Flakkari::Network::Address::IpType::IPv4);
    std::cout << address << std::endl;
    FLAKKARI_LOG_INFO("Server: Hello, World!");
    FLAKKARI_LOG_LOG("Server: Hello, World!");
    FLAKKARI_LOG_DEBUG("Server: Hello, World!");
    FLAKKARI_LOG_WARNING("Server: Hello, World!");
    FLAKKARI_LOG_ERROR("Server: Hello, World!");
    FLAKKARI_LOG_FATAL("Server: Hello, World!");
    return 0;
}
