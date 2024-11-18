using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Flakkari4Unity.Protocol.V0
{
    public enum ApiVersion : byte
    {
        V_0 = 0,
        MAX_VERSION
    }

    public enum Priority : byte
    {
        LOW = 0,
        MEDIUM = 1,
        HIGH = 2,
        CRITICAL = 3,
        MAX_PRIORITY = 4
    }

    public enum CommandId : byte
    {
        // 0 - 9: System
        REQ_CONNECT = 0,    // Client -> Server [Connect to server]: (username, password game)
        REP_CONNECT = 1,    // Server -> Client [Connection accepted]: (user_id)
        REQ_DISCONNECT = 2, // Client -> Server [Disconnect from server]: (user_id)
        REP_DISCONNECT = 3, // Server -> Client [Disconnect accepted]: ()
        REQ_PING = 4,       // Client -> Server [Ping server]: (user_id)
        REP_PING = 5,       // Server -> Client [Ping accepted]: ()
        REQ_PONG = 6,       // Client -> Server [Pong server]: (user_id)
        REP_PONG = 7,       // Server -> Client [Pong accepted]: ()
        REQ_HEARTBEAT = 8,  // Client -> Server [Heartbeat server) (Keep alive)]: (user_id)
        REP_HEARTBEAT = 9,  // Server -> Client [Heartbeat accepted) (Keep alive)]: ()
                            // 10 - 19: Network
        REQ_LOGIN = 10,    // Client -> Server [Login]: (username, password game)
        REP_LOGIN = 11,    // Server -> Client [Login accepted]: ()
        REQ_LOGOUT = 12,   // Client -> Server [Logout]: (user_id)
        REP_LOGOUT = 13,   // Server -> Client [Logout accepted]: ()
        REQ_REGISTER = 14, // Client -> Server [Register]: (user_id, username, password)
        REP_REGISTER = 15, // Server -> Client [Register accepted]: ()
                           // 20 - 29: Game
        REQ_ENTITY_SPAWN = 20,   // Server -> Client [Spawn entity]: (id)(component (position, rotation, velocity, etc))
        REP_ENTITY_SPAWN = 21,   // Client -> Server [Entity spawned]: ()
        REQ_ENTITY_UPDATE = 22,  // Server -> Client [Update entity]: (id)(component (position, rotation, velocity, etc))
        REP_ENTITY_UPDATE = 23,  // Client -> Server [Entity updated]: ()
        REQ_ENTITY_DESTROY = 24, // Server -> Client [Destroy entity]: (id)
        REP_ENTITY_DESTROY = 25, // Client -> Server [Entity destroyed]: ()
        REQ_ENTITY_MOVED = 26,   // Server -> Client [Move entity]: (id)(component (position, rotation, velocity, etc))
        REP_ENTITY_MOVED = 27,   // Client -> Server [Entity moved]: ()
        REQ_ENTITY_SHOOT = 28,   // Server -> Client [Shoot entity]: (id)(component (position, rotation, velocity, etc))
        REP_ENTITY_SHOOT = 29,   // Client -> Server [Entity shot]: ()
                                 // 30 - 39: User
        REQ_USER_UPDATE = 30, // Client -> Server [Update user]: (event_id, state)
        REP_USER_UPDATE = 31, // Server -> Client [User updated]: ()
        REQ_CREATE_ROOM = 50, // Client -> Server [Create room]: (user_id)
        REP_CREATE_ROOM = 51, // Server -> Client [Room created]: (room_id)
        REQ_JOIN_ROOM = 52,   // Client -> Server [Join room]: (user_id, room_id)
        REP_JOIN_ROOM = 53,   // Server -> Client [Room joined]: ()
        REQ_LEAVE_ROOM = 54,  // Client -> Server [Leave room]: (user_id)
        REP_LEAVE_ROOM = 55,  // Server -> Client [Room left]: ()
        REQ_START_GAME = 56,  // Client -> Server [Start game]: (user_id)
        REP_START_GAME = 57,  // Server -> Client [Game started]: ()
        REQ_END_GAME = 58,    // Client -> Server [End game]: (user_id)
        REP_END_GAME = 59,    // Server -> Client [Game ended]: ()
        MAX_COMMAND_ID
    }

    enum ComponentId : byte
    {
        // 0 - 9: 2D components
        CONTROL = 0,
        MOVABLE = 1,
        TRANSFORM = 2,
        COLLIDER = 3,
        RIGIDBODY = 4,
        // 10 - 19: Common components
        CHILD = 10,
        PARENT = 11,
        TAG = 12,
        SPAWNED = 13,
        TEMPLATE = 14,
        WEAPON = 15,
        LEVEL = 16,
        EVOLVE = 17,
        HEALTH = 18,
        // 20 - 29: Network components
        NETWORK_EVENT = 20,
        NETWORK_IP = 21,
        MAX_COMPONENT
    }

    public enum EventId : byte
    {
        MOVE_LEFT = 10,
        MOVE_RIGHT = 11,
        MOVE_UP = 12,
        MOVE_DOWN = 13,
        SHOOT = 14,
        MAX_EVENT
    }

    public enum EventState : byte
    {
        NONE = 0,
        PRESSED = 1,
        RELEASED = 2,
        MAX_STATE
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Event
    {
        public EventId id;
        public EventState state;
    }

    [Obsolete("This Header struct is deprecated, use the one in V1 namespace instead.")]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Header
    {
        public byte priorityAndVersion;  // 4 bits pour la priorité, 4 bits pour l'API version
        public CommandId commandId;
        public ushort contentLength;
        public uint sequenceNumber;

        public Header(Priority priority, ApiVersion apiVersion, CommandId commandId, ushort contentLength)
        {
            priorityAndVersion = (byte)(((int)priority << 4) | (int)apiVersion);
            this.commandId = commandId;
            this.contentLength = contentLength;
            sequenceNumber = (uint)DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
        }

        public void Deserialize(byte[] packet)
        {
            priorityAndVersion = packet[0];
            commandId = (CommandId)packet[1];
            contentLength = BitConverter.ToUInt16(packet, 2);
            sequenceNumber = BitConverter.ToUInt32(packet, 4);
        }

        public readonly byte[] Serialize()
        {
            byte[] packet = new byte[Marshal.SizeOf(this)];
            IntPtr headerPtr = Marshal.AllocHGlobal(packet.Length);
            Marshal.StructureToPtr(this, headerPtr, true);
            Marshal.Copy(headerPtr, packet, 0, packet.Length);
            Marshal.FreeHGlobal(headerPtr);
            return packet;
        }
    }

    [Obsolete("This Packet class is deprecated, use the one in V1 namespace instead.")]
    public class Packet
    {
        public static byte[] Serialize(Priority priority, ApiVersion apiVersion, CommandId commandId, string payload)
        {
            byte[] payloadBytes = Encoding.ASCII.GetBytes(payload);
            ushort contentLength = (ushort)payloadBytes.Length;

            Header header = new(priority, apiVersion, commandId, contentLength);

            byte[] headerBytes = header.Serialize();

            byte[] packet = new byte[headerBytes.Length + payloadBytes.Length];
            Buffer.BlockCopy(headerBytes, 0, packet, 0, headerBytes.Length);
            Buffer.BlockCopy(payloadBytes, 0, packet, headerBytes.Length, payloadBytes.Length);

            return packet;
        }

        public static void Deserialize(byte[] packet, out Priority priority, out ApiVersion apiVersion, out CommandId commandId, out uint sequenceNumber, out string payload)
        {
            Header header = new();
            header.Deserialize(packet);

            priority = (Priority)(header.priorityAndVersion >> 4);
            apiVersion = (ApiVersion)(header.priorityAndVersion & 0x0F);
            commandId = header.commandId;
            sequenceNumber = header.sequenceNumber;

            byte[] payloadBytes = new byte[header.contentLength];
            Buffer.BlockCopy(packet, Marshal.SizeOf<Header>(), payloadBytes, 0, header.contentLength);
            payload = Encoding.ASCII.GetString(payloadBytes);
        }
    }

} // namespace Flakkari4Unity.Protocol.V0
