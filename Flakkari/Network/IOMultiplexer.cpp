/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-23
** File description:
** IOMultiplexer
*/

#include "IOMultiplexer.hpp"

namespace Flakkari::Network {

#if defined(_PPOLL_)

PPOLL::PPOLL(int fileDescriptor, event_t events)
{
    if (fileDescriptor == -1) {
        FLAKKARI_LOG_ERROR("Socket is -1");
        throw std::runtime_error("Socket is -1");
    }

    if (_pollfds.size() < std::size_t(fileDescriptor))
        _pollfds.resize(fileDescriptor + 1);
    _pollfds[fileDescriptor] = pollfd{fileDescriptor, events, 0};

    _timeout.tv_sec = 1;
    _timeout.tv_nsec = 0;// 100000000;  // 100ms
}

PPOLL::PPOLL() {
    _timeout.tv_sec = 1;
    _timeout.tv_nsec = 0;// 100000000;  // 100ms
}

void PPOLL::addSocket(FileDescriptor socket) {
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (_pollfds.size() < std::size_t(socket))
        _pollfds.resize(socket + 1);
    _pollfds[socket] = pollfd{socket, POLLIN | POLLPRI, 0};
}

void PPOLL::addSocket(FileDescriptor socket, event_t events) {
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (_pollfds.size() < std::size_t(socket))
        _pollfds.resize(socket + 1);
    _pollfds[socket] = pollfd{socket, events, 0};
}

void PPOLL::removeSocket(FileDescriptor socket) {
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (_pollfds.size() < std::size_t(socket))
        throw std::runtime_error("Index out of range");
    if (_pollfds.size() == std::size_t(socket))
        _pollfds.pop_back();
    else
        _pollfds[socket] = pollfd{-1, 0, 0};
}

int PPOLL::wait() {
    return ppoll(_pollfds.data(), _pollfds.size(), &_timeout, nullptr);
}

pollfd &PPOLL::operator[](std::size_t index) {
    if (_pollfds.size() < index)
        throw std::runtime_error("Index out of range");
    return _pollfds[index];
}

pollfd &PPOLL::operator[](FileDescriptor socket) {
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (_pollfds.size() < std::size_t(socket))
        throw std::runtime_error("Index out of range");
    return _pollfds[socket];
}

bool PPOLL::isReady(FileDescriptor socket) {
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (_pollfds.size() < std::size_t(socket))
        throw std::runtime_error("Index out of range");
    if (_pollfds[socket].revents & (POLLIN | POLLPRI))
        return true;
    return false;
}

#elif defined(_EPOLL_)

#endif

} // namespace Flakkari::Network
