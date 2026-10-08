void do_if(void) {
  unsigned char c;
  const unsigned char *q;
  ++p;
  expect('(');
  expr();
  expect(')');
  c = truthy();
  if (c) statement();
  else skip_statement();
  q = p;
  skip_nl();
  if (*p == T_ELSE) {
    ++p;
    if (c) skip_statement();
    else statement();
  } else p = q;
}

void do_while(void) {
  const unsigned char *cond = ++p;
  for (;;) {
    p = cond;
    expect('(');
    expr();
    expect(')');
    if (!truthy()) break;
    statement();
    if (loop_ctrl()) break;
  }
  p = cond;
  skip_group();
  skip_statement();
}

void do_for(void) {
  const unsigned char *cond, *step, *body;
  ++p;
  expect('(');
  if ((p[0] == T_LET && p[1] == T_ID && p[3] == T_OF) || (p[0] == T_ID && p[2] == T_OF)) {
    for_of();
    return;
  }
  if (*p == ';') ++p;
  else statement();
  cond = p;
  if (*p != ';') skip_expr();
  expect(';');
  step = p;
  while (*p != ')') {
    if (*p == T_EOF) fail("SYNTAX");
    if (*p == '(' || *p == '[') skip_group();
    else skip_tok();
  }
  ++p;
  body = p;
  for (;;) {
    p = cond;
    if (*p != ';') {
      expr();
      if (!truthy()) break;
    }
    p = body;
    statement();
    if (loop_ctrl()) break;
    p = step;
    if (*p != ')') statement();
  }
  p = body;
  skip_statement();
}
