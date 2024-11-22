# Flakkari4Unity

Flakkari4Unity is a Unity package that provides a simple way to use Flakkari Protocol in Unity projects to communicate with Flakkari server.

## Usage

```csharp
using Flakkari4Unity;

public class Example : MonoBehaviour
{
    private Flakkari4Unity flakkari;
    [SerializeField] private string serverIP = "127.0.0.1";
    [SerializeField] private int serverPort = 54000;
    [SerializeField] private string gameName = "SpaceWar";

    private readonly Dictionary<CurrentProtocol.EventId, CurrentProtocol.EventState> axisEvents = new(4);

    void Start()
    {
        NetworkClient networkClient = gameObject.AddComponent<NetworkClient>();
        networkClient.Create(serverIP, serverPort, gameName);
    }

    public void FixedUpdate()
    {
        if (networkClient == null && networkClient.Enable)
            return;

        List<CurrentProtocol.Event> events = new(8);
        Dictionary<CurrentProtocol.EventId, float> axisValues = new(4);

        Net_HandleMovement(networkClient, ref events, ref axisValues);
        Net_HandleShooting(networkClient, ref events);

        if (events.Count > 0 || axisValues.Count > 0)
            networkClient.Send(Flk_API.APIClient.ReqUserUpdates(events, axisValues));
    }

    private void Net_HandleMovement(NetworkClient networkClient, ref List<CurrentProtocol.Event> events, ref Dictionary<CurrentProtocol.EventId, float> axisValues)
    {
        HandleNetworkInput("Fire2", CurrentProtocol.EventId.MOVE_FRONT, ref events);
        HandleNetworkInput("Fire1", CurrentProtocol.EventId.MOVE_BACK, ref events);

        HandleMouseMovement("Horizontal", CurrentProtocol.EventId.LOOK_LEFT, CurrentProtocol.EventId.LOOK_RIGHT, ref axisValues);
        HandleMouseMovement("Vertical", CurrentProtocol.EventId.LOOK_DOWN, CurrentProtocol.EventId.LOOK_UP, ref axisValues);
    }

    private void HandleNetworkInput(string inputName, CurrentProtocol.EventId eventId, ref List<CurrentProtocol.Event> events)
    {
        if (Input.GetButtonDown(inputName))
            events.Add(new CurrentProtocol.Event { id = eventId, state = CurrentProtocol.EventState.PRESSED });

        else if (Input.GetButtonUp(inputName))
            events.Add(new CurrentProtocol.Event { id = eventId, state = CurrentProtocol.EventState.RELEASED });

    }

    private void HandleMouseMovement(string axisName, CurrentProtocol.EventId negativeEventId, CurrentProtocol.EventId positiveEventId , ref Dictionary<CurrentProtocol.EventId, float> axisValues)
    {
        float axisValue = Input.GetAxis(axisName);

        if (axisValue < 0 && axisEvents[negativeEventId] != CurrentProtocol.EventState.PRESSED)
        {
            axisEvents[negativeEventId] = CurrentProtocol.EventState.PRESSED;
            axisEvents[positiveEventId] = CurrentProtocol.EventState.RELEASED;
            axisValues[negativeEventId] = axisValue;
            axisValues[positiveEventId] = 0;
        }
        else if (axisValue > 0 && axisEvents[positiveEventId] != CurrentProtocol.EventState.PRESSED)
        {
            axisEvents[positiveEventId] = CurrentProtocol.EventState.PRESSED;
            axisEvents[negativeEventId] = CurrentProtocol.EventState.RELEASED;
            axisValues[positiveEventId] = axisValue;
            axisValues[negativeEventId] = 0;
        }

        if (axisValue == 0 && axisEvents[negativeEventId] == CurrentProtocol.EventState.PRESSED)
        {
            axisEvents[negativeEventId] = CurrentProtocol.EventState.RELEASED;
            axisValues[negativeEventId] = 0;
        }
        if (axisValue == 0 && axisEvents[positiveEventId] == CurrentProtocol.EventState.PRESSED)
        {
            axisEvents[positiveEventId] = CurrentProtocol.EventState.RELEASED;
            axisValues[positiveEventId] = 0;
        }
    }

    private void Net_HandleShooting(NetworkClient networkClient, ref List<CurrentProtocol.Event> events)
    {
        if (Time.time - lastShotTime < cooldown)
            return;

        HandleNetworkInput(KeyCode.Joystick1Button5, CurrentProtocol.EventId.SHOOT, ref events);
    }
}
```

## License

Flakkari4Unity is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

## Contact

If you have any questions or feedback, please open an issue on the [GitHub repository](https://github.com/MasterLaplace/Flakkari/issues/new/choose).
