void unary(void) {
  static unsigned char t;
  t = *p;
  if (t == '-') {
    ++p;
    unary();
    vn = -numv();
    vt = V_NUM;
    return;
  }
  if (t == '!') {
    ++p;
    unary();
    vn = !truthy();
    vt = V_BOOL;
    return;
  }
  if (t == '~') {
    ++p;
    unary();
    vn = ~numv();
    vt = V_NUM;
    return;
  }
  if (t == '+') {
    ++p;
    unary();
    vn = numv();
    vt = V_NUM;
    return;
  }
  primary();
}

unsigned char quick_operand(void);

void binop(unsigned char minp) {
  static unsigned char pr, nt;
  unsigned char op, lt, mark0;
  int ln;
  const char *ls;
  if (!quick_operand()) unary();
  for (;;) {
    op = *p;
    pr = binprec(op);
    if (!pr || pr < minp) return;
    ++p;
    if (op == T_LAND || op == T_LOR) {
      if (truthy() == (op == T_LOR)) skip_binop(pr + 1);
      else binop(pr + 1);
      continue;
    }
    lt = vt;
    ln = vn;
    ls = vs;
    mark0 = ssp;
    if (lt == V_STR) ls = spush(vs);
    nt = *p == T_NUM ? p[3] : p[2];
    if (prec[nt] > pr || !quick_operand()) binop(pr + 1);
    a_lt = lt;
    a_ln = ln;
    a_ls = ls;
    APPLY(op);
    ssp = mark0;
  }
}

void expr(void) {
  binop(1);
  if (*p == '?') {
    ++p;
    if (truthy()) {
      expr();
      expect(':');
      skip_expr();
    } else {
      skip_expr();
      expect(':');
      expr();
    }
  }
}
