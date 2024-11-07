/**************************************************************************
 * Flakkari Library v0.2.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Components.hpp
 * @brief This file contains the Components enum class.
 *        It is used to identify the components in the Flakkari Protocol.
 *
 * @see inspired by the https://en.wikipedia.org/wiki/IPv4 header
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.2.0
 * @date 2024-01-12
 **************************************************************************/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include <cstdint>

namespace Flakkari::Protocol {

inline namespace V_0 {

enum class ComponentId : uint8_t {
    // 0 - 9: 2D components
    CONTROL = 0,
    MOVABLE = 1,
    TRANSFORM = 2,
    COLLIDER = 3,
    RIGIDBODY = 4,
    // 10 - 19: Common components
    CHILD = 10,
    PARENT = 11,
    TAG = 12,
    SPAWNED = 13,
    TEMPLATE = 14,
    WEAPON = 15,
    LEVEL = 16,
    EVOLVE = 17,
    HEALTH = 18,
    // 20 - 29: Network components
    NETWORK_EVENT = 20,
    NETWORK_IP = 21,
    MAX_COMPONENT
};

static_assert(static_cast<uint8_t>(ComponentId::MAX_COMPONENT) <= 30, "ComponentId::MAX_COMPONENT is too big");

class Components final {
    public:
    static std::string component_to_string(ComponentId id)
    {
        switch (id)
        {
        case ComponentId::CONTROL: return "CONTROL";
        case ComponentId::MOVABLE: return "MOVABLE";
        case ComponentId::TRANSFORM: return "TRANSFORM";
        case ComponentId::COLLIDER: return "COLLIDER";
        case ComponentId::RIGIDBODY: return "RIGIDBODY";
        case ComponentId::CHILD: return "CHILD";
        case ComponentId::PARENT: return "PARENT";
        case ComponentId::TAG: return "TAG";
        case ComponentId::SPAWNED: return "SPAWNED";
        case ComponentId::TEMPLATE: return "TEMPLATE";
        case ComponentId::WEAPON: return "WEAPON";
        case ComponentId::LEVEL: return "LEVEL";
        case ComponentId::EVOLVE: return "EVOLVE";
        case ComponentId::HEALTH: return "HEALTH";
        case ComponentId::NETWORK_EVENT: return "NETWORK_EVENT";
        case ComponentId::NETWORK_IP: return "NETWORK_IP";
        default: return "UNKNOWN";
        }
    }
};

} /* namespace V_0 */

} // namespace Flakkari::Protocol

#endif /* !COMPONENTS_HPP_ */
