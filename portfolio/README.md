# portfolio

> the project's portfolio layer. this folder holds the documentation artifacts that explain *what was built and why*, separately from the firmware that runs the shoe.

these aren't sales documents. they're working documents — written for future-me, for collaborators, for anyone who wants to read the project as a piece of design thinking rather than a hardware bill-of-materials.

## what's in here

| file                            | what it covers                                                  |
| ------------------------------- | --------------------------------------------------------------- |
| [system-architecture.md](./system-architecture.md) | the dataflow from heel-tap to LED frame; memory budget; module map |
| [interaction-storyboard.md](./interaction-storyboard.md) | the wearer's journey, frame by frame, from sleeping shoe to expressed emotion and back |
| [emotional-mapping.md](./emotional-mapping.md) | per-mode design rationale: palette, timing, what each mode is trying to communicate |
| [gesture-vocabulary.md](./gesture-vocabulary.md) | the gestures the system recognises, what they mean, and the design trade-offs behind that mapping |
| [photography-plan.md](./photography-plan.md) | the shot list — every photo and video that should exist by the end of the project |

## what is NOT in here

- **the firmware itself** — it lives in `firmware/`
- **the validation logs** — they live in `validate.md`
- **the engineering log** — it lives in `testing/changelog.md` (gitignored, local-only)

these documents reference each of those, but they don't duplicate them. the portfolio is the explanation; the rest is the evidence.

## how to use these documents

- **for portfolio submission**: each file maps onto a portfolio spread. system-architecture is one page, interaction-storyboard is one or two, emotional-mapping is two or three, gesture-vocabulary is one, photography-plan tells you what photos to put on every other page.
- **for collaboration**: hand someone these five files and they'll understand the project well enough to contribute meaningfully without needing to read the firmware.
- **for future-you**: when you come back to this project in six months and don't remember why a mode is exactly that shade of blue, the answer is in `emotional-mapping.md`.

## philosophy

> the strongest layer of this project is not electronics. it is human gesture becoming emotional expression.

the portfolio is here to make sure the documentation reflects that priority. wiring diagrams and code listings are evidence. the rationale for *why this exists at all* is the actual story.

every file in this folder should answer at least one of:

- what does this part of the project communicate?
- why does it work the way it does?
- what was considered and rejected on the way to this design?

if a file in this folder reads like a technical specification, it has lost the plot.
