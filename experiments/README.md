# experiments

> phase 10 design proposals. each file in this folder is a forward-looking architecture for an extension that has *not* been built yet.

these are **not implementations**. they are deliberate design documents — written so that any one of them could be picked up later, by future-you or a collaborator, with enough context to start building without re-deriving the constraints.

> not to be confused with `firmware/experiments/` — that folder holds working Arduino sketches (the phase 1 hello-world archive, the piezo debug tool). this folder holds **paper designs for hardware-altering expansions**.

---

## the proposals

| file                              | what it adds                                                                |
| --------------------------------- | --------------------------------------------------------------------------- |
| [dual-shoe-sync.md](./dual-shoe-sync.md) | two shoes mirroring each other's emotional state wirelessly                    |
| [motion-reactive.md](./motion-reactive.md) | accelerometer-driven behaviour: walking modulates intensity; jumping triggers special animations |
| [adaptive-brightness.md](./adaptive-brightness.md) | LDR-based ambient adaptation — shoe scales to room light, not always the same brightness |
| [haptic-feedback.md](./haptic-feedback.md) | small vibration motor for tactile gesture confirmation                        |

each proposal contains:
- **what it adds** — one paragraph, plain language
- **why it's interesting** — the emotional / interaction design value, not just "more sensor"
- **parts list** — what you'd buy
- **architecture** — how it integrates with the existing firmware
- **risks and open questions** — what isn't obvious, what could fail
- **decision points** — the calls that would need to be made before building

---

## the principle

> the test for whether an expansion is worth building: **what new emotional capability does this unlock?**

every proposal in this folder is designed to pass that test. each one introduces a kind of expression or interaction that the current system cannot produce. if a proposal here just adds "more features," it's badly written and should be rewritten or deleted.

## the order

if you were going to actually pursue these, the recommended order is:

1. **adaptive-brightness** — easiest to add, immediately improves real-world wearability
2. **haptic-feedback** — small hardware change, big UX win, low risk
3. **motion-reactive** — bigger architectural shift; needs an accelerometer and rolling state
4. **dual-shoe-sync** — most interesting, biggest commitment; probably needs an MCU upgrade

each builds on the last in terms of complexity and impact. don't try to do all four in one phase 10 sprint — each is a phase in itself.

## what's NOT in here

a few directions that have come up and been deliberately omitted:

- **bluetooth app control** — the project is about gesture-driven interaction, not phone-tethered. adding an app would dilute the core idea unless it served a real interaction need beyond mode switching. if a proposal emerges where the phone *adds* something that gestures can't, write it. otherwise, no.
- **AI emotional interpretation** — speculative without a real signal source. would make sense as a phase 11+ once the project has accelerometer + dual-shoe + months of usage data to feed it.
- **camera input** — too invasive, too power-hungry, breaks the "subtle wearable" framing.
- **GPS / location awareness** — privacy concerns, doesn't add emotional capability.

---

## conventions for adding a new proposal

if you have an idea for an expansion not listed here:

1. read the four existing proposals first. notice their shape.
2. write yours to the same template (what / why / parts / architecture / risks / decisions)
3. add it to the table at the top of this README
4. add an entry in the "the order" section above with your honest assessment of when it's worth tackling

if you can't write it to the template, it's not ready to be a proposal yet. that's useful information — it means the idea needs more specificity before it's actionable.
