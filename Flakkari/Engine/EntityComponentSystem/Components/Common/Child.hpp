/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
** File description:
** Child
*/

#ifndef CHILD_HPP_
#define CHILD_HPP_

#include <string>
#include <cstring>

#include "Network/Packed.hpp"

namespace Flakkari::Engine::ECS::Components::Common {
PACKED_START

/**
 * @brief Child component for ECS entities that have a child entity attached to them
 *
 * @details This component is used to create a child entity based on a prefab
 *          and attach it to the entity that has this component
 */
struct Child {
    const char *name;

    Child() : name("") {}
    Child(const std::string &nname) : name(nname.c_str()) {}
    Child(const char *nname) : name(nname) {}
    Child(const Child &other) : name(other.name) {}

    std::size_t size() const {
        return std::strlen(name);
    }
};

PACKED_END
} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !CHILD_HPP_ */
