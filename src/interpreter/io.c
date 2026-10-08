void do_input(void) {
  int val = 0;
  char nb[8];
  unsigned char c0;
  expect('(');
  if (*p != ')') {
    expr();
    out_value();
  }
  expect(')');
  if (lcol > LOGW - 10) log_newline();
  c0 = lcol;
  for (;;) {
    char pv[12];
    pv[0] = '?';
    pv[1] = ' ';
    num_to_str(val, pv + 2);
    strcat(pv, "_");
    log_preview(pv);
    lcol = c0;
    wait_frame();
    video_service();
    read_pad();
    if (pad_new & P_UP) ++val;
    if (pad_new & P_DOWN) --val;
    if (pad_new & P_RIGHT) val += 10;
    if (pad_new & P_LEFT) val -= 10;
    if (pad_new & P_SELECT) fail("BREAK");
    if (pad_new & (P_A | P_START)) break;
  }
  log_preview("");
  out_str("? ");
  out_num(val);
  log_newline();
  lastf = frame_count;
  vt = V_NUM;
  vn = val;
}

void put_value(int x, int y) {
  char nb[8];
  const char *s = tostr(vt, vn, vs, nb);
  if (y < 0 || y >= LROWS || x < 0 || x >= LOGW) return;
  log_put(x, y, s);
}
