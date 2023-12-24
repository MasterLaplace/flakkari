/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** core
*/

#if defined(CLIENT_MODE)
#include "examples/UDPClient.hpp"
#else
#include "examples/UDPServer.hpp"
#endif

int main() {
#if defined(CLIENT_MODE)
    Flakkari::UDPClient client("localhost", 8080);
    return client.run();
#else
    Flakkari::UDPServer server("localhost", 8080);
    return server.run();
#endif
}
