/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-11
** File description:
** Control
*/

#ifndef FLAKKARI_3D_CONTROL_HPP_
#define FLAKKARI_3D_CONTROL_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_3D {
LPL_PACKED_START

/**
 * @brief  Control component for 3D entities (player, enemies, etc...)
 *
 * @details
 *  _move_up:     move up (give access to the move up)
 *  _move_down:   move down (give access to the move down)
 *  _move_left:   move left (give access to the move left)
 *  _move_right:  move right (give access to the move right)
 *  _move_front:  move front (give access to the move front)
 *  _move_back:   move back (give access to the move back)
 *  _look_up:     look up (give access to the look up)
 *  _look_down:   look down (give access to the look down)
 *  _look_left:   look left (give access to the look left)
 *  _look_right:  look right (give access to the look right)
 *  _shoot:       shoot (give access to the shoot)
 *  _padding:     padding to align the size of the struct
 */
struct Control {
    bool _move_up = false;
    bool _move_down = false;
    bool _move_left = false;
    bool _move_right = false;
    bool _move_front = false;
    bool _move_back = false;
    bool _look_up = false;
    bool _look_down = false;
    bool _look_left = false;
    bool _look_right = false;
    bool _shoot = false;
    uint8_t _padding : 5 = 0;

    Control() = default;
    Control(bool m_up, bool m_down, bool m_left, bool m_right, bool m_front, bool m_back, bool l_up, bool l_down,
            bool l_left, bool l_right, bool s)
        : _move_up(m_up), _move_down(m_down), _move_left(m_left), _move_right(m_right), _move_front(m_front),
          _move_back(m_back), _look_up(l_up), _look_down(l_down), _look_left(l_left), _look_right(l_right),
          _shoot(s) {};
    Control(const Control &other)
        : _move_up(other._move_up), _move_down(other._move_down), _move_left(other._move_left),
          _move_right(other._move_right), _move_front(other._move_front), _move_back(other._move_back),
          _look_up(other._look_up), _look_down(other._look_down), _look_left(other._look_left),
          _look_right(other._look_right), _shoot(other._shoot) {};

    Control &operator=(const Control &other)
    {
        if (this != &other)
        {
            _move_up = other._move_up;
            _move_down = other._move_down;
            _move_left = other._move_left;
            _move_right = other._move_right;
            _move_front = other._move_front;
            _move_back = other._move_back;
            _look_up = other._look_up;
            _look_down = other._look_down;
            _look_left = other._look_left;
            _look_right = other._look_right;
            _shoot = other._shoot;
        }

        return *this;
    }

    unsigned short toSerialized()
    {
        return static_cast<unsigned short>((_move_up << 0) | (_move_down << 1) | (_move_left << 2) |
                                           (_move_right << 3) | (_move_front << 4) | (_move_back << 5) |
                                           (_look_up << 6) | (_look_down << 7) | (_look_left << 8) |
                                           (_look_right << 9) | (_shoot << 10));
    }

    std::size_t size() const { return sizeof(*this); }
};

LPL_PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_3D

#endif /* !FLAKKARI_3D_CONTROL_HPP_ */
