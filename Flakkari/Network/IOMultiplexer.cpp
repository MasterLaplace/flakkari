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

PSELECT::PSELECT(FileDescriptor fileDescriptor, long int seconds, long int microseconds)
{
    if (fileDescriptor == -1)
    {
        FLAKKARI_LOG_ERROR("Socket is -1");
        throw std::runtime_error("Socket is -1");
    }

    FD_ZERO(&_fds);
    _maxFd = fileDescriptor;
    _sockets.push_back(fileDescriptor);

    _timeout.tv_sec = seconds;
    _timeout.tv_nsec = microseconds;
}

PSELECT::PSELECT(long int seconds, long int microseconds)
{
    FD_ZERO(&_fds);

    _timeout.tv_sec = seconds;
    _timeout.tv_nsec = microseconds;
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
    _maxFd = *std::max_element(_sockets.begin(), _sockets.end());
}

int PSELECT::wait()
{
    FD_ZERO(&_fds);
    for (auto &fd : _sockets)
        FD_SET(fd, &_fds);
#    if defined(__APPLE__)
    return ::select(_maxFd + 1, &_fds, nullptr, nullptr, (struct timeval *) &_timeout);
#    else
    return ::pselect(_maxFd + 1, &_fds, nullptr, nullptr, &_timeout, nullptr);
#    endif
}

bool PSELECT::isReady(FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");
    if (socket > _maxFd)
        throw std::runtime_error("Index out of range");
    return FD_ISSET(socket, &_fds);
}

bool PSELECT::skipableError() { return errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK; }

#endif

#if defined(_PPOLL_)

PPOLL::PPOLL(FileDescriptor fileDescriptor, event_t events, long int seconds, long int microseconds)
{
    if (fileDescriptor == -1)
    {
        FLAKKARI_LOG_ERROR("Socket is -1");
        throw std::runtime_error("Socket is -1");
    }

    if (_pollfds.size() < std::size_t(fileDescriptor))
        _pollfds.resize(fileDescriptor + 1);
    _pollfds[fileDescriptor] = pollfd{fileDescriptor, events, 0};

    _timeout.tv_sec = seconds;
    _timeout.tv_nsec = microseconds;
}

PPOLL::PPOLL(long int seconds, long int microseconds)
{
    _timeout.tv_sec = seconds;
    _timeout.tv_nsec = microseconds;
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
#    ifdef __linux__
    return ppoll(_pollfds.data(), _pollfds.size(), &_timeout, nullptr);
#    elif defined(__APPLE__)
    return poll(_pollfds.data(), _pollfds.size(), 100);
#    endif
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

bool PPOLL::skipableError() { return errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK; }

#endif

#if defined(_WSA_)

WSA::WSA(FileDescriptor socket, int seconds, int microseconds)
{
    if (socket == -1)
        FLAKKARI_LOG_FATAL("Socket is -1");

    _sockets.reserve(MAX_POLLFD);
    _fdArray.reserve(MAX_POLLFD);
    _freeSpace.reserve(MAX_POLLFD);

    _timeoutInMs = seconds * 1000 + microseconds / 1000;

    WSAPOLLFD pollFd;
    pollFd.fd = socket;
    pollFd.events = POLLIN | POLLOUT;
    pollFd.revents = 0;
    _fdArray.emplace_back(pollFd);
    _sockets.emplace_back(socket);
}

void WSA::addSocket(FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");

    if (std::find(_sockets.begin(), _sockets.end(), socket) != _sockets.end())
        return;

    WSAPOLLFD pollFd;
    pollFd.fd = socket;
    pollFd.events = POLLIN | POLLOUT;
    pollFd.revents = 0;
    if (_freeSpace.empty())
    {
        _fdArray.emplace_back(pollFd);
        _sockets.emplace_back(socket);
        return;
    }

    size_t index = _freeSpace.back();
    _fdArray[index] = pollFd;
    _sockets[index] = socket;
    _freeSpace.pop_back();
}

void WSA::removeSocket(FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");

    auto it = std::find(_sockets.begin(), _sockets.end(), socket);
    if (it == _sockets.end())
        throw std::runtime_error("Socket not found");

    size_t index = std::distance(_sockets.begin(), it);
    _fdArray[index].fd = 0;
    _sockets[index] = 0;
    _freeSpace.emplace_back(index);
}

int WSA::wait() { return WSAPoll(_fdArray.data(), (ULONG) _fdArray.size(), _timeoutInMs); }

bool WSA::isReady(FileDescriptor socket)
{
    if (socket == -1)
        throw std::runtime_error("Socket is -1");

    auto it = std::find(_sockets.begin(), _sockets.end(), socket);
    if (it == _sockets.end())
        throw std::runtime_error("Socket not found");

    size_t index = std::distance(_sockets.begin(), it);
    return _fdArray[index].revents & (POLLIN | POLLOUT);
}

bool WSA::skipableError()
{
    int error = WSAGetLastError();
    return error == WSAEINTR || error == WSAEWOULDBLOCK || error == WSAEINPROGRESS;
}

#endif

} // namespace Flakkari::Network
