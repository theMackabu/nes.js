void prescan(void) {
  memset(funcpos, 0, sizeof(funcpos));
  p = toks;
  while (*p != T_EOF) {
    if (*p == T_FUNC) {
      ++p;
      if (*p != T_ID || p[1] < NB) fail("BAD FUNCTION NAME");
      funcpos[p[1] - NB] = p + 2;
    }
    skip_tok();
  }
}

void run_program(void) {
  unsigned char i;
  log_screen();
  nlines = nl;
  for (i = 0; i < MAXG; ++i) {
    globals[i].t = V_NONE;
    gptr[NB + i] = &globals[i];
  }
  lcount = fbase = depth = ssp = narr = 0;
  heaptop = 0;
  ctrl = C_NONE;
  rng ^= frame16 | 1;
  lastf = frame_count;
  p = toks;
  if (setjmp(errjmp) == 0) {
    __asm__("tsx");
    __asm__("stx %v", hw_base);
    spill_ptr = spill_area;
    tokenize();
    prescan();
    p = toks;
    for (;;) {
      while (*p == T_NL || *p == ';')
        ++p;
      if (*p == T_EOF || ctrl) break;
      if (*p == '}') fail("UNEXPECTED }");
      statement();
      poll();
    }
    if (lcol) log_newline();
  }
  log_class(3);
  out_str("-- B:EDIT  START:RUN --");
  log_newline();
}
