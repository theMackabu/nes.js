#define MROW 5
#include "demos.h"

void load_demo(unsigned char i) {
  bc_src = (const unsigned char *)demo_src[i];
  bc_dst = (unsigned char *)prog;
  bc_len = demo_len[i];
  bank_copy(DEMO_BANK);
  prog[demo_len[i]] = 0;
  prog_index();
  cache_ok = 0;
  memcpy(sig, "NJ03", 4);
  modified = 0;
  cx = cy = top = 0;
}

void load_empty(void) {
  prog[0] = 0;
  prog_index();
  cache_ok = 0;
  memcpy(sig, "NJ03", 4);
  modified = 0;
  cx = cy = top = 0;
}

unsigned char has_program(void) {
  return nl > 1 || line_len(0);
}

void menu_desc(unsigned char item, unsigned char base) {
  unsigned char b[30];
  const char *s =
    item < base ? (item == 0 && base == 2 ? "your saved program" : "start from scratch") : demo_desc[item - base];
  unsigned char i = 0;
  memset(b, 128, 30);
  while (*s && i < 30) {
    b[i++] = FONT(*s);
    ++s;
  }
  nt_write(0x2000 + (MROW + NDEMOS + 4) * 32 + 1, b, 30, 0);
}

unsigned char menu(void) {
  unsigned char base = has_program() ? 2 : 1, n, sel = 0, i, run, attr[64], c;
  n = base + NDEMOS;
  build_attr(attr, 0);
  attr[0] = attr[1] = attr[2] = attr[3] = attr[4] = attr[5] = attr[6] = attr[7] = 0x50;
  screen_begin(log_nt);
  c = 128;
  for (i = 0; i < 30; ++i)
    nt_write(0x2000 + i * 32, &c, 32, 0x80);
  nt_write(0x23C0, attr, 64, 0);
  vmode = 0;
  nt_write(0x2000 + 2 * 32 + 3, title_tiles, 6, 0);
  nt_text(12, 2, "pick a program");
  if (base == 2) nt_text(5, MROW, "RESUME SAVED PROGRAM");
  nt_text(5, MROW + base - 1, "NEW EMPTY PROGRAM");
  for (i = 0; i < NDEMOS; ++i) {
    nt_text(5, MROW + base + i, "DEMO:");
    nt_text(11, MROW + base + i, demo_title[i]);
  }
  nt_text(2, 26, "UP/DOWN:CHOOSE  A:OPEN");
  nt_text(2, 27, "START:OPEN AND RUN");
  menu_desc(0, base);
  ppu_on();
  {
    unsigned char shown = 0xFF;
    for (;;) {
      if (sel != shown) {
        if (shown != 0xFF) {
          c = 128;
          nt_write(0x2000 + (MROW + shown) * 32 + 3, &c, 1, 0);
        }
        c = FONT('>');
        nt_write(0x2000 + (MROW + sel) * 32 + 3, &c, 1, 0);
        menu_desc(sel, base);
        shown = sel;
      }
      wait_frame();
      read_pad();
      if ((pad_new & P_UP) && sel) --sel;
      if ((pad_new & P_DOWN) && sel < n - 1) ++sel;
      if (pad_new & (P_A | P_START)) {
        run = (pad_new & P_START) != 0;
        if (base == 2 && sel == 0) return run;
        if (base == 2 && modified) {
          nt_text(2, MROW + NDEMOS + 6, "REPLACE YOUR SAVED PROGRAM?");
          nt_text(2, MROW + NDEMOS + 7, "A:YES  B:NO");
          for (;;) {
            wait_frame();
            read_pad();
            if (pad_new & (P_A | P_B)) break;
          }
          c = 128;
          nt_write(0x2000 + (MROW + NDEMOS + 6) * 32, &c, 32, 0x80);
          nt_write(0x2000 + (MROW + NDEMOS + 7) * 32, &c, 32, 0x80);
          if (pad_new & P_B) continue;
        }
        if (sel < base) {
          load_empty();
          return 0;
        }
        load_demo(sel - base);
        return run;
      }
    }
  }
}
