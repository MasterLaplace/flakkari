/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-07
** File description:
** GameManager
*/

#include "GameManager.hpp"
#include "../Client/Client.hpp"

namespace Flakkari {

std::shared_ptr<GameManager> GameManager::_instance = nullptr;

#define STR_ADDRESS std::string(*client->getAddress())

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

std::vector<std::shared_ptr<Game>> GameManager::getGamesInstances()
{
    auto &_gamesInstances = getInstance()->_gamesInstances;
    std::vector<std::shared_ptr<Game>> gamesInstances;

    for (auto &game : _gamesInstances)
        gamesInstances.insert(gamesInstances.end(), game.second.begin(), game.second.end());
    return gamesInstances;
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

void GameManager::addClientToGame(std::string gameName, std::shared_ptr<Client> client)
{
    auto current = getInstance();
    auto &gamesStore = current->_gamesStore;
    auto &gamesInstances = current->_gamesInstances;
    auto &waitingClients = current->_waitingClients;
    if (gamesStore.find(gameName) == gamesStore.end())
        return FLAKKARI_LOG_ERROR("game not found"), void();

    auto &gameStore = gamesStore[gameName];
    auto &gameInstance = gamesInstances[gameName];

    auto minPlayers = gameStore->at("minPlayers").get<size_t>();
    auto maxPlayers = gameStore->at("maxPlayers").get<size_t>();
    auto maxInstances = gameStore->at("maxInstances").get<size_t>();
    auto lobby = gameStore->at("lobby").get<std::string>();

    if (gameInstance.empty() || gameInstance.back()->getPlayers().size() >= maxPlayers) {
        if (gameInstance.size() >= maxInstances) {
            FLAKKARI_LOG_ERROR("game \"" + gameName + "\"is full");
            waitingClients[gameName].push(client);
            return;
        }
        gameInstance.push_back(std::make_shared<Game>(gameName, gameStore));
        FLAKKARI_LOG_INFO("game \"" + gameName + "\" created");
    }

    if (gameInstance.back()->addPlayer(client)) {
        if (lobby == "Matchmaking" && gameInstance.back()->getPlayers().size() >= minPlayers && !gameInstance.back()->isRunning())
            gameInstance.back()->start();
        if (lobby == "OpenWorld" && !gameInstance.back()->isRunning())
            gameInstance.back()->start();
        return;
    }
    FLAKKARI_LOG_ERROR("could not add client \""+ STR_ADDRESS +"\" to game \"" + gameName + "\"");
}

void GameManager::removeClientFromGame(std::string gameName, std::shared_ptr<Client> client)
{
    auto &gamesStore = getInstance()->_gamesStore;
    auto &gamesInstances = getInstance()->_gamesInstances;
    auto &waitingClients = getInstance()->_waitingClients;
    if (gamesStore.find(gameName) == gamesStore.end())
        return FLAKKARI_LOG_ERROR("game not found"), void();

    auto &waitingQueue = waitingClients[gameName];

    auto minPlayers = gamesStore[gameName]->at("minPlayers").get<size_t>();

    for (auto &instance : gamesInstances[gameName]) {
        if (!instance->removePlayer(client))
            continue;

        if (instance->getPlayers().empty()) {
            gamesInstances[gameName].erase(
                std::find(
                    gamesInstances[gameName].begin(), gamesInstances[gameName].end(), instance
                )
            );
            FLAKKARI_LOG_INFO("game \"" + gameName + "\" removed");
        } else if (instance->getPlayers().size() > minPlayers) {
            FLAKKARI_LOG_INFO("game \"" + gameName + "\" is not full anymore");
            if (waitingQueue.empty())
                return;
            auto client = waitingQueue.front();
            if (instance->addPlayer(client)) {
                waitingQueue.pop();
                return;
            }
            FLAKKARI_LOG_WARNING("could not add client \""+ STR_ADDRESS +"\" to game \"" + gameName + "\"");
            if (client->isConnected())
                waitingQueue.pop();
        }
        return;
    }
    FLAKKARI_LOG_ERROR("could not remove client \""+ STR_ADDRESS +"\" from game \"" + gameName + "\"");
}

int GameManager::getIndexInWaitingQueue(std::string gameName, std::shared_ptr<Client> client)
{
    auto &waitingClients = getInstance()->_waitingClients;
    auto &gamesStore = getInstance()->_gamesStore;
    if (gamesStore.find(gameName) == gamesStore.end())
        return FLAKKARI_LOG_ERROR("game not found"), -1;

    auto tmpQueue = waitingClients[gameName];
    for (int i = 0; !tmpQueue.empty(); i++) {
        if (tmpQueue.front() == client)
            return FLAKKARI_LOG_INFO("client \""+ STR_ADDRESS +"\" found in waiting queue at " + std::to_string(i)), i;
        tmpQueue.pop();
    }
    return FLAKKARI_LOG_ERROR("client \""+ STR_ADDRESS +"\" not found in waiting queue"), -1;
}

} /* namespace Flakkari */
