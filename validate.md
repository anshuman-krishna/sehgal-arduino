# need to manually validate and check/cross

> this file is the **manual-validation logbook**. every item below requires a human, real hardware, and observation. firmware-side progress is tracked in `testing/changelog.md` (local-only); hardware-truth lives here, in the public repo, as proof of what was actually built — not just written.

## how to use this file

- each item is a checkbox. tick it (`- [x]`) once you have personally verified it on hardware.
- under each item, write what you observed: actual values, surprises, photos taken, decisions made.
- if a check fails, **don't delete it** — log the failure, log what you changed, and re-test. failures are valuable data and the most useful part of the portfolio.
- new phases add new sections. nothing is removed; this becomes the project's growing hardware archive.

---

## phase 1 — hardware bring-up

> sketch under test: `firmware/experiments/phase1_hello_world/phase1_hello_world.ino`
> what success looks like: soft fade-up to warm white, settles into calm-blue breathing baseline, runs cleanly for at least 60 seconds.

### power & upload path

- [ ] arduino nano accepts an upload over USB without bootloader errors
      _notes:_
- [ ] USB cable is a data cable, not a charge-only cable (verified by upload working)
      _notes:_
- [ ] correct port appears in arduino IDE under **tools → port**
      _port:_
- [ ] processor option correct (standard or "Old Bootloader" if clone)
      _which one:_

### LED strip behaviour

- [ ] strip lights up during the boot fade-up
      _notes:_
- [ ] calm-blue breathing baseline runs steadily for ≥60 seconds without flicker
      _notes:_
- [ ] strip can be power-cycled three times in a row with identical result
      _notes:_
- [ ] colours render correctly (no GRB/RGB mismatch — blue is blue, not red)
      _notes:_

### power stability

- [ ] 5V across the strip measures close to 5.0V with a multimeter
      _measured voltage:_
- [ ] no flicker observed when the strip ramps to peak brightness
      _notes:_
- [ ] nano onboard LED stays steady (no brown-out blinking)
      _notes:_

### protection components verified

- [ ] 330Ω series resistor in place between D6 and the strip's DIN
      _notes:_
- [ ] 1000µF capacitor wired across 5V/GND near the strip with correct polarity
      _notes:_
- [ ] no harsh white flash at power-on (or only a very brief one)
      _notes:_

### portfolio archive

- [ ] first wiring photo captured (close-up of breadboard, all components labelled)
      _filename / location:_
- [ ] short note written about what worked / what surprised you
      _link or excerpt:_

---

## phase 2 — piezo sensor

> sketches under test:
>   1. `firmware/experiments/piezo_debug/piezo_debug.ino` — for tuning
>   2. `firmware/main/main.ino` — for integrated tap-response validation
> what success looks like: every intentional tap produces exactly one warm-amber flash (or, with phase 3 firmware, exactly one mode change), no spontaneous fires when still.

### bleed resistor / resting behaviour

- [ ] 2.2MΩ bleed resistor wired across A0 and GND
      _notes:_
- [ ] resting analog reading sits near zero (expected 0–5)
      _measured noise floor:_
- [ ] no drifting baseline observed over 60 seconds of stillness
      _notes:_

### threshold characterisation (using piezo_debug sketch)

- [ ] light-tap peak measured (gentle finger tap)
      _measured value:_
- [ ] hard-tap peak measured (firm finger tap)
      _measured value:_
- [ ] handling-noise peak measured (drag a finger across, brush the wire)
      _measured value:_
- [ ] **`PIEZO_TAP_THRESHOLD` chosen and committed to `config.h`**
      _chosen value:_
      _reasoning:_

### tap-response validation (using main.ino)

- [ ] integrated sketch produces **exactly one** flash per intentional tap
      _notes:_
- [ ] no spontaneous fires when standing still / sensor untouched
      _notes:_
- [ ] eased decay from amber to calm-blue feels intentional, not abrupt
      _notes:_
- [ ] sensor wires don't induce false-fires when wiggled
      _notes:_

### walking false-triggers (characterisation, not solution — phase 4 firmware tries to filter these)

- [ ] characterised on hard floor (tile / wood / concrete)
      _trigger frequency, gait notes:_
- [ ] characterised on soft floor (carpet / rug)
      _trigger frequency, gait notes:_
- [ ] characterised on irregular surface (gravel, stairs, ramp)
      _trigger frequency, gait notes:_
- [ ] walking-trigger patterns logged below for phase 4 to design against
      _patterns observed:_

### decisions taken (write down even if "no")

- [ ] decided whether to add 1N4148 clamp diodes across the piezo
      _decision + reasoning:_
- [ ] decided whether `PIEZO_DEBOUNCE_MS` needs raising from 180ms (ring-out fires)
      _decision + reasoning:_
- [ ] decided whether `MAX_BRIGHTNESS` (currently 120) is right for indoor / outdoor
      _decision + reasoning:_

### portfolio archive

- [ ] photo of the piezo + bleed resistor on the breadboard
      _filename / location:_
- [ ] short serial-plotter screenshot showing the threshold chosen against your noise floor
      _filename / location:_

---

## phase 3 — emotional lighting engine

> sketch under test: `firmware/main/main.ino` (after compiling all six modes)
> what success looks like: each mode is recognisable from across the room, transitions between modes feel intentional rather than abrupt, no flicker, no eye-strain at any brightness.

### each mode renders correctly

- [ ] **calm** — soft blue, slow breathing (~4s/breath), feels grounded
      _observation:_
- [ ] **happy** — warm orange, faster pulse (~1.5s), feels open and energetic
      _observation:_
- [ ] **alert** — sharp red, 4 Hz blink, feels attentive (not seizure-inducing)
      _observation:_
- [ ] **sleepy** — dim purple, very slow drift (~8s), capped at low brightness
      _observation:_
- [ ] **flow** — cyan wave traveling along the strip
      _observation:_
- [ ] **cool** — rainbow motion, hue rotates over time
      _observation:_

### transitions

- [ ] cross-fade between any two modes feels smooth (no jump, no flicker)
      _notes:_
- [ ] transition duration (~600ms) feels right — not too quick, not laggy
      _notes:_
- [ ] cycling through all six modes back to calm feels natural
      _notes:_

### gamma correction & visual quality

- [ ] low-brightness portions of breathing curves are visible (gamma working)
      _notes:_
- [ ] no "hard floor" jump from off to dim
      _notes:_
- [ ] no visible per-pixel jitter or de-sync at standstill
      _notes:_

### emotional readability test (the most important one)

- [ ] showed each mode to **one outsider**, asked them to describe the feeling without prompting
      _person 1 — observations per mode:_
- [ ] showed each mode to **a second outsider**, same exercise
      _person 2 — observations per mode:_
- [ ] noted any modes that were misread or felt ambiguous
      _modes that need rework:_

### environment checks

- [ ] modes legible in indoor (room) light
      _notes:_
- [ ] modes legible in dim / evening light
      _notes:_
- [ ] modes legible (or gracefully fail) in direct sunlight
      _notes:_

### portfolio archive

- [ ] short video of all six modes (one clip per mode, ~5s each)
      _filename / location:_
- [ ] still photos of each mode at peak brightness
      _filenames / location:_

---

## phase 4 — gesture intelligence

> sketch under test: `firmware/main/main.ino` — single-tap / double-tap / walking-mute system live
> what success looks like: a deliberate single tap reliably advances mode; a deliberate double-tap reliably steps back; walking on hard floor doesn't fire random mode changes (or if it does, the walking-mute kicks in and stops the noise quickly).

### single-tap behaviour

- [ ] single tap (with quiet pause after) advances to the next mode
      _success rate over 10 attempts:_
- [ ] visible delay between tap and mode change (~400ms) feels acceptable
      _subjective notes:_
- [ ] mode change order is correct (calm → happy → alert → sleepy → flow → cool → calm)
      _notes:_

### double-tap behaviour

- [ ] two taps within ~400ms reliably step back one mode
      _success rate over 10 attempts:_
- [ ] when single-taps are intended, no accidental double-taps fire
      _notes:_
- [ ] window length feels natural (not too tight, not too lazy)
      _decision + reasoning:_

### walking-noise handling

- [ ] standing still, no spurious mode changes for ≥60 seconds
      _notes:_
- [ ] walking 10 steps on hard floor — mode changes / walking-mute behaviour:
      _observation:_
- [ ] walking 10 steps on carpet — mode changes / walking-mute behaviour:
      _observation:_
- [ ] after walking stops, gestures resume working within ~1 second
      _notes:_
- [ ] decided whether `WALKING_TAP_BUDGET` / window / quiet timing needs tuning
      _decision + reasoning:_

### EEPROM gesture memory

- [ ] change to a non-calm mode, power-cycle the nano, shoe boots back into that mode
      _which mode tested:_
- [ ] tested for each non-calm mode
      _notes:_
- [ ] no EEPROM corruption observed across multiple power-cycles
      _notes:_

### tap-feedback latency

- [ ] subjectively, the system feels "alive" rather than "delayed"
      _notes:_
- [ ] decided whether to add an immediate visual ack on raw tap (today there isn't one — only the mode change)
      _decision + reasoning:_

### serial debug verification

- [ ] serial monitor shows `mode -> <name>` lines on every change
      _notes:_
- [ ] serial monitor shows `walking noise -> gestures muted` when expected
      _notes:_

### portfolio archive

- [ ] video of intentional taps cycling through all six modes
      _filename / location:_
- [ ] short video of the walking-mute behaviour (walk, see no false-fires, stop, tap, mode changes)
      _filename / location:_

---

## phase 5 — wearable integration

> this phase has no firmware-side deliverable. it is the moment the project leaves the breadboard and becomes a thing you can wear. nearly everything below is observation, decision-making, and iteration.
> what success looks like: a wearable shoe that feels comfortable for ≥30 minutes, stays electrically reliable while walking, and looks intentional rather than improvised.

### piezo placement

- [ ] piezo mounted under the heel insole (not under direct compression points)
      _location notes / photo:_
- [ ] piezo wires routed without flex stress, secured to insole or shoe wall
      _routing notes:_
- [ ] tap detection still works reliably with insole pressed back in place
      _notes:_
- [ ] walking false-trigger rate measured **after** mounting in shoe (different from breadboard)
      _measured rate:_

### LED strip placement

- [ ] strip placed along sole edge, oriented so the LEDs face outward
      _placement notes:_
- [ ] strip protected from direct ground contact (raised slightly, or covered with diffuser)
      _notes:_
- [ ] strip survives flex when foot bends during walk
      _notes:_
- [ ] strip data line stays reliable when shoe flexes (no flicker mid-walk)
      _notes:_

### heel module / electronics enclosure

- [ ] arduino + battery housed in a heel-mounted module
      _module material / construction notes:_
- [ ] module is removable for maintenance without unwiring the shoe
      _notes:_
- [ ] module does not press into the heel during walking
      _comfort notes:_
- [ ] module weight is acceptable (subjectively, after 30 min wear)
      _measured weight + subjective notes:_

### wire routing

- [ ] all wires run along non-flex zones where possible
      _notes:_
- [ ] all solder joints heat-shrunk or otherwise insulated
      _notes:_
- [ ] no exposed conductors anywhere on the inside of the shoe
      _notes:_
- [ ] data wire from MCU to LED strip stays under 30 cm to avoid signal degradation
      _measured length:_

### power switch & accessibility

- [ ] external on/off switch is reachable without removing the shoe
      _switch type + location:_
- [ ] charging the battery is possible without dismantling the heel module
      _notes:_
- [ ] battery can be replaced without removing the shoe (or this is consciously deferred)
      _decision + reasoning:_

### walking comfort tests

- [ ] walked **5 minutes** in shoe — comfort observations
      _notes / pressure points:_
- [ ] walked **15 minutes** in shoe — comfort + electrical reliability
      _notes:_
- [ ] walked **30 minutes** in shoe — any hot spots, pressure marks, fatigue?
      _notes:_
- [ ] walked **1 hour** in shoe (stretch goal) — does it still feel like a shoe?
      _notes:_

### weight & balance

- [ ] left-right weight difference measured (single shoe vs un-modded shoe)
      _measured weight delta:_
- [ ] subjective gait check — does the modified shoe feel asymmetric?
      _notes:_

### environmental robustness

- [ ] tested in cool indoor temp
      _notes:_
- [ ] tested in warm / humid conditions (sweat exposure)
      _notes:_
- [ ] tested briefly outdoors (light rain or just outdoor air)
      _notes:_
- [ ] decided how to handle rain / moisture (ignore, conformal coat, or "wear-only-when-dry" rule)
      _decision + reasoning:_

### safety verification (non-negotiable)

- [ ] battery is in the external heel module, **not** under any compression zone
      _notes:_
- [ ] no exposed conductors inside the shoe interior
      _notes:_
- [ ] no overheating observed at any LED mode after 10 minutes continuous run
      _notes:_
- [ ] power can be killed instantly via the external switch
      _verified:_
- [ ] decided whether to add an inline fuse on the 5V rail
      _decision + reasoning:_

### portfolio archive

- [ ] photo: piezo mounted under insole (insole removed for clarity)
      _filename / location:_
- [ ] photo: LED strip routed along sole edge
      _filename / location:_
- [ ] photo: heel module open, showing wiring
      _filename / location:_
- [ ] photo: heel module closed, on the wearer's foot
      _filename / location:_
- [ ] short video: walking with the shoe on, modes responding to taps
      _filename / location:_
- [ ] sketch / diagram: exploded view of the heel module
      _filename / location:_

---

## phase 6 — emotional communication layer

> sketch under test: `firmware/main/main.ino` (idle dim-down + public/private toggle live)
> what success looks like: the shoe gracefully recedes when ignored, wakes instantly on a tap, and the public/private toggle is reliable and persistent.

### triple-tap behaviour

- [ ] three taps within ~800ms total reliably toggle public/private
      _success rate over 10 attempts:_
- [ ] when intending double-tap, no accidental triple-tap fires
      _notes:_
- [ ] when intending triple-tap, no early commit as double
      _notes:_
- [ ] decided whether `MULTI_TAP_WINDOW_MS` (400ms) needs widening for triple
      _decision + reasoning:_

### public vs private brightness

- [ ] public mode reads as "expressive" from across a room
      _notes:_
- [ ] private mode reads as "subtle / personal" — same emotion, dimmer
      _notes:_
- [ ] the difference between public and private is meaningful but not jarring
      _notes:_
- [ ] decided whether `PRIVATE_BRIGHTNESS_SCALE` (default 128 / 50%) is right
      _chosen value + reasoning:_

### private state persistence (EEPROM)

- [ ] toggle to private, power-cycle the nano, shoe boots back into private
      _notes:_
- [ ] toggle back to public, power-cycle, shoe boots back into public
      _notes:_
- [ ] private state survives at least 5 power-cycles without corruption
      _notes:_

### idle dim-down behaviour

- [ ] no taps for 30s — visible fade begins toward dim-idle
      _observed timing / smoothness:_
- [ ] no taps for 2 minutes — settled into deep-idle (~20% brightness)
      _observed brightness vs default:_
- [ ] fade is smooth (not stepped), eyes can track it as gentle movement
      _notes:_
- [ ] shoe NEVER goes fully dark during deep-idle
      _notes:_
- [ ] decided whether `IDLE_THRESHOLD_MS` (30s) is right for real wear
      _decision + reasoning:_
- [ ] decided whether `DEEP_IDLE_THRESHOLD_MS` (2min) is right
      _decision + reasoning:_

### wake-on-tap from idle

- [ ] tap during dim-idle restores full brightness instantly
      _notes:_
- [ ] tap during deep-idle restores full brightness instantly
      _notes:_
- [ ] no flash or jump on wake — feels like a smooth resume
      _notes:_

### combined behaviour (the real test)

- [ ] watched the shoe sit idle for 5 uninterrupted minutes — felt calming, not boring
      _subjective notes:_
- [ ] tapped to wake, cycled through modes, let it idle again — full loop felt natural
      _notes:_
- [ ] showed the dim-down + wake behaviour to one outsider — what did they say?
      _outsider observation:_

### serial debug verification

- [ ] serial monitor shows `private = on/off` lines on triple-tap
      _notes:_
- [ ] mode and private state log on startup matches the EEPROM-saved state
      _notes:_

### portfolio archive

- [ ] timelapse / long-exposure photo or video of the dim-down (full → deep-idle)
      _filename / location:_
- [ ] short video showing public ↔ private toggle in a real-feeling context
      _filename / location:_

---

## phase 7 — safety & reliability

> no firmware-side deliverable. this phase is structured stress, durability, and safety testing.
> what success looks like: the wearable can fail in ordinary ways without becoming dangerous, and you have written evidence that you have actually checked it.
> any item flagged **non-negotiable** below MUST pass before the shoe is worn outside a controlled bench.

### battery safety (NON-NEGOTIABLE items first)

- [ ] **battery is in the external heel module, never under any compression zone** _(non-negotiable)_
      _notes / photo of placement:_
- [ ] **battery cannot be punctured by foot-pressure or normal wear** _(non-negotiable)_
      _notes:_
- [ ] full discharge cycle completed at room temp without venting or visible swelling
      _start voltage / end voltage / runtime:_
- [ ] battery voltage stays within safe range under sustained cool-mode load
      _measured voltage at 0min / 5min / 10min / 20min:_
- [ ] charging the battery doesn't cause heat above ambient + 10°C
      _measured temp during charge:_

### electrical isolation

- [ ] **no exposed conductors anywhere on the interior of the shoe** _(non-negotiable)_
      _notes:_
- [ ] all solder joints heat-shrunk or otherwise insulated
      _notes:_
- [ ] no metal pin can contact the foot, sock, or insole
      _notes:_
- [ ] sweat exposure test: brief moisture contact does not short anything
      _notes / methodology:_

### power switch & emergency-off

- [ ] **power can be killed instantly via the external switch** _(non-negotiable)_
      _verified by:_
- [ ] switch is reachable without removing the shoe
      _switch type + location:_
- [ ] switch has a clear "off" position with tactile feedback
      _notes:_

### wire strain & flex testing

- [ ] flex test on the data wire (D6 → strip): bend back and forth 100 cycles
      _observed: any flicker, broken trace, intermittent contact?_
- [ ] flex test on the piezo wire: 100 cycles
      _observed:_
- [ ] flex test on the power wires (5V + GND): 100 cycles
      _observed:_
- [ ] tug test on every solder joint with mild pull
      _any joints that pulled free:_
- [ ] reinforcement applied where needed (heat-shrink, hot glue, cable ties)
      _what was reinforced and how:_

### enclosure robustness

- [ ] heel module survives a 0.5m drop onto a hard floor (without battery for safety)
      _outcome:_
- [ ] heel module stays seated during ordinary walking
      _notes:_
- [ ] enclosure can be opened for service without breaking
      _notes:_
- [ ] no rattle or loose component sound during walking
      _notes:_

### thermal behaviour

- [ ] cool-mode (highest current draw) run for 10 min — any hot spots?
      _hand-test or IR observations:_
- [ ] cool-mode run for 20 min — any thermal degradation, flicker, or shutdown?
      _notes:_
- [ ] arduino nano regulator does not get uncomfortably hot
      _hand-test notes:_
- [ ] LED strip stays within safe operating temp
      _notes:_

### grounding stability under flex

- [ ] LED strip data line stays clean when shoe flexes during walking
      _no mid-walk flicker observed for 60s of walking:_
- [ ] common ground (battery / strip / arduino) holds under flex
      _notes:_

### fuse / overcurrent decision

- [ ] decided whether to add an inline fuse on the 5V rail
      _decision + reasoning + chosen fuse rating if yes:_
- [ ] decided whether to add a polyfuse instead (resettable)
      _decision + reasoning:_

### durability under realistic use

- [ ] 1 hour continuous walking — shoe still functional, no failed components
      _notes:_
- [ ] 50 power-cycles in a row — EEPROM persists correctly each time
      _notes:_
- [ ] 100 mode-switching gestures in succession — no crashes or hangs
      _notes:_
- [ ] one full day of normal wear (or as close as feasible)
      _failures observed:_

### portfolio archive

- [ ] photo: any reinforcement / heat-shrink work added during this phase
      _filename:_
- [ ] thermal hand-test or IR photo of the shoe under cool-mode after 20 min
      _filename:_
- [ ] flex-test video (bend the shoe 100x, then test LEDs work)
      _filename:_
- [ ] written paragraph per non-negotiable item, explaining how it was tested
      _link or excerpt:_

---

## phase 8 — aesthetic refinement

> mostly physical. small firmware tweaks possible (palette, timing curves) — log any that happen below.
> what success looks like: a shoe that reads as **intentional** rather than improvised, both up close and from across a room.
> there are no objectively correct answers in this phase. honest decisions matter more than ticked boxes.

### enclosure design

- [ ] heel module material chosen and committed
      _material + finish + reasoning:_
- [ ] enclosure shape feels integrated with the shoe (not stuck-on)
      _notes:_
- [ ] enclosure removable for service without looking like an afterthought
      _notes:_
- [ ] decided whether to seal the enclosure or leave it removable
      _decision + reasoning:_

### LED diffusion experiments

- [ ] tried bare LEDs — observation
      _notes / photo:_
- [ ] tried frosted silicone tube — observation
      _notes / photo:_
- [ ] tried translucent fabric overlay — observation
      _notes / photo:_
- [ ] tried 3D-printed channel + diffuser — observation
      _notes / photo:_
- [ ] **chose** a diffusion approach
      _final decision + reasoning:_
- [ ] comparison photos captured for portfolio
      _filenames:_

### visible electronics philosophy

- [ ] decided whether wires are hidden or shown
      _decision + reasoning:_
- [ ] if shown, treatment chosen (cable management, intentional routing, sleeving)
      _notes:_
- [ ] if hidden, routing path chosen (inside shoe wall, under sole, behind heel cup)
      _notes:_

### visual symmetry

- [ ] (if dual shoe) left and right look like a designed pair
      _notes:_
- [ ] (if single shoe) the modded shoe looks intentional next to the un-modded one
      _notes:_
- [ ] LED placement is consistent left/right
      _notes:_

### visual identity

- [ ] enclosure colour / finish chosen — does it match the shoe?
      _decision:_
- [ ] decided whether to add any branding / logo / mark
      _decision + reasoning:_
- [ ] overall aesthetic reads as ___ (one word — fill in)
      _word:_

### animation timing polish (firmware tweaks log)

- [ ] watched all six modes for at least 30s each, in real wear context
      _notes per mode:_
- [ ] tweaks made to `*_PERIOD_MS` constants
      _what changed + before/after feeling:_
- [ ] tweaks made to mode colour values in `modes.h`
      _what changed + before/after feeling:_
- [ ] tweaks made to easing curves in `animations.h`
      _what changed + before/after feeling:_

### the from-across-the-room test

- [ ] modes still emotionally readable after diffusion treatment
      _notes:_
- [ ] modes still emotionally readable from 3m / 5m / 10m
      _notes:_
- [ ] showed final aesthetic to outsider, asked "what does this shoe seem to be"
      _outsider response:_

### portfolio archive

- [ ] before/after photos: any aesthetic decision (diffusion, wire routing, enclosure)
      _filenames:_
- [ ] full-system hero shot of the finished phase 8 shoe
      _filename:_
- [ ] one short video walking with the finished shoe
      _filename:_
- [ ] written paragraph: "the aesthetic decisions i made and why"
      _link or excerpt:_

---

## phase 9 — portfolio system

> no firmware-side deliverable. the deliverable here is the portfolio itself: five working documents in `portfolio/` plus the photo/video archive captured across phases 1–8.
> what success looks like: a stranger can read the five portfolio files plus the photo archive and understand the project well enough to discuss it intelligently — without needing the firmware, the changelog, or a live demo.

### portfolio documents — self-review

- [ ] read `portfolio/system-architecture.md` end-to-end. is the dataflow diagram still accurate vs. the live firmware?
      _notes / drift to fix:_
- [ ] read `portfolio/interaction-storyboard.md`. does each scene still match real behaviour after the latest firmware?
      _notes:_
- [ ] read `portfolio/emotional-mapping.md`. do the colour values match `firmware/main/modes.h`?
      _notes / discrepancies:_
- [ ] read `portfolio/gesture-vocabulary.md`. do the timings (`MULTI_TAP_WINDOW_MS`, walking budget) still match `config.h`?
      _notes:_
- [ ] read `portfolio/photography-plan.md`. is every phase's shot list reachable given what's actually been captured?
      _shots still missing:_

### portfolio documents — outsider review

- [ ] handed all five files to **one outsider** with no context. what did they understand? what didn't land?
      _person 1 — what they got, what they missed:_
- [ ] handed all five files to **a second outsider**. same exercise.
      _person 2 — what they got, what they missed:_
- [ ] noted any document that consistently fails to land
      _file + how it should be rewritten:_

### photo / video archive completeness

- [ ] phase 1 archive items present (see `photography-plan.md` § phase 1)
      _missing items:_
- [ ] phase 2 archive items present
      _missing items:_
- [ ] phase 3 archive items present (six mode clips, six stills, transition video, environment comparison)
      _missing items:_
- [ ] phase 4 archive items present
      _missing items:_
- [ ] phase 5 archive items present (piezo under insole, LED routing, heel module open/closed, walking video, iteration archive)
      _missing items:_
- [ ] phase 6 archive items present (idle timelapse, wake clip, public/private toggle clip)
      _missing items:_
- [ ] phase 7 archive items present (flex test, thermal, reinforcement, non-negotiable proof)
      _missing items:_
- [ ] phase 8 archive items present (diffusion comparison, hero shot, finished walking video, wire-visibility paired shot)
      _missing items:_
- [ ] master archive folder organised by phase, every file lower_case_underscored, no `IMG_xxxx` filenames anywhere
      _path / structure notes:_
- [ ] every photo has a one-line caption written somewhere (in validate.md under its phase, or in a sidecar `.txt`)
      _convention chosen:_

### portfolio spread mockups

- [ ] sketched a one-page mockup of the **system-architecture** spread
      _filename / location:_
- [ ] sketched the **interaction-storyboard** spread (1–2 pages, illustrated frame by frame)
      _filename / location:_
- [ ] sketched the **emotional-mapping** spread (per-mode panel: palette swatch + timing curve + photo)
      _filename / location:_
- [ ] sketched the **gesture-vocabulary** spread
      _filename / location:_
- [ ] sketched the **process / failure archive** spread (the unsexy but credible content from phases 5/7)
      _filename / location:_

### cross-link audit

- [ ] every internal link in the README resolves (no 404s in markdown previews)
      _broken links found:_
- [ ] every internal link in `portfolio/README.md` resolves
      _notes:_
- [ ] every internal link in `experiments/README.md` resolves
      _notes:_
- [ ] `validate.md` references in README still point at sections that exist
      _notes:_

### portfolio archive (meta — yes, the portfolio gets a portfolio entry)

- [ ] photo of the printed portfolio (or screenshot of the digital spread) at presentation quality
      _filename:_
- [ ] short paragraph: "the project, in one screen" — the elevator narrative
      _link or excerpt:_
- [ ] reflection paragraph: "what this project taught me about wearable interaction design"
      _link or excerpt:_

---

## phase 10 — experimental expansion

> no firmware-side deliverable. each entry in `experiments/` is a paper design, not an implementation. phase 10 validation is the **decision-making**: which proposals get pursued, which get parked, which get killed — and the reasoning for each.
> what success looks like: each of the four proposals has an explicit, dated decision attached. nothing sits in limbo.

### per-proposal decisions

- [ ] **adaptive-brightness** — decision logged
      _decision (pursue / defer / kill):_
      _date:_
      _reasoning:_
      _if pursued — parts ordered, sketch started:_
- [ ] **haptic-feedback** — decision logged
      _decision:_
      _date:_
      _reasoning:_
      _if pursued — parts ordered, sketch started:_
- [ ] **motion-reactive** — decision logged
      _decision:_
      _date:_
      _reasoning:_
      _if pursued — accelerometer chosen (MPU6050 vs ADXL345), `motion_debug.ino` written:_
- [ ] **dual-shoe-sync** — decision logged
      _decision:_
      _date:_
      _reasoning:_
      _if pursued — MCU upgrade decision (stay AVR with nRF24L01+ vs ESP32):_

### sequencing decision

- [ ] reviewed the recommended order in `experiments/README.md` (adaptive → haptic → motion → dual-sync)
      _agreed / overridden:_
- [ ] picked **the next one** to actually build, if any
      _name + reasoning:_
- [ ] committed to "no two at once" (each expansion is its own phase, not a parallel sprint)
      _notes:_

### prototyping notes (only fill in if you actually pursue an expansion)

- [ ] parts arrived; bench-test before integration
      _notes:_
- [ ] standalone debug sketch written (analog of `piezo_debug` for the new sensor)
      _filename:_
- [ ] integrated into `firmware/main/` behind a `#ifdef ENABLE_X` guard so phase 1–6 still flashes cleanly
      _notes:_
- [ ] `validate.md` updated with a new phase section for the expansion (e.g. "phase 11 — adaptive brightness")
      _section added:_
- [ ] `portfolio/emotional-mapping.md` (or a new `portfolio/` doc) updated if the expansion adds emotional capability
      _notes:_

### proposals deferred / killed (write down so future-you doesn't re-derive the rejection)

- [ ] reviewed the "what's NOT in here" list in `experiments/README.md` (bluetooth app, AI emotion, camera, GPS)
      _any to revisit?_
- [ ] new candidate ideas surfaced during phase 1–8 work, written up to the experiments template
      _filenames added:_

### portfolio archive (only if an expansion was actually built)

- [ ] short video of the expansion in real wear
      _filename:_
- [ ] before/after comparison: same scene, with and without the expansion
      _filename:_
- [ ] one paragraph: "what new emotional capability this unlocked" — written honestly, including failures
      _link or excerpt:_

---

## conventions for future phases

each phase from phase 6 onward will add a new section here as its hardware-side checks come due. the structure stays the same:

1. **what sketch is under test** — so future-you knows exactly which `.ino` produced these results
2. **what success looks like** — a single paragraph of observable behaviour
3. **grouped checklists** — short, specific, observable items
4. **notes / measurements / filenames** under every item — empty strings to fill in

if a check fails: cross it out, write what changed, re-add it below. never delete failed history.
