void index_value(void) {
  unsigned char ct = vt;
  int cn = vn, idx;
  const char *cs = vs;
  char *t;
  if (ct == V_STR) cs = spush(cs);
  ++p;
  expr();
  idx = numv();
  expect(']');
  if (ct == V_ARR) {
    struct arr *a = &arrs[cn];
    if (idx < 0 || idx >= a->len) {
      vt = V_UNDEF;
      vn = 0;
    } else {
      vt = V_NUM;
      vn = heap[a->start + idx];
    }
  } else if (ct == V_STR) {
    if (idx < 0 || idx >= (int)strlen(cs)) {
      vt = V_UNDEF;
      vn = 0;
    } else {
      t = tmpbuf();
      t[0] = cs[idx];
      t[1] = 0;
      vt = V_STR;
      vs = t;
    }
    ssp = cs - sstk;
  } else fail("NOT AN ARRAY");
}

void primary(void) {
  static unsigned char t, id;
  t = *p;
  switch (t) {
  case T_NUM:
    vn = *(const int *)(p + 1);
    vt = V_NUM;
    p += 3;
    break;
  case T_STR:
    vs = (const char *)p + 2;
    vt = V_STR;
    p += p[1] + 3;
    break;
  case T_TRUE:
    vt = V_BOOL;
    vn = 1;
    ++p;
    break;
  case T_FALSE:
    vt = V_BOOL;
    vn = 0;
    ++p;
    break;
  case T_NULL:
    vt = V_UNDEF;
    vn = 0;
    ++p;
    break;
  case '(':
    stack_check();
    ++p;
    expr();
    expect(')');
    break;
  case '[': {
    unsigned char h = arr_new(0);
    ++p;
    skip_nl();
    if (*p != ']') {
      for (;;) {
        expr();
        arr_push(h, numv());
        skip_nl();
        if (*p != ',') break;
        ++p;
        skip_nl();
      }
    }
    expect(']');
    vt = V_ARR;
    vn = h;
    break;
  }
  case T_ID:
    id = p[1];
    p += 2;
    if (*p == '(') {
      call(id);
      break;
    }
    {
      struct var *v = lcount == fbase && id >= NB ? gptr[id] : lookup(id);
      vt = v->t;
      if (vt == V_NONE) fail_name("UNDEFINED ", id);
      vn = v->n;
      vs = v->s;
    }
    break;
  default:
    fail("SYNTAX");
  }
  for (;;) {
    t = *p;
    if (t == '[') index_value();
    else if (t == T_LENGTH) {
      ++p;
      if (vt == V_ARR) vn = arrs[vn].len;
      else if (vt == V_STR) vn = strlen(vs);
      else fail("NO .length");
      vt = V_NUM;
    } else if (t == T_PUSH) {
      unsigned char h = vn;
      if (vt != V_ARR) fail("NOT AN ARRAY");
      ++p;
      expect('(');
      expr();
      arr_push(h, numv());
      expect(')');
      vt = V_NUM;
      vn = arrs[h].len;
    } else if (t == T_POP) {
      struct arr *a;
      if (vt != V_ARR) fail("NOT AN ARRAY");
      a = &arrs[vn];
      ++p;
      expect('(');
      expect(')');
      if (a->len) {
        vn = heap[a->start + --a->len];
        vt = V_NUM;
      } else {
        vt = V_UNDEF;
        vn = 0;
      }
    } else return;
  }
}
