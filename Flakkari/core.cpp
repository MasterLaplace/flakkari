/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** core
*/

#include "examples/UDPServer.hpp"

int main() {
    Flakkari::UDPServer server("localhost", 8080);
    return server.run();
}
