---
name: new-animation
description: Create or tune a Chromance LED animation, from picking the approach through Home Assistant sliders, flashing over OTA and checking it on the wall. Use when the user asks for a new animation or effect, or to change how an existing one looks or moves.
---

# New Chromance animation

Read the Architecture section of `CLAUDE.md` first. This skill adds what was learned building Plasma, Radar, Rainbow Swirl, Rings, Fire and Twinkle.

## 1. Pick the approach

Don't work with strips or LED indices directly. The four strips run in odd directions, which is what made the original animations hard to write. Pick the highest-level tool that fits:

| Look                                            | Tool                                                                                                                         | Example           |
| ----------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------- | ----------------- |
| Colour depends on where the LED is on the wall  | `LEDMap`: `GetX`, `GetY` (0–255 across and down), `GetDistance` (0–255 from the center node), `GetAngle` (0 = 12:00, clockwise) | `fireAnimation`   |
| Whole hex lines light up                        | `SegmentLED(segment, step)` in `ripples/map.h`, step 0 = top end                                                               | `twinkleAnimation` |
| Something travels along the lines, node to node | Subclass `RippleAnimation` and implement `Start()`                                                                             | `cubePulseAnimation` |

Most new ideas belong in the first row. Loop `i` over `NumberOfLEDs` and set `this->leds[i]` from the map values. Good building blocks from FastLED 3.7.6: `inoise8(x, y, z)`, `sin8`, `ColorFromPalette` with the built-in palettes (`PartyColors_p`, `HeatColors_p`, `OceanColors_p`, `LavaColors_p`, ...), and `dim8_raw` so fades look even to the eye.

## 2. Write it

1. Copy the closest existing animation's `.h` / `.cpp` (they're better starting points than `animationTemplate`). Position-based animations take `LEDMap* ledMap` in the constructor.
2. Append `ANIMATION_TYPE_*` in `models.h` just before `ANIMATION_TYPE_NUMBER_OF_ANIMATIONS`. Never reorder.
3. In `constants.h`, under `ANIMATIONS - PARAMETERS`, add the `*AnimationEnabled` flag, the parameter enum and the `AnimationParameter` table. Add a `case` to `GetAnimationParameters()`.
4. Include the header and instantiate it in `AnimationController::Setup()` (pass `&ledMap` if needed).
5. Add a row to the animation table in `README.md`.

### Expose the knobs, don't guess them

The user can't tune a hard-coded number without a reflash. Expect your first guesses to be wrong: the first Fire rose far too fast, and the "fixed" one was too slow. Every speed, size, count or amount the look depends on should be an `AnimationParameter`:

```cpp
enum FireParameter { FIRE_RISE_SPEED, FIRE_FLICKER_SPEED, FIRE_COOLING, FIRE_DETAIL };
constexpr AnimationParameter FireAnimationParameters[] =
{
    // name, min, max, step, default, unit
    {"Rise Speed", 0.0f, 1000.0f, 5.0f, 400.0f, nullptr},
    ...
};
```

Config, NVS, MQTT commands, state and HA discovery all pick it up automatically. Read it with `this->GetParameter(FIRE_RISE_SPEED)`. Rules:

- Name the entity so that bigger means more: "Rise Speed", not "Rise Divisor".
- Parameters are keyed by position (`ap<N>_<P>`, `chrap<N>_<P>`). Only append. Keep the enum in the same order as the table.
- At most `MaxAnimationParameters` (a `static_assert` catches it).
- Keep a minimum of 1 for anything you divide by.

### Movement

Keep a float member for each moving thing. Add `rate * seconds` to it every frame, and wrap it:

```cpp
float seconds = this->GetElapsedSeconds();   // once per Loop(), already scaled by the Speed slider
this->rise = fmodf(this->rise + this->GetParameter(FIRE_RISE_SPEED) * seconds, 65536.0f);
```

- Don't use `millis() * rate` or `millis() / divisor`. The whole pattern jumps when the rate slider moves.
- Wrap at 256 for hues and angles, and at 65536 for `inoise8` coordinates.
- Call `GetElapsedSeconds()` once per frame. Every call resets the clock.
- Override `Reset()` (and call `Animation::Reset()`) if the animation needs a clean start when woken, e.g. Twinkle sets every segment to already faded out.

## 3. Check it

1. Build both envs. `src/` must stay at zero warnings:

   ```sh
   pio run -e esp32dev-usb 2>&1 | grep -E "^src/.*(warning|error)|error:|SUCCESS|FAILED"
   pio run -e esp32dev 2>&1 | grep -E "^src/.*(warning|error)|error:|SUCCESS|FAILED"
   ```

   The FastLED `-Wpragmas` warnings come from the library and are expected.

2. Flash over OTA with `pio run -e esp32dev -t upload`. Look for `Update complete, rebooting`.
3. Through the Home Assistant MCP tools, check `Chromance FPS` (normally ~116) and `Chromance Current`. A big FPS drop while the new animation plays means the per-LED work is too heavy. One `inoise8` per LED (Plasma, Fire) has been fine. If FPS drops, look for per-LED `atan2f` / `sqrtf` / float maths and precompute it, as `LEDMap` does.
4. Ask the user to watch it on the wall. You can't see it. New sliders only appear in HA after the device resends discovery (on reboot or when HA restarts). Reloading the MQTT integration also works.
5. Tune from their feedback. Change the default in the parameter table once they find values they like.

## 4. Finish

- Update `README.md` and `CLAUDE.md` if the approach or the tools changed.
- Commit to `main` (trunk-based).
