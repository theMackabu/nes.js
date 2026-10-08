void tokenize(void) {
  const char *s, *st;
  unsigned char l, last, incomment = 0, len, k, c, c1, t;
  unsigned int num;
  tokenizing = 1;
  tw = toks;
  nnames = NB;
  for (l = 0; l < NB; ++l) {
    nameptr[l] = bnames[l];
    namelen[l] = strlen(bnames[l]);
  }
  for (l = 0; l < nlines; ++l) {
    tline = l;
    linetok[l] = tw - toks;
    s = prog + loff[l];
    last = 0;
    while (*s && *s != '\n') {
      c = *s;
      c1 = s[1];
      if (incomment) {
        if (c == '*' && c1 == '/') {
          incomment = 0;
          s += 2;
        } else ++s;
        continue;
      }
      if (c == ' ') {
        ++s;
        continue;
      }
      if (c == '/' && c1 == '/') break;
      if (c == '/' && c1 == '*') {
        incomment = 1;
        s += 2;
        continue;
      }
      if (isdig(c)) {
        num = 0;
        while (isdig(*s))
          num = num * 10 + (*s++ - '0');
        emit(T_NUM);
        emit(num & 0xFF);
        emit(num >> 8);
        last = T_NUM;
        continue;
      }
      if (c == '"' || c == '\'') {
        unsigned char *lenp;
        ++s;
        emit(T_STR);
        lenp = tw;
        emit(0);
        len = 0;
        while (*s && *s != '\n' && *s != c) {
          char ch = *s++;
          if (ch == '\\' && *s) {
            ch = *s++;
            if (ch == 'n') ch = '\n';
          }
          emit(ch);
          ++len;
        }
        if (*s != c) fail("UNCLOSED STRING");
        ++s;
        emit(0);
        *lenp = len;
        last = T_STR;
        continue;
      }
      if (isid0(c)) {
        st = s;
        while (isid(*s))
          ++s;
        while (*s == '.' && isid0(s[1])) {
          const char *e = s + 1;
          while (isid(*e))
            ++e;
          if (method_tok(s + 1, e - s - 1)) break;
          s = e;
        }
        len = s - st;
        t = 0xFF;
        for (k = 0; k < sizeof(kwtok); ++k)
          if (strlen(kwnames[k]) == len && !memcmp(kwnames[k], st, len)) {
            t = kwtok[k];
            break;
          }
        if (t == 0) continue;
        if (t != 0xFF) {
          emit(t);
          last = t;
          continue;
        }
        emit(T_ID);
        emit(intern(st, len));
        last = T_ID;
        continue;
      }
      if (c == '.' && isid0(c1)) {
        const char *e = s + 1;
        while (isid(*e))
          ++e;
        t = method_tok(s + 1, e - s - 1);
        if (!t) fail("UNKNOWN PROPERTY");
        emit(t);
        last = t;
        s = e;
        continue;
      }
      t = 0;
      len = 2;
      switch (c) {
      case '=':
        if (c1 == '=') {
          t = T_EQ;
          if (s[2] == '=') len = 3;
        }
        break;
      case '!':
        if (c1 == '=') {
          t = T_NE;
          if (s[2] == '=') len = 3;
        }
        break;
      case '<':
        if (c1 == '=') t = T_LE;
        else if (c1 == '<') t = T_SHL;
        break;
      case '>':
        if (c1 == '=') t = T_GE;
        else if (c1 == '>') t = T_SHR;
        break;
      case '&':
        if (c1 == '&') t = T_LAND;
        break;
      case '|':
        if (c1 == '|') t = T_LOR;
        break;
      case '+':
        if (c1 == '+') t = T_INC;
        else if (c1 == '=') t = T_ADDEQ;
        break;
      case '-':
        if (c1 == '-') t = T_DEC;
        else if (c1 == '=') t = T_SUBEQ;
        break;
      case '*':
        if (c1 == '=') t = T_MULEQ;
        break;
      case '/':
        if (c1 == '=') t = T_DIVEQ;
        break;
      case '%':
        if (c1 == '=') t = T_MODEQ;
        break;
      }
      if (!t) {
        if (!strchr("(){}[];,+-*/%<>!=?:&|^~", c)) fail("BAD CHARACTER");
        t = c;
        len = 1;
      }
      emit(t);
      last = t;
      s += len;
    }
    if (last && !continues(last)) emit(T_NL);
  }
  emit(T_EOF);
  emit(T_EOF);
  emit(T_EOF);
  tokenizing = 0;
}
