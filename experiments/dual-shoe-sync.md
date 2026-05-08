# dual-shoe sync (phase 10 design proposal)

> two shoes mirroring each other's emotional state wirelessly. when one expresses, the other expresses with it.

## what it adds

right now the system is one-shoe. each shoe runs independently. dual-shoe sync means: a tap on the left heel changes both shoes, and the wearer can also pair with another wearer's shoe and "handshake" emotionally, both shoes pulsing a shared colour briefly.

two distinct features sit under this umbrella:

1. **paired left/right (single wearer)**: both shoes always show the same emotional state, which solves the asymmetry problem of one-shoe-only wear.
2. **paired with another wearer**: temporary shared expression between two people's shoes. social signaling between humans, mediated by their shoes.

## why it's interesting

the most powerful emotional moments in this project are *shared* ones. one wearer expressing to themselves is meaningful; two wearers expressing *together* is a different category of meaningful. dual-shoe sync is the architectural step that makes both scales of "togetherness" possible.

it's also the feature that lets the project register as a *product* in its own right. paired wearables are a coherent product category in a way a lone wearable rarely is.

## parts list

option A: **stay on AVR, add radios**:
- 2 × nRF24L01+ wireless modules (~$3 each)
- some glue caps + level-shifter resistors

option B: **upgrade to ESP32 nano** (recommended):
- 2 × Arduino Nano ESP32 (~$15 each)
- ESP-NOW built-in, no extra radio module
- much more memory and CPU headroom for future expansions
- this is the natural home of phase 10+ work

the rest of the BOM stays identical: piezo, WS2812B strip, capacitors, resistors.

## architecture

### data flow (paired left/right)

```
   left shoe                           right shoe
   ─────────                           ─────────
   piezo → gestures.h                  piezo → gestures.h
       │                                   │
       ▼                                   ▼
   main.ino                            main.ino
       │                                   │
       │ on classified gesture            │ on classified gesture
       │ broadcast(STATE_UPDATE)          │ broadcast(STATE_UPDATE)
       │                                   │
       ▼ ESP-NOW                  ESP-NOW ▼
       └────────────► partner ◄───────────┘
                          │
                          ▼
                   handle peer state
                   = adopt their mode + private flag
```

both shoes broadcast `(mode, isPrivate, sequence#)` on every classified gesture. each shoe receiving a peer update adopts the new state and starts a cross-fade. last-writer-wins on sequence number.

### data flow (paired with another wearer)

an explicit "handshake gesture" (proposal: quadruple-tap) initiates pairing. both wearers tap quadruple within a small window of each other. the shoes negotiate a 30-second shared mode (e.g., a special "handshake" rainbow that no individual shoe can produce). after the window expires, each returns to its own state.

this is a deliberate pairing on demand, with both wearers explicitly opting in to the shared expression each time.

### module additions

- new file: `firmware/main/sync.h`
  - `syncBegin()`: bring up ESP-NOW, register peer MAC
  - `syncBroadcastState(mode, isPrivate, seq)`
  - `syncUpdate()`: process incoming peer state, return events to main.ino

main.ino calls `syncBroadcastState()` after every `changeMode()` and `persistPrivate()`. main.ino calls `syncUpdate()` once per loop tick.

### config additions

```cpp
#define PEER_MAC_ADDR  {0x...} // statically configured, or learned via pairing
#define HANDSHAKE_GESTURE GESTURE_QUADRUPLE_TAP
#define HANDSHAKE_WINDOW_MS 5000
```

## risks and open questions

- **sync drift**: if the two shoes' clocks differ slightly, breathing curves drift apart. need either (a) periodic time-sync messages or (b) a phase-locked render that uses a peer-shared epoch. (a) is simpler.
- **range**: ESP-NOW has ~50m line-of-sight range, but two shoes on one wearer are rarely line-of-sight (the body blocks the signal). pairing on the same person works in practice; we'd need to verify.
- **power**: ESP32 in active wifi/ESP-NOW mode draws significantly more than ATmega328P. battery runtime estimate drops from ~6 hours to ~2 hours. either accept it or sleep the radio between events.
- **failure mode**: what does a shoe do when its peer is offline? naive answer: act as a single shoe. the trouble is, if a wearer is used to seeing their shoes mirror, asymmetric behaviour reads as "broken." it needs a small "peer disconnected" indication (subtle desaturation? slower transitions?).
- **MAC pairing UX**: how do two shoes initially learn each other's MAC? options: (a) hard-code at compile time (shitty UX), (b) pairing button on first boot, (c) the handshake gesture itself doubles as pairing.
- **wearer-to-wearer pairing duration**: 30 seconds is a guess. real test data needed.

## decision points

before building, you'd need to answer:

1. **MCU upgrade or stay on AVR?** ESP32 unlocks several proposals (this one, motion-reactive's ML extensions, future bluetooth-app integration). recommend yes. cost is ~$15 + a port of the firmware (most of which is portable).
2. **how is the peer learned?** static MAC vs in-band pairing. recommend in-band pairing on first boot via the handshake gesture itself.
3. **what does the handshake animation look like?** designed once, lives forever. needs same emotional-design care as the six base modes. write a 7th mode-design entry in `portfolio/emotional-mapping.md`.
4. **does pairing also share private/public state?** subtle: if I'm in private and my shoe pairs with yours, do you become private too? recommendation: no, each wearer keeps their own privacy setting.
5. **time-sync strategy?** (a) periodic broadcast every 60s, (b) shared epoch on connection, (c) ignore drift.

## one-paragraph elevator pitch

> two shoes that share emotional state. on a single wearer, one tap changes both. between two wearers who choose to pair, the shoes can briefly express together with a 30-second shared rainbow, then return to their own emotional baselines. it turns a personal wearable into a relational one while keeping both halves feeling honest.
