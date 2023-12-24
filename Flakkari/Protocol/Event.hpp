/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** Event
*/

#ifndef EVENT_HPP_
#define EVENT_HPP_

namespace Flakkari::Protocol::API {

    inline namespace V_1 {

        enum class FlakkariEventId {
            // 0 - 99: System
            REQ_CONNECT = 0, // Client -> Server (Connect to server)
            REP_CONNECT = 1, // Server -> Client (Connection accepted)
            REQ_DISCONNECT = 2, // Client -> Server (Disconnect from server)
            REP_DISCONNECT = 3, // Server -> Client (Disconnect accepted)
            REQ_HEARTBEAT = 8, // Client -> Server (Heartbeat server) (Keep alive)
            REP_HEARTBEAT = 9, // Server -> Client (Heartbeat accepted) (Keep alive)
            // 100 - 199: Network
            // 200 - 299: Game
            REQ_ENTITY_SPAWN = 200, // Client -> Server (Spawn entity)
            REP_ENTITY_SPAWN = 201, // Server -> Client (Entity spawned)
            REQ_SOUND_PLAY = 202, // Client -> Server (Play sound)
            REP_SOUND_PLAY = 203, // Server -> Client (Sound played)
            REQ_ENTITY_DESTROY = 204, // Client -> Server (Destroy entity)
            REP_ENTITY_DESTROY = 205, // Server -> Client (Entity destroyed)
            REQ_ENTITY_MOVED = 206, // Client -> Server (Move entity)
            REP_ENTITY_MOVED = 207, // Server -> Client (Entity moved)
            REQ_ENTITY_SHOOT = 208, // Client -> Server (Shoot entity)
            REP_ENTITY_SHOOT = 209, // Server -> Client (Entity shot)
            // 300 - 399: User
            // 400 - 499: Chat
            // 500 - 599: Matchmaking
            // 600 - 699: Leaderboard
            // 700 - 799: Achievement
            // 800 - 899: Inventory
            // 900 - 999: Store
            // 1000 - 1099: Party
            // 1100 - 1199: Tournament
            // 1200 - 1299: Telemetry
            // 1300 - 1399: Commerce
            // 1400 - 1499: Season
            // 1500 - 1599: PlayerData
            // 1600 - 1699: TitleData
            // 1700 - 1799: Snapshots
            // 1800 - 1899: CloudScript
            // 1900 - 1999: Server-Side CloudScript
            // 2000 - 2099: Shared Group Data
            // 2100 - 2199: Files
            // 2200 - 2299: Party
            // 2300 - 2399: Insights
            // 2400 - 2499: Messaging
            // 2500 - 2599: Multiplayer
            // 2600 - 2699: Server-Side CloudScript
            // 2700 - 2799: PlayStream
            // 2800 - 2899: Advertising
            // 2900 - 2999: Analytics
            // 3000 - 3099: Authentication
            // 3100 - 3199: Profiles
            // 3200 - 3299: Push Notifications
            // 3300 - 3399: Shared Group Data
            // 3400 - 3499: Title-Wide Data Management
            // 3500 - 3599: Characters
            // 3600 - 3699: Friend List
            // 3700 - 3799: Matchmaking
            // 3800 - 3899: Platform Specific
            // 3900 - 3999: Player Data Management
            // 4000 - 4099: Player Item Management
            // 4100 - 4199: Player Profile
            // 4200 - 4299: Title Data Management
            // 4300 - 4399: Title Internal
            // 4400 - 4499: Virtual Currency
            // 4500 - 4599: Player Data Management
            // 4600 - 4699: Player Item Management
        };

    } /* namespace V_1 */

} // namespace Flakkari::Protocol::API

#endif /* !EVENT_HPP_ */
