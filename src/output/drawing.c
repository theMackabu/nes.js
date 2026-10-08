void log_preview(const char *s) {
  unsigned char c = lcol;
  while (*s && c < LOGW)
    lptr[c++] = *s++;
  if (c < LOGW) memset(lptr + c, 0, LOGW - c);
  log_dirty();
}

void log_put(unsigned char x, unsigned char y, const char *s) {
  unsigned char sl = lrs[1 + y], changed = 0;
  char *d = logtxt[sl] + x;
  while (*s && x < LOGW) {
    if (*d != *s) {
      *d = *s;
      changed = 1;
    }
    ++d;
    ++s;
    ++x;
  }
  if (changed) {
    s_dirty[sl] = 1;
    log_any = 1;
    if (sl != log_hint) {
      log_hint2 = log_hint;
      log_hint = sl;
    }
  }
}
