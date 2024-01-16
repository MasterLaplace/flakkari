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

namespace Flakkari::Engine::ECS::Components::Common {

struct Template {
    std::string name;

    Template() : name("") {}
    Template(const std::string &nname) : name(nname) {}
    Template(const Template &other) : name(other.name) {}

    std::size_t size() const {
        return name.size();
    }
};

} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !FLAKKARI_TEMPLATE_HPP_ */
