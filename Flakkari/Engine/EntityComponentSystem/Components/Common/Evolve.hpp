/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-13
** File description:
** Evolve
*/

#ifndef FLAKKARI_EVOLVE_HPP_
#define FLAKKARI_EVOLVE_HPP_

#include <string>
#include <cstring>

#include "Network/Packed.hpp"

namespace Flakkari::Engine::ECS::Components::Common {
PACKED_START

/**
 * @brief Evolve component for ECS entities that can evolve in to another Component
 *
 * @details This component is used to evolve an entity in to another entity
 *          based on a template
 */
struct Evolve {
    const char *name;

    Evolve() : name("") {}
    Evolve(const std::string &nname) : name(nname.c_str()) {}
    Evolve(const char *nname) : name(nname) {}
    Evolve(const Evolve &other) : name(other.name) {}

    std::size_t size() const {
        return std::strlen(name);
    }
};

PACKED_END
} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !FLAKKARI_EVOLVE_HPP_ */
