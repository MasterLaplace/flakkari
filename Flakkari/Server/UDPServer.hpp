/**************************************************************************
 * Flakkari Library v0.1.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file UDPServer.hpp
 * @brief This file contains the UDPServer class. It is used to handle
 * incoming packets and clients. It is the main class of the server.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.1.0
 * @date 2023-12-24
 **************************************************************************/

#ifndef UDPSERVER_HPP_
#define UDPSERVER_HPP_

#include "Network/IOMultiplexer.hpp"
#include "Client/ClientManager.hpp"

namespace Flakkari {

/**
 * @brief UDP Server class that handles incoming packets and clients
 *
 * @details This class is the main class of the server, it handles incoming
 * packets and clients, it also handles the client's timeout and disconnection
 * @see ClientManager
 * @see Network::Socket
 * @see Network::PPOLL
 *
 * @example "Flakkari/Server/UDPServer.cpp"
 * @code
 * #include "UDPServer.hpp"
 *
 * Flakkari::UDPServer server("localhost", 8080);
 * return server.run();
 * @endcode
 */
class UDPServer {
    public:
        /**
         * @brief Construct a new UDPServer object
         *
         * @param ip The ip to bind the server to (default: localhost)
         * @param port The port to bind the server to (default: 8080)
         */
        UDPServer(std::string ip = "localhost", std::size_t port = 8080);
        ~UDPServer() = default;

        /**
         * @brief Run the server and wait for incoming packets and clients
         *
         * @return int  0 if everything went well, 84 otherwise
         */
        int run();

    protected:
    private:
        Network::Socket _socket;
        std::unique_ptr<Network::PPOLL> _io;
        ClientManager _clientManager;
};

} /* namespace Flakkari */

#endif /* !UDPSERVER_HPP_ */
