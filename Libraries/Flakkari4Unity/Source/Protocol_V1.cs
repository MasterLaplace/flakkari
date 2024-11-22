using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

/// <summary>
/// The Protocol namespace contains classes and enumerations for handling
/// communication protocols in the Flakkari4Unity library.
/// </summary>
namespace Flakkari4Unity.Protocol
{
    /// <summary>
    /// Represents the API version.
    /// </summary>
    public enum ApiVersion : byte
    {
        /// <summary>
        /// Version 0.
        /// </summary>
        [Obsolete("Version 0 is deprecated.")]
        V_0 = 0,
        /// <summary>
        /// Version 1.
        /// </summary>
        V_1 = 1,
        /// <summary>
        /// Maximum version.
        /// </summary>
        MAX_VERSION
    }

    namespace V1
    {
        /// <summary>
        /// Represents the priority of a command.
        /// </summary>
        public enum Priority : byte
        {
            /// <summary>
            /// Low priority.
            /// </summary>
            LOW = 0,
            /// <summary>
            /// Medium priority.
            /// </summary>
            MEDIUM = 1,
            /// <summary>
            /// High priority.
            /// </summary>
            HIGH = 2,
            /// <summary>
            /// Critical priority.
            /// </summary>
            CRITICAL = 3,
            /// <summary>
            /// Maximum priority.
            /// </summary>
            MAX_PRIORITY = 4
        }

        /// <summary>
        /// Represents the command ID.
        /// </summary>
        public enum CommandId : byte
        {
            /// <summary>
            /// System command: Request to connect to the server.
            /// </summary>
            REQ_CONNECT = 0,
            /// <summary>
            /// System command: Response to connection request.
            /// </summary>
            REP_CONNECT = 1,
            /// <summary>
            /// System command: Request to disconnect from the server.
            /// </summary>
            REQ_DISCONNECT = 2,
            /// <summary>
            /// System command: Response to disconnection request.
            /// </summary>
            REP_DISCONNECT = 3,
            /// <summary>
            /// System command: Request to send a heartbeat to the server.
            /// </summary>
            REQ_HEARTBEAT = 4,
            /// <summary>
            /// System command: Response to heartbeat request.
            /// </summary>
            REP_HEARTBEAT = 5,

            /// <summary>
            /// Network command: Request to login.
            /// </summary>
            REQ_LOGIN = 10,
            /// <summary>
            /// Network command: Response to login request.
            /// </summary>
            REP_LOGIN = 11,
            /// <summary>
            /// Network command: Request to logout.
            /// </summary>
            REQ_LOGOUT = 12,
            /// <summary>
            /// Network command: Response to logout request.
            /// </summary>
            REP_LOGOUT = 13,
            /// <summary>
            /// Network command: Request to register.
            /// </summary>
            REQ_REGISTER = 14,
            /// <summary>
            /// Network command: Response to register request.
            /// </summary>
            REP_REGISTER = 15,

            /// <summary>
            /// Game command: Request to spawn an entity.
            /// </summary>
            REQ_ENTITY_SPAWN = 20,
            /// <summary>
            /// Game command: Response to entity spawn request.
            /// </summary>
            REP_ENTITY_SPAWN = 21,
            /// <summary>
            /// Game command: Request to update an entity.
            /// </summary>
            REQ_ENTITY_UPDATE = 22,
            /// <summary>
            /// Game command: Response to entity update request.
            /// </summary>
            REP_ENTITY_UPDATE = 23,
            /// <summary>
            /// Game command: Request to destroy an entity.
            /// </summary>
            REQ_ENTITY_DESTROY = 24,
            /// <summary>
            /// Game command: Response to entity destroy request.
            /// </summary>
            REP_ENTITY_DESTROY = 25,
            /// <summary>
            /// Game command: Request to move an entity.
            /// </summary>
            REQ_ENTITY_MOVED = 26,
            /// <summary>
            /// Game command: Response to entity move request.
            /// </summary>
            REP_ENTITY_MOVED = 27,
            /// <summary>
            /// Game command: Request to shoot an entity.
            /// </summary>
            REQ_ENTITY_SHOOT = 28,
            /// <summary>
            /// Game command: Response to entity shoot request.
            /// </summary>
            REP_ENTITY_SHOOT = 29,

            /// <summary>
            /// User command: Request to update a user.
            /// </summary>
            REQ_USER_UPDATE = 30,
            /// <summary>
            /// User command: Response to user update request.
            /// </summary>
            REP_USER_UPDATE = 31,
            /// <summary>
            /// User command: Request to update a user with a list of events.
            /// </summary>
            REQ_USER_UPDATES = 32,
            /// <summary>
            /// User command: Response to user update request with a list of events.
            /// </summary>
            REP_USER_UPDATES = 33,

            /// <summary>
            /// Matchmaking command: Request to create a room.
            /// </summary>
            REQ_CREATE_ROOM = 50,
            /// <summary>
            /// Matchmaking command: Response to room creation request.
            /// </summary>
            REP_CREATE_ROOM = 51,
            /// <summary>
            /// Matchmaking command: Request to join a room.
            /// </summary>
            REQ_JOIN_ROOM = 52,
            /// <summary>
            /// Matchmaking command: Response to room join request.
            /// </summary>
            REP_JOIN_ROOM = 53,
            /// <summary>
            /// Matchmaking command: Request to leave a room.
            /// </summary>
            REQ_LEAVE_ROOM = 54,
            /// <summary>
            /// Matchmaking command: Response to room leave request.
            /// </summary>
            REP_LEAVE_ROOM = 55,
            /// <summary>
            /// Matchmaking command: Request to start a game.
            /// </summary>
            REQ_START_GAME = 56,
            /// <summary>
            /// Matchmaking command: Response to game start request.
            /// </summary>
            REP_START_GAME = 57,
            /// <summary>
            /// Matchmaking command: Request to end a game.
            /// </summary>
            REQ_END_GAME = 58,
            /// <summary>
            /// Matchmaking command: Response to game end request.
            /// </summary>
            REP_END_GAME = 59,

            MAX_COMMAND_ID
        }

        /// <summary>
        /// Represents the component ID.
        /// </summary>
        enum ComponentId : byte
        {
            // 2D components
            [Obsolete("Component CONTROL is deprecated.")]
            CONTROL = 0,
            [Obsolete("Component MOVABLE is deprecated.")]
            MOVABLE = 1,
            [Obsolete("Component TRANSFORM is deprecated.")]
            TRANSFORM = 2,
            [Obsolete("Component COLLIDER is deprecated.")]
            COLLIDER = 3,
            [Obsolete("Component RIGIDBODY is deprecated.")]
            RIGIDBODY = 4,
            // 10 - 19: 3D components
            CONTROL_3D = 10,
            MOVABLE_3D = 11,
            TRANSFORM_3D = 12,
            BOXCOLLIDER = 13,
            SPHERECOLLIDER = 14,
            RIGIDBODY_3D = 15,
            // Common components
            CHILD = 20,
            EVOLVE = 21,
            HEALTH = 22,
            ID = 23,
            LEVEL = 24,
            PARENT = 25,
            TAG = 26,
            TEMPLATE = 27,
            TIMER = 28,
            WEAPON = 29,
            MAX_COMPONENT
        }

        /// <summary>
        /// Represents the event ID.
        /// </summary>
        public enum EventId : byte
        {
            MOVE_LEFT = 10,
            MOVE_RIGHT = 11,
            MOVE_UP = 12,
            MOVE_DOWN = 13,
            MOVE_FRONT = 14,
            MOVE_BACK = 15,
            LOOK_LEFT = 16,
            LOOK_RIGHT = 17,
            LOOK_UP = 18,
            LOOK_DOWN = 19,
            SHOOT = 20,
            MAX_EVENT
        }

        /// <summary>
        /// Represents the state of an event.
        /// </summary>
        public enum EventState : byte
        {
            PRESSED = 0,
            RELEASED = 1,
            MAX_STATE
        }

        /// <summary>
        /// Represents an event with an ID and state.
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct Event
        {
            public EventId id;
            public EventState state;

            /// <summary>
            /// Initializes a new instance of the <see cref="Event"/> struct.
            /// </summary>
            /// <param name="id">The event ID.</param>
            /// <param name="state">The event state.</param>
            /// <remarks>
            /// The event ID is the type of event, such as MOVE_LEFT or SHOOT.
            /// The event state is the state of the event, such as PRESSED or RELEASED.
            /// </remarks>
            public Event(EventId id, EventState state)
            {
                this.id = id;
                this.state = state;
            }

            /// <summary>
            /// Serializes the event to a byte array.
            /// </summary>
            /// <param name="_event">The event to serialize.</param>
            /// <returns>A byte array containing the serialized event.</returns>
            /// <remarks>
            /// The serialized event is a byte array of size 2, where the first byte
            /// contains the event ID and the second byte contains the event state.
            /// </remarks>
            public static byte[] Serialize(Event _event)
            {
                byte[] eventBytes = new byte[Marshal.SizeOf<Event>()];

                eventBytes[0] = (byte)_event.id;
                eventBytes[1] = (byte)_event.state;
                return eventBytes;
            }

            /// <summary>
            /// Serializes a list of events to a byte array.
            /// </summary>
            /// <param name="_event">The list of events to serialize.</param>
            /// <returns>A byte array containing the serialized events.</returns>
            /// <remarks>
            /// The serialized events is a byte array of size 2 * n, where n is the number of events in the list.
            /// Each event is serialized as a byte array of size 2, where the first byte contains the event ID
            /// and the second byte contains the event state.
            /// </remarks>
            public static byte[] Serialize(List<Event> _event)
            {
                byte[] eventBytes = new byte[Marshal.SizeOf<Event>() * _event.Count];

                for (int i = 0; i < _event.Count; i++)
                {
                    eventBytes[i * Marshal.SizeOf<Event>()] = (byte)_event[i].id;
                    eventBytes[i * Marshal.SizeOf<Event>() + 1] = (byte)_event[i].state;
                }
                return eventBytes;
            }

            public static byte[] Serialize(Dictionary<EventId, float> axisEvents)
            {
                byte[] eventBytes = new byte[axisEvents.Count * (sizeof(byte) + sizeof(float))];

                int i = 0;
                foreach (KeyValuePair<EventId, float> axisEvent in axisEvents)
                {
                    eventBytes[i * (sizeof(byte) + sizeof(float))] = (byte)axisEvent.Key;
                    BitConverter.GetBytes(axisEvent.Value).CopyTo(eventBytes, i * (sizeof(byte) + sizeof(float)) + sizeof(byte));
                    i++;
                }
                return eventBytes;
            }
        }

        /// <summary>
        /// Represents the header of a packet.
        /// </summary>
        /// <remarks>
        /// Flakkari Header v1 (new header)
        ///
        ///  0                   1                   2                   3
        ///  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        /// |Priority| Api V.|   CommandId   |       ContentLength          |
        /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        /// |                        SequenceNumber                         |
        /// |                                                               |
        /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        /// </remarks>
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct Header
        {
            public byte priorityAndVersion;  // 4 bits for priority, 4 bits for API version
            public CommandId commandId;
            public ushort contentLength;
            public ulong sequenceNumber;

            /// <summary>
            /// Initializes a new instance of the <see cref="Header"/> struct.
            /// </summary>
            /// <param name="priority">The priority of the command.</param>
            /// <param name="apiVersion">The API version.</param>
            /// <param name="commandId">The command ID.</param>
            /// <param name="contentLength">The length of the content.</param>
            public Header(Priority priority, ApiVersion apiVersion, CommandId commandId, ushort contentLength)
            {
                priorityAndVersion = (byte)(((int)apiVersion << 4) | ((int)priority & 0x0F));
                this.commandId = commandId;
                this.contentLength = contentLength;
                sequenceNumber = (ulong)DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
            }

            /// <summary>
            /// Deserializes the header from a byte array.
            /// </summary>
            /// <param name="packet">The byte array containing the packet data.</param>
            public void Deserialize(byte[] packet)
            {
                priorityAndVersion = packet[0];
                commandId = (CommandId)packet[1];
                contentLength = BitConverter.ToUInt16(packet, 2);
                sequenceNumber = BitConverter.ToUInt64(packet, 4);
            }

            /// <summary>
            /// Serializes the header to a byte array.
            /// </summary>
            /// <returns>A byte array containing the serialized header.</returns>
            public readonly byte[] Serialize()
            {
                byte[] headerBytes = new byte[Marshal.SizeOf<Header>()];
                headerBytes[0] = priorityAndVersion;
                headerBytes[1] = (byte)commandId;
                BitConverter.GetBytes(contentLength).CopyTo(headerBytes, 2);
                BitConverter.GetBytes(sequenceNumber).CopyTo(headerBytes, 4);
                return headerBytes;
            }
        }

        /// <summary>
        /// Represents a packet with serialization and deserialization methods.
        /// </summary>
        public class Packet
        {
            /// <summary>
            /// Serializes a packet with the specified parameters.
            /// </summary>
            /// <param name="priority">The priority of the command.</param>
            /// <param name="commandId">The command ID.</param>
            /// <param name="payload">The payload of the packet.</param>
            /// <returns>A byte array containing the serialized packet.</returns>
            public static byte[] Serialize(Priority priority, CommandId commandId, byte[] payload = null)
            {
                ushort contentLength = (ushort)(payload?.Length ?? 0);
                byte[] headerBytes = new Header(priority, ApiVersion.V_1, commandId, contentLength).Serialize();

                byte[] packet = new byte[headerBytes.Length + contentLength];
                Buffer.BlockCopy(headerBytes, 0, packet, 0, headerBytes.Length);

                if (payload != null)
                    Buffer.BlockCopy(payload, 0, packet, headerBytes.Length, contentLength);

                return packet;
            }

            /// <summary>
            /// Deserializes a packet from a byte array.
            /// </summary>
            /// <param name="packet">The byte array containing the packet data.</param>
            /// <param name="commandId">The command ID.</param>
            /// <param name="sequenceNumber">The sequence number.</param>
            /// <param name="payload">The payload of the packet.</param>
            public static void Deserialize(byte[] packet, out CommandId commandId, out ulong sequenceNumber, out byte[] payload)
            {
                if (packet.Length < Marshal.SizeOf<Header>())
                    throw new ArgumentException("Packet is too short to contain a header.");

                Header header = new();
                header.Deserialize(packet);

                if ((ApiVersion)(header.priorityAndVersion >> 4) != ApiVersion.V_1)
                    throw new ArgumentException("Unsupported API version: " + (ApiVersion)(header.priorityAndVersion >> 4));

                if ((Priority)(header.priorityAndVersion & 0x0) >= Priority.MAX_PRIORITY)
                    throw new ArgumentException("Priority is out of range.");

                commandId = header.commandId;
                if (commandId >= CommandId.MAX_COMMAND_ID)
                    throw new ArgumentException("Command ID is out of range.");

                sequenceNumber = header.sequenceNumber;
                if (header.contentLength > packet.Length - Marshal.SizeOf<Header>())
                    throw new ArgumentException("Content length is out of range : " + header.contentLength + " > " + packet.Length + " - " + Marshal.SizeOf<Header>());

                byte[] payloadBytes = new byte[header.contentLength];
                Buffer.BlockCopy(packet, Marshal.SizeOf<Header>(), payloadBytes, 0, header.contentLength);
                payload = payloadBytes;
            }
        }
    }
} // namespace Flakkari4Unity.Protocol
