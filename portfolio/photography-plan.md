# photography plan

> the master shot list for documenting the project. every photo and video that should exist by the end of phase 9.

the goal of this list is not to take pretty pictures. it is to make sure the **process** is captured — failures, iterations, decisions, and successes — so the portfolio reads as honest design work, not a polished marketing reel.

## philosophy

three rules for every shot on this list:

1. **process beats product.** a photo of three failed heel-module mockups in a row is more valuable than one photo of the final one.
2. **context matters.** a mode photographed in a dark studio looks different from the same mode worn in a kitchen at 6pm. capture both.
3. **caption everything.** every photo should have a one-line note explaining what it shows and why it's interesting. unlabeled photos rot.

## organisation

every photo / video gets:
- a filename that includes the phase and a short slug (`phase2_piezo_breadboard.jpg`)
- a one-line caption stored in `validate.md` under the relevant phase's portfolio archive section
- a backup copy outside the project folder (cloud sync or external drive)

---

## the master shot list

### phase 1 — hardware bring-up

- [ ] **breadboard wide shot**: full layout with arduino, strip, capacitor, resistor, USB cable visible. labelled.
- [ ] **detail shot**: 1000µF capacitor and 330Ω resistor close-up.
- [ ] **first power-on**: the moment the strip first lights up. ideally a video of the boot fade-up.
- [ ] **calm baseline running**: 10-second video of the breathing baseline in a darkened room.

### phase 2 — piezo sensor

- [ ] **piezo + bleed resistor**: macro shot of the 27mm disc with the 2.2MΩ resistor across A0/GND.
- [ ] **serial plotter screenshot**: showing your noise floor, light-tap peak, and the threshold you chose.
- [ ] **integrated tap response**: video of finger-tapping the piezo and watching the warm-amber flash and decay.

### phase 3 — emotional lighting engine

- [ ] **all six modes, separate clips** (~5s each):
  - [ ] calm
  - [ ] happy
  - [ ] alert
  - [ ] sleepy
  - [ ] flow
  - [ ] cool
- [ ] **all six modes, still photos** at peak brightness (for the portfolio mode-mapping spread).
- [ ] **transition video**: cycling through every mode in order, showing the cross-fades.
- [ ] **environment comparison**: each mode photographed indoor / dusk / direct sunlight (for the legibility spread).

### phase 4 — gesture intelligence

- [ ] **single-tap → mode change**: video showing the ~400ms delay and the cross-fade.
- [ ] **double-tap → mode reverse**: video.
- [ ] **walking-mute behaviour**: video of walking 10 steps without false-fires; mute kicking in.
- [ ] **EEPROM persistence**: video of changing mode, power-cycling, and seeing it boot back into that mode.
- [ ] **serial-monitor screenshot**: showing the `mode -> ...` and `walking noise` log lines.

### phase 5 — wearable integration

- [ ] **piezo under insole**: insole removed, piezo and routing visible.
- [ ] **LED strip on sole edge**: close-up of the strip routed along the outer sole.
- [ ] **heel module open**: arduino + battery + wiring inside the enclosure.
- [ ] **heel module closed, on foot**: the wearable as a wearable.
- [ ] **first walk video**: 30 seconds of walking, with the LEDs visible from a low angle.
- [ ] **iteration archive**: every heel-module prototype side-by-side. v1 cardboard, v2 foam, v3 printed, etc.

### phase 6 — emotional communication layer

- [ ] **idle dim-down timelapse**: 2-minute timelapse showing the brightness fading from full to deep-idle.
- [ ] **wake-on-tap clip**: tap during deep-idle, instant restoration of full brightness.
- [ ] **public ↔ private toggle**: short clip showing the ~50% brightness shift on triple-tap, in a real-feeling context (a desk, a kitchen, not a studio).

### phase 7 — safety & reliability

- [ ] **flex test video**: 100-cycle bend test followed by a check that the LEDs still work.
- [ ] **thermal photo**: hand-test or IR shot of the heel module after 20 min of cool-mode.
- [ ] **reinforcement archive**: before-and-after photos of any heat-shrink, hot glue, or cable management added.
- [ ] **non-negotiable proof**: one written paragraph + one photo per non-negotiable item (battery placement, kill-switch, etc.)

### phase 8 — aesthetic refinement

- [ ] **diffusion comparison spread**: 4 photos, same mode, four diffusion treatments (bare / silicone tube / fabric / 3D-printed channel).
- [ ] **hero shot**: the finished phase 8 shoe, lit, on a clean background.
- [ ] **walking video, finished**: 30 seconds of walking in the finished aesthetic.
- [ ] **wire visibility decision**: a paired shot — interior view (showing wiring decisions) and exterior view (showing what observers see).

### phase 9 — portfolio system

- [ ] **system architecture spread**: diagram from `portfolio/system-architecture.md`, redrawn cleanly.
- [ ] **interaction storyboard spread**: storyboard from `portfolio/interaction-storyboard.md`, illustrated frame by frame.
- [ ] **emotional mapping spread**: per-mode panel (palette, timing curve, photo).
- [ ] **gesture vocabulary spread**: gestures with diagrams.
- [ ] **process narrative**: the project arc — earliest sketches through wearable, including dead ends.

### phase 10 — experimental expansion (whichever expansions get prototyped)

- [ ] **dual-shoe sync**: video of two shoes mirroring each other.
- [ ] **motion-reactive**: clip showing accelerometer-driven behavior.
- [ ] **adaptive brightness**: same scene at two ambient light levels, shoe responding.
- [ ] **haptic feedback**: macro of vibration motor + gesture confirmation behaviour.

---

## what not to bother with

a few photo categories that look impressive but aren't worth the effort:

- **dramatic studio lighting on bare components**: looks like stock photography, communicates nothing
- **explainer infographics generated by LLMs**: cheap to make, easy to spot, harms credibility
- **slow-mo close-ups of LED individual pixels**: sometimes useful for diagnosis but rarely for portfolio
- **drone shots of the wearer walking**: the project is about a tap, not a landscape

what to spend the time on instead: **honest process documentation**. the portfolio's strongest asset is being able to show the gap between what was attempted and what was learned.

---

## storage and naming

```
photos/
├── phase01/
│   ├── breadboard_wide.jpg           caption: "...
│   ├── capacitor_detail.jpg
│   └── boot_fadeup.mp4
├── phase02/
│   └── ...
└── final/
    ├── hero.jpg
    └── walking_finished.mp4
```

every filename: lowercase, underscored, descriptive. no spaces. no "IMG_4382" anywhere in the project.

every video: 10-30 seconds typically. resist the urge to make 2-minute reels. portfolio reviewers don't have time, and the salient moment is usually under 5 seconds.

---

## the test for whether a photo is worth taking

ask: **could a reasonable person, looking only at this photo, understand one of the following?**
- what was being tested
- what was learned
- what decision was made
- what the wearer experiences

if the answer is no, don't take the photo. if the answer is "kind of" — take it but write a substantial caption to do the lifting. if the answer is yes — that's the photo.
