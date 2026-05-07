// firmware/modes/modes.h
//
// the six emotional modes currently live in firmware/main/modes.h. this
// file remains as conceptual organisation — once we migrate to PlatformIO,
// each mode will become its own file in this folder:
//
//   firmware/modes/calm.h
//   firmware/modes/happy.h
//   firmware/modes/alert.h
//   firmware/modes/sleepy.h
//   firmware/modes/flow.h
//   firmware/modes/cool.h
//
// the contract every mode obeys is documented in firmware/main/modes.h:
// each is a draw function that fills LED_COUNT pixels based on the current
// time. they share the animation primitives in firmware/main/animations.h.

#pragma once
