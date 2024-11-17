/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-11-11
** File description:
** Transform
*/

#ifndef FLAKKARI_3D_TRANSFORM_HPP_
#define FLAKKARI_3D_TRANSFORM_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_3D {
LPL_PACKED_START

/**
 * @brief Transform component for 3D entities
 */
struct Transform {
    Math::Vector3f _position;
    Math::Vector3f _scale;
    Math::Vector3f _rotation;

    Transform() : _position(0, 0, 0), _scale(1, 1, 1), _rotation(0, 0, 0){};
    Transform(const Math::Vector2f &position, const Math::Vector2f &scale, const Math::Vector3f &rotation)
        : _position(position), _scale(scale), _rotation(rotation){};
    Transform(const Transform &other) : _position(other._position), _scale(other._scale), _rotation(other._rotation){};

    Transform &operator=(const Transform &other)
    {
        if (this != &other)
        {
            _position = other._position;
            _scale = other._scale;
            _rotation = other._rotation;
        }

        return *this;
    }

    std::size_t size() const { return sizeof(*this); }
};

LPL_PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_3D

#endif /* !FLAKKARI_3D_TRANSFORM_HPP_ */
