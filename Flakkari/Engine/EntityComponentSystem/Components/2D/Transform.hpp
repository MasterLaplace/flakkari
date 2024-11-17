/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-06
** File description:
** Transform
*/

#ifndef FLAKKARI_TRANSFORM_HPP_
#define FLAKKARI_TRANSFORM_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_2D {
LPL_PACKED_START

/**
 * @brief Transform component for 2D entities
 */
struct Transform {
    Math::Vector2f _position;
    Math::Vector2f _scale;
    float _rotation;

    Transform() : _position(0, 0), _scale(1, 1), _rotation(0){};
    Transform(const Math::Vector2f &position, const Math::Vector2f &scale, float rotation)
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
} // namespace Flakkari::Engine::ECS::Components::_2D

#endif /* !FLAKKARI_TRANSFORM_HPP_ */
