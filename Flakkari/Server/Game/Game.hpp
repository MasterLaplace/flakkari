/**************************************************************************
 * Flakkari Library v0.4.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Game.hpp
 * @brief Game class header. Can be used to create a game.
 *        A game is a collection of scenes.
 *        A scene is a collection of entities.
 *        An entity is a collection of components.
 *        A component is a collection of data.
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.4.0
 * @date 2024-01-06
 **************************************************************************/

#ifndef GAME_HPP_
#define GAME_HPP_

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>

#include "Engine/EntityComponentSystem/EntityFactory.hpp"
#include "Engine/EntityComponentSystem/Systems/Systems.hpp"

#include "Protocol/PacketFactory.hpp"

#include "ResourceManager.hpp"

namespace Flakkari {

class Client;

using nl_entity = nlohmann::detail::iteration_proxy<nlohmann::detail::iter_impl<nlohmann::json>>;
using nl_template =
    nlohmann::basic_json<std::map, std::vector, std::string, bool, int64_t, uint64_t, double, std::allocator,
                         nlohmann::adl_serializer, std::vector<uint8_t, std::allocator<uint8_t>>, void>;
using nl_component = nlohmann::json;

class Game {
public:
    friend class Client;

public: // Constructors/Destructors
    /**
     * @brief Construct a new Game object and load the config file
     *        of the game.
     *
     * @param name  Name of the game (name of the file in Games/ folder)
     * @param config  Config of the game (config of the file in Games/ folder)
     */
    Game(const std::string &name, std::shared_ptr<nlohmann::json> config);
    ~Game();

public: // Loaders
    /**
     * @brief Add all the systems of the game to the registry.
     *
     * @param registry  Registry to add the systems to.
     * @param name  Name of the scene to load.
     */
    void loadSystems(Engine::ECS::Registry &registry, const std::string &name);

    /**
     * @brief Add all the components of the game to the registry.
     *
     * @param registry  Registry to add the components to.
     * @param componentInfo  Info of the components to add.
     * @param newEntity  Entity to add the components to.
     */
    void loadComponents(Engine::ECS::Registry &registry, const nl_component &componentInfo,
                        Engine::ECS::Entity newEntity);

    /**
     * @brief Add all the entities of the game to the registry.
     *
     * @param registry  Registry to add the entities to.
     * @param entity  Entity to add to the registry.
     * @param templates  Templates of the game.
     */
    void loadEntityFromTemplate(Engine::ECS::Registry &registry, const nl_entity &entity, const nl_template &templates);

    /**
     * @brief Load a scene from the game.
     *
     * @param name  Name of the scene to load.
     */
    void loadScene(const std::string &name);

public: // Actions
    void sendAllEntities(const std::string &sceneName, std::shared_ptr<Client> player);
    void sendOnSameScene(const std::string &sceneName, const Network::Buffer &message);

    void sendOnSameSceneExcept(const std::string &sceneName, const Network::Buffer &message,
                               std::shared_ptr<Client> except);

    /**
     * @brief Check if a player is disconnected.
     *
     */
    void checkDisconnect();

    /**
     * @brief Send a packet to a player.
     *
     * @param player  Player to send the packet to.
     * @param pos  Position of the player.
     * @param vel  Velocity of the player.
     */
    void sendUpdatePosition(std::shared_ptr<Client> player, Engine::ECS::Components::_2D::Transform pos,
                            Engine::ECS::Components::_2D::Movable vel);

    /**
     * @brief Handle an event from a player.
     *
     * @param player  Player that sent the event.
     * @param packet  Packet containing the event.
     */
    void handleEvent(std::shared_ptr<Client> player, Protocol::Packet<Protocol::CommandId> packet);

    /**
     * @brief Empty the incoming packets of the players and update the
     *        game with the new packets.
     *
     */
    void updateIncomingPackets(unsigned char maxMessagePerFrame = 10);

    /**
     * @brief Update the game. This function is called every frame.
     *
     */
    void update();

    /**
     * @brief Start the game. This function is called when the game is
     *        launched. It will start the game loop.
     *
     */
    void start();

    /**
     * @brief Run the game. This function is called when the game is
     *        started. It will run the game loop.
     *
     */
    void run();

    /**
     * @brief Add a player to the game instance.
     *
     * @param player  Player to add to the game instance.
     * @return true  Player added successfully
     * @return false  Player not added
     */
    [[nodiscard]] bool addPlayer(std::shared_ptr<Client> player);

    /**
     * @brief Remove a player from the game instance.
     *
     * @param player  Player to remove from the game instance.
     * @return true  Player removed successfully
     * @return false  Player not removed
     */
    bool removePlayer(std::shared_ptr<Client> player);

    /**
     * @brief Get if the game is running.
     *
     * @return true  Game is running
     * @return false  Game is not running
     */
    [[nodiscard]] bool isRunning() const;

public: // Getters
    /**
     * @brief Get the Name object (name of the game).
     *
     * @return std::string  Name of the game
     */
    [[nodiscard]] std::string getName() const;

    /**
     * @brief Get the Players object (players of the game).
     *
     * @return std::vector<std::shared_ptr<Client>>  Players of the game
     */
    [[nodiscard]] std::vector<std::shared_ptr<Client>> getPlayers() const;

protected:
private:
    bool _running = false;                                                                    // Is the game running
    std::thread _thread;                                                                      // Thread of the game
    std::string _name;                                                                        // Name of the game
    std::shared_ptr<nlohmann::json> _config;                                                  // Config of the game
    std::vector<std::shared_ptr<Client>> _players;                                            // Players of the game
    float _deltaTime;                                                                         // Time between two frames
    std::chrono::steady_clock::time_point _time;                                              // Time of the last frame
    std::unordered_map<std::string /*sceneName*/, Engine::ECS::Registry /*content*/> _scenes; // Scenes of the game
};

} /* namespace Flakkari */

#endif /* !GAME_HPP_ */
