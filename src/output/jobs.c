void log_finish(void) {
  if (job_gen == s_gen[job_slot]) {
    s_valid[job_slot] = 1;
    map_log();
  }
}

unsigned char log_take(unsigned char s) {
  unsigned char n;
  if (s >= NSLOT || !s_dirty[s]) return 0;
  s_dirty[s] = 0;
  for (n = LOGW; n && !logtxt[s][n - 1]; --n)
    ;
  memcpy(jobbuf, logtxt[s], n);
  job_len = n;
  job_x0 = 3;
  job_cls = 0;
  job_clsk = s_cls[s];
  job_base = 2;
  job_slot = s;
  job_gen = s_gen[s];
  return 1;
}

unsigned char log_next(void) {
  static unsigned char turn, pos = 1;
  unsigned char k;
  if (!log_any) return 0;
  turn ^= 1;
  if (turn && (log_take(log_hint) || log_take(log_hint2))) return 1;
  for (k = 0; k < LROWS; ++k) {
    if (log_take(lrs[pos])) {
      if (++pos > LROWS) pos = 1;
      return 1;
    }
    if (++pos > LROWS) pos = 1;
  }
  log_any = 0;
  return 0;
}
