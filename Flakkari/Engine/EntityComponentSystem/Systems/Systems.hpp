/**************************************************************************
 * Flakkari Engine Library v0.2.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Systems.hpp
 * @brief Systems header. Contains all systems.
 *        (position, rigid_body)
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.2.0
 * @date 2023-01-06
 **************************************************************************/

#ifndef FLAKKARI_SYSTEMS_HPP_
#define FLAKKARI_SYSTEMS_HPP_

#include "../Registry.hpp"
#include "../Components/Components2D.hpp"
#include "../Components/ComponentsCommon.hpp"

#include <cmath>
#include <chrono>

namespace Flakkari::Engine::ECS::Systems {

/**
 * @brief Updates the position of all entities with a Position and a Movable component based on their velocity.
 *
 * @note The velocity vector is normalized following this advice: https://youtube.com/shorts/0cYjreg7dpg
 *
 * @param r  The registry containing the entities to update.
 * @param deltaTime  The time elapsed since the last update.
 */
void position(Registry &r, float deltaTime);

} // namespace Flakkari::Engine::ECS::Systems

#endif /* !FLAKKARI_SYSTEMS_HPP_ */
