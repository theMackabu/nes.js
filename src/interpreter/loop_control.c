unsigned char loop_ctrl(void) {
  poll();
  if (!ctrl) return 0;
  if (ctrl == C_CONT) {
    ctrl = C_NONE;
    return 0;
  }
  if (ctrl == C_BREAK) ctrl = C_NONE;
  return 1;
}

void for_of(void) {
  struct var *v;
  const unsigned char *body;
  unsigned char h, i = 0;
  if (*p == T_LET) ++p;
  v = decl_var(p[1]);
  p += 3;
  expr();
  expect(')');
  body = p;
  if (vt != V_ARR) fail("CAN ONLY LOOP ARRAYS");
  h = vn;
  while (i < arrs[h].len) {
    v->t = V_NUM;
    v->n = heap[arrs[h].start + i++];
    p = body;
    statement();
    if (loop_ctrl()) break;
  }
  p = body;
  skip_statement();
}
