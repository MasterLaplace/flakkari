/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
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

    std::size_t size() const {
        return tag.size();
    }
};

} // namespace Game::ECS::Components::Common

#endif /* !FLAKKARI_TAG_HPP_ */
