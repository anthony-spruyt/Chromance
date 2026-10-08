# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository layout

Chromance is hexagonal LED wall art. The hardware design comes from Zack Freedman / Voidstar Lab, and `hardware/` holds those assets (`Models/*.f3d`, `STL's/*.stl`, assembly guide PDF). This repo is a fork: the firmware at the repo root (`platformio.ini`, `src/`) is largely a rewrite by the repo owner and shares little with upstream beyond the hex topology in `map.h`. Its conventions (explicit
`this->`, PascalCase methods, `namespace Chromance`, constants in `constants.h`) are deliberate, so match them. It is a PlatformIO project for an ESP32 (Arduino framework, FastLED, PubSubClient, ArduinoJson, ezTime).

## Git workflow

This repo uses trunk-based development. Commit straight to `main`. Open a branch and PR only for very large or risky changes, or when asked.

## Build / upload

Run from the repo root with the PlatformIO CLI `pio` (the dev container installs it with pipx in `.devcontainer/setup-devcontainer.sh`):

```sh
pio run -e esp32dev-usb                  # build (serial logging on, debug build)
pio run -e esp32dev-usb -t upload        # flash over USB (esptool)
pio device monitor -e esp32dev-usb       # serial monitor, 115200, with exception decoder
pio run -e esp32dev -t upload            # OTA upload over HTTP (curl POST to /update) — default env
pio run -e esp32rc -t upload             # OTA upload via ArduinoOTA/espota (fallback)
```

- There are no tests. `build_src_flags = -Wall -Wextra` applies to `src/` only, and `src/` builds with zero warnings, so keep it that way.
- `./lint.sh` runs MegaLinter, including clang-format (`.clang-format`), cppcheck and cpplint (`CPPLINT.cfg`) on `src/`, and ruff on `scripts/` (`pyproject.toml` extends the synced `ruff-base.toml`). These linters are turned on in `.mega-linter.yml`, which this repo owns. Keep hand-aligned tables in `// clang-format off` blocks.
- CI (`.github/workflows/ci.yaml`, owned by this repo) runs MegaLinter and builds `esp32dev` and `esp32dev-usb` with placeholder secrets. The `summary / Check Results` job fails if either fails.
- Secrets come from environment variables (`WIFI_SSID`, `WIFI_PASSWORD`, `OTA_PASSWORD`, `UPLOAD_PORT`, `MQTT_BROKER`, `MQTT_PORT`, `MQTT_USERNAME`, `MQTT_PASSWORD`). The dev container loads them from the host's `~/.secrets/.env.chromance`, so they only change on a container rebuild. Never print their values.
- The `pre:scripts/inject_secrets.py` extra script fails the build if any is missing (except IDE indexing and `clean`) and writes them into a generated `secretsEnv.h` in the build dir. Committed `src/secrets.h` wraps those macros as the `constexpr` `WifiSsid`, `OTAPassword`, `MQTTBroker` and so on. It uses a header, not `-D` flags, so values never pass through the shell or show up in compile
  commands.
- `[esp32dev-ota]` in `platformio.ini` reads `UPLOAD_PORT` and `OTA_PASSWORD` with `${sysenv.*}`. The HTTP upload's curl command uses `$$OTA_PASSWORD`, so the shell expands it and PlatformIO doesn't echo it.
- HTTP OTA (`OTAService::SetupHttpOTA`, basic auth `chromance`/`OTAPassword`) works from anywhere that can reach the device, including the dev container. `/update` is routed through `HttpOtaRequestHandler`, not `server.on(uri, method, fn, uploadFn)`. In this WebServer version a registered upload callback is also called for non-multipart POSTs, where `server.upload()` dereferences null and reboots
  the device. Don't add upload routes with `server.on`. espota does not work from a dev container or NAT-mode WSL, because the device connects back to the uploader. USB works from the WSL host once the device is passed through with `usbipd`. The synced dev container has no USB passthrough.
- Use `constexpr`, not `static const`, in `src/secrets.h`. With `-Wall`, unused `static const char*` variables warn in every translation unit.
- The repo is onboarded to `anthony-spruyt/repo-operator` (xfg). It syncs root tooling files (`.devcontainer/`, `.vscode/settings.json`, `.pre-commit-config.yaml`, `.claude/`, `renovate.json`), so change those there, not here. `.devcontainer/setup-devcontainer.sh` and `renovate-overrides.json5` are the exceptions: they are seeded once (`createOnly`) and owned by this repo.
- `Serial` output only exists when `SERIAL_ENABLED` is defined, which only the `esp32dev-usb` env does. `Logger` is a no-op otherwise.

## Architecture

**Tasks (`src/main.cpp`)**: Global service singletons are wired up by constructor injection. `setup()` starts four FreeRTOS tasks and `loop()` is empty. `AnimationControllerTask` runs alone on core 1. WiFi, OTA and MQTT run on core 0. Stack sizes, priorities and cores are in `constants.h`. Define `MONITOR_TASK_STACK_SIZES` in `definitions.h` to log stack usage. Animation rendering pauses while
`otaService.IsUpdating()`.

**Headers**: Every file includes `globals.h`, which pulls in `definitions.h` (FastLED `#define`s that must come before `FastLED.h`), `constants.h` (all tunables: pins, strip lengths/offsets, MQTT topics, per-animation enable flags, task config), `secrets.h`, and `models.h` (enums/structs).

**Cross-task communication**: The MQTT task never touches animations directly. It calls `AnimationController::Play/Sleep/Wake`, which set a pending `AnimationRequest` under a mutex. The animation task applies the request in `Loop()` (`xSemaphoreTake(..., 0)`, non-blocking). `MQTTClient::Publish` works the same way, filling a small queue that `MQTTClient::Loop` drains.

**Animations (`src/animations/`)**:

- `Animation` is the base class. Each instance owns its own `CRGB leds[NumberOfLEDs]` buffer and a status (`PLAYING`, `SLEEPING`, `WAKING_UP`, `GOING_TO_SLEEP`).

- Each animation owns one `transitionScale` that `Transition()` moves over the configured transition duration by `millis()`. A reversed fade carries on from the current scale. `Wake(false)`/`Sleep(false)` switch instantly. Waking from `SLEEPING` calls `Reset()`, which clears the buffer. `RippleAnimation` overrides it to release its ripples and `PulseAnimation` to pick a new colour.

- `AnimationController::Render()` runs every non-sleeping animation and adds each buffer into `leds`, scaled by `ease8InOutCubic(GetTransitionScale())`. Fades never modify animation buffers. `Show()` wakes one animation and sleeps the rest. The strip test always switches without a fade because its `Loop()` blocks for seconds.

- In "Random" mode a new animation is picked every random duration seconds (config `ra`).

- Brightness is applied per frame with `FastLED.show(brightness)`, not `FastLED.setBrightness()`. `Render()` scales the HA brightness by the max brightness percent, fades it over the transition duration, then lowers it if FastLED's power estimate for the frame exceeds the max current. Dithering is re-enabled before every `show()` because FastLED switches it off whenever its FPS reading is under
  100, which is always true at boot. Without it, dim colours step visibly.

- `RippleAnimation` subclasses (Cube/StarBurst/Center/Random Pulse, AroundTheWorld) implement `Start()`. They claim `Ripple`s from one `RipplePool` of 30 that all ripple animations share (`Claim(animationId)`). Ripples and trail decay advance in fixed steps at the configured ripple steps per second, not once per frame, so ripple speed and trail length don't depend on FPS.

- Ripples walk the hex graph defined in `animations/ripples/map.h`: `NodeConnections` (node → 6 segment slots, clockwise from 12:00, -1 = none), `SegmentConnections`, `LEDAssignments` (segment → LED indices) and node groups such as `BorderNodes`, `CubeNodes` and `StarBurstNode`. `mapping.jpg` shows the node and segment numbering.

- LEDs are four physical NEOPIXEL strips (blue/green/red/black) that map into one contiguous array through the offsets in `constants.h`.

- `LEDMap` (`animations/ledMap.*`, one instance owned by `AnimationController`) precomputes every LED's x, y, distance and angle from the center node (0–255) from `NodeCoordinates` in `map.h`. Position-based animations (Plasma, Radar, Rainbow Swirl, Rings, Fire) take an `LEDMap*` and never touch strip wiring. `SegmentLED(segment, step)` in `map.h` maps a segment step (0 = top) to an LED index.

- Move animations by accumulating `rate * GetElapsedSeconds()` (speed-scaled seconds since the last call, once per `Loop()`) into a float member, wrapped with `fmodf`. `millis() * rate` jumps whenever a slider changes the rate.

- Per-animation Home Assistant sliders: an `AnimationParameter` table plus an index enum in `constants.h`, returned from `GetAnimationParameters()`, read with `GetParameter(index)`. Config keys are `ap<N>_<P>` and unique IDs `chrap<N>_<P>`, so only append parameters, never reorder.

**Adding an animation**:

1. Copy `animationTemplate.{h,cpp}`.
2. Add an `ANIMATION_TYPE_*` entry in `models.h` before `ANIMATION_TYPE_NUMBER_OF_ANIMATIONS`.
3. Add an `*Enabled` flag in `constants.h`.
4. Instantiate it in `AnimationController::Setup()`.

The name string passed to the base constructor becomes the Home Assistant effect name.

**Enum ordering matters**: `RANDOM_ANIMATION` must stay 0, `STRIP_TEST` must stay 1, and `NUMBER_OF_ANIMATIONS` must stay last. `NextAnimation()` skips indices below 2. Config keys are built as prefix + enum integer (e.g. `as3`, `rl3`), and so are HA discovery unique IDs. Reordering the enum therefore scrambles persisted NVS settings and HA entities.

**Config (`services/config.*`)**: Values persist to ESP32 NVS through `Preferences` (namespace `config`): brightness, sleeping, log level, transition duration (`td`), ripple steps per second (`rs`), max brightness percent (`mb`), max current in mA (`mc`, sent and published as amps), random duration in seconds (`ra`), and per-animation speed / ripple lifespan / pulse period / decay. Setters only
update memory. `Config::Save()` writes changed keys after `ConfigSaveDelay` without changes, and only the animation task calls it, right after `FastLED.show()`. Flash writes stall the I2S LED driver's refill code and garble the 3-wire strips, so never write NVS from other tasks or mid-frame. The reboot command calls `Save(true)` first.

**MQTT / Home Assistant (`services/mqttClient.*`)**:

- On connect, and whenever `homeassistant/status` reports `online`, the client publishes HA MQTT discovery configs: a JSON-schema light with an effect list, FPS and estimated current sensors, global transition duration / ripple steps per second / max brightness / max current / random duration `number` entities, and per-animation `number` entities.
- Commands arrive as JSON on `chromance/v1/command`. Keys: `state`, `brightness`, `effect`, `reboot`, `td`, `rs`, `mb`, `mc`, `ra`, plus the per-animation config keys.
- State is published to `chromance/v1/state` periodically (faster while playing than while sleeping).
- Disabling an animation through its `*Enabled` flag leaves a `nullptr` at its index. Discovery, command handling, `Play()` and `NextAnimation()` skip those, so any new code that loops over `GetAnimation(i)` must null-check too.
