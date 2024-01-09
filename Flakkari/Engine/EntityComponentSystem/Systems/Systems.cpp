/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
** File description:
** Systems
*/

#include "Systems.hpp"

namespace Flakkari::Engine::ECS::Systems {

void position(Registry &r, float deltaTime)
{
    if (!r.isRegistered<ECS::Components::_2D::Transform>() || !r.isRegistered<ECS::Components::_2D::Movable>())
        return;
    auto& positions = r.getComponents<ECS::Components::_2D::Transform>();
    auto& velocities = r.getComponents<ECS::Components::_2D::Movable>();

    for (Entity i(0); i < positions.size() && i < velocities.size(); ++i) {
        auto& pos = positions[i];
        auto& vel = velocities[i];

        if (pos.has_value() && vel.has_value()) {
            pos->position.x += vel->velocity.dx * deltaTime * 0.5f * vel->acceleration.x * deltaTime * deltaTime;
            pos->position.y += vel->velocity.dy * deltaTime * 0.5f * vel->acceleration.y * deltaTime * deltaTime;
            vel->velocity.dx += vel->acceleration.x * deltaTime;
            vel->velocity.dy += vel->acceleration.y * deltaTime;
        }
    }
}

} // namespace Flakkari::Engine::ECS::Systems
