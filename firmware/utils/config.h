// firmware/utils/config.h
//
// the active config currently lives in firmware/main/config.h alongside the
// sketch that uses it. the arduino IDE does not traverse subfolders, so
// keeping config in the sketch directory is what lets the multi-file build
// compile.
//
// post-PlatformIO migration this file becomes the canonical home and
// firmware/main/config.h goes away.

#pragma once
