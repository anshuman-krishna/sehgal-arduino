# experiments

> phase 10 design proposals. each file in this folder is a forward-looking architecture for an extension that's still on paper, waiting for its turn.

these are deliberate design documents, written so any one of them can be picked up later, by future-you or a collaborator, with enough context to start building without having to re-derive the constraints.

> careful: this folder is different from `firmware/experiments/`. that one holds working Arduino sketches (the phase 1 hello-world archive, the piezo debug tool). this folder holds **paper designs for hardware-altering expansions**.

---

## the proposals

| file                              | what it adds                                                                |
| --------------------------------- | --------------------------------------------------------------------------- |
| [dual-shoe-sync.md](./dual-shoe-sync.md) | two shoes mirroring each other's emotional state wirelessly                    |
| [motion-reactive.md](./motion-reactive.md) | accelerometer-driven behaviour: walking modulates intensity, jumping triggers special animations |
| [adaptive-brightness.md](./adaptive-brightness.md) | LDR-based ambient adaptation; the shoe scales to room light instead of holding one fixed brightness |
| [haptic-feedback.md](./haptic-feedback.md) | small vibration motor for tactile gesture confirmation                        |

each proposal contains:
- **what it adds**: one paragraph, plain language
- **why it's interesting**: the emotional and interaction design payoff, beyond just adding another sensor
- **parts list**: what you'd buy
- **architecture**: how it integrates with the existing firmware
- **risks and open questions**: what isn't obvious, what could fail
- **decision points**: the calls that need to be made before building

---

## the principle

> the test for whether an expansion is worth building: **what new emotional capability does this unlock?**

every proposal in this folder is designed to pass that test. each one introduces a kind of expression or interaction the current system can't produce. a proposal that only adds "more features" is badly written and should be rewritten or removed.

## the order

if you're going to actually pursue these, the recommended order is:

1. **adaptive-brightness**: easiest to add, immediately improves real-world wearability
2. **haptic-feedback**: small hardware change, big UX win, low risk
3. **motion-reactive**: bigger architectural shift; needs an accelerometer and rolling state
4. **dual-shoe-sync**: most interesting, biggest commitment, probably needs an MCU upgrade

each builds on the last in complexity and impact. resist the urge to do all four in one phase 10 sprint. treat each as its own phase.

## what's deliberately left out

a few directions have come up and been parked on purpose:

- **bluetooth app control**: the project is about gesture-driven interaction, and a phone tether would dilute that unless it served a real need beyond mode switching. if a proposal emerges where the phone *adds* something gestures can't, write it. otherwise, leave it alone.
- **AI emotional interpretation**: speculative without a real signal source. it would make sense as a phase 11+ once the project has accelerometer data, dual-shoe history, and months of real wear to feed in.
- **camera input**: too invasive, too power-hungry, breaks the "subtle wearable" framing.
- **GPS / location awareness**: privacy concerns, and it doesn't add emotional capability.

---

## conventions for adding a new proposal

if you have an idea for an expansion that isn't listed here:

1. read the four existing proposals first. notice their shape.
2. write yours to the same template (what / why / parts / architecture / risks / decisions).
3. add it to the table at the top of this README.
4. add an entry in the "the order" section above with your honest assessment of when it's worth tackling.

if you can't write it to the template, the idea probably needs more specificity before it becomes actionable. that's useful information on its own.
