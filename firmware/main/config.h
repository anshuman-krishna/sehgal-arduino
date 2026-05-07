// firmware/main/config.h
//
// every tuneable knob in the system. if a number matters to behaviour, it
// lives here. nothing magical is scattered through the rest of the firmware.
//
// when something feels wrong on hardware (too bright, too dim, too jumpy,
// too slow), this is the file you change.

#pragma once

// ---- LED strip --------------------------------------------------------------

#define LED_DATA_PIN     6
#define LED_COUNT        30
#define LED_TYPE         WS2812B
#define COLOR_ORDER      GRB
#define MAX_BRIGHTNESS   120        // 0-255. wearables don't need 255.
#define WAKE_DURATION_MS 1500       // soft fade-up at boot

// ---- piezo / tap detection --------------------------------------------------
//
// PIEZO_TAP_THRESHOLD is the raw analogRead value that counts as a tap.
// tune via firmware/experiments/piezo_debug/piezo_debug.ino on YOUR hardware.

#define PIEZO_PIN              A0
#define PIEZO_TAP_THRESHOLD    120
#define PIEZO_DEBOUNCE_MS      180  // ignore re-triggers within this window (handles ring-out)

// ---- gesture interpretation -------------------------------------------------
//
// MULTI_TAP_WINDOW_MS:
//   max gap allowed between consecutive taps in a multi-tap sequence. after
//   this much silence, the pending tap count is committed as a gesture.
//   single, double, and triple all share this window.
//   side effect: single-tap fires AFTER this delay (we have to wait to confirm
//   "no second tap is coming"). triple-tap fires immediately on the 3rd tap.
//
// WALKING_TAP_BUDGET / WALKING_WINDOW_MS / WALKING_QUIET_MS:
//   if more than BUDGET taps arrive within WINDOW_MS, we suspect walking and
//   mute gestures for QUIET_MS. crude but effective. real walking-noise
//   rejection is a research problem.

#define MULTI_TAP_WINDOW_MS    400
#define WALKING_TAP_BUDGET     3
#define WALKING_WINDOW_MS      2000
#define WALKING_QUIET_MS       1000

// ---- mode transitions -------------------------------------------------------

#define MODE_TRANSITION_MS     600  // crossfade duration between emotional modes

// ---- mode-specific timing ---------------------------------------------------

#define CALM_BREATHE_PERIOD_MS     4000
#define HAPPY_PULSE_PERIOD_MS      1500
#define ALERT_BLINK_PERIOD_MS      250    // 4 Hz
#define SLEEPY_DRIFT_PERIOD_MS     8000
#define SLEEPY_MAX_BRIGHTNESS      60     // sleepy is intentionally dim
#define FLOW_WAVE_SPEED_MS         100    // ms per LED step
#define FLOW_WAVE_WIDTH            6      // wave half-width in LEDs
#define COOL_HUE_SPEED_MS          30     // ms per hue step

// ---- phase 6: idle / public-private ----------------------------------------
//
// IDLE_THRESHOLD_MS:
//   after this much time with no classified gesture, the shoe begins fading
//   toward DEEP_IDLE_BRIGHTNESS_SCALE. it never goes "off" — it just settles.
//
// DEEP_IDLE_THRESHOLD_MS:
//   once this much time has passed, the brightness has fully settled into
//   the deep-idle floor. the shoe is now in its "ambient breathing" state.
//
// DEEP_IDLE_BRIGHTNESS_SCALE:
//   0-255 multiplier (255 = full, 128 = half, etc). low enough to read as
//   "asleep" but never zero — the shoe stays alive.
//
// PRIVATE_BRIGHTNESS_SCALE:
//   when in private mode (toggled by triple-tap), the entire output is
//   scaled by this. half-brightness reads as "subtle" without changing
//   the emotional content of any mode.

#define IDLE_THRESHOLD_MS           30000     // 30s with no taps -> begin idle fade
#define DEEP_IDLE_THRESHOLD_MS      120000    // 2 min -> fully settled deep idle
#define DEEP_IDLE_BRIGHTNESS_SCALE  50        // ~20% of full brightness
#define PRIVATE_BRIGHTNESS_SCALE    128       // 50% scaling in private mode

// ---- EEPROM (gesture memory + private mode) --------------------------------
//
// the shoe remembers its last mode AND its public/private setting across
// power cycles. boot into the same emotional state and signaling level you
// left it in.

#define EEPROM_MODE_ADDR       0
#define EEPROM_MAGIC_ADDR      1
#define EEPROM_PRIVATE_ADDR    2
#define EEPROM_MAGIC_VALUE     0x42       // "we've written this before" marker
