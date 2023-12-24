/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-24
** File description:
** Client
*/

#include "Client.hpp"

namespace Flakkari {

Client::Client(std::shared_ptr<Network::Address> address) : _address(address) {
    _lastActivity = std::chrono::steady_clock::now();
}

bool Client::isConnected(float timeout) {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - _lastActivity).count() < timeout;
}

void Client::keepAlive() {
    _lastActivity = std::chrono::steady_clock::now();
}

} /* namespace Flakkari */