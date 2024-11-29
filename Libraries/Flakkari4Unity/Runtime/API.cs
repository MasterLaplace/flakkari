using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using UnityEngine;

using CurrentProtocol = Flakkari4Unity.Protocol.V1;

namespace Flakkari4Unity.API
{
    public class APIClient : MonoBehaviour
    {
        /// <summary>
        /// Creates a REQ_CONNECT message to connect to the server.
        /// </summary>
        /// <param name="gameName">The name of the game to connect to.</param>
        /// <returns>A byte array representing the serialized REQ_CONNECT message.</returns>
        public static byte[] ReqConnect(string gameName)
        {
            return CurrentProtocol.Packet.Serialize(
                CurrentProtocol.Priority.HIGH,
                CurrentProtocol.CommandId.REQ_CONNECT,
                System.Text.Encoding.UTF8.GetBytes(gameName)
            );
        }

        /// <summary>
        /// Processes the RES_CONNECT message and extracts the entity ID and template name.
        /// </summary>
        /// <param name="payload">The byte array containing the payload of the RES_CONNECT message.</param>
        /// <param name="entityId">The extracted entity ID from the payload.</param>
        /// <param name="templateName">The extracted template name from the payload.</param>
        /// <returns>The remaining payload after extracting the entity ID and template name.</returns>
        public static byte[] ResConnect(byte[] payload, out ulong entityId, out string templateName)
        {
            entityId = BitConverter.ToUInt64(payload, 0);
            int index = sizeof(ulong);

            uint length = BitConverter.ToUInt32(payload, index);
            index += sizeof(uint);
            templateName = System.Text.Encoding.UTF8.GetString(payload, index, (int)length);
            index += (int)length;
            return payload.Skip(index).ToArray();
        }

        /// <summary>
        /// Creates a REQ_DISCONNECT message to disconnect from the server.
        /// </summary>
        /// <returns>A byte array representing the serialized REQ_DISCONNECT message.</returns>
        public static byte[] ReqDisconnect()
        {
            return CurrentProtocol.Packet.Serialize(
                CurrentProtocol.Priority.HIGH,
                CurrentProtocol.CommandId.REQ_DISCONNECT
            );
        }

        /// <summary>
        /// Creates a REQ_HEARTBEAT message to keep the connection alive.
        /// </summary>
        /// <returns>A byte array representing the serialized REQ_HEARTBEAT message.</returns>
        /// <remarks>
        /// This message is sent to the server at regular intervals to keep the connection alive.
        /// </remarks>
        public static byte[] ReqKeepAlive()
        {
            // Debug.Log("REQ_HEARTBEAT message sent to the server.");
            return CurrentProtocol.Packet.Serialize(
                CurrentProtocol.Priority.LOW,
                CurrentProtocol.CommandId.REQ_HEARTBEAT
            );
        }

        public static byte[] ReqEntitySpawn(byte[] payload, out ulong entityId, out string templateName)
        {
            entityId = BitConverter.ToUInt64(payload, 0);
            int index = sizeof(ulong);

            uint length = BitConverter.ToUInt32(payload, index);
            index += sizeof(uint);
            templateName = System.Text.Encoding.UTF8.GetString(payload, index, (int)length);
            index += (int)length;
            return payload.Skip(index).ToArray();
        }

        public static void ReqEntityUpdate(byte[] payload, ref Synchronizer synchronizer)
        {
            ulong entityId = BitConverter.ToUInt64(payload, 0);
            synchronizer.GetEntity(entityId).UpdateComponents(payload.Skip(sizeof(ulong)).ToArray());
        }

        public static void ReqEntityDestroy(byte[] payload, ref Synchronizer synchronizer)
        {
            ulong entityId = BitConverter.ToUInt64(payload, 0);
            synchronizer.RemoveEntity(entityId);
        }

        public static void ReqEntityMoved(byte[] payload, ref Synchronizer synchronizer)
        {
            ulong entityId = BitConverter.ToUInt64(payload, 0);
            int i = sizeof(ulong);

            ECS.Entity entity = synchronizer.GetEntity(entityId);
            Transform transform = entity.transform;

            transform.position = new Vector3(BitConverter.ToSingle(payload, i), BitConverter.ToSingle(payload, i + sizeof(float)), BitConverter.ToSingle(payload, i + sizeof(float) * 2));
            i += sizeof(float) * 3;
            transform.rotation = new Quaternion(BitConverter.ToSingle(payload, i), BitConverter.ToSingle(payload, i + sizeof(float)), BitConverter.ToSingle(payload, i + sizeof(float) * 2), BitConverter.ToSingle(payload, i + sizeof(float) * 3));
            i += sizeof(float) * 4;
            transform.localScale = new Vector3(BitConverter.ToSingle(payload, i), BitConverter.ToSingle(payload, i + sizeof(float)), BitConverter.ToSingle(payload, i + sizeof(float) * 2));
            i += sizeof(float) * 3;

            ECS.Components._3D.Movable movable = entity.GetComponent<ECS.Components._3D.Movable>();

            movable.velocity = new Vector3(BitConverter.ToSingle(payload, i), BitConverter.ToSingle(payload, i + sizeof(float)), BitConverter.ToSingle(payload, i + sizeof(float) * 2));
            i += sizeof(float) * 3;
            movable.acceleration = new Vector3(BitConverter.ToSingle(payload, i), BitConverter.ToSingle(payload, i + sizeof(float)), BitConverter.ToSingle(payload, i + sizeof(float) * 2));
            i += sizeof(float) * 3;
            movable.minSpeed = BitConverter.ToSingle(payload, i);
            i += sizeof(float);
            movable.maxSpeed = BitConverter.ToSingle(payload, i);
        }

        public static byte[] ReqUserUpdates(List<CurrentProtocol.Event> events, Dictionary<CurrentProtocol.EventId, float> axisEvents)
        {
            byte[] eventCountBytes = BitConverter.GetBytes((ushort)events.Count);
            byte[] serializedEvents = CurrentProtocol.Event.Serialize(events);
            byte[] axisEventCountBytes = BitConverter.GetBytes((ushort)axisEvents.Count);
            byte[] serializedAxisEvents = CurrentProtocol.Event.Serialize(axisEvents);

            byte[] payload = ConcatByteArrays(eventCountBytes, serializedEvents);
            payload = ConcatByteArrays(payload, axisEventCountBytes);
            payload = ConcatByteArrays(payload, serializedAxisEvents);

            return CurrentProtocol.Packet.Serialize(
                CurrentProtocol.Priority.HIGH,
                CurrentProtocol.CommandId.REQ_USER_UPDATES,
                payload
            );
        }

        /// <summary>
        /// Processes the received data and extracts the command ID, sequence number, and message.
        /// </summary>
        /// <param name="receivedData">The byte array containing the received data.</param>
        /// <param name="commandIds">The extracted command IDs from the received data.</param>
        /// <param name="sequenceNumbers">The extracted sequence numbers from the received data.</param>
        /// <param name="payloads">The extracted payloads from the received data.</param>
        public static void Reply(byte[] receivedData, out List<CurrentProtocol.CommandId> commandIds, out List<ulong> sequenceNumbers, out List<byte[]> payloads)
        {
            commandIds = new List<CurrentProtocol.CommandId>();
            sequenceNumbers = new List<ulong>();
            payloads = new List<byte[]>();

            while (receivedData.Length > 0)
            {
                CurrentProtocol.Packet.Deserialize(receivedData, out CurrentProtocol.CommandId commandId, out ulong sequenceNumber, out byte[] payload);

                commandIds.Add(commandId);
                sequenceNumbers.Add(sequenceNumber);
                payloads.Add(payload);

                int headerSize = Marshal.SizeOf<CurrentProtocol.Header>();
                receivedData = receivedData.Skip(headerSize + (payload?.Length ?? 0)).ToArray();
            }
        }

        private static byte[] ConcatByteArrays(byte[] first, byte[] second)
        {
            byte[] result = new byte[first.Length + second.Length];
            Buffer.BlockCopy(first, 0, result, 0, first.Length);
            Buffer.BlockCopy(second, 0, result, first.Length, second.Length);
            return result;
        }
    }
} // namespace Flakkari4Unity.API
