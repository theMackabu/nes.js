unsigned char editor_next(void) {
  unsigned char i, L, s, d, below, close;
  L = cy;
  if (needs_draw(L)) goto draw;
  for (i = 0; i < VIEW; ++i) {
    L = top + i;
    if (needs_draw(L)) goto draw;
  }
  if (status_dirty) {
    status_prepare();
    return 1;
  }
  for (d = 1; d < MAXL; ++d) {
    below = top + VIEW - 1 + d;
    if (below >= nl && d > top) return 0;
    close = d <= (scroll_up ? PF_ABOVE : PF_BELOW);
    if (!close && !free_slot_exists()) return 0;
    if (!scroll_up || d <= PF_BELOW || free_slot_exists()) {
      L = below;
      if (needs_draw(L)) goto draw;
    }
    if (d <= top && (scroll_up || d <= PF_ABOVE || free_slot_exists())) {
      L = top - d;
      if (needs_draw(L)) goto draw;
    }
  }
  return 0;
draw:
  s = alloc_slot();
  editor_prepare(L);
  s_line[s] = SLOT_BUSY;
  lflag[L] = L_STALE;
  job_slot = s;
  job_line = L;
  return 1;
}
