/**************************************************************************
 * Flakkari Library v0.0.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file UDPClient.hpp
 * @brief UDPClient is a class that represents a Basic UDP Client. It is
 * used to create a UDP Client. It is a basic client that can only send
 * and receive data. It is not a full fledged client. It is used to test
 * the library. It is not meant to be used in production.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.0.0
 * @date 2023-12-24
 **************************************************************************/

#ifndef UDPCLIENT_HPP_
#define UDPCLIENT_HPP_

#include "Network/IOMultiplexer.hpp"

namespace Flakkari {

class UDPClient {
    public:
        UDPClient(std::string ip = "localhost", std::size_t port = 8080) :
            _socket(Flakkari::Network::Socket(ip, port, Flakkari::Network::Address::IpType::IPv4, Flakkari::Network::Address::SocketType::UDP))
        {
            std::cout << _socket << std::endl;

            _io = std::make_unique<Flakkari::Network::PPOLL>();
            _io->addSocket(_socket.getSocket());
        }
        ~UDPClient() = default;

        int run() {
            while (true)
            {
                int result = _io->wait();

                if (result == -1) {
                    FLAKKARI_LOG_FATAL("Failed to poll sockets, error: " + std::string(::strerror(errno)));
                    return 84;
                } else if (result == 0) {
                    FLAKKARI_LOG_DEBUG("ppoll timed out");
                    Network::Buffer buffer(1024);
                    std::cin >> buffer;
                    _socket.sendTo( _socket.getAddress(), buffer);
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
        std::unique_ptr<Network::PPOLL> _io;
};

} /* namespace Flakkari */

#endif /* !UDPCLIENT_HPP_ */
