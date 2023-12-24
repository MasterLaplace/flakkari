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
                Protocol::API::Header header(
                    Protocol::API::Priority::LOW,
                    Protocol::API::ApiVersion::V_1,
                    int(Protocol::API::FlakkariEventId::REP_ENTITY_SPAWN),
                    0
                );

                PlayerPacket playerPacket {
                    PlayerPacketType::REP_ENTITY_SPAWN,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    std::vector<std::uint8_t>()
                };

                header._contentLength = sizeof(playerPacket);

                Network::Buffer buffer(sizeof(header) + sizeof(playerPacket));
                std::copy(reinterpret_cast<const char*>(&header), reinterpret_cast<const char*>(&header) + sizeof(header), buffer.begin());

                std::cout << "Header: " << std::endl;
                std::cout << "  Priority: " << (int)header._priority << std::endl;
                std::cout << "  ApiVersion: " << (int)header._apiVersion << std::endl;
                std::cout << "  CommandId: " << (int)header._commandId << std::endl;
                std::cout << "  ContentLength: " << (int)header._contentLength << std::endl;

                std::cout << buffer << std::endl;

                _socket.sendTo(packet->first, buffer);
            }
        }
    }
    return 0;
}

} /* namespace Flakkari */
