/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-06
** File description:
** NetworkIp
*/

#ifndef FLAKKARI_NETWORKIP_HPP_
#define FLAKKARI_NETWORKIP_HPP_

#include <string>

namespace Flakkari::Engine::ECS::Components::Common {

struct NetworkIp {
    std::string ip;

    NetworkIp() : ip("") {}
    NetworkIp(std::string ip) : ip(ip) {}
    NetworkIp(const NetworkIp &other) : ip(other.ip) {}

    NetworkIp &operator=(const NetworkIp &other) {
        if (this != &other)
            ip = other.ip;

        return *this;
    }

    std::size_t size() const { return ip.size(); }
};

} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !FLAKKARI_NETWORKIP_HPP_ */
