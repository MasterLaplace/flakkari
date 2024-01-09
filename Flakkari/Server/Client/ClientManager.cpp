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

namespace Flakkari {


std::shared_ptr<ClientManager> ClientManager::_instance = nullptr;

std::shared_ptr<ClientManager> ClientManager::getInstance()
{
    if (!_instance)
        _instance = std::make_shared<ClientManager>();
    return _instance;
}

void ClientManager::addClient(std::shared_ptr<Network::Address> client)
{
    auto &clients = getInstance()->_clients;
    if (clients.find(client->toString().value_or("")) == clients.end()) {
        clients[client->toString().value_or("")] = std::make_shared<Client>(client);
        FLAKKARI_LOG_LOG("Client " + client->toString().value_or("Unknown") + " connected");
    } else
        clients[client->toString().value_or("")]->keepAlive();
}

void ClientManager::removeClient(std::shared_ptr<Network::Address> client)
{
    auto &clients = getInstance()->_clients;
    if (clients.find(client->toString().value_or("")) != clients.end()) {
        clients.erase(client->toString().value_or(""));
    }
}

void ClientManager::checkInactiveClients()
{
    auto &clients = getInstance()->_clients;
    for (auto it = clients.begin(); it != clients.end();) {
        if (!it->second->isConnected()) {
            FLAKKARI_LOG_LOG("Client " + it->first + " disconnected");
            it = clients.erase(it);
        } else {
            ++it;
        }
    }
}

std::shared_ptr<Client> ClientManager::getClient(std::shared_ptr<Network::Address> client) {
    return getInstance()->_clients[client->toString().value_or("")];
}

std::shared_ptr<Client> ClientManager::getClient(std::string ip) {
    return getInstance()->_clients[ip];
}

std::shared_ptr<Network::Address> ClientManager::getAddress(std::string ip) {
    return getInstance()->_clients[ip]->getAddress();
}

std::shared_ptr<Client> ClientManager::operator[](std::string ip) {
    return _clients[ip];
}

} /* namespace Flakkari */
