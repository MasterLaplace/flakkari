/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-24
** File description:
** ClientManager
*/

#include "ClientManager.hpp"
#include "../Game/GameManager.hpp"

#include <future>

namespace Flakkari {

std::shared_ptr<ClientManager> ClientManager::_instance = nullptr;
std::mutex ClientManager::_mutex;

std::shared_ptr<ClientManager> ClientManager::getInstance()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_instance)
        _instance = std::make_shared<ClientManager>();
    return _instance;
}

void ClientManager::setSocket(std::shared_ptr<Network::Socket> socket) { getInstance()->_socket = socket; }

void ClientManager::addClient(std::shared_ptr<Network::Address> client, Network::Buffer &buffer)
{
    auto &clients = getInstance()->_clients;

    if (ClientManager::isBanned(client))
    {
        FLAKKARI_LOG_LOG("Client " + client->toString().value_or("Unknown") + " tried to connect but is banned");
        return;
    }

    if (clients.find(client->toString().value_or("")) != clients.end())
        return clients[client->toString().value_or("")]->keepAlive(), void();

    Protocol::Packet<Protocol::CommandId> packet;
    if (!packet.deserialize(buffer))
    {
        FLAKKARI_LOG_WARNING("Client " + client->toString().value_or("Unknown") + " sent an invalid packet");
        return;
    }

    if (packet.header._commandId != Protocol::CommandId::REQ_CONNECT)
    {
        FLAKKARI_LOG_WARNING("Client " + client->toString().value_or("Unknown") + " sent an invalid packet");
        return;
    }

    std::string name = packet.extractString();
    clients[client->toString().value_or("")] = std::make_shared<Client>(client, name);

    FLAKKARI_LOG_LOG("Client " + client->toString().value_or("Unknown") + " connected");
    GameManager::addClientToGame(name, clients[client->toString().value_or("")]);
}

void ClientManager::removeClient(std::shared_ptr<Network::Address> client)
{
    auto &clients = getInstance()->_clients;

    if (clients.find(client->toString().value_or("")) == clients.end())
        return;

    auto _client = clients[client->toString().value_or("")];

    GameManager::removeClientFromGame(_client->getGameName(), _client);
    clients.erase(client->toString().value_or(""));
}

void ClientManager::banClient(std::shared_ptr<Network::Address> client)
{
    auto &clients = getInstance()->_clients;
    auto &bannedClients = getInstance()->_bannedClients;

    if (clients.find(client->toString().value_or("")) == clients.end())
        return;

    bannedClients.push_back(client->getIp().value());

    FLAKKARI_LOG_LOG("Client " + client->toString().value_or("Unknown") + " banned");
    auto _client = clients[client->toString().value_or("")];
    GameManager::removeClientFromGame(_client->getGameName(), _client);
    clients.erase(client->toString().value_or(""));
}

bool ClientManager::isBanned(std::shared_ptr<Network::Address> client)
{
    auto &bannedClients = getInstance()->_bannedClients;

    return std::find(bannedClients.begin(), bannedClients.end(), client->getIp().value_or("")) != bannedClients.end();
}

void ClientManager::checkInactiveClients()
{
    auto &clients = getInstance()->_clients;

    for (auto it = clients.begin(); it != clients.end();)
    {
        if (!it->second->isConnected())
        {
            FLAKKARI_LOG_LOG("Client " + it->first + " disconnected");
            it = clients.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void ClientManager::sendPacketToClient(std::shared_ptr<Network::Address> client, const Network::Buffer &packet)
{
    auto socket = getInstance()->_socket;

    std::thread([socket, client, packet] { socket->sendTo(client, packet); }).detach();
}

void ClientManager::sendPacketToAllClients(const Network::Buffer &packet)
{
    auto instance = getInstance();
    auto clients = instance->_clients;
    auto socket = instance->_socket;

    for (auto &tmp_client : clients)
    {
        if (tmp_client.second->isConnected())
            socket->sendTo(tmp_client.second->getAddress(), packet);
    }
}

void ClientManager::sendPacketToAllClientsExcept(std::shared_ptr<Network::Address> client,
                                                 const Network::Buffer &packet)
{
    auto instance = getInstance();
    auto clients = instance->_clients;
    auto socket = instance->_socket;
    auto clientKey = client->toString().value_or("");

    for (auto &tmp_client : clients)
    {
        auto tmp_clientKey = tmp_client.second->getName().value_or("");

        if (tmp_client.second->isConnected() && tmp_clientKey != clientKey)
            socket->sendTo(tmp_client.second->getAddress(), packet);
    }
}

void ClientManager::receivePacketFromClient(std::shared_ptr<Network::Address> client, const Network::Buffer &buffer)
{
    auto &clients = getInstance()->_clients;
    auto &bannedClients = getInstance()->_bannedClients;
    auto clientName = client->toString().value_or("");
    auto ip = client->getIp().value_or("");

    if (std::find(bannedClients.begin(), bannedClients.end(), ip) != bannedClients.end())
    {
        FLAKKARI_LOG_LOG("Client " + clientName + " tried to connect but is banned");
        return;
    }

    if (clients.find(clientName) == clients.end())
        return;
    auto &tmp_client = clients[clientName];

    Protocol::Packet<Protocol::CommandId> packet;
    if (packet.deserialize(buffer))
    {
        FLAKKARI_LOG_LOG("Client " + clientName + " sent a valid packet: " + packet.to_string());
        tmp_client->_receiveQueue.push_back(packet);
        return;
    }

    FLAKKARI_LOG_WARNING("Client " + clientName + " sent an invalid packet");

    if (!tmp_client->incrementWarningCount())
        return;

    FLAKKARI_LOG_LOG("Client " + clientName + " has been banned");

    bannedClients.push_back(ip);
    FLAKKARI_LOG_LOG("Client " + clientName + " banned");
    GameManager::removeClientFromGame(tmp_client->getGameName(), tmp_client);
    clients.erase(clientName);
}

std::shared_ptr<Client> ClientManager::getClient(std::shared_ptr<Network::Address> client)
{
    return getInstance()->_clients[client->toString().value_or("")];
}

std::shared_ptr<Client> ClientManager::getClient(std::string id) { return getInstance()->_clients[id]; }

std::shared_ptr<Network::Address> ClientManager::getAddress(std::string id)
{
    return getInstance()->_clients[id]->getAddress();
}

std::shared_ptr<Client> ClientManager::operator[](std::string id) { return _clients[id]; }

} /* namespace Flakkari */
