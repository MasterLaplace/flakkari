/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-05
** File description:
** Registry
*/

#include "Registry.hpp"

namespace Flakkari::Engine::ECS {

using entity_type = Registry::entity_type;

entity_type Registry::spawn_entity()
{
    if (!_deadEntities.empty()) {
        entity_type e = _deadEntities.front();
        _deadEntities.pop();
        return e;
    }
    if (_nextEntity < SIZE_MAX)
        return Entity(_nextEntity++);
    throw std::runtime_error("No more available entities to spawn.");
}

entity_type Registry::entity_from_index(std::size_t idx) {
    return Entity(idx);
}

void Registry::kill_entity(const entity_type &e)
{
    for (auto &pair : _eraseFunctions)
        pair.second(*this, e);

    _deadEntities.push(e);
}

void Registry::run_systems()
{
    for (auto &system : _systems)
        system(*this);
}

} // namespace Flakkari::Engine::ECS
