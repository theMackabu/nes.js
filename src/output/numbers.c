char *num_to_str(int n, char *b) {
  char t[6];
  unsigned char i = 0, k = 0;
  unsigned int u;
  if (n < 0) {
    b[k++] = '-';
    u = -n;
  } else u = n;
  do {
    t[i++] = '0' + u % 10;
    u /= 10;
  } while (u);
  while (i)
    b[k++] = t[--i];
  b[k] = 0;
  return b;
}

void out_num(int n) {
  char b[8];
  out_str(num_to_str(n, b));
}
