project('nesjs', meson_version: '>=0.57.0')

cl65 = find_program('cl65', native: true)
python = import('python').find_installation('python3', modules: ['PIL'])

font = custom_target(
  'font',
  input: files(
    'xtask/mkfont.py',
    'xtask/fontlib.py',
    'assets/fonts/font8x8_basic.h',
    'assets/fonts/koi5x8.pcf',
  ),
  output: ['font.chr', 'font5.inc'],
  command: [python, '@INPUT0@', '@OUTDIR@'],
  env: ['PYTHONDONTWRITEBYTECODE=1'],
)

demo_sources = files(
  'demos/01_hello.js',
  'demos/02_fizzbuzz.js',
  'demos/03_fibonacci.js',
  'demos/04_guess.js',
  'demos/05_primes.js',
  'demos/06_sort.js',
  'demos/07_life.js',
  'demos/08_walker.js',
  'demos/09_music.js',
  'demos/10_collatz.js',
)

demos = custom_target(
  'demos',
  input: files('xtask/mkdemos.py') + demo_sources,
  output: 'demos.h',
  command: [python, '@INPUT0@', '@OUTPUT@', demo_sources],
)

screens = custom_target(
  'screens',
  input: 'xtask/mkscreens.py',
  output: ['screens_edit.h', 'screens_log.h'],
  command: [python, '@INPUT@', '@OUTDIR@'],
)

main = configure_file(input: 'src/nesjs.c', output: 'main.c', copy: true)

crt0 = configure_file(input: 'src/nes/crt0.s', output: 'crt0.s', copy: true)
runtime_object = custom_target(
  'runtime-object',
  input: crt0,
  output: 'crt0.o',
  command: [
    cl65,
    '-t',
    'none',
    '-c',
    '--asm-include-dir',
    meson.current_build_dir(),
    '--bin-include-dir',
    meson.current_build_dir(),
    '@INPUT@',
    '-o',
    '@OUTPUT@',
  ],
  depends: font,
)

main_object = custom_target(
  'main-object',
  input: main,
  output: 'main.o',
  depfile: 'main.d',
  command: [
    cl65,
    '-t',
    'none',
    '-Oirs',
    '-c',
    '--create-dep',
    '@DEPFILE@',
    '-I',
    meson.current_build_dir(),
    '-I',
    meson.current_source_dir() / 'src',
    '@INPUT@',
    '-o',
    '@OUTPUT@',
  ],
  depends: [demos, screens],
)

custom_target(
  'nesjs',
  input: [runtime_object, main_object],
  output: 'nesjs.nes',
  command: [
    cl65,
    '-t',
    'none',
    '-C',
    files('src/nes/nes.cfg'),
    '@INPUT@',
    '-o',
    '@OUTPUT@',
  ],
  build_by_default: true,
)
