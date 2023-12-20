/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-20
** File description:
** Socket
*/

#include "Socket.hpp"

namespace Flakkari::Network {

Socket::Socket(std::shared_ptr<Address> address)
    : _socket(INVALID_SOCKET), _address(address)
{
    #ifdef _WIN32
        WSADATA WSAData;
        if (::WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
            FLAKKARI_LOG_FATAL("WSAStartup failed");
            return;
        }
    #endif

    auto &addr = _address->getAddrInfo();
    _socket = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (_socket == INVALID_SOCKET) {
        FLAKKARI_LOG_FATAL("Failed to create socket, error: " + std::string(::strerror(errno)));
        return;
    }

    if (::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, "\001", 4)) {
        FLAKKARI_LOG_FATAL("Failed to set socket to reuse address and port, error: " + std::string(::strerror(errno)));
        return;
    }
}

Socket::Socket(socket_t socket, std::shared_ptr<Address> address)
    : _socket(socket), _address(address)
{
    #ifdef _WIN32
        WSADATA WSAData;
        if (::WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
            FLAKKARI_LOG_FATAL("WSAStartup failed");
            return;
        }
    #endif

    if (::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, "\001", 4)) {
        FLAKKARI_LOG_FATAL("Failed to set socket to reuse address and port, error: " + std::string(::strerror(errno)));
        return;
    }
}

Socket::Socket(Address address)
    : _socket(INVALID_SOCKET), _address(std::make_shared<Address>(address))
{
    #ifdef _WIN32
        WSADATA WSAData;
        if (::WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
            FLAKKARI_LOG_FATAL("WSAStartup failed");
            return;
        }
    #endif

    auto &addr = _address->getAddrInfo();
    _socket = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (_socket == INVALID_SOCKET) {
        FLAKKARI_LOG_FATAL("Failed to create socket, error: " + std::string(::strerror(errno)));
        return;
    }

    if (::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, "\001", 4)) {
        FLAKKARI_LOG_FATAL("Failed to set socket to reuse address and port, error: " + std::string(::strerror(errno)));
        return;
    }
}

Socket::Socket(ip_t ip, port_t port, Address::IpType ip_type, Address::SocketType socket_type)
    : _socket(INVALID_SOCKET), _address(std::make_shared<Address>(ip, port, socket_type, ip_type))
{
    #ifdef _WIN32
        WSADATA WSAData;
        if (::WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return;
        }
    #endif

    auto &addr = _address->getAddrInfo();
    _socket = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (_socket == INVALID_SOCKET) {
        FLAKKARI_LOG_FATAL("Failed to create socket, error: " + std::string(::strerror(errno)));
        return;
    }

    if (::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, "\001", 4)) {
        FLAKKARI_LOG_FATAL("Failed to set socket to reuse address and port, error: " + std::string(::strerror(errno)));
        return;
    }
}

Socket::~Socket() {
    this->close();
    #ifdef _WIN32
        ::WSACleanup();
    #endif
}

void Socket::bind() {
    auto &addr = _address->getAddrInfo();
    if (::bind(_socket, addr->ai_addr, addr->ai_addrlen) == SOCKET_ERROR) {
        FLAKKARI_LOG_FATAL("Failed to bind socket, error: " + std::string(::strerror(errno)));
        return;
    }
}

void Socket::listen(int backlog) {
    if (::listen(_socket, backlog) == SOCKET_ERROR) {
        FLAKKARI_LOG_FATAL("Failed to listen on socket, error: " + std::string(::strerror(errno)));
        return;
    }
}

void Socket::connect() {
    auto &addr = _address->getAddrInfo();
    if (::connect(_socket, addr->ai_addr, addr->ai_addrlen) == SOCKET_ERROR) {
        FLAKKARI_LOG_FATAL("Failed to connect socket, error: " + std::string(::strerror(errno)));
        return;
    }
}

std::shared_ptr<Socket> Socket::accept() {
    sockaddr_storage clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    SOCKET clientSocket = ::accept(_socket, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (clientSocket == INVALID_SOCKET) {
        FLAKKARI_LOG_FATAL("Failed to accept socket, error: " + std::string(::strerror(errno)));
        return nullptr;
    }

    auto _ip_type = (clientAddr.ss_family == AF_INET) ? Address::IpType::IPv4 : (clientAddr.ss_family == AF_INET6) ? Address::IpType::IPv6 : Address::IpType::None;

    auto clientAddress = std::make_shared<Address>(clientAddr, _address->getSocketType(), _ip_type);
    return std::make_shared<Socket>(clientSocket, clientAddress);
}

void Socket::send(const std::string &data, int flags) {
    if (::send(_socket, data.c_str(), data.size(), flags) == SOCKET_ERROR) {
        FLAKKARI_LOG_ERROR("Failed to send \"" + data + "\" to socket(" + std::to_string(_socket) + "), error: " + std::string(::strerror(errno)));
        return;
    }
}

void Socket::send(const byte *data, size_t size, int flags) {
    if (::send(_socket, (const char*)data, size, flags) == SOCKET_ERROR) {
        FLAKKARI_LOG_ERROR("Failed to send \"" + std::string((const char*)data, size) + "\" to socket(" + std::to_string(_socket) + "), error: " + std::string(::strerror(errno)));
        return;
    }
}

void Socket::sendTo(const std::string &data, const std::shared_ptr<Address> &address, int flags) {
    auto &addr = address->getAddrInfo();

    if (::sendto(_socket, data.c_str(), data.size(), flags, addr->ai_addr, addr->ai_addrlen) == SOCKET_ERROR) {
        FLAKKARI_LOG_ERROR("Failed to send \"" + data + "\" to \"" + address->toString().value_or("No address") + "\", error: " + std::string(::strerror(errno)));
        return;
    }
}

void Socket::sendTo(const byte *data, const size_t &size, const std::shared_ptr<Address> &address, int flags) {
    auto &addr = address->getAddrInfo();

    if (::sendto(_socket, (const char*)data, size, flags, addr->ai_addr, addr->ai_addrlen) == SOCKET_ERROR) {
        FLAKKARI_LOG_ERROR("Failed to send \"" + std::string((const char*)data, size) + "\" to \"" + address->toString().value_or("No address") + "\", error: " + std::string(::strerror(errno)));
        return;
    }
}

std::optional<std::string> Socket::receive(size_t size, int flags) {
    std::string data(size, 0);

    if (::recv(_socket, &data[0], size, flags) == SOCKET_ERROR) {
        FLAKKARI_LOG_ERROR("Failed to receive data from socket(" + std::to_string(_socket) + "), error: " + std::string(::strerror(errno)));
        return {};
    }
    return data;
}

std::optional<std::string> Socket::receive(int flags) {
    std::string data(4096, 0);

    if (::recv(_socket, &data[0], 4096, flags) == SOCKET_ERROR) {
        FLAKKARI_LOG_ERROR("Failed to receive data from socket(" + std::to_string(_socket) + "), error: " + std::string(::strerror(errno)));
        return {};
    }
    return data;
}

std::optional<std::pair<Address, std::string>> Socket::receiveFrom(size_t size, int flags) {
    std::string data(size, 0);
    sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);

    if (::recvfrom(_socket, &data[0], size, flags, (sockaddr*)&addr, &addrlen) == SOCKET_ERROR) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return {};
        FLAKKARI_LOG_ERROR("Failed to receive data from \"" + _address->toString().value_or("No address") + "\", error: " + std::string(::strerror(errno)));
        return {};
    }
    auto _ip_type = (addr.ss_family == AF_INET) ? Address::IpType::IPv4 : (addr.ss_family == AF_INET6) ? Address::IpType::IPv6 : Address::IpType::None;

    return std::make_pair(Address(addr, _address->getSocketType(), _ip_type), data);
}

std::pair<Address, std::optional<std::string>> Socket::receiveFrom(int flags) {
    std::string data(4096, 0);
    sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);

    if (::recvfrom(_socket, &data[0], 4096, flags, (sockaddr*)&addr, &addrlen) == SOCKET_ERROR) {
        FLAKKARI_LOG_ERROR("Failed to receive data from \"" + _address->toString().value_or("No address") + "\", error: " + std::string(::strerror(errno)));
        return {};
    }

    auto _ip_type = (addr.ss_family == AF_INET) ? Address::IpType::IPv4 : (addr.ss_family == AF_INET6) ? Address::IpType::IPv6 : Address::IpType::None;

    return std::make_pair(Address(addr, _address->getSocketType(), _ip_type), data);
}

void Socket::close() {
    #ifdef _WIN32
        ::closesocket(_socket);
    #else
        ::close(_socket);
    #endif
}

void Socket::setBlocking(bool blocking) {
    #ifdef _WIN32
        u_long mode = blocking ? 0 : 1;
        int result = ::ioctlsocket(_socket, FIONBIO, &mode);
    #else
        int flags = ::fcntl(_socket, F_GETFL, 0);

        if (flags == -1) {
            FLAKKARI_LOG_ERROR("Failed to get socket flags, error: " + std::string(::strerror(errno)));
            return;
        }

        int result = ::fcntl(_socket, F_SETFL, flags | (blocking ? 0 : O_NONBLOCK));
    #endif
    if (result == SOCKET_ERROR) {
        FLAKKARI_LOG_ERROR("Failed to set socket to " + std::string(blocking?"":"non") + "blocking, error: " + std::string(::strerror(errno)));
    }
}

std::ostream &operator<<(std::ostream &os, const Socket &socket)
{
    os << *socket.getAddress();
    os << " (";
    os << socket.getSocket();
    os << ")";
    return os;
}

} // namespace Flakkari::Network
