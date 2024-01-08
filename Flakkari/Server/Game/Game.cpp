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

void Game::loadScene(const std::string &sceneName)
{
    for (auto &scene : (*_config)["scenes"].items()) {
        for (auto sceneInfo : scene.value().items()) {
            if (sceneInfo.key() != sceneName)
                continue;
            Engine::ECS::Registry registry;
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
    return true;
}

bool Game::removePlayer(std::shared_ptr<Client> player)
{
    auto it = std::find(_players.begin(), _players.end(), player);
    if (it == _players.end())
        return false;
    _players.erase(it);
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
