/**************************************************************************
 * Flakkari Library v0.1.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Header.hpp
 * @brief This file contains the Header struct. It is used to create a header for the Flakkari Protocol.
 *
 * @see inspired by the https://en.wikipedia.org/wiki/IPv4 header
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.1.0
 * @date 2023-12-24
 **************************************************************************/

#ifndef HEADER_HPP_
#define HEADER_HPP_

#include "Network/Buffer.hpp"

namespace Flakkari::Protocol::API {

    using byte = unsigned char;     // 8 bits  (max: 255)
    using ushort = unsigned short;  // 16 bits (max: 65535)
    using uint = unsigned int;      // 32 bits (max: 4294967295)
    using ulong = unsigned long;    // 64 bits (max: 18446744073709551615)

    inline namespace V_1 {

        /**
         * @brief The priority of the message in the queue
         *
         * @note The priority is used to determine the order of the messages in the queue.
         *      The higher the priority, the faster the message will be processed.
         */
        enum class Priority {
            LOW = 0,
            MEDIUM = 1,
            HIGH = 2,
            CRITICAL = 3
        };

        enum class ApiVersion {
            V_1 = 1
        };

        /** @brief Flakkari Header v1 (new header)
         *
         *  0                   1                   2                   3
         *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
         * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         * | PRIO  | API v.|  Command ID   |         Content Length        |
         * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         * |                        Sequence Number                        |
         * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         * |   Checksum    |
         * +-+-+-+-+-+-+-+-+
         *
         * @param migration: 4 bits (max 15)    - (not used)
         * @param priority: 4 bits (max 15)     - to determine the priority of the message in the queue
         * @param apiVersion: 4 bits (max 15)   - to determine the version of the protocol (v1)
         * @param commandId: 8 bits (max 255)   - to determine the command to execute
         * @param contentLength: 16 bits        - to determine the length of the content
         * @param sequenceNumber: 64 bits       - to determine the sequence of the message
         * @param checksum: 16 bits             - to determine the checksum of the message
         *
         * @example "Header for a message with a commandId of 1 and a contentLength of 0"
         * @code
         * Flakkari::Protocol::API::Header header(
         *      Flakkari::Protocol::API::Priority::LOW,
         *      Flakkari::Protocol::API::ApiVersion::V_1,
         *      1, 0, 0, 0
         * );
         * header.print();
         * @endcode
         */
        struct Header {
            byte _priority: 4;
            byte _apiVersion: 4;
            byte _commandId;
            ushort _contentLength;
            ulong _sequenceNumber;
            ushort _checksum;

            /**
             * @brief Construct a new Header object from a buffer
             * @see Network::Buffer
             *
             * @param data  the buffer containing the header
             */
            Header(Network::Buffer data);

            Header(
                Priority priority, ApiVersion apiVersion,
                byte commandId, ushort contentLength = 0,
                ulong sequenceNumber = 0, ushort checksum = 0
            );

            Header();

            /**
             * @brief Convert the header to a buffer to send it through the network
             * @see Network::Buffer
             *
             * @return Network::Buffer the buffer containing the header
             */
            [[nodiscard]] Network::Buffer toBuffer();

            /**
             * @brief Print the header in the console (for debug)
             *
             */
            void print();
        };

        /**
         * @brief Overload of the << operator to print the header in the console (for debug)
         *
         * @param os  the output stream
         * @param header  the header to print
         * @return std::ostream&  the output stream
         */
        std::ostream &operator<<(std::ostream &os, const Header &header);

    } /* namespace V_1 */

} // namespace Flakkari::Protocol::API

#endif /* !HEADER_HPP_ */
