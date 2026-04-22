# AGENTS.md — QMK Firmware (Glorious OpenRGB Fork)

This file provides guidance for AI coding agents working in this repository. It is a fork of [QMK Firmware](https://qmk.fm/) maintained by Glorious LLC, targeting the **GMMK V2** and **GMMK V3** keyboard families with OpenRGB support.

---

## Project Overview

- **Active boards:** `keyboards/gmmk/gmmk2/` (p65, p96) and `keyboards/gmmk/gmmk3/` (p65, p75, p100).
- **MCU:** WestBerry WB32F3G71 (ARM Cortex-M4); bootloader: `wb32-dfu`.
- **Key additions over upstream QMK:** OpenRGB protocol support (`OPENRGB_ENABLE`), VIA+OpenRGB hybrid mode (`VIA_OPENRGB_HYBRID`), AW20216S dual-driver RGB matrix tables, SPI Flash wear-leveling EEPROM.
- **Language:** C (firmware), Python (QMK CLI tooling), JSON (keyboard metadata).

---

## Repository Layout

| Path | Role |
|---|---|
| `keyboards/gmmk/` | All board definitions. Start every keyboard or keymap task here. |
| `quantum/` | Shared QMK subsystems and weak hook implementations. |
| `drivers/` | Low-level hardware drivers (LED ICs, SPI, I²C, EEPROM). |
| `platforms/` | MCU/platform shims (ARM ChibiOS, AVR, test stubs). |
| `builddefs/` | Make-layer feature detection and build wiring. |
| `data/` | JSON schemas, templates, and constant tables for QMK tooling. |
| `tests/` | Unit tests for shared quantum code. |

### Key files inside every board directory

```
keyboards/gmmk/<family>/<variant>/<layout>/
  keyboard.json     # processor, bootloader, USB IDs, matrix pins, features — primary metadata
  config.h          # C-level defines keyboard.json cannot express (LED split counts, timing)
  rules.mk          # Make-layer feature flags
  <layout>.c        # RGB LED tables, OpenRGB init, EEPROM/SPI wiring
  keymaps/<name>/keymap.c  # Layer maps, custom keycodes, user hooks
```

---

## Build Commands

```sh
# Compile only
make gmmk/gmmk2/p96/ansi:default

# Compile and flash
make gmmk/gmmk2/p96/ansi:default:flash

# General pattern
make <keyboard_path>:<keymap>
make <keyboard_path>:<keymap>:flash
```

Substitute `gmmk2/p65`, `gmmk3/p75/ansi`, etc. for other targets. Always verify the exact path in the board's `readme.md` before running.

---

## Bootloader Reset (GMMK Boards)

- Hold **Reset pin** (under spacebar) while plugging in USB.
- Hold **Escape** while plugging in USB (also clears EEPROM).
- **Fn + Backslash** from the default QMK keymap if already flashed.

---

## Testing

QMK unit tests live in `tests/`. Run them with:

```sh
make test:all
```

When a direct build is unavailable, validate a change by tracing it through the affected config and C code paths and confirming feature flags are consistent.

---

## Task Routing — Where to Make a Change

| Task type | Correct location |
|---|---|
| Layer maps, macros, key remapping | `keymaps/<name>/keymap.c` |
| Matrix pins, USB IDs, processor, bootloader | `keyboard.json` / `info.json` |
| Board-wide feature enable/disable | `rules.mk` or `features` block in `keyboard.json` |
| RGB LED tables, EEPROM init, board runtime wiring | `<layout>.c` and `config.h` |
| Shared behavior affecting multiple boards | `quantum/` — check for an existing weak hook first |
| Peripheral/transport/MCU-specific bugs | `drivers/` or `platforms/` as a last resort |

**Do not start in `quantum/` for board-specific changes.**

---

## QMK Hook Points

Check these standard weak hooks before adding new control flow:

| Hook | File | When to use |
|---|---|---|
| `process_record_user()` | keymap or keyboard `.c` | Custom keycode handling |
| `layer_state_set_user()` | keymap or keyboard `.c` | Layer transition side effects |
| `encoder_update_user()` | keymap or keyboard `.c` | Encoder behavior |
| `keyboard_post_init_user()` | keymap or keyboard `.c` | Post-init setup |
| `housekeeping_task_user()` | keymap or keyboard `.c` | Periodic work |
| `dip_switch_update_user()` | keymap or keyboard `.c` | DIP switch callbacks |

Place these in the **keymap** when only one keymap needs them; in the **keyboard-level source** when all keymaps on the board must inherit the behavior.

---

## Code Style

- 4-space indent; `snake_case` for variables and functions; `UPPER_CASE` for macros and keycodes.
- `#include QMK_KEYBOARD_H` in keymap files. `#include "quantum.h"` in keyboard-level C files.
- Guard hardware-specific code: `#ifdef RGB_MATRIX_ENABLE`, `#ifdef OPENRGB_ENABLE`, `#ifdef VIA_OPENRGB_HYBRID`.
- Constant LED tables must use `PROGMEM`: `const aw20216s_led_t PROGMEM g_aw20216s_leds[AW20216S_LED_COUNT]`.
- Use `_______` for transparent keys and `XXXXXXX` for blocked/disabled keys.
- License header on every new file: GPL-2.0-or-later, matching the existing files in the board directory.
- Follow [QMK C Coding Conventions](https://docs.qmk.fm/#/coding_conventions_c).

---

## OpenRGB / VIA Notes

- Enable via `OPENRGB_ENABLE = yes` in `rules.mk` or `"openrgb": true` in `keyboard.json`.
- Hybrid VIA+OpenRGB mode is gated by `VIA_OPENRGB_HYBRID` — always check for this guard before modifying RGB matrix behavior.
- The `is_orgb_mode` flag in keyboard-level `ansi.c` (GMMK3) controls which RGB path is active at runtime. Do not remove or bypass it.

---

## What NOT to Do

- Do not edit `quantum/` for behavior specific to a single board or keymap.
- Do not add board-specific keycodes to `quantum/keycodes.h`; define them in the keymap.
- Do not bypass `keyboard.json` with raw C defines when the JSON schema supports the field.
- Do not guess matrix wiring, MCU pins, or LED driver connections — read `keyboard.json` and `<layout>.c` first.
- Do not remove or reorder `g_aw20216s_leds[]` entries without updating the corresponding RGB matrix position map.
- Do not add unnecessary `#include` directives; keymap files need only `QMK_KEYBOARD_H`.
- Do not make style-only or unrelated refactoring edits alongside functional changes.

---

## PR Checklist

- [ ] Change is in the correct file layer: keymap → keyboard → quantum.
- [ ] Feature flags and `#ifdef` guards match the board's `rules.mk` / `keyboard.json`.
- [ ] GPL-2.0-or-later license header is present on every new file.
- [ ] No unrelated refactors or style-only edits included.
- [ ] LED table indices and RGB matrix position tables are consistent after any LED changes.
- [ ] Build target verified with `make <keyboard>:<keymap>` or traced manually.
