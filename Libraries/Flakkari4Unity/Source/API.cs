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
            Debug.Log("REQ_CONNECT message sent to the server.");
            return CurrentProtocol.Packet.Serialize(
                CurrentProtocol.Priority.HIGH,
                CurrentProtocol.CommandId.REQ_CONNECT,
                System.Text.Encoding.UTF8.GetBytes(gameName)
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

        public static byte[] ReqUserUpdates(List<CurrentProtocol.Event> events, Dictionary<CurrentProtocol.EventId, float> axisEvents)
        {
            byte[] eventCountBytes = BitConverter.GetBytes((ushort)events.Count);
            byte[] serializedEvents = CurrentProtocol.Event.Serialize(events);
            byte[] axisEventCountBytes = BitConverter.GetBytes((ushort)axisEvents.Count);
            byte[] serializedAxisEvents = CurrentProtocol.Event.Serialize(axisEvents);

            byte[] payload = ConcatByteArrays(eventCountBytes, serializedEvents);
            payload = ConcatByteArrays(payload, axisEventCountBytes);
            payload = ConcatByteArrays(payload, serializedAxisEvents);

            Debug.Log("REQ_USER_UPDATE message sent to the server.");
            return CurrentProtocol.Packet.Serialize(
                CurrentProtocol.Priority.HIGH,
                CurrentProtocol.CommandId.REQ_USER_UPDATES,
                payload
            );
        }

        public static byte[] ReqUserUpdate(CurrentProtocol.EventId id, CurrentProtocol.EventState state)
        {
            CurrentProtocol.Event _event = new()
            {
                id = id,
                state = state
            };

            Debug.Log("REQ_USER_UPDATE message sent to the server.");
            return CurrentProtocol.Packet.Serialize(
                CurrentProtocol.Priority.HIGH,
                CurrentProtocol.CommandId.REQ_USER_UPDATE,
                CurrentProtocol.Event.Serialize(_event)
            );
        }

        /// <summary>
        /// Processes the received data and extracts the command ID, sequence number, and message.
        /// </summary>
        /// <param name="receivedData">The byte array containing the received data.</param>
        /// <param name="commandIds">The extracted command IDs from the received data.</param>
        /// <param name="sequenceNumbers">The extracted sequence numbers from the received data.</param>
        /// <param name="payloads">The extracted payloads from the received data.</param>
        public static void Reply(byte[] receivedData, out List<CurrentProtocol.CommandId> commandIds, out List<uint> sequenceNumbers, out List<byte[]> payloads)
        {
            commandIds = new List<CurrentProtocol.CommandId>();
            sequenceNumbers = new List<uint>();
            payloads = new List<byte[]>();

            while (receivedData.Length > 0)
            {
                CurrentProtocol.Packet.Deserialize(receivedData, out CurrentProtocol.CommandId commandId, out uint sequenceNumber, out byte[] payload);

                Debug.Log($"[RECEIVED] CommandId: {commandId}, delay: {sequenceNumber - (uint)DateTimeOffset.UtcNow.ToUnixTimeMilliseconds()} ms, payload: {payload?.Length} bytes");

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
