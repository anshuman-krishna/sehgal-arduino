// firmware/main/modes.h
//
// the six emotional modes. each one is a small function that fills the LED
// buffer based on the current time. they share the animation primitives in
// animations.h.
//
// adding a new mode is intentionally easy:
//   1. add an enum value
//   2. write a draw function
//   3. add a case to modeRender()
//   4. add the name to modeName()
//
// the rule for each mode: it must feel emotionally distinct from every
// other mode at three feet, without prompting. if an outside observer can't
// guess the emotion within five seconds, the mode needs work — not a
// dictionary entry explaining what it "should" feel like.

#pragma once

#include <FastLED.h>
#include "config.h"
#include "animations.h"

enum EmotionalMode {
  MODE_CALM = 0,
  MODE_HAPPY,
  MODE_ALERT,
  MODE_SLEEPY,
  MODE_FLOW,
  MODE_COOL,
  MODE_COUNT
};

inline const char* modeName(EmotionalMode m) {
  switch (m) {
    case MODE_CALM:   return "calm";
    case MODE_HAPPY:  return "happy";
    case MODE_ALERT:  return "alert";
    case MODE_SLEEPY: return "sleepy";
    case MODE_FLOW:   return "flow";
    case MODE_COOL:   return "cool";
    default:          return "?";
  }
}

inline EmotionalMode modeNext(EmotionalMode m) {
  return (EmotionalMode)((m + 1) % MODE_COUNT);
}

inline EmotionalMode modePrev(EmotionalMode m) {
  return (EmotionalMode)((m + MODE_COUNT - 1) % MODE_COUNT);
}

// ---- mode draw functions ----------------------------------------------------
// each writes LED_COUNT pixels into the provided buffer.

// CALM — soft blue, slow breathing. the resting emotional state.
inline void drawCalm(unsigned long now, CRGB* leds) {
  uint8_t b = breatheBrightness(now, CALM_BREATHE_PERIOD_MS, 25, MAX_BRIGHTNESS);
  fillSolid(leds, LED_COUNT, CRGB(40, 90, 200), b);
}

// HAPPY — warm orange, faster pulse. socially open, energetic, generous.
inline void drawHappy(unsigned long now, CRGB* leds) {
  uint8_t b = pulseBrightness(now, HAPPY_PULSE_PERIOD_MS, 40, MAX_BRIGHTNESS);
  fillSolid(leds, LED_COUNT, CRGB(255, 140, 30), b);
}

// ALERT — sharp red, 4 Hz blink. attention. caution. the only mode without easing.
inline void drawAlert(unsigned long now, CRGB* leds) {
  unsigned long phase = now % ALERT_BLINK_PERIOD_MS;
  bool on = phase < (ALERT_BLINK_PERIOD_MS / 2);
  uint8_t b = on ? MAX_BRIGHTNESS : 20;
  fillSolid(leds, LED_COUNT, CRGB(255, 30, 30), b);
}

// SLEEPY — dim purple, very slow drift. low energy, drifting, withdrawn.
inline void drawSleepy(unsigned long now, CRGB* leds) {
  uint8_t b = breatheBrightness(now, SLEEPY_DRIFT_PERIOD_MS, 8, SLEEPY_MAX_BRIGHTNESS);
  fillSolid(leds, LED_COUNT, CRGB(80, 30, 120), b);
}

// FLOW — cyan wave traveling along the strip. focused, in-motion, flowing.
inline void drawFlow(unsigned long now, CRGB* leds) {
  CRGB base = CRGB(40, 200, 220);
  float center = (float)((now / FLOW_WAVE_SPEED_MS) % LED_COUNT);
  for (uint16_t i = 0; i < LED_COUNT; i++) {
    float dist = fabs((float)i - center);
    if (dist > LED_COUNT / 2.0f) dist = LED_COUNT - dist; // wrap distance
    float intensity = 0.0f;
    if (dist < FLOW_WAVE_WIDTH) {
      intensity = 1.0f - (dist / FLOW_WAVE_WIDTH);
    }
    uint8_t b = (uint8_t)(intensity * MAX_BRIGHTNESS);
    leds[i] = base;
    leds[i].nscale8(gamma8(b));
  }
}

// COOL — rainbow motion. playful, performative, look-at-me.
inline void drawCool(unsigned long now, CRGB* leds) {
  uint8_t hueOffset = (uint8_t)((now / COOL_HUE_SPEED_MS) & 0xFF);
  uint8_t hueStep   = 255 / LED_COUNT;
  for (uint16_t i = 0; i < LED_COUNT; i++) {
    leds[i] = CHSV(hueOffset + i * hueStep, 255, MAX_BRIGHTNESS);
  }
}

// dispatch.
inline void modeRender(EmotionalMode m, unsigned long now, CRGB* leds) {
  switch (m) {
    case MODE_CALM:   drawCalm(now, leds);   break;
    case MODE_HAPPY:  drawHappy(now, leds);  break;
    case MODE_ALERT:  drawAlert(now, leds);  break;
    case MODE_SLEEPY: drawSleepy(now, leds); break;
    case MODE_FLOW:   drawFlow(now, leds);   break;
    case MODE_COOL:   drawCool(now, leds);   break;
    default: break;
  }
}
