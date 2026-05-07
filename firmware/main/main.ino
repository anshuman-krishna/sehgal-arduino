// sehgal-arduino — current state (phase 3 + 4 + 6 integrated)
//
// what this sketch does:
//   - boots into the last emotional mode + public/private state (EEPROM)
//   - renders one of six modes: calm, happy, alert, sleepy, flow, cool
//   - listens for piezo taps, classifies them as single / double / triple / noise
//   - single-tap   -> next mode
//   - double-tap   -> previous mode
//   - triple-tap   -> toggle public / private (private = ~half brightness)
//   - rapid taps   -> walking detected, gestures muted briefly
//   - mode changes cross-fade smoothly over MODE_TRANSITION_MS
//   - 30s of no interaction -> begins fading to deep-idle (~20% brightness)
//   - 2 min of no interaction -> settled deep-idle. shoe stays alive but quiet.
//   - any classified gesture -> instant wake, full brightness restored
//
// architecture (header-only multi-file Arduino sketch):
//   config.h       all tuneable constants
//   animations.h   shared lighting primitives (breathe, pulse, blend, gamma)
//   gestures.h     piezo -> gesture event classifier
//   modes.h        the six emotional modes

#include <FastLED.h>
#include <EEPROM.h>

#include "config.h"
#include "animations.h"
#include "gestures.h"
#include "modes.h"

// ---- LED buffers ------------------------------------------------------------
// three buffers: the live one + two scratch buffers used during cross-fades
// (one rendering the previous mode, one rendering the next, blended into leds).

CRGB leds[LED_COUNT];
CRGB modeBufA[LED_COUNT];
CRGB modeBufB[LED_COUNT];

// ---- system state -----------------------------------------------------------

unsigned long bootMillis           = 0;
EmotionalMode currentMode          = MODE_CALM;
EmotionalMode prevMode             = MODE_CALM;
unsigned long modeTransitionStart  = 0;
bool          inTransition         = false;

// phase 6: idle + public/private state
bool          isPrivate            = false;
unsigned long lastInteractionMs    = 0;

// ---- EEPROM helpers ---------------------------------------------------------

static inline void persistMode(EmotionalMode m) {
  EEPROM.update(EEPROM_MODE_ADDR, (uint8_t)m);
  EEPROM.update(EEPROM_MAGIC_ADDR, (uint8_t)EEPROM_MAGIC_VALUE);
}

static inline void persistPrivate(bool p) {
  EEPROM.update(EEPROM_PRIVATE_ADDR, (uint8_t)(p ? 1 : 0));
  EEPROM.update(EEPROM_MAGIC_ADDR, (uint8_t)EEPROM_MAGIC_VALUE);
}

static inline EmotionalMode loadMode() {
  // first-boot guard: if EEPROM has never been written, start in calm.
  if (EEPROM.read(EEPROM_MAGIC_ADDR) != EEPROM_MAGIC_VALUE) return MODE_CALM;
  uint8_t saved = EEPROM.read(EEPROM_MODE_ADDR);
  if (saved >= MODE_COUNT) return MODE_CALM;
  return (EmotionalMode)saved;
}

static inline bool loadPrivate() {
  if (EEPROM.read(EEPROM_MAGIC_ADDR) != EEPROM_MAGIC_VALUE) return false;
  return EEPROM.read(EEPROM_PRIVATE_ADDR) != 0;
}

// ---- mode change ------------------------------------------------------------

static inline void changeMode(EmotionalMode newMode, unsigned long now) {
  if (newMode == currentMode) return;
  prevMode            = currentMode;
  currentMode         = newMode;
  modeTransitionStart = now;
  inTransition        = true;
  persistMode(currentMode);
  Serial.print(F("mode -> "));
  Serial.println(modeName(currentMode));
}

// ---- gesture handling -------------------------------------------------------

static inline void handleGesture(GestureEvent g, unsigned long now) {
  if (g == GESTURE_NONE) return;
  if (g == GESTURE_NOISE) {
    Serial.println(F("walking noise -> gestures muted"));
    return;
  }

  // any classified gesture wakes the shoe from idle
  lastInteractionMs = now;

  if (g == GESTURE_SINGLE_TAP) {
    changeMode(modeNext(currentMode), now);
  } else if (g == GESTURE_DOUBLE_TAP) {
    changeMode(modePrev(currentMode), now);
  } else if (g == GESTURE_TRIPLE_TAP) {
    isPrivate = !isPrivate;
    persistPrivate(isPrivate);
    Serial.print(F("private = "));
    Serial.println(isPrivate ? F("on") : F("off"));
  }
}

// ---- brightness scaling -----------------------------------------------------
//
// returns a final 0-255 multiplier applied uniformly to the rendered frame.
// composes three contributions:
//   - boot fade-up scale (only during first WAKE_DURATION_MS)
//   - idle decay (settles toward DEEP_IDLE_BRIGHTNESS_SCALE after IDLE_THRESHOLD_MS)
//   - private-mode dim (PRIVATE_BRIGHTNESS_SCALE if isPrivate)

static inline uint8_t computeBrightnessScale(unsigned long now, unsigned long sinceBoot) {
  // idle component
  unsigned long sinceInteraction = now - lastInteractionMs;
  uint8_t idleScale;
  if (sinceInteraction < IDLE_THRESHOLD_MS) {
    idleScale = 255;
  } else if (sinceInteraction < DEEP_IDLE_THRESHOLD_MS) {
    float t = (float)(sinceInteraction - IDLE_THRESHOLD_MS) /
              (float)(DEEP_IDLE_THRESHOLD_MS - IDLE_THRESHOLD_MS);
    float eased = smoothstep(t);
    idleScale = (uint8_t)(255 - eased * (255 - DEEP_IDLE_BRIGHTNESS_SCALE));
  } else {
    idleScale = DEEP_IDLE_BRIGHTNESS_SCALE;
  }

  // private-mode component
  uint8_t privateScale = isPrivate ? PRIVATE_BRIGHTNESS_SCALE : 255;

  // boot fade component (overrides idle while booting)
  uint8_t bootScale = 255;
  if (sinceBoot < WAKE_DURATION_MS) {
    float t = (float)sinceBoot / (float)WAKE_DURATION_MS;
    bootScale = (uint8_t)(smoothstep(t) * 255);
  }

  // multiplicative composition: each scale is 0-255 representing 0..1
  uint16_t s1 = (uint16_t)idleScale * privateScale / 255;
  uint16_t s2 = s1 * bootScale / 255;
  return (uint8_t)s2;
}

// ---- arduino lifecycle ------------------------------------------------------

void setup() {
  // small wait so the 1000uF capacitor settles before we drive the data line
  delay(50);

  FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.setBrightness(255);   // each mode bakes its own brightness via nscale8
  FastLED.clear(true);

  Serial.begin(115200);
  Serial.println(F("sehgal-arduino phase 3+4+6 boot"));

  currentMode = loadMode();
  prevMode    = currentMode;
  isPrivate   = loadPrivate();
  Serial.print(F("starting mode: "));
  Serial.println(modeName(currentMode));
  Serial.print(F("private = "));
  Serial.println(isPrivate ? F("on") : F("off"));

  bootMillis        = millis();
  lastInteractionMs = bootMillis;   // count boot as fresh interaction
}

void loop() {
  unsigned long now = millis();
  unsigned long sinceBoot = now - bootMillis;

  // ---- gestures (only after boot fade) -----------------------------------
  if (sinceBoot >= WAKE_DURATION_MS) {
    GestureEvent g = gestureUpdate(now);
    handleGesture(g, now);
  }

  // ---- render ------------------------------------------------------------
  if (inTransition) {
    unsigned long sinceTransition = now - modeTransitionStart;
    if (sinceTransition >= MODE_TRANSITION_MS) {
      inTransition = false;
      modeRender(currentMode, now, leds);
    } else {
      modeRender(prevMode,    now, modeBufA);
      modeRender(currentMode, now, modeBufB);
      float t = (float)sinceTransition / (float)MODE_TRANSITION_MS;
      float eased = smoothstep(t);
      for (uint16_t i = 0; i < LED_COUNT; i++) {
        leds[i] = blendColor(modeBufA[i], modeBufB[i], eased);
      }
    }
  } else {
    modeRender(currentMode, now, leds);
  }

  // ---- apply final brightness scale --------------------------------------
  // composes boot fade + idle decay + private dim into one multiplier and
  // applies it once across the whole strip.
  uint8_t finalScale = computeBrightnessScale(now, sinceBoot);
  for (uint16_t i = 0; i < LED_COUNT; i++) {
    leds[i].nscale8(finalScale);
  }

  FastLED.show();
  delay(16);  // ~60 fps. leaves plenty of headroom for sensor work.
}
