# Repository Map

This reference summarizes the most relevant QMK locations in this repository for firmware work.

## Core Areas
- `keyboards/`: board definitions and keymaps. In this repository, `keyboards/gmmk/gmmk2/` and `keyboards/gmmk/gmmk3/` are the active product families.
- `quantum/`: shared QMK behavior, weak hook implementations, and common subsystems such as layers, encoders, RGB, OpenRGB, VIA, and split support.
- `drivers/` and `platforms/`: lower-level hardware drivers and MCU/platform integrations. Inspect only when the issue is below keyboard-level firmware.
- `data/templates/keyboard/`: useful reference for the expected keyboard readme structure and bootloader guidance.

## Keyboard-Level Anchors In This Repo
- `keyboards/gmmk/gmmk2/p96/ansi/readme.md`: example build and flash target naming for a concrete keyboard.
- `keyboards/gmmk/gmmk2/p96/ansi/keyboard.json`: keyboard metadata, processor, bootloader, matrix pins, features, and layout coordinates.
- `keyboards/gmmk/gmmk2/p96/ansi/keymaps/default/keymap.c`: default keymap structure, layer enum usage, and a simple function-layer pattern.
- `keyboards/gmmk/gmmk3/p75/ansi/ansi.c`: keyboard-level implementation with RGB matrix tables, OpenRGB-related conditionals, and EEPROM/SPI initialization.

## Practical Reading Order
1. Start in the target keyboard directory under `keyboards/`.
2. Read `keyboard.json` or `info.json` plus any `readme.md`, `config.h`, and `rules.mk` files in that keyboard path.
3. Inspect the target keymap under `keymaps/<name>/` if the change is user-facing behavior.
4. Move to keyboard-level C files when the behavior is board-wide or hardware-specific.
5. Read the matching shared implementation under `quantum/` if the hook or subsystem behavior is unclear.

## Escalation Boundary
- Stay in keyboard or keymap files unless the requested change clearly belongs in shared QMK code.
- Move into `quantum/` only when multiple keyboards depend on the behavior or the relevant hook is defined there.
- Move into `drivers/` or `platforms/` only for transport, peripheral, or MCU-specific issues that are not explainable from the keyboard directory.
