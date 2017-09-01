# Usage

This library provides a class that reads and controls the MCP23008. The MCP23008.ino file serves as an example use case.

>Create an instance of the class
```cpp
MCP23008 RelayController;
```

>Set the address if the address jumpers are used. The method accepts 3 arguments that are mapped to the address pins as printed on the board: A0, A1, A2
```cpp
RelayController.setAddress(1,0,0);
```

>Set the number of relays on the board, this method will call the `setOutputs` method for the first n channels.
```cpp
RelayController.setRelays(16);
```

>Set which inputs should be pulled up (if any)
>This method expects an int that is essentially a bit map, each bit represents a channel, the set bits indicate the channel should be pulled high.
```cpp
RelayController.setInputs(128);
```

>Initialize the controller
```cpp
RelayController.init();
```

>Basic relay control functions
```cpp
RelayController.turnOnRelay(1);
RelayController.turnOffRelay(1);
RelayController.toggleRelay(2);
RelayController.turnOnAllRelays();
RelayController.turnOffAllRelays();
```

>This class also uses a software timer for momentary commands, the first argument should be a relay number, the second is the number of milliseconds to leave the relay active.
>If the second argument is omitted it defaults to 100 milliseconds.
```cpp
RelayController.momentaryRelay(1, 500)
```

>Read statuses. Statuses are, again, bit maps. Set bits of outputs indicate that a relay is active, set bits of an input indicate the input is high/connected
```cpp
relayController.readStatus();
int relayStatus = relayController.bankStatus;
int inputStatus = relayController.readAllInputs();
```

>This library also features a "relayTalk" method. It allows some basic "human readable" phrases to control relays when sent as a string.

```cpp
RelayController.relayTalk("turn on relay 1");
RelayController.relayTalk("turn off relay 1");
RelayController.relayTalk("toggle relay 1");
RelayController.relayTalk("turn on all relays");
RelayController.relayTalk("turn on off all relays");
RelayController.relayTalk("Turn on relay 1 for 1 second);
```
