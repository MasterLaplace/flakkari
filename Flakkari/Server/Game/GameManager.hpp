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

#include "Logger/Logger.hpp"

namespace Flakkari {

class GameManager {
    private:
        static std::shared_ptr<GameManager> _instance;

    public:
        GameManager(const GameManager &) = delete;
        GameManager(const std::shared_ptr<GameManager> &) = delete;
        void operator=(const GameManager &) = delete;
        void operator=(const std::shared_ptr<GameManager> &) = delete;

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

};

} /* namespace Flakkari */

#endif /* !GAMEMANAGER_HPP_ */
