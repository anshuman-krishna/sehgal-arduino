# haptic feedback (phase 10 design proposal)

> a small vibration motor in the heel module. each classified gesture gets a brief tactile confirmation, so the wearer feels the system register them.

## what it adds

today, gesture confirmation is purely visual — the wearer taps and waits for the cross-fade to begin. there's a ~400ms latency on single-tap (the multi-tap classification window). in real wear, that latency feels like uncertainty: *did the shoe register that, or do I need to tap again?*

a tiny vibration the moment a gesture is *classified* gives the wearer immediate, body-level confirmation:

- single-tap classified → one short pulse (~80ms)
- double-tap classified → two short pulses
- triple-tap classified → one longer pulse (~200ms)
- walking-noise mute engaged → no vibration (the absence is the signal)

the visual response still follows; the vibration just confirms "yes, the shoe heard you" before the cross-fade starts.

## why it's interesting

haptic confirmation is the cheapest possible UX upgrade in interaction design. it removes the largest source of wearable frustration — *did the device register my input?* — without adding a new screen, light, or sound.

it's also emotionally consistent with the project. visible LEDs are how the shoe talks to *others*. silent vibration is how it talks to *the wearer*. that split — public expression, private acknowledgement — gives the system a more complete sense of conversation.

## parts list

- 1 × small coin vibration motor (~10mm dia, 3V or 5V) — ~$1
- 1 × NPN transistor (2N2222 or similar)
- 1 × 1N4148 flyback diode (across the motor)
- 1 × 1kΩ resistor (transistor base current limit)
- some wire

that's it. classic motor-driving circuit, nothing exotic.

## architecture

### the circuit

```
                          5V (regulated)
                           │
                       ┌───┴───┐
                       │ motor │
                       │  (M)  │
                       └───┬───┘
                           │
                  ╲ │1N4148│
                   ╲│      │
                    │  (cathode at +5V end)
                           │
                           ├──────► collector
                                       │
                  ◄── 1kΩ ──── base   (NPN transistor)
                                       │
                                     emitter
                                       │
                                      GND

   gpio (e.g., D9, PWM) ─── 1kΩ ─── transistor base
```

the gpio drives the transistor, which switches the motor. the diode handles the back-EMF when the motor de-energises (without it, the transistor and the rest of the circuit see voltage spikes that can fry the nano).

D9 is a PWM pin on the nano, so we can vary vibration intensity if we want. start with full-on / full-off; PWM intensity becomes a tuning knob if needed.

### data flow

```
   gestures.h fires GESTURE_*
        │
        ▼
   main.ino: handleGesture()
        ├── visual response (mode change, private toggle, etc.)
        └── haptic.h: hapticForGesture(g)
                            │
                            ▼
              firmware/main/haptic.h
                ├── hapticBegin()                  // setup pin
                ├── hapticPulse(duration_ms)       // schedule a pulse
                └── hapticUpdate(now)              // non-blocking timing
```

`hapticPulse()` is non-blocking: it sets an end-time and `hapticUpdate()` (called once per loop) takes care of switching the motor on and off at the right moments.

### config additions

```cpp
#define HAPTIC_PIN                 9
#define HAPTIC_SHORT_PULSE_MS      80
#define HAPTIC_LONG_PULSE_MS       200
#define HAPTIC_GAP_MS              60      // gap between pulses in a sequence
#define HAPTIC_INTENSITY_PWM       180     // 0-255, only if using PWM
```

### gesture-to-haptic mapping

```cpp
inline void hapticForGesture(GestureEvent g) {
  switch (g) {
    case GESTURE_SINGLE_TAP:  hapticPulseSequence(1, HAPTIC_SHORT_PULSE_MS); break;
    case GESTURE_DOUBLE_TAP:  hapticPulseSequence(2, HAPTIC_SHORT_PULSE_MS); break;
    case GESTURE_TRIPLE_TAP:  hapticPulse(HAPTIC_LONG_PULSE_MS); break;
    case GESTURE_NOISE:       break; // intentionally silent
    default: break;
  }
}
```

### power consideration

vibration motors draw ~80-100mA when running. that's significant for a battery-powered wearable. the saving grace: pulses are short (≤200ms) and infrequent (only on classified gestures). average current draw stays negligible. but: the motor must be on a clean 5V rail with the nano well-decoupled, or you'll see voltage sag affecting the LEDs during pulses.

a separate small bypass cap (10µF electrolytic) near the motor helps a lot. add it to the circuit.

## risks and open questions

- **placement**: the motor needs to couple physically to the heel module so the wearer feels it. mounting it inside an enclosure that absorbs the vibration kills the effect. needs hot-glue / direct contact with the enclosure shell.
- **comfort**: too strong a vibration is irritating; too weak is invisible. test the chosen pulse duration in real wear, and have a way to disable it (config define or a long-press gesture once we have FSR support).
- **noise**: vibration motors are audible in quiet rooms. acceptable in most contexts, awkward in others (libraries, meetings). consider whether to suppress vibration in private mode (private = silent acknowledgement).
- **driving the motor directly from a gpio**: tempting but DON'T. nano gpios source ~20mA max; the motor wants 80-100mA. you'll pop the gpio. the transistor is non-negotiable.
- **flyback diode polarity**: anode toward GND-side, cathode toward +5V-side. wired backwards, it shorts the motor every cycle. very common mistake.

## decision points

before building, you'd need to answer:

1. **suppress haptic in private mode?** recommend yes — private is about subtlety, and a buzz from the heel undermines it.
2. **suppress haptic during walking-mute?** yes (already proposed — NOISE event maps to no haptic).
3. **distinct patterns for single vs double, or same pattern?** recommend distinct — the user learns the gesture vocabulary faster when they get a different tactile confirmation per gesture.
4. **pulse duration**: 80ms feels like "click confirmation." longer feels insistent. tune in real wear; 80-100ms is the right ballpark.
5. **PWM intensity or full-on?** start full-on. add PWM only if there's a real reason (e.g., context-aware intensity).

## one-paragraph elevator pitch

> a small motor in the heel gives the wearer a tactile confirmation every time the shoe registers a gesture. it removes the moment of uncertainty that otherwise sits between a tap and the visual response. visible LEDs are how the shoe talks to others; silent vibration is how it talks to you.
