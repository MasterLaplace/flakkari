/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
** File description:
** Systems
*/

#include "Systems.hpp"
#include "Protocol/Events.hpp"

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

void update_control(Registry &r)
{
    if (!r.isRegistered<ECS::Components::_2D::Movable>() || !r.isRegistered<ECS::Components::Common::NetworkEvent>()
        || !r.isRegistered<ECS::Components::_2D::Transform>())
        return;
    auto& positions = r.getComponents<ECS::Components::_2D::Transform>();
    auto& velocities = r.getComponents<ECS::Components::_2D::Movable>();
    auto& networkEvent = r.getComponents<Engine::ECS::Components::Common::NetworkEvent>();

    for (Entity i(0); i < velocities.size() && i < positions.size(); ++i) {
        auto& pos = positions[i];
        auto& vel = velocities[i];
        auto& net = networkEvent[i];

        if (!net.has_value() || !vel.has_value())
            continue;
        if (net->events.size() < int(Protocol::EventId::MOVE_UP))
            continue;
        if (net->events[int(Protocol::EventId::MOVE_UP)] == int(Protocol::EventState::PRESSED))
            vel->velocity.dy = -1;

        if (net->events.size() < int(Protocol::EventId::MOVE_DOWN))
            continue;
        if (net->events[int(Protocol::EventId::MOVE_DOWN)] == int(Protocol::EventState::PRESSED))
            vel->velocity.dy = 1;

        if (net->events.size() < int(Protocol::EventId::MOVE_LEFT))
            continue;
        if (net->events[int(Protocol::EventId::MOVE_LEFT)] == int(Protocol::EventState::PRESSED))
            vel->velocity.dx = -1;

        if (net->events.size() < int(Protocol::EventId::MOVE_RIGHT))
            continue;
        if (net->events[int(Protocol::EventId::MOVE_RIGHT)] == int(Protocol::EventState::PRESSED))
            vel->velocity.dx = 1;
    }
}

} // namespace Flakkari::Engine::ECS::Systems
