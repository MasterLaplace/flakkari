/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-05
** File description:
** CommandManager
*/

#include "CommandManager.hpp"

using namespace Flakkari::Internals;

static const std::string HELP_MESSAGE = "Commands:\n"
                                "version\n"
                                "help\n";


bool CommandManager::handleOpenCommand(const std::string &input)
{
    if (input == "version") {
        FLAKKARI_LOG_INFO(FLAKKARI_VERSION_STRING);
        return true;
    } else if (input == "help") {
        FLAKKARI_LOG_INFO(HELP_MESSAGE);
        return true;
    }
    return false;
}
void CommandManager::handleCommand()
{
    std::string input;
    std::getline(std::cin, input);

    if (handleOpenCommand(input))
        return;

    FLAKKARI_LOG_WARNING("Unknown command, type 'help' for a list of commands");
}
