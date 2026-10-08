void insert_text(const char *s) {
  unsigned char n = strlen(s), k;
  unsigned int pos;
  if (line_len(cy) + n > LW || prog_end() + n >= PROGN - 1) {
    beep(1000);
    return;
  }
  pos = loff[cy] + cx;
  memmove(prog + pos + n, prog + pos, prog_end() - pos + 1);
  memcpy(prog + pos, s, n);
  for (k = cy + 1; k <= nl; ++k)
    loff[k] += n;
  cx += n;
  line_changed(cy);
  touch();
}

void delete_before(unsigned char n) {
  unsigned char k;
  unsigned int pos = loff[cy] + cx - n;
  memmove(prog + pos, prog + pos + n, prog_end() - pos - n + 1);
  for (k = cy + 1; k <= nl; ++k)
    loff[k] -= n;
  cx -= n;
  line_changed(cy);
  touch();
}

void do_enter(void) {
  unsigned char ind = 0, j, k, len = line_len(cy);
  const char *l = line_ptr(cy);
  unsigned int pos;
  if (nl >= MAXL) {
    beep(1000);
    return;
  }
  while (ind < cx && l[ind] == ' ')
    ++ind;
  j = cx;
  while (j && l[j - 1] == ' ')
    --j;
  if (j && l[j - 1] == '{') ind += 2;
  if (ind + len - cx > LW) ind = 0;
  if (prog_end() + 1 + ind >= PROGN - 1) {
    beep(1000);
    return;
  }
  pos = loff[cy] + cx;
  memmove(prog + pos + 1 + ind, prog + pos, prog_end() - pos + 1);
  prog[pos] = '\n';
  memset(prog + pos + 1, ' ', ind);
  for (k = nl; k > cy; --k)
    loff[k + 1] = loff[k] + 1 + ind;
  loff[cy + 1] = pos + 1;
  ++nl;
  lines_inserted(cy + 1);
  line_changed(cy);
  line_changed(cy + 1);
  ++cy;
  cx = ind;
  touch();
  fix_view();
  map_editor();
  draw_gutter();
}

void delete_line(unsigned char y) {
  unsigned char k;
  unsigned int n;
  touch();
  if (nl == 1) {
    cx = line_len(0);
    cy = 0;
    if (cx) delete_before(cx);
    return;
  }
  n = loff[y + 1] - loff[y];
  memmove(prog + loff[y], prog + loff[y + 1], prog_end() - loff[y + 1] + 1);
  for (k = y; k < nl; ++k)
    loff[k] = loff[k + 1] - n;
  --nl;
  lines_deleted(y);
  map_editor();
  draw_gutter();
}

void do_backspace(void) {
  unsigned char pl;
  if (cx) {
    delete_before(1);
    return;
  }
  if (!cy) return;
  pl = line_len(cy - 1);
  if (pl + line_len(cy) > LW) {
    beep(1000);
    return;
  }
  cx = 0;
  {
    unsigned char k;
    unsigned int pos = loff[cy] - 1;
    memmove(prog + pos, prog + pos + 1, prog_end() - pos);
    for (k = cy; k < nl; ++k)
      loff[k] = loff[k + 1] - 1;
    --nl;
    lines_deleted(cy);
  }
  --cy;
  cx = pl;
  line_changed(cy);
  touch();
  fix_view();
  map_editor();
  draw_gutter();
}
