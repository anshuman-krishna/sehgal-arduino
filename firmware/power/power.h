// firmware/power/power.h
//
// intent stub — battery monitoring and power-aware behaviour will live here.
//
// responsibilities (planned):
//   - read the battery voltage via a divider on BATTERY_PIN
//   - expose a smoothed voltage and a "low battery" boolean
//   - drive the low-battery emotional state (slow somber pulse)
//   - dim the strip on low battery to extend the wearable's runtime
//   - log voltage trends to serial during testing
//
// this matters more once the shoe is actually being worn for hours at a time.
// premature power optimization is a waste — the priority is interaction
// quality first, then runtime.

#pragma once

// future api (planned shape):
//
//   void   powerBegin();
//   void   powerUpdate();
//   float  powerBatteryVolts();
//   bool   powerIsLow();
