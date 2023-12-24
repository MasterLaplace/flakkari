/**************************************************************************
 * Flakkari Library v0.1.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Packet.hpp
 * @brief Flakkari::Protocol::API::Packet class header. This class is used to
 * represent a packet. A packet is a header and a content.
 *  - The header is a Flakkari::Protocol::API::Header object.
 *  - The content is a Flakkari::Network::Buffer object.
 *
 * @see Flakkari::Protocol::API::Header
 * @see Flakkari::Network::Buffer
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.1.0
 * @date 2023-12-24
 **************************************************************************/

#ifndef PACKET_HPP_
#define PACKET_HPP_

#include "Header.hpp"

namespace Flakkari::Protocol::API {

    inline namespace V_1 {

        /**
         * @brief Flakkari Packet v1 (new packet)
         *
         * @param header: The header of the packet.
         * @param content: The content of the packet.
         */
        struct Packet {
            Header header;
            Network::Buffer content;

            Packet(Network::Buffer data);
            Packet(Header header, Network::Buffer content);
            Packet(Header header);
            Packet() = default;

            /**
             * @brief Add content to the packet.
             *
             * @tparam T  The type of the content to add.
             * @param content  The content to add.
             */
            template<typename T>
            void addContent(T content);

            /**
             * @brief Add a fragment to the packet.
             *
             * @param fragment  The fragment to add.
             */
            void addFragment(Network::Buffer fragment);

            /**
             * @brief Delete content from the packet.
             *
             * @tparam T  The type of the content to delete.
             * @param content  The content to delete.
             */
            template<typename T>
            void deleteContent(T content);

            /**
             * @brief Delete a fragment from the packet.
             *
             * @param fragment  The fragment to delete.
             */
            void deleteFragment(Network::Buffer fragment);
        };

        void serializeHeader(Header header, Network::Buffer& buffer);

        void serializeBuffer(Network::Buffer buffer, Network::Buffer& buffer2);

        void serializePacket(Packet packet, Network::Buffer& buffer);

        void deserializeHeader(Network::Buffer buffer, Header& header);

        void deserializeBuffer(Network::Buffer buffer, Network::Buffer& buffer2);

        void deserializePacket(Network::Buffer buffer, Packet& packet);

    } /* namespace V_1 */

} // namespace Flakkari::Protocol::API

#endif /* !PACKET_HPP_ */
