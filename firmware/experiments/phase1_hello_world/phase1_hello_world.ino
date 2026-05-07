// sehgal-arduino — phase 1 hello-world (archived reference)
//
// this is the original phase 1 sketch, preserved for reference.
// it does NOT read the piezo. its only job is to confirm:
//   - arduino nano boots and accepts an upload
//   - WS2812B strip receives a clean data signal on D6
//   - the 1000uF capacitor smooths the power-on inrush
//   - the 330R series resistor protects the data line
//
// behaviour: fades up softly to a warm-white "wake" frame, then eases into a
// calm-blue breathing baseline. that baseline is the emotional resting state
// of the shoe.
//
// the active "current state" sketch is firmware/main/main.ino — this archived
// copy stays in place so you can always come back to a known-good LED-only
// baseline if something stops working after phase 2.

#include <FastLED.h>

#define LED_DATA_PIN     6
#define LED_COUNT        30
#define LED_TYPE         WS2812B
#define COLOR_ORDER      GRB
#define MAX_BRIGHTNESS   120
#define WAKE_DURATION_MS 1500
#define BREATHE_PERIOD_MS 4000

const CRGB CALM_COLOR  = CRGB(40, 90, 200);
const CRGB WAKE_COLOR  = CRGB(180, 160, 120);

CRGB leds[LED_COUNT];
unsigned long bootMillis = 0;

float smoothstep(float t) {
  if (t <= 0.0f) return 0.0f;
  if (t >= 1.0f) return 1.0f;
  return t * t * (3.0f - 2.0f * t);
}

uint8_t breatheBrightness(unsigned long nowMs) {
  const uint8_t floorB = 25;
  const uint8_t ceilB  = MAX_BRIGHTNESS;
  float phase = (float)(nowMs % BREATHE_PERIOD_MS) / (float)BREATHE_PERIOD_MS;
  float wave  = 0.5f * (1.0f - cosf(2.0f * PI * phase));
  return (uint8_t)(floorB + wave * (ceilB - floorB));
}

void fillStrip(const CRGB& base, uint8_t brightness) {
  for (uint16_t i = 0; i < LED_COUNT; i++) {
    leds[i] = base;
  }
  FastLED.setBrightness(brightness);
  FastLED.show();
}

void setup() {
  delay(50);
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.clear(true);
  bootMillis = millis();
  Serial.begin(115200);
  Serial.println(F("sehgal-arduino phase 1: hello-world boot"));
}

void loop() {
  unsigned long now = millis();
  unsigned long sinceBoot = now - bootMillis;

  if (sinceBoot < WAKE_DURATION_MS) {
    float t = (float)sinceBoot / (float)WAKE_DURATION_MS;
    uint8_t b = (uint8_t)(smoothstep(t) * MAX_BRIGHTNESS);
    fillStrip(WAKE_COLOR, b);
  } else {
    uint8_t b = breatheBrightness(now);
    fillStrip(CALM_COLOR, b);
  }

  delay(16);
}
