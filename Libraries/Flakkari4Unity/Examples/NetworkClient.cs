using System;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using UnityEngine;

using Flk_API = Flakkari4Unity.API;
using CurrentProtocol = Flakkari4Unity.Protocol.V1;

public class NetworkClient : MonoBehaviour
{
    private UdpClient udpClient;
    private IPEndPoint serverEndpoint;
    private readonly float keepAliveInterval = 3;
    private string serverIP;
    private int serverPort;
    private string gameName;
    private bool enable = false;

    public bool Enable
    {
        get => enable;
    }

    public void Create(string serverIP, int serverPort, string gameName)
    {
        this.serverIP = serverIP;
        this.serverPort = serverPort;
        this.gameName = gameName;
        enable = true;

        udpClient = new UdpClient();
        serverEndpoint = new IPEndPoint(IPAddress.Parse(serverIP), serverPort);

        udpClient.BeginReceive(OnReceive, null);

        byte[] packet = Flk_API.APIClient.ReqConnect(gameName);
        udpClient.Send(packet, packet.Length, serverEndpoint);
        InvokeRepeating(nameof(ReqKeepAlive), keepAliveInterval, keepAliveInterval);
    }

    internal void Send(byte[] packet)
    {
        if (udpClient != null)
        {
            udpClient.Send(packet, packet.Length, serverEndpoint);
        }
        else
        {
            Debug.LogError("UDP client is not initialized.");
        }
    }

    private void ReqKeepAlive()
    {
        byte[] packet = Flk_API.APIClient.ReqKeepAlive();
        udpClient.Send(packet, packet.Length, serverEndpoint);
    }

    private void OnReceive(IAsyncResult result)
    {
        if (!enable)
            return;

        try
        {
            byte[] receivedData = udpClient.EndReceive(result, ref serverEndpoint);
            Flk_API.APIClient.Reply(receivedData, out List<CurrentProtocol.CommandId> commandId, out List<ulong> sequenceNumber, out List<byte[]> payload);

            Flakkari4Unity.Synchronizer synchronizer = gameObject.GetComponent<Flakkari4Unity.Synchronizer>();

            for (int i = 0; i < commandId.Count; i++)
            {
                switch (commandId[i])
                {
                    case CurrentProtocol.CommandId.REP_CONNECT:
                        Flk_API.APIClient.ResConnect(payload[i], ref synchronizer, out ulong entityId);

                        Player playerScript = synchronizer.GetEntity(entityId).GetComponent<Player>();
                        playerScript.SetupCameraViewport(new Rect(0, 0, 1, 1));
                        playerScript.SetupNetworkClient(this);
                        break;

                    case CurrentProtocol.CommandId.REQ_ENTITY_SPAWN:
                        Flk_API.APIClient.ReqEntitySpawn(payload[i], ref synchronizer);
                        break;

                    case CurrentProtocol.CommandId.REQ_ENTITY_UPDATE:
                        Flk_API.APIClient.ReqEntityUpdate(payload[i], ref synchronizer);
                        break;

                    case CurrentProtocol.CommandId.REQ_ENTITY_DESTROY:
                        Flk_API.APIClient.ReqEntityDestroy(payload[i], ref synchronizer);
                        break;

                    case CurrentProtocol.CommandId.REQ_ENTITY_MOVED:
                        Flk_API.APIClient.ReqEntityMoved(payload[i], ref synchronizer);
                        break;

                    default:
                        Debug.LogWarning("Unknown command ID received from the server.");
                        break;
                }
            }
        }
        catch (Exception e)
        {
            Debug.LogError("Error in OnReceive: " + e.Message);
        }
        udpClient.BeginReceive(OnReceive, null);
    }

    private void OnApplicationQuit()
    {
        if (!enable)
            return;
        enable = false;

        udpClient.Close();
    }
}
