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

    } /* namespace V_1 */

} // namespace Flakkari::Protocol::API
