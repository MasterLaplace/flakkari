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

Client::Client(const std::shared_ptr<Network::Address> &address, const std::string &name)
    : _address(address), _gameName(name), _name(address->toString().value_or(""))
{
    _lastActivity = std::chrono::steady_clock::now();
}

Client::~Client()
{
    _isConnected = false;
    _address->setId(-1);
}

bool Client::isConnected(float timeout)
{
    if (!_isConnected)
        return false;
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - _lastActivity).count() <
           timeout;
}

void Client::keepAlive() { _lastActivity = std::chrono::steady_clock::now(); }

void Client::addPacketToHistory(const Network::Buffer &packet)
{
    if (_packetHistory.size() >= _maxPacketHistory)
        _packetHistory.erase(_packetHistory.begin());
    _packetHistory.push_back(packet);
}

bool Client::incrementWarningCount()
{
    _warningCount++;
    return _warningCount >= _maxWarningCount;
}

} /* namespace Flakkari */
