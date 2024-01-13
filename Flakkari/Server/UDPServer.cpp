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
    _socket(std::make_shared<Network::Socket>(
        ip, port,
        Network::Address::IpType::IPv4,
        Network::Address::SocketType::UDP
    ))
{
    FLAKKARI_LOG_INFO(std::string(*_socket));
    _socket->bind();

    _io = std::make_unique<Network::PSELECT>();
    _io->addSocket(_socket->getSocket());
    _io->addSocket(STDIN_FILENO);

    ClientManager::setSocket(_socket);
    GameManager::getInstance();
}

UDPServer::~UDPServer() {
    FLAKKARI_LOG_INFO("UDPServer is now stopped");
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
    auto packet = _socket->receiveFrom();
    ClientManager::addClient(packet->first);
    ClientManager::checkInactiveClients();

    ClientManager::receivePacketFromClient(packet->first, packet->second);
    ClientManager::sendPacketToClient(packet->first, packet->second);
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
