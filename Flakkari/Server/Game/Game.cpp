/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-06
** File description:
** Game
*/

#include "Game.hpp"
#include "../Client/ClientManager.hpp"
#include "Engine/EntityComponentSystem/Components/ComponentsCommon.hpp"
#include "Protocol/PacketFactory.hpp"
#include "ResourceManager.hpp"

namespace Flakkari {

void Game::sendAllEntities(const std::string &sceneName, std::shared_ptr<Client> player)
{
    auto &registry = _scenes[sceneName];

    auto &transforms = registry.getComponents<Engine::ECS::Components::_2D::Transform>();

    for (Engine::ECS::Entity i(0); i < transforms.size(); ++i)
    {
        auto &transform = transforms[i];

        if (!transform.has_value())
            continue;

        Protocol::Packet<Protocol::CommandId> packet;
        packet.header._commandId = Protocol::CommandId::REQ_ENTITY_SPAWN;
        packet.header._apiVersion = player->getApiVersion();
        packet << i;
        packet.injectString(sceneName);
        Protocol::PacketFactory::addComponentsToPacketByEntity<Protocol::CommandId>(packet, registry, i);

        ClientManager::GetInstance().sendPacketToClient(player->getAddress(), packet.serialize());
        ClientManager::UnlockInstance();
    }
}

Game::Game(const std::string &name, std::shared_ptr<nlohmann::json> config)
{
    _name = name;
    _config = config;
    _time = std::chrono::steady_clock::now();

    if ((*_config)["scenes"].empty())
    {
        FLAKKARI_LOG_ERROR("Game: no scenes found");
        return;
    }

    loadScene((*_config)["startGame"]);
    ResourceManager::GetInstance().addScene(config, (*_config)["startGame"]);
    ResourceManager::UnlockInstance();
}

Game::~Game()
{
    _running = false;
    _thread.join();
    FLAKKARI_LOG_INFO("game \"" + _name + "\" is now stopped");
}

void Game::loadSystems(Engine::ECS::Registry &registry, const std::string &name)
{
    if (name == "position")
        return registry.add_system([this](Engine::ECS::Registry &r) { Engine::ECS::Systems::position(r, _deltaTime); }),
               void();
}

void Game::loadComponents(Engine::ECS::Registry &registry, const nl_component &components,
                          Engine::ECS::Entity newEntity)
{
    if (!components.is_object())
        return;
    for (auto &component : components.items())
    {
        auto componentName = component.key();
        auto componentContent = component.value();

        if (componentName == "Transform")
        {
            registry.registerComponent<Engine::ECS::Components::_2D::Transform>();
            Engine::ECS::Components::_2D::Transform transform;
            transform.position =
                Engine::Math::Vector2f(componentContent["position"]["x"], componentContent["position"]["y"]);
            transform.rotation = componentContent["rotation"];
            transform.scale = Engine::Math::Vector2f(componentContent["scale"]["x"], componentContent["scale"]["y"]);
            registry.add_component<Engine::ECS::Components::_2D::Transform>(newEntity, std::move(transform));
            continue;
        }

        if (componentName == "Movable")
        {
            registry.registerComponent<Engine::ECS::Components::_2D::Movable>();
            Engine::ECS::Components::_2D::Movable movable;
            movable.velocity =
                Engine::Math::Vector2f(componentContent["velocity"]["x"], componentContent["velocity"]["y"]);
            movable.acceleration =
                Engine::Math::Vector2f(componentContent["acceleration"]["x"], componentContent["acceleration"]["y"]);
            registry.add_component<Engine::ECS::Components::_2D::Movable>(newEntity, std::move(movable));
            continue;
        }

        if (componentName == "Control")
        {
            registry.registerComponent<Engine::ECS::Components::_2D::Control>();
            Engine::ECS::Components::_2D::Control control;
            control.up = componentContent["up"];
            control.down = componentContent["down"];
            control.left = componentContent["left"];
            control.right = componentContent["right"];
            control.shoot = componentContent["shoot"];
            registry.add_component<Engine::ECS::Components::_2D::Control>(newEntity, std::move(control));
            continue;
        }

        if (componentName == "Collider")
        {
            registry.registerComponent<Engine::ECS::Components::_2D::Collider>();
            Engine::ECS::Components::_2D::Collider collider;
            collider._size = Engine::Math::Vector2f(componentContent["size"]["x"], componentContent["size"]["y"]);
            registry.add_component<Engine::ECS::Components::_2D::Collider>(newEntity, std::move(collider));
            continue;
        }

        if (componentName == "Evolve")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Evolve>();
            Engine::ECS::Components::Common::Evolve evolve;
            evolve.name = componentContent["name"].get<std::string>().c_str();
            registry.add_component<Engine::ECS::Components::Common::Evolve>(newEntity, std::move(evolve));
            continue;
        }

        if (componentName == "Spawned")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Spawned>();
            Engine::ECS::Components::Common::Spawned spawned;
            spawned.has_spawned = componentContent["has_spawned"];
            registry.add_component<Engine::ECS::Components::Common::Spawned>(newEntity, std::move(spawned));
            continue;
        }

        if (componentName == "Tag")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Tag>();
            Engine::ECS::Components::Common::Tag tag;
            tag.tag = componentContent.get<std::string>().c_str();
            registry.add_component<Engine::ECS::Components::Common::Tag>(newEntity, std::move(tag));
            continue;
        }

        if (componentName == "Weapon")
        {
            registry.registerComponent<Engine::ECS::Components::Common::Weapon>();
            Engine::ECS::Components::Common::Weapon weapon;
            weapon.fireRate = componentContent["fireRate"];
            weapon.damage = componentContent["damage"];
            weapon.level = componentContent["level"];
            registry.add_component<Engine::ECS::Components::Common::Weapon>(newEntity, std::move(weapon));
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
            registry.add_component<Engine::ECS::Components::Common::Health>(newEntity, std::move(health));
            continue;
        }
    }
}

void Game::loadEntityFromTemplate(Engine::ECS::Registry &registry, const nl_entity &entity,
                                  const nl_template &templates)
{
    Engine::ECS::Entity newEntity = registry.spawn_entity();

    for (auto &componentInfo : entity.begin().value().items())
    {
        for (auto &templateInfo : templates.items())
        {
            if (templateInfo.key() != componentInfo.key())
                continue;
            loadComponents(registry, templateInfo.value(), newEntity);
            registry.registerComponent<Engine::ECS::Components::Common::Template>();
            registry.add_component<Engine::ECS::Components::Common::Template>(newEntity, templateInfo.key());
        }
    }
}

void Game::loadScene(const std::string &sceneName)
{
    for (auto &scene : (*_config)["scenes"].items())
    {
        for (auto sceneInfo : scene.value().items())
        {
            if (sceneInfo.key() != sceneName)
                continue;
            Engine::ECS::Registry registry;

            for (auto &system : sceneInfo.value()["systems"].items())
                loadSystems(registry, system.value());

            for (auto &entity : sceneInfo.value()["entities"].items())
                loadEntityFromTemplate(registry, entity.value().items(), sceneInfo.value()["templates"]);

            _scenes[sceneName] = registry;
            return;
        }
    }
}

void Game::sendOnSameScene(const std::string &sceneName, Protocol::Packet<Protocol::CommandId> &packet)
{
    for (auto &player : _players)
    {
        if (!player)
            continue;
        if (!player->isConnected())
            continue;
        if (player->getSceneName() != sceneName)
            continue;

        packet.header._apiVersion = player->getApiVersion();

        ClientManager::GetInstance().sendPacketToClient(player->getAddress(), packet.serialize());
        ClientManager::UnlockInstance();
    }
}

void Game::sendOnSameSceneExcept(const std::string &sceneName, Protocol::Packet<Protocol::CommandId> &packet,
                                 std::shared_ptr<Client> except)
{
    for (auto &player : _players)
    {
        if (!player)
            continue;
        if (!player->isConnected())
            continue;
        if (player->getSceneName() != sceneName)
            continue;
        if (player == except)
            continue;

        packet.header._apiVersion = player->getApiVersion();

        ClientManager::GetInstance().sendPacketToClient(player->getAddress(), packet.serialize());
        ClientManager::UnlockInstance();
    }
}

void Game::checkDisconnect()
{
    for (auto &player : _players)
    {
        if (!player || player->isConnected())
            continue;
        Protocol::Packet<Protocol::CommandId> packet;
        packet.header._commandId = Protocol::CommandId::REQ_DISCONNECT;
        packet << player->getEntity();
        sendOnSameScene(player->getSceneName(), packet);
        _scenes[player->getSceneName()].kill_entity(player->getEntity());
        removePlayer(player);
    }
}

void Game::sendUpdatePosition(std::shared_ptr<Client> player, Engine::ECS::Components::_2D::Transform pos,
                              Engine::ECS::Components::_2D::Movable vel)
{
    Protocol::Packet<Protocol::CommandId> packet;
    packet.header._commandId = Protocol::CommandId::REQ_ENTITY_MOVED;
    packet << player->getEntity();
    packet << pos.position.vec.x;
    packet << pos.position.vec.y;
    packet << pos.rotation;
    packet << pos.scale.vec.x;
    packet << pos.scale.vec.y;
    packet << vel.velocity.vec.x;
    packet << vel.velocity.vec.y;
    packet << vel.acceleration.vec.x;
    packet << vel.acceleration.vec.y;

    FLAKKARI_LOG_LOG("packet size: " + std::to_string(packet.size()) +
                     " bytes\n"
                     "packet sent: <Id: " +
                     std::to_string(player->getEntity()) + ", Pos: (" + std::to_string(pos.position.vec.x) + ", " +
                     std::to_string(pos.position.vec.y) + ")" + ", Vel: (" + std::to_string(vel.velocity.vec.x) + ", " +
                     std::to_string(vel.velocity.vec.y) + ")" + ", Acc: (" + std::to_string(vel.acceleration.vec.x) +
                     ", " + std::to_string(vel.acceleration.vec.y) + ")" + ">");
    sendOnSameScene(player->getSceneName(), packet);
}

void Game::handleEvent(std::shared_ptr<Client> player, Protocol::Packet<Protocol::CommandId> packet)
{
    auto sceneName = player->getSceneName();
    auto entity = player->getEntity();
    auto &registry = _scenes[sceneName];
    auto &ctrl = registry.getComponents<Engine::ECS::Components::_2D::Control>()[entity];
    auto &vel = registry.getComponents<Engine::ECS::Components::_2D::Movable>()[entity];
    auto &pos = registry.getComponents<Engine::ECS::Components::_2D::Transform>()[entity];
    auto &netEvent = registry.getComponents<Engine::ECS::Components::Common::NetworkEvent>()[entity];

    if (!ctrl.has_value() || !vel.has_value() || !pos.has_value())
        return;

    Protocol::Event event = *(Protocol::Event *) packet.payload.data();
    if (event.id == Protocol::EventId::MOVE_UP && ctrl->up)
    {
        if (netEvent->events.size() < size_t(event.id))
            netEvent->events.resize(size_t(event.id) + 1);
        netEvent->events[size_t(event.id)] = (unsigned short) event.state;

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->velocity.vec.y = -1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->velocity.vec.y = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::MOVE_DOWN && ctrl->down)
    {
        if (netEvent->events.size() < size_t(event.id))
            netEvent->events.resize(size_t(event.id) + 1);
        netEvent->events[size_t(event.id)] = (unsigned short) event.state;

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->velocity.vec.y = 1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->velocity.vec.y = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::MOVE_LEFT && ctrl->left)
    {
        if (netEvent->events.size() < size_t(event.id))
            netEvent->events.resize(size_t(event.id) + 1);
        netEvent->events[size_t(event.id)] = (unsigned short) event.state;

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->velocity.vec.x = -1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->velocity.vec.x = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::MOVE_RIGHT && ctrl->right)
    {
        if (netEvent->events.size() < size_t(event.id))
            netEvent->events.resize(size_t(event.id) + 1);
        netEvent->events[size_t(event.id)] = (unsigned short) event.state;

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->velocity.vec.x = 1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->velocity.vec.x = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::SHOOT && ctrl->shoot)
    {
        if (netEvent->events.size() < size_t(event.id))
            netEvent->events.resize(size_t(event.id) + 1);
        netEvent->events[size_t(event.id)] = (unsigned short) event.state;

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::RELEASED)
        {
            Protocol::Packet<Protocol::CommandId> shootPacket;
            shootPacket.header._commandId = Protocol::CommandId::REQ_ENTITY_SHOOT;
            shootPacket.injectString(player->getSceneName());
            shootPacket << player->getEntity();
            // create a bullet with player as parent
            sendOnSameScene(player->getSceneName(), shootPacket);
        }
        return;
    }
}

void Game::updateIncomingPackets(unsigned char maxMessagePerFrame)
{
    for (auto &player : _players)
    {
        if (!player->isConnected())
            continue;
        auto &packets = player->getReceiveQueue();
        auto messageCount = maxMessagePerFrame;

        while (!packets.empty() && messageCount > 0)
        {
            auto packet = packets.pop_front();
            FLAKKARI_LOG_INFO("packet received: " + packet.to_string());
            messageCount--;

            if (packet.header._commandId == Protocol::CommandId::REQ_USER_UPDATE)
                handleEvent(player, packet);
        }
    }
}

void Game::update()
{
    auto now = std::chrono::steady_clock::now();
    _deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - _time).count();
    _time = now;

    checkDisconnect();

    updateIncomingPackets();

    for (auto &scene : _scenes)
        scene.second.run_systems();
}

void Game::start()
{
    _running = true;
    _time = std::chrono::steady_clock::now();
    _thread = std::thread(&Game::run, this);
    FLAKKARI_LOG_INFO("game \"" + _name + "\" is now running");
}

void Game::run()
{
    while (_running)
        update();
}

bool Game::addPlayer(std::shared_ptr<Client> player)
{
    if (std::find(_players.begin(), _players.end(), player) != _players.end())
        return false;
    if (_players.size() >= (*_config)["maxPlayers"] || !player->isConnected())
        return false;

    auto sceneGame = (*_config)["startGame"].get<std::string>();
    auto &registry = _scenes[sceneGame];
    auto address = player->getAddress();

    player->setSceneName(sceneGame);

    Engine::ECS::Entity newEntity = registry.spawn_entity();
    auto p_Template = (*_config)["playerTemplate"];
    auto player_info = ResourceManager::GetInstance().getTemplateById(_name, sceneGame, p_Template);

    loadComponents(registry, player_info.value_or(nullptr), newEntity);
    ResourceManager::UnlockInstance();

    registry.registerComponent<Engine::ECS::Components::Common::NetworkIp>();
    registry.add_component<Engine::ECS::Components::Common::NetworkIp>(
        newEntity, Engine::ECS::Components::Common::NetworkIp(std::string(*address)));

    registry.registerComponent<Engine::ECS::Components::Common::Template>();
    registry.add_component<Engine::ECS::Components::Common::Template>(
        newEntity, Engine::ECS::Components::Common::Template(std::string(p_Template)));

    player->setEntity(newEntity);
    _players.push_back(player);
    FLAKKARI_LOG_INFO("client \"" + std::string(*address) + "\" added to game \"" + _name + "\"");

    // this->sendAllEntities(sceneGame, player);    // send all entities to the new player

    Protocol::Packet<Protocol::CommandId> packet;
    packet.header._commandId = Protocol::CommandId::REP_CONNECT;
    packet.header._apiVersion = player->getApiVersion();
    packet << newEntity;
    packet.injectString(sceneGame);
    packet.injectString(player->getName().value_or(""));
    packet.injectString(p_Template);
    ClientManager::GetInstance().sendPacketToClient(address, packet.serialize());
    ClientManager::UnlockInstance();

    Protocol::Packet<Protocol::CommandId> packet2;
    packet2.header._commandId = Protocol::CommandId::REQ_ENTITY_SPAWN;
    packet2.header._apiVersion = packet.header._apiVersion;
    packet2 << newEntity;
    packet2.injectString(sceneGame);
    packet2.injectString(p_Template);

    sendOnSameSceneExcept(sceneGame, packet2, player);
    return true;
}

bool Game::removePlayer(std::shared_ptr<Client> player)
{
    auto it = std::find(_players.begin(), _players.end(), player);
    if (it == _players.end())
        return false;

    auto sceneGame = player->getSceneName();
    auto &registry = _scenes[sceneGame];

    Engine::ECS::Entity entity = player->getEntity();

    Protocol::Packet<Protocol::CommandId> packet;
    packet.header._commandId = Protocol::CommandId::REQ_ENTITY_DESTROY;
    packet << entity;

    sendOnSameScene(sceneGame, packet);

    registry.kill_entity(entity);
    _players.erase(it);
    FLAKKARI_LOG_INFO("client \"" + std::string(*player->getAddress()) + "\" removed from game \"" + _name + "\"");
    return true;
}

bool Game::isRunning() const { return _running; }

std::string Game::getName() const { return _name; }

std::vector<std::shared_ptr<Client>> Game::getPlayers() const { return _players; }

} /* namespace Flakkari */
