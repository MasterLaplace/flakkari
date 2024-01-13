/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-13
** File description:
** Template
*/

#ifndef FLAKKARI_TEMPLATE_HPP_
#define FLAKKARI_TEMPLATE_HPP_

#include <string>
#include <cstring>

#include "Network/Packed.hpp"

namespace Flakkari::Engine::ECS::Components::Common {
PACKED_START

struct Template {
    const char *name;

    Template() : name("") {}
    Template(const std::string &nname) : name(nname.c_str()) {}
    Template(const char *nname) : name(nname) {}
    Template(const Template &other) : name(other.name) {}

    std::size_t size() const {
        return std::strlen(name);
    }
};

PACKED_END
} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !FLAKKARI_TEMPLATE_HPP_ */
