char *tmpbuf(void) {
  poolidx = (poolidx + 1) & (NPOOL - 1);
  return pool[poolidx];
}

unsigned char err_line(void) {
  unsigned int off;
  unsigned char i, l = 0;
  if (tokenizing) return tline + 1;
  off = p - toks;
  for (i = 0; i < nlines; ++i) {
    if (linetok[i] <= off) l = i;
    else break;
  }
  return l + 1;
}

void fail(const char *msg) {
  unsigned char ln = err_line();
  if (lcol) log_newline();
  log_class(2);
  out_str("! ");
  out_str(msg);
  out_str(" @");
  out_num(ln);
  log_newline();
  longjmp(errjmp, 1);
}

void fail_name(const char *msg, unsigned char id) {
  unsigned char l = namelen[id];
  strcpy(errbuf, msg);
  if (l > 14) l = 14;
  memcpy(errbuf + strlen(msg), nameptr[id], l);
  errbuf[strlen(msg) + l] = 0;
  fail(errbuf);
}

void stack_check(void) {
  static unsigned char hw;
  static unsigned int cs;
  __asm__("tsx");
  __asm__("stx %v", hw);
  __asm__("lda c_sp");
  __asm__("sta %v", cs);
  __asm__("lda c_sp+1");
  __asm__("sta %v+1", cs);
  if (hw < 0x20 || cs < (unsigned int)spill_ptr + 0x40) fail("TOO MUCH RECURSION");
}

void poll(void) {
  if (frame_count != lastf) {
    lastf = frame_count;
    read_pad();
    if (pad_new & P_SELECT) fail("BREAK");
    video_service();
  }
}
