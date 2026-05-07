# gesture vocabulary

> the gestures the system recognises, what they mean, and the design trade-offs behind that mapping.

the project's input vocabulary is intentionally tiny. the wearer has one piezo, no buttons, no touch surfaces. the question phase 4 had to answer was: **what are the smallest possible distinguishable gestures, and how many meanings can each carry?**

## the recognised gestures

| gesture       | trigger                                          | meaning                                      | latency to fire           |
| ------------- | ------------------------------------------------ | -------------------------------------------- | ------------------------- |
| single-tap    | one tap, then `MULTI_TAP_WINDOW_MS` (400ms) of silence | advance to next emotional mode               | ~400ms after the tap      |
| double-tap    | two taps within window, then 400ms silence       | step back to previous mode                   | ~400ms after the second tap |
| triple-tap    | three taps within window                         | toggle public ↔ private mode                 | immediate (on third tap)  |
| (rapid taps)  | more than `WALKING_TAP_BUDGET` (3) in 2s         | walking detected — gestures muted for 1s    | n/a (no positive trigger) |
| (long stillness) | no taps for 30s / 2min                        | begin idle / deep idle dim-down              | gradual                    |
| (any classified gesture during idle) | (described above)                | wake to full brightness                      | instant                    |

---

## the design rationale

### why these mappings

the mapping had to satisfy three constraints simultaneously:

1. **memorable** — a wearer should be able to remember what each gesture does without writing it down.
2. **physically natural** — taps that are easy to perform deliberately, hard to perform accidentally.
3. **emotionally consistent** — the gestures should map to actions that "feel right" given the affordance.

the chosen mapping leans on a small bit of intuition pumped from common UI:

- **single = forward** — the universal "next" action, like clicking a slideshow remote
- **double = back** — the universal "I overshot, undo" action
- **triple = privacy** — distinct from the navigation pair, signals "different category of action"

a wearer who hasn't been told the gestures will discover this in three taps:

> tap once. *the shoe changes.* okay, that's "next."  
> tap twice. *the shoe goes back.* okay, double = "back."  
> what does triple do? *oh, it dims itself.* that's a different kind of thing.

that's a 30-second learning curve, which is the bar for any wearable interaction.

### why single fires last (the latency trade-off)

every multi-tap classifier has the same fundamental tension:

- if you fire SINGLE immediately on the first tap, you can't distinguish it from the start of a double-tap or triple-tap
- if you wait for the multi-tap window to expire, single-tap has built-in latency

we chose latency. SINGLE_TAP fires **after** the window expires (~400ms after the tap). the cost: every mode change has a noticeable delay between gesture and visual response.

we accept this because:
1. the cross-fade is 600ms long — it would still feel "delayed" even with zero classifier latency
2. cancelling a single-tap mid-action (if the user is actually starting a triple) feels worse than waiting
3. the eased fade itself is the visual feedback — the wearer doesn't need an instant "tap acknowledged" flash

an alternative — fire a tentative SINGLE on the first tap, then "cancel" it visually if a second tap arrives — was considered and rejected. it would mean every triple-tap shows a partial mode change before reverting. emotionally jarring.

triple-tap, by contrast, fires *immediately* on the third tap. no waiting. there's no quadruple to disambiguate against, so there's nothing to wait for.

### why no quadruple, no quintuple, no patterns

we could have added a quadruple-tap, a long-press, a rhythmic pattern (tap-tap...long-pause...tap), and so on. we did not. reasons:

- **piezo can't sense pressure** — long-press would need a force-sensitive resistor, which is a hardware addition deferred to phase 10
- **quadruple-tap is hard to perform deliberately** — three is roughly the limit of "I can do this without thinking about my count"
- **emotional design isn't gated by gesture count** — adding more gestures doesn't make the project more interesting; it makes the system harder to learn

the gesture vocabulary will grow, but only when there's a real need. the test for adding a new gesture: **what new emotional capability does it unlock?** if you can't answer in one sentence, don't add it.

### the walking-mute problem

walking generates piezo activity. heel-strikes during a normal gait look a lot like deliberate taps to a simple threshold-based classifier. a phase 4 firmware that took every walking impact as a tap would cycle through modes randomly all day.

the mitigation we shipped is intentionally crude: **if more than 3 taps arrive within a 2-second window, suspect walking and mute gestures for 1 second.** crude because:
- it can't distinguish "fast double-tap then walking" from "all walking"
- it doesn't learn the wearer's gait
- it adds latency (the first 1-second mute window after walking starts means a deliberate tap right after stopping won't register)

we shipped it anyway because it's debuggable, predictable, and significantly better than nothing. real walking-noise rejection is a research-grade problem and the right approach probably involves rolling-buffer pattern matching or a small machine learning classifier — both of which are valid phase 10+ extensions.

what we *don't* claim: that this mitigation makes the shoe robust for serious walking use. it makes the shoe survive walking. that's a different thing.

---

## the wake gesture

idle dim-down adds a kind of meta-gesture: **any classified gesture during idle wakes the shoe.** this isn't really a separate gesture — it's just the side-effect that every gesture has. but it deserves explicit framing because it's how the wearer "starts a session."

the alternative (a dedicated wake gesture, e.g. "first tap wakes, doesn't classify") was considered and rejected. having the first tap also do something means the wearer's intent is honoured immediately, with no feeling of "I had to do an extra step to start."

---

## the future expansions worth considering

every gesture below is a phase 10+ proposal, currently un-implemented. each one would have to justify itself per the "what new emotional capability does this unlock?" test.

| candidate gesture          | possible meaning                                | what it requires                  |
| -------------------------- | ----------------------------------------------- | --------------------------------- |
| long-press                 | "save this moment as a snapshot"                | force-sensitive resistor (FSR)    |
| rhythmic tap pattern       | morse-like custom command sequences             | rolling-buffer pattern matcher    |
| stillness gesture (no motion for N seconds) | trigger a meditative/ambient mode | accelerometer (phase 10)          |
| mirrored tap (both shoes)  | sync expression across the pair                 | dual-shoe wireless (phase 10)     |
| paired wearer tap          | "emotional handshake" — two wearers tap simultaneously, both shoes pulse | BLE proximity (phase 10) |

these are tracked in `experiments/` (phase 10 design proposals). most of them would change the project's character significantly — for the better, but as deliberate design moves, not feature creep.
