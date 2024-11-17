/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-11-11
** File description:
** BoxCollider
*/

#ifndef FLAKKARI_BOXCOLLIDER_HPP_
#define FLAKKARI_BOXCOLLIDER_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_3D {
LPL_PACKED_START

/**
 * @brief BoxCollider component for 3D entities
 */
struct BoxCollider {
    Math::Vector3f _center;
    Math::Vector3f _size;

    BoxCollider() : _center(), _size() {}
    BoxCollider(Math::Vector3f center, Math::Vector3f size) : _center(center), _size(size) {}
    BoxCollider(const BoxCollider &other) : _center(other._center), _size(other._size) {}

    BoxCollider &operator=(const BoxCollider &other)
    {
        if (this != &other)
        {
            _center = other._center;
            _size = other._size;
        }

        return *this;
    }

    std::size_t size() const { return sizeof(*this); }
};

LPL_PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_3D

#endif /* !FLAKKARI_BOXCOLLIDER_HPP_ */
