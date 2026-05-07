// firmware/main/gestures.h
//
// turns raw piezo readings into discrete gesture events.
//
// the recognised gestures (today):
//   - SINGLE_TAP   one intentional tap, fires after the multi-tap window expires
//   - DOUBLE_TAP   two taps within MULTI_TAP_WINDOW_MS of each other
//   - TRIPLE_TAP   three taps within window — fires IMMEDIATELY on the third
//                  (no need to wait for a fourth, since we don't have a quad)
//   - NOISE        too many taps too fast — looks like walking, gestures muted
//
// the model:
//   each tap restarts a per-sequence timer. if the timer expires with N taps
//   pending, we classify by N. if N reaches 3, we fire triple immediately
//   and reset. simple, debuggable, latency-consistent.
//
// long-press is intentionally NOT here. piezos detect strikes, not pressure.
// adding a force-sensitive resistor (FSR) is the right path for long-press,
// and that lands in a future hardware iteration.

#pragma once

#include "config.h"

enum GestureEvent {
  GESTURE_NONE = 0,
  GESTURE_SINGLE_TAP,
  GESTURE_DOUBLE_TAP,
  GESTURE_TRIPLE_TAP,
  GESTURE_NOISE
};

// internal state — header-only is fine because main.ino is the only TU.
static unsigned long gLastTapMs            = 0;
static uint8_t       gPendingTapCount      = 0;
static unsigned long gLastTapInSequenceMs  = 0;
static uint8_t       gTapsInWalkingWindow  = 0;
static unsigned long gWalkingWindowStartMs = 0;
static unsigned long gWalkingMutedUntilMs  = 0;

// raw piezo reader with debounce. returns true once per physical tap.
static inline bool gestureRawTapDetected(unsigned long nowMs) {
  if (nowMs - gLastTapMs < PIEZO_DEBOUNCE_MS) return false;
  int raw = analogRead(PIEZO_PIN);
  if (raw < PIEZO_TAP_THRESHOLD) return false;
  gLastTapMs = nowMs;
  return true;
}

// call every loop. returns the gesture event for this tick (usually NONE).
inline GestureEvent gestureUpdate(unsigned long nowMs) {
  // rolling walking-detection window
  if (nowMs - gWalkingWindowStartMs > WALKING_WINDOW_MS) {
    gWalkingWindowStartMs = nowMs;
    gTapsInWalkingWindow = 0;
  }
  bool walkingMuted = (nowMs < gWalkingMutedUntilMs);

  bool rawTap = gestureRawTapDetected(nowMs);

  if (rawTap) {
    gTapsInWalkingWindow++;
    if (gTapsInWalkingWindow > WALKING_TAP_BUDGET) {
      gWalkingMutedUntilMs = nowMs + WALKING_QUIET_MS;
      gPendingTapCount = 0;
      return GESTURE_NOISE;
    }
  }

  if (walkingMuted) return GESTURE_NONE;

  // accumulate taps within the multi-tap window
  if (rawTap) {
    gPendingTapCount++;
    gLastTapInSequenceMs = nowMs;
    // triple-tap fires as soon as the third tap lands — no need to wait.
    if (gPendingTapCount >= 3) {
      gPendingTapCount = 0;
      return GESTURE_TRIPLE_TAP;
    }
    return GESTURE_NONE;
  }

  // sequence timeout — commit whatever count we have
  if (gPendingTapCount > 0 && nowMs - gLastTapInSequenceMs > MULTI_TAP_WINDOW_MS) {
    uint8_t count = gPendingTapCount;
    gPendingTapCount = 0;
    if (count == 1) return GESTURE_SINGLE_TAP;
    if (count == 2) return GESTURE_DOUBLE_TAP;
    // count >= 3 was already fired immediately above
  }

  return GESTURE_NONE;
}
