/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
** File description:
** Parent
*/

#ifndef PARENT_HPP_
#define PARENT_HPP_

#include <string>

namespace Flakkari::Engine::ECS::Components::Common {

/**
 * @brief Parent component for ECS entities that have a parent entity attached to them
 *
 * @details This component is used to store the parent entity of an entity in the ECS
 */
struct Parent {
    std::size_t entity;

    Parent() : entity(0) {}
    Parent(const std::size_t &entity) : entity(entity) {}
    Parent(const Parent &other) : entity(other.entity) {}
};

} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !PARENT_HPP_ */
