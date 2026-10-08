static unsigned char *tw;

unsigned char isid0(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$';
}
unsigned char isdig(char c) {
  return c >= '0' && c <= '9';
}
unsigned char isid(char c) {
  return isid0(c) || isdig(c);
}

void emit(unsigned char b) {
  if (tw >= toks + TOKN - 4) fail("PROGRAM TOO BIG");
  *tw++ = b;
}

static const char *const kwnames[] = {
  "let",
  "var",
  "const",
  "if",
  "else",
  "while",
  "for",
  "function",
  "return",
  "break",
  "continue",
  "true",
  "false",
  "null",
  "undefined",
  "of",
  "new"
};
static const unsigned char kwtok[] = {
  T_LET,
  T_LET,
  T_LET,
  T_IF,
  T_ELSE,
  T_WHILE,
  T_FOR,
  T_FUNC,
  T_RETURN,
  T_BREAK,
  T_CONT,
  T_TRUE,
  T_FALSE,
  T_NULL,
  T_NULL,
  T_OF,
  0
};

unsigned char intern(const char *s, unsigned char len) {
  unsigned char i;
  for (i = 0; i < nnames; ++i)
    if (namelen[i] == len && !memcmp(nameptr[i], s, len)) return i;
  if (nnames >= NB + MAXG) fail("TOO MANY NAMES");
  nameptr[nnames] = s;
  namelen[nnames] = len;
  return nnames++;
}

unsigned char method_tok(const char *s, unsigned char len) {
  if (len == 6 && !memcmp(s, "length", 6)) return T_LENGTH;
  if (len == 4 && !memcmp(s, "push", 4)) return T_PUSH;
  if (len == 3 && !memcmp(s, "pop", 3)) return T_POP;
  return 0;
}

unsigned char continues(unsigned char t) {
  if (t >= T_EQ && t <= T_SHR) return t != T_INC && t != T_DEC;
  switch (t) {
  case '{':
  case '(':
  case '[':
  case ',':
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '<':
  case '>':
  case '=':
  case '!':
  case '&':
  case '|':
  case '^':
  case '?':
  case ':':
  case '~':
  case T_NL:
    return 1;
  }
  return 0;
}
