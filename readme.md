# Rootiest's QMK Firmware

Personal fork of [QMK Firmware](https://github.com/qmk/qmk_firmware), built
around my own keyboards and keymaps. Upstream QMK is huge and generic; this
fork exists to carry my custom keymaps, board tweaks, and build tooling on
top of it.

## Keyboards & Keymaps

* **[Keychron Q5 Max](/keyboards/keychron/q5_max/ansi_encoder/keymaps/via/)**
  (ansi_encoder, `via` keymap) — main daily driver. Custom chorded Unicode
  input (Greek letters, math symbols) with a generated
  [cheatsheet](/keyboards/keychron/q5_max/ansi_encoder/keymaps/via/cheatsheet.html),
  autocorrect, tap-dance, a bidirectional Raw HID protocol, and custom RGB
  indicators.
* **[Keychron K17 Max](/keyboards/keychron/k17_max/ansi_encoder/rgb/keymaps/via/)**
  (ansi_encoder/rgb, `via` keymap)

## Building & Flashing

Standard QMK CLI workflow:

```sh
qmk compile -kb keychron/q5_max/ansi_encoder -km via
qmk flash -kb keychron/q5_max/ansi_encoder -km via
```

Merges to `main` also build and publish firmware via CI
(`.github/workflows/build-release.yml`).

## Repo Mirroring

[git.rootiest.dev](https://git.rootiest.dev/rootiest/qmk_firmware) (Gitea) is
the primary repo. GitHub is a mirror, pushed automatically from Gitea.
Push directly to GitHub and the next mirror sync overwrites it. Open
issues/PRs and push branches on Gitea.

GitHub does host the pre-built firmware releases (from CI in
`.github/workflows/build-release.yml`), so grab binaries there.

## Upstream Documentation

This fork tracks [qmk/qmk_firmware](https://github.com/qmk/qmk_firmware).
For general QMK docs (keycodes, features, config options, supported
hardware), see [docs.qmk.fm](https://docs.qmk.fm) — none of that is
duplicated here.
