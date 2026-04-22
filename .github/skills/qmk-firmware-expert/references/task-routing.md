# Task Routing

Use this reference to decide where a QMK task should land before editing.

## If The Task Is A Keymap Behavior Change
- Inspect `keyboards/<board>/.../keymaps/<keymap>/keymap.c` first.
- Put layer definitions, custom keycodes, and most `process_record_user()` logic in the keymap unless the behavior must apply to every keymap for that board.
- Reuse existing layer and macro patterns from nearby keymaps when possible.

## If The Task Is Keyboard Metadata Or Physical Layout
- Inspect `keyboard.json` and `info.json` first.
- Use these files for matrix pins, processor, bootloader, features, layout definitions, and layout geometry.
- Only drop into C when metadata cannot express the needed behavior.

## If The Task Is A Board-Wide Feature Or Hardware Behavior
- Inspect keyboard-level `config.h`, `rules.mk`, and `*.c` files in the target keyboard directory.
- Use keyboard-level sources for RGB matrix tables, EEPROM setup, transport setup, and board-specific runtime behavior.
- Confirm that feature guards such as `RGB_MATRIX_ENABLE` or board-specific defines already match the requested change.

## If The Task Involves Shared QMK Behavior
- Inspect the corresponding subsystem in `quantum/`.
- Typical destinations include `quantum/quantum.c`, `quantum/action_layer.c`, `quantum/encoder.c`, `quantum/keyboard.c`, `quantum/openrgb.c`, and related subsystem folders.
- Check whether the needed entry point is already exposed as a weak hook before adding new wiring.

## If The Task Is Build Or Flash Triage
- Read the keyboard `readme.md` for target names and bootloader instructions.
- Verify the keyboard path naming pattern used by `make <keyboard>:<keymap>`.
- Confirm bootloader, processor, and feature definitions in keyboard metadata before assuming the build target is wrong.

## Sanity Checks
- A board-specific change should not start in `quantum/`.
- A matrix or layout change should not start in the keymap.
- A pure keymap customization usually should not require edits to platform drivers.
