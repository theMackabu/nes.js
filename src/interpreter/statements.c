void statement(void) {
  static unsigned char t;
  skip_nl();
  t = *p;
  switch (t) {
  case ';':
    ++p;
    return;
  case '{':
    block();
    return;
  case T_LET:
    ++p;
    do_decl();
    end_stmt();
    return;
  case T_ID:
    id_statement();
    return;
  case T_IF:
    do_if();
    return;
  case T_WHILE:
    do_while();
    return;
  case T_FOR:
    do_for();
    return;
  case T_FUNC:
    p += 3;
    skip_group();
    skip_nl();
    skip_group();
    return;
  case T_RETURN:
    ++p;
    t = *p;
    if (t == T_NL || t == ';' || t == '}' || t == T_EOF) {
      vt = V_UNDEF;
      vn = 0;
    } else expr();
    end_stmt();
    ctrl = C_RET;
    return;
  case T_BREAK:
    ++p;
    end_stmt();
    ctrl = C_BREAK;
    return;
  case T_CONT:
    ++p;
    end_stmt();
    ctrl = C_CONT;
    return;
  case T_INC:
  case T_DEC: {
    struct var *v;
    ++p;
    if (*p != T_ID) fail("SYNTAX");
    v = lookup_def(p[1]);
    v->n = numval(v->t, v->n, v->s) + (t == T_INC ? 1 : -1);
    v->t = V_NUM;
    p += 2;
    end_stmt();
    return;
  }
  }
  expr();
  end_stmt();
}

void block(void) {
  stack_check();
  expect('{');
  for (;;) {
    while (*p == T_NL || *p == ';')
      ++p;
    if (*p == '}') {
      ++p;
      return;
    }
    if (*p == T_EOF) fail("MISSING }");
    if (ctrl) skip_statement();
    else statement();
  }
}
