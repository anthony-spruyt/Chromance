# Instructions

## How to build/upload

This will cover how to get this codebase up and running on your Chromance by Zack Freedman.

1. [Install VSCode](https://code.visualstudio.com/)
1. Install the [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) extension in vscode
1. Open a terminal (of your choosing) and go to the directory you want to store git repos
1. Clone this repo locally `git clone https://github.com/anthony-spruyt/Chromance.git`
1. Change directory to the cloned repo `cd Chromance`
1. Change directory to the `chromance-firmware` folder `cd chromance-firmware`
1. Open this folder in VSCode `code .` or via the VSCode UI (File -> Open Folder)
1. You should verify the contents of platformio.ini, details in How to Setup, then you will open the platformio extension and run build then upload (choose USB for initial)

## How to Setup

### platformio.ini

The following might need to be changed depending on your setup/hardware.

- Change `board = esp32dev` to match the model of ESP32 you have, a list can be [found here](https://docs.platformio.org/en/latest/boards/index.html)
- Change `upload_port = 192.168.50.95` to match the static IP you setup in your router after setting up the WiFi.

### constants.h

- Open up [constants.h](src/constants.h) and familiarize yourself with the config

#### Optionally Edit the Following as needed

- `BlueStripDataPin`: With the Data Pin you are using for the blue
- `GreenStripDataPin`: With the Data Pin you are using for the green
- `RedStripDataPin`: With the Data Pin you are using for the red
- `BlackStripDataPin`: With the Data Pin you are using for the black

## How to connect to wifi

Create a file with the name `secrets.h` in the src folder

```c++
#ifndef SECRETS_H_
#define SECRETS_H_

namespace Chromance
{
    //////////////////////////////////////////
    // WiFi
    //////////////////////////////////////////

    static const char* WifiSsid = "myssid";
    static const char* WifiPassword = "mywifipassword";

    //////////////////////////////////////////
    // OTA
    //////////////////////////////////////////

    static const char* OTAPassword = "myotapassword";

    //////////////////////////////////////////
    // MQTT
    //////////////////////////////////////////

    static const char* MQTTBroker = "mqttbroker.local";
    static const int32_t MQTTPort = 1883;
    static const char* MQTTUsername = "chromance";
    static const char* MQTTPassword = "mymqttpassword";
}

#endif
```

## OTA Updates

Crete a file in the chromance-formware folder with the name `platformioSecrets.ini`

```ini
; applies to all environments
; or use "extends" (https://docs.platformio.org/en/latest/projectconf/section_env_advanced.html#extends)
[esp32dev-ota] 
upload_flags = 
 --port=3232
 --auth=myotapassword
```

## How to make an animation

To create your own animations you will want to look at the [map.h](src/animations/ripples/map.h) file and the [ripple.cpp](src/animations/ripples/ripple.cpp) file to a lesser extent.  This repository contains a [mapping.jpeg](mapping.jpeg) that shows each nodes number and the segment numbers.  You can use this image to make sense of the `NodeConnections`, `SegmentConnections`, `BorderNodes`, `CubeNodes`, `FunNodes`, and `StarBurstNode` variables in [`map.h`](src/animations/ripples/map.h)
