/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-11
** File description:
** Control
*/

#ifndef CONTROL_HPP_
#define CONTROL_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_2D {
PACKED_START

/**
 * @brief  Control component for 2D entities (player, enemies, etc...)
 *
 * @details
 *  up:     move up (give access to the move up)
 *  down:   move down (give access to the move down)
 *  left:   move left (give access to the move left)
 *  right:  move right (give access to the move right)
 *  shoot:  shoot (give access to the shoot)
 */
struct Control {
    bool up;
    bool down;
    bool left;
    bool right;
    bool shoot;

    Control() : up(false), down(false), left(false), right(false), shoot(false) {};
    Control(bool up, bool down, bool left, bool right, bool shoot)
        : up(up), down(down), left(left), right(right), shoot(shoot) {};
    Control(const Control &other)
        : up(other.up), down(other.down), left(other.left), right(other.right), shoot(other.shoot) {};

    std::size_t size() const {
        return sizeof(*this);
    }
};

PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_2D

#endif /* !CONTROL_HPP_ */
