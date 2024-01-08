/**************************************************************************
 * Flakkari Library v0.1.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Game.hpp
 * @brief Game class header. Can be used to create a game.
 *        A game is a collection of scenes.
 *        A scene is a collection of entities.
 *        An entity is a collection of components.
 *        A component is a collection of data.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.1.0
 * @date 2023-01-06
 **************************************************************************/

#ifndef GAME_HPP_
    #define GAME_HPP_

#include <string>
#include <fstream>
#include <thread>
#include <nlohmann/json.hpp>

#include "Engine/EntityComponentSystem/Registry.hpp"

namespace Flakkari {

    class Client;

class Game {
    public:
        /**
         * @brief Construct a new Game object and load the config file
         *        of the game.
         *
         * @param name  Name of the game (name of the file in Games/ folder)
         * @param config  Config of the game (config of the file in Games/ folder)
         */
        Game(const std::string &name, std::shared_ptr<nlohmann::json> config);
        ~Game();

        /**
         * @brief Load a scene from the game.
         *
         * @param name  Name of the scene to load.
         */
        void loadScene(const std::string &name);

        /**
         * @brief Update the game. This function is called every frame.
         *
         */
        void update();

        /**
         * @brief Start the game. This function is called when the game is
         *        launched. It will start the game loop.
         *
         */
        void start();

        /**
         * @brief Run the game. This function is called when the game is
         *        started. It will run the game loop.
         *
         */
        void run();

        /**
         * @brief Add a player to the game instance.
         *
         * @param player  Player to add to the game instance.
         * @return true  Player added successfully
         * @return false  Player not added
         */
        [[nodiscard]] bool addPlayer(std::shared_ptr<Client> player);

        /**
         * @brief Remove a player from the game instance.
         *
         * @param player  Player to remove from the game instance.
         * @return true  Player removed successfully
         * @return false  Player not removed
         */
        [[nodiscard]] bool removePlayer(std::shared_ptr<Client> player);

        /**
         * @brief Get if the game is running.
         *
         * @return true  Game is running
         * @return false  Game is not running
         */
        [[nodiscard]] bool isRunning() const;

        /**
         * @brief Get the Name object (name of the game).
         *
         * @return std::string  Name of the game
         */
        [[nodiscard]] std::string getName() const;

        /**
         * @brief Get the Players object (players of the game).
         *
         * @return std::vector<std::shared_ptr<Client>>  Players of the game
         */
        [[nodiscard]] std::vector<std::shared_ptr<Client>> getPlayers() const;

    protected:
    private:
        bool _running = false; // Is the game running
        std::thread _thread; // Thread of the game
        std::string _name; // Name of the game
        std::shared_ptr<nlohmann::json> _config; // Config of the game
        std::vector<std::shared_ptr<Client>> _players; // Players of the game
        float _deltaTime; // Time between two frames
        std::chrono::steady_clock::time_point _time; // Time of the last frame
        std::unordered_map<std::string /*sceneName*/, Engine::ECS::Registry /*content*/> _scenes; // Scenes of the game
};

} /* namespace Flakkari */

#endif /* !GAME_HPP_ */
