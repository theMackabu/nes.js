unsigned char num_args(void) {
  unsigned char n = 0;
  int a[3];
  a[0] = a[1] = a[2] = 0;
  expect('(');
  if (*p != ')') {
    for (;;) {
      expr();
      if (n < 3) a[n++] = numv();
      if (*p != ',') break;
      ++p;
    }
  }
  expect(')');
  av[0] = a[0];
  av[1] = a[1];
  av[2] = a[2];
  return n;
}
