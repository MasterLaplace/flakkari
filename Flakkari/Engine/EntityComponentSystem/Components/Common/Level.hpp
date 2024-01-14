/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-14
** File description:
** Level
*/

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <cstring>

#include "Network/Packed.hpp"

namespace Flakkari::Engine::ECS::Components::Common {
PACKED_START

/**
 * @brief FontInfo is a structure that holds information about a Font
 *
 */
struct Level {
    unsigned int level;
    const char *currentWeapon;
    unsigned int currentExp;
    unsigned int requiredExp;

    Level() : level(1), currentWeapon(""), currentExp(0), requiredExp(100) {}
    Level(unsigned int level, std::string currentWeapon, unsigned int currentExp, unsigned int requiredExp)
        : level(level),
        currentWeapon(currentWeapon.c_str()),
        currentExp(currentExp),
        requiredExp(requiredExp) {}
    Level(const Level &other)
        : level(other.level),
        currentWeapon(other.currentWeapon),
        currentExp(other.currentExp),
        requiredExp(other.requiredExp) {}

    std::size_t size() const {
        return sizeof(level) + std::strlen(currentWeapon) + sizeof(currentExp) + sizeof(requiredExp);
    }
};

PACKED_END
} // namespace Flakkari::Engine::ECS::Components::Common

#endif //LEVEL_HPP
