<!--
 Copyright (C) 2024 Chris Laprade (chris@rootiest.com)
 
 This file is part of rootiest.
 
 rootiest is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 rootiest is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with rootiest.  If not, see <https://www.gnu.org/licenses/>.
-->

# Rootiest QMK Extensions

These QMK extensions are designed to make it easier to use QMK with the Rootiest Solutions keymaps. These keymaps are currently available for the [Keychron Q5 Max](https://www.keychron.com/products/keychron-q5-max-custom-mechanical-keyboard) and the [Planck rev6](https://olkb.com/products/planck-rev-6) keyboards.

The following features are included in the Rootiest QMK Extensions:

- **Unicode/Emoji Support**: Easily type Unicode characters and emojis using a large array of predefined custom keycodes, leader key sequences, and a custom Unicode/Emoji layer.
- **Custom Navigation Keycodes**: Easily navigate text using predefined custom keycodes. Includes keycodes for moving the cursor by word, line, and paragraph, as well as keycodes for selecting text and VIM-style keycodes for moving the cursor.
- **Extended Indicator LED Support**: Easily control the indicator LEDs with macros by configuring the LED matrix positions in the keymap.
- **Quick-text Support**: Easily insert predefined text snippets using custom keycodes or leader key sequences.
- **Auto Correction Support**: Automatically correct common typos using predefined custom keycodes. Additional auto correction rules can be added to the dictionary.
- **CapsWord Support**: Automatically capitalize the first letter of each word using predefined custom keycodes.
- **Auto-apostrophe Support**: Automatically insert an apostrophe when typing contractions.
- **Auto-pairing Support**: Automatically insert closing characters when typing opening characters. Tap-dance keycodes can also be used to insert opening and closing characters.
- **Mouse Keycodes**: Easily control the mouse cursor using predefined custom keycodes. Turbo click is also supported.
- **Digitizer Support**: Control the mouse cursor using absolute coordinates. Equivalent to a digital writing tablet rather than a mouse. Set specific coordinates for the mouse cursor to move to.
- **Mouse Jingling Support**: Automatically move the mouse cursor slightly to prevent the computer from going to sleep.
- **Linux CLI keycodes**: Easily run predefined Linux CLI commands using custom keycodes. Includes commonly-used shortcuts such as `../`, `cd`, `ls`, `sudo`, and `git`.
- **OS-detection Support**: The keyboard will attempt to detect the operating system and adjust the keycodes and layout accordingly. Keycodes are also mapped for manually selecting the operating system.
- **Dynamic Macros**: Easily create and use dynamic macros in realtime using predefined custom keycodes.
- **Extended layer support**: Adds more layers than the default keymaps. Tri-layer functionality is also supported.
- **VIA Support**: The keymaps are compatible with the VIA configurator. VIAL support is also planned.
- **Deferred Execution Support**: Allows code to be executed after a delay or in the background.
- **Secure Lock Support**: Easily lock the keyboard to prevent accidental or unwanted keypresses.
- **Key Lock Support**: Easily lock specific keys to prevent accidental or unwanted keypresses.
- **VSCode Chording Layer**: A layer that prefaces keycodes with a chord to allow for quick access to common VSCode shortcuts.
- **VSCode Unique Keycodes**: A script allows generating keycodes that will be ignored by the OS, but interpreted by VSCode as keyboard shortcuts. This allows for custom keycodes to be used in VSCode without conflicting with the OS via an easy-to-use script.
- **Audio Feedback**: Boards with audio support such as the Planck rev6 can play audio feedback for keypresses, layer changes, and other events.
- **Music Mode**: Boards with audio support can play music using a special music mode layer.
