/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-14
** File description:
** NetworkEvent
*/

#ifndef NETWORKEVENT_HPP_
#define NETWORKEVENT_HPP_

#include <vector>

namespace Flakkari::Engine::ECS::Components::Common {

struct NetworkEvent {
    std::vector<unsigned short> events;

    NetworkEvent() = default;
    NetworkEvent(const NetworkEvent &other) : events(other.events){};
    NetworkEvent(const std::vector<unsigned short> &events) : events(events){};

    std::size_t size() const { return events.size() * sizeof(unsigned short); }
};

} /* namespace Flakkari::Engine::ECS::Components::Common */

#endif /* !NETWORKEVENT_HPP_ */
