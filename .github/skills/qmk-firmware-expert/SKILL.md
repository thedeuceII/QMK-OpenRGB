---
name: qmk-firmware-expert
description: 'Use for QMK firmware authoring, editing, review, and debugging in this repository. Covers keyboard definitions, keymaps, custom keycodes, layers, combos, tap dance, encoders, RGB, OpenRGB, OLED, split keyboards, matrix configuration, and build triage. Best when the task needs a repeatable workflow plus repository-specific file touchpoints.'
argument-hint: 'Describe the keyboard, keymap, feature, or firmware issue to work on.'
user-invocable: true
---

# QMK Firmware Expert

Use this skill when working on QMK firmware in this repository and the task needs repository-aware guidance instead of generic keyboard firmware advice.

## When To Use
- Add or modify keymaps, layers, custom keycodes, combos, tap dance, encoder behavior, RGB, OpenRGB, or other firmware features.
- Review or debug a keyboard definition, matrix configuration, board feature flag, or keyboard-level implementation.
- Trace where a behavior belongs before editing, especially when deciding between `keyboard.json` or `info.json`, keyboard-level sources, and keymap-level hooks.
- Validate likely impact by following the repo's actual QMK touchpoints when direct build execution is unavailable.

## Workflow
1. Identify the exact keyboard target, keymap, and feature area before editing.
2. Use the repository map in [repo map](./references/repo-map.md) to find the likely keyboard directory, shared quantum implementation, and supporting templates.
3. Use [task routing](./references/task-routing.md) to decide which files should be inspected or changed for the requested behavior.
4. If the task involves runtime behavior or feature callbacks, consult [feature hooks](./references/feature-hooks.md) before editing keymap or keyboard code.
5. Prefer established patterns already present in this repository over introducing new abstractions.
6. Make the smallest coherent change, then report the affected files, expected firmware behavior, and any assumptions that could only be confirmed with a real build or hardware test.

## Decision Rules
- Change keymap-level behavior in the keymap first unless the behavior is board-wide.
- Change keyboard metadata, matrix layout, and feature flags in the keyboard definition files before editing C code.
- Inspect `quantum/` weak hooks before inventing new entry points.
- Treat lighting, EEPROM, transport, and bootloader details as hardware-coupled; verify the board files before changing shared code.

## Completion Checks
- The chosen file locations match the kind of change being requested.
- The implementation follows patterns already used by similar keyboards or keymaps in this repository.
- The response states what was changed, what was only inspected, and what still depends on build or hardware validation.

## Resources
- [repo map](./references/repo-map.md)
- [task routing](./references/task-routing.md)
- [feature hooks](./references/feature-hooks.md)
