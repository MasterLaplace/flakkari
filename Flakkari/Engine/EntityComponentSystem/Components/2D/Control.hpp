/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-11
** File description:
** Control
*/

#ifndef FLAKKARI_CONTROL_HPP_
#define FLAKKARI_CONTROL_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_2D {
LPL_PACKED_START

/**
 * @brief  Control component for 2D entities (player, enemies, etc...)
 *
 * @details
 *  _up:     move up (give access to the move up)
 *  _down:   move down (give access to the move down)
 *  _left:   move left (give access to the move left)
 *  _right:  move right (give access to the move right)
 *  _shoot:  shoot (give access to the shoot)
 */
struct Control {
    bool _up;
    bool _down;
    bool _left;
    bool _right;
    bool _shoot;

    Control() : _up(false), _down(false), _left(false), _right(false), _shoot(false) {};
    Control(bool up, bool down, bool left, bool right, bool shoot)
        : _up(up), _down(down), _left(left), _right(right), _shoot(shoot) {};
    Control(const Control &other)
        : _up(other._up), _down(other._down), _left(other._left), _right(other._right), _shoot(other._shoot) {};

    Control &operator=(const Control &other)
    {
        if (this != &other)
        {
            _up = other._up;
            _down = other._down;
            _left = other._left;
            _right = other._right;
            _shoot = other._shoot;
        }

        return *this;
    }

    std::size_t size() const { return sizeof(*this); }
};

LPL_PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_2D

#endif /* !FLAKKARI_CONTROL_HPP_ */
