/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** core
*/

#include "Server/UDPServer.hpp"

#include "Protocol/Packet.hpp"

int main() {
    Flakkari::UDPServer server("localhost", 8080);
    return server.run();
}
