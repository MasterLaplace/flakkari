/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-14
** File description:
** Health
*/

#ifndef Health_HPP_
#define Health_HPP_

#include <cstddef>

#include "Network/Packed.hpp"

namespace Flakkari::Engine::ECS::Components::Common {
PACKED_START

/**
 * @brief Health is a structure that represents the life of an "living object"
 *
 */
struct Health {
    unsigned int currentHealth;
    unsigned int maxHealth = 100;
    unsigned int shield = 0;
    unsigned int maxShield = 100;

    Health() : currentHealth(100), maxHealth(100), shield(0), maxShield(100) {};
    Health(unsigned int currentHealth, unsigned int maxHealth, unsigned int shield, unsigned int maxShield) :
        currentHealth(currentHealth), maxHealth(maxHealth), shield(shield), maxShield(maxShield) {};
    Health(const Health &other) : currentHealth(other.currentHealth), maxHealth(other.maxHealth), shield(other.shield), maxShield(other.maxShield) {};

    unsigned int size() const {
        return sizeof(*this);
    }
};

PACKED_END
} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !Health_HPP_ */
