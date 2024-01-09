/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-06
** File description:
** Game
*/

#include "Game.hpp"
#include "../Client/Client.hpp"

namespace Flakkari {

Game::Game(const std::string &name, std::shared_ptr<nlohmann::json> config)
{
    _name = name;
    _config = config;
    _time = std::chrono::steady_clock::now();
    if ((*_config)["scenes"].empty())
        throw std::runtime_error("Game: no scenes found");
    loadScene((*_config)["startScene"]);
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

void Game::loadComponents(Engine::ECS::Registry &registry, const nl_component &components, Engine::ECS::Entity newEntity)
{
    for (auto &component : components.items())
    {
        auto componentName = component.key();
        auto componentContent = component.value();

        if (componentName == "Transform") {
            registry.registerComponent<Engine::ECS::Components::_2D::Transform>();
            Engine::ECS::Components::_2D::Transform transform;
            transform.position = Engine::Math::Vector2d(componentContent["position"]["x"], componentContent["position"]["y"]);
            transform.rotation = componentContent["rotation"];
            transform.scale = Engine::Math::Vector2d(componentContent["scale"]["x"], componentContent["scale"]["y"]);
            registry.add_component<Engine::ECS::Components::_2D::Transform>(newEntity, std::move(transform));
            return;
        }

        if (componentName == "Movable") {
            registry.registerComponent<Engine::ECS::Components::_2D::Movable>();
            Engine::ECS::Components::_2D::Movable movable;
            movable.velocity = Engine::Math::Vector2d(componentContent["velocity"]["x"], componentContent["velocity"]["y"]);
            movable.acceleration = Engine::Math::Vector2d(componentContent["acceleration"]["x"], componentContent["acceleration"]["y"]);
            registry.add_component<Engine::ECS::Components::_2D::Movable>(newEntity, std::move(movable));
            return;
        }
    }
}

void Game::loadEntityFromTemplate(Engine::ECS::Registry &registry, const nl_entity &entity, const nl_template &templates)
{
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

void Game::update()
{
    auto now = std::chrono::steady_clock::now();
    _deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - _time).count();
    _time = now;

    for (auto &scene : _scenes)
        scene.second.run_systems();
}

void Game::start()
{
    _running = true;
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
