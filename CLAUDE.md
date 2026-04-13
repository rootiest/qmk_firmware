# CLAUDE.md - QMK Development (Keychron Q5 Max / K17 Max)

Guidelines and commands for the customized Keychron firmware project based on the `wireless_playground` fork.

## Project Scope

* **Origin:** [git.rootiest.dev/rootiest/qmk_firmware](https://git.rootiest.dev/rootiest/qmk_firmware)
* **Upstream:** [github.com/Keychron/qmk_firmware](https://github.com/Keychron/qmk_firmware) (branch: `wireless_playground`)
* **Primary Keyboard:** Keychron Q5 Max (ANSI Encoder)
* **Secondary Keyboard:** Keychron K17 Max (occasionally)
* **Development Branches:**
  * `dev/q5` — Q5 Max work-in-progress, merges to `main`
  * `dev/k17` — K17 Max work-in-progress, merges to `main`
* **Feature Goals:** Tap-Dance, expanded layers, advanced Chording, Unicode support, and Auto-correct.

## Build and Flash Commands

All commands must be run from the root of the repository **inside the project
Python virtual environment**. The system `qmk` is not used — activate the venv
first:

```bash
source .venv/bin/activate
```

Every `qmk` command below assumes the venv is active (or prefix each with
`source .venv/bin/activate &&`).

### Compilation

```bash
# Build the Q5 Max ANSI Encoder firmware
qmk compile -kb keychron/q5_max/ansi_encoder -km via

# Build the K17 Max firmware (rgb variant; separate 'white' LED variant exists)
qmk compile -kb keychron/k17_max/ansi_encoder/rgb -km via
```

### Flashing

```bash
# Flash the Q5 Max (requires the board to be in bootloader mode)
qmk flash -kb keychron/q5_max/ansi_encoder -km via

# Flash the K17 Max (requires the board to be in bootloader mode)
qmk flash -kb keychron/k17_max/ansi_encoder/rgb -km via
```

### Environment Setup

```bash
# Ensure the submodules are up to date (critical for the wireless_playground branch)
git submodule update --init --recursive

# Set the default keyboard/keymap
qmk setup
qmk config user.keyboard=keychron/q5_max/ansi_encoder
qmk config user.keymap=via
```

## Code Style and Patterns

* **Keymap Structure:** Keep the `keymap.c` organized by layers. Use descriptive defines for layer names (e.g., `_BASE`, `_FN`, `_CHORD`).
* **Feature Modules:** For advanced features like Chording or Tap-Dance, prefer creating separate headers/source files in the keymap folder to keep `keymap.c` readable.
* **Firmware Size:** Monitor the compiled `.bin` size, as wireless features and large feature sets (like Auto-correct) can quickly fill up flash memory.
* **Documentation:** Comment any complex chording logic or non-standard Tap-Dance implementations to ensure maintainability.

## Development Workflow

1. Verify the current branch is `dev/q5` (Q5 Max) or `dev/k17` (K17 Max).
2. Implement features in the relevant keymap directory:
   * Q5 Max: `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/`
   * K17 Max: `keyboards/keychron/k17_max/ansi_encoder/rgb/keymaps/via/`
3. Test compilation locally before committing.
4. Ensure `rules.mk` has the necessary flags enabled (e.g., `TAP_DANCE_ENABLE = yes`, `UNICODE_ENABLE = yes`).

## Git Conventions

* Use conventional commits (`feat:`, `fix:`, `docs:`, `chore:`, etc.) scoped to the keyboard where relevant (e.g. `feat(q5_max):`, `fix(k17_max):`).
* Do **not** include `Co-Authored-By: Claude` trailers in commit messages.

### Chained / Stacked PRs

When merging a chain of PRs (e.g. `A → main`, `B → A`, `C → B`), always **delete the branch after each merge**. Gitea (and GitHub) will automatically retarget any open PRs pointing at the deleted branch to the branch it was merged into. This keeps the chain collapsing cleanly into `main` without manual retargeting or cleanup PRs.

## EEPROM Layout Notes

The Q5 Max uses wear-leveling EEPROM (STM32F401). Key layout facts:

* `EECONFIG_RGB_MATRIX` is at bytes 24–31; byte 0 packs `mode[7:2] | enable[1:0]`.
* Keychron custom RGB data (effect list, regions, per-key colours, retail demo flag) lives in `EECONFIG_KB_DATABLOCK` immediately after `EECONFIG_BASE_SIZE` (37 bytes).
* `VIA_EEPROM_MAGIC_ADDR` is pinned to **544** in `ansi_encoder/config.h`. Do not lower this value — it must stay above `EECONFIG_BASE_SIZE + EECONFIG_KB_DATA_SIZE`. If Keychron EEPROM grows, raise 544 accordingly and clear EEPROM on the board.
* `EECONFIG_KB_DATA_SIZE` is computed in `eeconfig_kb.h` and requires an `#undef` before the `#define` to suppress QMK's default-zero value.

## Keychron RGB (`KEYCHRON_RGB_ENABLE`)

Enabled via `KEYCHRON_RGB_ENABLE = yes` in `rules.mk`. Key behavioural notes:

* `eeconfig_init_custom_rgb()` **loads** Keychron RGB state from EEPROM into RAM. It must be called in `keyboard_post_init_kb()` and in `wireless_enter_connected_kb()`; without it the arrays are zero-initialised and Launcher settings are lost on every boot or transport change.
* `eeconfig_reset_custom_rgb()` **writes** defaults to EEPROM and stamps the version. The version stamp (`eeprom_update_dword(EECONFIG_KEYBOARD, ...)`) belongs here only — not in the load path.
* `kc_rgb_save()` must call `eeconfig_update_rgb_matrix()` to persist the QMK RGB mode alongside the Keychron custom data; otherwise `rgb_matrix_init()` (triggered on every transport change by `REINIT_LED_DRIVER = 1`) reloads the compile-time default `RGB_MATRIX_TYPING_HEATMAP`.
* `retail_demo_enable` is a single byte. A bug in the original Keychron code used `eeprom_read_block` instead of `eeprom_update_block` in `eeconfig_reset_custom_rgb()`, leaving `0xFF` on freshly-flashed boards. `retail_demo_task()` treats any non-zero value as "demo active" and forces `CUSTOM_MIXED_RGB` every scan. The load path now clamps `> 1` to `0` and re-writes the byte as a one-time recovery.
* `default_per_key_led[]` and `default_region[]` must be defined in board-specific code (e.g. `ansi_encoder.c`) — `keychron_rgb.c` declares them `extern`.

## DIP Switch (Win/Mac)

The Win-side dip switch uses a **frame overlay** rather than calling `rgb_matrix_mode()`. A `dip_win_active` flag is set on switch change; `rgb_matrix_indicators_advanced_user()` paints all LEDs white each frame when the flag is set. This avoids writing to EEPROM and preserves the Launcher-configured effect, which would otherwise be overwritten by the direct mode call.
