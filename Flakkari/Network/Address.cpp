/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-12-20
** File description:
** Address
*/

#include "Address.hpp"

namespace Flakkari::Network {

Address::Address(address_t &address, port_t port, SocketType socket_type, IpType ip_type)
    : _socket_type(socket_type), _ip_type(ip_type)
{
    struct hostent *host = nullptr;
    addrinfo hints;
    hints.ai_family = (ip_type == IpType::IPv4) ? AF_INET : AF_INET6;
    hints.ai_socktype = (socket_type == SocketType::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    hints.ai_protocol = (socket_type == SocketType::TCP) ? IPPROTO_TCP : IPPROTO_UDP;
    hints.ai_flags = (AI_PASSIVE | AI_V4MAPPED | AI_ALL);

    if ((host = gethostbyname(address.c_str())) == nullptr) {
        FLAKKARI_LOG_ERROR("gethostbyname() failed for " + address);
        return;
    }

    addrinfo *result = nullptr;
    if (getaddrinfo(inet_ntoa(*(struct in_addr *)host->h_addr), std::to_string(port).c_str(), &hints, &result) != 0) {
        FLAKKARI_LOG_ERROR("getaddrinfo() failed");
        return;
    }

    _addrInfo = std::shared_ptr<addrinfo>(result, [](addrinfo *addrInfo) {
        if (addrInfo != nullptr)
            freeaddrinfo(addrInfo);
    });
}

Address::Address(port_t port, SocketType socket_type, IpType ip_type)
    : _socket_type(socket_type), _ip_type(ip_type)
{
    addrinfo hints;
    hints.ai_family = (ip_type == IpType::IPv4) ? AF_INET : AF_INET6;
    hints.ai_socktype = (socket_type == SocketType::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    hints.ai_protocol = (socket_type == SocketType::TCP) ? IPPROTO_TCP : IPPROTO_UDP;
    hints.ai_flags = (AI_PASSIVE | AI_V4MAPPED | AI_ALL);

    addrinfo *result = nullptr;
    if (getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &result) != 0) {
        FLAKKARI_LOG_ERROR("getaddrinfo() failed");
        return;
    }

    _addrInfo = std::shared_ptr<addrinfo>(result, [](addrinfo *addrInfo) {
        if (addrInfo != nullptr)
            freeaddrinfo(addrInfo);
    });
}

Address::Address(const sockaddr_in &clientAddr, SocketType socket_type, IpType ip_type)
    : _socket_type(socket_type), _ip_type(ip_type)
{
    addrinfo hints;

    hints.ai_family = (_ip_type == IpType::IPv4) ? AF_INET : AF_INET6;
    hints.ai_socktype = (_socket_type == SocketType::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    hints.ai_protocol = (_socket_type == SocketType::TCP) ? IPPROTO_TCP : IPPROTO_UDP;
    hints.ai_flags = (AI_PASSIVE | AI_V4MAPPED | AI_ALL);

    addrinfo *result = nullptr;
    if (getaddrinfo(inet_ntoa(clientAddr.sin_addr), std::to_string(ntohs(clientAddr.sin_port)).c_str(), &hints, &result) != 0) {
        FLAKKARI_LOG_ERROR("getaddrinfo() failed");
        return;
    }

    _addrInfo = std::shared_ptr<addrinfo>(result, [](addrinfo *addrInfo) {
        if (addrInfo != nullptr)
            freeaddrinfo(addrInfo);
    });
}

Address::Address(const sockaddr_storage &clientAddr, SocketType socket_type, IpType ip_type)
    : _socket_type(socket_type), _ip_type(ip_type)
{
    addrinfo hints;
    hints.ai_family = (_ip_type == IpType::IPv4) ? AF_INET : AF_INET6;
    hints.ai_socktype = (_socket_type == SocketType::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    hints.ai_protocol = (_socket_type == SocketType::TCP) ? IPPROTO_TCP : IPPROTO_UDP;
    hints.ai_flags = (AI_PASSIVE | AI_V4MAPPED | AI_ALL);

    addrinfo *result = nullptr;
    const char *name = inet_ntoa(((sockaddr_in *)&clientAddr)->sin_addr);
    const char *service = std::to_string(ntohs(((sockaddr_in *)&clientAddr)->sin_port)).c_str();

    if (getaddrinfo(name, service, &hints, &result) != 0) {
        FLAKKARI_LOG_ERROR("getaddrinfo() failed");
        return;
    }

    _addrInfo = std::shared_ptr<addrinfo>(result, [](addrinfo *addrInfo) {
        if (addrInfo != nullptr)
            freeaddrinfo(addrInfo);
    });
}

std::optional<std::string> Address::toString() const {
    if (_addrInfo == nullptr)
        return {};
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    if (getnameinfo(_addrInfo->ai_addr, _addrInfo->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV) != 0) {
        FLAKKARI_LOG_ERROR("getnameinfo() failed");
        return {};
    }
    return std::string(host) + ":" + std::string(service);
};

constexpr const char *Address::ipTypeToString(IpType ip_type)
{
    switch (ip_type) {
        case IpType::None:
            return "None";
        case IpType::IPv4:
            return "IPv4";
        case IpType::IPv6:
            return "IPv6";
        default:
            return "Unknown";
    }
}

constexpr const char *Address::socketTypeToString(SocketType socket_type)
{
    switch (socket_type) {
        case SocketType::None:
            return "None";
        case SocketType::TCP:
            return "TCP";
        case SocketType::UDP:
            return "UDP";
        default:
            return "Unknown";
    }
}

std::ostream &operator<<(std::ostream &os, const Address &addr) {
    os << addr.toString().value_or("null");
    os << " (";
    os << Address::socketTypeToString(addr.getSocketType());
    os << ", ";
    os << Address::ipTypeToString(addr.getIpType());
    os << ")";
    return os;
}

} // namespace Flakkari::Network
