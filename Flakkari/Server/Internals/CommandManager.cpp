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

std::regex CommandManager::PASSWORD_REGEX("^unlock [\\w-]+");
bool CommandManager::_unlocked = false;

static const std::string HELP_MESSAGE = "Commands:\n"
                                "unlock <password>\n"
                                "lock (admin only)\n"
                                "version\n"
                                "help\n"
                                "exit (admin only)";


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

bool CommandManager::handlePasswordCommand(const std::string &input)
{
    if (!std::regex_match(input, PASSWORD_REGEX))
        return false;

    const char *password = std::getenv("FLAKKARI_PASSWORD");

    if (password == nullptr || !*password) {
        FLAKKARI_LOG_WARNING("No password set: please set FLAKKARI_PASSWORD environment variable");
        return true;
    }

    if (input.substr(7) == password) {
        _unlocked = true;
        FLAKKARI_LOG_INFO("Command manager unlocked");
        return true;
    } else {
        FLAKKARI_LOG_WARNING("Wrong password");
        return true;
    }

    if (input == "lock" && _unlocked) {
        _unlocked = false;
        FLAKKARI_LOG_INFO("Command manager locked");
        return true;
    }
    return false;
}

bool CommandManager::handleAdminCommand(const std::string &input)
{
    if (!_unlocked) {
        FLAKKARI_LOG_WARNING("Please unlock or type 'help' for a list of commands");
        return true;
    }

    if (input == "exit") {
        FLAKKARI_LOG_INFO("Exiting...");
        throw std::runtime_error("exit");
    }

    return false;
}

void CommandManager::handleCommand()
{
    std::string input;
    std::getline(std::cin, input);

    if (handleOpenCommand(input))
        return;

    if (handlePasswordCommand(input))
        return;

    if (handleAdminCommand(input))
        return;

    FLAKKARI_LOG_WARNING("Unknown command, type 'help' for a list of commands");
}
