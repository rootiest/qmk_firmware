# CLAUDE.md - QMK Development (Keychron Q5 Max)

Guidelines and commands for the customized Keychron Q5 Max firmware project based on the `wireless_playground` fork.

## Project Scope

* **Origin:** [git.rootiest.dev/rootiest/qmk_firmware](https://git.rootiest.dev/rootiest/qmk_firmware)
* **Upstream:** [github.com/Keychron/qmk_firmware](https://github.com/Keychron/qmk_firmware) (branch: `wireless_playground`)
* **Primary Keyboard:** Keychron Q5 Max (ANSI Encoder)
* **Development Workflow:** Work is performed on the `q5_dev` branch before merging to `main`.
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
```

### Flashing

```bash
# Flash the Q5 Max (requires the board to be in bootloader mode)
qmk flash -kb keychron/q5_max/ansi_encoder -km via
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

1. Verify the current branch is `q5_dev`.
2. Implement features in `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/`.
3. Test compilation locally before committing.
4. Ensure `rules.mk` has the necessary flags enabled (e.g., `TAP_DANCE_ENABLE = yes`, `UNICODE_ENABLE = yes`).
