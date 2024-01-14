/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
** File description:
** Game
*/

#include "Game.hpp"
#include "../Client/ClientManager.hpp"

namespace Flakkari {

Game::Game(const std::string &name, std::shared_ptr<nlohmann::json> config)
{
    _name = name;
    _config = config;
    _time = std::chrono::steady_clock::now();
    if ((*_config)["scenes"].empty())
        throw std::runtime_error("Game: no scenes found");
    loadScene((*_config)["startGame"]);
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
        return registry.add_system([ this ](Engine::ECS::Registry &r) {
            Engine::ECS::Systems::position(r, _deltaTime);
        }), void();
}

void Game::loadComponents (
    Engine::ECS::Registry &registry, const nl_component &components, Engine::ECS::Entity newEntity
) {
    for (auto &component : components.items())
    {
        auto componentName = component.key();
        auto componentContent = component.value();

        if (componentName == "Transform") {
            registry.registerComponent<Engine::ECS::Components::_2D::Transform>();
            Engine::ECS::Components::_2D::Transform transform;
            transform.position = Engine::Math::Vector2f(componentContent["position"]["x"], componentContent["position"]["y"]);
            transform.rotation = componentContent["rotation"];
            transform.scale = Engine::Math::Vector2f(componentContent["scale"]["x"], componentContent["scale"]["y"]);
            registry.add_component<Engine::ECS::Components::_2D::Transform>(newEntity, std::move(transform));
            continue;
        }

        if (componentName == "Movable") {
            registry.registerComponent<Engine::ECS::Components::_2D::Movable>();
            Engine::ECS::Components::_2D::Movable movable;
            movable.velocity = Engine::Math::Vector2f(componentContent["velocity"]["x"], componentContent["velocity"]["y"]);
            movable.acceleration = Engine::Math::Vector2f(componentContent["acceleration"]["x"], componentContent["acceleration"]["y"]);
            registry.add_component<Engine::ECS::Components::_2D::Movable>(newEntity, std::move(movable));
            continue;
        }
    }
}

void Game::loadEntityFromTemplate (
    Engine::ECS::Registry &registry, const nl_entity &entity, const nl_template &templates
) {
    Engine::ECS::Entity newEntity = registry.spawn_entity();

    for (auto &componentInfo : entity.begin().value().items()) {
        for (auto &templateInfo : templates.items()) {
            if (templateInfo.value().begin().key() != componentInfo.value())
                continue;
            loadComponents(registry, templateInfo.value().begin().value(), newEntity);
        }
    }
}

void Game::loadScene(const std::string &sceneName)
{
    for (auto &scene : (*_config)["scenes"].items()) {
        for (auto sceneInfo : scene.value().items()) {
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

void Game::sendOnSameScene(const std::string &sceneName, const Network::Buffer &message)
{
    for (auto &player : _players) {
        if (!player || !player->isConnected())
            continue;
        if (player->getSceneName() != sceneName)
            continue;
        ClientManager::sendPacketToClient(player->getAddress(), message);
    }
}

void Game::checkDisconnect()
{
    for (auto &player : _players) {
        if (!player || player->isConnected())
            continue;
        Protocol::Packet<Protocol::CommandId> packet;
        packet.header._commandId = Protocol::CommandId::REQ_ENTITY_DESTROY;
        packet << player->getSceneName().size();
        packet << player->getSceneName().c_str();
        packet << player->getEntity();

        sendOnSameScene(player->getSceneName(), packet.serialize());

        _scenes[player->getSceneName()].kill_entity(player->getEntity());
        removePlayer(player);
    }
}

void Game::sendUpdatePosition (
    std::shared_ptr<Client> player,
    Engine::ECS::Components::_2D::Transform pos,
    Engine::ECS::Components::_2D::Movable vel
) {
    Protocol::Packet<Protocol::CommandId> packet;
    packet.header._commandId = Protocol::CommandId::REQ_ENTITY_MOVED;
    packet << player->getEntity();
    packet << pos.position.x;
    packet << pos.position.y;
    packet << pos.rotation;
    packet << pos.scale.x;
    packet << pos.scale.y;
    packet << vel.velocity.x;
    packet << vel.velocity.y;
    FLAKKARI_LOG_LOG(
        "packet sent: <Id: "
        + std::to_string(player->getEntity())
        + ", Pos: (" + std::to_string(pos.position.x) + ", " + std::to_string(pos.position.y) + ")"
        + ", Vel: (" + std::to_string(vel.velocity.x) + ", " + std::to_string(vel.velocity.y) + ")>"
    );
    sendOnSameScene(player->getSceneName(), packet.serialize());
}

void Game::handleEvent(std::shared_ptr<Client> player, Protocol::Packet<Protocol::CommandId> packet)
{
    auto sceneName = player->getSceneName();
    auto entity = player->getEntity();
    auto &registry = _scenes[sceneName];
    auto &control = registry.getComponents<Engine::ECS::Components::_2D::Control>();
    auto &velocity = registry.getComponents<Engine::ECS::Components::_2D::Movable>();
    auto &position = registry.getComponents<Engine::ECS::Components::_2D::Transform>();
    auto &networkEvent = registry.getComponents<Engine::ECS::Components::Common::NetworkEvent>();
    auto &ctrl = control[entity];
    auto &vel = velocity[entity];
    auto pos = position[entity];
    auto &netEvent = networkEvent[entity];

    if (!ctrl.has_value() || !vel.has_value() || !pos.has_value())
        return;
    Protocol::Event event = Network::Serializer::deserialize<Protocol::Event>(packet.payload);
    if (event.id == Protocol::EventId::MOVE_UP && ctrl->up) {
        if (netEvent->events.size() < int(event.id))
            netEvent->events.resize(int(event.id) + 1);
        netEvent->events[int(event.id)] = int(event.state);

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->velocity.dy = -1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->velocity.dy = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::MOVE_DOWN && ctrl->down) {
        if (netEvent->events.size() < int(event.id))
            netEvent->events.resize(int(event.id) + 1);
        netEvent->events[int(event.id)] = int(event.state);

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->velocity.dy = 1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->velocity.dy = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::MOVE_LEFT && ctrl->left) {
        if (netEvent->events.size() < int(event.id))
            netEvent->events.resize(int(event.id) + 1);
        netEvent->events[int(event.id)] = int(event.state);

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->velocity.dx = -1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->velocity.dx = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::MOVE_RIGHT && ctrl->right) {
        if (netEvent->events.size() < int(event.id))
            netEvent->events.resize(int(event.id) + 1);
        netEvent->events[int(event.id)] = int(event.state);

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::PRESSED)
            vel->velocity.dx = 1;
        if (event.state == Protocol::EventState::RELEASED)
            vel->velocity.dx = 0;
        sendUpdatePosition(player, pos.value(), vel.value());
        return;
    }
    if (event.id == Protocol::EventId::SHOOT && ctrl->shoot) {
        if (netEvent->events.size() < int(event.id))
            netEvent->events.resize(int(event.id) + 1);
        netEvent->events[int(event.id)] = int(event.state);

        FLAKKARI_LOG_INFO("event: " + std::to_string(int(event.id)) + " " + std::to_string(int(event.state)));

        if (event.state == Protocol::EventState::RELEASED) {
            Protocol::Packet<Protocol::CommandId> packet;
            packet.header._commandId = Protocol::CommandId::REQ_ENTITY_SHOOT;
            packet << player->getSceneName().size();
            packet << player->getSceneName().c_str();
            packet << player->getEntity();
            // create a bullet with player as parent
            sendOnSameScene(player->getSceneName(), packet.serialize());
        }
        return;
    }
}

void Game::updateIncomingPackets(unsigned char maxMessagePerFrame)
{
    for (auto &player : _players) {
        if (!player->isConnected())
            continue;
        auto &packets = player->_receiveQueue;
        auto messageCount = maxMessagePerFrame;

        while (!packets.empty() && messageCount > 0) {
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

    auto sceneGame = (*_config)["startGame"];
    auto &registry = _scenes[sceneGame];

    player->setSceneName(sceneGame);

    Engine::ECS::Entity newEntity = registry.spawn_entity();
    player->setEntity(newEntity);

    _players.push_back(player);
    FLAKKARI_LOG_INFO("client \""+ std::string(*player->getAddress()) +"\" added to game \""+ _name +"\"");
    return true;
}

bool Game::removePlayer(std::shared_ptr<Client> player)
{
    auto it = std::find(_players.begin(), _players.end(), player);
    if (it == _players.end())
        return false;
    _players.erase(it);
    FLAKKARI_LOG_INFO("client \""+ std::string(*player->getAddress()) +"\" removed from game \""+ _name +"\"");
    return true;
}

bool Game::isRunning() const {
    return _running;
}

std::string Game::getName() const {
    return _name;
}

std::vector<std::shared_ptr<Client>> Game::getPlayers() const {
    return _players;
}

} /* namespace Flakkari */
