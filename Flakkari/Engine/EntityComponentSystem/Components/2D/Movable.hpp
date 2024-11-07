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
PACKED_START

struct Movable {
    Math::Vector2f velocity;     // pixels / second
    Math::Vector2f acceleration; // pixels / second^2

    Movable() : velocity(0, 0), acceleration(0, 0) {}
    Movable(const Math::Vector2f &velocity, const Math::Vector2f &acceleration)
        : velocity(velocity), acceleration(acceleration){};
    Movable(const Movable &other) : velocity(other.velocity), acceleration(other.acceleration){};

    Movable& operator=(const Movable& other) {
        if (this != &other)
        {
            velocity = other.velocity;
            acceleration = other.acceleration;
        }

        return *this;
    }

    std::size_t size() const { return sizeof(*this); }
};

PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_2D

#endif /* !FLAKKARI_MOVABLE_HPP_ */
