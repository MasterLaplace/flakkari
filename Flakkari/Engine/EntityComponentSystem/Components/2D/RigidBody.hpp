/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-14
** File description:
** RigidBody
*/

#ifndef FLAKKARI_RIGIDBODY_HPP_
#define FLAKKARI_RIGIDBODY_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_2D {
LPL_PACKED_START

/**
 * @brief RigidBody represent the physical properties of a rigid body in a game engine
 *
 */
struct RigidBody {
    float _mass;
    float _restitution;
    float _friction;
    float _gravityScale;
    bool _isGravityAffected = true;
    bool _isKinematic = false;

    RigidBody()
        : _mass(0), _restitution(0), _friction(0), _gravityScale(0), _isGravityAffected(false), _isKinematic(false){};
    RigidBody(const RigidBody &other)
        : _mass(other._mass), _restitution(other._restitution), _friction(other._friction),
          _gravityScale(other._gravityScale), _isGravityAffected(other._isGravityAffected),
          _isKinematic(other._isKinematic){};
    RigidBody(float mass, float restitution, float friction, float gravityScale)
        : _mass(mass), _restitution(restitution), _friction(friction), _gravityScale(gravityScale),
          _isGravityAffected(true), _isKinematic(false){};

    RigidBody &operator=(const RigidBody &other)
    {
        if (this != &other)
        {
            _mass = other._mass;
            _restitution = other._restitution;
            _friction = other._friction;
            _gravityScale = other._gravityScale;
            _isGravityAffected = other._isGravityAffected;
            _isKinematic = other._isKinematic;
        }

        return *this;
    }

    std::size_t size() const { return sizeof(*this); }
};

LPL_PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_2D

#endif /* !FLAKKARI_RIGIDBODY_HPP_ */
