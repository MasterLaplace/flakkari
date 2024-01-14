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
        auto transform = registry.getComponents<Engine::ECS::Components::_2D::Transform>();
        auto pos = transform[entity];

        if (pos.has_value()) {
            packet << Protocol::ComponentId::TRANSFORM;
            packet << pos->size();
            packet << pos->position.x;
            packet << pos->position.y;
            packet << pos->rotation;
            packet << pos->scale.x;
            packet << pos->scale.y;
        }

        auto movable = registry.getComponents<Engine::ECS::Components::_2D::Movable>();
        auto vel = movable[entity];

        if (vel.has_value()) {
            packet << Protocol::ComponentId::MOVABLE;
            packet << vel->size();
            packet << vel->velocity.x;
            packet << vel->velocity.y;
            packet << vel->acceleration.x;
            packet << vel->acceleration.y;
        }

        auto control = registry.getComponents<Engine::ECS::Components::_2D::Control>();
        auto ctrl = control[entity];

        if (ctrl.has_value()) {
            packet << Protocol::ComponentId::CONTROL;
            packet << ctrl->size();
            packet << ctrl->up;
            packet << ctrl->down;
            packet << ctrl->left;
            packet << ctrl->right;
            packet << ctrl->shoot;
        }

        auto collider = registry.getComponents<Engine::ECS::Components::_2D::Collider>();
        auto col = collider[entity];

        if (col.has_value()) {
            packet << Protocol::ComponentId::COLLIDER;
            packet << col->size();
            packet << col->_size.x;
            packet << col->_size.y;
        }

        auto rigidbody = registry.getComponents<Engine::ECS::Components::_2D::RigidBody>();
        auto rb = rigidbody[entity];

        if (rb.has_value()) {
            packet << Protocol::ComponentId::RIGIDBODY;
            packet << rb->size();
            packet << rb->mass;
            packet << rb->restitution;
            packet << rb->friction;
            packet << rb->gravityScale;
            packet << rb->isKinematic;
        }

        auto health = registry.getComponents<Engine::ECS::Components::Common::Health>();
        auto hp = health[entity];

        if (hp.has_value()) {
            packet << Protocol::ComponentId::HEALTH;
            packet << hp->size();
            packet << hp->currentHealth;
            packet << hp->maxHealth;
            packet << hp->shield;
            packet << hp->maxShield;
        }

        auto weapon = registry.getComponents<Engine::ECS::Components::Common::Weapon>();
        auto wp = weapon[entity];

        if (wp.has_value()) {
            packet << Protocol::ComponentId::WEAPON;
            packet << wp->size();
            packet << wp->damage;
            packet << wp->fireRate;
            packet << wp->level;
        }

        auto level = registry.getComponents<Engine::ECS::Components::Common::Level>();
        auto lvl = level[entity];

        if (lvl.has_value()) {
            packet << Protocol::ComponentId::LEVEL;
            packet << lvl->size();
            packet << lvl->level;
            packet.injectString(std::string(lvl->currentWeapon));
            packet << lvl->currentExp;
            packet << lvl->requiredExp;
        }

        auto spawned = registry.getComponents<Engine::ECS::Components::Common::Spawned>();
        auto sp = spawned[entity];

        if (sp.has_value()) {
            packet << Protocol::ComponentId::SPAWNED;
            packet << sp->size();
            packet << sp->has_spawned;
        }

        auto networkEvent = registry.getComponents<Engine::ECS::Components::Common::NetworkEvent>();
        auto netEvent = networkEvent[entity];

        if (netEvent.has_value()) {
            packet << Protocol::ComponentId::NETWORK_EVENT;
            packet << netEvent->size();
            packet << netEvent->events.size();
            for (auto &event : netEvent->events) {
                packet << event;
            }
        }

        auto networkIp = registry.getComponents<Engine::ECS::Components::Common::NetworkIp>();
        auto ip = networkIp[entity];

        if (ip.has_value()) {
            packet << Protocol::ComponentId::NETWORK_IP;
            packet.injectString(std::string(ip->ip));
        }

        auto templateName = registry.getComponents<Engine::ECS::Components::Common::Template>();
        auto name = templateName[entity];

        if (name.has_value()) {
            packet << Protocol::ComponentId::TEMPLATE;
            packet.injectString(std::string(name->name));
        }

        auto parent = registry.getComponents<Engine::ECS::Components::Common::Parent>();
        auto parentEntity = parent[entity];

        if (parentEntity.has_value()) {
            packet << Protocol::ComponentId::PARENT;
            packet << parentEntity->size();
            packet << parentEntity->entity;
        }

        auto child = registry.getComponents<Engine::ECS::Components::Common::Child>();
        auto childEntity = child[entity];

        if (childEntity.has_value()) {
            packet << Protocol::ComponentId::CHILD;
            packet << childEntity->size();
            packet.injectString(std::string(childEntity->name));
        }

        auto tag = registry.getComponents<Engine::ECS::Components::Common::Tag>();
        auto tagEntity = tag[entity];

        if (tagEntity.has_value()) {
            packet << Protocol::ComponentId::TAG;
            packet << tagEntity->size();
            packet.injectString(std::string(tagEntity->tag));
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
