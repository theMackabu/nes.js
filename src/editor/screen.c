void build_attr(unsigned char *a, unsigned char editor) {
  unsigned char x, y, pal;
  memset(a, 0, 64);
  for (y = 0; y < 15; ++y) {
    for (x = 0; x < 16; ++x) {
      pal = 0;
      if (y == 0) pal = 1;
      else if (editor && y <= 9 && x == 0) pal = 1;
      else if (editor && y >= 10 && y <= 13) pal = 2;
      else if (y >= 13) pal = 3;
      a[(y >> 1) * 8 + (x >> 1)] |= pal << (((y & 1) << 2) | ((x & 1) << 1));
    }
  }
}

void editor_reset_cache(void) {
  unsigned char i;
  memset(lslot, SLOT_NONE, MAXL);
  memset(lflag, 0, MAXL);
  for (i = 0; i < NSLOT; ++i)
    s_line[i] = SLOT_NONE;
  s_line[STATUS_SLOT] = SLOT_BUSY;
  job_slot = 0xFF;
}

static unsigned char cache_ok;

void enter_editor(void) {
  screen_begin(editor_nt);
  if (shift) draw_keyboard_letters();
  draw_gutter();
  draw_status();
  vmode = 1;
  if (!cache_ok) {
    unsigned char r, L;
    editor_reset_cache();
    cache_ok = 1;
    for (r = 0; r < VIEW; ++r) {
      L = top + r;
      if (L >= nl) break;
      if (!line_len(L)) {
        lslot[L] = SLOT_EMPTY;
        continue;
      }
      editor_prepare(L);
      job_slot = r;
      job_now();
      lslot[L] = r;
      s_line[r] = L;
    }
  } else {
    unsigned char s;
    for (s = 0; s < STATUS_SLOT; ++s)
      if (s_line[s] == SLOT_BUSY) s_line[s] = SLOT_NONE;
  }
  status_prepare();
  job_now();
  job_slot = 0xFF;
  map_editor();
  ppu_on();
}
