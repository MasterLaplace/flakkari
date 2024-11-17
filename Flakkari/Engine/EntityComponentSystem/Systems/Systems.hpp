/**************************************************************************
 * Flakkari Engine Library v0.4.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Systems.hpp
 * @brief Systems header. Contains all systems.
 *        (position, rigid_body)
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.4.0
 * @date 2024-01-06
 **************************************************************************/

#ifndef FLAKKARI_SYSTEMS_HPP_
#define FLAKKARI_SYSTEMS_HPP_

#include "../Components/Components2D.hpp"
#include "../Components/Components3D.hpp"
#include "../Components/ComponentsCommon.hpp"
#include "../Registry.hpp"

#include <chrono>
#include <cmath>

namespace Flakkari::Engine::ECS::Systems::_2D {

/**
 * @brief Updates the position of all entities with a Position and a Movable component based on their velocity.
 *
 * @note The velocity vector is normalized following this advice: https://youtube.com/shorts/0cYjreg7dpg
 *
 * @param r  The registry containing the entities to update.
 * @param deltaTime  The time elapsed since the last update.
 */
void position(Registry &r, float deltaTime);

} // namespace Flakkari::Engine::ECS::Systems::_2D

namespace Flakkari::Engine::ECS::Systems::_3D {

/**
 * @brief Updates the position of all entities with a Transform and a Movable component based on their velocity.
 *
 * @param r  The registry containing the entities to update.
 * @param deltaTime  The time elapsed since the last update.
 */
void apply_movable(Registry &r, float deltaTime);

/**
 * @brief Spawns a random entity within a skybox.
 *
 * @param r  The registry containing the entities to update.
 */
void spawn_random_within_skybox(Registry &r);

} // namespace Flakkari::Engine::ECS::Systems::_3D

#endif /* !FLAKKARI_SYSTEMS_HPP_ */
