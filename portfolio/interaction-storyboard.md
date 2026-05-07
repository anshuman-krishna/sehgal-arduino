# interaction storyboard

> the wearer's journey, frame by frame, from a sleeping shoe to an expressed emotion and back. read it in order. each frame is one moment — usually under a second.

---

## scene 1: the shoe is at rest

**what's happening**
the wearer hasn't tapped the shoe in over two minutes. the firmware has eased the brightness down to about 20% of its full output. the **calm-blue breathing** continues, just quieter — a slow inhale and exhale every four seconds.

**what the wearer sees**
a soft glow along the sole edge. it's noticeable in dim rooms and almost invisible in daylight. not "off" — the shoe is alive but at rest.

**what the system knows**
- `lastInteractionMs` was set 130 seconds ago
- the brightness composer is multiplying mode output by `DEEP_IDLE_BRIGHTNESS_SCALE` = 50/255 ≈ 20%
- gestures are armed and waiting

---

## scene 2: a deliberate tap

**what's happening**
the wearer notices the shoe, becomes curious, and taps the heel firmly with their other foot. the piezo disc registers a sharp voltage spike. the analog input on A0 jumps from near-zero to ~600.

**what the wearer sees**
nothing yet — the visual response is two beats away.

**what the system knows**
- `gestureRawTapDetected()` returns `true`
- `gPendingTapCount` increments to 1
- `gLastTapInSequenceMs` is set to now
- the multi-tap window starts ticking

---

## scene 3: the system waits to see what kind of tap it was

**what's happening**
nothing visible. the firmware is holding its breath for up to 400ms (`MULTI_TAP_WINDOW_MS`) to find out if a second tap is coming. this is where single-tap latency lives — it's the cost of being able to also detect double and triple.

**what the wearer sees**
still nothing. about half a second has passed since the tap.

**what the system knows**
- pending count is 1, no second tap yet
- if a second tap arrives → DOUBLE_TAP → mode reverse
- if 400ms pass with no second tap → SINGLE_TAP → mode advance

---

## scene 4: single-tap classified, mode change begins

**what's happening**
the multi-tap window expires with one tap on the books. `gestureUpdate()` returns `GESTURE_SINGLE_TAP`. main.ino calls `changeMode(modeNext(currentMode), now)`. the previous mode (calm) is locked into `prevMode`. `currentMode` becomes the next one in the cycle (happy). a 600ms cross-fade begins. `lastInteractionMs` resets to now — the shoe instantly wakes from idle.

**what the wearer sees**
the shoe brightens (idle decay vanishes — back to full intensity) and the colour begins easing from blue toward warm orange.

**what the system knows**
- `inTransition = true`, `modeTransitionStart = now`
- the render loop is calling both `drawCalm` (into modeBufA) and `drawHappy` (into modeBufB), then blending pixel-wise via `blendColor` with t = smoothstep(elapsed/600ms)

---

## scene 5: mid cross-fade

**what's happening**
about 300ms into the transition. each pixel is ~50% of the way from calm-blue to happy-orange. the smoothstep curve makes the change feel like an in-breath rather than a wipe.

**what the wearer sees**
the strip is somewhere between blue and orange — a strange purple-y mid-state for a frame, then the orange takes over.

**what the system knows**
- `t = smoothstep(0.5) = 0.5`
- both mode buffers are being rendered every frame, then blended into the live buffer
- this costs ~50 bytes of extra RAM (the second buffer) and roughly 2x the CPU of a single mode

---

## scene 6: the new mode settles in

**what's happening**
600ms into the transition. `inTransition` flips to false. the live buffer is rendered directly from `drawHappy`. the warm-orange pulse is now fully expressed at full brightness.

**what the wearer sees**
the shoe is glowing warm orange, pulsing in a 1.5-second cycle that feels noticeably faster than the calm breathing it replaced. the colour reads as "open, energetic, social."

**what the system knows**
- mode index `1` (MODE_HAPPY) was just persisted to EEPROM
- the next reboot will start in happy unless a new mode is committed first

---

## scene 7: triple-tap toggles privacy

**what's happening**
a few minutes later the wearer enters a quieter setting and wants to be more subtle. they tap the heel three times, fast. each tap feeds into `gPendingTapCount`. on the third, the firmware fires `GESTURE_TRIPLE_TAP` immediately (no waiting — there's no quadruple to disambiguate against).

`isPrivate` flips to `true`. `EEPROM.update(EEPROM_PRIVATE_ADDR, 1)` writes it.

**what the wearer sees**
the shoe is still happy-orange, but the brightness drops to about 50% within one frame. same emotional content, half the volume. nobody else in the room notices anything has changed.

**what the system knows**
- `isPrivate = true`
- `computeBrightnessScale()` now multiplies by `PRIVATE_BRIGHTNESS_SCALE = 128/255`
- this state will persist across power-cycles

---

## scene 8: the wearer leaves the shoe alone

**what's happening**
the wearer goes about their day. no taps for 30 seconds. then 60. then 120. each tick of the loop, `lastInteractionMs` stays put, and `now - lastInteractionMs` grows.

**what the wearer sees**
the shoe gradually fades. at 30 seconds it's noticeably dimmer than baseline. at 90 seconds the eye barely registers it. at 2 minutes it's settled into deep-idle: a quiet pulse, present but not asking for attention.

**what the system knows**
- between IDLE_THRESHOLD_MS (30s) and DEEP_IDLE_THRESHOLD_MS (2min), an eased smoothstep curve takes the brightness scale from 255 down to `DEEP_IDLE_BRIGHTNESS_SCALE = 50`
- multiplied by `PRIVATE_BRIGHTNESS_SCALE = 128`, the effective brightness floor in this scene is roughly `50 × 128 / 255 = 25/255` — about 10% of the original full-public brightness

---

## scene 9: a tap, hours later

**what's happening**
hours later, on a different day. the shoe has been at deep-idle since the wearer last interacted. they remember it's there, and tap once.

**what the wearer sees**
the brightness snaps back to full instantly. then there's the familiar 400ms wait, the cross-fade into the next mode, and the shoe is alive again.

**what the system knows**
- `lastInteractionMs` jumps to `now`
- the idle scale snaps back to 255 on the very next loop tick
- the SINGLE_TAP that follows initiates the next mode change

---

## the loop

scenes 1 through 9 form the natural rhythm of the wearable:

```
   rest  →  attention  →  expression  →  rest
                                            │
                                            └──── tap →
```

every part of the firmware is in service of this loop feeling natural. nothing here is a "feature" — every behaviour exists because the loop demands it. the shoe is at rest most of the time. the moments of expression are the meaningful ones, and those are what the project is actually about.
