# system architecture

> a single dataflow diagram and the supporting notes that explain how a heel-tap becomes an emotional frame on the LED strip.

## the dataflow

```
                    PHYSICAL                                    INTERPRETATION                                EXPRESSION
                  ─────────────                              ──────────────────                            ────────────────

   ┌──────────────────────────────┐         ┌──────────────────────────────────────┐         ┌────────────────────────────────┐
   │                              │         │                                      │         │                                │
   │     27mm piezoelectric       │         │           ARDUINO NANO V3            │         │         WS2812B strip          │
   │     disc under heel insole   │         │             (ATmega328P)             │         │       along sole edge          │
   │                              │         │                                      │         │                                │
   │   ┌──────────────────────┐   │  raw    │   ┌──────────────────────────────┐  │  CRGB[]│   ┌────────────────────────┐   │
   │   │  2.2MΩ bleed         │   │ analog  │   │  gestures.h                   │  │ frame  │   │  30 addressable RGB    │   │
   │   │  resistor (rest      │ ──┼────────►│   │   ├ debounce (180ms)          │  │        │   │  pixels                │   │
   │   │  voltage stable)     │   │ A0      │   │   ├ tap-counting (multi-tap)  │  │        │   │                        │   │
   │   └──────────────────────┘   │         │   │   └ walking-mute (budget)     │  │        │   │  data ◄── D6 ── 330Ω  │   │
   │                              │         │   └────────────────┬─────────────┘  │        │   │                        │   │
   │                              │         │                    │                │        │   │                        │   │
   │                              │         │                    │ GestureEvent   │        │   │  5V ◄── battery       │   │
   │                              │         │                    ▼                │        │   │  GND ◄── common       │   │
   └──────────────────────────────┘         │   ┌──────────────────────────────┐  │        │   │                        │   │
                                            │   │  main.ino (dispatcher)        │  │        │   │  1000µF cap across   │   │
                                            │   │   ├ mode change (next/prev)   │  │        │   │   5V/GND near strip  │   │
                                            │   │   ├ public/private toggle     │  │        │   └────────────────────────┘   │
                                            │   │   ├ idle / wake state         │  │        │                                │
                                            │   │   └ EEPROM persist            │  │        │                                │
                                            │   └────────────────┬─────────────┘  │        │                                │
                                            │                    │                │        │                                │
                                            │                    │ EmotionalMode  │        │                                │
                                            │                    ▼                │        │                                │
                                            │   ┌──────────────────────────────┐  │        │                                │
                                            │   │  modes.h                      │  │        │                                │
                                            │   │   ├ drawCalm / Happy / Alert  │  │        │                                │
                                            │   │   ├ drawSleepy / Flow / Cool  │  │        │                                │
                                            │   │   └ cross-fade (smoothstep)   │  │        │                                │
                                            │   └────────────────┬─────────────┘  │        │                                │
                                            │                    │                │        │                                │
                                            │                    │ rendered CRGB  │        │                                │
                                            │                    ▼                │        │                                │
                                            │   ┌──────────────────────────────┐  │        │                                │
                                            │   │  brightness composer          │  │        │                                │
                                            │   │   = boot × idle × private     │  │        │                                │
                                            │   │   one nscale8 across strip    │  │        │                                │
                                            │   └────────────────┬─────────────┘  │        │                                │
                                            │                    │                │        │                                │
                                            │                    ▼ FastLED.show() │        │                                │
                                            └────────────────────┼────────────────┘        │                                │
                                                                 │                          │                                │
                                                                 └──────────────────────────►                                │
                                                                                            │                                │
                                                                                            └────────────────────────────────┘
```

## what each block actually does

### physical input layer

- the **piezo disc** generates a small voltage spike when struck. it's not pressure-sensitive — it's strike-sensitive. that's why long-press isn't on the gesture menu.
- the **2.2MΩ bleed resistor** drains the piezo's static charge to ground, so the resting analog reading sits cleanly near zero. without it, the baseline drifts and threshold tuning becomes unstable.
- the **A0 analog input** is sampled inside `gestureRawTapDetected()` only when needed, not on every loop tick.

### interpretation layer

- **gestures.h** is where raw analog readings become discrete gesture events. it owns the 180ms debounce window (handles piezo ring-out), the multi-tap counter (single → double → triple), and the walking-noise mute (budget of 3 taps in 2s → mute 1s).
- **main.ino** turns gesture events into intent: SINGLE_TAP advances mode, DOUBLE_TAP reverses, TRIPLE_TAP toggles public/private. it owns the EEPROM-backed mode + private state, the idle/wake state machine, and the cross-fade orchestration during mode changes.
- **modes.h** doesn't know about gestures. it knows only "draw mode X at time T into this CRGB buffer". this separation is what lets two modes blend pixel-wise during cross-fades.

### expression layer

- the **brightness composer** is a small but important piece: it multiplies three brightness scales (boot fade-up × idle decay × private dim) into one final scalar applied uniformly across the strip per frame. this is why the modes themselves don't need to know about idle, private, or boot — they just render at full intensity, and the composer handles emotional context.
- **FastLED.show()** clocks the rendered frame out over D6 with the 330Ω series resistor protecting the strip's data input.

## memory budget (ATmega328P, 2KB SRAM)

- live LED buffer: 30 × 3 = **90 bytes**
- two scratch buffers (cross-fade): 2 × 90 = **180 bytes**
- gesture state: ~24 bytes
- mode + private flags + transition state: ~16 bytes
- string literals (`F()`-wrapped): in flash, no SRAM cost
- FastLED internals: ~50 bytes

total used: roughly **360 bytes** of the 2048 available. plenty of headroom for phase 10 expansions like an accelerometer module or a small rolling buffer for walking-noise rejection.

## why this architecture (and not something else)

- **header-only multi-file Arduino sketch** rather than PlatformIO modular project. arduino IDE auto-compiles every `.h`/`.ino` in the sketch folder, which gives the modularity without forcing a toolchain on the user. PlatformIO migration is a tracked open question, deferred until there's a forcing function (extracted libraries, unit tests, dependency conflicts).
- **gestures and modes are decoupled.** modes don't know how taps are classified; gestures don't know what each tap means. main.ino is the only place that connects them. swapping the gesture classifier (for example, replacing piezo with a force-sensitive resistor and adding long-press) would touch only `gestures.h` and the dispatch in `main.ino`.
- **brightness as a composable scalar.** instead of having modes know about idle/private/boot, they always render at full and a single multiplier shapes the final output. this is what makes the system's emotional layers (mode, idle, private) feel composable rather than entangled.
- **EEPROM with a magic byte.** first-boot detection without a setup wizard. addr 1 holds `0x42`; if that's missing on boot, we're on a fresh nano and start in calm.
