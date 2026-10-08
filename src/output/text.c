void log_dirty(void) {
  s_dirty[lrs[lrow]] = 1;
  log_any = 1;
}

void log_class(unsigned char c) {
  s_cls[lrs[lrow]] = c;
  log_dirty();
}

void log_newline(void) {
  unsigned char s;
  log_dirty();
  lcol = 0;
  if (lrow < LROWS) ++lrow;
  else {
    s = lrs[1];
    memmove(lrs + 1, lrs + 2, LROWS - 1);
    lrs[LROWS] = s;
    slot_clear(s);
    map_log();
  }
  lptr = logtxt[lrs[lrow]];
}

void out_char(char c) {
  if (c == '\n') {
    log_newline();
    return;
  }
  if (lcol >= LOGW) log_newline();
  lptr[lcol++] = c;
  s_dirty[lrs[lrow]] = 1;
  log_any = 1;
}

void out_str(const char *s) {
  while (*s)
    out_char(*s++);
}
