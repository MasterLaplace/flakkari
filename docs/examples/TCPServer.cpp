/**************************************************************************
 * Flakkari Library v0.0.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file TCPServer.hpp
 * @brief TCPServer is a class that represents a Basic TCP Server.
 * It is used to create a TCP Server. It is a basic server that
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

#ifndef TCPSERVER_HPP_
#define TCPSERVER_HPP_

#include "Network/IOMultiplexer.hpp"

namespace Flakkari {

class TCPServer {
    public:
        TCPServer(std::string ip = "localhost", std::size_t port = 8080) :
            _socket(Network::Socket(ip, port, Network::Address::IpType::IPv6, Network::Address::SocketType::TCP))
        {
            std::cout << _socket << std::endl;
            _socket.bind();
            _socket.listen();

            _io = std::make_unique<Network::PPOLL>();
            _io->addSocket(_socket.getSocket());
            _io->addSocket(STDIN_FILENO);
        }
        ~TCPServer() = default;

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
                        auto packet = _socket.receive();
                        std::cout << (*packet.value().first.get());
                        std::cout << " : ";
                        std::cout << packet.value().second << std::endl;
                        _socket.send(packet.value().first, packet.value().second);
                    }
                }
            }
            return 0;
        }

    protected:
    private:
        Network::Socket _socket;
        std::unique_ptr<Network::PPOLL> _io;
};

} /* namespace Flakkari */

#endif /* !TCPSERVER_HPP_ */
