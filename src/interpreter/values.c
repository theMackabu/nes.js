int numval(unsigned char t, int n, const char *s) {
  int r = 0;
  unsigned char neg = 0;
  if (t == V_STR) {
    while (*s == ' ')
      ++s;
    if (*s == '-') {
      neg = 1;
      ++s;
    }
    while (isdig(*s))
      r = r * 10 + (*s++ - '0');
    return neg ? -r : r;
  }
  if (t == V_UNDEF) return 0;
  return n;
}

#define numv() (vt == V_STR ? numval(V_STR, 0, vs) : vn)

unsigned char truthy(void) {
  if (vt == V_STR) return *vs != 0;
  if (vt == V_UNDEF) return 0;
  if (vt == V_ARR) return 1;
  return vn != 0;
}

const char *arr_join(unsigned char h) {
  char *t = tmpbuf(), nb[8];
  struct arr *a = &arrs[h];
  unsigned char i, k = 0, l;
  t[0] = 0;
  for (i = 0; i < a->len; ++i) {
    num_to_str(heap[a->start + i], nb);
    l = strlen(nb);
    if (k + l + (i != 0) > SLEN) break;
    if (i) t[k++] = ',';
    memcpy(t + k, nb, l + 1);
    k += l;
  }
  return t;
}

const char *tostr(unsigned char t, int n, const char *s, char *nb) {
  switch (t) {
  case V_STR:
    return s;
  case V_BOOL:
    return n ? "true" : "false";
  case V_UNDEF:
    return "undefined";
  case V_ARR:
    return arr_join(n);
  }
  return num_to_str(n, nb);
}

void out_value(void) {
  char nb[8];
  if (vt == V_ARR) {
    struct arr *a = &arrs[vn];
    unsigned char i;
    for (i = 0; i < a->len; ++i) {
      if (i) out_char(',');
      out_num(heap[a->start + i]);
    }
    return;
  }
  out_str(tostr(vt, vn, vs, nb));
}
