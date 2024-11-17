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
        if (net->events.size() < int(Protocol::EventId::MOVE_UP))
            continue;
        if (net->events[int(Protocol::EventId::MOVE_UP)] == int(Protocol::EventState::PRESSED))
            vel->_velocity.vec.y = -1;

        if (net->events.size() < int(Protocol::EventId::MOVE_DOWN))
            continue;
        if (net->events[int(Protocol::EventId::MOVE_DOWN)] == int(Protocol::EventState::PRESSED))
            vel->_velocity.vec.y = 1;

        if (net->events.size() < int(Protocol::EventId::MOVE_LEFT))
            continue;
        if (net->events[int(Protocol::EventId::MOVE_LEFT)] == int(Protocol::EventState::PRESSED))
            vel->_velocity.vec.x = -1;

        if (net->events.size() < int(Protocol::EventId::MOVE_RIGHT))
            continue;
        if (net->events[int(Protocol::EventId::MOVE_RIGHT)] == int(Protocol::EventState::PRESSED))
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
#ifdef _WIN32
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
#else
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
#endif
}

void spawn_random_within_skybox(Registry &r)
{
    if (!r.isRegistered<ECS::Components::_3D::Transform>() || !r.isRegistered<ECS::Components::Common::Tag>() ||
        !r.isRegistered<ECS::Components::Common::Spawned>())
        return;

    auto &transforms = r.getComponents<ECS::Components::_3D::Transform>();
    auto &tags = r.getComponents<ECS::Components::Common::Tag>();
    auto &spawned = r.getComponents<ECS::Components::Common::Spawned>();

    float maxRangeX = 0;
    float maxRangeY = 0;
    float maxRangeZ = 0;

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
                break;
            }
        }
    }

    for (Entity i(0); i < transforms.size() && i < tags.size(); ++i)
    {
        auto &transform = transforms[i];
        auto &tag = tags[i];
        auto &spawn = spawned[i];

        if (transform.has_value() && tag.has_value() && !spawn.has_value())
        {
            if ((tag->tag == "Player" || tag->tag == "Enemy") && spawn->has_spawned == false)
            {
                transform->_position.vec.x = randomRange(-maxRangeX, maxRangeX);
                transform->_position.vec.y = randomRange(-maxRangeY, maxRangeY);
                transform->_position.vec.z = randomRange(-maxRangeZ, maxRangeZ);
                spawn->has_spawned = true;
            }
        }
    }
}

static void handleDeath(Registry &r, Entity bullet, Entity entity)
{
    auto &health = r.getComponents<Components::Common::Health>(entity);
    auto &weapon = r.getComponents<Components::Common::Weapon>(bullet);

    health->currentHealth -= weapon->damage;
    if (health->currentHealth <= 0)
        r.kill_entity(entity);
    r.kill_entity(bullet);
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
static bool outOfSkybox(float maxRangeX, float maxRangeY, float maxRangeZ, const Components::_3D::Transform &pos)
{
    return pos._position.vec.x < -maxRangeX || pos._position.vec.x > maxRangeX || pos._position.vec.y < -maxRangeY ||
           pos._position.vec.y > maxRangeY || pos._position.vec.z < -maxRangeZ || pos._position.vec.z > maxRangeZ;
}

} // namespace Flakkari::Engine::ECS::Systems::_3D
