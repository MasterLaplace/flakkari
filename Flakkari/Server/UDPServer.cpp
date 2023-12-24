/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-24
** File description:
** UDPServer
*/

#include "UDPServer.hpp"

namespace Flakkari {

UDPServer::UDPServer(std::string ip, std::size_t port) :
    _socket(Flakkari::Network::Socket(
        ip, port,
        Flakkari::Network::Address::IpType::IPv4,
        Flakkari::Network::Address::SocketType::UDP
    ))
{
    std::cout << _socket << std::endl;
    _socket.bind();

    _io = std::make_unique<Flakkari::Network::PPOLL>();
    _io->addSocket(_socket.getSocket());
}

int UDPServer::run() {
    while (true)
    {
        int result = _io->wait();

        if (result == -1) {
            FLAKKARI_LOG_FATAL("Failed to poll sockets, error: " + std::string(::strerror(errno)));
            return 84;
        } else if (result == 0) {
            FLAKKARI_LOG_DEBUG("ppoll timed out");
            ClientManager::checkInactiveClients();
            continue;
        }
        for (auto &fd : *_io) {
            if (_io->isReady(fd)) {
                auto packet = _socket.receiveFrom();
                std::cout << (*packet->first.get());
                std::cout << " : ";
                std::cout << packet->second << std::endl;
                ClientManager::addClient(packet->first);
                ClientManager::checkInactiveClients();
            }
        }
    }
    return 0;
}

} /* namespace Flakkari */
