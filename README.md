# nestri — NES-style triangle oscillator for the Korg minilogue xd

A custom oscillator for the **Korg minilogue xd**, built with Korg's
[logue SDK v1](https://github.com/korginc/logue-sdk). It morphs between a clean
analog triangle and the **stepped, bit-crushed triangle of the NES / Famicom**
sound chip (2A03), with the step resolution under your fingers.

- **File:** `triangle.mnlgxdunit`
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

| Knob              | Function                                                    |
|-------------------|-------------------------------------------------------------|
| **SHAPE**         | Morph smooth triangle (0) → stepped NES triangle (max); LFO-modulatable |
| **SHIFT+SHAPE**   | Step resolution: left = coarse (2 steps), center = NES (16), right = fine (128) |

There are no edit-menu parameters — everything lives on SHAPE and SHIFT+SHAPE.

## How it works

- The triangle is generated directly from the phase and quantized onto a grid of
  `levels = 2^(1 + shift·6)` steps, so SHIFT+SHAPE spans 2 … 128 levels (16 at
  center = the real NES 4-bit triangle).
- SHAPE crossfades between the raw triangle and its quantized version.
- **Sample rate:** 48 kHz, mono oscillator, q31 output with headroom.
- Core per sample:
  ```cpp
  const float smooth    = (phase < 0.5f) ? (4.f * phase - 1.f) : (3.f - 4.f * phase);
  const float quantized = clip1f(roundf(smooth / step) * step);
  const float sig       = smooth + shape * (quantized - smooth);
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

The resulting `triangle.mnlgxdunit` ends up in the project folder.

## Loading onto the device

1. Open the **minilogue xd Sound Librarian**, drag `triangle.mnlgxdunit` onto a
   free slot under *User Oscillators* and transfer it.
2. On the device, set MULTI ENGINE to **USR** and pick `nestri` with TYPE.
3. Turn VCO1/VCO2 down and MULTI up to hear the oscillator on its own.
4. Turn SHAPE for the chiptune amount, SHIFT+SHAPE for the step resolution.

minilogue xd firmware ≥ 2.00 is required for API 1.1-0.

## License

Private hobby project, built with the Korg logue SDK (BSD 3-Clause).
