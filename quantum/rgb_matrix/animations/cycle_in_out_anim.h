// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#ifdef ENABLE_RGB_MATRIX_CYCLE_IN_OUT
RGB_MATRIX_EFFECT(CYCLE_IN_OUT)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static hsv_t CYCLE_IN_OUT_math(hsv_t hsv, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
    hsv.h = time - dist;
    return hsv;
}

bool CYCLE_IN_OUT(effect_params_t* params) {
    return effect_runner_dx_dy_dist(params, &CYCLE_IN_OUT_math);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_CYCLE_IN_OUT
