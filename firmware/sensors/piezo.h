// firmware/sensors/piezo.h
//
// the piezo reader currently lives in firmware/main/gestures.h, alongside
// the gesture classifier that consumes its events. once we migrate to
// PlatformIO and cross-folder includes work, the raw piezo reader (debounced
// analogRead, threshold check) will move here as a standalone module.
//
// for now this folder remains as conceptual organisation. anything that
// reads from the world will live here in the future:
//   - the piezo (today, in main/)
//   - an accelerometer for motion-reactive behaviour (future)
//   - an LDR for adaptive brightness (future)
//   - an ambient temperature sensor (future)

#pragma once
