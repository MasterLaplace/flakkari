/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-14
** File description:
** RigidBody
*/

#ifndef RIGIDBODY_HPP_
#define RIGIDBODY_HPP_

#include "../../../Math/Vector.hpp"

namespace Flakkari::Engine::ECS::Components::_2D {
PACKED_START

/**
 * @brief RigidBody represent the physical properties of a rigid body in a game engine
 *
 */
struct RigidBody {
    float mass;
    float restitution;
    float friction;
    float gravityScale;
    bool isGravityAffected = true;
    bool isKinematic = false;

    RigidBody() : mass(0), restitution(0), friction(0), gravityScale(0), isGravityAffected(false), isKinematic(false){};
    RigidBody(const RigidBody &other)
        : mass(other.mass), restitution(other.restitution), friction(other.friction), gravityScale(other.gravityScale),
          isGravityAffected(other.isGravityAffected), isKinematic(other.isKinematic){};
    RigidBody(float mass, float restitution, float friction, float gravityScale)
        : mass(mass), restitution(restitution), friction(friction), gravityScale(gravityScale), isGravityAffected(true),
          isKinematic(false){};

    std::size_t size() const { return sizeof(*this); }
};

PACKED_END
} // namespace Flakkari::Engine::ECS::Components::_2D

#endif /* !RIGIDBODY_HPP_ */
