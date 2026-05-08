# firmware

this is where the shoe's brain lives.

the folder is organised by **what each part of the system does** rather than by file format. the goal is that anyone reading this, including future-me, can find the right place to look without hunting.

```
firmware/
├── main/         the active multi-file sketch (open main.ino in arduino IDE)
├── experiments/  preserved bring-up sketches (phase 1 hello-world, piezo debug)
├── sensors/      intent stub, future home of standalone sensor readers
├── modes/        intent stub, future home of one-file-per-mode
├── animations/   intent stub, future home of reusable animation primitives
├── utils/        intent stub, future home of shared config/state
└── power/        intent stub, battery monitoring, low-battery emotional state
```

---

## current state

we are at **phase 8** on the firmware side. phases 1 through 6 are all live in a single sketch:

- soft fade-up at boot
- six emotional modes (calm, happy, alert, sleepy, flow, cool) with eased cross-fades
- piezo gesture classifier (single, double, triple-tap) with walking-noise mute
- EEPROM-backed mode and public/private persistence
- idle dim-down (30s → 2min) with instant wake on any classified gesture

phases 7 and 8 are physical and don't change firmware. phase 5 is also physical. the next firmware-side work is phase 10, which is deliberately speculative for now (designs live in `/experiments/`, with no implementation yet).

## what's actually being compiled

`firmware/main/` is the Arduino sketch that the IDE opens and uploads. it's a small multi-file sketch:

```
firmware/main/
├── main.ino        boot, gesture dispatch, brightness composer, render loop
├── config.h        every tuneable constant (pins, brightness, timings, EEPROM)
├── animations.h    smoothstep, breathe, pulse, gamma8, blendColor, fillSolid
├── gestures.h      piezo read → debounce → tap-counter → SINGLE/DOUBLE/TRIPLE
└── modes.h         the six mode draw functions + dispatcher + cycling
```

the arduino IDE auto-compiles every `.h`/`.ino` in a sketch folder, so this works without any toolchain change. open `main.ino`, press upload.

## about the intent stubs

the sibling folders (`sensors/`, `modes/`, `animations/`, `utils/`, `power/`) each contain a short header file documenting what will live there once we migrate to **PlatformIO**. they sit outside the build today because the arduino IDE doesn't traverse subfolders.

this is deliberate. the intended architecture is visible from day one, and the migration becomes a mechanical move-files-into-the-shape-they-already-imply rather than a redesign.

PlatformIO migration is tracked as an open question in `testing/changelog.md` (local-only, gitignored). it'll happen when there's a forcing function: extracted libraries, unit tests, or dependency conflicts.

---

## how to flash the active firmware

1. open `firmware/main/main.ino` in the arduino IDE
2. install **FastLED** (sketch → include library → manage libraries)
3. `EEPROM` is built-in, no install needed
4. select **board: arduino nano**, and if upload fails, switch processor to **ATmega328P (Old Bootloader)**
5. wire the hardware per `README.md` § phase 1 + § phase 2 (5V/GND/D6/A0 with the protection components)
6. upload

if it doesn't behave as expected, the per-phase troubleshooting in the root `README.md` covers the common failure modes. start there before diving into the firmware.

## the bring-up sketches

`firmware/experiments/` holds two known-good sketches. they sit alongside the active firmware as bring-up tools and a fallback you can flash any time.

| sketch | what it does | when to use |
| ------ | ------------ | ----------- |
| `phase1_hello_world/` | LED-only: soft fade-up + calm-blue breathing | flash when you suspect the LED layer has regressed; if this works, the LED path is healthy |
| `piezo_debug/` | streams raw piezo readings @ 200Hz over serial | flash before phase 2 to characterise your noise floor and tap peaks (so you can pick a real `PIEZO_TAP_THRESHOLD`) |

both are documented in the root `README.md` § phase 1 / § phase 2.

---

## design principles for any code that lands here

these are the rules the firmware is held to. they exist because they have already saved us from past mistakes (and will again).

- **no `delay()` past phase 1.** every animation runs on `millis()` so the system stays responsive to taps. `main.ino` and every mode draw function obey this.
- **one mode per file** is the post-PlatformIO target. today they all live in `firmware/main/modes.h`, which works fine for six modes and would feel cramped at twelve.
- **animation primitives are shared.** breathing, pulsing, waving: these live as utilities in `animations.h` so each mode composes them instead of carrying its own copy.
- **all pin numbers, LED counts, brightness defaults, and timing constants live in `firmware/main/config.h`.** nothing magical scattered through code.
- **readability beats cleverness.** this is a wearable that has to be maintained by humans. a junior reader should be able to understand any single file in under five minutes.
- **comments explain *why*, not *what*.** the code already says what it does.
- **gestures and modes stay decoupled.** `gestures.h` doesn't know what taps mean; `modes.h` doesn't know how taps are classified. `main.ino` is the only place that wires them together. swapping the gesture classifier (e.g., adding an FSR for long-press) shouldn't touch `modes.h`.
- **brightness composes uniformly.** modes always render at full intensity. boot-fade × idle-decay × private-dim collapse into a single multiplier in `main.ino` and apply once per frame. modes leave brightness alone themselves.

---

## what each subfolder will hold (post-PlatformIO target)

### `main/`
the entry point. boots the system, owns the loop, dispatches to the active mode, composes brightness. today it also hosts every `.h` in the sketch; that splits up once cross-folder includes are real.

### `sensors/`
the piezo reader (today inside `main/gestures.h`) lands here as a standalone module. anything that reads from the world: piezo today, accelerometer (phase 10 motion-reactive), LDR (phase 10 adaptive-brightness), maybe an FSR for long-press support.

### `modes/`
each emotional mode in its own file. they all expose the same draw contract: render the current mode at time `t` into a CRGB buffer at full brightness. that contract is what lets two modes blend pixel-wise during cross-fades.

### `animations/`
the small, sharp tools that modes compose: breathe, pulse, wave, smoothstep, gamma8, blendColor. modes are *expressions*; animations are *primitives*. the rule for any primitive: stateless. callers pass time and any state they want to track.

### `utils/`
- `config.h`: pins, counts, thresholds, brightness defaults, EEPROM addresses
- `state.h`: the running state of the system (current mode, last tap, idle timer, private flag)

### `power/`
battery voltage reading, low-battery emotional state, power-down behaviour. matters once the shoe is being worn for hours. premature power optimization is a waste of effort: get interaction quality right first, then come back for runtime.
