/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-06
** File description:
** Id
*/

#ifndef ID_HPP_
#define ID_HPP_

#include <cstdint>

#include "Network/Packed.hpp"

namespace Engine::ECS::Components::Common {
PACKED_START

struct Id {
    std::size_t id;

    Id() : id(0) {}
    Id(const Id &other) : id(other.id) {}
    Id(std::size_t id) : id(id) {}

    std::size_t size() const {
        return sizeof(id);
    }
};

PACKED_END
} // namespace Engine::ECS::Components::Common

#endif /* !ID_HPP_ */
