struct var *new_local(unsigned char id) {
  struct var *v;
  if (lcount >= MAXLOC) fail("TOO MANY LOCALS");
  lid[lcount] = id;
  v = &lvar[lcount++];
  v->t = V_UNDEF;
  v->n = 0;
  return v;
}

struct var *lookup(unsigned char id) {
  unsigned char i = lcount;
  while (i > fbase) {
    --i;
    if (lid[i] == id) return &lvar[i];
  }
  if (id < NB) fail_name("RESERVED NAME ", id);
  return gptr[id];
}

struct var *lookup_def(unsigned char id) {
  struct var *v = lookup(id);
  if (v->t == V_NONE) fail_name("UNDEFINED ", id);
  return v;
}

void store(struct var *v) {
  v->t = vt;
  if (vt == V_STR) {
    if (vs != v->s) {
      strncpy(v->s, vs, SLEN);
      v->s[SLEN] = 0;
    }
  } else v->n = vt == V_UNDEF ? 0 : vn;
}

void load(struct var *v) {
  vt = v->t;
  vn = v->n;
  vs = v->s;
}

const char *spush(const char *s) {
  unsigned char l = strlen(s) + 1, at = ssp;
  if (ssp + l > sizeof(sstk)) fail("EXPRESSION TOO COMPLEX");
  memcpy(sstk + at, s, l);
  ssp += l;
  return sstk + at;
}
