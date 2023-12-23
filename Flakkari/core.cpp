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
        std::cout << (*packet.value().first.get());
        std::cout << " : ";
        std::cout << packet.value().second << std::endl;
        socket.sendTo(packet.value().first, packet.value().second);
    }
    return 0;
}
