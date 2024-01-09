/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-24
** File description:
** Header
*/

#include "Header.hpp"

namespace Flakkari::Protocol::API {

inline namespace V_1 {

    Header::Header(Network::Buffer data) {
        *this = *(Header *)data.data();
    }

    Header::Header(
        Priority priority, ApiVersion apiVersion,
        byte commandId, ushort contentLength//,
        // ulong sequenceNumber, ushort checksum
    ) {
        _priority = (byte)priority;
        _apiVersion = (byte)apiVersion;
        _commandId = commandId;
        _contentLength = contentLength;
        // _sequenceNumber = sequenceNumber;
        // _checksum = checksum;
    }

    Header::Header() {
        _priority = 0;
        _apiVersion = (byte)ApiVersion::V_1;
    }

    Network::Buffer Header::toBuffer()
    {
        Network::Buffer buffer(sizeof(Header));
        std::copy((byte *)this, (byte *)this + sizeof(Header), buffer.begin());
        return buffer;
    }

    void Header::print()
    {
        std::cout << "Header: " << std::endl;
        std::cout << "  Priority: " << (int)_priority << std::endl;
        std::cout << "  ApiVersion: " << (int)_apiVersion << std::endl;
        std::cout << "  CommandId: " << (int)_commandId << std::endl;
        std::cout << "  ContentLength: " << (int)_contentLength << std::endl;
        // std::cout << "  SequenceNumber: " << (int)_sequenceNumber << std::endl;
        // std::cout << "  Checksum: " << (int)_checksum << std::endl;
    }

    std::ostream &operator<<(std::ostream &os, const Header &header)
    {
        os << "Header: " << std::endl;
        os << "  Priority: " << (int)header._priority << std::endl;
        os << "  ApiVersion: " << (int)header._apiVersion << std::endl;
        os << "  CommandId: " << (int)header._commandId << std::endl;
        os << "  ContentLength: " << (int)header._contentLength << std::endl;
        // os << "  SequenceNumber: " << (int)header._sequenceNumber << std::endl;
        // os << "  Checksum: " << (int)header._checksum << std::endl;
        return os;
    }

} /* namespace V_1 */

} // namespace Flakkari::Protocol::API
