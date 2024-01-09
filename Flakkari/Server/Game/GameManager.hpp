/**************************************************************************
 * Flakkari Library v0.1.0
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
 * @version 0.1.0
 * @date 2023-01-06
 **************************************************************************/


#ifndef GAMEMANAGER_HPP_
    #define GAMEMANAGER_HPP_

#include <memory>
#include <iostream>
#include <filesystem>
#include <queue>

#include "Game.hpp"
#include "Logger/Logger.hpp"

namespace Flakkari {

class GameManager {
    private:
        static std::shared_ptr<GameManager> _instance;

    public:
        std::unordered_map<std::string /*gameName*/, std::queue<std::shared_ptr<Client>> /*waitingClients*/> _waitingClients;
        std::unordered_map<std::string /*gameName*/, std::vector<std::shared_ptr<Game>> /*gamesInstances*/> _gamesInstances;
        std::unordered_map<std::string /*gameName*/, std::shared_ptr<nlohmann::json> /*data*/> _gamesStore;

    public:
        GameManager(const GameManager &) = delete;
        GameManager(const std::shared_ptr<GameManager> &) = delete;
        void operator=(const GameManager &) = delete;
        void operator=(const std::shared_ptr<GameManager> &) = delete;

        /**
         * @brief Construct a new GameManager object and load all games
         * already present in the Games folder
         *
         */
        GameManager();

        /**
         * @brief Destroy the GameManager object
         *
         */
        ~GameManager() = default;

        /**
         * @brief Get the instance of the GameManager
         *
         * @return std::shared_ptr<GameManager> instance of the GameManager
         */
        static std::shared_ptr<GameManager> getInstance();

        /**
         * @brief Add a game to the GameManager and load it from the Games folder
         *
         * @param gameName Game to add
         * @return 0 Game added
         * @return 1 Game not added (already exists)
         * @return 2 Game not added (certificate not valid) (not implemented)
         * @return 3 Game not added (corrupted game) (not implemented)
         */
        static int addGame(std::string gameName);

        /**
         * @brief Get the Game object
         *
         * @param gameName Game to get
         * @return std::shared_ptr<Game> Game
         *
         * @deprecated Use getGameInstance instead
         */
        static std::shared_ptr<Game> getGame(std::string gameName);

        /**
         * @brief Update a game from the GameManager
         *
         * @param gameName Game to update
         * @return 0 Game updated
         * @return 1 Game not updated (not found)
         */
        static int updateGame(std::string gameName);

        /**
         * @brief Remove a game from the GameManager
         *
         * @param gameName Game to remove
         * @return 0 Game removed
         * @return 1 Game not removed (not found)
         */
        static int removeGame(std::string gameName);

        /**
         * @brief List all games present in the GameManager
         *
         */
        static void listGames();

        /**
         * @brief Add a client to a game
         *
         * @param gameName Game to add the client to
         * @param client Client to add to the game
         */
        static void addClientToGame(std::string gameName, std::shared_ptr<Client> client);
};

} /* namespace Flakkari */

#endif /* !GAMEMANAGER_HPP_ */
