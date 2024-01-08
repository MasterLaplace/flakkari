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

#if defined(_PSELECT_)

PSELECT::PSELECT(int fileDescriptor)
{
    if (fileDescriptor == -1) {
        FLAKKARI_LOG_ERROR("Socket is -1");
        throw std::runtime_error("Socket is -1");
    }

    FD_ZERO(&_fds);
    _maxFd = fileDescriptor;
    _sockets.push_back(fileDescriptor);

    _timeout.tv_sec = 1;
    _timeout.tv_nsec = 0; // 100ms
}
PSELECT::PSELECT()
{
    FD_ZERO(&_fds);
    _timeout.tv_sec = 1;
    _timeout.tv_nsec = 0; // 100ms
}

void PSELECT::addSocket(FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (socket > FD_SETSIZE)
        throw std::runtime_error("Index out of range");
    FD_SET(socket, &_fds);
    if (socket > _maxFd)
        _maxFd = socket;
    _sockets.push_back(socket);
}

void PSELECT::removeSocket(FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (socket > _maxFd)
        throw std::runtime_error("Index out of range");
    FD_CLR(socket, &_fds);
    _sockets.erase(std::remove(_sockets.begin(), _sockets.end(), socket), _sockets.end());
}

int PSELECT::wait()
{
    FD_ZERO(&_fds);
    for (auto &fd : _sockets)
        FD_SET(fd, &_fds);
    return ::pselect(_maxFd + 1, &_fds, nullptr, nullptr, &_timeout, nullptr);
}

bool PSELECT::isReady(FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (socket > _maxFd)
        throw std::runtime_error("Index out of range");
    return FD_ISSET(socket, &_fds);
}

#endif

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

void PPOLL::addSocket(FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (_pollfds.size() < std::size_t(socket))
        _pollfds.resize(socket + 1);
    _pollfds[socket] = pollfd{socket, POLLIN | POLLPRI, 0};
}

void PPOLL::addSocket(FileDescriptor socket, event_t events)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (_pollfds.size() < std::size_t(socket))
        _pollfds.resize(socket + 1);
    _pollfds[socket] = pollfd{socket, events, 0};
}

void PPOLL::removeSocket(FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (_pollfds.size() < std::size_t(socket))
        throw std::runtime_error("Index out of range");
    if (_pollfds.size() == std::size_t(socket))
        _pollfds.pop_back();
    else
        _pollfds[socket] = pollfd{-1, 0, 0};
}

int PPOLL::wait()
{
    #ifdef __linux__
        return ppoll(_pollfds.data(), _pollfds.size(), &_timeout, nullptr);
    #elif defined(__APPLE__)
        return poll(_pollfds.data(), _pollfds.size(), 100);
    #endif
}

pollfd &PPOLL::operator[](std::size_t index)
{
    if (_pollfds.size() < index)
        throw std::runtime_error("Index out of range");
    return _pollfds[index];
}

pollfd &PPOLL::operator[](FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (_pollfds.size() < std::size_t(socket))
        throw std::runtime_error("Index out of range");
    return _pollfds[socket];
}

bool PPOLL::isReady(pollfd fd)
{
    if (fd.fd == -1)
        throw std::runtime_error("Socket is -1");
    return fd.revents & (POLLIN | POLLPRI);
}

bool PPOLL::isReady(FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (_pollfds.size() < std::size_t(socket))
        throw std::runtime_error("Index out of range");
    if (_pollfds[socket].revents & (POLLIN | POLLPRI))
        return true;
    return false;
}

#endif

#if defined(_EPOLL_)

#endif

} // namespace Flakkari::Network
