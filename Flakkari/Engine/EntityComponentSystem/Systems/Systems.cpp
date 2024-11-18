/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-06
** File description:
** Systems
*/

#include "Systems.hpp"
#include "Protocol/Events.hpp"

namespace Flakkari::Engine::ECS::Systems::_2D {

void position(Registry &r, float deltaTime)
{
    if (!r.isRegistered<ECS::Components::_2D::Transform>() || !r.isRegistered<ECS::Components::_2D::Movable>())
        return;
    auto &positions = r.getComponents<ECS::Components::_2D::Transform>();
    auto &velocities = r.getComponents<ECS::Components::_2D::Movable>();

    for (Entity i(0); i < positions.size() && i < velocities.size(); ++i)
    {
        auto &pos = positions[i];
        auto &vel = velocities[i];

        if (pos.has_value() && vel.has_value())
        {
            float magnitude =
                std::sqrt(vel->_velocity.vec.x * vel->_velocity.vec.x + vel->_velocity.vec.y * vel->_velocity.vec.y);
            if (magnitude > 0.0f)
            {
                vel->_velocity.vec.x /= magnitude;
                vel->_velocity.vec.y /= magnitude;
            }
            pos->_position.vec.x += vel->_velocity.vec.x * vel->_acceleration.vec.x * deltaTime;
            pos->_position.vec.y += vel->_velocity.vec.y * vel->_acceleration.vec.y * deltaTime;
        }
    }
}

void update_control(Registry &r)
{
    if (!r.isRegistered<ECS::Components::_2D::Movable>() || !r.isRegistered<ECS::Components::Common::NetworkEvent>())
        return;
    auto &velocities = r.getComponents<ECS::Components::_2D::Movable>();
    auto &networkEvent = r.getComponents<Engine::ECS::Components::Common::NetworkEvent>();

    for (Entity i(0); i < velocities.size(); ++i)
    {
        auto &vel = velocities[i];
        auto &net = networkEvent[i];

        if (!net.has_value() || !vel.has_value())
            continue;
        if (net->events.size() < int(Protocol::V_0::EventId::MOVE_UP))
            continue;
        if (net->events[int(Protocol::V_0::EventId::MOVE_UP)] == int(Protocol::V_0::EventState::PRESSED))
            vel->_velocity.vec.y = -1;

        if (net->events.size() < int(Protocol::V_0::EventId::MOVE_DOWN))
            continue;
        if (net->events[int(Protocol::V_0::EventId::MOVE_DOWN)] == int(Protocol::V_0::EventState::PRESSED))
            vel->_velocity.vec.y = 1;

        if (net->events.size() < int(Protocol::V_0::EventId::MOVE_LEFT))
            continue;
        if (net->events[int(Protocol::V_0::EventId::MOVE_LEFT)] == int(Protocol::V_0::EventState::PRESSED))
            vel->_velocity.vec.x = -1;

        if (net->events.size() < int(Protocol::V_0::EventId::MOVE_RIGHT))
            continue;
        if (net->events[int(Protocol::V_0::EventId::MOVE_RIGHT)] == int(Protocol::V_0::EventState::PRESSED))
            vel->_velocity.vec.x = 1;
    }
}

} // namespace Flakkari::Engine::ECS::Systems::_2D

namespace Flakkari::Engine::ECS::Systems::_3D {

void apply_movable(Registry &r, float deltaTime)
{
    if (!r.isRegistered<ECS::Components::_3D::Transform>() || !r.isRegistered<ECS::Components::_3D::Movable>())
        return;
    auto &positions = r.getComponents<ECS::Components::_3D::Transform>();
    auto &velocities = r.getComponents<ECS::Components::_3D::Movable>();

    for (Entity i(0); i < positions.size() && i < velocities.size(); ++i)
    {
        auto &pos = positions[i];
        auto &vel = velocities[i];

        if (pos.has_value() && vel.has_value())
        {
            pos->_position.vec.x += vel->_velocity.vec.x * vel->_acceleration.vec.x * deltaTime;
            pos->_position.vec.y += vel->_velocity.vec.y * vel->_acceleration.vec.y * deltaTime;
            pos->_position.vec.z += vel->_velocity.vec.z * vel->_acceleration.vec.z * deltaTime;
        }
    }
}

static float randomRange(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void spawn_random_within_skybox(Registry &r, std::vector<Entity> &entities)
{
    if (!r.isRegistered<ECS::Components::_3D::Transform>() || !r.isRegistered<ECS::Components::Common::Tag>() ||
        !r.isRegistered<ECS::Components::Common::Spawned>())
        return;

    auto &transforms = r.getComponents<ECS::Components::_3D::Transform>();
    auto &tags = r.getComponents<ECS::Components::Common::Tag>();
    auto &boxcollier = r.getComponents<ECS::Components::_3D::BoxCollider>();
    auto &spawned = r.getComponents<ECS::Components::Common::Spawned>();

    float maxRangeX = 0;
    float maxRangeY = 0;
    float maxRangeZ = 0;

    for (Entity i(0); i < transforms.size() && i < tags.size(); ++i)
    {
        auto &transform = transforms[i];
        auto &tag = tags[i];
        auto &box = boxcollier[i];

        if (!transform.has_value() || !tag.has_value() || !box.has_value())
            continue;

        if (tag->tag == "Skybox")
        {
            maxRangeX = (box->_size.dimension.width * transform->_scale.vec.x) / 2;
            maxRangeY = (box->_size.dimension.height * transform->_scale.vec.y) / 2;
            maxRangeZ = (box->_size.dimension.depth * transform->_scale.vec.z) / 2;
            break;
        }
    }

    for (Entity i(0); i < transforms.size() && i < tags.size(); ++i)
    {
        auto &transform = transforms[i];
        auto &tag = tags[i];
        auto &spawn = spawned[i];

        if (!transform.has_value() || !tag.has_value() || !spawn.has_value())
            continue;

        if ((tag->tag == "Player" || tag->tag == "Enemy") && spawn->has_spawned == false)
        {
            transform->_position.vec.x = randomRange(-maxRangeX, maxRangeX);
            transform->_position.vec.y = randomRange(-maxRangeY, maxRangeY);
            transform->_position.vec.z = randomRange(-maxRangeZ, maxRangeZ);
            std::cout << "Entity spawned at: " << transform->_position.vec.x << ", " << transform->_position.vec.y
                      << ", " << transform->_position.vec.z << std::endl;
            spawn->has_spawned = true;
            entities.emplace_back(i);
        }
    }
}

static size_t count_entities(Registry &r, const std::string &tag)
{
    if (!r.isRegistered<ECS::Components::Common::Tag>())
        return 0;

    auto &tags = r.getComponents<ECS::Components::Common::Tag>();
    size_t count = 0;

    for (Entity i(0); i < tags.size(); ++i)
    {
        auto &tagComponent = tags[i];

        if (!tagComponent.has_value())
            continue;

        if (tagComponent->tag == tag)
            count++;
    }

    return count;
}

bool spawn_enemy(Registry &r, std::string &templateName, Entity &entity)
{
    if (!r.isRegistered<ECS::Components::_3D::Transform>() || !r.isRegistered<ECS::Components::Common::Tag>() ||
        !r.isRegistered<ECS::Components::Common::Spawned>())
        return false;

    auto &transforms = r.getComponents<ECS::Components::_3D::Transform>();
    auto &tags = r.getComponents<ECS::Components::Common::Tag>();
    auto &boxcollier = r.getComponents<ECS::Components::_3D::BoxCollider>();
    auto &timers = r.getComponents<ECS::Components::Common::Timer>();
    auto &templates = r.getComponents<ECS::Components::Common::Template>();

    float maxRangeX = 0;
    float maxRangeY = 0;
    float maxRangeZ = 0;

    for (Entity i(0); i < transforms.size() && i < tags.size(); ++i)
    {
        auto &transform = transforms[i];
        auto &tag = tags[i];
        auto &box = boxcollier[i];
        auto &timer = timers[i];
        auto &template_ = templates[i];

        if (!transform.has_value() || !tag.has_value() || !box.has_value() || !timer.has_value() ||
            !template_.has_value())
            continue;

        if (tag->tag != "Skybox")
            continue;

        maxRangeX = (box->_size.dimension.width * transform->_scale.vec.x) / 2;
        maxRangeY = (box->_size.dimension.height * transform->_scale.vec.y) / 2;
        maxRangeZ = (box->_size.dimension.depth * transform->_scale.vec.z) / 2;

        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - timer->lastTime);
        if (duration.count() <= timer->maxTime * 1000)
            return false;

        std::cout << "Time since last spawn: " << duration.count() << std::endl;

        timer->lastTime = now;
        if (count_entities(r, "Enemy") >= 10)
            return false;

        entity = r.spawn_entity();
        templateName = template_->name;
        Factory::RegistryEntityByTemplate(r, entity, template_->content);
        return true;
    }
    return false;
}

//////////////////////////
//*_ WORK IN PROGRESS _*//
//////////////////////////

static void handleDeath(Registry &r, Entity bullet, Entity entity, std::unordered_map<Entity, bool> &entities)
{
    auto &health = r.getComponents<Components::Common::Health>(entity);
    auto &weapon = r.getComponents<Components::Common::Weapon>(bullet);

    health->currentHealth -= weapon->minDamage;
    entities[entity] = true;
    if (health->currentHealth <= 0)
    {
        r.kill_entity(entity); // send death event to client
        entities[entity] = false;
    }
    // else
    //     r.emplace_component<Components::Common::Health>(entity, *health); // send health update event to client
    r.kill_entity(bullet); // send destroy event to client
    entities[entity] = false;
}

static bool BoxCollisions(const Components::_3D::Transform &pos1, const Components::_3D::BoxCollider &col1,
                          const Components::_3D::Transform &pos2, const Components::_3D::BoxCollider &col2)
{
    return pos1._position.vec.x < pos2._position.vec.x + (col2._size.dimension.width * pos2._scale.dimension.width) &&
           pos1._position.vec.x + (col1._size.dimension.width * pos1._scale.dimension.width) > pos2._position.vec.x &&
           pos1._position.vec.y < pos2._position.vec.y + (col2._size.dimension.height * pos2._scale.dimension.height) &&
           pos1._position.vec.y + (col1._size.dimension.height * pos1._scale.dimension.height) > pos2._position.vec.y &&
           pos1._position.vec.z < pos2._position.vec.z + (col2._size.dimension.depth * pos2._scale.dimension.depth) &&
           pos1._position.vec.z + (col1._size.dimension.depth * pos1._scale.dimension.depth) > pos2._position.vec.z;
}

// static Math::Vector3f resolveBoxCollisions(const Components::_3D::Transform &pos1,
//                                            const Components::_3D::BoxCollider &col1,
//                                            const Components::_3D::Transform &pos2,
//                                            const Components::_3D::BoxCollider &col2)
// {
//     Math::Vector3f normal;
//     float x = std::max(pos2._position.vec.x,
//                        std::min(pos1._position.vec.x,
//                                 pos2._position.vec.x + col2._size.dimension.width * pos2._scale.dimension.width));
//     float y = std::max(pos2._position.vec.y,
//                        std::min(pos1._position.vec.y,
//                                 pos2._position.vec.y + col2._size.dimension.height * pos2._scale.dimension.height));
//     float z = std::max(pos2._position.vec.z,
//                        std::min(pos1._position.vec.z,
//                                 pos2._position.vec.z + col2._size.dimension.depth * pos2._scale.dimension.depth));

//     float dx = x - pos1._position.vec.x;
//     float dy = y - pos1._position.vec.y;
//     float dz = z - pos1._position.vec.z;

//     float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
//     if (distance < col1._size.dimension.width * pos1._scale.dimension.width)
//     {
//         normal.vec.x = dx;
//         normal.vec.y = dy;
//         normal.vec.z = dz;
//     }
//     return normal;
// }

static bool SphereCollisions(const Components::_3D::Transform &pos1, const Components::_3D::SphereCollider &col1,
                             const Components::_3D::Transform &pos2, const Components::_3D::SphereCollider &col2)
{
    float dx = pos1._position.vec.x - pos2._position.vec.x;
    float dy = pos1._position.vec.y - pos2._position.vec.y;
    float dz = pos1._position.vec.z - pos2._position.vec.z;
    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    return distance < (col1._radius + col2._radius);
}

static Math::Vector3f resolveSphereCollisions(const Components::_3D::Transform &pos1,
                                              const Components::_3D::SphereCollider &col1,
                                              const Components::_3D::Transform &pos2,
                                              const Components::_3D::SphereCollider &col2)
{
    Math::Vector3f normal;
    float dx = pos1._position.vec.x - pos2._position.vec.x;
    float dy = pos1._position.vec.y - pos2._position.vec.y;
    float dz = pos1._position.vec.z - pos2._position.vec.z;
    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (distance < col1._radius + col2._radius)
    {
        normal.vec.x = dx / distance;
        normal.vec.y = dy / distance;
        normal.vec.z = dz / distance;
    }
    return normal;
}

static bool SphereBoxCollisions(const Components::_3D::Transform &pos1, const Components::_3D::SphereCollider &col1,
                                const Components::_3D::Transform &pos2, const Components::_3D::BoxCollider &col2)
{
    float x = std::max(pos2._position.vec.x,
                       std::min(pos1._position.vec.x,
                                pos2._position.vec.x + col2._size.dimension.width * pos2._scale.dimension.width));
    float y = std::max(pos2._position.vec.y,
                       std::min(pos1._position.vec.y,
                                pos2._position.vec.y + col2._size.dimension.height * pos2._scale.dimension.height));
    float z = std::max(pos2._position.vec.z,
                       std::min(pos1._position.vec.z,
                                pos2._position.vec.z + col2._size.dimension.depth * pos2._scale.dimension.depth));

    float dx = x - pos1._position.vec.x;
    float dy = y - pos1._position.vec.y;
    float dz = z - pos1._position.vec.z;

    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    return distance < col1._radius;
}

// static Math::Vector3f resolveSphereBoxCollisions(const Components::_3D::Transform &pos1,
//                                                  const Components::_3D::SphereCollider &col1,
//                                                  const Components::_3D::Transform &pos2,
//                                                  const Components::_3D::BoxCollider &col2)
// {
//     Math::Vector3f normal;
//     float x = std::max(pos2._position.vec.x,
//                        std::min(pos1._position.vec.x,
//                                 pos2._position.vec.x + col2._size.dimension.width * pos2._scale.dimension.width));
//     float y = std::max(pos2._position.vec.y,
//                        std::min(pos1._position.vec.y,
//                                 pos2._position.vec.y + col2._size.dimension.height * pos2._scale.dimension.height));
//     float z = std::max(pos2._position.vec.z,
//                        std::min(pos1._position.vec.z,
//                                 pos2._position.vec.z + col2._size.dimension.depth * pos2._scale.dimension.depth));

//     float dx = x - pos1._position.vec.x;
//     float dy = y - pos1._position.vec.y;
//     float dz = z - pos1._position.vec.z;

//     float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
//     if (distance < col1._radius)
//     {
//         normal.vec.x = dx;
//         normal.vec.y = dy;
//         normal.vec.z = dz;
//     }
//     return normal;
// }

static Math::Vector3f reflectVelocity(const Math::Vector3f &velocity, const Math::Vector3f &normal)
{
    Math::Vector3f reflected;
    float dot = 2 * (velocity.vec.x * normal.vec.x + velocity.vec.y * normal.vec.y + velocity.vec.z * normal.vec.z);
    reflected.vec.x = velocity.vec.x - dot * normal.vec.x;
    reflected.vec.y = velocity.vec.y - dot * normal.vec.y;
    reflected.vec.z = velocity.vec.z - dot * normal.vec.z;
    return reflected;
}

static bool outOfSkybox(float maxRangeX, float maxRangeY, float maxRangeZ, const Components::_3D::Transform &pos)
{
    return pos._position.vec.x < -maxRangeX || pos._position.vec.x > maxRangeX || pos._position.vec.y < -maxRangeY ||
           pos._position.vec.y > maxRangeY || pos._position.vec.z < -maxRangeZ || pos._position.vec.z > maxRangeZ;
}

void handle_collisions(Registry &r, std::unordered_map<Entity, bool> &entities)
{
    auto &transforms = r.getComponents<Components::_3D::Transform>();
    auto &boxcollider = r.getComponents<Components::_3D::BoxCollider>();
    auto &spherecollider = r.getComponents<Components::_3D::SphereCollider>();
    auto &tags = r.getComponents<Components::Common::Tag>();

    float maxRangeX = 0;
    float maxRangeY = 0;
    float maxRangeZ = 0;

    // Get the skybox size
    for (Entity i(0); i < transforms.size() && i < tags.size(); ++i)
    {
        auto &transform = transforms[i];
        auto &tag = tags[i];

        if (transform.has_value() && tag.has_value())
        {
            if (tag->tag == "Skybox")
            {
                maxRangeX = transform->_scale.vec.x / 2;
                maxRangeY = transform->_scale.vec.y / 2;
                maxRangeZ = transform->_scale.vec.z / 2;
            }
        }
    }

    bool bulletKilled = false;

    for (Entity i(0); i < transforms.size() && i < tags.size(); ++i)
    {
        auto &pos1 = transforms[i];
        auto &bcol1 = boxcollider[i];
        auto &scol1 = spherecollider[i];
        auto &tag1 = tags[i];

        if (!pos1.has_value() || !tag1.has_value())
            continue;

        // Check if the entity is out of the skybox
        if (tag1->tag == "Player" && outOfSkybox(maxRangeX, maxRangeY, maxRangeZ, *pos1))
        {
            // Resolve collision
            pos1->_position.vec.x = std::max(-maxRangeX, std::min(maxRangeX, pos1->_position.vec.x));
            pos1->_position.vec.y = std::max(-maxRangeY, std::min(maxRangeY, pos1->_position.vec.y));
            pos1->_position.vec.z = std::max(-maxRangeZ, std::min(maxRangeZ, pos1->_position.vec.z));

            entities[i] = true;
        }
        else if (tag1->tag == "Enemy" && outOfSkybox(maxRangeX, maxRangeY, maxRangeZ, *pos1))
        {
            // Resolve collision
            pos1->_position.vec.x = std::max(-maxRangeX, std::min(maxRangeX, pos1->_position.vec.x));
            pos1->_position.vec.y = std::max(-maxRangeY, std::min(maxRangeY, pos1->_position.vec.y));
            pos1->_position.vec.z = std::max(-maxRangeZ, std::min(maxRangeZ, pos1->_position.vec.z));

            entities[i] = true;
        }
        else if (tag1->tag == "Bullet" && outOfSkybox(maxRangeX, maxRangeY, maxRangeZ, *pos1))
        {
            // Destroy bullet
            r.kill_entity(i);
            bulletKilled = true;
            entities[i] = false;
        }

        for (Entity j(i + 1); j < transforms.size() && j < tags.size(); ++j)
        {
            auto &pos2 = transforms[j];
            auto &bcol2 = boxcollider[j];
            auto &scol2 = spherecollider[j];
            auto &tag2 = tags[j];

            if (!pos2.has_value() || !tag2.has_value())
                continue;

            // Two sphere entity collide with each other
            if (((tag1->tag == "Player" && tag2->tag == "Enemy") || (tag2->tag == "Player" && tag1->tag == "Enemy")) &&
                scol1.has_value() && scol2.has_value())
            {
                // Two sphere collide with each other
                if (SphereCollisions(pos1.value(), scol1.value(), pos2.value(), scol2.value()))
                {
                    // Resolve collision
                    Math::Vector3f normal = resolveSphereCollisions(*pos1, *scol1, *pos2, *scol2);
                    pos1->_position.vec.x += normal.vec.x;
                    pos1->_position.vec.y += normal.vec.y;
                    pos1->_position.vec.z += normal.vec.z;

                    pos2->_position.vec.x -= normal.vec.x;
                    pos2->_position.vec.y -= normal.vec.y;
                    pos2->_position.vec.z -= normal.vec.z;

                    // Reflect velocity
                    auto &vel1 = r.getComponents<Components::_3D::Movable>()[i];
                    auto &vel2 = r.getComponents<Components::_3D::Movable>()[j];
                    if (vel1.has_value() && vel2.has_value())
                    {
                        vel1->_velocity = reflectVelocity(vel1->_velocity, normal);
                        vel2->_velocity = reflectVelocity(vel2->_velocity, normal);
                    }

                    entities[i] = true;
                }
            }
            else if (tag2->tag == "Bullet" && tag1->tag == "Enemy" && scol1.has_value() && bcol2.has_value())
            {
                if (r.isRegistered<Components::Common::Health>(i) &&
                    SphereBoxCollisions(pos1.value(), scol1.value(), pos2.value(), bcol2.value()))
                    handleDeath(r, j, i, entities);
            }
            else if (tag2->tag == "Bullet" && tag1->tag == "Player" && scol1.has_value() && bcol2.has_value())
            {
                if (r.isRegistered<Components::Common::Health>(i) &&
                    SphereBoxCollisions(pos1.value(), scol1.value(), pos2.value(), bcol2.value()))
                    handleDeath(r, j, i, entities);
            }
            else if (bulletKilled)
                continue;
            else if ((tag1->tag == "Bullet" && tag2->tag == "Bullet") ||
                     (tag2->tag == "Bullet" && tag1->tag == "Bullet"))
            {
                // Two box collide with each other
                if (BoxCollisions(pos1.value(), bcol1.value(), pos2.value(), bcol2.value()))
                {
                    // Destroy both bullets
                    r.kill_entity(i);
                    r.kill_entity(j);

                    entities[i] = false;
                    entities[j] = false;
                }
            }
            else if (tag1->tag == "Bullet" && tag2->tag == "Enemy" && scol2.has_value() && bcol1.has_value())
            {
                if (r.isRegistered<Components::Common::Health>(j) &&
                    SphereBoxCollisions(pos2.value(), scol2.value(), pos1.value(), bcol1.value()))
                    handleDeath(r, i, j, entities);
            }
            else if (tag1->tag == "Bullet" && tag2->tag == "Player" && scol2.has_value() && bcol1.has_value())
            {
                if (r.isRegistered<Components::Common::Health>(j) &&
                    SphereBoxCollisions(pos2.value(), scol2.value(), pos1.value(), bcol1.value()))
                    handleDeath(r, i, j, entities);
            }
        }
        bulletKilled = false;
    }
}

} // namespace Flakkari::Engine::ECS::Systems::_3D
