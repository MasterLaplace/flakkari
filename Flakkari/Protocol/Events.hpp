/**************************************************************************
 * Flakkari Library v0.4.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Events.hpp
 * @brief This file contains the EventId enum class. It is used to identify
 *        the events in the Flakkari Protocol. It is used to send an event
 *        from the client to the server.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.4.0
 * @date 2024-01-14
 **************************************************************************/

#ifndef EVENTS_HPP_
#define EVENTS_HPP_

#include "config.h.in"

namespace Flakkari::Protocol {

namespace V_0 {

enum class EventId : uint8_t {
    MOVE_LEFT = 10,
    MOVE_RIGHT = 11,
    MOVE_UP = 12,
    MOVE_DOWN = 13,
    SHOOT = 14,
    MAX_EVENT
};

enum class EventState : uint8_t {
    NONE = 0,
    PRESSED = 1,
    RELEASED = 2,
    MAX_STATE
};

LPL_PACKED_START

struct Event {
    V_0::EventId id;
    V_0::EventState state;
};

LPL_PACKED_END

} /* namespace V_0 */

inline namespace V_1 {

enum class EventId : uint8_t {
    MOVE_LEFT = 10,
    MOVE_RIGHT = 11,
    MOVE_UP = 12,
    MOVE_DOWN = 13,
    MOVE_FRONT = 14,
    MOVE_BACK = 15,
    LOOK_LEFT = 16,
    LOOK_RIGHT = 17,
    LOOK_UP = 18,
    LOOK_DOWN = 19,
    SHOOT = 20,
    MAX_EVENT
};

enum class EventState : uint8_t {
    NONE = 0,
    PRESSED = 1,
    RELEASED = 2,
    MAX_STATE
};

LPL_PACKED_START

struct Event {
    V_1::EventId id;
    V_1::EventState state;
};

LPL_PACKED_END

} /* namespace V_1 */

} // namespace Flakkari::Protocol

#endif /* !EVENTS_HPP_ */
