/*
 * nestri.cpp
 *
 * NES-style triangle oscillator for the minilogue xd (logue SDK v1).
 *
 * Two sound controls:
 *   Morph: smooth triangle (0) → NES 4-bit stepped (max)
 *   Step : step size — low = coarse (2 steps), mid = NES (16), high = fine (128)
 *
 * Edit param "LFO Mod" chooses which control sits on the LFO-modulatable SHAPE knob:
 *   1 (default): SHAPE = Morph (LFO), SHIFT+SHAPE = Step
 *   2 (swapped): SHAPE = Step  (LFO), SHIFT+SHAPE = Morph
 */

#include "userosc.h"

static float   s_phase       = 0.f;
static float   s_shape       = 0.f;   // SHAPE knob position [0,1]
static float   s_shift_shape = 0.5f;  // SHIFT+SHAPE knob position [0,1]
static uint8_t s_swap        = 0;     // 0: SHAPE=Morph | 1: SHAPE=Step

void OSC_INIT(uint32_t platform, uint32_t api)
{
  (void)platform;
  (void)api;
  s_phase       = 0.f;
  s_shape       = 0.f;
  s_shift_shape = 0.5f;
  s_swap        = 0;
}

void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{
  const float w0 = osc_w0f_for_note((params->pitch) >> 8, params->pitch & 0xFF);

  // SHAPE knob gets the LFO added; SHIFT+SHAPE is static. The swap flag decides
  // which control (Morph / Step) each knob drives, so the LFO can modulate either.
  const float shape_knob = clip01f(s_shape + q31_to_f32(params->shape_lfo));
  const float morph = s_swap ? s_shift_shape : shape_knob; // smooth→stepped amount
  const float stepc = s_swap ? shape_knob    : s_shift_shape; // step-size control [0,1]

  // Step control: 0→2 levels (coarse), 0.5→16 (NES), 1→128 (fine)
  const float levels = powf(2.f, 1.f + stepc * 6.f);
  const float step   = 2.f / levels;

  float phase = s_phase;
  q31_t * __restrict y = (q31_t *)yn;
  const q31_t * y_e = y + frames;

  for (; y != y_e; ) {
    const float smooth    = (phase < 0.5f) ? (4.f * phase - 1.f) : (3.f - 4.f * phase);
    const float quantized = clip1f(roundf(smooth / step) * step);
    const float sig       = smooth + morph * (quantized - smooth);
    *(y++) = f32_to_q31(0.9f * sig);

    phase += w0;
    phase -= (uint32_t)phase;
  }

  s_phase = phase;
}

void OSC_NOTEON(const user_osc_param_t * const params)
{
  (void)params;
}

void OSC_NOTEOFF(const user_osc_param_t * const params)
{
  (void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value)
{
  switch (index) {
  case k_user_osc_param_shape:
    s_shape = param_val_to_f32(value); // 0..1
    break;
  case k_user_osc_param_shiftshape:
    s_shift_shape = param_val_to_f32(value); // 0..1
    break;
  case k_user_osc_param_id1:
    s_swap = (value != 0) ? 1 : 0; // 1 = normal, 2 = swapped (display +1)
    break;
  default:
    break;
  }
}
