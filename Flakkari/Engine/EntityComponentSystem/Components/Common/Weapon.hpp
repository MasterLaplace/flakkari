/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-14
** File description:
** Weapon
*/

#ifndef WEAPON_HPP_
#define WEAPON_HPP_

#include <cstring>
#include <string>

#include "Network/Packed.hpp"

namespace Flakkari::Engine::ECS::Components::Common {
PACKED_START

/**
 * @brief Weapon is a structure that defines the characteristics of a weapon.
 *
 * @details
 * The Weapon structure is used to define the characteristics of a weapon.
 * It is used by the WeaponComponent to handle the firing and reloading logic.
 *
 * @param
 * damage: The amount of damage dealt by the weapon.
 * fireRate: The rate of fire, shots per second.
 * currentAmmo: Current ammunition in the magazine.
 * reloadTime: Time it takes to reload the weapon in seconds.
 * timeSinceLastShot: Time elapsed since the last shot was fired.
 * isReloading: Is the weapon currently reloading.
 * maxAmmo: Maximum ammunition the weapon can hold.
 */
struct Weapon {
    std::size_t damage;
    float fireRate;
    std::size_t level;

    Weapon() = default;
    Weapon(const Weapon &other) = default;
    Weapon(std::size_t dmg, float rate, std::size_t lvl) : damage(dmg), fireRate(rate), level(lvl){};

    std::size_t size() const { return sizeof(*this); };
};

PACKED_END
} // namespace Flakkari::Engine::ECS::Components::Common

#endif /* !WEAPON_HPP_ */
