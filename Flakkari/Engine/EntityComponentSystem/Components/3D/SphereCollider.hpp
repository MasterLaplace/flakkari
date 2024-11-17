/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-11-11
** File description:
** SphereCollider
*/

#ifndef FLAKKARI_SPHERECOLLIDER_HPP_
#define FLAKKARI_SPHERECOLLIDER_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_3D {
LPL_PACKED_START

/**
 * @brief SphereCollider component for 3D entities
 */
struct SphereCollider {
    Math::Vector3f _center;
    float _radius;

    SphereCollider() : _center(), _radius() {}
    SphereCollider(Math::Vector3f center, float radius) : _center(center), _radius(radius) {}
    SphereCollider(const SphereCollider &other) : _center(other._center), _radius(other._radius) {}

    SphereCollider &operator=(const SphereCollider &other)
    {
        if (this != &other)
        {
            _center = other._center;
            _radius = other._radius;
        }

        return *this;
    }

    std::size_t size() const { return sizeof(*this); }
};

LPL_PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_3D

#endif /* !FLAKKARI_SPHERECOLLIDER_HPP_ */
