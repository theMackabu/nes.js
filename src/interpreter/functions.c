void call_user(const unsigned char *fp) {
  unsigned char base = lcount, nargs = 0, i = 0, ofb;
  const unsigned char *ret;
  stack_check();
  poll();
  expect('(');
  if (*p != ')') {
    for (;;) {
      expr();
      store(new_local(0xFF));
      ++nargs;
      if (*p != ',') break;
      ++p;
    }
  }
  expect(')');
  ret = p;
  p = fp;
  expect('(');
  while (*p == T_ID) {
    if (i < nargs) lid[base + i] = p[1];
    else new_local(p[1]);
    ++i;
    p += 2;
    if (*p == ',') ++p;
  }
  expect(')');
  skip_nl();
  ofb = fbase;
  fbase = base;
  ++depth;
  hw_spill();
  block();
  hw_unspill();
  if (ctrl == C_RET) {
    if (vt == V_STR) vs = strcpy(tmpbuf(), vs);
  } else {
    vt = V_UNDEF;
    vn = 0;
  }
  ctrl = C_NONE;
  --depth;
  fbase = ofb;
  lcount = base;
  p = ret;
}
