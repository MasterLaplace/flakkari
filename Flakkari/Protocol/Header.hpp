/**************************************************************************
 * Flakkari Library v0.2.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Header.hpp
 * @brief This file contains the Header struct. It is used to create a header
 *        for the Flakkari Protocol.
 *
 * @see inspired by the https://en.wikipedia.org/wiki/IPv4 header
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.2.0
 * @date 2023-12-24
 **************************************************************************/


#ifndef HEADER_HPP_
    #define HEADER_HPP_

#define PROTOCOL_VERSION 1

#include "../Network/Buffer.hpp"
#include "../Network/Packed.hpp"
#include "Commands.hpp"
#include <chrono>

namespace Flakkari::Protocol {

using byte = byte_t;            // 8 bits  (max: 255)
using ushort = unsigned short;  // 16 bits (max: 65535)
using uint = unsigned int;      // 32 bits (max: 4294967295)
using ulong = unsigned long;    // 64 bits (max: 18446744073709551615)

/**
 * @brief The version of the protocol used
 *
 */
enum class ApiVersion : byte {
    V_0,
    MAX_VERSION
};

inline namespace V_0 {

    /**
     * @brief The priority of the message in the queue
     *
     * @note The priority is used to determine the order of the messages in the queue.
     *      The higher the priority, the faster the message will be processed.
     */
    enum class Priority : byte {
        LOW = 0,
        MEDIUM = 1,
        HIGH = 2,
        CRITICAL = 3,
        MAX_PRIORITY
    };

    PACKED_START

    template<typename Id>
    struct Header {
        Priority _priority : 4 = Priority::LOW;
        ApiVersion _apiVersion : 4 = ApiVersion::V_0;
        Id _commandId;
        ushort _contentLength = 0;
        uint _sequenceNumber = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    };

    PACKED_END

} /* namespace V_1 */

} // namespace Flakkari::Protocol

#endif /* !HEADER_HPP_ */
