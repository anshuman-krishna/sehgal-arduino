// firmware/animations/animations.h
//
// the animation primitives currently live in firmware/main/animations.h.
// post-PlatformIO migration, this folder will house them as standalone
// reusable modules:
//
//   smoothstep   — eased 0..1 curve
//   breathe      — sinusoidal brightness with floor/ceiling
//   pulse        — fast attack, slow decay
//   gamma8       — perceptual brightness correction
//   blendColor   — RGB linear interpolation
//   fillSolid    — write a colour at brightness across a buffer
//
// the rule for any primitive that lands here: stateless. callers pass in
// the current time and any state they want to track. that lets two modes
// share a primitive without stepping on each other.

#pragma once
