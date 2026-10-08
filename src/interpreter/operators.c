void expr(void);
void block(void);
void call(unsigned char id);

static unsigned char a_lt;
static int a_ln;
static const char *a_ls;

void apply(unsigned char op, unsigned char lt, int ln, const char *ls);
#define APPLY(op)                                                                                                      \
  do {                                                                                                                 \
    if (a_lt != V_STR && vt != V_STR) arith(op);                                                                       \
    else apply(op, a_lt, a_ln, a_ls);                                                                                  \
  } while (0)

void arith(unsigned char op) {
  int a = a_ln, b = vn;
  switch (op) {
  case '+':
    vn = a + b;
    break;
  case '-':
    vn = a - b;
    break;
  case '*':
    vn = a * b;
    break;
  case '/':
    if (!b) fail("DIVIDE BY 0");
    vn = a / b;
    break;
  case '%':
    if (!b) fail("DIVIDE BY 0");
    vn = a % b;
    break;
  case '<':
    vt = V_BOOL;
    vn = a < b;
    return;
  case '>':
    vt = V_BOOL;
    vn = a > b;
    return;
  case T_LE:
    vt = V_BOOL;
    vn = a <= b;
    return;
  case T_GE:
    vt = V_BOOL;
    vn = a >= b;
    return;
  case T_EQ:
    vt = V_BOOL;
    vn = a == b;
    return;
  case T_NE:
    vt = V_BOOL;
    vn = a != b;
    return;
  case '&':
    vn = a & b;
    break;
  case '|':
    vn = a | b;
    break;
  case '^':
    vn = a ^ b;
    break;
  case T_SHL:
    vn = a << b;
    break;
  case T_SHR:
    vn = a >> b;
    break;
  }
  vt = V_NUM;
}

void apply(unsigned char op, unsigned char lt, int ln, const char *ls) {
  int a, b;
  static char nb[8], lb[8];
  char *t;
  const char *l2, *r;
  if (op == '+' && (lt == V_STR || vt == V_STR)) {
    t = tmpbuf();
    l2 = tostr(lt, ln, ls, lb);
    strncpy(t, l2, SLEN);
    t[SLEN] = 0;
    r = tostr(vt, vn, vs, nb);
    a = strlen(t);
    strncpy(t + a, r, SLEN - a);
    t[SLEN] = 0;
    vt = V_STR;
    vs = t;
    return;
  }
  if (op == T_EQ || op == T_NE || op == '<' || op == '>' || op == T_LE || op == T_GE) {
    if (lt == V_STR || vt == V_STR) {
      l2 = tostr(lt, ln, ls, lb);
      a = strcmp(l2, tostr(vt, vn, vs, nb));
      b = 0;
    } else {
      a = numval(lt, ln, ls);
      b = numv();
    }
    switch (op) {
    case T_EQ:
      vn = a == b;
      break;
    case T_NE:
      vn = a != b;
      break;
    case '<':
      vn = a < b;
      break;
    case '>':
      vn = a > b;
      break;
    case T_LE:
      vn = a <= b;
      break;
    default:
      vn = a >= b;
      break;
    }
    vt = V_BOOL;
    return;
  }
  a = numval(lt, ln, ls);
  b = numv();
  switch (op) {
  case '+':
    vn = a + b;
    break;
  case '-':
    vn = a - b;
    break;
  case '*':
    vn = a * b;
    break;
  case '/':
    if (!b) fail("DIVIDE BY 0");
    vn = a / b;
    break;
  case '%':
    if (!b) fail("DIVIDE BY 0");
    vn = a % b;
    break;
  case '&':
    vn = a & b;
    break;
  case '|':
    vn = a | b;
    break;
  case '^':
    vn = a ^ b;
    break;
  case T_SHL:
    vn = a << b;
    break;
  case T_SHR:
    vn = a >> b;
    break;
  }
  vt = V_NUM;
}
