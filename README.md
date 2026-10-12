# nestri — NES-style triangle oscillator for the Korg minilogue xd

A custom oscillator for the **Korg minilogue xd**, built with Korg's
[logue SDK v1](https://github.com/korginc/logue-sdk). It morphs between a clean
analog triangle and the **stepped, bit-crushed triangle of the NES / Famicom**
sound chip (2A03), with the step resolution under your fingers.

- **File:** `nestri.mnlgxdunit`
- **Display name on the device:** `nestri`
- **Type:** `osc` (user oscillator)

## Sound

The NES triangle channel has no volume control and no smooth slope — it steps
through a fixed set of levels, which is exactly what gives it its hollow,
chiptune bass-and-lead character. `nestri` reproduces that by quantizing an
analog triangle onto a staircase, and lets you dial how coarse that staircase is:

- **SHAPE at 0** → a smooth, continuous triangle.
- **SHAPE at max** → the same triangle quantized to hard steps (NES-style).
- Everything in between crossfades the two, so SHAPE acts as a "chiptune amount".

Because SHAPE responds to the SHAPE LFO, you can sweep from analog to 8-bit
rhythmically.

## Controls

`nestri` has two sound controls — **Morph** (smooth → stepped amount) and **Step**
(step resolution) — plus one edit-menu parameter that decides which of them sits
on the LFO-modulatable SHAPE knob.

| Knob / Param      | Function                                                    |
|-------------------|-------------------------------------------------------------|
| **SHAPE**         | The control selected by *LFO Mod*; always LFO-modulatable   |
| **SHIFT+SHAPE**   | The other control                                           |
| **LFO Mod** (edit)| `1` = SHAPE drives Morph, SHIFT+SHAPE drives Step (default) · `2` = swapped |

- **Morph:** smooth triangle (0) → stepped NES triangle (max) — the "chiptune amount".
- **Step:** step resolution — low = coarse (2 steps), center = NES (16), high = fine (128).

The minilogue xd can only route the SHAPE LFO to the SHAPE knob, so **LFO Mod**
exists to pick *which* control the LFO sweeps: the morph (default) or the step
resolution. Set it in the edit menu under MULTI ENGINE (shown as `1` / `2`).

## How it works

- The triangle is generated directly from the phase and quantized onto a grid of
  `levels = 2^(1 + shift·6)` steps, so SHIFT+SHAPE spans 2 … 128 levels (16 at
  center = the real NES 4-bit triangle).
- Morph crossfades between the raw triangle and its quantized version.
- The LFO is always added to the SHAPE knob; *LFO Mod* only decides whether that
  knob feeds Morph or Step, so either control becomes LFO-modulatable.
- **Sample rate:** 48 kHz, mono oscillator, q31 output with headroom.
- Core per sample:
  ```cpp
  const float smooth    = (phase < 0.5f) ? (4.f * phase - 1.f) : (3.f - 4.f * phase);
  const float quantized = clip1f(roundf(smooth / step) * step);
  const float sig       = smooth + morph * (quantized - smooth);
  ```

> Note: the stepped waveform is naive (not band-limited), so it aliases in high
> registers — part of the lo-fi charm, but worth knowing for lead lines.

## Building

Prerequisite: the logue SDK is cloned with its submodules and the Docker image is
built (see the [SDK README](https://github.com/korginc/logue-sdk)). This project
lives under `logue-sdk/platform/minilogue-xd/nestri/`.

From the `logue-sdk` directory:

```sh
docker/run_cmd.sh build minilogue-xd/nestri
```

The resulting `nestri.mnlgxdunit` ends up in the project folder.

## Loading onto the device

1. Open the **minilogue xd Sound Librarian**, drag `nestri.mnlgxdunit` onto a
   free slot under *User Oscillators* and transfer it.
2. On the device, set MULTI ENGINE to **USR** and pick `nestri` with TYPE.
3. Turn VCO1/VCO2 down and MULTI up to hear the oscillator on its own.
4. Turn SHAPE and SHIFT+SHAPE for the Morph / Step controls; set **LFO Mod** in
   the edit menu to choose which one the SHAPE LFO sweeps.

minilogue xd firmware ≥ 2.00 is required for API 1.1-0.

## License

Private hobby project, built with the Korg logue SDK (BSD 3-Clause).
