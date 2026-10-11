/*
 * triangle.cpp
 *
 * Triangle oscillator for the minilogue xd (logue SDK v1).
 *
 * SHAPE:       smooth triangle (0) → NES 4-bit stepped (max); LFO-modulatable
 * SHIFT+SHAPE: step size — left = coarse (2 steps), center = NES (16), right = fine (128)
 */

#include "userosc.h"

static float s_phase       = 0.f;
static float s_shape       = 0.f;   // SHAPE knob position [0,1]
static float s_shift_shape = 0.5f;  // default: NES (16 steps)

void OSC_INIT(uint32_t platform, uint32_t api)
{
  (void)platform;
  (void)api;
  s_phase       = 0.f;
  s_shape       = 0.f;
  s_shift_shape = 0.5f;
}

void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{
  const float w0    = osc_w0f_for_note((params->pitch) >> 8, params->pitch & 0xFF);
  // Knob position + LFO delta, clamped to [0,1]
  const float shape = clip01f(s_shape + q31_to_f32(params->shape_lfo));

  // SHIFT+SHAPE: 0→2 steps (coarse), 0.5→16 (NES), 1→128 (fine)
  const float levels = powf(2.f, 1.f + s_shift_shape * 6.f);
  const float step   = 2.f / levels;

  float phase = s_phase;
  q31_t * __restrict y = (q31_t *)yn;
  const q31_t * y_e = y + frames;

  for (; y != y_e; ) {
    const float smooth    = (phase < 0.5f) ? (4.f * phase - 1.f) : (3.f - 4.f * phase);
    const float quantized = clip1f(roundf(smooth / step) * step);
    const float sig       = smooth + shape * (quantized - smooth);
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
  default:
    break;
  }
}
