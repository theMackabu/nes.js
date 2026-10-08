unsigned char kpos(unsigned char cell) {
  return 3 + cell * 2;
}

void draw_keyboard_letters(void) {
  unsigned char r, c, b[25];
  for (r = 0; r < 2; ++r) {
    memset(b, 128, 25);
    for (c = 0; c < 13; ++c)
      b[c * 2] = FONT(shift ? keys[r][c] - 32 : keys[r][c]);
    nt_write(0x2000 + (KROW0 + r) * 32 + 3, b, 25, 0);
  }
}
