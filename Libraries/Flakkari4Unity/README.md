# Flakkari4Unity

Flakkari4Unity is a Unity package that provides a simple way to use Flakkari Protocol in Unity projects to communicate with Flakkari server.

## Usage

```csharp
using Flk_API = Flakkari4Unity.API;
using CurrentProtocol = Flakkari4Unity.Protocol.V1;

public class Player : Flakkari4Unity.ECS.Entity
{
    [HideInInspector] private NetworkClient networkClient = null;
    private readonly Dictionary<CurrentProtocol.EventId, CurrentProtocol.EventState> axisEvents = new(4);

    public override void Start()
    {
        networkClient = gameObject.AddComponent<NetworkClient>();
        networkClient.Create("127.0.0.1", 54000, "GameName");
    }

    public override void Update()
    {
        List<CurrentProtocol.Event> events = new(8);
        Dictionary<CurrentProtocol.EventId, float> axisValues = new(4);

        Net_HandleMovement(ref events, ref axisValues);

        if (events.Count > 0 || axisValues.Count > 0)
            networkClient.Send(Flk_API.APIClient.ReqUserUpdates(events, axisValues));
    }

    private void Net_HandleMovement(ref List<CurrentProtocol.Event> events, ref Dictionary<CurrentProtocol.EventId, float> axisValues)
    {
        HandleNetworkInput(KeyCode.w, CurrentProtocol.EventId.MOVE_FRONT, ref events);
        HandleNetworkInput(KeyCode.s, CurrentProtocol.EventId.MOVE_BACK, ref events);

        HandleMouseMovement("Mouse X", CurrentProtocol.EventId.LOOK_LEFT, CurrentProtocol.EventId.LOOK_RIGHT, ref axisValues);
        HandleMouseMovement("Mouse Y", CurrentProtocol.EventId.LOOK_DOWN, CurrentProtocol.EventId.LOOK_UP, ref axisValues);
        HandleMouseMovement("Horizontal", CurrentProtocol.EventId.LOOK_LEFT, CurrentProtocol.EventId.LOOK_RIGHT, ref axisValues);
        HandleMouseMovement("Vertical", CurrentProtocol.EventId.LOOK_DOWN, CurrentProtocol.EventId.LOOK_UP, ref axisValues);
    }

    private void HandleNetworkInput(KeyCode keyCode, CurrentProtocol.EventId eventId, ref List<CurrentProtocol.Event> events)
    {
        if (Input.GetKeyDown(keyCode))
            events.Add(new CurrentProtocol.Event { id = eventId, state = CurrentProtocol.EventState.PRESSED });

        else if (Input.GetKeyUp(keyCode))
            events.Add(new CurrentProtocol.Event { id = eventId, state = CurrentProtocol.EventState.RELEASED });
    }

    private void HandleNetworkMouseInput(int keyCode, CurrentProtocol.EventId eventId, ref List<CurrentProtocol.Event> events)
    {
        if (Input.GetMouseButtonDown(keyCode))
            events.Add(new CurrentProtocol.Event { id = eventId, state = CurrentProtocol.EventState.PRESSED });

        else if (Input.GetMouseButtonUp(keyCode))
            events.Add(new CurrentProtocol.Event { id = eventId, state = CurrentProtocol.EventState.RELEASED });
    }

    private void HandleMouseMovement(string axisName, CurrentProtocol.EventId negativeEventId, CurrentProtocol.EventId positiveEventId, ref Dictionary<CurrentProtocol.EventId, float> axisValues)
    {
        float axisValue = Input.GetAxis(axisName);

        if (axisValue != 0)
            axisValues[positiveEventId] = axisValue;
    }
}
```

## License

Flakkari4Unity is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

## Contact

If you have any questions or feedback, please open an issue on the [GitHub repository](https://github.com/MasterLaplace/Flakkari/issues/new/choose).
