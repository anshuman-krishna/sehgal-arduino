# sehgal-arduino

> an experimental wearable shoe that turns intentional foot taps into emotional light.

this project lives at the intersection of **wearable technology**, **interaction design**, and **emotional communication**. it is not a blinking-LED demo. it is a small, ongoing exploration into how a single human gesture — a tap of the heel — can become a readable emotional expression.

the interaction loop is intentionally simple:

```
tap  →  detect  →  interpret  →  express
```

a piezoelectric disc inside the heel listens for intentional taps. an arduino nano interprets the gesture. an addressable LED strip along the sole edge expresses the resulting emotion through colour, motion, and breathing rhythm.

---

## what this project is exploring

- can a wearable communicate **emotion** rather than **information**?
- how subtle can an interaction be while still feeling alive?
- how should a shoe *feel* to wear when it is also responding to you?
- what is the smallest gesture that can carry the largest emotional payload?

these are the questions the project is built around. every hardware decision, every animation curve, every wire route is in service of them.

---

## emotional modes

the system expresses six emotional states. each is designed to feel distinct and emotionally readable from a few feet away.

| mode    | feeling                | light behaviour                      |
| ------- | ---------------------- | ------------------------------------ |
| calm    | grounded, breathing    | soft blue breathing glow             |
| happy   | warm, social, open     | warm orange pulse                    |
| alert   | sharp, attentive       | fast red blink                       |
| sleepy  | low energy, drifting   | dim ambient purple                   |
| flow    | focused, in-motion     | travelling cyan wave                 |
| cool    | playful, performative  | rainbow motion                       |

all six modes are live as of phase 3. you switch between them by tapping the heel.

---

## hardware at a glance

| part                          | what it does                              |
| ----------------------------- | ----------------------------------------- |
| arduino nano v3 (ATmega328P)  | reads the sensor and drives the LEDs      |
| 27mm piezoelectric disc       | senses an intentional heel tap            |
| WS2812B LED strip             | the emotional output medium               |
| 5V battery / power bank       | the wearable power source                 |
| 1000µF capacitor              | smooths inrush current at LED power-on    |
| 330Ω (or 220Ω + 150Ω) series  | protects the WS2812B data line            |
| 2.2MΩ resistor                | bleeds the piezo sensor to a stable rest  |

all components are commonly available. nothing exotic.

---

## phase overview

this is a long project broken into small, testable phases. each one is a stable artifact you can hold up and look at — finish one before starting the next.

| phase | name                              | status                       |
| ----- | --------------------------------- | ---------------------------- |
| 0     | repository foundation             | ✅ done                      |
| 1     | hardware bring-up                 | ✅ firmware ready            |
| 2     | piezo sensor validation           | ✅ firmware ready            |
| 3     | emotional lighting engine         | ✅ firmware ready            |
| 4     | interaction intelligence          | ✅ firmware ready            |
| 5     | wearable integration              | 🔧 checklist driven          |
| 6     | emotional communication layer     | ✅ firmware ready            |
| 7     | safety & reliability              | 🔧 checklist driven          |
| 8     | aesthetic refinement              | 🔧 checklist driven          |
| 9     | portfolio system                  | (not yet started)            |
| 10    | experimental expansion            | (not yet started)            |

each phase ends with both a working artifact and a written reflection on what was learned.

## tracking your progress

as you wire, flash, and wear each phase, log what you observe in **[`validate.md`](./validate.md)** at the repo root. it's a structured checklist of every hardware-side check, with space under each item for measurements, notes, and decisions. tick items as you verify them. the file is the project's public hardware-truth logbook — it's how anyone reading the repo can see *what was actually built*, not just what was written.

---

## one-time setup (do this once before any phase)

> if this is your first arduino project, take it slow. nothing here is hard, but each piece is worth understanding before moving on.

### install the arduino IDE

download from [arduino.cc](https://www.arduino.cc/en/software). open it. from **tools → board** select **Arduino Nano**. some Nano clones use the older bootloader — if uploading later fails, switch the processor option to **ATmega328P (Old Bootloader)**. this is normal.

### install the FastLED library

inside the arduino IDE: **sketch → include library → manage libraries**. search for **FastLED** and install it. this is the library we use to drive the WS2812B strip.

### plug in the nano

connect over USB. in **tools → port**, pick the port that just appeared. on macOS it usually starts with `/dev/cu.usbserial` or `/dev/cu.wchusbserial`. on windows it's `COM3` or similar.

you are now ready for phase 1.

---

# phase 1 — hardware bring-up

> goal: prove your power, your strip, and your nano are all happy. no sensor yet. just light.

this is the cheapest possible test of the hardware path. if this works, you know the rest of the project has a solid foundation. if it doesn't, **stop and fix it here** — adding a sensor on top of a flaky LED setup will only make debugging harder later.

## phase 1 wiring

```
                 ┌──────────────┐
                 │ ARDUINO NANO │
                 │              │
       USB ──────┤              │
                 │           D6 ├──── 330Ω ──────┐
                 │              │                │
                 │          GND ├───────┐        │
                 │           5V ├───┐   │        │
                 └──────────────┘   │   │        │
                                    │   │        │
                                    │   │        ▼
                                    │   │  ┌───────────────┐
                                    │   │  │  WS2812B DIN  │
                                    │   └──┤  WS2812B GND  │
                                    └──────┤  WS2812B 5V   │
                                           └───────────────┘
                                                 │   │
                                                 │   │
                                          1000µF │   │
                                          (long  ├───┤  (short leg → GND)
                                          leg → 5V)
```

**checklist before you power on:**

- [ ] D6 → 330Ω resistor → strip's **DIN** pin (not DOUT)
- [ ] strip 5V → arduino 5V (for short prototype tests, USB power is fine)
- [ ] strip GND → arduino GND
- [ ] 1000µF capacitor across 5V and GND, **as close to the strip as you can get it** — this is what stops the inrush flicker
- [ ] capacitor polarity correct: long leg to 5V, short leg / striped side to GND
- [ ] no metal pins touching each other

> ⚠️ if the capacitor polarity is reversed, it can vent. don't skip this check.

## upload phase 1

1. open `firmware/experiments/phase1_hello_world/phase1_hello_world.ino`
2. in the IDE, confirm board = **Arduino Nano** and port is selected
3. press the **upload** button (the right-arrow icon)
4. wait. the IDE will say "compiling…", then "uploading…", then "done uploading."

## what you should see

- a soft fade-up over about 1.5 seconds, ending in a muted warm white
- then a slow, gentle breathing in soft blue. one breath every 4 seconds.
- the strip should never feel harsh, never strobe, never flicker

if that's what you see, **phase 1 is complete**. the hardware path is alive.

## what can go wrong (and how to fix it)

**nothing happens at all.**
the LEDs are dark and the nano's onboard LED is also off. it's a power problem. is the USB cable a *charging* cable instead of a *data* cable? cheap micro-USB cables sometimes only have power lines wired. try a different cable.

**nano blinks but LEDs are dark.**
the strip isn't getting data. check D6 → resistor → DIN. a common mistake is wiring to **DOUT** (output) instead of **DIN** (input) on the strip. DIN is on the side that the arrow on the strip points *away from*.

**LEDs flicker, glitch, or show wrong colours.**
this is almost always a grounding or capacitor issue. confirm:
- arduino GND and strip GND are tied together (a single common ground)
- the 1000µF capacitor is physically close to the strip, not back at the breadboard
- the data wire from D6 to the strip is short (under 30cm if possible)

**only the first LED lights up.**
the data signal is reaching pixel one but failing to propagate. check the solder joint or jumper on the strip's DIN. shorten the data wire. confirm 330Ω is in series, not in parallel.

**upload fails: "avrdude: stk500_recv()"**
this is the bootloader handshake failing. try **tools → processor → ATmega328P (Old Bootloader)**. cheap nano clones ship with the older bootloader and the IDE defaults to the new one.

**strip flashes white at the moment of power-on.**
this is expected. the data line is undefined for a brief moment before the firmware begins driving it. the 1000µF cap minimises it but cannot eliminate it. it will not damage anything.

once phase 1 is solid — meaning you can power-cycle the nano three or four times in a row and always get the same calm-blue breathing baseline — move on to phase 2.

---

# phase 2 — adding the piezo tap sensor

> goal: when you tap the sensor, the shoe responds. tap → warm flash → eased decay back to calm.

phase 2 introduces the input side of the interaction loop. you keep everything from phase 1 wired exactly as it was, then add the piezo sensor on top.

## phase 2 wiring (additions only — keep the LED wiring from phase 1)

```
            ┌─────────────────┐
            │  PIEZO DISC     │
            │   (27mm)        │
            │                 │
            │  + (red)  ──────┤────────────► A0
            │                 │              │
            │  - (black) ─────┤────────────► GND
            │                 │              │
            └─────────────────┘              │
                                             │
                              2.2MΩ resistor │
                              ┌──────────────┘
                              │
                              ▼
                             GND
```

**plain language:**
- the **red lead** of the piezo goes to **A0** (an analog input pin)
- the **black lead** of the piezo goes to **GND**
- the **2.2MΩ resistor** sits between **A0 and GND** in parallel with the piezo

**why the 2.2MΩ resistor?** a piezo, when undisturbed, can hold a lingering charge that pushes the analog reading high and noisy. the resistor "bleeds" that charge to ground slowly, so the resting reading sits cleanly near zero. without it, you'll see drifting baseline values that make threshold tuning miserable.

> 💡 if you have it on hand, you can also add two back-to-back 1N4148 diodes across A0 and GND. they clamp very large piezo voltage spikes and protect the analog pin. it's not strictly required for tapping with a finger, but it's a nice safety margin if you plan to actually stomp.

## step 1 — tune your threshold (use the debug sketch)

before flashing the integrated sketch, you need to know **what tap looks like on your hardware**. every piezo and every mounting surface produces different numbers.

1. open `firmware/experiments/piezo_debug/piezo_debug.ino`
2. upload it
3. open **tools → serial monitor**, set baud to **115200**
4. better: open **tools → serial plotter** for a live graph

now tap the sensor with your finger.

**what you should see:**
- at rest: numbers near zero (typically 0–5)
- a clear finger tap: a sharp spike, often somewhere between 200 and 800
- a hard tap: a bigger spike, sometimes hitting the 1023 ceiling
- between taps: the value comes back to near zero within ~50–150ms

**what you should write down:**
- your typical resting value (call it `noise_floor`)
- your typical light-tap peak (call it `light_tap`)
- your typical accidental-handling peak (sliding a finger across, brushing the wire) (call it `noise_peak`)

a good threshold sits **above `noise_peak` but below `light_tap`**. usually somewhere in the 100–200 range. write this number down — it goes into the next sketch.

> 🔧 if your at-rest value is high (above 50) or visibly noisy on the plotter, the **2.2MΩ bleed resistor is missing or wired wrong**. check that before tuning further. this is the #1 cause of bad piezo behaviour.

## step 2 — flash the integrated sketch

1. open `firmware/main/main.ino`
2. find this line near the top:

   ```cpp
   #define PIEZO_TAP_THRESHOLD    120
   ```
3. replace `120` with the threshold you wrote down in step 1
4. upload

## what you should see

- on boot: same soft fade-up to warm white as phase 1
- settled: same calm-blue breathing baseline as phase 1
- when you tap the piezo: the strip flashes to a warm amber, then eases smoothly back into the calm-blue baseline over about 600ms

every intentional tap should produce one — and only one — flash. that's the system feeling your gesture.

## what can go wrong (and how to fix it)

**no response to taps at all.**
- check the piezo wiring polarity: red to A0, black to GND
- open the serial monitor at 115200 and look for `tap: <value>` lines when you tap. if you see nothing, the threshold is too high — lower `PIEZO_TAP_THRESHOLD` and re-upload.
- if you still see nothing, go back to the debug sketch and verify the piezo is producing readable spikes at all. a cracked piezo or a broken solder joint at the disc edge is a common silent failure.

**every tap fires multiple flashes in quick succession.**
the piezo is "ringing" — its physical structure vibrates after a hit and produces decaying after-shock spikes. that's why the sketch enforces a `PIEZO_DEBOUNCE_MS` window of 180ms. if you're still seeing multi-fires, raise it to 250ms.

**it fires on its own with no tap.**
two possibilities:
- the threshold is too low. raise `PIEZO_TAP_THRESHOLD` until ambient noise no longer crosses it.
- the **2.2MΩ resistor is missing or loose**. without it the piezo's resting charge drifts, and any vibration nearby (your phone buzzing, a slammed door, footsteps in the room) registers.

**it fires when i walk but not when i tap.**
this is the classic walking-noise problem. you've found one of the central design challenges of the project — phase 4 (interaction intelligence) is where we filter walking impacts out using timing windows and gesture interpretation. for now, accept the false-triggers and note the patterns you observe. they are useful data.

**the flash is too short / too bright / wrong colour.**
all tunable. inside `main.ino`:
- `TAP_FLASH_DURATION_MS` — how long the response lasts
- `TAP_COLOR` — the colour of the flash (try `CRGB(255, 80, 30)` for a deeper amber, or `CRGB(220, 60, 200)` for a magenta)
- `MAX_BRIGHTNESS` — overall brightness ceiling

try one change at a time so you can feel its effect.

---

---

# phase 3 — bringing all six emotional modes online

> goal: see every mode work, switch between them, and check that each one is emotionally readable.

phase 3 is mostly a "flash and watch" experience. there is no new wiring — phase 2's hardware setup carries forward unchanged. what changed is the firmware: `firmware/main/main.ino` now renders **six** emotional modes instead of just calm-blue, and switching between them is what taps do.

the firmware was also reorganised into a small multi-file Arduino sketch:

```
firmware/main/
├── main.ino          entry point — boot, gesture dispatch, render loop
├── config.h          all tuneable constants (pins, brightness, timings)
├── animations.h      breathe, pulse, blend, gamma — shared primitives
├── gestures.h        piezo → tap → gesture event classifier
└── modes.h           the six emotional modes
```

the arduino IDE compiles all of these together automatically when you open `main.ino`. nothing extra to install.

## upload phase 3

1. open `firmware/main/main.ino`
2. confirm board = **Arduino Nano**, port selected, library installed (FastLED, EEPROM)
3. press **upload**

## what you should see

- soft fade-up at boot (same as before)
- the shoe enters the **last mode it was in** before power-down (phase 4 EEPROM memory)
- on a fresh nano with no saved state, it boots into **calm**

then you tap.

| if you do this    | this happens                                               |
| ----------------- | ---------------------------------------------------------- |
| single tap        | advances to the next mode (calm → happy → alert → ...)     |
| double tap (fast) | steps back one mode                                        |
| many rapid taps   | the system suspects walking and ignores taps for ~1 second |

mode changes cross-fade smoothly over about 600ms. nothing should ever feel jarring.

> the firmware also responds to **triple-tap** and dims itself when left alone. those are phase 6 features — explained below. you'll see them on the same upload; you don't need to flash anything new.

## checking each mode is right

flash the firmware, then cycle through every mode and look at it. record what you see in **[`validate.md`](./validate.md)** under the **phase 3** section. there is a specific item there for showing each mode to **an outsider** and asking them what feeling it gives them — that test is the most important one in the whole phase, because emotional readability is the bar the project exists to meet. if a mode confuses people, the mode needs work.

## what can go wrong

**mode never changes when i tap.**
the gesture system intentionally waits ~400ms after a tap to see if a second tap is coming (so it can distinguish single from double). if you tap once and don't see a mode change for almost half a second, that is by design. if it never changes, your `PIEZO_TAP_THRESHOLD` is probably too high — re-run the debug sketch and check.

**every tap fires multiple mode changes.**
ring-out — the piezo's natural mechanical bounce after a hit looks like a second tap. open `firmware/main/config.h` and raise `PIEZO_DEBOUNCE_MS` from 180 to 250.

**mode changes randomly when i'm just holding the shoe.**
something is jostling the piezo. check the wiring is clean and the bleed resistor is in place. if you cup the shoe in your hand and stand still, it should be silent. if it isn't, there's noise getting in.

**double-tap doesn't register, only single-taps fire.**
your two taps are arriving more than 400ms apart. either tap faster, or raise `DOUBLE_TAP_WINDOW_MS` in `config.h` (try 500–600ms).

**the strip flickers during transitions.**
you may be running out of arduino SRAM — the cross-fade uses two scratch buffers in addition to the live one. if your `LED_COUNT` is far above 30, lower it or skip the cross-fade. log the issue in the changelog.

---

# phase 4 — the gesture intelligence layer

phase 4 is what turns "the shoe felt a tap" into "the shoe understood what you meant." it is implemented inside `firmware/main/gestures.h` and is already running as part of the phase 3 firmware you just uploaded.

> there is **no separate sketch to flash** for phase 4. the same `main.ino` runs both. phase 4 is the *logic* that turns piezo events into mode changes.

## what phase 4 actually does

three small jobs, all in `gestures.h`:

1. **tap classification.** waits a short window after each tap to see if a second arrives. if yes → double-tap. if no → single-tap.
2. **walking-noise muting.** if more than three taps arrive within 2 seconds, the system assumes you're walking and ignores all gestures for the next second. once things quiet down, gestures resume.
3. **mode persistence.** every time the mode changes, the new mode is written to EEPROM. on the next power-on the shoe boots back into that mode rather than always starting over from calm.

these are deliberately simple. real noise rejection (telling intentional taps from heel-strikes during walking) is a research-grade problem and the techniques range from rolling-buffer pattern matching to small ML models. for now, the budget approach is honest about what it can and can't do.

## tuning phase 4

inside `firmware/main/config.h`:

```cpp
#define DOUBLE_TAP_WINDOW_MS   400   // how long to wait for a second tap
#define WALKING_TAP_BUDGET     3     // taps allowed within...
#define WALKING_WINDOW_MS      2000  // ...this many ms before walking-mute kicks in
#define WALKING_QUIET_MS       1000  // how long the mute lasts
```

most users won't need to change these. but: log in **[`validate.md`](./validate.md)** what you observe on real walking, and adjust if the defaults feel wrong on your gait or surface.

## what to validate

every phase 4 check sits in **[`validate.md`](./validate.md)** under "phase 4 — gesture intelligence". the most important items:

- single-tap success rate over 10 deliberate taps
- double-tap success rate over 10 deliberate double-taps
- walking on hard floor for 10 steps — does walking-mute kick in correctly?
- power-cycle test — does the shoe boot back into the mode you left it in?

---

# phase 5 — wearable integration

> goal: take everything off the breadboard and put it on a foot.

phase 5 is where the project stops being "an electronics prototype" and starts being "a wearable." there is no new firmware. there are decisions about mounting, materials, comfort, balance, safety — and a lot of small physical iterations.

## the high-level recipe

1. **piezo** — slips under the heel insole, with the wires routed along the inside of the shoe wall and tucked behind the heel module. the bleed resistor goes inside the heel module enclosure, not loose under the foot.
2. **LED strip** — runs along the outer sole edge, oriented so the LEDs face outward. ideally protected by a thin diffuser (frosted silicone tube, translucent fabric, or a 3D-printed channel).
3. **heel module** — a small enclosure on the back of the heel that houses the arduino nano + battery. removable for maintenance. accessible switch on the side.
4. **wires** — short, insulated, routed away from flex zones. every solder joint heat-shrunk before the shoe goes on a foot.

> ⚠️ before any walking test: **the battery must live in the external heel module, never under any compression zone.** this is non-negotiable. lithium cells under repeated foot-pressure are dangerous.

## how to work through phase 5

phase 5 is checklist-driven, not "flash and see." open **[`validate.md`](./validate.md)** under "phase 5 — wearable integration" and walk down the list. it is structured to push you through:

- piezo placement — does it still detect taps once it's under the insole?
- LED placement — does it survive the foot bending?
- heel module — does it press into your skin?
- walking comfort tests — 5 minutes, 15 minutes, 30 minutes, 1 hour
- weight balance — does the modded shoe feel asymmetric?
- safety verification — every check is non-negotiable

each item has space for notes, photos, and decisions. iteration is expected. the first heel module you build will probably be wrong. the second one will probably also be wrong. that's normal — the iteration is the project, not a failure to plan.

## what to capture for the portfolio

phase 5 is the visually richest phase so far. **[`validate.md`](./validate.md)** lists specific photos and videos to capture: piezo under insole (insole removed), LED routing, heel module open, heel module closed on foot, short walking video. these become primary portfolio material.

---

# phase 6 — emotional communication layer

> goal: give the shoe an inner life. it should feel like it has moods, attention, and self-restraint — not just a gesture-driven mode picker.

phase 6 is firmware-only. there is no new wiring. the same `firmware/main/main.ino` you flashed for phase 3 / 4 already contains it. flashing the latest from this repo gives you all of it.

## what's new in phase 6

three layers, each subtle but meaningful:

### 1. idle dim-down (the shoe gets sleepy when ignored)

after **30 seconds** with no taps, the shoe begins fading toward a dimmer "idle" brightness. by the **2 minute** mark, it has fully settled into a low-energy ambient state at about 20% brightness. it never goes off — the emotional mode stays the same, just quieter, like the shoe is at rest.

any classified gesture (single, double, or triple tap) instantly wakes it back to full brightness.

this matters because:
- a shoe that always blasts at 100% looks like a toy
- a shoe that gracefully recedes when ignored looks like it has presence
- the wearer doesn't have to "turn off" their shoe — it knows when to fade

### 2. public / private toggle (triple-tap)

three taps within ~800ms total → the shoe toggles between **public** and **private** mode.

| mode    | behaviour                                                                |
| ------- | ------------------------------------------------------------------------ |
| public  | full expressive output (default)                                         |
| private | scaled to ~50% brightness, otherwise unchanged                           |

private isn't "off" — it's "subtle." the same emotional content, dimmed enough to be personal rather than performative. on a date, in a meeting, at a quiet dinner: triple-tap and the shoe whispers instead of speaks.

the private/public state is **persisted in EEPROM**, like the current mode. power-cycle the shoe and it boots back into whichever signaling level you left it in.

### 3. boot fade respects everything

when the shoe boots, the soft fade-up at startup now composes with idle and private. so a private shoe that boots up at 2 minutes since last interaction will fade in *to its private deep-idle level*, not blast to full brightness and then settle. the entrance is consistent.

## the full gesture cheat-sheet (phase 6 onward)

| gesture          | effect                                                                |
| ---------------- | --------------------------------------------------------------------- |
| single tap       | advance to the next emotional mode                                    |
| double tap       | step back to the previous mode                                        |
| triple tap       | toggle public / private (subtle, half-brightness)                     |
| many rapid taps  | walking detected, gestures muted for ~1 second                        |
| no tap for 30s   | begin fading toward dim-idle                                          |
| no tap for 2min  | settled at deep-idle (~20% brightness)                                |
| any classified gesture during idle | instant wake to full brightness                     |

## tuning phase 6 in `config.h`

```cpp
#define IDLE_THRESHOLD_MS           30000     // 30s -> begin idle fade
#define DEEP_IDLE_THRESHOLD_MS      120000    // 2min -> settled deep-idle
#define DEEP_IDLE_BRIGHTNESS_SCALE  50        // ~20% of full brightness
#define PRIVATE_BRIGHTNESS_SCALE    128       // 50% scaling in private mode
```

if "30 seconds" feels too aggressive (the shoe dims while you're still wearing it actively), raise `IDLE_THRESHOLD_MS`. if private mode at 50% feels like too much (or too little) of a dim, change `PRIVATE_BRIGHTNESS_SCALE` (255 = no dim, 64 = quarter, etc).

## what to validate

every phase 6 check sits in **[`validate.md`](./validate.md)** under "phase 6 — emotional communication layer". the most important items:

- triple-tap success rate over 10 attempts
- visual difference between public and private is meaningful but not jarring
- idle dim-down is graceful at the 30s and 2min thresholds
- private state correctly persists across power-cycles
- watching the shoe for 5 uninterrupted minutes feels calming, not boring

## what can go wrong

**triple-tap never fires.**
your three taps are too slow. a triple-tap needs all three within roughly 800ms total (each gap ≤ `MULTI_TAP_WINDOW_MS` = 400ms). speed it up, or raise `MULTI_TAP_WINDOW_MS` in `config.h` (try 500–600ms).

**triple-tap fires when i intended a double-tap.**
likely the third spike is piezo ring-out from the second tap. raise `PIEZO_DEBOUNCE_MS` from 180 to 220–250ms to suppress the post-tap aftershocks.

**the shoe never dims.**
either you're triggering taps inadvertently every few seconds, or the walking-mute is misclassifying ambient vibration as taps. check the serial monitor — every classified tap prints `mode -> ...` or `private = ...`. if you see those at random, the threshold is too low. raise `PIEZO_TAP_THRESHOLD`.

**the shoe dims even though i'm wearing it actively.**
phase 6 dim-down is based on **classified gestures**, not motion. if you're walking around without tapping, idle decay still kicks in. that's by design — walking alone doesn't reset interaction. if you want it to, that's a phase 7+ feature (motion-aware idle reset via accelerometer). log it as an observation in `validate.md` if it bothers you in real wear.

**private mode looks too dim / not dim enough.**
tune `PRIVATE_BRIGHTNESS_SCALE` in `config.h`. the test is honest: turn private on, walk into a dim social setting, and ask yourself "would i be comfortable wearing this here?" — if the answer is no, dim it more.

---

# phase 7 — safety & reliability

> goal: stop being a prototype, start being a thing you can wear every day without worrying.

phase 7 is **entirely physical**. there is no firmware deliverable. it is a focused round of stress, durability, and safety testing on whatever you assembled in phase 5. nothing here is glamorous — it is the unglamorous work that decides whether the project is ready to leave the workbench.

## what phase 7 actually checks

| concern                | what to test                                                            |
| ---------------------- | ----------------------------------------------------------------------- |
| battery safety         | full discharge cycle without overheating; voltage stable under load     |
| electrical isolation   | no exposed conductors anywhere on the shoe interior                     |
| wire strain            | flex test at every bend point — does the wire survive 100+ cycles?      |
| solder joint integrity | tug test on every joint; nothing should pull free                       |
| enclosure robustness   | the heel module survives ordinary walking impacts                       |
| thermal behaviour      | run cool-mode (highest current) for 10–20 min, check for hot spots      |
| grounding under flex   | the data line stays clean while the shoe bends during a real walk      |
| inline fuse decision   | do you add a small fuse on the 5V rail? document the call               |

## why this matters

a wearable that fails safely is a project. a wearable that fails dangerously is a liability. lithium cells under foot-pressure can vent. a shorted 5V rail can fry the nano. an exposed conductor inside a sweaty shoe is unpleasant at best.

every item in this phase is a "until i've checked this i don't actually know if the shoe is safe" item. you cannot reason your way to safety from the firmware side; you have to measure it.

## how to work through phase 7

phase 7 is checklist-driven. open **[`validate.md`](./validate.md)** under "phase 7 — safety & reliability" and walk through it. the structure mirrors phase 5: every item has a checkbox plus space for measurements, photos, and decisions.

a few items in phase 7 are **non-negotiable** — they're flagged in the checklist. these include:

- battery in external module, never compression zone
- no exposed conductors on the interior
- power can be killed instantly via the external switch
- no overheating after 20 min continuous run

if any non-negotiable item fails, the shoe doesn't get worn until it's fixed.

## what to capture for the portfolio

- thermal photos (phone IR camera if you have one, or simply "hold your hand over it and note warm spots")
- a flex-test video (bend the shoe back and forth 100 times, then test the LED strip — proof it survives)
- a "before/after" photo of any reinforcement you add (cable ties, hot glue, heat-shrink)
- a written paragraph for each non-negotiable check, explaining what you tested and how

phase 7 generates the most boring-looking portfolio content — and the most credible. real product engineering is mostly this kind of work.

---

# phase 8 — aesthetic refinement

> goal: take a thing that *works* and make it a thing that *looks intentional*.

phase 8 is also mostly physical. there are some **small firmware tweaks possible** (palette adjustments, animation timing curve refinement) but the heart of this phase is decisions about how the shoe looks and feels, not how the code is structured.

## what phase 8 actually addresses

| concern              | what you decide                                                         |
| -------------------- | ----------------------------------------------------------------------- |
| enclosure design     | material, finish, shape, removable vs sealed                            |
| LED diffusion        | bare LEDs vs frosted silicone tube vs translucent fabric overlay        |
| visible electronics  | hide the wires, or show them as intentional design language?            |
| left/right symmetry  | (if dual shoe) — must look like a pair, not two prototypes              |
| visual identity      | colour palette of the enclosure, finish, branding (or its absence)      |
| animation polish     | subtle timing-curve adjustments to make modes feel more "right"         |

these are all judgment calls. there is no objectively correct answer. what makes phase 8 a **phase** rather than just "tweaks" is that you commit to the decisions and document them, so the project reads as intentional.

## the firmware tweaks worth considering

if you find a mode just doesn't *feel* right after wearing the shoe for a while, here's where to reach into `firmware/main/`:

- **`config.h`** — every mode's period, brightness ceiling, and timing constant. a mode that feels "too fast" usually wants its `*_PERIOD_MS` raised by 20–30%.
- **`modes.h`** — the actual colours each mode uses. shift the palette by adjusting `CRGB(r, g, b)` values. a happy that feels "too orange" can become slightly more yellow with `CRGB(255, 170, 30)`.
- **`animations.h`** — the easing curves themselves. if all modes feel mechanical, `smoothstep` might want replacing with a softer curve like `t * t * t * (t * (t * 6 - 15) + 10)` (smootherstep).

document any tweak you make in **[`validate.md`](./validate.md)** under "phase 8" — what you changed, why, and what it felt like before vs after.

## diffusion experiments

unfiltered WS2812B looks aggressive. some options to try, in roughly increasing effort:

1. **frosted silicone tube** — sleeve the strip inside; the LEDs become a continuous glow rather than discrete dots
2. **translucent fabric overlay** — softer, more "garment-like" look; harder to mount cleanly
3. **3D-printed channel with diffuser top** — most controllable, requires CAD time
4. **bare LEDs** — sometimes the right choice for a tech-forward aesthetic; commit to it visibly rather than apologetically

phase 8's validate.md section walks through capturing comparison photos for each diffusion option you try. those comparison shots are excellent portfolio material.

## what to validate

phase 8 is the most subjective phase. the validate.md checklist is mostly observation prompts and decision-logging:

- which diffusion approach won, and why
- whether the visible electronics aesthetic feels intentional or improvised
- whether the modes still read emotionally after the visual treatment
- whether left and right shoes look like a designed pair (if dual)

honest answers here matter more than ticked boxes. a "no, this still looks improvised" with notes is more useful than a "yes" with nothing under it.

---

## a note on safety

this is a wearable. the battery sits on a person's foot. that means a few rules are non-negotiable:

- the battery lives in an **external heel module**, never under a compression zone.
- all solder joints are heat-shrunk before any walking test.
- there are no exposed conductors on the inside of the shoe.
- power is switched externally and accessibly — the user must be able to kill power instantly.

if any of these cannot be guaranteed, the shoe does not get worn. simple as that.

---

## why this project exists

most wearables shout. this one is trying to whisper. it is an experiment in whether technology, when given a quiet enough voice, can carry something honest about how a person is feeling — without forcing them to type, speak, or perform.

the answer is still being worked out. the changelog is the working notebook.

---

## license

not yet decided. for now, this is a personal experimental project. licensing will be added when the system is mature enough to be useful to others.
