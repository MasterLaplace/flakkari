/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-07
** File description:
** GameManager
*/

#include "GameManager.hpp"

namespace Flakkari {

std::shared_ptr<GameManager> GameManager::_instance = nullptr;

GameManager::GameManager()
{
    for (const auto & entry : std::filesystem::directory_iterator("../Games")) {
        std::string gameName = entry.path().string().substr(9);
        if (_gamesStore.find(gameName) != _gamesStore.end()) {
            FLAKKARI_LOG_ERROR("game already loaded");
            continue;
        }
        std::ifstream configFile("../Games/" + gameName + "/config.cfg");
        nlohmann::json config;

        if (!configFile.is_open()) {
            FLAKKARI_LOG_ERROR("could not open config file for \"" + gameName + "\" game");
            continue;
        }
        configFile >> config;

        if (config["scenes"].empty()) {
            FLAKKARI_LOG_ERROR("no scenes found");
            continue;
        }

        _gamesStore[gameName] = std::make_shared<nlohmann::json>(config);
        FLAKKARI_LOG_INFO("\"" + gameName + "\" game loaded");
    }
}

std::shared_ptr<GameManager> GameManager::getInstance()
{
    if (!_instance)
        _instance = std::make_shared<GameManager>();
    return _instance;
}

int GameManager::addGame(std::string gameName)
{
    auto &_gamesStore = getInstance()->_gamesStore;
    if (_gamesStore.find(gameName) != _gamesStore.end())
        return FLAKKARI_LOG_ERROR("game already loaded"), 1;
    std::ifstream configFile("../Games/" + gameName + "/config.cfg");
    nlohmann::json config;

    if (!configFile.is_open())
        return FLAKKARI_LOG_ERROR("could not open config file"), 3;
    configFile >> config;

    if (config["scenes"].empty())
        return FLAKKARI_LOG_ERROR("no scenes found"), 3;

    _gamesStore[gameName] = std::make_shared<nlohmann::json>(config);
    FLAKKARI_LOG_INFO("\"" + gameName + "\" game loaded");
    return 0;
}

std::shared_ptr<Game> GameManager::getGame(std::string gameName)
{
    auto &_gamesStore = getInstance()->_gamesStore;
    if (_gamesStore.find(gameName) == _gamesStore.end())
        return FLAKKARI_LOG_ERROR("game not found"), nullptr;
    return std::make_shared<Game>(gameName, _gamesStore[gameName]);
}

int GameManager::updateGame(std::string gameName)
{
    auto &_gamesStore = getInstance()->_gamesStore;
    if (_gamesStore.find(gameName) == _gamesStore.end())
        return FLAKKARI_LOG_ERROR("game not found"), 1;
    std::ifstream configFile("../Games/" + gameName + "/config.cfg");
    nlohmann::json config;

    if (!configFile.is_open())
        return FLAKKARI_LOG_ERROR("could not open config file"), 3;
    configFile >> config;

    _gamesStore[gameName] = std::make_shared<nlohmann::json>(config);
    FLAKKARI_LOG_INFO("\"" + gameName + "\" game updated");
    return 0;
}

int GameManager::removeGame(std::string gameName)
{
    auto &_gamesStore = getInstance()->_gamesStore;
    if (_gamesStore.find(gameName) == _gamesStore.end())
        return FLAKKARI_LOG_ERROR("game not found"), 1;

    _gamesStore[gameName].reset();
    _gamesStore.erase(gameName);
    FLAKKARI_LOG_INFO("\"" + gameName + "\" game removed");
    return 0;
}

void GameManager::listGames()
{
    auto &_gamesStore = getInstance()->_gamesStore;
    std::string gamesList = "Games list:\n";

    for (const auto &game : _gamesStore)
        gamesList += " - " + game.first + "\n";
    FLAKKARI_LOG_INFO(gamesList);
}

} /* namespace Flakkari */
