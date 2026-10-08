void skip_tok(void) {
  unsigned char t = *p;
  if (t == T_NUM) p += 3;
  else if (t == T_STR) p += p[1] + 3;
  else if (t == T_ID) p += 2;
  else ++p;
}

void skip_nl(void) {
  while (*p == T_NL)
    ++p;
}

void expect(unsigned char t) {
  static char m[] = "EXPECTED x";
  if (*p != t) {
    m[9] = t < 0x20 || t >= 0x80 ? '?' : t;
    fail(m);
  }
  ++p;
}

void skip_group(void) {
  unsigned char d = 0, t;
  for (;;) {
    t = *p;
    if (t == T_EOF) fail("UNBALANCED BRACKETS");
    if (t == '(' || t == '[' || t == '{') ++d;
    else if (t == ')' || t == ']' || t == '}') {
      if (--d == 0) {
        ++p;
        return;
      }
    }
    skip_tok();
  }
}

#include "prec.h"
#define binprec(t) prec[t]

void skip_unary(void) {
  unsigned char t;
  while (*p == '-' || *p == '!' || *p == '~' || *p == '+')
    ++p;
  t = *p;
  if (t == '(' || t == '[') skip_group();
  else if (t == T_ID) {
    p += 2;
    if (*p == '(') skip_group();
  } else skip_tok();
  for (;;) {
    t = *p;
    if (t == '[') skip_group();
    else if (t == T_LENGTH) ++p;
    else if (t == T_PUSH || t == T_POP) {
      ++p;
      skip_group();
    } else return;
  }
}

void skip_binop(unsigned char minp) {
  unsigned char pr;
  skip_unary();
  for (;;) {
    pr = binprec(*p);
    if (!pr || pr < minp) return;
    ++p;
    skip_binop(pr + 1);
  }
}

void skip_expr(void) {
  skip_binop(1);
  if (*p == '?') {
    ++p;
    skip_expr();
    expect(':');
    skip_expr();
  }
}

void skip_statement(void) {
  unsigned char d = 0, t;
  skip_nl();
  switch (*p) {
  case '{':
    skip_group();
    return;
  case ';':
    ++p;
    return;
  case T_IF:
    ++p;
    skip_group();
    skip_statement();
    {
      const unsigned char *q = p;
      skip_nl();
      if (*p == T_ELSE) {
        ++p;
        skip_statement();
      } else p = q;
    }
    return;
  case T_WHILE:
  case T_FOR:
    ++p;
    skip_group();
    skip_statement();
    return;
  case T_FUNC:
    p += 3;
    skip_group();
    skip_nl();
    skip_group();
    return;
  }
  for (;;) {
    t = *p;
    if (t == T_EOF) return;
    if (!d) {
      if (t == ';' || t == T_NL) {
        ++p;
        return;
      }
      if (t == '}' || t == T_ELSE) return;
    }
    if (t == '(' || t == '[' || t == '{') ++d;
    else if (t == ')' || t == ']' || t == '}') --d;
    skip_tok();
  }
}
