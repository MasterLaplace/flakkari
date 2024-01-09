/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
** File description:
** Tag
*/

#ifndef TAG_HPP_
#define TAG_HPP_

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
    Tag(const std::string &tag) : tag(tag) {}
    Tag(const Tag &other) : tag(other.tag) {}
};

} // namespace Game::ECS::Components::Common

#endif /* !TAG_HPP_ */
