/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-10-27
** File description:
** Network
*/

#include "Network.hpp"

namespace Flakkari::Network {

void init()
{
#ifdef _WIN32
    WSADATA WSAData;

    if (::WSAStartup(MAKEWORD(2, 2), &WSAData) != NO_ERROR)
        throw std::runtime_error("WSAStartup failed");
#endif
}

void cleanup()
{
#ifdef _WIN32
    ::WSACleanup();
#endif
}

} // namespace Flakkari::Network
