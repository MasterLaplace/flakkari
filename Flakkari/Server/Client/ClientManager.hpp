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
 * Flakkari::ClientManager::addClient(std::make_shared<Flakkari::Network::Address>("...")>);
 * Flakkari::ClientManager::checkInactiveClients();
 * @endcode
 */
class ClientManager {
    private:
        static std::shared_ptr<ClientManager> _instance;

    public:
        std::unordered_map<std::string /*ip:port*/, std::shared_ptr<Client>> _clients;

    public:
        ClientManager(const ClientManager &) = delete;
        ClientManager(const std::shared_ptr<ClientManager> &) = delete;
        void operator=(const ClientManager &) = delete;
        void operator=(const std::shared_ptr<ClientManager> &) = delete;

        /**
         * @brief Construct a new ClientManager object
         *
         */
        ClientManager() = default;

        /**
         * @brief Destroy the ClientManager object
         *
         */
        ~ClientManager() = default;

        /**
         * @brief Get the instance of the client manager
         *
         * @return std::shared_ptr<ClientManager>  The instance of the client manager
         */
        static std::shared_ptr<ClientManager> getInstance();

        /**
         * @brief Add a client to the client manager or update the last activity of the client
         *
         * @param client  The client's address
         */
        static void addClient(std::shared_ptr<Network::Address> client);

        /**
         * @brief Remove a client from the client manager
         *
         * @param client  The client's address
         */
        static void removeClient(std::shared_ptr<Network::Address> client);

        /**
         * @brief Check if the clients are still connected to the server
         * and remove the inactive clients from the client manager
         * (inactive clients are clients that didn't send any packet to the server
         * for more than 5 seconds)
         *
         * @see Client::isConnected()
         * @see Client::keepAlive()
         */
        static void checkInactiveClients();

        /**
         * @brief Get the Client object
         *
         * @param client  The client's address
         * @return std::shared_ptr<Client>  The client object
         */
        static std::shared_ptr<Client> getClient(std::shared_ptr<Network::Address> client);

        /**
         * @brief Get the Client object
         *
         * @param ip  The client's ip address and port
         * @return std::shared_ptr<Client>  The client object
         */
        static std::shared_ptr<Client> getClient(std::string ip);

        /**
         * @brief Get the Address object
         *
         * @param ip  The client's ip address and port
         * @return std::shared_ptr<Network::Address>  The client's address
         */
        static std::shared_ptr<Network::Address> getAddress(std::string ip);

        /**
         * @brief Get the client object from the client manager
         *
         * @param ip  The client's ip address and port
         * @return std::shared_ptr<Client>  The client object
         */
        std::shared_ptr<Client> operator[](std::string ip);
};

} /* namespace Flakkari */

#endif /* !CLIENTMANAGER_HPP_ */
