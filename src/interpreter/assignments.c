void assign_op(struct var *v, unsigned char op) {
  unsigned char lt = v->t, mark0 = ssp;
  int ln = v->n;
  const char *ls = v->s;
  if (lt == V_STR) ls = spush(ls);
  expr();
  a_lt = lt;
  a_ln = ln;
  a_ls = ls;
  APPLY(op);
  ssp = mark0;
  store(v);
}

static const unsigned char opeq[] = {'+', '-', '*', '/', '%'};

unsigned char elem_assign(struct var *v) {
  const unsigned char *s = p;
  unsigned char h, t3;
  int idx, old;
  if (v->t != V_ARR) return 0;
  h = v->n;
  p += 3;
  expr();
  idx = numv();
  expect(']');
  t3 = *p;
  if (t3 == '=' || t3 == T_INC || t3 == T_DEC || (t3 >= T_ADDEQ && t3 <= T_MODEQ)) {
    ++p;
    if (t3 == '=') {
      expr();
      old = numv();
      *arr_slot(h, idx) = old;
    } else if (t3 == T_INC) ++*arr_slot(h, idx);
    else if (t3 == T_DEC) --*arr_slot(h, idx);
    else {
      old = *arr_slot(h, idx);
      expr();
      a_lt = V_NUM;
      a_ln = old;
      a_ls = 0;
      APPLY(opeq[t3 - T_ADDEQ]);
      *arr_slot(h, idx) = numv();
    }
    end_stmt();
    return 1;
  }
  p = s;
  return 0;
}

void id_statement(void) {
  unsigned char id = p[1], t2 = p[2];
  struct var *v;
  if (t2 == '=') {
    p += 3;
    expr();
    v = lookup(id);
    store(v);
    end_stmt();
    return;
  }
  if (t2 == T_INC || t2 == T_DEC) {
    v = lookup_def(id);
    v->n = numval(v->t, v->n, v->s) + (t2 == T_INC ? 1 : -1);
    v->t = V_NUM;
    p += 3;
    end_stmt();
    return;
  }
  if (t2 >= T_ADDEQ && t2 <= T_MODEQ) {
    v = lookup_def(id);
    p += 3;
    assign_op(v, opeq[t2 - T_ADDEQ]);
    end_stmt();
    return;
  }
  if (t2 == '[' && elem_assign(lookup_def(id))) return;
  expr();
  end_stmt();
}
