/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-11-11
** File description:
** RigidBody
*/

#ifndef FLAKKARI_3D_RIGIDBODY_HPP_
#define FLAKKARI_3D_RIGIDBODY_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_3D {
LPL_PACKED_START

/**
 * @brief RigidBody represent the physical properties of a rigid body in a game engine
 */
struct RigidBody {
    float _mass;
    float _drag;
    float _angularDrag;
    bool _useGravity = true;
    bool _isKinematic = false;
    uint8_t padding : 6 = 0;

    RigidBody() : _mass(0), _drag(0), _angularDrag(0), _useGravity(false), _isKinematic(false){};
    RigidBody(const RigidBody &other)
        : _mass(other._mass), _drag(other._drag), _angularDrag(other._angularDrag), _useGravity(other._useGravity),
          _isKinematic(other._isKinematic){};
    RigidBody(float mass, float drag, float angularDrag, bool useGravity, bool isKinematic)
        : _mass(mass), _drag(drag), _angularDrag(angularDrag), _useGravity(useGravity), _isKinematic(isKinematic){};

    RigidBody &operator=(const RigidBody &other)
    {
        if (this != &other)
        {
            _mass = other._mass;
            _drag = other._drag;
            _angularDrag = other._angularDrag;
            _useGravity = other._useGravity;
            _isKinematic = other._isKinematic;
        }

        return *this;
    }

    std::size_t size() const { return sizeof(*this); }
};

LPL_PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_3D

#endif /* !FLAKKARI_3D_RIGIDBODY_HPP_ */
