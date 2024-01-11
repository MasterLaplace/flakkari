/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-24
** File description:
** Packet
*/

#include "Packet.hpp"

namespace Flakkari::Protocol::API {

    inline namespace V_1 {

        Packet::Packet(Network::Buffer data) {
            header = Header(data);
            payload = data.extractData(sizeof(header), header._contentLength);
        }

        Packet::Packet(Header header, Network::Buffer payload)
            : header(header), payload(payload) {}

        Packet::Packet(Header header) : header(header) {}

        template<typename T>
        void Packet::addContent(T payload) {
            this->payload += Network::Buffer(payload);
            this->header._contentLength += sizeof(T);
        }

        void Packet::addFragment(Network::Buffer fragment) {
            this->payload += fragment;
            this->header._contentLength += fragment.size();
        }

        template<typename T>
        void Packet::deleteContent(T payload) {
            this->payload -= Network::Buffer(payload);
            this->header._contentLength -= sizeof(T);
        }

        void Packet::deleteFragment(Network::Buffer fragment) {
            this->payload -= fragment;
            this->header._contentLength -= fragment.size();
        }

        void serializeHeader(Header header, Network::Buffer& buffer) {
            std::copy((byte *) &header, (byte *) &header + sizeof(Header), buffer.begin());
        }

        void serializeBuffer(Network::Buffer buffer, Network::Buffer& dest) {
            std::copy(buffer.begin(), buffer.end(), dest.begin());
        }

        void serializePacket(Packet packet, Network::Buffer& buffer) {
            serializeHeader(packet.header, buffer);
            serializeBuffer(packet.payload, buffer);
        }

        void deserializeHeader(Network::Buffer buffer, Header& header) {
            std::copy(buffer.begin(), buffer.begin() + sizeof(Header), (byte *) &header);
        }

        void deserializeBuffer(Network::Buffer buffer, Network::Buffer& dest) {
            std::copy(buffer.begin(), buffer.end(), dest.begin());
        }

        void deserializePacket(Network::Buffer buffer, Packet& packet) {
            deserializeHeader(buffer, packet.header);
            deserializeBuffer(buffer, packet.payload);
        }

    } /* namespace V_1 */

} // namespace Flakkari::Protocol::API
