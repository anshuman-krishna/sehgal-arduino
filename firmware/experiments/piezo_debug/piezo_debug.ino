// sehgal-arduino — phase 2a piezo serial debug
//
// this sketch does ONE thing: it streams the raw piezo reading to the serial
// monitor so you can watch the values change in real time as you tap the
// sensor. there are no LEDs, no logic, no thresholds. just numbers.
//
// why this exists:
//   every piezo, every wiring layout, every shoe surface produces slightly
//   different readings. you cannot pick a tap-detection threshold from theory
//   — you have to MEASURE it on your actual hardware. this sketch is the
//   measuring tool.
//
// how to use it:
//   1. wire the piezo (see the README, "phase 2 wiring")
//   2. upload this sketch
//   3. open the serial monitor at 115200 baud
//   4. open the serial plotter (tools -> serial plotter) for a live graph
//   5. tap the sensor with your finger. watch the spike.
//   6. tap softer. tap harder. drag a finger across it. drop something near it.
//   7. note the typical resting value, the typical light-tap peak, the typical
//      hard-tap peak, and any walking/handling noise.
//   8. pick a threshold that sits comfortably above noise but below light taps.
//      write it down. that becomes PIEZO_TAP_THRESHOLD in the next sketch.
//
// expected behaviour:
//   - at rest with the 2.2M ohm bleed resistor in place, the value should sit
//     near zero (typically 0-5)
//   - a deliberate finger tap should produce a sharp spike, often 200-800
//   - released energy after a tap may produce a small negative-going dip
//     followed by ringing — this is normal piezo behaviour
//   - if your at-rest value is high (>50) or noisy, the bleed resistor is
//     missing or wired wrong. check that first.

#define PIEZO_PIN        A0
#define SAMPLE_INTERVAL_MS 5     // 200 Hz sample rate. fast enough to catch tap edges.

unsigned long lastSampleMs = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("piezo_debug ready. tap the sensor and watch the values."));
}

void loop() {
  unsigned long now = millis();
  if (now - lastSampleMs < SAMPLE_INTERVAL_MS) return;
  lastSampleMs = now;

  int raw = analogRead(PIEZO_PIN);
  Serial.println(raw);
}
