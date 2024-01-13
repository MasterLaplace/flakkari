/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
** File description:
** Transform
*/

#ifndef FLAKKARI_TRANSFORM_HPP_
#define FLAKKARI_TRANSFORM_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_2D {
PACKED_START

struct Transform {
    Math::Vector2f position;
    Math::Vector2f scale;
    float rotation;

    Transform() : position(0, 0), scale(1, 1), rotation(0) {};
    Transform(const Math::Vector2f &position, const Math::Vector2f &scale, float rotation) : position(position), scale(scale), rotation(rotation) {};
    Transform(const Transform &other) : position(other.position), scale(other.scale), rotation(other.rotation) {};

    std::size_t size() const {
        return sizeof(*this);
    }
};

PACKED_END
} // namespace Game::ECS::Components::_2D

#endif /* !FLAKKARI_TRANSFORM_HPP_ */
