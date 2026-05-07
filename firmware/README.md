# firmware

this is where the shoe's brain lives.

the folder is organised by **what each part of the system does**, not by what file format it is. the goal is that anyone reading this — including future-me — can find the right place to look without hunting.

```
firmware/
├── main/         the entry-point sketch (main.ino)
├── sensors/      everything that reads the world (piezo today, more later)
├── modes/        the emotional modes (calm, happy, alert, sleepy, flow, cool)
├── animations/   reusable lighting primitives (breathe, pulse, wave)
├── utils/        shared config, state, helpers
└── power/        battery monitoring and power-aware behaviour
```

---

## current state

we are at **phase 1 — hardware bring-up**.

`main/main.ino` is the only active firmware right now. it is a self-contained hello-world that fades the WS2812B strip up softly and settles into a calm-blue breathing rhythm. no sensor reading yet — that lands in phase 2.

the other folders contain **intent stubs**: short header files that document what will live there. they are not yet wired into the build. this is deliberate — we want the architecture to be visible from day one, even before the code lands inside it.

---

## a note on the build system

the arduino IDE compiles every file inside a single sketch folder, but it does not traverse subfolders. that means the modular layout above is **conceptual today** — the hello-world sketch keeps its own constants inline.

once we have more than two or three modes, this will become limiting. at that point we migrate to **PlatformIO**, which compiles everything under `firmware/` together. the migration is tracked as an open question in `testing/changelog.md` (local-only).

until then: open `firmware/main/main.ino` in the arduino IDE and you are good.

---

## how to run the hello-world

1. open `firmware/main/main.ino` in the arduino IDE
2. install the **FastLED** library if you don't have it (sketch → include library → manage libraries)
3. select **board: arduino nano**, and if upload fails, switch processor to **ATmega328P (Old Bootloader)**
4. wire 5V, GND, and D6 → 330Ω → strip DIN with a 1000µF capacitor across the strip's power
5. upload

the LEDs should fade up softly and breathe in blue. that is the baseline emotional state.

if it doesn't behave that way, see the **troubleshooting** section in the root `README.md` — the usual suspect is grounding or the series resistor.

---

## design principles for any code that lands here

these are the rules the firmware is held to. they exist because they have already saved us from past mistakes (and will again).

- **no `delay()` past phase 1.** every animation must run on `millis()` so the system stays responsive to taps.
- **one mode per file** once we have more than the hello-world. modes are easier to read in isolation.
- **animation primitives are shared.** breathing, pulsing, waving — these are utilities, not duplicated per mode.
- **all pin numbers, LED counts, brightness defaults, and timing constants live in `utils/config.h`** once the build system supports it. nothing magical scattered through code.
- **readability beats cleverness.** this is a wearable, not a contest. a junior reader should be able to understand any single file in under five minutes.
- **comments explain *why*, not *what*.** the code already says what it does.

---

## what each subfolder will hold

### `main/`
the actual `.ino` entry point. boots the system, owns the main loop, dispatches to the active mode.

### `sensors/`
the piezo tap reader is the first inhabitant. anything that reads from the world goes here — future motion sensors, ambient light sensors, accelerometers.

### `modes/`
each emotional mode is a self-contained file. they all expose the same lifecycle: enter, update, exit. that contract is what lets the main loop swap between them cleanly.

### `animations/`
the small, sharp tools that modes compose: a breathe curve, a pulse curve, a wave function, an eased cross-fade. modes are *expressions*; animations are *primitives*.

### `utils/`
- `config.h` — pins, counts, thresholds, brightness defaults
- `state.h` — the running state of the system (current mode, last tap, intensity)

### `power/`
battery voltage reading, low-battery emotional state, power-down behaviour. this matters more once the shoe is actually being worn for hours at a time.
