/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-13
** File description:
** Evolve
*/

#ifndef FLAKKARI_EVOLVE_HPP_
#define FLAKKARI_EVOLVE_HPP_

#include <string>

namespace Flakkari::Engine::ECS::Components::Common {

/**
 * @brief Evolve component for ECS entities that can evolve in to another Component
 *
 * @details This component is used to evolve an entity in to another entity
 *          based on a template
 */
struct Evolve {
    std::string name;

    Evolve() : name("") {}
    Evolve(const std::string &nname) : name(nname) {}
    Evolve(const Evolve &other) : name(other.name) {}

    Evolve &operator=(const Evolve &other) {
        if (this != &other)
            name = other.name;

        return *this;
    }

    std::size_t size() const { return name.size(); }
};

} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !FLAKKARI_EVOLVE_HPP_ */
