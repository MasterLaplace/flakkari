/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** core
*/

#include "Server/UDPServer.hpp"

int main(int ac, const char *av[])
{
    if (ac != 2)
        return FLAKKARI_LOG_FATAL("Usage: ./r-type_server <ip>"), 84;
    try {
        Flakkari::UDPServer server(av[1], 8080);
        server.run();
    } catch (const std::exception &e) {
        if (std::string(e.what()) != "exit")
            return FLAKKARI_LOG_FATAL(e.what()), 84;
    } catch (...) {
        return FLAKKARI_LOG_FATAL("Unknown error"), 84;
    }
    return 0;
}
