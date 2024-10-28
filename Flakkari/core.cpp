/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** core
*/

#include "Server/UDPServer.hpp"

int main(int ac, const char *av[])
{
    if (ac != 3)
        return FLAKKARI_LOG_ERROR("Usage: ./r-type_server <ip> <port>"), 84;
    try {
        Flakkari::UDPServer server(av[1], static_cast<unsigned short>(std::stoi(av[2])));
        server.run();
    } catch (const std::exception &e) {
        if (std::string(e.what()) != "exit")
            return FLAKKARI_LOG_FATAL(e.what()), 84;
    } catch (...) {
        return FLAKKARI_LOG_FATAL("Unknown error"), 84;
    }
    return 0;
}
