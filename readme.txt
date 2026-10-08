NES.JS: A small JavaScript-style interpreter and on-screen code editor for the NES.

The interpreter supports a small subset of JavaScript. Programs are limited
to 64 lines of 47 characters, with additional text and token storage limits.

Build requirements
------------------

  - A recent cc65 development build, with cl65 on PATH.
  - Python 3 with Pillow.
  - Meson 0.57 or newer and Ninja.

cl65 is the cc65 build driver: it runs the C compiler, assembler, and linker
to produce the ROM. Meson manages asset generation and the build steps.

The runtime uses cc65's c_sp symbol. The cc65 2.19 release uses sp instead,
so use the development toolchain rather than that release.

Example dependency setup on macOS with Homebrew, from the repository root:

  brew install --HEAD cc65
  python3 -m venv .venv
  . .venv/bin/activate
  python3 -m pip install meson ninja Pillow

Build with Meson
----------------

Run these commands from the repository root:

  meson setup build
  meson compile -C build

The finished ROM is build/nesjs.nes. To add a demo, create a .js file in 
demos/ and add its path to demo_sources in meson.build. Demo sources
must fit within 64 lines, 47 characters per line, and 1,700 bytes.

Running and controls
--------------------

Open build/nesjs.nes in an NES emulator supporting MMC3 (mapper 4) and
NES 2.0 headers with 32 KB of CHR-RAM. The cartridge has 64 KB of PRG-ROM
and 8 KB of battery-backed PRG-RAM. Enable the emulator's save RAM support
to preserve your program between sessions.

Menu:
  D-pad up/down      Choose a saved program, empty program, or demo.
  A                  Open the selection.
  Start              Open and run the selection.

Editor:
  D-pad              Move around the on-screen keyboard.
  A                  Type the selected key.
  B                  Backspace.
  Select + D-pad     Move the text cursor.
  Select + B         Delete the current line.
  Start              Run the program.
  Select + Start     Return to the menu.
  ABC key            Toggle uppercase letters.

While a program runs, Select interrupts it. On the output screen, B returns
to the editor and Start runs the program again. For input(), up/down changes
the number by 1, left/right changes it by 10, and A or Start submits it.
