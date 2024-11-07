/**************************************************************************
 * Flakkari Library v0.3.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file GameManager.hpp
 * @brief GameManager is a class that manages all the games present
 *        in the Games folder. It loads them and stores them in a map.
 *        It also manages the waiting queue for each game.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.3.0
 * @date 2023-01-06
 **************************************************************************/

#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <filesystem>
#include <iostream>
#include <memory>
#include <queue>

#include "Game.hpp"
#include "Logger/Logger.hpp"

#ifdef _WIN32
#    undef max
#endif

namespace Flakkari {

class GameManager : public Singleton<GameManager> {
    private:
    std::unordered_map<std::string /*gameName*/, std::queue<std::shared_ptr<Client>> /*waitingClients*/>
        _waitingClients;
    std::unordered_map<std::string /*gameName*/, std::vector<std::shared_ptr<Game>> /*gamesInstances*/> _gamesInstances;
    std::unordered_map<std::string /*gameName*/, std::shared_ptr<nlohmann::json> /*data*/> _gamesStore;
    std::string _game_dir;

    public:
    /**
     * @brief Construct a new GameManager object and load all games
     * already present in the Games folder
     *
     */
    explicit GameManager();

    /**
     * @brief Destroy the GameManager object
     *
     */
    ~GameManager() = default;

    /**
     * @brief Add a game to the GameManager and load it from the Games folder
     *
     * @param gameName Game to add
     * @return 0 Game added
     * @return 1 Game not added (already exists)
     * @return 2 Game not added (certificate not valid) (not implemented)
     * @return 3 Game not added (corrupted game) (not implemented)
     */
    int addGame(std::string gameName);

    /**
     * @brief Get the Game object
     *
     * @param gameName Game to get
     * @return std::shared_ptr<Game> Game
     *
     * @deprecated Use getGameInstance instead
     */
    std::shared_ptr<Game> getGame(std::string gameName);

    /**
     * @brief Get the Games Instances object (all games loaded)
     *
     * @return std::vector<std::shared_ptr<Game>> Games Instances
     */
    std::vector<std::shared_ptr<Game>> getGamesInstances();

    /**
     * @brief Update a game from the GameManager
     *
     * @param gameName Game to update
     * @return 0 Game updated
     * @return 1 Game not updated (not found)
     */
    int updateGame(std::string gameName);

    /**
     * @brief Remove a game from the GameManager
     *
     * @param gameName Game to remove
     * @return 0 Game removed
     * @return 1 Game not removed (not found)
     */
    int removeGame(std::string gameName);

    /**
     * @brief List all games present in the GameManager
     *
     */
    void listGames();

    /**
     * @brief Add a client to a game
     *
     * @param gameName Game to add the client to
     * @param client Client to add to the game
     */
    void addClientToGame(std::string gameName, std::shared_ptr<Client> client);

    /**
     * @brief Remove a client from a game
     *
     * @param gameName Game to remove the client from
     * @param client Client to remove from the game
     */
    void removeClientFromGame(std::string gameName, std::shared_ptr<Client> client);

    /**
     * @brief Get the index of a client in the waiting queue
     *
     * @param gameName Game to get the index from
     * @param client Client to get the index of
     * @return int Index of the client in the waiting queue
     */
    int getIndexInWaitingQueue(std::string gameName, std::shared_ptr<Client> client);
};

} /* namespace Flakkari */

#endif /* !GAMEMANAGER_HPP_ */
