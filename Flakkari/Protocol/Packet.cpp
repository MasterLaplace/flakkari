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
            content = data.extractData(sizeof(header), header._contentLength);
        }

        Packet::Packet(Header header, Network::Buffer content)
            : header(header), content(content) {}

        Packet::Packet(Header header) : header(header) {}

        template<typename T>
        void Packet::addContent(T content) {
            this->content += Network::Buffer(content);
        }

        void Packet::addFragment(Network::Buffer fragment) {
            this->content += fragment;
            this->header._contentLength += fragment.size();
        }

        template<typename T>
        void Packet::deleteContent(T content) {
            this->content -= Network::Buffer(content);
        }

        void Packet::deleteFragment(Network::Buffer fragment) {
            this->content -= fragment;
            this->header._contentLength -= fragment.size();
        }

        void serializeHeader(Header header, Network::Buffer& buffer) {
            std::copy((byte *) &header, (byte *) &header + sizeof(Header), buffer.begin());
        }

        void serializeBuffer(Network::Buffer buffer, Network::Buffer& buffer2) {
            std::copy(buffer.begin(), buffer.end(), buffer2.begin());
        }

        void serializePacket(Packet packet, Network::Buffer& buffer) {
            serializeHeader(packet.header, buffer);
            serializeBuffer(packet.content, buffer);
        }

        void deserializeHeader(Network::Buffer buffer, Header& header) {
            std::copy(buffer.begin(), buffer.begin() + sizeof(Header), (byte *) &header);
        }

        void deserializeBuffer(Network::Buffer buffer, Network::Buffer& buffer2) {
            std::copy(buffer.begin(), buffer.end(), buffer2.begin());
        }

        void deserializePacket(Network::Buffer buffer, Packet& packet) {
            deserializeHeader(buffer, packet.header);
            deserializeBuffer(buffer, packet.content);
        }

    } /* namespace V_1 */

} // namespace Flakkari::Protocol::API
