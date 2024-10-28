/**************************************************************************
 * Flakkari Library v0.0.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file UDPServer.hpp
 * @brief UDPServer is a class that represents a Basic UDP Server.
 * It is used to create a UDP Server. It is a basic server that
 * can only send and receive data. It is not a full fledged server.
 * It is used to test the library. It is not meant to be used in
 * production.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.0.0
 * @date 2023-12-24
 **************************************************************************/

#ifndef UDPSERVER_HPP_
#define UDPSERVER_HPP_

#include "Network/IOMultiplexer.hpp"
#include "Network/Network.hpp"

#ifndef STDIN_FILENO
    #define STDIN_FILENO 0
#endif

#ifdef _PSELECT_
    #define IO_SELECTED Network::PSELECT
#elif defined(_WSA_)
    #define IO_SELECTED Network::WSA
#endif

namespace Flakkari {

class UDPServer {
    public:
        UDPServer(std::string ip, unsigned short port) :
            _io(std::make_unique<IO_SELECTED>())
        {
            Network::initNetwork();

            _socket.create(ip, port, Network::Address::IpType::IPv4, Network::Address::SocketType::UDP);
            std::cout << _socket << std::endl;
            _socket.bind();

            _io->addSocket(_socket.getSocket());
            _io->addSocket(STDIN_FILENO);
        }
        ~UDPServer() {
            Network::cleanupNetwork();
        }

        int run() {
            while (true)
            {
                int result = _io->wait();

                if (result == -1) {
                    FLAKKARI_LOG_FATAL("Failed to poll sockets, error: " + std::string(::strerror(errno)));
                    return 84;
                } else if (result == 0) {
                    FLAKKARI_LOG_DEBUG("ppoll timed out");
                    continue;
                }
                for (auto &fd : *_io) {
                    if (_io->isReady(fd)) {
                        auto packet = _socket.receiveFrom();
                        std::cout << (*packet.value().first.get());
                        std::cout << " : ";
                        std::cout << packet.value().second << std::endl;
                        _socket.sendTo(packet.value().first, packet.value().second);
                    }
                }
            }
            return 0;
        }

    protected:
    private:
        Network::Socket _socket;
        std::unique_ptr<IO_SELECTED> _io;
};

} /* namespace Flakkari */

#endif /* !UDPSERVER_HPP_ */
