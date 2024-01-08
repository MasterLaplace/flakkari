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
    Internals::CommandManager::handleCommand();
    return true;
}

void UDPServer::handlePacket()
{
    auto packet = _socket.receiveFrom();
    ClientManager::addClient(packet->first);
    ClientManager::checkInactiveClients();

    // parse packet
    Protocol::API::Header header;
    std::copy(packet->second.begin(), packet->second.begin() + sizeof(header), reinterpret_cast<char*>(&header));

    std::cout << (*packet->first.get()); // Address
    std::cout << " : ";
    std::cout << packet->second << std::endl; // Buffer

    std::cout << "RECV Header: " << std::endl;
    std::cout << "  Priority: " << (int)header._priority << std::endl;
    std::cout << "  ApiVersion: " << (int)header._apiVersion << std::endl;
    std::cout << "  CommandId: " << (int)header._commandId << std::endl;
    std::cout << "  ContentLength: " << (int)header._contentLength << std::endl;

    // send to all clients
    Protocol::API::Header sendHeader(
        Protocol::API::Priority::LOW,
        Protocol::API::ApiVersion::V_1,
        int(Protocol::API::FlakkariEventId::REP_ENTITY_SPAWN),
        0
    );

    Protocol::API::PlayerPacket playerPacket;

    sendHeader._contentLength = sizeof(playerPacket);

    Network::Buffer buffer(sizeof(sendHeader) + sizeof(playerPacket));
    std::copy(reinterpret_cast<const char*>(&sendHeader), reinterpret_cast<const char*>(&sendHeader) + sizeof(sendHeader), buffer.begin());

    std::cout << "SEND Header: " << std::endl;
    std::cout << "  Priority: " << (int)sendHeader._priority << std::endl;
    std::cout << "  ApiVersion: " << (int)sendHeader._apiVersion << std::endl;
    std::cout << "  CommandId: " << (int)sendHeader._commandId << std::endl;
    std::cout << "  ContentLength: " << (int)sendHeader._contentLength << std::endl;

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
