/**************************************************************************
 * Flakkari Library v0.2.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Packet.hpp
 * @brief Flakkari::Protocol::Packet class header. This class is used to
 * represent a packet. A packet is a header and a payload.
 *  - The header is a Flakkari::Protocol::Header object.
 *  - The payload is a Flakkari::Network::Buffer object.
 *
 * @see Flakkari::Protocol::Header
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
#include "Events.hpp"

namespace Flakkari::Protocol {

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

        std::string to_string()
        {
            std::string str = "Packet<Id: "
            + Commands::command_to_string(header._commandId)
            + ", ContentLength: "
            + std::to_string(int(header._contentLength))
            + ", SequenceNumber: "
            + std::to_string(int(header._sequenceNumber))
            + ", Payload: "
            + std::string((const char*)payload.data(), payload.size())
            + ">";
            return str;
        }

        friend std::ostream& operator<<(std::ostream& os, const Packet& packet)
        {
            os << "Packet<Id: "
            << htons(packet.header._commandId)
            << ", ContentLength: "
            << htons(packet.header._contentLength)
            << ", SequenceNumber: "
            << htonl(packet.header._sequenceNumber)
            << ", Payload: "
            << packet.payload
            << ">";
            return os;
        }

        template<typename T>
        void injectComponent(T component)
        {
            int intValue = (int)component.size();
            const byte* dataBytes = reinterpret_cast<const byte*>(&intValue);
            payload.insert(payload.end(), dataBytes, dataBytes + sizeof(intValue));
            payload += component;
            header._contentLength += payload.size() + sizeof(intValue);
        }

        /**
         * @brief Add data to the packet.
         *
         * @tparam DataType  Type of the data to add.
         * @param packet  The packet to add the data to.
         * @param data  The data to add.
         * @return Packet<Id>&  The packet with the data added.
         */
        template<typename DataType>
        friend Packet<Id>& operator<<(Packet<Id>& packet, const DataType& data)
        {
            static_assert(std::is_trivially_copyable<DataType>::value, "DataType must be trivially copyable to be used in a packet.");
            static_assert(std::is_standard_layout<DataType>::value, "DataType must be standard layout to be used in a packet.");

            const byte* dataBytes = reinterpret_cast<const byte*>(&data);
            packet.payload.insert(packet.payload.end(), dataBytes, dataBytes + sizeof(data));
            packet.header._contentLength += packet.payload.size() + sizeof(data);
            return packet;
        }

        /**
         * @brief Extract data from the packet.
         *
         * @tparam DataType  Type of the data to extract.
         * @param packet  The packet to extract the data from.
         * @param data  The data to extract.
         * @return Packet<Id>&  The packet with the data extracted.
         *
         * @deprecated  This function is deprecated. Don't work with std::string.
         *              Use the other operator>> instead.
         */
        template<typename DataType>
        friend Packet<Id>& operator>>(Packet<Id>& packet, DataType& data)
        {
            static_assert(std::is_trivially_copyable<DataType>::value, "DataType must be trivially copyable to be used in a packet.");
            static_assert(std::is_standard_layout<DataType>::value, "DataType must be standard layout to be used in a packet.");

            if (packet.payload.size() < sizeof(data))
                throw std::runtime_error("Packet payload is too small to extract data.");

            std::memcpy(&data, packet.payload.data(), sizeof(data));
            packet.payload.erase(packet.payload.begin(), packet.payload.begin() + sizeof(data));
            packet.header._contentLength -= sizeof(data);
            return packet;
        }

        void injectString(std::string str)
        {
            int intValue = (int)str.size();
            const byte* dataBytes = reinterpret_cast<const byte*>(&intValue);
            payload.insert(payload.end(), dataBytes, dataBytes + sizeof(intValue));
            payload += str;
            header._contentLength += payload.size() + sizeof(intValue);
        }

        std::string extractString()
        {
            std::string str;
            int intValue;
            std::memcpy(&intValue, payload.data(), sizeof(intValue));
            payload.erase(payload.begin(), payload.begin() + sizeof(intValue));
            str = std::string((const char*)payload.data(), intValue);
            payload.erase(payload.begin(), payload.begin() + intValue);
            header._contentLength -= sizeof(intValue) + intValue;
            return str;
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
        [[nodiscard]] bool deserialize(const Network::Buffer &buffer)
        {
            if (buffer.size() < sizeof(header))
                return false;
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

} /* namespace V_0 */

} // namespace Flakkari::Protocol

#endif /* !PACKET_HPP_ */
