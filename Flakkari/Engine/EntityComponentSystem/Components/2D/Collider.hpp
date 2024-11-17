/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-14
** File description:
** Collider
*/

#ifndef FLAKKARI_COLLIDER_HPP_
#define FLAKKARI_COLLIDER_HPP_

#include "../../../Math/Vector.hpp"
#include <string>

namespace Flakkari::Engine::ECS::Components::_2D {
LPL_PACKED_START

/**
 * @brief  Collider component for 2D entities
 */
struct Collider {
    Math::Vector2f _size;

    Collider() : _size() {}
    Collider(Math::Vector2f nsize) : _size(nsize) {}
    Collider(const Collider &other) : _size(other._size) {}

    Collider &operator=(const Collider &other)
    {
        if (this != &other)
            _size = other._size;

        return *this;
    }

    std::size_t size() const { return sizeof(_size); }
};

LPL_PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_2D

#endif /* !FLAKKARI_COLLIDER_HPP_ */
