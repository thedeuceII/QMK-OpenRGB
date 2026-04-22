# GitHub Copilot Instructions — QMK Firmware (OpenRGB Fork)

This repository is a fork of [QMK Firmware](https://qmk.fm/) maintained by Glorious LLC. The active keyboard families are **GMMK V2** (`keyboards/gmmk/gmmk2/`) and **GMMK V3** (`keyboards/gmmk/gmmk3/`). All AI suggestions must follow QMK conventions and the patterns already established in this tree.

---

## Repository Layout

| Path | Purpose |
|---|---|
| `keyboards/gmmk/` | All board definitions for this fork. Start here for any keyboard or keymap change. |
| `quantum/` | Shared QMK subsystems and weak hook implementations. Edit only when multiple boards are affected or a hook is absent. |
| `drivers/` | Low-level hardware drivers (LED ICs, SPI, I²C, EEPROM). Edit only for peripheral or transport issues. |
| `platforms/` | MCU/platform shims (ARM ChibiOS, AVR, test stubs). |
| `builddefs/` | Make-layer feature detection and build wiring. |
| `data/` | JSON schemas, templates, and constant tables used by QMK tooling. |
| `tests/` | Unit tests for shared quantum code. |

---

## Key Files Per Board

For every `keyboards/gmmk/<family>/<variant>/<layout>/` directory:

- **`keyboard.json`** — processor, bootloader, USB IDs, matrix pins, diode direction, EEPROM driver, feature flags, and layout coordinates. This is the primary metadata source; prefer it over `config.h` for anything it can express.
- **`config.h`** — C-level defines that `keyboard.json` cannot express (e.g., LED driver split counts, custom timing constants).
- **`rules.mk`** — Feature enable/disable flags for the Make build system. Used when `keyboard.json` features are insufficient or for legacy boards.
- **`<layout>.c`** — Keyboard-level C implementation: RGB matrix LED tables (`g_aw20216s_leds[]`), OpenRGB conditionals, EEPROM/SPI init, DIP switch callbacks.
- **`keymaps/<name>/keymap.c`** — Keymap-level behavior: layer definitions, `process_record_user()`, `layer_state_set_user()`, `encoder_update_user()`.

### Concrete examples in this repo

- `keyboards/gmmk/gmmk2/p96/ansi/keyboard.json` — WB32F3G71 processor, `wb32-dfu` bootloader, `wear_leveling`/`spi_flash` EEPROM, OpenRGB enabled.
- `keyboards/gmmk/gmmk2/p96/ansi/keymaps/default/keymap.c` — Two-layer keymap (`_BL`, `_FL`) with `QK_GESC`, `QK_BOOT`, and `MO()`.
- `keyboards/gmmk/gmmk3/p75/ansi/ansi.c` — AW20216S dual-driver RGB LED table, `VIA_OPENRGB_HYBRID` conditional, keyboard-level init.
- `keyboards/gmmk/gmmk3/p75/ansi/rules.mk` — `ENCODER_ENABLE`, `DIP_SWITCH_ENABLE`, `RGB_MATRIX_ENABLE`, `OPENRGB_ENABLE`.

---

## Decision Rules

### Where to make a change

1. **Keymap behavior** (layers, macros, key remapping) → `keymaps/<name>/keymap.c` only.
2. **Physical layout, matrix, USB IDs, processor, bootloader** → `keyboard.json` / `info.json`.
3. **Board-wide feature flags** → `rules.mk` or the `features` block in `keyboard.json`.
4. **RGB LED tables, EEPROM init, board-specific runtime wiring** → keyboard-level `<layout>.c` / `config.h`.
5. **Shared hook behavior affecting multiple boards** → `quantum/` — check for existing weak hooks first.
6. **Peripheral or transport issues** → `drivers/` or `platforms/` as a last resort.

### Hook priority

Before adding new control flow, verify whether a standard QMK weak hook already covers the need:

- Custom keycodes: `process_record_user(uint16_t keycode, keyrecord_t *record)`
- Layer transitions: `layer_state_set_user(layer_state_t state)`
- Encoder turns: `encoder_update_user(uint8_t index, bool clockwise)`
- Post-init setup: `keyboard_post_init_user(void)`
- Periodic work: `housekeeping_task_user(void)`
- DIP switches: `dip_switch_update_user(uint8_t index, bool active)`

Put these hooks in the **keymap** when only one keymap needs them. Put them in keyboard-level sources when every keymap on the board must inherit the behavior.

---

## Coding Conventions

- Follow [QMK C Coding Conventions](https://docs.qmk.fm/#/coding_conventions_c): 4-space indent, `snake_case` for variables and functions, `UPPER_CASE` for macros and keycodes.
- `#include QMK_KEYBOARD_H` in keymap files; `#include "quantum.h"` in keyboard-level C files.
- Guard hardware-specific code with feature macros: `#ifdef RGB_MATRIX_ENABLE`, `#ifdef OPENRGB_ENABLE`, `#ifdef VIA_OPENRGB_HYBRID`.
- Use `PROGMEM` for constant LED tables: `const aw20216s_led_t PROGMEM g_aw20216s_leds[AW20216S_LED_COUNT]`.
- Use `_______` for transparent keys and `XXXXXXX` for blocked keys in keymap matrices.
- License header: GPL-2.0-or-later, matching the existing files in the board directory.

---

## Build Targets

```
# Compile only
make gmmk/gmmk2/p96/ansi:default

# Compile and flash
make gmmk/gmmk2/p96/ansi:default:flash

# General pattern
make <keyboard_path>:<keymap>
make <keyboard_path>:<keymap>:flash
```

Bootloader reset methods for GMMK boards:
- Hold **Reset** pin while plugging in USB.
- Hold **Escape** while plugging in USB (also clears EEPROM).
- `Fn + Backslash` from the default QMK keymap (if already flashed).

---

## OpenRGB / VIA Notes

- OpenRGB support is enabled via `OPENRGB_ENABLE = yes` in `rules.mk` or `"openrgb": true` in `keyboard.json`.
- The hybrid VIA+OpenRGB mode is gated by `VIA_OPENRGB_HYBRID` — check for this define before modifying RGB matrix behavior in keyboard-level sources.
- The global `is_orgb_mode` flag (defined in `ansi.c` on GMMK3 boards) controls which RGB path is active at runtime.

---

## What NOT to Do

- Do not edit `quantum/` for behavior that is specific to a single board or keymap.
- Do not add new keycodes to `quantum/keycodes.h` for board-specific functions; define them in the keymap instead.
- Do not bypass `keyboard.json` metadata in favor of raw C defines when the JSON schema supports the field.
- Do not guess matrix wiring, MCU pins, or LED driver connections — read `keyboard.json` and `<layout>.c` first.
- Do not remove or reorder entries in `g_aw20216s_leds[]` without updating the corresponding RGB matrix position map.
- Do not add unnecessary `#include` directives; keymap files only need `QMK_KEYBOARD_H`.

---

## Pull Request Checklist

- [ ] Change targets the correct file layer (keymap → keyboard → quantum).
- [ ] Feature flags and `#ifdef` guards match the board's `rules.mk` / `keyboard.json`.
- [ ] License header is present and correct (GPL-2.0-or-later).
- [ ] No unrelated refactors or style-only edits included.
- [ ] LED table indices and RGB matrix position tables are consistent after any LED changes.
- [ ] Build target confirmed with `make <keyboard>:<keymap>` (or traced manually when a build is unavailable).
