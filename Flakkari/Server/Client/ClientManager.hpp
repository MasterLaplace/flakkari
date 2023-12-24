/**************************************************************************
 * Flakkari Library v0.1.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file ClientManager.hpp
 * @brief This file contains the ClientManager class. It is used to handle
 * the clients connected to the server and the clients disconnection timeout.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.1.0
 * @date 2023-12-24
 **************************************************************************/

#ifndef CLIENTMANAGER_HPP_
#define CLIENTMANAGER_HPP_

#include "Network/Address.hpp"
#include "Client.hpp"

#include <unordered_map>
#include <memory>

namespace Flakkari {

/**
 * @brief Client manager class that handles the clients connected to the server
 * and the clients disconnection timeout.
 *
 * @details This class is used by the UDPServer to handle the clients connected
 * to the server and the clients disconnection timeout (5 seconds by default).
 * It is also used to check if the clients are still connected to the server
 * (using the checkInactiveClients() method) and to add a client to the client
 *
 * @see UDPServer
 * @see Client
 *
 * @example "Flakkari/Server/Client/ClientManager.cpp"
 * @code
 * #include "ClientManager.hpp"
 *
 * Flakkari::ClientManager clientManager;
 * clientManager.addClient(std::make_shared<Flakkari::Network::Address>("...")>);
 * clientManager.checkInactiveClients();
 * @endcode
 */
class ClientManager {
    public:
        /**
         * @brief Add a client to the client manager or update the last activity of the client
         *
         * @param client  The client's address
         */
        void addClient(std::shared_ptr<Network::Address> client);

        /**
         * @brief Remove a client from the client manager
         *
         * @param client  The client's address
         */
        void removeClient(std::shared_ptr<Network::Address> client);

        /**
         * @brief Check if the clients are still connected to the server
         * and remove the inactive clients from the client manager
         * (inactive clients are clients that didn't send any packet to the server
         * for more than 5 seconds)
         *
         * @see Client::isConnected()
         * @see Client::keepAlive()
         */
        void checkInactiveClients();

    protected:
    private:
        std::unordered_map<std::string /*ip:port*/, std::shared_ptr<Client>> _clients;
};

} /* namespace Flakkari */

#endif /* !CLIENTMANAGER_HPP_ */
