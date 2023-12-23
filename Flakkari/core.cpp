/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** core
*/

#include <iostream>
#include "Network/IOMultiplexer.hpp"

int main() {
    Flakkari::Network::Socket socket("localhost", 8080, Flakkari::Network::Address::IpType::IPv4, Flakkari::Network::Address::SocketType::UDP);
    std::cout << socket << std::endl;
    socket.bind();

    auto io = std::make_unique<Flakkari::Network::PPOLL>();
    io->addSocket(socket.getSocket());

    while (true)
    {
        int result = io->wait();

        if (result == -1) {
            FLAKKARI_LOG_FATAL("Failed to poll sockets, error: " + std::string(::strerror(errno)));
            return 84;
        } else if (result == 0) {
            FLAKKARI_LOG_DEBUG("ppoll timed out");
            continue;
        }
        for (auto &fd : *io) {
            if (io->isReady(fd)) {
        auto packet = socket.receiveFrom();
        std::cout << (*packet.value().first.get());
        std::cout << " : ";
        std::cout << packet.value().second << std::endl;
        socket.sendTo(packet.value().first, packet.value().second);
            }
        }
    }
    return 0;
}
