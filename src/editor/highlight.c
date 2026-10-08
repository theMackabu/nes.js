#define C_PLAIN 1
#define C_KEY 2
#define C_LIT 3

static const char *const hl_words[] = {
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

unsigned char hl_id0(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$';
}
unsigned char hl_id(char c) {
  return hl_id0(c) || (c >= '0' && c <= '9');
}

static const unsigned char hl_len[] = {3, 3, 5, 2, 4, 5, 3, 8, 6, 5, 8, 4, 5, 4, 9, 2, 3};

unsigned char is_keyword(const char *w, unsigned char len) {
  unsigned char k;
  if (len < 2 || len > 9 || w[0] < 'b' || w[0] > 'w') return 0;
  for (k = 0; k < sizeof(hl_len); ++k)
    if (hl_len[k] == len && hl_words[k][0] == w[0] && !memcmp(hl_words[k], w, len)) return 1;
  return 0;
}

void highlight(unsigned char *b, const char *s, unsigned char l) {
  unsigned char i = 0, j, k;
  char c, q;
  while (i < l) {
    c = s[i];
    if (c == '/' && i + 1 < l && s[i + 1] == '/') {
      memset(b + i, C_LIT, l - i);
      return;
    }
    if (c == '"' || c == '\'') {
      q = c;
      b[i++] = C_LIT;
      while (i < l) {
        c = s[i];
        b[i++] = C_LIT;
        if (c == '\\' && i < l) {
          b[i++] = C_LIT;
          continue;
        }
        if (c == q) break;
      }
      continue;
    }
    if (c >= '0' && c <= '9') {
      while (i < l && hl_id(s[i]))
        b[i++] = C_LIT;
      continue;
    }
    if (hl_id0(c)) {
      for (j = i; j < l && hl_id(s[j]); ++j)
        ;
      k = is_keyword(s + i, j - i) ? C_KEY : C_PLAIN;
      memset(b + i, k, j - i);
      i = j;
      continue;
    }
    b[i++] = C_PLAIN;
  }
}
