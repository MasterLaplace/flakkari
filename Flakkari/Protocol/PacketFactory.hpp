/**************************************************************************
 * Flakkari Library v0.2.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file PacketFactory.hpp
 * @brief Flakkari::Protocol::PacketFactory class header. This class is used to
 *        create a packet from an entity. It is used to send an entity to the
 *        server or to the client.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.2.0
 * @date 2024-01-13
 **************************************************************************/


#ifndef PACKETFACTORY_HPP_
    #define PACKETFACTORY_HPP_

#include "Packet.hpp"

#include "Engine/EntityComponentSystem/Systems/Systems.hpp"

namespace Flakkari::Protocol {

class PacketFactory {
public:
    // /**
    //  * @brief Add all the commons components of an entity to a packet.
    //  *
    //  * @tparam Id  Type of the entity id.
    //  * @param packet  Packet to add the components to.
    //  * @param registry  Registry to get the components from.
    //  * @param entity  Entity to get the components from.
    //  */
    // template<typename Id>
    // static void addCommonsToPacketByEntity (
    //     Protocol::Packet<Id> &packet, Engine::ECS::Registry &registry, Engine::ECS::Entity entity
    // ) {
    //     auto child = registry.getComponents<Engine::ECS::Components::Common::Child>();
    //     auto childEntity = child[entity];

    //     if (childEntity.has_value()) {
    //         packet << Protocol::ComponentId::CHILD;
    //         packet << childEntity->size();
    //         packet << childEntity->name;
    //     }

    //     // auto evolve = registry.getComponents<Engine::ECS::Components::Common::Evolve>();
    //     // auto evolveEntity = evolve[entity];

    //     // if (evolveEntity.has_value()) {
    //     //     packet << Protocol::ComponentId::EVOLVE;
    //     //     packet << evolveEntity->size();
    //     //     packet << evolveEntity->name;
    //     // }

    //     // auto id = registry.getComponents<Engine::ECS::Components::Common::Id>();
    //     // auto idEntity = id[entity];

    //     // if (idEntity.has_value()) {
    //     //     packet << Protocol::ComponentId::ID;
    //     //     packet << idEntity->size();
    //     //     packet << idEntity->id;
    //     // }

    //     auto parent = registry.getComponents<Engine::ECS::Components::Common::Parent>();
    //     auto parentEntity = parent[entity];

    //     if (parentEntity.has_value()) {
    //         packet << Protocol::ComponentId::PARENT;
    //         packet << parentEntity->size();
    //         packet << parentEntity->entity;
    //     }

    //     auto tag = registry.getComponents<Engine::ECS::Components::Common::Tag>();
    //     auto tagEntity = tag[entity];

    //     if (tagEntity.has_value()) {
    //         packet << Protocol::ComponentId::TAG;
    //         packet << tagEntity->size();
    //         packet << tagEntity->tag;
    //     }

    //     auto name = registry.getComponents<Engine::ECS::Components::Common::Template>();
    //     auto nameEntity = name[entity];

    //     if (nameEntity.has_value()) {
    //         packet << Protocol::ComponentId::TEMPLATE;
    //         packet << nameEntity->size();
    //         packet << nameEntity->name;
    //     }
    // }

    /**
     * @brief Add all the 2D components of an entity to a packet.
     *
     * @tparam Id  Type of the entity id.
     * @param packet  Packet to add the components to.
     * @param registry  Registry to get the components from.
     * @param entity  Entity to get the components from.
     */
    template<typename Id>
    static void add2dToPacketByEntity (
        Protocol::Packet<Id> &packet, Engine::ECS::Registry &registry, Engine::ECS::Entity entity
    ) {
        auto transform = registry.getComponents<Engine::ECS::Components::_2D::Transform>()[entity];

        if (transform.has_value()) {
            packet << Protocol::ComponentId::TRANSFORM;
            packet << transform->size();
            packet << transform->position.x;
            packet << transform->position.y;
            packet << transform->rotation;
            packet << transform->scale.x;
            packet << transform->scale.y;
        }

        auto movable = registry.getComponents<Engine::ECS::Components::_2D::Movable>()[entity];

        if (movable.has_value()) {
            packet << Protocol::ComponentId::MOVABLE;
            packet << movable->size();
            packet << movable->velocity.x;
            packet << movable->velocity.y;
            packet << movable->acceleration.x;
            packet << movable->acceleration.y;
        }

        auto control = registry.getComponents<Engine::ECS::Components::_2D::Control>()[entity];

        if (control.has_value()) {
            packet << Protocol::ComponentId::CONTROL;
            packet << control->size();
            packet << control->up;
            packet << control->down;
            packet << control->left;
            packet << control->right;
            packet << control->shoot;
        }

        auto collider = registry.getComponents<Engine::ECS::Components::_2D::Collider>()[entity];

        if (collider.has_value()) {
            packet << Protocol::ComponentId::COLLIDER;
            packet << collider->size();
            packet << collider->_size.x;
            packet << collider->_size.y;
        }

        auto rigidbody = registry.getComponents<Engine::ECS::Components::_2D::RigidBody>()[entity];

        if (rigidbody.has_value()) {
            packet << Protocol::ComponentId::RIGIDBODY;
            packet << rigidbody->size();
            packet << rigidbody->mass;
            packet << rigidbody->restitution;
            packet << rigidbody->friction;
            packet << rigidbody->gravityScale;
            packet << rigidbody->isKinematic;
        }

        auto health = registry.getComponents<Engine::ECS::Components::Common::Health>()[entity];

        if (health.has_value()) {
            packet << Protocol::ComponentId::HEALTH;
            packet << health->size();
            packet << health->currentHealth;
            packet << health->maxHealth;
            packet << health->shield;
            packet << health->maxShield;
        }

        auto weapon = registry.getComponents<Engine::ECS::Components::Common::Weapon>()[entity];

        if (weapon.has_value()) {
            packet << Protocol::ComponentId::WEAPON;
            packet << weapon->size();
            packet << weapon->damage;
            packet << weapon->fireRate;
            packet << weapon->level;
        }

        auto level = registry.getComponents<Engine::ECS::Components::Common::Level>()[entity];

        if (level.has_value()) {
            packet << Protocol::ComponentId::LEVEL;
            packet << level->size();
            packet << level->level;
            packet.injectString(level->currentWeapon);
            packet << level->currentExp;
            packet << level->requiredExp;
        }

        auto spawned = registry.getComponents<Engine::ECS::Components::Common::Spawned>()[entity];

        if (spawned.has_value()) {
            packet << Protocol::ComponentId::SPAWNED;
            packet << spawned->size();
            packet << spawned->has_spawned;
        }

        auto networkEvent = registry.getComponents<Engine::ECS::Components::Common::NetworkEvent>()[entity];

        if (networkEvent.has_value()) {
            packet << Protocol::ComponentId::NETWORK_EVENT;
            packet << networkEvent->size();
            packet << networkEvent->events.size();
            for (auto &event : networkEvent->events) {
                packet << event;
            }
        }

        auto templateName = registry.getComponents<Engine::ECS::Components::Common::Template>()[entity];

        if (templateName.has_value()) {
            packet << Protocol::ComponentId::TEMPLATE;
            packet.injectString(templateName->name);
        }

        auto parent = registry.getComponents<Engine::ECS::Components::Common::Parent>()[entity];

        if (parent.has_value()) {
            packet << Protocol::ComponentId::PARENT;
            packet << parent->size();
            packet << parent->entity;
        }

        auto child = registry.getComponents<Engine::ECS::Components::Common::Child>()[entity];

        if (child.has_value()) {
            packet << Protocol::ComponentId::CHILD;
            packet << child->size();
            packet.injectString(child->name);
        }

        auto tag = registry.getComponents<Engine::ECS::Components::Common::Tag>()[entity];

        if (tag.has_value()) {
            packet << Protocol::ComponentId::TAG;
            packet << tag->size();
            packet.injectString(tag->tag);
        }
    }

    /**
     * @brief Add all the components of an entity to a packet.
     *
     * @tparam Id  Type of the entity id.
     * @param packet  Packet to add the components to.
     * @param registry  Registry to get the components from.
     * @param entity  Entity to get the components from.
     */
    template<typename Id>
    static void addComponentsToPacketByEntity (
        Protocol::Packet<Id> &packet, Engine::ECS::Registry &registry, Engine::ECS::Entity entity
    ) {
        /*_ Common Components _*/

        // addCommonsToPacketByEntity<Id>(packet, registry, entity);

        /*_ 2D Components _*/

        add2dToPacketByEntity<Id>(packet, registry, entity);
    }

    /**
     * @brief Add requirement information for Command that use components.
     * @tparam Id  Type of the entity id.
     * @param packet  Packet to add the components to.
     * @param size  Size of the packet.
     * @param sceneName  Name of the scene.
     * @param entity  Entity to get the components from.
     */
    template<typename Id>
    static void addInfoToPacket (
        Protocol::Packet<Id> &packet, std::size_t size,
        const std::string &sceneName, Engine::ECS::Entity entity
    ) {
        packet.injectString(sceneName);
        packet << entity;
    }

    struct UpdateMovement {
        Engine::ECS::Entity entity;
        Engine::ECS::Components::_2D::Transform pos;
        Engine::ECS::Components::_2D::Movable vel;
    };

    template<typename Id>
    static void addUpdateMovementToPacket (
        Protocol::Packet<Id> &packet, Engine::ECS::Entity entity,
        Engine::ECS::Components::_2D::Transform pos,
        Engine::ECS::Components::_2D::Movable vel
    ) {
        packet << entity;
        packet << pos.position.x;
        packet << pos.position.y;
        packet << pos.rotation;
        packet << pos.scale.x;
        packet << pos.scale.y;
        packet << vel.velocity.x;
        packet << vel.velocity.y;
        packet << vel.acceleration.x;
        packet << vel.acceleration.y;
    }
};

} // namespace Flakkari::Protocol

#endif /* !PACKETFACTORY_HPP_ */
