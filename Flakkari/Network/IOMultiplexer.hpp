/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** IOMultiplexer
*/

#ifndef IOMULTIPLEXER_HPP_
#define IOMULTIPLEXER_HPP_

// if im on windows, define all compatible IOMultiplexer
#ifdef _WIN32
    #define _PSELECT_
    #define _PPOLL_
    #define _EPOLL_
    #define _KQUEUE_
    #define _IO_URING_
#else
    #define _PSELECT_
    #define _PPOLL_
    #define _EPOLL_
#endif

#include "Socket.hpp"
#include <vector>

namespace Flakkari::Network {
/**
 * @brief IOMultiplexer is an interface for the different I/O multiplexing
 * @interface IOMultiplexer
 */
class IOMultiplexer {
    public:
        using FileDescriptor = int;

    public:
        virtual ~IOMultiplexer() = default;

        virtual void addSocket(FileDescriptor socket) = 0;
        virtual void removeSocket(FileDescriptor socket) = 0;
        virtual int wait() = 0;
        virtual bool isReady(FileDescriptor socket) = 0;

};

#if defined(_PSELECT_)
#include <sys/select.h>
#endif

#if defined(_PPOLL_)
#include <sys/poll.h>

/**
 * @brief PPOLL is a class that represents a PPOLL
 *
 * @class PPOLL
 * @implements IOMultiplexer
 * @see IOMultiplexer
 *
 * @example "PPOLL example":
 * @code
 * auto socket = std::make_shared<Socket>(12345, Address::IpType::IPv4, Address::SocketType::UDP);
 * socket->bind();
 *
 * auto io = std::make_unique<PPOLL>();
 * io->addSocket(socket->getSocket());
 *
 * while (true) {
 *    int result = io->wait();
 *    if (result > 0) {
 *       for (auto &fd : *io) {
 *         if (io->isReady(fd)) {
 *          // do something
 *         }
 *       }
 *    }
 * }
 * @endcode
 */
class PPOLL : public IOMultiplexer {
    public:
        using pollfd = struct pollfd;
        using event_t = short int;
        using revents_t = short int;
        using nfds_t = unsigned long int;

    public:
        PPOLL(int fileDescriptor, event_t events)
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
        PPOLL() {
            _timeout.tv_sec = 1;
            _timeout.tv_nsec = 0;// 100000000;  // 100ms
        }
        ~PPOLL() = default;

        void addSocket(FileDescriptor socket) override {
            if (socket == -1)
                throw std::runtime_error("Socket is -1");
            if (_pollfds.size() < std::size_t(socket))
                _pollfds.resize(socket + 1);
            _pollfds[socket] = pollfd{socket, POLLIN | POLLPRI, 0};
        }

        void addSocket(FileDescriptor socket, event_t events) {
            if (socket == -1)
                throw std::runtime_error("Socket is -1");
            if (_pollfds.size() < std::size_t(socket))
                _pollfds.resize(socket + 1);
            _pollfds[socket] = pollfd{socket, events, 0};
        }

        void removeSocket(FileDescriptor socket) override {
            if (socket == -1)
                throw std::runtime_error("Socket is -1");
            if (_pollfds.size() < std::size_t(socket))
                throw std::runtime_error("Index out of range");
            if (_pollfds.size() == std::size_t(socket))
                _pollfds.pop_back();
            else
                _pollfds[socket] = pollfd{-1, 0, 0};
        }

        int wait() override {
            return ppoll(_pollfds.data(), _pollfds.size(), &_timeout, nullptr);
        }

        pollfd &operator[](std::size_t index) {
            if (_pollfds.size() < index)
                throw std::runtime_error("Index out of range");
            return _pollfds[index];
        }

        pollfd &operator[](FileDescriptor socket) {
            if (socket == -1)
                throw std::runtime_error("Socket is -1");
            if (_pollfds.size() < std::size_t(socket))
                throw std::runtime_error("Index out of range");
            return _pollfds[socket];
        }

        std::vector<pollfd>::iterator begin() { return _pollfds.begin(); }
        std::vector<pollfd>::iterator end() { return _pollfds.end(); }

        [[nodiscard]] bool isReady(pollfd fd) {
            if (fd.fd == -1)
                throw std::runtime_error("Socket is -1");
            return fd.revents & (POLLIN | POLLPRI);
        }

        [[nodiscard]] bool isReady(FileDescriptor socket) override {
            if (socket == -1)
                throw std::runtime_error("Socket is -1");
            if (_pollfds.size() < std::size_t(socket))
                throw std::runtime_error("Index out of range");
            if (_pollfds[socket].revents & (POLLIN | POLLPRI))
                return true;
            return false;
        }

    protected:
    private:
        std::vector<pollfd> _pollfds;
        struct timespec _timeout = {0, 0};
};
#endif

#if defined(_EPOLL_)
#include <sys/epoll.h>
#endif

#if defined(_KQUEUE_)
#include <sys/event.h>
#endif

#if defined(_IO_URING_)
#include <liburing.h>
#endif
} // namespace Flakkari::Network

#endif /* !IOMULTIPLEXER_HPP_ */
