/**************************************************************************
 * Flakkari Library v0.1.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file CommandManager.hpp
 * @brief CommandManager is a class that handles commands from the admin user
 *        and executes them.
 *
 * @note Need to export `FLAKKARI_PASSWORD` environment variable to use some
 *       commands.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.1.0
 * @date 2023-01-05
 **************************************************************************/


#ifndef COMMANDMANAGER_HPP_
#define COMMANDMANAGER_HPP_

#include <iostream>
#include "Logger/Logger.hpp"
#include "config.h.in"

namespace Flakkari::Internals {

class CommandManager {
    public:
        /**
         * @brief Handle a command from the admin user and execute it.
         *
         * @note Need to export `FLAKKARI_PASSWORD` environment variable to use some
         *       commands.
         */
        static void handleCommand();

    private:
        /**
         * @brief Handle a command from the admin user and execute it.
         *
         * @note dont need admin permission to use this command.
         *
         * @param input  The command to execute.
         * @return true  If the command was executed.
         * @return false  If the command was not executed.
         *
         * @example version
         * @example help
         */
        [[nodiscard]] static bool handleOpenCommand(const std::string &input);
};

} // namespace Flakkari::Internals

#endif /* !COMMANDMANAGER_HPP_ */
