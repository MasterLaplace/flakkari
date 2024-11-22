/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-11-11
** File description:
** Movable
*/

#ifndef FLAKKARI_3D_MOVABLE_HPP_
#define FLAKKARI_3D_MOVABLE_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_3D {
LPL_PACKED_START

/**
 * @brief Movable component for 3D entities
 */
struct Movable {
    Math::Vector3f _velocity;
    Math::Vector3f _acceleration;
    float _minSpeed;
    float _maxSpeed;

    Movable() : _velocity(0, 0), _acceleration(0, 0), _minSpeed(0), _maxSpeed(0){};
    Movable(const Math::Vector3f &velocity, const Math::Vector3f &acceleration, float minSpeed, float maxSpeed)
        : _velocity(velocity), _acceleration(acceleration), _minSpeed(minSpeed), _maxSpeed(maxSpeed){};
    Movable(const Movable &other)
        : _velocity(other._velocity), _acceleration(other._acceleration), _minSpeed(other._minSpeed),
          _maxSpeed(other._maxSpeed){};

    Movable &operator=(const Movable &other)
    {
        if (this != &other)
        {
            _velocity = other._velocity;
            _acceleration = other._acceleration;
            _minSpeed = other._minSpeed;
            _maxSpeed = other._maxSpeed;
        }

        return *this;
    }

    std::size_t size() const { return sizeof(*this); }
};

LPL_PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_3D

#endif /* !FLAKKARI_3D_MOVABLE_HPP_ */
