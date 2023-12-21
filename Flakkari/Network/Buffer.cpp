/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-21
** File description:
** Buffer
*/

#include "Buffer.hpp"

namespace Flakkari::Network {

std::size_t Buffer::getSize() const {
    return size();
}

std::size_t Buffer::getCapacity() const {
    return capacity();
}

std::size_t Buffer::getRemainingSpace() const {
    return capacity() - size();
}

byte *Buffer::getData() {
    return data();
}

const byte *Buffer::getData() const {
    return data();
}

Buffer::operator byte *() {
    return data();
}

Buffer::operator const byte *() const {
    return data();
}

Buffer::operator std::string() {
    return std::string((char *)data(), size());
}

Buffer::operator const std::string() const {
    return std::string((char *)data(), size());
}

std::ostream &operator<<(std::ostream &os, const Buffer &buffer) {
    os << buffer.getData();
    return os;
}

} // namespace Flakkari::Network
