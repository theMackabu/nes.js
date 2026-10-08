void map_editor(void) {
  unsigned char r, L, s;
  rowbank[1] = SLOTBANK(STATUS_SLOT);
  rowbank[28] = rowbank[29] = HELP_BANK;
  for (r = 0; r < VIEW; ++r) {
    L = top + r;
    s = L < nl ? lslot[L] : SLOT_NONE;
    rowbank[EROW + r] = s < NSLOT ? SLOTBANK(s) : 0;
  }
}

void line_changed(unsigned char y) {
  lflag[y] |= L_STALE | L_CHANGED;
}

void lines_inserted(unsigned char y) {
  unsigned char i;
  memmove(lslot + y + 1, lslot + y, MAXL - 1 - y);
  memmove(lflag + y + 1, lflag + y, MAXL - 1 - y);
  lslot[y] = SLOT_NONE;
  lflag[y] = 0;
  for (i = 0; i < NSLOT; ++i)
    if (s_line[i] < SLOT_BUSY && s_line[i] >= y) ++s_line[i];
  if (job_slot != 0xFF && job_line < MAXL && job_line >= y) ++job_line;
}

void lines_deleted(unsigned char y) {
  unsigned char i;
  if (lslot[y] < NSLOT) s_line[lslot[y]] = SLOT_NONE;
  memmove(lslot + y, lslot + y + 1, MAXL - 1 - y);
  memmove(lflag + y, lflag + y + 1, MAXL - 1 - y);
  lslot[MAXL - 1] = SLOT_NONE;
  lflag[MAXL - 1] = 0;
  for (i = 0; i < NSLOT; ++i)
    if (s_line[i] < SLOT_BUSY && s_line[i] > y) --s_line[i];
  if (job_slot != 0xFF && job_line < MAXL) {
    if (job_line == y) job_line = 0xFF;
    else if (job_line > y) --job_line;
  }
}

unsigned char alloc_slot(void) {
  unsigned char s, L, d, best = 0, bd = 0;
  for (s = 0; s < NSLOT; ++s) {
    L = s_line[s];
    if (L == SLOT_NONE) return s;
    if (L == SLOT_BUSY) continue;
    if (L + PF_ABOVE >= top && L <= top + VIEW - 1 + PF_BELOW) continue;
    d = L < top ? top - L : L - top;
    if (d >= bd) {
      bd = d;
      best = s;
    }
  }
  lslot[s_line[best]] = SLOT_NONE;
  return best;
}

void editor_prepare(unsigned char L) {
  job_len = line_len(L);
  memcpy(jobbuf, line_ptr(L), job_len);
  highlight(jobcls, jobbuf, job_len);
  job_x0 = CODE_X0;
  job_cls = jobcls;
  job_clsk = 1;
  job_base = 0;
}

void editor_finish(void) {
  unsigned char L = job_line, old;
  if (L == STATUS_JOB) return;
  if (L == 0xFF || (lflag[L] & L_CHANGED)) {
    s_line[job_slot] = SLOT_NONE;
    return;
  }
  old = lslot[L];
  if (old < NSLOT) s_line[old] = SLOT_NONE;
  lslot[L] = job_slot;
  s_line[job_slot] = L;
  lflag[L] = 0;
  map_editor();
}

unsigned char needs_draw(unsigned char L) {
  if (L >= nl) return 0;
  if (lslot[L] != SLOT_NONE && !(lflag[L] & L_STALE)) return 0;
  if (!line_len(L)) {
    if (lslot[L] < NSLOT) s_line[lslot[L]] = SLOT_NONE;
    lslot[L] = SLOT_EMPTY;
    lflag[L] = 0;
    map_editor();
    return 0;
  }
  return 1;
}

unsigned char free_slot_exists(void) {
  unsigned char s;
  for (s = 0; s < STATUS_SLOT; ++s)
    if (s_line[s] == SLOT_NONE) return 1;
  return 0;
}
