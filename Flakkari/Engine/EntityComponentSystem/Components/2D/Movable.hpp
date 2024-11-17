/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-06
** File description:
** Movable
*/

#ifndef FLAKKARI_MOVABLE_HPP_
#define FLAKKARI_MOVABLE_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_2D {
LPL_PACKED_START

/**
 * @brief Movable component for 2D entities
 */
struct Movable {
    Math::Vector2f _velocity;     // pixels / second
    Math::Vector2f _acceleration; // pixels / second^2

    Movable() : _velocity(0, 0), _acceleration(0, 0) {}
    Movable(const Math::Vector2f &velocity, const Math::Vector2f &acceleration)
        : _velocity(velocity), _acceleration(acceleration){};
    Movable(const Movable &other) : _velocity(other._velocity), _acceleration(other._acceleration){};

    Movable &operator=(const Movable &other)
    {
        if (this != &other)
        {
            _velocity = other._velocity;
            _acceleration = other._acceleration;
        }

        return *this;
    }

    std::size_t size() const { return sizeof(*this); }
};

LPL_PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_2D

#endif /* !FLAKKARI_MOVABLE_HPP_ */
