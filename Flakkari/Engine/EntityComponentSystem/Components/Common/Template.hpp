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

#include <nlohmann/json.hpp>
#include <string>

namespace Flakkari::Engine::ECS::Components::Common {

struct Template {
    std::string name;
    nlohmann::json content;

    Template() : name(""), content(nlohmann::json::object()) {}
    Template(const std::string &name, const nlohmann::json &content) : name(name), content(content) {}
    Template(const Template &other) : name(other.name), content(other.content) {}

    Template &operator=(const Template &other)
    {
        if (this != &other)
        {
            name = other.name;
            content = other.content;
        }

        return *this;
    }

    std::size_t size() const { return name.size(); }
};

} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !FLAKKARI_TEMPLATE_HPP_ */
