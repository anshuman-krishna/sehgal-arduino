// firmware/main/animations.h
//
// reusable lighting primitives. modes are emotional expressions; the
// functions here are the verbs they compose with.
//
// all primitives are non-blocking and millis()-driven. nothing in this file
// holds state — state belongs to the caller. that lets two modes share a
// primitive without stepping on each other.

#pragma once

#include <FastLED.h>

// smoothstep: 0..1 input, 0..1 output, S-shaped curve. linear motion feels
// mechanical; this gives the eye something organic to follow.
inline float smoothstep(float t) {
  if (t <= 0.0f) return 0.0f;
  if (t >= 1.0f) return 1.0f;
  return t * t * (3.0f - 2.0f * t);
}

// breathing: sinusoidal brightness between floor and ceiling. floor is
// non-zero so the strip never appears "off" while idle. used by calm + sleepy.
inline uint8_t breatheBrightness(unsigned long nowMs, uint16_t periodMs,
                                 uint8_t floorB, uint8_t ceilB) {
  float phase = (float)(nowMs % periodMs) / (float)periodMs;
  float wave  = 0.5f * (1.0f - cosf(2.0f * PI * phase));
  return (uint8_t)(floorB + wave * (ceilB - floorB));
}

// pulse: faster attack, slower decay. used by happy. feels more energetic
// than breathing — the shoe is reaching out to you, not idling.
inline uint8_t pulseBrightness(unsigned long nowMs, uint16_t periodMs,
                               uint8_t floorB, uint8_t ceilB) {
  float phase = (float)(nowMs % periodMs) / (float)periodMs;
  float wave;
  if (phase < 0.2f) {
    wave = phase / 0.2f;                 // fast 0->1 in first 20% of cycle
  } else {
    wave = 1.0f - (phase - 0.2f) / 0.8f; // slow decay over remaining 80%
  }
  return (uint8_t)(floorB + wave * (ceilB - floorB));
}

// gamma 2.0 approximation. cheap on AVR (no pow), good enough that low
// brightness values don't disappear. without this, breathing curves look
// linear in code but jumpy on the strip — the eye is logarithmic.
inline uint8_t gamma8(uint8_t b) {
  uint16_t x = (uint16_t)b * (uint16_t)b;
  return (uint8_t)(x >> 8);
}

// blend two colours by t in [0..1]. used for the eased decay during mode
// transitions. linear interpolation in RGB is technically wrong (HSL/Lab
// would be better) but visually acceptable for the colour palettes here.
inline CRGB blendColor(const CRGB& a, const CRGB& b, float t) {
  if (t <= 0.0f) return a;
  if (t >= 1.0f) return b;
  return CRGB(
    a.r + (int)((b.r - a.r) * t),
    a.g + (int)((b.g - a.g) * t),
    a.b + (int)((b.b - a.b) * t)
  );
}

// fill a buffer with a single colour scaled by gamma-corrected brightness.
// used by every "single colour" mode (calm, happy, alert, sleepy).
inline void fillSolid(CRGB* leds, uint16_t count, const CRGB& base, uint8_t brightness) {
  uint8_t scale = gamma8(brightness);
  for (uint16_t i = 0; i < count; i++) {
    leds[i] = base;
    leds[i].nscale8(scale);
  }
}
