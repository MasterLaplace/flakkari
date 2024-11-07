/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-06
** File description:
** Tag
*/

#ifndef FLAKKARI_TAG_HPP_
#define FLAKKARI_TAG_HPP_

#include <string>

namespace Flakkari::Engine::ECS::Components::Common {

/**
 * @brief  Tag component for ECS entities that have a script attached to them
 *
 * @details This component is used to store the path to the script that will be executed
 */
struct Tag {
    std::string tag;

    Tag() : tag("") {}
    Tag(const std::string &ntag) : tag(ntag) {}
    Tag(const Tag &other) : tag(other.tag) {}

    Tag &operator=(const Tag &other)
    {
        if (this != &other)
            tag = other.tag;

        return *this;
    }

    std::size_t size() const { return tag.size(); }
};

} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !FLAKKARI_TAG_HPP_ */
