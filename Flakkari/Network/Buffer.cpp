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

Buffer::Buffer(std::string data) {
    std::copy(data.begin(), data.end(), std::back_inserter(*this));
}

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

Buffer Buffer::extractData(std::size_t offset, std::size_t length) const {
    return Buffer(begin() + offset, begin() + offset + length);
}

void Buffer::concat(const Buffer& otherBuffer) {
    insert(end(), otherBuffer.begin(), otherBuffer.end());
}

uint16_t Buffer::calculateChecksum() const {
    uint16_t checksum = 0;
    for (const auto &byte : *this) {
        checksum += byte;
    }
    return checksum;
}

void Buffer::bitwiseXOR(const Buffer& otherBuffer) {
    std::size_t minSize = std::min(size(), otherBuffer.size());
    for (std::size_t i = 0; i < minSize; ++i) {
        (*this)[i] ^= otherBuffer[i];
    }
}

void Buffer::runLengthEncode() {
    Buffer encodedBuffer;
    byte currentByte = (*this)[0];
    byte currentCount = 1;
    for (std::size_t i = 1; i < size(); ++i) {
        if ((*this)[i] == currentByte) {
            ++currentCount;
        } else {
            encodedBuffer.push_back(currentCount);
            encodedBuffer.push_back(currentByte);
            currentByte = (*this)[i];
            currentCount = 1;
        }
    }
    encodedBuffer.push_back(currentCount);
    encodedBuffer.push_back(currentByte);
    std::copy(encodedBuffer.begin(), encodedBuffer.end(), begin());
}

void Buffer::runLengthDecode() {
    Buffer decodedBuffer;
    for (std::size_t i = 0; i < size(); i += 2) {
        byte currentCount = (*this)[i];
        byte currentByte = (*this)[i + 1];
        for (std::size_t j = 0; j < currentCount; ++j) {
            decodedBuffer.push_back(currentByte);
        }
    }
    std::copy(decodedBuffer.begin(), decodedBuffer.end(), begin());
}

void Buffer::convertToHex() {
    Buffer hexBuffer;
    for (const auto &byte : *this) {
        hexBuffer.push_back(byte / 16);
        hexBuffer.push_back(byte % 16);
    }
    std::copy(hexBuffer.begin(), hexBuffer.end(), begin());
}

void Buffer::convertFromHex() {
    Buffer hexBuffer;
    for (std::size_t i = 0; i < size(); i += 2) {
        byte currentByte = (*this)[i] * 16 + (*this)[i + 1];
        hexBuffer.push_back(currentByte);
    }
    std::copy(hexBuffer.begin(), hexBuffer.end(), begin());
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

std::istream &operator>>(std::istream &is, Buffer &buffer) {
    std::string data;
    is >> data;
    buffer = Buffer(data);
    return is;
}

} // namespace Flakkari::Network
