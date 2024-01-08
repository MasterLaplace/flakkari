/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
** File description:
** Transform
*/

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_2D {

struct Transform {
    Math::Vector2d position;
    Math::Vector2d scale;
    double rotation;

    Transform() : position(0, 0), scale(1, 1), rotation(0) {};
    Transform(const Math::Vector2d &position, const Math::Vector2d &scale, double rotation) : position(position), scale(scale), rotation(rotation) {};
    Transform(const Transform &other) : position(other.position), scale(other.scale), rotation(other.rotation) {};
};

} // namespace Game::ECS::Components::_2D

#endif /* !TRANSFORM_HPP_ */
