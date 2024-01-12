/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
** File description:
** Movable
*/

#ifndef MOVABLE_HPP_
#define MOVABLE_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_2D {
PACKED_START

struct Movable {
    Math::Vector2f velocity; // pixels / second
    float angularVelocity; // degrees / second
    Math::Vector2f acceleration; // pixels / second^2
    float angularAcceleration; // degrees / second^2

    Movable() : velocity(0, 0), acceleration(0, 0) {};
    Movable(const Math::Vector2f &velocity, const Math::Vector2f &acceleration) : velocity(velocity), acceleration(acceleration) {};
    Movable(const Movable &other) : velocity(other.velocity), acceleration(other.acceleration) {};

    std::size_t size() const {
        return sizeof(*this);
    }
};

PACKED_END
} // namespace Game::ECS::Components::_2D

#endif /* !MOVABLE_HPP_ */
