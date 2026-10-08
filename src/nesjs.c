// clang-format off
#include <string.h>
#include <setjmp.h>

#include "common/hardware.h"
#include "common/config.h"
#include "common/memory.c"
#pragma code-name(push, "HCODE")
#pragma codesize(push, 100)

#include "video/ppu.c"
#include "video/jobs.c"
#include "video/service.c"
#include "nes/controller.c"
#include "nes/audio.c"
#include "output/screen.c"
#include "output/text.c"
#include "output/numbers.c"
#include "output/drawing.c"
#include "output/jobs.c"
#include "app/main.c"
#pragma codesize(pop)
#pragma code-name(pop)
#pragma code-name(push, "ICODE")
#pragma rodata-name(push, "IRODATA")

#include "interpreter/tokens.h"
#include "interpreter/state.c"
#include "interpreter/errors.c"
#include "interpreter/token_helpers.c"
#include "interpreter/tokenizer.c"
#include "interpreter/skipping.c"
#include "interpreter/values.c"
#include "interpreter/arrays.c"
#include "interpreter/variables.c"
#include "interpreter/operators.c"
#include "interpreter/primary.c"
#include "interpreter/expressions.c"
#include "interpreter/arguments.c"
#include "interpreter/io.c"
#include "interpreter/functions.c"
#include "interpreter/builtins.c"
#include "interpreter/declarations.c"
#include "interpreter/loop_control.c"
#include "interpreter/assignments.c"
#include "interpreter/loops.c"
#include "interpreter/statements.c"
#include "interpreter/run.c"
#pragma rodata-name(pop)
#pragma code-name(pop)
#pragma code-name(push, "ECODE")
#pragma rodata-name(push, "ERODATA")
#pragma codesize(push, 100)

#include "editor/storage.c"
#include "editor/state.c"
#include "editor/highlight.c"
#include "editor/cache.c"
#include "editor/scheduler.c"
#include "editor/status.c"
#include "editor/keyboard_draw.c"
#include "editor/screen.c"
#include "editor/viewport.c"
#include "editor/editing.c"
#include "editor/keyboard.c"
#include "editor/run.c"
#include "editor/input.c"
#include "app/menu.c"
#include "editor/init.c"
#pragma codesize(pop)
#pragma code-name(pop)
#pragma rodata-name(pop)
// clang-format on
