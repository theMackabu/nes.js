unsigned char line_len(unsigned char i) {
  return loff[i + 1] - loff[i] - 1;
}
char *line_ptr(unsigned char i) {
  return prog + loff[i];
}
unsigned int prog_end(void) {
  return loff[nl];
}

void prog_index(void) {
  unsigned int k = 0;
  nl = 0;
  loff[0] = 0;
  while (prog[k] && nl < MAXL && k < PROGN - 2) {
    if (prog[k] == '\n') loff[++nl] = k + 1;
    ++k;
  }
  if (k && prog[k - 1] != '\n' && nl < MAXL) {
    prog[k++] = '\n';
    loff[++nl] = k;
  }
  if (!nl) {
    prog[0] = '\n';
    nl = 1;
    loff[1] = 1;
  }
  prog[loff[nl]] = 0;
}
