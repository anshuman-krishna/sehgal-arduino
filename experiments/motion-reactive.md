# motion-reactive behaviour (phase 10 design proposal)

> the shoe responds to how the wearer is moving as well as what they tap.

## what it adds

the system today knows about taps and time. it has no signal for whether the wearer is sitting, standing, walking, or running. motion-reactive behaviour adds a small accelerometer and uses its data to *modulate* the existing emotional output, sitting on top of the modes rather than swapping them out.

three concrete behaviours fall out of this:

1. **stillness boost**: when the wearer has been still for >30 seconds, the shoe gradually intensifies its current mode (slightly brighter, slightly slower breathing), a kind of "I'm with you in stillness" amplification.
2. **walking taper**: during active walking, the brightness softens by ~30% so the shoe whispers instead of shouting while the wearer is in motion. also nudges happy and cool toward calmer timings.
3. **jump trigger**: a sharp upward acceleration spike (a deliberate jump or strong heel-strike) could trigger a one-time celebratory animation in any mode, like a surprise burst of expression.

these are *modulations* layered on top of the existing six modes. the wearer still picks the mode via tap. motion-reactive just makes the chosen mode sensitive to context.

## why it's interesting

right now the shoe reacts only to taps. that makes it a "command and execute" relationship. motion-reactive makes the shoe *aware*, closer to the difference between a remote-control toy and a cat.

a shoe that gets brighter when you sit still is the kind of small detail that makes the wearable feel alive without ever being pushy. a shoe that responds to a jump with a flash of celebration is genuinely playful. these are emotional capabilities the current system can't reach on its own.

## parts list

- 1 × MPU6050 accelerometer + gyro module (~$3): I2C, well-supported, good arduino libraries
  - alternative: ADXL345, slightly older, also fine, similar price
- 4 × jumper wires (VCC, GND, SDA, SCL)
- (no extra resistors needed; the MPU6050 has internal pull-ups)

## architecture

### data flow

```
   accelerometer (I2C @ A4/A5)
        │
        ▼
   firmware/main/motion.h
        ├── motionUpdate()           // poll @ ~50Hz
        ├── motionIsStill(t)         // bool, true if no significant motion for t ms
        ├── motionIsWalking()        // bool, rolling-buffer cadence detection
        └── motionImpactDetected()   // bool, fires once per sharp upward spike
        │
        ▼
   main.ino
        │
        │ uses these signals to:
        │   - boost brightness during stillness (multiplier 1.0 → 1.2)
        │   - reduce brightness + slow timing during walking (multiplier × 0.7)
        │   - trigger one-off celebration on impact (overlay animation)
        │
        ▼
   render pipeline (modes + brightness composer)
```

### module additions

new file: `firmware/main/motion.h`. responsibilities:
- I2C bring-up of MPU6050
- maintain a rolling buffer of the last ~1 second of acceleration magnitudes
- expose three boolean queries (`isStill`, `isWalking`, `impactDetected`)
- expose a continuous "motion intensity" 0-255 that other modules can read

main.ino reads these signals each frame and composes them into the existing brightness pipeline.

### config additions

```cpp
#define MOTION_PIN_SDA       A4
#define MOTION_PIN_SCL       A5
#define STILLNESS_BOOST_MS   30000
#define STILLNESS_BOOST_MAX  1.2f      // 120% brightness when fully still
#define WALKING_DAMP_FACTOR  0.7f      // 70% brightness during walking
#define IMPACT_ACCEL_THRESHOLD 2.5f    // g, above 2.5g of vertical accel = jump
#define IMPACT_ANIMATION_MS  800
```

### celebration animation

the impact-triggered animation is a small expressive flourish. proposed:
- two-frame "flash" of high-saturation colour matched to the current mode
- 300ms hold
- ease back into the mode's normal animation over 500ms

think of it as an overlay rather than a new mode. the shoe is still in (say) calm; it just briefly broke into a smile. it's emotional punctuation.

## risks and open questions

- **walking cadence detection is non-trivial**: a simple "high acceleration variance = walking" works most of the time but fails on (a) very slow walking, (b) gentle activity like light typing, (c) wearer carrying the shoe in hand. needs real-data tuning.
- **stillness vs sitting still vs sleeping**: should a wearer sleeping in their shoes get the stillness boost? probably not; that's where the existing idle dim-down should take over. the two systems need to defer to each other instead of fighting (idle dimming while stillness-boosting would be contradictory).
- **impact threshold tuning**: 2.5g is a starting guess. some wearers walk heavy, some land softly when jumping. needs the same kind of threshold-tuning workflow as the piezo did in phase 2, probably with a dedicated `motion_debug.ino` sketch.
- **power**: MPU6050 in low-power mode draws ~3.5mA. tolerable. fully active mode draws ~5mA. negligible compared to LED strip.
- **interaction with walking-noise mute**: if the accelerometer can detect walking, the gesture system could *trust* the accelerometer for walking detection rather than the budget-based piezo heuristic. better walking-noise rejection. consider as a follow-up.

## decision points

before building, you'd need to answer:

1. **does motion *replace* the budget-based walking mute, or augment it?** recommend augment. accelerometer gives a more reliable walking signal; piezo budget keeps it as a fallback.
2. **does the celebration animation pass through during walking?** small philosophical question. recommend yes: a deliberate jump while walking is a meaningful moment that deserves to land.
3. **stillness boost cap**: 1.2x is conservative. test in real wear before raising.
4. **does walking taper apply in private mode?** private already dims by 50%. additional walking dampening would push the shoe past invisible. recommendation: private mode skips walking taper, since private is already quiet enough.
5. **rolling-buffer length**: 1 second is enough for cadence detection but adds 50 bytes of state. acceptable on AVR. on ESP32 we could do more sophisticated sliding-window analysis.

## one-paragraph elevator pitch

> the shoe gets quieter when the wearer walks, brighter when they're still, and breaks into a small celebration when they jump. it turns a tap-driven wearable into one that's aware of context, present in a way the current system can't reach on its own, while still letting the wearer's chosen emotional mode sit on top.
