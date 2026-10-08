void statement(void);

void end_stmt(void) {
  unsigned char t = *p;
  if (t == ';' || t == T_NL) ++p;
  else if (t != '}' && t != ')' && t != T_EOF && t != T_ELSE) fail("SYNTAX");
}

struct var *decl_var(unsigned char id) {
  unsigned char i;
  if (id < NB) fail_name("RESERVED NAME ", id);
  if (!depth) return gptr[id];
  for (i = fbase; i < lcount; ++i)
    if (lid[i] == id) return &lvar[i];
  return new_local(id);
}

void do_decl(void) {
  struct var *v;
  for (;;) {
    if (*p != T_ID) fail("EXPECTED NAME");
    v = decl_var(p[1]);
    p += 2;
    if (*p == '=') {
      ++p;
      expr();
      store(v);
    } else {
      v->t = V_UNDEF;
      v->n = 0;
    }
    if (*p != ',') break;
    ++p;
  }
}
