# adaptive brightness (phase 10 design proposal)

> the shoe scales its output to ambient light. brighter in sun, gentler in dim rooms, never inappropriately loud or invisible.

## what it adds

right now `MAX_BRIGHTNESS = 120` (in 0-255 scale). that's a single global ceiling. it's a compromise: bright enough to be readable in most rooms, low enough to not be obnoxious in a dim bar.

adaptive brightness replaces that compromise. the shoe samples ambient light and scales itself appropriately:

- bright sunlight → ceiling rises to 180-200 (still readable outdoors)
- normal indoor lighting → ceiling stays around 120 (current default)
- dim restaurant / evening / sleeping room → ceiling drops to 40-60 (visible but not harsh)

the wearer never has to think about it. the shoe just behaves correctly across contexts.

## why it's interesting

this is the unsexy expansion that makes the project genuinely wearable. the current shoe in a dimly-lit restaurant feels too loud; the same shoe outdoors in sunlight feels invisible. neither is the shoe's fault — it's the lack of context awareness.

adaptive brightness also opens an interaction: the wearer can use **shadow** as input. cup your hand over the shoe and it brightens (it thinks you've moved into a darker context). a small but charming feedback loop that costs nothing extra to add.

## parts list

- 1 × LDR (photoresistor) — small, ~$0.50
- 1 × 10kΩ resistor (the bottom half of a voltage divider)
- 2 × jumper wires

that's it. cheapest expansion in this folder by an order of magnitude.

## architecture

### the circuit

a classic voltage divider on an analog pin:

```
    5V ────┬────────────┐
           │            │
          LDR          (top half — variable, low in light, high in dark)
           │            │
           ├────────────► A1  (the analog input)
           │
          10kΩ
           │
   GND ────┘
```

bright light → LDR resistance drops → voltage at A1 rises → analogRead returns a high number.
dim light → LDR resistance rises → voltage at A1 drops → analogRead returns a low number.

### data flow

```
   LDR voltage divider
        │
        ▼ A1
   firmware/main/ambient.h
        ├── ambientUpdate()              // poll @ ~5Hz
        ├── ambientLightLevel()          // returns smoothed 0..1
        ├── ambientPreferredCeiling()    // maps light level to a brightness ceiling
```

main.ino reads `ambientPreferredCeiling()` once per second (changes slowly) and uses it as the working `MAX_BRIGHTNESS` for the next second.

### smoothing

raw LDR readings are noisy. need a rolling average (e.g., last 10 samples) plus an additional slow-following filter — when the wearer steps from a dim hallway into bright sunlight, the shoe should adapt over a few seconds, not instantly. instant adaptation feels mechanical; slow following feels alive.

```cpp
// pseudo-code
static float ambientSmoothed = 0.5f;
ambientSmoothed = 0.95f * ambientSmoothed + 0.05f * (rawSample / 1023.0f);
```

a 5% blend per sample at 5Hz means a ~4-second time constant. tweak to taste.

### config additions

```cpp
#define AMBIENT_PIN              A1
#define AMBIENT_SAMPLE_INTERVAL_MS 200
#define AMBIENT_SMOOTHING_ALPHA  0.05f
#define AMBIENT_BRIGHTNESS_MIN   40       // floor, even in pitch dark
#define AMBIENT_BRIGHTNESS_MAX   200      // ceiling, even in direct sun
```

`MAX_BRIGHTNESS` becomes a runtime variable rather than a compile-time constant.

## risks and open questions

- **LDR placement**: the LDR has to see ambient light, not the shoe's own LEDs. if mounted carelessly, the strip's output washes out the LDR's reading and creates a feedback loop (shoe gets brighter → LDR sees more light → shoe gets dimmer → loop). solution: mount the LDR pointing *up*, away from the strip, or shield it.
- **flex covering the LDR**: shoes flex while walking. if the LDR is mounted somewhere that gets covered by clothing or the foot's shadow during walking, it'll oscillate weirdly. test mounting locations.
- **what does "appropriate" actually mean?**: this is a design call. is "appropriate in a movie theatre" 5%? 1%? do we have a hard floor below which the shoe should go fully off? recommend never going off (consistent with the project's "never fully dark" principle).
- **shadow as input**: this is fun but might cause unintended dimming when the wearer crosses their legs, sits at a desk, etc. needs to be desirable, not intrusive.
- **calibration across devices**: different LDRs have different sensitivity. a hard-coded mapping might not generalize. consider an auto-calibration that learns "this is the brightest I've seen lately" and "this is the dimmest" over a few hours of wear.

## decision points

before building, you'd need to answer:

1. **mounting location**: top of the heel module, pointing up? or somewhere on the side? this is mostly an aesthetic/wearability call.
2. **time constant**: faster adaptation feels responsive but mechanical; slower feels alive but laggy. recommend ~4 seconds, validate in real wear.
3. **interact with private mode?** private already dims by 50%. should adaptive brightness compose with that, or override it? recommend compose — both signals are valid context.
4. **interact with idle decay?** idle and adaptive ambient both dim the shoe. they should compose multiplicatively, same as private. needs careful mental model so they don't double-dim into invisibility.
5. **auto-calibration vs hard-coded mapping**: hard-coded is simpler; auto-calibration is robust to LDR variation. recommend hard-coded for v1, auto-calibration as a follow-up if needed.

## one-paragraph elevator pitch

> add a single LDR and the shoe becomes context-aware. brighter in daylight, gentler at dusk, almost off in dim restaurants. the wearer never thinks about it; it just behaves correctly. and as a bonus, cupping your hand over the shoe makes it brighter — turning a hardware compromise into a small, charming interaction.
