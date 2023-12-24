/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-24
** File description:
** ClientManager
*/

#include "ClientManager.hpp"

namespace Flakkari {

void ClientManager::addClient(std::shared_ptr<Network::Address> client)
{
    if (_clients.find(client->toString().value_or("")) == _clients.end()) {
        _clients[client->toString().value_or("")] = std::make_shared<Client>(client);
        FLAKKARI_LOG_LOG("Client " + client->toString().value_or("") + " connected");
    } else
        _clients[client->toString().value_or("")]->keepAlive();
}

void ClientManager::removeClient(std::shared_ptr<Network::Address> client)
{
    if (_clients.find(client->toString().value_or("")) != _clients.end())
        _clients.erase(client->toString().value_or(""));
}

void ClientManager::checkInactiveClients()
{
    for (auto it = _clients.begin(); it != _clients.end();) {
        if (!it->second->isConnected()) {
            FLAKKARI_LOG_LOG("Client " + it->first + " disconnected");
            it = _clients.erase(it);
        } else {
            ++it;
        }
    }
}

} /* namespace Flakkari */
