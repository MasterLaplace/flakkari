/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-11-17
** File description:
** Factory
*/

#include "Factory.hpp"

namespace Flakkari::Engine::ECS::Factory {

Entity createEntityFromTemplate(Registry &registry, const nl_template &templateJson)
{
    Entity entity = registry.spawn_entity();

    RegistryEntityByTemplate(registry, entity, templateJson);

    return entity;
}

static const nl_template &getTemplateByName(const nl_templates &templates, const std::string &name)
{
    for (auto &template_ : templates.items())
    {
        if (template_.value().begin().key() == name)
            return template_.value().begin().value();
    }

    throw std::runtime_error("Template not found");
}

void RegistryEntityByTemplate(Registry &registry, Entity entity, const nl_template &templateJson,
                              const nl_templates &templates)
{
    for (auto &component : templateJson.items())
    {
        auto componentName = component.key();
        auto componentContent = component.value();

        //*_ 2D Components _*//

        if (componentName == "Collider")
        {
            registry.registerComponent<Engine::ECS::Components::_2D::Collider>();
            Engine::ECS::Components::_2D::Collider collider;
            collider._size = Engine::Math::Vector2f(componentContent["size"]["x"], componentContent["size"]["y"]);
            registry.add_component<Engine::ECS::Components::_2D::Collider>(entity, std::move(collider));
            continue;
        }

        if (componentName == "Control")
        {
            registry.registerComponent<Engine::ECS::Components::_2D::Control>();
            Engine::ECS::Components::_2D::Control control;
            control._up = componentContent["up"];
            control._down = componentContent["down"];
            control._left = componentContent["left"];
            control._right = componentContent["right"];
            control._shoot = componentContent["shoot"];
            registry.add_component<Engine::ECS::Components::_2D::Control>(entity, std::move(control));
            continue;
        }

        if (componentName == "Movable")
        {
            registry.registerComponent<Engine::ECS::Components::_2D::Movable>();
            Engine::ECS::Components::_2D::Movable movable;
            movable._velocity =
                Engine::Math::Vector2f(componentContent["velocity"]["x"], componentContent["velocity"]["y"]);
            movable._acceleration =
                Engine::Math::Vector2f(componentContent["acceleration"]["x"], componentContent["acceleration"]["y"]);
            registry.add_component<Engine::ECS::Components::_2D::Movable>(entity, std::move(movable));
            continue;
        }

        if (componentName == "RigidBody")
        {
            registry.registerComponent<Engine::ECS::Components::_2D::RigidBody>();
            Engine::ECS::Components::_2D::RigidBody rigidBody;
            rigidBody._mass = componentContent["mass"];
            rigidBody._restitution = componentContent["restitution"];
            rigidBody._friction = componentContent["friction"];
            rigidBody._gravityScale = componentContent["gravityScale"];
            rigidBody._isGravityAffected = componentContent["isGravityAffected"];
            rigidBody._isKinematic = componentContent["isKinematic"];
            registry.add_component<Engine::ECS::Components::_2D::RigidBody>(entity, std::move(rigidBody));
            continue;
        }

        if (componentName == "Transform")
        {
            registry.registerComponent<Engine::ECS::Components::_2D::Transform>();
            Engine::ECS::Components::_2D::Transform transform;
            transform._position =
                Engine::Math::Vector2f(componentContent["position"]["x"], componentContent["position"]["y"]);
            transform._rotation = componentContent["rotation"];
            transform._scale = Engine::Math::Vector2f(componentContent["scale"]["x"], componentContent["scale"]["y"]);
            registry.add_component<Engine::ECS::Components::_2D::Transform>(entity, std::move(transform));
            continue;
        }

        //*_ 3D Components _*//

        if (componentName == "BoxCollider")
        {
            registry.registerComponent<Engine::ECS::Components::_3D::BoxCollider>();
            Engine::ECS::Components::_3D::BoxCollider boxCollider;
            boxCollider._size = Engine::Math::Vector3f(componentContent["size"]["x"], componentContent["size"]["y"],
                                                       componentContent["size"]["z"]);
            boxCollider._center = Engine::Math::Vector3f(
                componentContent["center"]["x"], componentContent["center"]["y"], componentContent["center"]["z"]);
            registry.add_component<Engine::ECS::Components::_3D::BoxCollider>(entity, std::move(boxCollider));
            continue;
        }

        if (componentName == "3D_Control")
        {
            registry.registerComponent<Engine::ECS::Components::_3D::Control>();
            Engine::ECS::Components::_3D::Control control;
            control._move_up = componentContent["move_up"];
            control._move_down = componentContent["move_down"];
            control._move_left = componentContent["move_left"];
            control._move_right = componentContent["move_right"];
            control._move_front = componentContent["move_front"];
            control._move_back = componentContent["move_back"];
            control._look_up = componentContent["look_up"];
            control._look_down = componentContent["look_down"];
            control._look_left = componentContent["look_left"];
            control._look_right = componentContent["look_right"];
            control._shoot = componentContent["shoot"];
            registry.add_component<Engine::ECS::Components::_3D::Control>(entity, std::move(control));
            continue;
        }

        if (componentName == "3D_Movable")
        {
            registry.registerComponent<Engine::ECS::Components::_3D::Movable>();
            Engine::ECS::Components::_3D::Movable movable;
            movable._velocity =
                Engine::Math::Vector3f(componentContent["velocity"]["x"], componentContent["velocity"]["y"],
                                       componentContent["velocity"]["z"]);
            movable._acceleration =
                Engine::Math::Vector3f(componentContent["acceleration"]["x"], componentContent["acceleration"]["y"],
                                       componentContent["acceleration"]["z"]);
            movable._minSpeed = componentContent["minSpeed"];
            movable._maxSpeed = componentContent["maxSpeed"];
            registry.add_component<Engine::ECS::Components::_3D::Movable>(entity, std::move(movable));
            continue;
        }

        if (componentName == "RigidBody")
        {
            registry.registerComponent<Engine::ECS::Components::_3D::RigidBody>();
            Engine::ECS::Components::_3D::RigidBody rigidBody;
            rigidBody._mass = componentContent["mass"];
            rigidBody._drag = componentContent["drag"];
            rigidBody._angularDrag = componentContent["angularDrag"];
            rigidBody._useGravity = componentContent["useGravity"];
            rigidBody._isKinematic = componentContent["isKinematic"];
            registry.add_component<Engine::ECS::Components::_3D::RigidBody>(entity, std::move(rigidBody));
            continue;
        }

        if (componentName == "SphereCollider")
        {
            registry.registerComponent<Engine::ECS::Components::_3D::SphereCollider>();
            Engine::ECS::Components::_3D::SphereCollider sphereCollider;
            sphereCollider._center = Engine::Math::Vector3f(
                componentContent["center"]["x"], componentContent["center"]["y"], componentContent["center"]["z"]);
            sphereCollider._radius = componentContent["radius"];
            registry.add_component<Engine::ECS::Components::_3D::SphereCollider>(entity, std::move(sphereCollider));
            continue;
        }

        if (componentName == "3D_Transform")
        {
            registry.registerComponent<Engine::ECS::Components::_3D::Transform>();
            Engine::ECS::Components::_3D::Transform transform;
            transform._position =
                Engine::Math::Vector3f(componentContent["position"]["x"], componentContent["position"]["y"],
                                       componentContent["position"]["z"]);
            transform._rotation =
                Engine::Math::Vector3d(componentContent["rotation"]["x"], componentContent["rotation"]["y"],
                                       componentContent["rotation"]["z"]);
            transform._scale = Engine::Math::Vector3f(componentContent["scale"]["x"], componentContent["scale"]["y"],
                                                      componentContent["scale"]["z"]);
            registry.add_component<Engine::ECS::Components::_3D::Transform>(entity, std::move(transform));
            continue;
        }

        //*_ Common Components _*//

        if (componentName == "Child")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Child>();
            Engine::ECS::Components::Common::Child child(componentContent);
            registry.add_component<Engine::ECS::Components::Common::Child>(entity, std::move(child));
            continue;
        }

        if (componentName == "Evolve")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Evolve>();
            Engine::ECS::Components::Common::Evolve evolve(componentContent);
            registry.add_component<Engine::ECS::Components::Common::Evolve>(entity, std::move(evolve));
            continue;
        }

        if (componentName == "Health")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Health>();
            Engine::ECS::Components::Common::Health health;
            health.maxHealth = componentContent["maxHealth"];
            health.currentHealth = componentContent["currentHealth"];
            health.maxShield = componentContent["maxShield"];
            health.shield = componentContent["shield"];
            registry.add_component<Engine::ECS::Components::Common::Health>(entity, std::move(health));
            continue;
        }

        if (componentName == "Parent")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Parent>();
            Engine::ECS::Components::Common::Parent parent(componentContent);
            registry.add_component<Engine::ECS::Components::Common::Parent>(entity, std::move(parent));
            continue;
        }

        if (componentName == "Level")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Level>();
            Engine::ECS::Components::Common::Level level;
            level.level = componentContent["level"];
            level.currentExp = componentContent["currentExp"];
            level.requiredExp = componentContent["requiredExp"];
            level.currentWeapon = componentContent["currentWeapon"].get<std::string>().c_str();
            registry.add_component<Engine::ECS::Components::Common::Level>(entity, std::move(level));
            continue;
        }

        if (componentName == "Spawned")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Spawned>();
            Engine::ECS::Components::Common::Spawned spawned(componentContent);
            registry.add_component<Engine::ECS::Components::Common::Spawned>(entity, std::move(spawned));
            continue;
        }

        if (componentName == "Tag")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Tag>();
            Engine::ECS::Components::Common::Tag tag(componentContent);
            registry.add_component<Engine::ECS::Components::Common::Tag>(entity, std::move(tag));
            continue;
        }

        if (componentName == "Template")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Template>();
            Engine::ECS::Components::Common::Template template_(componentContent,
                                                                getTemplateByName(templates, componentContent));
            registry.add_component<Engine::ECS::Components::Common::Template>(entity, std::move(template_));
            continue;
        }

        if (componentName == "Timer")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Timer>();
            Engine::ECS::Components::Common::Timer timer;
            timer.maxTime = componentContent["maxTime"];
            registry.add_component<Engine::ECS::Components::Common::Timer>(entity, std::move(timer));
            continue;
        }

        if (componentName == "Weapon")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Weapon>();
            Engine::ECS::Components::Common::Weapon weapon;
            weapon.minDamage = componentContent["minDamage"];
            weapon.maxDamage = componentContent["maxDamage"];
            weapon.chargeMaxTime = componentContent["chargeMaxTime"];
            weapon.fireRate = componentContent["fireRate"];
            weapon.level = componentContent["level"];
            registry.add_component<Engine::ECS::Components::Common::Weapon>(entity, std::move(weapon));
            continue;
        }
    }
}

} // namespace Flakkari::Engine::ECS::Factory
