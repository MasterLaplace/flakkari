/**************************************************************************
 * Flakkari Library v0.3.0
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
 * @version 0.3.0
 * @date 2024-01-13
 **************************************************************************/

#ifndef PACKETFACTORY_HPP_
#define PACKETFACTORY_HPP_

#include "config.h.in"

#include "Engine/EntityComponentSystem/Systems/Systems.hpp"
#include "Packet.hpp"

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
    template <typename Id>
    static void add2dToPacketByEntity(Packet<Id> &packet, Engine::ECS::Registry &registry,
                                      Engine::ECS::Entity entity)
    {
        auto transform = registry.getComponents<Engine::ECS::Components::_2D::Transform>()[entity];

        if (transform.has_value())
        {
            packet << ComponentId::TRANSFORM;
            packet << transform->position.vec.x;
            packet << transform->position.vec.y;
            packet << transform->rotation;
            packet << transform->scale.vec.x;
            packet << transform->scale.vec.y;
        }

        auto movable = registry.getComponents<Engine::ECS::Components::_2D::Movable>()[entity];

        if (movable.has_value())
        {
            packet << ComponentId::MOVABLE;
            packet << movable->velocity.vec.x;
            packet << movable->velocity.vec.y;
            packet << movable->acceleration.vec.x;
            packet << movable->acceleration.vec.y;
        }

        auto control = registry.getComponents<Engine::ECS::Components::_2D::Control>()[entity];

        if (control.has_value())
        {
            packet << ComponentId::CONTROL;
            packet << control->up;
            packet << control->down;
            packet << control->left;
            packet << control->right;
            packet << control->shoot;
        }

        auto collider = registry.getComponents<Engine::ECS::Components::_2D::Collider>()[entity];

        if (collider.has_value())
        {
            packet << ComponentId::COLLIDER;
            packet << collider->_size.vec.x;
            packet << collider->_size.vec.y;
        }

        auto rigidbody = registry.getComponents<Engine::ECS::Components::_2D::RigidBody>()[entity];

        if (rigidbody.has_value())
        {
            packet << ComponentId::RIGIDBODY;
            packet << rigidbody->mass;
            packet << rigidbody->restitution;
            packet << rigidbody->friction;
            packet << rigidbody->gravityScale;
            packet << rigidbody->isKinematic;
        }

        auto health = registry.getComponents<Engine::ECS::Components::Common::Health>()[entity];

        if (health.has_value())
        {
            packet << ComponentId::HEALTH;
            packet << health->currentHealth;
            packet << health->maxHealth;
            packet << health->shield;
            packet << health->maxShield;
        }

        auto weapon = registry.getComponents<Engine::ECS::Components::Common::Weapon>()[entity];

        if (weapon.has_value())
        {
            packet << ComponentId::WEAPON;
            packet << weapon->damage;
            packet << weapon->fireRate;
            packet << weapon->level;
        }

        auto level = registry.getComponents<Engine::ECS::Components::Common::Level>()[entity];

        if (level.has_value())
        {
            packet << ComponentId::LEVEL;
            packet << level->level;
            packet.injectString(level->currentWeapon);
            packet << level->currentExp;
            packet << level->requiredExp;
        }

        auto spawned = registry.getComponents<Engine::ECS::Components::Common::Spawned>()[entity];

        if (spawned.has_value())
        {
            packet << ComponentId::SPAWNED;
            packet << spawned->has_spawned;
        }

        auto networkEvent = registry.getComponents<Engine::ECS::Components::Common::NetworkEvent>()[entity];

        if (networkEvent.has_value())
        {
            packet << ComponentId::NETWORK_EVENT;
            packet << networkEvent->events.size();
            for (auto &event : networkEvent->events)
            {
                packet << event;
            }
        }

        auto templateName = registry.getComponents<Engine::ECS::Components::Common::Template>()[entity];

        if (templateName.has_value())
        {
            packet << ComponentId::TEMPLATE;
            packet.injectString(templateName->name);
        }

        auto parent = registry.getComponents<Engine::ECS::Components::Common::Parent>()[entity];

        if (parent.has_value())
        {
            packet << ComponentId::PARENT;
            packet << parent->entity;
        }

        auto child = registry.getComponents<Engine::ECS::Components::Common::Child>()[entity];

        if (child.has_value())
        {
            packet << ComponentId::CHILD;
            packet.injectString(child->name);
        }

        auto tag = registry.getComponents<Engine::ECS::Components::Common::Tag>()[entity];

        if (tag.has_value())
        {
            packet << ComponentId::TAG;
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
    template <typename Id>
    static void addComponentsToPacketByEntity(Packet<Id> &packet, Engine::ECS::Registry &registry,
                                              Engine::ECS::Entity entity)
    {
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
    template <typename Id>
    static void addInfoToPacket(Packet<Id> &packet, std::size_t size, const std::string &sceneName,
                                Engine::ECS::Entity entity)
    {
        packet.injectString(sceneName);
        packet << entity;
    }

    struct UpdateMovement {
        Engine::ECS::Entity entity;
        Engine::ECS::Components::_2D::Transform pos;
        Engine::ECS::Components::_2D::Movable vel;
    };

    template <typename Id>
    static void addUpdateMovementToPacket(Packet<Id> &packet, Engine::ECS::Entity entity,
                                          Engine::ECS::Components::_2D::Transform pos,
                                          Engine::ECS::Components::_2D::Movable vel)
    {
        packet << entity;
        packet << pos.position.vec.x;
        packet << pos.position.vec.y;
        packet << pos.rotation;
        packet << pos.scale.vec.x;
        packet << pos.scale.vec.y;
        packet << vel.velocity.vec.x;
        packet << vel.velocity.vec.y;
        packet << vel.acceleration.vec.x;
        packet << vel.acceleration.vec.y;
    }
};

} // namespace Flakkari::Protocol

#endif /* !PACKETFACTORY_HPP_ */
