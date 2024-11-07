/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-14
** File description:
** Collider
*/

#ifndef COLLIDER_HPP_
#define COLLIDER_HPP_

#include "../../../Math/Vector.hpp"
#include <string>

#include "Network/Packed.hpp"

namespace Flakkari::Engine::ECS::Components::_2D {
PACKED_START

/**
 * @brief  Collider component for ECS entities that have a script attached to them
 *
 * @details This component is used to store the path to the script that will be executed
 */
struct Collider {
    Math::Vector2f _size;

    Collider() : _size() {}
    Collider(Math::Vector2f nsize) : _size(nsize) {}
    Collider(const Collider &other) : _size(other._size) {}

    std::size_t size() const { return sizeof(_size); }
};

PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_2D

#endif /* !COLLIDER_HPP_ */
