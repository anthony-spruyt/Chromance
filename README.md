# Chromance
Big, bold rainbow-glowing wall art that features the king of shapes - the hexagon. 
This project was designed to work with an EmotiBit wristband to display biometric data, but it doesn't need one!
A gorgeous animated ripple effect will automatically kick in after just a few seconds.

Project video: https://www.youtube.com/watch?v=g6n8XLmZ__I

Instructions: https://voidstar.dozuki.com/Guide/Chromance+Assembly+Instructions/6
([PDF in the repo](hardware/Chromance%20Assembly%20Guide.pdf) may be out of date)

The following information is for my configuration. Feel free to build your own - the code is very easy to edit to accommodate yours

Bill of materials:
* 10m LED channel w/ white diffuser, pref. black - https://amzn.to/3nFEC3w
* 10m DotStar LED tape, 60 pixels/m - https://www.adafruit.com/product/2239?length=4
	* OR: 10m WS2812B (NeoPixel) LED tape, 60 pixels/m - https://amzn.to/3gTGzYS
* 1x Node32S-style ESP32 module - https://amzn.to/2QO9brO
* 2.54mm headers - https://amzn.to/3ujX4S0
* 1x 8-position 2.54mm-pitch terminal - https://amzn.to/3uerfKd
* 1x 2-position 5mm-pitch terminal - https://amzn.to/3gU5WJV
* 1x 2.1mm barrel jack, PCB mount - https://amzn.to/33bmpRS
* Stripboard - https://amzn.to/3aXs2Yc
* Epoxy (I like JB Kwik-Weld) - https://amzn.to/3xHL1jp
* 5V PSU w/ 2.1mm barrel plug 5A or higher - https://amzn.to/2RkIL0A
* 1/2" wide double stick tape, at least 3m - https://amzn.to/3gT8mst
* 3D printer filament - I used carbon-fiber PLA - https://amzn.to/3xHIwh6
* Lots of wire, solder, and flux
* Hot glue

**These affiliate links support future projects and the videos about them!**

3D print list (STLs in [`hardware/STL's`](hardware/STL's), Fusion 360 sources in [`hardware/Models`](hardware/Models)):
- 1x TopLeftNode.stl
- 1x TopRightNode.stl
- 3x TopCenterNode.stl
- 20x RegularNode.stl

## Firmware

### How to build/upload

This will cover how to get this codebase up and running on your Chromance by Zack Freedman.

1. [Install VSCode](https://code.visualstudio.com/)
1. Install the [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) extension in vscode
1. Open a terminal (of your choosing) and go to the directory you want to store git repos
1. Clone this repo locally `git clone https://github.com/anthony-spruyt/Chromance.git`
1. Change directory to the cloned repo `cd Chromance`
1. Open this folder in VSCode `code .` or via the VSCode UI (File -> Open Folder)
1. You should verify the contents of platformio.ini, details in How to Setup, then you will open the platformio extension and run build then upload (choose USB for initial)

### How to Setup

#### platformio.ini

The following might need to be changed depending on your setup/hardware.

- Change `board = esp32dev` to match the model of ESP32 you have, a list can be [found here](https://docs.platformio.org/en/latest/boards/index.html)
- OTA uploads need the Chromance's IP, which goes in `platformioSecrets.ini` (see OTA Updates). Give it a static IP / DHCP reservation in your router after setting up the WiFi. The device advertises `chromance.local` over mDNS, but that only resolves on Windows/macOS, not in WSL or a dev container. Use the IP, or add a DNS A record for it on your DNS server.

#### constants.h

- Open up [constants.h](src/constants.h) and familiarize yourself with the config

##### Optionally Edit the Following as needed

- `BlueStripDataPin`: With the Data Pin you are using for the blue
- `GreenStripDataPin`: With the Data Pin you are using for the green
- `RedStripDataPin`: With the Data Pin you are using for the red
- `BlackStripDataPin`: With the Data Pin you are using for the black

### How to connect to wifi

Create a file with the name `secrets.h` in the src folder

```c++
#ifndef SECRETS_H_
#define SECRETS_H_

namespace Chromance
{
    //////////////////////////////////////////
    // WiFi
    //////////////////////////////////////////

    constexpr const char* WifiSsid = "myssid";
    constexpr const char* WifiPassword = "mywifipassword";

    //////////////////////////////////////////
    // OTA
    //////////////////////////////////////////

    constexpr const char* OTAPassword = "myotapassword";

    //////////////////////////////////////////
    // MQTT
    //////////////////////////////////////////

    constexpr const char* MQTTBroker = "mqttbroker.local";
    constexpr int32_t MQTTPort = 1883;
    constexpr const char* MQTTUsername = "chromance";
    constexpr const char* MQTTPassword = "mymqttpassword";
}

#endif
```

### OTA Updates

The first upload must be over USB (`esp32dev-usb`). After that, update over WiFi.

Create a file in the repo root with the name `platformioSecrets.ini`

```ini
; used by the OTA environments via "extends" (https://docs.platformio.org/en/latest/projectconf/section_env_advanced.html#extends)
[esp32dev-ota]
upload_port = 192.168.x.x
; same as OTAPassword in secrets.h
custom_ota_password = myotapassword
; only needed for the espota environment (esp32rc)
upload_flags =
 --port=3232
 --auth=myotapassword
```

Then run `pio run -e esp32dev -t upload`. This POSTs the firmware to the device's HTTP update endpoint (`http://<ip>/update`, user `chromance`, password `OTAPassword`) with curl. It works from a dev container, WSL, or any machine that can reach the device, because the device never has to connect back.

`esp32rc` still uses ArduinoOTA (espota) as a fallback. espota needs the device to connect back to the uploader, so it does not work from a dev container or from WSL in NAT networking mode.

### Home Assistant / MQTT

The Chromance connects to the MQTT broker set in `secrets.h` and registers itself with Home Assistant through [MQTT discovery](https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery). Add the [MQTT integration](https://www.home-assistant.io/integrations/mqtt/) to Home Assistant, pointed at the same broker and using the default `homeassistant` discovery prefix, and a **Chromance** device appears automatically. Discovery is sent again whenever Home Assistant restarts (it listens on `homeassistant/status`).

The device has these entities:

- **LED Controller** (light): on/off, brightness, and an effect list with `Random` plus every animation. `Random` switches to a random animation every 30 seconds (`RandomAnimationDuration` in [constants.h](src/constants.h)).
- **FPS** (sensor): current frame rate.
- **\<Animation\> Speed** (number, 0.01–10): per-animation speed multiplier.
- **\<Animation\> Pulse Period**, **Decay** and **Lifespan** (numbers): ripple animations only. Pulse Period (ms) is how often new ripples start, Lifespan (ms) is how long a ripple lives, and Decay (0–255) is how much of the trail is kept each frame: higher values leave longer trails.

Settings changed from Home Assistant are saved in the ESP32's flash (NVS), so they survive reboots and firmware updates.

#### Topics

| Topic | Direction | Payload |
|---|---|---|
| `chromance/v1/command` | to device | JSON command, see below |
| `chromance/v1/state` | from device | JSON state. Published every 5 s while on, every 15 s while off, and right after a command changes something. It is also the availability topic: the broker's last will sets `"availability": "0"` when the device drops off. |
| `homeassistant/status` | to device | `online` makes the device re-send discovery |

#### Commands

Send any combination of these keys in one JSON object to `chromance/v1/command`:

```json
{ "state": "ON", "brightness": 128, "effect": "Cube Pulse" }
```

| Key | Value |
|---|---|
| `state` | `"ON"` or `"OFF"` (fades in/out) |
| `brightness` | 0–255 |
| `effect` | `"Random"` or an animation name from the table below |
| `reboot` | any value restarts the device |
| `as<N>` | speed multiplier for animation `N` (default 1.0) |
| `rp<N>` | ripple pulse period in ms for animation `N` (default 2000) |
| `rl<N>` | ripple lifespan in ms for animation `N` (default 2000) |
| `rd<N>` | ripple trail decay 0–255 for animation `N`, higher = longer trails (default 247) |

| `N` | Animation | Ripple |
|---|---|---|
| 1 | Strip Test (wiring test, not in `Random`, no Home Assistant sliders) | |
| 2 | Random Pulse | yes |
| 3 | Cube Pulse | yes |
| 4 | Star Burst Pulse | yes |
| 5 | Center Pulse | yes |
| 6 | Rainbow Beat | |
| 7 | Rainbow March | |
| 8 | Pulse | |
| 9 | Around the World | yes |

For example, `{ "as3": 2.5, "rl3": 4000 }` makes Cube Pulse faster with longer-lived ripples. `N` is the animation's position in `AnimationType` in [models.h](src/models.h). Don't reorder that enum: saved settings and Home Assistant entity IDs are keyed by these numbers.

### How to make an animation

To create your own animations you will want to look at the [map.h](src/animations/ripples/map.h) file and the [ripple.cpp](src/animations/ripples/ripple.cpp) file to a lesser extent.  This repository contains a [mapping.jpg](mapping.jpg) that shows each nodes number and the segment numbers.  You can use this image to make sense of the `NodeConnections`, `SegmentConnections`, `BorderNodes`, `CubeNodes`, `FunNodes`, and `StarBurstNode` variables in [`map.h`](src/animations/ripples/map.h)

### USB Updates (WSL)

Needed for the first flash, or to recover a device that won't boot. Otherwise use OTA.

On Windows, pass the ESP32 through to WSL with [usbipd](https://learn.microsoft.com/en-us/windows/wsl/connect-usb):

```powershell
winget install usbipd
usbipd list                          # find the "CP210x" / "CH340" serial adapter's BUSID
usbipd bind --busid <BUSID>          # admin, once per device
usbipd attach --wsl --busid <BUSID>  # every time it is plugged in
```

It then appears as `/dev/ttyUSB0` in WSL. The dev container has no USB passthrough, so flash over USB from WSL itself. Flash with `pio run -e esp32dev-usb -t upload` and watch the logs with `pio device monitor -e esp32dev-usb`.

A PC's USB port can't power the LED strips. On USB power alone the ESP32 flashes fine, but browns out and reboot-loops once the firmware starts. Keep the main 5V supply connected if your board isolates USB 5V from VIN with a diode (most do). Otherwise set the brightness low, or turn it off in Home Assistant, before unplugging, and move it back to the main supply after flashing.

