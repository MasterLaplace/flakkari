/**************************************************************************
 * Flakkari Library v0.2.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Packet.hpp
 * @brief Flakkari::Protocol::API::Packet class header. This class is used to
 * represent a packet. A packet is a header and a payload.
 *  - The header is a Flakkari::Protocol::API::Header object.
 *  - The payload is a Flakkari::Network::Buffer object.
 *
 * @see Flakkari::Protocol::API::Header
 * @see Flakkari::Network::Buffer
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.2.0
 * @date 2023-12-24
 **************************************************************************/


#ifndef PACKET_HPP_
    #define PACKET_HPP_

#include "Header.hpp"
#include "Components.hpp"

namespace Flakkari::Protocol::API {

inline namespace V_0 {

    /**
     * @brief Flakkari Packet v0 (new packet)
     *
     * @tparam Id: The type of the command id.
     * @param header: The header of the packet.
     * @param payload: The payload of the packet.
     */
    template<typename Id>
    struct Packet {
        Header<Id> header;
        Network::Buffer payload;

        std::size_t size() const {
            return sizeof(header) + payload.size();
        }

        friend std::ostream& operator<<(std::ostream& os, const Packet& packet)
        {
            os << "Packet<Id: "
            << packet.header._commandId
            << ", ContentLength: "
            << packet.header._contentLength
            << ", SequenceNumber: "
            << packet.header._sequenceNumber
            << ", Payload: "
            << packet.payload
            << ">";
            return os;
        }

        template<typename DataType>
        friend Packet<Id>& operator<<(Packet<Id>& packet, const DataType& data)
        {
            static_assert(std::is_trivially_copyable<DataType>::value, "DataType must be trivially copyable to be used in a packet.");
            static_assert(std::is_standard_layout<DataType>::value, "DataType must be standard layout to be used in a packet.");

            std::size_t size = packet.payload.size();
            packet.payload.resize(size + sizeof(DataType));
            std::memcpy(packet.payload.data() + size, &data, sizeof(DataType));
            packet.header._contentLength = packet.payload.size();
            return packet;
        }

        template<typename DataType>
        friend Packet<Id>& operator>>(Packet<Id>& packet, DataType& data)
        {
            static_assert(std::is_trivially_copyable<DataType>::value, "DataType must be trivially copyable to be used in a packet.");
            static_assert(std::is_standard_layout<DataType>::value, "DataType must be standard layout to be used in a packet.");

            std::size_t size = packet.payload.size() - sizeof(DataType);
            std::memcpy(&data, packet.payload.data() + size, sizeof(DataType));
            packet.payload.resize(size);
            packet.header._contentLength = packet.payload.size();
            return packet;
        }

        /**
         * @brief Serialize the packet into a buffer to be sent over the network.
         *
         * @return Network::Buffer  The buffer containing the serialized packet.
         */
        Network::Buffer serialize() const
        {
            Network::Buffer buffer(size());
            std::memcpy(buffer.data(), &header, sizeof(header));
            std::memcpy(buffer.data() + sizeof(header), payload.data(), payload.size());
            return buffer;
        }

        /**
         * @brief Deserialize the buffer into a packet.
         *
         * @param buffer  The buffer containing the serialized packet.
         * @return true  The packet has been deserialized successfully.
         * @return false  The packet has not been deserialized successfully.
         */
        [[nodiscard]] bool deserialize(Network::Buffer buffer)
        {
            std::memcpy(&header, buffer.data(), sizeof(header));
            if (header._priority >= Priority::MAX_PRIORITY)
                return false;
            if (header._apiVersion >= ApiVersion::MAX_VERSION)
                return false;
            if (header._contentLength > buffer.size() - sizeof(header))
                return false;
            payload = buffer.extractData(sizeof(header), header._contentLength);
            return true;
        }
    };

} /* namespace V_1 */

} // namespace Flakkari::Protocol::API

#endif /* !PACKET_HPP_ */
