// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#ifdef ENABLE_RGB_MATRIX_CYCLE_IN_OUT_DUAL
RGB_MATRIX_EFFECT(CYCLE_IN_OUT_DUAL)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static hsv_t CYCLE_IN_OUT_DUAL_math(hsv_t hsv, int16_t dx, int16_t dy, uint8_t time) {
    dx           = (k_rgb_matrix_center.x / 2) - abs8(dx);
    uint8_t dist = sqrt16(dx * dx + dy * dy);
    hsv.h        = time - 2 * dist;
    return hsv;
}

bool CYCLE_IN_OUT_DUAL(effect_params_t* params) {
    return effect_runner_dx_dy(params, &CYCLE_IN_OUT_DUAL_math);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_CYCLE_IN_OUT_DUAL
