# Feature Hooks

This repository exposes standard QMK hook points in `quantum/`. Check these before adding custom control flow.

## Common Hook Entry Points
- `quantum/quantum.c`: `process_record_user(uint16_t keycode, keyrecord_t *record)` for custom keycode handling.
- `quantum/action_layer.c`: `layer_state_set_user(layer_state_t state)` for layer transition side effects.
- `quantum/matrix_common.c`: `matrix_init_user(void)` for keymap or board initialization.
- `quantum/encoder.c`: `encoder_update_user(uint8_t index, bool clockwise)` for encoder behavior.
- `quantum/keyboard.c`: `keyboard_post_init_user(void)` and `housekeeping_task_user(void)` for post-init and periodic work.

## How To Choose The Hook Level
- Put per-keymap behavior in the keymap implementation when only one keymap needs it.
- Put board-wide behavior in keyboard-level source files when every keymap on that board should inherit it.
- Touch shared `quantum/` code only when the repository lacks the required hook or the fix must apply across boards.

## Repo Examples To Imitate
- `keyboards/gmmk/gmmk2/p96/ansi/keymaps/default/keymap.c`: shows a straightforward layered keymap with board reset and RGB matrix controls placed in the keymap.
- `keyboards/gmmk/gmmk3/p75/ansi/ansi.c`: shows keyboard-level feature wiring for RGB matrix data, OpenRGB-related conditionals, and EEPROM initialization.
- `keyboards/gmmk/gmmk3/*/keymaps/viahybrid/keymap.c`: representative locations for `process_record_user()` in this repository's keyboard family.

## Completion Heuristic
- If the requested behavior can be expressed by an existing hook, prefer that hook.
- If the requested behavior is already implemented in a sibling keyboard or keymap, copy the pattern and adapt it instead of inventing a new structure.
- If direct build or hardware validation is unavailable, state the expected execution path through the chosen hook in the final report.
