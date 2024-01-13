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
     * @brief Add all the commons components of an entity to a packet.
     *
     * @tparam Id  Type of the entity id.
     * @param packet  Packet to add the components to.
     * @param registry  Registry to get the components from.
     * @param entity  Entity to get the components from.
     */
    template<typename Id>
    static void addCommonsToPacketByEntity (
        Protocol::API::Packet<Id> &packet, Engine::ECS::Registry &registry, Engine::ECS::Entity entity
    ) {
        auto child = registry.getComponents<Engine::ECS::Components::Common::Child>();
        auto childEntity = child[entity];

        if (childEntity.has_value()) {
            packet << Protocol::API::ComponentId::CHILD;
            packet << childEntity->size();
            packet << childEntity->name;
        }

        // auto evolve = registry.getComponents<Engine::ECS::Components::Common::Evolve>();
        // auto evolveEntity = evolve[entity];

        // if (evolveEntity.has_value()) {
        //     packet << Protocol::API::ComponentId::EVOLVE;
        //     packet << evolveEntity->size();
        //     packet << evolveEntity->name;
        // }

        // auto id = registry.getComponents<Engine::ECS::Components::Common::Id>();
        // auto idEntity = id[entity];

        // if (idEntity.has_value()) {
        //     packet << Protocol::API::ComponentId::ID;
        //     packet << idEntity->size();
        //     packet << idEntity->id;
        // }

        auto parent = registry.getComponents<Engine::ECS::Components::Common::Parent>();
        auto parentEntity = parent[entity];

        if (parentEntity.has_value()) {
            packet << Protocol::API::ComponentId::PARENT;
            packet << parentEntity->size();
            packet << parentEntity->entity;
        }

        auto tag = registry.getComponents<Engine::ECS::Components::Common::Tag>();
        auto tagEntity = tag[entity];

        if (tagEntity.has_value()) {
            packet << Protocol::API::ComponentId::TAG;
            packet << tagEntity->size();
            packet << tagEntity->tag;
        }

        auto name = registry.getComponents<Engine::ECS::Components::Common::Template>();
        auto nameEntity = name[entity];

        if (nameEntity.has_value()) {
            packet << Protocol::API::ComponentId::TEMPLATE;
            packet << nameEntity->size();
            packet << nameEntity->name;
        }
    }

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
        Protocol::API::Packet<Id> &packet, Engine::ECS::Registry &registry, Engine::ECS::Entity entity
    ) {
        auto transform = registry.getComponents<Engine::ECS::Components::_2D::Transform>();
        auto pos = transform[entity];

        if (pos.has_value()) {
            packet << Protocol::API::ComponentId::TRANSFORM;
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
            packet << Protocol::API::ComponentId::MOVABLE;
            packet << vel->size();
            packet << vel->velocity.x;
            packet << vel->velocity.y;
            packet << vel->acceleration.x;
            packet << vel->acceleration.y;
        }

        auto control = registry.getComponents<Engine::ECS::Components::_2D::Control>();
        auto ctrl = control[entity];

        if (ctrl.has_value()) {
            packet << Protocol::API::ComponentId::CONTROL;
            packet << ctrl->size();
            packet << ctrl->up;
            packet << ctrl->down;
            packet << ctrl->left;
            packet << ctrl->right;
            packet << ctrl->shoot;
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
        Protocol::API::Packet<Id> &packet, Engine::ECS::Registry &registry, Engine::ECS::Entity entity
    ) {
        /*_ Common Components _*/

        addCommonsToPacketByEntity<Id>(packet, registry, entity);

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
        Protocol::API::Packet<Id> &packet, std::size_t size,
        const std::string &sceneName, Engine::ECS::Entity entity
    ) {
        packet << size;
        packet << sceneName.c_str();
        packet << entity;
    }
};

} // namespace Flakkari::Protocol

#endif /* !PACKETFACTORY_HPP_ */
