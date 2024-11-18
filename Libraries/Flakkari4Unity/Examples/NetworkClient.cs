using System;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

using Flk_API = Flakkari4Unity.API;
using CurrentProtocol = Flakkari4Unity.Protocol.V1;
using System.Collections.Generic;

public class NetworkClient : MonoBehaviour
{
    private UdpClient udpClient;
    private IPEndPoint serverEndpoint;
    private readonly float keepAliveInterval = 3;
    [SerializeField] private string serverIP = "127.0.0.1";
    [SerializeField] private int serverPort = 54000;
    [SerializeField] private string gameName = "R-Type";
    [SerializeField] private bool enable;

    public NetworkClient(string serverIP, int serverPort, string gameName)
    {
        this.serverIP = serverIP;
        this.serverPort = serverPort;
        this.gameName = gameName;
        enable = true;
    }

    void Start()
    {
        if (!enable)
        {
            Destroy(this);
            return;
        }

        udpClient = new UdpClient();
        serverEndpoint = new IPEndPoint(IPAddress.Parse(serverIP), serverPort);

        udpClient.BeginReceive(OnReceive, null);

        byte[] packet = Flk_API.APIClient.ReqConnect(gameName);
        udpClient.Send(packet, packet.Length, serverEndpoint);
        InvokeRepeating(nameof(ReqKeepAlive), keepAliveInterval, keepAliveInterval);
    }

    public bool Enable
    {
        get => enable;
        set => enable = value;
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
            Flk_API.APIClient.Reply(receivedData, out List<CurrentProtocol.CommandId> commandId, out List<uint> sequenceNumber, out List<byte[]> payload);

            for (int i = 0; i < commandId.Count; i++)
            {
                switch (commandId[i])
                {
                    case CurrentProtocol.CommandId.REP_CONNECT:
                        Debug.Log("REP_CONNECT message received from the server.");
                        break;
                    case CurrentProtocol.CommandId.REP_HEARTBEAT:
                        Debug.Log("REP_HEARTBEAT message received from the server.");
                        break;
                    case CurrentProtocol.CommandId.REQ_ENTITY_SPAWN:
                        Debug.Log("REQ_ENTITY_SPAWN message received from the server.");
                        break;
                    case CurrentProtocol.CommandId.REQ_ENTITY_UPDATE:
                        Debug.Log("REQ_ENTITY_UPDATE message received from the server.");
                        break;
                    case CurrentProtocol.CommandId.REQ_ENTITY_DESTROY:
                        Debug.Log("REQ_ENTITY_DESTROY message received from the server.");
                        break;
                    case CurrentProtocol.CommandId.REP_USER_UPDATES:
                        Debug.Log("REP_USER_UPDATES message received from the server.");
                        break;
                    case CurrentProtocol.CommandId.REP_USER_UPDATE:
                        Debug.Log("REP_USER_UPDATE message received from the server.");
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
        Debug.Log("UDP client closed.");
    }
}
