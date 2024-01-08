/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-24
** File description:
** UDPServer
*/

#include "UDPServer.hpp"

using namespace Flakkari;


UDPServer::UDPServer(std::string ip, std::size_t port) :
    _socket(Network::Socket(
        ip, port,
        Network::Address::IpType::IPv4,
        Network::Address::SocketType::UDP
    ))
{
    FLAKKARI_LOG_INFO(std::string(_socket));
    _socket.bind();

    _io = std::make_unique<Network::PSELECT>();
    _io->addSocket(_socket.getSocket());
}

bool UDPServer::handleTimeout(int event)
{
    if (event != 0)
        return false;
            FLAKKARI_LOG_DEBUG("ppoll timed out");
            ClientManager::checkInactiveClients();
    return true;
        }

bool UDPServer::handleInput(int fd)
{
    if (fd != STDIN_FILENO)
        return false;
    if (std::cin.eof())
        throw std::runtime_error("EOF on stdin");
    return true;
}

void UDPServer::handlePacket()
{
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

                Protocol::API::PlayerPacket playerPacket;

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

void UDPServer::run()
{
    INIT_LOOP;
    int result = _io->wait();

    if (result == -1) {
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
            GOTO_LOOP;
        throw std::runtime_error("Failed to poll sockets, error: " + STD_ERROR);
    }

    if (handleTimeout(result))
        GOTO_LOOP;

    for (auto &fd : *_io) {
        if (!_io->isReady(fd))
            continue;
        if (handleInput(fd))
            continue;
        handlePacket();
    }
    GOTO_LOOP;
}

int runGames() {
    return 0;
}
