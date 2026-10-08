unsigned int frames(void) {
  unsigned int a, b;
  do {
    a = frame16;
    b = frame16;
  } while (a != b);
  return a;
}

void call(unsigned char id) {
  if (id >= NB) {
    const unsigned char *fp = funcpos[id - NB];
    if (!fp) fail_name("NOT A FUNCTION ", id);
    call_user(fp);
    return;
  }
  switch (id) {
  case B_LOG:
  case B_PRINT:
    expect('(');
    if (*p != ')') {
      for (;;) {
        expr();
        out_value();
        if (*p != ',') break;
        ++p;
        out_char(' ');
      }
    }
    expect(')');
    log_newline();
    vt = V_UNDEF;
    vn = 0;
    return;
  case B_INPUT:
    do_input();
    return;
  case B_STRING: {
    char nb[8], *t = tmpbuf();
    expect('(');
    expr();
    expect(')');
    strncpy(t, tostr(vt, vn, vs, nb), SLEN);
    t[SLEN] = 0;
    vt = V_STR;
    vs = t;
    return;
  }
  case B_NUMBER:
  case B_PARSEINT:
    expect('(');
    expr();
    expect(')');
    vn = numv();
    vt = V_NUM;
    return;
  case B_PUT: {
    int x, y;
    expect('(');
    expr();
    x = numv();
    expect(',');
    expr();
    y = numv();
    expect(',');
    expr();
    put_value(x, y);
    expect(')');
    vt = V_UNDEF;
    vn = 0;
    return;
  }
  }
  num_args();
  vt = V_NUM;
  switch (id) {
  case B_RAND:
    rng ^= rng << 7;
    rng ^= rng >> 9;
    rng ^= rng << 8;
    vn = av[0] > 0 ? (int)((rng & 0x7FFF) % av[0]) : (int)(rng & 0x7FFF);
    break;
  case B_ABS:
    vn = av[0] < 0 ? -av[0] : av[0];
    break;
  case B_MIN:
    vn = av[0] < av[1] ? av[0] : av[1];
    break;
  case B_MAX:
    vn = av[0] > av[1] ? av[0] : av[1];
    break;
  case B_FLOOR:
  case B_ROUND:
    vn = av[0];
    break;
  case B_SQRT: {
    int r = 0;
    if (av[0] > 0)
      while ((long)(r + 1) * (r + 1) <= av[0])
        ++r;
    vn = r;
    break;
  }
  case B_PAD:
    vn = raw_pad();
    break;
  case B_BEEP:
    beep(av[0]);
    vt = V_UNDEF;
    vn = 0;
    break;
  case B_WAIT:
    while (av[0]-- > 0) {
      wait_frame();
      poll();
    }
    vt = V_UNDEF;
    vn = 0;
    break;
  case B_CLS:
    log_cls();
    vt = V_UNDEF;
    vn = 0;
    break;
  case B_ARRAY:
    vn = arr_new(av[0] < 0 ? 0 : av[0]);
    vt = V_ARR;
    break;
  case B_FRAMES:
    vn = frames() & 0x7FFF;
    break;
  }
}
