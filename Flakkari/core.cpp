/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** core
*/

#include "Server/UDPServer.hpp"

int main()
{
    try {
        Flakkari::UDPServer server("localhost", 8080);
        server.run();
    } catch (const std::exception &e) {
        if (std::string(e.what()) != "exit")
            return FLAKKARI_LOG_FATAL(e.what()), 84;
    }
    return 0;
}
