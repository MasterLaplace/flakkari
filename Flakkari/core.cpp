/*
** EPITECH PROJECT, 2023
** B-CPP-500-REN-5-2-rtype-mathys.thevenot
** File description:
** core
*/

#include <iostream>
#include "FlakkariMessage.hpp"

using namespace std;

int main() {
    FLAKKARI_LOG_INFO("Server: Hello, World!");
    FLAKKARI_LOG_LOG("Server: Hello, World!");
    FLAKKARI_LOG_DEBUG("Server: Hello, World!");
    FLAKKARI_LOG_WARNING("Server: Hello, World!");
    FLAKKARI_LOG_ERROR("Server: Hello, World!");
    FLAKKARI_LOG_FATAL("Server: Hello, World!");
    return 0;
}
