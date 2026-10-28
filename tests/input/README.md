# Command/Control mapping regression test

This test uses the same SDL mapping helper as the streaming keyboard handler.
It needs a C++11 compiler and SDL2 headers, but no display, running stream, or Qt.
From the repository root on Linux or macOS:

```sh
c++ -std=c++11 -Wall -Wextra -Werror -Iapp $(pkg-config --cflags sdl2) \
  tests/input/keyboardmapping.cpp -o /tmp/artemis-keyboardmapping-test
/tmp/artemis-keyboardmapping-test
```

It covers left/right modifier keys on press and release, copy/paste and
Windows/Super shortcut combinations, all 65,536 modifier bit patterns, and all
SDL scancodes. Alt/Option, Shift, lock keys, and ordinary keys must be preserved.

## macOS streaming checks

These require an actual Mac client and a Windows or Linux host:

1. With a fresh preferences file, confirm the swap is off. Enable it in Input
   Settings, restart Artemis, and confirm it remains checked.
2. Set Capture system keyboard shortcuts to always, start a stream, and confirm
   Command+C/V acts as Ctrl+C/V, Control+V acts as Windows/Super+V, and
   Control+Shift+S acts as Windows/Super+Shift+S where the host binds that shortcut.
3. Test both left and right Command/Control, including holding both together.
   Use a host key-event viewer on Linux if the desktop uses different shortcuts.
4. Check Option+Tab cycles host windows and releasing Option selects the window.
   Confirm Artemis's existing Control+Option+Shift shortcuts still work.
5. Hold a mapped modifier while switching focus, toggling fullscreen, or
   ungrabbing input. Return to the stream and check no host modifier stays down.
6. Disable capture, or choose fullscreen-only capture and use a windowed stream.
   Confirm the swap is inactive and local macOS shortcuts retain their behavior.
7. Disable the swap and check the original key mapping returns. On Windows and
   Linux clients, confirm the checkbox is hidden and input behavior is unchanged,
   even if the saved `swapcmdctrl` preference is true.

macOS modifier remaps are applied before Artemis sees input. For example, a
physical Control key globally remapped to Command is received as Command and
then sent as Ctrl with this setting enabled.
