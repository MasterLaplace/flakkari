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

bool ClientManager::addClient(const std::shared_ptr<Network::Address> &client, Network::Buffer &buffer)
{
    if (this->isBanned(client))
    {
        FLAKKARI_LOG_LOG("Client " + client->toString().value_or("Unknown") + " tried to connect but is banned");
        return false;
    }

    if (_clients.find(client->toString().value_or("")) != _clients.end())
        return _clients[client->toString().value_or("")]->keepAlive(), true;

    Protocol::Packet<Protocol::CommandId> packet;
    if (!packet.deserialize(buffer))
    {
        FLAKKARI_LOG_WARNING("Client " + client->toString().value_or("Unknown") + " sent an invalid packet");
        _bannedClients.push_back(client->getIp().value());
        return false;
    }

    if (packet.header._commandId != Protocol::CommandId::REQ_CONNECT)
    {
        FLAKKARI_LOG_WARNING("Client " + client->toString().value_or("Unknown") + " sent an invalid packet");
        return false;
    }

    std::string name = packet.extractString();
    _clients[client->toString().value_or("")] = std::make_shared<Client>(client, name);

    FLAKKARI_LOG_LOG("Client " + client->toString().value_or("Unknown") + " connected");
    GameManager::GetInstance().addClientToGame(name, _clients[client->toString().value_or("")]);
    GameManager::UnlockInstance();
    return true;
}

void ClientManager::removeClient(const std::shared_ptr<Network::Address> &client)
{
    if (_clients.find(client->toString().value_or("")) == _clients.end())
        return;

    auto _client = _clients[client->toString().value_or("")];

    GameManager::GetInstance().removeClientFromGame(_client->getGameName(), _client);
    GameManager::UnlockInstance();
    _clients.erase(client->toString().value_or(""));
}

void ClientManager::banClient(const std::shared_ptr<Network::Address> &client)
{
    if (_clients.find(client->toString().value_or("")) == _clients.end())
        return;

    _bannedClients.push_back(client->getIp().value());

    FLAKKARI_LOG_LOG("Client " + client->toString().value_or("Unknown") + " banned");
    auto _client = _clients[client->toString().value_or("")];
    GameManager::GetInstance().removeClientFromGame(_client->getGameName(), _client);
    GameManager::UnlockInstance();
    _clients.erase(client->toString().value_or(""));
}

bool ClientManager::isBanned(const std::shared_ptr<Network::Address> &client)
{
    return std::find(_bannedClients.begin(), _bannedClients.end(), client->getIp().value_or("")) !=
           _bannedClients.end();
}

void ClientManager::checkInactiveClients()
{
    for (auto it = _clients.begin(); it != _clients.end();)
    {
        if (!it->second->isConnected())
        {
            FLAKKARI_LOG_LOG("Client " + it->first + " disconnected");
            it = _clients.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void ClientManager::sendPacketToClient(std::shared_ptr<Network::Address> client, const Network::Buffer &packet)
{
    std::thread([this, client, packet] { _socket->sendTo(client, packet); }).detach();
}

void ClientManager::sendPacketToAllClients(const Network::Buffer &packet)
{
    for (auto &tmp_client : _clients)
    {
        if (tmp_client.second->isConnected())
            _socket->sendTo(tmp_client.second->getAddress(), packet);
    }
}

void ClientManager::sendPacketToAllClientsExcept(const std::shared_ptr<Network::Address> &client,
                                                 const Network::Buffer &packet)
{
    auto clientKey = client->toString().value_or("");

    for (auto &tmp_client : _clients)
    {
        auto tmp_clientKey = tmp_client.second->getName().value_or("");

        if (tmp_client.second->isConnected() && tmp_clientKey != clientKey)
            _socket->sendTo(tmp_client.second->getAddress(), packet);
    }
}

void ClientManager::receivePacketFromClient(const std::shared_ptr<Network::Address> &client,
                                            const Network::Buffer &buffer)
{
    auto clientName = client->toString().value_or("");
    auto ip = client->getIp().value_or("");

    if (std::find(_bannedClients.begin(), _bannedClients.end(), ip) != _bannedClients.end())
    {
        FLAKKARI_LOG_LOG("Client " + clientName + " tried to connect but is banned");
        return;
    }

    if (_clients.find(clientName) == _clients.end())
        return;
    auto &tmp_client = _clients[clientName];

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

    _bannedClients.push_back(ip);
    FLAKKARI_LOG_LOG("Client " + clientName + " banned");
    GameManager::GetInstance().removeClientFromGame(tmp_client->getGameName(), tmp_client);
    GameManager::UnlockInstance();
    _clients.erase(clientName);
}

std::shared_ptr<Client> ClientManager::getClient(const std::shared_ptr<Network::Address> &client)
{
    return _clients[client->toString().value_or("")];
}

std::shared_ptr<Client> ClientManager::getClient(const std::string &id) { return _clients[id]; }

std::shared_ptr<Network::Address> ClientManager::getAddress(const std::string &id)
{
    return _clients[id]->getAddress();
}

std::shared_ptr<Client> ClientManager::operator[](const std::string &id) { return _clients[id]; }

} /* namespace Flakkari */
