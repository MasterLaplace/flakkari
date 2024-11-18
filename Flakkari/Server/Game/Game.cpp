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

void Game::loadSystems(Engine::ECS::Registry &registry, const std::string &sceneName, const std::string &sysName)
{
    if (sysName == "position")
        registry.add_system([this](Engine::ECS::Registry &r) { Engine::ECS::Systems::_2D::position(r, _deltaTime); });

    // else if (sysName == "apply_movable")
    //     registry.add_system(
    //         [this](Engine::ECS::Registry &r) { Engine::ECS::Systems::_3D::apply_movable(r, _deltaTime); });

    else if (sysName == "spawn_enemy")
        registry.add_system([this, sceneName](Engine::ECS::Registry &r) {
            std::string templateName;
            Engine::ECS::Entity entity;
            if (Engine::ECS::Systems::_3D::spawn_enemy(r, templateName, entity))
            {
                Protocol::Packet<Protocol::CommandId> packet;
                packet.header._commandId = Protocol::CommandId::REQ_ENTITY_SPAWN;
                packet << entity;
                packet.injectString(sceneName);
                packet.injectString(templateName);

                Protocol::PacketFactory::addComponentsToPacketByEntity(packet, r, entity);

                this->sendOnSameScene(sceneName, packet);
            }
        });

    else if (sysName == "spawn_random_within_skybox")
        registry.add_system([this, sceneName](Engine::ECS::Registry &r) {
            std::vector<Engine::ECS::Entity> entities(10);
            Engine::ECS::Systems::_3D::spawn_random_within_skybox(r, entities);

            for (auto &entity : entities)
            {
                Protocol::Packet<Protocol::CommandId> packet;
                packet.header._commandId = Protocol::CommandId::REQ_ENTITY_UPDATE;
                packet << entity;

                Protocol::PacketFactory::addComponentsToPacketByEntity(packet, r, entity);

                this->sendOnSameScene(sceneName, packet);
            }
        });

    else if (sysName == "handle_collisions")
        registry.add_system([this,sceneName ](Engine::ECS::Registry &r) {
            std::unordered_map<Engine::ECS::Entity, bool> entities;
            Engine::ECS::Systems::_3D::handle_collisions(r, entities);

            for (auto &entity : entities)
            {
                if (!entity.second)
                {
                    Protocol::Packet<Protocol::CommandId> packet;
                    packet.header._commandId = Protocol::CommandId::REQ_ENTITY_DESTROY;
                    packet << entity.first;
                    this->sendOnSameScene(sceneName, packet);
                    continue;
                }

                Protocol::Packet<Protocol::CommandId> packet;
                packet.header._commandId = Protocol::CommandId::REQ_ENTITY_UPDATE;
                packet << entity.first;

                Protocol::PacketFactory::addComponentsToPacketByEntity(packet, r, entity.first);

                this->sendOnSameScene(sceneName, packet);
            }
        });
}

void Game::loadEntityFromTemplate(Engine::ECS::Registry &registry, const nl_entity &entity,
                                  const nl_template &templates)
{
    Engine::ECS::Entity newEntity = registry.spawn_entity();

    for (auto &templateInfo : templates.items())
    {
        if (entity.value() != templateInfo.value().begin().key())
            continue;
        Engine::ECS::Factory::RegistryEntityByTemplate(registry, newEntity, templateInfo.value().begin().value(), templates);
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
                loadSystems(registry, sceneName, system.value());

            for (auto &entity : sceneInfo.value()["entities"].items())
                loadEntityFromTemplate(registry, entity, sceneInfo.value()["templates"]);

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

        player->addPacketToSendQueue(packet);
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

        player->addPacketToSendQueue(packet);
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
    packet << pos._position.vec.x;
    packet << pos._position.vec.y;
    packet << pos._rotation;
    packet << pos._scale.vec.x;
    packet << pos._scale.vec.y;
    packet << vel._velocity.vec.x;
    packet << vel._velocity.vec.y;
    packet << vel._acceleration.vec.x;
    packet << vel._acceleration.vec.y;

    FLAKKARI_LOG_LOG("packet size: " + std::to_string(packet.size()) +
                     " bytes\n"
                     "packet sent: <Id: " +
                     std::to_string(player->getEntity()) + ", Pos: (" + std::to_string(pos._position.vec.x) + ", " +
                     std::to_string(pos._position.vec.y) + ")" + ", Vel: (" + std::to_string(vel._velocity.vec.x) +
                     ", " + std::to_string(vel._velocity.vec.y) + ")" + ", Acc: (" +
                     std::to_string(vel._acceleration.vec.x) + ", " + std::to_string(vel._acceleration.vec.y) + ")" +
                     ">");
    sendOnSameScene(player->getSceneName(), packet);
}

void Game::sendUpdatePosition(std::shared_ptr<Client> player, Engine::ECS::Components::_3D::Transform pos,
                              Engine::ECS::Components::_3D::Movable vel)
{
    Protocol::Packet<Protocol::CommandId> packet;
    packet.header._commandId = Protocol::CommandId::REQ_ENTITY_MOVED;
    packet << player->getEntity();
    packet << pos._position.vec.x;
    packet << pos._position.vec.y;
    packet << pos._position.vec.z;
    packet << pos._rotation.vec.x;
    packet << pos._rotation.vec.y;
    packet << pos._rotation.vec.z;
    packet << pos._scale.vec.x;
    packet << pos._scale.vec.y;
    packet << pos._scale.vec.z;
    packet << vel._velocity.vec.x;
    packet << vel._velocity.vec.y;
    packet << vel._velocity.vec.z;
    packet << vel._acceleration.vec.x;
    packet << vel._acceleration.vec.y;
    packet << vel._acceleration.vec.z;

    FLAKKARI_LOG_LOG("packet size: " + std::to_string(packet.size()) +
                     " bytes\n"
                     "packet sent: <Id: " +
                     std::to_string(player->getEntity()) + ", Pos: (" + std::to_string(pos._position.vec.x) + ", " +
                     std::to_string(pos._position.vec.y) + ", " + std::to_string(pos._position.vec.z) + ")" +
                     ", Vel: (" + std::to_string(vel._velocity.vec.x) + ", " + std::to_string(vel._velocity.vec.y) +
                     ", " + std::to_string(vel._velocity.vec.z) + ")" + ", Acc: (" +
                     std::to_string(vel._acceleration.vec.x) + ", " + std::to_string(vel._acceleration.vec.y) + ", " +
                     std::to_string(vel._acceleration.vec.z) + ")" + ">");
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
    if (event.id == Protocol::EventId::MOVE_UP && ctrl->_up)
    {
        if (netEvent->events.size() < size_t(event.id))
            netEvent->events.resize(size_t(event.id) + 1);
        netEvent->events[size_t(event.id)] = (unsigned short) event.state;

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->_velocity.vec.y = -1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->_velocity.vec.y = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::MOVE_DOWN && ctrl->_down)
    {
        if (netEvent->events.size() < size_t(event.id))
            netEvent->events.resize(size_t(event.id) + 1);
        netEvent->events[size_t(event.id)] = (unsigned short) event.state;

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->_velocity.vec.y = 1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->_velocity.vec.y = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::MOVE_LEFT && ctrl->_left)
    {
        if (netEvent->events.size() < size_t(event.id))
            netEvent->events.resize(size_t(event.id) + 1);
        netEvent->events[size_t(event.id)] = (unsigned short) event.state;

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->_velocity.vec.x = -1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->_velocity.vec.x = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::MOVE_RIGHT && ctrl->_right)
    {
        if (netEvent->events.size() < size_t(event.id))
            netEvent->events.resize(size_t(event.id) + 1);
        netEvent->events[size_t(event.id)] = (unsigned short) event.state;

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->_velocity.vec.x = 1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->_velocity.vec.x = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::SHOOT && ctrl->_shoot)
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

void Game::handleEvents(std::shared_ptr<Client> player, Protocol::Packet<Protocol::CommandId> packet)
{
    auto sceneName = player->getSceneName();
    auto entity = player->getEntity();
    auto &registry = _scenes[sceneName];
    auto &ctrl = registry.getComponents<Engine::ECS::Components::_3D::Control>()[entity];
    auto &vel = registry.getComponents<Engine::ECS::Components::_3D::Movable>()[entity];
    auto &pos = registry.getComponents<Engine::ECS::Components::_3D::Transform>()[entity];

    if (!ctrl.has_value() || !vel.has_value() || !pos.has_value())
        return;

    // there is the number of the events in the two first (size of ushort) byte of the payload
    uint16_t count_events = *(uint16_t *) packet.payload.data();
    // there is the number of the axis events in the two next (size of ushort) byte of the payload after the events
    uint16_t count_axis =
        *(uint16_t *) (packet.payload.data() + sizeof(uint16_t) + count_events * sizeof(Protocol::Event));

    // jump to the first event
    auto data = packet.payload.data() + sizeof(uint16_t);

    for (uint16_t i = 0; i < count_events; ++i)
    {
        Protocol::Event event = *(Protocol::Event *) (data + i * sizeof(Protocol::Event));

        if (event.id == Protocol::EventId::MOVE_FRONT && ctrl->_move_front)
        {
            Engine::Math::Vector3f direction = pos->_rotation * Engine::Math::Vector3f(0, 0, 1) * vel->_velocity;
            pos->_position += direction * _deltaTime;
            continue;
        }
        else if (event.id == Protocol::EventId::MOVE_BACK && ctrl->_move_down)
        {
            Engine::Math::Vector3f direction = pos->_rotation * Engine::Math::Vector3f(0, 0, -1) * vel->_velocity;
            pos->_position += direction * _deltaTime;
            continue;
        }
    }

    // jump to the first axis event
    data += count_events * sizeof(Protocol::Event);

    for (uint16_t i = 0; i < count_axis; ++i)
    {
        Protocol::EventAxis event = *(Protocol::EventAxis *) (data + i * sizeof(Protocol::EventAxis));

        if (event.id == Protocol::EventAxisId::LOOK_RIGHT && ctrl->_look_right)
        {
            pos->_rotation.vec.y += event.value * 100 * _deltaTime;
            continue;
        }
        else if (event.id == Protocol::EventAxisId::LOOK_LEFT && ctrl->_look_left)
        {
            pos->_rotation.vec.y -= event.value * 100 * _deltaTime;
            continue;
        }
        else if (event.id == Protocol::EventAxisId::LOOK_UP && ctrl->_look_up)
        {
            pos->_rotation.vec.x += event.value * 100 * _deltaTime;
            continue;
        }
        else if (event.id == Protocol::EventAxisId::LOOK_DOWN && ctrl->_look_down)
        {
            pos->_rotation.vec.x -= event.value * 100 * _deltaTime;
            continue;
        }
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

            else if (packet.header._commandId == Protocol::CommandId::REQ_USER_UPDATES)
                handleEvents(player, packet);

            else if (packet.header._commandId == Protocol::CommandId::REQ_HEARTBEAT)
            {
                Protocol::Packet<Protocol::CommandId> repPacket;
                repPacket.header._commandId = Protocol::CommandId::REP_HEARTBEAT;
                repPacket.header._apiVersion = packet.header._apiVersion;

                player->addPacketToSendQueue(repPacket);
            }
        }
    }
}

void Game::updateOutcomingPackets(unsigned char maxMessagePerFrame)
{
    for (auto &player : _players)
    {
        if (!player->isConnected())
            continue;
        auto &packets = player->getSendQueue();
        auto messageCount = maxMessagePerFrame;

        Network::Buffer buffer;

        while (!packets.empty() && messageCount > 0)
        {
            auto packet = packets.pop_front();
            messageCount--;

            buffer += packet.serialize();

            ClientManager::GetInstance().sendPacketToClient(player->getAddress(), buffer);
            ClientManager::UnlockInstance();
        }
    }
}

void Game::update()
{
#if defined(USE_SFML) && defined(FLAKKARI_GRAPHICS_MODE)
    while (const std::optional event = window.pollEvent())
    {
        // Window closed or escape key pressed: exit
        if (event->is<sf::Event::Closed>() ||
            (event->is<sf::Event::KeyPressed>() &&
             event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            window.close();
    }
    window.clear();
#else if defined(USE_RAYLIB) && defined(FLAKKARI_GRAPHICS_MODE)
    UpdateCamera(&camera, CAMERA_FREE);

    if (IsKeyPressed('Z'))
        camera.target = Vector3{0.0f, 0.0f, 0.0f};

    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);
#endif

    auto now = std::chrono::steady_clock::now();
    _deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - _time).count();
    _time = now;

    checkDisconnect();

    updateIncomingPackets();

    for (auto &scene : _scenes)
    {
        auto &registry = scene.second;

        registry.run_systems();

        auto &transforms = registry.getComponents<Engine::ECS::Components::_3D::Transform>();
        auto &boxs = registry.getComponents<Engine::ECS::Components::_3D::BoxCollider>();
        auto &spheres = registry.getComponents<Engine::ECS::Components::_3D::SphereCollider>();
        auto &tags = registry.getComponents<Engine::ECS::Components::Common::Tag>();

        for (Engine::ECS::Entity i(0); i < transforms.size(); ++i)
        {
            auto &transform = transforms[i];
            auto &box = boxs[i];
            auto &sphere = spheres[i];
            auto &tag = tags[i];

            if (!transform.has_value() || !tag.has_value())
                continue;

#if defined(USE_SFML) && defined(FLAKKARI_GRAPHICS_MODE)
            if (tag->tag == "Player")
            {
                sf::CircleShape circle(sphere->_radius);
                circle.setFillColor(sf::Color::Blue);
                circle.setPosition(sf::Vector2f(transform->_position.vec.x + 120, transform->_position.vec.y + 120));
                window.draw(circle);
            }
            else if (tag->tag == "Enemy")
            {
                sf::CircleShape circle(sphere->_radius);
                circle.setFillColor(sf::Color::Red);
                circle.setPosition(sf::Vector2f(transform->_position.vec.x + 120, transform->_position.vec.y + 120));
                window.draw(circle);
            }
            else if (tag->tag == "Bullet")
            {
                sf::RectangleShape rectangle(sf::Vector2f(box->_size.vec.x, box->_size.vec.y));
                rectangle.setScale(sf::Vector2f(transform->_scale.vec.x, transform->_scale.vec.y));
                rectangle.setPosition(sf::Vector2f(transform->_position.vec.x + 120, transform->_position.vec.y + 120));
                rectangle.setFillColor(sf::Color::Yellow);
                window.draw(rectangle);
            }
#else if defined(USE_RAYLIB) && defined(FLAKKARI_GRAPHICS_MODE)
            if (tag->tag == "Player")
            {
                Vector3 position = {transform->_position.vec.x, transform->_position.vec.y, transform->_position.vec.z};
                DrawSphere(position, sphere->_radius, BLUE);
            }
            else if (tag->tag == "Enemy")
            {
                Vector3 position = {transform->_position.vec.x, transform->_position.vec.y, transform->_position.vec.z};
                DrawSphere(position, sphere->_radius, RED);
            }
            else if (tag->tag == "Bullet")
            {
                Vector3 position = {transform->_position.vec.x, transform->_position.vec.y, transform->_position.vec.z};
                DrawCube(position, box->_size.vec.x, box->_size.vec.y, box->_size.vec.z, YELLOW);
            }
#endif
        }
    }

    updateOutcomingPackets();

#if defined(USE_SFML) && defined(FLAKKARI_GRAPHICS_MODE)
    window.display();
#else if defined(USE_RAYLIB) && defined(FLAKKARI_GRAPHICS_MODE)
    DrawGrid(500, 1.0f);

    EndMode3D();

    DrawFPS(10, 10);

    EndDrawing();
#endif
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
#if defined(USE_SFML) && defined(FLAKKARI_GRAPHICS_MODE)
    window.create(sf::VideoMode(sf::Vector2u(800, 600)), "Flakkari Graphics: " + _name);

    while (_running || window.isOpen())
        update();
#else if defined(USE_RAYLIB) && defined(FLAKKARI_GRAPHICS_MODE)
    SetTraceLogLevel(LOG_NONE);
    InitWindow(800, 600, std::string("Flakkari Graphics: " + _name).c_str());

    // Define the camera to look into our 3d world
    camera.position = Vector3{0.0f, 10.0f, 10.0f}; // Camera position
    camera.target = Vector3{0.0f, 0.0f, 0.0f};     // Camera looking at point
    camera.up = Vector3{0.0f, 1.0f, 0.0f};         // Camera up vector (rotation towards target)
    camera.fovy = 120.0f;                          // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;        // Camera mode type
    while (_running)

    while (_running && !WindowShouldClose())
        update();

    KillWindow();
#endif
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

    Engine::ECS::Factory::RegistryEntityByTemplate(registry, newEntity, player_info.value_or(nullptr));
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

    Protocol::Packet<Protocol::CommandId> packet;
    packet.header._commandId = Protocol::CommandId::REP_CONNECT;
    packet.header._apiVersion = player->getApiVersion();
    packet << newEntity;
    packet.injectString(sceneGame);
    packet.injectString(player->getName().value_or(""));
    packet.injectString(p_Template);

    player->addPacketToSendQueue(packet);

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

    _players.erase(it);
    registry.kill_entity(entity);

    sendOnSameScene(sceneGame, packet);
    FLAKKARI_LOG_INFO("client \"" + std::string(*player->getAddress()) + "\" removed from game \"" + _name + "\"");
    return true;
}

bool Game::isRunning() const { return _running; }

std::string Game::getName() const { return _name; }

std::vector<std::shared_ptr<Client>> Game::getPlayers() const { return _players; }

} /* namespace Flakkari */
