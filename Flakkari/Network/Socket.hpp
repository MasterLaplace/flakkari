/**************************************************************************
 * Flakkari Library v0.0.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Socket.hpp
 * @brief This file contains the Socket class. It is used to create a socket and send/receive data from it.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.0.0
 * @date 2023-12-20
 **************************************************************************/

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include "Address.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>

    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "Mswsock.lib")

    typedef SOCKET SOCKET;
    #define INVALID_SOCKET (SOCKET)(~0)
    #define SOCKET_ERROR (-1)
#else
    #include <iostream>
    #include <cstring>
    #include <arpa/inet.h>

    #ifdef _POSIX_C_SOURCE
    #include <fcntl.h>
    #include <unistd.h>
    #else
    #error "POSIX not defined"
    #endif

    typedef int SOCKET;
    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR (-1)
#endif

namespace Flakkari::Network {

/**
 * @brief This class is used to create a socket and send/receive data from it.
 * It is used by the Server and Client classes.
 *
 * @see Server
 * @see Client
 * @see Address
 *
 * @example "Socket class for a UDP server":
 * @code
 * // Create a socket
 * auto socket = std::make_shared<Socket>(12345, Address::IpType::IPv4, Address::SocketType::UDP);
 * // Bind the socket
 * socket->bind();
 * // Send data to a client
 * socket->sendTo("Hello World!", socket->getAddress());
 * // Receive data from a client
 * auto data = socket->receiveFrom();
 * std::cout << data->second << std::endl;
 * @endcode
 *
 * @example "Socket class for a TCP server":
 * @code
 * // Create a socket
 * auto socket = std::make_shared<Socket>(12345, Address::IpType::IPv4, Address::SocketType::TCP);
 * // Bind the socket
 * socket->bind();
 * // Listen for incoming connections
 * socket->listen();
 * // Accept a client
 * auto clientSocket = socket->accept();
 * // Send data to a client
 * clientSocket->send("Hello World!");
 * // Receive data from a client
 * auto data = clientSocket->receive();
 * @endcode
 *
 * @example "Socket class for a UDP client":
 * @code
 * // Create a socket
 * auto socket = std::make_shared<Socket>(12345, Address::IpType::IPv4, Address::SocketType::UDP);
 * // Send data to the server
 * socket->sendTo("Hello World!", socket->getAddress());
 * // Receive data from the server
 * auto data = socket->receiveFrom();
 * std::cout << data->second << std::endl;
 * @endcode
 *
 * @example "Socket class for a TCP client":
 * @code
 * // Create a socket
 * auto socket = std::make_shared<Socket>(12345, Address::IpType::IPv4, Address::SocketType::TCP);
 * // Connect to a server
 * socket->connect();
 * // Send data to the server
 * socket->send("Hello World!");
 * // Receive data from the server
 * auto data = socket->receive();
 * @endcode
 */
class Socket {
    public:
        using port_t = Address::port_t;
        using ip_t = const std::string &;
        using socket_t = SOCKET;
        using byte = unsigned char;

    public:
        Socket(std::shared_ptr<Address> address);
        Socket(socket_t socket, std::shared_ptr<Address> address);
        Socket(Address address);
        Socket(ip_t address, port_t port, Address::IpType ip_type, Address::SocketType socket_type);
        Socket(const Socket &) = default;
        Socket(Socket &&) = default;
        ~Socket();

        bool operator==(const Socket &other) const { return _socket == other._socket; }
        bool operator==(const int &socket) const { return _socket == socket; }

        socket_t getSocket() const { return _socket; }
        std::shared_ptr<Address> getAddress() const { return _address; }

        void bind();
        void listen(int backlog = SOMAXCONN);
        void connect();
        std::shared_ptr<Socket> accept();
        void setBlocking(bool blocking = true);

        void send(const std::string &data, int flags = 0);
        void send(const byte *data, size_t size, int flags = 0);
        void sendTo(const std::string &data, const std::shared_ptr<Address> &address, int flags = 0);
        void sendTo(const byte *data, const size_t &size, const std::shared_ptr<Address> &address, int flags = 0);

        std::optional<std::string> receive(size_t size, int flags = 0);
        std::optional<std::string> receive(int flags = 0);
        std::optional<std::pair<Address, std::string>> receiveFrom(size_t size, int flags = 0);
        std::pair<Address, std::optional<std::string>> receiveFrom(int flags = 0);

        void close();

    protected:
    private:
        socket_t _socket;
        std::shared_ptr<Address> _address = nullptr;
};

/**
 * @brief Convert Socket to string
 *
 * @param os  Output stream
 * @param addr  Socket to convert
 * @return std::ostream&  Output stream
 */
std::ostream &operator<<(std::ostream &os, const Socket &socket);

} // namespace Flakkari::Network

#endif /* !SOCKET_HPP_ */