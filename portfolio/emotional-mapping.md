# emotional mapping

> for each of the six modes: the colour palette, the timing curve, what feeling it's trying to communicate, and what it must NOT be confused with.

the rule for any mode in this project: **an outside observer should be able to feel its emotion within five seconds, without prompting**. if they can't, the mode is wrong — colour, timing, or both. these mappings are the design rationale, not the spec.

---

## calm

| property            | value                                |
| ------------------- | ------------------------------------ |
| RGB                 | `(40, 90, 200)`                      |
| feeling             | grounded, breathing, settled         |
| timing              | 4-second sinusoidal breathing cycle  |
| brightness floor    | 25/255 (never fully dark)            |
| brightness ceiling  | `MAX_BRIGHTNESS` = 120/255           |

**why this colour**: cool blue but not cold. lower red than green channels make it slightly leaning toward cyan, which reads as "alive water" rather than "sterile sky."

**why this timing**: 4 seconds is roughly the resting respiratory cycle of a calm adult. the eye reads it as breathing without consciously identifying it as such.

**must not be confused with**: alert (red, fast). flow (cyan, traveling). a calm mode that drifted toward cyan would lose its grounding quality and start to feel like flow.

**what it's for**: this is the resting emotional state. the shoe is OK. nothing needs attention. the wearer is fine.

---

## happy

| property            | value                                |
| ------------------- | ------------------------------------ |
| RGB                 | `(255, 140, 30)`                     |
| feeling             | warm, social, open, generous         |
| timing              | 1.5-second pulse with fast attack, slow decay |
| brightness floor    | 40/255                               |
| brightness ceiling  | `MAX_BRIGHTNESS` = 120/255           |

**why this colour**: warm orange with strong red and notable green. it reads as "skin-tone-adjacent" warmth — the colour of a hearth, not a stoplight. green channel is critical: pure red-orange (no green) reads as alert, not happy.

**why this timing**: 1.5s is faster than calm but not anxious. the asymmetric envelope (fast rise, slow decay) gives each pulse the shape of a small "yes" — a warm exhale rather than a heartbeat.

**must not be confused with**: alert. without the green channel and slower timing, happy and alert would converge. that would be catastrophic for emotional readability.

**what it's for**: when the wearer wants the shoe to communicate that they're open, social, generous, "in a good mood." this is the mode for parties, friendly meetings, feeling well.

---

## alert

| property            | value                                |
| ------------------- | ------------------------------------ |
| RGB                 | `(255, 30, 30)`                      |
| feeling             | sharp, attentive, "look at me"       |
| timing              | 4 Hz square-wave blink (250ms period) |
| brightness floor    | 20/255 (never fully off — gamma helps)|
| brightness ceiling  | `MAX_BRIGHTNESS` = 120/255           |

**why this colour**: pure saturated red, minimal green or blue. the universal attention colour. there is no ambiguity about what red flashing means.

**why this timing**: 4 Hz is fast enough to communicate urgency but well below seizure-trigger rates (which start around 10 Hz and intensify at 15-25 Hz). 4 Hz also lines up with how the human visual system tracks "alert" — emergency vehicle lights typically operate in this range.

**must not be confused with**: nothing. alert needs to be unmistakable. that's why it's the only mode without easing — square-wave blink is intentional. easing would soften the urgency.

**what it's for**: the wearer wants the shoe to say "pay attention." not necessarily danger — could be social signaling ("I see you"), could be playful ("watch this"). but the energy is high.

---

## sleepy

| property            | value                                                  |
| ------------------- | ------------------------------------------------------ |
| RGB                 | `(80, 30, 120)`                                        |
| feeling             | low energy, drifting, withdrawn, cozy                  |
| timing              | 8-second very slow drift                               |
| brightness floor    | 8/255                                                  |
| brightness ceiling  | `SLEEPY_MAX_BRIGHTNESS` = 60/255 (intentionally capped)|

**why this colour**: violet with a touch more red than blue. desaturated relative to the other modes — the colour of dusk light, not midnight. fully saturated purple would feel performative; this is intentionally faded.

**why this timing**: 8 seconds is roughly twice as slow as calm breathing. it reads as "deeper rest" — not sleep, but the withdrawal that precedes it. the long cycle gives the eye time to almost forget the shoe is there before it returns.

**why the brightness cap**: sleepy maxes out at half the brightness of every other mode. this is the only mode where the ceiling is mode-specific. without that cap, sleepy would still dominate a dark room and undercut its own message.

**must not be confused with**: calm (blue, faster, brighter). a sleepy that drifted toward blue would just become a sad calm. keeping it firmly in the violet range is what gives it its own emotional territory.

**what it's for**: when the wearer wants the shoe to communicate "I'm here, but I'm low energy right now." good for late evenings, post-meal slumps, deliberate rest.

---

## flow

| property            | value                                                       |
| ------------------- | ----------------------------------------------------------- |
| RGB                 | `(40, 200, 220)`                                            |
| feeling             | focused, in-motion, flowing, productive                     |
| timing              | wave traveling along strip at 1 LED per 100ms; width 6 LEDs |
| brightness floor    | 0 (per-pixel, where wave is absent)                         |
| brightness ceiling  | `MAX_BRIGHTNESS` = 120/255 at wave peak                     |

**why this colour**: cyan-leaning aqua. cooler than calm-blue, more saturated. reads as "water in motion" rather than "sky at rest."

**why this animation**: traveling wave instead of breathing. the entire mode is about *movement* — perpetual, smooth, unidirectional. a wave that breathed in place would lose the flow quality.

**why the wave width**: 6 LEDs (out of 30) is wide enough to feel like a body of light passing through, narrow enough that the un-lit pixels are clearly resting. a wider wave would just look like an animated baseline.

**must not be confused with**: cool (rainbow, decorative). flow is monochromatic on purpose — it's about being absorbed in one thing. cool is about play.

**what it's for**: deep work moments. the wearer is locked into something. the shoe communicates "I'm in the zone, don't break it."

---

## cool

| property            | value                                          |
| ------------------- | ---------------------------------------------- |
| RGB                 | hue-rotated rainbow (CHSV)                     |
| feeling             | playful, performative, "look at me"            |
| timing              | hue offset shifts every 30ms; full strip       |
| brightness floor    | n/a (uniform across strip)                     |
| brightness ceiling  | `MAX_BRIGHTNESS` = 120/255                     |

**why this colour scheme**: full rainbow rotation is unmistakably "tech-forward, performative, decorative." this is the only mode that doesn't have a single dominant hue — the absence of a fixed colour is the emotional content.

**why this timing**: 30ms per hue step gives a smooth-moving rainbow without being headache-inducing. faster than that and it becomes nauseating; slower and it loses its "in motion" quality.

**must not be confused with**: flow. flow is one colour traveling; cool is many colours rotating in place. they are the two motion-based modes and the difference matters — flow is about focus, cool is about play.

**what it's for**: showing off the shoe. parties, performances, moments where the wearer wants the shoe to be the main event. cool is the only mode that's intentionally extroverted.

---

## the modes as a system

read all six modes as a 2D grid:

|              | low energy   | mid energy    | high energy        |
| ------------ | ------------ | ------------- | ------------------ |
| **introvert**| sleepy       | calm          | flow               |
| **extrovert**| (none yet)   | happy         | cool / alert       |

the absence of a "low-energy extrovert" mode is intentional. that emotional space is genuinely rare and adding a sixth mode just to fill the grid would dilute the others. better to leave the shape asymmetric than to ship a mode that has no clear emotional purpose.

## what's NOT a mode (and why)

candidates that have come up and been rejected:

- **angry** — covered by alert. adding a separate red "angry" would split the emotional vocabulary.
- **sad** — covered by sleepy. the sleepy violet already does the low-energy emotional work; sad would compete for the same space.
- **excited** — would compete with happy and cool. happy is warm-excited; cool is performative-excited. excited doesn't add a new emotional reading.
- **focused** — covered by flow.
- **proud** — too narrow. proud is a derivative of happy + cool; if the wearer wants proud, they can pick either depending on context.

the test for any new mode candidate: **does it occupy emotional territory none of the existing six already covers?** if not, don't add it.
